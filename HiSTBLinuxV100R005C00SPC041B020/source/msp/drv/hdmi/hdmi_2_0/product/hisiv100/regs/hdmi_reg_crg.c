// ****************************************************************************** 
// Copyright     :  Copyright (C) 2016, Hisilicon Technologies Co. Ltd.
// File name     :  hdmi_reg_crg.c
// Project line  :  Platform And Key Technologies Development
// Department    :  CAD Development Department
// Author        :  xxx
// Version       :  1
// Date          :  2016/7/18
// Description   :  The description of xxx project
// Others        :   
// History       :  xxx 2016/07/18 10:40:30 Create file
// ******************************************************************************

#include "hi_type.h"
#include "hdmi_reg_crg.h"
#include "hdmi_product_define.h"
#include "hdmi_platform.h"
#ifndef HDMI_BUILD_IN_BOOT
#include "drv_hdmi_platform.h"
#endif


#define  HDMI_ADDR_CRG  0xf8a22000

/* Define the struct pointor of the module hdmi2tx_dphy_reg */
volatile HDMI_REG_CRG_S *s_pstRegCrg = NULL;


int REG_HDMI_CRG_reg_Init(void)
{
    if (NULL != s_pstRegCrg)
    {
        return HI_SUCCESS;
    }
    
    s_pstRegCrg  = (HDMI_REG_CRG_S*)HDMI_IO_MAP(HDMI_ADDR_CRG, sizeof(HDMI_REG_CRG_S));

    if(NULL == s_pstRegCrg)
    {
        HDMI_ERR("ioremap_nocache gophdmi2tx_dphy_regAllReg failed!!!!!!!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

int REG_HDMI_CRG_reg_DeInit(void)
{
    if(HI_NULL != s_pstRegCrg)
    {
        HDMI_IO_UNMAP(s_pstRegCrg);    
        s_pstRegCrg = HI_NULL;
    } 

    return HI_SUCCESS;

}

int REG_HDMI_CRG_hpll_frac_Set(unsigned int uhpll_frac)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG_PLL10 o_peri_crg_pll10;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG_PLL10.u32);

    o_peri_crg_pll10.u32 = HDMITXRegRead(pu32RegAddr);
    o_peri_crg_pll10.bits.hpll_frac = uhpll_frac;
    HDMITXRegWrite(pu32RegAddr, o_peri_crg_pll10.u32);
    
    return HI_SUCCESS;
}

int REG_HDMI_CRG_hpll_frac_Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG_PLL10 o_peri_crg_pll10;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG_PLL10.u32);
    o_peri_crg_pll10.u32 = HDMITXRegRead(pu32RegAddr);
    return o_peri_crg_pll10.bits.hpll_frac;
}

int REG_HDMI_CRG_hpll_postdiv1_Set(unsigned int uhpll_postdiv1)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG_PLL10 o_peri_crg_pll10;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG_PLL10.u32);

    o_peri_crg_pll10.u32 = HDMITXRegRead(pu32RegAddr);
    o_peri_crg_pll10.bits.hpll_postdiv1 = uhpll_postdiv1;
    HDMITXRegWrite(pu32RegAddr, o_peri_crg_pll10.u32);
    
    return HI_SUCCESS;
}

int REG_HDMI_CRG_hpll_postdiv1_Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG_PLL10 o_peri_crg_pll10;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG_PLL10.u32);
    o_peri_crg_pll10.u32 = HDMITXRegRead(pu32RegAddr);
    return o_peri_crg_pll10.bits.hpll_postdiv1;
}


int REG_HDMI_CRG_hpll_postdiv2_Set(unsigned int uhpll_postdiv2)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG_PLL10 o_peri_crg_pll10;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG_PLL10.u32);

    o_peri_crg_pll10.u32 = HDMITXRegRead(pu32RegAddr);
    o_peri_crg_pll10.bits.hpll_postdiv2 = uhpll_postdiv2;
    HDMITXRegWrite(pu32RegAddr, o_peri_crg_pll10.u32);
    
    return HI_SUCCESS;
}

int REG_HDMI_CRG_hpll_postdiv2_Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG_PLL10 o_peri_crg_pll10;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG_PLL10.u32);
    o_peri_crg_pll10.u32 = HDMITXRegRead(pu32RegAddr);
    return o_peri_crg_pll10.bits.hpll_postdiv2;
}

