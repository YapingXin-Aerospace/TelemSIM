#ifndef _INC_COMMON_TMLIB_TM_Common_H
#define _INC_COMMON_TMLIB_TM_Common_H

#include "TM_Types.h"
#include "TMLibConfig.h"
#include "TM_CrossFrame_Def.h"

#include "../FileIO/TxtFileBuf.h"

#include "Common/DS/RefString.h"
#include "Common/common_FileSummary.h"

#include "Common/FileIO/BinFileBuf.h"


/* 现在实际使用到的类型有 1 2 3 4 5 6 7 10
   数据类型；
    1 uint8_t   unsigned char
    2 int8_t    char
    3 uint16_t  unsigned short
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
uint8_t TMLib_get_datatype_length(const uint8_t datatype);


// Fundamental Datatypes
// https://www.sqlite.org/c3ref/c_blob.html

#ifndef SQLITE_INTEGER
#define SQLITE_INTEGER  1
#endif // !SQLITE_INTEGER

#ifndef SQLITE_FLOAT
#define SQLITE_FLOAT    2
#endif // !SQLITE_FLOAT

#ifndef SQLITE_TEXT
#define SQLITE_TEXT     3
#endif // !SQLITE_TEXT


#ifndef SQLITE_BLOB
#define SQLITE_BLOB     4
#endif // !SQLITE_BLOB

#ifndef SQLITE_NULL
#define SQLITE_NULL     5
#endif // !SQLITE_NULL

#define DATATYPE_UInt64 TMSrcDATATYPE_UInt64

// 针对 datatype 返回SQLite类型簇，包括 SQLITE_INTEGER, SQLITE_FLOAT, SQLITE_BLOB
uint8_t TMLib_get_datatype_SQLITE_TYPECATEGORY(const uint8_t datatype);

HResult TMLib_SrcToIntVal(int64_t* p_OutVal, const uint8_t datatype, byte* databuf, const uint8_t datalen);

HResult TMLib_SrcToUInt64(uint64_t* p_OutVal, const uint8_t datatype, byte* databuf, const uint8_t datalen);

HResult TMLib_SrcToDblVal(double* p_OutVal, const uint8_t datatype, byte* databuf, const uint8_t datalen);

/* Bits reader */

// Notice:  [1] This function only works for `Little Endian` scenarios.
HResult TMLib_SrcBitsReaderUInt32(uint32_t* p_OutVal, byte* databuf, const uint8_t datalen, const uint8_t bitsnum, const uint8_t offset);

/* Bytes reader */

// Notice:  [1] This function only works for `Little Endian` scenarios.
HResult TMLib_SrcBytesReaderUInt32(uint32_t* p_OutVal, byte* databuf, const uint8_t datalen);

// Notice:  [1] This function only works for `Big Endian` scenarios.
HResult TMLib_SrcBytesReaderUInt32Reverse(uint32_t* p_OutVal, byte* databuf, const uint8_t datalen);

// =============================================
// DirIterItems
// =============================================

