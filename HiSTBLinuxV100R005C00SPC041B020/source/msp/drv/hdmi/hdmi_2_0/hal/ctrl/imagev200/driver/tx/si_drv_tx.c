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
* @file si_drv_tx.c
*
* @brief Tx API
*
*****************************************************************************/
//#define SII_DEBUG

/***** #include statements ***************************************************/

#include "si_datatypes.h"
#include "platform_api.h"
#include "sii_time.h"

#include "si_drv_tx_regs_enums.h"
#include "si_drv_tx_api.h"
#include "si_lib_log_api.h"
#include "si_lib_malloc_api.h"
#ifndef HDMI_BUILD_IN_BOOT
#include "si_lib_seq_api.h"
#endif
#include "si_lib_time_api.h"
#include "si_drv_cra_api.h"
#include "hdmi_platform.h"

/***** Register Module name **************************************************/

SII_LIB_OBJ_MODULE_DEF(drv_tx);

/***** public macro definitions **********************************************/
/***** local type definitions ************************************************/
typedef struct
{
    SiiDrvTxConfig_t		*pConfig;
    SiiDrvVidPathEvent_t    vidPath_events;
    SiiInst_t               instIntrHandler;

	SiiDrvTxEvent_t         tx_events;
	TxcbFunc				txEventNotifyCbFunc;
    SiiInst_t               instParam;
    
	SiiDrvHdcpStatus_t	 hdcpStatus;
	SiiInst_t            instHdmiTx;
    SiiInst_t            instMhlTx;
	SiiInst_t			 instHdcp;
    SiiInst_t            instVideoPath;

	SiiModHdmiTxConfig_t	hdmiConfig;
#ifdef HISI_TX_MHL    
	SiiModMhlTxConfig_t		mhlConfig;
#endif

#ifdef HDMI_HDCP_SUPPORT
	SiiModHdcpConfig_t		hdcpConfig;
#endif

	SiiDrvVideoPathCfg_t	vidPathConfig;

	bool_t isHdmiConnected;
	bool_t isMhlConnected;
} TxObj_t;


/***** local prototypes ******************************************************/
void SiiDrvTxHdmiModCallBack(SiiInst_t instTx, SiiDrvTxEvent_t eventFlags);
#ifdef HISI_TX_MHL
void SiiDrvTxMhlModCallBack(SiiInst_t instTx, SiiDrvTxEvent_t eventFlags);
#endif
#ifdef HDMI_HDCP_SUPPORT 
void SiiDrvTxHdcpModCallBack(SiiInst_t instTx, SiiDrvTxEvent_t eventFlags);
static void				sUpdateTxHdcpStatus(TxObj_t *pObj, SiiModHdcpState_t hdcpStatus, SiiModDsHdcpVersion_t dsHdcpVer);
#endif
#ifndef HDMI_BUILD_IN_BOOT
static void				sTxGroupInterruptHandler(SiiInst_t inst);
//static void				sTxLog(uint8_t *pData, uint16_t len);
#endif
static SiiDrvCraAddr_t	sBaseAddrGet(TxObj_t *pObj);
static SiiInst_t		sCraInstGet(TxObj_t* pObj);

