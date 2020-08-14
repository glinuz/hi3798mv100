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
* file si_drv_tx_scdc.c
*
* brief Tx SCDC driver
*
*****************************************************************************/
#ifdef HDMI_SCDC_SUPPORT
//#define SII_DEBUG

/***** #include statements ***************************************************/

#include "si_datatypes.h"
#include "sii_time.h"
#ifndef HDMI_BUILD_IN_BOOT
#include "si_lib_seq_api.h"
#endif
#include "si_lib_malloc_api.h"
#include "si_drv_tx_regs_enums.h"
#include "si_drv_cra_api.h"
#include "si_lib_log_api.h"
#include "si_lib_time_api.h"
#include "si_mod_tx_scdc_api.h"
#include "hdmi_platform.h"

/* scramble states */
#define SCRAMBLE_INITIAL    0
#define SCRAMBLE_WAIT       1
#define SCRAMBLE_ENABLED    2
#define SCRAMBLE_NONE       3

#define PAGE_ADDR           0x3000           

/***** Register Module name **************************************************/
SII_LIB_OBJ_MODULE_DEF(drv_tx_scdc);

/***** local definitions ************************************************/

typedef struct
{
	SiiInst_t              parentInst;             // Parent instance. Provided when with call back
	SiiModTxScdcConfig_t   *config;                 // Static instance configuration
	
	SiiInst_t   timerScrambleExpire;    // max wait for scrambling enabled
	SiiInst_t   timerScramblePoll;      // polling for scrambling enabled
	SiiInst_t   timerRrIntPoll;         // polling for Read Request Interrupt

//	uint16_t    vclk_mb;                // video clock in MB, submitted by app layer

	bit_fld_t   sc_wait_expired;        // flag( 1 - expired, 0 - not expired )

	uint8_t     sc_state;               // (0 - SCRAMBLE_INITIAL, 1 - SCRAMBLE_WAIT, 2 - SCRAMBLE_ENABLE, 3 - SCRAMBLE_NONE)

	uint8_t     sinkScStatus;           // 0 - disabled , 1 - enabled.

	SiiModTxScdcSinKCaps_t sinkCaps;

	uint8_t     scdcUpdate0;
	uint8_t     scdcUpdate1;


} ScdcObj_t;

/***** local prototypes ******************************************************/
#ifndef HDMI_BUILD_IN_BOOT
static void sScrambleTimeout(SiiInst_t inst);
static void sScdcHandler(SiiInst_t inst);
#endif

static void sWriteScdcReg8(ScdcObj_t* pObj, uint8_t addr, uint8_t val);
static uint8_t sReadScdcReg8(ScdcObj_t* pObj, uint8_t addr);
static void SiiDrvTxForcePhyOutputEnable(SiiInst_t inst, bool_t enable);

//static void sScdcRegReadTest(SiiInst_t inst);
//static void sScdcInterruptHandler(SiiInst_t inst);

/***** local data objects ****************************************************/

/***** public functions ******************************************************/

SiiInst_t SiiModTxScdcCreate(char *pNameStr, SiiInst_t parentInst, SiiModTxScdcConfig_t *pConfig)
{
	ScdcObj_t*  pObj = NULL;

	/* Allocate memory for object */
	pObj = (ScdcObj_t*)SII_LIB_OBJ_CREATE(pNameStr, sizeof(ScdcObj_t));
	SII_PLATFORM_DEBUG_ASSERT(pObj);

    pObj->parentInst = parentInst;
	pObj->config = (SiiModTxScdcConfig_t*)SiiLibMallocCreate(sizeof(SiiModTxScdcConfig_t));
	memcpy(pObj->config, pConfig, sizeof(SiiModTxScdcConfig_t));
#ifndef HDMI_BUILD_IN_BOOT
	//Create timers for scrambling control
	pObj->timerScrambleExpire = SII_LIB_SEQ_TIMER_CREATE("SCR_Wait_Expire", sScrambleTimeout, SII_LIB_OBJ_INST(pObj), 189, SII_TIMER_SCR_WAIT_E);
	SII_PLATFORM_DEBUG_ASSERT(pObj->timerScrambleExpire);

	pObj->timerScramblePoll = SII_LIB_SEQ_TIMER_CREATE("SCR_Ready_Poll", sScdcHandler, SII_LIB_OBJ_INST(pObj), 190, SII_TIMER_SCR_RP);
	SII_PLATFORM_DEBUG_ASSERT(pObj->timerScramblePoll);
#endif
//	pObj->timerRrIntPoll = SII_LIB_SEQ_TIMER_CREATE("SCDC_Int_Poll", sScdcInterruptHandler, SII_LIB_OBJ_INST(pObj), 188, SII_TIMER_SCDC_IP);
//	SII_PLATFORM_DEBUG_ASSERT(pObj->timerRrIntPoll);

	return SII_LIB_OBJ_INST(pObj);
}

void SiiModTxScdcDelete(SiiInst_t inst)
{
	ScdcObj_t* pObj = (ScdcObj_t*)SII_LIB_OBJ_PNTR(inst);
#ifndef HDMI_BUILD_IN_BOOT
	SiiLibSeqTimerDelete(pObj->timerScrambleExpire);
	SiiLibSeqTimerDelete(pObj->timerScramblePoll);
#endif
	SiiLibMallocDelete(pObj->config);
	SII_LIB_OBJ_DELETE(pObj);
}

