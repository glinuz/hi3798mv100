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
 * @file si_drv_hdmi_tx_internal.h
 *
 * @brief HDMI Tx Internal API
 *
 *****************************************************************************/
#ifndef __SI_DRV_HDMI_TX_INTERNAL_H__
#define __SI_DRV_HDMI_TX_INTERNAL_H__

#include "si_drv_tx_api.h"

/***** public type definitions ***********************************************/
typedef uint32_t SiiModHdmiTxEvent_t;
typedef void (*HdmiTxEventNotifyCallBack)(SiiInst_t, SiiModHdmiTxEvent_t);

//! Opcodes for Internal module variables.
typedef enum
{
	SII_MOD_HDMI_TX_OPCODE__HOTPLUG,
	SII_MOD_HDMI_TX_OPCODE__RSEN,
	SII_MOD_HDMI_TX_OPCODE__EDID,
	SII_MOD_HDMI_TX_OPCODE__HDMI_STATE,
	SII_MOD_HDMI_TX_OPCODE__TMDS_MODE,
	SII_MOD_HDMI_TX_OPCODE__AVMUTE,
	SII_MOD_HDMI_TX_OPCODE__INFOFRAME_TYPE,
	SII_MOD_HDMI_TX_OPCODE__INFOFRAME_ONOFF,
	SII_MOD_HDMI_TX_OPCODE__CHANNEL_STATUS,
	SII_MOD_HDMI_TX_OPCODE__AUDIO_FORMAT,
	SII_MOD_HDMI_TX_OPCODE__OUTPUT_BIT_DEPTH,
	SII_MOD_HDMI_TX_OPCODE__CEC_PHY_ADDR,
	SII_MOD_HDMI_TX_OPCODE__HDCP_PROTECTION,
	SII_MOD_HDMI_TX_OPCODE__HDCP_STATUS,
	SII_MOD_HDMI_TX_OPCODE__HDCP_CAPABILITY,
	SII_MOD_HDMI_TX_OPCODE__HW_UPDATE_START,
	SII_MOD_HDMI_TX_OPCODE__UPDATE_MHL_CONN_STATUS,
	SII_MOD_HDMI_TX_OPCODE__UPDATE_MHL_CONN_TYPE,
	SII_MOD_HDMI_TX_OPCODE__EDID_LIPSYNC,
	SII_MOD_HDMI_TX_OPCODE__AUDIO_MUTE,
	SII_MOD_HDMI_TX_OPCODE__AUDIO_ENABLE,
	SII_MOD_HDMI_TX_OPCODE__RESET,
	SII_MOD_HDMI_TX_OPCODE__INT_ENABLE,
	SII_MOD_HDMI_TX_OPCODE__HW_INIT,
	SII_MOD_HDMI_TX_OPCODE__SW_INIT,
	SII_MOD_HDMI_TX_OPCODE__PHY_POWER,
	SII_MOD_HDMI_TX_OPCODE__PHY_OE,//output enable 
	SII_MOD_HDMI_TX_OPCODE__PHY_CONFIG,
	SII_MOD_HDMI_TX_OPCODE__DDC_SPEED_CONFIG,
	SII_MOD_HDMI_TX_OPCODE__BYPASS_CONFIG,
	SII_MOD_HDMI_TX_OPCODE__HW_STATUS,
	SII_MOD_HDMI_TX_OPCODE__SCDC_STATUS,
	SII_MOD_HDMI_TX_OPCODE__SCDC_SET,
	SII_MOD_HDMI_TX_OPCODE__SCDC_CONFIG,
	SII_MOD_HDMI_TX_OPCODE__ZERO_DRMIF_TIMER_SET,
	SII_MOD_HDMI_TX_OPCODE__HDR_MODE_CHANGE_TIMER_SET,
} SiiModHdmiTxInternalOpcode_t;

/**
* @brief tx states
*/
typedef enum
{
	SII_MOD_HDMI_TX_EVENT__TMDS_OFF,
	SII_MOD_HDMI_TX_EVENT__TMDS_ON,
	SII_MOD_HDMI_TX_EVENT__HDCP_STATE,
	SII_MOD_HDMI_TX_EVENT__HDCP_ON,
	SII_MOD_HDMI_TX_EVENT__HDCP_OFF,
	SII_MOD_HDMI_TX_EVENT__UPDATE_TMDS,
} SiiModHdmiTxInternalState_t;

/**
* @brief hdmi external states
*/
typedef enum
{
	SII_MOD_HDMI_STATUS__TMDS_OFF,
	SII_MOD_HDMI_STATUS__TMDS_ON,
	SII_MOD_HDMI_STATUS__HDCP_OFF,
	SII_MOD_HDMI_STATUS__HDCP_ON,	
} SiiModHdmiTxState_t;

typedef struct
{
	SiiDrvCraAddr_t      baseAddrTx;    //!< Base Addrress of Tx register space
    uint32_t             *pu32VirtuBaseAddr;
	SiiInst_t			 instTxCra;
	SiiInst_t			 instTx;		//!< Tx Controller's instance.
	void				(*cbFunc)(SiiInst_t, SiiModHdmiTxEvent_t);
#ifndef HDMI_BUILD_IN_BOOT
    HDMI_MUTEX           stDdcLock;
#endif
}SiiModHdmiTxConfig_t;

/***** HDMI Tx public functions ******************************************************/

/*****************************************************************************/
/**
* @brief HDMI Tx driver constructor
*
* @param[in]  pNameStr   Name of instance
* @param[in]  pConfig    Static configuration parameters
*
* @retval                Handle to instance
*
*****************************************************************************/
SiiInst_t SiiModHdmiTxCreate(char *pNameStr, SiiModHdmiTxConfig_t *pConfig);

/*****************************************************************************/
/**
* @brief HDMI Tx driver destructor
*
* @param[in]  inst       Handle to instance
*
*****************************************************************************/
void SiiModHdmiTxDelete(SiiInst_t inst);
#ifndef HDMI_BUILD_IN_BOOT
/*****************************************************************************/
/**
* @brief HDMI Tx TPI Interrupt Handler
*
* @param[in]  inst       Handle to instance
*
*****************************************************************************/
void SiiModHdmiTxTpiInterruptHandler(SiiInst_t inst);
#endif
/*****************************************************************************/
/**
* @brief HDMI Tx Property Set API
*
* @param[in]  inst       Handle to instance
* @param[in]  opcode     Property's opcode
* @param[in]  inData	 input Data
*
*****************************************************************************/
bool_t SiiModHdmiTxSet(SiiInst_t inst, SiiModHdmiTxInternalOpcode_t opcode, const void *inData);
#ifndef HDMI_BUILD_IN_BOOT
/*****************************************************************************/
/**
* @brief HDMI Tx Property Get API
*
* @param[in]  inst       Handle to instance
* @param[in]  opcode     Property's opcode
* @param[in]  outData	 out Data
*
*****************************************************************************/
bool_t SiiModHdmiTxGet(SiiInst_t inst, SiiModHdmiTxInternalOpcode_t opcode, void *outData);
#endif

#endif //__SI_DRV_HDMI_TX_INTERNAL_H__