/*
Copyright (C) 2010 NXP B.V., All Rights Reserved.
This source code and any compilation or derivative thereof is the proprietary
information of NXP B.V. and is confidential in nature. Under no circumstances
is this software to be  exposed to or placed under an Open Source License of
any type without the expressed written permission of NXP B.V.
*
* \file          tmbslTDA18250A.c
*
* \date          %modify_time%
*
* \author        Alexandre TANT
*
* \brief         Describe briefly the purpose of this file.
*
* REFERENCE DOCUMENTS :
*                TDA18250A_Driver_User_Guide.pdf
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
#include "tmUnitParams.h"

/*============================================================================*/
/* Project include files:                                                     */
/*============================================================================*/
#include "tmbslTDA18250A.h"

#include "tmbslTDA18250A_Local.h"
#ifdef TDA18250A_DVBC_SUPPORTED
#include "tmbslTDA18250A_Config_DVBC.h"
#endif
#ifdef TDA18250A_DVBT_SUPPORTED
#include "tmbslTDA18250A_Config_DVBT.h"
#endif
#ifdef TDA18250A_DTMB_SUPPORTED
#include "tmbslTDA18250A_Config_DTMB.h"
#endif
#ifdef TDA18250A_ISDBT_SUPPORTED
#include "tmbslTDA18250A_Config_ISDBT.h"
#endif
#ifdef TDA18250A_ANALOG_SUPPORTED
#include "tmbslTDA18250A_Config_ANALOG.h"
#endif
#ifdef TDA18250A_ATSC_SUPPORTED
#include "tmbslTDA18250A_Config_ATSC.h"
#endif
#include "tmbslTDA18250A_Config_Common.h"

extern TDA18250A_RDivNDivTableDef RDivNDivTable[5];

/*============================================================================*/
/* Static internal functions:                                                 */
/*============================================================================*/
static tmErrorCode_t
iTDA18250A_OverrideICP(pTDA18250AObject_t pObj, UInt32 freq);

static tmErrorCode_t
iTDA18250A_OverrideWireless(pTDA18250AObject_t pObj);

static tmErrorCode_t
iTDA18250A_FindPostDivAndPrescalerWithBetterMargin(UInt32 LO, UInt8* PostDiv, UInt8* Prescaler);

static tmErrorCode_t
iTDA18250A_SetPLL(pTDA18250AObject_t pObj);

static tmErrorCode_t
iTDA18250A_SetRF_Freq(pTDA18250AObject_t pObj, UInt32 uRF);

static tmErrorCode_t
iTDA18250A_AGC1_Update( pTDA18250AObject_t pObj);

static tmErrorCode_t
iTDA18250A_GetRefMaxGainNB(pTDA18250AObject_t pObj, Int32* pMaxGain);

static tmErrorCode_t
iTDA18250A_LvlAgc1NBTunerContribution(pTDA18250AObject_t pObj, Int32* pDeltaGain);

static tmErrorCode_t
iTDA18250A_LvlAgc2NBContribution(pTDA18250AObject_t pObj, Int32* pDeltaGain);

static tmErrorCode_t
iTDA18250A_LvlAgc3NBContribution(pTDA18250AObject_t pObj, Int32* pDeltaGain);

static tmErrorCode_t
iTDA18250A_LvlS2DNBContribution(pTDA18250AObject_t pObj, Int32* pDeltaGain);

static tmErrorCode_t
iTDA18250A_GetPowerLevel(pTDA18250AObject_t pObj, UInt8 uRSSIValue, Int32* pPowerLevel);


/*============================================================================*/
/* Static variables:                                                          */
/*============================================================================*/
/* Driver instance */
TDA18250AObject_t gTDA18250AInstance[TDA18250A_MAX_UNITS] = 
{
    {
        (tmUnitSelect_t)(-1),               /* Unit not set */
            (tmUnitSelect_t)(-1),               /* UnitW not set */
            Null,                               /* pMutex */
            False,                              /* init (instance initialization default) */
        {                                   /* sIo */
            Null,
                Null
        },
        {                                   /* sTime */
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
        Null,                               /* getMasterContribution */
        TDA18250A_VersionES1,               /* eChipVersion */
        True,                               /* bMasterNotSlave */
        (tmUnitSelect_t)-1,                 /* tOtherUnit */
        {
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 /* sSmoothCurrentStateSave */
        },
        TDA18250A_Scanning_Mode_Enabled,
        TDA18250A_CONFIG_0          /* Instance Customizable Settings */
    },
    {
        (tmUnitSelect_t)(-1),               /* Unit not set */
            (tmUnitSelect_t)(-1),               /* UnitW not set */
            Null,                               /* pMutex */
            False,                              /* init (instance initialization default) */
        {                                   /* sIo */
            Null,
                Null
        },
        {                                   /* sTime */
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
        Null,                               /* getMasterContribution */
        TDA18250A_VersionES1,               /* eChipVersion */
        False,                              /* bMasterNotSlave */
        (tmUnitSelect_t)-1,                 /* tOtherUnit */
        {
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 /* sSmoothCurrentStateSave */
        },
        TDA18250A_Scanning_Mode_Enabled,
        TDA18250A_CONFIG_1          /* Instance Customizable Settings */
    },
    {
        (tmUnitSelect_t)(-1),               /* Unit not set */
            (tmUnitSelect_t)(-1),               /* UnitW not set */
            Null,                               /* pMutex */
            False,                              /* init (instance initialization default) */
        {                                   /* sIo */
            Null,
                Null
        },
        {                                   /* sTime */
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
        Null,                               /* getMasterContribution */
        TDA18250A_VersionES1,               /* eChipVersion */
        False,                               /* bMasterNotSlave */
        (tmUnitSelect_t)-1,                 /* tOtherUnit */
        {
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 /* sSmoothCurrentStateSave */
        },
        TDA18250A_Scanning_Mode_Enabled,
        TDA18250A_CONFIG_2          /* Instance Customizable Settings */
    }
};

/* Table that maps LO vs Prescaler & PostDiv values for better margin algo */
static TDA18250A_PostDivPrescalerTableDef PostDivPrescalerTableBetterMargin[35] =
{
    /* PostDiv 1 */
    {1080000, 882000, 7, 1},
    {882001, 771750, 8, 1},
    {784000, 686000, 9, 1},
    {705600, 617400, 10, 1},
    {641454, 561272, 11, 1},
    {587999, 514499, 12, 1},
    {542768, 474922, 13, 1},
    /* PostDiv 2 */
    {503998, 440998, 7, 2},
    {440999, 385873, 8, 2},
    {391998, 342998, 9, 2},
    {352798, 308698, 10, 2},
    {320725, 280634, 11, 2},
    {293997, 257247, 12, 2},
    {271381, 237458, 13, 2},
    /* PostDiv 4 */   
    {251996, 220496, 7, 4},
    {220497, 192934, 8, 4},
    {195996, 171496, 9, 4},
    {176396, 154346, 10, 4},
    {160360, 140315, 11, 4},
    {146996, 128621, 12, 4},
    {135688, 118727, 13, 4},
    /* PostDiv 8 */
    {125996, 110246, 7, 8},
    {110247, 96465, 8, 8},
    {97996, 85746, 9, 8},
    {88196, 77171, 10, 8},
    {80178, 70155, 11, 8},
    {73496, 64309, 12, 8},
    {67842, 59361, 13, 8},
    /* PostDiv 16 */
    {62996, 55121, 7, 16},
    {55122, 48230, 8, 16},
    {48996, 42871, 9, 16},
    {44096, 38584, 10, 16},
    {40087, 35076, 11, 16},
    {36746, 32152, 12, 16},
    {33919, 29679, 13, 16}
};

#define BITS32_MAX 0xFFFFFFFF

/* used for RSSI NB */
typedef Int32 AGC1NBTable_t[TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_AGC1_MAX][TDA18250A_Coeff_Y_Max];
static Int32 Lvl_MAX_GAIN_NB[TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_REF_GAIN_MAX][TDA18250A_Coeff_Y_Max] = TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_REF_GAIN_MAX_NB;
/*
static Int32 Lvl_AGC1NB_TUNER_LT_OFF_ST_OFF[TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_AGC1_MAX][TDA18250A_Coeff_Y_Max] = TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_AGC1NB_TUNER_LT_OFF_ST_OFF;
static Int32 Lvl_AGC1NB_TUNER_LT_OFF_ST_ON[TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_AGC1_MAX][TDA18250A_Coeff_Y_Max] = TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_AGC1NB_TUNER_LT_OFF_ST_ON;
static Int32 Lvl_AGC1NB_TUNER_LT_ON_ST_ON[TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_AGC1_MAX][TDA18250A_Coeff_Y_Max] = TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_AGC1NB_TUNER_LT_ON_ST_ON;

*/
static AGC1NBTable_t Lvl_AGC1NB_TUNER_LT_OFF_ST_OFF = TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_AGC1NB_TUNER_LT_OFF_ST_OFF;
static AGC1NBTable_t Lvl_AGC1NB_TUNER_LT_OFF_ST_ON = TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_AGC1NB_TUNER_LT_OFF_ST_ON;
static AGC1NBTable_t Lvl_AGC1NB_TUNER_LT_ON_ST_ON = TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_AGC1NB_TUNER_LT_ON_ST_ON;
/*
static Int32 Lvl_AGC1NB_MASTER_LT_OFF_ST_ON[TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_AGC1_MAX][TDA18250A_Coeff_Y_Max] = TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_AGC1NB_MASTER_LT_OFF_ST_ON;
static Int32 Lvl_AGC1NB_MASTER_LT_ON_ST_ON[TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_AGC1_MAX][TDA18250A_Coeff_Y_Max] = TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_AGC1NB_MASTER_LT_ON_ST_ON;
*/
static AGC1NBTable_t Lvl_AGC1NB_MASTER_LT_OFF_ST_ON = TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_AGC1NB_MASTER_LT_OFF_ST_ON;
static AGC1NBTable_t Lvl_AGC1NB_MASTER_LT_ON_ST_ON = TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_AGC1NB_MASTER_LT_ON_ST_ON;

static Int32 Lvl_AGC2NB[TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_AGC2_MAX][TDA18250A_Coeff_Y_Max] = TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_AGC2NB;
static Int32 Lvl_AGC3NB[TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_AGC3_MAX][TDA18250A_Coeff_Y_Max] = TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_AGC3NB;
static Int32 Lvl_S2DNB[TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_S2D_MAX][TDA18250A_Coeff_Y_Max] = TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_S2DNB;

/*============================================================================*/
/* FUNCTION:    tmbslTDA18250A_Open:                                          */
/*                                                                            */
/* DESCRIPTION: Opens driver setup environment.                               */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslTDA18250A_Open(
                    tmUnitSelect_t              tUnit,      /* I: FrontEnd unit number */
                    tmbslFrontEndDependency_t*  psSrvFunc   /* I: setup parameters */
                    )
{
    pTDA18250AObject_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Test parameter(s) */
    if(psSrvFunc == Null)
    {
        err = TDA18250A_ERR_BAD_PARAMETER;
    }

    if(err == TM_OK)
    {
        /* Get a driver instance */
        err = iTDA18250A_GetInstance(tUnit, &pObj);
    }

    /* Check driver instance state */
    if(err == TM_OK || err == TDA18250A_ERR_ERR_NO_INSTANCES)
    {
        if(P_OBJ_VALID && pObj->init == True)
        {
            err = TDA18250A_ERR_ALREADY_SETUP;
        }
        else 
        {
            if(P_OBJ_VALID == False)
            {
                /* Try to allocate an instance of the driver */
                err = iTDA18250A_AllocInstance(tUnit, &pObj);
                if(err != TM_OK || pObj == Null)
                {
                    err = TDA18250A_ERR_ERR_NO_INSTANCES;
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

                tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslTDA18250A_Open(0x%08X)", tUnit);

                if(err == TM_OK)
                {
                    if ((psSrvFunc->dwAdditionalDataSize != 0) && (psSrvFunc->pAdditionalData != Null))
                    {
                        TDA18250A_AdditionnalData_t *psAdditionnalData = (TDA18250A_AdditionnalData_t*)psSrvFunc->pAdditionalData;
                        pObj->tOtherUnit = psAdditionnalData->tOtherUnit;
                    }
                    pObj->init = True;
                }
            }
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslTDA18250A_Close:                                          */
/*                                                                            */
/* DESCRIPTION: Closes driver setup environment.                              */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslTDA18250A_Close(
                     tmUnitSelect_t  tUnit   /* I: FrontEnd unit number */
                     )
{
    pTDA18250AObject_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = iTDA18250A_GetInstance(tUnit, &pObj);

    if(err == TM_OK)
    {
#ifdef _TVFE_IMPLEMENT_MUTEX
        /* Try to acquire driver mutex */
        err = iTDA18250A_MutexAcquire(pObj, TDA18250A_MUTEX_TIMEOUT);

        if(err == TM_OK)
        {
#endif
            tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslTDA18250A_Close(0x%08X)", tUnit);

#ifdef _TVFE_IMPLEMENT_MUTEX
            P_SMUTEX_ACQUIRE = Null;

            /* Release driver mutex */
            (void)iTDA18250A_MutexRelease(pObj);

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

        err = iTDA18250A_DeAllocInstance(pObj);
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslTDA18250A_SetPowerState                                   */
/*                                                                            */
/* DESCRIPTION: Sets the power state.                                         */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslTDA18250A_SetPowerState(
                             tmUnitSelect_t  tUnit,      /* I: Unit number */
                             tmPowerState_t  powerState  /* I: Power state */
                             )
{
    pTDA18250AObject_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = iTDA18250A_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18250A)

        tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslTDA18250A_SetPowerState(0x%08X)", tUnit);

    if(powerState>=tmPowerMax)
    {
        err = TDA18250A_ERR_BAD_PARAMETER;
    }

    if(err == TM_OK)
    {
        if(pObj->mapLLPowerState[powerState] != pObj->curLLPowerState)
        {
            err = iTDA18250A_SetLLPowerState(pObj, pObj->mapLLPowerState[powerState]);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_SetLLPowerState(0x%08X, %d) failed.", tUnit, (int)pObj->mapLLPowerState[powerState]));

            if(err == TM_OK)
            {
                /* Store power state in driver instance */
                pObj->curPowerState = powerState;
            }
        }
    }

    _MUTEX_RELEASE(TDA18250A)

        return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslTDA18250A_SetStandardMode                                 */
/*                                                                            */
/* DESCRIPTION: Sets the standard mode.                                       */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslTDA18250A_SetStandardMode(
                               tmUnitSelect_t          tUnit,          /* I: Unit number */
                               TDA18250AStandardMode_t  StandardMode    /* I: Standard mode of this device */
                               )
{
    pTDA18250AObject_t           pObj = Null;
    tmErrorCode_t               err = TM_OK;
    pTDA18250AStdCoefficients    prevPStandard = Null;

    /* Get a driver instance */
    err = iTDA18250A_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18250A)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslTDA18250A_SetStandardMode(0x%08X)", tUnit);

    /* Check if Hw is ready to operate */
    err = iTDA18250A_CheckHwState(pObj, TDA18250A_HwStateCaller_SetStd);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_CheckHwState(0x%08X) failed.", pObj->tUnitW));

    /* item#67 : Pushing the "Program" button in the System window must not reset the "AGC Fast mode"and "AGC2 Gain Control En" */
    /* to allow this, if previous standard is the same do nothing */
    if( (err == TM_OK) && (pObj->StandardMode != StandardMode) )
    {
        /* Store standard mode */
        pObj->StandardMode = StandardMode;

        /* Reset standard map pointer */
        prevPStandard = pObj->pStandard;
        pObj->pStandard = Null;

        if(pObj->StandardMode>TDA18250A_StandardMode_Unknown && pObj->StandardMode<TDA18250A_StandardMode_Max)
        {
            /* Update standard map pointer */
            pObj->pStandard = &pObj->Std_Array[pObj->StandardMode - 1];

            /****************************************************************/
            /* IF SELECTIVITY Settings                                      */
            /****************************************************************/

            /* LP_Fc */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_IF_Byte_1__LP_Fc, pObj->pStandard->LPF, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));

            if(err == TM_OK)
            {
                /* IF_LPF_Offset */
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_IF_Byte_1__IF_LPF_Offset,(UInt8)(pObj->pStandard->LPF_Offset >>4), Bus_None);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }
            if(err == TM_OK)
            {
                /* IF_LPF_sign */
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_IF_Byte_1__IF_LPF_sign, (UInt8)(pObj->pStandard->LPF_Offset & 0x0f), Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }
            if(err == TM_OK)
            {
                /* DC_Notch */
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_IR_Mixer_byte_1__DC_Notch, pObj->pStandard->DC_Notch_IF_PPF, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }
            if(err == TM_OK)
            {
                /* IF_HPF_Fc */
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_IF_Byte_2__IF_HP_Fc, (UInt8)(pObj->pStandard->HPF >> 4), Bus_None);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }            
            if(err == TM_OK)
            {
                /* IF_HPF_Offset */
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_IF_Byte_2__IF_HPF_Offset, (UInt8)(pObj->pStandard->HPF_Offset >>4), Bus_None);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }
            if(err == TM_OK)
            {
                /* IF_HPF_sign */
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_IF_Byte_2__IF_HPF_sign, (UInt8)(pObj->pStandard->HPF_Offset & 0x0f), Bus_None);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }
            
            if(err == TM_OK)
            {
                /* IF_HPF_Enable */
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_IF_Byte_2__IF_HP_Enable,(UInt8)(pObj->pStandard->HPF & 0x0f), Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }
            if(err == TM_OK)
            {
                /* IF_Notch_Enable */
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_IF_Byte_3__IF_Notch_Enable, pObj->pStandard->IF_Notch_Enable, Bus_None);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }
            if(err == TM_OK)
            {
                /* IF_Notch_Freq */
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_IF_Byte_3__IF_Notch_Freq, pObj->pStandard->IF_Notch_Freq, Bus_None);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }
            if(err == TM_OK)
            {
                /* IF_Notch_Offset */
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_IF_Byte_3__IF_Notch_Offset, (UInt8)(pObj->pStandard->IF_Notch_Offset >>4), Bus_None);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));

                /* IF_Notch_sign */
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_IF_Byte_3__IF_Notch_sign, (UInt8)(pObj->pStandard->IF_Notch_Offset & 0x0f), Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }
			if(err == TM_OK)
            {
                /* IF Output Level */
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_IF_AGC_byte__IF_level, pObj->pStandard->IF_Output_Level, Bus_None);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }
            if(err == TM_OK)
            {
                /* IFnotchToRSSI */
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_IF_AGC_byte__IFnotchToRSSI, pObj->pStandard->IFnotchToRSSI, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }

            /****************************************************************/
            /* AGC Settings                                                 */
            /****************************************************************/
            if (err == TM_OK)
            {
                if ((pObj->eChipVersion == TDA18250A_VersionES1) || (pObj->eChipVersion == TDA18250A_VersionES3))
                {
                    /* Set Force_AGC1_gain */
                    if ((pObj->pStandard->AGC1_GAIN == TDA18250AAGC1_GAIN_Free) || (pObj->pStandard->AGC1_GAIN == TDA18250AAGC1_GAIN_Free_Frozen))
                    {
                        /* Desactivate AGC1 Fast mode */
                        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_2__AGC1_Up_step, 0x01, Bus_None);
                        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));

                        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_2__Fast_Mode_AGC, 0x00, Bus_NoRead);
                        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));

                        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_3__Force_AGC1_gain, 0x00, Bus_NoRead);
                        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
                    }
                    else
                    {
                        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_3__Force_AGC1_gain, 0x01, Bus_None);
                        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));

                        if (err == TM_OK)
                        {
                            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_3__AGC1_Gain, (UInt8)pObj->pStandard->AGC1_GAIN-1, Bus_NoRead);
                            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
                        }
                    }
                }
                else
                {
                    /* Set Force_AGC1_gain */
                    if (pObj->pStandard->AGC1_GAIN == TDA18250AAGC1_GAIN_Free)
                    {
                        /* Desactivate AGC1 Fast mode */
                        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_2__AGC1_Up_step, 0x01, Bus_None);
                        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));

                        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_2__Fast_Mode_AGC, 0x00, Bus_NoRead);
                        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));

                        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_3__Force_AGC1_gain, 0x00, Bus_NoRead);
                        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
                    }
                    else if (pObj->pStandard->AGC1_GAIN != TDA18250AAGC1_GAIN_Free_Frozen) /* Frozen to a specified value */
                    {
                        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_3__Force_AGC1_gain, 0x01, Bus_None);
                        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));

                        if (err == TM_OK)
                        {
                            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_3__AGC1_Gain, (UInt8)pObj->pStandard->AGC1_GAIN-1, Bus_NoRead);
                            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
                        }
                    }
                    else /* Free_Frozen algo will be used */
                    {                
                        /* Activate AGC1 Fast mode */
                        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_2__AGC1_Up_step, 0x02, Bus_None);
                        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));

                        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_2__Fast_Mode_AGC, 0x01, Bus_NoRead);
                        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
                    }
                }
            }

            if(err == TM_OK)
            {
                /* Set AGC1_Top_Strategy */
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_1__AGC1_Top_Strategy, pObj->pStandard->AGC1_TOP_STRATEGY, Bus_None);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }
            if(err == TM_OK)
            {
                /* Set AGC1_Det_Speed */
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_1__AGC1_Det_Speed, pObj->pStandard->AGC1_DET_SPEED, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }
            if(err == TM_OK)
            {
                /* Set GC1_smooth_t_cst */
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_4__AGC1_smooth_t_cst, pObj->pStandard->AGC1_Smooth_T_Cst, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }
            if(err == TM_OK)
            {
                /* LNA Zin -> Standard */
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_2__LNA_Zin, pObj->pStandard->LNA_Zin, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }
            if (err == TM_OK)
            {
                /* AGC2 Gain Control En */
                if (pObj->eChipVersion == TDA18250A_VersionES1)
                {
                    err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_3__AGC2_Gain_Control_En, (UInt8)pObj->pStandard->AGC2_GAIN_CONTROL_EN_ES1, Bus_NoRead);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
                }
                else if (pObj->eChipVersion == TDA18250A_VersionES2)
                {
                    err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_3__AGC2_Gain_Control_En, (UInt8)pObj->pStandard->AGC2_GAIN_CONTROL_EN_ES2, Bus_NoRead);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
                }
                else
                {
                    err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_3__AGC2_Gain_Control_En, (UInt8)pObj->pStandard->AGC2_GAIN_CONTROL_EN_ES3, Bus_NoRead);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
                }
            }
            if(err == TM_OK)
            {
                /* AGC2_Det_Speed */
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_3__AGC2_Det_Speed, pObj->pStandard->AGC2_Det_Speed, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }
            if ((pObj->eChipVersion == TDA18250A_VersionES2) || (pObj->eChipVersion == TDA18250A_VersionES3))
            { 
                if(err == TM_OK)
                { /* AGC2_Gain_Control_Speed False 1ms ; True 0.5ms    */
                    err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_3__AGC2_Gain_Control_Speed, (pObj->pStandard->AGC2_Gain_Control_Speed ? 0x01:0x00), Bus_NoRead);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
                }
                if(err == TM_OK)
                { /* AGC1_Do_step   */
                    err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_3__AGC1_Do_step, (UInt8)pObj->pStandard->AGC1_Do_Step, Bus_NoRead);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
                }
                if(err == TM_OK)
                { /* AGC2_Do_step   */
                    err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_1__AGC2_Do_step, (UInt8)pObj->pStandard->AGC2_Do_Step, Bus_NoRead);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
                }
                if(err == TM_OK)
                { /* AGC2_Up_step   */
                    err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_1__AGC2_Up_step, (UInt8)pObj->pStandard->AGC2_Up_Step, Bus_NoRead);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
                }
                if(err == TM_OK)
                { /* AGC2_Up_Udld_Step  */
                    err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_UDLD_byte_1__AGC2_Up_udld_step, (UInt8)pObj->pStandard->AGC2_Up_Udld_Step, Bus_NoRead);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
                }
                if(err == TM_OK)
                { /* AGC2_Fast_Auto_Delta */
                    err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_UDLD_byte_2__AGC2_FAST_AUTO_DELTA, (UInt8)pObj->pStandard->AGC2_Fast_Auto_Delta, Bus_NoRead);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
                }
            }
            if(err == TM_OK)
            {
                /* Det12_Cint */
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGCK_byte_1__DET12_Cint, pObj->pStandard->DET12_CINT, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }
            
            /****************************************************************/
            /* GSK Settings                                                 */
            /****************************************************************/

            if(err == TM_OK)
            {
                /* AGCK Time Constant */
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGCK_byte_1__AGCK_Mode, pObj->pStandard->AGCK_Time_Constant, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }
            if(err == TM_OK)
            {
                /* RSSI HP FC */
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC3_byte_3__RSSI_HP_Fc, pObj->pStandard->RSSI_HP_FC, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }

            /****************************************************************/
            /* H3H5 Settings                                                */
            /****************************************************************/

            if(err == TM_OK)
            {
                /* VHF_III_Mode */
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_W_Filter_byte__VHF_III_mode, pObj->pStandard->VHF_III_Mode, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }

            /****************************************************************/
            /* RSSI Settings                                                */
            /****************************************************************/

            if(err == TM_OK)
            {
                /* RSSI_Cap_Val */
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_RSSI_byte_2__RSSI_Cap_Val, pObj->pStandard->RSSI_Cap_Val, Bus_None);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }
            if(err == TM_OK)
            {
                /* RSSI_Ck_Speed */
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_RSSI_byte_2__RSSI_Ck_Speed, pObj->pStandard->RSSI_Ck_Speed, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }

            /****************************************************************/
            /* IF Settings                                                  */
            /****************************************************************/

            if(err == TM_OK)
            {
                /* Set IF */
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_IF_Frequency_byte__IF_Freq, (UInt8)((pObj->pStandard->IF - pObj->pStandard->CF_Offset)/50000), Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }
            if(err == TM_OK)
            {
                err = iTDA18250A_PowerSavingMode(pObj/*, pObj->pStandard->PowerSavingMode*/);
            }
        }

