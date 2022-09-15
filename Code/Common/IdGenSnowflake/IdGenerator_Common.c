#include "IdGenerator_Common.h"

#if ( defined _WIN32 )
#include <windows.h>
#include <share.h>
#include <io.h>
#include <fcntl.h>
#elif ( defined __unix ) || ( defined _AIX ) || ( defined __linux__ ) || ( defined __hpux )
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <syslog.h>
#include <pthread.h>
#endif


int64_t GetCurrentTime()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((int64_t)(tv.tv_sec)) * 1000 + tv.tv_usec / 1000;

    //static struct timeb t1;
    //    ftime(&t1);
    //    return (uint64_t) ((t1.time * 1000 + t1.millitm));
}
