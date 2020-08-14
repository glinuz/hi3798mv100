
/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : drv_disp_version.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/12/30
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#ifndef __DRV_DISP_VERSION_H__
#define __DRV_DISP_VERSION_H__

#include "hi_type.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */


#define DISP_DRVIER_VERSION 0x20130417UL
#define DISP_PLAT_FPGA

/* define CV200_ES */
#define DISP_CV200_ES_VERSION_L 0x76756F76UL
#define DISP_CV200_ES_VERSION_H 0x30313134UL

#define DISP_CV200_ES_BASE 0xf8cc0000UL

/*=============================================================================
BIT[   31] = reserved          <=
BIT[   30] = vou_srst_req      <= 1, soft reset
BIT[29-28] = vdp_clk_sel       <= 00,300MHz;01,400MHz; 10,345.6MHz;11,200MHz
BIT[   27] = reserved          <=
BIT[   26] = hdmi_clk_sel      <= 0, vo_sd; 1, vo_hd;
BIT[   25] = vo_clkout_pctrl   <= 0, +; 1, -
BIT[   24] = vo_clkout_sel     <= 0, sd; 1, hd
BIT[   23] = vdac_ch3_clk_sel  <= 0, sd; 1, hd
BIT[   22] = vdac_ch2_clk_sel  <= 0, sd; 1, hd
BIT[   21] = vdac_ch1_clk_sel  <= 0, sd; 1, hd
BIT[   20] = vdac_ch0_clk_sel  <= 0, sd; 1, hd
BIT[19-18] = vo_hd_clk_div     <= 00, 1/2; 01, 1/4; 1X, 1
BIT[17-16] = vo_hd_clk_sel     <= 00£ºsd_ini£»01£ºhd0_ini£»10£ºhd1_ini£»11£ºreserved
BIT[15-14] = vo_sd_clk_div     <= 00, 1/2; 01, 1/4; 1X, 1
BIT[13-12] = vo_sd_clk_sel     <= 00£ºsd_ini£»01£ºhd0_ini£»10£ºhd1_ini£»11£ºreserved
BIT[   11] = reserved          <=
BIT[   10] = vo_clkout_cken    <= 0,dis; 1, en;
BIT[    9] = vdac_ch3_cken     <= 0,dis; 1, en;
BIT[    8] = vdac_ch2_cken     <= 0,dis; 1, en;
BIT[    7] = vdac_ch1_cken     <= 0,dis; 1, en;
BIT[    6] = vdac_ch0_cken     <= 0,dis; 1, en;
BIT[    5] = vo_hdate_cken     <= 0,dis; 1, en;
BIT[    4] = vo_hd_cken        <= 0,dis; 1, en;
BIT[    3] = vo_sdate_cken     <= 0,dis; 1, en;
BIT[    2] = vo_sd_cken        <= 0,dis; 1, en;
BIT[    1] = vo_cken           <= 0,dis; 1, en;
BIT[    0] = vo_bus_cken       <= 0,dis; 1, en;
=============================================================================*/
// for hd div 1, bit[19,18] = 10b
#define DISP_CV200_ES_SYSCTRL_RESET_VALUE 0x05F147FFUL

#define DISP_CV200_ES_SYSCTRL_RESET_BIT   0x40000000UL

#define DISP_CV200_ES_PLL0_RESET_VALUE_A 0x0
#define DISP_CV200_ES_PLL0_RESET_VALUE_B 0x0

#define DISP_CV200_ES_PLL1_RESET_VALUE_A 0x0
#define DISP_CV200_ES_PLL1_RESET_VALUE_B 0x0

/*=============================================================================
BIT[31-20] = reserved         <= 
BIT[   19] = vdac_b_clk_pctrl <= 0,+; 1, -
BIT[   18] = vdac_g_clk_pctrl <= 0,+; 1, -
BIT[   17] = vdac_r_clk_pctrl <= 0,+; 1, -
BIT[   16] = vdac_c_clk_pctrl <= 0,+; 1, -
BIT[15-14] = reserved         <= 
BIT[13-12] = vdac_bg_clk_div  <= 00, 1; 01,1/2; 10, 1/3; 11, 1/4
BIT[ 11-8] = reserved         <= 
BIT[    7] = vdac_b_srst_req  <= 1, reset
BIT[    6] = vdac_g_srst_req  <= 1, reset
BIT[    5] = vdac_r_srst_req  <= 1, reset
BIT[    4] = vdac_c_srst_req  <= 1, reset
BIT[  3-2] = reserved         <= 
BIT[    1] = vdac_bg_cken     <= 0,dis; 1,en
BIT[    0] = vdac_rct_cken    <= 0,dis; 1,en
=============================================================================*/

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /*  __DRV_DISP_VERSION_H__  */


