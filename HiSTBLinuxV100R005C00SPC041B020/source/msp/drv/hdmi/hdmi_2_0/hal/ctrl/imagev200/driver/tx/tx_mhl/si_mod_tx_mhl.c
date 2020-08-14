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
* @file si_drv_tx_mhl.c
*
* @brief MHL Tx Driver
*
*****************************************************************************/
//#define SII_DEBUG
/***** #include statements ***************************************************/

#include "si_datatypes.h"
#include "si_drv_tx_regs_enums.h"
#include "si_drv_tx_api.h"
#include "si_mod_tx_cbus_emsc_internal.h"
#include "si_mod_tx_cbus_mdt_internal.h"
#include "si_mod_tx_cbus_defs.h"
#include "si_lib_edid_api.h"
#include "si_lib_log_api.h"
#include "si_lib_malloc_api.h"
#include "si_lib_seq_api.h"
#include "si_lib_time_api.h"
#include "sii_time.h"

/***** Register Module name **************************************************/

SII_LIB_OBJ_MODULE_DEF(drv_tx_mhl);

/***** global definitions ************************************************/
extern uint8_t rcpSupportTable[];

/***** local type definitions ************************************************/
typedef struct
{
	uint8_t disc_intr;
	uint8_t cbus_intr0;
	uint8_t cbus_intr1;
	uint8_t cbus_msc_agent_intr;
} MhlIntStat_t;

uint8_t gMhl_DeviceCapability[MHL_DEVCAP_SIZE] = 
{
	DEVCAP_VAL_DEV_STATE	,
	DEVCAP_VAL_MHL_VERSION	,
	DEVCAP_VAL_DEV_CAT		,
	DEVCAP_VAL_ADOPTER_ID_H	,
	DEVCAP_VAL_ADOPTER_ID_L	,	
	DEVCAP_VAL_VID_LINK_MODE,
	DEVCAP_VAL_AUD_LINK_MODE,
	DEVCAP_VAL_VIDEO_TYPE	,
	DEVCAP_VAL_LOG_DEV_MAP	,
	DEVCAP_VAL_BANDWIDTH	,	
	DEVCAP_VAL_FEATURE_FLAG	,		
	DEVCAP_VAL_DEVICE_ID_H	,		
	DEVCAP_VAL_DEVICE_ID_L	,	
	DEVCAP_VAL_SCRATCHPAD_SIZE,	
	DEVCAP_VAL_INT_STAT_SIZE,
	DEVCAP_VAL_RESERVED		,	
};

uint8_t gMhl_ScratchPad[MHL_SCRATCHPAD_SIZE] = 
{
	0,
};
uint8_t gMhl_XDeviceCapability[MHL_XDEVCAP_SIZE] = 
{
	XDEVCAP_VAL_ECBUS_SPEEDS	,
	XDEVCAP_VAL_TMDS_SPEEDS		,
	XDEVCAP_VAL_ECBUS_DEV_ROLES	,
	XDEVCAP_VAL_LOG_DEV_MAP		,
	XDEVCAP_VAL_RESERVED		,
	XDEVCAP_VAL_RESERVED		,
	XDEVCAP_VAL_RESERVED		,
	XDEVCAP_VAL_RESERVED		,
	XDEVCAP_VAL_RESERVED		,
	XDEVCAP_VAL_RESERVED		,
	XDEVCAP_VAL_RESERVED		,
	XDEVCAP_VAL_RESERVED		,
	XDEVCAP_VAL_RESERVED		,
	XDEVCAP_VAL_RESERVED		,
	XDEVCAP_VAL_RESERVED		,
	XDEVCAP_VAL_RESERVED		,
};

//MHL Tx Driver Data
typedef struct
{
	SiiModMhlTxConfig_t				*pConfig;
	MhlTxEventNotifyCbFunc			cbFunc;

	SiiModMhlTxInternalState_t	state;
	SiiModMhlTxDiscState_t		discState;
	SiiModMhlTxState_t			eState;
	SiiModMhlTxRgndState_t		rgnd;
	SiiDrvTxCbusMode_t			cbusMode;
	SiiTmdsMode_t				tmdsMode;
	SiiTmdsMode_t				prevTmdsMode;
	SiiEdid_t					edid;
	SiiLibEdidPar_t				parsedEdid;
	MhlIntStat_t				intStat;
	
	uint8_t		ucDevStatus[3];
	uint8_t		ucXDevStatus[4];
	SiiInst_t	timerHwUpdate;	//!< Timer for hw updates
	bool_t		bIsHdcpOn;
	bool_t		bWasHdcpOn;
	bool_t		hpd_status;
	bool_t		rsen_status;
	bool_t		cbus_ready;
	bool_t		cbus_connected;
	bool_t		cbus_abort;
	bool_t		isMhl3Connected;
	//MSC_MSG RCP received keyCode 
	uint8_t		rcpKeyCode;
	//Remember last MSC_MSG command (RCPE particularly)
	uint8_t		mscSaveRcpKeyCode;
	uint8_t		mscSaveUcpKeyCode;
	
	SiiDrvHdcpStatus_t	hdcpStatus;
	CbusState_t			cbus_state;
	SiiInst_t			emscInst;
	SiiInst_t			mdtInst;

	SiiDrvTxMHLDevCap_t			sDevCaps;
	SiiDrvTxMHLDevCap_t			peerDevCaps;
	SiiDrvTxMHLXDevCap_t		sXDevCaps;
	SiiDrvTxMHLXDevCap_t		peerXDevCaps;
} MhlTxObj_t;

/***** local prototypes ******************************************************/

static bool_t			sIsMhl3Enabled(MhlTxObj_t *pObj);
static SiiDrvCraAddr_t	sBaseAddrGet(MhlTxObj_t *pObj);
static SiiInst_t		sCraInstGet(MhlTxObj_t *pObj);
static void				sVirtualIsrHandler(MhlTxObj_t *pObj);
static void				sCbusTDMEnable(MhlTxObj_t *mhlObj, bool_t enable);
static void				sAvlinkEnable(MhlTxObj_t *mhlObj, bool_t bEnable, bool_t is_mhl3_connected);
static void				sUpdateTmdsMode(MhlTxObj_t *mhlObj);
static void				sNotifyMhlEvent(MhlTxObj_t *mhlObj, uint8_t eventFlag);
static void				sConfigDinoForMhl(MhlTxObj_t *mhlObj);
static int				sCbusIntr0IntrHandler(MhlTxObj_t *mhlObj);
static int				sCbusDiscIntrHandler(MhlTxObj_t *mhlObj);
static void				sUpdateCbusConnStatus(MhlTxObj_t *mhlObj);
static void				sUpdateCbusHpdStatus(MhlTxObj_t *mhlObj);
static void				sTmdsOutputControl(MhlTxObj_t *mhlObj, bool_t enable);
static void				sReadDevCaps(MhlTxObj_t *mhlObj);
static void				sReadExtDevCaps(MhlTxObj_t *mhlObj);
static void				setDevCaps(MhlTxObj_t *mhlObj);
static void				setXDevCaps(MhlTxObj_t *mhlObj);
static void				sSendDcapReady(MhlTxObj_t *mhlObj);

/***** local cbus prototypes ******************************************************/
static int						sCbusInit(MhlTxObj_t *pObj);
static bool_t					sCbusInitialize(MhlTxObj_t *pObj);
static bool_t					sCbusInitParam(MhlTxObj_t *pObj);
static bool_t					sCbusOutQReqTimeoutCount(MhlTxObj_t *pObj);
static bool_t					sCbusInQReqTimeoutCount(MhlTxObj_t *pObj);
static bool_t					sCbusWRITE_STAT_LINK_MODE(MhlTxObj_t *pObj, uint8_t ucLINK_MODE, PCOMMANDREPORT_t pCommandReport);
static bool_t					sCbusSET_INT_DCAP_CHG(MhlTxObj_t *pObj, PCOMMANDREPORT_t pCommandReport);
static bool_t					sCbusSET_INT_DSCR_CHG(MhlTxObj_t *pObj, PCOMMANDREPORT_t pCommandReport);
static bool_t					sCbusSET_INT_REQ_WRT(MhlTxObj_t *pObj, uint8_t* pArgument, uint8_t ucLength, PCOMMANDREPORT_t pCommandReport);
static bool_t					sCbusSET_INT_GRT_WRT(MhlTxObj_t *pObj, PCOMMANDREPORT_t pCommandReport);
static bool_t					sCbusSET_INT_3D_REQ(MhlTxObj_t *pObj, PCOMMANDREPORT_t pCommandReport);
static bool_t					sCbusSET_INT_EDID_CHG(MhlTxObj_t *pObj, PCOMMANDREPORT_t pCommandReport);
static bool_t					sCbusWRITE_STAT_DCAP_RDY(MhlTxObj_t *pObj, PCOMMANDREPORT_t pCommandReport);
static bool_t					sCbusWRITE_BURST(MhlTxObj_t *pObj, uint8_t* pArgument, uint8_t ucLength, PCOMMANDREPORT_t pCommandReport);
static bool_t					sCbusMSC_MSG_MSGE(MhlTxObj_t *pObj, PCOMMANDREPORT_t pCommandReport);
static bool_t					sCbusMSC_MSG_RCP(MhlTxObj_t *pObj, uint8_t ucKeycode, PCOMMANDREPORT_t pCommandReport);
static bool_t					sCbusMSC_MSG_RCPK(MhlTxObj_t *pObj, uint8_t ucKeycode, PCOMMANDREPORT_t pCommandReport);
static bool_t					sCbusMSC_MSG_RCPE(MhlTxObj_t *pObj, uint8_t ucStatusCode, PCOMMANDREPORT_t pCommandReport);
static bool_t					sCbusMSC_MSG_RAP(MhlTxObj_t *pObj, CBUS_API_MSC_COMMAND_t CbusMscCommand, uint8_t ucKeycode, PCOMMANDREPORT_t pCommandReport);
static bool_t					sCbusMSC_MSG_RAPK(MhlTxObj_t *pObj, uint8_t ucStatusCode, PCOMMANDREPORT_t pCommandReport);
static bool_t					sCbusMSC_MSG_UCP(MhlTxObj_t *pObj, uint8_t ucKeycode, PCOMMANDREPORT_t pCommandReport);
static bool_t					sCbusMSC_MSG_UCPK(MhlTxObj_t *pObj, uint8_t ucKeycode, PCOMMANDREPORT_t pCommandReport);
static bool_t					sCbusMSC_MSG_UCPE(MhlTxObj_t *pObj, uint8_t ucStatusCode, PCOMMANDREPORT_t pCommandReport);
static bool_t					sCbusMSC_SET_HOTPLUG(MhlTxObj_t *pObj, bool_t bSetHpd, PCOMMANDREPORT_t pCommandReport);
static bool_t					sCbusMscMsgRcpTimeoutHandler(MhlTxObj_t *pObj, uint8_t ucDirection);
static bool_t					sCbusMscMsgRapTimeoutHandler(MhlTxObj_t *pObj, uint8_t ucDirection);

static uint8_t					sCbusMscMsgTransferDone(MhlTxObj_t *pObj);
static bool_t					sCbusMscMsgRcpHandler(MhlTxObj_t *pObj);
static bool_t					sCbusMscMsgRapHandler(MhlTxObj_t *pObj);
static bool_t					sCbusMscMsgUcpHandler(MhlTxObj_t *pObj);

static bool_t					sCbusMscStartGetDevInfo(MhlTxObj_t *pObj);
static bool_t					sCbusMscGrtWrtRecv(MhlTxObj_t *pObj);

static bool_t					sCbusSetBurstToSend(MhlTxObj_t *pObj, bool_t bValue);
static bool_t					sCbusGetBurstToSend(MhlTxObj_t *pObj);
static bool_t					sCbusSetBurst(MhlTxObj_t *pObj, uint8_t *pCbusBurst);
static bool_t					sCbusBurstHandler(MhlTxObj_t *pObj);
static bool_t					sCbusRetDataHandler(MhlTxObj_t *pObj, uint8_t ucData);
static bool_t					sCbusSetVideoEntryIndex(MhlTxObj_t *pObj, uint8_t bValue);
static bool_t					sCbusAvMute(MhlTxObj_t *pObj, bool_t bMuteState);
static bool_t					sCbusGetAbortWait(MhlTxObj_t *pObj);
static bool_t					sCbusSetAbortWait(MhlTxObj_t *pObj, bool_t bValue);
static bool_t					sCbusMscMsgUcpTimeoutHandler(MhlTxObj_t *pObj, uint8_t ucDirection);

static bool_t					sCbusGetRcpSupport(MhlTxObj_t *pObj);
static bool_t					sCbusGetRapSupport(MhlTxObj_t *pObj);
static bool_t					sCbusGetUcpSendSupport(MhlTxObj_t *pObj);
static bool_t					sCbusGetReadDCAP(MhlTxObj_t *pObj);

static uint8_t *				sCbusGetPointer_PeerDeviceCapabiliy_RawData(MhlTxObj_t *pObj);
static uint8_t*					sCbusGetGeneralBurst(MhlTxObj_t *pObj);

static uint8_t					sCbusGetVideoEntryIndex(MhlTxObj_t *pObj);
static MhlVideoFormatData_t*	sCbusGetVideoFormatData(MhlTxObj_t *pObj);
static bool_t					sCbusGetPathEnable(MhlTxObj_t *pObj);

static bool_t					sCbusWriteBurstQueue(MhlTxObj_t *pObj, WRITEBURST_CIRCULARBUFFER_t * pBurstQueue, uint8_t *pWriteBurst);

static bool_t					sCbusGetDcapRdy(MhlTxObj_t *pObj);
static bool_t					sCbusSetDcapRdy(MhlTxObj_t *pObj, bool_t bValue);
static bool_t					sCbusSetPathEnable(MhlTxObj_t *mhlObj, bool_t bValue);
static bool_t					sCbusProcessMscMsgSubCommand (MhlTxObj_t *mhlObj);
static bool_t					sCbusWriteCommand (MhlTxObj_t *mhlObj, cbus_out_queue_t *pReq);
static bool_t					sCbus3DBuildBurstData(MhlTxObj_t *mhlObj);
static bool_t					sCbusProcessFailureInterrupts (MhlTxObj_t *mhlObj, uint8_t ucStatus);
static bool_t					sCbusInitDevCap(MhlTxObj_t *mhlObj);
static bool_t					sCbusInitXDevCap(MhlTxObj_t *mhlObj);
static bool_t					sCbusOutBurstHandler(MhlTxObj_t *mhlObj);
static uint8_t							sCbusRetryOutCommand(MhlTxObj_t *mhlObj);
static bool_t					sCbusSetReadDCAP(MhlTxObj_t *mhlObj, bool_t bValue);
static bool_t					sCbusWRITE_STAT_VERSION_STAT(MhlTxObj_t *mhlObj, PCOMMANDREPORT_t pCommandReport);
static bool_t					sCbusWRITE_STAT_Version_Stat_Done(MhlTxObj_t *mhlObj, uint8_t ucData);

static bool_t SiiCbusSendCommand(MhlTxObj_t *mhlObj, CBUS_API_MSC_COMMAND_t CbusMscCommand, uint8_t* pArgument, uint8_t ucLength,
							PCOMMANDREPORT_t pCommandReport);

/***** local data objects ****************************************************/
/***** public functions ******************************************************/

//-------------------------------------------------------------------------------------------------
//! @brief      Initialize MHL TX module
//-------------------------------------------------------------------------------------------------
SiiInst_t SiiModMhlTxCreate(char *pNameStr, SiiModMhlTxConfig_t *pConfig)
{
	MhlTxObj_t		*mhlObj = NULL;
	SiiModMdtConfig_t	mdtConfig;
	SiiDrvCraAddr_t	baseAddr;
	SiiInst_t		craInst;
	bool_t ret = 0;
	uint8_t val = 0;

	/* Allocate memory for mhl_tx object */
	mhlObj = (MhlTxObj_t*)SII_LIB_OBJ_CREATE(pNameStr, sizeof(MhlTxObj_t));
	SII_PLATFORM_DEBUG_ASSERT(mhlObj);

	mhlObj->pConfig = (SiiModMhlTxConfig_t*)SiiLibMallocCreate(sizeof(SiiModMhlTxConfig_t));
	memcpy(mhlObj->pConfig, pConfig, sizeof(SiiModMhlTxConfig_t));

	mhlObj->cbFunc = mhlObj->pConfig->cbFunc;

	if(mhlObj->pConfig->bMDTEn)
	{
		//Create Cbus_MDT obj
		mdtConfig.baseAddr  = mhlObj->pConfig->baseAddrTx;
		mdtConfig.craInst	= mhlObj->pConfig->instTxCra;
		mdtConfig.mhlInst	= SII_LIB_OBJ_INST(mhlObj);
		mhlObj->mdtInst		= SiiModCbusMdtInitialize("cbus_mdt", &mdtConfig);
		SII_PLATFORM_DEBUG_ASSERT(mhlObj->mdtInst);
	}

	if(mhlObj->pConfig->bMhl3En)
	{
		gMhl_DeviceCapability[DEVCAP_OFFSET_MHL_VERSION] = MHL_VERSION;
		//Create Cbus_EMSC obj
		if(mhlObj->pConfig->bEmscEn)
		{	SiiModEmscConfig_t	emscConfig;
			emscConfig.baseAddr = mhlObj->pConfig->baseAddrTx;
			emscConfig.craInst	= mhlObj->pConfig->instTxCra;
			emscConfig.mhlInst	= SII_LIB_OBJ_INST(mhlObj);
			mhlObj->emscInst = SiiModCbusEmscCreate("cbus_emsc", &emscConfig);
			SII_PLATFORM_DEBUG_ASSERT(mhlObj->emscInst);
		}
	}
	else
		gMhl_DeviceCapability[DEVCAP_OFFSET_MHL_VERSION] = MHL_VERSION_2;
	
	baseAddr = sBaseAddrGet(mhlObj);
	craInst  = sCraInstGet(mhlObj);

	/*--------------------------------*/
	/* Initialize states */
	/*--------------------------------*/
	mhlObj->state			= STATE_MHL_TMDS_OFF;
	mhlObj->discState		= STATE_MHL_DISCONNECTED;
	mhlObj->eState			= SII_MOD_MHL_STATUS__CLR_HPD_RCVD;
	mhlObj->rgnd			= RGND_UNKNOWN;
	mhlObj->cbusMode		= SII_DRV_TX_CBUS_MODE__NO_CONNECTION;
	mhlObj->tmdsMode		= SII_TMDS_MODE__NONE;
	mhlObj->prevTmdsMode	= SII_TMDS_MODE__NONE;
	mhlObj->bIsHdcpOn		= false;
	mhlObj->bWasHdcpOn		= false;
	mhlObj->hdcpStatus		= SII_DRV_HDCP_STATUS__OFF;

	mhlObj->hpd_status		= false;
	mhlObj->rsen_status		= false;
	mhlObj->cbus_connected	= false;
	mhlObj->isMhl3Connected	= false;

	mhlObj->cbus_ready = 0;
	mhlObj->cbus_abort = false;

	/*--------------------------------*/
	/* Initialize interrupts Status    */
	/*--------------------------------*/
	mhlObj->intStat.disc_intr			= 0x00;
	mhlObj->intStat.cbus_intr0			= 0x00;
	mhlObj->intStat.cbus_intr1			= 0x00;
	mhlObj->intStat.cbus_msc_agent_intr = 0x00;
	
	memset(mhlObj->ucDevStatus, 0, sizeof(mhlObj->ucDevStatus));
	memset(mhlObj->ucXDevStatus, 0, sizeof(mhlObj->ucXDevStatus));
	//Initialize Cbus
	ret = sCbusInit(mhlObj);

	//mhlObj->cbus_state.mhlInst = SII_LIB_OBJ_INST(mhlObj);
	return SII_LIB_OBJ_INST(mhlObj);
}

//-------------------------------------------------------------------------------------------------
//! @brief      Delete HDMI TX module
//-------------------------------------------------------------------------------------------------
void SiiModMhlTxDelete(SiiInst_t inst)
{
	MhlTxObj_t* mhlObj = (MhlTxObj_t*)SII_LIB_OBJ_PNTR(inst);

	if(mhlObj->pConfig->bMhl3En && mhlObj->pConfig->bEmscEn)
			SiiModCbusEmscDelete(mhlObj->emscInst);
	if(mhlObj->pConfig->bMDTEn)
		SiiModCbusMdtDelete(mhlObj->mdtInst);
	SiiLibMallocDelete(mhlObj->pConfig);
	SII_LIB_OBJ_DELETE(mhlObj);
}

//-------------------------------------------------------------------------------------------------
//! @brief      MHL TX Interrupt handler.
//-------------------------------------------------------------------------------------------------
void SiiModCbusInterruptHandler(SiiInst_t inst)
{
	//Read All Interrupts.
	MhlTxObj_t *mhlObj = (MhlTxObj_t *)SII_LIB_OBJ_PNTR(inst);
	bool_t bResult = false;

	SiiDrvCraAddr_t	baseAddr = sBaseAddrGet(mhlObj);
	SiiInst_t		craInst  = sCraInstGet(mhlObj);
	MhlIntStat_t	intStat;
	uint8_t ret = 0;

	if(mhlObj->pConfig->bEmscEn)
		SiiModCbusEmscIntrHandler(mhlObj->emscInst);
	
	intStat.disc_intr			= SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__CBUS_DISC_INTR0);
	intStat.cbus_intr0			= SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__CBUS_INT_0);
	intStat.cbus_intr1			= SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__CBUS_INT_1);
	intStat.cbus_msc_agent_intr = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__CBUS_MSC_AGENT_INTR);
	
	
	if(intStat.disc_intr || intStat.cbus_intr0 || intStat.cbus_intr1 || intStat.cbus_msc_agent_intr)
	SII_LIB_LOG_DEBUG1(mhlObj, ("Disc_Intr: %02x	Cbus_Intr1: %02x\n" \
		"Cbus_intr2: %02x	Cbus_MSC_Intr: %02x\n", intStat.disc_intr, intStat.cbus_intr0, intStat.cbus_intr1, intStat.cbus_msc_agent_intr));

	if(SII_MEMCMP(&mhlObj->intStat, &intStat, sizeof(MhlIntStat_t)))
	{
		uint8_t intreg = 0;

		// Find any interrupt status bit that changed to '1'
		intreg |= ((mhlObj->intStat.disc_intr ^ intStat.disc_intr) & intStat.disc_intr);
		intreg |= ((mhlObj->intStat.cbus_intr0 ^ intStat.cbus_intr0) & intStat.cbus_intr0);
		intreg |= ((mhlObj->intStat.cbus_intr1 ^ intStat.cbus_intr1) & intStat.cbus_intr1);
		intreg |= ((mhlObj->intStat.cbus_msc_agent_intr ^ intStat.cbus_msc_agent_intr) & intStat.cbus_msc_agent_intr);

		mhlObj->intStat = intStat;
		if(intreg)
		{
			// Call derived interupt handler
			sVirtualIsrHandler(mhlObj);

			bResult = sCbusOutQReqTimeoutCount(mhlObj);
			bResult = sCbusInQReqTimeoutCount(mhlObj);
		}
	}
}
//-------------------------------------------------------------------------------------------------
//! @brief      MHL TX Property Set API.
//-------------------------------------------------------------------------------------------------
bool_t SiiModMhlTxSet(SiiInst_t inst, SiiModMhlTxInternalOpcode_t opcode, const void *inData)
{
	MhlTxObj_t *mhlObj = (MhlTxObj_t *)SII_LIB_OBJ_PNTR(inst);
	SiiDrvCraAddr_t	baseAddr = sBaseAddrGet(mhlObj);
	SiiInst_t		craInst  = sCraInstGet(mhlObj);
	bool_t bEnable;
	switch(opcode)
	{
	case SII_MOD_MHL_TX_OPCODE__ENABLE_TDM:
		SII_MEMCPY(&(bEnable), inData, sizeof(bool_t));
		sCbusTDMEnable(mhlObj, bEnable);
		//Enable Cbus Interrupts.
		SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__CBUS_INT_0_MASK, 0xFF);
		SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__CBUS_INT_1_MASK, 0xCD);
		break;
	case SII_MOD_MHL_TX_OPCODE__ENABLE_MHL3_AVLINK:
		SII_MEMCPY(&(bEnable), inData, sizeof(bool_t));
		sAvlinkEnable(mhlObj, bEnable, true);
		break;
	case SII_MOD_MHL_TX_OPCODE__HDCP_STATUS:
		{
			SII_MEMCPY(&mhlObj->hdcpStatus, inData, sizeof(mhlObj->hdcpStatus));
			SiiLibSeqTimerStart(mhlObj->timerHwUpdate, TIMER_START__HW_UPDATE_5MS_WAIT, 0);
			break;
		}
	case SII_MOD_MHL_TX_OPCODE__SET_DEVCAPS:
		SII_MEMCPY(&(mhlObj->sDevCaps), inData, sizeof(SiiDrvTxMHLDevCap_t));
		setDevCaps(mhlObj);
		break;
	case SII_MOD_MHL_TX_OPCODE__SET_XDEVCAPS:
		setXDevCaps(mhlObj);
		SII_MEMCPY(&(mhlObj->sXDevCaps), inData, sizeof(SiiDrvTxMHLXDevCap_t));
		break;
	case SII_MOD_MHL_TX_OPCODE__ENABLE_EMSC_FLOODTEST:
		SiiModStartEmscFloodTest(mhlObj->emscInst);
		break;
	case SII_MOD_MHL_TX_OPCODE__ENABLE_MDT_FLOODTEST:
		SiiModStartMDTFloodTest(mhlObj->mdtInst);
		break;
	}
	return true;
}
//-------------------------------------------------------------------------------------------------
//! @brief      MHL TX Property Get API.
//-------------------------------------------------------------------------------------------------
bool_t SiiModMhlTxGet(SiiInst_t inst, SiiModMhlTxInternalOpcode_t opcode, void *outData)
{
	MhlTxObj_t *mhlObj = (MhlTxObj_t *)SII_LIB_OBJ_PNTR(inst);
	uint8_t ret = 0;

	switch(opcode)
	{
	case SII_MOD_MHL_TX_OPCODE__MHL_STATE:
		SII_MEMCPY(outData, &(mhlObj->eState), sizeof(mhlObj->eState));
		break;
	case SII_MOD_MHL_TX_OPCODE__CBUS_STATE:
		SII_MEMCPY(outData, &(mhlObj->cbusMode), sizeof(mhlObj->cbusMode));
		break;
	case SII_MOD_MHL_TX_OPCODE__RCP_KEYCODE:
		SII_MEMCPY(outData, &(mhlObj->rcpKeyCode), sizeof(mhlObj->rcpKeyCode));
		break;
	case SII_MOD_MHL_TX_OPCODE__READ_DEVCAPS:
		sReadDevCaps(mhlObj);
		SII_MEMCPY(outData, &(mhlObj->peerDevCaps), sizeof(mhlObj->peerDevCaps));
		break;
	case SII_MOD_MHL_TX_OPCODE__READ_XDEVCAPS:
		sReadExtDevCaps(mhlObj);
		SII_MEMCPY(outData, &(mhlObj->peerXDevCaps), sizeof(mhlObj->peerXDevCaps));
		break;
	case SII_MOD_MHL_TX_OPCODE__MHL_TYPE:
		SII_MEMCPY(outData, &(mhlObj->isMhl3Connected), sizeof(mhlObj->isMhl3Connected));	//0 - MHL1/2; 1 - MHL3
		break;		
	}
	return true;
}