int REG_HDMI_CRG_hpll_fbdiv_Set(unsigned int uhpll_fbdiv)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG_PLL11 o_peri_crgpll11;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG_PLL11.u32);

    o_peri_crgpll11.u32 = HDMITXRegRead(pu32RegAddr);
    o_peri_crgpll11.bits.hpll_fbdiv = uhpll_fbdiv;
    HDMITXRegWrite(pu32RegAddr, o_peri_crgpll11.u32);
    
    return HI_SUCCESS;
}

int REG_HDMI_CRG_hpll_fbdiv_Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG_PLL11 o_peri_crgpll11;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG_PLL11.u32);
    o_peri_crgpll11.u32 = HDMITXRegRead(pu32RegAddr);
    return o_peri_crgpll11.bits.hpll_fbdiv;
}

int REG_HDMI_CRG_hpll_refdiv_Set(unsigned int uhpll_refdiv)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG_PLL11 o_peri_crgpll11;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG_PLL11.u32);

    o_peri_crgpll11.u32 = HDMITXRegRead(pu32RegAddr);
    o_peri_crgpll11.bits.hpll_refdiv = uhpll_refdiv;
    HDMITXRegWrite(pu32RegAddr, o_peri_crgpll11.u32);
    
    return HI_SUCCESS;
}

int REG_HDMI_CRG_hpll_refdiv_Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG_PLL11 o_peri_crgpll11;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG_PLL11.u32);
    o_peri_crgpll11.u32 = HDMITXRegRead(pu32RegAddr);
    return o_peri_crgpll11.bits.hpll_refdiv;
}

int REG_HDMI_CRG_hpll_pd_Set(unsigned int uhpll_pd)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG_PLL11 o_peri_crgpll11;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG_PLL11.u32);

    o_peri_crgpll11.u32 = HDMITXRegRead(pu32RegAddr);
    o_peri_crgpll11.bits.hpll_pd = uhpll_pd;
    HDMITXRegWrite(pu32RegAddr, o_peri_crgpll11.u32);
    
    return HI_SUCCESS;
}

int REG_HDMI_CRG_hpll_pd_Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG_PLL11 o_peri_crgpll11;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG_PLL11.u32);
    o_peri_crgpll11.u32 = HDMITXRegRead(pu32RegAddr);
    return o_peri_crgpll11.bits.hpll_pd;
}


int REG_HDMI_CRG_hpll_foutvcopd_Set(unsigned int uhpll_foutvcopd)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG_PLL11 o_peri_crgpll11;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG_PLL11.u32);

    o_peri_crgpll11.u32 = HDMITXRegRead(pu32RegAddr);
    o_peri_crgpll11.bits.hpll_foutvcopd = uhpll_foutvcopd;
    HDMITXRegWrite(pu32RegAddr, o_peri_crgpll11.u32);
    
    return HI_SUCCESS;
}

int REG_HDMI_CRG_hpll_foutvcopd_Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG_PLL11 o_peri_crgpll11;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG_PLL11.u32);
    o_peri_crgpll11.u32 = HDMITXRegRead(pu32RegAddr);
    return o_peri_crgpll11.bits.hpll_foutvcopd;
}


int REG_HDMI_CRG_hpll_fout4phasepdPLL_Set(unsigned int uhpll_fout4phasepdPLL)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG_PLL11 o_peri_crgpll11;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG_PLL11.u32);

    o_peri_crgpll11.u32 = HDMITXRegRead(pu32RegAddr);
    o_peri_crgpll11.bits.hpll_fout4phasepdPLL = uhpll_fout4phasepdPLL;
    HDMITXRegWrite(pu32RegAddr, o_peri_crgpll11.u32);
    
    return HI_SUCCESS;
}

int REG_HDMI_CRG_hpll_fout4phasepdPLL_Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG_PLL11 o_peri_crgpll11;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG_PLL11.u32);
    o_peri_crgpll11.u32 = HDMITXRegRead(pu32RegAddr);
    return o_peri_crgpll11.bits.hpll_fout4phasepdPLL;
}


int REG_HDMI_CRG_hpll_foutpostdivpd_Set(unsigned int uhpll_foutpostdivpd)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG_PLL11 o_peri_crgpll11;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG_PLL11.u32);

    o_peri_crgpll11.u32 = HDMITXRegRead(pu32RegAddr);
    o_peri_crgpll11.bits.hpll_foutpostdivpd = uhpll_foutpostdivpd;
    HDMITXRegWrite(pu32RegAddr, o_peri_crgpll11.u32);
    
    return HI_SUCCESS;
}

