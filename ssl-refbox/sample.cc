#include "sample.h"
#include <limits>
#include <libbsmart/math.h>
#include <libbsmart/field.h>
#include <iostream>
#include <cassert>

Sample::Sample() {
	weighting = 0.0000001;
	timestamp = 0.;
	age = 0;
}

//Polar-Methode
void Sample::fuettere_polarbaer(BSmart::Double_Vector* polarbaer) {
	double q = 0.;
	double a1;
	double a2;

	while (q <= 0. || q > 1) {
		a1 = (2. * rand() / ((double) RAND_MAX)) - 1.;
		a2 = (2. * rand() / ((double) RAND_MAX)) - 1.;
		q = a1 * a1 + a2 * a2;
	}
	double p = sqrt((-2) * log(q) / q);
	polarbaer->x = a1 * p;
	polarbaer->y = a2 * p;
}

const QString Sample::status_names[STATUS_NUM] = { "UNKNOWN", "ROLLING",
		"CHIPPED", "FLYING", "KICKED", "BOUNCED", "LYING" };
const QString Sample::last_touched_names[LAST_TOUCHED_NUM] = { "UNKNOWN",
		"YELLOW", "BLUE", "REFEREE" };
const double Ball_Sample::ball_noise = 2.; // mm
const double Ball_Sample::ball_speed_noise = 0.02; // m/s
const double Robot_Sample::robot_noise = 5.; // mm
const double Robot_Sample::robot_speed_noise = 0.1; // m/s

Ball_Sample::Ball_Sample() :
	Sample(), pos(0., 0., 0.), speed(0., 0., 0.), last_touched_robot(-1, -1),
			ball_line(0., 0., 0., 0.), polarbaer(0., 0.) {
	status = STATUS_UNKNOWN;
	weighting = 0.0000001;
	type = BALL;
	last_touched = TOUCH_UNKNOWN;
	timestamp = 0.;
	age = 0;
}

Ball_Sample::Ball_Sample(BSmart::Pose3D pos_, BSmart::Pose3D speed_,
		Status status_, Last_Touched last_touched_) :
	Sample(), pos(pos_), speed(speed_), last_touched_robot(-1, -1),
			ball_line(0., 0., 0., 0.), polarbaer(0., 0.) {
	status = status_;
	weighting = 0.0000001;
	type = BALL;
	last_touched = last_touched_;
	timestamp = 0.;
	age = 0;
}

Ball_Sample::Ball_Sample(const Ball_Sample& other) :
	Sample(), ball_line(0., 0., 0., 0.), polarbaer(0., 0.) {
	status = other.status;
	weighting = other.weighting;
	type = other.type;
	pos = other.pos;
	speed = other.speed;
	last_pos = other.last_pos;
	last_touched = other.last_touched;
	timestamp = other.timestamp;
	age = other.age;
	last_touched_robot.x = other.last_touched_robot.x;
	last_touched_robot.y = other.last_touched_robot.y;
}

Ball_Sample::~Ball_Sample() {
	;
}

void Ball_Sample::move(const double ms,
		const Robot_Sample_List& robot_obstacles) {
	switch (status) {
	case KICKED:
	case BOUNCED:
		status = ROLLING;
		break;
	case CHIPPED:
		status = FLYING;
		break;
	default:
		break;
	}

	last_pos = pos;
	pos += (speed * ms);

	//noise auf Position
	//je höher die Geschwindigkeit desto stärker das Rauschen
	double factor = 0.5;
	//factor should be between 0.5 and 1.5, 10 m/s maximum speed
	factor += 0.1 * speed.length();

	fuettere_polarbaer(&polarbaer);
	pos.x += polarbaer.x * ball_noise * factor;
	pos.y += polarbaer.y * ball_noise * factor;

	speed *= pow(0.9999, ms);

	fuettere_polarbaer(&polarbaer);
	speed.x += polarbaer.x * ball_speed_noise * factor;
	speed.y += polarbaer.y * ball_speed_noise * factor;

	const double g = 9.80665;

	//Schwerkraft
	if (fabs(speed.z) > 1. || pos.z > 10.)
		speed.z -= (g * ms);
	else
		speed.z = 0.;

	fuettere_polarbaer(&polarbaer);
	if (speed.z != 0.) {
		speed.z += polarbaer.x * ball_speed_noise * factor;
	}

	check_collisions(robot_obstacles, ms);
}

