#ifndef SYSTEMCALL_H
#define SYSTEMCALL_H
#include <sys/time.h>
namespace BSmart
{
    typedef long long Time_Value;

    class Systemcall
    {
    public:
        // Die Aktuelle Zeit in Millisekunden
        static Time_Value get_current_system_time();
   
        // Vergangene Zeit seit time_stamp in Millisekunden
        static Time_Value get_time_since(Time_Value time_stamp);

        inline static double get_timef()
        {
          timeval tv;
          gettimeofday(&tv,0);
          return static_cast<double>(tv.tv_sec)*1000.0 + 0.001 * static_cast<double>(tv.tv_usec);
        }

        inline static double get_time_sincef(const double& utime)
        {
          return ( Systemcall::get_timef() - utime );
        }
   
    };
}

#endif
