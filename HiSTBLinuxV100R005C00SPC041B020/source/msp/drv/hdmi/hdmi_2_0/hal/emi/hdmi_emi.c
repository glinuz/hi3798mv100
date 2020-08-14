#include "hdmi_platform.h"
#include "hdmi_emi.h"
#ifndef HDMI_BUILD_IN_BOOT
#include "drv_hdmi_platform.h"
#include "hi_reg_common.h"
#else
#include "boot_hdmi_intf.h"
#endif


#define MAXREGADDR_LINE     2
#define MAXREGVALS          6

#define HDMI_FMT_PIXCLK_40500       40500
#define HDMI_FMT_PIXCLK_74250       74250
#define HDMI_FMT_PIXCLK_111375      111375
#define HDMI_FMT_PIXCLK_165000      165000
#define HDMI_FMT_PIXCLK_222750      222750
#define HDMI_FMT_PIXCLK_297000      297000

#ifdef HDMI_EMI_ENABLE
HDMI_EMI_STATUS_S stEmiEnable = {HI_TRUE, HI_FALSE, HI_FALSE};
#else
HDMI_EMI_STATUS_S stEmiEnable = {HI_FALSE, HI_FALSE, HI_FALSE};
#endif

typedef enum
{
    FMT_PIXCLK_RANGE_0 = 0,
    FMT_PIXCLK_RANGE_1,
    FMT_PIXCLK_RANGE_2,
    FMT_PIXCLK_RANGE_3,
    FMT_PIXCLK_RANGE_4,
    FMT_PIXCLK_RANGE_5,
    FMT_PIXCLK_RANGE_6,
    FMT_PIXCLK_RANGE_BUTT,
}EMI_FMT_PIXCLK_RANGE_INDEX_E;

#ifdef HDMI_BUILD_IN_BOOT
/*
**  |------------------------------------- open ---------------------------|---------- close -------|
**/
HI_U32 g_EmiRegAddr[FMT_PIXCLK_RANGE_BUTT + 1][MAXREGVALS] = {
    {		   0,          0,          0,          0, 0xf8a22140, 0xf8a22140},
    { 0xf8a2202c, 0xf8a22140, 0xf8a22140, 0xf8a22140, 0xf8a22140, 0xf8a22140}
};
#endif
/*
**  |------------------------------------- open ---------------------------|---------- close -------|
**/
HI_U32 g_EmiRegVal[FMT_PIXCLK_RANGE_BUTT + 1][MAXREGVALS] = {
	{		  0,          0,          0,          0,       0x06,        0x06},
	{(~(1<<25)),       0x06,     0x8876,     0x8871,     0x8876,        0x06},
	{(~(1<<25)),       0x06,     0x8846,     0x8841,     0x8846,        0x06},
	{(~(1<<25)),       0x06,     0x9036,     0x9031,     0x9036,        0x06},
	{(~(1<<25)),       0x06,     0x9026,     0x9021,     0x9026,        0x06},
	{(~(1<<25)),       0x06,     0x8816,     0x8811,     0x8816,        0x06},
	{		  0,          0,          0,          0,       0x06,        0x06},
	{		  0,          0,          0,          0,       0x06,        0x06}
};


EMI_FMT_PIXCLK_RANGE_INDEX_E EmiPixClkRangeIndex(HDMI_EMI_CONFIG_S* pstEmiCfg)
{
    HI_U32 u32TmdsClk = pstEmiCfg->u32TmdsClk;
    EMI_FMT_PIXCLK_RANGE_INDEX_E enFmtPixClkRange = FMT_PIXCLK_RANGE_BUTT;
    if(u32TmdsClk <= HDMI_FMT_PIXCLK_40500)
    {
        enFmtPixClkRange = FMT_PIXCLK_RANGE_0;
    }
    else if(u32TmdsClk > HDMI_FMT_PIXCLK_40500 && u32TmdsClk <= HDMI_FMT_PIXCLK_74250)
    {
        enFmtPixClkRange = FMT_PIXCLK_RANGE_1;
    }
    else if(u32TmdsClk > HDMI_FMT_PIXCLK_74250 && u32TmdsClk <= HDMI_FMT_PIXCLK_111375)
    {
        enFmtPixClkRange = FMT_PIXCLK_RANGE_2;
    }
    else if(u32TmdsClk > HDMI_FMT_PIXCLK_111375 && u32TmdsClk <= HDMI_FMT_PIXCLK_165000)
    {
        enFmtPixClkRange = FMT_PIXCLK_RANGE_3;
    }
    else if(u32TmdsClk > HDMI_FMT_PIXCLK_165000 && u32TmdsClk <= HDMI_FMT_PIXCLK_222750)
    {
        enFmtPixClkRange = FMT_PIXCLK_RANGE_4;
    }
    else if(u32TmdsClk > HDMI_FMT_PIXCLK_222750 && u32TmdsClk <= HDMI_FMT_PIXCLK_297000)
    {
        enFmtPixClkRange = FMT_PIXCLK_RANGE_5;
    }
    else if(u32TmdsClk > HDMI_FMT_PIXCLK_297000)
    {
        enFmtPixClkRange = FMT_PIXCLK_RANGE_6;
    }

    return enFmtPixClkRange;
}

