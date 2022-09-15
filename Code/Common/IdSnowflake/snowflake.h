#ifndef SNOWFLAKE_H
#define SNOWFLAKE_H

#include <stdio.h>

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

#include <stdint.h>
#include <stdlib.h>

#define START_TIMESTAMP 1497453478000

#define SEQ_BIT 12
#define MACHINE_BIT 5
#define DATACENTER_BIT 5

#define MAX_DATACENTER_NUM ((1 << DATACENTER_BIT) - 1)
#define MAX_MACHINE_NUM ((1 << MACHINE_BIT) - 1)
#define MAX_SEQ_NUM ((1 << SEQ_BIT) - 1)

#define MACHINE_LEFT_OFFSET SEQ_BIT
#define DATACENTER_LEFT_OFFSET (SEQ_BIT + MACHINE_BIT)
#define TIMESTAMP_LEFT_OFFSET (DATACENTER_LEFT_OFFSET + DATACENTER_BIT)

//uint64_t snowflake_id();
uint64_t snowflake_timestamp();

typedef struct
{
    uint64_t last_timestamp;
    int datacenter;
    int machine;
    int seq;
} snowflake_st;

uint64_t snowflake_timestamp_id();

#endif
