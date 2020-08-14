/******************************************************************************

  Copyright (C), 2014-2024, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hdmi_hal_hdcp22.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2016/03/02
  Description   :
  History       :
  Date          : 2016/03/02
  Author        : l00232354
  Modification  :
*******************************************************************************/

#include <linux/jiffies.h>
#include <linux/delay.h>

#include "hdmi_reg_aon.h"
#include "hdmi_reg_ctrl.h"

#include "hdmi_hal_ddc.h"
#include "hdmi_reg_hdcp.h"
#include "hdmi_reg_mcu.h"
#include "hdmi_hal_intf.h"

#include "hdmi_platform.h"
#include "hdmi_product_define.h"
#include "hdmi_hal_8051mcu.h"


/******************************************************************************/
/* Global defined                                                             */
/******************************************************************************/
#define HDCP22_DEFUALT_INTERVAL          (10)
#define HDCP22_RECVID_SIZE               (5)
//#define HDCP22_MCU_CODE_SIZE             (32*1024)
#define HDCP22_HW_VERION                 (0x100)
#define HDCP22_MCU_DRAM_SIZE             (4*1024)
#define HDCP22_RECVID_VILIDNUM_ADDR      (0xA80)
#define HDCP22_RECVID_LIST_ADDR          (0xA81)
#define HDCP22_RECVID_ADDR               (0xB20)
#define HDCP22_OFFSET_HDCP2XVERSION         0x50


#define DRAM_ADDR_RECVID_VALIDNUM  (0xA80)                            /* 1   Bytes    0xA80  */
#define DRAM_ADDR_RECVID_LIST      (DRAM_ADDR_RECVID_VALIDNUM + 0x1)  /* 160 Bytes    0xA81  */


#define HDCP22_NULL_CHK(p) do{\
	if(HI_NULL == p)\
	{HDMI_WARN("%s is null pointer!return fail.\n", #p);\
	return HI_FAILURE;}\
}while(0)

#define HDCP22_NULL_CHK_NORET(p) do{\
    if(HI_NULL == p)\
    {HDMI_WARN("%s is null pointer!\n", #p);\
    return;}\
}while(0)

#define HDCP22_INIT_CHK(pstHdcp22) do{\
	HDCP22_NULL_CHK(pstHdcp22);\
	if(pstHdcp22->stInit.bInit != HI_TRUE)\
	{HDMI_WARN("hdcp22 module didn't init!\n");\
	return HI_FAILURE;}\
}while(0)

#define HDCP22_INIT_CHK_NORET(pstHdcp22) do{\
    HDCP22_NULL_CHK_NORET(pstHdcp22);\
    if(pstHdcp22->stInit.bInit != HI_TRUE)\
    {HDMI_WARN("hdcp22 module didn't init!\n");\
    return;}\
}while(0)

#define HDCP22_CHK_FAILURE(s32Ret) do{\
    if(HI_SUCCESS != s32Ret)\
    {HDMI_WARN("retval=%d is not HI_SUCCESS! return failure!\n", s32Ret);\
    return HI_FAILURE;}\
}while(0)

#define HDCP22_CHK_FAILURE_NORET(s32Ret) do{\
	if(HI_SUCCESS != s32Ret)\
	{HDMI_WARN("retval=%d is not HI_SUCCESS!\n", s32Ret);\
	return ;}\
}while(0)


#define DRAM_WRITE(addr, value) \
do { \
    if (HI_NULL != g_Hdcp22McuDramVirAddr) { \
    HDMIRegWriteU32(BASE_ADDR_mcu_dram + addr*4, value); \
    } \
    else { \
        HDMI_ERR("g_Hdcp22McuDramVirAddr is null!!!\n"); \
    } \
}while(0)


#define DRAM_READ(addr, pvalue) \
do { \
    if (HI_NULL != g_Hdcp22McuDramVirAddr) { \
        *pvalue = HDMIRegReadU32(BASE_ADDR_mcu_dram + addr*4);\
    } \
    else { \
        HDMI_ERR("g_Hdcp22McuDramVirAddr is null!!!\n"); \
    } \
}while(0)

/******************************************************************************/
/* Global value                                                               */
/******************************************************************************/
static HDCP22_INFO_S  s_Hdcp22Info[HDMI_DEVICE_ID_BUTT];
volatile HI_U32 *g_Hdcp22McuDramVirAddr = NULL;


/******************************************************************************/
/* Private interface                                                          */
/******************************************************************************/
static HDCP22_INFO_S* HDCP22_InfoGet(HDMI_DEVICE_ID_E enHdmiId)
{
    if (enHdmiId < HDMI_DEVICE_ID_BUTT)
    {
        return &s_Hdcp22Info[enHdmiId];
    }
    
    return HI_NULL;
}



HI_VOID HDCP22_HwVersionSet(HI_U32 u32HwVersion)
{
    HI_U32 u32Data = 0;

    u32Data = u32HwVersion & 0xff;
    HDMI_HDCP2X_HW_VER_B0_hdcp2x_hw_ver_b0Set(u32Data);

    u32Data = (u32HwVersion >> 8) & 0xff;
    HDMI_HDCP2X_HW_VER_B1_hdcp2x_hw_ver_b1Set(u32Data);

    u32Data = (u32HwVersion >> 16) & 0xff;
    HDMI_HDCP2X_HW_VER_B2_hdcp2x_hw_ver_b2Set(u32Data);

    u32Data = (u32HwVersion >> 24) & 0xff;
    HDMI_HDCP2X_HW_VER_B3_hdcp2x_hw_ver_b3Set(u32Data);

    return;
}


HI_VOID HDCP22_SwVersionGet(HI_U32 *pu32SwVersion)
{
    HI_U32 u32Data = 0;

    HDCP22_NULL_CHK_NORET(pu32SwVersion);

    u32Data = HDMI_HDCP2X_SW_VER_B0_hdcp2x_sw_ver_b0Get();
    *pu32SwVersion = u32Data&0xff;

    u32Data = HDMI_HDCP2X_SW_VER_B1_hdcp2x_sw_ver_b1Get();
    *pu32SwVersion |= (u32Data&0xff) << 8;

    u32Data = HDMI_HDCP2X_SW_VER_B2_hdcp2x_sw_ver_b2Get();
    *pu32SwVersion |= (u32Data&0xff) << 16;

    u32Data = HDMI_HDCP2X_SW_VER_B3_hdcp2x_sw_ver_b3Get();
    *pu32SwVersion |= (u32Data&0xff) << 24;

    return;
}


