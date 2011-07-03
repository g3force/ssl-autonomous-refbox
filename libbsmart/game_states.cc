#include "game_states.h"

using namespace BSmart;

const char* Game_States::play_state_string(Play_State _play_state)
{
    switch (_play_state)
    {
        case HALTED:                    return "HALTED";
        case STOPPED:                   return "STOPPED";
        case RUNNING:                   return "RUNNING  :-)";
        case KICKOFF_YELLOW:            return "KICKOFF YELLOW";
        case PENALTY_YELLOW:            return "PENALTY YELLOW";
        case DIRECT_FREEKICK_YELLOW:    return "DIRECT FREEKICK YELLOW";
        case INDIRECT_FREEKICK_YELLOW:  return "INDIRECT FREEKICK YELLOW";
        case BEFORE_KICKOFF_YELLOW:     return "BEFORE KICKOFF YELLOW";
        case BEFORE_PENALTY_YELLOW:     return "BEFORE PENALTY YELLOW";
        case KICKOFF_BLUE:              return "KICKOFF BLUE";
        case PENALTY_BLUE:              return "PENALTY BLUE";
        case DIRECT_FREEKICK_BLUE:      return "DIRECT FREEKICK BLUE";
        case INDIRECT_FREEKICK_BLUE:    return "INDIRECT FREEKICK BLUE";
        case BEFORE_KICKOFF_BLUE:       return "BEFORE KICKOFF BLUE";
        case BEFORE_PENALTY_BLUE:       return "BEFORE PENALTY BLUE";
        default:                        return ("Unknown state!\n");
    }
}

const char* Game_States::play_state_string() const
{
    game_state_mutex.lock();
        const char* tmp = play_state_string(play_state);
    game_state_mutex.unlock();
    return tmp;
}

char Game_States::get_refbox_cmd() const
{
    game_state_mutex.lock();
        char tmp = this->refbox_cmd;
    game_state_mutex.unlock();
    return tmp;
}

void Game_States::set_refbox_cmd(char cmd)
{
    game_state_mutex.lock();
        this->refbox_cmd = cmd;
    game_state_mutex.unlock();
}

Game_States::Play_State Game_States::get_play_state() const
{
    game_state_mutex.lock();
        Play_State tmp = this->play_state;
    game_state_mutex.unlock();
    return tmp;
}

void Game_States::set_play_state(Play_State ps)
{
    game_state_mutex.lock();
        this->play_state = ps;
    game_state_mutex.unlock();
}

//timer_game
unsigned int Game_States::timer_game_get_overall()
{
    game_state_mutex.lock();
        unsigned int tmp = timer_game.get_overall();
    game_state_mutex.unlock();
    return tmp;
}

void Game_States::timer_game_start()
{
    game_state_mutex.lock();
        timer_game.start();
    game_state_mutex.unlock();
}

void Game_States::timer_game_pause()
{
    game_state_mutex.lock();
        timer_game.pause();
    game_state_mutex.unlock();
}

void Game_States::timer_game_unpause()
{
    game_state_mutex.lock();
        timer_game.unpause();
    game_state_mutex.unlock();
}

//timer_timeout_yellow
Timer_Status Game_States::timer_timeout_yellow_get_status()
{
    game_state_mutex.lock();
        Timer_Status tmp = timer_timeout_yellow.status;
    game_state_mutex.unlock();
    return tmp;
}

unsigned int Game_States::timer_timeout_yellow_get_overall()
{
    game_state_mutex.lock();
        unsigned int tmp = timer_timeout_yellow.get_overall();
    game_state_mutex.unlock();
    return tmp;
}

void Game_States::timer_timeout_yellow_start()
{
    game_state_mutex.lock();
        timer_timeout_yellow.start();
    game_state_mutex.unlock();
}

void Game_States::timer_timeout_yellow_pause()
{
    game_state_mutex.lock();
        timer_timeout_yellow.pause();
    game_state_mutex.unlock();
}

void Game_States::timer_timeout_yellow_unpause()
{
    game_state_mutex.lock();
        timer_timeout_yellow.unpause();
    game_state_mutex.unlock();
}

//timer_timeout_blue
Timer_Status Game_States::timer_timeout_blue_get_status()
{
    game_state_mutex.lock();
        Timer_Status tmp = timer_timeout_blue.status;
    game_state_mutex.unlock();
    return tmp;
}

unsigned int Game_States::timer_timeout_blue_get_overall()
{
    game_state_mutex.lock();
        unsigned int tmp = timer_timeout_blue.get_overall();
    game_state_mutex.unlock();
    return tmp;
}

void Game_States::timer_timeout_blue_start()
{
    game_state_mutex.lock();
        timer_timeout_blue.start();
    game_state_mutex.unlock();
}

void Game_States::timer_timeout_blue_pause()
{
    game_state_mutex.lock();
        timer_timeout_blue.pause();
    game_state_mutex.unlock();
}

void Game_States::timer_timeout_blue_unpause()
{
    game_state_mutex.lock();
        timer_timeout_blue.unpause();
    game_state_mutex.unlock();
}

//cards
void Game_States::add_card(int a, int b, int c, int d)
{
    game_state_mutex.lock();
        cards.yellow_yellow += a;
        cards.red_yellow    += b;
        cards.yellow_blue   += c;
        cards.red_blue      += d;
    game_state_mutex.unlock();
}

//goals
void Game_States::add_goal(int a, int b)
{
    game_state_mutex.lock();
        goals.yellow += a;
        goals.blue   += b;
    game_state_mutex.unlock();
}

void Game_States::set_game_time(Game_Time gt)
{
    game_state_mutex.lock();
        game_time = gt;
    game_state_mutex.unlock();
}

