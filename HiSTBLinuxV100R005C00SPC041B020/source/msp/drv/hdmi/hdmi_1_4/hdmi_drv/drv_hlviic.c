/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#include <linux/delay.h>
#include "si_config.h"
#include "si_typedefs.h"
#include "si_hlviic.h"
#include "hi_type.h"
//#include "mpi_priv_hdmi.h"
#include "hi_drv_hdmi.h"
#include "si_defstx.h"
#include "si_hdmitx.h"
#include "si_timer.h"
#include "hi_drv_sys.h"
#include "si_phy.h"
#include "hi_reg_common.h"
#include "drv_reg_proc.h"

static int g_ulHdmiFileHandle = -1;

//-------------------------------------------------------------------
HI_U8 ReadByte_8BA(HI_U8 Bus, HI_U8 SlaveAddr, HI_U8 RegAddr)
{
    HI_U8 Data = 0x00;
    Data = DRV_ReadByte_8BA(Bus, SlaveAddr, RegAddr);

    return Data;
}

//-------------------------------------------------------------------
void WriteByte_8BA(HI_U8 Bus, HI_U8 SlaveAddr, HI_U8 RegAddr, HI_U8 Data)
{
    DRV_WriteByte_8BA(Bus, SlaveAddr, RegAddr, Data);
    //udelay(1);  //very importance!!
}

//--------------------------------------------------------------------

HI_U16 ReadWord_8BA(HI_U8 Bus, HI_U8 SlaveAddr, HI_U8 RegAddr)
{
    HI_U16 Data = 0x00;
    Data = DRV_ReadWord_8BA(Bus, SlaveAddr, RegAddr);

    return Data;
}

//-------------------------------------------------------------------

void WriteWord_8BA(HI_U8 Bus, HI_U8 SlaveAddr, HI_U8 RegAddr, HI_U16 Data)
{
    DRV_WriteWord_8BA(Bus, SlaveAddr, RegAddr, Data);
    //udelay(1);  //very importance!!
    if (SlaveAddr==TX_SLV1)
    {
       // udelay(1);
    }
}

//------------------------------------------------------------------------------------
HI_U8 BlockRead_8BAS(I2CShortCommandType * I2CCommand, HI_U8 * Data)
{
	HI_U32 ulRet = HI_FAILURE;

    ulRet = DRV_BlockRead_8BAS(I2CCommand, Data);

    return ulRet;
}

//-------------------------------------------------------------------------------------

HI_U8 BlockWrite_8BAS( I2CShortCommandType * I2CCommand, HI_U8 * Data )
{
	HI_U32 ulRet = HI_FAILURE;

    ulRet = DRV_BlockWrite_8BAS(I2CCommand, Data);

    return ulRet;
}

//-------------------------------------------------------------------
HI_U8 SI_Region2_ReadByte(HI_U8 Bus, HI_U8 SlaveAddr, HI_U8 RegAddr)
{
    HI_U8 Data = 0xff;

	DRV_WriteByte_8BA(Bus, TX_SLV0, 0XFF, 0x81);
    Data = DRV_ReadByte_8BA(Bus, SlaveAddr, RegAddr);
	DRV_WriteByte_8BA(Bus, TX_SLV0, 0XFF, 0x80);

    return Data;
}

//-------------------------------------------------------------------
void SI_Region2_WriteByte(HI_U8 Bus, HI_U8 SlaveAddr, HI_U8 RegAddr, HI_U8 Data)
{
    DRV_WriteByte_8BA(Bus, TX_SLV0, 0XFF, 0x81);
    DRV_WriteByte_8BA(Bus, SlaveAddr, RegAddr, Data);
    DRV_WriteByte_8BA(Bus, TX_SLV0, 0XFF, 0x80);
}

HI_U32 SI_OpenHdmiDevice(void)
{
    return HI_SUCCESS;
}

void SI_DisableHdmiDevice(void)
{
    SI_TX_PHY_DisableHdmiOutput();
    return;
}

void SI_EnableHdmiDevice(void)
{
    SI_TX_PHY_EnableHdmiOutput();
    return;
}

extern HI_U8 OldOutputState;

void SI_CloseHdmiDevice(void)
{
    HI_U8 regvalue;
    HI_U32 Count = 3;

    while(Count --)
    {
        if (Count <= 0)
        {
            COM_ERR("timeout is occur\n");
            break;
        }
        regvalue = ReadByteHDMITXP0(0xF2);//MDDC_STATUS_ADDR
        COM_INFO("MDDC_STATUS_ADDR regvalue:0x%x\n", regvalue);
        if((regvalue & 0x10) != 0)
        {
            COM_INFO("loop untill DDC to idle status regvalue:0x%x\n", regvalue);
            msleep(10);
            continue;
        }
        break;
    }

    SI_SetHDMIOldOutputStatus(0xFF);
    SI_SetHDMIOutputStatus(CABLE_UNPLUG_);
    
    g_ulHdmiFileHandle = -1;
    
    return;
}


