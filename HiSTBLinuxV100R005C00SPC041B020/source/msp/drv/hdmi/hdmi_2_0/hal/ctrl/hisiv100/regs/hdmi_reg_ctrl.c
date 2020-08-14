// ****************************************************************************** 
// Copyright     :  Copyright (C) 2015, Hisilicon Technologies Co. Ltd.
// File name     :  tx_ctrl_reg_c_union_define.c
// Project line  :  Platform And Key Technologies Development
// Department    :  CAD Development Department
// Author        :  w00271848
// Version       :  1.0
// Date          :  Tester
// Description   :  The description of xxx project
// Others        :  Generated automatically by nManager V4.0.2.5 
// History       :  w00271848 2015/06/03 15:42:04 Create file
// ******************************************************************************


#include "hi_type.h"
#include "hdmi_reg_ctrl.h"
#include "hdmi_product_define.h"

#include "hdmi_platform.h"
#ifndef HDMI_BUILD_IN_BOOT
#include "drv_hdmi_platform.h"
#endif


/* Define the struct pointor of the module tx_ctrl_reg */
volatile S_tx_ctrl_reg_REGS_TYPE *goptx_ctrl_regAllReg = NULL;


int HDMI_TX_S_tx_ctrl_reg_REGS_TYPE_Init(void)
{
    if (goptx_ctrl_regAllReg)
    {
        return HI_SUCCESS;
    }

    goptx_ctrl_regAllReg        = (volatile S_tx_ctrl_reg_REGS_TYPE*)HDMI_IO_MAP(BASE_ADDR_tx_ctrl_reg,4);

    if (!goptx_ctrl_regAllReg)
    {
        HDMI_ERR("goptx_ctrl_regAllReg = null,fail!\n");
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;
}

int HDMI_TX_S_tx_ctrl_reg_REGS_TYPE_DeInit(void)
{
    if (goptx_ctrl_regAllReg)
    {
        HDMI_IO_UNMAP(goptx_ctrl_regAllReg);
        goptx_ctrl_regAllReg = HI_NULL;
    }
    
    return HI_SUCCESS;
}


//******************************************************************************
//  Function    : HDMI_TX_PWD_RST_CTRL_tx_mcu_srst_reqSet
//  Description : Set the value of the member TX_PWD_RST_CTRL.tx_mcu_srst_req
//  Input       : unsigned int utx_mcu_srst_req: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_PWD_RST_CTRL_tx_mcu_srst_reqSet(unsigned int utx_mcu_srst_req)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TX_PWD_RST_CTRL o_tx_pwd_rst_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TX_PWD_RST_CTRL.u32);
    o_tx_pwd_rst_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_pwd_rst_ctrl.bits.tx_mcu_srst_req = utx_mcu_srst_req;
    HDMITXRegWrite(pu32RegAddr, o_tx_pwd_rst_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_pwd_rst_ctrl.u32, "TX_PWD_RST_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_PWD_RST_CTRL_tx_hdcp2x_srst_reqSet
//  Description : Set the value of the member TX_PWD_RST_CTRL.tx_hdcp2x_srst_req
//  Input       : unsigned int utx_hdcp2x_srst_req: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_PWD_RST_CTRL_tx_hdcp2x_srst_reqSet(unsigned int utx_hdcp2x_srst_req)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TX_PWD_RST_CTRL o_tx_pwd_rst_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TX_PWD_RST_CTRL.u32);
    o_tx_pwd_rst_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_pwd_rst_ctrl.bits.tx_hdcp2x_srst_req = utx_hdcp2x_srst_req;
    HDMITXRegWrite(pu32RegAddr, o_tx_pwd_rst_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_pwd_rst_ctrl.u32, "TX_PWD_RST_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_PWD_RST_CTRL_tx_afifo_srst_reqSet
//  Description : Set the value of the member TX_PWD_RST_CTRL.tx_afifo_srst_req
//  Input       : unsigned int utx_afifo_srst_req: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_PWD_RST_CTRL_tx_afifo_srst_reqSet(unsigned int utx_afifo_srst_req)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TX_PWD_RST_CTRL o_tx_pwd_rst_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TX_PWD_RST_CTRL.u32);
    o_tx_pwd_rst_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_pwd_rst_ctrl.bits.tx_afifo_srst_req = utx_afifo_srst_req;
    HDMITXRegWrite(pu32RegAddr, o_tx_pwd_rst_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_pwd_rst_ctrl.u32, "TX_PWD_RST_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_PWD_RST_CTRL_tx_acr_srst_reqSet
//  Description : Set the value of the member TX_PWD_RST_CTRL.tx_acr_srst_req
//  Input       : unsigned int utx_acr_srst_req: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_PWD_RST_CTRL_tx_acr_srst_reqSet(unsigned int utx_acr_srst_req)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TX_PWD_RST_CTRL o_tx_pwd_rst_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TX_PWD_RST_CTRL.u32);
    o_tx_pwd_rst_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_pwd_rst_ctrl.bits.tx_acr_srst_req = utx_acr_srst_req;
    HDMITXRegWrite(pu32RegAddr, o_tx_pwd_rst_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_pwd_rst_ctrl.u32, "TX_PWD_RST_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_PWD_RST_CTRL_tx_aud_srst_reqSet
//  Description : Set the value of the member TX_PWD_RST_CTRL.tx_aud_srst_req
//  Input       : unsigned int utx_aud_srst_req: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_PWD_RST_CTRL_tx_aud_srst_reqSet(unsigned int utx_aud_srst_req)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TX_PWD_RST_CTRL o_tx_pwd_rst_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TX_PWD_RST_CTRL.u32);
    o_tx_pwd_rst_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_pwd_rst_ctrl.bits.tx_aud_srst_req = utx_aud_srst_req;
    HDMITXRegWrite(pu32RegAddr, o_tx_pwd_rst_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_pwd_rst_ctrl.u32, "TX_PWD_RST_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_PWD_RST_CTRL_tx_phy_srst_reqSet
//  Description : Set the value of the member TX_PWD_RST_CTRL.tx_phy_srst_req
//  Input       : unsigned int utx_phy_srst_req: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_PWD_RST_CTRL_tx_phy_srst_reqSet(unsigned int utx_phy_srst_req)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TX_PWD_RST_CTRL o_tx_pwd_rst_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TX_PWD_RST_CTRL.u32);
    o_tx_pwd_rst_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_pwd_rst_ctrl.bits.tx_phy_srst_req = utx_phy_srst_req;
    HDMITXRegWrite(pu32RegAddr, o_tx_pwd_rst_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_pwd_rst_ctrl.u32, "TX_PWD_RST_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_PWD_RST_CTRL_tx_hdcp1x_srst_reqSet
//  Description : Set the value of the member TX_PWD_RST_CTRL.tx_hdcp1x_srst_req
//  Input       : unsigned int utx_hdcp1x_srst_req: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_PWD_RST_CTRL_tx_hdcp1x_srst_reqSet(unsigned int utx_hdcp1x_srst_req)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TX_PWD_RST_CTRL o_tx_pwd_rst_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TX_PWD_RST_CTRL.u32);
    o_tx_pwd_rst_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_pwd_rst_ctrl.bits.tx_hdcp1x_srst_req = utx_hdcp1x_srst_req;
    HDMITXRegWrite(pu32RegAddr, o_tx_pwd_rst_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_pwd_rst_ctrl.u32, "TX_PWD_RST_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_PWD_RST_CTRL_tx_hdmi_srst_reqSet
//  Description : Set the value of the member TX_PWD_RST_CTRL.tx_hdmi_srst_req
//  Input       : unsigned int utx_hdmi_srst_req: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_PWD_RST_CTRL_tx_hdmi_srst_reqSet(unsigned int utx_hdmi_srst_req)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TX_PWD_RST_CTRL o_tx_pwd_rst_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TX_PWD_RST_CTRL.u32);
    o_tx_pwd_rst_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_pwd_rst_ctrl.bits.tx_hdmi_srst_req = utx_hdmi_srst_req;
    HDMITXRegWrite(pu32RegAddr, o_tx_pwd_rst_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_pwd_rst_ctrl.u32, "TX_PWD_RST_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_PWD_RST_CTRL_tx_vid_srst_reqSet
//  Description : Set the value of the member TX_PWD_RST_CTRL.tx_vid_srst_req
//  Input       : unsigned int utx_vid_srst_req: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_PWD_RST_CTRL_tx_vid_srst_reqSet(unsigned int utx_vid_srst_req)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TX_PWD_RST_CTRL o_tx_pwd_rst_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TX_PWD_RST_CTRL.u32);
    o_tx_pwd_rst_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_pwd_rst_ctrl.bits.tx_vid_srst_req = utx_vid_srst_req;
    HDMITXRegWrite(pu32RegAddr, o_tx_pwd_rst_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_pwd_rst_ctrl.u32, "TX_PWD_RST_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_PWD_RST_CTRL_tx_sys_srst_reqSet
//  Description : Set the value of the member TX_PWD_RST_CTRL.tx_sys_srst_req
//  Input       : unsigned int utx_sys_srst_req: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_PWD_RST_CTRL_tx_sys_srst_reqSet(unsigned int utx_sys_srst_req)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TX_PWD_RST_CTRL o_tx_pwd_rst_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TX_PWD_RST_CTRL.u32);
    o_tx_pwd_rst_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_pwd_rst_ctrl.bits.tx_sys_srst_req = utx_sys_srst_req;
    HDMITXRegWrite(pu32RegAddr, o_tx_pwd_rst_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_pwd_rst_ctrl.u32, "TX_PWD_RST_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_PWD_RST_CTRL_tx_pwd_srst_reqSet
//  Description : Set the value of the member TX_PWD_RST_CTRL.tx_pwd_srst_req
//  Input       : unsigned int utx_pwd_srst_req: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_PWD_RST_CTRL_tx_pwd_srst_reqSet(unsigned int utx_pwd_srst_req)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TX_PWD_RST_CTRL o_tx_pwd_rst_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TX_PWD_RST_CTRL.u32);
    o_tx_pwd_rst_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_pwd_rst_ctrl.bits.tx_pwd_srst_req = utx_pwd_srst_req;
    HDMITXRegWrite(pu32RegAddr, o_tx_pwd_rst_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_pwd_rst_ctrl.u32, "TX_PWD_RST_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_SCDC_FSM_CTRL_scdc_stall_reqSet
//  Description : Set the value of the member SCDC_FSM_CTRL.scdc_stall_req
//  Input       : unsigned int uscdc_stall_req: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_SCDC_FSM_CTRL_scdc_stall_reqSet(unsigned int uscdc_stall_req)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SCDC_FSM_CTRL o_scdc_fsm_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->SCDC_FSM_CTRL.u32);
    o_scdc_fsm_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_scdc_fsm_ctrl.bits.scdc_stall_req = uscdc_stall_req;
    HDMITXRegWrite(pu32RegAddr, o_scdc_fsm_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_scdc_fsm_ctrl.u32, "SCDC_FSM_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_SCDC_FSM_CTRL_scdc_hdcp_det_enSet
//  Description : Set the value of the member SCDC_FSM_CTRL.scdc_hdcp_det_en
//  Input       : unsigned int uscdc_hdcp_det_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_SCDC_FSM_CTRL_scdc_hdcp_det_enSet(unsigned int uscdc_hdcp_det_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SCDC_FSM_CTRL o_scdc_fsm_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->SCDC_FSM_CTRL.u32);
    o_scdc_fsm_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_scdc_fsm_ctrl.bits.scdc_hdcp_det_en = uscdc_hdcp_det_en;
    HDMITXRegWrite(pu32RegAddr, o_scdc_fsm_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_scdc_fsm_ctrl.u32, "SCDC_FSM_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_SCDC_FSM_CTRL_scdc_poll_selSet
//  Description : Set the value of the member SCDC_FSM_CTRL.scdc_poll_sel
//  Input       : unsigned int uscdc_poll_sel: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_SCDC_FSM_CTRL_scdc_poll_selSet(unsigned int uscdc_poll_sel)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SCDC_FSM_CTRL o_scdc_fsm_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->SCDC_FSM_CTRL.u32);
    o_scdc_fsm_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_scdc_fsm_ctrl.bits.scdc_poll_sel = uscdc_poll_sel;
    HDMITXRegWrite(pu32RegAddr, o_scdc_fsm_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_scdc_fsm_ctrl.u32, "SCDC_FSM_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_SCDC_FSM_CTRL_scdc_auto_reply_stopSet
//  Description : Set the value of the member SCDC_FSM_CTRL.scdc_auto_reply_stop
//  Input       : unsigned int uscdc_auto_reply_stop: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_SCDC_FSM_CTRL_scdc_auto_reply_stopSet(unsigned int uscdc_auto_reply_stop)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SCDC_FSM_CTRL o_scdc_fsm_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->SCDC_FSM_CTRL.u32);
    o_scdc_fsm_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_scdc_fsm_ctrl.bits.scdc_auto_reply_stop = uscdc_auto_reply_stop;
    HDMITXRegWrite(pu32RegAddr, o_scdc_fsm_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_scdc_fsm_ctrl.u32, "SCDC_FSM_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_SCDC_FSM_CTRL_scdc_auto_pollSet
//  Description : Set the value of the member SCDC_FSM_CTRL.scdc_auto_poll
//  Input       : unsigned int uscdc_auto_poll: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_SCDC_FSM_CTRL_scdc_auto_pollSet(unsigned int uscdc_auto_poll)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SCDC_FSM_CTRL o_scdc_fsm_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->SCDC_FSM_CTRL.u32);
    o_scdc_fsm_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_scdc_fsm_ctrl.bits.scdc_auto_poll = uscdc_auto_poll;
    HDMITXRegWrite(pu32RegAddr, o_scdc_fsm_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_scdc_fsm_ctrl.u32, "SCDC_FSM_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_SCDC_FSM_CTRL_scdc_auto_replySet
//  Description : Set the value of the member SCDC_FSM_CTRL.scdc_auto_reply
//  Input       : unsigned int uscdc_auto_reply: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_SCDC_FSM_CTRL_scdc_auto_replySet(unsigned int uscdc_auto_reply)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SCDC_FSM_CTRL o_scdc_fsm_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->SCDC_FSM_CTRL.u32);
    o_scdc_fsm_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_scdc_fsm_ctrl.bits.scdc_auto_reply = uscdc_auto_reply;
    HDMITXRegWrite(pu32RegAddr, o_scdc_fsm_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_scdc_fsm_ctrl.u32, "SCDC_FSM_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_SCDC_FSM_CTRL_scdc_access_enSet
//  Description : Set the value of the member SCDC_FSM_CTRL.scdc_access_en
//  Input       : unsigned int uscdc_access_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_SCDC_FSM_CTRL_scdc_access_enSet(unsigned int uscdc_access_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SCDC_FSM_CTRL o_scdc_fsm_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->SCDC_FSM_CTRL.u32);
    o_scdc_fsm_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_scdc_fsm_ctrl.bits.scdc_access_en = uscdc_access_en;
    HDMITXRegWrite(pu32RegAddr, o_scdc_fsm_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_scdc_fsm_ctrl.u32, "SCDC_FSM_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_SCDC_FSM_CTRL_scdc_ddcm_abortSet
//  Description : Set the value of the member SCDC_FSM_CTRL.scdc_ddcm_abort
//  Input       : unsigned int uscdc_ddcm_abort: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_SCDC_FSM_CTRL_scdc_ddcm_abortSet(unsigned int uscdc_ddcm_abort)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SCDC_FSM_CTRL o_scdc_fsm_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->SCDC_FSM_CTRL.u32);
    o_scdc_fsm_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_scdc_fsm_ctrl.bits.scdc_ddcm_abort = uscdc_ddcm_abort;
    HDMITXRegWrite(pu32RegAddr, o_scdc_fsm_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_scdc_fsm_ctrl.u32, "SCDC_FSM_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_SCDC_POLL_TIMER_scdc_poll_timerSet
//  Description : Set the value of the member SCDC_POLL_TIMER.scdc_poll_timer
//  Input       : unsigned int uscdc_poll_timer: 22 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_SCDC_POLL_TIMER_scdc_poll_timerSet(unsigned int uscdc_poll_timer)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SCDC_POLL_TIMER o_scdc_poll_timer;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->SCDC_POLL_TIMER.u32);
    o_scdc_poll_timer.u32 = HDMITXRegRead(pu32RegAddr);
    o_scdc_poll_timer.bits.scdc_poll_timer = uscdc_poll_timer;
    HDMITXRegWrite(pu32RegAddr, o_scdc_poll_timer.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_scdc_poll_timer.u32, "SCDC_POLL_TIMER");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_SCDC_FSM_STATE_scdc_rreq_in_progSet
//  Description : Set the value of the member SCDC_FSM_STATE.scdc_rreq_in_prog
//  Input       : unsigned int uscdc_rreq_in_prog: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_SCDC_FSM_STATE_scdc_rreq_in_progSet(unsigned int uscdc_rreq_in_prog)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SCDC_FSM_STATE o_scdc_fsm_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->SCDC_FSM_STATE.u32);
    o_scdc_fsm_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_scdc_fsm_state.bits.scdc_rreq_in_prog = uscdc_rreq_in_prog;
    HDMITXRegWrite(pu32RegAddr, o_scdc_fsm_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_scdc_fsm_state.u32, "SCDC_FSM_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_SCDC_FSM_STATE_scdc_in_progSet
//  Description : Set the value of the member SCDC_FSM_STATE.scdc_in_prog
//  Input       : unsigned int uscdc_in_prog: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_SCDC_FSM_STATE_scdc_in_progSet(unsigned int uscdc_in_prog)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SCDC_FSM_STATE o_scdc_fsm_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->SCDC_FSM_STATE.u32);
    o_scdc_fsm_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_scdc_fsm_state.bits.scdc_in_prog = uscdc_in_prog;
    HDMITXRegWrite(pu32RegAddr, o_scdc_fsm_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_scdc_fsm_state.u32, "SCDC_FSM_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_SCDC_FSM_STATE_scdc_activeSet
//  Description : Set the value of the member SCDC_FSM_STATE.scdc_active
//  Input       : unsigned int uscdc_active: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_SCDC_FSM_STATE_scdc_activeSet(unsigned int uscdc_active)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SCDC_FSM_STATE o_scdc_fsm_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->SCDC_FSM_STATE.u32);
    o_scdc_fsm_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_scdc_fsm_state.bits.scdc_active = uscdc_active;
    HDMITXRegWrite(pu32RegAddr, o_scdc_fsm_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_scdc_fsm_state.u32, "SCDC_FSM_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_SCDC_FSM_STATE_scdc_rreq_stateSet
//  Description : Set the value of the member SCDC_FSM_STATE.scdc_rreq_state
//  Input       : unsigned int uscdc_rreq_state: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_SCDC_FSM_STATE_scdc_rreq_stateSet(unsigned int uscdc_rreq_state)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SCDC_FSM_STATE o_scdc_fsm_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->SCDC_FSM_STATE.u32);
    o_scdc_fsm_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_scdc_fsm_state.bits.scdc_rreq_state = uscdc_rreq_state;
    HDMITXRegWrite(pu32RegAddr, o_scdc_fsm_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_scdc_fsm_state.u32, "SCDC_FSM_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_SCDC_FSM_STATE_scdc_fsm_stateSet
//  Description : Set the value of the member SCDC_FSM_STATE.scdc_fsm_state
//  Input       : unsigned int uscdc_fsm_state: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_SCDC_FSM_STATE_scdc_fsm_stateSet(unsigned int uscdc_fsm_state)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SCDC_FSM_STATE o_scdc_fsm_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->SCDC_FSM_STATE.u32);
    o_scdc_fsm_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_scdc_fsm_state.bits.scdc_fsm_state = uscdc_fsm_state;
    HDMITXRegWrite(pu32RegAddr, o_scdc_fsm_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_scdc_fsm_state.u32, "SCDC_FSM_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_SCDC_FLAG_BTYE_scdc_flag_byte1Set
//  Description : Set the value of the member SCDC_FLAG_BTYE.scdc_flag_byte1
//  Input       : unsigned int uscdc_flag_byte1: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_SCDC_FLAG_BTYE_scdc_flag_byte1Set(unsigned int uscdc_flag_byte1)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SCDC_FLAG_BTYE o_scdc_flag_btye;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->SCDC_FLAG_BTYE.u32);
    o_scdc_flag_btye.u32 = HDMITXRegRead(pu32RegAddr);
    o_scdc_flag_btye.bits.scdc_flag_byte1 = uscdc_flag_byte1;
    HDMITXRegWrite(pu32RegAddr, o_scdc_flag_btye.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_scdc_flag_btye.u32, "SCDC_FLAG_BTYE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_SCDC_FLAG_BTYE_scdc_flag_byte0Set
//  Description : Set the value of the member SCDC_FLAG_BTYE.scdc_flag_byte0
//  Input       : unsigned int uscdc_flag_byte0: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_SCDC_FLAG_BTYE_scdc_flag_byte0Set(unsigned int uscdc_flag_byte0)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SCDC_FLAG_BTYE o_scdc_flag_btye;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->SCDC_FLAG_BTYE.u32);
    o_scdc_flag_btye.u32 = HDMITXRegRead(pu32RegAddr);
    o_scdc_flag_btye.bits.scdc_flag_byte0 = uscdc_flag_byte0;
    HDMITXRegWrite(pu32RegAddr, o_scdc_flag_btye.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_scdc_flag_btye.u32, "SCDC_FLAG_BTYE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PWD_FIFO_RDATA_pwd_fifo_data_outSet
//  Description : Set the value of the member PWD_FIFO_RDATA.pwd_fifo_data_out
//  Input       : unsigned int upwd_fifo_data_out: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_FIFO_RDATA_pwd_fifo_data_outSet(unsigned int upwd_fifo_data_out)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_FIFO_RDATA o_pwd_fifo_rdata;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_FIFO_RDATA.u32);
    o_pwd_fifo_rdata.u32 = HDMITXRegRead(pu32RegAddr);
    o_pwd_fifo_rdata.bits.pwd_fifo_data_out = upwd_fifo_data_out;
    HDMITXRegWrite(pu32RegAddr, o_pwd_fifo_rdata.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pwd_fifo_rdata.u32, "PWD_FIFO_RDATA");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PWD_FIFO_WDATA_pwd_fifo_data_inSet
//  Description : Set the value of the member PWD_FIFO_WDATA.pwd_fifo_data_in
//  Input       : unsigned int upwd_fifo_data_in: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_FIFO_WDATA_pwd_fifo_data_inSet(unsigned int upwd_fifo_data_in)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_FIFO_WDATA o_pwd_fifo_wdata;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_FIFO_WDATA.u32);
    o_pwd_fifo_wdata.u32 = HDMITXRegRead(pu32RegAddr);
    o_pwd_fifo_wdata.bits.pwd_fifo_data_in = upwd_fifo_data_in;
    HDMITXRegWrite(pu32RegAddr, o_pwd_fifo_wdata.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pwd_fifo_wdata.u32, "PWD_FIFO_WDATA");


    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PWD_DATA_CNT_pwd_data_out_cntSet
//  Description : Set the value of the member PWD_DATA_CNT.pwd_data_out_cnt
//  Input       : unsigned int upwd_data_out_cnt: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_DATA_CNT_pwd_data_out_cntSet(unsigned int upwd_data_out_cnt)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_DATA_CNT o_pwd_data_cnt;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_DATA_CNT.u32);
    o_pwd_data_cnt.u32 = HDMITXRegRead(pu32RegAddr);
    o_pwd_data_cnt.bits.pwd_data_out_cnt = upwd_data_out_cnt;
    HDMITXRegWrite(pu32RegAddr, o_pwd_data_cnt.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pwd_data_cnt.u32, "PWD_DATA_CNT");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PWD_DATA_CNT_pwd_fifo_data_cntSet
//  Description : Set the value of the member PWD_DATA_CNT.pwd_fifo_data_cnt
//  Input       : unsigned int upwd_fifo_data_cnt: 5 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_DATA_CNT_pwd_fifo_data_cntSet(unsigned int upwd_fifo_data_cnt)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_DATA_CNT o_pwd_data_cnt;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_DATA_CNT.u32);
    o_pwd_data_cnt.u32 = HDMITXRegRead(pu32RegAddr);
    o_pwd_data_cnt.bits.pwd_fifo_data_cnt = upwd_fifo_data_cnt;
    HDMITXRegWrite(pu32RegAddr, o_pwd_data_cnt.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pwd_data_cnt.u32, "PWD_DATA_CNT");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PWD_SLAVE_CFG_pwd_slave_segSet
//  Description : Set the value of the member PWD_SLAVE_CFG.pwd_slave_seg
//  Input       : unsigned int upwd_slave_seg: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_SLAVE_CFG_pwd_slave_segSet(unsigned int upwd_slave_seg)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_SLAVE_CFG o_pwd_slave_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_SLAVE_CFG.u32);
    o_pwd_slave_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_pwd_slave_cfg.bits.pwd_slave_seg = upwd_slave_seg;
    HDMITXRegWrite(pu32RegAddr, o_pwd_slave_cfg.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pwd_slave_cfg.u32, "PWD_SLAVE_CFG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PWD_SLAVE_CFG_pwd_slave_offsetSet
//  Description : Set the value of the member PWD_SLAVE_CFG.pwd_slave_offset
//  Input       : unsigned int upwd_slave_offset: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_SLAVE_CFG_pwd_slave_offsetSet(unsigned int upwd_slave_offset)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_SLAVE_CFG o_pwd_slave_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_SLAVE_CFG.u32);
    o_pwd_slave_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_pwd_slave_cfg.bits.pwd_slave_offset = upwd_slave_offset;
    HDMITXRegWrite(pu32RegAddr, o_pwd_slave_cfg.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pwd_slave_cfg.u32, "PWD_SLAVE_CFG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PWD_SLAVE_CFG_pwd_slave_addrSet
//  Description : Set the value of the member PWD_SLAVE_CFG.pwd_slave_addr
//  Input       : unsigned int upwd_slave_addr: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_SLAVE_CFG_pwd_slave_addrSet(unsigned int upwd_slave_addr)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_SLAVE_CFG o_pwd_slave_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_SLAVE_CFG.u32);
    o_pwd_slave_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    o_pwd_slave_cfg.bits.pwd_slave_addr = upwd_slave_addr;
    HDMITXRegWrite(pu32RegAddr, o_pwd_slave_cfg.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pwd_slave_cfg.u32, "PWD_SLAVE_CFG");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PWD_MST_STATE_pwd_fifo_fullSet
//  Description : Set the value of the member PWD_MST_STATE.pwd_fifo_full
//  Input       : unsigned int upwd_fifo_full: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_MST_STATE_pwd_fifo_fullSet(unsigned int upwd_fifo_full)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_MST_STATE o_pwd_mst_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_MST_STATE.u32);
    o_pwd_mst_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_pwd_mst_state.bits.pwd_fifo_full = upwd_fifo_full;
    HDMITXRegWrite(pu32RegAddr, o_pwd_mst_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pwd_mst_state.u32, "PWD_MST_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PWD_MST_STATE_pwd_fifo_half_fullSet
//  Description : Set the value of the member PWD_MST_STATE.pwd_fifo_half_full
//  Input       : unsigned int upwd_fifo_half_full: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_MST_STATE_pwd_fifo_half_fullSet(unsigned int upwd_fifo_half_full)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_MST_STATE o_pwd_mst_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_MST_STATE.u32);
    o_pwd_mst_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_pwd_mst_state.bits.pwd_fifo_half_full = upwd_fifo_half_full;
    HDMITXRegWrite(pu32RegAddr, o_pwd_mst_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pwd_mst_state.u32, "PWD_MST_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PWD_MST_STATE_pwd_fifo_emptySet
//  Description : Set the value of the member PWD_MST_STATE.pwd_fifo_empty
//  Input       : unsigned int upwd_fifo_empty: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_MST_STATE_pwd_fifo_emptySet(unsigned int upwd_fifo_empty)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_MST_STATE o_pwd_mst_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_MST_STATE.u32);
    o_pwd_mst_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_pwd_mst_state.bits.pwd_fifo_empty = upwd_fifo_empty;
    HDMITXRegWrite(pu32RegAddr, o_pwd_mst_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pwd_mst_state.u32, "PWD_MST_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PWD_MST_STATE_pwd_fifo_rd_in_useSet
//  Description : Set the value of the member PWD_MST_STATE.pwd_fifo_rd_in_use
//  Input       : unsigned int upwd_fifo_rd_in_use: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_MST_STATE_pwd_fifo_rd_in_useSet(unsigned int upwd_fifo_rd_in_use)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_MST_STATE o_pwd_mst_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_MST_STATE.u32);
    o_pwd_mst_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_pwd_mst_state.bits.pwd_fifo_rd_in_use = upwd_fifo_rd_in_use;
    HDMITXRegWrite(pu32RegAddr, o_pwd_mst_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pwd_mst_state.u32, "PWD_MST_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PWD_MST_STATE_pwd_fifo_wr_in_useSet
//  Description : Set the value of the member PWD_MST_STATE.pwd_fifo_wr_in_use
//  Input       : unsigned int upwd_fifo_wr_in_use: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_MST_STATE_pwd_fifo_wr_in_useSet(unsigned int upwd_fifo_wr_in_use)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_MST_STATE o_pwd_mst_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_MST_STATE.u32);
    o_pwd_mst_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_pwd_mst_state.bits.pwd_fifo_wr_in_use = upwd_fifo_wr_in_use;
    HDMITXRegWrite(pu32RegAddr, o_pwd_mst_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pwd_mst_state.u32, "PWD_MST_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PWD_MST_STATE_pwd_i2c_in_progSet
//  Description : Set the value of the member PWD_MST_STATE.pwd_i2c_in_prog
//  Input       : unsigned int upwd_i2c_in_prog: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_MST_STATE_pwd_i2c_in_progSet(unsigned int upwd_i2c_in_prog)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_MST_STATE o_pwd_mst_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_MST_STATE.u32);
    o_pwd_mst_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_pwd_mst_state.bits.pwd_i2c_in_prog = upwd_i2c_in_prog;
    HDMITXRegWrite(pu32RegAddr, o_pwd_mst_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pwd_mst_state.u32, "PWD_MST_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PWD_MST_STATE_pwd_i2c_bus_lowSet
//  Description : Set the value of the member PWD_MST_STATE.pwd_i2c_bus_low
//  Input       : unsigned int upwd_i2c_bus_low: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_MST_STATE_pwd_i2c_bus_lowSet(unsigned int upwd_i2c_bus_low)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_MST_STATE o_pwd_mst_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_MST_STATE.u32);
    o_pwd_mst_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_pwd_mst_state.bits.pwd_i2c_bus_low = upwd_i2c_bus_low;
    HDMITXRegWrite(pu32RegAddr, o_pwd_mst_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pwd_mst_state.u32, "PWD_MST_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PWD_MST_STATE_pwd_i2c_no_ackSet
//  Description : Set the value of the member PWD_MST_STATE.pwd_i2c_no_ack
//  Input       : unsigned int upwd_i2c_no_ack: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_MST_STATE_pwd_i2c_no_ackSet(unsigned int upwd_i2c_no_ack)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_MST_STATE o_pwd_mst_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_MST_STATE.u32);
    o_pwd_mst_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_pwd_mst_state.bits.pwd_i2c_no_ack = upwd_i2c_no_ack;
    HDMITXRegWrite(pu32RegAddr, o_pwd_mst_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pwd_mst_state.u32, "PWD_MST_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PWD_MST_CMD_pwd_mst_cmdSet
//  Description : Set the value of the member PWD_MST_CMD.pwd_mst_cmd
//  Input       : unsigned int upwd_mst_cmd: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_MST_CMD_pwd_mst_cmdSet(unsigned int upwd_mst_cmd)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_MST_CMD o_pwd_mst_cmd;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_MST_CMD.u32);
    o_pwd_mst_cmd.u32 = HDMITXRegRead(pu32RegAddr);
    o_pwd_mst_cmd.bits.pwd_mst_cmd = upwd_mst_cmd;
    HDMITXRegWrite(pu32RegAddr, o_pwd_mst_cmd.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pwd_mst_cmd.u32, "PWD_MST_CMD");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PWD_MST_CLR_pwd_clr_no_ackSet
//  Description : Set the value of the member PWD_MST_CLR.pwd_clr_no_ack
//  Input       : unsigned int upwd_clr_no_ack: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_MST_CLR_pwd_clr_no_ackSet(unsigned int upwd_clr_no_ack)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_MST_CLR o_pwd_mst_clr;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_MST_CLR.u32);
    o_pwd_mst_clr.u32 = HDMITXRegRead(pu32RegAddr);
    o_pwd_mst_clr.bits.pwd_clr_no_ack = upwd_clr_no_ack;
    HDMITXRegWrite(pu32RegAddr, o_pwd_mst_clr.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pwd_mst_clr.u32, "PWD_MST_CLR");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PWD_MST_CLR_pwd_clr_bus_lowSet
//  Description : Set the value of the member PWD_MST_CLR.pwd_clr_bus_low
//  Input       : unsigned int upwd_clr_bus_low: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_MST_CLR_pwd_clr_bus_lowSet(unsigned int upwd_clr_bus_low)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_MST_CLR o_pwd_mst_clr;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_MST_CLR.u32);
    o_pwd_mst_clr.u32 = HDMITXRegRead(pu32RegAddr);
    o_pwd_mst_clr.bits.pwd_clr_bus_low = upwd_clr_bus_low;
    HDMITXRegWrite(pu32RegAddr, o_pwd_mst_clr.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pwd_mst_clr.u32, "PWD_MST_CLR");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_DDC_MST_ARB_CTRL_reg_auto_abort_enSet
//  Description : Set the value of the member DDC_MST_ARB_CTRL.reg_auto_abort_en
//  Input       : unsigned int ureg_auto_abort_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_MST_ARB_CTRL_reg_auto_abort_enSet(unsigned int ureg_auto_abort_en)
{
    HI_U32 *pu32RegAddr = NULL;
    U_DDC_MST_ARB_CTRL o_ddc_mst_arb_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->DDC_MST_ARB_CTRL.u32);
    o_ddc_mst_arb_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_ddc_mst_arb_ctrl.bits.reg_auto_abort_en = ureg_auto_abort_en;
    HDMITXRegWrite(pu32RegAddr, o_ddc_mst_arb_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_ddc_mst_arb_ctrl.u32, "DDC_MST_ARB_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_DDC_MST_ARB_CTRL_cpu_ddc_force_reqSet
//  Description : Set the value of the member DDC_MST_ARB_CTRL.cpu_ddc_force_req
//  Input       : unsigned int ucpu_ddc_force_req: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_MST_ARB_CTRL_cpu_ddc_force_reqSet(unsigned int ucpu_ddc_force_req)
{
    HI_U32 *pu32RegAddr = NULL;
    U_DDC_MST_ARB_CTRL o_ddc_mst_arb_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->DDC_MST_ARB_CTRL.u32);
    o_ddc_mst_arb_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    o_ddc_mst_arb_ctrl.bits.cpu_ddc_force_req = ucpu_ddc_force_req;
    HDMITXRegWrite(pu32RegAddr, o_ddc_mst_arb_ctrl.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_ddc_mst_arb_ctrl.u32, "DDC_MST_ARB_CTRL");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_DDC_MST_ARB_REQ_cpu_ddc_reqSet
//  Description : Set the value of the member DDC_MST_ARB_REQ.cpu_ddc_req
//  Input       : unsigned int ucpu_ddc_req: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_MST_ARB_REQ_cpu_ddc_reqSet(unsigned int ucpu_ddc_req)
{
    HI_U32 *pu32RegAddr = NULL;
    U_DDC_MST_ARB_REQ o_ddc_mst_arb_req;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->DDC_MST_ARB_REQ.u32);
    o_ddc_mst_arb_req.u32 = HDMITXRegRead(pu32RegAddr);
    o_ddc_mst_arb_req.bits.cpu_ddc_req = ucpu_ddc_req;
    HDMITXRegWrite(pu32RegAddr, o_ddc_mst_arb_req.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_ddc_mst_arb_req.u32, "DDC_MST_ARB_REQ");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_DDC_MST_ARB_ACK_cpu_ddc_req_ackSet
//  Description : Set the value of the member DDC_MST_ARB_ACK.cpu_ddc_req_ack
//  Input       : unsigned int ucpu_ddc_req_ack: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_MST_ARB_ACK_cpu_ddc_req_ackSet(unsigned int ucpu_ddc_req_ack)
{
    HI_U32 *pu32RegAddr = NULL;
    U_DDC_MST_ARB_ACK o_ddc_mst_arb_ack;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->DDC_MST_ARB_ACK.u32);
    o_ddc_mst_arb_ack.u32 = HDMITXRegRead(pu32RegAddr);
    o_ddc_mst_arb_ack.bits.cpu_ddc_req_ack = ucpu_ddc_req_ack;
    HDMITXRegWrite(pu32RegAddr, o_ddc_mst_arb_ack.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_ddc_mst_arb_ack.u32, "DDC_MST_ARB_ACK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_DDC_MST_ARB_STATE_ddc_arb_stateSet
//  Description : Set the value of the member DDC_MST_ARB_STATE.ddc_arb_state
//  Input       : unsigned int uddc_arb_state: 9 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_MST_ARB_STATE_ddc_arb_stateSet(unsigned int uddc_arb_state)
{
    HI_U32 *pu32RegAddr = NULL;
    U_DDC_MST_ARB_STATE o_ddc_mst_arb_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->DDC_MST_ARB_STATE.u32);
    o_ddc_mst_arb_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_ddc_mst_arb_state.bits.ddc_arb_state = uddc_arb_state;
    HDMITXRegWrite(pu32RegAddr, o_ddc_mst_arb_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_ddc_mst_arb_state.u32, "DDC_MST_ARB_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TX_PWD_INTR_STATE_tx_pwd_intr_stateSet
//  Description : Set the value of the member TX_PWD_INTR_STATE.tx_pwd_intr_state
//  Input       : unsigned int utx_pwd_intr_state: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_PWD_INTR_STATE_tx_pwd_intr_stateSet(unsigned int utx_pwd_intr_state)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TX_PWD_INTR_STATE o_tx_pwd_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TX_PWD_INTR_STATE.u32);
    o_tx_pwd_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_tx_pwd_intr_state.bits.tx_pwd_intr_state = utx_pwd_intr_state;
    HDMITXRegWrite(pu32RegAddr, o_tx_pwd_intr_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_tx_pwd_intr_state.u32, "TX_PWD_INTR_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PWD_SUB_INTR_STATE_hdcp2x_intr_stateSet
//  Description : Set the value of the member PWD_SUB_INTR_STATE.hdcp2x_intr_state
//  Input       : unsigned int uhdcp2x_intr_state: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_SUB_INTR_STATE_hdcp2x_intr_stateSet(unsigned int uhdcp2x_intr_state)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_SUB_INTR_STATE o_pwd_sub_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_SUB_INTR_STATE.u32);
    o_pwd_sub_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_pwd_sub_intr_state.bits.hdcp2x_intr_state = uhdcp2x_intr_state;
    HDMITXRegWrite(pu32RegAddr, o_pwd_sub_intr_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pwd_sub_intr_state.u32, "PWD_SUB_INTR_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PWD_SUB_INTR_STATE_txhdcp_intr_stateSet
//  Description : Set the value of the member PWD_SUB_INTR_STATE.txhdcp_intr_state
//  Input       : unsigned int utxhdcp_intr_state: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_SUB_INTR_STATE_txhdcp_intr_stateSet(unsigned int utxhdcp_intr_state)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_SUB_INTR_STATE o_pwd_sub_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_SUB_INTR_STATE.u32);
    o_pwd_sub_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_pwd_sub_intr_state.bits.txhdcp_intr_state = utxhdcp_intr_state;
    HDMITXRegWrite(pu32RegAddr, o_pwd_sub_intr_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pwd_sub_intr_state.u32, "PWD_SUB_INTR_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PWD_SUB_INTR_STATE_txhdmi_intr_stateSet
//  Description : Set the value of the member PWD_SUB_INTR_STATE.txhdmi_intr_state
//  Input       : unsigned int utxhdmi_intr_state: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_SUB_INTR_STATE_txhdmi_intr_stateSet(unsigned int utxhdmi_intr_state)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_SUB_INTR_STATE o_pwd_sub_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_SUB_INTR_STATE.u32);
    o_pwd_sub_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_pwd_sub_intr_state.bits.txhdmi_intr_state = utxhdmi_intr_state;
    HDMITXRegWrite(pu32RegAddr, o_pwd_sub_intr_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pwd_sub_intr_state.u32, "PWD_SUB_INTR_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PWD_SUB_INTR_STATE_audpath_intr_stateSet
//  Description : Set the value of the member PWD_SUB_INTR_STATE.audpath_intr_state
//  Input       : unsigned int uaudpath_intr_state: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_SUB_INTR_STATE_audpath_intr_stateSet(unsigned int uaudpath_intr_state)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_SUB_INTR_STATE o_pwd_sub_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_SUB_INTR_STATE.u32);
    o_pwd_sub_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_pwd_sub_intr_state.bits.audpath_intr_state = uaudpath_intr_state;
    HDMITXRegWrite(pu32RegAddr, o_pwd_sub_intr_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pwd_sub_intr_state.u32, "PWD_SUB_INTR_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PWD_SUB_INTR_STATE_vidpath_intr_stateSet
//  Description : Set the value of the member PWD_SUB_INTR_STATE.vidpath_intr_state
//  Input       : unsigned int uvidpath_intr_state: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_SUB_INTR_STATE_vidpath_intr_stateSet(unsigned int uvidpath_intr_state)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_SUB_INTR_STATE o_pwd_sub_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_SUB_INTR_STATE.u32);
    o_pwd_sub_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_pwd_sub_intr_state.bits.vidpath_intr_state = uvidpath_intr_state;
    HDMITXRegWrite(pu32RegAddr, o_pwd_sub_intr_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pwd_sub_intr_state.u32, "PWD_SUB_INTR_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PWD_SUB_INTR_STATE_tx_sys_intr_stateSet
//  Description : Set the value of the member PWD_SUB_INTR_STATE.tx_sys_intr_state
//  Input       : unsigned int utx_sys_intr_state: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_SUB_INTR_STATE_tx_sys_intr_stateSet(unsigned int utx_sys_intr_state)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_SUB_INTR_STATE o_pwd_sub_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_SUB_INTR_STATE.u32);
    o_pwd_sub_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_pwd_sub_intr_state.bits.tx_sys_intr_state = utx_sys_intr_state;
    HDMITXRegWrite(pu32RegAddr, o_pwd_sub_intr_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pwd_sub_intr_state.u32, "PWD_SUB_INTR_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PWD_SUB_INTR_MASK_hdcp2x_intr_maskSet
//  Description : Set the value of the member PWD_SUB_INTR_MASK.hdcp2x_intr_mask
//  Input       : unsigned int uhdcp2x_intr_mask: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_SUB_INTR_MASK_hdcp2x_intr_maskSet(unsigned int uhdcp2x_intr_mask)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_SUB_INTR_MASK o_pwd_sub_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_SUB_INTR_MASK.u32);
    o_pwd_sub_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    o_pwd_sub_intr_mask.bits.hdcp2x_intr_mask = uhdcp2x_intr_mask;
    HDMITXRegWrite(pu32RegAddr, o_pwd_sub_intr_mask.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pwd_sub_intr_mask.u32, "PWD_SUB_INTR_MASK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PWD_SUB_INTR_MASK_txhdcp_intr_maskSet
//  Description : Set the value of the member PWD_SUB_INTR_MASK.txhdcp_intr_mask
//  Input       : unsigned int utxhdcp_intr_mask: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_SUB_INTR_MASK_txhdcp_intr_maskSet(unsigned int utxhdcp_intr_mask)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_SUB_INTR_MASK o_pwd_sub_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_SUB_INTR_MASK.u32);
    o_pwd_sub_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    o_pwd_sub_intr_mask.bits.txhdcp_intr_mask = utxhdcp_intr_mask;
    HDMITXRegWrite(pu32RegAddr, o_pwd_sub_intr_mask.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pwd_sub_intr_mask.u32, "PWD_SUB_INTR_MASK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PWD_SUB_INTR_MASK_txhdmi_intr_maskSet
//  Description : Set the value of the member PWD_SUB_INTR_MASK.txhdmi_intr_mask
//  Input       : unsigned int utxhdmi_intr_mask: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_SUB_INTR_MASK_txhdmi_intr_maskSet(unsigned int utxhdmi_intr_mask)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_SUB_INTR_MASK o_pwd_sub_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_SUB_INTR_MASK.u32);
    o_pwd_sub_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    o_pwd_sub_intr_mask.bits.txhdmi_intr_mask = utxhdmi_intr_mask;
    HDMITXRegWrite(pu32RegAddr, o_pwd_sub_intr_mask.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pwd_sub_intr_mask.u32, "PWD_SUB_INTR_MASK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PWD_SUB_INTR_MASK_audpath_intr_maskSet
//  Description : Set the value of the member PWD_SUB_INTR_MASK.audpath_intr_mask
//  Input       : unsigned int uaudpath_intr_mask: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_SUB_INTR_MASK_audpath_intr_maskSet(unsigned int uaudpath_intr_mask)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_SUB_INTR_MASK o_pwd_sub_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_SUB_INTR_MASK.u32);
    o_pwd_sub_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    o_pwd_sub_intr_mask.bits.audpath_intr_mask = uaudpath_intr_mask;
    HDMITXRegWrite(pu32RegAddr, o_pwd_sub_intr_mask.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pwd_sub_intr_mask.u32, "PWD_SUB_INTR_MASK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PWD_SUB_INTR_MASK_vidpath_intr_maskSet
//  Description : Set the value of the member PWD_SUB_INTR_MASK.vidpath_intr_mask
//  Input       : unsigned int uvidpath_intr_mask: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_SUB_INTR_MASK_vidpath_intr_maskSet(unsigned int uvidpath_intr_mask)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_SUB_INTR_MASK o_pwd_sub_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_SUB_INTR_MASK.u32);
    o_pwd_sub_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    o_pwd_sub_intr_mask.bits.vidpath_intr_mask = uvidpath_intr_mask;
    HDMITXRegWrite(pu32RegAddr, o_pwd_sub_intr_mask.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pwd_sub_intr_mask.u32, "PWD_SUB_INTR_MASK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_PWD_SUB_INTR_MASK_tx_sys_intr_maskSet
//  Description : Set the value of the member PWD_SUB_INTR_MASK.tx_sys_intr_mask
//  Input       : unsigned int utx_sys_intr_mask: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_SUB_INTR_MASK_tx_sys_intr_maskSet(unsigned int utx_sys_intr_mask)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_SUB_INTR_MASK o_pwd_sub_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_SUB_INTR_MASK.u32);
    o_pwd_sub_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    o_pwd_sub_intr_mask.bits.tx_sys_intr_mask = utx_sys_intr_mask;
    HDMITXRegWrite(pu32RegAddr, o_pwd_sub_intr_mask.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_pwd_sub_intr_mask.u32, "PWD_SUB_INTR_MASK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TXSYS_INTR_STATE_tx_sys_intr_state5Set
//  Description : Set the value of the member TXSYS_INTR_STATE.tx_sys_intr_state5
//  Input       : unsigned int utx_sys_intr_state5: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TXSYS_INTR_STATE_tx_sys_intr_state5Set(unsigned int utx_sys_intr_state5)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TXSYS_INTR_STATE o_txsys_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TXSYS_INTR_STATE.u32);
    o_txsys_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_txsys_intr_state.bits.tx_sys_intr_state5 = utx_sys_intr_state5;
    HDMITXRegWrite(pu32RegAddr, o_txsys_intr_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_txsys_intr_state.u32, "TXSYS_INTR_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TXSYS_INTR_STATE_tx_sys_intr_state4Set
//  Description : Set the value of the member TXSYS_INTR_STATE.tx_sys_intr_state4
//  Input       : unsigned int utx_sys_intr_state4: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TXSYS_INTR_STATE_tx_sys_intr_state4Set(unsigned int utx_sys_intr_state4)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TXSYS_INTR_STATE o_txsys_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TXSYS_INTR_STATE.u32);
    o_txsys_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_txsys_intr_state.bits.tx_sys_intr_state4 = utx_sys_intr_state4;
    HDMITXRegWrite(pu32RegAddr, o_txsys_intr_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_txsys_intr_state.u32, "TXSYS_INTR_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TXSYS_INTR_STATE_tx_sys_intr_state3Set
//  Description : Set the value of the member TXSYS_INTR_STATE.tx_sys_intr_state3
//  Input       : unsigned int utx_sys_intr_state3: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TXSYS_INTR_STATE_tx_sys_intr_state3Set(unsigned int utx_sys_intr_state3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TXSYS_INTR_STATE o_txsys_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TXSYS_INTR_STATE.u32);
    o_txsys_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_txsys_intr_state.bits.tx_sys_intr_state3 = utx_sys_intr_state3;
    HDMITXRegWrite(pu32RegAddr, o_txsys_intr_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_txsys_intr_state.u32, "TXSYS_INTR_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TXSYS_INTR_STATE_tx_sys_intr_state2Set
//  Description : Set the value of the member TXSYS_INTR_STATE.tx_sys_intr_state2
//  Input       : unsigned int utx_sys_intr_state2: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TXSYS_INTR_STATE_tx_sys_intr_state2Set(unsigned int utx_sys_intr_state2)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TXSYS_INTR_STATE o_txsys_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TXSYS_INTR_STATE.u32);
    o_txsys_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_txsys_intr_state.bits.tx_sys_intr_state2 = utx_sys_intr_state2;
    HDMITXRegWrite(pu32RegAddr, o_txsys_intr_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_txsys_intr_state.u32, "TXSYS_INTR_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TXSYS_INTR_STATE_tx_sys_intr_state1Set
//  Description : Set the value of the member TXSYS_INTR_STATE.tx_sys_intr_state1
//  Input       : unsigned int utx_sys_intr_state1: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TXSYS_INTR_STATE_tx_sys_intr_state1Set(unsigned int utx_sys_intr_state1)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TXSYS_INTR_STATE o_txsys_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TXSYS_INTR_STATE.u32);
    o_txsys_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_txsys_intr_state.bits.tx_sys_intr_state1 = utx_sys_intr_state1;
    HDMITXRegWrite(pu32RegAddr, o_txsys_intr_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_txsys_intr_state.u32, "TXSYS_INTR_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TXSYS_INTR_STATE_tx_sys_intr_state0Set
//  Description : Set the value of the member TXSYS_INTR_STATE.tx_sys_intr_state0
//  Input       : unsigned int utx_sys_intr_state0: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TXSYS_INTR_STATE_tx_sys_intr_state0Set(unsigned int utx_sys_intr_state0)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TXSYS_INTR_STATE o_txsys_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TXSYS_INTR_STATE.u32);
    o_txsys_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_txsys_intr_state.bits.tx_sys_intr_state0 = utx_sys_intr_state0;
    HDMITXRegWrite(pu32RegAddr, o_txsys_intr_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_txsys_intr_state.u32, "TXSYS_INTR_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TXSYS_INTR_MASK_tx_sys_intr_mask5Set
//  Description : Set the value of the member TXSYS_INTR_MASK.tx_sys_intr_mask5
//  Input       : unsigned int utx_sys_intr_mask5: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TXSYS_INTR_MASK_tx_sys_intr_mask5Set(unsigned int utx_sys_intr_mask5)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TXSYS_INTR_MASK o_txsys_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TXSYS_INTR_MASK.u32);
    o_txsys_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    o_txsys_intr_mask.bits.tx_sys_intr_mask5 = utx_sys_intr_mask5;
    HDMITXRegWrite(pu32RegAddr, o_txsys_intr_mask.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_txsys_intr_mask.u32, "TXSYS_INTR_MASK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TXSYS_INTR_MASK_tx_sys_intr_mask4Set
//  Description : Set the value of the member TXSYS_INTR_MASK.tx_sys_intr_mask4
//  Input       : unsigned int utx_sys_intr_mask4: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TXSYS_INTR_MASK_tx_sys_intr_mask4Set(unsigned int utx_sys_intr_mask4)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TXSYS_INTR_MASK o_txsys_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TXSYS_INTR_MASK.u32);
    o_txsys_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    o_txsys_intr_mask.bits.tx_sys_intr_mask4 = utx_sys_intr_mask4;
    HDMITXRegWrite(pu32RegAddr, o_txsys_intr_mask.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_txsys_intr_mask.u32, "TXSYS_INTR_MASK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TXSYS_INTR_MASK_tx_sys_intr_mask3Set
//  Description : Set the value of the member TXSYS_INTR_MASK.tx_sys_intr_mask3
//  Input       : unsigned int utx_sys_intr_mask3: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TXSYS_INTR_MASK_tx_sys_intr_mask3Set(unsigned int utx_sys_intr_mask3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TXSYS_INTR_MASK o_txsys_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TXSYS_INTR_MASK.u32);
    o_txsys_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    o_txsys_intr_mask.bits.tx_sys_intr_mask3 = utx_sys_intr_mask3;
    HDMITXRegWrite(pu32RegAddr, o_txsys_intr_mask.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_txsys_intr_mask.u32, "TXSYS_INTR_MASK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TXSYS_INTR_MASK_tx_sys_intr_mask2Set
//  Description : Set the value of the member TXSYS_INTR_MASK.tx_sys_intr_mask2
//  Input       : unsigned int utx_sys_intr_mask2: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TXSYS_INTR_MASK_tx_sys_intr_mask2Set(unsigned int utx_sys_intr_mask2)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TXSYS_INTR_MASK o_txsys_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TXSYS_INTR_MASK.u32);
    o_txsys_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    o_txsys_intr_mask.bits.tx_sys_intr_mask2 = utx_sys_intr_mask2;
    HDMITXRegWrite(pu32RegAddr, o_txsys_intr_mask.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_txsys_intr_mask.u32, "TXSYS_INTR_MASK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TXSYS_INTR_MASK_tx_sys_intr_mask1Set
//  Description : Set the value of the member TXSYS_INTR_MASK.tx_sys_intr_mask1
//  Input       : unsigned int utx_sys_intr_mask1: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TXSYS_INTR_MASK_tx_sys_intr_mask1Set(unsigned int utx_sys_intr_mask1)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TXSYS_INTR_MASK o_txsys_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TXSYS_INTR_MASK.u32);
    o_txsys_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    o_txsys_intr_mask.bits.tx_sys_intr_mask1 = utx_sys_intr_mask1;
    HDMITXRegWrite(pu32RegAddr, o_txsys_intr_mask.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_txsys_intr_mask.u32, "TXSYS_INTR_MASK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TXSYS_INTR_MASK_tx_sys_intr_mask0Set
//  Description : Set the value of the member TXSYS_INTR_MASK.tx_sys_intr_mask0
//  Input       : unsigned int utx_sys_intr_mask0: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TXSYS_INTR_MASK_tx_sys_intr_mask0Set(unsigned int utx_sys_intr_mask0)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TXSYS_INTR_MASK o_txsys_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TXSYS_INTR_MASK.u32);
    o_txsys_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    o_txsys_intr_mask.bits.tx_sys_intr_mask0 = utx_sys_intr_mask0;
    HDMITXRegWrite(pu32RegAddr, o_txsys_intr_mask.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_txsys_intr_mask.u32, "TXSYS_INTR_MASK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_VIDPATH_INTR_STATE_vidpath_intr_state6Set
//  Description : Set the value of the member VIDPATH_INTR_STATE.vidpath_intr_state6
//  Input       : unsigned int uvidpath_intr_state6: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDPATH_INTR_STATE_vidpath_intr_state6Set(unsigned int uvidpath_intr_state6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VIDPATH_INTR_STATE o_vidpath_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->VIDPATH_INTR_STATE.u32);
    o_vidpath_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_vidpath_intr_state.bits.vidpath_intr_state6 = uvidpath_intr_state6;
    HDMITXRegWrite(pu32RegAddr, o_vidpath_intr_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_vidpath_intr_state.u32, "VIDPATH_INTR_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_VIDPATH_INTR_STATE_vidpath_intr_state5Set
//  Description : Set the value of the member VIDPATH_INTR_STATE.vidpath_intr_state5
//  Input       : unsigned int uvidpath_intr_state5: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDPATH_INTR_STATE_vidpath_intr_state5Set(unsigned int uvidpath_intr_state5)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VIDPATH_INTR_STATE o_vidpath_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->VIDPATH_INTR_STATE.u32);
    o_vidpath_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_vidpath_intr_state.bits.vidpath_intr_state5 = uvidpath_intr_state5;
    HDMITXRegWrite(pu32RegAddr, o_vidpath_intr_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_vidpath_intr_state.u32, "VIDPATH_INTR_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_VIDPATH_INTR_STATE_vidpath_intr_state4Set
//  Description : Set the value of the member VIDPATH_INTR_STATE.vidpath_intr_state4
//  Input       : unsigned int uvidpath_intr_state4: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDPATH_INTR_STATE_vidpath_intr_state4Set(unsigned int uvidpath_intr_state4)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VIDPATH_INTR_STATE o_vidpath_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->VIDPATH_INTR_STATE.u32);
    o_vidpath_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_vidpath_intr_state.bits.vidpath_intr_state4 = uvidpath_intr_state4;
    HDMITXRegWrite(pu32RegAddr, o_vidpath_intr_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_vidpath_intr_state.u32, "VIDPATH_INTR_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_VIDPATH_INTR_STATE_vidpath_intr_state3Set
//  Description : Set the value of the member VIDPATH_INTR_STATE.vidpath_intr_state3
//  Input       : unsigned int uvidpath_intr_state3: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDPATH_INTR_STATE_vidpath_intr_state3Set(unsigned int uvidpath_intr_state3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VIDPATH_INTR_STATE o_vidpath_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->VIDPATH_INTR_STATE.u32);
    o_vidpath_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_vidpath_intr_state.bits.vidpath_intr_state3 = uvidpath_intr_state3;
    HDMITXRegWrite(pu32RegAddr, o_vidpath_intr_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_vidpath_intr_state.u32, "VIDPATH_INTR_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_VIDPATH_INTR_STATE_vidpath_intr_state2Set
//  Description : Set the value of the member VIDPATH_INTR_STATE.vidpath_intr_state2
//  Input       : unsigned int uvidpath_intr_state2: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDPATH_INTR_STATE_vidpath_intr_state2Set(unsigned int uvidpath_intr_state2)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VIDPATH_INTR_STATE o_vidpath_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->VIDPATH_INTR_STATE.u32);
    o_vidpath_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_vidpath_intr_state.bits.vidpath_intr_state2 = uvidpath_intr_state2;
    HDMITXRegWrite(pu32RegAddr, o_vidpath_intr_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_vidpath_intr_state.u32, "VIDPATH_INTR_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_VIDPATH_INTR_STATE_vidpath_intr_state1Set
//  Description : Set the value of the member VIDPATH_INTR_STATE.vidpath_intr_state1
//  Input       : unsigned int uvidpath_intr_state1: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDPATH_INTR_STATE_vidpath_intr_state1Set(unsigned int uvidpath_intr_state1)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VIDPATH_INTR_STATE o_vidpath_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->VIDPATH_INTR_STATE.u32);
    o_vidpath_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_vidpath_intr_state.bits.vidpath_intr_state1 = uvidpath_intr_state1;
    HDMITXRegWrite(pu32RegAddr, o_vidpath_intr_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_vidpath_intr_state.u32, "VIDPATH_INTR_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_VIDPATH_INTR_STATE_vidpath_intr_state0Set
//  Description : Set the value of the member VIDPATH_INTR_STATE.vidpath_intr_state0
//  Input       : unsigned int uvidpath_intr_state0: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDPATH_INTR_STATE_vidpath_intr_state0Set(unsigned int uvidpath_intr_state0)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VIDPATH_INTR_STATE o_vidpath_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->VIDPATH_INTR_STATE.u32);
    o_vidpath_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_vidpath_intr_state.bits.vidpath_intr_state0 = uvidpath_intr_state0;
    HDMITXRegWrite(pu32RegAddr, o_vidpath_intr_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_vidpath_intr_state.u32, "VIDPATH_INTR_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_VIDPATH_INTR_MASK_vidpath_intr_mask6Set
//  Description : Set the value of the member VIDPATH_INTR_MASK.vidpath_intr_mask6
//  Input       : unsigned int uvidpath_intr_mask6: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDPATH_INTR_MASK_vidpath_intr_mask6Set(unsigned int uvidpath_intr_mask6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VIDPATH_INTR_MASK o_vidpath_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->VIDPATH_INTR_MASK.u32);
    o_vidpath_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    o_vidpath_intr_mask.bits.vidpath_intr_mask6 = uvidpath_intr_mask6;
    HDMITXRegWrite(pu32RegAddr, o_vidpath_intr_mask.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_vidpath_intr_mask.u32, "VIDPATH_INTR_MASK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_VIDPATH_INTR_MASK_vidpath_intr_mask5Set
//  Description : Set the value of the member VIDPATH_INTR_MASK.vidpath_intr_mask5
//  Input       : unsigned int uvidpath_intr_mask5: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDPATH_INTR_MASK_vidpath_intr_mask5Set(unsigned int uvidpath_intr_mask5)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VIDPATH_INTR_MASK o_vidpath_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->VIDPATH_INTR_MASK.u32);
    o_vidpath_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    o_vidpath_intr_mask.bits.vidpath_intr_mask5 = uvidpath_intr_mask5;
    HDMITXRegWrite(pu32RegAddr, o_vidpath_intr_mask.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_vidpath_intr_mask.u32, "VIDPATH_INTR_MASK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_VIDPATH_INTR_MASK_vidpath_intr_mask4Set
//  Description : Set the value of the member VIDPATH_INTR_MASK.vidpath_intr_mask4
//  Input       : unsigned int uvidpath_intr_mask4: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDPATH_INTR_MASK_vidpath_intr_mask4Set(unsigned int uvidpath_intr_mask4)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VIDPATH_INTR_MASK o_vidpath_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->VIDPATH_INTR_MASK.u32);
    o_vidpath_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    o_vidpath_intr_mask.bits.vidpath_intr_mask4 = uvidpath_intr_mask4;
    HDMITXRegWrite(pu32RegAddr, o_vidpath_intr_mask.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_vidpath_intr_mask.u32, "VIDPATH_INTR_MASK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_VIDPATH_INTR_MASK_vidpath_intr_mask3Set
//  Description : Set the value of the member VIDPATH_INTR_MASK.vidpath_intr_mask3
//  Input       : unsigned int uvidpath_intr_mask3: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDPATH_INTR_MASK_vidpath_intr_mask3Set(unsigned int uvidpath_intr_mask3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VIDPATH_INTR_MASK o_vidpath_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->VIDPATH_INTR_MASK.u32);
    o_vidpath_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    o_vidpath_intr_mask.bits.vidpath_intr_mask3 = uvidpath_intr_mask3;
    HDMITXRegWrite(pu32RegAddr, o_vidpath_intr_mask.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_vidpath_intr_mask.u32, "VIDPATH_INTR_MASK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_VIDPATH_INTR_MASK_vidpath_intr_mask2Set
//  Description : Set the value of the member VIDPATH_INTR_MASK.vidpath_intr_mask2
//  Input       : unsigned int uvidpath_intr_mask2: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDPATH_INTR_MASK_vidpath_intr_mask2Set(unsigned int uvidpath_intr_mask2)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VIDPATH_INTR_MASK o_vidpath_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->VIDPATH_INTR_MASK.u32);
    o_vidpath_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    o_vidpath_intr_mask.bits.vidpath_intr_mask2 = uvidpath_intr_mask2;
    HDMITXRegWrite(pu32RegAddr, o_vidpath_intr_mask.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_vidpath_intr_mask.u32, "VIDPATH_INTR_MASK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_VIDPATH_INTR_MASK_vidpath_intr_mask1Set
//  Description : Set the value of the member VIDPATH_INTR_MASK.vidpath_intr_mask1
//  Input       : unsigned int uvidpath_intr_mask1: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDPATH_INTR_MASK_vidpath_intr_mask1Set(unsigned int uvidpath_intr_mask1)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VIDPATH_INTR_MASK o_vidpath_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->VIDPATH_INTR_MASK.u32);
    o_vidpath_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    o_vidpath_intr_mask.bits.vidpath_intr_mask1 = uvidpath_intr_mask1;
    HDMITXRegWrite(pu32RegAddr, o_vidpath_intr_mask.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_vidpath_intr_mask.u32, "VIDPATH_INTR_MASK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_VIDPATH_INTR_MASK_vidpath_intr_mask0Set
//  Description : Set the value of the member VIDPATH_INTR_MASK.vidpath_intr_mask0
//  Input       : unsigned int uvidpath_intr_mask0: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDPATH_INTR_MASK_vidpath_intr_mask0Set(unsigned int uvidpath_intr_mask0)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VIDPATH_INTR_MASK o_vidpath_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->VIDPATH_INTR_MASK.u32);
    o_vidpath_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    o_vidpath_intr_mask.bits.vidpath_intr_mask0 = uvidpath_intr_mask0;
    HDMITXRegWrite(pu32RegAddr, o_vidpath_intr_mask.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_vidpath_intr_mask.u32, "VIDPATH_INTR_MASK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_AUDPATH_INTR_STATE_audpath_intr_state8Set
//  Description : Set the value of the member AUDPATH_INTR_STATE.audpath_intr_state8
//  Input       : unsigned int uaudpath_intr_state8: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUDPATH_INTR_STATE_audpath_intr_state8Set(unsigned int uaudpath_intr_state8)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AUDPATH_INTR_STATE o_audpath_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->AUDPATH_INTR_STATE.u32);
    o_audpath_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_audpath_intr_state.bits.audpath_intr_state8 = uaudpath_intr_state8;
    HDMITXRegWrite(pu32RegAddr, o_audpath_intr_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_audpath_intr_state.u32, "AUDPATH_INTR_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_AUDPATH_INTR_STATE_audpath_intr_state7Set
//  Description : Set the value of the member AUDPATH_INTR_STATE.audpath_intr_state7
//  Input       : unsigned int uaudpath_intr_state7: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUDPATH_INTR_STATE_audpath_intr_state7Set(unsigned int uaudpath_intr_state7)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AUDPATH_INTR_STATE o_audpath_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->AUDPATH_INTR_STATE.u32);
    o_audpath_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_audpath_intr_state.bits.audpath_intr_state7 = uaudpath_intr_state7;
    HDMITXRegWrite(pu32RegAddr, o_audpath_intr_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_audpath_intr_state.u32, "AUDPATH_INTR_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_AUDPATH_INTR_STATE_audpath_intr_state6Set
//  Description : Set the value of the member AUDPATH_INTR_STATE.audpath_intr_state6
//  Input       : unsigned int uaudpath_intr_state6: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUDPATH_INTR_STATE_audpath_intr_state6Set(unsigned int uaudpath_intr_state6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AUDPATH_INTR_STATE o_audpath_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->AUDPATH_INTR_STATE.u32);
    o_audpath_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_audpath_intr_state.bits.audpath_intr_state6 = uaudpath_intr_state6;
    HDMITXRegWrite(pu32RegAddr, o_audpath_intr_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_audpath_intr_state.u32, "AUDPATH_INTR_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_AUDPATH_INTR_STATE_audpath_intr_state5Set
//  Description : Set the value of the member AUDPATH_INTR_STATE.audpath_intr_state5
//  Input       : unsigned int uaudpath_intr_state5: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUDPATH_INTR_STATE_audpath_intr_state5Set(unsigned int uaudpath_intr_state5)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AUDPATH_INTR_STATE o_audpath_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->AUDPATH_INTR_STATE.u32);
    o_audpath_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_audpath_intr_state.bits.audpath_intr_state5 = uaudpath_intr_state5;
    HDMITXRegWrite(pu32RegAddr, o_audpath_intr_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_audpath_intr_state.u32, "AUDPATH_INTR_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_AUDPATH_INTR_STATE_audpath_intr_state4Set
//  Description : Set the value of the member AUDPATH_INTR_STATE.audpath_intr_state4
//  Input       : unsigned int uaudpath_intr_state4: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUDPATH_INTR_STATE_audpath_intr_state4Set(unsigned int uaudpath_intr_state4)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AUDPATH_INTR_STATE o_audpath_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->AUDPATH_INTR_STATE.u32);
    o_audpath_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_audpath_intr_state.bits.audpath_intr_state4 = uaudpath_intr_state4;
    HDMITXRegWrite(pu32RegAddr, o_audpath_intr_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_audpath_intr_state.u32, "AUDPATH_INTR_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_AUDPATH_INTR_STATE_audpath_intr_state3Set
//  Description : Set the value of the member AUDPATH_INTR_STATE.audpath_intr_state3
//  Input       : unsigned int uaudpath_intr_state3: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUDPATH_INTR_STATE_audpath_intr_state3Set(unsigned int uaudpath_intr_state3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AUDPATH_INTR_STATE o_audpath_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->AUDPATH_INTR_STATE.u32);
    o_audpath_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_audpath_intr_state.bits.audpath_intr_state3 = uaudpath_intr_state3;
    HDMITXRegWrite(pu32RegAddr, o_audpath_intr_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_audpath_intr_state.u32, "AUDPATH_INTR_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_AUDPATH_INTR_STATE_audpath_intr_state2Set
//  Description : Set the value of the member AUDPATH_INTR_STATE.audpath_intr_state2
//  Input       : unsigned int uaudpath_intr_state2: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUDPATH_INTR_STATE_audpath_intr_state2Set(unsigned int uaudpath_intr_state2)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AUDPATH_INTR_STATE o_audpath_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->AUDPATH_INTR_STATE.u32);
    o_audpath_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_audpath_intr_state.bits.audpath_intr_state2 = uaudpath_intr_state2;
    HDMITXRegWrite(pu32RegAddr, o_audpath_intr_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_audpath_intr_state.u32, "AUDPATH_INTR_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_AUDPATH_INTR_STATE_audpath_intr_state1Set
//  Description : Set the value of the member AUDPATH_INTR_STATE.audpath_intr_state1
//  Input       : unsigned int uaudpath_intr_state1: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUDPATH_INTR_STATE_audpath_intr_state1Set(unsigned int uaudpath_intr_state1)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AUDPATH_INTR_STATE o_audpath_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->AUDPATH_INTR_STATE.u32);
    o_audpath_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_audpath_intr_state.bits.audpath_intr_state1 = uaudpath_intr_state1;
    HDMITXRegWrite(pu32RegAddr, o_audpath_intr_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_audpath_intr_state.u32, "AUDPATH_INTR_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_AUDPATH_INTR_STATE_audpath_intr_state0Set
//  Description : Set the value of the member AUDPATH_INTR_STATE.audpath_intr_state0
//  Input       : unsigned int uaudpath_intr_state0: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUDPATH_INTR_STATE_audpath_intr_state0Set(unsigned int uaudpath_intr_state0)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AUDPATH_INTR_STATE o_audpath_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->AUDPATH_INTR_STATE.u32);
    o_audpath_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_audpath_intr_state.bits.audpath_intr_state0 = uaudpath_intr_state0;
    HDMITXRegWrite(pu32RegAddr, o_audpath_intr_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_audpath_intr_state.u32, "AUDPATH_INTR_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_AUDPATH_INTR_MASK_audpath_intr_mask8Set
//  Description : Set the value of the member AUDPATH_INTR_MASK.audpath_intr_mask8
//  Input       : unsigned int uaudpath_intr_mask8: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUDPATH_INTR_MASK_audpath_intr_mask8Set(unsigned int uaudpath_intr_mask8)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AUDPATH_INTR_MASK o_audpath_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->AUDPATH_INTR_MASK.u32);
    o_audpath_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    o_audpath_intr_mask.bits.audpath_intr_mask8 = uaudpath_intr_mask8;
    HDMITXRegWrite(pu32RegAddr, o_audpath_intr_mask.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_audpath_intr_mask.u32, "AUDPATH_INTR_MASK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_AUDPATH_INTR_MASK_audpath_intr_mask7Set
//  Description : Set the value of the member AUDPATH_INTR_MASK.audpath_intr_mask7
//  Input       : unsigned int uaudpath_intr_mask7: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUDPATH_INTR_MASK_audpath_intr_mask7Set(unsigned int uaudpath_intr_mask7)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AUDPATH_INTR_MASK o_audpath_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->AUDPATH_INTR_MASK.u32);
    o_audpath_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    o_audpath_intr_mask.bits.audpath_intr_mask7 = uaudpath_intr_mask7;
    HDMITXRegWrite(pu32RegAddr, o_audpath_intr_mask.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_audpath_intr_mask.u32, "AUDPATH_INTR_MASK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_AUDPATH_INTR_MASK_audpath_intr_mask6Set
//  Description : Set the value of the member AUDPATH_INTR_MASK.audpath_intr_mask6
//  Input       : unsigned int uaudpath_intr_mask6: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUDPATH_INTR_MASK_audpath_intr_mask6Set(unsigned int uaudpath_intr_mask6)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AUDPATH_INTR_MASK o_audpath_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->AUDPATH_INTR_MASK.u32);
    o_audpath_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    o_audpath_intr_mask.bits.audpath_intr_mask6 = uaudpath_intr_mask6;
    HDMITXRegWrite(pu32RegAddr, o_audpath_intr_mask.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_audpath_intr_mask.u32, "AUDPATH_INTR_MASK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_AUDPATH_INTR_MASK_audpath_intr_mask5Set
//  Description : Set the value of the member AUDPATH_INTR_MASK.audpath_intr_mask5
//  Input       : unsigned int uaudpath_intr_mask5: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUDPATH_INTR_MASK_audpath_intr_mask5Set(unsigned int uaudpath_intr_mask5)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AUDPATH_INTR_MASK o_audpath_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->AUDPATH_INTR_MASK.u32);
    o_audpath_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    o_audpath_intr_mask.bits.audpath_intr_mask5 = uaudpath_intr_mask5;
    HDMITXRegWrite(pu32RegAddr, o_audpath_intr_mask.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_audpath_intr_mask.u32, "AUDPATH_INTR_MASK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_AUDPATH_INTR_MASK_audpath_intr_mask4Set
//  Description : Set the value of the member AUDPATH_INTR_MASK.audpath_intr_mask4
//  Input       : unsigned int uaudpath_intr_mask4: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUDPATH_INTR_MASK_audpath_intr_mask4Set(unsigned int uaudpath_intr_mask4)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AUDPATH_INTR_MASK o_audpath_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->AUDPATH_INTR_MASK.u32);
    o_audpath_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    o_audpath_intr_mask.bits.audpath_intr_mask4 = uaudpath_intr_mask4;
    HDMITXRegWrite(pu32RegAddr, o_audpath_intr_mask.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_audpath_intr_mask.u32, "AUDPATH_INTR_MASK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_AUDPATH_INTR_MASK_audpath_intr_mask3Set
//  Description : Set the value of the member AUDPATH_INTR_MASK.audpath_intr_mask3
//  Input       : unsigned int uaudpath_intr_mask3: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUDPATH_INTR_MASK_audpath_intr_mask3Set(unsigned int uaudpath_intr_mask3)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AUDPATH_INTR_MASK o_audpath_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->AUDPATH_INTR_MASK.u32);
    o_audpath_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    o_audpath_intr_mask.bits.audpath_intr_mask3 = uaudpath_intr_mask3;
    HDMITXRegWrite(pu32RegAddr, o_audpath_intr_mask.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_audpath_intr_mask.u32, "AUDPATH_INTR_MASK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_AUDPATH_INTR_MASK_audpath_intr_mask2Set
//  Description : Set the value of the member AUDPATH_INTR_MASK.audpath_intr_mask2
//  Input       : unsigned int uaudpath_intr_mask2: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUDPATH_INTR_MASK_audpath_intr_mask2Set(unsigned int uaudpath_intr_mask2)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AUDPATH_INTR_MASK o_audpath_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->AUDPATH_INTR_MASK.u32);
    o_audpath_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    o_audpath_intr_mask.bits.audpath_intr_mask2 = uaudpath_intr_mask2;
    HDMITXRegWrite(pu32RegAddr, o_audpath_intr_mask.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_audpath_intr_mask.u32, "AUDPATH_INTR_MASK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_AUDPATH_INTR_MASK_audpath_intr_mask1Set
//  Description : Set the value of the member AUDPATH_INTR_MASK.audpath_intr_mask1
//  Input       : unsigned int uaudpath_intr_mask1: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUDPATH_INTR_MASK_audpath_intr_mask1Set(unsigned int uaudpath_intr_mask1)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AUDPATH_INTR_MASK o_audpath_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->AUDPATH_INTR_MASK.u32);
    o_audpath_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    o_audpath_intr_mask.bits.audpath_intr_mask1 = uaudpath_intr_mask1;
    HDMITXRegWrite(pu32RegAddr, o_audpath_intr_mask.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_audpath_intr_mask.u32, "AUDPATH_INTR_MASK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_AUDPATH_INTR_MASK_audpath_intr_mask0Set
//  Description : Set the value of the member AUDPATH_INTR_MASK.audpath_intr_mask0
//  Input       : unsigned int uaudpath_intr_mask0: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUDPATH_INTR_MASK_audpath_intr_mask0Set(unsigned int uaudpath_intr_mask0)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AUDPATH_INTR_MASK o_audpath_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->AUDPATH_INTR_MASK.u32);
    o_audpath_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    o_audpath_intr_mask.bits.audpath_intr_mask0 = uaudpath_intr_mask0;
    HDMITXRegWrite(pu32RegAddr, o_audpath_intr_mask.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_audpath_intr_mask.u32, "AUDPATH_INTR_MASK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TXHDMI_INTR_STATE_txhdmi_intr_state0Set
//  Description : Set the value of the member TXHDMI_INTR_STATE.txhdmi_intr_state0
//  Input       : unsigned int utxhdmi_intr_state0: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TXHDMI_INTR_STATE_txhdmi_intr_state0Set(unsigned int utxhdmi_intr_state0)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TXHDMI_INTR_STATE o_txhdmi_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TXHDMI_INTR_STATE.u32);
    o_txhdmi_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_txhdmi_intr_state.bits.txhdmi_intr_state0 = utxhdmi_intr_state0;
    HDMITXRegWrite(pu32RegAddr, o_txhdmi_intr_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_txhdmi_intr_state.u32, "TXHDMI_INTR_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_TXHDMI_INTR_MASK_txhdmi_intr_mask0Set
//  Description : Set the value of the member TXHDMI_INTR_MASK.txhdmi_intr_mask0
//  Input       : unsigned int utxhdmi_intr_mask0: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TXHDMI_INTR_MASK_txhdmi_intr_mask0Set(unsigned int utxhdmi_intr_mask0)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TXHDMI_INTR_MASK o_txhdmi_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TXHDMI_INTR_MASK.u32);
    o_txhdmi_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    o_txhdmi_intr_mask.bits.txhdmi_intr_mask0 = utxhdmi_intr_mask0;
    HDMITXRegWrite(pu32RegAddr, o_txhdmi_intr_mask.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_txhdmi_intr_mask.u32, "TXHDMI_INTR_MASK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP_INTR_STATE_hdcp_intr_state1Set
//  Description : Set the value of the member HDCP_INTR_STATE.hdcp_intr_state1
//  Input       : unsigned int uhdcp_intr_state1: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP_INTR_STATE_hdcp_intr_state1Set(unsigned int uhdcp_intr_state1)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDCP_INTR_STATE o_hdcp_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->HDCP_INTR_STATE.u32);
    o_hdcp_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp_intr_state.bits.hdcp_intr_state1 = uhdcp_intr_state1;
    HDMITXRegWrite(pu32RegAddr, o_hdcp_intr_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp_intr_state.u32, "HDCP_INTR_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP_INTR_STATE_hdcp_intr_state0Set
//  Description : Set the value of the member HDCP_INTR_STATE.hdcp_intr_state0
//  Input       : unsigned int uhdcp_intr_state0: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP_INTR_STATE_hdcp_intr_state0Set(unsigned int uhdcp_intr_state0)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDCP_INTR_STATE o_hdcp_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->HDCP_INTR_STATE.u32);
    o_hdcp_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp_intr_state.bits.hdcp_intr_state0 = uhdcp_intr_state0;
    HDMITXRegWrite(pu32RegAddr, o_hdcp_intr_state.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp_intr_state.u32, "HDCP_INTR_STATE");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP_INTR_MASK_hdcp_intr_mask1Set
//  Description : Set the value of the member HDCP_INTR_MASK.hdcp_intr_mask1
//  Input       : unsigned int uhdcp_intr_mask1: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP_INTR_MASK_hdcp_intr_mask1Set(unsigned int uhdcp_intr_mask1)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDCP_INTR_MASK o_hdcp_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->HDCP_INTR_MASK.u32);
    o_hdcp_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp_intr_mask.bits.hdcp_intr_mask1 = uhdcp_intr_mask1;
    HDMITXRegWrite(pu32RegAddr, o_hdcp_intr_mask.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp_intr_mask.u32, "HDCP_INTR_MASK");

    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : HDMI_HDCP_INTR_MASK_hdcp_intr_mask0Set
//  Description : Set the value of the member HDCP_INTR_MASK.hdcp_intr_mask0
//  Input       : unsigned int uhdcp_intr_mask0: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP_INTR_MASK_hdcp_intr_mask0Set(unsigned int uhdcp_intr_mask0)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDCP_INTR_MASK o_hdcp_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->HDCP_INTR_MASK.u32);
    o_hdcp_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    o_hdcp_intr_mask.bits.hdcp_intr_mask0 = uhdcp_intr_mask0;
    HDMITXRegWrite(pu32RegAddr, o_hdcp_intr_mask.u32);
    HDMITXVerifyRegWrite(pu32RegAddr, o_hdcp_intr_mask.u32, "HDCP_INTR_MASK");

    return HI_SUCCESS;
}


//******************************************************************************
//  Function    : HDMI_TX_PWD_RST_CTRL_tx_mcu_srst_reqGet
//  Description : Set the value of the member TX_PWD_RST_CTRL.tx_mcu_srst_req
//  Input       : unsigned int utx_mcu_srst_req: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_PWD_RST_CTRL_tx_mcu_srst_reqGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TX_PWD_RST_CTRL o_tx_pwd_rst_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TX_PWD_RST_CTRL.u32);
    o_tx_pwd_rst_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_pwd_rst_ctrl.bits.tx_mcu_srst_req;
}

//******************************************************************************
//  Function    : HDMI_TX_PWD_RST_CTRL_tx_hdcp2x_srst_reqGet
//  Description : Set the value of the member TX_PWD_RST_CTRL.tx_hdcp2x_srst_req
//  Input       : unsigned int utx_hdcp2x_srst_req: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_PWD_RST_CTRL_tx_hdcp2x_srst_reqGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TX_PWD_RST_CTRL o_tx_pwd_rst_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TX_PWD_RST_CTRL.u32);
    o_tx_pwd_rst_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_pwd_rst_ctrl.bits.tx_hdcp2x_srst_req;
}

//******************************************************************************
//  Function    : HDMI_TX_PWD_RST_CTRL_tx_afifo_srst_reqGet
//  Description : Set the value of the member TX_PWD_RST_CTRL.tx_afifo_srst_req
//  Input       : unsigned int utx_afifo_srst_req: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_PWD_RST_CTRL_tx_afifo_srst_reqGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TX_PWD_RST_CTRL o_tx_pwd_rst_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TX_PWD_RST_CTRL.u32);
    o_tx_pwd_rst_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_pwd_rst_ctrl.bits.tx_afifo_srst_req;
}

//******************************************************************************
//  Function    : HDMI_TX_PWD_RST_CTRL_tx_acr_srst_reqGet
//  Description : Set the value of the member TX_PWD_RST_CTRL.tx_acr_srst_req
//  Input       : unsigned int utx_acr_srst_req: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_PWD_RST_CTRL_tx_acr_srst_reqGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TX_PWD_RST_CTRL o_tx_pwd_rst_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TX_PWD_RST_CTRL.u32);
    o_tx_pwd_rst_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_pwd_rst_ctrl.bits.tx_acr_srst_req;
}

//******************************************************************************
//  Function    : HDMI_TX_PWD_RST_CTRL_tx_aud_srst_reqGet
//  Description : Set the value of the member TX_PWD_RST_CTRL.tx_aud_srst_req
//  Input       : unsigned int utx_aud_srst_req: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_PWD_RST_CTRL_tx_aud_srst_reqGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TX_PWD_RST_CTRL o_tx_pwd_rst_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TX_PWD_RST_CTRL.u32);
    o_tx_pwd_rst_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_pwd_rst_ctrl.bits.tx_aud_srst_req;
}

//******************************************************************************
//  Function    : HDMI_TX_PWD_RST_CTRL_tx_phy_srst_reqGet
//  Description : Set the value of the member TX_PWD_RST_CTRL.tx_phy_srst_req
//  Input       : unsigned int utx_phy_srst_req: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_PWD_RST_CTRL_tx_phy_srst_reqGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TX_PWD_RST_CTRL o_tx_pwd_rst_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TX_PWD_RST_CTRL.u32);
    o_tx_pwd_rst_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_pwd_rst_ctrl.bits.tx_phy_srst_req;
}

//******************************************************************************
//  Function    : HDMI_TX_PWD_RST_CTRL_tx_hdcp1x_srst_reqGet
//  Description : Set the value of the member TX_PWD_RST_CTRL.tx_hdcp1x_srst_req
//  Input       : unsigned int utx_hdcp1x_srst_req: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_PWD_RST_CTRL_tx_hdcp1x_srst_reqGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TX_PWD_RST_CTRL o_tx_pwd_rst_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TX_PWD_RST_CTRL.u32);
    o_tx_pwd_rst_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_pwd_rst_ctrl.bits.tx_hdcp1x_srst_req;
}

//******************************************************************************
//  Function    : HDMI_TX_PWD_RST_CTRL_tx_hdmi_srst_reqGet
//  Description : Set the value of the member TX_PWD_RST_CTRL.tx_hdmi_srst_req
//  Input       : unsigned int utx_hdmi_srst_req: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_PWD_RST_CTRL_tx_hdmi_srst_reqGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TX_PWD_RST_CTRL o_tx_pwd_rst_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TX_PWD_RST_CTRL.u32);
    o_tx_pwd_rst_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_pwd_rst_ctrl.bits.tx_hdmi_srst_req;
}

//******************************************************************************
//  Function    : HDMI_TX_PWD_RST_CTRL_tx_vid_srst_reqGet
//  Description : Set the value of the member TX_PWD_RST_CTRL.tx_vid_srst_req
//  Input       : unsigned int utx_vid_srst_req: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_PWD_RST_CTRL_tx_vid_srst_reqGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TX_PWD_RST_CTRL o_tx_pwd_rst_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TX_PWD_RST_CTRL.u32);
    o_tx_pwd_rst_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_pwd_rst_ctrl.bits.tx_vid_srst_req;
}

//******************************************************************************
//  Function    : HDMI_TX_PWD_RST_CTRL_tx_sys_srst_reqGet
//  Description : Set the value of the member TX_PWD_RST_CTRL.tx_sys_srst_req
//  Input       : unsigned int utx_sys_srst_req: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_PWD_RST_CTRL_tx_sys_srst_reqGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TX_PWD_RST_CTRL o_tx_pwd_rst_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TX_PWD_RST_CTRL.u32);
    o_tx_pwd_rst_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_pwd_rst_ctrl.bits.tx_sys_srst_req;
}

//******************************************************************************
//  Function    : HDMI_TX_PWD_RST_CTRL_tx_pwd_srst_reqGet
//  Description : Set the value of the member TX_PWD_RST_CTRL.tx_pwd_srst_req
//  Input       : unsigned int utx_pwd_srst_req: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_PWD_RST_CTRL_tx_pwd_srst_reqGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TX_PWD_RST_CTRL o_tx_pwd_rst_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TX_PWD_RST_CTRL.u32);
    o_tx_pwd_rst_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_pwd_rst_ctrl.bits.tx_pwd_srst_req;
}

//******************************************************************************
//  Function    : HDMI_SCDC_FSM_CTRL_scdc_stall_reqGet
//  Description : Set the value of the member SCDC_FSM_CTRL.scdc_stall_req
//  Input       : unsigned int uscdc_stall_req: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_SCDC_FSM_CTRL_scdc_stall_reqGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SCDC_FSM_CTRL o_scdc_fsm_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->SCDC_FSM_CTRL.u32);
    o_scdc_fsm_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_scdc_fsm_ctrl.bits.scdc_stall_req;
}

//******************************************************************************
//  Function    : HDMI_SCDC_FSM_CTRL_scdc_hdcp_det_enGet
//  Description : Set the value of the member SCDC_FSM_CTRL.scdc_hdcp_det_en
//  Input       : unsigned int uscdc_hdcp_det_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_SCDC_FSM_CTRL_scdc_hdcp_det_enGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SCDC_FSM_CTRL o_scdc_fsm_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->SCDC_FSM_CTRL.u32);
    o_scdc_fsm_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_scdc_fsm_ctrl.bits.scdc_hdcp_det_en;
}

//******************************************************************************
//  Function    : HDMI_SCDC_FSM_CTRL_scdc_poll_selGet
//  Description : Set the value of the member SCDC_FSM_CTRL.scdc_poll_sel
//  Input       : unsigned int uscdc_poll_sel: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_SCDC_FSM_CTRL_scdc_poll_selGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SCDC_FSM_CTRL o_scdc_fsm_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->SCDC_FSM_CTRL.u32);
    o_scdc_fsm_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_scdc_fsm_ctrl.bits.scdc_poll_sel;
}

//******************************************************************************
//  Function    : HDMI_SCDC_FSM_CTRL_scdc_auto_reply_stopGet
//  Description : Set the value of the member SCDC_FSM_CTRL.scdc_auto_reply_stop
//  Input       : unsigned int uscdc_auto_reply_stop: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_SCDC_FSM_CTRL_scdc_auto_reply_stopGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SCDC_FSM_CTRL o_scdc_fsm_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->SCDC_FSM_CTRL.u32);
    o_scdc_fsm_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_scdc_fsm_ctrl.bits.scdc_auto_reply_stop;
}

//******************************************************************************
//  Function    : HDMI_SCDC_FSM_CTRL_scdc_auto_pollGet
//  Description : Set the value of the member SCDC_FSM_CTRL.scdc_auto_poll
//  Input       : unsigned int uscdc_auto_poll: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_SCDC_FSM_CTRL_scdc_auto_pollGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SCDC_FSM_CTRL o_scdc_fsm_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->SCDC_FSM_CTRL.u32);
    o_scdc_fsm_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_scdc_fsm_ctrl.bits.scdc_auto_poll;
}

//******************************************************************************
//  Function    : HDMI_SCDC_FSM_CTRL_scdc_auto_replyGet
//  Description : Set the value of the member SCDC_FSM_CTRL.scdc_auto_reply
//  Input       : unsigned int uscdc_auto_reply: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_SCDC_FSM_CTRL_scdc_auto_replyGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SCDC_FSM_CTRL o_scdc_fsm_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->SCDC_FSM_CTRL.u32);
    o_scdc_fsm_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_scdc_fsm_ctrl.bits.scdc_auto_reply;
}

//******************************************************************************
//  Function    : HDMI_SCDC_FSM_CTRL_scdc_access_enGet
//  Description : Set the value of the member SCDC_FSM_CTRL.scdc_access_en
//  Input       : unsigned int uscdc_access_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_SCDC_FSM_CTRL_scdc_access_enGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SCDC_FSM_CTRL o_scdc_fsm_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->SCDC_FSM_CTRL.u32);
    o_scdc_fsm_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_scdc_fsm_ctrl.bits.scdc_access_en;
}

//******************************************************************************
//  Function    : HDMI_SCDC_FSM_CTRL_scdc_ddcm_abortGet
//  Description : Set the value of the member SCDC_FSM_CTRL.scdc_ddcm_abort
//  Input       : unsigned int uscdc_ddcm_abort: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_SCDC_FSM_CTRL_scdc_ddcm_abortGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SCDC_FSM_CTRL o_scdc_fsm_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->SCDC_FSM_CTRL.u32);
    o_scdc_fsm_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_scdc_fsm_ctrl.bits.scdc_ddcm_abort;
}

//******************************************************************************
//  Function    : HDMI_SCDC_POLL_TIMER_scdc_poll_timerGet
//  Description : Set the value of the member SCDC_POLL_TIMER.scdc_poll_timer
//  Input       : unsigned int uscdc_poll_timer: 22 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_SCDC_POLL_TIMER_scdc_poll_timerGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SCDC_POLL_TIMER o_scdc_poll_timer;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->SCDC_POLL_TIMER.u32);
    o_scdc_poll_timer.u32 = HDMITXRegRead(pu32RegAddr);
    return o_scdc_poll_timer.bits.scdc_poll_timer;
}

//******************************************************************************
//  Function    : HDMI_SCDC_FSM_STATE_scdc_rreq_in_progGet
//  Description : Set the value of the member SCDC_FSM_STATE.scdc_rreq_in_prog
//  Input       : unsigned int uscdc_rreq_in_prog: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_SCDC_FSM_STATE_scdc_rreq_in_progGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SCDC_FSM_STATE o_scdc_fsm_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->SCDC_FSM_STATE.u32);
    o_scdc_fsm_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_scdc_fsm_state.bits.scdc_rreq_in_prog;
}

//******************************************************************************
//  Function    : HDMI_SCDC_FSM_STATE_scdc_in_progGet
//  Description : Set the value of the member SCDC_FSM_STATE.scdc_in_prog
//  Input       : unsigned int uscdc_in_prog: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_SCDC_FSM_STATE_scdc_in_progGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SCDC_FSM_STATE o_scdc_fsm_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->SCDC_FSM_STATE.u32);
    o_scdc_fsm_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_scdc_fsm_state.bits.scdc_in_prog;
}

//******************************************************************************
//  Function    : HDMI_SCDC_FSM_STATE_scdc_activeGet
//  Description : Set the value of the member SCDC_FSM_STATE.scdc_active
//  Input       : unsigned int uscdc_active: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_SCDC_FSM_STATE_scdc_activeGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SCDC_FSM_STATE o_scdc_fsm_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->SCDC_FSM_STATE.u32);
    o_scdc_fsm_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_scdc_fsm_state.bits.scdc_active;
}

//******************************************************************************
//  Function    : HDMI_SCDC_FSM_STATE_scdc_rreq_stateGet
//  Description : Set the value of the member SCDC_FSM_STATE.scdc_rreq_state
//  Input       : unsigned int uscdc_rreq_state: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_SCDC_FSM_STATE_scdc_rreq_stateGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SCDC_FSM_STATE o_scdc_fsm_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->SCDC_FSM_STATE.u32);
    o_scdc_fsm_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_scdc_fsm_state.bits.scdc_rreq_state;
}

//******************************************************************************
//  Function    : HDMI_SCDC_FSM_STATE_scdc_fsm_stateGet
//  Description : Set the value of the member SCDC_FSM_STATE.scdc_fsm_state
//  Input       : unsigned int uscdc_fsm_state: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_SCDC_FSM_STATE_scdc_fsm_stateGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SCDC_FSM_STATE o_scdc_fsm_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->SCDC_FSM_STATE.u32);
    o_scdc_fsm_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_scdc_fsm_state.bits.scdc_fsm_state;
}

//******************************************************************************
//  Function    : HDMI_SCDC_FLAG_BTYE_scdc_flag_byte1Get
//  Description : Set the value of the member SCDC_FLAG_BTYE.scdc_flag_byte1
//  Input       : unsigned int uscdc_flag_byte1: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_SCDC_FLAG_BTYE_scdc_flag_byte1Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SCDC_FLAG_BTYE o_scdc_flag_btye;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->SCDC_FLAG_BTYE.u32);
    o_scdc_flag_btye.u32 = HDMITXRegRead(pu32RegAddr);
    return o_scdc_flag_btye.bits.scdc_flag_byte1;
}

//******************************************************************************
//  Function    : HDMI_SCDC_FLAG_BTYE_scdc_flag_byte0Get
//  Description : Set the value of the member SCDC_FLAG_BTYE.scdc_flag_byte0
//  Input       : unsigned int uscdc_flag_byte0: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_SCDC_FLAG_BTYE_scdc_flag_byte0Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_SCDC_FLAG_BTYE o_scdc_flag_btye;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->SCDC_FLAG_BTYE.u32);
    o_scdc_flag_btye.u32 = HDMITXRegRead(pu32RegAddr);
    return o_scdc_flag_btye.bits.scdc_flag_byte0;
}

//******************************************************************************
//  Function    : HDMI_PWD_FIFO_RDATA_pwd_fifo_data_outGet
//  Description : Set the value of the member PWD_FIFO_RDATA.pwd_fifo_data_out
//  Input       : unsigned int upwd_fifo_data_out: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_FIFO_RDATA_pwd_fifo_data_outGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_FIFO_RDATA o_pwd_fifo_rdata;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_FIFO_RDATA.u32);
    o_pwd_fifo_rdata.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pwd_fifo_rdata.bits.pwd_fifo_data_out;
}

//******************************************************************************
//  Function    : HDMI_PWD_FIFO_WDATA_pwd_fifo_data_inGet
//  Description : Set the value of the member PWD_FIFO_WDATA.pwd_fifo_data_in
//  Input       : unsigned int upwd_fifo_data_in: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_FIFO_WDATA_pwd_fifo_data_inGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_FIFO_WDATA o_pwd_fifo_wdata;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_FIFO_WDATA.u32);
    o_pwd_fifo_wdata.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pwd_fifo_wdata.bits.pwd_fifo_data_in;
}

//******************************************************************************
//  Function    : HDMI_PWD_DATA_CNT_pwd_data_out_cntGet
//  Description : Set the value of the member PWD_DATA_CNT.pwd_data_out_cnt
//  Input       : unsigned int upwd_data_out_cnt: 10 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_DATA_CNT_pwd_data_out_cntGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_DATA_CNT o_pwd_data_cnt;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_DATA_CNT.u32);
    o_pwd_data_cnt.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pwd_data_cnt.bits.pwd_data_out_cnt;
}

//******************************************************************************
//  Function    : HDMI_PWD_DATA_CNT_pwd_fifo_data_cntGet
//  Description : Set the value of the member PWD_DATA_CNT.pwd_fifo_data_cnt
//  Input       : unsigned int upwd_fifo_data_cnt: 5 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_DATA_CNT_pwd_fifo_data_cntGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_DATA_CNT o_pwd_data_cnt;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_DATA_CNT.u32);
    o_pwd_data_cnt.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pwd_data_cnt.bits.pwd_fifo_data_cnt;
}

//******************************************************************************
//  Function    : HDMI_PWD_SLAVE_CFG_pwd_slave_segGet
//  Description : Set the value of the member PWD_SLAVE_CFG.pwd_slave_seg
//  Input       : unsigned int upwd_slave_seg: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_SLAVE_CFG_pwd_slave_segGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_SLAVE_CFG o_pwd_slave_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_SLAVE_CFG.u32);
    o_pwd_slave_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pwd_slave_cfg.bits.pwd_slave_seg;
}

//******************************************************************************
//  Function    : HDMI_PWD_SLAVE_CFG_pwd_slave_offsetGet
//  Description : Set the value of the member PWD_SLAVE_CFG.pwd_slave_offset
//  Input       : unsigned int upwd_slave_offset: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_SLAVE_CFG_pwd_slave_offsetGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_SLAVE_CFG o_pwd_slave_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_SLAVE_CFG.u32);
    o_pwd_slave_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pwd_slave_cfg.bits.pwd_slave_offset;
}

//******************************************************************************
//  Function    : HDMI_PWD_SLAVE_CFG_pwd_slave_addrGet
//  Description : Set the value of the member PWD_SLAVE_CFG.pwd_slave_addr
//  Input       : unsigned int upwd_slave_addr: 8 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_SLAVE_CFG_pwd_slave_addrGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_SLAVE_CFG o_pwd_slave_cfg;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_SLAVE_CFG.u32);
    o_pwd_slave_cfg.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pwd_slave_cfg.bits.pwd_slave_addr;
}

//******************************************************************************
//  Function    : HDMI_PWD_MST_STATE_pwd_fifo_fullGet
//  Description : Set the value of the member PWD_MST_STATE.pwd_fifo_full
//  Input       : unsigned int upwd_fifo_full: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_MST_STATE_pwd_fifo_fullGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_MST_STATE o_pwd_mst_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_MST_STATE.u32);
    o_pwd_mst_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pwd_mst_state.bits.pwd_fifo_full;
}

//******************************************************************************
//  Function    : HDMI_PWD_MST_STATE_pwd_fifo_half_fullGet
//  Description : Set the value of the member PWD_MST_STATE.pwd_fifo_half_full
//  Input       : unsigned int upwd_fifo_half_full: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_MST_STATE_pwd_fifo_half_fullGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_MST_STATE o_pwd_mst_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_MST_STATE.u32);
    o_pwd_mst_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pwd_mst_state.bits.pwd_fifo_half_full;
}

//******************************************************************************
//  Function    : HDMI_PWD_MST_STATE_pwd_fifo_emptyGet
//  Description : Set the value of the member PWD_MST_STATE.pwd_fifo_empty
//  Input       : unsigned int upwd_fifo_empty: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_MST_STATE_pwd_fifo_emptyGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_MST_STATE o_pwd_mst_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_MST_STATE.u32);
    o_pwd_mst_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pwd_mst_state.bits.pwd_fifo_empty;
}

//******************************************************************************
//  Function    : HDMI_PWD_MST_STATE_pwd_fifo_rd_in_useGet
//  Description : Set the value of the member PWD_MST_STATE.pwd_fifo_rd_in_use
//  Input       : unsigned int upwd_fifo_rd_in_use: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_MST_STATE_pwd_fifo_rd_in_useGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_MST_STATE o_pwd_mst_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_MST_STATE.u32);
    o_pwd_mst_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pwd_mst_state.bits.pwd_fifo_rd_in_use;
}

//******************************************************************************
//  Function    : HDMI_PWD_MST_STATE_pwd_fifo_wr_in_useGet
//  Description : Set the value of the member PWD_MST_STATE.pwd_fifo_wr_in_use
//  Input       : unsigned int upwd_fifo_wr_in_use: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_MST_STATE_pwd_fifo_wr_in_useGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_MST_STATE o_pwd_mst_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_MST_STATE.u32);
    o_pwd_mst_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pwd_mst_state.bits.pwd_fifo_wr_in_use;
}

//******************************************************************************
//  Function    : HDMI_PWD_MST_STATE_pwd_i2c_in_progGet
//  Description : Set the value of the member PWD_MST_STATE.pwd_i2c_in_prog
//  Input       : unsigned int upwd_i2c_in_prog: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_MST_STATE_pwd_i2c_in_progGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_MST_STATE o_pwd_mst_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_MST_STATE.u32);
    o_pwd_mst_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pwd_mst_state.bits.pwd_i2c_in_prog;
}

//******************************************************************************
//  Function    : HDMI_PWD_MST_STATE_pwd_i2c_bus_lowGet
//  Description : Set the value of the member PWD_MST_STATE.pwd_i2c_bus_low
//  Input       : unsigned int upwd_i2c_bus_low: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_MST_STATE_pwd_i2c_bus_lowGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_MST_STATE o_pwd_mst_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_MST_STATE.u32);
    o_pwd_mst_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pwd_mst_state.bits.pwd_i2c_bus_low;
}

//******************************************************************************
//  Function    : HDMI_PWD_MST_STATE_pwd_i2c_no_ackGet
//  Description : Set the value of the member PWD_MST_STATE.pwd_i2c_no_ack
//  Input       : unsigned int upwd_i2c_no_ack: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_MST_STATE_pwd_i2c_no_ackGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_MST_STATE o_pwd_mst_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_MST_STATE.u32);
    o_pwd_mst_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pwd_mst_state.bits.pwd_i2c_no_ack;
}

//******************************************************************************
//  Function    : HDMI_PWD_MST_CMD_pwd_mst_cmdGet
//  Description : Set the value of the member PWD_MST_CMD.pwd_mst_cmd
//  Input       : unsigned int upwd_mst_cmd: 4 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_MST_CMD_pwd_mst_cmdGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_MST_CMD o_pwd_mst_cmd;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_MST_CMD.u32);
    o_pwd_mst_cmd.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pwd_mst_cmd.bits.pwd_mst_cmd;
}

//******************************************************************************
//  Function    : HDMI_PWD_MST_CLR_pwd_clr_no_ackGet
//  Description : Set the value of the member PWD_MST_CLR.pwd_clr_no_ack
//  Input       : unsigned int upwd_clr_no_ack: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_MST_CLR_pwd_clr_no_ackGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_MST_CLR o_pwd_mst_clr;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_MST_CLR.u32);
    o_pwd_mst_clr.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pwd_mst_clr.bits.pwd_clr_no_ack;
}

//******************************************************************************
//  Function    : HDMI_PWD_MST_CLR_pwd_clr_bus_lowGet
//  Description : Set the value of the member PWD_MST_CLR.pwd_clr_bus_low
//  Input       : unsigned int upwd_clr_bus_low: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_MST_CLR_pwd_clr_bus_lowGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_MST_CLR o_pwd_mst_clr;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_MST_CLR.u32);
    o_pwd_mst_clr.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pwd_mst_clr.bits.pwd_clr_bus_low;
}

//******************************************************************************
//  Function    : HDMI_DDC_MST_ARB_CTRL_reg_auto_abort_enGet
//  Description : Set the value of the member DDC_MST_ARB_CTRL.reg_auto_abort_en
//  Input       : unsigned int ureg_auto_abort_en: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_MST_ARB_CTRL_reg_auto_abort_enGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_DDC_MST_ARB_CTRL o_ddc_mst_arb_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->DDC_MST_ARB_CTRL.u32);
    o_ddc_mst_arb_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_ddc_mst_arb_ctrl.bits.reg_auto_abort_en;
}

//******************************************************************************
//  Function    : HDMI_DDC_MST_ARB_CTRL_cpu_ddc_force_reqGet
//  Description : Set the value of the member DDC_MST_ARB_CTRL.cpu_ddc_force_req
//  Input       : unsigned int ucpu_ddc_force_req: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_MST_ARB_CTRL_cpu_ddc_force_reqGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_DDC_MST_ARB_CTRL o_ddc_mst_arb_ctrl;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->DDC_MST_ARB_CTRL.u32);
    o_ddc_mst_arb_ctrl.u32 = HDMITXRegRead(pu32RegAddr);
    return o_ddc_mst_arb_ctrl.bits.cpu_ddc_force_req;
}

//******************************************************************************
//  Function    : HDMI_DDC_MST_ARB_REQ_cpu_ddc_reqGet
//  Description : Set the value of the member DDC_MST_ARB_REQ.cpu_ddc_req
//  Input       : unsigned int ucpu_ddc_req: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_MST_ARB_REQ_cpu_ddc_reqGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_DDC_MST_ARB_REQ o_ddc_mst_arb_req;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->DDC_MST_ARB_REQ.u32);
    o_ddc_mst_arb_req.u32 = HDMITXRegRead(pu32RegAddr);
    return o_ddc_mst_arb_req.bits.cpu_ddc_req;
}

//******************************************************************************
//  Function    : HDMI_DDC_MST_ARB_ACK_cpu_ddc_req_ackGet
//  Description : Set the value of the member DDC_MST_ARB_ACK.cpu_ddc_req_ack
//  Input       : unsigned int ucpu_ddc_req_ack: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_MST_ARB_ACK_cpu_ddc_req_ackGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_DDC_MST_ARB_ACK o_ddc_mst_arb_ack;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->DDC_MST_ARB_ACK.u32);
    o_ddc_mst_arb_ack.u32 = HDMITXRegRead(pu32RegAddr);
    return o_ddc_mst_arb_ack.bits.cpu_ddc_req_ack;
}

//******************************************************************************
//  Function    : HDMI_DDC_MST_ARB_STATE_ddc_arb_stateGet
//  Description : Set the value of the member DDC_MST_ARB_STATE.ddc_arb_state
//  Input       : unsigned int uddc_arb_state: 9 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_DDC_MST_ARB_STATE_ddc_arb_stateGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_DDC_MST_ARB_STATE o_ddc_mst_arb_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->DDC_MST_ARB_STATE.u32);
    o_ddc_mst_arb_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_ddc_mst_arb_state.bits.ddc_arb_state;
}

//******************************************************************************
//  Function    : HDMI_TX_PWD_INTR_STATE_tx_pwd_intr_stateGet
//  Description : Set the value of the member TX_PWD_INTR_STATE.tx_pwd_intr_state
//  Input       : unsigned int utx_pwd_intr_state: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TX_PWD_INTR_STATE_tx_pwd_intr_stateGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TX_PWD_INTR_STATE o_tx_pwd_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TX_PWD_INTR_STATE.u32);
    o_tx_pwd_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_tx_pwd_intr_state.bits.tx_pwd_intr_state;
}

//******************************************************************************
//  Function    : HDMI_PWD_SUB_INTR_STATE_hdcp2x_intr_stateGet
//  Description : Set the value of the member PWD_SUB_INTR_STATE.hdcp2x_intr_state
//  Input       : unsigned int uhdcp2x_intr_state: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_SUB_INTR_STATE_hdcp2x_intr_stateGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_SUB_INTR_STATE o_pwd_sub_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_SUB_INTR_STATE.u32);
    o_pwd_sub_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pwd_sub_intr_state.bits.hdcp2x_intr_state;
}

//******************************************************************************
//  Function    : HDMI_PWD_SUB_INTR_STATE_txhdcp_intr_stateGet
//  Description : Set the value of the member PWD_SUB_INTR_STATE.txhdcp_intr_state
//  Input       : unsigned int utxhdcp_intr_state: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_SUB_INTR_STATE_txhdcp_intr_stateGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_SUB_INTR_STATE o_pwd_sub_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_SUB_INTR_STATE.u32);
    o_pwd_sub_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pwd_sub_intr_state.bits.txhdcp_intr_state;
}

//******************************************************************************
//  Function    : HDMI_PWD_SUB_INTR_STATE_txhdmi_intr_stateGet
//  Description : Set the value of the member PWD_SUB_INTR_STATE.txhdmi_intr_state
//  Input       : unsigned int utxhdmi_intr_state: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_SUB_INTR_STATE_txhdmi_intr_stateGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_SUB_INTR_STATE o_pwd_sub_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_SUB_INTR_STATE.u32);
    o_pwd_sub_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pwd_sub_intr_state.bits.txhdmi_intr_state;
}

//******************************************************************************
//  Function    : HDMI_PWD_SUB_INTR_STATE_audpath_intr_stateGet
//  Description : Set the value of the member PWD_SUB_INTR_STATE.audpath_intr_state
//  Input       : unsigned int uaudpath_intr_state: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_SUB_INTR_STATE_audpath_intr_stateGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_SUB_INTR_STATE o_pwd_sub_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_SUB_INTR_STATE.u32);
    o_pwd_sub_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pwd_sub_intr_state.bits.audpath_intr_state;
}

//******************************************************************************
//  Function    : HDMI_PWD_SUB_INTR_STATE_vidpath_intr_stateGet
//  Description : Set the value of the member PWD_SUB_INTR_STATE.vidpath_intr_state
//  Input       : unsigned int uvidpath_intr_state: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_SUB_INTR_STATE_vidpath_intr_stateGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_SUB_INTR_STATE o_pwd_sub_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_SUB_INTR_STATE.u32);
    o_pwd_sub_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pwd_sub_intr_state.bits.vidpath_intr_state;
}

//******************************************************************************
//  Function    : HDMI_PWD_SUB_INTR_STATE_tx_sys_intr_stateGet
//  Description : Set the value of the member PWD_SUB_INTR_STATE.tx_sys_intr_state
//  Input       : unsigned int utx_sys_intr_state: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_SUB_INTR_STATE_tx_sys_intr_stateGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_SUB_INTR_STATE o_pwd_sub_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_SUB_INTR_STATE.u32);
    o_pwd_sub_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pwd_sub_intr_state.bits.tx_sys_intr_state;
}

//******************************************************************************
//  Function    : HDMI_PWD_SUB_INTR_MASK_hdcp2x_intr_maskGet
//  Description : Set the value of the member PWD_SUB_INTR_MASK.hdcp2x_intr_mask
//  Input       : unsigned int uhdcp2x_intr_mask: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_SUB_INTR_MASK_hdcp2x_intr_maskGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_SUB_INTR_MASK o_pwd_sub_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_SUB_INTR_MASK.u32);
    o_pwd_sub_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pwd_sub_intr_mask.bits.hdcp2x_intr_mask;
}

//******************************************************************************
//  Function    : HDMI_PWD_SUB_INTR_MASK_txhdcp_intr_maskGet
//  Description : Set the value of the member PWD_SUB_INTR_MASK.txhdcp_intr_mask
//  Input       : unsigned int utxhdcp_intr_mask: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_SUB_INTR_MASK_txhdcp_intr_maskGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_SUB_INTR_MASK o_pwd_sub_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_SUB_INTR_MASK.u32);
    o_pwd_sub_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pwd_sub_intr_mask.bits.txhdcp_intr_mask;
}

//******************************************************************************
//  Function    : HDMI_PWD_SUB_INTR_MASK_txhdmi_intr_maskGet
//  Description : Set the value of the member PWD_SUB_INTR_MASK.txhdmi_intr_mask
//  Input       : unsigned int utxhdmi_intr_mask: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_SUB_INTR_MASK_txhdmi_intr_maskGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_SUB_INTR_MASK o_pwd_sub_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_SUB_INTR_MASK.u32);
    o_pwd_sub_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pwd_sub_intr_mask.bits.txhdmi_intr_mask;
}

//******************************************************************************
//  Function    : HDMI_PWD_SUB_INTR_MASK_audpath_intr_maskGet
//  Description : Set the value of the member PWD_SUB_INTR_MASK.audpath_intr_mask
//  Input       : unsigned int uaudpath_intr_mask: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_SUB_INTR_MASK_audpath_intr_maskGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_SUB_INTR_MASK o_pwd_sub_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_SUB_INTR_MASK.u32);
    o_pwd_sub_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pwd_sub_intr_mask.bits.audpath_intr_mask;
}

//******************************************************************************
//  Function    : HDMI_PWD_SUB_INTR_MASK_vidpath_intr_maskGet
//  Description : Set the value of the member PWD_SUB_INTR_MASK.vidpath_intr_mask
//  Input       : unsigned int uvidpath_intr_mask: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_SUB_INTR_MASK_vidpath_intr_maskGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_SUB_INTR_MASK o_pwd_sub_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_SUB_INTR_MASK.u32);
    o_pwd_sub_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pwd_sub_intr_mask.bits.vidpath_intr_mask;
}

//******************************************************************************
//  Function    : HDMI_PWD_SUB_INTR_MASK_tx_sys_intr_maskGet
//  Description : Set the value of the member PWD_SUB_INTR_MASK.tx_sys_intr_mask
//  Input       : unsigned int utx_sys_intr_mask: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_PWD_SUB_INTR_MASK_tx_sys_intr_maskGet(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_PWD_SUB_INTR_MASK o_pwd_sub_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->PWD_SUB_INTR_MASK.u32);
    o_pwd_sub_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    return o_pwd_sub_intr_mask.bits.tx_sys_intr_mask;
}

//******************************************************************************
//  Function    : HDMI_TXSYS_INTR_STATE_tx_sys_intr_state5Get
//  Description : Set the value of the member TXSYS_INTR_STATE.tx_sys_intr_state5
//  Input       : unsigned int utx_sys_intr_state5: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TXSYS_INTR_STATE_tx_sys_intr_state5Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TXSYS_INTR_STATE o_txsys_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TXSYS_INTR_STATE.u32);
    o_txsys_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_txsys_intr_state.bits.tx_sys_intr_state5;
}

//******************************************************************************
//  Function    : HDMI_TXSYS_INTR_STATE_tx_sys_intr_state4Get
//  Description : Set the value of the member TXSYS_INTR_STATE.tx_sys_intr_state4
//  Input       : unsigned int utx_sys_intr_state4: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TXSYS_INTR_STATE_tx_sys_intr_state4Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TXSYS_INTR_STATE o_txsys_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TXSYS_INTR_STATE.u32);
    o_txsys_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_txsys_intr_state.bits.tx_sys_intr_state4;
}

//******************************************************************************
//  Function    : HDMI_TXSYS_INTR_STATE_tx_sys_intr_state3Get
//  Description : Set the value of the member TXSYS_INTR_STATE.tx_sys_intr_state3
//  Input       : unsigned int utx_sys_intr_state3: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TXSYS_INTR_STATE_tx_sys_intr_state3Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TXSYS_INTR_STATE o_txsys_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TXSYS_INTR_STATE.u32);
    o_txsys_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_txsys_intr_state.bits.tx_sys_intr_state3;
}

//******************************************************************************
//  Function    : HDMI_TXSYS_INTR_STATE_tx_sys_intr_state2Get
//  Description : Set the value of the member TXSYS_INTR_STATE.tx_sys_intr_state2
//  Input       : unsigned int utx_sys_intr_state2: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TXSYS_INTR_STATE_tx_sys_intr_state2Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TXSYS_INTR_STATE o_txsys_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TXSYS_INTR_STATE.u32);
    o_txsys_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_txsys_intr_state.bits.tx_sys_intr_state2;
}

//******************************************************************************
//  Function    : HDMI_TXSYS_INTR_STATE_tx_sys_intr_state1Get
//  Description : Set the value of the member TXSYS_INTR_STATE.tx_sys_intr_state1
//  Input       : unsigned int utx_sys_intr_state1: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TXSYS_INTR_STATE_tx_sys_intr_state1Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TXSYS_INTR_STATE o_txsys_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TXSYS_INTR_STATE.u32);
    o_txsys_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_txsys_intr_state.bits.tx_sys_intr_state1;
}

//******************************************************************************
//  Function    : HDMI_TXSYS_INTR_STATE_tx_sys_intr_state0Get
//  Description : Set the value of the member TXSYS_INTR_STATE.tx_sys_intr_state0
//  Input       : unsigned int utx_sys_intr_state0: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TXSYS_INTR_STATE_tx_sys_intr_state0Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TXSYS_INTR_STATE o_txsys_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TXSYS_INTR_STATE.u32);
    o_txsys_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_txsys_intr_state.bits.tx_sys_intr_state0;
}

//******************************************************************************
//  Function    : HDMI_TXSYS_INTR_MASK_tx_sys_intr_mask5Get
//  Description : Set the value of the member TXSYS_INTR_MASK.tx_sys_intr_mask5
//  Input       : unsigned int utx_sys_intr_mask5: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TXSYS_INTR_MASK_tx_sys_intr_mask5Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TXSYS_INTR_MASK o_txsys_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TXSYS_INTR_MASK.u32);
    o_txsys_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    return o_txsys_intr_mask.bits.tx_sys_intr_mask5;
}

//******************************************************************************
//  Function    : HDMI_TXSYS_INTR_MASK_tx_sys_intr_mask4Get
//  Description : Set the value of the member TXSYS_INTR_MASK.tx_sys_intr_mask4
//  Input       : unsigned int utx_sys_intr_mask4: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TXSYS_INTR_MASK_tx_sys_intr_mask4Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TXSYS_INTR_MASK o_txsys_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TXSYS_INTR_MASK.u32);
    o_txsys_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    return o_txsys_intr_mask.bits.tx_sys_intr_mask4;
}

//******************************************************************************
//  Function    : HDMI_TXSYS_INTR_MASK_tx_sys_intr_mask3Get
//  Description : Set the value of the member TXSYS_INTR_MASK.tx_sys_intr_mask3
//  Input       : unsigned int utx_sys_intr_mask3: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TXSYS_INTR_MASK_tx_sys_intr_mask3Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TXSYS_INTR_MASK o_txsys_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TXSYS_INTR_MASK.u32);
    o_txsys_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    return o_txsys_intr_mask.bits.tx_sys_intr_mask3;
}

//******************************************************************************
//  Function    : HDMI_TXSYS_INTR_MASK_tx_sys_intr_mask2Get
//  Description : Set the value of the member TXSYS_INTR_MASK.tx_sys_intr_mask2
//  Input       : unsigned int utx_sys_intr_mask2: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TXSYS_INTR_MASK_tx_sys_intr_mask2Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TXSYS_INTR_MASK o_txsys_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TXSYS_INTR_MASK.u32);
    o_txsys_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    return o_txsys_intr_mask.bits.tx_sys_intr_mask2;
}

//******************************************************************************
//  Function    : HDMI_TXSYS_INTR_MASK_tx_sys_intr_mask1Get
//  Description : Set the value of the member TXSYS_INTR_MASK.tx_sys_intr_mask1
//  Input       : unsigned int utx_sys_intr_mask1: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TXSYS_INTR_MASK_tx_sys_intr_mask1Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TXSYS_INTR_MASK o_txsys_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TXSYS_INTR_MASK.u32);
    o_txsys_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    return o_txsys_intr_mask.bits.tx_sys_intr_mask1;
}

//******************************************************************************
//  Function    : HDMI_TXSYS_INTR_MASK_tx_sys_intr_mask0Get
//  Description : Set the value of the member TXSYS_INTR_MASK.tx_sys_intr_mask0
//  Input       : unsigned int utx_sys_intr_mask0: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TXSYS_INTR_MASK_tx_sys_intr_mask0Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TXSYS_INTR_MASK o_txsys_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TXSYS_INTR_MASK.u32);
    o_txsys_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    return o_txsys_intr_mask.bits.tx_sys_intr_mask0;
}

//******************************************************************************
//  Function    : HDMI_VIDPATH_INTR_STATE_vidpath_intr_state6Get
//  Description : Set the value of the member VIDPATH_INTR_STATE.vidpath_intr_state6
//  Input       : unsigned int uvidpath_intr_state6: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDPATH_INTR_STATE_vidpath_intr_state6Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VIDPATH_INTR_STATE o_vidpath_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->VIDPATH_INTR_STATE.u32);
    o_vidpath_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_vidpath_intr_state.bits.vidpath_intr_state6;
}

//******************************************************************************
//  Function    : HDMI_VIDPATH_INTR_STATE_vidpath_intr_state5Get
//  Description : Set the value of the member VIDPATH_INTR_STATE.vidpath_intr_state5
//  Input       : unsigned int uvidpath_intr_state5: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDPATH_INTR_STATE_vidpath_intr_state5Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VIDPATH_INTR_STATE o_vidpath_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->VIDPATH_INTR_STATE.u32);
    o_vidpath_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_vidpath_intr_state.bits.vidpath_intr_state5;
}

//******************************************************************************
//  Function    : HDMI_VIDPATH_INTR_STATE_vidpath_intr_state4Get
//  Description : Set the value of the member VIDPATH_INTR_STATE.vidpath_intr_state4
//  Input       : unsigned int uvidpath_intr_state4: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDPATH_INTR_STATE_vidpath_intr_state4Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VIDPATH_INTR_STATE o_vidpath_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->VIDPATH_INTR_STATE.u32);
    o_vidpath_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_vidpath_intr_state.bits.vidpath_intr_state4;
}

//******************************************************************************
//  Function    : HDMI_VIDPATH_INTR_STATE_vidpath_intr_state3Get
//  Description : Set the value of the member VIDPATH_INTR_STATE.vidpath_intr_state3
//  Input       : unsigned int uvidpath_intr_state3: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDPATH_INTR_STATE_vidpath_intr_state3Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VIDPATH_INTR_STATE o_vidpath_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->VIDPATH_INTR_STATE.u32);
    o_vidpath_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_vidpath_intr_state.bits.vidpath_intr_state3;
}

//******************************************************************************
//  Function    : HDMI_VIDPATH_INTR_STATE_vidpath_intr_state2Get
//  Description : Set the value of the member VIDPATH_INTR_STATE.vidpath_intr_state2
//  Input       : unsigned int uvidpath_intr_state2: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDPATH_INTR_STATE_vidpath_intr_state2Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VIDPATH_INTR_STATE o_vidpath_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->VIDPATH_INTR_STATE.u32);
    o_vidpath_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_vidpath_intr_state.bits.vidpath_intr_state2;
}

//******************************************************************************
//  Function    : HDMI_VIDPATH_INTR_STATE_vidpath_intr_state1Get
//  Description : Set the value of the member VIDPATH_INTR_STATE.vidpath_intr_state1
//  Input       : unsigned int uvidpath_intr_state1: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDPATH_INTR_STATE_vidpath_intr_state1Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VIDPATH_INTR_STATE o_vidpath_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->VIDPATH_INTR_STATE.u32);
    o_vidpath_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_vidpath_intr_state.bits.vidpath_intr_state1;
}

//******************************************************************************
//  Function    : HDMI_VIDPATH_INTR_STATE_vidpath_intr_state0Get
//  Description : Set the value of the member VIDPATH_INTR_STATE.vidpath_intr_state0
//  Input       : unsigned int uvidpath_intr_state0: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDPATH_INTR_STATE_vidpath_intr_state0Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VIDPATH_INTR_STATE o_vidpath_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->VIDPATH_INTR_STATE.u32);
    o_vidpath_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_vidpath_intr_state.bits.vidpath_intr_state0;
}

//******************************************************************************
//  Function    : HDMI_VIDPATH_INTR_MASK_vidpath_intr_mask6Get
//  Description : Set the value of the member VIDPATH_INTR_MASK.vidpath_intr_mask6
//  Input       : unsigned int uvidpath_intr_mask6: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDPATH_INTR_MASK_vidpath_intr_mask6Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VIDPATH_INTR_MASK o_vidpath_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->VIDPATH_INTR_MASK.u32);
    o_vidpath_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    return o_vidpath_intr_mask.bits.vidpath_intr_mask6;
}

//******************************************************************************
//  Function    : HDMI_VIDPATH_INTR_MASK_vidpath_intr_mask5Get
//  Description : Set the value of the member VIDPATH_INTR_MASK.vidpath_intr_mask5
//  Input       : unsigned int uvidpath_intr_mask5: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDPATH_INTR_MASK_vidpath_intr_mask5Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VIDPATH_INTR_MASK o_vidpath_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->VIDPATH_INTR_MASK.u32);
    o_vidpath_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    return o_vidpath_intr_mask.bits.vidpath_intr_mask5;
}

//******************************************************************************
//  Function    : HDMI_VIDPATH_INTR_MASK_vidpath_intr_mask4Get
//  Description : Set the value of the member VIDPATH_INTR_MASK.vidpath_intr_mask4
//  Input       : unsigned int uvidpath_intr_mask4: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDPATH_INTR_MASK_vidpath_intr_mask4Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VIDPATH_INTR_MASK o_vidpath_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->VIDPATH_INTR_MASK.u32);
    o_vidpath_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    return o_vidpath_intr_mask.bits.vidpath_intr_mask4;
}

//******************************************************************************
//  Function    : HDMI_VIDPATH_INTR_MASK_vidpath_intr_mask3Get
//  Description : Set the value of the member VIDPATH_INTR_MASK.vidpath_intr_mask3
//  Input       : unsigned int uvidpath_intr_mask3: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDPATH_INTR_MASK_vidpath_intr_mask3Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VIDPATH_INTR_MASK o_vidpath_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->VIDPATH_INTR_MASK.u32);
    o_vidpath_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    return o_vidpath_intr_mask.bits.vidpath_intr_mask3;
}

//******************************************************************************
//  Function    : HDMI_VIDPATH_INTR_MASK_vidpath_intr_mask2Get
//  Description : Set the value of the member VIDPATH_INTR_MASK.vidpath_intr_mask2
//  Input       : unsigned int uvidpath_intr_mask2: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDPATH_INTR_MASK_vidpath_intr_mask2Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VIDPATH_INTR_MASK o_vidpath_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->VIDPATH_INTR_MASK.u32);
    o_vidpath_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    return o_vidpath_intr_mask.bits.vidpath_intr_mask2;
}

//******************************************************************************
//  Function    : HDMI_VIDPATH_INTR_MASK_vidpath_intr_mask1Get
//  Description : Set the value of the member VIDPATH_INTR_MASK.vidpath_intr_mask1
//  Input       : unsigned int uvidpath_intr_mask1: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDPATH_INTR_MASK_vidpath_intr_mask1Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VIDPATH_INTR_MASK o_vidpath_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->VIDPATH_INTR_MASK.u32);
    o_vidpath_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    return o_vidpath_intr_mask.bits.vidpath_intr_mask1;
}

//******************************************************************************
//  Function    : HDMI_VIDPATH_INTR_MASK_vidpath_intr_mask0Get
//  Description : Set the value of the member VIDPATH_INTR_MASK.vidpath_intr_mask0
//  Input       : unsigned int uvidpath_intr_mask0: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_VIDPATH_INTR_MASK_vidpath_intr_mask0Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_VIDPATH_INTR_MASK o_vidpath_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->VIDPATH_INTR_MASK.u32);
    o_vidpath_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    return o_vidpath_intr_mask.bits.vidpath_intr_mask0;
}

//******************************************************************************
//  Function    : HDMI_AUDPATH_INTR_STATE_audpath_intr_state8Get
//  Description : Set the value of the member AUDPATH_INTR_STATE.audpath_intr_state8
//  Input       : unsigned int uaudpath_intr_state8: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUDPATH_INTR_STATE_audpath_intr_state8Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AUDPATH_INTR_STATE o_audpath_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->AUDPATH_INTR_STATE.u32);
    o_audpath_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_audpath_intr_state.bits.audpath_intr_state8;
}

//******************************************************************************
//  Function    : HDMI_AUDPATH_INTR_STATE_audpath_intr_state7Get
//  Description : Set the value of the member AUDPATH_INTR_STATE.audpath_intr_state7
//  Input       : unsigned int uaudpath_intr_state7: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUDPATH_INTR_STATE_audpath_intr_state7Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AUDPATH_INTR_STATE o_audpath_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->AUDPATH_INTR_STATE.u32);
    o_audpath_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_audpath_intr_state.bits.audpath_intr_state7;
}

//******************************************************************************
//  Function    : HDMI_AUDPATH_INTR_STATE_audpath_intr_state6Get
//  Description : Set the value of the member AUDPATH_INTR_STATE.audpath_intr_state6
//  Input       : unsigned int uaudpath_intr_state6: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUDPATH_INTR_STATE_audpath_intr_state6Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AUDPATH_INTR_STATE o_audpath_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->AUDPATH_INTR_STATE.u32);
    o_audpath_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_audpath_intr_state.bits.audpath_intr_state6;
}

//******************************************************************************
//  Function    : HDMI_AUDPATH_INTR_STATE_audpath_intr_state5Get
//  Description : Set the value of the member AUDPATH_INTR_STATE.audpath_intr_state5
//  Input       : unsigned int uaudpath_intr_state5: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUDPATH_INTR_STATE_audpath_intr_state5Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AUDPATH_INTR_STATE o_audpath_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->AUDPATH_INTR_STATE.u32);
    o_audpath_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_audpath_intr_state.bits.audpath_intr_state5;
}

//******************************************************************************
//  Function    : HDMI_AUDPATH_INTR_STATE_audpath_intr_state4Get
//  Description : Set the value of the member AUDPATH_INTR_STATE.audpath_intr_state4
//  Input       : unsigned int uaudpath_intr_state4: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUDPATH_INTR_STATE_audpath_intr_state4Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AUDPATH_INTR_STATE o_audpath_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->AUDPATH_INTR_STATE.u32);
    o_audpath_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_audpath_intr_state.bits.audpath_intr_state4;
}

//******************************************************************************
//  Function    : HDMI_AUDPATH_INTR_STATE_audpath_intr_state3Get
//  Description : Set the value of the member AUDPATH_INTR_STATE.audpath_intr_state3
//  Input       : unsigned int uaudpath_intr_state3: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUDPATH_INTR_STATE_audpath_intr_state3Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AUDPATH_INTR_STATE o_audpath_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->AUDPATH_INTR_STATE.u32);
    o_audpath_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_audpath_intr_state.bits.audpath_intr_state3;
}

//******************************************************************************
//  Function    : HDMI_AUDPATH_INTR_STATE_audpath_intr_state2Get
//  Description : Set the value of the member AUDPATH_INTR_STATE.audpath_intr_state2
//  Input       : unsigned int uaudpath_intr_state2: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUDPATH_INTR_STATE_audpath_intr_state2Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AUDPATH_INTR_STATE o_audpath_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->AUDPATH_INTR_STATE.u32);
    o_audpath_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_audpath_intr_state.bits.audpath_intr_state2;
}

//******************************************************************************
//  Function    : HDMI_AUDPATH_INTR_STATE_audpath_intr_state1Get
//  Description : Set the value of the member AUDPATH_INTR_STATE.audpath_intr_state1
//  Input       : unsigned int uaudpath_intr_state1: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUDPATH_INTR_STATE_audpath_intr_state1Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AUDPATH_INTR_STATE o_audpath_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->AUDPATH_INTR_STATE.u32);
    o_audpath_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_audpath_intr_state.bits.audpath_intr_state1;
}

//******************************************************************************
//  Function    : HDMI_AUDPATH_INTR_STATE_audpath_intr_state0Get
//  Description : Set the value of the member AUDPATH_INTR_STATE.audpath_intr_state0
//  Input       : unsigned int uaudpath_intr_state0: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUDPATH_INTR_STATE_audpath_intr_state0Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AUDPATH_INTR_STATE o_audpath_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->AUDPATH_INTR_STATE.u32);
    o_audpath_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_audpath_intr_state.bits.audpath_intr_state0;
}

//******************************************************************************
//  Function    : HDMI_AUDPATH_INTR_MASK_audpath_intr_mask8Get
//  Description : Set the value of the member AUDPATH_INTR_MASK.audpath_intr_mask8
//  Input       : unsigned int uaudpath_intr_mask8: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUDPATH_INTR_MASK_audpath_intr_mask8Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AUDPATH_INTR_MASK o_audpath_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->AUDPATH_INTR_MASK.u32);
    o_audpath_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    return o_audpath_intr_mask.bits.audpath_intr_mask8;
}

