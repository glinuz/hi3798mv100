/*******************************************************************************
 *
 * FILE NAME          : MxL68x_OEM_Drv.h
 * 
 * AUTHOR             : Dong Liu 
 *
 * DATE CREATED       : 03/23/2012
 *
 * DESCRIPTION        : Header file for MxL68x_OEM_Drv.c
 *
 *******************************************************************************
 *                Copyright (c) 2010, MaxLinear, Inc.
 ******************************************************************************/

#ifndef __MxL68x_OEM_DRV_H__
#define __MxL68x_OEM_DRV_H__

/******************************************************************************
    Include Header Files
    (No absolute paths - paths handled by make file)
******************************************************************************/

#include "MaxLinearDataTypes.h"

/******************************************************************************
    Macros
******************************************************************************/

/******************************************************************************
    User-Defined Types (Typedefs)
******************************************************************************/

typedef void (*INTERRUPT_TRIGGERED_CB_FN)(void);

/******************************************************************************
    Global Variable Declarations
******************************************************************************/

extern void * MxL68x_OEM_DataPtr[];

/******************************************************************************
    Prototypes
******************************************************************************/

MXL_STATUS_E MxLWare68x_OEM_WriteRegister(UINT8 devId, UINT16 regAddr, UINT16 regData);
MXL_STATUS_E MxLWare68x_OEM_ReadRegister(UINT8 devId, UINT16 regAddr, UINT16 *dataPtr);

MXL_STATUS_E MxLWare68x_OEM_WriteBlock(UINT8 devId, UINT16 regAddr, UINT16 bufSize, UINT8* bufPtr);
MXL_STATUS_E MxLWare68x_OEM_ReadBlock(UINT8 devId, UINT16 regAddr, UINT16 readSize, UINT8* bufPtr);

void MxLWare68x_OEM_SleepInUs(UINT32 delayTimeInUs);  

MXL_STATUS_E MxLWare68x_OEM_RegisterCallBack(INTERRUPT_TRIGGERED_CB_FN FuncPtr, UINT8 IntSrcId);
MXL_STATUS_E MxLWare68x_OEM_UnRegisterCallBack(UINT8 IntSrcId);

#endif /* __MxL68x_OEM_DRV_H__*/