void Ball_Sample::check_collisions(const Robot_Sample_List& robot_obstacles,
		double ms) {
	Hitpoint* hitpoint = new Hitpoint();
	int cnt = 0;

	random = rand() / (double) RAND_MAX;

	//while collision
	bool intersect;
	do {
		hitpoint->dist = std::numeric_limits<double>::max();
		intersect = false;
		//check for collision
		intersect = intersect || check_robot_reflections(hitpoint,
				robot_obstacles);
		intersect = intersect || check_floor_reflection(hitpoint);
		intersect = intersect || check_goalpost_reflections(hitpoint);
		intersect = intersect || check_bar_reflections(hitpoint);
		intersect = intersect && (cnt < 5);

		if (intersect) {
			cnt++;

			collision3D.x = hitpoint->collisionpoint.x;
			collision3D.y = hitpoint->collisionpoint.y;

			const double dist_total_2D = pos.distance_to_2D(last_pos);
			const double dist_to_collision = last_pos.distance_to_2D(
					collision3D);
			const double quot1 = dist_to_collision / dist_total_2D;
			collision3D.z = last_pos.z + quot1 * (pos.z - last_pos.z);

			if (collision3D.z < hitpoint->height) {
				switch (hitpoint->obstacle) {
				case Field_Hardware::NO_OBSTACLE:
					//                        std::cout << "collision without obstacle - please check" << std::endl;
					break;
				case Field_Hardware::BAR_HORIZONTAL:
					pos.y = collision3D.y + (collision3D.y - pos.y);
					last_pos = collision3D;
					speed.x *= 0.6;
					speed.y *= -0.6;
					status = BOUNCED;
					break;
				case Field_Hardware::BAR_VERTICAL:
					pos.x = collision3D.x + (collision3D.x - pos.x);
					last_pos = collision3D;
					speed.x *= -0.6;
					speed.y *= 0.6;
					status = BOUNCED;
					break;
				case Field_Hardware::FLOOR:
					pos.z = abs(pos.z);
					last_pos = collision3D;
					speed.z = abs(speed.z) * 0.5;
					status = FLYING;
					break;
				case Field_Hardware::ROBOT_BLUE:
					last_touched = BLUE;
					last_touched_robot.x = 1;
				case Field_Hardware::ROBOT_YELLOW:
					if (hitpoint->obstacle == Field_Hardware::ROBOT_YELLOW) {
						last_touched = YELLOW;
						last_touched_robot.x = 0;
					}
					last_touched_robot.y = hitpoint->robot_id;

					status = BOUNCED;
					//shot or chipped?
					if (collision3D.z < 42) {
						//shot
						if (random < 0.1) {
							status = KICKED;
							random = rand() / (double) RAND_MAX;
						}
						//chipped
						else if (random < 0.15) {
							status = CHIPPED;
							random = rand() / (double) RAND_MAX;
						}
					}
					//reflection for robots and goalpost
				case Field_Hardware::GOALPOST:
					//                    if(cnt==0)
				{
					BSmart::Double_Vector normal(
							hitpoint->collisionpoint
									- hitpoint->orientation_point);
					last_pos = collision3D;

					switch (status) {
					case CHIPPED:
						speed.z = random * 6.;
						random = rand() / (double) RAND_MAX;
						//speed.z = gaussian(4,2);
					case KICKED:
						normal.normalize(10. * random);
						random = rand() / (double) RAND_MAX;
						//normal.normalize(gaussian(5,5));
						speed.x = normal.x;
						speed.y = normal.y;
						break;
					default:
						status = BOUNCED;
						const double angle_normal = normal.angle();
						BSmart::Double_Vector ball_speed(speed.x, speed.y);
						ball_speed.rotate(BSmart::pi);
						const double angle_ball = ball_speed.angle();

						ball_speed.rotate(2 * (angle_ball - angle_normal));

						speed.x = ball_speed.x * 0.6;
						speed.y = ball_speed.y * 0.6;
						//speed.z unchanged

						const double dist_after_collision = dist_total_2D
								- dist_to_collision;
						ball_speed.normalize(dist_after_collision);

						break;
					}

					pos.x = last_pos.x + speed.x * ms;
					pos.y = last_pos.y + speed.y * ms;
					pos.z = last_pos.z + speed.z * ms;

				}
					break;
				}
			}

			else if (pos.z < hitpoint->height) {
				pos.z = hitpoint->height;
			}
		}
	} while (intersect);

	delete hitpoint;
}

