/**
 * @file log_control.c
 * @brief Log_Control source file
 */
#include "log_control.h"
#include <math.h>
#include <sstream>

/**
 * @brief Initialize by calling reset
 */
Log_Control::Log_Control()
{
    reset ( 0 );
}

Log_Control::~Log_Control()
{
    ;
}

/**
 * @brief Reset all variables
 * @param size size of log_length
 */
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

/**
 * @brief Get the current frame
 * @return current frame or zero
 */
int Log_Control::get_current_frame()
{

    if ( current_frame >= log_length )
        return 0;

    return current_frame;
}

/**
 * @brief Return next frame depending on play_speed
 * @return next frame or -1 if end of log reached
 */
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

/**
 * @brief get next frame without actually going to next frame
 * @return
 */
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

/**
 * @brief Return play_speed
 * @return play_speed
 */
double Log_Control::get_play_speed()
{
    return play_speed;
}

// Slots

/**
 * @brief set speed to 5
 */
void Log_Control::log_forward()
{
    play_speed = 5.;
    update_play_speed();
    enable_log_frameNumber(false);
}

/**
 * @brief set speed to -5
 */
void Log_Control::log_backward()
{
    play_speed = -5.;
    update_play_speed();
    enable_log_frameNumber(false);
}

/**
 * @brief set speed to saved speed or 1
 */
void Log_Control::log_play()
{
    if ( play_speed != 0. )
        play_speed = play_speed_save;
    else
        play_speed = 1.;
    update_play_speed();
    enable_log_frameNumber(false);
}

/**
 * @brief load saved speed, even if it is zero
 */
void Log_Control::log_resume()
{
	play_speed = play_speed_save;
    update_play_speed();
    enable_log_frameNumber(false);
}

/**
 * @brief pause play and save speed
 */
void Log_Control::log_pause()
{
    play_speed_save = play_speed;
    play_speed = 0.;
    update_play_speed();
    enable_log_frameNumber(true);
}

/**
 * @brief increase speed
 */
void Log_Control::log_faster()
{
    play_speed *= 2.;
    update_play_speed();
}

/**
 * @brief decrease speed
 */
void Log_Control::log_slower()
{
    play_speed /= 2.;
    update_play_speed();
}

/**
 * @brief decrease frame
 */
void Log_Control::log_frame_back()
{
    goto_frame(current_frame-10);
}

/**
 * @brief increase frame
 */
void Log_Control::log_frame_forward()
{
	goto_frame(current_frame+10);
}

/**
 * @brief set frame and next frame
 * @param f frame number
 */
void Log_Control::goto_frame ( int f )
{
    current_frame = f;
    next_frame = f + 1;
}

/**
 * @brief transform speed into string and show it on GUI
 */
void Log_Control::update_play_speed()
{
    double speed_tmp = floor ( play_speed * 100.0 + 0.5 ) / 100.0;
    std::ostringstream o;
    o << speed_tmp;
    QString speed = QString::number ( play_speed, 'g', 4 );

    speed.append ( "x" );
    emit update_speed ( speed );
}
