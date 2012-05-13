/**
 * @file log_control.h
 * @brief Log_Control header file
 */
#ifndef LOG_CONTROL_H
#define LOG_CONTROL_H

#include <QObject>
#include <iostream>

/**
 * @class Log_Control
 * @brief Handle frames of log file and speed
 * Has nothing to do with actual log file
 */
class Log_Control : public QObject
{
    Q_OBJECT

public:
    Log_Control();
    ~Log_Control();

    void reset(int);
    int get_current_frame();
    int get_next_frame();
    int get_prop_next_frame();
    double get_play_speed();

public slots:
    void log_forward();
    void log_backward();
    void log_play();
    void log_resume();
    void log_pause();
    void log_faster();
    void log_slower();
    void log_frame_forward();
    void log_frame_back();
    void goto_frame(int);

signals:
    void update_speed(QString);
    void enable_log_frameNumber(bool);

private:
    int current_frame;
    int next_frame;
    int log_length;
    double play_speed;
    double play_speed_save;
    void update_play_speed();

};

#endif //LOG_CONTROL_H
