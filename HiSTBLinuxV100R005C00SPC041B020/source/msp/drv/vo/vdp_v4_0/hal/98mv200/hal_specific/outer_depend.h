#ifndef __VDP_OUTER_DEPEND_H_
#define __VDP_OUTER_DEPEND_H_

#include "hi_type.h"

#ifndef  __DISP_PLATFORM_BOOT__
#define  VDP_PQ_STRATEGY
#endif

#ifdef VDP_PQ_STRATEGY
#include "drv_pq_ext.h"
#endif

#if defined(HI_MINIBOOT_SUPPORT)

#define VDP_ZME_COPY_MODE
#include <string.h>

#elif (!defined(HI_MINIBOOT_SUPPORT) && defined(__DISP_PLATFORM_BOOT__))

#define VDP_ZME_COPY_MODE
#include <linux/string.h>

#elif (defined(VDP_CBB_FPGA) && VDP_CBB_FPGA && !defined(__DISP_PLATFORM_BOOT__))

#include <linux/string.h>
#include <linux/kernel.h>

#else

#include <linux/string.h>

#endif

#define  VDP_DDR_NAME_UBOOT "VDP_DDR_CFG"

#endif

