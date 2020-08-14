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
* @file si_drv_mhl_tx_internal.h
 *
 * @brief MHl Tx Internal API
 *
 *****************************************************************************/
#ifndef __SI_MOD_TX_MHL_INTERNAL_H__
#define __SI_MOD_TX_MHL_INTERNAL_H__

#include "si_drv_tx_api.h"
#include "si_mod_tx_cbus_defs.h"

/***** public type definitions ***********************************************/
typedef uint32_t SiiModMhlTxEvent_t;
typedef void (*MhlTxEventNotifyCbFunc)(SiiInst_t, SiiModMhlTxEvent_t);

#define BIT_RGND_READY_INT \
		BIT_MSK__CBUS_DISC_INTR0__REG_CBUS_DISC_INTR0_STAT6
#define BIT_CBUS_MHL12_DISCON_INT \
		BIT_MSK__CBUS_DISC_INTR0__REG_CBUS_DISC_INTR0_STAT5
#define BIT_NOT_MHL_EST_INT \
		BIT_MSK__CBUS_DISC_INTR0__REG_CBUS_DISC_INTR0_STAT3
#define BIT_MHL12_EST_INT \
		BIT_MSK__CBUS_DISC_INTR0__REG_CBUS_DISC_INTR0_STAT2

		
#define BIT_RGND_READY_INT_MASK \
		BIT_MSK__CBUS_DISC_INTR0_MASK__REG_CBUS_DISC_INTR0_MASK6
#define BIT_CBUS_MHL12_DISCON_INT_MASK \
		BIT_MSK__CBUS_DISC_INTR0_MASK__REG_CBUS_DISC_INTR0_MASK5
#define BIT_NOT_MHL_EST_INT_MASK \
		BIT_MSK__CBUS_DISC_INTR0_MASK__REG_CBUS_DISC_INTR0_MASK3
#define BIT_MHL_EST_INT_MASK \
		BIT_MSK__CBUS_DISC_INTR0_MASK__REG_CBUS_DISC_INTR0_MASK2

#define CBUS_DISC_INTR0_MASK (0 \
	| BIT_MSK__CBUS_DISC_INTR0_MASK__REG_CBUS_DISC_INTR0_MASK2 \
	| BIT_MSK__CBUS_DISC_INTR0_MASK__REG_CBUS_DISC_INTR0_MASK3 \
	| BIT_MSK__CBUS_DISC_INTR0_MASK__REG_CBUS_DISC_INTR0_MASK5 \
	| BIT_MSK__CBUS_DISC_INTR0_MASK__REG_CBUS_DISC_INTR0_MASK6 \
	)

#define BIT_CBUS_CONNECTION_CHG_INT		\
		BIT_MSK__CBUS_INT_0__REG_CBUS_INT_0_STAT0
#define BIT_MSC_XFER_DONE_INT			\
		BIT_MSK__CBUS_INT_0__REG_CBUS_INT_0_STAT1
#define BIT_CBUS_HPD_CHG_INT			\
		BIT_MSK__CBUS_INT_0__REG_CBUS_INT_0_STAT2
#define BIT_MSC_WRITE_STATE_RCV_INT		\
		BIT_MSK__CBUS_INT_0__REG_CBUS_INT_0_STAT3
#define BIT_MSC_MSG_RCV_INT				\
		BIT_MSK__CBUS_INT_0__REG_CBUS_INT_0_STAT4
#define BIT_MSC_WRITE_BURST_RCV_INT		\
		BIT_MSK__CBUS_INT_0__REG_CBUS_INT_0_STAT5
#define BIT_MSC_SET_INT_RCV_INT			\
		BIT_MSK__CBUS_INT_0__REG_CBUS_INT_0_STAT6
#define BIT_MSC_MSG_DONE_WITH_NACK_INT	\
		BIT_MSK__CBUS_INT_0__REG_CBUS_INT_0_STAT7

#define BIT_MSC_HEARTBEAT_MAX_FAIL_INT \
		BIT_MSK__CBUS_INT_1__REG_CBUS_INT_1_STAT0
