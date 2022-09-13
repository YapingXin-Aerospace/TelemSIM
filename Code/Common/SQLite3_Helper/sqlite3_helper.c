#include "sqlite3_helper.h"
#include <stdio.h>
#include <string.h>

#include "../common_functions.h"

// Return value:
//		HResult_OK					- Success
//		HResult_FILE_NOTEXISTS		- File not exist
//		HResult_SQLite3OpenFail		- SQLite3 DB open failed
HResult sqlite3_helper_open(const char* db_path, sqlite3** p_db)
{
    HResult rc = HResult_OK;
    int sqlite_rc = 0;

    if (!is_file_exist(db_path))
    {
        rc = HResult_FILE_NOTEXISTS;
        goto EXIT;
    }

    sqlite_rc = sqlite3_open(db_path, p_db);
    if (sqlite_rc != SQLITE_OK)
    {
        rc = HResult_SQLite3OpenFail;
        goto EXIT;
    }

EXIT:
    return rc;
}


void sqlite3_helper_close(sqlite3** p_db)
{
    if (*p_db)
    {
        sqlite3_close(*p_db);
        *p_db = NULL;
    }
}


void sqlite3_helper_BeginTransaction(sqlite3* db)
{
    if (db)
    {
        sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
    }
}

void sqlite3_helper_COMMIT(sqlite3* db)
{
    if (db)
    {
        sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
    }
}


// Return value:
//		HResult_OK					- Success
//		HResult_SQLite3PrepareErr	- failure occurs when preparing the query statement.
//		HResult_SQLite3QueryNULL	- the query result is NULL
HResult sqlite3_helper_query_int32(int32_t* p_value, sqlite3* db, const char* sql_QueryValue)
{
    HResult ret_code = HResult_OK;
    int sqlite_rc = 0;
    sqlite3_stmt* res = NULL;

    sqlite_rc = sqlite3_prepare_v2(db, sql_QueryValue, -1, &res, 0);
    if (sqlite_rc != SQLITE_OK)
    {
        ret_code = HResult_SQLite3PrepareErr;
        goto EXIT;
    }

    sqlite_rc = sqlite3_step(res);
    if (sqlite_rc != SQLITE_ROW)
    {
        ret_code = HResult_SQLite3QueryNULL;
        goto EXIT_STMT;
    }

    *p_value = sqlite3_column_int(res, 0);

EXIT_STMT:
    sqlite3_finalize(res);

EXIT:
    return ret_code;
}


// Return value:
//		HResult_OK					- Success
//		HResult_SQLite3PrepareErr	- failure occurs when preparing the query statement.
//		HResult_SQLite3QueryNULL	- the query result is NULL
HResult sqlite3_helper_query_int64(int64_t* p_value, sqlite3* db, const char* sql_QueryValue)
{
    HResult ret_code = HResult_OK;
    int sqlite_rc = 0;
    sqlite3_stmt* res = NULL;

    sqlite_rc = sqlite3_prepare_v2(db, sql_QueryValue, -1, &res, 0);
    if (sqlite_rc != SQLITE_OK)
    {
        ret_code = HResult_SQLite3PrepareErr;
        goto EXIT;
    }

    sqlite_rc = sqlite3_step(res);
    if (sqlite_rc != SQLITE_ROW)
    {
        ret_code = HResult_SQLite3QueryNULL;
        goto EXIT_STMT;
    }

    *p_value = sqlite3_column_int64(res, 0);

EXIT_STMT:
    sqlite3_finalize(res);

EXIT:
    return ret_code;
}


// Return value:
//		HResult_OK					- Success
//		HResult_SQLite3PrepareErr	- failure occurs when preparing the query statement.
//		HResult_SQLite3QueryNULL	- the query result is NULL
HResult sqlite3_helper_query_double(double* p_value, sqlite3* db, const char* sql_QueryValue)
{
    HResult ret_code = HResult_OK;
    int sqlite_rc = 0;
    sqlite3_stmt* res = NULL;

    sqlite_rc = sqlite3_prepare_v2(db, sql_QueryValue, -1, &res, 0);
    if (sqlite_rc != SQLITE_OK)
    {
        ret_code = HResult_SQLite3PrepareErr;
        goto EXIT;
    }

    sqlite_rc = sqlite3_step(res);
    if (sqlite_rc != SQLITE_ROW)
    {
        ret_code = HResult_SQLite3QueryNULL;
        goto EXIT_STMT;
    }

    *p_value = sqlite3_column_double(res, 0);

EXIT_STMT:
    sqlite3_finalize(res);

EXIT:
    return ret_code;
}


// Return value:
//		HResult_OK					- Success
//		HResult_SQLite3PrepareErr	- failure occurs when preparing the query statement.
//		HResult_SQLite3QueryNULL	- the query result is NULL
//      HResult_OptSkip             - the query result is NULL
HResult sqlite3_helper_query_RefString(RefString* p_Str, sqlite3* db, const char* sql_QueryValue)
{
    HResult retcode = HResult_OK;
    int sqlite_rc = 0;
    sqlite3_stmt* res = NULL;
    const unsigned char* retrieve_text = NULL;

    sqlite_rc = sqlite3_prepare_v2(db, sql_QueryValue, -1, &res, 0);
    if (sqlite_rc != SQLITE_OK)
    {
        retcode = HResult_SQLite3PrepareErr;
        goto EXIT;
    }

    sqlite_rc = sqlite3_step(res);
    if (sqlite_rc != SQLITE_ROW)
    {
        retcode = HResult_SQLite3QueryNULL;
        goto EXIT_STMT;
    }

    if (sqlite3_column_type(res, 0) == SQLITE_NULL)
    {
        retcode = HResult_OptSkip;
        goto EXIT_STMT;
    }

    retrieve_text = sqlite3_column_text(res, 0);
    if (retrieve_text == NULL)
    {
        retcode = HResult_OptSkip;
        goto EXIT_STMT;
    }

    RefString_Release(p_Str);
    retcode = RefString_InitByChars(p_Str, retrieve_text);

EXIT_STMT:
    sqlite3_finalize(res);

EXIT:
    return retcode;
}


