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
* @file si_mod_tx_cbus_emsc.c
*
* @brief Cbus EMSC Module
*
*****************************************************************************/
#define SII_DEBUG
/***** #include statements ***************************************************/
#include "si_mod_tx_cbus_emsc_internal.h"
#include "si_mod_tx_mhl_api.h"
#include "si_drv_tx_regs.h"
#include "si_lib_malloc_api.h"
#include "si_lib_log_api.h"
#include "si_lib_time_api.h"
//#include <conio.h>

/***** Register Module name **************************************************/
SII_LIB_OBJ_MODULE_DEF(drv_tx_cbus_emsc);

/***** local type definitions ************************************************/
#define MIN_XMIT_SIZE 1
#define MAX_XMIT_SIZE 256

typedef struct
{
	SiiModEmscConfig_t *pConfig;

	int			next_xmit_size;
	uint8_t		next_xmit_data;
	uint8_t		next_rcv_data;
	uint8_t		emsc_xmit_buff[MAX_XMIT_SIZE];
	uint8_t		emsc_rcv_buff[MAX_XMIT_SIZE];
	bool_t		emscPrintEnable;
	bool_t		emsc_xmit_enable;
	bool_t		emsc_rxc_enable;
	bool_t		emsc_flood_test_enable;
	uint8_t		tryCnt;
} EmscObj_t;

//#define RDEBUG_ONLY //Todo > Remove Later

//Enable any one type of Test...
//#define TEST_TYPE_FIXED
#define TEST_TYPE_RANDOM
//#define PACKET_SIZE_ROTATION

#if defined(TEST_TYPE_FIXED)
#if !defined(PACKET_SIZE_ROTATION)
#define TEST_XMIT_SIZE 256
#endif
#endif

/***** local prototypes ******************************************************/
static void				sPrepNextXxmitBuff(EmscObj_t *emscObj);
static void				sCheckEmscRcvFifo(EmscObj_t *emscObj);
static int				sGetFifoSpaceAvail(EmscObj_t *emscObj);
static SiiInst_t		sCraInstGet(EmscObj_t *emscObj);
static SiiDrvCraAddr_t	sBaseAddrGet(EmscObj_t *emscObj);
static void				sEmscWrite(EmscObj_t *emscObj, const uint8_t *buffer, uint16_t count);
static uint16_t			sEmscRead(EmscObj_t *emscObj, uint8_t *buffer, uint16_t expected_length, HI_BOOL check_count);
static void				sEmscRxcEnable(EmscObj_t *emscObj);
static void				sEmscXmitEnable(EmscObj_t *emscObj);
static bool_t			sEmscFloodTestEnable (EmscObj_t *emscObj);
static void				sEmscHelp(EmscObj_t *emscObj);
static void				sEmscFifo(EmscObj_t *emscObj);
static void				sEmscReadBlock(EmscObj_t *emscObj);
static void				sEmscWriteBlock(EmscObj_t *emscObj, uint8_t block, uint16_t count);

