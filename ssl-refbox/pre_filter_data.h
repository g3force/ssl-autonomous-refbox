#ifndef PRE_FILTER_DATA_H
#define PRE_FILTER_DATA_H

#include <QMutex>

#include "percept.h"
#include "filter_data.h"
#include <libbsmart/pose3d.h>
#include <libbsmart/game_states.h>

struct Camera_Position
{
    BSmart::Pose3D cam_pos;
    double belief;
};

class Pre_Filter_Data
{
public:
    Pre_Filter_Data();

    //ball
    void set_ball_framenumber(int camID, unsigned int framenumber);
    void set_ball_timestamp(int camID, const BSmart::Time_Value& timestamp);
    void set_balls(int camID, const Ball_Percept_List& pBalls);
    void clear_balls(int camID);
    Ball_Percept_List get_current_balls(int camID);

    //robots
    void set_robots(int camID, int team, int id, const Robot_Percept_List& pRobots);
    void clear_robots(int camID);
    Robot_Percept_List get_robots(int camID, int team, int id);

    //camera
    void set_camera_pos(int camID, const BSmart::Pose3D& new_pos);
    BSmart::Pose3D get_camera_position(int camID);

    //pre-filter ball heuristics
    void set_ball_direction_before(const BSmart::Pose& ball_direction_before_);
    BSmart::Pose get_ball_direction_before();
    void set_ball_direction_after(const BSmart::Pose& ball_direction_after_);
    BSmart::Pose get_ball_direction_after();

    //pre-filter robot heuristics
    void set_robot_direction(int team, int id, const BSmart::Pose& robot_direction_);
    BSmart::Pose get_robot_direction(int team, int id);

    //framenumber
    void set_newest_frame(int);
    int get_newest_frame();

    //timestamp
    void set_timestamp(BSmart::Time_Value);
    BSmart::Time_Value get_timestamp();

private:
    QMutex pf_data_mutex;
    Ball_Percept_List current_balls[2];
    Robot_Percept_List robots[2][Filter_Data::NUMBER_OF_TEAMS][Filter_Data::NUMBER_OF_IDS];
    Camera_Position camera_pos[2];
    int cam_dist_threshhold;
    BSmart::Pose ball_direction_before;
    BSmart::Pose ball_direction_after;
    BSmart::Pose robot_direction[Filter_Data::NUMBER_OF_TEAMS][Filter_Data::NUMBER_OF_IDS];
    int newest_frame;
    BSmart::Time_Value timestamp;
    char refbox_cmd;
    BSmart::Game_States::Play_State play_state;
};

#endif //PRE_FILTER_DATA_H