int REG_HDMI_CRG_hpll_foutpostdivpd_Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG_PLL11 o_peri_crgpll11;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG_PLL11.u32);
    o_peri_crgpll11.u32 = HDMITXRegRead(pu32RegAddr);
    return o_peri_crgpll11.bits.hpll_foutpostdivpd;
}


int REG_HDMI_CRG_hpll_dacpd_Set(unsigned int uhpll_dacpd)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG_PLL11 o_peri_crgpll11;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG_PLL11.u32);

    o_peri_crgpll11.u32 = HDMITXRegRead(pu32RegAddr);
    o_peri_crgpll11.bits.hpll_dacpd = uhpll_dacpd;
    HDMITXRegWrite(pu32RegAddr, o_peri_crgpll11.u32);
    
    return HI_SUCCESS;
}

int REG_HDMI_CRG_hpll_dacpd_Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG_PLL11 o_peri_crgpll11;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG_PLL11.u32);
    o_peri_crgpll11.u32 = HDMITXRegRead(pu32RegAddr);
    return o_peri_crgpll11.bits.hpll_dacpd;
}


int REG_HDMI_CRG_hpll_dsmpd_Set(unsigned int uhpll_dsmpd)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG_PLL11 o_peri_crgpll11;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG_PLL11.u32);

    o_peri_crgpll11.u32 = HDMITXRegRead(pu32RegAddr);
    o_peri_crgpll11.bits.hpll_dsmpd = uhpll_dsmpd;
    HDMITXRegWrite(pu32RegAddr, o_peri_crgpll11.u32);
    
    return HI_SUCCESS;
}

int REG_HDMI_CRG_hpll_dsmpd_Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG_PLL11 o_peri_crgpll11;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG_PLL11.u32);
    o_peri_crgpll11.u32 = HDMITXRegRead(pu32RegAddr);
    return o_peri_crgpll11.bits.hpll_dsmpd;
}


int REG_HDMI_CRG_hpll_bypass_Set(unsigned int uhpll_bypass)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG_PLL11 o_peri_crgpll11;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG_PLL11.u32);

    o_peri_crgpll11.u32 = HDMITXRegRead(pu32RegAddr);
    o_peri_crgpll11.bits.hpll_bypass = uhpll_bypass;
    HDMITXRegWrite(pu32RegAddr, o_peri_crgpll11.u32);
    
    return HI_SUCCESS;
}

int REG_HDMI_CRG_hpll_bypass_Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG_PLL11 o_peri_crgpll11;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG_PLL11.u32);
    o_peri_crgpll11.u32 = HDMITXRegRead(pu32RegAddr);
    return o_peri_crgpll11.bits.hpll_bypass;
}

int REG_HDMI_CRG_vo_bus_cken_Set(unsigned int uvo_bus_cken)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG54 o_peri_crg54;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG54.u32);

    o_peri_crg54.u32 = HDMITXRegRead(pu32RegAddr);
    o_peri_crg54.bits.vo_bus_cken = uvo_bus_cken;
    HDMITXRegWrite(pu32RegAddr, o_peri_crg54.u32);
    
    return HI_SUCCESS;
}

int REG_HDMI_CRG_vo_bus_cken_Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG54 o_peri_crg54;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG54.u32);
    o_peri_crg54.u32 = HDMITXRegRead(pu32RegAddr);
    return o_peri_crg54.bits.vo_bus_cken;
}

int REG_HDMI_CRG_vo_sd_cken_Set(unsigned int uvo_sd_cken)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG54 o_peri_crg54;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG54.u32);

    o_peri_crg54.u32 = HDMITXRegRead(pu32RegAddr);
    o_peri_crg54.bits.vo_sd_cken = uvo_sd_cken;
    HDMITXRegWrite(pu32RegAddr, o_peri_crg54.u32);
    
    return HI_SUCCESS;
}

int REG_HDMI_CRG_vo_sd_cken_Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG54 o_peri_crg54;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG54.u32);
    o_peri_crg54.u32 = HDMITXRegRead(pu32RegAddr);
    return o_peri_crg54.bits.vo_sd_cken;
}

int REG_HDMI_CRG_vo_sdate_cken_Set(unsigned int uvo_sdate_cken)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG54 o_peri_crg54;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG54.u32);

    o_peri_crg54.u32 = HDMITXRegRead(pu32RegAddr);
    o_peri_crg54.bits.vo_sdate_cken = uvo_sdate_cken;
    HDMITXRegWrite(pu32RegAddr, o_peri_crg54.u32);
    
    return HI_SUCCESS;
}

