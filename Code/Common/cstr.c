#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "cstr.h"

#include "PosixInterface.h"


// 函数功能描述：1. 返回字符串从左到右第一个非空白符的字符的指针 p_Begin ;
//             2. 如果 p_Length 不为NULL，则通过它输出左侧右侧都去除`空白符`之后的字符串的长度.
//             3. 该函数并不真的移除字符串左右两侧的`空白符`，而是给出左侧非空白符的字符的指针和假设去除`空白符`之后的字符串的长度.
// 注意：函数判断`空白符`与`非空白符`的依据是 isspace() 函数.
// Reference [0]: https://linux.die.net/man/3/isspace
//           [1]: https://www.runoob.com/cprogramming/c-function-isspace.html
const char* cstr_trim_space(const char* cstr, size_t* p_Length)
{
    const char* p_Begin = NULL;
    size_t length = 0;
    size_t trimlength = 0;
    size_t idx_0;
    size_t idx_1;

    if (cstr == NULL || (length = strlen(cstr)) == 0)
    {
        goto EXIT;
    }

    for (idx_0 = 0; idx_0 < length; idx_0++)
    {
        p_Begin = &cstr[idx_0];
        if (p_Begin[0] < 0 || 0 == isspace(p_Begin[0]))
        {
            break;
        }
    }

    if (idx_0 >= length)
    {
        p_Begin = NULL;
        goto EXIT;
    }

    if (p_Length == NULL)
    {
        goto EXIT;
    }

    trimlength = 1;

    if (idx_0 == length - 1)
    {
        goto EXIT;
    }

    for (idx_1 = length - 1; idx_1 > idx_0; idx_1--)
    {
        if (cstr[idx_1] < 0 || 0 == isspace(cstr[idx_1]))
        {
            trimlength = (idx_1 - idx_0 + 1);
            break;
        }
    }

EXIT:
    if (p_Length)
    {
        *p_Length = trimlength;
    }
    return p_Begin;
}


// 函数功能描述: 将 cstr 转换为 uint32_t 值，存入 p_U32
// 返回值:
//  HResult_OK              | 执行正常
//  HResult_PARAM_OUTRANGE  | 转换值超出 uint32_t 的范围
//  HResult_PARAM_NULL      | 参数为空
//  HResult_FAILED          | 转换出错
//  HResult_CUSTOM_7F_FAIL + errno | 自定义错误代码（含errno）
HResult cstr_strtol_UInt32(const char* cstr, uint32_t* p_U32)
{
    HResult retcode = HResult_OK;
    unsigned long long conval = 0;

    retcode = cstr_strtol_ULongInt(cstr, &conval);
    if (retcode != HResult_OK)
    {
        goto EXIT;
    }

    if (conval < 0 || conval > UInt32_MAX)
    {
        retcode = HResult_PARAM_OUTRANGE;
        goto EXIT;
    }

    *p_U32 = (uint32_t)conval;

EXIT:
    return retcode;
}


// 函数功能描述: 将 cstr 转换为 int64_t 值，存入 p_I64
// 返回值:
//  HResult_OK              | 执行正常
//  HResult_PARAM_OUTRANGE  | 转换值超出 int64_t 的范围
//  HResult_PARAM_NULL      | 参数为空
//  HResult_FAILED          | 转换出错
//  HResult_CUSTOM_7F_FAIL + errno | 自定义错误代码（含errno）
HResult cstr_strtol_Int64(const char* cstr, int64_t* p_I64)
{
    HResult retcode = HResult_OK;
    long long conval = 0;

    retcode = cstr_strtol_LongInt(cstr, &conval);
    if (retcode != HResult_OK)
    {
        goto EXIT;
    }

    if (conval < INT64_MIN || conval > INT64_MAX)
    {
        retcode = HResult_PARAM_OUTRANGE;
        goto EXIT;
    }

    *p_I64 = (int64_t)conval;

EXIT:
    return retcode;
}