//******************************************************************************
//  Function    : HDMI_AUDPATH_INTR_MASK_audpath_intr_mask7Get
//  Description : Set the value of the member AUDPATH_INTR_MASK.audpath_intr_mask7
//  Input       : unsigned int uaudpath_intr_mask7: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUDPATH_INTR_MASK_audpath_intr_mask7Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AUDPATH_INTR_MASK o_audpath_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->AUDPATH_INTR_MASK.u32);
    o_audpath_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    return o_audpath_intr_mask.bits.audpath_intr_mask7;
}

//******************************************************************************
//  Function    : HDMI_AUDPATH_INTR_MASK_audpath_intr_mask6Get
//  Description : Set the value of the member AUDPATH_INTR_MASK.audpath_intr_mask6
//  Input       : unsigned int uaudpath_intr_mask6: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUDPATH_INTR_MASK_audpath_intr_mask6Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AUDPATH_INTR_MASK o_audpath_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->AUDPATH_INTR_MASK.u32);
    o_audpath_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    return o_audpath_intr_mask.bits.audpath_intr_mask6;
}

//******************************************************************************
//  Function    : HDMI_AUDPATH_INTR_MASK_audpath_intr_mask5Get
//  Description : Set the value of the member AUDPATH_INTR_MASK.audpath_intr_mask5
//  Input       : unsigned int uaudpath_intr_mask5: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUDPATH_INTR_MASK_audpath_intr_mask5Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AUDPATH_INTR_MASK o_audpath_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->AUDPATH_INTR_MASK.u32);
    o_audpath_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    return o_audpath_intr_mask.bits.audpath_intr_mask5;
}

