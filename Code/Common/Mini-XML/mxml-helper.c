/*
 * Source code : mxml-helper.c
 * Module      : Common/Mini-XML
 * Purpose     : Define some helper functions for Mini-XML.
 * Author      : Yaping Xin <Yaping.Xin@foxmail.com>
 * Create Date : 2019-04-11
 * Description : NULL
 * History     : NULL
 *
 * Copyright (C) 2019 5-Dim StarLux Co., Ltd
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */


#include <assert.h>
#include <string.h>
#include "mxml-helper.h"
#include "../PosixInterface.h"


static bool is_file_exist(const char* filepath)
{
    return access(filepath, 0) == 0;
}


// ==================================================================================================================
// Function       : get_xml_SubNodesCount_byName
// Description    : Get the count of sub nodes with specific node name under the parent node.
// Parameters     : [In] node_parent | the parent node.
//                  [In] subnode_name| the specific node name for the sub nodes to be counted.
// Return value   : count result as uint16_t.
// Attention      : node_parent and subnode_name shall NOT be NULL or empty.
// ==================================================================================================================
uint16_t get_xml_SubNodesCount_byName(mxml_node_t* node_parent, const char* subnode_name)
{
    mxml_node_t* node_child = NULL;
    uint16_t count = 0;

    assert(node_parent != NULL);
    assert(subnode_name != NULL);

    node_child = mxmlFindPath(node_parent, subnode_name);
    if (!node_child)
    {
        goto EXIT;
    }

    while (node_child != NULL)
    {
        if (!check_xmlnode_name(node_child, subnode_name))
        {
            goto EXIT_ITER;
        }

        count++;

    EXIT_ITER:
        node_child = mxmlGetNextSibling(node_child);
    }

EXIT:
    return count;
}


// ==================================================================================================================
// Function       : check_xmlnode_name
// Description    : Check whether the given xml node has the specific name.
// Parameters     : [In] node        | the given xml node to be checked.
//                  [In] node_name   | the specific name.
// Return value   : bool - true for 'yes' result, false for otherwise.
// Attention      : node and node_name shall NOT be NULL or empty.
// ==================================================================================================================
bool check_xmlnode_name(mxml_node_t* node, const char* node_name)
{
    const char* element_name = NULL;
    size_t len_src = 0;
    size_t len_dst = 0;

    assert(node != NULL);
    assert(node_name != NULL);

    len_src = strlen(node_name);
    assert(len_src > 0);

    element_name = mxmlGetElement(node);
    if (!element_name)
    {
        return false;
    }

    len_dst = strlen(element_name);
    if (len_src != len_dst)
    {
        return false;
    }

    return strncmp(element_name, node_name, len_dst) == 0;
}


