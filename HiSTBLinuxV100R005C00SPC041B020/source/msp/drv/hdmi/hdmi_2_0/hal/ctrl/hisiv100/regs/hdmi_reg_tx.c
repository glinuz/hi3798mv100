// ****************************************************************************** 
// Copyright     :  Copyright (C) 2015, Hisilicon Technologies Co. Ltd.
// File name     :  tx_hdmi_reg_c_union_define.c
// Project line  :  Platform And Key Technologies Development
// Department    :  CAD Development Department
// Author        :  w00271848
// Version       :  1.0
// Date          :  Tester
// Description   :  The description of xxx project
// Others        :  Generated automatically by nManager V4.0.2.5 
// History       :  w00271848 2015/04/20 11:14:24 Create file
// ******************************************************************************
#include "hi_type.h"
#include "hdmi_reg_tx.h"
#include "hdmi_product_define.h"


#include "hdmi_platform.h"
#ifndef HDMI_BUILD_IN_BOOT
#include "drv_hdmi_platform.h"
#endif


/* Define the struct pointor of the module tx_hdmi_reg */
volatile S_tx_hdmi_reg_REGS_TYPE *goptx_hdmi_regAllReg = NULL;


int HDMI_TX_S_tx_hdmi_reg_REGS_TYPE_Init(void)
{
    if (goptx_hdmi_regAllReg)
    {
        return HI_SUCCESS;
    }

    goptx_hdmi_regAllReg = (volatile S_tx_hdmi_reg_REGS_TYPE*)HDMI_IO_MAP(BASE_ADDR_tx_hdmi_reg,4);

    if (!goptx_hdmi_regAllReg)
    {
        HDMI_ERR("goptx_hdmi_regAllReg = null,fail!\n");
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;
}

int HDMI_TX_S_tx_hdmi_reg_REGS_TYPE_DeInit(void)
{
    if (goptx_hdmi_regAllReg)
    {
        HDMI_IO_UNMAP(goptx_hdmi_regAllReg);
        goptx_hdmi_regAllReg = HI_NULL;
    }
    
    return HI_SUCCESS;
}


//******************************************************************************
//  Function    : HDMI_TX_PACK_FIFO_CTRL_reg_fifo_delay_cntSet
//  Description : Set the value of the member TX_PACK_FIFO_CTRL.reg_fifo_delay_cnt
//  Input       : unsigned int ureg_fifo_delay_cnt: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_PACK_FIFO_CTRL_reg_fifo_delay_cntSet(unsigned int ureg_fifo_delay_cnt)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TX_PACK_FIFO_CTRL o_tx_pack_fifo_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->TX_PACK_FIFO_CTRL.u32);
    o_tx_pack_fifo_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_pack_fifo_ctrl.bits.reg_fifo_delay_cnt = ureg_fifo_delay_cnt;
    HDMITXRegWrite(pu32RegAddr, o_tx_pack_fifo_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_pack_fifo_ctrl.u32, "TX_PACK_FIFO_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_PACK_FIFO_CTRL_tmds_rep_modeSet
//  Description : Set the value of the member TX_PACK_FIFO_CTRL.tmds_rep_mode
//  Input       : unsigned int utmds_rep_mode: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_PACK_FIFO_CTRL_reg_ext_tmds_paraSet(unsigned int ureg_ext_tmds_para)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TX_PACK_FIFO_CTRL o_tx_pack_fifo_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->TX_PACK_FIFO_CTRL.u32);
    o_tx_pack_fifo_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_pack_fifo_ctrl.bits.reg_ext_tmds_para = ureg_ext_tmds_para;
    HDMITXRegWrite(pu32RegAddr, o_tx_pack_fifo_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_pack_fifo_ctrl.u32, "TX_PACK_FIFO_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_PACK_FIFO_CTRL_reg_clock_det_enSet
//  Description : Set the value of the member TX_PACK_FIFO_CTRL.reg_clock_det_en
//  Input       : unsigned int ureg_clock_det_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_PACK_FIFO_CTRL_reg_clock_det_enSet(unsigned int ureg_clock_det_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TX_PACK_FIFO_CTRL o_tx_pack_fifo_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->TX_PACK_FIFO_CTRL.u32);
    o_tx_pack_fifo_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_pack_fifo_ctrl.bits.reg_clock_det_en = ureg_clock_det_en;
    HDMITXRegWrite(pu32RegAddr, o_tx_pack_fifo_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_pack_fifo_ctrl.u32, "TX_PACK_FIFO_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_PACK_FIFO_CTRL_reg_fifo_manu_rstSet
//  Description : Set the value of the member TX_PACK_FIFO_CTRL.reg_fifo_manu_rst
//  Input       : unsigned int ureg_fifo_manu_rst: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_PACK_FIFO_CTRL_reg_fifo_manu_rstSet(unsigned int ureg_fifo_manu_rst)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TX_PACK_FIFO_CTRL o_tx_pack_fifo_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->TX_PACK_FIFO_CTRL.u32);
    o_tx_pack_fifo_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_pack_fifo_ctrl.bits.reg_fifo_manu_rst = ureg_fifo_manu_rst;
    HDMITXRegWrite(pu32RegAddr, o_tx_pack_fifo_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_pack_fifo_ctrl.u32, "TX_PACK_FIFO_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_PACK_FIFO_CTRL_reg_fifo_auto_rst_enSet
//  Description : Set the value of the member TX_PACK_FIFO_CTRL.reg_fifo_auto_rst_en
//  Input       : unsigned int ureg_fifo_auto_rst_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_PACK_FIFO_CTRL_reg_fifo_auto_rst_enSet(unsigned int ureg_fifo_auto_rst_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TX_PACK_FIFO_CTRL o_tx_pack_fifo_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->TX_PACK_FIFO_CTRL.u32);
    o_tx_pack_fifo_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_pack_fifo_ctrl.bits.reg_fifo_auto_rst_en = ureg_fifo_auto_rst_en;
    HDMITXRegWrite(pu32RegAddr, o_tx_pack_fifo_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_pack_fifo_ctrl.u32, "TX_PACK_FIFO_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_PACK_FIFO_CTRL_tmds_pack_modeSet
//  Description : Set the value of the member TX_PACK_FIFO_CTRL.tmds_pack_mode
//  Input       : unsigned int utmds_pack_mode: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_PACK_FIFO_CTRL_tmds_pack_modeSet(unsigned int utmds_pack_mode)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TX_PACK_FIFO_CTRL o_tx_pack_fifo_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->TX_PACK_FIFO_CTRL.u32);
    o_tx_pack_fifo_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_pack_fifo_ctrl.bits.tmds_pack_mode = utmds_pack_mode;
    HDMITXRegWrite(pu32RegAddr, o_tx_pack_fifo_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_pack_fifo_ctrl.u32, "TX_PACK_FIFO_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_PACK_FIFO_ST_pclk2tclk_stableSet
//  Description : Set the value of the member TX_PACK_FIFO_ST.pclk2tclk_stable
//  Input       : unsigned int upclk2tclk_stable: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_PACK_FIFO_ST_pclk2tclk_stableSet(unsigned int upclk2tclk_stable)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TX_PACK_FIFO_ST o_tx_pack_fifo_st;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->TX_PACK_FIFO_ST.u32);
    o_tx_pack_fifo_st.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_pack_fifo_st.bits.pclk2tclk_stable = upclk2tclk_stable;
    HDMITXRegWrite(pu32RegAddr, o_tx_pack_fifo_st.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_pack_fifo_st.u32, "TX_PACK_FIFO_ST");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PCLK_REFER_CNT_reg_pclk_refer_cntSet
//  Description : Set the value of the member PCLK_REFER_CNT.reg_pclk_refer_cnt
//  Input       : unsigned int ureg_pclk_refer_cnt: 18 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PCLK_REFER_CNT_reg_pclk_refer_cntSet(unsigned int ureg_pclk_refer_cnt)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PCLK_REFER_CNT o_pclk_refer_cnt;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->PCLK_REFER_CNT.u32);
    o_pclk_refer_cnt.u32 = HDMITXRegRead(pu32RegAddr);
    o_pclk_refer_cnt.bits.reg_pclk_refer_cnt = ureg_pclk_refer_cnt;
    HDMITXRegWrite(pu32RegAddr, o_pclk_refer_cnt.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pclk_refer_cnt.u32, "PCLK_REFER_CNT");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TCLK_LOWER_THRESHOLD_reg_tcnt_lower_thresholdSet
//  Description : Set the value of the member TCLK_LOWER_THRESHOLD.reg_tcnt_lower_threshold
//  Input       : unsigned int ureg_tcnt_lower_threshold: 18 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TCLK_LOWER_THRESHOLD_reg_tcnt_lower_thresholdSet(unsigned int ureg_tcnt_lower_threshold)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TCLK_LOWER_THRESHOLD o_tclk_lower_threshold;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->TCLK_LOWER_THRESHOLD.u32);
    o_tclk_lower_threshold.u32 = HDMITXRegRead(pu32RegAddr);
    o_tclk_lower_threshold.bits.reg_tcnt_lower_threshold = ureg_tcnt_lower_threshold;
    HDMITXRegWrite(pu32RegAddr, o_tclk_lower_threshold.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tclk_lower_threshold.u32, "TCLK_LOWER_THRESHOLD");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TCLK_UPPER_THRESHOLD_reg_tcnt_upper_thresholdSet
//  Description : Set the value of the member TCLK_UPPER_THRESHOLD.reg_tcnt_upper_threshold
//  Input       : unsigned int ureg_tcnt_upper_threshold: 18 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TCLK_UPPER_THRESHOLD_reg_tcnt_upper_thresholdSet(unsigned int ureg_tcnt_upper_threshold)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TCLK_UPPER_THRESHOLD o_tclk_upper_threshold;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->TCLK_UPPER_THRESHOLD.u32);
    o_tclk_upper_threshold.u32 = HDMITXRegRead(pu32RegAddr);
    o_tclk_upper_threshold.bits.reg_tcnt_upper_threshold = ureg_tcnt_upper_threshold;
    HDMITXRegWrite(pu32RegAddr, o_tclk_upper_threshold.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tclk_upper_threshold.u32, "TCLK_UPPER_THRESHOLD");

    return HI_SUCCESS;
}


int HDMI_AVI_PKT_HEADER_hbSet(unsigned int uhb0,unsigned int uhb1,unsigned int uhb2)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVI_PKT_HEADER o_avi_pkt_header;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVI_PKT_HEADER.u32);
    o_avi_pkt_header.u32 = HDMITXRegRead(pu32RegAddr);
    o_avi_pkt_header.bits.avi_pkt_hb2 = uhb2;
    o_avi_pkt_header.bits.avi_pkt_hb1 = uhb1;
    o_avi_pkt_header.bits.avi_pkt_hb0 = uhb0;
    HDMITXRegWrite(pu32RegAddr, o_avi_pkt_header.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_avi_pkt_header.u32, "AVI_PKT_HEADER");
    return HI_SUCCESS;
}

int HDMI_AVI_SUB_PKT0_L_avi_pkt0_pbSet(unsigned int uavi_pkt0_pb0,unsigned int uavi_pkt0_pb1,unsigned int uavi_pkt0_pb2,unsigned int uavi_pkt0_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVI_SUB_PKT0_L o_avi_sub_pkt0_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVI_SUB_PKT0_L.u32);
    o_avi_sub_pkt0_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_avi_sub_pkt0_l.bits.avi_sub_pkt0_pb3 = uavi_pkt0_pb3;
    o_avi_sub_pkt0_l.bits.avi_sub_pkt0_pb2 = uavi_pkt0_pb2;
    o_avi_sub_pkt0_l.bits.avi_sub_pkt0_pb1 = uavi_pkt0_pb1;
    o_avi_sub_pkt0_l.bits.avi_sub_pkt0_pb0 = uavi_pkt0_pb0;
    HDMITXRegWrite(pu32RegAddr, o_avi_sub_pkt0_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_avi_sub_pkt0_l.u32, "AVI_SUB_PKT0_L");

    return HI_SUCCESS;
}

int HDMI_AVI_SUB_PKT0_H_avi_pkt0_pbSet(unsigned int uavi_pkt0_pb4,unsigned int uavi_pkt0_pb5,unsigned int uavi_pkt0_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVI_SUB_PKT0_H o_avi_sub_pkt0_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVI_SUB_PKT0_H.u32);
    o_avi_sub_pkt0_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_avi_sub_pkt0_h.bits.avi_sub_pkt0_pb6 = uavi_pkt0_pb6;
    o_avi_sub_pkt0_h.bits.avi_sub_pkt0_pb5 = uavi_pkt0_pb5;
    o_avi_sub_pkt0_h.bits.avi_sub_pkt0_pb4 = uavi_pkt0_pb4;
    HDMITXRegWrite(pu32RegAddr, o_avi_sub_pkt0_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_avi_sub_pkt0_h.u32, "AVI_SUB_PKT0_H");

    return HI_SUCCESS;
}

int HDMI_AVI_SUB_PKT1_L_avi_pkt1_pbSet(unsigned int uavi_pkt1_pb0,unsigned int uavi_pkt1_pb1,unsigned int uavi_pkt1_pb2,unsigned int uavi_pkt1_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVI_SUB_PKT1_L o_avi_sub_pkt1_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVI_SUB_PKT1_L.u32);
    o_avi_sub_pkt1_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_avi_sub_pkt1_l.bits.avi_sub_pkt1_pb3 = uavi_pkt1_pb3;
    o_avi_sub_pkt1_l.bits.avi_sub_pkt1_pb2 = uavi_pkt1_pb2;
    o_avi_sub_pkt1_l.bits.avi_sub_pkt1_pb1 = uavi_pkt1_pb1;
    o_avi_sub_pkt1_l.bits.avi_sub_pkt1_pb0 = uavi_pkt1_pb0;
    HDMITXRegWrite(pu32RegAddr, o_avi_sub_pkt1_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_avi_sub_pkt1_l.u32, "AVI_SUB_PKT1_L");

    return HI_SUCCESS;
}

int HDMI_AVI_SUB_PKT1_H_avi_pkt1_pbSet(unsigned int uavi_pkt1_pb4,unsigned int uavi_pkt1_pb5,unsigned int uavi_pkt1_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVI_SUB_PKT1_H o_avi_sub_pkt1_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVI_SUB_PKT1_H.u32);
    o_avi_sub_pkt1_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_avi_sub_pkt1_h.bits.avi_sub_pkt1_pb6 = uavi_pkt1_pb6;
    o_avi_sub_pkt1_h.bits.avi_sub_pkt1_pb5 = uavi_pkt1_pb5;
    o_avi_sub_pkt1_h.bits.avi_sub_pkt1_pb4 = uavi_pkt1_pb4;
    HDMITXRegWrite(pu32RegAddr, o_avi_sub_pkt1_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_avi_sub_pkt1_h.u32, "AVI_SUB_PKT1_H");

    return HI_SUCCESS;
}

int HDMI_AVI_SUB_PKT2_L_avi_pkt2_pbSet(unsigned int uavi_pkt2_pb0,unsigned int uavi_pkt2_pb1,unsigned int uavi_pkt2_pb2,unsigned int uavi_pkt2_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVI_SUB_PKT2_L o_avi_sub_pkt2_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVI_SUB_PKT2_L.u32);
    o_avi_sub_pkt2_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_avi_sub_pkt2_l.bits.avi_sub_pkt2_pb3 = uavi_pkt2_pb3;
    o_avi_sub_pkt2_l.bits.avi_sub_pkt2_pb2 = uavi_pkt2_pb2;
    o_avi_sub_pkt2_l.bits.avi_sub_pkt2_pb1 = uavi_pkt2_pb1;
    o_avi_sub_pkt2_l.bits.avi_sub_pkt2_pb0 = uavi_pkt2_pb0;
    HDMITXRegWrite(pu32RegAddr, o_avi_sub_pkt2_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_avi_sub_pkt2_l.u32, "AVI_SUB_PKT2_L");

    return HI_SUCCESS;
}

int HDMI_AVI_SUB_PKT2_H_avi_pkt2_pbSet(unsigned int uavi_pkt2_pb4,unsigned int uavi_pkt2_pb5,unsigned int uavi_pkt2_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVI_SUB_PKT2_H o_avi_sub_pkt2_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVI_SUB_PKT2_H.u32);
    o_avi_sub_pkt2_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_avi_sub_pkt2_h.bits.avi_sub_pkt2_pb6 = uavi_pkt2_pb6;
    o_avi_sub_pkt2_h.bits.avi_sub_pkt2_pb5 = uavi_pkt2_pb5;
    o_avi_sub_pkt2_h.bits.avi_sub_pkt2_pb4 = uavi_pkt2_pb4;
    HDMITXRegWrite(pu32RegAddr, o_avi_sub_pkt2_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_avi_sub_pkt2_h.u32, "AVI_SUB_PKT2_H");

    return HI_SUCCESS;
}

int HDMI_AVI_SUB_PKT3_L_avi_pkt3_pbSet(unsigned int uavi_pkt3_pb0,unsigned int uavi_pkt3_pb1,unsigned int uavi_pkt3_pb2,unsigned int uavi_pkt3_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVI_SUB_PKT3_L o_avi_sub_pkt3_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVI_SUB_PKT3_L.u32);
    o_avi_sub_pkt3_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_avi_sub_pkt3_l.bits.avi_sub_pkt3_pb3 = uavi_pkt3_pb3;
    o_avi_sub_pkt3_l.bits.avi_sub_pkt3_pb2 = uavi_pkt3_pb2;
    o_avi_sub_pkt3_l.bits.avi_sub_pkt3_pb1 = uavi_pkt3_pb1;
    o_avi_sub_pkt3_l.bits.avi_sub_pkt3_pb0 = uavi_pkt3_pb0;
    HDMITXRegWrite(pu32RegAddr, o_avi_sub_pkt3_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_avi_sub_pkt3_l.u32, "AVI_SUB_PKT3_L");

    return HI_SUCCESS;
}

int HDMI_AVI_SUB_PKT3_H_avi_pkt3_pbSet(unsigned int uavi_pkt3_pb4,unsigned int uavi_pkt3_pb5,unsigned int uavi_pkt3_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVI_SUB_PKT3_H o_avi_sub_pkt3_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVI_SUB_PKT3_H.u32);
    o_avi_sub_pkt3_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_avi_sub_pkt3_h.bits.avi_sub_pkt3_pb6 = uavi_pkt3_pb6;
    o_avi_sub_pkt3_h.bits.avi_sub_pkt3_pb5 = uavi_pkt3_pb5;
    o_avi_sub_pkt3_h.bits.avi_sub_pkt3_pb4 = uavi_pkt3_pb4;
    HDMITXRegWrite(pu32RegAddr, o_avi_sub_pkt3_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_avi_sub_pkt3_h.u32, "AVI_SUB_PKT3_H");

    return HI_SUCCESS;
}

//*************************************************************************************//
//******************************** Avi 信息帧获取*******************************//
//*************************************************************************************//
int HDMI_AVI_PKT_HEADER_hbGet(U_AVI_PKT_HEADER* u_avi_pkt_header)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVI_PKT_HEADER o_avi_pkt_header;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVI_PKT_HEADER.u32);
    o_avi_pkt_header.u32 = HDMITXRegRead(pu32RegAddr);
    u_avi_pkt_header->bits.avi_pkt_hb2 = o_avi_pkt_header.bits.avi_pkt_hb2;
    u_avi_pkt_header->bits.avi_pkt_hb1 = o_avi_pkt_header.bits.avi_pkt_hb1;
    u_avi_pkt_header->bits.avi_pkt_hb0 = o_avi_pkt_header.bits.avi_pkt_hb0;
    return HI_SUCCESS;
}

int HDMI_AVI_SUB_PKT0_L_avi_pkt0_pbGet(U_AVI_SUB_PKT0_L* u_avi_sub_pkt0_l)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVI_SUB_PKT0_L o_avi_sub_pkt0_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVI_SUB_PKT0_L.u32);
    o_avi_sub_pkt0_l.u32 = HDMITXRegRead(pu32RegAddr);
    u_avi_sub_pkt0_l->bits.avi_sub_pkt0_pb3 = o_avi_sub_pkt0_l.bits.avi_sub_pkt0_pb3;
    u_avi_sub_pkt0_l->bits.avi_sub_pkt0_pb2 = o_avi_sub_pkt0_l.bits.avi_sub_pkt0_pb2;
    u_avi_sub_pkt0_l->bits.avi_sub_pkt0_pb1 = o_avi_sub_pkt0_l.bits.avi_sub_pkt0_pb1;
    u_avi_sub_pkt0_l->bits.avi_sub_pkt0_pb0 = o_avi_sub_pkt0_l.bits.avi_sub_pkt0_pb0;
	return HI_SUCCESS;
}

int HDMI_AVI_SUB_PKT0_H_avi_pkt0_pbGet(U_AVI_SUB_PKT0_H* u_avi_sub_pkt0_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVI_SUB_PKT0_H o_avi_sub_pkt0_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVI_SUB_PKT0_H.u32);
    o_avi_sub_pkt0_h.u32 = HDMITXRegRead(pu32RegAddr);
    u_avi_sub_pkt0_h->bits.avi_sub_pkt0_pb6 = o_avi_sub_pkt0_h.bits.avi_sub_pkt0_pb6;
    u_avi_sub_pkt0_h->bits.avi_sub_pkt0_pb5 = o_avi_sub_pkt0_h.bits.avi_sub_pkt0_pb5;
    u_avi_sub_pkt0_h->bits.avi_sub_pkt0_pb4 = o_avi_sub_pkt0_h.bits.avi_sub_pkt0_pb4;

	return HI_SUCCESS;
}

int HDMI_AVI_SUB_PKT1_L_avi_pkt1_pbGet(U_AVI_SUB_PKT1_L* u_avi_sub_pkt1_l)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVI_SUB_PKT1_L o_avi_sub_pkt1_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVI_SUB_PKT1_L.u32);
    o_avi_sub_pkt1_l.u32 = HDMITXRegRead(pu32RegAddr);
    u_avi_sub_pkt1_l->bits.avi_sub_pkt1_pb3 = o_avi_sub_pkt1_l.bits.avi_sub_pkt1_pb3;
    u_avi_sub_pkt1_l->bits.avi_sub_pkt1_pb2 = o_avi_sub_pkt1_l.bits.avi_sub_pkt1_pb2;
    u_avi_sub_pkt1_l->bits.avi_sub_pkt1_pb1 = o_avi_sub_pkt1_l.bits.avi_sub_pkt1_pb1;
    u_avi_sub_pkt1_l->bits.avi_sub_pkt1_pb0 = o_avi_sub_pkt1_l.bits.avi_sub_pkt1_pb0;

	return HI_SUCCESS;
}

int HDMI_AVI_SUB_PKT1_H_avi_pkt1_pbGet(U_AVI_SUB_PKT1_H* u_avi_sub_pkt1_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVI_SUB_PKT1_H o_avi_sub_pkt1_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVI_SUB_PKT1_H.u32);
    o_avi_sub_pkt1_h.u32 = HDMITXRegRead(pu32RegAddr);
    u_avi_sub_pkt1_h->bits.avi_sub_pkt1_pb6 = o_avi_sub_pkt1_h.bits.avi_sub_pkt1_pb6;
    u_avi_sub_pkt1_h->bits.avi_sub_pkt1_pb5 = o_avi_sub_pkt1_h.bits.avi_sub_pkt1_pb5;
    u_avi_sub_pkt1_h->bits.avi_sub_pkt1_pb4 = o_avi_sub_pkt1_h.bits.avi_sub_pkt1_pb4;

	return HI_SUCCESS;	
}

int HDMI_AVI_SUB_PKT2_L_avi_pkt2_pbGet(U_AVI_SUB_PKT2_L* u_avi_sub_pkt2_l)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVI_SUB_PKT2_L o_avi_sub_pkt2_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVI_SUB_PKT2_L.u32);
    o_avi_sub_pkt2_l.u32 = HDMITXRegRead(pu32RegAddr);
    u_avi_sub_pkt2_l->bits.avi_sub_pkt2_pb3 = o_avi_sub_pkt2_l.bits.avi_sub_pkt2_pb3;
    u_avi_sub_pkt2_l->bits.avi_sub_pkt2_pb2 = o_avi_sub_pkt2_l.bits.avi_sub_pkt2_pb2;
    u_avi_sub_pkt2_l->bits.avi_sub_pkt2_pb1 = o_avi_sub_pkt2_l.bits.avi_sub_pkt2_pb1;
    u_avi_sub_pkt2_l->bits.avi_sub_pkt2_pb0 = o_avi_sub_pkt2_l.bits.avi_sub_pkt2_pb0;

	return HI_SUCCESS;
}

int HDMI_AVI_SUB_PKT2_H_avi_pkt2_pbGet(U_AVI_SUB_PKT2_H* u_avi_sub_pkt2_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVI_SUB_PKT2_H o_avi_sub_pkt2_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVI_SUB_PKT2_H.u32);
    o_avi_sub_pkt2_h.u32 = HDMITXRegRead(pu32RegAddr);
    u_avi_sub_pkt2_h->bits.avi_sub_pkt2_pb6 = o_avi_sub_pkt2_h.bits.avi_sub_pkt2_pb6;
    u_avi_sub_pkt2_h->bits.avi_sub_pkt2_pb5 = o_avi_sub_pkt2_h.bits.avi_sub_pkt2_pb5;
    u_avi_sub_pkt2_h->bits.avi_sub_pkt2_pb4 = o_avi_sub_pkt2_h.bits.avi_sub_pkt2_pb4;
	return HI_SUCCESS;

}

int HDMI_AVI_SUB_PKT3_L_avi_pkt3_pbGet(U_AVI_SUB_PKT3_L* u_avi_sub_pkt3_l)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVI_SUB_PKT3_L o_avi_sub_pkt3_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVI_SUB_PKT3_L.u32);
    o_avi_sub_pkt3_l.u32 = HDMITXRegRead(pu32RegAddr);
    u_avi_sub_pkt3_l->bits.avi_sub_pkt3_pb3 = o_avi_sub_pkt3_l.bits.avi_sub_pkt3_pb3;
    u_avi_sub_pkt3_l->bits.avi_sub_pkt3_pb2 = o_avi_sub_pkt3_l.bits.avi_sub_pkt3_pb2;
    u_avi_sub_pkt3_l->bits.avi_sub_pkt3_pb1 = o_avi_sub_pkt3_l.bits.avi_sub_pkt3_pb1;
    u_avi_sub_pkt3_l->bits.avi_sub_pkt3_pb0 = o_avi_sub_pkt3_l.bits.avi_sub_pkt3_pb0;
    return HI_SUCCESS;
}

int HDMI_AVI_SUB_PKT3_H_avi_pkt3_pbGet(U_AVI_SUB_PKT3_H* u_avi_sub_pkt3_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVI_SUB_PKT3_H o_avi_sub_pkt3_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVI_SUB_PKT3_H.u32);
    o_avi_sub_pkt3_h.u32 = HDMITXRegRead(pu32RegAddr);
    u_avi_sub_pkt3_h->bits.avi_sub_pkt3_pb6 = o_avi_sub_pkt3_h.bits.avi_sub_pkt3_pb6;
    u_avi_sub_pkt3_h->bits.avi_sub_pkt3_pb5 = o_avi_sub_pkt3_h.bits.avi_sub_pkt3_pb5;
    u_avi_sub_pkt3_h->bits.avi_sub_pkt3_pb4 = o_avi_sub_pkt3_h.bits.avi_sub_pkt3_pb4;
    return HI_SUCCESS;
}

//*************************************************************************************//
//******************************** Audio 信息帧配置*******************************//
//*************************************************************************************//

int HDMI_AUDIO_PKT_HEADER_hbSet(unsigned int uhb0,unsigned int uhb1,unsigned int uhb2)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AIF_PKT_HEADER o_audio_pkt_header;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AIF_PKT_HEADER.u32);
    o_audio_pkt_header.u32 = HDMITXRegRead(pu32RegAddr);
    o_audio_pkt_header.bits.aif_pkt_hb2= uhb2;
    o_audio_pkt_header.bits.aif_pkt_hb1= uhb1;
    o_audio_pkt_header.bits.aif_pkt_hb0= uhb0;
    HDMITXRegWrite(pu32RegAddr, o_audio_pkt_header.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_audio_pkt_header.u32, "AIF_PKT_HEADER");

    return HI_SUCCESS;
}

int HDMI_AUDIO_SUB_PKT0_L_audio_pkt0_pbSet(unsigned int uaudio_pkt0_pb0,unsigned int uaudio_pkt0_pb1,unsigned int uaudio_pkt0_pb2,unsigned int uaudio_pkt0_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AIF_SUB_PKT0_L o_audio_sub_pkt0_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AIF_SUB_PKT0_L.u32);
    o_audio_sub_pkt0_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_audio_sub_pkt0_l.bits.aif_sub_pkt0_pb3 = uaudio_pkt0_pb3;
    o_audio_sub_pkt0_l.bits.aif_sub_pkt0_pb2 = uaudio_pkt0_pb2;
    o_audio_sub_pkt0_l.bits.aif_sub_pkt0_pb1 = uaudio_pkt0_pb1;
    o_audio_sub_pkt0_l.bits.aif_sub_pkt0_pb0 = uaudio_pkt0_pb0;
    HDMITXRegWrite(pu32RegAddr, o_audio_sub_pkt0_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_audio_sub_pkt0_l.u32, "AIF_SUB_PKT0_L");

    return HI_SUCCESS;
}

int HDMI_AUDIO_SUB_PKT0_H_audio_pkt0_pbSet(unsigned int uaudio_pkt0_pb4,unsigned int uaudio_pkt0_pb5,unsigned int uaudio_pkt0_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AIF_SUB_PKT0_H o_audio_sub_pkt0_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AIF_SUB_PKT0_H.u32);
    o_audio_sub_pkt0_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_audio_sub_pkt0_h.bits.aif_sub_pkt0_pb6 = uaudio_pkt0_pb6;
    o_audio_sub_pkt0_h.bits.aif_sub_pkt0_pb5 = uaudio_pkt0_pb5;
    o_audio_sub_pkt0_h.bits.aif_sub_pkt0_pb4 = uaudio_pkt0_pb4;
    HDMITXRegWrite(pu32RegAddr, o_audio_sub_pkt0_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_audio_sub_pkt0_h.u32, "AIF_SUB_PKT0_H");

    return HI_SUCCESS;
}

int HDMI_AUDIO_SUB_PKT1_L_audio_pkt1_pbSet(unsigned int uaudio_pkt1_pb0,unsigned int uaudio_pkt1_pb1,unsigned int uaudio_pkt1_pb2,unsigned int uaudio_pkt1_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AIF_SUB_PKT1_L o_audio_sub_pkt1_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AIF_SUB_PKT1_L.u32);
    o_audio_sub_pkt1_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_audio_sub_pkt1_l.bits.aif_sub_pkt1_pb3 = uaudio_pkt1_pb3;
    o_audio_sub_pkt1_l.bits.aif_sub_pkt1_pb2 = uaudio_pkt1_pb2;
    o_audio_sub_pkt1_l.bits.aif_sub_pkt1_pb1 = uaudio_pkt1_pb1;
    o_audio_sub_pkt1_l.bits.aif_sub_pkt1_pb0 = uaudio_pkt1_pb0;
    HDMITXRegWrite(pu32RegAddr, o_audio_sub_pkt1_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_audio_sub_pkt1_l.u32, "AIF_SUB_PKT1_L");

    return HI_SUCCESS;
}

int HDMI_AUDIO_SUB_PKT1_H_audio_pkt1_pbSet(unsigned int uaudio_pkt1_pb4,unsigned int uaudio_pkt1_pb5,unsigned int uaudio_pkt1_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AIF_SUB_PKT1_H o_audio_sub_pkt1_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AIF_SUB_PKT1_H.u32);
    o_audio_sub_pkt1_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_audio_sub_pkt1_h.bits.aif_sub_pkt1_pb6 = uaudio_pkt1_pb6;
    o_audio_sub_pkt1_h.bits.aif_sub_pkt1_pb5 = uaudio_pkt1_pb5;
    o_audio_sub_pkt1_h.bits.aif_sub_pkt1_pb4 = uaudio_pkt1_pb4;
    HDMITXRegWrite(pu32RegAddr, o_audio_sub_pkt1_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_audio_sub_pkt1_h.u32, "AIF_SUB_PKT1_H");

    return HI_SUCCESS;
}

int HDMI_AUDIO_SUB_PKT2_L_audio_pkt2_pbSet(unsigned int uaudio_pkt2_pb0,unsigned int uaudio_pkt2_pb1,unsigned int uaudio_pkt2_pb2,unsigned int uaudio_pkt2_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AIF_SUB_PKT2_L o_audio_sub_pkt2_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AIF_SUB_PKT2_L.u32);
    o_audio_sub_pkt2_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_audio_sub_pkt2_l.bits.aif_sub_pkt2_pb3 = uaudio_pkt2_pb3;
    o_audio_sub_pkt2_l.bits.aif_sub_pkt2_pb2 = uaudio_pkt2_pb2;
    o_audio_sub_pkt2_l.bits.aif_sub_pkt2_pb1 = uaudio_pkt2_pb1;
    o_audio_sub_pkt2_l.bits.aif_sub_pkt2_pb0 = uaudio_pkt2_pb0;
    HDMITXRegWrite(pu32RegAddr, o_audio_sub_pkt2_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_audio_sub_pkt2_l.u32, "AIF_SUB_PKT2_L");

    return HI_SUCCESS;
}

int HDMI_AUDIO_SUB_PKT2_H_audio_pkt2_pbSet(unsigned int uaudio_pkt2_pb4,unsigned int uaudio_pkt2_pb5,unsigned int uaudio_pkt2_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AIF_SUB_PKT2_H o_audio_sub_pkt2_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AIF_SUB_PKT2_H.u32);
    o_audio_sub_pkt2_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_audio_sub_pkt2_h.bits.aif_sub_pkt2_pb6 = uaudio_pkt2_pb6;
    o_audio_sub_pkt2_h.bits.aif_sub_pkt2_pb5 = uaudio_pkt2_pb5;
    o_audio_sub_pkt2_h.bits.aif_sub_pkt2_pb4 = uaudio_pkt2_pb4;
    HDMITXRegWrite(pu32RegAddr, o_audio_sub_pkt2_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_audio_sub_pkt2_h.u32, "AIF_SUB_PKT2_H");

    return HI_SUCCESS;
}

int HDMI_AUDIO_SUB_PKT3_L_audio_pkt3_pbSet(unsigned int uaudio_pkt3_pb0,unsigned int uaudio_pkt3_pb1,unsigned int uaudio_pkt3_pb2,unsigned int uaudio_pkt3_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AIF_SUB_PKT3_L o_audio_sub_pkt3_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AIF_SUB_PKT3_L.u32);
    o_audio_sub_pkt3_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_audio_sub_pkt3_l.bits.aif_sub_pkt3_pb3 = uaudio_pkt3_pb3;
    o_audio_sub_pkt3_l.bits.aif_sub_pkt3_pb2 = uaudio_pkt3_pb2;
    o_audio_sub_pkt3_l.bits.aif_sub_pkt3_pb1 = uaudio_pkt3_pb1;
    o_audio_sub_pkt3_l.bits.aif_sub_pkt3_pb0 = uaudio_pkt3_pb0;
    HDMITXRegWrite(pu32RegAddr, o_audio_sub_pkt3_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_audio_sub_pkt3_l.u32, "AIF_SUB_PKT3_L");

    return HI_SUCCESS;
}

int HDMI_AUDIO_SUB_PKT3_H_audio_pkt3_pbSet(unsigned int uaudio_pkt3_pb4,unsigned int uaudio_pkt3_pb5,unsigned int uaudio_pkt3_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AIF_SUB_PKT3_H o_audio_sub_pkt3_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AIF_SUB_PKT3_H.u32);
    o_audio_sub_pkt3_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_audio_sub_pkt3_h.bits.aif_sub_pkt3_pb6 = uaudio_pkt3_pb6;
    o_audio_sub_pkt3_h.bits.aif_sub_pkt3_pb5 = uaudio_pkt3_pb5;
    o_audio_sub_pkt3_h.bits.aif_sub_pkt3_pb4 = uaudio_pkt3_pb4;
    HDMITXRegWrite(pu32RegAddr, o_audio_sub_pkt3_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_audio_sub_pkt3_h.u32, "AIF_SUB_PKT3_H");

    return HI_SUCCESS;
}


