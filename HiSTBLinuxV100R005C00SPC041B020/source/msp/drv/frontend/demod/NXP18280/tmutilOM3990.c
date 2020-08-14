/**
Copyright (C) 2008 NXP B.V., All Rights Reserved.
This source code and any compilation or derivative thereof is the proprietary
information of NXP B.V. and is confidential in nature. Under no circumstances
is this software to be  exposed to or placed under an Open Source License of
any type without the expressed written permission of NXP B.V.
*
* \file          tmutilOM3990.c
*                %version: CFR_FEAP#25 %
*
* \date          %date_modified%
*
* \brief         Describe briefly the purpose of this file.
*
* REFERENCE DOCUMENTS :
*
* Detailed description may be added here.
*
* \section info Change Information
*
* \verbatim
Date          Modified by CRPRNr  TASKNr  Maintenance description
-------------|-----------|-------|-------|-----------------------------------
             |           |       |       |
-------------|-----------|-------|-------|-----------------------------------
\endverbatim
*
*/

/*============================================================================*/
/*                   STANDARD INCLUDE FILES                                   */
/*============================================================================*/

/*============================================================================*/
/*                   PROJECT INCLUDE FILES                                    */
/*============================================================================*/
#include "tmNxTypes.h"
#include "tmCompId.h"
#include "tmFrontEnd.h"
#include "tmbslFrontEndTypes.h"
#include "tmUnitParams.h"

#include "tmbslTDA18204.h"
#include "tmbslTDA18280.h"
#include "tmbslTDA18280_RegDef.h"
#include "tmbslTDA18280_Local.h"
#include "tmutilOM3990.h"

/*============================================================================*/
/*                   MACRO DEFINITION                                         */
/*============================================================================*/
#define OM3990_OFFSET_INIT \
{ \
    {0, 0}, \
    {0, 0}, \
    {0, 0}, \
    {0, 0}, \
    {0, 0}, \
    {0, 0}, \
    {0, 0}, \
    {0, 0}, \
    {0, 0}, \
    {0, 0}, \
    {0, 0}, \
    {0, 0}, \
    {0, 0}, \
    {0, 0}, \
    {0, 0}, \
    {0, 0}, \
    {0, 0}, \
    {0, 0}, \
    {0, 0} \
}

#define NVRAM_HEADER_SIZE 2
#define uOffsetPowerLevel_IndexOffset_WB 1
#define uOffsetPowerLevel_IndexOffset_NB 2
#define ulSizeOfElement (sizeof(OM3990_SystemCalibrationNVRAMSample_t)/sizeof(Int32))

/*============================================================================*/
/*                   TYPE DEFINITION                                          */
/*============================================================================*/
/** 
* Offset sample.
*/
typedef struct _OM3990_SystemCalibrationSample_t
{
    UInt32  uRF;                  /**< RF frequency in hertz.  */
    Int32   uOffsetPowerLevel;    /**< Power level Offset (x100) at the RF, in dB */
}
OM3990_SystemCalibrationSample_t, *pOM3990_SystemCalibrationSample_t;

/*============================================================================*/
/*                   PUBLIC VARIABLES DEFINITION                              */
/*============================================================================*/

/*============================================================================*/
/*                   STATIC VARIABLES DECLARATIONS                            */
/*============================================================================*/
/* Contains ADC input transfer function from characterization data. here, */
/* a simple 1st order low-pass filter model is used */
static OM3990_TransfertFunctionADCTable gTransfertFunctionADCTable[] = OM3990_TF_ADC_TABLE;

/* Contains TopDigAGC table approximation */
static OM3990_TopDigAGCTable gTopDigAGCTable[] = OM3990_TOP_DIG_AGC_TABLE;

#define NVRAM_SIZE 42

static UInt32 gNVRAM[2+NVRAM_SIZE*2] = OM3990_DEFAULT_NVRAM;

static OM3990_SystemCalibrationSample_t gOffsetCalibrationWB[NVRAM_SIZE] = OM3990_OFFSET_INIT;
static OM3990_SystemCalibrationSample_t gOffsetCalibrationNB[NVRAM_SIZE] = OM3990_OFFSET_INIT;

/*============================================================================*/
/*                       EXTERN FUNCTION PROTOTYPES                           */
/*============================================================================*/

/*============================================================================*/
/*                   STATIC FUNCTIONS DECLARATIONS                            */
/*============================================================================*/
static tmErrorCode_t 
itmutilOM3990_GetRSSIContribution(
    tmUnitSelect_t  tUnitTDA18204,    /**< I: Unit number. */
    UInt32          uRF,              /**< I: RF in Hz. */
    Int32           *Contribution     /**< O: Pointer to the RSSI contribution in dB*100. */
    );

static Int32 
itmutilOM3990_GetRssiOffsetPowerLevel(UInt32 uRF, const OM3990_SystemCalibrationSample_t *gOffsetCalibration);

static void
itmutilOM3990_UpdateOffsetArrayWithNVRAM(
    OM3990_InitSystemCalibrationIoFunc_t* pInitSystemCalibrationFunc);

/*============================================================================*/
/*                   PUBLIC FUNCTIONS DEFINITIONS                             */
/*============================================================================*/

/*============================================================================*/
/*                   PROJECT INCLUDE FILES                                    */
/*============================================================================*/

/*============================================================================*/
/*                   TYPE DEFINITION                                          */
/*============================================================================*/

/*============================================================================*/
/*                   STATIC VARIABLES DECLARATIONS                            */
/*============================================================================*/

