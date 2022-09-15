#include "Common/common_functions.h"
#include "Common/LogInterface/log_interface.h"
#include "TelemSIM_Service.h"
#include "TelemSIM_Config.h"

#include "Common/IdSnowflake/snowflake.h"

#define HOSTNODE_CONFIGXMLFILE  "Resource/Configuration/TelemSIM.xml"


HResult service_setup(int argc, char* argv[])
{
    HResult retcode = HResult_OK;
    TelemSIMConfig* config = get_G_TelemSIMConfig();

    if (config == NULL)
    {
        retcode = HResult_CONFIG_FAIL;
        goto EXIT;
    }

    uint64_t snid = snowflake_id();

    // Very important step, to enable the Windows terminal to display UTF-8 Chinese characters.
    SetConsoleLocale("zh_CN.UTF-8");

    retcode = LoggingInitWithConfig("Resource/Configuration/TelemSIM_logsconf_kvfile", ".conf");
    if (retcode != HResult_OK)
    {
        printf("[ERROR] Logging system intialize failed.\n");
        goto EXIT;
    }

    LOGINFO("==================");
    LOGINFO("TelemSIM Start!");
    LOGINFO("==================");

    retcode = get_HostNode_configuration(HOSTNODE_CONFIGXMLFILE);
    if (retcode != HResult_OK)
    {
        LOGERROR("Load configuration failed.");
        goto EXIT;
    }

    LOGINFO("TelemetrySource.Dir = %s", config->TelemetrySourceDir);
    LOGINFO("TelemetrySourceDB.File = %s", config->TelemetrySourceDBFile);
    LOGINFO("ResultDump.Dir = %s", config->ResultDumpFolder);

EXIT:
    return retcode;
}


HResult service_start()
{
    HResult retcode = HResult_OK;
    TelemSIMConfig* config = get_G_TelemSIMConfig();

    if (config == NULL)
    {
        retcode = HResult_CONFIG_FAIL;
        goto EXIT;
    }

EXIT:
    return retcode;
}


HResult service_stop()
{
    HResult retcode = HResult_OK;
    return retcode;
}


void service_cleanup()
{
    LOGINFO("==================");
    LOGINFO("TelemSIM END!");
    LOGINFO("==================");

#if ( defined _WIN32 )
    // On Linux OS, this line of code below causes a run-time fatal error:
    // Segmentation fault (core dumped)
    logging_release();
#endif
}
