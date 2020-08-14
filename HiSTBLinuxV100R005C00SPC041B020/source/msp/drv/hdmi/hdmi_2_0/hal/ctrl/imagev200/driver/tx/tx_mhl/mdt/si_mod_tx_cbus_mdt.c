/*********************************************************************************/
/*  Copyright (c) 2002-2011, Silicon Image, Inc.  All rights reserved.           */
/*  No part of this work may be reproduced, modified, distributed, transmitted,  */
/*  transcribed, or translated into any language or computer format, in any form */
/*  or by any means without written permission of: Silicon Image, Inc.,          */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                         */
/*********************************************************************************/

#include "si_drv_tx_regs.h"
#include "si_lib_malloc_api.h"
#include "si_lib_log_api.h"
#include "si_lib_time_api.h"
#include "si_mod_tx_cbus_mdt_internal.h"
#include <conio.h>

/***** Register Module name **************************************************/
SII_LIB_OBJ_MODULE_DEF(drv_tx_cbus_mdt);

//#ifdef MDT_TEST

/***** local type definitions ************************************************/
typedef struct
{
	SiiModMdtConfig_t	*pConfig;
	uint8_t		outBuffer[18];
	uint8_t		inBuffer[18];
	bool_t		mdt_xmit_enable;
	bool_t		mdt_rxc_enable;
	bool_t		mdt_flood_test_enable;
	uint8_t		currentByteOut;
	uint8_t		currentByteIn;
} MdtObj_t;

/***** local prototypes ******************************************************/
static void sMdtErrorHalt(MdtObj_t *mdtObj);
static void sMdtDataCompare(MdtObj_t *mdtObj);
static void sMdtTest(MdtObj_t *mdtObj);
static void sMdtTestInit(MdtObj_t *mdtObj);
static void sMdtWrBlock(MdtObj_t *mdtObj, uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4);
static void sMdtRdBlock(MdtObj_t *mdtObj);
static void sMdtStatus(MdtObj_t *mdtObj);
static void sMdtHelp(MdtObj_t *mdtObj);
static HI_BOOL sMdtFloodTestEnable(MdtObj_t *mdtObj);
static void sMdtXmitEnable(MdtObj_t *mdtObj);
static void sMdtRdXmitFifoBlock(MdtObj_t *mdtObj);
static void sMdtRxcEnable(MdtObj_t *mdtObj);
static void sMdtFloodTestDisable(MdtObj_t *mdtObj);
static SiiInst_t		sCraInstGet(MdtObj_t *mdtObj);
static SiiDrvCraAddr_t	sBaseAddrGet(MdtObj_t *mdtObj);

/***** public functions ******************************************************/
SiiInst_t SiiModCbusMdtInitialize(char *pNameStr, SiiModMdtConfig_t *pConfig)
{
	MdtObj_t *mdtObj = NULL;
	SiiDrvCraAddr_t	baseAddr;
	SiiInst_t		craInst;

	/* Allocate memory for mhl_tx object */
	mdtObj = (MdtObj_t*)SII_LIB_OBJ_CREATE(pNameStr, sizeof(MdtObj_t));
	SII_PLATFORM_DEBUG_ASSERT(mdtObj);

	mdtObj->pConfig = (SiiModMdtConfig_t*)SiiLibMallocCreate(sizeof(SiiModMdtConfig_t));
	memcpy(mdtObj->pConfig, pConfig, sizeof(SiiModMdtConfig_t));

	baseAddr = sBaseAddrGet(mdtObj);
	craInst  = sCraInstGet(mdtObj);

	return SII_LIB_OBJ_INST(mdtObj);
}

void SiiModCbusMdtDelete(SiiInst_t mdtInst)
{
	MdtObj_t *mdtObj = (MdtObj_t*)SII_LIB_OBJ_PNTR(mdtInst);

	SiiLibMallocDelete(mdtObj->pConfig);
	SII_LIB_OBJ_DELETE(mdtObj);
}