/***** public functions ******************************************************/
//-------------------------------------------------------------------------------------------------
//! @brief      Initialize TX module
//-------------------------------------------------------------------------------------------------
SiiInst_t SiiDrvTxCreate(char *pNameStr, SiiDrvTxConfig_t *pConfig)
{
	TxObj_t *pObj = NULL;
	SiiDrvCraAddr_t         baseAddr;
	SiiInst_t				instCra;
    uint32_t                *pu32VirtualBaseAddr = NULL;
//    bool_t                  bPhyPowerEnable = HI_TRUE;
	
	/* Allocate memory for object */
	pObj = (TxObj_t*)SII_LIB_OBJ_CREATE(pNameStr, sizeof(TxObj_t));
	SII_PLATFORM_DEBUG_ASSERT(pObj);

	pObj->pConfig = (SiiDrvTxConfig_t*)SiiLibMallocCreate(sizeof(SiiDrvTxConfig_t));
	memcpy(pObj->pConfig, pConfig, sizeof(SiiDrvTxConfig_t));

	baseAddr = sBaseAddrGet(pObj);
	instCra  = sCraInstGet(pObj);
    pu32VirtualBaseAddr = pObj->pConfig->pu32CtrlVirBaseAddr;

	//Initialize
	pObj->hdcpStatus		= SII_DRV_HDCP_STATUS__OFF;
	pObj->isHdmiConnected	= false;
	pObj->isMhlConnected	= false;
    
#ifndef HDMI_BUILD_IN_BOOT
	// Create a timer to capture group interrupts
	pObj->instIntrHandler = SII_LIB_SEQ_TIMER_CREATE("Tx Interrupt Handler", sTxGroupInterruptHandler, SII_LIB_OBJ_INST(pObj), TIMER_START__TX_INTR_PRI, SII_TIMER_TX_INT);
	SII_PLATFORM_DEBUG_ASSERT(pObj->instIntrHandler);
	SiiLibSeqTimerStart(pObj->instIntrHandler, TIMER_START__TX_INTR, TIMER_START__TX_INTR_REPEAT);
#endif
	//Cypress Soft Reset
	//SiiDrvCraSetBit8(instCra, baseAddr | REG_ADDR__PWD_SRST, BIT_MSK__PWD_SRST__REG_SW_RST);

	//Creating HDMI Tx
	pObj->hdmiConfig.baseAddrTx			= baseAddr;
    pObj->hdmiConfig.pu32VirtuBaseAddr  = pu32VirtualBaseAddr;
	pObj->hdmiConfig.instTxCra			= instCra;
	pObj->hdmiConfig.instTx				= SII_LIB_OBJ_INST(pObj);
#ifndef HDMI_BUILD_IN_BOOT
	pObj->hdmiConfig.cbFunc	= SiiDrvTxHdmiModCallBack;
#endif

	pObj->instHdmiTx = SiiModHdmiTxCreate("tx_hdmi", &pObj->hdmiConfig);
	SII_PLATFORM_DEBUG_ASSERT(pObj->instHdmiTx);

#ifdef HISI_TX_MHL			
	if(pObj->pConfig->bMhlen)
	{
		//Creating MHL Tx
		pObj->mhlConfig.baseAddrTx		= baseAddr;
		pObj->mhlConfig.instTxCra		= instCra;
		pObj->mhlConfig.bMhl3En		    = pObj->pConfig->bMhl3en;
		pObj->mhlConfig.bEmscEn		    = pObj->pConfig->bEsmcEn;
		pObj->mhlConfig.bMDTEn			= pObj->pConfig->bMDTEn;
		pObj->mhlConfig.bProgramDino	= pObj->pConfig->bProgramDino;
		pObj->mhlConfig.instTx			= SII_LIB_OBJ_INST(pObj);
		pObj->mhlConfig.cbFunc			= SiiDrvTxMhlModCallBack;
		pObj->instMhlTx	= SiiModMhlTxCreate("tx_mhl", &pObj->mhlConfig);
		SII_PLATFORM_DEBUG_ASSERT(pObj->instMhlTx);
	}
#endif

#ifdef HDMI_HDCP_SUPPORT
	// create HDCP object
	pObj->hdcpConfig.baseAddrTx	    = baseAddr;
	pObj->hdcpConfig.bHdcpRepeat	= false;
	pObj->hdcpConfig.bHdcp2xEn		= pObj->pConfig->bHdcp2xEn;
	pObj->hdcpConfig.maxDsDev		= 127;
	pObj->hdcpConfig.instTxCra		= instCra;
	pObj->hdcpConfig.instTx		    = SII_LIB_OBJ_INST(pObj);
	pObj->hdcpConfig.cbFunc		    = SiiDrvTxHdcpModCallBack;
	pObj->instHdcp = SiiModHdcpCreate("tx_hdcp", &pObj->hdcpConfig);
	SII_PLATFORM_DEBUG_ASSERT(pObj->instHdcp);
#endif

	if(pObj->pConfig->bVidPathEn)
	{
		//Creating VideoPath
		pObj->vidPathConfig.instTxCra	= instCra;
		pObj->vidPathConfig.instTx		= SII_LIB_OBJ_INST(pObj);
		pObj->instVideoPath	= SiiModVideoPathCreate("VideoPath", &pObj->vidPathConfig);
		SII_PLATFORM_DEBUG_ASSERT(pObj->instVideoPath);
	}

	//clear reset
	//SiiDrvCraClrBit8(instCra, baseAddr | REG_ADDR__PWD_SRST, BIT_MSK__PWD_SRST__REG_SW_RST);

	return SII_LIB_OBJ_INST(pObj);
}

void SiiDrvTxDelete(SiiInst_t inst)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);

	/*--------------------------------*/
	/* Deallocate resources           */
	/* (reverse order)                */
	/*--------------------------------*/
	if(pObj->pConfig->bVidPathEn)
		SiiModVideoPathDelete(pObj->instVideoPath);

#ifdef HDMI_HDCP_SUPPORT
	SiiModHdcpDelete(pObj->instHdcp);
#endif

#ifdef HISI_TX_MHL	
	if(pObj->pConfig->bMhlen)
		SiiModMhlTxDelete(pObj->instMhlTx);
#endif
	SiiModHdmiTxDelete(pObj->instHdmiTx);
#ifndef HDMI_BUILD_IN_BOOT
    SiiLibSeqTimerDelete(pObj->instIntrHandler);
#endif
	SiiLibMallocDelete(pObj->pConfig);
	SII_LIB_OBJ_DELETE(pObj);
}
#ifndef HDMI_BUILD_IN_BOOT
//Register Application's callback function
void SiiDrvTxRegisterCallBack(SiiInst_t inst, TxcbFunc cbFunc, SiiInst_t data)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);
    
    pObj->instParam = data;
	pObj->txEventNotifyCbFunc = cbFunc;
}

//Tx Driver's Callback function - called by internal modules to notify respective events.
void SiiDrvTxHdmiModCallBack(SiiInst_t instTx, SiiDrvTxEvent_t eventFlags)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(instTx);
	
	//SiiModHdmiTxState_t hdmiState = SII_MOD_HDMI_STATUS__TMDS_OFF;
	//bool_t hdcpEnable = false;
	//bool_t avMute	  = false;
	bool_t hotPlug	  = false;
	
	if(eventFlags & (SII_DRV_TX_EVENT__HOT_PLUG_CHNG | SII_DRV_TX_EVENT__RSEN_CHNG))
	{
		SiiModHdmiTxGet(pObj->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__HOTPLUG, &hotPlug);
		if(hotPlug)
		{
			pObj->isHdmiConnected = true;
		}
		else
		{
#ifdef HDMI_HDCP_SUPPORT            
            SiiModHdcpSet(pObj->instHdcp, SII_MOD_HDCP_OPCODE__HDCP2x_RESET, &hotPlug);
#endif
			pObj->isHdmiConnected = false;
		}

#ifdef HDMI_HDCP_SUPPORT
		//Set Hot_Plug status in HDCP module.
		SiiModHdcpSet(pObj->instHdcp, SII_MOD_HDCP_OPCODE__TX_HPD_STATUS, &hotPlug);
#endif
	}