/*============================================================================*/
/*                   PUBLIC FUNCTIONS DECLARATIONS                            */
/*============================================================================*/

/*============================================================================*/
/*                   STATIC FUNCTIONS DECLARATIONS                            */
/*============================================================================*/

/*============================================================================*/
/*                   PUBLIC FUNCTIONS DEFINITIONS                             */
/*============================================================================*/

/*=================================================================================================*/
/**
\brief  Perform the Initialization of System calibration: Retrieve offset from NVRAM

\return TM_OK: Succeed.
\return OM3990_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
\return OM3990_ERR_BAD_PARAMETER: Parameters out of range.
\return Other optional return codes of the functions registered 
in DeviceOpenInstance API (I2C read errors, mutex...).

\note  To be performed once HwInit functions has been called.

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
    tmutilOM3990_InitSystemCalibration(
    tmUnitSelect_t              tUnitTDA18204,      /**< I: TDA18204 Unit number. */
    tmUnitSelect_t              tUnitTDA18280,      /**< I: TDA18280 Unit number. */
    OM3990_InitSystemCalibrationIoFunc_t* pInitSystemCalibrationFunc /**< I: Pointer to callback functions. */
    )
{
    tmErrorCode_t        err = TM_OK;
    tmTDA18280_TiltInputParameters_t       tInputParam;
    tmTDA18280_TiltOutputParameters_t      tOutputParam;
    tmTDA18280_PowerState_t eTDA18280PowerStatePrevious;
    tmTDA18280_PowerState_t eTDA18280PowerState = tmTDA18280_PowerWBOnly;
    TDA18204PowerState_t    eTDA18204PowerStatePrevious;
    TDA18204PowerState_t    eTDA18204PowerState = TDA18204_PowerSciNbNormalMode;

    /* handle power state */
    err = tmbslTDA18280_DeviceGetPowerState( tUnitTDA18280, &eTDA18280PowerStatePrevious);

    if( err == TM_OK )
    {
        err = tmbslTDA18280_DeviceSetPowerState( tUnitTDA18280, eTDA18280PowerState);
    }

    if( err == TM_OK )
    {
        err = tmbslTDA18204_GetLLPowerState( tUnitTDA18204, &eTDA18204PowerStatePrevious );
    }

    if( err == TM_OK )
    {
        err = tmbslTDA18204_SetLLPowerState( tUnitTDA18204, eTDA18204PowerState );
    }

    if(err == TM_OK)
    {
        err = tmbslTDA18204_SetTiltEqualizer( tUnitTDA18204, TDA18204_TEQSelect0dB);
    }

    if(err == TM_OK)
    {
        /* we don't specify Input parameter at system initialization */
        tInputParam.bUseInputParameter = False;

        err = tmbslTDA18280_DeviceGetTiltEqualizerEstimation(tUnitTDA18280, &tInputParam, &tOutputParam);
    }

    if(err == TM_OK)
    {
        err = tmbslTDA18204_SetTiltEqualizer( tUnitTDA18204, (TDA18204TEQSelect_t)tOutputParam.TDA18280_CalculatedTilt);
    }

    if( err == TM_OK)
    {
        /* update array with NVRAM values */
        itmutilOM3990_UpdateOffsetArrayWithNVRAM(pInitSystemCalibrationFunc);

        /* set to previous state */
        err = tmbslTDA18280_DeviceSetPowerState( tUnitTDA18280, eTDA18280PowerStatePrevious);
    }

    if( err == TM_OK )
    {
        err = tmbslTDA18204_SetLLPowerState( tUnitTDA18204, eTDA18204PowerStatePrevious );
    }

    return err;
}

