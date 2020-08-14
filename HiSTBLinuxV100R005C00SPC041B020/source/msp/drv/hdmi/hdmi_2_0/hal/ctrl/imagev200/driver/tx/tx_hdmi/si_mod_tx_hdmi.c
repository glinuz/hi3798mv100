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
* @file si_drv_tx_hdmi.c
*
* @brief HDMI Tx Driver  
*
*****************************************************************************/
//#define SII_DEBUG

/***** #include statements ***************************************************/
#include "si_datatypes.h"
#include "si_drv_tx_regs_enums.h"
#include "si_drv_tx_api.h"
#include "si_lib_log_api.h"
#include "si_lib_malloc_api.h"
#ifndef HDMI_BUILD_IN_BOOT
//#include "si_lib_edid_api.h"
#include "si_lib_seq_api.h"
#endif
#ifdef HDMI_SCDC_SUPPORT
#include "si_mod_tx_scdc_api.h"
#endif
#include "si_lib_time_api.h"


/***** Register Module name **************************************************/

SII_LIB_OBJ_MODULE_DEF(drv_tx_hdmi);

/***** local type definitions ************************************************/
#ifdef HDMI_CEC_SUPPORT
typedef void (*CecCbFunc_t)(bool_t);
#endif

//Hdmi Tx Driver Data
typedef struct
{
	SiiModHdmiTxConfig_t     *pConfig;

	/*--------------------------------*/
	/* User request states            */
	/*--------------------------------*/
	SiiTmdsMode_t        tmdsMode;   
	SiiTmdsMode_t        prevTmdsMode;
	SiiHvSyncPol_t       hvSyncPol;

	uint8_t				bIsHdcpOn;
	uint8_t				bWasHdcpOn;
	uint8_t				bIsScdc;
	uint8_t				bAvMute;
    uint8_t				bAudioMute;
    
	uint8_t				bIfOnAvi;
	uint8_t				bIfOnAudio;
	uint8_t				bIfOnVs;   
	uint8_t				bIfOnSpd;
	uint8_t				bIfOnGbd;
	uint8_t				bIfOnMpeg;
	uint8_t				bIfOnIsrc;
	uint8_t				bIfOnIsrc2;
	uint8_t				bIfOnAcp;
#ifdef HDMI_HDR_SUPPORT
    uint8_t             bIfOnDrm;
#endif

	SiiInfoFrame_t      ifAvi;
	SiiInfoFrame_t      ifAudio;
	SiiInfoFrame_t      ifVs;
	SiiInfoFrame_t      ifSpd;
	SiiInfoFrame_t		ifGbd;
	SiiInfoFrame_t		ifMpeg;
	SiiInfoFrame_t		ifIsrc;
	SiiInfoFrame_t		ifIsrc2;
	SiiInfoFrame_t		ifAcp;
#ifdef HDMI_HDR_SUPPORT
    SiiInfoFrame_t      ifDrm;
#endif

	SiiChannelStatus_t   channelStatus;
	SiiAudioFormat_t     audioFormat;

	SiiDrvTxBitDepth_t	inputBitDepth;
	SiiDrvTxBitDepth_t	outputBitDepth;
	SiiDrvClrSpc_t		inputClrSpc;
	SiiDrvClrSpc_t		outputClrSpc;

	/*--------------------------------*/
	/* User status                    */
	/*--------------------------------*/
	SiiEdid_t          edid;
	uint8_t            bHotPlug;
	uint8_t            bRsen;
	uint8_t            bInitHotPlug;

	/*--------------------------------*/
	/* Internal states                */
	/*--------------------------------*/
	SiiInst_t			timerHwUpdate;     //!< Timer for hw updates
	SiiInst_t           timerAvmute;
    SiiInst_t           timerZeroDrmIf; /**<Zero DRM Infoframe send timeout */
    SiiInst_t           timerHdrMode;   /**<HDR mode change timer */
    
	SiiInst_t			hdcpInst;          //!< hdcp instance
	SiiInst_t			scdcInst;
	IntStat_t			intStat;
	SiiDrvHdcpStatus_t	hdcpStatus;
    SiiHdcpCaps_t       stHdcpCaps;
        
	SiiModHdmiTxInternalState_t		iState;
	SiiModHdmiTxState_t			eState;
#if 0
	SiiLibEdidPar_t				parseEdid;
#endif
	HdmiTxEventNotifyCallBack	cbFunc;

#ifdef HDMI_CEC_SUPPORT
	CecCbFunc_t         cecCbFunc;
#endif
	bool_t		isMhlConnected;
	bool_t		isMhl3Connected;
	bool_t		prevMhlConnStatus;
} HdmiTxObj_t;

/***** local prototypes ******************************************************/
#ifndef HDMI_BUILD_IN_BOOT
//static void sTxHwUpdateHandler(SiiInst_t inst);
static void sVirtualIsrHandler(HdmiTxObj_t* pObj);
static void sPlugStatusUpdate(HdmiTxObj_t* pObj);
static ddcComErr_t sUpdateEdid(HdmiTxObj_t* pObj, uint8_t segmentIndex, uint8_t regAddr, uint8_t *pBuf, uint16_t length);
static bool_t sWaitForDdcBus(HdmiTxObj_t* pObj);
//static SiiLibEdidErrCode_t sParseEdid(HdmiTxObj_t* pObj);
#endif
static void sApplyInfoFrame(HdmiTxObj_t* pObj, uint8_t bApply);
static void sInfoframesSet(HdmiTxObj_t* pObj, SiiInfoFrameId_t ifId);
static void sInfoframeTypeSet(HdmiTxObj_t* pObj, SiiInfoFrame_t *pInfoFrame);
static void sInfoframeOnOffSet(HdmiTxObj_t* pObj, SiiInfoFrameId_t ifId, uint8_t onOff);
#ifndef HDMI_BUILD_IN_BOOT
static void sInfoframeOnOffGet(HdmiTxObj_t* pObj, SiiInfoFrameId_t ifId, uint8_t* onOff);
static void sInfoframeTypeGet(HdmiTxObj_t* pObj, SiiInfoFrame_t *pInfoFrame);
//static void sUpdateInfoframes(HdmiTxObj_t* pObj);
static void sAudioformatSet(HdmiTxObj_t* pObj);
//static void sUpdateAudioformat(HdmiTxObj_t* pObj);
static void sChannelStateSet(HdmiTxObj_t* pObj);
//static void sUpdateChannelState(HdmiTxObj_t* pObj);
#endif
static void sPutTMDSOnOff(HdmiTxObj_t* pObj, bool_t onOff);
static void sUpdateHdmiMode(HdmiTxObj_t* pObj);
static void sTmdsModeSet(HdmiTxObj_t* pObj);
static void sAvMuteSet(HdmiTxObj_t* pObj, uint8_t bAvMute);
#ifndef HDMI_BUILD_IN_BOOT
static void sAvMuteClrStopSet(SiiInst_t inst);
//static void sUpdateAvMute(HdmiTxObj_t* pObj);
static void sUpdateHotPlugRsen(HdmiTxObj_t* pObj, uint_t event);

#ifdef HDMI_HDR_SUPPORT
static void sZeroDrmIfTimeOut(SiiInst_t inst);
static void sZeroDrmIfTimeSet(HdmiTxObj_t* pObj, SiiHdrTimerCfg_S *pstZeroDrmIfTimer);

static void sHdrModeChangeTimeOut(SiiInst_t inst);
static void sHdrModeChangeTimeSet(HdmiTxObj_t* pObj, SiiHdrTimerCfg_S *pstHdrModeChangeTimer);
#endif
#endif
static void sUpdateOutputBitDepth(HdmiTxObj_t* pObj, SiiDrvBitDepth_t bitDepth);
static SiiDrvCraAddr_t sBaseAddrGet(HdmiTxObj_t *pObj);
static SiiInst_t sCraInstGet(HdmiTxObj_t *pObj);
//static void sTxLog(uint8_t *pData, uint16_t len);
#ifndef HDMI_BUILD_IN_BOOT
#if 0
static SiiLibEdidErrCode_t sParseEdid(HdmiTxObj_t* pObj);
#endif

#ifdef HDMI_HDCP_SUPPORT
static ddcComErr_t sHdcpCapabilityGet(HdmiTxObj_t* pObj);
#endif
static ddcComErr_t sEdidRead(HdmiTxObj_t* pObj);

void sUpdatePrevMhlConnStatus(HdmiTxObj_t* hdmiObj);
//Notify functions
void sNotifyHdmiState(HdmiTxObj_t *pObj);

static void sGetTMDSOnOff(HdmiTxObj_t* pObj, bool_t* onOff);
static void sAudioEnableSet(HdmiTxObj_t* pObj, uint8_t enable);

static void sAudioMuteSet(HdmiTxObj_t* pObj, uint8_t bAudioMute);
#endif
static void sReset(HdmiTxObj_t* pObj, SiiResetType_t resetSrc, uint8_t reset);
#ifndef HDMI_BUILD_IN_BOOT
static void sIRQEnableSet(HdmiTxObj_t* pObj, SiiInterruptType_t intSrc, uint8_t enable);
static void sHdmiHardWareStatusGet(HdmiTxObj_t* pObj,  SiiHdmiStatus_t *hdmiStatus);
#endif
static void sHardwareInit(HdmiTxObj_t* pObj);
static void sSoftwareInit(HdmiTxObj_t* hdmiObj);
static void sPhyPowerSet(HdmiTxObj_t* pObj, uint8_t onOff);
static void sTPIReset(HdmiTxObj_t* pObj, uint8_t bReset);
static void sPhyConfig(HdmiTxObj_t* pObj, SiiDrvPhyPara_t *pst_phy_param);
static void sDdcSpeedConfig(HdmiTxObj_t* pObj, uint32_t u32DdcSpeed);
static void sTransCodeBypassConfig(HdmiTxObj_t* pObj, bool_t b_bypass);

#define BIT_MASK_HDCP_2_2_SUPPORT   0x04
#define BIT_MASK_HDCP_1_4_SUPPORT   0x80
/***** local data objects ****************************************************/
#ifdef HDMI_SCDC_SUPPORT
static SiiModTxScdcConfig_t sScdcConfig = 
{
	SII_DRV_TX_BASE_ADDR_CYPRESS,      //!<  baseAddr   : Base Addrress of SCDC-Tx register space
	true,                            //!<  bReadReq   : If true then use read-request if Sink support read request as well.
	1,                               //!<  srcVersion : Source version
};
#endif
#if 0
typedef struct tagHDMI_WORK_S
{
	HdmiTxObj_t*             pObj;
	struct work_struct       work; 
}HDMI_WORK_S;
static struct workqueue_struct   *s_queue;

static 	HDMI_WORK_S    stHdmiEdidUpdate;

static HI_VOID SiiModHdmiTxEdidUpdate(struct work_struct *data)
{
	HdmiTxObj_t*             pObj;
    HDMI_WORK_S *EdidWork;
	EdidWork = container_of(data, HDMI_WORK_S, work);
	pObj	  = (EdidWork->pObj);
    sPlugStatusUpdate(pObj);
}
#endif
static void sDdcSpeedConfig(HdmiTxObj_t* pObj, uint32_t u32DdcSpeed)
{

    SiiDrvCraAddr_t baseAddr = sBaseAddrGet(pObj);
    SiiInst_t		craInst	 = sCraInstGet(pObj); 
    uint8_t         u8Tmp = 0;     
    
    u8Tmp = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__DDC_DOUT_CNT);
    u8Tmp |= ((u32DdcSpeed >>1) & BIT_MSK__DDC_DOUT_CNT__REG_DDC_DELAY_CNT_B1);
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DDC_DOUT_CNT, u8Tmp);
    
    u8Tmp = u32DdcSpeed & 0xff;
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DDC_DELAY_CNT, u8Tmp);

}
/***** public functions ******************************************************/

//-------------------------------------------------------------------------------------------------
//! @brief      Initialize HDMI TX module
//-------------------------------------------------------------------------------------------------
SiiInst_t SiiModHdmiTxCreate(char *pNameStr, SiiModHdmiTxConfig_t *pConfig)
{
	HdmiTxObj_t*	hdmiObj = NULL;
	//SiiDrvCraAddr_t	baseAddr;
	//SiiInst_t		craInst;
#if 0   
    s_queue = create_workqueue("hdmi_edid");
    if (s_queue != NULL)
    {
        INIT_WORK(&stHdmiEdidUpdate.work,   SiiModHdmiTxEdidUpdate);
    }
#endif
	/* Allocate memory for hdmi_tx object */
	hdmiObj = (HdmiTxObj_t*)SII_LIB_OBJ_CREATE(pNameStr, sizeof(HdmiTxObj_t));
	SII_PLATFORM_DEBUG_ASSERT(hdmiObj);

	hdmiObj->pConfig = (SiiModHdmiTxConfig_t*)SiiLibMallocCreate(sizeof(SiiModHdmiTxConfig_t));
	memcpy(hdmiObj->pConfig, pConfig, sizeof(SiiModHdmiTxConfig_t));
#ifndef HDMI_BUILD_IN_BOOT
    HDMI_INIT_MUTEX(hdmiObj->pConfig->stDdcLock);
#endif
    hdmiObj->cbFunc = hdmiObj->pConfig->cbFunc;

    sSoftwareInit(hdmiObj);
#if 0
	// Create a timer to update Tx states
	hdmiObj->timerHwUpdate = SII_LIB_SEQ_TIMER_CREATE("Hardware_Update", sTxHwUpdateHandler, SII_LIB_OBJ_INST(hdmiObj), TIMER_START__HW_UPDATE_PRI, SII_TIMER_HWUPDATE);
	SII_PLATFORM_DEBUG_ASSERT(hdmiObj->timerHwUpdate);
#endif
#ifndef HDMI_BUILD_IN_BOOT
    hdmiObj->timerAvmute = SII_LIB_SEQ_TIMER_CREATE("avmuteClrStop", sAvMuteClrStopSet, SII_LIB_OBJ_INST(hdmiObj), TIMER_START__AVMUTE_CLR_STOP, SII_TIMER_AVMUTE);
    sema_init(&g_avmuteTimerMute, 1);
#ifdef HDMI_HDR_SUPPORT    
    hdmiObj->timerZeroDrmIf = SII_LIB_SEQ_TIMER_CREATE("zeroDrmInfoTimeout", sZeroDrmIfTimeOut, SII_LIB_OBJ_INST(hdmiObj), TIMER_START_ZERO_DRM_IF, SII_TIMER_ZERO_DRM_INFO);
    hdmiObj->timerHdrMode   = SII_LIB_SEQ_TIMER_CREATE("HdrModeChangeTimeout", sHdrModeChangeTimeOut, SII_LIB_OBJ_INST(hdmiObj), TIMER_START_HDR_MODE_CHANGE, SII_TIMER_HDR_MODE_CHANGE);
#endif
#endif    
    
#ifdef HDMI_SCDC_SUPPORT
	//Creating SCDC
	sScdcConfig.instTxCra  = pConfig->instTxCra;
    sScdcConfig.cbFunc     = hdmiObj->pConfig->cbFunc;
    sScdcConfig.instTx     = hdmiObj->pConfig->instTx;
#ifndef HDMI_BUILD_IN_BOOT
    sScdcConfig.pstDdcLock = &hdmiObj->pConfig->stDdcLock;
#endif

    sScdcConfig.pu32VirtualBaseAddr = hdmiObj->pConfig->pu32VirtuBaseAddr;

	hdmiObj->scdcInst = SiiModTxScdcCreate("tx_scdc", SII_LIB_OBJ_INST(hdmiObj), &sScdcConfig);
	SII_PLATFORM_DEBUG_ASSERT(hdmiObj->scdcInst);
#endif
#if 0
	/*--------------------------------*/
	/* Static hardware configuration  */
	/*--------------------------------*/
	//Cypress2 initialization
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__CLKPWD		, 0x06);//Gate off pclk,mhl1/2/3 clk
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__FUNC_SEL		, 0x01);//enable HDMI for tx
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__SYS_MISC		, 0x00);
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DIPT_CNTL		, 0x06);//enable packets pass through function  

    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__P2T_CTRL		, 0x80);//enable Deep Color Packet
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__TEST_TXCTRL	, 0x02);//enable HDMI mode for output
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__TX_ZONEL_CTL4	, 0x04);

    // PHY init
	SiiDrvCraWrReg8(craInst, baseAddr | TXPHY_TOP_CTL0	, 0x90);
	SiiDrvCraWrReg8(craInst, baseAddr | TXPHY_PLL_CTL0	, 0x82);
	SiiDrvCraWrReg8(craInst, baseAddr | TXPHY_PLL_CTL2	, 0x30);//Phy power on

	//SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__EDID_CTRL	, 0x59);

	// Cypress2 - audio related initialization
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__I2S_CHST3, 0x02);             // set AIP channel Sampling frequency to 48 KHz
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__I2S_CHST4, 0x0b);             // set AIP channel status word length to 24 bits
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__ACR_CTRL, 0x0e);              // configure AIP cts generation
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__I2S_IN_SIZE, 0x0b);           // set AIP i2s word length to 24 bits
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__I2S_IN_CTRL, 0x60);           // configure AIP i2s input
	SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__AUD_MODE, 0x10, 0x10);       // enable sd0-3 input
	SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__AUDP_TXCTRL, BIT_MSK__AUDP_TXCTRL__REG_LAYOUT); // layout 0         
	SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__AUD_EN, BIT_MSK__AUD_EN__REG_AUD_IN_EN, BIT_MSK__AUD_EN__REG_AUD_IN_EN);// enable audio

	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__TPI_DOWN_SMPL_CTRL, BIT_MSK__TPI_DOWN_SMPL_CTRL__REG_TPI_AUDIO_LOOKUP_EN);// enable LUT  
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__TPI_AUD_CONFIG, 0x00);
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__TPI_AUD_FS, 0x02);// fs = 48kHz
	SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__TPI_AUD_FS, BIT_MSK__TPI_AUD_FS__REG_TPI_AUD_SF_OVRD, BIT_MSK__TPI_AUD_FS__REG_TPI_AUD_SF_OVRD);// enable fs override

	// reset AIP and AFIFO
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__AIP_RST, BIT_MSK__AIP_RST__REG_RST4AUDIO_FIFO | BIT_MSK__AIP_RST__REG_RST4AUDIO);              
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__AIP_RST, 0x00);              

	//SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__N_SVAL1, 0x00);               // enable sd0-3 input
	//SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__N_SVAL2, 0x18);               // enable sd0-3 input   
	//SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__N_SVAL3, 0x00);               // enable sd0-3 input
	//SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__AUD_EN, 0x03, 0x03);         // enable audio
	//SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__SPDIF_ERTH, BIT_MSK__SPDIF_ERTH__REG_I2S2DSD_EN);   //disable to use I2S or spdiff port for DSD
	// enable fs lookup tables 
	//SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__TPI_DOWN_SMPL_CTRL, BIT_MSK__TPI_DOWN_SMPL_CTRL__REG_TPI_AUDIO_LOOKUP_EN);

	// set HW TPI
	//SiiDrvCraClrBit8(pObj->pConfig->instTxCra, priBaseAddr | REG_ADDR__LM_DDC, BIT_MSK__LM_DDC__REG_SW_TPI_EN);

	//Enable Intr1 Interrupts.

	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__INTR1_MASK, BIT_MSK__INTR1_MASK__REG_INTR1_MASK5 | BIT_MSK__INTR1_MASK__REG_INTR1_MASK6);	

#endif

#if 0
    if (SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__TPI_HPD_RSEN) & BIT_MSK__TPI_HPD_STATE)
    {
        hdmiObj->bHotPlug = true;
    }
    else
    {
        hdmiObj->bHotPlug = false;
    }
    sUpdateHotPlugRsen(hdmiObj);
#endif   
	return SII_LIB_OBJ_INST(hdmiObj);
}

