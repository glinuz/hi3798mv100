/*
  Copyright (C) 2010 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmbslHCDP_Advanced.c
 *
 *                %version: 3 %
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
#include "tmbslHCDP_Advanced.h"

#include "tmbslHCDP_Reg.h"
#include "tmbslHCDP_Local.h"
#include "tmbslHCDP_Instance.h"

/*============================================================================*/
/* Static internal functions:                                                 */
/*============================================================================*/
static Int32
iHCDP_GetConPhyData(pHCDPObject_t pObj, UInt16 uRawData);

static tmErrorCode_t
iHCDP_EqualizerGetPoints(pHCDPObject_t pObj);

/*============================================================================*/
/* Exported functions:                                                        */
/*============================================================================*/

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_SetClocks                                           */
/*                                                                            */
/* DESCRIPTION: Sets the clocks.                                              */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_SetClocks(
    tmUnitSelect_t tUnit,
    UInt32 uSamplingClock,
    UInt32 uDClock
)
{
    pHCDPObject_t pObj = Null;
    tmErrorCode_t err = TM_OK;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);
    
    _MUTEX_ACQUIRE(HCDP)

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslHCDP_SetClocks(0x%08X)", tUnit);

    if (err == TM_OK)
    {
        pObj->sConfig.uDClock = uDClock;
        pObj->sConfig.uSamplingClock = uSamplingClock;
    }

    _MUTEX_RELEASE(HCDP)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_GetClocks                                           */
/*                                                                            */
/* DESCRIPTION: Gets the clocks.                                              */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_GetClocks(
    tmUnitSelect_t tUnit,
    UInt32 *puSamplingClock,
    UInt32* puDClock
)
{
    pHCDPObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);
    
    _MUTEX_ACQUIRE(HCDP)

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslHCDP_GetClocks(0x%08X)", tUnit);

    if (err == TM_OK)
    {
        *puSamplingClock = pObj->sConfig.uSamplingClock;
        *puDClock = pObj->sConfig.uDClock;
    }

    _MUTEX_RELEASE(HCDP)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_SetIFAGCThreshold                                   */
/*                                                                            */
/* DESCRIPTION: Sets the demod IF AGC threshold.                              */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_SetIFAGCThreshold(
    tmUnitSelect_t tUnit,
    UInt16 uIFAGCThr
)
{
    pHCDPObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);
    
    _MUTEX_ACQUIRE(HCDP)

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslHCDP_SetIFAGCThreshold(0x%08X)", tUnit);

    if (err == TM_OK)
    {
        pObj->sConfig.uIFAGCThreshold = uIFAGCThr;
    
        err = iHCDP_RegWrite( pObj, HCDPRegAagcEmThr0, uIFAGCThr);
    }

    _MUTEX_RELEASE(HCDP)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_GetIFAGCThreshold                                   */
/*                                                                            */
/* DESCRIPTION: Gets the demod IF AGC threshold.                              */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_GetIFAGCThreshold(
    tmUnitSelect_t tUnit,
    UInt16 *puIFAGCThr
)
{
    UInt16  uVal = 0;
    pHCDPObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);
    
    _MUTEX_ACQUIRE(HCDP)

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslHCDP_GetIFAGCThreshold(0x%08X)", tUnit);

    if (err == TM_OK)
    {
        err = iHCDP_RegRead( pObj, HCDPRegAagcEmThr0, &uVal );
    }

    if (err == TM_OK)
    {
        pObj->sConfig.uIFAGCThreshold = uVal;
        *puIFAGCThr = pObj->sConfig.uIFAGCThreshold;
    }

    _MUTEX_RELEASE(HCDP)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_SetRFAGCThreshold                                   */
/*                                                                            */
/* DESCRIPTION: Sets the demod RF AGC threshold.                              */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_SetRFAGCThreshold(
    tmUnitSelect_t tUnit,
    UInt16 uRFAGCThrLow,
    UInt16 uRFAGCThrHigh
)
{
    pHCDPObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);
    
    _MUTEX_ACQUIRE(HCDP)

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslHCDP_SetRFAGCThreshold(0x%08X)", tUnit);

    if (err == TM_OK)
    {
        err = iHCDP_SetRFAGCThreshold(pObj,uRFAGCThrLow,uRFAGCThrHigh);
    }

    _MUTEX_RELEASE(HCDP)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_ConstPreset                                         */
