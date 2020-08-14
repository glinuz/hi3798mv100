//***************************************************************************
//!file     si_drv_cpi.c
//!brief    Silicon Image CPI Driver.
//
// No part of this work may be reproduced, modified, distributed,
// transmitted, transcribed, or translated into any language or computer
// format, in any form or by any means without written permission of
// Silicon Image, Inc., 1140 East Arques Avenue, Sunnyvale, California 94085
//
// Copyright 2002-2012, Silicon Image, Inc.  All rights reserved.
//***************************************************************************/
#ifdef HDMI_CEC_SUPPORT
//#include "string.h"
#include "si_drv_cpi_internal.h"
#include "si_drv_tx_regs.h"
#include "si_lib_obj_api.h"
#include "si_datatypes.h"
#include "si_drv_cra_api.h"
#include "si_lib_seq_api.h"
#include "si_lib_obj_api.h"
#include "si_lib_log_api.h"
#include "si_lib_time_api.h"
#include "sii_time.h"

#define TIMER_START_CEC_AUTO_PING_PERIOD        100
#define TIMER_START_CEC_AUTO_PING_WAIT          1000

/***** Register Module name **************************************************/
SII_LIB_OBJ_MODULE_DEF(drv_cpi);

//------------------------------------------------------------------------------
//  Driver Data
//------------------------------------------------------------------------------
typedef struct _CpiInstanceData_t
{
	SiiDrvCpiError_t    lastResultCode;     // Contains the result of the last API function called
	uint16_t            statusFlags;

	SiiCpiStatus_t      cecStatus;          // Valid only after interrupt

	CpiMsgQueue_t       msgQueueOut;        // CEC output message queue

	SiiInst_t           cpiInstTimer;
	CecCallBackPtr_t    cecCbFunc;
    SiiInst_t           cbFuncParam;
	uint16_t            baseAddr;
	SiiInst_t			instTxCra;

    SiiInst_t           cpiCecAutoPingTimer;// CEC auto ping timer
    CecCallBackPtr_t    cecAutoPingCbFunc;
}	CpiInstanceData_t;


//------------------------------------------------------------------------------
//  CPI Driver Instance Data
//------------------------------------------------------------------------------
static bool_t DrvIsOpcodeSendPing(uint16_t opcode)
{
	if(opcode == 0x100)
		return true;
	return false;
}

//------------------------------------------------------------------------------
// Function:    SiiDrvCpiCreate
//! @brief      Create the CPI object instance
//              Note that the CPI hardware is left disabled.  It must be enabled
//              by calling SiiDrvCpiResume()
// Parameters:  baseAddr   - Base address of the CPI hardware Registers
//              cecCbFunc  - CEC call back function pointer
// Returns:     It returns the object instarnce.
//------------------------------------------------------------------------------
SiiInst_t SiiDrvCpiCreate (SiiInst_t inst, uint16_t baseAddr, CecCallBackPtr_t cecCbFunc, CecCallBackPtr_t cecAPingCbFunc, void* data)
{
	CpiInstanceData_t *pCpiInstance;

	pCpiInstance = (CpiInstanceData_t *)SII_LIB_OBJ_CREATE("Cpi", sizeof(CpiInstanceData_t));
	SII_PLATFORM_DEBUG_ASSERT(pCpiInstance);
	// Initialize current CEC instance.
	SII_MEMSET( &pCpiInstance->cecStatus, 0, sizeof(SiiCpiStatus_t));
	SII_MEMSET(&pCpiInstance->msgQueueOut, 0, sizeof(CpiMsgQueue_t));
	pCpiInstance->baseAddr = baseAddr;
	pCpiInstance->cecCbFunc = cecCbFunc;
    pCpiInstance->cbFuncParam = data;
	pCpiInstance->instTxCra = inst;
    pCpiInstance->cecAutoPingCbFunc = cecAPingCbFunc;

	pCpiInstance->cpiInstTimer = SII_LIB_SEQ_TIMER_CREATE("CpiIntr", SiiDrvCecInterruptHandler, SII_LIB_OBJ_INST(pCpiInstance), 253, SII_TIMER_CPI);
	SiiLibSeqTimerStart(pCpiInstance->cpiInstTimer, 100, 100);
	pCpiInstance->lastResultCode = RESULT_CPI_SUCCESS;

    pCpiInstance->cpiCecAutoPingTimer = SII_LIB_SEQ_TIMER_CREATE("CpiAutoPing", SiiDrvCecAutoPingHandler, SII_LIB_OBJ_INST(pCpiInstance), TIMER_START_CEC_AUTO_PING_PERIOD, SII_TIMER_CEC_AUTO_PING);

	return (SiiInst_t)SII_LIB_OBJ_INST(pCpiInstance);
}

