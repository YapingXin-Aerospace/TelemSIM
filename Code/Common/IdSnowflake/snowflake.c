#include "snowflake.h"
#include "../common_functions.h"

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

// IdWorker Struct
struct IdWorker {
    int64_t worker_id;
    int64_t last_time_stamp;
    unsigned int sequence;
};

typedef struct IdWorker id_worker;

// Reference: https://github.com/52fhy/snowflake-c


//wait until the next millisecond, regenerate the timestamp 
int64_t time_re_gen(int64_t last) {
    struct timeval tv;
    int64_t new_time;

    do {
        gettimeofday(&tv, NULL);
        new_time = (int64_t)tv.tv_sec * 1000 + (int64_t)tv.tv_usec / 1000;
    } while (new_time <= last);

    return new_time;
}

//get the current timestamp 
int64_t time_gen() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (int64_t)tv.tv_sec * 1000 + (int64_t)tv.tv_usec / 1000;
}

int64_t next_id() {
    static id_worker iw = { 0,0,0 };

    int64_t ts, id;
    ts = time_gen();

    //multiple IDs generated in the same millisecond
    if (ts == (iw.last_time_stamp)) {
        iw.sequence = (iw.sequence + 1) & MASK_SEQUENCE;

        //The serial number in the same millisecond is used up, waiting until the next millisecond
        if (iw.sequence == 0) {
            ts = time_re_gen(ts);
        }
    }
    else {
        iw.last_time_stamp = ts;
    }

    //当前时间戳比上一次时间还小，说明出错了 
    if (ts < (iw.last_time_stamp)) {
        id = 0;
    }
    else {
        id = (ts - EPOCH) << (NODEBITS + STEPBITS) |
            iw.worker_id << STEPBITS |
            iw.sequence;
    }

    return id;
}