int REG_HDMI_CRG_vo_sdate_cken_Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG54 o_peri_crg54;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG54.u32);
    o_peri_crg54.u32 = HDMITXRegRead(pu32RegAddr);
    return o_peri_crg54.bits.vo_sdate_cken;
}

int REG_HDMI_CRG_vo_hd_cken_Set(unsigned int uvo_hd_cken)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG54 o_peri_crg54;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG54.u32);

    o_peri_crg54.u32 = HDMITXRegRead(pu32RegAddr);
    o_peri_crg54.bits.vo_hd_cken = uvo_hd_cken;
    HDMITXRegWrite(pu32RegAddr, o_peri_crg54.u32);
    
    return HI_SUCCESS;
}

int REG_HDMI_CRG_vo_hd_cken_Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG54 o_peri_crg54;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG54.u32);
    o_peri_crg54.u32 = HDMITXRegRead(pu32RegAddr);
    return o_peri_crg54.bits.vo_hd_cken;
}

int REG_HDMI_CRG_vdac_ch0_cken_Set(unsigned int uvdac_ch0_cken)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG54 o_peri_crg54;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG54.u32);

    o_peri_crg54.u32 = HDMITXRegRead(pu32RegAddr);
    o_peri_crg54.bits.vdac_ch0_cken = uvdac_ch0_cken;
    HDMITXRegWrite(pu32RegAddr, o_peri_crg54.u32);
    
    return HI_SUCCESS;
}

int REG_HDMI_CRG_vdac_ch0_cken_Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG54 o_peri_crg54;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG54.u32);
    o_peri_crg54.u32 = HDMITXRegRead(pu32RegAddr);
    return o_peri_crg54.bits.vdac_ch0_cken;
}

int REG_HDMI_CRG_vo_sd_clk_sel_Set(unsigned int uvo_sd_clk_sel)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG54 o_peri_crg54;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG54.u32);

    o_peri_crg54.u32 = HDMITXRegRead(pu32RegAddr);
    o_peri_crg54.bits.vo_sd_clk_sel = uvo_sd_clk_sel;
    HDMITXRegWrite(pu32RegAddr, o_peri_crg54.u32);
    
    return HI_SUCCESS;
}

int REG_HDMI_CRG_vo_sd_clk_sel_Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG54 o_peri_crg54;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG54.u32);
    o_peri_crg54.u32 = HDMITXRegRead(pu32RegAddr);
    return o_peri_crg54.bits.vo_sd_clk_sel;
}

int REG_HDMI_CRG_vo_sd_clk_div_Set(unsigned int uvo_sd_clk_div)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG54 o_peri_crg54;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG54.u32);

    o_peri_crg54.u32 = HDMITXRegRead(pu32RegAddr);
    o_peri_crg54.bits.vo_sd_clk_div = uvo_sd_clk_div;
    HDMITXRegWrite(pu32RegAddr, o_peri_crg54.u32);
    
    return HI_SUCCESS;
}

int REG_HDMI_CRG_vo_sd_clk_div_Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG54 o_peri_crg54;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG54.u32);
    o_peri_crg54.u32 = HDMITXRegRead(pu32RegAddr);
    return o_peri_crg54.bits.vo_sd_clk_div;
}

int REG_HDMI_CRG_vo_hd_clk_sel_Set(unsigned int uvo_hd_clk_sel)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG54 o_peri_crg54;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG54.u32);

    o_peri_crg54.u32 = HDMITXRegRead(pu32RegAddr);
    o_peri_crg54.bits.vo_hd_clk_sel = uvo_hd_clk_sel;
    HDMITXRegWrite(pu32RegAddr, o_peri_crg54.u32);
    
    return HI_SUCCESS;
}

int REG_HDMI_CRG_vo_hd_clk_sel_Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG54 o_peri_crg54;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG54.u32);
    o_peri_crg54.u32 = HDMITXRegRead(pu32RegAddr);
    return o_peri_crg54.bits.vo_hd_clk_sel;
}

int REG_HDMI_CRG_vo_hd_clk_div_Set(unsigned int uvo_hd_clk_div)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG54 o_peri_crg54;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG54.u32);

    o_peri_crg54.u32 = HDMITXRegRead(pu32RegAddr);
    o_peri_crg54.bits.vo_hd_clk_div = uvo_hd_clk_div;
    HDMITXRegWrite(pu32RegAddr, o_peri_crg54.u32);
    
    return HI_SUCCESS;
}

