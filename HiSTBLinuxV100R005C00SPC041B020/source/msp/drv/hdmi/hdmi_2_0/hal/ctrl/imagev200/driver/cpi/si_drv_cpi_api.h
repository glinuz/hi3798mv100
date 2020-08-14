//***************************************************************************
//!file     si_drv_cpi_api.h
//!brief    Silicon Image CPI driver.
//
// No part of this work may be reproduced, modified, distributed,
// transmitted, transcribed, or translated into any language or computer
// format, in any form or by any means without written permission of
// Silicon Image, Inc., 1060 East Arques Avenue, Sunnyvale, California 94085
//
// Copyright 2002-2010, Silicon Image, Inc.  All rights reserved.
//***************************************************************************/

#ifndef __SI_CPI_DRV_API_H__
#define __SI_CPI_DRV_API_H__
#ifdef HDMI_CEC_SUPPORT
#include "si_datatypes.h"
#include "si_lib_obj_api.h"


typedef 	void (*CecCallBackPtr_t)(SiiInst_t, void*);

//-------------------------------------------------------------------------------
// CPI Enums and manifest constants
//-------------------------------------------------------------------------------


#define SII_MAX_CMD_SIZE 16

typedef enum _SiiDrvCpiError_t
{
	RESULT_CPI_SUCCESS,             // Success result code
	RESULT_CPI_FAIL,                // General Failure result code
	RESULT_CPI_WRITE_QUEUE_FULL,    // CPI Write Queue was full
} SiiDrvCpiError_t;


typedef enum _SiiCpiTxState_t
{
	SiiTX_IDLE          = 0,
	SiiTX_WAITCMD,
	SiiTX_SENDING,
	SiiTX_SENDACKED,
	SiiTX_SENDFAILED,
	SiiTX_TIMEOUT
} SiiCpiTxState_t;

typedef enum _SiiDrvCpiStatus_t
{
	SiiCPI_CEC_INT          = 0x0001,
	SiiCPI_CEC_STATUS_VALID = 0x0002,
	SiiCPI_CEC_DISABLED     = 0x0004,
} SiiDrvCpiStatus_t;

//-------------------------------------------------------------------------------
// CPI data structures
//-------------------------------------------------------------------------------

typedef struct _SiiCpiData_t
{
	uint8_t         srcDestAddr;            // Source in upper nibble, dest in lower nibble
	uint16_t         opcode;
	uint8_t         args[ SII_MAX_CMD_SIZE ];
	uint8_t         argCount;
	SiiCpiTxState_t txState;
	uint16_t        msgId;
} SiiCpiData_t;

//-------------------------------------------------------------------------------
// CPI data structures
//-------------------------------------------------------------------------------

typedef struct _SiiCpiStatus_t
{
	uint8_t         rxState;
	SiiCpiTxState_t txState;
	uint8_t         cecError;
	uint16_t        msgId;

} SiiCpiStatus_t;

//------------------------------------------------------------------------------
//  Standard driver functions
//------------------------------------------------------------------------------

SiiInst_t SiiDrvCpiCreate (SiiInst_t inst, uint16_t baseAddr, CecCallBackPtr_t cecCbFunc, CecCallBackPtr_t cecAPingCbFunc, void* data);
void        SiiDrvCpiDelete(SiiInst_t inst );
uint16_t    SiiDrvCpiStatus( SiiInst_t inst );
bool_t      SiiDrvCpiStandby( SiiInst_t inst );
bool_t      SiiDrvCpiResume( SiiInst_t inst );
uint_t      SiiDrvCpiGetLastResult( SiiInst_t inst );
void		SiiDrvCecInterruptHandler( SiiInst_t inst );

void        SiiDrvCpiEnable( SiiInst_t inst );
void        SiiDrvCpiDisable( SiiInst_t inst );

//------------------------------------------------------------------------------
//  Driver Specific functions
//------------------------------------------------------------------------------

bool_t      SiiDrvCpiRead( SiiInst_t inst, SiiCpiData_t *pMsg );
int32_t    SiiDrvCpiWrite( SiiInst_t inst, SiiCpiData_t *pMsg );
void        SiiDrvCpiServiceWriteQueue( SiiInst_t inst );
bool_t      SiiDrvCpiHwStatusGet( SiiInst_t inst, SiiCpiStatus_t *pCpiStat );
uint_t      SiiDrvCpiFrameCount( SiiInst_t inst );

bool_t      SiiDrvCpiSetLogicalAddr ( SiiInst_t inst, uint8_t logicalAddress );
uint8_t	    SiiDrvCpiGetLogicalAddr( SiiInst_t inst );
uint16_t    SiiDrvCpiSendPing( SiiInst_t inst, uint8_t destLa );
void        SiiDrvCpiAutoPingStart( SiiInst_t inst);
bool_t      SiiDrvCpiAPingDisc(SiiInst_t inst, uint16_t* autoPingMap);
void        SiiDrvCecAutoPingHandler( SiiInst_t inst );
void        SiiDrvCpiAPingTimerStart(SiiInst_t inst);
void        SiiDrvCpiAPingTimerStop(SiiInst_t inst);
void        SiiDrvCpiAutoPingEnable(SiiInst_t inst);
uint8_t     SiiDrvCpiPingStatusGet(SiiInst_t inst);
#endif
#endif // __SI_DRV_CPI_API_H__