void SiiModStartMDTFloodTest(SiiInst_t mdtInst)
{
	MdtObj_t *mdtObj = (MdtObj_t*)SII_LIB_OBJ_PNTR(mdtInst);
	char key[3];
	uint8_t input = 0;
	do {

		sMdtHelp(mdtObj);

		while(!_kbhit());
		gets_s(key, sizeof(key));
		input = atoi(key);

		if((key == "x") || (key == "X"))
			return;

		if(input < 1 && input > 7)
		{
			SII_LIB_LOG_PRINT2(("Invalid Input... Returning\n"));
			return;
		}

		switch(input)
		{
		case 0:
			sMdtDataCompare(mdtObj);
			break;
		case 1:
			//SII_LIB_LOG_PRINT2(("Read the MDT data from the FIFO\n"));			
			sMdtRdBlock(mdtObj);
			break;
		case 2:
			//SII_LIB_LOG_PRINT2(("Print out the MDT Status\n"));
			sMdtStatus(mdtObj);
			break;
		case 3:
			//SII_LIB_LOG_PRINT2(("Enable the MDT\n"));
			sMdtTestInit(mdtObj);
			break;
		case 4:
			//SII_LIB_LOG_PRINT2(("Run the MDT  flood test\n"));
			sMdtFloodTestEnable(mdtObj);
			break;
		case 5:
			//SII_LIB_LOG_PRINT2(("MDT send burst write data in normal mode\n"));
			sMdtWrBlock(mdtObj, 0x00, 0x10, 0x20, 0x30);
			break;
		case 6:
			//SII_LIB_LOG_PRINT2(("MDT send burst write data in fixed adopter ID mode\n"));
			sMdtWrBlock(mdtObj, 0x40, 0x50, 0x60, 0x70);
			break;
		case 7:
			//SII_LIB_LOG_PRINT2(("MDT send in fixed byte length\n"));		
			sMdtWrBlock(mdtObj, 0x80, 0x90, 0xA0, 0xb0);
			break;
		case 8:
			//SII_LIB_LOG_PRINT2(("MDT send in random byte length\n"));
			sMdtWrBlock(mdtObj, 0xC0, 0xD0, 0xE0, 0xF0);
			break;
		case 9:
			//SII_LIB_LOG_PRINT2(("Disable the MDT Flood Test\n"));
			sMdtFloodTestDisable(mdtObj);
			break;
		default:
			SII_LIB_LOG_PRINT2(("Invalid Input...\n"));
		}
	} while(true);
}

/********************local Functions****************************************/
static void sMdtHelp(MdtObj_t *mdtObj)
{
	SII_LIB_LOG_PRINT2(("\nMDT Menu:\n"));
	SII_LIB_LOG_PRINT2(("\t 0		- Compare MDT data\n"));
	SII_LIB_LOG_PRINT2(("\t 1		- Read the MDT data from the FIFO\n"));
	SII_LIB_LOG_PRINT2(("\t 2		- Print out the mdt status\n"));
	SII_LIB_LOG_PRINT2(("\t 3		- Enable the MDT\n"));
	SII_LIB_LOG_PRINT2(("\t 4		- Run the MDT flood test\n"));
	SII_LIB_LOG_PRINT2(("\t 5		- MDT send burst write data in normal mode\n"));
	SII_LIB_LOG_PRINT2(("\t 6		- MDT send burst write data in fixed adopter ID mode\n"));
	SII_LIB_LOG_PRINT2(("\t 7		- MDT send in fixed byte length\n"));
	SII_LIB_LOG_PRINT2(("\t 8		- MDT send in random byte length\n"));
	SII_LIB_LOG_PRINT2(("\t 9		- Disable the MDT Flood Test\n"));
	SII_LIB_LOG_PRINT2(("\t x		- Exit MDT Flood Test\n"));
}

static void sMdtTestInit(MdtObj_t *mdtObj)
{
	SiiDrvCraAddr_t	baseAddr = sBaseAddrGet(mdtObj);
	SiiInst_t		craInst = sCraInstGet(mdtObj);

	SII_LIB_LOG_PRINT2(("MDT Init\n"));

	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__MDT_RCV_TIMEOUT, 0x0F);			// 0x88[7] = 1	to enable the MDT Transmitter
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__MDT_XMIT_TIMEOUT, 0x0F);			// 0x88[7] = 1	to enable the MDT Transmitter


	//Enable RSM and XSM
