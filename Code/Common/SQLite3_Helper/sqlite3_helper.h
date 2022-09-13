#ifndef _INC_COMMON_SQLite3_Helper_H
#define _INC_COMMON_SQLite3_Helper_H

#include "../datatypes.h"
#include "../DS/RefString.h"
#include <sqlite3.h>


// Return value:
//		HResult_OK					- Success
//		HResult_FILE_NOTEXISTS		- File not exist
//		HResult_SQLite3OpenFail		- SQLite3 DB open failed
HResult sqlite3_helper_open(const char* db_path, sqlite3** p_db);

void sqlite3_helper_close(sqlite3** p_db);

void sqlite3_helper_BeginTransaction(sqlite3* db);
void sqlite3_helper_COMMIT(sqlite3* db);

// Return value:
//		HResult_OK					- Success
//		HResult_SQLite3PrepareErr	- failure occurs when preparing the query statement.
//		HResult_SQLite3QueryNULL	- the query result is NULL
HResult sqlite3_helper_query_int32(int32_t* p_value, sqlite3* db, const char* sql_QueryValue);
HResult sqlite3_helper_query_int64(int64_t* p_value, sqlite3* db, const char* sql_QueryValue);
HResult sqlite3_helper_query_double(double* p_value, sqlite3* db, const char* sql_QueryValue);

// Return value:
//		HResult_OK					- Success
//		HResult_SQLite3PrepareErr	- failure occurs when preparing the query statement.
//		HResult_SQLite3QueryNULL	- the query result is NULL
//      HResult_OptSkip             - the query result is NULL
HResult sqlite3_helper_query_RefString(RefString* p_Str, sqlite3* db, const char* sql_QueryValue);
HResult sqlite3_helper_query_RefString_RefString(RefString* p_Str0, RefString* p_Str1, sqlite3* db, const char* sql_QueryValue);
HResult sqlite3_helper_query_RefString_RefString_i64(RefString* p_Str0, RefString* p_Str1, int64_t* p_I64, sqlite3* db, const char* sql_QueryValue);

int64_t sqlite3_helper_GetNewId(sqlite3* db);

// Return value:
//		HResult_OK					- Success
//		HResult_SQLite3QueryFail	- failure occurs when executing the query.
//		HResult_SQLite3QueryNULL	- the query result is NULL
HResult sqlite3_helper_query_o_int32_i_int32(int32_t* p_OutVal, sqlite3* db, const char* sql_Query, const int32_t inVal);
HResult sqlite3_helper_query_o_i64_i_i64(int64_t* p_OutVal, sqlite3* db, const char* sql_Query, const int64_t inVal);
HResult sqlite3_helper_query_o_i32_i_i32_i32(int32_t* p_OutVal, sqlite3* db, const char* sql_Query, const int32_t inV0, const int32_t inV1);
HResult sqlite3_helper_query_o_i64_i_i64_i64(int64_t* p_OutVal, sqlite3* db, const char* sql_Query, const int64_t inV0, const int64_t inV1);
HResult sqlite3_helper_query_o_i64_i_i64_i64_i64(int64_t* p_OutVal, sqlite3* db, const char* sql_Query, const int64_t inV0, const int64_t inV1, const int64_t inV2);
HResult sqlite3_helper_query_o_int32_i_text(int32_t* p_OutVal, sqlite3* db, const char* sql_Query, const char* inTxt, const int inLength);
HResult sqlite3_helper_query_o_i32_i_i32_text(int32_t* p_OutVal, sqlite3* db, const char* sql_Query, const int32_t inVal, const char* inTxt, const int inLength);

HResult sqlite3_helper_query_o_string_i_i32(char* outbuf, sqlite3* db, const char* sql_Query, const int32_t inVal);

HResult sqlite3_helper_query_o_i64_i64_i_i64_i64(int64_t* p_Out0, int64_t* p_Out1, sqlite3* db, const char* sql_Query, const int64_t inVal0, const int64_t inVal1);
HResult sqlite3_helper_query_o_i64_i64_default0_i_i64_i64_i64(int64_t* p_Out0, int64_t* p_Out1, sqlite3* db, const char* sql_Query, const int64_t inVal0, const int64_t inVal1, const int64_t inVal2);