static int sCbusIntr0IntrHandler(MhlTxObj_t *mhlObj)
{
	SiiDrvCraAddr_t	baseAddr = sBaseAddrGet(mhlObj);
	SiiInst_t		craInst  = sCraInstGet(mhlObj);
	bool_t ret = 0;

	//Handle Cbus Connection.
	if(mhlObj->intStat.cbus_intr0 & BIT_CBUS_CONNECTION_CHG_INT)
	{
		SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Cbus Connection Change\n"));
		//Update CBUS_Connection Status
		sUpdateCbusConnStatus(mhlObj);
	}

	//Handle WRITE_STATE if any.
	if(mhlObj->intStat.cbus_intr0 & BIT_MSC_WRITE_STATE_RCV_INT)
	{
		uint8_t mhl_stat_0 = 0;
		uint8_t mhl_stat_1 = 0;

		mhl_stat_0 = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__MHL_STAT_0);
		if(mhlObj->ucDevStatus[0] != mhl_stat_0)
		{
			mhlObj->ucDevStatus[0] = mhl_stat_0;
			if(mhl_stat_0 & BIT_ENUM__MHL_STAT_0__DCAP_READY)
			{
				SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Peer DevCap Ready\n"));
				if (!sCbusGetDcapRdy(mhlObj))                              // to read the dcap when DCAP_RDY 0->1
				{
					mhlObj->cbus_state.uwPendingEvents |= CBUS_READ_DCAP;

				}
				sCbusSetDcapRdy(mhlObj, true);
			}
			if(mhl_stat_0 & BIT_ENUM__MHL_STAT_0__XDEVCAP_SUPP)
			{
				SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Peer Supports Extended DEVCAP\n"));
				SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__CBUS_MSC_COMPATIBILITY_CONTROL, BIT_MSK__CBUS_MSC_COMPATIBILITY_CONTROL__REG_XDEVCAP_EN);
				mhlObj->cbus_state.bXDevcap_Supp = true;
			}
			if(mhl_stat_0 & BIT_ENUM__MHL_STAT_0__PLIM_STAT_MASK)
			{
				SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Peer PLIM Status Received\n"));
			}
		}
		mhl_stat_1 = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__MHL_STAT_1);
		if(mhlObj->ucDevStatus[1] != mhl_stat_1)
		{
			uint8_t changedBits = mhlObj->ucDevStatus[1] ^ mhl_stat_0;
			mhlObj->ucDevStatus[1] = mhl_stat_1;
			if(changedBits & BIT_ENUM__MHL_STAT_1__PATH_EN)
			{
				if(mhl_stat_1 & BIT_ENUM__MHL_STAT_1__PATH_EN)
				{
					SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: PATH_EN Set\n"));
					/*if (!sCbusGetPathEnable(mhlObj))                              // to start video when PATH_EN 0->1
					{
					mhlObj->cbus_state.uwPendingEvents |= CBUS_START_VIDEO;
					}*/
					sCbusSetPathEnable(mhlObj, true);
				}
				else
				{
					SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: PATH_EN Clear\n"));
					/*if (sCbusGetPathEnable(mhlObj))                              // to stop video when PATH_EN 1->0
					{
					mhlObj->cbus_state.uwPendingEvents |= CBUS_STOP_VIDEO;
					}*/
					sCbusSetPathEnable(mhlObj, false);
				}
			}
			if(changedBits & BIT_ENUM__MHL_STAT_1__MUTED)
			{
				if(mhl_stat_1 & BIT_ENUM__MHL_STAT_1__MUTED)
				{
					SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Muted Set\n"));
					sCbusAvMute(mhlObj, true);
				}
				else
				{
					SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Muted Clear\n"));
					sCbusAvMute(mhlObj, false);
				}
			}
		}
	}

	//Handle SET_INT if any.
	if(mhlObj->intStat.cbus_intr0 & BIT_MSC_SET_INT_RCV_INT)
	{
		uint8_t intr[4];
		static bool_t bScratchPadBusy = false;
		static uint32_t bScratchPadBusyBaseTime = 0;

		SiiDrvCraBlockRead8(craInst, baseAddr | REG_ADDR__MHL_INT_0, intr, 4);		//Read MHL Interrupts
		SiiDrvCraBlockWrite8(craInst, baseAddr | REG_ADDR__MHL_INT_0, intr, 4);		//Clear MHL Interrupts

		if(intr[0] & BIT_REQ_WRT_INT)
		{
			SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: SET_INT ( REQ_WRT ) Received\n"));

			//Scratch pad is free Or that Scratch Pad is busy, but busy duration is so long to timeout.
			if ((false == bScratchPadBusy)
				/* || ((SiI_get_global_time() - ulScratchPadBusyBaseTime) > DEM_CBUS_SCRATCH_PAD_BUSY_TIMEOUT)*/
				)
			{
				bScratchPadBusy = true;

				//Remember the starting time when scratch pad is busy.
				//ulScratchPadBusyBaseTime = SiI_get_global_time();
				mhlObj->cbus_state.uwPendingEvents |= CBUS_GRT_WRT;
			}
		}

		if(intr[0] & BIT_DSCR_CHG_INT)
		{
			SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: SET_INT ( DSCR_CHG ) Received\n"));

			bScratchPadBusy = false;

			//Push Burst Data into In_Queue, which would be handled in BurstHandler.
			//sCbusWriteBurstQueue(&(mhlObj->cbus_state.WriteBurstInQueue), (char XDATA *)(REG_CBUS_SCRATCHPAD_0));
		}

		if(intr[0] & BIT_GRT_WRT_INT)
		{
			SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: SET_INT ( GRT_WRT ) Received\n"));

			//If 3D ready, send 3D data in WRITE_BURST.
			//sCbusMscGrtWrtRecv();
		}

		if(intr[0] & BIT_3D_REQ_INT)
		{
			SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: SET_INT ( 3D_REQ ) Received\n"));

			sCbus3DBuildBurstData(mhlObj);
		}

		if (intr[0] & BIT_DCAP_CHG_INT)
		{
			SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: SET_INT ( DCAP_CHG ) Received\n"));

			if (sCbusGetReadDCAP(mhlObj))
			{
				mhlObj->cbus_state.uwPendingEvents |= CBUS_READ_DCAP;
			}
		}

		if(intr[1] & BIT_EDID_CHG_INT)
		{
			SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: SET_INT ( EDID_CHG ) Received\n"));
		}
	}

	//If MSC Command is sent successfully, invoke application callback function.
	if(mhlObj->intStat.cbus_intr0 & BIT_MSC_XFER_DONE_INT)
	{
		uint8_t mscData = 0;
		SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: MSC Command Transfer Done\n"));
		mscData = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__MSC_MT_RCVD_DATA0);
		//Clear this intr - Clearing after virtualIsr()
		ret = sCbusRetDataHandler(mhlObj, mscData);
	}



	//Handle MSC MSG if any. These MSC_MSGs include RCP, RAP and UCP.
	if(mhlObj->intStat.cbus_intr0 & BIT_MSC_MSG_RCV_INT)
	{
		SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: MSC_MSG Received\n"));
		ret = sCbusProcessMscMsgSubCommand(mhlObj);	// process the received MSC_MSG command immediately
	}

	//Handle WRITE_BURST if any.
	if(mhlObj->intStat.cbus_intr0 & BIT_MSC_WRITE_BURST_RCV_INT)
	{}

	if(mhlObj->intStat.cbus_intr0 & BIT_MSC_MSG_DONE_WITH_NACK_INT)
	{
		SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: MSC Transfer Done with NACK. Need retry...\n"));

		// retry to send out last command
		ret = sCbusRetryOutCommand(mhlObj);
	}

	//Handle Cbus HPD
	if(mhlObj->intStat.cbus_intr0 & BIT_CBUS_HPD_CHG_INT)
	{
		SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Cbus HPD Change\n"));
		//Temp calling from here. Remove Later - Bug34696
		sUpdateCbusConnStatus(mhlObj);
		//Update CBUS_HPD Status
		sUpdateCbusHpdStatus(mhlObj);
		sNotifyMhlEvent(mhlObj, SII_DRV_TX_EVENT__MHL_STATE_CHNG);
	}
	return ret;
}
static int sCbusDiscIntrHandler(MhlTxObj_t *mhlObj)
{
	SiiDrvCraAddr_t	baseAddr = sBaseAddrGet(mhlObj);
	SiiInst_t		craInst  = sCraInstGet(mhlObj);
	uint8_t disc_stat2 = 0, rgnd = 0, ret = 0;

	if(mhlObj->intStat.disc_intr & BIT_MHL12_EST_INT)
	{
		mhlObj->discState = STATE_MHL_ESTABLISHED;
		mhlObj->cbusMode = SII_DRV_TX_CBUS_MODE__oCBUS_MHL1_2_PEER;
		SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS: MHL mode established\n"));
		//Enable Cbus Interrupts.
		SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__CBUS_INT_0_MASK, 0xFF);
		SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__CBUS_INT_1_MASK, 0xCD);
		//sSendDcapReady(mhlObj);
	}
	if(mhlObj->intStat.disc_intr & BIT_NOT_MHL_EST_INT)
	{
		mhlObj->discState = STATE_NON_MHL_DETECTED;
		mhlObj->cbusMode = SII_DRV_TX_CBUS_MODE__oCBUS_UNKNOWN_PEER;
		SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS: Non MHL mode established\n"));
	}
	if(mhlObj->intStat.disc_intr & BIT_CBUS_MHL12_DISCON_INT)
	{
		mhlObj->discState = STATE_MHL_DISCONNECTED;
		mhlObj->cbusMode = SII_DRV_TX_CBUS_MODE__NO_CONNECTION;
		SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS : MHL disconnected\n"));
	}
	if(mhlObj->intStat.disc_intr & BIT_RGND_READY_INT)
	{
		mhlObj->discState = STATE_MHL_READY_RGND_DETECT;
		SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS : RGND is ready\n"));
		//Read RGND Status
		disc_stat2 = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__DISC_STAT2);
		SII_LIB_LOG_DEBUG1(mhlObj, ("DISC_STAT2 : %02x\n", disc_stat2));

		switch(disc_stat2 & RGND_STATUS_MASK)
		{
		case RGND_STATUS_OPEN:
			SII_LIB_LOG_DEBUG1(mhlObj, ("RGND Open\n"));
			mhlObj->rgnd = RGND_OPEN;
			mhlObj->discState = STATE_MHL_USB_CONNECTED;
			break;
		case RGND_STATUS_2K:
			SII_LIB_LOG_DEBUG1(mhlObj, ("RGND 2K\n"));
			mhlObj->rgnd = RGND_2K;
			break;
		case RGND_STATUS_1K:
			SII_LIB_LOG_DEBUG1(mhlObj, ("RGND 1K\n"));
			mhlObj->rgnd = RGND_1K;
			mhlObj->discState = STATE_MHL_CONNECTED;
			break;		
		case RGND_STATUS_SHORT:
			SII_LIB_LOG_DEBUG1(mhlObj, ("RGND Short\n"));
			break;
		}
	}
	if(mhlObj->discState == STATE_MHL_CONNECTED && mhlObj->rgnd == RGND_1K)
	{
		//Initialize Cbus Discovery and Wake Pulses
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DISC_CTRL3, 0x80);
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DISC_CTRL8, 0x0);
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DISC_CTRL9, 0x15);
	}
	
	return ret;
}

static void sUpdateCbusConnStatus(MhlTxObj_t *mhlObj)
{
	SiiDrvCraAddr_t	baseAddr = sBaseAddrGet(mhlObj);
	SiiInst_t		craInst  = sCraInstGet(mhlObj);
	uint8_t cbusConnState = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__CBUS_STATUS) & BIT_MSK__CBUS_STATUS__REG_CBUS_CONNECTED;

	if(cbusConnState)
	{
		SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Cbus Connected\n"));
		mhlObj->cbus_connected = true;
	}
	else
	{
		SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Cbus Disconnected\n"));
		mhlObj->cbus_connected = false;
		mhlObj->cbusMode = SII_DRV_TX_CBUS_MODE__NO_CONNECTION;
	}
}

static void sUpdateCbusHpdStatus(MhlTxObj_t *mhlObj)
{
	SiiDrvCraAddr_t	baseAddr = sBaseAddrGet(mhlObj);
	SiiInst_t		craInst  = sCraInstGet(mhlObj);
	uint8_t cbusHpd = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__CBUS_STATUS) & BIT_MSK__CBUS_STATUS__REG_CBUS_HPD;
	if(!cbusHpd)
		SiiLibTimeMilliDelay(100); //Check if it is really Low
	cbusHpd = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__CBUS_STATUS) & BIT_MSK__CBUS_STATUS__REG_CBUS_HPD;
	if(mhlObj->hpd_status != cbusHpd)
	{
		if(cbusHpd)
		{
			SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: HPD High\n"));
			mhlObj->hpd_status = true;
			mhlObj->eState = SII_MOD_MHL_STATUS__SET_HPD_RCVD;
		}
		else
		{
			SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: HPD Low\n"));
			mhlObj->hpd_status = false;
			mhlObj->eState = SII_MOD_MHL_STATUS__CLR_HPD_RCVD;
		}
	}
}

static void sVirtualIsrHandler(MhlTxObj_t *mhlObj)
{
	SiiDrvCraAddr_t baseAddr = sBaseAddrGet(mhlObj);
	SiiInst_t		craInst	 = sCraInstGet(mhlObj);
	
	bool_t ret = 0;
	if(mhlObj->intStat.disc_intr)
	{
		//Clear MHL Disc Interrupts
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__CBUS_DISC_INTR0, mhlObj->intStat.disc_intr); 
		ret = sCbusDiscIntrHandler(mhlObj);
	}

	if(mhlObj->intStat.cbus_intr0)
	{
		//Clear cbus_intr0 Interrupts
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__CBUS_INT_0, mhlObj->intStat.cbus_intr0);
		ret = sCbusIntr0IntrHandler(mhlObj);		
	}

	if(mhlObj->intStat.cbus_intr1)
	{
		//Clear cbus_intr1 interrupt.
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__CBUS_INT_1, mhlObj->intStat.cbus_intr1);

		if(mhlObj->intStat.cbus_intr1 & BIT_MHL_CONNECTION_CHG_INT)
		{
			SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: MHL Cable Connect Chng\n"));
		}

		if(mhlObj->intStat.cbus_intr1 & BIT_CBUS_PACKET_RCV_INT)
		{
			//Process Incoming Packet
		}
		ret = sCbusProcessFailureInterrupts(mhlObj, mhlObj->intStat.cbus_intr1);
	}
	
	if (mhlObj->cbus_state.uwPendingEvents)
    {
        if (CBUS_READ_DCAP & mhlObj->cbus_state.uwPendingEvents)
        {
			sCbusSetReadDCAP(mhlObj, false);
			if (sCbusMscStartGetDevInfo(mhlObj))
            {
                mhlObj->cbus_state.uwPendingEvents &= ~CBUS_READ_DCAP;
            }
        }

        if (CBUS_GRT_WRT & mhlObj->cbus_state.uwPendingEvents)
        {
            if (sCbusSET_INT_GRT_WRT(mhlObj, NULL))
            {
                mhlObj->cbus_state.uwPendingEvents &= ~CBUS_GRT_WRT;
            }
        }
    }

	if (!mhlObj->cbus_state.bEcbusSynced)
    {
        if (SII_BIT7 == (SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__TRXSTA2) & (SII_BIT7 | SII_BIT6)))
        {
            mhlObj->cbus_state.bEcbusSynced = true;
            SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: ECBUS is synced.\n"));
            if (mhlObj->cbus_state.bEcbusSynced & mhlObj->cbus_state.bEnableECBUS)
            {
                SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: ECBUS is connected.\n"));
            }
        }
    }

	mhlObj->intStat.disc_intr = 0x00;
	mhlObj->intStat.cbus_intr0 = 0x00;
	mhlObj->intStat.cbus_intr1 = 0x00;
	mhlObj->intStat.cbus_msc_agent_intr = 0x00;
}

static void sConfigDinoForMhl(MhlTxObj_t *mhlObj)
{
	SiiInst_t		craInst  = sCraInstGet(mhlObj);
	uint8_t val = 0;

	val = 0x01; SiiDCardRegBlockWrite(craInst, 0x72, 0x05, &val, 1);	// reset on
	val = 0x00; SiiDCardRegBlockWrite(craInst, 0x72, 0x05, &val, 1);	// reset off
	val = 0x35; SiiDCardRegBlockWrite(craInst, 0x72, 0x08, &val, 1);	// power en
	val = 0x20; SiiDCardRegBlockWrite(craInst, 0x72, 0x82, &val, 1);	// vid bypass
	val = 0x08; SiiDCardRegBlockWrite(craInst, 0x7A, 0x3C, &val, 1);	// core iso
	val = 0x8F; SiiDCardRegBlockWrite(craInst, 0x7A, 0x3D, &val, 1);
	val = 0x83; SiiDCardRegBlockWrite(craInst, 0x72, 0x0C, &val, 1);
	val = 0x59; SiiDCardRegBlockWrite(craInst, 0x72, 0x83, &val, 1);
	val = 0x19; SiiDCardRegBlockWrite(craInst, 0x72, 0x84, &val, 1);
	val = 0x00; SiiDCardRegBlockWrite(craInst, 0x72, 0x85, &val, 1);
}

static void sCbusTDMEnable(MhlTxObj_t *mhlObj, bool_t enable)
{
	SiiDrvCraAddr_t	baseAddr = sBaseAddrGet(mhlObj);
	SiiInst_t		craInst  = sCraInstGet(mhlObj);
	mhlObj->isMhl3Connected	 = enable;

	if(enable)
	{
		SiiDrvCraWrReg8(craInst, baseAddr | TXPHY_TOP_CTL0		, 0x20);	// reg_mhl3ce_en=1;
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__TRXCTRL			, 0x14);	// reg_trx_from_se_coc=0
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DISC_CTRL3		, 0xC0);	// reg_force_mhl=1
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DISC_CTRL5		, 0x0B);	// reg_dsm_ovride=1
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DISC_STAT1		, 0x08);	// reg_disc_sm=8
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__CBUS1_CNVT		, 0x89);	// CBUS1_CNVT;reg_cbus1cnvt_en=1
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DDC_TIMEOUT		, 0xFF);
		
		mhlObj->cbus_state.bEnableECBUS = true;
		mhlObj->cbusMode = SII_DRV_TX_CBUS_MODE__eCBUS_S;
	}
	else
	{
		SiiDrvCraWrReg8(craInst,  baseAddr | TXPHY_TOP_CTL0		, 0x90);	//back to HDMI Configuration
		SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__TRXCTRL			, 0x14);
		SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__DISC_CTRL3		, 0xC0);
		SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__DISC_CTRL5		, 0x0B);
		SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__DISC_STAT1		, 0x08);
		SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__CBUS1_CNVT		, 0x89);
		SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__DDC_TIMEOUT		, 0xFF);
		
		mhlObj->cbus_state.bEnableECBUS = false;
		mhlObj->cbusMode = SII_DRV_TX_CBUS_MODE__NO_CONNECTION;
	}
}

//Eanble MHL1/2 or MHL3 AVLINK based on mhl3_connected flag.
static void sAvlinkEnable(MhlTxObj_t *mhlObj, bool_t bEnable, bool_t is_mhl3_connected)
{
	SiiDrvCraAddr_t	baseAddr = sBaseAddrGet(mhlObj);
	SiiInst_t		craInst  = sCraInstGet(mhlObj);
	
	if(bEnable)
	{
		SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Enabling TMDS\n"));
		if(is_mhl3_connected)
		{
			SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__FUNC_SEL			, 0x02);	// Enable MHL3 for TX
		}
		else
		{
			SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__FUNC_SEL			, 0x00);
		}
		if(!is_mhl3_connected)
		{
			SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__MHL_MISC_CTL1		, 0x01);
			SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDMI2MHL1_CTL3	, 0x08);
		}
		else		
		{
			SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__HDMI2MHL3_CTRL	, 0x21);	// reg_encode_tmds, reg_master_en	
			SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__HDMI2MHL3_P0CTRL	, 0x21);	// Enable ECC mode, port0 enable
			SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDMI2MHL3_SRP0	, 0xFF);	// The period of SR symbol[7:0]
			SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDMI2MHL3_SRP1	, 0x01);	// The period of SR symbol[15:8]
			SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDMI2MHL3_SRP2	, 0x00);	// The period of SR symbol[23:16]
			SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDMI2MHL3_SRP3	, 0x00);	// The period of SR symbol[31:24]
			SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDMI2MHL3_SCTRL	, 0x40);	//disabling scrambling
		}
	}
	else
	{
		SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Disabling TMDS\n"));
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDMI2MHL1_CTL3	, 0x0);
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__MHL_MISC_CTL1		, 0x0);
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__FUNC_SEL			, 0x1);
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HOST_CTRL2		, 0x0);
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__SW_RST3			, 0x0);
		SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__HDMI2MHL3_CTRL	, 0x0);
		SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__HDMI2MHL3_P0CTRL	, 0x0);
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDMI2MHL3_SRP0	, 0x0);
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDMI2MHL3_SRP1	, 0x0);
	}

}
static void sTmdsOutputControl(MhlTxObj_t *mhlObj, bool_t enable)
{
	SiiDrvCraAddr_t baseAddr = sBaseAddrGet(mhlObj);
	SiiInst_t		craInst	 = sCraInstGet(mhlObj);

	if(enable)
	{
		SII_LIB_LOG_DEBUG1(mhlObj, ("TMDS Output Enabled\n"));
		SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__TX_PHY_TMDS_CTL, BIT_MSK__TX_PHY_TMDS_CTL__REG_TMDS_OE);
	}
	else
	{
		SII_LIB_LOG_DEBUG1(mhlObj, ("TMDS Output Disabled\n"));
		SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__TX_PHY_TMDS_CTL, BIT_MSK__TX_PHY_TMDS_CTL__REG_TMDS_OE);
	}
}

#if 0
static uint8_t	sReadDevCaps(MhlTxObj_t *mhlObj)
{
	uint8_t offset = 0, val = 0, cbus_intr1 = 0;
	SiiDrvCraAddr_t baseAddr = sBaseAddrGet(mhlObj);
	SiiInst_t		craInst	 = sCraInstGet(mhlObj);

	while(offset < 16)
	{
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__CBUS_INT_1, 0xFF);		//Clearing INT_1 Intr
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__MSC_CMD_OR_OFFSET, offset);	//Devcap offset
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__MSC_COMMAND_START, 0x4);	//Read Devcap

		cbus_intr1 = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__CBUS_INT_1);
		if((cbus_intr1 & 0x40) == 0x40)
		{
			SII_LIB_LOG_DEBUG1(mhlObj, ("Read Devcaps Failed\n"));
			//return -1;
		}
		if((cbus_intr1 & 0x20) == 0x20)
		{
			val = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__MSC_MT_RCVD_DATA0);
			SII_LIB_LOG_DEBUG2(("Offset = %d	value = %x\n", offset, val));
		}
		offset++;
	}
	return 0;
}
#endif
static bool_t sIsMhl3Enabled(MhlTxObj_t *pObj)
{
	return pObj->pConfig->bMhl3En;
}

static SiiDrvCraAddr_t sBaseAddrGet(MhlTxObj_t *pObj)
{
	return pObj->pConfig->baseAddrTx;
}

static SiiInst_t sCraInstGet(MhlTxObj_t *pObj)
{
	return pObj->pConfig->instTxCra;
}

static void sNotifyMhlEvent(MhlTxObj_t *mhlObj, uint8_t eventFlags)
{
	if(mhlObj->cbFunc)
		mhlObj->cbFunc(mhlObj->pConfig->instTx, eventFlags);
}

static void sPrintDevCaps(MhlTxObj_t *mhlObj)
{
	int i = 0x0;
	SII_LIB_LOG_DEBUG1(mhlObj, ("Device Capabilities : \n"));
	for(i = 0x0; i < 0x10; i++)
	{
		SII_LIB_LOG_DEBUG2(("offset = %0X	value = %0X\n", i, mhlObj->cbus_state.ucPeerDCAP[i]));
	}
}
static void sPrintXDevCaps(MhlTxObj_t *mhlObj)
{
	int i = 0x0;
	SII_LIB_LOG_DEBUG1(mhlObj, ("Extended Device Capabilities : \n"));
	for(i = 0x0; i < 0x10; i++)
	{
		SII_LIB_LOG_DEBUG2(("offset = %X	value = %0X\n", (i+0x80), mhlObj->cbus_state.ucPeerXDCAP[i]));
	}
}

/******Cbus Internal APIs***********************************************************************/
/*****************************************************************************/
/**
 * @Function:     SiiCbusSendCommand
 *
 * @Description:
 *
 *        Send CBUS command. CBUS would do it immediately.
 *
 * @Parameters: CbusMscCommand
 *
 *         Msc Command to send. For details, please see the data structure definition.
 *
 * @Parameters: pArgument
 *
 *         The pointer to the buffer, which might be used inside.
 *
 * @Parameters: ucLength
 *
 *         The length of data area indicated by pArgument.
 *
 * @Parameters: pCommandReport
 *
 *         If NOT null, this function will be invoked when command is sent or timeout.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t SiiCbusSendCommand(MhlTxObj_t *mhlObj, CBUS_API_MSC_COMMAND_t CbusMscCommand, uint8_t* pArgument, uint8_t ucLength,
                          PCOMMANDREPORT_t pCommandReport)
{
    bool_t bResult = false;

    if (true == sCbusGetAbortWait(mhlObj))
    {
        SII_LIB_LOG_DEBUG2(("CBUS:: CBUS is unavailable in Abort Waiting. \n"));
        return false;
    }

    switch (CbusMscCommand)
    {
        //Interrupt Register
    case    API_MSC_INTERRUPT_DCAP_CHG:
        bResult = sCbusSET_INT_DCAP_CHG(mhlObj, pCommandReport);

        break;

    case    API_MSC_INTERRUPT_DSCR_CHG:
        bResult = sCbusSET_INT_DSCR_CHG(mhlObj, pCommandReport);

        break;

    case    API_MSC_INTERRUPT_REQ_WRT:
        bResult = sCbusSET_INT_REQ_WRT(mhlObj, pArgument, ucLength, pCommandReport);

        break;

    case    API_MSC_INTERRUPT_GRT_WRT:
        bResult = sCbusSET_INT_GRT_WRT(mhlObj, pCommandReport);

        break;

    case    API_MSC_INTERRUPT_3D_REQ:
        bResult = sCbusSET_INT_3D_REQ(mhlObj, pCommandReport);
        break;

    case    API_MSC_INTERRUPT_EDID_CHG:
        bResult = sCbusSET_INT_EDID_CHG(mhlObj, pCommandReport);

        break;

        //Status Register
    case    API_MSC_STATUS_DCAP_RDY:
        bResult = sCbusWRITE_STAT_DCAP_RDY(mhlObj, pCommandReport);

        break;

    case    API_MSC_STATUS_LINK_MODE:
        if ((NULL != pArgument) && (1 == ucLength))
        {
            bResult = sCbusWRITE_STAT_LINK_MODE(mhlObj, pArgument[0], pCommandReport);
        }
        else
        {
            SII_LIB_LOG_DEBUG2(("Parameter Error!\n"));
        }

        break;
	case API_MSC_STATUS_VERSION_STAT:
		 bResult = sCbusWRITE_STAT_VERSION_STAT(mhlObj, pCommandReport);
		break;

        //Scraptchpad Register
    case    API_MSC_SCRATCHPAD_WRITE_BURST:
        if ((NULL != pArgument) && (16 == ucLength))
        {
            bResult = sCbusSET_INT_REQ_WRT(mhlObj, pArgument, ucLength, pCommandReport);
        }
        else
        {
            SII_LIB_LOG_DEBUG2(("Parameter Error!\n"));
        }

        break;

        //MSC_MSG
    case     API_MSC_MSGE:
        bResult = sCbusMSC_MSG_MSGE(mhlObj, pCommandReport);

        break;

    case    API_MSC_MSG_RCP:
        if (false == sCbusGetRcpSupport(mhlObj))
        {
            bResult = false;
            SII_LIB_LOG_DEBUG2(("RCP is not supported by Peer Device!\n"));
            break;
        }

        if ((NULL != pArgument) && (1 == ucLength))
        {
            bResult = sCbusMSC_MSG_RCP(mhlObj, pArgument[0], pCommandReport);
        }
        else
        {
            SII_LIB_LOG_DEBUG2(("Parameter Error!\n"));
        }

        break;

    case    API_MSC_MSG_RCPK:
        if (false == sCbusGetRcpSupport(mhlObj))
        {
            bResult = false;
            SII_LIB_LOG_DEBUG2(("RCP is not supported by Peer Device!\n"));
            break;
        }

        if ((NULL != pArgument) && (1 == ucLength))
        {
            bResult = sCbusMSC_MSG_RCPK(mhlObj, pArgument[0], pCommandReport);
        }
        else
        {
            SII_LIB_LOG_DEBUG2(("Parameter Error!\n"));
        }

        break;

    case    API_MSC_MSG_RCPE:
        if (false == sCbusGetRcpSupport(mhlObj))
        {
            bResult = false;
            SII_LIB_LOG_DEBUG2(("RCP is not supported by Peer Device!\n"));
            break;
        }

        if ((NULL != pArgument) && (1 == ucLength))
        {
            bResult = sCbusMSC_MSG_RCPE(mhlObj, pArgument[0], pCommandReport);
        }
        else
        {
            SII_LIB_LOG_DEBUG2(("Parameter Error!\n"));
        }

        break;

    case    API_MSC_MSG_RAP:
        if (false == sCbusGetRapSupport(mhlObj))
        {
            bResult = false;
            SII_LIB_LOG_DEBUG2(("RAP is not supported by Peer Device!\n"));
            break;
        }

        if ((NULL != pArgument) && (1 == ucLength))
        {
            bResult = sCbusMSC_MSG_RAP(mhlObj, API_MSC_MSG_RAP, pArgument[0], pCommandReport);
        }
        else
        {
            SII_LIB_LOG_DEBUG2(("Parameter Error!\n"));
        }

        break;

    case    API_MSC_MSG_RAPK:
        if (false == sCbusGetRapSupport(mhlObj))
        {
            bResult = false;
            SII_LIB_LOG_DEBUG2(("RAP is not supported by Peer Device!\n"));
            break;
        }

        if ((NULL != pArgument) && (1 == ucLength))
        {
            bResult = sCbusMSC_MSG_RAPK(mhlObj, pArgument[0], pCommandReport);
        }
        else
        {
            SII_LIB_LOG_DEBUG2(("Parameter Error!\n"));
        }

        break;

    case    API_MSC_MSG_UCP:
        if (false == sCbusGetUcpSendSupport(mhlObj))
        {
            bResult = false;
            SII_LIB_LOG_DEBUG2(("Sending UCP is not supported by Peer Device!\n"));
            break;
        }

        if ((NULL != pArgument) && (1 == ucLength))
        {
            bResult = sCbusMSC_MSG_UCP(mhlObj, pArgument[0], pCommandReport);
        }
        else
        {
            SII_LIB_LOG_DEBUG2(("Parameter Error!\n"));
        }

        break;

    case    API_MSC_MSG_UCPK:
        if (false == sCbusGetUcpSendSupport(mhlObj))
        {
            bResult = false;
            SII_LIB_LOG_DEBUG2(("Sending UCP is not supported by Peer Device!\n"));
            break;
        }

        if ((NULL != pArgument) && (1 == ucLength))
        {
            bResult = sCbusMSC_MSG_UCPK(mhlObj, pArgument[0], pCommandReport);
        }
        else
        {
            SII_LIB_LOG_DEBUG2(("Parameter Error!\n"));
        }

        break;

    case    API_MSC_MSG_UCPE:
        if (false == sCbusGetUcpSendSupport(mhlObj))
        {
            bResult = false;
            SII_LIB_LOG_DEBUG2(("Sending UCP is not supported by Peer Device!\n"));
            break;
        }

        if ((NULL != pArgument) && (1 == ucLength))
        {
            bResult = sCbusMSC_MSG_UCPE(mhlObj, pArgument[0], pCommandReport);
        }
        else
        {
            SII_LIB_LOG_DEBUG2(("Parameter Error!\n"));
        }

        break;
    case    API_MSC_SET_HOTPLUG:
        if ((NULL != pArgument) && (1 == ucLength))
        {
            bResult = sCbusMSC_SET_HOTPLUG(mhlObj, (bool_t)pArgument[0], pCommandReport);
        }
        else
        {
            SII_LIB_LOG_DEBUG2(("Parameter Error!\n"));
        }

        break;

    default:
        SII_LIB_LOG_DEBUG2(("Parameter Error!\n"));

        break;
    }

    return bResult;
}

/*****************************************************************************/
/**
 * @Function:     sCbusSwitchToEcbus
 *
 * @Description:	Return the status of current output command
 *
 * @Return:
 *
 *****************************************************************************/
static bool_t sCbusSwitchToEcbus (MhlTxObj_t *mhlObj)
{
	SiiDrvCraAddr_t	baseAddr = sBaseAddrGet(mhlObj);
	SiiInst_t		craInst  = sCraInstGet(mhlObj);

    if (mhlObj->cbus_state.bEnableECBUS & mhlObj->cbus_state.bEcbusSynced)
    {
        return true;
    }

	//Step 1: Select Coc PHY. Set MHL3 output. Set SWAP bit. Set TMDS encoding.
    //SiiDrvCraWrReg8(craInst, baseAddr | REG_MHL3_MODE_TX, BIT_MHL3_MODE_TDM_TX | BIT_ENC_TMDS_AV_LCLK | BIT_TMDS_SWAP_BIT | BIT_MHL3_MODE_TX); //{0x7A}[0x13] =  0x27	//reg_mhl3_mode_tdm_Tx  bit5   page 5

    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__TRXCTRL, BIT_MSK__TRXCTRL__REG_TRX_CLR_WVALLOW | (BIT_MSK__TRXCTRL__REG_TRX_NUMBPS_B2_B0 & SII_BIT2)); //{0x40}[0x3A] =  0x14	//reg_trx_from_se_coc Step1          default: 0x10     Page 10
	SiiLibTimeMilliDelay(300); //Establishing TDM connection will consume some time. This delay is necessary, otherwise it fails.

    //Step 2: Enable CBUS3 Converter.
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__CBUS1_CNVT, 0x80 | BIT_MSK__CBUS1_CNVT__REG_CBUS1CNVT_EN | BIT_MSK__CBUS1_CNVT__REG_TEARCBUS_EN);
    mhlObj->cbus_state.bEnableECBUS = true;

    //Step 3: Check eCBUS state if synced.
    //     Set register 0x04 of page10 to 0x07, this will enable reg_status_en. Otherwise the status will not updated.
    //     Check register 0x5B of page 10, the high 2 bits (bit7,6) show the tdm state.
    //     There are 3 possible value: 0-No sync, 1-wait sync, 2-synced. In normal state it should be 2.
    SiiLibTimeMilliDelay(100);
    if (SII_BIT7 == (SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__TRXSTA2) & (SII_BIT7 | SII_BIT6)))
    {
        mhlObj->cbus_state.bEcbusSynced = true;
        SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: ECBUS is connected.\n"));
        return true;
    }
    else
    {
        SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: ECBUS connection failed.\n"));
        return false;
    }

}

/*****************************************************************************/
/**
 * @Function:     sCbusSwitchToBcbus
 *
 * @Description:	Return the status of current output command
 *
 * @Return:
 *
 *****************************************************************************/
static bool_t sCbusSwitchToBcbus (MhlTxObj_t *mhlObj)
{
	SiiDrvCraAddr_t	baseAddr = sBaseAddrGet(mhlObj);
	SiiInst_t		craInst  = sCraInstGet(mhlObj);

    //Step 1: Disable CBUS3 Converter.
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__CBUS1_CNVT, 0x80);

	return true;
}


/*****************************************************************************/
/**
 * @Function:     sCbusOutQGetReqStatus
 *
 * @Description:	Return the status of current output command
 *
 * @Return:	0, CBUS_REQ_IDLE
 *			1, CBUS_REQ_PENDING
 *			2, CBUS_REQ_WAITING
 *
 *****************************************************************************/
static uint8_t sCbusOutQGetReqStatus(MhlTxObj_t *mhlObj)
{
    return (mhlObj->cbus_state.OutCommand.ucReqStatus);
}

