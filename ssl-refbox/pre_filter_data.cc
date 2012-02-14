#include "pre_filter_data.h"
#include <libbsmart/field.h>
#include <iostream>

Pre_Filter_Data::Pre_Filter_Data()
{
        QMutex pf_data_mutex();
        for (int cam = 0; cam < 2; cam++) {
                for (int team = 0; team < Filter_Data::NUMBER_OF_TEAMS; ++team) {
                        for (int id = 0; id < Filter_Data::NUMBER_OF_TEAMS; ++id) {
                                robots[cam][team][id] = Robot_Percept_List();
                                robot_direction[team][id] = BSmart::Pose();
                        }
                }
                current_balls[cam] = Ball_Percept_List();
                camera_pos[cam] = Camera_Position();
                camera_pos[cam].belief = 0;
        }
        //cam 0 left
        camera_pos[0].cam_pos = BSmart::Pose3D(
                                        -(BSmart::Field::half_field_width / 2), 0., 4000.);
        camera_pos[1].cam_pos = BSmart::Pose3D(BSmart::Field::half_field_width / 2,
                                               0., 4000.);
        cam_dist_threshhold = 84;
        ball_direction_before = BSmart::Pose(0., 0., 0.);
        ball_direction_after = BSmart::Pose(0., 0., 0.);
        newest_frame = 0;
}

void Pre_Filter_Data::set_ball_framenumber(int camID, unsigned int framenumber)
{
        pf_data_mutex.lock();
        if (current_balls[camID].size() > 0)
                current_balls[camID].front().framenumber = framenumber;
        pf_data_mutex.unlock();
}

void Pre_Filter_Data::set_ball_timestamp(int camID,
                const BSmart::Time_Value& timestamp)
{
        pf_data_mutex.lock();
        if (current_balls[camID].size() > 0)
                current_balls[camID].front().timestamp = timestamp;
        pf_data_mutex.unlock();
}

void Pre_Filter_Data::set_balls(int camID, const Ball_Percept_List& pBalls)
{
        pf_data_mutex.lock();
        current_balls[camID] = pBalls;
        pf_data_mutex.unlock();
}

void Pre_Filter_Data::clear_balls(int camID)
{
        pf_data_mutex.lock();
        current_balls[camID].clear();
        pf_data_mutex.unlock();
}

Ball_Percept_List Pre_Filter_Data::get_current_balls(int camID)
{
        pf_data_mutex.lock();
        Ball_Percept_List tmp = current_balls[camID];
        pf_data_mutex.unlock();
        return tmp;
}

void Pre_Filter_Data::set_robots(int camID, int team, int id,
                                 const Robot_Percept_List& pRobots)
{
        pf_data_mutex.lock();
        robots[camID][team][id] = pRobots;
        pf_data_mutex.unlock();
}

void Pre_Filter_Data::clear_robots(int camID)
{
        pf_data_mutex.lock();
        for (int team = 0; team < Filter_Data::NUMBER_OF_TEAMS; ++team) {
                for (int id = 0; id < Filter_Data::NUMBER_OF_IDS; ++id) {
                        robots[camID][team][id].clear();
                }
        }
        pf_data_mutex.unlock();
}

Robot_Percept_List Pre_Filter_Data::get_robots(int camID, int team, int id)
{
        pf_data_mutex.lock();
        Robot_Percept_List tmp = robots[camID][team][id];
        pf_data_mutex.unlock();
        return tmp;
}

void Pre_Filter_Data::set_camera_pos(int camID, const BSmart::Pose3D& new_pos)
{
        pf_data_mutex.lock();
        if (new_pos.distance_to_3D(camera_pos[camID].cam_pos) < cam_dist_threshhold) {
                camera_pos[camID].cam_pos.x
                += (new_pos.x - camera_pos[camID].cam_pos.x) / 2;
                camera_pos[camID].cam_pos.y
                += (new_pos.y - camera_pos[camID].cam_pos.y) / 2;
                camera_pos[camID].cam_pos.z
                += (new_pos.z - camera_pos[camID].cam_pos.z) / 2;
                camera_pos[camID].belief += 0.2;
                if (camera_pos[camID].belief > 1.)
                        camera_pos[camID].belief = 1.;
        } else {
                if (camera_pos[camID].belief < 0.5) {
                        camera_pos[camID].cam_pos = new_pos;
                        camera_pos[camID].belief -= 0.2;
                } else {
                        camera_pos[camID].belief -= 0.1;
                }
                if (camera_pos[camID].belief < 0.)
                        camera_pos[camID].belief = 0.;
        }
        pf_data_mutex.unlock();
}

BSmart::Pose3D Pre_Filter_Data::get_camera_position(int camID)
{
        pf_data_mutex.lock();
        BSmart::Pose3D tmp = camera_pos[camID].cam_pos;
        pf_data_mutex.unlock();
        return tmp;
}

void Pre_Filter_Data::set_ball_direction_before(
        const BSmart::Pose& ball_direction_before_)
{
        pf_data_mutex.lock();
        ball_direction_before = ball_direction_before_;
        pf_data_mutex.unlock();
}

BSmart::Pose Pre_Filter_Data::get_ball_direction_before()
{
        pf_data_mutex.lock();
        BSmart::Pose tmp = ball_direction_before;
        pf_data_mutex.unlock();
        return tmp;
}

void Pre_Filter_Data::set_ball_direction_after(
        const BSmart::Pose& ball_direction_after_)
{
        pf_data_mutex.lock();
        ball_direction_after = ball_direction_after_;
        pf_data_mutex.unlock();
}

BSmart::Pose Pre_Filter_Data::get_ball_direction_after()
{
        pf_data_mutex.lock();
        BSmart::Pose tmp = ball_direction_after;
        pf_data_mutex.unlock();
        return tmp;
}

void Pre_Filter_Data::set_robot_direction(int team, int id,
                const BSmart::Pose& robot_direction_)
{
        pf_data_mutex.lock();
        robot_direction[team][id] = robot_direction_;
        pf_data_mutex.unlock();
}

BSmart::Pose Pre_Filter_Data::get_robot_direction(int team, int id)
{
        pf_data_mutex.lock();
        BSmart::Pose tmp = robot_direction[team][id];
        pf_data_mutex.unlock();
        return tmp;
}

void Pre_Filter_Data::set_newest_frame(int frame)
{
        pf_data_mutex.lock();
        newest_frame = frame;
        pf_data_mutex.unlock();
}

int Pre_Filter_Data::get_newest_frame()
{
        pf_data_mutex.lock();
        int tmp = newest_frame;
        pf_data_mutex.unlock();
        return tmp;
}

void Pre_Filter_Data::set_timestamp(BSmart::Time_Value frame_received)
{
        pf_data_mutex.lock();
        timestamp = frame_received;
        pf_data_mutex.unlock();
}

BSmart::Time_Value Pre_Filter_Data::get_timestamp()
{
        pf_data_mutex.lock();
        BSmart::Time_Value tmp = timestamp;
        pf_data_mutex.unlock();
        return tmp;
}
