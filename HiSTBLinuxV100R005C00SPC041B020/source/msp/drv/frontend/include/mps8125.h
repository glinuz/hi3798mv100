/******************************************************************************

  Copyright (C), 2012, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mps8125.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2012/02/25
  Description   : 
  History       :
  1.Date        : 2012/02/25
    Author      : l00185424
    Modification: Created file

 *******************************************************************************/

/**
 * \file
 * \brief For LNB control chipsets, e.g. MPS8125, ISL9492.
 */
#ifndef __MPS8125_H__
#define __MPS8125_H__

/*----------------------------- INCLUDE FILES ------------------------------------------*/

#include "hi_type.h"
#include "drv_tuner_ext.h"


/*--------------------------- MACRO DECLARATIONS ---------------------------------------*/


/*-------------------- GLOBAL STRUCTURE DECLARATIONS -----------------------------------*/


/*------------------------- GLOBAL DECLARATIONS ----------------------------------------*/

HI_S32 mps8125_init(HI_U32 u32TunerPort, HI_U32 u32I2CNum, HI_U8 u8DevAddr, HI_UNF_DEMOD_DEV_TYPE_E enDemodDevType);
HI_S32 mps8125_deInit(HI_U32 u32TunerPort);
HI_S32 mps8125_standby(HI_U32 u32TunerPort, HI_U32 u32Standby);
HI_S32 mps8125_set_lnb_out(HI_U32 u32TunerPort, TUNER_LNB_OUT_E enOut);
HI_S32 mps8125_send_continuous_22K(HI_U32 u32TunerPort, HI_U32 u32Continuous22K);
HI_S32 mps8125_send_tone(HI_U32 u32TunerPort, HI_U32 u32Tone);
HI_S32 mps8125_DiSEqC_send_msg(HI_U32 u32TunerPort, HI_UNF_TUNER_DISEQC_SENDMSG_S *pstSendMsg);
HI_S32 mps8125_DiSEqC_recv_msg(HI_U32 u32TunerPort, HI_UNF_TUNER_DISEQC_RECVMSG_S *pstRecvMsg);

#endif  /* __MPS8125_H__ */