//-------------------------------------------------------------------------------------------------
//! @brief      Delete HDMI TX module
//-------------------------------------------------------------------------------------------------
void SiiModHdmiTxDelete(SiiInst_t inst)
{
	HdmiTxObj_t* hdmiObj = (HdmiTxObj_t*)SII_LIB_OBJ_PNTR(inst);
#ifdef HDMI_SCDC_SUPPORT
	SiiModTxScdcDelete(hdmiObj->scdcInst);
#endif
	//SiiLibSeqTimerDelete(hdmiObj->timerHwUpdate);
#ifndef HDMI_BUILD_IN_BOOT	
    SiiLibSeqTimerDelete(hdmiObj->timerHdrMode);
    SiiLibSeqTimerDelete(hdmiObj->timerZeroDrmIf);
    SiiLibSeqTimerDelete(hdmiObj->timerAvmute);
#endif
	SiiLibMallocDelete(hdmiObj->pConfig);
	SII_LIB_OBJ_DELETE(hdmiObj);
}
#ifndef HDMI_BUILD_IN_BOOT
//-------------------------------------------------------------------------------------------------
//! @brief      HDMI TX Interrupt handler.
//!
//!             Check for HDMI TX related interrupts. If found, clear pending hardware interrupt
//!             bits and change the status to indicate pending interrupt.
//!
//!             This function is to be called from the Device Interrupt manager upon receiving
//!             a hardware interrupt from TX.
//!
//! @param[in]  inst  - instance of the notification addressee
//-------------------------------------------------------------------------------------------------
void SiiModHdmiTxTpiInterruptHandler(SiiInst_t inst)
{
	HdmiTxObj_t*	pObj		= (HdmiTxObj_t*)SII_LIB_OBJ_PNTR(inst);
	SiiDrvCraAddr_t baseAddr	= sBaseAddrGet(pObj);
	SiiInst_t		craInst		= sCraInstGet(pObj);
	IntStat_t       intStat;
	uint8_t         mask;

	// Capture and mask HPD(STAT6) and RSEN(STAT5) interrupt status bits.

	mask = BIT_MSK__INTR1__REG_INTR1_STAT6 | BIT_MSK__INTR1__REG_INTR1_STAT5;

	intStat.reg0 = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__INTR1);
	intStat.reg0 &= mask;
   
	if(SII_MEMCMP(&pObj->intStat, &intStat, sizeof(IntStat_t)))
	{
		uint8_t intreg = 0;
		// Find any interrupt status bit that changed to '1'
		intreg |= ((pObj->intStat.reg0 ^ intStat.reg0) & intStat.reg0);

		pObj->intStat = intStat;
		if(intreg)
		{
			// Call derived interupt handler
			sVirtualIsrHandler(pObj);
		}
	}
}
#endif
//Todo : Try to return ErrorStatus
//-------------------------------------------------------------------------------------------------
//! @brief      HDMI TX module Set API
//-------------------------------------------------------------------------------------------------
bool_t SiiModHdmiTxSet(SiiInst_t inst, SiiModHdmiTxInternalOpcode_t opcode, const void *inData)
{
	HdmiTxObj_t* pObj = (HdmiTxObj_t*)SII_LIB_OBJ_PNTR(inst);
	switch(opcode)
	{
	case SII_MOD_HDMI_TX_OPCODE__TMDS_MODE:
		{
			SII_MEMCPY(&pObj->tmdsMode, inData, sizeof(pObj->tmdsMode));
			sTmdsModeSet(pObj);		
			break;
		}
	case SII_MOD_HDMI_TX_OPCODE__AVMUTE:
		{
			SII_MEMCPY(&pObj->bAvMute, inData, sizeof(pObj->bAvMute));
			sAvMuteSet(pObj, pObj->bAvMute);
			break;
		}
	case SII_MOD_HDMI_TX_OPCODE__INFOFRAME_TYPE:
		{
			SiiInfoFrame_t *pInfoframe = (SiiInfoFrame_t*)inData;
			sInfoframeTypeSet(pObj, pInfoframe);		
			break;
		}
	case SII_MOD_HDMI_TX_OPCODE__INFOFRAME_ONOFF:
		{
			uint8_t *inp = (uint8_t *)inData;
			sInfoframeOnOffSet(pObj, (SiiInfoFrameId_t)inp[0], inp[1]);		
			break;
		}
#ifndef HDMI_BUILD_IN_BOOT    
	case SII_MOD_HDMI_TX_OPCODE__AUDIO_MUTE:
		{
			SII_MEMCPY(&pObj->bAudioMute, inData, sizeof(pObj->bAudioMute));
			sAudioMuteSet(pObj, pObj->bAudioMute);
			break;
		}  
	case SII_MOD_HDMI_TX_OPCODE__CHANNEL_STATUS:
		{
			SII_MEMCPY(&pObj->channelStatus, inData, sizeof(pObj->channelStatus));
            sChannelStateSet(pObj);
			//SiiLibSeqTimerStart(pObj->timerHwUpdate, TIMER_START__HW_UPDATE, 0); //Make this call for all Set cases
			break;
		}
	case SII_MOD_HDMI_TX_OPCODE__AUDIO_FORMAT:
		{
			SII_MEMCPY(&pObj->audioFormat, inData, sizeof(pObj->audioFormat));
            sAudioformatSet(pObj);
			//SiiLibSeqTimerStart(pObj->timerHwUpdate, TIMER_START__HW_UPDATE, 0);
			break;
		}
	case SII_MOD_HDMI_TX_OPCODE__AUDIO_ENABLE:
		{
            sAudioEnableSet(pObj, *(uint8_t*)inData);
			break;
		} 
#endif    
	case SII_MOD_HDMI_TX_OPCODE__OUTPUT_BIT_DEPTH:
		{
			SiiDrvBitDepth_t bitDepth = *(SiiDrvBitDepth_t *)inData;
			//SII_MEMCPY(&bitDepth, inData, sizeof(bitDepth));
			sUpdateOutputBitDepth(pObj, bitDepth);
			break;
		}
#ifndef HDMI_BUILD_IN_BOOT    
	case SII_MOD_HDMI_TX_OPCODE__HDCP_PROTECTION:
		{
			SII_MEMCPY(&pObj->bIsHdcpOn, inData, sizeof(pObj->bIsHdcpOn));
			SiiLibSeqTimerStart(pObj->timerHwUpdate, TIMER_START__HW_UPDATE, 0);
			break;
		}
	case SII_MOD_HDMI_TX_OPCODE__HDCP_STATUS:
		{
			SII_MEMCPY(&pObj->hdcpStatus, inData, sizeof(pObj->hdcpStatus));
			//SiiLibSeqTimerStart(pObj->timerHwUpdate, TIMER_START__HW_UPDATE_5MS_WAIT, 0);
			break;
		}
	case SII_MOD_HDMI_TX_OPCODE__HW_UPDATE_START:
		{
			//SiiLibSeqTimerStart(pObj->timerHwUpdate, TIMER_START__HW_UPDATE_5MS_WAIT, 0);
			break;
		}
	case SII_MOD_HDMI_TX_OPCODE__UPDATE_MHL_CONN_STATUS:
		{
			sUpdatePrevMhlConnStatus(pObj);
			SII_MEMCPY(&pObj->isMhlConnected, inData, sizeof(pObj->isMhlConnected));
			//SiiLibSeqTimerStart(pObj->timerHwUpdate, TIMER_START__HW_UPDATE_5MS_WAIT, 0);
			sPlugStatusUpdate(pObj);
			break;
		}
	case SII_MOD_HDMI_TX_OPCODE__UPDATE_MHL_CONN_TYPE:
		SII_MEMCPY(&pObj->isMhl3Connected, inData, sizeof(pObj->isMhl3Connected));
		break;
#endif        
    case SII_MOD_HDMI_TX_OPCODE__RESET:
        {
            SiiReset_t* resetInfo = (SiiReset_t*) inData;
            sReset(pObj, resetInfo->resetSrc, resetInfo->reset);
            break;
        }
#ifndef HDMI_BUILD_IN_BOOT    
    case SII_MOD_HDMI_TX_OPCODE__INT_ENABLE:
        {      
            SiiInterrupt_t* intInfo = (SiiInterrupt_t*) inData;
            sIRQEnableSet(pObj, intInfo->intSrc, intInfo->enable);  
            break;
        }
#endif    
    case SII_MOD_HDMI_TX_OPCODE__HW_INIT:
        {      
            sHardwareInit(pObj); 
            break;
        } 
    case SII_MOD_HDMI_TX_OPCODE__SW_INIT:
        {      
            sSoftwareInit(pObj); 
            break;
        } 
    case SII_MOD_HDMI_TX_OPCODE__PHY_POWER:
        {      
            sPhyPowerSet(pObj, *(uint8_t*)inData); 
            break;
        }     
    case SII_MOD_HDMI_TX_OPCODE__PHY_OE:
        {      
            sPutTMDSOnOff(pObj, *(uint8_t*)inData); 
            break;
        } 
    case SII_MOD_HDMI_TX_OPCODE__PHY_CONFIG:
        {      
            sPhyConfig(pObj, (SiiDrvPhyPara_t *)inData); 
            break;
        } 
        
    case SII_MOD_HDMI_TX_OPCODE__DDC_SPEED_CONFIG:
        {      
            sDdcSpeedConfig(pObj, *(uint32_t*)inData); 
            break;
        } 

    case SII_MOD_HDMI_TX_OPCODE__BYPASS_CONFIG:
        {      
            sTransCodeBypassConfig(pObj, *(bool_t*)inData); 
            break;
        } 
       
#ifdef HDMI_SCDC_SUPPORT
    case SII_MOD_HDMI_TX_OPCODE__SCDC_CONFIG:
        {      
            SiiModTxScdcSinKCaps_t* scdcSinkCaps = (SiiModTxScdcSinKCaps_t*)inData;
            if(scdcSinkCaps->bScdcEnable && scdcSinkCaps->bScdcPresent && !pObj->isMhlConnected)
            {
                SiiDrvTxScdcSinkCapsSet(pObj->scdcInst, scdcSinkCaps);
            }
    		else if(!scdcSinkCaps->bScdcEnable && scdcSinkCaps->bScdcPresent && !pObj->prevMhlConnStatus)
    		{
    			SiiModTxScdcReset(pObj->scdcInst);
    		}
            
            break;
        } 
#endif
#ifndef HDMI_BUILD_IN_BOOT  
#ifdef HDMI_HDR_SUPPORT
    case SII_MOD_HDMI_TX_OPCODE__ZERO_DRMIF_TIMER_SET:
        {
            SiiHdrTimerCfg_S *pstZeroDrmIfTimer =  (SiiHdrTimerCfg_S *)inData;
            sZeroDrmIfTimeSet(pObj, pstZeroDrmIfTimer);
            break;
        }
    case SII_MOD_HDMI_TX_OPCODE__HDR_MODE_CHANGE_TIMER_SET:
        {
            SiiHdrTimerCfg_S *pstZeroDrmIfTimer =  (SiiHdrTimerCfg_S *)inData;
            sHdrModeChangeTimeSet(pObj, pstZeroDrmIfTimer);
        }
#endif    
#endif  

    default:
        break;
	}

	return true;
}

#ifndef HDMI_BUILD_IN_BOOT
//Todo : Try to return ErrorStatus
//-------------------------------------------------------------------------------------------------
//! @brief      HDMI TX module Get API
//-------------------------------------------------------------------------------------------------
bool_t SiiModHdmiTxGet(SiiInst_t inst, SiiModHdmiTxInternalOpcode_t opcode, void *outData)
{
	HdmiTxObj_t* pObj = (HdmiTxObj_t*)SII_LIB_OBJ_PNTR(inst);
	//uint16_t cecAddr = 0;
	switch(opcode)
	{
	case SII_MOD_HDMI_TX_OPCODE__EDID:
        {
    		if (SI_TX_DDC_ERROR_CODE_NO_ERROR == sEdidRead(pObj))
            {
                SII_MEMCPY(outData, &(pObj->edid), sizeof(pObj->edid));
            }        
    		else
            {
                return false;
            }        
	    	break;
       }
	case SII_MOD_HDMI_TX_OPCODE__HOTPLUG:
		SII_MEMCPY(outData, &(pObj->bHotPlug), sizeof(pObj->bHotPlug));
		break;
	case SII_MOD_HDMI_TX_OPCODE__RSEN:
		SII_MEMCPY(outData, &(pObj->bRsen), sizeof(pObj->bRsen));
		break;
	case SII_MOD_HDMI_TX_OPCODE__HDMI_STATE:
		SII_MEMCPY(outData, &(pObj->eState), sizeof(pObj->eState));
		break;
	case SII_MOD_HDMI_TX_OPCODE__TMDS_MODE:
		SII_MEMCPY(outData, &(pObj->tmdsMode), sizeof(pObj->tmdsMode));
		break;
	case SII_MOD_HDMI_TX_OPCODE__CEC_PHY_ADDR:
    #if 0    
		cecAddr = ((pObj->parseEdid.cecAddr.sub[0] << 12) | (pObj->parseEdid.cecAddr.sub[1] << 8)
			| (pObj->parseEdid.cecAddr.sub[2] << 4) | pObj->parseEdid.cecAddr.sub[3]);
		SII_MEMCPY(outData, &(cecAddr), sizeof(cecAddr));
    #endif
        break;
	case SII_MOD_HDMI_TX_OPCODE__AUDIO_FORMAT:
		SII_MEMCPY(outData, &(pObj->audioFormat), sizeof(pObj->audioFormat));
		break;
	case SII_MOD_HDMI_TX_OPCODE__EDID_LIPSYNC:
		//SII_MEMCPY(outData, &(pObj->parseEdid.lipSync), sizeof(pObj->parseEdid.lipSync));
		break; 
    case SII_MOD_HDMI_TX_OPCODE__PHY_OE:
        sGetTMDSOnOff(pObj, (bool_t*)outData);
        break;
#ifndef HDMI_BUILD_IN_BOOT
    case SII_MOD_HDMI_TX_OPCODE__HW_STATUS:
        sHdmiHardWareStatusGet(pObj, (SiiHdmiStatus_t*)outData);
        break;
        
#ifdef HDMI_SCDC_SUPPORT
        case SII_MOD_HDMI_TX_OPCODE__SCDC_STATUS:
        SiiDrvTxScdcStatusGet(pObj->scdcInst, (SiiScdcStatus_t*)outData);
        break;
        
        case SII_MOD_HDMI_TX_OPCODE__SCDC_SET:
        SiiDrvTxScdcStatusSet(pObj->scdcInst, (SiiScdcStatus_t*)outData);
        break;
#endif
        break;
#endif
#ifdef HDMI_HDCP_SUPPORT  
    case SII_MOD_HDMI_TX_OPCODE__HDCP_CAPABILITY:
        sHdcpCapabilityGet(pObj);
   		SII_MEMCPY(outData, &(pObj->stHdcpCaps), sizeof(SiiHdcpCaps_t));
        break;
#endif
#if 0
	case SII_MOD_HDMI_TX_OPCODE__INFOFRAME_TYPE:
		{
			SiiInfoFrame_t *pInfoframe = (SiiInfoFrame_t*)outData;
			sInfoframeTypeGet(pObj, pInfoframe);		
			break;
		}
	case SII_MOD_HDMI_TX_OPCODE__INFOFRAME_ONOFF:
		{
			uint8_t *inp = (uint8_t *)outData;
			sInfoframeOnOffGet(pObj, (SiiInfoFrameId_t)inp[0], &inp[1]);		
			break;
		}
#endif
    default:
        break;
    }

	return true;
}
#if 0
/***** local functions *******************************************************/
//-------------------------------------------------------------------------------------------------
//! @brief      Hardware Update handler
//!
//!             update hardware registers based on the guidance from integration layer.
//!
//! @param[in]  inst  - instance of the notification addressee
//-------------------------------------------------------------------------------------------------
static void sTxHwUpdateHandler(SiiInst_t inst)
{
	HdmiTxObj_t* pObj = (HdmiTxObj_t*)SII_LIB_OBJ_PNTR(inst);

	SII_LIB_LOG_DEBUG1(pObj, ("sTxHwUpdateHandler()!! TMDS Mode: %02X, hdcp is: %s\n",pObj->tmdsMode, pObj->bIsHdcpOn ? "ON":"OFF"));

	switch (pObj->iState)
	{
	case SII_MOD_HDMI_TX_EVENT__TMDS_OFF:

		SII_LIB_LOG_DEBUG1(pObj, ("TMDS_OFF\n"));
		if(pObj->bWasHdcpOn)
		{
			pObj->bWasHdcpOn = false;

    		SII_LIB_LOG_DEBUG1(pObj, ("HDCP_OFF\n"));
    		// stop HDCP
    		pObj->eState = SII_MOD_HDMI_STATUS__HDCP_OFF;
    		sNotifyHdmiState(pObj);            
		}
		else
		{
			//pObj->iState = SII_MOD_HDMI_TX_EVENT__UPDATE_TMDS;
		}
    
		//SiiLibSeqTimerStart(pObj->timerHwUpdate, TIMER_START__HW_UPDATE_5MS_WAIT, 0);
		break;

	case SII_MOD_HDMI_TX_EVENT__TMDS_ON:
		SII_LIB_LOG_DEBUG1(pObj, ("TMDS_ON\n"));
    #if 0    
		if( ( pObj->tmdsMode == SII_TMDS_MODE__HDMI1 ) || ( pObj->tmdsMode == SII_TMDS_MODE__HDMI2 ) )
		{
			sUpdateInfoframes(pObj);
			sUpdateAudioformat(pObj);
			sUpdateChannelState(pObj);
		}
    #endif
		if( pObj->bIsHdcpOn && !pObj->bWasHdcpOn)
		{
			pObj->bWasHdcpOn = pObj->bIsHdcpOn;
    		SII_LIB_LOG_DEBUG1(pObj, ("HDCP_ON\n"));
    		pObj->eState = SII_MOD_HDMI_STATUS__HDCP_ON;
    		sNotifyHdmiState(pObj);
            
			//SiiLibSeqTimerStart(pObj->timerHwUpdate, TIMER_START__HW_UPDATE_5MS_WAIT, 0);

		}
		else if( !pObj->bIsHdcpOn && pObj->bWasHdcpOn )
		{
			pObj->bWasHdcpOn = pObj->bIsHdcpOn;
    		SII_LIB_LOG_DEBUG1(pObj, ("HDCP_OFF\n"));
    		// stop HDCP
    		pObj->eState = SII_MOD_HDMI_STATUS__HDCP_OFF;
    		sNotifyHdmiState(pObj);
            //SiiLibSeqTimerStart(pObj->timerHwUpdate, TIMER_START__HW_UPDATE_5MS_WAIT, 0);
		}
   		else
		{
			//pObj->iState = SII_MOD_HDMI_TX_EVENT__UPDATE_TMDS;
			//SiiLibSeqTimerStart(pObj->timerHwUpdate, TIMER_START__HW_UPDATE_5MS_WAIT, 0);
		}
		break;
#if 0
	case SII_MOD_HDMI_TX_EVENT__HDCP_ON:
		SII_LIB_LOG_DEBUG1(pObj, ("HDCP_ON\n"));
		sUpdateHdmiMode(pObj);
		pObj->prevTmdsMode = pObj->tmdsMode;
		pObj->eState = SII_MOD_HDMI_STATUS__HDCP_ON;
		sNotifyHdmiState(pObj);
		pObj->iState = SII_MOD_HDMI_TX_EVENT__HDCP_STATE;
		break;

	case SII_MOD_HDMI_TX_EVENT__HDCP_OFF:
		SII_LIB_LOG_DEBUG1(pObj, ("HDCP_OFF\n"));
		// stop HDCP
		pObj->eState = SII_MOD_HDMI_STATUS__HDCP_OFF;
		sNotifyHdmiState(pObj);
		pObj->iState = SII_MOD_HDMI_TX_EVENT__UPDATE_TMDS;
		SiiLibSeqTimerStart(pObj->timerHwUpdate, TIMER_START__HW_UPDATE_100MS_WAIT, 0);
		break;

	case SII_MOD_HDMI_TX_EVENT__HDCP_STATE:
		SII_LIB_LOG_DEBUG1(pObj, ("HDCP_STATE\n"));
		if( !pObj->bIsHdcpOn && pObj->bWasHdcpOn )
		{
			pObj->bWasHdcpOn = pObj->bIsHdcpOn;
			pObj->iState = SII_MOD_HDMI_TX_EVENT__HDCP_OFF;
			//SiiLibSeqTimerStart(pObj->timerHwUpdate, TIMER_START__HW_UPDATE_5MS_WAIT, 0);
            sTxHwUpdateHandler(inst);
            break;
		}
		switch(pObj->hdcpStatus) 
		{
		case SII_DRV_HDCP_STATUS__SUCCESS_1X: 
		case SII_DRV_HDCP_STATUS__SUCCESS_22: 
			{
				/*if(!pObj->bBksvListApproved)
				{
				return;
				}*/
				pObj->iState = SII_MOD_HDMI_TX_EVENT__TMDS_ON; 
				break; 
			}
		case SII_DRV_HDCP_STATUS__FAILED:
			return;

		default:
			return;
		}
		break;
	case SII_MOD_HDMI_TX_EVENT__UPDATE_TMDS:
		SII_LIB_LOG_DEBUG1(pObj, ("UPDATE_TMDS\n"));
		if(pObj->tmdsMode)
		{
			pObj->iState = SII_MOD_HDMI_TX_EVENT__TMDS_ON;
		}
		else
		{
			pObj->iState = SII_MOD_HDMI_TX_EVENT__TMDS_OFF;
		}
		pObj->prevTmdsMode = pObj->tmdsMode;
		sUpdateHdmiMode(pObj);
		break;
#endif
	default:
		break;
	}
}
#endif
static void sVirtualIsrHandler(HdmiTxObj_t* pObj)
{
	{
	#if 0
        if (s_queue)
        {
            stHdmiEdidUpdate.pObj = pObj;
            queue_work(s_queue, &stHdmiEdidUpdate.work);
        }
    #endif    
		sPlugStatusUpdate(pObj);
	}
	
	// Clear all pending hardware TX interrupts
	if (pObj->intStat.reg0)
	{
		SiiDrvCraWrReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__INTR1, pObj->intStat.reg0); 
		pObj->intStat.reg0 = 0x00;
	}
}
#if 0
static void hdcpReset(HdmiTxObj_t* pObj)
{
	SiiDrvCraAddr_t baseAddr = sBaseAddrGet(pObj);
	SiiInst_t		craInst	 = sCraInstGet(pObj);

	SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__TPI_COPP_DATA2, BIT_MSK__TPI_COPP_DATA2__REG_TPI_HDCP_PREP_EN, CLEAR_BITS);
	SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__TPI_COPP_DATA2, BIT_MSK__TPI_COPP_DATA2__REG_CANCEL_PROT_EN, SET_BITS);
	SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__TPI_COPP_DATA2, HDCP_CTRL_MODE, CLEAR_BITS);
}
#endif
void sUpdatePrevMhlConnStatus(HdmiTxObj_t* hdmiObj)
{
	hdmiObj->prevMhlConnStatus = hdmiObj->isMhlConnected;
}