/***** public functions ******************************************************/
SiiInst_t SiiModCbusEmscCreate(char *pNameStr, SiiModEmscConfig_t *pConfig)
{
	EmscObj_t *emscObj = NULL;
	SiiDrvCraAddr_t	baseAddr;
	SiiInst_t		craInst;

	/* Allocate memory for mhl_tx object */
	emscObj = (EmscObj_t*)SII_LIB_OBJ_CREATE(pNameStr, sizeof(EmscObj_t));
	SII_PLATFORM_DEBUG_ASSERT(emscObj);

	emscObj->pConfig = (SiiModEmscConfig_t*)SiiLibMallocCreate(sizeof(SiiModEmscConfig_t));
	memcpy(emscObj->pConfig, pConfig, sizeof(SiiModEmscConfig_t));

	baseAddr = sBaseAddrGet(emscObj);
	craInst  = sCraInstGet(emscObj);

	//uint32_t seed = (uint32_t)time(NULL);

	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__NULLCH			, 0x00);
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__TRAINCH			, 0xFF);
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__COMMAWIN			, 0x18);
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__EMSCINTRMASK		, 0x07);

	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__GENCTL			, 0x01);
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__COMMECNT			, 0x83);


	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__COMMECNT			, 0x83);
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__EMSCINTRMASK		, 0x01);
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__COMMACH1			, 0x96);
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__NULLCH			, 0x00);
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__TRAINCH			, 0xff);
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__COMMAWIN			, 0x30);
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__TXTRAINBCNTL		, 0x2c);
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__TXTRAINBCNTH		, 0x01);
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__RXTRAINBCNTL		, 0x2c);
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__RXTRAINBCNTH		, 0x01);
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__EMSCRCVTOUT		, 0xff);
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__EMSCXMITTOUT		, 0xff);
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__EMSCXMITRETRYMAXH	, 0x07);
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__EMSCXMITRETRYMAXL	, 0xff);
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__EMSCXMITRETRYLMT	, 0xff);
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__GENCTL			, 0x01);
	//SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__EMSCXFIFOSPACEL , 0x20);
	//SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__EMSCXFIFOSPACEH , 0x01);

	//# of retry													  
	SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__EMSCXMITRETRYLMT	, 0x06);

	//SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__DISC_CTRL9		, 0x14);	

#if defined(TEST_TYPE_RANDOM)
	//SII_LIB_LOG_PRINT2(("Random # Seed: 0x%08X\n", seed));
	//srand(seed);
#endif
	sPrepNextXxmitBuff(emscObj);

	return SII_LIB_OBJ_INST(emscObj);
}

void SiiModCbusEmscDelete(SiiInst_t emscInst)
{
	EmscObj_t *emscObj = (EmscObj_t*)SII_LIB_OBJ_PNTR(emscInst);

	SiiLibMallocDelete(emscObj->pConfig);
	SII_LIB_OBJ_DELETE(emscObj);
}

void SiiModCbusEmscIntrHandler(SiiInst_t emscInst)
{
	EmscObj_t *emscObj = (EmscObj_t*)SII_LIB_OBJ_PNTR(emscInst);

	SiiDrvCraAddr_t	baseAddr = sBaseAddrGet(emscObj);
	SiiInst_t		craInst = sCraInstGet(emscObj);
	uint8_t status;	

	if (emscObj->emsc_flood_test_enable || emscObj->emsc_rxc_enable)
	{
		status = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__EMSCINTR);
		if (status & BIT_MSK__EMSCINTR__REG_XMIT_DONE)
		{
			SII_LIB_LOG_PRINT2(("\t**EMSC Xmit Done.**\n"));
			SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__EMSCINTR, BIT_MSK__EMSCINTR__REG_XMIT_DONE);
		}
		if (status & BIT_MSK__EMSCINTR__REG_XMIT_GNT_TOUT)
		{
			SII_LIB_LOG_PRINT2(("EMSC Grant Time Out error: 0%0X\n", status));
			SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__EMSCINTR, BIT_MSK__EMSCINTR__REG_XMIT_GNT_TOUT);
			SII_LIB_LOG_PRINT2(("Press any key to continue..."));
			while (!_kbhit()) {	};
		}
		if (status & BIT_MSK__EMSCINTR__REG_XFIFO_WRITE_ERR)
		{
			SII_LIB_LOG_PRINT2(("EMSC Write Error: 0%0x\n", status));
			SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__EMSCINTR, BIT_MSK__EMSCINTR__REG_XFIFO_WRITE_ERR);
			SII_LIB_LOG_PRINT2(("Press any key to continue..."));
			while (!_kbhit()) {	};
		}
		if (status & BIT_MSK__EMSCINTR__REG_XMIT_ACK_TOUT)
		{
			SII_LIB_LOG_PRINT2(("EMSC Ack: 0%0x\n", status));
			SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__EMSCINTR, BIT_MSK__EMSCINTR__REG_XMIT_ACK_TOUT);
			SII_LIB_LOG_PRINT2(("Press any key to continue..."));
			while (!_kbhit()) {	};
		}
		if (status & BIT_MSK__EMSCINTR__REG_EMSC_RFIFO_NOT_EMPTY)
		{
			// INT is cleared when FIFO is empty
			sCheckEmscRcvFifo(emscObj);
		}
	}
}