//	SiiLibTimeMilliDelay(25);
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__MDT_XMIT_CONTROL, 0x82);			// 0x88[7] = 1	to enable the MDT Transmitter

//	SiiLibTimeMilliDelay(25);
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__MDT_RCV_CONTROL, 0x82);			// 0x86[7] = 1	to enable the MDT Receiver

	mdtObj->currentByteOut = 0;
	mdtObj->currentByteIn  = 0;

	sMdtStatus(mdtObj);
}

static void	sMdtErrorHalt(MdtObj_t *mdtObj)
{
	SiiDrvCraAddr_t	baseAddr = sBaseAddrGet(mdtObj);
	SiiInst_t		craInst  = sCraInstGet(mdtObj);
	int	i;
#if 0
	SII_LIB_LOG_PRINT2(("\n\n\n\nMDT_ERROR: HALTED\n\n\n\nRegister values...."));

	SII_LIB_LOG_PRINT2(("Reg 0x86 = %02X\n", (int)SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__MDT_RCV_CONTROL)));
	SII_LIB_LOG_PRINT2(("Reg 0x87 = %02X\n", (int)SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__MDT_RCV_READ_PORT)));
	SII_LIB_LOG_PRINT2(("Reg 0x88 = %02X\n", (int)SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__MDT_XMIT_CONTROL)));
	SII_LIB_LOG_PRINT2(("Reg 0x8A = %02X\n", (int)SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__MDT_RFIFO_STAT)));
	SII_LIB_LOG_PRINT2(("Reg 0x8B = %02X\n", (int)SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__MDT_XFIFO_STAT)));
	SII_LIB_LOG_PRINT2(("Reg 0x8C = %02X\n", (int)SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__MDT_INT_0)));
	SII_LIB_LOG_PRINT2(("Reg 0x8D = %02X\n", (int)SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__MDT_INT_0_MASK)));
	SII_LIB_LOG_PRINT2(("Reg 0x8E = %02X\n", (int)SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__MDT_INT_1)));
#endif
	SII_LIB_LOG_PRINT2(("Incoming buffer is.. LENGTH = %03d\n", (int)mdtObj->inBuffer[0]));
	for(i = 1 ; i < 17; i++)
	{
		SII_LIB_LOG_PRINT2(("%02X ", (int) mdtObj->inBuffer[ i ]));
	}

	// Completely stall to avoid any I2C traffic
	SII_LIB_LOG_PRINT2(("Hit any key to exit the application...\n"));
	while(!_kbhit());
	exit(1);
}

static void sMdtErrorStatusCheck(MdtObj_t *mdtObj)
{
	SiiDrvCraAddr_t	baseAddr = sBaseAddrGet(mdtObj);
	SiiInst_t		craInst  = sCraInstGet(mdtObj);
	uint8_t status = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__MDT_INT_1);

	if (status & BIT_MSK__MDT_INT_1__REG_MDT_INT_1_0)
		SII_LIB_LOG_PRINT2(("Receive time out status\n"));
	if (status & BIT_MSK__MDT_INT_1__REG_MDT_INT_1_1)
		SII_LIB_LOG_PRINT2(("Receive state machine received abort packet status\n"));
	if (status &BIT_MSK__MDT_INT_1__REG_MDT_INT_1_2)
		SII_LIB_LOG_PRINT2(("MDT Receive state machine error status\n"));
	if (status & BIT_MSK__MDT_INT_1__REG_MDT_INT_1_5)
		SII_LIB_LOG_PRINT2(("Transmit time out status	\n"));
	if (status & BIT_MSK__MDT_INT_1__REG_MDT_INT_1_6)
		SII_LIB_LOG_PRINT2(("Transmit state machine received abort packet status\n"));
	if (status & BIT_MSK__MDT_INT_1__REG_MDT_INT_1_7)
		SII_LIB_LOG_PRINT2(("Transmit state machine error status\n"));

	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__MDT_INT_1, status);
}
static void	sMdtDataCompare(MdtObj_t *mdtObj)
{
	SiiDrvCraAddr_t	baseAddr = sBaseAddrGet(mdtObj);
	SiiInst_t		craInst  = sCraInstGet(mdtObj);

	uint8_t	thisLength = 16;

	uint8_t	status;

	int	iterationCount = 0;
	int	sendCount = 0;

	int	i;
	int	printed = false;

	if (!( mdtObj->mdt_xmit_enable || mdtObj->mdt_flood_test_enable || mdtObj->mdt_rxc_enable))
		return;

	sMdtErrorStatusCheck(mdtObj);

	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__MDT_INT_0_MASK, 0x01);				// 0x8D = 1 (enable MDT receive interrupt)
