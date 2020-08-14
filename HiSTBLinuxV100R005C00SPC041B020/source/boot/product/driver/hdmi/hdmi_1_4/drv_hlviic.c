/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/

//#include "linux/string.h"
//#include "typedefs.h"
#include "hlviic.h"

//#include "hi_type.h"
//#include "config.h"
#include "defstx.h"
#include "hdmitx.h"
//#include "mpi_hdmi.h"
#include "hi_common.h"
#include "hi_reg_common.h"
#include "drv_reg_proc.h"


#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
#define HDMI_TX_SLV0_ADDR_BASE   0x10170000L
#define HDMI_TX_SLV1_ADDR_BASE   0x10170400L
#define HDMI_HARDWARE_RESET_ADDR 0x101f5048L
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/

//-------------------------------------------------------------------
HI_U8 ReadByte_8BA(HI_U8 Bus, HI_U8 SlaveAddr, HI_U8 RegAddr)
{
    HI_U8 Data = 0xff;

    if (TX_SLV0 == SlaveAddr)
    {
        Data = HDMI_REG_READ(HDMI_TX_SLV0_ADDR_BASE + RegAddr * 4);
    }
    else if(TX_SLV1 == SlaveAddr)
    {
        Data = HDMI_REG_READ(HDMI_TX_SLV1_ADDR_BASE + RegAddr * 4);
    }
    else
    {
        Data = HDMI_REG_READ(HDMI_TX_CEC_ADDR_BASE + RegAddr * 4);
    }

    return Data;
}

//-------------------------------------------------------------------
void WriteByte_8BA(HI_U8 Bus, HI_U8 SlaveAddr, HI_U8 RegAddr, HI_U8 Data)
{
    if (TX_SLV0 == SlaveAddr)
    {
        HDMI_REG_WRITE((HDMI_TX_SLV0_ADDR_BASE + RegAddr * 4), Data);
    }
    else if(TX_SLV1 == SlaveAddr)
    {
        HDMI_REG_WRITE((HDMI_TX_SLV1_ADDR_BASE + RegAddr * 4), Data);
    }
    else
    {
        HDMI_REG_WRITE((HDMI_TX_CEC_ADDR_BASE + RegAddr * 4), Data);
    }
    return;
}

//--------------------------------------------------------------------

HI_U16 ReadWord_8BA(HI_U8 Bus, HI_U8 SlaveAddr, HI_U8 RegAddr)
{
    HI_U16 Data = 0xffff;
    volatile HI_U32     *pu32VirAddr = HI_NULL;

    if(SlaveAddr==TX_SLV0)
    {
        pu32VirAddr = (volatile HI_U32 *)(HDMI_TX_SLV0_ADDR_BASE + RegAddr * 4);
        Data = (HI_U16)((pu32VirAddr[0]&0xff) | ((pu32VirAddr[1]&0xff) << 8));
    }
    else if(SlaveAddr==TX_SLV1)
    {
        pu32VirAddr = (volatile HI_U32 *)(HDMI_TX_SLV1_ADDR_BASE + RegAddr * 4);
        Data = (HI_U16)((pu32VirAddr[0]&0xff) | ((pu32VirAddr[1]&0xff) << 8));
    }
    return Data;
}

//-------------------------------------------------------------------

void WriteWord_8BA(HI_U8 Bus, HI_U8 SlaveAddr, HI_U8 RegAddr, HI_U16 Data)
{
    volatile HI_U32     *pu32VirAddr = HI_NULL;

    if(SlaveAddr==TX_SLV0)
    {
        pu32VirAddr = (volatile HI_U32 *)(HDMI_TX_SLV0_ADDR_BASE + RegAddr * 4);
        pu32VirAddr[0] = Data&0xff;
        pu32VirAddr[1] = (Data&0xff00) >> 8;
    }
    else if(SlaveAddr==TX_SLV1)
    {
        pu32VirAddr = (volatile HI_U32 *)(HDMI_TX_SLV1_ADDR_BASE + RegAddr * 4);
        pu32VirAddr[0] = Data&0xff;
        pu32VirAddr[1] = (Data&0xff00) >> 8;
    }
}


#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
//------------------------------------------------------------------------------------
void HdmiHardwareReset(int iEnable)
{
    U_PERI_CRG67    crg67;
    U_PERI_CRG68    crg68;

    if (iEnable == 0)
    {
        crg67.u32 = g_pstRegCrg->PERI_CRG67.u32;
        crg67.bits.hdmitx_ctrl_bus_cken     = 1;
        crg67.bits.hdmitx_ctrl_cec_cken     = 1;
        crg67.bits.hdmitx_ctrl_id_cken      = 1;
        crg67.bits.hdmitx_ctrl_mhl_cken     = 1;
        crg67.bits.hdmitx_ctrl_os_cken      = 1;
        crg67.bits.hdmitx_ctrl_as_cken      = 1;
        crg67.bits.hdmitx_ctrl_bus_srst_req = 0;
        crg67.bits.hdmitx_ctrl_srst_req     = 0;
        crg67.bits.hdmitx_ctrl_cec_clk_sel  = 0;
        crg67.bits.hdmitx_ctrl_osclk_sel    = 0;
        crg67.bits.hdmitx_ctrl_asclk_sel    = 0;
        g_pstRegCrg->PERI_CRG67.u32 = crg67.u32;
    }
    else
    {
        crg67.u32 = g_pstRegCrg->PERI_CRG67.u32;
        crg67.bits.hdmitx_ctrl_bus_cken     = 1;
        crg67.bits.hdmitx_ctrl_cec_cken     = 1;
        crg67.bits.hdmitx_ctrl_id_cken      = 1;
        crg67.bits.hdmitx_ctrl_mhl_cken     = 1;
        crg67.bits.hdmitx_ctrl_os_cken      = 1;
        crg67.bits.hdmitx_ctrl_as_cken      = 1;
        crg67.bits.hdmitx_ctrl_bus_srst_req = 1;
        crg67.bits.hdmitx_ctrl_srst_req     = 1;
        crg67.bits.hdmitx_ctrl_cec_clk_sel  = 0;
        crg67.bits.hdmitx_ctrl_osclk_sel    = 0;
        crg67.bits.hdmitx_ctrl_asclk_sel    = 0;
        g_pstRegCrg->PERI_CRG67.u32 = crg67.u32;
    }

    if (iEnable == 0)
    {
        crg68.u32 = g_pstRegCrg->PERI_CRG68.u32;
        crg68.bits.hdmitx_phy_bus_cken  = 1;
        crg68.bits.hdmitx_phy_srst_req  = 0;
        g_pstRegCrg->PERI_CRG68.u32 = crg68.u32;
    }
    else
    {
        crg68.u32 = g_pstRegCrg->PERI_CRG68.u32;
        crg68.bits.hdmitx_phy_bus_cken  = 0;
        crg68.bits.hdmitx_phy_srst_req  = 1;
        g_pstRegCrg->PERI_CRG68.u32 = crg68.u32;
    }
    return;
}
#endif
