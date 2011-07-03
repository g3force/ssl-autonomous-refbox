#ifndef PERCEPT_H
#define PERCEPT_H

#include <vector>

#include <libbsmart/pose.h>
#include <libbsmart/systemcall.h>
#include <libbsmart/vector2.h>

//ssl-refbox-version
#include "colors.h"

/**
 * Base class for all things which are percepted on the field.
 */
class Percept : public BSmart::Pose
{
public:
    int cam;                  //!< Camera from which the percept was detected
    unsigned int framenumber; //!< Frame in which the percept was detected
    unsigned int confidence;  //!< Quality measure of the observation

    BSmart::Time_Value timestamp; //!< Time stamp of the perception

    void init()
    {
        cam         = 0;
        framenumber = 0;
        confidence  = 0;
        timestamp   = BSmart::Time_Value();
    }

    Percept()
    {
        init();
    }

    //generating percepts, pf_tester
    Percept(double x, double y, double rot)
            :BSmart::Pose(x,y,rot)
    {
        init();
    }
};


/**
 * Special percept representing a robot.
 */
class Robot_Percept : public Percept
{
public:
    /** Number of robot */
    int id;

    /** This is exactly what you think it is */
    bool rotation_known;

    SSLRefbox::Colors::Color color;

    Robot_Percept() : Percept(), id(-1), rotation_known(false),
                      color(SSLRefbox::Colors::YELLOW)
    {}

    //generating percepts, pf_tester
    Robot_Percept(double x, double y, double rot)
                 :Percept(x,y,rot)
    {
        ;
    }
};


/**
 * A Ball_Percept is not more than a Percept that can be sorted by its timestamp. This
 * is used within the chipkick detection to build up an order the percepts from different
 * cameras are processed in
 */
class Ball_Percept : public Percept
{
public:

    bool operator<(const Ball_Percept& comp) const
    {
        return this->timestamp < comp.timestamp;
    }

    Ball_Percept(){};

    //generating percepts, pf_tester
    Ball_Percept(double x, double y, double rot)
                :Percept(x,y,rot)
    {
        ;
    }
};

/** Typedef for easier use */
typedef std::vector<BSmart::Double_Vector> Team_Marker_List;

/** Typedef for easier use */
typedef std::vector<Robot_Percept> Robot_Percept_List;
typedef std::vector<Ball_Percept>  Ball_Percept_List;


#endif