#ifdef TDA18250A_ANALOG_SUPPORTED
        if ( (pObj->StandardMode == TDA18250A_FM_Radio) ||
            (pObj->StandardMode == TDA18250A_ANLG_MN) ||
            (pObj->StandardMode == TDA18250A_ANLG_B) ||
            (pObj->StandardMode == TDA18250A_ANLG_GH) ||
            (pObj->StandardMode == TDA18250A_ANLG_I) ||
            (pObj->StandardMode == TDA18250A_ANLG_DK) ||
            (pObj->StandardMode == TDA18250A_ANLG_L) ||
            (pObj->StandardMode == TDA18250A_ANLG_LL) ||
            (pObj->StandardMode == TDA18250A_Scanning) )
        {
            /* RF_Reg */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Regulators_byte__RF_Reg, 0x02, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        else
        {
            /* RF_Reg */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Regulators_byte__RF_Reg, 0x01, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
#endif

        /* Update driver state machine */
        pObj->eHwState = TDA18250A_HwState_SetStdDone;
    }

    _MUTEX_RELEASE(TDA18250A)

        return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslTDA18250A_SetRF:                                          */
/*                                                                            */
/* DESCRIPTION: Tunes to a RF.                                                */
/*                                                                            */
/* RETURN:      TM_OK                                                         */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslTDA18250A_SetRF(
                     tmUnitSelect_t  tUnit,  /* I: Unit number */
                     UInt32          uRF     /* I: RF frequency in hertz */
                     )
{
    pTDA18250AObject_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = iTDA18250A_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18250A)

        tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslTDA18250A_SetRF(0x%08X)", tUnit);

    /* Test parameter(s) */
    if(   pObj->StandardMode<=TDA18250A_StandardMode_Unknown
        || pObj->StandardMode>=TDA18250A_StandardMode_Max
        || pObj->pStandard == Null)
    {
        err = TDA18250A_ERR_STD_NOT_SET;
    }

    if(err == TM_OK)
    {
    	/*Program AGC TOPs only when uRF changed*/
    	if(pObj->uRF != uRF)
        	err = iTDA18250A_SetFreqDependantStandard(pObj, uRF);
    }

    if (err == TM_OK)
    {
        /* Check if Hw is ready to operate */
        err = iTDA18250A_CheckHwState(pObj, TDA18250A_HwStateCaller_SetRF);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_CheckHwState(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        pObj->uRF = uRF;
        pObj->uProgRF = pObj->uRF + pObj->pStandard->CF_Offset;

        err = iTDA18250A_SetRF(pObj);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_SetRF(0x%08X) failed.", tUnit));

        if(err == TM_OK)
        {
            /* Update driver state machine */
            pObj->eHwState = TDA18250A_HwState_SetRFDone;
        }
    }

    _MUTEX_RELEASE(TDA18250A)

        return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslTDA18250A_HwInit:                                         */
/*                                                                            */
/* DESCRIPTION: Initializes TDA18250A Hardware.                                */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslTDA18250A_HwInit(
                      tmUnitSelect_t  tUnit   /* I: Unit number */
                      )
{
    pTDA18250AObject_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;
    UInt8 uVal = 0;
    UInt8 uVal2 = 0;
    UInt16 uIdentity=0;
    UInt32 counter = 10; /* Wait I2C ready */

    /* Get a driver instance */
    err = iTDA18250A_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18250A)
#if 1
        tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslTDA18250A_HwInit(0x%08X)", tUnit);

    /* Reset standard mode & Hw State */
    pObj->StandardMode = TDA18250A_StandardMode_Max;
    pObj->eHwState = TDA18250A_HwState_InitNotDone;

    while(counter > 0)
    {
        /* Read all bytes */
        err = iTDA18250A_ReadRegMap(pObj, 0x00, TDA18250A_REG_MAP_NB_BYTES);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_ReadRegMap(0x%08X) failed.", tUnit));

        if(err==TM_OK)
        {
            /* IC I2C responds */
            break;
        }

        if(counter)
        {
            /* Decrease the counter */
            counter--;

            /* Wait for a step of 1ms */
            err = iTDA18250A_Wait(pObj, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Wait(0x%08X) failed.", pObj->tUnitW));
        }
    }

    if(err == TM_OK)
    {
        err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_ID_byte_1__Ident_1, &uVal, Bus_None);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Read(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        uIdentity = (uVal << 8);
        err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_ID_byte_2__Ident_2, &uVal, Bus_None);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Read(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        uIdentity = uIdentity | uVal;
        err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_ID_byte_1__Master_Not_Slave, &uVal, Bus_None);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Read(0x%08X) failed.", pObj->tUnitW));
        pObj->bMasterNotSlave = ( uVal == 1 ) ? True : False;
    }
    if(err == TM_OK)
    {
        err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_ID_byte_3__Major_rev, &uVal, Bus_None);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Read(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_ID_byte_3__Minor_rev, &uVal2, Bus_None);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Read(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        if (uVal==1 &&  uVal2==0)
            pObj->eChipVersion = TDA18250A_VersionES1;
        else if (uVal==2 &&  uVal2==0)
            pObj->eChipVersion = TDA18250A_VersionES2;
        else if (uVal==2 &&  uVal2==1)
            pObj->eChipVersion = TDA18250A_VersionES3;
    }

    if(err == TM_OK)
    {
        /* Set power state on */
        err = iTDA18250A_SetLLPowerState(pObj, TDA18250A_PowerNormalMode);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_SetLLPowerState(0x%08X, PowerNormalMode) failed.", tUnit));
    }

    if (err == TM_OK /*&& uVal != 1*/)
    { 
		/* at least Software init request with HW not in POR -> so Rewrite needed POR values */

        /* S2B Dcboost */
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Power_Down_byte_1__S2B_Dcboost, 0x01, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));		
        
        if(err == TM_OK)
        {
            /* LNA Vcollec */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_lna_polar_casc_byte_1__lna_Vcollec, 0x06, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* LNA Vcasc */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_lna_polar_casc_byte_1__lna_Vcasc, 0x04, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* Mixer GateDC */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_IR_Mixer_byte_1__Mixer_GateDC, 0x00, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* AGC1_Boost */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_2__AGC1_boost, 0x01, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* AGC2_Dcboost */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Power_Down_byte_1__AGC2_Dcboost, 0x00, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* PSM_H3H5 */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_2__PSM_H3H5, 0x00, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* PSM_RFpoly */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_1__PSM_RFpoly, 0x01, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* PSM_LO_Driver */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_1__PSM_Lodriver, 0x01, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* PSM_IFpoly */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_1__PSM_Ifpoly, 0x01, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* PSM_lpf */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_3__PSM_lpf, 0x01, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* PSM_VCO */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_2__PSM_VCO, 0x02, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* PSM_DCC */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_3__PSM_DCC, 0x00, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* LO_Calc_Disable */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Sigma_delta_byte_5__LO_Calc_Disable, 0x00, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* N_K_correct_manual */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Sigma_delta_byte_5__N_K_correct_manual, 0x00, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* ICP_Offset */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Charge_pump_byte__ICP_offset, 0x01, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* Dig_Clock */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Reference_Byte__Dig_Clock, 0x03, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* xt_cal_mode */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_XtalOsc_byte_1__xt_cal_mode, 0x00, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* xt_cal_timer */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_XtalOsc_byte_1__xt_cal_timer, 0x03, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* AGC1_DO_step */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_3__AGC1_Do_step, 0x02, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* AGC2_DO_step */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_1__AGC2_Do_step, 0x02, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* AGC3_DO_step */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC3_byte_3__AGC3_Do_step, 0x02, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* AGC1_rst_do_enable */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_4__AGC1_rst_do_enable, 0x00, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* AGC1_rst_up_enable */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_4__AGC1_rst_up_enable, 0x00, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* AGC1_Up_Step_assym_En */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_2__AGC1_Up_Step_asym_En, 0x01, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* AGC1_Do_Step_assym_En */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_2__AGC1_Do_Step_asym_En, 0x01, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* AGC2_Up_Step_assym_En */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_2__AGC2_Up_Step_asym_En, 0x01, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* AGC2_Do_Step_assym_En */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_2__AGC2_Do_Step_asym_En, 0x01, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* AGC3_Up_Step_assym_En */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC3_byte_2__AGC3_Up_Step_asym_En, 0x01, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* AGC3_Do_Step_assym_En */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC3_byte_2__AGC3_Do_Step_asym_En, 0x01, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* AGC2_Gain_Control_En */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_3__AGC2_Gain_Control_En, 0x00, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* AGC2_Gain_Control_Speed */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_3__AGC2_Gain_Control_Speed, 0x01, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* Det3_FastAtt */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC3_byte_3__DET3_FastAtt, 0x00, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* Det3_LowBW */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC3_byte_3__DET3_LowBW, 0x00, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* AGCK_step */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGCK_byte_1__AGCK_Step, 0x00, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* Fast_Mode_AGC */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_2__Fast_Mode_AGC, 0x00, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* AGC2_cft_shift */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_1__AGC2_cfi_shift, 0x01, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* IRQ_Mode */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_IRQ_Ctrl_byte__IRQ_Mode, 0x00, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* DDS_polarity */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_IRQ_Ctrl_byte__DDS_Polarity, 0x00, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* Dig_Reg */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Regulators_byte__Dig_Reg, 0x01, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* IF_Reg */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Regulators_byte__IF_Reg, 0x01, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* RF_Reg */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Regulators_byte__RF_Reg, 0x01, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* VCO_Reg */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Regulators_byte__VCO_Reg, 0x01, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* IR_Step_Delta  */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_IR_Cal_byte_3__IR_Step_Delta, 0x08, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* IR_Cal_loop  */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_IR_Cal_byte_3__IR_Cal_loop, 0x05, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* IR_R2x_only  */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_IR_Cal_byte_4__IR_R2x_only, 0x00, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* RSSI_Cap_Reset_En */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_RSSI_byte_2__RSSI_Cap_Reset_En, 0x01, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* RSSI_Ck_speed */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_RSSI_byte_2__RSSI_Ck_Speed, 0x01, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* RSSI_Gv */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_RSSI_byte_2__RSSI_Gv, 0x00, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* RSSI_Cap_Val */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_RSSI_byte_2__RSSI_Cap_Val, 0x01, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* RSSI_Dicho_not */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_RSSI_byte_2__RSSI_Dicho_not, 0x00, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* W_Filter_Enable */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_W_Filter_byte__W_Filter_Enable, 0x01, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));		
        }
        if(err == TM_OK)
        {
            /* W_Filter_Offset */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_W_Filter_byte__W_Filter_Offset, 0x01, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));		
        }
        if(err == TM_OK)
        {
            /* W_Filter_Bypass */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_W_Filter_byte__W_Filter_Bypass, 0x00, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));		
        }
        if(err == TM_OK)
        {
            /* W_Filter */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_W_Filter_byte__W_Filter, 0x02, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));		
        }
        if(err == TM_OK)
        {
            /* LPF_DCOffset_Corr */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_IF_AGC_byte__LPF_DCOffset_Corr, 0x00, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));		
        }
        if(err == TM_OK)
        {
            /* Force_AGC1_gain */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_3__Force_AGC1_gain, 0x00, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));		
        }
        if(err == TM_OK)
        {
            /* AGC1_loop_off */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_3__AGC1_loop_off, 0x00, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));		
        }
        if(err == TM_OK)
        {
            /* Force_AGC2_gain */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_1__Force_AGC2_gain, 0x00, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));		
        }
        if(err == TM_OK)
        {
            /* AGC2_loop_off */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_2__AGC2_loop_off, 0x00, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));		
        }
        if(err == TM_OK)
        {
            /* Force_AGC3_gain */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC3_byte_2__Force_AGC3_gain, 0x00, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));		
        }
        if(err == TM_OK)
        {
            /* AGC3_loop_off */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC3_byte_2__AGC3_loop_off, 0x00, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));		
        }
        if(err == TM_OK)
        {
            /* PD_Det1 */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Power_Down_byte_2__PD_Det1, 0x00, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));		
        }
        if(err == TM_OK)
        {
            /* PD_Det2 */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Power_Down_byte_2__PD_Det2, 0x00, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));		
        }
        if(err == TM_OK)
        {
            /* PD_Det3 */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Power_Down_byte_2__PD_Det3, 0x00, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));		
        }
    }

    /* Fields written to their different POR values */
    if (pObj->eChipVersion == TDA18250A_VersionES3)
    {
        if (err == TM_OK)
        {
            /* AGC2_top_range_adapt */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_1__AGC2_top_range_adapt, 0x01, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
    }
    
    if(err == TM_OK)
    {
        /* LO_SetAll */
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Sigma_delta_byte_5__LO_SetAll, 0x00, Bus_None);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        /* LNA Zin */
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_2__LNA_Zin, 0x01, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        /* PD_LT */
        uVal = (pObj->eLoopThrough == TDA18250A_LoopThrough_Disabled) ? 1: 0;
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_LT_byte_1__PD_LT, uVal, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        /* RFsw_LT_RFin */
        uVal = ( pObj->bMasterNotSlave && pObj->TunerModeInUse == TDA18250A_Mode_Dual ) ? 0x01: 0x00;
       // err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_LT_byte_2__RFsw_LT_RFin, uVal, Bus_NoRead);
		err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_LT_byte_2__RFsw_LT_RFin, 0x01, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        /* PSM_Mixer */
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_1__PSM_Mixer, 0x03, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        /* PSM_hph_ifn */
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_3__PSM_hpf_ifn, 0x03, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        /* ICP */
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Charge_pump_byte__ICP, 0x04, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));		
    }

    if(err == TM_OK)
    {
        /* AGC1_UP_step */
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_2__AGC1_Up_step, 0x01, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        /* AGC2_UP_step */
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_1__AGC2_Up_step, 0x01, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        /* AGC3_UP_step */
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC3_byte_3__AGC3_Up_step, 0x01, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
    }


    if(err == TM_OK)
    {
        if ( pObj->bMasterNotSlave && pObj->TunerModeInUse == TDA18250A_Mode_Dual )
        {
            /* LT_smooth_t_cst */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_LT_byte_2__LT_smooth_t_cst, 0x06, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
    }

    if(err == TM_OK)
    {
        if ( pObj->bMasterNotSlave && pObj->TunerModeInUse == TDA18250A_Mode_Dual )
        {
            /* LT_smooth_t_cst_fast */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_LT_byte_2__LT_smooth_t_cst_fast, 0x01, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
    }

    if(err == TM_OK)
    {
        /* AGC1_smooth_t_cst */
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_4__AGC1_smooth_t_cst, 0x06, Bus_None);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        /* AGC1_smooth_t_cst_fast */
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_4__AGC1_smooth_t_cst_fast, 0x01, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        /* IF_Det_Caprst_En */
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC3_byte_2__IF_Det_Caprst_En, 0x00, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        /* IRQ_Polarity */
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_IRQ_Ctrl_byte__IRQ_Polarity, pObj->eIRQPolarity, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        /* XToutAmp */
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Reference_Byte__Xtout_Amp, pObj->eXTOutAmp, Bus_None);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        /* XTOut */
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Reference_Byte__XTout, pObj->eXTOut, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        /* Xt_Freq_2 */
        uVal = (RDivNDivTable[pObj->eXtalFreq-1].uXtalFreq/1000) & 0xFF;
        uVal2 = ((RDivNDivTable[pObj->eXtalFreq-1].uXtalFreq/1000) >> 8) & 0x7F;
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Xtal_Flex_byte_1__XT_Freq_2, uVal2, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        /* Xt_Freq_1 */
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Xtal_Flex_byte_2__XT_Freq_1, uVal, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        /* N_Div */
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Xtal_Flex_byte_3__N_Div, (UInt8)RDivNDivTable[pObj->eXtalFreq-1].psVCOModes[0].uNDiv, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        /* R_Div */
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Xtal_Flex_byte_3__R_Div, (UInt8)RDivNDivTable[pObj->eXtalFreq-1].psVCOModes[0].uRDiv, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
    }

	if(err == TM_OK)
    {
        /* Force_Icp_256us */
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Xtal_Flex_byte_4__Force_Icp_256us_1, (UInt8)(RDivNDivTable[pObj->eXtalFreq-1].uForceIcp256us & 0x00FF), Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
		
		if(err == TM_OK)
		{
			err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Xtal_Flex_byte_3__Force_Icp_256us_2, (UInt8)((RDivNDivTable[pObj->eXtalFreq-1].uForceIcp256us & 0x1F00) >> 8), Bus_NoRead);
			tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
		}
    }

	if(err == TM_OK)
    {
        /* RC_Delay_Ck_Length */
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Xtal_flex_byte_5__RC_Delay_Ck_Length, (UInt8)RDivNDivTable[pObj->eXtalFreq-1].uRcDelayLength, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        /* IR_target */
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_IR_Cal_byte_5__IR_target, 0x03, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        /* RSSI_AV */
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_RSSI_byte_2__RSSI_AV, 0x00, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        /* IFAgcToRSSI */
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_IF_AGC_byte__IFAgcToRSSI, 0x00, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        /* LNA_Range */
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_2__LNA_Range, pObj->eLNA_Range, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        /* Power Down LNA  -> 1 */
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_1__PD_LNA, 0x01, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        /* AGC2_FAST_AUTO_DELTA */
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_UDLD_byte_2__AGC2_FAST_AUTO_DELTA, 0x05, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        /* AGC_timer_mod_fast_auto */
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC_fast_auto_byte_1__AGC_timer_mod_fast_auto, TDA18250A_AGC_Timer_Mod_Fast_Auto_01, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        /* AGC_type_fast_auto */
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC_fast_auto_byte_1__AGC_type_fast_auto, TDA18250A_AGC_Type_Fast_Auto_11, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        /* RC_Cal_Offset */
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_RC_Cal_byte_1__RC_Cal_Offset, 0x01, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        /* Set IRQ_clear */
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_IRQ_clear, TDA18250A_IRQ_Global|TDA18250A_IRQ_HwInit, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        pObj->curPowerState = tmPowerOn;
#endif
        /* Launch tuner calibration */
#if 1
        /* Set state machine and Launch it */
        if(err == TM_OK)
        {
            err = iTDA18250A_SetMSM(pObj, TDA18250A_MSM_HwInit, True);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_SetMSM(0x%08X, TDA18250A_MSM_HwInit) failed.", pObj->tUnitW));
        }

        /* Inform that init phase has started */
        if(err == TM_OK)
        {
            pObj->eHwState = TDA18250A_HwState_InitPending;
        }

        if(err == TM_OK)
        {
            /* State reached after 500 ms max */
            err = iTDA18250A_WaitIRQ(pObj, 500, 10, TDA18250A_IRQ_HwInit);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_WaitIRQ(0x%08X) failed.", tUnit));
        }
#endif
#if 1
        /* Patch : launch the RC Cal after the other cals */
        if(err == TM_OK)
        {
            err = iTDA18250A_SetMSM(pObj, TDA18250A_MSM_RC_Cal, True);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_SetMSM(0x%08X, TDA18250A_MSM_HwInit) failed.", pObj->tUnitW));
        }

        if(err == TM_OK)
        {
            /* State reached after 500 ms max */
            err = iTDA18250A_WaitIRQ(pObj, 500, 10, TDA18250A_IRQ_Global|TDA18250A_IRQ_MSM_RCCal);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_WaitIRQ(0x%08X) failed.", tUnit));
        }
#endif  
#if 1
        if(err == TM_OK && pObj->eHwState == TDA18250A_HwState_InitPending)
        {
            pObj->eHwState = TDA18250A_HwState_InitDone;
        }
        else
        {
            pObj->eHwState = TDA18250A_HwState_InitNotDone;
        }
        if(err == TM_OK)
        {
            if ( pObj->bMasterNotSlave && pObj->TunerModeInUse == TDA18250A_Mode_Dual )
            {
                /* Set power state to stdby default 16Mhz ( XTOUT on , LT/LNA on  , PLL off) */
                if (pObj->eLoopThrough == TDA18250A_LoopThrough_Disabled)
                {
                    err = iTDA18250A_SetLLPowerState(pObj, TDA18250A_PowerStandbyWithXtalOn);
                }
                else
                {
                    err = iTDA18250A_SetLLPowerState(pObj, TDA18250A_PowerStandbyWithLtOnWithXtalOn);
                }
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_SetLLPowerState(0x%08X, PowerNormalMode) failed.", tUnit));
            }
            else
            {
                /* Set power state to stdby default 16Mhz ( XTOUT on , LT/LNA off , PLL off) */
                if (pObj->eLoopThrough == TDA18250A_LoopThrough_Disabled)
                {
                    err = iTDA18250A_SetLLPowerState(pObj, TDA18250A_PowerStandbyWithXtalOn);
                }
                else
                {
                    err = iTDA18250A_SetLLPowerState(pObj, TDA18250A_PowerStandbyWithLtOnWithXtalOn);
                }
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_SetLLPowerState(0x%08X, PowerNormalMode) failed.", tUnit));
            }
        }
        if(err == TM_OK)
        {
            /* Power Down LNA  -> 0 */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_1__PD_LNA, 0x00, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
    }
#endif
    _MUTEX_RELEASE(TDA18250A)

        return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslTDA18250A_GetIF:                                          */
/*                                                                            */
/* DESCRIPTION: Gets programmed IF.                                           */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslTDA18250A_GetIF(
    tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt32*         puIF    /* O: IF Frequency in hertz */
)
{
    pTDA18250AObject_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = iTDA18250A_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18250A)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslTDA18250A_GetIF(0x%08X)", tUnit);

    /* Test parameter(s) */
    if(   pObj->StandardMode<=TDA18250A_StandardMode_Unknown
        || pObj->StandardMode>=TDA18250A_StandardMode_Max
        || pObj->pStandard == Null
        || puIF == Null)
    {
        err = TDA18250A_ERR_BAD_PARAMETER;
    }

    if(err == TM_OK)
    {
        *puIF = pObj->pStandard->IF - pObj->pStandard->CF_Offset;
    }

    _MUTEX_RELEASE(TDA18250A)

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmbslTDA18250A_SetIFLevel                                     */
/*                                                                            */
/* DESCRIPTION: Sets the IF Level                                             */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslTDA18250A_SetIFLevel(
    tmUnitSelect_t              tUnit,      /* I: Unit number */
    TDA18250AIF_Output_Level_t  eIFLevel    /* I: IF Level */
)
{
    pTDA18250AObject_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = iTDA18250A_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18250A)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslTDA18250A_SetIFLevel(0x%08X)", tUnit);

    if (err == TM_OK)
    {
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_IF_AGC_byte__IF_level, eIFLevel, Bus_RW);
    }

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));

    _MUTEX_RELEASE(TDA18250A)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslTDA18250A_GetIFLevel                                     */