//*************************************************************************************//
//******************************** Audio 信息帧获取*******************************//
//*************************************************************************************//
int HDMI_AIF_PKT_HEADER_hbGet(U_AIF_PKT_HEADER* u_aif_pkt_header)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AIF_PKT_HEADER o_aif_pkt_header;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AIF_PKT_HEADER.u32);
    o_aif_pkt_header.u32 = HDMITXRegRead(pu32RegAddr);
    u_aif_pkt_header->bits.aif_pkt_hb2 = o_aif_pkt_header.bits.aif_pkt_hb2;
    u_aif_pkt_header->bits.aif_pkt_hb1 = o_aif_pkt_header.bits.aif_pkt_hb1;
    u_aif_pkt_header->bits.aif_pkt_hb0 = o_aif_pkt_header.bits.aif_pkt_hb0;
    return HI_SUCCESS;
}

int HDMI_AIF_SUB_PKT0_L_aif_pkt0_pbGet(U_AIF_SUB_PKT0_L* u_aif_sub_pkt0_l)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AIF_SUB_PKT0_L o_aif_sub_pkt0_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AIF_SUB_PKT0_L.u32);
    o_aif_sub_pkt0_l.u32 = HDMITXRegRead(pu32RegAddr);
    u_aif_sub_pkt0_l->bits.aif_sub_pkt0_pb3 = o_aif_sub_pkt0_l.bits.aif_sub_pkt0_pb3;
    u_aif_sub_pkt0_l->bits.aif_sub_pkt0_pb2 = o_aif_sub_pkt0_l.bits.aif_sub_pkt0_pb2;
    u_aif_sub_pkt0_l->bits.aif_sub_pkt0_pb1 = o_aif_sub_pkt0_l.bits.aif_sub_pkt0_pb1;
    u_aif_sub_pkt0_l->bits.aif_sub_pkt0_pb0 = o_aif_sub_pkt0_l.bits.aif_sub_pkt0_pb0;

    return HI_SUCCESS;
}

int HDMI_AIF_SUB_PKT0_H_aif_pkt0_pbGet(U_AIF_SUB_PKT0_H* u_aif_sub_pkt0_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AIF_SUB_PKT0_H o_aif_sub_pkt0_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AIF_SUB_PKT0_H.u32);
    o_aif_sub_pkt0_h.u32 = HDMITXRegRead(pu32RegAddr);
    u_aif_sub_pkt0_h->bits.aif_sub_pkt0_pb6 = o_aif_sub_pkt0_h.bits.aif_sub_pkt0_pb6;
    u_aif_sub_pkt0_h->bits.aif_sub_pkt0_pb5 = o_aif_sub_pkt0_h.bits.aif_sub_pkt0_pb5;
    u_aif_sub_pkt0_h->bits.aif_sub_pkt0_pb4 = o_aif_sub_pkt0_h.bits.aif_sub_pkt0_pb4;

    return HI_SUCCESS;
}

int HDMI_AIF_SUB_PKT1_L_aif_pkt1_pbGet(U_AIF_SUB_PKT1_L* u_aif_sub_pkt1_l)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AIF_SUB_PKT1_L o_aif_sub_pkt1_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AIF_SUB_PKT1_L.u32);
    o_aif_sub_pkt1_l.u32 = HDMITXRegRead(pu32RegAddr);
    u_aif_sub_pkt1_l->bits.aif_sub_pkt1_pb3 = o_aif_sub_pkt1_l.bits.aif_sub_pkt1_pb3;
    u_aif_sub_pkt1_l->bits.aif_sub_pkt1_pb2 = o_aif_sub_pkt1_l.bits.aif_sub_pkt1_pb2;
    u_aif_sub_pkt1_l->bits.aif_sub_pkt1_pb1 = o_aif_sub_pkt1_l.bits.aif_sub_pkt1_pb1;
    u_aif_sub_pkt1_l->bits.aif_sub_pkt1_pb0 = o_aif_sub_pkt1_l.bits.aif_sub_pkt1_pb0;

    return HI_SUCCESS;
}

int HDMI_AIF_SUB_PKT1_H_aif_pkt1_pbGet(U_AIF_SUB_PKT1_H* u_aif_sub_pkt1_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AIF_SUB_PKT1_H o_aif_sub_pkt1_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AIF_SUB_PKT1_H.u32);
    o_aif_sub_pkt1_h.u32 = HDMITXRegRead(pu32RegAddr);
    u_aif_sub_pkt1_h->bits.aif_sub_pkt1_pb6 = o_aif_sub_pkt1_h.bits.aif_sub_pkt1_pb6;
    u_aif_sub_pkt1_h->bits.aif_sub_pkt1_pb5 = o_aif_sub_pkt1_h.bits.aif_sub_pkt1_pb5;
    u_aif_sub_pkt1_h->bits.aif_sub_pkt1_pb4 = o_aif_sub_pkt1_h.bits.aif_sub_pkt1_pb4;

    return HI_SUCCESS;
}

int HDMI_AIF_SUB_PKT2_L_aif_pkt2_pbGet(U_AIF_SUB_PKT2_L* u_aif_sub_pkt2_l)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AIF_SUB_PKT2_L o_aif_sub_pkt2_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AIF_SUB_PKT2_L.u32);
    o_aif_sub_pkt2_l.u32 = HDMITXRegRead(pu32RegAddr);
    u_aif_sub_pkt2_l->bits.aif_sub_pkt2_pb3 = o_aif_sub_pkt2_l.bits.aif_sub_pkt2_pb3;
    u_aif_sub_pkt2_l->bits.aif_sub_pkt2_pb2 = o_aif_sub_pkt2_l.bits.aif_sub_pkt2_pb2;
    u_aif_sub_pkt2_l->bits.aif_sub_pkt2_pb1 = o_aif_sub_pkt2_l.bits.aif_sub_pkt2_pb1;
    u_aif_sub_pkt2_l->bits.aif_sub_pkt2_pb0 = o_aif_sub_pkt2_l.bits.aif_sub_pkt2_pb0;

    return HI_SUCCESS;
}

int HDMI_AIF_SUB_PKT2_H_aif_pkt2_pbGet(U_AIF_SUB_PKT2_H* u_aif_sub_pkt2_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AIF_SUB_PKT2_H o_aif_sub_pkt2_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AIF_SUB_PKT2_H.u32);
    o_aif_sub_pkt2_h.u32 = HDMITXRegRead(pu32RegAddr);
    u_aif_sub_pkt2_h->bits.aif_sub_pkt2_pb6 = o_aif_sub_pkt2_h.bits.aif_sub_pkt2_pb6;
    u_aif_sub_pkt2_h->bits.aif_sub_pkt2_pb5 = o_aif_sub_pkt2_h.bits.aif_sub_pkt2_pb5;
    u_aif_sub_pkt2_h->bits.aif_sub_pkt2_pb4 = o_aif_sub_pkt2_h.bits.aif_sub_pkt2_pb4;
    return HI_SUCCESS;
}

int HDMI_AIF_SUB_PKT3_L_aif_pkt3_pbGet(U_AIF_SUB_PKT3_L* u_aif_sub_pkt3_l)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AIF_SUB_PKT3_L o_aif_sub_pkt3_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AIF_SUB_PKT3_L.u32);
    o_aif_sub_pkt3_l.u32 = HDMITXRegRead(pu32RegAddr);
    u_aif_sub_pkt3_l->bits.aif_sub_pkt3_pb3 = o_aif_sub_pkt3_l.bits.aif_sub_pkt3_pb3;
    u_aif_sub_pkt3_l->bits.aif_sub_pkt3_pb2 = o_aif_sub_pkt3_l.bits.aif_sub_pkt3_pb2;
    u_aif_sub_pkt3_l->bits.aif_sub_pkt3_pb1 = o_aif_sub_pkt3_l.bits.aif_sub_pkt3_pb1;
    u_aif_sub_pkt3_l->bits.aif_sub_pkt3_pb0 = o_aif_sub_pkt3_l.bits.aif_sub_pkt3_pb0;
    return HI_SUCCESS;
}

int HDMI_AIF_SUB_PKT3_H_aif_pkt3_pbGet(U_AIF_SUB_PKT3_H* u_aif_sub_pkt3_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AIF_SUB_PKT3_H o_aif_sub_pkt3_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AIF_SUB_PKT3_H.u32);
    o_aif_sub_pkt3_h.u32 = HDMITXRegRead(pu32RegAddr);
    u_aif_sub_pkt3_h->bits.aif_sub_pkt3_pb6 = o_aif_sub_pkt3_h.bits.aif_sub_pkt3_pb6;
    u_aif_sub_pkt3_h->bits.aif_sub_pkt3_pb5 = o_aif_sub_pkt3_h.bits.aif_sub_pkt3_pb5;
    u_aif_sub_pkt3_h->bits.aif_sub_pkt3_pb4 = o_aif_sub_pkt3_h.bits.aif_sub_pkt3_pb4;
    return HI_SUCCESS;
}

//*************************************************************************************//
//******************************** Spif 信息帧配置*******************************//
//*************************************************************************************//
int HDMI_SPIF_PKT_HEADER_hbSet(unsigned int uhb0,unsigned int uhb1,unsigned int uhb2)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SPIF_PKT_HEADER o_spif_pkt_header;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->SPIF_PKT_HEADER.u32);
    o_spif_pkt_header.u32 = HDMITXRegRead(pu32RegAddr);
    o_spif_pkt_header.bits.spd_pkt_hb2= uhb2;
    o_spif_pkt_header.bits.spd_pkt_hb1= uhb1;
    o_spif_pkt_header.bits.spd_pkt_hb0= uhb0;
    HDMITXRegWrite(pu32RegAddr, o_spif_pkt_header.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_spif_pkt_header.u32, "SPIF_PKT_HEADER");

    return HI_SUCCESS;
}

int HDMI_SPIF_SUB_PKT0_L_spif_pkt0_pbSet(unsigned int uspif_pkt0_pb0,unsigned int uspif_pkt0_pb1,unsigned int uspif_pkt0_pb2,unsigned int uspif_pkt0_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SPIF_SUB_PKT0_L o_spif_sub_pkt0_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->SPIF_SUB_PKT0_L.u32);
    o_spif_sub_pkt0_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_spif_sub_pkt0_l.bits.spd_sub_pkt0_pb3 = uspif_pkt0_pb3;
    o_spif_sub_pkt0_l.bits.spd_sub_pkt0_pb2 = uspif_pkt0_pb2;
    o_spif_sub_pkt0_l.bits.spd_sub_pkt0_pb1 = uspif_pkt0_pb1;
    o_spif_sub_pkt0_l.bits.spd_sub_pkt0_pb0 = uspif_pkt0_pb0;
    HDMITXRegWrite(pu32RegAddr, o_spif_sub_pkt0_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_spif_sub_pkt0_l.u32, "SPIF_SUB_PKT0_L");

    return HI_SUCCESS;
}

int HDMI_SPIF_SUB_PKT0_H_spif_pkt0_pbSet(unsigned int uspif_pkt0_pb4,unsigned int uspif_pkt0_pb5,unsigned int uspif_pkt0_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SPIF_SUB_PKT0_H o_spif_sub_pkt0_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->SPIF_SUB_PKT0_H.u32);
    o_spif_sub_pkt0_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_spif_sub_pkt0_h.bits.spd_sub_pkt0_pb6 = uspif_pkt0_pb6;
    o_spif_sub_pkt0_h.bits.spd_sub_pkt0_pb5 = uspif_pkt0_pb5;
    o_spif_sub_pkt0_h.bits.spd_sub_pkt0_pb4 = uspif_pkt0_pb4;
    HDMITXRegWrite(pu32RegAddr, o_spif_sub_pkt0_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_spif_sub_pkt0_h.u32, "SPIF_SUB_PKT0_H");

    return HI_SUCCESS;
}

int HDMI_SPIF_SUB_PKT1_L_spif_pkt1_pbSet(unsigned int uspif_pkt1_pb0,unsigned int uspif_pkt1_pb1,unsigned int uspif_pkt1_pb2,unsigned int uspif_pkt1_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SPIF_SUB_PKT1_L o_spif_sub_pkt1_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->SPIF_SUB_PKT1_L.u32);
    o_spif_sub_pkt1_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_spif_sub_pkt1_l.bits.spd_sub_pkt1_pb3 = uspif_pkt1_pb3;
    o_spif_sub_pkt1_l.bits.spd_sub_pkt1_pb2 = uspif_pkt1_pb2;
    o_spif_sub_pkt1_l.bits.spd_sub_pkt1_pb1 = uspif_pkt1_pb1;
    o_spif_sub_pkt1_l.bits.spd_sub_pkt1_pb0 = uspif_pkt1_pb0;
    HDMITXRegWrite(pu32RegAddr, o_spif_sub_pkt1_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_spif_sub_pkt1_l.u32, "SPIF_SUB_PKT1_L");

    return HI_SUCCESS;
}

int HDMI_SPIF_SUB_PKT1_H_spif_pkt1_pbSet(unsigned int uspif_pkt1_pb4,unsigned int uspif_pkt1_pb5,unsigned int uspif_pkt1_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SPIF_SUB_PKT1_H o_spif_sub_pkt1_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->SPIF_SUB_PKT1_H.u32);
    o_spif_sub_pkt1_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_spif_sub_pkt1_h.bits.spd_sub_pkt1_pb6 = uspif_pkt1_pb6;
    o_spif_sub_pkt1_h.bits.spd_sub_pkt1_pb5 = uspif_pkt1_pb5;
    o_spif_sub_pkt1_h.bits.spd_sub_pkt1_pb4 = uspif_pkt1_pb4;
    HDMITXRegWrite(pu32RegAddr, o_spif_sub_pkt1_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_spif_sub_pkt1_h.u32, "SPIF_SUB_PKT1_H");

    return HI_SUCCESS;
}

int HDMI_SPIF_SUB_PKT2_L_spif_pkt2_pbSet(unsigned int uspif_pkt2_pb0,unsigned int uspif_pkt2_pb1,unsigned int uspif_pkt2_pb2,unsigned int uspif_pkt2_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SPIF_SUB_PKT2_L o_spif_sub_pkt2_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->SPIF_SUB_PKT2_L.u32);
    o_spif_sub_pkt2_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_spif_sub_pkt2_l.bits.spd_sub_pkt2_pb3 = uspif_pkt2_pb3;
    o_spif_sub_pkt2_l.bits.spd_sub_pkt2_pb2 = uspif_pkt2_pb2;
    o_spif_sub_pkt2_l.bits.spd_sub_pkt2_pb1 = uspif_pkt2_pb1;
    o_spif_sub_pkt2_l.bits.spd_sub_pkt2_pb0 = uspif_pkt2_pb0;
    HDMITXRegWrite(pu32RegAddr, o_spif_sub_pkt2_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_spif_sub_pkt2_l.u32, "SPIF_SUB_PKT2_L");

    return HI_SUCCESS;
}

int HDMI_SPIF_SUB_PKT2_H_spif_pkt2_pbSet(unsigned int uspif_pkt2_pb4,unsigned int uspif_pkt2_pb5,unsigned int uspif_pkt2_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SPIF_SUB_PKT2_H o_spif_sub_pkt2_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->SPIF_SUB_PKT2_H.u32);
    o_spif_sub_pkt2_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_spif_sub_pkt2_h.bits.spd_sub_pkt2_pb6 = uspif_pkt2_pb6;
    o_spif_sub_pkt2_h.bits.spd_sub_pkt2_pb5 = uspif_pkt2_pb5;
    o_spif_sub_pkt2_h.bits.spd_sub_pkt2_pb4 = uspif_pkt2_pb4;
    HDMITXRegWrite(pu32RegAddr, o_spif_sub_pkt2_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_spif_sub_pkt2_h.u32, "SPIF_SUB_PKT2_H");

    return HI_SUCCESS;
}

int HDMI_SPIF_SUB_PKT3_L_spif_pkt3_pbSet(unsigned int uspif_pkt3_pb0,unsigned int uspif_pkt3_pb1,unsigned int uspif_pkt3_pb2,unsigned int uspif_pkt3_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SPIF_SUB_PKT3_L o_spif_sub_pkt3_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->SPIF_SUB_PKT3_L.u32);
    o_spif_sub_pkt3_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_spif_sub_pkt3_l.bits.spd_sub_pkt3_pb3 = uspif_pkt3_pb3;
    o_spif_sub_pkt3_l.bits.spd_sub_pkt3_pb2 = uspif_pkt3_pb2;
    o_spif_sub_pkt3_l.bits.spd_sub_pkt3_pb1 = uspif_pkt3_pb1;
    o_spif_sub_pkt3_l.bits.spd_sub_pkt3_pb0 = uspif_pkt3_pb0;
    HDMITXRegWrite(pu32RegAddr, o_spif_sub_pkt3_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_spif_sub_pkt3_l.u32, "SPIF_SUB_PKT3_L");

    return HI_SUCCESS;
}

int HDMI_SPIF_SUB_PKT3_H_spif_pkt3_pbSet(unsigned int uspif_pkt3_pb4,unsigned int uspif_pkt3_pb5,unsigned int uspif_pkt3_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SPIF_SUB_PKT3_H o_spif_sub_pkt3_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->SPIF_SUB_PKT3_H.u32);
    o_spif_sub_pkt3_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_spif_sub_pkt3_h.bits.spd_sub_pkt3_pb6 = uspif_pkt3_pb6;
    o_spif_sub_pkt3_h.bits.spd_sub_pkt3_pb5 = uspif_pkt3_pb5;
    o_spif_sub_pkt3_h.bits.spd_sub_pkt3_pb4 = uspif_pkt3_pb4;
    HDMITXRegWrite(pu32RegAddr, o_spif_sub_pkt3_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_spif_sub_pkt3_h.u32, "SPIF_SUB_PKT3_H");

    return HI_SUCCESS;
}

//*************************************************************************************//
//******************************** Spd 信息帧获取*******************************//
//*************************************************************************************//
int HDMI_SPIF_PKT_HEADER_hbGet(U_SPIF_PKT_HEADER* u_spif_pkt_header)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SPIF_PKT_HEADER o_spif_pkt_header;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->SPIF_PKT_HEADER.u32);
    o_spif_pkt_header.u32 = HDMITXRegRead(pu32RegAddr);
    u_spif_pkt_header->bits.spd_pkt_hb2 = o_spif_pkt_header.bits.spd_pkt_hb2;
    u_spif_pkt_header->bits.spd_pkt_hb1 = o_spif_pkt_header.bits.spd_pkt_hb1;
    u_spif_pkt_header->bits.spd_pkt_hb0 = o_spif_pkt_header.bits.spd_pkt_hb0;
    return HI_SUCCESS;
}

int HDMI_SPIF_SUB_PKT0_L_spif_pkt0_pbGet(U_SPIF_SUB_PKT0_L* u_spif_sub_pkt0_l)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SPIF_SUB_PKT0_L o_spif_sub_pkt0_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->SPIF_SUB_PKT0_L.u32);
    o_spif_sub_pkt0_l.u32 = HDMITXRegRead(pu32RegAddr);
    u_spif_sub_pkt0_l->bits.spd_sub_pkt0_pb3 = o_spif_sub_pkt0_l.bits.spd_sub_pkt0_pb3;
    u_spif_sub_pkt0_l->bits.spd_sub_pkt0_pb2 = o_spif_sub_pkt0_l.bits.spd_sub_pkt0_pb2;
    u_spif_sub_pkt0_l->bits.spd_sub_pkt0_pb1 = o_spif_sub_pkt0_l.bits.spd_sub_pkt0_pb1;
    u_spif_sub_pkt0_l->bits.spd_sub_pkt0_pb0 = o_spif_sub_pkt0_l.bits.spd_sub_pkt0_pb0;
    return HI_SUCCESS;
}

int HDMI_SPIF_SUB_PKT0_H_spif_pkt0_pbGet(U_SPIF_SUB_PKT0_H* u_spif_sub_pkt0_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SPIF_SUB_PKT0_H o_spif_sub_pkt0_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->SPIF_SUB_PKT0_H.u32);
    o_spif_sub_pkt0_h.u32 = HDMITXRegRead(pu32RegAddr);
    u_spif_sub_pkt0_h->bits.spd_sub_pkt0_pb6 = o_spif_sub_pkt0_h.bits.spd_sub_pkt0_pb6;
    u_spif_sub_pkt0_h->bits.spd_sub_pkt0_pb5 = o_spif_sub_pkt0_h.bits.spd_sub_pkt0_pb5;
    u_spif_sub_pkt0_h->bits.spd_sub_pkt0_pb4 = o_spif_sub_pkt0_h.bits.spd_sub_pkt0_pb4;
    return HI_SUCCESS;
}

int HDMI_SPIF_SUB_PKT1_L_spif_pkt1_pbGet(U_SPIF_SUB_PKT1_L* u_spif_sub_pkt1_l)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SPIF_SUB_PKT1_L o_spif_sub_pkt1_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->SPIF_SUB_PKT1_L.u32);
    o_spif_sub_pkt1_l.u32 = HDMITXRegRead(pu32RegAddr);
    u_spif_sub_pkt1_l->bits.spd_sub_pkt1_pb3 = o_spif_sub_pkt1_l.bits.spd_sub_pkt1_pb3;
    u_spif_sub_pkt1_l->bits.spd_sub_pkt1_pb2 = o_spif_sub_pkt1_l.bits.spd_sub_pkt1_pb2;
    u_spif_sub_pkt1_l->bits.spd_sub_pkt1_pb1 = o_spif_sub_pkt1_l.bits.spd_sub_pkt1_pb1;
    u_spif_sub_pkt1_l->bits.spd_sub_pkt1_pb0 = o_spif_sub_pkt1_l.bits.spd_sub_pkt1_pb0;
    return HI_SUCCESS;
}

int HDMI_SPIF_SUB_PKT1_H_spif_pkt1_pbGet(U_SPIF_SUB_PKT1_H* u_spif_sub_pkt1_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SPIF_SUB_PKT1_H o_spif_sub_pkt1_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->SPIF_SUB_PKT1_H.u32);
    o_spif_sub_pkt1_h.u32 = HDMITXRegRead(pu32RegAddr);
    u_spif_sub_pkt1_h->bits.spd_sub_pkt1_pb6 = o_spif_sub_pkt1_h.bits.spd_sub_pkt1_pb6;
    u_spif_sub_pkt1_h->bits.spd_sub_pkt1_pb5 = o_spif_sub_pkt1_h.bits.spd_sub_pkt1_pb5;
    u_spif_sub_pkt1_h->bits.spd_sub_pkt1_pb4 = o_spif_sub_pkt1_h.bits.spd_sub_pkt1_pb4;
    return HI_SUCCESS;
}

int HDMI_SPIF_SUB_PKT2_L_spif_pkt2_pbGet(U_SPIF_SUB_PKT2_L* u_spif_sub_pkt2_l)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SPIF_SUB_PKT2_L o_spif_sub_pkt2_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->SPIF_SUB_PKT2_L.u32);
    o_spif_sub_pkt2_l.u32 = HDMITXRegRead(pu32RegAddr);
    u_spif_sub_pkt2_l->bits.spd_sub_pkt2_pb3 = o_spif_sub_pkt2_l.bits.spd_sub_pkt2_pb3;
    u_spif_sub_pkt2_l->bits.spd_sub_pkt2_pb2 = o_spif_sub_pkt2_l.bits.spd_sub_pkt2_pb2;
    u_spif_sub_pkt2_l->bits.spd_sub_pkt2_pb1 = o_spif_sub_pkt2_l.bits.spd_sub_pkt2_pb1;
    u_spif_sub_pkt2_l->bits.spd_sub_pkt2_pb0 = o_spif_sub_pkt2_l.bits.spd_sub_pkt2_pb0;
    return HI_SUCCESS;
}

int HDMI_SPIF_SUB_PKT2_H_spif_pkt2_pbGet(U_SPIF_SUB_PKT2_H* u_spif_sub_pkt2_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SPIF_SUB_PKT2_H o_spif_sub_pkt2_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->SPIF_SUB_PKT2_H.u32);
    o_spif_sub_pkt2_h.u32 = HDMITXRegRead(pu32RegAddr);
    u_spif_sub_pkt2_h->bits.spd_sub_pkt2_pb6 = o_spif_sub_pkt2_h.bits.spd_sub_pkt2_pb6;
    u_spif_sub_pkt2_h->bits.spd_sub_pkt2_pb5 = o_spif_sub_pkt2_h.bits.spd_sub_pkt2_pb5;
    u_spif_sub_pkt2_h->bits.spd_sub_pkt2_pb4 = o_spif_sub_pkt2_h.bits.spd_sub_pkt2_pb4;
    return HI_SUCCESS;    
}

int HDMI_SPIF_SUB_PKT3_L_spif_pkt3_pbGet(U_SPIF_SUB_PKT3_L* u_spif_sub_pkt3_l)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SPIF_SUB_PKT3_L o_spif_sub_pkt3_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->SPIF_SUB_PKT3_L.u32);
    o_spif_sub_pkt3_l.u32 = HDMITXRegRead(pu32RegAddr);
    u_spif_sub_pkt3_l->bits.spd_sub_pkt3_pb3 = o_spif_sub_pkt3_l.bits.spd_sub_pkt3_pb3;
    u_spif_sub_pkt3_l->bits.spd_sub_pkt3_pb2 = o_spif_sub_pkt3_l.bits.spd_sub_pkt3_pb2;
    u_spif_sub_pkt3_l->bits.spd_sub_pkt3_pb1 = o_spif_sub_pkt3_l.bits.spd_sub_pkt3_pb1;
    u_spif_sub_pkt3_l->bits.spd_sub_pkt3_pb0 = o_spif_sub_pkt3_l.bits.spd_sub_pkt3_pb0;
    return HI_SUCCESS;
}

int HDMI_SPIF_SUB_PKT3_H_spif_pkt3_pbGet(U_SPIF_SUB_PKT3_H* u_spif_sub_pkt3_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SPIF_SUB_PKT3_H o_spif_sub_pkt3_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->SPIF_SUB_PKT3_H.u32);
    o_spif_sub_pkt3_h.u32 = HDMITXRegRead(pu32RegAddr);
    u_spif_sub_pkt3_h->bits.spd_sub_pkt3_pb6 = o_spif_sub_pkt3_h.bits.spd_sub_pkt3_pb6;
    u_spif_sub_pkt3_h->bits.spd_sub_pkt3_pb5 = o_spif_sub_pkt3_h.bits.spd_sub_pkt3_pb5;
    u_spif_sub_pkt3_h->bits.spd_sub_pkt3_pb4 = o_spif_sub_pkt3_h.bits.spd_sub_pkt3_pb4;
    return HI_SUCCESS;
}

//*************************************************************************************//
//******************************** Mpeg 信息帧配置*******************************//
//*************************************************************************************//
int HDMI_MPEG_PKT_HEADER_hbSet(unsigned int uhb0,unsigned int uhb1,unsigned int uhb2)
{
    HI_U32 *pu32RegAddr = NULL;
    U_MPEG_PKT_HEADER o_mpeg_pkt_header;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->MPEG_PKT_HEADER.u32);
    o_mpeg_pkt_header.u32 = HDMITXRegRead(pu32RegAddr);
    o_mpeg_pkt_header.bits.mpeg_pkt_hb2= uhb2;
    o_mpeg_pkt_header.bits.mpeg_pkt_hb1= uhb1;
    o_mpeg_pkt_header.bits.mpeg_pkt_hb0= uhb0;
    HDMITXRegWrite(pu32RegAddr, o_mpeg_pkt_header.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_mpeg_pkt_header.u32, "MPEG_PKT_HEADER");

    return HI_SUCCESS;
}

int HDMI_MPEG_SUB_PKT0_L_mpeg_pkt0_pbSet(unsigned int umpeg_pkt0_pb0,unsigned int umpeg_pkt0_pb1,unsigned int umpeg_pkt0_pb2,unsigned int umpeg_pkt0_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_MPEG_SUB_PKT0_L o_mpeg_sub_pkt0_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->MPEG_SUB_PKT0_L.u32);
    o_mpeg_sub_pkt0_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_mpeg_sub_pkt0_l.bits.mpeg_sub_pkt0_pb3 = umpeg_pkt0_pb3;
    o_mpeg_sub_pkt0_l.bits.mpeg_sub_pkt0_pb2 = umpeg_pkt0_pb2;
    o_mpeg_sub_pkt0_l.bits.mpeg_sub_pkt0_pb1 = umpeg_pkt0_pb1;
    o_mpeg_sub_pkt0_l.bits.mpeg_sub_pkt0_pb0 = umpeg_pkt0_pb0;
    HDMITXRegWrite(pu32RegAddr, o_mpeg_sub_pkt0_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_mpeg_sub_pkt0_l.u32, "MPEG_SUB_PKT0_L");

    return HI_SUCCESS;
}

int HDMI_MPEG_SUB_PKT0_H_mpeg_pkt0_pbSet(unsigned int umpeg_pkt0_pb4,unsigned int umpeg_pkt0_pb5,unsigned int umpeg_pkt0_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_MPEG_SUB_PKT0_H o_mpeg_sub_pkt0_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->MPEG_SUB_PKT0_H.u32);
    o_mpeg_sub_pkt0_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_mpeg_sub_pkt0_h.bits.mpeg_sub_pkt0_pb6 = umpeg_pkt0_pb6;
    o_mpeg_sub_pkt0_h.bits.mpeg_sub_pkt0_pb5 = umpeg_pkt0_pb5;
    o_mpeg_sub_pkt0_h.bits.mpeg_sub_pkt0_pb4 = umpeg_pkt0_pb4;
    HDMITXRegWrite(pu32RegAddr, o_mpeg_sub_pkt0_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_mpeg_sub_pkt0_h.u32, "MPEG_SUB_PKT0_H");

    return HI_SUCCESS;
}

int HDMI_MPEG_SUB_PKT1_L_mpeg_pkt1_pbSet(unsigned int umpeg_pkt1_pb0,unsigned int umpeg_pkt1_pb1,unsigned int umpeg_pkt1_pb2,unsigned int umpeg_pkt1_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_MPEG_SUB_PKT1_L o_mpeg_sub_pkt1_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->MPEG_SUB_PKT1_L.u32);
    o_mpeg_sub_pkt1_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_mpeg_sub_pkt1_l.bits.mpeg_sub_pkt1_pb3 = umpeg_pkt1_pb3;
    o_mpeg_sub_pkt1_l.bits.mpeg_sub_pkt1_pb2 = umpeg_pkt1_pb2;
    o_mpeg_sub_pkt1_l.bits.mpeg_sub_pkt1_pb1 = umpeg_pkt1_pb1;
    o_mpeg_sub_pkt1_l.bits.mpeg_sub_pkt1_pb0 = umpeg_pkt1_pb0;
    HDMITXRegWrite(pu32RegAddr, o_mpeg_sub_pkt1_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_mpeg_sub_pkt1_l.u32, "MPEG_SUB_PKT1_L");

    return HI_SUCCESS;
}

int HDMI_MPEG_SUB_PKT1_H_mpeg_pkt1_pbSet(unsigned int umpeg_pkt1_pb4,unsigned int umpeg_pkt1_pb5,unsigned int umpeg_pkt1_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_MPEG_SUB_PKT1_H o_mpeg_sub_pkt1_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->MPEG_SUB_PKT1_H.u32);
    o_mpeg_sub_pkt1_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_mpeg_sub_pkt1_h.bits.mpeg_sub_pkt1_pb6 = umpeg_pkt1_pb6;
    o_mpeg_sub_pkt1_h.bits.mpeg_sub_pkt1_pb5 = umpeg_pkt1_pb5;
    o_mpeg_sub_pkt1_h.bits.mpeg_sub_pkt1_pb4 = umpeg_pkt1_pb4;
    HDMITXRegWrite(pu32RegAddr, o_mpeg_sub_pkt1_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_mpeg_sub_pkt1_h.u32, "MPEG_SUB_PKT1_H");

    return HI_SUCCESS;
}

int HDMI_MPEG_SUB_PKT2_L_mpeg_pkt2_pbSet(unsigned int umpeg_pkt2_pb0,unsigned int umpeg_pkt2_pb1,unsigned int umpeg_pkt2_pb2,unsigned int umpeg_pkt2_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_MPEG_SUB_PKT2_L o_mpeg_sub_pkt2_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->MPEG_SUB_PKT2_L.u32);
    o_mpeg_sub_pkt2_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_mpeg_sub_pkt2_l.bits.mpeg_sub_pkt2_pb3 = umpeg_pkt2_pb3;
    o_mpeg_sub_pkt2_l.bits.mpeg_sub_pkt2_pb2 = umpeg_pkt2_pb2;
    o_mpeg_sub_pkt2_l.bits.mpeg_sub_pkt2_pb1 = umpeg_pkt2_pb1;
    o_mpeg_sub_pkt2_l.bits.mpeg_sub_pkt2_pb0 = umpeg_pkt2_pb0;
    HDMITXRegWrite(pu32RegAddr, o_mpeg_sub_pkt2_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_mpeg_sub_pkt2_l.u32, "MPEG_SUB_PKT2_L");

    return HI_SUCCESS;
}

int HDMI_MPEG_SUB_PKT2_H_mpeg_pkt2_pbSet(unsigned int umpeg_pkt2_pb4,unsigned int umpeg_pkt2_pb5,unsigned int umpeg_pkt2_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_MPEG_SUB_PKT2_H o_mpeg_sub_pkt2_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->MPEG_SUB_PKT2_H.u32);
    o_mpeg_sub_pkt2_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_mpeg_sub_pkt2_h.bits.mpeg_sub_pkt2_pb6 = umpeg_pkt2_pb6;
    o_mpeg_sub_pkt2_h.bits.mpeg_sub_pkt2_pb5 = umpeg_pkt2_pb5;
    o_mpeg_sub_pkt2_h.bits.mpeg_sub_pkt2_pb4 = umpeg_pkt2_pb4;
    HDMITXRegWrite(pu32RegAddr, o_mpeg_sub_pkt2_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_mpeg_sub_pkt2_h.u32, "MPEG_SUB_PKT2_H");

    return HI_SUCCESS;
}

int HDMI_MPEG_SUB_PKT3_L_mpeg_pkt3_pbSet(unsigned int umpeg_pkt3_pb0,unsigned int umpeg_pkt3_pb1,unsigned int umpeg_pkt3_pb2,unsigned int umpeg_pkt3_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_MPEG_SUB_PKT3_L o_mpeg_sub_pkt3_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->MPEG_SUB_PKT3_L.u32);
    o_mpeg_sub_pkt3_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_mpeg_sub_pkt3_l.bits.mpeg_sub_pkt3_pb3 = umpeg_pkt3_pb3;
    o_mpeg_sub_pkt3_l.bits.mpeg_sub_pkt3_pb2 = umpeg_pkt3_pb2;
    o_mpeg_sub_pkt3_l.bits.mpeg_sub_pkt3_pb1 = umpeg_pkt3_pb1;
    o_mpeg_sub_pkt3_l.bits.mpeg_sub_pkt3_pb0 = umpeg_pkt3_pb0;
    HDMITXRegWrite(pu32RegAddr, o_mpeg_sub_pkt3_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_mpeg_sub_pkt3_l.u32, "MPEG_SUB_PKT3_L");

    return HI_SUCCESS;
}

