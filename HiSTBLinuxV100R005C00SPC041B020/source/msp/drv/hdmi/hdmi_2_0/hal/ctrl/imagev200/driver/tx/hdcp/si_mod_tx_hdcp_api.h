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
* @file si_mod_tx_hdcp_api.h
*
* @brief HDCP - Tx HDCP API
*
*****************************************************************************/
#ifndef __SI_DRV_TX_HDCP_API_H__
#define __SI_DRV_TX_HDCP_API_H__
#ifdef HDMI_HDCP_SUPPORT
/***** #include statements ***************************************************/
#include "si_drv_tx_api.h"

/***** public macro definitions **********************************************/

/***** public type definitions ***********************************************/

typedef uint32_t SiiModHdcpEvent_t;
typedef void (*HdcpEventNotifyCallBack)(SiiInst_t, SiiModHdcpEvent_t);

//HDCP Events
#define SII_MOD_HDCP_EVENT__HDCP_STATUS_CHNG		0x00000001		// HDCP status is changed


/**
* @brief HDCP Version
*/
typedef enum
{
	SII_DRV_DS_HDCP_VER__1X,                 //!< HDCP 1.4 Compliance
	SII_DRV_DS_HDCP_VER__22                  //!< HDCP 2.2 Compliance
} SiiModDsHdcpVersion_t;

//! Opcodes for Internal module variables.
typedef enum
{
	SII_MOD_HDCP_OPCODE__HDCP_PROTECTION,
	SII_MOD_HDCP_OPCODE__HDCP_ENABLE,
	SII_MOD_HDCP_OPCODE__HDCP_STATUS,
	SII_MOD_HDCP_OPCODE__HDCP_VERSION,
	SII_MOD_HDCP_OPCODE__HDCP_BKSV_LIST,
	SII_MOD_HDCP_OPCODE__HDCP_TOPOLOGY,
	SII_MOD_HDCP_OPCODE__HDCP_CONTENT_TYPE,
	SII_MOD_HDCP_OPCODE__HDCP2X_CUPD_STAT,
	SII_MOD_HDCP_OPCODE__HDCP_BKSV_LIST_APPROVAL,
	SII_MOD_HDCP_OPCODE__TX_HPD_STATUS,
	SII_MOD_HDCP_OPCODE__AVMUTE,
	SII_MOD_HDCP_OPCODE__HDCP_HW_INIT,
	SII_MOD_HDCP_OPCODE__HDCP_SW_INIT,
	SII_MOD_HDCP_OPCODE__HDCP_HW_STATUS,
	SII_MOD_HDCP_OPCODE__HDCP2x_RESET
} SiiModHdcpInternalOpcode_t;

typedef enum
{
	SII_MOD_HDCP_STATUS_OFF,            //!< Authentication has not been requested
	SII_MOD_HDCP_STATUS_AUTHENTICATING, //!< Authentication is in progress
	SII_MOD_HDCP_STATUS_SUCCESS,        //!< Authentication succeeded
	SII_MOD_HDCP_STATUS_FAILURE         //!< Authentication failed
} SiiModHdcpState_t;

//! HDCP control flags
#define HDCP_CTRL_MODE ( 0 \
	| BIT_MSK__TPI_COPP_DATA2__REG_COPP_PROTLEVEL \
	| BIT_MSK__TPI_COPP_DATA2__REG_DOUBLE_RI_CHECK \
	| BIT_MSK__TPI_COPP_DATA2__REG_KSV_FORWARD \
	)

//! HDCP Part 2 is successfully done
#define PART2_DONE (BIT_MSK__TPI_COPP_DATA1__REG_COPP_GPROT | BIT_MSK__TPI_COPP_DATA1__REG_COPP_LPROT)

//-------------------------------------------------------------------------------------------------
//  Module Instance Data
//-------------------------------------------------------------------------------------------------

typedef struct
{
	SiiDrvCraAddr_t      baseAddrTx;     //!< Base Addrress of Tx register space
	bool_t               bHdcpRepeat;    //!< true if enabled as HDCP repeater.
	bool_t				 bHdcp2xEn;
	uint8_t              maxDsDev;       //!< Maximum number down stream devices. Define 0 if KSV interrogation is not desired for transmitter mode.
	//tx driver instance
	SiiInst_t            instTx;
	SiiInst_t			 instTxCra;

	void				(*cbFunc)(SiiInst_t, SiiModHdcpEvent_t);		//!< hdcpEvents Notification callback funtion.
} SiiModHdcpConfig_t;


/***** public functions ******************************************************/

/*****************************************************************************/
/**
* @brief Tx HDCP driver constructor
*
* @param[in]  pNameStr   Name of instance
* @param[in]  pConfig    Static configuration parameters
*
* @retval                Handle to instance
*
*****************************************************************************/
SiiInst_t SiiModHdcpCreate(char *pNameStr, SiiModHdcpConfig_t *pConfig);

/*****************************************************************************/
/**
* @brief Tx driver destructor
*
* @param[in]  inst       Handle to instance
*
*****************************************************************************/
void SiiModHdcpDelete(SiiInst_t inst);

/*****************************************************************************/
/**
* @brief HDCP Interrupt Handler
*
* @param[in]  inst       Handle to instance
*
*****************************************************************************/
void SiiModHdcpInterruptHandler(SiiInst_t inst);

/*****************************************************************************/
/**
* @brief HDCP Property Set API
*
* @param[in]  inst       Handle to instance
* @param[in]  opcode     Property's opcode
* @param[in]  inData	 input Data
*
*****************************************************************************/
bool_t SiiModHdcpSet(SiiInst_t inst, SiiModHdcpInternalOpcode_t opcode, void *inData);

/*****************************************************************************/
/**
* @brief HDCP Property Get API
*
* @param[in]  inst       Handle to instance
* @param[in]  opcode     Property's opcode
* @param[in]  outData	 out Data
*
*****************************************************************************/
bool_t SiiModHdcpGet(SiiInst_t inst, SiiModHdcpInternalOpcode_t opcode, void *outData);

#endif
#endif // __SI_DRV_TX_HDCP_API_H__

/***** end of file ***********************************************************/
