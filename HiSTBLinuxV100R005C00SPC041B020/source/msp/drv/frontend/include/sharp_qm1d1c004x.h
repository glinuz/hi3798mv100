/******************************************************************************

  Copyright (C), 2012, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : sharp_qm1d1c004x.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2012/02/20
  Description   : 
  History       :
  1.Date        : 2012/02/20
    Author      : 
    Modification: Created file

 *******************************************************************************/

/**
 * \file
 * \brief For Sharp 7903 tuner.
 */
#ifndef __SHARPQM1D1C004X_H__
#define __SHARPQM1D1C004X_H__

/*----------------------------- INCLUDE FILES ------------------------------------------*/

#include "avl_dvbsx.h"
#include "ITuner.h"
#include "hi_type.h"
#include "hi_unf_frontend.h"

/*--------------------------- MACRO DECLARATIONS ---------------------------------------*/


/*-------------------- GLOBAL STRUCTURE DECLARATIONS -----------------------------------*/


/*------------------------- GLOBAL DECLARATIONS ----------------------------------------*/

AVL_DVBSx_ErrorCode ExtSharpQM1D1C004x_Initialize(struct AVL_Tuner * pTuner);
AVL_DVBSx_ErrorCode ExtSharpQM1D1C004x_GetLockStatus(struct AVL_Tuner * pTuner );
AVL_DVBSx_ErrorCode ExtSharpQM1D1C004x_Lock(struct AVL_Tuner * pTuner);

extern HI_S32 sharp7903_init_tuner(HI_U32 u32TunerPort);
extern HI_S32 sharp7903_set_tuner(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_U32 u32PuRF/*kHz*/);  //freq :unit MHZ, symbol:unit  Khz

#endif  /* __SHARPQM1D1C004X_H__ */

/** @}*/