HI_VOID HDCP22_IntrInfoCheck(HDCP22_INTR_INFO_S *pstIntrInfo)
{
    HDCP22_INTR_U uIntr;

    HDCP22_NULL_CHK_NORET(pstIntrInfo);
    HDMI_MEMSET(&uIntr, 0x0, sizeof(HDCP22_INTR_U));

    uIntr.bits.bMcuIntr00 = HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr0Get();
    uIntr.bits.bMcuIntr01 = HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr1Get();
    uIntr.bits.bMcuIntr02 = HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr2Get();
    uIntr.bits.bMcuIntr03 = HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr3Get();
    uIntr.bits.bMcuIntr04 = HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr4Get();
    uIntr.bits.bMcuIntr05 = HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr5Get();
    uIntr.bits.bMcuIntr06 = HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr6Get();
    uIntr.bits.bMcuIntr07 = HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr7Get();
    pstIntrInfo->uIntrB0.u8 = uIntr.u8;

    uIntr.bits.bMcuIntr00 = HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr8Get();
    uIntr.bits.bMcuIntr01 = HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr9Get();
    uIntr.bits.bMcuIntr02 = HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr10Get();
    uIntr.bits.bMcuIntr03 = HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr11Get();
    uIntr.bits.bMcuIntr04 = HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr12Get();
    uIntr.bits.bMcuIntr05 = HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr13Get();
    uIntr.bits.bMcuIntr06 = HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr14Get();
    uIntr.bits.bMcuIntr07 = HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr15Get();
    pstIntrInfo->uIntrB1.u8 = uIntr.u8;

    uIntr.bits.bMcuIntr00 = HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr16Get();
    uIntr.bits.bMcuIntr01 = HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr17Get();
    uIntr.bits.bMcuIntr02 = HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr18Get();
    uIntr.bits.bMcuIntr03 = HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr19Get();
    uIntr.bits.bMcuIntr04 = HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr20Get();
    uIntr.bits.bMcuIntr05 = HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr21Get();
    uIntr.bits.bMcuIntr06 = HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr22Get();
    uIntr.bits.bMcuIntr07 = HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr23Get();
    pstIntrInfo->uIntrB2.u8 = uIntr.u8;

    uIntr.bits.bMcuIntr00 = HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr24Get();
    uIntr.bits.bMcuIntr01 = HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr25Get();
    uIntr.bits.bMcuIntr02 = HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr26Get();
    uIntr.bits.bMcuIntr03 = HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr27Get();
    uIntr.bits.bMcuIntr04 = HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr28Get();
    uIntr.bits.bMcuIntr05 = HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr29Get();
    uIntr.bits.bMcuIntr06 = HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr30Get();
    uIntr.bits.bMcuIntr07 = HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr31Get();
    pstIntrInfo->uIntrB3.u8 = uIntr.u8;

}


HI_VOID HDCP22_IntrInfoClear(HDCP22_INTR_INFO_S *pstIntrInfo)
{
    HDCP22_INTR_U uIntr;

    HDCP22_NULL_CHK_NORET(pstIntrInfo);
    HDMI_MEMSET(&uIntr, 0x0, sizeof(HDCP22_INTR_U));

    uIntr.u8 = pstIntrInfo->uIntrB0.u8;
    HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr0Set(uIntr.bits.bMcuIntr00);
    HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr1Set(uIntr.bits.bMcuIntr01);
    HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr2Set(uIntr.bits.bMcuIntr02);
    HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr3Set(uIntr.bits.bMcuIntr03);
    HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr4Set(uIntr.bits.bMcuIntr04);
    HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr5Set(uIntr.bits.bMcuIntr05);
    HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr6Set(uIntr.bits.bMcuIntr06);
    HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr7Set(uIntr.bits.bMcuIntr07);

    uIntr.u8 = pstIntrInfo->uIntrB1.u8;
    HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr8Set(uIntr.bits.bMcuIntr00);
    HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr9Set(uIntr.bits.bMcuIntr01);
    HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr10Set(uIntr.bits.bMcuIntr02);
    HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr11Set(uIntr.bits.bMcuIntr03);
    HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr12Set(uIntr.bits.bMcuIntr04);
    HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr13Set(uIntr.bits.bMcuIntr05);
    HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr14Set(uIntr.bits.bMcuIntr06);
    HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr15Set(uIntr.bits.bMcuIntr07);

    uIntr.u8 = pstIntrInfo->uIntrB2.u8;
    HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr16Set(uIntr.bits.bMcuIntr00);
    HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr17Set(uIntr.bits.bMcuIntr01);
    HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr18Set(uIntr.bits.bMcuIntr02);
    HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr19Set(uIntr.bits.bMcuIntr03);
    HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr20Set(uIntr.bits.bMcuIntr04);
    HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr21Set(uIntr.bits.bMcuIntr05);
    HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr22Set(uIntr.bits.bMcuIntr06);
    HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr23Set(uIntr.bits.bMcuIntr07);

    uIntr.u8 = pstIntrInfo->uIntrB3.u8;
    HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr24Set(uIntr.bits.bMcuIntr00);
    HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr25Set(uIntr.bits.bMcuIntr01);
    HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr26Set(uIntr.bits.bMcuIntr02);
    HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr27Set(uIntr.bits.bMcuIntr03);
    HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr28Set(uIntr.bits.bMcuIntr04);
    HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr29Set(uIntr.bits.bMcuIntr05);
    HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr30Set(uIntr.bits.bMcuIntr06);
    HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr31Set(uIntr.bits.bMcuIntr07);

    return;
}