/*                                                                            */
/* DESCRIPTION: Gets the IF Level                                             */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslTDA18250A_GetIFLevel(
    tmUnitSelect_t              tUnit,      /* I: Unit number */
    TDA18250AIF_Output_Level_t  *peIFLevel  /* I: IF Level */
)
{
    pTDA18250AObject_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;
    UInt8 uData;

    /* Get a driver instance */
    err = iTDA18250A_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18250A)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslTDA18250A_GetIFLevel(0x%08X)", tUnit);

    err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_IF_AGC_byte__IF_level, &uData, Bus_RW);

    *peIFLevel = (TDA18250AIF_Output_Level_t)uData;

    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Read(0x%08X) failed.", pObj->tUnitW));

    _MUTEX_RELEASE(TDA18250A)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslTDA18250A_GetPowerLevel                                  */
/*                                                                            */
/* DESCRIPTION: Gets the power level                                          */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslTDA18250A_GetPowerLevel(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    Int32*          pPowerLevel /* O: Power Level in (x100) dBµVrms */
)
{
	pTDA18250AObject_t   pObj = Null;
	tmErrorCode_t       err = TM_OK;
	UInt8				RSSIValue = 0;
    UInt8 uRSSIMeasSave = 0x00;

	/* Get a driver instance */
	err = iTDA18250A_GetInstance(tUnit, &pObj);

	_MUTEX_ACQUIRE(TDA18250A)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslTDA18250A_GetPowerLevel(0x%08X)", tUnit);

	/* Test parameter(s) */
	if( pPowerLevel == Null ||
		pObj->StandardMode<=TDA18250A_StandardMode_Unknown ||
		pObj->StandardMode>=TDA18250A_StandardMode_Max ||
		pObj->pStandard == Null)
	{
		err = TDA18250A_ERR_BAD_PARAMETER;
	}

	if(err == TM_OK)
	{
        err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_MSM_byte_1__RSSI_Meas, &uRSSIMeasSave, Bus_RW);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Read(0x%08X) failed.", pObj->tUnitW));

		/* Get the RSSI value for the power level estimation*/
		err = iTDA18250A_GetRSSI(pObj, &RSSIValue);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_GetRSSI(0x%08X) failed.", tUnit));

		if (err == TM_OK)
		{
			err =  iTDA18250A_GetPowerLevel(pObj, RSSIValue, pPowerLevel);
			tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250AGetPower_Level(0x%08X) failed.", tUnit));
		}
		else
		{
			*pPowerLevel = 0;
		}

        if (err == TM_OK)
        {
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_MSM_byte_1__RSSI_Meas, uRSSIMeasSave, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", tUnit));
        }
	}

	_MUTEX_RELEASE(TDA18250A)

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslTDA18250A_CheckHWVersion                                 */
/*                                                                            */
/* DESCRIPTION: Gets the ES version of the HW                                 */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmbslTDA18250A_CheckHWVersion(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    pTDA18250AVersion_t pEsVersion  /*  o: ES version of the HW */
)
{
	pTDA18250AObject_t   pObj = Null;
	tmErrorCode_t       err = TM_OK;

	/* Get a driver instance */
	err = iTDA18250A_GetInstance(tUnit, &pObj);

	_MUTEX_ACQUIRE(TDA18250A)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslTDA18250A_CheckHWVersion(0x%08X)", tUnit);

	/* Test parameter(s) */
	if (pEsVersion == Null)
	{
		err = TDA18250A_ERR_BAD_PARAMETER;
	}

	if(err == TM_OK)
	{
        *pEsVersion = pObj->eChipVersion;
	}

	_MUTEX_RELEASE(TDA18250A)

    return err;
}
tmErrorCode_t
tmbslTDA18250A_Channel_Switching_Complete(
                    tmUnitSelect_t              tUnit     /* I: FrontEnd unit number */
                    )
{
		pTDA18250AObject_t	 pObj = Null;
		tmErrorCode_t		err = TM_OK;
	
		/* Get a driver instance */
		err = iTDA18250A_GetInstance(tUnit, &pObj);

		/* Disable Continuous FAST mode*/
		err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_2__Fast_Mode_AGC,0x00, Bus_NoRead);

        return err;
}                                      

