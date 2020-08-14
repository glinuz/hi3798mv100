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
* @file si_drv_mhl_tx_mdt_internal.h
 *
 * @brief CBUS MDT Internal API
 *
 *****************************************************************************/
#ifndef SI_MOD_TX_CBUS_MDT_INTERNAL_H
#define SI_MOD_TX_CBUS_MDT_INTERNAL_H

#include "si_drv_tx_api.h"
#include "si_mod_tx_cbus_defs.h"

typedef struct
{
	SiiInst_t		craInst;
	SiiDrvCraAddr_t baseAddr;
	SiiInst_t		mhlInst;
} SiiModMdtConfig_t;

SiiInst_t SiiModCbusMdtInitialize(char *pNameStr, SiiModMdtConfig_t *pConfig);
void SiiModCbusMdtDelete(SiiInst_t mdtInst);
void SiiModStartMDTFloodTest(SiiInst_t mdtInst);

//void MdtTest(SiiInst_t mdtInst);
//void MdtTestInit(SiiInst_t mdtInst);
//void MdtWrBlock(SiiInst_t mdtInst, uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4);
//void MdtRdBlock(SiiInst_t mdtInst);
//void mdtStatus(SiiInst_t mdtInst);
//void mdtHelp(SiiInst_t mdtInst);
//HI_BOOL mdtFloodTestEnable(SiiInst_t mdtInst);
//void mdtXmitEnable(SiiInst_t mdtInst);
//void MdtRdXmitFifoBlock(SiiInst_t mdtInst);
//void mdtRxcEnable(SiiInst_t mdtInst);
//void mdtFloodTestDisable(SiiInst_t mdtInst);

#endif //SI_MOD_TX_CBUS_MDT_INTERNAL_H