HI_VOID HDCP22_IntrMaskGet(HDCP22_MASK_INFO_S *pstMaskInfo)
{
    HDCP22_MASK_U uMask;

    HDCP22_NULL_CHK_NORET(pstMaskInfo);
    HDMI_MEMSET(&uMask, 0x0, sizeof(HDCP22_MASK_U));

    uMask.bits.bMcuMask00 = HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr0Get();
    uMask.bits.bMcuMask01 = HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr1Get();
    uMask.bits.bMcuMask02 = HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr2Get();
    uMask.bits.bMcuMask03 = HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr3Get();
    uMask.bits.bMcuMask04 = HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr4Get();
    uMask.bits.bMcuMask05 = HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr5Get();
    uMask.bits.bMcuMask06 = HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr6Get();
    uMask.bits.bMcuMask07 = HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr7Get();
    pstMaskInfo->uMaskB0.u8 = uMask.u8;

    uMask.bits.bMcuMask00 = HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr8Get();
    uMask.bits.bMcuMask01 = HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr9Get();
    uMask.bits.bMcuMask02 = HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr10Get();
    uMask.bits.bMcuMask03 = HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr11Get();
    uMask.bits.bMcuMask04 = HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr12Get();
    uMask.bits.bMcuMask05 = HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr13Get();
    uMask.bits.bMcuMask06 = HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr14Get();
    uMask.bits.bMcuMask07 = HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr15Get();
    pstMaskInfo->uMaskB1.u8 = uMask.u8;

    uMask.bits.bMcuMask00 = HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr16Get();
    uMask.bits.bMcuMask01 = HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr17Get();
    uMask.bits.bMcuMask02 = HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr18Get();
    uMask.bits.bMcuMask03 = HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr19Get();
    uMask.bits.bMcuMask04 = HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr20Get();
    uMask.bits.bMcuMask05 = HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr21Get();
    uMask.bits.bMcuMask06 = HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr22Get();
    uMask.bits.bMcuMask07 = HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr23Get();
    pstMaskInfo->uMaskB2.u8 = uMask.u8;

    uMask.bits.bMcuMask00 = HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr24Get();
    uMask.bits.bMcuMask01 = HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr25Get();
    uMask.bits.bMcuMask02 = HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr26Get();
    uMask.bits.bMcuMask03 = HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr27Get();
    uMask.bits.bMcuMask04 = HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr28Get();
    uMask.bits.bMcuMask05 = HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr29Get();
    uMask.bits.bMcuMask06 = HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr30Get();
    uMask.bits.bMcuMask07 = HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr31Get();
    pstMaskInfo->uMaskB3.u8 = uMask.u8;

    return;
}


HI_VOID HDCP22_IntrMaskSet(HDCP22_MASK_INFO_S *pstMaskInfo)
{
    HDCP22_INTR_U uMask;

    HDCP22_NULL_CHK_NORET(pstMaskInfo);
    HDMI_MEMSET(&uMask, 0x0, sizeof(HDCP22_MASK_U));

    uMask.u8 = pstMaskInfo->uMaskB0.u8;
    HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr0Set(uMask.bits.bMcuIntr00);
    HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr1Set(uMask.bits.bMcuIntr01);
    HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr2Set(uMask.bits.bMcuIntr02);
    HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr3Set(uMask.bits.bMcuIntr03);
    HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr4Set(uMask.bits.bMcuIntr04);
    HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr5Set(uMask.bits.bMcuIntr05);
    HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr6Set(uMask.bits.bMcuIntr06);
    HDMI_HDCP2X_MCU_INTR_B0_hdcp2x_mcu_intr7Set(uMask.bits.bMcuIntr07);

    uMask.u8 = pstMaskInfo->uMaskB1.u8;
    HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr8Set(uMask.bits.bMcuIntr00);
    HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr9Set(uMask.bits.bMcuIntr01);
    HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr10Set(uMask.bits.bMcuIntr02);
    HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr11Set(uMask.bits.bMcuIntr03);
    HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr12Set(uMask.bits.bMcuIntr04);
    HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr13Set(uMask.bits.bMcuIntr05);
    HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr14Set(uMask.bits.bMcuIntr06);
    HDMI_HDCP2X_MCU_INTR_B1_hdcp2x_mcu_intr15Set(uMask.bits.bMcuIntr07);

    uMask.u8 = pstMaskInfo->uMaskB2.u8;
    HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr16Set(uMask.bits.bMcuIntr00);
    HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr17Set(uMask.bits.bMcuIntr01);
    HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr18Set(uMask.bits.bMcuIntr02);
    HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr19Set(uMask.bits.bMcuIntr03);
    HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr20Set(uMask.bits.bMcuIntr04);
    HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr21Set(uMask.bits.bMcuIntr05);
    HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr22Set(uMask.bits.bMcuIntr06);
    HDMI_HDCP2X_MCU_INTR_B2_hdcp2x_mcu_intr23Set(uMask.bits.bMcuIntr07);

    uMask.u8 = pstMaskInfo->uMaskB3.u8;
    HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr24Set(uMask.bits.bMcuIntr00);
    HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr25Set(uMask.bits.bMcuIntr01);
    HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr26Set(uMask.bits.bMcuIntr02);
    HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr27Set(uMask.bits.bMcuIntr03);
    HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr28Set(uMask.bits.bMcuIntr04);
    HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr29Set(uMask.bits.bMcuIntr05);
    HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr30Set(uMask.bits.bMcuIntr06);
    HDMI_HDCP2X_MCU_INTR_B3_hdcp2x_mcu_intr31Set(uMask.bits.bMcuIntr07);

    return;
}


