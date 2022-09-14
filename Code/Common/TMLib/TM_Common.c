#include <stdlib.h>
#include <memory.h>
#include "TM_Common.h"

// =============================================
// Internal struct definitions
// =============================================

union Float32Bytes4
{
    float32_t val;
    byte raw[4];
};

union Float64Bytes8
{
    float64_t val;
    byte raw[8];
};

union UInt32Bytes4
{
    uint32_t val;
    byte raw[4];
};

// =============================================
// Internal (private) functions definition
// =============================================

// Specification: Read `datalen` bytes from buffer `databuf` as uint32_t value.
// Notice:  [1] This function only works for `Little Endian` scenarios.
//          [2] This function will *NOT* check the input parameters, so
//              pease ***make sure*** that databuf is NOT NULL and datalen in [1..4]
static inline uint32_t TMLib_SrcBytes_GetUInt32(byte* databuf, const uint8_t datalen);

// Specification: Read `datalen` bytes from buffer `databuf` as uint32_t value.
// Notice:  [1] This function only works for `Big Endian` scenarios.
//          [2] This function will *NOT* check the input parameters, so
//              pease ***make sure*** that databuf is NOT NULL and datalen in [1..4]
static inline uint32_t TMLib_SrcBytes_GetUInt32Reverse(byte* databuf, const uint8_t datalen);

static void CriterionExport_Release(CriterionExport* item);

// =============================================
// Public functions implementation
// =============================================

/* 数据类型；
    1 uint8_t   unsigned char
    2 int8_t    char
    3 uint16_t   unsigned short
    4 int16_t   short
    5 uint32_t  unsigned int
    6 int32_t   int
    7 float32_t float
    8 uint64_t  unsigned long
    9 int64_t   long
    10 float64_t double
    11 uint24_t  全帧计数类型

    (11~16暂不启用）
    11 一字节补码
    12 两字节补码
    13 四字节补码
    14 8字节补码
    15 字内小端两字大端
    16 字内小端四字大端
*/
uint8_t TMLib_get_datatype_length(const uint8_t datatype)
{
    switch (datatype)
    {
    case 1:
    case 2:
        return 1;
    case 3:
    case 4:
        return 2;
    case 5:
    case 6:
    case 7:
        return 4;
    case 8:
    case 9:
    case 10:
        return 8;
    case 11:
        return 3;
    default:
        return 0;
    }
}


// 针对 datatype 返回SQLite类型簇，包括 SQLITE_INTEGER, SQLITE_FLOAT, SQLITE_BLOB
uint8_t TMLib_get_datatype_SQLITE_TYPECATEGORY(const uint8_t datatype)
{
    switch (datatype)
    {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 9:
    case 11:
        return SQLITE_INTEGER;
    case 7:
    case 10:
        return SQLITE_FLOAT;
    case 8:
        return DATATYPE_UInt64;
    default:
        return 0;
    }
}


