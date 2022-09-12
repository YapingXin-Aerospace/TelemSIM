/*
 * Source code : mxml-helper.h
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


#ifndef _INC_COMMON_MINI_XML_mxml_helper_H
#define _INC_COMMON_MINI_XML_mxml_helper_H

#include "../../LibDev/Mini-XML/include/mxml.h"
#include "../datatypes.h"


// to be used with foreach_subnode_exec()
typedef HResult(*callback_subnode_lookup)(mxml_node_t* node, const uint32_t rowIndex, void* pexec, void* pdata);


// ==================================================================================================================
// Function       : get_xml_SubNodesCount_byName
// Description    : Get the count of sub nodes with specific node name under the parent node.
// Parameters     : [In] node_parent | the parent node.
//                  [In] subnode_name| the specific node name for the sub nodes to be counted.
// Return value   : count result as uint16_t.
// Attention      : node_parent and subnode_name shall NOT be NULL or empty.
// ==================================================================================================================
uint16_t get_xml_SubNodesCount_byName(mxml_node_t* node_parent, const char* subnode_name);


// ==================================================================================================================
// Function       : check_xmlnode_name
// Description    : Check whether the given xml node has the specific name.
// Parameters     : [In] node        | the given xml node to be checked.
//                  [In] node_name   | the specific name.
// Return value   : bool - true for 'yes' result, false for otherwise.
// Attention      : node and node_name shall NOT be NULL or empty.
// ==================================================================================================================
bool check_xmlnode_name(mxml_node_t* node, const char* node_name);


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
// ==================================================================================================================
uint32_t parse_enum_byTxtArray(const char* text, const uint32_t itemsCount, const char* text_array[], const uint32_t enum_values[]);


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
// ==================================================================================================================
uint32_t parse_enum_byTxtArray_full(const char* text, const uint32_t itemsCount, const char* text_array[], const uint32_t enum_values[]);


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
// ==================================================================================================================
uint32_t parse_enum_byTxtArray_u8(const char* text, const uint8_t itemsCount, const char* text_array[], const uint8_t enum_values[]);


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
// ==================================================================================================================
uint32_t parse_enum_byTxtArray_full_u8(const char* text, const uint8_t itemsCount, const char* text_array[], const uint8_t enum_values[]);


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
// ==================================================================================================================
uint32_t parse_enum_byTxtArray_full_u16(const char* text, const uint8_t itemsCount, const char* text_array[], const uint16_t enum_values[]);


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
HResult foreach_subnode_exec(mxml_node_t* parent_node, const char* subnode_name, callback_subnode_lookup callback, void* pexec, void* pdata);


// ==================================================================================================================
// Function       : parse_xmlstring_HexOrDec_numeric_long
// Description    : Convert string (NULL- terminated char*) to long value.
// Parameters     : [In] str         | string (NULL- terminated char*)
// Return value   : long
// Attention      : 1) str shall NOT be NULL and the string length shall NOT be less than 1.
//                  2) the value expressed in str shall be numeric long value.
// ==================================================================================================================
long parse_xmlstring_HexOrDec_numeric_long(const char* str);


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
HResult load_xml_config_file(mxml_node_t** p_TreePtr, char* const filepath);

#endif // !_INC_COMMON_MINI_XML_mxml_helper_H