//------------------------------------------------------------------------------
// Function:    SiiDrvCpiDelete
//! @brief      Delete the CPI object and release the momory.
// Parameters:  inst - CPI instance
// Returns:     none
//
//------------------------------------------------------------------------------
void SiiDrvCpiDelete ( SiiInst_t inst )
{
    CpiInstanceData_t *pCpiInstance = (CpiInstanceData_t *)SII_LIB_OBJ_PNTR(inst);

    SiiLibSeqTimerDelete(pCpiInstance->cpiInstTimer);
    SiiLibSeqTimerDelete(pCpiInstance->cpiCecAutoPingTimer);
    SiiLibObjDelete(pCpiInstance);
}

//------------------------------------------------------------------------------
// Function:    SiiDrvCpiEnable
//! @brief      Enable the CPI hardware and interrupts for the current instance.
// Parameters:  inst - CPI instance
// Returns:     none
//
// API NOTE:    The CPI instance must be initialized prior to calling this
//              function.  It does not have to be enabled, just initialized.
//------------------------------------------------------------------------------
void SiiDrvCpiEnable ( SiiInst_t inst )
{
	CpiInstanceData_t *pCpiInstance = (CpiInstanceData_t *)SII_LIB_OBJ_PNTR(inst);

    // Clear passthru function
    SiiDrvCraClrBit8(pCpiInstance->instTxCra, pCpiInstance->baseAddr | REG_ADDR__CEC_CONFIG_CPI, BIT_MSK__CEC_CONFIG_CPI__CEC_REG_I2C_CEC_PASSTHRU);

	// Clear any pre-existing junk from the RX FIFO
	SiiDrvCraPutBit8(pCpiInstance->instTxCra,  pCpiInstance->baseAddr | REG_ADDR__CEC_RX_CONTROL, BIT_MSK__CEC_RX_CONTROL__CEC_REG_RX_CLR_ALL, SET_BITS );

	// Clear any existing interrupts
	SiiDrvCraPutBit8(pCpiInstance->instTxCra,  pCpiInstance->baseAddr | REG_ADDR__CEC_INT_STATUS_0, BIT_MSK__CEC_INT_STATUS_0__INTRP_RX_FIFO_NEMPTY | BIT_MSK__CEC_INT_STATUS_0__INTRP_TX_FIFO_EMPTY | BIT_MSK__CEC_INT_STATUS_0__INTRP_TX_FF_CSTATE, SET_BITS );
	SiiDrvCraPutBit8(pCpiInstance->instTxCra,  pCpiInstance->baseAddr | REG_ADDR__CEC_INT_STATUS_1, BIT_MSK__CEC_INT_STATUS_1__INTRP_FRME_RETX_CNT | BIT_MSK__CEC_INT_STATUS_1__INTRP_SHRT_PULSE_DT | BIT_MSK__CEC_INT_STATUS_1__INTRP_START_ODD_BIT | BIT_MSK__CEC_INT_STATUS_1__INTRP_RX_FIFO_OVRUN, SET_BITS );

	// Enable the interrupts within the CEC hardware block
	SiiDrvCraPutBit8(pCpiInstance->instTxCra,  pCpiInstance->baseAddr | REG_ADDR__CEC_INT_ENABLE_0, BIT_MSK__CEC_INT_STATUS_0__INTRP_RX_FIFO_NEMPTY | BIT_MSK__CEC_INT_STATUS_0__INTRP_TX_FIFO_EMPTY | BIT_MSK__CEC_INT_STATUS_0__INTRP_TX_FF_CSTATE, SET_BITS );
	SiiDrvCraPutBit8(pCpiInstance->instTxCra,  pCpiInstance->baseAddr | REG_ADDR__CEC_INT_ENABLE_1, BIT_MSK__CEC_INT_STATUS_1__INTRP_FRME_RETX_CNT | BIT_MSK__CEC_INT_STATUS_1__INTRP_SHRT_PULSE_DT | BIT_MSK__CEC_INT_STATUS_1__INTRP_START_ODD_BIT | BIT_MSK__CEC_INT_STATUS_1__INTRP_RX_FIFO_OVRUN, SET_BITS );

	pCpiInstance->statusFlags   &= ~SiiCPI_CEC_DISABLED;
}

//------------------------------------------------------------------------------
// Function:    SiiDrvCpiDisable
//! @brief      Disable the CPI hardware and interrupts for the current instance.
// Parameters:  inst - CPI instance
// Returns:     none
//------------------------------------------------------------------------------
void SiiDrvCpiDisable ( SiiInst_t inst )
{
	CpiInstanceData_t *pCpiInstance = (CpiInstanceData_t *)SII_LIB_OBJ_PNTR(inst);
    //Soft Reset CEC
    SiiDrvCraPutBit8(pCpiInstance->instTxCra,  pCpiInstance->baseAddr | REG_ADDR__SW_RST3, BIT_MSK__SW_RST3__REG_CEC_RST, SET_BITS );     // Disable RX CEC

	// Disable the interrupts within the CEC hardware block
	SiiDrvCraWrReg8(pCpiInstance->instTxCra,  pCpiInstance->baseAddr | REG_ADDR__CEC_INT_ENABLE_0, CLEAR_BITS);
	SiiDrvCraWrReg8(pCpiInstance->instTxCra,  pCpiInstance->baseAddr | REG_ADDR__CEC_INT_ENABLE_1, CLEAR_BITS);


	pCpiInstance->statusFlags   |= SiiCPI_CEC_DISABLED;
}

