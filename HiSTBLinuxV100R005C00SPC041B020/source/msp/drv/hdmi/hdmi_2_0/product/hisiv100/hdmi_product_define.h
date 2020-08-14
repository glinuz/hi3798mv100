/******************************************************************************

  Copyright (C), 2014-2024, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hdmi_product_define.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2015/08/13
  Description   :
  History       :
  Date          : 2015/08/13
  Author        : t00273561
  Modification  :
*******************************************************************************/

#ifndef __HDMI_PRODUCT_DEFINE_H__
#define __HDMI_PRODUCT_DEFINE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



#include "hi_type.h"
#include "hdmi_reg_crg.h"


#ifdef HDMI_BUILD_IN_BOOT

#include "boot_hdmi_intf.h"

#ifndef HI_MINIBOOT_SUPPORT
#include <uboot.h>
#else
#include "stdio.h"
#include "string.h"
#include "boot_hdmi_intf.h"
#endif


#define HDMI_MEMSET             memset
#define HDMI_MEMCPY             memcpy


#ifdef HDMI_BUILD_IN_BOOT
#define msleep(x) do{\
    udelay(x * 1000); \
}while(0)
#endif


#endif //#ifdef HDMI_BUILD_IN_BOOT

/******************** HIFONEV300 define start *****************************/

/* HDMI TX CRG */
#define HDMI_TX_BASE_ADDR           0xf8ce0000
/* HDMI TX SUB-module */
#define HDMI_TX_BASE_ADDR_CTRL      0x0000
#define HDMI_TX_BASE_ADDR_AIDEO     0x0800
#define HDMI_TX_BASE_ADDR_AUDIO     0x1000
#define HDMI_TX_BASE_ADDR_HDMITX    0x1800
#define HDMI_TX_BASE_ADDR_HDCP1X    0x2000
#define HDMI_TX_BASE_ADDR_AON       0x4000
#define HDMI_TX_BASE_ADDR_CEC       0x4800
#define HDMI_TX_BASE_ADDR_MCU_PRAM  0x10000
#define HDMI_TX_BASE_ADDR_MCU_DRAM  0x18000
#define HDMI_TX_BASE_ADDR_MCU       0x1c000

#define BASE_ADDR_tx_ctrl_reg       (HDMI_TX_BASE_ADDR|HDMI_TX_BASE_ADDR_CTRL)
#define BASE_ADDR_video_path_reg    (HDMI_TX_BASE_ADDR|HDMI_TX_BASE_ADDR_AIDEO)
#define BASE_ADDR_audio_path_reg    (HDMI_TX_BASE_ADDR|HDMI_TX_BASE_ADDR_AUDIO)
#define BASE_ADDR_tx_hdmi_reg       (HDMI_TX_BASE_ADDR|HDMI_TX_BASE_ADDR_HDMITX)
#define BASE_ADDR_tx_hdcp_reg       (HDMI_TX_BASE_ADDR|HDMI_TX_BASE_ADDR_HDCP1X)
#define BASE_ADDR_tx_aon_reg        (HDMI_TX_BASE_ADDR|HDMI_TX_BASE_ADDR_AON)
#define BASE_ADDR_tx_cec_reg        (HDMI_TX_BASE_ADDR|HDMI_TX_BASE_ADDR_CEC)
#define BASE_ADDR_mcu_cpu_reg       (HDMI_TX_BASE_ADDR|HDMI_TX_BASE_ADDR_MCU)
#define BASE_ADDR_mcu_pram          (HDMI_TX_BASE_ADDR|HDMI_TX_BASE_ADDR_MCU_PRAM)
#define BASE_ADDR_mcu_dram          (HDMI_TX_BASE_ADDR|HDMI_TX_BASE_ADDR_MCU_DRAM)

#define BASE_ADDR_tx_dphy           0xf8d30000

/******************** HIFONEV300 define end *****************************/

typedef struct
{
    HI_U32 u32SscBypassDiv;
    HI_U32 u32TmdsClkDiv;
}HDMI_CRG_CFG_S;


HI_S32  HDMITXRegWrite(HI_U32 *pu32RegAddr, HI_U32 u32Value);
HI_U32  HDMITXRegRead(HI_U32 *pu32RegAddr);
HI_S32  HDMITXVerifyRegWrite(HI_U32 *pu32RegAddr, HI_U32 u32RegValue, HI_CHAR *pRegName);
HI_S32  HDMIRegWriteU32(HI_U32 u32RegAddr, HI_U32 u32Value);
HI_U32  HDMIRegReadU32 (HI_U32 u32RegAddr);

HI_VOID DRV_HDMI_ProdIoCfgSet(HI_VOID);

HI_VOID DRV_HDMI_ProdCrgAllResetSet(HI_BOOL bEnable);
HI_VOID DRV_HDMI_ProdCrgGateSet(HI_BOOL bEnable);
HI_VOID DRV_HDMI_ProdCrgDivSet(HDMI_CRG_CFG_S *pstCrgCfg);
HI_VOID DRV_HDMI_ProdCrgPhyResetSet(HI_BOOL bEnable);
HI_VOID DRV_HDMI_ProdCrgPhyResetGet(HI_BOOL *pbEnable);
HI_VOID DRV_HDMI_ProdCrgInit(HI_VOID);
HI_VOID DRV_HDMI_ProdCrgDeInit(HI_VOID);



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif


#endif  /* __HDMI_PRODUCT_DEFINE_H__ */


