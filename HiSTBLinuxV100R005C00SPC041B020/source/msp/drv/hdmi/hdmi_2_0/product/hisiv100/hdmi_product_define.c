/******************************************************************************

  Copyright (C), 2014-2024, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hdmi_product_define.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2015/08/13
  Description   :
  History       :
  Date          : 2015/08/13
  Author        : t00273561
  Modification  :
*******************************************************************************/

#include "hdmi_platform.h"
#ifndef HDMI_BUILD_IN_BOOT
#include "drv_hdmi_platform.h"
#endif
#include "hdmi_product_define.h"
#include "hdmi_reg_crg.h"

#define PRODUCT_NULL_CHK(p) do{\
	if(HI_NULL == p)\
	{HDMI_ERR("%s is null pointer!return fail.\n",#p);\
	return HI_FAILURE;}\
}while(0)

#define PRODUCT_NULL_CHK_NORET(p) do{\
        if(HI_NULL == p)\
        {HDMI_ERR("%s is null pointer!return fail.\n",#p);\
        return ;}\
}while(0)



/* HDMITX IO CONFIG,see Hi3798MV200T SI RFQ.xlsm */
#define HDMI_ADDR_BASE_IO_CFG       0xf8a21000
#define HDMI_ADDR_IO_CFG_HOTPLUG    (HDMI_ADDR_BASE_IO_CFG + 0xb4)
#define HDMI_ADDR_IO_CFG_SDA        (HDMI_ADDR_BASE_IO_CFG + 0xb8)
#define HDMI_ADDR_IO_CFG_SCL        (HDMI_ADDR_BASE_IO_CFG + 0xbc)
#define HDMI_ADDR_IO_CFG_CEC        (HDMI_ADDR_BASE_IO_CFG + 0xc0)

#define HDMI_IO_CFG_FUNCTION1_SEL       0x1



HI_S32 HDMITXRegWrite  (HI_U32 *pu32RegAddr, HI_U32 u32Value)
{
    PRODUCT_NULL_CHK(pu32RegAddr);
    *(volatile HI_U32*)pu32RegAddr = u32Value;
    return HI_SUCCESS;
}


HI_U32 HDMITXRegRead(HI_U32 *pu32RegAddr)
{
    PRODUCT_NULL_CHK(pu32RegAddr);
    return *(volatile HI_U32*)(pu32RegAddr);
}

HI_S32 HDMITXVerifyRegWrite(HI_U32 *pu32RegAddr, HI_U32 u32RegValue, HI_CHAR *pRegName)
{
#if 0
    HI_BOOL bWriteSuccess = HI_FALSE;

    PRODUCT_NULL_CHK(pu32RegAddr);
    bWriteSuccess = (*pu32RegAddr == u32RegValue);
    if (!bWriteSuccess)
    {
        HDMI_ERR("virsual addr=0x%p,re=0x%08x,value=0x%08x,name= %s , error!\n",  pu32RegAddr,*pu32RegAddr,u32RegValue,pRegName);
    }

    return !bWriteSuccess;
#endif
    return 0;
}

HI_S32 HDMIRegWriteU32(HI_U32 u32RegAddr, HI_U32 u32Value)
{
    volatile HI_U32 *pu32RegAddr = HI_NULL;

    pu32RegAddr = (volatile HI_U32 *)HDMI_IO_MAP(u32RegAddr,4) ;

    if (pu32RegAddr)
    {
        *pu32RegAddr = u32Value ;
        HDMI_IO_UNMAP(pu32RegAddr);
        return HI_SUCCESS;
    }
    else
    {
        HDMI_ERR("HDMI_IO_MAP addr=0x%x err!\n",u32RegAddr);
        return HI_FAILURE;
    }


}


