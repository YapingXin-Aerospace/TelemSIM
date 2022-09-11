#ifndef _INC_UTILITY_TOOLS_TelemSIM_Service_H
#define _INC_UTILITY_TOOLS_TelemSIM_Service_H

#include "Common/datatypes.h"
//#include "Common/PosixInterface.h"

HResult service_setup(int argc, char* argv[]);
HResult service_start();
HResult service_stop();
void service_cleanup();

#endif // !_INC_UTILITY_TOOLS_TelemSIM_Service_H
