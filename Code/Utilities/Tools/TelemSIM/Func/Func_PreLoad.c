#include "Func_PreLoad.h"
#include "Common/TMLib/TM_PreLoad.h"
#include "App/SQLiteTools/DRV_StepLog.h"


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

EXIT:
    return retcode;
}


void UtilityPreload_Release()
{
    StepLogSet* _StepLogSet = get_global_StepLogSet();
    StepLogSet_Release(_StepLogSet);
}