// ==================================================================================================================
// Function       : parse_enum_byTxtArray
// Description    : Parse the enum value in xml configuration content.
// Parameters     : [In] text        | the enum value string.
//                  [In] itemsCount  | Enum items count.
//                  [In] text_array  | Enum strings array.
//                  [In] enum_values | Enum values array.
// Notice         : Not full string comparation. Follow short cut of text_array.
// Return value   : matched emun value in uint32_t.
// Attention      : 1) Parameters shall NOT be NULL. 2) enum_values shall be uint32_t array pointer.
// Best practice:
/*
    SITE_CATEGORY parse_SITE_CATEGORY(const char* text)
    {
        #define SITE_CATEGORY_ENUM_COUNT 8

        static const char* SITE_CATEGORY_ENUM_STRS[SITE_CATEGORY_ENUM_COUNT] =
        {
            "SITE_ZERO",
            "SITE_SPECIFIC",
            "SITE_PLAT",
            "SITE_TEMPLATE",
            "SITE_ENT_ZERO",
            "SITE_ZERO_ONE",
            "SITE_ZERO_TWO",
            "SITE_PLAT_TRNG_DATA"
        };

        static const uint32_t SITE_CATEGORY_ENUM_VALS[SITE_CATEGORY_ENUM_COUNT] =
        {
            SITE_ZERO,
            SITE_SPECIFIC,
            SITE_PLAT,
            SITE_TEMPLATE,
            SITE_ENT_ZERO,
            SITE_ZERO_ONE,
            SITE_ZERO_TWO,
            SITE_PLAT_TRNG_DATA
        };

        return (SITE_CATEGORY)parse_enum_byTxtArray(text, SITE_CATEGORY_ENUM_COUNT, SITE_CATEGORY_ENUM_STRS, SITE_CATEGORY_ENUM_VALS);
    }
*/
// ==================================================================================================================
uint32_t parse_enum_byTxtArray(const char* text, const uint32_t itemsCount, const char* text_array[], const uint32_t enum_values[])
{
    uint32_t matched_value = 0;
    uint32_t idx;
    size_t item_len;
    size_t text_len;

    assert(text != NULL);
    assert(itemsCount > 0);
    assert(text_array != NULL);
    assert(enum_values != NULL);

    text_len = strlen(text);
    assert(text_len > 0);

    for (idx = 0; idx < itemsCount; idx++)
    {
        item_len = strlen(text_array[idx]);
        if (text_len >= item_len && strnicmp(text, text_array[idx], item_len) == 0)
        {
            matched_value = enum_values[idx];
            goto EXIT;
        }
    }

EXIT:
    return matched_value;
}


// ==================================================================================================================
// Function       : parse_enum_byTxtArray_full
// Description    : Parse the enum value in xml configuration content.
// Parameters     : [In] text        | the enum value string.
//                  [In] itemsCount  | Enum items count.
//                  [In] text_array  | Enum strings array.
//                  [In] enum_values | Enum values array.
// Notice         : Use full string comparation.
// Return value   : matched emun value in uint32_t.
// Attention      : 1) Parameters shall NOT be NULL. 2) enum_values shall be uint32_t array pointer.
// Best practice:
/*
    SITE_CATEGORY parse_SITE_CATEGORY(const char* text)
    {
        #define SITE_CATEGORY_ENUM_COUNT 8

        static const char* SITE_CATEGORY_ENUM_STRS[SITE_CATEGORY_ENUM_COUNT] =
        {
            "SITE_ZERO",
            "SITE_SPECIFIC",
            "SITE_PLAT",
            "SITE_TEMPLATE",
            "SITE_ENT_ZERO",
            "SITE_ZERO_ONE",
            "SITE_ZERO_TWO",
            "SITE_PLAT_TRNG_DATA"
        };

        static const uint32_t SITE_CATEGORY_ENUM_VALS[SITE_CATEGORY_ENUM_COUNT] =
        {
            SITE_ZERO,
            SITE_SPECIFIC,
            SITE_PLAT,
            SITE_TEMPLATE,
            SITE_ENT_ZERO,
            SITE_ZERO_ONE,
            SITE_ZERO_TWO,
            SITE_PLAT_TRNG_DATA
        };

        return (SITE_CATEGORY)parse_enum_byTxtArray(text, SITE_CATEGORY_ENUM_COUNT, SITE_CATEGORY_ENUM_STRS, SITE_CATEGORY_ENUM_VALS);
    }
*/
// ==================================================================================================================
uint32_t parse_enum_byTxtArray_full(const char* text, const uint32_t itemsCount, const char* text_array[], const uint32_t enum_values[])
{
    uint32_t matched_value = 0;
    uint32_t idx;
    size_t item_len;
    size_t text_len;

    assert(text != NULL);
    assert(itemsCount > 0);
    assert(text_array != NULL);
    assert(enum_values != NULL);

    text_len = strlen(text);
    assert(text_len > 0);

    for (idx = 0; idx < itemsCount; idx++)
    {
        item_len = strlen(text_array[idx]);
        if (text_len == item_len && strnicmp(text, text_array[idx], item_len) == 0)
        {
            matched_value = enum_values[idx];
            goto EXIT;
        }
    }

EXIT:
    return matched_value;
}


