#ifndef SNOWFLAKE_H
#define SNOWFLAKE_H

#include "../datatypes.h"


//2^12 -1 = 4095
#define MASK_SEQUENCE 4095

// Epoch is set to the twitter snowflake epoch of Nov 04 2010 01:42:54 UTC in milliseconds
#define EPOCH 1288834974657

// NodeBits holds the number of bits to use for Node
#define NODEBITS 10

// StepBits holds the number of bits to use for Step
#define STEPBITS 12

int64_t time_re_gen(int64_t);
int64_t time_gen();
int64_t next_id();

#endif // !SNOWFLAKE_H