/*=================================================================================================*/
/**
\brief  Perform the System calibration: compute and store offset 
for each of the reference points (Frequency, Reference_level) given, done only once in Factory phase.

\return TM_OK: Succeed.
\return OM3990_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
\return OM3990_ERR_BAD_PARAMETER: Parameters out of range.
\return Other optional return codes of the functions registered 
in DeviceOpenInstance API (I2C read errors, mutex...).

\note  To be performed once HwInit functions has been called. Only once in Factory phase.

\sa     NA
*/
/*=================================================================================================*/		
#if 0
tmErrorCode_t
    tmutilOM3990_SystemCalibration(
    tmUnitSelect_t              tUnitTDA18204,      /**< I: TDA18204 Unit number. */
    tmUnitSelect_t              tUnitTDA18280,      /**< I: TDA18280 Unit number. */
    OM3990_SystemCalibrationIoFunc_t* pSystemCalibrationFunc, /**< I: Pointer to callback functions. */
    OM3990_RefVector_t*               ptRefVector, /**< I: Pointer to the vector containing the reference points. (max size if NVRAM_SIZE = 42)*/
    OM3990_RefVectorPowerUnit_t       ePowerUnit,  /**< I: Power level unit of reference vector. */
    OM3990_BW_t                       eBW          /**< I: Bandwidth used for calibration */
    )
{
    tmErrorCode_t           err = TM_OK;
    UInt8                   i=0;
    tmTDA18280_PowerState_t eTDA18280PowerStatePrevious;
    tmTDA18280_PowerState_t eTDA18280PowerState = tmTDA18280_PowerWBOnly;
    TDA18204PowerState_t    eTDA18204PowerStatePrevious;
    TDA18204PowerState_t    eTDA18204PowerState = TDA18204_PowerSciNbNormalMode;

    tmTDA18280_PathCfg_t PathCfg ;
    tmTDA18280_Path_t    ePathIndex;
    Int32                uOffsetPowerLevel_IndexOffset = 1;
    Int32                ArrayIndex;

    Int32 RSSI = 0; /* RSSI in dBuV*100 */
    /* 2 is the size of the header */
    Int32 LocalNVRAM[2 + NVRAM_SIZE*ulSizeOfElement] = OM3990_DEFAULT_NVRAM;

    /* initialise in order to use GetRssi correctly */
    gOffsetCalibrationWB[0].uRF = 0;
    gOffsetCalibrationWB[0].uOffsetPowerLevel = 0;

    gOffsetCalibrationNB[0].uRF = 0;
    gOffsetCalibrationNB[0].uOffsetPowerLevel = 0;

    /* write header of NVRAM */
    LocalNVRAM[0] = 1; /* version */
    LocalNVRAM[1] = ptRefVector->uNbPoints; /* NbPoints */

    if(ptRefVector->uNbPoints > NVRAM_SIZE )
    {
        /* ATTENTION - array is too small!!! */
        err = TDA18280_ERR_BAD_PARAMETER;
        
        /* just to avoid crash */
        LocalNVRAM[1] = NVRAM_SIZE;
    }

    if( err == TM_OK )
    {
        /* handle power state */
        err = tmbslTDA18280_DeviceGetPowerState( tUnitTDA18280, &eTDA18280PowerStatePrevious);
    }

    if( err == TM_OK )
    {
        err = tmbslTDA18280_DeviceSetPowerState( tUnitTDA18280, eTDA18280PowerState);
    }

    if( err == TM_OK )
    {
        err = tmbslTDA18204_GetLLPowerState( tUnitTDA18204, &eTDA18204PowerStatePrevious );
    }

    if( err == TM_OK )
    {
        err = tmbslTDA18204_SetLLPowerState( tUnitTDA18204, eTDA18204PowerState );
    }

    // Calibration is done for Path 0 and Path Aux
    // start with Path 0, the while loop will switch to Path Aux
    ePathIndex = tmTDA18280_Path_0;
    
    do{
        if( ePathIndex == tmTDA18280_Path_0 )
        {
            /* WB index offset */
            uOffsetPowerLevel_IndexOffset = uOffsetPowerLevel_IndexOffset_WB;
        }
        else
        {
            /* NB index offset */
            uOffsetPowerLevel_IndexOffset = uOffsetPowerLevel_IndexOffset_NB;
        }

        /* we will calculate each offset to apply per vector */
        /* for each NB points of vector table,  get power level */
        for (i=0; i<ptRefVector->uNbPoints; i++)	
        {
            if(err == TM_OK)
            {
                err =  tmbslTDA18280_PathGet(tUnitTDA18280, ePathIndex, &PathCfg );
            }

            if( ePathIndex == tmTDA18280_Path_Aux )
            {
                if (err == TM_OK)
                {
                    if( eBW == OM3990_8MHz )
                    {
                        err = tmbslTDA18204_SetStandardMode(tUnitTDA18204, TDA18204_TUNER_QAM_8MHz);
                    }
                    else
                    {
                        err = tmbslTDA18204_SetStandardMode(tUnitTDA18204, TDA18204_TUNER_QAM_6MHz);
                    }
                }

                if (err == TM_OK)
                {
                    unsigned int uRF = (UInt32)ptRefVector->tRefPoints[i].uRF;
                    err = tmbslTDA18204_SetRF(tUnitTDA18204, uRF);
                }
            }
            else
            {
                if(err == TM_OK)
                {
                    PathCfg.tPathStatus = tmTDA18280_ON;
                    PathCfg.tGainAutoMode = tmTDA18280_ON;
                    PathCfg.uRF = ptRefVector->tRefPoints[i].uRF;
                    if( eBW == OM3990_8MHz )
                    {
                        PathCfg.tBW = tmTDA18280_8MHz;
                    }
                    else
                    {
                        PathCfg.tBW = tmTDA18280_6MHz;
                    }

                    err =  tmbslTDA18280_PathSet(tUnitTDA18280, ePathIndex, &PathCfg );
                }
            }

            if(err == TM_OK)
            {
                err = tmutilOM3990GetRSSI (tUnitTDA18204, tUnitTDA18280, ePathIndex, &RSSI);
            }

            if(err == TM_OK)
            {
                /* for debug */
                /*
                RSSI = -(ePathIndex + i*100);
                RSSI += ((ptRefVector->tRefPoints[i].sRefLevel + ePowerUnit));
                */

                /* RF */
                ArrayIndex = NVRAM_HEADER_SIZE + i*ulSizeOfElement;
                LocalNVRAM[ArrayIndex] = ptRefVector->tRefPoints[i].uRF;

                /* comparing the measured values to the values given in the parameter -> OffsetPowerLevel */
                ArrayIndex = NVRAM_HEADER_SIZE + i*ulSizeOfElement + uOffsetPowerLevel_IndexOffset;
                LocalNVRAM[ArrayIndex] = (Int32)(((ptRefVector->tRefPoints[i].sRefLevel + ePowerUnit)*100) - RSSI); /* in dBuV*100 */

                /*
                printf( "WB Cal [%d] freq = %d ref = %lf (ref = %lf) RSSI = %d offset = %d \n", 
                i, 
                LocalNVRAM[NVRAM_HEADER_SIZE + i*ulSizeOfElement],
                ptRefVector->tRefPoints[i].sRefLevel,
                ptRefVector->tRefPoints[i].sRefLevel + ePowerUnit,
                RSSI,
                LocalNVRAM[NVRAM_HEADER_SIZE + i*ulSizeOfElement + uOffsetPowerLevel_IndexOffset] 
                );
                */
            }
        }

        if( ePathIndex == tmTDA18280_Path_0)
        {
            ePathIndex = tmTDA18280_Path_Aux;
        }
        else
        {
            ePathIndex = tmTDA18280_Path_Max;
        }

    } while ( ePathIndex != tmTDA18280_Path_Max);

    if( err == TM_OK) 
    {
        /* fill the static array */
        for (i=0; i<ptRefVector->uNbPoints; i++)
        {
            gOffsetCalibrationWB[i].uRF = LocalNVRAM[NVRAM_HEADER_SIZE + i*ulSizeOfElement];
            gOffsetCalibrationWB[i].uOffsetPowerLevel = LocalNVRAM[NVRAM_HEADER_SIZE + i*ulSizeOfElement + uOffsetPowerLevel_IndexOffset_WB];

            gOffsetCalibrationNB[i].uRF = LocalNVRAM[NVRAM_HEADER_SIZE + i*ulSizeOfElement];
            gOffsetCalibrationNB[i].uOffsetPowerLevel = LocalNVRAM[NVRAM_HEADER_SIZE + i*ulSizeOfElement + uOffsetPowerLevel_IndexOffset_NB];
        }

        gOffsetCalibrationWB[i++].uRF = 0;
        gOffsetCalibrationNB[i++].uRF = 0;
    }

    if(err == TM_OK)
    {
        /* Store in NVRAM */
        pSystemCalibrationFunc->Write(NVRAM_HEADER_SIZE + (ptRefVector->uNbPoints*ulSizeOfElement), (UInt32*)LocalNVRAM);
    }

    /* set to previous state */
    if( err == TM_OK )
    {
        err = tmbslTDA18280_DeviceSetPowerState( tUnitTDA18280, eTDA18280PowerStatePrevious);
    }

    if( err == TM_OK )
    {
        err = tmbslTDA18204_SetLLPowerState( tUnitTDA18204, eTDA18204PowerStatePrevious );
    }

    return err;
}
#endif
/*=================================================================================================*/
/**
\brief  Perform the RSSI measurement: combine access to TDA18204/TDA18280.

\return TM_OK: Succeed.
\return OM3990_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
\return OM3990_ERR_BAD_PARAMETER: Parameters out of range.
\return Other optional return codes of the functions registered 
in DeviceOpenInstance API (I2C read errors, mutex...).

\note   To be performed once HwInit functions has been called.

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmutilOM3990GetRSSI(
    tmUnitSelect_t              tUnitTDA18204,      /**< I: TDA18204 Unit number. */
    tmUnitSelect_t              tUnitTDA18280,      /**< I: TDA18280 Unit number. */
    UInt8           path,  /**< I: Path to compute the RSSI on. */
    Int32             *RSSI  /**< O: Pointer to the RSSI in dBuV*100. */
)
{
    tmErrorCode_t        err = TM_OK;
    UInt32               ulIndex;
    Int32                powerLevel;
    tmTDA18280_PathCfg_t PathCfg ;
    UInt32               ulRF;
    Int32                OffsetPowerLevel;
    Int32                Contribution; /* in dB*100 */
    tmTDA18280_Feature_t productFeatures;

    /* DSP fixed gain: fixed gain DSP (indeed loss) */
    Int32  DSPFixedGain = -71; /* -0.71 dB x100 */

    /* TOP DigAGC */
    Int32                TopDigAGC = 0; /* dBuV x100 */
    UInt32               DigAgcTarget = 0;

    if (path == tmTDA18280_Path_Aux)
    {
        /* retrieve TDA18204 configuration */
        if (err == TM_OK)
        {
            err = tmbslTDA18204_GetRF( tUnitTDA18204, &ulRF );
        }

        if (err == TM_OK)
        {
            /* determine offset to apply, coming from box calibration */
            OffsetPowerLevel = itmutilOM3990_GetRssiOffsetPowerLevel(ulRF, (OM3990_SystemCalibrationSample_t*)&gOffsetCalibrationNB);
        }

        if (err == TM_OK)
        {
            err = tmbslTDA18204_GetPowerLevel(tUnitTDA18204, &powerLevel);
            *RSSI = powerLevel + OffsetPowerLevel; /* in dBuV*100 */
        }
    }
    else /* (path != tmTDA18280_Path_Aux) */
    {
        err = tmbslTDA18280_GetFeatures(tUnitTDA18280, &productFeatures);

        /* determine DigAgcTarget based on the path to demod or to DAC */
        if (err == TM_OK)
        {
            if (productFeatures.bDACSupported && path >= tmTDA18280_Path_4 && path <= tmTDA18280_Path_7)
            {
                DigAgcTarget = TDA18280_DIG_AGC_TARGET_RMS_FOR_DAC;
            }
            else
            {
                DigAgcTarget = TDA18280_DIG_AGC_TARGET_RMS;
            }
        }

        /* read the parameters from the TDA18280 */
        if (err == TM_OK)
        {
            err =  tmbslTDA18280_PathGet(tUnitTDA18280, path, &PathCfg);
        }

        if (PathCfg.tPathStatus != tmTDA18280_ON)
        {
            /* error: the path is not programmed */
            err = OM3990_ERR_NOT_INITIALIZED;
        }

        if (err == TM_OK)
        {
            err = itmutilOM3990_GetRSSIContribution(tUnitTDA18204, PathCfg.uRF, &Contribution); /* Contribution in dB*100 */
        }

        if (err == TM_OK)
        {
            // find out the log value in the table for top_DigAGC
            ulIndex = 0;
            while ( (DigAgcTarget <= gTopDigAGCTable[ulIndex].uX) && (gTopDigAGCTable[ulIndex].uX != 0) )
            {
                ulIndex ++;
            }
            if (ulIndex == 0)
            {
                TopDigAGC = gTopDigAGCTable[ulIndex].iY;
            }
            else
            {
                TopDigAGC = gTopDigAGCTable[ulIndex-1].iY;
            }

            *RSSI = TopDigAGC + Contribution - DSPFixedGain - (PathCfg.uGainMultBy10 * 10); /* in dBuV*100 */
        }
    }
    return err;
}