int REG_HDMI_CRG_vo_hd_clk_div_Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG54 o_peri_crg54;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG54.u32);
    o_peri_crg54.u32 = HDMITXRegRead(pu32RegAddr);
    return o_peri_crg54.bits.vo_hd_clk_div;
}

int REG_HDMI_CRG_vdac_ch0_clk_sel_Set(unsigned int uvdac_ch0_clk_sel)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG54 o_peri_crg54;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG54.u32);

    o_peri_crg54.u32 = HDMITXRegRead(pu32RegAddr);
    o_peri_crg54.bits.vdac_ch0_clk_sel = uvdac_ch0_clk_sel;
    HDMITXRegWrite(pu32RegAddr, o_peri_crg54.u32);
    
    return HI_SUCCESS;
}

int REG_HDMI_CRG_vdac_ch0_clk_sel_Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG54 o_peri_crg54;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG54.u32);
    o_peri_crg54.u32 = HDMITXRegRead(pu32RegAddr);
    return o_peri_crg54.bits.vdac_ch0_clk_sel;
}

int REG_HDMI_CRG_hdmi_clk_sel_Set(unsigned int uhdmi_clk_sel)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG54 o_peri_crg54;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG54.u32);

    o_peri_crg54.u32 = HDMITXRegRead(pu32RegAddr);
    o_peri_crg54.bits.hdmi_clk_sel = uhdmi_clk_sel;
    HDMITXRegWrite(pu32RegAddr, o_peri_crg54.u32);
    
    return HI_SUCCESS;
}

int REG_HDMI_CRG_hdmi_clk_sel_Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG54 o_peri_crg54;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG54.u32);
    o_peri_crg54.u32 = HDMITXRegRead(pu32RegAddr);
    return o_peri_crg54.bits.hdmi_clk_sel;
}

int REG_HDMI_CRG_vo_sd_hdmi_clk_sel_Set(unsigned int uvo_sd_hdmi_clk_sel)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG54 o_peri_crg54;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG54.u32);

    o_peri_crg54.u32 = HDMITXRegRead(pu32RegAddr);
    o_peri_crg54.bits.vo_sd_hdmi_clk_sel = uvo_sd_hdmi_clk_sel;
    HDMITXRegWrite(pu32RegAddr, o_peri_crg54.u32);
    
    return HI_SUCCESS;
}

int REG_HDMI_CRG_vo_sd_hdmi_clk_sel_Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG54 o_peri_crg54;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG54.u32);
    o_peri_crg54.u32 = HDMITXRegRead(pu32RegAddr);
    return o_peri_crg54.bits.vo_sd_hdmi_clk_sel;
}

int REG_HDMI_CRG_vdp_clk_sel_Set(unsigned int uvdp_clk_sel)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG54 o_peri_crg54;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG54.u32);

    o_peri_crg54.u32 = HDMITXRegRead(pu32RegAddr);
    o_peri_crg54.bits.vdp_clk_sel = uvdp_clk_sel;
    HDMITXRegWrite(pu32RegAddr, o_peri_crg54.u32);
    
    return HI_SUCCESS;
}

int REG_HDMI_CRG_vdp_clk_sel_Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG54 o_peri_crg54;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG54.u32);
    o_peri_crg54.u32 = HDMITXRegRead(pu32RegAddr);
    return o_peri_crg54.bits.vdp_clk_sel;
}

int REG_HDMI_CRG_vo_hd_hdmi_clk_sel_Set(unsigned int uvo_hd_hdmi_clk_sel)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG54 o_peri_crg54;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG54.u32);

    o_peri_crg54.u32 = HDMITXRegRead(pu32RegAddr);
    o_peri_crg54.bits.vo_hd_hdmi_clk_sel = uvo_hd_hdmi_clk_sel;
    HDMITXRegWrite(pu32RegAddr, o_peri_crg54.u32);
    
    return HI_SUCCESS;
}

int REG_HDMI_CRG_vo_hd_hdmi_clk_sel_Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG54 o_peri_crg54;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG54.u32);
    o_peri_crg54.u32 = HDMITXRegRead(pu32RegAddr);
    return o_peri_crg54.bits.vo_hd_hdmi_clk_sel;
}

