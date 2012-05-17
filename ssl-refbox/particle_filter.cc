#include "particle_filter.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <libbsmart/math.h>
#include <cmath>
#include <QWaitCondition>
#include <algorithm>
#include <stdio.h>

Particle_Filter_Mother::Particle_Filter_Mother(Pre_Filter_Data* pf_data_, Filter_Data* filter_data_,
		QWaitCondition* rules_wait_condition_, QWaitCondition* new_data_wait_condition_) :
		filter_data(filter_data_) {
	pf = new Particle_Filter(pf_data_, filter_data_);
	new_data = false;
	rules_wait_condition = rules_wait_condition_;
	new_data_wait_condition = new_data_wait_condition_;
	connectActions();
}

Particle_Filter_Mother::~Particle_Filter_Mother() {
	;
}

void Particle_Filter_Mother::connectActions() {
	connect(this->pf, SIGNAL ( change_ball_status_intern ( QString ) ), this,
			SIGNAL ( change_ball_status ( QString ) ));
	connect(this->pf, SIGNAL ( change_ball_last_touched_intern ( QString ) ), this,
			SIGNAL ( change_ball_last_touched ( QString ) ));
}

void Particle_Filter_Mother::run() {

	QMutex wait_for_data_mutex;

	for (;;) {
		// will be notified by sslvision.cc execute
		wait_for_data_mutex.lock();
		new_data_wait_condition->wait(&wait_for_data_mutex);

		new_data = false;
		pf->motion_update();
		pf->sensor_update();
		pf->resample();
		pf->create_models();
		rules_wait_condition->wakeAll();

		wait_for_data_mutex.unlock();
	}
}

void Particle_Filter_Mother::new_frame() //SLOT
{
	new_data = true;
}

Particle_Filter::Particle_Filter(Pre_Filter_Data* pf_data_, Filter_Data* filter_data_) :
		pf_data(pf_data_), filter_data(filter_data_) {
	srand((unsigned) time(NULL));

	Ball_Sample_List new_balls(Filter_Data::BALL_SAMPLES, random_ball_sample());

	filter_data->set_ball_samples(new_balls);

	Robot_Sample new_robot = random_robot_sample();
	new_robot.confidence = 0.;
	for (int team = 0; team < Filter_Data::NUMBER_OF_TEAMS; ++team) {
		for (int id = 0; id < Filter_Data::NUMBER_OF_IDS; ++id) {
			new_robot.team = team;
			new_robot.id = id;
			Robot_Sample_List new_robots(Filter_Data::ROBOT_SAMPLES, new_robot);
			filter_data->set_robot_samples(team, id, new_robots);
		}
	}

	last_movement = BSmart::Systemcall::get_timef();
	ball_distance_threshold = 500;
	std_dev_ball = 5;
	std_dev_robot = 5;

	alpha_slow_ball = 0.4; //0.4
	alpha_fast_ball = 0.8; //0.8
	o_slow_ball = 0.2;
	o_fast_ball = 0.5;

	alpha_slow_robots = 0.2;
	alpha_fast_robots = 0.5;
	for (int team = 0; team < Filter_Data::NUMBER_OF_TEAMS; ++team) {
		for (int id = 0; id < Filter_Data::NUMBER_OF_IDS; ++id) {
			o_slow_robots[team][id] = 0.;
			o_fast_robots[team][id] = 0.;
		}
	}

	ball_last_touched_saved = 0;
	ball_status_saved = 0;
	ball_direction_before = BSmart::Pose(0., 0., 0.);
	ball_direction_after = BSmart::Pose(0., 0., 0.);
	last_touched_robot_saved = BSmart::Int_Vector(0, 0);

	circle.radius = BSmart::Field::robot_radius + BSmart::Field::ball_radius;

	last_ball_model = Ball_Sample();
	ball_lying_counter = 0;
	newest_frame = 0;
	timestamp = 0;
	last_contacts.clear();
	flying_robots.clear();
	all_contacts_log.clear();
}

Particle_Filter::~Particle_Filter() {
	;
}