void SiiModStartEmscFloodTest(SiiInst_t emscInst)
{
	EmscObj_t *emscObj = (EmscObj_t*)SII_LIB_OBJ_PNTR(emscInst);
	char key[3];
	uint8_t input = 0;
	do {
		sEmscHelp(emscObj);
		while(!_kbhit());
		gets_s(key, sizeof(key));
		input = atoi(key);

		if((key == "x") || (key == "X"))
			return;

		if(input < 0 && input > 9)
		{
			SII_LIB_LOG_PRINT2(("Invalid Input... Returning\n"));
			return;
		}

		switch(input)
		{
		case 0:
			SII_LIB_LOG_PRINT2(("Read bytes from EMSC\n"));			
			sEmscReadBlock(emscObj);
			break;
		case 1:
			sEmscFloodTestEnable(emscObj);
			break;
		case 2:
			sEmscXmitEnable(emscObj);
			break;
		case 3:
			sEmscRxcEnable(emscObj);
			break;
		case 4:
			sEmscFifo(emscObj);
			break;
		case 5:
			//SII_LIB_LOG_PRINT2(("Transmit 1 block  of 64 bytes to peer\n"));
			sEmscWriteBlock(emscObj, 1, 64);
			break;
		case 6:
			//SII_LIB_LOG_PRINT2(("Transmit 2 block  of 64 bytes to peer\n"));
			sEmscWriteBlock(emscObj, 2, 64);
			break;
		case 7:
			//SII_LIB_LOG_PRINT2(("Transmit 4 block  of 64 bytes to peer\n"));		
			sEmscWriteBlock(emscObj, 4, 64);
			break;
		case 8:
			//SII_LIB_LOG_PRINT2(("Transmit 256 bytes\n"));
			sEmscWriteBlock(emscObj, 1, 256);	//Todo : Check block size
			break;
		case 9:
			//SII_LIB_LOG_PRINT2(("Transmit 256 bytes then 32 bytes to peer\n"));
			sEmscWriteBlock(emscObj, 1, 256);	//Todo : Check block size
			sEmscWriteBlock(emscObj, 1, 32);
			break;
		default:
			SII_LIB_LOG_PRINT2(("Invalid Input...\n"));
		}
	} while(true);
}

static void SiiModEmscTransmit(EmscObj_t *emscObj)
{
	SiiDrvCraAddr_t	baseAddr = sBaseAddrGet(emscObj);
	SiiInst_t		craInst = sCraInstGet(emscObj);
	uint8_t status;
	static uint16_t count=0;
	int xfifo_space;
	//uint8_t emsc_status[7]; // 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E
	if (emscObj->emsc_xmit_enable)
	{
		if ((xfifo_space = sGetFifoSpaceAvail(emscObj)) >= (emscObj->next_xmit_size + 1))
		{
			SII_LIB_LOG_DEBUG1(emscObj, "emsc # of bytes transferring (+ BurstCount): %d\n", emscObj->next_xmit_size + 1);	
			SII_LIB_LOG_DEBUG1("EMSC XFIFO Space:  %d bytes\n", xfifo_space);		
			sEmscWrite(emscObj, emscObj->emsc_xmit_buff, emscObj->next_xmit_size);
			status = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__EMSCINTR);
			SII_LIB_LOG_DEBUG2("Interrupt status:   %0X\n", status);
			sPrepNextXxmitBuff(emscObj);
			count = 0;
		}
	}
}

uint16_t packetData[] = {1, 20, 256, 5, 129, 256,  63};
uint8_t packetIndex=0;

