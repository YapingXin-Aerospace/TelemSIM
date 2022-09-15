#include "snowflake.h"
#include "../common_functions.h"

static snowflake_st global_sf;
static uint8_t is_global_sf_set = 0;

static uint64_t snowflake_next_timestamp(uint64_t last_timestamp);

/* uint64_t snowflake_id(uint64_t *last_timestamp,int datacenter, int machine, int *seq) */
uint64_t snowflake_id(snowflake_st *snowflake_st)
{
    uint64_t current_timestamp = snowflake_timestamp();

    if (current_timestamp == snowflake_st->last_timestamp)
    {
        snowflake_st->seq = (snowflake_st->seq + 1) & MAX_SEQ_NUM;

        if (snowflake_st->seq == 0)
        {
            current_timestamp = snowflake_next_timestamp(snowflake_st->last_timestamp);
        }
    }
    else
    {
        snowflake_st->seq = 0;
    }

    snowflake_st->last_timestamp = current_timestamp;

    return (current_timestamp - START_TIMESTAMP) << TIMESTAMP_LEFT_OFFSET | snowflake_st->datacenter << DATACENTER_LEFT_OFFSET | snowflake_st->machine << MACHINE_LEFT_OFFSET | snowflake_st->seq;
}

uint64_t snowflake_timestamp_id()
{
    if (is_global_sf_set == 0)
    {
        global_sf.last_timestamp = snowflake_timestamp();
        global_sf.datacenter = 0x01010101;
        global_sf.machine = 0x01010101;
        global_sf.seq = 0;

        is_global_sf_set = 1;
    }

    snowflake_st* sf = &global_sf;

    uint64_t current_timestamp = snowflake_timestamp();
    
    if (current_timestamp == sf->last_timestamp)
    {
        sf->seq = (sf->seq + 1) & MAX_SEQ_NUM;

        if (sf->seq == 0)
        {
            current_timestamp = snowflake_next_timestamp(sf->last_timestamp);
        }
    }
    else
    {
        // sf->seq = 0;
        sf->seq = (sf->seq + 1) & MAX_SEQ_NUM;
    }

    sf->last_timestamp = current_timestamp;

    return (current_timestamp - START_TIMESTAMP) << TIMESTAMP_LEFT_OFFSET | (uint64_t)(sf->datacenter << DATACENTER_LEFT_OFFSET) | (uint64_t)(sf->machine << MACHINE_LEFT_OFFSET) | sf->seq;
}

uint64_t snowflake_timestamp() 
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (uint64_t)tv.tv_sec * 1000 + (uint64_t)tv.tv_usec / 1000;
}

static uint64_t snowflake_next_timestamp(uint64_t last_timestamp)
{
    uint64_t cur;
    do {
        cur = snowflake_timestamp();
    } while (cur <= last_timestamp);
    return cur;
}
