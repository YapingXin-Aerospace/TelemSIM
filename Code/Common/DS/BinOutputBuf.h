#ifndef _INC_COMMON_DS_BinOutputBuf_H
#define _INC_COMMON_DS_BinOutputBuf_H

#include "Common/datatypes.h"

typedef HResult(*BinOutputBuf_FlushHandle)(const byte* buf, const size_t len, void* entiry);

typedef struct
{
    size_t Capacity;  // The maximum bytes count to storage in the buffer `Buf`.
    size_t Trig_Len;  // The critical point. While `Count` reach `Trig_Len`, Flush function shall be called with callback function been executed.
    size_t Count;     // Actual bytes count storaged in the buffer `Buf`.
    BinOutputBuf_FlushHandle callback;
    byte* Buf;

} BinOutputBuf;


/*
 * Initialize the BinOutputBuf instance with given `Capacity` and `Trig_Len` parameters.
 */
HResult BinOutputBuf_Init(BinOutputBuf* outbuf, const size_t capacity, const size_t trig_len);

/*
 * Append bytes content at the end of storage buffer. If `Count` reach `Trig_Len`, the Flush function 
 * shall be called with callback function been executed.
 */
HResult BinOutputBuf_Append(BinOutputBuf* outbuf, const byte* buf, const size_t len, void* entity);

/*
 * If `Count` > 0 then execute the callback function. Then cleanup the storage buffer and reset `Count` to be Zero.
 */
HResult BinOutputBuf_Flush(BinOutputBuf* outbuf, void* entity);

/*
 * Release the BinOutputBuf instance. Flush function shall be called if there is some content remain in storage buffer.
 */
void BinOutputBuf_Release(BinOutputBuf* outbuf, void* entity);


#endif // !_INC_COMMON_DS_BinOutputBuf_H
