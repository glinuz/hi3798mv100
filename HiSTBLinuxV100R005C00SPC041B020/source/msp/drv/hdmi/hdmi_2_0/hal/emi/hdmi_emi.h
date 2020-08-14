#ifndef __HDMI_EMI_H__
#define __HDMI_EMI_H__

#include "drv_hdmi_common.h"
#include "hi_reg_crg.h"

#define HDMI_CRG_PLL10      g_pstRegCrg->PERI_CRG_PLL10.u32
#define HDMI_CRG_PLL11      g_pstRegCrg->PERI_CRG_PLL11.u32
#define HDMI_CRG_80         g_pstRegCrg->PERI_CRG80.u32

#define HDMI_CRG_PLL10_ADDR     0x2028
#define HDMI_CRG_PLL11_ADDR     0x202c
#define HDMI_CRG_80_ADDR        0x2140


typedef struct hiHDMI_EMI_CONFIG
{       
    HI_BOOL                 bEmiEnable;
    HI_BOOL                 bDebugEnable;
    HI_BOOL                 bVesaFmt;
    HI_U32                  u32TmdsClk;
    HDMI_COLORSPACE_E       enOutColorSpace;
    HDMI_DEEP_COLOR_E       enDeepColor;
}HDMI_EMI_CONFIG_S;

typedef struct hiEmi_Status_S
{
    HI_BOOL bSWEmiEnable;
    HI_BOOL bHWEmiEnable;
    HI_BOOL bDebugEnable;   // user debug
}HDMI_EMI_STATUS_S;


HI_S32 HDMI_EmiSet(HDMI_EMI_CONFIG_S* pstEmiCfg);
HI_VOID HDMI_EmiStatusGet(HDMI_EMI_STATUS_S *pstEmiStatus);


#endif