// ==================================================================================================================
// Function       : parse_enum_byTxtArray_u8
// Description    : Parse the enum value in xml configuration content. (Base on uint8_t enum_values array)
// Parameters     : [In] text        | the enum value string.
//                  [In] itemsCount  | Enum items count.
//                  [In] text_array  | Enum strings array.
//                  [In] enum_values | Enum values array.
// Notice         : Not full string comparation. Follow short cut of text_array.
// Return value   : matched emun value in uint32_t.
// Attention      : 1) Parameters shall NOT be NULL. 2) enum_values shall be uint32_t array pointer.
// Best practice:
/*
    SITE_CATEGORY parse_SITE_CATEGORY(const char* text)
    {
        #define SITE_CATEGORY_ENUM_COUNT 8

        static const char* SITE_CATEGORY_ENUM_STRS[SITE_CATEGORY_ENUM_COUNT] =
        {
            "SITE_ZERO",
            "SITE_SPECIFIC",
            "SITE_PLAT",
            "SITE_TEMPLATE",
            "SITE_ENT_ZERO",
            "SITE_ZERO_ONE",
            "SITE_ZERO_TWO",
            "SITE_PLAT_TRNG_DATA"
        };

        static const uint8_t SITE_CATEGORY_ENUM_VALS[SITE_CATEGORY_ENUM_COUNT] =
        {
            SITE_ZERO,
            SITE_SPECIFIC,
            SITE_PLAT,
            SITE_TEMPLATE,
            SITE_ENT_ZERO,
            SITE_ZERO_ONE,
            SITE_ZERO_TWO,
            SITE_PLAT_TRNG_DATA
        };

        return (SITE_CATEGORY)parse_enum_byTxtArray(text, SITE_CATEGORY_ENUM_COUNT, SITE_CATEGORY_ENUM_STRS, SITE_CATEGORY_ENUM_VALS);
    }
*/
// ==================================================================================================================
uint32_t parse_enum_byTxtArray_u8(const char* text, const uint8_t itemsCount, const char* text_array[], const uint8_t enum_values[])
{
    uint32_t matched_value = 0;
    uint32_t idx;
    size_t item_len;
    size_t text_len;

    assert(text != NULL);
    assert(itemsCount > 0);
    assert(text_array != NULL);
    assert(enum_values != NULL);

    text_len = strlen(text);
    assert(text_len > 0);

    for (idx = 0; idx < itemsCount; idx++)
    {
        item_len = strlen(text_array[idx]);
        if (text_len >= item_len && strnicmp(text, text_array[idx], item_len) == 0)
        {
            matched_value = enum_values[idx];
            goto EXIT;
        }
    }

EXIT:
    return matched_value;
}