//------------------------------------------------------------------------------
// Function:    SiiDrvCpiStatus
// Description: Returns a status flag word containing CPI-specific
//				information about the state of the device.
// Parameters:  inst - CPI instance
// Returns:     Sixteen-bit status flags word for the CPI Component
//------------------------------------------------------------------------------
uint16_t SiiDrvCpiStatus ( SiiInst_t inst )
{
	CpiInstanceData_t *pCpiInstance = (CpiInstanceData_t *)SII_LIB_OBJ_PNTR(inst);
	uint16_t statusFlags =pCpiInstance->statusFlags;

	pCpiInstance->statusFlags &= ~SiiCPI_CEC_INT;   // INT flag only valid first time it is read.
	return( statusFlags );
}

//------------------------------------------------------------------------------
// Function:    SiiDrvCpiStandby
// Description: Places the CPI component into standby mode if available
// Parameters:  inst - CPI instance
// Returns:     true if the CPI component entered standby mode successfully,
//!             or false if some failure occurred.
//------------------------------------------------------------------------------
bool_t SiiDrvCpiStandby ( SiiInst_t inst )
{
	CpiInstanceData_t *pCpiInstance = (CpiInstanceData_t *)SII_LIB_OBJ_PNTR(inst);
	int i;

	// Setup the auto feature abort logic for this device by
	// starting out with all opcodes set to feature abort mode.
	for ( i = 0; i < 32; i++ )
	{
		SiiDrvCraWrReg8(pCpiInstance->instTxCra, (pCpiInstance->baseAddr |  REG_ADDR__CEC_OP_ABORT_0) + i, 0xFF );
	}

	SiiDrvCraWrReg8(pCpiInstance->instTxCra, pCpiInstance->baseAddr |  REG_ADDR__CEC_OP_ABORT_0,    ~0x10 );    // CECOP_IMAGE_VIEW_ON
	SiiDrvCraWrReg8(pCpiInstance->instTxCra, pCpiInstance->baseAddr |  REG_ADDR__CEC_OP_ABORT_1,    ~0x20 );    // CECOP_TEXT_VIEW_ON

	// Everybody must respond to GIVE_DEVICE_POWER_STATUS
	SiiDrvCraWrReg8(pCpiInstance->instTxCra, pCpiInstance->baseAddr |  REG_ADDR__CEC_OP_ABORT_17, (uint8_t)~0x80 );              // CECOP_GIVE_DEVICE_POWER_STATUS

	return( true );
}

//------------------------------------------------------------------------------
// Function:    SiiDrvCpiResume
// Description: Causes operation of the CPI component to return to the state
//				it was prior to the previous call to SiiCpiStandby.
// Parameters:  inst - CPI instance
// Returns:     true if the CPI component successfully exited standby mode,
//				or false if some failure occurred.
//------------------------------------------------------------------------------
bool_t SiiDrvCpiResume ( SiiInst_t inst )
{
	CpiInstanceData_t *pCpiInstance = (CpiInstanceData_t *)SII_LIB_OBJ_PNTR(inst);
	int i;

	// Setup the auto feature abort logic for this device by
	// starting out with all opcodes set to NOT feature abort mode.
	for ( i = 0; i < 32; i++ )
	{
		SiiDrvCraWrReg8(pCpiInstance->instTxCra, (pCpiInstance->baseAddr |  REG_ADDR__CEC_OP_ABORT_0) + i, 0x00 );
	}

	// Now set to feature abort the CECOP_ABORT opcode
	SiiDrvCraWrReg8(pCpiInstance->instTxCra, pCpiInstance->baseAddr |  REG_ADDR__CEC_OP_ABORT_31,   0x80 );    // CECOP_ABORT

	return( true );
}


//------------------------------------------------------------------------------
// Function:    SiiDrvCpiGetLastResult
// Description: Returns the result of the last SiiCpixxx function called
// Parameters:  inst - CPI instance
// Returns:     Result of last CPI component function.
//------------------------------------------------------------------------------
uint_t SiiDrvCpiGetLastResult ( SiiInst_t inst )
{
	CpiInstanceData_t *pCpiInstance = (CpiInstanceData_t *)SII_LIB_OBJ_PNTR(inst);

	return(pCpiInstance->lastResultCode );
}