void SI_PoweDownHdmiDevice(void)
{
    //HI_U32 Ret;
    U_PERI_CRG67 PeriCrg67;
    U_PERI_CRG68 PeriCrg68;

     /* powerdown HDMI PHY Output:TMDS CNTL3 Register:Powerdown Control */
    //Ret = DRV_HDMI_WriteRegister((HI_U32)0x10171808, (HI_U32)0x00);      
    SI_TX_PHY_PowerDown(HI_TRUE);
    //SI_TX_PHY_WriteRegister(0x02,0x00);
    //SI_PowerDownHdmiTx();
    WriteByteHDMITXP0(TX_SYS_CTRL1_ADDR, 0x00); //Most other register values are not affected by assertion of the PD# bit.
    /* powedown HDMI Controller:Diagnostic Power Down Register:(0x7A,0x3D) */
    WriteByteHDMITXP1(DIAG_PD_ADDR, 0x00);  //Power down everything; INT source is RSEN

#if    defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)   \
    || defined(CHIP_TYPE_hi3718mv100)   \
    || defined(CHIP_TYPE_hi3719mv100)   \
    || defined(CHIP_TYPE_hi3796cv100)   \
    || defined(CHIP_TYPE_hi3798cv100)


    PeriCrg67.u32 = g_pstRegCrg->PERI_CRG67.u32;

    PeriCrg67.bits.hdmitx_ctrl_bus_cken     = 0;
    PeriCrg67.bits.hdmitx_ctrl_cec_cken     = 0;
    PeriCrg67.bits.hdmitx_ctrl_id_cken      = 0;
    PeriCrg67.bits.hdmitx_ctrl_mhl_cken     = 0;
    PeriCrg67.bits.hdmitx_ctrl_os_cken      = 0;
    PeriCrg67.bits.hdmitx_ctrl_as_cken      = 0;
    PeriCrg67.bits.hdmitx_ctrl_bus_srst_req = 1;
    PeriCrg67.bits.hdmitx_ctrl_srst_req     = 1;
    PeriCrg67.bits.hdmitx_ctrl_cec_clk_sel  = 0;
    PeriCrg67.bits.hdmitx_ctrl_osclk_sel    = 0;
    PeriCrg67.bits.hdmitx_ctrl_asclk_sel    = 0;

    g_pstRegCrg->PERI_CRG67.u32 = PeriCrg67.u32;


#elif  defined(CHIP_TYPE_hi3798mv100) \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100) \
    || defined(CHIP_TYPE_hi3716mv420) \
    || defined(CHIP_TYPE_hi3716mv410)

    PeriCrg67.u32 = g_pstRegCrg->PERI_CRG67.u32;

    PeriCrg67.bits.hdmitx_ctrl_bus_cken     = 0;
    PeriCrg67.bits.hdmitx_ctrl_cec_cken     = 0;
    PeriCrg67.bits.hdmitx_ctrl_id_cken      = 0;
    PeriCrg67.bits.hdmitx_ctrl_mhl_cken     = 0;
    PeriCrg67.bits.hdmitx_ctrl_os_cken      = 0;
    PeriCrg67.bits.hdmitx_ctrl_as_cken      = 0;
    PeriCrg67.bits.hdmitx_ctrl_bus_srst_req = 1;
    PeriCrg67.bits.hdmitx_ctrl_srst_req     = 1;
    PeriCrg67.bits.hdmitx_ctrl_cec_clk_sel  = 0;
    /* In Taiyan Phy,Phy can not supply Osc clock.
      so crg force supply it,and no osc select in crg. */
    //PeriCrg67.bits.hdmitx_ctrl_osclk_sel    = 0;

    /* when intergrated Silicon series phy,
        hdmitx_ctrl_asclk_sel = 0 means from phy,1 means from crg.
       but intergrate Taiyan phy,
        hdmitx_ctrl_asclk_sel = 0 means from crg,1 means from phy.
    */
    PeriCrg67.bits.hdmitx_ctrl_asclk_sel    = 0;

    g_pstRegCrg->PERI_CRG67.u32 = PeriCrg67.u32;


#endif

    PeriCrg68.u32 = g_pstRegCrg->PERI_CRG68.u32;

    PeriCrg68.bits.hdmitx_phy_bus_cken = 0;
    PeriCrg68.bits.hdmitx_phy_srst_req = 1;

    g_pstRegCrg->PERI_CRG68.u32 = PeriCrg68.u32;

    return;
}