/*=================================================================================================*/
/**
\brief  Retrieve the correction array applied on the RSSI measurement (for debug purpose).

\return TM_OK: Succeed.
\return OM3990_ERR_BAD_PARAMETER: the array size does not match the size of data stored.
\return Other optional return codes of the functions registered 
in DeviceOpenInstance API (I2C read errors, mutex...).

\note This function read the current correction array used, then read the correction array stored in NVRAM 
and apply it as the the current correction array. This allow to apply corection stored in NVRAM if it is not
already done.

\sa     NA
*/
/*=================================================================================================*/
#if 0
tmErrorCode_t
tmutilOM3990_GetRssiCorrectionArray(
    OM3990_SystemCalibrationNVRAMSample_t **pOffsetCalibration,      /**< O: Pointer to correction array. */
    OM3990_SystemCalibrationNVRAMSample_t **pOffsetCalibrationNVRAM, /**< O: Pointer to correction array stored in NVRAM. */
    OM3990_InitSystemCalibrationIoFunc_t* pInitSystemCalibrationFunc /**< I: Pointer to callback functions. */
)
{
    tmErrorCode_t err = TM_OK;
    UInt32 ulIndex = 0;

    static OM3990_SystemCalibrationNVRAMSample_t OffsetCalibration[NVRAM_SIZE] = OM3990_OFFSET_CALIBRATION;
    static OM3990_SystemCalibrationNVRAMSample_t OffsetCalibrationNVRAM[NVRAM_SIZE] = OM3990_OFFSET_CALIBRATION;

    /* set output parameters */
    *pOffsetCalibration = OffsetCalibration;
    *pOffsetCalibrationNVRAM = OffsetCalibrationNVRAM;

    /* first store current array */
    ulIndex = 0;

    while( gOffsetCalibrationNB[ulIndex].uRF != 0 )
    {
        if(gOffsetCalibrationNB[ulIndex].uRF != gOffsetCalibrationWB[ulIndex].uRF)
        {
            /* there is a problem here */
            /* WB and NB ref vector should be the same */
        }
        else
        {
            OffsetCalibration[ulIndex].uRF = gOffsetCalibrationNB[ulIndex].uRF;
            OffsetCalibration[ulIndex].uOffsetPowerLevel = gOffsetCalibrationWB[ulIndex].uOffsetPowerLevel;
            OffsetCalibration[ulIndex].uOffsetPowerLevelNB = gOffsetCalibrationNB[ulIndex].uOffsetPowerLevel;
        }
            
        ulIndex ++;
    }  

    OffsetCalibration[ulIndex].uRF = 0;

    /* update array with NVRAM values */
    itmutilOM3990_UpdateOffsetArrayWithNVRAM(pInitSystemCalibrationFunc);
  
    /* store array retrieved from NVRAM */
    ulIndex = 0;

    while( gOffsetCalibrationNB[ulIndex].uRF != 0 )
    {
        if(gOffsetCalibrationNB[ulIndex].uRF != gOffsetCalibrationWB[ulIndex].uRF)
        {
            /* there is a problem here */
            /* WB and NB ref vector should be the same */
        }
        else
        {
            OffsetCalibrationNVRAM[ulIndex].uRF = gOffsetCalibrationNB[ulIndex].uRF;
            OffsetCalibrationNVRAM[ulIndex].uOffsetPowerLevel = gOffsetCalibrationWB[ulIndex].uOffsetPowerLevel;
            OffsetCalibrationNVRAM[ulIndex].uOffsetPowerLevelNB = gOffsetCalibrationNB[ulIndex].uOffsetPowerLevel;
        }
        ulIndex ++;
    }  

    OffsetCalibrationNVRAM[ulIndex].uRF = 0;

    return err;
}
#endif
/*=================================================================================================*/
/**
\brief  Perform the FFT.

\return TM_OK: Succeed.
\return OM3990_ERR_BAD_PARAMETER: the array size does not match the size of data stored.
\return Other optional return codes of the functions registered 
in DeviceOpenInstance API (I2C read errors, mutex...).

\note   NA

\sa     NA
*/
/*=================================================================================================*/
#if 0
tmErrorCode_t
tmutilOM3990_PerformFFT(
    tmUnitSelect_t              tUnitTDA18204,      /**< I: TDA18204 Unit number. */
    tmUnitSelect_t              tUnitTDA18280,      /**< I: TDA18280 Unit number. */
    UInt16 nbSamples,                   /**< I: number of samples (max = 0x4000). */
    UInt16 nbCaptures,                  /**< I: number of FFT used for average calculation. */
    double *fft,                        /**< O: pointer on the array containing the FFT (contains nbSamples/2 elements). */
    ptmTDA18280_FFTResults_t fftResult, /**< O: FFT feature. */
    double *fftCorrected /* in dBuV */
)
{
    tmErrorCode_t err = TM_OK;
    ptmTDA18280_Object_t       pObj = Null;
    UInt16 ulIndex;
    UInt32 uRF;
    Int32 Contribution; /* in dB*100 */

    UInt32  ulCaptureIndex;
    Int16   ReadData[RAM_SIZE] = {0};
    double  x[RAM_SIZE] = {0};
    double  y[RAM_SIZE] = {0};

    double  dblFreqVco = 0;
    UInt16  usFreqVco = 0;
    UInt16  usLowVcoSel = 0;

    double FactB = 0.1;
    double FactA = 1-FactB;
    double dblMagSq;
    double dblEnergy;
    tmTDA18280_DebugRamState_t eDbgRamState = tmTDA18280_DebugRamDisabled;
    Bool  bRestoreDebugRamState = False;

    FactB = 1/(double)nbCaptures;
    FactA = 1-FactB;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnitTDA18280, &pObj);

    /* check parameters */
    if( nbSamples > RAM_SIZE )
    {
        err = OM3990_ERR_BAD_PARAMETER;
    }

    /* acquire the debug RAM state to restore it at the end */
    if(err == TM_OK)
    {
        err = iTDA18280_GetStateDebugRam(pObj, &eDbgRamState);
    }

    /* enable the debug RAM if needed */
    if (err == TM_OK)
    {
        if (eDbgRamState == tmTDA18280_DebugRamDisabled)
        {
            bRestoreDebugRamState = True;
            err = iTDA18280_SetStateDebugRam(pObj, tmTDA18280_DebugRamEnabled);
        }
    }

    for( ulCaptureIndex = 0; ulCaptureIndex < nbCaptures; ulCaptureIndex++ )
    {
        if(err == TM_OK)
        {
            err = iTDA18280_SetConfigAndAcquireDebugRam(pObj, tmTDA18280_DebugRamCtrlMode_Store_Adc, tmTDA18280_DebugRamCtrlOneShot_OneShot, nbSamples);
        }

        /* wait for the capture in the debug ram to be finished */
        if(err == TM_OK)
        {
            UInt16 CaptureDone = 0;
            int nbLoop = 0;

            do{
                err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_dbg_ram_STATUS__done, &CaptureDone); 
                nbLoop ++;
            } while ((CaptureDone == 0) && (nbLoop < STATUS_TIMEOUT));

            if( nbLoop >= STATUS_TIMEOUT )
            {
                err = OM3990_ERR_HW;
            }
        }

        /* retrieve the debug ram content */
        if(err == TM_OK)
        {
            err = iTDA18280_BlockDebugRamRead(pObj, (UInt16)nbSamples, ReadData);
        }

        /* compute the FFT */
        if( err == TM_OK )
        {
            for(ulIndex = 0; ulIndex < nbSamples; ulIndex ++)
            {
                x[ulIndex] = (double)ReadData[ulIndex];
            }

            itmutilOM3990_CalculateFFT(1, nbSamples, x, y);
        }

        /* perform the average*/
        if( err == TM_OK )
        {
            for (ulIndex = 0; ulIndex < nbSamples/2; ulIndex++)
            {
                dblMagSq = 0; // mag squared

                //fft[ulIndex] = sqrt((x[ulIndex]*x[ulIndex])+(y[ulIndex]*y[ulIndex]));
                dblMagSq = (x[ulIndex]*x[ulIndex])+(y[ulIndex]*y[ulIndex]);

                if( ulCaptureIndex  == 0 )
                {
                    //fft[ulIndex] = sqrt(dblMagSq);
                    fft[ulIndex] = dblMagSq;
                }
                else
                {
                    fft[ulIndex] = FactB*dblMagSq + FactA*fft[ulIndex-1];
                }
            }
        }
    }

    /* fft array contains the average over all captures */
    if( err == TM_OK )
    {
        dblEnergy = 0;

        /* calculate energy of the FFT */
        for (ulIndex=0; ulIndex < nbSamples/2; ulIndex++)
        {
            dblEnergy += fft[ulIndex];
        }
        dblEnergy *= 2;
        
        for (ulIndex=0; ulIndex < nbSamples/2; ulIndex++)
        {
            /* normalise energy to 1 */
            fft[ulIndex] /= dblEnergy;

            /* signal in dB relative to total power */
            if( fft[ulIndex] != 0 )
            {
                fft[ulIndex] = 10 * log10(fft[ulIndex]);
            }
            else
            {
                /* to be tuned but avoid crash */
                fft[ulIndex] = 0;
            }

            if (fft[ulIndex] < fftResult->minfft)
            {
                fftResult->minfft = fft[ulIndex];
            }

            if (fft[ulIndex] > fftResult->maxfft)
            {
                fftResult->maxfft = fft[ulIndex];
            }
        }
    }

    /* calculate freqADC */
    if( err == TM_OK )
    {
        err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_crftech_pll_READVCOFREQ__freqvcoi, &usFreqVco); 
    }

    /* convert register value to double */
    if( err == TM_OK )
    {
        dblFreqVco = usFreqVco * 0.421875;
    }

    if( err == TM_OK )
    {
        err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_crftech_pll_SETUPFREQ__selvcooh, &usLowVcoSel); 
    }

    if( err == TM_OK )
    {
        if( usLowVcoSel == 1 )
        {
            fftResult->freqAdc = (UInt32)(dblFreqVco / 2);
        }
        else
        {
            fftResult->freqAdc = (UInt32)(dblFreqVco / 3);
        }
    }

    /* calculate the delta frequency between 2 samples */
    fftResult->SamplingDelta = (fftResult->freqAdc * 1000000) / nbSamples;

    /* restore the debug RAM state acquired at the beginning */
    if(err == TM_OK)
    {
        if (bRestoreDebugRamState)
        {
            err = iTDA18280_SetStateDebugRam(pObj, tmTDA18280_DebugRamDisabled);
        }
    }

    if( err == TM_OK && fftCorrected != Null )
    {
        for(ulIndex = 0; ulIndex < nbSamples/2; ulIndex ++ )
        {
            uRF = ulIndex * fftResult->SamplingDelta;

            Contribution = 0;

            err = itmutilOM3990_GetRSSIContribution(tUnitTDA18204, uRF, &Contribution); /* Contribution in dB*100 */
            if(err == TM_OK)
            {
                // fft[ulIndex] = 1; // for debug purpose
                fftCorrected[ulIndex] = fft[ulIndex] + Contribution/100; /* in dBuV */
            }
        }
    }

    return err;
}
#endif

