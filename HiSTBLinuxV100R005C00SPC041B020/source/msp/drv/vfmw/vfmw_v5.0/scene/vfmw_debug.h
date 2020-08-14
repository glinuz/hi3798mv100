

#ifndef VFMW_DEBUG
#define VFMW_DEBUG

//#include "hi_common_id.h"
//#include "hi_common_log.h"
#include "hi_module.h"
#include "hi_debug.h"
//#include "drv_log_ext.h"

#define HI_FATAL_VFMW(fmt...)      HI_FATAL_PRINT  (HI_ID_VFMW, fmt)
#define HI_ERR_VFMW(fmt...)        HI_ERR_PRINT    (HI_ID_VFMW, fmt)
#define HI_WARN_VFMW(fmt...)       HI_WARN_PRINT   (HI_ID_VFMW, fmt)
#define HI_INFO_VFMW(fmt...)       HI_INFO_PRINT   (HI_ID_VFMW, fmt)

#endif
