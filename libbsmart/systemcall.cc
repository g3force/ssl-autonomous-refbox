#include "systemcall.h"
#include <time.h>
#include <sys/timeb.h>

namespace BSmart
{
   

    long long Systemcall::get_current_system_time()
    {
        timeb sysTime;
        ftime(&sysTime);
        return static_cast<long long>(sysTime.time) * 1000 + static_cast<long long>(sysTime.millitm);
    }

    long long Systemcall::get_time_since(long long time_stamp)
    {
        return (get_current_system_time() - time_stamp);
    }




}
