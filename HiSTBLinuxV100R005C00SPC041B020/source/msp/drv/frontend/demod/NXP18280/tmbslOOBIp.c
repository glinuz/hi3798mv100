/**
$Header: 
(C) Copyright 2007 NXP Semiconductors, All rights reserved

This source code and any compilation or derivative thereof is the sole
property of NXP Corporation and is provided pursuant to a Software
License Agreement.    This code is the proprietary information of NXP
Corporation and is confidential in nature.    Its use and dissemination by
any party other than NXP Corporation is strictly limited by the
confidential information provisions of the Agreement referenced above.
-----------------------------------------------------------------------------
FILE NAME:        tmbslOOBIp.c

DESCRIPTION:    Function for the silicon demodulator OOBIp

DOCUMENT REF: <References to specification or other documents related to
              this module>

NOTES:
*/

/*============================================================================*/
/*                   STANDARD INCLUDE FILES                                   */
/*============================================================================*/
/*
tmCompId                (def errors)
nxtypes                 (standard types)
tmflags                 (dependance machine)
tmbslFrontEndCfgItem.h  (enum config)
tmFrontend.h            (common enum)
tmbslfrontend.h         (def struct init)
tmbslFrontEndTypes.h    (bsl commo, types)
*/

#include "tmNxTypes.h"
#include "tmCompId.h"
#include "tmFrontEnd.h"
#include "tmbslFrontEndTypes.h"
#include "tmUnitParams.h"

#include "tmbslOOBIp.h"
#include "tmbslOOBIplocal.h"
#include "tmbslOOBIp_cfg.h"

#include "tmbslOOBIpInstance.h"

/*============================================================================*/
/*                   PROJECT INCLUDE FILES                                    */
/*============================================================================*/

/*============================================================================*/
/*                   MACRO DEFINITION                                         */
/*============================================================================*/
#define SEND_TRACEFCT1(x,y)
#define SEND_TRACEFCT2(x,y)
#define SEND_TRACEFCT3(x,y)
#define SEND_TRACEFCT4(x,y)
#define SEND_TRACE(x,y) 

#define NO_FLOAT

/*============================================================================*/
/*                   TYPE DEFINITION                                          */
/*============================================================================*/

/*============================================================================*/
/*                   PUBLIC VARIABLES DEFINITION                              */
/*============================================================================*/

/*============================================================================*/
/*                   STATIC VARIABLES DECLARATIONS                            */
/*============================================================================*/

/*============================================================================*/
/*                       EXTERN FUNCTION PROTOTYPES                           */
/*============================================================================*/

/*============================================================================*/
/*                   STATIC FUNCTIONS DECLARATIONS                            */
/*============================================================================*/

/*============================================================================*/
/*                   PUBLIC FUNCTIONS DEFINITIONS                             */
/*============================================================================*/

/*============================================================================*/
/* tmbslOOBIp_Init                                                          */
/*============================================================================*/
tmErrorCode_t
tmbslOOBIp_Init
(
    tmUnitSelect_t            tUnit,
    tmbslFrontEndDependency_t *psSrvFunc
)
{
    tmOOBIpObject_t    *pObj = Null;
    tmErrorCode_t       err = OOBIP_ERR_NOT_INITIALIZED;
    UInt32              ulIndex;
    
    /* check input parameters */
    err = OOBIpGetInstance(tUnit, &pObj);

    /* check driver state */
    if (err == TM_OK || err == OOBIP_ERR_NOT_INITIALIZED)
    {
        if (pObj != Null && pObj->sApiFlags.iInit == True)
        {
            err = OOBIP_ERR_NOT_INITIALIZED;
        }
        else 
        {
            /* initialize the Object */
            if (pObj == Null)
            {
                err = OOBIpAllocInstance(tUnit, &pObj);
                if (err != TM_OK || pObj == Null)
                {
                    err = OOBIP_ERR_NOT_INITIALIZED;        
                }
            }

            if (err == TM_OK)
            {
                pObj->tUnit = tUnit;
                pObj->sRWFunc = psSrvFunc->sIo;
                pObj->sTime = psSrvFunc->sTime;
                pObj->sDebug = psSrvFunc->sDebug;

                if(  psSrvFunc->sMutex.Init != Null
                    && psSrvFunc->sMutex.DeInit != Null
                    && psSrvFunc->sMutex.Acquire != Null
                    && psSrvFunc->sMutex.Release != Null)
                {
                    pObj->sMutex = psSrvFunc->sMutex;

                    err = pObj->sMutex.Init(&pObj->pMutex);
                }

                pObj->sApiFlags.iInit = True;

                tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslOOBIp_Init(0x%08X)", tUnit);
            }
            
            if (err == TM_OK)
            {
                pObj->uStartAddress = 0x1000;
                pObj->sConfig.uSamplingClock = TMBSLOOBIp_CFG_SAMPLING_FREQ_ES1;
                pObj->sConfig.ePODIfcMode = OOBIP_PODIfcModeDisabled;
                pObj->sConfig.eFECMode = tmFrontEndFECModeAnnexA;
                pObj->sConfig.eA2DClkPol = OOBIP_PolarityNormal;
                pObj->sConfig.bEmoInitMode = 1;
                pObj->sConfig.eAAGCPol = OOBIP_PolarityNormal;
                pObj->sConfig.bAAGCThreshold = 0x10;
                pObj->sConfig.uIF = 5000000;
                pObj->sCurrentChannel.uSR = 762000;
                pObj->sCurrentChannel.eMod = tmFrontEndModulationQpsk;
                pObj->sCurrentChannel.eSI = tmFrontEndSpecInvOff;
                pObj->uUncors = 0;
                pObj->eHwSample = OOBIP_HwSampleVersion_Unknown;

                pObj->uMERDepth = OOBIP_MER_DEFAULT_DEPTH;

                for( ulIndex=0 ; ulIndex<OOBIP_MER_TABLE_SIZE ; ulIndex++)
                {
                    pObj->puMERError[ulIndex] = 0;
                }

                pObj->uMERElementToUpdate = 0;
            }
        }
    } 

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslOOBIp_Init(0x%08X) failed.", tUnit));

    return err;
}


/*============================================================================*/
/* tmbslOOBIp_DeInit                                                        */
/*============================================================================*/
tmErrorCode_t 
tmbslOOBIp_DeInit
(
    tmUnitSelect_t tUnit
)
{
    tmOOBIpObject_t    *pObj = Null;
    tmErrorCode_t       err = OOBIP_ERR_NOT_INITIALIZED;

    /* check input parameters */
    err = OOBIpGetInstance(tUnit, &pObj);
    
    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslOOBIp_DeInit(0x%08X)", tUnit);

    if(err == TM_OK)
    {
        if(pObj->sMutex.DeInit != Null)
        {
            if(pObj->pMutex != Null)
            {
                err = pObj->sMutex.DeInit(pObj->pMutex);
            }

            pObj->sMutex.Init = Null;
            pObj->sMutex.DeInit = Null;
            pObj->sMutex.Acquire = Null;
            pObj->sMutex.Release = Null;

            pObj->pMutex = Null;
        }
    }

    err = OOBIpDeAllocInstance(tUnit);

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslOOBIp_DeInit(0x%08X) failed.", tUnit));

    return err;
}
/*============================================================================*/
/* tmbslOOBIp_GetSWVersion                                                    */
/*============================================================================*/
tmErrorCode_t     
tmbslOOBIp_GetSWVersion
(
    ptmSWVersion_t      pSWVersion
)
{
    pSWVersion->compatibilityNr = OOBIP_BSL_COMP_NUM;
    pSWVersion->majorVersionNr  = OOBIP_BSL_MAJOR_VER;
    pSWVersion->minorVersionNr  = OOBIP_BSL_MINOR_VER;

    return TM_OK;
}

/*============================================================================*/
/* tmbslOOBIp_GetHwSample                                                     */
/*============================================================================*/
tmErrorCode_t
tmbslOOBIp_GetHwSample(
    tmUnitSelect_t tUnit,
    OOBIp_HwSampleVersion_t *peHwSample
)
{
    tmOOBIpObject_t    *pObj = Null;
    tmErrorCode_t       err = OOBIP_ERR_NOT_INITIALIZED;

    /* check input parameters */
    err = OOBIpGetInstance(tUnit, &pObj);

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslOOBIp_GetHwSample(0x%08X)", tUnit);

    if (err == TM_OK)
    {
        *peHwSample = pObj->eHwSample;
    }

    return err;
}

