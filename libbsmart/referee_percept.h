#ifndef REFEREE_PERCEPT_H
#define REFEREE_PERCEPT_H

class Referee_Percept
{
public:
    enum state
    {
        START,
        STOP,
        HALT,
        CANCEL,
        READY,

        BEGIN_FIRST_HALF,
        BEGIN_HALF_TIME,
        BEGIN_SECOND_HALF,
        BEGIN_OVERTIME_HALF_1,
        BEGIN_OVERTIME_HALF_2,
        BEGIN_PENALTY_SHOOTOUT,

        KICKOFF_YELLOW,
        FREEKICK_YELLOW,
        INDIRECT_YELLOW,
        PENALTY_YELLOW,
        TIMEOUT_YELLOW,
        YELLOW_CARD_YELLOW,
        RED_CARD_YELLOW,
        GOAL_YELLOW,
        DGOAL_YELLOW,

        KICKOFF_BLUE,
        FREEKICK_BLUE,
        INDIRECT_BLUE,
        PENALTY_BLUE,
        TIMEOUT_BLUE,
        YELLOW_CARD_BLUE,
        RED_CARD_BLUE,
        GOAL_BLUE,
        DGOAL_BLUE,

    };

};

#endif