static void sSetHwTpiBit(HdmiTxObj_t* pObj)
{
	SII_LIB_LOG_DEBUG1(pObj, ("Set HW TPI bit!!\n"));
	SiiDrvCraSetBit8(sCraInstGet(pObj), sBaseAddrGet(pObj) |
		REG_ADDR__LM_DDC, BIT_MSK__LM_DDC__REG_SW_TPI_EN );
}

static void sResetTpiStateMachine(HdmiTxObj_t* pObj)
{
	SII_LIB_LOG_DEBUG1(pObj, ("Resetting TPI State Machine!!!!!\n"));

	/* workaround for hardare ddc issue with scdc module */
	SiiDrvCraClrBit8(sCraInstGet(pObj), sBaseAddrGet(pObj)| REG_ADDR__SCDC_CTL,
		BIT_MSK__SCDC_CTL__REG_SCDC_AUTO_REPLY);

	SiiDrvCraSetBit8(sCraInstGet(pObj), sBaseAddrGet(pObj)|
		REG_ADDR__TPI_HW_OPT0, BIT_MSK__TPI_HW_OPT0__REG_HW_TPI_SM_RST );
	SiiDrvCraClrBit8(sCraInstGet(pObj), sBaseAddrGet(pObj)|
		REG_ADDR__TPI_HW_OPT0, BIT_MSK__TPI_HW_OPT0__REG_HW_TPI_SM_RST );

	/* workaround for hardare ddc issue with scdc module */
//	SiiDrvCraSetBit8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__SCDC_CTL,
//		BIT_MSK__SCDC_CTL__REG_SCDC_AUTO_REPLY);
}

//-------------------------------------------------------------------------------------------------
//! @brief      Update HPD and RxSense status
//-------------------------------------------------------------------------------------------------
static void sPlugStatusUpdate(HdmiTxObj_t* pObj)
{
	bool_t          bHotPlug	= pObj->bHotPlug;
    bool_t          bRsen    	= pObj->bRsen;
	SiiInst_t		craInst		= sCraInstGet(pObj);
	//uint8_t extensions			= 0;
	uint8_t	sysStatus			= 0;
	//ddcComErr_t			ddcErr;
	//SiiEdid_t			edid;
	bool_t				bSendUpdate;
	//SiiLibEdidErrCode_t edidError;

	bSendUpdate = false;
	if(!pObj->isMhlConnected || !pObj->prevMhlConnStatus)	//FPGA Limitations
		sysStatus   = SiiDrvCraRdReg8(craInst, sBaseAddrGet(pObj) | REG_ADDR__TPI_HPD_RSEN);

	if ((sysStatus & BIT_MSK__TPI_HPD_STATE) || pObj->isMhlConnected)
	{
		SII_LIB_LOG_DEBUG1(pObj, "HOT PLUG ASSERTED:\n");

		bHotPlug	= true;
        
		sSetHwTpiBit(pObj);
		sResetTpiStateMachine(pObj);

        //DDC master reset
        sTPIReset(pObj, true);
        SiiLibTimeMilliDelay(1);
        sTPIReset(pObj, false);
        
        pObj->iState = SII_MOD_HDMI_TX_EVENT__TMDS_ON;
		//SiiLibSeqTimerStart(pObj->timerHwUpdate, TIMER_START__HW_UPDATE_50MS_WAIT, 0);
	}
	else
	{
		SII_LIB_LOG_DEBUG1(pObj, "HOT PLUG DE-ASSERTED:\n");
		bHotPlug	= false;
		
    #ifdef HDMI_HDCP_SUPPORT   
        pObj->stHdcpCaps.bHdcp14 = false;
        pObj->stHdcpCaps.bHdcp22 = false;
    #endif
		//Clear Raw EDID Buffer and Parsed EDID Buffer
		SII_MEMSET(&pObj->edid, 0, sizeof(SiiEdid_t));
		//SII_MEMSET(&pObj->parseEdid, 0, sizeof(SiiLibEdidPar_t));

		//TODO:: remove this later on when RX drivers updates TMDS mode appropriately
		//pObj->tmdsMode = SII_TMDS_MODE__NONE;
		pObj->iState = SII_MOD_HDMI_TX_EVENT__TMDS_OFF;
		//SiiLibSeqTimerStart(pObj->timerHwUpdate, TIMER_START__HW_UPDATE_50MS_WAIT, 0);
	}

    if((bHotPlug != pObj->bHotPlug) || bSendUpdate)
	{
        SII_LIB_LOG_DEBUG1(pObj, "Old hotplug state(%d), New hotplug state(%d)\n",pObj->bHotPlug, bHotPlug);
		pObj->bHotPlug = bHotPlug;
		sUpdateHotPlugRsen(pObj, SII_DRV_TX_EVENT__HOT_PLUG_CHNG);
	}

    if(sysStatus & BIT_MSK__TPI_RSEN_STATE)
    {
        bRsen = true;
    }
    else
    {
        bRsen = false;
    }
    
    if(bRsen != pObj->bRsen)
	{
        SII_LIB_LOG_DEBUG1(pObj, "Old rsen state(%d), New sen state(%d)\n", pObj->bRsen, bRsen);
		pObj->bRsen = bRsen;
		sUpdateHotPlugRsen(pObj, SII_DRV_TX_EVENT__RSEN_CHNG);
	}
}

static bool_t sI2cIsDeadlock(HdmiTxObj_t* pObj)
{
    uint8_t ddcStatus;
    ddcStatus = SiiDrvCraRdReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__DDC_STATUS);
    if (BIT_MSK__DDC_STATUS__REG_DDC_BUS_LOW == (ddcStatus & BIT_MSK__DDC_STATUS__REG_DDC_BUS_LOW))
    {
        SII_LIB_LOG_WARN(pObj, "I2C is deadlock while reading EDID\n");
       return true;
    }

    return false;
}

static bool_t sClearI2cDeadlock(HdmiTxObj_t* pObj)
{
    uint8_t timeout = 16;
    uint8_t ddcStatus = 0;
    
    while(!(BIT_MSK__DDC_MANUAL__IO_DSDA & SiiDrvCraRdReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__DDC_MANUAL)) && timeout-- > 0)
    {
        //pull scl high
        SiiDrvCraWrReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__DDC_MANUAL, 0xb1);
        SiiLibTimeMilliDelay(1);
        //pull scl low
        SiiDrvCraWrReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__DDC_MANUAL, 0xa1);
        SiiLibTimeMilliDelay(1);
    }
    
    if (timeout > 0 && (BIT_MSK__DDC_MANUAL__IO_DSDA & SiiDrvCraRdReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__DDC_MANUAL)))
    {
        //pull scl high
        SiiDrvCraWrReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__DDC_MANUAL, 0xb1);    
        SiiLibTimeMilliDelay(1);
        //pull sda low
        SiiDrvCraWrReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__DDC_MANUAL, 0x91);           
        SiiLibTimeMilliDelay(1);
        SiiDrvCraWrReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__DDC_MANUAL, 0x11);  
        ddcStatus = SiiDrvCraRdReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__DDC_STATUS);
        SII_LIB_LOG_DEBUG1(pObj, "Clear I2C deadlock success.DDC_STATUS=0x%x\n",ddcStatus);
        
        /* BIT_MSK__DDC_STATUS__REG_DDC_BUS_LOW | BIT_MSK__DDC_STATUS__REG_DDC_NO_ACK  */
        if (ddcStatus & 0x60 )
        {
            SiiDrvCraWrReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__DDC_STATUS, 0xff & 0x9f);
            SII_LIB_LOG_DEBUG1(pObj, "After success.write DDC_BUS_LOW.DDC_STATUS=0x%x\n",SiiDrvCraRdReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__DDC_STATUS));

        }
        return true;
    }
    else
    {
        SII_LIB_LOG_WARN(pObj, "Clear I2C deadlock fail\n");
        SiiDrvCraWrReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__DDC_STATUS, 0xff & 0x9f);
        SII_LIB_LOG_WARN(pObj, "After clr deadlock fail. write DDC_BUS_LOW.DDC_STATUS=0x%x\n",SiiDrvCraRdReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__DDC_STATUS));
        return false;
    }
}
//-------------------------------------------------------------------------------------------------
//! @brief      Read from downstream DDC device.
//!
//!             The function can be used for downstream EDID or HDCP DDC reading.
//!
//! @param[in]  segmentIndex - EDID segment number; 0 for HDCP DDC
//! @param[in]  regAddr      - HDCP register offset or EDID data offset
//! @param[out] pBuf         - pointer to the buffer
//! @param[in]  length       - number of bytes to read
//!
//! @return     Error code, @see ddcComErr_t
//-------------------------------------------------------------------------------------------------
static ddcComErr_t sUpdateEdid(HdmiTxObj_t* pObj, uint8_t segmentIndex, uint8_t regAddr, uint8_t *pBuf, uint16_t length)
{
	SiiDrvCraAddr_t	baseAddr	= sBaseAddrGet(pObj);
	SiiInst_t		craInst		= sCraInstGet(pObj);
	ddcComErr_t		dsDdcError  = SI_TX_DDC_ERROR_CODE_NO_ERROR;
	uint16_t        fifoSize;
	uint8_t         timeOutMs;
    bool_t          bClear = false;
	do
	{
	
	 read_again:

		if( length == 0 )
		{
			break;
		}

		if( !pBuf )
		{
            break;
        }
			
		if (!sWaitForDdcBus(pObj))
		{
			dsDdcError = SI_TX_DDC_ERROR_CODE_BUSY;
			//return dsDdcError;
			break;
		}
        SII_LIB_LOG_DEBUG1(pObj, "Before Read DDC !! HPD_RSEN=0x%02x, DDC_STATUS=0x%02x, DDC_FIFO_CNT=0x%02x\n",
                                SiiDrvCraRdReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__TPI_HPD_RSEN),
                                SiiDrvCraRdReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__DDC_STATUS),
                                SiiDrvCraRdReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__DDC_DOUT_CNT));
        SII_LIB_LOG_DEBUG1(pObj, "dsDdcError=0x%x,TPI_HW_DBG5=0x%02x, TPI_HW_DBG6=0x%02x, TPI_HW_DBG7=0x%02x\n", 
                                dsDdcError,
                                SiiDrvCraRdReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | 0x367D),
                                SiiDrvCraRdReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | 0x367E),
                                SiiDrvCraRdReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | 0x367F) );

		//Abort DDC Command
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DDC_CMD, BIT_ENUM__DDC_CMD__ABORT_TRANSACTION); 
        //enable DDC master
		SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__TPI_DDC_MASTER_EN, BIT_MSK__TPI_DDC_MASTER_EN__REG_HW_DDC_MASTER);
        //DDC target slave address
        SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DDC_ADDR, (0xFF != segmentIndex) ? 0xA0 : 0x74 );
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DDC_SEGM, (0xFF != segmentIndex) ? segmentIndex : 0x00 );
        //DDC I2C Target Offset Adress
        SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DDC_OFFSET, regAddr); 
        //number of bytes to transfer in
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DDC_DIN_CNT2, ((length >> 8) & 0xFF) );
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DDC_DIN_CNT1,  (length & 0xFF) );
        
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DDC_CMD, BIT_ENUM__DDC_CMD__CLEAR_FIFO); 
		//SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DDC_CMD, segmentIndex ? BIT_ENUM__DDC_CMD__ENHANCED_DDC_READ : BIT_ENUM__DDC_CMD__SEQUENTIAL_READ);            
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DDC_CMD, BIT_ENUM__DDC_CMD__SEQUENTIAL_READ);
		//timeOutMs = length + 3; // timeout is proportional to length
		timeOutMs = 5;

		// wait until the FIFO is filled with several bytes
		SiiLibTimeMilliDelay(2); // also makes time aligning

		do
		{
			fifoSize = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__DDC_DOUT_CNT) & BIT_MSK__DDC_DOUT_CNT__DDC_DATA_OUT_CNT;
			if( fifoSize )
			{
				timeOutMs = 5;
				// if the FIFO has some bytes
				if( fifoSize > length )
				{
					dsDdcError = SI_TX_DDC_ERROR_CODE_TX_HW;
					break;
				}
				else if( fifoSize > LEN_TPI__DDC_FIFO_SIZE )
				{
					dsDdcError = SI_TX_DDC_ERROR_CODE_LIM_EXCEED;
					break;
				}
				else
				{
				    
					// read fifo_size bytes
					SiiDrvCraFifoRead8(craInst, baseAddr | REG_ADDR__DDC_DATA, pBuf, fifoSize);
					length -= fifoSize;
					pBuf += fifoSize;
                    //pObj->edid.validLen += fifoSize;
					
					#if 0
						*pBuf = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__DDC_DATA);
						length -= 1;
						pBuf += 1;
                    #endif

				}
			}
			else if (length > 0)
			{
				SiiLibTimeMilliDelay(4); // note, the time is aligned
				timeOutMs--;
			}
		} while (length && timeOutMs);

		if( dsDdcError )
		{
            break;
        }

		if( 0 == timeOutMs )
		{
			
		    if (sI2cIsDeadlock(pObj))
            {
            	SII_LIB_LOG_WARN(pObj," DDC Dead lock !\n");
                if (sClearI2cDeadlock(pObj) && !bClear)
                {
                    bClear = true;  
                 #if 0  
                    //DDC master reset
                    sTPIReset(pObj, true);
                    SiiLibTimeMilliDelay(1);
                    sTPIReset(pObj, false);
                 #endif   
                    //continue read again
                    goto read_again;
                }
                else
                {
    			    dsDdcError = SI_TX_DDC_ERROR_CODE_TIMEOUT;
                }
            } 
            else
            {
    			dsDdcError = SI_TX_DDC_ERROR_CODE_TIMEOUT;
            }
		}
	} while(false);

	if( dsDdcError )
	{       
        SII_LIB_LOG_WARN(pObj, "After Read DDC !! HPD_RSEN=0x%02x, DDC_STATUS=0x%02x, DDC_FIFO_CNT=0x%02x\n",
                                SiiDrvCraRdReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__TPI_HPD_RSEN),
                                SiiDrvCraRdReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__DDC_STATUS),
                                SiiDrvCraRdReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__DDC_DOUT_CNT));
        SII_LIB_LOG_WARN(pObj, "dsDdcError=0x%x,TPI_HW_DBG5=0x%02x, TPI_HW_DBG6=0x%02x, TPI_HW_DBG7=0x%02x\n", 
                                dsDdcError,
                                SiiDrvCraRdReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | 0x367D),
                                SiiDrvCraRdReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | 0x367E),
                                SiiDrvCraRdReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | 0x367F) );
    
        SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DDC_CMD, BIT_ENUM__DDC_CMD__ABORT_TRANSACTION);
	}

	// Disable DDC Master
	SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__TPI_DDC_MASTER_EN, BIT_MSK__TPI_DDC_MASTER_EN__REG_HW_DDC_MASTER);        
	//SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__DDC_CMD, 0x7F);
    return dsDdcError;
}

