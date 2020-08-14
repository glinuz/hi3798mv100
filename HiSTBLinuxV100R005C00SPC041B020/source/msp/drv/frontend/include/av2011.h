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
 * \brief For AV2011 tuner.
 */
#ifndef __AV2011_H__
#define __AV2011_H__

/*----------------------------- INCLUDE FILES ------------------------------------------*/

#include "II2CRepeater.h"
#include "ITuner.h"
#include "hi_type.h"
#include "hi_unf_frontend.h"

/*--------------------------- MACRO DECLARATIONS ---------------------------------------*/

#define AV2011_ENABLE_TIMEOUT
#define AV2011_TIMEOUT 100

#define  tuner_crystal 27 //unit is MHz
#define AV2011_Tuner

#define AV2011_R0_PLL_LOCK 0x01


/*-------------------- GLOBAL STRUCTURE DECLARATIONS -----------------------------------*/

/// AV2011_Setting: Stucture for AV2011 special setting
struct AV2011_Setting
{
    AVL_uchar AutoGain;
    AVL_uchar Single_End;
    AVL_uchar Auto_Scan;
    AVL_uchar RF_Loop;
};

enum AutoGain
{
    AV2011_AutoGain_OFF = 0, //FT_EN 0, fix LNA gain
    AV2011_AutoGain_ON = 1 //FT_EN 0, LNA gain Auto control
};

enum Single_End
{
    AV2011_Differential = 0,  //IQ Differential mode
    AV2011_SingleEnd = 1    //IQ Single end mode
};

enum Auto_Scan
{
    Auto_Scan_OFF = 0,  //0 = normal manual lock mode
    Auto_Scan_ON = 1 //1 = blindscan search mode
};

enum RF_Loop
{
    RF_Loop_OFF = 0,  //0 = RF loop through off
    RF_Loop_ON = 1 //1 = RF loop through on
};


/*------------------------- GLOBAL DECLARATIONS ----------------------------------------*/

AVL_DVBSx_ErrorCode AVL_DVBSx_ExtAV2011_Initialize(struct AVL_Tuner * pTuner);
AVL_DVBSx_ErrorCode AVL_DVBSx_ExtAV2011_GetLockStatus(struct AVL_Tuner * pTuner);
AVL_DVBSx_ErrorCode AVL_DVBSx_ExtAV2011_Lock( struct AVL_Tuner * pTuner);

extern HI_S32 av2011_init_tuner(HI_U32 u32TunerPort);
extern HI_S32 av2011_set_tuner(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_U32 u32PuRF/*kHz*/);  //freq :unit MHZ, symbol:unit  Khz
extern HI_S32 av2011_set_loopthrough(HI_U32 u32TunerPort);
extern HI_S32 av2011_cancel_loopthrough(HI_U32 u32TunerPort);

#endif /* __AV2011_H__ */