void Particle_Filter::motion_update() {
	double time_diff = BSmart::Systemcall::get_time_sincef(last_movement);
	last_movement = BSmart::Systemcall::get_timef();

	robot_obstacles.clear();
	robot_obstacles = filter_data->get_current_robot_obstacles();

	filter_data->move_robots(time_diff, robot_obstacles);
	filter_data->move_balls(time_diff, robot_obstacles);

}

void Particle_Filter::sensor_update() {
	newest_frame = pf_data->get_newest_frame();
	timestamp = pf_data->get_timestamp();

	//ball
	Ball_Percept_List cur_balls;
	cur_balls.clear();

	//robots
	Robot_Percept_List robots;

	Robot_Percept_List cur_robots[Filter_Data::NUMBER_OF_TEAMS][Filter_Data::NUMBER_OF_IDS];
	for (int team = 0; team < Filter_Data::NUMBER_OF_TEAMS; ++team) {
		for (int id = 0; id < Filter_Data::NUMBER_OF_IDS; ++id) {
			cur_robots[team][id].clear();
		}
	}

	filter_data->reduce_visibility();

	ball_direction_before = pf_data->get_ball_direction_before();
	ball_direction_after = pf_data->get_ball_direction_after();

	for (int cam = 0; cam < 2; ++cam) {
		//ball
		Ball_Percept_List balls = pf_data->get_current_balls(cam);
		Ball_Percept_List::iterator iter_ball = balls.begin();
		Ball_Percept_List::iterator iter_ball_end = balls.end();

		while (iter_ball != iter_ball_end) {
			//only if percept is good
			if (iter_ball->confidence > 0) {
				if (weight_ball(*iter_ball)) {
					;
				}
				cur_balls.push_back(*iter_ball);
			}
			iter_ball++;
		}

		//robots
		for (int team = 0; team < Filter_Data::NUMBER_OF_TEAMS; ++team) {
			for (int id = 0; id < Filter_Data::NUMBER_OF_IDS; ++id) {
				robots = pf_data->get_robots(cam, team, id);
				std::vector<Robot_Percept>::iterator iter_robot = robots.begin();
				std::vector<Robot_Percept>::iterator iter_robot_end = robots.end();
				while (iter_robot != iter_robot_end) {
					//only if percept is good
					if (iter_robot->confidence > 0) {
						if (weight_robot(*iter_robot, team, id)) {
							filter_data->set_robot_seen(team, id);
						}
						cur_robots[team][id].push_back(*iter_robot);
					}

					++iter_robot;
				}
				robots.clear();
			}
		}
	}

	//set new percepts, first balls...
	filter_data->set_current_ball_percepts(cur_balls);
	//then robots
	for (int team = 0; team < Filter_Data::NUMBER_OF_TEAMS; ++team) {
		for (int id = 0; id < Filter_Data::NUMBER_OF_IDS; ++id) {
			filter_data->set_current_robot_percepts(team, id, cur_robots[team][id]);
		}
	}
}

bool Particle_Filter::weight_ball(Ball_Percept& perc) {
	bool percept_used = false;
	BSmart::Pose percept(perc.x, perc.y);
	Ball_Sample_List samples = filter_data->get_ball_samples();

	for (Ball_Sample_List::iterator it = samples.begin(); it != samples.end(); it++) {
		if (std::isnan(it->pos.x)) {
			std::cout << "weight_ball isnan: it->pos.x: " << it->pos.x << std::endl;
		}
		double dist = dist_in_2d(perc.cam, it->pos, percept);
		//        if(dist < ball_distance_threshold)
		{
			double newWeighting = gaussian(dist, std_dev_ball);

			if (newWeighting < 0.000000001)
				newWeighting = 0.000000001;

			it->weighting = newWeighting;
			percept_used = true;
		}
	}

	filter_data->set_ball_samples(samples);
	return percept_used;
}

bool Particle_Filter::weight_robot(Robot_Percept& perc, int team, int id) {
	bool percept_used = false;
	BSmart::Pose percept(perc.x, perc.y, perc.rotation);
	Robot_Sample_List samples = filter_data->get_robot_samples(team, id);

	for (Robot_Sample_List::iterator it = samples.begin(); it != samples.end(); it++) {
		double dist = it->pos.distance_to(percept);
//		if (dist < robots_distance_threshold) {
		double newWeighting = gaussian(dist, std_dev_robot);

		if (newWeighting < 0.000000000001)
			newWeighting = 0.000000000001;

		it->weighting = newWeighting;
		percept_used = true;
//		}
	}

	filter_data->set_robot_samples(team, id, samples);
	return percept_used;
}

