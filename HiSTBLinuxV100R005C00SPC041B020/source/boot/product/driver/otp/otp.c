/******************************************************************************

Copyright (C), 2005-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : otp.c
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2013-08-28
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#include "hi_reg_common.h"
#if defined (CHIP_TYPE_hi3716mv310)
#include "hi_boot_common.h"
#else
#include "hi_common.h"
#endif
#include "command.h"
#include "otp.h"

#ifndef HI_MINIBOOT_SUPPORT
#include "common.h"
#include <linux/string.h>
#else
#include "delay.h"
#endif

HI_BOOL g_bOTPInit = HI_FALSE;

static HI_U32 DRV_OTP_GetOTPTimeValue(HI_VOID)
{
	HI_U32 u32Value = 0;

    u32Value = 0x34f;		//OTP_FREQ_28_8M

	return u32Value;
}

static HI_VOID otp_wait(int usec)
{
    udelay(usec);
}

#define OTP_V200_INTERNAL_WRITE_STATUS_ADDR    (0X17)
#define OTP_V200_INTERNAL_WRITE_ERROR_VALUE    (0X80)

static HI_U32 HAL_OTP_V200_Read(HI_U32 addr)
{
    OTP_V200_CTRL_STATUS_U CtrlStaut;
    OTP_V200_CHANNEL_SEL_U ChannelSel;
    OTP_V200_CPU_RW_CTRL_U CPURWCtrl;
    OTP_V200_MODE_U        Mode;
    OTP_V200_RADDR_U       RAddr;
    OTP_V200_RDATA_U       Redata;
    HI_U32 u32OTPTimeValue = 0;

    CtrlStaut.u32 = otp_read_reg(OTP_V200_CTRL_STATUS);
    //Check CTRL_STATUS ctrl_ready to 0x01
    while(CtrlStaut.bits.ctrl_ready != 1)
    {
        otp_wait(1);
        CtrlStaut.u32 = otp_read_reg(OTP_V200_CTRL_STATUS);
    }
    //Set OTP_V200_CHANNEL_SEL bit channel_sel to 2'b10
    ChannelSel.u32 = otp_read_reg(OTP_V200_CHANNEL_SEL);
    ChannelSel.bits.channel_sel = 0x02;
    otp_write_reg(OTP_V200_CHANNEL_SEL, ChannelSel.u32);
    //Set wr_sel to 0, set rd_enable to 1, read can only do by word
    CPURWCtrl.u32 = otp_read_reg(OTP_V200_CPU_RW_CTRL);
    CPURWCtrl.bits.wr_sel = 0x00;
    CPURWCtrl.bits.rd_enable = 0x01;
    CPURWCtrl.bits.cpu_size = 0x02;
    otp_write_reg(OTP_V200_CPU_RW_CTRL, CPURWCtrl.u32);

    //Set OTP_V200_MODE
    Mode.u32 = otp_read_reg(OTP_V200_MODE);
    u32OTPTimeValue = DRV_OTP_GetOTPTimeValue();
    Mode.u32 |= u32OTPTimeValue;
    otp_write_reg(OTP_V200_MODE, Mode.u32);
    //Set OTP_V200_RADDR
    RAddr.u32 = 0;
    RAddr.bits.raddr = addr;
    otp_write_reg(OTP_V200_RADDR, RAddr.u32);
    //Check CTRL_STATUS bit ctrl_ready to 0x01
    CtrlStaut.u32 = otp_read_reg(OTP_V200_CTRL_STATUS);
    while(CtrlStaut.bits.ctrl_ready != 1)
    {
        otp_wait(1);
        CtrlStaut.u32 = otp_read_reg(OTP_V200_CTRL_STATUS);
    }
    Redata.u32 = otp_read_reg(OTP_V200_RDATA);

    return Redata.u32;
}


HI_U8 HAL_OTP_V200_ReadByte(HI_U32 addr)
{
    HI_U32 Value = 0;
    HI_U32 readableAddr = 0;

    readableAddr = addr & (~0x3);
    Value = HAL_OTP_V200_Read(readableAddr);
    return (Value >> ((addr & 0x3)*8)) & 0xff ;
//    return (HI_U8)(Value & 0xff);
}

//Only used internally, No lock! Write to a special address 0x17[7]
static HI_S32 write_fail_flag(HI_VOID)
{
    OTP_V200_CTRL_STATUS_U CtrlStaut;
    OTP_V200_CHANNEL_SEL_U ChannelSel;
    OTP_V200_CPU_RW_CTRL_U CPURWCtrl;
    OTP_V200_MODE_U        Mode;
    OTP_V200_WADDR_U       WAddr;
    OTP_V200_WDATA_U       WDATA;
    OTP_V200_WR_START_U    WRStart;
    HI_U32 u32OTPTimeValue = 0;

    HI_U32 addr  = OTP_V200_INTERNAL_WRITE_STATUS_ADDR;
    HI_U32 tdata = OTP_V200_INTERNAL_WRITE_ERROR_VALUE;

    CtrlStaut.u32 = otp_read_reg(OTP_V200_CTRL_STATUS);
    //Check CTRL_STATUS ctrl_ready to 0x01
    while(CtrlStaut.bits.ctrl_ready != 1)
    {
        otp_wait(1);
        CtrlStaut.u32 = otp_read_reg(OTP_V200_CTRL_STATUS);
    }
    //Set OTP_V200_CHANNEL_SEL bit channel_sel to 2'b10
    ChannelSel.u32 = otp_read_reg(OTP_V200_CHANNEL_SEL);
    ChannelSel.bits.channel_sel = 0x02;
    otp_write_reg(OTP_V200_CHANNEL_SEL, ChannelSel.u32);
    //Set wr_sel to 1, set wr_enable to 1, set cpu_size 2'b01(Byte operation)
    CPURWCtrl.u32 = otp_read_reg(OTP_V200_CPU_RW_CTRL);
    CPURWCtrl.bits.wr_sel = 0x01;
    CPURWCtrl.bits.wr_enable = 0x01;
    CPURWCtrl.bits.cpu_size = 0x01;
    otp_write_reg(OTP_V200_CPU_RW_CTRL, CPURWCtrl.u32);
    //Set OTP_V200_MODE
    Mode.u32 = otp_read_reg(OTP_V200_MODE);
    u32OTPTimeValue = DRV_OTP_GetOTPTimeValue();
    Mode.u32 |= u32OTPTimeValue;
    otp_write_reg(OTP_V200_MODE, Mode.u32);
    //Set OTP_V200_WADDR
    WAddr.u32 = 0;
    WAddr.bits.waddr = addr;
    otp_write_reg(OTP_V200_WADDR, WAddr.u32);
    //Set OTP_V200_WDATA
    WDATA.bits.wdata = (HI_U32)tdata;
    otp_write_reg(OTP_V200_WDATA, WDATA.u32);
    //Set WR_START bit start to 1
    WRStart.u32 = otp_read_reg(OTP_V200_WR_START);
    WRStart.bits.start = 1;
    otp_write_reg(OTP_V200_WR_START, WRStart.u32);

    //Check CTRL_STATUS bit ctrl_ready to 0x01
    CtrlStaut.u32 = otp_read_reg(OTP_V200_CTRL_STATUS);
    while(CtrlStaut.bits.ctrl_ready != 1)
    {
        otp_wait(1);
        CtrlStaut.u32 = otp_read_reg(OTP_V200_CTRL_STATUS);
    }

    return HI_SUCCESS;
}

HI_S32  HAL_OTP_V200_Write(HI_U32 addr, HI_U32 tdata)
{
    volatile OTP_V200_CTRL_STATUS_U CtrlStaut;
    OTP_V200_CHANNEL_SEL_U ChannelSel;
    OTP_V200_CPU_RW_CTRL_U CPURWCtrl;
    OTP_V200_MODE_U        Mode;
    OTP_V200_WADDR_U       WAddr;
    OTP_V200_WDATA_U       WDATA;
    OTP_V200_WR_START_U    WRStart;
	HI_U32 u32OTPTimeValue = 0;

    /* check if addr is 4bytes aligned or not */
    if( 0 != (addr & 0x3))
    {
    	return HI_FAILURE;
    }

    CtrlStaut.u32 = otp_read_reg(OTP_V200_CTRL_STATUS);
    //Check CTRL_STATUS ctrl_ready to 0x01
    while(CtrlStaut.bits.ctrl_ready != 1)
    {
        otp_wait(1);
        CtrlStaut.u32 = otp_read_reg(OTP_V200_CTRL_STATUS);
    }
    //Set OTP_V200_CHANNEL_SEL bit channel_sel to 2'b10
    ChannelSel.u32 = otp_read_reg(OTP_V200_CHANNEL_SEL);
    ChannelSel.bits.channel_sel = 0x02;
    otp_write_reg(OTP_V200_CHANNEL_SEL, ChannelSel.u32);
    //Set wr_sel to 1, set wr_enable to 1, set cpu_size to 2'b10(Word Operation)
    CPURWCtrl.u32 = otp_read_reg(OTP_V200_CPU_RW_CTRL);
    CPURWCtrl.bits.wr_sel = 0x01;
    CPURWCtrl.bits.wr_enable = 0x01;
    CPURWCtrl.bits.cpu_size = 0x02;
    otp_write_reg(OTP_V200_CPU_RW_CTRL, CPURWCtrl.u32);
    //Set OTP_V200_MODE
    Mode.u32 = otp_read_reg(OTP_V200_MODE);

	u32OTPTimeValue = DRV_OTP_GetOTPTimeValue();
    Mode.u32 |= u32OTPTimeValue;

    otp_write_reg(OTP_V200_MODE, Mode.u32);
    //Set OTP_V200_WADDR
    WAddr.u32 = 0;
    WAddr.bits.waddr = addr;
    otp_write_reg(OTP_V200_WADDR, WAddr.u32);
    //Set OTP_V200_WDATA
    WDATA.bits.wdata = tdata;
    otp_write_reg(OTP_V200_WDATA, WDATA.u32);
    //Set WR_START bit start to 1
    WRStart.u32 = otp_read_reg(OTP_V200_WR_START);
    WRStart.bits.start = 1;
    otp_write_reg(OTP_V200_WR_START, WRStart.u32);

    //Check CTRL_STATUS bit ctrl_ready to 0x01
    CtrlStaut.u32 = otp_read_reg(OTP_V200_CTRL_STATUS);
    while(CtrlStaut.bits.ctrl_ready != 1)
    {
        otp_wait(1);
        CtrlStaut.u32 = otp_read_reg(OTP_V200_CTRL_STATUS);
    }

    if (0 != CtrlStaut.bits.fail_flag)
    {
        write_fail_flag();
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}