/*============================================================================*/
/* tmbslOOBIp_Reset                                                           */
/*============================================================================*/
tmErrorCode_t
tmbslOOBIp_Reset
(
    tmUnitSelect_t tUnit    /* I: Unit number */
)
{
    tmOOBIpObject_t    *pObj = Null;
    tmErrorCode_t       err = OOBIP_ERR_NOT_INITIALIZED;
    UInt8 uValue;
    UInt8 uMseBw;

    /* check input parameters */
    err = OOBIpGetInstance(tUnit, &pObj);

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslOOBIp_Reset(0x%08X)", tUnit);
    
    if (err == TM_OK)
    {
        err = OOBIpMutexAcquire(pObj, OOBIP_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        err = OOBIpRead(tUnit, OOBIP_RD_FRZ_VERSION_IND, 1, &uValue);

        if (err == TM_OK)
        {
            if (uValue == 0xC4)
            {
                uMseBw = 0x2;
                err = OOBIpWriteBit(tUnit, OOBIP_MSE_CTL_IND, OOBIP_MSE_BW_MSK | OOBIP_MSE_RESET_MSK, (uMseBw<<1) | OOBIP_MSE_RESET_MSK);
                if (err == TM_OK)
                {
                    err = OOBIpRead(tUnit, OOBIP_MSE_CTL_IND, 1, &uValue);
                }
                if (err == TM_OK)
                {
                    if (uValue != 0x0) /* temporary: to be adapted when ES1 is removed */
                    {
                        pObj->eHwSample = OOBIP_HwSampleVersion_ES2;
                        err = OOBIpWriteBit(tUnit, OOBIP_MSE_CTL_IND, OOBIP_MSE_RESET_MSK, 0);
                    }
                    else
                    {
                        pObj->eHwSample = OOBIP_HwSampleVersion_ES1;
                    }
                }
            }
            else
            {
                pObj->eHwSample = OOBIP_HwSampleVersion_Unknown;
            }
        }

        /* configure PLL clock depending on ES version */
        if (err == TM_OK)
        {
            if( pObj->eHwSample == OOBIP_HwSampleVersion_ES1 )
            {
                pObj->sConfig.uSamplingClock = TMBSLOOBIp_CFG_SAMPLING_FREQ_ES1;
            }
            else if( pObj->eHwSample == OOBIP_HwSampleVersion_ES2 )
            {
                pObj->sConfig.uSamplingClock = TMBSLOOBIp_CFG_SAMPLING_FREQ_ES2;
            }
            else
            {
                pObj->sConfig.uSamplingClock = 0;
            }
        }

        if (err == TM_OK)
        {
            uValue = 0;
            if (pObj->sConfig.bEmoInitMode)
            {
                uValue |= OOBIP_EMO_INIT_MODE_MSK;
            }
            else
            {
                uValue &= !OOBIP_EMO_INIT_MODE_MSK;
            }
            if (pObj->sConfig.ePODIfcMode == OOBIP_PODIfcModeEnabled)
            {
                uValue |= OOBIP_OOB_POD_IFC_MODE_MSK;
            }
            else
            {
                uValue &= !OOBIP_OOB_POD_IFC_MODE_MSK;
            }
            if (pObj->sConfig.eFECMode == tmFrontEndFECModeAnnexA)
            {
                /* Mode A/Aloha/GI, DVS/167, SCTE55-1 */
                uValue |= OOBIP_GI_MODE_MSK;
            }
            else
            {
                /* Mode B/DAVIC, DVS/178, SCTE55-2 */
                uValue &= !OOBIP_GI_MODE_MSK;
            }
            err = OOBIpWrite(tUnit, OOBIP_MODE_IND, 1, &uValue);
        }

        if (err == TM_OK)
        {
            uValue = 0;
            if (pObj->sConfig.eAAGCPol == OOBIP_PolarityNormal)
            {
                uValue &= !OOBIP_AAGC_SD_POL_MSK;
            }
            else
            {
                uValue |= OOBIP_AAGC_SD_POL_MSK;
            }
            uValue |= (pObj->sConfig.bAAGCThreshold & OOBIP_AAGC_THD_MSK);
            err = OOBIpWrite(tUnit, OOBIP_AGC1_IND, 1, &uValue);
        }

        if (err == TM_OK)
        {
            uValue = 0x07; 
            err = OOBIpWrite(tUnit, OOBIP_BTL_CTL_IND, 1, &uValue);
        }

        if (err == TM_OK)
        {
            uValue = 0x33;
            err = OOBIpWrite(tUnit, OOBIP_DAGC_CTL_IND, 1, &uValue);
        }

        if (err == TM_OK)
        {
            uValue = 0x02; /* FAGC.dec_mode_dis = 0, FAGC.loop_gain = 0, FAGC.inout_gain_sel = 2, FIR.alpha_sel = 2 */
            if (pObj->sConfig.eFECMode == tmFrontEndFECModeAnnexA)
            {
                /* Mode A/Aloha/GI, DVS/167, SCTE55-1 */
                uValue |= OOBIP_FIR_ALPHA_0_5_SRRC_BIT << 6;
            }
            else
            {
                /* Mode B/DAVIC, DVS/178, SCTE55-2 */
                uValue |= OOBIP_FIR_ALPHA_0_3_SRRC_BIT << 6;
            }

            err = OOBIpWrite(tUnit, OOBIP_MFIR_IND, 1, &uValue);
        }

        if (err == TM_OK)
        {
            uValue = 0x64; // CTL.use_eq_in = 1, CTL.dg_sel = 4, CTL.idg_sel = 4
            err = OOBIpWrite(tUnit, OOBIP_CTL_CTRL_IND, 1, &uValue);
        }

        if (err == TM_OK)
        {
            uValue = 0x04; // LOCK.new_qpsk_em = 1, LOCK.em_gain_sel = 0
            err = OOBIpWrite(tUnit, OOBIP_LOCK_IND, 1, &uValue);
        }

        if (err == TM_OK)
        {
            uValue = 0x00; // AAGC.zero_em = 1, BTL.zero_em = 0, CTL.zero_em = 0, DAGC.zero_em = 1, FAGC.zero_em = 1, LOCK.lock_em0_mode = 1, OOB.out_dis = 0
            err = OOBIpWrite(tUnit, OOBIP_DIS_IND, 1, &uValue);
        }

        if (err == TM_OK)
        {
            uValue = 0x20; // DEMOD.a2d_is_2c = 1
            err = OOBIpWrite(tUnit, OOBIP_MISC_IND, 1, &uValue);
        }

        if (err == TM_OK)
        {
            uValue = 0x10; // OOB.puncture_sel = 1
            err = OOBIpWrite(tUnit, OOBIP_FEC_OCTL_IND, 1, &uValue);
        }

        if (err == TM_OK)
        {
            uValue = 0x08; // TEST.oob_pod_in_mode = 0, TEST.test_enable = 1, TEST.test_sel = 0
            err = OOBIpWrite(tUnit, OOBIP_TST3_IND, 1, &uValue);
        }

        if (err == TM_OK)
        {
            uValue = 0x08; // TEST.oob_pod_in_mode = 0, TEST.test_enable = 1, TEST.test_sel = 0
            err = OOBIpWrite(tUnit, OOBIP_TST3_IND, 1, &uValue);
        }

        (void)OOBIpMutexRelease(pObj);
    }

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslOOBIp_Reset(0x%08X) failed.", tUnit));
    
    return err;
}

/*============================================================================*/
/* tmbslOOBIp_StartLock                                                        */
/*============================================================================*/
tmErrorCode_t
tmbslOOBIp_StartLock
(
    tmUnitSelect_t tUnit    /* I: Unit number */
)
{
    tmOOBIpObject_t    *pObj = Null;
    tmErrorCode_t       err = OOBIP_ERR_NOT_INITIALIZED;
    UInt8               uValue;
    UInt32              uIndex;

    /* check input parameters */
    err = OOBIpGetInstance(tUnit, &pObj);

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslOOBIp_StartLock(0x%08X)", tUnit);
    
    if(err == TM_OK)
    {
        err = OOBIpMutexAcquire(pObj, OOBIP_MUTEX_TIMEOUT);
    }

    if(err == TM_OK)
    {
        uValue = 0x0F; // Restart acquisition
        err = OOBIpWriteBit(tUnit, OOBIP_RACQ_CTL_IND, 0x0F, uValue);

        if (err == TM_OK)
        {
            err = OOBIpWait(tUnit, 100); /* TODO : determine the proper time */
        }

        if (err == TM_OK)
        {
            /* Reset the DFAIL_ERR flag */
            err = OOBIpWriteBit(tUnit, OOBIP_STAT1_IND, OOBIP_OOB_DFAIL_ERR_MSK, OOBIP_OOB_DFAIL_ERR_MSK);
        }
        
        if (err == TM_OK)
        {
            /* Reset the RS_ERR flag */
            err = OOBIpWriteBit(tUnit, OOBIP_STAT1_IND, OOBIP_OOB_RS_ERR_MSK, OOBIP_OOB_RS_ERR_MSK);
        }

        if (err == TM_OK)
        {
            pObj->uUncors = 0;
            pObj->uCorrected = 0;
            pObj->uIMean = 0;
            pObj->uQMean = 0;
            for( uIndex=0 ; uIndex<OOBIP_MER_TABLE_SIZE ; uIndex++)
            {
                pObj->puMERError[uIndex] = 0;
            }
            pObj->uMERElementToUpdate = 0;
        }
        
        (void)OOBIpMutexRelease(pObj);
    }

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslOOBIp_StartLock(0x%08X) failed.", tUnit));
    
    return err;
}


/*============================================================================*/
/* tmbslOOBIp_GetEsNo                                                        */
/*============================================================================*/
tmErrorCode_t
tmbslOOBIp_GetEsNo
(
    tmUnitSelect_t   tUnit,
    UInt8           *pbEsNo
)
{
    tmOOBIpObject_t    *pObj = Null;
    tmErrorCode_t       err = OOBIP_ERR_NOT_INITIALIZED;
    UInt8               uValue = 0;

    /* check input parameters */
    err = OOBIpGetInstance(tUnit, &pObj);

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslOOBIp_GetEsNo(0x%08X)", tUnit);
    
    if (err == TM_OK)
    {
        err = OOBIpMutexAcquire(pObj, OOBIP_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        err = OOBIpRead(tUnit, OOBIP_ESNO_IND, 1, &uValue);

        if (err == TM_OK)
        {
            *pbEsNo = uValue;
        }

        (void)OOBIpMutexRelease(pObj);
    }

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslOOBIp_GetEsNo(0x%08X) failed.", tUnit));

    return err;
}

/*============================================================================*/
/* tmbslOOBIp_GetLockStatus                                                   */
/*============================================================================*/
tmErrorCode_t
tmbslOOBIp_GetLockStatus
(
    tmUnitSelect_t        tUnit,
    tmbslFrontEndState_t *peLockStatus
)
{
    
    tmOOBIpObject_t      *pObj = Null;
    tmErrorCode_t        err = OOBIP_ERR_NOT_INITIALIZED;
    UInt8                uFECLock;
    UInt8                uCarrierLock;

    /* check input parameters */
    err = OOBIpGetInstance(tUnit, &pObj);

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslOOBIp_GetLockStatus(0x%08X)", tUnit);

    if (err == TM_OK)
    {
        err = OOBIpMutexAcquire(pObj, OOBIP_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        if (pObj->ePowerState == tmPowerOn)
        {
            err = OOBIpRead(tUnit, OOBIP_STAT1_IND, 1, &uFECLock);

            if (err == TM_OK)
            {
                err = OOBIpRead(tUnit, OOBIP_LOCK_IND, 1, &uCarrierLock);
            }

            if (err == TM_OK)
            {
                if (pObj->sConfig.ePODIfcMode == OOBIP_PODIfcModeDisabled)
                {
                    /* FEC is used */
                    if (!((uFECLock & OOBIP_FRAME_NOT_SYNC_MSK) == OOBIP_FRAME_NOT_SYNC_MSK) && ((uCarrierLock & OOBIP_LOCK_IND_MSK) == OOBIP_LOCK_IND_MSK))
                    {
                        *peLockStatus = tmbslFrontEndStateLocked;
                    }
                    else
                    {
                        *peLockStatus = tmbslFrontEndStateSearching;
                    }
                }
                else /* (pObj->sConfig.ePODIfcMode == OOBIP_PODIfcModeEnabled) */
                {
                    /* FEC is not used (POD) */
                    if ((uCarrierLock & OOBIP_LOCK_IND_MSK) == OOBIP_LOCK_IND_MSK)
                    {
                        *peLockStatus = tmbslFrontEndStateLocked;
                    }
                    else
                    {
                        *peLockStatus = tmbslFrontEndStateSearching;
                    }
                }
            }
        }
        else
        {
            *peLockStatus = tmbslFrontEndStateNotLocked;
        }
        (void)OOBIpMutexRelease(pObj);
    }

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslOOBIp_GetLockStatus(0x%08X) failed.", tUnit));
    
    return err;
}

/*============================================================================*/
/* tmbslOOBIp_GetLockInd                                                      */
/*============================================================================*/
tmErrorCode_t
tmbslOOBIp_GetLockInd
(
    tmUnitSelect_t       tUnit,
    OOBIP_LockInd_t     *psLockInd
)
{
    
    tmOOBIpObject_t     *pObj = Null;
    tmErrorCode_t        err = OOBIP_ERR_NOT_INITIALIZED;
    UInt8                uValue = 0;

    /* check input parameters */
    err = OOBIpGetInstance(tUnit, &pObj);

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslOOBIp_GetLockStatus(0x%08X)", tUnit);
    
    if (err == TM_OK)
    {
        err = OOBIpMutexAcquire(pObj, OOBIP_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        if (pObj->ePowerState == tmPowerOn)
        {
            /* carrier and demod lock */
            err = OOBIpRead(tUnit, OOBIP_LOCK_IND, 1, &uValue);

            if (err == TM_OK)
            {
                if ( (uValue & OOBIP_LOCK_IND_MSK) == OOBIP_LOCK_IND_MSK)
                {
                    psLockInd->bCarrierLock = True;
                }
                else
                {
                    psLockInd->bCarrierLock = False;
                }
                if ( (uValue & OOBIP_DEMOD_LOCKD_MSK) == OOBIP_DEMOD_LOCKD_MSK)
                {
                    psLockInd->bDemodLock = True;
                }
                else
                {
                    psLockInd->bDemodLock = False;
                }

                /* lock accumulator */
                err = OOBIpRead(tUnit, OOBIP_LOCK_ACC_IND, 1, &uValue);
            }

            if (err == TM_OK)
            {
                psLockInd->bLockAcc =uValue;


                /* FEC lock and Sync quality */
                err = OOBIpRead(tUnit, OOBIP_STAT2_IND, 1, &uValue);
            }

            if (err == TM_OK)
            {
                if ((uValue & OOBIP_FRAME_SYNC_MSK) == OOBIP_FRAME_SYNC_MSK)
                {
                    psLockInd->bFECLock = True;
                }
                else
                {
                    psLockInd->bFECLock = False;
                }

                psLockInd->bSyncQuality = uValue & OOBIP_TRACK_CNT_MSK;
            }
        }
        else
        {
            psLockInd->bCarrierLock = False;
            psLockInd->bDemodLock = False;
            psLockInd->bFECLock = False;
            psLockInd->bLockAcc = 0;
            psLockInd->bSyncQuality = 0;
        }
        (void)OOBIpMutexRelease(pObj);
    }

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslOOBIp_GetLockStatus(0x%08X) failed.", tUnit));
    
    return err;
}

/*============================================================================*/
/* tmbslOOBIp_GetAGC                                                           */
/*============================================================================*/
tmErrorCode_t
tmbslOOBIp_GetAGCInd
(
    tmUnitSelect_t    tUnit,
    OOBIP_AGCInd_t   *psAGCInd
)
{
    tmOOBIpObject_t    *pObj = Null;
    tmErrorCode_t       err = OOBIP_ERR_NOT_INITIALIZED;
    UInt8               puValue[2];

    /* check input parameters */
    err = OOBIpGetInstance(tUnit, &pObj);

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslOOBIp_GetAGC(0x%08X)", tUnit);
    
    if (err == TM_OK)
    {
        err = OOBIpMutexAcquire(pObj, OOBIP_MUTEX_TIMEOUT);
    }
    
    if (err == TM_OK)
    {
        /* freeze the registers*/
        puValue[0] = 0xC4;
        err = OOBIpWrite(tUnit, OOBIP_RD_FRZ_VERSION_IND, 1, puValue);
        
        if (err == TM_OK)
        {
            /* read AAGC */
            err = OOBIpRead(tUnit, OOBIP_AGCA_IND, 1, puValue);
        }

        if (err == TM_OK)
        {
            psAGCInd->bAAGCAcc = puValue[0] & OOBIP_AAGC_ACC0_MSK;

            /* read DAGC */
            err = OOBIpRead(tUnit, OOBIP_DAGC_ACC_IND, 1, puValue);
        }

        if (err == TM_OK)
        {
            psAGCInd->bDAGCAcc = puValue[0] & OOBIP_DAGC_ACC0_MSK;

            /* read FAGC */
            err = OOBIpRead(tUnit, OOBIP_FAGC_ACC_IND, 1, puValue);
        }

        if (err == TM_OK)
        {
            psAGCInd->bFAGCAcc = puValue[0] & OOBIP_FAGC_ACC0_MSK;

            /* compute the saturation indicators */
            if ( (psAGCInd->bAAGCAcc == OOBIP_AAGC_MIN) || (psAGCInd->bAAGCAcc == OOBIP_AAGC_MAX) )
            {
                psAGCInd->bAAGCSat = True;
            }
            else
            {
                psAGCInd->bAAGCSat = False;
            }

            if ( (psAGCInd->bDAGCAcc == OOBIP_DAGC_MIN) || (psAGCInd->bDAGCAcc == OOBIP_DAGC_MAX) )
            {
                psAGCInd->bDAGCSat = True;
            }
            else
            {
                psAGCInd->bDAGCSat = False;
            }

            if ( (psAGCInd->bFAGCAcc == OOBIP_FAGC_MIN) || (psAGCInd->bFAGCAcc == OOBIP_FAGC_MAX) )
            {
                psAGCInd->bFAGCSat = True;
            }
            else
            {
                psAGCInd->bFAGCSat = False;
            }

            err = OOBIpRead(tUnit, OOBIP_BTL_ACC_IND, 1, puValue);
        }

        if (err == TM_OK)
        {
            psAGCInd->bBTLAcc = puValue[0] & OOBIP_BTL_ACC0_MSK;

            err = OOBIpRead(tUnit, OOBIP_RFAGC_CTL_IND, 2, puValue);
        }

        if (err == TM_OK)
        {
            psAGCInd->bRFSDLevel = (puValue[0]<<2 | (puValue[1] & OOBIP_RF_SD_LEVEL_LSB_MSK)) & OOBIP_RF_SD_LEVEL_MSK;

            err = OOBIpRead(tUnit, OOBIP_CTL_ACC_MSB_IND, 2, puValue);
        }

        if (err == TM_OK)
        {
            psAGCInd->bCTLAcc = (puValue[0]<<4 | (puValue[1] & OOBIP_CTL_LSB_MSK)) & OOBIP_CTL_MSK;
        }

        (void)OOBIpMutexRelease(pObj);
    }

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslOOBIp_GetAGC(0x%08X) failed.", tUnit));
    
    return err;
}

/*============================================================================*/
/* tmbslOOBIp_SetSR                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslOOBIp_SetSR
(
    tmUnitSelect_t tUnit,
    UInt32         uSR
)
{
    unsigned long long uBuffer = 0LL;
    UInt32             uOOBIpSR;
    UInt8              uValueMSB;
    UInt8              uValueCSB;
    UInt8              uValueLSB;

    tmOOBIpObject_t   *pObj = Null;
    tmErrorCode_t      err = OOBIP_ERR_NOT_INITIALIZED;

    /* check input parameters */
    err = OOBIpGetInstance(tUnit, &pObj);

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslOOBIp_SetSR(0x%08X)", tUnit);
    
    if (err == TM_OK)
    {
        err = OOBIpMutexAcquire(pObj, OOBIP_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        pObj->sCurrentChannel.uSR = uSR;
        
        //uBuffer = ((unsigned long long)uSR*0x2000000/pObj->sConfig.uSamplingClock); //forrest
        
        uOOBIpSR =  (UInt32)(uBuffer & 0xFFFFFFFF);
        
        uValueLSB = (UInt8)(uOOBIpSR & 0xFF);
        uValueCSB = (UInt8)((uOOBIpSR>>8)&0xFF);
        uValueMSB = (UInt8)((uOOBIpSR>>16)&0xFF);

        err = OOBIpWrite(tUnit, OOBIP_SYM_RATE_LSB_IND, 1, &uValueLSB);

        if (err == TM_OK)
        {
            err = OOBIpWrite(tUnit, OOBIP_SYM_RATE_CSB_IND, 1, &uValueCSB);
        }

        if (err == TM_OK)
        {
            err = OOBIpWrite(tUnit, OOBIP_SYM_RATE_MSB_IND, 1, &uValueMSB);
        }
        
        (void)OOBIpMutexRelease(pObj);
    }

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslOOBIp_SetSR(0x%08X) failed.", tUnit));
    
    return err;
}

/*============================================================================*/
/* tmbslOOBIp_GetSR                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslOOBIp_GetSR
(
    tmUnitSelect_t tUnit,
    UInt32         *puSR
)
{
    unsigned long long uBuffer = 0LL;
    UInt8              uValueMSB;
    UInt8              uValueCSB;
    UInt8              uValueLSB;
    
    tmOOBIpObject_t   *pObj = Null;
    tmErrorCode_t      err = OOBIP_ERR_NOT_INITIALIZED;

    /* check input parameters */
    err = OOBIpGetInstance(tUnit, &pObj);

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslOOBIp_GetSR(0x%08X)", tUnit);
    
    if (err == TM_OK)
    {
        err = OOBIpMutexAcquire(pObj, OOBIP_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        err = OOBIpRead(tUnit, OOBIP_SYM_RATE_LSB_IND, 1, &uValueLSB);

        if (err == TM_OK)
        {
            err = OOBIpRead(tUnit, OOBIP_SYM_RATE_CSB_IND, 1, &uValueCSB);
        }

        if (err == TM_OK)
        {
            err = OOBIpRead(tUnit, OOBIP_SYM_RATE_MSB_IND, 1, &uValueMSB);
        }

        if (err == TM_OK)
        {
            //uBuffer = (unsigned long long)(uValueLSB | (uValueCSB << 8) | (uValueMSB << 16)); //forrest

            uBuffer = uBuffer * pObj->sConfig.uSamplingClock / 0x2000000;

            *puSR = (UInt32)uBuffer;
        }

        (void)OOBIpMutexRelease(pObj);
    }

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslOOBIp_GetSR(0x%08X) failed.", tUnit));

    return err;
}

/*============================================================================*/
/* tmbslOOBIp_GetConfiguredSR                                                  */
/*============================================================================*/
tmErrorCode_t
tmbslOOBIp_GetConfiguredSR
(
    tmUnitSelect_t tUnit,
    UInt32         *puSR
)
{
    tmOOBIpObject_t  *pObj = Null;
    tmErrorCode_t     err = OOBIP_ERR_NOT_INITIALIZED;

    /* check input parameters */
    err = OOBIpGetInstance(tUnit, &pObj);

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslOOBIp_GetConfiguredSR(0x%08X)", tUnit);
    
    if (err == TM_OK)
    {
        err = OOBIpMutexAcquire(pObj, OOBIP_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        *puSR = pObj->sCurrentChannel.uSR;

        (void)OOBIpMutexRelease(pObj);
    }

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslOOBIp_GetConfiguredSR(0x%08X) failed.", tUnit));

    return err;
}

/*============================================================================*/
/* tmbslOOBIp_SetMod                                                        */
/*============================================================================*/
tmErrorCode_t
tmbslOOBIp_SetMod
(
    tmUnitSelect_t         tUnit,
    tmFrontEndModulation_t eMod
)
{
    tmOOBIpObject_t    *pObj = Null;
    tmErrorCode_t       err = OOBIP_ERR_NOT_INITIALIZED;
    UInt8               uValue = 0;

    /* check input parameters */
    err = OOBIpGetInstance(tUnit, &pObj);

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslOOBIp_SetMod(0x%08X)", tUnit);
    
    if (err == TM_OK)
    {
        /* test the parameter value */
        if (eMod >= tmFrontEndModulationQam4)
        {
            err = OOBIP_ERR_BAD_PARAMETER;
        }
    }

    if (err == TM_OK)
    {
        pObj->sCurrentChannel.eMod = eMod;

        if (eMod == tmFrontEndModulationBpsk)
        {
            uValue |= OOBIP_BPSK_MODE_MSK;
        }
        else
        {
            uValue &= !OOBIP_BPSK_MODE_MSK;
        }
        err = OOBIpWriteBit(tUnit, OOBIP_MODE_IND, OOBIP_BPSK_MODE_MSK, uValue);

        (void)OOBIpMutexRelease(pObj);
    }

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslOOBIp_SetMod(0x%08X) failed.", tUnit));

    return err;
}

/*============================================================================*/
/* tmbslOOBIp_GetMod                                                        */
/*============================================================================*/
tmErrorCode_t
tmbslOOBIp_GetMod
(
    tmUnitSelect_t          tUnit,
    tmFrontEndModulation_t *peMod
)
{
    tmOOBIpObject_t     *pObj = Null;
    tmErrorCode_t        err = OOBIP_ERR_NOT_INITIALIZED;
    UInt8                uValue = 0;

    /* check input parameters */
    err = OOBIpGetInstance(tUnit, &pObj);

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslOOBIp_GetMod(0x%08X)", tUnit);
    
    if (err == TM_OK)
    {
        err = OOBIpMutexAcquire(pObj, OOBIP_MUTEX_TIMEOUT);
    }
    
    if (err == TM_OK)
    {
        err = OOBIpRead(tUnit, OOBIP_MODE_IND, 1, &uValue);

        uValue &= OOBIP_BPSK_MODE_MSK;
        if (uValue == OOBIP_BPSK_MODE_MSK)
        {
            *peMod = tmFrontEndModulationBpsk;
        }
        else
        {
            *peMod = tmFrontEndModulationQpsk;
        }

        (void)OOBIpMutexRelease(pObj);
    }

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslOOBIp_GetMod(0x%08X) failed.", tUnit));

    return err;
}

/*============================================================================*/
/* tmbslOOBIp_SetIF                                                         */
/*============================================================================*/
tmErrorCode_t
tmbslOOBIp_SetIF
(
    tmUnitSelect_t tUnit,   /* I: Unit number */
    UInt32         uIF      /* I: If in Herz  */
)
{
    UInt32               uOOBIpFreq;
    UInt8                uValueMSB;
    UInt8                uValueCSB;
    UInt8                uValueLSB;
    unsigned long long   uBuffer = 0LL;
    tmOOBIpObject_t     *pObj = Null;
    tmErrorCode_t        err = OOBIP_ERR_NOT_INITIALIZED;

    /* check input parameters */
    err = OOBIpGetInstance(tUnit, &pObj);

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslOOBIp_SetIF(0x%08X)", tUnit);
    
    if (err == TM_OK)
    {
        err = OOBIpMutexAcquire(pObj, OOBIP_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        pObj->sConfig.uIF = uIF;
        
        if (pObj->sConfig.uSamplingClock>= uIF)
        {
            //uBuffer = (((unsigned long long)pObj->sConfig.uSamplingClock-uIF)*0x1000000/pObj->sConfig.uSamplingClock); //forrest
            uOOBIpFreq =  (UInt32)(uBuffer & 0xFFFFFFFF);
        }
        else
        {
            uOOBIpFreq = ((uIF-pObj->sConfig.uSamplingClock)*0x1000000/pObj->sConfig.uSamplingClock);
        }

        uValueLSB = (UInt8)(uOOBIpFreq & 0xFF);
        uValueCSB = (UInt8)((uOOBIpFreq>>8)&0xFF);
        uValueMSB = (UInt8)((uOOBIpFreq>>16)&0xFF);

        err = OOBIpWrite(tUnit, OOBIP_FREQ_LSB_IND, 1, &uValueLSB);

        if (err == TM_OK)
        {
            err = OOBIpWrite(tUnit, OOBIP_FREQ_CSB_IND, 1, &uValueCSB);
        }

        if (err == TM_OK)
        {
            err = OOBIpWrite(tUnit, OOBIP_FREQ_MSB_IND, 1, &uValueMSB);
        }

        (void)OOBIpMutexRelease(pObj);
    }

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslOOBIp_SetIF(0x%08X) failed.", tUnit));

    return err;
}

/*============================================================================*/
/* tmbslOOBIp_GetIF                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslOOBIp_GetIFOffset
(
    tmUnitSelect_t tUnit,   /* I: Unit number */
    Int32         *puIFOffset    /* I: If in Herz  */
)
{
    UInt8                uValueMSB;
    UInt8                uValueCSB;
    UInt8                uValueLSB;
    unsigned long long   uBuffer = 0LL;
    tmOOBIpObject_t     *pObj = Null;
    tmErrorCode_t        err = OOBIP_ERR_NOT_INITIALIZED;
    UInt8                puData[2];
    Int32                iFreqError;
    UInt8                uValue;

    /* check input parameters */
    err = OOBIpGetInstance(tUnit, &pObj);

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslOOBIp_GetIF(0x%08X)", tUnit);

    if (err == TM_OK)
    {
        err = OOBIpMutexAcquire(pObj, OOBIP_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        err = OOBIpRead(tUnit, OOBIP_FREQ_LSB_IND, 1, &uValueLSB);

        if (err == TM_OK)
        {
            err = OOBIpRead(tUnit, OOBIP_FREQ_CSB_IND, 1, &uValueCSB);
        }

        if (err == TM_OK)
        {
            err = OOBIpRead(tUnit, OOBIP_FREQ_MSB_IND, 1, &uValueMSB);
        }

        if (err == TM_OK)
        {
            //uBuffer = (unsigned long long)(uValueLSB | (uValueCSB << 8) | (uValueMSB << 16)); //forrest

            uBuffer = uBuffer * pObj->sConfig.uSamplingClock / 0x1000000;

            if (pObj->sConfig.uSamplingClock >= pObj->sConfig.uIF)
            {
                *puIFOffset = (UInt32)(pObj->sConfig.uSamplingClock - uBuffer);
            }
            else
            {
                *puIFOffset = (UInt32)(pObj->sConfig.uSamplingClock + uBuffer);
            }

            /* Add the freq error */
            /* freeze the registers*/
            uValue = 0xC4;
            err = OOBIpWrite(tUnit, OOBIP_RD_FRZ_VERSION_IND, 1, &uValue);
        }

        if (err == TM_OK)
        {
            err = OOBIpRead(tUnit, OOBIP_CTL_ACC_MSB_IND, 2, puData);
        }

        if (err == TM_OK)
        {
            iFreqError = (Int32)(0xFFFFF000 * (puData[0]>>7) + (puData[0] << 4) + (0x0F & puData[1]));
            
            iFreqError *= (pObj->sCurrentChannel.uSR >> 14);

            *puIFOffset -= iFreqError;

            *puIFOffset -= pObj->sConfig.uIF;
        }

        (void)OOBIpMutexRelease(pObj);
    }

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslOOBIp_GetIF(0x%08X) failed.", tUnit));
    
    return err;
}

/*============================================================================*/
/* tmbslOOBIp_GetConfiguredIF                                                  */
/*============================================================================*/
tmErrorCode_t
tmbslOOBIp_GetConfiguredIF
(
    tmUnitSelect_t tUnit,   /* I: Unit number */
    UInt32         *puIF    /* I: If in Herz  */
)
{
    tmOOBIpObject_t    *pObj = Null;
    tmErrorCode_t       err = OOBIP_ERR_NOT_INITIALIZED;
    
    /* check input parameters */
    err = OOBIpGetInstance(tUnit, &pObj);

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslOOBIp_GetConfiguredIF(0x%08X)", tUnit);

    if (err == TM_OK)
    {
        err = OOBIpMutexAcquire(pObj, OOBIP_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        *puIF = pObj->sConfig.uIF;

        (void)OOBIpMutexRelease(pObj);
    }

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslOOBIp_GetConfiguredIF(0x%08X) failed.", tUnit));
    
    return err;
}

/*============================================================================*/
/* tmbslOOBIp_SetFECMode                                                    */
/*============================================================================*/
extern tmErrorCode_t
tmbslOOBIp_SetFECMode
(
    tmUnitSelect_t          tUnit,
    tmFrontEndFECMode_t     eMode
)
{
    tmOOBIpObject_t    *pObj = Null;
    tmErrorCode_t       err = OOBIP_ERR_NOT_INITIALIZED;
    UInt8               uFECMode = 0;
    UInt8               uMFIR = 0;

    /* check input parameters */
    err = OOBIpGetInstance(tUnit, &pObj);

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslOOBIp_SetFECMode(0x%08X)", tUnit);
    
    if(err == TM_OK)
    {
        err = OOBIpMutexAcquire(pObj, OOBIP_MUTEX_TIMEOUT);
    }
    
    if (err == TM_OK)
    {
        pObj->sConfig.eFECMode = eMode;

        if (eMode == tmFrontEndFECModeAnnexA)
        {
            /* Mode A/Aloha/GI, DVS/167, ANSI/SCTE55-1 */
            uFECMode |= OOBIP_GI_MODE_MSK;

            uMFIR = OOBIP_FIR_ALPHA_0_5_SRRC_BIT << 6;
        }
        else
        {
            /* Mode B/DAVIC, DVS/178, ANSI/SCTE55-2 */
            uFECMode &= !OOBIP_GI_MODE_MSK;

            uMFIR = OOBIP_FIR_ALPHA_0_3_SRRC_BIT << 6;
        }

        err = OOBIpWriteBit(tUnit, OOBIP_MODE_IND, OOBIP_GI_MODE_MSK, uFECMode);
        
        if (err == TM_OK)
        {
            err = OOBIpWriteBit(tUnit, OOBIP_MFIR_IND, OOBIP_FIR_ALPHA_SEL_MSK, uMFIR);
        }

        (void)OOBIpMutexRelease(pObj);
    }
    
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslOOBIp_SetFECMode(0x%08X) failed.", tUnit));

    return err;
}

/*============================================================================*/
/* tmbslOOBIp_GetFECMode                                                    */
/*============================================================================*/
tmErrorCode_t
tmbslOOBIp_GetFECMode
(
    tmUnitSelect_t         tUnit,
    tmFrontEndFECMode_t   *peMode
)
{
    tmOOBIpObject_t    *pObj = Null;
    tmErrorCode_t       err = OOBIP_ERR_NOT_INITIALIZED;
    UInt8               uValue = 0;

    /* check input parameters */
    err = OOBIpGetInstance(tUnit, &pObj);

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslOOBIp_GetFECMode(0x%08X)", tUnit);
    
    if(err == TM_OK)
    {
        err = OOBIpMutexAcquire(pObj, OOBIP_MUTEX_TIMEOUT);
    }
    
    if (err == TM_OK)
    {
        err = OOBIpRead(tUnit, OOBIP_MODE_IND, 1, &uValue);

        if (err == TM_OK)
        {
            uValue &= OOBIP_GI_MODE_MSK;
            if (uValue == OOBIP_GI_MODE_MSK)
            {
                /* Mode A/Aloha/GI, DVS/167, ANSI/SCTE55-1 */
                *peMode = tmFrontEndFECModeAnnexA;
            }
            else
            {
                /* Mode B/DAVIC, DVS/178, ANSI/SCTE55-2 */
                *peMode = tmFrontEndFECModeAnnexB;
            }
        }

        (void)OOBIpMutexRelease(pObj);
    }
    
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslOOBIp_GetFECMode(0x%08X) failed.", tUnit));

    return err;
}

/*============================================================================*/
/* tmbslOOBIp_SetSI                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslOOBIp_SetSI
(
    tmUnitSelect_t      tUnit,
    tmFrontEndSpecInv_t eSI
)
{
    tmOOBIpObject_t    *pObj = Null;
    tmErrorCode_t       err = OOBIP_ERR_NOT_INITIALIZED;
    UInt8               uValue = 0;

    /* check input parameters */
    err = OOBIpGetInstance(tUnit, &pObj);

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslOOBIp_SetSI(0x%08X)", tUnit);
    
    if (err == TM_OK)
    {
        /* test the parameter value*/
        if ((eSI >= tmFrontEndSpecInvMax) || (eSI == tmFrontEndSpecInvAuto))
        {
            err = OOBIP_ERR_BAD_PARAMETER;
        }
    }
    
    if (err == TM_OK)
    {
        err = OOBIpMutexAcquire(pObj, OOBIP_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        pObj->sCurrentChannel.eSI = eSI;

        if (eSI == tmFrontEndSpecInvOn)
        {
            uValue |=  OOBIP_SPECTRAL_INV_MSK;
        }
        else
        {
            uValue &=  !OOBIP_SPECTRAL_INV_MSK;
        }
        
        err = OOBIpWriteBit(tUnit, OOBIP_MODE_IND, OOBIP_SPECTRAL_INV_MSK, uValue);

        (void)OOBIpMutexRelease(pObj);
    }
    
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslOOBIp_SetSI(0x%08X) failed.", tUnit));
    
    return err;
}

/*============================================================================*/
/* tmbslOOBIp_GetSI                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslOOBIp_GetSI
(
    tmUnitSelect_t      tUnit,
    tmFrontEndSpecInv_t *peSI
)
{
    tmOOBIpObject_t    *pObj = Null;
    tmErrorCode_t       err = OOBIP_ERR_NOT_INITIALIZED;
    UInt8               uValue = 0;

    /* check input parameters */
    err = OOBIpGetInstance(tUnit, &pObj);

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslOOBIp_GetSI(0x%08X)", tUnit);
    
    if (err == TM_OK)
    {
        err = OOBIpMutexAcquire(pObj, OOBIP_MUTEX_TIMEOUT);
    }
    
    if (err == TM_OK)
    {
        err = OOBIpRead(tUnit, OOBIP_MODE_IND, 1, &uValue);

        uValue &= OOBIP_SPECTRAL_INV_MSK;
        if (uValue == OOBIP_SPECTRAL_INV_MSK)
        {
            *peSI = tmFrontEndSpecInvOn;
        }
        else
        {
            *peSI = tmFrontEndSpecInvOff;
        }

        (void)OOBIpMutexRelease(pObj);
    }

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslOOBIp_GetSI(0x%08X) failed.", tUnit));
    
    return err;
}

/*============================================================================*/
/* tmbslOOBIp_GetMER                                                           */
/*============================================================================*/
tmErrorCode_t
tmbslOOBIp_GetMER
(
    tmUnitSelect_t         tUnit,
    tmFrontEndFracNb32_t   *psMER
)
{
    tmOOBIpObject_t    *pObj = Null;
    tmErrorCode_t       err = OOBIP_ERR_NOT_INITIALIZED;
    UInt32              ulIndex;
    tmFrontEndFracNb8_t sI;
    tmFrontEndFracNb8_t sQ;
    UInt8               uValue;

    /* check input parameters */
    err = OOBIpGetInstance(tUnit, &pObj);

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslOOBIp_GetMER(0x%08X)", tUnit);
    
    if (err == TM_OK)
    {
        err = OOBIpMutexAcquire(pObj, OOBIP_MUTEX_TIMEOUT);
    }
    
    if (err == TM_OK)
    {
        if (pObj->eHwSample == OOBIP_HwSampleVersion_ES1) /* temporary: to be adapted when ES1 is removed */
        {
            pObj->uIMean = 16;
            pObj->uQMean = 16;

            psMER->lInteger = (pObj->uIMean*pObj->uIMean + pObj->uQMean*pObj->uQMean)*10;

            for( ulIndex = 0 ; ulIndex<pObj->uMERDepth ; ulIndex++ )
            {
                err = tmbslOOBIp_GetIQ(tUnit, &sI, &sQ);

                if (sI.lInteger >= 0)
                {
                    pObj->puMERError[pObj->uMERElementToUpdate] += (pObj->uIMean-sI.lInteger)*(pObj->uIMean-sI.lInteger);
                }
                else
                {
                    pObj->puMERError[pObj->uMERElementToUpdate] +=(pObj->uIMean+sI.lInteger)*(pObj->uIMean+sI.lInteger);
                }

                if (sQ.lInteger >= 0)
                {
                    pObj->puMERError[pObj->uMERElementToUpdate] += (pObj->uQMean-sQ.lInteger)*(pObj->uQMean-sQ.lInteger);
                }
                else
                {
                    pObj->puMERError[pObj->uMERElementToUpdate] += (pObj->uQMean+sQ.lInteger)*(pObj->uQMean+sQ.lInteger);
                }
            }

            pObj->puMERError[pObj->uMERElementToUpdate] /= (pObj->uMERDepth/10);

            psMER->uDivider = 0;
            for( ulIndex=0; ulIndex<OOBIP_MER_TABLE_SIZE; ulIndex++)
            {
                psMER->uDivider += pObj->puMERError[ulIndex];
            }
            psMER->uDivider /= OOBIP_MER_TABLE_SIZE;

            pObj->uMERElementToUpdate++;
            if (pObj->uMERElementToUpdate == OOBIP_MER_TABLE_SIZE)
            {
                pObj->uMERElementToUpdate = 0;
            }
        }
        else if (pObj->eHwSample == OOBIP_HwSampleVersion_ES2)
        {
            psMER->lInteger = 16384;
            /* freeze the registers*/
            uValue = 0xC4;
            err = OOBIpWrite(tUnit, OOBIP_RD_FRZ_VERSION_IND, 1, &uValue);
            if (err == TM_OK)
            {
                err = OOBIpRead(tUnit, OOBIP_MSE_ACC_MSB_IND, 1, &uValue);
            }
            if (err == TM_OK)
            {
                psMER->uDivider = (uValue & OOBIP_MSE_ACC_MSK)<<8;
                err = OOBIpRead(tUnit, OOBIP_MSE_ACC_LSB_IND, 1, &uValue);
            }
            if (err == TM_OK)
            {
                psMER->uDivider += (uValue & OOBIP_MSE_ACC_MSK);
            }
        }
        else
        {
            err = OOBIP_ERR_NOT_SUPPORTED;
        }

        if( (err == TM_OK) && (psMER->uDivider == 0) )
        {
            psMER->lInteger = 1000;
            psMER->uDivider = 1;
        }

        (void)OOBIpMutexRelease(pObj);
    }

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslOOBIp_GetMER(0x%08X) failed.", tUnit));

    return err;
}

/*============================================================================*/
/* tmbslOOBIp_GetUncors                                                        */
/*============================================================================*/
tmErrorCode_t
tmbslOOBIp_GetUncors
(
    tmUnitSelect_t         tUnit,
    UInt32                 *puUncors,
    UInt32                 *puCorrected
)
{
    tmOOBIpObject_t    *pObj = Null;
    tmErrorCode_t       err = OOBIP_ERR_NOT_INITIALIZED;
    UInt8               uValue;

    /* check input parameters */
    err = OOBIpGetInstance(tUnit, &pObj);

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslOOBIp_GetUncors(0x%08X)", tUnit);
    
    if (err == TM_OK)
    {
        err = OOBIpMutexAcquire(pObj, OOBIP_MUTEX_TIMEOUT);
    }
    
    if (err == TM_OK)
    {
        err = OOBIpRead(tUnit, OOBIP_STAT1_IND, 1, &uValue);

        if (err == TM_OK)
        {
            if ((uValue & OOBIP_OOB_DFAIL_ERR_MSK) == OOBIP_OOB_DFAIL_ERR_MSK)
            {
                pObj->uUncors += 1;
            }

            if ((uValue & OOBIP_OOB_RS_ERR_MSK) == OOBIP_OOB_RS_ERR_MSK)
            {
                pObj->uCorrected += 1;
            }

            /* Reset the DFAIL_ERR flag */
            err = OOBIpWriteBit(tUnit, OOBIP_STAT1_IND, OOBIP_OOB_DFAIL_ERR_MSK, OOBIP_OOB_DFAIL_ERR_MSK);
        }

        if (err == TM_OK)
        {
            /* Reset the RS_ERR flag */
            err = OOBIpWriteBit(tUnit, OOBIP_STAT1_IND, OOBIP_OOB_RS_ERR_MSK, OOBIP_OOB_RS_ERR_MSK);
        }

        if (err == TM_OK)
        {
            *puUncors = pObj->uUncors;
            *puCorrected = pObj->uCorrected;
        }

        (void)OOBIpMutexRelease(pObj);
    }

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslOOBIp_GetUncors(0x%08X) failed.", tUnit));

    return err;
}

/*============================================================================*/
/* tmbslOOBIp_ClearUncors                                                      */
/*============================================================================*/
tmErrorCode_t
tmbslOOBIp_ClearUncors
(
    tmUnitSelect_t         tUnit
)
{
    tmOOBIpObject_t    *pObj = Null;
    tmErrorCode_t       err = OOBIP_ERR_NOT_INITIALIZED;

    /* check input parameters */
    err = OOBIpGetInstance(tUnit, &pObj);

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslOOBIp_ClearUncors(0x%08X)", tUnit);
    
    if (err == TM_OK)
    {
        err = OOBIpMutexAcquire(pObj, OOBIP_MUTEX_TIMEOUT);
    }
    
    if (err == TM_OK)
    {
        /* Reset the DFAIL_ERR flag */
        err = OOBIpWriteBit(tUnit, OOBIP_STAT1_IND, OOBIP_OOB_DFAIL_ERR_MSK, OOBIP_OOB_DFAIL_ERR_MSK);
        
        if (err == TM_OK)
        {
            /* Reset the RS_ERR flag */
            err = OOBIpWriteBit(tUnit, OOBIP_STAT1_IND, OOBIP_OOB_RS_ERR_MSK, OOBIP_OOB_RS_ERR_MSK);
        }

        if (err == TM_OK)
        {
            pObj->uUncors = 0;
            pObj->uCorrected = 0;
        }

        (void)OOBIpMutexRelease(pObj);
    }

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslOOBIp_ClearUncors(0x%08X) failed.", tUnit));

    return err;
}

/*============================================================================*/
/* tmbslOOBIp_GetIQ                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslOOBIp_GetIQ
(
    tmUnitSelect_t       tUnit,
    tmFrontEndFracNb8_t  *psI,
    tmFrontEndFracNb8_t  *psQ
)
{
    tmOOBIpObject_t    *pObj = Null;
    tmErrorCode_t       err = OOBIP_ERR_NOT_INITIALIZED;
    UInt8               uValue;

    /* check input parameters */
    err = OOBIpGetInstance(tUnit, &pObj);

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslOOBIp_GetIQ(0x%08X)", tUnit);

    /* initialize to handle errors */
    psI->lInteger = 0;
    psI->uDivider = 1;
    psQ->lInteger = 0;
    psQ->uDivider = 1;

    if (err == TM_OK)
    {
        err = OOBIpMutexAcquire(pObj, OOBIP_MUTEX_TIMEOUT);
    }
    
    if (err == TM_OK)
    {
        /* Update the I Q registers */
        uValue = 0;
        err = OOBIpWrite(tUnit, OOBIP_CONSTELLATION_I_IND, 1, &uValue);

        if (err == TM_OK)
        {
            /* Read I */
            err = OOBIpRead(tUnit, OOBIP_CONSTELLATION_I_IND, 1, &uValue);
        }
        
        if (err == TM_OK)
        {
            psI->lInteger = (Int8)(0xC0*((uValue & OOBIP_CONSTELLATION_MSK)>>5) + uValue);
            psI->uDivider = (OOBIP_CONSTELLATION_MSK-1)/2;

            /* Read Q */
            err = OOBIpRead(tUnit, OOBIP_CONSTELLATION_Q_IND, 1, &uValue);
        }

        if (err == TM_OK)
        {
            psQ->lInteger = (Int8)(0xC0*((uValue & OOBIP_CONSTELLATION_MSK)>>5) + uValue);
            psQ->uDivider = (OOBIP_CONSTELLATION_MSK-1)/2;
        }

        (void)OOBIpMutexRelease(pObj);
    }

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslOOBIp_GetIQ(0x%08X) failed.", tUnit));

    return err;
}

/*============================================================================*/
/* tmbslOOBIp_SetClocks                                                        */
/*============================================================================*/
tmErrorCode_t
tmbslOOBIp_SetClocks
(
    tmUnitSelect_t       tUnit,
    UInt32               uSamplingClock
)
{
    tmOOBIpObject_t    *pObj = Null;
    tmErrorCode_t       err = OOBIP_ERR_NOT_INITIALIZED;

    /* check input parameters */
    err = OOBIpGetInstance(tUnit, &pObj);

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslOOBIp_SetClocks(0x%08X)", tUnit);
    
    if (err == TM_OK)
    {
        err = OOBIpMutexAcquire(pObj, OOBIP_MUTEX_TIMEOUT);
    }
    
    if (err == TM_OK)
    {
        pObj->sConfig.uSamplingClock = uSamplingClock;

        (void)OOBIpMutexRelease(pObj);
    }

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslOOBIp_SetClocks(0x%08X) failed.", tUnit));

    return err;
}

/*============================================================================*/
/* tmbslOOBIp_GetClocks                                                        */
/*============================================================================*/
tmErrorCode_t
tmbslOOBIp_GetClocks
(
    tmUnitSelect_t       tUnit,
    UInt32               *puSamplingClock
)
{
    tmOOBIpObject_t    *pObj = Null;
    tmErrorCode_t       err = OOBIP_ERR_NOT_INITIALIZED;

    /* check input parameters */
    err = OOBIpGetInstance(tUnit, &pObj);

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslOOBIp_GetClocks(0x%08X)", tUnit);
    
    if (err == TM_OK)
    {
        err = OOBIpMutexAcquire(pObj, OOBIP_MUTEX_TIMEOUT);
    }
    
    if (err == TM_OK)
    {
        *puSamplingClock = pObj->sConfig.uSamplingClock;

        (void)OOBIpMutexRelease(pObj);
    }

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslOOBIp_GetClocks(0x%08X) failed.", tUnit));

    return err;
}

/*============================================================================*/
/* tmbslOOBIp_SetPODInterfaceMode                                              */
/*============================================================================*/
extern tmErrorCode_t
tmbslOOBIp_SetPODInterfaceMode
(
    tmUnitSelect_t tUnit,
    OOBIP_PODIfcMode_t ePODIfcMode
)
{
    tmOOBIpObject_t    *pObj = Null;
    tmErrorCode_t       err = OOBIP_ERR_NOT_INITIALIZED;
    UInt8               uValue = 0;

    /* check input parameters */
    err = OOBIpGetInstance(tUnit, &pObj);

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslOOBIp_SetPODInterfaceMode(0x%08X)", tUnit);
    
    if (err == TM_OK)
    {
        err = OOBIpMutexAcquire(pObj, OOBIP_MUTEX_TIMEOUT);
    }
    
    if (err == TM_OK)
    {
        switch (ePODIfcMode)
        {
        case OOBIP_PODIfcModeDisabled:
            uValue &= !OOBIP_OOB_POD_IFC_MODE_BIT;
            pObj->sConfig.ePODIfcMode = OOBIP_PODIfcModeDisabled;
            break;
        case OOBIP_PODIfcModeEnabled:
            uValue |= OOBIP_OOB_POD_IFC_MODE_BIT;
            pObj->sConfig.ePODIfcMode = OOBIP_PODIfcModeEnabled;
            break;
        default:
            err = OOBIP_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = OOBIpWriteBit(tUnit, OOBIP_MODE_IND, OOBIP_OOB_POD_IFC_MODE_MSK, uValue);
        }

        (void)OOBIpMutexRelease(pObj);
    }

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslOOBIp_SetPODInterfaceMode(0x%08X) failed.", tUnit));

    return err;
}

/*============================================================================*/
/* tmbslOOBIp_GetPODInterfaceMode                                              */
/*============================================================================*/
extern tmErrorCode_t
tmbslOOBIp_GetPODInterfaceMode
(
    tmUnitSelect_t tUnit,
    OOBIP_PODIfcMode_t *pePODIfcMode
)
{
    UInt8               uValue = 0;
    tmOOBIpObject_t    *pObj = Null;
    tmErrorCode_t       err = OOBIP_ERR_NOT_INITIALIZED;

    /* check input parameters */
    err = OOBIpGetInstance(tUnit, &pObj);

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslOOBIp_GetPODInterfaceMode(0x%08X)", tUnit);
    
    if (err == TM_OK)
    {
        err = OOBIpMutexAcquire(pObj, OOBIP_MUTEX_TIMEOUT);
    }
    
    if (err == TM_OK)
    {
        err = OOBIpRead(tUnit, OOBIP_MODE_IND, 1, &uValue);

        uValue &= OOBIP_OOB_POD_IFC_MODE_MSK;

        if (uValue == OOBIP_OOB_POD_IFC_MODE_BIT)
        {
            *pePODIfcMode = OOBIP_PODIfcModeEnabled;
        }
        else
        {
            *pePODIfcMode = OOBIP_PODIfcModeDisabled;
        }

        (void)OOBIpMutexRelease(pObj);
    }

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslOOBIp_GetPODInterfaceMode(0x%08X) failed.", tUnit));

    return err;
}

/*============================================================================*/
/* tmbslOOBIp_SetStartAddressOffset                                           */
/*============================================================================*/
extern tmErrorCode_t
tmbslOOBIp_SetStartAddressOffset
(
    tmUnitSelect_t tUnit,
    UInt16         uStartAddress
)
{
    tmOOBIpObject_t    *pObj = Null;
    tmErrorCode_t       err = OOBIP_ERR_NOT_INITIALIZED;

    /* check input parameters */
    err = OOBIpGetInstance(tUnit, &pObj);

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslOOBIp_SetStartAddressOffset(0x%08X)", tUnit);
    
    if (err == TM_OK)
    {
        pObj->uStartAddress = uStartAddress;
    }

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslOOBIp_SetStartAddressOffset(0x%08X) failed.", tUnit));

    return err;
}

/*============================================================================*/
/*                   STATIC FUNCTIONS DEFINITIONS                             */
/*============================================================================*/

/*============================================================================*/
/*                   I2C FUNCTIONS                                            */
/*============================================================================*/

/*============================================================================*/
/* OOBIpWrite                                                              */
/*============================================================================*/
tmErrorCode_t
OOBIpWrite
(
    tmUnitSelect_t tUnit,
    UInt8          uIndex_U,
    UInt32         uNBytes_U,
    UInt8*         puData_U
)
{
    tmOOBIpObject_t    *pObj = Null;
    tmErrorCode_t       err = OOBIP_ERR_NOT_INITIALIZED;
    UInt16              i2c_index = 0;
    UInt8               i2c_data[500];
    UInt32              ulIndex;

    /* check input parameters */
    err = OOBIpGetInstance(tUnit, &pObj);

    tmDBGPRINTEx(DEBUGLVL_BLAB, "OOBIpWrite(0x%08X)", tUnit);
    
    if (err == TM_OK)
    {
        i2c_index = uIndex_U * 2 + pObj->uStartAddress;
        
        for( ulIndex=0 ; ulIndex<uNBytes_U ; ulIndex++)
        {
            i2c_data[2*ulIndex] = puData_U[ulIndex];
            i2c_data[2*ulIndex+1] = 0;
        }

        err = POBJ_SRVFUNC_SIO.Write (tUnit, 2, (UInt8*)&i2c_index, 2*uNBytes_U, i2c_data);
    }

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "OOBIpWrite(0x%08X) failed.", pObj->tUnit));

    return err;
}

/*============================================================================*/
/* OOBIpWriteBit                                                           */
/*============================================================================*/
tmErrorCode_t
OOBIpWriteBit
(
    tmUnitSelect_t tUnit,
    UInt8          uIndex_U,
    UInt32         uMask_U,
    UInt8          uData_U
)
{
    tmOOBIpObject_t    *pObj = Null;
    tmErrorCode_t       err = OOBIP_ERR_NOT_INITIALIZED;
    UInt8               byte;

    err = OOBIpGetInstance(tUnit, &pObj);

    tmDBGPRINTEx(DEBUGLVL_BLAB, "OOBIpWriteBit(0x%08X)", tUnit);
    
    if (err == TM_OK)
    {
        err = OOBIpRead(tUnit, uIndex_U, 1, &byte);
    }
    
    if (err == TM_OK)
    {
        byte = (~(UInt8)uMask_U & byte) | ((UInt8)uMask_U & (UInt8)uData_U);
        err = OOBIpWrite(tUnit, uIndex_U, 1, &byte);
    }
    
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "OOBIpWriteBit(0x%08X) failed.", pObj->tUnit));

    return err;
}

/*============================================================================*/
/* OOBIpRead                                                               */
/*============================================================================*/
tmErrorCode_t
OOBIpRead
(
    tmUnitSelect_t tUnit,
    UInt8          uIndex_U,
    UInt32         uNBytes_U,
    UInt8*         puData_U
)
{
    tmOOBIpObject_t    *pObj = Null;
    tmErrorCode_t       err = OOBIP_ERR_NOT_INITIALIZED;
    UInt16              i2c_index = 0;
    UInt8               i2c_data[500];
    UInt32              ulIndex;

    /* check input parameters */
    err = OOBIpGetInstance(tUnit, &pObj);

    tmDBGPRINTEx(DEBUGLVL_BLAB, "OOBIpRead(0x%08X)", tUnit);
    
    if (err == TM_OK)
    {
        i2c_index = uIndex_U * 2 + pObj->uStartAddress;

        err = POBJ_SRVFUNC_SIO.Read (tUnit, 2, (UInt8*)&i2c_index, 2*uNBytes_U, i2c_data);
    }

    if (err == TM_OK)
    {
        for( ulIndex=0 ; ulIndex<uNBytes_U ; ulIndex++)
        {
            puData_U[ulIndex] = i2c_data[2*ulIndex];
        }
    }

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "OOBIpRead(0x%08X) failed.", pObj->tUnit));

    return err;
}

