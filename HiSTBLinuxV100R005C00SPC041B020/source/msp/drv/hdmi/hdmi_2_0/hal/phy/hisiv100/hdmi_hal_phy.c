/******************************************************************************

  Copyright (C), 2014-2024, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hdmi_hal_phy.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2016/07/23
  Description   :
  History       :
  Date          : 2016/07/23
  Author        : owx354332
  Modification  :
*******************************************************************************/
#include "hdmi_hal_phy.h"
#include "hdmi_reg_dphy.h"
#include "hdmi_product_define.h"
#include "hdmi_reg_tx.h"

#define PHY_NULL_CHK(p) \
do{\
    if(HI_NULL == p) {\
	    HDMI_ERR("%s is null pointer!return fail.\n",#p);\
	    return HI_FAILURE;}\
}while(0)

#define PHY_NULL_CHK_NO_RET(p) \
do{\
    if(HI_NULL == p) {\
        HDMI_ERR("%s is null pointer!return fail.\n",#p);}\
}while(0)

#define PHY_RET_CHK(ret) \
do{\
    if(HI_FAILURE == ret) {\
        HDMI_ERR("Line: %d ret is failure\n", __LINE__);\
        return HI_FAILURE;}\
}while(0)

#define PHY_INIT_CHK do{\
    if(HI_TRUE != s_stHDMIPhyInfo.bInit)\
    {HDMI_ERR("phy intf module didn't init!return fail.\n");\
    return HI_FAILURE;}\
}while(0)


#define PHY_ARRAY_SIZEOF(a)  (sizeof(a)/sizeof(a[0]))


#define PHY_LOOP_DIV_CFG_25      0x640000    /* 25 x 4 = 100 = 0x64 */
#define PHY_LOOP_DIV_CFG_22      0x5a0000    /* 22 x 4 = 90  = 0x5a */

#define PHY_HWSPEC_I_MAX         43
#define PHY_RATIO_1_10_MASK      0x1f        /* 10bit/2 =1 */
#define PHY_RATIO_1_40_MASK      0xfffff     /* 40bit/2 =1 */
#define PHY_TXPLL_TEST_OFF       0
#define PHY_TXPLL_TEST_ON        0x30000
#define PHY_POWER_ON_MASK_CLK    0xe
#define PHY_POWER_ON_MASK_D0     0xd
#define PHY_POWER_ON_MASK_D1     0xb
#define PHY_POWER_ON_MASK_D2     0x7
#define PHY_POWER_OFF            0xf
#define PHY_POWER_ON             0
#define PHY_POWER_LDO_OFF        0x3
#define PHY_OE_OFF               0xf
#define PHY_OE_ON                0
#define PHY_I_CHARGE_PUMP        0x3
#define PHY_PLL_LOOP_DEVIDER     0x20



/******************************************************************************/
/* Data structure                                                             */
/******************************************************************************/
typedef enum
{
    PHY_CLK_RATIO_1_10 = 0x0,
    PHY_CLK_RATIO_1_40
}PHY_CLK_RATIO_E;

typedef enum
{
    INTERPOLATION_MODE_1X = 0x0,
    INTERPOLATION_MODE_2X = 0x1,
    INTERPOLATION_MODE_4X = 0x2,
    INTERPOLATION_MODE_8X = 0x3,
    INTERPOLATION_MODE_BUTT

} PHY_DIV_MODE_E;

typedef struct
{
    PHY_RTERM_MODE_E    enRTermMode;
    HI_U32              u32ModeDataClk;
    HI_U32              u32ModeDataD0;
    HI_U32              u32ModeDataD1;
    HI_U32              u32ModeDataD2;
} PHY_RTERM_MODE_DATA_E;

typedef struct
{
    HI_U32   u32PhyGcLdoPd;
    HI_U32   u32PhyGcDePd;
    HI_BOOL  bPhyGcBistPd;
    HI_BOOL  bPhyGcTxpllPd;
    HI_BOOL  bPhyGcRxsensePd;

} PHY_POWER_CFG_S;

typedef struct
{
    HI_U32              u32ClkMin;
    HI_U32              u32ClkMax;
}PHY_CLK_RANGE_S;

typedef struct
{
    PHY_CLK_RANGE_S     stPhyTmdsClkRange;
    PHY_HWSPEC_CFG_S    stPhyHWSpecCfg;
}PHY_HWSPEC_S;

typedef struct
{
    PHY_CLK_RANGE_S     stPhyTmdsClkRange;
    PHY_SSC_CFG_S       stPhySscCfg;
}PHY_SSC_S;

typedef struct
{
    PHY_CLK_RANGE_S     stPixClkRange;
    HDMI_DEEP_COLOR_E   enDeepColor;
    HI_U32              u32PixelClkDiv;
    /* CRG */
    HI_U32              u32SscBypDiv;
    HI_U32              u32TmdsNxdiv;
    /* PHY */
    //HI_U32              u32InputDiv;
    HI_U32              u32LoopDiv;
    PHY_CLK_RATIO_E     enClkRatio;
    PHY_DIV_MODE_E      enPhyDivMode;

}PHY_DIV_CFG_S;

/******************************************************************************/
/* Private value                                                              */
/******************************************************************************/

#ifndef HDMI_FPGA_SUPPORT
static PHY_HWSPEC_S s_stPhyHWSpecTab[] =
{
/****|-TmdsclkRange---||---DeIMain----||-iMain/0.5mA--||---DeIPre---- ||-iPre/0.25mA--||----------------DeRPre/Ohm------------------------||----------------RPre/Ohm-------------------------||------------------------RTerm--------------------------------------------------------------------------------------|**
*****|   Min  Max     ||clk d0  d1  d2||clk d0  d1  d2||clk d0  d1  d2|| clk d0  d1 d2||     clk       d0            d1          d2       ||     clk       d0            d1          d2      ||  clk_rterm_mode    clk_rterm    d0_rterm_mode   d0_rterm    d1_rterm_mode    d1_rterm     clk_rterm_mode  d2_rterm|*/
    {{ 25000, 100000}, { 5,  0,  0,  0, 18, 25, 25, 25,  2,  0,  0,  0, 20, 20, 20, 20, PHY_RPRE_50, PHY_RPRE_50, PHY_RPRE_50, PHY_RPRE_50, PHY_RPRE_83, PHY_RPRE_83, PHY_RPRE_83, PHY_RPRE_83, PHY_RTERM_MODE_LOAD  , 0x20, PHY_RTERM_MODE_LOAD  , 0x20, PHY_RTERM_MODE_LOAD  , 0x20, PHY_RTERM_MODE_LOAD  , 0x20}},
    {{100001, 165000}, { 5,  5,  5,  5, 22, 23, 23, 23, 10, 25, 25, 25, 20, 20, 20, 20, PHY_RPRE_50, PHY_RPRE_50, PHY_RPRE_50, PHY_RPRE_50, PHY_RPRE_83, PHY_RPRE_83, PHY_RPRE_83, PHY_RPRE_83, PHY_RTERM_MODE_LOAD  , 0x60, PHY_RTERM_MODE_LOAD  , 0x20, PHY_RTERM_MODE_LOAD  , 0x20, PHY_RTERM_MODE_LOAD  , 0x20}},
    {{165001, 340000}, { 9,  9,  9,  9, 22, 25, 25, 25, 30, 30, 30, 30, 20, 40, 40, 40, PHY_RPRE_50, PHY_RPRE_50, PHY_RPRE_50, PHY_RPRE_50, PHY_RPRE_50, PHY_RPRE_50, PHY_RPRE_50, PHY_RPRE_50, PHY_RTERM_MODE_LOAD  , 0x60, PHY_RTERM_MODE_LOAD  , 0x20, PHY_RTERM_MODE_LOAD  , 0x20, PHY_RTERM_MODE_LOAD  , 0x20}},
    {{340001, 600000}, { 5,  0,  0,  0, 29, 43, 43, 43,  0,  0,  0,  0, 20, 20, 20, 20, PHY_RPRE_50, PHY_RPRE_50, PHY_RPRE_50, PHY_RPRE_50, PHY_RPRE_50, PHY_RPRE_83, PHY_RPRE_83, PHY_RPRE_83, PHY_RTERM_MODE_SOURCE, 0x00, PHY_RTERM_MODE_LOAD  , 0x80, PHY_RTERM_MODE_LOAD  , 0x80, PHY_RTERM_MODE_LOAD  , 0x80}},
};

static PHY_SSC_S s_stPhySscCfgTab[] =
{
    /**************** {100x, 1x}*/
    {{ 25000,  70000}, { 0,     0}},  //0ppm
    {{ 70001,  90000}, {33, 45000}},  //0.33%=3300ppm
    {{ 90001, 110000}, {25, 45000}},  //0.25%
    {{110001, 145000}, {20, 45000}},  //0.20%
    {{145001, 180000}, {12, 45000}},  //0.12%
    {{180001, 220000}, {10, 45000}},  //0.10%
    {{220001, 250000}, { 8, 45000}},  //0.8%
    {{250001, 300000}, { 4, 45000}},  //0.4%
    {{300001, 600000}, { 0,     0}},  //0
};

static PHY_RTERM_MODE_DATA_E s_stPhyRTermModeDataTab[] =
{
/***********R Term mode     clk    d0    d1    d2*****/
    {PHY_RTERM_MODE_SINGLE, 0x03, 0x0c, 0x30, 0xc0},
    {PHY_RTERM_MODE_SOURCE, 0x02, 0x08, 0x20, 0x80},
    {PHY_RTERM_MODE_LOAD  , 0x01, 0x04, 0x10, 0x40},
};

static PHY_DIV_CFG_S s_stPhyDivCfgTab[] =
{
/**    pixel-range       deepcolor          pixDiv BypDiv TmdsNxDiv     loopdiv               clkRatio             phyDivMode**/
    {{ 25000,  50000}, HDMI_DEEP_COLOR_24BIT,   16,    10,      8,   PHY_LOOP_DIV_CFG_25   ,  PHY_CLK_RATIO_1_10, INTERPOLATION_MODE_8X},
    {{ 50001, 100000}, HDMI_DEEP_COLOR_24BIT,    8,    10,      4,   PHY_LOOP_DIV_CFG_25   ,  PHY_CLK_RATIO_1_10, INTERPOLATION_MODE_4X},
    {{100001, 200000}, HDMI_DEEP_COLOR_24BIT,    4,    10,      2,   PHY_LOOP_DIV_CFG_25   ,  PHY_CLK_RATIO_1_10, INTERPOLATION_MODE_2X},
    {{200001, 340000}, HDMI_DEEP_COLOR_24BIT,    2,    10,      1,   PHY_LOOP_DIV_CFG_25   ,  PHY_CLK_RATIO_1_10, INTERPOLATION_MODE_1X},
    {{340001, 600000}, HDMI_DEEP_COLOR_24BIT,    1,     5,      1,   PHY_LOOP_DIV_CFG_25   ,  PHY_CLK_RATIO_1_40, INTERPOLATION_MODE_1X},

    {{ 25001,  50000}, HDMI_DEEP_COLOR_30BIT,   16,     8,      8,   PHY_LOOP_DIV_CFG_25   ,  PHY_CLK_RATIO_1_10, INTERPOLATION_MODE_8X},
    {{ 50001, 100000}, HDMI_DEEP_COLOR_30BIT,    8,     8,      4,   PHY_LOOP_DIV_CFG_25   ,  PHY_CLK_RATIO_1_10, INTERPOLATION_MODE_4X},
    {{100001, 200000}, HDMI_DEEP_COLOR_30BIT,    4,     8,      2,   PHY_LOOP_DIV_CFG_25   ,  PHY_CLK_RATIO_1_10, INTERPOLATION_MODE_2X},
    {{200001, 272000}, HDMI_DEEP_COLOR_30BIT,    2,     8,      1,   PHY_LOOP_DIV_CFG_25   ,  PHY_CLK_RATIO_1_10, INTERPOLATION_MODE_1X},
    {{272001, 340000}, HDMI_DEEP_COLOR_30BIT,    2,     8,      1,   PHY_LOOP_DIV_CFG_25   ,  PHY_CLK_RATIO_1_40, INTERPOLATION_MODE_1X},
    {{340001, 600000}, HDMI_DEEP_COLOR_30BIT,    1,     8,      1,   PHY_LOOP_DIV_CFG_25   ,  PHY_CLK_RATIO_1_40, INTERPOLATION_MODE_1X},

    {{ 25001,  50000}, HDMI_DEEP_COLOR_36BIT,   16,     6,      8,   PHY_LOOP_DIV_CFG_22   ,  PHY_CLK_RATIO_1_10, INTERPOLATION_MODE_8X},
    {{ 50001, 100000}, HDMI_DEEP_COLOR_36BIT,    8,     6,      4,   PHY_LOOP_DIV_CFG_22   ,  PHY_CLK_RATIO_1_10, INTERPOLATION_MODE_4X},
    {{100001, 200000}, HDMI_DEEP_COLOR_36BIT,    4,     6,      2,   PHY_LOOP_DIV_CFG_22   ,  PHY_CLK_RATIO_1_10, INTERPOLATION_MODE_2X},
    {{200001, 227000}, HDMI_DEEP_COLOR_36BIT,    2,     6,      1,   PHY_LOOP_DIV_CFG_22   ,  PHY_CLK_RATIO_1_10, INTERPOLATION_MODE_1X},
    {{227001, 340000}, HDMI_DEEP_COLOR_36BIT,    2,     6,      1,   PHY_LOOP_DIV_CFG_22   ,  PHY_CLK_RATIO_1_40, INTERPOLATION_MODE_1X},
    {{340001, 600000}, HDMI_DEEP_COLOR_36BIT,    1,     3,      1,   PHY_LOOP_DIV_CFG_22   ,  PHY_CLK_RATIO_1_40, INTERPOLATION_MODE_1X},

};

static HDMI_PHY_INFO_S s_stHDMIPhyInfo;

/******************************************************************************/
/* Private interface                                                          */
/******************************************************************************/
static HI_U32 PhyTransferCur2Hex(HI_U32 u32Cur)
{
    HI_U32  u32Current = 0;

    if(u32Cur < 16)
    {
        u32Current = u32Cur;
    }
    else if(u32Cur < 28)
    {
        u32Current = (u32Cur + 0x04);
    }
    else if(u32Cur < 32)
    {
        u32Current = (u32Cur + 0x10);
    }
    else if(u32Cur <= PHY_HWSPEC_I_MAX)
    {
        u32Current = (u32Cur + 0x14);
    }
    else
    {
        HDMI_ERR("%d is exceed Max value %d.\n", u32Cur, PHY_HWSPEC_I_MAX);
        u32Current = 0;
    }

    return u32Current;

}

static PHY_HWSPEC_CFG_S *PhyHWSpecDataGet(HI_U32 u32TmdsClk)
{
    HI_U32              i = 0;
    PHY_HWSPEC_S        *pstPhyHWSpec = HI_NULL;

    for(pstPhyHWSpec= &s_stPhyHWSpecTab[0];
        (pstPhyHWSpec && (i<PHY_ARRAY_SIZEOF(s_stPhyHWSpecTab)));
        pstPhyHWSpec++, i++)
    {
        if( (u32TmdsClk >= pstPhyHWSpec->stPhyTmdsClkRange.u32ClkMin) \
            && (u32TmdsClk <= pstPhyHWSpec->stPhyTmdsClkRange.u32ClkMax) )
        {
            return (&pstPhyHWSpec->stPhyHWSpecCfg);
        }
    }

    return HI_NULL;
}

static PHY_SSC_CFG_S *PhySscDataGet(HI_U32 u32TmdsClk)
{
    HI_U32              i = 0;
    PHY_SSC_S           *pstPhySsc = HI_NULL;

    for(pstPhySsc= &s_stPhySscCfgTab[0];
        (pstPhySsc && (i<PHY_ARRAY_SIZEOF(s_stPhySscCfgTab)));
        pstPhySsc++, i++)
    {
        if( (u32TmdsClk >= pstPhySsc->stPhyTmdsClkRange.u32ClkMin) \
            && (u32TmdsClk <= pstPhySsc->stPhyTmdsClkRange.u32ClkMax) )
        {
            return (&pstPhySsc->stPhySscCfg);
        }
    }

    return HI_NULL;
}


static PHY_DIV_CFG_S *PhyDivCfgGet(HI_U32 u32PixClk, HDMI_DEEP_COLOR_E enDeepClr)
{
    HI_U32                  i = 0;
    PHY_DIV_CFG_S          *pstDivCfg = HI_NULL;

    for(pstDivCfg= &s_stPhyDivCfgTab[0];
        (pstDivCfg && (i<PHY_ARRAY_SIZEOF(s_stPhyDivCfgTab)));
        pstDivCfg++, i++)
    {
        if( (u32PixClk >= pstDivCfg->stPixClkRange.u32ClkMin) \
            && (u32PixClk <= pstDivCfg->stPixClkRange.u32ClkMax) \
            && (enDeepClr == pstDivCfg->enDeepColor) )
        {
            return pstDivCfg;
        }
    }

    HDMI_ERR("can't find param,u32TmdsClk:%u,enDeepClr=%u.\n", u32PixClk,enDeepClr);

    return &s_stPhyDivCfgTab[1];
}

static PHY_RTERM_MODE_DATA_E *PhyRTermModeDataGet(PHY_RTERM_MODE_E enRTermMode)
{
    HI_U32                  i = 0;
    PHY_RTERM_MODE_DATA_E   *pstPhyRTermModeCfg = HI_NULL;

    for(pstPhyRTermModeCfg= &s_stPhyRTermModeDataTab[0];
        (pstPhyRTermModeCfg && (i<PHY_ARRAY_SIZEOF(s_stPhyRTermModeDataTab)));
        pstPhyRTermModeCfg++, i++)
    {
        if(pstPhyRTermModeCfg->enRTermMode == enRTermMode)
        {
            return pstPhyRTermModeCfg;
        }
    }

    HDMI_ERR("can't find param,enRTermMode:%u.\n", enRTermMode);

    return &s_stPhyRTermModeDataTab[0];
}

static HI_VOID PhyDivModeSet(PHY_DIV_MODE_E enPhyDivMode)
{
    HDMI_APHY_TOP_PD_reg_divselSet(enPhyDivMode);
    return;
}

static HI_VOID PhyLoopDivSet(HI_U32 u32LoopDiv)
{
    HDMI_HDMITX_FBSSC_SET_reg_set_fbSet(u32LoopDiv);
    return;
}

static HI_VOID PhyClkRatioSet(PHY_CLK_RATIO_E enClkRatio)
{
    HI_U32    u32PhyFifoCk_l = 0;   // 30bit
    HI_U32    u32PhyFifoCk_h = 0;   // 10bit

    switch(enClkRatio)
    {
        case PHY_CLK_RATIO_1_10:
            u32PhyFifoCk_l = PHY_RATIO_1_10_MASK;
            u32PhyFifoCk_l <<= 10;
            u32PhyFifoCk_l |= PHY_RATIO_1_10_MASK;
            u32PhyFifoCk_l <<= 10;
            u32PhyFifoCk_l |= PHY_RATIO_1_10_MASK;
            u32PhyFifoCk_h = PHY_RATIO_1_10_MASK;
            break;
        case PHY_CLK_RATIO_1_40:
            u32PhyFifoCk_l = PHY_RATIO_1_40_MASK;
            u32PhyFifoCk_h = 0;
            break;
        default:
            HDMI_ERR("enClkRatio %d is invalid\n", enClkRatio);
            return;
    }

    HDMI_HDMITX_AFIFO_DATA_SEL_reg_aphy_data_clk_hSet(u32PhyFifoCk_h);
    HDMI_HDMITX_AFIFO_CLK_reg_aphy_data_clk_lSet(u32PhyFifoCk_l);

    return;

}

static HI_S32 PhyHWSpecSet(PHY_HWSPEC_CFG_S *pstHWSpecCfg)
{
    HI_U32 u32Tmp = 0;
    HI_U32 u32DeIMainPower = PHY_POWER_OFF;

    PHY_NULL_CHK(pstHWSpecCfg);

    HDMI_INFO("PHY HWSpec DeIMain   clk:%d, d0:%d, d1:%d, d2:%d\n", pstHWSpecCfg->u32IDeMainClk, pstHWSpecCfg->u32IDeMainD0, pstHWSpecCfg->u32IDeMainD1, pstHWSpecCfg->u32IDeMainD2);
    HDMI_INFO("PHY HWSpec iMain     clk:%d, d0:%d, d1:%d, d2:%d\n", pstHWSpecCfg->u32IMainClk, pstHWSpecCfg->u32IMainD0, pstHWSpecCfg->u32IMainD1, pstHWSpecCfg->u32IMainD2);
    HDMI_INFO("PHY HWSpec iPre      clk:%d, d0:%d, d1:%d, d2:%d\n", pstHWSpecCfg->u32IPreClk, pstHWSpecCfg->u32IPreD0, pstHWSpecCfg->u32IPreD1, pstHWSpecCfg->u32IPreD2);
    HDMI_INFO("PHY HWSpec RPre      clk:%d, d0:%d, d1:%d, d2:%d\n", pstHWSpecCfg->enRPreClk, pstHWSpecCfg->enRPreD0, pstHWSpecCfg->enRPreD1, pstHWSpecCfg->enRPreD2);
    HDMI_INFO("PHY HWSpec RTerm     clk:0x%x, d0:0x%x, d1:0x%x, d2:0x%x\n", pstHWSpecCfg->u32RTermClk, pstHWSpecCfg->u32RTermD0, pstHWSpecCfg->u32RTermD1, pstHWSpecCfg->u32RTermD2);

    /* de iMain*/
    if(0 != pstHWSpecCfg->u32IDeMainClk)
    {
        u32DeIMainPower &= PHY_POWER_ON_MASK_CLK;
    }
    if(0 != pstHWSpecCfg->u32IDeMainD0)
    {
        u32DeIMainPower &= PHY_POWER_ON_MASK_D0;
    }
    if(0 != pstHWSpecCfg->u32IDeMainD1)
    {
        u32DeIMainPower &= PHY_POWER_ON_MASK_D1;
    }
    if(0 != pstHWSpecCfg->u32IDeMainD2)
    {
        u32DeIMainPower &= PHY_POWER_ON_MASK_D2;
    }
    HDMI_APHY_TOP_PD_reg_gc_pd_deSet(u32DeIMainPower);
    HDMI_APHY_DRIVER_IMAIN_reg_isel_main_de_clkSet(PhyTransferCur2Hex(pstHWSpecCfg->u32IDeMainClk));
    HDMI_APHY_DRIVER_IPRE_reg_isel_main_de_d0Set(PhyTransferCur2Hex(pstHWSpecCfg->u32IDeMainD0));
    HDMI_APHY_DRIVER_IPRE_reg_isel_main_de_d1Set(PhyTransferCur2Hex(pstHWSpecCfg->u32IDeMainD1));
    HDMI_APHY_DRIVER_IPRE_reg_isel_main_de_d2Set(PhyTransferCur2Hex(pstHWSpecCfg->u32IDeMainD2));
    /* iMain */
    HDMI_APHY_DRIVER_IMAIN_reg_isel_main_clkSet(PhyTransferCur2Hex(pstHWSpecCfg->u32IMainClk));
    HDMI_APHY_DRIVER_IMAIN_reg_isel_main_d0Set(PhyTransferCur2Hex(pstHWSpecCfg->u32IMainD0));
    HDMI_APHY_DRIVER_IMAIN_reg_isel_main_d1Set(PhyTransferCur2Hex(pstHWSpecCfg->u32IMainD1));
    HDMI_APHY_DRIVER_IMAIN_reg_isel_main_d2Set(PhyTransferCur2Hex(pstHWSpecCfg->u32IMainD2));
    /* de iPre*/
	HDMI_APHY_DRIVER_IPREDE_reg_isel_pre_de_clkSet(PhyTransferCur2Hex(pstHWSpecCfg->u32IDePreClk));
	HDMI_APHY_DRIVER_IPREDE_reg_isel_pre_de_d0Set(PhyTransferCur2Hex(pstHWSpecCfg->u32IDePreD0));
	HDMI_APHY_DRIVER_IPREDE_reg_isel_pre_de_d1Set(PhyTransferCur2Hex(pstHWSpecCfg->u32IDePreD1));
	HDMI_APHY_DRIVER_RPRE_reg_isel_pre_de_d2Set(PhyTransferCur2Hex(pstHWSpecCfg->u32IDePreD2));
    /* iPre */
    HDMI_APHY_DRIVER_IPRE_reg_isel_pre_clkSet(PhyTransferCur2Hex(pstHWSpecCfg->u32IPreClk));
    HDMI_APHY_DRIVER_IPRE_reg_isel_pre_d0Set(PhyTransferCur2Hex(pstHWSpecCfg->u32IPreD0));
    HDMI_APHY_DRIVER_IPREDE_reg_isel_pre_d1Set(PhyTransferCur2Hex(pstHWSpecCfg->u32IPreD1));
    HDMI_APHY_DRIVER_IPREDE_reg_isel_pre_d2Set(PhyTransferCur2Hex(pstHWSpecCfg->u32IPreD2));
    /* de R Pre*/
    HDMI_APHY_DRIVER_RPRE_reg_rsel_pre_de_clkSet((HI_U32)(pstHWSpecCfg->enRDePreClk));
	HDMI_APHY_DRIVER_RPRE_reg_rsel_pre_de_d0Set((HI_U32)(pstHWSpecCfg->enRDePreD0));
	HDMI_APHY_DRIVER_RPRE_reg_rsel_pre_de_d1Set((HI_U32)(pstHWSpecCfg->enRDePreD1));
	HDMI_APHY_DRIVER_RPRE_reg_rsel_pre_de_d2Set((HI_U32)(pstHWSpecCfg->enRDePreD2));
    /* R Pre */
    HDMI_APHY_DRIVER_RPRE_reg_rsel_pre_clkSet((HI_U32)(pstHWSpecCfg->enRPreClk));
    HDMI_APHY_DRIVER_RPRE_reg_rsel_pre_d0Set((HI_U32)(pstHWSpecCfg->enRPreD0));
    HDMI_APHY_DRIVER_RPRE_reg_rsel_pre_d1Set((HI_U32)(pstHWSpecCfg->enRPreD1));
    HDMI_APHY_DRIVER_RPRE_reg_rsel_pre_d2Set((HI_U32)(pstHWSpecCfg->enRPreD2));
    /* R Term */
    HDMI_APHY_RTERM_CTRL_reg_rt_clkSet(pstHWSpecCfg->u32RTermClk);
    HDMI_APHY_RTERM_CTRL_reg_rt_d0Set(pstHWSpecCfg->u32RTermD0);
    HDMI_APHY_RTERM_CTRL_reg_rt_d1Set(pstHWSpecCfg->u32RTermD1);
    HDMI_APHY_RTERM_CTRL_reg_rt_d2Set(pstHWSpecCfg->u32RTermD2);
    u32Tmp |= PhyRTermModeDataGet(pstHWSpecCfg->enRTermModeClk)->u32ModeDataClk;
    u32Tmp |= PhyRTermModeDataGet(pstHWSpecCfg->enRTermModeD0)->u32ModeDataD0;
    u32Tmp |= PhyRTermModeDataGet(pstHWSpecCfg->enRTermModeD1)->u32ModeDataD1;
    u32Tmp |= PhyRTermModeDataGet(pstHWSpecCfg->enRTermModeD2)->u32ModeDataD2;
    HDMI_APHY_TOP_PD_reg_gc_pd_rtermSet(u32Tmp);

    /* status info set */
    HDMI_MEMCPY(&s_stHDMIPhyInfo.stHWSpecCfg, pstHWSpecCfg, sizeof(PHY_HWSPEC_CFG_S));

    return HI_SUCCESS;

}

#endif

/******************************************************************************/
/* Public interface                                                           */
/******************************************************************************/
HI_S32 HAL_HDMI_PhyInit(HI_VOID)
{
#ifndef HDMI_FPGA_SUPPORT

    if(!s_stHDMIPhyInfo.bInit)
    {
        HDMI_HDMITX_PHY_reg_Init();
        /* init */
        HDMI_APHY_TXPLL_CTRL_reg_txpll_icp_ictrlSet(PHY_I_CHARGE_PUMP);
        HDMI_APHY_TXPLL_CTRL_reg_txpll_divsel_loopSet(PHY_PLL_LOOP_DEVIDER);
        HDMI_HDMITX_INSSC_SET_reg_sscin_bypass_enSet(HI_TRUE);
    	HDMI_HDMITX_INSSC_SET_reg_pllfbmash111_enSet(HI_TRUE);
        s_stHDMIPhyInfo.bInit = HI_TRUE;
    }

#endif
    return HI_SUCCESS;
}


HI_S32 HAL_HDMI_PhyDeInit(HI_VOID)
{
#ifndef HDMI_FPGA_SUPPORT

    PHY_INIT_CHK;
    HAL_HDMI_PhyReset();
    HDMI_HDMITX_PHY_reg_DeInit();
    s_stHDMIPhyInfo.bInit = HI_FALSE;

#endif

    return HI_SUCCESS;
}


HI_S32 HAL_HDMI_PhyReset(HI_VOID)
{
#ifndef HDMI_FPGA_SUPPORT

    PHY_INIT_CHK;

    HAL_HDMI_PhyOeSet(HI_FALSE);
    HAL_HDMI_PhyPowerSet(HI_FALSE);


    DRV_HDMI_ProdCrgPhyResetSet(HI_TRUE);

#endif

    return HI_SUCCESS;
}


HI_S32 HAL_HDMI_PhyPowerGet(HI_BOOL *pbEnable)
{
#ifndef HDMI_FPGA_SUPPORT

    PHY_POWER_CFG_S stPhyPowerCfg;

    PHY_INIT_CHK;
    PHY_NULL_CHK(pbEnable);

    HDMI_MEMSET(&stPhyPowerCfg, 0, sizeof(PHY_POWER_CFG_S));

    stPhyPowerCfg.bPhyGcTxpllPd   = HDMI_APHY_TOP_PD_reg_gc_txpll_pdGet();
    stPhyPowerCfg.bPhyGcRxsensePd = HDMI_APHY_TOP_PD_reg_gc_pd_rxsenseGet();
    stPhyPowerCfg.u32PhyGcLdoPd   = HDMI_APHY_TOP_PD_reg_gc_pd_ldoGet();
    stPhyPowerCfg.u32PhyGcDePd    = HDMI_APHY_TOP_PD_reg_gc_pd_deGet();
    stPhyPowerCfg.bPhyGcBistPd    = HDMI_APHY_TOP_PD_reg_gc_pd_bistGet();

    if ((PHY_POWER_ON == stPhyPowerCfg.bPhyGcTxpllPd)
        && (PHY_POWER_ON == stPhyPowerCfg.bPhyGcRxsensePd)
        && (PHY_POWER_ON == stPhyPowerCfg.u32PhyGcLdoPd)
        && stPhyPowerCfg.bPhyGcBistPd )
    {
        *pbEnable = HI_TRUE;
    }
    else if (stPhyPowerCfg.bPhyGcTxpllPd
             && stPhyPowerCfg.bPhyGcRxsensePd
             && (PHY_POWER_LDO_OFF & stPhyPowerCfg.u32PhyGcLdoPd)
             && (PHY_POWER_OFF & stPhyPowerCfg.bPhyGcBistPd) )
    {
        *pbEnable = HI_FALSE;
    }
    else
    {
        HDMI_WARN("phy power configure is abnormal, please check!\n");
        *pbEnable = HI_FALSE;
    }

#endif

    return HI_SUCCESS;
}


HI_S32 HAL_HDMI_PhyPowerSet(HI_BOOL bEnable)
{
#ifndef HDMI_FPGA_SUPPORT

    PHY_INIT_CHK;

    if(bEnable)
    {
        HDMI_APHY_TOP_PD_reg_gc_txpll_pdSet(PHY_POWER_ON);
        HDMI_APHY_TOP_PD_reg_gc_pd_rxsenseSet(PHY_POWER_ON);
        HDMI_APHY_TOP_PD_reg_gc_pd_ldoSet(PHY_POWER_ON);
    }
    else
    {
        HDMI_APHY_TOP_PD_reg_gc_txpll_pdSet(PHY_POWER_OFF);
        HDMI_APHY_TOP_PD_reg_gc_pd_rxsenseSet(PHY_POWER_OFF);
        HDMI_APHY_TOP_PD_reg_gc_pd_ldoSet(PHY_POWER_OFF);
        HDMI_APHY_TOP_PD_reg_gc_pd_deSet(PHY_POWER_OFF);
    }
    HDMI_APHY_TOP_PD_reg_gc_pd_bistSet(PHY_POWER_OFF);

    s_stHDMIPhyInfo.bPowerEnable  = bEnable;

#endif

    return HI_SUCCESS;
}

HI_S32 HAL_HDMI_PhyOeSet(HI_BOOL bEnable)
{
#ifndef HDMI_FPGA_SUPPORT
    HI_U32  u32Oe = 0;

    PHY_INIT_CHK;

    u32Oe = bEnable ? PHY_OE_ON : PHY_OE_OFF;
    HDMI_APHY_TOP_PD_reg_gc_pdSet(u32Oe);

    s_stHDMIPhyInfo.bOeEnable     = bEnable;
#endif

    return HI_SUCCESS;
}

HI_S32 HAL_HDMI_PhyOeGet(HI_BOOL *pbEnable)
{
#ifndef HDMI_FPGA_SUPPORT
    HI_BOOL bPhyReset = HI_FALSE;

    PHY_INIT_CHK;
    PHY_NULL_CHK(pbEnable);

    DRV_HDMI_ProdCrgPhyResetGet(&bPhyReset);
    if(!bPhyReset)
    {
        *pbEnable = (PHY_OE_ON == HDMI_APHY_TOP_PD_reg_gc_pdGet())? HI_TRUE: HI_FALSE;
    }
    else
    {
        *pbEnable = HI_FALSE;
    }

#endif

    return HI_SUCCESS;
}

//for video path
HI_S32 HAL_HDMI_PhyTmdsSet(HDMI_PHY_TMDS_CFG_S *pstHdmiTmdsCfg)
{
    HI_S32                  s32Ret           = HI_SUCCESS;
#ifndef HDMI_FPGA_SUPPORT
    PHY_DIV_CFG_S          *pstDivCfg        = HI_NULL;
    HDMI_DEEP_COLOR_E       enDpColor        = HDMI_DEEP_COLOR_BUTT;
    HDMI_CRG_CFG_S          stHDMICrgCfg     = {0};
    HDMI_PHY_SSC_CFG_S      stHdmiSscCfg     = {0};
    HDMI_PHY_HWSPEC_CFG_S   stHdmiHWSpecCfg  = {0};

    PHY_INIT_CHK;
    PHY_NULL_CHK(pstHdmiTmdsCfg);

    enDpColor   = pstHdmiTmdsCfg->enDeepColor;
    if(HDMI_DEEP_COLOR_OFF == enDpColor)
    {
        enDpColor = HDMI_DEEP_COLOR_24BIT;
    }

    /* get config para */
    pstDivCfg = PhyDivCfgGet(pstHdmiTmdsCfg->u32PixelClk, enDpColor);
    PHY_NULL_CHK(pstDivCfg);

    HAL_HDMI_PhyPowerSet(HI_TRUE);

    /* set phy div mode */
    PhyDivModeSet(pstDivCfg->enPhyDivMode);

    /* set crg div */
    stHDMICrgCfg.u32SscBypassDiv = pstDivCfg->u32SscBypDiv;
    stHDMICrgCfg.u32TmdsClkDiv   = pstDivCfg->u32TmdsNxdiv;
    DRV_HDMI_ProdCrgDivSet(&stHDMICrgCfg);

    /* set clk ratio */
    PhyClkRatioSet(pstDivCfg->enClkRatio);

    /* set loopdiv */
    PhyLoopDivSet(pstDivCfg->u32LoopDiv);

    /* Spread Spectrum ctrl(ssc) */
    stHdmiSscCfg.u32PixClk                  = pstHdmiTmdsCfg->u32PixelClk;
    stHdmiSscCfg.u32TmdsClk                 = pstHdmiTmdsCfg->u32TmdsClk;
    stHdmiSscCfg.enDeepColor                = enDpColor;
    stHdmiSscCfg.stHDMIPhySsc.bSscEnable    = pstHdmiTmdsCfg->bEmiEnable;
    stHdmiSscCfg.stHDMIPhySsc.bSscDebugEn   = HI_FALSE;
    HAL_HDMI_PhySscSet(&stHdmiSscCfg);

    /* set HardWare Specification */
    stHdmiHWSpecCfg.u32TmdsClk                   = pstHdmiTmdsCfg->u32TmdsClk;
    stHdmiHWSpecCfg.stHDMIPhySpec.bHWSpecDebugEn = HI_FALSE;
    HAL_HDMI_PhySpecSet(&stHdmiHWSpecCfg);

    /* status info set */
    HDMI_MEMCPY(&s_stHDMIPhyInfo.stTmdsCfg, pstHdmiTmdsCfg, sizeof(HDMI_PHY_TMDS_CFG_S));

#endif

    return s32Ret;

}

//for video path & debug
HI_S32 HAL_HDMI_PhySpecSet(HDMI_PHY_HWSPEC_CFG_S * pstHdmiSpecCfg)
{
    HI_S32                  s32Ret              = HI_SUCCESS;
#ifndef HDMI_FPGA_SUPPORT
    PHY_HWSPEC_CFG_S       *pstPhyHWSpecCfgTmp = HI_NULL;

    PHY_INIT_CHK;
    PHY_NULL_CHK(pstHdmiSpecCfg);

    /* Spread Spectrum ctrl(ssc) */
    pstPhyHWSpecCfgTmp =  pstHdmiSpecCfg->stHDMIPhySpec.bHWSpecDebugEn ? \
                            &pstHdmiSpecCfg->stHDMIPhySpec.stPhyHWSpecCfg: \
                            PhyHWSpecDataGet(pstHdmiSpecCfg->u32TmdsClk);
    PHY_NULL_CHK(pstPhyHWSpecCfgTmp);
    s32Ret = PhyHWSpecSet(pstPhyHWSpecCfgTmp);

#endif

    return s32Ret;

}

//for video path & debug
HI_S32 HAL_HDMI_PhySscSet(HDMI_PHY_SSC_CFG_S * pstHdmiSscCfg)
{
    HI_S32                  s32Ret           = HI_SUCCESS;
#ifndef HDMI_FPGA_SUPPORT
    HI_U32                  u32PhyRefClk = 0;
    HI_U32                  u32SpanFb    = 0;
    HI_U32                  u32StepFb    = 0;
    HI_U32                  u32Tmp       = 0;
    HDMI_DEEP_COLOR_E       enDpColor    = HDMI_DEEP_COLOR_BUTT;
    PHY_SSC_CFG_S           *pstPhySscCfgTmp = HI_NULL;
    PHY_DIV_CFG_S           *pstDivCfg       = HI_NULL;

    PHY_INIT_CHK;
    PHY_NULL_CHK(pstHdmiSscCfg);

    enDpColor   = pstHdmiSscCfg->enDeepColor;
    if(HDMI_DEEP_COLOR_OFF == enDpColor)
    {
        enDpColor = HDMI_DEEP_COLOR_24BIT;
    }

    /* get config para */
    pstDivCfg = PhyDivCfgGet(pstHdmiSscCfg->u32PixClk, enDpColor);
    PHY_NULL_CHK(pstDivCfg);

    /* Spread Spectrum ctrl(ssc) */
    if(pstHdmiSscCfg->stHDMIPhySsc.bSscEnable)
    {
        pstPhySscCfgTmp = pstHdmiSscCfg->stHDMIPhySsc.bSscDebugEn ? \
                            &pstHdmiSscCfg->stHDMIPhySsc.stPhySscCfg : \
                            PhySscDataGet(pstHdmiSscCfg->u32TmdsClk);

        PHY_NULL_CHK(pstPhySscCfgTmp);


        if(    (0 == pstDivCfg->u32SscBypDiv)
            || (0 == pstPhySscCfgTmp->u32SscFreq) )
        {
            u32SpanFb = 0;
            u32StepFb = 0;
        }
        else
        {
            /****************************************************
            /RefClk Calculation
            /                         pixelclk(Hz) * Pixclkdiv
            /       Reference Clock = ----------------
            /                          SscBypassDiv
            ***************************************************/
            u32PhyRefClk  = pstHdmiSscCfg->u32PixClk * pstDivCfg->u32PixelClkDiv * 1000;
            u32PhyRefClk /= pstDivCfg->u32SscBypDiv;

            /***************************************************
            /   Span Fb calculation
            /                            Ref Clock
            /           Span        = -------------------
            /                          SscFreqence x 4
            ***************************************************/
            u32SpanFb    = (u32PhyRefClk / pstPhySscCfgTmp->u32SscFreq) / 4;
            /*********************************************************************
            /   Step Fb calculation
            /                             SscAmptd x LoopDiv
            /                          ----------------------- x SscFreqcence
            /                                    10000
            /           Step        = ------------------------------------------
            /                                      Ref clock / 4
            ********************************************************************/
            u32Tmp       = (pstDivCfg->u32LoopDiv / 10) * pstPhySscCfgTmp->u32SscAmptd;
            u32Tmp       = u32Tmp * (pstPhySscCfgTmp->u32SscFreq / 1000);
            u32StepFb    = u32PhyRefClk ? (u32Tmp  / (u32PhyRefClk/4) ) : 0 ;
        }

        HDMI_INFO("ssc info || LoopDiv:0x%x, RefClk:%d.\n",pstDivCfg->u32LoopDiv,u32PhyRefClk);
        HDMI_INFO("         || Amptd:%d, Freq:%d, Span:0x%x, Step:0x%x.\n",pstPhySscCfgTmp->u32SscAmptd,pstPhySscCfgTmp->u32SscFreq,u32SpanFb,u32StepFb);

		/* Step 1 */
        HDMI_HDMITX_FBSSC_STEP_reg_step_fbSet(u32StepFb);
        HDMI_HDMITX_FBSSC_STEP_reg_span_fbSet(u32SpanFb);
        /* Step 2 */
        HDMI_HDMITX_FBSSC_SET_reg_ssc_mode_fbSet(HI_TRUE);
        /* Step 3 */
        HDMI_HDMITX_FBSSC_SET_reg_set_load_fbSet(HI_FALSE);
        HDMI_HDMITX_FBSSC_SET_reg_set_load_fbSet(HI_TRUE);
        HDMI_HDMITX_FBSSC_SET_reg_set_load_fbSet(HI_FALSE);
        /* Step 4 */
        HDMI_APHY_TXPLL_TEST_reg_txpll_testSet(PHY_TXPLL_TEST_ON);
        /* Step 5 */
        HDMI_APHY_TXPLL_CTRL_reg_gc_txpll_en_sscdivSet(HI_TRUE);
    }
    else
    {
        HDMI_HDMITX_FBSSC_STEP_reg_step_fbSet(0);
        HDMI_HDMITX_FBSSC_STEP_reg_span_fbSet(0);

        /* 12bit need to enter decimal mode */
        if(HDMI_DEEP_COLOR_36BIT == pstDivCfg->enDeepColor)
        {
            /* Step 1 */
            HDMI_HDMITX_FBSSC_SET_reg_ssc_mode_fbSet(HI_TRUE);
            /* Step 2 */
            HDMI_HDMITX_FBSSC_SET_reg_set_load_fbSet(HI_FALSE);
            HDMI_HDMITX_FBSSC_SET_reg_set_load_fbSet(HI_TRUE);
            HDMI_HDMITX_FBSSC_SET_reg_set_load_fbSet(HI_FALSE);
            /* Step 3 */
            HDMI_APHY_TXPLL_TEST_reg_txpll_testSet(PHY_TXPLL_TEST_ON);
            /* Step 4 */
            HDMI_APHY_TXPLL_CTRL_reg_gc_txpll_en_sscdivSet(HI_TRUE);
        }
        else
        {
            HDMI_APHY_TXPLL_CTRL_reg_gc_txpll_en_sscdivSet(HI_FALSE);
            HDMI_APHY_TXPLL_TEST_reg_txpll_testSet(PHY_TXPLL_TEST_OFF);
            HDMI_HDMITX_FBSSC_SET_reg_ssc_mode_fbSet(HI_FALSE);
        }
    }

    /* status info set */
    HDMI_MEMCPY(&s_stHDMIPhyInfo.stSscCfg, &pstHdmiSscCfg->stHDMIPhySsc, sizeof(HDMI_PHY_SSC_S));

    return HI_SUCCESS;

#endif

    return s32Ret;

}

HI_S32 HAL_HDMI_PhySscGet(HI_BOOL * pbEnable)
{
#ifndef HDMI_FPGA_SUPPORT

    HI_U32                  u32SpanFb        = 0;
    HI_U32                  u32StepFb        = 0;
    HI_BOOL                 bHWSscEnable     = HI_FALSE;

    PHY_NULL_CHK(pbEnable);

    u32SpanFb    = HDMI_HDMITX_FBSSC_STEP_reg_step_fbGet();
    u32StepFb    = HDMI_HDMITX_FBSSC_STEP_reg_span_fbGet();
    bHWSscEnable = ((0 == u32SpanFb) && (0 == u32StepFb)) ? HI_FALSE : HI_TRUE;

    *pbEnable = bHWSscEnable;

#endif

    return HI_SUCCESS;

}


HI_S32 HAL_HDMI_PhyInfoGet(HDMI_PHY_INFO_S *pstPhyStatus)
{
#ifndef HDMI_FPGA_SUPPORT

    HDMI_MEMCPY(pstPhyStatus, &s_stHDMIPhyInfo, sizeof(HDMI_PHY_INFO_S));

#endif

    return HI_SUCCESS;
}