typedef struct
{
    RefString ItemName;
    RefString Fullpath;
    uint32_t Flag; // 1: OK
    uint16_t IterIdx;
    uint8_t FileTestType;
    size_t FileSize;

    uint32_t Checksum32;
    Checksum_MD5_Result MD5;
    Checksum_SHA256_Result SHA256;

    uint32_t FID;
    uint32_t DataSetId;

    BinFileReader Reader;

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


HResult get_dir_iter_items(DirIterItems* iterItems, const char* dirpath, const char* ext);

void DirIterItems_Cleanup(DirIterItems* iterItems);

// =============================================
// Integration functions definition
// =============================================


// DatatypeCategory:
// #define SQLITE_INTEGER  1
// #define SQLITE_FLOAT    2
// #define SQLITE_TEXT     3
// #define SQLITE_BLOB     4
// #define SQLITE_NULL     5

typedef struct
{
    int64_t  SrcIntVal; // 原码整型值
    double   SrcDblVal; // 原码浮点值
    uint64_t SrcUInt64; // 超长整型值
    uint8_t  DataType;  // 原码数据类型
    uint8_t  DatatypeCategory; // 类型簇

} TMSrcVal;

HResult TMLib_SrcToVal(TMSrcVal* srcval, const uint8_t datatype, byte* databuf, const uint8_t datalen, const uint8_t bitsnum, const uint8_t offset);


typedef struct
{
    int64_t  IntVal; // 原码整型值
    double   DblVal; // 原码浮点值
    uint8_t  CstrLen;
    char     CstrVal[TXTINDENTIFIER_SIZE];
    char     Unit[UNITES_TXT_SIZE];
    uint8_t  DatatypeCategory; // 类型簇

} TMDecVal;


#define TMCALTYPE_FormulaStep   1
#define TMCALTYPE_FuncSteps     2
#define TMCALTYPE_CriterionEnum 3

typedef struct
{
    uint8_t  CalType;
    uint16_t CalId;
    uint16_t CalIdx;

} TMCalParam;


typedef struct
{
    uint32_t RecId;
    uint32_t RecIdx;
    uint32_t MsgId;
    uint32_t MsgIdx;
    uint32_t CriterionId;
    uint32_t CriterionIdx;
    uint8_t EncType;
    uint32_t ExtType;
    uint32_t ExtIdx;
    uint16_t WordID;
    char WordName[TXTINDENTIFIER_SIZE];
    char WordTag[UNITES_TXT_SIZE];
    uint8_t WordPad;
    char FDesp[TXTINDENTIFIER_SIZE];
    uint16_t DataOffset;
    uint8_t DataBytes;
    uint8_t DataBitNum;
    uint8_t DataBitOffset;
    uint32_t CriterionInfoIdx;

} Criterion1553B;


typedef struct
{
    uint32_t Idx;
    uint32_t RowId;
    uint32_t Id;
    uint16_t MsgType;
    char msg_id[GUID_TXT_SIZE];
    uint32_t MsgId;
    uint32_t MsgIdx;
    char criterion_id[GUID_TXT_SIZE];
    uint32_t CriterionId;
    uint32_t CriterionIdx;
    char CriterionName[TXTINDENTIFIER_SIZE];

    /* 数据类型；
        1 uint8_t   unsigned char
        2 int8_t    char
        3 uint16_t  unsigned short
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
    uint8_t DataType;

    /* 解析类型（含义与 criterion_category 表中的 parse_type 字段相同）
        0： 以太网数据帧
        1： 1553B数据
        2： I2C数据
        3： 地面电源遥测数据
        4： 飞控计算机字数据
        5： 1553B - GNSS源码数据（数据处理和判读客户端使用)
        6： GNSS定位
        7： CPU组合导航
        8： 飞行控制I2C数据（1级综合I2C）
        9： 智能测控I2C数据（CPU I2C）
       10： I2C数据（2级综合I2C）
       11： I2C数据（1级采编组合I2C）
       12： 冲击数据
    */
    uint8_t ParseCode;

    uint16_t DataOffset;
    uint8_t DataBitNum;
    uint8_t DataBitOffset;
    uint8_t IsCriterion;    // 是否判读；0：否；1：是
    uint8_t CriterionType;  // 验证方式类型: 0:不需要判断; 1: ==; 2: ±; 3: ≥; 4: ≤; 5: 表示范围; 6: 表示需缓存记录进行异或操作，后紧跟异或次数;
    double CriterionValue;
    double CriterionOffsetValue;
    double CriterionValueOther;

    uint8_t IsTranslate;    // 是否转换；0：不转换；1：转换

    /* 转换类型
        0：比列转换(已废弃不用 * ，后将改为默认0不转换)
        1：公式转换
        2：Val转注释
        3：时间戳 转 年 / 月 / 日 / 时 / 分 / 秒
        4：转为十六进制
        5：特殊公式转换(Val为0时转注释, 不为0转公式)
        6：时间戳 转 时 / 分 / 秒
        7：时间戳 转 分 / 秒 / 毫秒
        8:  补码转原码(暂不启用*)
        10：电量转换(遥测)
        11：GNSS卫星定位状态码
    */
    uint8_t TranslateType;

    char TranslateExpression[TXTINDENTIFIER_SIZE]; // 公式转换（eg: a*x*x+b*x+c）

    char Units[UNITES_TXT_SIZE];

    /* 注释类型
        0：默认值（无注释）
        1：Val直接匹配注释
        2：判读结果转注释
    */
    uint8_t CommentType;

    char Comment[TXTINDENTIFIER_SIZE]; // 注释翻译格式：0:正常,1:不正常

    char DeviceId[GUID_TXT_SIZE];

    char CorrelationCriterionId[GUID_TXT_SIZE];
    char criterion_category_id[GUID_TXT_SIZE];
    char CriterionCategoryMainId[GUID_TXT_SIZE];

    uint32_t CriterionCategoryId;

    uint8_t ColumnNo;       // 数据处理使用
    uint8_t IsAlgorithm;    // 是否是飞控计算机字缓存解析数据
    uint8_t IsStatusRecord; // 开关状态变化记录标志位(默认为0) 默认为0: 不需要记录; 1: 将该变量状态变化时间记录到表criterion_status_record 中;
    uint8_t IsTransmit;     // 是否转发

    /* 健康监测开关显示 （默认0，客户端使用）
        1: 表示主控显示开关
        2: 表示健康监测显示开关（绿灯）
        3: 表示飞控算法物理量
        4: 表示健康监测显示开关（红灯）
        5: 百分比仪表盘
        6: 指针式仪表盘
        7: 数值式仪表盘
        8: 剩余时间仪表盘
        9: 重复变量
       10: 保持灯
       11: 时序灯
       12: 时序名称
       13: 电流电压图表
       14: 电池容量图表
    */
    uint8_t Type;

    uint32_t LampOrder; // 健康监测客户端顶部灯排序字段，如果为空则不再顶部显示

    uint8_t CriterionValueType; // 0: 不随流程做判读; 1: 跟随流程做判读
    uint8_t IsSymbol;           // 符号位（数据处理软件使用）0.无符号位; 1.是符号位

    /* 特殊转换类型：
        1.补码转换
        2.高字低字转换(高字在前，低字在后)
        3.处理全帧计数三个字节
        4.时标替换为时间戳 + 高字低字转换
        5.时标替换为时间戳
    */
    uint8_t SpecialConvertType;
    uint8_t EffectiveLen;       // 解析后的数据有效位数 3：默认为3位小数; 6：飞控计算机字类别的有效位为 6位小数

    uint16_t CalType;
    uint16_t CalId;
    uint16_t CalIdx;

} CriterionInfo;


// =============================================
// Integration functions definition
// =============================================

typedef struct
{
    uint16_t id;
    uint16_t idx;
    char msg_name[PATH_MAX];
    char desp[PATH_MAX];
    uint16_t FlyPhaseId;
    uint16_t period;
    char bh_txt[PATH_MAX];
    uint16_t hdr;
    uint32_t flag;
    uint16_t wordcnt;
    uint8_t hdr_b0;
    uint8_t hdr_b1;

} FlySegmentMsg;

typedef struct
{
    uint16_t count;
    FlySegmentMsg* items;

} FlySegmentMsgSet;


HResult FlySegmentMsgSet_InitByCount(FlySegmentMsgSet* set, const uint16_t count);
void FlySegmentMsgSet_Release(FlySegmentMsgSet* set);


// =============================================
// For HF Export :: Begin
// =============================================

typedef struct
{
    uint32_t CriterionIdx;
    uint32_t CriterionId;
    uint8_t DataType;
    uint8_t DataOffset;
    uint8_t ColumnNo;
    uint8_t DataBitNum;
    uint8_t DataBitOffset;

} CriterionExpItem;


typedef struct
{
    uint16_t Idx;
    uint16_t Id;
    uint16_t TN;
    uint16_t OID;
    uint32_t CriterionIdx;
    uint32_t CriterionId;
    char CriterionName[TXTINDENTIFIER_SIZE];
    char CategoryName[TXTINDENTIFIER_SIZE];
    uint16_t Points;
    uint8_t Colscnt;
    uint8_t Colsel;
    uint8_t ColFull;
    uint8_t DataType;
    uint8_t TranslateType;
    uint8_t CommentType;
    char Units[UNITES_TXT_SIZE];
    uint32_t Flag;

    uint16_t ItemsCount;
    CriterionExpItem* ExpItems;
    CriterionExpItem* PosItems;

} CriterionExport;

typedef struct
{
    uint16_t count;
    CriterionExport* items;

} CriterionExportSet;

HResult CriterionExportSet_InitByCount(CriterionExportSet* set, const uint16_t count);
void CriterionExportSet_Release(CriterionExportSet* set);

HResult CriterionExport_InitExpItemsByCount(CriterionExport* expEntity, const uint16_t items_count);
void CriterionExport_ReleaseExpItems(CriterionExport* expEntity);

// =============================================
// For HF Export :: END
// =============================================

typedef struct
{
    uint64_t MinRecId;
    uint64_t MaxRecId;
    uint32_t count;
    TMSrcRec* items;

} TMSrcRecSet;

HResult TMSrcRecSet_InitByCount(TMSrcRecSet* set, const uint32_t count);
void TMSrcRecSet_Release(TMSrcRecSet* set);

#endif // !_INC_COMMON_TMLIB_TM_Common_H
