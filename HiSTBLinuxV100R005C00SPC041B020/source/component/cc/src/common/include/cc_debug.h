#ifndef __CC_DEBUG_H__
#define __CC_DEBUG_H__

#include "hi_debug.h"
#include "hi_module.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif

#define HI_FATAL_CC(fmt...)      HI_FATAL_PRINT(HI_ID_CC, fmt)
#define HI_ERR_CC(fmt...)        HI_ERR_PRINT(HI_ID_CC, fmt)
#define HI_WARN_CC(fmt...)       HI_WARN_PRINT(HI_ID_CC, fmt)
#define HI_INFO_CC(fmt...)       HI_INFO_PRINT(HI_ID_CC, fmt)

#define CC_DEC_DEBUG  (0)
#define CC_OSD_DEBUG  (0)
#define CC_XDS_DEBUG  (0)
#define CC_DISP_DEBUG (0)

#if CC_DEC_DEBUG
    #define DEC_TRACE(fmt,args...) printf("DEC : " fmt,##args)
#else
    #define DEC_TRACE(fmt,args...)
#endif

#if CC_OSD_DEBUG
    #define OSD_TRACE(fmt,args...) printf("OSD : " fmt,##args)
#else
    #define OSD_TRACE(fmt,args...)
#endif

#if CC_XDS_DEBUG
    #define XDS_TRACE(fmt,args...) printf("XDS : " fmt,##args)
#else
    #define XDS_TRACE(fmt,args...)
#endif

#if CC_DISP_DEBUG
    #define DISP_TRACE(fmt,args...) printf("DISP : " fmt,##args)
#else
    #define DISP_TRACE(fmt,args...)
#endif



extern HI_U8 DBG_CLR_OUT[9][10];

extern HI_U8 DBG_OPA_OUT[5][12];

#ifdef __cplusplus
 #if __cplusplus
 }
 #endif
#endif

#endif
