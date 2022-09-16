#include "DRV_StepLog.h"
#include "DRV_Internal.h"


HResult DRV_StepLog_RetrieveSteps(StepLogSet* _set)
{
    const char* sql_Count = "SELECT COUNT(*) N FROM StepLog;";

    HResult retcode = HResult_OK;
    sqlite3* dbconn = NULL;
    int32_t i32 = 0;

    if (_set == NULL)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    retcode = sqlite3_dbopen(&dbconn);
    if (retcode != HResult_OK)
    {
        goto EXIT;
    }

    retcode = sqlite3_helper_query_int32(&i32, dbconn, sql_Count);
    if (retcode != HResult_OK)
    {
        goto EXIT_DB;
    }

    if (i32 < 0 || i32 > UINT16_MAX)
    {
        retcode = HResult_DATA_INVALID;
        goto EXIT_DB;
    }

    retcode = StepLogSet_InitByCount(_set, (const uint16_t)i32);
    if (retcode != HResult_OK)
    {
        goto EXIT_DB;
    }

    if (_set->count > 0)
    {
    }

EXIT_DB:
    sqlite3_helper_close(&dbconn);
    
EXIT:
    return retcode;
}