#define BIT_DDC_ABORT_INT				\
		BIT_MSK__CBUS_INT_1__REG_CBUS_INT_1_STAT2
#define BIT_MSC_RCV_CMD_ERROR_INT		\
		BIT_MSK__CBUS_INT_1__REG_CBUS_INT_1_STAT3
#define BIT_CBUS_PACKET_RCV_INT			\
		BIT_MSK__CBUS_INT_1__REG_CBUS_INT_1_STAT5
#define BIT_MSC_XFER_CMD_ERROR_INT			\
		BIT_MSK__CBUS_INT_1__REG_CBUS_INT_1_STAT6
#define BIT_MHL_CONNECTION_CHG_INT		\
		BIT_MSK__CBUS_INT_1__REG_CBUS_INT_1_STAT7

#define BIT_DCAP_CHG_INT		\
		BIT_MSK__MHL_INT_0__REG_MHL_INT_0_STAT0
#define BIT_DSCR_CHG_INT		\
		BIT_MSK__MHL_INT_0__REG_MHL_INT_0_STAT1
#define BIT_REQ_WRT_INT			\
		BIT_MSK__MHL_INT_0__REG_MHL_INT_0_STAT2
#define BIT_GRT_WRT_INT			\
		BIT_MSK__MHL_INT_0__REG_MHL_INT_0_STAT3
#define BIT_3D_REQ_INT			\
		BIT_MSK__MHL_INT_0__REG_MHL_INT_0_STAT4
#define BIT_FEAT_REQ_INT		\
		BIT_MSK__MHL_INT_0__REG_MHL_INT_0_STAT5
#define BIT_FEAT_COMPLETE_INT	\
		BIT_MSK__MHL_INT_0__REG_MHL_INT_0_STAT6

#define BIT_EDID_CHG_INT	\
		BIT_MSK__MHL_INT_1__REG_MHL_INT_1_STAT1

#define BIT_CBUSABORT_MAXFAIL				\
		BIT_MSK__MSC_MR_ABORT_INT__REG_MSC_MR_ABORT_INT_STAT0
#define BIT_CBUSABORT_PROTOCOL_ERROR		\
		BIT_MSK__MSC_MR_ABORT_INT__REG_MSC_MR_ABORT_INT_STAT1
#define BIT_CBUSABORT_TIMEDOUT				\
		BIT_MSK__MSC_MR_ABORT_INT__REG_MSC_MR_ABORT_INT_STAT2
#define BIT_CBUSABORT_UNDEFINED_OPCODE	\
		BIT_MSK__MSC_MR_ABORT_INT__REG_MSC_MR_ABORT_INT_STAT3
#define BIT_CBUSABORT_BAD_OFFSET		\
		BIT_MSK__MSC_MR_ABORT_INT__REG_MSC_MR_ABORT_INT_STAT4
#define BIT_CBUSABORT_OVERFLOW			\
		BIT_MSK__MSC_MR_ABORT_INT__REG_MSC_MR_ABORT_INT_STAT5
#define BIT_CBUSABORT_PEER_ABORTED		\
		BIT_MSK__MSC_MR_ABORT_INT__REG_MSC_MR_ABORT_INT_STAT7

#define	MAX_VIDEO_ENTRY		5	//Max entry for 3D writeburst 4 for 3D_SVD, 1 for 3D_DTD

#define RGND_STATUS_MASK	0x03
#define RGND_STATUS_OPEN	0x0
#define RGND_STATUS_2K		0x1
#define RGND_STATUS_1K		0x2
#define RGND_STATUS_SHORT	0x3

/*
 * Cbus_State related definitions
 */
