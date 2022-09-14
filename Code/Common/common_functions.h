#ifndef _INC_CLANG_COMMON_common_functions_H
#define _INC_CLANG_COMMON_common_functions_H

#include "datatypes.h"

#ifdef _WIN32
#include <wchar.h>
#endif // !_WIN32

// _locale suggest value: zh_CN.UTF-8
void SetConsoleLocale(const char* _locale);

RETCODE_U8 is_path_exist(const char* path);
RETCODE_U8 is_path_accessible(const char* path, uint8_t mode);

#ifdef _WIN32
HResult Win32_PathConvert_UTF8ToUCS2(wchar_t* outbuf, uint16_t* p_wcsLen, const char* path_utf8, const uint16_t pathLen_limit);
HResult Win32_TextConvert_UTF8ToUCS2(wchar_t* outbuf, uint32_t* p_wcsLen, const char* text_utf8, const uint32_t textLen_limit);
#endif // !_WIN32

bool is_file_exist(const char* filepath);
bool is_directory_exist(const char* dirpath);

#endif // !_INC_CLANG_COMMON_common_functions_H
