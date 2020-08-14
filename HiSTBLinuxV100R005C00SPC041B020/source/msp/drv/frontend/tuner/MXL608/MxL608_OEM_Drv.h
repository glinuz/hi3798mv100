/*******************************************************************************
 *
 * FILE NAME          : MxL608_OEM_Drv.h
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

#ifndef __MxL608_OEM_DRV_H__
#define __MxL608_OEM_DRV_H__

/******************************************************************************
    Include Header Files
    (No absolute paths - paths handled by make file)
******************************************************************************/

#include "MaxLinearDataTypes.h"
#include "MxL_Debug.h"

/******************************************************************************
    Macros
******************************************************************************/

/******************************************************************************
    User-Defined Types (Typedefs)
******************************************************************************/

/******************************************************************************
    Global Variable Declarations
******************************************************************************/
extern void * MxL608_OEM_DataPtr[];
/*****************************************************************************
    Prototypes
******************************************************************************/

MXL_STATUS MxLWare608_OEM_WriteRegister(UINT8 devId, UINT8 regAddr, UINT8 regData);
MXL_STATUS MxLWare608_OEM_ReadRegister(UINT8 devId, UINT8 regAddr, UINT8 *regDataPtr);
void MxLWare608_OEM_Sleep(UINT16 delayTimeInMs);  
void MxL608_I2C_SetChn(UINT32 I2CChn,UINT32 I2CAddr);
void MxL608_I2C_SetPort(UINT32 TunerPort);

#endif /* __MxL601_OEM_DRV_H__*/