HI_U32 HDMIRegReadU32 (HI_U32 u32RegAddr)
{
    volatile HI_U32 *pu32RegAddr = HI_NULL;
	HI_U32  u32Value = 0;

    pu32RegAddr = (volatile HI_U32 *)HDMI_IO_MAP(u32RegAddr,4);

    if (pu32RegAddr)
    {
        u32Value = *pu32RegAddr ;
        HDMI_IO_UNMAP(pu32RegAddr);
    }
    else
    {
        HDMI_ERR("HDMI_IO_MAP addr=0x%x\n err!\n",u32RegAddr);
    }

	return  u32Value;
}


//==========================================================================

HI_VOID DRV_HDMI_ProdIoCfgSet(HI_VOID)
{
#ifndef HDMI_FPGA_SUPPORT
    HI_U32  u32TmpRegValue = 0;

    u32TmpRegValue = HDMIRegReadU32(HDMI_ADDR_IO_CFG_HOTPLUG);
    u32TmpRegValue |= HDMI_IO_CFG_FUNCTION1_SEL;
    HDMIRegWriteU32(HDMI_ADDR_IO_CFG_HOTPLUG,u32TmpRegValue);

    u32TmpRegValue = HDMIRegReadU32(HDMI_ADDR_IO_CFG_SDA);
    u32TmpRegValue |= HDMI_IO_CFG_FUNCTION1_SEL;
    HDMIRegWriteU32(HDMI_ADDR_IO_CFG_SDA    ,u32TmpRegValue);

    u32TmpRegValue = HDMIRegReadU32(HDMI_ADDR_IO_CFG_SCL);
    u32TmpRegValue |= HDMI_IO_CFG_FUNCTION1_SEL;
    HDMIRegWriteU32(HDMI_ADDR_IO_CFG_SCL    ,u32TmpRegValue);

    u32TmpRegValue = HDMIRegReadU32(HDMI_ADDR_IO_CFG_CEC);
    u32TmpRegValue |= HDMI_IO_CFG_FUNCTION1_SEL;
    HDMIRegWriteU32(HDMI_ADDR_IO_CFG_CEC    ,u32TmpRegValue);
#endif
    return ;
}

HI_VOID DRV_HDMI_ProdCrgGateSet(HI_BOOL bEnable)
{
#ifndef HDMI_FPGA_SUPPORT
    /* Gate */
    /* PERI_CRG67  HDMITX_CTRL CRG  0x010C   0x0000043F */
    REG_HDMI_CRG_ssc_in_cken_Set(bEnable);
    REG_HDMI_CRG_ssc_bypass_cken_Set(bEnable);
    REG_HDMI_CRG_hdmitx_ctrl_osc_24m_cken_Set(bEnable);
    REG_HDMI_CRG_hdmitx_ctrl_cec_cken_Set(bEnable);
    REG_HDMI_CRG_hdmitx_ctrl_os_cken_Set(bEnable);
    REG_HDMI_CRG_hdmitx_ctrl_as_cken_Set(bEnable);
    /* PERI_CRG68  HDMITX_PHY      0x0110  0x00000001 */
    REG_HDMI_CRG_phy_tmds_cken_Set(bEnable);

    /* CLK Sel */
    /* PERI_CRG67  HDMITX_CTRL CRG  0x010C   0x0000043F */
    REG_HDMI_CRG_hdmitx_ctrl_cec_clk_sel_Set(HI_FALSE);
#endif
    return ;
}