#define DEM_RSEN_VALID_TIME				1100	// RSEN Valid time 1s
#define DEM_MHL_WAIT_TIME				10		// MHL wait time after connection 10ms
#define DEM_MHL_RCP_TIMEOUT				1100	// MHL Rcp/Rap wait time
#define DEM_MHL_BURST_WAIT				1100	// MHL grt wrt wait time
#define DEM_MSC_MAX_REPLY_DELAY			500		// MSC Max reply delay for transfer done, 500ms
#define DEM_CEC_FEATURE_ABORT_MAX_DELAY 800		// CEC Feature abort delay, 1000ms according to CEC spec Chapter7, but rcpk need within 1000ms, so set it 800ms
#define DEM_CBUS_ABORT_WAIT				2200	// CBUS Abort wait time, 2000ms
#define DEM_MDT_WAIT					2000	// wait for MDT timeout, 2000ms
#define DEV_PKT_SENDER_TIMEOUT			150

//When scratch pad is busy, GRT_WRT will not be sent.
//When BIT_DSCR_CHG occurs or timeout, scratch pad becomes free.
#define DEM_CBUS_SCRATCH_PAD_BUSY_TIMEOUT 2000

typedef enum
{
	//Interrupt Register
	API_MSC_INTERRUPT_DCAP_CHG = 0x01,
	API_MSC_INTERRUPT_DSCR_CHG,
	API_MSC_INTERRUPT_REQ_WRT,
	API_MSC_INTERRUPT_GRT_WRT,
	API_MSC_INTERRUPT_3D_REQ,
	API_MSC_INTERRUPT_FEAT_REQ,
	API_MSC_INTERRUPT_FEAT_COMPLETE,

	API_MSC_INTERRUPT_EDID_CHG,

	//Status Register
	API_MSC_STATUS_DCAP_RDY,
	API_MSC_STATUS_LINK_MODE,
	API_MSC_STATUS_VERSION_STAT,

	//Scraptchpad Register
	API_MSC_SCRATCHPAD_WRITE_BURST,

	//MSC_MSG
	API_MSC_MSGE,

	API_MSC_MSG_RCP,
	API_MSC_MSG_RCPK,
	API_MSC_MSG_RCPE,

	API_MSC_MSG_RAP,
	API_MSC_MSG_RAPK,

	API_MSC_MSG_UCP,
	API_MSC_MSG_UCPK,
	API_MSC_MSG_UCPE,

	API_MSC_SET_HOTPLUG,
} CBUS_API_MSC_COMMAND_t;

typedef struct
{
    CBUS_API_MSC_COMMAND_t eSentCommand;	//last command to sent
    uint8_t                ucData[16];		//Command paratemeter for eSentCommand
    bool_t                 bSent;			//the command is sent successfully or not.
    uint8_t *              sFailureReason;	//fail reason if bSent is false.
} CBUS_CALLBACK_PARAMETERS_t;
typedef bool_t (*PCOMMANDREPORT_t)(void *pObj, CBUS_CALLBACK_PARAMETERS_t *pCbusCallBackParameters);

#define MAX_WRITE_BURST		6
typedef struct
{
    uint8_t ucGeneralWriteBurst[MAX_WRITE_BURST][MHL_SCRATCHPAD_SIZE];
    uint8_t ucReadPointer;
    uint8_t ucWritePointer;
    bool_t  bBusy;
} WRITEBURST_CIRCULARBUFFER_t;

typedef enum
{
    CBUS_REQ_IDLE = 0,
    CBUS_REQ_PENDING,           // Request is waiting to be processed
    CBUS_REQ_WAITING,           // Request is waiting response
} CBUS_REQ_t;

