#ifndef GAME_STATES_H
#define GAME_STATES_H

#include <QMutex>

#include "timer.h"

namespace BSmart
{
    class Game_States
    {
    public:
        Game_States()
        {
            game_time =  FIRST_HALF;
            play_state = HALTED;
            goals.yellow = 0;
            goals.blue = 0;
            cards.yellow_yellow = 0;
            cards.red_yellow = 0;
            cards.yellow_blue = 0;
            cards.red_blue = 0;
            timer_game = Timer();
            timer_timeout_yellow = Timer();
            timer_timeout_blue = Timer();
            timouts_yellow = 0;
            timouts_blue = 0;
            refbox_cmd = 'H';
            QMutex game_state_mutex();
        }

        enum Play_State
        {
            HALTED = 0,
            STOPPED,//1
            RUNNING,//2
            KICKOFF_YELLOW,//3
            PENALTY_YELLOW,//4
            DIRECT_FREEKICK_YELLOW, //5
            INDIRECT_FREEKICK_YELLOW, //6
            BEFORE_KICKOFF_YELLOW, // Pre-States
            BEFORE_PENALTY_YELLOW, //8
            KICKOFF_BLUE, //9
            PENALTY_BLUE, //10
            DIRECT_FREEKICK_BLUE, //11
            INDIRECT_FREEKICK_BLUE, //12
            BEFORE_KICKOFF_BLUE, // Pre-States
            BEFORE_PENALTY_BLUE//14
        };

        enum Game_Time
        {
             FIRST_HALF,
             HALF_TIME,
             SECOND_HALF,
             OVERTIME_HALF_1,
             OVERTIME_HALF_2,
             PENALTY_SHOOTOUT
        };

        char get_refbox_cmd() const;
        void set_refbox_cmd(char);
        const char* play_state_string() const;
        Play_State get_play_state() const;
        void set_play_state(Play_State);

        //timer_game
        unsigned int timer_game_get_overall();
        void timer_game_start();
        void timer_game_pause();
        void timer_game_unpause();

        //timer_timeout_yellow
        Timer_Status timer_timeout_yellow_get_status();
        unsigned int timer_timeout_yellow_get_overall();
        void timer_timeout_yellow_start();
        void timer_timeout_yellow_pause();
        void timer_timeout_yellow_unpause();

        //timer_timeout_blue
        Timer_Status timer_timeout_blue_get_status();
        unsigned int timer_timeout_blue_get_overall();
        void timer_timeout_blue_start();
        void timer_timeout_blue_pause();
        void timer_timeout_blue_unpause();

        //cards
        void add_card(int, int, int, int);

        //goals
        void add_goal(int, int);

        //game_time
        void set_game_time(Game_Time);

    private:
        mutable QMutex game_state_mutex;

        static const char* play_state_string(Play_State);

        //timers for game, actual timout and counters for taken timouts, current refbox_cmd
        Timer timer_game;
        Timer timer_timeout_yellow;
        Timer timer_timeout_blue;
        int timouts_yellow;
        int timouts_blue;
        char refbox_cmd;

        Play_State play_state;
        Game_Time game_time;

        //int timeouts_left;
        //long int timeout_time;

        struct Goals
        {
            int yellow;
            int blue;
        } goals;

        struct Cards
        {
            int yellow_yellow;
            int red_yellow;
            int yellow_blue;
            int red_blue;
        } cards;

        // Game_State_stack !
    };

}

#endif // GAME_STATES_H