/*============================================================================*/
/*                   STATIC FUNCTIONS                                         */
/*============================================================================*/

static tmErrorCode_t
itmutilOM3990_GetRSSIContribution(
    tmUnitSelect_t  tUnitTDA18204,           /**< I: Unit number. */
    UInt32          uRF,                     /**< I: RF in Hz. */
    Int32           *Contribution             /**< O: Pointer to the RSSI contribution in dB*100. */
    )
{
    tmErrorCode_t   err = TM_OK;
    UInt32          ulIndex;
    UInt32 ulRSSICorrection = TDA18280_RSSI_CORRECTION;

    Int32  TF_ADC = 0; /* dB x100 */
    Int32  GainLNA;
    Int32  OffsetPowerLevel = 0;

    /* apply a fixed shift (difference btw TDA18265 ES1 & ES2) */
    //ulRSSICorrection = TDA18280_RSSI_CORRECTION - 600;

    /* find out the log value in the table */
    ulIndex = 0;
    while ( (gTransfertFunctionADCTable[ulIndex].uX <= (uRF/1000000)) && (gTransfertFunctionADCTable[ulIndex].uX != 0) )
    {
        ulIndex ++;
    }
    if (ulIndex == 0)
    {
        TF_ADC = gTransfertFunctionADCTable[ulIndex].iY;
    }
    else
    {
        TF_ADC = gTransfertFunctionADCTable[ulIndex-1].iY;
    }

    /* Global Gain of TDA18280 */
    *Contribution = -TF_ADC + ulRSSICorrection;

    if(err == TM_OK)
    {
        err = tmbslTDA18204_CalculateGain(tUnitTDA18204, uRF, &GainLNA); /* GainLNA in dB*1000 */
    }

    if (err == TM_OK)
    {
        GainLNA = GainLNA/10; /* GainLNA in dB*100 */

        /* determine offset to apply, coming from factory calibration */
        OffsetPowerLevel = itmutilOM3990_GetRssiOffsetPowerLevel( uRF, (OM3990_SystemCalibrationSample_t*)&gOffsetCalibrationWB );

        *Contribution = *Contribution - GainLNA + OffsetPowerLevel; /* in dB*100 */
    }

    return err;
}

