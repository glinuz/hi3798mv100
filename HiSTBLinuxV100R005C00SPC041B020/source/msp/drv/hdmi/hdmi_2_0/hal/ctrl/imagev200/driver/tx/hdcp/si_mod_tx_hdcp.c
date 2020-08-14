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
* @file si_drv_tx_hdcp.c
*
* @brief Tx HDCP API
*
*****************************************************************************/
#ifdef HDMI_HDCP_SUPPORT
//#define SII_DEBUG

/***** #include statements ***************************************************/

#include "si_datatypes.h"
#include "sii_time.h"
#include "si_drv_tx_regs_enums.h"
#include "si_drv_cra_api.h"
#include "si_drv_tx_api.h"
#include "si_lib_log_api.h"
#include "si_lib_malloc_api.h"
#include "si_lib_seq_api.h"
#include "si_lib_time_api.h"

/***** Register Module name **************************************************/

SII_LIB_OBJ_MODULE_DEF(drv_tx_hdcp);


/***** public macro definitions **********************************************/
#define HDCP2X_RETRY_THRESHOLD					7
#define HDCP2X_CONTENT_TYPE_SET_MAX_TIMEOUT		10	
#define LEN_TPI__DDC_FIFO_SIZE                  16

#define TIMER_START__TX_HDCP__INTR          	150
#define TIMER_START__TX_HDCP__STATE         	40
#define TIMER_START__TX_HDCP__INTR_INTVAL     	10
#define TIMER_START__TX_HDCP__STATE_INTVAL      20 

#define HDCP1X_QUERY_STATUS_FAIL_TIMES          5

/***** local type definitions ************************************************/
/**
* @brief HDCP state
*/
typedef enum
{
	SII_MOD_HDCP_EVENT__OFF,
	SII_MOD_HDCP_EVENT__WAIT_FOR_START,
	SII_MOD_HDCP_EVENT__WAIT_FOR_DONE,
	SII_MOD_HDCP_EVENT__WAIT_FIFO_READY,
	SII_MOD_HDCP_EVENT__WAIT_FIFO_READ_DONE,
	SII_MOD_HDCP_EVENT__AUTHENTICATED,
	SII_MOD_HDCP_EVENT__FAILED,
} SiiDrvTxHdcpInternalState_t;

typedef struct
{
	SiiModHdcpConfig_t				*pConfig;
	IntStat_t                       hdcp1xIntStat;
//#if SII_INC_HDCP2XCORE
	IntStat_t						hdcp2xIntStat;
	uint8_t							authFailCounter;
//#endif

	/*--------------------------------*/
	/* User request states            */
	/*--------------------------------*/
	bool_t                          isAuthRequested;            //!< set if downstream authentication is requested
	SiiModHdcpState_t				hdcpStatus;
	bool_t							bBksvListApproved;
	/*--------------------------------*/
	/* Internal states                */
	/*--------------------------------*/
	SiiDrvTxHdcpInternalState_t     authState;                  //!< authentication state SiiModHdcpState_t
	SiiDrvTxHdcpInternalState_t     prevAuthState;              //!< previous value of authState
	SiiDrvHdcpKsvList_t  			ksvList;
	SiiDrvHdcpTopology_t			hdcpTopology;
    unsigned long                   hdcp1xPrepEnTime;
    unsigned long                   fifoReadDoneTime;
//#if SII_INC_HDCP2XCORE
	uint8_t                         dsHdcp_2_2_Supported;       //!< down stream HDCP2.2 capability
	SiiDrvHdcpContentType_t	   	    hdcpContentType;			//!< hdcp2.2 content type
	SiiDrvHdcp2xCupdChkStat_t		hdcp2xCupdStat;
//#endif
	SiiInst_t			 	        instTimerIsrPoll;			//!< Timer for interrupts
	SiiInst_t			 	        instTimerStatePoll;			//!< Timer for state machine

	HdcpEventNotifyCallBack			cbFunc;

	uint32_t			            hdcp2x_seq_num_m;
	bool_t				            hdcp2x_repeater_ready;

	bool_t                          isTxHpdAsserted;
    uint8_t                         u8QuerySatusFailTimes;
} HdcpObj_t;

/***** local prototypes ******************************************************/

static void sUpdateHdcpState(HdcpObj_t *hdcpObj);
static void sHdcp2ContentTypeSet(HdcpObj_t *hdcpObj, SiiDrvHdcpContentType_t pContentType);
static SiiModDsHdcpVersion_t sDsHdcpVerGet(HdcpObj_t *hdcpObj);

static SiiDrvCraAddr_t sbaseAddrGet(HdcpObj_t* hdcpObj);
static SiiInst_t sCraInstGet(HdcpObj_t* hdcpObj);
static void sResetKsvFifo(HdcpObj_t* hdcpObj);
static void sToggleHwTpiBit(HdcpObj_t* hdcpObj);
static void sStartHdcp(HdcpObj_t* hdcpObj, bool_t bEnable);
static void sTpiHdcpProtectionEnable(HdcpObj_t* hdcpObj, bool_t isEnabled);
static bool_t sTpiIsDownstreamHdcpAvailable(HdcpObj_t* hdcpObj);
static uint8_t sTpiHdcpStatusGet(HdcpObj_t* hdcpObj);
static bool_t sTpiHdcpIsPart2Done(HdcpObj_t* hdcpObj);
static void sTpiBksvGet(HdcpObj_t* hdcpObj, uint8_t* pBksv);
static bool_t sTpiKsvListPortionSizeGet(HdcpObj_t* hdcpObj, uint8_t *pBytesToRead);
static void sTpiKsvListGet(HdcpObj_t* hdcpObj, uint8_t *pBuffer, uint8_t length);
static SiiDrvTxHdcpInternalState_t sTpiGetKSVList(HdcpObj_t* hdcpObj, uint8_t dsBstatus[2]);
static void sTpiBStatusGet(HdcpObj_t* hdcpObj, uint8_t *pDsBStatus);
static void sPrintKsvList(HdcpObj_t* hdcpObj);
static void sPrintHdcpStatus(HdcpObj_t* hdcpObj);
static void sPrintHdcpQueryStatus(HdcpObj_t* hdcpObj, uint8_t query);
static void sHdcpStateMachineHandler(SiiInst_t inst);
static void sVirtualIsrHandler(HdcpObj_t* hdcpObj);
//static void sTpiHdcpEncriptionEnable(HdcpObj_t* hdcpObj, bool_t isEnable);
//static void sTpiHdcpDynamicAuthenticationEnable(HdcpObj_t* hdcpObj, bool_t isEnabled);
//static bool_t sTpiHdcpIsAuthenticationGood(HdcpObj_t* hdcpObj);
static void sHdcp2xIntrHandler(HdcpObj_t *hdcpObj);
static void sHdcp2xCodeUpdate(HdcpObj_t* hdcpObj);
static void Hdcp22AuthStop(HdcpObj_t* hdcpObj);
//static bool_t sWaitForDdcBus(HdcpObj_t* hdcpObj);
//static ddcComErr_t sUpdateEdid(HdcpObj_t* hdcpObj, uint8_t segmentIndex, uint8_t regAddr, uint8_t *pBuf, uint16_t length);
static void SiiDrvTpiHdcp2ProtectionEnable(HdcpObj_t* hdcpObj , bool_t isEnabled);
static bool_t DsHdcp22SupportGet(HdcpObj_t* hdcpObj);
static void Hdcp22AuthStart(HdcpObj_t* hdcpObj);
static bool_t IsDSdeviceHDCP2Repeater(HdcpObj_t *hdcpObj);
static void sRcvIdListGet(HdcpObj_t* hdcpObj);
static void sUpdateHdcpTopology(HdcpObj_t* hdcpObj);

//Notify functions
static void sNotifyHdcpStatus(HdcpObj_t* hdcpObj);

static void sHdcp2xCupdStatusGet(HdcpObj_t* hdcpObj);
static bool_t sClearI2cDeadlock(HdcpObj_t* hdcpObj);

static void sSoftwareInit(HdcpObj_t* hdcpObj);

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
SiiInst_t SiiModHdcpCreate(char *pNameStr, SiiModHdcpConfig_t *pConfig)
{
	HdcpObj_t*	hdcpObj = NULL;
	SiiDrvCraAddr_t	baseAddr;
	SiiInst_t		craInst;

	/* Allocate memory for object */
	hdcpObj = (HdcpObj_t*)SII_LIB_OBJ_CREATE(pNameStr, sizeof(HdcpObj_t));
	SII_PLATFORM_DEBUG_ASSERT(hdcpObj);

	hdcpObj->pConfig = (SiiModHdcpConfig_t*)SiiLibMallocCreate(sizeof(SiiModHdcpConfig_t));
	SII_MEMCPY(hdcpObj->pConfig, pConfig, sizeof(SiiModHdcpConfig_t));

	//Register Callback Function
	hdcpObj->cbFunc = hdcpObj->pConfig->cbFunc;

	baseAddr = sbaseAddrGet(hdcpObj);
	craInst	 = sCraInstGet(hdcpObj);
	
	/*--------------------------------*/
	/* Initialize user request states */
	/*--------------------------------*/
 	if( pConfig->maxDsDev )
	{
		hdcpObj->ksvList.pListStart = hdcpObj->ksvList.pList = (uint8_t*)SiiLibMallocCreate(pConfig->maxDsDev*sizeof(SiiDrvHdcpKsv_t));
		hdcpObj->ksvList.length  = 0;
		SII_PLATFORM_DEBUG_ASSERT(hdcpObj->ksvList.pList);
	}
    
    sSoftwareInit(hdcpObj);
#if 0
    hdcpObj->isAuthRequested	= false;
	hdcpObj->bBksvListApproved	= false;
	hdcpObj->hdcpStatus			= SII_MOD_HDCP_STATUS_OFF;

	/*--------------------------------*/
	/* Initialize user status         */
	/*--------------------------------*/
	hdcpObj->authState				= SII_MOD_HDCP_EVENT__OFF;
	hdcpObj->dsHdcp_2_2_Supported	= false;
	hdcpObj->hdcp2xIntStat.reg0		= 0;
	hdcpObj->hdcp2xIntStat.reg1		= 0;
	hdcpObj->authFailCounter		= 0;

	/*-------Set HDCP Topology to default----------*/
	hdcpObj->hdcpTopology.depth				= 0;
	hdcpObj->hdcpTopology.deviceCount		= 0;
	hdcpObj->hdcpTopology.hdcp1xRepeaterDs	= 0;
	hdcpObj->hdcpTopology.hdcp20RepeaterDs	= 0;
	hdcpObj->hdcpTopology.maxCascadeExceeded = 0;
	hdcpObj->hdcpTopology.maxDevsExceeded	= 0;	
#endif
	//if(hdcpObj->pConfig->bHdcp2xEn)
	{
		//HDCP2x Patch Update
//		sHdcp2xCodeUpdate(hdcpObj);
#if 0
        SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_CTL_1		, BIT_MSK__HDCP2X_CTL_1__REG_HDCP2X_REAUTH_SW);
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_TP0		, 0x02);  // HDCP2X TP0=2    2
        SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_TP1		, 0x8c);  // HDCP2X TP1=24   24 (20MHz->98, 24MHz->117 27MHz->140)

 
        SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_TP2		, 0x01);  // HDCP2X TP2=1    1
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_TP3		, 0x32);  // HDCP2X TP3=50(0x32)   42(0x2A)
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_TP4		, 0x1E);  // HDCP2X TP4=30(0x1E)   17(0x11)
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_TP5		, 0x78);  // HDCP2X TP5=120(0x78)  100(0x64)
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_TP6		, 0x02);  // HDCP2X TP6=2    2
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_TP7		, 0x0A);  // HDCP2X TP7=10(0x0A)   9(0x09)
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_TP8		, 0x0A);  // HDCP2X TP8=10(0x0A)   9(0x09) Cert read timeout
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_TP9		, 0x14);  // HDCP2X TP9=20(0x14)   17(0x11)
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_TP10		, 0x16);  // HDCP2X TP10=22(0x16)  17(0x11)
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_TP11		, 0xC8);  // HDCP2X TP11=200(0xC8) 167(0xA7)
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_TP12		, 0x60);  // HDCP2X TP12=150(0x96) 125(0x7D)
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_TP13		, 0x10);  // HDCP2X TP13=16(0x10)  13(0xD)
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_TP14		, 0xC8);  // HDCP2X TP14=200(0xC8) 250(0xFA)
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_TP15		, 0x00);  // HDCP2X TP15=0   0

		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_GP_IN0		, 0x00);  // 
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_GP_IN1		, 0x22);  // 0x22, 0x00 
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_GP_IN2		, 0x80);  // 0x80, 0x01
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_GP_IN3		, 0x00);  // 
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_POLL_VAL0	, 0x05);  // DDC polling interval

        SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_GP_OUT0    , 0xB0);  //
 
        SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_INTR0		, 0xFF);  // Clearing Intr0
        SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_INTR1		, 0xFF);  // clearing Intr1
#endif
    }
	// Create a timer to update hdcp states
	hdcpObj->instTimerStatePoll = SII_LIB_SEQ_TIMER_CREATE("HDCP_State_Machine_Handler", sHdcpStateMachineHandler, SII_LIB_OBJ_INST(hdcpObj), 252, SII_TIMER_HDCP);
	SII_PLATFORM_DEBUG_ASSERT(hdcpObj->instTimerStatePoll);

	return SII_LIB_OBJ_INST(hdcpObj);
}

void SiiModHdcpDelete(SiiInst_t inst)
{
	HdcpObj_t* hdcpObj = (HdcpObj_t*)SII_LIB_OBJ_PNTR(inst);

	SiiLibSeqTimerDelete(hdcpObj->instTimerStatePoll);
	SiiLibMallocDelete(hdcpObj->ksvList.pList);
	SiiLibMallocDelete(hdcpObj->pConfig);
	SII_LIB_OBJ_DELETE(hdcpObj);
}