void SiiModTxScdcReset(SiiInst_t inst)
{
    SiiScdcStatus_t stScdcStatus;
    HI_U8           u8ScdcSetTimes = 10;
    
	ScdcObj_t* pObj = (ScdcObj_t*)SII_LIB_OBJ_PNTR(inst);

	SII_MEMSET(&pObj->sinkCaps, 0, sizeof(SiiModTxScdcSinKCaps_t));

	pObj->sc_state = SCRAMBLE_INITIAL;
    SiiDrvTxScdcStatusGet(inst, &stScdcStatus);

	do {    
        SiiDrvTxForcePhyOutputEnable(inst, HI_FALSE);
    
        SiiLibTimeMilliDelay(1 + (10 - u8ScdcSetTimes)* 5);
        stScdcStatus.bSinkScrambleOn = HI_FALSE;
        stScdcStatus.bSourceScrambleOn = HI_FALSE;
        stScdcStatus.tmdsBitClkRatio = 10;
        SiiDrvTxScdcStatusSet(inst, &stScdcStatus);

        SiiDrvTxForcePhyOutputEnable(inst, HI_TRUE);
        
        SiiLibTimeMilliDelay(1 + (10 - u8ScdcSetTimes) * 5);
        SII_LIB_LOG_DEBUG1(pObj, "scdc reset u8ScdcSetTimes: %d \n", u8ScdcSetTimes);
        SiiDrvTxScdcStatusGet(inst, &stScdcStatus);
        u8ScdcSetTimes--;

    }while(u8ScdcSetTimes && (stScdcStatus.tmdsBitClkRatio != 10));
    
    if((stScdcStatus.tmdsBitClkRatio != 10) || (!u8ScdcSetTimes))
    {
        SII_LIB_LOG_ERR(pObj, "scdc reset failed, u8ScdcSetTimes: %d \n", u8ScdcSetTimes);
    }
    else
    {
        SII_LIB_LOG_DEBUG1(pObj, "scdc reset success \n");
    }
    
    SiiDrvTxForcePhyOutputEnable(inst, HI_TRUE);
	pObj->sc_state = SCRAMBLE_INITIAL;
}


/***** local static functions ******************************************************/

static void sWriteScdcReg8(ScdcObj_t* pObj, uint8_t addr, uint8_t val)
{
	SiiDrvCraAddr_t baseAddr = pObj->config->baseAddr;
#ifndef HDMI_BUILD_IN_BOOT
    HDMI_MUTEX_LOCK(*pObj->config->pstDdcLock);
#endif
	do
	{
		SiiDrvCraSetBit8(pObj->config->instTxCra, baseAddr | REG_ADDR__SCDC_INTR0, BIT_MSK__SCDC_INTR0__REG_SCDC_INTR0_STAT2); // Clear DDC Conflict Interrupt
		SiiDrvCraClrBit8(pObj->config->instTxCra, baseAddr | REG_ADDR__SCDC_CTL, BIT_MSK__SCDC_CTL__REG_SCDC_ACCESS);     // Clear SCDC DDC selection
		SiiDrvCraSetBit8(pObj->config->instTxCra, baseAddr | REG_ADDR__SCDC_CTL, BIT_MSK__SCDC_CTL__REG_SCDC_ACCESS);     // Select SCDC DDC cycle
	}while( (SiiDrvCraRdReg8(pObj->config->instTxCra, baseAddr | REG_ADDR__SCDC_INTR0) & BIT_MSK__SCDC_INTR0__REG_SCDC_INTR0_STAT2) );   // Check for DDC conflict interrupt

	SiiDrvCraSetBit8(pObj->config->instTxCra, baseAddr | REG_ADDR__TPI_DDC_MASTER_EN, BIT_MSK__TPI_DDC_MASTER_EN__REG_HW_DDC_MASTER);
	SiiDrvCraWrReg8(pObj->config->instTxCra, baseAddr | REG_ADDR__DDC_ADDR, 0xA8);
	SiiDrvCraWrReg8(pObj->config->instTxCra, baseAddr | REG_ADDR__DDC_SEGM, 0);
	SiiDrvCraWrReg8(pObj->config->instTxCra, baseAddr | REG_ADDR__DDC_OFFSET, addr);
	SiiDrvCraWrReg8(pObj->config->instTxCra, baseAddr | REG_ADDR__DDC_DIN_CNT2, 0);
	SiiDrvCraWrReg8(pObj->config->instTxCra, baseAddr | REG_ADDR__DDC_DIN_CNT1, 1);
	SiiDrvCraWrReg8(pObj->config->instTxCra, baseAddr | REG_ADDR__DDC_CMD, BIT_ENUM__DDC_CMD__CLEAR_FIFO | 0x30); 
	SiiDrvCraWrReg8(pObj->config->instTxCra, baseAddr | REG_ADDR__DDC_DATA, val);
	SiiDrvCraWrReg8(pObj->config->instTxCra, baseAddr | REG_ADDR__DDC_CMD, BIT_ENUM__DDC_CMD__SEQUENTIAL_WRITE | 0x30);  

	// Disable DDC Master
	SiiDrvCraWrReg8(pObj->config->instTxCra, baseAddr |   REG_ADDR__DDC_CMD, BIT_ENUM__DDC_CMD__ABORT_TRANSACTION | 0x30);
	SiiDrvCraClrBit8(pObj->config->instTxCra, baseAddr | REG_ADDR__TPI_DDC_MASTER_EN, BIT_MSK__TPI_DDC_MASTER_EN__REG_HW_DDC_MASTER);        
	SiiDrvCraClrBit8(pObj->config->instTxCra, baseAddr | REG_ADDR__DDC_CMD, 0x7F); 	
#ifndef HDMI_BUILD_IN_BOOT
    HDMI_MUTEX_UNLOCK(*pObj->config->pstDdcLock);
#endif
}	

//----------------------------------------------------------------------------