int HDMI_MPEG_SUB_PKT3_H_mpeg_pkt3_pbSet(unsigned int umpeg_pkt3_pb4,unsigned int umpeg_pkt3_pb5,unsigned int umpeg_pkt3_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_MPEG_SUB_PKT3_H o_mpeg_sub_pkt3_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->MPEG_SUB_PKT3_H.u32);
    o_mpeg_sub_pkt3_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_mpeg_sub_pkt3_h.bits.mpeg_sub_pkt3_pb6 = umpeg_pkt3_pb6;
    o_mpeg_sub_pkt3_h.bits.mpeg_sub_pkt3_pb5 = umpeg_pkt3_pb5;
    o_mpeg_sub_pkt3_h.bits.mpeg_sub_pkt3_pb4 = umpeg_pkt3_pb4;
    HDMITXRegWrite(pu32RegAddr, o_mpeg_sub_pkt3_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_mpeg_sub_pkt3_h.u32, "MPEG_SUB_PKT3_H");

    return HI_SUCCESS;
}

//*************************************************************************************//
//******************************** Avi 信息帧获取*******************************//
//*************************************************************************************//
int HDMI_MPEG_PKT_HEADER_hbGet(U_MPEG_PKT_HEADER* u_mpeg_pkt_header)
{
    HI_U32 *pu32RegAddr = NULL;
    U_MPEG_PKT_HEADER o_mpeg_pkt_header;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->MPEG_PKT_HEADER.u32);
    o_mpeg_pkt_header.u32 = HDMITXRegRead(pu32RegAddr);
    u_mpeg_pkt_header->bits.mpeg_pkt_hb2 = o_mpeg_pkt_header.bits.mpeg_pkt_hb2;
    u_mpeg_pkt_header->bits.mpeg_pkt_hb1 = o_mpeg_pkt_header.bits.mpeg_pkt_hb1;
    u_mpeg_pkt_header->bits.mpeg_pkt_hb0 = o_mpeg_pkt_header.bits.mpeg_pkt_hb0;
    return HI_SUCCESS;
}

int HDMI_MPEG_SUB_PKT0_L_mpeg_pkt0_pbGet(U_MPEG_SUB_PKT0_L* u_mpeg_sub_pkt0_l)
{
    HI_U32 *pu32RegAddr = NULL;
    U_MPEG_SUB_PKT0_L o_mpeg_sub_pkt0_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->MPEG_SUB_PKT0_L.u32);
    o_mpeg_sub_pkt0_l.u32 = HDMITXRegRead(pu32RegAddr);
    u_mpeg_sub_pkt0_l->bits.mpeg_sub_pkt0_pb3 = o_mpeg_sub_pkt0_l.bits.mpeg_sub_pkt0_pb3;
    u_mpeg_sub_pkt0_l->bits.mpeg_sub_pkt0_pb2 = o_mpeg_sub_pkt0_l.bits.mpeg_sub_pkt0_pb2;
    u_mpeg_sub_pkt0_l->bits.mpeg_sub_pkt0_pb1 = o_mpeg_sub_pkt0_l.bits.mpeg_sub_pkt0_pb1;
    u_mpeg_sub_pkt0_l->bits.mpeg_sub_pkt0_pb0 = o_mpeg_sub_pkt0_l.bits.mpeg_sub_pkt0_pb0;
    return HI_SUCCESS;
}

int HDMI_MPEG_SUB_PKT0_H_mpeg_pkt0_pbGet(U_MPEG_SUB_PKT0_H* u_mpeg_sub_pkt0_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_MPEG_SUB_PKT0_H o_mpeg_sub_pkt0_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->MPEG_SUB_PKT0_H.u32);
    o_mpeg_sub_pkt0_h.u32 = HDMITXRegRead(pu32RegAddr);
    u_mpeg_sub_pkt0_h->bits.mpeg_sub_pkt0_pb6 = o_mpeg_sub_pkt0_h.bits.mpeg_sub_pkt0_pb6;
    u_mpeg_sub_pkt0_h->bits.mpeg_sub_pkt0_pb5 = o_mpeg_sub_pkt0_h.bits.mpeg_sub_pkt0_pb5;
    u_mpeg_sub_pkt0_h->bits.mpeg_sub_pkt0_pb4 = o_mpeg_sub_pkt0_h.bits.mpeg_sub_pkt0_pb4;
    return HI_SUCCESS;
}

int HDMI_MPEG_SUB_PKT1_L_mpeg_pkt1_pbGet(U_MPEG_SUB_PKT1_L* u_mpeg_sub_pkt1_l)
{
    HI_U32 *pu32RegAddr = NULL;
    U_MPEG_SUB_PKT1_L o_mpeg_sub_pkt1_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->MPEG_SUB_PKT1_L.u32);
    o_mpeg_sub_pkt1_l.u32 = HDMITXRegRead(pu32RegAddr);
    u_mpeg_sub_pkt1_l->bits.mpeg_sub_pkt1_pb3 = o_mpeg_sub_pkt1_l.bits.mpeg_sub_pkt1_pb3;
    u_mpeg_sub_pkt1_l->bits.mpeg_sub_pkt1_pb2 = o_mpeg_sub_pkt1_l.bits.mpeg_sub_pkt1_pb2;
    u_mpeg_sub_pkt1_l->bits.mpeg_sub_pkt1_pb1 = o_mpeg_sub_pkt1_l.bits.mpeg_sub_pkt1_pb1;
    u_mpeg_sub_pkt1_l->bits.mpeg_sub_pkt1_pb0 = o_mpeg_sub_pkt1_l.bits.mpeg_sub_pkt1_pb0;
    return HI_SUCCESS;
}

int HDMI_MPEG_SUB_PKT1_H_mpeg_pkt1_pbGet(U_MPEG_SUB_PKT1_H* u_mpeg_sub_pkt1_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_MPEG_SUB_PKT1_H o_mpeg_sub_pkt1_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->MPEG_SUB_PKT1_H.u32);
    o_mpeg_sub_pkt1_h.u32 = HDMITXRegRead(pu32RegAddr);
    u_mpeg_sub_pkt1_h->bits.mpeg_sub_pkt1_pb6 = o_mpeg_sub_pkt1_h.bits.mpeg_sub_pkt1_pb6;
    u_mpeg_sub_pkt1_h->bits.mpeg_sub_pkt1_pb5 = o_mpeg_sub_pkt1_h.bits.mpeg_sub_pkt1_pb5;
    u_mpeg_sub_pkt1_h->bits.mpeg_sub_pkt1_pb4 = o_mpeg_sub_pkt1_h.bits.mpeg_sub_pkt1_pb4;
    return HI_SUCCESS;
}

int HDMI_MPEG_SUB_PKT2_L_mpeg_pkt2_pbGet(U_MPEG_SUB_PKT2_L* u_mpeg_sub_pkt2_l)
{
    HI_U32 *pu32RegAddr = NULL;
    U_MPEG_SUB_PKT2_L o_mpeg_sub_pkt2_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->MPEG_SUB_PKT2_L.u32);
    o_mpeg_sub_pkt2_l.u32 = HDMITXRegRead(pu32RegAddr);
    u_mpeg_sub_pkt2_l->bits.mpeg_sub_pkt2_pb3 = o_mpeg_sub_pkt2_l.bits.mpeg_sub_pkt2_pb3;
    u_mpeg_sub_pkt2_l->bits.mpeg_sub_pkt2_pb2 = o_mpeg_sub_pkt2_l.bits.mpeg_sub_pkt2_pb2;
    u_mpeg_sub_pkt2_l->bits.mpeg_sub_pkt2_pb1 = o_mpeg_sub_pkt2_l.bits.mpeg_sub_pkt2_pb1;
    u_mpeg_sub_pkt2_l->bits.mpeg_sub_pkt2_pb0 = o_mpeg_sub_pkt2_l.bits.mpeg_sub_pkt2_pb0;
    return HI_SUCCESS;
}

int HDMI_MPEG_SUB_PKT2_H_mpeg_pkt2_pbGet(U_MPEG_SUB_PKT2_H* u_mpeg_sub_pkt2_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_MPEG_SUB_PKT2_H o_mpeg_sub_pkt2_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->MPEG_SUB_PKT2_H.u32);
    o_mpeg_sub_pkt2_h.u32 = HDMITXRegRead(pu32RegAddr);
    u_mpeg_sub_pkt2_h->bits.mpeg_sub_pkt2_pb6 = o_mpeg_sub_pkt2_h.bits.mpeg_sub_pkt2_pb6;
    u_mpeg_sub_pkt2_h->bits.mpeg_sub_pkt2_pb5 = o_mpeg_sub_pkt2_h.bits.mpeg_sub_pkt2_pb5;
    u_mpeg_sub_pkt2_h->bits.mpeg_sub_pkt2_pb4 = o_mpeg_sub_pkt2_h.bits.mpeg_sub_pkt2_pb4;
    return HI_SUCCESS;
}

int HDMI_MPEG_SUB_PKT3_L_mpeg_pkt3_pbGet(U_MPEG_SUB_PKT3_L* u_mpeg_sub_pkt3_l)
{
    HI_U32 *pu32RegAddr = NULL;
    U_MPEG_SUB_PKT3_L o_mpeg_sub_pkt3_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->MPEG_SUB_PKT3_L.u32);
    o_mpeg_sub_pkt3_l.u32 = HDMITXRegRead(pu32RegAddr);
    u_mpeg_sub_pkt3_l->bits.mpeg_sub_pkt3_pb3 = o_mpeg_sub_pkt3_l.bits.mpeg_sub_pkt3_pb3;
    u_mpeg_sub_pkt3_l->bits.mpeg_sub_pkt3_pb2 = o_mpeg_sub_pkt3_l.bits.mpeg_sub_pkt3_pb2;
    u_mpeg_sub_pkt3_l->bits.mpeg_sub_pkt3_pb1 = o_mpeg_sub_pkt3_l.bits.mpeg_sub_pkt3_pb1;
    u_mpeg_sub_pkt3_l->bits.mpeg_sub_pkt3_pb0 = o_mpeg_sub_pkt3_l.bits.mpeg_sub_pkt3_pb0;
    return HI_SUCCESS;
}

int HDMI_MPEG_SUB_PKT3_H_mpeg_pkt3_pbGet(U_MPEG_SUB_PKT3_H* u_mpeg_sub_pkt3_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_MPEG_SUB_PKT3_H o_mpeg_sub_pkt3_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->MPEG_SUB_PKT3_H.u32);
    o_mpeg_sub_pkt3_h.u32 = HDMITXRegRead(pu32RegAddr);
    u_mpeg_sub_pkt3_h->bits.mpeg_sub_pkt3_pb6 = o_mpeg_sub_pkt3_h.bits.mpeg_sub_pkt3_pb6;
    u_mpeg_sub_pkt3_h->bits.mpeg_sub_pkt3_pb5 = o_mpeg_sub_pkt3_h.bits.mpeg_sub_pkt3_pb5;
    u_mpeg_sub_pkt3_h->bits.mpeg_sub_pkt3_pb4 = o_mpeg_sub_pkt3_h.bits.mpeg_sub_pkt3_pb4;
    return HI_SUCCESS;
}

//*************************************************************************************//
//********************************  Gen    信息帧设置******************************//
//*************************************************************************************//
int HDMI_GEN_PKT_HEADER_hbSet(unsigned int uhb0,unsigned int uhb1,unsigned int uhb2)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN_PKT_HEADER o_gen_pkt_header;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN_PKT_HEADER.u32);
    o_gen_pkt_header.u32 = HDMITXRegRead(pu32RegAddr);
    o_gen_pkt_header.bits.gen_pkt_hb2= uhb2;
    o_gen_pkt_header.bits.gen_pkt_hb1= uhb1;
    o_gen_pkt_header.bits.gen_pkt_hb0= uhb0;
    HDMITXRegWrite(pu32RegAddr, o_gen_pkt_header.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gen_pkt_header.u32, "GEN_PKT_HEADER");

    return HI_SUCCESS;
}

int HDMI_GEN_SUB_PKT0_L_gen_pkt0_pbSet(unsigned int ugen_pkt0_pb0,unsigned int ugen_pkt0_pb1,unsigned int ugen_pkt0_pb2,unsigned int ugen_pkt0_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN_SUB_PKT0_L o_gen_sub_pkt0_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN_SUB_PKT0_L.u32);
    o_gen_sub_pkt0_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_gen_sub_pkt0_l.bits.gen_sub_pkt0_pb3 = ugen_pkt0_pb3;
    o_gen_sub_pkt0_l.bits.gen_sub_pkt0_pb2 = ugen_pkt0_pb2;
    o_gen_sub_pkt0_l.bits.gen_sub_pkt0_pb1 = ugen_pkt0_pb1;
    o_gen_sub_pkt0_l.bits.gen_sub_pkt0_pb0 = ugen_pkt0_pb0;
    HDMITXRegWrite(pu32RegAddr, o_gen_sub_pkt0_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gen_sub_pkt0_l.u32, "GEN_SUB_PKT0_L");

    return HI_SUCCESS;
}

int HDMI_GEN_SUB_PKT0_H_gen_pkt0_pbSet(unsigned int ugen_pkt0_pb4,unsigned int ugen_pkt0_pb5,unsigned int ugen_pkt0_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN_SUB_PKT0_H o_gen_sub_pkt0_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN_SUB_PKT0_H.u32);
    o_gen_sub_pkt0_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_gen_sub_pkt0_h.bits.gen_sub_pkt0_pb6 = ugen_pkt0_pb6;
    o_gen_sub_pkt0_h.bits.gen_sub_pkt0_pb5 = ugen_pkt0_pb5;
    o_gen_sub_pkt0_h.bits.gen_sub_pkt0_pb4 = ugen_pkt0_pb4;
    HDMITXRegWrite(pu32RegAddr, o_gen_sub_pkt0_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gen_sub_pkt0_h.u32, "GEN_SUB_PKT0_H");

    return HI_SUCCESS;
}

int HDMI_GEN_SUB_PKT1_L_gen_pkt1_pbSet(unsigned int ugen_pkt1_pb0,unsigned int ugen_pkt1_pb1,unsigned int ugen_pkt1_pb2,unsigned int ugen_pkt1_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN_SUB_PKT1_L o_gen_sub_pkt1_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN_SUB_PKT1_L.u32);
    o_gen_sub_pkt1_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_gen_sub_pkt1_l.bits.gen_sub_pkt1_pb3 = ugen_pkt1_pb3;
    o_gen_sub_pkt1_l.bits.gen_sub_pkt1_pb2 = ugen_pkt1_pb2;
    o_gen_sub_pkt1_l.bits.gen_sub_pkt1_pb1 = ugen_pkt1_pb1;
    o_gen_sub_pkt1_l.bits.gen_sub_pkt1_pb0 = ugen_pkt1_pb0;
    HDMITXRegWrite(pu32RegAddr, o_gen_sub_pkt1_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gen_sub_pkt1_l.u32, "GEN_SUB_PKT1_L");

    return HI_SUCCESS;
}

int HDMI_GEN_SUB_PKT1_H_gen_pkt1_pbSet(unsigned int ugen_pkt1_pb4,unsigned int ugen_pkt1_pb5,unsigned int ugen_pkt1_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN_SUB_PKT1_H o_gen_sub_pkt1_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN_SUB_PKT1_H.u32);
    o_gen_sub_pkt1_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_gen_sub_pkt1_h.bits.gen_sub_pkt1_pb6 = ugen_pkt1_pb6;
    o_gen_sub_pkt1_h.bits.gen_sub_pkt1_pb5 = ugen_pkt1_pb5;
    o_gen_sub_pkt1_h.bits.gen_sub_pkt1_pb4 = ugen_pkt1_pb4;
    HDMITXRegWrite(pu32RegAddr, o_gen_sub_pkt1_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gen_sub_pkt1_h.u32, "GEN_SUB_PKT1_H");

    return HI_SUCCESS;
}

int HDMI_GEN_SUB_PKT2_L_gen_pkt2_pbSet(unsigned int ugen_pkt2_pb0,unsigned int ugen_pkt2_pb1,unsigned int ugen_pkt2_pb2,unsigned int ugen_pkt2_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN_SUB_PKT2_L o_gen_sub_pkt2_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN_SUB_PKT2_L.u32);
    o_gen_sub_pkt2_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_gen_sub_pkt2_l.bits.gen_sub_pkt2_pb3 = ugen_pkt2_pb3;
    o_gen_sub_pkt2_l.bits.gen_sub_pkt2_pb2 = ugen_pkt2_pb2;
    o_gen_sub_pkt2_l.bits.gen_sub_pkt2_pb1 = ugen_pkt2_pb1;
    o_gen_sub_pkt2_l.bits.gen_sub_pkt2_pb0 = ugen_pkt2_pb0;
    HDMITXRegWrite(pu32RegAddr, o_gen_sub_pkt2_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gen_sub_pkt2_l.u32, "GEN_SUB_PKT2_L");

    return HI_SUCCESS;
}

int HDMI_GEN_SUB_PKT2_H_gen_pkt2_pbSet(unsigned int ugen_pkt2_pb4,unsigned int ugen_pkt2_pb5,unsigned int ugen_pkt2_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN_SUB_PKT2_H o_gen_sub_pkt2_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN_SUB_PKT2_H.u32);
    o_gen_sub_pkt2_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_gen_sub_pkt2_h.bits.gen_sub_pkt2_pb6 = ugen_pkt2_pb6;
    o_gen_sub_pkt2_h.bits.gen_sub_pkt2_pb5 = ugen_pkt2_pb5;
    o_gen_sub_pkt2_h.bits.gen_sub_pkt2_pb4 = ugen_pkt2_pb4;
    HDMITXRegWrite(pu32RegAddr, o_gen_sub_pkt2_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gen_sub_pkt2_h.u32, "GEN_SUB_PKT2_H");

    return HI_SUCCESS;
}

int HDMI_GEN_SUB_PKT3_L_gen_pkt3_pbSet(unsigned int ugen_pkt3_pb0,unsigned int ugen_pkt3_pb1,unsigned int ugen_pkt3_pb2,unsigned int ugen_pkt3_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN_SUB_PKT3_L o_gen_sub_pkt3_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN_SUB_PKT3_L.u32);
    o_gen_sub_pkt3_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_gen_sub_pkt3_l.bits.gen_sub_pkt3_pb3 = ugen_pkt3_pb3;
    o_gen_sub_pkt3_l.bits.gen_sub_pkt3_pb2 = ugen_pkt3_pb2;
    o_gen_sub_pkt3_l.bits.gen_sub_pkt3_pb1 = ugen_pkt3_pb1;
    o_gen_sub_pkt3_l.bits.gen_sub_pkt3_pb0 = ugen_pkt3_pb0;
    HDMITXRegWrite(pu32RegAddr, o_gen_sub_pkt3_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gen_sub_pkt3_l.u32, "GEN_SUB_PKT3_L");

    return HI_SUCCESS;
}

int HDMI_GEN_SUB_PKT3_H_gen_pkt3_pbSet(unsigned int ugen_pkt3_pb4,unsigned int ugen_pkt3_pb5,unsigned int ugen_pkt3_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN_SUB_PKT3_H o_gen_sub_pkt3_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN_SUB_PKT3_H.u32);
    o_gen_sub_pkt3_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_gen_sub_pkt3_h.bits.gen_sub_pkt3_pb6 = ugen_pkt3_pb6;
    o_gen_sub_pkt3_h.bits.gen_sub_pkt3_pb5 = ugen_pkt3_pb5;
    o_gen_sub_pkt3_h.bits.gen_sub_pkt3_pb4 = ugen_pkt3_pb4;
    HDMITXRegWrite(pu32RegAddr, o_gen_sub_pkt3_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gen_sub_pkt3_h.u32, "GEN_SUB_PKT3_H");

    return HI_SUCCESS;
}


//*************************************************************************************//
//********************************  Gen    信息帧获取******************************//
//*************************************************************************************//
int HDMI_GEN_PKT_HEADER_hbGet(U_GEN_PKT_HEADER* u_gen_pkt_header)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN_PKT_HEADER o_gen_pkt_header;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN_PKT_HEADER.u32);
    o_gen_pkt_header.u32 = HDMITXRegRead(pu32RegAddr);
    u_gen_pkt_header->bits.gen_pkt_hb2 = o_gen_pkt_header.bits.gen_pkt_hb2;
    u_gen_pkt_header->bits.gen_pkt_hb1 = o_gen_pkt_header.bits.gen_pkt_hb1;
    u_gen_pkt_header->bits.gen_pkt_hb0 = o_gen_pkt_header.bits.gen_pkt_hb0;
	return HI_SUCCESS;
}

int HDMI_GEN_SUB_PKT0_L_gen_pkt0_pbGet(U_GEN_SUB_PKT0_L* u_gen_sub_pkt0_l)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN_SUB_PKT0_L o_gen_sub_pkt0_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN_SUB_PKT0_L.u32);
    o_gen_sub_pkt0_l.u32 = HDMITXRegRead(pu32RegAddr);
    u_gen_sub_pkt0_l->bits.gen_sub_pkt0_pb3 = o_gen_sub_pkt0_l.bits.gen_sub_pkt0_pb3;
    u_gen_sub_pkt0_l->bits.gen_sub_pkt0_pb2 = o_gen_sub_pkt0_l.bits.gen_sub_pkt0_pb2;
    u_gen_sub_pkt0_l->bits.gen_sub_pkt0_pb1 = o_gen_sub_pkt0_l.bits.gen_sub_pkt0_pb1;
    u_gen_sub_pkt0_l->bits.gen_sub_pkt0_pb0 = o_gen_sub_pkt0_l.bits.gen_sub_pkt0_pb0;
	return HI_SUCCESS;
}

int HDMI_GEN_SUB_PKT0_H_gen_pkt0_pbGet(U_GEN_SUB_PKT0_H* u_gen_sub_pkt0_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN_SUB_PKT0_H o_gen_sub_pkt0_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN_SUB_PKT0_H.u32);
    o_gen_sub_pkt0_h.u32 = HDMITXRegRead(pu32RegAddr);
    u_gen_sub_pkt0_h->bits.gen_sub_pkt0_pb6 = o_gen_sub_pkt0_h.bits.gen_sub_pkt0_pb6;
    u_gen_sub_pkt0_h->bits.gen_sub_pkt0_pb5 = o_gen_sub_pkt0_h.bits.gen_sub_pkt0_pb5;
    u_gen_sub_pkt0_h->bits.gen_sub_pkt0_pb4 = o_gen_sub_pkt0_h.bits.gen_sub_pkt0_pb4;
	
	return HI_SUCCESS;
}

int HDMI_GEN_SUB_PKT1_L_gen_pkt1_pbGet(U_GEN_SUB_PKT1_L* u_gen_sub_pkt1_l)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN_SUB_PKT1_L o_gen_sub_pkt1_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN_SUB_PKT1_L.u32);
    o_gen_sub_pkt1_l.u32 = HDMITXRegRead(pu32RegAddr);
    u_gen_sub_pkt1_l->bits.gen_sub_pkt1_pb3 = o_gen_sub_pkt1_l.bits.gen_sub_pkt1_pb3;
    u_gen_sub_pkt1_l->bits.gen_sub_pkt1_pb2 = o_gen_sub_pkt1_l.bits.gen_sub_pkt1_pb2;
    u_gen_sub_pkt1_l->bits.gen_sub_pkt1_pb1 = o_gen_sub_pkt1_l.bits.gen_sub_pkt1_pb1;
    u_gen_sub_pkt1_l->bits.gen_sub_pkt1_pb0 = o_gen_sub_pkt1_l.bits.gen_sub_pkt1_pb0;
	
	return HI_SUCCESS;
}

int HDMI_GEN_SUB_PKT1_H_gen_pkt1_pbGet(U_GEN_SUB_PKT1_H* u_gen_sub_pkt1_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN_SUB_PKT1_H o_gen_sub_pkt1_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN_SUB_PKT1_H.u32);
    o_gen_sub_pkt1_h.u32 = HDMITXRegRead(pu32RegAddr);
    u_gen_sub_pkt1_h->bits.gen_sub_pkt1_pb6 = o_gen_sub_pkt1_h.bits.gen_sub_pkt1_pb6;
    u_gen_sub_pkt1_h->bits.gen_sub_pkt1_pb5 = o_gen_sub_pkt1_h.bits.gen_sub_pkt1_pb5;
    u_gen_sub_pkt1_h->bits.gen_sub_pkt1_pb4 = o_gen_sub_pkt1_h.bits.gen_sub_pkt1_pb4;
	
	return HI_SUCCESS;
}

int HDMI_GEN_SUB_PKT2_L_gen_pkt2_pbGet(U_GEN_SUB_PKT2_L* u_gen_sub_pkt2_l)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN_SUB_PKT2_L o_gen_sub_pkt2_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN_SUB_PKT2_L.u32);
    o_gen_sub_pkt2_l.u32 = HDMITXRegRead(pu32RegAddr);
    u_gen_sub_pkt2_l->bits.gen_sub_pkt2_pb3 = o_gen_sub_pkt2_l.bits.gen_sub_pkt2_pb3;
    u_gen_sub_pkt2_l->bits.gen_sub_pkt2_pb2 = o_gen_sub_pkt2_l.bits.gen_sub_pkt2_pb2;
    u_gen_sub_pkt2_l->bits.gen_sub_pkt2_pb1 = o_gen_sub_pkt2_l.bits.gen_sub_pkt2_pb1;
    u_gen_sub_pkt2_l->bits.gen_sub_pkt2_pb0 = o_gen_sub_pkt2_l.bits.gen_sub_pkt2_pb0;
	
	return HI_SUCCESS;
}

int HDMI_GEN_SUB_PKT2_H_gen_pkt2_pbGet(U_GEN_SUB_PKT2_H* u_gen_sub_pkt2_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN_SUB_PKT2_H o_gen_sub_pkt2_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN_SUB_PKT2_H.u32);
    o_gen_sub_pkt2_h.u32 = HDMITXRegRead(pu32RegAddr);
    u_gen_sub_pkt2_h->bits.gen_sub_pkt2_pb6 = o_gen_sub_pkt2_h.bits.gen_sub_pkt2_pb6;
    u_gen_sub_pkt2_h->bits.gen_sub_pkt2_pb5 = o_gen_sub_pkt2_h.bits.gen_sub_pkt2_pb5;
    u_gen_sub_pkt2_h->bits.gen_sub_pkt2_pb4 = o_gen_sub_pkt2_h.bits.gen_sub_pkt2_pb4;
	
	return HI_SUCCESS;
}

int HDMI_GEN_SUB_PKT3_L_gen_pkt3_pbGet(U_GEN_SUB_PKT3_L* u_gen_sub_pkt3_l)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN_SUB_PKT3_L o_gen_sub_pkt3_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN_SUB_PKT3_L.u32);
    o_gen_sub_pkt3_l.u32 = HDMITXRegRead(pu32RegAddr);
    u_gen_sub_pkt3_l->bits.gen_sub_pkt3_pb3 = o_gen_sub_pkt3_l.bits.gen_sub_pkt3_pb3;
    u_gen_sub_pkt3_l->bits.gen_sub_pkt3_pb2 = o_gen_sub_pkt3_l.bits.gen_sub_pkt3_pb2;
    u_gen_sub_pkt3_l->bits.gen_sub_pkt3_pb1 = o_gen_sub_pkt3_l.bits.gen_sub_pkt3_pb1;
    u_gen_sub_pkt3_l->bits.gen_sub_pkt3_pb0 = o_gen_sub_pkt3_l.bits.gen_sub_pkt3_pb0;
	
	return HI_SUCCESS;
}

int HDMI_GEN_SUB_PKT3_H_gen_pkt3_pbGet(U_GEN_SUB_PKT3_H* u_gen_sub_pkt3_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN_SUB_PKT3_H o_gen_sub_pkt3_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN_SUB_PKT3_H.u32);
    o_gen_sub_pkt3_h.u32 = HDMITXRegRead(pu32RegAddr);
    u_gen_sub_pkt3_h->bits.gen_sub_pkt3_pb6 = o_gen_sub_pkt3_h.bits.gen_sub_pkt3_pb6;
    u_gen_sub_pkt3_h->bits.gen_sub_pkt3_pb5 = o_gen_sub_pkt3_h.bits.gen_sub_pkt3_pb5;
    u_gen_sub_pkt3_h->bits.gen_sub_pkt3_pb4 = o_gen_sub_pkt3_h.bits.gen_sub_pkt3_pb4;
	
	return HI_SUCCESS;
}

//*************************************************************************************//
//********************************  Gen2    信息帧设置******************************//
//*************************************************************************************//
int HDMI_GEN2_PKT_HEADER_hbSet(unsigned int uhb0,unsigned int uhb1,unsigned int uhb2)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN2_PKT_HEADER o_gen2_pkt_header;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN2_PKT_HEADER.u32);
    o_gen2_pkt_header.u32 = HDMITXRegRead(pu32RegAddr);
    o_gen2_pkt_header.bits.gen2_pkt_hb2= uhb2;
    o_gen2_pkt_header.bits.gen2_pkt_hb1= uhb1;
    o_gen2_pkt_header.bits.gen2_pkt_hb0= uhb0;
    HDMITXRegWrite(pu32RegAddr, o_gen2_pkt_header.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gen2_pkt_header.u32, "GEN2_PKT_HEADER");

    return HI_SUCCESS;
}

int HDMI_GEN2_SUB_PKT0_L_gen2_pkt0_pbSet(unsigned int ugen2_pkt0_pb0,unsigned int ugen2_pkt0_pb1,unsigned int ugen2_pkt0_pb2,unsigned int ugen2_pkt0_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN2_SUB_PKT0_L o_gen2_sub_pkt0_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN2_SUB_PKT0_L.u32);
    o_gen2_sub_pkt0_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_gen2_sub_pkt0_l.bits.gen2_sub_pkt0_pb3 = ugen2_pkt0_pb3;
    o_gen2_sub_pkt0_l.bits.gen2_sub_pkt0_pb2 = ugen2_pkt0_pb2;
    o_gen2_sub_pkt0_l.bits.gen2_sub_pkt0_pb1 = ugen2_pkt0_pb1;
    o_gen2_sub_pkt0_l.bits.gen2_sub_pkt0_pb0 = ugen2_pkt0_pb0;
    HDMITXRegWrite(pu32RegAddr, o_gen2_sub_pkt0_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gen2_sub_pkt0_l.u32, "GEN2_SUB_PKT0_L");

    return HI_SUCCESS;
}

int HDMI_GEN2_SUB_PKT0_H_gen2_pkt0_pbSet(unsigned int ugen2_pkt0_pb4,unsigned int ugen2_pkt0_pb5,unsigned int ugen2_pkt0_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN2_SUB_PKT0_H o_gen2_sub_pkt0_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN2_SUB_PKT0_H.u32);
    o_gen2_sub_pkt0_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_gen2_sub_pkt0_h.bits.gen2_sub_pkt0_pb6 = ugen2_pkt0_pb6;
    o_gen2_sub_pkt0_h.bits.gen2_sub_pkt0_pb5 = ugen2_pkt0_pb5;
    o_gen2_sub_pkt0_h.bits.gen2_sub_pkt0_pb4 = ugen2_pkt0_pb4;
    HDMITXRegWrite(pu32RegAddr, o_gen2_sub_pkt0_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gen2_sub_pkt0_h.u32, "GEN2_SUB_PKT0_H");

    return HI_SUCCESS;
}

int HDMI_GEN2_SUB_PKT1_L_gen2_pkt1_pbSet(unsigned int ugen2_pkt1_pb0,unsigned int ugen2_pkt1_pb1,unsigned int ugen2_pkt1_pb2,unsigned int ugen2_pkt1_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN2_SUB_PKT1_L o_gen2_sub_pkt1_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN2_SUB_PKT1_L.u32);
    o_gen2_sub_pkt1_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_gen2_sub_pkt1_l.bits.gen2_sub_pkt1_pb3 = ugen2_pkt1_pb3;
    o_gen2_sub_pkt1_l.bits.gen2_sub_pkt1_pb2 = ugen2_pkt1_pb2;
    o_gen2_sub_pkt1_l.bits.gen2_sub_pkt1_pb1 = ugen2_pkt1_pb1;
    o_gen2_sub_pkt1_l.bits.gen2_sub_pkt1_pb0 = ugen2_pkt1_pb0;
    HDMITXRegWrite(pu32RegAddr, o_gen2_sub_pkt1_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gen2_sub_pkt1_l.u32, "GEN2_SUB_PKT1_L");

    return HI_SUCCESS;
}

int HDMI_GEN2_SUB_PKT1_H_gen2_pkt1_pbSet(unsigned int ugen2_pkt1_pb4,unsigned int ugen2_pkt1_pb5,unsigned int ugen2_pkt1_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN2_SUB_PKT1_H o_gen2_sub_pkt1_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN2_SUB_PKT1_H.u32);
    o_gen2_sub_pkt1_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_gen2_sub_pkt1_h.bits.gen2_sub_pkt1_pb6 = ugen2_pkt1_pb6;
    o_gen2_sub_pkt1_h.bits.gen2_sub_pkt1_pb5 = ugen2_pkt1_pb5;
    o_gen2_sub_pkt1_h.bits.gen2_sub_pkt1_pb4 = ugen2_pkt1_pb4;
    HDMITXRegWrite(pu32RegAddr, o_gen2_sub_pkt1_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gen2_sub_pkt1_h.u32, "GEN2_SUB_PKT1_H");

    return HI_SUCCESS;
}

int HDMI_GEN2_SUB_PKT2_L_gen2_pkt2_pbSet(unsigned int ugen2_pkt2_pb0,unsigned int ugen2_pkt2_pb1,unsigned int ugen2_pkt2_pb2,unsigned int ugen2_pkt2_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN2_SUB_PKT2_L o_gen2_sub_pkt2_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN2_SUB_PKT2_L.u32);
    o_gen2_sub_pkt2_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_gen2_sub_pkt2_l.bits.gen2_sub_pkt2_pb3 = ugen2_pkt2_pb3;
    o_gen2_sub_pkt2_l.bits.gen2_sub_pkt2_pb2 = ugen2_pkt2_pb2;
    o_gen2_sub_pkt2_l.bits.gen2_sub_pkt2_pb1 = ugen2_pkt2_pb1;
    o_gen2_sub_pkt2_l.bits.gen2_sub_pkt2_pb0 = ugen2_pkt2_pb0;
    HDMITXRegWrite(pu32RegAddr, o_gen2_sub_pkt2_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gen2_sub_pkt2_l.u32, "GEN2_SUB_PKT2_L");

    return HI_SUCCESS;
}

int HDMI_GEN2_SUB_PKT2_H_gen2_pkt2_pbSet(unsigned int ugen2_pkt2_pb4,unsigned int ugen2_pkt2_pb5,unsigned int ugen2_pkt2_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN2_SUB_PKT2_H o_gen2_sub_pkt2_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN2_SUB_PKT2_H.u32);
    o_gen2_sub_pkt2_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_gen2_sub_pkt2_h.bits.gen2_sub_pkt2_pb6 = ugen2_pkt2_pb6;
    o_gen2_sub_pkt2_h.bits.gen2_sub_pkt2_pb5 = ugen2_pkt2_pb5;
    o_gen2_sub_pkt2_h.bits.gen2_sub_pkt2_pb4 = ugen2_pkt2_pb4;
    HDMITXRegWrite(pu32RegAddr, o_gen2_sub_pkt2_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gen2_sub_pkt2_h.u32, "GEN2_SUB_PKT2_H");

    return HI_SUCCESS;
}

int HDMI_GEN2_SUB_PKT3_L_gen2_pkt3_pbSet(unsigned int ugen2_pkt3_pb0,unsigned int ugen2_pkt3_pb1,unsigned int ugen2_pkt3_pb2,unsigned int ugen2_pkt3_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN2_SUB_PKT3_L o_gen2_sub_pkt3_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN2_SUB_PKT3_L.u32);
    o_gen2_sub_pkt3_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_gen2_sub_pkt3_l.bits.gen2_sub_pkt3_pb3 = ugen2_pkt3_pb3;
    o_gen2_sub_pkt3_l.bits.gen2_sub_pkt3_pb2 = ugen2_pkt3_pb2;
    o_gen2_sub_pkt3_l.bits.gen2_sub_pkt3_pb1 = ugen2_pkt3_pb1;
    o_gen2_sub_pkt3_l.bits.gen2_sub_pkt3_pb0 = ugen2_pkt3_pb0;
    HDMITXRegWrite(pu32RegAddr, o_gen2_sub_pkt3_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gen2_sub_pkt3_l.u32, "GEN2_SUB_PKT3_L");

    return HI_SUCCESS;
}