HI_S32 HDCP22_AuthFollowGet(HDCP22_AUTH_FOLLOW_S *pAuthFollow)
{
    HDCP22_AUTH_STATE_RECV_U uStatusRecv;
    HDCP22_AUTH_STATE_RPT_U  uStatusRpt;
    HDCP22_AUTH_STATE0_U     uStatus0;
    HDCP22_AUTH_STATE1_U     uStatus1;
    HDCP22_AUTH_STATE2_U     uStatus2;
    HDCP22_AUTH_STATE3_U     uStatus3;

    HDCP22_NULL_CHK(pAuthFollow);
    HDMI_MEMSET(&uStatusRecv, 0x0, sizeof(HDCP22_AUTH_STATE_RECV_U));
    HDMI_MEMSET(&uStatusRpt, 0x0, sizeof(HDCP22_AUTH_STATE_RPT_U));
    HDMI_MEMSET(&uStatus0, 0x0, sizeof(HDCP22_AUTH_STATE0_U));
    HDMI_MEMSET(&uStatus1, 0x0, sizeof(HDCP22_AUTH_STATE1_U));
    HDMI_MEMSET(&uStatus2, 0x0, sizeof(HDCP22_AUTH_STATE2_U));
    HDMI_MEMSET(&uStatus3, 0x0, sizeof(HDCP22_AUTH_STATE3_U));
    
    uStatusRecv.bits.bAkeInitDone  = HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_ake_init_doneGet();
    uStatusRecv.bits.bSendCertDone = HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_send_cert_doneGet();
    uStatusRecv.bits.bKmDone       = HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_km_doneGet();
    uStatusRecv.bits.bSendHDone    = HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_send_h_doneGet();
    uStatusRecv.bits.bPairingDone  = HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_pairing_doneGet();
    uStatusRecv.bits.bLcInitDone   = HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_lc_init_doneGet();
    uStatusRecv.bits.bSendLDone    = HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_send_l_doneGet();
    uStatusRecv.bits.bSendSksDone  = HDMI_HDCP2X_RX_AUTH_STATE_hdcp2x_send_sks_doneGet();
    pAuthFollow->uStatusRecv.u8    = uStatusRecv.u8;

    uStatusRpt.bits.bIdListDone     = HDMI_HDCP2X_RPT_AUTH_STATE_hdcp2x_id_list_doneGet();
    uStatusRpt.bits.bSendAckDone    = HDMI_HDCP2X_RPT_AUTH_STATE_hdcp2x_send_ack_doneGet();
    uStatusRpt.bits.bStrmManageDone = HDMI_HDCP2X_RPT_AUTH_STATE_hdcp2x_strm_id_doneGet();
    uStatusRpt.bits.bStrmRdyDone    = HDMI_HDCP2X_RPT_AUTH_STATE_hdcp2x_strm_rdy_doneGet();
    pAuthFollow->uStatusRpt.u8      = uStatusRpt.u8;

    uStatus0.u8 = HDMI_HDCP2X_AUTH_STATE0_hdcp2x_auth_state0Get();
    uStatus1.u8 = HDMI_HDCP2X_AUTH_STATE1_hdcp2x_auth_state1Get();
    uStatus2.u8 = HDMI_HDCP2X_AUTH_STATE2_hdcp2x_auth_state2Get();
    uStatus3.u8 = HDMI_HDCP2X_AUTH_STATE3_hdcp2x_auth_state3Get();
    pAuthFollow->uStatus0.u8 = uStatus0.u8;
    pAuthFollow->uStatus1.u8 = uStatus1.u8;
    pAuthFollow->uStatus2.u8 = uStatus2.u8;
    pAuthFollow->uStatus3.u8 = uStatus3.u8;

    return HI_SUCCESS;
}


HI_S32 HDCP22_InvalidRecvIdCheck(HI_U8 *pu8RecvId, HI_U8 u8DecvCnt, HI_BOOL *pbInvalid)
{
    HI_S32 s32Ret = HI_SUCCESS;

    HDCP22_NULL_CHK(pu8RecvId);

    /* Need call SRM model interface */

    return s32Ret;
}


HI_VOID HDCP22_RecvIdGet(HI_U8 *pu8RecvId)
{
    HI_S32 i = 0;
    HI_U32 offset = 0;

    HDCP22_NULL_CHK_NORET(pu8RecvId);

    offset = HDCP22_RECVID_ADDR;
    /* Need read from DRAM */
    for (i = 0; i < HDCP22_RECVID_SIZE; i++)
    {
        DRAM_READ(offset, pu8RecvId);
        pu8RecvId++;
        offset++;
    }

    return;
}


HI_VOID HDCP22_RecvIdListGet(HI_U8 *pu8RecvIdList, HI_U8 u8DecvCnt)
{
    HI_S32 i = 0, j = 0;
    HI_U32 offset = 0;
    
    HDCP22_NULL_CHK_NORET(pu8RecvIdList);

    offset = HDCP22_RECVID_LIST_ADDR;
    /* Need read from DRAM */
    for (i = 0; i < u8DecvCnt; i++)
    {
        for (j = 0; j < HDCP22_RECVID_SIZE; j++)
        {
            DRAM_READ(offset, pu8RecvIdList);
            pu8RecvIdList++;
            offset++;
        }
    }

    return;
}


HI_VOID HDCP22_RxCapsGet(HDCP22_RX_CAPS_S *pRxCaps)
{
    HDCP22_NULL_CHK_NORET(pRxCaps);

    pRxCaps->bRepeater = HDMI_HDCP2X_CERT_INFO_hdcp2x_cert_rptGet();
    pRxCaps->u8Version = HDMI_HDCP2X_CERT_VER_hdcp2x_cert_verGet();

    return;
}


HI_VOID HDCP22_SeqNumVGet(HDCP22_SEQ_NUM_V_S *pSeqNumV)
{
    HDCP22_NULL_CHK_NORET(pSeqNumV);

    pSeqNumV->u8SeqNumV0 = HDMI_HDCP2X_RPT_V0_hdcp2x_rpt_v0Get();
    pSeqNumV->u8SeqNumV1 = HDMI_HDCP2X_RPT_V1_hdcp2x_rpt_v1Get();
    pSeqNumV->u8SeqNumV2 = HDMI_HDCP2X_RPT_V2_hdcp2x_rpt_v2Get();

    return;
}