/*****************************************************************************/
/**
 * @Function:     sCbusOutQSetReqStatus
 *
 * @Description:	Set the status of current output command
 *
 * @Parameters:		ucReqStatus
 *					0, CBUS_REQ_IDLE
 *					1, CBUS_REQ_PENDING
 *					2, CBUS_REQ_WAITING
 *
 * @Return: bool_t	true:  success
 *					false: failed
 *
 *****************************************************************************/
static bool_t sCbusOutQSetReqStatus(MhlTxObj_t *mhlObj, uint8_t ucReqStatus)
{
    mhlObj->cbus_state.OutCommand.ucReqStatus = ucReqStatus;
    return true;
}

/*****************************************************************************/
/**
 * @Function:     sCbusInQReqCmd
 *
 * @Description:
 *
 *         Return the opcode in MSC_MSG in input queue.
 *
 * @Return:
 *
 *        0x10, MSC_MSG_RCP
 *        0x11, MSC_MSG_RCPK
 *        0x12, MSC_MSG_RCPE
 *        0x20, MSC_MSG_RAP
 *        0x21, MSC_MSG_RAPK
 *
 *****************************************************************************/
static uint8_t sCbusInQReqCmd(MhlTxObj_t *mhlObj)
{
    return (mhlObj->cbus_state.InCommand.ucCommand);
}

/*****************************************************************************/

/**
 * @Function:     sCbusOutQReqCmd
 *
 * @Description:
 *
 *         Return the opcode in MSC_MSG in output queue
 *
 * @Return:
 *
 *        0x10, MSC_MSG_RCP
 *        0x11, MSC_MSG_RCPK
 *        0x12, MSC_MSG_RCPE
 *        0x20, MSC_MSG_RAP
 *        0x21, MSC_MSG_RAPK
 *
 *****************************************************************************/
static uint8_t sCbusOutQReqCmd(MhlTxObj_t *mhlObj)
{
    return (mhlObj->cbus_state.OutCommand.ucCommand);
}

/*****************************************************************************/

/**
 * @Function:     sCbusInQReqData
 *
 * @Description:
 *
 *         Return the KeyID in MSC_MSG
 *
 * @Return:
 *
 *        KeyID
 *
 *****************************************************************************/
static uint8_t sCbusInQReqData(MhlTxObj_t *mhlObj)
{
    return (mhlObj->cbus_state.InCommand.ucMsgData);
}

/*****************************************************************************/

/**
 * @Function:     sCbusOutQReqData0
 *
 * @Description:
 *
 *         Return Out-queue's active Req's ucMsgData[0]
 *
 * @Return:
 *
 *        the data[0] of MSC command
 *
 *****************************************************************************/
static uint8_t sCbusOutQReqData0(MhlTxObj_t *mhlObj)
{
    return (mhlObj->cbus_state.OutCommand.ucMsgData[0]);
}

/*****************************************************************************/

/**
 * @Function:     sCbusOutQReqData1
 *
 * @Description:
 *
 *         Return Out-queue's active Req's ucMsgData[1]
 *
 * @Return:
 *
 *        the data[1] of MSC command
 *
 *****************************************************************************/
static uint8_t sCbusOutQReqData1(MhlTxObj_t *mhlObj)
{
    return (mhlObj->cbus_state.OutCommand.ucMsgData[1]);
}

/*****************************************************************************/

/**
 * @Function:     sCbusOQReqData2
 *
 * @Description:
 *
 *         Return Out-queue's active Req's ucMsgData[2]
 *
 * @Return:
 *
 *        the data[2] of MSC command
 *
 *****************************************************************************/
static uint8_t sCbusOQReqData2(MhlTxObj_t *mhlObj)
{
    return (mhlObj->cbus_state.OutCommand.ucMsgData[2]);
}

/*****************************************************************************/

/**
 * @Function:     sCbusGetIQReq
 *
 * @Description:
 *
 *         Return In-queue's active Req
 *
 * @Return: cbus_in_queue_t
 *
 *        In-queue's active Req
 *
 *****************************************************************************/
static cbus_in_queue_t *sCbusGetIQReq(MhlTxObj_t *mhlObj)
{
    return (&mhlObj->cbus_state.InCommand);
}

/*****************************************************************************/

/**
 * @Function:     sCbusGetOutQReq
 *
 * @Description:
 *
 *         Return Out-queue's active Req
 *
 * @Return: cbus_out_queue_t
 *
 *        Out-queue's active Req
 *
 *****************************************************************************/
static cbus_out_queue_t *sCbusGetOutQReq(MhlTxObj_t *mhlObj)
{
    return (&mhlObj->cbus_state.OutCommand);
}

/*****************************************************************************/
/**
 * @Function:     sCbusInQCleanActiveReq
 *
 * @Description:	(1)Clean In-queue's active Req.
 *					(2)Set the ucReqStatus to IDLE and timeout value to 0
 *
 * @Return: bool_t	true:  success
 *					false: failed
 *
 *****************************************************************************/
static bool_t sCbusInQCleanActiveReq(MhlTxObj_t *pObj)
{
    cbus_in_queue_t *pReq;

	pReq = &pObj->cbus_state.InCommand; //sCbusIQActiveReq(channel);

    pReq->ucReqStatus = CBUS_REQ_IDLE;
    pReq->ulTimeout  = 0;
    pReq->bTimer_set = false;

    return true;
}

/*****************************************************************************/
/**
 * @Function:     sCbusOutQCleanActiveReq
 *
 * @Description:	(1)Clean Out-queue's active Req.
 *
 * @Return: bool_t	true:  success
 *					false: failed
 *
 *****************************************************************************/
static bool_t sCbusOutQCleanActiveReq(MhlTxObj_t *pObj)
{
	cbus_out_queue_t *pReq = &pObj->cbus_state.OutCommand;

    pReq->ucReqStatus = CBUS_REQ_IDLE;
    pReq->ucRetry   = 0;
    pReq->ulTimeout = 0;
    pReq->bVip = false;
    pReq->bTimer_set = false;
    pReq->dataRetHandler = NULL;
    pReq->sDataRetHandlerName = NULL;
    memset(pReq->ucMsgData, 0, MHL_MAX_BUFFER_SIZE);
    memset(pReq->ucMsgData2, 0, MHL_MAX_BUFFER_SIZE);

    return true;
}

/*****************************************************************************/

/**
 * @Function:     sCbusOQRetHandlerEmpty
 *
 * @Description:
 *
 *         Tell if there is any callback handler for current command in Out's queue
 *
 * @Return: bool_t
 *
 *        true:  Yes, there is.
 *        false: No, there isn't.
 *
 *****************************************************************************/
static bool_t sCbusOQRetHandlerEmpty(MhlTxObj_t *mhlObj)
{
    cbus_out_queue_t *pReq = &mhlObj->cbus_state.OutCommand;

    return (pReq->dataRetHandler == NULL);
}

/*****************************************************************************/

/**
 * @Function:     sCbusGetPathEnable
 *
 * @Description:
 *
 *         Get if Path_En interrupt has been received.
 *
 * @Return: bool_t
 *
 *        true:  Yes.
 *        false: No
 *
 *****************************************************************************/
static bool_t sCbusGetPathEnable(MhlTxObj_t *mhlObj)
{
    return (mhlObj->cbus_state.bPathEnRecv);
}

/*****************************************************************************/

/**
 * @Function:     sCbusSetPathEnable
 *
 * @Description:
 *
 *         Set if Path_En interrupt has been received.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusSetPathEnable(MhlTxObj_t *mhlObj, bool_t bValue)
{
    mhlObj->cbus_state.bPathEnRecv = bValue;
	
    return true;
}

/*****************************************************************************/
/**
 * @Function:     sCbusGetDcapRdy
 *
 * @Description:	Get if the Device Capability Registers in peer devices are ready to read.
 *
 * @Return: bool_t	true:  peer dcap reg ready
 *					false: peer dcap reg not ready
 *
 *****************************************************************************/
static bool_t sCbusGetDcapRdy(MhlTxObj_t *pObj)
{
	return (pObj->cbus_state.bDcapRdy);
}

/*****************************************************************************/
/**
 * @Function:     sCbusSetDcapRdy
 *
 * @Description:	Set if the Device Capability Registers in peer devices are ready to read.
 *
 * @Return: bool_t	true:  peer dcap reg ready
 *					false: peer dcap reg not ready
 *
 *****************************************************************************/
static bool_t sCbusSetDcapRdy(MhlTxObj_t *pObj, bool_t bValue)
{
    pObj->cbus_state.bDcapRdy = bValue;
    return true;
}

/*****************************************************************************/

/**
 * @Function:     sCbusGetReadDCAP
 *
 * @Description:
 *
 *         Get If Peer Device Capability Register has been read out.
 *
 * @Return: bool_t
 *
 *        true:  Peer Device Capability Registers have been read out and stored in ucPeerDCAP[]
 *        false: Peer Device Capability Registers have not been read out.
 *
 *****************************************************************************/
static bool_t sCbusGetReadDCAP(MhlTxObj_t *mhlObj)
{
    return (mhlObj->cbus_state.bReadDCAP);
}

/*****************************************************************************/

/**
 * @Function:     sCbusSetReadDCAP
 *
 * @Description:
 *
 *         Set If Peer Device Capability Register has been read out.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusSetReadDCAP(MhlTxObj_t *mhlObj, bool_t bValue)
{
    mhlObj->cbus_state.bReadDCAP = bValue;
    return true;
}

/*****************************************************************************/

/**
 * @Function:     sCbusSetRcpSupport
 *
 * @Description:
 *
 *         Set If Peer Device supports RCP.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusSetRcpSupport(MhlTxObj_t *mhlObj, bool_t bValue)
{
    mhlObj->cbus_state.bRCP_SUPPORT = bValue;
    return true;
}

/*****************************************************************************/

/**
 * @Function:     sCbusGetRcpSupport
 *
 * @Description:
 *
 *         Get If Peer Device supports RCP.
 *
 * @Return: bool_t
 *
 *        true:  Peer Device supports RCP.
 *        false: Peer Device does not support RCP.
 *
 *****************************************************************************/
static bool_t  sCbusGetRcpSupport(MhlTxObj_t *mhlObj)
{
    return mhlObj->cbus_state.bRCP_SUPPORT;
}

/*****************************************************************************/

/**
 * @Function:     sCbusSetRapSupport
 *
 * @Description:
 *
 *         Set If Peer Device supports RAP.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusSetRapSupport(MhlTxObj_t *mhlObj, bool_t bValue)
{
    mhlObj->cbus_state.bRAP_SUPPORT = bValue;
    return true;
}

/*****************************************************************************/

/**
 * @Function:     sCbusGetRapSupport
 *
 * @Description:
 *
 *         Get If Peer Device supports RAP.
 *
 * @Return: bool_t
 *
 *        true:  Peer Device supports RAP.
 *        false: Peer Device does not support RAP.
 *
 *****************************************************************************/
static bool_t  sCbusGetRapSupport(MhlTxObj_t *mhlObj)
{
    return mhlObj->cbus_state.bRAP_SUPPORT;
}

/*****************************************************************************/

/**
 * @Function:     sCbusSetSpSupport
 *
 * @Description:
 *
 *         Set If Peer Device supports Scratch Pad.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusSetSpSupport(MhlTxObj_t *mhlObj, bool_t bValue)
{
    mhlObj->cbus_state.bSP_SUPPORT = bValue;
    return true;
}

/*****************************************************************************/

/**
 * @Function:     sCbusGetSpSupport
 *
 * @Description:
 *
 *         Get If Peer Device supports Scratch Pad.
 *
 * @Return: bool_t
 *
 *        true:  Peer Device supports Scratch Pad.
 *        false: Peer Device does not support Scratch Pad.
 *
 *****************************************************************************/
static bool_t  sCbusGetSpSupport(MhlTxObj_t *mhlObj)
{
    return mhlObj->cbus_state.bSP_SUPPORT;
}

/*****************************************************************************/

/**
 * @Function:     sCbusSetUcpSendSupport
 *
 * @Description:
 *
 *         Set If Peer Device supports sending UCP.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusSetUcpSendSupport(MhlTxObj_t *mhlObj, bool_t bValue)
{
    mhlObj->cbus_state.bUCP_SEND_SUPPORT = bValue;
    return true;
}

/*****************************************************************************/

/**
 * @Function:     sCbusGetUcpSendSupport
 *
 * @Description:
 *
 *         Get If Peer Device supports sending UCP.
 *
 * @Return: bool_t
 *
 *        true:  Peer Device supports sending UCP.
 *        false: Peer Device does not support sending UCP.
 *
 *****************************************************************************/
static bool_t  sCbusGetUcpSendSupport(MhlTxObj_t *mhlObj)
{
    return mhlObj->cbus_state.bUCP_SEND_SUPPORT;
}

/*****************************************************************************/

/**
 * @Function:     sCbusSetUcpRecvSupport
 *
 * @Description:
 *
 *         Set If Peer Device supports receiving UCP.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusSetUcpRecvSupport(MhlTxObj_t *mhlObj, bool_t bValue)
{
    mhlObj->cbus_state.bUCP_RECV_SUPPORT = bValue;
    return true;
}

/*****************************************************************************/

/**
 * @Function:     sCbusGetUcpRecvSupport
 *
 * @Description:
 *
 *         Get If Peer Device supports receiving UCP.
 *
 * @Return: bool_t
 *
 *        true:  Peer Device supports receiving UCP.
 *        false: Peer Device does not support receiving UCP.
 *
 *****************************************************************************/
static bool_t  sCbusGetUcpRecvSupport(MhlTxObj_t *mhlObj)
{
    return mhlObj->cbus_state.bUCP_RECV_SUPPORT;
}

/*****************************************************************************/

/**
 * @Function:     sCbusSetAbortWait
 *
 * @Description:
 *
 *         Set itself in Abort Waiting due to Abort Packet is received. According to spec, if Abort packet
 *         is received, device must wait at least 2 seconds.
 *
 * @Parameters: b_AVMute
 *
 *        true:  Enable Abort Waiting
 *        false: Disable Abort Waiting
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusSetAbortWait(MhlTxObj_t *mhlObj, bool_t bValue)
{
    mhlObj->cbus_state.bAbortWait = bValue;

    if (bValue)
    {
        mhlObj->cbus_state.ulBase_time_abort = SiI_get_global_time();
    }
    else
    {
        mhlObj->cbus_state.ulBase_time_abort = 0;
    }

    return true;
}

/*****************************************************************************/

/**
 * @Function:     sCbusGetAbortWait
 *
 * @Description:
 *
 *         Get if device is in Abort Waiting.
 *
 * @Return: bool_t
 *
 *        true:  Device is in Abort Waiting.
 *        false: Device is NOT in Abort Waiting.
 *
 *****************************************************************************/
static bool_t sCbusGetAbortWait(MhlTxObj_t *mhlObj)
{
    return mhlObj->cbus_state.bAbortWait;
}

/*****************************************************************************/

/**
 * @Function:     sCbusOQReqAbort
 *
 * @Description:
 *
 *         Set out's queue in Abort Waiting.
 *         (1)Get into Abort Waiting for at least 2 seconds.
 *         (2)Clear all Commands in output quene except for VIP commands.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusOQReqAbort(MhlTxObj_t *mhlObj)
{
    cbus_out_queue_t *pReq;

    sCbusSetAbortWait(mhlObj, true);

    pReq = &mhlObj->cbus_state.OutCommand;

    if (pReq->ucRetry != 0)
    {
        pReq->ucRetry--;
        pReq->ucReqStatus = CBUS_REQ_PENDING;
    }
    else
    {
        sCbusOutQCleanActiveReq(mhlObj);
    }

    return true;
}

/*****************************************************************************/

/**
 * @Function:     sCbusRetDataHandler
 *
 * @Description:
 *
 *        This funciton will be invoked when command is transfered succesfully.
 *        It will call upper level callback function to inform caller.
 *
 * @Parameters: ucData
 *
 *        ucData = SiiDrvCraRdReg8(craInst, baseAddr | ( REG_CBUS_PRI_RD_DATA_1ST );
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusRetDataHandler(MhlTxObj_t *mhlObj, uint8_t ucData)
{
    uint8_t ucCommand, ucReqStatus;
    bool_t bResult = true;

    cbus_out_queue_t *pReq = sCbusGetOutQReq(mhlObj);

    //Step 1: Clear timing count.
    mhlObj->cbus_state.OutCommand.ulTimeout = 0;  // clean timeout count

    ucCommand   = sCbusOutQReqCmd(mhlObj);
    ucReqStatus = sCbusOutQGetReqStatus(mhlObj);

    if (ucReqStatus != CBUS_REQ_WAITING)
    {
        SII_LIB_LOG_DEBUG1(mhlObj, ("ERROR:: Logical error. Please check here.\n"));
        return (false);
    }

    //Step 2: Call internal callback function to clear itself.
    if (NULL != mhlObj->cbus_state.OutCommand.dataRetHandler)
    {
        bResult = mhlObj->cbus_state.OutCommand.dataRetHandler(mhlObj, ucData);
    }

	//Step 3: Save data before clear sending buffer.
    mhlObj->cbus_state.CbusCallBackParameters.bSent = true;
    mhlObj->cbus_state.CbusCallBackParameters.sFailureReason = NULL;
    mhlObj->cbus_state.CbusCallBackParameters.eSentCommand = pReq->CbusMscCommand;
    memcpy(mhlObj->cbus_state.CbusCallBackParameters.ucData, pReq->ucMsgData, MHL_MAX_BUFFER_SIZE);

    //Step 4: Call application callback funtion to notify outside.
    if (NULL != mhlObj->cbus_state.OutCommand.pCommandReport)
    {
		bResult = mhlObj->cbus_state.OutCommand.pCommandReport(mhlObj, &(mhlObj->cbus_state.CbusCallBackParameters));
    }

    return (bResult);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMscMsgHandler
 *
 * @Description:
 *
 *        This funciton will be invoked when MSC MSG command is received.
 *        (1)When NOT Dongle, it will invoke UI function to handle it and clear its input queue.
 *        (2)When Dongle mode, it will handle MSC MSG itself.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMscMsgHandler(MhlTxObj_t *mhlObj)
{
    bool_t bResult = true;
    uint8_t ucCommand = sCbusInQReqCmd(mhlObj);
    uint8_t ucAPICommand = 0;
    uint8_t ucData = sCbusInQReqData(mhlObj);

    switch (ucCommand)
    {
    case MHL_MSC_MSGE:
        sCbusInQCleanActiveReq(mhlObj);
        break;

    case MHL_MSC_MSG_RCP:
    case MHL_MSC_MSG_RCPK:
    case MHL_MSC_MSG_RCPE:
        bResult = sCbusMscMsgRcpHandler(mhlObj);
        break;

    case MHL_MSC_MSG_RAP:
    case MHL_MSC_MSG_RAPK:
        bResult = sCbusMscMsgRapHandler(mhlObj);
        break;

    case MHL_MSC_MSG_UCP:
    case MHL_MSC_MSG_UCPK:
    case MHL_MSC_MSG_UCPE:
        bResult = sCbusMscMsgUcpHandler(mhlObj);
        break;

    default:
        bResult = sCbusMSC_MSG_MSGE(mhlObj, NULL);
        break;
    }
    return (bResult);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMscMsgHandler
 *
 * @Description:
 *
 *        This funciton reduces the retry-counter by one.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusOQReduceReqRetry(MhlTxObj_t *mhlObj)
{
    mhlObj->cbus_state.OutCommand.ucRetry--;
    return true;
}

/*****************************************************************************/

/**
 * @Function:     sCbusOQReqRetry
 *
 * @Description:
 *
 *        Get the value of retry-counter, which means how many times of retry.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static uint8_t sCbusOQReqRetry(MhlTxObj_t *mhlObj)
{
    return (mhlObj->cbus_state.OutCommand.ucRetry);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMscMsgTimeoutHandler
 *
 * @Description:
 *
 *        The MSC MSG timeout handler. This function is only available in Dongle Mode.
 *
 * @Parameters: ucDirection
 *
 *        CBUS_IN
 *        CBUS_OUT
 *
 * @Return:
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static uint8_t sCbusMscMsgTimeoutHandler(MhlTxObj_t *mhlObj, uint8_t ucDirection)
{
    uint8_t bResult = true;
    uint8_t ucCommand;

    if (ucDirection == CBUS_IN)
    {
        ucCommand = sCbusInQReqCmd(mhlObj);

        if ((ucCommand == MHL_MSC_MSG_RCP) || (ucCommand == MHL_MSC_MSG_RCPK) || (ucCommand == MHL_MSC_MSG_RCPE))
        {
            bResult = sCbusMscMsgRcpTimeoutHandler(mhlObj, ucDirection);
        }
        else if ((ucCommand == MHL_MSC_MSG_RAP) || (ucCommand == MHL_MSC_MSG_RAPK))
        {
            bResult = sCbusMscMsgRapTimeoutHandler(mhlObj, ucDirection);
        }
        else if ((ucCommand == MHL_MSC_MSG_UCP) || (ucCommand == MHL_MSC_MSG_UCPK) || (ucCommand == MHL_MSC_MSG_UCPE))
        {
            bResult = sCbusMscMsgUcpTimeoutHandler(mhlObj, ucDirection);
        }
    }
    else if (ucDirection == CBUS_OUT)
    {
        ucCommand = sCbusOutQReqData0(mhlObj);

        if ((ucCommand == MHL_MSC_MSG_RCP) || (ucCommand == MHL_MSC_MSG_RCPK) || (ucCommand == MHL_MSC_MSG_RCPE))
        {
            bResult = sCbusMscMsgRcpTimeoutHandler(mhlObj, ucDirection);
        }
        else if ((ucCommand == MHL_MSC_MSG_RAP) || (ucCommand == MHL_MSC_MSG_RAPK))
        {
            bResult = sCbusMscMsgRapTimeoutHandler(mhlObj, ucDirection);
        }
        else if ((ucCommand == MHL_MSC_MSG_UCP) || (ucCommand == MHL_MSC_MSG_UCPK) || (ucCommand == MHL_MSC_MSG_UCPE))
        {
            bResult = sCbusMscMsgUcpTimeoutHandler(mhlObj, ucDirection);
        }
    }
    else
    {
        bResult = ERROR_WRONG_DIRECTION;
    }

    return (bResult);
}

/*****************************************************************************/

/**
 * @Function:     sCbusOQSetReqTimeout
 *
 * @Description:
 *
 *        Set Out-queue's active Req's timeout value
 *
 * @Parameters: ulValue
 *
 *        The value of timeout
 *
 * @Return:
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusOQSetReqTimeout(MhlTxObj_t *mhlObj, uint32_t ulValue)
{
    mhlObj->cbus_state.OutCommand.ulTimeout = ulValue;
    return true;
}

/*****************************************************************************/

/**
 * @Function:     sCbusMscMsgRcpTransferDoneHandler
 *
 * @Description:
 *
 *        In Dongle mode when RCP/RCPK/RCPE is sent out, this function will be invoked to
 *        set timer(RCP) or clear out's queue.
 *        Step 1: If the command sent is RCP, set timeout-timer to wait for RCPK.
 *        Step 2: If the command sent is RCPK, just clear the output queue because because this transaction is finished.
 *        Step 3: If the command sent is RCPE, still need to send RCPK.
 *
 * @Return:
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMscMsgRcpTransferDoneHandler(MhlTxObj_t *mhlObj)
{
    bool_t bResult = true;
    uint8_t ucCommand;

    ucCommand = sCbusOutQReqData0(mhlObj);

    //Step 1: If the command sent is RCP, set timeout-timer to wait for RCPK.
    if (ucCommand == MHL_MSC_MSG_RCP)
    {
        sCbusOQSetReqTimeout(mhlObj, DEM_MHL_RCP_TIMEOUT);  // set time out when sending out RCP/RAP 1000ms
    }
    //Step 2:  If the command sent is RCPK, just clear the output queue because because this transaction is finished.
    else if (ucCommand == MHL_MSC_MSG_RCPK)
    {
        sCbusOutQCleanActiveReq(mhlObj);
    }
    //Step 3: If the command sent is RCPE, still need to send RCPK.
    else if (ucCommand == MHL_MSC_MSG_RCPE)
    {
        uint8_t ucData;

        ucData = sCbusOQReqData2(mhlObj); // the RCP keycode store in RCPE output command
        sCbusOutQCleanActiveReq(mhlObj);
        bResult = sCbusMSC_MSG_RCPK(mhlObj, ucData, NULL); //SI_RcpSendAck( uData );
    }

    return (bResult);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMscMsgRapTransferDoneHandler
 *
 * @Description:
 *
 *        In Dongle mode when RAP/RAPK is sent out, this function will be invoked to
 *        set timer(RAP) or clear out's queue.
 *        Step 1:  If the command sent is RAP, set timeout-timer to wait for RAPK
 *        Step 2:  If the command sent is RAPK, just clear the output queue.
 *
 * @Return:
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMscMsgRapTransferDoneHandler(MhlTxObj_t *mhlObj)
{
    bool_t bResult = true;
    uint8_t ucCommand;

    ucCommand = sCbusOutQReqData0(mhlObj);

    //Step 1: If the command sent is RAP, set timeout-timer to wait for RAPK
    if (ucCommand == MHL_MSC_MSG_RAP)
    {
        sCbusOQSetReqTimeout(mhlObj, DEM_MHL_RCP_TIMEOUT);
    }
    //Step 2: If the command sent is RAPK, just clear the output queue.
    else if (ucCommand == MHL_MSC_MSG_RAPK)
    {
        if (sCbusOQRetHandlerEmpty(mhlObj))
        {
            sCbusOutQCleanActiveReq(mhlObj);
        }
    }

    return (bResult);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMscMsgUcpTransferDoneHandler
 *
 * @Description:
 *
 *        In Dongle mode when UCP/UCPK is sent out, this function will be invoked to
 *        set timer(UCP) or clear out's queue.
 *        Step 1:  If the command sent is UCP, set timeout-timer to wait for UCPK.
 *        Step 2:  If the command sent is UCPK, it means this transaction is finished. Clear output queue.
 *        Step 3:  If the command sent is UCPE, it still needs to send UCPK.
 *
 * @Return:
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static uint8_t sCbusMscMsgUcpTransferDoneHandler(MhlTxObj_t *mhlObj)
{
    uint8_t result = 0;
    uint8_t command;

    command = sCbusOutQReqData0(mhlObj);
    switch (command)
    {
        //Step 1: If the command sent is UCP, set timeout-timer to wait for UCPK.
    case MHL_MSC_MSG_UCP:
    {
        sCbusOQSetReqTimeout(mhlObj, DEM_MHL_RCP_TIMEOUT);
    }
        break;

        //Step 2: If the command sent is UCPK, it means this transaction is finished. Clear output queue.
    case MHL_MSC_MSG_UCPK:
    {
        sCbusOutQCleanActiveReq(mhlObj);
    }
        break;

        //Step 3: If the command sent is UCPE, it still needs to send UCPK.
    case MHL_MSC_MSG_UCPE:
    {
        uint8_t uData;

        uData = sCbusInQReqData(mhlObj);
        sCbusOutQCleanActiveReq(mhlObj);
        result = sCbusMSC_MSG_UCPK(mhlObj, uData, NULL);
    }
        break;
    default:
        SII_LIB_LOG_DEBUG1(mhlObj, ("UCP:: Unknown Command Code: 0x%02X\n", (int)command));
    }

    return (result);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMscMsgTransferDone
 *
 * @Description:
 *
 *        In Dongle mode when MSC MSG is sent out, this function will be invoked.
 *
 * @Return:
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static uint8_t sCbusMscMsgTransferDone(MhlTxObj_t *mhlObj)
{
    uint8_t bResult = true;
    uint8_t ucCommand;

    ucCommand = sCbusOutQReqData0(mhlObj);

    //Step 1: Handle RCP class MSG
    if ((ucCommand == MHL_MSC_MSG_RCP) || (ucCommand == MHL_MSC_MSG_RCPK) || (ucCommand == MHL_MSC_MSG_RCPE))
    {
        bResult = sCbusMscMsgRcpTransferDoneHandler(mhlObj);
    }
    //Step 2: Handle RAP class MSG
    else if ((ucCommand == MHL_MSC_MSG_RAP) || (ucCommand == MHL_MSC_MSG_RAPK))
    {
        bResult = sCbusMscMsgRapTransferDoneHandler(mhlObj);
    }
    //Step 3: Handle UCP class MSG
    else if ((ucCommand == MHL_MSC_MSG_UCP) || (ucCommand == MHL_MSC_MSG_UCPK) || (ucCommand == MHL_MSC_MSG_UCPE))
    {
        bResult = sCbusMscMsgUcpTransferDoneHandler(mhlObj);
    }
    //Step 4: Handle MSGE class MSG
    else if (ucCommand == MHL_MSC_MSGE)
    {
        sCbusOutQCleanActiveReq(mhlObj);
    }

    return (bResult);
}

/*****************************************************************************/

/**
 * @Function:     sCbusInQReqTimeoutCount
 *
 * @Description:
 *
 *        Handle timeout events in output queue.
 *        Step 1: If some request is suspended, begin to timing.
 *        Step 2: If some request is suspended and timeout, call timeout handler.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusInQReqTimeoutCount(MhlTxObj_t *mhlObj)
{
    bool_t bResult = true;
    cbus_in_queue_t *pReq;

    pReq = sCbusGetIQReq(mhlObj);

    //Step 1: If some request is suspended, begin to timing.
    if (pReq->ucReqStatus == CBUS_REQ_PENDING)
    {
        if (!pReq->bTimer_set)
        {
            pReq->bTimer_set  = true;
            pReq->ulBase_time = SiI_get_global_time();
        }
        else
        {
            //Step 2: If some request is suspended and timeout, call timeout handler.
            if (SiI_TimerDelay(pReq->ulBase_time, pReq->ulTimeout))
            {
                pReq->ulBase_time = 0;
                pReq->bTimer_set = false;
                bResult = sCbusMscMsgTimeoutHandler(mhlObj, CBUS_IN);
            }
        }
    }

    return (bResult);
}

/*****************************************************************************/

/**
 * @Function:     sCbusOutQReqTimeoutCount
 *
 * @Description:
 *
 *        Handle timeout events in output queue.
 *        Step 1: Check if Cbus is in Abort Waiting and Cbus has waiting enough time.
 *        Step 2: Check if it is time out to transfer present MSC command.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusOutQReqTimeoutCount(MhlTxObj_t *mhlObj)
{
    bool_t bResult = true;
    cbus_out_queue_t *pReq = &mhlObj->cbus_state.OutCommand;

    //Step 1: Check if Cbus is in Abort Waiting and Cbus has waiting enough time.
    if (true == sCbusGetAbortWait(mhlObj))
    {
        if (SiI_TimerDelay(mhlObj->cbus_state.ulBase_time_abort, DEM_CBUS_ABORT_WAIT))
        {
            sCbusSetAbortWait(mhlObj, false);
        }
    }

    //Step 2: Check if it is time out to transfer present MSC command.
    if (pReq->ucReqStatus == CBUS_REQ_WAITING)
    {
        if (!pReq->bTimer_set)
        {
            pReq->bTimer_set  = true;
            pReq->ulBase_time = SiI_get_global_time();
        }
        else
        {
            if (SiI_TimerDelay(pReq->ulBase_time, pReq->ulTimeout))
            {
                pReq->ulBase_time = 0;
                pReq->bTimer_set = false;

                if (pReq->ucRetry != 0)
                {
                    pReq->ucRetry--;
                    bResult = sCbusWriteCommand(mhlObj, pReq);
                    pReq->ulBase_time = SiI_get_global_time();
                    pReq->ucReqStatus = CBUS_REQ_WAITING;
                    pReq->ulTimeout = DEM_MSC_MAX_REPLY_DELAY;
                }
                else
                {
                    sCbusOutQCleanActiveReq(mhlObj);
                }
            }
        }
    }

    //Step 3: Check if there are any suspending Command in Abort Waiting.
    else if (pReq->ucReqStatus == CBUS_REQ_PENDING)
    {
        pReq = sCbusGetOutQReq(mhlObj);
        bResult = sCbusWriteCommand(mhlObj, pReq);
        pReq->ulBase_time = SiI_get_global_time();
        pReq->ucReqStatus = CBUS_REQ_WAITING;
        pReq->ulTimeout = DEM_MSC_MAX_REPLY_DELAY;
    }

    return (bResult);
}

/*****************************************************************************/

