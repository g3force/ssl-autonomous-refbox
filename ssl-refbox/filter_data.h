#ifndef FILTER_DATA_H
#define FILTER_DATA_H

#include <QMutex>

#include "sample.h"
#include "field_hardware.h"
#include "percept.h"
#include <limits>
#include <libbsmart/systemcall.h>

struct Broken_Rule {
	int rule_number;
	BSmart::Time_Value when_broken;
	BSmart::Int_Vector freekick_pos;
	BSmart::Int_Vector rule_breaker;
	bool circle_around_ball;
	int defense_area; //-1 no, 0 left, 1 right
	BSmart::Line line_for_smth;
	BSmart::Int_Vector standing;
	int frame_broken; // first frame where rule was broken
};

class Filter_Data {
public:
	enum {
		NUMBER_OF_TEAMS = 2,
		NUMBER_OF_IDS = 12,
		ROBOT_SAMPLES = 50,
		BALL_SAMPLES = 250
	};

	Filter_Data();

	//Balls
	void set_ball_samples(const Ball_Sample_List&);
	Ball_Sample_List get_ball_samples();
	void set_ball_model(const Ball_Sample&);
	Ball_Sample get_ball_model();
	void set_current_ball_percepts(const Ball_Percept_List& ball_percepts);
	Ball_Percept_List get_current_ball_percepts();

	void move_balls(double, const Robot_Sample_List&);

	//Robots
	void set_robot_samples(int, int, const Robot_Sample_List&);
	Robot_Sample_List get_robot_samples(int, int);
	void set_robot_model(int, int, const Robot_Sample&);
	Robot_Sample get_robot_model(int, int);
	void set_current_robot_percepts(int, int, const Robot_Percept_List&);
	Robot_Percept_List get_current_robot_percepts(int, int);

	//for collision detection
	Robot_Sample_List get_current_robot_obstacles();

	//Visibility of robots
	void reduce_visibility();
	void set_robot_seen(int, int);
	bool get_robot_seen(int, int);

	void move_robots(double, const Robot_Sample_List&);

	void set_timestamp(const BSmart::Time_Value&);
	BSmart::Time_Value get_timestamp();

	void set_frame(const int&);
	int get_frame();

	//updates for broken rules for GUI
	void set_broken_rules(std::vector<Broken_Rule>);
	std::vector<Broken_Rule> get_broken_rules();

	void set_internal_play_states(BSmart::Int_Vector);
	BSmart::Int_Vector get_internal_play_states();

private:
	QMutex samples_mutex;

	Ball_Sample_List ball_samples;
	Ball_Sample ball_model;
	Ball_Percept_List current_ball_percepts;

	Robot_Sample_List robot_samples[NUMBER_OF_TEAMS][NUMBER_OF_IDS];
	Robot_Sample robot_models[NUMBER_OF_TEAMS][NUMBER_OF_IDS];
	Robot_Percept_List current_robot_percepts[NUMBER_OF_TEAMS][NUMBER_OF_IDS];

	double visibility[NUMBER_OF_TEAMS][NUMBER_OF_IDS];
	double visibility_threshhold;

	BSmart::Time_Value timestamp;
	int frame;

	//rule system results
	std::vector<Broken_Rule> broken_rules;
	BSmart::Int_Vector internal_play_states;
};

#endif //FILTER_DATA_H
