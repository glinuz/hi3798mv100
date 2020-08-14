#ifndef __DRV_CGMS_H__
#define __DRV_CGMS_H__


#include "hi_type.h"

#ifndef __DISP_PLATFORM_BOOT__
#include "drv_disp_hal.h"
#include "drv_disp_isr.h"
#endif
#include "hi_drv_disp.h"

#define CgmsCheckNullPointer(ptr) \
    {                                \
        if (!ptr)                    \
        {                            \
            DISP_ERROR("DISP ERROR! Input null pointer in %s!\n", __FUNCTION__); \
            return HI_ERR_DISP_NULL_PTR;  \
        }                             \
    }

typedef struct hiDISP_CGMS_INFO_S
{
#ifndef __DISP_PLATFORM_BOOT__
	HI_BOOL bEnable;
	HI_DRV_DISPLAY_E enAttachedDisp;
    HI_DRV_DISP_CGMSA_TYPE_E  enType;
    HI_DRV_DISP_CGMSA_MODE_E  enMode;
	HI_DRV_DISP_FMT_E enCurFormat;
#endif
}DISP_CGMS_INFO_S;


typedef struct hiDISP_CGMS_STATE_S
{
#ifndef __DISP_PLATFORM_BOOT__
	HI_BOOL bEnable;
	HI_DRV_DISPLAY_E enAttachedDisp;
    HI_DRV_DISP_CGMSA_TYPE_E  enType;
    HI_DRV_DISP_CGMSA_MODE_E  enMode;
#endif
}DISP_CGMS_STATE_S;

HI_S32 DRV_CGMS_Init(HI_DRV_DISPLAY_E enDisp,HI_DRV_DISP_FMT_E enFmt,DISP_CGMS_INFO_S *pstCgms);

HI_S32 DRV_CGMS_DeInit(DISP_CGMS_INFO_S *pstCgms);

HI_S32 DRV_CGMS_Set(DISP_CGMS_INFO_S *pstCgms,HI_DRV_DISP_FMT_E enFormat,HI_DRV_DISP_CGMSA_CFG_S stCgmsCfg) ;

HI_S32 DRV_CGMS_Get(DISP_CGMS_INFO_S *pstCgms,DISP_CGMS_STATE_S *pstCurState) ;

#endif
