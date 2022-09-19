#include "DRV_StepLog.h"
#include "DRV_Internal.h"

// SQL: SELECT StepId, PhaseId, ToSkip, RetCode, StepName, Desp FROM StepLog;
static HResult callback_RetrieveFormulaStep(sqlite3_stmt* stmt, const size_t rowIndex, void* p_StepLogSet);


HResult DRV_StepLog_RetrieveSteps(StepLogSet* _set)
{
    const char* sql_Count = "SELECT COUNT(*) N FROM StepLog;";
    const char* sql_Items = "SELECT StepId, PhaseId, ToSkip, RetCode, StepName, Desp FROM StepLog;";

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
        retcode = sqlite3_helper_query_callback(dbconn, sql_Items, callback_RetrieveFormulaStep, _set);
        if (retcode != HResult_OK)
        {
            goto EXIT;
        }
    }

EXIT_DB:
    sqlite3_helper_close(&dbconn);
    
EXIT:
    return retcode;
}


// SQL: SELECT StepId, PhaseId, ToSkip, RetCode, StepName, Desp FROM StepLog;
static HResult callback_RetrieveFormulaStep(sqlite3_stmt* stmt, const size_t rowIndex, void* p_StepLogSet)
{
    HResult retcode = HResult_OK;
    const unsigned char* retrieve_text = NULL;
    size_t text_length;

    if (p_StepLogSet == NULL)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    StepLogSet* _set = (StepLogSet*)p_StepLogSet;
    StepLogItem* item = &_set->items[rowIndex];

    item->StepId = (uint16_t)sqlite3_column_int(stmt, 0);
    item->PhaseId = (uint8_t)sqlite3_column_int(stmt, 1);
    item->ToSkip = (uint8_t)sqlite3_column_int(stmt, 2);
    item->RetCode = (uint32_t)sqlite3_column_int64(stmt, 3);

    retrieve_text = sqlite3_column_text(stmt, 4);
    if (retrieve_text != NULL)
    {
        text_length = strlen(retrieve_text);
        if (text_length >= ShortNameTXT_MaxSIZE)
        {
            retcode = HResult_DATA_OUTRANGE;
            goto EXIT;
        }

        strcpy(item->StepName, retrieve_text);
    }

    retrieve_text = sqlite3_column_text(stmt, 5);
    if (retrieve_text != NULL)
    {
        text_length = strlen(retrieve_text);
        if (text_length >= DescriptionTxtMaxSIZE)
        {
            retcode = HResult_DATA_OUTRANGE;
            goto EXIT;
        }

        strcpy(item->Desp, retrieve_text);
    }

EXIT:
    return retcode;
}