int REG_HDMI_CRG_voR_srst_req_Set(unsigned int uvoR_srst_req)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG54 o_peri_crg54;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG54.u32);

    o_peri_crg54.u32 = HDMITXRegRead(pu32RegAddr);
    o_peri_crg54.bits.voR_srst_req = uvoR_srst_req;
    HDMITXRegWrite(pu32RegAddr, o_peri_crg54.u32);
    
    return HI_SUCCESS;
}

int REG_HDMI_CRG_voR_srst_req_Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG54 o_peri_crg54;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG54.u32);
    o_peri_crg54.u32 = HDMITXRegRead(pu32RegAddr);
    return o_peri_crg54.bits.voR_srst_req;
}


int REG_HDMI_CRG_ssc_in_cken_Set(unsigned int ussc_in_cken)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG67 o_peri_crg67;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG67.u32);

    o_peri_crg67.u32 = HDMITXRegRead(pu32RegAddr);
    o_peri_crg67.bits.ssc_in_cken = ussc_in_cken;
    HDMITXRegWrite(pu32RegAddr, o_peri_crg67.u32);
    
    return HI_SUCCESS;
}

int REG_HDMI_CRG_ssc_in_cken_Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG67 o_peri_crg67;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG67.u32);
    o_peri_crg67.u32 = HDMITXRegRead(pu32RegAddr);
    return o_peri_crg67.bits.ssc_in_cken;
}

int REG_HDMI_CRG_ssc_bypass_cken_Set(unsigned int ussc_bypass_cken)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG67 o_peri_crg67;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG67.u32);

    o_peri_crg67.u32 = HDMITXRegRead(pu32RegAddr);
    o_peri_crg67.bits.ssc_bypass_cken = ussc_bypass_cken;
    HDMITXRegWrite(pu32RegAddr, o_peri_crg67.u32);
    
    return HI_SUCCESS;
}

int REG_HDMI_CRG_ssc_bypass_cken_Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG67 o_peri_crg67;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG67.u32);
    o_peri_crg67.u32 = HDMITXRegRead(pu32RegAddr);
    return o_peri_crg67.bits.ssc_bypass_cken;
}

int REG_HDMI_CRG_hdmitx_ctrl_osc_24m_cken_Set(unsigned int uhdmitx_ctrl_osc_24m_cken)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG67 o_peri_crg67;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG67.u32);

    o_peri_crg67.u32 = HDMITXRegRead(pu32RegAddr);
    o_peri_crg67.bits.hdmitx_ctrl_osc_24m_cken = uhdmitx_ctrl_osc_24m_cken;
    HDMITXRegWrite(pu32RegAddr, o_peri_crg67.u32);
    
    return HI_SUCCESS;
}

int REG_HDMI_CRG_hdmitx_ctrl_osc_24m_cken_Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG67 o_peri_crg67;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG67.u32);
    o_peri_crg67.u32 = HDMITXRegRead(pu32RegAddr);
    return o_peri_crg67.bits.hdmitx_ctrl_osc_24m_cken;
}

int REG_HDMI_CRG_hdmitx_ctrl_cec_cken_Set(unsigned int uhdmitx_ctrl_cec_cken)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG67 o_peri_crg67;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG67.u32);

    o_peri_crg67.u32 = HDMITXRegRead(pu32RegAddr);
    o_peri_crg67.bits.hdmitx_ctrl_cec_cken = uhdmitx_ctrl_cec_cken;
    HDMITXRegWrite(pu32RegAddr, o_peri_crg67.u32);
    
    return HI_SUCCESS;
}

int REG_HDMI_CRG_hdmitx_ctrl_cec_cken_Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG67 o_peri_crg67;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG67.u32);
    o_peri_crg67.u32 = HDMITXRegRead(pu32RegAddr);
    return o_peri_crg67.bits.hdmitx_ctrl_cec_cken;
}

int REG_HDMI_CRG_hdmitx_ctrl_os_cken_Set(unsigned int uhdmitx_ctrl_os_cken)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG67 o_peri_crg67;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG67.u32);

    o_peri_crg67.u32 = HDMITXRegRead(pu32RegAddr);
    o_peri_crg67.bits.hdmitx_ctrl_os_cken = uhdmitx_ctrl_os_cken;
    HDMITXRegWrite(pu32RegAddr, o_peri_crg67.u32);
    
    return HI_SUCCESS;
}

int REG_HDMI_CRG_hdmitx_ctrl_os_cken_Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG67 o_peri_crg67;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG67.u32);
    o_peri_crg67.u32 = HDMITXRegRead(pu32RegAddr);
    return o_peri_crg67.bits.hdmitx_ctrl_os_cken;
}

