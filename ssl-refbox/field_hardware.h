#ifndef FIELD_HARDWARE_H
#define FIELD_HARDWARE_H

#include <libbsmart/line.h>
#include <libbsmart/circle.h>
#include <libbsmart/pose.h>

class Field_Bar;
class Field_Goalpost;

class Field_Hardware {

public:
	enum Obstacle {
		NO_OBSTACLE,
		BAR_HORIZONTAL,
		BAR_VERTICAL,
		GOALPOST,
		ROBOT_BLUE,
		ROBOT_YELLOW,
		FLOOR
	};

	enum {
		NUMBER_FIELD_BARS = 20, NUMBER_FIELD_GOALPOSTS = 8
	};

	Field_Hardware();

	static const Field_Bar field_bars_ball[NUMBER_FIELD_BARS];
	static const Field_Goalpost field_goalposts_ball[NUMBER_FIELD_GOALPOSTS];

	static const Field_Bar field_bars_robot[NUMBER_FIELD_BARS];
	static const Field_Goalpost field_goalposts_robot[NUMBER_FIELD_GOALPOSTS];

};

struct Hitpoint {
	BSmart::Double_Vector collisionpoint; //2d
	double dist;
	int height;
	Field_Hardware::Obstacle obstacle;
	int robot_id;
	BSmart::Double_Vector orientation_point;
};

class Field_Obstacle {
public:
	Field_Obstacle();

	Field_Hardware::Obstacle obstacle;
};

class Field_Bar: public Field_Obstacle {
public:
	Field_Bar();
	Field_Bar(BSmart::Pose, BSmart::Pose, Field_Hardware::Obstacle, double);
	Field_Bar(const Field_Bar&);

	BSmart::Pose point_a;
	BSmart::Pose point_b;
	BSmart::Line line;
	int dist;
	int height;
};

class Field_Goalpost: public Field_Obstacle {
public:
	Field_Goalpost();
	Field_Goalpost(BSmart::Pose, int, int);
	Field_Goalpost(const Field_Goalpost&);

	BSmart::Pose center;
	BSmart::Circle circle;
	int radius;
	int height;
};

#endif //FIELD_HARDWARE_H
