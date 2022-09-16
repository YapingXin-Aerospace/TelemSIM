#include "DRV_StepLog.h"
#include "DRV_Internal.h"


HResult DRV_StepLog_RetrieveSteps()
{
    HResult retcode = HResult_OK;
    sqlite3* dbconn = NULL;

    retcode = sqlite3_dbopen(&dbconn);
    if (retcode != HResult_OK)
    {
        goto EXIT;
    }

//EXIT_DB:
    sqlite3_helper_close(&dbconn);
    
EXIT:
    return retcode;
}