/*============================================================================*/
/* Internal functions:                                                        */
/*============================================================================*/
/*============================================================================*/
/* FUNCTION:    iTDA18250A_GetRSSI                                             */
/*                                                                            */
/* DESCRIPTION: Reads RSSI.                                                   */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18250A_GetRSSI(
    pTDA18250AObject_t   pObj,   /* I: Driver object */
    UInt8*              puValue /* O: RSSI value */
)
{
    tmErrorCode_t   err = TM_OK;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18250A_GetRSSI(0x%08X)", pObj->tUnitW);

    *puValue = 0;

    /* Set IRQ_clear*/
    err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_IRQ_clear, TDA18250A_IRQ_Global|TDA18250A_IRQ_GetPowerLevel, Bus_NoRead);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));

    if(err == TM_OK)
    {
        /* Set state machine and Launch it */
        err = iTDA18250A_SetMSM(pObj, TDA18250A_MSM_GetPowerLevel, True);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_SetMSM(0x%08X, TDA18250A_MSM_GetPowerLevel) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        err = iTDA18250A_WaitIRQ(pObj, 700, 1, TDA18250A_IRQ_GetPowerLevel);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_WaitIRQ(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        /* Read Power_Level_byte */
        err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_RSSI_byte_1__RSSI, puValue, Bus_RW);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Read(0x%08X) failed.", pObj->tUnitW));

        if(err != TM_OK)
        {
            err = TDA18250A_ERR_HW_FAILED;
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18250A_GetPowerLevel                                      */
/*                                                                            */
/* DESCRIPTION: Get the input PowerLevel from RSSI                            */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18250A_GetPowerLevel(
    pTDA18250AObject_t   pObj,     /* I: Driver object */
	UInt8           uRSSIValue,   /* I: RSSI value reading previously                */
	Int32*          pPowerLevel   /* O: Address of the variable to output item value */
)
{
    tmErrorCode_t   err = TM_OK;
	Int32           PowerLevel = 0; /* in (x100) dBµV */
    Int32           DeltaAGC1NBTunerGain = 0; /* in dB x 1000 */
    Int32           DeltaAGC1NBMasterGain = 0; /* in dB x 1000 */
    Int32           DeltaAGC2NBGain = 0; /* in dB x 1000 */
    Int32           DeltaAGC3NBGain = 0; /* in dB x 1000 */
    Int32           DeltaS2DNBGain = 0; /* in dB x 1000 */
    Int32           GainMaxReference = 0; /* in dB x 1000 */
    UInt32 uRF = 0;
    pTDA18250AObject_t   psMasterObj = {0};

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18250A_GetPowerLevel(0x%08X)", pObj->tUnitW);

        
    uRF = pObj->uRF;        

    /*************************************/
    /*  RSSI contribution                */
    /*************************************/
	/* For 0 < RSSI[7:0] <= 203  => RSSI value= 109.5 - RSSI[7:0] *0.25 (in dBµV) */
	/* For RSSI[7:0] > 204     => RSSI value = 58.75dBµVrms (in dBµV) */
	if ( uRSSIValue > 204 )
	{
		PowerLevel = 10975;
	}
	else
	{
		PowerLevel = 5875 + uRSSIValue*25;
	}
 
    /**********************************************************/
    /*  Retrieve reference Max Gain of NB path (in dB x 1000) */
    /**********************************************************/
    err = iTDA18250A_GetRefMaxGainNB(pObj, &GainMaxReference);

    /*************************************/
    /*  AGC1 contribution (in dB x 1000) */
    /*************************************/
    if (err == TM_OK)
    {
        err = iTDA18250A_LvlAgc1NBTunerContribution(pObj, &DeltaAGC1NBTunerGain);
    }

    if (err == TM_OK)
    {
        if (!pObj->bMasterNotSlave)
        {
            if (pObj->getMasterContribution != Null)
            {
                err = pObj->getMasterContribution(uRF, &DeltaAGC1NBMasterGain); /* result is in 100 * dB */
                if (err == TM_OK)
                {
                    DeltaAGC1NBMasterGain *= 10;
                }
            }
            else if (pObj->tOtherUnit != (tmUnitSelect_t)-1)
            {
                err = iTDA18250A_GetInstance(pObj->tOtherUnit, &psMasterObj); 
                err = iTDA18250A_LvlAgc1NBMasterContribution(psMasterObj, uRF, &DeltaAGC1NBMasterGain);
            }
        }
    }

    /****************************************/
    /*  AGC2 NB contribution (in dB x 1000) */
    /****************************************/
    if (err == TM_OK)
    {
        err = iTDA18250A_LvlAgc2NBContribution(pObj, &DeltaAGC2NBGain);
    }

    /****************************************/
    /*  AGC3 NB contribution (in dB x 1000) */
    /****************************************/
    if (err == TM_OK)
    {
        err = iTDA18250A_LvlAgc3NBContribution(pObj, &DeltaAGC3NBGain);
    }

    /****************************************/
    /*  AGC3 NB contribution (in dB x 1000) */
    /****************************************/
    if (err == TM_OK)
    {
        err = iTDA18250A_LvlS2DNBContribution(pObj, &DeltaS2DNBGain);
    }

    /* *pPowerLevel = PowerLevel + ((DeltaAGC1NBGain + DeltaAGC2NBGain + DeltaAGC3NBGain + DeltaRFFilterGain)/10); */
    *pPowerLevel = PowerLevel - ((GainMaxReference + DeltaAGC1NBTunerGain + DeltaAGC1NBMasterGain + DeltaAGC2NBGain + DeltaAGC3NBGain + DeltaS2DNBGain)/10);

    return err;
}


/*============================================================================*/
/* FUNCTION:    iTDA18250A_GetRefMaxGainNB                                    */
/*                                                                            */
/* DESCRIPTION: Max reference gain on NB path (taking into account H3H5,      */
/*              LPF (Wifi), HPF (agc2) and S2D filters                        */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18250A_GetRefMaxGainNB(
    pTDA18250AObject_t   pObj,        /* I: Driver object */
    Int32*              pMaxGain     /* O: Max gain value in dB (x1000) */
)
{
    tmErrorCode_t   err = TM_OK;
    UInt32          uIndex = 0;
    Int32           MaxGain = 0;
    UInt32          uRF = 0; /* RF in MHz */


    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18250A_GetRefMaxGainNB(0x%08X)", pObj->tUnitW);

    uRF = ( pObj->uRF / 1000000 );

    while ( (uIndex < TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_REF_GAIN_MAX) && ((Int32)pObj->uRF >= Lvl_MAX_GAIN_NB[uIndex][TDA18250A_Coeff_Y_Code]) )
        uIndex = uIndex + 1;

    if (uIndex == TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_REF_GAIN_MAX)
    {
        uIndex = uIndex - 1;
    }

    /*   MaxGain (dB)        = X2*RF^2/10000000 + X1*RF/1000000 + X0/1000000   */
    /*   delta gain_AGC1_NB (dB * 1000) = ( X2*RF^2/10 + X1*RF + X0 )/1000   */
    /* X0 : Lvl_MAX_GAIN_NB[uIndex][TDA18250A_Coeff_X0] */
    /* X1 : Lvl_MAX_GAIN_NB[uIndex][TDA18250A_Coeff_X1] */
    /* X2 : Lvl_MAX_GAIN_NB[uIndex][TDA18250A_Coeff_X2] */
    MaxGain = (Int32)(Lvl_MAX_GAIN_NB[uIndex][TDA18250A_Coeff_X0] + (Int32)(Lvl_MAX_GAIN_NB[uIndex][TDA18250A_Coeff_X1] * uRF ) + (Int32)(Lvl_MAX_GAIN_NB[uIndex][TDA18250A_Coeff_X2] * uRF * uRF)/(Int32)10 );
    MaxGain = MaxGain / (Int32)1000;

    *pMaxGain = (Int32)MaxGain;

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18250A_LvlAgc1NBTunerContribution                         */
/*                                                                            */
/* DESCRIPTION: AGC1 NB Contribution for iTDA18250A_GetPowerLevel()           */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18250A_LvlAgc1NBTunerContribution(
    pTDA18250AObject_t   pObj,        /* I: Driver object */
    Int32*              pDeltaGain   /* O: Delta AGC1 Gain value in dB (x1000) */
)
{
    tmErrorCode_t   err = TM_OK;
    UInt32          uIndex = 0;
    UInt8           uAGC1NBGainRead = 0;
    UInt8           uAGC1GainForce = 0;
/*    Int32           lReferenceDeltaGain = 0; */
    Int32           lSwLTDeltaGain = 0;
    UInt32          uRF = 0; /* RF in MHz */
    AGC1NBTable_t   *plCurrentTable;
    UInt8           uRFsw_LT_RFin = 0;
    UInt8           uPD_LT = 0;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18250A_LvlAgc1NBContribution(0x%08X)", pObj->tUnitW);

    uRF = ( pObj->uRF / 1000000 );

    /* retrieve AGC1_NB_GAIN_READ */
    err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_AGC1_byte_3__Force_AGC1_gain, &uAGC1GainForce, Bus_NoWrite);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Read(0x%08X) failed.", pObj->tUnitW));

    if (err == TM_OK)
    {
        /* if AGC1 Gain is forced, the real value should be read in AGC1_Gain */
        if ( uAGC1GainForce == 0x0 )
        {
            err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_AGC1_LT_Gain_status__AGC1_gain_read, &uAGC1NBGainRead, Bus_NoWrite);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Read(0x%08X) failed.", pObj->tUnitW));
        }
        else
        {
            err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_AGC1_byte_3__AGC1_Gain, &uAGC1NBGainRead, Bus_NoWrite);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Read(0x%08X) failed.", pObj->tUnitW));
        }
    }

    if (err == TM_OK)
    {
        while ( (uIndex < TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_AGC1_MAX) && (uAGC1NBGainRead > (Lvl_AGC1NB_TUNER_LT_OFF_ST_OFF[uIndex][TDA18250A_Coeff_Y_Code]-1)) )
        {
            uIndex = uIndex + 1;
        }
        
        if (uIndex == TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_AGC1_MAX)
        {
            uIndex = uIndex - 1;
        }
#if 0
        /*   delta gain_AGC1_NB (dB)        = X2*RF^2/100000000 + X1*RF/10000000 + X0/10000000   */
        /*   delta gain_AGC1_NB (dB * 1000) = ( X2*RF^2/10 + X1*RF + X0 )/10000   */
        /* X0 : Lvl_AGC1NB[uIndex][TDA18250A_Coeff_X0] */
        /* X1 : Lvl_AGC1NB[uIndex][TDA18250A_Coeff_X1] */
        /* X2 : Lvl_AGC1NB[uIndex][TDA18250A_Coeff_X2] */
        plCurrentTable = &Lvl_AGC1NB_TUNER_LT_OFF_ST_OFF;
        
        lReferenceDeltaGain = (*plCurrentTable)[uIndex][TDA18250A_Coeff_X0];
        lReferenceDeltaGain += (*plCurrentTable)[uIndex][TDA18250A_Coeff_X1] * (Int32)uRF;
        lReferenceDeltaGain += (*plCurrentTable)[uIndex][TDA18250A_Coeff_X2] * (Int32)uRF * (Int32)uRF / 10;
        lReferenceDeltaGain /= 10000;

        *pDeltaGain = lReferenceDeltaGain;
#endif
        *pDeltaGain = 0;
    }
    
    if(err == TM_OK)
    {
        /* PD_LT */
        err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_LT_byte_1__PD_LT, &uPD_LT, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Read(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        /* RFsw_LT_RFin */
        err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_LT_byte_2__RFsw_LT_RFin, &uRFsw_LT_RFin, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Read(0x%08X) failed.", pObj->tUnitW));
    }

    /* Add the contribution of RF Switch and LT if necessary */
    if(err == TM_OK)
    {
        if (uRFsw_LT_RFin == 0x01)
        {
            if (uPD_LT == 0x01)
            {
                plCurrentTable = &Lvl_AGC1NB_TUNER_LT_OFF_ST_ON;
            }
            else
            {
                plCurrentTable = &Lvl_AGC1NB_TUNER_LT_ON_ST_ON;
            }
        }
        else
        {
            plCurrentTable = &Lvl_AGC1NB_TUNER_LT_OFF_ST_OFF;
        }

        lSwLTDeltaGain = (*plCurrentTable)[uIndex][TDA18250A_Coeff_X0];
        lSwLTDeltaGain += (*plCurrentTable)[uIndex][TDA18250A_Coeff_X1] * (Int32)uRF;
        lSwLTDeltaGain += (*plCurrentTable)[uIndex][TDA18250A_Coeff_X2] * (Int32)uRF * (Int32)uRF / 10;
        lSwLTDeltaGain /= 10000;
        *pDeltaGain += lSwLTDeltaGain;
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18250A_LvlAgc1NBMasterContribution                        */
/*                                                                            */
/* DESCRIPTION: Master AGC1 NB Contribution for iTDA18250A_GetPowerLevel()    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18250A_LvlAgc1NBMasterContribution(
    pTDA18250AObject_t  pObj,        /* I: Driver object */
    UInt32              uRF,         /* I : RF in MHz */
    Int32*              pDeltaGain   /* O: Delta AGC1 Gain value in dB (x1000) */
)
{
    tmErrorCode_t   err = TM_OK;
    UInt32          uIndex = 0;
    UInt8           uAGC1NBGainRead = 0;
    UInt8           uAGC1GainForce = 0;
    Int32           lReferenceDeltaGain = 0;
    AGC1NBTable_t   *plCurrentTable = 0;
    UInt8           uPD_LT = 0;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18250A_LvlAgc1NBContribution(0x%08X)", pObj->tUnitW);

    /* retrieve AGC1_NB_GAIN_READ */
    err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_AGC1_byte_3__Force_AGC1_gain, &uAGC1GainForce, Bus_NoWrite);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Read(0x%08X) failed.", pObj->tUnitW));

    if (err == TM_OK)
    {
        /* if AGC1 Gain is forced, the real value should be read in AGC1_Gain */
        if ( uAGC1GainForce == 0x0 )
        {
            err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_AGC1_LT_Gain_status__AGC1_gain_read, &uAGC1NBGainRead, Bus_NoWrite);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Read(0x%08X) failed.", pObj->tUnitW));
        }
        else
        {
            err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_AGC1_byte_3__AGC1_Gain, &uAGC1NBGainRead, Bus_NoWrite);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Read(0x%08X) failed.", pObj->tUnitW));
        }
    }

    if(err == TM_OK)
    {
        /* PD_LT */
        err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_LT_byte_1__PD_LT, &uPD_LT, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Read(0x%08X) failed.", pObj->tUnitW));
    }

    if (err == TM_OK)
    {
        if (uPD_LT == 0x01)
        {
            plCurrentTable = &Lvl_AGC1NB_MASTER_LT_OFF_ST_ON;
        }
        else
        {
            plCurrentTable = &Lvl_AGC1NB_MASTER_LT_ON_ST_ON;
        }

        while ( (uIndex < TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_AGC1_MAX) && (uAGC1NBGainRead > ((*plCurrentTable)[uIndex][TDA18250A_Coeff_Y_Code]-1)) )
        {
            uIndex = uIndex + 1;
        }

        if (uIndex == TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_AGC1_MAX)
        {
            uIndex = uIndex - 1;
        }

        lReferenceDeltaGain = (*plCurrentTable)[uIndex][TDA18250A_Coeff_X0];
        lReferenceDeltaGain += (*plCurrentTable)[uIndex][TDA18250A_Coeff_X1] * (Int32)uRF;
        lReferenceDeltaGain += (*plCurrentTable)[uIndex][TDA18250A_Coeff_X2] * (Int32)uRF * (Int32)uRF / 10;
        lReferenceDeltaGain /= 10000;

        *pDeltaGain = lReferenceDeltaGain;
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18250A_LvlAgc2NBContribution                              */
/*                                                                            */
/* DESCRIPTION: AGC2 NB Contribution for iTDA18250A_GetPowerLevel()           */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18250A_LvlAgc2NBContribution(
    pTDA18250AObject_t   pObj,        /* I: Driver object */
    Int32*              pDeltaGain   /* O: Delta AGC2 Gain value in dB (x1000) */
)
{
    tmErrorCode_t   err = TM_OK;
    UInt32          uIndex = 0;
    UInt8           uAGC2NBGainRead = 0;
    Int32           DeltaGain = 0;
    UInt32          uRF = 0; /* RF in MHz */

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18250A_LvlAgc2NBContribution(0x%08X)", pObj->tUnitW);

    uRF = ( pObj->uRF / 1000000 );

    /* retrieve AGC2_NB_GAIN_READ */
    err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_AGC2_Gain_status__AGC2_gain_read, &uAGC2NBGainRead, Bus_NoWrite);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Read(0x%08X) failed.", pObj->tUnitW));

    if (err == TM_OK)
    {
        while ( (uIndex < TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_AGC2_MAX) && (uAGC2NBGainRead > Lvl_AGC2NB[uIndex][TDA18250A_Coeff_Y_Code]) )
            uIndex = uIndex + 1;
        
        if (uIndex == TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_AGC2_MAX)
        {
            uIndex = uIndex - 1;
        }
    }

    /*   delta gain_AGC2_NB (dB)        = X2*RF/100000 + X1*uAGC2NBGainRead/10000 + X0/10000   */
    /*   delta gain_AGC2_NB (dB * 1000) = (X2*RF/10 + X1*uAGC2NBGainRead + X0) / 10   */
    /* X0 : Lvl_AGC2NB[uIndex][TDA18250A_Coeff_X0] */
    /* X1 : Lvl_AGC2NB[uIndex][TDA18250A_Coeff_X1] */
    /* X2 : Lvl_AGC2NB[uIndex][TDA18250A_Coeff_X2] */
    DeltaGain = (Int32)(Lvl_AGC2NB[uIndex][TDA18250A_Coeff_X0] + (Int32)(Lvl_AGC2NB[uIndex][TDA18250A_Coeff_X1] * uAGC2NBGainRead ) + (Int32)(Lvl_AGC2NB[uIndex][TDA18250A_Coeff_X2] * uRF )/(Int32)10 );
    DeltaGain = DeltaGain / (Int32)10;

    *pDeltaGain = (Int32)DeltaGain;

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18250A_LvlAgc3NBContribution                              */
/*                                                                            */
/* DESCRIPTION: AGC3 NB Contribution for iTDA18250A_GetPowerLevel()           */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18250A_LvlAgc3NBContribution(
    pTDA18250AObject_t   pObj,        /* I: Driver object */
    Int32*              pDeltaGain   /* O: Delta AGC3 Gain value in dB (x1000) */
)
{
    tmErrorCode_t   err = TM_OK;
    UInt32          uIndex = 0;
    UInt8           uAGC3NBGainRead = 0;
    Int32           DeltaGain = 0;
    UInt32          uRF = 0; /* RF in MHz */

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18250A_LvlAgc3NBContribution(0x%08X)", pObj->tUnitW);

    uRF = ( pObj->uRF / 1000000 );

    /* retrieve AGC3_NB_GAIN_READ */
    err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_AGC3_gain_status__AGC3_Gain_Read, &uAGC3NBGainRead, Bus_NoWrite);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Read(0x%08X) failed.", pObj->tUnitW));

    if (err == TM_OK)
    {
        while ( (uIndex < TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_AGC3_MAX) && (uAGC3NBGainRead > (Lvl_AGC3NB[uIndex][TDA18250A_Coeff_Y_Code]-1)) )
            uIndex = uIndex + 1;
        
        if (uIndex == TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_AGC3_MAX)
        {
            uIndex = uIndex - 1;
        }
    }

    /*   delta gain_AGC3_NB (dB)        = X2*RF^2/100000000 + X1*RF/10000000 + X0/10000000   */
    /*   delta gain_AGC3_NB (dB * 1000) = ( X2*RF^2/10 + X1*RF + X0 )/10000   */
    /* X0 : Lvl_AGC3NB[uIndex][TDA18250A_Coeff_X0] */
    /* X1 : Lvl_AGC3NB[uIndex][TDA18250A_Coeff_X1] */
    /* X2 : Lvl_AGC3NB[uIndex][TDA18250A_Coeff_X2] */
    DeltaGain = (Int32)(Lvl_AGC3NB[uIndex][TDA18250A_Coeff_X0] + (Int32)(Lvl_AGC3NB[uIndex][TDA18250A_Coeff_X1] * uRF ) + (Int32)(Lvl_AGC3NB[uIndex][TDA18250A_Coeff_X2] * uRF * uRF)/(Int32)10 );
    DeltaGain = DeltaGain / (Int32)10000;

    *pDeltaGain = (Int32)DeltaGain;

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18250A_LvlS2DNBContribution                               */
/*                                                                            */
/* DESCRIPTION: S2D NB Contribution for iTDA18250A_GetPowerLevel()            */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18250A_LvlS2DNBContribution(
    pTDA18250AObject_t   pObj,        /* I: Driver object */
    Int32*              pDeltaGain   /* O: Delta S2D Gain value in dB (x1000) */
)
{
    tmErrorCode_t   err = TM_OK;
    UInt32          uIndex = 0;
    UInt8           uS2DNBGainRead = 0;
    Int32           DeltaGain = 0;
    UInt32          uRF = 0; /* RF in MHz */

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18250A_LvlS2DNBContribution(0x%08X)", pObj->tUnitW);

    uRF = ( pObj->uRF / 1000000 );

    /* retrieve S2D GAIN */
    err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_IR_Mixer_byte_1__S2D_Gain, &uS2DNBGainRead, Bus_NoWrite);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Read(0x%08X) failed.", pObj->tUnitW));

    if (err == TM_OK)
    {
        uIndex = (UInt32)uS2DNBGainRead;
    }

    /*   delta gain_S2D_NB (dB)        = X2*RF^2/100000000 + X1*RF/10000000 + X0/10000000   */
    /*   delta gain_S2D_NB (dB * 1000) = ( X2*RF^2/10 + X1*RF + X0 )/10000   */
    /* X0 : Lvl_S2DNB[uIndex][TDA18250A_Coeff_X0] */
    /* X1 : Lvl_S2DNB[uIndex][TDA18250A_Coeff_X1] */
    /* X2 : Lvl_S2DNB[uIndex][TDA18250A_Coeff_X2] */
    DeltaGain = (Int32)(Lvl_S2DNB[uIndex][TDA18250A_Coeff_X0] + (Int32)(Lvl_S2DNB[uIndex][TDA18250A_Coeff_X1] * uRF ) + (Int32)(Lvl_S2DNB[uIndex][TDA18250A_Coeff_X2] * uRF * uRF)/(Int32)10 );
    DeltaGain = DeltaGain / (Int32)10000;

    *pDeltaGain = (Int32)DeltaGain;

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18250A_CheckHwState                                        */
/*                                                                            */
/* DESCRIPTION: Checks if Hw is ready to operate.                             */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18250A_CheckHwState(
                        pTDA18250AObject_t       pObj,   /* I: Driver object */
                        TDA18250AHwStateCaller_t caller  /* I: Caller API */
                        )
{
    tmErrorCode_t   err = TM_OK;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18250A_CheckHwState(0x%08X)", pObj->tUnitW);

    switch(pObj->eHwState)
    {
    case TDA18250A_HwState_InitNotDone:
        switch(caller)
        {
        case TDA18250A_HwStateCaller_SetPower:
            break;

        default:
            err = TDA18250A_ERR_NOT_INITIALIZED;
            break;
        }
        break;

    case TDA18250A_HwState_InitDone:
        switch(caller)
        {
        case TDA18250A_HwStateCaller_SetRF:
        case TDA18250A_HwStateCaller_SetFineRF:
            /* SetStandardMode API must be called before calling SetRF and SetFineRF */
            err = TDA18250A_ERR_STD_NOT_SET;
            break;

        default:
            break;
        }
        break;

    case TDA18250A_HwState_SetStdDone:
        switch(caller)
        {
        case TDA18250A_HwStateCaller_SetFineRF:
            /* SetRF API must be called before calling SetFineRF */
            err = TDA18250A_ERR_RF_NOT_SET;
            break;

        default:
            break;
        }
        break;

    case TDA18250A_HwState_SetRFDone:
    case TDA18250A_HwState_SetFineRFDone:
        break;

    case TDA18250A_HwState_InitPending:
        err = TDA18250A_ERR_NOT_READY;
        break;            
    default:
        err = TDA18250A_ERR_NOT_READY;
        break;
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18250A_SetLLPowerState                                     */
/*                                                                            */
/* DESCRIPTION: Sets the power state.                                         */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18250A_SetLLPowerState(
                           pTDA18250AObject_t       pObj,       /* I: Driver object */
                           TDA18250APowerState_t    powerState  /* I: Power state of TDA18250A */
                           )
{
    tmErrorCode_t   err = TM_OK;
    UInt8           uValue = 0;
    UInt8 uAGC2;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18250A_SetLLPowerState(0x%08X)", pObj->tUnitW);

    if(err == TM_OK)
    {
        /* Check if Hw is ready to operate */
        err = iTDA18250A_CheckHwState(pObj, TDA18250A_HwStateCaller_SetPower);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_CheckHwState(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        switch(powerState)
        {
        case TDA18250A_PowerNormalMode:
            if ( (pObj->curLLPowerState == TDA18250A_PowerStandbySmoothLTOnXtOutOn) || (pObj->curLLPowerState == TDA18250A_PowerStandbySmoothLTOffXtOutOff) || (pObj->curLLPowerState == TDA18250A_PowerStandbySmoothLTOffLNAOnXtOutOn))
            {
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_LT_byte_1__PD_LT, pObj->sSmoothCurrentStateSave.uPDLT, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Power_state_byte_2__Power_State_Mode, 0, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Power_Down_byte_2__PD_S2B_IF, pObj->sSmoothCurrentStateSave.uPDS2BIF, Bus_RW);

                /* If this instance is a slave set the AGC1 Gain to -5dB before changing the LNA */
                if (err == TM_OK)
                {
                    if (!pObj->bMasterNotSlave)
                    {
                        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_3__Force_AGC1_gain, 0x01, Bus_RW);
                        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
                        
                        if (err == TM_OK)
                        {
                            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_3__AGC1_Gain, (UInt8)TDA18250AAGC1_GAIN_Minus_5dB-1, Bus_RW);
                            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
                        }
                    }
                }

                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_1__PD_LNA, pObj->sSmoothCurrentStateSave.uPDLNA, Bus_RW);

                /* If this instance is a salve, set the AGC1 Gain back */
                if (err == TM_OK)
                {
                    if ((!pObj->bMasterNotSlave) && (pObj->StandardMode != TDA18250A_StandardMode_Max))
                    {
                        if ((pObj->pStandard->AGC1_GAIN == TDA18250AAGC1_GAIN_Free) || (pObj->pStandard->AGC1_GAIN == TDA18250AAGC1_GAIN_Free_Frozen))
                        {
                            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_3__Force_AGC1_gain, 0x00, Bus_RW);
                            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
                        }
                        else
                        {
                            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_3__Force_AGC1_gain, 0x01, Bus_RW);
                            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));

                            if (err == TM_OK)
                            {
                                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_3__AGC1_Gain, (UInt8)pObj->pStandard->AGC1_GAIN-1, Bus_RW);
                                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
                            }
                        }
                    }
                }

                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Reference_Byte__XTout, pObj->sSmoothCurrentStateSave.uXtOut, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_W_Filter_byte__W_Filter_Enable, pObj->sSmoothCurrentStateSave.uWFilterEnable, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Gpio_ctrl_byte__GPIO2, pObj->sSmoothCurrentStateSave.uGPIO2, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Gpio_ctrl_byte__GPIO1, pObj->sSmoothCurrentStateSave.uGPIO1, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_IR_Mixer_byte_1__DC_Notch, pObj->sSmoothCurrentStateSave.uDCNotch, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_IF_Byte_3__IF_Notch_Enable, pObj->sSmoothCurrentStateSave.uIFNotchEnable, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_IF_Byte_2__IF_HP_Enable, pObj->sSmoothCurrentStateSave.uIFHPEnable, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Power_Down_byte_1__PD_Synthe, pObj->sSmoothCurrentStateSave.uPDSynthe, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Power_Down_byte_1__PD_VCO, pObj->sSmoothCurrentStateSave.uPDVCO, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Power_Down_byte_2__PD_Det3, pObj->sSmoothCurrentStateSave.uPDDet3, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Power_Down_byte_2__PD_Det2, pObj->sSmoothCurrentStateSave.uPDDet2, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Power_Down_byte_2__PD_Det1, pObj->sSmoothCurrentStateSave.uPDDet1, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_2__PSM_VCO, pObj->sSmoothCurrentStateSave.uPSMVCO, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_2__PSM_H3H5, pObj->sSmoothCurrentStateSave.uPSMH3H5, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_3__PSM_DCC, pObj->sSmoothCurrentStateSave.uPSMDCC, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_3__PSM_lpf, pObj->sSmoothCurrentStateSave.uPSMLpf, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_3__PSM_hpf_ifn, pObj->sSmoothCurrentStateSave.uPSMHpfIfn, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_2__AGC1_boost, pObj->sSmoothCurrentStateSave.uAGC1Boost, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_1__PSM_Lodriver, pObj->sSmoothCurrentStateSave.uPSMLodriver, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_1__PSM_Mixer, pObj->sSmoothCurrentStateSave.uPSMMixer, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_1__PSM_Ifpoly, pObj->sSmoothCurrentStateSave.uPSMIfPoly, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_1__PSM_RFpoly, pObj->sSmoothCurrentStateSave.uPSMRFPoly, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Reference_Byte__Dig_Clock, pObj->sSmoothCurrentStateSave.uDigClock, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_1__Force_AGC2_gain, 0x0, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_2__AGC2_loop_off, pObj->sSmoothCurrentStateSave.uAGC2LoopOff, Bus_RW);
            }
            else
            {
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Power_state_byte_2__Power_State_Mode, powerState, Bus_RW);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
                if(err == TM_OK)
                {
                    /* Set digital clock mode to XTAL if normal mode is entered */
                    err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Reference_Byte__Dig_Clock, 0x03, Bus_RW);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
                }
            }
            break;				
        case TDA18250A_PowerStandbyWithXtalOn:
            /* Set digital clock mode to 16 Mhz before entering standby */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Reference_Byte__Dig_Clock, 0x02, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            if(err == TM_OK)
            {
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Power_state_byte_2__Power_State_Mode, powerState, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }
            if(err == TM_OK) /* AGC1 gain init to improve antenna leakage */
            {   /* save LNA_gain range */
                err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_AGC1_byte_2__LNA_Range, &uValue, Bus_None);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
                /* write dummy value 3 */
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_2__LNA_Range, 0x03, Bus_None);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
                /* PD_LT */
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_LT_byte_1__PD_LT, 0x01, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }
            if(err == TM_OK)
            {   /* restore LNA_gain range */
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_2__LNA_Range, uValue, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }
            break;
        case TDA18250A_PowerStandbyWithPllOnWithXtalOn:
            /* Set digital clock mode to 16 Mhz before entering standby */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Reference_Byte__Dig_Clock, 0x03, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            if(err == TM_OK)
            {
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Power_state_byte_2__Power_State_Mode, powerState, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }
            if(err == TM_OK) /* AGC1 gain init to improve antenna leakage */
            {   /* save LNA_gain range */
                err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_AGC1_byte_2__LNA_Range, &uValue, Bus_None);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
                /* write dummy value 3 */
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_2__LNA_Range, 0x03, Bus_None);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));

                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_LT_byte_1__PD_LT, 0x01, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }
            if(err == TM_OK)
            {   /* restore LNA_gain range */
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_2__LNA_Range, uValue, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }
            break;
        case TDA18250A_PowerStandbyWithLtOnWithXtalOn:
            /* Set digital clock mode to 16 Mhz before entering standby */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Reference_Byte__Dig_Clock, 0x02, Bus_RW);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            if(err == TM_OK)
            {
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Power_state_byte_2__Power_State_Mode, powerState, Bus_RW);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }
            if(err == TM_OK)
            {
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_LT_byte_1__PD_LT, 0x00, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }
            break;
        case TDA18250A_PowerStandbyWithLtOnWithPllOnWithXtalOn:
            /* Set digital clock mode to 16 Mhz before entering standby */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Reference_Byte__Dig_Clock, 0x03, Bus_RW);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            if(err == TM_OK)
            {
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Power_state_byte_2__Power_State_Mode, powerState, Bus_RW);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }
            if(err == TM_OK)
            {
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_LT_byte_1__PD_LT, 0x00, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }
            break;
        case TDA18250A_PowerStandbySmoothLTOnXtOutOn:
            /* if ( (pObj->curLLPowerState != TDA18250A_PowerStandbySmoothLTOnXtOutOn) && (pObj->curLLPowerState != TDA18250A_PowerStandbySmoothLTOffXtOutOff) ) */
            if (pObj->curLLPowerState == TDA18250A_PowerNormalMode)
            {
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_AGC2_Gain_status__AGC2_gain_read, &uAGC2, Bus_RW);
                pObj->sSmoothCurrentStateSave.uAGC2 = uAGC2;
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_AGC2_byte_2__AGC2_loop_off, &pObj->sSmoothCurrentStateSave.uAGC2LoopOff, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_2__AGC2_loop_off, 0x01, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_4__AGC2_Gain, uAGC2, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_1__Force_AGC2_gain, 0x1, Bus_RW);
                while (uAGC2 < 45)
                {
                    uAGC2 += 15;
                    if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_4__AGC2_Gain, uAGC2, Bus_RW);
                }
                while (uAGC2 > 75)
                {
                    uAGC2 -= 15;
                    if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_4__AGC2_Gain, uAGC2, Bus_RW);
                }
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_4__AGC2_Gain, 0, Bus_RW);

                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_Reference_Byte__Dig_Clock, &pObj->sSmoothCurrentStateSave.uDigClock, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Reference_Byte__Dig_Clock, 0x02, Bus_RW);

                /* Set all the PSM at minimum */
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_1__PSM_RFpoly, &pObj->sSmoothCurrentStateSave.uPSMRFPoly, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_1__PSM_RFpoly, 0x03, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_1__PSM_Ifpoly, &pObj->sSmoothCurrentStateSave.uPSMIfPoly, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_1__PSM_Ifpoly, 0x03, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_1__PSM_Mixer, &pObj->sSmoothCurrentStateSave.uPSMMixer, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_1__PSM_Mixer, 0x03, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_1__PSM_Lodriver, &pObj->sSmoothCurrentStateSave.uPSMLodriver, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_1__PSM_Lodriver, 0x03, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_2__AGC1_boost, &pObj->sSmoothCurrentStateSave.uAGC1Boost, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_2__AGC1_boost, 0x00, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_3__PSM_hpf_ifn, &pObj->sSmoothCurrentStateSave.uPSMHpfIfn, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_3__PSM_hpf_ifn, 0x03, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_3__PSM_lpf, &pObj->sSmoothCurrentStateSave.uPSMLpf, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_3__PSM_lpf, 0x03, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_3__PSM_DCC, &pObj->sSmoothCurrentStateSave.uPSMDCC, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_3__PSM_DCC, 0x01, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_2__PSM_H3H5, &pObj->sSmoothCurrentStateSave.uPSMH3H5, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_2__PSM_H3H5, 0x03, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_2__PSM_VCO, &pObj->sSmoothCurrentStateSave.uPSMVCO, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_2__PSM_VCO, 0x03, Bus_RW);

                /* Power down */
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_Power_Down_byte_2__PD_Det1, &pObj->sSmoothCurrentStateSave.uPDDet1, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Power_Down_byte_2__PD_Det1, 0x00, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_Power_Down_byte_2__PD_Det2, &pObj->sSmoothCurrentStateSave.uPDDet2, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Power_Down_byte_2__PD_Det2, 0x01, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_Power_Down_byte_2__PD_Det3, &pObj->sSmoothCurrentStateSave.uPDDet3, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Power_Down_byte_2__PD_Det3, 0x01, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_Power_Down_byte_1__PD_VCO, &pObj->sSmoothCurrentStateSave.uPDVCO, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Power_Down_byte_1__PD_VCO, 0x01, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_Power_Down_byte_1__PD_Synthe, &pObj->sSmoothCurrentStateSave.uPDSynthe, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Power_Down_byte_1__PD_Synthe, 0x01, Bus_RW);

                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_IF_Byte_2__IF_HP_Enable, &pObj->sSmoothCurrentStateSave.uIFHPEnable, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_IF_Byte_2__IF_HP_Enable, 0x00, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_IF_Byte_3__IF_Notch_Enable, &pObj->sSmoothCurrentStateSave.uIFNotchEnable, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_IF_Byte_3__IF_Notch_Enable, 0x00, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_IR_Mixer_byte_1__DC_Notch, &pObj->sSmoothCurrentStateSave.uDCNotch, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_IR_Mixer_byte_1__DC_Notch, 0x00, Bus_RW);

                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_Gpio_ctrl_byte__GPIO1, &pObj->sSmoothCurrentStateSave.uGPIO1, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Gpio_ctrl_byte__GPIO1, 0x00, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_Gpio_ctrl_byte__GPIO2, &pObj->sSmoothCurrentStateSave.uGPIO2, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Gpio_ctrl_byte__GPIO2, 0x00, Bus_RW);

                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_W_Filter_byte__W_Filter_Enable, &pObj->sSmoothCurrentStateSave.uWFilterEnable, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_W_Filter_byte__W_Filter_Enable, 0x00, Bus_RW);
                
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_Reference_Byte__XTout, &pObj->sSmoothCurrentStateSave.uXtOut, Bus_RW);
                /* if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Reference_Byte__XTout, 0x00, Bus_RW); */

                /* If this instance is a salve set the AGC1 Gain to -5dB before changing the LNA */
                if (err == TM_OK)
                {
                    if (!pObj->bMasterNotSlave)
                    {
                        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_3__Force_AGC1_gain, 0x01, Bus_RW);
                        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
                        
                        if (err == TM_OK)
                        {
                            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_3__AGC1_Gain, (UInt8)TDA18250AAGC1_GAIN_Minus_5dB-1, Bus_RW);
                            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
                        }
                    }
                }
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_AGC1_byte_1__PD_LNA, &pObj->sSmoothCurrentStateSave.uPDLNA, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_1__PD_LNA, 0x00, Bus_RW);

                /* If this instance is a salve, set the AGC1 Gain back */
                /*
                if (err == TM_OK)
                {
                    if ((!pObj->bMasterNotSlave) && (pObj->StandardMode != TDA18250A_StandardMode_Max))
                    {
                        if ((pObj->pStandard->AGC1_GAIN == TDA18250AAGC1_GAIN_Free) || (pObj->pStandard->AGC1_GAIN == TDA18250AAGC1_GAIN_Free_Frozen))
                        {
                            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_3__Force_AGC1_gain, 0x00, Bus_RW);
                            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
                        }
                        else
                        {
                            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_3__Force_AGC1_gain, 0x01, Bus_RW);
                            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));

                            if (err == TM_OK)
                            {
                                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_3__AGC1_Gain, (UInt8)pObj->pStandard->AGC1_GAIN-1, Bus_RW);
                                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
                            }
                        }
                    }
                }
                */

                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_Power_Down_byte_2__PD_S2B_IF, &pObj->sSmoothCurrentStateSave.uPDS2BIF, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Power_Down_byte_2__PD_S2B_IF, 0x00, Bus_RW);

                /* 1 =>  xt        */
                /* 2 => + xt + lt  */
                /* 3 => xt + pll   */
                /* 4 xt + lt + pll */
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Power_state_byte_2__Power_State_Mode, 2, Bus_RW);
                
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_LT_byte_1__PD_LT, &pObj->sSmoothCurrentStateSave.uPDLT, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_LT_byte_1__PD_LT, 0x00, Bus_RW);
            }
            else
            {
                powerState = pObj->curLLPowerState;
            }
            break;
        case TDA18250A_PowerStandbySmoothLTOffXtOutOff:
            /* if ( (pObj->curLLPowerState != TDA18250A_PowerStandbySmoothLTOnXtOutOn) && (pObj->curLLPowerState != TDA18250A_PowerStandbySmoothLTOffXtOutOff) ) */
            if (pObj->curLLPowerState == TDA18250A_PowerNormalMode)
            {
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_AGC2_Gain_status__AGC2_gain_read, &uAGC2, Bus_RW);
                pObj->sSmoothCurrentStateSave.uAGC2 = uAGC2;
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_AGC2_byte_2__AGC2_loop_off, &pObj->sSmoothCurrentStateSave.uAGC2LoopOff, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_2__AGC2_loop_off, 0x01, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_4__AGC2_Gain, uAGC2, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_1__Force_AGC2_gain, 0x1, Bus_RW);
                while (uAGC2 < 45)
                {
                    uAGC2 += 15;
                    if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_4__AGC2_Gain, uAGC2, Bus_RW);
                }
                while (uAGC2 > 75)
                {
                    uAGC2 -= 15;
                    if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_4__AGC2_Gain, uAGC2, Bus_RW);
                }
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_4__AGC2_Gain, 0, Bus_RW);

                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_Reference_Byte__Dig_Clock, &pObj->sSmoothCurrentStateSave.uDigClock, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Reference_Byte__Dig_Clock, 0x02, Bus_RW);

                /* Set all the PSM at minimum */
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_1__PSM_RFpoly, &pObj->sSmoothCurrentStateSave.uPSMRFPoly, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_1__PSM_RFpoly, 0x03, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_1__PSM_Ifpoly, &pObj->sSmoothCurrentStateSave.uPSMIfPoly, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_1__PSM_Ifpoly, 0x03, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_1__PSM_Mixer, &pObj->sSmoothCurrentStateSave.uPSMMixer, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_1__PSM_Mixer, 0x03, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_1__PSM_Lodriver, &pObj->sSmoothCurrentStateSave.uPSMLodriver, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_1__PSM_Lodriver, 0x03, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_2__AGC1_boost, &pObj->sSmoothCurrentStateSave.uAGC1Boost, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_2__AGC1_boost, 0x00, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_3__PSM_hpf_ifn, &pObj->sSmoothCurrentStateSave.uPSMHpfIfn, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_3__PSM_hpf_ifn, 0x03, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_3__PSM_lpf, &pObj->sSmoothCurrentStateSave.uPSMLpf, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_3__PSM_lpf, 0x03, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_3__PSM_DCC, &pObj->sSmoothCurrentStateSave.uPSMDCC, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_3__PSM_DCC, 0x01, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_2__PSM_H3H5, &pObj->sSmoothCurrentStateSave.uPSMH3H5, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_2__PSM_H3H5, 0x03, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_2__PSM_VCO, &pObj->sSmoothCurrentStateSave.uPSMVCO, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_2__PSM_VCO, 0x03, Bus_RW);

                /* Power down */
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_Power_Down_byte_2__PD_Det1, &pObj->sSmoothCurrentStateSave.uPDDet1, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Power_Down_byte_2__PD_Det1, 0x01, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_Power_Down_byte_2__PD_Det2, &pObj->sSmoothCurrentStateSave.uPDDet2, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Power_Down_byte_2__PD_Det2, 0x01, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_Power_Down_byte_2__PD_Det3, &pObj->sSmoothCurrentStateSave.uPDDet3, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Power_Down_byte_2__PD_Det3, 0x01, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_Power_Down_byte_1__PD_VCO, &pObj->sSmoothCurrentStateSave.uPDVCO, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Power_Down_byte_1__PD_VCO, 0x01, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_Power_Down_byte_1__PD_Synthe, &pObj->sSmoothCurrentStateSave.uPDSynthe, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Power_Down_byte_1__PD_Synthe, 0x01, Bus_RW);

                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_IF_Byte_2__IF_HP_Enable, &pObj->sSmoothCurrentStateSave.uIFHPEnable, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_IF_Byte_2__IF_HP_Enable, 0x00, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_IF_Byte_3__IF_Notch_Enable, &pObj->sSmoothCurrentStateSave.uIFNotchEnable, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_IF_Byte_3__IF_Notch_Enable, 0x00, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_IR_Mixer_byte_1__DC_Notch, &pObj->sSmoothCurrentStateSave.uDCNotch, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_IR_Mixer_byte_1__DC_Notch, 0x00, Bus_RW);

                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_Gpio_ctrl_byte__GPIO1, &pObj->sSmoothCurrentStateSave.uGPIO1, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Gpio_ctrl_byte__GPIO1, 0x00, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_Gpio_ctrl_byte__GPIO2, &pObj->sSmoothCurrentStateSave.uGPIO2, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Gpio_ctrl_byte__GPIO2, 0x00, Bus_RW);

                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_W_Filter_byte__W_Filter_Enable, &pObj->sSmoothCurrentStateSave.uWFilterEnable, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_W_Filter_byte__W_Filter_Enable, 0x00, Bus_RW);
                
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_Reference_Byte__XTout, &pObj->sSmoothCurrentStateSave.uXtOut, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Reference_Byte__XTout, 0x00, Bus_RW);

                /* If this instance is a salve set the AGC1 Gain to -5dB before changing the LNA */
                if (err == TM_OK)
                {
                    if (!pObj->bMasterNotSlave)
                    {
                        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_3__Force_AGC1_gain, 0x01, Bus_RW);
                        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
                        
                        if (err == TM_OK)
                        {
                            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_3__AGC1_Gain, (UInt8)TDA18250AAGC1_GAIN_Minus_5dB-1, Bus_RW);
                            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
                        }
                    }
                }
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_AGC1_byte_1__PD_LNA, &pObj->sSmoothCurrentStateSave.uPDLNA, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_1__PD_LNA, 0x01, Bus_RW);

                /* If this instance is a salve, set the AGC1 Gain back */
                /*
                if (err == TM_OK)
                {
                    if ((!pObj->bMasterNotSlave) && (pObj->StandardMode != TDA18250A_StandardMode_Max))
                    {
                        if ((pObj->pStandard->AGC1_GAIN == TDA18250AAGC1_GAIN_Free) || (pObj->pStandard->AGC1_GAIN == TDA18250AAGC1_GAIN_Free_Frozen))
                        {
                            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_3__Force_AGC1_gain, 0x00, Bus_RW);
                            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
                        }
                        else
                        {
                            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_3__Force_AGC1_gain, 0x01, Bus_RW);
                            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));

                            if (err == TM_OK)
                            {
                                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_3__AGC1_Gain, (UInt8)pObj->pStandard->AGC1_GAIN-1, Bus_RW);
                                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
                            }
                        }
                    }
                }
                */

                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_Power_Down_byte_2__PD_S2B_IF, &pObj->sSmoothCurrentStateSave.uPDS2BIF, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Power_Down_byte_2__PD_S2B_IF, 0x00, Bus_RW);

                /* 1 =>  xt        */
                /* 2 => + xt + lt  */
                /* 3 => xt + pll   */
                /* 4 xt + lt + pll */
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Power_state_byte_2__Power_State_Mode, 1, Bus_RW);
                
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_LT_byte_1__PD_LT, &pObj->sSmoothCurrentStateSave.uPDLT, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_LT_byte_1__PD_LT, 0x01, Bus_RW);
            }
            else
            {
                powerState = pObj->curLLPowerState;
            }
            break;
        case TDA18250A_PowerStandbySmoothLTOffLNAOnXtOutOn:
            /* if ( (pObj->curLLPowerState != TDA18250A_PowerStandbySmoothLTOnXtOutOn) && (pObj->curLLPowerState != TDA18250A_PowerStandbySmoothLTOffXtOutOff) ) */
            if (pObj->curLLPowerState == TDA18250A_PowerNormalMode)
            {
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_AGC2_Gain_status__AGC2_gain_read, &uAGC2, Bus_RW);
                pObj->sSmoothCurrentStateSave.uAGC2 = uAGC2;
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_AGC2_byte_2__AGC2_loop_off, &pObj->sSmoothCurrentStateSave.uAGC2LoopOff, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_2__AGC2_loop_off, 0x01, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_4__AGC2_Gain, uAGC2, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_1__Force_AGC2_gain, 0x1, Bus_RW);
                while (uAGC2 < 45)
                {
                    uAGC2 += 15;
                    if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_4__AGC2_Gain, uAGC2, Bus_RW);
                }
                while (uAGC2 > 75)
                {
                    uAGC2 -= 15;
                    if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_4__AGC2_Gain, uAGC2, Bus_RW);
                }
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_4__AGC2_Gain, 0, Bus_RW);

                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_Reference_Byte__Dig_Clock, &pObj->sSmoothCurrentStateSave.uDigClock, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Reference_Byte__Dig_Clock, 0x02, Bus_RW);

                /* Set all the PSM at minimum */
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_1__PSM_RFpoly, &pObj->sSmoothCurrentStateSave.uPSMRFPoly, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_1__PSM_RFpoly, 0x03, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_1__PSM_Ifpoly, &pObj->sSmoothCurrentStateSave.uPSMIfPoly, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_1__PSM_Ifpoly, 0x03, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_1__PSM_Mixer, &pObj->sSmoothCurrentStateSave.uPSMMixer, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_1__PSM_Mixer, 0x03, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_1__PSM_Lodriver, &pObj->sSmoothCurrentStateSave.uPSMLodriver, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_1__PSM_Lodriver, 0x03, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_2__AGC1_boost, &pObj->sSmoothCurrentStateSave.uAGC1Boost, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_2__AGC1_boost, 0x00, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_3__PSM_hpf_ifn, &pObj->sSmoothCurrentStateSave.uPSMHpfIfn, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_3__PSM_hpf_ifn, 0x03, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_3__PSM_lpf, &pObj->sSmoothCurrentStateSave.uPSMLpf, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_3__PSM_lpf, 0x03, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_3__PSM_DCC, &pObj->sSmoothCurrentStateSave.uPSMDCC, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_3__PSM_DCC, 0x01, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_2__PSM_H3H5, &pObj->sSmoothCurrentStateSave.uPSMH3H5, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_2__PSM_H3H5, 0x03, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_2__PSM_VCO, &pObj->sSmoothCurrentStateSave.uPSMVCO, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_2__PSM_VCO, 0x03, Bus_RW);

                /* Power down */
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_Power_Down_byte_2__PD_Det1, &pObj->sSmoothCurrentStateSave.uPDDet1, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Power_Down_byte_2__PD_Det1, 0x00, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_Power_Down_byte_2__PD_Det2, &pObj->sSmoothCurrentStateSave.uPDDet2, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Power_Down_byte_2__PD_Det2, 0x01, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_Power_Down_byte_2__PD_Det3, &pObj->sSmoothCurrentStateSave.uPDDet3, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Power_Down_byte_2__PD_Det3, 0x01, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_Power_Down_byte_1__PD_VCO, &pObj->sSmoothCurrentStateSave.uPDVCO, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Power_Down_byte_1__PD_VCO, 0x01, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_Power_Down_byte_1__PD_Synthe, &pObj->sSmoothCurrentStateSave.uPDSynthe, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Power_Down_byte_1__PD_Synthe, 0x01, Bus_RW);

                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_IF_Byte_2__IF_HP_Enable, &pObj->sSmoothCurrentStateSave.uIFHPEnable, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_IF_Byte_2__IF_HP_Enable, 0x00, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_IF_Byte_3__IF_Notch_Enable, &pObj->sSmoothCurrentStateSave.uIFNotchEnable, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_IF_Byte_3__IF_Notch_Enable, 0x00, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_IR_Mixer_byte_1__DC_Notch, &pObj->sSmoothCurrentStateSave.uDCNotch, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_IR_Mixer_byte_1__DC_Notch, 0x00, Bus_RW);

                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_Gpio_ctrl_byte__GPIO1, &pObj->sSmoothCurrentStateSave.uGPIO1, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Gpio_ctrl_byte__GPIO1, 0x00, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_Gpio_ctrl_byte__GPIO2, &pObj->sSmoothCurrentStateSave.uGPIO2, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Gpio_ctrl_byte__GPIO2, 0x00, Bus_RW);

                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_W_Filter_byte__W_Filter_Enable, &pObj->sSmoothCurrentStateSave.uWFilterEnable, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_W_Filter_byte__W_Filter_Enable, 0x00, Bus_RW);
                
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_Reference_Byte__XTout, &pObj->sSmoothCurrentStateSave.uXtOut, Bus_RW);
                /* if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Reference_Byte__XTout, 0x00, Bus_RW); */

                /* If this instance is a salve set the AGC1 Gain to -5dB before changing the LNA */
                if (err == TM_OK)
                {
                    if (!pObj->bMasterNotSlave)
                    {
                        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_3__Force_AGC1_gain, 0x01, Bus_RW);
                        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
                        
                        if (err == TM_OK)
                        {
                            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_3__AGC1_Gain, (UInt8)TDA18250AAGC1_GAIN_Minus_5dB-1, Bus_RW);
                            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
                        }
                    }
                }
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_AGC1_byte_1__PD_LNA, &pObj->sSmoothCurrentStateSave.uPDLNA, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_1__PD_LNA, 0x00, Bus_RW);

                /* If this instance is a salve, set the AGC1 Gain back */
                /*
                if (err == TM_OK)
                {
                    if ((!pObj->bMasterNotSlave) && (pObj->StandardMode != TDA18250A_StandardMode_Max))
                    {
                        if ((pObj->pStandard->AGC1_GAIN == TDA18250AAGC1_GAIN_Free) || (pObj->pStandard->AGC1_GAIN == TDA18250AAGC1_GAIN_Free_Frozen))
                        {
                            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_3__Force_AGC1_gain, 0x00, Bus_RW);
                            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
                        }
                        else
                        {
                            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_3__Force_AGC1_gain, 0x01, Bus_RW);
                            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));

                            if (err == TM_OK)
                            {
                                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_3__AGC1_Gain, (UInt8)pObj->pStandard->AGC1_GAIN-1, Bus_RW);
                                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
                            }
                        }
                    }
                }
                */

                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_Power_Down_byte_2__PD_S2B_IF, &pObj->sSmoothCurrentStateSave.uPDS2BIF, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Power_Down_byte_2__PD_S2B_IF, 0x00, Bus_RW);

                /* 1 =>  xt        */
                /* 2 => + xt + lt  */
                /* 3 => xt + pll   */
                /* 4 xt + lt + pll */
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Power_state_byte_2__Power_State_Mode, 2, Bus_RW);
                
                if (err == TM_OK) err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_LT_byte_1__PD_LT, &pObj->sSmoothCurrentStateSave.uPDLT, Bus_RW);
                if (err == TM_OK) err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_LT_byte_1__PD_LT, 0x01, Bus_RW);
            }
            else
            {
                powerState = pObj->curLLPowerState;
            }
            break;
        default:
            err = TM_ERR_BAD_PARAMETER;
            break;
        }

        if(err == TM_OK)
        {
            /* Store low-level power state in driver instance */
            pObj->curLLPowerState = powerState;
        }
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    iTDA18250A_SetRF:                                              */
/*                                                                            */
/* DESCRIPTION: Tunes to a RF.                                                */
/*                                                                            */
/* RETURN:      TM_OK                                                         */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18250A_SetRF(
                 pTDA18250AObject_t   pObj    /* I: Driver object */
                 )
{
    tmErrorCode_t   err = TM_OK;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18250A_SetRF(0x%08X)", pObj->tUnitW);

    if(pObj->curPowerState != tmPowerOn)
    {
        /* Set power state ON */
        err = iTDA18250A_SetLLPowerState(pObj, TDA18250A_PowerNormalMode);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_SetLLPowerState(0x%08X, PowerNormalMode) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        pObj->curPowerState = tmPowerOn;

        /* Set RF frequency */
        err = iTDA18250A_SetRF_Freq(pObj, pObj->uProgRF);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_SetRF_Freq(0x%08X) failed.", pObj->tUnitW));
    }

    /* Bypass ROM settings for wireless filters */
    if(err == TM_OK)
    {
        err = iTDA18250A_OverrideWireless(pObj);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_OverrideWireless(0x%08X) failed.", pObj->tUnitW));
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18250A_SetRF_Freq                                          */
/*                                                                            */
/* DESCRIPTION: Sets Tuner Frequency registers.                               */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18250A_SetRF_Freq(
                      pTDA18250AObject_t   pObj,   /* I: Driver object */
                      UInt32              uRF     /* I: Wanted frequency */
                      )
{
    tmErrorCode_t   err = TM_OK;
    UInt32          uRFLocal = 0;
    UInt32 uTimeOut;
    UInt32 uWaitStep = 10;
    UInt32 uCurrentTime = 0;
    UInt8 uValue = 0x00;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18250A_SetRF_Freq(0x%08X)", pObj->tUnitW);

    /* Pre lock settings */
    if (pObj->eChipVersion == TDA18250A_VersionES3)
    {
        if(err == TM_OK)
        {
            /* AGCK Time Constant */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGCK_byte_1__AGCK_Mode, (UInt8)TDA18250A_AGCK_Time_Constant_0_512ms, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* AGC2_Gain_Control_En */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_3__AGC2_Gain_Control_En, 0x00, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
    }

    /* Set RF */
    if(err == TM_OK)
    {
        uRFLocal = (uRF + 500) / 1000;

        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_RF_Frequency_byte_1__RF_Freq_1, (UInt8)((uRFLocal & 0x00FF0000) >> 16), Bus_None);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_RF_Frequency_byte_2__RF_Freq_2, (UInt8)((uRFLocal & 0x0000FF00) >> 8), Bus_None);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_RF_Frequency_byte_3__RF_Freq_3, (UInt8)(uRFLocal & 0x000000FF), Bus_None);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18250A_WriteRegMap(pObj, gTDA18250A_Reg_RF_Frequency_byte_1.Address, TDA18250A_REG_DATA_LEN(gTDA18250A_Reg_RF_Frequency_byte_1, gTDA18250A_Reg_RF_Frequency_byte_3));
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_WriteRegMap(0x%08X) failed.", pObj->tUnitW));
    }
    if (err == TM_OK)
    {
        err = iTDA18250A_SetNDivRDiv(pObj);
    }
    if(err == TM_OK)
    {
        /* Set IRQ_clear */
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_IRQ_clear, TDA18250A_IRQ_Global|TDA18250A_IRQ_SetRF, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)  /* <> value max ? */
    {
        /* Set ICP to Max*/
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Charge_pump_byte__ICP, 0x00, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK )
    {
        if  (pObj->bOverridePLL) /* or standard dependant */
        {
            /* Override the calculated PLL to get the best margin in case fine tuning is used */
            /* which means set the PLL in manual mode that provides the best occurence of LO tuning (+-2 MHz) */
            /* without touching PostDiv and Prescaler */

            if(err == TM_OK)
            {
                err = iTDA18250A_SetPLL(pObj);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_SetPLL failed.", pObj->tUnitW));
            }
            if(err == TM_OK)
            {
                err = iTDA18250A_Wait(pObj, 0x03); /* PLL convergence */
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_SetMSM(0x%08X, TDA18250A_MSM_SetRF) failed.", pObj->tUnitW));
            }
        }
        else
        {
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Sigma_delta_byte_5__N_K_correct_manual, 0x00, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Sigma_delta_byte_5__LO_Calc_Disable, 0x00, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));			
            if(err == TM_OK)
            {
                err = iTDA18250A_SetMSM(pObj, TDA18250A_MSM_SetRF, True);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_SetMSM(0x%08X, TDA18250A_MSM_SetRF) failed.", pObj->tUnitW));
            }
            if(err == TM_OK)
            {
                /* Wait for IRQ to trigger */
                err = iTDA18250A_WaitIRQ(pObj, 500, 10, TDA18250A_IRQ_SetRF);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_WaitIRQ(0x%08X, 50, 5, TDA18250A_IRQ_SetRF) failed.", pObj->tUnitW));
            }
        }
    }
    if(err == TM_OK)
    {
        err = iTDA18250A_Wait(pObj, 0x03);  /* ensure PLL convergence with ICP max*/
    }

    if (pObj->eChipVersion == TDA18250A_VersionES3)
    {
        if (pObj->eScanningMode == TDA18250A_Scanning_Mode_Disabled)
        {
            /* Post lock settings */
            if(err == TM_OK)
            {
                /* Fast_Auto_Launch */
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_MSM_byte_2__Fast_Auto_Launch, 0x01, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }
            if (err == TM_OK)
            {
                err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_AGC_fast_auto_byte_1__AGC_timer_mod_fast_auto, &uValue, Bus_RW);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Read(0x%08X) failed.", pObj->tUnitW));
                
                if (err == TM_OK)
                {
                    switch (uValue)
                    {
                    case 0x00:
                        uTimeOut = 2 * 75;
                        break;
                    case 0x01:
                        uTimeOut = 2 * 150;
                        break;
                    case 0x02:
                        uTimeOut = 2 * 225;
                        break;
                    case 0x03:
                    default:
                        uTimeOut = 2 * 300;
                        break;
                    }
                       
                    uCurrentTime = 0;

                    do
                    {
                        err = iTDA18250A_Wait(pObj, uWaitStep);
                        if (err == TM_OK)
                        {
                            uCurrentTime += uWaitStep;

                            err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_Power_state_byte_1__AGC_Lock, &uValue, Bus_RW);
                            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Read(0x%08X) failed.", pObj->tUnitW));
                        }
                    } while ((err == TM_OK) && (uCurrentTime < uTimeOut) && (uValue == 0x00));
                }
            }
        }
        if(err == TM_OK)
        {
            /* AGCK Time Constant */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGCK_byte_1__AGCK_Mode, (UInt8)pObj->pStandard->AGCK_Time_Constant, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* AGC2_Gain_Control_En */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_3__AGC2_Gain_Control_En, (UInt8)pObj->pStandard->AGC2_GAIN_CONTROL_EN_ES3, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
		if(err == TM_OK)
        {
        	/* Force Continuous FAST mode*/
			err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_2__Fast_Mode_AGC,0x01, Bus_NoRead);
		}
    }

    if(err == TM_OK)
    {
        /* Override ICP */
        err = iTDA18250A_OverrideICP(pObj, pObj->uProgRF);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_OverrideICP(0x%08X) failed.", pObj->tUnitW));
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18250A_OverrideICP                                         */
/*                                                                            */
/* DESCRIPTION: Overrides ICP.                                                */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
static tmErrorCode_t
iTDA18250A_OverrideICP(
                       pTDA18250AObject_t   pObj,   /* I: Driver object */
                       UInt32              uRF     /* I: Wanted frequency */
                       )
{
    tmErrorCode_t   err = TM_OK;
    UInt32          uIF = 0;
    UInt8           ProgIF = 0;
    UInt8           LOPostdiv = 0;
    UInt8           LOPrescaler = 0;
    UInt32          FVCO = 0;
    UInt8           ICP = 0;
    UInt8           uPrevICP = 0;
    UInt32          uXtalFreqId = pObj->eXtalFreq-1;
    UInt32          i;
    UInt8           uRDiv;
    UInt8           uNDiv;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18250A_OverrideICP(0x%08X)", pObj->tUnitW);

    /*
    if fvco<6552 MHz ==> icp = 150? (register = 01b)
    if fvco<6860 MHz ==> icp = 300? (register = 10b)
    500 elsewhere (register 00b)

    Reminder : fvco = postdiv*presc*(RFfreq+IFfreq) 
    */

    /* Read PostDiv et Prescaler */
    err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_Main_Post_Divider_byte, &LOPostdiv, Bus_RW);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Read(0x%08X) failed.", pObj->tUnitW));

    if(err == TM_OK)
    {
        /* PostDiv */
        err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_Main_Post_Divider_byte__LOPostDiv, &LOPostdiv, Bus_RW);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Read(0x%08X) failed.", pObj->tUnitW));

        if(err == TM_OK)
        {
            /* Prescaler */
            err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_Main_Post_Divider_byte__LOPresc, &LOPrescaler, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Read(0x%08X) failed.", pObj->tUnitW));
        }

        if(err == TM_OK)
        {
            /* IF */
            err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_IF_Frequency_byte__IF_Freq, &ProgIF, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Read(0x%08X) failed.", pObj->tUnitW));
        }

        if(err == TM_OK)
        {
            /* Decode IF */
            uIF = ProgIF*50000;

            /* Decode PostDiv */
            switch(LOPostdiv)
            {
            case 1:
                LOPostdiv = 1;
                break;
            case 2:
                LOPostdiv = 2;
                break;
            case 3:
                LOPostdiv = 4;
                break;
            case 4:
                LOPostdiv = 8;
                break;
            case 5:
                LOPostdiv = 16;
                break;
            default:
                err = TDA18250A_ERR_BAD_PARAMETER;
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_OverrideICP(0x%08X) LOPostDiv value is wrong.", pObj->tUnitW));
                break;
            }
        }
        if(err == TM_OK)
        {
            /* Calculate FVCO in KHz*/
            FVCO = LOPostdiv * LOPrescaler * ((uRF + uIF) / 1000);

            /* Set correct ICP */

            
            for (i = 0 ; i < RDivNDivTable[uXtalFreqId].uNbMode ; i++)
            {
                if (FVCO >= RDivNDivTable[uXtalFreqId].psVCOModes[i].uVCOFreq)
                {
                    ICP = RDivNDivTable[uXtalFreqId].psVCOModes[i].uICP;
                    uRDiv = RDivNDivTable[uXtalFreqId].psVCOModes[i].uRDiv;
                    uNDiv = RDivNDivTable[uXtalFreqId].psVCOModes[i].uNDiv;
                }
            }

            if(err == TM_OK)
            {
                /* Get ICP */
                err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_Charge_pump_byte__ICP, &uPrevICP, Bus_None);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Read(0x%08X) failed.", pObj->tUnitW));
            }

            if (err == TM_OK)
            {
                if(uPrevICP != ICP )
                {
                    /* Set ICP new value */
                    err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Charge_pump_byte__ICP, ICP, Bus_NoRead);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
                }
            }
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18250A_OverrideWireless                                    */
/*                                                                            */
/* DESCRIPTION: Overrides Wireless settings.                                  */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
static tmErrorCode_t
iTDA18250A_OverrideWireless(
                            pTDA18250AObject_t   pObj    /* I: Driver object */
                            )
{
    tmErrorCode_t   err = TM_OK;
    UInt8           uPrevW_Filter_byte = 0;
    UInt8           uW_Filter_byte = 0;
    UInt8           W_Filter = 0;
    UInt8           W_Filter_Bypass = 0;
    UInt8           W_Filter_Offset = 0;
    UInt8           W_Filter_Enable = 0;
    UInt8           uWF = 0;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18250A_OverrideWireless(0x%08X)", pObj->tUnitW);

    /* TODO : update the comment */
    /* Bypass ROM for wireless filters */
    /* WF7 = 1.7GHz - 1.98GHz */
    /* WF8 = 1.98GHz - 2.1GHz */
    /* WF9 = 2.1GHz - 2.4GHz */
    /* For all frequencies requiring WF7 and WF8, add -8% shift */
    /* For all frequencies requiring WF9, change to WF8 and add +4% shift */

    /* Firstly check which WF we are in : */
    if ( /* WF7 */
        ((pObj->uProgRF >= 335360000) && (pObj->uProgRF < 390656000)) ||
        ((pObj->uProgRF >= 562688000) && (pObj->uProgRF < 654848000))
       )
    {
        uWF = 7;
    }
    else if ( /* WF8 */
             ((pObj->uProgRF >= 390656000) && (pObj->uProgRF < 430592000)) ||
             ((pObj->uProgRF >= 654848000) && (pObj->uProgRF < 720384000))
            )
    {
        uWF = 8;
    }
    else if ( /* WF9 */
             ((pObj->uProgRF >= 473600000) && (pObj->uProgRF < 535040000)) ||
             ((pObj->uProgRF >= 793088000) && (pObj->uProgRF < 865792000))
            )
    {
        uWF = 9;
    }
    else
    {
        uWF = 0;
    }

    /* Let ROM do the job */
    
    
    /* Set the correct offset and filter */
    switch (uWF)
    {
    case 7:
        W_Filter_Bypass = 0x01;
        W_Filter_Offset = 0x01;
        W_Filter = 0x03;
        W_Filter_Enable = 1;
        break;
    case 8:
        W_Filter_Bypass = 0x01;
        W_Filter_Offset = 0x02;
        W_Filter = 0x02;
        W_Filter_Enable = 1;
        break;
    case 9:
        W_Filter_Bypass = 0x01;
        W_Filter_Offset = 0x01;
        W_Filter = 0x01;
        W_Filter_Enable = 1;
        break;
    default:
        W_Filter_Bypass = 0x00;
        W_Filter_Offset = 0x00;
        W_Filter = 0x01;
        if ((pObj->uProgRF >= 289280000) && (pObj->uProgRF < 320000000)) 
        {
            W_Filter_Enable = 1;
        }
        else
        {
            W_Filter_Enable = 0;
        }
        break;
    }

    /* Read current W_Filter_byte */
    err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_W_Filter_byte, &uPrevW_Filter_byte, Bus_RW);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Read(0x%08X) failed.", pObj->tUnitW));

    if(err == TM_OK)
    {
        /* Set Wireless Filter Bypass */
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_W_Filter_byte__W_Filter_Bypass, W_Filter_Bypass, Bus_None);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));

        if(err == TM_OK)
        {
            /* Set Wireless Filter */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_W_Filter_byte__W_Filter, W_Filter, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }

        if(err == TM_OK)
        {
            /* Set Wireless Filter Offset */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_W_Filter_byte__W_Filter_Offset, W_Filter_Offset, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }

        if(err == TM_OK)
        {
            /* Set Wireless filters ON */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_W_Filter_byte__W_Filter_Enable, W_Filter_Enable, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }

        /* Actual write */
        if(err == TM_OK)
        {
            /* Read above-modified W_Filter_byte */
            err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_W_Filter_byte, &uW_Filter_byte, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Read(0x%08X) failed.", pObj->tUnitW));
        }

        if(err == TM_OK && uPrevW_Filter_byte != uW_Filter_byte)
        {
            /* W_Filter_byte changed: Update it */
            err = iTDA18250A_WriteRegMap(pObj, gTDA18250A_Reg_W_Filter_byte.Address, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_WriteRegMap(0x%08X) failed.", pObj->tUnitW));
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18250A_SetPLL                                             */
/*                                                                            */
/* DESCRIPTION: Set the PLL in manual mode                                    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
static tmErrorCode_t
iTDA18250A_SetPLL(
                  pTDA18250AObject_t   pObj    /* I: Driver object */
                  )
{
    tmErrorCode_t err = TM_OK;

    /* LO wanted = RF wanted + IF in KHz */
    UInt32 LO = 0;
    UInt8 uProgIF = 0;

    /* Algorithm that calculates PostDiv */
    UInt8 PostDiv = 0; /* absolute value */
    UInt8 LOPostDiv = 0; /* register value */

    /* Algorithm that calculates Prescaler */
    UInt8 Prescaler = 0;

    /* Algorithm that calculates N, K */
    UInt32 N_int = 0;
    UInt32 K_int = 0;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18250A_SetPLL(0x%08X)", pObj->tUnitW);

    /* Calculate wanted LO = RF + IF in Hz */
    err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_IF_Frequency_byte__IF_Freq, &uProgIF, Bus_None);
    LO = (pObj->uProgRF /1000) + (uProgIF*50);

    /* Calculate the best PostDiv and Prescaler : the ones that provide the best margin */
    /* in case of fine tuning +-2 MHz */
    err = iTDA18250A_FindPostDivAndPrescalerWithBetterMargin(LO, &PostDiv, &Prescaler);

    if (err == TM_OK)
    {
        /* Program the PLL only if valid values are found, in that case err == TM_OK */

        /* Decode PostDiv */
        switch(PostDiv)
        {
        case 1:
            LOPostDiv = 1;
            break;
        case 2:
            LOPostDiv = 2;
            break;
        case 4:
            LOPostDiv = 3;
            break;
        case 8:
            LOPostDiv = 4;
            break;
        case 16:
            LOPostDiv = 5;
            break;
        default:
            err = TDA18250A_ERR_BAD_PARAMETER;
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_SetPLL(0x%08X) *PostDiv value is wrong.", pObj->tUnitW));
            break;
        }

        /* Affect register map without writing into IC */
        if(err == TM_OK)
        {
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Main_Post_Divider_byte__LOPostDiv, LOPostDiv, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }

        if(err == TM_OK)
        {
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Main_Post_Divider_byte__LOPresc, Prescaler, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }

        if(err == TM_OK)
        {
            /* Calculate N & K values of the PLL */
            err = iTDA18250A_CalculateNIntKInt(LO, PostDiv, Prescaler, &N_int, &K_int, pObj->eXtalFreq-1);

            /* Affect registers map without writing to IC */
            if(err == TM_OK)
            {
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Sigma_delta_byte_4__LO_Frac_0, (UInt8)(K_int & 0xFF), Bus_None);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }

            if(err == TM_OK)
            {
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Sigma_delta_byte_3__LO_Frac_1, (UInt8)((K_int >> 8) & 0xFF), Bus_None);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }

            if(err == TM_OK)
            {
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Sigma_delta_byte_2__LO_Frac_2, (UInt8)((K_int >> 16) & 0xFF), Bus_None);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }

            if(err == TM_OK)
            {
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Sigma_delta_byte_1__LO_Int, (UInt8)(N_int & 0xFF), Bus_None);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }

            if(err == TM_OK)
            {
                /* Force manual selection mode : 0x3 at @0x56 */
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Sigma_delta_byte_5__N_K_correct_manual, 0x01, Bus_None);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));

                if(err == TM_OK)
                {
                    /* Force manual selection mode : 0x3 at @0x56 */
                    err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Sigma_delta_byte_5__LO_Calc_Disable, 0x01, Bus_None);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
                }
            }

            if(err == TM_OK)
            {
                /* Write bytes Main_Post_Divider_byte (0x51) to Sigma_delta_byte_5 (0x56) */
                err = iTDA18250A_WriteRegMap(pObj, gTDA18250A_Reg_Main_Post_Divider_byte.Address, TDA18250A_REG_DATA_LEN(gTDA18250A_Reg_Main_Post_Divider_byte, gTDA18250A_Reg_Sigma_delta_byte_5));
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18250A_CalculatePostDivAndPrescaler                        */
/*                                                                            */
/* DESCRIPTION: Calculate PostDiv and Prescaler by starting from lowest value */
/*              of LO or not                                                  */
/*              LO must be passed in Hz                                       */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
static tmErrorCode_t
iTDA18250A_FindPostDivAndPrescalerWithBetterMargin(
    UInt32 LO,          /* In kHz */
    UInt8* PostDiv,     /* Directly the value to set in the register  */
    UInt8* Prescaler    /* Directly the value to set in the register  */
    )
{
    /* Initialize to error in case no valuable values are found */
    tmErrorCode_t err = TM_OK;

    UInt8 PostDivGrowing;
    UInt8 PrescalerGrowing;
    UInt8 PostDivDecreasing;
    UInt8 PrescalerDecreasing;
    UInt32 FCVOGrowing = 0;
    UInt32 DistanceFCVOGrowing = 0;
    UInt32 FVCODecreasing = 0;
    UInt32 DistanceFVCODecreasing = 0;

    /* Get the 2 possible values for PostDiv & Prescaler to find the one
    which provides the better margin on LO */
    err = iTDA18250A_CalculatePostDivAndPrescalerBetterMargin(LO, True, &PostDivGrowing, &PrescalerGrowing);
    if (err == TM_OK)
    {
        /* Calculate corresponding FVCO value in kHz */
        FCVOGrowing = LO * PrescalerGrowing * PostDivGrowing;
    }

    err = iTDA18250A_CalculatePostDivAndPrescalerBetterMargin(LO, False, &PostDivDecreasing, &PrescalerDecreasing);
    if (err == TM_OK)
    {
        /* Calculate corresponding FVCO value in kHz */
        FVCODecreasing = LO * PrescalerDecreasing * PostDivDecreasing;
    }

    /* Now take the values that are providing the better margin, the goal is +-2 MHz on LO */
    /* So take the point that is the nearest of (FVCOmax - FVCOmin)/2 = 6391,875 MHz */
    if (FCVOGrowing != 0)
    {
        if (FCVOGrowing >= TDA18250A_MIDDLE_FVCO_RANGE)
        {
            DistanceFCVOGrowing = FCVOGrowing - TDA18250A_MIDDLE_FVCO_RANGE;
        }
        else
        {
            DistanceFCVOGrowing = TDA18250A_MIDDLE_FVCO_RANGE - FCVOGrowing;
        }
    }

    if (FVCODecreasing != 0)
    {
        if (FVCODecreasing >= TDA18250A_MIDDLE_FVCO_RANGE)
        {
            DistanceFVCODecreasing = FVCODecreasing - TDA18250A_MIDDLE_FVCO_RANGE;
        }
        else
        {
            DistanceFVCODecreasing = TDA18250A_MIDDLE_FVCO_RANGE - FVCODecreasing;
        }
    }

    if (FCVOGrowing == 0)
    {
        if (FVCODecreasing == 0)
        {
            /* No value at all are found */
            err = TDA18250A_ERR_BAD_PARAMETER;
        }
        else
        {
            /* No value in growing mode, so take the decreasing ones */
            *PostDiv = PostDivDecreasing;
            *Prescaler = PrescalerDecreasing;
        }
    }
    else
    {
        if (FVCODecreasing == 0)
        {
            /* No value in decreasing mode, so take the growing ones */
            *PostDiv = PostDivGrowing;
            *Prescaler = PrescalerGrowing;
        }
        else
        {
            /* Find the value which are the nearest of the middle of VCO range */
            if (DistanceFCVOGrowing <= DistanceFVCODecreasing)
            {
                *PostDiv = PostDivGrowing;
                *Prescaler = PrescalerGrowing;
            }
            else
            {
                *PostDiv = PostDivDecreasing;
                *Prescaler = PrescalerDecreasing;
            }
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18250A_Division32                                         */
/*                                                                            */
/* DESCRIPTION: Calculate the result of (num1 * num2) / denom                 */
/*              where num1 * num2 is larger than 32bits                       */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
static UInt32 iTDA18250A_Division32(UInt32 ulNum1, UInt32 ulNum2, UInt16 ulDenom)
{
    UInt32 ulRes;
    UInt32 ulRemain;
    UInt32 ulTmp;
    UInt32 ulPart1 = 0;
    UInt32 ulNbPart1 = 0;

    if( (ulNum1 == 0) || (ulNum2 == 0) )
    {
        ulRes = 0;
    }
    else
    {
        ulRes = 0;
        ulRemain = 0;

        /* select the smaller of 2 numerators */
        if( ulNum2 < ulNum1 )
        {
            ulTmp = ulNum1;
            ulNum1 = ulNum2;
            ulNum2 = ulTmp;
        }

        ulPart1 = BITS32_MAX / ulNum1;

        if( ulPart1 < ulNum2 )
        {
            /* calculate number of part 1 */
            ulNbPart1 = ulNum2 / ulPart1;

            ulRes = (ulPart1 * ulNum1) / ulDenom;
            ulRes *= ulNbPart1;

            ulRemain = (ulPart1 * ulNum1) % ulDenom;
            ulRemain *= ulNbPart1;

            /* update  ulNum2 */
            ulNum2 -= (ulNbPart1 * ulPart1);
        }

        /* add the remaining part */
        ulRes += (ulNum2 * ulNum1) / ulDenom;
        ulRemain += (ulNum2 * ulNum1) % ulDenom;
        ulRemain /= ulDenom;

        ulRes += ulRemain;
    }

    return ulRes;
}

/*============================================================================*/
/* FUNCTION:    iTDA18250A_CalculateNIntKInt                                   */
/*                                                                            */
/* DESCRIPTION: Calculate PLL N & K values                                    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
extern tmErrorCode_t
iTDA18250A_CalculateNIntKInt(
                             UInt32 LO, 
                             UInt8 PostDiv, 
                             UInt8 Prescaler, 
                             UInt32* NInt, 
                             UInt32* KInt,
                             UInt32 uXtalFreqId
                             )
{
    tmErrorCode_t err = TM_OK;

    /* Algorithm that calculates N_K */
    UInt32 numerator = 0; 
	UInt16 denominator = 0;
    UInt32 N_K_prog = 0;

    /* Algorithm that calculates N, K corrected */
    UInt32 Nprime = 0;
    UInt32 KforceK0_1 = 0;
    UInt32 K2msb = 0;
    UInt32 N0 = 0;
    UInt32 Nm1 = 0;

    /* Calculate N_K_Prog */

	/* RDivNDivTable contains the value of RDiv & Ndiv in the I2C register */
	/* RDiv value is the same as the value in the register */
	/* Ndiv value is not the same, 0 (register) -> 1 (decoded), 1 (register) -> 2 (decoded) */

    /* TODO infunction of VCO */

	/* Max value is 21168000, so it fits into UInt32 */
	numerator = LO * RDivNDivTable[uXtalFreqId].psVCOModes[0].uRDiv * PostDiv * Prescaler;

	/* Max value of denominator is 30000 * 2, so it fits into UInt16 */
	denominator = (UInt16)((RDivNDivTable[uXtalFreqId].uXtalFreq / 1000) * (RDivNDivTable[uXtalFreqId].psVCOModes[0].uNDiv + 1));

	/* But numerator * 16384 = 346816512000, so it doesn't fit into UInt32, to avoid using UInt64, a special function is used */
    N_K_prog = iTDA18250A_Division32(16384, numerator, denominator);

    /* Calculate N & K corrected values */
    Nprime = N_K_prog & 0xFF0000;

    /* Force LSB to 1 */
    KforceK0_1 = 2*(((N_K_prog - Nprime) << 7) / 2) + 1;

    /* Check MSB bit around 2 */
    K2msb = KforceK0_1 >> 21;
    if (K2msb < 1)
    {
        N0 = 1;
    }
    else
    {
        if (K2msb >= 3)
        {
            N0 = 1;
        }
        else
        {
            N0 = 0;
        }
    }
    if (K2msb < 1)
    {
        Nm1 = 1;
    }
    else
    {
        Nm1 = 0;
    }

    /* Calculate N */
    *NInt = (2 * ((Nprime >> 16) - Nm1) + N0) - 128;

    /* Calculate K */
    if (K2msb < 1)
    {
        *KInt = KforceK0_1 + (2 << 21);
    }
    else
    {
        if (K2msb >= 3)
        {
            *KInt = KforceK0_1 - (2 << 21);
        }
        else
        {
            *KInt = KforceK0_1;
        }
    }

    /* Force last 7 bits of K_int to 0x5D, as the IC is doing for spurs optimization */
    *KInt &= 0xFFFFFF80;
    *KInt |= 0x5D;

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18250A_CalculatePostDivAndPrescalerBetterMargin           */
/*                                                                            */
/* DESCRIPTION: Calculate PostDiv and Prescaler by starting from lowest value */
/*              of LO or not                                                  */
/*              LO must be passed in Hz                                       */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18250A_CalculatePostDivAndPrescalerBetterMargin(
                                        UInt32 LO,         /* In Hz */
                                        Bool growingOrder, /* Start from LO = 28.681 kHz or LO = 974 kHz */
                                        UInt8* PostDiv,    /* Absolute value */
                                        UInt8* Prescaler   /* Absolute value  */
                                        )
{
    tmErrorCode_t err = TM_OK;
    Int8 index;
    Int8 sizeTable = sizeof(PostDivPrescalerTableBetterMargin) / sizeof(TDA18250A_PostDivPrescalerTableDef);

    if (growingOrder == True)
    {
        /* Start from LO = 32 MHz */
        for (index = (sizeTable - 1); index >= 0; index--)
        {
            if (
                (LO > PostDivPrescalerTableBetterMargin[index].LO_min) &&
                (LO < PostDivPrescalerTableBetterMargin[index].LO_max)
                )
            {
                /* We are at correct index in the table */
                break;
            }
        }
    }
    else
    {
        /* Start from LO = 1008000 KHz */
        for (index = 0; index < sizeTable; index++)
        {
            if (
                (LO > PostDivPrescalerTableBetterMargin[index].LO_min) &&
                (LO < PostDivPrescalerTableBetterMargin[index].LO_max)
                )
            {
                /* We are at correct index in the table */
                break;
            }
        }
    }

    if ((index == -1) || (index == sizeTable))
    {
        err = TDA18250A_ERR_BAD_PARAMETER;
    }
    else
    {
        /* Write Prescaler */
        *Prescaler = PostDivPrescalerTableBetterMargin[index].Prescaler;

        /* Decode PostDiv */
        *PostDiv = PostDivPrescalerTableBetterMargin[index].PostDiv;
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18250A_SetMSM                                              */
/*                                                                            */
/* DESCRIPTION: Set the MSM bit(s).                                           */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18250A_SetMSM(
                  pTDA18250AObject_t   pObj,   /* I: Driver object */
                  UInt8               uValue, /* I: Item value */
                  Bool                bLaunch /* I: Launch MSM */
                  )
{
    tmErrorCode_t   err = TM_OK;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18250A_SetMSM(0x%08X)", pObj->tUnitW);

    /* Set state machine and Launch it */
    err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_MSM_byte_1, uValue, Bus_None);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));

    if(err == TM_OK && bLaunch)
    {
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_MSM_byte_2__MSM_Launch, 0x01, Bus_None);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        err = iTDA18250A_WriteRegMap(pObj, gTDA18250A_Reg_MSM_byte_1.Address, bLaunch?0x02:0x01);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_WriteRegMap(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK && bLaunch)
    {
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_MSM_byte_2__MSM_Launch, 0x00, Bus_None);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18250A_WaitIRQ                                             */
/*                                                                            */
/* DESCRIPTION: Wait the IRQ to trigger                                       */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18250A_WaitIRQ(
                   pTDA18250AObject_t   pObj,       /* I: Driver object */
                   UInt32              timeOut,    /* I: timeout */
                   UInt32              waitStep,   /* I: wait step */
                   UInt8               irqStatus   /* I: IRQs to wait */
                   )
{
    tmErrorCode_t   err = TM_OK;
    UInt32          counter = timeOut/waitStep; /* Wait max timeOut/waitStep ms */
    UInt8           uIRQ = 0;
    UInt8           uIRQStatus = 0;
    Bool            bIRQTriggered = False;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18250A_WaitIRQ(0x%08X)", pObj->tUnitW);

    while(err == TM_OK && (counter--)>0)
    {
        err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_IRQ_status__IRQ_status, &uIRQ, Bus_RW);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Read(0x%08X) failed.", pObj->tUnitW));

        if(err == TM_OK && uIRQ == 1)
        {
            bIRQTriggered = True;
        }

        if(bIRQTriggered)
        {
            /* IRQ triggered => Exit */
            break;
        }

        if(err == TM_OK && irqStatus != 0x00)
        {
            err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_IRQ_status, &uIRQStatus, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Read(0x%08X) failed.", pObj->tUnitW));

            if(irqStatus == uIRQStatus)
            {
                bIRQTriggered = True;
            }
        }

        if(counter)
        {
            err = iTDA18250A_Wait(pObj, waitStep);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Wait(0x%08X) failed.", pObj->tUnitW));
        }
    }

    if(err == TM_OK && bIRQTriggered == False)
    {
        err = TDA18250A_ERR_NOT_READY;
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18250A_Write                                               */
/*                                                                            */
/* DESCRIPTION: Writes in TDA18250A hardware                                   */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18250A_Write(
                 pTDA18250AObject_t           pObj,       /* I: Driver object */
                 const TDA18250A_BitField_t*  pBitField, /* I: Bitfield structure */
                 UInt8                       uData,      /* I: Data to write */
                 tmbslFrontEndBusAccess_t                eBusAccess  /* I: Access to bus */
                 )
{
    tmErrorCode_t   err = TM_OK;
    UInt8           RegAddr = 0;
    UInt32          DataLen = 1;
    UInt8           RegData = 0;
    pUInt8          pRegData = Null;
    UInt32          RegMask = 0;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18250A_Write(0x%08X)", pObj->tUnitW);

    if(pBitField == Null)
    {
        err = TDA18250A_ERR_BAD_PARAMETER;
    }

    if(err == TM_OK)
    {
        /* Set Register Address */
        RegAddr = pBitField->Address;

        if(RegAddr < TDA18250A_REG_MAP_NB_BYTES)
        {
            pRegData = (UInt8 *)(&(pObj->RegMap)) + RegAddr;
        }
        else
        {
            pRegData = &RegData;
        }

        if( (eBusAccess&Bus_NoRead) == False && P_SIO_READ_VALID)
        {
            /* Read data from TDA18250A */
            err = P_SIO_READ(pObj->tUnitW, TDA18250A_REG_ADD_SZ, &RegAddr, DataLen, pRegData);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "IO_Read(0x%08X, 1, 0x%02X, %d) failed.", pObj->tUnitW, RegAddr, DataLen));
        }

        if(err == TM_OK)
        {
            RegMask = ( (1 << pBitField->WidthInBits) - 1);
            /* Limit uData to WidthInBits */
            uData &= RegMask;

            /* Set Data */
            RegMask = RegMask << pBitField->PositionInBits;
            *pRegData &= (UInt8)(~RegMask);
            *pRegData |= uData << pBitField->PositionInBits;

            if( (eBusAccess&Bus_NoWrite) == False && P_SIO_WRITE_VALID)
            {
                /* Write data to TDA18250A */
                err = P_SIO_WRITE(pObj->tUnitW, TDA18250A_REG_ADD_SZ, &RegAddr, DataLen, pRegData);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "IO_Write(0x%08X, 1, 0x%02X, %d) failed.", pObj->tUnitW, RegAddr, DataLen));
            }
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18250A_Read                                                */
/*                                                                            */
/* DESCRIPTION: Reads in TDA18250A hardware                                    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18250A_Read(
                pTDA18250AObject_t           pObj,       /* I: Driver object */
                const TDA18250A_BitField_t*  pBitField, /* I: Bitfield structure */
                UInt8*                      puData,     /* I: Data to read */
                tmbslFrontEndBusAccess_t                eBusAccess  /* I: Access to bus */
                )
{
    tmErrorCode_t   err = TM_OK;
    UInt8           RegAddr = 0;
    UInt32          DataLen = 1;
    UInt8           RegData = 0;
    pUInt8          pRegData = Null;
    UInt32          RegMask = 0;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18250A_Read(0x%08X)", pObj->tUnitW);

    if(pBitField == Null)
    {
        err = TDA18250A_ERR_BAD_PARAMETER;
    }

    if(err == TM_OK)
    {
        /* Set Register Address */
        RegAddr = pBitField->Address;

        if(RegAddr < TDA18250A_REG_MAP_NB_BYTES)
        {
            pRegData = (UInt8 *)(&(pObj->RegMap)) + RegAddr;
        }
        else
        {
            pRegData = &RegData;
        }

        if( (eBusAccess&Bus_NoRead) == False && P_SIO_READ_VALID)
        {
            /* Read data from TDA18250A */
            err = P_SIO_READ(pObj->tUnitW, TDA18250A_REG_ADD_SZ, &RegAddr, DataLen, pRegData);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "IO_Read(0x%08X, 1, 0x%02X, %d) failed.", pObj->tUnitW, RegAddr, DataLen));
        }

        if(err == TM_OK && puData != Null)
        {
            /* Copy Raw Data */
            *puData = *pRegData;

            /* Get Data */
            RegMask = ( (1 << pBitField->WidthInBits) - 1) << pBitField->PositionInBits;
            *puData &= (UInt8)RegMask;
            *puData = (*puData) >> pBitField->PositionInBits;
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18250A_WriteRegMap                                         */
/*                                                                            */
/* DESCRIPTION: Writes driver RegMap cached data to TDA18250A hardware.        */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18250A_WriteRegMap(
                       pTDA18250AObject_t   pObj,       /* I: Driver object */
                       UInt8               uAddress,   /* I: Data to write */
                       UInt32              uWriteLen   /* I: Number of data to write */
                       )
{
    tmErrorCode_t   err = TM_OK;
    pUInt8          pRegData = Null;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18250A_WriteRegMap(0x%08X)", pObj->tUnitW);

    if( uAddress < TDA18250A_REG_MAP_NB_BYTES &&
        (uAddress + uWriteLen) <= TDA18250A_REG_MAP_NB_BYTES )
    {
        pRegData = (UInt8 *)(&(pObj->RegMap)) + uAddress;
    }
    else
    {
        err = TDA18250A_ERR_BAD_PARAMETER;
    }

    if(err == TM_OK && P_SIO_WRITE_VALID)
    {
        /* Write data to TDA18250A */
        err = P_SIO_WRITE(pObj->tUnitW, TDA18250A_REG_ADD_SZ, &uAddress, uWriteLen, pRegData);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "IO_Write(0x%08X, 1, 0x%02X, %d) failed.", pObj->tUnitW, uAddress, uWriteLen));
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18250A_ReadRegMap                                          */
/*                                                                            */
/* DESCRIPTION: Reads driver RegMap cached data from TDA18250A hardware.       */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18250A_ReadRegMap(
                      pTDA18250AObject_t   pObj,       /* I: Driver object */
                      UInt8               uAddress,   /* I: Data to read */
                      UInt32              uReadLen    /* I: Number of data to read */
                      )
{
    tmErrorCode_t   err = TM_OK;
    pUInt8          pRegData = Null;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18250A_ReadRegMap(0x%08X)", pObj->tUnitW);

    if( uAddress < TDA18250A_REG_MAP_NB_BYTES &&
        (uAddress + uReadLen) <= TDA18250A_REG_MAP_NB_BYTES )
    {
        pRegData = (UInt8 *)(&(pObj->RegMap)) + uAddress;
    }
    else
    {
        err = TDA18250A_ERR_BAD_PARAMETER;
    }

    if(err == TM_OK && P_SIO_READ_VALID)
    {
        /* Read data from TDA18250A */
        err = P_SIO_READ(pObj->tUnitW, TDA18250A_REG_ADD_SZ, &uAddress, uReadLen, pRegData);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "IO_Read(0x%08X, 1, 0x%02X, %d) failed.", pObj->tUnitW, uAddress, uReadLen));
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18250A_Wait                                                */
/*                                                                            */
/* DESCRIPTION: Waits for requested time.                                     */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t 
iTDA18250A_Wait(
                pTDA18250AObject_t   pObj,   /* I: Driver object */
                UInt32              Time    /* I: time to wait for */
                )
{
    tmErrorCode_t   err = TDA18250A_ERR_NULL_CONTROLFUNC;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18250A_Wait(0x%08X)", pObj->tUnitW);

    if(P_STIME_WAIT_VALID)
    {
        /* Wait Time ms */
        err = P_STIME_WAIT(pObj->tUnitW, Time);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TIME_Wait(0x%08X, %d) failed.", pObj->tUnitW, Time));
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18250A_PowerSavingMode                                     */
/*                                                                            */
/* DESCRIPTION: set registers according  the  PowerSavingMode                 */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t 
iTDA18250A_PowerSavingMode(
                           pTDA18250AObject_t   pObj/*,*/   /* I: Driver object */
                           /*TDA18250APowerSavingMode_t  PowerSavingMode*/    /* I: time to wait for */
                           )
{
    tmErrorCode_t   err = TDA18250A_ERR_NULL_CONTROLFUNC;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18250A_PowerSavingMode(0x%08X)", pObj->tUnitW);

    if ( (pObj->StandardMode != TDA18250A_StandardMode_Unknown) && (pObj->StandardMode != TDA18250A_StandardMode_Max) )
    {
        if (pObj->eChipVersion == TDA18250A_VersionES1)
        {
            /* PowerSavingMode nominal */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_1, pObj->pStandard->uES1PSMByte1, Bus_None);
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_2, pObj->pStandard->uES1PSMByte2, Bus_None);
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_3, pObj->pStandard->uES1PSMByte3, Bus_None);
        }
        else if (pObj->eChipVersion == TDA18250A_VersionES2)
        {
            /* PowerSavingMode low */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_1, pObj->pStandard->uES2PSMByte1, Bus_None);
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_2, pObj->pStandard->uES2PSMByte2, Bus_None);
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_3, pObj->pStandard->uES2PSMByte3, Bus_None);
        }
        else
        {
            /* PowerSavingMode low */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_1, pObj->pStandard->uES3PSMByte1, Bus_None);
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_2, pObj->pStandard->uES3PSMByte2, Bus_None);
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_3, pObj->pStandard->uES3PSMByte3, Bus_None);
        }
        if(err == TM_OK)
        {
            /* Write gTDA18250A_Reg_PowerSavingMode_byte_1 (0x23) to gTDA18250A_Reg_PowerSavingMode_byte_4 (0x26) Registers */
            err = iTDA18250A_WriteRegMap(pObj, gTDA18250A_Reg_PowerSavingMode_byte_1.Address, TDA18250A_REG_DATA_LEN(gTDA18250A_Reg_PowerSavingMode_byte_1, gTDA18250A_Reg_PowerSavingMode_byte_3));
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_WriteRegMap(0x%08X) failed.", pObj->tUnitW));
        }
    }
    else
    {
        if (pObj->eChipVersion == TDA18250A_VersionES1)
        {
            /* PowerSavingMode nominal */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_1, 0x75, Bus_None);
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_2, 0x06, Bus_None);
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_3, 0x47, Bus_None);
        }
        else
        {
            /* PowerSavingMode low */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_1, 0x44, Bus_None);
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_2, 0x42, Bus_None);
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_PowerSavingMode_byte_3, 0x05, Bus_None);
        }
        if(err == TM_OK)
        {
            /* Write gTDA18250A_Reg_PowerSavingMode_byte_1 (0x23) to gTDA18250A_Reg_PowerSavingMode_byte_4 (0x26) Registers */
            err = iTDA18250A_WriteRegMap(pObj, gTDA18250A_Reg_PowerSavingMode_byte_1.Address, TDA18250A_REG_DATA_LEN(gTDA18250A_Reg_PowerSavingMode_byte_1, gTDA18250A_Reg_PowerSavingMode_byte_3));
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_WriteRegMap(0x%08X) failed.", pObj->tUnitW));
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18250A_SetNDivRDiv                                        */
/*                                                                            */
/* DESCRIPTION: set registers according  the  PowerSavingMode                 */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18250A_SetNDivRDiv(
                       pTDA18250AObject_t pObj)
{
    tmErrorCode_t       err = TM_OK;
    UInt32          uIF = 0;
    UInt8           ProgIF = 0;
    UInt8           uLOPostdiv = 0;
    UInt8           uLOPostdivRegister = 0;
    UInt8           uLOPrescaler = 0;
    UInt32          FVCO = 0;
    UInt32          uXtalFreqId;
    UInt32          i;
    UInt8           uRDiv = 0;
    UInt8           uNDiv = 0;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18250A_SetNDivKDiv(0x%08X)", pObj->tUnitW);


    /* Get IF */
    if(err == TM_OK)
    {
        /* IF */
        err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_IF_Frequency_byte__IF_Freq, &ProgIF, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Read(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        /* Decode IF */
        uIF = ProgIF*50000;

        /* Set IRQ_clear */
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_IRQ_clear, TDA18250A_IRQ_Global|TDA18250A_IRQ_SetRF, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18250A_SetMSM(pObj, TDA18250A_MSM_SetRF, True);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_SetMSM(0x%08X, TDA18250A_MSM_SetRF) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        /* Wait for IRQ to trigger */
        err = iTDA18250A_WaitIRQ(pObj, 500, 10, TDA18250A_IRQ_SetRF);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_WaitIRQ(0x%08X, 50, 5, TDA18250A_IRQ_SetRF) failed.", pObj->tUnitW));
    }
    if (err == TM_OK)
    {
        err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_Main_Post_Divider_byte__LOPostDiv, &uLOPostdivRegister, Bus_RW);
    }
    if (err == TM_OK)
    {
        switch (uLOPostdivRegister)
        {
        case 1:
            uLOPostdiv = 1;
            break;

        case 2:
            uLOPostdiv = 2;
            break;

        case 3:
            uLOPostdiv = 4;
            break;

        case 4:
            uLOPostdiv = 8;
            break;

        case 5:
            uLOPostdiv = 16;
            break;

        default:
            uLOPostdiv = 0;
            break;
        }
        err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_Main_Post_Divider_byte__LOPresc, &uLOPrescaler, Bus_RW);
    }

    if (err == TM_OK)
    {
        uXtalFreqId = pObj->eXtalFreq-1;
    
        /* Calculate FVCO in MHz*/
        FVCO = uLOPostdiv * uLOPrescaler * ((pObj->uRF + uIF) / 1000);

        for (i = 0 ; i < RDivNDivTable[uXtalFreqId].uNbMode ; i++)
        {
            if (FVCO >= RDivNDivTable[uXtalFreqId].psVCOModes[i].uVCOFreq)
            {
                uRDiv = RDivNDivTable[uXtalFreqId].psVCOModes[i].uRDiv;
                uNDiv = RDivNDivTable[uXtalFreqId].psVCOModes[i].uNDiv;
            }
        }

        /* N_Div */
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Xtal_Flex_byte_3__N_Div, uNDiv, Bus_RW);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        /* R_Div */
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_Xtal_Flex_byte_3__R_Div, uRDiv, Bus_RW);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
    }

	return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18250A_SetFreqDependantStandard                           */