//	SiiLibTimeMilliDelay(25);
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__MDT_INT_0, 0xFF);					// Clear the interrupt bits at 0x8C
//	SiiLibTimeMilliDelay(25);
   // SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__MDT_XMIT_CONTROL, 0x80);			// 0x88[7] = 1	to enable the MDT Transmitter
   // SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__MDT_RCV_CONTROL, 0x80);			// 0x86[7] = 1	to enable the MDT Receiver

#ifdef SWWA_29209
{
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__MDT_RCV_TIMEOUT, 0x00);			    // Disable MDT receiving timeout - CBUS:0x84
//	SiiLibTimeMilliDelay(25);
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__MDT_XMIT_TIMEOUT, 0x00);			// Disable MDT transmission timeout - CBUS:0x85
//	SiiLibTimeMilliDelay(25);
}
#else
{
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__MDT_RCV_TIMEOUT, 0x02);			   
    SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__MDT_XMIT_TIMEOUT, 0x02);			 
}
#endif

   	//mdtObj->currentByteOut = 0;

	// Forever do
	//	If 8B[7:5] > 0 it means we can write
	//		Write 1+16 bytes in a loop on [89]
	//	if 8C[0] == 1 it means we got data on receiver
	//		Read [87]. if not 16, go to error
	//		Read 1+16 bytes in a loop from [87]
	//	if(unexpected byte) go to error
	//		Move pointer to the next 86[0] = 0x81
	// done
	// error: dump registers 86 to 8F, except 87 and 89?
	//		  dump 16 bytes from 87
	//
//	while(1)
//	{
if (mdtObj->mdt_xmit_enable || mdtObj->mdt_flood_test_enable)
{
	status = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__MDT_XFIFO_STAT);
	if(status & 0xE0)
	{
		sendCount++;
		mdtObj->outBuffer[0] = 15;

		SII_LIB_LOG_PRINT2(("MDT XMited Data: "));
		for(i = 1 ; i < 17; i++)
		{
			mdtObj->outBuffer[ i ] = mdtObj->currentByteOut++;
			SII_LIB_LOG_PRINT2(("%02x ", mdtObj->outBuffer[i]));
		}
		SII_LIB_LOG_PRINT2(("\n"));	

		//				SiiLibTimeMilliDelay(50);

		SiiDrvCraBlockRead8(craInst, baseAddr | REG_ADDR__MDT_XMIT_WRITE_PORT, mdtObj->outBuffer, 17);
	}
}

	if (mdtObj->mdt_rxc_enable || mdtObj->mdt_flood_test_enable)
	{
//		SiiLibTimeMilliDelay(25);
		//SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__MDT_INT_0, 0xFF);		// Clear the interrupt bits at 0x8C

		//
		// Receive when you can
		// if 8C[0] == 1 it means we got data on receiver
		//
		status = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__MDT_INT_0);
		if(0x01 & status)
			{
				if( 0 == mdtObj->currentByteIn)
				{
					//ERROR_DEBUG_PRINT("MDT: Iteration Count = %d\n", (int)iterationCount++);
				}

				for(i = 0 ; i < 17; i++)
				{
					mdtObj->inBuffer[ i ] = 0xa5;
				}

				//ERROR_DEBUG_PRINT("MDT: Reading bytes Expecting from %02X\n", (int)mdtObj->currentByteIn);

//#if defined(BUS_TYPE_SPI)
//				mhl_tx_mdt_fifo_read(mdtObj->inBuffer, 17);
//#elif defined(BUS_TYPE_I2C)
				SiiDrvCraBlockRead8(craInst, baseAddr | REG_ADDR__MDT_RCV_READ_PORT, mdtObj->inBuffer, 17);
//#endif


				//
				// Check length
				//
				if( mdtObj->inBuffer[ 0 ] != 16)
				{
					SII_LIB_LOG_PRINT2(("MDT_ERROR: LENGTH MISMATCH. Expected %02d, Got %02d\n", (int)16, (int)mdtObj->inBuffer[ 0 ]));
					sMdtErrorHalt(mdtObj);
				}
				//
				// Check data
				//
				SII_LIB_LOG_PRINT2(("MDT Reveived Data: "));
				for(i = 1 ; i < 17; i++)
				{
					SII_LIB_LOG_PRINT2(("%02x ", mdtObj->inBuffer[i]));
					if( mdtObj->inBuffer[i] != mdtObj->currentByteIn)
					{
						SII_LIB_LOG_PRINT2(("MDT_ERROR: DATA MISMATCH. Expected %02X, Got %02X\n", (int)mdtObj->currentByteIn, (int)mdtObj->inBuffer[i]));
						sMdtErrorHalt(mdtObj);
					}
					mdtObj->currentByteIn++;
				}
				SII_LIB_LOG_PRINT2(("\n"));
				//	Move pointer to the next 86[0] = 0x81
		    	//mhl_tx_reg_modify(REG_ADDR__MDT_RCV_CONTROL, 0x81, 0x81);
				SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__MDT_RCV_CONTROL, 0x81);

				// Clear interrupt bit
				//SiiLibTimeMilliDelay(25);
      			SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__MDT_INT_0, 0x01);
			}
	}
}

