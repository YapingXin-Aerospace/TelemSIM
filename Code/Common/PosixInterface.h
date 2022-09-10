#ifndef _INC_COMMON_PosixInterface_H
#define _INC_COMMON_PosixInterface_H

#include <stdbool.h>
#include <stdint.h>


#ifdef _WIN32
#include <io.h>
#   ifndef access
#       define access _access
#   endif
#   ifndef strncasecmp
#       define strncasecmp _strnicmp
#   endif
#   ifndef strcasecmp
#       define strcasecmp _stricmp
#   endif
#   ifndef strnicmp
#       define strnicmp _strnicmp
#   endif
#   ifndef strtok_r
#       define strtok_r strtok_s
#   endif
#   ifndef getcwd
#       define getcwd _getcwd
#   endif
#   ifndef stricmp
#       define stricmp _stricmp
#   endif


/* 
 * Values for the second argument to access. These may be OR'd together.
 * Reference: 
 * [1] https://linux.die.net/man/2/access
 * [2] https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/access-waccess
 * [3] https://www.gnu.org/software/libc/manual/html_node/Testing-File-Access.html 
 */
#ifndef R_OK
#define R_OK    4   /* Flag meaning test for read permission.  */
#endif

#ifndef W_OK
#define W_OK    2   /* Flag meaning test for write permission.  */
#endif

#ifndef F_OK
#define F_OK    0   /* Flag meaning test for existence of the file.  */
#endif

//#define X_OK  1   /* Flag meaning test for execute/search permission. - unsupported in windows*/

#endif

#endif // !_INC_COMMON_PosixInterface_H