// 函数功能描述: 将 cstr 转换为 unsigned long long 值，存入 p_vull
// 返回值:
//  HResult_OK              | 执行正常
//  HResult_PARAM_NULL      | 参数为空
//  HResult_FAILED          | 转换出错
//  HResult_CUSTOM_7F_FAIL + errno | 自定义错误代码（含errno）
HResult cstr_strtol_ULongInt(const char* cstr, unsigned long long* p_vull)
{
    HResult retcode = HResult_OK;
    size_t cstrlen;
    char* endptr = 0;

    if (cstr == NULL || p_vull == NULL || (cstrlen = strlen(cstr)) == 0)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    int radix = 10;
    if (cstrlen > 2)
    {
        // Reference [1]: https://www.ibm.com/docs/en/i/7.2?topic=functions-strnicmp-compare-substrings-without-case-sensitivity
        if (0 == _strnicmp(cstr, "0x", 2))
        {
            radix = 16;
        }
    }

    errno = 0;

    // Reference [1]: https://cloud.tencent.com/developer/ask/sof/122066
    //           [2]: https://cplusplus.com/reference/cstdlib/strtol/
    unsigned long long conval = strtoull(cstr, &endptr, radix);
    if (errno != 0)
    {
        // [Debug]
        // printf("Conversion error, %s\n", strerror(errno));
        // ---------------------------------------------------------------------------------------------------------------------------
        // ERRORS:
        // 1. EINVAL (not in C99) The given base contains an unsupported value.
        // 2. ERANGE The resulting value was out of range.
        // 3. The implementation may also set errno to EINVAL in case no conversion was performed (no digits seen, and 0 returned).
        // ---------------------------------------------------------------------------------------------------------------------------
        retcode = HResult_CUSTOM_7F_FAIL + (HResult)errno;
        goto EXIT;
    }
    else if (*endptr)
    {
        // [Debug]
        // printf("Converted partially: %i, non-convertible part: %s\n", conval, endptr);

        retcode = HResult_FAILED;
        goto EXIT;
    }

    *p_vull = conval;

EXIT:
    return retcode;
}


// 函数功能描述: 将 cstr 转换为 long long 值，存入 p_vll
// 返回值:
//  HResult_OK              | 执行正常
//  HResult_PARAM_NULL      | 参数为空
//  HResult_FAILED          | 转换出错
//  HResult_CUSTOM_7F_FAIL + errno | 自定义错误代码（含errno）
HResult cstr_strtol_LongInt(const char* cstr, long long* p_vll)
{
    HResult retcode = HResult_OK;
    size_t cstrlen;
    char* endptr = 0;

    if (cstr == NULL || p_vll == NULL || (cstrlen = strlen(cstr)) == 0)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    int radix = 10;
    if (cstrlen > 2)
    {
        // Reference [1]: https://www.ibm.com/docs/en/i/7.2?topic=functions-strnicmp-compare-substrings-without-case-sensitivity
        if (0 == _strnicmp(cstr, "0x", 2))
        {
            radix = 16;
        }
    }

    errno = 0;

    // Reference [1]: https://cloud.tencent.com/developer/ask/sof/122066
    //           [2]: https://cplusplus.com/reference/cstdlib/strtol/
    long long conval = strtoll(cstr, &endptr, radix);
    if (errno != 0)
    {
        // [Debug]
        // printf("Conversion error, %s\n", strerror(errno));
        // ---------------------------------------------------------------------------------------------------------------------------
        // ERRORS:
        // 1. EINVAL (not in C99) The given base contains an unsupported value.
        // 2. ERANGE The resulting value was out of range.
        // 3. The implementation may also set errno to EINVAL in case no conversion was performed (no digits seen, and 0 returned).
        // ---------------------------------------------------------------------------------------------------------------------------
        retcode = HResult_CUSTOM_7F_FAIL + (HResult)errno;
        goto EXIT;
    }
    else if (*endptr)
    {
        // [Debug]
        // printf("Converted partially: %i, non-convertible part: %s\n", conval, endptr);

        retcode = HResult_FAILED;
        goto EXIT;
    }

    *p_vll = conval;

EXIT:
    return retcode;
}