HI_VOID HDCP22_RxInfoGet(HDCP22_RX_INFO_S *pRxInfo)
{
    HDCP22_NULL_CHK_NORET(pRxInfo);

    pRxInfo->u8Depth              = HDMI_HDCP2X_RPT_DEV_hdcp2x_rpt_dep_cntGet();
    pRxInfo->u8DevCnt             = HDMI_HDCP2X_RPT_DEV_hdcp2x_rpt_dev_cntGet();
    pRxInfo->bMaxDevsExceeded     = HDMI_HDCP2X_RPT_INFO_hdcp2x_rpt_dev_excGet();
    pRxInfo->bMaxCascadeExceeded  = HDMI_HDCP2X_RPT_INFO_hdcp2x_rpt_cas_excGet();
    pRxInfo->bMaxHdcp20DownStream = HDMI_HDCP2X_RPT_INFO_hdcp20_rpt_onGet();
    pRxInfo->bMaxHdcp1xDownStream = HDMI_HDCP2X_RPT_INFO_hdcp1x_dev_onGet();

    return;
}


HI_VOID HDCP22_StreamManageSet(HDCP22_STREAM_MANAGE_S *pStreamManage)
{
    HDCP22_NULL_CHK_NORET(pStreamManage);

    HDMI_HDCP2X_RPT_M0_hdcp2x_rpt_m0Set(pStreamManage->u8SeqNumM0);
    HDMI_HDCP2X_RPT_M1_hdcp2x_rpt_m1Set(pStreamManage->u8SeqNumM1);
    HDMI_HDCP2X_RPT_M2_hdcp2x_rpt_m2Set(pStreamManage->u8SeqNumM2);

    HDMI_HDCP2X_RPT_K0_hdcp2x_rpt_k0Set(pStreamManage->u8K0);
    HDMI_HDCP2X_RPT_K1_hdcp2x_rpt_k1Set(pStreamManage->u8K1);

    HDMI_HDCP2X_RPT_STRM_ID_hdcp2x_rpt_strm_idSet(pStreamManage->u8StreamID);
    HDMI_HDCP2X_RPT_STRM_TPYE_hdcp2x_rpt_strm_typeSet(pStreamManage->u8StreamType);

    return;
}


HI_VOID HDCP22_StreamManageGet(HDCP22_STREAM_MANAGE_S *pStreamManage)
{
    HDCP22_NULL_CHK_NORET(pStreamManage);

    pStreamManage->u8SeqNumM0   = HDMI_HDCP2X_RPT_M0_hdcp2x_rpt_m0Get();
    pStreamManage->u8SeqNumM1   = HDMI_HDCP2X_RPT_M1_hdcp2x_rpt_m1Get();
    pStreamManage->u8SeqNumM2   = HDMI_HDCP2X_RPT_M2_hdcp2x_rpt_m2Get();
    pStreamManage->u8K0         = HDMI_HDCP2X_RPT_K0_hdcp2x_rpt_k0Get();
    pStreamManage->u8K1         = HDMI_HDCP2X_RPT_K1_hdcp2x_rpt_k1Get();
    pStreamManage->u8StreamID   = HDMI_HDCP2X_RPT_STRM_ID_hdcp2x_rpt_strm_idGet();
    pStreamManage->u8StreamType = HDMI_HDCP2X_RPT_STRM_TPYE_hdcp2x_rpt_strm_typeGet();

    return;
}


HI_VOID HDCP22_AuthStart(HI_VOID)
{
    /* release reset request */
    HDMI_HDCP_FUN_SEL_hdcp_fun_selSet(HI_TRUE);
    HDMI_TX_PWD_RST_CTRL_tx_mcu_srst_reqSet(HI_FALSE);

    /* start auth */
    HDMI_HDCP2X_AUTH_CTRL_mcu_auth_startSet(HI_TRUE);
    HDMI_HDCP2X_AUTH_CTRL_mcu_auth_stopSet(HI_FALSE);

    HDMI_INFO("hdcp22 start!\n");
    return;
}


HI_VOID HDCP22_AuthStop(HI_VOID)
{
    /* stop auth */
    HDMI_HDCP2X_AUTH_CTRL_mcu_auth_startSet(HI_FALSE);
    HDMI_HDCP2X_AUTH_CTRL_mcu_auth_stopSet(HI_TRUE);

    /* send reset request */
    HDMI_HDCP_FUN_SEL_hdcp_fun_selSet(HI_FALSE);
    HDMI_TX_PWD_RST_CTRL_tx_mcu_srst_reqSet(HI_TRUE);
    
    HDMI_INFO("hdcp22 stop!\n");
    
    return;
}


HI_VOID HDCP22_IntrProcess(HDCP22_INFO_S *pstHdcp22)
{
    HDCP22_INTR_INFO_S  stIntrInfo;
    HDCP22_INTR_B0_U    uIntrB0;

    HDCP22_NULL_CHK_NORET(pstHdcp22);
    HDMI_MEMSET(&stIntrInfo, 0x0, sizeof(HDCP22_INTR_INFO_S));
    HDMI_MEMSET(&uIntrB0, 0x0, sizeof(HDCP22_INTR_B0_U));

    HDCP22_IntrInfoCheck(&stIntrInfo);
    uIntrB0.u8 = stIntrInfo.uIntrB0.u8;

    if (HI_TRUE == uIntrB0.bits.bRevidReady)
    {
        HDCP22_SwVersionGet(&(pstHdcp22->stAuth.stVerInfo.u32SwVersion));
        HDCP22_RxCapsGet(&(pstHdcp22->stAuth.stRxCaps));
        HDCP22_RecvIdGet(&(pstHdcp22->stAuth.u8RecvId[0]));
    }

    if (HI_TRUE == uIntrB0.bits.bRevidListReady)
    {
        HDCP22_RxInfoGet(&(pstHdcp22->stAuth.stRxInfo));
        HDCP22_RecvIdListGet(&(pstHdcp22->stAuth.u8RecvIdList[0]), pstHdcp22->stAuth.stRxInfo.u8DevCnt);
    }

    if (HI_TRUE == uIntrB0.bits.bReauthRequset)
    {
        ;
    }

    if (HI_TRUE == uIntrB0.bits.bAuthFail)
    {
        pstHdcp22->stInit.pfnEventCallBack(pstHdcp22->stInit.pvEventData, HDMI_EVENT_HDCP_FAIL);
        pstHdcp22->stRun.enMachStatus = HDCP22_MACH_AUTH_EN;
    }

    if (HI_TRUE == uIntrB0.bits.bAuthDone)
    {
        pstHdcp22->stInit.pfnEventCallBack(pstHdcp22->stInit.pvEventData, HDMI_EVENT_HDCP_SUCCESS);
    }

    HDCP22_IntrInfoClear(&stIntrInfo);

    return;
}


