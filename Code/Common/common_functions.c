#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>

#ifdef _WIN32
#   include <Windows.h>
#   define stat _stat
#   define stat_s struct _stat
#elif defined( __linux )
#   define stat_s struct stat
#endif // !_WIN32

#include "PosixInterface.h"
#include "common_functions.h"


// _locale suggest value: zh_CN.UTF-8
void SetConsoleLocale(const char* _locale)
{
    setlocale(LC_ALL, _locale);
}

RETCODE_U8 is_path_exist(const char* path)
{
    return is_path_accessible(path, F_OK);
}


RETCODE_U8 is_path_accessible(const char* path, uint8_t mode)
{
    RETCODE_U8 retcode = RETCODE_U8_OK;
    int retc;

#ifndef _FORCE_WCHAR_API
    retc = access(path, (int)mode);
#else
    static wchar_t path_ucs[PATH_MAX] = { 0 };
    uint16_t path_ucs_length = 0;

    memset(path_ucs, 0, PATH_MAX * sizeof(wchar_t));

    HResult rc = Win32_PathConvert_UTF8ToUCS2(path_ucs, &path_ucs_length, path, 0);
    if (rc != RETCODE_U8_OK)
    {
        retcode = RETCODE_U8_FAILED;
        goto EXIT;
    }

    if (path_ucs_length == 0)
    {
        retcode = RETCODE_U8_FAILED;
        goto EXIT;
    }

    retc = _waccess(path_ucs, (int)mode);
#endif // !_FORCE_WCHAR_API

    if (retc != 0)
    {
        retcode = RETCODE_U8_FAILED;
        goto EXIT;
    }

EXIT:
    return retcode;
}


#ifdef _WIN32
HResult Win32_PathConvert_UTF8ToUCS2(wchar_t* outbuf, uint16_t* p_wcsLen, const char* path_utf8, const uint16_t pathLen_limit)
{
    HResult retcode = HResult_OK;
    size_t csLen = 0;
    uint16_t convLength = 0;
    int wcsLen;
    int rc;

    if (path_utf8 == NULL)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    csLen = strlen(path_utf8);
    if (csLen == 0)
    {
        if (p_wcsLen)
        {
            *p_wcsLen = 0;
        }
        goto EXIT;
    }

    if (outbuf == NULL)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    if (pathLen_limit == 0)
    {
        if (csLen > UINT16_MAX)
        {
            retcode = HResult_DATA_OUTRANGE;
            goto EXIT;
        }
        convLength = (uint16_t)csLen;
    }
    else
    {
        convLength = (csLen >= pathLen_limit) ? pathLen_limit : (uint16_t)csLen;
    }

    // MSDN: https://docs.microsoft.com/zh-cn/windows/win32/api/stringapiset/nf-stringapiset-multibytetowidechar
    wcsLen = MultiByteToWideChar(CP_UTF8, 0, path_utf8, convLength, NULL, 0);
    if (wcsLen == 0)
    {
        retcode = HResult_CUSTOM_7F_FAIL + 1;
        goto EXIT;
    }

    rc = MultiByteToWideChar(CP_UTF8, 0, path_utf8, convLength, outbuf, wcsLen);
    if (rc == 0)
    {
        retcode = HResult_CUSTOM_7F_FAIL + 2;
        goto EXIT;
    }

    if (p_wcsLen)
    {
        *p_wcsLen = wcsLen;
    }

EXIT:
    return retcode;
}


HResult Win32_TextConvert_UTF8ToUCS2(wchar_t* outbuf, uint32_t* p_wcsLen, const char* text_utf8, const uint32_t textLen_limit)
{
    HResult retcode = HResult_OK;
    size_t csLen = 0;
    int convLength = 0;
    int wcsLen;
    int rc;

    if (text_utf8 == NULL)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    csLen = strlen(text_utf8);
    if (csLen == 0)
    {
        if (p_wcsLen)
        {
            *p_wcsLen = 0;
        }
        goto EXIT;
    }

    if (outbuf == NULL)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    if (textLen_limit == 0)
    {
        if (csLen >= UINT32_MAX)
        {
            retcode = HResult_DATA_OUTRANGE;
            goto EXIT;
        }
        convLength = (int)csLen;
    }
    else
    {
        convLength = (csLen >= textLen_limit) ? (int)textLen_limit : (int)csLen;
    }

    // MSDN: https://docs.microsoft.com/zh-cn/windows/win32/api/stringapiset/nf-stringapiset-multibytetowidechar
    wcsLen = MultiByteToWideChar(CP_UTF8, 0, text_utf8, convLength, NULL, 0);
    if (wcsLen == 0)
    {
        retcode = HResult_CUSTOM_7F_FAIL + 1;
        goto EXIT;
    }

    rc = MultiByteToWideChar(CP_UTF8, 0, text_utf8, convLength, outbuf, wcsLen);
    if (rc == 0)
    {
        retcode = HResult_CUSTOM_7F_FAIL + 2;
        goto EXIT;
    }

    if (p_wcsLen)
    {
        *p_wcsLen = (uint32_t)wcsLen;
    }

EXIT:
    return retcode;
}
#endif // !_WIN32


bool is_file_exist(const char* filepath)
{
    /*stat_s st = { 0 };
    stat(filepath, &st);
    return S_IFREG == (st.st_mode & S_IFREG);*/
    return RETCODE_U8_OK == is_path_exist(filepath);
}

bool is_directory_exist(const char* dirpath)
{
    stat_s st = { 0 };
    stat(dirpath, &st);
    return S_IFDIR == (st.st_mode & S_IFDIR);
}