/*                                                                            */
/* DESCRIPTION: set registers according  the  PowerSavingMode                 */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18250A_SetFreqDependantStandard(
                       pTDA18250AObject_t pObj,
                       UInt32 uRF)
{
    tmErrorCode_t       err = TM_OK;
    UInt8 uValue1 = 0;
    UInt8 uValue2 = 0;
    UInt i = 0;
    Bool bSmoothEnabled = True;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18250A_SetFreqDependantStandard(0x%08X)", pObj->tUnitW);

    if (pObj->StandardMode != TDA18250A_StandardMode_Unknown)
    {
#ifdef TDA18250A_DVBC_SUPPORTED
        if (pObj->StandardMode == TDA18250A_QAM_6MHz)
        {
            /* RF BPF */
            if ((uRF >= 244224000) && (uRF < 320000000))
            {
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_RF_Band_Pass_Filter_byte__RF_BPF_Bypass, 0x00, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));

                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_RF_Band_Pass_Filter_byte__RF_BPF, 0x06, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }
            else
            {
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_RF_Band_Pass_Filter_byte__RF_BPF_Bypass, 0x00, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));

                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_RF_Band_Pass_Filter_byte__RF_BPF, 0x06, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }
            /* CSTAB */
            if (uRF < 100000000)
            {
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_H3H5_CSTAB_byte__CSTAB_EN, 0x01, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));

                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_H3H5_CSTAB_byte__CSTAB, 0x00, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));

            }
            else
            {
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_H3H5_CSTAB_byte__CSTAB_EN, 0x00, Bus_RW);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));

                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_H3H5_CSTAB_byte__CSTAB, 0x00, Bus_RW);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }
        }
        else
        {
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_RF_Band_Pass_Filter_byte__RF_BPF_Bypass, 0x00, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));

            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_RF_Band_Pass_Filter_byte__RF_BPF, 0x06, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));

            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_H3H5_CSTAB_byte__CSTAB_EN, 0x00, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));

            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_H3H5_CSTAB_byte__CSTAB, 0x00, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