//------------------------------------------------------------------------------
//! @brief  Configure the CPI subsystem to respond to and transmit as a
//!         specific CEC logical address.
//! @param[in]  inst -  CPI instance
//!             logicalAddress - CEC logical address or 0xFF if unregistered
//! @return     always true
//------------------------------------------------------------------------------
bool_t SiiDrvCpiSetLogicalAddr ( SiiInst_t inst, uint8_t logicalAddress )
{
	CpiInstanceData_t *pCpiInstance = (CpiInstanceData_t *)SII_LIB_OBJ_PNTR(inst);
	uint8_t capture_address[2];
	uint8_t capture_addr_sel = 0x01;

	capture_address[0] = 0;
	capture_address[1] = 0;
	//    SiiDrvCraBlockRead8(pCpiInstance->instTxCra,  REG_ADDR__CEC_CAPTURE_ID0, capture_address, 2 );
	if ( logicalAddress == 0xFF )
	{
		logicalAddress = 0x0F;  // unregistered LA
	}
	else if ( logicalAddress < 8 )
	{
		capture_addr_sel = capture_addr_sel << logicalAddress;
		capture_address[0] |= capture_addr_sel;
	}
	else
	{
		capture_addr_sel   = capture_addr_sel << ( logicalAddress - 8 );
		capture_address[1] |= capture_addr_sel;
	}

	// Set Capture Address
	SiiDrvCraBlockWrite8(pCpiInstance->instTxCra, pCpiInstance->baseAddr |  REG_ADDR__CEC_CAPTURE_ID0, capture_address, 2 );

	// Set device logical address for transmit
	SiiDrvCraWrReg8(pCpiInstance->instTxCra, pCpiInstance->baseAddr |  REG_ADDR__CEC_TX_INIT, logicalAddress );

	return( true );
}

//------------------------------------------------------------------------------
// Function:    SiiDrvCpiServiceWriteQueue
// Description: If there is a CEC message waiting in the write queue and the
//              hardware TX buffer is empty, send the message.
// Parameters:  inst - CPI instance
// Returns:     None
//------------------------------------------------------------------------------
void SiiDrvCpiServiceWriteQueue ( SiiInst_t inst )
{
	CpiInstanceData_t *pCpiInstance = (CpiInstanceData_t *)SII_LIB_OBJ_PNTR(inst);
	SiiCpiData_t *pOutMsg;
	uint8_t cecStatus[2];
	SiiLibSeqTimeMs_t mSec;
    uint8_t i = 0;

	do
	{
		// No message in the queue?  Get out.
		if (pCpiInstance->msgQueueOut.queue[pCpiInstance->msgQueueOut.outIndex ].txState == SiiTX_IDLE )
		{
			break;
		}

		// If last TX command is still being sent (waiting for ACK/NACK), check timeout
		if (pCpiInstance->msgQueueOut.queue[pCpiInstance->msgQueueOut.outIndex ].txState == SiiTX_SENDING)
		{
			// If a timeout has occurred, mark the current message as failed.
			// This will be picked up by the SiiDrvCpiHwStatusGet function and passed to the CEC controller
			mSec = (SiiLibSeqTimeMs_t)SiI_get_global_time();

			if (( mSec -pCpiInstance->msgQueueOut.msStart ) >=pCpiInstance->msgQueueOut.msTimeout )
			{
				pCpiInstance->msgQueueOut.queue[pCpiInstance->msgQueueOut.outIndex ].txState = SiiTX_TIMEOUT;

				if (!DrvIsOpcodeSendPing(pCpiInstance->msgQueueOut.queue[pCpiInstance->msgQueueOut.outIndex].opcode) )
				{
					SII_LIB_LOG_DEBUG1("%s:CEC Message [W%02X][%02X] send timeout!\n",
						"RX",
						pCpiInstance->msgQueueOut.queue[pCpiInstance->msgQueueOut.outIndex].srcDestAddr,
						pCpiInstance->msgQueueOut.queue[pCpiInstance->msgQueueOut.outIndex].opcode
						);
				}
				pCpiInstance->statusFlags |= SiiCPI_CEC_STATUS_VALID;
				break;
			}
		}

		// If NOT waiting to be sent, get out.
		if (pCpiInstance->msgQueueOut.queue[pCpiInstance->msgQueueOut.outIndex ].txState != SiiTX_WAITCMD)
		{
			break;
		}

		// Current queue entry is waiting to be sent, so send it
		pCpiInstance->msgQueueOut.queue[pCpiInstance->msgQueueOut.outIndex ].txState = SiiTX_SENDING;
		pOutMsg = &pCpiInstance->msgQueueOut.queue[pCpiInstance->msgQueueOut.outIndex ];

		// Clear Tx-related interrupts; write 1 to bits to be cleared.
		cecStatus[0] = BIT_MSK__CEC_INT_STATUS_0__INTRP_TX_FIFO_FULL | BIT_MSK__CEC_INT_STATUS_0__INTRP_TX_FF_CSTATE | BIT_MSK__CEC_INT_STATUS_0__INTRP_TX_FIFO_EMPTY;
		cecStatus[1] = BIT_MSK__CEC_INT_STATUS_1__INTRP_FRME_RETX_CNT;
		SiiDrvCraBlockWrite8(pCpiInstance->instTxCra,  pCpiInstance->baseAddr | REG_ADDR__CEC_INT_STATUS_0, cecStatus, 2 );

		// Special handling for a special opcode.

		if ( DrvIsOpcodeSendPing(pOutMsg->opcode) )
		{
			SiiDrvCraWrReg8(pCpiInstance->instTxCra,  pCpiInstance->baseAddr | REG_ADDR__CEC_TX_DEST, BIT_MSK__CEC_TX_DEST__CEC_REG_SD_POLL_INTERN | pOutMsg->srcDestAddr);
		}
		else
		{
			// Set the initiator to the LA specified by the srcDestAddr
			SiiDrvCraWrReg8(pCpiInstance->instTxCra,  pCpiInstance->baseAddr | REG_ADDR__CEC_TX_INIT, (( pOutMsg->srcDestAddr >> 4) & 0x0F));
			// Send the command
			SiiDrvCraWrReg8(pCpiInstance->instTxCra,  pCpiInstance->baseAddr | REG_ADDR__CEC_TX_DEST, pOutMsg->srcDestAddr & 0x0F );
			SiiDrvCraWrReg8(pCpiInstance->instTxCra,  pCpiInstance->baseAddr | REG_ADDR__CEC_TX_COMMAND, (uint8_t)pOutMsg->opcode );
			SiiDrvCraBlockWrite8(pCpiInstance->instTxCra,  pCpiInstance->baseAddr | REG_ADDR__CEC_TX_OPERAND_0, pOutMsg->args, pOutMsg->argCount );
			SiiDrvCraWrReg8(pCpiInstance->instTxCra,  pCpiInstance->baseAddr | REG_ADDR__CEC_TRANSMIT_DATA, BIT_MSK__CEC_TRANSMIT_DATA__MANUAL_CMD_SET | pOutMsg->argCount );
            SII_LIB_LOG_DEBUG1(pCpiInstance, "send CEC command, srcDestAddr:0x%x, opcode:0x%x, argCount:%d\n", pOutMsg->srcDestAddr, pOutMsg->opcode, pOutMsg->argCount);
            for(i = 0; i < pOutMsg->argCount; i++)
            {
                SII_LIB_LOG_DEBUG1(pCpiInstance, "send data %d: 0x%x \n", i, pOutMsg->args[i]);
            }
		}

		mSec = (SiiLibSeqTimeMs_t)SiI_get_global_time();
		pCpiInstance->msgQueueOut.msStart = mSec;
	} while (0);
}