static Int32 itmutilOM3990_GetRssiOffsetPowerLevel(UInt32 uRF, const OM3990_SystemCalibrationSample_t *gOffsetCalibration)
{
    Int32 OffsetPowerLevel = 0;
    UInt32 ulIndex = 0;

#ifdef PRINTF_DEBUG
    printf( " GetRssi [%d] %d\n", ulIndex, gOffsetCalibration[ulIndex].uRF );
#endif

    // find out the offset in the table

    /* check that offset data are valid */
    if( gOffsetCalibration[ulIndex].uRF != 0 )
    {
        /* find out the offset in the table */
        while ( (gOffsetCalibration[ulIndex].uRF < uRF) && (gOffsetCalibration[ulIndex].uRF != 0) )
        {
#ifdef PRINTF_DEBUG
            printf( " GetRssi [%d] %d\n", ulIndex, gOffsetCalibration[ulIndex].uRF );
#endif
            ulIndex ++;
        }

        if( ulIndex == 0 )
        {
            /* the frequency is out of the boundaries of the offset array */
            OffsetPowerLevel = gOffsetCalibration[ulIndex].uOffsetPowerLevel;
        }
        else
        {
            if(gOffsetCalibration[ulIndex].uRF == 0 )
            {
                /* the frequency is out of the boundaries of the offset array */
                OffsetPowerLevel = gOffsetCalibration[ulIndex - 1].uOffsetPowerLevel;
            }
            else
            {
                /* regression for the offset value*/
                Int32 deltaRF = uRF - gOffsetCalibration[ulIndex-1].uRF;
                Int32 DeltaOffset = gOffsetCalibration[ulIndex].uOffsetPowerLevel - gOffsetCalibration[ulIndex - 1].uOffsetPowerLevel;
                Int32 DeltaRF = gOffsetCalibration[ulIndex].uRF - gOffsetCalibration[ulIndex-1].uRF;
                Int32 deltaOffset = 0;

                deltaRF /= 1000;
                DeltaRF /= 1000;

                deltaOffset = DeltaOffset * deltaRF;
                deltaOffset /= DeltaRF;

                OffsetPowerLevel = gOffsetCalibration[ulIndex - 1].uOffsetPowerLevel + deltaOffset;
            }
        }
    }

    return OffsetPowerLevel;
}