#endif

        /* AGC1 TOP */
        if (pObj->eChipVersion == TDA18250A_VersionES1)
        {
            i=0;
            while ( (i<TDA18250A_CONFIG_STD_FREQ_NUM) && (uRF >= pObj->pStandard->AGC1_TOP_ES1[i].uFreq) )
            {
                i++;
            }
            i--;
            if (pObj->pStandard->AGC1_TOP_ES1[i].uFreq == (UInt32)-1)
            {
                i--;
            }
            uValue1 = pObj->pStandard->AGC1_TOP_ES1[i].uTOPDnUp;

            if(err == TM_OK)
            {
                /* Set AGC1 TOP I2C DN/UP */
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_1__AGC1_Top, uValue1, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }
        }
        else if (pObj->eChipVersion == TDA18250A_VersionES2)
        {
            i=0;
            while ( (i<TDA18250A_CONFIG_STD_FREQ_NUM) && (uRF >= pObj->pStandard->AGC1_TOP_ES2[i].uFreq) )
            {
                i++;
            }
            i--;
            if (pObj->pStandard->AGC1_TOP_ES2[i].uFreq == (UInt32)-1)
            {
                i--;
            }
            uValue1 = pObj->pStandard->AGC1_TOP_ES2[i].uTOPDnUp;

            if(err == TM_OK)
            {
                /* Set AGC1 TOP I2C DN/UP */
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_1__AGC1_Top, uValue1, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }
        }
        else
        {
            i=0;
            while ( (i<TDA18250A_CONFIG_STD_FREQ_NUM) && (uRF >= pObj->pStandard->AGC1_TOP_ES3[i].uFreq) )
            {
                i++;
            }
            i--;
            if (pObj->pStandard->AGC1_TOP_ES3[i].uFreq == (UInt32)-1)
            {
                i--;
            }
            uValue1 = pObj->pStandard->AGC1_TOP_ES3[i].uTOPDnUp;

            if(err == TM_OK)
            {
                /* Set AGC1 TOP I2C DN/UP */
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_1__AGC1_Top, uValue1, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }
        }

        /* AGC2 TOP */
        if (pObj->eChipVersion == TDA18250A_VersionES1)
        {
            i=0;
            while ( (i<TDA18250A_CONFIG_STD_FREQ_NUM) && (uRF >= pObj->pStandard->AGC2_TOP_ES1[i].uFreq) )
            {
                i++;
            }
            i--;
            if (pObj->pStandard->AGC2_TOP_ES1[i].uFreq == (UInt32)-1)
            {
                i--;
            }
            uValue1 = pObj->pStandard->AGC2_TOP_ES1[i].uTOPDn;
            uValue2 = pObj->pStandard->AGC2_TOP_ES1[i].uTOPUp;
            
            if(err == TM_OK)
            {
                /* AGC2_TOP_DO */
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_2__AGC2_TOP_DO, 107-uValue1, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }

            if(err == TM_OK)
            {
                /* AGC2_TOP_UP */ 
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_3__AGC2_TOP_UP, 107-uValue2, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }
        }
        else if (pObj->eChipVersion == TDA18250A_VersionES2)
        {
            i=0;
            while ( (i<TDA18250A_CONFIG_STD_FREQ_NUM) && (uRF >= pObj->pStandard->AGC2_TOP_ES2[i].uFreq) )
            {
                i++;
            }
            i--;
            if (pObj->pStandard->AGC2_TOP_ES2[i].uFreq == (UInt32)-1)
            {
                i--;
            }
            uValue1 = pObj->pStandard->AGC2_TOP_ES2[i].uTOPDn;
            uValue2 = pObj->pStandard->AGC2_TOP_ES2[i].uTOPUp;
            /* AGC2_TOP_Up_Udld */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_UDLD_byte_1__AGC2_TOP_UP_UDLD, (107-uValue2)*2-pObj->pStandard->AGC2_TOP_Up_Udld, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));

            if(err == TM_OK)
            {
                /* AGC2_TOP_DO */
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_2__AGC2_TOP_DO, (107-uValue1)*2, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }

            if(err == TM_OK)
            {
                /* AGC2_TOP_UP */ 
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_3__AGC2_TOP_UP, (107-uValue2)*2, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }
            if (uRF >= 320000000)
            {
                if(err == TM_OK)
                { /* AGC2_adapt_top23_delta */
                    err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_5__AGC2_adapt_top23_delta, pObj->pStandard->AGC2_Adapt_TOP23_Delta_ES2, Bus_None);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
                }            
                if(err == TM_OK)
                { /* AGC2_adapt_top23_enable */
                    err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_5__AGC2_adapt_top23_enable, (pObj->pStandard->AGC2_Adapt_TOP23_Enable_ES2 ? 0x01:0x00), Bus_NoRead);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
                }   
            }
            else
            {
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_5__AGC2_adapt_top23_enable, 0x00, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }

        }
        else
        {
            i=0;
            while ( (i<TDA18250A_CONFIG_STD_FREQ_NUM) && (uRF >= pObj->pStandard->AGC2_TOP_ES3[i].uFreq) )
            {
                i++;
            }
            i--;
            if (pObj->pStandard->AGC2_TOP_ES3[i].uFreq == (UInt32)-1)
            {
                i--;
            }
            uValue1 = pObj->pStandard->AGC2_TOP_ES3[i].uTOPDn;
            uValue2 = pObj->pStandard->AGC2_TOP_ES3[i].uTOPUp;
            /* AGC2_TOP_Up_Udld */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_UDLD_byte_1__AGC2_TOP_UP_UDLD, (107-uValue2)*2-pObj->pStandard->AGC2_TOP_Up_Udld, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));

            if(err == TM_OK)
            {
                /* AGC2_TOP_DO */
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_2__AGC2_TOP_DO, (107-uValue1)*2, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }

            if(err == TM_OK)
            {
                /* AGC2_TOP_UP */ 
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_3__AGC2_TOP_UP, (107-uValue2)*2, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }
            if (uRF >= 320000000)
            {
                if(err == TM_OK)
                { /* AGC2_adapt_top23_delta */
                    err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_5__AGC2_adapt_top23_delta, 9, Bus_None);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
                }            
                if(err == TM_OK)
                { /* AGC2_adapt_top23_enable */
                    err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_5__AGC2_adapt_top23_enable, (pObj->pStandard->AGC2_Adapt_TOP23_Enable_ES3 ? 0x01:0x00), Bus_NoRead);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
                }   
            }
            else
            {
                if(err == TM_OK)
                { /* AGC2_adapt_top23_delta */
                    err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_5__AGC2_adapt_top23_delta, 9, Bus_None);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
                }            
                if(err == TM_OK)
                { /* AGC2_adapt_top23_enable */
                    err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_5__AGC2_adapt_top23_enable, (pObj->pStandard->AGC2_Adapt_TOP23_Enable_ES3 ? 0x01:0x00), Bus_NoRead);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
                }
            }

        }
        /* AGC2_cft_shift */
        if (uRF > 800000000)
        {
            if(err == TM_OK)
            {
                /* AGC2_cft_shift */
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_1__AGC2_cfi_shift, 0x02, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }
        }
        else
        {
            if(err == TM_OK)
            {
                /* AGC2_cft_shift */
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_1__AGC2_cfi_shift, 0x01, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }
        }

        /* AGC3 TOP */
        if(err == TM_OK)
        {
            i=0;
            while ( (i<TDA18250A_CONFIG_STD_FREQ_NUM) && (uRF >= pObj->pStandard->AGC3_TOP[i].uFreq) )
            {
                i++;
            }
            i--;
            if (pObj->pStandard->AGC3_TOP[i].uFreq == (UInt32)-1)
            {
                i--;
            }
            uValue1 = pObj->pStandard->AGC3_TOP[i].uTOPDn;
            uValue2 = pObj->pStandard->AGC3_TOP[i].uTOPUp;
        
            /* AGC3_TOP_DO */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC3_byte_1__AGC3_TOP_DO, 117-uValue1, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* AGC3_TOP_UP */
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC3_byte_1__AGC3_TOP_UP, 117-uValue2, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }

        /* S2D gain */
        if(err == TM_OK)
        {
            i=0;
            while ((i<TDA18250A_CONFIG_STD_FREQ_NUM) && (uRF >= pObj->pStandard->S2D_Gain[i].uFreq) )
            {
                i++;
            }
            i--;
            if (pObj->pStandard->S2D_Gain[i].uFreq == (UInt32)-1)
            {
                i--;
            }
            uValue1 = pObj->pStandard->S2D_Gain[i].eS2DGain;

            /* S2D gain */ 
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_IR_Mixer_byte_1__S2D_Gain, uValue1, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
        }

        /* AGC2_HP_EN_BP and AGC2_HP_EN */
        if(err == TM_OK)
        {
#ifdef TDA18250A_DVBC_SUPPORTED 
            if ( (pObj->StandardMode == TDA18250A_QAM_8MHz) || (pObj->StandardMode == TDA18250A_QAM_6MHz) )
            {
                if (uRF < 600000000)
                {
                    uValue1 = 0x00;
                    uValue2 = 0x00;
                }
                else
                {
                    uValue1 = 0x01;
                    uValue2 = 0x01;
                }
            }
            else
            {
#endif
                uValue1 = 0x00;
                uValue2 = 0x00;
#ifdef TDA18250A_DVBC_SUPPORTED 
            }
#endif
        
            /* AGC2_HP_EN_BP */ 
            err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_5__AGC2_HP_EN_BP, uValue1, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));

            if (err == TM_OK)
            {
                /* AGC2_HP_EN */ 
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC2_byte_5__AGC2_HP_EN, uValue2, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }
        }

        /* AGC1 Smooth management */
        if (err == TM_OK)
        {
            /* Smooth algo must be done only on ES2 */
            if (pObj->eChipVersion == TDA18250A_VersionES2)
            {
                if (pObj->pStandard->AGC1_GAIN != TDA18250AAGC1_GAIN_Free_Frozen) /* Free_Frozen algo overrides the gain smooth algo */
                {
                    if (pObj->pStandard->AGC1_GAIN_SMOOTH_ALGO == TDA18250AAGC1_GAIN_SMOOTH_ALGO_Enabled) /* smooth algo */
                    {
                        err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_AGC1_LT_Gain_status__AGC1_gain_read, &uValue1, Bus_NoWrite);
                        if (uValue1 < 6) /* 7dB index 6   {-11,-8,-5,-2,1,4,7,10,13,16,19,22}  */
                        {
                            bSmoothEnabled = False;
                        }
                        else /* > 7dB */
                        {
                            bSmoothEnabled = True;
                        }
                    }
                    else /* no smooth algo */
                    {
                        bSmoothEnabled = True;
                    }
                }
                else /* Free_Frozen algo forces smooth to disabled */
                {
                    bSmoothEnabled = False;
                }
                
            }
            else /* For ES1 and ES3, smooth is always enabled */
            { 
                bSmoothEnabled = True;
            }
        }

        if (err == TM_OK)
        {
            if (bSmoothEnabled)
            {
                /* restore values standard dependant */
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_4__AGC1_smooth_t_cst, pObj->pStandard->AGC1_Smooth_T_Cst, Bus_None);
                /* restore value As in HWini() */
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_4__AGC1_smooth_t_cst_fast, 0x01, Bus_NoRead);                
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }
            else
            {
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_4__AGC1_smooth_t_cst, 0x00, Bus_None);
                err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_4__AGC1_smooth_t_cst_fast, 0x00, Bus_NoRead);                
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
            }
        }

        /* AGC1 Free_Frozen */
        if (err == TM_OK)
        {
            if (pObj->eChipVersion == TDA18250A_VersionES2)
            {
                if (pObj->pStandard->AGC1_GAIN == TDA18250AAGC1_GAIN_Free_Frozen)
                {
                    err = iTDA18250A_AGC1_Update(pObj);
                }
            }
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18250A_AGC1_Update                                        */
/*                                                                            */
/* DESCRIPTION: Update AGC1 on AGC1 freeze mode                               */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18250A_AGC1_Update( pTDA18250AObject_t pObj)
{
    UInt8 uGainIndexPrev, uGainIndexCur;
    tmErrorCode_t       err = TM_OK;

    /* Get current AGC1 */
    err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_AGC1_LT_Gain_status__AGC1_gain_read, &uGainIndexCur, Bus_NoWrite);

    /* Free AGC */
    if(err == TM_OK)
    {
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_3__AGC1_loop_off, 0x00, Bus_None);
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_3__Force_AGC1_gain, 0x00, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
    }
    
    /* Wait until AGC1 stable ( no change, min or max reached ) */
    do
    {
        uGainIndexPrev = uGainIndexCur;
        if(err == TM_OK)
        {
            err = iTDA18250A_Wait( pObj, 50); /* wait one potential Step*/
            err = iTDA18250A_Read(pObj, &gTDA18250A_Reg_AGC1_LT_Gain_status__AGC1_gain_read, &uGainIndexCur, Bus_NoWrite);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Read(0x%08X) failed.", pObj->tUnitW));
        }
    } while ( err == TM_OK && uGainIndexCur != uGainIndexPrev && uGainIndexCur != 0 && uGainIndexCur != 11 );

    /* Freeze AGC1 on current Free Value */
    if(err == TM_OK)
    {
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_3__AGC1_Gain, uGainIndexCur, Bus_None);
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_3__AGC1_loop_off, 0x01, Bus_None);
        err = iTDA18250A_Write(pObj, &gTDA18250A_Reg_AGC1_byte_3__Force_AGC1_gain, 0x01, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18250A_Write(0x%08X) failed.", pObj->tUnitW));
    }
    return err;
}