#if 0
void sMdtWrBlock(MdtObj_t *mdtObj, uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4)
{
	SiiDrvCraAddr_t	baseAddr = sBaseAddrGet(mdtObj);
	SiiInst_t		craInst  = sCraInstGet(mdtObj);
	uint8_t outBuffer[17];
	uint8_t i;
	uint8_t byteSend=17;

	uint8_t MDT_XFIFO_STAT;
	uint8_t MDT_XMIT_CONTROL;

	//MDT_XFIFO_STAT = read ("RX CBUS(0)", 0x8B);
	//print ("MDT_XFIFO_STAT = %x\n\n", MDT_XFIFO_STAT);

	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__MDT_XMIT_CONTROL, 0x12);  // MDT XFIFO clear, fixed aid
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__MDT_RCV_CONTROL, 0x02);  // MDT RFIFO clear

	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__MDT_PEER_ADOPER_ID_LOW, 0x55);  // adopter ID low
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__MDT_PEER_ADOPER_ID_HIGH, 0xaa);  // adopter ID high

	outBuffer[0] = byteSend-1;
	for(i = 1 ; i < byteSend; i++)
			outBuffer[ i ] = 0xb1 | i;
	SiiDrvCraBlockWrite8(craInst, baseAddr | REG_ADDR__MDT_XMIT_WRITE_PORT, outBuffer, byteSend); 

	for(i = 1 ; i < byteSend; i++)
		outBuffer[ i ] = 0xb2 | i;
	SiiDrvCraBlockWrite8(craInst, baseAddr | REG_ADDR__MDT_XMIT_WRITE_PORT, outBuffer, byteSend); 

	for(i = 1 ; i < byteSend; i++)
		outBuffer[ i ] = 0xb3 | i;
	SiiDrvCraBlockWrite8(craInst, baseAddr | REG_ADDR__MDT_XMIT_WRITE_PORT, outBuffer, byteSend); 

	for(i = 1 ; i < byteSend; i++)
		outBuffer[ i ] = 0xb4 | i;
	SiiDrvCraBlockWrite8(craInst, baseAddr | REG_ADDR__MDT_XMIT_WRITE_PORT, outBuffer, byteSend); 
	
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__MDT_XMIT_CONTROL, 0x90); // enable MDT transmit, fixed aid
	MDT_XFIFO_STAT = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__MDT_XMIT_CONTROL);
	SII_LIB_LOG_PRINT2(("MDT_XMIT_CONTROL = %x\n\n", MDT_XFIFO_STAT));
}
#endif

