#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include "TM_Common.h"


HResult StepLogSet_InitByCount(StepLogSet* _set, const uint16_t _count)
{
    HResult retcode = HResult_OK;

    if (_set == NULL)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    _set->count = _count;
    if (_set->count > 0)
    {
        _set->items = malloc(_set->count * sizeof(StepLogItem));
        if (!_set->items)
        {
            retcode = HResult_ALLOC_FAIL;
            goto EXIT;
        }
        memset(_set->items, 0, _set->count * sizeof(StepLogItem));
    }
    else
    {
        _set->items = NULL;
    }

EXIT:
    return retcode;
}

void StepLogSet_Release(StepLogSet* _set)
{
    if (_set)
    {
        if (_set->items)
        {
            free(_set->items);
            _set->items = NULL;
        }
        _set->count = 0;
    }
}