static uint8_t sReadScdcReg8(ScdcObj_t* pObj, uint8_t addr)
{
	uint8_t rz;				

	SiiDrvCraAddr_t baseAddr = pObj->config->baseAddr;
#ifndef HDMI_BUILD_IN_BOOT
    HDMI_MUTEX_LOCK(*pObj->config->pstDdcLock);
#endif
    do
	{
		SiiDrvCraSetBit8(pObj->config->instTxCra, baseAddr | REG_ADDR__SCDC_INTR0, BIT_MSK__SCDC_INTR0__REG_SCDC_INTR0_STAT2); // Clear DDC Conflict Interrupt
		SiiDrvCraClrBit8(pObj->config->instTxCra, baseAddr | REG_ADDR__SCDC_CTL, BIT_MSK__SCDC_CTL__REG_SCDC_ACCESS);     // Clear SCDC DDC selection
		SiiDrvCraSetBit8(pObj->config->instTxCra, baseAddr | REG_ADDR__SCDC_CTL, BIT_MSK__SCDC_CTL__REG_SCDC_ACCESS);     // Select SCDC DDC cycle
	}while( (SiiDrvCraRdReg8(pObj->config->instTxCra, baseAddr | REG_ADDR__SCDC_INTR0) & BIT_MSK__SCDC_INTR0__REG_SCDC_INTR0_STAT2) );   // Check for DDC conflict interrupt

	SiiDrvCraSetBit8(pObj->config->instTxCra, baseAddr |  REG_ADDR__TPI_DDC_MASTER_EN, BIT_MSK__TPI_DDC_MASTER_EN__REG_HW_DDC_MASTER);
	SiiDrvCraWrReg8(pObj->config->instTxCra, baseAddr |   REG_ADDR__DDC_ADDR, 0xA8);
	SiiDrvCraWrReg8(pObj->config->instTxCra, baseAddr |   REG_ADDR__DDC_SEGM, 0);
	SiiDrvCraWrReg8(pObj->config->instTxCra, baseAddr |   REG_ADDR__DDC_OFFSET, addr);
	SiiDrvCraWrReg8(pObj->config->instTxCra, baseAddr |   REG_ADDR__DDC_DIN_CNT2, 0);
	SiiDrvCraWrReg8(pObj->config->instTxCra, baseAddr |   REG_ADDR__DDC_DIN_CNT1, 1);
	SiiDrvCraWrReg8(pObj->config->instTxCra, baseAddr |   REG_ADDR__DDC_CMD, BIT_ENUM__DDC_CMD__CLEAR_FIFO | 0x30); 
	SiiDrvCraWrReg8(pObj->config->instTxCra, baseAddr |   REG_ADDR__DDC_CMD, BIT_ENUM__DDC_CMD__SEQUENTIAL_READ | 0x30); 

	SiiLibTimeMilliDelay(2);

//	while( !(SiiDrvCraRdReg8(pObj->config->instTxCra, baseAddr | REG_ADDR__SCDC_INTR0) & BIT_MSK__SCDC_INTR0__REG_SCDC_INTR0_STAT0) );

//	SiiDrvCraSetBit8(pObj->config->instTxCra, baseAddr | REG_ADDR__SCDC_INTR0, BIT_MSK__SCDC_INTR0__REG_SCDC_INTR0_STAT0);

	rz = SiiDrvCraRdReg8(pObj->config->instTxCra, baseAddr | REG_ADDR__DDC_DATA);

	// Disable DDC Master
	SiiDrvCraWrReg8(pObj->config->instTxCra, baseAddr |   REG_ADDR__DDC_CMD, BIT_ENUM__DDC_CMD__ABORT_TRANSACTION | 0x30);
	SiiDrvCraClrBit8(pObj->config->instTxCra, baseAddr | REG_ADDR__TPI_DDC_MASTER_EN, BIT_MSK__TPI_DDC_MASTER_EN__REG_HW_DDC_MASTER);        
	SiiDrvCraClrBit8(pObj->config->instTxCra, baseAddr | REG_ADDR__DDC_CMD, 0x7F);
#ifndef HDMI_BUILD_IN_BOOT
    HDMI_MUTEX_UNLOCK(*pObj->config->pstDdcLock);
#endif
	return rz;
}

