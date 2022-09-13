#include <stdlib.h>
#include <memory.h>
#include "RefBlob.h"


HResult RefBlob_InitByBytes(RefBlob* blob, const byte* _buffer, const uint32_t _length)
{
    HResult retcode = HResult_OK;

    if (blob == NULL)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    if (_length > 0 && _buffer == NULL)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    blob->length = _length;

    if (blob->length > 0)
    {
        blob->buffer = malloc(blob->length * sizeof(byte));
        if (!blob->buffer)
        {
            retcode = HResult_ALLOC_FAIL;
            goto EXIT;
        }
        memset(blob->buffer, 0, blob->length * sizeof(byte));
    }

    memcpy(blob->buffer, _buffer, blob->length);

EXIT:
    return retcode;
}


HResult RefBlob_InitByLength(RefBlob* blob, const uint32_t _length)
{
    HResult retcode = HResult_OK;

    if (blob == NULL)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    blob->length = _length;

    if (blob->length > 0)
    {
        blob->buffer = malloc(blob->length * sizeof(byte));
        if (!blob->buffer)
        {
            retcode = HResult_ALLOC_FAIL;
            goto EXIT;
        }
        memset(blob->buffer, 0, blob->length * sizeof(byte));
    }

EXIT:
    return retcode;
}


void RefBlob_Release(RefBlob* blob)
{
    if (blob)
    {
        if (blob->buffer)
        {
            free(blob->buffer);
            blob->buffer = NULL;
        }
        blob->length = 0;
    }
}
