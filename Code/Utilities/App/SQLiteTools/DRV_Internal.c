#include "DRV_Config.h"
#include "DRV_Internal.h"

static char sqlbuf[PATH_MAX] = { 0 };

static HResult callback_RetrieveId_ApplyIdx(sqlite3_stmt* stmt, const size_t rowIndex, void* output_buf);
static HResult callback_RetrieveIdxAuxPair(sqlite3_stmt* stmt, const size_t rowIndex, void* output_buf);


HResult IdxSet_InitByCount(IdxSet* set, const size_t _count)
{
    HResult retcode = HResult_OK;

    if (set == NULL)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    set->count = _count;
    set->items = NULL;

    if (set->count > 0)
    {
        set->items = malloc(set->count * sizeof(IdxPair));
        if (!set->items)
        {
            retcode = HResult_ALLOC_FAIL;
            goto EXIT;
        }
        memset(set->items, 0, set->count * sizeof(IdxPair));
    }

EXIT:
    return retcode;
}


void IdxSet_Release(IdxSet* set)
{
    if (set)
    {
        if (set->items)
        {
            free(set->items);
            set->items = NULL;
        }
        set->count = 0;
    }
}


HResult IdxAuxSet_InitByCount(IdxAuxSet* set, const size_t _count)
{
    HResult retcode = HResult_OK;

    if (set == NULL)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    set->count = _count;
    set->items = NULL;

    if (set->count > 0)
    {
        set->items = malloc(set->count * sizeof(IdxAuxPair));
        if (!set->items)
        {
            retcode = HResult_ALLOC_FAIL;
            goto EXIT;
        }
        memset(set->items, 0, set->count * sizeof(IdxAuxPair));
    }

EXIT:
    return retcode;
}


void IdxAuxSet_Release(IdxAuxSet* set)
{
    if (set)
    {
        if (set->items)
        {
            free(set->items);
            set->items = NULL;
        }
        set->count = 0;
    }
}


/*
 * Open the SQLite connection from the default DB Path.
 */
HResult sqlite3_dbopen(sqlite3** p_dbconn)
{
    HResult retcode = HResult_OK;
    DRVConfig* config = get_G_DRVConfig();

    if (config == NULL || config->dbpath == NULL)
    {
        retcode = HResult_CONFIG_FAIL;
        goto EXIT;
    }

    retcode = sqlite3_helper_open(config->dbpath, p_dbconn);
    if (retcode != HResult_OK)
    {
        if (*p_dbconn != NULL)
        {
            sqlite3_helper_close(p_dbconn);
        }

#ifndef LOG_OFF
        LOGERROR("Failed open BIN: %s", config->dbpath);
#endif

        goto EXIT;
    }

EXIT:
    return retcode;
}


/*
 * DELETE all the records from table and reset the sequence id to be 0
 */
HResult sqlite3_cleanup_table(sqlite3* dbconn, const char* table_name)
{
    const char* DELETE_TABLE_FMT = "DELETE FROM \"%s\";";
    const char* TRUNCATE_TABLE_FMT = "UPDATE sqlite_sequence SET seq = 0 WHERE name = '%s';";

    HResult retcode = HResult_OK;
    size_t name_length = 0;

    if (dbconn == NULL || table_name == NULL || 0 == (name_length = strlen(table_name)))
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    if (name_length >= PATH_MAX / 2)
    {
        retcode = HResult_PARAM_OUTRANGE;
        goto EXIT;
    }

    memset(sqlbuf, 0, PATH_MAX * sizeof(char));
    sprintf(sqlbuf, DELETE_TABLE_FMT, table_name);

    retcode = sqlite3_helper_exec(dbconn, sqlbuf);
    if (retcode != HResult_OK)
    {
        goto EXIT;
    }

    memset(sqlbuf, 0, PATH_MAX * sizeof(char));
    sprintf(sqlbuf, TRUNCATE_TABLE_FMT, table_name);

    retcode = sqlite3_helper_exec(dbconn, sqlbuf);

EXIT:
    memset(sqlbuf, 0, PATH_MAX * sizeof(char));
    return retcode;
}


/*
 * Retrieve the count of table using sql statement "select count(*) from <table_name>;"
 * Input parameters:
 *  [0] dbconn
 *  [1] table_name
 * Output parameters:
 *  [0] p_Count
 */
HResult sqlite3_table_count(sqlite3* dbconn, const char* table_name, size_t* p_Count)
{
    const char* TABLE_COUNT_FMT = "SELECT COUNT(*) FROM \"%s\";";

    HResult retcode = HResult_OK;
    size_t name_length = 0;
    int64_t i64 = 0;

    if (dbconn == NULL || table_name == NULL || 0 == (name_length = strlen(table_name)) || p_Count == NULL)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    if (name_length >= PATH_MAX / 2)
    {
        retcode = HResult_PARAM_OUTRANGE;
        goto EXIT;
    }

    memset(sqlbuf, 0, PATH_MAX * sizeof(char));
    sprintf(sqlbuf, TABLE_COUNT_FMT, table_name);

    retcode = sqlite3_helper_query_int64(&i64, dbconn, sqlbuf);
    if (retcode != HResult_OK)
    {
        goto EXIT;
    }

    *p_Count = (size_t)i64;

EXIT:
    memset(sqlbuf, 0, PATH_MAX * sizeof(char));
    return retcode;
}