// Return value:
//		HResult_OK					- Success
//		HResult_SQLite3PrepareErr	- failure occurs when preparing the query statement.
//		HResult_SQLite3QueryNULL	- the query result is NULL
//      HResult_OptSkip             - the query result is NULL
HResult sqlite3_helper_query_RefString_RefString(RefString* p_Str0, RefString* p_Str1, sqlite3* db, const char* sql_QueryValue)
{
    HResult retcode = HResult_OK;
    int sqlite_rc = 0;
    sqlite3_stmt* res = NULL;
    const unsigned char* retrieve_text = NULL;

    sqlite_rc = sqlite3_prepare_v2(db, sql_QueryValue, -1, &res, 0);
    if (sqlite_rc != SQLITE_OK)
    {
        retcode = HResult_SQLite3PrepareErr;
        goto EXIT;
    }

    sqlite_rc = sqlite3_step(res);
    if (sqlite_rc != SQLITE_ROW)
    {
        retcode = HResult_SQLite3QueryNULL;
        goto EXIT_STMT;
    }

    if (sqlite3_column_type(res, 0) == SQLITE_NULL)
    {
        retcode = HResult_OptSkip;
        goto EXIT_STMT;
    }

    retrieve_text = sqlite3_column_text(res, 0);
    if (retrieve_text == NULL)
    {
        retrieve_text = "";
    }

    RefString_Release(p_Str0);
    retcode = RefString_InitByChars(p_Str0, retrieve_text);
    if (retcode != HResult_OK)
    {
        goto EXIT_STMT;
    }

    retrieve_text = sqlite3_column_text(res, 1);
    if (retrieve_text == NULL)
    {
        retrieve_text = "";
    }

    RefString_Release(p_Str1);
    retcode = RefString_InitByChars(p_Str1, retrieve_text);
    if (retcode != HResult_OK)
    {
        goto EXIT_STMT;
    }

EXIT_STMT:
    sqlite3_finalize(res);

EXIT:
    return retcode;
}


// Return value:
//		HResult_OK					- Success
//		HResult_SQLite3PrepareErr	- failure occurs when preparing the query statement.
//		HResult_SQLite3QueryNULL	- the query result is NULL
//      HResult_OptSkip             - the query result is NULL
HResult sqlite3_helper_query_RefString_RefString_i64(RefString* p_Str0, RefString* p_Str1, int64_t* p_I64, sqlite3* db, const char* sql_QueryValue)
{
    HResult retcode = HResult_OK;
    int sqlite_rc = 0;
    sqlite3_stmt* res = NULL;
    const unsigned char* retrieve_text = NULL;

    sqlite_rc = sqlite3_prepare_v2(db, sql_QueryValue, -1, &res, 0);
    if (sqlite_rc != SQLITE_OK)
    {
        retcode = HResult_SQLite3PrepareErr;
        goto EXIT;
    }

    sqlite_rc = sqlite3_step(res);
    if (sqlite_rc != SQLITE_ROW)
    {
        retcode = HResult_SQLite3QueryNULL;
        goto EXIT_STMT;
    }

    if (sqlite3_column_type(res, 0) == SQLITE_NULL)
    {
        retcode = HResult_OptSkip;
        goto EXIT_STMT;
    }

    retrieve_text = sqlite3_column_text(res, 0);
    if (retrieve_text == NULL)
    {
        retrieve_text = "";
    }

    RefString_Release(p_Str0);
    retcode = RefString_InitByChars(p_Str0, retrieve_text);
    if (retcode != HResult_OK)
    {
        goto EXIT_STMT;
    }

    retrieve_text = sqlite3_column_text(res, 1);
    if (retrieve_text == NULL)
    {
        retrieve_text = "";
    }

    RefString_Release(p_Str1);
    retcode = RefString_InitByChars(p_Str1, retrieve_text);
    if (retcode != HResult_OK)
    {
        goto EXIT_STMT;
    }

    *p_I64 = sqlite3_column_int64(res, 2);

EXIT_STMT:
    sqlite3_finalize(res);

EXIT:
    return retcode;
}


int64_t sqlite3_helper_GetNewId(sqlite3* db)
{
    const char* sql = "select last_insert_rowid() newid;";
    int64_t newId = -1;
    int sqlite_rc = 0;
    sqlite3_stmt* res = NULL;

    sqlite_rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if (sqlite_rc != SQLITE_OK)
    {
        newId = -2; // HResult_SQLite3PrepareErr
        goto EXIT;
    }

    sqlite_rc = sqlite3_step(res);
    if (sqlite_rc != SQLITE_ROW)
    {
        newId = -3; // HResult_SQLite3QueryNULL;
        goto EXIT_STMT;
    }

    newId = sqlite3_column_int64(res, 0);

EXIT_STMT:
    sqlite3_finalize(res);

EXIT:
    return newId;
}


// Return value:
//		HResult_OK					- Success
//		HResult_SQLite3QueryFail	- failure occurs when executing the query.
//		HResult_SQLite3QueryNULL	- the query result is NULL
HResult sqlite3_helper_query_o_int32_i_int32(int32_t* p_OutVal, sqlite3* db, const char* sql_Query, const int32_t inVal)
{
    HResult ret_code = HResult_OK;
    int sqlite_rc = 0;
    sqlite3_stmt* res = NULL;

    sqlite_rc = sqlite3_prepare_v2(db, sql_Query, -1, &res, 0);
    if (sqlite_rc != SQLITE_OK)
    {
        ret_code = HResult_SQLite3QueryFail;
        goto EXIT;
    }

    sqlite3_bind_int(res, 1, inVal);

    sqlite_rc = sqlite3_step(res);
    if (sqlite_rc != SQLITE_ROW)
    {
        ret_code = HResult_SQLite3QueryNULL;
        goto EXIT_STMT;
    }

    if (sqlite3_column_type(res, 0) == SQLITE_NULL)
    {
        ret_code = HResult_SQLite3QueryNULL;
        goto EXIT_STMT;
    }

    *p_OutVal = sqlite3_column_int(res, 0);

EXIT_STMT:
    sqlite3_finalize(res);

EXIT:
    return ret_code;
}