void SiiModHdcpInterruptHandler(SiiInst_t inst)
{
	HdcpObj_t*		hdcpObj		= (HdcpObj_t*)SII_LIB_OBJ_PNTR(inst);
	SiiDrvCraAddr_t baseAddr    = sbaseAddrGet(hdcpObj);
	SiiInst_t		craInst		= sCraInstGet(hdcpObj);
	IntStat_t       intStat		= {0};

	if(hdcpObj->pConfig->bHdcp2xEn)
	{
		if(DsHdcp22SupportGet(hdcpObj))
		{
			sHdcp2xIntrHandler(hdcpObj);
			return;
		}
	}

    // Capture and mask interrupt status bits. (ignore status bits and non-serving interrupts)
	intStat.reg0 = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__TPI_INTR_ST0);// & BIT_MSK__TPI_INTR_ST0__TPI_INTR_ST7;
	intStat.reg1 = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__TPI_INTR_ST0) & BIT_MSK__TPI_INTR_ST0__TPI_INTR_ST3;
	
	
	if(SII_MEMCMP(&hdcpObj->hdcp1xIntStat, &intStat, sizeof(IntStat_t)))
	{
		uint8_t intreg = 0;

		// Find any interrupt status bit that changed to '1'
		intreg |= ((hdcpObj->hdcp1xIntStat.reg0 ^ intStat.reg0) & intStat.reg0);
		intreg |= ((hdcpObj->hdcp1xIntStat.reg1 ^ intStat.reg1) & intStat.reg1);

		hdcpObj->hdcp1xIntStat = intStat;
		if(intreg)
		{
			// Call derived interupt handler
			sVirtualIsrHandler(hdcpObj);
		}
	}
}

static void sSoftwareInit(HdcpObj_t* hdcpObj)
{
	/*--------------------------------*/
	/* Initialize user status         */
	/*--------------------------------*/
	hdcpObj->isTxHpdAsserted = false;
    hdcpObj->ksvList.length        = 0;
    hdcpObj->isAuthRequested	   = false;
	hdcpObj->bBksvListApproved	   = false;
	hdcpObj->hdcp2x_seq_num_m      = 0;
	hdcpObj->hdcpContentType       = SII_DRV_HDCP_CONTENT_TYPE__0;
	hdcpObj->hdcp2x_repeater_ready = false;
	hdcpObj->hdcpStatus			   = SII_MOD_HDCP_STATUS_OFF;

	hdcpObj->authState			   = SII_MOD_HDCP_EVENT__OFF;
	hdcpObj->dsHdcp_2_2_Supported  = false;
	hdcpObj->hdcp2xIntStat.reg0	   = 0;
	hdcpObj->hdcp2xIntStat.reg1	   = 0;
	hdcpObj->authFailCounter	   = 0;

	/*-------Set HDCP Topology to default----------*/
	hdcpObj->hdcpTopology.depth				= 0;
	hdcpObj->hdcpTopology.deviceCount		= 0;
	hdcpObj->hdcpTopology.hdcp1xRepeaterDs	= 0;
	hdcpObj->hdcpTopology.hdcp20RepeaterDs	= 0;
	hdcpObj->hdcpTopology.maxCascadeExceeded = 0;
	hdcpObj->hdcpTopology.maxDevsExceeded	= 0;	
}

static void sHardwareInit(HdcpObj_t* hdcpObj)
{
	SiiDrvCraAddr_t baseAddr	= sbaseAddrGet(hdcpObj);
	SiiInst_t craInst			= sCraInstGet(hdcpObj);

	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_CTL_1		, 0x00);
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_TP0		, 0x02);  // HDCP2X TP0=2
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_TP1		, 0x8c);  // HDCP2X TP1=24, 20MHz->98, 24MHz->117

    // 20151004, add by longhua for hdcp auth failed bug
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_CTL_1      , BIT_MSK__HDCP2X_CTL_1__REG_HDCP2X_REAUTH_SW);
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_TP0        , 0x02);  // HDCP2X TP0=2    2
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_TP1        , 0x8c);  // HDCP2X TP1=24   24 (20MHz->98, 24MHz->117 27MHz->140)
        
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_TP2        , 0x01);  // HDCP2X TP2=1    1
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_TP3        , 0x32);  // HDCP2X TP3=50(0x32)   42(0x2A)
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_TP4        , 0x1E);  // HDCP2X TP4=30(0x1E)   17(0x11)
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_TP5        , 0x78);  // HDCP2X TP5=120(0x78)  100(0x64)
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_TP6        , 0x02);  // HDCP2X TP6=2    2
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_TP7        , 0x0A);  // HDCP2X TP7=10(0x0A)   9(0x09)
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_TP8        , 0x0A);  // HDCP2X TP8=10(0x0A)   9(0x09) Cert read timeout
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_TP9        , 0x14);  // HDCP2X TP9=20(0x14)   17(0x11)
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_TP10       , 0x16);  // HDCP2X TP10=22(0x16)  17(0x11)
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_TP11       , 0xC8);  // HDCP2X TP11=200(0xC8) 167(0xA7)
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_TP12       , 0x60);  // HDCP2X TP12=150(0x96) 125(0x7D)
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_TP13       , 0x10);  // HDCP2X TP13=16(0x10)  13(0xD)
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_TP14       , 0xC8);  // HDCP2X TP14=200(0xC8) 250(0xFA)
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_TP15       , 0x00);  // HDCP2X TP15=0   0
    
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_GP_IN0     , 0x00);  // 
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_GP_IN1     , 0x22);  // 0x22, 0x00 
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_GP_IN2     , 0x80);  // 0x80, 0x01
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_GP_IN3     , 0x00);  // 
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_POLL_VAL0  , 0x05);  // DDC polling interval
    
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_GP_OUT0    , 0xB0);  //

    
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_INTR0		, 0xFF);  // Clearing Intr0
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_INTR1		, 0xFF);  // clearing Intr1
    /* add by q00352704 HDCP1.4 encryption enable */
    SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__TPI_COPP_DATA2	, BIT_MSK__TPI_COPP_DATA2__REG_INTR_ENCRYPTION);  // disable encryption
}
#if 0
static void sTPIReset(HdcpObj_t* hdcpObj, uint8_t bReset)
{
    if (bReset)
    {
        SiiDrvCraSetBit8(sCraInstGet(hdcpObj), sbaseAddrGet(hdcpObj) | REG_ADDR__TPI_HW_OPT0, BIT_MSK__TPI_HW_OPT0__REG_HW_TPI_SM_RST);     
    }
    else
    {
        SiiDrvCraClrBit8(sCraInstGet(hdcpObj), sbaseAddrGet(hdcpObj) | REG_ADDR__TPI_HW_OPT0, BIT_MSK__TPI_HW_OPT0__REG_HW_TPI_SM_RST);
    }
}
#endif
static void sHdcpHardwareStatusGet(HdcpObj_t* hdcpObj, SiiHdcpStatus_t* hdcpStatus)
{
	SiiDrvCraAddr_t baseAddr	= sbaseAddrGet(hdcpObj);
	SiiInst_t craInst			= sCraInstGet(hdcpObj);

    //hdcpStatus->authState       = hdcpObj->authState;

    if (SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__HDCP2X_CTL_0) & BIT_MSK__HDCP2X_CTL_0__REG_HDCP2X_EN)
    {
        hdcpStatus->bHdcp2_2Enable = true;
    }
    else
    {
        hdcpStatus->bHdcp2_2Enable = false;
    }

    if (SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__TPI_COPP_DATA2) & BIT_MSK__TPI_COPP_DATA2__REG_TPI_HDCP_PREP_EN)
    {
        hdcpStatus->bHdcp1_4Enable = true;
    }
    else
    {
        hdcpStatus->bHdcp1_4Enable = false;
    }

}

static void sHdcp2xReset(HdcpObj_t* hdcpObj)
{
    SiiDrvCraAddr_t baseAddr	= sbaseAddrGet(hdcpObj);
	SiiInst_t craInst			= sCraInstGet(hdcpObj);
    
    SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__HDCP2X_POLL_CS, BIT_MSK__HDCP2X_DS_POLL_EN);
    //Diasabling HDCP2.2 Encryption
    SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__HDCP2X_CTL_0, (BIT_MSK__HDCP2X_CTL_0__REG_HDCP2X_ENCRYPT_EN | BIT_MSK__HDCP2X_CTL_0__REG_HDCP2X_EN)); 
    SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__HDCP2X_TX_CTRL_0,BIT_MSK__HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_SMNG_XFER_START);
    //hdcp2x_core_reset
    SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__HDCP2X_TX_SRST, BIT_MSK__HDCP2X_TX_SRST__REG_HDCP2X_CRST);
    //SiiLibTimeMilliDelay(5);
    SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__HDCP2X_TX_SRST, BIT_MSK__HDCP2X_TX_SRST__REG_HDCP2X_CRST);
}

//Todo : Try to return ErrorStatus
//-------------------------------------------------------------------------------------------------
//! @brief      HDMI TX module Set API
//-------------------------------------------------------------------------------------------------
bool_t SiiModHdcpSet(SiiInst_t inst, SiiModHdcpInternalOpcode_t opcode, void *inData)
{

	HdcpObj_t* hdcpObj = (HdcpObj_t*)SII_LIB_OBJ_PNTR(inst);

	switch(opcode)
	{
	case SII_MOD_HDCP_OPCODE__HDCP_ENABLE:
		{
			uint8_t hdcpEnable = *(uint8_t *)inData;
			sStartHdcp(hdcpObj, hdcpEnable);
			break;
		}
	case SII_MOD_HDCP_OPCODE__HDCP_VERSION:
        {
            SiiModDsHdcpVersion_t dsHdcpVer =  *(SiiModDsHdcpVersion_t*)inData;
            if (dsHdcpVer == SII_DRV_DS_HDCP_VER__22)
            {
                hdcpObj->pConfig->bHdcp2xEn = true;
            }
            else
            {
                hdcpObj->pConfig->bHdcp2xEn = false;
            }
            break;
        }
	case SII_MOD_HDCP_OPCODE__HDCP_CONTENT_TYPE:
		{
			SiiDrvHdcpContentType_t contentType = *(SiiDrvHdcpContentType_t*)inData;
			sHdcp2ContentTypeSet(hdcpObj, contentType);
			break;
		}
	case SII_MOD_HDCP_OPCODE__HDCP_BKSV_LIST_APPROVAL:
		{
			SII_MEMCPY(&hdcpObj->bBksvListApproved, inData, sizeof(hdcpObj->bBksvListApproved));
			break;
		}
	case SII_MOD_HDCP_OPCODE__HDCP_HW_INIT:
		{
            sHardwareInit(hdcpObj);
			break;
		}
	case SII_MOD_HDCP_OPCODE__HDCP_SW_INIT:
		{
            sSoftwareInit(hdcpObj);
			break;
		}
	case SII_MOD_HDCP_OPCODE__TX_HPD_STATUS:
	{
		SII_MEMCPY(&hdcpObj->isTxHpdAsserted, inData, sizeof(hdcpObj->isTxHpdAsserted));
		break;
	}
    case SII_MOD_HDCP_OPCODE__HDCP2x_RESET:
    {
        sHdcp2xReset(hdcpObj);
        break;
    }
	default:
		break;
	}
  
	return true;
}

bool_t SiiModHdcpGet(SiiInst_t inst, SiiModHdcpInternalOpcode_t opcode, void *outData)
{
	HdcpObj_t* hdcpObj = (HdcpObj_t*)SII_LIB_OBJ_PNTR(inst);

	switch(opcode)
	{
	case SII_MOD_HDCP_OPCODE__HDCP_STATUS:
		sUpdateHdcpState(hdcpObj);
		SII_MEMCPY(outData, &(hdcpObj->hdcpStatus), sizeof(hdcpObj->hdcpStatus));
		break;
	case SII_MOD_HDCP_OPCODE__HDCP_VERSION:
		{
			SiiModDsHdcpVersion_t dsHdcpVer = sDsHdcpVerGet(hdcpObj);
			SII_MEMCPY(outData, &dsHdcpVer, sizeof(dsHdcpVer));
			break;
		}
	case SII_MOD_HDCP_OPCODE__HDCP_BKSV_LIST:
		SII_MEMCPY(outData, &(hdcpObj->ksvList), sizeof(hdcpObj->ksvList));
		break;
	case SII_MOD_HDCP_OPCODE__HDCP_TOPOLOGY:
		sUpdateHdcpTopology(hdcpObj);
		SII_MEMCPY(outData, &(hdcpObj->hdcpTopology), sizeof(hdcpObj->hdcpTopology));
		break;
	case SII_MOD_HDCP_OPCODE__HDCP2X_CUPD_STAT:
        sHdcp2xCupdStatusGet(hdcpObj);
		SII_MEMCPY(outData, &(hdcpObj->hdcp2xCupdStat), sizeof(hdcpObj->hdcp2xCupdStat));
		break;
	case SII_MOD_HDCP_OPCODE__HDCP_HW_STATUS:
        sHdcpHardwareStatusGet(hdcpObj, (SiiHdcpStatus_t*)outData);
		break;
    default:
        break;
	}
	return true;
}

/***** local functions *******************************************************/
static void sUpdateHdcpState(HdcpObj_t *hdcpObj)
{
	switch (hdcpObj->authState)
	{
	case SII_MOD_HDCP_EVENT__OFF:
		hdcpObj->hdcpStatus = SII_MOD_HDCP_STATUS_OFF;
		break;
	case SII_MOD_HDCP_EVENT__WAIT_FOR_START:
	case SII_MOD_HDCP_EVENT__WAIT_FOR_DONE:
	case SII_MOD_HDCP_EVENT__WAIT_FIFO_READY:
		hdcpObj->hdcpStatus = SII_MOD_HDCP_STATUS_AUTHENTICATING;
		break;
	case SII_MOD_HDCP_EVENT__AUTHENTICATED:
		hdcpObj->hdcpStatus = SII_MOD_HDCP_STATUS_SUCCESS;
		break;
	case SII_MOD_HDCP_EVENT__FAILED:
		hdcpObj->hdcpStatus = SII_MOD_HDCP_STATUS_FAILURE;
		break;
    default:
        break;
    }
}

