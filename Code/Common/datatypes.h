#ifndef _INC_COMMON_datatypes_H
#define _INC_COMMON_datatypes_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef _WIN32
#   ifndef _SSIZE_T_DEFINED
#   define _SSIZE_T_DEFINED
#   undef ssize_t
#   ifdef _WIN64
typedef int64_t ssize_t;
#   else
typedef int ssize_t;
#   endif /* _WIN64 */
#   endif /* _SSIZE_T_DEFINED */    
#endif

    typedef float       float32_t;
    typedef double      float64_t;

    typedef uint8_t	    byte;
    typedef int8_t      sbyte;

#ifndef PATH_MAX
    #define PATH_MAX 250
#endif

// ========================================
// Definition of reserved values
// ----------------------------------------
#define UInt16_MAX      0xFFFF
#define UInt16_NOP      0xFFFF
#define UInt16_ANY      0xFFFF


#define UInt32_MAX      0xFFFFFFFF
#define UInt32_NOP      0xFFFFFFFF
#define UInt32_ANY      0xFFFFFFFF

#define UInt64_MAX      0xFFFFFFFFFFFFFFFF
#define UInt64_NOP      0xFFFFFFFFFFFFFFFF
#define UInt64_ANY      0xFFFFFFFFFFFFFFFF


#ifdef _WIN32
#ifndef INT64_MIN
#define INT64_MIN	0x8000000000000000i64
#endif
#ifndef INT64_MAX
#define INT64_MAX	0x7FFFFFFFFFFFFFFFi64
#endif
#else
#ifndef INT64_MIN
#define INT64_MIN	0x8000000000000000LL
#endif
#ifndef INT64_MAX
#define INT64_MAX	0x7FFFFFFFFFFFFFFFLL
#endif
#endif

// ========================================

#define PASS 1
#define FAIL 0

typedef uint8_t RETCODE_U8;

#define RETCODE_U8_Unknown  0
#define RETCODE_U8_OK       1
#define RETCODE_U8_FAILED   2

typedef uint32_t HResult;

#define HResult_Unknown 0
#define HResult_OK      1
#define HResult_OptSkip 2


#define HResult_FAILED              0x00100000
#define HResult_PARAM_NULL          0x00110000
#define HResult_PARAM_OUTRANGE      0x00120000
#define HResult_BUF_NOSPACE         0x00120001
#define HResult_FILE_NO_ACCESS      0x00130000
#define HResult_FILE_NOTEXISTS      0x00130001
#define HResult_FILE_CannotCreate   0x00130002
#define HResult_FILE_CannotOpen     0x00130004
#define HResult_FILE_CannotRead     0x00130008
#define HResult_FILE_CannotWrite    0x00130010
#define HResult_DIR_LOOKUP_FAILED   0x00130020
#define HResult_PATH_RetrieveFail   0x00130040
#define HResult_FILE_OPEN_FAIL      0x00130080
#define HResult_MODULE_NotFound     0x00140001
#define HResult_MODULE_LOAD_FAIL    0x00140002
#define HResult_SYMBLE_LOAD_FAIL    0x00140003

#define HResult_ALLOC_FAIL          0x00170000
#define HResult_DATA_INVALID        0x00180000
#define HResult_DATA_OUTRANGE       0x00180001
#define HResult_DATA_CONFLICT       0x00180002
#define HResult_NOT_SUPPORTED       0x00180003
#define HResult_OBJECT_IS_NULL      0x00190000
#define HResult_DECODE_FAIL         0x001B0000
#define HResult_Calc_DivOverflow    0x001C0001
#define HResult_Calc_PwrOverflow    0x001C0002
#define HResult_Calc_VarCombFail    0x001C0003

// for serial-port
#define HResult_SERIAL_FAIL         0x7D000000
#define HResult_SERIAL_OpenFAIL     0x7D000001
#define HResult_SERIAL_GetStateFAIL 0x7D000002
#define HResult_SERIAL_SetStateFAIL 0x7D000003
#define HResult_SERIAL_SetTimeoutsFAIL 0x7D000004
#define HResult_SERIAL_InvalidHandle 0x7D000005
#define HResult_SERIAL_SendFAIL     0x7D000006

// MSDN Topic: Windows Sockets Error Codes
// MSDN Link: https://docs.microsoft.com/zh-cn/windows/win32/winsock/windows-sockets-error-codes-2
#define HResult_SOCKET_FAIL               0x7E000000
#define HResult_SOCKET_WSAEINTR           (0x7E000000 + 10004)
#define HResult_SOCKET_WSAEFAULT          (0x7E000000 + 10014)
#define HResult_SOCKET_WSAEWOULDBLOCK     (0x7E000000 + 10035)
#define HResult_SOCKET_WSAEINPROGRESS     (0x7E000000 + 10036)
#define HResult_SOCKET_WSAENOTSOCK        (0x7E000000 + 10038)
#define HResult_SOCKET_WSAENETDOWN        (0x7E000000 + 10050)
#define HResult_SOCKET_WSAEPROCLIM        (0x7E000000 + 10067)
#define HResult_SOCKET_WSASYSNOTREADY     (0x7E000000 + 10091)
#define HResult_SOCKET_WSAVERNOTSUPPORTED (0x7E000000 + 10092)
#define HResult_SOCKET_WSANOTINITIALISED  (0x7E000000 + 10093)
#define HResult_SOCKET_TypeNotSupported   0x7E000001
#define HResult_SOCKET_INVALID            0x7E000002
#define HResult_SOCKET_SetOptFailed       0x7E000003
#define HResult_SOCKET_BindFailed         0x7E000004

#define HResult_CUSTOM_7F_FAIL      0x7F000000

#define HResult_SQLite3OpenFail     0x00140001
#define HResult_SQLite3QueryFail    0x00150001
#define HResult_SQLite3QueryNULL    0x00150002
#define HResult_SQLite3ExecFailed   0x00150004
#define HResult_SQLite3PrepareErr   0x00150008

#define HResult_THREAD_CREATEFAIL   0x00210001
#define HResult_MUTEX_CREATEFAIL    0x00210002

#define HResult_FILE_READ_FAIL      0x00150000
#define HResult_FILE_WRITEFAIL      0x00160000
#define HResult_STRING_TOOLONG      0x001A0000
#define HResult_LOG_SETUP_FAIL      0x001C0000
#define HResult_CONFIG_FAIL         0x001D0000


typedef HResult(*MappedFileHandle)(const byte* bytes, const size_t filesize, void* data);

#endif // !_INC_COMMON_datatypes_H