int REG_HDMI_CRG_hdmitx_ctrl_as_cken_Set(unsigned int uhdmitx_ctrl_as_cken)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG67 o_peri_crg67;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG67.u32);

    o_peri_crg67.u32 = HDMITXRegRead(pu32RegAddr);
    o_peri_crg67.bits.hdmitx_ctrl_as_cken = uhdmitx_ctrl_as_cken;
    HDMITXRegWrite(pu32RegAddr, o_peri_crg67.u32);
    
    return HI_SUCCESS;
}

int REG_HDMI_CRG_hdmitx_ctrl_as_cken_Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG67 o_peri_crg67;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG67.u32);
    o_peri_crg67.u32 = HDMITXRegRead(pu32RegAddr);
    return o_peri_crg67.bits.hdmitx_ctrl_as_cken;
}

int REG_HDMI_CRG_hdmitx_ctrl_bus_srst_req_Set(unsigned int uhdmitx_ctrl_bus_srst_req)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG67 o_peri_crg67;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG67.u32);

    o_peri_crg67.u32 = HDMITXRegRead(pu32RegAddr);
    o_peri_crg67.bits.hdmitx_ctrl_bus_srst_req = uhdmitx_ctrl_bus_srst_req;
    HDMITXRegWrite(pu32RegAddr, o_peri_crg67.u32);
    
    return HI_SUCCESS;
}

int REG_HDMI_CRG_hdmitx_ctrl_bus_srst_req_Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG67 o_peri_crg67;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG67.u32);
    o_peri_crg67.u32 = HDMITXRegRead(pu32RegAddr);
    return o_peri_crg67.bits.hdmitx_ctrl_bus_srst_req;
}

int REG_HDMI_CRG_hdmitx_ctrl_srst_req_Set(unsigned int uhdmitx_ctrl_srst_req)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG67 o_peri_crg67;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG67.u32);

    o_peri_crg67.u32 = HDMITXRegRead(pu32RegAddr);
    o_peri_crg67.bits.hdmitx_ctrl_srst_req = uhdmitx_ctrl_srst_req;
    HDMITXRegWrite(pu32RegAddr, o_peri_crg67.u32);
    
    return HI_SUCCESS;
}

int REG_HDMI_CRG_hdmitx_ctrl_srst_req_Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG67 o_peri_crg67;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG67.u32);
    o_peri_crg67.u32 = HDMITXRegRead(pu32RegAddr);
    return o_peri_crg67.bits.hdmitx_ctrl_srst_req;
}

int REG_HDMI_CRG_hdmitx_ctrl_cec_srst_req_Set(unsigned int uhdmitx_ctrl_cec_srst_req)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG67 o_peri_crg67;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG67.u32);

    o_peri_crg67.u32 = HDMITXRegRead(pu32RegAddr);
    o_peri_crg67.bits.hdmitx_ctrl_cec_srst_req = uhdmitx_ctrl_cec_srst_req;
    HDMITXRegWrite(pu32RegAddr, o_peri_crg67.u32);
    
    return HI_SUCCESS;
}

int REG_HDMI_CRG_hdmitx_ctrl_cec_srst_req_Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG67 o_peri_crg67;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG67.u32);
    o_peri_crg67.u32 = HDMITXRegRead(pu32RegAddr);
    return o_peri_crg67.bits.hdmitx_ctrl_cec_srst_req;
}

int REG_HDMI_CRG_hdmitx_ssc_srst_req_Set(unsigned int uhdmitx_ssc_srst_req)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG67 o_peri_crg67;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG67.u32);

    o_peri_crg67.u32 = HDMITXRegRead(pu32RegAddr);
    o_peri_crg67.bits.hdmitx_ssc_srst_req = uhdmitx_ssc_srst_req;
    HDMITXRegWrite(pu32RegAddr, o_peri_crg67.u32);
    
    return HI_SUCCESS;
}

int REG_HDMI_CRG_hdmitx_ssc_srst_req_Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG67 o_peri_crg67;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG67.u32);
    o_peri_crg67.u32 = HDMITXRegRead(pu32RegAddr);
    return o_peri_crg67.bits.hdmitx_ssc_srst_req;
}

