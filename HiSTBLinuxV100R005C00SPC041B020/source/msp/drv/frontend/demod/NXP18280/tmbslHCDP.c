/*
  Copyright (C) 2010 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmbslHCDP.c
 *
 *                %version: 11 %
 *
 * \date          %modify_time%
 *
 * \author        Alexandre TANT
 *
 * \brief         Describe briefly the purpose of this file.
 *
 * REFERENCE DOCUMENTS :
 *                
 *
 * TVFE SW Arch V4 Template: Author Christophe CAZETTES
 *
 * \section info Change Information
 *
*/
#define TMBSL_HCDP_NO_64BITS

/*============================================================================*/
/* Standard include files:                                                    */
/*============================================================================*/
#include "tmNxTypes.h"
#include "tmCompId.h"
#include "tmFrontEnd.h"
#include "tmbslFrontEndTypes.h"

/*============================================================================*/
/* Project include files:                                                     */
/*============================================================================*/
#include "tmbslHCDP.h"

#include "tmbslHCDP_Reg.h"
#include "tmbslHCDP_Local.h"
#include "tmbslHCDP_Instance.h"
#include "tmbslHCDP_cfg.h"

#define BITS32_MAX 0xFFFFFFFF

/*============================================================================*/
/* Static Variable:                                                           */
/*============================================================================*/
static HCDP_LogTable_t sLogTable[] = { HCDP_LOG_TABLE };
static UInt16 uLogTableMaxIndex = sizeof(sLogTable)/(2*sizeof(UInt32))-1;

/*============================================================================*/
/* Static internal functions:                                                 */
/*============================================================================*/
static tmErrorCode_t
iHCDP_SetMatchedFilter(pHCDPObject_t pObj);

#ifdef NOT_IMPLEMENTED
static tmErrorCode_t
iHCDPSetAntiPulseEnhance(pHCDPObject_t pObj);
#endif

static tmErrorCode_t
iHCDP_ResetCdp(pHCDPObject_t pObj, UInt16 mode);

static tmErrorCode_t
iHCDP_ConfigCTL(pHCDPObject_t pObj);

static tmErrorCode_t
iHCDP_CheckDemodLocked(pHCDPObject_t pObj, tmbslFrontEndState_t *peStatus);

tmErrorCode_t
iHCDP_CheckFECLocked(pHCDPObject_t pObj, tmbslFrontEndState_t *peStatus);

static tmErrorCode_t
iHCDP_GetEsNo( pHCDPObject_t pObj, tmFrontEndFracNb32_t  *pEsno);

static tmErrorCode_t
iHCDP_StartBER( pHCDPObject_t pObj);

static tmErrorCode_t
iHCDP_GetBER(pHCDPObject_t pObj, tmFrontEndFracNb32_t *psBER, UInt32 *puUncors);

static tmErrorCode_t
iHCDP_StopBER(pHCDPObject_t pObj);

static tmErrorCode_t
iHCDP_GetAnxBInterleaverMode(pHCDPObject_t pObj, HCDP_InterleaverMode_t *peInterleaverMode);

#ifdef NOT_IMPLEMENTED
static tmErrorCode_t
iHCDP_Write( pHCDPObject_t pObj, UInt16 uIndex_U, UInt32 uNBytes_U, UInt16* puData_U );
static tmErrorCode_t 
iHCDP_Read( pHCDPObject_t pObj, UInt16 uIndex_U, UInt32 uNBytes_U, UInt16* puData_U );
static tmErrorCode_t
iHCDP_WriteBit( pHCDPObject_t pObj, UInt16 uIndex_U, UInt16 uMask_U, UInt16 uData_U );
#endif

//#define TMBSL_HCDP_NO_64BITS
#ifdef TMBSL_HCDP_NO_64BITS
static UInt32 iHCDP_Division32( UInt32 ulNum1, UInt32 ulNum2, UInt32 ulDenom );
#endif

static tmErrorCode_t
iHCDP_SetExtendSymbolRateRange(
    pHCDPObject_t pObj,
    HCDP_ExtendSymbolRateMode_t eMode
);