// 函数功能描述: 将 cstr 转换为 long double 值，存入 p_v
// 返回值:
//  HResult_OK              | 执行正常
//  HResult_PARAM_NULL      | 参数为空
//  HResult_FAILED          | 转换出错
//  HResult_CUSTOM_7F_FAIL + errno | 自定义错误代码（含errno）
HResult cstr_strtol_LongDouble(const char* cstr, long double* p_v)
{
    HResult retcode = HResult_OK;
    size_t cstrlen;
    char* endptr = 0;

    if (cstr == NULL || p_v == NULL || (cstrlen = strlen(cstr)) == 0)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    errno = 0;

    // Reference [1]: https://cloud.tencent.com/developer/ask/sof/122066
    //           [2]: https://cplusplus.com/reference/cstdlib/strtol/
    long double conval = strtold(cstr, &endptr);
    if (errno != 0)
    {
        // [Debug]
        // printf("Conversion error, %s\n", strerror(errno));
        // ---------------------------------------------------------------------------------------------------------------------------
        // ERRORS:
        // 1. EINVAL (not in C99) The given base contains an unsupported value.
        // 2. ERANGE The resulting value was out of range.
        // 3. The implementation may also set errno to EINVAL in case no conversion was performed (no digits seen, and 0 returned).
        // ---------------------------------------------------------------------------------------------------------------------------
        retcode = HResult_CUSTOM_7F_FAIL + (HResult)errno;
        goto EXIT;
    }
    else if (*endptr)
    {
        // [Debug]
        // printf("Converted partially: %i, non-convertible part: %s\n", conval, endptr);

        retcode = HResult_FAILED;
        goto EXIT;
    }

    *p_v = conval;

EXIT:
    return retcode;
}


// 函数功能描述: 将 cstr 转换为 double 值，存入 p_v
// 返回值:
//  HResult_OK              | 执行正常
//  HResult_PARAM_NULL      | 参数为空
//  HResult_FAILED          | 转换出错
//  HResult_CUSTOM_7F_FAIL + errno | 自定义错误代码（含errno）
HResult cstr_strtol_Double(const char* cstr, double* p_v)
{
    HResult retcode = HResult_OK;
    size_t cstrlen;
    char* endptr = 0;

    if (cstr == NULL || p_v == NULL || (cstrlen = strlen(cstr)) == 0)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    errno = 0;

    // Reference [1]: https://cloud.tencent.com/developer/ask/sof/122066
    //           [2]: https://cplusplus.com/reference/cstdlib/strtol/
    double conval = strtod(cstr, &endptr);
    if (errno != 0)
    {
        // [Debug]
        // printf("Conversion error, %s\n", strerror(errno));
        // ---------------------------------------------------------------------------------------------------------------------------
        // ERRORS:
        // 1. EINVAL (not in C99) The given base contains an unsupported value.
        // 2. ERANGE The resulting value was out of range.
        // 3. The implementation may also set errno to EINVAL in case no conversion was performed (no digits seen, and 0 returned).
        // ---------------------------------------------------------------------------------------------------------------------------
        retcode = HResult_CUSTOM_7F_FAIL + (HResult)errno;
        goto EXIT;
    }
    else if (*endptr)
    {
        // [Debug]
        // printf("Converted partially: %i, non-convertible part: %s\n", conval, endptr);

        retcode = HResult_FAILED;
        goto EXIT;
    }

    *p_v = conval;

EXIT:
    return retcode;
}


