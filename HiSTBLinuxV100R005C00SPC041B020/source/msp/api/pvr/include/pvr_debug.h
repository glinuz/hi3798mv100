
#ifndef __PVR_DEBUG_H__
#define __PVR_DEBUG_H__

#include "hi_debug.h"

#define HI_FATAL_PVR(fmt...)        HI_FATAL_PRINT  (HI_ID_PVR, fmt)
#define HI_ERR_PVR(fmt...)          HI_ERR_PRINT    (HI_ID_PVR, fmt)
#define HI_WARN_PVR(fmt...)         HI_WARN_PRINT   (HI_ID_PVR, fmt)
#define HI_INFO_PVR(fmt...)         HI_INFO_PRINT   (HI_ID_PVR, fmt)

#endif