HI_VOID HDCP22_MachCallBack(HI_VOID *pData)
{
    HDCP22_INFO_S       *pstHdcp22 = (HDCP22_INFO_S *)pData;
    HDCP22_MACH_E       *penMachStatus = HI_NULL;
    HI_U64              *pu64StartStamp = HI_NULL;
    HDCP22_INTR_INFO_S  stIntrInfo;
    
    if((!pstHdcp22) || (!pstHdcp22->stInit.bInit))
    {
        HDMI_WARN("pstHdcp22 is null OR module hdcp22 didn't init!\n");
        return; 
    }

    HDMI_MEMSET(&stIntrInfo, 0x0, sizeof(HDCP22_INTR_INFO_S));
    penMachStatus   = &pstHdcp22->stRun.enMachStatus;
    pu64StartStamp  = &pstHdcp22->stRun.u64MachStartStamp;
        
    switch(pstHdcp22->stRun.enMachStatus)
    {
        case HDCP22_MACH_AUTH_EN:
            HDCP22_StreamManageSet(&(pstHdcp22->stAuth.stStreamManage));
            stIntrInfo.uIntrB0.u8 = 0xff;
            stIntrInfo.uIntrB1.u8 = 0xff;
            stIntrInfo.uIntrB2.u8 = 0xff;
            stIntrInfo.uIntrB3.u8 = 0xff;
            HDCP22_IntrInfoClear(&stIntrInfo);

            *pu64StartStamp = HAL_HDMI_MachMsGet();
            *penMachStatus  = HDCP22_MACH_WAIT_START;
            break;

        case HDCP22_MACH_WAIT_START:
            if ( HAL_HDMI_MachMsGet()-(*pu64StartStamp) > pstHdcp22->stAttr.u64StartWaitMs )
            {
                HDCP22_AuthStart();
                *penMachStatus = HDCP22_MACH_CHK_INTR;
            }
            break;

        case HDCP22_MACH_CHK_INTR:
            HDCP22_IntrProcess(pstHdcp22);
            break;

        default:
            HDMI_ERR("hdcp22 error status machine!\n");
            break;
    }
    
    return ;
}

/******************************************************************************/
/* Public interface                                                           */
/******************************************************************************/
HI_S32  HAL_HDMI_Hdcp22Init(HDMI_DEVICE_ID_E enHdmiId, HDMI_HAL_INIT_S *pstHalInit)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HDMI_MACH_CTRL_S stMachCtrl = {0};
    HDCP22_INFO_S   *pstHdcp22 = HI_NULL;

    pstHdcp22 = HDCP22_InfoGet(enHdmiId);
    HDCP22_NULL_CHK(pstHdcp22);
    HDCP22_NULL_CHK(pstHalInit);

    if (!pstHdcp22->stInit.bInit)
    {
        s32Ret = HDMI_TX_S_tx_hdcp_reg_REGS_TYPE_Init();
        HDCP22_CHK_FAILURE(s32Ret);

        s32Ret = HDMI_TX_S_mcu_cpu_reg_REGS_TYPE_Init();
        HDCP22_CHK_FAILURE(s32Ret);

        g_Hdcp22McuDramVirAddr = (volatile HI_U32 *)ioremap_nocache(BASE_ADDR_mcu_dram, 4);

        HDMI_MEMSET(pstHdcp22, 0x0, sizeof(HDCP22_INFO_S));
        pstHdcp22->stInit.bInit             = HI_TRUE;
        pstHdcp22->stInit.pfnEventCallBack  = pstHalInit->pfnEventCallBack;
        pstHdcp22->stInit.pvEventData       = pstHalInit->pvEventData;
        pstHdcp22->stInit.enHdmiId          = enHdmiId;

        pstHdcp22->stRun.bMcuLoad           = HI_FALSE;
        pstHdcp22->stRun.enMachStatus       = HDCP22_MACH_AUTH_EN;

        pstHdcp22->stAttr.bFailStopMach     = HI_FALSE;
        pstHdcp22->stAttr.u32MachInterval   = HDCP22_DEFUALT_INTERVAL;

        pstHdcp22->stAuth.stVerInfo.u32HwVersion = HDCP22_HW_VERION;
        HDCP22_HwVersionSet(pstHdcp22->stAuth.stVerInfo.u32HwVersion);

        pstHdcp22->stMask.uMaskB0.u8 = 0xff;
        HDCP22_IntrMaskSet(&(pstHdcp22->stMask));

        pstHdcp22->stAuth.stStreamManage.u8K0 = 0x1;
        pstHdcp22->stAuth.stStreamManage.u8K1 = 0x0;
        pstHdcp22->stAuth.stStreamManage.u8StreamID   = 0x0;
        pstHdcp22->stAuth.stStreamManage.u8StreamType = 0x1;
        HDCP22_StreamManageSet(&(pstHdcp22->stAuth.stStreamManage));

        stMachCtrl.u64Interval              = HDCP22_DEFUALT_INTERVAL; 
        stMachCtrl.pCbData                  = pstHdcp22;
        stMachCtrl.pfnCallBack              = HDCP22_MachCallBack;
        stMachCtrl.pu8Name                  = "HDCP22"; 

        s32Ret = HAL_HDMI_MachRegister(&stMachCtrl, &pstHdcp22->stInit.u32MachId);
        HDCP22_CHK_FAILURE(s32Ret);
    }

    return s32Ret;
}