int REG_HDMI_CRG_ssc_clk_div_Set(unsigned int ussc_clk_div)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG67 o_peri_crg67;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG67.u32);

    o_peri_crg67.u32 = HDMITXRegRead(pu32RegAddr);
    o_peri_crg67.bits.ssc_clk_div = ussc_clk_div;
    HDMITXRegWrite(pu32RegAddr, o_peri_crg67.u32);
    
    return HI_SUCCESS;
}

int REG_HDMI_CRG_ssc_clk_div_Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG67 o_peri_crg67;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG67.u32);
    o_peri_crg67.u32 = HDMITXRegRead(pu32RegAddr);
    return o_peri_crg67.bits.ssc_clk_div;
}

int REG_HDMI_CRG_hdmitx_ctrl_cec_clk_sel_Set(unsigned int uhdmitx_ctrl_cec_clk_sel)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG67 o_peri_crg67;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG67.u32);

    o_peri_crg67.u32 = HDMITXRegRead(pu32RegAddr);
    o_peri_crg67.bits.hdmitx_ctrl_cec_clk_sel = uhdmitx_ctrl_cec_clk_sel;
    HDMITXRegWrite(pu32RegAddr, o_peri_crg67.u32);
    
    return HI_SUCCESS;
}

int REG_HDMI_CRG_hdmitx_ctrl_cec_clk_sel_Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG67 o_peri_crg67;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG67.u32);
    o_peri_crg67.u32 = HDMITXRegRead(pu32RegAddr);
    return o_peri_crg67.bits.hdmitx_ctrl_cec_clk_sel;
}

int REG_HDMI_CRG_phy_tmds_cken_Set(unsigned int uphy_tmds_cken)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG68 o_peri_crg68;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG68.u32);

    o_peri_crg68.u32 = HDMITXRegRead(pu32RegAddr);
    o_peri_crg68.bits.phy_tmds_cken = uphy_tmds_cken;
    HDMITXRegWrite(pu32RegAddr, o_peri_crg68.u32);
    
    return HI_SUCCESS;
}

int REG_HDMI_CRG_phy_tmds_cken_Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG68 o_peri_crg68;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG68.u32);
    o_peri_crg68.u32 = HDMITXRegRead(pu32RegAddr);
    return o_peri_crg68.bits.phy_tmds_cken;
}

int REG_HDMI_CRG_hdmitx_phy_srst_req_Set(unsigned int uhdmitx_phy_srst_req)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG68 o_peri_crg68;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG68.u32);

    o_peri_crg68.u32 = HDMITXRegRead(pu32RegAddr);
    o_peri_crg68.bits.hdmitx_phy_srst_req = uhdmitx_phy_srst_req;
    HDMITXRegWrite(pu32RegAddr, o_peri_crg68.u32);
    
    return HI_SUCCESS;
}

int REG_HDMI_CRG_hdmitx_phy_srst_req_Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG68 o_peri_crg68;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG68.u32);
    o_peri_crg68.u32 = HDMITXRegRead(pu32RegAddr);
    return o_peri_crg68.bits.hdmitx_phy_srst_req;
}

int REG_HDMI_CRG_phy_tmds_srst_req_Set(unsigned int uphy_tmds_srst_req)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG68 o_peri_crg68;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG68.u32);

    o_peri_crg68.u32 = HDMITXRegRead(pu32RegAddr);
    o_peri_crg68.bits.phy_tmds_srst_req = uphy_tmds_srst_req;
    HDMITXRegWrite(pu32RegAddr, o_peri_crg68.u32);
    
    return HI_SUCCESS;
}

int REG_HDMI_CRG_phy_tmds_srst_req_Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG68 o_peri_crg68;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG68.u32);
    o_peri_crg68.u32 = HDMITXRegRead(pu32RegAddr);
    return o_peri_crg68.bits.phy_tmds_srst_req;
}

int REG_HDMI_CRG_tmds_clk_div_Set(unsigned int utmds_clk_div)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG68 o_peri_crg68;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG68.u32);

    o_peri_crg68.u32 = HDMITXRegRead(pu32RegAddr);
    o_peri_crg68.bits.tmds_clk_div = utmds_clk_div;
    HDMITXRegWrite(pu32RegAddr, o_peri_crg68.u32);
    
    return HI_SUCCESS;
}

int REG_HDMI_CRG_tmds_clk_div_Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    R_PERI_CRG68 o_peri_crg68;

    pu32RegAddr = (HI_U32*)&(s_pstRegCrg->PERI_CRG68.u32);
    o_peri_crg68.u32 = HDMITXRegRead(pu32RegAddr);
    return o_peri_crg68.bits.tmds_clk_div;
}