//-----------------------------------------------------------------------------
#ifndef HDMI_BUILD_IN_BOOT
static void sScdcHandler(SiiInst_t inst)
{
	ScdcObj_t* pObj = (ScdcObj_t*)SII_LIB_OBJ_PNTR(inst);
	SiiDrvCraAddr_t baseAddr = pObj->config->baseAddr;

	for(;;)
	{	
		if(pObj->sc_state == SCRAMBLE_INITIAL)
		{	
			// disable scrambling in TX
			SiiDrvCraClrBit8(pObj->config->instTxCra, baseAddr | REG_ADDR__SCRCTL, BIT_MSK__SCRCTL__REG_SCR_ON /*| BIT_MSK__SCRCTL__REG_HDMI2_ON*/);
			if(pObj->sinkCaps.vclk_mb > 340)
			{
				if(pObj->sinkCaps.bScdcPresent)
				{
					//enable 1/4 bit clock in the sink by writing 1 into SCDC reg.
					//enable scrambling in the sink by writing 1 into SCDC reg.
					sWriteScdcReg8(pObj, DDC_OFFSET__SCDC_TMDS_CONFIG, BIT_MASK__TMDS_CONFIG__SCRAMBLE_ENABLE | BIT_MASK__TMDS_CONFIG__BIT_CLOCK_RATIO); 
                    //enable scrambling in TX
					SiiDrvCraSetBit8(pObj->config->instTxCra, baseAddr | REG_ADDR__SCRCTL, BIT_MSK__SCRCTL__REG_SCR_ON /*|BIT_MSK__SCRCTL__REG_HDMI2_ON */);
     
					pObj->sc_state = SCRAMBLE_WAIT;
					pObj->sc_wait_expired = 0;

					//set timer for 500MS ( will make sc_wait_expired == 1, when hit )
					SiiLibSeqTimerStart(pObj->timerScrambleExpire, 500, 0);
                    SII_LIB_LOG_DEBUG1(pObj, "TMDS CONFIG\n");
                    SiiLibTimeMilliDelay(2);
					continue;
				}
				else
				{
					// enable scrambling in TX
					SiiDrvCraSetBit8(pObj->config->instTxCra, baseAddr | REG_ADDR__SCRCTL, BIT_MSK__SCRCTL__REG_SCR_ON /*| BIT_MSK__SCRCTL__REG_HDMI2_ON*/);

					pObj->sc_state = SCRAMBLE_ENABLED;
					SII_LIB_LOG_DEBUG1(pObj, "SCRAMBLE ON\n");
					break;
				}
			}
			else
			{
				if(pObj->sinkCaps.bScdcPresent)
				{

					if(pObj->sinkCaps.bLTE340MscsScramble)
					{
						//enable scrambling in the sink by writing 1 into SCDC reg.

						//sWriteScdcReg8(pObj, DDC_OFFSET_TMDS_CONFIG, BIT_MASK__TMDS_CONFIG__SCRAMBLE_ENABLE);
						sWriteScdcReg8(pObj, DDC_OFFSET__SCDC_TMDS_CONFIG, BIT_MASK__TMDS_CONFIG__SCRAMBLE_ENABLE | BIT_MASK__TMDS_CONFIG__BIT_CLOCK_RATIO);
						//enable scrambling in TX
						SiiDrvCraSetBit8(pObj->config->instTxCra, baseAddr | REG_ADDR__SCRCTL, BIT_MSK__SCRCTL__REG_SCR_ON /*| BIT_MSK__SCRCTL__REG_HDMI2_ON*/);

						pObj->sc_state = SCRAMBLE_WAIT;
						pObj->sc_wait_expired = 0;
						//set timer for 500MS ( will make sc_wait_expired == 1 when hit )
						SiiLibSeqTimerStart(pObj->timerScrambleExpire, 500, 0);

						continue;
					}
					else
					{
						// disable scrambling in the sink by writing 0 into SCDC reg.
						sWriteScdcReg8(pObj, DDC_OFFSET__SCDC_TMDS_CONFIG, 0);

						// disable scrambling in TX.
						SiiDrvCraClrBit8(pObj->config->instTxCra, baseAddr | REG_ADDR__SCRCTL, BIT_MSK__SCRCTL__REG_SCR_ON /*| BIT_MSK__SCRCTL__REG_HDMI2_ON*/);

						pObj->sc_state = SCRAMBLE_NONE;
						SII_LIB_LOG_DEBUG1(pObj, "SCRAMBLE NONE\n");
						break;		 
					}
				}
				else
				{	
					// disable scrambling in the sink by writing 0 into SCDC reg.
					sWriteScdcReg8(pObj, DDC_OFFSET__SCDC_TMDS_CONFIG, 0);

					// disable scrambling in TX
					SiiDrvCraClrBit8(pObj->config->instTxCra, baseAddr | REG_ADDR__SCRCTL, BIT_MSK__SCRCTL__REG_SCR_ON /*| BIT_MSK__SCRCTL__REG_HDMI2_ON*/);
					pObj->sc_state = SCRAMBLE_NONE;
					SII_LIB_LOG_DEBUG1(pObj, "SCRAMBLE NONE\n");
					break;
				}
			}
		}

		if(pObj->sc_state == SCRAMBLE_WAIT)
		{
			if(pObj->sc_wait_expired)
			{
				//disable scrambling in the sink by writing 0 into SCDC reg
//				sWriteScdcRegb8(pObj, DDC_OFFSET__SCDC_TMDS_CONFIG, 0);
                SII_LIB_LOG_ERR(pObj, "WAIT SCRAMBLE STATUS EXPIRED\n");
				if(pObj->sinkCaps.vclk_mb <= 340)
				{	
					/* disable scrambling in TX */
					SiiDrvCraClrBit8(pObj->config->instTxCra, baseAddr | REG_ADDR__SCRCTL, BIT_MSK__SCRCTL__REG_SCR_ON /*| BIT_MSK__SCRCTL__REG_HDMI2_ON*/);
					pObj->sc_state = SCRAMBLE_NONE;

					SII_LIB_LOG_ERR(pObj, "SCRAMBLE OFF\n");
				}
				else
				{
					pObj->sc_state = SCRAMBLE_ENABLED;
					SII_LIB_LOG_ERR(pObj, "SCRAMBLE ON\n");
					
					//enable  scrambling in RX & TX
					sWriteScdcReg8(pObj, DDC_OFFSET__SCDC_TMDS_CONFIG, BIT_MASK__TMDS_CONFIG__SCRAMBLE_ENABLE | BIT_MASK__TMDS_CONFIG__BIT_CLOCK_RATIO); 
					SiiDrvCraSetBit8(pObj->config->instTxCra, baseAddr | REG_ADDR__SCRCTL, BIT_MSK__SCRCTL__REG_SCR_ON );
					SII_LIB_LOG_ERR(pObj, " After timeout ,enable RX & TX\n");
				}
                pObj->config->cbFunc(pObj->config->instTx, SII_DRV_TX_EVENT_SCRAMBLE_FAIL);
				break;
			} 
			else
			{
				uint8_t scr_status;
      
				scr_status = sReadScdcReg8(pObj, DDC_OFFSET__SCDC_TMDS_STATUS);
				if( scr_status & BIT_MASK__TMDS_STATUS__SCRAMBLE_STATUS )
                {
					pObj->sc_state = SCRAMBLE_ENABLED;
					SII_LIB_LOG_DEBUG1(pObj, "SCRAMBLE ON :: %02x\n", scr_status);
                    pObj->config->cbFunc(pObj->config->instTx, SII_DRV_TX_EVENT_SCRAMBLE_SUCCESS);
					break;
				}
				else
				{
				 #if 0    
                    if (sReadScdcReg8(pObj, DDC_OFFSET__SCDC_TMDS_CONFIG) != BIT_MASK__TMDS_CONFIG__SCRAMBLE_ENABLE | BIT_MASK__TMDS_CONFIG__BIT_CLOCK_RATIO)
                    {
                        SiiLibTimeMilliDelay(50);  
					    sWriteScdcReg8(pObj, DDC_OFFSET__SCDC_TMDS_CONFIG, BIT_MASK__TMDS_CONFIG__SCRAMBLE_ENABLE | BIT_MASK__TMDS_CONFIG__BIT_CLOCK_RATIO);  
                        SII_LIB_LOG_DEBUG1(pObj, ("TMDS CONFIG :: %02x\n", sReadScdcReg8(pObj, DDC_OFFSET__SCDC_TMDS_CONFIG)));
                    }
                #endif
                    SII_LIB_LOG_DEBUG1(pObj, "SCRAMBLE STATUS :: %02x\n", scr_status);
					//stay in the same state
					//continue processing in 10MS
					SiiLibSeqTimerStart(pObj->timerScramblePoll, 10, 0);
					break;
				}	
			}
		}
		break;
	}	
}
#else
// in case of warning in boot
#if 0
static void sScdcHandler(SiiInst_t inst)
{

	ScdcObj_t* pObj = (ScdcObj_t*)SII_LIB_OBJ_PNTR(inst);
	SiiDrvCraAddr_t baseAddr = pObj->config->baseAddr;

    if(pObj->sc_state == SCRAMBLE_INITIAL)
	{	
		// disable scrambling in TX
		SiiDrvCraClrBit8(pObj->config->instTxCra, baseAddr | REG_ADDR__SCRCTL, BIT_MSK__SCRCTL__REG_SCR_ON /*| BIT_MSK__SCRCTL__REG_HDMI2_ON*/);
		if(pObj->sinkCaps.vclk_mb > 340)
		{
			if(pObj->sinkCaps.bScdcPresent)
			{        
				//enable 1/4 bit clock in the sink by writing 1 into SCDC reg.
				//enable scrambling in the sink by writing 1 into SCDC reg.
				sWriteScdcReg8(pObj, DDC_OFFSET__SCDC_TMDS_CONFIG, BIT_MASK__TMDS_CONFIG__SCRAMBLE_ENABLE | BIT_MASK__TMDS_CONFIG__BIT_CLOCK_RATIO); 
                //enable scrambling in TX
				SiiDrvCraSetBit8(pObj->config->instTxCra, baseAddr | REG_ADDR__SCRCTL, BIT_MSK__SCRCTL__REG_SCR_ON /*|BIT_MSK__SCRCTL__REG_HDMI2_ON */);
 
				pObj->sc_state = SCRAMBLE_WAIT;
				pObj->sc_wait_expired = 0;

				//set timer for 500MS ( will make sc_wait_expired == 1, when hit )
				//SiiLibSeqTimerStart(pObj->timerScrambleExpire, 500, 0);
                SII_LIB_LOG_DEBUG1(pObj, "TMDS CONFIG\n");
                SiiLibTimeMilliDelay(2);
				//continue;
			}
			else
			{
				//enable scrambling in TX
				SiiDrvCraSetBit8(pObj->config->instTxCra, baseAddr | REG_ADDR__SCRCTL, BIT_MSK__SCRCTL__REG_SCR_ON /*| BIT_MSK__SCRCTL__REG_HDMI2_ON*/);

				pObj->sc_state = SCRAMBLE_ENABLED;
				SII_LIB_LOG_DEBUG1(pObj, "SCRAMBLE ON\n");
				//break;
			}
		}
		else
		{
			if(pObj->sinkCaps.bScdcPresent)
			{

				if(pObj->sinkCaps.bLTE340MscsScramble)
				{
					//enable scrambling in the sink by writing 1 into SCDC reg.

					//sWriteScdcReg8(pObj, DDC_OFFSET_TMDS_CONFIG, BIT_MASK__TMDS_CONFIG__SCRAMBLE_ENABLE);
					sWriteScdcReg8(pObj, DDC_OFFSET__SCDC_TMDS_CONFIG, BIT_MASK__TMDS_CONFIG__SCRAMBLE_ENABLE | BIT_MASK__TMDS_CONFIG__BIT_CLOCK_RATIO);
					//enable scrambling in TX
					SiiDrvCraSetBit8(pObj->config->instTxCra, baseAddr | REG_ADDR__SCRCTL, BIT_MSK__SCRCTL__REG_SCR_ON /*| BIT_MSK__SCRCTL__REG_HDMI2_ON*/);

					pObj->sc_state = SCRAMBLE_WAIT;
					pObj->sc_wait_expired = 0;
					//set timer for 500MS ( will make sc_wait_expired == 1 when hit )
					//SiiLibSeqTimerStart(pObj->timerScrambleExpire, 500, 0);

					//continue;
				}
				else
				{
					//disable scrambling in the sink by writing 0 into SCDC reg.
					sWriteScdcReg8(pObj, DDC_OFFSET__SCDC_TMDS_CONFIG, 0);

					//disable scrambling in TX.
					SiiDrvCraClrBit8(pObj->config->instTxCra, baseAddr | REG_ADDR__SCRCTL, BIT_MSK__SCRCTL__REG_SCR_ON /*| BIT_MSK__SCRCTL__REG_HDMI2_ON*/);

					pObj->sc_state = SCRAMBLE_NONE;
					SII_LIB_LOG_DEBUG1(pObj, "SCRAMBLE NONE\n");
					//break;		 
				}
			}
			else
			{	
				//disable scrambling in the sink by writing 0 into SCDC reg.
				sWriteScdcReg8(pObj, DDC_OFFSET__SCDC_TMDS_CONFIG, 0);

				//disable scrambling in TX
				SiiDrvCraClrBit8(pObj->config->instTxCra, baseAddr | REG_ADDR__SCRCTL, BIT_MSK__SCRCTL__REG_SCR_ON /*| BIT_MSK__SCRCTL__REG_HDMI2_ON*/);
				pObj->sc_state = SCRAMBLE_NONE;
				SII_LIB_LOG_DEBUG1(pObj, "SCRAMBLE NONE\n");
				//break;
			}
		}
	}

}
#endif
#endif
//-----------------------------------------------------------------------------
#ifndef HDMI_BUILD_IN_BOOT
static void sScrambleTimeout(SiiInst_t inst)
{
    ScdcObj_t* pObj = (ScdcObj_t*)SII_LIB_OBJ_PNTR(inst);
	pObj->sc_wait_expired = true;
}	
#endif

