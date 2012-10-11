#include "filter_data.h"
#include <cassert>
#include <iostream>
#include <cmath>

Filter_Data::Filter_Data()
{
    QMutex samples_mutex();

    for ( int team = 0; team < NUMBER_OF_TEAMS; ++team ) {
        for ( int id = 0; id < NUMBER_OF_IDS; ++id ) {
            visibility[team][id] = 0.49;
        }
    }
    visibility_threshhold = 0.5;
    ball_samples = Ball_Sample_List ( BALL_SAMPLES, Ball_Sample() );
    broken_rules.clear();
    internal_play_states = BSmart::Int_Vector ( 0, 0 );
}

void Filter_Data::set_ball_samples ( const Ball_Sample_List& balls )
{
    samples_mutex.lock();
    ball_samples = balls;
    samples_mutex.unlock();
}

Ball_Sample_List Filter_Data::get_ball_samples()
{
    samples_mutex.lock();
    Ball_Sample_List tmp = ball_samples;
    samples_mutex.unlock();
    return tmp;
}

void Filter_Data::set_ball_model ( const Ball_Sample& model )
{
    samples_mutex.lock();
    ball_model = model;
    samples_mutex.unlock();
}

Ball_Sample Filter_Data::get_ball_model()
{
    samples_mutex.lock();
    Ball_Sample tmp = ball_model;
    samples_mutex.unlock();
    return tmp;
}

void Filter_Data::set_current_ball_percepts (
    const Ball_Percept_List& ball_percepts )
{
    samples_mutex.lock();
    current_ball_percepts = ball_percepts;
    samples_mutex.unlock();
}

Ball_Percept_List Filter_Data::get_current_ball_percepts()
{
    samples_mutex.lock();
    Ball_Percept_List tmp = current_ball_percepts;
    samples_mutex.unlock();
    return tmp;
}

void Filter_Data::move_balls ( double ms, const Robot_Sample_List& robots )
{
    samples_mutex.lock();
    for ( int i = 0; i < BALL_SAMPLES; ++i ) {
        ball_samples[i].move ( ms, robots );
    }
    samples_mutex.unlock();
}

void Filter_Data::set_robot_samples ( int team, int id,
//                                      const Robot_Sample_List& robots )
        Robot_Sample_List& robots )
{
    assert ( team >= 0 && team < NUMBER_OF_TEAMS );
    assert ( id >= 0 && id < NUMBER_OF_IDS );

    samples_mutex.lock();
    robot_samples[team][id] = robots;
    samples_mutex.unlock();
}

Robot_Sample_List Filter_Data::get_robot_samples ( int team, int id )
{
    assert ( team >= 0 && team < NUMBER_OF_TEAMS );
    assert ( id >= 0 && id < NUMBER_OF_IDS );

    samples_mutex.lock();
    Robot_Sample_List tmp = robot_samples[team][id];
    samples_mutex.unlock();
    return tmp;
}

void Filter_Data::set_robot_model ( int team, int id, const Robot_Sample& model )
{
    assert ( team >= 0 && team < NUMBER_OF_TEAMS );
    assert ( id >= 0 && id < NUMBER_OF_IDS );

    samples_mutex.lock();
    robot_models[team][id] = model;
    samples_mutex.unlock();
}

Robot_Sample Filter_Data::get_robot_model ( int team, int id )
{
    assert ( team >= 0 && team < NUMBER_OF_TEAMS );
    assert ( id >= 0 && id < NUMBER_OF_IDS );

    samples_mutex.lock();
    Robot_Sample tmp = robot_models[team][id];
    samples_mutex.unlock();
    return tmp;
}

void Filter_Data::set_current_robot_percepts ( int team, int id,
        const Robot_Percept_List& cur_robots )
{
    assert ( team >= 0 && team < NUMBER_OF_TEAMS );
    assert ( id >= 0 && id < NUMBER_OF_IDS );

    samples_mutex.lock();
    current_robot_percepts[team][id] = cur_robots;
    samples_mutex.unlock();
}

