/**
 * @file glextra.h
 * @brief GLExtra header file
 */
#ifndef _BSMARTGUI_GL_EXTRA_H_
#define _BSMARTGUI_GL_EXTRA_H_ 1

//NOTE Not including <GL/gl.h> to allow for OS X compatability..
#ifndef GL_TRUE
#error You must include <GL/gl.h> before including this file!
#endif

#include <GL/glu.h>
#include "pre_filter_data.h"
#include "filter_data.h"
#include "colors.h"
#include <log4cxx/logger.h>

/**
 * @class GLExtra
 * @brief Do all the drawing of the field
 * This class provides all the functions that draw e.g. robots, ball, field and rules
 */
class GLExtra
{
public:

    enum Quadrant
    {
    Q_I   = 0x1,
    Q_II  = 0x2,
    Q_III = 0x4,
    Q_IV  = 0x8,
    Q_N   = Q_I | Q_II,                 //north hemicircle
    Q_S   = Q_III | Q_IV,               //south hemicircle
    Q_W   = Q_II | Q_III,               //west  hemicircle
    Q_E   = Q_I | Q_IV,                 //east  hemicircle
    //     Q_SENW  = Q_II | Q_IV,
    //     Q_SWNE  = Q_III | Q_I,
    //     Q_3CCW  = Q_I | Q_II | Q_III,
    //     Q_3CW   = Q_I | Q_IV | Q_III,
    Q_ALL = Q_I | Q_II | Q_III | Q_IV   //full  circle
    }; /* enum Quadrant */

    GLExtra();
    GLExtra(Filter_Data*);
    ~GLExtra();

    //NOTE keep in sync with field.h
    static void bglDrawField();

    void bglDrawFilterData();

    void bglDrawRulesystemData();

private:
    static log4cxx::LoggerPtr logger;
    ///Plot all symmetric points to given (x,y). \see bglBresCircle
    static void symmPlotPoints(const int& x, const int& y, const Quadrant& q);

    static void bglBresCircle(const int& r, const Quadrant& q = Q_ALL);

    ///Since glPointSize is not available on all graphic cards
    ///we have to use a wrapper
    static void bglPoint(const float x, const float y, const float sz = 20);

    //NOTE keep in sync with field.h
    static void draw_defense_area(int);

    //NOTE keep in sync with field.h
    static void draw_goal();

    //NOTE keep in sync with field.h
    static void draw_marks();

    Filter_Data* filter_data;
    //container for filter_data
    //ball_percepts
    Ball_Percept_List current_ball_percepts;
    //ball_samples
    Ball_Sample_List ball_samples;
    //ball_model
    Ball_Sample ball_model;
    //robot_percepts
    Robot_Percept_List tmp_perc_robots;
    Robot_Percept_List current_robot_percepts;
    //robot_samples
    Robot_Sample_List tmp_sample_robots;
    Robot_Sample_List robot_samples;
    //robot_models
    Robot_Sample_List robot_models;

    int cur_timestamp;

    void draw_robot(int, int, SSLRefbox::Colors::Color, double rotation = 0, int team = -1, int id = -1, bool last_touched = false);

    void draw_ball(double, double, double, SSLRefbox::Colors::Color);

    int int_to_string(std::string& string, int i);

    //container for RulesystemData
    std::vector<Broken_Rule> broken_rule_vector;
    BSmart::Int_Vector internal_play_states;
    BSmart::Game_States* gamestate;
};

#endif /* _BSMARTGUI_GL_EXTRA_H_ */