static void sPrepNextXxmitBuff (EmscObj_t *emscObj)
{
	int i;
#if defined(TEST_TYPE_FIXED)
#if defined(PACKET_SIZE_ROTATION)
	static int packet_size = 1;
#else
	static int packet_size = TEST_XMIT_SIZE;
#endif
	emscObj->next_xmit_size = packet_size;

	for (i = 0; i < emscObj->next_xmit_size; i++)
	{
		emscObj->emsc_xmit_buff[i] = emscObj->next_xmit_data++;
	}

#ifdef RDEBUG_ONLY
	emscObj->emsc_xmit_buff[0] = 255;
#endif

#if defined(PACKET_SIZE_ROTATION)
	switch (packet_size)
	{
	case 1:
		packet_size = 2;
		break;
	case 2:
		packet_size = 4;
		break;
	case 4:
		packet_size = 8;
		break;
	case 8:
		packet_size = 16;
		break;
	case 16:q

				packet_size = 40;
		break;
	case 40:
		packet_size = 100;
		break;
	case 100:
		packet_size = 256;
		break;
	case 256:
	default:
		packet_size = 1;
		break;
	}
#endif
#elif defined(TEST_TYPE_RANDOM)
	//emscObj->next_xmit_size = (int)((double)rand() / (RAND_MAX + 1) * ((MAX_XMIT_SIZE + 1) - MIN_XMIT_SIZE) + MIN_XMIT_SIZE);
	emscObj->next_xmit_size = packetData[packetIndex++];
	packetIndex = packetIndex % 7;

	for (i = 0; i < emscObj->next_xmit_size; i++)
	{
		emscObj->emsc_xmit_buff[i] = emscObj->next_xmit_data++;
	}
#endif
}

static int sGetFifoSpaceAvail(EmscObj_t *emscObj)
{
	SiiDrvCraAddr_t	baseAddr = sBaseAddrGet(emscObj);
	SiiInst_t		craInst = sCraInstGet(emscObj);
	int rfifo_bcnt, xfifo_space, xfifo_bcnt;
	uint8_t emsc_status[7]; // 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E

	SiiDrvCraBlockRead8(craInst, baseAddr | REG_ADDR__EMSCXFIFOBCNTL, emsc_status, 7);
	xfifo_bcnt = ((emsc_status[1] << 8) | (emsc_status[0]));
	rfifo_bcnt = ((emsc_status[3] << 8) | (emsc_status[2]));
	xfifo_space = ((emsc_status[5] << 8) | (emsc_status[4]));

#if 0
	if (emscObj->emsc_xmit_enable)
	{
		SII_LIB_LOG_PRINT2(("EMSC XFIFO BCnt:   %d bytes.\n", xfifo_bcnt));
		SII_LIB_LOG_PRINT2(("EMSC XFIFO Space:  %d bytes", xfifo_space));
	}
	else if (emscObj->emsc_rxc_enable)
	{
		SII_LIB_LOG_PRINT2(("EMSC RFIFO BCnt:   %d bytes.\n", rfifo_bcnt));
		SII_LIB_LOG_PRINT2(("EMSC SPI BurstCnt: %d bytes.\n", emsc_status[6]));
	}
#endif
	if (xfifo_space > 288)
	{
		if (emscObj->tryCnt == 1)
		{
			emscObj->tryCnt++;
			xfifo_space = sGetFifoSpaceAvail(emscObj);

			if (xfifo_space > 288)
			{
				SII_LIB_LOG_PRINT2(("eMSC xfifo space is too big.\n"));			
				emscObj->tryCnt = 1;				
			}
		}		
	}

	return xfifo_space;
}

