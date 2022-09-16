#include "Common/common_functions.h"
#include "Common/LogInterface/log_interface.h"
#include "TelemSIM_Service.h"
#include "TelemSIM_Config.h"
#include "Func/Func_PreLoad.h"

//#include "Common/IdSnowflake/snowflake.h"

#include "App/SQLiteTools/DRV_Config.h"

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

    get_G_DRVConfig()->dbpath = config->TelemetrySourceDBFile;

    LOGINFO("BIN: %s", config->TelemetrySourceDBFile);
    LOGINFO("*** Doing the pre-processing tasks ***");

    /*uint64_t snid0 = next_id();
    LOGINFO("SnowFlake Id0: 0x%llx", snid0);*/

    retcode = UtilityPreload_Load();
    if (retcode != HResult_OK)
    {
        LOGERROR("Load utility pre-load data failed.");
        goto EXIT;
    }

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
    UtilityPreload_Release();

    LOGINFO("==================");
    LOGINFO("TelemSIM END!");
    LOGINFO("==================");

#if ( defined _WIN32 )
    // On Linux OS, this line of code below causes a run-time fatal error:
    // Segmentation fault (core dumped)
    logging_release();
#endif
}
