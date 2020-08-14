#ifndef _SAMPLE_FSDEBUG_H_
#define _SAMPLE_FSDEBUG_H_

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif     /* __cplusplus */
#endif  /* __cplusplus */

#define USB_MOUNT_POINT "/tmp" 

typedef enum hiMAPI_ERRLEVEL_E
{
    MAPI_TRACE_DEBUG = 1,  /* debug-level                       */
    MAPI_TRACE_WARNING,    /* warning information               */
    MAPI_TRACE_ERROR,      /* error conditions                  */
    MAPI_TRACE_INFO
} MAPI_ERRLEVEL_E;

#define HI_MAPI_DEBUG
#define HI_MAPI_TRACE_LEVEL MAPI_TRACE_ERROR

#ifdef HI_MAPI_DEBUG
 #define hi_mapi_trace( level, fmt, args... ) \
    do { \
        if (level >= HI_MAPI_TRACE_LEVEL)\
        {\
            printf("%04d@%s(): ", __LINE__, __FUNCTION__); \
            printf( fmt, ## args ); \
        } \
    } while (0)

#else
 #define hi_mapi_trace( level, fmt, args... )
#endif

#define MAPI_ASSERT(expr) do {      \
        if (!(expr))\
        { \
            printf( "Assertion [%s] failed! %s:%s(line=%d)\n", # expr, __FILE__, __FUNCTION__, __LINE__); \
            _exit(0);     \
        } } while (0)

#ifdef __cplusplus
 #if __cplusplus
}
 #endif     /* __cplusplus */
#endif  /* __cplusplus */

#endif /* __MAPI_DEBUG_H__ */