// ==================================================================================================================
// Function       : parse_enum_byTxtArray_full_u8
// Description    : Parse the enum value in xml configuration content. (Base on uint8_t enum_values array)
// Parameters     : [In] text        | the enum value string.
//                  [In] itemsCount  | Enum items count.
//                  [In] text_array  | Enum strings array.
//                  [In] enum_values | Enum values array.
// Notice         : Use full string comparation.
// Return value   : matched emun value in uint32_t.
// Attention      : 1) Parameters shall NOT be NULL. 2) enum_values shall be uint32_t array pointer.
// Best practice:
/*
    SITE_CATEGORY parse_SITE_CATEGORY(const char* text)
    {
        #define SITE_CATEGORY_ENUM_COUNT 8

        static const char* SITE_CATEGORY_ENUM_STRS[SITE_CATEGORY_ENUM_COUNT] =
        {
            "SITE_ZERO",
            "SITE_SPECIFIC",
            "SITE_PLAT",
            "SITE_TEMPLATE",
            "SITE_ENT_ZERO",
            "SITE_ZERO_ONE",
            "SITE_ZERO_TWO",
            "SITE_PLAT_TRNG_DATA"
        };

        static const uint8_t SITE_CATEGORY_ENUM_VALS[SITE_CATEGORY_ENUM_COUNT] =
        {
            SITE_ZERO,
            SITE_SPECIFIC,
            SITE_PLAT,
            SITE_TEMPLATE,
            SITE_ENT_ZERO,
            SITE_ZERO_ONE,
            SITE_ZERO_TWO,
            SITE_PLAT_TRNG_DATA
        };

        return (SITE_CATEGORY)parse_enum_byTxtArray(text, SITE_CATEGORY_ENUM_COUNT, SITE_CATEGORY_ENUM_STRS, SITE_CATEGORY_ENUM_VALS);
    }
*/
// ==================================================================================================================
uint32_t parse_enum_byTxtArray_full_u8(const char* text, const uint8_t itemsCount, const char* text_array[], const uint8_t enum_values[])
{
    uint32_t matched_value = 0;
    uint32_t idx;
    size_t item_len;
    size_t text_len;

    assert(text != NULL);
    assert(itemsCount > 0);
    assert(text_array != NULL);
    assert(enum_values != NULL);

    text_len = strlen(text);
    assert(text_len > 0);

    for (idx = 0; idx < itemsCount; idx++)
    {
        item_len = strlen(text_array[idx]);
        if (text_len == item_len && strnicmp(text, text_array[idx], item_len) == 0)
        {
            matched_value = enum_values[idx];
            goto EXIT;
        }
    }

EXIT:
    return matched_value;
}


// ==================================================================================================================
// Function       : parse_enum_byTxtArray_full_u16
// Description    : Parse the enum value in xml configuration content. (Base on uint16_t enum_values array)
// Parameters     : [In] text        | the enum value string.
//                  [In] itemsCount  | Enum items count.
//                  [In] text_array  | Enum strings array.
//                  [In] enum_values | Enum values array.
// Notice         : Use full string comparation.
// Return value   : matched emun value in uint32_t.
// Attention      : 1) Parameters shall NOT be NULL. 2) enum_values shall be uint32_t array pointer.
// Best practice:
/*
    SITE_CATEGORY parse_SITE_CATEGORY(const char* text)
    {
        #define SITE_CATEGORY_ENUM_COUNT 8

        static const char* SITE_CATEGORY_ENUM_STRS[SITE_CATEGORY_ENUM_COUNT] =
        {
            "SITE_ZERO",
            "SITE_SPECIFIC",
            "SITE_PLAT",
            "SITE_TEMPLATE",
            "SITE_ENT_ZERO",
            "SITE_ZERO_ONE",
            "SITE_ZERO_TWO",
            "SITE_PLAT_TRNG_DATA"
        };

        static const uint16_t SITE_CATEGORY_ENUM_VALS[SITE_CATEGORY_ENUM_COUNT] =
        {
            SITE_ZERO,
            SITE_SPECIFIC,
            SITE_PLAT,
            SITE_TEMPLATE,
            SITE_ENT_ZERO,
            SITE_ZERO_ONE,
            SITE_ZERO_TWO,
            SITE_PLAT_TRNG_DATA
        };

        return (SITE_CATEGORY)parse_enum_byTxtArray_full_u16(text, SITE_CATEGORY_ENUM_COUNT, SITE_CATEGORY_ENUM_STRS, SITE_CATEGORY_ENUM_VALS);
    }
*/
// ==================================================================================================================
uint32_t parse_enum_byTxtArray_full_u16(const char* text, const uint8_t itemsCount, const char* text_array[], const uint16_t enum_values[])
{
    uint32_t matched_value = 0;
    uint32_t idx;
    size_t item_len;
    size_t text_len;

    assert(text != NULL);
    assert(itemsCount > 0);
    assert(text_array != NULL);
    assert(enum_values != NULL);

    text_len = strlen(text);
    assert(text_len > 0);

    for (idx = 0; idx < itemsCount; idx++)
    {
        item_len = strlen(text_array[idx]);
        if (text_len == item_len && strnicmp(text, text_array[idx], item_len) == 0)
        {
            matched_value = enum_values[idx];
            goto EXIT;
        }
    }

EXIT:
    return matched_value;
}


