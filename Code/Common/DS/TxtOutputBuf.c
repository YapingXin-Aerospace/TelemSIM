#include <stdlib.h>
#include <string.h>
#include "TxtOutputBuf.h"


HResult TxtOutputBuf_Init(TxtOutputBuf* outbuf, const uint32_t capacity)
{
    HResult retcode = HResult_OK;

    if (outbuf == NULL || capacity == 0)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    outbuf->Capacity = capacity;
    outbuf->Buf = malloc((outbuf->Capacity + 1) * sizeof(char));
    if (!outbuf->Buf)
    {
        retcode = HResult_ALLOC_FAIL;
        goto EXIT;
    }
    memset(outbuf->Buf, 0, (outbuf->Capacity + 1) * sizeof(char));
    outbuf->Length = 0;

EXIT:
    return retcode;
}


HResult TxtOutputBuf_Append(TxtOutputBuf* outbuf, const char* txt, const uint32_t txtlen, void* data)
{
    HResult retcode = HResult_OK;
    char* dst = NULL;

    if (outbuf == NULL)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    if (txt == NULL || txtlen == 0)
    {
        goto EXIT;
    }

    if (txtlen > outbuf->Capacity)
    {
        retcode = HResult_PARAM_OUTRANGE;
        goto EXIT;
    }

    if (outbuf->Length + txtlen >= outbuf->Capacity)
    {
        if (outbuf->callback)
        {
            outbuf->callback(outbuf->Buf, outbuf->Length, data);
        }

        outbuf->Buf[0] = '\0';
        outbuf->Length = 0;
    }

    dst = &outbuf->Buf[outbuf->Length];
    memcpy(dst, txt, txtlen);

    outbuf->Length += txtlen;
    outbuf->Buf[outbuf->Length] = '\0';

EXIT:
    return retcode;
}


void TxtOutputBuf_Cleanup(TxtOutputBuf* outbuf, void* data)
{
    if (outbuf)
    {
        if (outbuf->Buf)
        {
            if (outbuf->callback != NULL && outbuf->Length > 0)
            {
                outbuf->callback(outbuf->Buf, outbuf->Length, data);
            }

            free(outbuf->Buf);
            outbuf->Buf = NULL;
        }
        outbuf->Capacity = 0;
        outbuf->Length = 0;
        outbuf->callback = NULL;
    }
}