#ifdef _TVFE_IMPLEMENT_MUTEX

/*============================================================================*/
/* FUNCTION:    iTDA18250A_MutexAcquire:                                       */
/*                                                                            */
/* DESCRIPTION: Acquires driver mutex.                                        */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
iTDA18250A_MutexAcquire(
                        pTDA18250AObject_t   pObj,
                        UInt32              timeOut
                        )
{
    tmErrorCode_t   err = TDA18250A_ERR_NULL_CONTROLFUNC;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18250A_MutexAcquire(0x%08X)", pObj->tUnitW);

    if(P_SMUTEX_ACQUIRE_VALID && P_MUTEX_VALID)
    {
        err = P_SMUTEX_ACQUIRE(P_MUTEX, timeOut);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "Mutex_Acquire(0x%08X, %d) failed.", pObj->tUnitW, timeOut));
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18250A_MutexRelease:                                       */
/*                                                                            */
/* DESCRIPTION: Releases driver mutex.                                        */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
iTDA18250A_MutexRelease(
                        pTDA18250AObject_t   pObj
                        )
{
    tmErrorCode_t   err = TDA18250A_ERR_NULL_CONTROLFUNC;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18250A_MutexRelease(0x%08X)", pObj->tUnitW);

    if(P_SMUTEX_RELEASE_VALID && P_MUTEX_VALID)
    {
        err = P_SMUTEX_RELEASE(P_MUTEX);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "Mutex_Release(0x%08X) failed.", pObj->tUnitW));
    }

    return err;
}

