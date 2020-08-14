
#ifndef __DEMUX_DEBUG_H__
#define __DEMUX_DEBUG_H__

#include "hi_debug.h"

#define HI_FATAL_DEMUX(fmt...)      HI_FATAL_PRINT  (HI_ID_DEMUX, fmt)
#define HI_ERR_DEMUX(fmt...)        HI_ERR_PRINT    (HI_ID_DEMUX, fmt)
#define HI_WARN_DEMUX(fmt...)       HI_WARN_PRINT   (HI_ID_DEMUX, fmt)
#define HI_INFO_DEMUX(fmt...)       HI_INFO_PRINT   (HI_ID_DEMUX, fmt)
#define HI_DBG_DEMUX(fmt...)       HI_DBG_PRINT   (HI_ID_DEMUX, fmt)

#endif