// 函数功能描述: 对于 cstr 在长度为 length 的范围内，将子串 findstr 替换为 replace，只替换 1 次。
// 输入参数:
//  cstr    类型: const char*  | 原字符串
//  length  类型: const size_t | 在原字符串中搜索的长度。如果该参数为原字符串的长度，则搜索范围为整个原字符串。
//  findstr 类型: const char*  | 要搜索的`子串`
//  replace 类型: const char*  | 要将`子串`替换成的字符串。如果这一参数为 NULL 或 ""，则执行效果是将要搜索的`子串`移除。
// 输出参数:
//  outbuf  类型: char*        | 输出的字符串。如果这一参数为 NULL 则执行的效果是只获取`输出的长度`而不生成`输出的字符串`。
//  outlen  类型: size_t*      | 输出的长度
// 返回值:
//  HResult_OK              | 执行正常
//  HResult_PARAM_NULL      | 参数为空
//  HResult_DATA_INVALID    | 数据异常
HResult cstr_replace_withlength(char* outbuf, size_t* outlen, const char* cstr, const size_t length, const char* findstr, const char* replace)
{
    HResult retcode = HResult_OK;
    ssize_t findlength;
    ssize_t replacelen;
    ssize_t offset;
    const char* p;
    char* q = outbuf;

    if (cstr == NULL || length == 0 || findstr == NULL || (findlength = (ssize_t)strlen(findstr)) == 0 || outlen == NULL)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    p = strstr(cstr, findstr);
    if (p == NULL)
    {
        *outlen = length;
        if (outbuf)
        {
            goto EXIT_CopySameSrc;
        }
        goto EXIT;
    }

    offset = p - cstr;
    if (offset < 0)
    {
        retcode = HResult_DATA_INVALID;
        goto EXIT;
    }

    if ((ssize_t)length < offset + findlength)
    {
        *outlen = length;
        goto EXIT_CopySameSrc;
    }

    if (offset >= (ssize_t)length)
    {
        *outlen = length;
        if (outbuf)
        {
            goto EXIT_CopySameSrc;
        }
        goto EXIT;
    }

    replacelen = (replace == NULL) ? 0 : (ssize_t)strlen(replace);
    *outlen = (size_t)((ssize_t)length - findlength + replacelen);

    if (outbuf == NULL)
    {
        goto EXIT;
    }

    if (replacelen == findlength)
    {
        if (0 == strcmp(findstr, replace))
        {
            goto EXIT_CopySameSrc;
        }
    }

    memset(outbuf, 0, ((*outlen) + 1) * sizeof(char));

    if (offset > 0)
    {
        memcpy(q, cstr, (size_t)offset * sizeof(char));
        q += offset;
    }

    if (replacelen > 0)
    {
        memcpy(q, replace, (size_t)replacelen * sizeof(char));
        q += replacelen;
    }

    offset = (ssize_t)length - offset - findlength;
    if (offset > 0)
    {
        p += findlength;
        memcpy(q, p, (size_t)offset * sizeof(char));
    }

EXIT:
    return retcode;

EXIT_CopySameSrc:
    memset(outbuf, 0, (length + 1) * sizeof(char));
    memcpy(outbuf, cstr, length * sizeof(char));
    return retcode;
}


// 函数功能描述: 对于原字符串 cstr，将子串 findstr 替换为 replace，只替换 1 次。
// 输入参数:
//  cstr    类型: const char*  | 原字符串（搜索范围为整个原字符串）
//  findstr 类型: const char*  | 要搜索的`子串`
//  replace 类型: const char*  | 要将`子串`替换成的字符串。如果这一参数为 NULL 或 ""，则执行效果是将要搜索的`子串`移除。
// 输出参数:
//  outbuf  类型: char*        | 输出的字符串。如果这一参数为 NULL 则执行的效果是只获取`输出的长度`而不生成`输出的字符串`。
//  outlen  类型: size_t*      | 输出的长度
// 返回值:
//  HResult_OK              | 执行正常
//  HResult_PARAM_NULL      | 参数为空
//  HResult_DATA_INVALID    | 数据异常
HResult cstr_replace(char* outbuf, size_t* outlen, const char* cstr, const char* findstr, const char* replace)
{
    HResult retcode = HResult_OK;
    size_t length;

    if (cstr == NULL || (length = strlen(cstr)) == 0)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    retcode = cstr_replace_withlength(outbuf, outlen, cstr, length, findstr, replace);

EXIT:
    return retcode;
}