/*                                                                            */
/* DESCRIPTION: Initialises the constellation getting.                        */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_ConstPreset(
    tmUnitSelect_t tUnit,
    HCDP_ConstellationSource_t eSource
)
{
    UInt8 mode = 0;
    
    pHCDPObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);
    
    _MUTEX_ACQUIRE(HCDP)

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslHCDP_ConstPreset(0x%08X)", tUnit);

    if (err == TM_OK)
    {
        switch (eSource)
        {
            case HCDP_ConstellationSource_ADC:
                mode = 0;
                pObj->usConDMask = 0xFFF; /*12 bits*/
                pObj->usConDScale = 2048;
                break;
            case HCDP_ConstellationSource_FEDR:
                mode = 1;
                pObj->usConDMask = 0x1FFF; /*13 bits*/
                pObj->usConDScale = 4096;
                break;
            case HCDP_ConstellationSource_PDF:
                pObj->usConDMask = 0x7FFF; /*15 bits*/
                pObj->usConDScale = 16384;
                mode = 2;
                break;
            case HCDP_ConstellationSource_DAGC:
                pObj->usConDMask = 0x1FFF; /*13 bits*/
                pObj->usConDScale = 4096;
                mode = 3;
                break;
            case HCDP_ConstellationSource_MF:
                pObj->usConDMask = 0x3FFF; /*14 bits*/
                pObj->usConDScale = 8192;
                mode = 4;
                break;
            case HCDP_ConstellationSource_CAGC:
                pObj->usConDMask = 0x1FFF; /*13 bits*/
                pObj->usConDScale = 4096 ;
                mode = 5;
                break;
            case HCDP_ConstellationSource_Equalizer:
                pObj->usConDMask = 0x1FFF; /*13 bits*/
                pObj->usConDScale = 4096;
                mode = 6;
                break;
            case HCDP_ConstellationSource_BEDR:
                pObj->usConDMask = 0x1FFF;  /*13 bits*/
                pObj->usConDScale = 4096;
                mode = 7;
                break;
            default:
                err = HCDP_ERR_NOT_SUPPORTED;
                break;
        }
    }

    if (err == TM_OK)
    {
        err = iHCDP_RegWrite(pObj,HCDPRegConstellationCtrl,mode);
    }

    if (err == TM_OK)
    {
        err = iHCDP_RegWrite(pObj,HCDPRegConstellationData,0xFF);
    }

    _MUTEX_RELEASE(HCDP)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_ConstGetIQ                                          */