void Particle_Filter::resample() {

	double average_weight = 0.;
	double total_weight = 0.;
	double best_weight = 0.;
	double augment = 0.;
	double random = 0.;

	//balls
	Ball_Percept_List balls = filter_data->get_current_ball_percepts();
	int num_balls = balls.size();

	if (num_balls > 0) {
		ball_samples_old.clear();
		ball_samples_new.clear();
		ball_samples_old = filter_data->get_ball_samples();

		//ball
		//calculate total weight
		total_weight = 0.;
		best_weight = 0.;
		for (Ball_Sample_List::iterator it = ball_samples_old.begin(); it != ball_samples_old.end(); ++it) {
			if (it->weighting > best_weight)
				best_weight = it->weighting;
			total_weight += it->weighting;
			it->age++;
		}

		if (std::isnan(total_weight)) {
			total_weight = 0;
			std::cout << "total_weight is not a number - please check!" << std::endl;
		}

		average_weight = total_weight / Filter_Data::BALL_SAMPLES;

		o_slow_ball += alpha_slow_ball * (average_weight - o_slow_ball);

		if (o_slow_ball < 0.000000001)
			o_slow_ball = 0.000000001;

		o_fast_ball += alpha_fast_ball * (average_weight - o_fast_ball);

		//augmented part
		Ball_Sample new_ball;
		augment = std::max(0., 1. - (o_fast_ball / o_slow_ball));
		random = 0.;

		//speed heuristic for new ball samples
		BSmart::Pose3D ball_speed(pf_data->get_ball_direction_after().x, pf_data->get_ball_direction_after().y, 0.);

		double random_derivation;
		int augment_ball_counter = 0;

		for (int i = 0; i < Filter_Data::BALL_SAMPLES; ++i) {
			random = (double) rand() / (double) RAND_MAX;
			if (random < augment) { // insert new samples
				//only when there are percepts. no random samples
				int ball_percept = random_number(0, (num_balls - 1));
				new_ball.pos = BSmart::Pose3D(balls[ball_percept].x, balls[ball_percept].y, 0.);
				new_ball.speed = ball_speed;

				new_ball.status = Sample::STATUS_UNKNOWN;
				new_ball.age = 0;
				augment_ball_counter++;
			} else { //draw from derivation
				random_derivation = (double) rand() / (double) RAND_MAX * total_weight;

				double cnt = 0.;
				int j = 0;
				while (cnt < random_derivation) {
					cnt += ball_samples_old[j].weighting;
					j++;
				}
				if (j == 0)
					++j;

				new_ball = ball_samples_old[j - 1];
			}
			ball_samples_new.push_back(new_ball);
		}

		//copy new samples
		filter_data->set_ball_samples(ball_samples_new);
	}

	//speed Heuristic for robot samples
	BSmart::Pose robot_speed(0., 0.);

	//robots
	for (int team = 0; team < Filter_Data::NUMBER_OF_TEAMS; ++team) {
		for (int id = 0; id < Filter_Data::NUMBER_OF_IDS; ++id) {
			Robot_Percept_List robots = filter_data->get_current_robot_percepts(team, id);
			int num_robots = robots.size();
			if (filter_data->get_robot_seen(team, id) && num_robots > 0) {

				robot_samples_old.clear();
				robot_samples_new.clear();
				robot_samples_old = filter_data->get_robot_samples(team, id);

				//calc total weight
				total_weight = 0.;
				for (Robot_Sample_List::iterator it = robot_samples_old.begin(); it != robot_samples_old.end(); ++it) {
					total_weight += it->weighting;
					it->age++;
				}

				average_weight = total_weight / Filter_Data::ROBOT_SAMPLES;

				o_slow_robots[team][id] += alpha_slow_robots * (average_weight - o_slow_robots[team][id]);
				if (o_slow_robots[team][id] < 0.00000000001)
					o_slow_robots[team][id] = 0.00000000001;
				o_fast_robots[team][id] += alpha_fast_robots * (average_weight - o_fast_robots[team][id]);
				if (o_fast_robots[team][id] < 0.000000000001)
					o_fast_robots[team][id] = 0.000000000001;

				augment = std::max(0., 1. - (o_fast_robots[team][id] / o_slow_robots[team][id]));

				//augmented preparation
				Robot_Sample augment_robot;
				//heuristical approach
				robot_speed = pf_data->get_robot_direction(team, id);

				for (int i = 0; i < Filter_Data::ROBOT_SAMPLES; ++i) {

					random = (double) rand() / (double) RAND_MAX;

					if (random < augment) { // insert new samples
						int robot_percept = random_number(0, (num_robots - 1));
						augment_robot.pos = BSmart::Pose(robots[robot_percept].x, robots[robot_percept].y,
								robots[robot_percept].rotation);

						//Heuristik
						augment_robot.speed = robot_speed;

						augment_robot.team = team;
						augment_robot.id = id;
						robot_samples_new.push_back(augment_robot);
					} else { //draw from derivation
						double r = (double) rand() / (double) RAND_MAX * total_weight;
						double cnt = 0.;
						int j = 0;

						while (cnt < r) {
							cnt += robot_samples_old[j].weighting;
							j++;
						}
						if (j == 0)
							++j;
						robot_samples_new.push_back(robot_samples_old[j - 1]);
					}
				}
				//copy new samples
				filter_data->set_robot_samples(team, id, robot_samples_new);
			}
		}
	}
}

