#ifndef _INC_COMMON_log_interface_H
#define _INC_COMMON_log_interface_H

#include "../datatypes.h"
#include "../../LibDev/iLog3/LOGS.h"

#define LOGINFO     INFOLOGSG
#define LOGDEBUG    DEBUGLOGSG
#define LOGWARN     WARNLOGSG
#define LOGFATAL    FATALLOGSG
#define LOGERROR    ERRORLOGSG

#define LOGINFOHEX  INFOHEXLOGSG


HResult logging_initialize(char* log_file);
HResult LoggingInitWithConfig(char* config_filename, char* file_ext_name);
void logging_release();


#endif // !_INC_COMMON_log_interface_H