/**
 * @Function:     sCbusProcessFailureInterrupts
 *
 * @Description:
 *
 *        Handle failure interrupts.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusProcessFailureInterrupts (MhlTxObj_t *mhlObj, uint8_t ucStatus)
{
	SiiDrvCraAddr_t	baseAddr = sBaseAddrGet(mhlObj);
	SiiInst_t		craInst  = sCraInstGet(mhlObj);

    bool_t bResult = true;
    uint8_t ucMscAbortReason = true;
    uint8_t ucCecAbortReason = true;

    /* At this point, we only need to look at the abort interrupts. */

    ucStatus &= BIT_DDC_ABORT_INT | BIT_MSC_RCV_CMD_ERROR_INT | BIT_MSC_XFER_CMD_ERROR_INT;
    if (ucStatus)
    {
        bResult = false;      // No Retry will help
		 SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: sCbusProcessFailureInterrupts\n"));
        //Step 1: Display the Abort Source.

        /* If transfer abort or MSC abort, clear the abort reason register. */
        if (ucStatus & BIT_MSC_XFER_CMD_ERROR_INT)
        {
            ucMscAbortReason |= SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__MSC_MT_ABORT_INT);

            SiiDrvCraWrReg8(craInst, baseAddr |  REG_ADDR__MSC_MT_ABORT_INT, ucMscAbortReason );

            SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Transfer MSC Abort. \n"));
            sCbusOQReqAbort(mhlObj);
        }

        if (ucStatus & BIT_MSC_RCV_CMD_ERROR_INT)
        {
            ucMscAbortReason |= SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__MSC_MR_ABORT_INT);
			SiiDrvCraWrReg8(craInst, baseAddr |  REG_ADDR__MSC_MR_ABORT_INT, ucMscAbortReason);
            
			SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: MSC Responder Abort reason: %02X\n", (int)ucMscAbortReason));
			sCbusOQReqAbort(mhlObj);
        }

        if (ucStatus & BIT_DDC_ABORT_INT)
        {
            ucMscAbortReason |= SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__DDC_ABORT_INT);
			SiiDrvCraWrReg8(craInst, baseAddr |  REG_ADDR__DDC_ABORT_INT, ucMscAbortReason);

			SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: DDC Abort reason: %02X\n", (int)ucMscAbortReason));
            
        }
		       
        //Step 2: Display the Abort Reason.
        if (ucMscAbortReason != 0)
        {
            if (ucMscAbortReason & BIT_CBUSABORT_MAXFAIL)
            {
                SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Reason for ABORT--Requestor MAXFAIL - retry threshold exceeded\n"));
            }

            if (ucMscAbortReason & BIT_CBUSABORT_PROTOCOL_ERROR)
            {
                SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Reason for ABORT--Protocol Error\n"));
            }

			if (ucMscAbortReason & BIT_CBUSABORT_TIMEDOUT)
            {
                SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Reason for ABORT--Requestor translation layer timeout\n"));
            }

            if (ucMscAbortReason & BIT_CBUSABORT_UNDEFINED_OPCODE)
            {
                SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Reason for ABORT--Undefined opcode\n"));
            }

            if (ucMscAbortReason & BIT_CBUSABORT_BAD_OFFSET)
            {
                SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Reason for ABORT--Bad offset\n"));
            }

            if (ucMscAbortReason & BIT_CBUSABORT_OVERFLOW)
            {
                SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Reason for ABORT--MSC MSG overflow\n"));
            }

            if (ucMscAbortReason & BIT_CBUSABORT_PEER_ABORTED)
            {
                SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Reason for ABORT--Peer sent an abort\n"));
            }
        }
    }
    return (bResult);
}

/*****************************************************************************/

/**
 * @Function:     sCbusProcessMscMsgSubCommand
 *
 * @Description:
 *
 *        Handle received MSC MSG Sub command. Only available in Dongle Mode.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusProcessMscMsgSubCommand (MhlTxObj_t *mhlObj)
{
	SiiDrvCraAddr_t	baseAddr = sBaseAddrGet(mhlObj);
	SiiInst_t		craInst  = sCraInstGet(mhlObj);

    bool_t bResult = true;
    uint8_t msc_msg[2];

    /* Get MSC message data from the hardware. */

    msc_msg[0] = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__MSC_MR_MSC_MSG_RCVD_1ST_DATA);
	msc_msg[1] = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__MSC_MR_MSC_MSG_RCVD_2ND_DATA);

    if (mhlObj->cbus_state.InCommand.ucReqStatus == CBUS_REQ_IDLE)
    {
        mhlObj->cbus_state.InCommand.ucReqStatus = CBUS_REQ_PENDING;
        mhlObj->cbus_state.InCommand.ucCommand = msc_msg[0];
        mhlObj->cbus_state.InCommand.ucMsgData = msc_msg[1];
    }
    else
    {
        return false;
    }

    bResult = sCbusMscMsgHandler(mhlObj);

    return (bResult);
}

/*****************************************************************************/

/**
 * @Function:     sCbusWriteCommand
 *
 * @Description :
 *
 *        Write the specified Sideband Channel command to the CBUS.
 *        Command can be a MSC_MSG command (RCP/MCW/RAP), or another command
 *        such as READ_DEVCAP, GET_VENDOR_ID, SET_HPD, CLR_HPD, etc.
 *
 * @Parameters : pReq
 *
 *        MSC Command to write on CBus.
 *
 * @Return : bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusWriteCommand(MhlTxObj_t *mhlObj, cbus_out_queue_t *pReq)
{
	SiiDrvCraAddr_t	baseAddr = sBaseAddrGet(mhlObj);
	SiiInst_t		craInst  = sCraInstGet(mhlObj);

	uint8_t i, startbit;
    uint8_t ucData1 = 0;
    uint8_t ucData2 = 0;
    uint8_t bResult = true;
    uint16_t uwDebugFormat = 0;
    static uint32_t s_ulLastTime = 0;

    //Step 1: Check if it is in Aboar Waiting status.
    if (true == sCbusGetAbortWait(mhlObj))
    {
        SII_LIB_LOG_DEBUG1(mhlObj, ("sCbusWriteCommand:: CBUS is unavailable in Abort Waiting. \n"));
        return false;
    }

    //Step 2: Make sure that the interval between two commands is longer than 100 msec.
    //In MHL2.0, Timeout for a device waiting for a packet within a command is 100 msec.
    //So the interval between two commands must be longer than 100 msec.
    while (false == SiI_TimerDelay(s_ulLastTime, DEV_PKT_SENDER_TIMEOUT ))
    {
        ;
    }

    /****************************************************************************************/
    /* Setup for the command - write appropriate registers and determine the correct        */
    /*                         start bit.                                                   */
    /****************************************************************************************/

    SiiDrvCraWrReg8(craInst, baseAddr |  REG_ADDR__MSC_1ST_TRANSMIT_DATA, pReq->ucCommand);

    startbit = 0x00;
    switch (pReq->ucCommand)
    {
	case MHL_WRITE_STAT: //MHL_SET_INT    // Same opCode as MHL_SET_INT
	case MHL_SET_INT:
        SiiDrvCraWrReg8(craInst, baseAddr |  REG_ADDR__MSC_CMD_OR_OFFSET, pReq->ucOffsetData );
        SiiDrvCraWrReg8(craInst, baseAddr |  REG_ADDR__MSC_1ST_TRANSMIT_DATA, pReq->ucMsgData[0] );
        startbit = BIT_MSK__MSC_COMMAND_START__REG_MSC_WRITE_STAT_CMD;
        ucData1 = pReq->ucOffsetData;
        ucData2 = pReq->ucMsgData[0];
		SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Sending MSC command %s(0x%02x)  \n", \
												(pReq->ucCommand == 0x60) ? "MHL_SET_INT" : "MSC_WRITE_STAT", (int)pReq->ucCommand));
		switch (pReq->ucOffsetData)
        {
		case MHL_REG_CONNECTED_RDY_OFFSET:

            SII_LIB_LOG_DEBUG1(mhlObj, ("CONNECTED_RDY(0x%02x)  ", (int)pReq->ucOffsetData));
            if (pReq->ucMsgData[0] & BIT_ENUM__MHL_STAT_0__DCAP_READY)
            {
                //SII_LIB_LOG_DEBUG1(mhlObj, ("DCAP_RDY(0x%02x)  \n", (int)pReq->ucMsgData[0]));
				SII_LIB_LOG_DEBUG2(("DCAP_RDY "));
            }
			if(pReq->ucMsgData[0] & BIT_ENUM__MHL_STAT_0__XDEVCAP_SUPP)
			{
				//SII_LIB_LOG_DEBUG1(mhlObj, ("XDEVCAP_SUPP(0x%02x)  \n", (int)pReq->ucMsgData[0]));
				SII_LIB_LOG_DEBUG2(("XDEVCAP_SUPP "));
			}
			SII_LIB_LOG_DEBUG2(("(0x%02x)\n", (int)pReq->ucMsgData[0]));
            break;
        case MHL_REG_LINK_MODE_OFFSET:

            SII_LIB_LOG_DEBUG1(mhlObj, ("LINK_MODE(0x%02x)  ", (int)pReq->ucOffsetData));

            if (pReq->ucMsgData[0] & BIT_ENUM__MHL_STAT_1__MUTED)
            {
                //SII_LIB_LOG_DEBUG1(mhlObj, ("MUTED "));
				SII_LIB_LOG_DEBUG2(("MUTED "));
            }

            if (pReq->ucMsgData[0] & BIT_ENUM__MHL_STAT_1__PATH_EN)
            {
                SII_LIB_LOG_DEBUG2(("PATH_EN "));
            }

            if ((pReq->ucMsgData[0] & BIT_ENUM__MHL_STAT_1__CLK_MODE_MASK) == CLK_MODE_NORMAL)
            {
                SII_LIB_LOG_DEBUG2(("Normal CLK_MODE "));
            }
            else if ((pReq->ucMsgData[0] & BIT_ENUM__MHL_STAT_1__CLK_MODE_MASK) == CLK_MODE_PACKED_PIXEL)
            {
                SII_LIB_LOG_DEBUG2(("PackedPixel CLK_MODE "));
            }

            SII_LIB_LOG_DEBUG2(("(0x%02x)\n", (int)pReq->ucMsgData[0]));

            break;
		case MHL_REG_VERSION_STAT_OFFSET:
			SII_LIB_LOG_DEBUG1(mhlObj, ("VERSION_STAT(0x%02x)  ", (int)pReq->ucOffsetData));
			break;
        case MHL_REG_RCHANGE_INT_OFFSET:

            SII_LIB_LOG_DEBUG1(mhlObj, ("RCHANGE_INT(0x%02x)  ", (int)pReq->ucOffsetData));
            if (pReq->ucMsgData[0] & BIT_DCAP_CHG_INT)
            {
                SII_LIB_LOG_DEBUG2(("DCAP_CHG "));
            }

            if (pReq->ucMsgData[0] & BIT_DSCR_CHG_INT)
            {
                SII_LIB_LOG_DEBUG2(("DSCR_CHG "));
            }

            if (pReq->ucMsgData[0] & BIT_REQ_WRT_INT)
            {
                SII_LIB_LOG_DEBUG2(("REQ_WRT "));
            }

            if (pReq->ucMsgData[0] & BIT_GRT_WRT_INT)
            {
                SII_LIB_LOG_DEBUG2(("GRT_WRT "));
            }

            if (pReq->ucMsgData[0] & BIT_3D_REQ_INT)
            {
                SII_LIB_LOG_DEBUG2(("3D_REQ "));
            }

            SII_LIB_LOG_DEBUG2(("(0x%02x)\n", (int)pReq->ucMsgData[0]));

            break;

        case MHL_REG_DCHANGE_INT_OFFSET:

            SII_LIB_LOG_DEBUG1(mhlObj, ("DCHANGE_INT(0x%02x)  ", (int)pReq->ucOffsetData));
            if (pReq->ucMsgData[0] & BIT_EDID_CHG_INT)
            {
                SII_LIB_LOG_DEBUG2(("EDID_CHG "));
            }

            SII_LIB_LOG_DEBUG2(("(0x%02x)\n", (int)pReq->ucMsgData[0]));

            break;

        default:

            SII_LIB_LOG_DEBUG1(mhlObj, ("ERROR:Invalid parameter. Please have a check. Command = 0x%02x, Offset = 0x%02x, MsgData[0] = 0x%02x\n",
                              (int)pReq->ucCommand,
                              (int)pReq->ucOffsetData,
                              (int)pReq->ucMsgData[0]));
            break;
        }
        break;

    case MHL_READ_DEVCAP:
        SiiDrvCraWrReg8(craInst, baseAddr |  REG_ADDR__MSC_CMD_OR_OFFSET, pReq->ucOffsetData );
        startbit = BIT_MSK__MSC_COMMAND_START__REG_MSC_READ_DEVCAP_CMD;
        ucData1 = pReq->ucOffsetData;
        SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Sending MSC command MSC_READ_DEVCAP(0x%02x)  \n", (int)pReq->ucCommand));

        switch (pReq->ucOffsetData)
        {
        case 0x00:

            SII_LIB_LOG_DEBUG1(mhlObj, ("DEV_STATE(0x%02x)", (int)pReq->ucOffsetData));
            break;
        case 0x01:

            SII_LIB_LOG_DEBUG1(mhlObj, ("MHL_VERSION(0x%02x)", (int)pReq->ucOffsetData));

            break;
        case 0x02:

            SII_LIB_LOG_DEBUG1(mhlObj, ("DEV_CAT(0x%02x)", (int)pReq->ucOffsetData));

            break;
        case 0x03:

            SII_LIB_LOG_DEBUG1(mhlObj, ("ADOPTER_ID_H(0x%02x)", (int)pReq->ucOffsetData));

            break;
        case 0x04:

            SII_LIB_LOG_DEBUG1(mhlObj, ("ADOPTER_ID_L(0x%02x)", (int)pReq->ucOffsetData));

            break;
        case 0x05:

            SII_LIB_LOG_DEBUG1(mhlObj, ("VID_LINK_MODE(0x%02x)", (int)pReq->ucOffsetData));
            break;
        case 0x06:

            SII_LIB_LOG_DEBUG1(mhlObj, ("AUD_LINK_MODE(0x%02x)", (int)pReq->ucOffsetData));
            break;
        case 0x07:

            SII_LIB_LOG_DEBUG1(mhlObj, ("VIDEO_TYPE(0x%02x)", (int)pReq->ucOffsetData));

            break;
        case 0x08:

            SII_LIB_LOG_DEBUG1(mhlObj, ("LOG_DEV_MAP(0x%02x)", (int)pReq->ucOffsetData));

            break;
        case 0x09:

            SII_LIB_LOG_DEBUG1(mhlObj, ("BANDWIDTH(0x%02x)", (int)pReq->ucOffsetData));

            break;
        case 0x0A:

            SII_LIB_LOG_DEBUG1(mhlObj, ("FEATURE(0x%02x)", (int)pReq->ucOffsetData));

            break;
        case 0x0B:

            SII_LIB_LOG_DEBUG1(mhlObj, ("DEVICE_ID_H(0x%02x)", (int)pReq->ucOffsetData));

            break;
        case 0x0C:

            SII_LIB_LOG_DEBUG1(mhlObj, ("DEVICE_ID_L(0x%02x)", (int)pReq->ucOffsetData));

            break;
        case 0x0D:

            SII_LIB_LOG_DEBUG1(mhlObj, ("SCRATCHPAD_SIZE(0x%02x)", (int)pReq->ucOffsetData));

            break;
        case 0x0E:

            SII_LIB_LOG_DEBUG1(mhlObj, ("INT_STAT_SIZE(0x%02x)", (int)pReq->ucOffsetData));

            break;
        default:

            SII_LIB_LOG_DEBUG1(mhlObj, ("Invalid parameter. Please have a check. "));
            break;
        }

        SII_LIB_LOG_DEBUG2(("\n"));
	    break;

    case MHL_READ_XDEVCAP:
        SiiDrvCraWrReg8(craInst, baseAddr |  REG_ADDR__MSC_CMD_OR_OFFSET, pReq->ucOffsetData );
        startbit = BIT_MSK__MSC_COMMAND_START__REG_MSC_READ_DEVCAP_CMD;
        ucData1 = pReq->ucOffsetData;
        SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Sending MSC command MSC_READ_XDEVCAP(0x%02x)  \n", (int)pReq->ucCommand));

		switch (pReq->ucOffsetData)
        {
        case MHL_DEV_XCAP_ECBUS_SPEEDS_REG_OFFSET:
            SII_LIB_LOG_DEBUG1(mhlObj, ("ECBUS_SPEEDS(0x%02x)", (int)pReq->ucOffsetData));
            break;
        case MHL_DEV_XCAP_TMDS_SPEEDS_REG_OFFSET:
            SII_LIB_LOG_DEBUG1(mhlObj, ("TMDS_SPEEDS(0x%02x)", (int)pReq->ucOffsetData));
            break;
        case MHL_DEV_XCAP_ECBUS_DEV_ROLES_REG_OFFSET:
            SII_LIB_LOG_DEBUG1(mhlObj, ("ECBUS_DEV_ROLES(0x%02x)", (int)pReq->ucOffsetData));
            break;
		case MHL_DEV_XCAP_LOG_DEV_MAP_REG_OFFSET:
			SII_LIB_LOG_DEBUG1(mhlObj, ("LOG_DEV_MAP(0x%02x)", (int)pReq->ucOffsetData));
        default:
            SII_LIB_LOG_DEBUG1(mhlObj, ("Invalid parameter. Please have a check. "));
            break;
        }

        SII_LIB_LOG_DEBUG2(("\n"));
        break;

    case MHL_GET_STATE:
        startbit = BIT_MSK__MSC_COMMAND_START__REG_MSC_PEER_CMD;
        SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Sending MSC command MSC_GET_STATE(0x%02x)  \n", (int)pReq->ucCommand));
        break;

    case MHL_GET_VENDOR_ID:
        startbit = BIT_MSK__MSC_COMMAND_START__REG_MSC_PEER_CMD;
        SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Sending MSC command MSC_GET_VENDOR_ID(0x%02x)  \n", (int)pReq->ucCommand));
        break;

    case MHL_SET_HPD:
        startbit = BIT_MSK__MSC_COMMAND_START__REG_MSC_PEER_CMD;
        SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Sending MSC command MSC_SET_HPD(0x%02x)  \n", (int)pReq->ucCommand));
        break;

    case MHL_CLR_HPD:
        startbit = BIT_MSK__MSC_COMMAND_START__REG_MSC_PEER_CMD;
        SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Sending MSC command MSC_CLR_HPD(0x%02x)  \n", (int)pReq->ucCommand));
        break;

    case MHL_GET_VS_ERRORCODE:
        startbit = BIT_MSK__MSC_COMMAND_START__REG_MSC_PEER_CMD;
        SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Sending MSC command MSC_GET_VS_ABORT(0x%02x)  \n", (int)pReq->ucCommand));
        break;

    case MHL_GET_DDC_ERRORCODE:
        startbit = BIT_MSK__MSC_COMMAND_START__REG_MSC_PEER_CMD;
        SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Sending MSC command MSC_GET_DDC_ABORT(0x%02x)  \n", (int)pReq->ucCommand));
        break;

    case MHL_GET_MSC_ERRORCODE:
        startbit = BIT_MSK__MSC_COMMAND_START__REG_MSC_PEER_CMD;
        SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Sending MSC command MSC_GET_MSC_ABORT(0x%02x)  \n", (int)pReq->ucCommand));
        break;

    case MHL_MSC_MSG:
        SiiDrvCraWrReg8(craInst, baseAddr |  REG_ADDR__MSC_1ST_TRANSMIT_DATA, pReq->ucMsgData[0] );
        SiiDrvCraWrReg8(craInst, baseAddr |  REG_ADDR__MSC_2ND_TRANSMIT_DATA, pReq->ucMsgData[1] );
        startbit = BIT_MSK__MSC_COMMAND_START__REG_MSC_MSC_MSG_CMD;

        ucData1 = pReq->ucMsgData[0];
        ucData2 = pReq->ucMsgData[1];

        switch (ucData1)
        {
        case MHL_MSC_MSGE:

            SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Sending MSC command MSC_MSG(0x%02X), MSGE(0x%02X), 0x%02X\n",
                                  (int)pReq->ucCommand,
                                  (int)ucData1,
                                  (int)ucData2));
            break;
        case MHL_MSC_MSG_RCP:

            SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Sending MSC command MSC_MSG(0x%02X), RCP(0x%02X), 0x%02X\n",
                                  (int)pReq->ucCommand,
                                  (int)ucData1,
                                  (int)ucData2));
            break;
        case MHL_MSC_MSG_RCPK:

            SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Sending MSC command MSC_MSG(0x%02X), RCPK(0x%02X), 0x%02X\n",
                                  (int)pReq->ucCommand,
                                  (int)ucData1,
                                  (int)ucData2));
            break;
        case MHL_MSC_MSG_RCPE:

            SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Sending MSC command MSC_MSG(0x%02X), RCPE(0x%02X), 0x%02X\n",
                                  (int)pReq->ucCommand,
                                  (int)ucData1,
                                  (int)ucData2));
            break;
        case MHL_MSC_MSG_RAP:

            SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Sending MSC command MSC_MSG(0x%02X), RAP(0x%02X), 0x%02X\n",
                                  (int)pReq->ucCommand,
                                  (int)ucData1,
                                  (int)ucData2));
            break;
        case MHL_MSC_MSG_RAPK:

            SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Sending MSC command MSC_MSG(0x%02X), RAPK(0x%02X), 0x%02X\n",
                                  (int)pReq->ucCommand,
                                  (int)ucData1,
                                  (int)ucData2));
            break;
        case MHL_MSC_MSG_UCP:

            SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Sending MSC command MSC_MSG(0x%02X), UCP(0x%02X), 0x%02X\n",
                                  (int)pReq->ucCommand,
                                  (int)ucData1,
                                  (int)ucData2));
            break;
        case MHL_MSC_MSG_UCPK:

            SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Sending MSC command MSC_MSG(0x%02X), UCPK(0x%02X), 0x%02X\n",
                                  (int)pReq->ucCommand,
                                  (int)ucData1,
                                  (int)ucData2));
            break;
        case MHL_MSC_MSG_UCPE:

            SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Sending MSC command MSC_MSG(0x%02X), UCPE(0x%02X), 0x%02X\n",
                                  (int)pReq->ucCommand,
                                  (int)ucData1,
                                  (int)ucData2));
            break;
        default:

            SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Sending MSC command MSC_MSG(0x%02X), UNKNOWN(0x%02X), 0x%02X\n",
                              (int)pReq->ucCommand,
                              (int)ucData1,
                              (int)ucData2));
            break;
        }
		break;

    case MHL_WRITE_BURST:
        SiiDrvCraWrReg8(craInst, baseAddr |  REG_ADDR__MSC_CMD_OR_OFFSET, pReq->ucOffsetData );
        SiiDrvCraWrReg8(craInst, baseAddr |  REG_ADDR__MSC_WRITE_BURST_DATA_LEN, MHL_SCRATCHPAD_SIZE - 1 );    //be care of length , if length = 16 , write 15 into register

        // Now copy all bytes from array to local scratchpad

        for (i = 0; i < MHL_SCRATCHPAD_SIZE; i++)
        {
            SiiDrvCraWrReg8(craInst, baseAddr | (REG_ADDR__WB_XMIT_DATA_0 + i), pReq->ucMsgData[i] );
        }

        startbit = BIT_MSK__MSC_COMMAND_START__REG_MSC_WRITE_BURST_CMD;
        ucData1 = pReq->ucOffsetData;

        ucData2 = MHL_SCRATCHPAD_SIZE;

        SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Sending MSC command MHL_WRITE_BURST(0x%02x)  \n", (int)pReq->ucCommand));

        break;

    default:
        bResult = ERROR_INVALID_MSC_CMD;

        SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Sending MSC command ERROR_INVALID_MSC_CMD(0x%02x)  \n", (int)pReq->ucCommand));

        break;
    }

    /****************************************************************************************/
    /* Trigger the CBUS command transfer using the determined start bit.                    */
    /****************************************************************************************/
    if (bResult == true)
    {
        SiiDrvCraWrReg8(craInst, baseAddr |  REG_ADDR__MSC_COMMAND_START, startbit );
    }

    s_ulLastTime = SiI_get_global_time();

    return (bResult);
}

/*****************************************************************************/

/**
 * @Function:     sCbusRetryOutCommand
 *
 * @Description :
 *
 *        Retry sending out command. This function will be invoked when
 *        (1)ransfer done with NACK
 *        (2)received RCPE with responder busy
 *        (3)received RAPK with error
 *
 * @Return : bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static uint8_t sCbusRetryOutCommand(MhlTxObj_t *mhlObj)
{
    uint8_t bResult = true;
    cbus_out_queue_t *pReq = &mhlObj->cbus_state.OutCommand;

    if ((pReq->ucReqStatus == CBUS_REQ_WAITING) && pReq->ucRetry)
    {
        bResult = sCbusWriteCommand(mhlObj, pReq);
        pReq->ucRetry -= 1;
        pReq->ulBase_time = SiI_get_global_time();
        pReq->ucReqStatus = CBUS_REQ_WAITING;
        pReq->ulTimeout = DEM_MSC_MAX_REPLY_DELAY;
    }
    else
    {
        sCbusOutQCleanActiveReq(mhlObj);
    }

    return bResult;
}

/*****************************************************************************/

/**
 * @Function:     sCbus3DBuildBurstData
 *
 * @Description :
 *
 *        Build 3D WRITE_BURST data.
 *
 * @Return : bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/

static bool_t sCbus3DBuildBurstData(MhlTxObj_t *mhlObj)
{
    SII_LIB_LOG_DEBUG1(mhlObj, ("TX should not receive 3D_REQ INT!"));
    return true;
}

/*****************************************************************************/

/**
 * @Function:     sCbusMscGrtWrtRecv
 *
 * @Description :
 *
 *        This function will be invoked when received GRT_WRT. It will send WRITE_BURST to peer device.
 *
 * @Return : bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMscGrtWrtRecv(MhlTxObj_t *mhlObj)
{
    uint8_t ucCmd, ucMsgData0, ucReqStatus;
    cbus_out_queue_t *pReq = sCbusGetOutQReq(mhlObj);

    ucCmd = sCbusOutQReqCmd(mhlObj);
    ucMsgData0  = sCbusOutQReqData0(mhlObj);
    ucReqStatus = sCbusOutQGetReqStatus(mhlObj);

    SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: GRT_WRT received\n"));

    if ((ucCmd == MHL_SET_INT) && (ucMsgData0 == BIT_REQ_WRT_INT))  // check whether the receieved grt_wrt is for req_wrt
    {
        sCbusWRITE_BURST(mhlObj, pReq->ucMsgData2, (uint8_t)MHL_SCRATCHPAD_SIZE, pReq->pCommandReport);
    }

    return true;
}

/*****************************************************************************/

/**
 *  Cbus RAP operation:  RAP error process
 *
 *
 *  @param[in]          rapStatusCode	including Unrecognized Action Code, Unsupported Action Code, responder busy
 *
 *  @return             Result
 *
 *****************************************************************************/
static bool_t sCbusMscMsgRapErrProcess(MhlTxObj_t *mhlObj, uint8_t ucErrorCode)
{
    //SI_CpiData_t cecFrame;
    uint8_t ucRetry;
    uint8_t ucCmd   = sCbusOutQReqCmd(mhlObj);
    uint8_t ucvsCmd = sCbusOutQReqData0(mhlObj);

    switch (ucErrorCode)
    {
    case MHL_RAPK_UNRECOGNIZED:
    case MHL_RAPK_UNSUPPORTED:
        sCbusOutQCleanActiveReq(mhlObj);
        break;

    case MHL_RAPK_BUSY:
        if ((ucCmd == MHL_MSC_MSG) && (ucvsCmd = MHL_MSC_MSG_RAP))
        {
            ucRetry = sCbusOQReqRetry(mhlObj);
            if (ucRetry > 0)
            {
                sCbusRetryOutCommand(mhlObj);
            }
            else if (ucRetry == 0)
            {
                sCbusOutQCleanActiveReq(mhlObj);
            }
        }

        break;
    }

    return true;
}

/*****************************************************************************/

/**
 * @Function:     sCbusMscMsgRapAckRcvd
 *
 * @Description :
 *
 *        This function will be invoked when received RAPK.
 *
 * @Parameters : ucErrorCode
 *
 *        MHL_RAPK_NO_ERR          0x00
 *        MHL_RAPK_UNRECOGNIZED 0x01
 *        MHL_RAPK_UNSUPPORTED   0x02
 *        MHL_RAPK_BUSY                 0x03
 *
 * @Return : bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMscMsgRapAckRcvd(MhlTxObj_t *mhlObj, uint8_t ucErrorCode)
{
    if (ucErrorCode)
    {
        sCbusMscMsgRapErrProcess (mhlObj, ucErrorCode);
    }
    else
    {
        //Step 1: If RAPK{CBUS_MODE_UP/DOWN}, enable/disable eCBUS.
        cbus_out_queue_t *pReq = &mhlObj->cbus_state.OutCommand;

        if (MHL_RAP_CMD_CBUS_MODE_UP == pReq->ucMsgData[1])
        {
            SII_LIB_LOG_DEBUG1(mhlObj, ("RAP:: RAPK{CBUS_MODE_UP} received.\n"));        
            sCbusSwitchToEcbus(mhlObj);
        }
        else if (MHL_RAP_CMD_CBUS_MODE_DOWN == pReq->ucMsgData[1])
        {
            SII_LIB_LOG_DEBUG1(mhlObj, ("RAP:: RAPK{CBUS_MODE_DOWN} received.\n"));        
            sCbusSwitchToBcbus(mhlObj);
        }

        //Step 2: Clear Output Queue.
        sCbusOutQCleanActiveReq(mhlObj);
    }

    return true;
}

/*****************************************************************************/