HResult TMLib_SrcToIntVal(int64_t* p_OutVal, const uint8_t datatype, byte* databuf, const uint8_t datalen)
{
    HResult retcode = HResult_OK;

    if (p_OutVal == NULL || databuf == NULL)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    if (SQLITE_INTEGER != TMLib_get_datatype_SQLITE_TYPECATEGORY(datatype))
    {
        retcode = HResult_PARAM_OUTRANGE;
        goto EXIT;
    }

    if (datalen < TMLib_get_datatype_length(datatype))
    {
        retcode = HResult_DATA_INVALID;
        goto EXIT;
    }

    switch (datatype)
    {
    case 1:
        *p_OutVal = (uint8_t)databuf[0];
        break;
    case 2:
        *p_OutVal = (int8_t)databuf[0];
        break;
    case 3: // uint16_t
        {
            /*
            uint16_t value = ((uint16_t)databuf[0] & 0x00FF);
            value |= (((uint16_t)databuf[1] << 8)  & 0xFF00);
            */
            uint16_t value = 0;
            memcpy(&value, databuf, 2);

            *p_OutVal = (int64_t)value;
        }
        break;
    case 4: // int16_t
        {
            /*
            int16_t value = ((uint16_t)databuf[0] & 0x00FF);
            value |= (((uint16_t)databuf[1] << 8) & 0xFF00);
            */
            int16_t value = 0;
            memcpy(&value, databuf, 2);

            *p_OutVal = (int64_t)value;
        }
        break;
    case 5: // uint32_t
        {
            /*
            uint32_t value = ((uint32_t)databuf[0] & 0x000000FF);
            value |= (((uint32_t)databuf[1] << 8)  & 0x0000FF00);
            value |= (((uint32_t)databuf[2] << 16) & 0x00FF0000);
            value |= (((uint32_t)databuf[3] << 24) & 0xFF000000);
            */
            uint32_t value = 0;
            memcpy(&value, databuf, 4);

            *p_OutVal = (int64_t)value;
        }
        break;
    case 6: // int32_t
        {
            /*
            int32_t value = ((int32_t)databuf[0]   & 0x000000FF);
            value |= (((uint32_t)databuf[1] << 8)  & 0x0000FF00);
            value |= (((uint32_t)databuf[2] << 16) & 0x00FF0000);
            value |= (((uint32_t)databuf[3] << 24) & 0xFF000000);
            */
            int32_t value = 0;
            memcpy(&value, databuf, 4);

            *p_OutVal = (int64_t)value;
        }
        break;
    case 9: // int64_t
        {
            /*
            int64_t value = ((int64_t)databuf[0]   & 0x00000000000000FF);
            value |= (((uint64_t)databuf[1] << 8)  & 0x000000000000FF00);
            value |= (((uint64_t)databuf[2] << 16) & 0x0000000000FF0000);
            value |= (((uint64_t)databuf[3] << 24) & 0x00000000FF000000);
            value |= (((uint64_t)databuf[4] << 32) & 0x000000FF00000000);
            value |= (((uint64_t)databuf[5] << 40) & 0x0000FF0000000000);
            value |= (((uint64_t)databuf[6] << 48) & 0x00FF000000000000);
            value |= (((uint64_t)databuf[7] << 56) & 0xFF00000000000000);
            */
            int64_t value = 0;
            memcpy(&value, databuf, 8);

            *p_OutVal = (int64_t)value;
        }
        break;
    case 11: // uint24_t
        {
            /*
            uint32_t value = ((uint32_t)databuf[0] & 0x000000FF);
            value |= (((uint32_t)databuf[1] << 8)  & 0x0000FF00);
            value |= (((uint32_t)databuf[2] << 16) & 0x00FF0000);
            */
            uint32_t value = 0;
            memcpy(&value, databuf, 3);

            *p_OutVal = (int64_t)value;
        }
        break;
    default:
        retcode = HResult_NOT_SUPPORTED;
        break;
    }

EXIT:
    return retcode;
}


HResult TMLib_SrcToUInt64(uint64_t* p_OutVal, const uint8_t datatype, byte* databuf, const uint8_t datalen)
{
    HResult retcode = HResult_OK;

    if (p_OutVal == NULL || databuf == NULL)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    if (DATATYPE_UInt64 != TMLib_get_datatype_SQLITE_TYPECATEGORY(datatype))
    {
        retcode = HResult_PARAM_OUTRANGE;
        goto EXIT;
    }

    if (datalen < TMLib_get_datatype_length(datatype))
    {
        retcode = HResult_DATA_INVALID;
        goto EXIT;
    }

    switch (datatype)
    {
    case 8: // uint64_t
        {
            /*
            uint64_t value = ((uint64_t)databuf[0] & 0x00000000000000FF);
            value |= (((uint64_t)databuf[1] << 8)  & 0x000000000000FF00);
            value |= (((uint64_t)databuf[2] << 16) & 0x0000000000FF0000);
            value |= (((uint64_t)databuf[3] << 24) & 0x00000000FF000000);
            value |= (((uint64_t)databuf[4] << 32) & 0x000000FF00000000);
            value |= (((uint64_t)databuf[5] << 40) & 0x0000FF0000000000);
            value |= (((uint64_t)databuf[6] << 48) & 0x00FF000000000000);
            value |= (((uint64_t)databuf[7] << 56) & 0xFF00000000000000);
            */
            uint64_t value = 0;
            memcpy(&value, databuf, 8);

            *p_OutVal = (int64_t)value;
        }
        break;
    default:
        retcode = HResult_NOT_SUPPORTED;
        break;
    }

EXIT:
    return retcode;
}


