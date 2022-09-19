#include "Func_PreLoad.h"
#include "Common/TMLib/TM_PreLoad.h"
#include "App/SQLiteTools/DRV_StepLog.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void Debug_PrintStepLogSet(StepLogSet* _set);


HResult UtilityPreload_Load()
{
    HResult retcode = HResult_OK;
    TelemSIMConfig* config = get_G_TelemSIMConfig();

    if (config == NULL)
    {
        retcode = HResult_CONFIG_FAIL;
        goto EXIT;
    }

    StepLogSet* _StepLogSet = get_global_StepLogSet();
    retcode = DRV_StepLog_RetrieveSteps(_StepLogSet);
    if (retcode != HResult_OK)
    {
        goto EXIT;
    }

    Debug_PrintStepLogSet(_StepLogSet);

EXIT:
    return retcode;
}


void UtilityPreload_Release()
{
    StepLogSet* _StepLogSet = get_global_StepLogSet();
    StepLogSet_Release(_StepLogSet);
}


static void Debug_PrintStepLogSet(StepLogSet* _set)
{
    if (_set)
    {
        printf("[StepLogSet] Count: %d >>>\n", _set->count);
        if (_set->count > 0)
        {
            for (uint16_t idx = 0; idx < _set->count; idx++)
            {
                StepLogItem* item = &_set->items[idx];
                printf("[StepLogItem] StepId: %d, %s\n", item->StepId, item->StepName);
            }
        }
        printf("[StepLogSet] <<<\n");
    }
}