// Return value:
//		HResult_OK					- Success
//		HResult_SQLite3QueryFail	- failure occurs when executing the query.
//		HResult_SQLite3QueryNULL	- the query result is NULL
HResult sqlite3_helper_query_o_i64_i_i64(int64_t* p_OutVal, sqlite3* db, const char* sql_Query, const int64_t inVal)
{
    HResult ret_code = HResult_OK;
    int sqlite_rc = 0;
    sqlite3_stmt* res = NULL;

    sqlite_rc = sqlite3_prepare_v2(db, sql_Query, -1, &res, 0);
    if (sqlite_rc != SQLITE_OK)
    {
        ret_code = HResult_SQLite3QueryFail;
        goto EXIT;
    }

    sqlite3_bind_int64(res, 1, inVal);

    sqlite_rc = sqlite3_step(res);
    if (sqlite_rc != SQLITE_ROW)
    {
        ret_code = HResult_SQLite3QueryNULL;
        goto EXIT_STMT;
    }

    *p_OutVal = sqlite3_column_int64(res, 0);

EXIT_STMT:
    sqlite3_finalize(res);

EXIT:
    return ret_code;
}


// Return value:
//		HResult_OK					- Success
//		HResult_SQLite3QueryFail	- failure occurs when executing the query.
//		HResult_SQLite3QueryNULL	- the query result is NULL
HResult sqlite3_helper_query_o_i32_i_i32_i32(int32_t* p_OutVal, sqlite3* db, const char* sql_Query, const int32_t inV0, const int32_t inV1)
{
    HResult ret_code = HResult_OK;
    int sqlite_rc = 0;
    sqlite3_stmt* res = NULL;

    sqlite_rc = sqlite3_prepare_v2(db, sql_Query, -1, &res, 0);
    if (sqlite_rc != SQLITE_OK)
    {
        ret_code = HResult_SQLite3QueryFail;
        goto EXIT;
    }

    sqlite3_bind_int(res, 1, inV0);
    sqlite3_bind_int(res, 2, inV1);

    sqlite_rc = sqlite3_step(res);
    if (sqlite_rc != SQLITE_ROW)
    {
        ret_code = HResult_SQLite3QueryNULL;
        goto EXIT_STMT;
    }

    *p_OutVal = sqlite3_column_int(res, 0);

EXIT_STMT:
    sqlite3_finalize(res);

EXIT:
    return ret_code;
}


// Return value:
//		HResult_OK					- Success
//		HResult_SQLite3QueryFail	- failure occurs when executing the query.
//		HResult_SQLite3QueryNULL	- the query result is NULL
HResult sqlite3_helper_query_o_i64_i_i64_i64(int64_t* p_OutVal, sqlite3* db, const char* sql_Query, const int64_t inV0, const int64_t inV1)
{
    HResult ret_code = HResult_OK;
    int sqlite_rc = 0;
    sqlite3_stmt* res = NULL;

    sqlite_rc = sqlite3_prepare_v2(db, sql_Query, -1, &res, 0);
    if (sqlite_rc != SQLITE_OK)
    {
        ret_code = HResult_SQLite3QueryFail;
        goto EXIT;
    }

    sqlite3_bind_int64(res, 1, inV0);
    sqlite3_bind_int64(res, 2, inV1);

    sqlite_rc = sqlite3_step(res);
    if (sqlite_rc != SQLITE_ROW)
    {
        ret_code = HResult_SQLite3QueryNULL;
        goto EXIT_STMT;
    }

    *p_OutVal = sqlite3_column_int64(res, 0);

EXIT_STMT:
    sqlite3_finalize(res);

EXIT:
    return ret_code;
}


// Return value:
//		HResult_OK					- Success
//		HResult_SQLite3QueryFail	- failure occurs when executing the query.
//		HResult_SQLite3QueryNULL	- the query result is NULL
HResult sqlite3_helper_query_o_i64_i_i64_i64_i64(int64_t* p_OutVal, sqlite3* db, const char* sql_Query, const int64_t inV0, const int64_t inV1, const int64_t inV2)
{
    HResult ret_code = HResult_OK;
    int sqlite_rc = 0;
    sqlite3_stmt* res = NULL;

    sqlite_rc = sqlite3_prepare_v2(db, sql_Query, -1, &res, 0);
    if (sqlite_rc != SQLITE_OK)
    {
        ret_code = HResult_SQLite3QueryFail;
        goto EXIT;
    }

    sqlite3_bind_int64(res, 1, inV0);
    sqlite3_bind_int64(res, 2, inV1);
    sqlite3_bind_int64(res, 3, inV2);

    sqlite_rc = sqlite3_step(res);
    if (sqlite_rc != SQLITE_ROW)
    {
        ret_code = HResult_SQLite3QueryNULL;
        goto EXIT_STMT;
    }

    *p_OutVal = sqlite3_column_int64(res, 0);

EXIT_STMT:
    sqlite3_finalize(res);

EXIT:
    return ret_code;
}


// Return value:
//		HResult_OK					- Success
//		HResult_SQLite3QueryFail	- failure occurs when executing the query.
//		HResult_SQLite3QueryNULL	- the query result is NULL
HResult sqlite3_helper_query_o_int32_i_text(int32_t* p_OutVal, sqlite3* db, const char* sql_Query, const char* inTxt, const int inLength)
{
    HResult ret_code = HResult_OK;
    int sqlite_rc = 0;
    sqlite3_stmt* res = NULL;

    sqlite_rc = sqlite3_prepare_v2(db, sql_Query, -1, &res, 0);
    if (sqlite_rc != SQLITE_OK)
    {
        ret_code = HResult_SQLite3QueryFail;
        goto EXIT;
    }

    sqlite3_bind_text(res, 1, inTxt, inLength, NULL);

    sqlite_rc = sqlite3_step(res);
    if (sqlite_rc != SQLITE_ROW)
    {
        ret_code = HResult_SQLite3QueryNULL;
        goto EXIT_STMT;
    }

    *p_OutVal = sqlite3_column_int(res, 0);

EXIT_STMT:
    sqlite3_finalize(res);

EXIT:
    return ret_code;
}