static SiiModDsHdcpVersion_t sDsHdcpVerGet(HdcpObj_t *hdcpObj)
{
    if(hdcpObj->pConfig->bHdcp2xEn)
        return SII_DRV_DS_HDCP_VER__22;
    else
        return SII_DRV_DS_HDCP_VER__1X;
}

static void sHdcp2ContentTypeSet(HdcpObj_t *hdcpObj, SiiDrvHdcpContentType_t pContentType)
{
	SiiDrvCraAddr_t baseAddr	= sbaseAddrGet(hdcpObj);
	SiiInst_t craInst			= sCraInstGet(hdcpObj);
	uint8_t timeout;

	if(SII_DRV_DS_HDCP_VER__22 == sDsHdcpVerGet(hdcpObj))
	{
		hdcpObj->hdcp2x_seq_num_m = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__HDCP2X_RX_SEQ_NUM_M_0);
		hdcpObj->hdcp2x_seq_num_m = (hdcpObj->hdcp2x_seq_num_m << 8) |
			SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__HDCP2X_RX_SEQ_NUM_M_1);
		hdcpObj->hdcp2x_seq_num_m = (hdcpObj->hdcp2x_seq_num_m << 8) |
			SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__HDCP2X_RX_SEQ_NUM_M_2);

		SiiDrvCraWrReg8(craInst,  baseAddr | REG_ADDR__HDCP2X_RPT_SMNG_K,     0x01);		// k -> always 0x01/*REG_ADDR__HDCP2X_RPT_SMNG_K*/
		SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__HDCP2X_TX_CTRL_0,      BIT_MSK__HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_SMNG_WR_START);	// smng_wr_start=1
		SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__HDCP2X_TX_CTRL_0,      BIT_MSK__HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_SMNG_WR);	// smng_wr=1
		SiiDrvCraWrReg8(craInst,  baseAddr | REG_ADDR__HDCP2X_TX_RPT_SMNG_IN, pContentType);		// smng_in (type)
		SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__HDCP2X_TX_CTRL_0,      BIT_MSK__HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_SMNG_WR);	// smng_wr=0
		SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__HDCP2X_TX_CTRL_0,      BIT_MSK__HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_SMNG_WR);	// smng_wr=1
		SiiDrvCraWrReg8(craInst,  baseAddr | REG_ADDR__HDCP2X_TX_RPT_SMNG_IN, 0x00);		// smng_in (stream ID)
		SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__HDCP2X_TX_CTRL_0,      BIT_MSK__HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_SMNG_WR);	// smng_wr=0
		SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__HDCP2X_TX_CTRL_0,      BIT_MSK__HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_SMNG_XFER_START);	// smng_xfer_start=1-rising edge will start transfer
		SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__HDCP2X_TX_CTRL_0,	  BIT_MSK__HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_SMNG_XFER_START);	// smng_xfer_start=0-just to make sure
        hdcpObj->hdcp2x_seq_num_m++;

		SiiDrvCraWrReg8(craInst,  baseAddr | REG_ADDR__HDCP2X_RX_SEQ_NUM_M_0, (uint8_t)hdcpObj->hdcp2x_seq_num_m);		// seq_num_m0
		SiiDrvCraWrReg8(craInst,  baseAddr | REG_ADDR__HDCP2X_RX_SEQ_NUM_M_1, (uint8_t)hdcpObj->hdcp2x_seq_num_m >> 8);		// seq_num_m1
		SiiDrvCraWrReg8(craInst,  baseAddr | REG_ADDR__HDCP2X_RX_SEQ_NUM_M_2, (uint8_t)hdcpObj->hdcp2x_seq_num_m >> 16);		// seq_num_m2

		SiiLibTimeMilliDelay(5);
		timeout = HDCP2X_CONTENT_TYPE_SET_MAX_TIMEOUT;
		while(--timeout && !( SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__HDCP2X_TX_STATUS) & BIT_MSK__HDCP2X_TX_STATUS__RO_HDCP2TX_RPT_SMNG_XFER_DONE)) // Wait smng_xfer_done
		{
			SiiLibTimeMilliDelay(1);
		}
		if(!timeout)
		{
			SII_LIB_LOG_DEBUG1(hdcpObj, "Unable to set contentType... Stream Manage Transfer failed");
		}
		else
			SII_LIB_LOG_DEBUG1(hdcpObj, "ContentType set to %s", pContentType ? "SII_DRV_HDCP_CONTENT_TYPE__1" : "SII_DRV_HDCP_CONTENT_TYPE__0");
	}
	else
	{
		SII_LIB_LOG_DEBUG1(hdcpObj, "Unable to set contentType... DS Device is not HDCP2.2 Capable\n");
	}

}


/***** local functions *******************************************************/
static void sHdcp2streamManageMessageSet(HdcpObj_t *hdcpObj, SiiDrvHdcpContentType_t pContentType)
{
	SiiDrvCraAddr_t baseAddr	= sbaseAddrGet(hdcpObj);
	SiiInst_t craInst			= sCraInstGet(hdcpObj);
	//uint8_t timeout;

	if(SII_DRV_DS_HDCP_VER__22 == sDsHdcpVerGet(hdcpObj))
	{
		SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__HDCP2X_CTL_0, BIT_MSK__HDCP2X_CTL_0__REG_HDCP2X_ENCRYPT_EN);
/* 980B 1B-10 auth failed, becase hdcp2x_seq_num_m always send 1 (reg REG_ADDR__HDCP2X_RX_SEQ_NUM_M_0 is 0). */        
        hdcpObj->hdcp2x_seq_num_m = hdcpObj->hdcp2x_seq_num_m & 0xffffff;
        if(hdcpObj->hdcp2x_seq_num_m == 0xffffff)
        {
            hdcpObj->hdcp2x_seq_num_m = 0;
        }
        else
        {
            hdcpObj->hdcp2x_seq_num_m++;
        }

		SiiDrvCraWrReg8(craInst,  baseAddr | REG_ADDR__HDCP2X_RPT_SMNG_K,     0x01);		// k -> always 0x01/*REG_ADDR__HDCP2X_RPT_SMNG_K*/
		SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__HDCP2X_TX_CTRL_0,      BIT_MSK__HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_SMNG_WR_START);	// smng_wr_start=1
		SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__HDCP2X_TX_CTRL_0,      BIT_MSK__HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_SMNG_WR);	// smng_wr=1
		SiiDrvCraWrReg8(craInst,  baseAddr | REG_ADDR__HDCP2X_TX_RPT_SMNG_IN, pContentType);		// smng_in (stream ID)
		SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__HDCP2X_TX_CTRL_0,      BIT_MSK__HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_SMNG_WR);	// smng_wr=0
		SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__HDCP2X_TX_CTRL_0,      BIT_MSK__HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_SMNG_WR);	// smng_wr=1
		SiiDrvCraWrReg8(craInst,  baseAddr | REG_ADDR__HDCP2X_TX_RPT_SMNG_IN, 0x00);		// smng_in (type)
		SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__HDCP2X_TX_CTRL_0,      BIT_MSK__HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_SMNG_WR);	// smng_wr=0
		SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__HDCP2X_TX_CTRL_0,      BIT_MSK__HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_SMNG_XFER_START);	// smng_xfer_start=1-rising edge will start transfer

		SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__HDCP2X_TX_CTRL_0,	  BIT_MSK__HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_SMNG_XFER_START);	// smng_xfer_start=0-just to make sure
                
//        SII_LIB_LOG_PRINT1(hdcpObj, ("Sending stream message...\n"));

		SiiDrvCraWrReg8(craInst,  baseAddr | REG_ADDR__HDCP2X_RX_SEQ_NUM_M_0, (uint8_t)hdcpObj->hdcp2x_seq_num_m);		// seq_num_m0
		SiiDrvCraWrReg8(craInst,  baseAddr | REG_ADDR__HDCP2X_RX_SEQ_NUM_M_1, (uint8_t)hdcpObj->hdcp2x_seq_num_m >> 8);		// seq_num_m1
		SiiDrvCraWrReg8(craInst,  baseAddr | REG_ADDR__HDCP2X_RX_SEQ_NUM_M_2, (uint8_t)hdcpObj->hdcp2x_seq_num_m >> 16);		// seq_num_m2
	}
	else
	{
		SII_LIB_LOG_PRINT1(hdcpObj, ("Unable to set contentType... DS Device is not HDCP2.2 Capable\n"));
	}
}


static void sUpdateHdcpTopology(HdcpObj_t* hdcpObj)
{
	uint8_t hdcpMiscStatus;
	SiiDrvCraAddr_t baseAddr	= sbaseAddrGet(hdcpObj);
	SiiInst_t craInst			= sCraInstGet(hdcpObj);
	if(hdcpObj->pConfig->bHdcp2xEn && DsHdcp22SupportGet(hdcpObj))
	{
		hdcpObj->hdcpTopology.depth = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__HDCP2X_RPT_DEPTH);
		hdcpObj->hdcpTopology.deviceCount = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__HDCP2X_RPT_DEVCNT);

		hdcpMiscStatus = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__HDCP2X_RPT_DETAIL);
		hdcpObj->hdcpTopology.maxDevsExceeded = (hdcpMiscStatus & BIT_MSK__HDCP2X_RPT_DETAIL__RI_HDCP2RX_RPT_MX_DEVS_EXC) ? true : false;
		hdcpObj->hdcpTopology.maxCascadeExceeded = (hdcpMiscStatus & BIT_MSK__HDCP2X_RPT_DETAIL__RI_HDCP2RX_RPT_MX_CASC_EXC) ? true : false;
		hdcpObj->hdcpTopology.hdcp20RepeaterDs = (hdcpMiscStatus & BIT_MSK__HDCP2X_RPT_DETAIL__RI_HDCP2RX_RPT_HDCP20RPT_DSTRM) ? true : false;
		hdcpObj->hdcpTopology.hdcp1xRepeaterDs = (hdcpMiscStatus & BIT_MSK__HDCP2X_RPT_DETAIL__RI_HDCP2RX_RPT_HDCP1DEV_DSTRM) ? true : false;
	}
	else
	{
		uint8_t bStatus[2] = {0};
		SiiDrvCraBlockRead8(craInst, baseAddr | REG_ADDR__TPI_BSTATUS1, bStatus, 2);
		hdcpObj->hdcpTopology.depth = bStatus[1] & BIT_MSK__TPI_BSTATUS2__REG_DS_DEPTH;
		hdcpObj->hdcpTopology.deviceCount = bStatus[0] & BIT_MSK__TPI_BSTATUS1__REG_DS_DEV_CNT;
		hdcpObj->hdcpTopology.maxDevsExceeded =	bStatus[0] & BIT_MSK__TPI_BSTATUS1__REG_DS_DEV_EXCEED;
		hdcpObj->hdcpTopology.maxCascadeExceeded = bStatus[1] & BIT_MSK__TPI_BSTATUS2__REG_DS_CASC_EXCEED;
		hdcpObj->hdcpTopology.hdcp20RepeaterDs = false;
		hdcpObj->hdcpTopology.hdcp1xRepeaterDs = false;

	}
}

static SiiDrvCraAddr_t sbaseAddrGet(HdcpObj_t* hdcpObj)
{
	return hdcpObj->pConfig->baseAddrTx;
}

static SiiInst_t sCraInstGet(HdcpObj_t* hdcpObj)
{
	return hdcpObj->pConfig->instTxCra;
}

static void sResetKsvFifo(HdcpObj_t* hdcpObj)
{
	SII_LIB_LOG_DEBUG1(hdcpObj, "Resetting KSV fifo!!\n");
	hdcpObj->ksvList.length = 0;
	hdcpObj->ksvList.pList = hdcpObj->ksvList.pListStart;
}

static void sSetHwTpiBit(HdcpObj_t* hdcpObj)
{
	SiiDrvCraAddr_t baseAddr	= sbaseAddrGet(hdcpObj);
	SiiInst_t craInst			= sCraInstGet(hdcpObj);

	SII_LIB_LOG_DEBUG1(hdcpObj, ("Set HW TPI bit!!\n"));
	SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__LM_DDC, BIT_MSK__LM_DDC__REG_SW_TPI_EN );
}

static void sClearHwTpiBit(HdcpObj_t* hdcpObj)
{
	SiiDrvCraAddr_t baseAddr	= sbaseAddrGet(hdcpObj);
	SiiInst_t craInst			= sCraInstGet(hdcpObj);

	SII_LIB_LOG_DEBUG1(hdcpObj, ("Clear HW TPI bit!!\n"));
	SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__LM_DDC, BIT_MSK__LM_DDC__REG_SW_TPI_EN );
}

static void sToggleHwTpiBit(HdcpObj_t* hdcpObj)
{
	SiiDrvCraAddr_t baseAddr	= sbaseAddrGet(hdcpObj);
	SiiInst_t craInst			= sCraInstGet(hdcpObj);

	SII_LIB_LOG_DEBUG1(hdcpObj, "Toggle HW TPI bit!!\n");
	//Toggle h/w TPI bit:: necessary as it triggers the HDCP interrupts, without it HDCP interrupts may not be triggered by h/w
	SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__LM_DDC, BIT_MSK__LM_DDC__REG_SW_TPI_EN );
	SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__LM_DDC, BIT_MSK__LM_DDC__REG_SW_TPI_EN );
}

