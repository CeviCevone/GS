#ifndef _ERRORHANDLER_H
#define _ERRORHANDLER_H


#include <stdbool.h>

// Do not call this function directly. Use the wrapper macro ERROR_HANDLER.
extern void waitOnError(bool cnd, char *file, int line, char *msg);

/**
* @brief Wrapper of the simple error handler waitOnError that
*        writes an error message von LCD display and loops forever.
* @param cnd  If this condition is true, an error will be reported.
* @param msg  Error message
* @retval None
*/

#define ERROR_HANDLER(cnd,msg) waitOnError((cnd),__FILE__,__LINE__,(msg))

#endif
// EOF

