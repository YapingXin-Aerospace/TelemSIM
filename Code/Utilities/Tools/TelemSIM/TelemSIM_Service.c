#include "Common/common_functions.h"
#include "Common/LogInterface/log_interface.h"
#include "TelemSIM_Service.h"


HResult service_setup(int argc, char* argv[])
{
    HResult retcode = HResult_OK;

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

EXIT:
    return retcode;
}


HResult service_start()
{
    HResult retcode = HResult_OK;

//EXIT:
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
    LOGINFO("TMSrcImport END!");
    LOGINFO("==================");

#if ( defined _WIN32 )
    // On Linux OS, this line of code below causes a run-time fatal error:
    // Segmentation fault (core dumped)
    logging_release();
#endif
}