static ddcComErr_t sUpdateEnhandEdid(HdmiTxObj_t* pObj,uint8_t u8DdcAddr, uint8_t u8SegmentId, uint8_t u8Offset, uint8_t *pu8Buf, uint16_t u16BufLen)
{
	SiiDrvCraAddr_t	baseAddr	= sBaseAddrGet(pObj);
	SiiInst_t		craInst		= sCraInstGet(pObj);
	ddcComErr_t		dsDdcError  = SI_TX_DDC_ERROR_CODE_NO_ERROR;
	uint8_t         u8timeOutMs     = 5;
    uint8_t         u8MaxReadTimes  = 2;
    uint16_t        u16FifoSize = u16BufLen;
    uint8_t         *pu8TmpBuf  = pu8Buf;

    if (!pu8Buf)
    {
        SII_LIB_LOG_ERR(pObj, "pu8Buf = null!\n");
        return SI_TX_DDC_ERROR_CODE_LIM_EXCEED;
    }
    
    SII_LIB_LOG_DEBUG1(pObj, " Before Read DDC!! u8DdcAddr=0x%02x,u8SegmentId=%u,u8Offset=%u,pu8Buf=0x%p,u16BufLen=%u \n",
                                        u8DdcAddr,u8SegmentId,u8Offset,pu8Buf,u16BufLen);
   
    while(u8MaxReadTimes--)
    {
        u16FifoSize = u16BufLen;
        pu8TmpBuf   = pu8Buf;
        dsDdcError  = SI_TX_DDC_ERROR_CODE_NO_ERROR;
    
		if (!sWaitForDdcBus(pObj))
		{
			dsDdcError = SI_TX_DDC_ERROR_CODE_BUSY;
            SII_LIB_LOG_DEBUG1(pObj," DDC BUSY!u8MaxReadTimes=%u \n",u8MaxReadTimes);
			continue;
		}
		
        SII_LIB_LOG_DEBUG1(pObj, "Before Read DDC ,u8MaxReadTimes=%u!! HPD_RSEN=0x%02x, DDC_STATUS=0x%02x, DDC_FIFO_CNT=0x%02x\n",u8MaxReadTimes,
                                SiiDrvCraRdReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__TPI_HPD_RSEN),
                                SiiDrvCraRdReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__DDC_STATUS),
                                SiiDrvCraRdReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__DDC_DOUT_CNT));
        SII_LIB_LOG_DEBUG1(pObj, "dsDdcError=0x%x,TPI_HW_DBG5=0x%02x, TPI_HW_DBG6=0x%02x, TPI_HW_DBG7=0x%02x\n", 
                                dsDdcError,
                                SiiDrvCraRdReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | 0x367D),
                                SiiDrvCraRdReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | 0x367E),
                                SiiDrvCraRdReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | 0x367F) );

		//Abort DDC Command
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DDC_CMD, BIT_ENUM__DDC_CMD__ABORT_TRANSACTION); 
        //enable DDC master
		SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__TPI_DDC_MASTER_EN, BIT_MSK__TPI_DDC_MASTER_EN__REG_HW_DDC_MASTER);
        //DDC target slave address
        SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DDC_ADDR, u8DdcAddr );
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DDC_SEGM, u8SegmentId );
        //DDC I2C Target Offset Adress
        SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DDC_OFFSET, u8Offset); 
        //number of bytes to transfer in (Enhanced DDC read )
		//SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DDC_DIN_CNT2, 0x00 );//H-8bit
		//SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DDC_DIN_CNT1, 0x80 );//L-8bit
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DDC_DIN_CNT2, ((u16FifoSize >> 8) & 0xFF) );
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DDC_DIN_CNT1,  (u16FifoSize & 0xFF) );
        //clear FIFO
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DDC_CMD, BIT_ENUM__DDC_CMD__CLEAR_FIFO); 
		//ENHANCED DDC READ
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DDC_CMD, BIT_ENUM__DDC_CMD__ENHANCED_DDC_READ );            
		//SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DDC_CMD, BIT_ENUM__DDC_CMD__SEQUENTIAL_READ);

        for (; pu8TmpBuf && u16FifoSize; u16FifoSize--,pu8TmpBuf++)
        {
            for(u8timeOutMs=5; u8timeOutMs ;u8timeOutMs--)
            {
                if(SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__DDC_DOUT_CNT) & BIT_MSK__DDC_DOUT_CNT__DDC_DATA_OUT_CNT)
                {
                    break;
                }
                else
                {
                    SiiLibTimeMilliDelay(4); // note, the time is aligned
                }
            }
            
            if( 0 == u8timeOutMs )
    		{
    			
    		    if (sI2cIsDeadlock(pObj))
                {
                    SII_LIB_LOG_WARN(pObj, "DDC dead lock ! u8MaxReadTimes=%u \n",u8MaxReadTimes);
                    sClearI2cDeadlock(pObj);
                } 
                else
                {
                    SII_LIB_LOG_WARN(pObj, "DDC Time Out ! u8MaxReadTimes=%u \n",u8MaxReadTimes);
        			dsDdcError = SI_TX_DDC_ERROR_CODE_TIMEOUT;
                }
                
                break;
    		}
            //SiiLibTimeMilliDelay(3); // also makes time aligning
            SiiDrvCraFifoRead8(craInst, baseAddr | REG_ADDR__DDC_DATA, pu8TmpBuf, 1);
        }

        if (u16FifoSize)
        {
            continue;
        }
        else
        {
            break;
        }

    }

	if( dsDdcError )
	{      
        SII_LIB_LOG_WARN(pObj, "After Read DDC !! HPD_RSEN=0x%02x, DDC_STATUS=0x%02x, DDC_FIFO_CNT=0x%02x\n",
                                SiiDrvCraRdReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__TPI_HPD_RSEN),
                                SiiDrvCraRdReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__DDC_STATUS),
                                SiiDrvCraRdReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__DDC_DOUT_CNT));
        SII_LIB_LOG_WARN(pObj, "dsDdcError=0x%x,TPI_HW_DBG5=0x%02x, TPI_HW_DBG6=0x%02x, TPI_HW_DBG7=0x%02x\n", 
                                dsDdcError,
                                SiiDrvCraRdReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | 0x367D),
                                SiiDrvCraRdReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | 0x367E),
                                SiiDrvCraRdReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | 0x367F) );
	}
	else
	{
        SII_LIB_LOG_DEBUG1(pObj," read DDC ok !! u16FifoSize=%u \n",u16FifoSize);
	}

    //Abort DDC Command
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DDC_CMD, BIT_ENUM__DDC_CMD__ABORT_TRANSACTION);
	// Disable DDC Master
	SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__TPI_DDC_MASTER_EN, BIT_MSK__TPI_DDC_MASTER_EN__REG_HW_DDC_MASTER);        

    return dsDdcError;
}
#ifdef HDMI_HDCP_SUPPORT
static ddcComErr_t sBksvRead(HdmiTxObj_t* pObj, uint8_t *pu8Buf)
{
    SiiDrvCraAddr_t	baseAddr	= sBaseAddrGet(pObj);
	SiiInst_t		craInst		= sCraInstGet(pObj);
	ddcComErr_t		dsDdcError  = SI_TX_DDC_ERROR_CODE_NO_ERROR;
	uint8_t         u8timeOutMs     = 5;
    uint8_t         u8MaxReadTimes  = 2;
    uint16_t        u16FifoSize = SII_BKSV_LIST_BYTES;
    uint8_t         *pu8TmpBuf  = pu8Buf;

    if (!pu8Buf)
    {
        SII_LIB_LOG_ERR(pObj, "pu8Buf = null!\n");
        return SI_TX_DDC_ERROR_CODE_LIM_EXCEED;
    }

    while(u8MaxReadTimes--)
    {
        u16FifoSize = SII_BKSV_LIST_BYTES;
        pu8TmpBuf   = pu8Buf;
        dsDdcError  = SI_TX_DDC_ERROR_CODE_NO_ERROR;

        SII_LIB_LOG_DEBUG1(pObj, "Before Read DDC ,u8MaxReadTimes=%u!! HPD_RSEN=0x%02x, DDC_STATUS=0x%02x, DDC_FIFO_CNT=0x%02x\n",u8MaxReadTimes,
                                SiiDrvCraRdReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__TPI_HPD_RSEN),
                                SiiDrvCraRdReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__DDC_STATUS),
                                SiiDrvCraRdReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__DDC_DOUT_CNT));
        SII_LIB_LOG_DEBUG1(pObj, "dsDdcError=0x%x,TPI_HW_DBG5=0x%02x, TPI_HW_DBG6=0x%02x, TPI_HW_DBG7=0x%02x\n", 
                                dsDdcError,
                                SiiDrvCraRdReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | 0x367D),
                                SiiDrvCraRdReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | 0x367E),
                                SiiDrvCraRdReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | 0x367F) );
        
        //Abort DDC Command
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DDC_CMD, BIT_ENUM__DDC_CMD__ABORT_TRANSACTION); 
        //enable DDC master
		SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__TPI_DDC_MASTER_EN, BIT_MSK__TPI_DDC_MASTER_EN__REG_HW_DDC_MASTER);
        //DDC target slave address
        SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DDC_ADDR, 0x74);
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DDC_SEGM, 0x00);
        //DDC I2C Target Offset Adress
        SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DDC_OFFSET, 0x00); 
        //number of bytes to transfer in (Enhanced DDC read )
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DDC_DIN_CNT2, ((u16FifoSize >> 8) & 0xFF) );
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DDC_DIN_CNT1,  (u16FifoSize & 0xFF) );
        //clear FIFO
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DDC_CMD, BIT_ENUM__DDC_CMD__CLEAR_FIFO); 
		//SEQUENTIAL DDC READ
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DDC_CMD, BIT_ENUM__DDC_CMD__SEQUENTIAL_READ );

        for (; pu8TmpBuf && u16FifoSize; u16FifoSize--, pu8TmpBuf++)
        {
            for(u8timeOutMs = 5; u8timeOutMs; u8timeOutMs--)
            {
                if(SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__DDC_DOUT_CNT) & BIT_MSK__DDC_DOUT_CNT__DDC_DATA_OUT_CNT)
                {
                    break;
                }
                else
                {
                    SiiLibTimeMilliDelay(4); // note, the time is aligned
                }
            }
            
            if( 0 == u8timeOutMs )
    		{
    			
    		    if (sI2cIsDeadlock(pObj))
                {
                    SII_LIB_LOG_WARN(pObj, "DDC dead lock ! u8MaxReadTimes=%u \n",u8MaxReadTimes);
                    sClearI2cDeadlock(pObj);
                } 
                else
                {
                    SII_LIB_LOG_WARN(pObj, "DDC Time Out ! u8MaxReadTimes=%u \n",u8MaxReadTimes);
        			dsDdcError = SI_TX_DDC_ERROR_CODE_TIMEOUT;
                }
                
                break;
    		}
            SiiDrvCraFifoRead8(craInst, baseAddr | REG_ADDR__DDC_DATA, pu8TmpBuf, 1);
        }

        if (u16FifoSize)
        {
            continue;
        }
        else
        {
            break;
        }
    }

    if( dsDdcError )
	{      
        SII_LIB_LOG_WARN(pObj, "After Read DDC !! HPD_RSEN=0x%02x, DDC_STATUS=0x%02x, DDC_FIFO_CNT=0x%02x\n",
                                SiiDrvCraRdReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__TPI_HPD_RSEN),
                                SiiDrvCraRdReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__DDC_STATUS),
                                SiiDrvCraRdReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__DDC_DOUT_CNT));
        SII_LIB_LOG_WARN(pObj, "dsDdcError=0x%x,TPI_HW_DBG5=0x%02x, TPI_HW_DBG6=0x%02x, TPI_HW_DBG7=0x%02x\n", 
                                dsDdcError,
                                SiiDrvCraRdReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | 0x367D),
                                SiiDrvCraRdReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | 0x367E),
                                SiiDrvCraRdReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | 0x367F) );
	}
	else
	{
        SII_LIB_LOG_DEBUG1(pObj," read DDC ok !! u16FifoSize=%u \n",u16FifoSize);
	}

    // Abort DDC Command
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DDC_CMD, BIT_ENUM__DDC_CMD__ABORT_TRANSACTION);
	// Disable DDC Master
	SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__TPI_DDC_MASTER_EN, BIT_MSK__TPI_DDC_MASTER_EN__REG_HW_DDC_MASTER);        

    return dsDdcError;
}

static bool_t sHdcpCheckBksvIsValid(uint8_t *pu8Bksv)
{
    uint8_t i       = 0;
    uint8_t j       = 0;
    uint8_t u8Num   = 0;
    
    for(i = 0; i < SII_BKSV_LIST_BYTES; i++)
    {
        for(j = 0; j < 8; j++)
        {
            if((pu8Bksv[i] >> j) & 0x01)
            {
                u8Num++;
            }
        }
    }

    return (20 == u8Num)? true : false;
}

static ddcComErr_t sHdcpCapabilityGet(HdmiTxObj_t* pObj)
{
    uint8_t dsHdcp_2_2_Supported    = 0;
    uint8_t u8Bksv[5]               = {0};
    
    ddcComErr_t ddcErr;
    ddcErr = SI_TX_DDC_ERROR_CODE_NO_ERROR;
    
#ifndef HDMI_BUILD_IN_BOOT
    HDMI_MUTEX_LOCK(pObj->pConfig->stDdcLock);
#endif

    /* read BKSV */
//    SiiDrvCraBlockRead8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__TPI_WR_BKSV_1, u8Bksv, SII_BKSV_LIST_BYTES);
    sBksvRead(pObj, u8Bksv);
    
    ddcErr = sUpdateEdid(pObj, 0xFF, 0x50, &dsHdcp_2_2_Supported, 1);
    if(SI_TX_DDC_ERROR_CODE_NO_ERROR != ddcErr)
    {
		// Second attempt to make sure
		SiiLibTimeMilliDelay(20);
        ddcErr = sUpdateEdid(pObj, 0xFF, 0x50, &dsHdcp_2_2_Supported, 1);
		if(SI_TX_DDC_ERROR_CODE_NO_ERROR != ddcErr)
        {
			//SiiDrvTxDdcReset();
			SII_LIB_LOG_WARN(pObj, "Error happened during HDCP2.2 Capability read 2 times (%d)!!\n\n", dsHdcp_2_2_Supported);
		}
		else
		{
			SII_LIB_LOG_DEBUG1(pObj, "HDCP2.2 capability has been read successfully(2nd)(%d) !!\n\n", dsHdcp_2_2_Supported);
		}
	}
	else
	{
		SII_LIB_LOG_DEBUG1(pObj, "HDCP2.2 capability has been read successfully(1st)(%d) !!\n\n", dsHdcp_2_2_Supported);
	}

    pObj->stHdcpCaps.bHdcp22 = (dsHdcp_2_2_Supported & BIT_MASK_HDCP_2_2_SUPPORT) ? true : false;
    pObj->stHdcpCaps.bHdcp14 = (true == sHdcpCheckBksvIsValid(u8Bksv)) ? true : false;

#ifndef HDMI_BUILD_IN_BOOT
    HDMI_MUTEX_UNLOCK(pObj->pConfig->stDdcLock);
#endif

    return ddcErr;
}
#endif  
static ddcComErr_t sEdidRead(HdmiTxObj_t* pObj)
{
    uint8_t     extensions = 0;
    ddcComErr_t ddcErr;
    SiiEdid_t	edid;
#ifndef HDMI_BUILD_IN_BOOT
    HDMI_MUTEX_LOCK(pObj->pConfig->stDdcLock);
#endif
    // read EDID
	SII_MEMSET(&edid, 0, sizeof(SiiEdid_t));
	// read block# 0
	ddcErr = sUpdateEdid(pObj, 0, 0, &(edid.b[0]), SII_EDID_BLOCK_SIZE);
	if(ddcErr == SI_TX_DDC_ERROR_CODE_NO_ERROR)
	{
        SII_LIB_LOG_DEBUG1(pObj, "EDID 1st-BLK has been read successfully!!\n");
		//find how many extension blocks are there, read them and store.
		extensions = edid.b[0x7E];

		if(extensions >= SII_EDID_TOTAL_BLOCKS)
		{
			SII_LIB_LOG_WARN(pObj, "****Warning****:: EDID: DS supports %d Extensions. But Hisilicon Supports %d extensions !!\n", extensions, (SII_EDID_TOTAL_BLOCKS-1) );
			extensions = (SII_EDID_TOTAL_BLOCKS-1);
		}
        edid.validLen = SII_EDID_BLOCK_SIZE;

		if(extensions)
        {
			ddcErr = sUpdateEdid( pObj, 0, SII_EDID_BLOCK_SIZE, &(edid.b[SII_EDID_BLOCK_SIZE]), SII_EDID_BLOCK_SIZE );
            if(ddcErr == SI_TX_DDC_ERROR_CODE_NO_ERROR)
            {
                SII_LIB_LOG_DEBUG1(pObj, "EDID Ext-BLK 1 (num=%d) has been read successfully!!\n",extensions);
                edid.validLen +=  SII_EDID_BLOCK_SIZE;
            }
            if (extensions-1)
            {
                //ddcErr = sUpdateEdid( pObj, 1, SII_EDID_BLOCK_SIZE, &(edid.b[2*SII_EDID_BLOCK_SIZE]), (2 * SII_EDID_BLOCK_SIZE) );
                sUpdateEnhandEdid( pObj, 0xa0, 1, 0,  &(edid.b[2*SII_EDID_BLOCK_SIZE]), (extensions-1)*SII_EDID_BLOCK_SIZE );
                if(ddcErr == SI_TX_DDC_ERROR_CODE_NO_ERROR)
                {
                    SII_LIB_LOG_DEBUG1(pObj, "EDID Ext-BLK 2~3 (num=%d) has been read successfully!!\n",extensions);
                    edid.validLen += (extensions-1) * SII_EDID_BLOCK_SIZE;
                }
            }
        }      
    }

	if(ddcErr == SI_TX_DDC_ERROR_CODE_NO_ERROR)
	{
		//SII_LIB_LOG_DEBUG1(pObj, "EDID has been read no error!!\n");
		//if( SII_MEMCMP(&pObj->edid, &edid, sizeof(SiiEdid_t)) )
		{
			pObj->edid = edid;
		}
    }

#ifndef HDMI_BUILD_IN_BOOT
    HDMI_MUTEX_UNLOCK(pObj->pConfig->stDdcLock);
#endif
    return ddcErr;
}
//-------------------------------------------------------------------------------------------------
//! @brief      Wait for DS DDC operation to finish.
//-------------------------------------------------------------------------------------------------
static bool_t sWaitForDdcBus(HdmiTxObj_t* pObj)
{
	uint8_t          val;
	SiiDrvCraAddr_t  baseAddr	= sBaseAddrGet(pObj);
	SiiInst_t		 craInst	= sCraInstGet(pObj);
	uint8_t          time_out   = LEN_TPI__DDC_FIFO_SIZE + 1;

	// time_out is time in ms, which is proportional to the FIFO size.
	// Since the time required to transmit one byte is 100 uS, the time_out
	// is as much as 10 times longer. +1 is additional time due to SI_Sleep() function precision
	// allows error +0-1 ms.
	do
	{
		val = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__DDC_STATUS) & BIT_MSK__DDC_STATUS__DDC_I2C_IN_PROG;

		if(0 == val)
		{ 
			return true;
		}
		else
		{
			SiiLibTimeMilliDelay(1);
		}
	}
	while(--time_out);

	return false;
}
#if 0
//-------------------------------------------------------------------------------------------------
//! @brief      Parse Edid after reading edid.
//!
//!             The function can be used for parsing downstream EDID.
//!
//! @return     Error code, @see SiiLibEdidErrCode_t
//-------------------------------------------------------------------------------------------------
static SiiLibEdidErrCode_t sParseEdid(HdmiTxObj_t* pObj)
{
	return SiiLibEdidParse(&pObj->parseEdid, (const SiiLibEdidRaw_t*)&pObj->edid);
}
#endif
#endif

static void sApplyInfoFrame(HdmiTxObj_t* pObj, uint8_t bApply)
{   
	SiiDrvCraAddr_t baseAddr = sBaseAddrGet(pObj);
	SiiInst_t		craInst	 = sCraInstGet(pObj);

	if(bApply)
	{
		//start inforframes
		SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__TPI_INFO_EN, 0xC0, 0xC0);
	}
	else
	{
		//stop inforframes
		//SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__TPI_INFO_EN, 0xC0, 0x00);
		SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__TPI_INFO_EN, BIT_MSK__TPI_INFO_EN__REG_TPI_INFO_RPT);
		SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__TPI_INFO_EN, BIT_MSK__TPI_INFO_EN__REG_TPI_INFO_EN);
	}	
}

