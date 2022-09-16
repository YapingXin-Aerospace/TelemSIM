#ifndef _INC_COMMON_TMLIB_TMLibConfig_H
#define _INC_COMMON_TMLIB_TMLibConfig_H

#include "../datatypes.h"

#define TelemetryFrameCols  198
#define TelemetryFrameRows  64
#define TelemetryFrameSize  (TelemetryFrameCols * TelemetryFrameRows)
// 时标长度 | 8字节时间B000码，见《数据处理软件任务书》章节4.2.1 输入
#define TelemetryTimeCodeLEN 8
// 字长（单位：字节）
#define TelemetryWordLEN    1
// Before Launch | 起飞前的QF信号
#define TelemetryBFCode     0x55
// Launch code   | 起飞信号
#define TelemetryQFCode     0xAA
// KP code
#define TelemetryKPCode     0x55
// KP -> W186 (Base 1)
#define TelemetryKPColumn   186
// 子帧同步码
#define TelemetrySubFrameSyncCode 0xFAF320
#define TelemetrySubFrameSyncCode_HB (((uint32_t)TelemetrySubFrameSyncCode & 0x00FF0000) >> 16)
#define TelemetrySubFrameSyncCode_MB (((uint32_t)TelemetrySubFrameSyncCode & 0x0000FF00) >> 8)
#define TelemetrySubFrameSyncCode_LB ((uint32_t)TelemetrySubFrameSyncCode & 0x000000FF)

// 副帧同步码
#define TelemetryAuxFrameSyncCode 0x050CDF
#define TelemetryAuxFrameSyncCode_HB (((uint32_t)TelemetryAuxFrameSyncCode & 0x00FF0000) >> 16)
#define TelemetryAuxFrameSyncCode_MB (((uint32_t)TelemetryAuxFrameSyncCode & 0x0000FF00) >> 8)
#define TelemetryAuxFrameSyncCode_LB ((uint32_t)TelemetryAuxFrameSyncCode & 0x000000FF)


/* =======================================================
 * String variables length definitions
 ======================================================= */

#define ShortNameTXT_MaxSIZE    64
#define DescriptionTxtMaxSIZE   256

//#define TXTINDENTIFIER_SIZE 256

//#define GUID_TXT_SIZE       37
//
//#define UNITES_TXT_SIZE     24
//#define LONGTEXT_SIZE       4096
//#define ContentLine_SIZE    1024
//
//#define FLYSEGMENT_MSGLEN   28
//#define FLYSEGMENT_BUFLEN   (FLYSEGMENT_MSGLEN * 20)
//#define FLYSEGMENT_CPYLEN   (FLYSEGMENT_MSGLEN * 10)
//
//#define FLYHALFSEGMSGLEN    (14 * sizeof(uint16_t))
//#define FLYFULLSEGMSGLEN    (FLYHALFSEGMSGLEN * 2)


#endif // !_INC_COMMON_TMLIB_TMLibConfig_H
