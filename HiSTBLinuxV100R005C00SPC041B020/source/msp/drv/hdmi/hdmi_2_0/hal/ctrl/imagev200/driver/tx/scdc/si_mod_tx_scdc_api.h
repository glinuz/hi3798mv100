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
 * file si_mod_tx_scdc_api.h
 *
 * brief HDCP - Tx SCDC API
 *
 *****************************************************************************/
#ifndef __SI_DRV_TX_SCDC_API_H__
#define __SI_DRV_TX_SCDC_API_H__

/***** #include statements ***************************************************/
#ifdef HDMI_SCDC_SUPPORT
#include "si_datatypes.h"
#include "si_drv_tx_api.h"
#include "si_lib_time_api.h"

/***** public macro definitions **********************************************/
#define SII_DRV_TX_SCDC_EVENT__CHAR_ERR_DET     0x00000001 // SCDC status changed
#define SII_DRV_TX_SCDC_EVENT__TEST_RR_ACK      0x00000002 // Returning response from sink upon 'test read request'

#define SCDC_ID                                                 0xA8

#define DDC_OFFSET__SCDC_SINK_VERSION                           0x01

#define DDC_OFFSET__SCDC_SOURCE_VERSION                         0x02

#define DDC_OFFSET__SCDC_UPDATE_0                               0x10
#define BIT_MASK__UPDATE_0__STATUS_UPDATE                       0x01
#define BIT_MASK__UPDATE_0__CED_UPDATE                          0x02
#define BIT_MASK__UPDATE_0__RR_TEST                             0x04

#define DDC_OFFSET__SCDC_UPDATE_1                               0x11

#define DDC_OFFSET__SCDC_TMDS_CONFIG                            0x20
#define BIT_MASK__TMDS_CONFIG__SCRAMBLE_ENABLE                  0x01
#define BIT_MASK__TMDS_CONFIG__BIT_CLOCK_RATIO                  0x02

#define DDC_OFFSET__SCDC_TMDS_STATUS                            0x21
#define BIT_MASK__TMDS_STATUS__SCRAMBLE_STATUS                  0x01

#define DDC_OFFSET__SCDC_CONGIG_0                               0x30
#define BIT_MASK__CONGIG_0__RR_ENABLE                           0x01

#define DDC_OFFSET__SCDC_STATUS_FLAGS_0                         0x40
#define BIT_MASK__STATUS_FLAGS__CLK_DETECTED                    0x01
#define BIT_MASK__STATUS_FLAGS__CH0_LOCKED                      0x02
#define BIT_MASK__STATUS_FLAGS__CH1_LOCKED                      0x04
#define BIT_MASK__STATUS_FLAGS__CH2_LOCKED                      0x08

#define DDC_OFFSET__SCDC_TEST_CONFIG_0                          0xC0
#define BIT_MASK__TEST_CONFIG_0__TEST_READ_REQUEST              0x80
#define BIT_MASK__TEST_CONFIG_0__TRR_DELAY_MS                   0x7F

#define DDC_OFFSET__SCDC_MANF_OUI_3                             0xD0
#define DDC_OFFSET__SCDC_MANF_OUI_2                             0xD1
#define DDC_OFFSET__SCDC_MANF_OUI_1                             0xD2

#define DDC_OFFSET__SCDC_MANF_DEV_ID_STR                        0xD3

#define DDC_OFFSET__SCDC_MANF_DEV_ID_HW_REV                        0xDB
#define BIT_MASK__MANF_DEV_ID_HW_REV__MAJOR                        0xF0
#define BIT_MASK__MANF_DEV_ID_HW_REV__MINOR                        0x0F

#define DDC_OFFSET__SCDC_MANF_DEV_ID_SW_MAJOR_REV                  0xDC

#define DDC_OFFSET__SCDC_MANF_DEV_ID_SW_MINOR_REV                  0xDD

/***** public type definitions ***********************************************/

typedef uint32_t SiiDrvTxScdcEvent_t;

typedef struct
{
	SiiDrvCraAddr_t      baseAddr;       //!< Base Addrress of SCDC-Tx register space
    bool_t               bReadReq;       //!< If true then use read-request if Sink support read request as well.
    uint8_t              srcVersion;     //!< Source version
	SiiInst_t			 instTxCra;
    SiiInst_t			 instTx;		//!< Tx Controller's instance.
    HdmiTxEventNotifyCallBack	cbFunc;
#ifndef HDMI_BUILD_IN_BOOT
    HDMI_MUTEX           *pstDdcLock;
#endif
    uint32_t             *pu32VirtualBaseAddr;
} SiiModTxScdcConfig_t;