static void sInfoframesSet(HdmiTxObj_t* pObj, SiiInfoFrameId_t ifId)
{
    SiiDrvCraAddr_t baseAddr = sBaseAddrGet(pObj);
    SiiInst_t		craInst	 = sCraInstGet(pObj);
    
    switch (ifId)
    {
        case SII_INFO_FRAME_ID__AVI:
            //AVI InfoFrame
            SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__TPI_INFO_FSEL, BIT_MSK__TPI_INFO_FSEL__REG_TPI_INFO_SEL, BIT_ENUM__TPI_INFO_FSEL__AVI); 
            // InfoFrame Data Update
            SiiDrvCraBlockWrite8(craInst, baseAddr | REG_ADDR__TPI_INFO_B0, (uint8_t*)pObj->ifAvi.b, LEN_TPI__IF_BUFFER_LENGTH);
            //make the changes effective
            sApplyInfoFrame(pObj, pObj->bIfOnAvi);

            break;
       case SII_INFO_FRAME_ID__AUDIO:
          	//AUDIO InfoFrame
        	SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__TPI_INFO_FSEL, BIT_MSK__TPI_INFO_FSEL__REG_TPI_INFO_SEL, BIT_ENUM__TPI_INFO_FSEL__AUDIO);
        	// InfoFrame Data Update
        	SiiDrvCraBlockWrite8(craInst, baseAddr | REG_ADDR__TPI_INFO_B0, (uint8_t*)pObj->ifAudio.b, LEN_TPI__IF_BUFFER_LENGTH);
        	//make the changes effective
        	sApplyInfoFrame(pObj, pObj->bIfOnAudio);      

            break;

    case SII_INFO_FRAME_ID__VS:
        	//VSIF InfoFrame
        	SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__TPI_INFO_FSEL, BIT_MSK__TPI_INFO_FSEL__REG_TPI_INFO_SEL, BIT_ENUM__TPI_INFO_FSEL__VSIF);
        	// InfoFrame Data Update
        	SiiDrvCraBlockWrite8(craInst, baseAddr | REG_ADDR__TPI_INFO_B0, (uint8_t*)pObj->ifVs.b, LEN_TPI__IF_BUFFER_LENGTH);
        	//make the changes effective
        	sApplyInfoFrame(pObj, pObj->bIfOnVs);    
    
            break;

    case SII_INFO_FRAME_ID__SPD:
        	//SPD InfoFrame
        	SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__TPI_INFO_FSEL, BIT_MSK__TPI_INFO_FSEL__REG_TPI_INFO_SEL, BIT_ENUM__TPI_INFO_FSEL__SPD); 
        	// InfoFrame Data Update
        	SiiDrvCraBlockWrite8(craInst, baseAddr | REG_ADDR__TPI_INFO_B0, (uint8_t*)pObj->ifSpd.b, LEN_TPI__IF_BUFFER_LENGTH);
        	//make the changes effective
        	sApplyInfoFrame(pObj, pObj->bIfOnSpd);   
        
            break;
    case SII_INFO_FRAME_ID__GBD:
            //GBD InfoFrame
            SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__TPI_INFO_FSEL, BIT_MSK__TPI_INFO_FSEL__REG_TPI_INFO_SEL, BIT_ENUM__TPI_INFO_FSEL__GBD); 
            // InfoFrame Data Update
            SiiDrvCraBlockWrite8(craInst, baseAddr | REG_ADDR__TPI_INFO_B0, (uint8_t*)pObj->ifGbd.b, LEN_TPI__IF_BUFFER_LENGTH);
            //make the changes effective
            sApplyInfoFrame(pObj, pObj->bIfOnGbd); 

            break;
    case SII_INFO_FRAME_ID__MPEG:
        	//MPEG InfoFrame
        	SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__TPI_INFO_FSEL, BIT_MSK__TPI_INFO_FSEL__REG_TPI_INFO_SEL, BIT_ENUM__TPI_INFO_FSEL__MPEG); 
        	// InfoFrame Data Update
        	SiiDrvCraBlockWrite8(craInst, baseAddr | REG_ADDR__TPI_INFO_B0, (uint8_t*)pObj->ifMpeg.b, LEN_TPI__IF_BUFFER_LENGTH);
        	//make the changes effective
        	sApplyInfoFrame(pObj, pObj->bIfOnMpeg); 
        
            break;
            
#ifdef HDMI_HDR_SUPPORT
    case SII_INFO_FRAME_ID__DRM:
            //DRM InfoFrame
        	SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__TPI_INFO_FSEL, BIT_MSK__TPI_INFO_FSEL__REG_TPI_INFO_SEL, BIT_ENUM__TPI_INFO_FSEL__GEN3); 
        	// InfoFrame Data Update
        	SiiDrvCraBlockWrite8(craInst, baseAddr | REG_ADDR__TPI_INFO_B0, (uint8_t*)pObj->ifDrm.b, LEN_TPI__IF_BUFFER_LENGTH);
        	//make the changes effective
        	sApplyInfoFrame(pObj, pObj->bIfOnDrm);
            break;
#endif

    default:
        break;

    }

/*
// TODO:: We need to understand how to use these buffers, they seem to create problems with DDC line

// use GEN1 buffer for ISRC
SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__TPI_INFO_FSEL, BIT_MSK__TPI_INFO_FSEL__REG_TPI_INFO_SEL, BIT_ENUM__TPI_INFO_FSEL__GEN1); 

// InfoFrame Data Update
SiiDrvCraBlockWrite8(craInst, baseAddr | REG_ADDR__TPI_INFO_B0, (uint8_t*)pObj->ifIsrc.b, LEN_TPI__IF_BUFFER_LENGTH);

//make the changes effective
sApplyInfoFrame(pObj, pObj->bIfOnIsrc);

// use GEN1 buffer for ISRC2
SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__TPI_INFO_FSEL, BIT_MSK__TPI_INFO_FSEL__REG_TPI_INFO_SEL, BIT_ENUM__TPI_INFO_FSEL__GEN2); 

// InfoFrame Data Update
SiiDrvCraBlockWrite8(craInst, baseAddr | REG_ADDR__TPI_INFO_B0, (uint8_t*)pObj->ifIsrc2.b, LEN_TPI__IF_BUFFER_LENGTH);

//make the changes effective
sApplyInfoFrame(pObj, pObj->bIfOnIsrc2);

// use GEN3 buffer for ACP
txCraTpiAipPutBit8(pObj->priBaseAddr, REG_ADDR__TPI_INFO_FSEL, BIT_MSK__TPI_INFO_FSEL__REG_TPI_INFO_SEL, BIT_ENUM__TPI_INFO_FSEL__GEN3); 

// InfoFrame Data Update
SiiDrvCraBlockWrite8(craInst, baseAddr | REG_ADDR__TPI_INFO_B0, (uint8_t*)pObj->ifAcp.b, LEN_TPI__IF_BUFFER_LENGTH);

//make the changes effective
sApplyInfoFrame(pObj, pObj->bIfOnAcp);
*/
}


static void sInfoframeTypeSet(HdmiTxObj_t* pObj, SiiInfoFrame_t *pInfoFrame)
{
#if defined(CONFIG_HDMI_STB_SDK)
	uint8_t *ptr = (uint8_t*)pInfoFrame->b;
	int i = 0;
    int len = SII_INFOFRAME_MAX_LEN;
#endif
    uint8_t offset = 0;
	char tempBuffer[SII_INFOFRAME_MAX_LEN*5];
    memset(tempBuffer, 0, SII_INFOFRAME_MAX_LEN*5);
    offset += SII_SNPRINTF((tempBuffer+offset,SII_INFOFRAME_MAX_LEN*5,"%c", '\n'));
    
	switch(pInfoFrame->ifId)
	{
	case SII_INFO_FRAME_ID__AVI: 
		SII_MEMCPY(&pObj->ifAvi, pInfoFrame, sizeof(SiiInfoFrame_t));
		//SII_LIB_LOG_DEBUG2("IF-AVI	:\n");
        offset += SII_SNPRINTF((tempBuffer+offset,SII_INFOFRAME_MAX_LEN*5,"%s", "IF-AVI	:\n"));
		break;

	case SII_INFO_FRAME_ID__AUDIO: 
		SII_MEMCPY(&pObj->ifAudio, pInfoFrame, sizeof(pObj->ifAudio)); 
        //SII_LIB_LOG_DEBUG2("IF-AUDIO	:\n");
        offset += SII_SNPRINTF((tempBuffer+offset,SII_INFOFRAME_MAX_LEN*5,"%s", "IF-AUDIO	:\n"));
		break;

	case SII_INFO_FRAME_ID__VS: 
		SII_MEMCPY(&pObj->ifVs, pInfoFrame, sizeof(pObj->ifVs));
		//SII_LIB_LOG_DEBUG2("IF-VS	:\n");
        offset += SII_SNPRINTF((tempBuffer+offset,SII_INFOFRAME_MAX_LEN*5,"%s", "IF-VS	:\n"));
		break;

	case SII_INFO_FRAME_ID__SPD: 
		SII_MEMCPY(&pObj->ifSpd, pInfoFrame, sizeof(pObj->ifSpd));
		//SII_LIB_LOG_DEBUG2("IF-SPD	:\n");
        offset += SII_SNPRINTF((tempBuffer+offset,SII_INFOFRAME_MAX_LEN*5,"%s", "IF-SPD	:\n"));
		break;

	case SII_INFO_FRAME_ID__GBD: 
		SII_MEMCPY(&pObj->ifGbd, pInfoFrame, sizeof(pObj->ifGbd));
		//SII_LIB_LOG_DEBUG2("IF-GBD	:\n");
        offset += SII_SNPRINTF((tempBuffer+offset,SII_INFOFRAME_MAX_LEN*5,"%s", "IF-GBD	:\n"));
		break;

	case SII_INFO_FRAME_ID__MPEG: 
		SII_MEMCPY(&pObj->ifMpeg, pInfoFrame, sizeof(pObj->ifMpeg));
		//SII_LIB_LOG_DEBUG2("IF-MPEG	:\n");
        offset += SII_SNPRINTF((tempBuffer+offset,SII_INFOFRAME_MAX_LEN*5,"%s", "IF-MPEG	:\n"));
		break;

	case SII_INFO_FRAME_ID__ISRC: 
		SII_MEMCPY(&pObj->ifIsrc, pInfoFrame, sizeof(pObj->ifIsrc));
		//SII_LIB_LOG_DEBUG2("IF-ISRC	:\n");
        offset += SII_SNPRINTF((tempBuffer+offset,SII_INFOFRAME_MAX_LEN*5,"%s", "IF-ISRC	:\n"));
		break;

	case SII_INFO_FRAME_ID__ISRC2: 
		SII_MEMCPY(&pObj->ifIsrc2, pInfoFrame, sizeof(pObj->ifIsrc2));
		//SII_LIB_LOG_DEBUG2("IF-ISRC2	:\n");
        offset += SII_SNPRINTF((tempBuffer+offset,SII_INFOFRAME_MAX_LEN*5,"%s", "IF-ISRC2	:\n"));
		break;

	case SII_INFO_FRAME_ID__ACP: 
		SII_MEMCPY(&pObj->ifAcp, pInfoFrame, sizeof(pObj->ifAcp));
		//SII_LIB_LOG_DEBUG2("IF-ACP	:\n");
        offset += SII_SNPRINTF((tempBuffer+offset,SII_INFOFRAME_MAX_LEN*5,"%s", "IF-ACP	:\n"));
		break;
        
#ifdef HDMI_HDR_SUPPORT
    case SII_INFO_FRAME_ID__DRM:
        SII_MEMCPY(&pObj->ifDrm, pInfoFrame, sizeof(pObj->ifDrm));
        offset += SII_SNPRINTF((tempBuffer+offset,SII_INFOFRAME_MAX_LEN*5,"%s", "IF-DRM	:\n"));
        break;
#endif
	default : 
		break;
	}

#if defined(CONFIG_HDMI_STB_SDK)
    while(len--)
    {
        offset += SII_SNPRINTF((tempBuffer+offset,SII_INFOFRAME_MAX_LEN*5,"%02x ", *ptr++));
        i++;
        if(++i == 0x10)
		{
			offset += SII_SNPRINTF((tempBuffer+offset,SII_INFOFRAME_MAX_LEN*5,"%c", '\n'));
			i = 0;
		}
    }
    offset += SII_SNPRINTF((tempBuffer+offset,SII_INFOFRAME_MAX_LEN*5,"%c", '\n'));
#endif    

    SII_LIB_LOG_DEBUG2(tempBuffer);


	//sTxLog((uint8_t*)ptr, SII_INFOFRAME_MAX_LEN);
    //sInfoframesSet(pObj); 
	//SiiLibSeqTimerStart(pObj->timerHwUpdate, TIMER_START__HW_UPDATE, 0);
}

static void sInfoframeOnOffSet(HdmiTxObj_t* pObj, SiiInfoFrameId_t ifId, uint8_t onOff)
{
	switch(ifId)
	{
	case SII_INFO_FRAME_ID__AVI: 
		pObj->bIfOnAvi = onOff; 
		break;

	case SII_INFO_FRAME_ID__AUDIO  : 
		pObj->bIfOnAudio = onOff; 
		break;

	case SII_INFO_FRAME_ID__VS     : 
		pObj->bIfOnVs = onOff; 
		break;

	case SII_INFO_FRAME_ID__SPD    : 
		pObj->bIfOnSpd = onOff; 
		break;

	case SII_INFO_FRAME_ID__GBD    : 
		pObj->bIfOnGbd = onOff; 
		break;

	case SII_INFO_FRAME_ID__ISRC   : 
		pObj->bIfOnIsrc = onOff; 
		break;

	case SII_INFO_FRAME_ID__ISRC2  : 
		pObj->bIfOnIsrc2 = onOff; 
		break;

	case SII_INFO_FRAME_ID__ACP    : 
		pObj->bIfOnAcp = onOff; 
		break;

	case SII_INFO_FRAME_ID__MPEG   : 
		pObj->bIfOnMpeg = onOff; 
		break;
        
#ifdef HDMI_HDR_SUPPORT
    case SII_INFO_FRAME_ID__DRM    :
        pObj->bIfOnDrm = onOff;
        break;
#endif

	default : break;
	}
    sInfoframesSet(pObj, ifId); 
	//SiiLibSeqTimerStart(pObj->timerHwUpdate, TIMER_START__HW_UPDATE, 0);
}
#ifndef HDMI_BUILD_IN_BOOT
static void sInfoframeTypeGet(HdmiTxObj_t* pObj, SiiInfoFrame_t *pInfoFrame)
{
	SiiDrvCraAddr_t baseAddr = sBaseAddrGet(pObj);
	SiiInst_t		craInst	 = sCraInstGet(pObj);
    uint8_t infoframeSel = 0;
    
    switch (pInfoFrame->ifId)
    {
	case SII_INFO_FRAME_ID__AVI: 
		infoframeSel = BIT_ENUM__TPI_INFO_FSEL__AVI; 
		break;

	case SII_INFO_FRAME_ID__AUDIO  : 
		infoframeSel = BIT_ENUM__TPI_INFO_FSEL__AUDIO; 
		break;

	case SII_INFO_FRAME_ID__VS     : 
		infoframeSel = BIT_ENUM__TPI_INFO_FSEL__VSIF; 
		break;

	case SII_INFO_FRAME_ID__SPD    : 
		infoframeSel = BIT_ENUM__TPI_INFO_FSEL__SPD; 
		break;

	case SII_INFO_FRAME_ID__GBD    : 
		infoframeSel = BIT_ENUM__TPI_INFO_FSEL__GBD; 
		break;

	case SII_INFO_FRAME_ID__ISRC   : 
		infoframeSel = BIT_ENUM__TPI_INFO_FSEL__GEN1; 
		break;

	case SII_INFO_FRAME_ID__ISRC2  : 
		infoframeSel = BIT_ENUM__TPI_INFO_FSEL__GEN2; 
		break;

	case SII_INFO_FRAME_ID__ACP    : 
		infoframeSel = BIT_ENUM__TPI_INFO_FSEL__GEN3; 
		break;

	case SII_INFO_FRAME_ID__MPEG   : 
		infoframeSel = BIT_ENUM__TPI_INFO_FSEL__MPEG; 
		break;
#ifdef HDMI_HDR_SUPPORT
    case SII_INFO_FRAME_ID__DRM    :
        infoframeSel = BIT_ENUM__TPI_INFO_FSEL__GEN3;
        break;
#endif  

	default : 
        break;
    }

	SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__TPI_INFO_FSEL, BIT_MSK__TPI_INFO_FSEL__REG_TPI_INFO_SEL, infoframeSel);
	// InfoFrame Data Read
	SiiDrvCraBlockRead8(craInst, baseAddr | REG_ADDR__TPI_INFO_B0, (uint8_t*)pInfoFrame->b, SII_INFOFRAME_MAX_LEN);

}

static void sInfoframeOnOffGet(HdmiTxObj_t* pObj, SiiInfoFrameId_t ifId, uint8_t* onOff)
{
	SiiDrvCraAddr_t baseAddr = sBaseAddrGet(pObj);
	SiiInst_t		craInst	 = sCraInstGet(pObj);
    uint8_t infoframeSel = 0;
    
	switch(ifId)
	{
	case SII_INFO_FRAME_ID__AVI: 
		infoframeSel = BIT_ENUM__TPI_INFO_FSEL__AVI; 
		break;

	case SII_INFO_FRAME_ID__AUDIO  : 
		infoframeSel = BIT_ENUM__TPI_INFO_FSEL__AUDIO; 
		break;

	case SII_INFO_FRAME_ID__VS     : 
		infoframeSel = BIT_ENUM__TPI_INFO_FSEL__VSIF; 
		break;

	case SII_INFO_FRAME_ID__SPD    : 
		infoframeSel = BIT_ENUM__TPI_INFO_FSEL__SPD; 
		break;

	case SII_INFO_FRAME_ID__GBD    : 
		infoframeSel = BIT_ENUM__TPI_INFO_FSEL__GBD; 
		break;

	case SII_INFO_FRAME_ID__ISRC   : 
		infoframeSel = BIT_ENUM__TPI_INFO_FSEL__GEN1; 
		break;

	case SII_INFO_FRAME_ID__ISRC2  : 
		infoframeSel = BIT_ENUM__TPI_INFO_FSEL__GEN2; 
		break;

	case SII_INFO_FRAME_ID__ACP    : 
		infoframeSel = BIT_ENUM__TPI_INFO_FSEL__GEN3; 
		break;

	case SII_INFO_FRAME_ID__MPEG   : 
		infoframeSel = BIT_ENUM__TPI_INFO_FSEL__MPEG; 
		break;
#ifdef HDMI_HDR_SUPPORT
    case SII_INFO_FRAME_ID__DRM    :
        infoframeSel = BIT_ENUM__TPI_INFO_FSEL__GEN3;
        break;
#endif        
	default : break;
	}

	SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__TPI_INFO_FSEL, BIT_MSK__TPI_INFO_FSEL__REG_TPI_INFO_SEL, infoframeSel);
    if ((SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__TPI_INFO_EN) & 0xC0) == 0xC0)
    {
        *onOff = true;
    }
    else
    {
        *onOff = false;
    }
}