static void sResetTpiStateMachine(HdcpObj_t* hdcpObj)
{
	SiiDrvCraAddr_t baseAddr	= sbaseAddrGet(hdcpObj);
	SiiInst_t craInst			= sCraInstGet(hdcpObj);
	SII_LIB_LOG_DEBUG1(hdcpObj, ("Resetting TPI State Machine!!!!!\n"));
	
	/* workaround for hardare ddc issue with scdc module */
	SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__SCDC_CTL,
		BIT_MSK__SCDC_CTL__REG_SCDC_AUTO_REPLY); // enable SCDC auto reply read request from slave
					//for SCDC registers up_flag0 and up_flag1
	SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__TPI_HW_OPT0,
		BIT_MSK__TPI_HW_OPT0__REG_HW_TPI_SM_RST );
	SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__TPI_HW_OPT0,
		BIT_MSK__TPI_HW_OPT0__REG_HW_TPI_SM_RST );
	
	/* workaround for hardare ddc issue with scdc module */
//	SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__SCDC_CTL,
//		BIT_MSK__SCDC_CTL__REG_SCDC_AUTO_REPLY); // enable SCDC auto reply read request from slave
					//for SCDC registers up_flag0 and up_flag1
}


//-------------------------------------------------------------------------------------------------
//! @brief      Start HDCP
//!
//! @param[in]  Start authentication
//-------------------------------------------------------------------------------------------------
static void sStartHdcp(HdcpObj_t* hdcpObj, bool_t bEnable)
{
	SiiDrvCraAddr_t baseAddr	= sbaseAddrGet(hdcpObj);
	SiiInst_t craInst			= sCraInstGet(hdcpObj);
    uint8_t timeout             = 10;
    uint8_t avmuteStatus        = 0;
	hdcpObj->authState			= SII_MOD_HDCP_EVENT__OFF;
	hdcpObj->isAuthRequested	= bEnable;
	
	if(bEnable)
	{
		if(hdcpObj->isTxHpdAsserted)
		{
            do
            {
                msleep(50);
                SiiDrvCraWrReg8(sCraInstGet(hdcpObj), sbaseAddrGet(hdcpObj) | REG_ADDR__TPI_INFO_FSEL, BIT_ENUM__TPI_INFO_FSEL__GEN2);
                avmuteStatus = SiiDrvCraRdReg8(sCraInstGet(hdcpObj), sbaseAddrGet(hdcpObj) | REG_ADDR__TPI_INFO_EN);
            }while((avmuteStatus != 0x00) && (timeout--));
            
			SII_LIB_LOG_DEBUG1(hdcpObj, "HDCP ON, timeout(%d)!!\n", timeout);
			SiiLibSeqTimerStart(hdcpObj->instTimerStatePoll, TIMER_START__TX_HDCP__STATE, TIMER_START__TX_HDCP__STATE_INTVAL);
            SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__HDCP2X_POLL_CS, BIT_MSK__HDCP2X_DS_POLL_EN);
            /* HDCP1.4 encryption disable. */
            SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__TPI_COPP_DATA2, BIT_MSK__TPI_COPP_DATA2__REG_INTR_ENCRYPTION);
		}
	}
	else
	{
		SiiLibSeqTimerStop(hdcpObj->instTimerStatePoll);
		if(hdcpObj->pConfig->bHdcp2xEn && DsHdcp22SupportGet(hdcpObj))
		{
			Hdcp22AuthStop(hdcpObj);
			SiiDrvTpiHdcp2ProtectionEnable(hdcpObj, false);
			hdcpObj->dsHdcp_2_2_Supported = 0;
            //hdcpObj->pConfig->bHdcp2xEn   = false;
			sResetKsvFifo(hdcpObj);

			//Disable HDCP2x Intr Masks
			SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_INTR0_MASK, 0x0); //HDCP2x Interrupt0 Mask Register
			SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_INTR1_MASK, 0x0); //HDCP2x Interrupt1 Mask Register
		}
		else
		{
            hdcpObj->u8QuerySatusFailTimes = 0;
			sResetKsvFifo(hdcpObj);
			SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__TPI_COPP_DATA2, BIT_MSK__TPI_COPP_DATA2__REG_TPI_HDCP_PREP_EN, CLEAR_BITS);
			SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__TPI_COPP_DATA2, BIT_MSK__TPI_COPP_DATA2__REG_CANCEL_PROT_EN, SET_BITS);
			SiiLibTimeMilliDelay(5);
			sTpiHdcpProtectionEnable(hdcpObj, false);
		    //Disable load the KSV from OPT
		    SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__EPCM, BIT_MSK__EPCM__REG_LD_KSV, CLEAR_BITS);

			//DisableHDCP1x Intr Masks
			SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__TPI_INTR_EN, 0x0);
		}
		SII_LIB_LOG_DEBUG1(hdcpObj, "HDCP OFF!!\n");
		hdcpObj->authState = SII_MOD_HDCP_EVENT__OFF;
		sPrintHdcpStatus(hdcpObj);
		sNotifyHdcpStatus(hdcpObj);
	}
}

//-------------------------------------------------------------------------------------------------
//! @brief      Enable/Disable HDCP protection.
//!
//! @param[in]  isEnabled - true, if HDCP protection has to be enabled.
//-------------------------------------------------------------------------------------------------

static void sTpiHdcpProtectionEnable(HdcpObj_t* hdcpObj, bool_t isEnabled)
{
	SiiDrvCraPutBit8(sCraInstGet(hdcpObj), sbaseAddrGet(hdcpObj) | REG_ADDR__TPI_COPP_DATA2, HDCP_CTRL_MODE, isEnabled ? HDCP_CTRL_MODE : CLEAR_BITS);
}

static uint8_t sTpiHdcpAuthStatusGet(HdcpObj_t* hdcpObj)
{
	uint8_t hdcpStatus = SiiDrvCraRdReg8(sCraInstGet(hdcpObj), sbaseAddrGet(hdcpObj) | REG_ADDR__TPI_HW_DBG5);
	return hdcpStatus;
}

//-------------------------------------------------------------------------------------------------
//! @brief      Return HDCP Status byte.
//-------------------------------------------------------------------------------------------------

static uint8_t sTpiHdcpStatusGet(HdcpObj_t* hdcpObj)
{
	uint8_t hdcpStatus = SiiDrvCraRdReg8(sCraInstGet(hdcpObj), sbaseAddrGet(hdcpObj) | REG_ADDR__TPI_COPP_DATA1);
	return hdcpStatus;
}

//-------------------------------------------------------------------------------------------------
//! @brief      Return true if Sink HDCP is available, false otherwise.
//-------------------------------------------------------------------------------------------------

static bool_t sTpiIsDownstreamHdcpAvailable(HdcpObj_t* hdcpObj)
{
	uint8_t hdcpStatus = SiiDrvCraRdReg8(sCraInstGet(hdcpObj), sbaseAddrGet(hdcpObj) | REG_ADDR__TPI_COPP_DATA1);
	return ((hdcpStatus & BIT_MSK__TPI_COPP_DATA1__REG_COPP_PROTYPE) != 0);
}

//-------------------------------------------------------------------------------------------------
//! @brief      Check if HDCP authentication Part 2 is successfully done.
//-------------------------------------------------------------------------------------------------

static bool_t sTpiHdcpIsPart2Done(HdcpObj_t* hdcpObj)
{
	uint8_t hdcp_status = sTpiHdcpStatusGet(hdcpObj);
	return (PART2_DONE == (hdcp_status & PART2_DONE));
}

//-------------------------------------------------------------------------------------------------
//! @brief      Read BKSV that is 8*5 = 40 bits.
//!
//! @param[in]  pBksv - pointer to an array to store the BKSV.
//-------------------------------------------------------------------------------------------------

static void sTpiBksvGet(HdcpObj_t* hdcpObj, uint8_t* pBksv)
{
	SiiDrvCraBlockRead8(sCraInstGet(hdcpObj), sbaseAddrGet(hdcpObj) | REG_ADDR__TPI_WR_BKSV_1, pBksv, SII_BKSV_LIST_BYTES);
}


//-------------------------------------------------------------------------------------------------
//! @brief      Return a number of bytes in the KSV list FIFO ready to be read and Done flag.
//!
//!             To be used in repeater operation when DS KSV list is copied from DS to US.
//!
//! @param[out] pBytesToRead - pointer to a variable holding the number of bytes to read,
//!
//! @return     KSV List reading done flag.
//! @retval     true - if a final portion of the KSV list is awaiting to be read.
//-------------------------------------------------------------------------------------------------

static bool_t sTpiKsvListPortionSizeGet(HdcpObj_t* hdcpObj, uint8_t *pBytesToRead)
{
	bool_t isDone;
	uint8_t fifoStatus = SiiDrvCraRdReg8(sCraInstGet(hdcpObj), sbaseAddrGet(hdcpObj) | REG_ADDR__TPI_KSV_FIFO_STAT);

	*pBytesToRead = fifoStatus & BIT_MSK__TPI_KSV_FIFO_STAT__KSV_FIFO_BYTES;
	isDone = ((fifoStatus & BIT_MSK__TPI_KSV_FIFO_STAT__KSV_FIFO_LAST) != 0);

	return isDone;
}


//-------------------------------------------------------------------------------------------------
//! @brief      Get DS BKSV list.
//!
//! @param[in]  pBuffer - pointer to a buffer for the KSV list storage,
//! @param[in]  length  - number of bytes to read.
//-------------------------------------------------------------------------------------------------

static void sTpiKsvListGet(HdcpObj_t* hdcpObj, uint8_t *pBuffer, uint8_t length)
{
	// Note: this FIFO register is a special case. While reading from it in burst mode
	// the slave I2C interface don't increment the offset after every single reading.
	// Content of the register gets immediately updated by data waiting in the FIFO
	// after every reading.
	SiiDrvCraFifoRead8(sCraInstGet(hdcpObj), sbaseAddrGet(hdcpObj) | REG_ADDR__TPI_KSV_FIFO_FORW, pBuffer, length);
}

//-------------------------------------------------------------------------------------------------
//! @brief      Send BSTATUS to US device FIFO.
//-------------------------------------------------------------------------------------------------

static SiiDrvTxHdcpInternalState_t sTpiGetKSVList(HdcpObj_t* hdcpObj, uint8_t dsBstatus[2])
{
	uint8_t bytesToRead;
	uint8_t dsCount = dsBstatus[0] & BIT_MSK__TPI_BSTATUS1__REG_DS_DEV_CNT;
	uint8_t dsDepth = dsBstatus[1] & BIT_MSK__TPI_BSTATUS2__REG_DS_DEPTH;
	int16_t fifoByteCounter;
	bool_t  isDone = false;
    int16_t timeout = 1000;
    
	if((dsCount > 0x7F) || (dsDepth > 0x07))
	{
		SII_LIB_LOG_ERR(hdcpObj, "Error:: DS device count: %02X, DS device depth: %02X\n", dsCount, dsDepth);
		return SII_MOD_HDCP_EVENT__FAILED;
	}
	
	hdcpObj->ksvList.length = (dsCount * SII_BKSV_LIST_BYTES);
	fifoByteCounter = hdcpObj->ksvList.length;
	
    while (!isDone && timeout && fifoByteCounter)
    {
        isDone = sTpiKsvListPortionSizeGet(hdcpObj, &bytesToRead);
        if (bytesToRead > 0)  
        {
    		// get DS BKSV list
    		sTpiKsvListGet(hdcpObj, hdcpObj->ksvList.pList, bytesToRead);
    		hdcpObj->ksvList.pList += bytesToRead;
    		fifoByteCounter -= bytesToRead;
        }
        else
        {
            timeout--;
            SiiLibTimeMilliDelay(1);
        }

    }

    if (!isDone && !timeout && fifoByteCounter)
    {
        SII_LIB_LOG_ERR(hdcpObj, "Read BKSV list timeout\n");
        return SII_MOD_HDCP_EVENT__FAILED;       
    }
	return SII_MOD_HDCP_EVENT__WAIT_FIFO_READ_DONE;
}

//-------------------------------------------------------------------------------------------------
//! @brief      Get DS BSTATUS information.
//!
//! @param[out] pDsBStatus - pointer to a buffer of length 2
//-------------------------------------------------------------------------------------------------

static void sTpiBStatusGet(HdcpObj_t* hdcpObj, uint8_t *pDsBStatus)
{
	SiiDrvCraBlockRead8(sCraInstGet(hdcpObj), sbaseAddrGet(hdcpObj) | REG_ADDR__TPI_BSTATUS1, pDsBStatus, 2);
}

static void sPrintKsvList(HdcpObj_t* hdcpObj)
{
	uint8_t		*ptrTemp;
	uint16_t	length;

	length = hdcpObj->ksvList.length;
	ptrTemp = hdcpObj->ksvList.pListStart;
	while(length--)
	{
		SII_LIB_LOG_DEBUG2(" %02x\n", *ptrTemp);
		ptrTemp++;
	}
}

//-------------------------------------------------------------------------------------------------
//! @brief      Show HDCP authentication phase and error messages, if any.
//-------------------------------------------------------------------------------------------------

