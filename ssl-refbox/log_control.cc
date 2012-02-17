#include "log_control.h"
#include <math.h>
#include <sstream>

Log_Control::Log_Control()
{
    reset ( 0 );
}

Log_Control::~Log_Control()
{
    ;
}

void Log_Control::reset ( int size )
{
    current_frame = 0;
    next_frame = 0;
    log_length = size;
    play_speed = 1.;
    play_speed_save = 1.;
    // send signals to GUI
    update_play_speed();
}

int Log_Control::get_current_frame()
{

    if ( current_frame >= log_length )
        return 0;

    return current_frame;
}

int Log_Control::get_next_frame()
{
    if ( play_speed == 0. )
        return current_frame;

    current_frame = next_frame;
    if ( play_speed < 0. )
        next_frame--;
    else
        next_frame++;

    if ( current_frame >= log_length || current_frame < 0 )
        return -1;

    return current_frame;
}

int Log_Control::get_prop_next_frame()
{
    int prop_next_frame = current_frame;
    if ( play_speed < 0. )
        prop_next_frame--;
    else if ( play_speed > 0. )
        prop_next_frame++;

    if ( prop_next_frame < 0 || prop_next_frame >= log_length )
        return -1;

    return prop_next_frame;
}

double Log_Control::get_play_speed()
{
    return play_speed;
}

// Slots
void Log_Control::log_forward()
{
    play_speed = 5.;
    update_play_speed();
}

void Log_Control::log_backward()
{
    play_speed = -5.;
    update_play_speed();
}

void Log_Control::log_play()
{
    if ( play_speed != 0. )
        play_speed = play_speed_save;
    else
        play_speed = 1.;
    update_play_speed();
}

void Log_Control::log_pause()
{
    play_speed_save = play_speed;
    play_speed = 0.;
    update_play_speed();
}

void Log_Control::log_faster()
{
    play_speed *= 2.;
    update_play_speed();
}

void Log_Control::log_slower()
{
    play_speed /= 2.;
    update_play_speed();
}

void Log_Control::log_frame_back()
{
    current_frame--;
}

void Log_Control::log_frame_forward()
{
    current_frame++;
}

void Log_Control::goto_frame ( int f )
{
    current_frame = f;
    next_frame = f + 1;
}

void Log_Control::update_play_speed()
{
    double speed_tmp = floor ( play_speed * 100.0 + 0.5 ) / 100.0;
    std::ostringstream o;
    o << speed_tmp;
    QString speed = QString::number ( play_speed, 'g', 4 );

    speed.append ( "x" );
    emit update_speed ( speed );
}