static HI_S32 HAL_OTP_V200_WriteByte(HI_U32 addr, HI_U8 tdata)
{
    OTP_V200_CTRL_STATUS_U CtrlStaut;
    OTP_V200_CHANNEL_SEL_U ChannelSel;
    OTP_V200_CPU_RW_CTRL_U CPURWCtrl;
    OTP_V200_MODE_U        Mode;
    OTP_V200_WADDR_U       WAddr;
    OTP_V200_WDATA_U       WDATA;
    OTP_V200_WR_START_U    WRStart;
	HI_U32 u32OTPTimeValue = 0;

    CtrlStaut.u32 = otp_read_reg(OTP_V200_CTRL_STATUS);
    //Check CTRL_STATUS ctrl_ready to 0x01
    while(CtrlStaut.bits.ctrl_ready != 1)
    {
        otp_wait(1);
        CtrlStaut.u32 = otp_read_reg(OTP_V200_CTRL_STATUS);
    }
    //Set OTP_V200_CHANNEL_SEL bit channel_sel to 2'b10
    ChannelSel.u32 = otp_read_reg(OTP_V200_CHANNEL_SEL);
    ChannelSel.bits.channel_sel = 0x02;
    otp_write_reg(OTP_V200_CHANNEL_SEL, ChannelSel.u32);
    //Set wr_sel to 1, set wr_enable to 1, set cpu_size 2'b01(Byte operation)
    CPURWCtrl.u32 = otp_read_reg(OTP_V200_CPU_RW_CTRL);
    CPURWCtrl.bits.wr_sel = 0x01;
    CPURWCtrl.bits.wr_enable = 0x01;
    CPURWCtrl.bits.cpu_size = 0x01;
    otp_write_reg(OTP_V200_CPU_RW_CTRL, CPURWCtrl.u32);
    //Set OTP_V200_MODE
    Mode.u32 = otp_read_reg(OTP_V200_MODE);

    u32OTPTimeValue = DRV_OTP_GetOTPTimeValue();
    Mode.u32 |= u32OTPTimeValue;

    otp_write_reg(OTP_V200_MODE, Mode.u32);
    //Set OTP_V200_WADDR
    WAddr.u32 = 0;
    WAddr.bits.waddr = addr;
    otp_write_reg(OTP_V200_WADDR, WAddr.u32);
    //Set OTP_V200_WDATA
    WDATA.bits.wdata = (HI_U32)tdata;
    otp_write_reg(OTP_V200_WDATA, WDATA.u32);
    //Set WR_START bit start to 1
    WRStart.u32 = otp_read_reg(OTP_V200_WR_START);
    WRStart.bits.start = 1;
    otp_write_reg(OTP_V200_WR_START, WRStart.u32);

    //Check CTRL_STATUS bit ctrl_ready to 0x01
    CtrlStaut.u32 = otp_read_reg(OTP_V200_CTRL_STATUS);
    while(CtrlStaut.bits.ctrl_ready != 1)
    {
        otp_wait(1);
        CtrlStaut.u32 = otp_read_reg(OTP_V200_CTRL_STATUS);
    }

    if (0 != CtrlStaut.bits.fail_flag)
    {
        HI_ERR_OTP("Write OTP failed!\n");
        write_fail_flag();
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}



HI_S32 OTP_V200_getChipID(HI_U32 *pID)
{
    if (pID == NULL)
    {
        return HI_ERR_CA_COMMON_INVALID_PARA;
    }

    *pID = otp_read_reg(OTP_V200_CHIPID);

    return HI_SUCCESS;
}

HI_S32 OTP_GetMarketID(HI_U8 u8Id[4])
{
    unsigned int id;

    if (u8Id == NULL)
    {
        return HI_ERR_CA_COMMON_INVALID_PARA;
    }

    id = otp_read_reg(OTP_V200_MARKETID);

    u8Id[0] = (id >> 24) & 0xff;
    u8Id[1] = (id >> 16) & 0xff;
    u8Id[2] = (id >> 8) & 0xff;
    u8Id[3] = id & 0xff;

    return HI_SUCCESS;
}

HI_S32 OTP_GetR2RLadderLevel(OTP_KEY_LEVEL_E *pLevel)
{
    if (pLevel == NULL)
    {
        return HI_ERR_CA_COMMON_INVALID_PARA;
    }

    *pLevel = otp_read_reg(OTP_V200_PVREGISTER0);
    *pLevel = (*pLevel >> 6) & 0x1;
    if(0 ==*pLevel)
    {
        *pLevel = OTP_KEY_LEV2;
    }
    else
    {
        *pLevel = OTP_KEY_LEV3;
    }
    return HI_SUCCESS;
}

HI_S32 OTP_V200_getCustomerKey(OTP_CUSTOMER_KEY_S *pCustomerKey)
{
	HI_U32 i;
    HI_U32 *pu32CustomerKey = NULL;

    if(NULL == pCustomerKey)
    {
        return HI_FAILURE;
    }
    pu32CustomerKey = (HI_U32*)(pCustomerKey->u32CustomerKey);
    for(i = 0; i < 4; i++)
    {
        pu32CustomerKey[i] = HAL_OTP_V200_Read(OTP_CUSTOMER_KEY_ADDR + i * 4);
    }

    return HI_SUCCESS;
}


/*==============================COMMON INTERFACE==============================*/

#define CHECK_OTP_Init()\
do{\
    if (HI_FALSE == g_bOTPInit )\
    {\
        HI_ERR_OTP("OTP is not init.\n");\
        return HI_FAILURE;\
    }\
}while(0)

static HI_VOID ca_SetClock(HI_VOID)
{
    OTP_V200_CRG_CTRL_U unCRG;

    unCRG.u32 = 0;
    unCRG.u32 = g_pstRegCrg->PERI_CRG48.u32;

    unCRG.bits.ca_kl_srst_req = 0;
    unCRG.bits.ca_ci_srst_req = 0;
    unCRG.bits.otp_srst_req = 0;
	/* Select 200M clock */
    unCRG.bits.ca_ci_clk_sel = 0;
    g_pstRegCrg->PERI_CRG48.u32 = unCRG.u32;

    return;
}

static HI_VOID ca_common_init(HI_VOID)
{
    ca_SetClock();
	return;
}

HI_VOID OTP_Init(void)
{
    ca_common_init();
    g_bOTPInit = HI_TRUE;
    return ;
}

HI_VOID OTP_DeInit(void)
{
    g_bOTPInit = HI_FALSE;
    return ;
}

HI_S32 OTP_SetByte(HI_U32 addr, HI_U8 tdata)
{
    CHECK_OTP_Init();

    return HAL_OTP_V200_WriteByte(addr, tdata);
}

HI_S32 OTP_Write(HI_U32 addr, HI_U32 u32Data)
{
    CHECK_OTP_Init();

	return HAL_OTP_V200_Write(addr, u32Data);
}

HI_U8 OTP_ReadByte(HI_U32 addr)
{
	return HAL_OTP_V200_ReadByte(addr);
}

HI_U32 OTP_Read(HI_U32 addr)
{
    CHECK_OTP_Init();

    return HAL_OTP_V200_Read(addr);
}

HI_S32 OTP_V200_Get_StbPrivData(OTP_STB_PRIV_DATA_S *pStbPrivData)
{
    if(NULL == pStbPrivData)
    {
        return HI_FAILURE;
    }

    pStbPrivData->u8Data = HAL_OTP_V200_ReadByte(OTP_STB_PRIV_DATA_ADDR + pStbPrivData->u32Offset);

    return HI_SUCCESS;
}

HI_S32 OTP_V200_Set_StbPrivData(OTP_STB_PRIV_DATA_S *pStbPrivData)
{
    if(NULL == pStbPrivData)
    {
        return HI_FAILURE;
    }

    return HAL_OTP_V200_WriteByte(OTP_STB_PRIV_DATA_ADDR + pStbPrivData->u32Offset, pStbPrivData->u8Data);
}
