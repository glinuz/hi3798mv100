#ifndef __VDP_RWZB_H_
#define __VDP_RWZB_H_

#include "drv_disp_com.h"

#ifdef HI_VO_VDAC_FULL_DRIVE

#define  VDAC_DEFAULT_VALUE     0x800003b0

#define     SCH_PHASE 0xba
#define     C_GAIN 0

#else
/*HI_VO_VDAC_QUARTER_DRIVE*/

#define     VDAC_DEFAULT_VALUE   0x800000a0
#define     SCH_PHASE 0xba
#define     C_GAIN 0
#endif

#define    VO_DAC_CTRL1_OTP_RESISTANCE   0x00004080

HI_U32 * VDP_Get_HDateSrc13Coef( HI_U32 u32Fmt);
HI_U32 * VDP_Get_SDateSrcCoef( HI_U32 Index);

#endif


