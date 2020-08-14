#ifndef __TELETEXT_DEBUG_H__
#define __TELETEXT_DEBUG_H__

#include "hi_debug.h"
#ifdef HI_FATAL_PRINT
#define HI_FATAL_TTX(fmt...)      HI_FATAL_PRINT(HI_ID_TTX, fmt)
#else
#define HI_FATAL_TTX(fmt...)      HI_TRACE(HI_LOG_LEVEL_FATAL, HI_DEBUG_ID_TTX, fmt)
#endif

#ifdef HI_ERR_PRINT
#define HI_ERR_TTX(fmt...)        HI_ERR_PRINT(HI_ID_TTX, fmt)
#else
#define HI_ERR_TTX(fmt...)        HI_TRACE(HI_LOG_LEVEL_ERROR, HI_DEBUG_ID_TTX, fmt)
#endif

#ifdef HI_WARN_PRINT
#define HI_WARN_TTX(fmt...)       HI_WARN_PRINT(HI_ID_TTX, fmt)
#else
#define HI_WARN_TTX(fmt...)       HI_TRACE(HI_LOG_LEVEL_WARNING, HI_DEBUG_ID_TTX, fmt)
#endif

#ifdef HI_INFO_PRINT
#define HI_INFO_TTX(fmt...)       HI_INFO_PRINT(HI_ID_TTX, fmt)
#else
#define HI_INFO_TTX(fmt...)       HI_TRACE(HI_LOG_LEVEL_INFO, HI_DEBUG_ID_TTX, fmt)
#endif

#endif