HI_VOID EmiEnable(EMI_FMT_PIXCLK_RANGE_INDEX_E enIdex)
{
    HI_U32 u32RegVal;
#ifndef HDMI_BUILD_IN_BOOT
    u32RegVal = HDMI_CRG_PLL11;
    u32RegVal &= g_EmiRegVal[enIdex][0];
    HDMI_CRG_PLL11 = u32RegVal;
    HDMI_CRG_80    = g_EmiRegVal[enIdex][1];
    DRV_HDMI_MilliDelay(1);
    HDMI_CRG_80    = g_EmiRegVal[enIdex][2];
    DRV_HDMI_MilliDelay(1);
    HDMI_CRG_80    = g_EmiRegVal[enIdex][3];
#else

    u32RegVal = HdmiRegRead((volatile HI_VOID *)g_EmiRegAddr[1][0]);
    u32RegVal &= g_EmiRegVal[enIdex][0];

    HdmiRegWrite((volatile HI_VOID *)g_EmiRegAddr[1][0], u32RegVal);
    HdmiRegWrite((volatile HI_VOID *)g_EmiRegAddr[1][1], g_EmiRegVal[enIdex][1]);
    DRV_HDMI_MilliDelay(1);
    HdmiRegWrite((volatile HI_VOID *)g_EmiRegAddr[1][2], g_EmiRegVal[enIdex][2]);
    DRV_HDMI_MilliDelay(1);
    HdmiRegWrite((volatile HI_VOID *)g_EmiRegAddr[1][3], g_EmiRegVal[enIdex][3]);

#endif
}

HI_VOID EmiDisable(EMI_FMT_PIXCLK_RANGE_INDEX_E enIdex)
{
#ifndef HDMI_BUILD_IN_BOOT
    HDMI_CRG_80 = g_EmiRegVal[enIdex][MAXREGVALS - 2];
    DRV_HDMI_MilliDelay(1);
    HDMI_CRG_80 = g_EmiRegVal[enIdex][MAXREGVALS - 1];
#else
    HdmiRegWrite((volatile HI_VOID *)g_EmiRegAddr[0][MAXREGVALS - 2], g_EmiRegVal[enIdex][MAXREGVALS - 2]);
    DRV_HDMI_MilliDelay(1);
    HdmiRegWrite((volatile HI_VOID *)g_EmiRegAddr[0][MAXREGVALS - 1], g_EmiRegVal[enIdex][MAXREGVALS - 1]);
#endif
}

HI_VOID EmiParamSet(HDMI_EMI_CONFIG_S* pstEmiCfg, EMI_FMT_PIXCLK_RANGE_INDEX_E enIdex)
{
    if(enIdex <= FMT_PIXCLK_RANGE_0 || enIdex >= FMT_PIXCLK_RANGE_6 || pstEmiCfg->bVesaFmt == HI_TRUE)
    {
        stEmiEnable.bHWEmiEnable = HI_FALSE;
    }
    else
    {
        stEmiEnable.bHWEmiEnable = pstEmiCfg->bEmiEnable;
    }

    if(stEmiEnable.bHWEmiEnable == HI_FALSE)
    {
        EmiDisable(enIdex);
    }
    else
    {
        EmiEnable(enIdex);
    }
    HDMI_INFO("hardware emi enable %d, index is %x. \n", stEmiEnable.bHWEmiEnable, enIdex);
}

HI_S32 HDMI_EmiSet(HDMI_EMI_CONFIG_S* pstEmiCfg)
{
    EMI_FMT_PIXCLK_RANGE_INDEX_E enPixClkRangeIndex = FMT_PIXCLK_RANGE_0;

    if(pstEmiCfg == HI_NULL)
    {
        HDMI_ERR("pstEmiCfg is null! \n");
        return HI_FAILURE;
    }

    /* when user debug cmd(disable) is change
       or debug is enable -- don't change any emi regs
       or pixclk is 0, return. */
    if(stEmiEnable.bDebugEnable != pstEmiCfg->bDebugEnable
       || pstEmiCfg->bDebugEnable == HI_TRUE
       || pstEmiCfg->u32TmdsClk == 0)
    {
        HDMI_INFO("Emi debug mode change, old(%d)->new(%d), or debug is enbale.\n", stEmiEnable.bDebugEnable, pstEmiCfg->bDebugEnable);
        stEmiEnable.bDebugEnable = pstEmiCfg->bDebugEnable;
        return HI_SUCCESS;
    }

    stEmiEnable.bSWEmiEnable = pstEmiCfg->bEmiEnable;

    enPixClkRangeIndex = EmiPixClkRangeIndex(pstEmiCfg);
    if(enPixClkRangeIndex >= FMT_PIXCLK_RANGE_BUTT)
    {
        HDMI_ERR("enPixClkRangeIndex get error! \n");
        return HI_FAILURE;
    }

    EmiParamSet(pstEmiCfg, enPixClkRangeIndex);

    return HI_SUCCESS;
}

HI_VOID HDMI_EmiStatusGet(HDMI_EMI_STATUS_S *pstEmiStatus)
{
    HI_U32 u32RegVal;
    if(pstEmiStatus == HI_NULL)
    {
        HDMI_ERR("pstEmiStatus is null! \n");
        return;
    }

#ifndef HDMI_BUILD_IN_BOOT
    u32RegVal = HDMI_CRG_80;
#else
    u32RegVal = HdmiRegRead((volatile HI_VOID *)g_EmiRegAddr[0][MAXREGVALS - 1]);
#endif

    pstEmiStatus->bHWEmiEnable = (u32RegVal & 0x01) ? HI_TRUE : HI_FALSE;
    pstEmiStatus->bSWEmiEnable = stEmiEnable.bSWEmiEnable;
    pstEmiStatus->bDebugEnable = stEmiEnable.bDebugEnable;
}