/**
 * @Function:     sCbusMscMsgRcpHandler
 *
 * @Description :
 *
 *        This function will be invoked when received RCP/RCPK/RCPE.
 *
 * @Return : bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMscMsgRcpHandler(MhlTxObj_t *mhlObj)
{
    bool_t bResult = true;
    uint8_t ucCommand, ucData;
    static bool_t bRetryCommand = false;

    ucCommand = sCbusInQReqCmd(mhlObj);
    ucData = sCbusInQReqData(mhlObj);

    SII_LIB_LOG_DEBUG1(mhlObj, ("RCP:: RCP Received <- KeyCode = 0x%02x\n", ucData));

    if (ucCommand == MHL_MSC_MSG_RCP)
    {
		// If we get a RCP key that we do NOT support, send back RCPE and donot notify.
		if(MHL_LOGICAL_DEVICE_MAP & rcpSupportTable[ucData & 0x7F])
		{
			mhlObj->rcpKeyCode = ucData;
			//Notify to Tx Controller which in turn will notify to App Layer
			sNotifyMhlEvent(mhlObj, SII_DRV_TX_EVENT_MHL_RCP_RECEIVED);
			//Send RCPK.
			//SiiCbusSendCommand(mhlObj, API_MSC_MSG_RCPK, &ucData, 0x1, NULL);
			bResult = sCbusMSC_MSG_RCPK(mhlObj, ucData, NULL);
			//sCbusInQCleanActiveReq(mhlObj);
		}
		else
		{
			uint8_t key = MHL_RCPE_STATUS_INEFFECTIVE_KEY_CODE;
			// Save keycode to send a RCPK after RCPE.
			mhlObj->mscSaveRcpKeyCode = ucData; //KeyCode
			//SiiCbusSendCommand(mhlObj, API_MSC_MSG_RCPE, &key, 0x1, NULL);
			bResult = sCbusMSC_MSG_RCPE(mhlObj, key, NULL);
			//sCbusInQCleanActiveReq(mhlObj);
		}
		
    }
    else if (ucCommand == MHL_MSC_MSG_RCPK)
    {
        uint8_t ucCmd, ucMsgData0, ucReqStatus;
        uint8_t ucMsgData1;

		ucCmd = sCbusOutQReqCmd(mhlObj);
        ucMsgData0  = sCbusOutQReqData0(mhlObj);
        ucMsgData1  = sCbusOutQReqData1(mhlObj);
        ucReqStatus = sCbusOutQGetReqStatus(mhlObj);

        SII_LIB_LOG_DEBUG1(mhlObj, ("RCP:: RCPK Received <- KeyCode = 0x%02x\n", ucData));

        // check whether the received RCPK is for the RCP we sent out
        if ((ucCmd == MHL_MSC_MSG) && (ucMsgData0 == MHL_MSC_MSG_RCP) && (ucMsgData1 == ucData))
        {
            if ((ucReqStatus == CBUS_REQ_WAITING) && (!bRetryCommand))
            {
                // If the req status is waiting, it means it didn't receive RCPE before RCPK
                // If the req status is pending, there're 2 situations:
                // 1. it received RCPE (Responder busy), and is waiting to retry.
                // 2. it received RCPE (Ineffective KeyID), the current req is a new req.
				sCbusOutQCleanActiveReq(mhlObj);
            }

            if (bRetryCommand)
            {
                // retry the out command after receiving the RCPK
                bRetryCommand = false;
                bResult = sCbusRetryOutCommand(mhlObj);
            }
        }
		//Notify to Tx Controller which in turn will notify to App Layer
		sNotifyMhlEvent(mhlObj, SII_DRV_TX_EVENT_MHL_RCPK_RECEIVED);
    }
    else if (ucCommand == MHL_MSC_MSG_RCPE)
    {
        SII_LIB_LOG_DEBUG1(mhlObj, ("RCP:: RCPE Received <-Status Code: 0x%02X\n", (int)ucData));

        if (ucData == MHL_RCPE_STATUS_INEFFECTIVE_KEY_CODE)  // if don'd update the ignore list, how to deal with such case? do nothing??  TBD
        {}
        else if (ucData == MHL_RCPE_STATUS_BUSY)
        {
            uint8_t ucRetry, ucCmd, ucMsgData0;
            ucCmd = sCbusOutQReqCmd(mhlObj);
            ucMsgData0 = sCbusOutQReqData0(mhlObj);

            if ((ucCmd == MHL_MSC_MSG) && (ucMsgData0 == MHL_MSC_MSG_RCP)) // make sure the command in OQ is msg
            {
                ucRetry = sCbusOQReqRetry(mhlObj);
                if (ucRetry > 0)
                {
                    // set retry when got RCPE, retry it when after receiving RCPK
                    bRetryCommand = true;

                    //sCbusOQSetReqTimeout(mhlObj, 0);
                }
                else if (ucRetry == 0)
                {
                    sCbusOutQCleanActiveReq(mhlObj);
                }
            }
        }
		//Notify to Tx Controller which in turn will notify to App Layer
		sNotifyMhlEvent(mhlObj, SII_DRV_TX_EVENT_MHL_RCPE_RECEIVED);
    }

    return (bResult);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMscMsgRapHandler
 *
 * @Description :
 *
 *        This function will be invoked when received RAP/RAPK.
 *
 * @Return : bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMscMsgRapHandler(MhlTxObj_t *mhlObj)
{
    bool_t bResult = false;
    uint8_t ucCommand;
    uint8_t ucRapKeyCode;

    ucCommand = sCbusInQReqCmd(mhlObj);
    ucRapKeyCode = sCbusInQReqData(mhlObj);

    if (ucCommand == MHL_MSC_MSG_RAP)
    {
        SII_LIB_LOG_DEBUG1(mhlObj, ("RAP:: RAP Received <- (KeyCode = 0x%2x)\n", ucRapKeyCode));

        switch (ucRapKeyCode)
        {
        case MHL_RAP_CMD_POLL:
            bResult = sCbusMSC_MSG_RAPK(mhlObj, MHL_RAPK_NO_ERR, NULL);
            sCbusInQCleanActiveReq(mhlObj);
            SII_LIB_LOG_DEBUG1(mhlObj, ("RAP:: RAP Received <- RAP_CMD_POLL (KeyCode = 0x%2x)\n", ucRapKeyCode));
            break;
        case MHL_RAP_CMD_CONTENT_ON:
            bResult = sCbusMSC_MSG_RAPK(mhlObj, MHL_RAPK_NO_ERR, NULL);
            sCbusInQCleanActiveReq(mhlObj);
            SII_LIB_LOG_DEBUG1(mhlObj, ("RAP:: RAP Received <- RAP_CMD_CONTENT_ON (KeyCode = 0x%2x)\n", ucRapKeyCode));
			//sTmdsOutputControl(mhlObj, true);
            break;
        case MHL_RAP_CMD_CONTENT_OFF:
            bResult = sCbusMSC_MSG_RAPK(mhlObj, MHL_RAPK_NO_ERR, NULL);
            sCbusInQCleanActiveReq(mhlObj);
            SII_LIB_LOG_DEBUG1(mhlObj, ("RAP:: RAP Received <- RAP_CMD_CONTENT_OFF (KeyCode = 0x%2x)\n", ucRapKeyCode));
			sTmdsOutputControl(mhlObj, false);
            break;
		//Non-transmitter device should NOT receive such two RAP sub-commands.
        case MHL_RAP_CMD_CBUS_MODE_UP:
            bResult = sCbusMSC_MSG_RAPK(mhlObj, MHL_RAPK_NO_ERR, NULL);
            sCbusInQCleanActiveReq(mhlObj);
            SII_LIB_LOG_DEBUG1(mhlObj, ("RAP:: RAP Received <- RAP_CMD_CBUS_MODE_UP (KeyCode = 0x%2x)\n", ucRapKeyCode));
            sCbusSwitchToEcbus(mhlObj);
            break;
        case MHL_RAP_CMD_CBUS_MODE_DOWN:
            bResult = sCbusMSC_MSG_RAPK(mhlObj, MHL_RAPK_NO_ERR, NULL);
            sCbusInQCleanActiveReq(mhlObj);
            SII_LIB_LOG_DEBUG1(mhlObj, ("RAP:: RAP Received <- RAP_CMD_CBUS_MODE_DOWN (KeyCode = 0x%2x)\n", ucRapKeyCode));
            sCbusSwitchToBcbus(mhlObj);
            break;

        default:
            bResult = sCbusMSC_MSG_RAPK(mhlObj, MHL_RAPK_UNRECOGNIZED, NULL);
            SII_LIB_LOG_DEBUG1(mhlObj, ("RAP:: RAP Received <- UNKNOWN (KeyCode = 0x%2x)\n", ucRapKeyCode));
            sCbusInQCleanActiveReq(mhlObj);
            break;
        }
    }
    else
    {
        sCbusInQCleanActiveReq(mhlObj);
        bResult = sCbusMscMsgRapAckRcvd(mhlObj, ucRapKeyCode);
    }

    return bResult;
}

/*****************************************************************************/

/**
 * @Function:     sCbusMscMsgUcpHandler
 *
 * @Description :
 *
 *        This function will be invoked when received RCP/RCPK.
 *
 * @Return : bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMscMsgUcpHandler(MhlTxObj_t *mhlObj)
{
    bool_t bResult = false;
    uint8_t ucCommand;
    uint8_t ucUcpKeyCode;

    ucCommand = sCbusInQReqCmd(mhlObj);
    ucUcpKeyCode = sCbusInQReqData(mhlObj);

    return bResult;
}

/*****************************************************************************/

/**
 * @Function:     sCbusMscMsgRapTimeoutHandler
 *
 * @Description :
 *
 *        If time is out when sending or waiting for RAP.
 *
 * @Return : bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMscMsgRapTimeoutHandler(MhlTxObj_t *mhlObj, uint8_t ucDirection)
{
    bool_t bResult = true;

    if (ucDirection == CBUS_IN)
    {
        sCbusInQCleanActiveReq(mhlObj);
    }
    else if (ucDirection == CBUS_OUT)
    {
        uint8_t ucRetry;

        ucRetry = sCbusOQReqRetry(mhlObj);
        if (ucRetry > 0)
        {
            sCbusOQReduceReqRetry(mhlObj);
            sCbusOutQSetReqStatus(mhlObj, CBUS_REQ_PENDING);
        }
        else if (ucRetry == 0)
        {
            sCbusOutQCleanActiveReq(mhlObj);
        }
    }

    return (bResult);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMscMsgRcpTimeoutHandler
 *
 * @Description :
 *
 *        If time is out when sending or waiting for RCP.
 *
 * @Return : bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMscMsgRcpTimeoutHandler(MhlTxObj_t *mhlObj, uint8_t ucDirection)
{
    bool_t bResult = true;

    if (ucDirection == CBUS_IN)
    {
        sCbusInQCleanActiveReq(mhlObj);
    }
    else if (ucDirection == CBUS_OUT)
    {
        uint8_t ucRetry;

        ucRetry = sCbusOQReqRetry(mhlObj);
        if (ucRetry > 0)
        {
            sCbusOQReduceReqRetry(mhlObj);
            sCbusOutQSetReqStatus(mhlObj, CBUS_REQ_PENDING);
        }
        else if (ucRetry == 0)
        {
            sCbusOutQCleanActiveReq(mhlObj);
        }
    }

    return (bResult);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMscMsgUcpTimeoutHandler
 *
 * @Description :
 *
 *        If time is out when sending or waiting for UCP.
 *
 * @Return : bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMscMsgUcpTimeoutHandler(MhlTxObj_t *mhlObj, uint8_t ucDirection)
{
    bool_t bResult = true;

    if (ucDirection == CBUS_IN)
    {
        sCbusInQCleanActiveReq(mhlObj);
    }
    else if (ucDirection == CBUS_OUT)
    {
        uint8_t ucRetry;

        ucRetry = sCbusOQReqRetry(mhlObj);
        if (ucRetry > 0)
        {
            sCbusOQReduceReqRetry(mhlObj);
            sCbusOutQSetReqStatus(mhlObj, CBUS_REQ_PENDING);
        }
        else if (ucRetry == 0)
        {
            sCbusOutQCleanActiveReq(mhlObj);
            SII_LIB_LOG_DEBUG1(mhlObj, ("UCP:: Timeout. Clear request from Output Queue.\n"));
        }
    }

    return (bResult);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMSC_SET_HOTPLUG_Done
 *
 * @Description :
 *
 *        Internal Callback function for sCbusMSC_SET_HOTPLUG
 *
 * @Return : bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMSC_SET_HOTPLUG_Done(void *pObj, uint8_t ucData)
{
	MhlTxObj_t *mhlObj = (MhlTxObj_t*)pObj;
    ucData = ucData;
    sCbusOutQCleanActiveReq(mhlObj);

    return (true);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMSC_SET_HOTPLUG
 *
 * @Description :
 *
 *        Set HPD High or Low
 *
 * @Return : bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMSC_SET_HOTPLUG(MhlTxObj_t *mhlObj, bool_t bSetHpd, PCOMMANDREPORT_t pCommandReport)
{
    uint8_t bResult = true;
    cbus_out_queue_t *pReq = sCbusGetOutQReq(mhlObj);

    if (true == sCbusGetAbortWait(mhlObj))
    {
        // SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: CBUS is unavailable in Abort Waiting.\n"));
        return false;
    }

    if (pReq->ucReqStatus == CBUS_REQ_IDLE)
    {
        pReq->CbusMscCommand = API_MSC_SET_HOTPLUG;

        if (bSetHpd)
        {
            pReq->ucCommand = MHL_SET_HPD;
        }
        else
        {
            pReq->ucCommand = MHL_CLR_HPD;
        }

        pReq->dataRetHandler = sCbusMSC_SET_HOTPLUG_Done;
        pReq->pCommandReport = pCommandReport;
        pReq->ucRetry = 1;

        bResult = sCbusWriteCommand(mhlObj, pReq);
        pReq->ulBase_time = SiI_get_global_time();
        pReq->ucReqStatus = CBUS_REQ_WAITING;
        pReq->ulTimeout = DEM_MSC_MAX_REPLY_DELAY;

        SII_LIB_LOG_DEBUG1(mhlObj, ("HOT_PLUG:: Sending %s HOT_PLUG\n", (pReq->ucCommand == MHL_SET_HPD) ? "SET" : "CLR"));
    }
    else
    {
        SII_LIB_LOG_DEBUG1(mhlObj, ("HOT_PLUG:: Fail to Send %s HOT_PLUG. CBus is busy. Try later.\n", (pReq->ucCommand
                                                                                                  == MHL_SET_HPD) ? "SET" : "CLR"));
        bResult = false;
    }

    return bResult;
}

/*****************************************************************************/

/**
 * @Function:     sCbusSET_INT_REQ_WRT_Done
 *
 * @Description:
 *
 *         Return handler for sCbusSET_INT_REQ_WRT
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusSET_INT_REQ_WRT_Done(void *pObj, uint8_t ucData)
{
	MhlTxObj_t *mhlObj = (MhlTxObj_t *)pObj; 
    ucData = ucData;

	sCbusOQSetReqTimeout(mhlObj, DEM_MHL_BURST_WAIT);  // set time out when sending out req wrt 1000ms

    return (true);
}

/*****************************************************************************/

/**
 * @Function:     sCbusSET_INT_REQ_WRT
 *
 * @Description:
 *
 *         Set the Interrupt Register of Peer Device.
 *         The Interrupt bit is REQ_WRT.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusSET_INT_REQ_WRT(MhlTxObj_t *mhlObj, uint8_t* pArgument, uint8_t ucLength, PCOMMANDREPORT_t pCommandReport)
{
    uint8_t bResult = true;
    cbus_out_queue_t *pReq = sCbusGetOutQReq(mhlObj);

    if (true == sCbusGetAbortWait(mhlObj))
    {
        // SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: CBUS is unavailable in Abort Waiting.\n"));
        return false;
    }

    if (pReq->ucReqStatus == CBUS_REQ_IDLE)
    {
        pReq->CbusMscCommand = API_MSC_INTERRUPT_REQ_WRT;
        pReq->ucCommand = MHL_SET_INT;
        pReq->ucOffsetData = MHL_REG_RCHANGE_INT_OFFSET;
        pReq->ucMsgData[0] = BIT_REQ_WRT_INT;
        memcpy(pReq->ucMsgData2, pArgument, ucLength);
        pReq->dataRetHandler = sCbusSET_INT_REQ_WRT_Done;
        pReq->pCommandReport = pCommandReport;
        pReq->ucRetry = 1;

        bResult = sCbusWriteCommand(mhlObj, pReq);
        pReq->ulBase_time = SiI_get_global_time();
        pReq->ucReqStatus = CBUS_REQ_WAITING;
        pReq->ulTimeout = DEM_MDT_WAIT;

        SII_LIB_LOG_DEBUG1(mhlObj, ("REQ_WRT:: Sending REQ_WRT\n"));
    }
    else
    {
        SII_LIB_LOG_DEBUG1(mhlObj, ("REQ_WRT:: Fail to Send REQ_WRT. CBus is busy. Try later.\n"));
        bResult = false;
    }

    return bResult;
}

/*****************************************************************************/

/**
 * @Function:     sCbusSET_INT_GRT_WRT_Done
 *
 * @Description:
 *
 *         Return handler for sCbusSET_INT_GRT_WRT
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusSET_INT_GRT_WRT_Done(void *pObj, uint8_t ucData)
{
	MhlTxObj_t *mhlObj = (MhlTxObj_t *)pObj;
    ucData = ucData;

    sCbusOutQCleanActiveReq(mhlObj);

    return (true);
}

/*****************************************************************************/

/**
 * @Function:     sCbusSET_INT_GRT_WRT
 *
 * @Description:
 *
 *         Set the Interrupt Register of Peer Device.
 *         The Interrupt bit is GRT_WRT.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusSET_INT_GRT_WRT(MhlTxObj_t *mhlObj, PCOMMANDREPORT_t pCommandReport)
{
    uint8_t bResult = true;
    cbus_out_queue_t *pReq = sCbusGetOutQReq(mhlObj);

    if (true == sCbusGetAbortWait(mhlObj))
    {
        // SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: CBUS is unavailable in Abort Waiting.\n"));
        return false;
    }

    if (pReq->ucReqStatus == CBUS_REQ_IDLE)
    {
        pReq->CbusMscCommand = API_MSC_INTERRUPT_GRT_WRT;
        pReq->ucCommand = MHL_SET_INT;
        pReq->ucOffsetData   = MHL_REG_RCHANGE_INT_OFFSET;
        pReq->ucMsgData[0]   = BIT_GRT_WRT_INT;
        pReq->dataRetHandler = sCbusSET_INT_GRT_WRT_Done;
        pReq->pCommandReport = pCommandReport;
        pReq->ucRetry = 1;

        bResult = sCbusWriteCommand(mhlObj, pReq);
        pReq->ulBase_time = SiI_get_global_time();
        pReq->ucReqStatus = CBUS_REQ_WAITING;
        pReq->ulTimeout = DEM_MDT_WAIT;
        SII_LIB_LOG_DEBUG1(mhlObj, ("GRT_WRT:: Sending GRT_WRT\n"));
    }
    else
    {
        SII_LIB_LOG_DEBUG1(mhlObj, ("GRT_WRT:: Fail to Send GRT_WRT. CBus is busy. Try later.\n"));
        bResult = false;
    }

    return bResult;
}

/*****************************************************************************/

/**
 * @Function:     sCbusSET_INT_DSCR_CHG_Done
 *
 * @Description:
 *
 *        Return handler for sCbusSET_INT_DSCR_CHG
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusSET_INT_DSCR_CHG_Done(void *pObj, uint8_t ucData)
{
	MhlTxObj_t *mhlObj = (MhlTxObj_t *)pObj;
    ucData = ucData;

    sCbusOutQCleanActiveReq(mhlObj);

    return (true);
}

/*****************************************************************************/

/**
 * @Function:     sCbusSET_INT_DSCR_CHG
 *
 * @Description:
 *
 *         Set the Interrupt Register of Peer Device.
 *         The Interrupt bit is DSCR_CHG.
 *
 * @Parameters: pCommandReport
 *
 *         If NOT null, this function will be invoked when command is sent or timeout.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusSET_INT_DSCR_CHG(MhlTxObj_t *mhlObj, PCOMMANDREPORT_t pCommandReport)
{
    uint8_t bResult = true;
    cbus_out_queue_t *pReq = sCbusGetOutQReq(mhlObj);

    if (true == sCbusGetAbortWait(mhlObj))
    {
        // SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: CBUS is unavailable in Abort Waiting.\n"));
        return false;
    }

    if (pReq->ucReqStatus == CBUS_REQ_IDLE)
    {
        pReq->CbusMscCommand = API_MSC_INTERRUPT_DSCR_CHG;
        pReq->ucCommand = MHL_SET_INT;
        pReq->ucOffsetData   = MHL_REG_RCHANGE_INT_OFFSET;
        pReq->ucMsgData[0]   = BIT_DSCR_CHG_INT;
        pReq->dataRetHandler = sCbusSET_INT_DSCR_CHG_Done;
        pReq->pCommandReport = pCommandReport;
        pReq->ucRetry = 1;

        bResult = sCbusWriteCommand(mhlObj, pReq);
        pReq->ulBase_time = SiI_get_global_time();
        pReq->ucReqStatus = CBUS_REQ_WAITING;
        pReq->ulTimeout = DEM_MDT_WAIT;

        SII_LIB_LOG_DEBUG1(mhlObj, ("DSCR_CHG:: Sending DSCR_CHG\n"));
    }
    else
    {
        SII_LIB_LOG_DEBUG1(mhlObj, ("DSCR_CHG:: Fail to Send DSCR_CHG. CBus is busy. Try later.\n"));
        bResult = false;
    }

    return bResult;
}

/*****************************************************************************/

/**
 * @Function:     sCbusSET_INT_DCAP_CHG_Done
 *
 * @Description:
 *
 *        Return Handler for sCbusSET_INT_DCAP_CHG
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusSET_INT_DCAP_CHG_Done(void *pObj, uint8_t ucData)
{
	MhlTxObj_t *mhlObj = (MhlTxObj_t *)pObj;
    ucData = ucData;

    sCbusOutQCleanActiveReq(mhlObj);

    return (true);
}

/*****************************************************************************/

/**
 * @Function:     sCbusSET_INT_DCAP_CHG
 *
 * @Description:
 *
 *         Set the Interrupt Register of Peer Device.
 *         The Interrupt bit is DCAP_CHG.
 *
 * @Parameters: pCommandReport
 *
 *         If NOT null, this function will be invoked when command is sent or timeout.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusSET_INT_DCAP_CHG(MhlTxObj_t *mhlObj, PCOMMANDREPORT_t pCommandReport)
{
    uint8_t bResult = true;
    cbus_out_queue_t *pReq = sCbusGetOutQReq(mhlObj);

    if (true == sCbusGetAbortWait(mhlObj))
    {
        // SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: CBUS is unavailable in Abort Waiting.\n"));
        return false;
    }

    if (pReq->ucReqStatus == CBUS_REQ_IDLE)
    {
        //Make sure DEV_CAP initialized by FW. Cbus reset will also reset DEV_CAP.
        sCbusInitDevCap(mhlObj);
        sCbusInitXDevCap(mhlObj);
		SII_LIB_LOG_DEBUG1(mhlObj, ("DCAP_CHG:: Sending DCAP_CHG\n"));
        pReq->CbusMscCommand = API_MSC_INTERRUPT_DCAP_CHG;
        pReq->ucCommand = MHL_SET_INT;
        pReq->ucOffsetData   = MHL_REG_RCHANGE_INT_OFFSET;
        pReq->ucMsgData[0]   = BIT_DCAP_CHG_INT;
        pReq->dataRetHandler = sCbusSET_INT_DCAP_CHG_Done;
        pReq->pCommandReport = pCommandReport;
        pReq->ucRetry = 1;

        bResult = sCbusWriteCommand(mhlObj, pReq);
        pReq->ulBase_time = SiI_get_global_time();
        pReq->ucReqStatus = CBUS_REQ_WAITING;
        pReq->ulTimeout = DEM_MSC_MAX_REPLY_DELAY;
	}
    else
    {
        SII_LIB_LOG_DEBUG1(mhlObj, ("DCAP_CHG:: Fail to Send DCAP_CHG. CBus is busy. Try later.\n"));
        bResult = false;
    }

    return bResult;
}

/*****************************************************************************/

/**
 * @Function:     sCbusSET_INT_3D_REQ_Done
 *
 * @Description:
 *
 *        Return Handler for sCbusSET_INT_3D_REQ
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusSET_INT_3D_REQ_Done(void *pObj, uint8_t ucData)
{
	MhlTxObj_t *mhlObj = (MhlTxObj_t *)pObj;
    ucData = ucData;

    sCbusOutQCleanActiveReq(mhlObj);

    return (true);
}

/*****************************************************************************/

/**
 * @Function:     sCbusSET_INT_3D_REQ
 *
 * @Description:
 *
 *         Set the Interrupt Register of Peer Device.
 *         The Interrupt bit is 3D_REQ.
 *
 * @Parameters: pCommandReport
 *
 *         If NOT null, this function will be invoked when command is sent or timeout.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusSET_INT_3D_REQ(MhlTxObj_t *mhlObj, PCOMMANDREPORT_t pCommandReport)
{
    uint8_t bResult = true;
    cbus_out_queue_t *pReq = sCbusGetOutQReq(mhlObj);

    if (true == sCbusGetAbortWait(mhlObj))
    {
        // SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: CBUS is unavailable in Abort Waiting.\n"));
        return false;
    }

    if (pReq->ucReqStatus == CBUS_REQ_IDLE)
    {
        pReq->CbusMscCommand = API_MSC_INTERRUPT_3D_REQ;
        pReq->ucCommand = MHL_SET_INT;
        pReq->ucOffsetData   = MHL_REG_RCHANGE_INT_OFFSET;
        pReq->ucMsgData[0]   = BIT_3D_REQ_INT;
        pReq->dataRetHandler = sCbusSET_INT_3D_REQ_Done;
        pReq->pCommandReport = pCommandReport;
        pReq->ucRetry = 2; // retry 2 times if timeout or abort for important MSC commands

        bResult = sCbusWriteCommand(mhlObj, pReq);

        pReq->ulBase_time = SiI_get_global_time();
        pReq->ucReqStatus = CBUS_REQ_WAITING;
        pReq->ulTimeout = DEM_MSC_MAX_REPLY_DELAY;

        SII_LIB_LOG_DEBUG1(mhlObj, ("3D_REQ:: Sending 3D_REQ\n"));
    }
    else
    {
        SII_LIB_LOG_DEBUG1(mhlObj, ("3D_REQ:: Fail to Send 3D_REQ. CBus is busy. Try later.\n"));
        bResult = false;
    }

    return bResult;
}

/*****************************************************************************/

/**
 * @Function:     sCbusSET_INT_EDID_CHG_Done
 *
 * @Description:
 *
 *        Return Handler for sCbusSET_INT_EDID_CHG
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusSET_INT_EDID_CHG_Done(void *pObj, uint8_t ucData)
{
	MhlTxObj_t *mhlObj = (MhlTxObj_t *)pObj;
    ucData = ucData;

    sCbusOutQCleanActiveReq(mhlObj);

    return (true);
}

/*****************************************************************************/

/**
 * @Function:     sCbusSET_INT_EDID_CHG
 *
 * @Description:
 *
 *         Set the Interrupt Register of Peer Device.
 *         The Interrupt bit is EDID_REQ.
 *
 * @Parameters: pCommandReport
 *
 *         If NOT null, this function will be invoked when command is sent or timeout.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusSET_INT_EDID_CHG(MhlTxObj_t *mhlObj, PCOMMANDREPORT_t pCommandReport)
{
    uint8_t bResult = true;
    cbus_out_queue_t *pReq = sCbusGetOutQReq(mhlObj);

    if (true == sCbusGetAbortWait(mhlObj))
    {
        // SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: CBUS is unavailable in Abort Waiting.\n"));
        return false;
    }

    if (pReq->ucReqStatus == CBUS_REQ_IDLE)
    {
        pReq->CbusMscCommand = API_MSC_INTERRUPT_EDID_CHG;
        pReq->ucCommand = MHL_SET_INT;
        pReq->ucOffsetData   = MHL_REG_RCHANGE_INT_OFFSET;
        pReq->ucMsgData[0]   = BIT_EDID_CHG_INT;
        pReq->dataRetHandler = sCbusSET_INT_EDID_CHG_Done;
        pReq->pCommandReport = pCommandReport;
        pReq->ucRetry = 1;

        bResult = sCbusWriteCommand(mhlObj, pReq);
        pReq->ulBase_time = SiI_get_global_time();
        pReq->ucReqStatus = CBUS_REQ_WAITING;
        pReq->ulTimeout = DEM_MSC_MAX_REPLY_DELAY;

        SII_LIB_LOG_DEBUG1(mhlObj, ("EDID_CHG:: Sending EDID_CHG\n"));
    }
    else
    {
        SII_LIB_LOG_DEBUG1(mhlObj, ("EDID_CHG:: Fail to Send EDID_CHG. CBus is busy. Try later.\n"));
        bResult = false;
    }

    return bResult;
}

/*****************************************************************************/

/**
 * @Function:     sCbusWRITE_STAT_LINK_MODE_Done
 *
 * @Description:
 *
 *        Return Handler for sCbusWRITE_STAT_LINK_MODE
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusWRITE_STAT_LINK_MODE_Done(void *pObj, uint8_t ucData)
{
	MhlTxObj_t *mhlObj = (MhlTxObj_t *)pObj;
    ucData = ucData;

    sCbusOutQCleanActiveReq(mhlObj);

    return (true);
}

/*****************************************************************************/

/**
 * @Function:     sCbusWRITE_STAT_LINK_MODE
 *
 * @Description:
 *
 *         Set the Status Register of Peer Device.
 *         The status bit is CLK_MODE.
 *
 * @Parameters: ucLINK_MODE
 *
 *         The LINK_MODE to set.
 *
 * @Parameters: pCommandReport
 *
 *         If NOT null, this function will be invoked when command is sent or timeout.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusWRITE_STAT_LINK_MODE(MhlTxObj_t *mhlObj, uint8_t ucLINK_MODE, PCOMMANDREPORT_t pCommandReport)
{
    uint8_t bResult = true;
    cbus_out_queue_t *pReq = sCbusGetOutQReq(mhlObj);

    if (true == sCbusGetAbortWait(mhlObj))
    {
        // SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: CBUS is unavailable in Abort Waiting.\n"));
        return false;
    }

    if (pReq->ucReqStatus == CBUS_REQ_IDLE)
    {
        pReq->CbusMscCommand = API_MSC_STATUS_LINK_MODE;
        pReq->ucCommand = MHL_WRITE_STAT;
        pReq->ucOffsetData   = MHL_REG_LINK_MODE_OFFSET;
        pReq->ucMsgData[0]   = (uint8_t)ucLINK_MODE;
        pReq->dataRetHandler = sCbusWRITE_STAT_LINK_MODE_Done;
        pReq->pCommandReport = pCommandReport;
        pReq->ucRetry = 1;

        bResult = sCbusWriteCommand(mhlObj, pReq);
        pReq->ulBase_time = SiI_get_global_time();
        pReq->ucReqStatus = CBUS_REQ_WAITING;
        pReq->ulTimeout = DEM_MSC_MAX_REPLY_DELAY;

        SII_LIB_LOG_DEBUG1(mhlObj, ("LINK_MODE:: Sending LINK_MODE\n"));
    }
    else
    {
        SII_LIB_LOG_DEBUG1(mhlObj, ("LINK_MODE:: Fail to Send LINK_MODE. CBus is busy. Try later.\n"));
        bResult = false;
    }

    return bResult;
}

/*****************************************************************************/

/**
 * @Function:     sCbusWRITE_BURST_Done
 *
 * @Description:
 *
 *        Return Handler for sCbusWRITE_BURST
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusWRITE_BURST_Done(void *pObj, uint8_t ucData)
{
	MhlTxObj_t *mhlObj = (MhlTxObj_t *)pObj;
    ucData = ucData;

    sCbusOutQCleanActiveReq(mhlObj);

    sCbusSET_INT_DSCR_CHG(mhlObj, NULL);

    return (true);
}

/*****************************************************************************/

/**
 * @Function:     sCbusWRITE_BURST
 *
 * @Description:
 *
 *         Send WRITE_BURST Command
 *
 * @Parameters: pArgument
 *
 *         Buffer to store data.
 *
 * @Parameters: ucLength
 *
 *         The length of buffer indicated by pArgument.
 *
 * @Parameters: pCommandReport
 *
 *         If NOT null, this function will be invoked when command is sent or timeout.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusWRITE_BURST(MhlTxObj_t *mhlObj, uint8_t* pArgument, uint8_t ucLength, PCOMMANDREPORT_t pCommandReport)
{
    uint8_t bResult = true;
    cbus_out_queue_t *pReq = sCbusGetOutQReq(mhlObj);

    if (pReq->ucReqStatus == CBUS_REQ_WAITING)
    {
        pReq->CbusMscCommand = API_MSC_SCRATCHPAD_WRITE_BURST;
        pReq->ucCommand = MHL_WRITE_BURST;
        pReq->ucOffsetData = MHL_SCRATCHPAD_OFFSET;
        memcpy(pReq->ucMsgData, pArgument, ucLength);
        pReq->dataRetHandler = sCbusWRITE_BURST_Done;
        pReq->pCommandReport = pCommandReport;
        pReq->ucRetry = 1;

        bResult = sCbusWriteCommand(mhlObj, pReq);
        pReq->ulBase_time = SiI_get_global_time();
        pReq->ucReqStatus = CBUS_REQ_WAITING;
        pReq->ulTimeout = DEM_MSC_MAX_REPLY_DELAY;

        SII_LIB_LOG_DEBUG1(mhlObj, ("WRITE_BURST:: Sending WRITE_BURST\n"));
    }
    else
    {
        SII_LIB_LOG_DEBUG1(mhlObj, ("WRITE_BURST:: Fail to Send WRITE_BURST.\n"));
        bResult = false;
    }

    return bResult;
}

/*****************************************************************************/

