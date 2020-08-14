//***************************************************************************
//!file     si_drv_cpi_internal.h
//!brief    Silicon Image CPI internal driver functions.
//
// No part of this work may be reproduced, modified, distributed,
// transmitted, transcribed, or translated into any language or computer
// format, in any form or by any means without written permission of
// Silicon Image, Inc., 1060 East Arques Avenue, Sunnyvale, California 94085
//
// Copyright 2002-2010, Silicon Image, Inc.  All rights reserved.
//***************************************************************************/

#ifndef __SI_CPI_DRV_INTERNAL_H__
#define __SI_CPI_DRV_INTERNAL_H__
#include "si_drv_cpi_api.h"

//-------------------------------------------------------------------------------
// CPI Enums and manifest constants
//-------------------------------------------------------------------------------

typedef enum _SiiCecError_t
{
	SiiCEC_SHORTPULSE       = 0x80,
	SiiCEC_BADSTART         = 0x40,
	SiiCEC_RXOVERFLOW       = 0x20,
	SiiCEC_ERRORS           = (SiiCEC_SHORTPULSE | SiiCEC_BADSTART | SiiCEC_RXOVERFLOW)
} SiiCecError_t;

#define SII_CPI_OUTPUT_QUEUE_LEN    4

//------------------------------------------------------------------------------
//  CPI Driver Instance Data
//------------------------------------------------------------------------------

typedef struct _CpiMsgQueue_t
{
	int_t           inIndex;
	int_t           outIndex;
	uint32_t        msTimeout;
	uint32_t        msStart;
	SiiCpiData_t    queue[SII_CPI_OUTPUT_QUEUE_LEN];
} CpiMsgQueue_t;



//------------------------------------------------------------------------------
//  Driver Specific functions
//------------------------------------------------------------------------------

void    DrvCpiProcessInterrupts( SiiInst_t inst );

//------------------------------------------------------------------------------
//  CPI register access - supports multiple CPI units
//------------------------------------------------------------------------------

uint8_t SiiDrvCpiRegisterRead( uint8_t devicePage, uint16_t regAddr );
void SiiDrvCpiRegisterWrite( uint8_t devicePage, uint16_t regAddr, uint8_t value );
void SiiDrvCpiRegisterReadBlock( uint8_t devicePage, uint16_t regAddr, uint8_t* buffer, uint_t count );
void SiiDrvCpiRegisterWriteBlock( uint8_t devicePage, uint16_t regAddr, uint8_t *pBuffer, uint_t count );
void SiiDrvCpiRegisterModify( uint8_t devicePage, uint16_t regAddr, uint8_t mask, uint8_t value);
void SiiDrvCpiRegisterBitsSet( uint8_t devicePage, uint16_t regAddr, uint8_t bits );
void SiiDrvCpiRegisterBitsClear( uint8_t devicePage, uint16_t regAddr, uint8_t bits );


#endif // __SI_CPI_DRV_INTERNAL_H__