HResult TMLib_SrcToDblVal(double* p_OutVal, const uint8_t datatype, byte* databuf, const uint8_t datalen)
{
    HResult retcode = HResult_OK;

    if (p_OutVal == NULL || databuf == NULL)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    if (SQLITE_FLOAT != TMLib_get_datatype_SQLITE_TYPECATEGORY(datatype))
    {
        retcode = HResult_PARAM_OUTRANGE;
        goto EXIT;
    }

    if (datalen < TMLib_get_datatype_length(datatype))
    {
        retcode = HResult_DATA_INVALID;
        goto EXIT;
    }

    switch (datatype)
    {
    case 7: // float32_t
        {
            union Float32Bytes4 value = { 0 };
            /*
            value.raw[0] = databuf[0];
            value.raw[1] = databuf[1];
            value.raw[2] = databuf[2];
            value.raw[3] = databuf[3];
            */
            memcpy(value.raw, databuf, 4);

            *p_OutVal = (double)value.val;
        }
        break;
    case 10: // float64_t
        {
            union Float64Bytes8 value = { 0 };
            /*
            value.raw[0] = databuf[0];
            value.raw[1] = databuf[1];
            value.raw[2] = databuf[2];
            value.raw[3] = databuf[3];
            value.raw[4] = databuf[4];
            value.raw[5] = databuf[5];
            value.raw[6] = databuf[6];
            value.raw[7] = databuf[7];
            */
            memcpy(value.raw, databuf, 8);

            *p_OutVal = value.val;
        }
        break;
    default:
        retcode = HResult_NOT_SUPPORTED;
        break;
    }

EXIT:
    return retcode;
}


/* Bits reader */


// Notice:  [1] This function only works for `Little Endian` scenarios.
HResult TMLib_SrcBitsReaderUInt32(uint32_t* p_OutVal, byte* databuf, const uint8_t datalen, const uint8_t bitsnum, const uint8_t offset)
{
    HResult retcode = HResult_OK;

    if (p_OutVal == NULL || databuf == NULL)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    if (datalen == 0 || datalen > 4 || (offset + bitsnum) > (datalen * 8))
    {
        retcode = HResult_PARAM_OUTRANGE;
        goto EXIT;
    }

    uint32_t srcData = TMLib_SrcBytes_GetUInt32(databuf, datalen);
    uint32_t mask = (((uint32_t)1 << bitsnum) - 1);
    uint32_t value = ((srcData >> offset) & mask);

    *p_OutVal = value;

EXIT:
    return retcode;
}


/* Bytes reader */


// Notice:  [1] This function only works for `Little Endian` scenarios.
HResult TMLib_SrcBytesReaderUInt32(uint32_t* p_OutVal, byte* databuf, const uint8_t datalen)
{
    HResult retcode = HResult_OK;

    if (p_OutVal == NULL || databuf == NULL)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    if (datalen == 0 || datalen > 4)
    {
        retcode = HResult_PARAM_OUTRANGE;
        goto EXIT;
    }

    *p_OutVal = TMLib_SrcBytes_GetUInt32(databuf, datalen);

EXIT:
    return retcode;
}


// Notice:  [1] This function only works for `Big Endian` scenarios.
HResult TMLib_SrcBytesReaderUInt32Reverse(uint32_t* p_OutVal, byte* databuf, const uint8_t datalen)
{
    HResult retcode = HResult_OK;

    if (p_OutVal == NULL || databuf == NULL)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    if (datalen == 0 || datalen > 4)
    {
        retcode = HResult_PARAM_OUTRANGE;
        goto EXIT;
    }

    *p_OutVal = TMLib_SrcBytes_GetUInt32Reverse(databuf, datalen);

EXIT:
    return retcode;
}