void itmutilOM3990_UpdateOffsetArrayWithNVRAM(OM3990_InitSystemCalibrationIoFunc_t* pInitSystemCalibrationFunc)
{
    tmErrorCode_t err;
    UInt32 i = 0;
    UInt32 OffsetArraySize = 0;

    /* read NVRAM, store in global array and store again the array */

    /* retrieve nb of offset samples stored inside */
    err = pInitSystemCalibrationFunc->Read(2, gNVRAM);

    if(err == TM_OK)
    {
        OffsetArraySize = gNVRAM[1];

        /* check the size of the NVRAM */
        if( OffsetArraySize > NVRAM_SIZE )
        {
            /* ATTENTION - This is will skip data */
            OffsetArraySize = NVRAM_SIZE;

            err = OM3990_ERR_BAD_PARAMETER;
        }

        /* read offset samples */
        if( err == TM_OK )
        {
            err = pInitSystemCalibrationFunc->Read(NVRAM_HEADER_SIZE + (OffsetArraySize*ulSizeOfElement), gNVRAM);
        }
    }

    if(err == TM_OK)
    {
#ifdef PRINTF_DEBUG
        printf( " tmutilOM3990_InitSystemCalibration -> check fill \n" );
#endif
        /* reset the static array */
        gOffsetCalibrationNB[0].uRF = 0;
        gOffsetCalibrationWB[0].uRF = 0;

        /* fill offset array with nvram values */
        for (i=0; i < OffsetArraySize; i++)
        {
            gOffsetCalibrationWB[i].uRF                 = gNVRAM[NVRAM_HEADER_SIZE + i*ulSizeOfElement];
            gOffsetCalibrationWB[i].uOffsetPowerLevel   = gNVRAM[NVRAM_HEADER_SIZE + i*ulSizeOfElement + uOffsetPowerLevel_IndexOffset_WB];
            
            gOffsetCalibrationNB[i].uRF                 = gNVRAM[NVRAM_HEADER_SIZE + i*ulSizeOfElement];
            gOffsetCalibrationNB[i].uOffsetPowerLevel   = gNVRAM[NVRAM_HEADER_SIZE + i*ulSizeOfElement + uOffsetPowerLevel_IndexOffset_NB];
#ifdef PRINTF_DEBUG
            printf( " Init fill [%d] %d %d %d \n", i, gOffsetCalibrationWB[i].uRF, gOffsetCalibrationWB[i].uOffsetPowerLevel, gOffsetCalibrationNB[i].uOffsetPowerLevel);
#endif
        }

        /* reset the static array */
        gOffsetCalibrationNB[i++].uRF = 0;
        gOffsetCalibrationWB[i++].uRF = 0;
    }

    return;
}