static void sPrintHdcpStatus(HdcpObj_t* hdcpObj)
{
	if (hdcpObj->prevAuthState != hdcpObj->authState)
	{
		switch (hdcpObj->authState)
		{
		case SII_MOD_HDCP_EVENT__OFF:
			SII_LIB_LOG_DEBUG1(hdcpObj, "DS HDCP: OFF\n");
			break;
		case SII_MOD_HDCP_EVENT__WAIT_FOR_START:
			SII_LIB_LOG_DEBUG1(hdcpObj, "DS HDCP: WAIT FOR START\n");
			break;
		case SII_MOD_HDCP_EVENT__WAIT_FOR_DONE:
			SII_LIB_LOG_DEBUG1(hdcpObj, "DS HDCP: WAIT FOR DONE\n");
			break;
		case SII_MOD_HDCP_EVENT__WAIT_FIFO_READY:
			SII_LIB_LOG_DEBUG1(hdcpObj, "DS HDCP: WAIT FOR FIFO READY\n");
			break;
		case SII_MOD_HDCP_EVENT__AUTHENTICATED:
			SII_LIB_LOG_DEBUG1(hdcpObj, "DS HDCP: AUTHENTICATED\n");
			break;
		case SII_MOD_HDCP_EVENT__FAILED:
			SII_LIB_LOG_DEBUG1(hdcpObj, "DS HDCP: FAILED\n");
            break;
        case SII_MOD_HDCP_EVENT__WAIT_FIFO_READ_DONE:
            SII_LIB_LOG_DEBUG1(hdcpObj, "DS HDCP: WAIT FIFO READ DONE\n");
			break;
		}
		hdcpObj->prevAuthState = hdcpObj->authState;
	}
}

//-------------------------------------------------------------------------------------------------
//! @brief      Show HDCP query status
//-------------------------------------------------------------------------------------------------

static void sPrintHdcpQueryStatus(HdcpObj_t* hdcpObj, uint8_t query)
{
	SII_LIB_LOG_DEBUG1(hdcpObj, "HDCP Query Status:\n");
	SII_LIB_LOG_DEBUG1(hdcpObj, "Link:                 \n");

	switch (query &  BIT_MSK__TPI_COPP_DATA1__REG_COPP_LINK_STATUS)
	{
	case BIT_ENUM__TPI_HDCP_QUERY__STATUS_NORMAL:
		SII_LIB_LOG_DEBUG1(hdcpObj, "Normal\n");
		break;
	case BIT_ENUM__TPI_HDCP_QUERY__STATUS_LOST:
		SII_LIB_LOG_DEBUG1(hdcpObj, "Lost\n");
		break;
	case BIT_ENUM__TPI_HDCP_QUERY__STATUS_FAILED:
		SII_LIB_LOG_DEBUG1(hdcpObj, "Failed\n");
		break;
	//case BIT_ENUM__TPI_HDCP_QUERY__STATUS_SUSPENDED:
	//	SII_LIB_LOG_DEBUG1(hdcpObj, ("Suspended\n"));
	//	break;
	}

	SII_LIB_LOG_DEBUG1(hdcpObj, "DS HDCP:              \n");
	if (query & BIT_MSK__TPI_COPP_DATA1__REG_COPP_PROTYPE)
	{
		SII_LIB_LOG_DEBUG1(hdcpObj, "Available\n");
	}
	else
	{
		SII_LIB_LOG_DEBUG1(hdcpObj, "Unavailable\n");
	}

	SII_LIB_LOG_DEBUG1(hdcpObj, "Repeater:             \n");
	if (query & BIT_MSK__TPI_COPP_DATA1__REG_COPP_HDCP_REP)
	{
		SII_LIB_LOG_DEBUG1(hdcpObj, "Yes\n");
	}
	else
	{
		SII_LIB_LOG_DEBUG1(hdcpObj, "No\n");
	}

	SII_LIB_LOG_DEBUG1(hdcpObj, "Connected Sink Protection:     \n");
	if (query & BIT_MSK__TPI_COPP_DATA1__REG_COPP_LPROT)
	{
		SII_LIB_LOG_DEBUG1(hdcpObj, "Yes\n");
	}
	else
	{
		SII_LIB_LOG_DEBUG1(hdcpObj, "No\n");
	}

	SII_LIB_LOG_DEBUG1(hdcpObj, "Connected Repeater Protection:     \n");
	if (query & BIT_MSK__TPI_COPP_DATA1__REG_COPP_GPROT)
	{
		SII_LIB_LOG_DEBUG1(hdcpObj, "Yes\n");
	}
	else
	{
		SII_LIB_LOG_DEBUG1(hdcpObj, "No\n");
	}
}

static bool_t sI2cIsDeadlock(HdcpObj_t* hdcpObj)
{
    uint8_t ddcStatus;
    ddcStatus = SiiDrvCraRdReg8(sCraInstGet(hdcpObj), sbaseAddrGet(hdcpObj) | REG_ADDR__DDC_STATUS);
    if (BIT_MSK__DDC_STATUS__REG_DDC_BUS_LOW == (ddcStatus & BIT_MSK__DDC_STATUS__REG_DDC_BUS_LOW))
    {
        SII_LIB_LOG_WARN(hdcpObj, "I2C is deadlock while reading BCAPs and BASV form sink\n");
        return true;
    }

    return false;
}

static bool_t sClearI2cDeadlock(HdcpObj_t* hdcpObj)
{
    uint8_t timeout = 16;
    
    while(!(BIT_MSK__DDC_MANUAL__IO_DSDA & SiiDrvCraRdReg8(sCraInstGet(hdcpObj), sbaseAddrGet(hdcpObj) | REG_ADDR__DDC_MANUAL)) && timeout-- > 0)
    {
        //pull scl high
        SiiDrvCraWrReg8(sCraInstGet(hdcpObj), sbaseAddrGet(hdcpObj) | REG_ADDR__DDC_MANUAL, 0xb1);
        SiiLibTimeMilliDelay(1);
        //pull scl low
        SiiDrvCraWrReg8(sCraInstGet(hdcpObj), sbaseAddrGet(hdcpObj) | REG_ADDR__DDC_MANUAL, 0xa1);
        SiiLibTimeMilliDelay(1);
    }
    
    if (timeout > 0 && (BIT_MSK__DDC_MANUAL__IO_DSDA & SiiDrvCraRdReg8(sCraInstGet(hdcpObj), sbaseAddrGet(hdcpObj) | REG_ADDR__DDC_MANUAL)))
    {
        //pull scl high
        SiiDrvCraWrReg8(sCraInstGet(hdcpObj), sbaseAddrGet(hdcpObj) | REG_ADDR__DDC_MANUAL, 0xb1);    
        SiiLibTimeMilliDelay(1);
        //pull sda low
        SiiDrvCraWrReg8(sCraInstGet(hdcpObj), sbaseAddrGet(hdcpObj) | REG_ADDR__DDC_MANUAL, 0x91);           
        SiiLibTimeMilliDelay(1);
        SiiDrvCraWrReg8(sCraInstGet(hdcpObj), sbaseAddrGet(hdcpObj) | REG_ADDR__DDC_MANUAL, 0x11);  
        SII_LIB_LOG_DEBUG1(hdcpObj, "Clear I2C deadlock success\n");
        return true;
    }
    else
    {
        SII_LIB_LOG_ERR(hdcpObj, "Clear I2C deadlock fail\n");
        return false;
    }
}

//-------------------------------------------------------------------------------------------------
//! @brief      HDCP Timer event handler.
//!
//!             This function is to be called periodically. The time past from the last call
//!             should be indicated as a parameter.
//-------------------------------------------------------------------------------------------------

static void sHdcpStateMachineHandler(SiiInst_t inst)
{
	HdcpObj_t*		hdcpObj		= (HdcpObj_t*)SII_LIB_OBJ_PNTR(inst);
	SiiDrvCraAddr_t baseAddr	= sbaseAddrGet(hdcpObj);
	SiiInst_t		craInst		= sCraInstGet(hdcpObj);
  
	switch (hdcpObj->authState)
	{
	case SII_MOD_HDCP_EVENT__OFF:
		if (hdcpObj->isAuthRequested)
		{
            sSetHwTpiBit(hdcpObj);
            sResetTpiStateMachine(hdcpObj);
#if 0
            // Check if Downstrem is capable of HDCP2.2 
			if(hdcpObj->pConfig->bHdcp2xEn)
			{
				// Check DDC 0x50 to read the HDCP2.2 capability
				if(hdcpObj->dsHdcp_2_2_Supported != 0x04)
				{
					if(SI_TX_DDC_ERROR_CODE_NO_ERROR != sUpdateEdid(hdcpObj, 0xFF, 0x50, &hdcpObj->dsHdcp_2_2_Supported, 1))
					{
						// Second attempt to make sure
						if(SI_TX_DDC_ERROR_CODE_NO_ERROR != sUpdateEdid(hdcpObj, 0xFF, 0x50, &hdcpObj->dsHdcp_2_2_Supported, 1))
						{
							//SiiDrvTxDdcReset();
							SII_LIB_LOG_ERR(hdcpObj, "Error happened during HDCP2.2 Capability read!!\n");
						}
						else
						{
							SII_LIB_LOG_DEBUG1(hdcpObj, "HDCP2.2 capability has been read successfully!!\n");
						}
					}
					else
					{
						SII_LIB_LOG_DEBUG1(hdcpObj, "HDCP2.2 capability has been read successfully!!\n");
					}
				}
			}
#endif            
			if(hdcpObj->pConfig->bHdcp2xEn && DsHdcp22SupportGet(hdcpObj) )
			{
				SII_LIB_LOG_DEBUG1(hdcpObj, "Down Stream: HDCP2.2 capable!!\n");
				sResetTpiStateMachine(hdcpObj);
                SiiDrvTpiHdcp2ProtectionEnable(hdcpObj, true);
				Hdcp22AuthStart(hdcpObj);

				//Enable HDCP2x Interrupts
				SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_INTR0_MASK, BIT_MSK__HDCP2X_INTR0_MASK__INTR0_MASK_B0 | \
					                                                             BIT_MSK__HDCP2X_INTR0_MASK__INTR0_MASK_B1 | \
					                                                             BIT_MSK__HDCP2X_INTR0_MASK__INTR0_MASK_B2 | \
					                                                             BIT_MSK__HDCP2X_INTR0_MASK__INTR0_MASK_B3 | \
                                                                                 BIT_MSK__HDCP2X_INTR0_MASK__INTR0_MASK_B6);
				SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_INTR1_MASK, BIT_MSK__HDCP2X_INTR1_MASK__INTR1_MASK_B0 | \
                                                                                 BIT_MSK__HDCP2X_INTR1_MASK__INTR1_MASK_B2 | \
                                                                                 BIT_MSK__HDCP2X_INTR1_MASK__INTR1_MASK_B5);
				
				hdcpObj->authState = SII_MOD_HDCP_EVENT__WAIT_FOR_DONE;
			}
			else
			{
				sClearHwTpiBit(hdcpObj);
				sResetTpiStateMachine(hdcpObj);
				SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__TPI_INTR_ST0, 0xff);

                SII_LIB_LOG_DEBUG1(hdcpObj, "Down Stream: HDCP1.4 capable!!\n");
                SII_LIB_LOG_DEBUG1(hdcpObj, "TPI_HW_DBG5:0x%x\n", SiiDrvCraRdReg8(craInst, baseAddr | 0x367D));
                SII_LIB_LOG_DEBUG1(hdcpObj, "TPI_HW_DBG6:0x%x\n", SiiDrvCraRdReg8(craInst, baseAddr | 0x367E));
                SII_LIB_LOG_DEBUG1(hdcpObj, "TPI_HW_DBG7:0x%x\n", SiiDrvCraRdReg8(craInst, baseAddr | 0x367F));
                SII_LIB_LOG_DEBUG1(hdcpObj, "DDC_STATUS:0x%x\n", SiiDrvCraRdReg8(craInst, baseAddr | 0x30F2));
                SII_LIB_LOG_DEBUG1(hdcpObj, "DDC_FIFO_CNT:0x%x\n", SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__DDC_DOUT_CNT));
                    
    		    //Enable load the KSV from OPT
    		    SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__EPCM, BIT_MSK__EPCM__REG_LD_KSV, SET_BITS);
                SiiLibTimeMilliDelay(20);
    		    SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__EPCM, BIT_MSK__EPCM__REG_LD_KSV, CLEAR_BITS);   
                SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__TPI_COPP_DATA2, BIT_MSK__TPI_COPP_DATA2__REG_TPI_HDCP_PREP_EN, BIT_MSK__TPI_COPP_DATA2__REG_TPI_HDCP_PREP_EN);
				sToggleHwTpiBit(hdcpObj);  
				hdcpObj->authState = SII_MOD_HDCP_EVENT__WAIT_FOR_START;
                hdcpObj->hdcp1xPrepEnTime = SiI_get_global_time();
			}
		}
		break;

	case SII_MOD_HDCP_EVENT__WAIT_FOR_START:
		// TX chip input is provided and stable
		// (If scaler or/and OSD are installed in the schematic,
		// their output signals are provided and stable as well as their input ones)
        if (sTpiIsDownstreamHdcpAvailable(hdcpObj)) // DDC ACK check
		{
     		// DS device must be ready to start HDCP authentication
            SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__TPI_COPP_DATA2, BIT_MSK__TPI_COPP_DATA2__REG_CANCEL_PROT_EN, SET_BITS);
			SiiLibTimeMilliDelay(5);
			sTpiHdcpProtectionEnable(hdcpObj, false); // just in case
			SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__TPI_COPP_DATA2, BIT_MSK__TPI_COPP_DATA2__REG_CANCEL_PROT_EN, CLEAR_BITS);
			sTpiHdcpProtectionEnable(hdcpObj, true);  // start authentication			
			SiiLibTimeMilliDelay(100);
			//Enable Masks for HDCP1x Interrupts
			SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__TPI_INTR_EN, BIT_MSK__TPI_INTR_EN__REG_TPI_INTR_MASK_B3 | BIT_MSK__TPI_INTR_EN__REG_TPI_INTR_MASK_B7 | BIT_MSK__TPI_INTR_EN__REG_TPI_INTR_MASK_B6);
			hdcpObj->authState = SII_MOD_HDCP_EVENT__WAIT_FOR_DONE;
			//SiiLibSeqTimerStart(hdcpObj->instTimerIsrPoll, TIMER_START__TX_HDCP__INTR, TIMER_START__TX_HDCP__INTR_INTVAL);
		}
        else//I2C maybe happen deadlock while reading BCAPs and BKSV from sink
        {
           if ((SiI_get_global_time() - hdcpObj->hdcp1xPrepEnTime) > 5000)
           {
               if (sI2cIsDeadlock(hdcpObj))
               {
                   if (!sClearI2cDeadlock(hdcpObj))
                   {
                        hdcpObj->authState = SII_MOD_HDCP_EVENT__FAILED;
                        sNotifyHdcpStatus(hdcpObj);
                   }
               }
               else
               {
                    hdcpObj->authState = SII_MOD_HDCP_EVENT__FAILED;
                    SII_LIB_LOG_ERR(hdcpObj, "Down Stream: HDCP1.4 capable!!\n");
                    SII_LIB_LOG_ERR(hdcpObj, "TPI_HW_DBG5:0x%x\n", SiiDrvCraRdReg8(craInst, baseAddr | 0x367D));
                    SII_LIB_LOG_ERR(hdcpObj, "TPI_HW_DBG6:0x%x\n", SiiDrvCraRdReg8(craInst, baseAddr | 0x367E));
                    SII_LIB_LOG_ERR(hdcpObj, "TPI_HW_DBG7:0x%x\n", SiiDrvCraRdReg8(craInst, baseAddr | 0x367F));
                    SII_LIB_LOG_ERR(hdcpObj, "DDC_STATUS:0x%x\n", SiiDrvCraRdReg8(craInst, baseAddr | 0x30F2));
                    SII_LIB_LOG_ERR(hdcpObj, "DDC_FIFO_CNT:0x%x\n", SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__DDC_DOUT_CNT));
                    SII_LIB_LOG_ERR(hdcpObj, "Read BCAPs and BKSV from sink timeout\n");
                    sNotifyHdcpStatus(hdcpObj);
               }
           }
       }
		break;

	case SII_MOD_HDCP_EVENT__WAIT_FIFO_READ_DONE:
		if (sTpiHdcpIsPart2Done(hdcpObj))
		{
			hdcpObj->authState = SII_MOD_HDCP_EVENT__AUTHENTICATED;
            /* if HDCP1.4 auth success, denable encryption. */
            SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__TPI_COPP_DATA2, BIT_MSK__TPI_COPP_DATA2__REG_INTR_ENCRYPTION);
            sNotifyHdcpStatus(hdcpObj);
		}
        else
        {
            /* repeater need to reAuthentication*/
            if ((SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__TPI_KSV_V) & BIT_MSK__TPI_KSV_V__TPI_AUTH_STATE) == 0x40)
            {
                 SII_LIB_LOG_WARN(hdcpObj, "Repeater reAuthentication!!\n");
                 sStartHdcp(hdcpObj, false);
                 sStartHdcp(hdcpObj, true);
            }
            else if ((SiI_get_global_time() - hdcpObj->fifoReadDoneTime) > 5000)
            {
                SII_LIB_LOG_ERR(hdcpObj, "Auth3 authentication timeout\n");
                hdcpObj->authState = SII_MOD_HDCP_EVENT__FAILED;
                sNotifyHdcpStatus(hdcpObj);
            }
        }
		break;
	case SII_MOD_HDCP_EVENT__AUTHENTICATED:
		break;

	case SII_MOD_HDCP_EVENT__FAILED:
		break;

	default:
		break;
	}

	sPrintHdcpStatus(hdcpObj);

}