void SiiDrvTxScdcReadRequestTest(SiiInst_t inst, SiiLibTimeMilli_t msDelay)
{
    uint8_t updateFlag0;
    ScdcObj_t* pObj = (ScdcObj_t*)SII_LIB_OBJ_PNTR(inst);
    SiiDrvCraAddr_t baseAddr = pObj->config->baseAddr;
//    uint8_t scdcInt;

    SiiDrvCraSetBit8(pObj->config->instTxCra, baseAddr | REG_ADDR__SCDC_INTR0, BIT_MSK__SCDC_INTR0__REG_SCDC_INTR0_STAT3); // Clear Slave Read Requrest Interrupt

    sWriteScdcReg8(pObj, DDC_OFFSET__SCDC_TEST_CONFIG_0, (BIT_MASK__TEST_CONFIG_0__TEST_READ_REQUEST | (uint8_t)msDelay));

    // delay for 5 msec.
    SiiLibTimeMilliDelay(msDelay+2);
    /*
    do
    {
        scdcInt = SiiDrvCraRdReg8(pObj->config->instTxCra, baseAddr | REG_ADDR__SCDC_INTR0);
    } while( !(scdcInt & BIT_MSK__SCDC_INTR0__REG_SCDC_INTR0_STAT3) );
    
    SiiDrvCraSetBit8(pObj->config->instTxCra, baseAddr | REG_ADDR__SCDC_INTR0, BIT_MSK__SCDC_INTR0__REG_SCDC_INTR0_STAT3); // Clear Slave Read Requrest Interrupt
    
    do
    {
         updateFlag0 = sReadScdcReg8(pObj, DDC_OFFSET__SCDC_UPDATE_0);
    } while( !(updateFlag0 & BIT_MASK__UPDATE_0__RR_TEST) );
*/
    updateFlag0 = sReadScdcReg8(pObj, DDC_OFFSET__SCDC_UPDATE_0);
        
    if(updateFlag0 & BIT_MASK__UPDATE_0__RR_TEST)
    {
        sWriteScdcReg8(pObj, DDC_OFFSET__SCDC_UPDATE_0, BIT_MASK__UPDATE_0__RR_TEST);

        SII_LIB_LOG_DEBUG1(pObj, "******Read Request Test PASSED******\n");

        //updateFlag0 = sReadScdcReg8(pObj, DDC_OFFSET__SCDC_UPDATE_0);
        //SII_LIB_LOG_DEBUG1(pObj, ("\nJust for update flag write confirmation %02x\n", updateFlag0));
    }
    else
    {
        SII_LIB_LOG_DEBUG1(pObj, "******Read Request Test FAILED******\n");
    }

    sWriteScdcReg8(pObj, DDC_OFFSET__SCDC_TEST_CONFIG_0, CLEAR_BITS);
}