void Particle_Filter::create_models() {
	//ball
	Ball_Sample ball_model;
	Ball_Sample_List ball_samples = filter_data->get_ball_samples();
	double total_weight = 0.000000001;
	robot_models.clear();

	for (Ball_Sample_List::iterator it = ball_samples.begin(); it != ball_samples.end(); ++it) {
		ball_model.pos += it->pos * it->weighting;
		ball_model.speed += it->speed * it->weighting;
		total_weight += it->weighting;
	}

	if (ball_model.pos.x == 0) {
		std::cout << "ball_model.pos: " << ball_model.pos << std::endl;
		std::cout << "total_weight: " << total_weight << std::endl;
		int i = 0;
		for (Ball_Sample_List::iterator it = ball_samples.begin(); it != ball_samples.end(); ++it) {
			i++;
		}
	}

	ball_model.pos /= total_weight;
	ball_model.speed /= total_weight;

	//robots
	for (int team = 0; team < Filter_Data::NUMBER_OF_TEAMS; ++team) {
		for (int id = 0; id < Filter_Data::NUMBER_OF_IDS; ++id) {
			Robot_Sample robot_model;
			total_weight = 0.000000001;

			if (filter_data->get_robot_seen(team, id)) {
				Robot_Sample_List robot_samples = filter_data->get_robot_samples(team, id);
				for (Robot_Sample_List::iterator it = robot_samples.begin(); it != robot_samples.end(); ++it) {
					robot_model.pos += it->pos * it->weighting;
					robot_model.speed += it->speed * it->weighting;
					total_weight += it->weighting;
				}
				robot_model.pos /= total_weight;
				robot_model.speed /= total_weight;

				robot_model.team = team;
				robot_model.id = id;
				//confidence
				filter_data->set_robot_model(team, id, robot_model);

				robot_models.push_back(robot_model);
			}
		}
	}

	//Find last touched and ball status
	determine_ball_status(ball_model);

	ball_model.last_touched = (Sample::Last_Touched) ball_last_touched_saved;
	ball_model.last_touched_robot = last_touched_robot_saved;
	ball_model.status = (Sample::Status) ball_status_saved;

	filter_data->set_ball_model(ball_model);

	filter_data->set_timestamp(BSmart::Systemcall::get_current_system_time());
	filter_data->set_frame(newest_frame);
}