//#if SII_INC_HDCP2XCORE
static void sRcvIdListGet(HdcpObj_t* hdcpObj)
{
	SiiDrvCraAddr_t baseAddr	= sbaseAddrGet(hdcpObj);
	SiiInst_t		craInst		= sCraInstGet(hdcpObj);

	//uint8_t dsDepth = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__HDCP2X_RPT_DEPTH);
	uint8_t dsCount = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__HDCP2X_RPT_DEVCNT);
	uint16_t fifoByteCounter;
	sResetKsvFifo(hdcpObj);
	hdcpObj->ksvList.length = dsCount * SII_HDCP2X_RCVID_LENGTH;
	fifoByteCounter = hdcpObj->ksvList.length;

	SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__HDCP2X_TX_CTRL_0, BIT_MSK__HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_RCVID_RD_START); //rcvid_rd_start=1
	while(fifoByteCounter--)
	{
		*(hdcpObj->ksvList.pList) = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__HDCP2X_TX_RPT_RCVID_OUT);
		hdcpObj->ksvList.pList += 1;
		SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__HDCP2X_TX_CTRL_0, BIT_MSK__HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_RCVID_RD); //rcvid_rd=1
		SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__HDCP2X_TX_CTRL_0, BIT_MSK__HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_RCVID_RD); //rcvid_rd=0
	}
	SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__HDCP2X_TX_CTRL_0, BIT_MSK__HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_RCVID_RD_START); //rcvid_rd_start=0
}

static bool_t IsDSdeviceHDCP2Repeater(HdcpObj_t *hdcpObj)
{
	uint8_t val;
	// return 1 if HDCP 2 repeater
	val = SiiDrvCraRdReg8(sCraInstGet(hdcpObj), sbaseAddrGet(hdcpObj) | REG_ADDR__HDCP2X_GEN_STATUS);
	return ( val & BIT_MSK__HDCP2X_GEN_STATUS__RO_HDCP2_REPEATER) ? true : false;
}

//-------------------------------------------------------------------------------------------------
//! @brief      HDCP2x Interrupt handler routine.
//!
//!             More detailed description.
//!
//! @param[in]  Hdcp Object
//-------------------------------------------------------------------------------------------------
static void sHdcp2xVirtualIsrHandler(HdcpObj_t* hdcpObj)
{
	SiiDrvCraAddr_t baseAddr    = sbaseAddrGet(hdcpObj);
	SiiInst_t		craInst		= sCraInstGet(hdcpObj);
	uint8_t hdcp2AuthStatus = 0;
	uint8_t hdcp2AuthStateStatus = 0;
	bool_t rpt_fail = 0;

	if(hdcpObj->hdcp2xIntStat.reg1 & BIT_MSK__HDCP2X_INTR1__INTR1_STAT2) //Ake sent interrupt received
	{
		SiiDrvCraWrReg8(craInst,  baseAddr | REG_ADDR__HDCP2X_RX_SEQ_NUM_M_0, 0x00);
		SiiDrvCraWrReg8(craInst,  baseAddr | REG_ADDR__HDCP2X_RX_SEQ_NUM_M_1, 0x00);
		SiiDrvCraWrReg8(craInst,  baseAddr | REG_ADDR__HDCP2X_RX_SEQ_NUM_M_2, 0x00);
		hdcpObj->hdcp2x_seq_num_m = 0;
		hdcpObj->hdcp2x_repeater_ready = false;
		SII_LIB_LOG_DEBUG1(hdcpObj, ("Ake Init sent"));
	}

	if(hdcpObj->hdcp2xIntStat.reg0 & BIT_MSK__HDCP2X_INTR0__INTR0_STAT2) //Repeater Ready Interrupt
	{
		// Check repeater parameters
		//if(SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR_HDCP2X_MISC_STATUS) & (BIT_MASK_HDCP2X_DEVS_EXC | BIT_MASK_HDCP2X_MX_CASC_EXC))
		if(SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__HDCP2X_RPT_DETAIL) & \
			(BIT_MSK__HDCP2X_RPT_DETAIL__RI_HDCP2RX_RPT_MX_DEVS_EXC | BIT_MSK__HDCP2X_RPT_DETAIL__RI_HDCP2RX_RPT_MX_CASC_EXC))
		{
			// MAX_DEVS_EXCEEDED or MAX_CASCADE_EXCEEDED
			// if MAX_DEV set or MAX_CASC set re-auth after 1.5 secs
			SiiLibTimeMilliDelay(1500);
			rpt_fail = 1;
		}
        hdcpObj->hdcp2x_repeater_ready = true;
        SII_LIB_LOG_DEBUG1(hdcpObj, ("Repeater Ready"));
    }

    if(hdcpObj->hdcp2xIntStat.reg1 & BIT_MSK__HDCP2X_INTR1__INTR1_STAT0) //ReceiverID Changed Interrupt
	{
			// seq_num_v check
			uint32_t seq_num_v;
			seq_num_v = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__HDCP2X_RX_SEQ_NUM_V_2);
			seq_num_v = (seq_num_v << 8) | SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__HDCP2X_RX_SEQ_NUM_V_1);
			seq_num_v = (seq_num_v << 8) | SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__HDCP2X_RX_SEQ_NUM_V_0);
			if(seq_num_v == 0xffffff)
			{
				//Seq_num_v is neither valid nor equal to expected seq_num_v
				SII_LIB_LOG_DEBUG1(hdcpObj, ("Maximum sequence Number"));
			}
//			SII_LIB_LOG_DEBUG1(hdcpObj, ("seq_num_v:%x ", seq_num_v));
			//Read ReceiverID List
			sRcvIdListGet(hdcpObj);
			SII_LIB_LOG_DEBUG1(hdcpObj, ("Receiver ID List::  "));
			sPrintKsvList(hdcpObj);
		    if ((hdcpObj->hdcp2x_repeater_ready) && (!rpt_fail)) 
            {
			    if ((seq_num_v == 0x0000) && (hdcpObj->authState != SII_MOD_HDCP_EVENT__AUTHENTICATED)) 
                {
				   sHdcp2streamManageMessageSet(hdcpObj, hdcpObj->hdcpContentType);
			    }
        }
	}

	if(hdcpObj->hdcp2xIntStat.reg0 & BIT_MSK__HDCP2X_INTR0__INTR0_STAT3) //Hash fail
	{
		/*SII_LIB_LOG_DEBUG1(hdcpObj, ("Hash Fail  "));*/
		sHdcp2streamManageMessageSet(hdcpObj, hdcpObj->hdcpContentType);
	}

	if(hdcpObj->hdcp2xIntStat.reg1 & BIT_MSK__HDCP2X_INTR1__INTR1_STAT5) //stream manage message xfer done Interrupt
	{
		SII_LIB_LOG_DEBUG1(hdcpObj, ("Stream manage message xfer done"));
	}

	if(hdcpObj->hdcp2xIntStat.reg0 & BIT_MSK__HDCP2X_INTR0__INTR0_STAT0) //Authenctication Done Interrupt
	{
		hdcp2AuthStatus = SiiDrvCraRdReg8(craInst,  baseAddr | REG_ADDR__HDCP2X_AUTH_STAT );
		hdcp2AuthStateStatus = SiiDrvCraRdReg8(craInst,  baseAddr | REG_ADDR__HDCP2X_STATE );

		if((0x81 == hdcp2AuthStatus) && (0x2B == hdcp2AuthStateStatus))
		{
			hdcpObj->authFailCounter = 0;
			// HDCP2 Authentication Successfull
			// Check if downstream is a HDCP2 repeater
			if(IsDSdeviceHDCP2Repeater(hdcpObj))
			{
				SII_LIB_LOG_DEBUG2("\nDownstream repeater\n");
				SiiLibTimeMilliDelay(100); //If repeater, wait atleast 100ms before enabling encryption
			}
			SiiLibTimeMilliDelay(200);
			SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__HDCP2X_CTL_0, BIT_MSK__HDCP2X_CTL_0__REG_HDCP2X_ENCRYPT_EN);
			hdcpObj->authState = SII_MOD_HDCP_EVENT__AUTHENTICATED;
            SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__TPI_COPP_DATA2	, BIT_MSK__TPI_COPP_DATA2__REG_INTR_ENCRYPTION);
			sPrintHdcpStatus(hdcpObj);
			sNotifyHdcpStatus(hdcpObj);
		}
	}

	if((hdcpObj->hdcp2xIntStat.reg0 & BIT_MSK__HDCP2X_INTR0__INTR0_STAT1) || rpt_fail)
	{
		hdcpObj->authFailCounter++;

		if(hdcpObj->authFailCounter > HDCP2X_RETRY_THRESHOLD || rpt_fail)
		{
			SII_LIB_LOG_DEBUG1(hdcpObj, ("Auth fail"));
            hdcpObj->authFailCounter = 0;
			rpt_fail = 0;

			SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__HDCP2X_CTL_0, BIT_MSK__HDCP2X_CTL_0__REG_HDCP2X_ENCRYPT_EN);
			hdcpObj->authState = SII_MOD_HDCP_EVENT__FAILED;
			sStartHdcp(hdcpObj, false);
			sStartHdcp(hdcpObj, true);
		}
	}
	if(hdcpObj->hdcp2xIntStat.reg0 & BIT_MSK__HDCP2X_INTR0__INTR0_STAT6) {
	hdcp2AuthStatus = SiiDrvCraRdReg8(craInst,  baseAddr | 
		REG_ADDR__HDCP2X_AUTH_STAT );
	if (hdcp2AuthStatus & 0x40) {/* reauth request status*/
		SII_LIB_LOG_DEBUG1(hdcpObj, ("Reauth Request"));
		hdcpObj->authState = SII_MOD_HDCP_EVENT__FAILED;
		sStartHdcp(hdcpObj, false);
		sStartHdcp(hdcpObj, true);
	} else {
	}
}

}
//#endif
static void sTPIReset(HdcpObj_t* hdcpObj, uint8_t bReset)
{
    if (bReset)
    {
        SiiDrvCraSetBit8(sCraInstGet(hdcpObj), sbaseAddrGet(hdcpObj) | REG_ADDR__TPI_HW_OPT0, BIT_MSK__TPI_HW_OPT0__REG_HW_TPI_SM_RST);     
    }
    else
    {
        SiiDrvCraClrBit8(sCraInstGet(hdcpObj), sbaseAddrGet(hdcpObj) | REG_ADDR__TPI_HW_OPT0, BIT_MSK__TPI_HW_OPT0__REG_HW_TPI_SM_RST);
    }
}
//-------------------------------------------------------------------------------------------------
//! @brief      HDCP Interrupt handler routine.
//!
//!             More detailed description.
//!
//! @param[in]  Hdcp Object.
//-------------------------------------------------------------------------------------------------