// =============================================
// DirIterItems
// ============================================

static void DirIterItemInfo_Cleanup(DirIterItemInfo* itemInfo);
static void DirIterItemInfo_Display(DirIterItemInfo* itemInfo);


void DirIterItems_Cleanup(DirIterItems* iterItems)
{
    uint16_t idx;
    DirIterItemInfo* itemInfo;

    if (iterItems)
    {
        RefString_Release(&iterItems->DirPath);
        RefString_Release(&iterItems->DirName);

        if (iterItems->ItemsCount > 0)
        {
            for (idx = 0; idx < iterItems->ItemsCount; idx++)
            {
                itemInfo = &iterItems->IterItems[idx];
                DirIterItemInfo_Cleanup(itemInfo);
            }
            free(iterItems->IterItems);
            iterItems->IterItems = NULL;
            iterItems->ItemsCount = 0;
        }
    }
}


static void DirIterItemInfo_Cleanup(DirIterItemInfo* itemInfo)
{
    if (itemInfo)
    {
        RefString_Release(&itemInfo->ItemName);
        RefString_Release(&itemInfo->Fullpath);
        BinFileReader_Release(&itemInfo->Reader);
    }
}


// =============================================
// Integration functions implementation
// =============================================

HResult TMLib_SrcToVal(TMSrcVal* srcval, const uint8_t datatype, byte* databuf, const uint8_t datalen, const uint8_t bitsnum, const uint8_t offset)
{
    HResult retcode = HResult_OK;

    if (srcval == NULL || databuf == NULL)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    memset(srcval, 0, sizeof(TMSrcVal));
    srcval->DataType = datatype;
    srcval->DatatypeCategory = TMLib_get_datatype_SQLITE_TYPECATEGORY(datatype);
    switch (srcval->DatatypeCategory)
    {
    case SQLITE_INTEGER:
        if (bitsnum == 0)
        {
            retcode = TMLib_SrcToIntVal(&srcval->SrcIntVal, datatype, databuf, datalen);
        }
        else
        {
            uint32_t srcVal = 0;
            retcode = TMLib_SrcBitsReaderUInt32(&srcVal, databuf, datalen, bitsnum, offset);
            if (retcode == HResult_OK)
            {
                srcval->SrcIntVal = srcVal;
            }
        }
        break;
    case SQLITE_FLOAT:
        retcode = TMLib_SrcToDblVal(&srcval->SrcDblVal, datatype, databuf, datalen);
        break;
    case DATATYPE_UInt64:
        retcode = TMLib_SrcToUInt64(&srcval->SrcUInt64, datatype, databuf, datalen);
    default:
        retcode = HResult_NOT_SUPPORTED;
        break;
    }

EXIT:
    return retcode;
}


HResult FlySegmentMsgSet_InitByCount(FlySegmentMsgSet* set, const uint16_t count)
{
    HResult retcode = HResult_OK;

    if (set == NULL)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    set->count = count;
    if (set->count > 0)
    {
        set->items = malloc(set->count * sizeof(FlySegmentMsg));
        if (!set->items)
        {
            retcode = HResult_ALLOC_FAIL;
            goto EXIT;
        }
        memset(set->items, 0, set->count * sizeof(FlySegmentMsg));
    }

EXIT:
    return retcode;
}


void FlySegmentMsgSet_Release(FlySegmentMsgSet* set)
{
    if (set)
    {
        if (set->items)
        {
            /*for (uint16_t idx = 0; idx < set->count; idx++)
            {
                FlySegmentMsg* item = &set->items[idx];
            }*/

            free(set->items);
            set->items = NULL;
        }
        set->count = 0;
    }
}


// =============================================
// Internal (private) functions implementation
// =============================================

// Specification: Read `datalen` bytes from buffer `databuf` as uint32_t value.
// Notice:  [1] This function only works for `Little Endian` scenarios.
//          [2] This function will *NOT* check the input parameters, so
//              pease ***make sure*** that databuf is NOT NULL and datalen in [1..4]
static inline uint32_t TMLib_SrcBytes_GetUInt32(byte* databuf, const uint8_t datalen)
{
    uint32_t value = 0;
    memcpy(&value, databuf, datalen);
    return value;
}