#if 0    
	if(eventFlags & SII_DRV_TX_EVENT__HDMI_STATE_CHNG)
	{
		//SiiModHdmiTxGet(pObj->pConfig->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__TMDS_STATUS, &tmdsStatus); //to read tmds_on/tmds_off
		//Get HDMI External State
		SiiModHdmiTxGet(pObj->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__HDMI_STATE, &hdmiState);
		
		if(hdmiState == SII_MOD_HDMI_STATUS__HDCP_ON) 
		{
			hdcpEnable	= true;
			avMute		= true;
			SiiModHdmiTxSet(pObj->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__AVMUTE, &avMute);
			SiiModHdcpSet(pObj->instHdcp, SII_MOD_HDCP_OPCODE__HDCP_ENABLE, &hdcpEnable);
		}
		else if(hdmiState == SII_MOD_HDMI_STATUS__HDCP_OFF)
		{
			hdcpEnable	= false;
			avMute		= true;
			SiiModHdmiTxSet(pObj->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__AVMUTE, &avMute);		//Set AVMUTE
			SiiModHdcpSet(pObj->instHdcp, SII_MOD_HDCP_OPCODE__HDCP_ENABLE, &hdcpEnable);		//Disable HDCP	
		}
	}
#endif 

	//Notifing to application.
	if(pObj->txEventNotifyCbFunc)
		pObj->txEventNotifyCbFunc(pObj->instParam, eventFlags);
}
#endif

#ifdef HDMI_HDCP_SUPPORT
//Tx Driver's HDCP Callback function - called by HDCP module to notify events.
void SiiDrvTxHdcpModCallBack(SiiInst_t instTx, SiiDrvTxEvent_t eventFlags)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(instTx);
	
	SiiModHdcpState_t		hdcpStatus;
	SiiModDsHdcpVersion_t	dsHdcpVer;

	if(eventFlags & SII_DRV_TX_EVENT__HDCP_STATE_CHNG)
	{
		SiiModHdcpGet(pObj->instHdcp, SII_MOD_HDCP_OPCODE__HDCP_STATUS, &hdcpStatus); //Get Hdcp Status from hdcp module
		SiiModHdcpGet(pObj->instHdcp, SII_MOD_HDCP_OPCODE__HDCP_VERSION, &dsHdcpVer); //Get DS Hdcp Version from hdcp module

		sUpdateTxHdcpStatus(pObj, hdcpStatus, dsHdcpVer);
		SiiModHdmiTxSet(pObj->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__HDCP_STATUS, &pObj->hdcpStatus); //Update Tx Hdcp Status in hdmi module
	}

	//Notifing to application.
	if(pObj->txEventNotifyCbFunc)
		pObj->txEventNotifyCbFunc(pObj->instParam,eventFlags);

}
#endif


#ifdef HISI_TX_MHL
void SiiDrvTxMhlModCallBack(SiiInst_t instTx, SiiDrvTxEvent_t eventFlags)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(instTx);
	SiiModMhlTxState_t mhlState;
	bool_t hdcpEnable = false;
	bool_t avMute	  = false;
	bool_t isMhl3Connected = false;	//Remove Later
	
	if(eventFlags & SII_DRV_TX_EVENT__MHL_STATE_CHNG)
	{
		//Get MHL External State
		SiiModMhlTxGet(pObj->instMhlTx, SII_MOD_MHL_TX_OPCODE__MHL_STATE, &mhlState);

		//Get MHL Cable Connection Type false(0)- MHL1/2 true(1) - MHL3
		SiiModMhlTxGet(pObj->instMhlTx, SII_MOD_MHL_TX_OPCODE__MHL_TYPE, &isMhl3Connected);	//Remove Later
		SiiModHdmiTxSet(pObj->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__UPDATE_MHL_CONN_TYPE, &isMhl3Connected);	//Remove Later

		if(SII_MOD_MHL_STATUS__CLR_HPD_RCVD == mhlState)
		{
			pObj->isMhlConnected = false;
			SiiModHdmiTxSet(pObj->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__UPDATE_MHL_CONN_STATUS, &mhlState);
		}
		if(SII_MOD_MHL_STATUS__SET_HPD_RCVD == mhlState)
		{
			pObj->isMhlConnected = true;
			SiiModHdmiTxSet(pObj->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__UPDATE_MHL_CONN_STATUS, &mhlState);

		}
		if(SII_MOD_MHL_STATUS__HDCP_ON == mhlState)
		{
			hdcpEnable	= true;
			avMute		= true;
			SiiModHdmiTxSet(pObj->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__AVMUTE, &avMute);	//Set AVMUTE
			SiiModHdcpSet(pObj->instHdcp, SII_MOD_HDCP_OPCODE__HDCP_ENABLE, &hdcpEnable);	//Enable HDCP	
		}
		else if(SII_MOD_MHL_STATUS__HDCP_OFF == mhlState)
		{
			hdcpEnable	= false;
			avMute		= true;
			SiiModHdmiTxSet(pObj->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__AVMUTE, &avMute);	//Set AVMUTE to avoid noise
			SiiModHdcpSet(pObj->instHdcp, SII_MOD_HDCP_OPCODE__HDCP_ENABLE, &hdcpEnable);	//Disable HDCP	
		}
	}
	if(eventFlags & SII_DRV_TX_EVENT_MHL_RCP_RECEIVED)
	{
		uint8_t rcpKeyCode = 0;
		//Get RCP KeyCode
		SiiModMhlTxGet(pObj->instMhlTx, SII_MOD_MHL_TX_OPCODE__RCP_KEYCODE, &rcpKeyCode);
		SII_LIB_LOG_DEBUG1(pObj, "Received an RCP key code = 0x%02x\n", rcpKeyCode);
		//If valid key, Send RCPK
		//Already done in cbus driver.
	}
	if(eventFlags & SII_DRV_TX_EVENT_MHL_RCPK_RECEIVED)
	{}
	if(eventFlags & SII_DRV_TX_EVENT_MHL_RCPE_RECEIVED)
	{}
	//Notifing to application.
	if(pObj->txEventNotifyCbFunc)
		pObj->txEventNotifyCbFunc(pObj->instParam, eventFlags);
}
#endif

