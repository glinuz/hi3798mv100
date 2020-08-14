#ifndef __FASTBOOT_DEBUG_H__
#define __FASTBOOT_DEBUG_H__

#include <boot.h>

#define DEBUG_ENABLE         ( 0x1 << 0 )
#define INFO_ENABLE          ( 0x1 << 1 )
#define WARRING_ENABLE       ( 0x1 << 2 )
#define ERROR_ENABLE         ( 0x1 << 3 )
#define ASSERT_ENABLE        ( 0x1 << 4 )

#define PRINT_FLAG    (WARRING_ENABLE|ERROR_ENABLE|ASSERT_ENABLE)

#if ( PRINT_FLAG )
#define PRINT_INIT() console_init()
#else
#define PRINT_INIT()
#endif

#if ( PRINT_FLAG & DEBUG_ENABLE )
#define PRINT_DEBUG cprintf
#else
#define PRINT_DEBUG(exp, ...)
#endif

#if ( PRINT_FLAG & INFO_ENABLE )
#define PRINT_INFO cprintf
#else
#define PRINT_INFO(exp, ...)
#endif

#if ( PRINT_FLAG & WARRING_ENABLE )
#define PRINT_WARRING cprintf
#else
#define PRINT_WARRING(exp, ...)
#endif

#if ( PRINT_FLAG & ERROR_ENABLE )
#define PRINT_ERROR cprintf
#else
#define PRINT_ERROR(exp, ...)
#endif

#if ( PRINT_FLAG & ASSERT_ENABLE )
#define ASSERT(cond,exp) if(!(cond)) {cprintf((char *)exp);while(TRUE);}
#else
#define ASSERT(cond,exp)
#endif

#endif

