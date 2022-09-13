#include <stdlib.h>
#include <memory.h>
#include "BinOutputBuf.h"


/*
 * Initialize the BinOutputBuf instance with given `Capacity` and `Trig_Len` parameters.
 */
HResult BinOutputBuf_Init(BinOutputBuf* outbuf, const size_t capacity, const size_t trig_len)
{
    HResult retcode = HResult_OK;

    if (outbuf == NULL || capacity == 0)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    outbuf->Capacity = capacity;
    outbuf->Trig_Len = trig_len;
    outbuf->Count = 0;

    outbuf->Buf = malloc(outbuf->Capacity * sizeof(byte));
    if (!outbuf->Buf)
    {
        retcode = HResult_ALLOC_FAIL;
        goto EXIT;
    }
    memset(outbuf->Buf, 0, outbuf->Capacity * sizeof(byte));

EXIT:
    return retcode;
}


/*
 * Append bytes content at the end of storage buffer. If `Count` reach `Trig_Len`, the Flush function
 * shall be called with callback function been executed.
 */
HResult BinOutputBuf_Append(BinOutputBuf* outbuf, const byte* buf, const size_t len, void* entity)
{
    HResult retcode = HResult_OK;
    byte* dst = NULL;

    if (outbuf == NULL)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    if (buf == NULL || len == 0)
    {
        goto EXIT;
    }

    if (outbuf->Count + len > outbuf->Capacity)
    {
        retcode = HResult_PARAM_OUTRANGE;
        goto EXIT;
    }

    dst = &outbuf->Buf[outbuf->Count];
    memcpy(dst, buf, len);
    outbuf->Count += len;

    if (outbuf->Count >= outbuf->Trig_Len)
    {
        retcode = BinOutputBuf_Flush(outbuf, entity);
    }

EXIT:
    return retcode;
}


/*
 * If `Count` > 0 then execute the callback function. Then cleanup the storage buffer and reset `Count` to be Zero.
 */
HResult BinOutputBuf_Flush(BinOutputBuf* outbuf, void* entity)
{
    HResult retcode = HResult_OK;

    if (outbuf == NULL)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    if (outbuf->Count > 0)
    {
        if (outbuf->callback)
        {
            retcode = outbuf->callback(outbuf->Buf, outbuf->Count, entity);
        }

        memset(outbuf->Buf, 0, outbuf->Capacity * sizeof(byte));
        outbuf->Count = 0;
    }

EXIT:
    return retcode;
}


/*
 * Release the BinOutputBuf instance. Flush function shall be called if there is some content remain in storage buffer.
 */
void BinOutputBuf_Release(BinOutputBuf* outbuf, void* entity)
{
    if (outbuf)
    {
        BinOutputBuf_Flush(outbuf, entity);

        if (outbuf->Buf)
        {
            free(outbuf->Buf);
            outbuf->Buf = NULL;
        }
        outbuf->Capacity = 0;
        outbuf->Trig_Len = 0;
    }
}