//
// structure to hold command details from upper layer to CBUS module
//
typedef struct
{
    uint8_t                ucReqStatus;							// REQ_IDLE, REQ_PENDING, REQ_WAITING
    uint8_t                ucCommand;							// MSC command used in low lever.
    CBUS_API_MSC_COMMAND_t CbusMscCommand;						//Store the command in function  SiiCbusSendCommand.
    uint8_t                ucOffsetData;						// Offset of register on CBUS
    uint8_t                ucLength;							// Only applicable to write burst. ignored otherwise.
    uint8_t                ucMsgData[MHL_MAX_BUFFER_SIZE];		// Pointer to message data area.
    uint8_t                ucMsgData2[MHL_MAX_BUFFER_SIZE];		// Pointer to next message data area.
    
	bool_t (*dataRetHandler)(void*, uint8_t);				// Data return handler for MSC commands
    char *sDataRetHandlerName;								// Name of the function pointed by dataRetHandler
    
	uint32_t ulTimeout;											// timing control
    uint32_t ulBase_time;										// record the base time
    bool_t   bTimer_set;										// flag for enable/disable timer
    uint8_t  ucRetry;											// How many times this command has been retried.
    bool_t   bCecReq;											// define whether the Req is from Cec
    bool_t   bVip;												// define whether the Req is a high priority request

    PCOMMANDREPORT_t pCommandReport;
} cbus_out_queue_t;

typedef struct
{
    uint8_t  ucReqStatus;					// REQ_IDLE, REQ_PENDING, REQ_WAITING
    uint8_t  ucCommand;						// RAP or  RCP opcode
    uint8_t  ucMsgData;						// RAP or RCP data
    uint32_t ulTimeout;						// timing control
    uint32_t ulBase_time;					// record the base time
    bool_t   bTimer_set;					// flag for enable/disable timer
} cbus_in_queue_t;

typedef enum
{
    KEY_NOT_IN_IGNORELIST = 0x00,
    KEY_IN_IGNORELIST = 0x01,
    KEY_STATUS_NOT_INITLIAED = 0x20,
}CBUS_KeyStatus_t;

typedef enum
{
    CBUS_IN = 0,  // Receiving
    CBUS_OUT,           // Sending
    CBUS_INOUT,         // Both
} CBUS_Direction_t;

typedef enum
{
    CBUS_NO_PENDING_EVENTS	= 0x00,
    CBUS_READ_DCAP			= 0x01,
    CBUS_3D_REQ				= 0x02,
    CBUS_GRT_WRT			= 0x04,
	CBUS_START_VIDEO		= 0x08,
	CBUS_STOP_VIDEO			= 0x10,
} SiiCbusEvents_t;

//
// CBUS module reports these error types
//
typedef enum
{
    STATUS_true = 0,
    ERROR_CBUS_CAN_RETRY,
    ERROR_CBUS_ABORT,
    ERROR_CBUS_TIMEOUT,
    ERROR_CBUS_LINK_DOWN,
    ERROR_INVALID,
    ERROR_INIT,
    ERROR_WRITE_FAILED,
    ERROR_NO_HEARTBEAT,
    ERROR_WRONG_DIRECTION,
    ERROR_QUEUE_FULL,
    ERROR_MSC_MSG,
    ERROR_INVALID_MSC_CMD,
    ERROR_RET_HANDLER,
} CBUS_SOFTWARE_ERRORS_t;

typedef struct
{
	cbus_out_queue_t	OutCommand;						// sending command  (RAP/RCP/MSC)
    cbus_in_queue_t		InCommand;						// receiving command  (RAP/RCP)

    bool_t		bRCP_SUPPORT;							// peer RCP_SUPPORT
    bool_t		bRAP_SUPPORT;							// peer RAP_SUPPORT
    bool_t		bSP_SUPPORT;							// peer Scratch_Pad_SUPPORT
    bool_t		bUCP_SEND_SUPPORT;
    bool_t		bUCP_RECV_SUPPORT;

	bool_t		bXDevcap_Supp;								// Peer's XDEVCAP Support
	bool_t		bDcapRdy;								// For Peer Dcap Reg ready
    uint8_t		ucPeerDCAP[MHL_MAX_BUFFER_SIZE];		//Store Peer Dcap Reg.
    bool_t		bReadDCAP;								//If Peer Device Capability Register has been read out.
    bool_t		bPathEnRecv;							// For path enable recieved
    uint8_t		ucClkMode;								//Packed Pixel Mode
    bool_t		bAbortWait;								//If need to wait 2 seconds due to receiving abort message.
    uint32_t	ulBase_time_abort;						//The starting point of timing.

	MhlVideoFormatData_t	videoFormatData[MAX_VIDEO_ENTRY];
    uint8_t					ucVideoEntryIndex;
    uint8_t					ucGeneralWriteBurst[MHL_SCRATCHPAD_SIZE];
    bool_t					bBurstToSend; //true: need to send burst. false: no burst to send

    WRITEBURST_CIRCULARBUFFER_t WriteBurstOutQueue;
    WRITEBURST_CIRCULARBUFFER_t WriteBurstInQueue;

    uint16_t uwPendingEvents;

	CBUS_CALLBACK_PARAMETERS_t CbusCallBackParameters;

    uint8_t ucPeerXDCAP[MHL_MAX_BUFFER_SIZE];	//Store Peer XDCAP Reg, only available when opposite device is MHL3 device.
    bool_t  bReadXDCAP;							//If peer Extended Device Capability Registers have been read out.
    bool_t  bEnableECBUS;                       //If eCBUS is established.
    bool_t  bEcbusSynced;
    bool_t  bTxDevCapRdySent;
    bool_t  bTxDevCapChgSent;
} CbusState_t;
/* cbus_state END */