#ifndef HDMI_BUILD_IN_BOOT
bool_t SiiDrvTxEdidGet(SiiInst_t inst, SiiEdid_t* pEdid)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);
	return SiiModHdmiTxGet(pObj->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__EDID, pEdid);
}

void SiiDrvTxLipSyncInfoGet(SiiInst_t inst, SiiLipSyncInfo_t* lipSync)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);

	SiiModHdmiTxGet(pObj->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__EDID_LIPSYNC, lipSync);
}

bool_t SiiDrvTxHotPlugStatusGet(SiiInst_t inst)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);
	
	bool_t hotPlug = false;
	SiiModHdmiTxGet(pObj->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__HOTPLUG, &hotPlug);
	
	return hotPlug;
}

bool_t SiiDrvTxRsenStatusGet(SiiInst_t inst)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);
	
	bool_t rsen = false;
	SiiModHdmiTxGet(pObj->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__RSEN, &rsen);
	
	return rsen;
}
#endif

void SiiDrvTxReset(SiiInst_t inst, SiiResetType_t resetSrc, bool_t onOff)
{
    SiiReset_t resetInfo;
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);
    
    resetInfo.resetSrc = resetSrc;
    resetInfo.reset    = onOff;
	SiiModHdmiTxSet(pObj->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__RESET, &resetInfo);
}

void SiiDrvTxHardwareInit(SiiInst_t inst)
{
    TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);
    SiiModHdmiTxSet(pObj->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__HW_INIT, NULL);
#ifdef HDMI_HDCP_SUPPORT
    SiiModHdcpSet(pObj->instHdcp, SII_MOD_HDCP_OPCODE__HDCP_HW_INIT, NULL);
#endif
    SiiModVidpathSet(pObj->instVideoPath, SII_MOD_VIDPATH__HW_INIT, NULL);
}

void SiiDrvTxSoftwareInit(SiiInst_t inst)
{
    TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);
	pObj->hdcpStatus		= SII_DRV_HDCP_STATUS__OFF;
	pObj->isHdmiConnected	= false;
	pObj->isMhlConnected	= false;
    SiiModHdmiTxSet(pObj->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__SW_INIT, NULL);
#ifdef HDMI_HDCP_SUPPORT
    SiiModHdcpSet(pObj->instHdcp, SII_MOD_HDCP_OPCODE__HDCP_SW_INIT, NULL);
#endif
    SiiModVidpathSet(pObj->instVideoPath, SII_MOD_VIDPATH__SW_INIT, NULL);   
}

#ifndef HDMI_BUILD_IN_BOOT
void SiiDrvTxHardwareStatusGet(SiiInst_t inst, SiiHardwareStatus_t *hwStatus)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);

	SiiModHdmiTxGet(pObj->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__HW_STATUS, &hwStatus->hdmiStatus);
	SiiModVidpathGet(pObj->instVideoPath, SII_MOD_VIDPATH__HW_STATUS, &hwStatus->videoStatus);

}
void SiiDrvTxHwScdcStatusGet(SiiInst_t inst, SiiScdcStatus_t *pScdcStatus)
{
    TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);
   
    SiiModHdmiTxGet(pObj->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__SCDC_STATUS, pScdcStatus);
}

void SiiDrvTxHwScdcStatusSet(SiiInst_t inst, SiiScdcStatus_t *pScdcStatus)
{
    TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);
   
    SiiModHdmiTxGet(pObj->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__SCDC_SET, pScdcStatus);
}
#endif

void SiiDrvTxInterruptEnableSet(SiiInst_t inst, SiiInterruptType_t intSrc, bool_t onOff)
{
    SiiInterrupt_t intInfo;
    TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);

    intInfo.intSrc    = intSrc;
    intInfo.enable    = onOff;
	SiiModHdmiTxSet(pObj->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__INT_ENABLE, &intInfo);    
}

void SiiDrvTxTmdsModeSet(SiiInst_t inst, SiiTmdsMode_t tmdsMode)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);

	SiiModHdmiTxSet(pObj->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__TMDS_MODE, &tmdsMode);
}

#ifdef HDMI_SCDC_SUPPORT
void SiiDrvTxScdcConfig(SiiInst_t inst, SiiModTxScdcSinKCaps_t *scdcSinkCaps)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);

	SiiModHdmiTxSet(pObj->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__SCDC_CONFIG, scdcSinkCaps);
}
#endif

#ifndef HDMI_BUILD_IN_BOOT
SiiTmdsMode_t SiiDrvTxTmdsModeStatusGet(SiiInst_t inst)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);
	
	SiiTmdsMode_t tmdsMode = SII_TMDS_MODE__NONE;
	SiiModHdmiTxGet(pObj->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__TMDS_MODE, &tmdsMode);
	
	return tmdsMode;
}