int HDMI_GEN2_SUB_PKT3_H_gen2_pkt3_pbSet(unsigned int ugen2_pkt3_pb4,unsigned int ugen2_pkt3_pb5,unsigned int ugen2_pkt3_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN2_SUB_PKT3_H o_gen2_sub_pkt3_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN2_SUB_PKT3_H.u32);
    o_gen2_sub_pkt3_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_gen2_sub_pkt3_h.bits.gen2_sub_pkt3_pb6 = ugen2_pkt3_pb6;
    o_gen2_sub_pkt3_h.bits.gen2_sub_pkt3_pb5 = ugen2_pkt3_pb5;
    o_gen2_sub_pkt3_h.bits.gen2_sub_pkt3_pb4 = ugen2_pkt3_pb4;
    HDMITXRegWrite(pu32RegAddr, o_gen2_sub_pkt3_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gen2_sub_pkt3_h.u32, "GEN2_SUB_PKT3_H");

    return HI_SUCCESS;
}


//*************************************************************************************//
//********************************  Gen2    信息帧获取******************************//
//*************************************************************************************//
int HDMI_GEN2_PKT_HEADER_hbGet(U_GEN2_PKT_HEADER* u_gen2_pkt_header)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN2_PKT_HEADER o_gen2_pkt_header;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN2_PKT_HEADER.u32);
    o_gen2_pkt_header.u32 = HDMITXRegRead(pu32RegAddr);
    u_gen2_pkt_header->bits.gen2_pkt_hb2 = o_gen2_pkt_header.bits.gen2_pkt_hb2;
    u_gen2_pkt_header->bits.gen2_pkt_hb1 = o_gen2_pkt_header.bits.gen2_pkt_hb1;
    u_gen2_pkt_header->bits.gen2_pkt_hb0 = o_gen2_pkt_header.bits.gen2_pkt_hb0;
	
	return HI_SUCCESS;
}

int HDMI_GEN2_SUB_PKT0_L_gen2_pkt0_pbGet(U_GEN2_SUB_PKT0_L* u_gen2_sub_pkt0_l)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN2_SUB_PKT0_L o_gen2_sub_pkt0_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN2_SUB_PKT0_L.u32);
    o_gen2_sub_pkt0_l.u32 = HDMITXRegRead(pu32RegAddr);
    u_gen2_sub_pkt0_l->bits.gen2_sub_pkt0_pb3 = o_gen2_sub_pkt0_l.bits.gen2_sub_pkt0_pb3;
    u_gen2_sub_pkt0_l->bits.gen2_sub_pkt0_pb2 = o_gen2_sub_pkt0_l.bits.gen2_sub_pkt0_pb2;
    u_gen2_sub_pkt0_l->bits.gen2_sub_pkt0_pb1 = o_gen2_sub_pkt0_l.bits.gen2_sub_pkt0_pb1;
    u_gen2_sub_pkt0_l->bits.gen2_sub_pkt0_pb0 = o_gen2_sub_pkt0_l.bits.gen2_sub_pkt0_pb0;
	
	return HI_SUCCESS;
}

int HDMI_GEN2_SUB_PKT0_H_gen2_pkt0_pbGet(U_GEN2_SUB_PKT0_H* u_gen2_sub_pkt0_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN2_SUB_PKT0_H o_gen2_sub_pkt0_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN2_SUB_PKT0_H.u32);
    o_gen2_sub_pkt0_h.u32 = HDMITXRegRead(pu32RegAddr);
    u_gen2_sub_pkt0_h->bits.gen2_sub_pkt0_pb6 = o_gen2_sub_pkt0_h.bits.gen2_sub_pkt0_pb6;
    u_gen2_sub_pkt0_h->bits.gen2_sub_pkt0_pb5 = o_gen2_sub_pkt0_h.bits.gen2_sub_pkt0_pb5;
    u_gen2_sub_pkt0_h->bits.gen2_sub_pkt0_pb4 = o_gen2_sub_pkt0_h.bits.gen2_sub_pkt0_pb4;
	
	return HI_SUCCESS;
}


int HDMI_GEN2_SUB_PKT1_L_gen2_pkt1_pbGet(U_GEN2_SUB_PKT1_L* u_gen2_sub_pkt1_l)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN2_SUB_PKT1_L o_gen2_sub_pkt1_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN2_SUB_PKT1_L.u32);
    o_gen2_sub_pkt1_l.u32 = HDMITXRegRead(pu32RegAddr);
    u_gen2_sub_pkt1_l->bits.gen2_sub_pkt1_pb3 = o_gen2_sub_pkt1_l.bits.gen2_sub_pkt1_pb3;
    u_gen2_sub_pkt1_l->bits.gen2_sub_pkt1_pb2 = o_gen2_sub_pkt1_l.bits.gen2_sub_pkt1_pb2;
    u_gen2_sub_pkt1_l->bits.gen2_sub_pkt1_pb1 = o_gen2_sub_pkt1_l.bits.gen2_sub_pkt1_pb1;
    u_gen2_sub_pkt1_l->bits.gen2_sub_pkt1_pb0 = o_gen2_sub_pkt1_l.bits.gen2_sub_pkt1_pb0;
	
	return HI_SUCCESS;
}

int HDMI_GEN2_SUB_PKT1_H_gen2_pkt1_pbGet(U_GEN2_SUB_PKT1_H* u_gen2_sub_pkt1_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN2_SUB_PKT1_H o_gen2_sub_pkt1_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN2_SUB_PKT1_H.u32);
    o_gen2_sub_pkt1_h.u32 = HDMITXRegRead(pu32RegAddr);
    u_gen2_sub_pkt1_h->bits.gen2_sub_pkt1_pb6 = o_gen2_sub_pkt1_h.bits.gen2_sub_pkt1_pb6;
    u_gen2_sub_pkt1_h->bits.gen2_sub_pkt1_pb5 = o_gen2_sub_pkt1_h.bits.gen2_sub_pkt1_pb5;
    u_gen2_sub_pkt1_h->bits.gen2_sub_pkt1_pb4 = o_gen2_sub_pkt1_h.bits.gen2_sub_pkt1_pb4;
	
	return HI_SUCCESS;
}

int HDMI_GEN2_SUB_PKT2_L_gen2_pkt2_pbGet(U_GEN2_SUB_PKT2_L* u_gen2_sub_pkt2_l)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN2_SUB_PKT2_L o_gen2_sub_pkt2_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN2_SUB_PKT2_L.u32);
    o_gen2_sub_pkt2_l.u32 = HDMITXRegRead(pu32RegAddr);
    u_gen2_sub_pkt2_l->bits.gen2_sub_pkt2_pb3 = o_gen2_sub_pkt2_l.bits.gen2_sub_pkt2_pb3;
    u_gen2_sub_pkt2_l->bits.gen2_sub_pkt2_pb2 = o_gen2_sub_pkt2_l.bits.gen2_sub_pkt2_pb2;
    u_gen2_sub_pkt2_l->bits.gen2_sub_pkt2_pb1 = o_gen2_sub_pkt2_l.bits.gen2_sub_pkt2_pb1;
    u_gen2_sub_pkt2_l->bits.gen2_sub_pkt2_pb0 = o_gen2_sub_pkt2_l.bits.gen2_sub_pkt2_pb0;
	
	return HI_SUCCESS;
}

int HDMI_GEN2_SUB_PKT2_H_gen2_pkt2_pbGet(U_GEN2_SUB_PKT2_H* u_gen2_sub_pkt2_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN2_SUB_PKT2_H o_gen2_sub_pkt2_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN2_SUB_PKT2_H.u32);
    o_gen2_sub_pkt2_h.u32 = HDMITXRegRead(pu32RegAddr);
    u_gen2_sub_pkt2_h->bits.gen2_sub_pkt2_pb6 = o_gen2_sub_pkt2_h.bits.gen2_sub_pkt2_pb6;
    u_gen2_sub_pkt2_h->bits.gen2_sub_pkt2_pb5 = o_gen2_sub_pkt2_h.bits.gen2_sub_pkt2_pb5;
    u_gen2_sub_pkt2_h->bits.gen2_sub_pkt2_pb4 = o_gen2_sub_pkt2_h.bits.gen2_sub_pkt2_pb4;
	
	return HI_SUCCESS;
}

int HDMI_GEN2_SUB_PKT3_L_gen2_pkt3_pbGet(U_GEN2_SUB_PKT3_L* u_gen2_sub_pkt3_l)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN2_SUB_PKT3_L o_gen2_sub_pkt3_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN2_SUB_PKT3_L.u32);
    o_gen2_sub_pkt3_l.u32 = HDMITXRegRead(pu32RegAddr);
    u_gen2_sub_pkt3_l->bits.gen2_sub_pkt3_pb3 = o_gen2_sub_pkt3_l.bits.gen2_sub_pkt3_pb3;
    u_gen2_sub_pkt3_l->bits.gen2_sub_pkt3_pb2 = o_gen2_sub_pkt3_l.bits.gen2_sub_pkt3_pb2;
    u_gen2_sub_pkt3_l->bits.gen2_sub_pkt3_pb1 = o_gen2_sub_pkt3_l.bits.gen2_sub_pkt3_pb1;
    u_gen2_sub_pkt3_l->bits.gen2_sub_pkt3_pb0 = o_gen2_sub_pkt3_l.bits.gen2_sub_pkt3_pb0;
	
	return HI_SUCCESS;
}

int HDMI_GEN2_SUB_PKT3_H_gen2_pkt3_pbGet(U_GEN2_SUB_PKT3_H* u_gen2_sub_pkt3_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN2_SUB_PKT3_H o_gen2_sub_pkt3_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN2_SUB_PKT3_H.u32);
    o_gen2_sub_pkt3_h.u32 = HDMITXRegRead(pu32RegAddr);
    u_gen2_sub_pkt3_h->bits.gen2_sub_pkt3_pb6 = o_gen2_sub_pkt3_h.bits.gen2_sub_pkt3_pb6;
    u_gen2_sub_pkt3_h->bits.gen2_sub_pkt3_pb5 = o_gen2_sub_pkt3_h.bits.gen2_sub_pkt3_pb5;
    u_gen2_sub_pkt3_h->bits.gen2_sub_pkt3_pb4 = o_gen2_sub_pkt3_h.bits.gen2_sub_pkt3_pb4;
	
	return HI_SUCCESS;
}

//*************************************************************************************//
//********************************  Gen3    信息帧设置******************************//
//*************************************************************************************//
int HDMI_GEN3_PKT_HEADER_hbSet(unsigned int uhb0,unsigned int uhb1,unsigned int uhb2)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN3_PKT_HEADER o_gen3_pkt_header;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN3_PKT_HEADER.u32);
    o_gen3_pkt_header.u32 = HDMITXRegRead(pu32RegAddr);
    o_gen3_pkt_header.bits.gen3_pkt_hb2= uhb2;
    o_gen3_pkt_header.bits.gen3_pkt_hb1= uhb1;
    o_gen3_pkt_header.bits.gen3_pkt_hb0= uhb0;
    HDMITXRegWrite(pu32RegAddr, o_gen3_pkt_header.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gen3_pkt_header.u32, "GEN3_PKT_HEADER");

    return HI_SUCCESS;
}

int HDMI_GEN3_SUB_PKT0_L_gen3_pkt0_pbSet(unsigned int ugen3_pkt0_pb0,unsigned int ugen3_pkt0_pb1,unsigned int ugen3_pkt0_pb2,unsigned int ugen3_pkt0_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN3_SUB_PKT0_L o_gen3_sub_pkt0_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN3_SUB_PKT0_L.u32);
    o_gen3_sub_pkt0_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_gen3_sub_pkt0_l.bits.gen3_sub_pkt0_pb3 = ugen3_pkt0_pb3;
    o_gen3_sub_pkt0_l.bits.gen3_sub_pkt0_pb2 = ugen3_pkt0_pb2;
    o_gen3_sub_pkt0_l.bits.gen3_sub_pkt0_pb1 = ugen3_pkt0_pb1;
    o_gen3_sub_pkt0_l.bits.gen3_sub_pkt0_pb0 = ugen3_pkt0_pb0;
    HDMITXRegWrite(pu32RegAddr, o_gen3_sub_pkt0_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gen3_sub_pkt0_l.u32, "GEN3_SUB_PKT0_L");

    return HI_SUCCESS;
}

int HDMI_GEN3_SUB_PKT0_H_gen3_pkt0_pbSet(unsigned int ugen3_pkt0_pb4,unsigned int ugen3_pkt0_pb5,unsigned int ugen3_pkt0_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN3_SUB_PKT0_H o_gen3_sub_pkt0_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN3_SUB_PKT0_H.u32);
    o_gen3_sub_pkt0_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_gen3_sub_pkt0_h.bits.gen3_sub_pkt0_pb6 = ugen3_pkt0_pb6;
    o_gen3_sub_pkt0_h.bits.gen3_sub_pkt0_pb5 = ugen3_pkt0_pb5;
    o_gen3_sub_pkt0_h.bits.gen3_sub_pkt0_pb4 = ugen3_pkt0_pb4;
    HDMITXRegWrite(pu32RegAddr, o_gen3_sub_pkt0_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gen3_sub_pkt0_h.u32, "GEN3_SUB_PKT0_H");

    return HI_SUCCESS;
}

int HDMI_GEN3_SUB_PKT1_L_gen3_pkt1_pbSet(unsigned int ugen3_pkt1_pb0,unsigned int ugen3_pkt1_pb1,unsigned int ugen3_pkt1_pb2,unsigned int ugen3_pkt1_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN3_SUB_PKT1_L o_gen3_sub_pkt1_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN3_SUB_PKT1_L.u32);
    o_gen3_sub_pkt1_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_gen3_sub_pkt1_l.bits.gen3_sub_pkt1_pb3 = ugen3_pkt1_pb3;
    o_gen3_sub_pkt1_l.bits.gen3_sub_pkt1_pb2 = ugen3_pkt1_pb2;
    o_gen3_sub_pkt1_l.bits.gen3_sub_pkt1_pb1 = ugen3_pkt1_pb1;
    o_gen3_sub_pkt1_l.bits.gen3_sub_pkt1_pb0 = ugen3_pkt1_pb0;
    HDMITXRegWrite(pu32RegAddr, o_gen3_sub_pkt1_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gen3_sub_pkt1_l.u32, "GEN3_SUB_PKT1_L");

    return HI_SUCCESS;
}

int HDMI_GEN3_SUB_PKT1_H_gen3_pkt1_pbSet(unsigned int ugen3_pkt1_pb4,unsigned int ugen3_pkt1_pb5,unsigned int ugen3_pkt1_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN3_SUB_PKT1_H o_gen3_sub_pkt1_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN3_SUB_PKT1_H.u32);
    o_gen3_sub_pkt1_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_gen3_sub_pkt1_h.bits.gen3_sub_pkt1_pb6 = ugen3_pkt1_pb6;
    o_gen3_sub_pkt1_h.bits.gen3_sub_pkt1_pb5 = ugen3_pkt1_pb5;
    o_gen3_sub_pkt1_h.bits.gen3_sub_pkt1_pb4 = ugen3_pkt1_pb4;
    HDMITXRegWrite(pu32RegAddr, o_gen3_sub_pkt1_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gen3_sub_pkt1_h.u32, "GEN3_SUB_PKT1_H");

    return HI_SUCCESS;
}

int HDMI_GEN3_SUB_PKT2_L_gen3_pkt2_pbSet(unsigned int ugen3_pkt2_pb0,unsigned int ugen3_pkt2_pb1,unsigned int ugen3_pkt2_pb2,unsigned int ugen3_pkt2_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN3_SUB_PKT2_L o_gen3_sub_pkt2_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN3_SUB_PKT2_L.u32);
    o_gen3_sub_pkt2_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_gen3_sub_pkt2_l.bits.gen3_sub_pkt2_pb3 = ugen3_pkt2_pb3;
    o_gen3_sub_pkt2_l.bits.gen3_sub_pkt2_pb2 = ugen3_pkt2_pb2;
    o_gen3_sub_pkt2_l.bits.gen3_sub_pkt2_pb1 = ugen3_pkt2_pb1;
    o_gen3_sub_pkt2_l.bits.gen3_sub_pkt2_pb0 = ugen3_pkt2_pb0;
    HDMITXRegWrite(pu32RegAddr, o_gen3_sub_pkt2_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gen3_sub_pkt2_l.u32, "GEN3_SUB_PKT2_L");

    return HI_SUCCESS;
}

int HDMI_GEN3_SUB_PKT2_H_gen3_pkt2_pbSet(unsigned int ugen3_pkt2_pb4,unsigned int ugen3_pkt2_pb5,unsigned int ugen3_pkt2_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN3_SUB_PKT2_H o_gen3_sub_pkt2_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN3_SUB_PKT2_H.u32);
    o_gen3_sub_pkt2_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_gen3_sub_pkt2_h.bits.gen3_sub_pkt2_pb6 = ugen3_pkt2_pb6;
    o_gen3_sub_pkt2_h.bits.gen3_sub_pkt2_pb5 = ugen3_pkt2_pb5;
    o_gen3_sub_pkt2_h.bits.gen3_sub_pkt2_pb4 = ugen3_pkt2_pb4;
    HDMITXRegWrite(pu32RegAddr, o_gen3_sub_pkt2_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gen3_sub_pkt2_h.u32, "GEN3_SUB_PKT2_H");

    return HI_SUCCESS;
}

int HDMI_GEN3_SUB_PKT3_L_gen3_pkt3_pbSet(unsigned int ugen3_pkt3_pb0,unsigned int ugen3_pkt3_pb1,unsigned int ugen3_pkt3_pb2,unsigned int ugen3_pkt3_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN3_SUB_PKT3_L o_gen3_sub_pkt3_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN3_SUB_PKT3_L.u32);
    o_gen3_sub_pkt3_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_gen3_sub_pkt3_l.bits.gen3_sub_pkt3_pb3 = ugen3_pkt3_pb3;
    o_gen3_sub_pkt3_l.bits.gen3_sub_pkt3_pb2 = ugen3_pkt3_pb2;
    o_gen3_sub_pkt3_l.bits.gen3_sub_pkt3_pb1 = ugen3_pkt3_pb1;
    o_gen3_sub_pkt3_l.bits.gen3_sub_pkt3_pb0 = ugen3_pkt3_pb0;
    HDMITXRegWrite(pu32RegAddr, o_gen3_sub_pkt3_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gen3_sub_pkt3_l.u32, "GEN3_SUB_PKT3_L");

    return HI_SUCCESS;
}

int HDMI_GEN3_SUB_PKT3_H_gen3_pkt3_pbSet(unsigned int ugen3_pkt3_pb4,unsigned int ugen3_pkt3_pb5,unsigned int ugen3_pkt3_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN3_SUB_PKT3_H o_gen3_sub_pkt3_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN3_SUB_PKT3_H.u32);
    o_gen3_sub_pkt3_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_gen3_sub_pkt3_h.bits.gen3_sub_pkt3_pb6 = ugen3_pkt3_pb6;
    o_gen3_sub_pkt3_h.bits.gen3_sub_pkt3_pb5 = ugen3_pkt3_pb5;
    o_gen3_sub_pkt3_h.bits.gen3_sub_pkt3_pb4 = ugen3_pkt3_pb4;
    HDMITXRegWrite(pu32RegAddr, o_gen3_sub_pkt3_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gen3_sub_pkt3_h.u32, "GEN3_SUB_PKT3_H");

    return HI_SUCCESS;
}


//*************************************************************************************//
//********************************  Gen3    信息帧获取******************************//
//*************************************************************************************//
int HDMI_GEN3_PKT_HEADER_hbGet(U_GEN3_PKT_HEADER* u_gen3_pkt_header)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN3_PKT_HEADER o_gen3_pkt_header;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN3_PKT_HEADER.u32);
    o_gen3_pkt_header.u32 = HDMITXRegRead(pu32RegAddr);
    u_gen3_pkt_header->bits.gen3_pkt_hb2 = o_gen3_pkt_header.bits.gen3_pkt_hb2;
    u_gen3_pkt_header->bits.gen3_pkt_hb1 = o_gen3_pkt_header.bits.gen3_pkt_hb1;
    u_gen3_pkt_header->bits.gen3_pkt_hb0 = o_gen3_pkt_header.bits.gen3_pkt_hb0;
	
	return HI_SUCCESS;
}

int HDMI_GEN3_SUB_PKT0_L_gen3_pkt0_pbGet(U_GEN3_SUB_PKT0_L* u_gen3_sub_pkt0_l)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN3_SUB_PKT0_L o_gen3_sub_pkt0_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN3_SUB_PKT0_L.u32);
    o_gen3_sub_pkt0_l.u32 = HDMITXRegRead(pu32RegAddr);
    u_gen3_sub_pkt0_l->bits.gen3_sub_pkt0_pb3 = o_gen3_sub_pkt0_l.bits.gen3_sub_pkt0_pb3;
    u_gen3_sub_pkt0_l->bits.gen3_sub_pkt0_pb2 = o_gen3_sub_pkt0_l.bits.gen3_sub_pkt0_pb2;
    u_gen3_sub_pkt0_l->bits.gen3_sub_pkt0_pb1 = o_gen3_sub_pkt0_l.bits.gen3_sub_pkt0_pb1;
    u_gen3_sub_pkt0_l->bits.gen3_sub_pkt0_pb0 = o_gen3_sub_pkt0_l.bits.gen3_sub_pkt0_pb0;
	
	return HI_SUCCESS;
}


int HDMI_GEN3_SUB_PKT0_H_gen3_pkt0_pbGet(U_GEN3_SUB_PKT0_H* u_gen3_sub_pkt0_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN3_SUB_PKT0_H o_gen3_sub_pkt0_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN3_SUB_PKT0_H.u32);
    o_gen3_sub_pkt0_h.u32 = HDMITXRegRead(pu32RegAddr);
    u_gen3_sub_pkt0_h->bits.gen3_sub_pkt0_pb6 = o_gen3_sub_pkt0_h.bits.gen3_sub_pkt0_pb6;
    u_gen3_sub_pkt0_h->bits.gen3_sub_pkt0_pb5 = o_gen3_sub_pkt0_h.bits.gen3_sub_pkt0_pb5;
    u_gen3_sub_pkt0_h->bits.gen3_sub_pkt0_pb4 = o_gen3_sub_pkt0_h.bits.gen3_sub_pkt0_pb4;
	
	return HI_SUCCESS;
}

int HDMI_GEN3_SUB_PKT1_L_gen3_pkt1_pbGet(U_GEN3_SUB_PKT1_L* u_gen3_sub_pkt1_l)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN3_SUB_PKT1_L o_gen3_sub_pkt1_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN3_SUB_PKT1_L.u32);
    o_gen3_sub_pkt1_l.u32 = HDMITXRegRead(pu32RegAddr);
    u_gen3_sub_pkt1_l->bits.gen3_sub_pkt1_pb3 = o_gen3_sub_pkt1_l.bits.gen3_sub_pkt1_pb3;
    u_gen3_sub_pkt1_l->bits.gen3_sub_pkt1_pb2 = o_gen3_sub_pkt1_l.bits.gen3_sub_pkt1_pb2;
    u_gen3_sub_pkt1_l->bits.gen3_sub_pkt1_pb1 = o_gen3_sub_pkt1_l.bits.gen3_sub_pkt1_pb1;
    u_gen3_sub_pkt1_l->bits.gen3_sub_pkt1_pb0 = o_gen3_sub_pkt1_l.bits.gen3_sub_pkt1_pb0;
	
	return HI_SUCCESS;
}

int HDMI_GEN3_SUB_PKT1_H_gen3_pkt1_pbGet(U_GEN3_SUB_PKT1_H* u_gen3_sub_pkt1_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN3_SUB_PKT1_H o_gen3_sub_pkt1_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN3_SUB_PKT1_H.u32);
    o_gen3_sub_pkt1_h.u32 = HDMITXRegRead(pu32RegAddr);
    u_gen3_sub_pkt1_h->bits.gen3_sub_pkt1_pb6 = o_gen3_sub_pkt1_h.bits.gen3_sub_pkt1_pb6;
    u_gen3_sub_pkt1_h->bits.gen3_sub_pkt1_pb5 = o_gen3_sub_pkt1_h.bits.gen3_sub_pkt1_pb5;
    u_gen3_sub_pkt1_h->bits.gen3_sub_pkt1_pb4 = o_gen3_sub_pkt1_h.bits.gen3_sub_pkt1_pb4;
	
	return HI_SUCCESS;
}

int HDMI_GEN3_SUB_PKT2_L_gen3_pkt2_pbGet(U_GEN3_SUB_PKT2_L* u_gen3_sub_pkt2_l)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN3_SUB_PKT2_L o_gen3_sub_pkt2_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN3_SUB_PKT2_L.u32);
    o_gen3_sub_pkt2_l.u32 = HDMITXRegRead(pu32RegAddr);
    u_gen3_sub_pkt2_l->bits.gen3_sub_pkt2_pb3 = o_gen3_sub_pkt2_l.bits.gen3_sub_pkt2_pb3;
    u_gen3_sub_pkt2_l->bits.gen3_sub_pkt2_pb2 = o_gen3_sub_pkt2_l.bits.gen3_sub_pkt2_pb2;
    u_gen3_sub_pkt2_l->bits.gen3_sub_pkt2_pb1 = o_gen3_sub_pkt2_l.bits.gen3_sub_pkt2_pb1;
    u_gen3_sub_pkt2_l->bits.gen3_sub_pkt2_pb0 = o_gen3_sub_pkt2_l.bits.gen3_sub_pkt2_pb0;
	
	return HI_SUCCESS;
}

int HDMI_GEN3_SUB_PKT2_H_gen3_pkt2_pbGet(U_GEN3_SUB_PKT2_H* u_gen3_sub_pkt2_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN3_SUB_PKT2_H o_gen3_sub_pkt2_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN3_SUB_PKT2_H.u32);
    o_gen3_sub_pkt2_h.u32 = HDMITXRegRead(pu32RegAddr);
    u_gen3_sub_pkt2_h->bits.gen3_sub_pkt2_pb6 = o_gen3_sub_pkt2_h.bits.gen3_sub_pkt2_pb6;
    u_gen3_sub_pkt2_h->bits.gen3_sub_pkt2_pb5 = o_gen3_sub_pkt2_h.bits.gen3_sub_pkt2_pb5;
    u_gen3_sub_pkt2_h->bits.gen3_sub_pkt2_pb4 = o_gen3_sub_pkt2_h.bits.gen3_sub_pkt2_pb4;
	
	return HI_SUCCESS;
}

int HDMI_GEN3_SUB_PKT3_L_gen3_pkt3_pbGet(U_GEN3_SUB_PKT3_L* u_gen3_sub_pkt3_l)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN3_SUB_PKT3_L o_gen3_sub_pkt3_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN3_SUB_PKT3_L.u32);
    o_gen3_sub_pkt3_l.u32 = HDMITXRegRead(pu32RegAddr);
    u_gen3_sub_pkt3_l->bits.gen3_sub_pkt3_pb3 = o_gen3_sub_pkt3_l.bits.gen3_sub_pkt3_pb3;
    u_gen3_sub_pkt3_l->bits.gen3_sub_pkt3_pb2 = o_gen3_sub_pkt3_l.bits.gen3_sub_pkt3_pb2;
    u_gen3_sub_pkt3_l->bits.gen3_sub_pkt3_pb1 = o_gen3_sub_pkt3_l.bits.gen3_sub_pkt3_pb1;
    u_gen3_sub_pkt3_l->bits.gen3_sub_pkt3_pb0 = o_gen3_sub_pkt3_l.bits.gen3_sub_pkt3_pb0;
	
	return HI_SUCCESS;
}

int HDMI_GEN3_SUB_PKT3_H_gen3_pkt3_pbGet(U_GEN3_SUB_PKT3_H* u_gen3_sub_pkt3_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN3_SUB_PKT3_H o_gen3_sub_pkt3_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN3_SUB_PKT3_H.u32);
    o_gen3_sub_pkt3_h.u32 = HDMITXRegRead(pu32RegAddr);
    u_gen3_sub_pkt3_h->bits.gen3_sub_pkt3_pb6 = o_gen3_sub_pkt3_h.bits.gen3_sub_pkt3_pb6;
    u_gen3_sub_pkt3_h->bits.gen3_sub_pkt3_pb5 = o_gen3_sub_pkt3_h.bits.gen3_sub_pkt3_pb5;
    u_gen3_sub_pkt3_h->bits.gen3_sub_pkt3_pb4 = o_gen3_sub_pkt3_h.bits.gen3_sub_pkt3_pb4;
	
	return HI_SUCCESS;
}

//*************************************************************************************//
//********************************  Gen4    信息帧设置******************************//
//*************************************************************************************//
int HDMI_GEN4_PKT_HEADER_hbSet(unsigned int uhb0,unsigned int uhb1,unsigned int uhb2)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN4_PKT_HEADER o_gen4_pkt_header;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN4_PKT_HEADER.u32);
    o_gen4_pkt_header.u32 = HDMITXRegRead(pu32RegAddr);
    o_gen4_pkt_header.bits.gen4_pkt_hb2= uhb2;
    o_gen4_pkt_header.bits.gen4_pkt_hb1= uhb1;
    o_gen4_pkt_header.bits.gen4_pkt_hb0= uhb0;
    HDMITXRegWrite(pu32RegAddr, o_gen4_pkt_header.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gen4_pkt_header.u32, "GEN4_PKT_HEADER");

    return HI_SUCCESS;
}

int HDMI_GEN4_SUB_PKT0_L_gen4_pkt0_pbSet(unsigned int ugen4_pkt0_pb0,unsigned int ugen4_pkt0_pb1,unsigned int ugen4_pkt0_pb2,unsigned int ugen4_pkt0_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN4_SUB_PKT0_L o_gen4_sub_pkt0_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN4_SUB_PKT0_L.u32);
    o_gen4_sub_pkt0_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_gen4_sub_pkt0_l.bits.gen4_sub_pkt0_pb3 = ugen4_pkt0_pb3;
    o_gen4_sub_pkt0_l.bits.gen4_sub_pkt0_pb2 = ugen4_pkt0_pb2;
    o_gen4_sub_pkt0_l.bits.gen4_sub_pkt0_pb1 = ugen4_pkt0_pb1;
    o_gen4_sub_pkt0_l.bits.gen4_sub_pkt0_pb0 = ugen4_pkt0_pb0;
    HDMITXRegWrite(pu32RegAddr, o_gen4_sub_pkt0_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gen4_sub_pkt0_l.u32, "GEN4_SUB_PKT0_L");

    return HI_SUCCESS;
}

int HDMI_GEN4_SUB_PKT0_H_gen4_pkt0_pbSet(unsigned int ugen4_pkt0_pb4,unsigned int ugen4_pkt0_pb5,unsigned int ugen4_pkt0_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN4_SUB_PKT0_H o_gen4_sub_pkt0_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN4_SUB_PKT0_H.u32);
    o_gen4_sub_pkt0_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_gen4_sub_pkt0_h.bits.gen4_sub_pkt0_pb6 = ugen4_pkt0_pb6;
    o_gen4_sub_pkt0_h.bits.gen4_sub_pkt0_pb5 = ugen4_pkt0_pb5;
    o_gen4_sub_pkt0_h.bits.gen4_sub_pkt0_pb4 = ugen4_pkt0_pb4;
    HDMITXRegWrite(pu32RegAddr, o_gen4_sub_pkt0_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gen4_sub_pkt0_h.u32, "GEN4_SUB_PKT0_H");

    return HI_SUCCESS;
}

int HDMI_GEN4_SUB_PKT1_L_gen4_pkt1_pbSet(unsigned int ugen4_pkt1_pb0,unsigned int ugen4_pkt1_pb1,unsigned int ugen4_pkt1_pb2,unsigned int ugen4_pkt1_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN4_SUB_PKT1_L o_gen4_sub_pkt1_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN4_SUB_PKT1_L.u32);
    o_gen4_sub_pkt1_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_gen4_sub_pkt1_l.bits.gen4_sub_pkt1_pb3 = ugen4_pkt1_pb3;
    o_gen4_sub_pkt1_l.bits.gen4_sub_pkt1_pb2 = ugen4_pkt1_pb2;
    o_gen4_sub_pkt1_l.bits.gen4_sub_pkt1_pb1 = ugen4_pkt1_pb1;
    o_gen4_sub_pkt1_l.bits.gen4_sub_pkt1_pb0 = ugen4_pkt1_pb0;
    HDMITXRegWrite(pu32RegAddr, o_gen4_sub_pkt1_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gen4_sub_pkt1_l.u32, "GEN4_SUB_PKT1_L");

    return HI_SUCCESS;
}

int HDMI_GEN4_SUB_PKT1_H_gen4_pkt1_pbSet(unsigned int ugen4_pkt1_pb4,unsigned int ugen4_pkt1_pb5,unsigned int ugen4_pkt1_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN4_SUB_PKT1_H o_gen4_sub_pkt1_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN4_SUB_PKT1_H.u32);
    o_gen4_sub_pkt1_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_gen4_sub_pkt1_h.bits.gen4_sub_pkt1_pb6 = ugen4_pkt1_pb6;
    o_gen4_sub_pkt1_h.bits.gen4_sub_pkt1_pb5 = ugen4_pkt1_pb5;
    o_gen4_sub_pkt1_h.bits.gen4_sub_pkt1_pb4 = ugen4_pkt1_pb4;
    HDMITXRegWrite(pu32RegAddr, o_gen4_sub_pkt1_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gen4_sub_pkt1_h.u32, "GEN4_SUB_PKT1_H");

    return HI_SUCCESS;
}

int HDMI_GEN4_SUB_PKT2_L_gen4_pkt2_pbSet(unsigned int ugen4_pkt2_pb0,unsigned int ugen4_pkt2_pb1,unsigned int ugen4_pkt2_pb2,unsigned int ugen4_pkt2_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN4_SUB_PKT2_L o_gen4_sub_pkt2_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN4_SUB_PKT2_L.u32);
    o_gen4_sub_pkt2_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_gen4_sub_pkt2_l.bits.gen4_sub_pkt2_pb3 = ugen4_pkt2_pb3;
    o_gen4_sub_pkt2_l.bits.gen4_sub_pkt2_pb2 = ugen4_pkt2_pb2;
    o_gen4_sub_pkt2_l.bits.gen4_sub_pkt2_pb1 = ugen4_pkt2_pb1;
    o_gen4_sub_pkt2_l.bits.gen4_sub_pkt2_pb0 = ugen4_pkt2_pb0;
    HDMITXRegWrite(pu32RegAddr, o_gen4_sub_pkt2_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gen4_sub_pkt2_l.u32, "GEN4_SUB_PKT2_L");

    return HI_SUCCESS;
}

int HDMI_GEN4_SUB_PKT2_H_gen4_pkt2_pbSet(unsigned int ugen4_pkt2_pb4,unsigned int ugen4_pkt2_pb5,unsigned int ugen4_pkt2_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN4_SUB_PKT2_H o_gen4_sub_pkt2_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN4_SUB_PKT2_H.u32);
    o_gen4_sub_pkt2_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_gen4_sub_pkt2_h.bits.gen4_sub_pkt2_pb6 = ugen4_pkt2_pb6;
    o_gen4_sub_pkt2_h.bits.gen4_sub_pkt2_pb5 = ugen4_pkt2_pb5;
    o_gen4_sub_pkt2_h.bits.gen4_sub_pkt2_pb4 = ugen4_pkt2_pb4;
    HDMITXRegWrite(pu32RegAddr, o_gen4_sub_pkt2_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gen4_sub_pkt2_h.u32, "GEN4_SUB_PKT2_H");

    return HI_SUCCESS;
}