/**
 * @Function:     sCbusWRITE_STAT_DCAP_RDY_Done
 *
 * @Description:
 *
 *        Return Handler for sCbusWRITE_STAT_DCAP_RDY
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusWRITE_STAT_DCAP_RDY_Done(void *pObj, uint8_t ucData)
{
	MhlTxObj_t *mhlObj = (MhlTxObj_t *)pObj;
    ucData = ucData;

    sCbusOutQCleanActiveReq(mhlObj);

    return (true);
}

/*****************************************************************************/

/**
 * @Function:     sCbusWRITE_STAT_DCAP_RDY
 *
 * @Description:
 *
 *         Set the Status Register of Peer Device.
 *         The status bit is DCAP_RDY.
 *
 * @Parameters: pCommandReport
 *
 *         If NOT null, this function will be invoked when command is sent or timeout.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusWRITE_STAT_DCAP_RDY(MhlTxObj_t *mhlObj, PCOMMANDREPORT_t pCommandReport)
{
    uint8_t bResult = true;
    cbus_out_queue_t *pReq = sCbusGetOutQReq(mhlObj);

    if (true == sCbusGetAbortWait(mhlObj))
    {
        // SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: CBUS is unavailable in Abort Waiting.\n"));
        return false;
    }

    if (pReq->ucReqStatus == CBUS_REQ_IDLE)
    {
        //Make sure DEV_CAP initialized by FW. Cbus reset will also reset DEV_CAP.
        sCbusInitDevCap(mhlObj);
        sCbusInitXDevCap(mhlObj);
		SII_LIB_LOG_DEBUG1(mhlObj, ("DCAP_RDY:: Sending DCAP_RDY\n"));
        pReq->CbusMscCommand = API_MSC_STATUS_DCAP_RDY;
        pReq->ucCommand		 = MHL_WRITE_STAT;
        pReq->ucOffsetData   = MHL_REG_CONNECTED_RDY_OFFSET;
        pReq->ucMsgData[0]   = BIT_ENUM__MHL_STAT_0__DCAP_READY | BIT_ENUM__MHL_STAT_0__XDEVCAP_SUPP;
        pReq->dataRetHandler = sCbusWRITE_STAT_DCAP_RDY_Done;
        pReq->pCommandReport = pCommandReport;
        pReq->ucRetry = 1;

        bResult = sCbusWriteCommand(mhlObj, pReq);

        pReq->ulBase_time = SiI_get_global_time();
        pReq->ucReqStatus = CBUS_REQ_WAITING;
        pReq->ulTimeout = DEM_MSC_MAX_REPLY_DELAY;
	}
    else
    {
        SII_LIB_LOG_DEBUG1(mhlObj, ("DCAP_RDY:: Fail to Send DCAP_RDY. CBus is busy. Try later.\n"));
        bResult = false;
    }

    return bResult;
}

static bool_t sCbusWRITE_STAT_VERSION_STAT(MhlTxObj_t *mhlObj, PCOMMANDREPORT_t pCommandReport)
{
	uint8_t bResult = true;
    cbus_out_queue_t *pReq = sCbusGetOutQReq(mhlObj);

    if (true == sCbusGetAbortWait(mhlObj))
    {
        // SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: CBUS is unavailable in Abort Waiting.\n"));
        return false;
    }

	if (pReq->ucReqStatus == CBUS_REQ_IDLE)
    {
		SII_LIB_LOG_DEBUG1(mhlObj, ("VERSION_STAT:: Sending MHL Version\n"));
        pReq->CbusMscCommand = API_MSC_STATUS_VERSION_STAT;
        pReq->ucCommand		 = MHL_WRITE_STAT;
		pReq->ucOffsetData   = MHL_REG_VERSION_STAT_OFFSET;
		pReq->ucMsgData[0]   = BIT_ENUM__MHL_STAT_2__MHL_VER_MINOR | BIT_ENUM__MHL_STAT_2__MHL_VER_MAJOR;
        pReq->dataRetHandler = sCbusWRITE_STAT_Version_Stat_Done;
        pReq->pCommandReport = pCommandReport;
        pReq->ucRetry = 1;

        bResult = sCbusWriteCommand(mhlObj, pReq);

        pReq->ulBase_time = SiI_get_global_time();
        pReq->ucReqStatus = CBUS_REQ_WAITING;
        pReq->ulTimeout = DEM_MSC_MAX_REPLY_DELAY;
	}
    else
    {
        SII_LIB_LOG_DEBUG1(mhlObj, ("VERSION_STAT:: Fail to Send VERSION_STAT. CBus is busy. Try later.\n"));
        bResult = false;
    }

    return bResult;
}
/*****************************************************************************/

/**
 * @Function:     sCbusMSC_MSG_UCP_Done
 *
 * @Description:
 *
 *        Return Handler for sCbusMSC_MSG_UCP
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMSC_MSG_UCP_Done(MhlTxObj_t *mhlObj, uint8_t ucData)
{
    ucData = ucData;

    sCbusOutQCleanActiveReq(mhlObj);

    return (true);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMSC_MSG_UCP
 *
 * @Description:
 *
 *
 * @Parameters: ucKeycode
 *
 *         UCP keycode to send
 *
 * @Parameters: pCommandReport
 *
 *         If NOT null, this function will be invoked when command is sent or timeout.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMSC_MSG_UCP(MhlTxObj_t *mhlObj, uint8_t ucKeycode, PCOMMANDREPORT_t pCommandReport)
{
    uint8_t bResult = true;
    cbus_out_queue_t *pReq = sCbusGetOutQReq(mhlObj);

    if (true == sCbusGetAbortWait(mhlObj))
    {
        // SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: CBUS is unavailable in Abort Waiting.\n"));
        return false;
    }

    if (pReq->ucReqStatus == CBUS_REQ_IDLE)
    {
        pReq->CbusMscCommand = API_MSC_MSG_UCP;

        pReq->ucCommand = MHL_MSC_MSG;
        pReq->ucMsgData[0]   = MHL_MSC_MSG_UCP;
        pReq->ucMsgData[1]   = ucKeycode;
        pReq->pCommandReport = pCommandReport;
        pReq->dataRetHandler = sCbusMSC_MSG_UCP_Done;
        pReq->sDataRetHandlerName = "sCbusMSC_MSG_UCP_Done";
        pReq->ucRetry = 1;

        bResult = sCbusWriteCommand(mhlObj, pReq);

        pReq->ucReqStatus = CBUS_REQ_WAITING;
        pReq->ulBase_time = SiI_get_global_time();
        pReq->ulTimeout = DEM_MSC_MAX_REPLY_DELAY;

        SII_LIB_LOG_DEBUG1(mhlObj, ("RCP:: Sending UCP -> Key Code: %02d\n", (int)ucKeycode));
    }
    else
    {
        SII_LIB_LOG_DEBUG1(mhlObj, ("RCP:: Fail to Send UCP. CBus is busy. Try later -> Keycode = 0x%02X\n", (int)ucKeycode));
        bResult = false;
    }

    return (bResult);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMSC_MSG_UCPE_Done
 *
 * @Description:
 *
 *        Return Handler for sCbusMSC_MSG_UCPE
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMSC_MSG_UCPE_Done(MhlTxObj_t *mhlObj, uint8_t ucData)
{
    ucData = ucData;

    sCbusOutQCleanActiveReq(mhlObj);

    return (true);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMSC_MSG_UCPE
 *
 * @Description:
 *
 *
 * @Parameters: ucStatusCode
 *
 *         UCPE status code
 *
 * @Parameters: ucKeycode
 *
 *         UCP keycode which has the status code.
 *
 * @Parameters: pCommandReport
 *
 *         If NOT null, this function will be invoked when command is sent or timeout.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMSC_MSG_UCPE(MhlTxObj_t *mhlObj, uint8_t ucStatusCode, PCOMMANDREPORT_t pCommandReport)
{
    uint8_t bResult = true;
    cbus_out_queue_t *pReq = sCbusGetOutQReq(mhlObj);

    if (true == sCbusGetAbortWait(mhlObj))
    {
        // SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: CBUS is unavailable in Abort Waiting.\n"));
        return false;
    }

    if (pReq->ucReqStatus == CBUS_REQ_IDLE)
    {
        pReq->CbusMscCommand = API_MSC_MSG_UCPE;

        pReq->ucCommand = MHL_MSC_MSG;
        pReq->ucMsgData[0]   = MHL_MSC_MSG_UCPE;
        pReq->ucMsgData[1]   = ucStatusCode;
        pReq->pCommandReport = pCommandReport;
        pReq->dataRetHandler = sCbusMSC_MSG_UCPE_Done;
        pReq->sDataRetHandlerName = "sCbusMSC_MSG_UCPE_Done";
        pReq->ucRetry = 1;

        bResult = sCbusWriteCommand(mhlObj, pReq);

        pReq->ucReqStatus = CBUS_REQ_WAITING;
        pReq->ulBase_time = SiI_get_global_time();
        pReq->ulTimeout = DEM_MSC_MAX_REPLY_DELAY;

        SII_LIB_LOG_DEBUG1(mhlObj, ("RCP:: Sending UCPE -> Status Code: %02d\n", (int)ucStatusCode));
    }
    else
    {
        SII_LIB_LOG_DEBUG1(mhlObj, ("RCP:: Fail to Send UCPE. CBus is busy. Try later -> Status Code = 0x%02X\n",
                              (int)ucStatusCode));
        bResult = false;
    }

    return (bResult);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMSC_MSG_UCPK_Done
 *
 * @Description:
 *
 *        Return Handler for sCbusMSC_MSG_UCPK
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMSC_MSG_UCPK_Done(MhlTxObj_t *mhlObj, uint8_t ucData)
{
    ucData = ucData;

    sCbusOutQCleanActiveReq(mhlObj);

    return (true);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMSC_MSG_UCPK
 *
 * @Description:
 *
 *        Send MSC_MSG sub command UCPK.
 *
 * @Parameters: ucKeycode
 *
 *        Keycode received.
 *
 * @Parameters: pCommandReport
 *
 *         If NOT null, this function will be invoked when command is sent or timeout.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMSC_MSG_UCPK(MhlTxObj_t *mhlObj, uint8_t ucKeycode, PCOMMANDREPORT_t pCommandReport)
{
    uint8_t bResult = true;
    cbus_out_queue_t *pReq = sCbusGetOutQReq(mhlObj);

    if (true == sCbusGetAbortWait(mhlObj))
    {
        // SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: CBUS is unavailable in Abort Waiting.\n"));
        return false;
    }

    if (pReq->ucReqStatus == CBUS_REQ_IDLE)
    {
        pReq->CbusMscCommand = API_MSC_MSG_UCPK;

        pReq->ucCommand = MHL_MSC_MSG;
        pReq->ucMsgData[0]   = MHL_MSC_MSG_UCPK;
        pReq->ucMsgData[1]   = ucKeycode;
        pReq->pCommandReport = pCommandReport;
        pReq->dataRetHandler = sCbusMSC_MSG_UCPK_Done;
        pReq->sDataRetHandlerName = "sCbusMSC_MSG_UCPK_Done";
        pReq->ucRetry = 1;

        bResult = sCbusWriteCommand(mhlObj, pReq);

        pReq->ucReqStatus = CBUS_REQ_WAITING;
        pReq->ulBase_time = SiI_get_global_time();
        pReq->ulTimeout = DEM_MSC_MAX_REPLY_DELAY;

        SII_LIB_LOG_DEBUG1(mhlObj, ("RCP:: Sending UCPK -> Key Code: %02d\n", (int)ucKeycode));
    }
    else
    {
        SII_LIB_LOG_DEBUG1(mhlObj, ("RCP:: Fail to Send UCPK. CBus is busy. Try later -> Key Code = 0x%02X\n", (int)ucKeycode));
        bResult = false;
    }

    return (bResult);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMSC_MSG_RCP_Done
 *
 * @Description:
 *
 *        Return Handler for sCbusMSC_MSG_RCP
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMSC_MSG_RCP_Done(MhlTxObj_t *mhlObj, uint8_t ucData)
{
    ucData = ucData;

    sCbusOutQCleanActiveReq(mhlObj);

    return (true);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMSC_MSG_RCP
 *
 * @Description:
 *
 *
 * @Parameters: ucKeycode
 *
 *         RCP keycode to send
 *
 * @Parameters: pCommandReport
 *
 *         If NOT null, this function will be invoked when command is sent or timeout.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMSC_MSG_RCP(MhlTxObj_t *mhlObj, uint8_t ucKeycode, PCOMMANDREPORT_t pCommandReport)
{
    uint8_t bResult = true;
    cbus_out_queue_t *pReq = sCbusGetOutQReq(mhlObj);

    if (true == sCbusGetAbortWait(mhlObj))
    {
        // SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: CBUS is unavailable in Abort Waiting.\n"));
        return false;
    }

    if (pReq->ucReqStatus == CBUS_REQ_IDLE)
    {
        pReq->CbusMscCommand = API_MSC_MSG_RCP;

        pReq->ucCommand = MHL_MSC_MSG;
        pReq->ucMsgData[0]   = MHL_MSC_MSG_RCP;
        pReq->ucMsgData[1]   = ucKeycode;
        pReq->pCommandReport = pCommandReport;
        pReq->dataRetHandler = sCbusMSC_MSG_RCP_Done;
        pReq->sDataRetHandlerName = "sCbusMSC_MSG_RCP_Done";
        pReq->ucRetry = 1;

        bResult = sCbusWriteCommand(mhlObj, pReq);

        pReq->ucReqStatus = CBUS_REQ_WAITING;
        pReq->ulBase_time = SiI_get_global_time();
        pReq->ulTimeout = DEM_MSC_MAX_REPLY_DELAY;

        SII_LIB_LOG_DEBUG1(mhlObj, ("RCP:: Sending RCP -> Keycode = 0x%02x\n", (int)ucKeycode));
    }
    else
    {
        SII_LIB_LOG_DEBUG1(mhlObj, ("RCP:: Fail to Send RCP. CBus is busy. Try later -> Keycode = 0x%02X\n", (int)ucKeycode));
        bResult = false;
    }

    return (bResult);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMSC_MSG_RCPE_Done
 *
 * @Description:
 *
 *        Return Handler for sCbusMSC_MSG_RCPE
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMSC_MSG_RCPE_Done(MhlTxObj_t *mhlObj, uint8_t ucData)
{
	uint8_t bResult;
    ucData = ucData;
	
    sCbusOutQCleanActiveReq(mhlObj);	
	// RCPE is always followed by an RCPK with original key code that came.
	//SiiCbusSendCommand(mhlObj, API_MSC_MSG_RCPK, &(mhlObj->mscSaveRcpKeyCode), 0x1, NULL);
	bResult = sCbusMSC_MSG_RCPK(mhlObj, mhlObj->mscSaveRcpKeyCode, NULL);
	sCbusInQCleanActiveReq(mhlObj);
    return (true);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMSC_MSG_RCPE
 *
 * @Description:
 *
 *
 * @Parameters: ucStatusCode
 *
 *         RCPE status code
 *
 * @Parameters: ucKeycode
 *
 *         RCP keycode which has the status code.
 *
 * @Parameters: pCommandReport
 *
 *         If NOT null, this function will be invoked when command is sent or timeout.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMSC_MSG_RCPE(MhlTxObj_t *mhlObj, uint8_t ucStatusCode, PCOMMANDREPORT_t pCommandReport)
{
    uint8_t bResult = true;
    cbus_out_queue_t *pReq = sCbusGetOutQReq(mhlObj);

    if (true == sCbusGetAbortWait(mhlObj))
    {
        // SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: CBUS is unavailable in Abort Waiting.\n"));
        return false;
    }

    if (pReq->ucReqStatus == CBUS_REQ_IDLE)
    {
        pReq->CbusMscCommand = API_MSC_MSG_RCPE;

        pReq->ucCommand = MHL_MSC_MSG;
        pReq->ucMsgData[0]   = MHL_MSC_MSG_RCPE;
        pReq->ucMsgData[1]   = ucStatusCode;
        pReq->pCommandReport = pCommandReport;
        pReq->dataRetHandler = sCbusMSC_MSG_RCPE_Done;
        pReq->sDataRetHandlerName = "sCbusMSC_MSG_RCPE_Done";
        pReq->ucRetry = 1;

        bResult = sCbusWriteCommand(mhlObj, pReq);

        pReq->ucReqStatus = CBUS_REQ_WAITING;
        pReq->ulBase_time = SiI_get_global_time();
        pReq->ulTimeout = DEM_MSC_MAX_REPLY_DELAY;

        SII_LIB_LOG_DEBUG1(mhlObj, ("RCP:: Sending RCPE -> Status Code: %02d\n", (int)ucStatusCode));
    }
    else
    {
        SII_LIB_LOG_DEBUG1(mhlObj, ("RCP:: Fail to Send RCPE. CBus is busy. Try later -> Status Code = 0x%02X\n",
                              (int)ucStatusCode));
        bResult = false;
    }

    return (bResult);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMSC_MSG_RCPK_Done
 *
 * @Description:
 *
 *        Return Handler for sCbusMSC_MSG_RCPK
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMSC_MSG_RCPK_Done(MhlTxObj_t *mhlObj, uint8_t ucData)
{
	
    ucData = ucData;

    sCbusOutQCleanActiveReq(mhlObj);

    return (true);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMSC_MSG_RCPK
 *
 * @Description:
 *
 *        Send MSC_MSG sub command RCPK.
 *
 * @Parameters: ucKeycode
 *
 *        Keycode received.
 *
 * @Parameters: pCommandReport
 *
 *         If NOT null, this function will be invoked when command is sent or timeout.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMSC_MSG_RCPK(MhlTxObj_t *mhlObj, uint8_t ucKeycode, PCOMMANDREPORT_t pCommandReport)
{
    uint8_t bResult = true;
    cbus_out_queue_t *pReq = sCbusGetOutQReq(mhlObj);

    if (true == sCbusGetAbortWait(mhlObj))
    {
        // SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: CBUS is unavailable in Abort Waiting.\n"));
        return false;
    }

    if (pReq->ucReqStatus == CBUS_REQ_IDLE)
    {
        pReq->CbusMscCommand = API_MSC_MSG_RCPK;

        pReq->ucCommand = MHL_MSC_MSG;
        pReq->ucMsgData[0]   = MHL_MSC_MSG_RCPK;
        pReq->ucMsgData[1]   = ucKeycode;
        pReq->pCommandReport = pCommandReport;
        pReq->dataRetHandler = sCbusMSC_MSG_RCPK_Done;
        pReq->sDataRetHandlerName = "sCbusMSC_MSG_RCPK_Done";
        pReq->ucRetry = 1;

        bResult = sCbusWriteCommand(mhlObj, pReq);

        pReq->ucReqStatus = CBUS_REQ_WAITING;
        pReq->ulBase_time = SiI_get_global_time();
        pReq->ulTimeout = DEM_MSC_MAX_REPLY_DELAY;

        SII_LIB_LOG_DEBUG1(mhlObj, ("RCP:: Sending RCPK -> Key Code: 0x%02x\n", (int)ucKeycode));
    }
    else
    {
        SII_LIB_LOG_DEBUG1(mhlObj, ("RCP:: Fail to Send RCPK. CBus is busy. Try later -> Key Code = 0x%02X\n", (int)ucKeycode));
        bResult = false;
    }

    return (bResult);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMSC_MSG_RAP_Done
 *
 * @Description:
 *
 *        Return Handler for sCbusMSC_MSG_RAP
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMSC_MSG_RAP_Done(MhlTxObj_t *mhlObj, uint8_t ucData)
{
	
    cbus_out_queue_t    *pReq = sCbusGetOutQReq(mhlObj);

    ucData = ucData;

    //Case 1: API request, Clean Output Queue directly.
    if (API_MSC_MSG_RAP == pReq->CbusMscCommand)
    {
        sCbusOutQCleanActiveReq(mhlObj);
    }
    //Case 2:Internal use. Dont clean output queue till RAPK is received.
    else
    {}

    return (true);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMSC_MSG_RCP
 *
 * @Description:
 *
 *
 * @Parameters: ucKeycode
 *
 *         RCP keycode to send
 *
 * @Parameters: pCommandReport
 *
 *         If NOT null, this function will be invoked when command is sent or timeout.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMSC_MSG_RAP(MhlTxObj_t *mhlObj, CBUS_API_MSC_COMMAND_t CbusMscCommand, uint8_t ucKeycode, PCOMMANDREPORT_t pCommandReport)
{
    bool_t bResult = false;
    cbus_out_queue_t    *pReq = sCbusGetOutQReq(mhlObj);

    if (true == sCbusGetAbortWait(mhlObj))
    {
        // SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: CBUS is unavailable in Abort Waiting.\n"));
        return false;
    }

    if (pReq->ucReqStatus == CBUS_REQ_IDLE)
    {
        pReq->CbusMscCommand = CbusMscCommand;

        pReq->ucCommand = MHL_MSC_MSG;
        pReq->ucMsgData[0]   = MHL_MSC_MSG_RAP;
        pReq->ucMsgData[1]   = ucKeycode;
        pReq->pCommandReport = pCommandReport;
        pReq->dataRetHandler = sCbusMSC_MSG_RAP_Done;
        pReq->sDataRetHandlerName = "sCbusMSC_MSG_RAP_Done";
        pReq->ucRetry = 1;
        bResult = sCbusWriteCommand(mhlObj, pReq);

        pReq->ucReqStatus = CBUS_REQ_WAITING;
        pReq->ulBase_time = SiI_get_global_time();
        pReq->ulTimeout = DEM_MSC_MAX_REPLY_DELAY;

        SII_LIB_LOG_DEBUG1(mhlObj, ("RAP:: Sending RAP -> Key Code: %02d\n", (int)ucKeycode));
    }
    else
    {
        SII_LIB_LOG_DEBUG1(mhlObj, ("RAP:: Fail to Send RAP. CBus is busy. Try later -> Key Code = 0x%02X\n", (int)ucKeycode));
        bResult = false;
    }

    return (bResult);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMSC_MSG_RAPK_Done
 *
 * @Description:
 *
 *        Return Handler for sCbusMSC_MSG_RCPK
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMSC_MSG_RAPK_Done(MhlTxObj_t *mhlObj, uint8_t ucData)
{
	
    ucData = ucData;

    sCbusOutQCleanActiveReq(mhlObj);

    return (true);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMSC_MSG_RAPK
 *
 * @Description:
 *
 *        Send MSC_MSG sub command RAPK.
 *
 * @Parameters: ucKeycode
 *
 *        Keycode received.
 *
 * @Parameters: pCommandReport
 *
 *        If NOT null, this function will be invoked when command is sent or timeout.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMSC_MSG_RAPK(MhlTxObj_t *mhlObj, uint8_t ucStatusCode, PCOMMANDREPORT_t pCommandReport)
{
    bool_t bResult = false;
    cbus_out_queue_t    *pReq = sCbusGetOutQReq(mhlObj);

    if (true == sCbusGetAbortWait(mhlObj))
    {
        // SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: CBUS is unavailable in Abort Waiting.\n"));
        return false;
    }

    if (pReq->ucReqStatus == CBUS_REQ_IDLE)
    {
        pReq->CbusMscCommand = API_MSC_MSG_RAPK;

        pReq->ucCommand = MHL_MSC_MSG;
        pReq->ucMsgData[0]   = MHL_MSC_MSG_RAPK;
        pReq->ucMsgData[1]   = ucStatusCode;
        pReq->pCommandReport = pCommandReport;
        pReq->dataRetHandler = sCbusMSC_MSG_RAPK_Done;
        pReq->sDataRetHandlerName = "sCbusMSC_MSG_RAPK_Done";
        pReq->ucRetry = 1;
        bResult = sCbusWriteCommand(mhlObj, pReq);

        pReq->ucReqStatus = CBUS_REQ_WAITING;
        pReq->ulBase_time = SiI_get_global_time();
        pReq->ulTimeout = DEM_MSC_MAX_REPLY_DELAY;

        SII_LIB_LOG_DEBUG1(mhlObj, ("RAP:: Sending RAPK -> Status Code: %02d\n", (int)ucStatusCode));
    }
    else
    {
        SII_LIB_LOG_DEBUG1(mhlObj, ("RAP:: Fail to Send RAPK. CBus is busy. Try later -> Status Code = 0x%02X\n",
                              (int)ucStatusCode));
        bResult = false;
    }

    return (bResult);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMSC_MSG_MSGE_Done
 *
 * @Description:
 *
 *         Return handler for sCbusMSC_MSG_MSGE
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMSC_MSG_MSGE_Done(MhlTxObj_t *mhlObj, uint8_t ucData)
{
	
    ucData = ucData;

    sCbusOutQCleanActiveReq(mhlObj);

    return (true);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMSC_MSG_MSGE
 *
 * @Description:
 *
 *         Set the Interrupt Register of Peer Device.
 *         The Interrupt bit is REQ_WRT.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMSC_MSG_MSGE(MhlTxObj_t *mhlObj, PCOMMANDREPORT_t pCommandReport)
{
    bool_t bResult = false;
    cbus_out_queue_t    *pReq = sCbusGetOutQReq(mhlObj);

    if (true == sCbusGetAbortWait(mhlObj))
    {
        // SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: CBUS is unavailable in Abort Waiting.\n"));
        return false;
    }

    if (pReq->ucReqStatus == CBUS_REQ_IDLE)
    {
        pReq->CbusMscCommand = API_MSC_MSGE;
        pReq->ucCommand = MHL_MSC_MSG;
        pReq->ucMsgData[0]   = MHL_MSC_MSGE;
        pReq->pCommandReport = pCommandReport;
        pReq->dataRetHandler = sCbusMSC_MSG_MSGE_Done;
        pReq->sDataRetHandlerName = "sCbusMSC_MSG_MSGE_Done";
        pReq->ucRetry = 1;

        bResult = sCbusWriteCommand(mhlObj, pReq);

        pReq->ulBase_time = SiI_get_global_time();
        pReq->ucReqStatus = CBUS_REQ_WAITING;
        pReq->ulTimeout = DEM_MDT_WAIT;

        SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Sending MSGE \n"));
    }
    else
    {
        SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Fail to Send MSGE. CBus is busy. Try later.\n"));
        bResult = false;
    }

    return (bResult);
}

static bool_t sCbusPrintoutPeerXDevCap(MhlTxObj_t *mhlObj)
{
    SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Peer's Extended Capability Registers:\n"));

    //Step 1: Parse ECBUS_SPEEDS.
    SII_LIB_LOG_DEBUG2(("    ECBUS_SPEEDS:"));
    if (mhlObj->cbus_state.ucPeerXDCAP[0] | BIT_ECBUS_S_075)
    {
        SII_LIB_LOG_DEBUG2(("        ECBUS_S_075: 1 = supports 75MHz eCBUS-S.\n"));
    }
    else
    {
        SII_LIB_LOG_DEBUG2(("        ECBUS_S_075: 0 = no support for 75MHz eCBUS-S.\n"));
    }

    if (mhlObj->cbus_state.ucPeerXDCAP[0] | BIT_ECBUS_S_8BIT)
    {
        SII_LIB_LOG_DEBUG2(("        ECBUS_S_8BIT: 1 = supports eCBUS-S 8-bit TDM slot mode.\n"));
    }
    else
    {
        SII_LIB_LOG_DEBUG2(("        ECBUS_S_8BIT: 0 = no support for eCBUS-S 8-bit TDM slot mode.\n"));
    }

    if (mhlObj->cbus_state.ucPeerXDCAP[0] | BIT_ECBUS_S_12BIT)
    {
        SII_LIB_LOG_DEBUG2(("        ECBUS_S_12BIT: 1 = supports eCBUS-S 12-bit TDM slot mode.\n"));
    }
    else
    {
        SII_LIB_LOG_DEBUG2(("        ECBUS_S_12BIT: 0 = no support for eCBUS-S 12-bit TDM slot mode.\n"));
    }

    if (mhlObj->cbus_state.ucPeerXDCAP[0] | BIT_ECBUS_D_150)
    {
        SII_LIB_LOG_DEBUG2(("        ECBUS_D_150: 1 = supports 150MHz eCBUS-D.\n"));
    }
    else
    {
        SII_LIB_LOG_DEBUG2(("        ECBUS_D_150: 0 = no support for 150MHz eCBUS-D.\n"));
    }

    if (mhlObj->cbus_state.ucPeerXDCAP[0] | BIT_ECBUS_D_8BIT)
    {
        SII_LIB_LOG_DEBUG2(("        ECBUS_D_8BIT: 1 = supports eCBUS-D 8-bit TDM slot mode.\n"));
    }
    else
    {
        SII_LIB_LOG_DEBUG2(("        ECBUS_D_8BIT: 0 = no support for eCBUS-D 8-bit TDM slot mode.\n"));
    }

    //Step 2: Parse TMDS_SPEEDS
    SII_LIB_LOG_DEBUG2(("    TMDS_SPEEDS:\n"));
    if (mhlObj->cbus_state.ucPeerXDCAP[1] | BIT_TMDS_150)
    {
        SII_LIB_LOG_DEBUG2(("        TMDS_150: 1 = supports 1.5Gbps on TMDS.\n"));
    }
    else
    {
        SII_LIB_LOG_DEBUG2(("        TMDS_150: 0 = no support for 1.5Gbps on TMDS.\n"));
    }

    if (mhlObj->cbus_state.ucPeerXDCAP[1] | BIT_TMDS_300)
    {
        SII_LIB_LOG_DEBUG2(("        TMDS_300: 1 = supports 3Gbps on TMDS.\n"));
    }
    else
    {
        SII_LIB_LOG_DEBUG2(("        TMDS_300: 0 = no support for 3Gbps on TMDS.\n"));
    }

    if (mhlObj->cbus_state.ucPeerXDCAP[1] | BIT_TMDS_600)
    {
        SII_LIB_LOG_DEBUG2(("        TMDS_600: 1 = supports 6Gbps on TMDS.\n"));
    }
    else
    {
        SII_LIB_LOG_DEBUG2(("        TMDS_600: 0 = no support for 6Gbps on TMDS.\n"));
    }

    //Step 3: Parse ECBUS_DEV_ROLES
    SII_LIB_LOG_DEBUG2(("    ECBUS_DEV_ROLES:\n"));
    if (mhlObj->cbus_state.ucPeerXDCAP[2] | BIT_DEV_HOST)
    {
        SII_LIB_LOG_DEBUG2(("        DEV_HOST: 1 = The device is capable of providing Host role.\n"));
    }
    else
    {
        SII_LIB_LOG_DEBUG2(("        DEV_HOST: 0 = The device is not capable of providing Host role.\n"));
    }

    if (mhlObj->cbus_state.ucPeerXDCAP[2] | BIT_DEV_DEVICE)
    {
        SII_LIB_LOG_DEBUG2(("        DEV_DEVICE: 1 = The device is capable of providing Device role.\n"));
    }
    else
    {
        SII_LIB_LOG_DEBUG2(("        DEV_DEVICE: 0 = The device is not capable of providing Device role.\n"));
    }

    if (mhlObj->cbus_state.ucPeerXDCAP[2] | BIT_DEV_CHARGER)
    {
        SII_LIB_LOG_DEBUG2(("        DEV_CHARGER: 1 = The device is capable of providing Charger role.\n"));
    }
    else
    {
        SII_LIB_LOG_DEBUG2(("        DEV_CHARGER: 0 = The device is not capable of providing Charger role.\n"));
    }

    //Step 4: Parse LOG_DEV_MAPX
    SII_LIB_LOG_DEBUG2(("    LOG_DEV_MAPX:\n"));
    if (mhlObj->cbus_state.ucPeerXDCAP[3] | BIT_LD_PHONE)
    {
        SII_LIB_LOG_DEBUG2(("        LD_PHONE: 1 = Device Supports Phone functions.\n"));
    }
    else
    {
        SII_LIB_LOG_DEBUG2(("        LD_PHONE: 0 = Device Does not support Phone functions.\n"));
    }

    return true;
}

static bool_t sCbusMscTxSendDcapChg_Done(void *pObj, CBUS_CALLBACK_PARAMETERS_t *pCbusCallBackParameters)
{
	MhlTxObj_t *mhlObj = (MhlTxObj_t *)pObj;
    //ucData = ucData;
    mhlObj->cbus_state.bTxDevCapChgSent = true;
    return true;
}

static bool_t sCbusMscTxSendDcapRdy_Done(void *pObj, CBUS_CALLBACK_PARAMETERS_t *pCbusCallBackParameters)
{
	MhlTxObj_t *mhlObj = (MhlTxObj_t *)pObj;
    //ucData = ucData;
    mhlObj->cbus_state.bTxDevCapRdySent = true;
	sCbusOutQCleanActiveReq(mhlObj);
    SiiCbusSendCommand(mhlObj, API_MSC_INTERRUPT_DCAP_CHG, NULL, 0, sCbusMscTxSendDcapChg_Done);
    return true;
}


/*****************************************************************************/

