#ifndef _INC_COMMON_DS_TxtOutputBuf_H
#define _INC_COMMON_DS_TxtOutputBuf_H

#include "Common/datatypes.h"

typedef HResult(*TxtOutputBuf_FlushHandle)(const char* buf, const uint32_t len, void* data);

typedef struct
{
    uint32_t Capacity;
    uint32_t Length;
    TxtOutputBuf_FlushHandle callback;
    char* Buf;

} TxtOutputBuf;


HResult TxtOutputBuf_Init(TxtOutputBuf* outbuf, const uint32_t capacity);
HResult TxtOutputBuf_Append(TxtOutputBuf* outbuf, const char* txt, const uint32_t txtlen, void* data);
void TxtOutputBuf_Cleanup(TxtOutputBuf* outbuf, void* data);

#endif // !_INC_COMMON_DS_TxtOutputBuf_H
