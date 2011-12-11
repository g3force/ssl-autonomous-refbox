#include "pf_tester.h"
#include <iostream>
#include <libbsmart/field.h>

PF_Tester::PF_Tester(Pre_Filter_Data* pf_data_, BSmart::Game_States* gamestate_) :
	pf_data(pf_data_), gamestate(gamestate_) {
	pf_data->clear_balls(0);
	pf_data->clear_balls(1);
	reset_speed();
	ms = 20;
}

PF_Tester::~PF_Tester() {
	;
}

void PF_Tester::run() {
	while (1) {
		//        move_ball_respawn(ms);
		//        move_ball_bounce(ms);
		move_ball_rest();
		emit
		new_frame();
		msleep(ms);
	}
}

void PF_Tester::reset_speed() {
	speed = 5.;
}

/**
 * speed in m/s
 * always from left goal to right goal and jump back
 */
void PF_Tester::move_ball_respawn(int ms) {
	Ball_Percept_List cur_balls_0 = pf_data->get_current_balls(0);
	Ball_Percept_List cur_balls_1 = pf_data->get_current_balls(1);
	Ball_Percept new_ball(0., 0., 0.);
	Ball_Percept_List new_balls;
	new_balls.clear();

	int cam = 0;
	double movement = speed * ms;
	if (cur_balls_0.size() == cur_balls_1.size()) {
		new_ball.x = -3000.;
		cam = 0;
	} else if (cur_balls_0.size() > 0) {
		Ball_Percept old_ball = cur_balls_0[0];
		new_ball.x = old_ball.x + movement;
		if (new_ball.x >= 0) {
			cam = 1;
		}
	} else if (cur_balls_1.size() > 0) {
		Ball_Percept old_ball = cur_balls_1[0];
		new_ball.x = old_ball.x + movement;
		if (new_ball.x >= 3000.) {
			new_ball.x = -3000.;
			cam = 0;
			reset_speed();
		}
	} else
		std::cout << "pf_tester: komisch" << std::endl;

	pf_data->clear_balls(0);
	pf_data->clear_balls(1);
	new_balls.push_back(new_ball);
	pf_data->set_balls(cam, new_balls);

	//per second 5% velocity loose
	//    speed *= (1. - (0.50 * ms/1000.));
}

/**
 * speed in m/s
 * always on y = 0 with bounce back
 */
void PF_Tester::move_ball_bounce(int ms) {
	Ball_Percept_List cur_balls_0 = pf_data->get_current_balls(0);
	Ball_Percept_List cur_balls_1 = pf_data->get_current_balls(1);
	Ball_Percept new_ball(0., 0., 0.);
	Ball_Percept_List new_balls;
	new_balls.clear();
	int cam = 0;
	double movement = speed * ms;

	if (cur_balls_0.size() == cur_balls_1.size()) {
		new_ball.x = -3000.;
		cam = 0;
	} else if (cur_balls_0.size() > 0) {
		Ball_Percept old_ball = cur_balls_0[0];
		new_ball.x = old_ball.x + movement;
		if (new_ball.x >= 0) {
			cam = 1;
		} else if (new_ball.x <= (BSmart::Field::lower_own_goal_corner_back.x
				+ BSmart::Field::ball_radius)) {
			speed = 1.;
		}
	} else if (cur_balls_1.size() > 0) {
		Ball_Percept old_ball = cur_balls_1[0];
		new_ball.x = old_ball.x + movement;
		if (new_ball.x <= 0) {
			cam = 0;
		}
		if (new_ball.x >= BSmart::Field::lower_opponent_goal_corner_back.x
				- BSmart::Field::ball_radius) {
			speed = -1.;
		}
	} else
		std::cout << "pf_tester: strange" << std::endl;

	pf_data->clear_balls(0);
	pf_data->clear_balls(1);
	new_balls.push_back(new_ball);
	pf_data->set_balls(cam, new_balls);
}

void PF_Tester::move_ball_rest() {
	Ball_Percept_List new_balls;
	new_balls.clear();
	Robot_Percept_List new_blue_robots;
	new_blue_robots.clear();
	Robot_Percept_List new_yellow_robots;
	new_yellow_robots.clear();

	pf_data->clear_balls(0);
	pf_data->clear_balls(1);
	for (int i = 0; i < 2; ++i) {
		pf_data->clear_robots(i);
	}

	Ball_Percept new_ball(-1000., 0., 0.);
	new_ball.confidence = 100;

	int cam = 0;
	new_balls.push_back(new_ball);
	pf_data->set_balls(cam, new_balls);

	cam = 1;

	Robot_Percept new_yellow_robot(1500., 000., 0.);
	new_yellow_robot.confidence = 100;
	new_yellow_robot.color = SSLRefbox::Colors::YELLOW;
	new_yellow_robot.id = 1;
	new_yellow_robots.push_back(new_yellow_robot);

	pf_data->set_robots(cam, 0, new_yellow_robot.id, new_yellow_robots);

	Robot_Percept new_blue_robot(2000., 0., 0.);
	new_blue_robot.confidence = 100;
	new_blue_robot.color = SSLRefbox::Colors::BLUE;
	new_blue_robot.id = 3;
	new_blue_robots.push_back(new_blue_robot);

	pf_data->set_robots(cam, 1, new_blue_robot.id, new_blue_robots);
	new_blue_robots.clear();

	Robot_Percept new_blue_robot_2(1000., 000., 0.);
	new_blue_robot_2.confidence = 100;
	new_blue_robot_2.color = SSLRefbox::Colors::BLUE;
	new_blue_robot_2.id = 5;
	new_blue_robots.push_back(new_blue_robot_2);

	pf_data->set_robots(cam, 1, new_blue_robot.id, new_blue_robots);

}
