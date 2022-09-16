#ifndef _INC_UTILITY_APP_SQLITETOOLS_DRV_Config_H
#define _INC_UTILITY_APP_SQLITETOOLS_DRV_Config_H

#include "Common/datatypes.h"

typedef struct
{
    const char* dbpath;

} DRVConfig;

DRVConfig* get_G_DRVConfig();


typedef struct
{
    uint32_t BatchTrigCnt;
    uint32_t BatchCounter;

    void* entity;

} DataRecordBatchHandle;


#endif // !_INC_UTILITY_APP_SQLITETOOLS_DRV_Config_H