#ifdef HDMI_HDR_SUPPORT
void SiiDrvTxZeroDrmIfTimerStart(SiiInst_t inst, SiiHdrTimerCfg_S *pstTimer)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);
	SiiModHdmiTxSet(pObj->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__ZERO_DRMIF_TIMER_SET, pstTimer);
}

void SiiDrvTxHdrModeChangeTimerStart(SiiInst_t inst, SiiHdrTimerCfg_S *pstTimer)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);
	SiiModHdmiTxSet(pObj->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__HDR_MODE_CHANGE_TIMER_SET, pstTimer);
}
#endif
#endif

void SiiDrvTxAvMuteSet(SiiInst_t inst, bool_t onOff)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);
	SiiModHdmiTxSet(pObj->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__AVMUTE, &onOff);
}

void SiiDrvTxInfoframeSet(SiiInst_t inst, SiiInfoFrameId_t ifId, const SiiInfoFrame_t *pInfoFrame)
{
	TxObj_t*  pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);

	//SII_LIB_LOG_DEBUG1(pObj, "SiiDrvTxInfoframeSet():: ");
	SiiModHdmiTxSet(pObj->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__INFOFRAME_TYPE, pInfoFrame);
}

void SiiDrvTxInfoframeOnOffSet(SiiInst_t inst, SiiInfoFrameId_t ifId, bool_t onOff)
{	
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);
	uint8_t inData[]={ifId, onOff};

	SII_LIB_LOG_DEBUG1(pObj, "ifId: %d, onOff: %s\n", ifId, onOff ? "ON":"OFF");
	SiiModHdmiTxSet(pObj->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__INFOFRAME_ONOFF, inData);
}
#if 0
void SiiDrvTxInfoframeGet(SiiInst_t inst, SiiInfoFrameId_t ifId, SiiInfoFrame_t *pInfoFrame)
{
	TxObj_t*  pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);

	SII_LIB_LOG_DEBUG1(pObj, ("SiiDrvTxInfoframeGet():: "));
	SiiModHdmiTxGet(pObj->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__INFOFRAME_TYPE, pInfoFrame);
}

void SiiDrvTxInfoframeOnOffGet(SiiInst_t inst, SiiInfoFrameId_t ifId, bool_t* onOff)
{	
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);
	uint8_t inData[]={ifId, 0};

	SiiModHdmiTxGet(pObj->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__INFOFRAME_ONOFF, inData);
    *onOff = inData[1];
	SII_LIB_LOG_DEBUG1(pObj, ("SiiDrvTxInfoframeOnOffGet():: ifId: %i, onOff: %s\n", ifId, *onOff ? "ON":"OFF"));

}
#endif
#ifndef HDMI_BUILD_IN_BOOT
void SiiDrvTxChannelStatusSet(SiiInst_t inst, const SiiChannelStatus_t *pChannelStatus)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);
    char  tempBuffer[128];
    uint8_t  len = sizeof(SiiChannelStatus_t);
    uint8_t  offset = 0;
#if defined(CONFIG_HDMI_STB_SDK)
    uint8_t  *ptr = (uint8_t*)pChannelStatus;
#endif
    memset(tempBuffer, 0, 128);
    //offset += SII_SPRINTF((tempBuffer+offset,"%c", '\n'));
    while(len--)
    {
#if defined(CONFIG_HDMI_STB_SDK)
        offset += SII_SNPRINTF((tempBuffer+offset,128,"%02x ", *ptr++));
#endif

    }
    offset += SII_SNPRINTF((tempBuffer+offset,128,"%c", '\n'));
    SII_LIB_LOG_DEBUG2(tempBuffer);
    
	//SII_LIB_LOG_DEBUG1(pObj, "SiiDrvTxChannelStatusSet():: ");
	//sTxLog((uint8_t*)pChannelStatus, sizeof(SiiChannelStatus_t));

	SiiModHdmiTxSet(pObj->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__CHANNEL_STATUS, pChannelStatus);
}

void SiiDrvTxAudioEnableSet(SiiInst_t inst, bool_t onOff)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);
	SiiModHdmiTxSet(pObj->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__AUDIO_ENABLE, &onOff);
}

void SiiDrvTxAudioMuteSet(SiiInst_t inst, bool_t onOff)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);
	SiiModHdmiTxSet(pObj->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__AUDIO_MUTE, &onOff);
}

void SiiDrvTxAudioFormatSet(SiiInst_t inst, const SiiAudioFormat_t *pAudioFormat)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);
    char  tempBuffer[128];
    uint8_t  len = sizeof(SiiAudioFormat_t);
    uint8_t  offset = 0;
#if defined(CONFIG_HDMI_STB_SDK)
    uint8_t  *ptr = (uint8_t*)pAudioFormat;
#endif
    memset(tempBuffer, 0, 128);
    //offset += SII_SPRINTF((tempBuffer+offset,"%c", '\n'));
    while(len--)
    {
#if defined(CONFIG_HDMI_STB_SDK)
        offset += SII_SNPRINTF((tempBuffer+offset,128,"%02x ", *ptr++));
#endif
    }
    offset += SII_SNPRINTF((tempBuffer+offset,128,"%c", '\n'));
    SII_LIB_LOG_DEBUG2(tempBuffer);
	//SII_LIB_LOG_DEBUG1(pObj, "SiiDrvTxAudioFormatSet():: ");
	//sTxLog((uint8_t*)pAudioFormat, sizeof(SiiAudioFormat_t));
	
	SiiModHdmiTxSet(pObj->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__AUDIO_FORMAT, pAudioFormat);
}

SiiAudioFormat_t SiiDrvTxAudioFormatStatusGet(SiiInst_t inst)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);
	
	SiiAudioFormat_t audioFormat;
	SiiModHdmiTxGet(pObj->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__AUDIO_FORMAT, &audioFormat);

	return audioFormat;
}
#endif

