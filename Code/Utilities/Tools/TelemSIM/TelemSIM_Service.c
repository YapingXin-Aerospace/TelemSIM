#include "Common/common_functions.h"
#include "TelemSIM_Service.h"


HResult service_setup(int argc, char* argv[])
{
    HResult retcode = HResult_OK;

    // Very important step, to enable the Windows terminal to display UTF-8 Chinese characters.
    SetConsoleLocale("zh_CN.UTF-8");

EXIT:
    return retcode;
}


HResult service_start()
{
    HResult retcode = HResult_OK;

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
}