#if 0
static void sUpdateInfoframes(HdmiTxObj_t* pObj)
{
	sInfoframesSet(pObj); 
}
#endif
static void sAudioformatSet(HdmiTxObj_t* pObj)
{
    uint8_t         u8_reg_asrc = 0;
    uint8_t u8_n_value = 0;

	SiiDrvCraAddr_t baseAddr = sBaseAddrGet(pObj);
	SiiInst_t		craInst	 = sCraInstGet(pObj);

    //sample frequency
	switch(pObj->audioFormat.audioFs)
	{
	case SII_AUDIO_FS__22_05KHZ: 
		SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__TPI_AUD_FS, 0x0F, 0x04); 
		break;

	case SII_AUDIO_FS__24KHZ: 
		SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__TPI_AUD_FS, 0x0F, 0x06); 
		break;

	case SII_AUDIO_FS__32KHZ: 
		SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__TPI_AUD_FS, 0x0F, 0x03); 
		break;

	case SII_AUDIO_FS__44_1KHZ: 
		SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__TPI_AUD_FS, 0x0F, 0x00); 
		break;

	case SII_AUDIO_FS__48KHZ: 
		SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__TPI_AUD_FS, 0x0F, 0x02); 
		break;

	case SII_AUDIO_FS__88_2KHZ: 
		SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__TPI_AUD_FS, 0x0F, 0x08);  
		break;

	case SII_AUDIO_FS__96KHZ: 
		SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__TPI_AUD_FS, 0x0F, 0x0A); 
		break;

	case SII_AUDIO_FS__176_4KHZ: 
		SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__TPI_AUD_FS, 0x0F, 0x0C); 
		break;

	case SII_AUDIO_FS__192KHZ: 
		SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__TPI_AUD_FS, 0x0F, 0x0E); 
		break;

	case SII_AUDIO_FS__768KHZ: 
		SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__TPI_AUD_FS, 0x0F, 0x09); 
		break;			

	default : 
		break;
	}

	switch(pObj->audioFormat.layout1)
	{
	case AUDIO_FORMAT__2CH: 
		//txCraTpiAipClrBit8(baseAddr, REG_ADDR__I2S_CHST3, BIT_MSK__AUDP_TXCTRL__REG_LAYOUT); 
		SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__AUDP_TXCTRL, BIT_MSK__AUDP_TXCTRL__REG_LAYOUT);
		break;
	case AUDIO_FORMAT__3CH: 
	case AUDIO_FORMAT__4CH: 
	case AUDIO_FORMAT__5CH: 
	case AUDIO_FORMAT__6CH: 
	case AUDIO_FORMAT__7CH: 
	case AUDIO_FORMAT__8CH: 
		//txCraTpiAipSetBit8(baseAddr, REG_ADDR__I2S_CHST3, BIT_MSK__AUDP_TXCTRL__REG_LAYOUT); 
		SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__AUDP_TXCTRL, BIT_MSK__AUDP_TXCTRL__REG_LAYOUT);        
		break;
	default : 
		break;
	}

    u8_reg_asrc = 0x10;

	if(pObj->audioFormat.i2s)
	{
		switch(pObj->audioFormat.layout1)
		{
		case AUDIO_FORMAT__2CH: 
			SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__AUD_MODE, 0xFF, SII_BIT4);
			break;
		case AUDIO_FORMAT__3CH: 
		case AUDIO_FORMAT__4CH: 
			SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__AUD_MODE, 0xFF, (SII_BIT5 |SII_BIT4) );
			break;
		case AUDIO_FORMAT__5CH: 
		case AUDIO_FORMAT__6CH: 
			SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__AUD_MODE, 0xFF, (SII_BIT6 |SII_BIT5 |SII_BIT4) );
			break;
		case AUDIO_FORMAT__7CH: 
		case AUDIO_FORMAT__8CH: 
			SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__AUD_MODE, 0xFF, (SII_BIT7 |SII_BIT6 |SII_BIT5 |SII_BIT4) );
			break;
		default : 
			break;
		}
		
	}
	else if(pObj->audioFormat.dsd)
	{
		SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__AUD_MODE, 0xFF, SII_BIT3);                // enable dsd
	}
	else if(pObj->audioFormat.hbrA)
	{
		SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__AUDP_TXCTRL, BIT_MSK__AUDP_TXCTRL__REG_LAYOUT);
		SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__AUD_MODE, 0xFF, SII_BIT2);                // enable hbrA
		u8_reg_asrc = 0x00;
	}
	else if(pObj->audioFormat.spdif)
	{
		SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__AUD_MODE, 0xFF, SII_BIT1);                // enable spdif
	}
	else
	{
		SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__AUD_MODE, 0xFF, 0x00);                    // enable spdiff/pkt pass thru
	}

    SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__ASRC, BIT_MSK__ASRC__REG_HBR_SPR_MASK, u8_reg_asrc);

	// Enable/Disable Down sampling
	if(pObj->audioFormat.downSample)
	{
		SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__TPI_DOWN_SMPL_CTRL, BIT_MSK__TPI_DOWN_SMPL_CTRL__REG_TPI_AUD_HNDL, BIT_ENUM__TPI_DOWN_SMPL_CTRL__AUDIO_DOWNSAMP_EN );
	}
	else
	{
		//SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__TPI_DOWN_SMPL_CTRL, 0x03, CLEAR_BITS );
		SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__TPI_DOWN_SMPL_CTRL, BIT_MSK__TPI_DOWN_SMPL_CTRL__REG_TPI_AUD_HNDL, 0x3 );
    }

	// Set N value
	u8_n_value = pObj->audioFormat.n & 0xff;
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__N_SVAL1, u8_n_value);

	u8_n_value = (pObj->audioFormat.n >> 8) & 0xff;
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__N_SVAL2, u8_n_value);

	u8_n_value = (pObj->audioFormat.n >> 16) & 0x0f;
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__N_SVAL3, u8_n_value);
}
#if 0
static void sUpdateAudioformat(HdmiTxObj_t* pObj)
{
	sAudioformatSet(pObj);
}
#endif
static void sChannelStateSet(HdmiTxObj_t* pObj)
{
	SiiDrvCraAddr_t baseAddr = sBaseAddrGet(pObj);
	SiiInst_t		craInst	 = sCraInstGet(pObj);

	SiiDrvCraWrReg8(craInst, baseAddr |  REG_ADDR__I2S_CHST0, pObj->channelStatus.i2s_chst0);
	SiiDrvCraWrReg8(craInst, baseAddr |  REG_ADDR__I2S_CHST1, pObj->channelStatus.i2s_chst1);
	SiiDrvCraWrReg8(craInst, baseAddr |  REG_ADDR__I2S_CHST2, pObj->channelStatus.i2s_chst2);
	SiiDrvCraWrReg8(craInst, baseAddr |  REG_ADDR__I2S_CHST3, pObj->channelStatus.i2s_chst3);
	SiiDrvCraWrReg8(craInst, baseAddr |  REG_ADDR__I2S_CHST4, pObj->channelStatus.i2s_chst4);
	SiiDrvCraWrReg8(craInst, baseAddr |  REG_ADDR__I2S_CHST6, pObj->channelStatus.i2s_chst5);
	SiiDrvCraWrReg8(craInst, baseAddr |  REG_ADDR__I2S_CHST7, pObj->channelStatus.i2s_chst6); 
}
#if 0
static void sUpdateChannelState(HdmiTxObj_t* pObj)
{
	sChannelStateSet(pObj);
}
#endif
#if 0
//Eanble MHL1/2 or MHL3 AVLINK based on mhl3_connected flag.
static void sAvlinkEnable(HdmiTxObj_t *hdmiObj, bool_t bEnable)
{
	SiiDrvCraAddr_t	baseAddr = sBaseAddrGet(hdmiObj);
	SiiInst_t		craInst  = sCraInstGet(hdmiObj);
	bool_t is_mhl3_connected = hdmiObj->isMhl3Connected;

	if(bEnable)
	{
		SII_LIB_LOG_DEBUG1(hdmiObj, ("Enabling AVLINK\n"));
		if(is_mhl3_connected)
		{
			SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__FUNC_SEL			, 0x02);	// Enable MHL3 for TX
			SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__SW_RST3			, 0x00);
		}
		else
		{
			SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__FUNC_SEL			, 0x00);
		}
		SiiDrvCraWrReg8(craInst, baseAddr | TXPHY_TOP_CTL0			, 0x20);
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
		SII_LIB_LOG_DEBUG1(hdmiObj, ("Disabling AVLINK\n"));
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__FUNC_SEL			, 0x1);	// Back to HDMI Configuration
		SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__HDMI2MHL1_CTL3	, 0x08);
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__MHL_MISC_CTL1		, 0x0);

		SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__HDMI2MHL3_CTRL	, 0x0);
		SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__HDMI2MHL3_P0CTRL	, 0x0);
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDMI2MHL3_SRP0	, 0x0);
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDMI2MHL3_SRP1	, 0x0);
		SiiDrvCraWrReg8(craInst, baseAddr | TXPHY_TOP_CTL0		, 0x90); //Back to HDMI Configuration.
	}

}
#endif
#endif

static void sPutTMDSOnOff(HdmiTxObj_t* pObj, bool_t onOff)
{
	SiiDrvCraAddr_t baseAddr = sBaseAddrGet(pObj);
	SiiInst_t		craInst	 = sCraInstGet(pObj);

    /* 20150617: l232354, fixed the snmsung UA65F900AJ TV display issue(1080P_30, 2160P_30), have two things must do for this TV.
       one: must clr the OE regs before open.
       two: must sleep between the TXPHY_TMDS_CTL and TXPHY_DP_CTL0.
    */
    if(onOff)
    {
        SiiDrvCraSetBit8(craInst, baseAddr | TXPHY_TMDS_CTL, BIT_MSK__TXPHY_TMDS_CTL_REG_TMDS_OE);
        SiiLibTimeMilliDelay(1);
        SiiDrvCraSetBit8(craInst, baseAddr | TXPHY_DP_CTL0, BIT_MSK__TXPHY_DP_CTL0__REG_DP_OE | BIT_MSK__TXPHY_DP_CTL0__REG_TX_OE);
    }
    else
    {
        SiiDrvCraClrBit8(craInst, baseAddr | TXPHY_TMDS_CTL, BIT_MSK__TXPHY_TMDS_CTL_REG_TMDS_OE);
        SiiDrvCraClrBit8(craInst, baseAddr | TXPHY_DP_CTL0, BIT_MSK__TXPHY_DP_CTL0__REG_DP_OE | BIT_MSK__TXPHY_DP_CTL0__REG_TX_OE);
    }

}

static void sPhyConfig(HdmiTxObj_t* pObj, SiiDrvPhyPara_t *pst_phy_param)
{
	SiiDrvCraAddr_t baseAddr = sBaseAddrGet(pObj);
	SiiInst_t		craInst	 = sCraInstGet(pObj);

    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__PHY_DATA_SWING, pst_phy_param->u8_data_swing);       //data-swing
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__PHY_CLK_SWING, pst_phy_param->u8_clk_swing);        //CLK-swing
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__PHY_SRC_TERMINATION, pst_phy_param->u8_src_termination);  //source termination
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__PHY_NVB_VALUE, pst_phy_param->u8_drv_vnb_value);    //Tx driver vnb value setting	
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__PHY_CLK_FINE_ADJUST, pst_phy_param->u8_clk_src_fine_djust);// Fine adjust clock source termination
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__PHY_PLL_BAND, pst_phy_param->u8_pll_band);         // PLL bandwidth
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__PHY_PLL_LOOP, pst_phy_param->u8_pll_loop);         // PLL loop filter
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__PHY_PLL_VCO, pst_phy_param->u8_pll_vco);          // PLL VCO bias

    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__PHY_PLL_PI, pst_phy_param->u8_pll_pi);           // PLL PI
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__PHY_BRG_BIAS, pst_phy_param->u8_brg_bias);         // BGR and Bias value
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__PHY_DATA_FINE_ADJUST, pst_phy_param->u8_data_fine_djust);  // Fine adjust data source termination
    
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__PHY_DATA_DRV_CTRL, pst_phy_param->u8_data_drv_ctrl);    // Data driver control
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__PHY_PLL_LDO, pst_phy_param->u8_pll_ldo);          // PLL LDO control
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__PHY_PLL_ZONE, pst_phy_param->u8_pll_zone);         // PLL zone calibration

    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__PHY_PLL_CHARGE, pst_phy_param->u8_pll_charge);       // PLL charge pump
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__PHY_IN_BRG, pst_phy_param->u8_in_brg);           // in BGR resistor
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__PHY_TX_REG_ZONE, pst_phy_param->u8_tx_reg_zone);      // tx_reg_zone

    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__PHY_RISE_TIME, pst_phy_param->u8_rise_time);        // rise time
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__PHY_FALL_TIME, pst_phy_param->u8_fall_time);        // fall time
#if 0
    SII_LIB_LOG_DEBUG1("0x%02x\n",pst_phy_param->u8_dataS_swing);
    SII_LIB_LOG_DEBUG1("0x%02x\n",pst_phy_param->u8_clk_swing);
    SII_LIB_LOG_DEBUG1("0x%02x\n",pst_phy_param->u8_src_termination);
    SII_LIB_LOG_DEBUG1("0x%02x\n",pst_phy_param->u8_drv_vnb_value);
    SII_LIB_LOG_DEBUG1("0x%02x\n",pst_phy_param->u8_clk_src_fine_djust);
    SII_LIB_LOG_DEBUG1("0x%02x\n",pst_phy_param->u8_tx_regZ_zone);
    SII_LIB_LOG_DEBUG1("0x%02x\n",pst_phy_param->u8_rise_time);
    SII_LIB_LOG_DEBUG1("0x%02x\n",pst_phy_param->u8_fall_time);
#endif
    return ;
}

#ifndef HDMI_BUILD_IN_BOOT
static void sPhyConfigGet(HdmiTxObj_t* pObj,  SiiPhyConfig_t *pPhyConfig)
{
    SiiDrvCraAddr_t baseAddr = sBaseAddrGet(pObj);
    SiiInst_t       craInst  = sCraInstGet(pObj);

    if (!pPhyConfig)
    {
        return;
    }

    pPhyConfig->u8DataSwing      = SiiDrvCraRdReg8(craInst, baseAddr | 0x7b3); //data-swing
    pPhyConfig->u8CLKSwing       = SiiDrvCraRdReg8(craInst, baseAddr | 0x7b4); //CLK-swing
    pPhyConfig->u8SrcTermination = SiiDrvCraRdReg8(craInst, baseAddr | 0x7b5); //source termination
    pPhyConfig->u8TxVnbValue     = SiiDrvCraRdReg8(craInst, baseAddr | 0x7b6); //Tx driver vnb value setting
    pPhyConfig->u8AdjustClk      = SiiDrvCraRdReg8(craInst, baseAddr | 0x7b7); // Fine adjust clock source termination
    pPhyConfig->u8TxRegZone      = SiiDrvCraRdReg8(craInst, baseAddr | 0x222); // tx_reg_zone

    return;
}
#endif
static void sTransCodeBypassConfig(HdmiTxObj_t* pObj, bool_t b_bypass)
{
    SiiDrvCraAddr_t baseAddr = sBaseAddrGet(pObj);
    SiiInst_t		craInst	 = sCraInstGet(pObj);   
    uint8_t         u8reg_sys_ctrl4 = 0;
    
    u8reg_sys_ctrl4 = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__SYS_CTRL4);

    if (b_bypass)
    {
        u8reg_sys_ctrl4 |= BIT_MSK__SYS_CTRL4__REG_TRANSCODE;
    }
    else
    {
        u8reg_sys_ctrl4 &= ~BIT_MSK__SYS_CTRL4__REG_TRANSCODE;
    }
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__SYS_CTRL4, u8reg_sys_ctrl4);
}


#ifndef HDMI_BUILD_IN_BOOT
static SiiTmdsMode_t sTmdsModeGet(HdmiTxObj_t* pObj)
{
	SiiDrvCraAddr_t baseAddr = sBaseAddrGet(pObj);
	SiiInst_t		craInst	 = sCraInstGet(pObj);
    SiiTmdsMode_t   tmdsMode = SII_TMDS_MODE__NONE;
    uint8_t         u8AviOnOff = 0;
    uint8_t         u8AudioOnOff = 0;
    
	//if (SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__TPI_SC) & BIT_MSK__TPI_SC__REG_TPI_OUTPUT_MODE_B0)
    {
        tmdsMode = SII_TMDS_MODE__HDMI1;
        if (SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__TXC_DATA_DIV) == 0x2)
        {
            tmdsMode = SII_TMDS_MODE__HDMI2;
        }
    }

    sInfoframeOnOffGet(pObj, SII_INFO_FRAME_ID__AVI, &u8AviOnOff);
    sInfoframeOnOffGet(pObj, SII_INFO_FRAME_ID__AUDIO, &u8AudioOnOff);

    if (!(u8AudioOnOff && u8AviOnOff))
    {
        tmdsMode = SII_TMDS_MODE__DVI;  
    }

    return tmdsMode;
}

static void sGetTMDSOnOff(HdmiTxObj_t* pObj, bool_t* onOff)
{
	SiiDrvCraAddr_t baseAddr = sBaseAddrGet(pObj);
	SiiInst_t		craInst	 = sCraInstGet(pObj);

    if (!(0x10 == SiiDrvCraRdReg8(craInst, baseAddr | TXPHY_TMDS_CTL) && 
          0xa7 == SiiDrvCraRdReg8(craInst, baseAddr | TXPHY_DP_CTL0)))
    {
        *onOff = false;
    }
    else
    {
        *onOff = true;
    }

}
#endif
static void sUpdateHdmiMode(HdmiTxObj_t* pObj)
{
	SiiDrvCraAddr_t baseAddr = sBaseAddrGet(pObj);
	SiiInst_t		craInst	 = sCraInstGet(pObj);
	bool_t updateDone = true;
    
	do
	{
		SII_LIB_LOG_DEBUG1(pObj, "sUpdateHdmiMode:: Setting TMDS mode: %02X\n", pObj->tmdsMode);
		updateDone = true;

		switch(pObj->tmdsMode)
		{
		case SII_TMDS_MODE__NONE:
			//tmdsOn = false;
			break;

		case SII_TMDS_MODE__AUTO:
#if 0
			if((pObj->parseEdid.ieee_id == 0x000C03)/* HDMI1.4 VSDB */ || (pObj->parseEdid.ieee_id == 0xC45DD8)/* HDMIForum VSDB */)
			{
				pObj->tmdsMode = SII_TMDS_MODE__HDMI1;
				//HDMI1 supports upto 300 MHz and HDMI2 supports upto 600 MHz
				if(pObj->parseEdid.maxTmds > 300000000 && !pObj->isMhlConnected) 
					pObj->tmdsMode = SII_TMDS_MODE__HDMI2;
			}
			else
				pObj->tmdsMode = SII_TMDS_MODE__DVI;

			updateDone = false;
#endif
			break;

		case SII_TMDS_MODE__HDMI2:
			//SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__SCRCTL, BIT_MSK__SCRCTL__REG_HDMI2_ON);

			//SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__TPI_SC, BIT_MSK__TPI_SC__REG_TPI_OUTPUT_MODE_B0);
            SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__TEST_TXCTRL, BIT_MSK__TEST_TXCTRL__REG_HDMI_MODE);
            SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__TXC_DATA_DIV, 0x2);
            SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__SCRCTL, BIT_MSK__SCRCTL__REG_SCR_ON |BIT_MSK__SCRCTL__REG_HDMI2_ON );
			break;
		case SII_TMDS_MODE__HDMI1:
			//SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__SCRCTL, BIT_MSK__SCRCTL__REG_HDMI2_ON);

			//SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__TPI_SC, BIT_MSK__TPI_SC__REG_TPI_OUTPUT_MODE_B0);
            SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__TEST_TXCTRL, BIT_MSK__TEST_TXCTRL__REG_HDMI_MODE);
            SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__TXC_DATA_DIV, 0x0);
            SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__SCRCTL, BIT_MSK__SCRCTL__REG_SCR_ON |BIT_MSK__SCRCTL__REG_HDMI2_ON );
			break;
		case SII_TMDS_MODE__DVI:
			//SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__TPI_SC, BIT_MSK__TPI_SC__REG_TPI_OUTPUT_MODE_B0);
			SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__TEST_TXCTRL, BIT_MSK__TEST_TXCTRL__REG_HDMI_MODE);
            SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__TXC_DATA_DIV, 0x0);
            SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__SCRCTL, BIT_MSK__SCRCTL__REG_SCR_ON |BIT_MSK__SCRCTL__REG_HDMI2_ON );
			break;

		default:
			//tmdsOn = false;
			break;
		}
    #if 0   
		if(pObj->isMhlConnected || pObj->prevMhlConnStatus)
			sAvlinkEnable(pObj, tmdsOn);

			sPutTMDSOnOff(pObj, tmdsOn);
    #endif
	}while (false == updateDone);

	return;
}

static void sTmdsModeSet(HdmiTxObj_t* pObj)
{
	SII_LIB_LOG_DEBUG1(pObj, "Setting TMDS mode to: %02X\n", pObj->tmdsMode);
	if(pObj->tmdsMode)
	{
		pObj->iState = SII_MOD_HDMI_TX_EVENT__TMDS_ON;
	}
	else
	{
		pObj->iState = SII_MOD_HDMI_TX_EVENT__TMDS_OFF;
	}
    sUpdateHdmiMode(pObj);
	//SiiLibSeqTimerStart(pObj->timerHwUpdate, TIMER_START__HW_UPDATE, 0);
	
}

/**************************************add by h00180450*********************************************************/


static void sSoftwareReset(HdmiTxObj_t* pObj, uint8_t bReset)
{
    if (bReset)
    {
        SiiDrvCraSetBit8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__PWD_SRST, BIT_MSK__PWD_SRST__REG_SW_RST);
    }
    else
    {
        SiiDrvCraClrBit8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__PWD_SRST, BIT_MSK__PWD_SRST__REG_SW_RST);
    }
}

