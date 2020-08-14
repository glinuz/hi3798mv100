/*******************************************************************************
 *
 * FILE NAME          : MxL601_OEM_Drv.h
 * 
 * AUTHOR             : Dong Liu 
 *
 * DATE CREATED       : 11/23/2011
 *
 * DESCRIPTION        : Header file for MxL601_OEM_Drv.c
 *
 *******************************************************************************
 *                Copyright (c) 2010, MaxLinear, Inc.
 ******************************************************************************/

#ifndef __MxL603_OEM_DRV_H__
#define __MxL603_OEM_DRV_H__

/******************************************************************************
    Include Header Files
    (No absolute paths - paths handled by make file)
******************************************************************************/

#include "MaxLinearDataTypes.h"
#include "MxL_Debug.h"
#include "hi_unf_frontend.h"
#include "drv_tuner_ioctl.h"

/******************************************************************************
    Macros
******************************************************************************/

/******************************************************************************
    User-Defined Types (Typedefs)
******************************************************************************/

/******************************************************************************
    Global Variable Declarations
******************************************************************************/
extern void * MxL603_OEM_DataPtr[];
/*****************************************************************************
    Prototypes
******************************************************************************/
extern HI_UNF_TUNER_SIG_TYPE_E  enMxl603TunerSigType[TUNER_NUM];
extern HI_UNF_DEMOD_DEV_TYPE_E enMxl603DemodType;
MXL_STATUS MxLWare603_OEM_WriteRegister(UINT8 devId, UINT8 regAddr, UINT8 regData);
MXL_STATUS MxLWare603_OEM_ReadRegister(UINT8 devId, UINT8 regAddr, UINT8 *regDataPtr);
void MxLWare603_OEM_Sleep(UINT16 delayTimeInMs);  
void MxL603_I2C_SetChn(UINT32 I2CChn);
void MxL603_I2C_SetPort(UINT32 TunerPort);

#endif /* __MxL601_OEM_DRV_H__*/