uint8_t SiiDrvTxScdcSinkVersionGet(SiiInst_t inst)
{
    uint8_t sinkVer;
    ScdcObj_t* pObj = (ScdcObj_t*)SII_LIB_OBJ_PNTR(inst);
//    SiiDrvCraAddr_t baseAddr = pObj->config->baseAddr;

    sinkVer = sReadScdcReg8(pObj, DDC_OFFSET__SCDC_SINK_VERSION);

    return (sinkVer);

}

void SiiDrvTxScdcSourceVersionSet(SiiInst_t inst, uint8_t ver)
{

    ScdcObj_t* pObj = (ScdcObj_t*)SII_LIB_OBJ_PNTR(inst);
//    SiiDrvCraAddr_t baseAddr = pObj->config->baseAddr;

    sWriteScdcReg8(pObj, DDC_OFFSET__SCDC_SOURCE_VERSION, 1);
}

void SiiDrvTxScdcReadRequestEnable(SiiInst_t inst, uint8_t enable)
{

    ScdcObj_t* pObj = (ScdcObj_t*)SII_LIB_OBJ_PNTR(inst);
  //  SiiDrvCraAddr_t baseAddr = pObj->config->baseAddr;

    sWriteScdcReg8(pObj, DDC_OFFSET__SCDC_CONGIG_0, enable?0x01:0x00);
}

void SiiDrvTxScdcStatusGet(SiiInst_t inst, SiiScdcStatus_t* scdcStatus)
{
    ScdcObj_t* pObj = (ScdcObj_t*)SII_LIB_OBJ_PNTR(inst);
    SiiDrvCraAddr_t baseAddr = pObj->config->baseAddr;
    uint8_t regRead=0;
    //SII_LIB_LOG_DEBUG2("SCDC REG: 0x20 : TMDS_CONFIG : %2x :: TMDS_Bit_Clk_Rat = %d, Scramb_Enable = %d\n", regRead, (regRead & 0x02), (regRead & 0x01));
	scdcStatus->bSourceScrambleOn = (SiiDrvCraRdReg8(pObj->config->instTxCra, baseAddr | REG_ADDR__SCRCTL) & BIT_MSK__SCRCTL__REG_SCR_ON) ? true : false;
    regRead = sReadScdcReg8(pObj, DDC_OFFSET__SCDC_TMDS_CONFIG);
    //SII_LIB_LOG_DEBUG2("SCDC REG: 0x20 : TMDS_CONFIG : %2x :: TMDS_Bit_Clk_Rat = %d, Scramb_Enable = %d\n", regRead, (regRead & 0x02), (regRead & 0x01));
    //scdcStatus->bSourceScrambleOn = (regRead & 0x01) ? true : false;
    scdcStatus->tmdsBitClkRatio   = (regRead & 0x02) ? 40 : 10;
    regRead = sReadScdcReg8(pObj, DDC_OFFSET__SCDC_TMDS_STATUS);
    //SII_LIB_LOG_DEBUG2("SCDC REG: 0x21 : SCRAMBLER_STATUS : %2x:: Scrambler_Status = %d\n", regRead, (regRead & 0x01));
    scdcStatus->bSinkScrambleOn   = (regRead & 0x01) ? true : false;
}
void SiiDrvTxScdcStatusSet(SiiInst_t inst, SiiScdcStatus_t* scdcStatus)
{
    ScdcObj_t* pObj = (ScdcObj_t*)SII_LIB_OBJ_PNTR(inst);
    SiiDrvCraAddr_t baseAddr = pObj->config->baseAddr;
    uint8_t regwrite=0;
    
    regwrite |= scdcStatus->bSinkScrambleOn ? BIT_MASK__TMDS_CONFIG__SCRAMBLE_ENABLE : 0 ;
    regwrite |= scdcStatus->tmdsBitClkRatio>=40 ? BIT_MASK__TMDS_CONFIG__BIT_CLOCK_RATIO : 0 ;
    sWriteScdcReg8(pObj, DDC_OFFSET__SCDC_TMDS_CONFIG, regwrite);

	if (scdcStatus->bSourceScrambleOn)
        SiiDrvCraSetBit8(pObj->config->instTxCra, baseAddr | REG_ADDR__SCRCTL, BIT_MSK__SCRCTL__REG_SCR_ON /*| BIT_MSK__SCRCTL__REG_HDMI2_ON*/);
    else
        SiiDrvCraClrBit8(pObj->config->instTxCra, baseAddr | REG_ADDR__SCRCTL, BIT_MSK__SCRCTL__REG_SCR_ON /*| BIT_MSK__SCRCTL__REG_HDMI2_ON*/);

    return ;
}