//******************************************************************************
//  Function    : HDMI_AUDPATH_INTR_MASK_audpath_intr_mask4Get
//  Description : Set the value of the member AUDPATH_INTR_MASK.audpath_intr_mask4
//  Input       : unsigned int uaudpath_intr_mask4: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUDPATH_INTR_MASK_audpath_intr_mask4Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AUDPATH_INTR_MASK o_audpath_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->AUDPATH_INTR_MASK.u32);
    o_audpath_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    return o_audpath_intr_mask.bits.audpath_intr_mask4;
}

//******************************************************************************
//  Function    : HDMI_AUDPATH_INTR_MASK_audpath_intr_mask3Get
//  Description : Set the value of the member AUDPATH_INTR_MASK.audpath_intr_mask3
//  Input       : unsigned int uaudpath_intr_mask3: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUDPATH_INTR_MASK_audpath_intr_mask3Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AUDPATH_INTR_MASK o_audpath_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->AUDPATH_INTR_MASK.u32);
    o_audpath_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    return o_audpath_intr_mask.bits.audpath_intr_mask3;
}

//******************************************************************************
//  Function    : HDMI_AUDPATH_INTR_MASK_audpath_intr_mask2Get
//  Description : Set the value of the member AUDPATH_INTR_MASK.audpath_intr_mask2
//  Input       : unsigned int uaudpath_intr_mask2: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUDPATH_INTR_MASK_audpath_intr_mask2Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AUDPATH_INTR_MASK o_audpath_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->AUDPATH_INTR_MASK.u32);
    o_audpath_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    return o_audpath_intr_mask.bits.audpath_intr_mask2;
}

