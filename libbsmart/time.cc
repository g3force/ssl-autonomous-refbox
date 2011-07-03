#include "time.h"
#include <iostream>

using namespace BSmart::Time;


TimerList::TimerList()
    : tmap()
{
}


TimerList::~TimerList()
{
}


std::ostream& Timer::printme(std::ostream& os)
{
    os<< tname << "( "
#ifdef BSMART_VERBOSE_TIMERS
            << tstart <<'-'
            << tend << '='
#endif
            << parttime() << " )";
    return os;
}

std::ostream& Timer::plotme(std::ostream& os)
{
    os << parttime() << ' ';
    return os;
}


std::ostream& TimerList::printme(std::ostream& os)
{
    bool foo = true;
    std::map<std::string,Timer>::iterator i = tmap.begin();
    for(; i!=tmap.end(); ++i)
    {
        Timer& t = i->second;
        //if(!t.tstopped) continue;
        os << "Timer: ";
        t.printme(os) << "\n";
           //<< (foo ? ' ':'\n');
        foo = !foo;
    }
    return os;
}


std::ostream& TimerList::plotme(std::ostream& os)
{
    std::map<std::string,Timer>::iterator i = tmap.begin();
    for(; i!=tmap.end(); ++i)
    {
        Timer& t = i->second;
        t.plotme(os);
    }
    os << '\n';
    return os;
}


std::string   TimerList::get_title_row()
{
    std::string rv("#");
    std::map<std::string,Timer>::iterator i = tmap.begin();
    for(; i!=tmap.end(); ++i)
    {
        rv += i->second.tname + '\t';
    }
    rv += '\n';
    return rv;
}


bool TimerList::remove(const std::string& name)
{
    std::map<std::string,Timer>::iterator i = tmap.find(name);
    if( i == tmap.end() ) return false;
    tmap.erase(i);
    return true;

}


void TimerList::add(const std::string& name)
{
    Timer& t = tmap[name];
    t.tname = name;
    t.reset();
    //return t;
}


BSmart::Time::bs_time_t TimerList::pause(const std::string& name)
{
    Timer& t = tmap[name];
    t.tend = MYGETTIME();
    t.tpaused = true;
    return (t.tend - t.tstart);
}



void TimerList::reset(const std::string& name)
{
    tmap[name].reset();
}


BSmart::Time::bs_time_t TimerList::parttime(const std::string& name)
{
    return tmap[name].parttime();
}


void TimerList::remove_all()
{
    std::map<std::string,Timer>::iterator i = tmap.begin();
    for(; i!=tmap.end(); ++i)
    {
        tmap.erase(i);
    }
}


/* 
 * Required by the XABSL Engine
 * Since a reference to the function is used, it
 * may not be inlined!
 * FIXME some reasonable rounding to reduce overflow errors
 */
unsigned BSmart::Time::get_system_time_uint()
{
    return static_cast<unsigned>(get_system_time());
}
