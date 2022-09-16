#ifndef _INC_COMMON_TMLIB_TMLibConfig_H
#define _INC_COMMON_TMLIB_TMLibConfig_H

#include "../datatypes.h"

#define TelemetryFrameCols  198
#define TelemetryFrameRows  64
#define TelemetryFrameSize  (TelemetryFrameCols * TelemetryFrameRows)
// ʱ�곤�� | 8�ֽ�ʱ��B000�룬�������ݴ�����������顷�½�4.2.1 ����
#define TelemetryTimeCodeLEN 8
// �ֳ�����λ���ֽڣ�
#define TelemetryWordLEN    1
// Before Launch | ���ǰ��QF�ź�
#define TelemetryBFCode     0x55
// Launch code   | ����ź�
#define TelemetryQFCode     0xAA
// KP code
#define TelemetryKPCode     0x55
// KP -> W186 (Base 1)
#define TelemetryKPColumn   186
// ��֡ͬ����
#define TelemetrySubFrameSyncCode 0xFAF320
#define TelemetrySubFrameSyncCode_HB (((uint32_t)TelemetrySubFrameSyncCode & 0x00FF0000) >> 16)
#define TelemetrySubFrameSyncCode_MB (((uint32_t)TelemetrySubFrameSyncCode & 0x0000FF00) >> 8)
#define TelemetrySubFrameSyncCode_LB ((uint32_t)TelemetrySubFrameSyncCode & 0x000000FF)

// ��֡ͬ����
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