void Particle_Filter::determine_ball_status(const Ball_Sample& new_ball_model) {
	ball_direction_before = pf_data->get_ball_direction_before();
	ball_direction_after = pf_data->get_ball_direction_after();

	double speed_before = ball_direction_before.abs();
	double speed_after = ball_direction_after.abs();
	double speed_diff_percept = speed_after - speed_before;
	double speed_diff_model = new_ball_model.speed.abs() - last_ball_model.speed.abs();
	double angle_to = acos(
			((ball_direction_before.x * ball_direction_after.x + ball_direction_before.y * ball_direction_after.y)
					/ (ball_direction_before.abs() * ball_direction_after.abs()))) / BSmart::pi * 180;
	if (std::isnan(angle_to)) {
		angle_to = 0.;
	}

	int new_ball_status = -1;
	int new_ball_last_touched = -1;
	Last_Contact last_contact;
	last_contact.timestamp = -1;
	last_contact.lying_shot = false;

	//Ball lying?
	if ((speed_before <= 0.05 && speed_after <= 0.05)
			|| (new_ball_model.pos.distance_to_3D(last_ball_model.pos) < 5 && ball_lying_counter > 4)) {
		new_ball_status = 6; //Sample::LYING
		ball_lying_counter++;
	} else if (ball_lying_counter > 4) {
		ball_lying_counter = -1;
	} else {
		ball_lying_counter = 0;
	}

	bool intersect = false;

	//Ball touched by robot?
	//Ball shot
	if ((speed_diff_percept > 1. || speed_diff_model > 1.) && (ball_lying_counter <= 0)) {
		last_touched_dist_tmp = std::numeric_limits<double>::max();
		for (Robot_Sample_List::const_iterator it = robot_models.begin(); it != robot_models.end(); it++) {
			circle.x = it->pos.x;
			circle.y = it->pos.y;
			ball_line.p1.x = new_ball_model.pos.x;
			ball_line.p1.y = new_ball_model.pos.y;
			ball_line.p2.x = new_ball_model.pos.x + ball_direction_after.x; //2d
			ball_line.p2.y = new_ball_model.pos.y + ball_direction_after.y; //2d
			intersections.clear();
			if (test_intersection(circle, ball_line)) {
				if (intersection_point(circle, ball_line, intersections)) {
					for (unsigned int j = 0; j < intersections.size(); ++j) {
						if (new_ball_model.pos.distance_to_2D(intersections[j])
								< new_ball_model.pos.distance_to_2D(it->pos)) {
							BSmart::Pose dir_intersect_to_new_ball_pos = BSmart::Pose(
									new_ball_model.pos.x - intersections[j].x,
									new_ball_model.pos.y - intersections[j].y);
							double collision_angle = acos(
									((dir_intersect_to_new_ball_pos.x * ball_direction_after.x
											+ dir_intersect_to_new_ball_pos.y * ball_direction_after.y)
											/ (dir_intersect_to_new_ball_pos.abs() * ball_direction_after.abs())))
									/ BSmart::pi * 180;
							if (collision_angle < 1.) {
								if (new_ball_model.pos.distance_to_2D(intersections[j]) < last_touched_dist_tmp) {
									last_touched_dist_tmp = new_ball_model.pos.distance_to_2D(intersections[j]);
									intersect = true;
									last_contact.frame = newest_frame;
									last_contact.robot.x = it->team;
									last_contact.robot.y = it->id;
									last_contact.timestamp = timestamp;
									last_contact.type = 4; //Sample::KICKED
								}
							}
						}
					}
				}
			}
		}
	}
	//ball lying and then shot
	if ((ball_lying_counter == -1) && !intersect) {
		last_touched_dist_tmp = std::numeric_limits<double>::max();
		for (Robot_Sample_List::const_iterator it = robot_models.begin(); it != robot_models.end(); it++) {
			if (last_ball_model.pos.distance_to_2D(it->pos)
					< (BSmart::Field::robot_radius + BSmart::Field::ball_radius + 50)) {
				last_touched_dist_tmp = new_ball_model.pos.distance_to_2D(it->pos) - BSmart::Field::robot_radius
						- BSmart::Field::ball_radius;
				intersect = true;
				last_contact.frame = newest_frame;
				last_contact.robot.x = it->team;
				last_contact.robot.y = it->id;
				last_contact.timestamp = timestamp;

				if (new_ball_model.speed.abs() > 0.6 || speed_after > 0.6) {
					last_contact.type = 4; //Sample::KICKED
				} else {
					last_contact.type = 5; //Sample::BOUNCED
				}
				last_contact.lying_shot = true;

			}
		}

	}
	//ball touched or bounced
	if ((fabs(speed_diff_percept) > 1 || fabs(speed_diff_model) > 1 || angle_to > 10) && (ball_lying_counter <= 0)
			&& !intersect) {
		last_touched_dist_tmp = std::numeric_limits<double>::max();
		for (Robot_Sample_List::const_iterator it = robot_models.begin(); it != robot_models.end(); it++) {
			if (new_ball_model.pos.distance_to_2D(it->pos)
					< (BSmart::Field::robot_radius + BSmart::Field::ball_radius + 100)) {
				circle.x = it->pos.x;
				circle.y = it->pos.y;
				ball_line.p1.x = last_ball_model.pos.x;
				ball_line.p1.y = last_ball_model.pos.y;
				ball_line.p2.x = last_ball_model.pos.x + ball_direction_before.x; //2d
				ball_line.p2.y = last_ball_model.pos.y + ball_direction_before.y; //2d
				intersections.clear();
				if (test_intersection(circle, ball_line)) {
					if (intersection_point(circle, ball_line, intersections)) {
						for (unsigned int j = 0; j < intersections.size(); ++j) {
							if (new_ball_model.pos.distance_to_2D(intersections[j]) < last_touched_dist_tmp) {
								last_touched_dist_tmp = new_ball_model.pos.distance_to_2D(intersections[j]);

								intersect = true;
								last_contact.frame = newest_frame;
								last_contact.robot.x = it->team;
								last_contact.robot.y = it->id;
								last_contact.timestamp = timestamp;
								last_contact.type = 5; //Sample::BOUNCED
							}
						}
					}
				}
			}
		}
	}
	// if angle greater than 60° then guaranteed collision
	if (angle_to > 60 && !intersect && !ball_lying_counter) {
		last_touched_dist_tmp = std::numeric_limits<double>::max();
		for (Robot_Sample_List::const_iterator it = robot_models.begin(); it != robot_models.end(); it++) {
			if (new_ball_model.pos.distance_to_2D(it->pos)
					< (BSmart::Field::robot_radius + BSmart::Field::ball_radius + 40)) {
				intersect = true;
				last_contact.frame = newest_frame;
				last_contact.robot.x = it->team;
				last_contact.robot.y = it->id;
				last_contact.timestamp = timestamp;
				last_contact.type = 5; //Sample::BOUNCED
			}
		}
	}

	// add last contact
	if (intersect) {
		last_contacts.push_back(last_contact);
	}

	//Ball flying and not touching robot
	//last contacts DELETE?
	bool flying = false;
	for (Robot_Sample_List::const_iterator it = robot_models.begin(); it != robot_models.end(); it++) {
		if (new_ball_model.pos.distance_to_2D(it->pos) < (BSmart::Field::robot_radius)) {
			last_contact.frame = newest_frame;
			last_contact.robot.x = it->team;
			last_contact.robot.y = it->id;
			last_contact.timestamp = timestamp;
			flying_robots.push_back(last_contact);
			flying = true;
		}
	}

	// delete "flying" from lc
	for (std::vector<Last_Contact>::iterator fr_it = flying_robots.begin(); fr_it != flying_robots.end();) {
		if ((timestamp - fr_it->timestamp) > 1000) {
			fr_it = flying_robots.erase(fr_it);
		} else {
			for (std::vector<Last_Contact>::iterator lc_it = last_contacts.begin(); lc_it != last_contacts.end();) {
				if (lc_it->robot == fr_it->robot && lc_it->type != 4 && !lc_it->lying_shot) {
					lc_it = last_contacts.erase(lc_it);
				} else {
					lc_it++;
				}
			}
			fr_it++;
		}
	}

	int log_size = all_contacts_log.size();
	// write lc to log
	for (std::vector<Last_Contact>::iterator lc_it = last_contacts.begin(); lc_it != last_contacts.end();) {
		if ((timestamp - lc_it->timestamp) > 1000) {
			if (log_size == 0 || lc_it->robot != all_contacts_log[log_size - 1].robot
					|| (lc_it->timestamp - all_contacts_log[log_size - 1].timestamp) > 1000) {
				all_contacts_log.push_back(*lc_it);
				log_size++;
			} else if (log_size != 0 || lc_it->robot == all_contacts_log[log_size - 1].robot) {
				all_contacts_log[log_size - 1].type = lc_it->type;
			}
			lc_it = last_contacts.erase(lc_it);
		} else {
			lc_it++;
		}
	}

	int len = last_contacts.size();

	if (flying) {
		if (len > 0) {
			last_contacts[len - 1].type = 3; //Flying
		} else if (log_size > 0) {
			all_contacts_log[log_size - 1].type = 3; //Flying
		}
		new_ball_status = 3;
	}

	if (intersect || flying) {
		if (len > 0) {
			if (last_touched_robot_saved != last_contacts[len - 1].robot) {
				last_touched_robot_saved = last_contacts[len - 1].robot;
				new_ball_status = last_contacts[len - 1].type;
				new_ball_last_touched = last_touched_robot_saved.x + 1;
			}
		} else if (log_size > 0 && last_touched_robot_saved != all_contacts_log[log_size - 1].robot) {
			last_touched_robot_saved = all_contacts_log[log_size - 1].robot;
			new_ball_status = all_contacts_log[log_size - 1].type;
			new_ball_last_touched = last_touched_robot_saved.x + 1;
		}
	}

	last_ball_model = new_ball_model;

	if (new_ball_status != -1 && new_ball_status != ball_status_saved) {
		ball_status_saved = new_ball_status;
		emit change_ball_status_intern(Sample::status_names[ball_status_saved]);
	}

	if (new_ball_last_touched != -1 && new_ball_last_touched != ball_last_touched_saved) {
		ball_last_touched_saved = new_ball_last_touched;
		emit change_ball_last_touched_intern(Sample::last_touched_names[ball_last_touched_saved]);
	}

}

