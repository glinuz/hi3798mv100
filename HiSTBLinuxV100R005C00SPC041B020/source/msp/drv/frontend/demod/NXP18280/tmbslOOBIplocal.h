/**
  Copyright (C) 2008 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmbslOOBIp_local.h
 *                %version: CFR_FEAP#5 %
 *
 * \date          %date_modified%
 *
 * \brief         Describe briefly the purpose of this file.
 *
 * REFERENCE DOCUMENTS :
 *
 * Detailled description may be added here.
 *
 * \section info Change Information
 *
 * \par Changelog
 *
 * \par Version_Definition
 *  VERSION_TAG:OOBIP_BSL_COMP_NUM.OOBIP_BSL_MAJOR_VER.OOBIP_BSL_MINOR_VER
 *
*/

#ifndef _tmbslOOBIp_LOCAL_H //-----------------
#define _tmbslOOBIp_LOCAL_H

/*============================================================================*/
/*                       INCLUDE FILES                                        */
/*============================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*/
/*                       MACRO DEFINITION                                     */
/*============================================================================*/
#define OOBIP_BSL_COMP_NUM       2
#define OOBIP_BSL_MAJOR_VER      0
#define OOBIP_BSL_MINOR_VER      15

#define OOBIP_MUTEX_TIMEOUT          TMBSL_FRONTEND_MUTEX_TIMEOUT_INFINITE


#define POBJ_SRVFUNC_SIO pObj->sRWFunc
#define POBJ_SRVFUNC_STIME pObj->sTime
#define P_DBGPRINTEx pObj->sDebug.Print
#define P_DBGPRINTVALID ((pObj != Null) && (pObj->sDebug.Print != Null))


#define OOBIP_AAGC_MIN 0x80 /* this value is signed */
#define OOBIP_AAGC_MAX 0x7F /* this value is signed */
#define OOBIP_DAGC_MIN 0x00
#define OOBIP_DAGC_MAX 0xFF
#define OOBIP_FAGC_MIN 0x00
#define OOBIP_FAGC_MAX 0x3F /* this value is on 6 bits */

/* MER real depth will be (TABLE_SIZE * DEPTH) */
#define OOBIP_MER_TABLE_SIZE 10
#define OOBIP_MER_DEFAULT_DEPTH 100
    
/*---------*/
/* CHANNEL */
/*---------*/
#define OOBIP_MAX_UNITS              4

/*-----------------------------------------------*/
/*  DEFAULT VALUES FOR CONFIGURATION MANAGEMENT  */
/*-----------------------------------------------*/
    
/*---------*/
/*  INDEX  */
/*---------*/
#define OOBIP_MODE_IND              0x00
#define OOBIP_FREQ_MSB_IND          0x01
#define OOBIP_FREQ_CSB_IND          0x02
#define OOBIP_FREQ_LSB_IND          0x03
#define OOBIP_SYM_RATE_MSB_IND      0x04
#define OOBIP_SYM_RATE_CSB_IND      0x05
#define OOBIP_SYM_RATE_LSB_IND      0x06
#define OOBIP_AGC1_IND              0x07
#define OOBIP_AGC2_IND              0x08
#define OOBIP_AGCA_IND              0x09
#define OOBIP_DAGC_CTL_IND          0x0A
#define OOBIP_DAGC_ACC_IND          0x0B
#define OOBIP_MFIR_IND              0x0C
#define OOBIP_FAGC_ACC_IND          0x0D
#define OOBIP_EQU_ADP_IND           0x0E
#define OOBIP_EQU_CTL_IND           0x0F
#define OOBIP_EQU_COEFV_IND         0x10
#define OOBIP_CTL_CTRL_IND          0x11
#define OOBIP_AFC_CTL_IND           0x12
#define OOBIP_CTL_FREQ_IND          0x13
#define OOBIP_CTL_ACC_MSB_IND       0x14
#define OOBIP_CTL_ACC_LSB_IND       0x15
#define OOBIP_BTL_CTL_IND           0x16
#define OOBIP_BTL_ACC_IND           0x17
#define OOBIP_LOCK_IND              0x18
#define OOBIP_LOCK_ACC_IND          0x19
#define OOBIP_RACQ_CTL_IND          0x1A
#define OOBIP_BYP_IND               0x1B
#define OOBIP_DIS_IND               0x1C
#define OOBIP_TRK_CTL_IND           0x1D
#define OOBIP_TST1_IND              0x1E
#define OOBIP_FEC_OCTL_IND          0x1F
#define OOBIP_TST2_IND              0x20
#define OOBIP_BIST_IND              0x21
#define OOBIP_MISC_IND              0x22
#define OOBIP_TST3_IND              0x23
#define OOBIP_GICTL_IND             0x24
#define OOBIP_RFAGC_CTL_IND         0x25
#define OOBIP_RFAGC_CTL2_IND        0x26
#define OOBIP_RFA_MINTH_IND         0x27
#define OOBIP_RFA_MAXTH_IND         0x28
#define OOBIP_TNR_CTL_IND           0x29
#define OOBIP_TNR_CTL2_IND          0x2A
#define OOBIP_TNR_CTL3_IND          0x2B
#define OOBIP_TST4_IND              0x2C
#define OOBIP_ERR_CNT_IND           0x2D
#define OOBIP_FRM_CNT_IND           0x2E
#define OOBIP_STAT1_IND             0x2F
#define OOBIP_ESNO_IND              0x30
#define OOBIP_STAT2_IND             0x31
#define OOBIP_TST5_IND              0x32
#define OOBIP_MSE_CTL_IND           0x33
#define OOBIP_MSE_ACC_MSB_IND       0x34
#define OOBIP_MSE_ACC_LSB_IND       0x35

