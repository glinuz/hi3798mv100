#ifndef __VDP_RWZB_H_
#define __VDP_RWZB_H_
#include "drv_disp_com.h"

#if (defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)  )
#define  VDAC_DEFAULT_VALUE   0x801e8000
#define SCH_PHASE 0xa6

#elif  (defined(CHIP_TYPE_hi3798cv200_a)|| defined(CHIP_TYPE_hi3798cv200_b)) 

#define  VDAC_DEFAULT_VALUE   0x80000380
#define SCH_PHASE 0xa6

#else
#define  VDAC_DEFAULT_VALUE   0x801f8000
#define SCH_PHASE 0xbb

#endif

HI_U32 * VDP_Get_HDateSrc13Coef( HI_U32 u32Fmt);
HI_U32 * VDP_Get_SDateSrcCoef( HI_U32 Index);

#endif


