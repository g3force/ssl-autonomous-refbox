#include "timer.h"

#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

using namespace BSmart;

Timer::Timer( ) 
    : status(STOP), time_elapsed(0)
{
    tests = 1;
    act_test = 0;
    test_min = 0xFFFFFFFF;
    test_max = 0;
    test_avg = 0;
    test_last = 0;
    start_time = 0;
}

Timer::Timer( unsigned int tests )
    : status(STOP), time_elapsed(0)
{
    this->tests = tests;
    act_test = 0;
    test_min = 0xFFFFFFFF;
    test_max = 0;
    test_avg = 0;
    test_last = 0;
    start_time = 0;
}

void Timer::start()
{
    start_time = BSmart::Systemcall::get_current_system_time();
    time_elapsed = 0;
    status = ACTIVE;
}

void Timer::pause()
{
    if (status == ACTIVE)
    {
        time_elapsed += get();
        status = PAUSE;
    }
}

void Timer::unpause()
{
    if (status == PAUSE)
    {
        start_time = BSmart::Systemcall::get_current_system_time();
        status = ACTIVE;
    }
}

unsigned int Timer::get()
{
    return BSmart::Systemcall::get_current_system_time() - start_time;
}

unsigned int Timer::get_overall()
{
   if (status == PAUSE)
       return time_elapsed;
   else if(start_time) //timer bereits gestartet
       return BSmart::Systemcall::get_current_system_time() - start_time + time_elapsed;
   else
       return 0;
}

unsigned int Timer::get_last()
{
    return test_last;
}

unsigned int Timer::get_average()
{
    return static_cast<unsigned int>(old_avg);
}

unsigned int Timer::get_highest()
{
    return old_max;
}

unsigned int Timer::get_lowest()
{
    return old_min;
}

void Timer::end()
{
    status = STOP;
    //Get system time and compare with starttime
    long long tmp_timediff = BSmart::Systemcall::get_current_system_time() - start_time;
    tmp_timediff += time_elapsed;
    act_test++;
    //calculate new average value
    test_avg = (test_avg*(act_test-1) + tmp_timediff)/act_test;
    //New highest/lowest values?
    test_max = (test_max<tmp_timediff)?tmp_timediff:test_max;
    test_min = (test_min>tmp_timediff)?tmp_timediff:test_min;
    test_last = tmp_timediff;
    if (act_test >= tests)
    {
        //Reset values
        old_avg = test_avg;
        old_max = test_max;
        old_min = test_min;
        act_test = 0;
        test_min = 0xFFFFFFFF;
        test_max = 0;
        test_avg = 0;
    }
}

std::string Timer::humanReadable(unsigned int timeval){
    std::stringstream string_temp;
    string_temp << std::setfill('0') << std::setw(2) << static_cast<int>(timeval / 60000.0) % 60;
    string_temp << ":";
    string_temp << std::setfill('0') << std::setw(2) << static_cast<int>(timeval / 1000.0) % 60;
    string_temp << ":";
    string_temp << std::setfill('0') << std::setw(2) << static_cast<int>(timeval) % 60; 
    return string_temp.str();
}



Timer* Timer::load(unsigned int trigger_time)
{
    this->start();
    this->trigger_time = trigger_time;
    return this;
}



unsigned int Timer::left()
{
    if(status == STOP)
    {
        return 0;
    }
    else
    {
        unsigned int elapsed = get_overall();

        //beware the unsigned
        return (elapsed >= trigger_time) ? 0 : trigger_time - elapsed;
    }
}



unsigned int Timer::get_trigger_time() const
{
    return trigger_time;
}