/*                                                                            */
/* DESCRIPTION: Gets the current IQ.                                          */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_ConstGetIQ(
    tmUnitSelect_t tUnit,
    tmFrontEndFracNb32_t *pI,
    tmFrontEndFracNb32_t *pQ
)
{
    UInt16 uVal;

    pHCDPObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(HCDP)

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslHCDP_ConstGetIQ(0x%08X)", tUnit);

    /* initialize to handle errors */
    pI->lInteger = 0;
    pI->uDivider = 1;
    pQ->lInteger = 0;
    pQ->uDivider = 1;

    if (err == TM_OK)
    {
        err = iHCDP_RegRead(pObj,HCDPRegConstellationData,&uVal);
    }

    if (err == TM_OK)
    {
        /* when drawing graph,using: float i = (float)pI->lInteger / (float)pI->uDivider;
             the result I value will between (-1.0 ~ 1.0)         */
        if (uVal%2)
        {
            pQ->lInteger = iHCDP_GetConPhyData(pObj, uVal>>1);
            pQ->uDivider = pObj->usConDScale;
        }
        else
        {
            err = iHCDP_RegRead(pObj,HCDPRegConstellationData,&uVal);
            
            if (err == TM_OK)
            {
                if (uVal%2)
                {
                    pQ->lInteger = iHCDP_GetConPhyData(pObj, uVal>>1);
                    pQ->uDivider = pObj->usConDScale;
                }
                else
                {
                    err = HCDP_ERR_HW_FAILED;
                }
            }
        }
    
        if (err == TM_OK)
        {
            err = iHCDP_RegRead(pObj,HCDPRegConstellationData,&uVal);
        }

        if (err == TM_OK)
        {
            /* when drawing graph,using: float i = (float)pQ->lInteger / (float)pQ->uDivider;
                 the result Q value will between (-1.0 ~ 1.0)         */
            if (!(uVal%2))
            {
                pI->lInteger = iHCDP_GetConPhyData(pObj, uVal>>1);
                pI->uDivider = pObj->usConDScale;
            }
            else
            {
                err = HCDP_ERR_HW_FAILED;
            }
        }
    }

    _MUTEX_RELEASE(HCDP)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_FFTPreset                                           */
/*                                                                            */
/* DESCRIPTION: Initialises the FFT getting.                                  */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_FFTPreset(
    tmUnitSelect_t tUnit
)
{
    pHCDPObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);
    
    _MUTEX_ACQUIRE(HCDP)

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslHCDP_FFTPreset(0x%08X)", tUnit);

    if (err == TM_OK)
    {
        err =  iHCDP_RegWrite(pObj,HCDPRegFFTReadAddress,0x20);
    }

    _MUTEX_RELEASE(HCDP)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_FFTGetPoint                                         */
/*                                                                            */
/* DESCRIPTION: Gets the next FFT point.                                      */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_FFTGetPoint(
    tmUnitSelect_t tUnit,
    tmFrontEndFracNb32_t *pY
)
{
    UInt16 uValue = 0;

    pHCDPObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);
    
    _MUTEX_ACQUIRE(HCDP)

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslHCDP_FFTGetPoint(0x%08X)", tUnit);

    /* initialize to handle errors */
    pY->lInteger = 1;
    pY->uDivider = 255;

    /*usage of drawing FFT graph:
    The FFT curve consists of 64 points(x[i],y[i], i=0...63). 
    x[i] = i;
    y[i] = (float)pY[i].lInteger / (float) pY[i].uDivider;
    the value of y[i] will be between 0.0 ~ 1.0  

    call tmbslHCDP_FFTPreset() once,
    then call tmbslHCDP_FFTGetPoints() 64 times to get 64 points
    */
    
    err = iHCDP_RegRead(pObj,HCDPRegFFTDataRead,&uValue);
    
    if (err == TM_OK)
    {
        if (uValue != 0) /* checks log compatibility */
        {
            pY->lInteger = uValue;
            pY->uDivider = 255;
        }
        else /* 10*log(1/255) = -24dB */
        {
            pY->lInteger = 1;
            pY->uDivider = 255;
        }
    }

    _MUTEX_RELEASE(HCDP)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_EqualizerPreset                                     */
/*                                                                            */
/* DESCRIPTION: Initialises the equalizer getting.                            */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_EqualizerPreset(
    tmUnitSelect_t tUnit
)
{
    pHCDPObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);
    
    _MUTEX_ACQUIRE(HCDP)

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslHCDP_EqualizerPreset(0x%08X)", tUnit);

    if (err == TM_OK)
    {
        err = iHCDP_EqualizerGetPoints(pObj);
    }

    _MUTEX_RELEASE(HCDP)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslHCDP_EqualizerGetPoint                                   */
/*                                                                            */
/* DESCRIPTION: Gets the specified equalizer tap.                             */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_EqualizerGetPoint(
    tmUnitSelect_t tUnit,
    UInt8 uTapIndex,
    tmFrontEndFracNb32_t *pI,
    tmFrontEndFracNb32_t *pQ
)
{
    pHCDPObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(HCDP)

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslHCDP_EqualizerGetPoint(0x%08X)", tUnit);

    /* initialize to handle errors */
    pI->lInteger = 0;
    pI->uDivider = 1;
    pQ->lInteger = 0;
    pQ->uDivider = 1;

    if (uTapIndex > 63)
    {
        err = HCDP_ERR_BAD_PARAMETER;
    }

    if (err == TM_OK)
    {
        pI->lInteger = pObj->sI[uTapIndex].lInteger;
        pI->uDivider = pObj->sI[uTapIndex].uDivider;
        pQ->lInteger = pObj->sQ[uTapIndex].lInteger;
        pQ->uDivider = pObj->sQ[uTapIndex].uDivider;
    }

    _MUTEX_RELEASE(HCDP)

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmbslHCDP_GetSignalLevel                                      */
/*                                                                            */
/* DESCRIPTION: Gets the AAGC signal level.                                    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslHCDP_GetSignalLevel(
    tmUnitSelect_t tUnit,
    UInt16 *puLevel
)
{
    pHCDPObject_t pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* check input parameters */
    err = iHCDP_GetInstance(tUnit, &pObj);
    
    _MUTEX_ACQUIRE(HCDP)

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "tmbslHCDP_GetSignalLevel(0x%08X)", tUnit);

    if (err == TM_OK)
    {
        err = iHCDP_RegRead(pObj,HCDPRegAagcSigLevel,puLevel);
    }

    _MUTEX_RELEASE(HCDP)

    return err;
}

/*============================================================================*/
/* Internal functions:                                                        */
/*============================================================================*/

/*============================================================================*/
/* FUNCTION:    iHCDP_GetConPhyData                                           */
/*                                                                            */
/* DESCRIPTION: Processes raw data(2's complement) and output an integer.     */
/*                                                                            */
/* RETURN:      Corresponding integer                                         */
/*                                                                            */
/*============================================================================*/
static Int32
iHCDP_GetConPhyData
(
    pHCDPObject_t pObj,
    UInt16 uRawData
)
{
    Int32 iSample = (UInt16)uRawData;

    if ((iSample >> 14 ) == 1)
    {
        /*it is a negative value in 2's complement*/
        iSample = (UInt16)((~iSample) + (UInt16) 0x01);
        iSample &=pObj->usConDMask;
        iSample *= -1;
    }
    else
    {
        iSample &= pObj->usConDMask;
    }
    return iSample;
}