static void SiiDrvTxForcePhyOutputEnable(SiiInst_t inst, bool_t enable)
{
    ScdcObj_t* pObj = (ScdcObj_t*)SII_LIB_OBJ_PNTR(inst);
    SiiDrvCraAddr_t baseAddr = pObj->config->baseAddr;

    if(enable)
    {
        SiiDrvCraSetBit8(pObj->config->instTxCra, baseAddr | TXPHY_TMDS_CTL, BIT_MSK__TXPHY_TMDS_CTL_REG_TMDS_OE);
        SiiLibTimeMilliDelay(1);
        SiiDrvCraSetBit8(pObj->config->instTxCra, baseAddr | TXPHY_DP_CTL0, BIT_MSK__TXPHY_DP_CTL0__REG_DP_OE | BIT_MSK__TXPHY_DP_CTL0__REG_TX_OE);
    }
    else
    {
        SiiDrvCraClrBit8(pObj->config->instTxCra, baseAddr | TXPHY_TMDS_CTL, BIT_MSK__TXPHY_TMDS_CTL_REG_TMDS_OE);
        SiiDrvCraClrBit8(pObj->config->instTxCra, baseAddr | TXPHY_DP_CTL0, BIT_MSK__TXPHY_DP_CTL0__REG_DP_OE | BIT_MSK__TXPHY_DP_CTL0__REG_TX_OE);
    }

#if 0    
    uint32_t *pu32VirtualAddr_TMDS_CTL = NULL;
    uint32_t *pu32VirtualAddr_DP_CTL0 = NULL;
    ScdcObj_t* pObj = (ScdcObj_t*)SII_LIB_OBJ_PNTR(inst);

    if(pObj->config->pu32VirtualBaseAddr == NULL)
    {
        SII_LIB_LOG_ERR(pObj, "pu32VirtualBaseAddr is null ! \n");
        return ;
    }

    pu32VirtualAddr_DP_CTL0 = (uint32_t *)((uint32_t)(pObj->config->pu32VirtualBaseAddr) | 0x1EC8);
    pu32VirtualAddr_TMDS_CTL = (uint32_t *)((uint32_t)(pObj->config->pu32VirtualBaseAddr) | 0x1FE0);

    if(enable)
    {
        * (volatile HI_U32 *)(pu32VirtualAddr_DP_CTL0) = 0xa7; 
        SiiLibTimeMilliDelay(1);
        * (volatile HI_U32 *)(pu32VirtualAddr_TMDS_CTL) = 0x10;    // oe enable
    }
    else
    {
        * (volatile HI_U32 *)(pu32VirtualAddr_DP_CTL0) = 0;   
        * (volatile HI_U32 *)(pu32VirtualAddr_TMDS_CTL) = 0;       // oe disable
    }
#endif
}

void SiiDrvTxScdcSinkCapsSet(SiiInst_t inst, SiiModTxScdcSinKCaps_t* sinkCaps)
{
    SiiScdcStatus_t     stScdcStatus;
    bit_fld_t           u8ScdcSetTimes = 10;
    ScdcObj_t* pObj = (ScdcObj_t*)SII_LIB_OBJ_PNTR(inst);
   
//    SiiDrvCraAddr_t baseAddr = pObj->config->baseAddr;
    //uint8_t sinkVer = 0;

    pObj->sinkCaps = *sinkCaps;

    if(pObj->sinkCaps.bScdcPresent)
    {
        //sScdcRegReadTest(inst);

        //sinkVer = SiiDrvTxScdcSinkVersionGet(inst);

        SiiDrvTxScdcSourceVersionSet(inst, pObj->config->srcVersion);

        if(pObj->sinkCaps.bReadReqCapable)
        {
            if(pObj->config->bReadReq)
            {
                SiiDrvTxScdcReadRequestEnable(inst, pObj->config->bReadReq);

                SiiLibTimeMilliDelay(1);

                 //sScdcRegReadTest(inst);

                SiiDrvTxScdcReadRequestTest(inst, 5);

                 //sScdcRegReadTest(inst);
            }
        }
    }
#if 0
    SiiLibSeqTimerStart(pObj->timerScramblePoll, 10, 0);
#else
    SiiLibTimeMilliDelay(20);
    pObj->sc_state = SCRAMBLE_INITIAL;
    pObj->sc_wait_expired = false;

//    sScdcHandler(inst);   
    do {
        SiiDrvTxForcePhyOutputEnable(inst, HI_FALSE);
        
        SiiLibTimeMilliDelay(1 + (10 - u8ScdcSetTimes)* 10);
        stScdcStatus.bSinkScrambleOn = HI_TRUE;
        stScdcStatus.bSourceScrambleOn = HI_TRUE;
        stScdcStatus.tmdsBitClkRatio = 40;
        SiiDrvTxScdcStatusSet(inst, &stScdcStatus);

        SiiDrvTxForcePhyOutputEnable(inst, HI_TRUE);

        SiiLibTimeMilliDelay(1 + (10 - u8ScdcSetTimes) * 20);
        SII_LIB_LOG_DEBUG1(pObj, "scdc set u8ScdcSetTimes: %d \n", u8ScdcSetTimes);
        SiiDrvTxScdcStatusGet(inst, &stScdcStatus);
        u8ScdcSetTimes--;

    }while(u8ScdcSetTimes && (stScdcStatus.tmdsBitClkRatio != 40));
    
    if((stScdcStatus.tmdsBitClkRatio != 40) || (!u8ScdcSetTimes))
    {
        SII_LIB_LOG_ERR(pObj, "scdc set failed, u8ScdcSetTimes: %d \n", u8ScdcSetTimes);
    }
    else
    {
        SII_LIB_LOG_DEBUG1(pObj, "sink scramble success \n");
    }
    
    SiiLibTimeMilliDelay(2);

    SiiDrvTxForcePhyOutputEnable(inst, HI_TRUE);
    
#endif
    //SiiLibSeqTimerStart(pObj->timerRrIntPoll, 200, 500);

}