// Return value:
//		HResult_OK					- Success
//		HResult_SQLite3QueryFail	- failure occurs when executing the query.
//		HResult_SQLite3QueryNULL	- the query result is NULL
HResult sqlite3_helper_query_o_i32_i_i32_text(int32_t* p_OutVal, sqlite3* db, const char* sql_Query, const int32_t inVal, const char* inTxt, const int inLength)
{
    HResult ret_code = HResult_OK;
    int sqlite_rc = 0;
    sqlite3_stmt* res = NULL;

    sqlite_rc = sqlite3_prepare_v2(db, sql_Query, -1, &res, 0);
    if (sqlite_rc != SQLITE_OK)
    {
        ret_code = HResult_SQLite3QueryFail;
        goto EXIT;
    }

    sqlite3_bind_int(res, 1, inVal);
    sqlite3_bind_text(res, 2, inTxt, inLength, NULL);

    sqlite_rc = sqlite3_step(res);
    if (sqlite_rc != SQLITE_ROW)
    {
        ret_code = HResult_SQLite3QueryNULL;
        goto EXIT_STMT;
    }

    *p_OutVal = sqlite3_column_int(res, 0);

EXIT_STMT:
    sqlite3_finalize(res);

EXIT:
    return ret_code;
}


// Return value:
//		HResult_OK					- Success
//		HResult_SQLite3QueryFail	- failure occurs when executing the query.
//		HResult_SQLite3QueryNULL	- the query result is NULL
HResult sqlite3_helper_query_o_string_i_i32(char* outbuf, sqlite3* db, const char* sql_Query, const int32_t inVal)
{
    HResult ret_code = HResult_OK;
    int sqlite_rc = 0;
    sqlite3_stmt* res = NULL;
    const unsigned char* retrieve_text = NULL;

    sqlite_rc = sqlite3_prepare_v2(db, sql_Query, -1, &res, 0);
    if (sqlite_rc != SQLITE_OK)
    {
        ret_code = HResult_SQLite3QueryFail;
        goto EXIT;
    }

    sqlite3_bind_int(res, 1, inVal);

    sqlite_rc = sqlite3_step(res);
    if (sqlite_rc != SQLITE_ROW)
    {
        ret_code = HResult_SQLite3QueryNULL;
        goto EXIT_STMT;
    }

    if (sqlite3_column_type(res, 0) != SQLITE_NULL)
    {
        retrieve_text = sqlite3_column_text(res, 0);
        strcpy(outbuf, retrieve_text);
    }

EXIT_STMT:
    sqlite3_finalize(res);

EXIT:
    return ret_code;
}


// Return value:
//		HResult_OK					- Success
//		HResult_SQLite3QueryFail	- failure occurs when executing the query.
//		HResult_SQLite3QueryNULL	- the query result is NULL
HResult sqlite3_helper_query_o_i64_i64_i_i64_i64(int64_t* p_Out0, int64_t* p_Out1, sqlite3* db, const char* sql_Query, const int64_t inVal0, const int64_t inVal1)
{
    HResult ret_code = HResult_OK;
    int sqlite_rc = 0;
    sqlite3_stmt* res = NULL;

    sqlite_rc = sqlite3_prepare_v2(db, sql_Query, -1, &res, 0);
    if (sqlite_rc != SQLITE_OK)
    {
        ret_code = HResult_SQLite3QueryFail;
        goto EXIT;
    }

    sqlite3_bind_int64(res, 1, inVal0);
    sqlite3_bind_int64(res, 2, inVal1);

    sqlite_rc = sqlite3_step(res);
    if (sqlite_rc != SQLITE_ROW)
    {
        ret_code = HResult_SQLite3QueryNULL;
        goto EXIT_STMT;
    }

    *p_Out0 = sqlite3_column_int64(res, 0);
    *p_Out1 = sqlite3_column_int64(res, 1);

EXIT_STMT:
    sqlite3_finalize(res);

EXIT:
    return ret_code;
}


// Return value:
//		HResult_OK					- Success
//		HResult_SQLite3QueryFail	- failure occurs when executing the query.
//		HResult_SQLite3QueryNULL	- the query result is NULL
HResult sqlite3_helper_query_o_i64_i64_default0_i_i64_i64_i64(int64_t* p_Out0, int64_t* p_Out1, sqlite3* db, const char* sql_Query, const int64_t inVal0, const int64_t inVal1, const int64_t inVal2)
{
    HResult ret_code = HResult_OK;
    int sqlite_rc = 0;
    sqlite3_stmt* res = NULL;

    sqlite_rc = sqlite3_prepare_v2(db, sql_Query, -1, &res, 0);
    if (sqlite_rc != SQLITE_OK)
    {
        ret_code = HResult_SQLite3QueryFail;
        goto EXIT;
    }

    sqlite3_bind_int64(res, 1, inVal0);
    sqlite3_bind_int64(res, 2, inVal1);
    sqlite3_bind_int64(res, 3, inVal2);

    sqlite_rc = sqlite3_step(res);
    if (sqlite_rc != SQLITE_ROW)
    {
        ret_code = HResult_SQLite3QueryNULL;
        goto EXIT_STMT;
    }

    *p_Out0 = (SQLITE_NULL == sqlite3_column_type(res, 0)) ? 0 : sqlite3_column_int64(res, 0);
    *p_Out1 = (SQLITE_NULL == sqlite3_column_type(res, 1)) ? 0 : sqlite3_column_int64(res, 1);

EXIT_STMT:
    sqlite3_finalize(res);

EXIT:
    return ret_code;
}


