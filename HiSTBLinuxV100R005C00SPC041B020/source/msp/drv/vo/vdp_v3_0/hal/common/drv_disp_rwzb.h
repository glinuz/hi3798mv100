#ifndef __VDP_RWZB_H_
#define __VDP_RWZB_H_

#include "drv_disp_com.h"

/*for full drive capability*/
#ifdef HI_VO_VDAC_FULL_DRIVE
	 #if (defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)  )
	    #define  VDAC_DEFAULT_VALUE   0x801e8000
    	#define SCH_PHASE 0xa6
	 #endif
	 
	 #if (defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420))
     #define  VDAC_DEFAULT_VALUE   0x801f0000
     #define SCH_PHASE 0xa3
	 #endif	 
#endif

/*for half drive capability*/
#ifdef HI_VO_VDAC_HALF_DRIVE
		
	#if (defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420))
    #define  VDAC_DEFAULT_VALUE   0x86050000
    #define SCH_PHASE 0xa3
	#endif
	
#endif

	
/*for quanter drive capability*/
#ifdef HI_VO_VDAC_QUARTER_DRIVE
	
	#if (defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420))
    #define  VDAC_DEFAULT_VALUE   0x86048000
    #define SCH_PHASE 0x90
	#endif
	
#endif	

HI_U32 * VDP_Get_HDateSrc13Coef( HI_U32 u32Fmt);
HI_U32 * VDP_Get_SDateSrcCoef( HI_U32 Index);

#endif


