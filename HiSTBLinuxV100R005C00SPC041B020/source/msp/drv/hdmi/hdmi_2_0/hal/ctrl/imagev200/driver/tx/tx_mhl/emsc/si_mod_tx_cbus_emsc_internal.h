/******************************************************************************
*
 * Copyright 2014, Silicon Image, Inc.  All rights reserved.
 * No part of this work may be reproduced, modified, distributed, transmitted,
 * transcribed, or translated into any language or computer format, in any form
 * or by any means without written permission of
 * Silicon Image, Inc., 1140 East Arques Avenue, Sunnyvale, California 94085
 *
 *****************************************************************************/
/**
* @file si_drv_mhl_tx_emsc_internal.h
 *
 * @brief CBUS EMSC Internal API
 *
 *****************************************************************************/
#ifndef __SI_MOD_TX_CBUS_EMSC_INTERNAL_H__
#define __SI_MOD_TX_CBUS_EMSC_INTERNAL_H__

#include "si_drv_tx_api.h"
#include "si_mod_tx_cbus_defs.h"

typedef struct
{
	SiiInst_t		craInst;
	SiiDrvCraAddr_t baseAddr;
	SiiInst_t		mhlInst;
} SiiModEmscConfig_t;

SiiInst_t SiiModCbusEmscCreate(char *pNameStr, SiiModEmscConfig_t *pConfig);
void SiiModCbusEmscDelete(SiiInst_t emscInst);
void SiiModCbusEmscIntrHandler(SiiInst_t emscInst);
void SiiModStartEmscFloodTest(SiiInst_t emscInst);
//void SiiModEmscTransmit(SiiInst_t emscInst);
//void SiiModEmscWriteBlock(SiiInst_t emscInst, uint8_t block, uint16_t count);
//void SiiModEmscReadBlock(SiiInst_t emscInst);
//void SiiModEmscFifo(SiiInst_t emscInst);
//void SiiModEmscHelp(SiiInst_t emscInst);
//bool_t SiiModEmscFloodTestEnable (SiiInst_t emscInst);
//void SiiModEmscXmitEnable (SiiInst_t emscInst);
//void SiiModEmscRxcEnable (SiiInst_t emscInst);

#endif //__SI_MOD_TX_CBUS_EMSC_INTERNAL_H__