static void sCheckEmscRcvFifo (EmscObj_t *emscObj)
{
	uint16_t i, rcv_size;

	sGetFifoSpaceAvail(emscObj);

	rcv_size = sEmscRead(emscObj, emscObj->emsc_rcv_buff, 0, true);

#ifdef RDEBUG_ONLY
	if (emscObj->emsc_rcv_buff[0] != 255)
	{
		SII_LIB_LOG_PRINT2(("EMSC RFIFO[%d] Data = 0x%02X. Expected: 0x%02X\nPress any key to continue.\n", 0, emscObj->emsc_rcv_buff[0], 255));
		while (!_kbhit()) {	};
	}
	emscObj->next_rcv_data++;

	for (i = 1; i < rcv_size; i++)
#else
	for (i = 0; i < rcv_size; i++)
#endif
	{
		if (emscObj->emsc_rcv_buff[i] != emscObj->next_rcv_data++)
		{
			SII_LIB_LOG_PRINT2(("EMSC RFIFO[%d] Data = 0x%02X. Expected: 0x%02X\nPress any key to continue.\n", i, emscObj->emsc_rcv_buff[i], emscObj->next_rcv_data - 1));
			while (!_kbhit()) {	};
			break;
		}
	}

	if (i == rcv_size)
	{
		SII_LIB_LOG_PRINT2(("EMSC RFIFO data matches expected.\n"));
	}
}

static void sEmscWrite(EmscObj_t *emscObj, const uint8_t *buffer, uint16_t count)
{
	SiiDrvCraAddr_t	baseAddr = sBaseAddrGet(emscObj);
	SiiInst_t		craInst = sCraInstGet(emscObj);
	uint16_t i = 0;
	i = i;

	// Enable I2C communication with EMSC instead of SPI
	SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__COMMECNT, BIT_MSK__COMMECNT__REG_I2C_TO_EMSC_EN);
	// eMSC Transmitter Write Port for I2C
	SiiDrvCraBlockWrite8(craInst, baseAddr | REG_ADDR__EMSC_XMIT_WRITE_PORT, (uint8_t*)buffer, (uint8_t)count);

	SII_LIB_LOG_PRINT2(("\tEMSC W (%x)", count));
	for (i = 0; i < count; i++)
	{
		if (i != 0 && ((i & 0x000F) == 0x0000))
		{
			SII_LIB_LOG_PRINT2(("\n\t      "));
		}
		SII_LIB_LOG_PRINT2((" %02X", buffer[i]));
	}
	SII_LIB_LOG_PRINT2(("\n"));
}

static uint16_t sEmscRead(EmscObj_t *emscObj, uint8_t *buffer, uint16_t expected_length, HI_BOOL check_count)
{
	SiiDrvCraAddr_t	baseAddr = sBaseAddrGet(emscObj);
	SiiInst_t		craInst = sCraInstGet(emscObj);
	uint16_t count, i;
	uint8_t burstCnt;		
	uint8_t temp;		

	// Enable I2C communication with EMSC instead of SPI
	SiiDrvCraSetBit8(craInst, baseAddr | REG_ADDR__COMMECNT, BIT_MSK__COMMECNT__REG_I2C_TO_EMSC_EN);
	if (check_count)
	{
#ifndef ROGUE_DEBUG
		// For Titan/Cypress, read the burst count register before reading the count from the EMSC RFIFO
		burstCnt = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__EMSCBURSTCNT);
#endif
		// eMSC Receive Read Port for I2C
		temp = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__EMSC_RCV_READ_PORT);

		count = temp + 1; // byte count in fifo is 0-255 for 1-256 data bytes respectively.
		SII_LIB_LOG_PRINT2(("EMSC RFIFO COUNT: %d bytes\n", count));
		SiiLibTimeMilliDelay(5);

#ifdef ROGUE_DEBUG
		// For Rogue, read the burst count register after reading the count from the EMSC RFIFO
		//burstCnt = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__EMSCBURSTCNT);