#define OOBIP_CONSTELLATION_I_IND   0x3D
#define OOBIP_CONSTELLATION_Q_IND   0x3E
#define OOBIP_RD_FRZ_VERSION_IND    0x3F

#define OOBIP_CLK_EN_IND            0x180A
#define OOBIP_CLK_EXIT_RESET_IND    0x180C
#define OOBIP_RESET_CONTROL_IND     0x2000
#define OOBIP_ADC0_CFG_IND          0x2800
#define OOBIP_ADC1_CFG_IND          0x2804
#define OOBIP_TS_COMMON_CFG_IND     0x280C
#define OOBIP_DEMOD1_TS_CFG_IND     0x280E
#define OOBIP_DEMOD0_TS_CFG_IND     0x2810
#define OOBIP_IF_CFG                0x2812
#define OOBIP_PAD_CTRL_IND          0x281C

/*------------------*/
/*  DEFAULT VALUES  */
/*------------------*/

/*----------------*/
/*  DEFINE MASKS  */
/*----------------*/
#define OOBIP_GI_MODE_MSK                       0x80
#define OOBIP_OOB_POD_IFC_MODE_MSK              0x20
#define OOBIP_SPECTRAL_INV_MSK                  0x08
#define OOBIP_BPSK_MODE_MSK                     0x04
#define OOBIP_A2D_CLK_POL_MSK                   0x10
#define OOBIP_OOB_POD_IFC_MODE_MSK              0x20
#define OOBIP_EMO_INIT_MODE_MSK                 0x40
#define OOBIP_AAGC_THD_MSK                      0x3F
#define OOBIP_AAGC_SD_POL_MSK                   0x40
#define OOBIP_AAGC_BW_MSK                       0x02
#define OOBIP_FIR_ALPHA_SEL_MSK                 0xC0
#define OOBIP_LOCK_IND_MSK                      0x80
#define OOBIP_DEMOD_LOCKD_MSK                   0x40
#define OOBIP_AAGC_ACC0_MSK                     0xFF
#define OOBIP_DAGC_ACC0_MSK                     0xFF
#define OOBIP_FAGC_ACC0_MSK                     0x3F
#define OOBIP_AAGC_SAT_MSK                      0x10
#define OOBIP_DAGC_SAT_MSK                      0x08
#define OOBIP_FAGC_SAT_MSK                      0x04
#define OOBIP_CTL_LSB_MSK                       0x0F
#define OOBIP_CTL_MSK                           0x0FFF
#define OOBIP_BTL_ACC0_MSK                      0xFF
#define OOBIP_RF_SD_LEVEL_LSB_MSK               0x03
#define OOBIP_RF_SD_LEVEL_MSK                   0x03FF
#define OOBIP_FRAME_FAILS_MSK                   0xF0
#define OOBIP_FRAME_ERRORS_MSK                  0x0F
#define OOBIP_OOB_RS_ERR_MSK                    0x08
#define OOBIP_OOB_DFAIL_ERR_MSK                 0x04
#define OOBIP_FRAME_NOT_SYNC_MSK                0x02
#define OOBIP_DEMOD_NOT_LOCKED_MSK              0x01
#define OOBIP_FRAME_SYNC_MSK                    0x80
#define OOBIP_TRACK_CNT_MSK                     0x7F
#define OOBIP_CONSTELLATION_MSK                 0x3F
#define OOBIP_MSE_BW_MSK                        0x06
#define OOBIP_MSE_RESET_MSK                     0x01
#define OOBIP_MSE_ACC_MSK                       0xFF