static void sVirtualIsrHandler(HdcpObj_t* hdcpObj)
{
	SiiDrvCraAddr_t baseAddr    = sbaseAddrGet(hdcpObj);
	SiiInst_t		craInst		= sCraInstGet(hdcpObj);
    uint8_t         hdcpStatus  = 0;

    
	if (hdcpObj->hdcp1xIntStat.reg0 & BIT_MSK__TPI_INTR_ST0__TPI_INTR_ST7)
	{
        hdcpStatus = sTpiHdcpAuthStatusGet(hdcpObj);
        if((hdcpStatus & BIT_MSK__TPI_HW_DBG5__TPI_HW_CS) == SII_HDCP14_HDCP_AUTH3)
        {
            sResetKsvFifo(hdcpObj);
    		hdcpObj->ksvList.length  += SII_BKSV_LIST_BYTES;
    		sTpiBksvGet(hdcpObj, hdcpObj->ksvList.pList);
    		hdcpObj->ksvList.pList += SII_BKSV_LIST_BYTES;
    		SII_LIB_LOG_DEBUG1(hdcpObj, "Sink BKSVs::  \n");
    		sPrintKsvList(hdcpObj);
    		// Part 1 is done and no downstream Part 2 needs to be done
    		hdcpObj->authState = SII_MOD_HDCP_EVENT__AUTHENTICATED;
            SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__TPI_COPP_DATA2, BIT_MSK__TPI_COPP_DATA2__REG_INTR_ENCRYPTION);
    		sNotifyHdcpStatus(hdcpObj);
        }
        else
        {
            hdcpStatus = sTpiHdcpStatusGet(hdcpObj);
    		sPrintHdcpQueryStatus(hdcpObj, hdcpStatus);

    		switch (hdcpStatus & BIT_MSK__TPI_COPP_DATA1__REG_COPP_LINK_STATUS)
    		{
        		case BIT_ENUM__TPI_HDCP_QUERY__STATUS_NORMAL:
        			if (hdcpObj->authState == SII_MOD_HDCP_EVENT__WAIT_FOR_DONE)
        			{
        				if (hdcpStatus & BIT_MSK__TPI_COPP_DATA1__REG_COPP_HDCP_REP)
        				{
        					hdcpObj->authState = SII_MOD_HDCP_EVENT__WAIT_FIFO_READY;
        				}
        				else
        				{
        					if(!(hdcpStatus & BIT_MSK__TPI_COPP_DATA1__REG_COPP_LPROT))
        					{
        						hdcpObj->authState = SII_MOD_HDCP_EVENT__FAILED;
                                sNotifyHdcpStatus(hdcpObj);
        					}
        					else
        					{
        					    sResetKsvFifo(hdcpObj);
        						hdcpObj->ksvList.length  += SII_BKSV_LIST_BYTES;
        						sTpiBksvGet(hdcpObj, hdcpObj->ksvList.pList);
        						hdcpObj->ksvList.pList += SII_BKSV_LIST_BYTES;
        						SII_LIB_LOG_DEBUG1(hdcpObj, "Sink BKSVs::  \n");
        						sPrintKsvList(hdcpObj);
        						// Part 1 is done and no downstream Part 2 needs to be done
        						hdcpObj->authState = SII_MOD_HDCP_EVENT__AUTHENTICATED;
                                hdcpObj->u8QuerySatusFailTimes = 0;
                                SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__TPI_COPP_DATA2, BIT_MSK__TPI_COPP_DATA2__REG_INTR_ENCRYPTION);
        						sNotifyHdcpStatus(hdcpObj);
        					}
        				}
        			}
        			break;

        		case BIT_ENUM__TPI_HDCP_QUERY__STATUS_FAILED:
                    hdcpObj->authState = SII_MOD_HDCP_EVENT__FAILED;
                    hdcpObj->u8QuerySatusFailTimes++;
                    if(hdcpObj->u8QuerySatusFailTimes >= HDCP1X_QUERY_STATUS_FAIL_TIMES)
                    {
                        sNotifyHdcpStatus(hdcpObj);  
                    }
        			SII_LIB_LOG_WARN(hdcpObj, "HDCP query status failed, times:%d!\n", hdcpObj->u8QuerySatusFailTimes);
                    sTpiHdcpProtectionEnable(hdcpObj, false); // just in case
                    SiiLibTimeMilliDelay(10);
        			sTpiHdcpProtectionEnable(hdcpObj, true); // start authentication
        			hdcpObj->authState = SII_MOD_HDCP_EVENT__OFF;
                    break;
        		case BIT_ENUM__TPI_HDCP_QUERY__STATUS_LOST:
        			hdcpObj->authState = SII_MOD_HDCP_EVENT__FAILED;
        			sNotifyHdcpStatus(hdcpObj);
                    sTPIReset(hdcpObj, true);
                    SiiLibTimeMilliDelay(10);
                    sTPIReset(hdcpObj, false);
                    hdcpObj->authState = SII_MOD_HDCP_EVENT__WAIT_FOR_DONE;
                    break;
    		} 
        }

		sPrintHdcpStatus(hdcpObj);
	}

	if(sTpiHdcpStatusGet(hdcpObj) & BIT_MSK__TPI_COPP_DATA1__REG_COPP_HDCP_REP)
	{
		if (hdcpObj->authState == SII_MOD_HDCP_EVENT__WAIT_FIFO_READY)
		{
			uint8_t aDsBStatus[2];
			uint8_t dev_exceed = 0, casc_exceed = 0;
			sTpiBStatusGet(hdcpObj, aDsBStatus);
			SII_LIB_LOG_DEBUG1(hdcpObj,"aDsBStatus[0] = 0x%x, aDsBStatus[1] = 0x%x\n",aDsBStatus[0],aDsBStatus[1]);

			dev_exceed  = aDsBStatus[0] & BIT_MSK__TPI_BSTATUS1__REG_DS_DEV_EXCEED;
			casc_exceed = aDsBStatus[1] & BIT_MSK__TPI_BSTATUS2__REG_DS_CASC_EXCEED;
			if(dev_exceed || casc_exceed)
			{
				if(dev_exceed)
					SII_LIB_LOG_DEBUG1(hdcpObj, "Max DS Devices Exceeded...\n");
				if(casc_exceed)
					SII_LIB_LOG_DEBUG1(hdcpObj, "Max Cascaded Devices Exceeded...\n");
				hdcpObj->authState = SII_MOD_HDCP_EVENT__FAILED;
				sPrintHdcpStatus(hdcpObj);
				sNotifyHdcpStatus(hdcpObj);
			}
			else if((aDsBStatus[0] & BIT_MSK__TPI_BSTATUS1__REG_DS_DEV_CNT))
			{
				if (hdcpObj->hdcp1xIntStat.reg1 & BIT_MSK__TPI_INTR_ST0__TPI_INTR_ST3)
				{
					//Read KSV Fifo
					sResetKsvFifo(hdcpObj);
					hdcpObj->authState = sTpiGetKSVList(hdcpObj, aDsBStatus);
                    if (SII_MOD_HDCP_EVENT__WAIT_FIFO_READ_DONE == hdcpObj->authState)
                    {
                        hdcpObj->fifoReadDoneTime = SiI_get_global_time();
                    }
                    else 
                    {
                        hdcpObj->authState = SII_MOD_HDCP_EVENT__FAILED;
                        sNotifyHdcpStatus(hdcpObj); 
                    }
                    
					SII_LIB_LOG_DEBUG1(hdcpObj, "Repeater KSV List::  \n");
					sPrintKsvList(hdcpObj);   
					if(sTpiHdcpIsPart2Done(hdcpObj))
					{
						//Part2 Authentication is Done
						hdcpObj->authState = SII_MOD_HDCP_EVENT__AUTHENTICATED;
                        hdcpObj->u8QuerySatusFailTimes = 0;
                        sNotifyHdcpStatus(hdcpObj);
					}
					
					sPrintHdcpStatus(hdcpObj);
				}
			}
			else if(aDsBStatus[1])
			{
				hdcpObj->authState = SII_MOD_HDCP_EVENT__FAILED;
				sPrintHdcpStatus(hdcpObj);
				sNotifyHdcpStatus(hdcpObj);
			}
		}
	}
    
	// Clear all pending HDCP interrupts
	if (hdcpObj->hdcp1xIntStat.reg0)
	{
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__TPI_INTR_ST0, hdcpObj->hdcp1xIntStat.reg0);
		hdcpObj->hdcp1xIntStat.reg0 = 0x00;
	}

	if (hdcpObj->hdcp1xIntStat.reg1)
	{
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__TPI_INTR_ST0, hdcpObj->hdcp1xIntStat.reg1);
		hdcpObj->hdcp1xIntStat.reg1 = 0x00;
	}

}

//#if SII_INC_HDCP2XCORE
static void sHdcp2xIntrHandler(HdcpObj_t *hdcpObj)
{
	SiiDrvCraAddr_t baseAddr    = sbaseAddrGet(hdcpObj);
	SiiInst_t		craInst		= sCraInstGet(hdcpObj);
	IntStat_t       intStat		= {0};


	intStat.reg0 = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__HDCP2X_INTR0);
	intStat.reg1 = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__HDCP2X_INTR1);

	if(SII_MEMCMP(&hdcpObj->hdcp2xIntStat, &intStat, sizeof(IntStat_t)))
	{
		uint8_t intreg0 = 0;
		uint8_t intreg1 = 0;

		// Find any interrupt status bit that changed to '1'
		intreg0 |= ((hdcpObj->hdcp2xIntStat.reg0 ^ intStat.reg0) & intStat.reg0);
		intreg1 |= ((hdcpObj->hdcp2xIntStat.reg1 ^ intStat.reg1) & intStat.reg1); //Jagan

		hdcpObj->hdcp2xIntStat = intStat;
		if(intreg0 | intreg1)
		{
			// Call derived interrupt handler
			sHdcp2xVirtualIsrHandler(hdcpObj);

			//Clear the Interrupts
			SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_INTR0, hdcpObj->hdcp2xIntStat.reg0);
			SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_INTR1, hdcpObj->hdcp2xIntStat.reg1); //Jagan
			hdcpObj->hdcp2xIntStat.reg0 = 0;
			hdcpObj->hdcp2xIntStat.reg1 = 0; //Jagan
		}
	}
}

//-------------------------------------------------------------------------------------------------
//! @brief      Enable/Disable HDCP2 protection.
//!
//! @param[in]  isEnabled - true, if HDCP protection has to be enabled.
//-------------------------------------------------------------------------------------------------
void SiiDrvTpiHdcp2ProtectionEnable(HdcpObj_t* hdcpObj , bool_t isEnabled)
{
	SiiDrvCraAddr_t baseAddr    = sbaseAddrGet(hdcpObj);
	SiiInst_t		craInst		= sCraInstGet(hdcpObj);
	

	/*if( (isEnabled) && 
		(!(SiiDrvCraRdReg8(craInst,  baseAddr | REG_ADDR__HDCP2X_CTL_0 ) & BIT_MSK__HDCP2X_CTL_0__REG_HDCP2X_EN)) // Don't enable again.

		)
	{
		//SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__HDCP2X_GEN_CTRL0, BIT_MASK_HDCP2X_PROT_EN); //Jagan
		SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__HDCP2X_CTL_0, BIT_MSK__HDCP2X_CTL_0__REG_HDCP2X_EN);
		SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__HDCP2X_CTRL_0, BIT_MSK__HDCP2X_CTRL_0__RI_HDCP2_HDMIMODE);
	}
	else
	{
		// Revert to HDCP1 mode
		uint8_t protMask = HDCP_CTRL_MODE;
		SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__TPI_COPP_DATA2, protMask, isEnabled ? SET_BITS : CLEAR_BITS);

		//SiiDrvCraPutBit8(craInst, sbaseAddrGet(hdcpObj) | REG_ADDR__HDCP2X_GEN_CTRL0, BIT_MASK_HDCP2X_PROT_EN, CLEAR_BITS);
		//SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__HDCP2X_GEN_CTRL0, BIT_MASK_HDCP2X_PROT_EN);//Jagan
		SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__HDCP2X_CTL_0, BIT_MSK__HDCP2X_CTL_0__REG_HDCP2X_EN);
		SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__HDCP2X_CTRL_0, BIT_MSK__HDCP2X_CTRL_0__RI_HDCP2_HDMIMODE);
	}*/

	if(isEnabled)
	{
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_CTL_0	, BIT_MSK__HDCP2X_CTL_0__REG_HDCP2X_EN); // enabling HDCP2.2 mode
		SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__HDCP2X_CTRL_0, (BIT_MSK__HDCP2X_CTRL_0__RI_HDCP2_HDCPTX | BIT_MSK__HDCP2X_CTRL_0__RI_HDCP2_HDMIMODE));// enabling HDCP2.2 control reg for Tx
	}
	else
	{
		// Revert to HDCP1 mode
		uint8_t protMask = HDCP_CTRL_MODE;
		SiiDrvCraPutBit8(craInst, baseAddr | REG_ADDR__TPI_COPP_DATA2, protMask, isEnabled ? SET_BITS : CLEAR_BITS);

		SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__HDCP2X_CTL_0	, BIT_MSK__HDCP2X_CTL_0__REG_HDCP2X_EN); // disabling HDCP2.2 mode
		SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__HDCP2X_CTRL_0, (BIT_MSK__HDCP2X_CTRL_0__RI_HDCP2_HDCPTX | BIT_MSK__HDCP2X_CTRL_0__RI_HDCP2_HDMIMODE));// disabling HDCP2.2 control reg for Tx
	}
}