// ==================================================================================================================
// Function       : foreach_subnode_exec
// Description    : Lookup sub nodes under parent node with specific sub node name, and execute callback functions 
//                  on each matched sub node.
// Parameters     : [In] parent_node | the parent node to lookup the sub nodes.
//                  [In] subnode_name| the specific sub node's name. Only matched sub node will be used to execute callback.
//                  [In] callback    | the callback function for each matched sub node to execute with.
//                  [In] pexec       | the main parameter for the callback function to execute with.
//                  [In] pdata       | the aux parameter for the callback function to execute with.
// Return value   : HResult - HResult_OK for success, otherwise means error occurs.
// Attention      : 1) all parameters except pdata shall be NOT NULL.
//                  2) If error occurs, then the whole foreach-lookup procedure will be terminated with return code.
// ==================================================================================================================
HResult foreach_subnode_exec(mxml_node_t* parent_node, const char* subnode_name, callback_subnode_lookup callback, void* pexec, void* pdata)
{
    HResult retcode = HResult_OK;
    mxml_node_t* subnode = NULL;
    uint32_t rowIndex = 0;

    if (parent_node == NULL || subnode_name == NULL || callback == NULL || pexec == NULL)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    subnode = mxmlGetFirstChild(parent_node);
    while (subnode != NULL)
    {
        if (!check_xmlnode_name(subnode, subnode_name))
        {
            goto EXIT_ITER;
        }

        retcode = callback(subnode, rowIndex, pexec, pdata);
        rowIndex++;
        if (retcode != HResult_OK)
        {
            goto EXIT;
        }

    EXIT_ITER:
        subnode = mxmlGetNextSibling(subnode);
    }

EXIT:
    return retcode;
}


// ==================================================================================================================
// Function       : parse_xmlstring_HexOrDec_numeric_long
// Description    : Convert string (NULL- terminated char*) to long value.
// Parameters     : [In] str         | string (NULL- terminated char*)
// Return value   : long
// Attention      : 1) str shall NOT be NULL and the string length shall NOT be less than 1.
//                  2) the value expressed in str shall be numeric long value.
// ==================================================================================================================
long parse_xmlstring_HexOrDec_numeric_long(const char* str)
{
    size_t str_len = 0;

    assert(str != NULL);

    str_len = strlen(str);

    if (str_len == 0) return 0;

    if (str_len < 3)
    {
        return atol(str);
    }

    if (strnicmp(str, "0x", 2) == 0)
    {
        return strtol(str, NULL, 16);
    }

    return atol(str);
}


// ==================================================================================================================
// XML configuration file helper functions
// ==================================================================================================================

// ==================================================================================================================
// Function       : load_xml_config_file
// Description    : Load XML configuration file as (mxml_node_t*) instance.
// Parameters     : [Out] p_TreePtr  | Pointer to the root tree of the (mxml_node_t*) instance as output.
//                  [In]  filepath   | the configuration file path.
// Return value   : HResult - HResult_OK for success, otherwise means error occurs.
// Attention      : 1) all parameters except pdata shall be NOT NULL.
//                  2) If error occurs, then the whole foreach-lookup procedure will be terminated with return code.
// ==================================================================================================================
HResult load_xml_config_file(mxml_node_t** p_TreePtr, char* const filepath)
{
    HResult retcode = HResult_OK;
    FILE* file_stream = NULL;

    if (!is_file_exist(filepath))
    {
        retcode = HResult_FILE_NOTEXISTS;
        goto EXIT;
    }

    file_stream = fopen(filepath, "r");
    if (!file_stream)
    {
        retcode = HResult_FILE_OPEN_FAIL;
        goto EXIT;
    }

    *p_TreePtr = mxmlLoadFile(NULL, file_stream, MXML_IGNORE_CALLBACK);
    fclose(file_stream);

    if (!(*p_TreePtr))
    {
        retcode = HResult_DECODE_FAIL;
        goto EXIT;
    }

EXIT:
    return retcode;
}