// Specification: Read `datalen` bytes from buffer `databuf` as uint32_t value.
// Notice:  [1] This function only works for `Big Endian` scenarios.
//          [2] This function will *NOT* check the input parameters, so
//              pease ***make sure*** that databuf is NOT NULL and datalen in [1..4]
static inline uint32_t TMLib_SrcBytes_GetUInt32Reverse(byte* databuf, const uint8_t datalen)
{
    union UInt32Bytes4 value = { 0 };
    for (uint8_t idx = 0; idx < datalen; idx++)
    {
        value.raw[datalen - idx - 1] = databuf[idx];
    }
    return value.val;
}


// =============================================
// For HF Export :: Begin
// =============================================

static void CriterionExport_Release(CriterionExport* item)
{
    if (item)
    {
        if (item->PosItems)
        {
            free(item->PosItems);
            item->PosItems = NULL;
        }
    }
}

HResult CriterionExportSet_InitByCount(CriterionExportSet* set, const uint16_t count)
{
    HResult retcode = HResult_OK;

    if (set == NULL)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    set->count = count;
    if (set->count > 0)
    {
        set->items = malloc(set->count * sizeof(CriterionExport));
        if (!set->items)
        {
            retcode = HResult_ALLOC_FAIL;
            goto EXIT;
        }
        memset(set->items, 0, set->count * sizeof(CriterionExport));
    }
    else
    {
        set->items = NULL;
    }

EXIT:
    return retcode;
}

void CriterionExportSet_Release(CriterionExportSet* set)
{
    if (set)
    {
        if (set->items != NULL)
        {
            if (set->count > 0)
            {
                for (uint16_t idx = 0; idx < set->count; idx++)
                {
                    CriterionExport* item = &set->items[idx];
                    CriterionExport_ReleaseExpItems(item);
                    CriterionExport_Release(item);
                }
            }

            set->count = 0;
            free(set->items);
            set->items = NULL;
        }
    }
}


HResult CriterionExport_InitExpItemsByCount(CriterionExport* expEntity, const uint16_t items_count)
{
    HResult retcode = HResult_OK;

    if (expEntity == NULL)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    expEntity->ItemsCount = items_count;
    if (expEntity->ItemsCount > 0)
    {
        expEntity->ExpItems = malloc(expEntity->ItemsCount * sizeof(CriterionExpItem));
        if (!expEntity->ExpItems)
        {
            retcode = HResult_ALLOC_FAIL;
            goto EXIT;
        }
        memset(expEntity->ExpItems, 0, expEntity->ItemsCount * sizeof(CriterionExpItem));
    }
    else
    {
        expEntity->ExpItems = NULL;
    }

EXIT:
    return retcode;
}

void CriterionExport_ReleaseExpItems(CriterionExport* expEntity)
{
    if (expEntity)
    {
        if (expEntity->ExpItems)
        {
            free(expEntity->ExpItems);
            expEntity->ExpItems = NULL;
        }
        expEntity->ItemsCount = 0;
    }
}

// =============================================
// For HF Export :: END
// =============================================

HResult TMSrcRecSet_InitByCount(TMSrcRecSet* set, const uint32_t count)
{
    HResult retcode = HResult_OK;

    if (set == NULL)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    set->count = count;
    if (set->count > 0)
    {
        set->items = malloc(set->count * sizeof(TMSrcRec));
        if (!set->items)
        {
            retcode = HResult_ALLOC_FAIL;
            goto EXIT;
        }
        memset(set->items, 0, set->count * sizeof(TMSrcRec));
    }
    else
    {
        set->items = NULL;
    }

EXIT:
    return retcode;
}

void TMSrcRecSet_Release(TMSrcRecSet* set)
{
    if (set)
    {
        if (set->items)
        {
            free(set->items);
            set->items = NULL;
        }
        set->count = 0;
        set->MinRecId = 0;
        set->MaxRecId = 0;
    }
}
