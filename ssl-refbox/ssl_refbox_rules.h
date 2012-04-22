#ifndef SSL_REFBOX_RULES_H
#define SSL_REFBOX_RULES_H

#include <QThread>
#include <QWaitCondition>
#include <libbsmart/game_states.h>
#include "filter_data.h"
#include <string.h>
#include "../ConfigFile/ConfigFile.h"
#include <log4cxx/logger.h>

extern char* argv_global;

class SSL_Refbox_Rules : public QThread
{
    Q_OBJECT

public:
    SSL_Refbox_Rules(QWaitCondition*, Filter_Data*, BSmart::Game_States*);
    ~SSL_Refbox_Rules();
    void run();
    static log4cxx::LoggerPtr logger;

signals:
    void new_filter_data();
    void new_broken_rule(Broken_Rule*);

private:
    char* argv_tmp[];
    QWaitCondition* rules_wait_condition;
    Filter_Data* filter_data;
    BSmart::Game_States* gamestate;
    BSmart::Game_States::Play_State play_state_old;
    int cur_frm;
    int cur_timestamp;
    BSmart::Int_Vector internal_play_states;

    char refbox_cmd_alt;
    bool written;
    bool written_halftime;
    int local_play_state_alt;
    int last_break;
    int last_msg;
    int touches;
};

#endif /* SSL_REFBOX_RULES_H */