/**
 * @Function:     sCbusMscReadExtLogDevMap_Done
 *
 * @Description:
 *
 *        This function will be invoked when finishing reading LOG_DEV_MAP in Device Capability Registers.
 *        This is the last register in Extended Device Capabilities.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMscReadExtLogDevMap_Done(MhlTxObj_t *mhlObj, uint8_t ucData)
{
	//Step 1: Save raw data what we read in global variable. and set corresponding state machine.
	mhlObj->cbus_state.ucPeerXDCAP[MHL_DEV_XCAP_LOG_DEV_MAP_REG_OFFSET - MHL_DEV_XCAP_ECBUS_SPEEDS_REG_OFFSET] = ucData;
	sCbusOutQCleanActiveReq(mhlObj);

    //sCbusPrintoutPeerXDevCap(mhlObj);
	sPrintXDevCaps(mhlObj);


    //If ECBUS is NOT established, Send RAP{CBUS_MODE_UP} to request eCBUS.
    if (!mhlObj->cbus_state.bEnableECBUS)
    {
		sCbusSetReadDCAP(mhlObj, true);
		//For TX case, TX sends DCAP_RDY and DCAP_CHG to Rx after finishing reading Device Capability Registers.
		if (!mhlObj->cbus_state.bTxDevCapRdySent)
		{
			SiiCbusSendCommand(mhlObj, API_MSC_STATUS_VERSION_STAT, NULL, 0, NULL);
			SiiCbusSendCommand(mhlObj, API_MSC_STATUS_DCAP_RDY, NULL, 0, sCbusMscTxSendDcapRdy_Done); //Narain : Todo : Implement
		}
    }
    return true;
}

/*****************************************************************************/

