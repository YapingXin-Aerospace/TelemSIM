// TelemSIM.c : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <stdlib.h>

#include "TelemSIM_Service.h"


#if defined (_DEBUG) && defined (_WIN32)
#include <vld.h>
#endif // _DEBUG && _WIN32


int main(int argc, char* argv[])
{
    HResult retcode = HResult_OK;

    retcode = service_setup(argc, argv);
    if (retcode != HResult_OK)
    {
        printf("[ERROR] service_setup() failed.\n");
        goto EXIT;
    }

    retcode = service_start();
    if (retcode != HResult_OK)
    {
        printf("[ERROR] service_start() failed.\n");
        goto EXIT;
    }

    retcode = service_stop();
    if (retcode != HResult_OK)
    {
        printf("[ERROR] service_stop() failed.\n");
        goto EXIT;
    }

EXIT:
    service_cleanup();
    return EXIT_SUCCESS;
}