int HDMI_GEN4_SUB_PKT3_L_gen4_pkt3_pbSet(unsigned int ugen4_pkt3_pb0,unsigned int ugen4_pkt3_pb1,unsigned int ugen4_pkt3_pb2,unsigned int ugen4_pkt3_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN4_SUB_PKT3_L o_gen4_sub_pkt3_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN4_SUB_PKT3_L.u32);
    o_gen4_sub_pkt3_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_gen4_sub_pkt3_l.bits.gen4_sub_pkt3_pb3 = ugen4_pkt3_pb3;
    o_gen4_sub_pkt3_l.bits.gen4_sub_pkt3_pb2 = ugen4_pkt3_pb2;
    o_gen4_sub_pkt3_l.bits.gen4_sub_pkt3_pb1 = ugen4_pkt3_pb1;
    o_gen4_sub_pkt3_l.bits.gen4_sub_pkt3_pb0 = ugen4_pkt3_pb0;
    HDMITXRegWrite(pu32RegAddr, o_gen4_sub_pkt3_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gen4_sub_pkt3_l.u32, "GEN4_SUB_PKT3_L");

    return HI_SUCCESS;
}

int HDMI_GEN4_SUB_PKT3_H_gen4_pkt3_pbSet(unsigned int ugen4_pkt3_pb4,unsigned int ugen4_pkt3_pb5,unsigned int ugen4_pkt3_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN4_SUB_PKT3_H o_gen4_sub_pkt3_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN4_SUB_PKT3_H.u32);
    o_gen4_sub_pkt3_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_gen4_sub_pkt3_h.bits.gen4_sub_pkt3_pb6 = ugen4_pkt3_pb6;
    o_gen4_sub_pkt3_h.bits.gen4_sub_pkt3_pb5 = ugen4_pkt3_pb5;
    o_gen4_sub_pkt3_h.bits.gen4_sub_pkt3_pb4 = ugen4_pkt3_pb4;
    HDMITXRegWrite(pu32RegAddr, o_gen4_sub_pkt3_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gen4_sub_pkt3_h.u32, "GEN4_SUB_PKT3_H");

    return HI_SUCCESS;
}


//*************************************************************************************//
//********************************  Gen4    信息帧获取******************************//
//*************************************************************************************//
int HDMI_GEN4_PKT_HEADER_hbGet(U_GEN4_PKT_HEADER* u_gen4_pkt_header)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN4_PKT_HEADER o_gen4_pkt_header;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN4_PKT_HEADER.u32);
    o_gen4_pkt_header.u32 = HDMITXRegRead(pu32RegAddr);
    u_gen4_pkt_header->bits.gen4_pkt_hb2 = o_gen4_pkt_header.bits.gen4_pkt_hb2;
    u_gen4_pkt_header->bits.gen4_pkt_hb1 = o_gen4_pkt_header.bits.gen4_pkt_hb1;
    u_gen4_pkt_header->bits.gen4_pkt_hb0 = o_gen4_pkt_header.bits.gen4_pkt_hb0;

	return HI_SUCCESS;
}

int HDMI_GEN4_SUB_PKT0_L_gen4_pkt0_pbGet(U_GEN4_SUB_PKT0_L* u_gen4_sub_pkt0_l)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN4_SUB_PKT0_L o_gen4_sub_pkt0_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN4_SUB_PKT0_L.u32);
    o_gen4_sub_pkt0_l.u32 = HDMITXRegRead(pu32RegAddr);
    u_gen4_sub_pkt0_l->bits.gen4_sub_pkt0_pb3 = o_gen4_sub_pkt0_l.bits.gen4_sub_pkt0_pb3;
    u_gen4_sub_pkt0_l->bits.gen4_sub_pkt0_pb2 = o_gen4_sub_pkt0_l.bits.gen4_sub_pkt0_pb2;
    u_gen4_sub_pkt0_l->bits.gen4_sub_pkt0_pb1 = o_gen4_sub_pkt0_l.bits.gen4_sub_pkt0_pb1;
    u_gen4_sub_pkt0_l->bits.gen4_sub_pkt0_pb0 = o_gen4_sub_pkt0_l.bits.gen4_sub_pkt0_pb0;

    return HI_SUCCESS;
}

int HDMI_GEN4_SUB_PKT0_H_gen4_pkt0_pbGet(U_GEN4_SUB_PKT0_H* u_gen4_sub_pkt0_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN4_SUB_PKT0_H o_gen4_sub_pkt0_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN4_SUB_PKT0_H.u32);
    o_gen4_sub_pkt0_h.u32 = HDMITXRegRead(pu32RegAddr);
    u_gen4_sub_pkt0_h->bits.gen4_sub_pkt0_pb6 = o_gen4_sub_pkt0_h.bits.gen4_sub_pkt0_pb6;
    u_gen4_sub_pkt0_h->bits.gen4_sub_pkt0_pb5 = o_gen4_sub_pkt0_h.bits.gen4_sub_pkt0_pb5;
    u_gen4_sub_pkt0_h->bits.gen4_sub_pkt0_pb4 = o_gen4_sub_pkt0_h.bits.gen4_sub_pkt0_pb4;

    return HI_SUCCESS;
}

int HDMI_GEN4_SUB_PKT1_L_gen4_pkt1_pbGet(U_GEN4_SUB_PKT1_L* u_gen4_sub_pkt1_l)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN4_SUB_PKT1_L o_gen4_sub_pkt1_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN4_SUB_PKT1_L.u32);
    o_gen4_sub_pkt1_l.u32 = HDMITXRegRead(pu32RegAddr);
    u_gen4_sub_pkt1_l->bits.gen4_sub_pkt1_pb3 = o_gen4_sub_pkt1_l.bits.gen4_sub_pkt1_pb3;
    u_gen4_sub_pkt1_l->bits.gen4_sub_pkt1_pb2 = o_gen4_sub_pkt1_l.bits.gen4_sub_pkt1_pb2;
    u_gen4_sub_pkt1_l->bits.gen4_sub_pkt1_pb1 = o_gen4_sub_pkt1_l.bits.gen4_sub_pkt1_pb1;
    u_gen4_sub_pkt1_l->bits.gen4_sub_pkt1_pb0 = o_gen4_sub_pkt1_l.bits.gen4_sub_pkt1_pb0;

    return HI_SUCCESS;
}

int HDMI_GEN4_SUB_PKT1_H_gen4_pkt1_pbGet(U_GEN4_SUB_PKT1_H* u_gen4_sub_pkt1_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN4_SUB_PKT1_H o_gen4_sub_pkt1_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN4_SUB_PKT1_H.u32);
    o_gen4_sub_pkt1_h.u32 = HDMITXRegRead(pu32RegAddr);
    u_gen4_sub_pkt1_h->bits.gen4_sub_pkt1_pb6 = o_gen4_sub_pkt1_h.bits.gen4_sub_pkt1_pb6;
    u_gen4_sub_pkt1_h->bits.gen4_sub_pkt1_pb5 = o_gen4_sub_pkt1_h.bits.gen4_sub_pkt1_pb5;
    u_gen4_sub_pkt1_h->bits.gen4_sub_pkt1_pb4 = o_gen4_sub_pkt1_h.bits.gen4_sub_pkt1_pb4;

    return HI_SUCCESS;
}

int HDMI_GEN4_SUB_PKT2_L_gen4_pkt2_pbGet(U_GEN4_SUB_PKT2_L* u_gen4_sub_pkt2_l)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN4_SUB_PKT2_L o_gen4_sub_pkt2_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN4_SUB_PKT2_L.u32);
    o_gen4_sub_pkt2_l.u32 = HDMITXRegRead(pu32RegAddr);
    u_gen4_sub_pkt2_l->bits.gen4_sub_pkt2_pb3 = o_gen4_sub_pkt2_l.bits.gen4_sub_pkt2_pb3;
    u_gen4_sub_pkt2_l->bits.gen4_sub_pkt2_pb2 = o_gen4_sub_pkt2_l.bits.gen4_sub_pkt2_pb2;
    u_gen4_sub_pkt2_l->bits.gen4_sub_pkt2_pb1 = o_gen4_sub_pkt2_l.bits.gen4_sub_pkt2_pb1;
    u_gen4_sub_pkt2_l->bits.gen4_sub_pkt2_pb0 = o_gen4_sub_pkt2_l.bits.gen4_sub_pkt2_pb0;

    return HI_SUCCESS;
}

int HDMI_GEN4_SUB_PKT2_H_gen4_pkt2_pbGet(U_GEN4_SUB_PKT2_H* u_gen4_sub_pkt2_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN4_SUB_PKT2_H o_gen4_sub_pkt2_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN4_SUB_PKT2_H.u32);
    o_gen4_sub_pkt2_h.u32 = HDMITXRegRead(pu32RegAddr);
    u_gen4_sub_pkt2_h->bits.gen4_sub_pkt2_pb6 = o_gen4_sub_pkt2_h.bits.gen4_sub_pkt2_pb6;
    u_gen4_sub_pkt2_h->bits.gen4_sub_pkt2_pb5 = o_gen4_sub_pkt2_h.bits.gen4_sub_pkt2_pb5;
    u_gen4_sub_pkt2_h->bits.gen4_sub_pkt2_pb4 = o_gen4_sub_pkt2_h.bits.gen4_sub_pkt2_pb4;

    return HI_SUCCESS;
}

int HDMI_GEN4_SUB_PKT3_L_gen4_pkt3_pbGet(U_GEN4_SUB_PKT3_L* u_gen4_sub_pkt3_l)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN4_SUB_PKT3_L o_gen4_sub_pkt3_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN4_SUB_PKT3_L.u32);
    o_gen4_sub_pkt3_l.u32 = HDMITXRegRead(pu32RegAddr);
    u_gen4_sub_pkt3_l->bits.gen4_sub_pkt3_pb3 = o_gen4_sub_pkt3_l.bits.gen4_sub_pkt3_pb3;
    u_gen4_sub_pkt3_l->bits.gen4_sub_pkt3_pb2 = o_gen4_sub_pkt3_l.bits.gen4_sub_pkt3_pb2;
    u_gen4_sub_pkt3_l->bits.gen4_sub_pkt3_pb1 = o_gen4_sub_pkt3_l.bits.gen4_sub_pkt3_pb1;
    u_gen4_sub_pkt3_l->bits.gen4_sub_pkt3_pb0 = o_gen4_sub_pkt3_l.bits.gen4_sub_pkt3_pb0;

    return HI_SUCCESS;
}

int HDMI_GEN4_SUB_PKT3_H_gen4_pkt3_pbGet(U_GEN4_SUB_PKT3_H* u_gen4_sub_pkt3_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN4_SUB_PKT3_H o_gen4_sub_pkt3_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN4_SUB_PKT3_H.u32);
    o_gen4_sub_pkt3_h.u32 = HDMITXRegRead(pu32RegAddr);
    u_gen4_sub_pkt3_h->bits.gen4_sub_pkt3_pb6 = o_gen4_sub_pkt3_h.bits.gen4_sub_pkt3_pb6;
    u_gen4_sub_pkt3_h->bits.gen4_sub_pkt3_pb5 = o_gen4_sub_pkt3_h.bits.gen4_sub_pkt3_pb5;
    u_gen4_sub_pkt3_h->bits.gen4_sub_pkt3_pb4 = o_gen4_sub_pkt3_h.bits.gen4_sub_pkt3_pb4;

    return HI_SUCCESS;
}

//*************************************************************************************//
//********************************  Gen5    信息帧设置******************************//
//*************************************************************************************//
int HDMI_GEN5_PKT_HEADER_hbSet(unsigned int uhb0,unsigned int uhb1,unsigned int uhb2)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN5_PKT_HEADER o_gen5_pkt_header;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN5_PKT_HEADER.u32);
    o_gen5_pkt_header.u32 = HDMITXRegRead(pu32RegAddr);
    o_gen5_pkt_header.bits.gen5_pkt_hb2= uhb2;
    o_gen5_pkt_header.bits.gen5_pkt_hb1= uhb1;
    o_gen5_pkt_header.bits.gen5_pkt_hb0= uhb0;
    HDMITXRegWrite(pu32RegAddr, o_gen5_pkt_header.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gen5_pkt_header.u32, "GEN5_PKT_HEADER");

    return HI_SUCCESS;
}

int HDMI_GEN5_SUB_PKT0_L_gen5_pkt0_pbSet(unsigned int ugen5_pkt0_pb0,unsigned int ugen5_pkt0_pb1,unsigned int ugen5_pkt0_pb2,unsigned int ugen5_pkt0_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN5_SUB_PKT0_L o_gen5_sub_pkt0_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN5_SUB_PKT0_L.u32);
    o_gen5_sub_pkt0_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_gen5_sub_pkt0_l.bits.gen5_sub_pkt0_pb3 = ugen5_pkt0_pb3;
    o_gen5_sub_pkt0_l.bits.gen5_sub_pkt0_pb2 = ugen5_pkt0_pb2;
    o_gen5_sub_pkt0_l.bits.gen5_sub_pkt0_pb1 = ugen5_pkt0_pb1;
    o_gen5_sub_pkt0_l.bits.gen5_sub_pkt0_pb0 = ugen5_pkt0_pb0;
    HDMITXRegWrite(pu32RegAddr, o_gen5_sub_pkt0_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gen5_sub_pkt0_l.u32, "GEN5_SUB_PKT0_L");

    return HI_SUCCESS;
}

int HDMI_GEN5_SUB_PKT0_H_gen5_pkt0_pbSet(unsigned int ugen5_pkt0_pb4,unsigned int ugen5_pkt0_pb5,unsigned int ugen5_pkt0_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN5_SUB_PKT0_H o_gen5_sub_pkt0_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN5_SUB_PKT0_H.u32);
    o_gen5_sub_pkt0_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_gen5_sub_pkt0_h.bits.gen5_sub_pkt0_pb6 = ugen5_pkt0_pb6;
    o_gen5_sub_pkt0_h.bits.gen5_sub_pkt0_pb5 = ugen5_pkt0_pb5;
    o_gen5_sub_pkt0_h.bits.gen5_sub_pkt0_pb4 = ugen5_pkt0_pb4;
    HDMITXRegWrite(pu32RegAddr, o_gen5_sub_pkt0_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gen5_sub_pkt0_h.u32, "GEN5_SUB_PKT0_H");

    return HI_SUCCESS;
}

int HDMI_GEN5_SUB_PKT1_L_gen5_pkt1_pbSet(unsigned int ugen5_pkt1_pb0,unsigned int ugen5_pkt1_pb1,unsigned int ugen5_pkt1_pb2,unsigned int ugen5_pkt1_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN5_SUB_PKT1_L o_gen5_sub_pkt1_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN5_SUB_PKT1_L.u32);
    o_gen5_sub_pkt1_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_gen5_sub_pkt1_l.bits.gen5_sub_pkt1_pb3 = ugen5_pkt1_pb3;
    o_gen5_sub_pkt1_l.bits.gen5_sub_pkt1_pb2 = ugen5_pkt1_pb2;
    o_gen5_sub_pkt1_l.bits.gen5_sub_pkt1_pb1 = ugen5_pkt1_pb1;
    o_gen5_sub_pkt1_l.bits.gen5_sub_pkt1_pb0 = ugen5_pkt1_pb0;
    HDMITXRegWrite(pu32RegAddr, o_gen5_sub_pkt1_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gen5_sub_pkt1_l.u32, "GEN5_SUB_PKT1_L");

    return HI_SUCCESS;
}

int HDMI_GEN5_SUB_PKT1_H_gen5_pkt1_pbSet(unsigned int ugen5_pkt1_pb4,unsigned int ugen5_pkt1_pb5,unsigned int ugen5_pkt1_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN5_SUB_PKT1_H o_gen5_sub_pkt1_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN5_SUB_PKT1_H.u32);
    o_gen5_sub_pkt1_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_gen5_sub_pkt1_h.bits.gen5_sub_pkt1_pb6 = ugen5_pkt1_pb6;
    o_gen5_sub_pkt1_h.bits.gen5_sub_pkt1_pb5 = ugen5_pkt1_pb5;
    o_gen5_sub_pkt1_h.bits.gen5_sub_pkt1_pb4 = ugen5_pkt1_pb4;
    HDMITXRegWrite(pu32RegAddr, o_gen5_sub_pkt1_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gen5_sub_pkt1_h.u32, "GEN5_SUB_PKT1_H");

    return HI_SUCCESS;
}

int HDMI_GEN5_SUB_PKT2_L_gen5_pkt2_pbSet(unsigned int ugen5_pkt2_pb0,unsigned int ugen5_pkt2_pb1,unsigned int ugen5_pkt2_pb2,unsigned int ugen5_pkt2_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN5_SUB_PKT2_L o_gen5_sub_pkt2_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN5_SUB_PKT2_L.u32);
    o_gen5_sub_pkt2_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_gen5_sub_pkt2_l.bits.gen5_sub_pkt2_pb3 = ugen5_pkt2_pb3;
    o_gen5_sub_pkt2_l.bits.gen5_sub_pkt2_pb2 = ugen5_pkt2_pb2;
    o_gen5_sub_pkt2_l.bits.gen5_sub_pkt2_pb1 = ugen5_pkt2_pb1;
    o_gen5_sub_pkt2_l.bits.gen5_sub_pkt2_pb0 = ugen5_pkt2_pb0;
    HDMITXRegWrite(pu32RegAddr, o_gen5_sub_pkt2_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gen5_sub_pkt2_l.u32, "GEN5_SUB_PKT2_L");

    return HI_SUCCESS;
}

int HDMI_GEN5_SUB_PKT2_H_gen5_pkt2_pbSet(unsigned int ugen5_pkt2_pb4,unsigned int ugen5_pkt2_pb5,unsigned int ugen5_pkt2_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN5_SUB_PKT2_H o_gen5_sub_pkt2_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN5_SUB_PKT2_H.u32);
    o_gen5_sub_pkt2_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_gen5_sub_pkt2_h.bits.gen5_sub_pkt2_pb6 = ugen5_pkt2_pb6;
    o_gen5_sub_pkt2_h.bits.gen5_sub_pkt2_pb5 = ugen5_pkt2_pb5;
    o_gen5_sub_pkt2_h.bits.gen5_sub_pkt2_pb4 = ugen5_pkt2_pb4;
    HDMITXRegWrite(pu32RegAddr, o_gen5_sub_pkt2_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gen5_sub_pkt2_h.u32, "GEN5_SUB_PKT2_H");

    return HI_SUCCESS;
}

int HDMI_GEN5_SUB_PKT3_L_gen5_pkt3_pbSet(unsigned int ugen5_pkt3_pb0,unsigned int ugen5_pkt3_pb1,unsigned int ugen5_pkt3_pb2,unsigned int ugen5_pkt3_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN5_SUB_PKT3_L o_gen5_sub_pkt3_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN5_SUB_PKT3_L.u32);
    o_gen5_sub_pkt3_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_gen5_sub_pkt3_l.bits.gen5_sub_pkt3_pb3 = ugen5_pkt3_pb3;
    o_gen5_sub_pkt3_l.bits.gen5_sub_pkt3_pb2 = ugen5_pkt3_pb2;
    o_gen5_sub_pkt3_l.bits.gen5_sub_pkt3_pb1 = ugen5_pkt3_pb1;
    o_gen5_sub_pkt3_l.bits.gen5_sub_pkt3_pb0 = ugen5_pkt3_pb0;
    HDMITXRegWrite(pu32RegAddr, o_gen5_sub_pkt3_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gen5_sub_pkt3_l.u32, "GEN5_SUB_PKT3_L");

    return HI_SUCCESS;
}

int HDMI_GEN5_SUB_PKT3_H_gen5_pkt3_pbSet(unsigned int ugen5_pkt3_pb4,unsigned int ugen5_pkt3_pb5,unsigned int ugen5_pkt3_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN5_SUB_PKT3_H o_gen5_sub_pkt3_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN5_SUB_PKT3_H.u32);
    o_gen5_sub_pkt3_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_gen5_sub_pkt3_h.bits.gen5_sub_pkt3_pb6 = ugen5_pkt3_pb6;
    o_gen5_sub_pkt3_h.bits.gen5_sub_pkt3_pb5 = ugen5_pkt3_pb5;
    o_gen5_sub_pkt3_h.bits.gen5_sub_pkt3_pb4 = ugen5_pkt3_pb4;
    HDMITXRegWrite(pu32RegAddr, o_gen5_sub_pkt3_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gen5_sub_pkt3_h.u32, "GEN5_SUB_PKT3_H");

    return HI_SUCCESS;
}


//*************************************************************************************//
//********************************  Gen5    信息帧获取******************************//
//*************************************************************************************//
int HDMI_GEN5_PKT_HEADER_hbGet(U_GEN5_PKT_HEADER* u_gen5_pkt_header)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN5_PKT_HEADER o_gen5_pkt_header;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN5_PKT_HEADER.u32);
    o_gen5_pkt_header.u32 = HDMITXRegRead(pu32RegAddr);
    u_gen5_pkt_header->bits.gen5_pkt_hb2 = o_gen5_pkt_header.bits.gen5_pkt_hb2;
    u_gen5_pkt_header->bits.gen5_pkt_hb1 = o_gen5_pkt_header.bits.gen5_pkt_hb1;
    u_gen5_pkt_header->bits.gen5_pkt_hb0 = o_gen5_pkt_header.bits.gen5_pkt_hb0;

    return HI_SUCCESS;
}

int HDMI_GEN5_SUB_PKT0_L_gen5_pkt0_pbGet(U_GEN5_SUB_PKT0_L* u_gen5_sub_pkt0_l)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN5_SUB_PKT0_L o_gen5_sub_pkt0_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN5_SUB_PKT0_L.u32);
    o_gen5_sub_pkt0_l.u32 = HDMITXRegRead(pu32RegAddr);
    u_gen5_sub_pkt0_l->bits.gen5_sub_pkt0_pb3 = o_gen5_sub_pkt0_l.bits.gen5_sub_pkt0_pb3;
    u_gen5_sub_pkt0_l->bits.gen5_sub_pkt0_pb2 = o_gen5_sub_pkt0_l.bits.gen5_sub_pkt0_pb2;
    u_gen5_sub_pkt0_l->bits.gen5_sub_pkt0_pb1 = o_gen5_sub_pkt0_l.bits.gen5_sub_pkt0_pb1;
    u_gen5_sub_pkt0_l->bits.gen5_sub_pkt0_pb0 = o_gen5_sub_pkt0_l.bits.gen5_sub_pkt0_pb0;

    return HI_SUCCESS;
}

int HDMI_GEN5_SUB_PKT0_H_gen5_pkt0_pbGet(U_GEN5_SUB_PKT0_H* u_gen5_sub_pkt0_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN5_SUB_PKT0_H o_gen5_sub_pkt0_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN5_SUB_PKT0_H.u32);
    o_gen5_sub_pkt0_h.u32 = HDMITXRegRead(pu32RegAddr);
    u_gen5_sub_pkt0_h->bits.gen5_sub_pkt0_pb6 = o_gen5_sub_pkt0_h.bits.gen5_sub_pkt0_pb6;
    u_gen5_sub_pkt0_h->bits.gen5_sub_pkt0_pb5 = o_gen5_sub_pkt0_h.bits.gen5_sub_pkt0_pb5;
    u_gen5_sub_pkt0_h->bits.gen5_sub_pkt0_pb4 = o_gen5_sub_pkt0_h.bits.gen5_sub_pkt0_pb4;

    return HI_SUCCESS;
}

int HDMI_GEN5_SUB_PKT1_L_gen5_pkt1_pbGet(U_GEN5_SUB_PKT1_L* u_gen5_sub_pkt1_l)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN5_SUB_PKT1_L o_gen5_sub_pkt1_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN5_SUB_PKT1_L.u32);
    o_gen5_sub_pkt1_l.u32 = HDMITXRegRead(pu32RegAddr);
    u_gen5_sub_pkt1_l->bits.gen5_sub_pkt1_pb3 = o_gen5_sub_pkt1_l.bits.gen5_sub_pkt1_pb3;
    u_gen5_sub_pkt1_l->bits.gen5_sub_pkt1_pb2 = o_gen5_sub_pkt1_l.bits.gen5_sub_pkt1_pb2;
    u_gen5_sub_pkt1_l->bits.gen5_sub_pkt1_pb1 = o_gen5_sub_pkt1_l.bits.gen5_sub_pkt1_pb1;
    u_gen5_sub_pkt1_l->bits.gen5_sub_pkt1_pb0 = o_gen5_sub_pkt1_l.bits.gen5_sub_pkt1_pb0;

    return HI_SUCCESS;
}

int HDMI_GEN5_SUB_PKT1_H_gen5_pkt1_pbGet(U_GEN5_SUB_PKT1_H* u_gen5_sub_pkt1_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN5_SUB_PKT1_H o_gen5_sub_pkt1_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN5_SUB_PKT1_H.u32);
    o_gen5_sub_pkt1_h.u32 = HDMITXRegRead(pu32RegAddr);
    u_gen5_sub_pkt1_h->bits.gen5_sub_pkt1_pb6 = o_gen5_sub_pkt1_h.bits.gen5_sub_pkt1_pb6;
    u_gen5_sub_pkt1_h->bits.gen5_sub_pkt1_pb5 = o_gen5_sub_pkt1_h.bits.gen5_sub_pkt1_pb5;
    u_gen5_sub_pkt1_h->bits.gen5_sub_pkt1_pb4 = o_gen5_sub_pkt1_h.bits.gen5_sub_pkt1_pb4;

    return HI_SUCCESS;
}

int HDMI_GEN5_SUB_PKT2_L_gen5_pkt2_pbGet(U_GEN5_SUB_PKT2_L* u_gen5_sub_pkt2_l)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN5_SUB_PKT2_L o_gen5_sub_pkt2_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN5_SUB_PKT2_L.u32);
    o_gen5_sub_pkt2_l.u32 = HDMITXRegRead(pu32RegAddr);
    u_gen5_sub_pkt2_l->bits.gen5_sub_pkt2_pb3 = o_gen5_sub_pkt2_l.bits.gen5_sub_pkt2_pb3;
    u_gen5_sub_pkt2_l->bits.gen5_sub_pkt2_pb2 = o_gen5_sub_pkt2_l.bits.gen5_sub_pkt2_pb2;
    u_gen5_sub_pkt2_l->bits.gen5_sub_pkt2_pb1 = o_gen5_sub_pkt2_l.bits.gen5_sub_pkt2_pb1;
    u_gen5_sub_pkt2_l->bits.gen5_sub_pkt2_pb0 = o_gen5_sub_pkt2_l.bits.gen5_sub_pkt2_pb0;

    return HI_SUCCESS;
}

int HDMI_GEN5_SUB_PKT2_H_gen5_pkt2_pbGet(U_GEN5_SUB_PKT2_H* u_gen5_sub_pkt2_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN5_SUB_PKT2_H o_gen5_sub_pkt2_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN5_SUB_PKT2_H.u32);
    o_gen5_sub_pkt2_h.u32 = HDMITXRegRead(pu32RegAddr);
    u_gen5_sub_pkt2_h->bits.gen5_sub_pkt2_pb6 = o_gen5_sub_pkt2_h.bits.gen5_sub_pkt2_pb6;
    u_gen5_sub_pkt2_h->bits.gen5_sub_pkt2_pb5 = o_gen5_sub_pkt2_h.bits.gen5_sub_pkt2_pb5;
    u_gen5_sub_pkt2_h->bits.gen5_sub_pkt2_pb4 = o_gen5_sub_pkt2_h.bits.gen5_sub_pkt2_pb4;

    return HI_SUCCESS;
}

int HDMI_GEN5_SUB_PKT3_L_gen5_pkt3_pbGet(U_GEN5_SUB_PKT3_L* u_gen5_sub_pkt3_l)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN5_SUB_PKT3_L o_gen5_sub_pkt3_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN5_SUB_PKT3_L.u32);
    o_gen5_sub_pkt3_l.u32 = HDMITXRegRead(pu32RegAddr);
    u_gen5_sub_pkt3_l->bits.gen5_sub_pkt3_pb3 = o_gen5_sub_pkt3_l.bits.gen5_sub_pkt3_pb3;
    u_gen5_sub_pkt3_l->bits.gen5_sub_pkt3_pb2 = o_gen5_sub_pkt3_l.bits.gen5_sub_pkt3_pb2;
    u_gen5_sub_pkt3_l->bits.gen5_sub_pkt3_pb1 = o_gen5_sub_pkt3_l.bits.gen5_sub_pkt3_pb1;
    u_gen5_sub_pkt3_l->bits.gen5_sub_pkt3_pb0 = o_gen5_sub_pkt3_l.bits.gen5_sub_pkt3_pb0;

    return HI_SUCCESS;
}

int HDMI_GEN5_SUB_PKT3_H_gen5_pkt3_pbGet(U_GEN5_SUB_PKT3_H* u_gen5_sub_pkt3_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GEN5_SUB_PKT3_H o_gen5_sub_pkt3_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GEN5_SUB_PKT3_H.u32);
    o_gen5_sub_pkt3_h.u32 = HDMITXRegRead(pu32RegAddr);
    u_gen5_sub_pkt3_h->bits.gen5_sub_pkt3_pb6 = o_gen5_sub_pkt3_h.bits.gen5_sub_pkt3_pb6;
    u_gen5_sub_pkt3_h->bits.gen5_sub_pkt3_pb5 = o_gen5_sub_pkt3_h.bits.gen5_sub_pkt3_pb5;
    u_gen5_sub_pkt3_h->bits.gen5_sub_pkt3_pb4 = o_gen5_sub_pkt3_h.bits.gen5_sub_pkt3_pb4;

    return HI_SUCCESS;
}

//*************************************************************************************//
//********************************  Gamut    信息帧设置******************************//
//*************************************************************************************//
int HDMI_GAMUT_PKT_HEADER_hbSet(unsigned int uhb0,unsigned int uhb1,unsigned int uhb2)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GAMUT_PKT_HEADER o_gamut_pkt_header;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GAMUT_PKT_HEADER.u32);
    o_gamut_pkt_header.u32 = HDMITXRegRead(pu32RegAddr);
    o_gamut_pkt_header.bits.gamut_pkt_hb2= uhb2;
    o_gamut_pkt_header.bits.gamut_pkt_hb1= uhb1;
    o_gamut_pkt_header.bits.gamut_pkt_hb0= uhb0;
    HDMITXRegWrite(pu32RegAddr, o_gamut_pkt_header.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gamut_pkt_header.u32, "GAMUT_PKT_HEADER");

    return HI_SUCCESS;
}

int HDMI_GAMUT_SUB_PKT0_L_gamut_pkt0_pbSet(unsigned int ugamut_pkt0_pb0,unsigned int ugamut_pkt0_pb1,unsigned int ugamut_pkt0_pb2,unsigned int ugamut_pkt0_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GAMUT_SUB_PKT0_L o_gamut_sub_pkt0_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GAMUT_SUB_PKT0_L.u32);
    o_gamut_sub_pkt0_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_gamut_sub_pkt0_l.bits.gamut_sub_pkt0_pb3 = ugamut_pkt0_pb3;
    o_gamut_sub_pkt0_l.bits.gamut_sub_pkt0_pb2 = ugamut_pkt0_pb2;
    o_gamut_sub_pkt0_l.bits.gamut_sub_pkt0_pb1 = ugamut_pkt0_pb1;
    o_gamut_sub_pkt0_l.bits.gamut_sub_pkt0_pb0 = ugamut_pkt0_pb0;
    HDMITXRegWrite(pu32RegAddr, o_gamut_sub_pkt0_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gamut_sub_pkt0_l.u32, "GAMUT_SUB_PKT0_L");

    return HI_SUCCESS;
}

int HDMI_GAMUT_SUB_PKT0_H_gamut_pkt0_pbSet(unsigned int ugamut_pkt0_pb4,unsigned int ugamut_pkt0_pb5,unsigned int ugamut_pkt0_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GAMUT_SUB_PKT0_H o_gamut_sub_pkt0_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GAMUT_SUB_PKT0_H.u32);
    o_gamut_sub_pkt0_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_gamut_sub_pkt0_h.bits.gamut_sub_pkt0_pb6 = ugamut_pkt0_pb6;
    o_gamut_sub_pkt0_h.bits.gamut_sub_pkt0_pb5 = ugamut_pkt0_pb5;
    o_gamut_sub_pkt0_h.bits.gamut_sub_pkt0_pb4 = ugamut_pkt0_pb4;
    HDMITXRegWrite(pu32RegAddr, o_gamut_sub_pkt0_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gamut_sub_pkt0_h.u32, "GAMUT_SUB_PKT0_H");

    return HI_SUCCESS;
}

int HDMI_GAMUT_SUB_PKT1_L_gamut_pkt1_pbSet(unsigned int ugamut_pkt1_pb0,unsigned int ugamut_pkt1_pb1,unsigned int ugamut_pkt1_pb2,unsigned int ugamut_pkt1_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GAMUT_SUB_PKT1_L o_gamut_sub_pkt1_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GAMUT_SUB_PKT1_L.u32);
    o_gamut_sub_pkt1_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_gamut_sub_pkt1_l.bits.gamut_sub_pkt1_pb3 = ugamut_pkt1_pb3;
    o_gamut_sub_pkt1_l.bits.gamut_sub_pkt1_pb2 = ugamut_pkt1_pb2;
    o_gamut_sub_pkt1_l.bits.gamut_sub_pkt1_pb1 = ugamut_pkt1_pb1;
    o_gamut_sub_pkt1_l.bits.gamut_sub_pkt1_pb0 = ugamut_pkt1_pb0;
    HDMITXRegWrite(pu32RegAddr, o_gamut_sub_pkt1_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gamut_sub_pkt1_l.u32, "GAMUT_SUB_PKT1_L");

    return HI_SUCCESS;
}

int HDMI_GAMUT_SUB_PKT1_H_gamut_pkt1_pbSet(unsigned int ugamut_pkt1_pb4,unsigned int ugamut_pkt1_pb5,unsigned int ugamut_pkt1_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GAMUT_SUB_PKT1_H o_gamut_sub_pkt1_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GAMUT_SUB_PKT1_H.u32);
    o_gamut_sub_pkt1_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_gamut_sub_pkt1_h.bits.gamut_sub_pkt1_pb6 = ugamut_pkt1_pb6;
    o_gamut_sub_pkt1_h.bits.gamut_sub_pkt1_pb5 = ugamut_pkt1_pb5;
    o_gamut_sub_pkt1_h.bits.gamut_sub_pkt1_pb4 = ugamut_pkt1_pb4;
    HDMITXRegWrite(pu32RegAddr, o_gamut_sub_pkt1_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gamut_sub_pkt1_h.u32, "GAMUT_SUB_PKT1_H");

    return HI_SUCCESS;
}

int HDMI_GAMUT_SUB_PKT2_L_gamut_pkt2_pbSet(unsigned int ugamut_pkt2_pb0,unsigned int ugamut_pkt2_pb1,unsigned int ugamut_pkt2_pb2,unsigned int ugamut_pkt2_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GAMUT_SUB_PKT2_L o_gamut_sub_pkt2_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GAMUT_SUB_PKT2_L.u32);
    o_gamut_sub_pkt2_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_gamut_sub_pkt2_l.bits.gamut_sub_pkt2_pb3 = ugamut_pkt2_pb3;
    o_gamut_sub_pkt2_l.bits.gamut_sub_pkt2_pb2 = ugamut_pkt2_pb2;
    o_gamut_sub_pkt2_l.bits.gamut_sub_pkt2_pb1 = ugamut_pkt2_pb1;
    o_gamut_sub_pkt2_l.bits.gamut_sub_pkt2_pb0 = ugamut_pkt2_pb0;
    HDMITXRegWrite(pu32RegAddr, o_gamut_sub_pkt2_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gamut_sub_pkt2_l.u32, "GAMUT_SUB_PKT2_L");

    return HI_SUCCESS;
}