//------------------------------------------------------------------------------
// Function:    SiiDrvCpiWrite
// Description: Send CEC command via CPI register set
// Parameters:  inst  - CPI instance
//              *pMsg - Pointer to message data buffer
// Returns:     Message ID
//
// API NOTE:    The SiiDrvCpiServiceWriteQueue() function must be called
//              periodically to send any messages queued up by this function.
//              The SiiDrvCpiHwStatusGet() function must be called to release
//              the queue from waiting for the previously sent message to complete
//------------------------------------------------------------------------------
int32_t SiiDrvCpiWrite( SiiInst_t inst, SiiCpiData_t *pMsg )
{
	CpiInstanceData_t *pCpiInstance = (CpiInstanceData_t *)SII_LIB_OBJ_PNTR(inst);
    int32_t     s32Ret = 0;
	uint16_t    msgId = 0;
	bool_t      success = true;
	int         i;

	// Store the message in the output queue
	if (pCpiInstance->msgQueueOut.queue[pCpiInstance->msgQueueOut.inIndex ].txState == SiiTX_IDLE )
	{
		memcpy( &pCpiInstance->msgQueueOut.queue[pCpiInstance->msgQueueOut.inIndex ], pMsg, sizeof( SiiCpiData_t ) );
		pCpiInstance->msgQueueOut.msTimeout = 2000;     // timeout after 2 seconds
		pCpiInstance->msgQueueOut.queue[pCpiInstance->msgQueueOut.inIndex ].txState    = SiiTX_WAITCMD;
		msgId = (uint16_t)(pCpiInstance->msgQueueOut.inIndex << 8) | (pMsg->opcode - 1);
		pCpiInstance->msgQueueOut.queue[pCpiInstance->msgQueueOut.inIndex ].msgId      = msgId;
		pCpiInstance->msgQueueOut.inIndex = (pCpiInstance->msgQueueOut.inIndex + 1) % SII_CPI_OUTPUT_QUEUE_LEN;
        s32Ret = 0;
    }
	else
	{
		SII_LIB_LOG_DEBUG1( pCpiInstance, "SiiCpiWrite:: CEC Write Queue full!\n" );
		for ( i = 0; i < SII_CPI_OUTPUT_QUEUE_LEN; i++ )
		{
			SII_LIB_LOG_DEBUG2("SRCDST: %02X - Opcode: %02X\n",pCpiInstance->msgQueueOut.queue[i].srcDestAddr,pCpiInstance->msgQueueOut.queue[i].opcode);
		}
		success = false;
        s32Ret = -1;
	}

	SiiDrvCpiServiceWriteQueue((SiiInst_t)SII_LIB_OBJ_INST(pCpiInstance));  // Send the message if possible

	pCpiInstance->lastResultCode = (success) ? RESULT_CPI_SUCCESS : RESULT_CPI_WRITE_QUEUE_FULL;
	return( s32Ret );
}


