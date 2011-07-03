#ifndef TIMER_H_
#define TIMER_H_

#include "sys/timeb.h"
#include "systemcall.h"
#include <string> 

namespace BSmart{

enum Timer_Status{ACTIVE,PAUSE,STOP};

class Timer
{
public:
    /**
     * one test can be performed before timer is resetted
     */
    Timer();
    /**
     * @param tests is the number of tests that should be performed until resetting all values and start new measurement
     */
    Timer( unsigned int tests );
    
    /**
     * @return time since last unpausing
     */
    unsigned int get();
     
   /**
     * Starts the timer with the given amount of milliseconds so it returns false
     * before this time has elapsed and true afterwards. Returns itself so it can be
     * used while object construction
     */
    Timer* load(unsigned int milliseconds);
    
    /**
     * @return time until load-interval will elapse and -1 if not loaded
     */
    unsigned int left();
    
    /**
     * start time measurement
     */
    void start();
    
    /**
     * pause measurement
     */
    void pause();
    
    /**
     * continue measurement
     */
    void unpause();
    
    /* returns the time since last unpausing but includes all time elapsed before
     * last pausing
     */
    unsigned int get_overall();
    /**
     * stops time measurement and writes the total time difference since first
     * start and also all other values like maximum or average taking into account
     * the end time of the end() execution.
     * If end() is called again afterwards it populates the same values but uses
     * the actual time of this execution as end time.
     * 
     * once end() is called, the timer cannot be paused again until it is resetted
     * or started again.
     */
    void end();
    /**
     * @return average value of last test row
     */
    unsigned int get_average();
    /**
     * @return lowest value of last test row
     */
    unsigned int get_lowest();
    /**
     * @return highest value of last test row
     */
    unsigned int get_highest();
    /**
     * @return the time value last measured by a call to timer.end()
     */
    unsigned int get_last();
    
    Timer_Status status;
    
    /**
     * @param timeval a timer based number of milliseconds
     * @return a human readable time value like xx:xx:xx
     */
    static std::string humanReadable(unsigned int timeval);
    
    unsigned int get_trigger_time() const;
    
private:
    long long start_time;
    /**
     * elapsed time is stored here upon pausing the timer
     */
    long long time_elapsed;
    /**
     * the time value last measured by a call to timer.end()
     */
    long long test_last;
    unsigned int act_test;
    long long test_max, test_min;
    /**
     * timer.end() can be executed test times before the timer is resetted
     */
    unsigned int tests;
    double test_avg;
    double old_avg;
    long long old_max;
    long long old_min;
    
    unsigned int trigger_time; //!< milliseconds after which left returns 0
};

} //namespace BSmart

#endif