void sMDTWrData(MdtObj_t *mdtObj, uint8_t val)
{
	SiiDrvCraAddr_t	baseAddr = sBaseAddrGet(mdtObj);
	SiiInst_t		craInst  = sCraInstGet(mdtObj);
	uint8_t outBuffer[17];
	uint8_t byteSend=17;
	uint8_t i;

	outBuffer[0] = 15;
	
	for(i = 1 ; i < byteSend; i++)
	{
			outBuffer[i] = val | (i-1);
			SII_LIB_LOG_PRINT2(("%02x ", outBuffer[i]));
	}
	SII_LIB_LOG_PRINT2(("\n"));
	SiiDrvCraBlockRead8(craInst, baseAddr | REG_ADDR__MDT_XMIT_WRITE_PORT, outBuffer, byteSend); 
	//val = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__MDT_RFIFO_STAT);  // added dummy write required for Sherman
}
static void sMdtWrBlock(MdtObj_t *mdtObj, uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4)
{
	SiiDrvCraAddr_t	baseAddr = sBaseAddrGet(mdtObj);
	SiiInst_t		craInst  = sCraInstGet(mdtObj);
	uint8_t xfifoStatus;

	//MDT_XFIFO_STAT = read ("RX CBUS(0)", 0x8B);
	//print ("MDT_XFIFO_STAT = %x\n\n", MDT_XFIFO_STAT);

	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__MDT_XMIT_CONTROL, 0x02);  // MDT XFIFO clear, fixed aid
	//SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__MDT_RCV_CONTROL, 0x02);  // MDT RFIFO clear

	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__MDT_PEER_ADOPER_ID_LOW, 0x55);  // adopter ID low
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__MDT_PEER_ADOPER_ID_HIGH, 0xaa);  // adopter ID high

	SII_LIB_LOG_PRINT2(("Transfer MDT data:\n"));
	sMDTWrData(mdtObj, b1);
	sMDTWrData(mdtObj, b2);
	sMDTWrData(mdtObj, b3);
	sMDTWrData(mdtObj, b4);
	
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__MDT_XMIT_CONTROL, 0x80); // enable MDT transmit, fixed aid
	xfifoStatus = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__MDT_XMIT_CONTROL);
}

static void sMdtRdBlock(MdtObj_t *mdtObj)
{
	SiiDrvCraAddr_t	baseAddr = sBaseAddrGet(mdtObj);
	SiiInst_t		craInst  = sCraInstGet(mdtObj);
	uint8_t inBuffer[17];
	uint8_t i;
	uint8_t val;
	//SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__MDT_XMIT_CONTROL, 0x80);			// 0x88[7] = 1	to enable the MDT Transmitter

	//SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__MDT_RCV_CONTROL, 0x80);	
	//SII_LIB_LOG_PRINT2(("MDT Read Block:\n"));
	//val = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__MDT_RFIFO_STAT);
	for (val=0; val < 4; val++)
	{
		SiiDrvCraBlockRead8(craInst, baseAddr | REG_ADDR__MDT_RCV_READ_PORT, inBuffer, 17);
		for(i = 0 ; i < 17; i++)
			SII_LIB_LOG_PRINT2(("%02x ", inBuffer[i]));
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__MDT_RCV_CONTROL, 0x81);	
		SII_LIB_LOG_PRINT2(("\n"));
	}
}

static void sMdtRdXmitFifoBlock(MdtObj_t *mdtObj)
{
	SiiDrvCraAddr_t	baseAddr = sBaseAddrGet(mdtObj);
	SiiInst_t		craInst  = sCraInstGet(mdtObj);
	uint8_t inBuffer[17];
	uint8_t i;
	uint8_t val;
	//SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__MDT_XMIT_CONTROL, 0x80);			// 0x88[7] = 1	to enable the MDT Transmitter

	//SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__MDT_RCV_CONTROL, 0x80);	
	//SII_LIB_LOG_PRINT2(("MDT Read Block:\n"));
	//val = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__MDT_RFIFO_STAT);
	for (val=0; val < 4; val++)
	{
		SiiDrvCraBlockRead8(craInst, baseAddr | REG_ADDR__MDT_XMIT_WRITE_PORT, inBuffer, 17);
		for(i = 0 ; i < 17; i++)
			SII_LIB_LOG_PRINT2(("%02x ", inBuffer[i]));
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__MDT_XMIT_CONTROL, 0x81);	
		SII_LIB_LOG_PRINT2(("\n"));
	}
}

