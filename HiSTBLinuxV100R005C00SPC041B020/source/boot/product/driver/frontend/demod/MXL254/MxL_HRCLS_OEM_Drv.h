/*****************************************************************************************
 *
 * FILE NAME          : MxL_HRCLS_OEM_Drv.h
 * 
 * AUTHOR             : Jun Huang
 *
 * DATE CREATED       : 08/25/2011
 *
 * DESCRIPTION        : Header file for OEM driver interface
 *
 *****************************************************************************************
 *                Copyright (c) 2006, MaxLinear, Inc.
 ****************************************************************************************/

#ifndef __MXL_HRCLS_OEM_DRV_H__
#define __MXL_HRCLS_OEM_DRV_H__

/*****************************************************************************************
    Include Header Files
    (No absolute paths - paths handled by make file)
*****************************************************************************************/
#include "MaxLinearDataTypes.h"
#include "MxL_HRCLS_OEM_Defines.h"

/*****************************************************************************************
    Macros
*****************************************************************************************/

/*****************************************************************************************
    User-Defined Types (Typedefs)
*****************************************************************************************/

/*****************************************************************************************
    Global Variable Declarations
*****************************************************************************************/
extern void * MxL_HRCLS_OEM_DataPtr[];
/*****************************************************************************************
    Prototypes
*****************************************************************************************/

MXL_STATUS_E MxLWare_HRCLS_OEM_Reset(UINT8 devId);
MXL_STATUS_E MxLWare_HRCLS_OEM_WriteRegister(UINT8 devId, UINT16 regAddr, UINT16 regData);
MXL_STATUS_E MxLWare_HRCLS_OEM_ReadRegister(UINT8 devId, UINT16 regAddr,/*@out@*/  UINT16 *dataPtr);
MXL_STATUS_E MxLWare_HRCLS_OEM_ReadBlock(UINT8 devId, UINT16 regAddr, UINT16 readSize, /*@out@*/ UINT8 *bufPtr);
MXL_STATUS_E MxLWare_HRCLS_OEM_ReadBlockExt(UINT8 devId, UINT16 cmdId, UINT16 offset, UINT16 readSize, /*@out@*/ UINT8* bufPtr);
MXL_STATUS_E MxLWare_HRCLS_OEM_WriteBlock(UINT8 devId, UINT16 regAddr, UINT16 bufSize, UINT8* bufPtr);
MXL_STATUS_E MxLWare_HRCLS_OEM_ReadBlockExt(UINT8 devId, UINT16 cmdId, UINT16 offset, UINT16 readSize, UINT8 *bufPtr);
MXL_STATUS_E MxLWare_HRCLS_OEM_LoadNVRAMFile(UINT8 devId, UINT8 *bufPtr, UINT32 bufLen);
MXL_STATUS_E MxLWare_HRCLS_OEM_SaveNVRAMFile(UINT8 devId, UINT8 *bufPtr, UINT32 bufLen);
void MxLWare_HRCLS_OEM_DelayUsec(UINT32 usec);
void MxLWare_HRCLS_OEM_GetCurrTimeInUsec(/*@out@*/ UINT64* usecPtr);

#endif /* __MXL_HRCLS_OEM_DRV_H__*/