Robot_Percept_List Filter_Data::get_current_robot_percepts ( int team, int id )
{
    assert ( team >= 0 && team < NUMBER_OF_TEAMS );
    assert ( id >= 0 && id < NUMBER_OF_IDS );

    samples_mutex.lock();
    Robot_Percept_List tmp = current_robot_percepts[team][id];
    samples_mutex.unlock();
    return tmp;
}

Robot_Sample_List Filter_Data::get_current_robot_obstacles()
{
    Robot_Sample_List obstacles;
    obstacles.clear();

    samples_mutex.lock();
    for ( int team = 0; team < NUMBER_OF_TEAMS; ++team )
        for ( int id = 0; id < NUMBER_OF_IDS; ++id )
            if ( visibility[team][id] > visibility_threshhold )
                obstacles.push_back ( robot_models[team][id] );
    samples_mutex.unlock();

    return obstacles;
}

void Filter_Data::reduce_visibility()
{
    samples_mutex.lock();
    for ( int team = 0; team < NUMBER_OF_TEAMS; ++team )
        for ( int id = 0; id < NUMBER_OF_IDS; ++id ) {
            visibility[team][id] -= 0.025;//subtracted from visibility every cycle
            if ( visibility[team][id] < 0. )
                visibility[team][id] = 0.;
        }
    samples_mutex.unlock();
}

void Filter_Data::set_robot_seen ( int team, int id )
{
    assert ( team >= 0 && team < NUMBER_OF_TEAMS );
    assert ( id >= 0 && id < NUMBER_OF_IDS );

    samples_mutex.lock();
    visibility[team][id] += 0.2;//added to Visibility, can be large for good vision
    if ( visibility[team][id] > 1. )
        visibility[team][id] = 1.;
    samples_mutex.unlock();
}

bool Filter_Data::get_robot_seen ( int team, int id )
{
    assert ( team >= 0 && team < NUMBER_OF_TEAMS );
    assert ( id >= 0 && id < NUMBER_OF_IDS );

    samples_mutex.lock();
    bool tmp = ( visibility[team][id] > visibility_threshhold );//Threshold for visibility
    samples_mutex.unlock();
    return tmp;
}

void Filter_Data::move_robots ( double ms, const Robot_Sample_List& robots )
{
    samples_mutex.lock();
    for ( int team = 0; team < NUMBER_OF_TEAMS; ++team ) {
        for ( int id = 0; id < NUMBER_OF_IDS; ++id ) {
            if ( visibility[team][id] > visibility_threshhold ) {
                for ( int i = 0; i < ROBOT_SAMPLES; ++i ) {
                    robot_samples[team][id][i].move ( ms, robots );
                }
            }
        }
    }
    samples_mutex.unlock();
}

void Filter_Data::set_timestamp ( const BSmart::Time_Value& timestamp_ )
{
    samples_mutex.lock();
    timestamp = timestamp_;
    samples_mutex.unlock();
}

BSmart::Time_Value Filter_Data::get_timestamp()
{
    samples_mutex.lock();
    BSmart::Time_Value tmp = timestamp;
    samples_mutex.unlock();
    return tmp;
}

void Filter_Data::set_frame ( const int& frame_ )
{
    samples_mutex.lock();
    frame = frame_;
    samples_mutex.unlock();
}

int Filter_Data::get_frame()
{
    samples_mutex.lock();
    int tmp = frame;
    samples_mutex.unlock();
    return tmp;
}

void Filter_Data::set_broken_rules ( std::vector<Broken_Rule> broken_rules_ )
{
    samples_mutex.lock();
    broken_rules = broken_rules_;
    samples_mutex.unlock();
}

std::vector<Broken_Rule> Filter_Data::get_broken_rules()
{
    samples_mutex.lock();
    std::vector<Broken_Rule> tmp = broken_rules;
    samples_mutex.unlock();
    return tmp;
}

void Filter_Data::set_internal_play_states (
    BSmart::Int_Vector internal_play_states_ )
{
    samples_mutex.lock();
    internal_play_states = internal_play_states_;
    samples_mutex.unlock();
}

BSmart::Int_Vector Filter_Data::get_internal_play_states()
{
    samples_mutex.lock();
    BSmart::Int_Vector tmp = internal_play_states;
    samples_mutex.unlock();
    return tmp;
}