/*20150511: not used */
#if 0  
static void sScdcRegReadTest(SiiInst_t inst)
{
    ScdcObj_t* pObj = (ScdcObj_t*)SII_LIB_OBJ_PNTR(inst);
//    SiiDrvCraAddr_t baseAddr = pObj->config->baseAddr;
    uint8_t regRead=0;
    
    regRead = sReadScdcReg8(pObj, DDC_OFFSET__SCDC_SINK_VERSION);
    SII_LIB_LOG_DEBUG2("SCDC REG: 0x01 : DDC_OFFSET__SCDC_SINK_VERSION : %2x\n", regRead);

    regRead = sReadScdcReg8(pObj, DDC_OFFSET__SCDC_SOURCE_VERSION);
    SII_LIB_LOG_DEBUG2("SCDC REG: 0x02 : DDC_OFFSET__SCDC_SOURCE_VERSION : %2x\n", regRead);

    regRead = sReadScdcReg8(pObj, DDC_OFFSET__SCDC_UPDATE_0);
    SII_LIB_LOG_DEBUG2("SCDC REG: 0x10 : UPDATE_0: %2x :: RR_TEST = %d, CED_Update = %d, Status_update = %d\n", regRead, (regRead & 0x04), (regRead & 0x02), (regRead & 0x01));

    regRead = sReadScdcReg8(pObj, DDC_OFFSET__SCDC_UPDATE_1);
    SII_LIB_LOG_DEBUG2("SCDC REG: 0x11 : DDC_OFFSET__SCDC_UPDATE_1 : %2x\n", regRead);

    regRead = sReadScdcReg8(pObj, DDC_OFFSET__SCDC_TMDS_CONFIG);
    SII_LIB_LOG_DEBUG2("SCDC REG: 0x20 : TMDS_CONFIG : %2x :: TMDS_Bit_Clk_Rat = %d, Scramb_Enable = %d\n", regRead, (regRead & 0x02), (regRead & 0x01));

    regRead = sReadScdcReg8(pObj, DDC_OFFSET__SCDC_TMDS_STATUS);
    SII_LIB_LOG_DEBUG2("SCDC REG: 0x21 : SCRAMBLER_STATUS : %2x:: Scrambler_Status = %d\n", regRead, (regRead & 0x01));

    regRead = sReadScdcReg8(pObj, DDC_OFFSET__SCDC_CONGIG_0);
    SII_LIB_LOG_DEBUG2("SCDC REG: 0x30 : SCDC_CONGIG_0 : %2x:: RR_Enable =  %d\n", regRead, (regRead & 0x01));

    regRead = sReadScdcReg8(pObj, DDC_OFFSET__SCDC_STATUS_FLAGS_0);
    SII_LIB_LOG_DEBUG2("SCDC REG: 0x40 : STATUS_FLAGS_0 : %2x:: Ch2_Lock = %d, Ch1_Lock = %d, Ch0_Lock = %d, Clock_Det = %d\n", regRead, (regRead & 0x08), (regRead & 0x04), (regRead & 0x02), (regRead & 0x01));

    regRead = sReadScdcReg8(pObj, DDC_OFFSET__SCDC_TEST_CONFIG_0);
    SII_LIB_LOG_DEBUG2("SCDC REG: 0x40 : TEST_CONFIG_0 : %2x\n", regRead);

/*
    regRead = sReadScdcReg8(pObj, DDC_OFFSET__SCDC_MANF_OUI_3);
    SII_LIB_LOG_DEBUG2(("SCDC REG: 0xD0 : DDC_OFFSET__SCDC_MANF_OUI_3 : %2x\n", regRead));

    regRead = sReadScdcReg8(pObj, DDC_OFFSET__SCDC_MANF_OUI_2);
    SII_LIB_LOG_DEBUG2(("SCDC REG: 0xD1 : DDC_OFFSET__SCDC_MANF_OUI_2 : %2x\n", regRead));

    regRead = sReadScdcReg8(pObj, DDC_OFFSET__SCDC_MANF_OUI_1);
    SII_LIB_LOG_DEBUG2(("SCDC REG: 0xD2 : DDC_OFFSET__SCDC_MANF_OUI_1 : %2x\n", regRead));

    regRead = sReadScdcReg8(pObj, DDC_OFFSET__SCDC_MANF_DEV_ID_STR);
    SII_LIB_LOG_DEBUG2(("SCDC REG: 0xD3 - 0xDA : DDC_OFFSET__SCDC_MANF_DEV_ID_STR : %2x\n", regRead));

    regRead = sReadScdcReg8(pObj, DDC_OFFSET__SCDC_MANF_DEV_ID_HW_REV);
    SII_LIB_LOG_DEBUG2(("SCDC REG: 0xDB : DDC_OFFSET__SCDC_MANF_DEV_ID_HW_REV : %2x\n", regRead));
    
    regRead = sReadScdcReg8(pObj, DDC_OFFSET__SCDC_MANF_DEV_ID_SW_MAJOR_REV);
    SII_LIB_LOG_DEBUG2(("SCDC REG: 0xDC : DDC_OFFSET__SCDC_MANF_DEV_ID_SW_MAJOR_REV : %2x\n", regRead));

    regRead = sReadScdcReg8(pObj, DDC_OFFSET__SCDC_MANF_DEV_ID_SW_MINOR_REV);
    SII_LIB_LOG_DEBUG2(("SCDC REG: 0xDD : DDC_OFFSET__SCDC_MANF_DEV_ID_SW_MINOR_REV : %2x\n", regRead));
    */
}
#endif
#endif
/*
void sScdcInterruptHandler(SiiInst_t inst)
{
    ScdcObj_t* pObj = (ScdcObj_t*)SII_LIB_OBJ_PNTR(inst);
    SiiDrvCraAddr_t baseAddr = pObj->config->baseAddr;
    uint8_t scdcInt = 0;

    scdcInt = SiiDrvCraRdReg8(pObj->config->instTxCra, baseAddr | REG_ADDR__SCDC_INTR0);
    
    if(scdcInt & BIT_MSK__SCDC_INTR0__REG_SCDC_INTR0_STAT3)
    {
        SiiDrvCraSetBit8(pObj->config->instTxCra, baseAddr | REG_ADDR__SCDC_INTR0, BIT_MSK__SCDC_INTR0__REG_SCDC_INTR0_STAT3); // Clear Slave Read Requrest Interrupt

        pObj->scdcUpdate0 = sReadScdcReg8(pObj, DDC_OFFSET__SCDC_UPDATE_0);
        pObj->scdcUpdate1 = sReadScdcReg8(pObj, DDC_OFFSET__SCDC_UPDATE_1);

        sWriteScdcReg8(pObj, DDC_OFFSET__SCDC_UPDATE_0, pObj->scdcUpdate0);
        sWriteScdcReg8(pObj, DDC_OFFSET__SCDC_UPDATE_1, pObj->scdcUpdate1);
    }
}
*/
/***** end of file ***********************************************************/
