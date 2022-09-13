#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "RefString.h"
#include "../cstr.h"


// ==================================================================================================================
// Function       : RefString_InitByChars
// Requirement ID : []
// Description    : Assign text to the str instance, and maintain the internal resource.
// Parameters     : [Out] str        | The RefString* instance as output.
//                  [In]  text       | The text as input content which will be assigned to str.
// Return value   : HResult type: HResult_OK as success and others for error occurs.
// Attention      : 
// ==================================================================================================================
#ifdef _MSC_VER
    #pragma warning(disable : 4996)
#endif
HResult RefString_InitByChars(RefString* str, const char* text)
{
    HResult retcode = HResult_OK;

    if (str == NULL)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    str->strlen = (text == NULL) ? 0 : (uint16_t)strlen(text);
    if (str->strlen == 0)
    {
        str->strbuf = NULL;
    }
    else
    {
        str->strbuf = (char*)calloc(((size_t)str->strlen + 1), sizeof(char));
        if (!str->strbuf)
        {
            retcode = HResult_ALLOC_FAIL;
            goto EXIT;
        }

        strncpy(str->strbuf, text, (size_t)str->strlen);
    }

EXIT:
    return retcode;
}


// ==================================================================================================================
// Function       : RefString_InitByChars
// Requirement ID : []
// Description    : Assign text to the str instance, and maintain the internal resource.
// Parameters     : [Out] str        | The RefString* instance as output.
//                  [In]  text       | The text as input content which will be assigned to str.
//                  [In]  offset     | Copy text into str instance from the position of offset (0 based).
//                  [In]  length     | Copy amount of `charcount` chars from text into str instance.
// Return value   : HResult type: HResult_OK as success and others for error occurs.
// Attention      : 
// ==================================================================================================================
#ifdef _MSC_VER
    #pragma warning(disable : 4996)
#endif
HResult RefString_InitByChars_WithOffsetLength(RefString* str, const char* text, const size_t offset, const size_t charcount)
{
    HResult retcode = HResult_OK;
    size_t length;

    if (str == NULL)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    length = (text == NULL) ? 0 : strlen(text);

    if (length == 0 || charcount == 0)
    {
        str->strlen = 0;
        str->strbuf = NULL;
        goto EXIT;
    }

    if (offset + charcount > length)
    {
        retcode = HResult_PARAM_OUTRANGE;
        goto EXIT;
    }

    if (charcount > UINT16_MAX)
    {
        retcode = HResult_PARAM_OUTRANGE;
        goto EXIT;
    }

    str->strlen = (uint16_t)charcount;
    str->strbuf = (char*)calloc(((size_t)str->strlen + 1), sizeof(char));
    if (!str->strbuf)
    {
        retcode = HResult_ALLOC_FAIL;
        goto EXIT;
    }

    strncpy(str->strbuf, (text + offset), charcount);

EXIT:
    return retcode;
}


// ==================================================================================================================
// Function       : RefString_InitByChars
// Requirement ID : []
// Description    : Assign text to the str instance, and maintain the internal resource.
// Parameters     : [Out] str        | The RefString* instance as output.
//                  [In]  text       | The text as input content which will be assigned to str.
//                  [In]  offset     | Copy text into str instance from the position of offset (0 based).
// Return value   : HResult type: HResult_OK as success and others for error occurs.
// Attention      : 
// ==================================================================================================================
#ifdef _MSC_VER
    #pragma warning(disable : 4996)
#endif
HResult RefString_InitByChars_WithOffset(RefString* str, const char* text, const size_t offset)
{
    HResult retcode = HResult_OK;
    size_t length;

    if (str == NULL)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    length = (text == NULL) ? 0 : strlen(text);
    if (length == 0)
    {
        str->strlen = 0;
        str->strbuf = NULL;
        goto EXIT;
    }

    if (offset > length - 1)
    {
        retcode = HResult_PARAM_OUTRANGE;
        goto EXIT;
    }

    retcode = RefString_InitByChars_WithOffsetLength(str, text, offset, length - offset);

EXIT:
    return retcode;
}


// ==================================================================================================================
// Function       : RefString_Replace
// Requirement ID : []
// Description    : Replace the sub c-str `findstr` with `replace` in str->strbuf, and maintain the internal resource.
// Parameters     : [Out] str       | The RefString* instance as output.
//                  [In]  findstr   | the sub c-str `findstr`
//                  [In]  replace   | The replaced sub c-str instead of `findstr`
// Return value   : HResult type: HResult_OK as success and others for error occurs.
// Attention      : 
// ==================================================================================================================
HResult RefString_Replace(RefString* str, const char* findstr, const char* replace)
{
    HResult retcode = HResult_OK;
    size_t output_length = 0;
    char* output_buffer = NULL;

    if (str == NULL || findstr == NULL)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    retcode = cstr_replace(NULL, &output_length, str->strbuf, findstr, replace);
    if (retcode != HResult_OK)
    {
        goto EXIT;
    }

    if (output_length >= UINT16_MAX)
    {
        retcode = HResult_DATA_OUTRANGE;
        goto EXIT;
    }

    if (output_length == 0)
    {
        RefString_Release(str);
        goto EXIT;
    }

    output_buffer = malloc((output_length + 1) * sizeof(char));
    if (!output_buffer)
    {
        retcode = HResult_ALLOC_FAIL;
        goto EXIT;
    }
    memset(output_buffer, 0, (output_length + 1) * sizeof(char));

    retcode = cstr_replace(output_buffer, &output_length, str->strbuf, findstr, replace);
    if (retcode != HResult_OK)
    {
        goto EXIT;
    }

    RefString_Release(str);

    retcode = RefString_InitByChars(str, output_buffer);

    free(output_buffer);
    output_buffer = NULL;

EXIT:
    return retcode;
}


// ==================================================================================================================
// Function       : RefString_Release
// Requirement ID : []
// Description    : Release the RefString instance.
// Parameters     : [In] str         | The RefString* instance as output.
// Return value   : None.
// Attention      : str shall NOT be NULL.
// ==================================================================================================================
void RefString_Release(RefString* str)
{
    assert(str != NULL);

    if (str->strbuf)
    {
        free(str->strbuf);
        str->strbuf = NULL;
    }

    str->strlen = 0;
}