void SiiDrvTxVideoMuteSet(SiiInst_t inst, bool_t onOff)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);

    SiiModVidpathSet(pObj->instVideoPath, SII_MOD_VIDPATH__MUTE, &onOff);
}


void SiiDrvTxVideoInputPramSet(SiiInst_t inst, SiiBlackPramInfo_t *pstBlackPram)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);

    SiiModVidpathSet(pObj->instVideoPath, SII_MOD_VIDPATH__VIDEO_INPUT_INFO, pstBlackPram);

    return;
}

void SiiDrvTxPatternTpgEnable(SiiInst_t inst, bool_t onOff)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);

    SiiModVidpathSet(pObj->instVideoPath, SII_MOD_VIDPATH__PATTERN_TPG_ENABLE, &onOff);

    return;
}

void SiiDrvTxVideoBlackDataSet(SiiInst_t inst, bool_t onOff)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);

    SiiModVidpathSet(pObj->instVideoPath, SII_MOD_VIDPATH__BLACK_DATA, &onOff);

    return;
}


void SiiDrvTxOutputBitDepthSet(SiiInst_t inst, SiiDrvBitDepth_t bitDepth)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);
	SiiModHdmiTxSet(pObj->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__OUTPUT_BIT_DEPTH, &bitDepth);
	SiiModVidpathSet(pObj->instVideoPath, SII_MOD_VIDPATH__BIT_DEPTH, &bitDepth);
}

void SiiDrvTxOutputMappingSet(SiiInst_t inst, bool_t bYCbcr422)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);
	SiiModVidpathSet(pObj->instVideoPath, SII_MOD_VIDPATH_OUT_MAPPING, &bYCbcr422);
}

#ifdef HDMI_HDCP_SUPPORT
void SiiDrvTxHdcpProtectionSet(SiiInst_t inst, bool_t onOff)
{
    bool_t hdcpEnable;
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);

    SiiDrvTxVideoBlackDataSet(inst, true);
	SiiDrvTxAudioMuteSet(inst, true);
            
    hdcpEnable = onOff ? true: false;
    SiiModHdcpSet(pObj->instHdcp, SII_MOD_HDCP_OPCODE__HDCP_HW_INIT, &hdcpEnable);
    SiiModHdcpSet(pObj->instHdcp, SII_MOD_HDCP_OPCODE__HDCP_ENABLE, &hdcpEnable);
	//SiiModHdmiTxSet(pObj->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__HDCP_PROTECTION, &onOff);
}

void SiiDrvTxHdcpVersionSet(SiiInst_t inst, SiiModDsHdcpVersion_t hdcpVer)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);
	SiiModHdcpSet(pObj->instHdcp, SII_MOD_HDCP_OPCODE__HDCP_VERSION, &hdcpVer);
}

void SiiDrvTxHdcpStateStatusGet(SiiInst_t inst, SiiDrvHdcpStatus_t *pHdcpStatus)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);
	*pHdcpStatus = pObj->hdcpStatus;
}

void SiiDrvTxHdcpCapabilityGet(SiiInst_t inst, SiiHdcpCaps_t *pstHdcpSupport)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);
    SiiModHdmiTxGet(pObj->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__HDCP_CAPABILITY, pstHdcpSupport);
}

void SiiDrvTxHdcpKsvListGet(SiiInst_t inst, SiiDrvHdcpKsvList_t *pBksvList)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);
	SiiModHdcpGet(pObj->instHdcp, SII_MOD_HDCP_OPCODE__HDCP_BKSV_LIST, pBksvList);
}

void SiiDrvTxHdcpKsvListApprovalSet(SiiInst_t inst, bool_t bApproved)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);
	SiiModHdcpSet(pObj->instHdcp, SII_MOD_HDCP_OPCODE__HDCP_BKSV_LIST_APPROVAL, &bApproved);
}

void SiiDrvTxHdcpTopologyGet(SiiInst_t inst,  SiiDrvHdcpTopology_t *pTopology)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);
	SiiModHdcpGet(pObj->instHdcp, SII_MOD_HDCP_OPCODE__HDCP_TOPOLOGY, pTopology);
}

void SiiDrvTxHdcp2ContentTypeSet(SiiInst_t inst, SiiDrvHdcpContentType_t *pContentType)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);
	SiiModHdcpSet(pObj->instHdcp, SII_MOD_HDCP_OPCODE__HDCP_CONTENT_TYPE, pContentType);

}

void SiiDrvTxHdcp2xCupdStatusGet(SiiInst_t inst, SiiDrvHdcp2xCupdChkStat_t *pHdcp2xCupdStat)
{
    TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);
	SiiModHdcpGet(pObj->instHdcp, SII_MOD_HDCP_OPCODE__HDCP2X_CUPD_STAT, pHdcp2xCupdStat);
}
#endif

//hsync vsync polarity set
void SiiDrvTxHvSyncPolaritySet(SiiInst_t inst, SiiOutPutSyncCfg_t *pstOutPutSyncCfg)
{
    TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);

    SiiModVidpathSet(pObj->instVideoPath, SII_MOD_VIDPATH__HV_SYNC_POLARITY, pstOutPutSyncCfg);
}

//video input format config:color space、color space standard、input deep color depth 
void SiiDrvTxColorInfoConfig(SiiInst_t inst, SiiDrvTxColorInfoCfg_t *clrInfo)
{
    TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);

	SiiModVidpathSet(pObj->instVideoPath, SII_MOD_VIDPATH__COLOR_INFO_CONFIG, clrInfo);
}