Ball_Sample Particle_Filter::random_ball_sample() {
	BSmart::Pose3D pos = get_random_pos_3d(); //BSmart::Pose3D(-3000,0,0);//
	BSmart::Pose3D speed = get_random_pos_3d(0, 0, 0, 0, 0, 0); //BSmart::Pose3D(10,0,0);//
	Sample::Status status = Sample::STATUS_UNKNOWN;
	Sample::Last_Touched last_touched = Sample::TOUCH_UNKNOWN;

	Ball_Sample random_ball(pos, speed, status, last_touched);
	return random_ball;
}

BSmart::Pose3D Particle_Filter::get_random_pos_3d(int min_x, int max_x, int min_y, int max_y, int min_z, int max_z) {
	return BSmart::Pose3D(random_number(min_x, max_x), random_number(min_y, max_y), random_number(min_z, max_z));
}

Sample::Status Particle_Filter::get_random_status() {
	return Sample::STATUS_UNKNOWN;
}

Robot_Sample Particle_Filter::random_robot_sample() {
	BSmart::Pose pos = get_random_pos_2d();
	BSmart::Pose speed = get_random_pos_2d();

	Robot_Sample random_robot(pos, speed);
	return random_robot;
}

BSmart::Pose Particle_Filter::get_random_pos_2d(int min_x, int max_x, int min_y, int max_y) {
	return BSmart::Pose(random_number(min_x, max_x), random_number(min_y, max_y));
}

//calculates the dist between a sample in 3D and a percept in 2D
double Particle_Filter::dist_in_2d(int camID, BSmart::Pose3D sample, BSmart::Pose percept) {
	BSmart::Pose3D cam = pf_data->get_camera_position(camID);

	BSmart::Pose3D direction(sample - cam);
	if (direction.z >= 0)
		return 2000000;

	double factor = sample.z / direction.z;
	BSmart::Pose pose_2d(sample.x + factor * direction.x, sample.y + factor * direction.y);

	double dist = percept.distance_to(pose_2d);

	return dist;
}

double Particle_Filter::gaussian(double v, double std_dev) {
	// reference: "Mathematik für Informatiker" page 437
	return (1.0 / (std_dev * sqrt(2 * BSmart::pi))) * exp(-0.5 * pow(v / std_dev, 2));
}

int Particle_Filter::random_number(int bottom, int top) {
	return bottom + (rand() % (top - bottom + 1));
}