static void sAudioReset(HdmiTxObj_t* pObj, uint8_t bReset)
{
    if (bReset)
    {
		// reset AIP and AFIFO
    	SiiDrvCraSetBit8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__AIP_RST, BIT_MSK__AIP_RST__REG_RST4AUDIO_FIFO | BIT_MSK__AIP_RST__REG_RST4AUDIO);              
    }
    else
    {
        SiiDrvCraClrBit8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__AIP_RST, BIT_MSK__AIP_RST__REG_RST4AUDIO_FIFO | BIT_MSK__AIP_RST__REG_RST4AUDIO);  
    }
}

static void sTPIReset(HdmiTxObj_t* pObj, uint8_t bReset)
{
    if (bReset)
    {
        SiiDrvCraSetBit8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__TPI_HW_OPT0, BIT_MSK__TPI_HW_OPT0__REG_HW_TPI_SM_RST);     
    }
    else
    {
        SiiDrvCraClrBit8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__TPI_HW_OPT0, BIT_MSK__TPI_HW_OPT0__REG_HW_TPI_SM_RST);
    }
}

static void sHDCP2xReset(HdmiTxObj_t* pObj, uint8_t bReset)
{
    if (bReset)
    {
        SiiDrvCraSetBit8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__HDCP2X_TX_SRST, BIT_MSK__HDCP2X_TX_SRST__REG_HDCP2X_SW_RST);     
    }
    else
    {
        SiiDrvCraClrBit8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__HDCP2X_TX_SRST, BIT_MSK__HDCP2X_TX_SRST__REG_HDCP2X_SW_RST);
    }
}

static void sCECReset(HdmiTxObj_t* pObj, uint8_t bReset)
{
    if (bReset)
    {
        SiiDrvCraSetBit8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__SW_RST3, BIT_MSK__SW_RST3__REG_CEC_RST);     
    }
    else
    {
        SiiDrvCraClrBit8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__SW_RST3, BIT_MSK__SW_RST3__REG_CEC_RST);
    }
}

static void sReset(HdmiTxObj_t* pObj, SiiResetType_t resetSrc, uint8_t reset)
{
    switch (resetSrc)
    {
        case SII_RESET_TYPE__SOFTWARE:
            sSoftwareReset(pObj, reset);
            break;
	    case SII_RESET_TYPE__AUDIO:
            sAudioReset(pObj, reset);
            break;
    	case SII_RESET_TYPE__TPI:
            sTPIReset(pObj, reset);
            break;
	    case SII_RESET_TYPE__HDCP2x:
            sHDCP2xReset(pObj, reset);
            break;
	    case SII_RESET_TYPE__CEC:
            sCECReset(pObj, reset);
            break;
        default:
            break;
    }

 	// l232354, Because the reset software(SII_RESET_TYPE__SOFTWARE) can chage the reg REG_ADDR__CEC_CONFIG_CPI to default value,
 	// so need to set the vale again.
	SiiDrvCraClrBit8(sCraInstGet(pObj), sBaseAddrGet(pObj)|REG_ADDR__CEC_CONFIG_CPI, BIT_MSK__CEC_CONFIG_CPI__CEC_REG_I2C_CEC_PASSTHRU);
}
#ifndef HDMI_BUILD_IN_BOOT

static void sAudioFormatGet(HdmiTxObj_t* pObj, SiiAudioFormat_t *audioFmt)
{
	SiiDrvCraAddr_t baseAddr = sBaseAddrGet(pObj);
	SiiInst_t		craInst	 = sCraInstGet(pObj);
    uint8_t audioFs, audioMode, downSample;
    bool_t layout1; 

    audioFs = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__TPI_AUD_FS);

    //sample frequency
	switch(audioFs & 0xf)
	{
	case 0x04: 
		audioFmt->audioFs = SII_AUDIO_FS__22_05KHZ; 
		break;

	case 0x06: 
		audioFmt->audioFs = SII_AUDIO_FS__24KHZ; 
		break;

	case 0x03: 
		audioFmt->audioFs = SII_AUDIO_FS__32KHZ; 
		break;

	case 0x00: 
		audioFmt->audioFs = SII_AUDIO_FS__44_1KHZ; 
		break;

	case 0x02: 
		audioFmt->audioFs = SII_AUDIO_FS__48KHZ; 
		break;

	case 0x08: 
		audioFmt->audioFs = SII_AUDIO_FS__88_2KHZ;  
		break;

	case 0x0A: 
		audioFmt->audioFs = SII_AUDIO_FS__96KHZ; 
		break;

	case 0x0C: 
		audioFmt->audioFs = SII_AUDIO_FS__176_4KHZ; 
		break;

	case 0x0E: 
		audioFmt->audioFs = SII_AUDIO_FS__192KHZ; 
		break;

	case 0x09: 
		audioFmt->audioFs = SII_AUDIO_FS__768KHZ; 
		break;			

	default : 
		break;
	}
    
    layout1 = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__AUDP_TXCTRL)& BIT_MSK__AUDP_TXCTRL__REG_LAYOUT; 
    if (!layout1)
    {
        audioFmt->layout1 = AUDIO_FORMAT__2CH;
    }
    else
    {
        audioFmt->layout1 = AUDIO_FORMAT__8CH;
    }

    audioMode = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__AUD_MODE);
	switch(audioMode & 0xff)
	{
	case SII_BIT4: 
		audioFmt->layout1 = AUDIO_FORMAT__2CH;
        audioFmt->i2s = true;
		break;
	case (SII_BIT5 |SII_BIT4): 
        audioFmt->layout1 = AUDIO_FORMAT__4CH;
        audioFmt->i2s = true;
        break;
	case (SII_BIT6 |SII_BIT5 |SII_BIT4): 
        audioFmt->layout1 = AUDIO_FORMAT__6CH;
        audioFmt->i2s = true;
		break;
	case (SII_BIT7 |SII_BIT6 |SII_BIT5 |SII_BIT4): 
        audioFmt->layout1 = AUDIO_FORMAT__8CH;
        audioFmt->i2s = true;			
        break;
	case SII_BIT3: 
        audioFmt->dsd = true;	
		break;
	case SII_BIT2: 
        audioFmt->hbrA = true;
		break; 
	case SII_BIT1: 
        audioFmt->spdif = true;
		break;   
    default:
        break;
	}

    downSample = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__TPI_DOWN_SMPL_CTRL) & BIT_MSK__TPI_DOWN_SMPL_CTRL__REG_TPI_AUD_HNDL;
	// Enable/Disable Down sampling
	if(downSample == BIT_ENUM__TPI_DOWN_SMPL_CTRL__AUDIO_DOWNSAMP_EN)
	{
	    audioFmt->downSample = true;
	}
	else
	{
	    audioFmt->downSample = false;
	}

    audioFmt->n   = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__N_SVAL1);
    audioFmt->n  |= SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__N_SVAL2) << 8;
    audioFmt->n  |= SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__N_SVAL3) << 16;

    audioFmt->cts  = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__CTS_TXHVAL1);
    audioFmt->cts |= SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__CTS_TXHVAL2) << 8;
    audioFmt->cts |= SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__CTS_TXHVAL3) << 16;

}

static void sChannelStatusGet(HdmiTxObj_t* pObj, SiiChannelStatus_t *channelStatus)
{
	SiiDrvCraAddr_t baseAddr = sBaseAddrGet(pObj);
	SiiInst_t		craInst	 = sCraInstGet(pObj);

	channelStatus->i2s_chst0 = SiiDrvCraRdReg8(craInst, baseAddr |  REG_ADDR__I2S_CHST0);
	channelStatus->i2s_chst1 = SiiDrvCraRdReg8(craInst, baseAddr |  REG_ADDR__I2S_CHST1);
	channelStatus->i2s_chst2 = SiiDrvCraRdReg8(craInst, baseAddr |  REG_ADDR__I2S_CHST2);
	channelStatus->i2s_chst3 = SiiDrvCraRdReg8(craInst, baseAddr |  REG_ADDR__I2S_CHST3);
	channelStatus->i2s_chst4 = SiiDrvCraRdReg8(craInst, baseAddr |  REG_ADDR__I2S_CHST4);
	channelStatus->i2s_chst5 = SiiDrvCraRdReg8(craInst, baseAddr |  REG_ADDR__I2S_CHST6);
	channelStatus->i2s_chst6 = SiiDrvCraRdReg8(craInst, baseAddr |  REG_ADDR__I2S_CHST7); 
}
#ifndef HDMI_BUILD_IN_BOOT
static void sHdmiHardWareStatusGet(HdmiTxObj_t* pObj,  SiiHdmiStatus_t *hdmiStatus)
{
    uint8_t frameId = SII_INFO_FRAME_ID__AVI;
	SiiDrvCraAddr_t baseAddr = sBaseAddrGet(pObj);
	SiiInst_t		craInst	 = sCraInstGet(pObj);

    hdmiStatus->tmdsMode = sTmdsModeGet(pObj);
    // select gen2
    SiiDrvCraWrReg8(sCraInstGet(pObj), sBaseAddrGet(pObj)| REG_ADDR__TPI_INFO_FSEL, BIT_ENUM__TPI_INFO_FSEL__GEN2); 
    if( (SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__TPI_INFO_B0)  == 0x03)
      && SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__TPI_INFO_B3)  == 0x01
      && SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__TPI_INFO_B10) == 0x01
      && SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__TPI_INFO_B17) == 0x01
      && SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__TPI_INFO_B24) == 0x01 )
    {
        hdmiStatus->avMute = true;
    }
    else
    {
        hdmiStatus->avMute = false;
    }

    hdmiStatus->hotPlug  = (SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__TPI_HPD_RSEN) & BIT_MSK__TPI_HPD_STATE) ? true : false;
    hdmiStatus->rsen     = (SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__TPI_HPD_RSEN) & BIT_MSK__TPI_RSEN_STATE) ? true : false;
    //hdmiStatus->hotPlug  = (hdmiStatus->hotPlug || hdmiStatus->rsen);
    hdmiStatus->audioEnable = (SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__AUD_EN) & BIT_MSK__AUD_EN__REG_AUD_IN_EN) ? true : false;
   	hdmiStatus->phyPower = (SiiDrvCraRdReg8(craInst, baseAddr | TXPHY_PLL_CTL2) & BIT_MSK__TXPHY_PLL_CTL2__REG_PWRON) \
                          == BIT_MSK__TXPHY_PLL_CTL2__REG_PWRON ? true : false;
	hdmiStatus->phyBitDepth = (SiiDrvCraRdReg8(craInst, baseAddr | TXPHY_TOP_CTL1) & BIT_MSK__TXPHY_TOP_CTL1__REG_DP);

	hdmiStatus->outBitDepth = (SiiDrvCraRdReg8(craInst,  baseAddr | REG_ADDR__P2T_CTRL) & BIT_MSK__P2T_CTRL__REG_PACK_MODE);
    sGetTMDSOnOff(pObj, (bool_t*)&hdmiStatus->phyOutput);
    
	if ((SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__AUDP_TXCTRL) & BIT_MSK__AUDP_TXCTRL__REG_AUD_MUTE_EN) ||
        hdmiStatus->avMute)
    {
        hdmiStatus->audioMute = true;
    }
    else
    {
        hdmiStatus->audioMute = false;
    }

    sAudioFormatGet(pObj, &hdmiStatus->audioFmt);
    sChannelStatusGet(pObj, &hdmiStatus->channelStatus);

    for (; frameId <= SII_INFO_FRAME_ID__MPEG; frameId++)
    {
        SiiInfoFrame_t *infoframe = &hdmiStatus->infoframe[frameId]; 
        infoframe->ifId           = frameId;
        sInfoframeTypeGet(pObj, infoframe);
        sInfoframeOnOffGet(pObj, frameId, &hdmiStatus->bIfOn[frameId]);
    }

    sPhyConfigGet(pObj, &hdmiStatus->PhyConfig);

}
#endif
static void sIRQMaskOn(HdmiTxObj_t* pObj, SiiInterruptType_t intType)
{
    switch (intType)
    {
        case SII_INT_TYPE__HPD_RSEN:
        	SiiDrvCraWrReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__INTR1_MASK, BIT_MSK__INTR1_MASK__REG_INTR1_MASK5 | BIT_MSK__INTR1_MASK__REG_INTR1_MASK6);	
            break;
        case SII_INT_TYPE__HDCP:
			//Enable Masks for HDCP1x Interrupts
			SiiDrvCraWrReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__TPI_INTR_EN, BIT_MSK__TPI_INTR_EN__REG_TPI_INTR_MASK_B3 | BIT_MSK__TPI_INTR_EN__REG_TPI_INTR_MASK_B7);           
            break;
        case SII_INT_TYPE__HDCP2x:
			//Enable HDCP2x Interrupts
			SiiDrvCraWrReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__HDCP2X_INTR0_MASK, BIT_MSK__HDCP2X_INTR0_MASK__INTR0_MASK_B0 \
				| BIT_MSK__HDCP2X_INTR0_MASK__INTR0_MASK_B1 | BIT_MSK__HDCP2X_INTR0_MASK__INTR0_MASK_B2);
			SiiDrvCraWrReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__HDCP2X_INTR1_MASK, BIT_MSK__HDCP2X_INTR1_MASK__INTR1_MASK_B0);	            
            break;
        case SII_INT_TYPE__CEC:
        	// Enable the interrupts within the CEC hardware block
        	SiiDrvCraPutBit8(sCraInstGet(pObj), 0x3F00 | REG_ADDR__CEC_INT_ENABLE_0, BIT_MSK__CEC_INT_STATUS_0__INTRP_RX_FIFO_NEMPTY | BIT_MSK__CEC_INT_STATUS_0__INTRP_TX_FIFO_EMPTY | BIT_MSK__CEC_INT_STATUS_0__INTRP_TX_FF_CSTATE, SET_BITS );
        	SiiDrvCraPutBit8(sCraInstGet(pObj),  0x3F00 | REG_ADDR__CEC_INT_ENABLE_1, BIT_MSK__CEC_INT_STATUS_1__INTRP_FRME_RETX_CNT | BIT_MSK__CEC_INT_STATUS_1__INTRP_SHRT_PULSE_DT | BIT_MSK__CEC_INT_STATUS_1__INTRP_START_ODD_BIT | BIT_MSK__CEC_INT_STATUS_1__INTRP_RX_FIFO_OVRUN, SET_BITS );
            break;
        default:
            break;
    }
}

static void sIRQMaskOff(HdmiTxObj_t* pObj, SiiInterruptType_t intType)
{
    switch (intType)
    {
        case SII_INT_TYPE__HPD_RSEN:
          	SiiDrvCraWrReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__INTR1_MASK, 0x0);	
            break;
        case SII_INT_TYPE__HDCP:
			//DisableHDCP1x Intr Masks
			SiiDrvCraWrReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__TPI_INTR_EN, 0x0);            
            break;
        case SII_INT_TYPE__HDCP2x:
			//Disable HDCP2x Intr Masks
			SiiDrvCraWrReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__HDCP2X_INTR0_MASK, 0x0); //HDCP2x Interrupt0 Mask Register
			SiiDrvCraWrReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__HDCP2X_INTR1_MASK, 0x0); //HDCP2x Interrupt1 Mask Register
            break;
        case SII_INT_TYPE__CEC:
        	SiiDrvCraWrReg8(sCraInstGet(pObj), 0x3F00 | REG_ADDR__CEC_INT_ENABLE_0, CLEAR_BITS);
        	SiiDrvCraWrReg8(sCraInstGet(pObj), 0x3F00 | REG_ADDR__CEC_INT_ENABLE_1, CLEAR_BITS);
            break;
        default:
            break;
    }
}

static void sIRQEnableSet(HdmiTxObj_t* pObj, SiiInterruptType_t intSrc, uint8_t enable)
{
    if (enable)
    {
        sIRQMaskOn(pObj, intSrc);
    }
    else
    {
        sIRQMaskOff(pObj, intSrc);
    }
}
#endif

static void sSoftwareInit(HdmiTxObj_t* hdmiObj)
{
	/*--------------------------------*/
	/* Initialize user request states */
	/*--------------------------------*/
	//hdmiObj->primLink        = pConfig->primLink;
	//hdmiObj->bDualLink       = false; 
	hdmiObj->tmdsMode			= SII_TMDS_MODE__NONE;
	hdmiObj->prevTmdsMode		= SII_TMDS_MODE__NONE;
	hdmiObj->hvSyncPol			= SII_HV_SYNC_POL__HPVP;
	hdmiObj->eState				= SII_MOD_HDMI_STATUS__TMDS_OFF;
	//Set Internal State to TMDS_OFF
	hdmiObj->iState             = SII_MOD_HDMI_TX_EVENT__TMDS_OFF;
	hdmiObj->isMhlConnected		= false;
	hdmiObj->isMhl3Connected	= false;	//Remove Later
	hdmiObj->prevMhlConnStatus	= false;

	hdmiObj->bInitHotPlug		= false;  // set this to false, so it will check the h/w status

	hdmiObj->bIsHdcpOn			= false;
	hdmiObj->bWasHdcpOn			= false;
	hdmiObj->bAvMute			= false;
	hdmiObj->bIfOnAvi			= true;
	hdmiObj->bIfOnAudio			= true;
	hdmiObj->bIfOnVs			= true;
	hdmiObj->bIfOnSpd			= false;
	hdmiObj->bIfOnGbd			= false;
	hdmiObj->bIfOnMpeg			= false;
	hdmiObj->bIfOnIsrc			= false;
	hdmiObj->bIfOnIsrc2			= false;
	hdmiObj->bIfOnAcp			= false;
#ifdef HDMI_HDR_SUPPORT
    hdmiObj->bIfOnDrm           = true;
#endif
	/*--------------------------------*/
	/* Initialize user status         */
	/*--------------------------------*/
	hdmiObj->bHotPlug	= false;
	hdmiObj->bRsen		= false;

	/*--------------------------------*/
	/* Initialize interrupts Status    */
	/*--------------------------------*/
	hdmiObj->intStat.reg0 = 0x00;
	hdmiObj->intStat.reg1 = 0x00;

	//Clear Infoframes
	SiiDrvClearInfoFrame(SII_INFO_FRAME_ID__AVI,    &hdmiObj->ifAvi);
	SiiDrvClearInfoFrame(SII_INFO_FRAME_ID__AUDIO,  &hdmiObj->ifAudio);
	SiiDrvClearInfoFrame(SII_INFO_FRAME_ID__VS,     &hdmiObj->ifVs);
	SiiDrvClearInfoFrame(SII_INFO_FRAME_ID__SPD,    &hdmiObj->ifSpd);
	SiiDrvClearInfoFrame(SII_INFO_FRAME_ID__GBD,    &hdmiObj->ifGbd);
	SiiDrvClearInfoFrame(SII_INFO_FRAME_ID__MPEG,   &hdmiObj->ifMpeg);
	SiiDrvClearInfoFrame(SII_INFO_FRAME_ID__ISRC,   &hdmiObj->ifIsrc);
	SiiDrvClearInfoFrame(SII_INFO_FRAME_ID__ISRC2,  &hdmiObj->ifIsrc2);
	SiiDrvClearInfoFrame(SII_INFO_FRAME_ID__ACP,    &hdmiObj->ifAcp);
#ifdef HDMI_HDR_SUPPORT
    SiiDrvClearInfoFrame(SII_INFO_FRAME_ID__DRM,    &hdmiObj->ifDrm);
#endif

	//Set Audio Infoframes
	hdmiObj->ifAudio.b[0] = 0x84;
	hdmiObj->ifAudio.b[1] = 0x01;
	hdmiObj->ifAudio.b[2] = 0x0a;
	hdmiObj->ifAudio.b[3] = 0x70;
	hdmiObj->ifAudio.b[4] = 0x01;	// 2-ch
	hdmiObj->ifAudio.b[5] = 0x00;    	
	hdmiObj->ifAudio.b[6] = 0x00;
	hdmiObj->ifAudio.b[7] = 0x00;
	hdmiObj->ifAudio.b[8] = 0x00;
	hdmiObj->ifAudio.b[9] = 0x00;
	hdmiObj->ifAudio.b[10] = 0x00;
	hdmiObj->ifAudio.b[11] = 0x00;
	hdmiObj->ifAudio.b[12] = 0x00;
	hdmiObj->ifAudio.b[13] = 0x00;

	//Set Audio Channel Status
	hdmiObj->channelStatus.i2s_chst0 = 0x00;
	hdmiObj->channelStatus.i2s_chst1 = 0x00;
	hdmiObj->channelStatus.i2s_chst2 = 0x00;
	hdmiObj->channelStatus.i2s_chst3 = 0x02;//48kHz
	hdmiObj->channelStatus.i2s_chst4 = 0x02;//16bit
	hdmiObj->channelStatus.i2s_chst5 = 0x00;
	hdmiObj->channelStatus.i2s_chst6 = 0x00;

	//Set Audio Format
	//hdmiObj->audioFormat.spdif = true;
	hdmiObj->audioFormat.i2s = true;
	hdmiObj->audioFormat.layout1 = AUDIO_FORMAT__2CH;
	hdmiObj->audioFormat.audioFs = SII_AUDIO_FS__48KHZ;
}