void SiiDrvTxCscConfig(SiiInst_t inst, SiiDrvTxCscCfg_t *pstCscInfo)
{
    TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);

    SiiModVidpathSet(pObj->instVideoPath, SII_MOD_VIDPATH_CSC_CONFIG, pstCscInfo);
}

void SiiDrvTxYuvRgbConvertion(SiiInst_t inst, SiiColorConvertion_t *convertion)
{
    TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);

	SiiModVidpathSet(pObj->instVideoPath, SII_MOD_VIDPATH_YUV_RGB_CONVERTION, convertion);
}
//video output format config:color space、color space standard
void SiiDrvTxOutputColorSpaceSet(SiiInst_t inst, SiiDrvTxClrSpc_t *clrSpc)
{
    TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);

	SiiModVidpathSet(pObj->instVideoPath, SII_MOD_VIDPATH__OUTPUT_COLORSPACE, clrSpc);
}

void SiiDrvTxPhyPowerOnOffSet(SiiInst_t inst, bool_t onOff)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);

	SiiModHdmiTxSet(pObj->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__PHY_POWER, &onOff);
}

void SiiDrvTxPhyConfig(SiiInst_t inst, SiiDrvPhyPara_t *pst_phy_param)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);

	SiiModHdmiTxSet(pObj->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__PHY_CONFIG, pst_phy_param);
}
void SiiDrvTxDdcSpeedConfig(SiiInst_t inst, uint32_t ddcspeed)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);

	SiiModHdmiTxSet(pObj->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__DDC_SPEED_CONFIG, &ddcspeed);
}

void SiiDrvTxTansCodeBypassConfig(SiiInst_t inst, bool_t b_enable)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);

	SiiModHdmiTxSet(pObj->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__BYPASS_CONFIG, &b_enable);
}

#ifndef HDMI_BUILD_IN_BOOT

bool_t SiiDrvTxPhyOutputEnableGet(SiiInst_t inst)
{
    bool_t enable;
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);

	SiiModHdmiTxGet(pObj->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__PHY_OE, &enable);
    return enable;
}
#endif
void SiiDrvTxPhyOutputEnableSet(SiiInst_t inst, bool_t enable)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);

	SiiModHdmiTxSet(pObj->instHdmiTx, SII_MOD_HDMI_TX_OPCODE__PHY_OE, &enable);
}

#ifdef HISI_TX_MHL
void SiiDrvTxEnableTDM(SiiInst_t inst, bool_t *enable)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);
	SiiModMhlTxSet(pObj->instMhlTx, SII_MOD_MHL_TX_OPCODE__ENABLE_TDM, enable);
}

void SiiDrvTxEnableMHL3AVLink(SiiInst_t inst, bool_t *enable)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);
	SiiModMhlTxSet(pObj->instMhlTx, SII_MOD_MHL_TX_OPCODE__ENABLE_MHL3_AVLINK, enable);
}

void SiiDrvTxEnableEMSCFloodTest(SiiInst_t inst)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);
	SiiModMhlTxSet(pObj->instMhlTx, SII_MOD_MHL_TX_OPCODE__ENABLE_EMSC_FLOODTEST, NULL);
}

void SiiDrvTxEnableMDTFloodTest(SiiInst_t inst)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);
	SiiModMhlTxSet(pObj->instMhlTx, SII_MOD_MHL_TX_OPCODE__ENABLE_MDT_FLOODTEST, NULL);
}

void SiiDrvTxPeerDevcapsGet(SiiInst_t inst, SiiDrvTxMHLDevCap_t *peerDevCaps)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);
	SiiModMhlTxGet(pObj->instMhlTx, SII_MOD_MHL_TX_OPCODE__READ_DEVCAPS, peerDevCaps);
}

void SiiDrvTxDevCapsSet(SiiInst_t inst, SiiDrvTxMHLDevCap_t *devCaps)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);
	SiiModMhlTxSet(pObj->instMhlTx, SII_MOD_MHL_TX_OPCODE__SET_DEVCAPS, devCaps);
}

void SiiDrvTxPeerXDevcapsGet(SiiInst_t inst, SiiDrvTxMHLXDevCap_t *peerXDevCaps)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);
	SiiModMhlTxGet(pObj->instMhlTx, SII_MOD_MHL_TX_OPCODE__READ_XDEVCAPS, peerXDevCaps);
}

void SiiDrvTxXDevCapsSet(SiiInst_t inst, SiiDrvTxMHLXDevCap_t *xDevCaps)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);
	SiiModMhlTxSet(pObj->instMhlTx, SII_MOD_MHL_TX_OPCODE__SET_XDEVCAPS, xDevCaps);
}

void SiiDrvTxCbusModeGet(SiiInst_t inst, SiiDrvTxCbusMode_t *cbusMode)
{
	TxObj_t* pObj = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);
	SiiModMhlTxGet(pObj->instMhlTx, SII_MOD_MHL_TX_OPCODE__CBUS_STATE, cbusMode);
}
#endif
#ifndef HDMI_BUILD_IN_BOOT
/***** local functions *******************************************************/
void SiiModTxDsEdidInterruptHandler(TxObj_t *pObj)
{
	SiiInst_t		craInst		= sCraInstGet(pObj);
	SiiDrvCraAddr_t	baseAddr	= sBaseAddrGet(pObj);

	uint8_t edidIntr = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__EDID_INTR);
	if(edidIntr)
	{
		SII_LIB_LOG_DEBUG1(pObj, "EDID Intr..................\n");
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__EDID_INTR, edidIntr);
	}
	//Disable Edid Intr Mask
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__EDID_INTR_MASK, 0x0);
}

