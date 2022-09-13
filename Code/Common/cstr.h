#ifndef _INC_CLANG_COMMON_CSTR_H
#define _INC_CLANG_COMMON_CSTR_H

#include "datatypes.h"


// 函数功能描述：1. 返回字符串从左到右第一个非空白符的字符的指针 p_Begin ;
//             2. 如果 p_Length 不为NULL，则通过它输出左侧右侧都去除`空白符`之后的字符串的长度.
//             3. 该函数并不真的移除字符串左右两侧的`空白符`，而是给出左侧非空白符的字符的指针和假设去除`空白符`之后的字符串的长度.
// 注意：函数判断`空白符`与`非空白符`的依据是 isspace() 函数.
// Reference [0]: https://linux.die.net/man/3/isspace
//           [1]: https://www.runoob.com/cprogramming/c-function-isspace.html
const char* cstr_trim_space(const char* cstr, size_t* p_Length);


// 函数功能描述: 将 cstr 转换为 uint32_t 值，存入 p_U32
// 返回值:
//  HResult_OK              | 执行正常
//  HResult_PARAM_OUTRANGE  | 转换值超出 uint32_t 的范围
//  HResult_PARAM_NULL      | 参数为空
//  HResult_FAILED          | 转换出错
//  HResult_CUSTOM_7F_FAIL + errno | 自定义错误代码（含errno）
HResult cstr_strtol_UInt32(const char* cstr, uint32_t* p_U32);


// 函数功能描述: 将 cstr 转换为 int64_t 值，存入 p_I64
// 返回值:
//  HResult_OK              | 执行正常
//  HResult_PARAM_OUTRANGE  | 转换值超出 int64_t 的范围
//  HResult_PARAM_NULL      | 参数为空
//  HResult_FAILED          | 转换出错
//  HResult_CUSTOM_7F_FAIL + errno | 自定义错误代码（含errno）
HResult cstr_strtol_Int64(const char* cstr, int64_t* p_I64);


// 函数功能描述: 将 cstr 转换为 unsigned long long 值，存入 p_vull
// 返回值:
//  HResult_OK              | 执行正常
//  HResult_PARAM_NULL      | 参数为空
//  HResult_FAILED          | 转换出错
//  HResult_CUSTOM_7F_FAIL + errno | 自定义错误代码（含errno）
HResult cstr_strtol_ULongInt(const char* cstr, unsigned long long* p_vull);


// 函数功能描述: 将 cstr 转换为 long long 值，存入 p_vll
// 返回值:
//  HResult_OK              | 执行正常
//  HResult_PARAM_NULL      | 参数为空
//  HResult_FAILED          | 转换出错
//  HResult_CUSTOM_7F_FAIL + errno | 自定义错误代码（含errno）
HResult cstr_strtol_LongInt(const char* cstr, long long* p_vll);


// 函数功能描述: 将 cstr 转换为 long double 值，存入 p_v
// 返回值:
//  HResult_OK              | 执行正常
//  HResult_PARAM_NULL      | 参数为空
//  HResult_FAILED          | 转换出错
//  HResult_CUSTOM_7F_FAIL + errno | 自定义错误代码（含errno）
HResult cstr_strtol_LongDouble(const char* cstr, long double* p_v);


// 函数功能描述: 将 cstr 转换为 double 值，存入 p_v
// 返回值:
//  HResult_OK              | 执行正常
//  HResult_PARAM_NULL      | 参数为空
//  HResult_FAILED          | 转换出错
//  HResult_CUSTOM_7F_FAIL + errno | 自定义错误代码（含errno）
HResult cstr_strtol_Double(const char* cstr, double* p_v);


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
HResult cstr_replace_withlength(char* outbuf, size_t* outlen, const char* cstr, const size_t length, const char* findstr, const char* replace);


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
HResult cstr_replace(char* outbuf, size_t* outlen, const char* cstr, const char* findstr, const char* replace);


#endif // !_INC_CLANG_COMMON_CSTR_H