// Return value:
//		HResult_OK					- Success
//		HResult_SQLite3PrepareErr	- failure occurs on sqlite3_prepare_v2.
//		HResult_SQLite3ExecFailed	- get an error on sqlite3_step execution.
HResult sqlite3_helper_exec(sqlite3* db, const char* sql_Exec)
{
    HResult ret_code = HResult_OK;
    int sqlite_rc = 0;
    sqlite3_stmt* res = NULL;

    sqlite_rc = sqlite3_prepare_v2(db, sql_Exec, -1, &res, 0);
    if (sqlite_rc != SQLITE_OK)
    {
        ret_code = HResult_SQLite3PrepareErr;
        goto EXIT;
    }

    sqlite_rc = sqlite3_step(res);
    if (sqlite_rc != SQLITE_DONE)
    {
        ret_code = HResult_SQLite3ExecFailed;
    }

    sqlite3_finalize(res);

EXIT:
    return ret_code;
}


// Return value:
//		HResult_OK					- Success
//		HResult_SQLite3PrepareErr	- failure occurs on sqlite3_prepare_v2.
//		HResult_SQLite3ExecFailed	- get an error on sqlite3_step execution.
HResult sqlite3_helper_exec_int32(sqlite3* db, const char* sql_ExecWithParam, const int32_t val_i32)
{
    HResult ret_code = HResult_OK;
    int sqlite_rc = 0;
    sqlite3_stmt* res = NULL;

    sqlite_rc = sqlite3_prepare_v2(db, sql_ExecWithParam, -1, &res, 0);
    if (sqlite_rc != SQLITE_OK)
    {
        ret_code = HResult_SQLite3PrepareErr;
        goto EXIT;
    }

    sqlite3_bind_int(res, 1, val_i32);

    sqlite_rc = sqlite3_step(res);
    if (sqlite_rc != SQLITE_DONE)
    {
        ret_code = HResult_SQLite3ExecFailed;
    }

    sqlite3_finalize(res);

EXIT:
    return ret_code;
}


// Return value:
//		HResult_OK					- Success
//		HResult_SQLite3PrepareErr	- failure occurs on sqlite3_prepare_v2.
//		HResult_SQLite3ExecFailed	- get an error on sqlite3_step execution.
HResult sqlite3_helper_exec_int32_int32(sqlite3* db, const char* sql_ExecWith2Params, const int32_t val_0, const int32_t val_1)
{
    HResult ret_code = HResult_OK;
    int sqlite_rc = 0;
    sqlite3_stmt* res = NULL;

    sqlite_rc = sqlite3_prepare_v2(db, sql_ExecWith2Params, -1, &res, 0);
    if (sqlite_rc != SQLITE_OK)
    {
        ret_code = HResult_SQLite3PrepareErr;
        goto EXIT;
    }

    sqlite3_bind_int(res, 1, val_0);
    sqlite3_bind_int(res, 2, val_1);

    sqlite_rc = sqlite3_step(res);
    if (sqlite_rc != SQLITE_DONE)
    {
        ret_code = HResult_SQLite3ExecFailed;
    }

    sqlite3_finalize(res);

EXIT:
    return ret_code;
}


// Return value:
//		HResult_OK					- Success
//		HResult_SQLite3PrepareErr	- failure occurs on sqlite3_prepare_v2.
//		HResult_SQLite3ExecFailed	- get an error on sqlite3_step execution.
HResult sqlite3_helper_exec_i64_i64(sqlite3* db, const char* sql_ExecWith2Params, const int64_t val_0, const int64_t val_1)
{
    HResult ret_code = HResult_OK;
    int sqlite_rc = 0;
    sqlite3_stmt* res = NULL;

    sqlite_rc = sqlite3_prepare_v2(db, sql_ExecWith2Params, -1, &res, 0);
    if (sqlite_rc != SQLITE_OK)
    {
        ret_code = HResult_SQLite3PrepareErr;
        goto EXIT;
    }

    sqlite3_bind_int64(res, 1, val_0);
    sqlite3_bind_int64(res, 2, val_1);

    sqlite_rc = sqlite3_step(res);
    if (sqlite_rc != SQLITE_DONE)
    {
        ret_code = HResult_SQLite3ExecFailed;
    }

    sqlite3_finalize(res);

EXIT:
    return ret_code;
}


// Return value:
//		HResult_OK					- Success
//		HResult_SQLite3PrepareErr	- failure occurs on sqlite3_prepare_v2.
//		HResult_SQLite3ExecFailed	- get an error on sqlite3_step execution.
HResult sqlite3_helper_exec_i32_i32_i32(sqlite3* db, const char* sql_ExecWith3Params, const int32_t val_0, const int32_t val_1, const int32_t val_2)
{
    HResult ret_code = HResult_OK;
    int sqlite_rc = 0;
    sqlite3_stmt* res = NULL;

    sqlite_rc = sqlite3_prepare_v2(db, sql_ExecWith3Params, -1, &res, 0);
    if (sqlite_rc != SQLITE_OK)
    {
        ret_code = HResult_SQLite3PrepareErr;
        goto EXIT;
    }

    sqlite3_bind_int(res, 1, val_0);
    sqlite3_bind_int(res, 2, val_1);
    sqlite3_bind_int(res, 3, val_2);

    sqlite_rc = sqlite3_step(res);
    if (sqlite_rc != SQLITE_DONE)
    {
        ret_code = HResult_SQLite3ExecFailed;
    }

    sqlite3_finalize(res);

EXIT:
    return ret_code;
}