typedef enum
{
	POWER_STATE_D3, // Lowest Power State
	POWER_STATE_D2, 
	POWER_STATE_D0,
	POWER_STATE_NONE, // Highest Power State
} SiiModMhlTxPowerState_t;

typedef enum
{
	RGND_UNKNOWN = 0,
	RGND_OPEN,
	RGND_1K,
	RGND_2K,
	RGND_SHORT,
} SiiModMhlTxRgndState_t;

typedef enum
{
	STATE_MHL_TMDS_OFF,
	STATE_MHL_TMDS_ON,
	STATE_MHL_TMDS_UPDATE,
	STATE_MHL_HDCP_ON,
	STATE_MHL_HDCP_OFF,
	STATE_MHL_HDCP_STATE,
} SiiModMhlTxInternalState_t;

typedef enum
{
	STATE_MHL_DISCONNECTED = 0,
	STATE_MHL_READY_RGND_DETECT,
	STATE_MHL_USB_CONNECTED,
	STATE_MHL_CONNECTED,
	STATE_NON_MHL_DETECTED,	
	STATE_MHL_DISCOVERY_ON,
	STATE_MHL_DISCOVERY_SUCCESS,
	STATE_MHL_DISCOVERY_FAIL,
	STATE_MHL_ESTABLISHED,
	STATE_WAIT_FOR_DISCONNECTION,
} SiiModMhlTxDiscState_t;

typedef enum
{
	SII_MOD_MHL_STATUS__CLR_HPD_RCVD,
	SII_MOD_MHL_STATUS__SET_HPD_RCVD,
	SII_MOD_MHL_STATUS__HDCP_OFF,
	SII_MOD_MHL_STATUS__HDCP_ON,
} SiiModMhlTxState_t;

typedef enum
{
	CM_NO_CONNECTION,
	CM_NO_CONNECTION_BIST_SETUP,
	CM_NO_CONNECTION_BIST_STAT,
	CM_oCBUS_PEER_VERSION_PENDING,
	CM_oCBUS_PEER_VERSION_PENDING_BIST_SETUP,
	CM_oCBUS_PEER_VERSION_PENDING_BIST_STAT,
	CM_oCBUS_PEER_IS_MHL1_2,
	CM_oCBUS_PEER_IS_MHL3_BIST_SETUP,
	CM_oCBUS_PEER_IS_MHL3_BIST_SETUP_SENT,
	CM_oCBUS_PEER_IS_MHL3_BIST_SETUP_PEER_READY,
	CM_oCBUS_PEER_IS_MHL3_BIST_STAT,
	CM_oCBUS_PEER_IS_MHL3,
	CM_TRANSITIONAL_TO_eCBUS_S_BIST,
	CM_TRANSITIONAL_TO_eCBUS_D_BIST,
	CM_TRANSITIONAL_TO_eCBUS_S,
	CM_TRANSITIONAL_TO_eCBUS_D,
	CM_TRANSITIONAL_TO_eCBUS_S_CAL_BIST,
	CM_TRANSITIONAL_TO_eCBUS_D_CAL_BIST,
	CM_TRANSITIONAL_TO_eCBUS_S_CALIBRATED,
	CM_TRANSITIONAL_TO_eCBUS_D_CALIBRATED,
	CM_eCBUS_S_BIST,
	CM_eCBUS_D_BIST,
	CM_eCBUS_S,
	CM_eCBUS_D,
	CM_eCBUS_S_AV_BIST,
	eCBUS_D_AV_BIST,

	NUM_CM_MODES,
} SiiModCbusModes_t;

