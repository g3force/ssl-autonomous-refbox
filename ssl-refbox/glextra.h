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

    static const std::string rulenames[42];

    GLExtra();
    GLExtra(Filter_Data*);
    ~GLExtra();

    //NOTE keep in sync with field.h
    static void bglDrawField();

    void bglDrawFilterData();

    void bglDrawRulesystemData();

private:
    ///Plot all symmetric points to given (x,y). \see bglBresCircle
    static void symmPlotPoints(const int& x, const int& y, const Quadrant& q);

    /**
    * Draw a circle with radius @c r using the Bresenham algorithm
    * (center is the current position)
    * @see http://www.cs.fit.edu/~wds/classes/graphics/Rasterize/rasterize/
    * @param r
    * @param q Enables drawing partial bow of a circle.
    */
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

    void draw_robot(int, int, SSLRefbox::Colors::Color, int team = -1, int id = -1, bool last_touched = false);

    void draw_ball(double, double, double, SSLRefbox::Colors::Color);

    int int_to_string(std::string& string, int i);

    //container for RulesystemData
    std::vector<Broken_Rule> broken_rule_vector;
    BSmart::Int_Vector internal_play_states;
    BSmart::Game_States* gamestate;
};

#endif /* _BSMARTGUI_GL_EXTRA_H_ */
