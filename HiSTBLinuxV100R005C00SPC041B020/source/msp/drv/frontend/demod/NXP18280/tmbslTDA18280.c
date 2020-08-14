/*
Copyright (C) 2010 NXP B.V., All Rights Reserved.
This source code and any compilation or derivative thereof is the proprietary
information of NXP B.V. and is confidential in nature. Under no circumstances
is this software to be  exposed to or placed under an Open Source License of
any type without the expressed written permission of NXP B.V.
*
* \file          tmbslTDA18280.c
*
*                %version: 2 %
*
* \date          %modify_time%
*
* \author        JMLD
*
* \brief         Describe briefly the purpose of this file.
*
* REFERENCE DOCUMENTS :
*                TDA18280_Driver_User_Guide.pdf
*
* \section info Change Information
*
*/

/*============================================================================*/
/* Standard include files:                                                    */
/*============================================================================*/
//#include <math.h>
#include "linux/kernel.h"

#include "tmNxTypes.h"
#include "tmCompId.h"
#include "tmFrontEnd.h"
#include "tmbslFrontEndTypes.h"
#include "tmUnitParams.h"

/*============================================================================*/
/* Project include files:                                                     */
/*============================================================================*/
#include "tmbslTDA18280_InstanceCustom.h"
#include "tmbslTDA18280.h"
#include "tmbslTDA18280_RegDef.h"
#include "tmbslTDA18280_Local.h"

#include "tmbslHCDP.h"
#include "tmbslHCDP_Advanced.h"

#include "tmbslOOBIp.h"

/*============================================================================*/
/* Static internal functions:                                                 */
/*============================================================================*/

/* Tilt Equalizer frequency plan */
static UInt32 iTDA18280_DefaultFreqPlan[TDA18280_TEQ_ARRAY_SIZE] = TDA18280_DEFAULT_FREQUENCY_PLAN;
static UInt32 iTDA18280_DefaultChannelPlan[tmTDA18280_Path_Max] = TDA18280_DEFAULT_CHANNEL_PLAN;
static tmTDA18280_TiltInputParameters_t  iDefaultTiltInputParameters = TDA18280_INSTANCE_CUSTOM_TEQ_INPUT;

static UInt32 iTDA18280_FreqPlan[TDA18280_TEQ_ARRAY_SIZE] = TDA18280_DEFAULT_FREQUENCY_PLAN;
static UInt32 iTDA18280_ChannelPlan[tmTDA18280_Path_Max] = TDA18280_DEFAULT_CHANNEL_PLAN;
static tmTDA18280_TiltMeasure_t iTDA18280_FreqGain[TDA18280_TEQ_ARRAY_SIZE];

/* read and write function used for CDP/OOB components */
#define CDP_ADDR_PATH0 (tda18280_reg_hcdp_ip0_CM_QAM_MODE__qam_mode_Base_Address)
#define CDP_ADDR_OFFSET ((tda18280_reg_hcdp_ip1_CM_QAM_MODE__qam_mode_Base_Address-tda18280_reg_hcdp_ip0_CM_QAM_MODE__qam_mode_Base_Address))
#define OOB_ADDR_PATH (tda18280_reg_oob_demod_OOBD_MODE_Base_Address)

static UInt16        iTDA18280_CdpRegisterAddr( UInt16 ulAddr, UInt8 ulCdpIndex );
static tmErrorCode_t iTDA18280_CdpRead(tmUnitSelect_t tUnit, UInt32 AddrSize, UInt8* pAddr, UInt32 ReadLen, UInt8* pData);
static tmErrorCode_t iTDA18280_CdpWrite(tmUnitSelect_t tUnit, UInt32 AddrSize, UInt8* pAddr, UInt32 WriteLen, UInt8* pData);

static tmErrorCode_t iTDA18280_OobRead(tmUnitSelect_t tUnit, UInt32 AddrSize, UInt8* pAddr, UInt32 ReadLen, UInt8* pData);
static tmErrorCode_t iTDA18280_OobWrite(tmUnitSelect_t tUnit, UInt32 AddrSize, UInt8* pAddr, UInt32 WriteLen, UInt8* pData);

static tmErrorCode_t iTDA18280_MutexInitDummy( ptmbslFrontEndMutexHandle *ppMutexHandle );
static tmErrorCode_t iTDA18280_MutexDeInitDummy( ptmbslFrontEndMutexHandle pMutexHandle );
static tmErrorCode_t iTDA18280_MutexAcquireDummy( ptmbslFrontEndMutexHandle pMutexHandle, UInt32 timeOut );
static tmErrorCode_t iTDA18280_MutexReleaseDummy( ptmbslFrontEndMutexHandle pMutexHandle );

/* Tilt Equalizer Rejection Lookup Table for 12.5dB */
static Int32 iTDA18280_RejectionLookupTable[23][2] = {{-40, 1}, {13, 3}, {22,4}, {31,5}, {38,6}, \
{46,8}, {55,10}, {64,12}, {72,15}, {78,18}, {84,21}, {90,26}, {94,29}, {101,37}, {106,45}, \
{109,51}, {111,56}, {116,73}, {117,78}, {121,106}, {126,480}, {127,480}, {128,480}};

static UInt16 TDA18280_IrqCDPMask[8] = {0x0001, 0x0004, 0x0010, 0x0040, 0x0100, 0x4000, 0x1000, 0x0400};

/*============================================================================*/
/* Static internal functions:                                                 */
/*============================================================================*/
Int32 iTDA18280_TEQCalculStepAverage( ptmTDA18280_Object_t pObj, UInt32 freq_index);
static UInt32 iTDA18280_TEQNextValidFreq( ptmTDA18280_Object_t pObj, UInt32 ulIndex );
static UInt32 iTDA18280_TEQPreviousValidFreq( ptmTDA18280_Object_t pObj, UInt32 ulIndex );
static UInt16 iTDA18280_CalculateGainRegValue( UInt32 ulGain );

static tmErrorCode_t iTDA18280_PathStart( ptmTDA18280_Object_t pObj, tmTDA18280_Path_t uPathNumber, const tmTDA18280_PathCfg_t* ptPathCfg, TDA18280_PathSetAGCMode_t eMode);
static tmErrorCode_t iTDA18280_PathStop( ptmTDA18280_Object_t pObj, tmTDA18280_Path_t uPathNumber);

#define TMBSL_TDA18280_NO_64BITS
/* array of temperatures in tenth of degree Celsius (value_Celsius*10) */
#define TEMPERATURE_INDEX_MAX 40
Int32 TDA18280_Temperature[TEMPERATURE_INDEX_MAX] = {
    -550, -457, -417, -375, -330, -284, -235, -183, -114,  -61,
     -21,   21,   65,  110,  157,  206,  256,  309,  364,  420, 
     461,  502,  545,  588,  633,  679,  726,  774,  824,  875,
     928,  982, 1025, 1069, 1114, 1160, 1207, 1255, 1305, 1355 };

static tmTDA18280_LogTable_t gTDA18280LogTable[] = { TDA18280_LOG_TABLE };

/*============================================================================*/
/* Global data:                                                               */
/*============================================================================*/
/* Driver instance */
tmTDA18280_Object_t gTDA18280Instance[TDA18280_MAX_UNITS] = 
{
    {
        (tmUnitSelect_t)(-1),                   /* Unit not set */
            (tmUnitSelect_t)(-1),               /* UnitW not set */
            Null,                               /* pMutex */
            False,                              /* init (instance initialization default) */
        {                                       /* sIo */
            Null,
                Null
        },
        {                                       /* sTime */
            Null,
                Null
            },
            {                                   /* sDebug */
                Null
            },
            {                                   /* sMutex */
                Null,
                    Null,
                    Null,
                    Null
                },
                TDA18280_INSTANCE_CUSTOM_0      /* Instance Customizable Settings */
    },
    {
        (tmUnitSelect_t)(-1),                   /* Unit not set */
            (tmUnitSelect_t)(-1),               /* UnitW not set */
            Null,                               /* pMutex */
            False,                              /* init (instance initialization default) */
        {                                       /* sIo */
            Null,
                Null
        },
        {                                       /* sTime */
            Null,
                Null
            },
            {                                   /* sDebug */
                Null
            },
            {                                   /* sMutex */
                Null,
                    Null,
                    Null,
                    Null
                },
                TDA18280_INSTANCE_CUSTOM_0     /* Instance Customizable Settings */
                }
};

/*============================================================================*/
/* Exported functions:                                                        */
/*============================================================================*/
/*=================================================================================================*/
/**
\brief  Create an instance of the TDA18280 device : initialize the memory 
reserved for the specified unit number with the specified data structure 
containing pointer to external functions.

\return TM_OK: Succeed.
\return TDA18280_ERR_BAD_PARAMETER: Pointer to external functions is not correct.
\return TDA18280_ERR_ALREADY_SETUP: This instance has already been open previously.
\return TDA18280_ERR_ERR_NO_INSTANCES: No Memory prepared for this instance.
\return TDA18280_ERR_NOT_INITIALIZED: The memory prepared is not correct (Marker issue).
\return Other optional return codes of the functions registered 
in DeviceOpenInstance API (Mutex...).

\note   No hardware access is done in this function.

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18280_DeviceOpenInstance(
                                 tmUnitSelect_t                    tUnit,      /* I: FrontEnd unit number */
                                 const tmbslFrontEndDependency_t*  psSrvFunc   /* I: setup parameters */
                                 )
{
    ptmTDA18280_Object_t       pObj = Null;
    tmErrorCode_t              err = TM_OK;
    UInt32                     ulIndex = 0;
    tmbslFrontEndDependency_t  sSrvFunc;
    tmbslFrontEndDependency_t  sSrvFuncOOB;
    tmUnitSelect_t             tUnitCdp = 0;

    /* Test parameter(s) */
    if(psSrvFunc == Null)
    {
        err = TDA18280_ERR_BAD_PARAMETER;
    }

    if(err == TM_OK)
    {
        /* Get a driver instance */
        err = iTDA18280_DeviceGetInstance(tUnit, &pObj);
    }

    /* Check driver instance state */
    if(err == TM_OK || err == TDA18280_ERR_ERR_NO_INSTANCES)
    {
        if(P_OBJ_VALID && pObj->init == True)
        {
            err = TDA18280_ERR_ALREADY_SETUP;
        }
        else 
        {
            if(P_OBJ_VALID == False)
            {
                /* Try to allocate an instance of the driver */
                err = iTDA18280_DeviceAllocInstance(tUnit, &pObj);
                if(err != TM_OK || pObj == Null)
                {
                    err = TDA18280_ERR_ERR_NO_INSTANCES;
                }
            }

            if(err == TM_OK)
            {
                /* Initialize the Object by default values */
                P_SIO = P_FUNC_SIO(psSrvFunc);
                P_STIME = P_FUNC_STIME(psSrvFunc);
                P_SDEBUG = P_FUNC_SDEBUG(psSrvFunc);

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

                tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, tUnit);

                if(err == TM_OK)
                {
                    pObj->init = True;

                    /* Initialize TEQ Default frequency Plan */
                    while( iTDA18280_FreqPlan[ulIndex]!= 0 )
                    {
                        iTDA18280_FreqGain[ulIndex].Frequency = iTDA18280_FreqPlan[ulIndex];
                        ulIndex += 1;
                    }
                   
                    if (ulIndex >= 1)
                    {
                        pObj->TiltOutputParameters.uTDA18280_LastFreqIndex = ulIndex - 1;
                    }
                    else
                    {
                        pObj->TiltOutputParameters.uTDA18280_LastFreqIndex = 0;
                    }

                    /* fill the rest of iTDA18280_FreqGain[] with 0 value */
                    while( ulIndex < TDA18280_TEQ_ARRAY_SIZE)
                    {
                        iTDA18280_FreqGain[ulIndex].Frequency = 0;
                        iTDA18280_FreqGain[ulIndex].Gain = 0;
                        iTDA18280_FreqGain[ulIndex].GainAfterRejection = 0;
                        iTDA18280_FreqGain[ulIndex].GainCorrected = 0;

                        ulIndex += 1;                        
                    }   
                }
            }
        }
    }

    /* open instances required for CDP */
    if( err == TM_OK )
    {
        /* first check that tUnit does not already used the specified bits */
        tUnitCdp = ADDITIONNAL_INFO_GET(tUnit);

        if( tUnitCdp != 0 )
        {
            err = TDA18280_ERR_BAD_UNIT_NUMBER;
        }
    }

    if( err == TM_OK )
    {
        // do not use semaphore
        sSrvFunc.sMutex.Init = iTDA18280_MutexInitDummy;
        sSrvFunc.sMutex.DeInit = iTDA18280_MutexDeInitDummy;
        sSrvFunc.sMutex.Acquire = iTDA18280_MutexAcquireDummy;
        sSrvFunc.sMutex.Release = iTDA18280_MutexReleaseDummy;

        sSrvFunc.sTime.Get = psSrvFunc->sTime.Get;
        sSrvFunc.sTime.Wait = psSrvFunc->sTime.Wait;

        sSrvFunc.sDebug.Print = psSrvFunc->sDebug.Print;
        
        sSrvFunc.dwAdditionalDataSize = 0;

        /* update read and write  */
        sSrvFunc.sIo.Read  = iTDA18280_CdpRead;
        sSrvFunc.sIo.Write = iTDA18280_CdpWrite;

        /* initialise each CDP (demod) path */
        ulIndex = 0;

        do
        {
            /* tunit is modified in order to indicate CDP path */
            tUnitCdp = tUnit + ADDITIONNAL_INFO_VAL((UInt16)ulIndex);
            
            err = tmbslHCDP_Init( tUnitCdp, &sSrvFunc );

            ulIndex ++;
        } while( (ulIndex < TDA18280_NB_CDP) && (err == TM_OK) );
    }

    /* open instance OOB*/
    if( err == TM_OK )
    {
        sSrvFuncOOB.sMutex.Init = iTDA18280_MutexInitDummy;
        sSrvFuncOOB.sMutex.DeInit = iTDA18280_MutexDeInitDummy;
        sSrvFuncOOB.sMutex.Acquire = iTDA18280_MutexAcquireDummy;
        sSrvFuncOOB.sMutex.Release = iTDA18280_MutexReleaseDummy;

        sSrvFuncOOB.sTime.Get = psSrvFunc->sTime.Get;
        sSrvFuncOOB.sTime.Wait = psSrvFunc->sTime.Wait;

        sSrvFuncOOB.sDebug.Print = psSrvFunc->sDebug.Print;

        sSrvFuncOOB.dwAdditionalDataSize = 0;

        /* update read and write  */
        sSrvFuncOOB.sIo.Read  = iTDA18280_OobRead;
        sSrvFuncOOB.sIo.Write = iTDA18280_OobWrite;

        err = tmbslOOBIp_Init( pObj->tUnitW, &sSrvFuncOOB);
    }

    /* this should be in the configuration file, but the component was not designed this way */
    if( err == TM_OK )
    {
        /* indicate the address of the first register */
        err = tmbslOOBIp_SetStartAddressOffset( pObj->tUnitW, OOB_ADDR_PATH );
    }

    return err;
}

/*=================================================================================================*/
/**
\brief  Close the instance of the TDA18280 device: Desinititalize the memory 
reserved for the specified unit number.

\return TM_OK: Succeed.
\return TDA18280_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
\return Other optional return codes of the functions registered
in DeviceOpenInstance API (Mutex...).

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18280_DeviceCloseInstance(
                                  tmUnitSelect_t  tUnit   /* I: FrontEnd unit number */
                                  )
{
    ptmTDA18280_Object_t   pObj = Null;
    tmErrorCode_t          err = TM_OK;
    tmUnitSelect_t         tUnitCdp = 0;
    UInt32                 ulIndex = 0;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);

    if(err == TM_OK)
    {
#ifdef _TVFE_IMPLEMENT_MUTEX
        /* Try to acquire driver mutex */
        err = iTDA18280_MutexAcquire(pObj, TDA18280_MUTEX_TIMEOUT);

        if(err == TM_OK)
        {
#endif
            tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, tUnit);

#ifdef _TVFE_IMPLEMENT_MUTEX
            P_SMUTEX_ACQUIRE = Null;

            /* Release driver mutex */
            (void)iTDA18280_MutexRelease(pObj);

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

        /* close each CDP (demod) path */
        ulIndex = 0;

        do
        {
            /* tunit is modified in order to indicate CDP path */
            tUnitCdp = tUnit + ADDITIONNAL_INFO_VAL((UInt16)ulIndex);
            
            err = tmbslHCDP_DeInit( tUnitCdp );

            ulIndex ++;
        } while( (ulIndex < TDA18280_NB_CDP) && (err == TM_OK) );

        if( err == TM_OK )
        {
            err = iTDA18280_DeviceDeAllocInstance(pObj);
        }

        /* close OOB */
        if( err == TM_OK )
        {
            err = tmbslOOBIp_DeInit( pObj->tUnitW);
        }
    }

    return err;
}

/*=================================================================================================*/
/**
\brief  Get the software version of the TDA18280 device driver.

\return TM_OK: Succeed.

\note   ptmSWVersion_t is defined in the tmnxtypes.h file.

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18280_DeviceGetSWVersion(
                                 ptmSWVersion_t  pSWVersion  /* I: Receives SW Version */
                                 )
{
    pSWVersion->compatibilityNr = TDA18280_COMP_NUM;
    pSWVersion->majorVersionNr  = TDA18280_MAJOR_VER;
    pSWVersion->minorVersionNr  = TDA18280_MINOR_VER;

    return TM_OK;
}

/*=================================================================================================*/
/**
\brief  Get the version of the TDA18280 device configuration file.

\return TM_OK: Succeed.

\note   ptmSWSettingsVersion_t is defined in the tmnxtypes.h file.

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18280_DeviceGetSWSettingsVersion(
    ptmSWSettingsVersion_t pSWSettingsVersion   /* O: Receives SW Settings Version */
    )
{
    pSWSettingsVersion->customerNr      = TDA18280_SETTINGS_CUSTOMER_NUM;
    pSWSettingsVersion->projectNr       = TDA18280_SETTINGS_PROJECT_NUM;
    pSWSettingsVersion->majorVersionNr  = TDA18280_SETTINGS_MAJOR_VER;
    pSWSettingsVersion->minorVersionNr  = TDA18280_SETTINGS_MINOR_VER;

    return TM_OK;
}

/*=================================================================================================*/
/**
\brief  Perform the hardware initialization of the device.

\return TM_OK: Succeed.
\return TDA18280_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
\return TDA18280_ERR_BAD_PARAMETER: Parameters out of range.
\return Other optional return codes of the functions registered 
in DeviceOpenInstance API (I2C read errors, mutex...).

\note   To be performed once tmbslTDA18280_DeviceOpenInstance has been called.

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18280_DeviceHwInit(
                           tmUnitSelect_t  tUnit   /* I: Unit number */
                           )
{
    ptmTDA18280_Object_t   pObj = Null;
    tmErrorCode_t          err = TM_OK;
    UInt32                 ulIndex = 0;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18280)

        tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, tUnit);

    /* Reset Hw State */
    pObj->eHwState = tmTDA18280_HwState_InitNotDone;
    pObj->curPowerState = tmTDA18280_PowerMax;

    if (err == TM_OK)
    {
        err = iTDA18280_DeviceGetHWVersion(pObj);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18280_DeviceGetHWVersion(0x%08X) failed.", tUnit));
    }

    if (err == TM_OK)
    {
        err = iTDA18280_DeviceHwInit(pObj);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18280_DeviceHwInit(0x%08X) failed.", tUnit));
    }

    /* set power state in standby mode */
    if (err == TM_OK)
    {
        err = iTDA18280_DevicePowerStateSet(pObj, tmTDA18280_PowerStandby);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18280_DevicePowerStateSet(0x%08X) failed.", tUnit));
    }

    if(err == TM_OK)
    {
        /* inform that init phase has been done */
        if(err == TM_OK)
        {
            pObj->eHwState = tmTDA18280_HwState_InitDone;
        }
        /* initialize pObj */
        if(err == TM_OK)
        {
            for (ulIndex = 0; ulIndex <= tmTDA18280_Path_Aux; ulIndex++)
            {
                pObj->Path[ulIndex].tPathStatus = tmTDA18280_OFF;
                pObj->PathCurrentState[ulIndex] = tmTDA18280_OFF;
            }
        }
    }

    _MUTEX_RELEASE(TDA18280)

    return err;
}

/*=================================================================================================*/
/**
\brief  Set the power state of the device.

\return TM_OK: Succeed.
\return TDA18280_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
\return TDA18280_ERR_BAD_PARAMETER: Parameters out of range.
\return Other optional return codes of the functions registered 
in DeviceOpenInstance API (I2C read errors, mutex...).

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18280_DeviceSetPowerState(
                                  tmUnitSelect_t            tUnit,      /* I: Unit number */
                                  tmTDA18280_PowerState_t   powerState  /* I: Power state */
                                  )
{
    ptmTDA18280_Object_t   pObj = Null;
    tmErrorCode_t          err = TM_OK;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18280)

        tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, tUnit);

    if(powerState >= tmTDA18280_PowerMax)
    {
        err = TDA18280_ERR_BAD_PARAMETER;
    }
    
    if(err == TM_OK)
    {
        if (pObj->eHwState == tmTDA18280_HwState_InitNotDone)
        {
            err = TDA18280_ERR_NOT_INITIALIZED;
        }
    }

    if(err == TM_OK)
    {
        if(powerState != pObj->curPowerState)
        {
            err = iTDA18280_DevicePowerStateSet(pObj, powerState);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18280_DevicePowerStateSet(0x%08X, %d) failed.", tUnit, (int)powerState));
        }
    }

    if(err == TM_OK)
    {
        /* Store power state in driver instance */
        pObj->curPowerState = powerState;
    }

    _MUTEX_RELEASE(TDA18280)

        return err;
}

/*=================================================================================================*/
/**
\brief  Get the power state of the device.

\return TM_OK: Succeed.
\return TDA18280_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
\return TDA18280_ERR_BAD_PARAMETER: Parameters out of range.
\return Other optional return codes of the functions registered 
in DeviceOpenInstance API (I2C read errors, mutex...).

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18280_DeviceGetPowerState(
                                  tmUnitSelect_t            tUnit,      /* I: Unit number */
                                  tmTDA18280_PowerState_t*  pPowerState /* O: Power state */
                                  )
{
    ptmTDA18280_Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18280)

        tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, tUnit);

    /* Test parameter(s) */
    if(pPowerState == Null)
    {
        err = TDA18280_ERR_BAD_PARAMETER;
    }

    if(err == TM_OK)
    {
        /* retrieve power configuration */
        err = iTDA18280_DevicePowerStateGet(pObj, pPowerState);
    }

    _MUTEX_RELEASE(TDA18280)

        return err;
}

/*=================================================================================================*/
/**
\brief  Get Product name

\return TM_OK: Succeed.
\return TDA18280_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
\return TDA18280_ERR_BAD_PARAMETER: Parameters out of range.
\return Other optional return codes of the functions registered 
        in DeviceOpenInstance API (I2C read errors, mutex...).

\note   .

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
    tmbslTDA18280_DeviceGetProductName(
    tmUnitSelect_t       tUnit,
    tmTDA18280_ProductType_t* pProductName
    )
{
    ptmTDA18280_Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18280)

        tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, tUnit);

    /* Test parameter(s) */
    if(pProductName == Null)
    {
        err = TDA18280_ERR_BAD_PARAMETER;
    }

    if(err == TM_OK)
    {
        *pProductName = pObj->eProductType;
    }

    _MUTEX_RELEASE(TDA18280)

        return err;
}

/*=================================================================================================*/
/**
\brief  Set the demod FEC mode.

\return TM_OK: Succeed.
\return TDA18280_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
\return TDA18280_ERR_BAD_PARAMETER: Parameters out of range.
\return Other optional return codes of the functions registered 
in DeviceOpenInstance API (I2C read errors, mutex...).

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
    tmbslTDA18280_DeviceSetFECMode(
    tmUnitSelect_t              tUnit,          /**< I: Unit number. */
    tmFrontEndFECMode_t         eFECMode        /**< I: FEC mode */
    )
{
    ptmTDA18280_Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18280)

        tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, tUnit);

    if(err == TM_OK)
    {
        if( (eFECMode > tmFrontEndFECModeUnknown) &&  (eFECMode < tmFrontEndFECModeMax))
        {
            pObj->eFECMode = eFECMode;
        }
    }

    _MUTEX_RELEASE(TDA18280)

        return err;
}

/*=================================================================================================*/
/**
\brief  Get the demod FEC mode.

\return TM_OK: Succeed.
\return TDA18280_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
\return TDA18280_ERR_BAD_PARAMETER: Parameters out of range.
\return Other optional return codes of the functions registered 
in DeviceOpenInstance API (I2C read errors, mutex...).

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
    tmbslTDA18280_DeviceGetFECMode(
    tmUnitSelect_t              tUnit,          /**< I: Unit number. */
    tmFrontEndFECMode_t*        peFECMode       /**< O: FEC mode */
    )
{
    ptmTDA18280_Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18280)

        tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, tUnit);

    /* Test parameter(s) */
    if (err == TM_OK)
    {
        if(peFECMode == Null)
        {
            err = TDA18280_ERR_BAD_PARAMETER;
        }
        else
        {
            *peFECMode = pObj->eFECMode;
        }
    }

    _MUTEX_RELEASE(TDA18280)

    return err;
}

/*=================================================================================================*/
/**
\brief  Set the OOB FEC mode.

\return TM_OK: Succeed.
\return TDA18280_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
\return TDA18280_ERR_BAD_PARAMETER: Parameters out of range.
\return Other optional return codes of the functions registered 
in DeviceOpenInstance API (I2C read errors, mutex...).

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
    tmbslTDA18280_DeviceSetOOBFECMode(
    tmUnitSelect_t              tUnit,          /**< I: Unit number. */
    tmTDA18280_OOBFECMode_t     eFECMode        /**< I: FEC mode */
    )
{
    ptmTDA18280_Object_t   pObj = Null;
    tmErrorCode_t          err = TM_OK;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18280)

        tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, tUnit);

    if (err == TM_OK)
    {
        if (!pObj->tFeature.bOOBSupported)
        {
            err = TDA18280_ERR_NOT_SUPPORTED;
        }
        else if( eFECMode < tmTDA18280_OOBFECMode_Max)
        {
            pObj->eOOBFECMode = eFECMode;
        }
    }

    _MUTEX_RELEASE(TDA18280)

        return err;
}

/*=================================================================================================*/
/**
\brief  Get the OOB FEC mode.

\return TM_OK: Succeed.
\return TDA18280_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
\return TDA18280_ERR_BAD_PARAMETER: Parameters out of range.
\return Other optional return codes of the functions registered 
in DeviceOpenInstance API (I2C read errors, mutex...).

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
    tmbslTDA18280_DeviceGetOOBFECMode(
    tmUnitSelect_t              tUnit,          /**< I: Unit number. */
    tmTDA18280_OOBFECMode_t*    peFECMode       /**< O: FEC mode */
    )
{
    ptmTDA18280_Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18280)

        tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, tUnit);

    /* Test parameter(s) */
    if (err == TM_OK)
    {
        if(peFECMode == Null)
        {
            err = TDA18280_ERR_BAD_PARAMETER;
        }
        else if (!pObj->tFeature.bOOBSupported)
        {
            err = TDA18280_ERR_NOT_SUPPORTED;
        }
        else
        {
            *peFECMode = pObj->eOOBFECMode;
        }
    }

    _MUTEX_RELEASE(TDA18280)

    return err;
}

/*=================================================================================================*/
/**
\brief  Clear, Enable or Disable IRQ lock or unlock of the specified path

\return TM_OK: Succeed.
\return TDA18280_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
\return TDA18280_ERR_BAD_PARAMETER: Parameters out of range.
\return Other optional return codes of the functions registered 
in DeviceOpenInstance API (I2C read errors, mutex...).

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
    tmbslTDA18280_PathIrqSet(
    tmUnitSelect_t              tUnit,          /**< I: Unit number. */
    tmTDA18280_Path_t           tPathNumber,    /**< Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).*/
    tmTDA18280_typeIrq_t        TypeIRQ         /**< I: None / Lock / unlock */
    )
{
    ptmTDA18280_Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;
    UInt16  uIrq_demod = (UInt16)TDA18280_NO_IRQ;
    UInt16 tIrq;
    tmTDA18280_Demod_t tDemodIndex;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18280)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, tUnit);

    /* interrupt for demod lock/unlock is not supported for ES1 */
    if (pObj->EsVersion == tmTDA18280_EsVersion1)
    {
        err = TDA18280_ERR_NOT_SUPPORTED;
    }

    if (err == TM_OK)
    {
        err = iTDA18280_GetDemodIndex(pObj, tPathNumber, &tDemodIndex );
    }

    if (err == TM_OK && tDemodIndex<=tmTDA18280_Demod_CDP_7)  /* CDP IRQ */
    {
        tIrq = TDA18280_IrqCDPMask[tDemodIndex];


        /*  clear the UNLOCK IRQ  */
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_config_IRQ_UNLOCK_HCDP_CLEAR, tIrq);

        /* read IRQ UNLOCK register */
        if(err == TM_OK)
        {
            err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_config_IRQ_UNLOCK_HCDP_ENABLE, 1, &uIrq_demod);
        }

        /* enable or disable UNLOCK IRQ */
        uIrq_demod = ( TypeIRQ == tmTDA18280_typeIrq_UnLock ?  uIrq_demod | tIrq : uIrq_demod & ~ tIrq);

        if(err == TM_OK)
        {
            err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_config_IRQ_UNLOCK_HCDP_ENABLE, uIrq_demod);
        }

        /*  clear the LOCK IRQ  */
        if(err == TM_OK)
        {
            err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_config_IRQ_LOCK_HCDP_CLEAR, tIrq);
        }

        if(err == TM_OK)
        {
            err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_config_IRQ_LOCK_HCDP_ENABLE, 1, &uIrq_demod);
        }

        /* enable or disable LOCK IRQ */
        uIrq_demod = ( TypeIRQ == tmTDA18280_typeIrq_Lock ? uIrq_demod | tIrq : uIrq_demod & ~ tIrq);

        if(err == TM_OK)
        {
            err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_config_IRQ_LOCK_HCDP_ENABLE, uIrq_demod);
        }

    }
    else if (err == TM_OK && tDemodIndex==tmTDA18280_Demod_OOB) /* OOB IRQ */
    {
        /* first clear the IRQ FEC LOCK and UNLOCK */
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_config_IRQ_OOB_CLEAR, 0x05);

        tIrq = ( TypeIRQ == tmTDA18280_typeIrq_Lock ? 0x04: TypeIRQ == tmTDA18280_typeIrq_UnLock ? 0x01 : 0x00); // FEC lock / unlock / none
        /* enable or disable IRQ FEC LOCK or UNLOCK*/
        if(err == TM_OK)
        {
            err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_config_IRQ_OOB_ENABLE, tIrq);
        }
    }

   _MUTEX_RELEASE(TDA18280)

   return err;
}


/*=================================================================================================*/
/**
\brief  Read IRQ lock or unlock of the specified path 

\return TM_OK: Succeed.
\return TDA18280_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
\return TDA18280_ERR_BAD_PARAMETER: Parameters out of range.
\return Other optional return codes of the functions registered 
in DeviceOpenInstance API (I2C read errors, mutex...).

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
    tmbslTDA18280_PathIrqRead(
    tmUnitSelect_t              tUnit,          /**< I: Unit number. */
    tmTDA18280_Path_t           tPathNumber,    /**< Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).*/
    tmTDA18280_typeIrq_t        TypeIRQ,        /**< I: Lock / unlock */
    Bool*                       bIrq            /**< O: return True if <TypeIRQ> raised else False */
    ) 
{
    ptmTDA18280_Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;
    UInt16  uIrq_status = (UInt16)TDA18280_NO_IRQ;
    UInt16  uIrq_enable = (UInt16)TDA18280_NO_IRQ;
    tmTDA18280_Demod_t tDemodIndex;
    UInt16 tIrq;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18280)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, tUnit);

    /* interrupt for demod lock/unlock is not supported for ES1 */
    if (pObj->EsVersion == tmTDA18280_EsVersion1)
    {
        err = TDA18280_ERR_NOT_SUPPORTED;
    }

    /* initialize to handle errors */
    if (err == TM_OK)
    {
        if (bIrq == Null)
        {
            err = TDA18280_ERR_BAD_PARAMETER;
        }
    }
    if (err == TM_OK)
    {
        err = iTDA18280_GetDemodIndex(pObj, tPathNumber, &tDemodIndex );
    }


    if (err == TM_OK && tDemodIndex<=tmTDA18280_Demod_CDP_7)  /* CDP IRQ */
    {
        tIrq = TDA18280_IrqCDPMask[tDemodIndex];

        /*get IRQs for CDP */
        if (TypeIRQ == tmTDA18280_typeIrq_Lock)
        {
            err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_config_IRQ_LOCK_HCDP_STATUS, 1, &uIrq_status);
            if (err == TM_OK)
            {
                err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_config_IRQ_LOCK_HCDP_ENABLE , 1, &uIrq_enable);
            }
        }
        else  // Unlock
        {
            err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_config_IRQ_UNLOCK_HCDP_STATUS, 1, &uIrq_status);
            if (err == TM_OK)
            {
                err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_config_IRQ_UNLOCK_HCDP_ENABLE , 1, &uIrq_enable);
            }
        }
        *bIrq = ( uIrq_status & uIrq_enable & tIrq) ? True:False;
    }
    else if (err == TM_OK && tDemodIndex==tmTDA18280_Demod_OOB) /* OOB IRQ */
    {
        err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_config_IRQ_OOB_STATUS, 1, &uIrq_status);
        if (err == TM_OK)
        {
            err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_config_IRQ_OOB_ENABLE, 1, &uIrq_enable);
        }
        *bIrq = ((uIrq_status) & (uIrq_enable)& (TypeIRQ == tmTDA18280_typeIrq_Lock ? 0x04:0x01)) ? True:False;
    }

    _MUTEX_RELEASE(TDA18280)

    return err;
}

/*=================================================================================================*/
/**
\brief  Perform the configuration of the specified path of the device.

\return TM_OK: Succeed.
\return TDA18280_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
\return TDA18280_ERR_BAD_PARAMETER: Parameters out of range.
\return TDA18280_ERR_NOT_SUPPORTED: indicates that the function can not be called in current state
\return Other optional return codes of the functions registered 
in DeviceOpenInstance API (I2C read errors, mutex...).

\note   To be performed once tmbslTDA18280_DeviceHwInit has been called.

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18280_PathSet(
                      tmUnitSelect_t              tUnit,      /**< I: Unit number. */
                      tmTDA18280_Path_t           tPathNumber,/**< Path selector. From 0 to (tmTDA18280_Path_Max-1), including Auxiliary path (tmTDA18280_Path_Aux).*/
                      const tmTDA18280_PathCfg_t* ptPathCfg   /**< I: Pointer to Path Configuration structure. */
                      )
{
    ptmTDA18280_Object_t pObj = Null;
    tmErrorCode_t        err = TM_OK;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18280)

        tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, tUnit);

    /* test input parameters */
    if (err == TM_OK)
    {
        if ((ptPathCfg == Null) || (tPathNumber >= tmTDA18280_Path_Max))
        {
            err = TDA18280_ERR_BAD_PARAMETER;
        }
    }

    /* check power state */
    switch( pObj->curPowerState )
    {
    case tmTDA18280_PowerWBOnly:
        /* all paths can be used except NB/auxiliary path */
        if (tPathNumber == tmTDA18280_Path_Aux)
        {
            err = TDA18280_ERR_NOT_SUPPORTED;
        }
        break;
    case tmTDA18280_PowerWBAndNB:
        /* all paths (WB and NB) can be used */
        break;
    case tmTDA18280_PowerNBOnly:
        /* only NB/auxiliary path can be used */
        if (tPathNumber != tmTDA18280_Path_Aux)
        {
            err = TDA18280_ERR_NOT_SUPPORTED;
        }
        break;
    default:
    case tmTDA18280_PowerStandby:
        /* nothing can be programmed */
        err = TDA18280_ERR_NOT_SUPPORTED;
        break;
    }

    if(err == TM_OK)
    {
        if (pObj->eHwState == tmTDA18280_HwState_InitNotDone)
        {
            err = TDA18280_ERR_NOT_INITIALIZED;
        }
    }

    if (err == TM_OK)
    {
        err = iTDA18280_PathSet(pObj, tPathNumber, ptPathCfg, tmTDA18280_PathSetAGCMode_Normal);
    }

    _MUTEX_RELEASE(TDA18280)

    return err;
}

/*=================================================================================================*/
/**
\brief  Read the configuration of the specified path of the device.

\return TM_OK: Succeed.
\return TDA18280_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
\return TDA18280_ERR_BAD_PARAMETER: Parameters out of range.
\return Other optional return codes of the functions registered 
in DeviceOpenInstance API (I2C read errors, mutex...).

\note   To be performed once tmbslTDA18280_DeviceHwInit has been called.

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18280_PathGet(
                      tmUnitSelect_t              tUnit,      /**< I: Unit number. */
                      tmTDA18280_Path_t           tPathNumber,/**< Path selector. From 0 to (tmTDA18280_Path_Max-1), including Auxiliary path (tmTDA18280_Path_Aux).*/
                      tmTDA18280_PathCfg_t*       ptPathCfg   /**< I: Pointer to Path Configuration structure. */                 
                      )
{
    ptmTDA18280_Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;
    UInt8               Clock_OK = 0 ;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18280)

        tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, tUnit);

    if (err == TM_OK)
    {
        /* test input parameters */
        if ((ptPathCfg == Null) || (tPathNumber >= tmTDA18280_Path_Max))
        {
            err = TDA18280_ERR_BAD_PARAMETER;
        }
        else
        {
            /* initialize ptPathCfg to handle errors */
            /* in case the config can not be retrieved */
            *ptPathCfg = pObj->Path[tPathNumber];
            ptPathCfg->tPathStatus = tmTDA18280_OFF;
            ptPathCfg->uGainMultBy10 = 0;
        }
    }

    if (pObj->eHwState == tmTDA18280_HwState_InitNotDone ||
        pObj->curPowerState == tmTDA18280_PowerStandby ||
        pObj->curPowerState == tmTDA18280_PowerMax)
    {
        pObj->PathCurrentState[tPathNumber] = tmTDA18280_OFF;
    }
    else
    {
        if(err == TM_OK)
        {
            err = iTDA18280_PathDdcClockCheck(pObj, tPathNumber, &Clock_OK);
        }

        if( (err == TM_OK) && (Clock_OK == 1) )
        {
            pObj->PathCurrentState[tPathNumber] = tmTDA18280_ON;
        }
        else
        {
            pObj->PathCurrentState[tPathNumber] = tmTDA18280_OFF;
        }

        /* retrieve the path information */
        if( (err == TM_OK) && (pObj->PathCurrentState[tPathNumber] == tmTDA18280_ON) )
        {
            err = iTDA18280_PathGet(pObj, tPathNumber, ptPathCfg);
        }
    }

    if(err == TM_OK)
    {
        ptPathCfg->tPathStatus = pObj->PathCurrentState[tPathNumber];
    }

    _MUTEX_RELEASE(TDA18280)

    return err;
}

/*=================================================================================================*/
/**
\brief  Write the I2C gate configuration of the device.

\return TM_OK: Succeed.
\return TDA18280_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
\return TDA18280_ERR_BAD_PARAMETER: Parameters out of range.
\return Other optional return codes of the functions registered 
in DeviceOpenInstance API (I2C read errors, mutex...).

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18280_DeviceI2CGate_SetCfg(
                                   tmUnitSelect_t              tUnit,          /**< I: Unit number. */
                                   tmTDA18280_I2CGate_t*       pI2CGateCfg     /**< I: Configuration of the Gate. */
                                   )
{
    ptmTDA18280_Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18280)

        tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, tUnit);

    /* Test parameter(s) */
    if(pI2CGateCfg == Null)
    {
        err = TDA18280_ERR_BAD_PARAMETER;
    }

    if(err == TM_OK)
    {
        pObj->I2CGateCfg = *pI2CGateCfg;
    }

    if(err == TM_OK)
    {
        err=iTDA18280_BlockI2CGate_SetCfg(pObj, pI2CGateCfg);
    }

    _MUTEX_RELEASE(TDA18280)

        return err;
}

/*=================================================================================================*/
/**
\brief  Read the I2C gate configuration of the device.

\return TM_OK: Succeed.
\return TDA18280_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
\return TDA18280_ERR_BAD_PARAMETER: Parameters out of range.
\return Other optional return codes of the functions registered 
in DeviceOpenInstance API (I2C read errors, mutex...).

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18280_DeviceI2CGate_GetCfg(
                                   tmUnitSelect_t              tUnit,          /**< I: Unit number. */
                                   tmTDA18280_I2CGate_t*       pI2CGateCfg     /**< O: Configuration of the Gate. */
                                   )
{
    ptmTDA18280_Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18280)

        tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, tUnit);

    /* Test parameter(s) */
    if(pI2CGateCfg == Null)
    {
        err = TDA18280_ERR_BAD_PARAMETER;
    }

    if(err == TM_OK)
    {
        err=iTDA18280_BlockI2CGate_GetCfg(pObj);
    }

    if(err == TM_OK)
    {
        *pI2CGateCfg = pObj->I2CGateCfg;
    }

    _MUTEX_RELEASE(TDA18280)

        return err;
}

/*=================================================================================================*/
/**
\brief  Get Tilt Equalizer Estimation.

\return TM_OK: Succeed.
\return TDA18280_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
\return TDA18280_ERR_BAD_PARAMETER: Parameters out of range.
\return Other optional return codes of the functions registered 
in DeviceOpenInstance API (I2C read errors, mutex...).

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18280_DeviceGetTiltEqualizerEstimation( tmUnitSelect_t          tUnit,
                                               tmTDA18280_TiltInputParameters_t* pTiltInputParameters,
                                               tmTDA18280_TiltOutputParameters_t* pTiltOutputParameters )
{
    ptmTDA18280_Object_t   pObj = Null;
    tmErrorCode_t          err = TM_OK;
    UInt32                 ulIndex = 0;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18280)

        tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, tUnit);

    /* initialise result */
    pObj->TiltOutputParameters.TDA18280_CalculatedTilt = tmTDA18280_TiltEqualizer0dB;
    pObj->TiltOutputParameters.iTDA18280_Slope = 0;
    pObj->TiltOutputParameters.uTDA18280_RSquare = 0;
    pObj->TiltOutputParameters.uTDA18280_T0Square = 0;
    pObj->TiltOutputParameters.uTDA18280_MaxGain = 0;
    pObj->TiltOutputParameters.uTDA18280_FreqMaxGain = 0;
    pObj->TiltOutputParameters.uTDA18280_MinGain = 0;
    pObj->TiltOutputParameters.uTDA18280_FreqMinGain = 0;
    pObj->TiltOutputParameters.uTDA18280_NbEmptyChannel = 0;
    pObj->TiltOutputParameters.bTDA18280_BlockOfChannelDetected = False;
    pObj->TiltOutputParameters.bTDA18280_StepDetected = False;
    pObj->TiltOutputParameters.uTDA18280_StepDetectedFreq = 0;
    pObj->TiltOutputParameters.iTDA18280_StepDetectedValue = 0;

    /* Take into account TEQ Input Parameters */
    if (pTiltInputParameters->bUseInputParameter == True)
    {
        /* Initialize TEQ Default channel Plan */
        while( pTiltInputParameters->pTiltChannelPlan[ulIndex]!= 0xFF )
        {
            iTDA18280_ChannelPlan[ulIndex] = pTiltInputParameters->pTiltChannelPlan[ulIndex];
            ulIndex += 1;
        }
        iTDA18280_ChannelPlan[ulIndex] = 0xFF;
        ulIndex = 0;

        /* Initialize TEQ Default Frequency Plan */
        while( pTiltInputParameters->pTiltFrequencyPlan[ulIndex]!= 0x0 )
        {
            iTDA18280_FreqPlan[ulIndex] = pTiltInputParameters->pTiltFrequencyPlan[ulIndex];

            iTDA18280_FreqGain[ulIndex].Frequency = iTDA18280_FreqPlan[ulIndex];
            iTDA18280_FreqGain[ulIndex].Gain = 0;
            iTDA18280_FreqGain[ulIndex].GainAfterRejection = 0;
            iTDA18280_FreqGain[ulIndex].GainCorrected = 0;

            ulIndex += 1;
        }

        if( ulIndex >= 1 )
        {
            pObj->TiltOutputParameters.uTDA18280_LastFreqIndex = ulIndex-1;
        }
        else
        {
            pObj->TiltOutputParameters.uTDA18280_LastFreqIndex = 0;
        }
        iTDA18280_FreqPlan[ulIndex] = 0x0;
        iTDA18280_FreqGain[ulIndex].Frequency = 0x0;

        pObj->TiltInputParameters.TDA18280_DefaultTilt                = pTiltInputParameters->TDA18280_DefaultTilt;
        pObj->TiltInputParameters.bTDA18280_RejectionEnabled          = pTiltInputParameters->bTDA18280_RejectionEnabled;
        pObj->TiltInputParameters.uTDA18280_NbChannels                = pTiltInputParameters->uTDA18280_NbChannels;
        pObj->TiltInputParameters.uTDA18280_MinNbChannelsNotEmpty     = pTiltInputParameters->uTDA18280_MinNbChannelsNotEmpty;
        pObj->TiltInputParameters.uTDA18280_StepAverage               = pTiltInputParameters->uTDA18280_StepAverage;
        pObj->TiltInputParameters.uTDA18280_SlopeThreshold            = pTiltInputParameters->uTDA18280_SlopeThreshold;
        pObj->TiltInputParameters.uTDA18280_StepThreshold             = pTiltInputParameters->uTDA18280_StepThreshold;
        pObj->TiltInputParameters.uTDA18280_CorrelThreshold           = pTiltInputParameters->uTDA18280_CorrelThreshold;
        pObj->TiltInputParameters.uTDA18280_EmptyThreshold            = pTiltInputParameters->uTDA18280_EmptyThreshold;
        pObj->TiltInputParameters.uTDA18280_FrequencyPercentThreshold = pTiltInputParameters->uTDA18280_FrequencyPercentThreshold;
        pObj->TiltInputParameters.uTDA18280_StepPercentThreshold      = pTiltInputParameters->uTDA18280_StepPercentThreshold;
        pObj->TiltInputParameters.uTDA18280_StepFreqRange             = pTiltInputParameters->uTDA18280_StepFreqRange;
        pObj->TiltInputParameters.uTDA18280_Wait                      = pTiltInputParameters->uTDA18280_Wait;
        pObj->TiltInputParameters.eTDA18280_BandWidth                 = pTiltInputParameters->eTDA18280_BandWidth;
    }
    /* reload default TEQ parameters */
    else
    {
        /* Initialize TEQ Default channel Plan */
        while( iTDA18280_DefaultChannelPlan[ulIndex]!= 0xFF )
        {
            iTDA18280_ChannelPlan[ulIndex] = iTDA18280_DefaultChannelPlan[ulIndex];
            ulIndex += 1;
        }
        iTDA18280_ChannelPlan[ulIndex] = 0xFF;
        ulIndex = 0;

        /* Initialize TEQ Default Frequency Plan */
        while( iTDA18280_DefaultFreqPlan[ulIndex]!= 0x0 )
        {
            iTDA18280_FreqPlan[ulIndex] = iTDA18280_DefaultFreqPlan[ulIndex];

            iTDA18280_FreqGain[ulIndex].Frequency = iTDA18280_FreqPlan[ulIndex];
            iTDA18280_FreqGain[ulIndex].Gain = 0;
            iTDA18280_FreqGain[ulIndex].GainAfterRejection = 0;
            iTDA18280_FreqGain[ulIndex].GainCorrected = 0;

            ulIndex += 1;
        }

        if (ulIndex >= 1)
        {
            pObj->TiltOutputParameters.uTDA18280_LastFreqIndex = ulIndex-1;
        }
        else
        {
            pObj->TiltOutputParameters.uTDA18280_LastFreqIndex = 0;
        }
        iTDA18280_FreqPlan[ulIndex] = 0x0;
        iTDA18280_FreqGain[ulIndex].Frequency = 0x0;

        /* Initialize TEQ Default input parameters */
        pObj->TiltInputParameters.TDA18280_DefaultTilt                = iDefaultTiltInputParameters.TDA18280_DefaultTilt;
        pObj->TiltInputParameters.bTDA18280_RejectionEnabled          = iDefaultTiltInputParameters.bTDA18280_RejectionEnabled;
        pObj->TiltInputParameters.uTDA18280_NbChannels                = iDefaultTiltInputParameters.uTDA18280_NbChannels;
        pObj->TiltInputParameters.uTDA18280_MinNbChannelsNotEmpty     = iDefaultTiltInputParameters.uTDA18280_MinNbChannelsNotEmpty;
        pObj->TiltInputParameters.uTDA18280_StepAverage               = iDefaultTiltInputParameters.uTDA18280_StepAverage;
        pObj->TiltInputParameters.uTDA18280_SlopeThreshold            = iDefaultTiltInputParameters.uTDA18280_SlopeThreshold;
        pObj->TiltInputParameters.uTDA18280_StepThreshold             = iDefaultTiltInputParameters.uTDA18280_StepThreshold;
        pObj->TiltInputParameters.uTDA18280_CorrelThreshold           = iDefaultTiltInputParameters.uTDA18280_CorrelThreshold;
        pObj->TiltInputParameters.uTDA18280_EmptyThreshold            = iDefaultTiltInputParameters.uTDA18280_EmptyThreshold;
        pObj->TiltInputParameters.uTDA18280_FrequencyPercentThreshold = iDefaultTiltInputParameters.uTDA18280_FrequencyPercentThreshold;
        pObj->TiltInputParameters.uTDA18280_StepPercentThreshold      = iDefaultTiltInputParameters.uTDA18280_StepPercentThreshold;
        pObj->TiltInputParameters.uTDA18280_StepFreqRange             = iDefaultTiltInputParameters.uTDA18280_StepFreqRange;
        pObj->TiltInputParameters.uTDA18280_Wait                      = iDefaultTiltInputParameters.uTDA18280_Wait;
        pObj->TiltInputParameters.eTDA18280_BandWidth                 = iDefaultTiltInputParameters.eTDA18280_BandWidth;
    }

    /* Measure the digital AGC Gain for all channels in tilt equalizer frequency plan */
    if(err == TM_OK)
    {
        err = iTDA18280_TEQMeasureIFGain(pObj);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18280_TEQMeasureIFGain(0x%08X, PowerNormalMode) failed.", tUnit));
    }

    /* not enough valid channel % nb specified to apply tilt algorithm -> no Tilt will be applied */
    if ((pObj->TiltOutputParameters.uTDA18280_LastFreqIndex - pObj->TiltOutputParameters.uTDA18280_NbEmptyChannel) < pObj->TiltInputParameters.uTDA18280_MinNbChannelsNotEmpty )
    {
        pObj->TiltOutputParameters.TDA18280_CalculatedTilt = tmTDA18280_TiltEqualizer0dB;
    }
    else
    {
        /* Detect rejection due to adjacent signals */
        if((err == TM_OK) && (pObj->TiltInputParameters.bTDA18280_RejectionEnabled == True ))
        {
            err = iTDA18280_TEQDetectRejection(pObj);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18280_TEQDetectRejection(0x%08X, PowerNormalMode) failed.", tUnit));
        }

        /* Detect block of channels */
        if(err == TM_OK)
        {
            err = iTDA18280_TEQDetectBlockOfChannel(pObj);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18280_TEQDetectBlockOfChannel(0x%08X, PowerNormalMode) failed.", tUnit));
        }

        /* if block of channels found -> no Tilt will be applied */
        if( pObj->TiltOutputParameters.bTDA18280_BlockOfChannelDetected == True )
        {
            pObj->TiltOutputParameters.TDA18280_CalculatedTilt = tmTDA18280_TiltEqualizer0dB;
        }
        else
        {
            /* Detect Analog/Digital step */
            if(err == TM_OK)
            {
                err = iTDA18280_TEQHandleAnalogDigitalStep(pObj);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18280_TEQHandleAnalogDigitalStep(0x%08X, PowerNormalMode) failed.", tUnit));
            }

            /* Least Square analysis */
            if(err == TM_OK)
            {
                err = iTDA18280_TEQLeastSquareAnalysis(pObj);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18280_TEQLeastSquareAnalysis(0x%08X, PowerNormalMode) failed.", tUnit));
            }

            /* Calculate T0^2 , will be used in the future TBD */
            if(err == TM_OK)
            {
                err = iTDA18280_TEQCalculateT0Square(pObj);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18280_TEQCalculateT0Square(0x%08X, PowerNormalMode) failed.", tUnit));
            }

            /* R^2 > Correlation Threshold */
            if( pObj->TiltOutputParameters.uTDA18280_RSquare > pObj->TiltInputParameters.uTDA18280_CorrelThreshold )
            {
                if( pObj->TiltOutputParameters.bTDA18280_StepDetected == False )
                {
                    /* To be refined */
                    err = iTDA18280_TEQGetClosestSlope(pObj);
                }
                else
                {
                    /* To be refined */
                    err = iTDA18280_TEQGetClosestSlope(pObj);
                }
            }

            /* R^2 < Correlation Threshold :  */
            else
            {
                /* Slope <= 0 */
                if (pObj->TiltOutputParameters.iTDA18280_Slope <= 0)
                {
                    pObj->TiltOutputParameters.TDA18280_CalculatedTilt = tmTDA18280_TiltEqualizer0dB;
                }
                /* Slope > 0 */
                else
                {
                    if (pObj->TiltOutputParameters.iTDA18280_Slope > pObj->TiltInputParameters.uTDA18280_SlopeThreshold)
                    {
                        pObj->TiltOutputParameters.TDA18280_CalculatedTilt = tmTDA18280_TiltEqualizer0dB;
                    }
                    else
                    {
                        pObj->TiltOutputParameters.TDA18280_CalculatedTilt = pObj->TiltInputParameters.TDA18280_DefaultTilt;
                    }
                }
            }
        }
    }

    /* fill Tilt Equalizer output parameters */
    pTiltOutputParameters->uTDA18280_MaxGain                = pObj->TiltOutputParameters.uTDA18280_MaxGain;
    pTiltOutputParameters->uTDA18280_FreqMaxGain            = pObj->TiltOutputParameters.uTDA18280_FreqMaxGain;
    pTiltOutputParameters->uTDA18280_MinGain                = pObj->TiltOutputParameters.uTDA18280_MinGain;
    pTiltOutputParameters->uTDA18280_FreqMinGain            = pObj->TiltOutputParameters.uTDA18280_FreqMinGain;
    pTiltOutputParameters->uTDA18280_NbEmptyChannel         = pObj->TiltOutputParameters.uTDA18280_NbEmptyChannel;
    pTiltOutputParameters->bTDA18280_BlockOfChannelDetected = pObj->TiltOutputParameters.bTDA18280_BlockOfChannelDetected;
    pTiltOutputParameters->bTDA18280_StepDetected           = pObj->TiltOutputParameters.bTDA18280_StepDetected;
    pTiltOutputParameters->uTDA18280_LastFreqIndex          = pObj->TiltOutputParameters.uTDA18280_LastFreqIndex;
    pTiltOutputParameters->bTDA18280_FirstTimeTEQHandle     = pObj->TiltOutputParameters.bTDA18280_FirstTimeTEQHandle;
    pTiltOutputParameters->uTDA18280_StepDetectedFreq       = pObj->TiltOutputParameters.uTDA18280_StepDetectedFreq;
    pTiltOutputParameters->iTDA18280_StepDetectedValue      = pObj->TiltOutputParameters.iTDA18280_StepDetectedValue;
    pTiltOutputParameters->iTDA18280_Slope                  = pObj->TiltOutputParameters.iTDA18280_Slope;
    pTiltOutputParameters->uTDA18280_RSquare                = pObj->TiltOutputParameters.uTDA18280_RSquare;
    pTiltOutputParameters->uTDA18280_T0Square               = pObj->TiltOutputParameters.uTDA18280_T0Square;
    pTiltOutputParameters->TDA18280_CalculatedTilt          = pObj->TiltOutputParameters.TDA18280_CalculatedTilt;

    pTiltOutputParameters->pFreqGain = iTDA18280_FreqGain;

    _MUTEX_RELEASE(TDA18280)

        return err;
}

/*=================================================================================================*/
/**
 \brief  Read Debug Ram of the device.

 \return TM_OK: Succeed.
 \return TDA18280_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
 \return TDA18280_ERR_BAD_PARAMETER: Parameters out of range.
 \return Other optional return codes of the functions registered 
 in DeviceOpenInstance API (I2C read errors, mutex...).

 \note   RAMsize should be <= 0x4000

 \sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18280_DeviceReadDebugRam(
                                 tmUnitSelect_t  tUnit,           /**< I: Unit number. */
                                 tmTDA18280_DebugRamMode_t eMode, /**< I: Debug Ram Read Mode. */
                                 UInt16          RAMsize,         /**< I: Size of the RAM. */
                                 Int16 *         pData            /**< O: Pointer to data. */
                                 )
{
    ptmTDA18280_Object_t   pObj = Null;
    tmErrorCode_t          err = TM_OK;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18280)

        tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, tUnit);

    if( err == TM_OK )
    {
        err = iTDA18280_DeviceReadDebugRam(pObj, eMode, RAMsize, pData);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18280_DeviceReadDebugRam(0x%08X) failed.", tUnit));
    }

    _MUTEX_RELEASE(TDA18280)

        return err;
}

/*=================================================================================================*/
/**
\brief  Write a register of the device.

\return TM_OK: Succeed.
\return TDA18280_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
\return TDA18280_ERR_BAD_PARAMETER: Parameters out of range.
\return Other optional return codes of the functions registered 
in DeviceOpenInstance API (I2C read errors, mutex...).

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18280_DeviceWriteRegister(
                                  tmUnitSelect_t              tUnit,      /* I: Unit number */
                                  UInt16   Base_Address, 
                                  UInt16   Address, 
#ifdef PRINT_I2C
                                  Char Str[50], 
#endif
                                  UInt16   uData      /* I: Data to write */
                                  )
{
    ptmTDA18280_Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18280)

        tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, tUnit);

    err = iTDA18280_I2CWriteRegister(pObj, 
        Base_Address, 
        Address, 
#ifdef PRINT_I2C
        Str, 
#endif
        uData);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18280_I2CWriteRegister(0x%08X) failed.", pObj->tUnitW));

    _MUTEX_RELEASE(TDA18280)

        return err;
}

/*=================================================================================================*/
/**
\brief  Read a register of the device.

\return TM_OK: Succeed.
\return TDA18280_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
\return TDA18280_ERR_BAD_PARAMETER: Parameters out of range.
\return Other optional return codes of the functions registered 
in DeviceOpenInstance API (I2C read errors, mutex...).

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18280_DeviceReadRegister(
                                 tmUnitSelect_t              tUnit,      /* I: Unit number */
                                 UInt16   Base_Address, 
                                 UInt16   Address, 
#ifdef PRINT_I2C
                                 Char Str[50], 
#endif

                                 UInt16*  puData     /* 0: Data to read */
                                 )
{
    ptmTDA18280_Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18280)

        tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, tUnit);

    err = iTDA18280_I2CReadRegister(pObj, 
        Base_Address, 
        Address, 
#ifdef PRINT_I2C
        Str, 
#endif
        1, 
        puData);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18280_I2CReadRegister(0x%08X) failed.", pObj->tUnitW));

    _MUTEX_RELEASE(TDA18280)

        return err;
}

/*=================================================================================================*/
/**
\brief  Get the temperature of TDA18280 component

\param  tUnit   Device unit number
\param  plTemp  Pointer to the device temperature in tenth of degree Celsius (value_Celsius*10)

\return TM_OK: Succeed.
\return TDA18280_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
\return TDA18280_ERR_BAD_PARAMETER: Parameters out of range.
\return Other optional return codes of the functions registered 
in DeviceOpenInstance API (I2C read errors, mutex...).

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t 
tmbslTDA18280_GetTemperature( 
    tmUnitSelect_t                  tUnit,    /**< I: Unit number */
    Int32                          *plTemp   /**< O: Temperature */
    )
{
    ptmTDA18280_Object_t  pObj = Null;
    tmErrorCode_t         err = TM_OK;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18280)

        tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, tUnit);

    if(err == TM_OK)
    {
        err = iTDA18280_GetTemperatureMeasurement( pObj, tmTDA18280_TemperatureSourceRFADC, plTemp );
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18280_GetTemperatureMeasurement(0x%08X) failed.", pObj->tUnitW));
    }

    _MUTEX_RELEASE(TDA18280)

    return err;
}

/*=================================================================================================*/
/**
\brief  Get CDP or OOB demodulator lock status

\param  tUnit Demodulator unit number
\param  peLockStatus Pointer to the the lock status (Searching, Locked or NotLocked)

\return TM_OK Succeed
\return TDA18280_ERR_NOT_INITIALIZED This unit is not initialised
\return error codes of the functions registered in tmbslTDA18280_Open
\return TDA18280_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA18280_Open)

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18280_PathGetDemodLockStatus
    (
    tmUnitSelect_t        tUnit,        /**< I: Unit number. */
    tmTDA18280_Path_t     tPathNumber,  /**< Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).*/
    tmbslFrontEndState_t *peLockStatus  /**< O: Demod lock status. */
    )
{
    ptmTDA18280_Object_t  pObj = Null;
    tmErrorCode_t         err = TDA18280_ERR_NOT_INITIALIZED;
    tmUnitSelect_t        tUnitCdp = 0;
    tmTDA18280_Demod_t    DemodIndex = tmTDA18280_Demod_Max;
    UInt8                 Clock_OK = 0;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18280)

        tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, tUnit);

    /* initialize to handle errors */
    if ((err == TM_OK) && (peLockStatus != Null))
    {
        *peLockStatus = tmbslFrontEndStateNotLocked;
    }
    else
    {
        err = TDA18280_ERR_BAD_PARAMETER;
    }

    if (err == TM_OK)
    {
        if ((pObj->curPowerState == tmTDA18280_PowerWBOnly) ||
            (pObj->curPowerState == tmTDA18280_PowerWBAndNB) ||
            (pObj->curPowerState == tmTDA18280_PowerNBOnly))
        {
            if (pObj->PathCurrentState[tPathNumber] == tmTDA18280_ON)
            {
                if (err == TM_OK)
                {
                    err = iTDA18280_GetDemodIndex(pObj, tPathNumber, &DemodIndex);
                }
                if (err == TM_OK)
                {
                    err = iTDA18280_PathDemodClockCheck(pObj, DemodIndex, &Clock_OK);
                }
                if (err == TM_OK)
                {
                    if (Clock_OK == 1)
                    {
                        if (DemodIndex <= tmTDA18280_Demod_CDP_7)
                        {
                            /* demodulator is CDP */
                            /* tunit is modified in order to indicate CDP path */
                            tUnitCdp = tUnit + ADDITIONNAL_INFO_VAL(DemodIndex);
                            err = tmbslHCDP_GetLockStatus(tUnitCdp, peLockStatus);
                        }
                        else if (DemodIndex == tmTDA18280_Demod_OOB)
                        {
                            /* demodulator is OOB */
                            err = tmbslOOBIp_GetLockStatus(pObj->tUnitW, peLockStatus);
                        }
                    }
                }
            }
        }
    }
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslTDA18280_PathGetDemodLockStatus(0x%08X) failed.", tUnit));

    _MUTEX_RELEASE(TDA18280)

    return err;
}

/*=================================================================================================*/
/**
\brief  Get CDP or OOB demodulator BER value

\param  tUnit         Demodulator unit number
\param  tPathNumber   Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).
\param  psBER         Pointer to BER value
\param  puUncors      Pointer to Uncor value

\return TM_OK Succeed
\return TDA18280_ERR_NOT_INITIALIZED This unit is not initialised
\return error codes of the functions registered in tmbslTDA18280_Open
\return TDA18280_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA18280_Open)

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18280_PathGetDemodBER
(
    tmUnitSelect_t        tUnit,        /**< I: Unit number. */
    tmTDA18280_Path_t     tPathNumber,  /**< I: Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).*/
    tmFrontEndFracNb32_t *psBER,        /**< O: Bit Error Ratio value. */
    UInt32               *puUncors      /**< O: Uncorrected packets value. */
)
{
    ptmTDA18280_Object_t  pObj = Null;
    tmErrorCode_t         err = TDA18280_ERR_NOT_INITIALIZED;
    tmUnitSelect_t        tUnitCdp = 0;
    tmTDA18280_Demod_t    DemodIndex = tmTDA18280_Demod_Max;
    UInt8                 Clock_OK = 0;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18280)

        tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, tUnit);

    /* initialize to handle errors */
    if ((err == TM_OK) && (psBER != Null) && (puUncors != Null))
    {
        psBER->lInteger = 0;
        psBER->uDivider = 1;
        *puUncors = 0;
    }
    else
    {
        err = TDA18280_ERR_BAD_PARAMETER;
    }

    if (err == TM_OK)
    {
        if ((pObj->curPowerState == tmTDA18280_PowerWBOnly) ||
            (pObj->curPowerState == tmTDA18280_PowerWBAndNB) ||
            (pObj->curPowerState == tmTDA18280_PowerNBOnly))
        {
            if (pObj->PathCurrentState[tPathNumber] == tmTDA18280_ON)
            {
                if (err == TM_OK)
                {
                    err = iTDA18280_GetDemodIndex(pObj, tPathNumber, &DemodIndex);
                }
                if (err == TM_OK)
                {
                    err = iTDA18280_PathDemodClockCheck(pObj, DemodIndex, &Clock_OK);
                }
                if (err == TM_OK)
                {
                    if (Clock_OK == 1)
                    {
                        if (DemodIndex <= tmTDA18280_Demod_CDP_7)
                        {
                            /* demodulator is CDP */
                            /* tunit is modified in order to indicate CDP path */
                            tUnitCdp = tUnit + ADDITIONNAL_INFO_VAL(DemodIndex);
                            err = tmbslHCDP_GetBER(tUnitCdp, psBER, puUncors);
                        }
                        else if (DemodIndex == tmTDA18280_Demod_OOB)
                        {
                            /* demodulator is OOB */
                            err = TDA18280_ERR_NOT_SUPPORTED;
                        }
                    }
                }
            }
        }
    }
    if (err != TDA18280_ERR_NOT_SUPPORTED) tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslTDA18280_PathGetDemodBER(0x%08X) failed.", tUnit));

    _MUTEX_RELEASE(TDA18280)

    return err;
}

/*=================================================================================================*/
/**
\brief  Get the BER computation window

\param  tUnit         Demodulator unit number
\param  tPathNumber   Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).
\param  peBERWindow   Pointer to the BER computation window

\return TM_OK Succeed
\return TDA18280_ERR_NOT_INITIALIZED This unit is not initialised
\return error codes of the functions registered in tmbslTDA18280_Open
\return TDA18280_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA18280_Open)

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18280_PathGetDemodBERWindow
(
    tmUnitSelect_t          tUnit,        /**< I: Unit number. */
    tmTDA18280_Path_t       tPathNumber,  /**< I: Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).*/
    tmTDA18280_BERWindow_t *peBERWindow   /**< O: Size of the BER computation window */
)
{
    ptmTDA18280_Object_t  pObj = Null;
    tmErrorCode_t         err = TDA18280_ERR_NOT_INITIALIZED;
    tmUnitSelect_t        tUnitCdp = 0;
    tmTDA18280_Demod_t    DemodIndex = tmTDA18280_Demod_Max;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18280)

        tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, tUnit);

    /* initialize to handle errors */
    if ((err == TM_OK) && (peBERWindow != Null))
    {
        *peBERWindow = tmTDA18280_HCDPBERWindowUnknown;
    }
    else
    {
        err = TDA18280_ERR_BAD_PARAMETER;
    }

    if (err == TM_OK)
    {
        err = iTDA18280_GetDemodIndex(pObj, tPathNumber, &DemodIndex);
    }

    if (err == TM_OK)
    {
        if (DemodIndex <= tmTDA18280_Demod_CDP_7)
        {
            /* demodulator is CDP */
            /* tunit is modified in order to indicate CDP path */
            tUnitCdp = tUnit + ADDITIONNAL_INFO_VAL(DemodIndex);
            err = tmbslHCDP_GetBERWindow(tUnitCdp, (HCDP_BERWindow_t*)peBERWindow);
        }
        else if (DemodIndex == tmTDA18280_Demod_OOB)
        {
            /* demodulator is OOB */
            err = TDA18280_ERR_NOT_SUPPORTED;
        }
    }

    if (err != TDA18280_ERR_NOT_SUPPORTED) tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslTDA18280_PathGetDemodBERWindow(0x%08X) failed.", tUnit));

    _MUTEX_RELEASE(TDA18280)

    return err;
}

/*=================================================================================================*/
/**
\brief  Get IF Offset value

\param  tUnit         Demodulator unit number
\param  tPathNumber   Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).
\param  pIFOffset     Pointer to IF Offset value (in Hz)

\return TM_OK Succeed
\return TDA18280_ERR_NOT_INITIALIZED This unit is not initialised
\return error codes of the functions registered in tmbslTDA18280_Open
\return TDA18280_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA18280_Open)

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18280_PathGetDemodIFOffset
(
    tmUnitSelect_t        tUnit,        /**< I: Unit number. */
    tmTDA18280_Path_t     tPathNumber,  /**< I: Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).*/
    Int32                *pIFOffset     /**< O: IF offset value (in Hz). */
)
{
    ptmTDA18280_Object_t  pObj = Null;
    tmErrorCode_t         err = TDA18280_ERR_NOT_INITIALIZED;
    tmUnitSelect_t        tUnitCdp = 0;
    tmTDA18280_Demod_t    DemodIndex = tmTDA18280_Demod_Max;
    UInt8                 Clock_OK = 0;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18280)

        tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, tUnit);

    /* initialize to handle errors */
    if ((err == TM_OK) && (pIFOffset != Null))
    {
        *pIFOffset = 0;
    }
    else
    {
        err = TDA18280_ERR_BAD_PARAMETER;
    }

    if (err == TM_OK)
    {
        if ((pObj->curPowerState == tmTDA18280_PowerWBOnly) ||
            (pObj->curPowerState == tmTDA18280_PowerWBAndNB) ||
            (pObj->curPowerState == tmTDA18280_PowerNBOnly))
        {
            if (pObj->PathCurrentState[tPathNumber] == tmTDA18280_ON)
            {
                if (err == TM_OK)
                {
                    err = iTDA18280_GetDemodIndex(pObj, tPathNumber, &DemodIndex);
                }
                if (err == TM_OK)
                {
                    err = iTDA18280_PathDemodClockCheck(pObj, DemodIndex, &Clock_OK);
                }
                if (err == TM_OK)
                {
                    if (Clock_OK == 1)
                    {
                        if (DemodIndex <= tmTDA18280_Demod_CDP_7)
                        {
                            /* demodulator is CDP */
                            /* tunit is modified in order to indicate CDP path */
                            tUnitCdp = tUnit + ADDITIONNAL_INFO_VAL(DemodIndex);
                            err = tmbslHCDP_GetFreqOffset(tUnitCdp, pIFOffset);
                            /* Frequency offset to be inverted with current settings of registers: */
                            /* -for WB: tda18280_reg_ddc_slow0_GLOBAL_CONFIG__spec_inv = 0 (same for slow1 to slow7) */
                            /* -for NB: tda18280_reg_aux_ddc_LPF_CONFIG__spec_inv = 1 */
                            *pIFOffset = -(*pIFOffset);
                        }
                        else if (DemodIndex == tmTDA18280_Demod_OOB)
                        {
                            /* demodulator is OOB */
                            err = tmbslOOBIp_GetIFOffset(pObj->tUnitW, pIFOffset);
                        }
                    }
                }
            }
        }
    }
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslTDA18280_PathGetDemodIFOffset(0x%08X) failed.", tUnit));

    _MUTEX_RELEASE(TDA18280)

    return err;
}

/*=================================================================================================*/
/**
\brief  Get Symbol Rate value

\param  tUnit         Demodulator unit number
\param  tPathNumber   Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).
\param  puSR          Pointer to Symbol Rate value

\return TM_OK Succeed
\return TDA18280_ERR_NOT_INITIALIZED This unit is not initialised
\return error codes of the functions registered in tmbslTDA18280_Open
\return TDA18280_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA18280_Open)

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18280_PathGetDemodSR
(
    tmUnitSelect_t        tUnit,        /**< I: Unit number. */
    tmTDA18280_Path_t     tPathNumber,  /**< I: Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).*/
    UInt32                *puSR         /**< O: Symbol Rate value. */
)
{
    ptmTDA18280_Object_t  pObj = Null;
    tmErrorCode_t         err = TDA18280_ERR_NOT_INITIALIZED;
    tmUnitSelect_t        tUnitCdp = 0;
    tmTDA18280_Demod_t    DemodIndex = tmTDA18280_Demod_Max;
    UInt8                 Clock_OK = 0;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18280)

        tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, tUnit);

    /* initialize to handle errors */
    if ((err == TM_OK) && (puSR != Null))
    {
        *puSR = 0;
    }
    else
    {
        err = TDA18280_ERR_BAD_PARAMETER;
    }

    if (err == TM_OK)
    {
        if ((pObj->curPowerState == tmTDA18280_PowerWBOnly) ||
            (pObj->curPowerState == tmTDA18280_PowerWBAndNB) ||
            (pObj->curPowerState == tmTDA18280_PowerNBOnly))
        {
            if (pObj->PathCurrentState[tPathNumber] == tmTDA18280_ON)
            {
                if (err == TM_OK)
                {
                    err = iTDA18280_GetDemodIndex(pObj, tPathNumber, &DemodIndex);
                }
                if (err == TM_OK)
                {
                    err = iTDA18280_PathDemodClockCheck(pObj, DemodIndex, &Clock_OK);
                }
                if (err == TM_OK)
                {
                    if (Clock_OK == 1)
                    {
                        if (DemodIndex <= tmTDA18280_Demod_CDP_7)
                        {
                            /* demodulator is CDP */
                            /* tunit is modified in order to indicate CDP path */
                            tUnitCdp = tUnit + ADDITIONNAL_INFO_VAL(DemodIndex);
                            err = tmbslHCDP_GetSR(tUnitCdp, puSR);
                        }
                        else if (DemodIndex == tmTDA18280_Demod_OOB)
                        {
                            /* demodulator is OOB */
                            err = tmbslOOBIp_GetSR(pObj->tUnitW, puSR);
                        }
                    }
                }
            }
        }
    }
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslTDA18280_PathGetDemodSR(0x%08X) failed.", tUnit));

    _MUTEX_RELEASE(TDA18280)

    return err;
}

/*=================================================================================================*/
/**
\brief  Get Spectral Inversion value

\param  tUnit         Demodulator unit number
\param  tPathNumber   Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).
\param  peSI          Pointer to Spectral Inversion value

\return TM_OK Succeed
\return TDA18280_ERR_NOT_INITIALIZED This unit is not initialised
\return error codes of the functions registered in tmbslTDA18280_Open
\return TDA18280_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA18280_Open)

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18280_PathGetDemodSI
(
    tmUnitSelect_t        tUnit,        /**< I: Unit number. */
    tmTDA18280_Path_t     tPathNumber,  /**< I: Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).*/
    tmFrontEndSpecInv_t   *peSI         /**< O: Spectral Inversion value. */
)
{
    ptmTDA18280_Object_t  pObj = Null;
    tmErrorCode_t         err = TDA18280_ERR_NOT_INITIALIZED;
    tmUnitSelect_t        tUnitCdp = 0;
    tmTDA18280_Demod_t    DemodIndex = tmTDA18280_Demod_Max;
    UInt8                 Clock_OK = 0;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18280)

        tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, tUnit);

    /* initialize to handle errors */
    if ((err == TM_OK) && (peSI != Null))
    {
        *peSI = (tmFrontEndSpecInv_t )0;
    }
    else
    {
        err = TDA18280_ERR_BAD_PARAMETER;
    }

    if (err == TM_OK)
    {
        if ((pObj->curPowerState == tmTDA18280_PowerWBOnly) ||
            (pObj->curPowerState == tmTDA18280_PowerWBAndNB) ||
            (pObj->curPowerState == tmTDA18280_PowerNBOnly))
        {
            if (pObj->PathCurrentState[tPathNumber] == tmTDA18280_ON)
            {
                if (err == TM_OK)
                {
                    err = iTDA18280_GetDemodIndex(pObj, tPathNumber, &DemodIndex);
                }
                if (err == TM_OK)
                {
                    err = iTDA18280_PathDemodClockCheck(pObj, DemodIndex, &Clock_OK);
                }
                if (err == TM_OK)
                {
                    if (Clock_OK == 1)
                    {
                        if (DemodIndex <= tmTDA18280_Demod_CDP_7)
                        {
                            /* demodulator is CDP */
                            /* tunit is modified in order to indicate CDP path */
                            tUnitCdp = tUnit + ADDITIONNAL_INFO_VAL(DemodIndex);
                            err = tmbslHCDP_GetSI(tUnitCdp, peSI);
                        }
                        else if (DemodIndex == tmTDA18280_Demod_OOB)
                        {
                            /* demodulator is OOB */
                            err = tmbslOOBIp_GetSI(pObj->tUnitW, peSI);
                        }
                    }
                }
            }
        }
    }
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslTDA18280_PathGetDemodSI(0x%08X) failed.", tUnit));

    _MUTEX_RELEASE(TDA18280)

    return err;
}

/*=================================================================================================*/
/**
\brief  Gets the actual interleaver mode value

\param  tUnit         Demodulator unit number
\param  tPathNumber   Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).
\param  peSI          Pointer to interleaver mode value

\return TM_OK Succeed
\return TDA18280_ERR_NOT_INITIALIZED This unit is not initialised
\return error codes of the functions registered in tmbslTDA18280_Open
\return TDA18280_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA18280_Open)

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18280_PathGetDemodInterleaverMode
(
     tmUnitSelect_t        tUnit,        /**< I: Unit number. */
     tmTDA18280_Path_t     tPathNumber,  /**< I: Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).*/
     tmTDA18280_InterleaverMode_t *peInterleaverMode  /**< O: Size of the BER computation window.*/
)
{
    ptmTDA18280_Object_t  pObj = Null;
    tmErrorCode_t         err = TDA18280_ERR_NOT_INITIALIZED;
    tmUnitSelect_t        tUnitCdp = 0;
    tmTDA18280_Demod_t    DemodIndex = tmTDA18280_Demod_Max;
    UInt8                 Clock_OK = 0;
    HCDP_InterleaverMode_t eHCDPInterleaverMode;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18280)

        tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, tUnit);

    /* initialize to handle errors */
    if ((err == TM_OK) && (peInterleaverMode != Null))
    {
        *peInterleaverMode = (tmTDA18280_InterleaverMode_t)0;
    }
    else
    {
        err = TDA18280_ERR_BAD_PARAMETER;
    }

    if (err == TM_OK)
    {
        if ((pObj->curPowerState == tmTDA18280_PowerWBOnly) ||
            (pObj->curPowerState == tmTDA18280_PowerWBAndNB) ||
            (pObj->curPowerState == tmTDA18280_PowerNBOnly))
        {
            if (pObj->PathCurrentState[tPathNumber] == tmTDA18280_ON)
            {
                if (err == TM_OK)
                {
                    err = iTDA18280_GetDemodIndex(pObj, tPathNumber, &DemodIndex);
                }
                if (err == TM_OK)
                {
                    err = iTDA18280_PathDemodClockCheck(pObj, DemodIndex, &Clock_OK);
                }
                if (err == TM_OK)
                {
                    if (Clock_OK == 1)
                    {
                        if (DemodIndex <= tmTDA18280_Demod_CDP_7)
                        {
                            /* demodulator is CDP */
                            /* tunit is modified in order to indicate CDP path */
                            tUnitCdp = tUnit + ADDITIONNAL_INFO_VAL(DemodIndex);
                            err = tmbslHCDP_GetActualInterleaverMode(tUnitCdp, &eHCDPInterleaverMode);
                        }
                    }
                }
            }
        }
    }
    if (err == TM_OK)
    {
        *peInterleaverMode = (tmTDA18280_InterleaverMode_t)eHCDPInterleaverMode;
    }

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslTDA18280_PathGetDemodSI(0x%08X) failed.", tUnit));

    _MUTEX_RELEASE(TDA18280)

        return err;
}

/*=================================================================================================*/
/**
\brief  Get the channel EsNo in dB (or SNR, Signal to Noise Ratio).

\param  tUnit         Demodulator unit number
\param  tPathNumber   Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).
\param  pEsno         Pointer to channel EsNo value

\return TM_OK Succeed
\return TDA18280_ERR_NOT_INITIALIZED This unit is not initialised
\return error codes of the functions registered in tmbslTDA18280_Open
\return TDA18280_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA18280_Open)

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18280_PathGetDemodChannelEsNo
(
    tmUnitSelect_t        tUnit,        /**< I: Unit number. */
    tmTDA18280_Path_t     tPathNumber,  /**< I: Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).*/
    tmFrontEndFracNb32_t *pEsno         /**< O: Demod channel EsNo. */
)
{
    ptmTDA18280_Object_t  pObj = Null;
    tmErrorCode_t         err = TDA18280_ERR_NOT_INITIALIZED;
    tmUnitSelect_t        tUnitCdp = 0;
    tmTDA18280_Demod_t    DemodIndex = tmTDA18280_Demod_Max;
    UInt8                 Clock_OK = 0;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18280)

        tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, tUnit);

    if (pEsno == Null)
    {
        err = TDA18280_ERR_NOT_INITIALIZED;
    }

    /* initialize to handle errors */
    if ((err == TM_OK) && (pEsno != Null))
    {
        pEsno->lInteger = 0;
        pEsno->uDivider = 1;
    }
    else
    {
        err = TDA18280_ERR_BAD_PARAMETER;
    }

    if (err == TM_OK)
    {
        if ((pObj->curPowerState == tmTDA18280_PowerWBOnly) ||
            (pObj->curPowerState == tmTDA18280_PowerWBAndNB) ||
            (pObj->curPowerState == tmTDA18280_PowerNBOnly))
        {
            if (pObj->PathCurrentState[tPathNumber] == tmTDA18280_ON)
            {
                if (err == TM_OK)
                {
                    err = iTDA18280_GetDemodIndex(pObj, tPathNumber, &DemodIndex);
                }
                if (err == TM_OK)
                {
                    err = iTDA18280_PathDemodClockCheck(pObj, DemodIndex, &Clock_OK);
                }
                if (err == TM_OK)
                {
                    if (Clock_OK == 1)
                    {
                        if (DemodIndex <= tmTDA18280_Demod_CDP_7)
                        {
                            /* demodulator is CDP */
                            /* tunit is modified in order to indicate CDP path */
                            tUnitCdp = tUnit + ADDITIONNAL_INFO_VAL(DemodIndex);
                            err = tmbslHCDP_GetChannelEsNo(tUnitCdp, pEsno);
                        }
                        else if (DemodIndex == tmTDA18280_Demod_OOB)
                        {
                            /* demodulator is OOB */
                            err = TDA18280_ERR_NOT_SUPPORTED;
                        }
                    }
                }
            }
        }
    }
    if (err != TDA18280_ERR_NOT_SUPPORTED) tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslTDA18280_PathGetDemodBER(0x%08X) failed.", tUnit));

    _MUTEX_RELEASE(TDA18280)

    return err;
}

/*=================================================================================================*/
/**
\brief  Gets the OOB Uncors

\param  tUnit         Out Of Band unit number
\param  puUncors      Pointer to Uncors value
\param  puCorrected   Pointer to Corrected value

\return TM_OK Succeed
\return TDA18280_ERR_NOT_INITIALIZED This unit is not initialised
\return error codes of the functions registered in tmbslTDA18280_Open
\return TDA18280_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA18280_Open)

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18280_DeviceGetOOBUncors
(
    tmUnitSelect_t            tUnit,        /**< I: Unit number. */
    UInt32                   *puUncors,     /**< O: OOB Uncors. */
    UInt32                   *puCorrected   /**< O: OOB Corrected. */
)
{
    ptmTDA18280_Object_t  pObj = Null;
    tmErrorCode_t         err = TDA18280_ERR_NOT_INITIALIZED;
    UInt8                 Clock_OK = 0;
    tmTDA18280_Path_t     PathIndex = tmTDA18280_Path_Max;
    tmTDA18280_Demod_t    DemodOOBIndex = tmTDA18280_Demod_OOB;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18280)

        tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, tUnit);

    /* initialize to handle errors */
    if (err == TM_OK)
    {
        if (!pObj->tFeature.bOOBSupported)
        {
            err = TDA18280_ERR_NOT_SUPPORTED;
        }
        else if ((puUncors != Null) && (puCorrected != Null))
        {
            *puUncors = 0;
            *puCorrected = 0;
        }
        else
        {
            err = TDA18280_ERR_BAD_PARAMETER;
        }
    }

    if (err == TM_OK)
    {
        if ((pObj->curPowerState == tmTDA18280_PowerWBOnly) ||
            (pObj->curPowerState == tmTDA18280_PowerWBAndNB) ||
            (pObj->curPowerState == tmTDA18280_PowerNBOnly))
        {
            if (err == TM_OK)
            {
                /* get the path index matching the OOB demod */
                err = iTDA18280_GetPathIndexFromDemod(pObj, DemodOOBIndex, &PathIndex);
            }
            if (err == TM_OK)
            {
                if ((PathIndex < tmTDA18280_Path_Max) && (pObj->PathCurrentState[PathIndex] == tmTDA18280_ON))
                {
                    if (err == TM_OK)
                    {
                        err = iTDA18280_PathDemodClockCheck(pObj, DemodOOBIndex, &Clock_OK);
                    }
                    if ((err == TM_OK) && (Clock_OK == 1))
                    {
                        err = tmbslOOBIp_GetUncors(pObj->tUnitW, puUncors, puCorrected);
                    }
                }
            }
        }
    }
    if (err != TDA18280_ERR_NOT_SUPPORTED) tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslTDA18280_DeviceGetOOBUncors(0x%08X) failed.", tUnit));

    _MUTEX_RELEASE(TDA18280)

    return err;
}

/*=================================================================================================*/
/**
\brief  Gets the OOB modulation error rate 

\param  tUnit         Demodulator unit number
\param  psMER         Pointer to OOB MER value

\return TM_OK Succeed
\return TDA18280_ERR_NOT_INITIALIZED This unit is not initialised
\return error codes of the functions registered in tmbslTDA18280_Open
\return TDA18280_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA18280_Open)

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18280_DeviceGetOOBMER
(
 tmUnitSelect_t            tUnit,        /**< I: Unit number. */
 tmFrontEndFracNb32_t     *psMER         /**< O: OOB MER. */
)
{
    ptmTDA18280_Object_t  pObj = Null;
    tmErrorCode_t         err = TDA18280_ERR_NOT_INITIALIZED;
    UInt8                 Clock_OK = 0;
    tmTDA18280_Path_t     PathIndex = tmTDA18280_Path_Max;
    tmTDA18280_Demod_t    DemodOOBIndex = tmTDA18280_Demod_OOB;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18280)

        tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, tUnit);

    /* initialize to handle errors */
    if (err == TM_OK)
    {
        if (!pObj->tFeature.bOOBSupported)
        {
            err = TDA18280_ERR_NOT_SUPPORTED;
        }
        else if (psMER != Null)
        {
            psMER->lInteger = 0;
            psMER->uDivider = 1;
        }
        else
        {
            err = TDA18280_ERR_BAD_PARAMETER;
        }
    }

    if (err == TM_OK)
    {
        if ((pObj->curPowerState == tmTDA18280_PowerWBOnly) ||
            (pObj->curPowerState == tmTDA18280_PowerWBAndNB) ||
            (pObj->curPowerState == tmTDA18280_PowerNBOnly))
        {
            if (err == TM_OK)
            {
                /* get the path index matching the OOB demod */
                err = iTDA18280_GetPathIndexFromDemod(pObj, DemodOOBIndex, &PathIndex);
            }
            if (err == TM_OK)
            {
                if ((PathIndex < tmTDA18280_Path_Max) && (pObj->PathCurrentState[PathIndex] == tmTDA18280_ON))
                {
                    if (err == TM_OK)
                    {
                        err = iTDA18280_PathDemodClockCheck(pObj, DemodOOBIndex, &Clock_OK);
                    }
                    if ((err == TM_OK) && (Clock_OK == 1))
                    {
                        err = tmbslOOBIp_GetMER(pObj->tUnitW, psMER);
                    }
                }
            }
        }
    }
    if (err != TDA18280_ERR_NOT_SUPPORTED) tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslTDA18280_DeviceGetOOBMER(0x%08X) failed.", tUnit));

    _MUTEX_RELEASE(TDA18280)

    return err;
}

/*=================================================================================================*/
/**
\brief  Clear uncor

\param  tUnit         Demodulator unit number
\param  tPathNumber   Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).

\return TM_OK Succeed
\return TDA18280_ERR_NOT_INITIALIZED This unit is not initialised
\return error codes of the functions registered in tmbslTDA18280_Open
\return TDA18280_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA18280_Open)

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18280_PathClearDemodUncor
(
    tmUnitSelect_t        tUnit,        /**< I: Unit number. */
    tmTDA18280_Path_t     tPathNumber   /**< I: Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).*/
)
{
    ptmTDA18280_Object_t  pObj = Null;
    tmErrorCode_t         err = TDA18280_ERR_NOT_INITIALIZED;
    tmUnitSelect_t        tUnitCdp = 0;
    tmTDA18280_Demod_t    DemodIndex = tmTDA18280_Demod_Max;
    UInt8                 Clock_OK = 0;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18280)

        tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, tUnit);

    if (err == TM_OK)
    {
        if ((pObj->curPowerState == tmTDA18280_PowerWBOnly) ||
            (pObj->curPowerState == tmTDA18280_PowerWBAndNB) ||
            (pObj->curPowerState == tmTDA18280_PowerNBOnly))
        {
            if (pObj->PathCurrentState[tPathNumber] == tmTDA18280_ON)
            {
                if (err == TM_OK)
                {
                    err = iTDA18280_GetDemodIndex(pObj, tPathNumber, &DemodIndex);
                }
                if (err == TM_OK)
                {
                    err = iTDA18280_PathDemodClockCheck(pObj, DemodIndex, &Clock_OK);
                }
                if (err == TM_OK)
                {
                    if (Clock_OK == 1)
                    {
                        if (DemodIndex <= tmTDA18280_Demod_CDP_7)
                        {
                            /* demodulator is CDP */
                            /* tunit is modified in order to indicate CDP path */
                            tUnitCdp = tUnit + ADDITIONNAL_INFO_VAL(DemodIndex);
                            err = tmbslHCDP_ClearUncor(tUnitCdp);
                        }
                        else if (DemodIndex == tmTDA18280_Demod_OOB)
                        {
                            /* demodulator is OOB */
                            err = tmbslOOBIp_ClearUncors(pObj->tUnitW);
                        }
                    }
                }
            }
        }
    }
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslTDA18280_PathClearDemodUncor(0x%08X) failed.", tUnit));

    _MUTEX_RELEASE(TDA18280)

    return err;
}

/*=================================================================================================*/
/**
\brief  Get driver features

\param  tUnit        Demodulator unit number
\param  psFeature    TDA18280 Features

\return TM_OK Succeed
\return TDA18280_ERR_NOT_INITIALIZED This unit is not initialised
\return error codes of the functions registered in tmbslTDA18280_Open
\return TDA18280_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA18280_Open)

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18280_GetFeatures
    (
    tmUnitSelect_t           tUnit,       /**< I: Unit number. */
    tmTDA18280_Feature_t    *ptFeature    /**< O: TDA18280 Features.*/
    )
{
    ptmTDA18280_Object_t  pObj = Null;
    tmErrorCode_t         err = TDA18280_ERR_NOT_INITIALIZED;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18280)

        tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, tUnit);

    if (err == TM_OK)
    {
        /* copy pObj into output pointer */
        *ptFeature = pObj->tFeature;

        if (pObj->eHwState != tmTDA18280_HwState_InitDone)
        {
            err = TDA18280_ERR_NOT_INITIALIZED;
        }
    }

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslTDA18280_GetFeatures(0x%08X) failed.", tUnit));

    _MUTEX_RELEASE(TDA18280)

    return err;
}

/*=================================================================================================*/
/**
\brief  Set OOB input mode

\param  tUnit        Demodulator unit number
\param  eOobInput    OOB input (none, Wide Band or Narrow Band)

\return TM_OK Succeed
\return TDA18280_ERR_NOT_INITIALIZED This unit is not initialised
\return error codes of the functions registered in tmbslTDA18280_Open
\return TDA18280_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA18280_Open)

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18280_DeviceSetOOBInput
    ( 
    tmUnitSelect_t        tUnit, 
    tmTDA18280_OobInput_t eOobInput
    )
{
    ptmTDA18280_Object_t  pObj = Null;
    tmErrorCode_t         err = TDA18280_ERR_NOT_INITIALIZED;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18280)

        tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, tUnit);

    /* check feature */
    if (err == TM_OK && !pObj->tFeature.bOOBSupported)
    {
        err = TDA18280_ERR_NOT_SUPPORTED;
    }

    if (err == TM_OK)
    {
        tmTDA18280_Path_t CurrentOobPath = pObj->eOOBInput;
        tmTDA18280_Path_t NextOobPath = tmTDA18280_Path_Max;

        switch( eOobInput )
        {
        case tmTDA18280_OobInput_None:
            NextOobPath = tmTDA18280_Path_Max;
            break;
        case tmTDA18280_OobInput_WB:
            NextOobPath = TDA18280_OOB_DDC_PATH;
            break;
        case tmTDA18280_OobInput_NB:
            NextOobPath = tmTDA18280_Path_Aux;
            break;
        }

        if( CurrentOobPath != NextOobPath )
        {
            /* switch off current path */
            if(CurrentOobPath != tmTDA18280_Path_Max && pObj->PathCurrentState[CurrentOobPath] == tmTDA18280_ON)
            {
                if(err == TM_OK)
                {
                    err = iTDA18280_PathStop(pObj, CurrentOobPath);
                }
            }

            if(NextOobPath != tmTDA18280_Path_Max&& pObj->PathCurrentState[NextOobPath] == tmTDA18280_ON)
            {
                if(err == TM_OK)
                {
                    err = iTDA18280_PathStop(pObj, NextOobPath);
                }
            }

            pObj->eOOBInput = NextOobPath;
        }
    }

    if (err != TDA18280_ERR_NOT_SUPPORTED) tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslTDA18280_DeviceSetOOBInput(0x%08X) failed.", tUnit));

    _MUTEX_RELEASE(TDA18280)

    return err;
}

/*=================================================================================================*/
/**
\brief  Get OOB input mode

\param  tUnit        Demodulator unit number
\param  peOobInput   Pointer to OOB input (none, Wide Band or Narrow Band)

\return TM_OK Succeed
\return TDA18280_ERR_NOT_INITIALIZED This unit is not initialised
\return error codes of the functions registered in tmbslTDA18280_Open
\return TDA18280_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA18280_Open)

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18280_DeviceGetOOBInput
    ( 
    tmUnitSelect_t        tUnit, 
    tmTDA18280_OobInput_t *peOobInput
    )
{
    ptmTDA18280_Object_t  pObj = Null;
    tmErrorCode_t         err = TDA18280_ERR_NOT_INITIALIZED;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18280)

        tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, tUnit);

    /* initialize to handle errors or OOB not supported */
    *peOobInput = tmTDA18280_OobInput_None;

    /* Test parameter(s) */
    if (err == TM_OK)
    {
        if(peOobInput == Null)
        {
            err = TDA18280_ERR_BAD_PARAMETER;
        }
        else if (!pObj->tFeature.bOOBSupported)
        {
            err = TDA18280_ERR_NOT_SUPPORTED;
        }
        else
        {
            if (pObj->eOOBInput == tmTDA18280_Path_Max)
            {
                *peOobInput = tmTDA18280_OobInput_None;
            }
            else if (pObj->eOOBInput == tmTDA18280_Path_Aux)
            {
                *peOobInput = tmTDA18280_OobInput_NB;
            }
            else
            {
                *peOobInput = tmTDA18280_OobInput_WB;
            }
        }
    }

    if (err != TDA18280_ERR_NOT_SUPPORTED) tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslTDA18280_DeviceGetOOBInput(0x%08X) failed.", tUnit));

    _MUTEX_RELEASE(TDA18280)

    return err;
}

/*=================================================================================================*/
/**
\brief  Set the TSMF mode.

\param  tUnit         Demodulator unit number
\param  tPathNumber   Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).
\param  eTsmfMode     TSMF mode (Disable, Forced or Auto)

\return TM_OK Succeed
\return TDA18280_ERR_NOT_INITIALIZED This unit is not initialised
\return error codes of the functions registered in tmbslTDA18280_Open
\return TDA18280_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA18280_Open)

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18280_PathSetDemodTsmfMode
(
 tmUnitSelect_t        tUnit,        /**< I: Unit number. */
 tmTDA18280_Path_t     tPathNumber,  /**< I: Path selector. */
 tmTDA18280_TsmfMode_t eTsmfMode     /**< I: TSMF mode. */
 )
{
    ptmTDA18280_Object_t  pObj = Null;
    tmErrorCode_t         err = TM_OK;
    tmTDA18280_Demod_t    DemodIndex = tmTDA18280_Demod_Max;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18280)

        tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, tUnit);

    if( eTsmfMode < tmTDA18280_TsmfModeInvalid)
    {
        if (err == TM_OK)
        {
            if ((pObj->curPowerState == tmTDA18280_PowerWBOnly) ||
                (pObj->curPowerState == tmTDA18280_PowerWBAndNB) ||
                (pObj->curPowerState == tmTDA18280_PowerNBOnly))
            {
                if (pObj->PathCurrentState[tPathNumber] == tmTDA18280_ON)
                {
                    if (err == TM_OK)
                    {
                        err = iTDA18280_GetDemodIndex(pObj, tPathNumber, &DemodIndex);
                    }
                    if (err == TM_OK)
                    {
                        err = iTDA18280_SetDemodTsmfMode(pObj, DemodIndex, eTsmfMode);
                    }
                }
            }
        }
    }
    else
    {
        err = TDA18280_ERR_BAD_PARAMETER;
    }

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslTDA18280_PathSetDemodTsmfMode(0x%08X) failed.", tUnit));

    _MUTEX_RELEASE(TDA18280)

        return err;
}

/*=================================================================================================*/
/**
\brief  Get the TSMF mode.

\param  tUnit         Demodulator unit number
\param  tPathNumber   Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).
\param  peTsmfMode    Pointer to TSMF mode (Disable, Forced or Auto)

\return TM_OK Succeed
\return TDA18280_ERR_NOT_INITIALIZED This unit is not initialised
\return error codes of the functions registered in tmbslTDA18280_Open
\return TDA18280_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA18280_Open)

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18280_PathGetDemodTsmfMode
(
 tmUnitSelect_t         tUnit,        /**< I: Unit number. */
 tmTDA18280_Path_t      tPathNumber,  /**< I: Path selector. */
 tmTDA18280_TsmfMode_t* peTsmfMode    /**< O: Pointer to TSMF mode. */
 )
{
    ptmTDA18280_Object_t  pObj = Null;
    tmErrorCode_t         err = TDA18280_ERR_NOT_INITIALIZED;
    tmTDA18280_Demod_t    DemodIndex = tmTDA18280_Demod_Max;
    tmTDA18280_TsmfMode_t eTsmfMode = tmTDA18280_TsmfModeInvalid;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18280)

        tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, tUnit);

    /* initialize to handle errors */
    if ((err == TM_OK) && (peTsmfMode != Null))
    {
        *peTsmfMode = tmTDA18280_TsmfModeDisabled;
    }
    else
    {
        err = TDA18280_ERR_BAD_PARAMETER;
    }

    if (err == TM_OK)
    {
        if ((pObj->curPowerState == tmTDA18280_PowerWBOnly) ||
            (pObj->curPowerState == tmTDA18280_PowerWBAndNB) ||
            (pObj->curPowerState == tmTDA18280_PowerNBOnly))
        {
            if (pObj->PathCurrentState[tPathNumber] == tmTDA18280_ON)
            {
                if (err == TM_OK)
                {
                    err = iTDA18280_GetDemodIndex(pObj, tPathNumber, &DemodIndex);
                }
                if (err == TM_OK)
                {
                    err = iTDA18280_GetDemodTsmfMode(pObj, DemodIndex, &eTsmfMode);
                }
            }
        }
    }
    if (err == TM_OK)
    {
        *peTsmfMode = eTsmfMode;
    }

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslTDA18280_PathGetDemodTsmfMode(0x%08X) failed.", tUnit));

    _MUTEX_RELEASE(TDA18280)

        return err;
}

/*=================================================================================================*/
/**
\brief  Get the TSMF status.

\param  tUnit         Demodulator unit number
\param  tPathNumber   Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).
\param  psTsmfStatus  Pointer to TSMF status.

\return TM_OK Succeed
\return TDA18280_ERR_NOT_INITIALIZED This unit is not initialised
\return error codes of the functions registered in tmbslTDA18280_Open
\return TDA18280_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA18280_Open)

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18280_PathGetDemodTsmfStatus
(
 tmUnitSelect_t         tUnit,        /**< I: Unit number. */
 tmTDA18280_Path_t      tPathNumber,  /**< I: Path selector. */
 tmTDA18280_TsmfStatus_t* psTsmfStatus  /**< O: Pointer to TSMF status. */
 )
{
    ptmTDA18280_Object_t  pObj = Null;
    tmErrorCode_t         err = TDA18280_ERR_NOT_INITIALIZED;
    tmTDA18280_Demod_t    DemodIndex = tmTDA18280_Demod_Max;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18280)

        tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, tUnit);

    if ((err != TM_OK) || (psTsmfStatus == Null))
    {
        err = TDA18280_ERR_BAD_PARAMETER;
    }

    if (err == TM_OK)
    {
        if ((pObj->curPowerState == tmTDA18280_PowerWBOnly) ||
            (pObj->curPowerState == tmTDA18280_PowerWBAndNB) ||
            (pObj->curPowerState == tmTDA18280_PowerNBOnly))
        {
            if (pObj->PathCurrentState[tPathNumber] == tmTDA18280_ON)
            {
                if (err == TM_OK)
                {
                    err = iTDA18280_GetDemodIndex(pObj, tPathNumber, &DemodIndex);
                }
                if (err == TM_OK)
                {
                    err = iTDA18280_GetDemodTsmfStatus(pObj, DemodIndex, psTsmfStatus);
                }
            }
        }
    }

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslTDA18280_PathGetDemodTsmfStatus(0x%08X) failed.", tUnit));

    _MUTEX_RELEASE(TDA18280)

        return err;
}

/*=================================================================================================*/
/**
\brief  Get the available TS IDs

\param  tUnit                  Demodulator unit number
\param  tPathNumber            Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).
\param  psTsmfAvailableTs  Pointer to available TS IDs.

\return TM_OK Succeed
\return TDA18280_ERR_NOT_INITIALIZED This unit is not initialised
\return error codes of the functions registered in tmbslTDA18280_Open
\return TDA18280_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA18280_Open)

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18280_PathGetDemodTsmfAvailableTsId
(
 tmUnitSelect_t         tUnit,        /**< I: Unit number. */
 tmTDA18280_Path_t      tPathNumber,  /**< I: Path selector. */
 tmTDA18280_TsmfAvailableTs_t* psTsmfAvailableTs  /**< O: Pointer to available TS IDs. */
 )
{
    ptmTDA18280_Object_t  pObj = Null;
    tmErrorCode_t         err = TDA18280_ERR_NOT_INITIALIZED;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18280)

        tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, tUnit);

    /* TBF */

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslTDA18280_PathGetDemodTsmfAvailableTsId(0x%08X) failed.", tUnit));

    _MUTEX_RELEASE(TDA18280)

        return err;
}


/*=================================================================================================*/
/**
\brief  Select one of the available TS IDs.

\param  tUnit         Demodulator unit number
\param  tPathNumber   Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).
\param  uTsId         Index of the selected TsId (between 1 and 15)

\return TM_OK Succeed
\return TDA18280_ERR_NOT_INITIALIZED This unit is not initialised
\return error codes of the functions registered in tmbslTDA18280_Open
\return TDA18280_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA18280_Open)

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18280_PathSelectDemodTsmfTsId
(
 tmUnitSelect_t        tUnit,        /**< I: Unit number. */
 tmTDA18280_Path_t     tPathNumber,  /**< I: Path selector. */
 UInt32                uTsId         /**< I: TS Id. */
 )
{
    ptmTDA18280_Object_t  pObj = Null;
    tmErrorCode_t         err = TDA18280_ERR_NOT_INITIALIZED;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18280)

        tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, tUnit);

    /* TBF */

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslTDA18280_PathSelectDemodTsmfTsId(0x%08X) failed.", tUnit));

    _MUTEX_RELEASE(TDA18280)

        return err;
}

/*=================================================================================================*/
/**
\brief  Set Extend Symbol Rate Range

\param  tUnit         Demodulator unit number
\param  tPathNumber   Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).
\param  eMode         Extended Symbol Rate Range

\return TM_OK Succeed
\return TDA18280_ERR_NOT_INITIALIZED This unit is not initialised
\return error codes of the functions registered in tmbslTDA18280_Open
\return TDA18280_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA18280_Open)

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18280_PathSetDemodExtendSymbolRateRange
(
    tmUnitSelect_t                   tUnit,        /**< I: Unit number. */
    tmTDA18280_Path_t                tPathNumber,  /**< I: Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).*/
    TDA18280_ExtendSymbolRateMode_t  eMode         /**< I: Extended Symbol Rate Range. */
)
{
    ptmTDA18280_Object_t  pObj = Null;
    tmErrorCode_t         err = TDA18280_ERR_NOT_INITIALIZED;
    tmUnitSelect_t        tUnitCdp = 0;
    tmTDA18280_Demod_t    CdpIndex = tmTDA18280_Demod_Max;
    HCDP_ExtendSymbolRateMode_t eHcdpMode;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18280)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslTDA18280_PathSetDemodExtendSymbolRateRange(0x%08X)", tUnit);

    if( err == TM_OK )
    {
        err = iTDA18280_GetDemodIndex(pObj, tPathNumber, &CdpIndex);
    }

    /* check driver state */
    if (err == TM_OK)
    {
        /* tunit is modified in order to indicate CDP path */
        tUnitCdp = tUnit + ADDITIONNAL_INFO_VAL(CdpIndex);

        if (err == TM_OK)
        {
            switch (eMode)
            {
            case TDA18280_ExtendSymbolRateModeEnable700ppm:
                eHcdpMode = HCDP_ExtendSymbolRateModeEnable700ppm;
                break;
            case TDA18280_ExtendSymbolRateModeEnable1500ppm:
                eHcdpMode = HCDP_ExtendSymbolRateModeEnable1500ppm;
                break;
            case TDA18280_ExtendSymbolRateModeDisable:
                eHcdpMode = HCDP_ExtendSymbolRateModeDisable;
                break;
            default:
                err = TDA18280_ERR_BAD_PARAMETER;
                break;
            }
        }
        if (err == TM_OK)
        {
            err = tmbslHCDP_SetExtendSymbolRateRange(tUnitCdp, eHcdpMode);
        }
        if (err == TM_OK)
        {
            pObj->PathCfgFixed[CdpIndex].eExtendSRMode = eMode;
        }
    }

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslTDA18280_PathSetDemodExtendSymbolRateRange(0x%08X) failed.", tUnit));
    
    _MUTEX_RELEASE(TDA18280)

    return err;
}

/*=================================================================================================*/
/**
\brief  Get Extend Symbol Rate Range

\param  tUnit         Demodulator unit number
\param  tPathNumber   Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).
\param  peMode        Extended Symbol Rate Range

\return TM_OK Succeed
\return TDA18280_ERR_NOT_INITIALIZED This unit is not initialised
\return error codes of the functions registered in tmbslTDA18280_Open
\return TDA18280_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA18280_Open)

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18280_PathGetDemodExtendSymbolRateRange
(
    tmUnitSelect_t                   tUnit,          /**< I: Unit number. */
    tmTDA18280_Path_t                tPathNumber,    /**< I: Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).*/
    TDA18280_ExtendSymbolRateMode_t *peMode          /**< 0: Extended Symbol Rate Range. */
)
{
    ptmTDA18280_Object_t  pObj = Null;
    tmErrorCode_t         err = TDA18280_ERR_NOT_INITIALIZED;
    tmTDA18280_Demod_t    CdpIndex = tmTDA18280_Demod_Max;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18280)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslTDA18280_PathGetDemodExtendSymbolRateRange(0x%08X)", tUnit);

    if( err == TM_OK )
    {
        err = iTDA18280_GetDemodIndex(pObj, tPathNumber, &CdpIndex);
    }

    if (err == TM_OK)
    {
        *peMode = pObj->PathCfgFixed[CdpIndex].eExtendSRMode;
    }

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslTDA18280_PathGetDemodExtendSymbolRateRange(0x%08X) failed.", tUnit));
    
    _MUTEX_RELEASE(TDA18280)

    return err;
}

/*=================================================================================================*/
/**
\brief  Change some settings after demod lock

\param  tUnit         Demodulator unit number
\param  tPathNumber   Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).

\return TM_OK Succeed
\return TDA18280_ERR_NOT_INITIALIZED This unit is not initialised
\return error codes of the functions registered in tmbslTDA18280_Open
\return TDA18280_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA18280_Open)

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
    tmbslTDA18280_PathSetAfterDemodLock
    (
    tmUnitSelect_t                   tUnit,        /**< I: Unit number. */
    tmTDA18280_Path_t                tPathNumber   /**< I: Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).*/
    )
{
    ptmTDA18280_Object_t  pObj = Null;
    tmErrorCode_t         err = TDA18280_ERR_NOT_INITIALIZED;
    tmTDA18280_Demod_t    DemodIndex = tmTDA18280_Demod_Max;
    tmUnitSelect_t             tUnitCdp = 0;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18280)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslTDA18280_PathSetAfterDemodLock(0x%08X)", tUnit);

    if( err == TM_OK )
    {
        err = iTDA18280_GetDemodIndex(pObj, tPathNumber, &DemodIndex);
    }

    if (err == TM_OK && DemodIndex <= tmTDA18280_Demod_CDP_7)
    {
        /* demodulator is CDP */
        /* tunit is modified in order to indicate CDP path */
        tUnitCdp = tUnit + ADDITIONNAL_INFO_VAL(DemodIndex);
        err = tmbslHCDP_SetAfterLock(tUnitCdp);
    }

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslTDA18280_PathSetAfterDemodLock(0x%08X) failed.", tUnit));
    
    _MUTEX_RELEASE(TDA18280)

    return err;
}

/*============================================================================*/
/* Internal functions:                                                        */
/*============================================================================*/
/*============================================================================*/
/* FUNCTION:    iTDA18280_DevicePowerStateSet                                 */
/*                                                                            */
/* DESCRIPTION: Sets the power state.                                         */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_DevicePowerStateSet(
                              ptmTDA18280_Object_t       pObj,       /* I: Driver object */
                              tmTDA18280_PowerState_t    powerState  /* I: Power state of TDA18280 */
                              )
{
    tmErrorCode_t   err = TM_OK;
    UInt8 PathIndex;
    tmTDA18280_PowerState_t curPowerState;
    tmTDA18280_PathCfg_t PathCfg;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    /* store current power state in case of error */
    curPowerState = pObj->curPowerState;

    /* Store low-level power state in driver instance */
    pObj->curPowerState = powerState;

    switch(powerState)
    {
    case tmTDA18280_PowerWBOnly:
        tmDBGPRINTEx(DEBUGLVL_INOUT, "%s: WBOnly", __FUNCTION__);

        if (curPowerState == tmTDA18280_PowerStandby)
        {
            /* transition Standby -> WBOnly */
            tmDBGPRINTEx(DEBUGLVL_INOUT, "%s: Standby -> WBOnly", __FUNCTION__);

            if (err == TM_OK)
            {
                err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_cgu_PLL_CONTROL__synonmain, 1);
            }

            /* Init LO */
            if (err == TM_OK)
            {
                err = iTDA18280_BlockLOChainConfigAndStartup(pObj, pObj->PllCfg.LOChainMode);
            }

            /* Activate CGU and reset */
            if (err == TM_OK)
            {
                err = iTDA18280_BlockCGUAndResetInitWBNB(pObj);
            }
            if (err == TM_OK)
            {
                err = iTDA18280_BlockCGUAndResetStartupWB(pObj);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18280_BlockCGUAndResetStartupWB(0x%08X) failed.", pObj->tUnitW));
            }

            /* Activate ADC */
#ifndef SIMU_RTL
            if (err == TM_OK)
            {
                err = iTDA18280_BlockADCStartup(pObj);
            }
            if (err == TM_OK)
            {
                err = iTDA18280_BlockADCCalibration(pObj);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18280_BlockADCCalibration(0x%08X) failed.", pObj->tUnitW));
            }
#endif

            /* Activate RF AGC if config is enabled */
            if (err == TM_OK)
            {
                if (pObj->RfAgcCfg.eRfAgcCtrl == tmTDA18280_RfAgcCtrl_Enabled)
                {
                    err = iTDA18280_EnableWbRfAgc(pObj, True);
                }
            }
        }
        else if (curPowerState == tmTDA18280_PowerWBOnly)
        {
            /* transition WBOnly -> WBOnly */
            tmDBGPRINTEx(DEBUGLVL_INOUT, "%s: WBOnly -> WBOnly", __FUNCTION__);
            /* nothing to do */
        }
        else if (curPowerState == tmTDA18280_PowerWBAndNB)
        {
            /* transition WBAndNB -> WBOnly */
            tmDBGPRINTEx(DEBUGLVL_INOUT, "%s: WBAndNB -> WBOnly", __FUNCTION__);

            /* switch off NB path */
            if (err == TM_OK)
            {
                PathCfg.tPathStatus = tmTDA18280_OFF;
                err = iTDA18280_PathSet(pObj, tmTDA18280_Path_Aux, &PathCfg, tmTDA18280_PathSetAGCMode_Normal);
            }
        }
        else if (curPowerState == tmTDA18280_PowerNBOnly)
        {
            /* transition NBOnly -> WBOnly */
            tmDBGPRINTEx(DEBUGLVL_INOUT, "%s: NBOnly -> WBOnly", __FUNCTION__);

            /* switch off NB path */
            if (err == TM_OK)
            {
                PathCfg.tPathStatus = tmTDA18280_OFF;
                err = iTDA18280_PathSet(pObj, tmTDA18280_Path_Aux, &PathCfg, tmTDA18280_PathSetAGCMode_Normal);
            }

            /* Activate CGU and reset */
            if (err == TM_OK)
            {
                err = iTDA18280_BlockCGUAndResetStartupWB(pObj);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18280_BlockCGUAndResetStartupWB(0x%08X) failed.", pObj->tUnitW));
            }

            /* Activate ADC */
#ifndef SIMU_RTL
            if (err == TM_OK)
            {
                err = iTDA18280_BlockADCStartup(pObj);
            }
            if (err == TM_OK)
            {
                err = iTDA18280_BlockADCCalibration(pObj);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18280_BlockADCCalibration(0x%08X) failed.", pObj->tUnitW));
            }
#endif

            /* Activate RF AGC if config is enabled */
            if (err == TM_OK)
            {
                if (pObj->RfAgcCfg.eRfAgcCtrl == tmTDA18280_RfAgcCtrl_Enabled)
                {
                    err = iTDA18280_EnableWbRfAgc(pObj, True);
                }
            }
        }
        break;

    case tmTDA18280_PowerWBAndNB:
        tmDBGPRINTEx(DEBUGLVL_INOUT, "%s: WBAndNB", __FUNCTION__);

        if (curPowerState == tmTDA18280_PowerStandby)
        {
            /* transition Standby -> WBAndNB */
            tmDBGPRINTEx(DEBUGLVL_INOUT, "%s: Standby -> WBAndNB", __FUNCTION__);

            if (err == TM_OK)
            {
                err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_cgu_PLL_CONTROL__synonmain, 1);
            }

            /* Init LO */
            if (err == TM_OK)
            {
                err = iTDA18280_BlockLOChainConfigAndStartup(pObj, pObj->PllCfg.LOChainMode);
            }

            /* Activate CGU and reset */
            if (err == TM_OK)
            {
                err = iTDA18280_BlockCGUAndResetInitWBNB(pObj);
            }
            if (err == TM_OK)
            {
                err = iTDA18280_BlockCGUAndResetStartupWB(pObj);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18280_BlockCGUAndResetStartupWB(0x%08X) failed.", pObj->tUnitW));
            }

            /* Activate ADC */
#ifndef SIMU_RTL
            if (err == TM_OK)
            {
                err = iTDA18280_BlockADCStartup(pObj);
            }
            if (err == TM_OK)
            {
                err = iTDA18280_BlockADCCalibration(pObj);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18280_BlockADCCalibration(0x%08X) failed.", pObj->tUnitW));
            }
#endif

            /* Activate RF AGC if config is enabled */
            if (err == TM_OK)
            {
                if (pObj->RfAgcCfg.eRfAgcCtrl == tmTDA18280_RfAgcCtrl_Enabled)
                {
                    err = iTDA18280_EnableWbRfAgc(pObj, True);
                }
            }
        }
        else if (curPowerState == tmTDA18280_PowerWBOnly)
        {
            /* transition WBOnly -> WBAndNB */
            tmDBGPRINTEx(DEBUGLVL_INOUT, "%s: WBOnly -> WBAndNB", __FUNCTION__);
            /* nothing to do */
        }
        else if (curPowerState == tmTDA18280_PowerWBAndNB)
        {
            /* transition WBAndNB -> WBAndNB */
            tmDBGPRINTEx(DEBUGLVL_INOUT, "%s: WBAndNB -> WBAndNB", __FUNCTION__);
            /* nothing to do */
        }
        else if (curPowerState == tmTDA18280_PowerNBOnly)
        {
            /* transition NBOnly -> WBAndNB */
            tmDBGPRINTEx(DEBUGLVL_INOUT, "%s: NBOnly -> WBAndNB", __FUNCTION__);

            /* Activate CGU and reset */
            if (err == TM_OK)
            {
                err = iTDA18280_BlockCGUAndResetStartupWB(pObj);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18280_BlockCGUAndResetStartupWB(0x%08X) failed.", pObj->tUnitW));
            }

            /* Activate ADC */
#ifndef SIMU_RTL
            if (err == TM_OK)
            {
                err = iTDA18280_BlockADCStartup(pObj);
            }
            if (err == TM_OK)
            {
                err = iTDA18280_BlockADCCalibration(pObj);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18280_BlockADCCalibration(0x%08X) failed.", pObj->tUnitW));
            }
#endif

            /* Activate RF AGC if config is enabled */
            if (err == TM_OK)
            {
                if (pObj->RfAgcCfg.eRfAgcCtrl == tmTDA18280_RfAgcCtrl_Enabled)
                {
                    err = iTDA18280_EnableWbRfAgc(pObj, True);
                }
            }
        }
        break;

    case tmTDA18280_PowerNBOnly:
        tmDBGPRINTEx(DEBUGLVL_INOUT, "%s: NBOnly", __FUNCTION__);

        if (curPowerState == tmTDA18280_PowerStandby)
        {
            /* transition Standby -> NBOnly */
            tmDBGPRINTEx(DEBUGLVL_INOUT, "%s: Standby -> NBOnly", __FUNCTION__);

            if (err == TM_OK)
            {
                err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_cgu_PLL_CONTROL__synonmain, 1);
            }

            /* Init LO */
            if (err == TM_OK)
            {
                err = iTDA18280_BlockLOChainConfigAndStartup(pObj, pObj->PllCfg.LOChainMode);
            }

            /* Activate CGU and reset */
            if (err == TM_OK)
            {
                err = iTDA18280_BlockCGUAndResetInitWBNB(pObj);
            }
        }
        else if (curPowerState == tmTDA18280_PowerWBOnly)
        {
            /* transition WBOnly -> NBOnly */
            tmDBGPRINTEx(DEBUGLVL_INOUT, "%s: WBOnly -> NBOnly", __FUNCTION__);

            /* switch off all supported CDP paths but aux */
            for (PathIndex = 0; PathIndex <= pObj->tFeature.uMaxIndexSupported; PathIndex++)
            {
                if (err == TM_OK)
                {
                    PathCfg.tPathStatus = tmTDA18280_OFF;
                    err = iTDA18280_PathSet(pObj, (tmTDA18280_Path_t)PathIndex, &PathCfg, tmTDA18280_PathSetAGCMode_Normal);
                }
            }

            /* Deactivate RF AGC if config is enabled */
            if (err == TM_OK)
            {
                if (pObj->RfAgcCfg.eRfAgcCtrl == tmTDA18280_RfAgcCtrl_Enabled)
                {
                    err = iTDA18280_EnableWbRfAgc(pObj, False);
                }
            }

            /* Deactivate ADC */
            if (err == TM_OK)
            {
                err = iTDA18280_BlockADCStop(pObj);
            }

            /* Deactivate CGU and reset */
            if (err == TM_OK)
            {
                err = iTDA18280_BlockCGUAndResetStopWB(pObj);
            }
        }
        else if (curPowerState == tmTDA18280_PowerWBAndNB)
        {
            /* transition WBAndNB -> NBOnly */
            tmDBGPRINTEx(DEBUGLVL_INOUT, "%s: WBAndNB -> NBOnly", __FUNCTION__);

            /* switch off supported CDP paths but aux */
            for (PathIndex = 0; PathIndex <= pObj->tFeature.uMaxIndexSupported; PathIndex++)
            {
                if (err == TM_OK)
                {
                    PathCfg.tPathStatus = tmTDA18280_OFF;
                    err = iTDA18280_PathSet(pObj, (tmTDA18280_Path_t)PathIndex, &PathCfg, tmTDA18280_PathSetAGCMode_Normal);
                }
            }

            /* Deactivate RF AGC if config is enabled */
            if (err == TM_OK)
            {
                if (pObj->RfAgcCfg.eRfAgcCtrl == tmTDA18280_RfAgcCtrl_Enabled)
                {
                    err = iTDA18280_EnableWbRfAgc(pObj, False);
                }
            }

            /* Deactivate ADC */
            if (err == TM_OK)
            {
                err = iTDA18280_BlockADCStop(pObj);
            }

            /* Deactivate CGU and reset */
            if (err == TM_OK)
            {
                err = iTDA18280_BlockCGUAndResetStopWB(pObj);
            }
        }
        else if (curPowerState == tmTDA18280_PowerNBOnly)
        {
            /* transition NBOnly -> NBOnly */
            tmDBGPRINTEx(DEBUGLVL_INOUT, "%s: NBOnly -> NBOnly", __FUNCTION__);
            /* nothing to do */
        }
        break;

    case tmTDA18280_PowerStandby:
        tmDBGPRINTEx(DEBUGLVL_INOUT, "%s: Standby", __FUNCTION__);

        if (curPowerState == tmTDA18280_PowerStandby)
        {
            /* transition Standby -> Standby */
            tmDBGPRINTEx(DEBUGLVL_INOUT, "%s: Standby -> Standby", __FUNCTION__);
            /* nothing to do */
        }
        else
        {
            /* transition WBOnly/WBAndNB/NBOnly -> Standby */
            tmDBGPRINTEx(DEBUGLVL_INOUT, "%s: WBOnly/WBAndNB/NBOnly -> Standby", __FUNCTION__);

            /* switch off supported CDP paths */
            for (PathIndex = 0; PathIndex <= pObj->tFeature.uMaxIndexSupported; PathIndex++)
            {
                if (err == TM_OK)
                {
                    PathCfg.tPathStatus = tmTDA18280_OFF;
                    err = iTDA18280_PathSet(pObj, (tmTDA18280_Path_t)PathIndex, &PathCfg, tmTDA18280_PathSetAGCMode_Normal);
                }
            }
            /* switch off NB path */
            if (err == TM_OK)
            {
                PathCfg.tPathStatus = tmTDA18280_OFF;
                err = iTDA18280_PathSet(pObj, tmTDA18280_Path_Aux, &PathCfg, tmTDA18280_PathSetAGCMode_Normal);
            }

            /* Activate CGU and reset */
            if(err == TM_OK)
            {
                err = iTDA18280_BlockCGUAndResetInitWBNB(pObj);
            }

            /* Put ADC in Standby */
    #ifndef SIMU_RTL
            /* set pon2reset=0, (bias is powered down, digital keeps state) */
            if (err == TM_OK)
            {
                err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_adc_launch_control_RESET_CONTROL_EN2__pon2reset_enable, 0);
                tmDBGPRINTEx(DEBUGLVL_BLAB, "      - 1)    write_control_registers(reg_adc_launch_control, 16#008#, 2#0111111#");
            }

            if (err == TM_OK)
            {
                err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_adc_launch_control_CALIBRATION_SETTINGS__pon, 0);
                tmDBGPRINTEx(DEBUGLVL_BLAB, "   - powered off. ");
            }
    #endif

            /* Deactivate CGU and reset */
            if (err == TM_OK)
            {
                err = iTDA18280_BlockCGUAndResetStopWBNB(pObj);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18280_BlockCGUAndResetStopWBNB(0x%08X) failed.", pObj->tUnitW));
            }

            /* Cut all LO clocks except ADC */
            if (err == TM_OK)
            {
                err = iTDA18280_BlockLOChainStop(pObj);
            }

            /* PLL1 6G 8G OFF */
            if (err == TM_OK)
            {
                err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_cgu_PLL_CONTROL__synonmain, 0);
            }
        }
        break;

    default:
        /* power state not supported */
        break;
    }

    if (err != TM_OK)
    {
        /* in case of failure, come back to previous state */
        pObj->curPowerState = curPowerState;
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_DevicePowerStateGet                                 */
/*                                                                            */
/* DESCRIPTION: Gets the power state.                                         */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_DevicePowerStateGet(
                              ptmTDA18280_Object_t       pObj,       /* I: Driver object */
                              tmTDA18280_PowerState_t    *pePowerState  /* I: Power state of TDA18280 */
                              )
{
    tmErrorCode_t   err = TM_OK;
    UInt16 ulValue;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    if (pObj->curPowerState == tmTDA18280_PowerMax)
    {
        /* not initialized, return current state */
        *pePowerState = pObj->curPowerState;
    }
    else /* (pObj->curPowerState != tmTDA18280_PowerMax) */
    {
        *pePowerState = tmTDA18280_PowerMax;

        err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_cgu_PLL_CONTROL__synonmain, &ulValue);

        if (err == TM_OK)
        {
            switch(ulValue)
            {
            case 0:
                *pePowerState = tmTDA18280_PowerStandby;
                break;
            case 1:
                *pePowerState = pObj->curPowerState;
                break;
            default:
                break;
            }
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_I2CWriteBitField                                    */
/*                                                                            */
/* DESCRIPTION: Writes in TDA18280 hardware                                   */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_I2CWriteBitField(
                           ptmTDA18280_Object_t           pObj,       /* I: Driver object */
                           UInt16  Base_Address, 
                           UInt16  Address, 
#ifdef PRINT_I2C
                           Char    Str[50], 
#endif
                           UInt8   PositionInBits, 
                           UInt8   WidthInBits, 
                           UInt16                 uData      /* I: Data to write */
                           )
{
    tmErrorCode_t   err = TM_OK;
    UInt16          RegAddr = 0;
    UInt32          DataLen = 2;
    UInt16          DataRead = 0;
    UInt8           pRegData[4] = {0, 0, 0, 0};
    UInt32          RegMask = 0;
    UInt8           i = 0;
    UInt8           pAddr[4] = {0, 0, 0, 0};

    /* tmDBGPRINTEx(DEBUGLVL_INOUT, "%s ", __FUNCTION__); */
#ifdef PRINT_I2C
    /* this test is to avoid writing a log if no action has been performed */
    /* the real test is done in P_SIO function */
    if( UNIT_PATH_CONFIG_GET(pObj->tUnitW)!=0x80 )
    {
        tmDBGPRINTEx(DEBUGLVL_BLAB, "WBF:0x%04X>%s", uData ,Str);
    }
#endif

    if( Address == 0xFF || PositionInBits == 0xFF || WidthInBits == 0xFF )
    {
        /* invalid definition of the bit field */
        /* this might mean this bit field is not available for this ES version */
        /* a check should have been done earlier in the call stack */
    }
    else
    {

        /* Set Register Address */
        RegAddr = Base_Address+Address;
        for( i = 0; i < TDA18280_REG_ADD_SZ; i++ )
        {
            /* start with MSB */
            pAddr[i] = (RegAddr >> ((TDA18280_REG_ADD_SZ-i-1)*8)) & 0xFF;
        }

        if ( P_SIO_READ_VALID)
        {
            /* Read data from TDA18280 */
            err = P_SIO_READ(pObj->tUnitW, TDA18280_REG_ADD_SZ, pAddr, DataLen, pRegData);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "IO_Read(0x%08X, 1, 0x%02X, %d) failed.", pObj->tUnitW, RegAddr, DataLen));
        }

        if(err == TM_OK)
        {
            RegMask = ( (1 << WidthInBits) - 1);
            /* Limit uData to WidthInBits */
            uData &= RegMask;

            /* Set Data */
            RegMask = RegMask << PositionInBits;
            DataRead = (*pRegData << 8) + *(pRegData+1);
            DataRead &= (UInt16)(~RegMask);
            DataRead |= uData << PositionInBits;
            *pRegData = DataRead >> 8;
            *(pRegData+1) = DataRead & 0xFF;

            /* if( (eBusAccess&Bus_NoWrite) == False && P_SIO_WRITE_VALID) */
            if( P_SIO_WRITE_VALID)
            {
                /* Write data to TDA18280 */
                err = P_SIO_WRITE(pObj->tUnitW, TDA18280_REG_ADD_SZ, pAddr, DataLen, pRegData);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "IO_Write(0x%08X, 1, 0x%02X, %d) failed.", pObj->tUnitW, RegAddr, DataLen));
            }
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_I2CReadBitField                                     */
/*                                                                            */
/* DESCRIPTION: Reads in TDA18280 hardware                                    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_I2CReadBitField(
                          ptmTDA18280_Object_t           pObj,       /* I: Driver object */
                          UInt16  Base_Address, 
                          UInt16   Address, 
#ifdef PRINT_I2C
                          Char Str[50], 
#endif
                          UInt8   PositionInBits, 
                          UInt8   WidthInBits, 
                          UInt16*                 puData     /* I: Data to read */
                          )
{
    tmErrorCode_t   err = TM_OK;
    UInt16          RegAddr = 0;
    UInt32          DataLen = 2;
    UInt8           pRegData[4] = {0, 0, 0, 0};
    UInt32          RegMask = 0;
    UInt8           i = 0;
    UInt8           pAddr[4] = {0, 0, 0, 0};

    if( Address == 0xFF || PositionInBits == 0xFF || WidthInBits == 0xFF )
    {
        /* invalid definition of the bit field */
        /* this might mean this bit field is not available for this ES version */
        /* a check should have been done earlier in the call stack */
    }
    else
    {
        /* Set Register Address */
        RegAddr = Base_Address+Address;
        for( i = 0; i < TDA18280_REG_ADD_SZ; i++ )
        {
            /* start with MSB */
            pAddr[i] = (RegAddr >> ((TDA18280_REG_ADD_SZ-i-1)*8)) & 0xFF;
        }

        /* if( (eBusAccess&Bus_NoRead) == False && P_SIO_READ_VALID) */
        if((err == TM_OK) && ( P_SIO_READ_VALID))
        {
            /* Read data from TDA18280 */
            err = P_SIO_READ(pObj->tUnitW, TDA18280_REG_ADD_SZ, pAddr, DataLen, pRegData);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "IO_Read(0x%08X, 1, 0x%02X, %d) failed.", pObj->tUnitW, RegAddr, DataLen));
        }

        if(err == TM_OK && puData != Null)
        {
            /* Copy Raw Data */
            *puData = (*pRegData << 8) + *(pRegData+1);

            /* Get Data */
            RegMask = ( (1 << WidthInBits) - 1) << PositionInBits;
            *puData &= (UInt16)RegMask;
            *puData = (*puData) >> PositionInBits;
        }
    }

#ifdef PRINT_I2C
    /* this test is to avoid writing a log if no action has been performed */
    /* the real test is done in P_SIO function */
    if( UNIT_PATH_CONFIG_GET(pObj->tUnitW)!=0x80 )
    {
        tmDBGPRINTEx(DEBUGLVL_BLAB, "RBF:%s<0x%04X", Str, *puData);
    }
#endif

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_I2CWriteRegister                                    */
/*                                                                            */
/* DESCRIPTION: Writes 1 registers of the TDA18280 hardware.                  */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_I2CWriteRegister(
                           ptmTDA18280_Object_t          pObj,       /* I: Driver object */
                           UInt16  Base_Address, 
                           UInt16   Address, 
#ifdef PRINT_I2C
                           Char Str[50], 
#endif
                           UInt16   RegData 
                           )
{
    tmErrorCode_t   err = TM_OK;
    UInt16          RegAddr = 0;
    UInt8           uBytesBuffer[2];
    UInt8           i = 0;
    UInt8           pAddr[4] = {0, 0, 0, 0};

#ifdef PRINT_I2C
    tmDBGPRINTEx(DEBUGLVL_BLAB, "WREG:0x%04X>%s", RegData ,Str);
#endif

    if( Address == 0xFF )
    {
        /* invalid definition of the register */
        /* this might mean this register is not available for this ES version */
        /* a check should have been done earlier in the call stack */
    }
    else
    {
        /* Set Register Address */
        RegAddr = Base_Address+Address;
        for( i = 0; i < TDA18280_REG_ADD_SZ; i++ )
        {
            /* start with MSB */
            pAddr[i] = (RegAddr >> ((TDA18280_REG_ADD_SZ-i-1)*8)) & 0xFF;
        }

        uBytesBuffer[0] = RegData >>8;
        uBytesBuffer[1] = RegData & 0xFF;

        if(err == TM_OK && P_SIO_WRITE_VALID)
        {
            /* Write data to TDA18280 */
            err = P_SIO_WRITE(pObj->tUnitW, TDA18280_REG_ADD_SZ, pAddr, 2, uBytesBuffer);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "IO_Write(0x%08X, 1, 0x%02X, %d) failed.", pObj->tUnitW, Address, 2));
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_I2CReadRegister                                     */
/*                                                                            */
/* DESCRIPTION: Reads driver RegMap cached data from TDA18280 hardware.       */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_I2CReadRegister(
                          ptmTDA18280_Object_t          pObj,      /* I: Driver object */
                          UInt16  Base_Address, 
                          UInt16   Address, 
#ifdef PRINT_I2C
                          Char Str[50], 
#endif
                          UInt32                uReadLen,  /* I: Number of registers to read */
                          UInt16*               pRegData 
                          )
{
    tmErrorCode_t   err = TM_OK;
    UInt16          RegAddr = 0;
    UInt8           uBytesBuffer[500];
    unsigned int i = 0;
    UInt8           pAddr[4] = {0, 0, 0, 0};

    /* tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, tUnit); */

    if( Address == 0xFF )
    {
        /* invalid definition of the register */
        /* this might mean this register is not available for this ES version */
        /* a check should have been done earlier in the call stack */
    }
    else
    {
        if ( uReadLen >250) 
        {
            err = TDA18280_ERR_BAD_PARAMETER;
        }

        if ( pRegData == Null) 
        {
            err = TDA18280_ERR_BAD_PARAMETER;
        }

        /* Set Register Address */
        RegAddr = Base_Address+Address;
        for( i = 0; i < TDA18280_REG_ADD_SZ; i++ )
        {
            /* start with MSB */
            pAddr[i] = (RegAddr >> ((TDA18280_REG_ADD_SZ-i-1)*8)) & 0xFF;
        }

        if(err == TM_OK && P_SIO_READ_VALID)
        {
            /* Read data from TDA18280 */
            err = P_SIO_READ(pObj->tUnitW, TDA18280_REG_ADD_SZ, pAddr, uReadLen*2, uBytesBuffer);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "IO_Read(0x%08X, 1, 0x%02X, %d) failed.", pObj->tUnitW, Address, uReadLen));
        }

        for (i=0; i<uReadLen; i++)
        {
            *(pRegData+i) = (uBytesBuffer[2*i] << 8 ) + (uBytesBuffer[(2*i)+1] & 0xFF);
#ifdef PRINT_I2C
            if (uReadLen==1)
            {
                tmDBGPRINTEx(DEBUGLVL_BLAB, "RREG:%s<0x%04X", Str, *(pRegData+i));
            }
            else
            {
                if (i==0)
                {
                    tmDBGPRINTEx(DEBUGLVL_BLAB, "RREG : %d data from %s", uReadLen ,Str);
                    tmDBGPRINTEx(DEBUGLVL_BLAB, "RREG Data %d is 0x%04X", i, *(pRegData+i));    
                }
                else
                {
                    tmDBGPRINTEx(DEBUGLVL_BLAB, "RREG Data %d is 0x%04X", i, *(pRegData+i));    
                }
            }
#endif
        }
    }
    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_UtilWait                                            */
/*                                                                            */
/* DESCRIPTION: Waits for requested time.                                     */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t 
iTDA18280_UtilWait(
                   ptmTDA18280_Object_t   pObj,   /* I: Driver object */
                   UInt32              Time    /* I: time to wait for */
                   )
{
    tmErrorCode_t   err = TDA18280_ERR_NULL_CONTROLFUNC;

    /* tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, tUnit); */

    if(P_STIME_WAIT_VALID)
    {
        /* Wait Time ms */
        err = P_STIME_WAIT(pObj->tUnitW, Time);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TIME_Wait(0x%08X, %d) failed.", pObj->tUnitW, Time));
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_BlockADCStop                                        */
/*                                                                            */
/* DESCRIPTION: Calibrate ADC                                                 */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_BlockADCStop(
                          ptmTDA18280_Object_t       pObj   /* I: Driver object */
                          )
{
    tmErrorCode_t   err = TM_OK;
    UInt16 uVal = 0;

    if (pObj==Null)
    {
        err = TDA18280_ERR_BAD_PARAMETER;
    }

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    /* read initial current settings prior to the smooth current decrease */
    if (err == TM_OK)
    {
        err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_adc_launch_control_PON_ENABLES, 1, &uVal);
    }
    /* smooth current decrease to avoid uncors in NB */
    if (err == TM_OK)
    {
        uVal = uVal & (~PON_INT_DFT_MASK);
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_adc_launch_control_PON_ENABLES, uVal);
    }
    if (err == TM_OK)
    {
        uVal = uVal & (~PON_INT_DEL1_MASK);
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_adc_launch_control_PON_ENABLES, uVal);
    }
    if (err == TM_OK)
    {
        uVal = uVal & (~PON_INT_DEL2_MASK);
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_adc_launch_control_PON_ENABLES, uVal);
    }
    if (err == TM_OK)
    {
        err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_adc_launch_control_CALIBRATION_SETTINGS__pon, 0);
    }

    /* Reset all blocks including ADC */
    if (err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_reset_RESET_GESTION, 0);
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_BlockADCStartup                                     */
/*                                                                            */
/* DESCRIPTION: Calibrate ADC                                                 */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_BlockADCStartup(
                          ptmTDA18280_Object_t       pObj   /* I: Driver object */
                          )
{
    tmErrorCode_t   err = TM_OK;
    UInt16 uVal = 0;

    if (pObj==Null)
    {
        err = TDA18280_ERR_BAD_PARAMETER;
    }

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    if (err == TM_OK)
    {
        err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_cgu_LO_CONTROL__dig_clkgen_pd_voh, 0);
    }

    /* Reset all blocks including ADC */
    if (err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_reset_RESET_GESTION, 0);
    }
    /* Release ADC reset */
    if (err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_reset_RESET_GESTION, 7);
    }

    /* set lowest currents before performing smooth current increase */
    if (err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_adc_launch_control_PON_ENABLES, 0x0000);
    }
    if (err == TM_OK)
    {
        err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_adc_launch_control_CALIBRATION_SETTINGS__pon, 1);
    }
    /* smooth current increase to avoid uncors in NB */
    if (err == TM_OK)
    {
        uVal = uVal | PON_INT_DFT_MASK; /* 0xF81C */
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_adc_launch_control_PON_ENABLES, uVal);
    }
    if (err == TM_OK)
    {
        uVal = uVal | PON_INT_DEL1_MASK; /* 0xFAFC */
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_adc_launch_control_PON_ENABLES, uVal);
    }
    if (err == TM_OK)
    {
        uVal = uVal | PON_INT_DEL2_MASK; /* 0xFFFF */
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_adc_launch_control_PON_ENABLES, uVal);
    }
    /* PRCR#566: patch for smooth current increase: */
    /* reset the clock engine to ensure proper start of current sources */
    if (err == TM_OK)
    {
        err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_adc_launch_control_PERFORM_MANUAL_RESET1__clkengine_ctrl, 1);
    }
    /* reset the sequencers */
    if (err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_adc_launch_control_PERFORM_MANUAL_RESET1, 0x00F0);
    }

    /* WB ADC optimisation */
    /* Tda18280_reg_adc_bias1 BG_caldacs gain_cascode   to 0x3 */
    /* Tda18280_reg_adc_bias1 BG_caldacs offset_cascode to 0x3 */
    /* Tda18280_reg_adc_bias1 BG_caldacs offsetcurrentsource to 0xC */
    /* register default value is 0x7777 -> 0x373C */

    if (err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_adc_bias1_BG_CALDACS, 0x373C);
    }

    /* configure calibration current source */
    if (err == TM_OK)
    {
        err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_adc_bias1_SU_CALDACS__offset_currentsource, 12);
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_BlockADCCalibration                                 */
/*                                                                            */
/* DESCRIPTION: Calibrate ADC                                                 */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_BlockADCCalibration(
                              ptmTDA18280_Object_t       pObj   /* I: Driver object */
                              )
{
    tmErrorCode_t   err = TM_OK;
    UInt16          value16 = 0;
    UInt16 state_bg  = 0;  
    UInt16 status_bg = 0; 
    UInt16 status_q3 = 0;
    UInt16 status_q2 = 0;
    UInt16 status_q1 = 0;
    UInt16 status_q0 = 0;
    UInt16 status_calibration_1  = 0 ;
    UInt16 status_calibration_2  = 0 ;
    UInt16 state_cala = 0;
    UInt16 state_calb = 0;
    UInt16 state_calc = 0;
    UInt16 state_cald = 0;
    UInt16 cal_not_finish = 1;
    UInt16 i=0, delay = 0;
    const UInt16 step = 10; /* step in millisecond */

    if (pObj==Null)
    {
        err = TDA18280_ERR_BAD_PARAMETER;
    }

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    /* start automagic calibration (no need to wait after setting pon=1, command is only executed when system is ready to accept it) */
    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_adc_launch_control_START_CALIBRATION__launch_cal , 1);
    }
    tmDBGPRINTEx(DEBUGLVL_BLAB, "      - 1)    write_control_registers(reg_adc_launch_control, 16#000#, 2#000000000000001#");

    while (cal_not_finish==1)
    {
        if(err == TM_OK)
        {
            err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_adc_launch_control_STATUS_CALIBRATION_1, 1, &value16);
        }
        if(err == TM_OK)
        {
            status_calibration_1  = value16 ;
            state_bg  = status_calibration_1 >>  5 & 3;  
            status_bg = status_calibration_1 >>  4 & 1; 
            status_q3 = status_calibration_1 >>  3 & 1;
            status_q2 = status_calibration_1 >>  2 & 1;
            status_q1 = status_calibration_1 >>  1 & 1;
            status_q0 = status_calibration_1 >>  0 & 1;
        }
        if(err == TM_OK)
        {
            err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_adc_launch_control_STATUS_CALIBRATION_2, 1,&value16);
        }
        if(err == TM_OK)
        {
            status_calibration_2  = value16 ;
            state_cala = status_calibration_2 >>  12 & 0xF;
            state_calb = status_calibration_2 >>   8 & 0xF;
            state_calc = status_calibration_2 >>   4 & 0xF;
            state_cald = status_calibration_2 >>   0 & 0xF;
        }
        if ( (state_bg == 3) && (state_cala == 5) && (state_calb == 5) && (state_calc == 5) && (state_cald == 5))
        {
            cal_not_finish = 0;
        }
        if (cal_not_finish == 1) {
            iTDA18280_UtilWait(pObj, step);
            delay = step * i ;
            i++;
        }
        if (delay > 600) {
            cal_not_finish = 0;
            tmDBGPRINTEx(DEBUGLVL_BLAB, "      - ADC Cal failed !!");
        }
        else
        {
            tmDBGPRINTEx(DEBUGLVL_BLAB, "      - ADC Cal succeeded: %d ms", delay);
        }
    }
    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_BlockDAC_SetCfg                                     */
/*                                                                            */
/* DESCRIPTION: Configure DAC block                                           */
/*                                                                            */
/* RETURN:                                                                    */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t iTDA18280_BlockDAC_SetCfg( ptmTDA18280_Object_t pObj, tmTDA18280_DACConfiguration_t* ptConfiguration )
{
    tmErrorCode_t   err = TM_OK;
    UInt8 uValue = 0;

    tmDBGPRINTEx(DEBUGLVL_INOUT, __FUNCTION__);

    if( ptConfiguration->eEnable == tmTDA18280_OFF )
    {
        tmDBGPRINTEx(DEBUGLVL_BLAB, "  - Disable DAC");

        if(err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_cgu_LO_CONTROL__dig_clkdsp_pd_dac_debug_vih , 1);
        }
    }
    else
    {
        tmDBGPRINTEx(DEBUGLVL_BLAB, "  - Enable and Configure DAC");

        /* Enable and configure DAC */
        if(err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_cgu_LO_CONTROL__dig_clkdsp_pd_dac_debug_vih , 0);
        }
        if( err == TM_OK )
        {
            uValue = (UInt8)ptConfiguration->ulInternalRefResistor;
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_channel_stacker_CTRL_TETRA_DAC__set_rint , uValue);
        }

        if( err == TM_OK )
        {
            uValue = (UInt8)ptConfiguration->ulFineTuning;
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_channel_stacker_FINE_CUR_CTRL__fine_cur_ctrl, uValue );
        }

        if( err == TM_OK )
        {
            uValue = (UInt8)ptConfiguration->ulCoarseTuning;
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_channel_stacker_COARSE_CUR_CTRL_DAC__coarse_cur_ctrl_dac1, uValue );
        }
    }    

    return err;
}


/*============================================================================*/
/* FUNCTION:    iTDA18280_BlockDAC_GetCfg                                     */
/*                                                                            */
/* DESCRIPTION: Get DAC configuration                                         */
/*                                                                            */
/* RETURN:                                                                    */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t iTDA18280_BlockDAC_GetCfg( ptmTDA18280_Object_t pObj, tmTDA18280_DACConfiguration_t* ptConfiguration )
{
    tmErrorCode_t  err = TM_OK;
    UInt16         ulValue;

    /* initialize to handle errors */
    ptConfiguration->eEnable = tmTDA18280_OFF;
    ptConfiguration->ulInternalRefResistor = 0;
    ptConfiguration->ulFineTuning = 0;
    ptConfiguration->ulCoarseTuning = 0;

    /* check if the channel stacker block is readable */
    err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_reset_RESET_DDC__chstk , &ulValue);

    if (err == TM_OK && ulValue != 1)
    {
        err = TDA18280_ERR_HW_FAILED;
    }

    /* update enable status */
    if (err == TM_OK)
    {
        err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_cgu_LO_CONTROL__dig_clkdsp_pd_dac_debug_vih , &ulValue);
    }

    if( err == TM_OK )
    {
        if( ulValue == 1 )
        {
            /* disable */
            ptConfiguration->eEnable = tmTDA18280_OFF;
        }
        else
        {
            /* enable */
            ptConfiguration->eEnable = tmTDA18280_ON;
        }
    }

    if( err == TM_OK )
    {
        err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_channel_stacker_CTRL_TETRA_DAC__set_rint , &ulValue);
        ptConfiguration->ulInternalRefResistor = (UInt32)ulValue;
    }
    if( err == TM_OK )
    {
        err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_channel_stacker_FINE_CUR_CTRL__fine_cur_ctrl, &ulValue);
        ptConfiguration->ulFineTuning = (UInt32)ulValue;
    }
    if( err == TM_OK )
    {
        err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_channel_stacker_COARSE_CUR_CTRL_DAC__coarse_cur_ctrl_dac1, &ulValue);
        ptConfiguration->ulCoarseTuning = (UInt32)ulValue;
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_DeviceHwInit                                        */
/*                                                                            */
/* DESCRIPTION: Reset TDA18280                                                */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_DeviceHwInit(
                       ptmTDA18280_Object_t       pObj   /* I: Driver object */
                       )
{
    tmErrorCode_t   err = TM_OK;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    /* configure Inputs/Outputs */
    if (err == TM_OK)
    {
        err = iTDA18280_ConfigureIOs(pObj);
    }

    /* Activate CGU and reset */
    if (err == TM_OK)
    {
        err = iTDA18280_BlockCGUAndResetInitWBNB(pObj);
    }

    /* Init PLL1 & Check lock PLL */
    if (err == TM_OK)
    {
        err = iTDA18280_BlockPLL1Startup(pObj);
    }

    /* Init LO for ADC cal */
    if (err == TM_OK)
    {
        err = iTDA18280_BlockLOChainConfigAndStartup(pObj, pObj->PllCfg.LOChainMode);
    }

    /* switch off Narrow Band ADC (auxiliary path) */
    /* need to be started first because the state is not determined */
    if (err == TM_OK)
    {
        err = iTDA18280_NbAdcConfiguration(pObj, tmTDA18280_ON);
    }
    if (err == TM_OK)
    {
        err = iTDA18280_NbAdcConfiguration(pObj, tmTDA18280_OFF);
    }

    /* Configure irq */
    if (err == TM_OK)
    {
        err = iTDA18280_IrqConfiguration(pObj);
    }

    /* demods CDP and OOB are not initialized here */
    /* but when enabled in iTDA18280_PathDemodStartup */

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_BlockLOChainConfigAndStartup                        */
/*                                                                            */
/* DESCRIPTION: LO_Chain Programming                                          */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_BlockLOChainConfigAndStartup(ptmTDA18280_Object_t pObj, tmTDA18280_LOChainMode_t LOChainMode)
{
    tmErrorCode_t   err = TM_OK;
    /*
    +---------+------------------+--------------------------------------------------------------------------------------------------------------------------------+-------------------------------------------------------+-------------------------------------------------------+--------------------
    | Prog_idx|serdes pll        |LO CHAIN                                                                                                                        |  2700                                                 |  2592                                                 | pll_div_ovrrd_val
    +---------+------------------+--------------------------------------------------------------------------------------------------------------------------------+-------------------------------------------------------+-------------------------------------------------------+--------------------
    |         |pll_div_ovrrd_val |Ndiv     DivUS   DivUStoSD en_sw0  en_sw1  en_integer_mode en_sw2_0    en_sw2_1    en_sw2_2*    sd_divratio_1   sd_divratio_0   |  freqLO  freqUSout   freqUStoSDout   freqSDout   Nsd  |  freqLO  freqUSout   freqUStoSDout   freqSDout   Nsd  |
    +---------+------------------+--------------------------------------------------------------------------------------------------------------------------------+-------------------------------------------------------+-------------------------------------------------------+--------------------
    | 1       |      23          |  4        4           2       0       1               1       0           1           0                   1               0    |  2700    675             1350        112,5       12   |  2592    648         1296            108         12   |
    | 3       |      19          |  5        5           2,5     0       1               0       0           1           0                   0               0    |  2700    540             1080        135         8    |  2592    518,4       1036,8          129,6       8    |
    |4 default|      23          |  6        6           3       1       1               1       0           1           0                   0               0    |  2700    450             900         112,5       8    |  2592    432         864             108         8    |
    |xxxxxxxxx|      20          |  7        7           3,5     1       1               0       0           1           0                   1               1    |  2700    385,7142857     771,4285714 128,5714286 6    |  2592    370,2857143 740,5714286     123,4285714 6    |
    |5        |      23          |  8        8           2       0       1               1       1           0           0                   1               0    |  2700    337,5           1350        112,5       12   |  2592    324         1296            108         12   |
    |xxxxxxxxx|      17          |  9        9           2,25    0       0               0       0           0           1                   0               0    |  2700    300             1200        150         8    |  2592    288         1152            144         8    |
    | 8       |      19          | 10       10           2,5     0       1               0       1           0           0                   0               0    |  2700    270             1080        135         8    |  2592    259,2       1036,8          129,6       8    |
    +---------+------------------+--------------------------------------------------------------------------------------------------------------------------------+-------------------------------------------------------+-------------------------------------------------------+--------------------
    */

    UInt16 dig_clkdsp_current           = 0 ; 
    UInt16 dig_clkdsp_pd_adc_nb_vih     = 1 ; 
    UInt16 dig_clkdsp_pd_dac_debug_vih  = 1 ; 
    UInt16 dig_clkdsp_pd_vih            = 0 ; 
    UInt16 ext_xtal_to_pad_ena          = 0 ; 
    UInt16 dig_clkgen_pd_voh            = 0 ; 

    UInt16 dig_lo_current               = 0 ; 
    UInt16 dig_lo_inbuf_current         = 0 ; 
    UInt16 dig_lo_mode                  = 0 ; 
    UInt16 dig_lo_pd_div2_voh           = 1 ;  /* Path 6GHz off */
    UInt16 dig_lo_pd_div3_voh           = 0 ; 

    UInt16 dig_sd_current               = 3 ; 
    UInt16 dig_sd_divratio              = 0 ; 
    UInt16 dig_sd_en_test_voh           = 0 ; 
    UInt16 dig_sd_pd_voh                = 1 ; 

    UInt16 dig_us_current               = 3 ; 
    UInt16 dig_us_en_frac_mode_vih      = 0 ; 
    UInt16 dig_us_en_integer_mode_vih   = 0 ; 
    UInt16 dig_us_en_integer_vih        = 0 ; 
    UInt16 dig_us_en_sw0_vih            = 0 ; 
    UInt16 dig_us_en_sw1_vih            = 0 ; 
    UInt16 dig_us_en_sw2_0_vih          = 0 ; 
    UInt16 dig_us_en_sw2_1_vih          = 0 ; 
    UInt16 dig_us_en_sw2_2_vih          = 0 ; 
    UInt16 dig_us_en_test_vih           = 0 ; 
    UInt16 dig_us_pd_vih                = 1 ; 

    UInt16 lo_control    = 0;
    UInt16 lo_control_sd = 0 ;
    UInt16 lo_control_us = 0 ;

    UInt16 pd_vco_8g=0;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    if (pObj->PllCfg.pll1Freq < tmTDA18280_PLL1_7776MHz) 
    {
        pd_vco_8g = 1;
    }
    else
    {
        pd_vco_8g = 0;
    }

    switch (LOChainMode) {
        case 1:
            tmDBGPRINTEx(DEBUGLVL_BLAB, "************************************************************************************");
            tmDBGPRINTEx(DEBUGLVL_BLAB, "* INFO --> LO Chain TEST_Case 1           ...");
            tmDBGPRINTEx(DEBUGLVL_BLAB, "* INFO --> US_div = 4 / SRDes_div = 12    ...");
            tmDBGPRINTEx(DEBUGLVL_BLAB, "* INFO --> Test ADC Clk / Current  = 10   ...");
            tmDBGPRINTEx(DEBUGLVL_BLAB, "************************************************************************************");
            dig_lo_mode                 = 3;

            dig_us_en_test_vih          = 0;
            dig_us_current              = 3;

            dig_sd_en_test_voh          = 0;
            dig_sd_current              = 3;

            dig_lo_pd_div2_voh          = !pd_vco_8g ;  /* Path 6GHz off */
            dig_lo_pd_div3_voh          = pd_vco_8g ; 
            dig_lo_inbuf_current        = 3;
            dig_lo_current              = 2;

            dig_clkdsp_current          = 2;
            
            dig_us_en_frac_mode_vih     = 0;
            dig_us_en_integer_mode_vih  = 1;
            dig_us_en_integer_vih       = 0;
            dig_us_en_sw0_vih           = 0;
            dig_us_en_sw1_vih           = 1;
            dig_us_en_sw2_0_vih         = 0;
            dig_us_en_sw2_1_vih         = 1;
            dig_us_en_sw2_2_vih         = 0;

            dig_sd_divratio             = 2;

            break;
        case 3:
            tmDBGPRINTEx(DEBUGLVL_BLAB, "************************************************************************************");
            tmDBGPRINTEx(DEBUGLVL_BLAB, "* INFO --> LO Chain TEST_Case 3         ...");
            tmDBGPRINTEx(DEBUGLVL_BLAB, "* INFO --> US_div = 5 / SRDes_div = 8   ...");
            tmDBGPRINTEx(DEBUGLVL_BLAB, "* Test US Clk / Current  = 11           ...");
            tmDBGPRINTEx(DEBUGLVL_BLAB, "************************************************************************************");
            dig_lo_mode                 = 1;

            dig_us_en_test_vih          = 1;
            dig_us_current              = 3;

            dig_sd_en_test_voh          = 0;
            dig_sd_current              = 3;

            dig_lo_pd_div2_voh           = !pd_vco_8g ;  /* Path 6GHz off */
            dig_lo_pd_div3_voh           = pd_vco_8g ; 
            dig_lo_inbuf_current        = 3;
            dig_lo_current              = 3;

            dig_clkdsp_current          = 3;

            dig_us_en_integer_mode_vih  = 0;
            dig_us_en_sw0_vih           = 0;
            dig_us_en_sw1_vih           = 1;
            dig_us_en_sw2_0_vih         = 0;
            dig_us_en_sw2_1_vih         = 1;
            dig_us_en_sw2_2_vih         = 0;

            dig_sd_divratio             = 0;

            break;
        case 4: /* default mode */
            tmDBGPRINTEx(DEBUGLVL_BLAB, "************************************************************************************");
            tmDBGPRINTEx(DEBUGLVL_BLAB, "* INFO --> LO Chain TEST_Case 4         ...");
            tmDBGPRINTEx(DEBUGLVL_BLAB, "* INFO --> US_div = 6 / SRDes_div = 8   ...");
            tmDBGPRINTEx(DEBUGLVL_BLAB, "* Test SD Clk / Current  = 01           ...");
            tmDBGPRINTEx(DEBUGLVL_BLAB, "************************************************************************************");
            dig_lo_mode                 = 1;

            dig_us_en_test_vih          = 0;
            dig_us_current              = 3;

            dig_sd_en_test_voh          = 0;
            dig_sd_current              = 3;

            dig_lo_pd_div2_voh          = !pd_vco_8g ;  /* Path 6GHz off */
            dig_lo_pd_div3_voh          = pd_vco_8g ; 
            dig_lo_inbuf_current        = 3;
            dig_lo_current              = 3;

            dig_clkdsp_current          = 1;

            dig_us_en_integer_mode_vih  = 1;
            dig_us_en_sw0_vih           = 1;
            dig_us_en_sw1_vih           = 1;
            dig_us_en_sw2_0_vih         = 0;
            dig_us_en_sw2_1_vih         = 1;
            dig_us_en_sw2_2_vih         = 0;

            dig_sd_divratio             = 0;

            break;
        case 5: 
            tmDBGPRINTEx(DEBUGLVL_BLAB, "************************************************************************************");
            tmDBGPRINTEx(DEBUGLVL_BLAB, "* INFO --> LO Chain TEST_Case 5                         ...");
            tmDBGPRINTEx(DEBUGLVL_BLAB, "* INFO --> Div3 Mode / US_div = 8 / SRDes_div = 12      ...");
            tmDBGPRINTEx(DEBUGLVL_BLAB, "* Test SD Clk / Current  = 00                           ...");
            tmDBGPRINTEx(DEBUGLVL_BLAB, "************************************************************************************");
            dig_lo_mode                 = 1;

            dig_us_en_test_vih          = 0;
            dig_us_current              = 3;

            dig_sd_en_test_voh          = 0;
            dig_sd_current              = 3;

            dig_lo_pd_div2_voh          = !pd_vco_8g ;  /* Path 6GHz off */
            dig_lo_pd_div3_voh          = pd_vco_8g ; 
            dig_lo_inbuf_current        = 3;
            dig_lo_current              = 0;

            dig_clkdsp_current          = 0;

            dig_us_en_integer_mode_vih  = 1;
            dig_us_en_sw0_vih           = 0;
            dig_us_en_sw1_vih           = 1;
            dig_us_en_sw2_0_vih         = 1;
            dig_us_en_sw2_1_vih         = 0;
            dig_us_en_sw2_2_vih         = 0;

            dig_sd_divratio             = 2;

            break;
        case 8:
            tmDBGPRINTEx(DEBUGLVL_BLAB, "************************************************************************************");
            tmDBGPRINTEx(DEBUGLVL_BLAB, "* INFO --> LO Chain TEST_Case 8                         ...");
            tmDBGPRINTEx(DEBUGLVL_BLAB, "* INFO --> Div3 Mode / US_div = 10 / SRDes_div = 8      ...");
            tmDBGPRINTEx(DEBUGLVL_BLAB, "* Test SD Clk / Current  = 10                           ...");
            tmDBGPRINTEx(DEBUGLVL_BLAB, "************************************************************************************");
            dig_lo_mode                 = 1;

            dig_us_en_test_vih          = 0;
            dig_us_current              = 3;

            dig_sd_en_test_voh          = 0;
            dig_sd_current              = 3;

            dig_lo_pd_div2_voh          = !pd_vco_8g ;  /* Path 6GHz off */
            dig_lo_pd_div3_voh          = pd_vco_8g ; 
            dig_lo_inbuf_current        = 3;
            dig_lo_current              = 3;

            dig_clkdsp_current          = 1;

            dig_us_en_integer_mode_vih  = 0;
            dig_us_en_sw0_vih           = 0;
            dig_us_en_sw1_vih           = 1;
            dig_us_en_sw2_0_vih         = 1;
            dig_us_en_sw2_1_vih         = 0;
            dig_us_en_sw2_2_vih         = 0;

            dig_sd_divratio             = 0;

            break;
        case 9: /* 6Ghz test mode */
            tmDBGPRINTEx(DEBUGLVL_BLAB, "************************************************************************************");
            tmDBGPRINTEx(DEBUGLVL_BLAB, "* INFO --> LO Chain TEST_Case 9         ...");
            tmDBGPRINTEx(DEBUGLVL_BLAB, "* INFO --> US_div = 6 / SRDes_div = 6   ...");
            tmDBGPRINTEx(DEBUGLVL_BLAB, "* Test SD Clk / Current  = 01           ...");
            tmDBGPRINTEx(DEBUGLVL_BLAB, "************************************************************************************");
            dig_lo_mode                 = 1;

            dig_us_en_test_vih          = 0;
            dig_us_current              = 3;

            dig_sd_en_test_voh          = 0;
            dig_sd_current              = 3;

            dig_lo_pd_div2_voh          = !pd_vco_8g ;  /* Path 6GHz off */
            dig_lo_pd_div3_voh          = pd_vco_8g ; 
            dig_lo_inbuf_current        = 3;
            dig_lo_current              = 3;

            dig_clkdsp_current          = 1;

            dig_us_en_integer_mode_vih  = 1;
            dig_us_en_sw0_vih           = 1;
            dig_us_en_sw1_vih           = 1;
            dig_us_en_sw2_0_vih         = 0;
            dig_us_en_sw2_1_vih         = 1;
            dig_us_en_sw2_2_vih         = 0;

            dig_sd_divratio             = 3;

            break;
        default:
            tmDBGPRINTEx(DEBUGLVL_BLAB, "+---------+------------------+--------------------------------------------------------------------------------------------------------------------------------+------------------------------------------------------+------------------------------------------------------+---------------------");
            tmDBGPRINTEx(DEBUGLVL_BLAB, "|         |serdes pll        |LO CHAIN                                                                                                                        |  2700                                                |  2592                                                | pll_div_ovrrd_val");
            tmDBGPRINTEx(DEBUGLVL_BLAB, "+---------+------------------+--------------------------------------------------------------------------------------------------------------------------------+------------------------------------------------------+------------------------------------------------------+--------------------");
            tmDBGPRINTEx(DEBUGLVL_BLAB, "|Prog_idx |pll_div_ovrrd_val |Ndiv     DivUS   DivUStoSD en_sw0  en_sw1  en_integer_mode en_sw2_0    en_sw2_1    en_sw2_2    sd_divratio_1   sd_divratio_0    |  freqLO  freqUSout   freqUStoSDout   freqSDout   Nsd |  freqLO  freqUSout   freqUStoSDout  freqSDout   Nsd  |");
            tmDBGPRINTEx(DEBUGLVL_BLAB, "+---------+------------------+--------------------------------------------------------------------------------------------------------------------------------+------------------------------------------------------+------------------------------------------------------+--------------------");
            tmDBGPRINTEx(DEBUGLVL_BLAB, "|1        |    23            |  4        4           2       0       1               1       0           1           0                   1               0    |  2700    675             1350        112,5       12  |  2592    648         1296            108         12  |");
            tmDBGPRINTEx(DEBUGLVL_BLAB, "|3        |    19            |  5        5           2,5     0       1               0       0           1           0                   0               0    |  2700    540             1080        135         8   |  2592    518,4       1036,8          129,6       8   |");
            tmDBGPRINTEx(DEBUGLVL_BLAB, "|4 default|    23            |  6        6           3       1       1               1       0           1           0                   0               0    |  2700    450             900         112,5       8   |  2592    432         864             108         8   |");
            tmDBGPRINTEx(DEBUGLVL_BLAB, "|xxxxxxxxx|    20            |  7        7           3,5     1       1               0       0           1           0                   1               1    |  2700    385,7142857     771,4285714 128,5714286 6   |  2592    370,2857143 740,5714286     123,4285714 6   |");
            tmDBGPRINTEx(DEBUGLVL_BLAB, "|5        |    23            |  8        8           2       0       1               1       1           0           0                   1               0    |  2700    337,5           1350        112,5       12  |  2592    324         1296            108         12  |");
            tmDBGPRINTEx(DEBUGLVL_BLAB, "|xxxxxxxxx|    17            |  9        9           2,25    0       0               0       0           0           1                   0               0    |  2700    300             1200        150         8   |  2592    288         1152            144         8   |");
            tmDBGPRINTEx(DEBUGLVL_BLAB, "|8        |    19            | 10       10           2,5     0       1               0       1           0           0                   0               0    |  2700    270             1080        135         8   |  2592    259,2       1036,8          129,6       8   |");
            tmDBGPRINTEx(DEBUGLVL_BLAB, "+---------+------------------+--------------------------------------------------------------------------------------------------------------------------------+------------------------------------------------------+------------------------------------------------------+--------------------");
            break;
    }

    lo_control    = (dig_clkdsp_current            << 13)|
        (dig_clkdsp_pd_adc_nb_vih      << 12)|
        (dig_clkdsp_pd_dac_debug_vih   << 11)|
        (dig_clkdsp_pd_vih             << 10)|
        (ext_xtal_to_pad_ena           <<  9)|
        (dig_clkgen_pd_voh             <<  8)|
        (dig_lo_current                <<  6)|
        (dig_lo_inbuf_current          <<  4)|
        (dig_lo_mode                   <<  2)|
        (dig_lo_pd_div2_voh            <<  1)|
        (dig_lo_pd_div3_voh            <<  0);

    lo_control_sd = (dig_sd_current               << 4 )|
        (dig_sd_divratio              << 2 )|
        (dig_sd_en_test_voh           << 1 )|
        (dig_sd_pd_voh                << 0 );

    lo_control_us = (dig_us_current               << 10)|
        (dig_us_en_frac_mode_vih      << 9 )|
        (dig_us_en_integer_mode_vih   << 8 )|
        (dig_us_en_integer_vih        << 7 )|
        (dig_us_en_sw0_vih            << 6 )|
        (dig_us_en_sw1_vih            << 5 )|
        (dig_us_en_sw2_0_vih          << 4 )|
        (dig_us_en_sw2_1_vih          << 3 )|
        (dig_us_en_sw2_2_vih          << 2 )|
        (dig_us_en_test_vih           << 1 )|
        (dig_us_pd_vih                << 0 );

    /* Settings Prog */
    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_cgu_LO_CONTROL, lo_control);
    }
    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_cgu_LO_CONTROL_SD, lo_control_sd);
    }
    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_cgu_LO_CONTROL_US, lo_control_us);
    }

    if( err == TM_OK )
    {
        /* to increase the temperature area for 8GHz VCO */
        /* err = iTDA18280_I2CWriteBitField( pObj, BFtda18280_reg_crftech_pll_DEBUDCURSRC2__ctrlvco8icopytune100uoh, 7 );*/
    }

    /* Restart CAL PLL1 */
    if(err == TM_OK)
    {
        err = iTDA18280_BlockPLL1ProgramTempLoopPrecharge(pObj);
    }
    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_crftech_pll_SETUPFREQ__startcaloh, 0);
    }
    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_crftech_pll_SETUPFREQ__startcaloh, 1);
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_BlockLOChainStop                                    */
/*                                                                            */
/* DESCRIPTION: LO_Chain Programming                                          */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_BlockLOChainStop(ptmTDA18280_Object_t pObj)
{
    tmErrorCode_t   err = TM_OK;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    /* Clock Gen */
    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_cgu_LO_CONTROL__dig_clkgen_pd_voh, 1);
    }
    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_cgu_LO_CONTROL__dig_clkdsp_pd_adc_nb_vih, 1);
    }
    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_cgu_LO_CONTROL__dig_clkdsp_pd_dac_debug_vih, 1);
    }
    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_cgu_LO_CONTROL__dig_clkdsp_pd_vih, 1); /* cgu digital */
    }
    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_cgu_LO_CONTROL_SD__dig_sd_pd_voh, 1);
    }
    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_cgu_LO_CONTROL_US__dig_us_pd_vih, 1);
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_BlockPLL1Startup                                    */
/*                                                                            */
/* DESCRIPTION: lock_PLL1                                                     */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_BlockPLL1Startup(ptmTDA18280_Object_t       pObj   /* I: Driver object */)
{                                                                                                        
    tmErrorCode_t   err = TM_OK;
    UInt16 start_cal;
    UInt16 vco_select = 0;                                                                                        
    UInt16 divider_ratio = 96;                                                                                        
    UInt16 charge_pump_current = 56; 
    UInt16 lpf_bw = 5;                                                                                       
    UInt16 selvcobiasoh = 2;                                                                                       
    UInt16 ulValue;
    UInt32 RegMask = 0;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    /* PLL1 6G 8G OFF */
    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_cgu_PLL_CONTROL__synonmain, 1);
    }

    switch (pObj->PllCfg.pll1Freq)
    {
    case tmTDA18280_PLL1_5184MHz:
        tmDBGPRINTEx(DEBUGLVL_BLAB, "************************************************************************************");
        tmDBGPRINTEx(DEBUGLVL_BLAB, "* INFO --> VCO @ 5.184GHz & Icp=1.6mA & R1=6.076k     ...");
        tmDBGPRINTEx(DEBUGLVL_BLAB, "************************************************************************************");
        start_cal = 1;
        vco_select =  1;                                                                                        
        divider_ratio =  96;                                                                                        
        charge_pump_current =  56;                                                                                        
        lpf_bw = 5;                                                                                       
        selvcobiasoh = 2;
        break;
    case tmTDA18280_PLL1_5400MHz:
        tmDBGPRINTEx(DEBUGLVL_BLAB, "************************************************************************************");
        tmDBGPRINTEx(DEBUGLVL_BLAB, "* INFO --> VCO @ 5.4GHz & Icp=1.6mA & R1=6.076k   ...");
        tmDBGPRINTEx(DEBUGLVL_BLAB, "************************************************************************************");
        start_cal = 1;
        vco_select =  1;                                                                                        
        divider_ratio =  100;                                                                                        
        charge_pump_current =  56;                                                                                        
        lpf_bw = 5;                                                                                       
        selvcobiasoh = 2;
        break;
    case tmTDA18280_PLL1_7776MHz:
        tmDBGPRINTEx(DEBUGLVL_BLAB, "************************************************************************************");
        tmDBGPRINTEx(DEBUGLVL_BLAB, "* INFO --> VCO @ 7.776GHz & Icp=1.2mA & R1=8k   ...");
        tmDBGPRINTEx(DEBUGLVL_BLAB, "************************************************************************************");
        start_cal = 1;
        vco_select =  0;                                                                                        
        divider_ratio =  144;                                                                                        
        charge_pump_current =  63; 
        lpf_bw = 32;                                                                                       
        selvcobiasoh = 3;
        break;
    case tmTDA18280_PLL1_8100MHz:
        tmDBGPRINTEx(DEBUGLVL_BLAB, "************************************************************************************");
        tmDBGPRINTEx(DEBUGLVL_BLAB, "* INFO --> VCO @ 8.1GHz & Icp=1.2mA & R1=8k   ...");
        tmDBGPRINTEx(DEBUGLVL_BLAB, "************************************************************************************");
        start_cal = 1;
        vco_select =  0;                                                                                        
        divider_ratio =  150;                                                                                        
        charge_pump_current =  63; 
        lpf_bw = 34;                                                                                       
        selvcobiasoh = 3;
        break;
    default:
        err = TDA18280_ERR_NOT_INITIALIZED;
        break;
    }

    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_crftech_pll_TESTVCO1__entempdetectotoh,0);
    }
    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_crftech_pll_SETUPBW, (lpf_bw << 6) | charge_pump_current);
    }
    if(err == TM_OK)
    {
        err = iTDA18280_BlockPLL1ProgramTempLoopPrecharge(pObj);
    }
    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_crftech_pll_SETUPFREQ, (divider_ratio << 2) | (vco_select << 1) | 0);
    }
    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_crftech_pll_SETUPFREQ, (divider_ratio << 2) | (vco_select << 1) | start_cal);
    }

    /* patch for FAST configuration */
    /* set REGtda18280_reg_crftech_pll_DEBUDCURSRC2 */
    if(err == TM_OK)
    {
        err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_crftech_pll_DEBUDCURSRC2, 1, &ulValue);
    }

    if(err == TM_OK)
    {
        RegMask = ( (1 << tda18280_reg_crftech_pll_DEBUDCURSRC2__ctrlbias1maindivtune100uoh_WidthInBits) - 1);
        RegMask = RegMask << tda18280_reg_crftech_pll_DEBUDCURSRC2__ctrlbias1maindivtune100uoh_PositionInBits;

        ulValue &= ~RegMask;
        ulValue += 3 << tda18280_reg_crftech_pll_DEBUDCURSRC2__ctrlbias1maindivtune100uoh_PositionInBits;

        RegMask = ( (1 << tda18280_reg_crftech_pll_DEBUDCURSRC2__ctrlbias2maindivtune20uoh_WidthInBits) - 1);
        RegMask = RegMask << tda18280_reg_crftech_pll_DEBUDCURSRC2__ctrlbias2maindivtune20uoh_PositionInBits;

        ulValue &= ~RegMask;
        ulValue += 2 << tda18280_reg_crftech_pll_DEBUDCURSRC2__ctrlbias2maindivtune20uoh_PositionInBits;

        RegMask = ( (1 << tda18280_reg_crftech_pll_DEBUDCURSRC2__ctrlvco5icopytune100uoh_WidthInBits) - 1);
        RegMask = RegMask << tda18280_reg_crftech_pll_DEBUDCURSRC2__ctrlvco5icopytune100uoh_PositionInBits;

        ulValue &= ~RegMask;
        ulValue += 3 << tda18280_reg_crftech_pll_DEBUDCURSRC2__ctrlvco5icopytune100uoh_PositionInBits;


        RegMask = ( (1 << tda18280_reg_crftech_pll_DEBUDCURSRC2__ctrlvco8icopytune100uoh_WidthInBits) - 1);
        RegMask = RegMask << tda18280_reg_crftech_pll_DEBUDCURSRC2__ctrlvco8icopytune100uoh_PositionInBits;

        ulValue &= ~RegMask;
        ulValue += 3 << tda18280_reg_crftech_pll_DEBUDCURSRC2__ctrlvco8icopytune100uoh_PositionInBits;
    }

    /* patch : improve VCO8G functionality at high temperature */
    ulValue = 0x707C;

    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_crftech_pll_DEBUDCURSRC2, ulValue);
    }

    /* set REGtda18280_reg_crftech_pll_TESTVCO2 */
    if(err == TM_OK)
    {
        err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_crftech_pll_TESTVCO2, 1, &ulValue);
    }

    if(err == TM_OK)
    {
        RegMask = ( (1 << tda18280_reg_crftech_pll_TESTVCO2__selvcobiasoh_WidthInBits) - 1);
        RegMask = RegMask << tda18280_reg_crftech_pll_TESTVCO2__selvcobiasoh_PositionInBits;

        ulValue &= ~RegMask;
        ulValue += 3 << tda18280_reg_crftech_pll_TESTVCO2__selvcobiasoh_PositionInBits;
    }

    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_crftech_pll_TESTVCO2, ulValue);
    }
    /* end patch for FAST configuration */

    if(err == TM_OK)
    {
        err = iTDA18280_BlockPLL1CheckLock(pObj, 500, pObj->PllCfg.pll1Freq);
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_BlockPLL1CheckLock                                  */
/*                                                                            */
/* DESCRIPTION: lock_PLL1                                                     */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_BlockPLL1CheckLock(
                             ptmTDA18280_Object_t       pObj,   /* I: Driver object */
                             UInt16 max_delay_ms,
                             UInt32 wanted_freq_MHz
                             )
{
    tmErrorCode_t err   = TM_OK;
    UInt16 value16      = 0;
    UInt16 lock         = 1;
    const UInt16 step   = 50; /* step in millisecond */
    UInt16 i            = 0;
    UInt16 status       = 0;
    UInt16 eocoh        = 0;
    UInt16 lockinoh     = 0;
    UInt16 recaloh      = 0;
    UInt16 delay        = 0;
    UInt32 freq_vco_reg = 0;
    UInt32 tstlockinih  = 0;
    UInt32 freqvcoi     = 0;
    UInt32 freq_vco_MHz = 0;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);
    tmDBGPRINTEx(DEBUGLVL_BLAB, "  Wait for lock of PLL1 (lockinoh) ");

    while (lockinoh == 0)
    {
        if (err == TM_OK)
        {
            err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_cgu_PLL_STATUS, 1, &value16); /* Or enable IRQ in DIG_MB (12) */
            status  = value16 ;
        }

        eocoh    = (status >>2) & 0x1;
        lockinoh = (status >>1) & 0x1;
        recaloh  = (status >>0) & 0x1;
        if (lockinoh == 0)
        {
            iTDA18280_UtilWait(pObj, step);
            delay = step * i;
            i++;
        }

        if ((delay > max_delay_ms) || (err != TM_OK))
        {
            lockinoh = 1;
            lock = 0;
        }
    }

    if (lock == 1)
    {
        if (err == TM_OK)
        {
            err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_crftech_pll_READVCOFREQ, 1, &value16);
            freq_vco_reg  = value16;
        }
        if (err == TM_OK)
        {
            tstlockinih  = freq_vco_reg >> 15;
            freqvcoi = freq_vco_reg & 0x7FFF;
            freq_vco_MHz = (UInt32)((unsigned long)freqvcoi * TDA18280_VCO_CALCULATION_FACTOR_x1E6 / 1000000); /* in MHz */

            tmDBGPRINTEx(DEBUGLVL_BLAB, "   Wanted Frequency : %d MHz", wanted_freq_MHz);
            tmDBGPRINTEx(DEBUGLVL_BLAB, "   - readvcofreq      0x%0x (%d)", freq_vco_reg, freq_vco_reg);
            tmDBGPRINTEx(DEBUGLVL_BLAB, "   - tstlockinih      0x%0x (%d)", tstlockinih, tstlockinih);
            tmDBGPRINTEx(DEBUGLVL_BLAB, "   - freqvcoi         0x%0x (%d)", freqvcoi, freqvcoi);
            tmDBGPRINTEx(DEBUGLVL_BLAB, "   VCO Frequency : %d MHz", freq_vco_MHz);

            /* check wanted PLL frequency accuracy is within +/-1% */
            if ((100*wanted_freq_MHz > 99*freq_vco_MHz) && (100*wanted_freq_MHz < 101*freq_vco_MHz))
            {
                tmDBGPRINTEx(DEBUGLVL_BLAB, "   Wanted Frequency == Expected Frequency after calibration");
            }
            else
            {
                tmDBGPRINTEx(DEBUGLVL_BLAB, "   Wanted Frequency != Expected Frequency after calibration");
                lock = 0;
            }
        }
    }

    if (lock == 1)
    {
        tmDBGPRINTEx(DEBUGLVL_BLAB, "   PLL1 locked (%dms)", delay);
    }
    else if (delay > max_delay_ms)
    {
        tmDBGPRINTEx(DEBUGLVL_BLAB, "   PLL1 unlocked, Time over (>%dms)", max_delay_ms);
    }
    else
    {
        tmDBGPRINTEx(DEBUGLVL_BLAB, "   PLL1 unlocked");
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_BlockCGUAndResetInitWBNB                            */
/*                                                                            */
/* DESCRIPTION: Set all clocks (CGU/CLock gen), and release all reset         */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_BlockCGUAndResetInitWBNB(
                                  ptmTDA18280_Object_t       pObj   /* I: Driver object */
                                  )
{
    tmErrorCode_t   err = TM_OK;

    /* reg_cgu/global_cfg */
    UInt16 powerdown  = 0;
    UInt16 force_all  = 0;
    UInt16 reg_ena    = 1; /* enable register access */

    UInt16 global_cfg = \
        powerdown      << 2 |
        force_all      << 1 |
        reg_ena        << 0 ;

    /* reg_cgu/clocks_cfg */
    UInt16 dbg_dac_ena         = 0;
    UInt16 dbg_dac_inv         = 0;
    UInt16 sci_refclk_inv      = 0;
    UInt16 sci_refclk_ena      = 0;
    UInt16 demod_core_sel      = 0;
    UInt16 demod_inp_sel       = 0;
    UInt16 nb_ddc_ena          = 0;
    UInt16 nb_adc_fast_select  = 0;
    UInt16 nb_adc_ena          = 0;
    UInt16 nb_adc_inv          = 0;
    UInt16 adc_rfif_ena        = 0;
    UInt16 adc_rfif_inv        = 0;
    UInt16 rfdsp_ena           = 0;
    UInt16 rfagc_ena           = 0;
    UInt16 dbg_ram_ena         = 0;

    UInt16 clocks_cfg = \
        dbg_dac_ena             <<14 |
        dbg_dac_inv             <<13 |
        sci_refclk_inv          <<12 |
        sci_refclk_ena          <<11 |
        demod_core_sel          <<10 |
        demod_inp_sel           << 9 |
        nb_ddc_ena              << 8 |
        nb_adc_fast_select      << 7 |
        nb_adc_ena              << 6 |
        nb_adc_inv              << 5 |
        adc_rfif_ena            << 4 |
        adc_rfif_inv            << 3 |
        rfdsp_ena               << 2 |
        rfagc_ena               << 1 |
        dbg_ram_ena             << 0;

    /* reg_reset/reset_gestion */
    UInt16 config           = 1;
#ifdef SIMU_RTL
    UInt16 adc              = 0;
#else
    UInt16 adc              = 0;
#endif
    UInt16 nc               = 0;

    UInt16 reset_gestion = \
        config      << 2 |
        adc         << 1 |
        nc          << 0;

    /* reg_reset/reset_dwn_dsp */
    UInt16 dbg_dac       = 0;
    UInt16 nb_ddc        = 0;
    UInt16 ts            = 0;
    UInt16 rfif          = 0;
    UInt16 dbg_ram       = 0;
    UInt16 rfagc         = 0;
    UInt16 rfdsp         = 0;
    UInt16 cross_switch  = 0;
    UInt16 nb_adc        = 0;
    UInt16 reset_dwn_dsp = \
        dbg_dac         << 8 |
        nb_ddc          << 7 |
        ts              << 6 |
        rfif            << 5 |
        dbg_ram         << 4 |
        rfagc           << 3 |
        rfdsp           << 2 |
        cross_switch    << 1 |
        nb_adc          << 0;

    /* reg_reset/reset_ddc */
    UInt16 chstk        = 0;
    UInt16 octo_ddc     = 0;
    UInt16 reset_ddc = \
        chstk       << 1 |
        octo_ddc    << 0;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    /* CGU */
    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_cgu_GLOBAL_CFG, global_cfg);
    }
    tmDBGPRINTEx(DEBUGLVL_BLAB, "   - CGU power up");

    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_cgu_CLOCKS_CFG, clocks_cfg);
    }
    tmDBGPRINTEx(DEBUGLVL_BLAB, "     . clocks_cfg ");

    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_cgu_CLOCKS_DEMOD_CFG, 0);
    }
    tmDBGPRINTEx(DEBUGLVL_BLAB, "     . clocks_demod_cfg ");

    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_cgu_CLOCKS_DWN_DDC_CFG, 0);
    }
    tmDBGPRINTEx(DEBUGLVL_BLAB, "     . clocks_dwn_ddc_cfg ");

    if(err == TM_OK)
    {
        /* program Narrow Band ADC */
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_cgu_CLOCKS_NB_ADC_PRG_CFG, pObj->PllCfg.deciDDC);
    }
    tmDBGPRINTEx(DEBUGLVL_BLAB, "     . clocks_nb_adc_prg_cfg ");

    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_cgu_CLOCKS_TS_SER_OUT_CFG, 0);
    }
    tmDBGPRINTEx(DEBUGLVL_BLAB, "     . clocks_ts_ser_out_cfg ");

    /* Release Reset for Modules */
    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_reset_RESET_GESTION, reset_gestion );
    }

    tmDBGPRINTEx(DEBUGLVL_BLAB, "   - reset_demod : ");
    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_reset_RESET_DEMOD, 0);
    }

    tmDBGPRINTEx(DEBUGLVL_BLAB, "   - reset_ddc : ");
    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_reset_RESET_DDC, reset_ddc);
    }

    tmDBGPRINTEx(DEBUGLVL_BLAB, "   - reset_dwn_dsp : ");
    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_reset_RESET_DWN_DSP, reset_dwn_dsp );
    }

    tmDBGPRINTEx(DEBUGLVL_BLAB, "   - ts outputs : ");
    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_config_TS_SYNC_EN, 0xFF);
    }
    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_config_TS_DATA_EN, 0xFF);
    }
    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_config_TS_CLK_EN, 0xFF);
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_BlockCGUAndResetStopWBNB                            */
/*                                                                            */
/* DESCRIPTION: Cut all clocks (CGU/CLock gen), and set all reset             */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_BlockCGUAndResetStopWBNB(
                               ptmTDA18280_Object_t       pObj   /* I: Driver object */
                               )
{
    tmErrorCode_t   err = TM_OK;

    /* reg_cgu/global_cfg */
    UInt16 powerdown    = 1;
    UInt16 force_all    = 0;
    UInt16 reg_ena      = 0; /* disable register access */
    UInt16 global_cfg = \
        powerdown      << 2 |
        force_all      << 1 |
        reg_ena        << 0 ;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    /* Reset all blocks except ADC */
    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_reset_RESET_GESTION, 7);
    }
    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_reset_RESET_DWN_DSP, 0);
    }
    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_reset_RESET_DDC, 0);
    }

    /* CLOCKS */

    /* CGU */
    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_cgu_GLOBAL_CFG, global_cfg);
    }
    tmDBGPRINTEx(DEBUGLVL_BLAB, "   - CGU power down");

    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_cgu_CLOCKS_CFG, 0);
    }

    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_cgu_CLOCKS_DWN_DDC_CFG, 0);
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_BlockCGUAndResetStartupWB                           */
/*                                                                            */
/* DESCRIPTION: Set all clocks (CGU/CLock gen), and release all reset         */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_BlockCGUAndResetStartupWB(
                                  ptmTDA18280_Object_t       pObj   /* I: Driver object */
                                  )
{
    tmErrorCode_t   err = TM_OK;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    /* enable RF interface */
    if (err == TM_OK)
    {
        err = iTDA18280_I2CWriteBitField (pObj, BFtda18280_reg_cgu_CLOCKS_CFG__adc_rfif_ena, 1);
    }
    /* toggle RFIF reset */
    if (err == TM_OK)
    {
        err = iTDA18280_I2CWriteBitField (pObj, BFtda18280_reg_reset_RESET_DWN_DSP__rfif, 0);
    }
    if (err == TM_OK)
    {
        err = iTDA18280_I2CWriteBitField (pObj, BFtda18280_reg_reset_RESET_DWN_DSP__rfif, 1);
    }

    /* enable RF DSP */
    if (err == TM_OK)
    {
        err = iTDA18280_I2CWriteBitField (pObj, BFtda18280_reg_cgu_CLOCKS_CFG__rfdsp_ena, 1);
    }
    /* toggle RF DSP reset */
    if (err == TM_OK)
    {
        err = iTDA18280_I2CWriteBitField (pObj, BFtda18280_reg_reset_RESET_DWN_DSP__rfdsp, 0);
    }
    if (err == TM_OK)
    {
        err = iTDA18280_I2CWriteBitField (pObj, BFtda18280_reg_reset_RESET_DWN_DSP__rfdsp, 1);
    }

    ///* Enable RF DSP power measurement */
    //if (err == TM_OK )
    //{
    //    /* activate powana */
    //    err = iTDA18280_I2CWriteBitField (pObj, BFtda18280_reg_rfdsp_RFDSP_POWANA_CFG__activate, 1 );
    //}

    //if (err == TM_OK )
    //{
    //    /* enable the indicator */
    //    err = iTDA18280_I2CWriteBitField (pObj, BFtda18280_reg_rfdsp_RFDSP_POWANA_CFG__indicator_upd, 1 );
    //}

    //if( err == TM_OK )
    //{
    //    /* enable the interrupt for tilt detection */
    //    err = iTDA18280_I2CWriteRegister (pObj, REGtda18280_reg_rfdsp_RFDSP_POWANA_EVENTS_ENABLE, 0xFFFF);
    //}

    return err;
}


/*============================================================================*/
/* FUNCTION:    iTDA18280_BlockCGUAndResetStopWB                              */
/*                                                                            */
/* DESCRIPTION: Reset all clocks (CGU/CLock gen)                              */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_BlockCGUAndResetStopWB(
                                  ptmTDA18280_Object_t       pObj   /* I: Driver object */
                                  )
{
    tmErrorCode_t   err = TM_OK;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    /* disable RF interface */
    if (err == TM_OK)
    {
        err = iTDA18280_I2CWriteBitField (pObj, BFtda18280_reg_cgu_CLOCKS_CFG__adc_rfif_ena, 0);
    }

    /* disable RF DSP */
    if (err == TM_OK)
    {
        err = iTDA18280_I2CWriteBitField (pObj, BFtda18280_reg_cgu_CLOCKS_CFG__rfdsp_ena, 0);
    }

    /* disable RF DSP power measurement */
    if (err == TM_OK)
    {
        /* deactivate powana */
        err = iTDA18280_I2CWriteBitField (pObj, BFtda18280_reg_rfdsp_RFDSP_POWANA_CFG__activate, 0);
    }

    if (err == TM_OK)
    {
        /* disable the indicator */
        err = iTDA18280_I2CWriteBitField (pObj, BFtda18280_reg_rfdsp_RFDSP_POWANA_CFG__indicator_upd, 0);
    }

    if( err == TM_OK)
    {
        /* disable the interrupt for tilt detection */
        err = iTDA18280_I2CWriteRegister (pObj, REGtda18280_reg_rfdsp_RFDSP_POWANA_EVENTS_ENABLE, 0x0000);
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_PathDdcClockCheck                                   */
/*                                                                            */
/* DESCRIPTION: Check Clocks                                                  */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_PathDdcClockCheck(
                         ptmTDA18280_Object_t  pObj,      /* I: Driver object */
                         tmTDA18280_Path_t     Idx_path,  /* I: Path indicator */
                         UInt8*                pClock_OK  /* O: 0 clock off, 1 clock on */
                         )
{
    tmErrorCode_t   err = TM_OK;
    UInt16          reg_ena, value16 = 0;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    /* initialize clock to off to handle errors */
    if ((err == TM_OK) && (pClock_OK != Null))
    {
        *pClock_OK = 0;
    }
    else
    {
        err = TDA18280_ERR_BAD_PARAMETER;
    }

    /* check reg_ena */
    /* if reg_ena is enabled, reset is assumed to be released also */
    if (err == TM_OK)
    {
        err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_cgu_GLOBAL_CFG__reg_ena, &reg_ena);
    }

    if ((err == TM_OK) && (reg_ena == 1))
    {
        if (Idx_path < tmTDA18280_Path_Aux)
        {
            if(err == TM_OK)
            {
                err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_cgu_CLOCKS_DWN_DDC_CFG, 1, &value16);
            }
            if(err == TM_OK)
            {
                value16 = (value16 & (1 << (UInt16)Idx_path));
                if (value16 != 0)
                {
                    *pClock_OK = 1;
                }
                else
                {
                    *pClock_OK = 0;
                }
            }
        }
        else if (Idx_path==tmTDA18280_Path_Aux) /* Auxiliary path */
        {
            if(err == TM_OK)
            {
                err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_cgu_CLOCKS_CFG__nb_ddc_ena, &value16);
            }
            if(err == TM_OK)
            {
                if (value16 == 1)
                {
                    *pClock_OK = 1;
                }
                else
                {
                    *pClock_OK = 0;
                }
            }
        }
        else
        {
            tmDBGPRINTEx(DEBUGLVL_BLAB, "!! ERROR !!: Path %d not supported", (UInt16)Idx_path);
            err = TDA18280_ERR_NOT_INITIALIZED;
        }
    }
    return err;
}


/*============================================================================*/
/* FUNCTION:    iTDA18280_PathDdcStartup                                      */
/*                                                                            */
/* DESCRIPTION: Enable DDC                                                    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_PathDdcStartup(
                           ptmTDA18280_Object_t    pObj,     /* I: Driver object */
                           tmTDA18280_Path_t       Idx_path  /* I: Path indicator */
                           )
{
    tmErrorCode_t   err = TM_OK;
    UInt16          value16 = 0;
    Bool            bEnableDAC = False;
    tmTDA18280_DACConfiguration_t DACCfg;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    if (Idx_path < tmTDA18280_Path_Aux)
    {
        /* release the common reset of all WB DDCs */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_reset_RESET_DDC__octo_ddc, 1);
        }
        /* enable clock of WB DDC */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_cgu_CLOCKS_DWN_DDC_CFG, 1, &value16);
        }
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_cgu_CLOCKS_DWN_DDC_CFG, value16 | (1 << (UInt16)Idx_path));
        }
        /* apply soft reset on slow clock (before fast clock) */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteRegister(pObj, REGBO(tda18280_reg_ddc_slow0_POWERDOWN, (UInt16)(2*Idx_path)), 3);
        }
        /* apply soft reset on fast clock (after slow clock) */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteRegister(pObj, REGBO(tda18280_reg_ddc_fast0_POWERDOWN, (UInt16)(2*Idx_path)), 3);
        }

        /* manage DAC and channel stacker */
        if (pObj->tFeature.bDACSupported && Idx_path >= tmTDA18280_Path_4 && Idx_path <= tmTDA18280_Path_7)
        {
            /* switch on DAC when all 4 channels are currently OFF */
            bEnableDAC = (pObj->PathCurrentState[tmTDA18280_Path_4] == tmTDA18280_OFF) &&
                         (pObj->PathCurrentState[tmTDA18280_Path_5] == tmTDA18280_OFF) &&
                         (pObj->PathCurrentState[tmTDA18280_Path_6] == tmTDA18280_OFF) &&
                         (pObj->PathCurrentState[tmTDA18280_Path_7] == tmTDA18280_OFF);
            if (bEnableDAC)
            {
                /* enable DAC and channel stacker */
                /* can not call iTDA18280_BlockDAC_SetCfg() here because value of CoarseTuning, FineTuning and InternalRefResistor  */
                /* are overwritten by HW  */
                if (err == TM_OK)
                {
                    err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_cgu_LO_CONTROL__dig_clkdsp_pd_dac_debug_vih, 0);
                }

                /* enable DAC clock */
                if (err == TM_OK)
                {
                    err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_cgu_CLOCKS_CFG__dbg_dac_ena, 1);
                }
                /* release DAC reset */
                if (err == TM_OK)
                {
                    err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_reset_RESET_DWN_DSP__dbg_dac, 1);
                }
                /* enable channel stacker clock */
                if (err == TM_OK)
                {
                    err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_cgu_CLOCKS_DWN_DDC_CFG__chstk_ena, 1);
                }
                /* release channel stacker reset */
                if (err == TM_OK)
                {
                    err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_reset_RESET_DDC__chstk, 1);
                }
                if (err == TM_OK)
                {
                    err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_channel_stacker_PD_TETRA_DAC__pd_refbg, 0);
                }
                if (err == TM_OK)
                {
                    err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_channel_stacker_PD_TETRA_DAC__pd_dac1, 0);
                }
                if (err == TM_OK)
                {
                    err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_channel_stacker_CONFIG__activate, 1);
                }
                if (err == TM_OK)
                {
                    DACCfg.eEnable = tmTDA18280_ON;
                    DACCfg.ulCoarseTuning = 1;
                    DACCfg.ulFineTuning = 10;
                    DACCfg.ulInternalRefResistor = 5;

                    err = iTDA18280_BlockDAC_SetCfg(pObj,&DACCfg);
                }
            }
            /* enable selected channel */
            if (err == TM_OK)
            {
                switch (Idx_path) {
                    case tmTDA18280_Path_4:
                        err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_channel_stacker_CONFIG__ena_1st_ch, 1);
                        break;
                    case tmTDA18280_Path_5:
                        err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_channel_stacker_CONFIG__ena_2nd_ch, 1);
                        break;
                    case tmTDA18280_Path_6:
                        err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_channel_stacker_CONFIG__ena_3rd_ch, 1);
                        break;
                    case tmTDA18280_Path_7:
                        err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_channel_stacker_CONFIG__ena_4th_ch, 1);
                        break;
                    default:
                        err = TDA18280_ERR_NOT_SUPPORTED;
                        break;
                }
            }
        }
        /* release cross switch reset */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_reset_RESET_DWN_DSP__cross_switch, 1);
        }
    }
    else if (Idx_path == tmTDA18280_Path_Aux) /* Auxiliary path */
    {
        /* enable clocks of NB ADC and DDC */
        if (err == TM_OK)
        {
            err = iTDA18280_NbAdcConfiguration(pObj, tmTDA18280_ON);
        }
    }
    else
    {
        tmDBGPRINTEx(DEBUGLVL_BLAB, "!! ERROR !!: Path %d not supported", (UInt16)Idx_path);
        err = TDA18280_ERR_NOT_INITIALIZED;
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_PathDdcStop                                         */
/*                                                                            */
/* DESCRIPTION: Disable DDC                                                   */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_PathDdcStop(
                        ptmTDA18280_Object_t    pObj,     /* I: Driver object */
                        tmTDA18280_Path_t       Idx_path  /* I: Path indicator */
                        )
{
    tmErrorCode_t   err = TM_OK;
    UInt16          value16 = 0;
    Bool            bDisableDAC = False;
    UInt8 DdcLoop = (tmTDA18280_Path_t)tmTDA18280_Path_Max;
    UInt8           uNbOfEnabledDdc = 0;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    if (Idx_path < tmTDA18280_Path_Aux)
    {
        /* manage DAC and channel stacker */
        /* before switching off DDC clock */
        if (pObj->tFeature.bDACSupported && Idx_path >= tmTDA18280_Path_4 && Idx_path <= tmTDA18280_Path_7)
        {
            /* disable selected channel */
            if (err == TM_OK)
            {
                switch (Idx_path) {
                    case tmTDA18280_Path_4:
                        err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_channel_stacker_CONFIG__ena_1st_ch, 0);
                        bDisableDAC = (pObj->PathCurrentState[tmTDA18280_Path_5] == tmTDA18280_OFF) &&
                                      (pObj->PathCurrentState[tmTDA18280_Path_6] == tmTDA18280_OFF) &&
                                      (pObj->PathCurrentState[tmTDA18280_Path_7] == tmTDA18280_OFF);
                        break;
                    case tmTDA18280_Path_5:
                        err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_channel_stacker_CONFIG__ena_2nd_ch, 0);
                        bDisableDAC = (pObj->PathCurrentState[tmTDA18280_Path_4] == tmTDA18280_OFF) &&
                                      (pObj->PathCurrentState[tmTDA18280_Path_6] == tmTDA18280_OFF) &&
                                      (pObj->PathCurrentState[tmTDA18280_Path_7] == tmTDA18280_OFF);
                        break;
                    case tmTDA18280_Path_6:
                        err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_channel_stacker_CONFIG__ena_3rd_ch, 0);
                        bDisableDAC = (pObj->PathCurrentState[tmTDA18280_Path_4] == tmTDA18280_OFF) &&
                                      (pObj->PathCurrentState[tmTDA18280_Path_5] == tmTDA18280_OFF) &&
                                      (pObj->PathCurrentState[tmTDA18280_Path_7] == tmTDA18280_OFF);
                        break;
                    case tmTDA18280_Path_7:
                        err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_channel_stacker_CONFIG__ena_4th_ch, 0);
                        bDisableDAC = (pObj->PathCurrentState[tmTDA18280_Path_4] == tmTDA18280_OFF) &&
                                      (pObj->PathCurrentState[tmTDA18280_Path_5] == tmTDA18280_OFF) &&
                                      (pObj->PathCurrentState[tmTDA18280_Path_6] == tmTDA18280_OFF);
                        break;
                    default:
                        err = TDA18280_ERR_NOT_SUPPORTED;
                        break;
                }
            }
            /* if all 4 channels will be OFF, switch off also the DAC*/
            if (bDisableDAC)
            {
                /* disable DAC and channel stacker */
                if (err == TM_OK)
                {
                    err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_channel_stacker_CONFIG__activate, 0);
                }
                if (err == TM_OK)
                {
                    err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_channel_stacker_PD_TETRA_DAC__pd_refbg, 1);
                }
                if (err == TM_OK)
                {
                    err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_channel_stacker_PD_TETRA_DAC__pd_dac1, 1);
                }
                /* activate DAC reset */
                if (err == TM_OK)
                {
                    err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_reset_RESET_DWN_DSP__dbg_dac, 0);
                }
                /* disable DAC clock */
                if (err == TM_OK)
                {
                    err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_cgu_CLOCKS_CFG__dbg_dac_ena, 0);
                }
                /* activate channel stacker reset */
                if (err == TM_OK)
                {
                    err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_reset_RESET_DDC__chstk, 0);
                }
                /* disable channel stacker clock */
                if (err == TM_OK)
                {
                    err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_cgu_CLOCKS_DWN_DDC_CFG__chstk_ena, 0);
                }
                if (err == TM_OK)
                {
                    err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_cgu_LO_CONTROL__dig_clkdsp_pd_dac_debug_vih, 1);
                }
            }
        }

        /* disable clock of WB DDC */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_cgu_CLOCKS_DWN_DDC_CFG, 1, &value16);
        }
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_cgu_CLOCKS_DWN_DDC_CFG, value16 & (~(1 << (UInt16)Idx_path)));
        }

        /* count the number of enabled DDCs, except the current one */
        for (DdcLoop = tmTDA18280_Path_0; DdcLoop <= tmTDA18280_Path_7; DdcLoop++)
        {
            if (DdcLoop != Idx_path)
            {
                /* add 1 for each enabled DDC */
                uNbOfEnabledDdc += ((pObj->PathCurrentState[DdcLoop] == tmTDA18280_OFF)?0:1);
            }
        }
        /* if all 8 DDCs will be OFF, force the reset */
        if (uNbOfEnabledDdc == 0)
        {
            /* activate the common reset of all WB DDCs */
            if (err == TM_OK)
            {
                err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_reset_RESET_DDC__octo_ddc, 0);
            }
            /* activate cross switch reset */
            if (err == TM_OK)
            {
                err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_reset_RESET_DWN_DSP__cross_switch, 0);
            }
        }
    }
    else if (Idx_path == tmTDA18280_Path_Aux) /* Auxiliary path */
    {
        /* disable clocks of NB ADC and DDC */
        if (err == TM_OK)
        {
            err = iTDA18280_NbAdcConfiguration( pObj, tmTDA18280_OFF );
        }
    }
    else
    {       
        tmDBGPRINTEx(DEBUGLVL_BLAB, "!! ERROR !!: Path %d not supported", (UInt16)Idx_path);
        err = TDA18280_ERR_NOT_INITIALIZED;
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_PathConfig                                          */
/*                                                                            */
/* DESCRIPTION: Config path for selected parameters                           */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_PathConfig(
                     ptmTDA18280_Object_t        pObj,       /* I: Driver object */
                     UInt8                       Idx_ddc,    /* I: index of the path */
                     const tmTDA18280_PathCfg_t* ptPathCfg,  /* I: Pointer to Path Configuration structure. */
                     TDA18280_PathSetAGCMode_t   eMode       /* I: Programming mode (in calibration mode, it is mandatory to set back AGC mode to normal afterward) */
                     )
{
    tmErrorCode_t   err = TM_OK;

    UInt32 F_adc = 0;
    UInt32 F_adc_div4 = 0;
    UInt32 F_rf = ptPathCfg->uRF;
    UInt16 lpf_select = 0x0;
    Int32 freq;
    Int32 ddc_cordic_freq;
    UInt16 cfg_cross_switch;
    UInt32 cordic_prog;
    UInt16 Idx_rfdsp_output;
    UInt16 gain_reg;
    Int32 lValue;
#ifndef TMBSL_TDA18280_NO_64BITS
    long long llValue;
#endif


//add by wll

	/* set accuracy to 1KHz */
	
		F_rf /= 1000;
	
		F_rf *= 1000;
	
//end add 


    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    /* Set Dig AGC in fast mode */
    if (err == TM_OK)
    {
        err = iTDA18280_SetDigAgcMode(pObj, Idx_ddc, tmTDA18280_AgcMode_Fast);
    }

    if (pObj->PllCfg.pll1Freq < tmTDA18280_PLL1_7776MHz)
    {
        F_adc = pObj->PllCfg.pll1Freq/2*1000000; /* in Hz */
    }
    else
    {
        F_adc = pObj->PllCfg.pll1Freq/3*1000000; /* in Hz */
    }
    F_adc_div4 = F_adc/4;

    /* Cordic freq and cross_switch configuration calculation */
    if (F_rf > F_adc_div4)
    {
        Idx_rfdsp_output = 0x4;
        freq = F_rf - ((F_adc/8)*3);
    }
    else
    {
        Idx_rfdsp_output = 0x0;
        freq = F_rf - (F_adc/8);
    }

    if (freq > 0) 
    {
        Idx_rfdsp_output += 0x0;
        freq = freq - (F_adc/16);
    }
    else
    {
        Idx_rfdsp_output += 0x2;
        freq = freq + (F_adc/16);
    }

    if (freq > 0)
    {
        Idx_rfdsp_output += 0x0;
        freq = freq - (F_adc/32);
    }
    else
    {
        Idx_rfdsp_output += 0x1;
        freq = freq + (F_adc/32);
    }

    cfg_cross_switch = (Idx_rfdsp_output << 1) + 1;
    /* cordic programming */
    /* note: register value is a signed value */
   // ddc_cordic_freq = TDA18280_DEMOD_IF - freq;
#ifndef TMBSL_TDA18280_NO_64BITS
    /* 64-bit division allowed */
    llValue = ((long long)(1 << 25)*2*ddc_cordic_freq)/F_adc_div4;
    lValue = (Int32)llValue;
#else
    /* option to avoid 64-bit division with long long */
    /* but at the expense of the accuracy (1KHz) */
//add by wll
    
    ddc_cordic_freq = TDA18280_DEMOD_IF - freq;
    ddc_cordic_freq /= 1000;
    F_adc_div4 /= 1000;
    lValue = (Int32)(1 << 13)*2*ddc_cordic_freq;
    lValue = lValue/100;
    lValue = (1 << 7)*lValue;
    lValue = lValue/10/5;
    lValue = (1 << 5)*lValue;
    F_adc_div4 = F_adc_div4/1000/5;
    lValue = lValue/(Int32)F_adc_div4;
#endif
    cordic_prog = (UInt32)(lValue);

    tmDBGPRINTEx(DEBUGLVL_BLAB, "     cordic_prog......................: %u", cordic_prog);
    tmDBGPRINTEx(DEBUGLVL_BLAB, "     RFDSP output frequency...........: %d Hz", freq);

    /* cross_switch programming */
    if (err == TM_OK)
    {
        err = iTDA18280_BlockCrossSwitchProg(pObj, Idx_ddc, cfg_cross_switch);
    }

    if (ptPathCfg->tGainAutoMode == tmTDA18280_ON)
    {
        tmDBGPRINTEx(DEBUGLVL_BLAB, "     Gain Auto");
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFBO(tda18280_reg_ddc_slow0_GLOBAL_CONFIG__gain_select, 2*Idx_ddc), 0);
        }
    }
    else /* (ptPathCfg->tGainAutoMode == tmTDA18280_OFF) */
    {
        /* Gain calculation and programming */
        gain_reg = iTDA18280_CalculateGainRegValue(ptPathCfg->uGainMultBy10);

        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFBO(tda18280_reg_ddc_slow0_GLOBAL_CONFIG__gain_select, 2*Idx_ddc), 1);
        }
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFBO(tda18280_reg_ddc_slow0_GAIN_MANUAL, 2*Idx_ddc), gain_reg);
        }
    }

    /* Low pass Filter (LPF) programming        */
    /* choose between sets of taps              */
    /* ES1 :                                    */
    /* 00: 6MHz BW (def)                        */
    /* 01: 8MHz BW                              */
    /*                                          */
    /* ES2 :                                    */
    /* 00: 6MHz BW (def)                        */
    /* 01: 8MHz BW                              */
    /* 10: 5MHz BW                              */
    /* 11: 7MHz BW                              */
    /*                                          */
    /* if filter selected is not supported by   */
    /* ES version, driver will return           */
    /* TDA18280_ERR_NOT_SUPPORTED               */
    /*                                          */
    if (pObj->EsVersion == tmTDA18280_EsVersion1)
    {
        switch (ptPathCfg->tBW)
        {
        case tmTDA18280_6MHz:
            lpf_select = 0x0;
            break;

        case tmTDA18280_8MHz:
            lpf_select = 0x1;
            break;

        case tmTDA18280_2MHz:
        case tmTDA18280_5MHz:
        case tmTDA18280_7MHz:
            err = TDA18280_ERR_NOT_SUPPORTED;
            break;
        }
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFBO(tda18280_reg_ddc_slow0_GLOBAL_CONFIG__sel_deci6lpf, 2*Idx_ddc ), lpf_select);
        }
    }
    else /* (pObj->EsVersion == tmTDA18280_EsVersion2) */
    {
        /* CDP: filters 5/6/7/8MHz allowed */
        switch (ptPathCfg->tBW)
        {
        case tmTDA18280_5MHz:
            lpf_select = 0x2;
            break;

        case tmTDA18280_6MHz:
            lpf_select = 0x0;
            break;

        case tmTDA18280_7MHz:
            lpf_select = 0x3;
            break;

        case tmTDA18280_8MHz:
            lpf_select = 0x1;
            break;

        case tmTDA18280_2MHz:
            /* currently the 2MHz filter cannot be used due to some HW limitations */
            /* use 5MHz instead for OOB */
            err = TDA18280_ERR_NOT_SUPPORTED;
            break;
        }

        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFBO(tda18280_reg_ddc_slow0_GLOBAL_CONFIG__sel_deci6lpf, 2*Idx_ddc), lpf_select);
        }
    }

    /* Cordic (Digital tone synthetizer) frequency programming */
    if (err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGBO(tda18280_reg_ddc_fast0_COR_FREQ_MSB, 2*Idx_ddc), (cordic_prog >> 16) & 0x1ff);
    }
    if (err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGBO(tda18280_reg_ddc_fast0_COR_FREQ_LSB, 2*Idx_ddc), cordic_prog        & 0xffff);
    }
    if (err == TM_OK)
    {
        err = iTDA18280_I2CWriteBitField(pObj, BFBO(tda18280_reg_ddc_fast0_COR_FREQ_LOAD__load, 2*Idx_ddc), 1);
    }

    if (eMode == tmTDA18280_PathSetAGCMode_Normal)
    {
        /* wait stabilisation of DIG AGC */
        if (err == TM_OK)
        {
            err = iTDA18280_UtilWait(pObj, 10);
        }

        /* Set Dig AGC in normal mode */
        if (err == TM_OK)
        {
            err = iTDA18280_SetDigAgcMode(pObj, Idx_ddc, tmTDA18280_AgcMode_Normal);
        }
    }
    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_PathAuxConfig                                       */
/*                                                                            */
/* DESCRIPTION: Config auxiliary path                                        */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_PathAuxConfig(
                        ptmTDA18280_Object_t        pObj,       /* I: Driver object */
                        const tmTDA18280_PathCfg_t* ptPathCfg   /**< I: Pointer to Path Configuration structure. */                 
                        )
{
    tmErrorCode_t   err = TM_OK;

    UInt32 F_adc = 0;
    UInt32 cordic_prog;
    Int32 F_in;
    UInt16 usValue;
    Int32 lValue;
#ifndef TMBSL_TDA18280_NO_64BITS
    long long llValue;
#endif
    UInt16 gain_reg;
    UInt16 filter_select = 0;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    /* Set Dig AGC in fast mode */
    if (err == TM_OK)
    {
        err = iTDA18280_SetDigAgcMode(pObj, tmTDA18280_Path_Aux, tmTDA18280_AgcMode_Fast);
    }

    /* calculate freq ADC */
    if (pObj->PllCfg.pll1Freq < tmTDA18280_PLL1_7776MHz)
    {
        /* F_adc = pObj->PllCfg.pll1Freq/2 */
        /* F_adc /= (4*pObj->PllCfg.deciDDC) */

        F_adc = pObj->PllCfg.pll1Freq*1000/(8*pObj->PllCfg.deciDDC); /* in KHz */
        F_adc *= 1000; /* in Hz */
    }
    else
    {
        /* F_adc = pObj->PllCfg.pll1Freq/3 */
        /* F_adc /= (4*pObj->PllCfg.deciDDC) */

        F_adc = pObj->PllCfg.pll1Freq*1000/(12*pObj->PllCfg.deciDDC); /* in KHz */
        F_adc *= 1000; /* in Hz */
    }

    /* for NB ADC sample freq set to 27MHz, F ADC must be divided by 2 */
    if (pObj->PllCfg.NBADCSampleFreq == tmTDA18280_NBADCSampleFreq28MHz125)
    {
        F_adc /= 2;
    }

    /* IF frequency */
    F_in = TDA18280_DEMOD_IF - ptPathCfg->uRF; /* in Hz */

    /* cordic programming */
    /* note: register value is a signed value */
#ifndef TMBSL_TDA18280_NO_64BITS
    /* 64-bit division allowed */
    llValue = (long long)(1 << 24)*F_in/F_adc;
    lValue = (Int32)llValue;
#else
    /* option to avoid 64-bit division with long long */
    /* but at the expense of the accuracy (1KHz) */
    F_in /= 1000;
    F_adc /= 1000;
    lValue = (1 << 16)*F_in;
    lValue = lValue/25;
    lValue = (1 << 5)*lValue;
    lValue = lValue/25;
    lValue = (1 << 3)*lValue;
    F_adc = F_adc/25/25;
    lValue = lValue/(Int32)F_adc;
#endif
    cordic_prog = (UInt32)(lValue);

    tmDBGPRINTEx(DEBUGLVL_BLAB, "     cordic_prog......................: %u", cordic_prog);

    /* Cordic (Digital tone synthetizer) frequency programming */
    if (err == TM_OK)
    {
        usValue = (cordic_prog >> 16) & 0xff;
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_aux_ddc_CORDIC_FREQ_MSB, usValue);
    }

    if (err == TM_OK)
    {
        usValue = cordic_prog & 0xffff;
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_aux_ddc_CORDIC_FREQ_LSB, usValue);
    }

    if (ptPathCfg->tGainAutoMode == tmTDA18280_ON)
    {
        tmDBGPRINTEx(DEBUGLVL_BLAB, "     Gain Auto");
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_aux_ddc_DAGC_GAIN__gain_select, 0);
        }
    }
    else /* (ptPathCfg->tGainAutoMode == tmTDA18280_OFF) */
    {
        /* Gain calculation and programming */
        gain_reg = iTDA18280_CalculateGainRegValue(ptPathCfg->uGainMultBy10);

        if(err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_aux_ddc_DAGC_GAIN__gain_select, 1);
        }
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_aux_ddc_DAGC_GAIN__manual_gain, gain_reg);
        }
    }

    /* wait stabilisation of DIG AGC */
    if (err == TM_OK)
    {
        err = iTDA18280_UtilWait(pObj, 10);
    }

    /* Set Dig AGC in normal mode */
    if (err == TM_OK)
    {
        err = iTDA18280_SetDigAgcMode(pObj, tmTDA18280_Path_Aux, tmTDA18280_AgcMode_Normal);
    }

    /* Low pass Filter (LPF) programming        */
    /* choose between sets of taps              */
    /* ES1 :                                    */
    /* 00: 6MHz BW (def)                        */
    /* 01: 8MHz BW                              */
    /*                                          */
    /* ES2 :                                    */
    /* 00: 6MHz BW (def)                        */
    /* 01: 8MHz BW                              */
    /* 10: 5MHz BW                              */
    /* 11: 7MHz BW                              */
    /*                                          */
    /* if filter selected is not supported by   */
    /* ES version, driver will return           */
    /* TDA18280_ERR_NOT_SUPPORTED               */
    /*                                          */
    if (pObj->EsVersion == tmTDA18280_EsVersion1)
    {
        switch (ptPathCfg->tBW)
        {
        case tmTDA18280_6MHz:
            filter_select = 0x0;
            break;

        case tmTDA18280_8MHz:
            filter_select = 0x1;
            break;

        case tmTDA18280_2MHz:
        case tmTDA18280_5MHz:
        case tmTDA18280_7MHz:
            err = TDA18280_ERR_NOT_SUPPORTED;
            break;
        }
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_aux_ddc_LPF_CONFIG__filter_select, filter_select);
        }
    }
    else /* (pObj->EsVersion == tmTDA18280_EsVersion2) */
    {
        /* CDP: filters 5/6/7/8MHz allowed */
        switch (ptPathCfg->tBW)
        {
        case tmTDA18280_5MHz:
            filter_select = 0x2;
            break;

        case tmTDA18280_6MHz:
            filter_select = 0x0;
            break;

        case tmTDA18280_7MHz:
            filter_select = 0x3;
            break;

        case tmTDA18280_8MHz:
            filter_select = 0x1;
            break;

        case tmTDA18280_2MHz:
            /* currently the 2MHz filter cannot be used due to some HW limitations */
            /* use 5MHz instead for OOB */
            err = TDA18280_ERR_NOT_SUPPORTED;
            break;
        }

        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_aux_ddc_LPF_CONFIG__filter_select, filter_select);
        }
    }

    if (err == TM_OK)
    {
        /* set NB spectral inversion in order to match with WB spectral inversion */
        err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_aux_ddc_LPF_CONFIG__spec_inv, 1);
    }

    return err;
}



/*============================================================================*/
/* FUNCTION:    iTDA18280_PathDemodClockCheck                                 */
/*                                                                            */
/* DESCRIPTION: Check Clocks                                                  */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_PathDemodClockCheck(
                         ptmTDA18280_Object_t  pObj,        /* I: Driver object */
                         tmTDA18280_Demod_t    DemodIndex,  /* I: Path indicator */
                         UInt8*                pClock_OK    /* O: 0 clock off, 1 clock on */
                         )
{
    tmErrorCode_t   err = TM_OK;
    UInt16          reg_ena = 0;
    UInt16          uDemodEnable = 0;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    /* initialize clock to off to handle errors */
    if ((err == TM_OK) && (pClock_OK != Null))
    {
        *pClock_OK = 0;
    }
    else
    {
        err = TDA18280_ERR_BAD_PARAMETER;
    }

    /* check reg_ena */
    /* if reg_ena is enabled, reset is assumed to be released also */
    if (err == TM_OK)
    {
        err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_cgu_GLOBAL_CFG__reg_ena, &reg_ena);
    }

    if ((err == TM_OK) && (reg_ena == 1))
    {
        if (DemodIndex <= tmTDA18280_Demod_CDP_7)
        {
            /* demodulator is CDP */
            /* read clock for demod + FEC */
            if (err == TM_OK)
            {
                err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_cgu_CLOCKS_DEMOD_CFG__qam_demod_ena, &uDemodEnable);
            }
            if (err == TM_OK)
            {
                uDemodEnable = (uDemodEnable & (1 << (UInt16)DemodIndex));
                if (uDemodEnable != 0)
                {
                    *pClock_OK = 1;
                }
                else
                {
                    *pClock_OK = 0;
                }
            }
        }
        else if (DemodIndex == tmTDA18280_Demod_OOB)
        {
            /* demodulator is OOB */
            /* read clock for demod + FEC */
            if (err == TM_OK)
            {
                err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_cgu_CLOCKS_DEMOD_CFG__oob_demod_ena, &uDemodEnable);
            }
            if (err == TM_OK)
            {
                if (uDemodEnable != 0)
                {
                    *pClock_OK = 1;
                }
                else
                {
                    *pClock_OK = 0;
                }
            }
        }
        else if (DemodIndex == tmTDA18280_Demod_None)
        {
            /* demod not used */
            *pClock_OK = 0;
        }
        else
        {
            tmDBGPRINTEx(DEBUGLVL_BLAB, "!! ERROR !!: Demod %d not supported", (UInt16)DemodIndex);
            err = TDA18280_ERR_NOT_INITIALIZED;
        }
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    iTDA18280_PathDemodStartup                                    */
/*                                                                            */
/* DESCRIPTION: Enable demodulator                                            */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_PathDemodStartup(
                           ptmTDA18280_Object_t         pObj,      /* I: Driver object */
                           tmTDA18280_Path_t            Idx_path,  /* I: Path index */
                           const tmTDA18280_PathCfg_t*  ptPathCfg  /* I: Pointer to Path Configuration structure. */
                           )
{
    tmErrorCode_t   err = TM_OK;
    UInt16 uMask = 0;
    UInt16 uCdpEnable = 0;
    tmTDA18280_Demod_t CdpIndex = tmTDA18280_Demod_Max;
    UInt16 uOobEnable = 0;
    Bool bDemodClockIsOff = False;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    if (pObj->tFeature.bDACSupported && Idx_path >= tmTDA18280_Path_4 && Idx_path <= tmTDA18280_Path_7)
    {
        /* do not set up demod for paths 4-7 when DAC supported */
    }
    else
    {
        if (Idx_path != pObj->eOOBInput)
        {
            /* configure CDP */
            /* CDP2 input switch: select WB (DDC2) or NB (NB DDC) */
            if (Idx_path == TDA18280_AUX_CDP_PATH)
            {
                /* DDC 2 -> CDP 2 */
                if (err == TM_OK)
                {
                    err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_config_DEMOD_TS_IO_CTRL__ddc_nbddc_to_dmd_select, 0);
                }
            }
            else if (Idx_path == tmTDA18280_Path_Aux)
            {
                /* AUX is not used for OOB */
                /* DDC AUX -> CDP 2 */
                if (err == TM_OK)
                {
                    err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_config_DEMOD_TS_IO_CTRL__ddc_nbddc_to_dmd_select, 1);
                }
            }

            if (err == TM_OK)
            {
                err = iTDA18280_GetDemodIndex(pObj, Idx_path, &CdpIndex);
            }

            /* check CDP demod+FEC clock */
            if (err == TM_OK)
            {
                err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_cgu_CLOCKS_DEMOD_CFG__qam_demod_ena, &uCdpEnable);
            }
            if (err == TM_OK)
            {
                uMask = 1 << (UInt16)CdpIndex;
                bDemodClockIsOff = !(uCdpEnable & uMask); /* check if CDP clock is disabled */
                uCdpEnable = uCdpEnable | uMask;
            }

            if (bDemodClockIsOff)
            {
                /* enable CDP demod+FEC clock */
                if (err == TM_OK)
                {
                    err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_cgu_CLOCKS_DEMOD_CFG__qam_demod_ena, uCdpEnable);
                }

                /* release CDP demod+FEC reset */
                if (err == TM_OK)
                {
                    err = iTDA18280_I2CWriteBitField(pObj, BFAOBO(tda18280_reg_reset_RESET_DEMOD__qam_demod0, 0, (UInt8)CdpIndex), 1);
                }

                /* init CDP */
                if (err == TM_OK)
                {
                    err = iTDA18280_CdpHwInit(pObj, CdpIndex);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18280_CdpHwInit(0x%08X) failed.", pObj->tUnitW));
                }
            }

            /* configure CDP demod + start lock */
            if (err == TM_OK)
            {
                err = iTDA18280_CdpConfig(pObj, Idx_path, ptPathCfg);
            }
        }
        else /* (Idx_path == pObj->eOOBInput) */
        {
            /* configure OOB */
            /* OOB input switch: connect OOB to valid DDC */
            if (Idx_path == TDA18280_OOB_DDC_PATH)
            {
                err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_config_DEMOD_TS_IO_CTRL__ddc_nbddc_to_oob_select, 0);
            }
            else if (Idx_path == tmTDA18280_Path_Aux)
            {
                err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_config_DEMOD_TS_IO_CTRL__ddc_nbddc_to_oob_select, 1);
                if (err == TM_OK)
                {
                    /* force CDP2 input switch: DDC2 -> CDP2 */
                    err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_config_DEMOD_TS_IO_CTRL__ddc_nbddc_to_dmd_select, 0);
                }
            }

            /* check OOB demod+FEC clock */
            if (err == TM_OK)
            {
                err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_cgu_CLOCKS_DEMOD_CFG__oob_demod_ena, &uOobEnable);
            }
            if (err == TM_OK)
            {
                bDemodClockIsOff = (uOobEnable == 0); /* check if OOB clock is disabled */
            }

            if (bDemodClockIsOff)
            {
                /* enable OOB demod+FEC clock */
                if (err == TM_OK)
                {
                    err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_cgu_CLOCKS_DEMOD_CFG__oob_demod_ena, 1);
                }

                /* release OOB demod+FEC reset */
                if (err == TM_OK)
                {
                    err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_reset_RESET_DEMOD__oob_demod, 1);
                }

                /* init OOB */
                if (err == TM_OK)
                {
                    err = iTDA18280_OobHwInit(pObj);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18280_OobHwInit(0x%08X) failed.", pObj->tUnitW));
                }
            }

            /* configure OOB demod + start lock */
            if (err == TM_OK)
            {
                err = iTDA18280_OobConfig(pObj, ptPathCfg);
            }
        }
    }

    /* update path config */
    /* except PathCurrentState[Idx_path] */
    if (err == TM_OK)
    {
        pObj->Path[Idx_path] = *ptPathCfg;
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_PathDemodStop                                       */
/*                                                                            */
/* DESCRIPTION: Disable demodulator                                           */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_PathDemodStop(
                        ptmTDA18280_Object_t  pObj,     /* I: Driver object */
                        tmTDA18280_Path_t     Idx_path  /* I: Path index */
                        )
{
    tmErrorCode_t err = TM_OK;
    UInt16 uMask = 0;
    UInt16 uCdpEnable = 0;
    tmTDA18280_Demod_t CdpIndex = tmTDA18280_Demod_Max;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    if (pObj->tFeature.bDACSupported && Idx_path >= tmTDA18280_Path_4 && Idx_path <= tmTDA18280_Path_7)
    {
        /* do not set up demod for paths 4-7 when DAC supported */
    }
    else
    {
        if(Idx_path != pObj->eOOBInput)
        {
            /* configure CDP */
            if(err == TM_OK)
            {
                err = iTDA18280_GetDemodIndex(pObj, Idx_path, &CdpIndex);
            }

            /* activate CDP demod+FEC reset */
            if (err == TM_OK)
            {
                err = iTDA18280_I2CWriteBitField(pObj, BFAOBO(tda18280_reg_reset_RESET_DEMOD__qam_demod0, 0, (UInt8)CdpIndex), 0);
            }

            /* disable CDP demod+FEC clock */
            if (err == TM_OK)
            {
                err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_cgu_CLOCKS_DEMOD_CFG__qam_demod_ena, &uCdpEnable);
            }
            if (err == TM_OK)
            {
                uMask = 1 << (UInt16)CdpIndex;
                uCdpEnable = uCdpEnable & ~uMask;
                err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_cgu_CLOCKS_DEMOD_CFG__qam_demod_ena, uCdpEnable);
            }
        }
        else /* (Idx_path == pObj->eOOBInput) */
        {
            /* configure OOB */

            /* activate OOB demod+FEC reset */
            if (err == TM_OK)
            {
                err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_reset_RESET_DEMOD__oob_demod, 0);
            }

            /* disable CDP demod+FEC clock */
            if(err == TM_OK)
            {
                err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_cgu_CLOCKS_DEMOD_CFG__oob_demod_ena, 0);
            }
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_GetDemodTsmfAvailableTsId                           */
/*                                                                            */
/* DESCRIPTION: Get the available TS IDs                                      */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_GetDemodTsmfAvailableTsId(
                           ptmTDA18280_Object_t   pObj,          /* I: Driver object */
                           tmTDA18280_Demod_t     DemodIndex,    /* I: Demod index */
                           tmTDA18280_TsmfAvailableTs_t* psTsmfAvailableTs  /* O: Pointer to available TS IDs */
                           )
{
    tmErrorCode_t err = TM_OK;
    tmTDA18280_TsmfMode_t eTsmfMode;
    UInt16 uValue = 0;
    UInt16 uTsStatus = 0;
    UInt16 uReStatusMsb = 0;
    UInt16 uReStatusLsb = 0;
    UInt16 uIndex = 0;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    if (DemodIndex <= tmTDA18280_Demod_CDP_7)
    {
        err = iTDA18280_GetDemodTsmfMode(pObj, DemodIndex, &eTsmfMode);

        if (err == TM_OK)
        {
            if ((eTsmfMode == tmTDA18280_TsmfModeDisabled) || (eTsmfMode == tmTDA18280_TsmfModeInvalid))
            {
                for( uIndex = 0; uIndex < TDA18280_TSMF_AVAILABLE_TS_NUMBER; uIndex ++ )
                {
                    psTsmfAvailableTs[uIndex].eTsStatus = tmTDA18280_TsmfTsStatusInvalid;
                    psTsmfAvailableTs[uIndex].eRcvStatus = tmTDA18280_TsmfReceiveStatusNA;
                    psTsmfAvailableTs[uIndex].TsId = 0;
                    psTsmfAvailableTs[uIndex].OnId = 0;
                }
            }
            else /* (eTsmfMode == tmTDA18280_TsmfModeForced) || (eTsmfMode == tmTDA18280_TsmfModeAuto) */
            {
                if( err == TM_OK)
                {
                    /* TS status bit */
                    err = iTDA18280_I2CReadBitField(pObj, BFBO(tda18280_reg_jqam0_TS_STATUS__ts_status, 2*(UInt8)DemodIndex ), &uTsStatus);
                }

                if( err == TM_OK)
                {
                    /* RE status MSB */
                    err = iTDA18280_I2CReadBitField(pObj, BFBO(tda18280_reg_jqam0_RE_STATUS_MSB__re_status_msb, 2*(UInt8)DemodIndex ), &uReStatusMsb);
                }

                if( err == TM_OK)
                {
                    /* RE status LSB */
                    err = iTDA18280_I2CReadBitField(pObj, BFBO(tda18280_reg_jqam0_RE_STATUS_LSB__re_status_lsb, 2*(UInt8)DemodIndex ), &uReStatusLsb);
                }

                if( err == TM_OK)
                {
                    /* there are 15 streams -> uIndex from 0 to 14 */
                    for( uIndex = 0; uIndex < TDA18280_TSMF_AVAILABLE_TS_NUMBER; uIndex ++ )
                    {
                        /* TS status bit */
                        /* TS_STATUS[0] for the relative_ts_id[1] */
                        uValue = (uTsStatus >> uIndex) & BIT_MSK;

                        if( uValue == 1 )
                        {
                            psTsmfAvailableTs[uIndex].eTsStatus = tmTDA18280_TsmfTsStatusValid;
                        }
                        else
                        {
                            psTsmfAvailableTs[uIndex].eTsStatus = tmTDA18280_TsmfTsStatusInvalid;
                        }

                        /* check receive status if the TS is valid */
                        /* else set it to Not Applicable */
                        if( psTsmfAvailableTs[uIndex].eTsStatus == tmTDA18280_TsmfTsStatusValid )
                        {
                            /* RE status */
                            /* RE_STATUS[0] for the relative_ts_id[1] */
                            uValue = ((uReStatusLsb >> uIndex) & BIT_MSK) | (((uReStatusMsb >> uIndex) & BIT_MSK) << 1);

                            switch( uValue )
                            {
                            case 0:
                                psTsmfAvailableTs[uIndex].eRcvStatus = tmTDA18280_TsmfReceiveStatusGood;
                                break;
                            case 1:
                                psTsmfAvailableTs[uIndex].eRcvStatus = tmTDA18280_TsmfReceiveStatusMedium;
                                break;
                            case 2:
                                psTsmfAvailableTs[uIndex].eRcvStatus = tmTDA18280_TsmfReceiveStatusBad;
                                break;
                            default:
                                psTsmfAvailableTs[uIndex].eRcvStatus = tmTDA18280_TsmfReceiveStatusNA;
                                break;
                            }
                        }
                        else
                        {
                            psTsmfAvailableTs[uIndex].eRcvStatus = tmTDA18280_TsmfReceiveStatusNA;
                        }
                    }
                }

                for( uIndex = 0; uIndex < TDA18280_TSMF_AVAILABLE_TS_NUMBER; uIndex ++ )
                {
                    if( err == TM_OK)
                    {
                        err = iTDA18280_I2CReadBitField(pObj, BFBOAO(tda18280_reg_jqam0_TS_ID1__ts_id1, 2*(UInt8)DemodIndex , (uIndex * TDA18280_ON_ID_OFFSET)), &uTsStatus);
                    }
                    psTsmfAvailableTs[uIndex].TsId = uValue;

                    if( err == TM_OK)
                    {
                        err = iTDA18280_I2CReadBitField(pObj, BFBOAO(tda18280_reg_jqam0_ON_ID1__on_id1, 2*(UInt8)DemodIndex , (uIndex * TDA18280_ON_ID_OFFSET)), &uTsStatus);
                    }
                    psTsmfAvailableTs[uIndex].OnId = uValue;
                }
            }
        }
    }
    else
    {
        tmDBGPRINTEx(DEBUGLVL_BLAB, "!! ERROR !!: Demod %d not supported", (UInt16)DemodIndex);
        err = TDA18280_ERR_NOT_INITIALIZED;
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    iTDA18280_GetDemodTsmfStatus                                  */
/*                                                                            */
/* DESCRIPTION: Get Demod TSMF Mode                                           */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_GetDemodTsmfStatus(
                           ptmTDA18280_Object_t   pObj,          /* I: Driver object */
                           tmTDA18280_Demod_t     DemodIndex,    /* I: Demod index */
                           tmTDA18280_TsmfStatus_t* psTsmfStatus /* O: Pointer to TSMF status */
                           )
{
    tmErrorCode_t err = TM_OK;
    tmTDA18280_TsmfMode_t eTsmfMode;
    UInt16 uValue = 0;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    /* initialize to handle errors */
    psTsmfStatus->eLockStatus = tmTDA18280_TsmfStatusLockInvalid;
    psTsmfStatus->eError = tmTDA18280_TsmfStatusErrorInvalid;
    psTsmfStatus->eEmergency = tmTDA18280_TsmfEmergencyNA;
    psTsmfStatus->eCurrentReceiveStatus = tmTDA18280_TsmfReceiveStatusNA;
    psTsmfStatus->uVersionNumber = 0;
    psTsmfStatus->uSelectedTsId = 0;
    psTsmfStatus->uSelectedOnId = 0;

    if (DemodIndex <= tmTDA18280_Demod_CDP_7)
    {
        err = iTDA18280_GetDemodTsmfMode(pObj, DemodIndex, &eTsmfMode);

        if (err == TM_OK)
        {
            if (eTsmfMode == tmTDA18280_TsmfModeDisabled)
            {
                psTsmfStatus->eLockStatus = tmTDA18280_TsmfStatusNotLocked;
                psTsmfStatus->eError = tmTDA18280_TsmfStatusErrorInvalid;
                psTsmfStatus->eEmergency = tmTDA18280_TsmfEmergencyNA;
                psTsmfStatus->eCurrentReceiveStatus = tmTDA18280_TsmfReceiveStatusNA;
                psTsmfStatus->uVersionNumber = 0;
                psTsmfStatus->uSelectedTsId = 0;
                psTsmfStatus->uSelectedOnId = 0;
            }
            else if (eTsmfMode == tmTDA18280_TsmfModeForced || eTsmfMode == tmTDA18280_TsmfModeAuto)
            {
                if (err == TM_OK)
                {
                    err = iTDA18280_I2CReadBitField(pObj, BFBO(tda18280_reg_jqam0_REG0X05, 2*(UInt8)DemodIndex ), &uValue);

                    if( err == TM_OK )
                    {
                        /* Lock status */
                        if( ((uValue & TDA18280_REG0x05_M_LOCK_MSK) >> TDA18280_REG0x05_M_LOCK_BIT) == 0 )
                        {
                            psTsmfStatus->eLockStatus = tmTDA18280_TsmfStatusNotLocked;
                        }
                        else
                        {
                            psTsmfStatus->eLockStatus = tmTDA18280_TsmfStatusLocked;
                        }

                        /* version number */
                        psTsmfStatus->uVersionNumber = (uValue & TDA18280_REG0x05_VER_MSK) >> TDA18280_REG0x05_VER_BIT;

                        /* error */
                        if( ((uValue & TDA18280_REG0x05_ERROR_MSK) >> TDA18280_REG0x05_ERROR_BIT) == 0 )
                        {
                            psTsmfStatus->eError = tmTDA18280_TsmfStatusNoError;
                        }
                        else
                        {
                            psTsmfStatus->eError = tmTDA18280_TsmfStatusError;
                        }

                        if( psTsmfStatus->eError == tmTDA18280_TsmfStatusNoError )
                        {
                            /* emergency */
                            if( ((uValue & TDA18280_REG0x05_EMERGENCY_MSK) >> TDA18280_REG0x05_EMERGENCY_BIT ) == 0 )
                            {
                                psTsmfStatus->eEmergency = tmTDA18280_TsmfEmergencyInactive;
                            }
                            else
                            {
                                psTsmfStatus->eEmergency = tmTDA18280_TsmfEmergencyActive;
                            }

                            /* current receive status */
                            switch( (uValue & TDA18280_REG0x05_CRE_ST_MSK) >> TDA18280_REG0x05_CRE_ST_BIT )
                            {
                            case 0:
                                psTsmfStatus->eCurrentReceiveStatus = tmTDA18280_TsmfReceiveStatusGood;
                                break;
                            case 1:
                                psTsmfStatus->eCurrentReceiveStatus = tmTDA18280_TsmfReceiveStatusMedium;
                                break;
                            case 2:
                                psTsmfStatus->eCurrentReceiveStatus = tmTDA18280_TsmfReceiveStatusBad;
                                break;
                            default:
                                psTsmfStatus->eCurrentReceiveStatus = tmTDA18280_TsmfReceiveStatusNA;
                                break;
                            }
                        }
                        else
                        {
                            psTsmfStatus->eEmergency = tmTDA18280_TsmfEmergencyNA;
                            psTsmfStatus->eCurrentReceiveStatus = tmTDA18280_TsmfReceiveStatusNA;
                        }
                    }

                    if( err == TM_OK )
                    {
                        /* get TS ID matching with the parameter */
                        err = iTDA18280_I2CReadBitField(pObj, BFBO(tda18280_reg_jqam0_TS_ID, 2*(UInt8)DemodIndex ), &uValue);
                    }

                    if( err == TM_OK )
                    {
                        psTsmfStatus->uSelectedTsId = uValue;
                        /* get ON ID (original network) matching with the parameter */
                        err = iTDA18280_I2CReadBitField(pObj, BFBO(tda18280_reg_jqam0_ON_ID, 2*(UInt8)DemodIndex ), &uValue);
                    }

                    if (err == TM_OK)
                    {
                        psTsmfStatus->uSelectedOnId = uValue;
                    }
                }
            }
        }
    }
    else
    {
        tmDBGPRINTEx(DEBUGLVL_BLAB, "!! ERROR !!: Demod %d not supported", (UInt16)DemodIndex);
        err = TDA18280_ERR_NOT_INITIALIZED;
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_GetDemodTsmfMode                                    */
/*                                                                            */
/* DESCRIPTION: Get Demod TSMF Mode                                           */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_GetDemodTsmfMode(
                           ptmTDA18280_Object_t   pObj,        /* I: Driver object */
                           tmTDA18280_Demod_t     DemodIndex,  /* I: Demod index */
                           tmTDA18280_TsmfMode_t* peTsmfMode   /* O: Pointer to TSMF mode */
                           )
{
    tmErrorCode_t err = TM_OK;
    UInt16 uBypass = 0;
    UInt16 uMode = 0;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    /* initialize to handle errors */
    *peTsmfMode = tmTDA18280_TsmfModeInvalid;

    if (DemodIndex <= tmTDA18280_Demod_CDP_7)
    {
        err = iTDA18280_I2CReadBitField(pObj, BFBO(tda18280_reg_jqam0_BYPASS__bypass, 2*(UInt8)DemodIndex ), &uBypass);

        if (err == TM_OK)
        {
            if (uBypass == TDA18280_BYPASS_BYPASS_DISABLE)
            {
                *peTsmfMode = tmTDA18280_TsmfModeDisabled;
            }
            else /* (uBypass == TDA18280_BYPASS_BYPASS_ENABLE) */
            {
                if (err == TM_OK)
                {
                    err = iTDA18280_I2CReadBitField(pObj, BFBO(tda18280_reg_jqam0_MODE__mode, 2*(UInt8)DemodIndex ), &uMode);
                }

                if (err == TM_OK)
                {
                    switch(uMode)
                    {
                    case TDA18280_MODE_BYPASS_DISABLE:
                        *peTsmfMode = tmTDA18280_TsmfModeDisabled;
                        break;

                    case TDA18280_MODE_BYPASS_ENABLE:
                        *peTsmfMode = tmTDA18280_TsmfModeForced;
                        break;

                    case TDA18280_MODE_BYPASS_AUTOMATIC:
                        *peTsmfMode = tmTDA18280_TsmfModeAuto;
                        break;

                    default:
                        err = TDA18280_ERR_BAD_PARAMETER;
                        break;
                    }
                }
            }
        }
    }
    else
    {
        tmDBGPRINTEx(DEBUGLVL_BLAB, "!! ERROR !!: Demod %d not supported", (UInt16)DemodIndex);
        err = TDA18280_ERR_NOT_INITIALIZED;
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_SetDemodTsmfMode                                    */
/*                                                                            */
/* DESCRIPTION: Set Demod TSMF Mode                                           */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_SetDemodTsmfMode(
                           ptmTDA18280_Object_t   pObj,        /* I: Driver object */
                           tmTDA18280_Demod_t     DemodIndex,  /* I: Demod index */
                           tmTDA18280_TsmfMode_t  eTsmfMode    /* I: TSMF mode */
                           )
{
    tmErrorCode_t err = TM_OK;
    UInt16 uMode = 0x0;
    UInt16 uBypass = 0x0;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    if (DemodIndex <= tmTDA18280_Demod_CDP_7)
    {
        switch( eTsmfMode )
        {
        case tmTDA18280_TsmfModeDisabled:
            uBypass = TDA18280_BYPASS_BYPASS_DISABLE;
            uMode = TDA18280_MODE_BYPASS_DISABLE;
            break;

        case tmTDA18280_TsmfModeForced:
            uBypass = TDA18280_BYPASS_BYPASS_ENABLE;
            uMode = TDA18280_MODE_BYPASS_ENABLE;
            break;

        case tmTDA18280_TsmfModeAuto:
            uBypass = TDA18280_BYPASS_BYPASS_ENABLE;
            uMode = TDA18280_MODE_BYPASS_AUTOMATIC;
            break;

        default:
            err = TDA18280_ERR_BAD_PARAMETER;
            break;
        }

        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFBO(tda18280_reg_jqam0_BYPASS__bypass, 2*(UInt8)DemodIndex ), uBypass);
        }

        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFBO(tda18280_reg_jqam0_MODE__mode, 2*(UInt8)DemodIndex ), uMode);
        }
    }
    else
    {
        tmDBGPRINTEx(DEBUGLVL_BLAB, "!! ERROR !!: Demod %d not supported", (UInt16)DemodIndex);
        err = TDA18280_ERR_NOT_INITIALIZED;
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_PathTsOutputStartup                                 */
/*                                                                            */
/* DESCRIPTION: Enable TS output                                              */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_PathTsOutputStartup(
                           ptmTDA18280_Object_t  pObj,     /* I: Driver object */
                           tmTDA18280_Path_t     Idx_path  /* I: Path index */
                           )
{
    tmErrorCode_t   err = TM_OK;
    UInt16 uTsEnable = 0;
    tmTDA18280_Ts_t TsIndex = tmTDA18280_Ts_Max;
    UInt16 uMask = 0;
    Bool bEnableTSCLK6 = False;
    UInt16 uTsClkMode = 0;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    /* select TS path */
    if (err == TM_OK)
    {
        err = iTDA18280_GetTsIndex(pObj, Idx_path, &TsIndex);
    }

    /* TS7 input switch: select DEMOD7 or OOB output */
    if (TsIndex == tmTDA18280_Ts_7)
    {
        if (pObj->eOOBInput == tmTDA18280_Path_Max)
        {
            /* connect to CDP */
            if (err == TM_OK)
            {
                err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_config_DEMOD_TS_IO_CTRL__oob_dmd_ts_out_select, 1);
            }
        }
        else if ((pObj->eOOBInput == tmTDA18280_Path_Aux) || (pObj->eOOBInput == tmTDA18280_Path_7)) /* Auxiliary path */
        {
            /* connect to OOB */
            if (err == TM_OK)
            {
                err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_config_DEMOD_TS_IO_CTRL__oob_dmd_ts_out_select, 0);
            }
            /* select OOB output: demodulator (POD for external CableCard) or FEC */
            if (err == TM_OK)
            {
                if (pObj->eOOBFECMode == tmTDA18280_OOBFECMode_POD)
                {
                    err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_config_DEMOD_TS_IO_CTRL__oob_pod_select, 1);
                }
                else
                {
                    err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_config_DEMOD_TS_IO_CTRL__oob_pod_select, 0);
                }
            }
        }
        else
        {
            tmDBGPRINTEx(DEBUGLVL_BLAB, "!! ERROR !!: Path %d not supported", Idx_path);
            err = TDA18280_ERR_NOT_SUPPORTED;
        }
    }

    if (err == TM_OK)
    {
        if (pObj->PathCfgFixed[Idx_path].eTsClkMode == tmTDA18280_TsClkGated)
        {
            /* gated clock signal is routed to TS_CLK pad */
            uTsClkMode = 0;
        }
        else if (pObj->PathCfgFixed[Idx_path].eTsClkMode == tmTDA18280_TsClkPermanent)
        {
            /* permanent clock signal on TS_CLK6 -> TS_VALID signal is routed to TS_CLK pad */
            uTsClkMode = 2;
        }
    }

    /* manage available features */
    if (!pObj->tFeature.bDACSupported)
    {
        /* check TS gated clock */
        if (err == TM_OK)
        {
            if (pObj->PathCfgFixed[Idx_path].eTsClkMode != tmTDA18280_TsClkGated)
            {
                /* permanent TS clock is not supported for these products */
                err = TDA18280_ERR_NOT_SUPPORTED;
            }
        }

        /* select TS_CLK mode */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFAO(tda18280_reg_config_TS0_CONFIG__clock_mode, (UInt16)(2*TsIndex)), uTsClkMode);
        }

        /* select TS data order */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFAO(tda18280_reg_config_TS0_CONFIG__lsb_first, (UInt16)(2*TsIndex)), pObj->PathCfgFixed[Idx_path].eTsDataOrder);
        }

        /* select TS sync length */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFAO(tda18280_reg_config_TS0_CONFIG__short_ts_sync, (UInt16)(2*TsIndex)), pObj->PathCfgFixed[Idx_path].eTsSyncLength);
        }

        /* release TS reset */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_reset_RESET_DWN_DSP__ts, 1);
        }

        /* enable TS output */
        /* activate TS internal clock */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_cgu_CLOCKS_TS_SER_OUT_CFG__ts_ser_out_ena, &uTsEnable);
        }
        if (err == TM_OK)
        {
            uMask = 1 << (UInt16)TsIndex;
            uTsEnable = uTsEnable | uMask;
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_cgu_CLOCKS_TS_SER_OUT_CFG__ts_ser_out_ena, uTsEnable);
        }

        /* activate TS pads: LOW = output, HIGH = input/high impedance */
        /* TS_SYNC */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_config_TS_SYNC_EN__en_l, &uTsEnable);
        }
        if (err == TM_OK)
        {
            uMask = 1 << (UInt16)TsIndex;
            uTsEnable = uTsEnable & ~uMask;
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_config_TS_SYNC_EN__en_l, uTsEnable);
        }

        /* TS_DATA */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_config_TS_DATA_EN__en_l, &uTsEnable);
        }
        if (err == TM_OK)
        {
            uMask = 1 << (UInt16)TsIndex;
            uTsEnable = uTsEnable & ~uMask;
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_config_TS_DATA_EN__en_l, uTsEnable);
        }

        /* TS_CLK */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_config_TS_CLK_EN__en_l, &uTsEnable);
        }
        if (err == TM_OK)
        {
            uMask = 1 << (UInt16)TsIndex;
            uTsEnable = uTsEnable & ~uMask;
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_config_TS_CLK_EN__en_l, uTsEnable);
        }
    }
    else /* DAC supported */
    {
        if (TsIndex >= tmTDA18280_Ts_0 && TsIndex <= tmTDA18280_Ts_3)
        {
            /* release TS reset */
            if (err == TM_OK)
            {
                err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_reset_RESET_DWN_DSP__ts, 1);
            }

            /* switch on permanent TS clock based on the path configuration */
            if (err == TM_OK)
            {
                bEnableTSCLK6 = (pObj->PathCfgFixed[Idx_path].eTsClkMode == tmTDA18280_TsClkPermanent);
            }

            if (bEnableTSCLK6)
            {
                /* select TS6 permanent clock */
                if (err == TM_OK)
                {
                    err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_config_TS6_CONFIG__clock_mode, 1);
                }
                /* activate TS6 internal clock */
                if (err == TM_OK)
                {
                    err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_cgu_CLOCKS_TS_SER_OUT_CFG__ts_ser_out_ena, &uTsEnable);
                }
                if (err == TM_OK)
                {
                    uMask = 1 << (UInt16)tmTDA18280_Ts_6;
                    uTsEnable = uTsEnable | uMask;
                    err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_cgu_CLOCKS_TS_SER_OUT_CFG__ts_ser_out_ena, uTsEnable);
                }
                /* activate TS_CLK6 pad: LOW = output, HIGH = input/high impedance */
                if (err == TM_OK)
                {
                    err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_config_TS_CLK_EN__en_l, &uTsEnable);
                }
                if (err == TM_OK)
                {
                    uMask = 1 << (UInt16)tmTDA18280_Ts_6;
                    uTsEnable = uTsEnable & ~uMask;
                    err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_config_TS_CLK_EN__en_l, uTsEnable);
                }
            }

            /* select TS_CLK mode (choose TS_VALID or TS_CLK on pin TS_VALIDx when DAC supported */
            if (err == TM_OK)
            {
                err = iTDA18280_I2CWriteBitField(pObj, BFAO(tda18280_reg_config_TS0_CONFIG__clock_mode, (UInt16)(2*TsIndex)), uTsClkMode);
            }

            /* select TS data order */
            if (err == TM_OK)
            {
                err = iTDA18280_I2CWriteBitField(pObj, BFAO(tda18280_reg_config_TS0_CONFIG__lsb_first, (UInt16)(2*TsIndex)), pObj->PathCfgFixed[Idx_path].eTsDataOrder);
            }

            /* select TS sync length */
            if (err == TM_OK)
            {
                err = iTDA18280_I2CWriteBitField(pObj, BFAO(tda18280_reg_config_TS0_CONFIG__short_ts_sync, (UInt16)(2*TsIndex)), pObj->PathCfgFixed[Idx_path].eTsSyncLength);
            }

            /* enable TS output */
            /* activate TS pads: LOW = output, HIGH = input/high impedance */
            /* TS_SYNC */
            if (err == TM_OK)
            {
                err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_config_TS_SYNC_EN__en_l, &uTsEnable);
            }
            if (err == TM_OK)
            {
                uMask = 1 << (UInt16)TsIndex;
                uTsEnable = uTsEnable & ~uMask;
                err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_config_TS_SYNC_EN__en_l, uTsEnable);
            }

            /* TS_DATA */
            if (err == TM_OK)
            {
                err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_config_TS_DATA_EN__en_l, &uTsEnable);
            }
            if (err == TM_OK)
            {
                uMask = 1 << (UInt16)TsIndex;
                uTsEnable = uTsEnable & ~uMask;
                err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_config_TS_DATA_EN__en_l, uTsEnable);
            }

            /* TS_CLK (pin TS_VALIDx with permanent TS clock) */
            if (err == TM_OK)
            {
                err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_config_TS_CLK_EN__en_l, &uTsEnable);
            }
            if (err == TM_OK)
            {
                uMask = 1 << (UInt16)TsIndex;
                uTsEnable = uTsEnable & ~uMask;
                err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_config_TS_CLK_EN__en_l, uTsEnable);
            }

            /* activate TS internal clock */
            if (err == TM_OK)
            {
                err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_cgu_CLOCKS_TS_SER_OUT_CFG__ts_ser_out_ena, &uTsEnable);
            }
            if (err == TM_OK)
            {
                uMask = 1 << (UInt16)TsIndex;
                uTsEnable = uTsEnable | uMask;
                err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_cgu_CLOCKS_TS_SER_OUT_CFG__ts_ser_out_ena, uTsEnable);
            }
        }
        else /* (TsIndex >= tmTDA18280_Ts_4 && TsIndex <= tmTDA18280_Ts_7) */
        {
            /* no need to enable TS for paths 4-7 when DAC supported */
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_PathTsOutputStop                                    */
/*                                                                            */
/* DESCRIPTION: Disable TS output                                             */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_PathTsOutputStop(
                        ptmTDA18280_Object_t  pObj,     /* I: Driver object */
                        tmTDA18280_Path_t     Idx_path  /* I: Path index */
                        )
{
    tmErrorCode_t   err = TM_OK;
    UInt16 uTsEnable = 0;
    tmTDA18280_Ts_t TsIndex = tmTDA18280_Ts_Max;
    UInt16 uMask = 0;
    tmTDA18280_Path_t PathLoop = tmTDA18280_Path_Max;
    UInt8 TsLoop = tmTDA18280_Ts_Max;
    UInt8 uNbOfEnabledTs = 0;
    UInt8 uNbOfEnabledTsWithPermanentClk = 0;
    Bool bDisableTSCLK6 = False;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    /* select TS path */
    err = iTDA18280_GetTsIndex(pObj, Idx_path, &TsIndex);

    /* manage available features */
    if (!pObj->tFeature.bDACSupported)
    {
        /* disable TS output */
        /* deactivate TS pads: LOW = output, HIGH = input/high impedance */
        /* TS_SYNC */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_config_TS_SYNC_EN__en_l, &uTsEnable);
        }
        if (err == TM_OK)
        {
            uMask = 1 << (UInt16)TsIndex;
            uTsEnable = uTsEnable | uMask;
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_config_TS_SYNC_EN__en_l, uTsEnable);
        }

        /* TS_DATA */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_config_TS_DATA_EN__en_l, &uTsEnable);
        }
        if (err == TM_OK)
        {
            uMask = 1 << (UInt16)TsIndex;
            uTsEnable = uTsEnable | uMask;
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_config_TS_DATA_EN__en_l, uTsEnable);
        }

        /* TS_CLK */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_config_TS_CLK_EN__en_l, &uTsEnable);
        }
        if (err == TM_OK)
        {
            uMask = 1 << (UInt16)TsIndex;
            uTsEnable = uTsEnable | uMask;
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_config_TS_CLK_EN__en_l, uTsEnable);
        }

        /* deactivate TS internal clock */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_cgu_CLOCKS_TS_SER_OUT_CFG__ts_ser_out_ena, &uTsEnable);
        }
        if (err == TM_OK)
        {
            uMask = 1 << (UInt16)TsIndex;
            uTsEnable = uTsEnable & ~uMask;
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_cgu_CLOCKS_TS_SER_OUT_CFG__ts_ser_out_ena, uTsEnable);
        }

        /* check if all 8 channels will be OFF */
        /* count the number of enabled TS, except the current one */
        uNbOfEnabledTs = 0;
        for (TsLoop = tmTDA18280_Ts_0; TsLoop <= tmTDA18280_Ts_7; TsLoop++)
        {
            if (TsLoop != TsIndex)
            {
                /* find the TS matching the path */
                if (err == TM_OK)
                {
                    err = iTDA18280_GetPathIndexFromTs(pObj, (tmTDA18280_Ts_t)TsLoop, &PathLoop);
                }
                /* add 1 for each enabled TS */
                if (PathLoop < tmTDA18280_Path_Max)
                {
                    uNbOfEnabledTs += ((pObj->PathCurrentState[PathLoop] == tmTDA18280_OFF)?0:1);
                }
            }
        }
        /* if all 8 TS channels will be OFF, force the reset */
        if (uNbOfEnabledTs == 0)
        {
            /* activate TS reset */
            if (err == TM_OK)
            {
                err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_reset_RESET_DWN_DSP__ts, 0);
            }
        }
    }
    else /* DAC supported */
    {
        if (TsIndex >= tmTDA18280_Ts_0 && TsIndex <= tmTDA18280_Ts_3)
        {
            /* disable TS output */
            /* deactivate TS pads: LOW = output, HIGH = input/high impedance */
            /* TS_SYNC */
            if (err == TM_OK)
            {
                err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_config_TS_SYNC_EN__en_l, &uTsEnable);
            }
            if (err == TM_OK)
            {
                uMask = 1 << (UInt16)TsIndex;
                uTsEnable = uTsEnable | uMask;
                err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_config_TS_SYNC_EN__en_l, uTsEnable);
            }

            /* TS_DATA */
            if (err == TM_OK)
            {
                err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_config_TS_DATA_EN__en_l, &uTsEnable);
            }
            if (err == TM_OK)
            {
                uMask = 1 << (UInt16)TsIndex;
                uTsEnable = uTsEnable | uMask;
                err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_config_TS_DATA_EN__en_l, uTsEnable);
            }

            /* TS_CLK (pin TS_VALIDx with permanent TS clock) */
            if (err == TM_OK)
            {
                err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_config_TS_CLK_EN__en_l, &uTsEnable);
            }
            if (err == TM_OK)
            {
                uMask = 1 << (UInt16)TsIndex;
                uTsEnable = uTsEnable | uMask;
                err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_config_TS_CLK_EN__en_l, uTsEnable);
            }

            /* deactivate TS internal clock */
            if (err == TM_OK)
            {
                err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_cgu_CLOCKS_TS_SER_OUT_CFG__ts_ser_out_ena, &uTsEnable);
            }
            if (err == TM_OK)
            {
                uMask = 1 << (UInt16)TsIndex;
                uTsEnable = uTsEnable & ~uMask;
                err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_cgu_CLOCKS_TS_SER_OUT_CFG__ts_ser_out_ena, uTsEnable);
            }

            /* check if all 4 channels will be OFF */
            /* check if all channels with permanent TS clock will be OFF */
            /* count the number of enabled TS, except the current one */
            for (TsLoop = tmTDA18280_Ts_0; TsLoop <= tmTDA18280_Ts_3; TsLoop++)
            {
                if (TsLoop != TsIndex)
                {
                    /* find the TS matching the path */
                    if (err == TM_OK)
                    {
                        err = iTDA18280_GetPathIndexFromTs(pObj, (tmTDA18280_Ts_t)TsLoop, &PathLoop);
                    }
                    /* add 1 for each enabled TS */
                    if (PathLoop < tmTDA18280_Path_Max)
                    {
                        /* nb of enabled TS except the current */
                        uNbOfEnabledTs += ((pObj->PathCurrentState[PathLoop] == tmTDA18280_OFF)?0:1);
                        /* nb of enabled TS with permanent TS clock except the current */
                        uNbOfEnabledTsWithPermanentClk += (((pObj->PathCfgFixed[PathLoop].eTsClkMode == tmTDA18280_TsClkPermanent) &&
                                                           (pObj->PathCurrentState[PathLoop] == tmTDA18280_ON))?1:0);
                    }
                }
            }

            bDisableTSCLK6 = (pObj->PathCfgFixed[Idx_path].eTsClkMode == tmTDA18280_TsClkPermanent) &&
                             (uNbOfEnabledTsWithPermanentClk == 0);
            /* if all TS channels with permanent TS clock will be OFF, switch off also the permanent TS clock */
            if (bDisableTSCLK6)
            {
                /* deactivate TS_CLK6 pad: LOW = output, HIGH = input/high impedance */
                if (err == TM_OK)
                {
                    err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_config_TS_CLK_EN__en_l, &uTsEnable);
                }
                if (err == TM_OK)
                {
                    uMask = 1 << (UInt16)tmTDA18280_Ts_6;
                    uTsEnable = uTsEnable | uMask;
                    err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_config_TS_CLK_EN__en_l, uTsEnable);
                }

                /* deactivate TS6 internal clock */
                if (err == TM_OK)
                {
                    err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_cgu_CLOCKS_TS_SER_OUT_CFG__ts_ser_out_ena, &uTsEnable);
                }
                if (err == TM_OK)
                {
                    uMask = 1 << (UInt16)tmTDA18280_Ts_6;
                    uTsEnable = uTsEnable & ~uMask;
                    err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_cgu_CLOCKS_TS_SER_OUT_CFG__ts_ser_out_ena, uTsEnable);
                }
            }

            /* if all 4 channels will be OFF, activate also the TS reset */
            if (uNbOfEnabledTs == 0)
            {
                /* activate TS reset */
                if (err == TM_OK)
                {
                    err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_reset_RESET_DWN_DSP__ts, 0);
                }
            }
        }
        else /* (TsIndex >= tmTDA18280_Ts_4 && TsIndex <= tmTDA18280_Ts_7) */
        {
            /* no need to disable TS4 to TS7 */
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_NbAdcConfiguration                                  */
/*                                                                            */
/* DESCRIPTION: Config Narrow Band ADC (auxiliary path)                      */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_NbAdcConfiguration(
                             ptmTDA18280_Object_t    pObj,       /* I: Driver object */
                             tmTDA18280_ONOFF_t      eStatus
                             )
{
    tmErrorCode_t   err = TM_OK;
    UInt16 ulValue = 0;
    UInt32 RegMask = 0;
    UInt16 ulValueCs = 0;
    UInt16 ulValueDgec0 = 0;
    UInt16 ulValueDgec1 = 0;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    if (eStatus == tmTDA18280_ON)
    {
        /* activate NB ADC reset */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_reset_RESET_DWN_DSP__nb_adc, 0);
        }
        /* enable NB ADC clock */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_cgu_CLOCKS_CFG__nb_adc_fast_select, pObj->NBADCFastSel);
        }
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_cgu_CLOCKS_CFG__nb_adc_inv, 1);
        }
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_cgu_CLOCKS_CFG__nb_adc_ena, 1);
        }
        //if (err == TM_OK)
        //{
        //    err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_aux_ddc_POWERDOWN__powerdown, 0);
        //}
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_cgu_LO_CONTROL__dig_clkdsp_pd_adc_nb_vih, 0);
        }
        /* release NB ADC reset */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_reset_RESET_DWN_DSP__nb_adc, 1);
        }

        /* activate NB DDC reset */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_reset_RESET_DWN_DSP__nb_ddc, 0);
        }
        /* enable NB DDC clock */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_cgu_CLOCKS_CFG__nb_ddc_ena, 1);
        }
        /* release NB DDC reset */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_reset_RESET_DWN_DSP__nb_ddc, 1);
        }

        /* switch AUX DDC on */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_aux_ddc_ADC_CFG__pd, 0);
        }

        /* configure NB ADC speed */
        if (pObj->PllCfg.NBADCSampleFreq == tmTDA18280_NBADCSampleFreq28MHz125)
        {
            ulValueCs = 2;
            ulValueDgec0 = 0x2222;
            ulValueDgec1 = 0x0022;
        }
        else /* (pObj->PllCfg.NBADCSampleFreq == tmTDA18280_NBADCSampleFreq56MHz25) */
        {
            ulValueCs = 4;
            ulValueDgec0 = 0x4444;
            ulValueDgec1 = 0x0044;
        }

        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_aux_ddc_ADC_CFG__cs, ulValueCs);
        }

        /* digital Gain Error Correction */
        /* set all 6 bits fields to 2 instead of 4 by default */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_aux_ddc_ADC_DGEC_0, ulValueDgec0);
        }
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_aux_ddc_ADC_DGEC_1, ulValueDgec1);
        }

        /* drive IF AGC loop */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_aux_ddc_AAGC_CFG, 1, &ulValue);
        }

        if (err == TM_OK)
        {
            /* Enable AAGC PWM signal - pwm_enable = 1 */
            RegMask = ( (1 << tda18280_reg_aux_ddc_AAGC_CFG__pwm_enable_WidthInBits) - 1);
            RegMask = RegMask << tda18280_reg_aux_ddc_AAGC_CFG__pwm_enable_PositionInBits;

            ulValue &= ~RegMask;
            ulValue += 1 << tda18280_reg_aux_ddc_AAGC_CFG__pwm_enable_PositionInBits;

            /*Error metric gain selection - en_gain_sel = 5 */
            RegMask = ( (1 << tda18280_reg_aux_ddc_AAGC_CFG__em_gain_sel_WidthInBits) - 1);
            RegMask = RegMask << tda18280_reg_aux_ddc_AAGC_CFG__em_gain_sel_PositionInBits;

            ulValue &= ~RegMask;
            ulValue += 5 << tda18280_reg_aux_ddc_AAGC_CFG__em_gain_sel_PositionInBits;

            /* RMS error metric cutoff frequency selection  - rms_eps_sel = 4 */
            RegMask = ( (1 << tda18280_reg_aux_ddc_AAGC_CFG__rms_eps_sel_WidthInBits) - 1);
            RegMask = RegMask << tda18280_reg_aux_ddc_AAGC_CFG__rms_eps_sel_PositionInBits;

            ulValue &= ~RegMask;
            ulValue += 4 << tda18280_reg_aux_ddc_AAGC_CFG__rms_eps_sel_PositionInBits;

            /* RMS error metric enable - rms_enable = 1 */
            RegMask = ( (1 << tda18280_reg_aux_ddc_AAGC_CFG__rms_enable_WidthInBits) - 1);
            RegMask = RegMask << tda18280_reg_aux_ddc_AAGC_CFG__rms_enable_PositionInBits;

            ulValue &= ~RegMask;
            ulValue += 1 << tda18280_reg_aux_ddc_AAGC_CFG__rms_enable_PositionInBits;

            err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_aux_ddc_AAGC_CFG, ulValue);
        }

        /* configure IFAGC */
        if (err == TM_OK)
        {
            /* rms_target = 0x09C4 */
            ulValue = 0x09C4;
            err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_aux_ddc_AAGC_RMS_TARGET, ulValue);
        }
        /* enable IFAGC output */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_config_IO_DISABLE_OUTPUT_DRIVER__aux_tuner_agc_en, 0);
        }
    }
    else /* (eStatus == tmTDA18280_OFF) */
    {
        /* disable IFAGC output */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_config_IO_DISABLE_OUTPUT_DRIVER__aux_tuner_agc_en, 1);
        }

        /* switch AUX DDC off */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_aux_ddc_ADC_CFG__pd, 1);
        }

        /* activate NB DDC reset */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_reset_RESET_DWN_DSP__nb_ddc, 0);
        }
        /* disable NB DDC clock */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_cgu_CLOCKS_CFG__nb_ddc_ena, 0);
        }
        //if (err == TM_OK)
        //{
        //    /* PRCR#558: the powerdown bit is activated to avoid I2C read artefact */
        //    /* which can occur when reading reg_aux_ddc registers with disabled clock */
        //    err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_aux_ddc_POWERDOWN__powerdown, 1);
        //}

        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_cgu_CLOCKS_CFG__nb_adc_fast_select, 0);
        }
        /* activate NB ADC reset */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_reset_RESET_DWN_DSP__nb_adc, 0);
        }
        /* disable NB ADC clock */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_cgu_CLOCKS_CFG__nb_adc_ena, 0);
        }

        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_cgu_LO_CONTROL__dig_clkdsp_pd_adc_nb_vih, 1);
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_BlockCrossSwitchProg                                */
/*                                                                            */
/* DESCRIPTION: Set RF                                                        */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_BlockCrossSwitchProg(
                               ptmTDA18280_Object_t    pObj,       /* I: Driver object */
                               UInt16  Idx_ddc,
                               UInt16 cfg_cross_switch
                               )
{
    tmErrorCode_t   err = TM_OK;
    UInt16          value16 = 0;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    switch (Idx_ddc) {
        case 0:
            if(err == TM_OK)
            {
                err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_3_2_1_0, 1, &value16);
            }
            if(err == TM_OK)
            {
                err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_3_2_1_0,   (value16 & 0xfff0) | (cfg_cross_switch << 0));
            }
            break;
        case 1:
            if(err == TM_OK)
            {
                err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_3_2_1_0, 1, &value16);
            }
            if(err == TM_OK)
            {
                err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_3_2_1_0,   (value16 & 0xff0f) | (cfg_cross_switch << 4));
            }
            break;
        case 2:
            if(err == TM_OK)
            {
                err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_3_2_1_0, 1, &value16);
            }
            if(err == TM_OK)
            {
                err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_3_2_1_0,   (value16 & 0xf0ff) | (cfg_cross_switch << 8));
            }
            break;
        case 3:
            if(err == TM_OK)
            {
                err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_3_2_1_0, 1, &value16);
            }
            if(err == TM_OK)
            {
                err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_3_2_1_0,   (value16 & 0x0fff) | (cfg_cross_switch << 12));
            }
            break;
        case 4:
            if(err == TM_OK)
            {
                err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_7_6_5_4, 1, &value16);
            }
            if(err == TM_OK)
            {
                err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_7_6_5_4,   (value16 & 0xfff0) | (cfg_cross_switch << 0));
            }
            break;
        case 5:
            if(err == TM_OK)
            {
                err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_7_6_5_4, 1, &value16);
            }
            if(err == TM_OK)
            {
                err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_7_6_5_4,   (value16 & 0xff0f) | (cfg_cross_switch << 4));
            }
            break;
        case 6:
            if(err == TM_OK)
            {
                err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_7_6_5_4, 1, &value16);
            }
            if(err == TM_OK)
            {
                err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_7_6_5_4,   (value16 & 0xf0ff) | (cfg_cross_switch << 8));
            }
            break;
        case 7:
            if(err == TM_OK)
            {
                err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_7_6_5_4, 1, &value16);
            }
            if(err == TM_OK)
            {
                err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_7_6_5_4,   (value16 & 0x0fff) | (cfg_cross_switch << 12));
            }
            break;
        case 8:
            if(err == TM_OK)
            {
                err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_11_10_9_8, 1, &value16);
            }
            if(err == TM_OK)
            {
                err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_11_10_9_8,   (value16 & 0xfff0) | (cfg_cross_switch << 0));
            }
            break;
        case 9:
            if(err == TM_OK)
            {
                err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_11_10_9_8, 1, &value16);
            }
            if(err == TM_OK)
            {
                err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_11_10_9_8,   (value16 & 0xff0f) | (cfg_cross_switch << 4));
            }
            break;
        case 10:
            if(err == TM_OK)
            {
                err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_11_10_9_8, 1, &value16);
            }
            if(err == TM_OK)
            {
                err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_11_10_9_8,   (value16 & 0xf0ff) | (cfg_cross_switch << 8));
            }
            break;
        case 11:
            if(err == TM_OK)
            {
                err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_11_10_9_8, 1, &value16);
            }
            if(err == TM_OK)
            {
                err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_11_10_9_8,   (value16 & 0x0fff) | (cfg_cross_switch << 12));
            }
            break;
        case 12:
            if(err == TM_OK)
            {
                err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_15_14_13_12, 1, &value16);
            }
            if(err == TM_OK)
            {
                err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_15_14_13_12,   (value16 & 0xfff0) | (cfg_cross_switch << 0));
            }
            break;
        case 13:
            if(err == TM_OK)
            {
                err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_15_14_13_12, 1, &value16);
            }
            if(err == TM_OK)
            {
                err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_15_14_13_12,   (value16 & 0xff0f) | (cfg_cross_switch << 4));
            }
            break;
        case 14:
            if(err == TM_OK)
            {
                err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_15_14_13_12, 1, &value16);
            }
            if(err == TM_OK)
            {
                err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_15_14_13_12,   (value16 & 0xf0ff) | (cfg_cross_switch << 8));
            }
            break;
        case 15:
            if(err == TM_OK)
            {
                err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_15_14_13_12, 1, &value16);
            }
            if(err == TM_OK)
            {
                err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_15_14_13_12,   (value16 & 0x0fff) | (cfg_cross_switch << 12));
            }
            break;
        case 16:
            if(err == TM_OK)
            {
                err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_19_18_17_16, 1, &value16);
            }
            if(err == TM_OK)
            {
                err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_19_18_17_16,   (value16 & 0xfff0) | (cfg_cross_switch << 0));
            }
            break;
        case 17:
            if(err == TM_OK)
            {
                err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_19_18_17_16, 1, &value16);
            }
            if(err == TM_OK)
            {
                err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_19_18_17_16,   (value16 & 0xff0f) | (cfg_cross_switch << 4));
            }
            break;
        case 18:
            if(err == TM_OK)
            {
                err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_19_18_17_16, 1, &value16);
            }
            if(err == TM_OK)
            {
                err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_19_18_17_16,   (value16 & 0xf0ff) | (cfg_cross_switch << 8));
            }
            break;
        case 19:
            if(err == TM_OK)
            {
                err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_19_18_17_16, 1, &value16);
            }
            if(err == TM_OK)
            {
                err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_19_18_17_16,   (value16 & 0x0fff) | (cfg_cross_switch << 12));
            }
            break;
        case 20:
            if(err == TM_OK)
            {
                err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_23_22_21_20, 1, &value16);
            }
            if(err == TM_OK)
            {
                err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_23_22_21_20,   (value16 & 0xfff0) | (cfg_cross_switch << 0));
            }
            break;
        case 21:
            if(err == TM_OK)
            {
                err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_23_22_21_20, 1, &value16);
            }
            if(err == TM_OK)
            {
                err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_23_22_21_20,   (value16 & 0xff0f) | (cfg_cross_switch << 4));
            }
            break;
        case 22:
            if(err == TM_OK)
            {
                err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_23_22_21_20, 1, &value16);
            }
            if(err == TM_OK)
            {
                err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_23_22_21_20,   (value16 & 0xf0ff) | (cfg_cross_switch << 8));
            }
            break;
        case 23:
            if(err == TM_OK)
            {
                err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_23_22_21_20, 1, &value16);
            }
            if(err == TM_OK)
            {
                err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_cross_switch_CFG_IQ_23_22_21_20,   (value16 & 0x0fff) | (cfg_cross_switch << 12));
            }
            break;
        default:
            break;
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_PathReadGain                                        */
/*                                                                            */
/* DESCRIPTION: Set RF                                                        */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_PathReadGain(
                       ptmTDA18280_Object_t    pObj,       /* I: Driver object */
                       tmTDA18280_Path_t       PathNumber  /* I: Path indicator */
                       )
{
    tmErrorCode_t   err = TM_OK;
    UInt16 gain_manual_shift_rd =0;
    UInt16 gain_manual_mult_rd =0;
    UInt16 gain_rd =0;
    UInt16 gain = 0;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    if (pObj->PathCurrentState[PathNumber] == tmTDA18280_ON)
    {
        if (PathNumber<tmTDA18280_Path_Aux)
        {
            err = iTDA18280_I2CReadRegister(pObj, REGBO(tda18280_reg_ddc_slow0_GAIN_RD, (UInt16)(2*PathNumber)), 1, &gain_rd);
        }
        else if (PathNumber==tmTDA18280_Path_Aux)
        {
            err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_aux_ddc_DAGC_GAIN_READ, 1, &gain_rd);
        }
        else
        {
            err = TDA18280_ERR_BAD_PARAMETER;
        }

        /* This is an additional precaution in case the DDC clock has been disabled. */
        /* 0xDEAD is never a possible value for this register under normal operation. */
        if (gain_rd != 0xDEAD)
        {
            gain_manual_mult_rd = gain_rd & 0x3F;
            gain_manual_shift_rd = (gain_rd>>6);
        }

        if (err == TM_OK)
        {
            gain = gain_manual_shift_rd*60 + gain_manual_mult_rd;
        }
        else
        {
            /* in case of error, return 0 */
            gain = 0;
        }
    }

    pObj->Path[PathNumber].uGainMultBy10 = gain;

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_BlockI2CGate_SetCfg                                 */
/*                                                                            */
/* DESCRIPTION:                                                               */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_BlockI2CGate_SetCfg(
                              ptmTDA18280_Object_t pObj,
                              tmTDA18280_I2CGate_t*       pI2CGateCfg     /**< I: Configuration of the Gate. */
                              )
{
    tmErrorCode_t   err = TM_OK;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    /* tmTDA18280_ONOFF_t  tI2CGateActivation  0: OFF Gate inactive, 1: ON Gate active */
    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_config_CMD_GATE_SCI__close, pI2CGateCfg->tI2CGateActivation); 
    }

    /* UInt8  tI2CGateSCIAdd  If Gate active, address of the SCI from 0 to 7 */
    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_config_CMD_GATE_SCI__sci_address, pI2CGateCfg->tI2CGateSCIAdd); 
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_BlockI2CGate_GetCfg                                 */
/*                                                                            */
/* DESCRIPTION:                                                               */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_BlockI2CGate_GetCfg(
                              ptmTDA18280_Object_t pObj
                              )
{
    tmErrorCode_t   err = TM_OK;
    UInt16          value16 = 0;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    /* tmTDA18280_ONOFF_t tI2CGateActivation  0: OFF Gate inactive, 1: ON Gate active */
    if(err == TM_OK)
    {
        err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_config_CMD_GATE_SCI__close, &value16); 
    }
    if(err == TM_OK)
    {
        pObj->I2CGateCfg.tI2CGateActivation = (tmTDA18280_ONOFF_t)value16;
    }

    /* UInt8 tI2CGateSCIAdd  If Gate active, address of the SCI from 0 to 7 */
    if(err == TM_OK)
    {
        err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_config_CMD_GATE_SCI__sci_address, &value16); 
    }
    if(err == TM_OK)
    {
        pObj->I2CGateCfg.tI2CGateSCIAdd = (UInt8)(value16&0x7);
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_TEQMeasureIFGain                                    */
/*                                                                            */
/* DESCRIPTION:                                                               */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_TEQMeasureIFGain(
                           ptmTDA18280_Object_t    pObj    /* I: Driver object */
                           )
{
    tmErrorCode_t   err = TM_OK;
    UInt32   i = 0;
    UInt8    j = 0;
    UInt8    k = 0;
    UInt8    path = 0;
    Bool     bLastIndexReached = False;
    tmTDA18280_PathCfg_t pathCfg;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    /* initialize Min, Max and Nb empty channels */
    pObj->TiltOutputParameters.uTDA18280_MaxGain = 0;
    pObj->TiltOutputParameters.uTDA18280_FreqMaxGain = 0;
    pObj->TiltOutputParameters.uTDA18280_MinGain = TDA18280_TEQ_MAX_GAIN;
    pObj->TiltOutputParameters.uTDA18280_FreqMinGain = 0;
    pObj->TiltOutputParameters.uTDA18280_NbEmptyChannel = 0;

    /* start up the DDC of all paths used */
    for (path = 0; path < pObj->TiltInputParameters.uTDA18280_NbChannels; path++)
    {
        if( err == TM_OK )
        {
            err = iTDA18280_PathDdcStartup(pObj, (tmTDA18280_Path_t)path);
        }
    }

    for (i = 0; i <= pObj->TiltOutputParameters.uTDA18280_LastFreqIndex;)
    {
        for (j = 0; j < pObj->TiltInputParameters.uTDA18280_NbChannels; j++)
        {
            path = j%pObj->TiltInputParameters.uTDA18280_NbChannels;

            /* check if we have reached last index in case */
            /* pObj->TiltOutputParameters.uTDA18280_LastFreqIndex %pObj->TiltInputParameters.uTDA18280_NbChannels */
            /* is different from 0 */
            if ((i+j) == pObj->TiltOutputParameters.uTDA18280_LastFreqIndex)
            {
                bLastIndexReached = True;
            }

            /* fill pathCfg to be used in iTDA18280_TEQPathConfig() */
            if(err == TM_OK)
            {
                pathCfg.tPathStatus = tmTDA18280_ON;
                pathCfg.uRF = iTDA18280_FreqGain[i+j].Frequency * 1000000 ;
                pathCfg.tGainAutoMode = tmTDA18280_ON;

                pathCfg.tBW = pObj->TiltInputParameters.eTDA18280_BandWidth;

                err = iTDA18280_PathConfig(pObj, path, &pathCfg, tmTDA18280_PathSetAGCMode_Calibration);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18280_PathConfig(0x%08X) failed.", pObj->tUnitW));
            }

            if (bLastIndexReached == True)
            {
                break;
            }
        }

        /* read gains */

        /* wait for stabilisation of DIG AGC */
        if( err == TM_OK )
        {
            err = iTDA18280_UtilWait(pObj, pObj->TiltInputParameters.uTDA18280_Wait );
        }

        for (k = 0; k <= j; k++)
        {
            path = k%pObj->TiltInputParameters.uTDA18280_NbChannels;

            /* Set Dig AGC in normal mode */
            if( err == TM_OK )
            {
                err = iTDA18280_SetDigAgcMode( pObj, path, tmTDA18280_AgcMode_Normal );
            }

            if( err == TM_OK )
            {
                err = iTDA18280_PathReadGain(pObj, (tmTDA18280_Path_t)path);
            }

            iTDA18280_FreqGain[i+k].Gain = pObj->Path[path].uGainMultBy10;

            /* fill gain calculated after rejection algorithm (minimise side channel effects) */
            iTDA18280_FreqGain[i+k].GainAfterRejection = iTDA18280_FreqGain[i+k].Gain;

            /* fill gain corrected array in order to have valid data in case of block of channel detected*/
            iTDA18280_FreqGain[i+k].GainCorrected = iTDA18280_FreqGain[i+k].Gain;

            /* detect Min, Max and Nb empty channels */
            if( (iTDA18280_FreqGain[i+k].Gain >= pObj->TiltOutputParameters.uTDA18280_MaxGain) && (iTDA18280_FreqGain[i+k].Gain < pObj->TiltInputParameters.uTDA18280_EmptyThreshold) ) 
            {
                /* store Max gain information */
                pObj->TiltOutputParameters.uTDA18280_MaxGain     = iTDA18280_FreqGain[i+k].Gain;
                pObj->TiltOutputParameters.uTDA18280_FreqMaxGain = iTDA18280_FreqGain[i+k].Frequency;
            }

            if (iTDA18280_FreqGain[i+k].Gain <= pObj->TiltOutputParameters.uTDA18280_MinGain)
            {
                /* store Min gain information */
                pObj->TiltOutputParameters.uTDA18280_MinGain     = iTDA18280_FreqGain[i+k].Gain;
                pObj->TiltOutputParameters.uTDA18280_FreqMinGain = iTDA18280_FreqGain[i+k].Frequency;
            }

            if( iTDA18280_FreqGain[i+k].Gain >= pObj->TiltInputParameters.uTDA18280_EmptyThreshold )
            {
                pObj->TiltOutputParameters.uTDA18280_NbEmptyChannel += 1;
            }
        }

        i+=k;
    }

    for (path = 0; path < pObj->TiltInputParameters.uTDA18280_NbChannels; path++)
    {
        if( err == TM_OK )
        {
            err = iTDA18280_PathDdcStop(pObj, (tmTDA18280_Path_t)path);
        }

        if( err == TM_OK )
        {
            /* although the DDC is enabled, it should be considered as stopped, because it is not usable */
            pObj->Path[path].tPathStatus = tmTDA18280_OFF;
            pObj->PathCurrentState[path] = tmTDA18280_OFF;
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_TEQDetectRejection                                  */
/*                                                                            */
/* DESCRIPTION:                                                               */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       in this function calculs on gain are done in 10 x Gain (dB)   */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_TEQDetectRejection(
                             ptmTDA18280_Object_t    pObj   /* I: Driver object */
                             )
{
    tmErrorCode_t   err = TM_OK;
    UInt32          i = 0;
    UInt32          j = 0;
    Int32           deriv = 0;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    /* firstly compare gain[i] to gain[i-1] and correct gain[i] */
    for (i = 0; i <= pObj->TiltOutputParameters.uTDA18280_LastFreqIndex; i++)
    {
        if(i != 0)
        {
            deriv = iTDA18280_FreqGain[i].Gain - iTDA18280_FreqGain[i-1].Gain;
        }
        else
        {
            deriv = iTDA18280_FreqGain[i].Gain - TDA18280_TEQ_MAX_GAIN;
        }

        /* find the correction to apply in lookup table */
        j=0;
        while ((deriv > iTDA18280_RejectionLookupTable[j][0]) && (j<23))
        {
            j+=1;
        }

        if (j == 23)
        {
            iTDA18280_FreqGain[i].GainAfterRejection = iTDA18280_FreqGain[i].GainAfterRejection + iTDA18280_RejectionLookupTable[j-1][1];

            if (iTDA18280_FreqGain[i].GainAfterRejection > TDA18280_TEQ_MAX_GAIN)
            {
                iTDA18280_FreqGain[i].GainAfterRejection = TDA18280_TEQ_MAX_GAIN;
            }
        }
        else
        {
            if (j != 0)
            {
                iTDA18280_FreqGain[i].GainAfterRejection = iTDA18280_FreqGain[i].GainAfterRejection + iTDA18280_RejectionLookupTable[j][1];

                if (iTDA18280_FreqGain[i].GainAfterRejection > TDA18280_TEQ_MAX_GAIN)
                {
                    iTDA18280_FreqGain[i].GainAfterRejection = TDA18280_TEQ_MAX_GAIN;
                }
            }
        }
    }

    /* secondly compare gain[i] to gain[i+1] and correct gain[i] */
    for (i = 0; i <= pObj->TiltOutputParameters.uTDA18280_LastFreqIndex; i++)
    {
        if(i != pObj->TiltOutputParameters.uTDA18280_LastFreqIndex)
        {
            deriv = iTDA18280_FreqGain[i].GainAfterRejection - iTDA18280_FreqGain[i+1].GainAfterRejection;
        }
        else
        {
            deriv = iTDA18280_FreqGain[i].GainAfterRejection - TDA18280_TEQ_MAX_GAIN;
        }

        /* find the correction to apply in lookup table */
        j=0;

        while ((deriv > iTDA18280_RejectionLookupTable[j][0]) && (j<23))
        {
            j+=1;
        }

        if (j == 23)
        {
            iTDA18280_FreqGain[i].GainAfterRejection = iTDA18280_FreqGain[i].GainAfterRejection + iTDA18280_RejectionLookupTable[j-1][1];

            if (iTDA18280_FreqGain[i].GainAfterRejection > TDA18280_TEQ_MAX_GAIN)
            {
                iTDA18280_FreqGain[i].GainAfterRejection = TDA18280_TEQ_MAX_GAIN;
            }
        }
        else
        {
            if (j != 0)
            {
                iTDA18280_FreqGain[i].GainAfterRejection = iTDA18280_FreqGain[i].GainAfterRejection + iTDA18280_RejectionLookupTable[j][1];

                if (iTDA18280_FreqGain[i].GainAfterRejection > TDA18280_TEQ_MAX_GAIN)
                {
                    iTDA18280_FreqGain[i].GainAfterRejection = TDA18280_TEQ_MAX_GAIN;
                }
            }
        }

        /* fill  gain corrected with corrected values */
        iTDA18280_FreqGain[i].GainCorrected = iTDA18280_FreqGain[i].GainAfterRejection;
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_TEQDetectBlockOfChannel                             */
/*                                                                            */
/* DESCRIPTION:                                                               */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_TEQDetectBlockOfChannel(
                                  ptmTDA18280_Object_t    pObj       /* I: Driver object */
                                  )
{
    tmErrorCode_t    err = TM_OK;
    UInt32           i_first = 0;
    UInt32           i_last = pObj->TiltOutputParameters.uTDA18280_LastFreqIndex;
    UInt32           first_freq_not_empty_ch = 0;
    UInt32           last_freq_not_empty_ch = 0;
    UInt32           first_freq = 0;
    UInt32           last_freq = 0;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    first_freq = iTDA18280_FreqGain[i_first].Frequency;
    last_freq = iTDA18280_FreqGain[i_last].Frequency;
    /* find first freq from lowest index */
    while( i_first <= i_last )
    {
        if( iTDA18280_FreqGain[i_first].GainAfterRejection < pObj->TiltInputParameters.uTDA18280_EmptyThreshold )
        {
            /* not empty channel */
            first_freq_not_empty_ch = iTDA18280_FreqGain[i_first].Frequency;
            break;
        }
        i_first+=1;
    }

    /* find last freq from highest index but not lower than the first freq index */
    while( i_first <= i_last )
    {
        if( iTDA18280_FreqGain[i_last].GainAfterRejection < pObj->TiltInputParameters.uTDA18280_EmptyThreshold )
        {
            /* not empty channel */
            last_freq_not_empty_ch = iTDA18280_FreqGain[i_last].Frequency;
            break;
        }
        i_last-=1;
    }

    /* detection of block of channel */
    if (last_freq_not_empty_ch - first_freq_not_empty_ch < (((last_freq - first_freq) * pObj->TiltInputParameters.uTDA18280_FrequencyPercentThreshold)/100))
    {
        pObj->TiltOutputParameters.bTDA18280_BlockOfChannelDetected = True;
    }
    else
    {
        pObj->TiltOutputParameters.bTDA18280_BlockOfChannelDetected = False;
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_TEQHandleAnalogDigitalStep                          */
/*                                                                            */
/* DESCRIPTION:                                                               */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_TEQHandleAnalogDigitalStep(
                                     ptmTDA18280_Object_t    pObj       /* I: Driver object */
                                     )
{
    tmErrorCode_t   err = TM_OK;
    UInt32          i = 0;
    UInt32          iplus1 = 0;
    Int32           deriv = 0;
    Int32           deriv_at_step_index = 0;
    Int32           biggest_step_average_current = 0;
    Int32           biggest_step_average_previous = 0;
    UInt32          frequency_step_index = 0;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    while( iTDA18280_FreqGain[i].Frequency != 0 )
    {
        if( iTDA18280_FreqGain[i].GainAfterRejection < pObj->TiltInputParameters.uTDA18280_EmptyThreshold )
        {
            /* Step detection */
            iplus1 = iTDA18280_TEQNextValidFreq( pObj, i);

            /* both sample used must be in the range */
            /* or no valid iplus1 index */
            if( ((iTDA18280_FreqGain[iplus1].Frequency - iTDA18280_FreqGain[i].Frequency) > pObj->TiltInputParameters.uTDA18280_StepFreqRange) || (iplus1 == i) )
            {
                /* skip the step evaluation */
                deriv = 0;
            }
            else
            {
                deriv = iTDA18280_FreqGain[iplus1].GainAfterRejection - iTDA18280_FreqGain[i].GainAfterRejection;
                deriv = ABS(deriv);
            }

            /* is there a local step ? */
            if( (UInt32)deriv > pObj->TiltInputParameters.uTDA18280_StepThreshold )
            {
                /* keep step value */
                biggest_step_average_current = iTDA18280_TEQCalculStepAverage( pObj, i );
                biggest_step_average_current = ABS(biggest_step_average_current);

                if( biggest_step_average_current > biggest_step_average_previous)
                {
                    /* previous biggest step becomes the biggest */
                    biggest_step_average_previous = biggest_step_average_current;

                    /* store frequence index of biggest step only if it's a real step */
                    if( (biggest_step_average_previous * 100) > (deriv * (Int32)pObj->TiltInputParameters.uTDA18280_StepPercentThreshold) )
                    {
                        frequency_step_index = i;
                    }
                }
            }
        }

        i+=1;
    }

    /* if a step is found, correct IF gain after step and store it in iTDA18280_Gain[i][2] table */
    /* the original IF Gain is kept in order to be plotted later */
    if (frequency_step_index == 0)
    {
        for (i = 0; i <= pObj->TiltOutputParameters.uTDA18280_LastFreqIndex; i++)
        {
            iTDA18280_FreqGain[i].GainCorrected = iTDA18280_FreqGain[i].GainAfterRejection;
        }
    }
    else
    {
        pObj->TiltOutputParameters.bTDA18280_StepDetected = True;
        pObj->TiltOutputParameters.uTDA18280_StepDetectedFreq = iTDA18280_FreqGain[frequency_step_index].Frequency;

        deriv_at_step_index = iTDA18280_FreqGain[iTDA18280_TEQNextValidFreq( pObj, frequency_step_index)].Gain - iTDA18280_FreqGain[frequency_step_index].Gain;

        pObj->TiltOutputParameters.iTDA18280_StepDetectedValue = deriv_at_step_index;

        /* step found */
        for (i = 0; i <= pObj->TiltOutputParameters.uTDA18280_LastFreqIndex; i++)
        {
            /* do not apply the correction if no signal */
            if( (i <= frequency_step_index) || (iTDA18280_FreqGain[i].Gain >= pObj->TiltInputParameters.uTDA18280_EmptyThreshold) )
            {
                iTDA18280_FreqGain[i].GainCorrected = iTDA18280_FreqGain[i].GainAfterRejection;
            }
            else
            {
                if ( (Int32)iTDA18280_FreqGain[i].GainAfterRejection >= deriv_at_step_index)
                {
                    iTDA18280_FreqGain[i].GainCorrected = iTDA18280_FreqGain[i].GainAfterRejection - deriv_at_step_index;
                }
                else
                {
                    iTDA18280_FreqGain[i].GainCorrected = 0;
                }
            }
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_TEQLeastSquareAnalysis                              */
/*                                                                            */
/* DESCRIPTION:                                                               */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_TEQLeastSquareAnalysis(
                                 ptmTDA18280_Object_t    pObj       /* I: Driver object */
                                 )
{
    tmErrorCode_t   err = TM_OK;
    UInt8           i = 0;

    UInt32          uIFMoy = 0;
    UInt32          uGainMoy = 0;
    UInt8           uNbValidSample = 0;
    Int32           IFi_IFMoy_Gaini_GainMoy = 0;
    UInt32          uGaini_GainMoy_Square = 0;
    UInt32          uIFi_IFMoy_Square = 0;

    UInt32 ulNum;
    UInt32 ulDenom;

    UInt32 ulNum1;
    UInt32 ulNum2;

    UInt32 ulDenom1;
    UInt32 ulDenom2;

    UInt32 ulNumFactor;
    UInt32 ulDenomFactor;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    /* default value */
    pObj->TiltOutputParameters.uTDA18280_RSquare = 1000;

    while(iTDA18280_FreqGain[i].Frequency != 0)
    {
        /* IFmoy calculation*/
        if( iTDA18280_FreqGain[i].GainCorrected < pObj->TiltInputParameters.uTDA18280_EmptyThreshold )
        {
            uIFMoy += iTDA18280_FreqGain[i].Frequency;

            /* calculation is done on recalculated Gain after step correction */
            uGainMoy += iTDA18280_FreqGain[i].GainCorrected;

            uNbValidSample += 1;
        }
        i += 1;
    }

    /* all gains are > Empty threshold so no valid samples */
    /* in this case, end of calculation */

    if (uNbValidSample != 0)
    {
        uIFMoy = uIFMoy / uNbValidSample;
        uGainMoy = uGainMoy / uNbValidSample;

        i = 0;
        while( iTDA18280_FreqGain[i].Frequency != 0 )
        {
            if( iTDA18280_FreqGain[i].GainCorrected < pObj->TiltInputParameters.uTDA18280_EmptyThreshold )
            {
                /* (Xi - Xmoy)(Yi - Ymoy) */
                IFi_IFMoy_Gaini_GainMoy += ((iTDA18280_FreqGain[i].Frequency - uIFMoy)*(iTDA18280_FreqGain[i].GainCorrected - uGainMoy));

                /* (Xi - Xmoy)^2 */
                uIFi_IFMoy_Square += ((iTDA18280_FreqGain[i].Frequency - uIFMoy)*(iTDA18280_FreqGain[i].Frequency - uIFMoy));

                /* (Yi - Ymoy)^2 */
                uGaini_GainMoy_Square += ((iTDA18280_FreqGain[i].GainCorrected - uGainMoy)*(iTDA18280_FreqGain[i].GainCorrected- uGainMoy));
            }
            i += 1;
        }

        /* calculate iTDA18280_Slope = (Int32)((IFi_IFMoy_Gaini_GainMoy * 1000) / (Int32)uIFi_IFMoy_Square)*/
        /* to avoid Int32 overflow, changed into: */
        pObj->TiltOutputParameters.iTDA18280_Slope = (Int32)((IFi_IFMoy_Gaini_GainMoy * 100) / (Int32)(uIFi_IFMoy_Square / 10));

        /* avoid divide by zero */
        if( (uIFi_IFMoy_Square != 0) && (uGaini_GainMoy_Square != 0) )
        {
            /* evaluate max numerator value */
            /* take care of 32 bits limit */
            /* IFi_IFMoy_Gaini_GainMoy * IFi_IFMoy_Gaini_GainMoy < MAX_32_BITS */

            ulNumFactor = 1;
            ulNum1 = (UInt32)ABS(IFi_IFMoy_Gaini_GainMoy);
            ulNum2 = ulNum1;

            while( ulNum1 > MAX_32_BITS / ulNum2 ) 
            {
                ulNumFactor *= 10;

                if( ulNum1 > ulNum2 )
                {
                    ulNum1 /= 10;
                }
                else
                {
                    ulNum2 /= 10;
                }
            }

            /* evaluate max denominator value */
            /* take care of 32 bits limit */
            /* uIFi_IFMoy_Square * uGaini_GainMoy_Square < MAX_32_BITS */

            ulDenomFactor = 1;
            ulDenom1 = uIFi_IFMoy_Square;
            ulDenom2 = uGaini_GainMoy_Square;

            while( ulDenom1 > MAX_32_BITS / ulDenom2 )
            {
                ulDenomFactor *= 10;
                if( ulDenom1 > ulDenom2 )
                {
                    ulDenom1 /= 10;
                }
                else
                {
                    ulDenom2 /= 10;
                }
            }

            /* ratio between numerator and denominator must be 1000 in order to get some precision */
            if( ulNumFactor > ulDenomFactor )
            {
                /* this case must never happen */
                /* set R square to default value */
                pObj->TiltOutputParameters.uTDA18280_RSquare = 0;
            }
            else
            {
                ulNum = ulNum1 * ulNum2;
                ulDenom = ulDenom1 * ulDenom2;

                if( ulDenomFactor / ulNumFactor != 1000 )
                {
                    /* tune the calculation */

                    /* first increase numerator factor */
                    while( (ulNum < (MAX_32_BITS / 10)) && (ulDenomFactor/ulNumFactor != 1000) )
                    {
                        ulNum *= 10;

                        if( ulNumFactor  >= 10 )
                        {
                            ulNumFactor /= 10;
                        }
                        else
                        {
                            ulDenomFactor *= 10;
                        }

                    }

                    /* then tune the demominator */
                    while( ulDenomFactor/ulNumFactor != 1000 )
                    {
                        ulDenom /= 10;

                        ulDenomFactor *= 10;
                    }
                }

                /* eventually calculate R square with proper precision */
                pObj->TiltOutputParameters.uTDA18280_RSquare = ulNum / ulDenom;
            }
        }
    }

    /* due to possible calculation errors, clamp RSquare */
    if( pObj->TiltOutputParameters.uTDA18280_RSquare > 1000 )
    {
        pObj->TiltOutputParameters.uTDA18280_RSquare = 1000;
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_TEQCalculateT0Square                                */
/*                                                                            */
/* DESCRIPTION:                                                               */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_TEQCalculateT0Square(
                               ptmTDA18280_Object_t    pObj       /* I: Driver object */
                               )
{
    tmErrorCode_t   err = TM_OK;
    UInt8           i = 0;
    UInt32 ulNum = 0;
    UInt32 ulDenom = 1;
    UInt8  uNbValidSample = 0;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    while(iTDA18280_FreqGain[i].Frequency != 0)
    {
        /* calculation of valid samples */
        if( iTDA18280_FreqGain[i].GainCorrected < pObj->TiltInputParameters.uTDA18280_EmptyThreshold )
        {
            uNbValidSample += 1;
        }
        i += 1;
    }

    ulNum = pObj->TiltOutputParameters.uTDA18280_RSquare*(uNbValidSample-2);

    /* denominator must be != 0 */
    if (pObj->TiltOutputParameters.uTDA18280_RSquare != 1000)
    {
        ulDenom = 1000-pObj->TiltOutputParameters.uTDA18280_RSquare;
    }

    pObj->TiltOutputParameters.uTDA18280_T0Square = ulNum/ulDenom;

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_TEQGetClosestSlope                                  */
/*                                                                            */
/* DESCRIPTION:                                                               */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_TEQGetClosestSlope(
                             ptmTDA18280_Object_t         pObj          /* I: Driver object */
                             )
{
    tmErrorCode_t   err = TM_OK;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    if (pObj->TiltOutputParameters.iTDA18280_Slope > 125)
    {
        pObj->TiltOutputParameters.TDA18280_CalculatedTilt = tmTDA18280_TiltEqualizer15dB;
    }
    else if (pObj->TiltOutputParameters.iTDA18280_Slope > 75)
    {
        pObj->TiltOutputParameters.TDA18280_CalculatedTilt = tmTDA18280_TiltEqualizer10dB;
    }
    else if (pObj->TiltOutputParameters.iTDA18280_Slope > 40)
    {
        pObj->TiltOutputParameters.TDA18280_CalculatedTilt = tmTDA18280_TiltEqualizer5dB;
    }
    else if (pObj->TiltOutputParameters.iTDA18280_Slope > 15)
    {
        pObj->TiltOutputParameters.TDA18280_CalculatedTilt = tmTDA18280_TiltEqualizer3dB;
    }
    else if (pObj->TiltOutputParameters.iTDA18280_Slope > -25)
    {
        pObj->TiltOutputParameters.TDA18280_CalculatedTilt = tmTDA18280_TiltEqualizer0dB;
    }
    else if (pObj->TiltOutputParameters.iTDA18280_Slope > -75)
    {
        pObj->TiltOutputParameters.TDA18280_CalculatedTilt = tmTDA18280_TiltEqualizerMinus5dB;
    }
    else /* if (pObj->TiltOutputParameters.iTDA18280_Slope <= -75) */
    {
        pObj->TiltOutputParameters.TDA18280_CalculatedTilt = tmTDA18280_TiltEqualizerMinus10dB;
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_TEQNextValidFreq                                    */
/*                                                                            */
/* DESCRIPTION:                                                               */
/*                                                                            */
/* RETURN:                                                                    */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
static UInt32 iTDA18280_TEQNextValidFreq( ptmTDA18280_Object_t pObj, UInt32 ulIndex )
{
    UInt32 ulValidIndex;
    UInt32 Index;

    ulValidIndex = ulIndex;
    Index = ulIndex;

    while( Index < pObj->TiltOutputParameters.uTDA18280_LastFreqIndex )
    {
        Index ++;

        /* check that value has a valid signal */
        if( iTDA18280_FreqGain[Index].GainAfterRejection < pObj->TiltInputParameters.uTDA18280_EmptyThreshold )
        {
            ulValidIndex = Index;
            break;
        }
    }

    return ulValidIndex;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_TEQPreviousValidFreq                                */
/*                                                                            */
/* DESCRIPTION:                                                               */
/*                                                                            */
/* RETURN:                                                                    */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
static UInt32 iTDA18280_TEQPreviousValidFreq( ptmTDA18280_Object_t pObj, UInt32 ulIndex )
{
    UInt32 ulValidIndex;

    ulValidIndex = ulIndex;

    while( ulIndex > 0 )
    {
        ulIndex --;

        /* check that value has a valid signal */
        if( iTDA18280_FreqGain[ulIndex].Gain < pObj->TiltInputParameters.uTDA18280_EmptyThreshold )
        {
            ulValidIndex = ulIndex;
            break;
        }
    }

    return ulValidIndex;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_TEQCalculStepAverage:                              */
/*                                                                            */
/* DESCRIPTION:                                                               */
/*                                                                            */
/* RETURN:                                                                    */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
Int32 iTDA18280_TEQCalculStepAverage( ptmTDA18280_Object_t pObj, UInt32 freq_index)
{
    Int32  res;
    UInt32 uAverageGainAfterFreq = 0;
    UInt32 uAverageGainBeforeFreq = 0;
    UInt32 uNbGainAfter = 0;
    UInt32 uNbGainBefore = 0;
    UInt32 ulDownIndex = freq_index;
    UInt32 ulUpIndex = freq_index;
    UInt32 ulPreviousDownIndex = freq_index;
    UInt32 ulNextUpIndex = freq_index;
    UInt32 ulIndex;

    for( ulIndex = 0; ulIndex < pObj->TiltInputParameters.uTDA18280_StepAverage; ulIndex ++ )
    {
        ulPreviousDownIndex = iTDA18280_TEQPreviousValidFreq( pObj, ulDownIndex );
        if( ulDownIndex != ulPreviousDownIndex )
        {
            uAverageGainBeforeFreq += iTDA18280_FreqGain[ulPreviousDownIndex].Gain;
            uNbGainBefore += 1;
        }
        ulDownIndex = ulPreviousDownIndex;

        ulNextUpIndex = iTDA18280_TEQNextValidFreq( pObj, ulUpIndex );
        if( ulUpIndex != ulNextUpIndex )
        {       
            uAverageGainAfterFreq += iTDA18280_FreqGain[ulNextUpIndex].Gain;
            uNbGainAfter += 1;
        }
        ulUpIndex = ulNextUpIndex;
    }

    if ((uNbGainAfter > 2) && (uNbGainBefore > 2))
    {
        res = (uAverageGainBeforeFreq/uNbGainBefore) - (uAverageGainAfterFreq/uNbGainAfter);
    }
    else
    {
        /* for the moment if not enough samples, set res to 0 */
        res = 0;
    }

    return res;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_CalculateGainRegValue                               */
/*                                                                            */
/* DESCRIPTION: calculate register value for gain                             */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
UInt16 
iTDA18280_CalculateGainRegValue(UInt32 ulGain)
{
    UInt16 gain_shift; /* Manual gain (shift) : step 6.0 dB *  7 = 42.0 dB */
    UInt16 gain_mult;  /* Manual gain (mult)  : step 0.1 dB * 59 =  5.9 dB */
    UInt16 gain_reg;
    UInt16 gain_dBx10;

    /* Gain calculation and programming */
    gain_shift = (UInt16)(ulGain/60);
    if (gain_shift > 0x7) /* gain_shift is 3 bits */
    {
        gain_shift = 0x7;
    }
    gain_mult = (UInt16)ulGain - (gain_shift * 60);
    if (gain_mult > 0x3F) /* gain_mult is 6 bits */
    {
        gain_mult = 0x3F;
    }
    gain_dBx10 = gain_shift * 60 + gain_mult;
    gain_reg = (gain_shift << 6) + gain_mult;

    return gain_reg;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_UtilSignExtension                                   */
/*                                                                            */
/* DESCRIPTION: Perform sign extension                                        */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
int iTDA18280_UtilSignExtension(int nbbits, int value)
{
    int sign_mask = (1 << (nbbits-1));
    int sign; 
    int i;
    int mask;
    int res;

    sign = ((value & sign_mask) >> (nbbits-1));
    mask = 0;
    for(i=32;i>nbbits;i--) {
        mask = mask + (1<<(i-1));    
    }
    mask = mask * sign; 
    res = value | mask; 
    return res;
} 

/*============================================================================*/
/* FUNCTION:    iTDA18280_GetTemperatureMeasurement                           */
/*                                                                            */
/* DESCRIPTION:                                                               */
/*                                                                            */
/* RETURN:                                                                    */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t iTDA18280_GetTemperatureMeasurement(ptmTDA18280_Object_t pObj, tmTDA18280_TemperatureSource_t eTempSource, Int32 *plTemp)
{   
    tmErrorCode_t   err = TM_OK;
#ifndef SIMU_RTL
    UInt16          ulTempIndex = 0;
    UInt16          ulTempStatus = 0;
    UInt16          ulSel_15_00 = 0; 
    UInt16          ulSel_31_16 = 0; 
    UInt16          ulSel_39_32 = 0; 
    UInt16          ulPdOffset = 0;

    switch( eTempSource )
    {
    case tmTDA18280_TemperatureSourceHS:
        ulPdOffset = 0;
        break;
    case tmTDA18280_TemperatureSourceLO:
        ulPdOffset = 0xA;
        break;
    case tmTDA18280_TemperatureSourceRFADC:
        ulPdOffset = 0x14;
        break;
    default:
        break;
    }

    /* sensor in powerdown */
    err = iTDA18280_I2CWriteBitField(pObj, BFAO(tda18280_reg_config_SENSOR_HS_PD__pd, ulPdOffset), 1 );

    /* reset value */
    if( err == TM_OK )
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGAO(tda18280_reg_config_SENSOR_HS_SEL_39_32, ulPdOffset), 0 );
    }
    if( err == TM_OK )
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGAO(tda18280_reg_config_SENSOR_HS_SEL_31_16, ulPdOffset), 0 );
    }
    if( err == TM_OK )
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGAO(tda18280_reg_config_SENSOR_HS_SEL_15_00, ulPdOffset), 0 );
    }

    /* active sensor */
    if( err == TM_OK )
    {
        err = iTDA18280_I2CWriteBitField(pObj, BFAO(tda18280_reg_config_SENSOR_HS_PD__pd, ulPdOffset), 0 );
    }

    /* perform the measurement */
    ulTempIndex = 0;

    ulSel_15_00 = 1; 
    ulSel_31_16 = 0; 
    ulSel_39_32 = 0; 

    do
    {
        if( err == TM_OK )
        {
            err = iTDA18280_I2CWriteRegister( pObj, REGAO(tda18280_reg_config_SENSOR_HS_SEL_39_32, ulPdOffset), ulSel_39_32 );
        }
        if( err == TM_OK )
        {
            err = iTDA18280_I2CWriteRegister( pObj, REGAO(tda18280_reg_config_SENSOR_HS_SEL_31_16, ulPdOffset), ulSel_31_16 );
        }
        if( err == TM_OK )
        {
            err = iTDA18280_I2CWriteRegister( pObj, REGAO(tda18280_reg_config_SENSOR_HS_SEL_15_00, ulPdOffset), ulSel_15_00 );
        }

        /* check that the temperature match */
        if( err == TM_OK )
        {
            err = iTDA18280_I2CReadRegister( pObj, REGAO(tda18280_reg_config_SENSOR_HS_OUTTEMPERATURE, ulPdOffset), 1, &ulTempStatus );
        }

        if( ulTempStatus == 0 )
        {
            /* temperature match -> stop the measure*/
            err = iTDA18280_I2CWriteBitField(pObj, BFAO(tda18280_reg_config_SENSOR_HS_PD__pd, ulPdOffset), 1 );
        }
        else
        {
            /* temperature does not match check next value */
            if( ulTempIndex < 15 )
            {
                ulSel_15_00 = ulSel_15_00 << 1; 
                ulSel_31_16 = 0; 
                ulSel_39_32 = 0; 
            }
            else if( ulTempIndex == 15 )
            {
                ulSel_15_00 = 0; 
                ulSel_31_16 = 1; 
                ulSel_39_32 = 0; 
            }
            else if( ulTempIndex < 31 )
            {
                ulSel_15_00 = 0; 
                ulSel_31_16 = ulSel_31_16 << 1; 
                ulSel_39_32 = 0; 
            }
            else if( ulTempIndex == 31 )
            {
                ulSel_15_00 = 0;
                ulSel_31_16 = 0;
                ulSel_39_32 = 1;
            }
            else
            {
                ulSel_15_00 = 0; 
                ulSel_31_16 = 0;
                ulSel_39_32 = ulSel_39_32 << 1; 
            }

            ulTempIndex ++;
        }

    } while( (ulTempIndex < TEMPERATURE_INDEX_MAX) && (ulTempStatus != 0) && (err == TM_OK) );

    if( err == TM_OK )
    {
        if (ulTempIndex < TEMPERATURE_INDEX_MAX)
        {
            *plTemp = TDA18280_Temperature[ulTempIndex];
        }
        else
        {
            err = TDA18280_ERR_HW_FAILED;
        }
    }
#else
    /* set default temperature */
    *plTemp = 500;
#endif

    return err;
}


/*============================================================================*/
/* FUNCTION:    iTDA18280_BlockPLL1ProgramTempLoopPrecharge                   */
/*                                                                            */
/* DESCRIPTION:                                                               */
/*                                                                            */
/* RETURN:                                                                    */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t iTDA18280_BlockPLL1ProgramTempLoopPrecharge(ptmTDA18280_Object_t pObj)
{   
    tmErrorCode_t   err = TM_OK;
    Int32 lTemp;
    UInt16  selTempLoopPrechargeOH;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    /* Read Temp */
    if(err == TM_OK)
    {
        err = iTDA18280_GetTemperatureMeasurement( pObj, tmTDA18280_TemperatureSourceLO, &lTemp );
    }

    if( lTemp <= 425 )
    {
        /* T <= 42.5 degrees */
        selTempLoopPrechargeOH = 0;
    }
    else if( lTemp <= 825 )
    {
        /* T <= 82.5 degrees */
        selTempLoopPrechargeOH = 1;
    }
    else if( lTemp <= 1175 )
    {
        /* T <= 117.5 degrees */
        selTempLoopPrechargeOH = 2;
    }
    else
    {
        /* T > 117.5 degrees */
        selTempLoopPrechargeOH = 3;
    }

    /* Programm selTempLoopPrechargeOH */
    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_crftech_pll_TESTVCO1__seltemploopprechargeoh,selTempLoopPrechargeOH);
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_SetStateDebugRam                                      */
/*                                                                            */
/* DESCRIPTION:                                                               */
/*                                                                            */
/* RETURN:                                                                    */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t iTDA18280_SetStateDebugRam(ptmTDA18280_Object_t pObj,
                                         tmTDA18280_DebugRamState_t eState)
{
    tmErrorCode_t   err = TM_OK;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    if (eState == tmTDA18280_DebugRamEnabled)
    {
        /* PRCR#558: the powerdown bit is reactivated */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_dbg_ram_POWERDOWN__powerdown, 0);
        }
        /* enable debug RAM clock */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_cgu_CLOCKS_CFG__dbg_ram_ena, 1);
        }
        /* release debug RAM reset */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_reset_RESET_DWN_DSP__dbg_ram, 1);
        }
    }
    else /* (eState == tmTDA18280_DebugRamDisabled) */
    {
        /* disable debug RAM clock */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_cgu_CLOCKS_CFG__dbg_ram_ena, 0);
        }
        /* release debug RAM reset */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_reset_RESET_DWN_DSP__dbg_ram, 0);
        }
        if (err == TM_OK)
        {
            /* PRCR#558: the powerdown bit is activated to avoid I2C read artefact */
            /* which can occur when reading reg_dbg_ram registers with disabled clock */
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_dbg_ram_POWERDOWN__powerdown, 1);
        }
    }
    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_GetStateDebugRam                                    */
/*                                                                            */
/* DESCRIPTION:                                                               */
/*                                                                            */
/* RETURN:                                                                    */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t iTDA18280_GetStateDebugRam(ptmTDA18280_Object_t pObj,
                                         ptmTDA18280_DebugRamState_t peState)
{
    tmErrorCode_t   err = TM_OK;
    UInt16          uClkRam = 0;
    UInt16          uRstRam = 0;

    *peState = tmTDA18280_DebugRamDisabled;

    if(err == TM_OK)
    {
        err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_cgu_CLOCKS_CFG__dbg_ram_ena, &uClkRam);
    }
    if(err == TM_OK)
    {
        err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_reset_RESET_DWN_DSP__dbg_ram, &uRstRam);
    }
    if(err == TM_OK)
    {
        if ((uClkRam == 1) && (uRstRam == 1))
        {
            *peState = tmTDA18280_DebugRamEnabled;
        }
        else
        {
            *peState = tmTDA18280_DebugRamDisabled;
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_SetConfigAndAcquireDebugRam                         */
/*                                                                            */
/* DESCRIPTION:                                                               */
/*                                                                            */
/* RETURN:                                                                    */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t iTDA18280_SetConfigAndAcquireDebugRam(ptmTDA18280_Object_t pObj,
                                                    tmTDA18280_DebugRamCtrlMode_t eMode,
                                                    tmTDA18280_DebugRamCtrlOneShot_t eOneShot,
                                                    UInt16 uNbSamples)
{
    tmErrorCode_t   err = TM_OK;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    /* reset ctrl mode */
    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_dbg_ram_CTRL__mode, 0);
    }

    /* set one shot or continuous acquisition */
    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_dbg_ram_CTRL__one_shot, (UInt16)eOneShot);
    }

    /* configure RAM size and addr */
    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_dbg_ram_I2C_ADDR, 0);
    }
    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_dbg_ram_END_ADDR, (UInt16)(uNbSamples-8));
    }

    /* launch capture */
    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_dbg_ram_CTRL__mode, (UInt16)eMode); 
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_DeviceReadDebugRam                                  */
/*                                                                            */
/* DESCRIPTION:                                                               */
/*                                                                            */
/* RETURN:                                                                    */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t iTDA18280_DeviceReadDebugRam(ptmTDA18280_Object_t pObj,
                                           tmTDA18280_DebugRamMode_t eMode,
                                           UInt16          RAMsize,
                                           Int16 *         pData
                                           )
{   
    tmErrorCode_t   err = TM_OK;
    UInt16 CaptureDone = 0;
    int nbLoop = 0;
    tmTDA18280_DebugRamState_t eState;
    Bool bRestoreDebugRamState = False;

    /* check parameters */
    if( RAMsize > RAM_SIZE )
    {
        err =TDA18280_ERR_BAD_PARAMETER;
    }

    /* acquire the debug RAM state to restore it at the end */
    if(err == TM_OK)
    {
        err = iTDA18280_GetStateDebugRam(pObj, &eState);
    }

    /* enable the debug RAM if needed */
    if (err == TM_OK)
    {
        if (eState == tmTDA18280_DebugRamDisabled)
        {
            bRestoreDebugRamState = True;
            err = iTDA18280_SetStateDebugRam(pObj, tmTDA18280_DebugRamEnabled);
        }
    }

    if(err == TM_OK)
    {
        if ( eMode == tmTDA18280_DebugRamMode_StoreNbAdc)
        {
            err = iTDA18280_SetConfigAndAcquireDebugRam(pObj, tmTDA18280_DebugRamCtrlMode_Store_Nb_Adc, tmTDA18280_DebugRamCtrlOneShot_OneShot, RAMsize);
        }
        else
        {
            err = iTDA18280_SetConfigAndAcquireDebugRam(pObj, tmTDA18280_DebugRamCtrlMode_Store_Adc, tmTDA18280_DebugRamCtrlOneShot_OneShot, RAMsize);
        }
    }

    /* wait for the capture in the debug ram to be finished */
    if(err == TM_OK)
    {
        do{
            err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_dbg_ram_STATUS__done, &CaptureDone); 
            nbLoop ++;
        } while ((CaptureDone == 0) && (nbLoop < STATUS_TIMEOUT));

        if( nbLoop >= STATUS_TIMEOUT )
        {
            err = TDA18280_ERR_HW_FAILED;
        }
    }

    /* retrieve the debug ram content */
    if(err == TM_OK)
    {
        err = iTDA18280_BlockDebugRamRead(pObj, RAMsize, pData);
    }

    /* restore the debug RAM state acquired at the beginning */
    if(err == TM_OK)
    {
        if (bRestoreDebugRamState)
        {
            err = iTDA18280_SetStateDebugRam(pObj, tmTDA18280_DebugRamDisabled);
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_DeviceAllocInstance:                                */
/*                                                                            */
/* DESCRIPTION: Allocates an instance.                                        */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_DeviceAllocInstance(
                              tmUnitSelect_t      tUnit,      /* I: Unit number */
                              pptmTDA18280_Object_t  ppDrvObject /* I: Driver Object */
                              )
{
    tmErrorCode_t       err = TDA18280_ERR_ERR_NO_INSTANCES;
    ptmTDA18280_Object_t   pObj = Null;
    UInt32              uLoopCounter = 0;

    /* Find a free instance */
    for (uLoopCounter = 0; uLoopCounter<TDA18280_MAX_UNITS; uLoopCounter++)
    {
        pObj = &gTDA18280Instance[uLoopCounter];
        if (pObj->init == False)
        {
            err = TM_OK;

            pObj->tUnit = tUnit;
            pObj->tUnitW = tUnit;

            err = iTDA18280_DeviceResetInstance(pObj);

            if (pObj->Marker != TDA18280_INSTANCE_CUSTOM_MARKER)
            {
                err = TDA18280_ERR_NOT_INITIALIZED;
            }

            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "Instance Init %d failed. Check parameters in tmbslTDA18280_instancecustom.h !", pObj->tUnitW));

            if(err == TM_OK)
            {
                *ppDrvObject = pObj;
            }
            break;
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_DeviceDeAllocInstance:                              */
/*                                                                            */
/* DESCRIPTION: Deallocates an instance.                                      */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_DeviceDeAllocInstance(
                                ptmTDA18280_Object_t  pDrvObject  /* I: Driver Object */
                                )
{
    tmErrorCode_t   err = TM_OK;

    if(pDrvObject != Null)
    {
        pDrvObject->init = False;
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_DeviceGetInstance:                                  */
/*                                                                            */
/* DESCRIPTION: Gets an instance.                                             */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_DeviceGetInstance(
                            tmUnitSelect_t      tUnit,      /* I: Unit number   */
                            pptmTDA18280_Object_t  ppDrvObject /* I: Driver Object */
                            )
{
    tmErrorCode_t       err = TDA18280_ERR_ERR_NO_INSTANCES;
    ptmTDA18280_Object_t   pObj = Null;
    UInt32              uLoopCounter = 0;

    /* Get instance */
    for (uLoopCounter = 0; uLoopCounter<TDA18280_MAX_UNITS; uLoopCounter++)
    {
        pObj = &gTDA18280Instance[uLoopCounter];
        if (pObj->init == True && pObj->tUnit == GET_INDEX_TYPE_TUNIT(tUnit))
        {
            pObj->tUnitW = tUnit;

            *ppDrvObject = pObj;
            err = TM_OK;
            break;
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_DeviceResetInstance:                                */
/*                                                                            */
/* DESCRIPTION: Resets an instance.                                           */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_DeviceResetInstance(
                              ptmTDA18280_Object_t  pDrvObject  /* I: Driver Object */
                              )
{
    tmErrorCode_t   err = TM_OK ;

    pDrvObject->curPowerState = tmTDA18280_PowerMax;
    pDrvObject->eHwState = tmTDA18280_HwState_InitNotDone;
    pDrvObject->EsVersion = tmTDA18280_EsVersionUnknown;

    /* TBD if new elements added to pObj */

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_DeviceGetHWVersion:                                 */
/*                                                                            */
/* DESCRIPTION: Get Hardware Version.                                         */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_DeviceGetHWVersion(
                               ptmTDA18280_Object_t   pObj
                               )
{
    tmErrorCode_t       err = TM_OK;
    UInt16              uIdentity = 0;
    UInt16              ID_byte_2 = 0;
    UInt16              majorRevision = 0;
    UInt16              minorRevision = 0;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    /* initialize to handle errors */
    pObj->EsVersion = tmTDA18280_EsVersionInvalid;
    pObj->tFeature.uMaxIndexSupported = 0;
    pObj->tFeature.bOOBSupported = False;
    pObj->tFeature.bDACSupported = False;

    err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_config_ID_BYTE_2__identifier, &ID_byte_2);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18280_I2CReadBitField(0x%08X) failed.", pObj->tUnit));

    if(err == TM_OK)
    {
        /* Construct Identity */
        uIdentity = ID_byte_2;

        if(uIdentity == 18280)
        {
            /* TDA18280 found. Check Major & Minor Revision*/
            err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_config_ID_BYTE_3__major_revision, &majorRevision);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18280_I2CReadBitField(0x%08X) failed.", pObj->tUnit));

            if (err == TM_OK)
            {
                err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_config_ID_BYTE_3__minor_revision, &minorRevision);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18280_I2CReadBitField(0x%08X) failed.", pObj->tUnit));
            }

            if (err == TM_OK)
            {
                switch (majorRevision)
                {
                case 0x0:
                    {
                        switch (minorRevision)
                        {
                        case 0x1:
                            /* TDA18280 ES1 */
                            pObj->EsVersion = tmTDA18280_EsVersion1;

                            tmDBGPRINTEx(DEBUGLVL_BLAB, "TDA18280 ES1");
                            break;

                        case 0x2:
                            /* TDA18280 ES2 */
                            pObj->EsVersion = tmTDA18280_EsVersion2;

                            tmDBGPRINTEx(DEBUGLVL_BLAB, "TDA18280 ES2");
                            break;

                        default:
                            /* if unsupported */
                            err = TDA18280_ERR_BAD_MODULE_ID;
                            break;
                        }
                    }
                    break;

                default:
                    /* if unsupported */
                    err = TDA18280_ERR_BAD_MODULE_ID;
                    break;
                }
            }

            if (err == TM_OK)
            {
                /* update supported features */
                if (pObj->eProductType == tmTDA18280_ProductTypeInvalid)
                {
                    err = TDA18280_ERR_BAD_MODULE_ID;
                }
                else /* product known */
                {
                    pObj->tFeature.uMaxIndexSupported = (pObj->eProductType == tmTDA18280_ProductTypeTDA18280 || \
                                                         pObj->eProductType == tmTDA18280_ProductTypeTDA18256 ? tmTDA18280_Demod_CDP_7 : \
                                                         pObj->eProductType == tmTDA18280_ProductTypeTDA18286 ||
                                                         pObj->eProductType == tmTDA18280_ProductTypeTDA18287 ?  tmTDA18280_Demod_CDP_5 : tmTDA18280_Demod_CDP_3);

                    pObj->tFeature.bOOBSupported = ((pObj->eProductType == tmTDA18280_ProductTypeTDA18285) ||
                                                    (pObj->eProductType == tmTDA18280_ProductTypeTDA18287));

                    pObj->tFeature.bDACSupported = (pObj->eProductType == tmTDA18280_ProductTypeTDA18256);
                }
            }
        }
        else
        {
            err = TDA18280_ERR_BAD_MODULE_ID;
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_ConfigureIOs                                        */
/*                                                                            */
/* DESCRIPTION: Config Inputs/Outputs                                         */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_ConfigureIOs(
        ptmTDA18280_Object_t    pObj
        )
{
    tmErrorCode_t   err = TM_OK;
    UInt16          default_io = 0;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    /* configure all output IOs in pull-down when disabled (50Kohms) */
    default_io = 0x36DB; /* reg_(pad)_EPD = 1 & reg_(pad)_EPUN = 1 */
    if (err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_config_IO_CTRL_INPUT_SET_0, default_io);
    }
    if (err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_config_IO_CTRL_INPUT_SET_1, default_io);
    }
    if (err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_config_IO_CTRL_INPUT_SET_2, default_io);
    }
    if (err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_config_IO_CTRL_INPUT_SET_3, default_io);
    }
    if (err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_config_IO_CTRL_INPUT_SET_4, default_io);
    }
    if (err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_config_IO_CTRL_INPUT_SET_5, default_io);
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_IrqConfiguration                                    */
/*                                                                            */
/* DESCRIPTION: Config IRQ                                                    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_IrqConfiguration(
        ptmTDA18280_Object_t    pObj
        )
{
    tmErrorCode_t       err = TM_OK;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    /* configure IRQ edge */
    if (err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_config_IRQ_EDGE, (pObj->IrqCfg.eIrqPolarity == tmTDA18280_IrqPolarity_ActiveHigh)?0:1);
    }

    /* enable interrupt */
    if (err == TM_OK)
    {
        err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_config_IO_DISABLE_OUTPUT_DRIVER__irq_en, (pObj->IrqCfg.eIrqMode == tmTDA18280_IrqMode_Enabled)?0:1);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    iTDA18280_BlockDebugRamRead                                   */
/*                                                                            */
/* DESCRIPTION: Read the debug ram                                            */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_BlockDebugRamRead( 
                                ptmTDA18280_Object_t   pObj,
                                UInt16          RAMsize,/**< I: Size of the RAM. */ 
                                Int16 *         pData   /* I: Pointer to data */
                                )
{
    tmErrorCode_t err = TM_OK;
    UInt32 i = 0;
    UInt16 value16;
    Int16 Signed_value16;
    Int16 *pDataWr = pData;   /* I: Pointer to data */

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_dbg_ram_CTRL__mode , 0); // reset
    }
    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_dbg_ram_I2C_ADDR, 0);
    }
    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_dbg_ram_END_ADDR, RAMsize-8);
    }
    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_dbg_ram_CTRL__mode, (UInt16)tmTDA18280_DebugRamCtrlMode_I2c_Rd);
    }

    /* read a first salve for even address to workaround speculative read DB_RAM bug */
    pDataWr = pData;   
    for (i = 0; i<(UInt16)(RAMsize/2); i++)
    {
        if(err == TM_OK)
        {
            err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_dbg_ram_RAM, 1, &value16);
            Signed_value16 = value16;
            Signed_value16 = (Int16)((Int16)(Signed_value16 << 3)>>3);  /* report sign bit on the 15th bit */
            *pDataWr  = Signed_value16 ;
            pDataWr += 2;
        }
    }

    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_dbg_ram_CTRL__mode , 0); // reset
    }
    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_dbg_ram_I2C_ADDR, 0+1);
    }
    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_dbg_ram_END_ADDR, RAMsize-8);
    }
    if(err == TM_OK)
    {
        err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_dbg_ram_CTRL__mode , (UInt16)tmTDA18280_DebugRamCtrlMode_I2c_Rd);
    }

    /* read a second salve for odd address to workaround speculative read DB_RAM bug */
    pDataWr = pData + 1 ;   
    for (i = 0; i<(UInt16)(RAMsize/2); i++)
    {
        if(err == TM_OK)
        {
            err = iTDA18280_I2CReadRegister(pObj, REGtda18280_reg_dbg_ram_RAM, 1, &value16);
            Signed_value16 = value16;
            Signed_value16 = (Int16)((Int16)(Signed_value16 << 3)>>3);  /* report sign bit on the 15th bit */
            *pDataWr  = Signed_value16 ;
            pDataWr += 2;
        }
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    iTDA18280_SetDigAgcMode                                       */
/*                                                                            */
/* DESCRIPTION: Change Dig AGC dynamic.                                       */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_SetDigAgcMode(
                        ptmTDA18280_Object_t pObj,
                        UInt8                Idx_ddc,
                        tmTDA18280_AgcMode_t eMode )
{
    tmErrorCode_t         err = TM_OK;
    UInt16                DigAgcTargetRms = 0;
    UInt16                AccuWindowSel = 0;
    UInt16                ErrorWindowSel = 0;
    UInt16                ThresholdHigh = 0;
    UInt16                ThresholdLow = 0;
    UInt16                ThresholdVeryHigh = 0;
    UInt16                ThresholdVeryLow = 0;
    UInt16                CableCut = 0;
    Bool                  isPathToDemod = True;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    /* if DAC is supported and 3 < path <= 7, the digital AGC gain must be compatible with the DAC */
    if (pObj->tFeature.bDACSupported && (Idx_ddc>tmTDA18280_Path_3) && (Idx_ddc<=tmTDA18280_Path_7))
    {
        isPathToDemod = False;
    }

    ThresholdHigh = TDA18280_DIG_AGC_TH_H_NORMAL;
    ThresholdLow = TDA18280_DIG_AGC_TH_L_NORMAL;

    if (eMode == tmTDA18280_AgcMode_Normal)
    {
        /* normal mode */
        AccuWindowSel = TDA18280_DIG_AGC_ACCU_WINDOW_SEL_NORMAL;
        ErrorWindowSel = TDA18280_DIG_AGC_ERROR_WINDOW_SEL_NORMAL;

        if (isPathToDemod)
        {
            ThresholdVeryLow = TDA18280_DIG_AGC_TH_VL_NORMAL;
            ThresholdVeryHigh = TDA18280_DIG_AGC_TH_VH_NORMAL;
            DigAgcTargetRms = TDA18280_DIG_AGC_TARGET_RMS;
            /* Cable Cut Ultra Low threshold must be set to -7dB vs default target value (0x40) */
            /* 10*log10((TARGET+UL)/TARGET) = -6.922 with UL = -51 (0xFCD)*/
            CableCut = TDA18280_DIG_AGC_TH_UL;
        }
        else /* path to DAC */
        {
            ThresholdVeryLow = TDA18280_DIG_AGC_TH_VL_NORMAL_FOR_DAC;
            ThresholdVeryHigh = TDA18280_DIG_AGC_TH_VH_NORMAL_FOR_DAC;
            DigAgcTargetRms = TDA18280_DIG_AGC_TARGET_RMS_FOR_DAC;
            /* Cable Cut Ultra Low threshold must be set to -7dB vs default target value (0x0B) */
            /* 10*log10((TARGET+UL)/TARGET) = -7.4 with UL = -9 (0xFF7)*/
            CableCut = TDA18280_DIG_AGC_TH_UL_FOR_DAC;
        }
    }
    else /* (eMode == tmTDA18280_AgcMode_Fast) */
    {
        /* fast mode - increase AGC dynamic */
        AccuWindowSel = TDA18280_DIG_AGC_ACCU_WINDOW_SEL_FAST;
        ErrorWindowSel = TDA18280_DIG_AGC_ERROR_WINDOW_SEL_FAST;

        if (isPathToDemod)
        {
            ThresholdVeryLow = TDA18280_DIG_AGC_TH_VL_FAST;
            ThresholdVeryHigh = TDA18280_DIG_AGC_TH_VH_FAST;
            DigAgcTargetRms = TDA18280_DIG_AGC_TARGET_RMS;
            /* disable cable cut */
            CableCut = TDA18280_DIG_AGC_TH_UL_DISABLE;
        }
        else /* path to DAC */
        {
            ThresholdVeryLow = TDA18280_DIG_AGC_TH_VL_FAST_FOR_DAC;
            ThresholdVeryHigh = TDA18280_DIG_AGC_TH_VH_FAST_FOR_DAC;
            DigAgcTargetRms = TDA18280_DIG_AGC_TARGET_RMS_FOR_DAC;
            /* disable cable cut */
            CableCut = TDA18280_DIG_AGC_TH_UL_DISABLE;
        }
    }

    if( Idx_ddc < tmTDA18280_Path_Aux )
    {
        /* wide band paths */
        if(err == TM_OK)
        {
            /* Digital AGC target is adjusted for CDP or DAC paths */
            err = iTDA18280_I2CWriteRegister(pObj, REGBO(tda18280_reg_ddc_slow0_TARGET, 2*Idx_ddc ), DigAgcTargetRms);
        }

        if( err == TM_OK )
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFBO(tda18280_reg_ddc_slow0_ESTIMATION_CONFIG__accu_window_sel, 2*Idx_ddc ), AccuWindowSel);
        }

        if( err == TM_OK )
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFBO(tda18280_reg_ddc_slow0_ESTIMATION_CONFIG__error_window_sel, 2*Idx_ddc ), ErrorWindowSel);
        }

        if( err == TM_OK )
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFBO(tda18280_reg_ddc_slow0_THRESHOLD_L__threshold_l, 2*Idx_ddc ), ThresholdLow);
        }

        if( err == TM_OK )
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFBO(tda18280_reg_ddc_slow0_THRESHOLD_H__threshold_h, 2*Idx_ddc ), ThresholdHigh);
        }

        if( err == TM_OK )
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFBO(tda18280_reg_ddc_slow0_THRESHOLD_VL__threshold_vl, 2*Idx_ddc ), ThresholdVeryLow);
        }

        if( err == TM_OK )
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFBO(tda18280_reg_ddc_slow0_THRESHOLD_VH__threshold_vh, 2*Idx_ddc ), ThresholdVeryHigh);
        }

        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteRegister (pObj, REGBO(tda18280_reg_ddc_slow0_THRESHOLD_UL, 2*Idx_ddc ), CableCut);
        }

        /* Cable Cut watch dog: granularity is 0.6ms -> 160 is 96ms */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteRegister (pObj, REGBO(tda18280_reg_ddc_slow0_CABLE_CUT, 2*Idx_ddc ), 160);
        }
    }
    else
    {
        /* Auxiliary */
        if( err == TM_OK )
        {
            err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_aux_ddc_DAGC_TARGET, DigAgcTargetRms);
        }

        if( err == TM_OK )
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_aux_ddc_DAGC_WINDOW__accu_window, AccuWindowSel);
        }

        if( err == TM_OK )
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_aux_ddc_DAGC_WINDOW__error_window, ErrorWindowSel);
        }

        if( err == TM_OK )
        {
            err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_aux_ddc_DAGC_K, TDA18280_DIG_AGC_K);
        }

        if( err == TM_OK )
        {
            err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_aux_ddc_DAGC_THRESHOLD_L, ThresholdLow);
        }

        if( err == TM_OK )
        {
            err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_aux_ddc_DAGC_THRESHOLD_H, ThresholdHigh);
        }

        if( err == TM_OK )
        {
            err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_aux_ddc_DAGC_THRESHOLD_VL, ThresholdVeryLow);
        }

        if( err == TM_OK )
        {
            err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_aux_ddc_DAGC_THRESHOLD_VH, ThresholdVeryHigh);
        }

        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteRegister (pObj, REGtda18280_reg_aux_ddc_DAGC_THRESHOLD_UL, CableCut);
        }

        /* Cable Cut watch dog: granularity is 0.6ms -> 160 is 96ms */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteRegister (pObj, REGtda18280_reg_aux_ddc_DAGC_CABLE_CUT, 160);
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_EnableWbRfAgc                                       */
/*                                                                            */
/* DESCRIPTION: Enable/Disable RF AGC (mixed signal AGC)                      */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_EnableWbRfAgc(
                        ptmTDA18280_Object_t pObj,
                        Bool bState)
{
    tmErrorCode_t  err = TM_OK;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    if (bState == True)
    {
        /* enable debug RAM */
        if (err == TM_OK)
        {
            err = iTDA18280_SetStateDebugRam(pObj, tmTDA18280_DebugRamEnabled);
        }
        /* enable RF AGC clock */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_cgu_CLOCKS_CFG__rfagc_ena, 1);
        }
        /* release RF AGC reset */
        if(err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_reset_RESET_DWN_DSP__rfagc, 1);
        }
        /* enable AGC_CLK_GATE and AGC_UP_DOWN outputs */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_config_IO_DISABLE_OUTPUT_DRIVER__agcclkgate_en, 0);
        }
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_config_IO_DISABLE_OUTPUT_DRIVER__agcupdown_en, 0);
        }
        /* enable RF AGC */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_rfagc_ENABLE_AGC, 1);
        }
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_rfagc_OUTPUT_CFG__output_enable, 1);
        }
        /* set up mode (RMS, Peak or RMS+Peak) */
        switch (pObj->RfAgcCfg.eRfAgcMode)
        {
        case tmTDA18280_RfAgcMode_RMS:
            if (err == TM_OK)
            {
                err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_rfagc_RMS_SAT_CMB, 0);
            }
            if (err == TM_OK)
            {
                err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_rfagc_RMS_SAT_SEL, 1);
            }
            break;
        case tmTDA18280_RfAgcMode_Peak:
            /* FIXME: not fully supported yet */
            err = TDA18280_ERR_NOT_SUPPORTED;
            /*if (err == TM_OK)
            {
                err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_rfagc_RMS_SAT_CMB, 0);
            }
            if (err == TM_OK)
            {
                err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_rfagc_RMS_SAT_SEL, 0);
            }*/
            break;
        case tmTDA18280_RfAgcMode_RMSAndPeak:
            /* FIXME: not fully supported yet */
            err = TDA18280_ERR_NOT_SUPPORTED;
            /*if (err == TM_OK)
            {
                err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_rfagc_RMS_SAT_CMB, 1);
            }
            if (err == TM_OK)
            {
                err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_rfagc_RMS_SAT_SEL, 1);
            }*/
            break;
        default:
            err = TDA18280_ERR_BAD_PARAMETER;
            break;
        }
        /* RMS_TARGET (24 bits): backoff -14dB */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_rfagc_RMS_TARGET_MSB, 0x0A);
        }
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_rfagc_RMS_TARGET_LSB, 0x2FC2);
        }
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_rfagc_RMS_TARGET_LOAD, 1);
        }
        /* RMS_THRES_POS (25 bits): hysteresis +0.2dB */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_rfagc_RMS_THRES_POS_MSB, 0x0);
        }
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_rfagc_RMS_THRES_POS_LSB, 0x7AE6);
        }
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_rfagc_RMS_THRES_POS_LOAD, 1);
        }
        /* RMS_THRES_NEG (25 bits): hysteresis -0.2dB */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_rfagc_RMS_THRES_NEG_MSB, 0x1FF);
        }
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_rfagc_RMS_THRES_NEG_LSB, 0x8AA2);
        }
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_rfagc_RMS_THRES_NEG_LOAD, 1);
        }
        /* RMS_DEC_FACTOR (25 bits): slope 0.0275dB/ms */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_rfagc_RMS_DEC_FACTOR_MSB, 0x4);
        }
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_rfagc_RMS_DEC_FACTOR_LSB, 0x0);
        }
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_rfagc_RMS_DEC_FACTOR_LOAD, 1);
        }
    }
    else /* (bState == False) */
    {
        /* disable RF AGC */
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_rfagc_ENABLE_AGC, 0);
        }
        if(err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_rfagc_OUTPUT_CFG__output_enable, 0);
        }
        /* disable AGC_CLK_GATE and AGC_UP_DOWN outputs */
        if(err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_config_IO_DISABLE_OUTPUT_DRIVER__agcclkgate_en, 1);
        }
        if(err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_config_IO_DISABLE_OUTPUT_DRIVER__agcupdown_en, 1);
        }
        /* disable RF AGC clock */
        if(err == TM_OK)
        {
            err = iTDA18280_I2CWriteBitField(pObj, BFtda18280_reg_cgu_CLOCKS_CFG__rfagc_ena, 0);
        }
        /* disable debug RAM */
        if (err == TM_OK)
        {
            err = iTDA18280_SetStateDebugRam(pObj, tmTDA18280_DebugRamDisabled);
        }
    }
    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_PathSet                                             */
/*                                                                            */
/* DESCRIPTION: Program path                                                  */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_PathSet(
                  ptmTDA18280_Object_t         pObj,
                  tmTDA18280_Path_t            tPathNumber,
                  const tmTDA18280_PathCfg_t*  ptPathCfg,
                  TDA18280_PathSetAGCMode_t    eMode
                  )
{
    tmErrorCode_t   err = TM_OK;
    tmTDA18280_Path_t tSwitchOffPathNumber = tmTDA18280_Path_Max;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    if (pObj==Null)
    {
        err = TDA18280_ERR_BAD_PARAMETER;
    }

    if (err == TM_OK)
    {
        if ((tPathNumber < tmTDA18280_Path_7 && tPathNumber > pObj->tFeature.uMaxIndexSupported ) ||
            (tPathNumber == tmTDA18280_Path_7 && tPathNumber > pObj->tFeature.uMaxIndexSupported && pObj->eOOBInput != tmTDA18280_Path_7 ) )
            
        err = TDA18280_ERR_NOT_SUPPORTED;
    }

    if((ptPathCfg->tPathStatus == tmTDA18280_ON)&&
       (pObj->PathCurrentState[tPathNumber] != tmTDA18280_ON) )
    {
        /* TDA18280_AUX_CDP_PATH and AUX can not be switched on together */
        /* except if AUX is used with OOB */
        if( pObj->eOOBInput != tmTDA18280_Path_Aux )
        {
            /* Aux path share a CDP with another path */
            if( tPathNumber == TDA18280_AUX_CDP_PATH )
            {
                tSwitchOffPathNumber = tmTDA18280_Path_Aux;
            }
            else if( tPathNumber == tmTDA18280_Path_Aux )
            {
                tSwitchOffPathNumber = TDA18280_AUX_CDP_PATH;
            }

            /* switch off the path if already switched on */
            if( (tSwitchOffPathNumber < tmTDA18280_Path_Max) && (pObj->PathCurrentState[tSwitchOffPathNumber] == tmTDA18280_ON) )
            {
                if( err == TM_OK )
                {
                    err = iTDA18280_PathStop(pObj, tSwitchOffPathNumber);
                }
            }
        }
    }

    if((ptPathCfg->tPathStatus == tmTDA18280_OFF)&&
       (pObj->PathCurrentState[tPathNumber] != tmTDA18280_OFF))
    {
        if(err == TM_OK)
        {
            err = iTDA18280_PathStop(pObj, tPathNumber);
        }
    }

    if (ptPathCfg->tPathStatus == tmTDA18280_ON)
    {
        if(err == TM_OK)
        {
            err = iTDA18280_PathStart(pObj, tPathNumber, ptPathCfg, eMode);
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_PathGet                                             */
/*                                                                            */
/* DESCRIPTION: Retrieve the configuration of the specified path              */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_PathGet(
                  ptmTDA18280_Object_t         pObj,
                  tmTDA18280_Path_t            tPathNumber,
                  tmTDA18280_PathCfg_t*        ptPathCfg
                  )
{
    tmErrorCode_t       err = TM_OK;
    tmUnitSelect_t      tUnitCdp = 0;
    tmTDA18280_Demod_t  DemodIndex = tmTDA18280_Demod_Max;
    UInt8               Clock_OK = 0;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    /* the path is supposed to be ON */

    if (pObj==Null)
    {
        err = TDA18280_ERR_BAD_PARAMETER;
    }

    /* retrieve the gain */
    if (err == TM_OK)
    {
        err = iTDA18280_PathReadGain(pObj, tPathNumber);
    }

    /* apply the gain read to the new structure */
    if (err == TM_OK)
    {
        ptPathCfg->uGainMultBy10 = pObj->Path[tPathNumber].uGainMultBy10;
    }

    /* retrieve demod information, except when disabled when DAC supported and paths 4-7 (DDCs to DAC) */

    if (!(pObj->tFeature.bDACSupported && tPathNumber >= tmTDA18280_Path_4 && tPathNumber <= tmTDA18280_Path_7))
    {
        if (err == TM_OK)
        {
            err = iTDA18280_GetDemodIndex(pObj, tPathNumber, &DemodIndex);
        }

        if (err == TM_OK)
        {
            err = iTDA18280_PathDemodClockCheck(pObj, DemodIndex, &Clock_OK);
        }

        if (err == TM_OK)
        {
            if (Clock_OK == 1)
            {
                if (DemodIndex <= tmTDA18280_Demod_CDP_7)
                {
                    /* demodulator is CDP */
                    /* tunit is modified in order to indicate CDP path */
                    tUnitCdp = pObj->tUnitW + ADDITIONNAL_INFO_VAL(DemodIndex);

                    /* retrieve the Demod FEC mode */
                    if (err == TM_OK)
                    {
                        err = tmbslHCDP_GetFECMode(tUnitCdp, &ptPathCfg->eFECMode);
                    }

                    /* retrieve the Demod configured symbol rate */
                    if (err == TM_OK)
                    {
                        err = tmbslHCDP_GetConfiguredSR(tUnitCdp, &ptPathCfg->uSR);
                    }

                    /* retrieve the Demod modulation */
                    if (err == TM_OK)
                    {
                        err = tmbslHCDP_GetMod( tUnitCdp, &ptPathCfg->eMod );
                    }

                    /* retrieve the Demod configured spectal inversion */
                    if (err == TM_OK)
                    {
                        err = tmbslHCDP_GetConfiguredSI(tUnitCdp, &ptPathCfg->eSI);
                    }
                }
                else if (DemodIndex == tmTDA18280_Demod_OOB)
                {
                    /* demodulator is OOB */
                    /* retrieve the Demod FEC mode */
                    if (err == TM_OK)
                    {
                        err = tmbslOOBIp_GetFECMode(pObj->tUnitW, &ptPathCfg->eFECMode);
                    }

                    /* retrieve the Demod configured symbol rate */
                    if (err == TM_OK)
                    {
                        err = tmbslOOBIp_GetConfiguredSR(pObj->tUnitW, &ptPathCfg->uSR);
                    }

                    /* retrieve the Demod modulation */
                    if (err == TM_OK)
                    {
                        err = tmbslOOBIp_GetMod( pObj->tUnitW, &ptPathCfg->eMod );
                    }

                    /* retrieve the Demod configured spectal inversion */
                    if (err == TM_OK)
                    {
                        err = tmbslOOBIp_GetSI(pObj->tUnitW, &ptPathCfg->eSI);
                    }
                }
            }
        }
    }

    return err;
}

/*============================================================================*/
/* iTDA18280_PathStart                                                        */
/*============================================================================*/
tmErrorCode_t
iTDA18280_PathStart(ptmTDA18280_Object_t            pObj,
                    tmTDA18280_Path_t               uPathNumber,
                    const tmTDA18280_PathCfg_t*     ptPathCfg,
                    TDA18280_PathSetAGCMode_t       eMode
                    )
{
    tmErrorCode_t err = TM_OK;

    /* sequence for Off to On transition: DDC -> CDP/OOB/DAC -> TS */

    /* DDC part */
    if(err == TM_OK)
    {
        err = iTDA18280_PathDdcStartup(pObj, uPathNumber);
    }

    if (uPathNumber < tmTDA18280_Path_Aux)
    {
        /* configure WB DDC */
        if(err == TM_OK)
        {
            err = iTDA18280_PathConfig(pObj, uPathNumber, ptPathCfg, eMode);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18280_PathConfig(0x%08X) failed.", pObj->tUnitW));
        }
    }
    else if (uPathNumber == tmTDA18280_Path_Aux) /* Auxiliary path */
    {
        /* configure NB DDC */
        if(err == TM_OK)
        {
            err = iTDA18280_PathAuxConfig(pObj, ptPathCfg);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18280_PathAuxConfig(0x%08X) failed.", pObj->tUnitW));
        }
    }
    else
    {
        tmDBGPRINTEx(DEBUGLVL_BLAB, "!! ERROR !!: Path %d not supported", (UInt16)uPathNumber);
        err = TDA18280_ERR_NOT_INITIALIZED;
    }

    /* demod part */
    if(err == TM_OK)
    {
        err = iTDA18280_PathDemodStartup(pObj, uPathNumber, ptPathCfg);
    }

    /* TS part */
    if (err == TM_OK)
    {
        err = iTDA18280_PathTsOutputStartup(pObj, uPathNumber);
    }

    /* update path current state */
    if (err == TM_OK)
    {
        pObj->PathCurrentState[uPathNumber] = tmTDA18280_ON;
    }

    return err;
}


/*============================================================================*/
/* iTDA18280_PathStop                                                         */
/*============================================================================*/
tmErrorCode_t
iTDA18280_PathStop( ptmTDA18280_Object_t pObj, tmTDA18280_Path_t uPathNumber)
{
    tmErrorCode_t err = TM_OK;

    /* sequence for On to Off transition: TS -> CDP/OOB/DAC -> DDC */

    /* TS part */
    if (err == TM_OK)
    {
        err = iTDA18280_PathTsOutputStop(pObj, uPathNumber);
    }

    /* demod part */
    if(err == TM_OK)
    {
        err = iTDA18280_PathDemodStop(pObj, uPathNumber);
    }

    /* DDC part */
    if(err == TM_OK)
    {
        err = iTDA18280_PathDdcStop(pObj, uPathNumber);
    }

    /* update path current state */
    if (err == TM_OK)
    {
        pObj->PathCurrentState[uPathNumber] = tmTDA18280_OFF;
    }

    return err;
}

#ifdef _TVFE_IMPLEMENT_MUTEX

/*============================================================================*/
/* FUNCTION:    iTDA18280_MutexAcquire:                                       */
/*                                                                            */
/* DESCRIPTION: Acquires driver mutex.                                        */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_MutexAcquire(
                       ptmTDA18280_Object_t   pObj,
                       UInt32              timeOut
                       )
{
    tmErrorCode_t   err = TDA18280_ERR_NULL_CONTROLFUNC;

    /* tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, tUnit); */

    if(P_SMUTEX_ACQUIRE_VALID && P_MUTEX_VALID)
    {
        err = P_SMUTEX_ACQUIRE(P_MUTEX, timeOut);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "Mutex_Acquire(0x%08X, %d) failed.", pObj->tUnitW, timeOut));
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_MutexRelease:                                       */
/*                                                                            */
/* DESCRIPTION: Releases driver mutex.                                        */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_MutexRelease(
                       ptmTDA18280_Object_t   pObj
                       )
{
    tmErrorCode_t   err = TDA18280_ERR_NULL_CONTROLFUNC;

    /* tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, tUnit); */

    if(P_SMUTEX_RELEASE_VALID && P_MUTEX_VALID)
    {
        err = P_SMUTEX_RELEASE(P_MUTEX);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "Mutex_Release(0x%08X) failed.", pObj->tUnitW));
    }

    return err;
}

#endif

/*============================================================================*/
/* FUNCTION:    iTDA18280_GetDemodIndex:                                      */
/*                                                                            */
/* DESCRIPTION: Get the CDP/OOB demod index matching with the path index      */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_GetDemodIndex( ptmTDA18280_Object_t pObj, tmTDA18280_Path_t PathIndex, tmTDA18280_Demod_t *pDemodIndex)
{
    tmErrorCode_t err = TM_OK;

    /* initialize to handle errors */
    if ((err == TM_OK) && (pDemodIndex != Null))
    {
        *pDemodIndex = tmTDA18280_Demod_Max;
    }
    else
    {
        err = TDA18280_ERR_BAD_PARAMETER;
    }

    if (pObj == Null)
    {
        err = TDA18280_ERR_BAD_PARAMETER;
    }

    if (err == TM_OK)
    {
        if (pObj->tFeature.bDACSupported && PathIndex >= tmTDA18280_Path_4 && PathIndex <= tmTDA18280_Path_7)
        {
            *pDemodIndex = tmTDA18280_Demod_None;
        }
        else
        {
            switch (PathIndex)
            {
            case tmTDA18280_Path_7:
                if (pObj->eOOBInput != tmTDA18280_Path_7)
                {
                    /* DDC7 is connected to CDP7 */
                    *pDemodIndex = tmTDA18280_Demod_CDP_7;
                }
                else /* (pObj->eOOBInput == tmTDA18280_Path_7) */
                {
                    /* DDC7 is connected to OOB */
                    *pDemodIndex = tmTDA18280_Demod_OOB;
                }
                break;
            case tmTDA18280_Path_Aux:
                if (pObj->eOOBInput != tmTDA18280_Path_Aux)
                {
                    /* AUX is connected to CDP2 */
                    *pDemodIndex = tmTDA18280_Demod_CDP_2;
                }
                else /* (pObj->eOOBInput == tmTDA18280_Path_Aux) */
                {
                    /* AUX is connected to OOB */
                    *pDemodIndex = tmTDA18280_Demod_OOB;
                }
                break;
            default:
                /* in other cases, the CDP demod matches the path (DDC) */
                *pDemodIndex = (tmTDA18280_Demod_t)PathIndex;
                break;
            }
            /* check selected CDP usable by current product */
            if (( *pDemodIndex <= tmTDA18280_Demod_CDP_7 && *pDemodIndex > pObj->tFeature.uMaxIndexSupported ) ||
               ( *pDemodIndex == tmTDA18280_Demod_OOB && !pObj->tFeature.bOOBSupported ) )
            {
                err = TDA18280_ERR_NOT_SUPPORTED;
            }
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_GetTsIndex:                                         */
/*                                                                            */
/* DESCRIPTION: Get the TS index matching with the path index                 */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_GetTsIndex( ptmTDA18280_Object_t pObj, tmTDA18280_Path_t PathIndex, tmTDA18280_Ts_t *pTsIndex)
{
    tmErrorCode_t err = TM_OK;

    *pTsIndex = tmTDA18280_Ts_Max;

    if (pObj == Null)
    {
        err = TDA18280_ERR_BAD_PARAMETER;
    }

    if (!pObj->tFeature.bDACSupported)
    {
        if (PathIndex < tmTDA18280_Path_Aux)
        {
            /* Path 7 cannot use TS7 when OOB is on AUX */
            if ((PathIndex == tmTDA18280_Path_7) && (pObj->eOOBInput == tmTDA18280_Path_Aux))
            {
                tmDBGPRINTEx(DEBUGLVL_BLAB, "!! ERROR !!: Path 7 cannot use TS7 when OOB uses the auxiliary path ");
                err = TDA18280_ERR_ALREADY_SETUP;
            }
            else
            {
                /* TS index matches path index */
                *pTsIndex = (tmTDA18280_Ts_t)PathIndex;
            }
        }
        else if (PathIndex == tmTDA18280_Path_Aux) /* Auxiliary path */
        {
            if (pObj->eOOBInput == tmTDA18280_Path_Aux)
            {
                *pTsIndex = tmTDA18280_Ts_7; /* AUX uses TS7 */
            }
            else
            {
                *pTsIndex = tmTDA18280_Ts_2; /* AUX uses TS2 */
            }
        }
        else
        {
            tmDBGPRINTEx(DEBUGLVL_BLAB, "!! ERROR !!: Path %d not supported", PathIndex);
            err = TDA18280_ERR_NOT_INITIALIZED;
        }
    }
    else /* DAC supported */
    {
        if (PathIndex >= tmTDA18280_Path_0 && PathIndex <= tmTDA18280_Path_3)
        {
            /* TS index matches path index */
            *pTsIndex = (tmTDA18280_Ts_t)PathIndex;
        }
        else if (PathIndex >= tmTDA18280_Path_4 && PathIndex <= tmTDA18280_Path_7)
        {
            /* no TS */
            *pTsIndex = tmTDA18280_Ts_Max;
        }
        else if (PathIndex == tmTDA18280_Path_Aux) /* Auxiliary path */
        {
            if (pObj->eOOBInput == tmTDA18280_Path_Max)
            {
                *pTsIndex = tmTDA18280_Ts_2; /* AUX uses TS2 */
            }
            else
            {
                tmDBGPRINTEx(DEBUGLVL_BLAB, "!! ERROR !!: OOB not supported ");
                err = TDA18280_ERR_NOT_SUPPORTED;
            }
        }
        else
        {
            tmDBGPRINTEx(DEBUGLVL_BLAB, "!! ERROR !!: Path %d not supported", PathIndex);
            err = TDA18280_ERR_NOT_INITIALIZED;
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_GetPathIndexFromDemod:                              */
/*                                                                            */
/* DESCRIPTION: Get the path index matching with the CDP/OOB demod index      */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_GetPathIndexFromDemod(ptmTDA18280_Object_t pObj, tmTDA18280_Demod_t DemodIndex, tmTDA18280_Path_t *pPathIndex)
{
    tmErrorCode_t err = TM_OK;
    UInt16 uValue = 0;

    /* initialize to handle errors */
    if ((err == TM_OK) && (pPathIndex != Null))
    {
        *pPathIndex = tmTDA18280_Path_Max;
    }
    else
    {
        err = TDA18280_ERR_BAD_PARAMETER;
    }

    if (pObj == Null)
    {
        err = TDA18280_ERR_BAD_PARAMETER;
    }

    if (err == TM_OK)
    {
        if (pObj->tFeature.bDACSupported && DemodIndex >= tmTDA18280_Demod_CDP_4 && DemodIndex <= tmTDA18280_Demod_CDP_7)
        {
            *pPathIndex = tmTDA18280_Path_Max;
        }
        else
        {
            switch (DemodIndex)
            {
            case tmTDA18280_Demod_CDP_2:
                /* check the state of the switch for DDC2 */
                err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_config_DEMOD_TS_IO_CTRL__ddc_nbddc_to_dmd_select, &uValue);
                if (uValue == 0)
                {
                    /* DDC2 is connected to CDP2 */
                    *pPathIndex = tmTDA18280_Path_2;
                }
                else
                {
                    /* AUX is connected to CDP2 */
                    *pPathIndex = tmTDA18280_Path_Aux;
                }
                break;
            case tmTDA18280_Demod_OOB:
                if (pObj->eOOBInput == tmTDA18280_Path_7)
                {
                    /* DDC7 is connected to OOB */
                    *pPathIndex = tmTDA18280_Path_7;
                }
                else if (pObj->eOOBInput == tmTDA18280_Path_Aux)
                {
                    /* AUX is connected to OOB */
                    *pPathIndex = tmTDA18280_Path_Aux;
                }
                break;
            default:
                /* in other cases, the path (DDC) matches the CDP demod */
                *pPathIndex = (tmTDA18280_Path_t)DemodIndex;
                break;
            }
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18280_GetPathIndexFromTs:                                 */
/*                                                                            */
/* DESCRIPTION: Get the path index matching with the TS index                 */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18280_GetPathIndexFromTs(ptmTDA18280_Object_t pObj, tmTDA18280_Ts_t TsIndex, tmTDA18280_Path_t *pPathIndex)
{
    tmErrorCode_t err = TM_OK;
    UInt16 uValue = 0;

    /* initialize to handle errors */
    if ((err == TM_OK) && (pPathIndex != Null))
    {
        *pPathIndex = tmTDA18280_Path_Max;
    }
    else
    {
        err = TDA18280_ERR_BAD_PARAMETER;
    }

    if (pObj == Null)
    {
        err = TDA18280_ERR_BAD_PARAMETER;
    }

    if (err == TM_OK)
    {
        if (pObj->tFeature.bDACSupported && TsIndex >= tmTDA18280_Ts_4 && TsIndex <= tmTDA18280_Ts_7)
        {
            *pPathIndex = tmTDA18280_Path_Max;
        }
        else
        {
            switch (TsIndex)
            {
            case tmTDA18280_Ts_2:
                /* check the state of the switch for DDC2 */
                err = iTDA18280_I2CReadBitField(pObj, BFtda18280_reg_config_DEMOD_TS_IO_CTRL__ddc_nbddc_to_dmd_select, &uValue);
                if (uValue == 0)
                {
                    /* DDC2 is connected to CDP2/TS2 */
                    *pPathIndex = tmTDA18280_Path_2;
                }
                else
                {
                    /* AUX is connected to CDP2/TS2 */
                    *pPathIndex = tmTDA18280_Path_Aux;
                }
                break;
            case tmTDA18280_Ts_7:
                if (pObj->eOOBInput == tmTDA18280_Path_Max)
                {
                    /* DDC7 is connected to CDP */
                    *pPathIndex = tmTDA18280_Path_7;
                }
                else if (pObj->eOOBInput == tmTDA18280_Path_7)
                {
                    /* DDC7 is connected to OOB */
                    *pPathIndex = tmTDA18280_Path_7;
                }
                else if (pObj->eOOBInput == tmTDA18280_Path_Aux)
                {
                    /* AUX is connected to OOB */
                    *pPathIndex = tmTDA18280_Path_Aux;
                }
                break;
            default:
                /* in other cases, the path (DDC) matches the TS */
                *pPathIndex = (tmTDA18280_Path_t)TsIndex;
                break;
            }
        }
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    iTDA18280_Log:                                                */
/*                                                                            */
/* DESCRIPTION: Get the log                                                   */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
UInt32 
iTDA18280_Log( UInt32 ulValue )
{
    UInt32 ulLogValue;
    UInt32 ulIndex = 0;

    if (ulValue > 0)
    {
        // find out the log value in the table
        while ( (gTDA18280LogTable[ulIndex].uX < ulValue) && (gTDA18280LogTable[ulIndex].uX != 0) )
        {
            ulIndex ++;
        }

        ulLogValue = gTDA18280LogTable[ulIndex].uLogX;
    }
    else
    {
        // error case
        ulLogValue = 0;
    }

    return ulLogValue;
}

/*============================================================================*/
/*                   DEMOD FUNCTIONS                                          */
/*============================================================================*/


/*============================================================================*/
/* iTDA18280_CdpIQPreset                                                      */
/*============================================================================*/
tmErrorCode_t
iTDA18280_CdpIQPreset
(
    ptmTDA18280_Object_t              pObj,
    tmTDA18280_Demod_t                uCdpIndex,
    tmTDA18280_ConstellationSource_t  eSource
)
{
    tmErrorCode_t       err = TM_OK;
    tmUnitSelect_t      tCdpUnit;
    HCDP_ConstellationSource_t  sConstellationSource;
    tmTDA18280_Path_t   tPathNumber = tmTDA18280_Path_Max;

    /* tUnit is modified in order to indicate CDP path */
    tCdpUnit = pObj->tUnitW + ADDITIONNAL_INFO_VAL((UInt16)uCdpIndex);

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    if (err == TM_OK)
    {
        err = iTDA18280_GetPathIndexFromDemod(pObj, uCdpIndex, &tPathNumber);
    }

    if (err == TM_OK)
    {
        pObj->PathCfgFixed[tPathNumber].sConstSource = eSource;

        switch (eSource)
        {
        case tmTDA18280_ConstellationSourceADC:
            sConstellationSource = HCDP_ConstellationSource_ADC;
            break;
        case tmTDA18280_ConstellationSourceFEDR:
            sConstellationSource = HCDP_ConstellationSource_FEDR;
            break;
        case tmTDA18280_ConstellationSourcePDF:
            sConstellationSource = HCDP_ConstellationSource_PDF;
            break;
        case tmTDA18280_ConstellationSourceDAGC:
            sConstellationSource = HCDP_ConstellationSource_DAGC;
            break;
        case tmTDA18280_ConstellationSourceMF:
            sConstellationSource = HCDP_ConstellationSource_MF;
            break;
        case tmTDA18280_ConstellationSourceCAGC:
            sConstellationSource = HCDP_ConstellationSource_CAGC;
            break;
        case tmTDA18280_ConstellationSourceEqualizer:
            sConstellationSource = HCDP_ConstellationSource_Equalizer;
            break;
        case tmTDA18280_ConstellationSourceBEDR:
            sConstellationSource = HCDP_ConstellationSource_BEDR;
            break;
        default:
            err = TDA18280_ERR_NOT_SUPPORTED;
            break;
        }
    }

    if (err == TM_OK)
    {
        err = tmbslHCDP_ConstPreset(tCdpUnit, sConstellationSource);
    }

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18280_CdpIQPreset(0x%08X) failed.", tCdpUnit));

    return err;
}

/*============================================================================*/
/* iTDA18280_CdpOobHwInit                                                     */
/*============================================================================*/
tmErrorCode_t
iTDA18280_CdpOobHwInit
(
    ptmTDA18280_Object_t              pObj
)
{
    tmErrorCode_t  err = TM_OK;

    /* enable all CDP and OOB clocks */
    if (err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_cgu_CLOCKS_DEMOD_CFG, 0x1FF);
    }

    /* force all CDP and OOB reset */
    if (err == TM_OK)
    {
        /* force bits 0:8 to 0 */
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_reset_RESET_DEMOD, 0x000);
    }

    /* wait 1ms */
    if (err == TM_OK)
    {
        err = iTDA18280_UtilWait(pObj, 1); 
    }

    /* release all CDP and OOB reset */
    if (err == TM_OK)
    {
        /* force bits 0:8 to 1 */
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_reset_RESET_DEMOD, 0x1FF);
    }

    /* CDP and OOB HwInit function calls are disabled here (commented out below) */
    /* - HwInit is done each time the demod is enabled in iTDA18280_PathDemodStartup */
    /* - each time the demod clock is enabled, the demod block must be reset, hence HwInit is called */

    ///* initialise all CDPs */
    //if (err == TM_OK)
    //{
    //    ulIndex = 0;
    //    do
    //    {
    //        err = iTDA18280_CdpHwInit(pObj, ulIndex);
    //        ulIndex ++;
    //    } while( (ulIndex < TDA18280_NB_CDP) && (err == TM_OK) );
    //    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18280_CdpHwInit(0x%08X) failed.", pObj->tUnitW));
    //}

    ///* initialise OOB */
    //if (err == TM_OK)
    //{
    //    err = iTDA18280_OobHwInit(pObj);
    //    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18280_OobHwInit(0x%08X) failed.", pObj->tUnitW));
    //}

    /* disable all CDP and OOB clocks */
    if (err == TM_OK)
    {
        err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_cgu_CLOCKS_DEMOD_CFG, 0x000);
    }

    return err;
}

/*============================================================================*/
/* iTDA18280_CdpHwInit                                                        */
/*============================================================================*/
tmErrorCode_t
iTDA18280_CdpHwInit
(
    ptmTDA18280_Object_t              pObj,
    UInt32                            uCdpIndex
)
{
    tmErrorCode_t       err = TM_OK;
    tmUnitSelect_t      tCdpUnit;
    HCDP_ExtendSymbolRateMode_t eExtendSRMode = HCDP_ExtendSymbolRateModeInvalid;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    /* tUnit is modified in order to indicate CDP path */
    if (err == TM_OK)
    {
        tCdpUnit = pObj->tUnitW + ADDITIONNAL_INFO_VAL((UInt16)uCdpIndex);
    }

    if (err == TM_OK)
    {
        err = tmbslHCDP_Reset(tCdpUnit);
    }

    if (err == TM_OK)
    {
        err = iTDA18280_CdpIQPreset(pObj, (tmTDA18280_Demod_t)uCdpIndex, tmTDA18280_ConstellationSourceBEDR);
    }

    if (err == TM_OK)
    {
        switch (pObj->PathCfgFixed[uCdpIndex].eExtendSRMode)
        {
        case tmTDA18280_ExtendSymbolRateModeEnable700ppm:
            eExtendSRMode = HCDP_ExtendSymbolRateModeEnable700ppm;
            break;
        case tmTDA18280_ExtendSymbolRateModeEnable1500ppm:
            eExtendSRMode = HCDP_ExtendSymbolRateModeEnable1500ppm;
            break;
        case tmTDA18280_ExtendSymbolRateModeDisable:
            eExtendSRMode = HCDP_ExtendSymbolRateModeDisable;
            break;
        default:
            eExtendSRMode = HCDP_ExtendSymbolRateModeInvalid;
            break;
        }

        /* configure config Extended SR mode */
        err = tmbslHCDP_SetExtendSymbolRateRange(tCdpUnit, eExtendSRMode);
    }

    return err;
}

/*============================================================================*/
/* iTDA18280_CdpConfig                                                        */
/*============================================================================*/
tmErrorCode_t
iTDA18280_CdpConfig
(
    ptmTDA18280_Object_t         pObj,
    tmTDA18280_Path_t            tPathNumber,
    const tmTDA18280_PathCfg_t*  ptPathCfg
    )
{
    tmErrorCode_t   err = TM_OK;
    tmUnitSelect_t  tUnitCdp = 0;
    tmTDA18280_Demod_t  CdpIndex = tmTDA18280_Demod_Max;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    /* configure CDP */
    if(err == TM_OK)
    {
        err = iTDA18280_GetDemodIndex(pObj, tPathNumber, &CdpIndex);
    }

    /* get CDP tUnit matching with the instance */
    if (err == TM_OK)
    {
        tUnitCdp = pObj->tUnitW + ADDITIONNAL_INFO_VAL(CdpIndex);
    }

    if (err == TM_OK)
    {
        err = tmbslHCDP_SetFECMode( tUnitCdp, ptPathCfg->eFECMode );
    }

    if(err == TM_OK)
    {
        err = tmbslHCDP_SetIF( tUnitCdp, TDA18280_DEMOD_IF );
    }

    if(err == TM_OK)
    {
        err = tmbslHCDP_SetSI( tUnitCdp, ptPathCfg->eSI );
    }

    /* adjust equalizer settings depending on the path */
    if(err == TM_OK)
    {
        if (tPathNumber == tmTDA18280_Path_Aux)
        {
            if (ptPathCfg->eMod == tmFrontEndModulationQam64)
            {
                err = tmbslHCDP_SelectEqualizerSettings(tUnitCdp, 0xC, 0xD);
            }
            else if (ptPathCfg->eMod == tmFrontEndModulationQam128)
            {
                err = tmbslHCDP_SelectEqualizerSettings(tUnitCdp, 0xC, 0xF);
            }
            else if (ptPathCfg->eMod == tmFrontEndModulationQam256)
            {
                err = tmbslHCDP_SelectEqualizerSettings(tUnitCdp, 0xD, 0xD);
            }
            else
            {
                err = tmbslHCDP_SelectEqualizerSettings(tUnitCdp, 0xC, 0xF);
            }
        }
        else /* (tPathNumber != tmTDA18280_Path_Aux) */
        {
            if (ptPathCfg->eMod == tmFrontEndModulationQam64)
            {
                err = tmbslHCDP_SelectEqualizerSettings(tUnitCdp, 0xC, 0xD);
            }
            else if (ptPathCfg->eMod == tmFrontEndModulationQam128)
            {
                err = tmbslHCDP_SelectEqualizerSettings(tUnitCdp, 0xB, 0xD);
            }
            else if (ptPathCfg->eMod == tmFrontEndModulationQam256)
            {
                err = tmbslHCDP_SelectEqualizerSettings(tUnitCdp, 0xA, 0xA);
            }
            else
            {
                err = tmbslHCDP_SelectEqualizerSettings(tUnitCdp, 0xB, 0xF);
            }
        }
    }

    if(err == TM_OK)
    {
        err = tmbslHCDP_SetMod( tUnitCdp, ptPathCfg->eMod );
    }

    if(err == TM_OK)
    {
        err = tmbslHCDP_SetSR( tUnitCdp, ptPathCfg->uSR );
    }

    /* start CDP lock */
    if(err == TM_OK)
    {
        err = tmbslHCDP_StartLock( tUnitCdp );
    }

    return err;
}

/*============================================================================*/
/* iTDA18280_OobHwInit                                                        */
/*============================================================================*/
tmErrorCode_t
iTDA18280_OobHwInit
(
    ptmTDA18280_Object_t              pObj
)
{
    tmErrorCode_t       err = TM_OK;
    unsigned long long ulOob_freq_shift = 0;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "%s (0x%08X)", __FUNCTION__, pObj->tUnit);

    /* configure OOB IF */
    if (err == TM_OK)
    {
       // ulOob_freq_shift = (unsigned long long)0x2000000 * (TDA18280_OOB_IF/1000); /* IF in KHz */ //forrest
       // ulOob_freq_shift /= (unsigned long long)(TDA18280_DEMOD_INPUT_SAMPLE_FREQ/1000); /* Fsampling in KHz */ //forrest
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_config_OOB_FREQ_SHIFT_MSB, (UInt16)((ulOob_freq_shift>>16)&0xFFFF));
        }
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_config_OOB_FREQ_SHIFT_LSB, (UInt16)(ulOob_freq_shift&0xFFFF));
        }
        if (err == TM_OK)
        {
            err = iTDA18280_I2CWriteRegister(pObj, REGtda18280_reg_config_OOB_FREQ_SHIFT_LOAD, 1);
        }
    }

    if (err == TM_OK)
    {
        /* initialise OOB */
        err = tmbslOOBIp_Reset(pObj->tUnitW);
    }

    return err;
}

/*============================================================================*/
/* iTDA18280_OobConfig                                                        */
/*============================================================================*/
tmErrorCode_t
iTDA18280_OobConfig
(
    ptmTDA18280_Object_t         pObj,
    const tmTDA18280_PathCfg_t*  ptPathCfg
)
{
    tmErrorCode_t        err = TM_OK;
    tmFrontEndFECMode_t  eFECMode;
    OOBIP_PODIfcMode_t   ePODIfcMode = OOBIP_PODIfcModeDisabled;

    /* configure OOB */
    switch (pObj->eOOBFECMode)
    {
    case tmTDA18280_OOBFECMode_SCTE_55_1:
        eFECMode = tmFrontEndFECModeAnnexA;
        ePODIfcMode = OOBIP_PODIfcModeDisabled;
        break;
    case tmTDA18280_OOBFECMode_SCTE_55_2:
        eFECMode = tmFrontEndFECModeAnnexB;
        ePODIfcMode = OOBIP_PODIfcModeDisabled;
        break;
    case tmTDA18280_OOBFECMode_POD:
        eFECMode = tmFrontEndFECModeAnnexA; /* select arbitrarily SCTE 55-1; TODO: preferably disable FEC if possible */
        ePODIfcMode = OOBIP_PODIfcModeEnabled;
        break;
    default:
        err = TDA18280_ERR_BAD_PARAMETER;
        break;
    }

    if (err == TM_OK)
    {
        err = tmbslOOBIp_SetFECMode(pObj->tUnitW, eFECMode);
    }

    if (err == TM_OK)
    {
        err = tmbslOOBIp_SetPODInterfaceMode(pObj->tUnitW, ePODIfcMode);
    }

    if (err == TM_OK)
    {
        err = tmbslOOBIp_SetIF(pObj->tUnitW, TDA18280_OOB_IF);
    }

    if (err == TM_OK)
    {
        err = tmbslOOBIp_SetSI(pObj->tUnitW, ptPathCfg->eSI);
    }

    if (err == TM_OK)
    {
        err = tmbslOOBIp_SetMod(pObj->tUnitW, ptPathCfg->eMod);
    }

    if (err == TM_OK)
    {
        err = tmbslOOBIp_SetSR(pObj->tUnitW, ptPathCfg->uSR);
    }

    /* start OOB lock */
    if (err == TM_OK)
    {
        err = tmbslOOBIp_StartLock(pObj->tUnitW);
    }

    return err;
}

/*============================================================================*/
/*                   I2C FUNCTIONS                                            */
/*============================================================================*/

static UInt16 
iTDA18280_CdpRegisterAddr( UInt16 ulAddr, UInt8 ulCdpIndex )
{
    UInt16 ulRegAddr = 0;

    ulRegAddr = ulAddr*2;
    if( ulCdpIndex < TDA18280_NB_CDP )
    {
        ulRegAddr += CDP_ADDR_PATH0;
        ulRegAddr += ulCdpIndex * CDP_ADDR_OFFSET;
    }

    return ulRegAddr;
}

/*============================================================================*/
/* iTDA18280_CdpWrite                                                         */
/*============================================================================*/
static tmErrorCode_t
iTDA18280_CdpWrite
(
    tmUnitSelect_t tUnit,
    UInt32 AddrSize,
    UInt8* pAddr,
    UInt32 WriteLen,
    UInt8* pData
)
{
    ptmTDA18280_Object_t   pObj = Null;
    tmErrorCode_t          err = TDA18280_ERR_NOT_INITIALIZED;
    UInt8                  uCdpIndex = 0;
    UInt16                 i2c_index = 0;

    /*  retrieve tUnit and CDP path index */
    uCdpIndex = (UInt8)ADDITIONNAL_INFO_GET(tUnit);
    tUnit &= ~ADDITIONNAL_INFO_MASK;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);
 
    tmDBGPRINTEx(DEBUGLVL_BLAB, "iTDA18280_CdpWrite(0x%08X) CDP%d", pObj->tUnit, uCdpIndex);

    if (err == TM_OK)
    {
        switch (AddrSize)
        {
        case 2:
            i2c_index = *((UInt16*)pAddr);
            break;
        default:
            err = TDA18280_ERR_BAD_PARAMETER;
            break;
        }

        if (err == TM_OK)
        {
            /* apply the offset on the register address depending of CDP path number */
            i2c_index = iTDA18280_CdpRegisterAddr( i2c_index, uCdpIndex);
            if( i2c_index == 0 )
            {
                err = TDA18280_ERR_NOT_INITIALIZED;
            }
        }

        /* depending on the endianness - swap or not */
#ifndef _TARGET_PLATFORM_ENDIANNESS_BIG
        {
        UInt16 ulTmp;

        ulTmp = (i2c_index & 0xFF) << 8;
        ulTmp += i2c_index >> 8;

        i2c_index = ulTmp;
        }
#endif

#ifndef _TARGET_PLATFORM_ENDIANNESS_BIG
        {
            UInt32 ulDataIndex = 0;

            for( ulDataIndex = 0; ulDataIndex < WriteLen/2; ulDataIndex ++)
            {
                UInt8 usTmp;
                usTmp = pData[ulDataIndex*2];
                pData[ulDataIndex*2] = pData[ulDataIndex*2 + 1];
                pData[ulDataIndex*2 + 1] = usTmp;
            }
        }
#endif

        if (err == TM_OK)
        {
            err = P_SIO_WRITE( pObj->tUnitW, 2, (UInt8*)&i2c_index, WriteLen, pData);
        }
    }

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18280_CdpWrite(0x%08X) CDP%d  failed", pObj->tUnit, uCdpIndex));

    return err;
}

/*============================================================================*/
/* iTDA18280_CdpRead                                                          */
/*============================================================================*/
static tmErrorCode_t
iTDA18280_CdpRead
(
    tmUnitSelect_t tUnit,
    UInt32 AddrSize,
    UInt8* pAddr,
    UInt32 ReadLen,
    UInt8* pData
)
{
    tmErrorCode_t          err = TDA18280_ERR_NOT_INITIALIZED;
    ptmTDA18280_Object_t   pObj = Null;
    UInt8                  uCdpIndex = 0;
    UInt16                 i2c_index = 0;

    /*  retrieve tUnit and CDP path index */
    uCdpIndex = (UInt8)ADDITIONNAL_INFO_GET(tUnit);
    tUnit &= ~ADDITIONNAL_INFO_MASK;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);

    tmDBGPRINTEx(DEBUGLVL_BLAB, "iTDA18280_CdpRead(0x%08X) CDP%d", pObj->tUnit, uCdpIndex);
 
    if (err == TM_OK)
    {
        switch (AddrSize)
        {
        case 2:
            i2c_index = *((UInt16*)pAddr);
            break;
        default:
            err = TDA18280_ERR_BAD_PARAMETER;
            break;
        }
        
        if (err == TM_OK)
        {
            /* apply the offset on the register address depending of CDP path number */
            i2c_index = iTDA18280_CdpRegisterAddr( i2c_index, uCdpIndex);
            if( i2c_index == 0 )
            {
                err = TDA18280_ERR_NOT_INITIALIZED;
            }
        }

        /* depending on the endianness - swap or not */
#ifndef _TARGET_PLATFORM_ENDIANNESS_BIG
        {
        UInt16 ulTmp;

        ulTmp = (i2c_index & 0xFF) << 8;
        ulTmp += i2c_index >> 8;

        i2c_index = ulTmp;
        }
#endif

        if (err == TM_OK)
        {
            err = P_SIO_READ( pObj->tUnitW, 2, (UInt8*)&i2c_index, ReadLen, pData);
        }

#ifndef _TARGET_PLATFORM_ENDIANNESS_BIG
        {
            UInt32 ulDataIndex = 0;
            UInt8  usTmp = 0;

            /* swap data */
            for( ulDataIndex = 0; ulDataIndex < ReadLen/2; ulDataIndex ++ )
            {
                usTmp = pData[ulDataIndex*2];

                pData[ulDataIndex*2] = pData[ulDataIndex*2 + 1];
                pData[ulDataIndex*2 + 1] = usTmp;
            }
        }
#endif
    }

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18280_CdpRead(0x%08X) CDP%d  failed", pObj->tUnit, uCdpIndex));

    return err;
}

/*============================================================================*/
/* iTDA18280_OobWrite                                                          */
/*============================================================================*/
static tmErrorCode_t
iTDA18280_OobWrite
(
    tmUnitSelect_t tUnit,
    UInt32 AddrSize,
    UInt8* pAddr,
    UInt32 WriteLen,
    UInt8* pData
)
{
    ptmTDA18280_Object_t   pObj = Null;
    tmErrorCode_t          err = TDA18280_ERR_NOT_INITIALIZED;
    UInt16                 i2c_index = 0;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);
 
    tmDBGPRINTEx(DEBUGLVL_BLAB, "iTDA18280_OobWrite(0x%08X)", pObj->tUnit);

    if (err == TM_OK)
    {
        switch (AddrSize)
        {
        case 2:
            i2c_index = *((UInt16*)pAddr);
            break;
        default:
            err = TDA18280_ERR_BAD_PARAMETER;
            break;
        }

        /* depending on the endianness - swap or not */
#ifndef _TARGET_PLATFORM_ENDIANNESS_BIG
        {
        UInt16 ulTmp;

        ulTmp = (i2c_index & 0xFF) << 8;
        ulTmp += i2c_index >> 8;

        i2c_index = ulTmp;
        }
#endif

#ifndef _TARGET_PLATFORM_ENDIANNESS_BIG
        {
            UInt32 ulDataIndex = 0;

            for( ulDataIndex = 0; ulDataIndex < WriteLen/2; ulDataIndex ++)
            {
                UInt8 usTmp;
                usTmp = pData[ulDataIndex*2];
                pData[ulDataIndex*2] = pData[ulDataIndex*2 + 1];
                pData[ulDataIndex*2 + 1] = usTmp;
            }
        }
#endif

        if (err == TM_OK)
        {
            err = P_SIO_WRITE( pObj->tUnitW, 2, (UInt8*)&i2c_index, WriteLen, pData);
        }
    }

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18280_OobWrite(0x%08X)  failed", pObj->tUnit));

    return err;
}

/*============================================================================*/
/* iTDA18280_OobRead                                                           */
/*============================================================================*/
static tmErrorCode_t
iTDA18280_OobRead
(
    tmUnitSelect_t tUnit,
    UInt32 AddrSize,
    UInt8* pAddr,
    UInt32 ReadLen,
    UInt8* pData
)
{
    tmErrorCode_t          err = TDA18280_ERR_NOT_INITIALIZED;
    ptmTDA18280_Object_t   pObj = Null;
    UInt16                 i2c_index = 0;

    /* Get a driver instance */
    err = iTDA18280_DeviceGetInstance(tUnit, &pObj);

    tmDBGPRINTEx(DEBUGLVL_BLAB, "iTDA18280_OobRead(0x%08X)", pObj->tUnit);
 
    if (err == TM_OK)
    {
        switch (AddrSize)
        {
        case 2:
            i2c_index = *((UInt16*)pAddr);
            break;
        default:
            err = TDA18280_ERR_BAD_PARAMETER;
            break;
        }

        /* depending on the endianness - swap or not */
#ifndef _TARGET_PLATFORM_ENDIANNESS_BIG
        {
        UInt16 ulTmp;

        ulTmp = (i2c_index & 0xFF) << 8;
        ulTmp += i2c_index >> 8;

        i2c_index = ulTmp;
        }
#endif

        if (err == TM_OK)
        {
            err = P_SIO_READ( pObj->tUnitW, 2, (UInt8*)&i2c_index, ReadLen, pData);
        }

#ifndef _TARGET_PLATFORM_ENDIANNESS_BIG
        {
            UInt32 ulDataIndex = 0;
            UInt8  usTmp = 0;

            /* swap data */
            for( ulDataIndex = 0; ulDataIndex < ReadLen/2; ulDataIndex ++ )
            {
                usTmp = pData[ulDataIndex*2];

                pData[ulDataIndex*2] = pData[ulDataIndex*2 + 1];
                pData[ulDataIndex*2 + 1] = usTmp;
            }
        }
#endif
    }

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18280_OobRead(0x%08X)  failed", pObj->tUnit));

    return err;
}

/*============================================================================*/
/* Open a mutex object                                                        */
/*============================================================================*/
static tmErrorCode_t iTDA18280_MutexInitDummy(ptmbslFrontEndMutexHandle *ppMutexHandle)
{
    tmErrorCode_t err = TM_OK;
    
    *ppMutexHandle = (ptmbslFrontEndMutexHandle)1;

    return err;
}

/*============================================================================*/
/* Close a mutex object                                                       */
/*============================================================================*/
static tmErrorCode_t iTDA18280_MutexDeInitDummy(ptmbslFrontEndMutexHandle pMutexHandle)
{
    tmErrorCode_t err = TM_OK;
    return err;
}

/*============================================================================*/
/* Acquire a mutex object                                                     */
/*============================================================================*/
static tmErrorCode_t iTDA18280_MutexAcquireDummy(ptmbslFrontEndMutexHandle pMutexHandle, UInt32 timeOut)
{
    tmErrorCode_t err = TM_OK;
    return err;
}

/*============================================================================*/
/* Release a mutex object                                                     */
/*============================================================================*/
static tmErrorCode_t iTDA18280_MutexReleaseDummy(ptmbslFrontEndMutexHandle pMutexHandle)
{
    tmErrorCode_t err = TM_OK;
    return err;
}