HResult sqlite3_IdxSetInit_ApplyIdxById(sqlite3* dbconn, const char* table_name, IdxSet* set)
{
    return sqlite3_IdxSetInit_ApplyIdxByIdColumn(dbconn, table_name, "Id", "Idx", set);
}


HResult sqlite3_IdxSetInit_ApplyIdxByIdColumn(sqlite3* dbconn, const char* table_name, const char* id_name, const char* idx_name, IdxSet* set)
{
    // const char* SQL_SelectId_FMT = "SELECT Id FROM FormulaSteps ORDER BY Id;";
    const char* SQL_SelectId_FMT = "SELECT \"%s\" FROM \"%s\" ORDER BY \"%s\";";

    // const char* SQL_UpdateIdx_FMT = "UPDATE FormulaSteps SET Idx = 0 WHERE Id = 1;";
    const char* SQL_UpdateIdx_FMT = "UPDATE \"%s\" SET \"%s\" = ? WHERE \"%s\" = ?;";

    HResult retcode = HResult_OK;
    size_t name_length = 0;
    size_t count = 0;

    if (id_name == NULL || 0 == (name_length = strlen(id_name)))
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    if (dbconn == NULL || table_name == NULL || 0 == (name_length = strlen(table_name)) || set == NULL)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    if (name_length >= PATH_MAX / 2)
    {
        retcode = HResult_PARAM_OUTRANGE;
        goto EXIT;
    }

    retcode = sqlite3_table_count(dbconn, table_name, &count);
    if (retcode != HResult_OK)
    {
        goto EXIT;
    }

    retcode = IdxSet_InitByCount(set, count);
    if (retcode != HResult_OK)
    {
        goto EXIT;
    }

    if (count == 0)
    {
        goto EXIT;
    }

    memset(sqlbuf, 0, PATH_MAX * sizeof(char));
    sprintf(sqlbuf, SQL_SelectId_FMT, id_name, table_name, id_name);

    retcode = sqlite3_helper_query_callback(dbconn, sqlbuf, callback_RetrieveId_ApplyIdx, set);
    if (retcode != HResult_OK)
    {
        goto EXIT;
    }

    if (idx_name == NULL || 0 == (name_length = strlen(idx_name)))
    {
        goto EXIT;
    }

    memset(sqlbuf, 0, PATH_MAX * sizeof(char));
    sprintf(sqlbuf, SQL_UpdateIdx_FMT, table_name, idx_name, id_name);

    sqlite3_helper_BeginTransaction(dbconn);

    for (size_t idsz = 0; idsz < set->count; idsz++)
    {
        IdxPair* item = &set->items[idsz];
        retcode = sqlite3_helper_exec_i64_i64(dbconn, sqlbuf, (const int64_t)item->Idx, (const int64_t)item->Id);
        if (retcode != HResult_OK)
        {
            goto EXIT;
        }
    }

    sqlite3_helper_COMMIT(dbconn);

EXIT:
    memset(sqlbuf, 0, PATH_MAX * sizeof(char));
    return retcode;
}


HResult sqlite3_LoadIdxAuxSet(sqlite3* dbconn, const char* sql_Count, const char* sql_IdxAuxPair, IdxAuxSet* set)
{
    HResult retcode = HResult_OK;
    // size_t name_length = 0;
    int64_t i64 = 0;

    if (dbconn == NULL || set == NULL || sql_Count == NULL || 0 == strlen(sql_Count) || sql_IdxAuxPair == NULL || 0 == strlen(sql_IdxAuxPair))
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    retcode = sqlite3_helper_query_int64(&i64, dbconn, sql_Count);
    if (retcode != HResult_OK)
    {
        goto EXIT;
    }

    if (i64 < 0)
    {
        retcode = HResult_DATA_INVALID;
        goto EXIT;
    }

    retcode = IdxAuxSet_InitByCount(set, (const size_t)i64);
    if (retcode != HResult_OK)
    {
        goto EXIT;
    }

    if (set->count == 0)
    {
        goto EXIT;
    }

    retcode = sqlite3_helper_query_callback(dbconn, sql_IdxAuxPair, callback_RetrieveIdxAuxPair, set);
    if (retcode != HResult_OK)
    {
        goto EXIT;
    }

EXIT:
    return retcode;
}


static HResult callback_RetrieveId_ApplyIdx(sqlite3_stmt* stmt, const size_t rowIndex, void* output_buf)
{
    HResult retcode = HResult_OK;
    IdxSet* set = (IdxSet*)output_buf;
    IdxPair* item = &set->items[rowIndex];

    item->Id = (size_t)sqlite3_column_int64(stmt, 0);
    item->Idx = rowIndex;

    return retcode;
}


static HResult callback_RetrieveIdxAuxPair(sqlite3_stmt* stmt, const size_t rowIndex, void* output_buf)
{
    HResult retcode = HResult_OK;
    IdxAuxSet* set = (IdxAuxSet*)output_buf;
    IdxAuxPair* item = &set->items[rowIndex];

    item->Id = (size_t)sqlite3_column_int64(stmt, 0);
    item->Idx = (size_t)sqlite3_column_int64(stmt, 1);
    item->AuxId = (size_t)sqlite3_column_int64(stmt, 2);

    return retcode;
}