#if 0
/* extract values from math.h */
#ifndef M_2_PI
#define M_2_PI     0.636619772367581343076
#endif

void 
itmutilOM3990_CalculateFFT(
        short int dir, long SampleNb, double *x, double *y
        )
{
       long n,i,i1,j,k,i2,l,l1,l2;
       double c1,c2,tx,ty,t1,t2,u1,u2,z;

       long nbP = (long)(log((double)SampleNb)/log((double)2));

       /* create Hann window */
       /* double  Hann[RAM_SIZE] = {1}; */

       long ulIndex;

       double dblN = 0;
       double dblHann = 0;

       /* apply the Hann window */
       for(ulIndex = 0; ulIndex < SampleNb; ulIndex ++)
       {
           dblN = ulIndex + 1;

           dblHann = (M_2_PI * dblN) / (SampleNb + 1);
           dblHann = 0.5 * (1 - cos(dblHann));

           x[ulIndex] = x[ulIndex] * dblHann;
       }

       /* Calculate the number of points */
       n = 1;
       for (i=0; i<nbP; i++) 
       {
          n *= 2;
       }

       /* Do the bit reversal */
       i2 = n >> 1;
       j = 0;
       for (i=0;i<n-1;i++) 
       {
          if (i < j) 
          {
             tx = x[i];
             ty = y[i];
             x[i] = x[j];
             y[i] = y[j];
             x[j] = tx;
             y[j] = ty;
          }
          k = i2;
          while (k <= j) 
          {
             j -= k;
             k >>= 1;
          }
          j += k;
       }

       /* Compute the FFT */
       c1 = -1.0; 
       c2 = 0.0;
       l2 = 1;
       for (l=0; l<nbP; l++) 
       {
          l1 = l2;
          l2 <<= 1;
          u1 = 1.0; 
          u2 = 0.0;
          for (j=0;j<l1;j++)
          {
             for (i=j;i<n;i+=l2)
             {
                i1 = i + l1;
                t1 = u1 * x[i1] - u2 * y[i1];
                t2 = u1 * y[i1] + u2 * x[i1];
                x[i1] = x[i] - t1; 
                y[i1] = y[i] - t2;
                x[i] += t1;
                y[i] += t2;
             }
             z =  u1 * c1 - u2 * c2;
             u2 = u1 * c2 + u2 * c1;
             u1 = z;
          }
          
          c2 = sqrt((1.0 - c1) / 2.0);
          
          if (dir == 1) 
          {
             c2 = -c2;
          }
          
          c1 = sqrt((1.0 + c1) / 2.0);
       }

       /* Scaling for forward transform */
       if (dir == 1) 
       {
          for (i=0;i<n;i++) 
          {
             x[i] /= n;
             y[i] /= n;
          }
       }
       
       return;
    }
#endif