/**
 * @Function:     sCbusMscReadEcbusDevRoles_Done
 *
 * @Description:
 *
 *        This function will be invoked when finishing reading ECBUS_DEV_ROLES in Device Capability Registers.
 *        And it continues to read LOG_DEV_MAP.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMscReadEcbusDevRoles_Done(MhlTxObj_t *mhlObj, uint8_t ucData)
{
	
    uint8_t bResult = true;
    cbus_out_queue_t *pReq = sCbusGetOutQReq(mhlObj);

    //Step 1: Save raw data what we read in global variable. and set corresponding state machine.

    mhlObj->cbus_state.ucPeerXDCAP[MHL_DEV_XCAP_ECBUS_DEV_ROLES_REG_OFFSET - MHL_DEV_XCAP_ECBUS_SPEEDS_REG_OFFSET] = ucData;

    //Step 2: Continue to read next XDEV_CAP register.

    pReq->ucOffsetData   = MHL_DEV_XCAP_LOG_DEV_MAP_REG_OFFSET;
    pReq->dataRetHandler = sCbusMscReadExtLogDevMap_Done;
    pReq->sDataRetHandlerName = "sCbusMscReadExtLogDevMap_Done";

    pReq->ucRetry = 1;        // retry 2 times if timeout or abort for important MSC commands

    bResult = sCbusWriteCommand(mhlObj, pReq);
    pReq->ulBase_time = SiI_get_global_time();

    //If the command can't be processed now, suspend it to the later.
    if (false == bResult)
    {
        pReq->ucReqStatus = CBUS_REQ_PENDING;
    }
    else
    {
        pReq->ucReqStatus = CBUS_REQ_WAITING;
    }

    pReq->ulTimeout = DEM_MSC_MAX_REPLY_DELAY;
    return (bResult);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMscReadTmdsSpeeds_Done
 *
 * @Description:
 *
 *        This function will be invoked when finishing reading TMDS_SPEEDS in Device Capability Registers.
 *        And it continues to read ECBUS_DEV_ROLES.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMscReadTmdsSpeeds_Done(MhlTxObj_t *mhlObj, uint8_t ucData)
{
	
    uint8_t bResult = true;
    cbus_out_queue_t *pReq = sCbusGetOutQReq(mhlObj);

    //Step 1: Save raw data what we read in global variable. and set corresponding state machine.

    mhlObj->cbus_state.ucPeerXDCAP[MHL_DEV_XCAP_TMDS_SPEEDS_REG_OFFSET - MHL_DEV_XCAP_ECBUS_SPEEDS_REG_OFFSET] = ucData;

    //Step 2: Continue to read next XDEV_CAP register.
	pReq->ucOffsetData   = MHL_DEV_XCAP_ECBUS_DEV_ROLES_REG_OFFSET;
    pReq->dataRetHandler = sCbusMscReadEcbusDevRoles_Done;
    pReq->sDataRetHandlerName = "sCbusMscReadEcbusDevRoles_Done";

    pReq->ucRetry = 1;        // retry 2 times if timeout or abort for important MSC commands

    bResult = sCbusWriteCommand(mhlObj, pReq);
    pReq->ulBase_time = SiI_get_global_time();

    //If the command can't be processed now, suspend it to the later.
    if (false == bResult)
    {
        pReq->ucReqStatus = CBUS_REQ_PENDING;
    }
    else
    {
        pReq->ucReqStatus = CBUS_REQ_WAITING;
    }

    pReq->ulTimeout = DEM_MSC_MAX_REPLY_DELAY;
    return (bResult);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMscReadEcbusSpeeds_Done
 *
 * @Description:
 *
 *        This function will be invoked when finishing reading INT_STAT_SIZE in Device Capability Registers.
 *        And it continues to read TMDS_SPEEDS.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMscReadEcbusSpeeds_Done(MhlTxObj_t *mhlObj, uint8_t ucData)
{
	
    uint8_t bResult = true;
    cbus_out_queue_t *pReq = sCbusGetOutQReq(mhlObj);

    //Step 1: Save raw data what we read in global variable. and set corresponding state machine.

    mhlObj->cbus_state.ucPeerXDCAP[MHL_DEV_XCAP_ECBUS_SPEEDS_REG_OFFSET - MHL_DEV_XCAP_ECBUS_SPEEDS_REG_OFFSET] = ucData;

    //Step 2: Continue to read next XDEV_CAP register.

    pReq->ucOffsetData   = MHL_DEV_XCAP_TMDS_SPEEDS_REG_OFFSET;
    pReq->dataRetHandler = sCbusMscReadTmdsSpeeds_Done;
    pReq->sDataRetHandlerName = "sCbusMscReadTmdsSpeeds_Done";

    pReq->ucRetry = 1;        // retry 2 times if timeout or abort for important MSC commands

    bResult = sCbusWriteCommand(mhlObj, pReq);
    pReq->ulBase_time = SiI_get_global_time();

    //If the command can't be processed now, suspend it to the later.
    if (false == bResult)
    {
        pReq->ucReqStatus = CBUS_REQ_PENDING;
    }
    else
    {
        pReq->ucReqStatus = CBUS_REQ_WAITING;
    }

    pReq->ulTimeout = DEM_MSC_MAX_REPLY_DELAY;
    return (bResult);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMscReadInt_Stat_Size_Done
 *
 * @Description:
 *
 *        This function will be invoked when finishing reading Device Capability Registers.
 *        INT_STAT_SIZE is the last device capability register.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMscReadInt_Stat_Size_Done(MhlTxObj_t *mhlObj, uint8_t ucData)
{
	
    uint8_t bResult = true;
    cbus_out_queue_t *pReq = sCbusGetOutQReq(mhlObj);

    //Step 1: Save raw data what we read in global variable. and set corresponding state machine.

    mhlObj->cbus_state.ucPeerDCAP[DEVCAP_OFFSET_INT_STAT_SIZE] = ucData;
	sPrintDevCaps(mhlObj);
      //At present, Tx module doesn't support MHL1/2. TX_LINK_MHL means MHL3.
    //if (TX_LINK_MHL == SiiTxGetLinkType())
	if(mhlObj->pConfig->bMhl3En)
	{
		if(mhlObj->cbus_state.bXDevcap_Supp)/* || (mhlObj->cbus_state.ucPeerDCAP[DEVCAP_OFFSET_MHL_VERSION] >= 0x30))*/
		{
			pReq->ucCommand = MHL_READ_XDEVCAP;
			pReq->ucOffsetData   = MHL_DEV_XCAP_ECBUS_SPEEDS_REG_OFFSET;
			pReq->dataRetHandler = sCbusMscReadEcbusSpeeds_Done;
			pReq->sDataRetHandlerName = "sCbusMscReadEcbusSpeeds_Done";

			pReq->ucRetry = 1;    // retry 2 times if timeout or abort for important MSC commands

			bResult = sCbusWriteCommand(mhlObj, pReq);
			pReq->ulBase_time = SiI_get_global_time();

			//If the command can't be processed now, suspend it to the later.
			if (false == bResult)
			{
				pReq->ucReqStatus = CBUS_REQ_PENDING;
			}
			else
			{
				pReq->ucReqStatus = CBUS_REQ_WAITING;
			}

			pReq->ulTimeout = DEM_MSC_MAX_REPLY_DELAY;
		}
		else
		{
			sCbusSetReadDCAP(mhlObj, true);
			sCbusOutQCleanActiveReq(mhlObj);
			//For TX case, TX sends DCAP_RDY and DCAP_CHG to Rx after finishing reading Device Capability Registers.
			if (!mhlObj->cbus_state.bTxDevCapRdySent)
			{
				SiiCbusSendCommand(mhlObj, API_MSC_STATUS_VERSION_STAT, NULL, 0, NULL);
				//SiiCbusSendCommand(mhlObj, API_MSC_STATUS_DCAP_RDY, NULL, 0, sCbusMscTxSendDcapRdy_Done); //Narain : Todo : Implement
			}
		}
	}
	else
	{
		sCbusSetReadDCAP(mhlObj, true);
		sCbusOutQCleanActiveReq(mhlObj);
		//For TX case, TX sends DCAP_RDY and DCAP_CHG to Rx after finishing reading Device Capability Registers.
		if (!mhlObj->cbus_state.bTxDevCapRdySent)
		{
			//SiiCbusSendCommand(mhlObj, API_MSC_STATUS_VERSION_STAT, NULL, 0, NULL);
			SiiCbusSendCommand(mhlObj, API_MSC_STATUS_DCAP_RDY, NULL, 0, sCbusMscTxSendDcapRdy_Done); //Narain : Todo : Implement
		}
	}
    return (bResult);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMscReadScratchpad_Size_Done
 *
 * @Description:
 *
 *        This function will be invoked when finishing reading SCRATCHPAD_SIZE in Device Capability Registers.
 *        And it continues to read INT_STAT_SIZE.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMscReadScratchpad_Size_Done(MhlTxObj_t *mhlObj, uint8_t ucData)
{
	
    uint8_t bResult = true;
    cbus_out_queue_t *pReq = sCbusGetOutQReq(mhlObj);

    //Step 1: Save raw data what we read in global variable. and set corresponding state machine.

    mhlObj->cbus_state.ucPeerDCAP[DEVCAP_OFFSET_SCRATCHPAD_SIZE] = ucData;

    //Step 2: Continue to read next DEV_CAP register.

    pReq->ucOffsetData   = DEVCAP_OFFSET_INT_STAT_SIZE;
    pReq->dataRetHandler = sCbusMscReadInt_Stat_Size_Done;
    pReq->sDataRetHandlerName = "sCbusMscReadInt_Stat_Size_Done";

    pReq->ucRetry = 2;    // retry 2 times if timeout or abort for important MSC commands

    bResult = sCbusWriteCommand(mhlObj, pReq);
    pReq->ulBase_time = SiI_get_global_time();
    if (false == bResult)
    {
        pReq->ucReqStatus = CBUS_REQ_PENDING;
    }
    else
    {
        pReq->ucReqStatus = CBUS_REQ_WAITING;
    }

    pReq->ulTimeout = DEM_MSC_MAX_REPLY_DELAY;

    return (bResult);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMscReadDevice_IDL_Done
 *
 * @Description:
 *
 *        This function will be invoked when finishing reading DEVICE_ID_L in Device Capability Registers.
 *        And it continues to read SCRATCHPAD_SIZE.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMscReadDevice_IDL_Done(MhlTxObj_t *mhlObj, uint8_t ucData)
{
	
    uint8_t bResult = true;
    cbus_out_queue_t *pReq = sCbusGetOutQReq(mhlObj);

    //Step 1: Save raw data what we read in global variable. and set corresponding state machine.

    mhlObj->cbus_state.ucPeerDCAP[DEVCAP_OFFSET_DEVICE_ID_L] = ucData;

    //Step 2: Continue to read next DEV_CAP register.

    pReq->ucOffsetData   = DEVCAP_OFFSET_SCRATCHPAD_SIZE;
    pReq->dataRetHandler = sCbusMscReadScratchpad_Size_Done;
    pReq->sDataRetHandlerName = "sCbusMscReadScratchpad_Size_Done";

    pReq->ucRetry = 2;    // retry 2 times if timeout or abort for important MSC commands

    bResult = sCbusWriteCommand(mhlObj, pReq);
    pReq->ulBase_time = SiI_get_global_time();
    if (false == bResult)
    {
        pReq->ucReqStatus = CBUS_REQ_PENDING;
    }
    else
    {
        pReq->ucReqStatus = CBUS_REQ_WAITING;
    }

    pReq->ulTimeout = DEM_MSC_MAX_REPLY_DELAY;

    return (bResult);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMscReadDevice_IDH_Done
 *
 * @Description:
 *
 *        This function will be invoked when finishing reading DEVICE_ID_H in Device Capability Registers.
 *        And it continues to read DEVICE_ID_L.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMscReadDevice_IDH_Done(MhlTxObj_t *mhlObj, uint8_t ucData)
{
	
    uint8_t bResult = true;
    cbus_out_queue_t *pReq = sCbusGetOutQReq(mhlObj);

    //Step 1: Save raw data what we read in global variable. and set corresponding state machine.

    mhlObj->cbus_state.ucPeerDCAP[DEVCAP_OFFSET_DEVICE_ID_H] = ucData;

    //Step 2: Continue to read next DEV_CAP register.

    pReq->ucOffsetData   = DEVCAP_OFFSET_DEVICE_ID_L;
    pReq->dataRetHandler = sCbusMscReadDevice_IDL_Done;
    pReq->sDataRetHandlerName = "sCbusMscReadDevice_IDL_Done";

    pReq->ucRetry = 2;    // retry 2 times if timeout or abort for important MSC commands

    bResult = sCbusWriteCommand(mhlObj, pReq);
    pReq->ulBase_time = SiI_get_global_time();
    if (false == bResult)
    {
        pReq->ucReqStatus = CBUS_REQ_PENDING;
    }
    else
    {
        pReq->ucReqStatus = CBUS_REQ_WAITING;
    }

    pReq->ulTimeout = DEM_MSC_MAX_REPLY_DELAY;

    return (bResult);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMscReadFeatureFlag_Done
 *
 * @Description:
 *
 *        This function will be invoked when finishing reading FEATURE_FLAG in Device Capability Registers.
 *        And it continues to read DEVICE_ID_H.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMscReadFeatureFlag_Done(MhlTxObj_t *mhlObj, uint8_t ucData)
{
	
    uint8_t bResult = true;
    cbus_out_queue_t *pReq = sCbusGetOutQReq(mhlObj);
    bool_t bRcp_support, bRap_support, bSp_support;

    //Step 1: Save raw data what we read in global variable. and set corresponding state machine.

    mhlObj->cbus_state.ucPeerDCAP[DEVCAP_OFFSET_FEATURE_FLAG] = ucData;

    //DEBUG_PRINT(MSG_ALWAYS, "CBUS:: MHL Source Device Info: Feature: RCP Support: %s\n", (ucData
    //&RCP_SUPPORT) ? "Yes" : "No");

    bRcp_support = (ucData & MHL_FEATURE_RCP_SUPPORT) ? true : false;
    sCbusSetRcpSupport(mhlObj, bRcp_support);

    //DEBUG_PRINT(MSG_ALWAYS, "CBUS:: MHL Source Device Info: Feature: RAP Support: %s\n", (ucData
    //&RAP_SUPPORT) ? "Yes" : "No");

    bRap_support = (ucData & MHL_FEATURE_RAP_SUPPORT) ? true : false;
    sCbusSetRapSupport(mhlObj, bRap_support);

    bSp_support = (ucData & MHL_FEATURE_SP_SUPPORT) ? true : false;
    sCbusSetSpSupport(mhlObj, bSp_support);

    //Step 2: Continue to read next DEV_CAP register.

    pReq->ucOffsetData   = DEVCAP_OFFSET_DEVICE_ID_H;
    pReq->dataRetHandler = sCbusMscReadDevice_IDH_Done;
    pReq->sDataRetHandlerName = "sCbusMscReadDevice_IDH_Done";

    pReq->ucRetry = 2;    // retry 2 times if timeout or abort for important MSC commands

    bResult = sCbusWriteCommand(mhlObj, pReq);
    pReq->ulBase_time = SiI_get_global_time();
    if (false == bResult)
    {
        pReq->ucReqStatus = CBUS_REQ_PENDING;
    }
    else
    {
        pReq->ucReqStatus = CBUS_REQ_WAITING;
    }

    pReq->ulTimeout = DEM_MSC_MAX_REPLY_DELAY;

    return (bResult);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMscReadBandwidth_Done
 *
 * @Description:
 *
 *        This function will be invoked when finishing reading BANDWIDTH in Device Capability Registers.
 *        And it continues to read FEATURE_FLAG.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMscReadBandwidth_Done(MhlTxObj_t *mhlObj, uint8_t ucData)
{
	
    bool_t bResult = true;
    uint32_t frequency;
    cbus_out_queue_t *pReq = sCbusGetOutQReq(mhlObj);

    //Step 1: Save raw data what we read in global variable. and set corresponding state machine.

    mhlObj->cbus_state.ucPeerDCAP[DEVCAP_OFFSET_BANDWIDTH] = ucData;

    if (ucData == 0)
    {
        //DEBUG_PRINT(MSG_ALWAYS, "CBUS:: MHL Source Device Info: No Bandwidth is indicated\n");
    }
    else
    {
        frequency = ucData * 5;

        //DEBUG_PRINT(MSG_ALWAYS, "CBUS:: MHL Source Device Info: Bandwidth: %dMHz\n", (int)frequency);
    }

    //Step 2: Continue to read next DEV_CAP register.

    pReq->ucOffsetData   = DEVCAP_OFFSET_FEATURE_FLAG;
    pReq->dataRetHandler = sCbusMscReadFeatureFlag_Done;
    pReq->sDataRetHandlerName = "sCbusMscReadFeatureFlag_Done";
    pReq->ucRetry = 1;

    bResult = sCbusWriteCommand(mhlObj, pReq);
    pReq->ulBase_time = SiI_get_global_time();
    if (false == bResult)
    {
        pReq->ucReqStatus = CBUS_REQ_PENDING;
    }
    else
    {
        pReq->ucReqStatus = CBUS_REQ_WAITING;
    }

    pReq->ulTimeout = DEM_MSC_MAX_REPLY_DELAY;

    return (bResult);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMscReadLogDevMap_Done
 *
 * @Description:
 *
 *        This function will be invoked when finishing reading LOG_DEV_MAP in Device Capability Registers.
 *        And it continues to read BANDWIDTH.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMscReadLogDevMap_Done(MhlTxObj_t *mhlObj, uint8_t ucData)
{
	
    uint8_t bResult;
    cbus_out_queue_t *pReq = sCbusGetOutQReq(mhlObj);

    //Step 1: Save raw data what we read in global variable. and set corresponding state machine.

    mhlObj->cbus_state.ucPeerDCAP[DEVCAP_OFFSET_LOG_DEV_MAP] = ucData;

    //DEBUG_PRINT(MSG_ALWAYS, "CBUS:: MHL Source Device Info: Log Dev Map: ");

    if (ucData == 0)
    {
        //DEBUG_PRINT(MSG_ALWAYS, "None");
    }
    else
    {
        if (ucData & MHL_DEV_LD_DISPLAY)
        {
            //DEBUG_PRINT(MSG_ALWAYS, "Display ");
        }

        if (ucData & MHL_DEV_LD_VIDEO)
        {
            //DEBUG_PRINT(MSG_ALWAYS, "Video ");
        }

        if (ucData & MHL_DEV_LD_AUDIO)
        {
            //DEBUG_PRINT(MSG_ALWAYS, "Audio ");
        }

        if (ucData & MHL_DEV_LD_MEDIA)
        {
            //DEBUG_PRINT(MSG_ALWAYS, "Media ");
        }

        if (ucData & MHL_DEV_LD_TUNER)
        {
            //DEBUG_PRINT(MSG_ALWAYS, "Tuner ");
        }

        if (ucData & MHL_DEV_LD_RECORD)
        {
            //DEBUG_PRINT(MSG_ALWAYS, "Record ");
        }

        if (ucData & MHL_DEV_LD_SPEAKER)
        {
            //DEBUG_PRINT(MSG_ALWAYS, "Speaker ");
        }

        if (ucData & MHL_DEV_LD_GUI)
        {
            //DEBUG_PRINT(MSG_ALWAYS, "Gui ");
        }
    }

    //DEBUG_PRINT(MSG_ALWAYS, "\n");

    //Step 2: Continue to read next DEV_CAP register.

    pReq->ucOffsetData   = DEVCAP_OFFSET_BANDWIDTH;
    pReq->dataRetHandler = sCbusMscReadBandwidth_Done;
    pReq->sDataRetHandlerName = "sCbusMscReadBandwidth_Done";
    pReq->ucRetry = 1;

    bResult = sCbusWriteCommand(mhlObj, pReq);
    pReq->ulBase_time = SiI_get_global_time();
    if (false == bResult)
    {
        pReq->ucReqStatus = CBUS_REQ_PENDING;
    }
    else
    {
        pReq->ucReqStatus = CBUS_REQ_WAITING;
    }

    pReq->ulTimeout = DEM_MSC_MAX_REPLY_DELAY;

    return (bResult);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMscReadVideoType_Done
 *
 * @Description:
 *
 *        This function will be invoked when finishing reading VIDEO_TYPE in Device Capability Registers.
 *        And it continues to read LOG_DEV_MAP.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMscReadVideoType_Done(MhlTxObj_t *mhlObj, uint8_t ucData)
{
	
    uint8_t bResult;
    cbus_out_queue_t *pReq = sCbusGetOutQReq(mhlObj);

    //Step 1: Save raw data what we read in global variable. and set corresponding state machine.

    mhlObj->cbus_state.ucPeerDCAP[DEVCAP_OFFSET_VIDEO_TYPE] = ucData;

    //DEBUG_PRINT(MSG_ALWAYS, "CBUS:: MHL Source Device Info: Support Video Type: ");

    if (ucData & MHL_SUPP_VT)
    {
        if (ucData & MHL_VT_GRAPHICS)
        {
            //DEBUG_PRINT(MSG_ALWAYS, "Graphics type of video content; ");
        }

        if (ucData & MHL_VT_PHOTO)
        {
            //DEBUG_PRINT(MSG_ALWAYS, "Photo type of video content; ");
        }

        if (ucData & MHL_VT_CINEMA)
        {
            //DEBUG_PRINT(MSG_ALWAYS, "Cinema type of video content; ");
        }

        if (ucData & MHL_VT_GAMES)
        {
            //DEBUG_PRINT(MSG_ALWAYS, "Game type of video content; ");
        }
    }
    else
    {
        //DEBUG_PRINT(MSG_ALWAYS, "None");
    }

    //DEBUG_PRINT(MSG_ALWAYS, "\n");

    //Step 2: Continue to read next DEV_CAP register.

    pReq->ucOffsetData   = DEVCAP_OFFSET_LOG_DEV_MAP;
    pReq->dataRetHandler = sCbusMscReadLogDevMap_Done;
    pReq->sDataRetHandlerName = "sCbusMscReadLogDevMap_Done";
    pReq->ucRetry = 1;

    bResult = sCbusWriteCommand(mhlObj, pReq);
    pReq->ulBase_time = SiI_get_global_time();
    if (false == bResult)
    {
        pReq->ucReqStatus = CBUS_REQ_PENDING;
    }
    else
    {
        pReq->ucReqStatus = CBUS_REQ_WAITING;
    }

    pReq->ulTimeout = DEM_MSC_MAX_REPLY_DELAY;

    return (bResult);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMscReadAudioLinkMode_Done
 *
 * @Description:
 *
 *        This function will be invoked when finishing reading AUD_LINK_MODE in Device Capability Registers.
 *        And it continues to read VIDEO_TYPE.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMscReadAudioLinkMode_Done(MhlTxObj_t *mhlObj, uint8_t ucData)
{
	
    uint8_t bResult;
    cbus_out_queue_t *pReq = sCbusGetOutQReq(mhlObj);

    //Step 1: Save raw data what we read in global variable. and set corresponding state machine.

    mhlObj->cbus_state.ucPeerDCAP[DEVCAP_OFFSET_AUD_LINK_MODE] = ucData;

    //DEBUG_PRINT(MSG_ALWAYS, "CBUS:: MHL Source Device Info: Audio Link Mode: ");

    if (ucData == 0)
    {
        //DEBUG_PRINT(MSG_ALWAYS, "None");
    }
    else
    {
        if (ucData & MHL_DEV_AUD_LINK_2CH)
        {
            //DEBUG_PRINT(MSG_ALWAYS, "2-Channel PCM audio streams (with Layout 0 audio data packets) ");
        }

        if (ucData & MHL_DEV_AUD_LINK_8CH)
        {
            //DEBUG_PRINT(MSG_ALWAYS, "8-channel PCM audio streams (with Layout 1 audio data packets) ");
        }
    }

    //DEBUG_PRINT(MSG_ALWAYS, ("\n"));

    //Step 2: Continue to read next DEV_CAP register.

    pReq->ucOffsetData   = DEVCAP_OFFSET_VIDEO_TYPE;
    pReq->dataRetHandler = sCbusMscReadVideoType_Done;
    pReq->sDataRetHandlerName = "sCbusMscReadVideoType_Done";
    pReq->ucRetry = 1;

    bResult = sCbusWriteCommand(mhlObj, pReq);
    pReq->ulBase_time = SiI_get_global_time();
    if (false == bResult)
    {
        pReq->ucReqStatus = CBUS_REQ_PENDING;
    }
    else
    {
        pReq->ucReqStatus = CBUS_REQ_WAITING;
    }

    pReq->ulTimeout = DEM_MSC_MAX_REPLY_DELAY;

    return (bResult);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMscReadVideoLinkMode_Done
 *
 * @Description:
 *
 *        This function will be invoked when finishing reading VID_LINK_MODE in Device Capability Registers.
 *        And it continues to read AUD_LINK_MODE.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/

static bool_t sCbusMscReadVideoLinkMode_Done(MhlTxObj_t *mhlObj, uint8_t ucData)
{
	
    uint8_t bResult;
    cbus_out_queue_t *pReq = sCbusGetOutQReq(mhlObj);

    //Step 1: Save raw data what we read in global variable. and set corresponding state machine.

    mhlObj->cbus_state.ucPeerDCAP[DEVCAP_OFFSET_VID_LINK_MODE] = ucData;

    //DEBUG_PRINT(MSG_ALWAYS, "CBUS:: MHL Source Device Info: Video Link Mode: ");

    if (ucData == 0)
    {
        //DEBUG_PRINT(MSG_ALWAYS, "None");
    }
    else
    {
        if (ucData & MHL_DEV_VID_LINK_SUPP_RGB444)
        {
            //DEBUG_PRINT(MSG_ALWAYS, "RGB 4:4:4 ");
        }

        if (ucData & MHL_DEV_VID_LINK_SUPP_YCBCR444)
        {
            //DEBUG_PRINT(MSG_ALWAYS, "YCBCR 4:4:4 ");
        }

        if (ucData & MHL_DEV_VID_LINK_SUPP_YCBCR422)
        {
            //DEBUG_PRINT(MSG_ALWAYS, "YCBCR 4:2:2 ");
        }

        if (ucData & MHL_DEV_VID_LINK_SUPP_PPIXEL)
        {
            //DEBUG_PRINT(MSG_ALWAYS, "Packed Pixel ");
        }

        if (ucData & MHL_DEV_VID_LINK_SUPP_ISLANDS)
        {
            //DEBUG_PRINT(MSG_ALWAYS, "Data Islands ");
        }
    }

    //DEBUG_PRINT(MSG_ALWAYS, "\n");

    //Step 2: Continue to read next DEV_CAP register.

    pReq->ucOffsetData   = DEVCAP_OFFSET_AUD_LINK_MODE;
    pReq->dataRetHandler = sCbusMscReadAudioLinkMode_Done;
    pReq->sDataRetHandlerName = "sCbusMscReadAudioLinkMode_Done";
    pReq->ucRetry = 1;

    bResult = sCbusWriteCommand(mhlObj, pReq);
    pReq->ulBase_time = SiI_get_global_time();
    if (false == bResult)
    {
        pReq->ucReqStatus = CBUS_REQ_PENDING;
    }
    else
    {
        pReq->ucReqStatus = CBUS_REQ_WAITING;
    }

    pReq->ulTimeout = DEM_MSC_MAX_REPLY_DELAY;

    return (bResult);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMscReadAdoperIDL_Done
 *
 * @Description:
 *
 *        This function will be invoked when finishing reading ADOPTER_ID_L in Device Capability Registers.
 *        And it continues to read VID_LINK_MODE.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/

static bool_t sCbusMscReadAdoperIDL_Done(MhlTxObj_t *mhlObj, uint8_t ucData)
{
	
    uint8_t bResult;
    cbus_out_queue_t *pReq = sCbusGetOutQReq(mhlObj);

    //Step 1: Save raw data what we read in global variable. and set corresponding state machine.

    mhlObj->cbus_state.ucPeerDCAP[DEVCAP_OFFSET_ADOPTER_ID_L] = ucData;

    //Step 2: Continue to read next DEV_CAP register.

    pReq->ucOffsetData   = DEVCAP_OFFSET_VID_LINK_MODE;
    pReq->dataRetHandler = sCbusMscReadVideoLinkMode_Done;
    pReq->sDataRetHandlerName = "sCbusMscReadVideoLinkMode_Done";
    pReq->ucRetry = 1;

    bResult = sCbusWriteCommand(mhlObj, pReq);
    pReq->ulBase_time = SiI_get_global_time();
    if (false == bResult)
    {
        pReq->ucReqStatus = CBUS_REQ_PENDING;
    }
    else
    {
        pReq->ucReqStatus = CBUS_REQ_WAITING;
    }

    pReq->ulTimeout = DEM_MSC_MAX_REPLY_DELAY;

    return (bResult);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMscReadAdoperIDH_Done
 *
 * @Description:
 *
 *        This function will be invoked when finishing reading ADOPTER_ID_H in Device Capability Registers.
 *        And it continues to read ADOPTER_ID_L.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/

static bool_t sCbusMscReadAdoperIDH_Done(MhlTxObj_t *mhlObj, uint8_t ucData)
{
	
    uint8_t bResult;
    cbus_out_queue_t *pReq = sCbusGetOutQReq(mhlObj);

    //Step 1: Save raw data what we read in global variable. and set corresponding state machine.

    mhlObj->cbus_state.ucPeerDCAP[DEVCAP_OFFSET_ADOPTER_ID_H] = ucData;

    //Step 2: Continue to read next DEV_CAP register.

    pReq->ucOffsetData   = DEVCAP_OFFSET_ADOPTER_ID_L;
    pReq->dataRetHandler = sCbusMscReadAdoperIDL_Done;
    pReq->sDataRetHandlerName = "sCbusMscReadAdoperIDL_Done";
    pReq->ucRetry = 1;

    bResult = sCbusWriteCommand(mhlObj, pReq);
    pReq->ulBase_time = SiI_get_global_time();
    if (false == bResult)
    {
        pReq->ucReqStatus = CBUS_REQ_PENDING;
    }
    else
    {
        pReq->ucReqStatus = CBUS_REQ_WAITING;
    }

    pReq->ulTimeout = DEM_MSC_MAX_REPLY_DELAY;

    return (true);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMscReadDevCat_Done
 *
 * @Description:
 *
 *        This function will be invoked when finishing reading DEV_CAT in Device Capability Registers.
 *        And it continues to read ADOPTER_ID_H.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMscReadDevCat_Done(MhlTxObj_t *mhlObj, uint8_t ucData)
{
	
    uint8_t bResult;
    cbus_out_queue_t *pReq = sCbusGetOutQReq(mhlObj);

    //Step 1: Save raw data what we read in global variable. and set corresponding state machine.

    mhlObj->cbus_state.ucPeerDCAP[DEVCAP_OFFSET_DEV_CAT] = ucData;

    switch (ucData)
    {
    case MHL_DEV_CAT_SOURCE:
        //DEBUG_PRINT(MSG_ALWAYS, "CBUS:: MHL Source Device Info: Category: Source Device\n");
        break;
	case OTHER_DEV_CAT:
		//DEBUG_PRINT(MSG_ALWAYS, "CBUS:: MHL Source Device Info: Category: Other Device\n");
		break;
    default:
        //DEBUG_PRINT(MSG_ALWAYS, "CBUS:: MHL Source Device Info: Category: Incorrect\n");
        break;
    }

    //Step 2: Continue to read next DEV_CAP register.

    pReq->ucOffsetData   = DEVCAP_OFFSET_ADOPTER_ID_H;
    pReq->dataRetHandler = sCbusMscReadAdoperIDH_Done;
    pReq->sDataRetHandlerName = "sCbusMscReadAdoperIDH_Done";
    pReq->ucRetry = 1;

    bResult = sCbusWriteCommand(mhlObj, pReq);
    pReq->ulBase_time = SiI_get_global_time();
    if (false == bResult)
    {
        pReq->ucReqStatus = CBUS_REQ_PENDING;
    }
    else
    {
        pReq->ucReqStatus = CBUS_REQ_WAITING;
    }

    pReq->ulTimeout = DEM_MSC_MAX_REPLY_DELAY;

    return (bResult);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMscReadMhlVersion_Done
 *
 * @Description:
 *
 *        This function will be invoked when finishing reading MHL_VERSION in Device Capability Registers.
 *        And it continues to read DEV_CAT.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMscReadMhlVersion_Done(MhlTxObj_t *mhlObj, uint8_t ucData)
{
	
    uint8_t bResult;
    cbus_out_queue_t *pReq = sCbusGetOutQReq(mhlObj);

    //Step 1: Save raw data what we read in global variable. and set corresponding state machine.

    mhlObj->cbus_state.ucPeerDCAP[DEVCAP_OFFSET_MHL_VERSION] = ucData;

    //Step 2: Continue to read next DEV_CAP register.

    pReq->ucOffsetData   = DEVCAP_OFFSET_DEV_CAT;
    pReq->dataRetHandler = sCbusMscReadDevCat_Done;
    pReq->sDataRetHandlerName = "sCbusMscReadDevCat_Done";
    pReq->ucRetry = 1;

    bResult = sCbusWriteCommand(mhlObj, pReq);
    pReq->ulBase_time = SiI_get_global_time();
    if (false == bResult)
    {
        pReq->ucReqStatus = CBUS_REQ_PENDING;
    }
    else
    {
        pReq->ucReqStatus = CBUS_REQ_WAITING;
    }

    pReq->ulTimeout = DEM_MSC_MAX_REPLY_DELAY;

    return (bResult);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMscReadDevice_IDH_Done
 *
 * @Description:
 *
 *        This function will be invoked when finishing reading DEV_STATE in Device Capability Registers.
 *        And it continues to read MHL_VERSION.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMscReadDevState_Done(MhlTxObj_t *mhlObj, uint8_t ucData)
{
	
    uint8_t bResult;
    cbus_out_queue_t *pReq = sCbusGetOutQReq(mhlObj);

    //Step 1: Save raw data what we read in global variable. and set corresponding state machine.

    mhlObj->cbus_state.ucPeerDCAP[DEVCAP_OFFSET_DEV_STATE] = ucData;

    //Step 2: Continue to read next DEV_CAP register.

    pReq->ucOffsetData   = DEVCAP_OFFSET_MHL_VERSION;
    pReq->dataRetHandler = sCbusMscReadMhlVersion_Done;
    pReq->sDataRetHandlerName = "sCbusMscReadMhlVersion_Done";
    pReq->ucRetry = 1;

    bResult = sCbusWriteCommand(mhlObj, pReq);
    pReq->ulBase_time = SiI_get_global_time();
    if (false == bResult)
    {
        pReq->ucReqStatus = CBUS_REQ_PENDING;
    }
    else
    {
        pReq->ucReqStatus = CBUS_REQ_WAITING;
    }

    pReq->ulTimeout = DEM_MSC_MAX_REPLY_DELAY;

    return (bResult);
}

/*****************************************************************************/

/**
 * @Function:     sCbusMscStartGetDevInfo
 *
 * @Description:
 *
 *        This function begins to read Device Capability Registers.
 *
 * @Return: bool_t
 *
 *        true:  success
 *        false: failed
 *
 *****************************************************************************/
static bool_t sCbusMscStartGetDevInfo(MhlTxObj_t *mhlObj)
{
    bool_t bResult;
    cbus_out_queue_t *pReq = sCbusGetOutQReq(mhlObj);

    if (true == sCbusGetAbortWait(mhlObj))
    {
        SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: CBUS is unavailable in Abort Waiting.\n"));
        return false;
    }

    if (pReq->ucReqStatus == CBUS_REQ_IDLE)
    {
        pReq->ucCommand = MHL_READ_DEVCAP;
        pReq->ucOffsetData   = DEVCAP_OFFSET_DEV_STATE;
        pReq->dataRetHandler = sCbusMscReadDevState_Done;
        pReq->sDataRetHandlerName = "sCbusMscReadDevState_Done";

        pReq->ucRetry = 1;

        bResult = sCbusWriteCommand(mhlObj, pReq);
        pReq->ulBase_time = SiI_get_global_time();
        pReq->ucReqStatus = CBUS_REQ_WAITING;
        pReq->ulTimeout = DEM_MSC_MAX_REPLY_DELAY;
    }
    else
    {
        bResult = false;
    }

    return bResult;
}

/*****************************************************************************/
/**
 * @Function:		sCbusInitDevCap
 *
 * @Description:	Initialize device capability registers.
 *
 * @Return: bool_t	true:  success
 *					false: failed
 *
 *****************************************************************************/
static bool_t sCbusInitDevCap(MhlTxObj_t *mhlObj)
{
	SiiDrvCraAddr_t	baseAddr = sBaseAddrGet(mhlObj);
	SiiInst_t		craInst  = sCraInstGet(mhlObj);

	SiiDrvCraBlockWrite8(craInst, baseAddr | REG_ADDR__MHL_DEVCAP_0, gMhl_DeviceCapability, sizeof(gMhl_DeviceCapability));
    return true;
}

/*****************************************************************************/
/**
 * @Function:		sCbusInitXDevCap
 *
 * @Description:	Initialize device capability registers.
 *
 * @Return: bool_t	true:  success
 *					false: failed
 *
 *****************************************************************************/
static bool_t sCbusInitXDevCap(MhlTxObj_t *mhlObj)
{
	SiiDrvCraAddr_t	baseAddr = sBaseAddrGet(mhlObj);
	SiiInst_t		craInst  = sCraInstGet(mhlObj);

	SiiDrvCraBlockWrite8(craInst, baseAddr | REG_ADDR__MHL_EXTDEVCAP_0, gMhl_XDeviceCapability, sizeof(gMhl_XDeviceCapability));
    return true;
}

static bool_t sCbusInitParam(MhlTxObj_t *mhlObj)
{
	CbusState_t *cbusState = &mhlObj->cbus_state;
	memset(cbusState, 0x00, sizeof(CbusState_t));

    cbusState->ucVideoEntryIndex = MAX_VIDEO_ENTRY;     //do not sent out writeburst at startup
    return true;
}

/*****************************************************************************/
/**
 * @Function:     sCbusInitialize
 *
 * @Description:	Attempts to intialize the CBUS. If register reads return 0xFF,
 *					it declares error in initialization.
 *					Initializes discovery enabling registers and anything needed in
 *					config register, interrupt masks. *
 *
 * @Return: bool_t	true:  success
 *					false: failed
 *
 *****************************************************************************/
static bool_t sCbusInitialize (MhlTxObj_t *mhlObj)
{
	SiiDrvCraAddr_t	baseAddr = sBaseAddrGet(mhlObj);
	SiiInst_t		craInst  = sCraInstGet(mhlObj);

	memset( &mhlObj->cbus_state, 0, sizeof(CbusState_t));

    //Step 1: Initialize Device Capability.
    sCbusInitDevCap(mhlObj);

    //Step 2: Initialize Extended Device Capability.
    sCbusInitXDevCap(mhlObj);

	sCbusInQCleanActiveReq(mhlObj);
    sCbusOutQCleanActiveReq(mhlObj);

	return true;
}

static uint8_t * sCbusGetPointer_PeerDeviceCapabiliy_RawData(MhlTxObj_t *mhlObj)
{
    return mhlObj->cbus_state.ucPeerDCAP;
}

/*****************************************************************************/

/**
 *  Cbus data structure operation: Set the value of clk_mode
 *
 *
 *  @param[in]          value			value of clk_mode
 *
 *  @return             void
 *
 *****************************************************************************/
static bool_t sCbusSetClkMode(MhlTxObj_t *mhlObj, uint8_t value)
{
	SiiDrvCraAddr_t	baseAddr = sBaseAddrGet(mhlObj);
	SiiInst_t		craInst  = sCraInstGet(mhlObj);
    mhlObj->cbus_state.ucClkMode = value;
	
    if (value == CLK_MODE_NORMAL)
    {
		//SiiDrvCraWrReg8(craInst, baseAddr | REG_DPLL_MISC2, 0x1D); //Todo: Update Later
    }
    else if (value == CLK_MODE_PACKED_PIXEL)
    {
        //SiiDrvCraWrReg8(craInst, baseAddr | REG_DPLL_MISC2, 0x1E); //Todo: Update Later
    }
    else
    {
        return false;
    }

	return true;
}

/*****************************************************************************/

/**
 *  Cbus data structure operation: Get the value of clk_mode
 *
 *
 *  @param[in]          value			value of clk_mode
 *
 *  @return             void
 *
 *****************************************************************************/
static uint8_t sCbusGetClkMode(MhlTxObj_t *mhlObj)
{
    return mhlObj->cbus_state.ucClkMode;
}

static bool_t sCbusVerifyCheckSum(MhlTxObj_t *mhlObj, uint8_t *pBurstData,uint8_t ucLength)
{
    uint8_t ucCheckSum = 0;
    uint8_t i;

    for (i = 0; i < ucLength; i++)
    {
        ucCheckSum += (uint8_t)(*(uint8_t *)(pBurstData+i));
    }

    if ( ucCheckSum )
    {
        return false;
    }

    return true;
}

static bool_t sCbusWriteBurstQueue(MhlTxObj_t *mhlObj, WRITEBURST_CIRCULARBUFFER_t * pBurstQueue, uint8_t * pWriteBurst)
{
    //Step 1: Check if buffer available.
    if ((pBurstQueue->ucWritePointer + 1) % MAX_WRITE_BURST == pBurstQueue->ucReadPointer)
    {
        SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: WriteBurst Buffer Full.\n"));
        return false;
    }

    //Step 2: Verify BURST's checksum correct
    if (false == sCbusVerifyCheckSum(mhlObj, pWriteBurst, MHL_SCRATCHPAD_SIZE))
    {
        SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: WriteBurst ChecSum Fail. \n"));
       //true means that current burst has been handled correctly.		
        return true;
    }

    //Step 3: Copy BURST data into Queue buffer.
    memcpy(pBurstQueue->ucGeneralWriteBurst[pBurstQueue->ucWritePointer], pWriteBurst, 16);

    //Step 4: Move writing pointer to next
    pBurstQueue->ucWritePointer = (pBurstQueue->ucWritePointer + 1) % MAX_WRITE_BURST;

    return true;
}

/*****************************************************************************/

/**
 * @Function:     sCbusGetVideoEntryIndex
 *
 * @Description:
 *
 *        Return the video entry index in the 3D video format table.
 *
 * @Return: uint8_t
 *
 *
 *****************************************************************************/
static uint8_t sCbusGetVideoEntryIndex(MhlTxObj_t *mhlObj)
{
    return (mhlObj->cbus_state.ucVideoEntryIndex);
}

static bool_t sCbusSetVideoEntryIndex(MhlTxObj_t *mhlObj, uint8_t bValue)
{
    mhlObj->cbus_state.ucVideoEntryIndex = bValue;
    return true;
}

static bool_t sCbusSetBurstToSend(MhlTxObj_t *mhlObj, bool_t bValue)
{
    mhlObj->cbus_state.bBurstToSend = bValue;
    return true;
}

static bool_t sCbusGetBurstToSend(MhlTxObj_t *mhlObj)
{
    return (mhlObj->cbus_state.bBurstToSend);
}

static uint8_t* sCbusGetGeneralBurst(MhlTxObj_t *mhlObj)
{
    return (mhlObj->cbus_state.ucGeneralWriteBurst);
}

static MhlVideoFormatData_t* sCbusGetVideoFormatData(MhlTxObj_t *mhlObj)
{
    return mhlObj->cbus_state.videoFormatData;
}

static bool_t sCbusSetBurst(MhlTxObj_t *mhlObj, uint8_t *pCbusBurst)
{
    memcpy(mhlObj->cbus_state.ucGeneralWriteBurst, pCbusBurst, MHL_SCRATCHPAD_SIZE);
    sCbusSetBurstToSend(mhlObj, true);
    return true;
}

static bool_t sCbusPrintOutWriteBurst(MhlTxObj_t *mhlObj, uint8_t * pBurstData)
{
    uint8_t i = 0;

    pBurstData = pBurstData;
    SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: "));
   for (i = 0; i < 8; i++)
    {
        SII_LIB_LOG_DEBUG1(mhlObj, ("[%02x] ", (int)(*(pBurstData + i))));
    }

    SII_LIB_LOG_DEBUG1(mhlObj, ("\n"));
    SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: "));
    for (i = 8; i < 16; i++)
    {
        SII_LIB_LOG_DEBUG1(mhlObj, ("[%02x] ", (int)(*(pBurstData + i))));
    }

    SII_LIB_LOG_DEBUG1(mhlObj, ("\n"));
    return true;
}

/*****************************************************************************/

/**
 * @Function:     sCbusVcConfirmBuildBurstData
 *
 * @Description :
 *
 *        Build VC_CONFRIM WRITE_BURST data.
 *
 * @Return : bool_t
 *
 *        true:  current burst has been handled correctly. 
 *        false: need retry.
 *
 *****************************************************************************/
static bool_t sCbusVcConfirmBuildBurstData(MhlTxObj_t *mhlObj, MhlVcAssignFormatData_t *pVcAssignWriteBurstData )
{
    MhlVcConfirmFormatData_t VcConfirmWriteBurstData;
    uint8_t i, ucChecksum = 0;

    //Step 1: Copy VC_ASSIGN
    memcpy(&VcConfirmWriteBurstData, pVcAssignWriteBurstData, MHL_SCRATCHPAD_SIZE);

    //Step 2: Modify response sign.
    if (VcConfirmWriteBurstData.ucNumEntriesThisBurst > 3)
    {
        SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Invalid NUM_ENT value.\n"));

        //true means that current burst has been handled correctly.
        return true;
    }

    for (i = 0; i < VcConfirmWriteBurstData.ucNumEntriesThisBurst; i++)
    {
        VcConfirmWriteBurstData.virtualChannelDescriptors[i].ucCHANNEL_SIZE_RESPONSE = RESPONSE_ACCEPT;
    }

    //Step 3: Change Burst_ID to VC_CONFIRM
    VcConfirmWriteBurstData.burstId.high = (uint8_t)(VC_CONFIRM >> 8);
    VcConfirmWriteBurstData.burstId.low = (uint8_t)(VC_CONFIRM);

    //Step 4: Re-calculate check sum.
    VcConfirmWriteBurstData.ucCheckSum = 0;
    ucChecksum = 0;
    for (i = 0; i < sizeof(MhlVideoFormatData_t); i++)
    {
        ucChecksum += *((uint8_t *)((uint8_t *)(&VcConfirmWriteBurstData) + i));
    }

    VcConfirmWriteBurstData.ucCheckSum = (0 - ucChecksum);

    //Step 5: Push into Out-Queue.

    return sCbusWriteBurstQueue(mhlObj, &(mhlObj->cbus_state.WriteBurstOutQueue), (uint8_t * )(&VcConfirmWriteBurstData));
	return true;
}

static bool_t sCbusOutBurstHandler(MhlTxObj_t *mhlObj)
{
    bool_t bResult = true;

	//Step 1: Make sure peer device support Scraptch Pad.
    if (false == sCbusGetSpSupport(mhlObj))
    {
        //Peer device doesn't support scratch pad.
        return false;
    }

    //Step 2: Make sure idle state.
    if (true == mhlObj->cbus_state.WriteBurstOutQueue.bBusy)
    {
        //Busy in sending WRITE_BURST.
        return false;
    }

    //Step 3: Check if any BURST waiting to send.
    if (mhlObj->cbus_state.WriteBurstOutQueue.ucReadPointer == mhlObj->cbus_state.WriteBurstOutQueue.ucWritePointer)
    {
        //There is no WRITE BURST in buffer.
        return false;
    }

    //Step 4: If BURST is present in buffer, send.
    bResult =
        sCbusSET_INT_REQ_WRT(mhlObj, mhlObj->cbus_state.WriteBurstOutQueue.ucGeneralWriteBurst[mhlObj->cbus_state.WriteBurstOutQueue.ucReadPointer],
                               sizeof(MhlVideoFormatData_t), NULL);
    if (bResult)
    {
        mhlObj->cbus_state.WriteBurstOutQueue.bBusy = true;
        mhlObj->cbus_state.WriteBurstOutQueue.ucReadPointer = (mhlObj->cbus_state.WriteBurstOutQueue.ucReadPointer + 1) % MAX_WRITE_BURST;
    }

    return bResult;
}

static bool_t sCbusInBurstHandler(MhlTxObj_t *mhlObj)
{
    bool_t bResult = true;

    //Step 1: Make sure peer device support Scraptch Pad.
    if (false == sCbusGetSpSupport(mhlObj))
    {
        //Peer device doesn't support scratch pad.
        return false;
    }

    //Step 2: Make sure idle state.
    if (true == mhlObj->cbus_state.WriteBurstInQueue.bBusy)
    {
        //Busy in sending WRITE_BURST.
        return false;
    }

    //Step 3: Check if any BURST waiting to handle.
    if (mhlObj->cbus_state.WriteBurstInQueue.ucReadPointer == mhlObj->cbus_state.WriteBurstInQueue.ucWritePointer)
    {
        //There is no WRITE BURST in buffer.
        return false;
    }

    //Step 4: If BURST is present in buffer, handle.
    {
        uint8_t * pWriteBurstData =
            mhlObj->cbus_state.WriteBurstInQueue.ucGeneralWriteBurst[mhlObj->cbus_state.WriteBurstInQueue.ucReadPointer];
        SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: Got WRITE BURST \n"));
        sCbusPrintOutWriteBurst(mhlObj, pWriteBurstData);
        switch (*((uint16_t *)(pWriteBurstData)))
        {
        case BURST_3D_VIC:
            SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: 3D_VIC \n"));
            break;
        case BURST_3D_DTD:
            SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: 3D_VIC \n"));
            break;
        case HEV_VIC:
            SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: HEV_VIC \n"));
            break;
        case HEV_DTDA:
            SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: HEV_DTDA \n"));
            break;
        case HEV_DTDB:
            SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: HEV_DTDB \n"));
            break;
        case VC_ASSIGN:
            SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: VC_ASSIGN \n"));
            bResult = sCbusVcConfirmBuildBurstData(mhlObj, (MhlVcAssignFormatData_t*)pWriteBurstData);
            break;
        case VC_CONFIRM:
            SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: VC_CONFIRM \n"));
            break;
        case AUD_DELAY:
            SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: AUD_DELAY \n"));
            break;
        case ADT_BURSTID:
            SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: ADT_BURSTID \n"));
            break;
        case BIST_SETUP:
            SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: BIST_SETUP \n"));
            // bResult = sCbusMSC_MSG_BIST_READY(BIST_READY_ECBUS_READY|BIST_READY_AVLINK_READY, NULL);
            break;
        case BIST_RETURN_STAT:
            SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: BIST_RETURN_STAT \n"));
            break;
        case EMSC_SUPPORT:
            SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: EMSC_SUPPORT \n"));
            break;
        case HID_PAYLOAD:
            SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: HID_PAYLOAD \n"));
            break;
        default:
            SII_LIB_LOG_DEBUG1(mhlObj, ("CBUS:: ERROR! Unknown Burst ID [0x%02x] [0x%02x].\n", (int)(*pWriteBurstData),
                                  (int)(*((uint8_t *)pWriteBurstData + 1))));
        }

        if (bResult)
        {
            mhlObj->cbus_state.WriteBurstInQueue.ucReadPointer = (mhlObj->cbus_state.WriteBurstInQueue.ucReadPointer + 1) % MAX_WRITE_BURST;
        }
    }

    return bResult;
}

static bool_t sCbusBurstHandler(MhlTxObj_t *mhlObj)
{
    //Step 1: Process Burst in Burst-Out-Queue.
    sCbusOutBurstHandler(mhlObj);

    //Step 2: Process Burst in Burst-In-Queue.
    sCbusInBurstHandler(mhlObj);

    return true;
}

/*****************************************************************************/
/**
 * @Function:     sCbusAvMute
 *
 * @Description:	Mute AV.
 *
 * @Return: bool_t	true:  success
 *					false: failed
 *
 *****************************************************************************/
static bool_t sCbusAvMute(MhlTxObj_t *mhlObj, bool_t bMuteState)
{
    //SiiTxSetAVMute( bMuteState, MUTE_CBUS);
    return true;
}

static bool_t sCbusWRITE_STAT_Version_Stat_Done(MhlTxObj_t *mhlObj, uint8_t ucData)
{
	
	ucData = ucData;

	sCbusOutQCleanActiveReq(mhlObj);
	SiiCbusSendCommand(mhlObj, API_MSC_STATUS_DCAP_RDY, NULL, 0, sCbusMscTxSendDcapRdy_Done);
	return true;
}

static int sCbusInit(MhlTxObj_t *mhlObj)
{
	SiiDrvCraAddr_t	baseAddr = sBaseAddrGet(mhlObj);
	SiiInst_t		craInst  = sCraInstGet(mhlObj);
	bool_t bResult;
	bResult = sCbusInitialize(mhlObj);
	bResult = sCbusInitParam(mhlObj);
	
	//Initialize RGND Impedence Detection
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DISC_CTRL1		, 0x25);
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DISC_CTRL4		, 0x80);
	//Clear Cbus_Disc_Intr status(if any) and enable its mask.
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__CBUS_DISC_INTR0	, 0x6C);
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__CBUS_DISC_INTR0_MASK, 0x6C);
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__INTR5_MASK		, 0x00); //Mask Intr5.

	return bResult;
}

static void sReadDevCaps(MhlTxObj_t *mhlObj)
{
	memset(&mhlObj->peerDevCaps, 0, sizeof(SiiDrvTxMHLDevCap_t));
	mhlObj->peerDevCaps.state				= mhlObj->cbus_state.ucPeerDCAP[0];
	mhlObj->peerDevCaps.mhl_version			= mhlObj->cbus_state.ucPeerDCAP[1]; 
	mhlObj->peerDevCaps.deviceCategory		= mhlObj->cbus_state.ucPeerDCAP[2];
	mhlObj->peerDevCaps.adopterIdHigh		= mhlObj->cbus_state.ucPeerDCAP[3];
	mhlObj->peerDevCaps.adopterIdLow		= mhlObj->cbus_state.ucPeerDCAP[4];
	mhlObj->peerDevCaps.vid_link_mode		= mhlObj->cbus_state.ucPeerDCAP[5];
	mhlObj->peerDevCaps.audLinkMode			= mhlObj->cbus_state.ucPeerDCAP[6];
	mhlObj->peerDevCaps.videoType			= mhlObj->cbus_state.ucPeerDCAP[7];
	mhlObj->peerDevCaps.logicalDeviceMap	= mhlObj->cbus_state.ucPeerDCAP[8];
	mhlObj->peerDevCaps.bandWidth			= mhlObj->cbus_state.ucPeerDCAP[9];
	mhlObj->peerDevCaps.featureFlag			= mhlObj->cbus_state.ucPeerDCAP[10];
	mhlObj->peerDevCaps.deviceIdHigh		= mhlObj->cbus_state.ucPeerDCAP[11];
	mhlObj->peerDevCaps.deviceIdLow			= mhlObj->cbus_state.ucPeerDCAP[12]; 
	mhlObj->peerDevCaps.scratchPadSize		= mhlObj->cbus_state.ucPeerDCAP[13];
	mhlObj->peerDevCaps.int_state_size		= mhlObj->cbus_state.ucPeerDCAP[14];
	mhlObj->peerDevCaps.reserved			= mhlObj->cbus_state.ucPeerDCAP[15];
}

static void sReadExtDevCaps(MhlTxObj_t *mhlObj)
{
	memset(&mhlObj->peerXDevCaps, 0, sizeof(SiiDrvTxMHLXDevCap_t));
	mhlObj->sXDevCaps.ecbus_speeds		= mhlObj->cbus_state.ucPeerXDCAP[0];
	mhlObj->sXDevCaps.tmds_speeds		= mhlObj->cbus_state.ucPeerXDCAP[1];
	mhlObj->sXDevCaps.ecbus_dev_roles	= mhlObj->cbus_state.ucPeerXDCAP[2];
	mhlObj->sXDevCaps.log_dev_mapx		= mhlObj->cbus_state.ucPeerXDCAP[3];
}

static void setDevCaps(MhlTxObj_t *mhlObj)
{
	memset(&gMhl_DeviceCapability, 0, sizeof(gMhl_DeviceCapability));
	gMhl_DeviceCapability[DEVCAP_OFFSET_DEV_STATE]		= mhlObj->sDevCaps.state;
	gMhl_DeviceCapability[DEVCAP_OFFSET_MHL_VERSION]	= mhlObj->sDevCaps.mhl_version;	
	gMhl_DeviceCapability[DEVCAP_OFFSET_DEV_CAT]		= mhlObj->sDevCaps.deviceCategory;
	gMhl_DeviceCapability[DEVCAP_OFFSET_ADOPTER_ID_H]	= mhlObj->sDevCaps.adopterIdHigh;
	gMhl_DeviceCapability[DEVCAP_OFFSET_ADOPTER_ID_L]	= mhlObj->sDevCaps.adopterIdLow;
	gMhl_DeviceCapability[DEVCAP_OFFSET_VID_LINK_MODE]  = mhlObj->sDevCaps.vid_link_mode;	
	gMhl_DeviceCapability[DEVCAP_OFFSET_AUD_LINK_MODE]  = mhlObj->sDevCaps.audLinkMode;	
	gMhl_DeviceCapability[DEVCAP_OFFSET_VIDEO_TYPE]		= mhlObj->sDevCaps.videoType;	
	gMhl_DeviceCapability[DEVCAP_OFFSET_LOG_DEV_MAP]	= mhlObj->sDevCaps.logicalDeviceMap;
	gMhl_DeviceCapability[DEVCAP_OFFSET_BANDWIDTH]		= mhlObj->sDevCaps.bandWidth;
	gMhl_DeviceCapability[DEVCAP_OFFSET_FEATURE_FLAG]	= mhlObj->sDevCaps.featureFlag;
	gMhl_DeviceCapability[DEVCAP_OFFSET_DEVICE_ID_H]	= mhlObj->sDevCaps.deviceIdHigh;
	gMhl_DeviceCapability[DEVCAP_OFFSET_DEVICE_ID_L]	= mhlObj->sDevCaps.deviceIdLow;	
	gMhl_DeviceCapability[DEVCAP_OFFSET_SCRATCHPAD_SIZE] = mhlObj->sDevCaps.scratchPadSize;
	gMhl_DeviceCapability[DEVCAP_OFFSET_INT_STAT_SIZE]	= mhlObj->sDevCaps.int_state_size;
	gMhl_DeviceCapability[DEVCAP_OFFSET_RESERVED]		= mhlObj->sDevCaps.reserved;

	//Write DevCaps.
	sCbusInitDevCap(mhlObj);

	sSendDcapReady(mhlObj);
}

static void setXDevCaps(MhlTxObj_t *mhlObj)
{
	memset(&gMhl_XDeviceCapability, 0, sizeof(gMhl_XDeviceCapability));
	gMhl_XDeviceCapability[XDEVCAP_ADDR_ECBUS_SPEEDS - XDEVCAP_START]		= mhlObj->sXDevCaps.ecbus_speeds;
	gMhl_XDeviceCapability[XDEVCAP_ADDR_TMDS_SPEEDS - XDEVCAP_START]		= mhlObj->sXDevCaps.tmds_speeds;
	gMhl_XDeviceCapability[XDEVCAP_ADDR_ECBUS_DEV_ROLES - XDEVCAP_START]	= mhlObj->sXDevCaps.ecbus_dev_roles;
	gMhl_XDeviceCapability[XDEVCAP_ADDR_LOG_DEV_MAPX - XDEVCAP_START]		= mhlObj->sXDevCaps.log_dev_mapx;
	//Write Ext DevCaps
	sCbusInitXDevCap(mhlObj);

	sSendDcapReady(mhlObj);
}

static void sSendDcapReady(MhlTxObj_t *mhlObj) 
{ 
	sCbusOutQCleanActiveReq(mhlObj); 
	//For TX case, TX sends DCAP_RDY and DCAP_CHG to Rx after finishing reading Device Capability Registers. 
	if (!mhlObj->cbus_state.bTxDevCapRdySent) 
	{ 
		SiiCbusSendCommand(mhlObj, API_MSC_STATUS_DCAP_RDY, NULL, 0, sCbusMscTxSendDcapRdy_Done);
	}
	else
	{
		//Send DCAP_RDY Command
		SiiCbusSendCommand(mhlObj, API_MSC_INTERRUPT_DCAP_CHG, NULL, 0, sCbusMscTxSendDcapChg_Done);
	}
}

/***** end of file ***********************************************************/