// Return value:
//		HResult_OK					- Success
//		HResult_SQLite3PrepareErr	- failure occurs on sqlite3_prepare_v2.
//		HResult_SQLite3ExecFailed	- get an error on sqlite3_step execution.
HResult sqlite3_helper_exec_i64_i64_i64(sqlite3* db, const char* sql_ExecWith3Params, const int64_t val_0, const int64_t val_1, const int64_t val_2)
{
    HResult ret_code = HResult_OK;
    int sqlite_rc = 0;
    sqlite3_stmt* res = NULL;

    sqlite_rc = sqlite3_prepare_v2(db, sql_ExecWith3Params, -1, &res, 0);
    if (sqlite_rc != SQLITE_OK)
    {
        ret_code = HResult_SQLite3PrepareErr;
        goto EXIT;
    }

    sqlite3_bind_int64(res, 1, val_0);
    sqlite3_bind_int64(res, 2, val_1);
    sqlite3_bind_int64(res, 3, val_2);

    sqlite_rc = sqlite3_step(res);
    if (sqlite_rc != SQLITE_DONE)
    {
        ret_code = HResult_SQLite3ExecFailed;
    }

    sqlite3_finalize(res);

EXIT:
    return ret_code;
}


// Return value:
//		HResult_OK					- Success
//		HResult_SQLite3PrepareErr	- failure occurs on sqlite3_prepare_v2.
//		HResult_SQLite3ExecFailed	- get an error on sqlite3_step execution.
HResult sqlite3_helper_exec_double_double_i32(sqlite3* db, const char* sql_ExecWith3Params, const double val_0, const double val_1, const int32_t val_2)
{
    HResult ret_code = HResult_OK;
    int sqlite_rc = 0;
    sqlite3_stmt* res = NULL;

    sqlite_rc = sqlite3_prepare_v2(db, sql_ExecWith3Params, -1, &res, 0);
    if (sqlite_rc != SQLITE_OK)
    {
        ret_code = HResult_SQLite3PrepareErr;
        goto EXIT;
    }

    sqlite3_bind_double(res, 1, val_0);
    sqlite3_bind_double(res, 2, val_1);
    sqlite3_bind_int(res, 3, val_2);

    sqlite_rc = sqlite3_step(res);
    if (sqlite_rc != SQLITE_DONE)
    {
        ret_code = HResult_SQLite3ExecFailed;
    }

    sqlite3_finalize(res);

EXIT:
    return ret_code;
}


// Return value:
//		HResult_OK					- Success
//		HResult_SQLite3PrepareErr	- failure occurs on sqlite3_prepare_v2.
//		HResult_SQLite3ExecFailed	- get an error on sqlite3_step execution.
HResult sqlite3_helper_exec_int32_string(sqlite3* db, const char* sql_ExecWith2Params, const int32_t intVal, const char* str_id)
{
    HResult ret_code = HResult_OK;
    int sqlite_rc = 0;
    sqlite3_stmt* res = NULL;

    sqlite_rc = sqlite3_prepare_v2(db, sql_ExecWith2Params, -1, &res, 0);
    if (sqlite_rc != SQLITE_OK)
    {
        ret_code = HResult_SQLite3PrepareErr;
        goto EXIT;
    }

    sqlite3_bind_int(res, 1, intVal);
    sqlite3_bind_text(res, 2, str_id, (int)strlen(str_id), NULL);

    sqlite_rc = sqlite3_step(res);
    if (sqlite_rc != SQLITE_DONE)
    {
        ret_code = HResult_SQLite3ExecFailed;
    }

    sqlite3_finalize(res);

EXIT:
    return ret_code;
}


// Return value:
//		HResult_OK					- Success
//		HResult_SQLite3PrepareErr	- failure occurs on sqlite3_prepare_v2.
//		HResult_SQLite3ExecFailed	- get an error on sqlite3_step execution.
HResult sqlite3_helper_exec_string_i32(sqlite3* db, const char* sql_ExecWith2Params, const char* str_id, const int32_t intVal)
{
    HResult ret_code = HResult_OK;
    int sqlite_rc = 0;
    sqlite3_stmt* res = NULL;

    sqlite_rc = sqlite3_prepare_v2(db, sql_ExecWith2Params, -1, &res, 0);
    if (sqlite_rc != SQLITE_OK)
    {
        ret_code = HResult_SQLite3PrepareErr;
        goto EXIT;
    }

    sqlite3_bind_text(res, 1, str_id, (int)strlen(str_id), NULL);
    sqlite3_bind_int(res, 2, intVal);

    sqlite_rc = sqlite3_step(res);
    if (sqlite_rc != SQLITE_DONE)
    {
        ret_code = HResult_SQLite3ExecFailed;
    }

    sqlite3_finalize(res);

EXIT:
    return ret_code;
}


// Return value:
//		HResult_OK					- Success
//		HResult_SQLite3PrepareErr	- failure occurs on sqlite3_prepare_v2.
//		HResult_SQLite3ExecFailed	- get an error on sqlite3_step execution.
HResult sqlite3_helper_exec_i64_string_string(sqlite3* db, const char* sql_ExecWith2Params, const int64_t i64, const char* str0, const char* str1)
{
    HResult ret_code = HResult_OK;
    int sqlite_rc = 0;
    sqlite3_stmt* res = NULL;

    sqlite_rc = sqlite3_prepare_v2(db, sql_ExecWith2Params, -1, &res, 0);
    if (sqlite_rc != SQLITE_OK)
    {
        ret_code = HResult_SQLite3PrepareErr;
        goto EXIT;
    }

    sqlite3_bind_int64(res, 1, i64);
    sqlite3_bind_text(res, 2, str0, (int)strlen(str0), NULL);
    sqlite3_bind_text(res, 3, str1, (int)strlen(str1), NULL);

    sqlite_rc = sqlite3_step(res);
    if (sqlite_rc != SQLITE_DONE)
    {
        ret_code = HResult_SQLite3ExecFailed;
    }

    sqlite3_finalize(res);

EXIT:
    return ret_code;
}


