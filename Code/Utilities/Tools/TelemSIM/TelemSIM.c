// TelemSIM.c : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <stdlib.h>

#include "Common/datatypes.h"
#include "Common/PosixInterface.h"

#if defined (_DEBUG) && defined (_WIN32)
#include <vld.h>
#endif // _DEBUG && _WIN32


int main(int argc, char* argv[])
{
    HResult retcode = HResult_OK;

    if (retcode != HResult_OK)
    {
        printf("[ERROR] service_stop() failed.\n");
        goto EXIT;
    }

EXIT:
    return EXIT_SUCCESS;
}