#endif
		SII_LIB_LOG_PRINT2(("EMSC SPI BurstCnt: %d bytes.\n", burstCnt));
		SiiLibTimeMilliDelay(5);

		if (burstCnt != (count - 1))
		{
			SII_LIB_LOG_PRINT2(("spi burst count (%d) not equal data length %d (first byte in buffer)\n", burstCnt, count -1));
			SII_LIB_LOG_PRINT2(("Press any key to continue\n"));
			while (!_kbhit()) {	};
		}

		if (expected_length != 0) // expected length of 0 means caller does not know ahead of time what length to expect
		{
			if (expected_length != count)
			{
				SII_LIB_LOG_PRINT2(("\tExpected: %d bytes\nPress any key to continue.\n", expected_length));
				while (!_kbhit()) {	};
			}
		}
	}
	else
	{
		count = expected_length;
	}

	SII_LIB_LOG_PRINT2(("emsc # of bytes receving (including the Burst Count)   : %d %s\n", count+1, (count+1==257) ? "***" : ""));

	// eMSC Receive Read Port for I2C
	SiiDrvCraBlockRead8(craInst, baseAddr | REG_ADDR__EMSC_RCV_READ_PORT, buffer, (uint8_t)count);

	SII_LIB_LOG_PRINT2(("\tEMSC R"));

	for (i = 0; i < count; i++)
	{
		if (i != 0 && ((i & 0x000F) == 0x0000))
		{
			SII_LIB_LOG_PRINT2(("\n\t      "));
		}
		SII_LIB_LOG_PRINT2((" %02X", buffer[i]));
	}
	SII_LIB_LOG_PRINT2(("\n"));

	return count; 
}

static SiiDrvCraAddr_t sBaseAddrGet(EmscObj_t *pObj)
{
	return pObj->pConfig->baseAddr;
}

static SiiInst_t sCraInstGet(EmscObj_t *pObj)
{
	return pObj->pConfig->craInst;
}

static void sEmscWriteBlock(EmscObj_t *emscObj, uint8_t block, uint16_t count)
{
	SiiDrvCraAddr_t	baseAddr = sBaseAddrGet(emscObj);
	SiiInst_t		craInst = sCraInstGet(emscObj);

	// eMSC Transmitter Write Port for I2C

	uint16_t i;
	uint8_t buffer[1024];
	uint8_t val1;
	uint8_t val2;
	uint8_t index=0;
	int rfifo_bcnt, xfifo_space, xfifo_bcnt;
	uint8_t emsc_status[7]; // 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E	
	uint8_t cnt;

	SII_LIB_LOG_PRINT2(("EMSC WR Block:\n"));

	while (index < block)
	{
		//clear the status interrupts
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__EMSCINTR, 0xFF);

		for (i=0; i < count; i++)
		{
			buffer[i]= i + (index * 0x11);
			if ((i%16) == 0)
				SII_LIB_LOG_PRINT2(("\n"));	
			SII_LIB_LOG_PRINT2(("%02x ", buffer[i]));
		}
		SII_LIB_LOG_PRINT2(("\n"));
		SiiDrvCraBlockWrite8(craInst, baseAddr | REG_ADDR__EMSC_XMIT_WRITE_PORT, buffer, count);

		SiiDrvCraBlockRead8(craInst, baseAddr | REG_ADDR__EMSCXFIFOBCNTL, emsc_status, 7);
		xfifo_bcnt = ((emsc_status[1] << 8) | (emsc_status[0]));
		rfifo_bcnt = ((emsc_status[3] << 8) | (emsc_status[2]));
		xfifo_space = ((emsc_status[5] << 8) | (emsc_status[4]));	

		val2 = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__EMSCINTR);
		SII_LIB_LOG_PRINT2(("EMSC XFIFO BCnt:   %d bytes.\n", xfifo_bcnt));
		SII_LIB_LOG_PRINT2(("EMSC RFIFO BCnt:   %d bytes.\n", rfifo_bcnt));
		SII_LIB_LOG_PRINT2(("EMSC XFIFO Space:  %d bytes.\n", xfifo_space));
		SII_LIB_LOG_PRINT2(("Int Status:  0x%02x\n", val2));

		cnt=100;
		do
		{
			cnt--;
			val1 = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__EMSCINTR);
			if (!cnt)
			{
				SII_LIB_LOG_PRINT2(("Fifo is full cannot tranfer any more data\n"));
				SII_LIB_LOG_PRINT2(("Hit any key to continue\n"));
				while (!_kbhit()) {	};
				break;
			}
		}
		while (!(val1 & BIT_MSK__EMSCINTR__REG_XMIT_DONE));
		SiiDrvCraWrReg8(craInst, baseAddr | REG_ADDR__EMSCINTR, BIT_MSK__EMSCINTR__REG_XMIT_DONE);
		index++;
	}
}