HI_VOID HAL_HDMI_Hdcp22DeInit(HDMI_DEVICE_ID_E enHdmiId)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HDCP22_INFO_S *pstHdcp22 = HI_NULL;

    pstHdcp22 = HDCP22_InfoGet(enHdmiId);
    HDCP22_NULL_CHK_NORET(pstHdcp22);
    HDCP22_INIT_CHK_NORET(pstHdcp22);
    
    s32Ret = HAL_HDMI_MachStop(pstHdcp22->stInit.u32MachId);
    HDCP22_CHK_FAILURE_NORET(s32Ret);

    s32Ret = HAL_HDMI_MachUnRegister(pstHdcp22->stInit.u32MachId);
    HDCP22_CHK_FAILURE_NORET(s32Ret);

    s32Ret = HDMI_TX_S_tx_hdcp_reg_REGS_TYPE_DeInit();
    HDCP22_CHK_FAILURE_NORET(s32Ret);

    s32Ret = HDMI_TX_S_mcu_cpu_reg_REGS_TYPE_DeInit();
    HDCP22_CHK_FAILURE_NORET(s32Ret);

    if (HI_NULL != g_Hdcp22McuDramVirAddr)
    {
        iounmap(g_Hdcp22McuDramVirAddr);
    }
    g_Hdcp22McuDramVirAddr = HI_NULL;

    HDMI_MEMSET(pstHdcp22, 0x0, sizeof(HDCP22_INFO_S));
    pstHdcp22->stInit.bInit = HI_FALSE;
    
    return;
}


HI_VOID HAL_HDMI_Hdcp22Reset(HDMI_DEVICE_ID_E enHdmiId)
{
    HI_S32         s32Ret = HI_SUCCESS;
    HDCP22_INFO_S *pstHdcp22 = HI_NULL;

    pstHdcp22 = HDCP22_InfoGet(enHdmiId);
    HDCP22_NULL_CHK_NORET(pstHdcp22);
    HDCP22_INIT_CHK_NORET(pstHdcp22);    

    s32Ret = HAL_HDMI_MachStop(pstHdcp22->stInit.u32MachId);
    HDCP22_CHK_FAILURE_NORET(s32Ret);

    HDCP22_AuthStop();
    HDMI_MEMSET(&pstHdcp22->stRun, 0x0, sizeof(HDCP22_RUN_STATUS_S));
    HDMI_MEMSET(&pstHdcp22->stAttr, 0x0, sizeof(HDCP22_ATTR_S));
    HDMI_MEMSET(&pstHdcp22->stAuth, 0x0, sizeof(HDCP22_AUTH_STATUS_S));

    pstHdcp22->stAuth.stStreamManage.u8K0 = 0x1;
    pstHdcp22->stAuth.stStreamManage.u8K1 = 0x0;
    pstHdcp22->stAuth.stStreamManage.u8StreamID   = 0x0;
    pstHdcp22->stAuth.stStreamManage.u8StreamType = 0x1;
    HDCP22_StreamManageSet(&(pstHdcp22->stAuth.stStreamManage));

    return;
}

HI_S32  HAL_HDMI_Hdcp22McuCodeLoad(HDMI_DEVICE_ID_E enHdmiId)
{
    HI_S32          s32Ret = HI_SUCCESS;
    HDCP22_INFO_S   *pstHdcp22 = HI_NULL;
#ifndef HDMI_TEE_SUPPORT
    HI_U32          *pu32Code = g_8051Code ;
    HI_U32          u32Cnt = 0;
    HI_U32          u32MaxWriteTimes = 5;
    HI_U32          u32Data = 0;

    pstHdcp22 = HDCP22_InfoGet(enHdmiId);
    HDCP22_NULL_CHK(pstHdcp22);

    pstHdcp22->stRun.bMcuLoad = (g_8051Code[0] == HDMIRegReadU32(BASE_ADDR_mcu_pram) ) ? HI_TRUE : HI_FALSE ;
    pstHdcp22->stRun.bMcuLoad &= (g_8051Code[1] == HDMIRegReadU32(BASE_ADDR_mcu_pram + 4) ) ? HI_TRUE : HI_FALSE ;

    if(!pstHdcp22->stRun.bMcuLoad) 
    {
        HDCP22_AuthStop();
        for(u32Cnt = 0;(u32Cnt < HDMI_ARRAY_SIZE(g_8051Code)); u32Cnt++)
        {
            for(u32MaxWriteTimes = 5,u32Data = pu32Code[u32Cnt] + 1;
                (u32Data != pu32Code[u32Cnt]) && u32MaxWriteTimes ;
                u32MaxWriteTimes--)
            {
                u32Data = pu32Code[u32Cnt];
                HDMIRegWriteU32((BASE_ADDR_mcu_pram  + u32Cnt*4), u32Data); 
                u32Data = HDMIRegReadU32(BASE_ADDR_mcu_pram  + u32Cnt*4);
                s32Ret = HI_SUCCESS;
            }
            
            if(!u32MaxWriteTimes)
            {
                HDMI_ERR("u32Data: %08x, g_8051Code[%d]: %08x is not equal!\n", u32Data, u32Cnt, *(pu32Code + u32Cnt));
                s32Ret = HI_FAILURE;
                break;
            }
        }
    }
#endif
    pstHdcp22->stRun.bMcuLoad = ( s32Ret == HI_SUCCESS ) ? HI_TRUE : HI_FALSE ;

    return s32Ret;
}


HI_S32 HAL_HDMI_Hdcp22AuthStatusGet(HDMI_DEVICE_ID_E enHdmiId, HDCP22_AUTH_STATUS_S *pstAuthStauts)
{
    HI_S32          s32Ret = HI_SUCCESS;
    HDCP22_INFO_S   *pstHdcp22 = HI_NULL;

    pstHdcp22 = HDCP22_InfoGet(enHdmiId);
    HDCP22_NULL_CHK(pstAuthStauts);
    HDCP22_NULL_CHK(pstHdcp22);    
    HDCP22_INIT_CHK(pstHdcp22);    

    s32Ret = HDCP22_AuthFollowGet(&(pstHdcp22->stAuth.stAuthFollow));
    HDCP22_CHK_FAILURE(s32Ret);
    
    HDMI_MEMCPY(&(pstAuthStauts->stAuthFollow), &(pstHdcp22->stAuth.stAuthFollow), sizeof(HDCP22_AUTH_FOLLOW_S));

    return HI_SUCCESS;
}