// Return value:
//		HResult_OK					- Success
//		HResult_SQLite3PrepareErr	- failure occurs on sqlite3_prepare_v2.
//		HResult_SQLite3ExecFailed	- get an error on sqlite3_step execution.
HResult sqlite3_helper_exec_i32_i32_string(sqlite3* db, const char* sql_ExecWith3Params, const int32_t intVal0, const int32_t intVal1, const char* str_id)
{
    HResult ret_code = HResult_OK;
    int sqlite_rc = 0;
    sqlite3_stmt* res = NULL;

    sqlite_rc = sqlite3_prepare_v2(db, sql_ExecWith3Params, -1, &res, 0);
    if (sqlite_rc != SQLITE_OK)
    {
        ret_code = HResult_SQLite3PrepareErr;
        goto EXIT;
    }

    sqlite3_bind_int(res, 1, intVal0);
    sqlite3_bind_int(res, 2, intVal1);
    sqlite3_bind_text(res, 3, str_id, (int)strlen(str_id), NULL);

    sqlite_rc = sqlite3_step(res);
    if (sqlite_rc != SQLITE_DONE)
    {
        ret_code = HResult_SQLite3ExecFailed;
    }

    sqlite3_finalize(res);

EXIT:
    return ret_code;
}


// Return value:
//		HResult_OK					- Success
//		HResult_SQLite3PrepareErr	- failure occurs on sqlite3_prepare_v2.
//		HResult_SQLite3ExecFailed	- get an error on sqlite3_step execution.
HResult sqlite3_helper_exec_i32_i32_RefString(sqlite3* db, const char* sql_ExecWith3Params, const int32_t intVal0, const int32_t intVal1, const RefString* refstr)
{
    HResult ret_code = HResult_OK;
    int sqlite_rc = 0;
    sqlite3_stmt* res = NULL;

    sqlite_rc = sqlite3_prepare_v2(db, sql_ExecWith3Params, -1, &res, 0);
    if (sqlite_rc != SQLITE_OK)
    {
        ret_code = HResult_SQLite3PrepareErr;
        goto EXIT;
    }

    sqlite3_bind_int(res, 1, intVal0);
    sqlite3_bind_int(res, 2, intVal1);
    sqlite3_bind_text(res, 3, refstr->strbuf, refstr->strlen, NULL);

    sqlite_rc = sqlite3_step(res);
    if (sqlite_rc != SQLITE_DONE)
    {
        ret_code = HResult_SQLite3ExecFailed;
    }

    sqlite3_finalize(res);

EXIT:
    return ret_code;
}


// Return value:
//		HResult_OK					- Success
//		HResult_SQLite3PrepareErr	- failure occurs on sqlite3_prepare_v2.
//		HResult_SQLite3ExecFailed	- get an error on sqlite3_step execution.
HResult sqlite3_helper_exec_i32_i32_i32_string(sqlite3* db, const char* sql_ExecWith4Params, const int32_t intVal0, const int32_t intVal1, const int32_t intVal2, const char* str_id)
{
    HResult ret_code = HResult_OK;
    int sqlite_rc = 0;
    sqlite3_stmt* res = NULL;

    sqlite_rc = sqlite3_prepare_v2(db, sql_ExecWith4Params, -1, &res, 0);
    if (sqlite_rc != SQLITE_OK)
    {
        ret_code = HResult_SQLite3PrepareErr;
        goto EXIT;
    }

    sqlite3_bind_int(res, 1, intVal0);
    sqlite3_bind_int(res, 2, intVal1);
    sqlite3_bind_int(res, 3, intVal2);
    sqlite3_bind_text(res, 4, str_id, (int)strlen(str_id), NULL);

    sqlite_rc = sqlite3_step(res);
    if (sqlite_rc != SQLITE_DONE)
    {
        ret_code = HResult_SQLite3ExecFailed;
    }

    sqlite3_finalize(res);

EXIT:
    return ret_code;
}


// Return value:
//		HResult_OBJECT_IS_NULL		| Any of the *pointer* parameters is NULL.
//		HResult_SQLite3PrepareErr	| sqlite3_prepare_v2() failed.
HResult sqlite3_helper_query_callback(sqlite3* db, const char* sql_query, get_sqlite3_column_callback callback, void* output_buf)
{
    HResult rc = HResult_OK;
    HResult step_rc = HResult_Unknown;
    int sqlite_rc = 0;
    sqlite3_stmt* stmt = NULL;
    size_t rowIndex = 0;

    if (db == NULL || sql_query == NULL || callback == NULL || output_buf == NULL)
    {
        rc = HResult_OBJECT_IS_NULL;
        goto EXIT;
    }

    sqlite_rc = sqlite3_prepare_v2(db, sql_query, -1, &stmt, 0);
    if (sqlite_rc != SQLITE_OK)
    {
        rc = HResult_SQLite3PrepareErr;
        goto EXIT;
    }

    rowIndex = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        step_rc = callback(stmt, rowIndex, output_buf);
        if (step_rc != HResult_OK)
        {
            rc = step_rc;
            goto EXIT_STMT;
        }

        rowIndex++;
    }

EXIT_STMT:
    sqlite3_finalize(stmt);

EXIT:
    return rc;
}


// Return value:
//		HResult_OBJECT_IS_NULL		| Any of the *pointer* parameters is NULL.
//		HResult_SQLite3PrepareErr	| sqlite3_prepare_v2() failed.
HResult sqlite3_helper_query_callback_i_i32(sqlite3* db, const char* sql_query, get_sqlite3_column_callback callback, void* output_buf, const int32_t inval)
{
    HResult rc = HResult_OK;
    HResult step_rc = HResult_Unknown;
    int sqlite_rc = 0;
    sqlite3_stmt* stmt = NULL;
    size_t rowIndex = 0;

    if (db == NULL || sql_query == NULL || callback == NULL || output_buf == NULL)
    {
        rc = HResult_OBJECT_IS_NULL;
        goto EXIT;
    }

    sqlite_rc = sqlite3_prepare_v2(db, sql_query, -1, &stmt, 0);
    if (sqlite_rc != SQLITE_OK)
    {
        rc = HResult_SQLite3PrepareErr;
        goto EXIT;
    }

    sqlite3_bind_int(stmt, 1, inval);

    rowIndex = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        step_rc = callback(stmt, rowIndex, output_buf);
        if (step_rc != HResult_OK)
        {
            rc = step_rc;
            goto EXIT_STMT;
        }

        rowIndex++;
    }

EXIT_STMT:
    sqlite3_finalize(stmt);