/*============================================================================*/
/* OOBIpWait                                                               */
/*============================================================================*/
tmErrorCode_t
OOBIpWait
(
    tmUnitSelect_t tUnit,
    UInt32         uTime
)
{
    tmOOBIpObject_t    *pObj = Null;
    tmErrorCode_t       err = OOBIP_ERR_NOT_INITIALIZED;

    /* check input parameters */
    err = OOBIpGetInstance(tUnit, &pObj);

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "OOBIpWait(0x%08X)", tUnit);
    
    if (err == TM_OK)
    {
        err = POBJ_SRVFUNC_STIME.Wait (tUnit, uTime);
    }

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "OOBIpWait(0x%08X) failed.", pObj->tUnit));

    return err;
}

/*============================================================================*/
/* OOBIpMutexAcquire                                                       */
/*============================================================================*/
extern tmErrorCode_t
OOBIpMutexAcquire
(
    tmOOBIpObject_t *pObj,
    UInt32              timeOut
)
{
    tmErrorCode_t   err = TM_OK;

    if(pObj->sMutex.Acquire != Null && pObj->pMutex != Null)
    {
        err = pObj->sMutex.Acquire(pObj->pMutex, timeOut);
    }
    
    return err;
}

/*============================================================================*/
/* OOBIpMutexRelease                                                       */
/*============================================================================*/
extern tmErrorCode_t
OOBIpMutexRelease
(
    tmOOBIpObject_t *pObj
)
{
    tmErrorCode_t   err = TM_OK;

    if(pObj->sMutex.Release != Null && pObj->pMutex != Null)
    {
        err = pObj->sMutex.Release(pObj->pMutex);
    }
    
    return err;
}
