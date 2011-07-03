#ifndef PF_TESTER_H
#define PF_TESTER_H

#include <QThread>

#include <libbsmart/game_states.h>
#include "pre_filter_data.h"

class PF_Tester : public QThread
{
    Q_OBJECT

public:
    PF_Tester(Pre_Filter_Data*, BSmart::Game_States*);
    ~PF_Tester();
    void run();

signals:
    void new_frame();

private:
    Pre_Filter_Data* pf_data;
    BSmart::Game_States* gamestate;
    double speed;
    int ms;

    void reset_speed();
    void move_ball_respawn(int);
    void move_ball_bounce(int);
    void move_ball_rest();
};


#endif //PF_TESTER_H


