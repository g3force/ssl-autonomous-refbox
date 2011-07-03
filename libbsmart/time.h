//
// C++ Interface: timers
//
// Description: 
//
//
// Author: bsmart <grp-bsmarter@informatik.uni-bremen.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BSMARTTIMERS_H
#define BSMARTTIMERS_H

#include <map>
#include <string>
#include <stdint.h>

#include <sys/time.h>
#include <ctime>
#include <iostream>

#define MYGETTIME(...) BSmart::Time::get_system_time()
//#define MKTIMER(d,n)  BSmart::Time::Timer d (n);

namespace BSmart {
    namespace Time {


typedef int64_t bs_time_t;

inline double get_system_time_s_d()
{
    timeval tv1;
    gettimeofday(&tv1, NULL);
    return ( static_cast<double>(tv1.tv_sec) + tv1.tv_usec*0.000001);
}


inline double get_system_time_d()
{
    timeval tv1;
    gettimeofday(&tv1, NULL);
    return ( static_cast<double>(tv1.tv_sec) * 1000.0 + tv1.tv_usec*0.001);
}


inline bs_time_t get_system_time()
{
    timeval tv1;
    gettimeofday(&tv1, NULL);
    return ( static_cast<bs_time_t>(tv1.tv_usec)/1000ll)
            +
            (static_cast<bs_time_t>(tv1.tv_sec)*1000ll );
}


/**
 * Required by the XABSL engine
 */
unsigned get_system_time_uint();

inline int usleep(long int t)
{
    struct timespec ts = {0,t*1000};
    return nanosleep(&ts,NULL);
}


inline bs_time_t get_time_since(bs_time_t t) {return (BSmart::Time::get_system_time() - t);}


struct Timer
{
    Timer() : tname("unnamed"),tstart(0), tend(0), tpaused(false),tstopped(false) { }
    Timer(const std::string& n) : tname(n),tstart(0), tend(0), tpaused(false),tstopped(false) { }
    ~Timer() { }

    bs_time_t parttime() { return (MYGETTIME() - tstart); }


    inline void start() { tstart = MYGETTIME() + tpaused*(tstart - tend); }


    void reset() { tstart = 0; tend=0; tpaused = false; tstopped = true; }


    bs_time_t stop()
    {
        tend = MYGETTIME();
        tstopped = true;
        return (tend - tstart);
    }


    std::ostream& printme(std::ostream&);
    std::ostream& plotme(std::ostream&);

    std::string tname;
    bs_time_t   tstart;
    bs_time_t   tend;
    bool tpaused;
    bool tstopped;
};


inline void stopPrintTimer(Timer& t,const std::string& n="name")
{   printf("Time: %s = %lld\n",n.c_str(), t.stop()); }

/**
 * Implements some timer functionality
 *
 * @author Teodosiy Kirilov <grp-bsmarter@informatik.uni-bremen.de>
 */
class TimerList
{
  public:
    TimerList();
    ~TimerList();

    std::ostream& printme(std::ostream&);
    std::ostream& plotme(std::ostream&);
    std::string   get_title_row();

    bool remove(const std::string& name);

    void add(const std::string& name);
    inline void start(const std::string& name) {  tmap[name].start(); }
    inline bs_time_t stop(const std::string& name) { return tmap[name].stop(); }
    bs_time_t pause(const std::string& name);
    void reset(const std::string& name);

    bs_time_t parttime(const std::string& name);

    void remove_all();
    
    /**
     * \note Don't add elements to the list after
     *       using this function !!!
     * @param n 
     * @return Pointer to an object, stored in an STL Container!!!!
     */
    Timer& get(const std::string n) { return tmap[n]; }

  private:
    std::map<std::string, Time::Timer> tmap;

};



    } // namespace Timers
} // namespace BSmart

#endif