bool Ball_Sample::check_bar_reflections(Hitpoint* hitpoint) {
	ball_line.p1.x = last_pos.x;
	ball_line.p1.y = last_pos.y;
	ball_line.p2.x = pos.x; //2d
	ball_line.p2.y = pos.y; //2d

	for (int i = 0; i < Field_Hardware::NUMBER_FIELD_BARS; ++i) {
		//        if(test_intersection_2(ball_line, Field_Hardware::field_bars_ball[i].line))
		{
			if (test_intersection(ball_line,
					Field_Hardware::field_bars_ball[i].line)) {
				if (intersection_point(ball_line,
						Field_Hardware::field_bars_ball[i].line, &intersection)) {
					//reflection?
					if (Field_Hardware::field_bars_ball[i].point_a.distance_to(
							intersection)
							< Field_Hardware::field_bars_ball[i].dist && //2d
							Field_Hardware::field_bars_ball[i].point_b.distance_to(
									intersection)
									< Field_Hardware::field_bars_ball[i].dist && //2d
							last_pos.distance_to_2D(intersection)
									< last_pos.distance_to_2D(pos) && //2d
							pos.distance_to_2D(intersection)
									< last_pos.distance_to_2D(pos)) //2d
					{
						if (last_pos.distance_to_2D(intersection)
								< hitpoint->dist) //2d
						{
							hitpoint->dist = last_pos.distance_to_2D(
									intersection); //2d
							hitpoint->collisionpoint = intersection;
							hitpoint->obstacle
									= Field_Hardware::field_bars_ball[i].obstacle;
							hitpoint->orientation_point
									= Field_Hardware::field_bars_ball[i].point_a;
							hitpoint->height
									= Field_Hardware::field_bars_ball[i].height;
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

bool Ball_Sample::check_floor_reflection(Hitpoint* hitpoint) {
	if (pos.z < 0.) {
		if (last_pos.z < 0.)
			last_pos.z = abs(last_pos.z);

		//        double z_div = abs(pos.z - last_pos.z);
		const double quot = last_pos.z / abs(pos.z - last_pos.z);
		const double dist = pos.distance_to_2D(last_pos) * quot;

		if (dist < hitpoint->dist) {
			hitpoint->dist = dist;
			hitpoint->collisionpoint = BSmart::Double_Vector(
					last_pos.x + quot * (pos.x - last_pos.x),
					last_pos.y + quot * (pos.y - last_pos.y));
			hitpoint->obstacle = Field_Hardware::FLOOR;
			hitpoint->height = 0;
		}
		return true;
	}

	return false;
}

bool Ball_Sample::check_goalpost_reflections(Hitpoint* hitpoint) {
	ball_line.p1.x = last_pos.x;
	ball_line.p1.y = last_pos.y;
	ball_line.p2.x = pos.x; //2d
	ball_line.p2.y = pos.y; //2d
	intersections.clear();
	bool intersect = false;

	for (int i = 0; i < Field_Hardware::NUMBER_FIELD_GOALPOSTS; ++i) {
		if (test_intersection(Field_Hardware::field_goalposts_ball[i].circle,
				ball_line)) {
			if (intersection_point(
					Field_Hardware::field_goalposts_ball[i].circle, ball_line,
					intersections)) {
				//                std::cout << "intersections.size(): " << intersections.size() << std::endl;
				for (unsigned int j = 0; j < intersections.size(); ++j) {
					//                    std::cout << "intersections["<<j<<"]: " << intersections[j] << std::endl;
					if (pos.distance_to_2D(intersections[j])
							< last_pos.distance_to_2D(pos)
							&& last_pos.distance_to_2D(intersections[j])
									< last_pos.distance_to_2D(pos)) {
						if (last_pos.distance_to_2D(intersections[j])
								< hitpoint->dist) {
							hitpoint->dist = last_pos.distance_to_2D(
									intersections[j]);
							hitpoint->collisionpoint = intersections[j];
							hitpoint->obstacle
									= Field_Hardware::field_goalposts_ball[i].obstacle;
							hitpoint->orientation_point
									= Field_Hardware::field_goalposts_ball[i].center;
							hitpoint->height
									= Field_Hardware::field_goalposts_ball[i].height;
							intersect = true;
						}
					}
				}
			}
		}
	}
	return intersect;
}

bool Ball_Sample::check_robot_reflections(Hitpoint* hitpoint,
		const Robot_Sample_List& robot_obstacles) {
	ball_line.p1.x = last_pos.x;
	ball_line.p1.y = last_pos.y;
	ball_line.p2.x = pos.x; //2d
	ball_line.p2.y = pos.y; //2d
	intersections.clear();
	bool intersect = false;

	Robot_Sample_List::const_iterator it_obstacles = robot_obstacles.begin();
	const Robot_Sample_List::const_iterator it_obstacles_end =
			robot_obstacles.end();
	circle.radius = BSmart::Field::robot_radius + BSmart::Field::ball_radius;
	random = rand() / (double) RAND_MAX;

	while (it_obstacles != it_obstacles_end) {
		circle.x = it_obstacles->pos.x;
		circle.y = it_obstacles->pos.y;

		if (test_intersection(circle, ball_line)) {
			if (intersection_point(circle, ball_line, intersections)) {
				for (unsigned int j = 0; j < intersections.size(); ++j) {
					if (pos.distance_to_2D(intersections[j])
							< last_pos.distance_to_2D(pos)
							&& last_pos.distance_to_2D(intersections[j])
									< last_pos.distance_to_2D(pos)) {
						if (last_pos.distance_to_2D(intersections[j])
								< hitpoint->dist) {
							hitpoint->dist = last_pos.distance_to_2D(
									intersections[j]);
							hitpoint->collisionpoint = intersections[j];
							if (it_obstacles->team) {
								hitpoint->obstacle = Field_Hardware::ROBOT_BLUE;
							} else {
								hitpoint->obstacle
										= Field_Hardware::ROBOT_YELLOW;
							}

							hitpoint->robot_id = it_obstacles->id;
							hitpoint->orientation_point = it_obstacles->pos;
							hitpoint->height = BSmart::Field::robot_height;
							intersect = true;
						}
					}
				}
			}
		} else if (pos.distance_to_2D(it_obstacles->pos)
				< (BSmart::Field::ball_radius + BSmart::Field::robot_radius
						+ 40)) {
			if (pos.distance_to_2D(it_obstacles->pos) < hitpoint->dist) {
				if (random < 0.3)//30% Chance zu kollidieren wenn er nah dran vorbei rollt oder nah dran liegt
				{
					hitpoint->dist = pos.distance_to_2D(it_obstacles->pos);
					hitpoint->collisionpoint = BSmart::Double_Vector(pos.x,
							pos.y);
					if (it_obstacles->team)
						hitpoint->obstacle = Field_Hardware::ROBOT_BLUE;
					else
						hitpoint->obstacle = Field_Hardware::ROBOT_YELLOW;

					hitpoint->robot_id = it_obstacles->id;
					hitpoint->orientation_point = it_obstacles->pos;
					hitpoint->height = BSmart::Field::robot_height;
					intersect = true;
				}
			}
		}

		it_obstacles++;
	}

	return intersect;
}

Robot_Sample::Robot_Sample() :
	pos(0., 0., 0.), speed(0., 0., 0.), polarbaer(0., 0.) {
	weighting = 0.0000001;
	status = STATUS_UNKNOWN;
	type = ROBOT;
	timestamp = 0.;
	double factor = 0.5;
	//factor should be between 0.5 and 1.5, 10 m/s maximum speed
	factor += 0.1 * speed.length();

	confidence = 0.;
	team = -1;
	id = -1;
	age = 0;
}

Robot_Sample::Robot_Sample(BSmart::Pose pos_, BSmart::Pose speed_) :
	pos(pos_), speed(speed_), polarbaer(0., 0.) {
	weighting = 0.0000001;
	status = STATUS_UNKNOWN;
	type = ROBOT;
	timestamp = 0.;
	confidence = 0.;
	team = -1;
	id = -1;
	age = 0;
}

Robot_Sample::Robot_Sample(const Robot_Sample& other) :
	polarbaer(0., 0.) {
	pos = other.pos;
	speed = other.speed;
	confidence = other.confidence;
	weighting = other.weighting;
	status = other.status;
	type = other.type;
	timestamp = other.timestamp;
	confidence = other.confidence;
	team = other.team;
	id = other.id;
	age = other.age;
}

Robot_Sample::~Robot_Sample() {
	;
}

void Robot_Sample::move(const double ms,
		const Robot_Sample_List& robot_obstacles) {
	last_pos = pos;
	pos += (speed * ms);

	double factor = 0.5;
	//factor should be between 0.5 and 1.5, 10 m/s maximum speed
	factor += 0.1 * speed.length();

	fuettere_polarbaer(&polarbaer);
	//noise auf Position
	pos.x += polarbaer.x * robot_noise * factor;
	pos.y += polarbaer.y * robot_noise * factor;

	fuettere_polarbaer(&polarbaer);
	//Geschwindigkeit wird nicht reduziert wegen des Antriebs der Roboter
	speed.x += polarbaer.x * robot_speed_noise * factor;
	speed.y += polarbaer.y * robot_speed_noise * factor;
	check_collisions(robot_obstacles);
}

void Robot_Sample::check_collisions(const Robot_Sample_List& robot_obstacles) {
	Hitpoint* hitpoint = new Hitpoint();
	int cnt = 0;

	//while collision
	bool intersect;
	do {
		hitpoint->dist = std::numeric_limits<double>::max();
		intersect = false;
		//check for collision
		intersect = intersect || check_bar_reflections(hitpoint);
		intersect = intersect || check_goalpost_reflections(hitpoint);
		intersect = intersect || check_robot_reflections(hitpoint,
				robot_obstacles);
		intersect = intersect && (cnt < 5);
		cnt++;

		if (intersect) {

			switch (hitpoint->obstacle) {
			case Field_Hardware::NO_OBSTACLE:
				std::cout << "collision without obstacle - please check"
						<< std::endl;
				break;
			case Field_Hardware::BAR_HORIZONTAL:
				pos.y = hitpoint->collisionpoint.y
						+ (hitpoint->collisionpoint.y - pos.y);
				last_pos = hitpoint->collisionpoint;
				speed.y *= -1;//-0.8;
				break;
			case Field_Hardware::BAR_VERTICAL:
				pos.x = hitpoint->collisionpoint.x
						+ (hitpoint->collisionpoint.x - pos.x);
				last_pos = hitpoint->collisionpoint;
				speed.x *= -1;//-0.8;
				break;
			case Field_Hardware::FLOOR:
				std::cout
						<< "sample.cc Robot_Sample::check_collisions FLOOR: This should not happen."
						<< std::endl;
				break;
			case Field_Hardware::GOALPOST:
			case Field_Hardware::ROBOT_BLUE:
			case Field_Hardware::ROBOT_YELLOW:
				if (cnt == 0) {
					BSmart::Double_Vector normal(
							hitpoint->collisionpoint
									- hitpoint->orientation_point);
					double angle_normal = normal.angle();
					BSmart::Double_Vector robot_speed(speed.x, speed.y);
					robot_speed.rotate(BSmart::pi);
					double angle_ball = robot_speed.angle();
					/*
					 std::cout << "Ball has hit a Goalpost at " << hitpoint->collisionpoint << " with center at " << hitpoint->orientation_point << std::endl;
					 std::cout << "angle_normal: " << angle_normal << "   angle_ball: " << angle_ball << std::endl;
					 std::cout << "ball_speed vorher  : " << ball_speed << std::endl;
					 */
					robot_speed.rotate(2 * (angle_ball - angle_normal));
					//                    std::cout << "robot_speed nachher : " << robot_speed << std::endl;
					last_pos = hitpoint->collisionpoint;

					pos = hitpoint->collisionpoint + robot_speed * 0.01;
					speed.x = robot_speed.x;
					speed.y = robot_speed.y;
				}
				break;
			}
		}
	} while (intersect);

	delete hitpoint;
}

bool Robot_Sample::check_bar_reflections(Hitpoint* hitpoint) {
	robot_line.p1 = last_pos;
	robot_line.p2 = pos;

	for (int i = 0; i < Field_Hardware::NUMBER_FIELD_BARS; ++i) {
		//        std::cout << "i: " << i << std::endl;
		//        if(test_intersection_2(robot_line, Field_Hardware::field_bars_robot[i].line))
		{
			if (test_intersection(robot_line,
					Field_Hardware::field_bars_robot[i].line)) {
				if (intersection_point(robot_line,
						Field_Hardware::field_bars_robot[i].line, &intersection)) {
					//reflection?
					if (Field_Hardware::field_bars_robot[i].point_a.distance_to(
							intersection)
							< Field_Hardware::field_bars_robot[i].dist
							&& Field_Hardware::field_bars_robot[i].point_b.distance_to(
									intersection)
									< Field_Hardware::field_bars_robot[i].dist
							&& last_pos.distance_to(intersection)
									< last_pos.distance_to(pos)
							&& pos.distance_to(intersection)
									< last_pos.distance_to(pos)) {
						if (last_pos.distance_to(intersection) < hitpoint->dist) {
							hitpoint->dist = last_pos.distance_to(intersection);
							hitpoint->collisionpoint = intersection;
							hitpoint->obstacle
									= Field_Hardware::field_bars_robot[i].obstacle;
							hitpoint->orientation_point
									= Field_Hardware::field_bars_robot[i].point_a;
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

bool Robot_Sample::check_goalpost_reflections(Hitpoint* hitpoint) {
	robot_line.p1 = last_pos;
	robot_line.p2 = pos;
	intersections.clear();

	for (int i = 0; i < Field_Hardware::NUMBER_FIELD_GOALPOSTS; ++i) {
		if (test_intersection(Field_Hardware::field_goalposts_robot[i].circle,
				robot_line)) {
			if (BSmart::intersection_point(
					Field_Hardware::field_goalposts_robot[i].circle,
					robot_line, intersections)) {
				//                std::cout << "intersections.size(): " << intersections.size() << std::endl;
				for (unsigned int j = 0; j < intersections.size(); ++j) {
					//                    std::cout << "intersections["<<j<<"]: " << intersections[j] << std::endl;
					if (pos.distance_to(intersections[j])
							< last_pos.distance_to(pos)
							&& last_pos.distance_to(intersections[j])
									< last_pos.distance_to(pos)) {
						if (last_pos.distance_to(intersections[j])
								< hitpoint->dist) {
							hitpoint->dist = last_pos.distance_to(
									intersections[j]);
							hitpoint->collisionpoint = intersections[j];
							hitpoint->obstacle
									= Field_Hardware::field_goalposts_robot[i].obstacle;
							hitpoint->orientation_point
									= Field_Hardware::field_goalposts_robot[i].center;
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

bool Robot_Sample::check_robot_reflections(Hitpoint* hitpoint,
		const Robot_Sample_List& robot_obstacles) {
	robot_line.p1 = last_pos;
	robot_line.p2 = pos;
	intersections.clear();
	bool intersect = false;

	//    int num_bots = robot_obstacles.size();

	Robot_Sample_List::const_iterator it_obstacles = robot_obstacles.begin();
	const Robot_Sample_List::const_iterator it_obstacles_end =
			robot_obstacles.end();

	circle.radius = 2 * BSmart::Field::robot_radius;

	while (it_obstacles != it_obstacles_end) {
		if (!((it_obstacles->team == this->team) && (it_obstacles->id
				== this->id))) {
			//            std::cout << "crr: it_obstacles->team: " << it_obstacles->team << "  it_obstacles->id: " << it_obstacles->id << std::endl;
			//            std::cout << "crr:               team: " << this->team         << "                id: " << this->id << std::endl;

			circle.x = it_obstacles->pos.x;
			circle.y = it_obstacles->pos.y;

			if (test_intersection(circle, robot_line)) {
				if (intersection_point(circle, robot_line, intersections)) {
					//                    std::cout << "(" << team << "|" << id << "):" << " intersections.size(): " << intersections.size() << std::endl;
					for (unsigned int j = 0; j < intersections.size(); ++j) {
						//                        std::cout << "intersections["<<j<<"]: " << intersections[j] << std::endl;
						if (pos.distance_to(intersections[j])
								< last_pos.distance_to(pos)
								&& last_pos.distance_to(intersections[j])
										< last_pos.distance_to(pos)) {
							if (last_pos.distance_to(intersections[j])
									< hitpoint->dist) {
								hitpoint->dist = last_pos.distance_to(
										intersections[j]);
								hitpoint->collisionpoint = intersections[j];
								if (it_obstacles->team)
									hitpoint->obstacle
											= Field_Hardware::ROBOT_BLUE;
								else
									hitpoint->obstacle
											= Field_Hardware::ROBOT_YELLOW;

								hitpoint->orientation_point = it_obstacles->pos;
								intersect = true;
							}
						}
					}
				}
			}
		}

		it_obstacles++;
	}

	return intersect;
}