HI_S32  HAL_HDMI_Hdcp22RunStatusGet(HDMI_DEVICE_ID_E enHdmiId, HDCP22_RUN_STATUS_S *pstRunStatus)
{
    HI_S32          s32Ret = HI_SUCCESS;
    HDCP22_INFO_S  *pstHdcp22 = HI_NULL;

    pstHdcp22 = HDCP22_InfoGet(enHdmiId);
    HDCP22_NULL_CHK(pstRunStatus);
    HDCP22_NULL_CHK(pstHdcp22);
    HDCP22_INIT_CHK(pstHdcp22);
    
    HDMI_MEMCPY(pstRunStatus, &(pstHdcp22->stRun), sizeof(HDCP22_RUN_STATUS_S));

    return s32Ret;
}


HI_S32  HAL_HDMI_Hdcp22AttrSet(HDMI_DEVICE_ID_E enHdmiId, HDCP22_ATTR_S *pstAttr)
{
    HI_S32           s32Ret = HI_SUCCESS;
    HDMI_MACH_CTRL_S stMachCtrl = {0};
    HDCP22_INFO_S   *pstHdcp22 = HI_NULL;

    pstHdcp22 = HDCP22_InfoGet(enHdmiId);
    HDCP22_NULL_CHK(pstAttr);
    HDCP22_NULL_CHK(pstHdcp22);
    HDCP22_INIT_CHK(pstHdcp22); 

    HDMI_MEMCPY(&pstHdcp22->stAttr, pstAttr, sizeof(HDCP22_ATTR_S));
    
    s32Ret = HAL_HDMI_MachCfgGet(pstHdcp22->stInit.u32MachId, &stMachCtrl);
    HDCP22_CHK_FAILURE(s32Ret);

    stMachCtrl.u64Interval = (HI_U64)pstAttr->u32MachInterval;
    s32Ret = HAL_HDMI_MachCfgSet(pstHdcp22->stInit.u32MachId, &stMachCtrl);
    HDCP22_CHK_FAILURE(s32Ret);

    return s32Ret;
}


HI_S32  HAL_HDMI_Hdcp22AttrGet(HDMI_DEVICE_ID_E enHdmiId, HDCP22_ATTR_S *pstAttr)
{
    HDCP22_INFO_S *pstHdcp22 = HI_NULL;

    pstHdcp22 = HDCP22_InfoGet(enHdmiId);
    HDCP22_NULL_CHK(pstAttr);
    HDCP22_NULL_CHK(pstHdcp22);
    HDCP22_INIT_CHK(pstHdcp22);    

    HDMI_MEMCPY(pstAttr, &pstHdcp22->stAttr, sizeof(HDCP22_ATTR_S));

    return HI_SUCCESS;
}

HI_S32  HAL_HDMI_Hdcp22MachEnable(HDMI_DEVICE_ID_E enHdmiId, HI_BOOL bEnable)
{
    HI_S32         s32Ret = HI_SUCCESS;
    HDCP22_INFO_S *pstHdcp22 = HI_NULL;

    pstHdcp22 = HDCP22_InfoGet(enHdmiId);
    HDCP22_NULL_CHK(pstHdcp22);
    HDCP22_INIT_CHK(pstHdcp22);

#ifndef HDMI_TEE_SUPPORT

    if( (!pstHdcp22->stRun.bMcuLoad) && bEnable )
    {
        HDMI_ERR("un-load mcu code!\n");
        return HI_FAILURE;
    }
    
#endif

    if (bEnable)
    {
        pstHdcp22->stRun.bEnable = HI_TRUE;
        pstHdcp22->stRun.enMachStatus = HDCP22_MACH_AUTH_EN;
        s32Ret = HAL_HDMI_MachStart(pstHdcp22->stInit.u32MachId);
    }
    else
    {
        pstHdcp22->stRun.bEnable = HI_FALSE;
        pstHdcp22->stRun.enMachStatus = HDCP22_MACH_BUTT;
        s32Ret = HAL_HDMI_MachStop(pstHdcp22->stInit.u32MachId);
        HDCP22_AuthStop();
    }

    return s32Ret;
}

HI_S32  HAL_HDMI_Hdcp22Support(HDMI_DEVICE_ID_E enHdmiId, HI_BOOL *pbSupport)
{
    HI_U8           u8DdcByte = 0;
    HI_S32          s32Ret = HI_SUCCESS;
    HDCP22_INFO_S   *pstHdcp22 = HI_NULL;
    DDC_CFG_S       stDdcCfg;
    
    HDCP22_NULL_CHK(pbSupport);
    pstHdcp22 = HDCP22_InfoGet(enHdmiId);
    HDCP22_NULL_CHK(pstHdcp22);
    HDCP22_INIT_CHK(pstHdcp22);


    HAL_HDMI_DdcDefaultCfgGet(enHdmiId,&stDdcCfg);
    stDdcCfg.enFuncType     = DDC_FUNC_TYPE_HDCP;
    stDdcCfg.enIssueMode    = DDC_MODE_READ_MUTIL_NO_ACK;
    stDdcCfg.s32DataSize    = HDCP14_AN_SIZE_8BYTES;
    stDdcCfg.pu8Data        = &u8DdcByte;
    stDdcCfg.s32DataSize    = 1;
    stDdcCfg.u8Offset       = HDCP22_OFFSET_HDCP2XVERSION;
    stDdcCfg.enMasterMode   = DDC_MASTER_MODE_PWD;
    s32Ret = HAL_HDMI_DdcIssue(enHdmiId,&stDdcCfg);  
    
    if (s32Ret == stDdcCfg.s32DataSize)
    {
        *pbSupport = u8DdcByte ? HI_TRUE : HI_FALSE;
        s32Ret = HI_SUCCESS;
    }
    else
    {
        *pbSupport = HI_FALSE;
        s32Ret = HI_FAILURE;
    }
    return s32Ret;
    
}



