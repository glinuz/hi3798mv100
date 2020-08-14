/******************************************************************************
 *
 * Copyright 2013, Silicon Image, Inc.  All rights reserved.
 * No part of this work may be reproduced, modified, distributed, transmitted,
 * transcribed, or translated into any language or computer format, in any form
 * or by any means without written permission of
 * Silicon Image, Inc., 1140 East Arques Avenue, Sunnyvale, California 94085
 *
 *****************************************************************************/
/**
 * @file
 *
 * @brief VideoPath API
 *
 *****************************************************************************/
#ifndef __SI_DRV_VIDEOPATH_H__
#define __SI_DRV_VIDEOPATH_H__

#include "si_datatypes.h"

typedef enum{
	SII_MOD_VIDPATH__COLOR_INFO_CONFIG,
	SII_MOD_VIDPATH__OUTPUT_COLORSPACE,
	SII_MOD_VIDPATH__BIT_DEPTH,
	SII_MOD_VIDPATH__HV_SYNC_POLARITY,
	SII_MOD_VIDPATH__MUTE,
	SII_MOD_VIDPATH__VIDEO_INPUT_INFO,
    SII_MOD_VIDPATH__BLACK_DATA,
    SII_MOD_VIDPATH__PATTERN_TPG_ENABLE,
	SII_MOD_VIDPATH__HW_STATUS,
	SII_MOD_VIDPATH__HW_INIT,
    SII_MOD_VIDPATH__SW_INIT,
    SII_MOD_VIDPATH_OUT_MAPPING,
    SII_MOD_VIDPATH_YUV_RGB_CONVERTION,
    SII_MOD_VIDPATH_CSC_CONFIG,
}SiiModVidPathInternalOpcode_t;


/***** public type definitions ***********************************************/
typedef uint32_t SiiDrvVidPathEvent_t;

typedef struct
{
    SiiInst_t instTxCra;
	SiiInst_t instTx;
} SiiDrvVideoPathCfg_t;

/*****************************************************************************/
/**
* @brief Video Path Module creation.
*
* @retval              Handle of instance created
*****************************************************************************/
SiiInst_t SiiModVideoPathCreate(char *pNameStr, SiiDrvVideoPathCfg_t *pConfig);

/*****************************************************************************/
/**
* @brief Video Path Module instance deletion.
*
* @param[in]  inst     Handle to instance
*
*****************************************************************************/
void SiiModVideoPathDelete( SiiInst_t inst);

/*****************************************************************************/
/**
* @brief Video Path Module Interrupt Handler
*
* @param[in]  inst     Handle to instance
*
*****************************************************************************/
void SiiModVidPathInterruptHandler(SiiInst_t inst);

/*****************************************************************************/
/**
* @brief Video Path Property Set API
*
* @param[in]  inst       Handle to instance
* @param[in]  opcode     Property's opcode
* @param[in]  inData	 input Data
*
*****************************************************************************/
bool_t SiiModVidpathSet(SiiInst_t inst, SiiModVidPathInternalOpcode_t opcode, void *inData);

/*****************************************************************************/
/**
* @brief Video Path Property Get API
*
* @param[in]  inst       Handle to instance
* @param[in]  opcode     Property's opcode
* @param[in]  outData	 out Data
*
*****************************************************************************/
bool_t SiiModVidpathGet(SiiInst_t inst, SiiModVidPathInternalOpcode_t opcode, void *outData);

#endif // __SI_DRV_VIDEOPATH_H__