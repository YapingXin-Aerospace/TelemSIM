#ifndef _INC_UTILITY_TOOLS_TelemSIM_Config_H
#define _INC_UTILITY_TOOLS_TelemSIM_Config_H

#include "Common/datatypes.h"


typedef struct
{
    char TelemetrySourceDir[PATH_MAX];
    char TelemetrySourceDBFile[PATH_MAX];
    char ResultDumpFolder[PATH_MAX];

} TelemSIMConfig;


TelemSIMConfig* get_G_TelemSIMConfig();

HResult get_HostNode_configuration(char* const filepath);


#endif // !_INC_UTILITY_TOOLS_TelemSIM_Config_H