static void sMdtStatus(MdtObj_t *mdtObj)
{
	SiiDrvCraAddr_t	baseAddr = sBaseAddrGet(mdtObj);
	SiiInst_t		craInst  = sCraInstGet(mdtObj);
	uint8_t status;
	status = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__MDT_RCV_CONTROL);
	SII_LIB_LOG_PRINT2(("MDT Receive: 0x%x\n", status));
	status = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__MDT_XMIT_CONTROL);
	SII_LIB_LOG_PRINT2(("MDT Xmit: 0x%x\n", status));
}

HI_BOOL sMdtFloodTestEnable(MdtObj_t *mdtObj)
{
	SiiDrvCraAddr_t	baseAddr = sBaseAddrGet(mdtObj);
	SiiInst_t		craInst  = sCraInstGet(mdtObj);
	mdtObj->mdt_flood_test_enable = !mdtObj->mdt_flood_test_enable;
	mdtObj->mdt_xmit_enable = false;
	if (mdtObj->mdt_flood_test_enable) 
		sMdtTestInit(mdtObj);
	
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__MDT_RCV_TIMEOUT, 0x00);			// 0x88[7] = 1	to enable the MDT Transmitter
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__MDT_XMIT_TIMEOUT, 0x00);			// 0x88[7] = 1	to enable the MDT Transmitter

	SII_LIB_LOG_PRINT2(("MDT flood test %s for both transmit and receive\n", mdtObj->mdt_flood_test_enable ? "enable" : "disable"));
	return mdtObj->mdt_flood_test_enable;
}

static void sMdtXmitEnable(MdtObj_t *mdtObj)
{
	SiiDrvCraAddr_t	baseAddr = sBaseAddrGet(mdtObj);
	SiiInst_t		craInst  = sCraInstGet(mdtObj);
	sMdtTestInit(mdtObj);
	mdtObj->mdt_xmit_enable = true;
	mdtObj->mdt_flood_test_enable = false;
	mdtObj->mdt_rxc_enable = false;
	
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__MDT_RCV_TIMEOUT, 0x00);			// 0x88[7] = 1	to enable the MDT Transmitter
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__MDT_XMIT_TIMEOUT, 0x00);			// 0x88[7] = 1	to enable the MDT Transmitter

	SII_LIB_LOG_PRINT2(("MDT flood transmit data to peer\n"));
}

static void sMdtRxcEnable(MdtObj_t *mdtObj)
{
	SiiDrvCraAddr_t	baseAddr = sBaseAddrGet(mdtObj);
	SiiInst_t		craInst  = sCraInstGet(mdtObj);
	sMdtTestInit(mdtObj);
	mdtObj->mdt_xmit_enable = false;
	mdtObj->mdt_flood_test_enable = false;
	mdtObj->mdt_rxc_enable = true;
	
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__MDT_RCV_TIMEOUT, 0x00);			// 0x88[7] = 1	to enable the MDT Transmitter
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__MDT_XMIT_TIMEOUT, 0x00);			// 0x88[7] = 1	to enable the MDT Transmitter

	SII_LIB_LOG_PRINT2(("MDT flood test receive data from peer\n"));
}

static void sMdtFloodTestDisable(MdtObj_t *mdtObj)
{
	mdtObj->mdt_xmit_enable = false;
	mdtObj->mdt_flood_test_enable = false;
	mdtObj->mdt_rxc_enable = false;
	SII_LIB_LOG_PRINT2(("MDT flood turn off\n"));
}

static SiiDrvCraAddr_t sBaseAddrGet(MdtObj_t *pObj)
{
	return pObj->pConfig->baseAddr;
}

static SiiInst_t sCraInstGet(MdtObj_t *pObj)
{
	return pObj->pConfig->craInst;
}

//#endif