static void sEmscReadBlock(EmscObj_t *emscObj)
{
	SiiDrvCraAddr_t	baseAddr = sBaseAddrGet(emscObj);
	SiiInst_t		craInst = sCraInstGet(emscObj);
	uint8_t count = 0;
	uint16_t i;
	int rfifo_bcnt, xfifo_space, xfifo_bcnt;			
	uint8_t buffer[1024];
	uint8_t emsc_status[7]; // 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E	

	SiiDrvCraBlockRead8(craInst, baseAddr | REG_ADDR__EMSCXFIFOBCNTL, emsc_status, 7);
	xfifo_bcnt = ((emsc_status[1] << 8) | (emsc_status[0]));
	rfifo_bcnt = ((emsc_status[3] << 8) | (emsc_status[2]));
	xfifo_space = ((emsc_status[5] << 8) | (emsc_status[4]));

	SII_LIB_LOG_PRINT2(("EMSC XFIFO BCnt:   %d bytes.\n", xfifo_bcnt));
	SII_LIB_LOG_PRINT2(("EMSC RFIFO BCnt:   %d bytes.\n", rfifo_bcnt));
	SII_LIB_LOG_PRINT2(("EMSC XFIFO Space:  %d bytes\n", xfifo_space));
	SII_LIB_LOG_PRINT2(("EMSC SPI BurstCnt: %d bytes.\n", emsc_status[6]));

	count = emsc_status[6] + 2; 
	if (count > 2)
	{
		// eMSC Receive Read Port for I2C
		SiiDrvCraBlockRead8(craInst, baseAddr | REG_ADDR__EMSC_RCV_READ_PORT, buffer, count);

		SII_LIB_LOG_PRINT2(("EMSC R (count: %d)\n", count));

		for (i = 0; i < count; i++)
		{
			if ((i % 16) == 0) 
			{
				SII_LIB_LOG_PRINT2(("\n"));
			}
			SII_LIB_LOG_PRINT2((" %02X", buffer[i]));
		}
		SII_LIB_LOG_PRINT2(("\n"));
	}
}

static void sEmscFifo(EmscObj_t *emscObj)
{
	SiiDrvCraAddr_t	baseAddr = sBaseAddrGet(emscObj);
	SiiInst_t		craInst = sCraInstGet(emscObj);
	int rfifo_bcnt, xfifo_space, xfifo_bcnt;
	uint8_t val;

	uint8_t emsc_status[7]; // 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E	

	SiiDrvCraBlockRead8(craInst, baseAddr | REG_ADDR__EMSCXFIFOBCNTL, emsc_status, 7);
	xfifo_bcnt = ((emsc_status[1] << 8) | (emsc_status[0]));
	rfifo_bcnt = ((emsc_status[3] << 8) | (emsc_status[2]));
	xfifo_space = ((emsc_status[5] << 8) | (emsc_status[4]));

	val = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__GENCTL);
	SII_LIB_LOG_PRINT2(("%s\n", (val & BIT_MSK__GENCTL__REG_EMSC_EN) ? "EMSC enable" : "EMSC disable")); 
	val = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__EMSCSTAT);
	SII_LIB_LOG_PRINT2(("%s\n", (val & BIT_MSK__EMSCSTAT__REG_EMSC_NORMAL_MODE) ? "EMSC on" : "EMSC off")); 
	val = SiiDrvCraRdReg8(craInst, baseAddr | REG_ADDR__TRXSTA2); //Page3_6B:Bits[7-6]
	SII_LIB_LOG_PRINT2(("%s\n", (val & 0x80) ? "TDM on" : "TDM off")); 

	SII_LIB_LOG_PRINT2(("EMSC XFIFO BCnt:   %d bytes.\n", xfifo_bcnt));
	SII_LIB_LOG_PRINT2(("EMSC RFIFO BCnt:   %d bytes.\n", rfifo_bcnt));
	SII_LIB_LOG_PRINT2(("EMSC XFIFO Space:  %d bytes\n", xfifo_space));
	SII_LIB_LOG_PRINT2(("EMSC SPI BurstCnt: %d bytes.\n", emsc_status[6]));	
}