// Return value:
//		HResult_OK					- Success
//		HResult_SQLite3PrepareErr	- failure occurs on sqlite3_prepare_v2.
//		HResult_SQLite3ExecFailed	- get an error on sqlite3_step execution.
HResult sqlite3_helper_exec(sqlite3* db, const char* sql_Exec);

// Return value:
//		HResult_OK					- Success
//		HResult_SQLite3PrepareErr	- failure occurs on sqlite3_prepare_v2.
//		HResult_SQLite3ExecFailed	- get an error on sqlite3_step execution.
HResult sqlite3_helper_exec_int32(sqlite3* db, const char* sql_ExecWithParam, const int32_t val_i32);

// Return value:
//		HResult_OK					- Success
//		HResult_SQLite3PrepareErr	- failure occurs on sqlite3_prepare_v2.
//		HResult_SQLite3ExecFailed	- get an error on sqlite3_step execution.
HResult sqlite3_helper_exec_int32_int32(sqlite3* db, const char* sql_ExecWith2Params, const int32_t val_0, const int32_t val_1);
HResult sqlite3_helper_exec_i64_i64(sqlite3* db, const char* sql_ExecWith2Params, const int64_t val_0, const int64_t val_1);
HResult sqlite3_helper_exec_i32_i32_i32(sqlite3* db, const char* sql_ExecWith3Params, const int32_t val_0, const int32_t val_1, const int32_t val_2);
HResult sqlite3_helper_exec_i64_i64_i64(sqlite3* db, const char* sql_ExecWith3Params, const int64_t val_0, const int64_t val_1, const int64_t val_2);

HResult sqlite3_helper_exec_double_double_i32(sqlite3* db, const char* sql_ExecWith3Params, const double val_0, const double val_1, const int32_t val_2);

HResult sqlite3_helper_exec_int32_string(sqlite3* db, const char* sql_ExecWith2Params, const int32_t intVal, const char* str_id);
HResult sqlite3_helper_exec_string_i32(sqlite3* db, const char* sql_ExecWith2Params, const char* str_id, const int32_t intVal);
HResult sqlite3_helper_exec_i64_string_string(sqlite3* db, const char* sql_ExecWith2Params, const int64_t i64, const char* str0, const char* str1);
HResult sqlite3_helper_exec_i32_i32_string(sqlite3* db, const char* sql_ExecWith3Params, const int32_t intVal0, const int32_t intVal1, const char* str_id);
HResult sqlite3_helper_exec_i32_i32_RefString(sqlite3* db, const char* sql_ExecWith3Params, const int32_t intVal0, const int32_t intVal1, const RefString* refstr);
HResult sqlite3_helper_exec_i32_i32_i32_string(sqlite3* db, const char* sql_ExecWith4Params, const int32_t intVal0, const int32_t intVal1, const int32_t intVal2, const char* str_id);

// Must return HResult_OK if ready to move on to next row.
typedef HResult(*get_sqlite3_column_callback)(sqlite3_stmt* stmt, const size_t rowIndex, void* output_buf);

// Return value:
//		HResult_OBJECT_IS_NULL		| Any of the *pointer* parameters is NULL.
//		HResult_SQLite3PrepareErr	| sqlite3_prepare_v2() failed.
HResult sqlite3_helper_query_callback(sqlite3* db, const char* sql_query, get_sqlite3_column_callback callback, void* output_buf);
HResult sqlite3_helper_query_callback_i_i32(sqlite3* db, const char* sql_query, get_sqlite3_column_callback callback, void* output_buf, const int32_t inval);
HResult sqlite3_helper_query_callback_i_i32_i32(sqlite3* db, const char* sql_query, get_sqlite3_column_callback callback, void* output_buf, const int32_t inVal0, const int32_t inVal1);
HResult sqlite3_helper_query_callback_i_i64(sqlite3* db, const char* sql_query, get_sqlite3_column_callback callback, void* output_buf, const int64_t inval);
HResult sqlite3_helper_query_callback_i_i64_i64(sqlite3* db, const char* sql_query, get_sqlite3_column_callback callback, void* output_buf, const int64_t inVal0, const int64_t inVal1);
HResult sqlite3_helper_query_callback_i_i64_i64_i64(sqlite3* db, const char* sql_query, get_sqlite3_column_callback callback, void* output_buf, const int64_t inVal0, const int64_t inVal1, const int64_t inVal2);

#endif // !_INC_COMMON_SQLite3_Helper_H
