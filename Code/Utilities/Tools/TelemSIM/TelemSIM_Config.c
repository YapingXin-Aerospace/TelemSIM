#include "TelemSIM_Config.h"
#include "Common/Mini-XML/mxml-helper.h"


// ------------------------------------------------------------------------
// Global pre-load data
// ------------------------------------------------------------------------

static TelemSIMConfig G_TelemSIMConfig = { 0 };

// ------------------------------------------------------------------------
// Private functions declaration
// ------------------------------------------------------------------------

static HResult get_XML_TelemSIMConfig(TelemSIMConfig* config, char* const filepath);

// ------------------------------------------------------------------------
// Public functions Implementation
// ------------------------------------------------------------------------

TelemSIMConfig* get_G_TelemSIMConfig()
{
    return &G_TelemSIMConfig;
}


HResult get_HostNode_configuration(char* const filepath)
{
    return get_XML_TelemSIMConfig(&G_TelemSIMConfig, filepath);
}

// ------------------------------------------------------------------------
// Private functions Implementation
// ------------------------------------------------------------------------

static HResult get_XML_TelemSIMConfig(TelemSIMConfig* config, char* const filepath)
{
    HResult retcode = HResult_OK;
    mxml_node_t* tree = NULL;
    mxml_node_t* node_Configuration = NULL;

    if (NULL == config || NULL == filepath || 0 == strlen(filepath))
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    retcode = load_xml_config_file(&tree, filepath);
    if (retcode != HResult_OK)
    {
        goto EXIT;
    }

    memset(config, 0, sizeof(TelemSIMConfig));

    node_Configuration = mxmlFindPath(tree, "Configuration");
    if (!node_Configuration)
    {
        retcode = HResult_DECODE_FAIL;
        goto EXIT;
    }

EXIT:

    if (tree)
    {
        mxmlDelete(tree);
        tree = NULL;
    }

    return retcode;
}