#ifndef _INC_UTILITY_APP_SQLITETOOLS_DRV_Internal_H
#define _INC_UTILITY_APP_SQLITETOOLS_DRV_Internal_H

#include "Common/datatypes.h"
#include "Common/SQLite3_Helper/sqlite3_helper.h"

#ifndef LOG_OFF
#include "Common/LogInterface/log_interface.h"
#endif


typedef struct
{
    size_t Id;
    size_t Idx;

} IdxPair;

typedef struct
{
    size_t count;
    IdxPair* items;

} IdxSet;

HResult IdxSet_InitByCount(IdxSet* set, const size_t _count);
void IdxSet_Release(IdxSet* set);


typedef struct
{
    size_t Id;
    size_t Idx;
    size_t AuxId;

} IdxAuxPair;

typedef struct
{
    size_t count;
    IdxAuxPair* items;

} IdxAuxSet;

HResult IdxAuxSet_InitByCount(IdxAuxSet* set, const size_t _count);
void IdxAuxSet_Release(IdxAuxSet* set);


/*
 * Open the SQLite connection from the default DB Path. 
 */
HResult sqlite3_dbopen(sqlite3** p_dbconn);


/*
 * DELETE all the records from table and reset the sequence id to be 0
 */
HResult sqlite3_cleanup_table(sqlite3* dbconn, const char* table_name);


/*
 * Retrieve the count of table using sql statement "select count(*) from <table_name>;"
 * Input parameters:
 *  [0] dbconn
 *  [1] table_name
 * Output parameters:
 *  [0] p_Count
 */
HResult sqlite3_table_count(sqlite3* dbconn, const char* table_name, size_t* p_Count);


HResult sqlite3_IdxSetInit_ApplyIdxById(sqlite3* dbconn, const char* table_name, IdxSet* set);
HResult sqlite3_IdxSetInit_ApplyIdxByIdColumn(sqlite3* dbconn, const char* table_name, const char* id_name, const char* idx_name, IdxSet* set);

HResult sqlite3_LoadIdxAuxSet(sqlite3* dbconn, const char* sql_Count, const char* sql_IdxAuxPair, IdxAuxSet* set);


#endif // !_INC_UTILITY_APP_SQLITETOOLS_DRV_Internal_H
