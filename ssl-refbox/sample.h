#ifndef SAMPLE_H
#define SAMPLE_H

#include <vector>
#include <QString>

#include "field_hardware.h"
#include <libbsmart/pose.h>
#include <libbsmart/pose3d.h>

class Ball_Sample;
class Robot_Sample;
/** Typedef for easier use */
typedef std::vector<Ball_Sample> Ball_Sample_List;
typedef std::vector<Robot_Sample> Robot_Sample_List;

class Sample
{

public:

    enum Status
    {
        STATUS_UNKNOWN = 0,
        ROLLING,
        CHIPPED,
        FLYING,
        KICKED,
        BOUNCED,
        LYING,
        STATUS_NUM
    };
    static const QString status_names[STATUS_NUM];

    enum Last_Touched
    {
        TOUCH_UNKNOWN = 0,
        YELLOW,
        BLUE,
        REFEREE,
        LAST_TOUCHED_NUM
    };
    static const QString last_touched_names[LAST_TOUCHED_NUM];

    enum Type
    {
        BALL = 0,
        ROBOT
    };

    Sample();
    ~Sample() {};

    double weighting;
    Type type;
    Status status;
    int age;
    //ball_model only
    double timestamp;

    void fuettere_polarbaer(BSmart::Double_Vector*);

private:

};

class Ball_Sample : public Sample
{
public:
    Ball_Sample();
    Ball_Sample(BSmart::Pose3D, BSmart::Pose3D, Status, Last_Touched);
    Ball_Sample(const Ball_Sample&);
    ~Ball_Sample();

    BSmart::Pose3D pos;
    BSmart::Pose3D speed;

    //last_touched: true is blue
    Last_Touched last_touched;
    BSmart::Int_Vector last_touched_robot;

    void move(const double, const Robot_Sample_List&);

private:
    void check_collisions(const Robot_Sample_List&, double);
    bool check_bar_reflections(Hitpoint*);
    bool check_floor_reflection(Hitpoint*);
    bool check_goalpost_reflections(Hitpoint*);
    bool check_robot_reflections(Hitpoint*, const Robot_Sample_List&);

    static const double ball_noise;
    static const double ball_speed_noise;

    BSmart::Pose3D last_pos;

    //optimisation
    BSmart::Line ball_line;
    double random;
    BSmart::Pose3D collision3D;
    BSmart::Double_Vector intersection;
    std::vector<BSmart::Double_Vector> intersections;
    BSmart::Circle circle;

    BSmart::Double_Vector polarbaer;
};

class Robot_Sample : public Sample
{
public:
    Robot_Sample();
    Robot_Sample(BSmart::Pose, BSmart::Pose);
    Robot_Sample(const Robot_Sample&);
    ~Robot_Sample();

    BSmart::Pose pos;
    BSmart::Pose speed;
    int team;
    int id;
    double confidence;

    void move(const double, const Robot_Sample_List&);

private:
    void check_collisions(const Robot_Sample_List&);
    bool check_bar_reflections(Hitpoint*);
    bool check_goalpost_reflections(Hitpoint*);
    bool check_robot_reflections(Hitpoint*, const Robot_Sample_List&);

    static const double robot_noise;
    static const double robot_speed_noise;

    BSmart::Pose last_pos;

    //optimisation
    BSmart::Line robot_line;
    BSmart::Double_Vector intersection;
    std::vector<BSmart::Double_Vector> intersections;
    BSmart::Circle circle;

    BSmart::Double_Vector polarbaer;
};

#endif //SAMPLE_H