#endif

/*============================================================================*/
/* FUNCTION:    iTDA18250A_AllocInstance:                                      */
/*                                                                            */
/* DESCRIPTION: Allocates an instance.                                        */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
iTDA18250A_AllocInstance(
                         tmUnitSelect_t      tUnit,      /* I: Unit number */
                         ppTDA18250AObject_t  ppDrvObject /* I: Driver Object */
                         )
{
    tmErrorCode_t       err = TDA18250A_ERR_ERR_NO_INSTANCES;
    pTDA18250AObject_t   pObj = Null;
    UInt32              uLoopCounter = 0;

    /* Find a free instance */
    for (uLoopCounter = 0; uLoopCounter<TDA18250A_MAX_UNITS; uLoopCounter++)
    {
        pObj = &gTDA18250AInstance[uLoopCounter];
        if (pObj->init == False)
        {
            err = TM_OK;

            pObj->tUnit = tUnit;
            pObj->tUnitW = tUnit;

            err = iTDA18250A_ResetInstance(pObj);

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
/* FUNCTION:    iTDA18250A_DeAllocInstance:                                    */
/*                                                                            */
/* DESCRIPTION: Deallocates an instance.                                      */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
iTDA18250A_DeAllocInstance(
                           pTDA18250AObject_t  pDrvObject  /* I: Driver Object */
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
/* FUNCTION:    iTDA18250A_GetInstance:                                        */
/*                                                                            */
/* DESCRIPTION: Gets an instance.                                             */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
iTDA18250A_GetInstance(
                       tmUnitSelect_t      tUnit,      /* I: Unit number   */
                       ppTDA18250AObject_t  ppDrvObject /* I: Driver Object */
                       )
{
    tmErrorCode_t       err = TDA18250A_ERR_ERR_NO_INSTANCES;
    pTDA18250AObject_t   pObj = Null;
    UInt32              uLoopCounter = 0;

    /* Get instance */
    for (uLoopCounter = 0; uLoopCounter<TDA18250A_MAX_UNITS; uLoopCounter++)
    {
        pObj = &gTDA18250AInstance[uLoopCounter];
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
/* FUNCTION:    iTDA18250A_ResetInstance:                                      */
/*                                                                            */
/* DESCRIPTION: Resets an instance.                                           */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
iTDA18250A_ResetInstance(
                         pTDA18250AObject_t  pDrvObject  /* I: Driver Object */
                         )
{
    tmErrorCode_t   err = TM_OK ;

    pDrvObject->curPowerState = TDA18250A_CONFIG_CURPOWERSTATE_DEF;
    pDrvObject->curLLPowerState = TDA18250A_CONFIG_CURLLPOWERSTATE_DEF;
    pDrvObject->uRF = TDA18250A_CONFIG_RF_DEF;
    pDrvObject->uProgRF = TDA18250A_CONFIG_PROG_RF_DEF;

    pDrvObject->StandardMode = TDA18250A_CONFIG_STANDARDMODE_DEF;

    pDrvObject->eHwState = TDA18250A_HwState_InitNotDone;

    return err;
}