//-------------------------------------------------------------------------------------------------
//! @brief      TX Interrupt handler.
//!
//!             Check for TX related interrupts. If found, clear pending hardware interrupt
//!             bits and change the status to indicate pending interrupt.
//!
//!             This function is to be called from the Device Interrupt manager upon receiving
//!             a hardware interrupt from TX.
//!
//! @param[in]  inst  - instance of the notification addressee
//-------------------------------------------------------------------------------------------------
static void sTxGroupInterruptHandler(SiiInst_t inst)
{
	TxObj_t*        pObj        = (TxObj_t*)SII_LIB_OBJ_PNTR(inst);
	SiiInst_t		craInst		= sCraInstGet(pObj);
	SiiDrvCraAddr_t	baseAddr	= sBaseAddrGet(pObj);

	if(SiiDrvCraIsInterruptRcvd(pObj->pConfig->instCra))
	{
		uint8_t l1_intr_stat_0 = 0;
		uint8_t l1_intr_stat_1 = 0;

		//Read All Group Interrupts here.
		l1_intr_stat_0 = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__L1_INTR_STAT_0);
		l1_intr_stat_1 = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__L1_INTR_STAT_1);
        if(l1_intr_stat_0 || l1_intr_stat_1)
//		SII_LIB_LOG_DEBUG1(pObj, "L1_INTR_STAT_0: %02x	L1_INTR_STAT_1: %02x\n", l1_intr_stat_0, l1_intr_stat_1);

		if(l1_intr_stat_0 & BIT_MSK__L1_INTR_STAT_0__L1_INTR_STAT_B0) //Check for INTR1 Interrupts
		{
			SiiModHdmiTxTpiInterruptHandler(pObj->instHdmiTx);//检测HOTPLUG信号、处理HOTPLUG事件
		}
    #ifdef HDMI_HDCP_SUPPORT     
		if(l1_intr_stat_0 & (BIT_MSK__L1_INTR_STAT_0__L1_INTR_STAT_B4 | BIT_MSK__L1_INTR_STAT_0__L1_INTR_STAT_B7)) //Check for Hdcp(TPI IP) or HDCP2x Group Interrupts
		{
			SiiModHdcpInterruptHandler(pObj->instHdcp);//Check for HDCP Interrupts
		}
    #endif    
    #ifdef HISI_TX_MHL
		if(pObj->pConfig->bMhlen && l1_intr_stat_0 & BIT_MSK__L1_INTR_STAT_0__L1_INTR_STAT_B5)	//Check for MHL IP Interrupts
		{
			SiiModCbusInterruptHandler(pObj->instMhlTx);
		}
    #endif
        if(pObj->pConfig->bVidPathEn && l1_intr_stat_1 & BIT_MSK__L1_INTR_STAT_1__L1_INTR_STAT_B10) //Check for VideoPath Interrupts
		{
			SiiModVidPathInterruptHandler(pObj->instVideoPath);
		}
    #ifdef HDMI_CEC_SUPPORT    
		if(pObj->pConfig->bCpiEn && l1_intr_stat_1 & BIT_MSK__L1_INTR_STAT_1__L1_INTR_STAT_B9) //Check for CEC IP Interrupts
		{
			pObj->txEventNotifyCbFunc(pObj->instParam, SII_DRV_CEC_EVENT);
		}
    #endif
		if(l1_intr_stat_1 & BIT_MSK__L1_INTR_STAT_1__L1_INTR_STAT_B11)
		{
			SiiModTxDsEdidInterruptHandler(pObj);
		}
	}
}
#endif
#ifdef HDMI_HDCP_SUPPORT 
//Update Tx HDCP Status
static void sUpdateTxHdcpStatus(TxObj_t *pObj, SiiModHdcpState_t hdcpStatus, SiiModDsHdcpVersion_t dsHdcpVer)
{
	switch (hdcpStatus)
	{
	case SII_MOD_HDCP_STATUS_OFF:
		pObj->hdcpStatus = SII_DRV_HDCP_STATUS__OFF;
		break;
	case SII_MOD_HDCP_STATUS_AUTHENTICATING:
		pObj->hdcpStatus = SII_DRV_HDCP_STATUS__AUTHENTICATING;
		break;
	case SII_MOD_HDCP_STATUS_SUCCESS:
		if(SII_DRV_DS_HDCP_VER__1X == dsHdcpVer)
			pObj->hdcpStatus = SII_DRV_HDCP_STATUS__SUCCESS_1X;
		else
			pObj->hdcpStatus = SII_DRV_HDCP_STATUS__SUCCESS_22;
		break;
	case SII_MOD_HDCP_STATUS_FAILURE:
		pObj->hdcpStatus = SII_DRV_HDCP_STATUS__FAILED;
		break;
	default:
		break;
	}
}
#endif

static SiiDrvCraAddr_t sBaseAddrGet(TxObj_t* pObj)
{
	return pObj->pConfig->baseAddr;
}

static SiiInst_t sCraInstGet(TxObj_t* pObj)
{
	return pObj->pConfig->instCra;
}
#if 0
static void sTxLog(uint8_t *pData, uint16_t len)
{
	int i = 0;
	SII_LIB_LOG_DEBUG2("\n");
	while(len--)
	{
		SII_LIB_LOG_DEBUG2(" %02X", *pData);
		pData++;
		if(++i == 0x10)
		{
			SII_LIB_LOG_DEBUG2("\n");
			i = 0;
		}

	}
	SII_LIB_LOG_DEBUG2("\n");
}
#endif
/***** end of file ***********************************************************/