int HDMI_GAMUT_SUB_PKT2_H_gamut_pkt2_pbSet(unsigned int ugamut_pkt2_pb4,unsigned int ugamut_pkt2_pb5,unsigned int ugamut_pkt2_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GAMUT_SUB_PKT2_H o_gamut_sub_pkt2_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GAMUT_SUB_PKT2_H.u32);
    o_gamut_sub_pkt2_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_gamut_sub_pkt2_h.bits.gamut_sub_pkt2_pb6 = ugamut_pkt2_pb6;
    o_gamut_sub_pkt2_h.bits.gamut_sub_pkt2_pb5 = ugamut_pkt2_pb5;
    o_gamut_sub_pkt2_h.bits.gamut_sub_pkt2_pb4 = ugamut_pkt2_pb4;
    HDMITXRegWrite(pu32RegAddr, o_gamut_sub_pkt2_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gamut_sub_pkt2_h.u32, "GAMUT_SUB_PKT2_H");

    return HI_SUCCESS;
}

int HDMI_GAMUT_SUB_PKT3_L_gamut_pkt3_pbSet(unsigned int ugamut_pkt3_pb0,unsigned int ugamut_pkt3_pb1,unsigned int ugamut_pkt3_pb2,unsigned int ugamut_pkt3_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GAMUT_SUB_PKT3_L o_gamut_sub_pkt3_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GAMUT_SUB_PKT3_L.u32);
    o_gamut_sub_pkt3_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_gamut_sub_pkt3_l.bits.gamut_sub_pkt3_pb3 = ugamut_pkt3_pb3;
    o_gamut_sub_pkt3_l.bits.gamut_sub_pkt3_pb2 = ugamut_pkt3_pb2;
    o_gamut_sub_pkt3_l.bits.gamut_sub_pkt3_pb1 = ugamut_pkt3_pb1;
    o_gamut_sub_pkt3_l.bits.gamut_sub_pkt3_pb0 = ugamut_pkt3_pb0;
    HDMITXRegWrite(pu32RegAddr, o_gamut_sub_pkt3_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gamut_sub_pkt3_l.u32, "GAMUT_SUB_PKT3_L");

    return HI_SUCCESS;
}

int HDMI_GAMUT_SUB_PKT3_H_gamut_pkt3_pbSet(unsigned int ugamut_pkt3_pb4,unsigned int ugamut_pkt3_pb5,unsigned int ugamut_pkt3_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GAMUT_SUB_PKT3_H o_gamut_sub_pkt3_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GAMUT_SUB_PKT3_H.u32);
    o_gamut_sub_pkt3_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_gamut_sub_pkt3_h.bits.gamut_sub_pkt3_pb6 = ugamut_pkt3_pb6;
    o_gamut_sub_pkt3_h.bits.gamut_sub_pkt3_pb5 = ugamut_pkt3_pb5;
    o_gamut_sub_pkt3_h.bits.gamut_sub_pkt3_pb4 = ugamut_pkt3_pb4;
    HDMITXRegWrite(pu32RegAddr, o_gamut_sub_pkt3_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_gamut_sub_pkt3_h.u32, "GAMUT_SUB_PKT3_H");

    return HI_SUCCESS;
}


//*************************************************************************************//
//********************************  Gamut    信息帧获取******************************//
//*************************************************************************************//
int HDMI_GAMUT_PKT_HEADER_hbGet(U_GAMUT_PKT_HEADER* u_gamut_pkt_header)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GAMUT_PKT_HEADER o_gamut_pkt_header;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GAMUT_PKT_HEADER.u32);
    o_gamut_pkt_header.u32 = HDMITXRegRead(pu32RegAddr);
    u_gamut_pkt_header->bits.gamut_pkt_hb2 = o_gamut_pkt_header.bits.gamut_pkt_hb2;
    u_gamut_pkt_header->bits.gamut_pkt_hb1 = o_gamut_pkt_header.bits.gamut_pkt_hb1;
    u_gamut_pkt_header->bits.gamut_pkt_hb0 = o_gamut_pkt_header.bits.gamut_pkt_hb0;

    return HI_SUCCESS;
}

int HDMI_GAMUT_SUB_PKT0_L_gamut_pkt0_pbGet(U_GAMUT_SUB_PKT0_L* u_gamut_sub_pkt0_l)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GAMUT_SUB_PKT0_L o_gamut_sub_pkt0_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GAMUT_SUB_PKT0_L.u32);
    o_gamut_sub_pkt0_l.u32 = HDMITXRegRead(pu32RegAddr);
    u_gamut_sub_pkt0_l->bits.gamut_sub_pkt0_pb3 = o_gamut_sub_pkt0_l.bits.gamut_sub_pkt0_pb3;
    u_gamut_sub_pkt0_l->bits.gamut_sub_pkt0_pb2 = o_gamut_sub_pkt0_l.bits.gamut_sub_pkt0_pb2;
    u_gamut_sub_pkt0_l->bits.gamut_sub_pkt0_pb1 = o_gamut_sub_pkt0_l.bits.gamut_sub_pkt0_pb1;
    u_gamut_sub_pkt0_l->bits.gamut_sub_pkt0_pb0 = o_gamut_sub_pkt0_l.bits.gamut_sub_pkt0_pb0;

    return HI_SUCCESS;
}

int HDMI_GAMUT_SUB_PKT0_H_gamut_pkt0_pbGet(U_GAMUT_SUB_PKT0_H* u_gamut_sub_pkt0_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GAMUT_SUB_PKT0_H o_gamut_sub_pkt0_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GAMUT_SUB_PKT0_H.u32);
    o_gamut_sub_pkt0_h.u32 = HDMITXRegRead(pu32RegAddr);
    u_gamut_sub_pkt0_h->bits.gamut_sub_pkt0_pb6 = o_gamut_sub_pkt0_h.bits.gamut_sub_pkt0_pb6;
    u_gamut_sub_pkt0_h->bits.gamut_sub_pkt0_pb5 = o_gamut_sub_pkt0_h.bits.gamut_sub_pkt0_pb5;
    u_gamut_sub_pkt0_h->bits.gamut_sub_pkt0_pb4 = o_gamut_sub_pkt0_h.bits.gamut_sub_pkt0_pb4;

    return HI_SUCCESS;
}

int HDMI_GAMUT_SUB_PKT1_L_gamut_pkt1_pbGet(U_GAMUT_SUB_PKT1_L* u_gamut_sub_pkt1_l)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GAMUT_SUB_PKT1_L o_gamut_sub_pkt1_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GAMUT_SUB_PKT1_L.u32);
    o_gamut_sub_pkt1_l.u32 = HDMITXRegRead(pu32RegAddr);
    u_gamut_sub_pkt1_l->bits.gamut_sub_pkt1_pb3 = o_gamut_sub_pkt1_l.bits.gamut_sub_pkt1_pb3;
    u_gamut_sub_pkt1_l->bits.gamut_sub_pkt1_pb2 = o_gamut_sub_pkt1_l.bits.gamut_sub_pkt1_pb2;
    u_gamut_sub_pkt1_l->bits.gamut_sub_pkt1_pb1 = o_gamut_sub_pkt1_l.bits.gamut_sub_pkt1_pb1;
    u_gamut_sub_pkt1_l->bits.gamut_sub_pkt1_pb0 = o_gamut_sub_pkt1_l.bits.gamut_sub_pkt1_pb0;

    return HI_SUCCESS;
}

int HDMI_GAMUT_SUB_PKT1_H_gamut_pkt1_pbGet(U_GAMUT_SUB_PKT1_H* u_gamut_sub_pkt1_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GAMUT_SUB_PKT1_H o_gamut_sub_pkt1_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GAMUT_SUB_PKT1_H.u32);
    o_gamut_sub_pkt1_h.u32 = HDMITXRegRead(pu32RegAddr);
    u_gamut_sub_pkt1_h->bits.gamut_sub_pkt1_pb6 = o_gamut_sub_pkt1_h.bits.gamut_sub_pkt1_pb6;
    u_gamut_sub_pkt1_h->bits.gamut_sub_pkt1_pb5 = o_gamut_sub_pkt1_h.bits.gamut_sub_pkt1_pb5;
    u_gamut_sub_pkt1_h->bits.gamut_sub_pkt1_pb4 = o_gamut_sub_pkt1_h.bits.gamut_sub_pkt1_pb4;

    return HI_SUCCESS;
}

int HDMI_GAMUT_SUB_PKT2_L_gamut_pkt2_pbGet(U_GAMUT_SUB_PKT2_L* u_gamut_sub_pkt2_l)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GAMUT_SUB_PKT2_L o_gamut_sub_pkt2_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GAMUT_SUB_PKT2_L.u32);
    o_gamut_sub_pkt2_l.u32 = HDMITXRegRead(pu32RegAddr);
    u_gamut_sub_pkt2_l->bits.gamut_sub_pkt2_pb3 = o_gamut_sub_pkt2_l.bits.gamut_sub_pkt2_pb3;
    u_gamut_sub_pkt2_l->bits.gamut_sub_pkt2_pb2 = o_gamut_sub_pkt2_l.bits.gamut_sub_pkt2_pb2;
    u_gamut_sub_pkt2_l->bits.gamut_sub_pkt2_pb1 = o_gamut_sub_pkt2_l.bits.gamut_sub_pkt2_pb1;
    u_gamut_sub_pkt2_l->bits.gamut_sub_pkt2_pb0 = o_gamut_sub_pkt2_l.bits.gamut_sub_pkt2_pb0;

    return HI_SUCCESS;
}

int HDMI_GAMUT_SUB_PKT2_H_gamut_pkt2_pbGet(U_GAMUT_SUB_PKT2_H* u_gamut_sub_pkt2_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GAMUT_SUB_PKT2_H o_gamut_sub_pkt2_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GAMUT_SUB_PKT2_H.u32);
    o_gamut_sub_pkt2_h.u32 = HDMITXRegRead(pu32RegAddr);
    u_gamut_sub_pkt2_h->bits.gamut_sub_pkt2_pb6 = o_gamut_sub_pkt2_h.bits.gamut_sub_pkt2_pb6;
    u_gamut_sub_pkt2_h->bits.gamut_sub_pkt2_pb5 = o_gamut_sub_pkt2_h.bits.gamut_sub_pkt2_pb5;
    u_gamut_sub_pkt2_h->bits.gamut_sub_pkt2_pb4 = o_gamut_sub_pkt2_h.bits.gamut_sub_pkt2_pb4;

    return HI_SUCCESS;
}

int HDMI_GAMUT_SUB_PKT3_L_gamut_pkt3_pbGet(U_GAMUT_SUB_PKT3_L* u_gamut_sub_pkt3_l)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GAMUT_SUB_PKT3_L o_gamut_sub_pkt3_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GAMUT_SUB_PKT3_L.u32);
    o_gamut_sub_pkt3_l.u32 = HDMITXRegRead(pu32RegAddr);
    u_gamut_sub_pkt3_l->bits.gamut_sub_pkt3_pb3 = o_gamut_sub_pkt3_l.bits.gamut_sub_pkt3_pb3;
    u_gamut_sub_pkt3_l->bits.gamut_sub_pkt3_pb2 = o_gamut_sub_pkt3_l.bits.gamut_sub_pkt3_pb2;
    u_gamut_sub_pkt3_l->bits.gamut_sub_pkt3_pb1 = o_gamut_sub_pkt3_l.bits.gamut_sub_pkt3_pb1;
    u_gamut_sub_pkt3_l->bits.gamut_sub_pkt3_pb0 = o_gamut_sub_pkt3_l.bits.gamut_sub_pkt3_pb0;

    return HI_SUCCESS;
}

int HDMI_GAMUT_SUB_PKT3_H_gamut_pkt3_pbGet(U_GAMUT_SUB_PKT3_H* u_gamut_sub_pkt3_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_GAMUT_SUB_PKT3_H o_gamut_sub_pkt3_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->GAMUT_SUB_PKT3_H.u32);
    o_gamut_sub_pkt3_h.u32 = HDMITXRegRead(pu32RegAddr);
    u_gamut_sub_pkt3_h->bits.gamut_sub_pkt3_pb6 = o_gamut_sub_pkt3_h.bits.gamut_sub_pkt3_pb6;
    u_gamut_sub_pkt3_h->bits.gamut_sub_pkt3_pb5 = o_gamut_sub_pkt3_h.bits.gamut_sub_pkt3_pb5;
    u_gamut_sub_pkt3_h->bits.gamut_sub_pkt3_pb4 = o_gamut_sub_pkt3_h.bits.gamut_sub_pkt3_pb4;

    return HI_SUCCESS;
}

//*************************************************************************************//
//********************************  Vsif    信息帧设置******************************//
//*************************************************************************************//
int HDMI_VSIF_PKT_HEADER_hbSet(unsigned int uhb0,unsigned int uhb1,unsigned int uhb2)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VSIF_PKT_HEADER o_vsif_pkt_header;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->VSIF_PKT_HEADER.u32);
    o_vsif_pkt_header.u32 = HDMITXRegRead(pu32RegAddr);
    o_vsif_pkt_header.bits.vsif_pkt_hb2= uhb2;
    o_vsif_pkt_header.bits.vsif_pkt_hb1= uhb1;
    o_vsif_pkt_header.bits.vsif_pkt_hb0= uhb0;
    HDMITXRegWrite(pu32RegAddr, o_vsif_pkt_header.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_vsif_pkt_header.u32, "VSIF_PKT_HEADER");

    return HI_SUCCESS;
}

int HDMI_VSIF_SUB_PKT0_L_vsif_pkt0_pbSet(unsigned int uvsif_pkt0_pb0,unsigned int uvsif_pkt0_pb1,unsigned int uvsif_pkt0_pb2,unsigned int uvsif_pkt0_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VSIF_SUB_PKT0_L o_vsif_sub_pkt0_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->VSIF_SUB_PKT0_L.u32);
    o_vsif_sub_pkt0_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_vsif_sub_pkt0_l.bits.vsif_sub_pkt0_pb3 = uvsif_pkt0_pb3;
    o_vsif_sub_pkt0_l.bits.vsif_sub_pkt0_pb2 = uvsif_pkt0_pb2;
    o_vsif_sub_pkt0_l.bits.vsif_sub_pkt0_pb1 = uvsif_pkt0_pb1;
    o_vsif_sub_pkt0_l.bits.vsif_sub_pkt0_pb0 = uvsif_pkt0_pb0;
    HDMITXRegWrite(pu32RegAddr, o_vsif_sub_pkt0_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_vsif_sub_pkt0_l.u32, "VSIF_SUB_PKT0_L");

    return HI_SUCCESS;
}

int HDMI_VSIF_SUB_PKT0_H_vsif_pkt0_pbSet(unsigned int uvsif_pkt0_pb4,unsigned int uvsif_pkt0_pb5,unsigned int uvsif_pkt0_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VSIF_SUB_PKT0_H o_vsif_sub_pkt0_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->VSIF_SUB_PKT0_H.u32);
    o_vsif_sub_pkt0_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_vsif_sub_pkt0_h.bits.vsif_sub_pkt0_pb6 = uvsif_pkt0_pb6;
    o_vsif_sub_pkt0_h.bits.vsif_sub_pkt0_pb5 = uvsif_pkt0_pb5;
    o_vsif_sub_pkt0_h.bits.vsif_sub_pkt0_pb4 = uvsif_pkt0_pb4;
    HDMITXRegWrite(pu32RegAddr, o_vsif_sub_pkt0_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_vsif_sub_pkt0_h.u32, "VSIF_SUB_PKT0_H");

    return HI_SUCCESS;
}

int HDMI_VSIF_SUB_PKT1_L_vsif_pkt1_pbSet(unsigned int uvsif_pkt1_pb0,unsigned int uvsif_pkt1_pb1,unsigned int uvsif_pkt1_pb2,unsigned int uvsif_pkt1_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VSIF_SUB_PKT1_L o_vsif_sub_pkt1_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->VSIF_SUB_PKT1_L.u32);
    o_vsif_sub_pkt1_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_vsif_sub_pkt1_l.bits.vsif_sub_pkt1_pb3 = uvsif_pkt1_pb3;
    o_vsif_sub_pkt1_l.bits.vsif_sub_pkt1_pb2 = uvsif_pkt1_pb2;
    o_vsif_sub_pkt1_l.bits.vsif_sub_pkt1_pb1 = uvsif_pkt1_pb1;
    o_vsif_sub_pkt1_l.bits.vsif_sub_pkt1_pb0 = uvsif_pkt1_pb0;
    HDMITXRegWrite(pu32RegAddr, o_vsif_sub_pkt1_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_vsif_sub_pkt1_l.u32, "VSIF_SUB_PKT1_L");

    return HI_SUCCESS;
}

int HDMI_VSIF_SUB_PKT1_H_vsif_pkt1_pbSet(unsigned int uvsif_pkt1_pb4,unsigned int uvsif_pkt1_pb5,unsigned int uvsif_pkt1_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VSIF_SUB_PKT1_H o_vsif_sub_pkt1_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->VSIF_SUB_PKT1_H.u32);
    o_vsif_sub_pkt1_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_vsif_sub_pkt1_h.bits.vsif_sub_pkt1_pb6 = uvsif_pkt1_pb6;
    o_vsif_sub_pkt1_h.bits.vsif_sub_pkt1_pb5 = uvsif_pkt1_pb5;
    o_vsif_sub_pkt1_h.bits.vsif_sub_pkt1_pb4 = uvsif_pkt1_pb4;
    HDMITXRegWrite(pu32RegAddr, o_vsif_sub_pkt1_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_vsif_sub_pkt1_h.u32, "VSIF_SUB_PKT1_H");

    return HI_SUCCESS;
}

int HDMI_VSIF_SUB_PKT2_L_vsif_pkt2_pbSet(unsigned int uvsif_pkt2_pb0,unsigned int uvsif_pkt2_pb1,unsigned int uvsif_pkt2_pb2,unsigned int uvsif_pkt2_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VSIF_SUB_PKT2_L o_vsif_sub_pkt2_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->VSIF_SUB_PKT2_L.u32);
    o_vsif_sub_pkt2_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_vsif_sub_pkt2_l.bits.vsif_sub_pkt2_pb3 = uvsif_pkt2_pb3;
    o_vsif_sub_pkt2_l.bits.vsif_sub_pkt2_pb2 = uvsif_pkt2_pb2;
    o_vsif_sub_pkt2_l.bits.vsif_sub_pkt2_pb1 = uvsif_pkt2_pb1;
    o_vsif_sub_pkt2_l.bits.vsif_sub_pkt2_pb0 = uvsif_pkt2_pb0;
    HDMITXRegWrite(pu32RegAddr, o_vsif_sub_pkt2_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_vsif_sub_pkt2_l.u32, "VSIF_SUB_PKT2_L");

    return HI_SUCCESS;
}

int HDMI_VSIF_SUB_PKT2_H_vsif_pkt2_pbSet(unsigned int uvsif_pkt2_pb4,unsigned int uvsif_pkt2_pb5,unsigned int uvsif_pkt2_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VSIF_SUB_PKT2_H o_vsif_sub_pkt2_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->VSIF_SUB_PKT2_H.u32);
    o_vsif_sub_pkt2_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_vsif_sub_pkt2_h.bits.vsif_sub_pkt2_pb6 = uvsif_pkt2_pb6;
    o_vsif_sub_pkt2_h.bits.vsif_sub_pkt2_pb5 = uvsif_pkt2_pb5;
    o_vsif_sub_pkt2_h.bits.vsif_sub_pkt2_pb4 = uvsif_pkt2_pb4;
    HDMITXRegWrite(pu32RegAddr, o_vsif_sub_pkt2_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_vsif_sub_pkt2_h.u32, "VSIF_SUB_PKT2_H");

    return HI_SUCCESS;
}

int HDMI_VSIF_SUB_PKT3_L_vsif_pkt3_pbSet(unsigned int uvsif_pkt3_pb0,unsigned int uvsif_pkt3_pb1,unsigned int uvsif_pkt3_pb2,unsigned int uvsif_pkt3_pb3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VSIF_SUB_PKT3_L o_vsif_sub_pkt3_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->VSIF_SUB_PKT3_L.u32);
    o_vsif_sub_pkt3_l.u32 = HDMITXRegRead(pu32RegAddr);
    o_vsif_sub_pkt3_l.bits.vsif_sub_pkt3_pb3 = uvsif_pkt3_pb3;
    o_vsif_sub_pkt3_l.bits.vsif_sub_pkt3_pb2 = uvsif_pkt3_pb2;
    o_vsif_sub_pkt3_l.bits.vsif_sub_pkt3_pb1 = uvsif_pkt3_pb1;
    o_vsif_sub_pkt3_l.bits.vsif_sub_pkt3_pb0 = uvsif_pkt3_pb0;
    HDMITXRegWrite(pu32RegAddr, o_vsif_sub_pkt3_l.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_vsif_sub_pkt3_l.u32, "VSIF_SUB_PKT3_L");

    return HI_SUCCESS;
}

int HDMI_VSIF_SUB_PKT3_H_vsif_pkt3_pbSet(unsigned int uvsif_pkt3_pb4,unsigned int uvsif_pkt3_pb5,unsigned int uvsif_pkt3_pb6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VSIF_SUB_PKT3_H o_vsif_sub_pkt3_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->VSIF_SUB_PKT3_H.u32);
    o_vsif_sub_pkt3_h.u32 = HDMITXRegRead(pu32RegAddr);
    o_vsif_sub_pkt3_h.bits.vsif_sub_pkt3_pb6 = uvsif_pkt3_pb6;
    o_vsif_sub_pkt3_h.bits.vsif_sub_pkt3_pb5 = uvsif_pkt3_pb5;
    o_vsif_sub_pkt3_h.bits.vsif_sub_pkt3_pb4 = uvsif_pkt3_pb4;
    HDMITXRegWrite(pu32RegAddr, o_vsif_sub_pkt3_h.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_vsif_sub_pkt3_h.u32, "VSIF_SUB_PKT3_H");

    return HI_SUCCESS;
}


//*************************************************************************************//
//********************************  Vsif    信息帧获取******************************//
//*************************************************************************************//
int HDMI_VSIF_PKT_HEADER_hbGet(U_VSIF_PKT_HEADER* u_vsif_pkt_header)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VSIF_PKT_HEADER o_vsif_pkt_header;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->VSIF_PKT_HEADER.u32);
    o_vsif_pkt_header.u32 = HDMITXRegRead(pu32RegAddr);
    u_vsif_pkt_header->bits.vsif_pkt_hb2 = o_vsif_pkt_header.bits.vsif_pkt_hb2;
    u_vsif_pkt_header->bits.vsif_pkt_hb1 = o_vsif_pkt_header.bits.vsif_pkt_hb1;
    u_vsif_pkt_header->bits.vsif_pkt_hb0 = o_vsif_pkt_header.bits.vsif_pkt_hb0;

    return HI_SUCCESS;
}

int HDMI_VSIF_SUB_PKT0_L_vsif_pkt0_pbGet(U_VSIF_SUB_PKT0_L* u_vsif_sub_pkt0_l)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VSIF_SUB_PKT0_L o_vsif_sub_pkt0_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->VSIF_SUB_PKT0_L.u32);
    o_vsif_sub_pkt0_l.u32 = HDMITXRegRead(pu32RegAddr);
    u_vsif_sub_pkt0_l->bits.vsif_sub_pkt0_pb3 = o_vsif_sub_pkt0_l.bits.vsif_sub_pkt0_pb3;
    u_vsif_sub_pkt0_l->bits.vsif_sub_pkt0_pb2 = o_vsif_sub_pkt0_l.bits.vsif_sub_pkt0_pb2;
    u_vsif_sub_pkt0_l->bits.vsif_sub_pkt0_pb1 = o_vsif_sub_pkt0_l.bits.vsif_sub_pkt0_pb1;
    u_vsif_sub_pkt0_l->bits.vsif_sub_pkt0_pb0 = o_vsif_sub_pkt0_l.bits.vsif_sub_pkt0_pb0;

    return HI_SUCCESS;
}

int HDMI_VSIF_SUB_PKT0_H_vsif_pkt0_pbGet(U_VSIF_SUB_PKT0_H* u_vsif_sub_pkt0_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VSIF_SUB_PKT0_H o_vsif_sub_pkt0_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->VSIF_SUB_PKT0_H.u32);
    o_vsif_sub_pkt0_h.u32 = HDMITXRegRead(pu32RegAddr);
    u_vsif_sub_pkt0_h->bits.vsif_sub_pkt0_pb6 = o_vsif_sub_pkt0_h.bits.vsif_sub_pkt0_pb6;
    u_vsif_sub_pkt0_h->bits.vsif_sub_pkt0_pb5 = o_vsif_sub_pkt0_h.bits.vsif_sub_pkt0_pb5;
    u_vsif_sub_pkt0_h->bits.vsif_sub_pkt0_pb4 = o_vsif_sub_pkt0_h.bits.vsif_sub_pkt0_pb4;

    return HI_SUCCESS;
}

int HDMI_VSIF_SUB_PKT1_L_vsif_pkt1_pbGet(U_VSIF_SUB_PKT1_L* u_vsif_sub_pkt1_l)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VSIF_SUB_PKT1_L o_vsif_sub_pkt1_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->VSIF_SUB_PKT1_L.u32);
    o_vsif_sub_pkt1_l.u32 = HDMITXRegRead(pu32RegAddr);
    u_vsif_sub_pkt1_l->bits.vsif_sub_pkt1_pb3 = o_vsif_sub_pkt1_l.bits.vsif_sub_pkt1_pb3;
    u_vsif_sub_pkt1_l->bits.vsif_sub_pkt1_pb2 = o_vsif_sub_pkt1_l.bits.vsif_sub_pkt1_pb2;
    u_vsif_sub_pkt1_l->bits.vsif_sub_pkt1_pb1 = o_vsif_sub_pkt1_l.bits.vsif_sub_pkt1_pb1;
    u_vsif_sub_pkt1_l->bits.vsif_sub_pkt1_pb0 = o_vsif_sub_pkt1_l.bits.vsif_sub_pkt1_pb0;

    return HI_SUCCESS;
}

int HDMI_VSIF_SUB_PKT1_H_vsif_pkt1_pbGet(U_VSIF_SUB_PKT1_H* u_vsif_sub_pkt1_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VSIF_SUB_PKT1_H o_vsif_sub_pkt1_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->VSIF_SUB_PKT1_H.u32);
    o_vsif_sub_pkt1_h.u32 = HDMITXRegRead(pu32RegAddr);
    u_vsif_sub_pkt1_h->bits.vsif_sub_pkt1_pb6 = o_vsif_sub_pkt1_h.bits.vsif_sub_pkt1_pb6;
    u_vsif_sub_pkt1_h->bits.vsif_sub_pkt1_pb5 = o_vsif_sub_pkt1_h.bits.vsif_sub_pkt1_pb5;
    u_vsif_sub_pkt1_h->bits.vsif_sub_pkt1_pb4 = o_vsif_sub_pkt1_h.bits.vsif_sub_pkt1_pb4;

    return HI_SUCCESS;
}

int HDMI_VSIF_SUB_PKT2_L_vsif_pkt2_pbGet(U_VSIF_SUB_PKT2_L* u_vsif_sub_pkt2_l)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VSIF_SUB_PKT2_L o_vsif_sub_pkt2_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->VSIF_SUB_PKT2_L.u32);
    o_vsif_sub_pkt2_l.u32 = HDMITXRegRead(pu32RegAddr);
    u_vsif_sub_pkt2_l->bits.vsif_sub_pkt2_pb3 = o_vsif_sub_pkt2_l.bits.vsif_sub_pkt2_pb3;
    u_vsif_sub_pkt2_l->bits.vsif_sub_pkt2_pb2 = o_vsif_sub_pkt2_l.bits.vsif_sub_pkt2_pb2;
    u_vsif_sub_pkt2_l->bits.vsif_sub_pkt2_pb1 = o_vsif_sub_pkt2_l.bits.vsif_sub_pkt2_pb1;
    u_vsif_sub_pkt2_l->bits.vsif_sub_pkt2_pb0 = o_vsif_sub_pkt2_l.bits.vsif_sub_pkt2_pb0;

    return HI_SUCCESS;
}

int HDMI_VSIF_SUB_PKT2_H_vsif_pkt2_pbGet(U_VSIF_SUB_PKT2_H* u_vsif_sub_pkt2_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VSIF_SUB_PKT2_H o_vsif_sub_pkt2_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->VSIF_SUB_PKT2_H.u32);
    o_vsif_sub_pkt2_h.u32 = HDMITXRegRead(pu32RegAddr);
    u_vsif_sub_pkt2_h->bits.vsif_sub_pkt2_pb6 = o_vsif_sub_pkt2_h.bits.vsif_sub_pkt2_pb6;
    u_vsif_sub_pkt2_h->bits.vsif_sub_pkt2_pb5 = o_vsif_sub_pkt2_h.bits.vsif_sub_pkt2_pb5;
    u_vsif_sub_pkt2_h->bits.vsif_sub_pkt2_pb4 = o_vsif_sub_pkt2_h.bits.vsif_sub_pkt2_pb4;

    return HI_SUCCESS;
}

int HDMI_VSIF_SUB_PKT3_L_vsif_pkt3_pbGet(U_VSIF_SUB_PKT3_L* u_vsif_sub_pkt3_l)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VSIF_SUB_PKT3_L o_vsif_sub_pkt3_l;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->VSIF_SUB_PKT3_L.u32);
    o_vsif_sub_pkt3_l.u32 = HDMITXRegRead(pu32RegAddr);
    u_vsif_sub_pkt3_l->bits.vsif_sub_pkt3_pb3 = o_vsif_sub_pkt3_l.bits.vsif_sub_pkt3_pb3;
    u_vsif_sub_pkt3_l->bits.vsif_sub_pkt3_pb2 = o_vsif_sub_pkt3_l.bits.vsif_sub_pkt3_pb2;
    u_vsif_sub_pkt3_l->bits.vsif_sub_pkt3_pb1 = o_vsif_sub_pkt3_l.bits.vsif_sub_pkt3_pb1;
    u_vsif_sub_pkt3_l->bits.vsif_sub_pkt3_pb0 = o_vsif_sub_pkt3_l.bits.vsif_sub_pkt3_pb0;

    return HI_SUCCESS;
}

int HDMI_VSIF_SUB_PKT3_H_vsif_pkt3_pbGet(U_VSIF_SUB_PKT3_H* u_vsif_sub_pkt3_h)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VSIF_SUB_PKT3_H o_vsif_sub_pkt3_h;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->VSIF_SUB_PKT3_H.u32);
    o_vsif_sub_pkt3_h.u32 = HDMITXRegRead(pu32RegAddr);
    u_vsif_sub_pkt3_h->bits.vsif_sub_pkt3_pb6 = o_vsif_sub_pkt3_h.bits.vsif_sub_pkt3_pb6;
    u_vsif_sub_pkt3_h->bits.vsif_sub_pkt3_pb5 = o_vsif_sub_pkt3_h.bits.vsif_sub_pkt3_pb5;
    u_vsif_sub_pkt3_h->bits.vsif_sub_pkt3_pb4 = o_vsif_sub_pkt3_h.bits.vsif_sub_pkt3_pb4;

    return HI_SUCCESS;
}