/*============================================================================*/
/* FUNCTION:    iHCDP_EqualizerGetPoints                                      */
/*                                                                            */
/* DESCRIPTION: Gets all the equalizer points.                                */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
static tmErrorCode_t
iHCDP_EqualizerGetPoints
(
    pHCDPObject_t pObj
)
{
    UInt16 uCenterTapGain = 0;
    UInt16 uRegVal = 0;
    UInt8 uNbLoops;
    int i = 0;
    tmErrorCode_t err = TM_OK;

    tmDBGPRINTEx(DEBUGLVL_VERBOSE, "iHCDP_EqualizerGetPoints(0x%08X)", pObj->tUnit);

#define MAXNBLOOPS 10

    for (i= 0; i < HCDP_TAPS_NUMBER; i++)
    {
        pObj->sI[i].lInteger = 0;
        pObj->sI[i].uDivider = 1;
        pObj->sQ[i].lInteger = 0;
        pObj->sQ[i].uDivider = 1;
    }
    
    err = iHCDP_RegRead(pObj,HCDPRegEqCenterTapGain,&uCenterTapGain);

    if (err == TM_OK)
    {
        /*usage of drawing FFT graph:
        The Equalizer I curve consists of 64 points(x[i],y[i], i=0...63). 
        x[i] = i;
        y[i] = (float)pI[i].lInteger / (float) pI[i].uDivider;
        the value of y[i] will be between -1.0~ 1.0  

        The Equalizer Q curve consists of 64 points(x[i],y[i], i=0...63). 
        x[i] = i;
        y[i] = (float)pQ[i].lInteger / (float) pQ[i].uDivider;
        the value of y[i] will be between -1.0~ 1.0  */
        err = iHCDP_RegWrite(pObj,HCDPRegEqCoefNumber,0);
    }

    if (err == TM_OK)
    {
        /*select I component*/
        err = iHCDP_RegWrite(pObj,HCDPRegEqCoefIsReal,1);
    }

    if (err == TM_OK)
    {
        /*read 10 MSB*/
        err = iHCDP_RegWrite(pObj,HCDPRegEqCoefIsHigh,1);
    }

    for (i=0; i < HCDP_TAPS_NUMBER; i++)
    {
        if (err == TM_OK)
        {
            /*make sure read valid is achieved*/
            err = iHCDP_RegRead(pObj,HCDPRegEqCoefRdValid,&uRegVal);
        }

        if (err == TM_OK)
        {
            uNbLoops = 0;
            while ((uRegVal != 1) && (err == TM_OK) && (uNbLoops <= MAXNBLOOPS))
            {
                err = iHCDP_RegRead(pObj,HCDPRegEqCoefRdValid,&uRegVal);
                uNbLoops++;
            }
        }

        if (err == TM_OK)
        {
            /*read I*/
            err = iHCDP_RegRead(pObj,HCDPRegEqCoefData,&uRegVal);
        }

        if (err == TM_OK)
        {
            pObj->sI[i].lInteger = uRegVal & 0x3FF;
            if (pObj->sI[i].lInteger > 511) /*negative*/
            {
                pObj->sI[i].lInteger -= 1024;
            }

            if (i == 14)
            {
                if (uCenterTapGain == 1)
                {
                    if (pObj->sI[i].lInteger != 0)
                    {
                        err = HCDP_ERR_HW_FAILED;
                    }
                    pObj->sI[i].lInteger = 1024;
                }
                else
                {
                    pObj->sI[i].lInteger = 512;
                }
            }
            pObj->sI[i].uDivider = 512;


            /*make sure read valid is achieved*/
            err = iHCDP_RegRead(pObj,HCDPRegEqCoefRdValid,&uRegVal);
        }

        if (err == TM_OK)
        {
            uNbLoops = 0;
            while ((uRegVal != 1) && (err == TM_OK) && (uNbLoops <= MAXNBLOOPS))
            {
                err = iHCDP_RegRead(pObj,HCDPRegEqCoefRdValid,&uRegVal);
                uNbLoops++;
            }
        }

        if (err == TM_OK)
        {
            /*read Q*/
            err = iHCDP_RegRead(pObj,HCDPRegEqCoefData,&uRegVal);
        }
        if (err == TM_OK)
        {
            pObj->sQ[i].lInteger = uRegVal & 0x3FF;
            if (pObj->sQ[i].lInteger> 511) /*negative*/
            {
                pObj->sQ[i].lInteger -= 1024;
            }

            if (i == 14)
            {
                pObj->sQ[i].lInteger = 0;
            }
            pObj->sQ[i].uDivider = 512;
        }
    }
    
    return err;
}