//******************************************************************************
//  Function    : HDMI_AUDPATH_INTR_MASK_audpath_intr_mask1Get
//  Description : Set the value of the member AUDPATH_INTR_MASK.audpath_intr_mask1
//  Input       : unsigned int uaudpath_intr_mask1: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUDPATH_INTR_MASK_audpath_intr_mask1Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AUDPATH_INTR_MASK o_audpath_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->AUDPATH_INTR_MASK.u32);
    o_audpath_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    return o_audpath_intr_mask.bits.audpath_intr_mask1;
}

//******************************************************************************
//  Function    : HDMI_AUDPATH_INTR_MASK_audpath_intr_mask0Get
//  Description : Set the value of the member AUDPATH_INTR_MASK.audpath_intr_mask0
//  Input       : unsigned int uaudpath_intr_mask0: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_AUDPATH_INTR_MASK_audpath_intr_mask0Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_AUDPATH_INTR_MASK o_audpath_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->AUDPATH_INTR_MASK.u32);
    o_audpath_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    return o_audpath_intr_mask.bits.audpath_intr_mask0;
}

//******************************************************************************
//  Function    : HDMI_TXHDMI_INTR_STATE_txhdmi_intr_state0Get
//  Description : Set the value of the member TXHDMI_INTR_STATE.txhdmi_intr_state0
//  Input       : unsigned int utxhdmi_intr_state0: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TXHDMI_INTR_STATE_txhdmi_intr_state0Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TXHDMI_INTR_STATE o_txhdmi_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TXHDMI_INTR_STATE.u32);
    o_txhdmi_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_txhdmi_intr_state.bits.txhdmi_intr_state0;
}