//******************************************************************************
//  Function    : HDMI_CEA_AVI_CFG_cea_avi_rpt_enSet
//  Description : Set the value of the member CEA_AVI_CFG.cea_avi_rpt_en
//  Input       : unsigned int ucea_avi_rpt_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_AVI_CFG_cea_avi_rpt_enSet(unsigned int ucea_avi_rpt_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_AVI_CFG o_cea_avi_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_AVI_CFG.u32);
    o_cea_avi_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_cea_avi_cfg.bits.cea_avi_rpt_en = ucea_avi_rpt_en;
    HDMITXRegWrite(pu32RegAddr, o_cea_avi_cfg.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cea_avi_cfg.u32, "CEA_AVI_CFG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEA_AVI_CFG_cea_avi_enSet
//  Description : Set the value of the member CEA_AVI_CFG.cea_avi_en
//  Input       : unsigned int ucea_avi_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_AVI_CFG_cea_avi_enSet(unsigned int ucea_avi_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_AVI_CFG o_cea_avi_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_AVI_CFG.u32);
    o_cea_avi_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_cea_avi_cfg.bits.cea_avi_en = ucea_avi_en;
    HDMITXRegWrite(pu32RegAddr, o_cea_avi_cfg.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cea_avi_cfg.u32, "CEA_AVI_CFG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEA_SPF_CFG_cea_spf_rpt_enSet
//  Description : Set the value of the member CEA_SPF_CFG.cea_spf_rpt_en
//  Input       : unsigned int ucea_spf_rpt_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_SPF_CFG_cea_spf_rpt_enSet(unsigned int ucea_spf_rpt_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_SPF_CFG o_cea_spf_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_SPF_CFG.u32);
    o_cea_spf_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_cea_spf_cfg.bits.cea_spf_rpt_en = ucea_spf_rpt_en;
    HDMITXRegWrite(pu32RegAddr, o_cea_spf_cfg.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cea_spf_cfg.u32, "CEA_SPF_CFG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEA_SPF_CFG_cea_spf_enSet
//  Description : Set the value of the member CEA_SPF_CFG.cea_spf_en
//  Input       : unsigned int ucea_spf_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_SPF_CFG_cea_spf_enSet(unsigned int ucea_spf_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_SPF_CFG o_cea_spf_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_SPF_CFG.u32);
    o_cea_spf_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_cea_spf_cfg.bits.cea_spf_en = ucea_spf_en;
    HDMITXRegWrite(pu32RegAddr, o_cea_spf_cfg.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cea_spf_cfg.u32, "CEA_SPF_CFG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEA_AUD_CFG_cea_aud_rpt_enSet
//  Description : Set the value of the member CEA_AUD_CFG.cea_aud_rpt_en
//  Input       : unsigned int ucea_aud_rpt_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_AUD_CFG_cea_aud_rpt_enSet(unsigned int ucea_aud_rpt_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_AUD_CFG o_cea_aud_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_AUD_CFG.u32);
    o_cea_aud_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_cea_aud_cfg.bits.cea_aud_rpt_en = ucea_aud_rpt_en;
    HDMITXRegWrite(pu32RegAddr, o_cea_aud_cfg.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cea_aud_cfg.u32, "CEA_AUD_CFG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEA_AUD_CFG_cea_aud_enSet
//  Description : Set the value of the member CEA_AUD_CFG.cea_aud_en
//  Input       : unsigned int ucea_aud_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_AUD_CFG_cea_aud_enSet(unsigned int ucea_aud_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_AUD_CFG o_cea_aud_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_AUD_CFG.u32);
    o_cea_aud_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_cea_aud_cfg.bits.cea_aud_en = ucea_aud_en;
    HDMITXRegWrite(pu32RegAddr, o_cea_aud_cfg.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cea_aud_cfg.u32, "CEA_AUD_CFG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEA_MPEG_CFG_cea_mpeg_rpt_enSet
//  Description : Set the value of the member CEA_MPEG_CFG.cea_mpeg_rpt_en
//  Input       : unsigned int ucea_mpeg_rpt_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_MPEG_CFG_cea_mpeg_rpt_enSet(unsigned int ucea_mpeg_rpt_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_MPEG_CFG o_cea_mpeg_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_MPEG_CFG.u32);
    o_cea_mpeg_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_cea_mpeg_cfg.bits.cea_mpeg_rpt_en = ucea_mpeg_rpt_en;
    HDMITXRegWrite(pu32RegAddr, o_cea_mpeg_cfg.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cea_mpeg_cfg.u32, "CEA_MPEG_CFG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEA_MPEG_CFG_cea_mpeg_enSet
//  Description : Set the value of the member CEA_MPEG_CFG.cea_mpeg_en
//  Input       : unsigned int ucea_mpeg_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_MPEG_CFG_cea_mpeg_enSet(unsigned int ucea_mpeg_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_MPEG_CFG o_cea_mpeg_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_MPEG_CFG.u32);
    o_cea_mpeg_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_cea_mpeg_cfg.bits.cea_mpeg_en = ucea_mpeg_en;
    HDMITXRegWrite(pu32RegAddr, o_cea_mpeg_cfg.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cea_mpeg_cfg.u32, "CEA_MPEG_CFG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEA_GEN_CFG_cea_gen_rpt_enSet
//  Description : Set the value of the member CEA_GEN_CFG.cea_gen_rpt_en
//  Input       : unsigned int ucea_gen_rpt_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_GEN_CFG_cea_gen_rpt_enSet(unsigned int ucea_gen_rpt_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_GEN_CFG o_cea_gen_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_GEN_CFG.u32);
    o_cea_gen_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_cea_gen_cfg.bits.cea_gen_rpt_en = ucea_gen_rpt_en;
    HDMITXRegWrite(pu32RegAddr, o_cea_gen_cfg.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cea_gen_cfg.u32, "CEA_GEN_CFG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEA_GEN_CFG_cea_gen_enSet
//  Description : Set the value of the member CEA_GEN_CFG.cea_gen_en
//  Input       : unsigned int ucea_gen_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_GEN_CFG_cea_gen_enSet(unsigned int ucea_gen_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_GEN_CFG o_cea_gen_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_GEN_CFG.u32);
    o_cea_gen_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_cea_gen_cfg.bits.cea_gen_en = ucea_gen_en;
    HDMITXRegWrite(pu32RegAddr, o_cea_gen_cfg.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cea_gen_cfg.u32, "CEA_GEN_CFG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEA_CP_CFG_cea_cp_rpt_cntSet
//  Description : Set the value of the member CEA_CP_CFG.cea_cp_rpt_en
//  Input       : unsigned int cea_cp_rpt_cnt: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_CP_CFG_cea_cp_rpt_cntSet(unsigned int ucea_cp_rpt_cnt)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_CP_CFG o_cea_cp_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_CP_CFG.u32);
    o_cea_cp_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_cea_cp_cfg.bits.cea_cp_rpt_cnt = ucea_cp_rpt_cnt;
    HDMITXRegWrite(pu32RegAddr, o_cea_cp_cfg.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cea_cp_cfg.u32, "CEA_CP_CFG");

    return HI_SUCCESS;
}


//******************************************************************************
//  Function    : HDMI_CEA_CP_CFG_cea_cp_rpt_enSet
//  Description : Set the value of the member CEA_CP_CFG.cea_cp_rpt_en
//  Input       : unsigned int ucea_cp_rpt_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_CP_CFG_cea_cp_rpt_enSet(unsigned int ucea_cp_rpt_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_CP_CFG o_cea_cp_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_CP_CFG.u32);
    o_cea_cp_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_cea_cp_cfg.bits.cea_cp_rpt_en = ucea_cp_rpt_en;
    HDMITXRegWrite(pu32RegAddr, o_cea_cp_cfg.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cea_cp_cfg.u32, "CEA_CP_CFG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEA_CP_CFG_cea_cp_enSet
//  Description : Set the value of the member CEA_CP_CFG.cea_cp_en
//  Input       : unsigned int ucea_cp_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_CP_CFG_cea_cp_enSet(unsigned int ucea_cp_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_CP_CFG o_cea_cp_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_CP_CFG.u32);
    o_cea_cp_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_cea_cp_cfg.bits.cea_cp_en = ucea_cp_en;
    HDMITXRegWrite(pu32RegAddr, o_cea_cp_cfg.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cea_cp_cfg.u32, "CEA_CP_CFG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEA_GEN2_CFG_cea_gen2_rpt_enSet
//  Description : Set the value of the member CEA_GEN2_CFG.cea_gen2_rpt_en
//  Input       : unsigned int ucea_gen2_rpt_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_GEN2_CFG_cea_gen2_rpt_enSet(unsigned int ucea_gen2_rpt_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_GEN2_CFG o_cea_gen2_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_GEN2_CFG.u32);
    o_cea_gen2_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_cea_gen2_cfg.bits.cea_gen2_rpt_en = ucea_gen2_rpt_en;
    HDMITXRegWrite(pu32RegAddr, o_cea_gen2_cfg.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cea_gen2_cfg.u32, "CEA_GEN2_CFG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEA_GEN2_CFG_cea_gen2_enSet
//  Description : Set the value of the member CEA_GEN2_CFG.cea_gen2_en
//  Input       : unsigned int ucea_gen2_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_GEN2_CFG_cea_gen2_enSet(unsigned int ucea_gen2_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_GEN2_CFG o_cea_gen2_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_GEN2_CFG.u32);
    o_cea_gen2_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_cea_gen2_cfg.bits.cea_gen2_en = ucea_gen2_en;
    HDMITXRegWrite(pu32RegAddr, o_cea_gen2_cfg.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cea_gen2_cfg.u32, "CEA_GEN2_CFG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEA_GEN3_CFG_cea_gen3_rpt_enSet
//  Description : Set the value of the member CEA_GEN3_CFG.cea_gen3_rpt_en
//  Input       : unsigned int ucea_gen3_rpt_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_GEN3_CFG_cea_gen3_rpt_enSet(unsigned int ucea_gen3_rpt_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_GEN3_CFG o_cea_gen3_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_GEN3_CFG.u32);
    o_cea_gen3_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_cea_gen3_cfg.bits.cea_gen3_rpt_en = ucea_gen3_rpt_en;
    HDMITXRegWrite(pu32RegAddr, o_cea_gen3_cfg.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cea_gen3_cfg.u32, "CEA_GEN3_CFG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEA_GEN3_CFG_cea_gen3_enSet
//  Description : Set the value of the member CEA_GEN3_CFG.cea_gen3_en
//  Input       : unsigned int ucea_gen3_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_GEN3_CFG_cea_gen3_enSet(unsigned int ucea_gen3_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_GEN3_CFG o_cea_gen3_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_GEN3_CFG.u32);
    o_cea_gen3_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_cea_gen3_cfg.bits.cea_gen3_en = ucea_gen3_en;
    HDMITXRegWrite(pu32RegAddr, o_cea_gen3_cfg.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cea_gen3_cfg.u32, "CEA_GEN3_CFG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEA_GEN4_CFG_cea_gen4_rpt_enSet
//  Description : Set the value of the member CEA_GEN4_CFG.cea_gen4_rpt_en
//  Input       : unsigned int ucea_gen4_rpt_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_GEN4_CFG_cea_gen4_rpt_enSet(unsigned int ucea_gen4_rpt_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_GEN4_CFG o_cea_gen4_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_GEN4_CFG.u32);
    o_cea_gen4_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_cea_gen4_cfg.bits.cea_gen4_rpt_en = ucea_gen4_rpt_en;
    HDMITXRegWrite(pu32RegAddr, o_cea_gen4_cfg.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cea_gen4_cfg.u32, "CEA_GEN4_CFG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEA_GEN4_CFG_cea_gen4_enSet
//  Description : Set the value of the member CEA_GEN4_CFG.cea_gen4_en
//  Input       : unsigned int ucea_gen4_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_GEN4_CFG_cea_gen4_enSet(unsigned int ucea_gen4_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_GEN4_CFG o_cea_gen4_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_GEN4_CFG.u32);
    o_cea_gen4_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_cea_gen4_cfg.bits.cea_gen4_en = ucea_gen4_en;
    HDMITXRegWrite(pu32RegAddr, o_cea_gen4_cfg.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cea_gen4_cfg.u32, "CEA_GEN4_CFG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEA_GEN5_CFG_cea_gen5_rpt_enSet
//  Description : Set the value of the member CEA_GEN5_CFG.cea_gen5_rpt_en
//  Input       : unsigned int ucea_gen5_rpt_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_GEN5_CFG_cea_gen5_rpt_enSet(unsigned int ucea_gen5_rpt_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_GEN5_CFG o_cea_gen5_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_GEN5_CFG.u32);
    o_cea_gen5_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_cea_gen5_cfg.bits.cea_gen5_rpt_en = ucea_gen5_rpt_en;
    HDMITXRegWrite(pu32RegAddr, o_cea_gen5_cfg.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cea_gen5_cfg.u32, "CEA_GEN5_CFG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEA_GEN5_CFG_cea_gen5_enSet
//  Description : Set the value of the member CEA_GEN5_CFG.cea_gen5_en
//  Input       : unsigned int ucea_gen5_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_GEN5_CFG_cea_gen5_enSet(unsigned int ucea_gen5_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_GEN5_CFG o_cea_gen5_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_GEN5_CFG.u32);
    o_cea_gen5_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_cea_gen5_cfg.bits.cea_gen5_en = ucea_gen5_en;
    HDMITXRegWrite(pu32RegAddr, o_cea_gen5_cfg.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cea_gen5_cfg.u32, "CEA_GEN5_CFG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEA_GAMUT_CFG_cea_gamut_rpt_enSet
//  Description : Set the value of the member CEA_GAMUT_CFG.cea_gamut_rpt_en
//  Input       : unsigned int ucea_gamut_rpt_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_GAMUT_CFG_cea_gamut_rpt_enSet(unsigned int ucea_gamut_rpt_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_GAMUT_CFG o_cea_gamut_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_GAMUT_CFG.u32);
    o_cea_gamut_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_cea_gamut_cfg.bits.cea_gamut_rpt_en = ucea_gamut_rpt_en;
    HDMITXRegWrite(pu32RegAddr, o_cea_gamut_cfg.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cea_gamut_cfg.u32, "CEA_GAMUT_CFG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEA_GAMUT_CFG_cea_gamut_enSet
//  Description : Set the value of the member CEA_GAMUT_CFG.cea_gamut_en
//  Input       : unsigned int ucea_gamut_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_GAMUT_CFG_cea_gamut_enSet(unsigned int ucea_gamut_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_GAMUT_CFG o_cea_gamut_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_GAMUT_CFG.u32);
    o_cea_gamut_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_cea_gamut_cfg.bits.cea_gamut_en = ucea_gamut_en;
    HDMITXRegWrite(pu32RegAddr, o_cea_gamut_cfg.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cea_gamut_cfg.u32, "CEA_GAMUT_CFG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEA_VSIF_CFG_cea_vsif_rpt_enSet
//  Description : Set the value of the member CEA_VSIF_CFG.cea_vsif_rpt_en
//  Input       : unsigned int ucea_vsif_rpt_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_VSIF_CFG_cea_vsif_rpt_enSet(unsigned int ucea_vsif_rpt_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_VSIF_CFG o_cea_vsif_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_VSIF_CFG.u32);
    o_cea_vsif_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_cea_vsif_cfg.bits.cea_vsif_rpt_en = ucea_vsif_rpt_en;
    HDMITXRegWrite(pu32RegAddr, o_cea_vsif_cfg.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cea_vsif_cfg.u32, "CEA_VSIF_CFG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEA_VSIF_CFG_cea_vsif_enSet
//  Description : Set the value of the member CEA_VSIF_CFG.cea_vsif_en
//  Input       : unsigned int ucea_vsif_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_VSIF_CFG_cea_vsif_enSet(unsigned int ucea_vsif_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_VSIF_CFG o_cea_vsif_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_VSIF_CFG.u32);
    o_cea_vsif_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_cea_vsif_cfg.bits.cea_vsif_en = ucea_vsif_en;
    HDMITXRegWrite(pu32RegAddr, o_cea_vsif_cfg.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cea_vsif_cfg.u32, "CEA_VSIF_CFG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEA_PKT_STATE_cea_vsif_stateSet
//  Description : Set the value of the member CEA_PKT_STATE.cea_vsif_state
//  Input       : unsigned int ucea_vsif_state: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_PKT_STATE_cea_vsif_stateSet(unsigned int ucea_vsif_state)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_PKT_STATE o_cea_pkt_state;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_PKT_STATE.u32);
    o_cea_pkt_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_cea_pkt_state.bits.cea_vsif_state = ucea_vsif_state;
    HDMITXRegWrite(pu32RegAddr, o_cea_pkt_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cea_pkt_state.u32, "CEA_PKT_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEA_PKT_STATE_cea_gamut_stateSet
//  Description : Set the value of the member CEA_PKT_STATE.cea_gamut_state
//  Input       : unsigned int ucea_gamut_state: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_PKT_STATE_cea_gamut_stateSet(unsigned int ucea_gamut_state)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_PKT_STATE o_cea_pkt_state;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_PKT_STATE.u32);
    o_cea_pkt_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_cea_pkt_state.bits.cea_gamut_state = ucea_gamut_state;
    HDMITXRegWrite(pu32RegAddr, o_cea_pkt_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cea_pkt_state.u32, "CEA_PKT_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEA_PKT_STATE_cea_mpeg_stateSet
//  Description : Set the value of the member CEA_PKT_STATE.cea_mpeg_state
//  Input       : unsigned int ucea_mpeg_state: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_PKT_STATE_cea_mpeg_stateSet(unsigned int ucea_mpeg_state)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_PKT_STATE o_cea_pkt_state;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_PKT_STATE.u32);
    o_cea_pkt_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_cea_pkt_state.bits.cea_mpeg_state = ucea_mpeg_state;
    HDMITXRegWrite(pu32RegAddr, o_cea_pkt_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cea_pkt_state.u32, "CEA_PKT_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEA_PKT_STATE_cea_spd_stateSet
//  Description : Set the value of the member CEA_PKT_STATE.cea_spd_state
//  Input       : unsigned int ucea_spd_state: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_PKT_STATE_cea_spd_stateSet(unsigned int ucea_spd_state)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_PKT_STATE o_cea_pkt_state;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_PKT_STATE.u32);
    o_cea_pkt_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_cea_pkt_state.bits.cea_spd_state = ucea_spd_state;
    HDMITXRegWrite(pu32RegAddr, o_cea_pkt_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cea_pkt_state.u32, "CEA_PKT_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEA_PKT_STATE_cea_gen5_stateSet
//  Description : Set the value of the member CEA_PKT_STATE.cea_gen5_state
//  Input       : unsigned int ucea_gen5_state: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_PKT_STATE_cea_gen5_stateSet(unsigned int ucea_gen5_state)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_PKT_STATE o_cea_pkt_state;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_PKT_STATE.u32);
    o_cea_pkt_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_cea_pkt_state.bits.cea_gen5_state = ucea_gen5_state;
    HDMITXRegWrite(pu32RegAddr, o_cea_pkt_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cea_pkt_state.u32, "CEA_PKT_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEA_PKT_STATE_cea_gen4_stateSet
//  Description : Set the value of the member CEA_PKT_STATE.cea_gen4_state
//  Input       : unsigned int ucea_gen4_state: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_PKT_STATE_cea_gen4_stateSet(unsigned int ucea_gen4_state)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_PKT_STATE o_cea_pkt_state;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_PKT_STATE.u32);
    o_cea_pkt_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_cea_pkt_state.bits.cea_gen4_state = ucea_gen4_state;
    HDMITXRegWrite(pu32RegAddr, o_cea_pkt_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cea_pkt_state.u32, "CEA_PKT_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEA_PKT_STATE_cea_gen3_stateSet
//  Description : Set the value of the member CEA_PKT_STATE.cea_gen3_state
//  Input       : unsigned int ucea_gen3_state: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_PKT_STATE_cea_gen3_stateSet(unsigned int ucea_gen3_state)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_PKT_STATE o_cea_pkt_state;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_PKT_STATE.u32);
    o_cea_pkt_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_cea_pkt_state.bits.cea_gen3_state = ucea_gen3_state;
    HDMITXRegWrite(pu32RegAddr, o_cea_pkt_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cea_pkt_state.u32, "CEA_PKT_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEA_PKT_STATE_cea_gen2_stateSet
//  Description : Set the value of the member CEA_PKT_STATE.cea_gen2_state
//  Input       : unsigned int ucea_gen2_state: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_PKT_STATE_cea_gen2_stateSet(unsigned int ucea_gen2_state)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_PKT_STATE o_cea_pkt_state;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_PKT_STATE.u32);
    o_cea_pkt_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_cea_pkt_state.bits.cea_gen2_state = ucea_gen2_state;
    HDMITXRegWrite(pu32RegAddr, o_cea_pkt_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cea_pkt_state.u32, "CEA_PKT_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEA_PKT_STATE_cea_gen_stateSet
//  Description : Set the value of the member CEA_PKT_STATE.cea_gen_state
//  Input       : unsigned int ucea_gen_state: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_PKT_STATE_cea_gen_stateSet(unsigned int ucea_gen_state)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_PKT_STATE o_cea_pkt_state;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_PKT_STATE.u32);
    o_cea_pkt_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_cea_pkt_state.bits.cea_gen_state = ucea_gen_state;
    HDMITXRegWrite(pu32RegAddr, o_cea_pkt_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cea_pkt_state.u32, "CEA_PKT_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEA_PKT_STATE_cea_cp_stateSet
//  Description : Set the value of the member CEA_PKT_STATE.cea_cp_state
//  Input       : unsigned int ucea_cp_state: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_PKT_STATE_cea_cp_stateSet(unsigned int ucea_cp_state)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_PKT_STATE o_cea_pkt_state;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_PKT_STATE.u32);
    o_cea_pkt_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_cea_pkt_state.bits.cea_cp_state = ucea_cp_state;
    HDMITXRegWrite(pu32RegAddr, o_cea_pkt_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cea_pkt_state.u32, "CEA_PKT_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEA_PKT_STATE_cea_aud_stateSet
//  Description : Set the value of the member CEA_PKT_STATE.cea_aud_state
//  Input       : unsigned int ucea_aud_state: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_PKT_STATE_cea_aud_stateSet(unsigned int ucea_aud_state)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_PKT_STATE o_cea_pkt_state;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_PKT_STATE.u32);
    o_cea_pkt_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_cea_pkt_state.bits.cea_aud_state = ucea_aud_state;
    HDMITXRegWrite(pu32RegAddr, o_cea_pkt_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cea_pkt_state.u32, "CEA_PKT_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CEA_PKT_STATE_cea_avi_stateSet
//  Description : Set the value of the member CEA_PKT_STATE.cea_avi_state
//  Input       : unsigned int ucea_avi_state: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_PKT_STATE_cea_avi_stateSet(unsigned int ucea_avi_state)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_PKT_STATE o_cea_pkt_state;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_PKT_STATE.u32);
    o_cea_pkt_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_cea_pkt_state.bits.cea_avi_state = ucea_avi_state;
    HDMITXRegWrite(pu32RegAddr, o_cea_pkt_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cea_pkt_state.u32, "CEA_PKT_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_AVMIXER_CONFIG_pkt_bypass_modeSet
//  Description : Set the value of the member AVMIXER_CONFIG.pkt_bypass_mode
//  Input       : unsigned int upkt_bypass_mode: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AVMIXER_CONFIG_pkt_bypass_modeSet(unsigned int upkt_bypass_mode)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVMIXER_CONFIG o_avmixer_config;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVMIXER_CONFIG.u32);
    o_avmixer_config.u32 = HDMITXRegRead(pu32RegAddr);
    o_avmixer_config.bits.pkt_bypass_mode = upkt_bypass_mode;
    HDMITXRegWrite(pu32RegAddr, o_avmixer_config.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_avmixer_config.u32, "AVMIXER_CONFIG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_AVMIXER_CONFIG_pb_priotity_ctlSet
//  Description : Set the value of the member AVMIXER_CONFIG.pb_priotity_ctl
//  Input       : unsigned int upb_priotity_ctl: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AVMIXER_CONFIG_pb_priotity_ctlSet(unsigned int upb_priotity_ctl)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVMIXER_CONFIG o_avmixer_config;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVMIXER_CONFIG.u32);
    o_avmixer_config.u32 = HDMITXRegRead(pu32RegAddr);
    o_avmixer_config.bits.pb_priotity_ctl = upb_priotity_ctl;
    HDMITXRegWrite(pu32RegAddr, o_avmixer_config.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_avmixer_config.u32, "AVMIXER_CONFIG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_AVMIXER_CONFIG_pb_ovr_dc_pkt_enSet
//  Description : Set the value of the member AVMIXER_CONFIG.pb_ovr_dc_pkt_en
//  Input       : unsigned int upb_ovr_dc_pkt_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AVMIXER_CONFIG_pb_ovr_dc_pkt_enSet(unsigned int upb_ovr_dc_pkt_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVMIXER_CONFIG o_avmixer_config;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVMIXER_CONFIG.u32);
    o_avmixer_config.u32 = HDMITXRegRead(pu32RegAddr);
    o_avmixer_config.bits.pb_ovr_dc_pkt_en = upb_ovr_dc_pkt_en;
    HDMITXRegWrite(pu32RegAddr, o_avmixer_config.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_avmixer_config.u32, "AVMIXER_CONFIG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_AVMIXER_CONFIG_intr_encryptionSet
//  Description : Set the value of the member AVMIXER_CONFIG.intr_encryption
//  Input       : unsigned int uintr_encryption: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AVMIXER_CONFIG_intr_encryptionSet(unsigned int uintr_encryption)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVMIXER_CONFIG o_avmixer_config;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVMIXER_CONFIG.u32);
    o_avmixer_config.u32 = HDMITXRegRead(pu32RegAddr);
    o_avmixer_config.bits.intr_encryption = uintr_encryption;
    HDMITXRegWrite(pu32RegAddr, o_avmixer_config.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_avmixer_config.u32, "AVMIXER_CONFIG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_AVMIXER_CONFIG_null_pkt_en_vs_highSet
//  Description : Set the value of the member AVMIXER_CONFIG.null_pkt_en_vs_high
//  Input       : unsigned int unull_pkt_en_vs_high: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AVMIXER_CONFIG_null_pkt_en_vs_highSet(unsigned int unull_pkt_en_vs_high)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVMIXER_CONFIG o_avmixer_config;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVMIXER_CONFIG.u32);
    o_avmixer_config.u32 = HDMITXRegRead(pu32RegAddr);
    o_avmixer_config.bits.null_pkt_en_vs_high = unull_pkt_en_vs_high;
    HDMITXRegWrite(pu32RegAddr, o_avmixer_config.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_avmixer_config.u32, "AVMIXER_CONFIG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_AVMIXER_CONFIG_null_pkt_enSet
//  Description : Set the value of the member AVMIXER_CONFIG.null_pkt_en
//  Input       : unsigned int unull_pkt_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AVMIXER_CONFIG_null_pkt_enSet(unsigned int unull_pkt_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVMIXER_CONFIG o_avmixer_config;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVMIXER_CONFIG.u32);
    o_avmixer_config.u32 = HDMITXRegRead(pu32RegAddr);
    o_avmixer_config.bits.null_pkt_en = unull_pkt_en;
    HDMITXRegWrite(pu32RegAddr, o_avmixer_config.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_avmixer_config.u32, "AVMIXER_CONFIG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_AVMIXER_CONFIG_dc_pkt_enSet
//  Description : Set the value of the member AVMIXER_CONFIG.dc_pkt_en
//  Input       : unsigned int udc_pkt_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AVMIXER_CONFIG_dc_pkt_enSet(unsigned int udc_pkt_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVMIXER_CONFIG o_avmixer_config;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVMIXER_CONFIG.u32);
    o_avmixer_config.u32 = HDMITXRegRead(pu32RegAddr);
    o_avmixer_config.bits.dc_pkt_en = udc_pkt_en;
    HDMITXRegWrite(pu32RegAddr, o_avmixer_config.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_avmixer_config.u32, "AVMIXER_CONFIG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_AVMIXER_CONFIG_hdmi_modeSet
//  Description : Set the value of the member AVMIXER_CONFIG.hdmi_mode
//  Input       : unsigned int uhdmi_mode: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AVMIXER_CONFIG_hdmi_modeSet(unsigned int uhdmi_mode)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVMIXER_CONFIG o_avmixer_config;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVMIXER_CONFIG.u32);
    o_avmixer_config.u32 = HDMITXRegRead(pu32RegAddr);
    o_avmixer_config.bits.hdmi_mode = uhdmi_mode;
    HDMITXRegWrite(pu32RegAddr, o_avmixer_config.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_avmixer_config.u32, "AVMIXER_CONFIG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CP_PKT_AVMUTE_cp_clr_avmuteSet
//  Description : Set the value of the member CP_PKT_AVMUTE.cp_clr_avmute
//  Input       : unsigned int ucp_clr_avmute: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CP_PKT_AVMUTE_cp_clr_avmuteSet(unsigned int ucp_clr_avmute)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CP_PKT_AVMUTE o_cp_pkt_avmute;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CP_PKT_AVMUTE.u32);
    o_cp_pkt_avmute.u32 = HDMITXRegRead(pu32RegAddr);
    o_cp_pkt_avmute.bits.cp_clr_avmute = ucp_clr_avmute;
    HDMITXRegWrite(pu32RegAddr, o_cp_pkt_avmute.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cp_pkt_avmute.u32, "CP_PKT_AVMUTE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_CP_PKT_AVMUTE_cp_set_avmuteSet
//  Description : Set the value of the member CP_PKT_AVMUTE.cp_set_avmute
//  Input       : unsigned int ucp_set_avmute: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CP_PKT_AVMUTE_cp_set_avmuteSet(unsigned int ucp_set_avmute)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CP_PKT_AVMUTE o_cp_pkt_avmute;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CP_PKT_AVMUTE.u32);
    o_cp_pkt_avmute.u32 = HDMITXRegRead(pu32RegAddr);
    o_cp_pkt_avmute.bits.cp_set_avmute = ucp_set_avmute;
    HDMITXRegWrite(pu32RegAddr, o_cp_pkt_avmute.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_cp_pkt_avmute.u32, "CP_PKT_AVMUTE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_VIDEO_BLANK_CFG_video_blankSet
//  Description : Set the value of the member VIDEO_BLANK_CFG.video_blank
//  Input       : unsigned int uvideo_blank: 24 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDEO_BLANK_CFG_video_blankSet(unsigned int uvideo_blank)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VIDEO_BLANK_CFG o_video_blank_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->VIDEO_BLANK_CFG.u32);
    o_video_blank_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_video_blank_cfg.bits.video_blank = uvideo_blank;
    HDMITXRegWrite(pu32RegAddr, o_video_blank_cfg.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_video_blank_cfg.u32, "VIDEO_BLANK_CFG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TMDS_BIST_CTRL_reg_tbist_patt_selSet
//  Description : Set the value of the member TMDS_BIST_CTRL.reg_tbist_patt_sel
//  Input       : unsigned int ureg_tbist_patt_sel: 5 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TMDS_BIST_CTRL_reg_tbist_patt_selSet(unsigned int ureg_tbist_patt_sel)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TMDS_BIST_CTRL o_tmds_bist_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->TMDS_BIST_CTRL.u32);
    o_tmds_bist_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_tmds_bist_ctrl.bits.reg_tbist_patt_sel = ureg_tbist_patt_sel;
    HDMITXRegWrite(pu32RegAddr, o_tmds_bist_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tmds_bist_ctrl.u32, "TMDS_BIST_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TMDS_BIST_CTRL_reg_tbist_timing_selSet
//  Description : Set the value of the member TMDS_BIST_CTRL.reg_tbist_timing_sel
//  Input       : unsigned int ureg_tbist_timing_sel: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TMDS_BIST_CTRL_reg_tbist_timing_selSet(unsigned int ureg_tbist_timing_sel)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TMDS_BIST_CTRL o_tmds_bist_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->TMDS_BIST_CTRL.u32);
    o_tmds_bist_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_tmds_bist_ctrl.bits.reg_tbist_timing_sel = ureg_tbist_timing_sel;
    HDMITXRegWrite(pu32RegAddr, o_tmds_bist_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tmds_bist_ctrl.u32, "TMDS_BIST_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TMDS_BIST_CTRL_reg_tbist_syn_polSet
//  Description : Set the value of the member TMDS_BIST_CTRL.reg_tbist_syn_pol
//  Input       : unsigned int ureg_tbist_syn_pol: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TMDS_BIST_CTRL_reg_tbist_syn_polSet(unsigned int ureg_tbist_syn_pol)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TMDS_BIST_CTRL o_tmds_bist_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->TMDS_BIST_CTRL.u32);
    o_tmds_bist_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_tmds_bist_ctrl.bits.reg_tbist_syn_pol = ureg_tbist_syn_pol;
    HDMITXRegWrite(pu32RegAddr, o_tmds_bist_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tmds_bist_ctrl.u32, "TMDS_BIST_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TMDS_BIST_CTRL_reg_tbist_enSet
//  Description : Set the value of the member TMDS_BIST_CTRL.reg_tbist_en
//  Input       : unsigned int ureg_tbist_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TMDS_BIST_CTRL_reg_tbist_enSet(unsigned int ureg_tbist_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TMDS_BIST_CTRL o_tmds_bist_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->TMDS_BIST_CTRL.u32);
    o_tmds_bist_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_tmds_bist_ctrl.bits.reg_tbist_en = ureg_tbist_en;
    HDMITXRegWrite(pu32RegAddr, o_tmds_bist_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tmds_bist_ctrl.u32, "TMDS_BIST_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMI_ENC_CTRL_enc_ck_div_selSet
//  Description : Set the value of the member HDMI_ENC_CTRL.enc_ck_div_sel
//  Input       : unsigned int uenc_ck_div_sel: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDMI_ENC_CTRL_enc_ck_div_selSet(unsigned int uenc_ck_div_sel)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMI_ENC_CTRL o_hdmi_enc_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->HDMI_ENC_CTRL.u32);
    o_hdmi_enc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmi_enc_ctrl.bits.enc_ck_div_sel = uenc_ck_div_sel;
    HDMITXRegWrite(pu32RegAddr, o_hdmi_enc_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmi_enc_ctrl.u32, "HDMI_ENC_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMI_ENC_CTRL_enc_bypassSet
//  Description : Set the value of the member HDMI_ENC_CTRL.enc_bypass
//  Input       : unsigned int uenc_bypass: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDMI_ENC_CTRL_enc_bypassSet(unsigned int uenc_bypass)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMI_ENC_CTRL o_hdmi_enc_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->HDMI_ENC_CTRL.u32);
    o_hdmi_enc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmi_enc_ctrl.bits.enc_bypass = uenc_bypass;
    HDMITXRegWrite(pu32RegAddr, o_hdmi_enc_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmi_enc_ctrl.u32, "HDMI_ENC_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMI_ENC_CTRL_enc_hdmi_ovrSet
//  Description : Set the value of the member HDMI_ENC_CTRL.enc_hdmi_ovr
//  Input       : unsigned int uenc_hdmi_ovr: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDMI_ENC_CTRL_enc_hdmi_ovrSet(unsigned int uenc_hdmi_ovr)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMI_ENC_CTRL o_hdmi_enc_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->HDMI_ENC_CTRL.u32);
    o_hdmi_enc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmi_enc_ctrl.bits.enc_hdmi_ovr = uenc_hdmi_ovr;
    HDMITXRegWrite(pu32RegAddr, o_hdmi_enc_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmi_enc_ctrl.u32, "HDMI_ENC_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMI_ENC_CTRL_enc_hdmi_valSet
//  Description : Set the value of the member HDMI_ENC_CTRL.enc_hdmi_val
//  Input       : unsigned int uenc_hdmi_val: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDMI_ENC_CTRL_enc_hdmi_valSet(unsigned int uenc_hdmi_val)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMI_ENC_CTRL o_hdmi_enc_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->HDMI_ENC_CTRL.u32);
    o_hdmi_enc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmi_enc_ctrl.bits.enc_hdmi_val = uenc_hdmi_val;
    HDMITXRegWrite(pu32RegAddr, o_hdmi_enc_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmi_enc_ctrl.u32, "HDMI_ENC_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMI_ENC_CTRL_enc_scr_mdSet
//  Description : Set the value of the member HDMI_ENC_CTRL.enc_scr_md
//  Input       : unsigned int uenc_scr_md: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDMI_ENC_CTRL_enc_scr_mdSet(unsigned int uenc_scr_md)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMI_ENC_CTRL o_hdmi_enc_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->HDMI_ENC_CTRL.u32);
    o_hdmi_enc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmi_enc_ctrl.bits.enc_scr_md = uenc_scr_md;
    HDMITXRegWrite(pu32RegAddr, o_hdmi_enc_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmi_enc_ctrl.u32, "HDMI_ENC_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMI_ENC_CTRL_enc_scr_onSet
//  Description : Set the value of the member HDMI_ENC_CTRL.enc_scr_on
//  Input       : unsigned int uenc_scr_on: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDMI_ENC_CTRL_enc_scr_onSet(unsigned int uenc_scr_on)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMI_ENC_CTRL o_hdmi_enc_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->HDMI_ENC_CTRL.u32);
    o_hdmi_enc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmi_enc_ctrl.bits.enc_scr_on = uenc_scr_on;
    HDMITXRegWrite(pu32RegAddr, o_hdmi_enc_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmi_enc_ctrl.u32, "HDMI_ENC_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDMI_ENC_CTRL_enc_hdmi2_onSet
//  Description : Set the value of the member HDMI_ENC_CTRL.enc_hdmi2_on
//  Input       : unsigned int uenc_hdmi2_on: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDMI_ENC_CTRL_enc_hdmi2_onSet(unsigned int uenc_hdmi2_on)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMI_ENC_CTRL o_hdmi_enc_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->HDMI_ENC_CTRL.u32);
    o_hdmi_enc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdmi_enc_ctrl.bits.enc_hdmi2_on = uenc_hdmi2_on;
    HDMITXRegWrite(pu32RegAddr, o_hdmi_enc_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdmi_enc_ctrl.u32, "HDMI_ENC_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_ENC_CK_SHARP_enc_ck_sharp2Set
//  Description : Set the value of the member ENC_CK_SHARP.enc_ck_sharp2
//  Input       : unsigned int uenc_ck_sharp2: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_ENC_CK_SHARP_enc_ck_sharp2Set(unsigned int uenc_ck_sharp2)
{
    HI_U32 *pu32RegAddr = NULL;
    U_ENC_CK_SHARP o_enc_ck_sharp;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->ENC_CK_SHARP.u32);
    o_enc_ck_sharp.u32 = HDMITXRegRead(pu32RegAddr);
    o_enc_ck_sharp.bits.enc_ck_sharp2 = uenc_ck_sharp2;
    HDMITXRegWrite(pu32RegAddr, o_enc_ck_sharp.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_enc_ck_sharp.u32, "ENC_CK_SHARP");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_ENC_CK_SHARP_enc_ck_sharp1Set
//  Description : Set the value of the member ENC_CK_SHARP.enc_ck_sharp1
//  Input       : unsigned int uenc_ck_sharp1: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_ENC_CK_SHARP_enc_ck_sharp1Set(unsigned int uenc_ck_sharp1)
{
    HI_U32 *pu32RegAddr = NULL;
    U_ENC_CK_SHARP o_enc_ck_sharp;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->ENC_CK_SHARP.u32);
    o_enc_ck_sharp.u32 = HDMITXRegRead(pu32RegAddr);
    o_enc_ck_sharp.bits.enc_ck_sharp1 = uenc_ck_sharp1;
    HDMITXRegWrite(pu32RegAddr, o_enc_ck_sharp.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_enc_ck_sharp.u32, "ENC_CK_SHARP");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_ENC_CK_SHARP_enc_ck_sharp0Set
//  Description : Set the value of the member ENC_CK_SHARP.enc_ck_sharp0
//  Input       : unsigned int uenc_ck_sharp0: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_ENC_CK_SHARP_enc_ck_sharp0Set(unsigned int uenc_ck_sharp0)
{
    HI_U32 *pu32RegAddr = NULL;
    U_ENC_CK_SHARP o_enc_ck_sharp;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->ENC_CK_SHARP.u32);
    o_enc_ck_sharp.u32 = HDMITXRegRead(pu32RegAddr);
    o_enc_ck_sharp.bits.enc_ck_sharp0 = uenc_ck_sharp0;
    HDMITXRegWrite(pu32RegAddr, o_enc_ck_sharp.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_enc_ck_sharp.u32, "ENC_CK_SHARP");

    return HI_SUCCESS;
}


//******************************************************************************
//  Function    : HDMI_TX_PACK_FIFO_CTRL_reg_fifo_delay_cntGet
//  Description : Set the value of the member TX_PACK_FIFO_CTRL.reg_fifo_delay_cnt
//  Input       : unsigned int ureg_fifo_delay_cnt: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_PACK_FIFO_CTRL_reg_fifo_delay_cntGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TX_PACK_FIFO_CTRL o_tx_pack_fifo_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->TX_PACK_FIFO_CTRL.u32);
    o_tx_pack_fifo_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_pack_fifo_ctrl.bits.reg_fifo_delay_cnt;
}

//******************************************************************************
//  Function    : HDMI_TX_PACK_FIFO_CTRL_reg_ext_tmds_paraGet
//  Description : Set the value of the member TX_PACK_FIFO_CTRL.reg_ext_tmds_para
//  Input       : unsigned int ureg_ext_tmds_para: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_PACK_FIFO_CTRL_reg_ext_tmds_paraGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TX_PACK_FIFO_CTRL o_tx_pack_fifo_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->TX_PACK_FIFO_CTRL.u32);
    o_tx_pack_fifo_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_pack_fifo_ctrl.bits.reg_ext_tmds_para;
}

//******************************************************************************
//  Function    : HDMI_TX_PACK_FIFO_CTRL_reg_clock_det_enGet
//  Description : Set the value of the member TX_PACK_FIFO_CTRL.reg_clock_det_en
//  Input       : unsigned int ureg_clock_det_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_PACK_FIFO_CTRL_reg_clock_det_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TX_PACK_FIFO_CTRL o_tx_pack_fifo_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->TX_PACK_FIFO_CTRL.u32);
    o_tx_pack_fifo_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_pack_fifo_ctrl.bits.reg_clock_det_en;
}

//******************************************************************************
//  Function    : HDMI_TX_PACK_FIFO_CTRL_reg_fifo_manu_rstGet
//  Description : Set the value of the member TX_PACK_FIFO_CTRL.reg_fifo_manu_rst
//  Input       : unsigned int ureg_fifo_manu_rst: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_PACK_FIFO_CTRL_reg_fifo_manu_rstGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TX_PACK_FIFO_CTRL o_tx_pack_fifo_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->TX_PACK_FIFO_CTRL.u32);
    o_tx_pack_fifo_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_pack_fifo_ctrl.bits.reg_fifo_manu_rst;
}

//******************************************************************************
//  Function    : HDMI_TX_PACK_FIFO_CTRL_reg_fifo_auto_rst_enGet
//  Description : Set the value of the member TX_PACK_FIFO_CTRL.reg_fifo_auto_rst_en
//  Input       : unsigned int ureg_fifo_auto_rst_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_PACK_FIFO_CTRL_reg_fifo_auto_rst_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TX_PACK_FIFO_CTRL o_tx_pack_fifo_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->TX_PACK_FIFO_CTRL.u32);
    o_tx_pack_fifo_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_pack_fifo_ctrl.bits.reg_fifo_auto_rst_en;
}

//******************************************************************************
//  Function    : HDMI_TX_PACK_FIFO_CTRL_tmds_pack_modeGet
//  Description : Set the value of the member TX_PACK_FIFO_CTRL.tmds_pack_mode
//  Input       : unsigned int utmds_pack_mode: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_PACK_FIFO_CTRL_tmds_pack_modeGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TX_PACK_FIFO_CTRL o_tx_pack_fifo_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->TX_PACK_FIFO_CTRL.u32);
    o_tx_pack_fifo_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_pack_fifo_ctrl.bits.tmds_pack_mode;
}

//******************************************************************************
//  Function    : HDMI_TX_PACK_FIFO_ST_pclk2tclk_stableGet
//  Description : Set the value of the member TX_PACK_FIFO_ST.pclk2tclk_stable
//  Input       : unsigned int upclk2tclk_stable: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_PACK_FIFO_ST_pclk2tclk_stableGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TX_PACK_FIFO_ST o_tx_pack_fifo_st;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->TX_PACK_FIFO_ST.u32);
    o_tx_pack_fifo_st.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_pack_fifo_st.bits.pclk2tclk_stable;
}

//******************************************************************************
//  Function    : HDMI_PCLK_REFER_CNT_reg_pclk_refer_cntGet
//  Description : Set the value of the member PCLK_REFER_CNT.reg_pclk_refer_cnt
//  Input       : unsigned int ureg_pclk_refer_cnt: 18 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PCLK_REFER_CNT_reg_pclk_refer_cntGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PCLK_REFER_CNT o_pclk_refer_cnt;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->PCLK_REFER_CNT.u32);
    o_pclk_refer_cnt.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pclk_refer_cnt.bits.reg_pclk_refer_cnt;
}

//******************************************************************************
//  Function    : HDMI_TCLK_LOWER_THRESHOLD_reg_tcnt_lower_thresholdGet
//  Description : Set the value of the member TCLK_LOWER_THRESHOLD.reg_tcnt_lower_threshold
//  Input       : unsigned int ureg_tcnt_lower_threshold: 18 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TCLK_LOWER_THRESHOLD_reg_tcnt_lower_thresholdGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TCLK_LOWER_THRESHOLD o_tclk_lower_threshold;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->TCLK_LOWER_THRESHOLD.u32);
    o_tclk_lower_threshold.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tclk_lower_threshold.bits.reg_tcnt_lower_threshold;
}

//******************************************************************************
//  Function    : HDMI_TCLK_UPPER_THRESHOLD_reg_tcnt_upper_thresholdGet
//  Description : Set the value of the member TCLK_UPPER_THRESHOLD.reg_tcnt_upper_threshold
//  Input       : unsigned int ureg_tcnt_upper_threshold: 18 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TCLK_UPPER_THRESHOLD_reg_tcnt_upper_thresholdGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TCLK_UPPER_THRESHOLD o_tclk_upper_threshold;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->TCLK_UPPER_THRESHOLD.u32);
    o_tclk_upper_threshold.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tclk_upper_threshold.bits.reg_tcnt_upper_threshold;
}

//******************************************************************************
//  Function    : HDMI_CEA_AVI_CFG_cea_avi_rpt_enGet
//  Description : Set the value of the member CEA_AVI_CFG.cea_avi_rpt_en
//  Input       : unsigned int ucea_avi_rpt_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_AVI_CFG_cea_avi_rpt_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_AVI_CFG o_cea_avi_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_AVI_CFG.u32);
    o_cea_avi_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cea_avi_cfg.bits.cea_avi_rpt_en;
}

//******************************************************************************
//  Function    : HDMI_CEA_AVI_CFG_cea_avi_enGet
//  Description : Set the value of the member CEA_AVI_CFG.cea_avi_en
//  Input       : unsigned int ucea_avi_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_AVI_CFG_cea_avi_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_AVI_CFG o_cea_avi_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_AVI_CFG.u32);
    o_cea_avi_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cea_avi_cfg.bits.cea_avi_en;
}

//******************************************************************************
//  Function    : HDMI_CEA_SPF_CFG_cea_spf_rpt_enGet
//  Description : Set the value of the member CEA_SPF_CFG.cea_spf_rpt_en
//  Input       : unsigned int ucea_spf_rpt_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_SPF_CFG_cea_spf_rpt_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_SPF_CFG o_cea_spf_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_SPF_CFG.u32);
    o_cea_spf_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cea_spf_cfg.bits.cea_spf_rpt_en;
}

//******************************************************************************
//  Function    : HDMI_CEA_SPF_CFG_cea_spf_enGet
//  Description : Set the value of the member CEA_SPF_CFG.cea_spf_en
//  Input       : unsigned int ucea_spf_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_SPF_CFG_cea_spf_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_SPF_CFG o_cea_spf_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_SPF_CFG.u32);
    o_cea_spf_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cea_spf_cfg.bits.cea_spf_en;
}

//******************************************************************************
//  Function    : HDMI_CEA_AUD_CFG_cea_aud_rpt_enGet
//  Description : Set the value of the member CEA_AUD_CFG.cea_aud_rpt_en
//  Input       : unsigned int ucea_aud_rpt_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_AUD_CFG_cea_aud_rpt_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_AUD_CFG o_cea_aud_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_AUD_CFG.u32);
    o_cea_aud_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cea_aud_cfg.bits.cea_aud_rpt_en;
}

//******************************************************************************
//  Function    : HDMI_CEA_AUD_CFG_cea_aud_enGet
//  Description : Set the value of the member CEA_AUD_CFG.cea_aud_en
//  Input       : unsigned int ucea_aud_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_AUD_CFG_cea_aud_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_AUD_CFG o_cea_aud_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_AUD_CFG.u32);
    o_cea_aud_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cea_aud_cfg.bits.cea_aud_en;
}

//******************************************************************************
//  Function    : HDMI_CEA_MPEG_CFG_cea_mpeg_rpt_enGet
//  Description : Set the value of the member CEA_MPEG_CFG.cea_mpeg_rpt_en
//  Input       : unsigned int ucea_mpeg_rpt_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_MPEG_CFG_cea_mpeg_rpt_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_MPEG_CFG o_cea_mpeg_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_MPEG_CFG.u32);
    o_cea_mpeg_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cea_mpeg_cfg.bits.cea_mpeg_rpt_en;
}

//******************************************************************************
//  Function    : HDMI_CEA_MPEG_CFG_cea_mpeg_enGet
//  Description : Set the value of the member CEA_MPEG_CFG.cea_mpeg_en
//  Input       : unsigned int ucea_mpeg_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_MPEG_CFG_cea_mpeg_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_MPEG_CFG o_cea_mpeg_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_MPEG_CFG.u32);
    o_cea_mpeg_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cea_mpeg_cfg.bits.cea_mpeg_en;
}

//******************************************************************************
//  Function    : HDMI_CEA_GEN_CFG_cea_gen_rpt_enGet
//  Description : Set the value of the member CEA_GEN_CFG.cea_gen_rpt_en
//  Input       : unsigned int ucea_gen_rpt_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_GEN_CFG_cea_gen_rpt_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_GEN_CFG o_cea_gen_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_GEN_CFG.u32);
    o_cea_gen_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cea_gen_cfg.bits.cea_gen_rpt_en;
}

//******************************************************************************
//  Function    : HDMI_CEA_GEN_CFG_cea_gen_enGet
//  Description : Set the value of the member CEA_GEN_CFG.cea_gen_en
//  Input       : unsigned int ucea_gen_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_GEN_CFG_cea_gen_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_GEN_CFG o_cea_gen_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_GEN_CFG.u32);
    o_cea_gen_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cea_gen_cfg.bits.cea_gen_en;
}

//******************************************************************************
//  Function    : HDMI_CEA_CP_CFG_cea_cp_rpt_cntGet
//  Description : Set the value of the member CEA_CP_CFG.cea_cp_rpt_en
//  Input       : unsigned int cea_cp_rpt_cnt: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_CP_CFG_cea_cp_rpt_cntGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_CP_CFG o_cea_cp_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_CP_CFG.u32);
    o_cea_cp_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cea_cp_cfg.bits.cea_cp_rpt_cnt;
}


//******************************************************************************
//  Function    : HDMI_CEA_CP_CFG_cea_cp_rpt_enGet
//  Description : Set the value of the member CEA_CP_CFG.cea_cp_rpt_en
//  Input       : unsigned int ucea_cp_rpt_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_CP_CFG_cea_cp_rpt_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_CP_CFG o_cea_cp_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_CP_CFG.u32);
    o_cea_cp_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cea_cp_cfg.bits.cea_cp_rpt_en;
}

//******************************************************************************
//  Function    : HDMI_CEA_CP_CFG_cea_cp_enGet
//  Description : Set the value of the member CEA_CP_CFG.cea_cp_en
//  Input       : unsigned int ucea_cp_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_CP_CFG_cea_cp_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_CP_CFG o_cea_cp_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_CP_CFG.u32);
    o_cea_cp_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cea_cp_cfg.bits.cea_cp_en;
}

//******************************************************************************
//  Function    : HDMI_CEA_GEN2_CFG_cea_gen2_rpt_enGet
//  Description : Set the value of the member CEA_GEN2_CFG.cea_gen2_rpt_en
//  Input       : unsigned int ucea_gen2_rpt_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_GEN2_CFG_cea_gen2_rpt_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_GEN2_CFG o_cea_gen2_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_GEN2_CFG.u32);
    o_cea_gen2_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cea_gen2_cfg.bits.cea_gen2_rpt_en;
}

//******************************************************************************
//  Function    : HDMI_CEA_GEN2_CFG_cea_gen2_enGet
//  Description : Set the value of the member CEA_GEN2_CFG.cea_gen2_en
//  Input       : unsigned int ucea_gen2_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_GEN2_CFG_cea_gen2_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_GEN2_CFG o_cea_gen2_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_GEN2_CFG.u32);
    o_cea_gen2_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cea_gen2_cfg.bits.cea_gen2_en;
}

//******************************************************************************
//  Function    : HDMI_CEA_GEN3_CFG_cea_gen3_rpt_enGet
//  Description : Set the value of the member CEA_GEN3_CFG.cea_gen3_rpt_en
//  Input       : unsigned int ucea_gen3_rpt_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_GEN3_CFG_cea_gen3_rpt_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_GEN3_CFG o_cea_gen3_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_GEN3_CFG.u32);
    o_cea_gen3_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cea_gen3_cfg.bits.cea_gen3_rpt_en;
}

//******************************************************************************
//  Function    : HDMI_CEA_GEN3_CFG_cea_gen3_enGet
//  Description : Set the value of the member CEA_GEN3_CFG.cea_gen3_en
//  Input       : unsigned int ucea_gen3_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_GEN3_CFG_cea_gen3_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_GEN3_CFG o_cea_gen3_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_GEN3_CFG.u32);
    o_cea_gen3_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cea_gen3_cfg.bits.cea_gen3_en;
}

//******************************************************************************
//  Function    : HDMI_CEA_GEN4_CFG_cea_gen4_rpt_enGet
//  Description : Set the value of the member CEA_GEN4_CFG.cea_gen4_rpt_en
//  Input       : unsigned int ucea_gen4_rpt_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_GEN4_CFG_cea_gen4_rpt_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_GEN4_CFG o_cea_gen4_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_GEN4_CFG.u32);
    o_cea_gen4_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cea_gen4_cfg.bits.cea_gen4_rpt_en;
}

//******************************************************************************
//  Function    : HDMI_CEA_GEN4_CFG_cea_gen4_enGet
//  Description : Set the value of the member CEA_GEN4_CFG.cea_gen4_en
//  Input       : unsigned int ucea_gen4_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_GEN4_CFG_cea_gen4_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_GEN4_CFG o_cea_gen4_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_GEN4_CFG.u32);
    o_cea_gen4_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cea_gen4_cfg.bits.cea_gen4_en;
}

//******************************************************************************
//  Function    : HDMI_CEA_GEN5_CFG_cea_gen5_rpt_enGet
//  Description : Set the value of the member CEA_GEN5_CFG.cea_gen5_rpt_en
//  Input       : unsigned int ucea_gen5_rpt_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_GEN5_CFG_cea_gen5_rpt_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_GEN5_CFG o_cea_gen5_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_GEN5_CFG.u32);
    o_cea_gen5_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cea_gen5_cfg.bits.cea_gen5_rpt_en;
}

//******************************************************************************
//  Function    : HDMI_CEA_GEN5_CFG_cea_gen5_enGet
//  Description : Set the value of the member CEA_GEN5_CFG.cea_gen5_en
//  Input       : unsigned int ucea_gen5_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_GEN5_CFG_cea_gen5_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_GEN5_CFG o_cea_gen5_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_GEN5_CFG.u32);
    o_cea_gen5_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cea_gen5_cfg.bits.cea_gen5_en;
}

//******************************************************************************
//  Function    : HDMI_CEA_GAMUT_CFG_cea_gamut_rpt_enGet
//  Description : Set the value of the member CEA_GAMUT_CFG.cea_gamut_rpt_en
//  Input       : unsigned int ucea_gamut_rpt_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_GAMUT_CFG_cea_gamut_rpt_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_GAMUT_CFG o_cea_gamut_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_GAMUT_CFG.u32);
    o_cea_gamut_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cea_gamut_cfg.bits.cea_gamut_rpt_en;
}

//******************************************************************************
//  Function    : HDMI_CEA_GAMUT_CFG_cea_gamut_enGet
//  Description : Set the value of the member CEA_GAMUT_CFG.cea_gamut_en
//  Input       : unsigned int ucea_gamut_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_GAMUT_CFG_cea_gamut_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_GAMUT_CFG o_cea_gamut_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_GAMUT_CFG.u32);
    o_cea_gamut_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cea_gamut_cfg.bits.cea_gamut_en;
}

//******************************************************************************
//  Function    : HDMI_CEA_VSIF_CFG_cea_vsif_rpt_enGet
//  Description : Set the value of the member CEA_VSIF_CFG.cea_vsif_rpt_en
//  Input       : unsigned int ucea_vsif_rpt_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_VSIF_CFG_cea_vsif_rpt_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_VSIF_CFG o_cea_vsif_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_VSIF_CFG.u32);
    o_cea_vsif_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cea_vsif_cfg.bits.cea_vsif_rpt_en;
}

//******************************************************************************
//  Function    : HDMI_CEA_VSIF_CFG_cea_vsif_enGet
//  Description : Set the value of the member CEA_VSIF_CFG.cea_vsif_en
//  Input       : unsigned int ucea_vsif_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_VSIF_CFG_cea_vsif_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_VSIF_CFG o_cea_vsif_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_VSIF_CFG.u32);
    o_cea_vsif_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cea_vsif_cfg.bits.cea_vsif_en;
}

//******************************************************************************
//  Function    : HDMI_CEA_PKT_STATE_cea_vsif_stateGet
//  Description : Set the value of the member CEA_PKT_STATE.cea_vsif_state
//  Input       : unsigned int ucea_vsif_state: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_PKT_STATE_cea_vsif_stateGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_PKT_STATE o_cea_pkt_state;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_PKT_STATE.u32);
    o_cea_pkt_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cea_pkt_state.bits.cea_vsif_state;
}

//******************************************************************************
//  Function    : HDMI_CEA_PKT_STATE_cea_gamut_stateGet
//  Description : Set the value of the member CEA_PKT_STATE.cea_gamut_state
//  Input       : unsigned int ucea_gamut_state: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_PKT_STATE_cea_gamut_stateGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_PKT_STATE o_cea_pkt_state;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_PKT_STATE.u32);
    o_cea_pkt_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cea_pkt_state.bits.cea_gamut_state;
}

//******************************************************************************
//  Function    : HDMI_CEA_PKT_STATE_cea_mpeg_stateGet
//  Description : Set the value of the member CEA_PKT_STATE.cea_mpeg_state
//  Input       : unsigned int ucea_mpeg_state: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_PKT_STATE_cea_mpeg_stateGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_PKT_STATE o_cea_pkt_state;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_PKT_STATE.u32);
    o_cea_pkt_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cea_pkt_state.bits.cea_mpeg_state;
}

//******************************************************************************
//  Function    : HDMI_CEA_PKT_STATE_cea_spd_stateGet
//  Description : Set the value of the member CEA_PKT_STATE.cea_spd_state
//  Input       : unsigned int ucea_spd_state: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_PKT_STATE_cea_spd_stateGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_PKT_STATE o_cea_pkt_state;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_PKT_STATE.u32);
    o_cea_pkt_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cea_pkt_state.bits.cea_spd_state;
}

//******************************************************************************
//  Function    : HDMI_CEA_PKT_STATE_cea_gen5_stateGet
//  Description : Set the value of the member CEA_PKT_STATE.cea_gen5_state
//  Input       : unsigned int ucea_gen5_state: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_PKT_STATE_cea_gen5_stateGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_PKT_STATE o_cea_pkt_state;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_PKT_STATE.u32);
    o_cea_pkt_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cea_pkt_state.bits.cea_gen5_state;
}

//******************************************************************************
//  Function    : HDMI_CEA_PKT_STATE_cea_gen4_stateGet
//  Description : Set the value of the member CEA_PKT_STATE.cea_gen4_state
//  Input       : unsigned int ucea_gen4_state: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_PKT_STATE_cea_gen4_stateGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_PKT_STATE o_cea_pkt_state;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_PKT_STATE.u32);
    o_cea_pkt_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cea_pkt_state.bits.cea_gen4_state;
}

//******************************************************************************
//  Function    : HDMI_CEA_PKT_STATE_cea_gen3_stateGet
//  Description : Set the value of the member CEA_PKT_STATE.cea_gen3_state
//  Input       : unsigned int ucea_gen3_state: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_PKT_STATE_cea_gen3_stateGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_PKT_STATE o_cea_pkt_state;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_PKT_STATE.u32);
    o_cea_pkt_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cea_pkt_state.bits.cea_gen3_state;
}

//******************************************************************************
//  Function    : HDMI_CEA_PKT_STATE_cea_gen2_stateGet
//  Description : Set the value of the member CEA_PKT_STATE.cea_gen2_state
//  Input       : unsigned int ucea_gen2_state: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_PKT_STATE_cea_gen2_stateGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_PKT_STATE o_cea_pkt_state;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_PKT_STATE.u32);
    o_cea_pkt_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cea_pkt_state.bits.cea_gen2_state;
}

//******************************************************************************
//  Function    : HDMI_CEA_PKT_STATE_cea_gen_stateGet
//  Description : Set the value of the member CEA_PKT_STATE.cea_gen_state
//  Input       : unsigned int ucea_gen_state: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_PKT_STATE_cea_gen_stateGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_PKT_STATE o_cea_pkt_state;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_PKT_STATE.u32);
    o_cea_pkt_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cea_pkt_state.bits.cea_gen_state;
}

//******************************************************************************
//  Function    : HDMI_CEA_PKT_STATE_cea_cp_stateGet
//  Description : Set the value of the member CEA_PKT_STATE.cea_cp_state
//  Input       : unsigned int ucea_cp_state: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_PKT_STATE_cea_cp_stateGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_PKT_STATE o_cea_pkt_state;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_PKT_STATE.u32);
    o_cea_pkt_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cea_pkt_state.bits.cea_cp_state;
}

//******************************************************************************
//  Function    : HDMI_CEA_PKT_STATE_cea_aud_stateGet
//  Description : Set the value of the member CEA_PKT_STATE.cea_aud_state
//  Input       : unsigned int ucea_aud_state: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_PKT_STATE_cea_aud_stateGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_PKT_STATE o_cea_pkt_state;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_PKT_STATE.u32);
    o_cea_pkt_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cea_pkt_state.bits.cea_aud_state;
}

//******************************************************************************
//  Function    : HDMI_CEA_PKT_STATE_cea_avi_stateGet
//  Description : Set the value of the member CEA_PKT_STATE.cea_avi_state
//  Input       : unsigned int ucea_avi_state: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CEA_PKT_STATE_cea_avi_stateGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CEA_PKT_STATE o_cea_pkt_state;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CEA_PKT_STATE.u32);
    o_cea_pkt_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cea_pkt_state.bits.cea_avi_state;
}

//******************************************************************************
//  Function    : HDMI_AVMIXER_CONFIG_pkt_bypass_modeGet
//  Description : Set the value of the member AVMIXER_CONFIG.pkt_bypass_mode
//  Input       : unsigned int upkt_bypass_mode: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AVMIXER_CONFIG_pkt_bypass_modeGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVMIXER_CONFIG o_avmixer_config;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVMIXER_CONFIG.u32);
    o_avmixer_config.u32 = HDMITXRegRead(pu32RegAddr);
    return o_avmixer_config.bits.pkt_bypass_mode;
}

//******************************************************************************
//  Function    : HDMI_AVMIXER_CONFIG_pb_priotity_ctlGet
//  Description : Set the value of the member AVMIXER_CONFIG.pb_priotity_ctl
//  Input       : unsigned int upb_priotity_ctl: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AVMIXER_CONFIG_pb_priotity_ctlGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVMIXER_CONFIG o_avmixer_config;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVMIXER_CONFIG.u32);
    o_avmixer_config.u32 = HDMITXRegRead(pu32RegAddr);
    return o_avmixer_config.bits.pb_priotity_ctl;
}

//******************************************************************************
//  Function    : HDMI_AVMIXER_CONFIG_pb_ovr_dc_pkt_enGet
//  Description : Set the value of the member AVMIXER_CONFIG.pb_ovr_dc_pkt_en
//  Input       : unsigned int upb_ovr_dc_pkt_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AVMIXER_CONFIG_pb_ovr_dc_pkt_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVMIXER_CONFIG o_avmixer_config;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVMIXER_CONFIG.u32);
    o_avmixer_config.u32 = HDMITXRegRead(pu32RegAddr);
    return o_avmixer_config.bits.pb_ovr_dc_pkt_en;
}

//******************************************************************************
//  Function    : HDMI_AVMIXER_CONFIG_intr_encryptionGet
//  Description : Set the value of the member AVMIXER_CONFIG.intr_encryption
//  Input       : unsigned int uintr_encryption: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AVMIXER_CONFIG_intr_encryptionGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVMIXER_CONFIG o_avmixer_config;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVMIXER_CONFIG.u32);
    o_avmixer_config.u32 = HDMITXRegRead(pu32RegAddr);
    return o_avmixer_config.bits.intr_encryption;
}

//******************************************************************************
//  Function    : HDMI_AVMIXER_CONFIG_null_pkt_en_vs_highGet
//  Description : Set the value of the member AVMIXER_CONFIG.null_pkt_en_vs_high
//  Input       : unsigned int unull_pkt_en_vs_high: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AVMIXER_CONFIG_null_pkt_en_vs_highGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVMIXER_CONFIG o_avmixer_config;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVMIXER_CONFIG.u32);
    o_avmixer_config.u32 = HDMITXRegRead(pu32RegAddr);
    return o_avmixer_config.bits.null_pkt_en_vs_high;
}

//******************************************************************************
//  Function    : HDMI_AVMIXER_CONFIG_null_pkt_enGet
//  Description : Set the value of the member AVMIXER_CONFIG.null_pkt_en
//  Input       : unsigned int unull_pkt_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AVMIXER_CONFIG_null_pkt_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVMIXER_CONFIG o_avmixer_config;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVMIXER_CONFIG.u32);
    o_avmixer_config.u32 = HDMITXRegRead(pu32RegAddr);
    return o_avmixer_config.bits.null_pkt_en;
}

//******************************************************************************
//  Function    : HDMI_AVMIXER_CONFIG_dc_pkt_enGet
//  Description : Set the value of the member AVMIXER_CONFIG.dc_pkt_en
//  Input       : unsigned int udc_pkt_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AVMIXER_CONFIG_dc_pkt_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVMIXER_CONFIG o_avmixer_config;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVMIXER_CONFIG.u32);
    o_avmixer_config.u32 = HDMITXRegRead(pu32RegAddr);
    return o_avmixer_config.bits.dc_pkt_en;
}

//******************************************************************************
//  Function    : HDMI_AVMIXER_CONFIG_hdmi_modeGet
//  Description : Set the value of the member AVMIXER_CONFIG.hdmi_mode
//  Input       : unsigned int uhdmi_mode: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AVMIXER_CONFIG_hdmi_modeGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AVMIXER_CONFIG o_avmixer_config;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->AVMIXER_CONFIG.u32);
    o_avmixer_config.u32 = HDMITXRegRead(pu32RegAddr);
    return o_avmixer_config.bits.hdmi_mode;
}

//******************************************************************************
//  Function    : HDMI_CP_PKT_AVMUTE_cp_clr_avmuteGet
//  Description : Set the value of the member CP_PKT_AVMUTE.cp_clr_avmute
//  Input       : unsigned int ucp_clr_avmute: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CP_PKT_AVMUTE_cp_clr_avmuteGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CP_PKT_AVMUTE o_cp_pkt_avmute;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CP_PKT_AVMUTE.u32);
    o_cp_pkt_avmute.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cp_pkt_avmute.bits.cp_clr_avmute;
}