void Hdcp22AuthStart(HdcpObj_t* hdcpObj)
{
	SiiDrvCraAddr_t baseAddr    = sbaseAddrGet(hdcpObj);
	SiiInst_t		craInst		= sCraInstGet(hdcpObj);
	
	// If 0x50 response with HDCP2.2 capable
	if(hdcpObj->pConfig->bHdcp2xEn && DsHdcp22SupportGet(hdcpObj) )
	{
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_INTR0, 0x03); // //Clearing auth done , fail interrupts 
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_CTRL_1, 0xE1); //enabling re-authentication --- one pulse
		SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__HDCP2X_CTRL_1, BIT_MSK__HDCP2X_CTRL_1__RI_HDCP2_REAUTH_SW); // disabling re-authentication
	}
	hdcpObj->hdcp2x_repeater_ready = false;
	hdcpObj->hdcp2x_seq_num_m = 0;
}

void Hdcp22AuthStop(HdcpObj_t* hdcpObj)
{
	SiiDrvCraAddr_t baseAddr    = sbaseAddrGet(hdcpObj);
	SiiInst_t		craInst		= sCraInstGet(hdcpObj);
	uint8_t intStatus;

	hdcpObj->authFailCounter = 0;
	intStatus = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__HDCP2X_INTR0);
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_INTR0, intStatus); // //Clearing auth done , fail interrupts 

	intStatus = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__HDCP2X_INTR1);
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__HDCP2X_INTR1, intStatus); 

	SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__HDCP2X_INTR0_MASK, 0x00);
	SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__HDCP2X_INTR1_MASK, 0x00);
	//Diasabling HDCP2.2 Encryption
	SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__HDCP2X_CTL_0, (BIT_MSK__HDCP2X_CTL_0__REG_HDCP2X_ENCRYPT_EN | BIT_MSK__HDCP2X_CTL_0__REG_HDCP2X_EN)); 
	SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__HDCP2X_TX_CTRL_0,BIT_MSK__HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_SMNG_XFER_START);
	//hdcp2x_core_reset
	SiiDrvCraSetBit8(hdcpObj->pConfig->instTxCra, baseAddr | REG_ADDR__HDCP2X_TX_SRST, BIT_MSK__HDCP2X_TX_SRST__REG_HDCP2X_CRST);
	//SiiLibTimeMilliDelay(5);
	SiiDrvCraClrBit8(hdcpObj->pConfig->instTxCra, baseAddr | REG_ADDR__HDCP2X_TX_SRST, BIT_MSK__HDCP2X_TX_SRST__REG_HDCP2X_CRST);

    SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__HDCP2X_POLL_CS, BIT_MSK__HDCP2X_DS_POLL_EN);
	sHdcp2xCodeUpdate(hdcpObj);
}


/******* Update HDCP2.2 Patch********************/
static void sHdcp2xCodeUpdate(HdcpObj_t* hdcpObj)
{
	uint16_t cupdTimeout		= HDCP2X_CUPD_MAX_TIMEOUT;
	SiiDrvCraAddr_t baseAddr	= sbaseAddrGet(hdcpObj);
	SiiInst_t		craInst		= sCraInstGet(hdcpObj);
	hdcpObj->hdcp2xCupdStat		= SII_DRV_HDCP2X_CUPD_CHK__ERROR;

	//Disabling HW Code Update :: Pease verify later
	SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__HDCP2X_CTRL_2, BIT_MSK__HDCP2X_CTRL_2__RI_HDCP2_CUPD_HW);

	//If cupd_start is already set :: Remove this Later
	if(SiiDrvCraRdReg8(hdcpObj->pConfig->instTxCra, baseAddr | REG_ADDR__HDCP2X_CTRL_2) & BIT_MSK__HDCP2X_CTRL_2__RI_HDCP2_CUPD_START)
	{
		SiiDrvCraClrBit8(hdcpObj->pConfig->instTxCra, baseAddr | REG_ADDR__HDCP2X_CTRL_2, BIT_MSK__HDCP2X_CTRL_2__RI_HDCP2_CUPD_START);
		//hdcp2x_core_reset
		SiiDrvCraSetBit8(hdcpObj->pConfig->instTxCra, baseAddr | REG_ADDR__HDCP2X_TX_SRST, BIT_MSK__HDCP2X_TX_SRST__REG_HDCP2X_CRST);
		SiiLibTimeMilliDelay(5);
		SiiDrvCraClrBit8(hdcpObj->pConfig->instTxCra, baseAddr | REG_ADDR__HDCP2X_TX_SRST, BIT_MSK__HDCP2X_TX_SRST__REG_HDCP2X_CRST);
	}
	
	SiiDrvCraSetBit8(hdcpObj->pConfig->instTxCra, baseAddr | REG_ADDR__HDCP2X_CTRL_2, BIT_MSK__HDCP2X_CTRL_2__RI_HDCP2_CUPD_START);
	SiiDrvCraSetBit8(hdcpObj->pConfig->instTxCra, baseAddr | REG_ADDR__HDCP2X_CTRL_2, BIT_MSK__HDCP2X_CTRL_2__RI_HDCP2_CUPD_DONE);
	while(--cupdTimeout && (!(SiiDrvCraRdReg8(hdcpObj->pConfig->instTxCra, baseAddr | REG_ADDR__HDCP2X_INTR0) & BIT_MSK__HDCP2X_INTR0__INTR0_STAT4)))
	{
		SiiLibTimeMilliDelay(1);
	}
	if(!cupdTimeout)
	{
		hdcpObj->hdcp2xCupdStat = SII_DRV_HDCP2X_CUPD_CHK__ERROR;
	}
	else
	{
		if(HDCP2X_CCHK_FAIL == (SiiDrvCraRdReg8(hdcpObj->pConfig->instTxCra, baseAddr | REG_ADDR__HDCP2X_AUTH_STAT) & HDCP2X_CCHK_FAIL))
		{
			hdcpObj->hdcp2xCupdStat = SII_DRV_HDCP2X_CUPD_CHK__FAIL;
		}
		else
		{
			hdcpObj->hdcp2xCupdStat = SII_DRV_HDCP2X_CUPD_CHK__DONE;
		}

		//Clear CCHK_DONE interrupt
		SiiDrvCraWrReg8(hdcpObj->pConfig->instTxCra, baseAddr | REG_ADDR__HDCP2X_INTR0, BIT_MSK__HDCP2X_INTR0__INTR0_STAT4);
	}
	SiiDrvCraClrBit8(hdcpObj->pConfig->instTxCra, baseAddr | REG_ADDR__HDCP2X_CTRL_2, BIT_MSK__HDCP2X_CTRL_2__RI_HDCP2_CUPD_DONE);

	//Hisilicon need config this register
    SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__HDCP2X_CTRL_2, BIT_MSK__HDCP2X_CTRL_2__RI_HDCP2_CUPD_HW);
}

static void sHdcp2xCupdStatusGet(HdcpObj_t* hdcpObj)
{
	SiiDrvCraAddr_t baseAddr	= sbaseAddrGet(hdcpObj);
    hdcpObj->hdcp2xCupdStat		= SII_DRV_HDCP2X_CUPD_CHK__ERROR;
    if ((SiiDrvCraRdReg8(hdcpObj->pConfig->instTxCra, baseAddr | REG_ADDR__HDCP2X_INTR0) & BIT_MSK__HDCP2X_INTR0__INTR0_STAT4))
    {
    	if(HDCP2X_CCHK_FAIL == (SiiDrvCraRdReg8(hdcpObj->pConfig->instTxCra, baseAddr | REG_ADDR__HDCP2X_AUTH_STAT) & HDCP2X_CCHK_FAIL))
    	{
    		hdcpObj->hdcp2xCupdStat = SII_DRV_HDCP2X_CUPD_CHK__FAIL;
    	}
    	else
    	{
    		hdcpObj->hdcp2xCupdStat = SII_DRV_HDCP2X_CUPD_CHK__DONE;
            //Clear CCHK_DONE interrupt
		    SiiDrvCraWrReg8(hdcpObj->pConfig->instTxCra, baseAddr | REG_ADDR__HDCP2X_INTR0, BIT_MSK__HDCP2X_INTR0__INTR0_STAT4);
    	}
    }

}
#if 0
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
static ddcComErr_t sUpdateEdid(HdcpObj_t* hdcpObj, uint8_t segmentIndex, uint8_t regAddr, uint8_t *pBuf, uint16_t length)
{
	SiiDrvCraAddr_t baseAddr    = sbaseAddrGet(hdcpObj);
	SiiInst_t		craInst		= sCraInstGet(hdcpObj);
	ddcComErr_t		dsDdcError  = SI_TX_DDC_ERROR_CODE_NO_ERROR;
	uint16_t		fifoSize;
	uint8_t			timeOutMs;
    bool_t          bClear = false;
  
	do
	{
	
	 read_again:
        
		if( length == 0 )
			break;

		if( !pBuf )
			break;

		if (!sWaitForDdcBus(hdcpObj))
		{
			dsDdcError = SI_TX_DDC_ERROR_CODE_BUSY;
			return dsDdcError;
		}

		SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__TPI_DDC_MASTER_EN, BIT_MSK__TPI_DDC_MASTER_EN__REG_HW_DDC_MASTER);
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DDC_ADDR, (0xFF != segmentIndex) ? 0xA0 : 0x74 );
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DDC_SEGM, (0xFF != segmentIndex) ? segmentIndex : 0x00);
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DDC_OFFSET, regAddr);
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DDC_DIN_CNT2, 0);
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DDC_DIN_CNT1, (uint8_t)length);
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DDC_CMD, BIT_ENUM__DDC_CMD__CLEAR_FIFO); 
		//SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DDC_CMD, segmentIndex ? BIT_ENUM__DDC_CMD__ENHANCED_DDC_READ : BIT_ENUM__DDC_CMD__SEQUENTIAL_READ);            
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DDC_CMD, BIT_ENUM__DDC_CMD__SEQUENTIAL_READ);

		timeOutMs = length + 3; // timeout is proportional to length

		// wait until the FIFO is filled with several bytes
		SiiLibTimeMilliDelay(2); // also makes time aligning

		do
		{
			fifoSize = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__DDC_DOUT_CNT) & BIT_MSK__DDC_DOUT_CNT__DDC_DATA_OUT_CNT;

			if( fifoSize )
			{
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
				}
			}
			else
			{
				SiiLibTimeMilliDelay(1); // note, the time is aligned
				timeOutMs--;
			}
		} while (length && timeOutMs);

		if( dsDdcError )
			break;

		if( 0 == timeOutMs )
		{
		    if (sI2cIsDeadlock(hdcpObj))
            {
                if (sClearI2cDeadlock(hdcpObj) && !bClear)
                {
                    bClear = true;                    
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
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DDC_CMD, BIT_ENUM__DDC_CMD__ABORT_TRANSACTION);
	}

	// Disable DDC Master
	SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__TPI_DDC_MASTER_EN, BIT_MSK__TPI_DDC_MASTER_EN__REG_HW_DDC_MASTER);        
	SiiDrvCraClrBit8(craInst, baseAddr | REG_ADDR__DDC_CMD, 0x7F);

	return dsDdcError;
}
#endif
//-------------------------------------------------------------------------------------------------
//! @brief      Down stream hdcp2.2 support
//-------------------------------------------------------------------------------------------------

bool_t DsHdcp22SupportGet(HdcpObj_t* hdcpObj)
{
	//return (hdcpObj->dsHdcp_2_2_Supported == 0x04);
	return true;
}
//#endif
#if 0
static bool_t sWaitForDdcBus(HdcpObj_t* hdcpObj)
{
	SiiDrvCraAddr_t baseAddr    = sbaseAddrGet(hdcpObj);
	SiiInst_t		craInst		= sCraInstGet(hdcpObj);
	uint8_t         val			= 0;
	uint8_t         time_out    = LEN_TPI__DDC_FIFO_SIZE + 1;
	
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

//-------------------------------------------------------------------------------------------------
//! @brief      Set dynamic or static Link Integrity mode.
//!
//!             If downstream repeater is discovered, the dynamic mode shall be enabled.
//!             It forces the chip logic to re-authenticate any time the incoming clock
//!             resolution changes. Dynamic mode works for non-repeater sink. However
//!             the authentication may take a little longer time.
//!
//! @param[in]  isEnabled - true for dynamic, false for static mode.
//-------------------------------------------------------------------------------------------------

static void sTpiHdcpDynamicAuthenticationEnable(HdcpObj_t* hdcpObj, bool_t isEnabled)
{
	SiiDrvCraPutBit8(sCraInstGet(hdcpObj), sbaseAddrGet(hdcpObj) | REG_ADDR__TPI_SC, BIT_MSK__TPI_SC__REG_TPI_REAUTH_CTL, isEnabled ? SET_BITS : CLEAR_BITS);
}

//-------------------------------------------------------------------------------------------------
//! @brief      Enable/Disable HDCP encryption of HDMI data.
//!
//! @param[in]  isEnabled - true: to enable encryption if authentication succeeded,
//!                         false: to disable encryption (even after successful authentication).
//-------------------------------------------------------------------------------------------------
static void sTpiHdcpEncriptionEnable(HdcpObj_t* hdcpObj, bool_t isEnable)
{
	SiiDrvCraPutBit8(sCraInstGet(hdcpObj), sbaseAddrGet(hdcpObj) | REG_ADDR__TPI_COPP_DATA2, BIT_MSK__TPI_COPP_DATA2__REG_INTR_ENCRYPTION, isEnable ? SET_BITS : CLEAR_BITS);
}
#endif

static void sNotifyHdcpStatus(HdcpObj_t* hdcpObj)
{
	if(hdcpObj->cbFunc)
		hdcpObj->cbFunc(hdcpObj->pConfig->instTx, SII_DRV_TX_EVENT__HDCP_STATE_CHNG);
}
#endif
/***** end of file ***********************************************************/