HI_VOID DRV_HDMI_ProdCrgAllResetSet(HI_BOOL bEnable)
{
#ifndef HDMI_FPGA_SUPPORT
    /* PERI_CRG67  HDMITX_CTRL CRG  0x010C   0x0000043F */
    REG_HDMI_CRG_hdmitx_ctrl_bus_srst_req_Set(bEnable);
    REG_HDMI_CRG_hdmitx_ctrl_srst_req_Set(bEnable);
    REG_HDMI_CRG_hdmitx_ctrl_cec_srst_req_Set(bEnable);
    REG_HDMI_CRG_hdmitx_ssc_srst_req_Set(bEnable);
    /* PERI_CRG68  HDMITX_PHY      0x0110  0x00000001 */
    REG_HDMI_CRG_hdmitx_phy_srst_req_Set(bEnable);
    REG_HDMI_CRG_phy_tmds_srst_req_Set(bEnable);

    bEnable = !bEnable;

    /* PERI_CRG67  HDMITX_CTRL CRG  0x010C   0x0000043F */
    REG_HDMI_CRG_hdmitx_ctrl_bus_srst_req_Set(bEnable);
    REG_HDMI_CRG_hdmitx_ctrl_srst_req_Set(bEnable);
    REG_HDMI_CRG_hdmitx_ctrl_cec_srst_req_Set(bEnable);
    REG_HDMI_CRG_hdmitx_ssc_srst_req_Set(bEnable);
    /* PERI_CRG68  HDMITX_PHY      0x0110  0x00000001 */
    REG_HDMI_CRG_hdmitx_phy_srst_req_Set(bEnable);
    REG_HDMI_CRG_phy_tmds_srst_req_Set(bEnable);
#endif
    return ;
}

HI_VOID DRV_HDMI_ProdCrgPhyResetSet(HI_BOOL bEnable)
{
#ifndef HDMI_FPGA_SUPPORT
    /* PERI_CRG68  HDMITX_PHY      0x0110  0x00000001 */
    REG_HDMI_CRG_hdmitx_phy_srst_req_Set(bEnable);
    REG_HDMI_CRG_phy_tmds_srst_req_Set(bEnable);

    bEnable = !bEnable;

    /* PERI_CRG68  HDMITX_PHY      0x0110  0x00000001 */
    REG_HDMI_CRG_hdmitx_phy_srst_req_Set(bEnable);
    REG_HDMI_CRG_phy_tmds_srst_req_Set(bEnable);
#endif
    return ;
}

HI_VOID DRV_HDMI_ProdCrgPhyResetGet(HI_BOOL *pbEnable)
{
#ifndef HDMI_FPGA_SUPPORT
    PRODUCT_NULL_CHK_NORET(pbEnable);

    /* PERI_CRG68  HDMITX_PHY      0x0110  0x00000001 */
    if(REG_HDMI_CRG_hdmitx_phy_srst_req_Get() \
        || REG_HDMI_CRG_phy_tmds_srst_req_Get() )
    {
        *pbEnable = HI_TRUE;
    }
    else
    {
        *pbEnable = HI_FALSE;
    }

#endif
    return ;
}

HI_VOID DRV_HDMI_ProdCrgDivSet(HDMI_CRG_CFG_S *pstCrgCfg)
{
#ifndef HDMI_FPGA_SUPPORT
    HI_U32 u32Tmp   = 0;

    if(pstCrgCfg)
    {
        u32Tmp      = pstCrgCfg->u32SscBypassDiv - 1;
        REG_HDMI_CRG_ssc_clk_div_Set(u32Tmp);
        u32Tmp      = pstCrgCfg->u32TmdsClkDiv - 1;
        REG_HDMI_CRG_tmds_clk_div_Set(u32Tmp);
        REG_HDMI_CRG_hdmitx_phy_srst_req_Set(HI_TRUE);
        REG_HDMI_CRG_hdmitx_phy_srst_req_Set(HI_FALSE);
    }
#endif
    return ;
}

HI_VOID DRV_HDMI_ProdCrgInit(HI_VOID)
{
#ifndef HDMI_FPGA_SUPPORT

    DRV_HDMI_ProdIoCfgSet();

    DRV_HDMI_ProdCrgGateSet(HI_TRUE);
    DRV_HDMI_ProdCrgAllResetSet(HI_TRUE);

#endif
    return ;
}


HI_VOID DRV_HDMI_ProdCrgDeInit(HI_VOID)
{
#ifndef HDMI_FPGA_SUPPORT

#endif
    return ;
}