//------------------------------------------------------------------------------
// Function:    SiiDrvCpiSendPing
// Description: Initiate sending a ping, and used for checking available
//              CEC devices
// Parameters:  inst - CPI instance
//              destLA -  destination logical address
// Returns:     Message ID
//------------------------------------------------------------------------------
uint16_t SiiDrvCpiSendPing ( SiiInst_t inst, uint8_t destLA )
{
	SiiCpiData_t cecFrame;

	// Send the ping via the normal CPI message queue.

	cecFrame.opcode         = 0x100;
	cecFrame.srcDestAddr    = destLA;
	cecFrame.argCount       = 0;

	return( SiiDrvCpiWrite( inst, &cecFrame ));
}

void SiiDrvCpiAutoPingDisable(SiiInst_t inst)
{
    CpiInstanceData_t* pCpiInstance = (CpiInstanceData_t*)SII_LIB_OBJ_PNTR(inst);
    SiiDrvCraPutBit8(pCpiInstance->instTxCra, pCpiInstance->baseAddr | REG_ADDR__CEC_AUTO_DISCOVERY, BIT_MSK__CEC_AUTO_DISCOVERY__CEC_AUTO_PING_CLEAR, BIT_MSK__CEC_AUTO_DISCOVERY__CEC_AUTO_PING_CLEAR);
    return;
}

uint8_t SiiDrvCpiPingStatusGet(SiiInst_t inst)
{
    CpiInstanceData_t* pCpiInstance = (CpiInstanceData_t*)SII_LIB_OBJ_PNTR(inst);

    return SiiDrvCraRdReg8(pCpiInstance->instTxCra, pCpiInstance->baseAddr | REG_ADDR__CEC_AUTO_DISCOVERY);;
}


void SiiDrvCpiAutoPingEnable(SiiInst_t inst)
{
    CpiInstanceData_t* pCpiInstance = (CpiInstanceData_t*)SII_LIB_OBJ_PNTR(inst);
    SiiDrvCraPutBit8(pCpiInstance->instTxCra, pCpiInstance->baseAddr | REG_ADDR__CEC_AUTO_DISCOVERY, BIT_MSK__CEC_AUTO_DISCOVERY__CEC_AUTO_PING_START, BIT_MSK__CEC_AUTO_DISCOVERY__CEC_AUTO_PING_START);
    return;
}


void SiiDrvCpiAutoPingStart(SiiInst_t inst)
{
    SiiDrvCpiAutoPingDisable(inst);
    SiiDrvCpiAutoPingEnable(inst);
    SiiDrvCpiAPingTimerStart(inst);

    return;
}

void SiiDrvCpiAPingTimerStart(SiiInst_t inst)
{
    CpiInstanceData_t* pCpiInstance = (CpiInstanceData_t*)SII_LIB_OBJ_PNTR(inst);

    SiiLibSeqTimerStart(pCpiInstance->cpiCecAutoPingTimer, TIMER_START_CEC_AUTO_PING_PERIOD, TIMER_START_CEC_AUTO_PING_PERIOD);

    return;
}

void SiiDrvCpiAPingTimerStop(SiiInst_t inst)
{
    CpiInstanceData_t *pCpiInstance = (CpiInstanceData_t *)SII_LIB_OBJ_PNTR(inst);

    SiiLibSeqTimerStop(pCpiInstance->cpiCecAutoPingTimer);

    return;
}

bool_t SiiDrvCpiAPingDisc(SiiInst_t inst, uint16_t* autoPingMap)
{
    uint8_t autoPing = 0;
    CpiInstanceData_t *pCpiInstance = (CpiInstanceData_t *)SII_LIB_OBJ_PNTR(inst);
    uint8_t *status = (uint8_t*)autoPingMap;

    autoPing = SiiDrvCraRdReg8(pCpiInstance->instTxCra, pCpiInstance->baseAddr | REG_ADDR__CEC_AUTO_DISCOVERY );

    if (!(autoPing & BIT_MSK__CEC_AUTO_DISCOVERY__CEC_AUTO_PING_DONE))
    {
        return false;
    }

    status[0] = SiiDrvCraRdReg8(pCpiInstance->instTxCra,  pCpiInstance->baseAddr | REG_ADDR__CEC_AUTODISC_MAP0 );
    status[1] = SiiDrvCraRdReg8(pCpiInstance->instTxCra,  pCpiInstance->baseAddr | REG_ADDR__CEC_AUTODISC_MAP1 );
    SiiDrvCraWrReg8(pCpiInstance->instTxCra,  pCpiInstance->baseAddr | REG_ADDR__CEC_AUTO_DISCOVERY, 0x0 );

    return true;
}

