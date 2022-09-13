#ifndef _INC_COMMON_DS_RefBlob_H
#define _INC_COMMON_DS_RefBlob_H

#include "Common/datatypes.h"

typedef struct
{
    uint32_t length;
    byte* buffer;

} RefBlob;


HResult RefBlob_InitByBytes(RefBlob* blob, const byte* _buffer, const uint32_t _length);
HResult RefBlob_InitByLength(RefBlob* blob, const uint32_t _length);
void RefBlob_Release(RefBlob* blob);

#endif // !_INC_COMMON_DS_RefBlob_H