//******************************************************************************
//  Function    : HDMI_TXHDMI_INTR_MASK_txhdmi_intr_mask0Get
//  Description : Set the value of the member TXHDMI_INTR_MASK.txhdmi_intr_mask0
//  Input       : unsigned int utxhdmi_intr_mask0: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_TXHDMI_INTR_MASK_txhdmi_intr_mask0Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_TXHDMI_INTR_MASK o_txhdmi_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->TXHDMI_INTR_MASK.u32);
    o_txhdmi_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    return o_txhdmi_intr_mask.bits.txhdmi_intr_mask0;
}

//******************************************************************************
//  Function    : HDMI_HDCP_INTR_STATE_hdcp_intr_state1Get
//  Description : Set the value of the member HDCP_INTR_STATE.hdcp_intr_state1
//  Input       : unsigned int uhdcp_intr_state1: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP_INTR_STATE_hdcp_intr_state1Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDCP_INTR_STATE o_hdcp_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->HDCP_INTR_STATE.u32);
    o_hdcp_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp_intr_state.bits.hdcp_intr_state1;
}

//******************************************************************************
//  Function    : HDMI_HDCP_INTR_STATE_hdcp_intr_state0Get
//  Description : Set the value of the member HDCP_INTR_STATE.hdcp_intr_state0
//  Input       : unsigned int uhdcp_intr_state0: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP_INTR_STATE_hdcp_intr_state0Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDCP_INTR_STATE o_hdcp_intr_state;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->HDCP_INTR_STATE.u32);
    o_hdcp_intr_state.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp_intr_state.bits.hdcp_intr_state0;
}