//------------------------------------------------------------------------------
// Function:    SiiDrvCpiRead
// Description: Reads a CEC message from the CPI read FIFO, if present.
// Parameters:  inst - CPI instance
//              *pMsg -  Pointer to output message data buffer
// Returns:     true if valid message, false if an error occurred
//------------------------------------------------------------------------------
bool_t SiiDrvCpiRead( SiiInst_t inst, SiiCpiData_t *pMsg )
{
	CpiInstanceData_t *pCpiInstance = (CpiInstanceData_t *)SII_LIB_OBJ_PNTR(inst);
	bool_t    success = true;
	uint8_t argCount;

	argCount = SiiDrvCraRdReg8(pCpiInstance->instTxCra,  pCpiInstance->baseAddr | REG_ADDR__CEC_RX_COUNT );

	if ( argCount & BIT_MSK__CEC_RX_COUNT__CEC_RX_ERROR )
	{
		success = false;
	}
	else
	{
		pMsg->argCount = argCount & 0x0F;
		pMsg->srcDestAddr = SiiDrvCraRdReg8(pCpiInstance->instTxCra,  pCpiInstance->baseAddr | REG_ADDR__CEC_RX_CMD_HEADER );
		pMsg->opcode = SiiDrvCraRdReg8(pCpiInstance->instTxCra,  pCpiInstance->baseAddr | REG_ADDR__CEC_RX_COMMAND );
		if ( pMsg->argCount )
		{
			SiiDrvCraBlockRead8(pCpiInstance->instTxCra,  pCpiInstance->baseAddr | REG_ADDR__CEC_RX_OPERAND_0, pMsg->args, pMsg->argCount );
		}
	}

	// Clear CLR_RX_FIFO_CUR;
	// Clear current frame from Rx FIFO

	SiiDrvCraPutBit8(pCpiInstance->instTxCra,  pCpiInstance->baseAddr | REG_ADDR__CEC_RX_CONTROL, BIT_MSK__CEC_RX_CONTROL__CEC_REG_RX_CLR_CUR_SET, BIT_MSK__CEC_RX_CONTROL__CEC_REG_RX_CLR_CUR_SET );


	return( success );
}

//------------------------------------------------------------------------------
// Function:    SiiCpiFrameCount
// Description: Return the number of CEC frames currently available
// Parameters:  inst - CPI instance
// Returns:     Message ID
//------------------------------------------------------------------------------
uint_t SiiDrvCpiFrameCount( SiiInst_t inst )
{
	CpiInstanceData_t *pCpiInstance = (CpiInstanceData_t *)SII_LIB_OBJ_PNTR(inst);

	return(( SiiDrvCraRdReg8(pCpiInstance->instTxCra,  pCpiInstance->baseAddr | REG_ADDR__CEC_RX_COUNT) & 0xF0) >> 4);
}

//------------------------------------------------------------------------------
// Function:    SiiCpiGetLogicalAddr
// Description: Get Logical Address
// Parameters:  inst - CPI instance
// Returns:     Returns Logical Address
//------------------------------------------------------------------------------
uint8_t SiiDrvCpiGetLogicalAddr( SiiInst_t inst )
{
	CpiInstanceData_t *pCpiInstance = (CpiInstanceData_t *)SII_LIB_OBJ_PNTR(inst);
	return( SiiDrvCraRdReg8(pCpiInstance->instTxCra,  pCpiInstance->baseAddr | REG_ADDR__CEC_TX_INIT));
}

//------------------------------------------------------------------------------
// Function:    SiiDrvCpiHwStatusGet
// Description: returns CPI CEC status register info if it has been updated
//              since the last call to this function.
// Parameters:  inst - CPI Instance
//              pCpiStat - pointer to CPI hardware status for return value.
// Returns:     true if a new status was available, false if not.
//------------------------------------------------------------------------------
bool_t  SiiDrvCpiHwStatusGet( SiiInst_t inst, SiiCpiStatus_t *pCpiStat )
{
	CpiInstanceData_t *pCpiInstance = (CpiInstanceData_t *)SII_LIB_OBJ_PNTR(inst);
	memset( pCpiStat, 0, sizeof( SiiCpiStatus_t ));     // Always clear status for return
	if (pCpiInstance->statusFlags & SiiCPI_CEC_STATUS_VALID )
	{
		memcpy( pCpiStat, &pCpiInstance->cecStatus, sizeof( SiiCpiStatus_t ));
		if (pCpiInstance->msgQueueOut.queue[pCpiInstance->msgQueueOut.outIndex ].txState == SiiTX_TIMEOUT)
		{
			// A CPI write timed out; mark it as a NACK.  Since the CPI writes
			// are serialized, there is no other TX status currently valid.
			pCpiStat->txState = SiiTX_SENDFAILED;
		}

		// Add the message ID to the returned status

		pCpiStat->msgId =pCpiInstance->msgQueueOut.queue[pCpiInstance->msgQueueOut.outIndex ].msgId;

		// If the status indicates an ACKed or NACKed transmission,
		// update the sent message queue so that any pending message
		// may be sent.  Note that this is the only way that a message
		// waiting in the CPI output message queue can be sent.

		if (( pCpiStat->txState == SiiTX_SENDACKED ) || ( pCpiStat->txState == SiiTX_SENDFAILED))
		{
			// Mark this queue entry as available and bump to next entry in queue

			pCpiInstance->msgQueueOut.queue[pCpiInstance->msgQueueOut.outIndex ].txState = SiiTX_IDLE;
			pCpiInstance->msgQueueOut.outIndex = (pCpiInstance->msgQueueOut.outIndex + 1) % SII_CPI_OUTPUT_QUEUE_LEN;
		}

		pCpiInstance->statusFlags &= ~SiiCPI_CEC_STATUS_VALID;
		return( true );
	}
	return( false );
}