/***** call-back functions ***************************************************/

/*****************************************************************************/
/**
* @brief SCDC-Tx Driver notification call-back function
* @note  This is function is called by SCDC driver only and must be externally provided
*
* @param[in]  inst        Handle to instance
* @param[in]  eventFlags  Notification flags
*
*****************************************************************************/
void SiiModTxScdcCallBack(SiiInst_t instTx, SiiDrvTxScdcEvent_t eventFlags);

/*****************************************************************************/
/**
* @brief Tx SCDC driver constructor
*
* @param[in]  pNameStr   Name of instance
* @param[in]  parentInst Handle to parent instance
* @param[in]  pConfig    Static configuration parameters
*
* @retval                Handle to instance
*
*****************************************************************************/
SiiInst_t SiiModTxScdcCreate(char *pNameStr, SiiInst_t parentInst, SiiModTxScdcConfig_t *pConfig);

/*****************************************************************************/
/**
* @brief Tx SCDC driver destructor
*
* @param[in]  inst       Handle to instance
*
* @retval                None
*
*****************************************************************************/
void SiiModTxScdcDelete(SiiInst_t inst);

/*****************************************************************************/
/**
* @brief Tx SCDC Reset
*
* @param[in]  inst       Handle to instance
*
* @retval                None
*
*****************************************************************************/
void SiiModTxScdcReset(SiiInst_t inst);

/*****************************************************************************/
/**
* @brief Set Sink capabilities to SCDC module
*
* @param[in]  inst       Handle to instance
* @param[in]  sinkCaps   Pointer to sink capabilities strucure
*
* @retval                None
*
*****************************************************************************/
void SiiDrvTxScdcSinkCapsSet(SiiInst_t inst, SiiModTxScdcSinKCaps_t* sinkCaps);

/*****************************************************************************/
/**
* @brief Get the version of connected Sink from SCDCS
*
* @param[in]  inst       Handle to instance
*
* @retval     uint8_t    Sink Version
*
*****************************************************************************/
uint8_t SiiDrvTxScdcSinkVersionGet(SiiInst_t inst);

/*****************************************************************************/
/**
* @brief Set the version of the Source to connected Sink's SCDCS
*
* @param[in]  inst       Handle to instance
* @param[in]  ver        Source Version
*
* @retval                None
*
*****************************************************************************/
void SiiDrvTxScdcSourceVersionSet(SiiInst_t inst, uint8_t ver);

/*****************************************************************************/
/**
* @brief Test Sink's Read Requrest feature
*
* @param[in]  inst       Handle to instance
* @param[in]  SiiLibTimeMilli_t Delay in millisec after which Sink 
*                               should respond with Read Request.
*
* @retval                None
*
*****************************************************************************/
void SiiDrvTxScdcReadRequestTest(SiiInst_t inst, SiiLibTimeMilli_t msDelay);

/*****************************************************************************/
/**
* @brief Get the status of SCDCS
*
* @param[in]  inst       Handle to instance
*
* @retval     SiiScdcStatus_t    scdc status
*
*****************************************************************************/
void SiiDrvTxScdcStatusGet(SiiInst_t inst, SiiScdcStatus_t* scdcStatus);

/*****************************************************************************/
/**
* @brief Set the status of SCDCS
*
* @param[in]  inst       Handle to instance
*
* @param[in]     SiiScdcStatus_t    scdc status
*
* @retval  none
*****************************************************************************/
void SiiDrvTxScdcStatusSet(SiiInst_t inst, SiiScdcStatus_t* scdcStatus);
//void SiiDrvTxScdcReadReqEnable(SiiInst_t inst, uint8_t enable);
//bool_t SiiDrvTxScdcReadRequestEnabledGet(SiiInst_t inst);
//void SiiDrvTxScdcTmdsBitClockRatioSet(SiiInst_t inst, SiiDrvScdcTmdsBitClockRatio_t ratio);
//void SiiDrvTxScdcCharErrorGet(SiiInst_t inst, uint8_t channel);
//void SiiDrvTxScdcManufacturerSpecificDataGet(SiiInst_t inst, SiiDrvScdcMsd_t* pMsd);
#endif
#endif // __SI_DRV_TX_SCDC_API_H__

/***** end of file ***********************************************************/