/* IF_CFG MASK */
#define OOBIP_IF_CFG_CLK_DIV_MSK               0x1F


/*---------------*/
/*  DEFINE BITS  */
/*---------------*/
#define OOBIP_OOB_POD_IFC_MODE_BIT              0x20
#define OOBIP_FIR_ALPHA_0_3_SRRC_BIT            0x00
#define OOBIP_FIR_ALPHA_0_35_SRRC_BIT           0x01
#define OOBIP_FIR_ALPHA_0_5_SRRC_BIT            0x02
#define OOBIP_FIR_ALPHA_0_5_RC_BIT              0x03


#define NB_MAX_CHANNEL                  136


/*============================================================================*/
/*                       ENUM OR TYPE DEFINITION                              */
/*============================================================================*/
typedef struct _tmOOBIpconfig_t
{
    UInt32                  uSamplingClock;
    UInt8                   bSpecInv;
    UInt32                  uIF;
    OOBIP_PODIfcMode_t      ePODIfcMode;
    tmFrontEndFECMode_t     eFECMode;
    OOBIP_Polarity_t        eA2DClkPol;
    UInt8                   bEmoInitMode;
    UInt8                   bAAGCThreshold;
    OOBIP_Polarity_t        eAAGCPol;
    UInt8                   bAAGCBw;

} tmOOBIpconfig_t;

typedef struct _tmOOBIpflags_t
{
    unsigned int iInit      :1;
} tmOOBIpflags_t;

typedef struct _tmOOBIpchannel_t
{
    UInt32              uSR;
    tmFrontEndSpecInv_t   eSI;
    tmFrontEndModulation_t eMod;
} tmOOBIpchannel_t;

typedef struct _tmOOBIpObject_t 
{
    tmUnitSelect_t              tUnit;
    OOBIp_HwSampleVersion_t     eHwSample;
    ptmbslFrontEndMutexHandle   pMutex;
    UInt16                      uStartAddress;
    tmOOBIpchannel_t            sCurrentChannel;
    tmOOBIpconfig_t             sConfig;
    tmOOBIpflags_t              sApiFlags;
    tmbslFrontEndIoFunc_t       sRWFunc;
    tmbslFrontEndTimeFunc_t     sTime;
    tmbslFrontEndDebugFunc_t    sDebug;
    tmbslFrontEndMutexFunc_t    sMutex;
    tmPowerState_t              ePowerState;
    /* Uncors management */
    UInt32                      uUncors;
    UInt32                      uCorrected;
    /* MER measurement */
    UInt32                      uIMean;
    UInt32                      uQMean;
    UInt32                      uMERDepth;
    UInt32                      puMERError[OOBIP_MER_TABLE_SIZE];
    UInt8                       uMERElementToUpdate;
} tmOOBIpObject_t;

/*============================================================================*/
/* Internal Prototypes:                                                       */
/*============================================================================*/

/* Local functions */
tmErrorCode_t OOBIpWrite(tmUnitSelect_t tUnit, UInt8 uIndex_U, UInt32 uNBytes_U, UInt8* puData_U);
tmErrorCode_t OOBIpWriteBit(tmUnitSelect_t tUnit, UInt8 uIndex_U, UInt32 uMask_U, UInt8 uData_U);
tmErrorCode_t OOBIpRead(tmUnitSelect_t tUnit, UInt8 uIndex_U, UInt32 uNBytes_U, UInt8* puData_U);
tmErrorCode_t OOBIpWait (tmUnitSelect_t tUnit, UInt32 uTime);

extern tmErrorCode_t OOBIpMutexAcquire(tmOOBIpObject_t *pObj, UInt32 timeOut);
extern tmErrorCode_t OOBIpMutexRelease(tmOOBIpObject_t *pObj);

#ifdef __cplusplus
}
#endif

#endif /* tmbslOOBIp_LOCAL_H */
/*============================================================================*/
/*                            END OF FILE                                     */
/*============================================================================*/