void SiiDrvCecAutoPingHandler( SiiInst_t inst )
{
    CpiInstanceData_t* pCpiInstance = (CpiInstanceData_t*)SII_LIB_OBJ_PNTR(inst);

    SiiLibSeqTimerStop(pCpiInstance->cpiCecAutoPingTimer);

    pCpiInstance->cecAutoPingCbFunc(SII_LIB_OBJ_INST(pCpiInstance), pCpiInstance->cbFuncParam);
}

//------------------------------------------------------------------------------
// Function:    SiiDrvCecInterruptHandler
// Description: Check CPI registers for a CEC event
// Parameters:  inst - CPI instance
// Returns:     None
//------------------------------------------------------------------------------
void SiiDrvCecInterruptHandler( SiiInst_t inst )
{
	uint8_t cecStatus[2];
	CpiInstanceData_t* pCpiInstance = (CpiInstanceData_t*)SII_LIB_OBJ_PNTR(inst);

	SiiDrvCraBlockRead8(pCpiInstance->instTxCra,  pCpiInstance->baseAddr | REG_ADDR__CEC_INT_STATUS_0, cecStatus, 2);

	if ( (cecStatus[0] & 0x7F) || cecStatus[1] )
	{
		pCpiInstance->cecStatus.cecError    = 0;
		pCpiInstance->cecStatus.rxState     = 0;
		pCpiInstance->cecStatus.txState     = 0;

		// Clear interrupts

		if ( cecStatus[1] & BIT_MSK__CEC_INT_STATUS_1__INTRP_FRME_RETX_CNT )
		{
			/* Flush TX, otherwise after clearing the BIT_MSK__CEC_INT_STATUS_1__INTRP_FRME_RETX_CNT */
			/* interrupt, the TX command will be re-sent.                   */

			SiiDrvCraPutBit8(pCpiInstance->instTxCra,  pCpiInstance->baseAddr | REG_ADDR__CEC_DEBUG_3,BIT_MSK__CEC_DEBUG_3__CEC_REG_FLUSH_TX_FF, BIT_MSK__CEC_DEBUG_3__CEC_REG_FLUSH_TX_FF );
		}

		// Clear interrupt bits that are set
		SiiDrvCraBlockWrite8(pCpiInstance->instTxCra,  pCpiInstance->baseAddr | REG_ADDR__CEC_INT_STATUS_0, cecStatus, 2 );

		// RX Processing
		if ( cecStatus[0] & BIT_MSK__CEC_INT_STATUS_0__INTRP_RX_FIFO_NEMPTY )
		{
			pCpiInstance->cecStatus.rxState = 1;    // Flag caller that CEC frames are present in RX FIFO
		}

		// RX Errors processing
		if ( cecStatus[1] & BIT_MSK__CEC_INT_STATUS_1__INTRP_SHRT_PULSE_DT )
		{
			pCpiInstance->cecStatus.cecError |= SiiCEC_SHORTPULSE;
		}

		if ( cecStatus[1] & BIT_MSK__CEC_INT_STATUS_1__INTRP_START_ODD_BIT )
		{
			pCpiInstance->cecStatus.cecError |= SiiCEC_BADSTART;
		}

		if ( cecStatus[1] & BIT_MSK__CEC_INT_STATUS_1__INTRP_RX_FIFO_OVRUN )
		{
			pCpiInstance->cecStatus.cecError |= SiiCEC_RXOVERFLOW;
		}

		// TX Processing
		if ( cecStatus[0] & BIT_MSK__CEC_INT_STATUS_0__INTRP_TX_FF_CSTATE )
		{
			pCpiInstance->cecStatus.txState = SiiTX_SENDACKED;
		}
		if ( cecStatus[1] & BIT_MSK__CEC_INT_STATUS_1__INTRP_FRME_RETX_CNT )
		{
			pCpiInstance->cecStatus.txState = SiiTX_SENDFAILED;
		}

		// Indicate that an interrupt occurred and status is valid.

		pCpiInstance->statusFlags |= (SiiCPI_CEC_INT | SiiCPI_CEC_STATUS_VALID);
	}

	pCpiInstance->cecCbFunc(SII_LIB_OBJ_INST(pCpiInstance), pCpiInstance->cbFuncParam);
}

#endif
