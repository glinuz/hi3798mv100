#ifndef __DRV_MCVN_H__
#define __DRV_MCVN_H__

#include "hi_drv_disp.h"
#include "drv_disp_hal.h"

typedef struct  hiDRV_MCVN_S
{
	HI_DRV_DISP_MACROVISION_E enMcvn;
}DRV_MCVN_S;

HI_S32 DRV_MCVN_Init(HI_DRV_DISPLAY_E enDisp,HI_DRV_DISP_FMT_E enFmt);

HI_S32 DRV_MCVN_DeInit(HI_DRV_DISPLAY_E enDisp);

HI_S32 DRV_MCVN_SetMacrovision(HI_DRV_DISPLAY_E enDisp, 
                                HI_DRV_DISP_MACROVISION_E enMode,
                                HI_DRV_DISP_FMT_E enFmt);
HI_S32 DRV_MCVN_SetDateMcvn(HI_DRV_DISP_FMT_E enFmt, HI_DRV_DISP_MACROVISION_E enMcvn);

HI_S32 DRV_MCVN_GetMacrovision(HI_DRV_DISPLAY_E enDisp, 
                                HI_DRV_DISP_MACROVISION_E *penMode);

#endif