//! Opcodes for Internal module variables.
typedef enum
{
	SII_MOD_MHL_TX_OPCODE__MHL_STATE,
	SII_MOD_MHL_TX_OPCODE__CBUS_STATE,
	SII_MOD_MHL_TX_OPCODE__RCP_KEYCODE,
	SII_MOD_MHL_TX_OPCODE__ENABLE_TDM,
	SII_MOD_MHL_TX_OPCODE__ENABLE_MHL3_AVLINK,
	SII_MOD_MHL_TX_OPCODE__READ_DEVCAPS,
	SII_MOD_MHL_TX_OPCODE__READ_XDEVCAPS,
	SII_MOD_MHL_TX_OPCODE__SET_DEVCAPS,
	SII_MOD_MHL_TX_OPCODE__SET_XDEVCAPS,
	SII_MOD_MHL_TX_OPCODE__HDCP_STATUS,
	SII_MOD_MHL_TX_OPCODE__MHL_TYPE,
	SII_MOD_MHL_TX_OPCODE__ENABLE_EMSC_FLOODTEST,
	SII_MOD_MHL_TX_OPCODE__ENABLE_MDT_FLOODTEST,
} SiiModMhlTxInternalOpcode_t;

typedef struct
{
	SiiDrvCraAddr_t		baseAddrTx;
    SiiInst_t			instTxCra;
	SiiInst_t			instTx;

	bool_t				bMhl3En;
	bool_t				bEmscEn;
	bool_t				bMDTEn;
	bool_t				bProgramDino;

	void				(*cbFunc)(SiiInst_t, SiiModMhlTxEvent_t);
} SiiModMhlTxConfig_t;


 /***** MHL Tx Internal functions ******************************************************/

/*****************************************************************************/
/**
* @brief MHL Tx driver constructor
*
* @param[in]  pNameStr   Name of instance
* @param[in]  pConfig    Static configuration parameters
*
* @retval                Handle to instance
*
*****************************************************************************/
SiiInst_t SiiModMhlTxCreate(char *pNameStr, SiiModMhlTxConfig_t *pConfig);

/*****************************************************************************/
/**
* @brief MHL Tx driver destructor
*
* @param[in]  inst       Handle to instance
*
*****************************************************************************/
void SiiModMhlTxDelete(SiiInst_t inst);
	
/*****************************************************************************/
/**
* @brief MHL Tx Interrupt Handler
*
* @param[in]  inst       Handle to instance
*
*****************************************************************************/
void SiiModCbusInterruptHandler(SiiInst_t inst);
	
/*****************************************************************************/
/**
* @brief MHL Tx Property Set API
*
* @param[in]  inst       Handle to instance
* @param[in]  opcode     Property's opcode
* @param[in]  inData	 input Data
*
*****************************************************************************/
bool_t SiiModMhlTxSet(SiiInst_t inst, SiiModMhlTxInternalOpcode_t opcode, const void *inData);
	
/*****************************************************************************/
/**
* @brief MHL Tx Property Get API
*
* @param[in]  inst       Handle to instance
* @param[in]  opcode     Property's opcode
* @param[in]  outData	 out Data
*
*****************************************************************************/
bool_t SiiModMhlTxGet(SiiInst_t inst, SiiModMhlTxInternalOpcode_t opcode, void *outData);

#endif //__SI_MOD_TX_MHL_INTERNAL_H__