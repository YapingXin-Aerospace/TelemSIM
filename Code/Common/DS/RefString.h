#ifndef _INC_COMMON_DS_RefString_H
#define _INC_COMMON_DS_RefString_H

#include "Common/datatypes.h"


typedef struct
{
    uint16_t strlen;
    char*    strbuf;

} RefString;


// ==================================================================================================================
// Function       : RefString_InitByChars
// Requirement ID : []
// Description    : Assign text to the str instance, and maintain the internal resource.
// Parameters     : [Out] str        | The RefString* instance as output.
//                  [In]  text       | The text as input content which will be assigned to str.
// Return value   : HResult type: HResult_OK as success and others for error occurs.
// Attention      : 
// ==================================================================================================================
HResult RefString_InitByChars(RefString* str, const char* text);


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
HResult RefString_InitByChars_WithOffsetLength(RefString* str, const char* text, const size_t offset, const size_t charcount);


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
HResult RefString_InitByChars_WithOffset(RefString* str, const char* text, const size_t offset);


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
HResult RefString_Replace(RefString* str, const char* findstr, const char* replace);


// ==================================================================================================================
// Function       : RefString_Release
// Requirement ID : []
// Description    : Release the RefString instance.
// Parameters     : [In] str         | The RefString* instance as output.
// Return value   : None.
// Attention      : str shall NOT be NULL.
// ==================================================================================================================
void RefString_Release(RefString* str);

#endif // !_INC_COMMON_DS_RefString_H