//******************************************************************************
//  Function    : HDMI_HDCP_INTR_MASK_hdcp_intr_mask1Get
//  Description : Set the value of the member HDCP_INTR_MASK.hdcp_intr_mask1
//  Input       : unsigned int uhdcp_intr_mask1: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP_INTR_MASK_hdcp_intr_mask1Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDCP_INTR_MASK o_hdcp_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->HDCP_INTR_MASK.u32);
    o_hdcp_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp_intr_mask.bits.hdcp_intr_mask1;
}

//******************************************************************************
//  Function    : HDMI_HDCP_INTR_MASK_hdcp_intr_mask0Get
//  Description : Set the value of the member HDCP_INTR_MASK.hdcp_intr_mask0
//  Input       : unsigned int uhdcp_intr_mask0: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int HDMI_HDCP_INTR_MASK_hdcp_intr_mask0Get(void)
{
    HI_U32 *pu32RegAddr = NULL;
    U_HDCP_INTR_MASK o_hdcp_intr_mask;
    pu32RegAddr = (HI_U32*)&(goptx_ctrl_regAllReg->HDCP_INTR_MASK.u32);
    o_hdcp_intr_mask.u32 = HDMITXRegRead(pu32RegAddr);
    return o_hdcp_intr_mask.bits.hdcp_intr_mask0;
}