EXIT:
    return rc;
}


// Return value:
//		HResult_OBJECT_IS_NULL		| Any of the *pointer* parameters is NULL.
//		HResult_SQLite3PrepareErr	| sqlite3_prepare_v2() failed.
HResult sqlite3_helper_query_callback_i_i32_i32(sqlite3* db, const char* sql_query, get_sqlite3_column_callback callback, void* output_buf, const int32_t inVal0, const int32_t inVal1)
{
    HResult rc = HResult_OK;
    HResult step_rc = HResult_Unknown;
    int sqlite_rc = 0;
    sqlite3_stmt* stmt = NULL;
    size_t rowIndex = 0;

    if (db == NULL || sql_query == NULL || callback == NULL || output_buf == NULL)
    {
        rc = HResult_OBJECT_IS_NULL;
        goto EXIT;
    }

    sqlite_rc = sqlite3_prepare_v2(db, sql_query, -1, &stmt, 0);
    if (sqlite_rc != SQLITE_OK)
    {
        rc = HResult_SQLite3PrepareErr;
        goto EXIT;
    }

    sqlite3_bind_int(stmt, 1, inVal0);
    sqlite3_bind_int(stmt, 2, inVal1);

    rowIndex = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        step_rc = callback(stmt, rowIndex, output_buf);
        if (step_rc != HResult_OK)
        {
            rc = step_rc;
            goto EXIT_STMT;
        }

        rowIndex++;
    }

EXIT_STMT:
    sqlite3_finalize(stmt);

EXIT:
    return rc;
}


// Return value:
//		HResult_OBJECT_IS_NULL		| Any of the *pointer* parameters is NULL.
//		HResult_SQLite3PrepareErr	| sqlite3_prepare_v2() failed.
HResult sqlite3_helper_query_callback_i_i64(sqlite3* db, const char* sql_query, get_sqlite3_column_callback callback, void* output_buf, const int64_t inval)
{
    HResult rc = HResult_OK;
    HResult step_rc = HResult_Unknown;
    int sqlite_rc = 0;
    sqlite3_stmt* stmt = NULL;
    size_t rowIndex = 0;

    if (db == NULL || sql_query == NULL || callback == NULL || output_buf == NULL)
    {
        rc = HResult_OBJECT_IS_NULL;
        goto EXIT;
    }

    sqlite_rc = sqlite3_prepare_v2(db, sql_query, -1, &stmt, 0);
    if (sqlite_rc != SQLITE_OK)
    {
        rc = HResult_SQLite3PrepareErr;
        goto EXIT;
    }

    sqlite3_bind_int64(stmt, 1, inval);

    rowIndex = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        step_rc = callback(stmt, rowIndex, output_buf);
        if (step_rc != HResult_OK)
        {
            rc = step_rc;
            goto EXIT_STMT;
        }

        rowIndex++;
    }

EXIT_STMT:
    sqlite3_finalize(stmt);

EXIT:
    return rc;
}


// Return value:
//		HResult_OBJECT_IS_NULL		| Any of the *pointer* parameters is NULL.
//		HResult_SQLite3PrepareErr	| sqlite3_prepare_v2() failed.
HResult sqlite3_helper_query_callback_i_i64_i64(sqlite3* db, const char* sql_query, get_sqlite3_column_callback callback, void* output_buf, const int64_t inVal0, const int64_t inVal1)
{
    HResult rc = HResult_OK;
    HResult step_rc = HResult_Unknown;
    int sqlite_rc = 0;
    sqlite3_stmt* stmt = NULL;
    size_t rowIndex = 0;

    if (db == NULL || sql_query == NULL || callback == NULL || output_buf == NULL)
    {
        rc = HResult_OBJECT_IS_NULL;
        goto EXIT;
    }

    sqlite_rc = sqlite3_prepare_v2(db, sql_query, -1, &stmt, 0);
    if (sqlite_rc != SQLITE_OK)
    {
        rc = HResult_SQLite3PrepareErr;
        goto EXIT;
    }

    sqlite3_bind_int64(stmt, 1, inVal0);
    sqlite3_bind_int64(stmt, 2, inVal1);

    rowIndex = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        step_rc = callback(stmt, rowIndex, output_buf);
        if (step_rc != HResult_OK)
        {
            rc = step_rc;
            goto EXIT_STMT;
        }

        rowIndex++;
    }

EXIT_STMT:
    sqlite3_finalize(stmt);

EXIT:
    return rc;
}


// Return value:
//		HResult_OBJECT_IS_NULL		| Any of the *pointer* parameters is NULL.
//		HResult_SQLite3PrepareErr	| sqlite3_prepare_v2() failed.
HResult sqlite3_helper_query_callback_i_i64_i64_i64(sqlite3* db, const char* sql_query, get_sqlite3_column_callback callback, void* output_buf, const int64_t inVal0, const int64_t inVal1, const int64_t inVal2)
{
    HResult rc = HResult_OK;
    HResult step_rc = HResult_Unknown;
    int sqlite_rc = 0;
    sqlite3_stmt* stmt = NULL;
    size_t rowIndex = 0;

    if (db == NULL || sql_query == NULL || callback == NULL || output_buf == NULL)
    {
        rc = HResult_OBJECT_IS_NULL;
        goto EXIT;
    }

    sqlite_rc = sqlite3_prepare_v2(db, sql_query, -1, &stmt, 0);
    if (sqlite_rc != SQLITE_OK)
    {
        rc = HResult_SQLite3PrepareErr;
        goto EXIT;
    }

    sqlite3_bind_int64(stmt, 1, inVal0);
    sqlite3_bind_int64(stmt, 2, inVal1);
    sqlite3_bind_int64(stmt, 3, inVal2);

    rowIndex = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        step_rc = callback(stmt, rowIndex, output_buf);
        if (step_rc != HResult_OK)
        {
            rc = step_rc;
            goto EXIT_STMT;
        }

        rowIndex++;
    }

EXIT_STMT:
    sqlite3_finalize(stmt);

EXIT:
    return rc;
}
