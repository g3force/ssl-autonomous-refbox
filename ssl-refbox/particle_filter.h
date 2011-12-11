#ifndef PARTICLE_FILTER_H
#define PARTICLE_FILTER_H

#include <QThread>
#include <QList>
#include <QWaitCondition>

#include "pre_filter_data.h"
#include "filter_data.h"
#include <libbsmart/field.h>
#include <libbsmart/systemcall.h>

struct Last_Contact
{
    int frame;
    BSmart::Int_Vector robot;
    BSmart::Time_Value timestamp;
    int type;
    bool lying_shot;
};

class Particle_Filter;

class Particle_Filter_Mother : public QThread
{
    Q_OBJECT

public:
    Particle_Filter_Mother(Pre_Filter_Data*, Filter_Data*, QWaitCondition*, QWaitCondition*);
    ~Particle_Filter_Mother();
    void connectActions();
    void run();

signals:
    void change_ball_status(QString);
    void change_ball_last_touched(QString);

public slots:
    void new_frame();

private:
    Particle_Filter* pf;
    Filter_Data* filter_data;
    QWaitCondition* new_data_wait_condition;
    bool new_data;
    QWaitCondition* rules_wait_condition;
};

class Particle_Filter : public QObject
{
    Q_OBJECT

public:
    Particle_Filter(Pre_Filter_Data*, Filter_Data*);
    ~Particle_Filter();

    void motion_update();

    void sensor_update(); // update weighting
    void resample();
    void create_models();

signals:
    void change_ball_status_intern(QString);
    void change_ball_last_touched_intern(QString);

private:
    Pre_Filter_Data* pf_data;
    Filter_Data* filter_data;

    //Timestamps for movement
    double last_movement;
    double ball_distance_threshold;
    double std_dev_ball;
    double std_dev_robot;

    //how many particles shall be augmented
    double alpha_slow_ball;
    double alpha_fast_ball;
    double o_slow_ball;
    double o_fast_ball;

    double alpha_slow_robots;
    double alpha_fast_robots;
    double o_slow_robots[Filter_Data::NUMBER_OF_TEAMS][Filter_Data::NUMBER_OF_IDS];
    double o_fast_robots[Filter_Data::NUMBER_OF_TEAMS][Filter_Data::NUMBER_OF_IDS];

    //for better runtime
    //resampling
    Ball_Sample_List ball_samples_old;
    Ball_Sample_List ball_samples_new;
    Robot_Sample_List robot_samples_old;
    Robot_Sample_List robot_samples_new;

    //Last_touched
    BSmart::Pose ball_direction_before;
    BSmart::Pose ball_direction_after;
    BSmart::Int_Vector last_touched_robot_saved;

    //Ball_Status
    Robot_Sample_List robot_models;
    Ball_Sample last_ball_model;
    int ball_lying_counter;
    Robot_Sample_List robot_obstacles;
    BSmart::Line ball_line;
    BSmart::Circle circle;
    std::vector<BSmart::Double_Vector> intersections;
//    BSmart::Int_Vector last_touched_tmp;
    double last_touched_dist_tmp;
    int newest_frame;
    BSmart::Time_Value timestamp;
    std::vector<Last_Contact> last_contacts;
    std::vector<Last_Contact> flying_robots;
    std::vector<Last_Contact> all_contacts_log;

    //saves for gui updates
    int ball_last_touched_saved; //Sample::Last_Touched
    int ball_status_saved; //Sample::Status

    Ball_Sample random_ball_sample();
    BSmart::Pose3D get_random_pos_3d(int min_x = -(BSmart::Field::half_field_width + BSmart::Field::off_width) , int max_x = BSmart::Field::half_field_width + BSmart::Field::off_width, int min_y = - (BSmart::Field::half_field_height + BSmart::Field::off_width), int max_y = BSmart::Field::half_field_height  + BSmart::Field::off_width, int min_z = 0, int max_z = 10);
    Sample::Status get_random_status();

    Robot_Sample random_robot_sample();
    BSmart::Pose get_random_pos_2d(int min_x = -(BSmart::Field::half_field_width + BSmart::Field::off_width) , int max_x = BSmart::Field::half_field_width + BSmart::Field::off_width, int min_y = - (BSmart::Field::half_field_height + BSmart::Field::off_width), int max_y = BSmart::Field::half_field_height  + BSmart::Field::off_width);

    bool weight_ball(Ball_Percept&);
    bool weight_robot(Robot_Percept&, int, int);

    void determine_ball_status(const Ball_Sample&);

    //calc the sample ball pos into 2d and then distance to percept
    double dist_in_2d(int camID, BSmart::Pose3D sample, BSmart::Pose percept);

    double gaussian(double v, double std_dev);

    void print_ball_percept(Ball_Percept);
    void print_robot_percept(Robot_Percept);

    int random_number(int bottom, int top);

};

#endif //PARTICLE_FILTER_H