static void sEmscHelp (EmscObj_t *emscObj)
{
	SII_LIB_LOG_PRINT2(("\n\nEMSC Menu:\n"));
	SII_LIB_LOG_PRINT2(("\t 0		- Read the EMSC data from the FIFO\n"));
	SII_LIB_LOG_PRINT2(("\t 1		- Run the EMSC flood test on both xmiting and receiving data\n"));
	SII_LIB_LOG_PRINT2(("\t 2		- Enable the EMSC to transmit data to peer (no receiving data)\n"));
	SII_LIB_LOG_PRINT2(("\t 3		- Enable the EMSC to receive data to peer (no transmitting data)\n"));
	SII_LIB_LOG_PRINT2(("\t 4		- Show TDM/EMSC status\n"));
	SII_LIB_LOG_PRINT2(("\t 5		- Transmit 1 block  of 64 bytes to peer\n"));
	SII_LIB_LOG_PRINT2(("\t 6		- Transmit 2 block  of 64 bytes to peer\n"));
	SII_LIB_LOG_PRINT2(("\t 7		- Transmit 4 block  of 64 bytes to peer\n"));
	SII_LIB_LOG_PRINT2(("\t 8		- Transmit 256 bytes\n"));
	SII_LIB_LOG_PRINT2(("\t 9		- Transmit 256 bytes then 32 bytes to peer\n"));
	SII_LIB_LOG_PRINT2(("\t x		- Exit ESMC Flood Test\n"));
}

static bool_t sEmscFloodTestEnable (EmscObj_t *emscObj)
{
	emscObj->emsc_flood_test_enable = !emscObj->emsc_flood_test_enable;
	emscObj->emsc_xmit_enable = true;
	SII_LIB_LOG_PRINT2(("EMSC flood test %s for both transmit and receive\n", emscObj->emsc_flood_test_enable ? "enable" : "disable"));
	//Enable ESMC INTR MASK
	SiiDrvCraWrReg8(emscObj->pConfig->craInst, emscObj->pConfig->baseAddr | REG_ADDR__EMSCINTRMASK, 0xFF);
	return emscObj->emsc_flood_test_enable;
}

static void sEmscXmitEnable (EmscObj_t *emscObj)
{
	emscObj->emsc_xmit_enable = true;
	emscObj->emsc_flood_test_enable = false;
	emscObj->emsc_rxc_enable = false;
	SII_LIB_LOG_PRINT2(("EMSC flood transmit data to peer\n"));
	//Enable ESMC INTR MASK
	SiiDrvCraWrReg8(emscObj->pConfig->craInst, emscObj->pConfig->baseAddr | REG_ADDR__EMSCINTRMASK, 0xFF);
}

static void sEmscRxcEnable (EmscObj_t *emscObj)
{
	emscObj->emsc_xmit_enable = false;
	emscObj->emsc_flood_test_enable = false;
	emscObj->emsc_rxc_enable = true;
	SII_LIB_LOG_PRINT2(("EMSC flood test receive data from peer\n"));
	//Enable ESMC INTR MASK
	SiiDrvCraWrReg8(emscObj->pConfig->craInst, emscObj->pConfig->baseAddr | REG_ADDR__EMSCINTRMASK, 0xFF);
}