static void sHardwareInit(HdmiTxObj_t* pObj)
{
    SiiInst_t craInst        = sCraInstGet(pObj);
    SiiDrvCraAddr_t baseAddr = sBaseAddrGet(pObj);
	/*--------------------------------*/
	/* Static hardware configuration  */
	/*--------------------------------*/
	//Cypress2 initialization
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__CLKPWD		, 0x06);//Gate off pclk,mhl1/2/3 clk
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__FUNC_SEL		, 0x01);//enable HDMI for tx
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__SYS_MISC		, 0x00);
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DIPT_CNTL		, 0x06);//enable packets pass through function  

    //SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__P2T_CTRL		, 0x00);//enable Deep Color Packet
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__TEST_TXCTRL	, 0x02);//enable HDMI mode for output
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__TX_ZONEL_CTL4	, 0x04);

#ifndef HDMI_FPGA_SUPPORT
	// PHY init
	SiiDrvCraWrReg8(craInst, baseAddr | TXPHY_TOP_CTL0	, 0x90);
	SiiDrvCraWrReg8(craInst, baseAddr | TXPHY_PLL_CTL0	, 0x82);
	SiiDrvCraWrReg8(craInst, baseAddr | TXPHY_PLL_CTL2	, 0x30);//Phy power on
    SiiDrvCraClrBit8(craInst, baseAddr | TXPHY_TMDS_CTL, BIT_MSK__TXPHY_TMDS_CTL_REG_TMDS_OE);
    //pll
    SiiDrvCraWrReg8(craInst, baseAddr | 0x7E2, 0x4);
    SiiDrvCraWrReg8(craInst, baseAddr | 0x7E0, 0x6);
	//SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__EDID_CTRL	, 0x59);
#endif
	// Cypress2 - audio related initialization
	//SiiDrvCraWrReg8(craInst, baseAddr |REG_ADDR__PKT_FILTER_0, BIT_MSK__PKT_FILTER_0__REG_DROP_CTS_PKT);
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__I2S_CHST3, 0x02);             // set AIP channel Sampling frequency to 48 KHz
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__I2S_CHST4, 0x02);             // set AIP channel status word length to 16 bits
    /* to solve the problem of blurred screen in boot on HKC-F24PA1100 TV. the value should be change to 0x0e. */
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__ACR_CTRL, 0x0c);              // configure AIP cts generation
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__I2S_IN_SIZE, 0x0b);           // set AIP i2s word length to 24 bits
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__I2S_IN_CTRL, 0x60);           // configure AIP i2s input
	SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__AUD_MODE, 0x10, 0x10);       // enable sd0-3 input
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__AIP_HDMI2MHL, 0x0);
    
    SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__AUDP_TXCTRL, BIT_MSK__AUDP_TXCTRL__REG_LAYOUT); // layout 0         
	SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__TPI_DOWN_SMPL_CTRL, BIT_MSK__TPI_DOWN_SMPL_CTRL__REG_TPI_AUDIO_LOOKUP_EN);// disable LUT  

	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__TPI_AUD_CONFIG, 0x00);
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__TPI_AUD_FS, 0x02);// fs = 48kHz
	SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__TPI_AUD_FS, BIT_MSK__TPI_AUD_FS__REG_TPI_AUD_SF_OVRD, BIT_MSK__TPI_AUD_FS__REG_TPI_AUD_SF_OVRD);// enable fs override
    // clear hotplug¡¢rsen int
    //SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__INTR1, BIT_MSK__INTR1__REG_INTR1_STAT6 | BIT_MSK__INTR1__REG_INTR1_STAT5); 
#if 0
	// reset AIP and AFIFO
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__AIP_RST, BIT_MSK__AIP_RST__REG_RST4AUDIO_FIFO | BIT_MSK__AIP_RST__REG_RST4AUDIO);              
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__AIP_RST, 0x00);              
#endif
	//SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__INTR1_MASK, BIT_MSK__INTR1_MASK__REG_INTR1_MASK5 | BIT_MSK__INTR1_MASK__REG_INTR1_MASK6);	
    //HOT PLUG timer
    SiiDrvCraWrReg8(craInst, baseAddr |REG_ADDR__HTPLG_T2, 0x4b);
    SiiDrvCraWrReg8(craInst, baseAddr |REG_ADDR__HTPLG_T1, 0x64);
    sDdcSpeedConfig(pObj, 0x15); // 0x15 -> 60KHz

	//hdmi tx default audio mute
    SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__AUDP_TXCTRL, BIT_MSK__AUDP_TXCTRL__REG_AUD_MUTE_EN);

    //hdmi pattern tpg_enable default
    SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__PATTERN_GENERATOR, BIT_MASK_PATTERN_GENERATOR_TPG_ENABLE);
    
 	// Clear passthru function 
	SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__CEC_CONFIG_CPI, BIT_MSK__CEC_CONFIG_CPI__CEC_REG_I2C_CEC_PASSTHRU);
}


static void sPhyPowerSet(HdmiTxObj_t* pObj, uint8_t onOff)
{
	SiiInst_t craInst        = sCraInstGet(pObj);
    SiiDrvCraAddr_t baseAddr = sBaseAddrGet(pObj);
    
    if (onOff)
    {
        SiiDrvCraSetBit8(craInst, baseAddr | TXPHY_PLL_CTL2, BIT_MSK__TXPHY_PLL_CTL2__REG_PWRON);//Phy power on
    }
    else
    {
        SiiDrvCraClrBit8(craInst, baseAddr | TXPHY_PLL_CTL2, BIT_MSK__TXPHY_PLL_CTL2__REG_PWRON);//Phy power off
    }
}

static void sAvMuteSet(HdmiTxObj_t* pObj, uint8_t bAvMute)
{
    SII_LIB_LOG_DEBUG1(pObj, "sAvMuteSet (IN GEN):: %s\n", bAvMute ? "ON":"OFF");

    if( bAvMute )
    { 
        // select gen2
        //himm 0xf8ce1afc 0x7
        SiiDrvCraWrReg8(sCraInstGet(pObj), sBaseAddrGet(pObj)| REG_ADDR__TPI_INFO_FSEL, BIT_ENUM__TPI_INFO_FSEL__GEN2); 

        //DISable GEN
        //himm 0xf8ce1b7c 0x0
        SiiDrvCraWrReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__TPI_INFO_EN, 0x00);
        
        // set gcp
        //himm 0xf8ce1b00 0x3
        //himm 0xf8ce1b04 0x0
        //himm 0xf8ce1b08 0x0
        SiiDrvCraWrReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__TPI_INFO_B0, 0x3);
        SiiDrvCraWrReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__TPI_INFO_B1, 0x0);
        SiiDrvCraWrReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__TPI_INFO_B2, 0x0);
        
        //setAvmute=1
        //himm 0xf8ce1b0c 0x1
        //himm 0xf8ce1b28 0x1
        //himm 0xf8ce1b44 0x1
        //himm 0xf8ce1b60 0x1
        SiiDrvCraWrReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__TPI_INFO_B3, 0x1);      
        SiiDrvCraWrReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__TPI_INFO_B10, 0x1);    
        SiiDrvCraWrReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__TPI_INFO_B17, 0x1);
        SiiDrvCraWrReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__TPI_INFO_B24, 0x1);
        

        //enable GEN
        //himm 0xf8ce1b7c 0xc0 
        SiiDrvCraWrReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__TPI_INFO_EN, 0xe0);
    }
    else
    {
        // select gen2
        //himm 0xf8ce1afc 0x7
        SiiDrvCraWrReg8(sCraInstGet(pObj), sBaseAddrGet(pObj)| REG_ADDR__TPI_INFO_FSEL, BIT_ENUM__TPI_INFO_FSEL__GEN2); 
        
        // set gcp
        //himm 0xf8ce1b00 0x3
        //himm 0xf8ce1b04 0x0
        //himm 0xf8ce1b08 0x0
        SiiDrvCraWrReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__TPI_INFO_B0, 0x3);
        SiiDrvCraWrReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__TPI_INFO_B1, 0x0);
        SiiDrvCraWrReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__TPI_INFO_B2, 0x0);
        
        //clrAvmute=1
        //himm 0xf8ce1b0c 0x10
        //himm 0xf8ce1b28 0x10
        //himm 0xf8ce1b44 0x10
        //himm 0xf8ce1b60 0x10
        SiiDrvCraWrReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__TPI_INFO_B3, 0x10);      
        SiiDrvCraWrReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__TPI_INFO_B10, 0x10);    
        SiiDrvCraWrReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__TPI_INFO_B17, 0x10);
        SiiDrvCraWrReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__TPI_INFO_B24, 0x10);

        //enable GEN2's GCP
        SiiDrvCraWrReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__TPI_INFO_FSEL, BIT_ENUM__TPI_INFO_FSEL__GEN2);
        SiiDrvCraWrReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__TPI_INFO_EN, 0xe0);

#ifndef HDMI_BUILD_IN_BOOT
        // start timer
        HDMI_MUTEX_LOCK(g_avmuteTimerMute);
        SiiLibSeqTimerStop(pObj->timerAvmute);
        SiiLibSeqTimerStart(pObj->timerAvmute, TIMER_START__AVMUTE_CLR_STOP_WAIT, 0);
        HDMI_MUTEX_UNLOCK(g_avmuteTimerMute);
#else
        // stop clr mute
        SiiLibTimeMilliDelay(280);
            
        // disable GEN2's GCP
        SiiDrvCraWrReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__TPI_INFO_FSEL, BIT_ENUM__TPI_INFO_FSEL__GEN2);
        SiiDrvCraWrReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__TPI_INFO_EN, 0x00);
#endif
    }
    return ;
}

#ifndef HDMI_BUILD_IN_BOOT

static void sAvMuteClrStopSet(SiiInst_t inst)
{
    HdmiTxObj_t* pObj = (HdmiTxObj_t*)SII_LIB_OBJ_PNTR(inst);
    SiiLibSeqTimerStop(pObj->timerAvmute);
    // disable GEN2's GCP
    SiiDrvCraWrReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__TPI_INFO_FSEL, BIT_ENUM__TPI_INFO_FSEL__GEN2);
    SiiDrvCraWrReg8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__TPI_INFO_EN, 0x00);
}
#ifdef HDMI_HDR_SUPPORT
static void sZeroDrmIfTimeSet(HdmiTxObj_t* pObj, SiiHdrTimerCfg_S *pstZeroDrmIfTimer)
{       
    SiiLibSeqTimerStop(pObj->timerZeroDrmIf);  

    if(false == pstZeroDrmIfTimer->bTimerStart)
    {
        return;
    }
    
    if(0 == pstZeroDrmIfTimer->u32Time)
    {
        pstZeroDrmIfTimer->u32Time = TIMER_START_ZERO_DRM_IF_WAIT;
    }
    
    SiiLibSeqTimerStart(pObj->timerZeroDrmIf, pstZeroDrmIfTimer->u32Time, TIMER_START_ZERO_DRM_IF);

    return;
}

static void sZeroDrmIfTimeOut(SiiInst_t inst)
{
    HdmiTxObj_t* pObj = (HdmiTxObj_t*)SII_LIB_OBJ_PNTR(inst);
    SiiLibSeqTimerStop(pObj->timerZeroDrmIf);

    pObj->cbFunc(pObj->pConfig->instTx, SII_DRV_TX_EVENT_ZERO_DRMIF_TIMEOUT);

    return;
}

static void sHdrModeChangeTimeSet(HdmiTxObj_t* pObj, SiiHdrTimerCfg_S *pstHdrModeChangeTimer)
{       
    SiiLibSeqTimerStop(pObj->timerHdrMode);  

    if(false == pstHdrModeChangeTimer->bTimerStart)
    {
        return;
    }
    
    if(0 == pstHdrModeChangeTimer->u32Time)
    {
        pstHdrModeChangeTimer->u32Time = TIMER_START_HDR_MODE_CHANGE_WAIT;
    }
    
    SiiLibSeqTimerStart(pObj->timerHdrMode, pstHdrModeChangeTimer->u32Time, TIMER_START_HDR_MODE_CHANGE);

    return;
}

static void sHdrModeChangeTimeOut(SiiInst_t inst)
{
    HdmiTxObj_t* pObj = (HdmiTxObj_t*)SII_LIB_OBJ_PNTR(inst);
    SiiLibSeqTimerStop(pObj->timerHdrMode);
    
    pObj->cbFunc(pObj->pConfig->instTx, SII_DRV_TX_EVENT_HDRMODE_CHANGE_TIMEOUT);

    return;
}
#endif

static void sAudioEnableSet(HdmiTxObj_t* pObj, uint8_t enable)
{
    if (enable)
    {
    	SiiDrvCraSetBit8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__AUD_EN, BIT_MSK__AUD_EN__REG_AUD_IN_EN);// enable audio
        SiiDrvCraClrBit8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__AUD_EN, BIT_MSK__AUD_EN__REG_AUD_SEL_OWRT); // HBRA authentication pass
        SiiDrvCraSetBit8(sCraInstGet(pObj), sBaseAddrGet(pObj)|REG_ADDR__ACR_CTRL, BIT_MSK__ACR_CTRL__REG_CTS_REQ_EN);              // configure AIP cts generation
    }
    else
    {
    	SiiDrvCraClrBit8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__AUD_EN, BIT_MSK__AUD_EN__REG_AUD_IN_EN);// disable audio
        //SiiDrvCraClrBit8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__AUD_EN, BIT_MSK__AUD_EN__REG_AUD_SEL_OWRT);// HBRA authentication pass
        SiiDrvCraClrBit8(sCraInstGet(pObj), sBaseAddrGet(pObj)| REG_ADDR__ACR_CTRL, BIT_MSK__ACR_CTRL__REG_CTS_REQ_EN);              // configure AIP cts generation
    }
}

static void sAudioMuteSet(HdmiTxObj_t* pObj, uint8_t bAudioMute)
{
	if(pObj->tmdsMode == SII_TMDS_MODE__HDMI1 || pObj->tmdsMode == SII_TMDS_MODE__HDMI2)
	{
		//SII_LIB_LOG_DEBUG1(pObj, ("Setting AUDIOMUTE:: %s\n", bAudioMute ? "ON":"OFF"));
		if( bAudioMute )
		{ 
    		SiiDrvCraSetBit8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__AUDP_TXCTRL, BIT_MSK__AUDP_TXCTRL__REG_AUD_MUTE_EN);
            sAudioReset(pObj, true);
            SiiLibTimeMilliDelay(2);
            sAudioReset(pObj, false); 
        }
		else
		{
		
            //sAudioReset(pObj, false); 
			SiiDrvCraClrBit8(sCraInstGet(pObj), sBaseAddrGet(pObj) | REG_ADDR__AUDP_TXCTRL, BIT_MSK__AUDP_TXCTRL__REG_AUD_MUTE_EN);
        }
	}
}
/*****************************************************************************************************************/


static void sUpdateHotPlugRsen(HdmiTxObj_t* pObj, uint_t event)
{
	if(pObj->cbFunc)
	{
        pObj->cbFunc(pObj->pConfig->instTx, event);
    }
}
#endif

static void sOutputBitDepthSet(HdmiTxObj_t* pObj , SiiDrvCraAddr_t baseAddr)
{
    SiiDrvCraPutBit8(pObj->pConfig->instTxCra,  baseAddr | REG_ADDR__P2T_CTRL, BIT_MSK__P2T_CTRL__REG_PACK_MODE, pObj->outputBitDepth );
    if (pObj->outputBitDepth == SII_DRV_TX_PACK_MODE__8_BPP)
    {
        SiiDrvCraClrBit8(pObj->pConfig->instTxCra,  baseAddr | REG_ADDR__P2T_CTRL,  BIT_MSK__P2T_CTRL__REG_DC_PKT_EN);
    }
    else
    {
        SiiDrvCraSetBit8(pObj->pConfig->instTxCra,  baseAddr | REG_ADDR__P2T_CTRL,  BIT_MSK__P2T_CTRL__REG_DC_PKT_EN);
    }
    
    SiiDrvCraPutBit8(pObj->pConfig->instTxCra,  baseAddr | TXPHY_TOP_CTL1, BIT_MSK__TXPHY_TOP_CTL1__REG_DP, pObj->outputBitDepth );
}

static void sUpdateOutputBitDepth(HdmiTxObj_t* pObj, SiiDrvBitDepth_t bitDepth)
{
	switch(bitDepth)
	{
	case SII_DRV_BIT_DEPTH__8_BIT:
		pObj->outputBitDepth = SII_DRV_TX_PACK_MODE__8_BPP;
		break;
	case SII_DRV_BIT_DEPTH__10_BIT:
		pObj->outputBitDepth = SII_DRV_TX_PACK_MODE__10_BPP;
		break;
	case SII_DRV_BIT_DEPTH__12_BIT:
		pObj->outputBitDepth = SII_DRV_TX_PACK_MODE__12_BPP;
		break;
	case SII_DRV_BIT_DEPTH__16_BIT:
		pObj->outputBitDepth = SII_DRV_TX_PACK_MODE__16_BPP;
		break;
	default:
		SII_LIB_LOG_ERR(pObj, "Error:: Wrong Output Bit Depth : %02X\n", bitDepth);
		pObj->outputBitDepth = SII_DRV_TX_PACK_MODE__8_BPP;
		break;
	}

	SII_LIB_LOG_DEBUG1(pObj, "Setting Output Bit Depth to: %02X\n", pObj->outputBitDepth);

	sOutputBitDepthSet(pObj, sBaseAddrGet(pObj));
}
static SiiDrvCraAddr_t sBaseAddrGet(HdmiTxObj_t *pObj)
{
	return pObj->pConfig->baseAddrTx;
}

static SiiInst_t sCraInstGet(HdmiTxObj_t *pObj)
{
	return pObj->pConfig->instTxCra;
}
#if 0
static void sTxLog(uint8_t *pData, uint16_t len)
{
	int i = 0;
    uint8_t offset = 0;
	uint8_t *tempBuffer = SiiLibMallocCreate(6*len);
    if (tempBuffer != NULL)
    {
        offset += SII_SPRINTF((tempBuffer+offset,"%c", '\n'));
        while(len--)
        {
            offset += SII_SPRINTF((tempBuffer+offset,"%02x ", *pData++));
            i++;
            if(++i == 0x10)
    		{
    			offset += SII_SPRINTF((tempBuffer+offset,"%c", '\n'));
    			i = 0;
    		}
        }
        offset += SII_SPRINTF((tempBuffer+offset,"%c", '\n'));
        SII_LIB_LOG_DEBUG2(tempBuffer);
        SiiLibMallocDelete(tempBuffer);
    }
}
#endif
#ifndef HDMI_BUILD_IN_BOOT
void sNotifyHdmiState(HdmiTxObj_t *pObj)
{
	if(pObj->cbFunc)
		pObj->cbFunc(pObj->pConfig->instTx, SII_DRV_TX_EVENT__HDMI_STATE_CHNG);
}
#endif
/***** end of file ***********************************************************/