/*============================================================================*/
/* Exported functions:                                                        */
/*============================================================================*/

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_Init                                                */
/*                                                                            */
/* DESCRIPTION: Initialises the driver.                                       */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_Init(
 tmUnitSelect_t tUnit,
 const tmbslFrontEndDependency_t *psSrvFunc
 )
{
    pHCDPObject_t     pObj = Null;
    tmErrorCode_t     err = TM_OK;

    /* Test parameter(s) */
    if (psSrvFunc == Null)
    {
        err = HCDP_ERR_BAD_PARAMETER;
    }
    
    if (err == TM_OK)
    {
        /* Get a driver instance */
        err = iHCDP_GetInstance(tUnit, &pObj);
    }

    /* Check driver instance state */
    if(err == TM_OK || err == HCDP_ERR_ERR_NO_INSTANCES)
    {
        if(P_OBJ_VALID && pObj->init == True)
        {
            err = HCDP_ERR_ALREADY_SETUP;
        }
        else 
        {
            if(P_OBJ_VALID == False)
            {
                /* Try to allocate an instance of the driver */
                err = iHCDP_AllocInstance(tUnit, &pObj);
                if(err != TM_OK || pObj == Null)
                {
                    err = HCDP_ERR_ERR_NO_INSTANCES;
                }
            }

            if(err == TM_OK)
            {
                /* Initialize the Object by default values */
                P_SIO = P_FUNC_SIO(psSrvFunc);
                P_STIME = P_FUNC_STIME(psSrvFunc);
                P_SDEBUG = P_FUNC_SDEBUG(psSrvFunc);
                pObj->eHwCdpSample = HCDP_SAMPLE_UNKNOWN;

#ifdef _TVFE_IMPLEMENT_MUTEX
                if(    P_FUNC_SMUTEX_OPEN_VALID(psSrvFunc)
                    && P_FUNC_SMUTEX_CLOSE_VALID(psSrvFunc)
                    && P_FUNC_SMUTEX_ACQUIRE_VALID(psSrvFunc)
                    && P_FUNC_SMUTEX_RELEASE_VALID(psSrvFunc) )
                {
                    P_SMUTEX = psSrvFunc->sMutex;

                    err = P_SMUTEX_OPEN(&P_MUTEX);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "Mutex_Open(0x%08X) failed.", tUnit));
                }
#endif

                tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslHCDP_Init(0x%08X)", tUnit);

                if(err == TM_OK)
                {
                    pObj->init = True;
                    pObj->bReset = False;
                }
            }
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_DeInit                                              */
/*                                                                            */
/* DESCRIPTION: Deinitialises the driver.                                     */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_DeInit(
    tmUnitSelect_t tUnit   /* I: FrontEnd unit number */
)
{
    pHCDPObject_t   pObj = Null;
    tmErrorCode_t   err = TM_OK;

    /* Get a driver instance */
    err = iHCDP_GetInstance(tUnit, &pObj);

    if (err == TM_OK)
    {
#ifdef _TVFE_IMPLEMENT_MUTEX
        /* Try to acquire driver mutex */
        err = iHCDP_MutexAcquire(pObj, HCDP_MUTEX_TIMEOUT);

        if(err == TM_OK)
        {
#endif
            tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslHCDP_DeInit(0x%08X)", tUnit);

#ifdef _TVFE_IMPLEMENT_MUTEX
            P_SMUTEX_ACQUIRE = Null;

            /* Release driver mutex */
            (void)iHCDP_MutexRelease(pObj);

            if(P_SMUTEX_CLOSE_VALID && P_MUTEX_VALID)
            {
                err = P_SMUTEX_CLOSE(P_MUTEX);
            }

            P_SMUTEX_OPEN = Null;
            P_SMUTEX_CLOSE = Null;
            P_SMUTEX_RELEASE = Null;

            P_MUTEX = Null;
        }
#endif

        err = iHCDP_DeAllocInstance(pObj);
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_GetSWVersion                                        */
/*                                                                            */
/* DESCRIPTION: Gets the driver version.                                      */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_GetSWVersion(
    ptmSWVersion_t pSWVersion
)
{
    pSWVersion->compatibilityNr = HCDP_COMP_NUM;
    pSWVersion->majorVersionNr  = HCDP_MAJOR_VER;
    pSWVersion->minorVersionNr  = HCDP_MINOR_VER;

    return TM_OK;
}  

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_GetCdpHwSample                                      */
/*                                                                            */
/* DESCRIPTION: Gets the silicon die version (ES1 or ES2).                    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_GetCdpHwSample(
    tmUnitSelect_t tUnit,
    HCDP_HwCdpSample_t *peCdpHwSample
)
{
    pHCDPObject_t pObj = Null;
    tmErrorCode_t err = TM_OK;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);

    if (err == TM_OK)
    {
        *peCdpHwSample = pObj->eHwCdpSample;
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_Reset                                               */
/*                                                                            */
/* DESCRIPTION: Resets the hardware.                                          */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_Reset(
 tmUnitSelect_t tUnit
 )
{
    pHCDPObject_t pObj = Null;
    tmErrorCode_t err = TM_OK;
    UInt16 uValue = 0;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(HCDP)

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslHCDP_Reset(0x%08X)", tUnit);

    if (err == TM_OK)
    {
        pObj->sCurrentChannel.uIF = 0;
        pObj->sCurrentChannel.uSR = 0;
        pObj->sCurrentChannel.eExtendedSRModeCurrent = HCDP_ExtendSymbolRateModeDisable; /* disabled by default after reset */
        pObj->sCurrentChannel.eExtendedSRModeNew = HCDP_ExtendSymbolRateModeDisable; /* disabled by default after reset */
        pObj->sCurrentChannel.eFECMode = tmFrontEndFECModeMax;
        pObj->sCurrentChannel.eMod = tmFrontEndModulationMax;
        pObj->sCurrentChannel.eSI = tmFrontEndSpecInvMax;

        /* perform a full CDP reset (default registers + reset) */
        err = iHCDP_ResetCdp(pObj, HCDP_ResetMode_Full);
    }

    /* Differenciate ES1/ES2/ES3 to support specific settings */
    if (err == TM_OK)
    {
        err = iHCDP_RegRead(pObj, HCDPRegCdpVersion, &uValue);
        if (err == TM_OK)
        {
            switch (uValue) /* temporary: to be adapted when ES1 is removed */
            {
            case 0x0301:
                pObj->eHwCdpSample = HCDP_SAMPLE_ES1;
                break;
            case 0x0910:
                pObj->eHwCdpSample = HCDP_SAMPLE_ES2;
                break;
            case 0x0303:
                pObj->eHwCdpSample = HCDP_SAMPLE_ES3;
                break;
            default:
                pObj->eHwCdpSample = HCDP_SAMPLE_UNKNOWN;
                err = HCDP_ERR_BAD_VERSION;
            }
        }
    }

    /* ADC Staggered Sampling enabled, thus do not use AdcFifoFreqWord */
    if (err == TM_OK)
    {
        err = iHCDP_RegWrite(pObj, HCDPRegAdcStaggeredSampling, 0x1);
    }

    /* for ES2: allows the Cagc accumulator to be set back to a medium value in case of saturation */
    if (err == TM_OK)
    {
        err = iHCDP_RegWrite(pObj, HCDPRegCagcSatResetEn, 0x1);
    }
    if (err == TM_OK)
    {
        err = iHCDP_RegWrite(pObj, HCDPRegCagcOutGain, 0x5);
    }
    /* CagcBandwidth changed from 1 to 3 for hum (and slightly CCI) */
    if (err == TM_OK)
    {
        err = iHCDP_RegWrite(pObj, HCDPRegCagcBandwidth, 0x3);
    }

    /* config timer */
    /* ES1 : 0.455 msec with DCLK=144MHz or 0.410 msec with DCLK=160MHz */
    /* ES2 : 0.404 msec with DCLK=162MHz */
    if (err == TM_OK)
    {
        err = iHCDP_RegWrite(pObj,HCDPRegTimersTicDivider,0xFFFF);
    }

    /* lock driven count */
    if (err == TM_OK)
    {
        err = iHCDP_RegWrite(pObj,HCDPRegTimersMode,0x0);
    }

    if (err == TM_OK)
    {
        if (pObj->eHwCdpSample == HCDP_SAMPLE_ES1) /* temporary: to be adapted when ES1 is removed */
        {
            /* count the first event. Warning: this mode always fails for 64QAM with ES1 */
            err = iHCDP_RegWrite(pObj,HCDPRegTimersXmode,0x0);
        }
        else if ((pObj->eHwCdpSample == HCDP_SAMPLE_ES2)
              || (pObj->eHwCdpSample == HCDP_SAMPLE_ES3))
        {
            /* count the last event. Works for ES2 only */
            err = iHCDP_RegWrite(pObj,HCDPRegTimersXmode,0xF);
        }
        else
        {
            err = HCDP_ERR_BAD_VERSION;
        }
    }

    /* count time not symbols */
    if (err == TM_OK)
    {
        err = iHCDP_RegWrite(pObj,HCDPRegTimersSmode,0);
    }

    if (err == TM_OK)
    {
        err = iHCDP_RegWrite(pObj,HCDPRegDemodTestEn,0);
    }

    /* disable the agc saturation thresholds */
    if (err == TM_OK)
    {
        err = iHCDP_RegWrite(pObj,HCDPRegAagcSatEmThr,63);
    }

    if (err == TM_OK)
    {
        err = iHCDP_RegWrite(pObj,HCDPRegAagcPreSatEmThr,63);
    }

    if (pObj->eHwCdpSample == HCDP_SAMPLE_ES1) /* temporary: to be adapted when ES1 is removed */
    {
        /* Timeout on read and write are too short for low symbol rates */
        if (err == TM_OK)
        {
            err = iHCDP_RegWrite(pObj, HCDPRegMiMaxWrWait, 0xFF);
        }

        if (err == TM_OK)
        {
            err = iHCDP_RegWrite(pObj, HCDPRegMiMinWrWait, 0x40);
        }

        if (err == TM_OK)
        {
            err = iHCDP_RegWrite(pObj, HCDPRegMiMaxRdWait, 0xFF);
        }

        if (err == TM_OK)
        {
            err = iHCDP_RegWrite(pObj, HCDPRegMiMinRdWait, 0x40);
        }
    }

    if (err == TM_OK)
    {
        /* disable T/2 mode */
        err = iHCDP_RegWrite(pObj,HCDPRegEqTspaceMode,1);
    }

    if (err == TM_OK)
    {
        err = iHCDP_RegWrite(pObj,HCDPRegPdfGain, 1);
    }

    if (err == TM_OK)
    {
        err = iHCDP_RegWrite(pObj,HCDPRegCtlPhaseEmSel, 1);
    }

    if (err == TM_OK)
    {
        err = iHCDP_RegWrite(pObj,HCDPRegCtlFreqEmSel, 0);
    }

    if (err == TM_OK)
    {
        err = iHCDP_RegWrite(pObj,HCDPRegStlLockPol, 0);
    }

    /* StlAcqGain */
    if (err == TM_OK)
    {
        err = iHCDP_RegWrite(pObj, HCDPRegStlAcqDirGain, 0x6);
    }

    if (err == TM_OK)
    {
        if (0 != pObj->sConfig.uIFAGCThreshold)
        {
            err = iHCDP_RegWrite(pObj,HCDPRegAagcEmThr0, pObj->sConfig.uIFAGCThreshold);
        }
    }

    if (err == TM_OK)
    {
        err = iHCDP_RegWrite(pObj,HCDPRegAagcSdRate0, 2);
    }

    if (err == TM_OK)
    {
        err = iHCDP_RegWrite(pObj,HCDPRegAagcBw0, 7);
    }

    if (err == TM_OK)
    {
        /* force AagcIfAttnSd output to 0 */
        err = iHCDP_RegWrite(pObj,HCDPRegAagcIfAttnSdSw, 3);
    }

    if (err == TM_OK)
    {
        /* force AagcRfSd output to 0 */
        err = iHCDP_RegWrite(pObj,HCDPRegAagcRfSdSw, 3);
    }

    /* initialise PNBER stopped */
    if (err == TM_OK)
    {
        err = iHCDP_RegWrite(pObj,HCDPRegPnBerStart,0);
    }
    if (err == TM_OK)
    {
        err = iHCDP_RegWrite(pObj,HCDPRegPnBerReset,1);
    }

    if (err == TM_OK)
    {
        err = iHCDP_RegWrite(pObj,HCDPRegPnBerMode,0);
    }

    if (err == TM_OK)
    {
        err = iHCDP_RegWrite(pObj, HCDPRegEqFbScale, 0x6);
    }

    if (err == TM_OK)
    {
        err = iHCDP_RegWrite(pObj, HCDPRegEqEfScale, 0x5);
    }

    if (err == TM_OK)
    {
        err = iHCDP_RegWrite(pObj, HCDPRegEqAcqFfAdaptGain, 0xA);
    }

    if (err == TM_OK)
    {
        err = iHCDP_RegWrite(pObj, HCDPRegEqAcqFfHsymAdaptGain, 0xA);
    }

    if (err == TM_OK)
    {
        err = iHCDP_RegWrite(pObj, HCDPRegEqTrkFfAdaptGain, 0x9);
    }

    if (err == TM_OK)
    {
        err = iHCDP_RegWrite(pObj, HCDPRegNxaDfailThr, 0xE);
    }

    if (err == TM_OK)
    {
        pObj->bReset = True;
    }

    _MUTEX_RELEASE(HCDP)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_StartLock                                           */
/*                                                                            */
/* DESCRIPTION: Starts the lock.                                              */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_StartLock(
    tmUnitSelect_t tUnit
)
{
    pHCDPObject_t pObj = Null;
    tmErrorCode_t err = TM_OK;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(HCDP)

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslHCDP_StartLock(0x%08X)", tUnit);
    
    if (err == TM_OK)
    {
        /* stop BER  computation */
        err = iHCDP_StopBER(pObj);
    }

    if (err == TM_OK)
    {
        err = iHCDP_RegWrite(pObj, HCDPRegEqFfLeakageAlpha, pObj->sConfig.uEqFfLeakageAlpha_BeforeLock);
        pObj->bSetAfterLock = (err == TM_OK ? False : True);
    }

    if (err == TM_OK)
    {
        /* reset equalizer */
        err = iHCDP_ResetCdp(pObj, HCDP_ResetMode_Eq);
    }

    if (err == TM_OK)
    {
        /* reset timers */
        err = iHCDP_RegWrite(pObj, HCDPRegTimersResetPulse,0xF);
    }

    if (err == TM_OK)
    {
        /* perform a soft reset and start acquisition */
        err = iHCDP_ResetCdp(pObj, HCDP_ResetMode_DemodSoft);
    }

    if (err == TM_OK)
    {
        /* reset FEC */
        err = iHCDP_ResetCdp(pObj, HCDP_ResetMode_FECSoft);
    }

    if (err == TM_OK)
    {
        pObj->uBaseUncors = 0;
        pObj->uPreviousUncors = 0;
        pObj->sPreviousBER.lInteger = 0;
        pObj->sPreviousBER.uDivider = 0;

        /* start BER computation in infinite window */
        err = iHCDP_StartBER(pObj);
    }

    _MUTEX_RELEASE(HCDP)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_GetLockIndicators                                   */
/*                                                                            */
/* DESCRIPTION: Gets all demod lock indicators.                               */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_GetLockIndicators(
    tmUnitSelect_t tUnit,
    HCDP_LockInd_t *psLockInd
)
{
    UInt16  uVal = 0;
    tmbslFrontEndState_t eLockStatus;

    pHCDPObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);
    
    _MUTEX_ACQUIRE(HCDP)

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslHCDP_GetLockIndicators(0x%08X)", tUnit);

    psLockInd->bCagcSat = False;
    psLockInd->bDagcSat = False;
    psLockInd->bAagc = False;
    psLockInd->bStl = False;
    psLockInd->bCtl = False;
    psLockInd->bSctl = False;
    psLockInd->bDemod = False;
    psLockInd->bFEC = False;
    psLockInd->uDemodLockTime = 0;
    psLockInd->uFecLockTime = 0;

    if (err == TM_OK)
    {
        err = iHCDP_RegRead(pObj,HCDPRegLockStatus,&uVal);
    }

    if (err == TM_OK)
    {
        if (0x400 & uVal ) 
        {
            psLockInd->bCagcSat = True;
        }

        if (0x200 & uVal ) 
        {
            psLockInd->bDagcSat = True;
        }

        if (0x40 & uVal ) 
        {
            psLockInd->bAagc = True;
        }

        if (0x10 & uVal)
        {
            psLockInd->bStl = True;
        }

        if (0x8 & uVal)
        {
            psLockInd->bCtl = True;
        }

        if (0x2 & uVal)
        {
            psLockInd->bSctl = True;
        }

        if (0x187F == uVal || 0x186F == uVal)
        {
            psLockInd->bDemod = True; /*locked*/
        }

        err = iHCDP_CheckFECLocked(pObj, &eLockStatus);
    }

    if (err == TM_OK)
    {
        if (eLockStatus == tmbslFrontEndStateLocked)
        {
            psLockInd->bFEC = True; /*locked*/
        }

        err = tmbslHCDP_GetLockTime(tUnit,&(psLockInd->uDemodLockTime),&(psLockInd->uFecLockTime));
    }

    _MUTEX_RELEASE(HCDP)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_GetLockStatus                                       */
/*                                                                            */
/* DESCRIPTION: Gets lock status o the HCDP.                                  */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_GetLockStatus(
    tmUnitSelect_t tUnit,
    tmbslFrontEndState_t *peLockStatus
)
{
    tmbslFrontEndState_t eFECLockStatus;
    tmbslFrontEndState_t eDemodLockStatus;

    pHCDPObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(HCDP)

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslHCDP_GetLockStatus(0x%08X)", tUnit);

    if (err == TM_OK)
    {
        if( pObj->bReset == True )
        {
            err = iHCDP_CheckDemodLocked(pObj, &eDemodLockStatus);

            if (err == TM_OK)
            {
                err = iHCDP_CheckFECLocked(pObj, &eFECLockStatus);
            }

            if (err == TM_OK)
            {
                if ((eFECLockStatus == tmbslFrontEndStateLocked) && (eDemodLockStatus == tmbslFrontEndStateLocked))
                {
                    *peLockStatus = tmbslFrontEndStateLocked;
                }
                else
                {
                    *peLockStatus = tmbslFrontEndStateNotLocked;
                }
            }
        }
        else
        {
            /* this path has not been initialised */
            *peLockStatus = tmbslFrontEndStateNotLocked;
        }
    }

    _MUTEX_RELEASE(HCDP)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_GetAAGCAcc                                          */
/*                                                                            */
/* DESCRIPTION: Gets the demod IF & RF AGC accumulator values.                */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_GetAAGCAcc(
    tmUnitSelect_t tUnit,
    UInt16 *puIfAGC,
    UInt16 *puRFAGC
)
{
    pHCDPObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);
    
    _MUTEX_ACQUIRE(HCDP)

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslHCDP_GetAAGCAcc(0x%08X)", tUnit);

    if (err == TM_OK)
    {
        err = iHCDP_RegRead(pObj,HCDPRegAagcAcc0,puIfAGC);
    }

    if (err == TM_OK)
    {
        err = iHCDP_RegRead(pObj,HCDPRegAagcAcc1,puRFAGC);
    }

    _MUTEX_RELEASE(HCDP)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_SetMod                                              */
/*                                                                            */
/* DESCRIPTION: Sets de the modulation.                                       */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_SetMod(
    tmUnitSelect_t tUnit,
    tmFrontEndModulation_t eMod
)
{
    pHCDPObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;
    UInt16 uValue;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);
    
    _MUTEX_ACQUIRE(HCDP)

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslHCDP_SetMod(0x%08X)", tUnit);

    if (err == TM_OK && pObj->sCurrentChannel.eMod != eMod)
    {
        pObj->sCurrentChannel.eMod = eMod;

        switch (eMod)
        {
        case tmFrontEndModulationQpsk:
            uValue = 0;
            break;
        case tmFrontEndModulationQam16:
            uValue = 1;
            break;
        case tmFrontEndModulationQam32:
            uValue = 2;
            break;
        case tmFrontEndModulationQam64:
            uValue = 3;
            break;
        case tmFrontEndModulationQam128:
            uValue = 4;
            break;
        case tmFrontEndModulationQam256:
            uValue = 5;
            break;
        default:
            err = HCDP_ERR_BAD_PARAMETER;
        }
 
        if (err == TM_OK)
        {
            err = iHCDP_RegWrite(pObj,HCDPRegQamMode,uValue);
        }

        switch (eMod)
        {
        case tmFrontEndModulationQam64:
            if (err == TM_OK)
            {
                err = iHCDP_RegWrite(pObj,HCDPRegAfcAccelerate, 2);
            }

            if (err == TM_OK)
            {
                err = iHCDP_RegWrite(pObj, HCDPRegEqEfLeakageAlpha, 0xE);
            }

            /* CtlAcqGain (default defined by QamMode = 0xC2) */
            if (err == TM_OK)
            {
                err = iHCDP_RegWrite(pObj, HCDPRegCtlAcqGain, HCDP_CFG_CTLACQGAIN_64QAM);
            }

            /* CtlTrkGain (default defined by QamMode = 0x142) */
            if (err == TM_OK)
            {
                err = iHCDP_RegWrite(pObj, HCDPRegCtlTrkGain, HCDP_CFG_CTLTRKGAIN_64QAM);
            }

            /* SctlTrkGain (default defined by QamMode = 0x83) */
            if (err == TM_OK)
            {
                err = iHCDP_RegWrite(pObj, HCDPRegSctlTrkGain, 0x84);
            }

            /* DagcThreshold */
            if (err == TM_OK)
            {
                err = iHCDP_RegWrite(pObj, HCDPRegDagcThreshold, 0x40);
            }
            break;

        case tmFrontEndModulationQam128:
            if (err == TM_OK)
            {
                err = iHCDP_RegWrite(pObj,HCDPRegAfcAccelerate, 2);
            }

            if (err == TM_OK)
            {
                err = iHCDP_RegWrite(pObj, HCDPRegEqEfLeakageAlpha, 0xE);
            }

            /* CtlAcqGain (default defined by QamMode = 0xC2) */
            if (err == TM_OK)
            {
                err = iHCDP_RegWrite(pObj, HCDPRegCtlAcqGain, HCDP_CFG_CTLACQGAIN_128QAM);
            }

            /* CtlTrkGain (default defined by QamMode = 0x142) */
            if (err == TM_OK)
            {
                err = iHCDP_RegWrite(pObj, HCDPRegCtlTrkGain, HCDP_CFG_CTLTRKGAIN_128QAM);
            }

            /* SctlTrkGain (default defined by QamMode = 0x83) */

            /* DagcThreshold */
            if (err == TM_OK)
            {
                err = iHCDP_RegWrite(pObj, HCDPRegDagcThreshold, 0x64);
            }

            /* overwrites the default values defined by QamMode */
            if (err == TM_OK)
            {
                err = iHCDP_RegWrite(pObj,HCDPRegAfcUseInnerCircle,0x0);
            }
            if (err == TM_OK)
            {
                err = iHCDP_RegWrite(pObj,HCDPRegAfcPhaseThr,0xFE);
            }
            if (err == TM_OK)
            {
                err = iHCDP_RegWrite(pObj,HCDPRegAfcEnergyThr,0x24D);
            }
            break;


        case tmFrontEndModulationQam256:
            if (err == TM_OK)
            {
                err = iHCDP_RegWrite(pObj,HCDPRegAfcAccelerate, 0);
            }

            if (err == TM_OK)
            {
                err = iHCDP_RegWrite(pObj, HCDPRegEqEfLeakageAlpha, 0xA);
            }

            /* CtlAcqGain (default defined by QamMode = 0xC2) */
            if (err == TM_OK)
            {
                if (pObj->sCurrentChannel.eFECMode == tmFrontEndFECModeAnnexA) /* should rather depend on SR value not FEC */
                {
                    /* for SR > 6Msym/s */
                    err = iHCDP_RegWrite(pObj, HCDPRegCtlAcqGain, HCDP_CFG_CTLACQGAIN_ANXA_256QAM);
                }
                else
                {
                    /* for SR < 6Msym/s: better lock with phase noise */
                    err = iHCDP_RegWrite(pObj, HCDPRegCtlAcqGain, HCDP_CFG_CTLACQGAIN_ANXB_256QAM);
                }
            }

            /* CtlTrkGain (default defined by QamMode = 0x182) */
            /* Annex A/C: set to 0x120 */
            /* Annex B: keeps the default value */
            if (err == TM_OK)
            {
                err = iHCDP_RegWrite(pObj, HCDPRegCtlTrkGain, HCDP_CFG_CTLTRKGAIN_256QAM);
            }

            /* SctlTrkGain (default defined by QamMode = 0x83) */
            if (err == TM_OK)
            {
                err = iHCDP_RegWrite(pObj, HCDPRegSctlTrkGain, 0x84);
            }

            /* DagcThreshold */
            if (err == TM_OK)
            {
                err = iHCDP_RegWrite(pObj, HCDPRegDagcThreshold, 0x40);
            }

            break;

        default:
            if (err == TM_OK)
            {
                err = iHCDP_RegWrite(pObj,HCDPRegAfcAccelerate, 2);
            }

            if (err == TM_OK)
            {
                err = iHCDP_RegWrite(pObj, HCDPRegEqEfLeakageAlpha, 0xF);
            }

            /* CtlAcqGain (default defined by QamMode = 0xC2) */

            /* SctlTrkGain (default defined by QamMode = 0x83) */

            /* DagcThreshold */
            if (err == TM_OK)
            {
                err = iHCDP_RegWrite(pObj, HCDPRegDagcThreshold, 0x40);
            }
            break;
        }

        if (err == TM_OK)
        {
            err = iHCDP_SetMatchedFilter(pObj);
        }

#ifdef HCDPANTI_PULSE_ENHANCE
        if (err == TM_OK)
        {
            err = HCDPSetAntiPulseEnhance(tUnit);
        }
#endif //HCDPANTI_PULSE_ENHANCE

        /* update structure pObj if error */
        if (err != TM_OK)
        {
            pObj->sCurrentChannel.eMod = tmFrontEndModulationMax;
        }
   }

    _MUTEX_RELEASE(HCDP)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_GetMod                                              */
/*                                                                            */
/* DESCRIPTION: gets the modulation.                                          */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_GetMod(
    tmUnitSelect_t tUnit,
    tmFrontEndModulation_t *peMod
)
{
    UInt16 uValue = 0;

    pHCDPObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(HCDP)

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslHCDP_GetMod(0x%08X)", tUnit);

    if (err == TM_OK)
    {
        err = iHCDP_RegRead(pObj,HCDPRegQamMode,&uValue);
    }

    if (err == TM_OK)
    {
        switch (uValue)
        {
        case 0:
            *peMod = tmFrontEndModulationQpsk;
            break;
        case 1:
            *peMod = tmFrontEndModulationQam16;
            break;
        case 2:
            *peMod = tmFrontEndModulationQam32;
            break;
        case 3:
            *peMod = tmFrontEndModulationQam64;
            break;
        case 4:
            *peMod = tmFrontEndModulationQam128;
            break;
        case 5:
            *peMod = tmFrontEndModulationQam256;
            break;
        default:
            err = HCDP_ERR_BAD_PARAMETER;
        }
    }

    _MUTEX_RELEASE(HCDP)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_SetSR                                               */
/*                                                                            */
/* DESCRIPTION: Sets the symbol rate.                                         */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_SetSR(
    tmUnitSelect_t tUnit,
    UInt32 uSR
)
{
#ifndef TMBSL_HCDP_NO_64BITS
    unsigned long long ulHigh = 0;
#else
    UInt32 ulHigh = 0;
    UInt32 ulPrecision = 0x100;
#endif
    UInt16 uLow = 0;

    pHCDPObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(HCDP)

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslHCDP_SetSR(0x%08X)", tUnit);

    if (err == TM_OK)
    {
        if (uSR <= 4000000)
        {
            err = iHCDP_RegWrite(pObj,HCDPRegCtlOctaveScale, 1);
        }
        else
        {
            err = iHCDP_RegWrite(pObj,HCDPRegCtlOctaveScale, 0);
        }
    }

    if (err == TM_OK)
    {
#ifndef TMBSL_HCDP_NO_64BITS
        /* set symbol rate */
        ulHigh = 1UL << 25;
        ulHigh *= uSR;
        /* rounding */
        ulHigh += pObj->sConfig.uSamplingClock / 2;
        ulHigh /= pObj->sConfig.uSamplingClock;
#else
        ulHigh = iHCDP_Division32( 0x2000000/ulPrecision, uSR, pObj->sConfig.uSamplingClock/ulPrecision);
#endif
        uLow = (UInt16)(ulHigh & 0xFFFF);
        ulHigh >>=16;

        /* set symbol rate register */
        err = iHCDP_RegWrite(pObj,HCDPRegSymRateHigh,(UInt16)(ulHigh&0xFFFF));
    }

    if (err == TM_OK)
    {
        err = iHCDP_RegWrite(pObj,HCDPRegSymRateLow,uLow);
    }

    if (err == TM_OK)
    {
        if ((pObj->sCurrentChannel.uSR != uSR) ||
            (pObj->sCurrentChannel.eExtendedSRModeCurrent != pObj->sCurrentChannel.eExtendedSRModeNew))
        {
            pObj->sCurrentChannel.uSR = uSR;
            /* Re-calculate ExtendSymbolRateRange with new SR and apply */
            err = iHCDP_SetExtendSymbolRateRange(pObj, pObj->sCurrentChannel.eExtendedSRModeNew);
        }
    }

    _MUTEX_RELEASE(HCDP)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_GetSR                                               */
/*                                                                            */
/* DESCRIPTION: Gets the symbol rate                                          */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_GetSR(
    tmUnitSelect_t tUnit,
    UInt32 *puSR
)
{
#ifndef TMBSL_HCDP_NO_64BITS
    unsigned long long ulTemp = 0;
#else
    UInt32  ulTemp = 0;
    UInt32  ulPrecision = 0x100;
#endif

    UInt16 uLow = 0;
    UInt16 uHigh = 0;

    pHCDPObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);
    
    _MUTEX_ACQUIRE(HCDP)

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslHCDP_GetSR(0x%08X)", tUnit);

    if (err == TM_OK)
    {
        /* SymRate value latched by SymRateLow, so read it first */
        err = iHCDP_RegRead(pObj,HCDPRegSymRateLow,&uLow);
    }

    if (err == TM_OK)
    {
        err = iHCDP_RegRead(pObj,HCDPRegSymRateHigh,&uHigh);
    }

    if (err == TM_OK)
    {
#ifndef TMBSL_HCDP_NO_64BITS
        ulTemp = uHigh;
        ulTemp = (ulTemp << 16) | uLow;
        ulTemp = ulTemp * pObj->sConfig.uSamplingClock;
        ulTemp >>= 25;
#else
        ulTemp = (uHigh << 16) | uLow;
        ulTemp = iHCDP_Division32( ulTemp, pObj->sConfig.uSamplingClock/ulPrecision, 0x2000000/ulPrecision );
#endif
        *puSR = (UInt32)ulTemp;
    }

    _MUTEX_RELEASE(HCDP)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_GetConfiguredSR                                     */
/*                                                                            */
/* DESCRIPTION: Gets the configured symbol rate                               */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_GetConfiguredSR(
    tmUnitSelect_t tUnit,
    UInt32 *puSR
)
{
    pHCDPObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);
    
    _MUTEX_ACQUIRE(HCDP)

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslHCDP_GetConfiguredSR(0x%08X)", tUnit);

    if (err == TM_OK)
    {
        *puSR = pObj->sCurrentChannel.uSR;
    }

    _MUTEX_RELEASE(HCDP)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_SetFECMode                                          */
/*                                                                            */
/* DESCRIPTION: sets the FEC mode.                                            */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_SetFECMode(
    tmUnitSelect_t tUnit,
    tmFrontEndFECMode_t eFECMode
)
{
    pHCDPObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);
    
    _MUTEX_ACQUIRE(HCDP)

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslHCDP_SetFECMode(0x%08X)", tUnit);

    if (err == TM_OK && pObj->sCurrentChannel.eFECMode != eFECMode)
    {
        pObj->sCurrentChannel.eFECMode = eFECMode;

        switch (eFECMode)
        {
        case tmFrontEndFECModeAnnexA:
        case tmFrontEndFECModeAnnexC:
            err = iHCDP_RegWrite(pObj, HCDPRegFecMode, 1);

            if (err == TM_OK)
            {
                err = iHCDP_RegWrite(pObj, HCDPRegNxaSoftReset, 1);
            }
            if (err == TM_OK)
            {
                err = iHCDP_RegWrite(pObj, HCDPRegNxaSoftReset, 0);
            }
            break;
        case tmFrontEndFECModeAnnexB:
            err = iHCDP_RegWrite(pObj,HCDPRegFecMode, 0);

            /* feature for PRCR#342 & PRCR#512 */
            /* TS framing sync issue with 0xFF null stuffing packets */
            if (pObj->eHwCdpSample == HCDP_SAMPLE_ES3)
            {
                if (err == TM_OK)
                {
                    err = iHCDP_RegWrite(pObj,HCDPRegNxbRsMpegTsHeaderTEICheck, 1);
                }
            }
            break;
        default:
            err = HCDP_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = iHCDP_SetMatchedFilter(pObj);
        }

    #ifdef HCDPANTI_PULSE_ENHANCE
        if (err == TM_OK)
        {
            err = HCDPSetAntiPulseEnhance(tUnit);
        }
    #endif //HCDPANTI_PULSE_ENHANCE

        /* update structure pObj if error */
        if (err != TM_OK)
        {
            pObj->sCurrentChannel.eFECMode = tmFrontEndFECModeMax;
        }
    }

    _MUTEX_RELEASE(HCDP)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_GetFECMode                                          */
/*                                                                            */
/* DESCRIPTION: Gets the FED mode.                                            */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_GetFECMode(
    tmUnitSelect_t tUnit,
    tmFrontEndFECMode_t *peFECMode
)
{
    UInt16 uFecMode = 0;
    UInt16 uValue = 0;

    pHCDPObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);
    
    _MUTEX_ACQUIRE(HCDP)

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslHCDP_GetFECMode(0x%08X)", tUnit);

    if (err == TM_OK)
    {
        if( pObj->bReset == True )
        {
            err = iHCDP_RegRead(pObj,HCDPRegFecMode,&uFecMode);

            if (err == TM_OK)
            {
                switch (uFecMode)
                {
                case 0:
                    /* case of AnnexB */
                    *peFECMode = tmFrontEndFECModeAnnexB;
                    break;
                case 1:
                    /* check whether AnnexA or AnnexC */
                    err = iHCDP_RegRead(pObj,HCDPRegMfCoeffSel, &uValue);

                    if( err == TM_OK )
                    {
                        switch( uValue )
                        {
                        case 0x1:
                            /* AnnexA */
                            *peFECMode = tmFrontEndFECModeAnnexA;
                            break;
                        case 0x0:
                            /* AnnexC */
                            *peFECMode = tmFrontEndFECModeAnnexC;
                            break;
                        default:
                            err = HCDP_ERR_NOT_SUPPORTED;
                            break;
                        }
                    }
                    break;
                default:
                    err = HCDP_ERR_NOT_SUPPORTED;
                    break;
                }
            }
        }
        else
        {
            err = HCDP_ERR_NOT_INITIALIZED;
        }
    }

    _MUTEX_RELEASE(HCDP)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_SetSI                                               */
/*                                                                            */
/* DESCRIPTION: Sets the spectral inversion.                                  */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_SetSI(
    tmUnitSelect_t tUnit,
    tmFrontEndSpecInv_t eSI
)
{
    pHCDPObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);
    
    _MUTEX_ACQUIRE(HCDP)

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslHCDP_SetSI(0x%08X)", tUnit);

    if (err == TM_OK && pObj->sCurrentChannel.eSI != eSI)
    {
        pObj->sCurrentChannel.eSI = eSI;

        switch (eSI)
        {
        case tmFrontEndSpecInvOff:
            err = iHCDP_RegWrite(pObj,HCDPRegSpectralInversion,0);
            
            if (err == TM_OK)
            {
                err = iHCDP_RegWrite(pObj,HCDPRegNxbTrManualSpecInv,1);
            }

            if (err == TM_OK)
            {
                err = iHCDP_RegWrite(pObj,HCDPRegDemodOutManualSpecInv,1);
            }
            break;

        case tmFrontEndSpecInvOn:
            err = iHCDP_RegWrite(pObj,HCDPRegSpectralInversion,1);
            
            if (err == TM_OK)
            {
                err = iHCDP_RegWrite(pObj,HCDPRegNxbTrManualSpecInv,1);
            }

            if (err == TM_OK)
            {
                err = iHCDP_RegWrite(pObj,HCDPRegDemodOutManualSpecInv,1);
            }
            break;

        case tmFrontEndSpecInvAuto:
            err = iHCDP_RegWrite(pObj,HCDPRegSpectralInversion,0);

            if (err == TM_OK)
            {
                err = iHCDP_RegWrite(pObj,HCDPRegNxbTrManualSpecInv,0);
            }

            if (err == TM_OK)
            {
                err = iHCDP_RegWrite(pObj,HCDPRegDemodOutManualSpecInv,0);
            }
            break;

        default:
            err = HCDP_ERR_BAD_PARAMETER;
            break;
        }

        /* update structure pObj if error */
        if (err != TM_OK)
        {
            pObj->sCurrentChannel.eSI = tmFrontEndSpecInvMax;
        }
    }

    _MUTEX_RELEASE(HCDP)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_GetSI                                               */
/*                                                                            */
/* DESCRIPTION: Gets the spectal inversion.                                   */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_GetSI(
    tmUnitSelect_t tUnit,
    tmFrontEndSpecInv_t *peSI
)
{
    pHCDPObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;
    UInt16 uValue;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(HCDP)

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslHCDP_GetSI(0x%08X)", tUnit);

    if (err == TM_OK)
    {
        if( (pObj->sCurrentChannel.eFECMode == tmFrontEndFECModeAnnexA) || (pObj->sCurrentChannel.eFECMode == tmFrontEndFECModeAnnexC) )
        {
            /* Annex A - DVB-C */
            /* DemodOutSpecInvState */
            err = iHCDP_RegRead(pObj, HCDPRegDemodOutSpecInvState, &uValue);

            if (err == TM_OK)
            {
                if( uValue == 0 )
                {
                    *peSI = tmFrontEndSpecInvOff;
                }
                else
                {
                    *peSI = tmFrontEndSpecInvOn;
                }
            }
        }
        else
        {
            /* Annex B - J83C */
            /* NxbTrSpecInvState */
            err = iHCDP_RegRead(pObj, HCDPRegNxbTrSpecInvState, &uValue);

            if (err == TM_OK)
            {
                if( uValue == 0 )
                {
                    *peSI = tmFrontEndSpecInvOff;
                }
                else
                {
                    *peSI = tmFrontEndSpecInvOn;
                }
            }
        }
    }

    _MUTEX_RELEASE(HCDP)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_GetConfiguredSI                                     */
/*                                                                            */
/* DESCRIPTION: Gets the configured spectal inversion.                        */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_GetConfiguredSI(
    tmUnitSelect_t tUnit,
    tmFrontEndSpecInv_t *peSI
)
{
    pHCDPObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);
    
    _MUTEX_ACQUIRE(HCDP)

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslHCDP_GetConfiguredSI(0x%08X)", tUnit);

    if (err == TM_OK)
    {
        *peSI = pObj->sCurrentChannel.eSI;
    }

    _MUTEX_RELEASE(HCDP)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_GetChannelEsNo                                      */
/*                                                                            */
/* DESCRIPTION: Gets the channel EsNo in dB.                                  */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_GetChannelEsNo(
    tmUnitSelect_t tUnit,
    tmFrontEndFracNb32_t *pEsno
)
{
    pHCDPObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);
    
    _MUTEX_ACQUIRE(HCDP)

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslHCDP_GetChannelEsNo(0x%08X)", tUnit);

    if (err == TM_OK)
    {
        err = iHCDP_GetEsNo(pObj,pEsno);
    }

    _MUTEX_RELEASE(HCDP)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_GetBER                                              */
/*                                                                            */
/* DESCRIPTION: Gets the Bit/Byte/Block BERs.                                 */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_GetBER(
    tmUnitSelect_t tUnit,
    tmFrontEndFracNb32_t *psBER,
    UInt32 *puUncors
)
{
    pHCDPObject_t pObj = Null;
    tmErrorCode_t err = TM_OK;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(HCDP)

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslHCDP_GetBER(0x%08X)", tUnit);

    if (err == TM_OK)
    {
        err = iHCDP_GetBER( pObj, psBER, puUncors);
    }

    _MUTEX_RELEASE(HCDP)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_SetBERWindow                                        */
/*                                                                            */
/* DESCRIPTION: Gets the Bit/Byte/Block BERs.                                 */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_SetBERWindow(
    tmUnitSelect_t tUnit,
    HCDP_BERWindow_t eBERWindow
)
{
    pHCDPObject_t pObj = Null;
    tmErrorCode_t err = TM_OK;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(HCDP)

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslHCDP_SetBERWindow(0x%08X)", tUnit);

    if (err == TM_OK)
    {
        pObj->eBERWindow = eBERWindow;
    }

    _MUTEX_RELEASE(HCDP)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_GetBERWindow                                        */
/*                                                                            */
/* DESCRIPTION: Gets the Bit/Byte/Block BERs.                                 */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_GetBERWindow(
    tmUnitSelect_t tUnit,
    HCDP_BERWindow_t *peBERWindow
)
{
    pHCDPObject_t pObj = Null;
    tmErrorCode_t err = TM_OK;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(HCDP)

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslHCDP_GetBERWindow(0x%08X)", tUnit);

    if (err == TM_OK)
    {
        *peBERWindow = pObj->eBERWindow;
    }

    _MUTEX_RELEASE(HCDP)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_GetLockTime                                         */
/*                                                                            */
/* DESCRIPTION: Gets the lock time of demod and FEC.                          */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_GetLockTime(
    tmUnitSelect_t tUnit,
    UInt16 *uDemodLockTime,
    UInt16 *uFecLockTime
)
{
    UInt16 uVal, uVal1, uVal2, uVal3;
    UInt16 uTicDivider, uTimer, uTimer2;

    pHCDPObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);
    
    _MUTEX_ACQUIRE(HCDP)

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslHCDP_GetLockTime(0x%08X)", tUnit);

    *uDemodLockTime = 0;
    *uFecLockTime = 0;

    if (err == TM_OK)
    {
        err = iHCDP_RegRead(pObj,HCDPRegTimersLockInd,&uVal);
    }
    
    if (err == TM_OK)
    {
        if (uVal)
        { /* timers locked */
            if (pObj->eHwCdpSample == HCDP_SAMPLE_ES1) /* temporary: to be adapted when ES1 is removed */
            {
                err = iHCDP_RegRead(pObj,HCDPRegTimersWrapInd,&uVal1);

                if (err == TM_OK)
                {
                    err = iHCDP_RegRead(pObj,HCDPRegTimersBreakAfterLockInd,&uVal2);
                }

                if (err == TM_OK)
                {
                    err = iHCDP_RegRead(pObj,HCDPRegTimerslockAfterBreakInd,&uVal3);
                }

                if (err == TM_OK)
                {
                    err = iHCDP_RegRead(pObj,HCDPRegTimersTicDivider,&uTicDivider);
                }

                if (err == TM_OK)
                {
                    /* Demod lock */
                    if ((uVal1 | uVal2 | uVal3) & 0x1)
                    { /* Timer 0 wrap around or loose lock */
                        *uDemodLockTime = 0xFFFF;
                    }
                    else
                    {
                        err = iHCDP_RegRead(pObj,HCDPRegTimer0,&uTimer);

                        if (err == TM_OK)
                        {
                            *uDemodLockTime = (UInt16)(uTimer*uTicDivider/(pObj->sConfig.uDClock/1000000)/1000); /* ms */
                        }
                    }
                }

                if (err == TM_OK)
                {
                    /* FEC lock */
                    if  ((uVal1 | uVal2 | uVal3) & 0xE)
                    { /* Timer 1, 2 or 3 wrap around or loose lock */
                        *uFecLockTime = 0xFFFF;
                    }
                    else
                    {
                        err = iHCDP_RegRead(pObj,HCDPRegTimer3,&uTimer);

                        if (err == TM_OK)
                        {
                            err = iHCDP_RegRead(pObj,HCDPRegTimer2,&uTimer2);
                        }

                        if (err == TM_OK)
                        {
                            /* Timer2 sometimes overtakes Timer3, hence keep the highest */
                            if (uTimer2 > uTimer)
                            {
                                uTimer = uTimer2;
                            }

                            *uFecLockTime = (UInt16)(uTimer*uTicDivider/(pObj->sConfig.uDClock/1000000)/1000); /* ms */
                        }
                    }
                }
            }
            else if ((pObj->eHwCdpSample == HCDP_SAMPLE_ES2)
                  || (pObj->eHwCdpSample == HCDP_SAMPLE_ES3))
            {
                err = iHCDP_RegRead(pObj,HCDPRegTimersTicDivider,&uTicDivider);

                if (err == TM_OK)
                {
                    err = iHCDP_RegRead(pObj,HCDPRegTimer0,&uTimer);
                }

                if (err == TM_OK)
                {
                    *uDemodLockTime = (UInt16)(uTimer*uTicDivider/(pObj->sConfig.uDClock/1000000)/1000); /* ms */
                    if (pObj->sCurrentChannel.eFECMode == tmFrontEndFECModeAnnexB)
                    {
                        /* Annex B uses NxbRsMpegSync */
                        err = iHCDP_RegRead(pObj,HCDPRegTimer3,&uTimer);
                    }
                    else
                    {
                        /* Annex A and Annex C use NxaRsSync */
                        err = iHCDP_RegRead(pObj,HCDPRegTimer2,&uTimer);
                    }
                }

                if (err == TM_OK)
                {
                    *uFecLockTime = (UInt16)(uTimer*uTicDivider/(pObj->sConfig.uDClock/1000000)/1000); /* ms */
                }
            }
            else
            {
                err = HCDP_ERR_BAD_VERSION;
            }
        }
        else
        { /* timers not locked */
            /* returns the default values 0 for both*/
        }
    }

    _MUTEX_RELEASE(HCDP)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_GetFreqOffset                                       */
/*                                                                            */
/* DESCRIPTION: Gets the frequency offset in Hz.                              */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_GetFreqOffset(
    tmUnitSelect_t tUnit,
    Int32 *plFreqOffsetHz
)
{
    UInt16 uAcc = 0;
    UInt16 uOctave = 0;
    UInt16 uScale = 0;
    UInt32 uSymbolrate = 0;
    UInt32 uSampleFreq;

    pHCDPObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* initialize to handle errors */
    *plFreqOffsetHz = 0;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(HCDP)

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslHCDP_GetFreqOffset(0x%08X)", tUnit);

    if (err == TM_OK)
    {
        uSampleFreq = pObj->sConfig.uSamplingClock;

        err = iHCDP_RegRead(pObj,HCDPRegCtlAcc,&uAcc);
    }

    if (err == TM_OK)
    {
        if (uAcc & 0x8000)
        {
            /* negative */
            uAcc = ~uAcc + 1;
            *plFreqOffsetHz = -1;
        }
        else
        {
            *plFreqOffsetHz = 1;
        }

        err = iHCDP_RegRead(pObj, HCDPRegCtlOctaveScale, &uOctave);
    }

    if (err == TM_OK)
    {

        if (0 == uOctave)
        {
            *plFreqOffsetHz *= uSampleFreq;
            *plFreqOffsetHz /= 4096; // 4096=2^12
            *plFreqOffsetHz *= uAcc;
            *plFreqOffsetHz /= 256; // +2^8=2^20
        }
        else
        {
            err = tmbslHCDP_GetSR(tUnit, &uSymbolrate);

            if (err == TM_OK)
            {
                uScale = (UInt16)(uSampleFreq/uSymbolrate);

                *plFreqOffsetHz *= uSampleFreq;
                *plFreqOffsetHz /= 4096; // 4096=2^12
                *plFreqOffsetHz *= uAcc;

                if (uScale >= 4 && uScale < 8)
                {
                    *plFreqOffsetHz /= 256; // *2^8=2^20
                }
                else if (uScale >= 8 && uScale < 16)
                {
                    *plFreqOffsetHz /= 512; // *2^9=2^21
                }
                else if (uScale >= 16 && uScale < 32)
                {
                    *plFreqOffsetHz /= 1024; // *2^10=2^22
                }
                else if (uScale >= 32 && uScale < 64)
                {
                    *plFreqOffsetHz /= 2048; // *2^11=2^23
                }
                else if (uScale >= 64 && uScale < 128)
                {
                    *plFreqOffsetHz /= 4096; // *2^12=2^24
                }
                else
                {
                    err = HCDP_ERR_NOT_SUPPORTED;
                }
            }
        }
    }

    _MUTEX_RELEASE(HCDP)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_GetSignalToISI                                      */
/*                                                                            */
/* DESCRIPTION: Gets the Equaliser DFE Power.  TODO check                     */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_GetSignalToISI(
    tmUnitSelect_t tUnit,
    UInt16 *puSignalToISI,
    UInt16 *puEqCenterTapGain
)
{
    UInt16 EqTapPowerReady;
    UInt16 TimeOut = 500;

    pHCDPObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(HCDP)

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslHCDP_GetSignalToISI(0x%08X)", tUnit);

    if (err == TM_OK)
    {
        /* start measurement */
        err = iHCDP_RegWrite(pObj,HCDPRegEqTapPowerStart,0);
    }

    if (err == TM_OK)
    {
        err = iHCDP_RegWrite(pObj,HCDPRegEqTapPowerStart,1);
    }

    if (err == TM_OK)
    {
        err = iHCDP_RegWrite(pObj,HCDPRegEqTapPowerStart,0);
    }

    if (err == TM_OK)
    {

        /* wait value ready around 1ms */
        do
        {
            err = iHCDP_RegRead(pObj,HCDPRegEqTapPowerReady, &EqTapPowerReady);
            TimeOut--;
        } while ((EqTapPowerReady != 1) && (TimeOut != 0) && (err == TM_OK));
    }

    if (err == TM_OK)
    {
        if (TimeOut == 0)
        {
            err = HCDP_ERR_NOT_SUPPORTED;
        }
    }

    if (err == TM_OK)
    {
        err = iHCDP_RegRead(pObj,  HCDPRegEqDfePwr,puSignalToISI);
    }

    if (err == TM_OK)
    {
        err = iHCDP_RegRead(pObj, HCDPRegEqCenterTapGain, puEqCenterTapGain);
    }

    _MUTEX_RELEASE(HCDP)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_GetSignalToInterference                             */
/*                                                                            */
/* DESCRIPTION: Gets the signal to interference ratio.                        */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_GetSignalToInterference(
    tmUnitSelect_t tUnit,
    UInt16 *puSignalToInterference,
    UInt16 *puEqCenterTapGain
)
{
    UInt16 EqTapPowerReady;
    UInt16 TimeOut = 500;

    pHCDPObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(HCDP)

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslHCDP_GetSignalToInterference(0x%08X)", tUnit);

    if (err == TM_OK)
    {
        /* start measurement */
        err = iHCDP_RegWrite(pObj,HCDPRegEqTapPowerStart,0);
    }
    
    if (err == TM_OK)
    {
        err = iHCDP_RegWrite(pObj,HCDPRegEqTapPowerStart,1);
    }

    if (err == TM_OK)
    {
        err = iHCDP_RegWrite(pObj,HCDPRegEqTapPowerStart,0);
    }

    if (err == TM_OK)
    {
        /* wait value ready around 1ms */
        do
        {
            err = iHCDP_RegRead(pObj,HCDPRegEqTapPowerReady, &EqTapPowerReady);
            TimeOut--;
        } while ((EqTapPowerReady != 1) && (TimeOut != 0) && (err == TM_OK));
    }

    if (err == TM_OK)
    {
        if (TimeOut == 0)
        {
            err = HCDP_ERR_NOT_SUPPORTED;
        }
    }
    
    if (err == TM_OK)
    {
        err = iHCDP_RegRead(pObj, HCDPRegEqEfePwr, puSignalToInterference);
    }

    if (err == TM_OK)
    {
        err = iHCDP_RegRead(pObj, HCDPRegEqCenterTapGain, puEqCenterTapGain);
    }

    _MUTEX_RELEASE(HCDP)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_SetIF                                               */
/*                                                                            */
/* DESCRIPTION: Sets the IF in Hz.                                            */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_SetIF(
    tmUnitSelect_t tUnit,
    UInt32 uIF
)
{
    pHCDPObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(HCDP)

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslHCDP_SetIF(0x%08X)", tUnit);

    if (err == TM_OK)
    {
        pObj->sCurrentChannel.uIF = uIF;

        err = iHCDP_ConfigCTL(pObj);
    }

    _MUTEX_RELEASE(HCDP)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_GetConfiguredIF                                     */
/*                                                                            */
/* DESCRIPTION: Gets the configured IF in Hz.                                 */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_GetConfiguredIF(
    tmUnitSelect_t tUnit,
    UInt32 *puIF
)
{
    pHCDPObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(HCDP)

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslHCDP_GetConfiguredIF(0x%08X)", tUnit);

    if (err == TM_OK)
    {
        *puIF = pObj->sCurrentChannel.uIF;
    }

    _MUTEX_RELEASE(HCDP)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_GetIFOffset                                         */
/*                                                                            */
/* DESCRIPTION: Gets the IF offset.                                           */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_GetIFOffset(
    tmUnitSelect_t tUnit,
    Int32 *plIFOffset
)
{
#ifndef TMBSL_HCDP_NO_64BITS
    unsigned long long ulTemp = 0;
#else
    UInt32 ulTemp = 0;
    UInt32 ulPrecision = 0x100;
#endif
    UInt16 uLow = 0;
    UInt16 uHigh = 0;
    Int32 lSign = 1;

    pHCDPObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* initialize to handle errors */
    *plIFOffset = 0;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(HCDP)

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslHCDP_GetIFOffset(0x%08X)", tUnit);

    if (err == TM_OK)
    {
        /* DerotateFreq value latched by DerotateFreqLow, so read it first */
        err = iHCDP_RegRead(pObj,HCDPRegDerotateFreqLow,&uLow);
    }

    if (err == TM_OK)
    {
        err = iHCDP_RegRead(pObj,HCDPRegDerotateFreqHigh,&uHigh);
    }

    if (err == TM_OK)
    {
        ulTemp = uHigh;
        ulTemp = (ulTemp << 16) | uLow;
        if (ulTemp & 0x800000) /* convert negative value */
        {
            lSign = -1;
            ulTemp = (~ulTemp & 0xFFFFFF) + 1;
        }
#ifndef TMBSL_HCDP_NO_64BITS
        ulTemp = ulTemp * pObj->sConfig.uSamplingClock;
        ulTemp >>= 24;
#else
        ulTemp = iHCDP_Division32( ulTemp, pObj->sConfig.uSamplingClock/ulPrecision, 0x1000000/ulPrecision );
#endif
        *plIFOffset = (Int32)ulTemp*lSign; /* apply the sign */

        *plIFOffset -= (Int32)pObj->sCurrentChannel.uIF;
    }

    _MUTEX_RELEASE(HCDP)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_ClearUncor                                          */
/*                                                                            */
/* DESCRIPTION: Clears the uncorrected blocks counter in the BER measurement. */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_ClearUncor(
    tmUnitSelect_t tUnit
)
{
    pHCDPObject_t  pObj = Null;
    tmErrorCode_t        err = TM_OK;

    UInt16 uValHigh;
    UInt16 uValLow;
    UInt32 uUncorrectable;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(HCDP)

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslHCDP_ClearUncor(0x%08X)", tUnit);

    /* copy internal to external registers */
    /* (does not mean that something is frozen) */
    if (err == TM_OK)
    {
        err = iHCDP_RegWrite(pObj,HCDPRegRsDecodFreeze,1);
    }

    if (err == TM_OK)
    {
        /* retrieve current UNCOR value */
        err = iHCDP_RegRead(pObj,HCDPRegRsDecodUncorrectableCwHigh,&uValHigh);
    }

    if (err == TM_OK)
    {
        err = iHCDP_RegRead(pObj,HCDPRegRsDecodUncorrectableCwLow,&uValLow);
    }

    if (err == TM_OK)
    {
        uUncorrectable = ((UInt32)uValHigh << 16) |uValLow;
    }

    if (err == TM_OK)
    {
        /* Sets the base of the uncors to the current value */
        pObj->uBaseUncors = uUncorrectable;
        pObj->uPreviousUncors = 0;
    }

    _MUTEX_RELEASE(HCDP)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_GetSignalQuality                                    */
/*                                                                            */
/* DESCRIPTION: Gets the signal quality.                                      */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_GetSignalQuality(
    tmUnitSelect_t tUnit,
    UInt32 *puSignalQuality
)
{
    pHCDPObject_t pObj = Null;
    tmErrorCode_t err = TM_OK;
    tmbslFrontEndState_t eLockStatus = tmbslFrontEndStateNotLocked;
    tmFrontEndFECMode_t eFECMode = tmFrontEndFECModeUnknown;

    tmFrontEndFracNb32_t sBER;
    UInt32 uUncors;

    UInt32 ulBer;
    UInt32 ulPercent = 1;
    UInt32 ulPercentIndex = 1;

    UInt32 ulPercentNormMin = 0;
    UInt32 ulPercentNormMax = 80;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(HCDP)

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslHCDP_GetSignalQuality(0x%08X)", tUnit);
    
    /* evaluate signal quality in percent */
    /* - unlock          ->  0 % */
    /* - uncor present   -> 10 % */
    /* - ber 10 exp -2   -> 20 % */

    if (err == TM_OK)
    {
        /* get lock status */
        err = tmbslHCDP_GetLockStatus(tUnit, &eLockStatus);
    }

    if (err == TM_OK)
    {
        if (eLockStatus == tmbslFrontEndStateLocked)
        {
            /* Get Ber */
            err = iHCDP_GetBER(pObj, &sBER, &uUncors);
            
            if (err == TM_OK)
            {
                err = tmbslHCDP_GetFECMode(tUnit, &eFECMode);
            }
            
            if (err == TM_OK)
            {
                /* locked -> check if uncor are present */
                if( (uUncors == pObj->uPreviousUncors) )
                {
                    if( sBER.lInteger == 0 )
                    {
                        *puSignalQuality = 100;
                    }
                    else
                    {
                        /* get 1/BER (in order to be sure to have a 32 bits result) */
                        ulBer = sBER.uDivider/sBER.lInteger;

                        /* get percentage tens */
                        ulPercent = 0;
                        ulPercentIndex = 1;

                        while( ulBer > ulPercentIndex * 10 ) 
                        {
                            ulPercentIndex *= 10;
                            ulPercent++;
                        }
                        
                        /* normalise 1/BER value 0 and 100 (by dividing by power of 10) */
                        /* then calculate percentage */
                        ulPercent = (ulPercent * 10) + (ulBer / ulPercentIndex);

                        /* normalise the percent value */
                        /* current value is between 0 and 90 */
                        /* should be between 0 and 100 */
                        if( (eFECMode == tmFrontEndFECModeAnnexA) || (eFECMode == tmFrontEndFECModeAnnexC) )
                        {
                            /* Annex A - DVB-C */
                            ulPercentNormMin = HCDP_SIGNAL_QUALITY_NORM_MIN_DVBC;
                        }
                        else
                        {
                            /* J83B */
                            ulPercentNormMin = HCDP_SIGNAL_QUALITY_NORM_MIN_J83B;
                        }

                        ulPercentNormMax = HCDP_SIGNAL_QUALITY_NORM_MAX;

                        /* minimum value is 20% */
                        if( ulPercent <= ulPercentNormMin )
                        {
                            *puSignalQuality = 20;
                        }
                        else
                        {
                            /* Calculate normalized percentage */
                            /* Since there is an offset of 20%, the value is normalized between 0 and 80 */
                            /* So when ulPercent = 0 -> normalized value = 0 + 20 */
                            /* So when ulPercent = 80 (max value) -> normalized value = 80 + 20 = 100 */
                            *puSignalQuality = 20 + ((ulPercent - ulPercentNormMin) * 80) / (ulPercentNormMax - ulPercentNormMin);
                        }

                        if( *puSignalQuality > 100 )
                        {
                            *puSignalQuality = 100;
                        }
                    }
                }
                else
                {
                    /* uncor are present since last check */
                    /* reset uncor variable */
                    pObj->uPreviousUncors = uUncors;

                    /* set quality to 10% */
                    *puSignalQuality = 10;
                }
            }
        }
        else
        {
            /* set quality to 0% */
            *puSignalQuality = 0;
        }
    }

    _MUTEX_RELEASE(HCDP)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_SetExtendSymbolRateRange                            */
/*                                                                            */
/* DESCRIPTION: Select the extended timing recovery mode                      */
/*              applied during SetSR() API                                    */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_SetExtendSymbolRateRange(
    tmUnitSelect_t tUnit,
    HCDP_ExtendSymbolRateMode_t eMode
)
{
    pHCDPObject_t pObj = Null;
    tmErrorCode_t err = TM_OK;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(HCDP)

    if (err == TM_OK)
    {
        pObj->sCurrentChannel.eExtendedSRModeNew = eMode;
    }

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslHCDP_SetExtendSymbolRateRange(0x%08X)", tUnit);

    _MUTEX_RELEASE(HCDP)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_GetExtendSymbolRateRange                            */
/*                                                                            */
/* DESCRIPTION: Get the selected extended timing recovery mode                */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_GetExtendSymbolRateRange(
    tmUnitSelect_t tUnit,
    HCDP_ExtendSymbolRateMode_t *peMode
)
{
    pHCDPObject_t pObj = Null;
    tmErrorCode_t err = TM_OK;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(HCDP)

    if (err == TM_OK)
    {
       *peMode = pObj->sCurrentChannel.eExtendedSRModeCurrent;
    }

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslHCDP_GetExtendSymbolRateRange(0x%08X)", tUnit);

    _MUTEX_RELEASE(HCDP)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_GetActualInterleaverMode                            */
/*                                                                            */
/* DESCRIPTION: Gets the actual interleaver mode.                             */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_GetActualInterleaverMode(
    tmUnitSelect_t tUnit,
    HCDP_InterleaverMode_t *peInterleaverMode
)
{
    tmFrontEndFECMode_t  eFECMode;

    pHCDPObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);
    
    _MUTEX_ACQUIRE(HCDP)

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslHCDP_GetActualInterleaverMode(0x%08X)", tUnit);

    if (err == TM_OK)
    {
        err = tmbslHCDP_GetFECMode(tUnit,&eFECMode);
    }

    if (err == TM_OK)
    {
        if( (eFECMode == tmFrontEndFECModeAnnexA) || (eFECMode == tmFrontEndFECModeAnnexC) )
        {
            *peInterleaverMode = HCDP_InterleaverMode_12_17;  /*HCDP only support for 12x17 in Annex A */
        }
        else
        {
            err = iHCDP_GetAnxBInterleaverMode(pObj,peInterleaverMode);
        }
    }
    
    _MUTEX_RELEASE(HCDP)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_SelectEqualizerSettings                                */
/*                                                                            */
/* DESCRIPTION: change the equalizer settings                                 */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_SelectEqualizerSettings(
    tmUnitSelect_t tUnit,
    UInt16 uEqFfLeakageAlpha_BeforeLock,
    UInt16 uEqFfLeakageAlpha_AfterLock
)
{
    pHCDPObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(HCDP)

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslHCDP_SelectEqualizerSettings(0x%08X)", tUnit);

    if (err == TM_OK)
    {
        pObj->sConfig.uEqFfLeakageAlpha_BeforeLock = uEqFfLeakageAlpha_BeforeLock;
        pObj->sConfig.uEqFfLeakageAlpha_AfterLock = uEqFfLeakageAlpha_AfterLock;
    }

    _MUTEX_RELEASE(HCDP)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_SetAfterLock                                        */
/*                                                                            */
/* DESCRIPTION: change some settings after lock                               */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/

tmErrorCode_t
tmbslHCDP_SetAfterLock(
    tmUnitSelect_t tUnit
)
{
    pHCDPObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(HCDP)

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslHCDP_SetAfterLock(0x%08X)", tUnit);

    if (err == TM_OK && pObj->bSetAfterLock == False)
    {
        err = iHCDP_RegWrite(pObj, HCDPRegEqFfLeakageAlpha, pObj->sConfig.uEqFfLeakageAlpha_AfterLock);
        //if (pObj->sCurrentChannel.eMod == tmFrontEndModulationQam64)
        //{
        //    /* optimization for CCI in 64QAM after lock */
        //    err = iHCDP_RegWrite(pObj, HCDPRegEqFfLeakageAlpha, 0xD);
        //}
        //else
        //{
        //    /* no change for other modulations */
        //}
        pObj->bSetAfterLock = (err == TM_OK ? True : False);
    }

    _MUTEX_RELEASE(HCDP)

    return err;
}

/*============================================================================*/
/* Internal functions:                                                        */
/*============================================================================*/

/*============================================================================*/
/* FUNCTION:    iHCDP_SetMatchedFilter                                        */
/*                                                                            */
/* DESCRIPTION: Sets the matched filter coefficients.                         */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
static tmErrorCode_t
iHCDP_SetMatchedFilter(
    pHCDPObject_t pObj
)
{
    tmErrorCode_t       err = TM_OK;

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "iHCDP_SetMatchedFilter(0x%08X)", pObj->tUnit);

    /*Set the filter type*/
    if (pObj->sCurrentChannel.eFECMode == tmFrontEndFECModeAnnexA)
    {
        err = iHCDP_RegWrite(pObj,HCDPRegMfCoeffSel,0x1); /*0.15 SRRC*/
    }
    else if (pObj->sCurrentChannel.eFECMode == tmFrontEndFECModeAnnexB)
    {
        if (pObj->sCurrentChannel.eMod == tmFrontEndModulationQam64)
        {
            err = iHCDP_RegWrite(pObj,HCDPRegMfCoeffSel,0x2); /*0.18 SRRC*/
        }
        else /* pObj->sCurrentChannel.eMod == tmFrontEndModulationQam256 */
        {
            err = iHCDP_RegWrite(pObj,HCDPRegMfCoeffSel,0x0); /*0.12 SRRC*/
        }
    }
    else if (pObj->sCurrentChannel.eFECMode == tmFrontEndFECModeAnnexC)
    {
        err = iHCDP_RegWrite(pObj,HCDPRegMfCoeffSel,0x0); /*0.12 but 0.13 SRRC ideally*/
    }
    else
    {
        err = HCDP_ERR_NOT_SUPPORTED;
    }

    return err;
}

#ifdef NOT_IMPLEMENTED
/*============================================================================*/
/* FUNCTION:    iHCDP_SetAntiPulseEnhance                                     */
/*                                                                            */
/* DESCRIPTION: Enhances the pulse resistance in 64-QAM Annex A.              */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
static tmErrorCode_t
iHCDP_SetAntiPulseEnhance(
    pHCDPObject_t pObj
)
{
    tmErrorCode_t       err = TM_OK;
    
    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "iHCDPSetAntiPulseEnhance(0x%08X)", pObj->tUnit);

    if ((pObj->sCurrentChannel.eFECMode == tmFrontEndFECModeAnnexA) && (pObj->sCurrentChannel.eMod == tmFrontEndModulationQam64))
    {
        // Following code was the writing bellow
        // pObj->sConfig.SBWrite(tUnit,0x302,0x42,&bRet); /* SctlTrkGain */
        if (err == TM_OK)
        {
            err = iHCDP_RegWrite(pObj, HCDPRegSctlTrkDirGain, 0x2);
        }

        if (err == TM_OK)
        {
            err = iHCDP_RegWrite(pObj, HCDPRegSctlTrkIndGain, 0x1);
        }

        // Following code was the writing bellow
        // pObj->sConfig.SBWrite(tUnit,0x255,0x48,&bRet); /* DagcThreshold */
        if (err == TM_OK)
        {
            err = iHCDP_RegWrite(pObj, HCDPRegDagcThreshold, 0x48);
        }
    }

    return err;
}
#endif

/*============================================================================*/
/* FUNCTION:    iHCDP_ResetCdp                                                */
/*                                                                            */
/* DESCRIPTION: Resets the different specified block.                         */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
static tmErrorCode_t
iHCDP_ResetCdp(
    pHCDPObject_t pObj,
    UInt16 uMode
)
{
    tmErrorCode_t       err = TM_OK;

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "iHCDP_ResetCdp(0x%08X)", pObj->tUnit);

    if (uMode & HCDP_ResetMode_Soft)
    {
        err = iHCDP_RegWrite(pObj,HCDPRegCdpSoftRst, 0x1);

        if (err == TM_OK)
        {
            err = iHCDP_RegWrite(pObj,HCDPRegCdpSoftRst,0x0);
        }
    }

    if (err == TM_OK)
    {
        if (uMode & HCDP_ResetMode_Micro)
        {
            err = iHCDP_RegWrite(pObj,HCDPRegCdpMicroRst, 0x1);

            if (err == TM_OK)
            {
                err = iHCDP_RegWrite(pObj,HCDPRegCdpMicroRst,0x0);
            }
        }
    }

    if (err == TM_OK)
    {
        if (uMode & HCDP_ResetMode_Full)
        {
            err = iHCDP_RegWrite(pObj,HCDPRegCdpFullRst, 0x1);

            if (err == TM_OK)
            {
                err = iHCDP_RegWrite(pObj,HCDPRegCdpFullRst,0x0);
            }
        }
    }

    if (err == TM_OK)
    {
        if (uMode & HCDP_ResetMode_DemodSoft)
        {
            err = iHCDP_RegWrite(pObj,HCDPRegDemodSoftReset, 0x1); /*0x346*/

            if (err == TM_OK)
            {
                err = iHCDP_RegWrite(pObj,HCDPRegDemodSoftReset,0x0);/*0x346*/
            }
        }
    }

    if (err == TM_OK)
    {
        if (uMode & HCDP_ResetMode_DemodMicro)
        {
            err = iHCDP_RegWrite(pObj,HCDPRegDemodMicroRst, 0x1);

            if (err == TM_OK)
            {
                err = iHCDP_RegWrite(pObj,HCDPRegDemodMicroRst,0x0);
            }
        }
    }

    if (err == TM_OK)
    {
        if (uMode & HCDP_ResetMode_FECSoft)
        {
            err = iHCDP_RegWrite(pObj,HCDPRegFecSoftRst, 0x1);

            if (err == TM_OK)
            {
                err = iHCDP_RegWrite(pObj,HCDPRegFecSoftRst,0x0);
            }
        }
    }

    if (err == TM_OK)
    {
        if (uMode & HCDP_ResetMode_FECMicro)
        {
            err = iHCDP_RegWrite(pObj,HCDPRegFecMicroRst, 0x1);

            if (err == TM_OK)
            {
                err = iHCDP_RegWrite(pObj,HCDPRegFecMicroRst,0x0);
            }
        }
    }

    if (err == TM_OK)
    {
        if (uMode & HCDP_ResetMode_CsmSoft)
        {
            err = iHCDP_RegWrite(pObj,HCDPRegCsmSoftRst, 0x1);

            if (err == TM_OK)
            {
                err = iHCDP_RegWrite(pObj,HCDPRegCsmSoftRst,0x0);
            }
        }
    }

    if (err == TM_OK)
    {
        if (uMode & HCDP_ResetMode_CsmMicro)
        {
            err = iHCDP_RegWrite(pObj,HCDPRegCsmMicroRst, 0x1);

            if (err == TM_OK)
            {
                err = iHCDP_RegWrite(pObj,HCDPRegCsmMicroRst,0x0);
            }
        }
    }

    if (err == TM_OK)
    {
        if (uMode & HCDP_ResetMode_Eq)
        {
            err = iHCDP_RegWrite(pObj,HCDPRegEqReset, 0x1);

            if (err == TM_OK)
            {
                err = iHCDP_RegWrite(pObj,HCDPRegEqReset,0x0);
            }
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iHCDP_ConfigCTL                                               */
/*                                                                            */
/* DESCRIPTION: Sets the two derotators' frequencies.                         */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
static tmErrorCode_t
iHCDP_ConfigCTL(
    pHCDPObject_t pObj
)
{
#ifndef TMBSL_HCDP_NO_64BITS
    unsigned long long ulHigh = 0;
#else
    UInt32 ulHigh = 0;
    UInt32  ulPrecision = 0x100;
#endif
    UInt16 uLow = 0;
    tmErrorCode_t       err = TM_OK;

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "iHCDP_ConfigCTL(0x%08X)", pObj);

    /* Set the IF rotator(derotator2) */
    ulHigh = 1UL << 24;
    /* ulHigh = 0x1000000; */
    err = iHCDP_RegWrite(pObj,HCDPRegCtlAccReset,1);

    if (err == TM_OK)
    {
        err = iHCDP_RegWrite(pObj,HCDPRegCtlDisable,1);
    }

    if (err == TM_OK)
    {
        err = iHCDP_RegWrite(pObj,HCDPRegAfcDisable,1);
    }

#define MAX_BANDWITH 8000000
    if (err == TM_OK)
    {
        if (pObj->sCurrentChannel.uIF < ((pObj->sConfig.uSamplingClock/2) - (MAX_BANDWITH/2)))
        {
            tmDBGPRINTEx(DEBUGLVL_VERBOSE, "iHCDP_ConfigCTL(0x%08X): Low IF mode detected", pObj);
#ifndef TMBSL_HCDP_NO_64BITS
            ulHigh *= pObj->sCurrentChannel.uIF;  /*Low IF mode*/
#else
            ulHigh = iHCDP_Division32( ulHigh/ulPrecision,
                                       pObj->sCurrentChannel.uIF,
                                       pObj->sConfig.uSamplingClock/ulPrecision );
#endif
        }
        else
        {
            tmDBGPRINTEx(DEBUGLVL_VERBOSE, "iHCDP_ConfigCTL(0x%08X): High IF mode detected", pObj);
#ifndef TMBSL_HCDP_NO_64BITS
            ulHigh *= pObj->sConfig.uSamplingClock - pObj->sCurrentChannel.uIF;  /*High IF mode*/
#else
            ulHigh = iHCDP_Division32( ulHigh/ulPrecision,
                                       pObj->sConfig.uSamplingClock - pObj->sCurrentChannel.uIF,
                                       pObj->sConfig.uSamplingClock/ulPrecision );
#endif
        }

#ifndef TMBSL_HCDP_NO_64BITS
        /*make a round operation*/
        ulHigh += (pObj->sConfig.uSamplingClock >> 1);
        ulHigh /= pObj->sConfig.uSamplingClock;
#endif

        tmDBGPRINTEx(DEBUGLVL_VERBOSE, "iHCDP_ConfigCTL(0x%08X): DerotateFreq = %ld", pObj, ulHigh);
        uLow = (UInt16)(ulHigh & 0xFFFF);
        ulHigh >>= 16;
    }

    if (err == TM_OK)
    {
        err = iHCDP_RegWrite(pObj,HCDPRegDerotateFreqHigh,(UInt16)(ulHigh & 0xFF));
    }
    
    if (err == TM_OK)
    {
        err = iHCDP_RegWrite(pObj,HCDPRegDerotateFreqLow,uLow);
    }

    if (err == TM_OK)
    {
        err = iHCDP_RegWrite(pObj,HCDPRegCtlDisable,0);
    }

    if (err == TM_OK)
    {
        /* AFC enabled */
        err = iHCDP_RegWrite(pObj,HCDPRegAfcDisable,0);
    }

    if (err == TM_OK)
    {
        err = iHCDP_RegWrite(pObj,HCDPRegCtlAccReset,0);
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iHCDP_CheckDemodLocked                                        */
/*                                                                            */
/* DESCRIPTION: Chekcs if the HCDP is locked.                                 */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iHCDP_CheckDemodLocked(
    pHCDPObject_t pObj,
    tmbslFrontEndState_t *peStatus
)
{
    UInt16         uVal = 0;
    tmErrorCode_t  err = TM_OK;

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "iHCDP_CheckDemodLocked(0x%08X)", pObj);

    /* to handle errors */
    *peStatus = tmbslFrontEndStateNotLocked;

    if (err == TM_OK)
    {
        err = iHCDP_RegRead(pObj,HCDPRegLockStatus, &uVal);
    }

    if (err == TM_OK)
    {
        if (0x187F == uVal || 0x186F == uVal)
        {
            *peStatus = tmbslFrontEndStateLocked;
        }
        else
        {
            *peStatus = tmbslFrontEndStateNotLocked;
        }
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    iHCDP_CheckFECLocked                                          */
/*                                                                            */
/* DESCRIPTION: Chekcs if the HCDP is locked.                                 */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iHCDP_CheckFECLocked(
    pHCDPObject_t pObj,
    tmbslFrontEndState_t *peStatus
)
{
    UInt16 uVal = 0;
    tmErrorCode_t       err = TM_OK;

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "iHCDP_CheckFECLocked(0x%08X)", pObj);

    if( (pObj->sCurrentChannel.eFECMode == tmFrontEndFECModeAnnexA) || (pObj->sCurrentChannel.eFECMode == tmFrontEndFECModeAnnexC) )
    {
        /* Annex A */
        err = iHCDP_RegRead(pObj, HCDPRegNxaStatus,&uVal);
        if (err == TM_OK)
        {
            if (0x33!= uVal)
            {
                *peStatus = tmbslFrontEndStateNotLocked; /*not locked*/
            }
            else
            {
                *peStatus = tmbslFrontEndStateLocked;
            }
        }

    }
    else
    {
        /* Annex B */
        err = iHCDP_RegRead(pObj, HCDPRegNxbRsMpegSync,&uVal);
        if (err == TM_OK)
        {
            uVal = uVal & 0x1F; /* mask required due to new fields in RegNxbRsMpegSync for ES3 */
            if ((0x1B != uVal) && (0x13 != uVal))
            {
                *peStatus = tmbslFrontEndStateNotLocked; /*not locked*/
            }
            else
            {
                *peStatus = tmbslFrontEndStateLocked;
            }
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iHCDP_GetEsNo                                                 */
/*                                                                            */
/* DESCRIPTION: Gets the channel EsNo.                                        */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
static tmErrorCode_t
iHCDP_GetEsNo(
    pHCDPObject_t pObj,
    tmFrontEndFracNb32_t *psEsno /* returned esno value */
)
{
    UInt32 uMse = 0;
    UInt32 uResult = 0;
    UInt16 uVal = 0;
    UInt16 uCoef = 0;
    Int32 ii = 0;
    tmErrorCode_t       err = TM_OK;

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "iHCDP_GetEsNo(0x%08X)", pObj);

    if (psEsno == Null)
    {
        err = HCDP_ERR_BAD_PARAMETER;
    }

    if (err == TM_OK)
    {
        psEsno->lInteger = 0;
        psEsno->uDivider = 1;
        /* UMse value latched by UMseLow, so read it first */
        err = iHCDP_RegRead(pObj,HCDPRegUMseLow,&uVal);
    }

    if (err == TM_OK)
    {
        uMse = uVal;
        err = iHCDP_RegRead(pObj,HCDPRegUMseHigh,&uVal);
    }

    if (err == TM_OK)
    {
        uMse |=  (uVal<<16);

        switch(pObj->sCurrentChannel.eMod)
        {
        case tmFrontEndModulationQpsk:
            uCoef = 2;
            break;
        case tmFrontEndModulationQam16:
            uCoef = 10;
            break;
        case tmFrontEndModulationQam32:
            uCoef = 20;
            break;
        case tmFrontEndModulationQam64:
            uCoef = 42;
            break;
        case tmFrontEndModulationQam128:
            uCoef = 82;
            break;
        case tmFrontEndModulationQam256:
            uCoef = 170;
            break;
        default:
            err = HCDP_ERR_NOT_SUPPORTED;
        }

        if (0 == uMse)
        {
            err = HCDP_ERR_NOT_SUPPORTED;
        }
    }

    if (err == TM_OK)
    {
        uResult = (1<<16);
        uResult *= uCoef;
        uResult /= uMse;
    }

    if (err == TM_OK)
    {
        if (sLogTable[0].uX > uResult)
        {
            /* returns the lowest estimated value instead of an error */
            psEsno->lInteger = sLogTable[0].uLogX;
            psEsno->uDivider = 10;
        }
        else
        {
            for (ii = 0; ii <= uLogTableMaxIndex; ii++)
            {
                if (sLogTable[ii].uX >= uResult)
                {
                    psEsno->lInteger = sLogTable[ii].uLogX;
                    psEsno->uDivider = 10;
                    break;
                }
            }

            if (ii > uLogTableMaxIndex)
            {
                /* returns the highest estimated value instead of an error */
                psEsno->lInteger = sLogTable[uLogTableMaxIndex].uLogX;
                psEsno->uDivider = 10;
            }
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iHCDP_StartBER                                                */
/*                                                                            */
/* DESCRIPTION: Starts the BER measurement.                                   */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
static tmErrorCode_t
iHCDP_StartBER(
    pHCDPObject_t pObj
)
{
    UInt16 uVal = 0;
    tmErrorCode_t       err = TM_OK;

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "iHCDP_StartBER(0x%08X)", pObj);

    
    // initialise variables used for BER measurement
    pObj->uStoredBitErrs = 0;
    pObj->uStoredUncorrectable = 0;
    pObj->uStoredCorrected = 0;
    pObj->uStoredNonerrors = 0;
    pObj->sPreviousBER.lInteger = 0;
    pObj->sPreviousBER.uDivider = 1; /* avoid divide by 0 */

    uVal = 4;

    err = iHCDP_RegWrite(pObj,HCDPRegRsDecodWinMode,uVal);

    if (err == TM_OK)
    {
        err = iHCDP_RegWrite(pObj,HCDPRegRsDecodClear,1);
    }

    if (err == TM_OK)
    {
        err = iHCDP_RegWrite(pObj,HCDPRegRsDecodFreeze,0);
    }

    if (err == TM_OK)
    {
        err = iHCDP_RegRead(pObj,HCDPRegRsDecodWindowCount,&pObj->usBerWindowCount);
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iHCDP_GetBER                                                  */
/*                                                                            */
/* DESCRIPTION: Gets the BER.                                                 */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
static tmErrorCode_t
iHCDP_GetBER(
    pHCDPObject_t pObj,
    tmFrontEndFracNb32_t *psBER,
    UInt32 *puUncors
)
{
    UInt16 uValLow = 0;
    UInt16 uValHigh = 0;
    UInt32 uUncorrectable = 0;
    UInt32 uCorrected = 0;
    UInt32 uBitErrs = 0;
    UInt32 uSymErrs = 0;
    UInt32 uNonerrors = 0;
    
    UInt32 ulSymbolsPerPacket;
    UInt32 ulBitsPerPacket;
    UInt32 ulUncorBits;
    UInt32 ulUncorSymbols;

    tmbslFrontEndState_t eLockStatus;

    UInt32 uStoredNonerrorsSave = 0;
    UInt32 uStoredBitErrsSave = 0;
    UInt32 uStoredUncorrectableSave = 0;
    UInt32 uStoredCorrectedSave = 0;

    UInt32 uSamplesNb = 0;

    tmErrorCode_t       err = TM_OK;

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "iHCDP_GetBER(0x%08X)", pObj);

    /* initialise output structures */
    if (psBER != Null)
    {
        psBER->lInteger = 1;
        psBER->uDivider = 1;
    }
    else
    {
        err = TM_ERR_BAD_PARAMETER;
    }
        
    if (err == TM_OK)
    {
        if (puUncors != Null)
        {
            *puUncors = 0;
        }
        else
        {
            err = TM_ERR_BAD_PARAMETER;
        }
    }

    if (err == TM_OK)
    {
        /* check FE is locked */
        err = iHCDP_CheckFECLocked(pObj, &eLockStatus);
    }
    
    if (err == TM_OK)
    {
        if (eLockStatus == tmbslFrontEndStateLocked)
        {
            /* configure constants depending on standard */
            if( (pObj->sCurrentChannel.eFECMode == tmFrontEndFECModeAnnexA) || (pObj->sCurrentChannel.eFECMode == tmFrontEndFECModeAnnexC) )
            {
                /*Annex A */
                ulBitsPerPacket = BITS_PER_PACKET_DVBC;
                ulSymbolsPerPacket = SYMBOLS_PER_PACKET_DVBC;
                ulUncorBits = UNCOR_BITS_DVBC;
                ulUncorSymbols = UNCOR_SYMBOLS_DVBC;
            }
            else
            {
                /*Annex B*/
                ulBitsPerPacket = BITS_PER_PACKET_J83B;
                ulSymbolsPerPacket = SYMBOLS_PER_PACKET_J83B;
                ulUncorBits = UNCOR_BITS_J83B;
                ulUncorSymbols = UNCOR_SYMBOLS_J83B;
            }

            switch (pObj->eBERWindow)
            {
            case HCDP_BERWindow_1E5:
                uSamplesNb = 100000;
                break;
            case HCDP_BERWindow_1E6:
                uSamplesNb = 1000000;
                break;
            case HCDP_BERWindow_1E7:
                uSamplesNb = 10000000;
                break;
            case HCDP_BERWindow_1E8:
                uSamplesNb = 100000000;
                break;
            default:
                uSamplesNb = 10000000;
                break;
            }

            /* do not check HCDPRegRsDecodFreeze because it does not have any meaning (only toggling) */

            if (err == TM_OK)
            {
                /* infinite window case */
                /* copy internal to external registers */
                /* (does not mean that something is frozen) */
                err = iHCDP_RegWrite(pObj,HCDPRegRsDecodFreeze,1);
            
                if (err == TM_OK)
                {
                    err = iHCDP_RegRead(pObj,HCDPRegRsDecodBitErrsHigh,&uValHigh);
                }
                        
                if (err == TM_OK)
                {
                    err = iHCDP_RegRead(pObj,HCDPRegRsDecodBitErrsLow,&uValLow);
                }
                
                if (err == TM_OK)
                {
                    uBitErrs = ((UInt32)uValHigh << 16) |uValLow;

                    err = iHCDP_RegRead(pObj,HCDPRegRsDecodSymErrsHigh,&uValHigh);
                }
                
                if (err == TM_OK)
                {
                    err = iHCDP_RegRead(pObj,HCDPRegRsDecodSymErrsLow,&uValLow);
                }

                if (err == TM_OK)
                {
                    uSymErrs = ((UInt32)uValHigh << 16) |uValLow;

                    err = iHCDP_RegRead(pObj,HCDPRegRsDecodUncorrectableCwHigh,&uValHigh);
                }

                if (err == TM_OK)
                {
                    err = iHCDP_RegRead(pObj,HCDPRegRsDecodUncorrectableCwLow,&uValLow);
                }

                if (err == TM_OK)
                {
                    uUncorrectable = ((UInt32)uValHigh << 16) |uValLow;

                    err = iHCDP_RegRead(pObj,HCDPRegRsDecodCorrectedCwHigh,&uValHigh);
                }

                if (err == TM_OK)
                {
                    err = iHCDP_RegRead(pObj,HCDPRegRsDecodCorrectedCwLow,&uValLow);
                }

                if (err == TM_OK)
                {
                    uCorrected = ((UInt32)uValHigh << 16) |uValLow;

                    err = iHCDP_RegRead(pObj,HCDPRegRsDecodUnerroredCwHigh,&uValHigh);
                }

                if (err == TM_OK)
                {
                    err = iHCDP_RegRead(pObj,HCDPRegRsDecodUnerroredCwLow,&uValLow);
                }

                if (err == TM_OK)
                {
                    uNonerrors = ((UInt32)uValHigh << 16) |uValLow;

                    /* uncor blocks */
                    /* return all errors since startup has been done */
                    /* note that *puUncors will automatically wrap */
                    if (uUncorrectable >= pObj->uBaseUncors)
                    {
                        *puUncors = uUncorrectable - pObj->uBaseUncors;
                    }
                    else
                    {
                        /* the Uncorrectable register wrapped */
                        *puUncors = 0xFFFFFFFF - pObj->uBaseUncors + uUncorrectable;
                    }

                    /* manage the potential register wrap around */
                    uStoredBitErrsSave = pObj->uStoredBitErrs;
                    if( uBitErrs < pObj->uStoredBitErrs )
                    {
                        uBitErrs += 0xFFFFFFFF - pObj->uStoredBitErrs + 1;
                        pObj->uStoredBitErrs = 0;
                    }

                    uStoredUncorrectableSave = pObj->uStoredUncorrectable;
                    if( uUncorrectable < pObj->uStoredUncorrectable )
                    {
                        uUncorrectable += 0xFFFFFFFF - pObj->uStoredUncorrectable + 1;
                        pObj->uStoredUncorrectable = 0;
                    }
                    
                    uStoredCorrectedSave = pObj->uStoredCorrected;
                    if( uCorrected < pObj->uStoredCorrected )
                    {
                        uCorrected += 0xFFFFFFFF - pObj->uStoredCorrected + 1;
                        pObj->uStoredCorrected = 0;
                    }

                    uStoredNonerrorsSave = pObj->uStoredNonerrors;
                    if( uNonerrors < pObj->uStoredNonerrors )
                    {
                        uNonerrors += 0xFFFFFFFF - pObj->uStoredNonerrors + 1;
                        pObj->uStoredNonerrors = 0;
                    }

                    /* BER */
                    /* this is the measurement since the last call of GetBER function */
                    psBER->lInteger = (uBitErrs - pObj->uStoredBitErrs) + ulUncorBits*(uUncorrectable - pObj->uStoredUncorrectable);
                    psBER->uDivider = ((uUncorrectable - pObj->uStoredUncorrectable) + (uCorrected - pObj->uStoredCorrected) + (uNonerrors - pObj->uStoredNonerrors));
                    /* check that it does not go out of 32 bits */
                    if( psBER->uDivider < (0xFFFFFFFF/ulBitsPerPacket) )
                    {
                        psBER->uDivider = ulBitsPerPacket * psBER->uDivider;

                        if( psBER->uDivider < uSamplesNb )
                        {
                            /* calculation window is too small */
                            /* return the previous value */
                            psBER->lInteger = pObj->sPreviousBER.lInteger;
                            psBER->uDivider = pObj->sPreviousBER.uDivider;

                            /* The previous state is restored until the BER window actually changes */
                            pObj->uStoredBitErrs = uStoredBitErrsSave;
                            pObj->uStoredUncorrectable = uStoredUncorrectableSave;
                            pObj->uStoredCorrected = uStoredCorrectedSave;
                            pObj->uStoredNonerrors = uStoredNonerrorsSave;
                        }
                        else
                        {
                            /* store current BER */
                            pObj->sPreviousBER.lInteger = psBER->lInteger;
                            pObj->sPreviousBER.uDivider = psBER->uDivider;

                            /* store current register values for next time */
                            pObj->uStoredBitErrs = uBitErrs;
                            pObj->uStoredUncorrectable = uUncorrectable;
                            pObj->uStoredCorrected = uCorrected;
                            pObj->uStoredNonerrors = uNonerrors;
                        }

                    }
                    else
                    {
                        /* Bit number overflow */
                        /* in case of overflow, reset the variable */
                        pObj->uStoredBitErrs = uBitErrs;
                        pObj->uStoredUncorrectable = uUncorrectable;
                        pObj->uStoredUncorrectable = uUncorrectable; 
                        pObj->uStoredCorrected = uCorrected; 
                        pObj->uStoredNonerrors = uNonerrors;

                        /* we also return the last BER */
                        psBER->lInteger = pObj->sPreviousBER.lInteger;
                        psBER->uDivider = pObj->sPreviousBER.uDivider;
                    }
                }
            }
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iHCDP_StopBER                                                 */
/*                                                                            */
/* DESCRIPTION: Stops the BER measurement.                                    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
static tmErrorCode_t
iHCDP_StopBER(
    pHCDPObject_t pObj
)
{
    tmErrorCode_t       err = TM_OK;

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "iHCDP_StopBER(0x%08X)", pObj);

    err = iHCDP_RegWrite(pObj,HCDPRegRsDecodFreeze,1);

    if (err == TM_OK)
    {
        err = iHCDP_RegWrite(pObj,HCDPRegRsDecodClear,1);
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iHCDP_SetRFAGCThreshold                                       */
/*                                                                            */
/* DESCRIPTION: Sets the RF AGC threshold.                                    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iHCDP_SetRFAGCThreshold(
    pHCDPObject_t pObj,
    UInt16 uRFAGCThrLow,
    UInt16 uRFAGCThrHigh
)
{
    tmErrorCode_t       err = TM_OK;
    
    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "iHCDP_SetRFAGCThreshold(0x%08X)", pObj);

    err = iHCDP_RegWrite(pObj,HCDPRegAagcEmThLow1,uRFAGCThrLow);

    if (err == TM_OK)
    {
        err = iHCDP_RegWrite(pObj,HCDPRegAagcEmThHigh1,uRFAGCThrHigh);
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iHCDP_SetExtendSymbolRateRange                                */
/*                                                                            */
/* DESCRIPTION: Set Extend the SR recovery range.                             */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
static tmErrorCode_t
iHCDP_SetExtendSymbolRateRange(
    pHCDPObject_t pObj,
    HCDP_ExtendSymbolRateMode_t eMode
)
{
    tmErrorCode_t err = TM_OK;
    UInt32 uTemp = 0;
    Int16 iTemp = 0;
    UInt32 uSRMinPpm = 0;
    UInt32 uSRMaxPpm = 0;

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "iHCDP_ExtendSymbolRate(0x%08X)", pObj);

#define MIN_SR_FOR_EXTENDED_MODE 3000000 /* in Msym/s */

    /* check input parameter */
    switch (eMode)
    {
    case HCDP_ExtendSymbolRateModeEnable700ppm:
        uSRMinPpm = 1000;
        uSRMaxPpm = 500;
        break;
    case HCDP_ExtendSymbolRateModeEnable1500ppm:
        uSRMinPpm = 2000;
        uSRMaxPpm = 1500;
        break;
    case HCDP_ExtendSymbolRateModeDisable:
        break;
    default: /* HCDP_ExtendSymbolRateModeInvalid */
        err = HCDP_ERR_BAD_PARAMETER;
    }

    /* StlSweep (applicable to ES2/ES3 only) */
    if ((pObj->eHwCdpSample == HCDP_SAMPLE_ES2) ||
        (pObj->eHwCdpSample == HCDP_SAMPLE_ES3))
    {
        if ((eMode != HCDP_ExtendSymbolRateModeDisable) &&
            (pObj->sCurrentChannel.uSR >= MIN_SR_FOR_EXTENDED_MODE))
        {
            if (err == TM_OK)
            {
                /* SR step(Sym/s) = step * Fs(Hz) / 2^20 */
                /* (step == 0) means StlSweep is not used */
                /* in ppm */
                uTemp = 25;

                /* value proportionnaly increased for lower SR */
                uTemp *= (6900000/1000);
                uTemp /= (pObj->sCurrentChannel.uSR/1000);

                /* in KSym/s */
                uTemp *= (pObj->sCurrentChannel.uSR/1000); 
                uTemp <<= 10;
                uTemp /= 1000;
                uTemp <<= 10;
                /* for rounding */
                uTemp += pObj->sConfig.uSamplingClock/2;
                uTemp /= pObj->sConfig.uSamplingClock;
                if (uTemp == 0)
                {
                    uTemp = 1;
                }
                err = iHCDP_RegWrite(pObj,HCDPRegStlSweepStep, (UInt16)uTemp);
            }

            if (err == TM_OK)
            {
                /* step duration(us) = step_time * 2^6 / SR(Sym/s) */
                /* in us (tuned for 6.9MSps) */
                uTemp = 200;

                /* value proportionnaly increased for lower SR */
                uTemp *= (6900000/1000); 
                uTemp /= (pObj->sCurrentChannel.uSR/1000);

                uTemp *= (pObj->sCurrentChannel.uSR/1000);
                uTemp >>= 6;
                /* for rounding */
                uTemp += 1000/2;
                uTemp /= 1000;
                err = iHCDP_RegWrite(pObj,HCDPRegStlSweepStepTime, (UInt16)uTemp);
            }

            if (err == TM_OK)
            {
                /* min SR offset(Sym/s) = sweep_min * Fs(Hz) / 2^20 (signed value) */
                /* symbol rate offset in ppm */
                uTemp = uSRMinPpm;
                uTemp *= (pObj->sCurrentChannel.uSR/1000);
                /* in Sym/s */
                uTemp /= 1000; 
                uTemp <<= 10;
                uTemp /= (pObj->sConfig.uSamplingClock/1000);
                uTemp <<= 10;
                /* for rounding */
                uTemp += 1000/2;
                uTemp /= 1000;
                iTemp = -(Int16)uTemp;
                err = iHCDP_RegWrite(pObj,HCDPRegStlSweepMin, (UInt16)iTemp);
            }

            if (err == TM_OK)
            {
                /* max SR offset(Sym/s) = sweep_max * Fs(Hz) / 2^20 (signed value) */
                /* symbol rate offset in ppm */
                uTemp = uSRMaxPpm;
                uTemp *= (pObj->sCurrentChannel.uSR/1000);
                /* in Sym/s */
                uTemp /= 1000; 
                uTemp <<= 10;
                uTemp /= (pObj->sConfig.uSamplingClock/1000);
                uTemp <<= 10;
                /* for rounding */
                uTemp += 1000/2;
                uTemp /= 1000;
                err = iHCDP_RegWrite(pObj,HCDPRegStlSweepMax, (UInt16)uTemp);
            }

            if (err == TM_OK)
            {
                /* timer duration(s) = timer * 2^12 / SR(Sym/s) */
                /* in ms (tuned for 6.9MSps) */
                uTemp = 100;

                /* value proportionnaly increased for lower SR */
                uTemp *= (6900000/1000); 
                uTemp /= (pObj->sCurrentChannel.uSR/1000);

                uTemp *= (pObj->sCurrentChannel.uSR/1000);
                uTemp >>= 12;
                err = iHCDP_RegWrite(pObj,HCDPRegStlSweepTimer, (UInt16)uTemp);
            }
        }
        else
        {
            /* extended mode is disabled */
            /* only when the mode change */

            if ((eMode != pObj->sCurrentChannel.eExtendedSRModeCurrent)
                || (pObj->sCurrentChannel.uSR < MIN_SR_FOR_EXTENDED_MODE))
            {
                /* write default values */
                if (err == TM_OK)
                {
                    err = iHCDP_RegWrite(pObj,HCDPRegStlSweepStep, 0);
                }

                if (err == TM_OK)
                {
                    err = iHCDP_RegWrite(pObj,HCDPRegStlSweepStepTime, 0);
                }

                if (err == TM_OK)
                {
                    err = iHCDP_RegWrite(pObj,HCDPRegStlSweepMin, 0);
                }

                if (err == TM_OK)
                {
                    err = iHCDP_RegWrite(pObj,HCDPRegStlSweepMax, 0);
                }

                if (err == TM_OK)
                {
                    err = iHCDP_RegWrite(pObj,HCDPRegStlSweepTimer, 0);
                }
            }
        }
    }

    /* store current configuration */
    if (err == TM_OK)
    {
        pObj->sCurrentChannel.eExtendedSRModeCurrent = eMode;
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iHCDP_GetAnxBInterleaverMode                                  */
/*                                                                            */
/* DESCRIPTION: Gets actual annex B interleaver mode.                         */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
static tmErrorCode_t
iHCDP_GetAnxBInterleaverMode(
    pHCDPObject_t pObj,
    HCDP_InterleaverMode_t *peInterleaverMode
)
{
    UInt16 uMode = 0;
    tmErrorCode_t       err = TM_OK;

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "iHCDP_GetAnxBInterleaverMode(0x%08X)", pObj->tUnit);

    err =iHCDP_RegRead(pObj,HCDPRegNxbDeintActualMode,&uMode);

    if (err == TM_OK)
    {
        if (HCDP_InterleaverMode_8_16 < uMode)
        {
            err  = HCDP_ERR_NOT_SUPPORTED;  /*illeagal interleaver mode in FEC B*/
        }
    }

    if (err == TM_OK)
    {
        switch (uMode)
        {
        case 0:
            *peInterleaverMode = HCDP_InterleaverMode_128_1_A;
            break;
        case 1:
            *peInterleaverMode = HCDP_InterleaverMode_128_1_B;
            break;
        case 2:
            *peInterleaverMode = HCDP_InterleaverMode_128_2;
            break;
        case 3:
            *peInterleaverMode = HCDP_InterleaverMode_64_2;
            break;
        case 4:
            *peInterleaverMode = HCDP_InterleaverMode_128_3;
            break;
        case 5:
            *peInterleaverMode = HCDP_InterleaverMode_32_4;
            break;
        case 6:
            *peInterleaverMode = HCDP_InterleaverMode_128_4;
            break;
        case 7:
            *peInterleaverMode = HCDP_InterleaverMode_16_8;
            break;
        case 8:
            *peInterleaverMode = HCDP_InterleaverMode_NA;
            break;
        case 9:
            *peInterleaverMode = HCDP_InterleaverMode_8_16;
            break;
        default:
            err = HCDP_ERR_NOT_SUPPORTED;
            break;
        }
    }

    return err;
}

#ifdef NOT_IMPLEMENTED
/*============================================================================*/
/* FUNCTION:    iHCDP_Write                                                    */
/*                                                                            */
/* DESCRIPTION: Writes bytes on the HCDP.                                     */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iHCDP_Write(
    pHCDPObject_t pObj,
    UInt16 uIndex_U,
    UInt32 uNBytes_U,
    UInt16* puData_U
)
{
    tmErrorCode_t       err = TM_OK;

    tmDBGPRINTEx(DEBUGLVL_BLAB, "iHCDP_Write(0x%08X)", pObj->tUnit);
    
    err = P_SIO_WRITE(pObj->tUnit, 2, (UInt8*)&uIndex_U, uNBytes_U*2, (UInt8*)puData_U);

    return err;
}

/*============================================================================*/
/* FUNCTION:    iHCDP_Read                                                    */
/*                                                                            */
/* DESCRIPTION: Reads bytes on the HCDP.                                      */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iHCDP_Read(
    pHCDPObject_t pObj,
    UInt16 uIndex_U,
    UInt32 uNBytes_U,
    UInt16* puData_U
)
{
    tmErrorCode_t       err = TM_OK;

    tmDBGPRINTEx(DEBUGLVL_BLAB, "iHCDP_Read(0x%08X)", pObj->tUnit);
    
    err = P_SIO_READ(pObj->tUnit, 2, (UInt8*)&uIndex_U, uNBytes_U*2, (UInt8*)puData_U);
    
    return err;
}

/*============================================================================*/
/* FUNCTION:    iHCDP_WriteBit                                                */
/*                                                                            */
/* DESCRIPTION: Writes a masked byte on the HCDP.                             */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iHCDP_WriteBit(
    pHCDPObject_t pObj,
    UInt16 uIndex_U,
    UInt16 uMask_U,
    UInt16 uData_U
)
{
    tmErrorCode_t       err = TM_OK;
    UInt16              byte;

    tmDBGPRINTEx(DEBUGLVL_BLAB, "iHCDP_WriteBit(0x%08X)", pObj->tUnit);
    
    err = iHCDP_Read(pObj, uIndex_U, 1, &byte);
    
    if (err == TM_OK)
    {
        byte = (~(UInt16)uMask_U & byte) | ((UInt16)uMask_U & (UInt16)uData_U);
        err = iHCDP_Write(pObj, uIndex_U, 1, &byte);
    }

    return err;
}
#endif

/*============================================================================*/
/* FUNCTION:    iHCDP_Wait                                                    */
/*                                                                            */
/* DESCRIPTION: Waits for requested time.                                     */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t 
iHCDP_Wait(
    pHCDPObject_t pObj,   /* I: Driver object */
    UInt32 uTime    /* I: time to wait for (in ms) */
)
{
    tmErrorCode_t   err = HCDP_ERR_NULL_CONTROLFUNC;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iHCDP_Wait(0x%08X)", pObj->tUnit);

    if(P_STIME_WAIT_VALID)
    {
        /* Wait Time ms */
        err = P_STIME_WAIT(pObj->tUnit, uTime);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "Time_Wait(0x%08X, %d) failed.", pObj->tUnit, uTime));
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iHCDP_RegWrite                                                */
/*                                                                            */
/* DESCRIPTION: Writes given field in proper register.                        */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iHCDP_RegWrite(
    pHCDPObject_t pObj,
    HCDPRegisterIndex_t eRegIndex,
    UInt16 uValue
)
{
    UInt16         uRegReading = 0;
    tmErrorCode_t  err = TM_OK;

    //tmDBGPRINTEx(DEBUGLVL_VERBOSE, "iHCDP_RegWrite(0x%08X)", tUnit);

    if (eRegIndex != sRegisterMap[eRegIndex].eID)
    {
        err = HCDP_ERR_BAD_PARAMETER;
    }

    if( err == TM_OK)
    {
        /*read 16 bits*/
        err = P_SIO_READ(pObj->tUnit, 2, (UInt8*)&sRegisterMap[eRegIndex].uAddress, 2, (UInt8*)&uRegReading);
    }
    
    if (err == TM_OK)
    {
        uRegReading &= ~(sRegisterMap[eRegIndex].uMask);
        uRegReading |= ((uValue << sRegisterMap[eRegIndex].uShift) & sRegisterMap[eRegIndex].uMask);

        /*write 16 bits */
        err = P_SIO_WRITE(pObj->tUnit, 2, (UInt8*)&sRegisterMap[eRegIndex].uAddress, 2, (UInt8*)&uRegReading);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    iHCDP_RegRead                                                 */
/*                                                                            */
/* DESCRIPTION: Reads given field in proper register.                         */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iHCDP_RegRead(
    pHCDPObject_t pObj,
    HCDPRegisterIndex_t eRegIndex,
    UInt16 *puValue
)
{
    UInt16 uRegReading = 0;

    tmErrorCode_t       err = TM_OK;

    //tmDBGPRINTEx(DEBUGLVL_VERBOSE, "iHCDP_RegRead(0x%08X)", tUnit);

    if (eRegIndex != sRegisterMap[eRegIndex].eID)
    {
         err = HCDP_ERR_BAD_PARAMETER;
    }

    if (err == TM_OK)
    {
        /*read 16 bits*/
        err = P_SIO_READ(pObj->tUnit, 2, (UInt8*)&sRegisterMap[eRegIndex].uAddress, 2, (UInt8*)&uRegReading);
    }

    if (err == TM_OK)
    {
        *puValue = (uRegReading & sRegisterMap[eRegIndex].uMask) >> sRegisterMap[eRegIndex].uShift;
    }

    return err;
}

#ifdef _TVFE_IMPLEMENT_MUTEX
/*============================================================================*/
/* FUNCTION:    iHCDP_MutexAcquire                                            */
/*                                                                            */
/* DESCRIPTION: Acquires driver mutex.                                        */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
iHCDP_MutexAcquire(
    pHCDPObject_t   pObj,
    UInt32              timeOut
)
{
    tmErrorCode_t   err = HCDP_ERR_NULL_CONTROLFUNC;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iHCDP_MutexAcquire(0x%08X)", pObj->tUnitW);

    if(P_SMUTEX_ACQUIRE_VALID && P_MUTEX_VALID)
    {
        err = P_SMUTEX_ACQUIRE(P_MUTEX, timeOut);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "Mutex_Acquire(0x%08X, %d) failed.", pObj->tUnitW, timeOut));
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iHCDP_MutexRelease                                            */
/*                                                                            */
/* DESCRIPTION: Releases driver mutex.                                        */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
iHCDP_MutexRelease(
    pHCDPObject_t   pObj
)
{
    tmErrorCode_t   err = HCDP_ERR_NULL_CONTROLFUNC;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iHCDP_MutexRelease(0x%08X)", pObj->tUnitW);

    if(P_SMUTEX_RELEASE_VALID && P_MUTEX_VALID)
    {
        err = P_SMUTEX_RELEASE(P_MUTEX);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "Mutex_Release(0x%08X) failed.", pObj->tUnitW));
    }

    return err;
}
#endif

#ifdef TMBSL_HCDP_NO_64BITS
UInt32 iHCDP_Division32(UInt32 ulNum1, UInt32 ulNum2, UInt32 ulDenom)
{
    UInt32 ulRes = 0;
    UInt32 ulTmp;
    UInt32 ulRemain = 0;
    UInt32 ulRemainPrecision = 0;
    UInt32 ulPrecision;
    UInt32 ulPrecisionMax = 100000;
    Bool bSwap = False; 

//#define LONG_DEBUG
#ifdef LONG_DEBUG
    unsigned long long ulDebugResult;

    ulDebugResult = (long long)ulNum1 * (long long)ulNum2;
    ulDebugResult /= ulDenom;
#endif

    /* try to find out the best numerator */ 
    /* calculation will be do on ulNum1 */ 
    if( ulNum1 % ulDenom == 0 ) 
    {
        bSwap = False; 
    }
    else if( ulNum2 % ulDenom == 0) 
    {
        bSwap = True; 
    }
    else if( ulDenom % ulNum1 == 0) 
    {
        bSwap = False; 
    }
    else if( ulDenom % ulNum2 == 0) 
    {
        bSwap = True; 
    }
    else if(ulNum1 < ulNum2) 
    {
        bSwap = True; 
    }
    if( bSwap ) 
    {
        ulTmp = ulNum1;
        ulNum1 = ulNum2;
        ulNum2 = ulTmp;
    }
    
    // result (order 1)
    ulRes = ulNum1 / ulDenom;
    ulRes *= ulNum2;
    
    // remain of the division
    ulRemain = ulNum1 % ulDenom;

    ulRemainPrecision = 0;
    ulPrecision = 1;
    
    do 
    {
        ulPrecision *= 10;
        ulNum1 = ulRemain;
        ulNum1 *= 10;
        ulRemain = ulNum1 % ulDenom;
        ulNum1 /= ulDenom;
        ulNum1 *= ulNum2; 
        ulNum1 += ulRemainPrecision * 10;
        ulRes += ulNum1 / ulPrecision;
        ulRemainPrecision = ulNum1 % ulPrecision;
    } while (ulPrecision != ulPrecisionMax); 

#ifdef LONG_DEBUG
    if( ulDebugResult != ulRes )
    {
        return (UInt32)ulDebugResult;
    }
#endif

    return ulRes; 
}

#endif