//******************************************************************************
//  Function    : HDMI_CP_PKT_AVMUTE_cp_set_avmuteGet
//  Description : Set the value of the member CP_PKT_AVMUTE.cp_set_avmute
//  Input       : unsigned int ucp_set_avmute: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_CP_PKT_AVMUTE_cp_set_avmuteGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_CP_PKT_AVMUTE o_cp_pkt_avmute;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->CP_PKT_AVMUTE.u32);
    o_cp_pkt_avmute.u32 = HDMITXRegRead(pu32RegAddr);
    return o_cp_pkt_avmute.bits.cp_set_avmute;
}

//******************************************************************************
//  Function    : HDMI_VIDEO_BLANK_CFG_video_blankGet
//  Description : Set the value of the member VIDEO_BLANK_CFG.video_blank
//  Input       : unsigned int uvideo_blank: 24 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDEO_BLANK_CFG_video_blankGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VIDEO_BLANK_CFG o_video_blank_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->VIDEO_BLANK_CFG.u32);
    o_video_blank_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_video_blank_cfg.bits.video_blank;
}

//******************************************************************************
//  Function    : HDMI_TMDS_BIST_CTRL_reg_tbist_patt_selGet
//  Description : Set the value of the member TMDS_BIST_CTRL.reg_tbist_patt_sel
//  Input       : unsigned int ureg_tbist_patt_sel: 5 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TMDS_BIST_CTRL_reg_tbist_patt_selGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TMDS_BIST_CTRL o_tmds_bist_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->TMDS_BIST_CTRL.u32);
    o_tmds_bist_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tmds_bist_ctrl.bits.reg_tbist_patt_sel;
}

//******************************************************************************
//  Function    : HDMI_TMDS_BIST_CTRL_reg_tbist_timing_selGet
//  Description : Set the value of the member TMDS_BIST_CTRL.reg_tbist_timing_sel
//  Input       : unsigned int ureg_tbist_timing_sel: 6 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TMDS_BIST_CTRL_reg_tbist_timing_selGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TMDS_BIST_CTRL o_tmds_bist_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->TMDS_BIST_CTRL.u32);
    o_tmds_bist_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tmds_bist_ctrl.bits.reg_tbist_timing_sel;
}

//******************************************************************************
//  Function    : HDMI_TMDS_BIST_CTRL_reg_tbist_syn_polGet
//  Description : Set the value of the member TMDS_BIST_CTRL.reg_tbist_syn_pol
//  Input       : unsigned int ureg_tbist_syn_pol: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TMDS_BIST_CTRL_reg_tbist_syn_polGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TMDS_BIST_CTRL o_tmds_bist_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->TMDS_BIST_CTRL.u32);
    o_tmds_bist_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tmds_bist_ctrl.bits.reg_tbist_syn_pol;
}

//******************************************************************************
//  Function    : HDMI_TMDS_BIST_CTRL_reg_tbist_enGet
//  Description : Set the value of the member TMDS_BIST_CTRL.reg_tbist_en
//  Input       : unsigned int ureg_tbist_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TMDS_BIST_CTRL_reg_tbist_enGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TMDS_BIST_CTRL o_tmds_bist_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->TMDS_BIST_CTRL.u32);
    o_tmds_bist_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tmds_bist_ctrl.bits.reg_tbist_en;
}

//******************************************************************************
//  Function    : HDMI_HDMI_ENC_CTRL_enc_ck_div_selGet
//  Description : Set the value of the member HDMI_ENC_CTRL.enc_ck_div_sel
//  Input       : unsigned int uenc_ck_div_sel: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDMI_ENC_CTRL_enc_ck_div_selGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMI_ENC_CTRL o_hdmi_enc_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->HDMI_ENC_CTRL.u32);
    o_hdmi_enc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmi_enc_ctrl.bits.enc_ck_div_sel;
}

//******************************************************************************
//  Function    : HDMI_HDMI_ENC_CTRL_enc_bypassGet
//  Description : Set the value of the member HDMI_ENC_CTRL.enc_bypass
//  Input       : unsigned int uenc_bypass: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDMI_ENC_CTRL_enc_bypassGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMI_ENC_CTRL o_hdmi_enc_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->HDMI_ENC_CTRL.u32);
    o_hdmi_enc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmi_enc_ctrl.bits.enc_bypass;
}

//******************************************************************************
//  Function    : HDMI_HDMI_ENC_CTRL_enc_hdmi_ovrGet
//  Description : Set the value of the member HDMI_ENC_CTRL.enc_hdmi_ovr
//  Input       : unsigned int uenc_hdmi_ovr: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDMI_ENC_CTRL_enc_hdmi_ovrGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMI_ENC_CTRL o_hdmi_enc_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->HDMI_ENC_CTRL.u32);
    o_hdmi_enc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmi_enc_ctrl.bits.enc_hdmi_ovr;
}

//******************************************************************************
//  Function    : HDMI_HDMI_ENC_CTRL_enc_hdmi_valGet
//  Description : Set the value of the member HDMI_ENC_CTRL.enc_hdmi_val
//  Input       : unsigned int uenc_hdmi_val: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDMI_ENC_CTRL_enc_hdmi_valGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMI_ENC_CTRL o_hdmi_enc_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->HDMI_ENC_CTRL.u32);
    o_hdmi_enc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmi_enc_ctrl.bits.enc_hdmi_val;
}

//******************************************************************************
//  Function    : HDMI_HDMI_ENC_CTRL_enc_scr_mdGet
//  Description : Set the value of the member HDMI_ENC_CTRL.enc_scr_md
//  Input       : unsigned int uenc_scr_md: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDMI_ENC_CTRL_enc_scr_mdGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMI_ENC_CTRL o_hdmi_enc_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->HDMI_ENC_CTRL.u32);
    o_hdmi_enc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmi_enc_ctrl.bits.enc_scr_md;
}

//******************************************************************************
//  Function    : HDMI_HDMI_ENC_CTRL_enc_scr_onGet
//  Description : Set the value of the member HDMI_ENC_CTRL.enc_scr_on
//  Input       : unsigned int uenc_scr_on: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDMI_ENC_CTRL_enc_scr_onGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMI_ENC_CTRL o_hdmi_enc_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->HDMI_ENC_CTRL.u32);
    o_hdmi_enc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmi_enc_ctrl.bits.enc_scr_on;
}

//******************************************************************************
//  Function    : HDMI_HDMI_ENC_CTRL_enc_hdmi2_onGet
//  Description : Set the value of the member HDMI_ENC_CTRL.enc_hdmi2_on
//  Input       : unsigned int uenc_hdmi2_on: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDMI_ENC_CTRL_enc_hdmi2_onGet(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDMI_ENC_CTRL o_hdmi_enc_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->HDMI_ENC_CTRL.u32);
    o_hdmi_enc_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdmi_enc_ctrl.bits.enc_hdmi2_on;
}

//******************************************************************************
//  Function    : HDMI_ENC_CK_SHARP_enc_ck_sharp2Get
//  Description : Set the value of the member ENC_CK_SHARP.enc_ck_sharp2
//  Input       : unsigned int uenc_ck_sharp2: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_ENC_CK_SHARP_enc_ck_sharp2Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_ENC_CK_SHARP o_enc_ck_sharp;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->ENC_CK_SHARP.u32);
    o_enc_ck_sharp.u32 = HDMITXRegRead(pu32RegAddr);
    return o_enc_ck_sharp.bits.enc_ck_sharp2;
}

//******************************************************************************
//  Function    : HDMI_ENC_CK_SHARP_enc_ck_sharp1Get
//  Description : Set the value of the member ENC_CK_SHARP.enc_ck_sharp1
//  Input       : unsigned int uenc_ck_sharp1: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_ENC_CK_SHARP_enc_ck_sharp1Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_ENC_CK_SHARP o_enc_ck_sharp;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->ENC_CK_SHARP.u32);
    o_enc_ck_sharp.u32 = HDMITXRegRead(pu32RegAddr);
    return o_enc_ck_sharp.bits.enc_ck_sharp1;
}

//******************************************************************************
//  Function    : HDMI_ENC_CK_SHARP_enc_ck_sharp0Get
//  Description : Set the value of the member ENC_CK_SHARP.enc_ck_sharp0
//  Input       : unsigned int uenc_ck_sharp0: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_ENC_CK_SHARP_enc_ck_sharp0Get(HI_VOID)
{
    HI_U32 *pu32RegAddr = NULL;
    U_ENC_CK_SHARP o_enc_ck_sharp;
    pu32RegAddr = (HI_U32*)&(goptx_hdmi_regAllReg->ENC_CK_SHARP.u32);
    o_enc_ck_sharp.u32 = HDMITXRegRead(pu32RegAddr);
    return o_enc_ck_sharp.bits.enc_ck_sharp0;
}
