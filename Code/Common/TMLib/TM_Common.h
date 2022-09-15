﻿#ifndef _INC_COMMON_TMLIB_TM_Common_H
#define _INC_COMMON_TMLIB_TM_Common_H

#include "../datatypes.h"
#include "../DS/RefString.h"


// =============================================
// DirIterItems
// =============================================

typedef struct
{
    RefString   ItemName;
    RefString   Fullpath;
    uint32_t    Flag; // 1: OK
    uint16_t    IterIdx;
    uint8_t     FileTestType;
    size_t      FileSize;

    uint32_t    Chksum32_0;

    uint32_t    FID;
    uint32_t    DataSetId;

} DirIterItemInfo;


#define DirIterItems_Exists     0xE001
#define DirIterItems_InsertFail 0xE002

typedef struct
{
    RefString DirPath;
    RefString DirName;
    uint16_t ItemsCount;
    DirIterItemInfo* IterItems;
    uint32_t Flag; // 1: OK
    uint32_t DataSetId;
    uint32_t h32_0;
    uint32_t h32_1;

} DirIterItems;

#endif // !_INC_COMMON_TMLIB_TM_Common_H
