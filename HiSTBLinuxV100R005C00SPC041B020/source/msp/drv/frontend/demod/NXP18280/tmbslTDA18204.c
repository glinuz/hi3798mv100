/*
  Copyright (C) 2010 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmbslTDA18204.c
 *
 *                %version: CFR_FEAP#39 %
 *
 * \date          %modify_time%
 *
 * \author        Bruno LEMESLE
 *
 * \brief         Describe briefly the purpose of this file.
 *
 * REFERENCE DOCUMENTS :
 *                TDA18204_Driver_User_Guide.pdf
 *
 * TVFE SW Arch V4 Template: Author Christophe CAZETTES
 *
 * \section info Change Information
 *
*/

/*============================================================================*/
/* Standard include files:                                                    */
/*============================================================================*/
//#include <string.h>
#include <linux/string.h>
#include <linux/kernel.h>


#include "tmNxTypes.h"
#include "tmCompId.h"
#include "tmFrontEnd.h"
#include "tmbslFrontEndTypes.h"
#include "tmUnitParams.h"

/*============================================================================*/
/* Project include files:                                                     */
/*============================================================================*/
#include "tmbslTDA18204.h"

#include "tmbslTDA18204_RegDef.h"
#include "tmbslTDA18204_InstanceCustom.h"
#include "tmbslTDA18204_Local.h"

/*============================================================================*/
/* Global data:                                                               */
/*============================================================================*/
static const UInt32 uVCOMaxMargin = 100000; /* VCO max margin (in KHz) */
static const UInt32 uVCOSwitch = 130; /* VCO switch decision */

/* used for RSSI WB */
static Int32 Lvl_AGC1_ES1[TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC1_MAX][TDA18204_Coeff_X_Max] = TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC1_ES1;
static Int32 Lvl_AGC2WB_ES1[TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC2_MAX][TDA18204_Coeff_X_Max] = TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC2WB_ES1;
static Int32 Lvl_TEQ_ES1[TDA18204_INSTANCE_CUSTOM_COMMON_LVL_TEQ_MAX][TDA18204_Coeff_X_Max] = TDA18204_INSTANCE_CUSTOM_COMMON_LVL_TEQ_ES1;
static Int32 Lvl_AGC1_ES2[TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC1_MAX][TDA18204_Coeff_X_Max] = TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC1_ES2;
static Int32 Lvl_AGC2WB_ES2[TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC2_MAX][TDA18204_Coeff_X_Max] = TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC2WB_ES2;
static Int32 Lvl_TEQ_ES2[TDA18204_INSTANCE_CUSTOM_COMMON_LVL_TEQ_MAX][TDA18204_Coeff_X_Max] = TDA18204_INSTANCE_CUSTOM_COMMON_LVL_TEQ_ES2;

/* used for RSSI NB */
static Int32 Lvl_MAX_GAIN_NB_ES1[TDA18204_INSTANCE_CUSTOM_COMMON_LVL_REF_GAIN_MAX][TDA18204_Coeff_Y_Max] = TDA18204_INSTANCE_CUSTOM_COMMON_LVL_REF_GAIN_MAX_NB_ES1;
static Int32 Lvl_AGC1NB_ES1[TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC1_MAX][TDA18204_Coeff_Y_Max] = TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC1NB_ES1;
static Int32 Lvl_AGC2NB_ES1[TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC2_MAX][TDA18204_Coeff_Y_Max] = TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC2NB_ES1;
static Int32 Lvl_AGC3NB_ES1[TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC3_MAX][TDA18204_Coeff_Y_Max] = TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC3NB_ES1;
static Int32 Lvl_MAX_GAIN_NB_ES2[TDA18204_INSTANCE_CUSTOM_COMMON_LVL_REF_GAIN_MAX][TDA18204_Coeff_Y_Max] = TDA18204_INSTANCE_CUSTOM_COMMON_LVL_REF_GAIN_MAX_NB_ES2;
static Int32 Lvl_AGC1NB_ES2[TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC1_MAX][TDA18204_Coeff_Y_Max] = TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC1NB_ES2;
static Int32 Lvl_AGC2NB_ES2[TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC2_MAX][TDA18204_Coeff_Z_Max] = TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC2NB_ES2;
static Int32 Lvl_AGC3NB_ES2[TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC3_MAX][TDA18204_Coeff_Y_Max] = TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC3NB_ES2;

/* Table that maps LO vs Prescaler & PostDiv values */
static UInt32 LOFracTab[23] =
{500000000,		/* LO_Frac[22] * 1000000000 */
 250000000,		/* LO_Frac[21] * 1000000000 */
 125000000,		/* LO_Frac[20] * 1000000000 */
 62500000,		/* LO_Frac[19] * 1000000000 */
 31250000,		/* LO_Frac[18] * 1000000000 */
 15625000,		/* LO_Frac[17] * 1000000000 */
 7812500,		/* LO_Frac[16] * 1000000000 */
 3906250,		/* LO_Frac[15] * 1000000000 */
 1953125,		/* LO_Frac[14] * 1000000000 */
 976563,		/* LO_Frac[13] * 1000000000 */
 488281,		/* LO_Frac[12] * 1000000000 */
 244141,		/* LO_Frac[11] * 1000000000 */
 122070,		/* LO_Frac[10] * 1000000000 */
 61035,			/* LO_Frac[9] * 1000000000 */
 30518,			/* LO_Frac[8] * 1000000000 */
 15259,			/* LO_Frac[7] * 1000000000 */
 7629,			/* LO_Frac[6] * 1000000000 */
 3815,			/* LO_Frac[5] * 1000000000 */
 1907,			/* LO_Frac[4] * 1000000000 */
 954,			/* LO_Frac[3] * 1000000000 */
 477,			/* LO_Frac[2] * 1000000000 */
 238,			/* LO_Frac[1] * 1000000000 */
 119			/* LO_Frac[0] * 1000000000 */
};

/* Driver instance */
TDA18204Object_t gTDA18204Instance[TDA18204_MAX_UNITS] = 
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
        TDA18204_INSTANCE_CUSTOM_0          /* Instance Customizable Settings */
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
        TDA18204_INSTANCE_CUSTOM_1          /* Instance Customizable Settings */
    }
};

/*============================================================================*/
/* Static internal functions:                                                 */
/*============================================================================*/

/*============================================================================*/
/* Static variables:                                                          */
/*============================================================================*/

/*============================================================================*/
/* Exported functions:                                                        */
/*============================================================================*/
/*=================================================================================================*/
/**
\brief  Opens an instance of the TDA18204 driver : inititalize the memory 
reserved for the specified unit number with the specified data structure 
containing pointer to external functions.

\param  tUnit  Concerned unit.
\param  psSrvFunc  Structure containing the Hardware Access functions, the Time functions and the Debug functions.

\return TM_OK: Succeed.
\return TDA18204_ERR_BAD_PARAMETER: Pointer to external functions is not correct.
\return TDA18204_ERR_ALREADY_SETUP: This instance has already been open previously.
\return TDA18204_ERR_ERR_NO_INSTANCES: No Memory prepared for this instance.
\return TDA18204_ERR_NOT_INITIALIZED: The memory prepared is not correct (Marker issue).
\return error codes of the functions registered in tmbslTDA18204_Open (I2C read errors, mutex...).

\note   No hardware access is done in this function.

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18204_Open(
    tmUnitSelect_t              tUnit,      /*  I: Unit number */
    tmbslFrontEndDependency_t*  psSrvFunc   /*  I: setup parameters */
)
{
    pTDA18204Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Test parameter(s) */
    if(psSrvFunc == Null)
    {
        err = TDA18204_ERR_BAD_PARAMETER;
    }

    if(err == TM_OK)
    {
        /* Get a driver instance */
        err = iTDA18204_GetInstance(tUnit, &pObj);
    }

    /* Check driver instance state */
    if(err == TM_OK || err == TDA18204_ERR_ERR_NO_INSTANCES)
    {
        if(P_OBJ_VALID && pObj->init == True)
        {
            err = TDA18204_ERR_ALREADY_SETUP;
        }
        else 
        {
            if(P_OBJ_VALID == False)
            {
                /* Try to allocate an instance of the driver */
                err = iTDA18204_AllocInstance(tUnit, &pObj);
                if(err != TM_OK || pObj == Null)
                {
                    err = TDA18204_ERR_ERR_NO_INSTANCES;
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

                tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslTDA18204_Open(0x%08X)", tUnit);

                if(err == TM_OK)
                {
                    pObj->init = True;
                }
            }
        }
    }

    return err;
}

/*=================================================================================================*/
/**
\brief  Close the instance of the TDA18204 driver: Desinititalize the memory 
reserved for the specified unit number.

\param  tUnit  Concerned unit.

\return TM_OK: Succeed.
\return TDA18204_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
\return error codes of the functions registered in tmbslTDA18204_Open (I2C read errors, mutex...).

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18204_Close(
    tmUnitSelect_t  tUnit   /*  I: Unit number */
)
{
    pTDA18204Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = iTDA18204_GetInstance(tUnit, &pObj);

    if(err == TM_OK)
    {
#ifdef _TVFE_IMPLEMENT_MUTEX
        /* Try to acquire driver mutex */
        err = iTDA18204_MutexAcquire(pObj, TDA18204_MUTEX_TIMEOUT);

        if(err == TM_OK)
        {
#endif
            tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslTDA18204_Close(0x%08X)", tUnit);

#ifdef _TVFE_IMPLEMENT_MUTEX
            P_SMUTEX_ACQUIRE = Null;

            /* Release driver mutex */
            (void)iTDA18204_MutexRelease(pObj);

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

        err = iTDA18204_DeAllocInstance(pObj);
    }

    return err;
}

/*=================================================================================================*/
/**
\brief  Get the software version of the TDA18204 driver.

\param  pSWVersion  Pointer to the software version.

\return TM_OK: Succeed.

\note   ptmSWVersion_t is defined in the tmNxTypes.h file.

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18204_GetSWVersion(
    ptmSWVersion_t  pSWVersion  /*  I: Receives SW Version */
)
{
    pSWVersion->compatibilityNr = TDA18204_COMP_NUM;
    pSWVersion->majorVersionNr  = TDA18204_MAJOR_VER;
    pSWVersion->minorVersionNr  = TDA18204_MINOR_VER;

    return TM_OK;
}

/*=================================================================================================*/
/**
\brief  Get the version of the TDA18204 configuration file.

\param  pSWSettingsVersion  Pointer to the configuration file version.

\return TM_OK: Succeed.

\note   ptmSWVersion_t is defined in the tmbslFrontEndTypes.h file.

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18204_GetSWSettingsVersion(
    ptmSWSettingsVersion_t pSWSettingsVersion   /* O: Receives SW Settings Version */
)
{
    pSWSettingsVersion->customerNr      = TDA18204_SETTINGS_CUSTOMER_NUM;
    pSWSettingsVersion->projectNr       = TDA18204_SETTINGS_PROJECT_NUM;
    pSWSettingsVersion->majorVersionNr  = TDA18204_SETTINGS_MAJOR_VER;
    pSWSettingsVersion->minorVersionNr  = TDA18204_SETTINGS_MINOR_VER;

    return TM_OK;
}

/*=================================================================================================*/
/**
\brief  Read the ID of the TDA18204 (via register access), check it is correct and return ES version

\param  tUnit  Concerned unit.
\param  pEsVersion  Pointer to the ES hardware version.

\return TM_OK: Succeed.
\return TDA18204_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
\return TDA18204_ERR_BAD_MODULE_ID Read version is not OK.
\return error codes of the functions registered in tmbslTDA18204_Open (I2C read errors, mutex...).

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18204_CheckHWVersion(
    tmUnitSelect_t tUnit,    /* I: Unit number */
    pTDA18204EsVersion_t pEsVersion  /*  o: ES version of the HW */
)
{
    pTDA18204Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;
    UInt16              uIdentity = 0;
    UInt8               ID_byte_1 = 0;
    UInt8               ID_byte_2 = 0;
    UInt8               majorRevision = 0;
    UInt8               minorRevision = 0;


    /* Get a driver instance */
    err = iTDA18204_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18204)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslTDA18204_CheckHWVersion(0x%08X)", tUnit);
    if(err == TM_OK)
    {
        err = iTDA18204_ReadRegMap(pObj, gTDA18204_Reg_ID_1.Address, TDA18204_REG_DATA_LEN(gTDA18204_Reg_ID_1__IDENT_1, gTDA18204_Reg_ID_3));
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_ReadRegMap(0x%08X) failed.", tUnit));
    }

    if(err == TM_OK)
    {
        err = iTDA18204_Read(pObj, &gTDA18204_Reg_ID_1__IDENT_1, &ID_byte_1, Bus_None);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", tUnit));
    }

    if(err == TM_OK)
    {
        err = iTDA18204_Read(pObj, &gTDA18204_Reg_ID_2__IDENT_2, &ID_byte_2, Bus_None);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", tUnit));
    }

    if(err == TM_OK)
    {
        /* Construct Identity */
        uIdentity = (ID_byte_1 << 8) | ID_byte_2;

        if(uIdentity == 18204)
        {
            /* TDA18204 found. Check Major & Minor Revision*/
            err = iTDA18204_Read(pObj, &gTDA18204_Reg_ID_3__MAJOR_REV, &majorRevision, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", tUnit));

            err = iTDA18204_Read(pObj, &gTDA18204_Reg_ID_3__MINOR_REV, &minorRevision, Bus_None);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", tUnit));

            switch (majorRevision)
            {
                case 1:
                {
                    switch (minorRevision)
                    {
                        case 0:
                            /* TDA18204 ES1_A Full programmable */
                            err = TM_OK;
                            *pEsVersion = TDA18204_EsVersion1;
                            break;

                        case 1:
                            /* TDA18204 ES1_B Performance optimized */
                            err = TM_OK;
                            *pEsVersion = TDA18204_EsVersion1;
                            break;

                        case 2:
                            /* TDA18204 ES1 MF Performance optimized */
                            err = TM_OK;
                            *pEsVersion = TDA18204_EsVersion1;
                            break;

                        default:
                            /* Only TDA18204 ES1 are supported */
                            err = TDA18204_ERR_BAD_VERSION;
                            break;
                    }
                }
                break;

                case 2:
                {
                    switch (minorRevision)
                    {
                        case 0:
                            /* TDA18204 ES2 */
                            err = TM_OK;
                            *pEsVersion = TDA18204_EsVersion2;
                            break;

                        default:
                            /* Only TDA18204 ES1 are supported */
                            err = TDA18204_ERR_BAD_VERSION;
                            break;
                    }
                }
                break;

                default:
                    /* Only TDA18204 ES1 are supported */
                    err = TDA18204_ERR_BAD_VERSION;
                    break;
            }
        }
        else
        {
            err = TDA18204_ERR_BAD_VERSION;
        }
    }

    _MUTEX_RELEASE(TDA18204)

    return err;
}

/*=================================================================================================*/
/**
    \brief  Set the power state.

    \param  tUnit  Concerned unit.
    \param  tPowerState  Power state.

    \return TM_OK Succeed.
    \return TDA18204_ERR_ERR_NO_INSTANCES No instances available.
    \return TDA18204_ERR_BAD_PARAMETER Parameter is not correctly filled.
    \return error codes of the functions registered in tmbslTDA18204_Open (I2C read errors, mutex...).

    \note   NA
    
    \sa     NA
 */
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18204_SetPowerState(
    tmUnitSelect_t        tUnit,       /* I: Unit number */
    tmPowerState_t        tPowerState  /* I: Power state */
)
{
    pTDA18204Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = iTDA18204_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18204)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslTDA18204_SetPowerState(0x%08X)", tUnit);

    if(tPowerState>=tmPowerMax)
    {
        err = TDA18204_ERR_BAD_PARAMETER;
    }

    if(err == TM_OK)
    {
        if(pObj->mapLLPowerState[tPowerState] != pObj->curLLPowerState)
        {
            err = iTDA18204_SetLLPowerState(pObj, pObj->mapLLPowerState[tPowerState]);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_SetLLPowerState(0x%08X, %d) failed.", tUnit, (int)pObj->mapLLPowerState[tPowerState]));
            
            if(err == TM_OK)
            {
                /* Store power state in driver instance */
                pObj->curPowerState = tPowerState;
            }
        }
    }

    _MUTEX_RELEASE(TDA18204)

    return err;
}

/*=================================================================================================*/
/**
    \brief  Get the power state.

    \param  tUnit  Concerned unit.
    \param  pPowerState  Pointer to the power state.

    \return TM_OK Succeed
    \return TDA18204_ERR_ERR_NO_INSTANCES No instances available.
    \return TDA18204_ERR_BAD_PARAMETER Parameter is not correctly filled.
    \return error codes of the functions registered in tmbslTDA18204_Open (I2C read errors, mutex...).

    \note   NA
    
    \sa     NA
 */
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18204_GetPowerState(
    tmUnitSelect_t         tUnit,       /* I: Unit number */
    tmPowerState_t*        pPowerState  /* O: Power state */
)
{
    pTDA18204Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = iTDA18204_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18204)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslTDA18204_GetPowerState(0x%08X)", tUnit);

    /* Test parameter(s) */
    if(pPowerState == Null)
    {
        err = TDA18204_ERR_BAD_PARAMETER;
    }

    if(err == TM_OK)
    {
        *pPowerState = pObj->curPowerState;
    }

    _MUTEX_RELEASE(TDA18204)

    return err;
}

/*=================================================================================================*/
/**
    \brief  Sets the low-level power state of the TDA18204.

    \param  tUnit  Concerned unit.
    \param  tPowerState  Power state that needs to be set. 
							Possible enumeration values are:
								TDA18204_PowerNormalMode
								TDA18204_PowerStandbyWithXtalOn
								TDA18204_PowerStandbyWithLtOnWithXtalOn
								TDA18204_PowerStandbyWithPllOnWithXtalOn
								TDA18204_PowerStandbyWithLtOnWithPllOnWithXtalOn

			Current mapping with tmPowerState_t parameter of tmbslTDA18204_SetPowerState():
								tmPowerOn = TDA18204_PowerNormalMode
								tmPowerStandby = TDA18204_PowerStandbyWithXtalOn
								tmPowerSuspend = TDA18204_PowerStandbyWithXtalOn
								tmPowerOff = TDA18204_PowerStandbyWithXtalOn

    \return TM_OK Succeed.
    \return TDA18204_ERR_ERR_NO_INSTANCES No instances available.
    \return TDA18204_ERR_BAD_PARAMETER Parameter is not correctly filled.
    \return error codes of the functions registered in tmbslTDA18204_Open (I2C read errors, mutex...).

    \note   NA
    
    \sa     NA
 */
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18204_SetLLPowerState(
    tmUnitSelect_t        tUnit,       /* I: Unit number */
    TDA18204PowerState_t  tPowerState  /* I: Power state of TDA18204 */
)
{
	pTDA18204Object_t   pObj = Null;
	tmErrorCode_t       err = TM_OK;

	/* Get a driver instance */
	err = iTDA18204_GetInstance(tUnit, &pObj);

	_MUTEX_ACQUIRE(TDA18204)

	tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslTDA18204_SetLLPowerState(0x%08X)", tUnit);


	/* tmbslTDA18204_HwInit() has never been called */
	if (pObj->eHwState == TDA18204_HwState_InitNotDone)
	{
		err = TDA18204_ERR_NOT_INITIALIZED;
	}
	/* normal case */
	else
	{
		pObj->curPowerState = tmPowerMax;

		err = iTDA18204_SetLLPowerState(pObj, tPowerState);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_SetLLPowerState(0x%08X, %d) failed.", tUnit, (int)tPowerState));
	}

	_MUTEX_RELEASE(TDA18204)

		return err;
}

/*=================================================================================================*/
/**
    \brief  Gets the low-level power state of the TDA18204.

    \param  tUnit  Concerned unit.
    \param  pPowerState  Pointer to the object in which the read power state must be stored. 
						 Possible return values are the same as tmbslTDA18204_SetLLPowerState.

    \return TM_OK Succeed
    \return TDA18204_ERR_ERR_NO_INSTANCES No instances available.
    \return TDA18204_ERR_BAD_PARAMETER Parameter is not correctly filled.
    \return error codes of the functions registered in tmbslTDA18204_Open (I2C read errors, mutex...).

    \note   NA
    
    \sa     NA
 */
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18204_GetLLPowerState(
    tmUnitSelect_t         tUnit,       /* I: Unit number */
    TDA18204PowerState_t*  pPowerState /* O: Power state of TDA18204 */
)
{
	pTDA18204Object_t   pObj = Null;
	tmErrorCode_t       err = TM_OK;

	/* Get a driver instance */
	err = iTDA18204_GetInstance(tUnit, &pObj);

	_MUTEX_ACQUIRE(TDA18204)

		tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslTDA18204_GetLLPowerState(0x%08X)", tUnit);

	/* Test parameter(s) */
	if(pPowerState == Null)
	{
		err = TDA18204_ERR_BAD_PARAMETER;
	}

	if(err == TM_OK)
	{
		*pPowerState = pObj->curLLPowerState;
	}

	_MUTEX_RELEASE(TDA18204)

		return err;
}

/*=================================================================================================*/
/**
    \brief  Sets the TDA18204 standard mode preset.

    \param  tUnit  Concerned unit.
    \param  tStandardMode  Standard mode to be set. Possible values are:
                            TDA18204_TUNER_QAM_6MHz
                            TDA18204_TUNER_QAM_8MHz
                            TDA18204_TUNER_OOB_2MHz

    \return TM_OK Succeed.
    \return TDA18204_ERR_ERR_NO_INSTANCES No instances available.
    \return TDA18204_ERR_BAD_PARAMETER Parameter is not correctly filled.
    \return error codes of the functions registered in tmbslTDA18204_Open (I2C read errors, mutex...).

    \note   NA
    
    \sa     NA
 */
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18204_SetStandardMode(
    tmUnitSelect_t            tUnit,           /*  I: Unit number */
    TDA18204StandardMode_t    tStandardMode    /*  I: Standard mode of this device */
)
{
	pTDA18204Object_t           pObj = Null;
	tmErrorCode_t               err = TM_OK;

	/* Get a driver instance */
	err = iTDA18204_GetInstance(tUnit, &pObj);

	_MUTEX_ACQUIRE(TDA18204)

		tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslTDA18204_SetStandard(0x%08X)", tUnit);

	/* Check if Hw is ready to operate */
	err = iTDA18204_CheckHwState(pObj, TDA18204_HwStateCaller_SetStd);
	tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_CheckHwState(0x%08X) failed.", pObj->tUnitW));

	if(err == TM_OK)
	{
		/* Store standard mode */
		pObj->StandardMode = tStandardMode;

		/* Reset standard map pointer */
		pObj->pStandard = Null;

		if(pObj->StandardMode>TDA18204_StandardMode_Unknown && pObj->StandardMode<TDA18204_StandardMode_Max)
		{
			/* Update standard map pointer */
			pObj->pStandard = &pObj->Std_Array[pObj->StandardMode-1];

			/****************************************************************/
			/* IF SELECTIVITY Settings                                      */
			/****************************************************************/

			/* LP_Fc */
			err = iTDA18204_Write(pObj, &gTDA18204_Reg_IF_Byte_1__LP_Fc, (UInt8)(pObj->pStandard->LPF), Bus_None);
			tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));

			if(err == TM_OK)
			{
				/* IF_LPF_Offset */
				err = iTDA18204_Write(pObj, &gTDA18204_Reg_IF_Byte_1__IF_LPF_Offset, (UInt8)(pObj->pStandard->LPF_Offset & 0x03), Bus_None);
				tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
			}
			if(err == TM_OK)
			{
				/* IF_LPF_sign */
				err = iTDA18204_Write(pObj, &gTDA18204_Reg_IF_Byte_1__IF_LPF_sign, (UInt8)(pObj->pStandard->LPF_Offset >>4), Bus_NoRead);
				tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
			}
			if(err == TM_OK)
			{
				/* DC_Notch */
				err = iTDA18204_Write(pObj, &gTDA18204_Reg_IR_Mixer_byte_1__DC_Notch, pObj->pStandard->DC_Notch_IF_PPF, Bus_NoRead);
				tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
			}
			if(err == TM_OK)
			{
				/* IF_HPF_Fc */
				err = iTDA18204_Write(pObj, &gTDA18204_Reg_IF_Byte_2__IF_HP_Fc, (UInt8)(pObj->pStandard->HPF), Bus_None);
				tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
			}            
			if(err == TM_OK)
			{
				/* IF_HPF_Offset */
				err = iTDA18204_Write(pObj, &gTDA18204_Reg_IF_Byte_2__IF_HPF_Offset, (UInt8)(pObj->pStandard->HPF_Offset & 0x03), Bus_None);
				tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
			}
			if(err == TM_OK)
			{
				/* IF_HPF_sign */
				err = iTDA18204_Write(pObj, &gTDA18204_Reg_IF_Byte_2__IF_HPF_sign, (UInt8)(pObj->pStandard->HPF_Offset >>4), Bus_None);
				tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
			}

			if(err == TM_OK)
			{
				/* IF_HPF_Enable */
				err = iTDA18204_Write(pObj, &gTDA18204_Reg_IF_Byte_2__IF_HP_Enable, (UInt8)(pObj->pStandard->HPF_Enable), Bus_NoRead);
				tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
			}
			if(err == TM_OK)
			{
				/* IF Output Level */
				err = iTDA18204_Write(pObj, &gTDA18204_Reg_IF_AGC_byte__IF_level, pObj->pStandard->IF_Output_Level, Bus_NoRead);
				tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
			}

            /************************************/
            /***  set AGC2_NB_TOP UP and DOWN  **/
            /************************************/
            if( err == TM_OK )
            {
                err = iTDA18204_SetAGC2NBTOP(pObj);
            }

            if(err == TM_OK)
            {
                /* AGC2_Det_Speed */
                err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_NB_byte_3__AGC2_NB_Det_Speed, pObj->pStandard->AGC2_DET_SPEED, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
			}
			if(err == TM_OK)
			{
				/* Det12_Cint */
				err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGCK_byte_1__DET12_Cint, pObj->pStandard->DET12_CINT, Bus_RW);
				tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
			}
			if(err == TM_OK)
			{
				/* AGC3_TOP_DO */
				err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC3_NB_byte_1__AGC3_NB_TOP_DO, 117-pObj->pStandard->AGC3_TOP_DN, Bus_NoRead);
				tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
			}
			if(err == TM_OK)
			{
				/* AGC3_TOP_UP */
				err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC3_NB_byte_1__AGC3_NB_TOP_UP, 117-pObj->pStandard->AGC3_TOP_UP, Bus_NoRead);
				tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
			}

			/****************************************************************/
			/* GSK Settings                                                 */
			/****************************************************************/
			if(err == TM_OK)
			{
				/* RSSI HP FC */
				err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC3_NB_byte_3__RSSI_HP_Fc, pObj->pStandard->RSSI_HP_FC, Bus_RW);
				tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
			}

			/****************************************************************/
			/* RSSI Settings                                                */
			/****************************************************************/

			if(err == TM_OK)
			{
				/* RSSI_Cap_Val */
				err = iTDA18204_Write(pObj, &gTDA18204_Reg_RSSI_byte_2__RSSI_Cap_Val, pObj->pStandard->RSSI_Cap_Val, Bus_NoRead);
				tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
			}
			if(err == TM_OK)
			{
				/* RSSI_Ck_Speed */
				err = iTDA18204_Write(pObj, &gTDA18204_Reg_RSSI_byte_2__RSSI_Ck_Speed, pObj->pStandard->RSSI_Ck_Speed, Bus_NoRead);
				tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
			}
		}

		/* Update driver state machine */
		pObj->eHwState = TDA18204_HwState_SetStdDone;
	}

	_MUTEX_RELEASE(TDA18204)

		return err;
}

/*=================================================================================================*/
/**
\brief  Gets the TDA18204 standard mode preset.

\param  tUnit  Concerned unit.
\param  pStandard  Pointer to the object in which the standard mode preset must be stored. 
						Possible return values are the same as tmbslTDA18204_SetStandard.

\return TM_OK Succeed.
\return TDA18204_ERR_ERR_NO_INSTANCES No instances available.
\return TDA18204_ERR_BAD_PARAMETER Parameter is not correctly filled.
\return error codes of the functions registered in tmbslTDA18204_Open (I2C read errors, mutex...).

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18204_GetStandardMode(
    tmUnitSelect_t            tUnit,       /*  I: Unit number */
    TDA18204StandardMode_t*   pStandard    /*  O: Standard mode of this device */
)
{
    pTDA18204Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = iTDA18204_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18204)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslTDA18204_GetStandard(0x%08X)", tUnit);

    if(pStandard == Null)
    {
        err = TDA18204_ERR_BAD_PARAMETER;
    }

    if(err == TM_OK)
    {
        /* Get standard mode */
        *pStandard = pObj->StandardMode;
    }

    _MUTEX_RELEASE(TDA18204)

    return err;
}

/*=================================================================================================*/
/**
\brief  Sets the TDA18204 to a specified RF frequency.

\param  tUnit  Concerned unit.
\param  uRF  Frequency to set in Hertz.

\return TM_OK Succeed.
\return TDA18204_ERR_ERR_NO_INSTANCES No instances available.
\return TDA18204_ERR_BAD_PARAMETER Parameter is not correctly filled.
\return error codes of the functions registered in tmbslTDA18204_Open (I2C read errors, mutex...).

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18204_SetRF(
    tmUnitSelect_t  tUnit,  /*  I: Unit number */
    UInt32          uRF     /*  I: RF frequency in hertz */
)
{
    pTDA18204Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = iTDA18204_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18204)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslTDA18204_SetRF(0x%08X)", tUnit);

    /* Test parameter(s) */
    if(   pObj->StandardMode<=TDA18204_StandardMode_Unknown
       || pObj->StandardMode>=TDA18204_StandardMode_Max
       || pObj->pStandard == Null)
    {
        err = TDA18204_ERR_STD_NOT_SET;
    }

    if(err == TM_OK)
    {
        /* Check if Hw is ready to operate */
        err = iTDA18204_CheckHwState(pObj, TDA18204_HwStateCaller_SetRF);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_CheckHwState(0x%08X) failed.", pObj->tUnitW));
    }
	if(err == TM_OK)
	{
		pObj->uRF = uRF;

		err = iTDA18204_SetRF(pObj);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_SetRF(0x%08X) failed.", tUnit));

		if(err == TM_OK)
		{
			/* Update driver state machine */
			pObj->eHwState = TDA18204_HwState_SetRFDone;
		}
	}

	_MUTEX_RELEASE(TDA18204)

    return err;
}

/*=================================================================================================*/
/**
\brief  Gets the last set RF frequency.

\param  tUnit  Concerned unit.
\param  pRF  Pointer to the object in which the RF frequency in Hz must be stored.

\return TM_OK Succeed.
\return TDA18204_ERR_ERR_NO_INSTANCES No instances available.
\return TDA18204_ERR_BAD_PARAMETER Parameter is not correctly filled.
\return error codes of the functions registered in tmbslTDA18204_Open (I2C read errors, mutex...).

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18204_GetRF(
    tmUnitSelect_t  tUnit,  /*  I: Unit number */
    UInt32*         pRF     /*  O: RF frequency in hertz */
)
{
    pTDA18204Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = iTDA18204_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18204)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslTDA18204_GetRF(0x%08X)", tUnit);

    if(pRF == Null)
    {
        err = TDA18204_ERR_BAD_PARAMETER;
    }

    if(err == TM_OK)
    {
        /* Get RF */
        *pRF = pObj->uRF;
    }

    _MUTEX_RELEASE(TDA18204)

    return err;
}

/*=================================================================================================*/
/**
\brief  Perform the hardware Initialization of the TDA18204.

\param  tUnit  Concerned unit.

\return TM_OK: Succeed.
\return TDA18204_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
\return TDA18204_ERR_BAD_PARAMETER: Parameters out of range.
\return Other optional return codes of the functions registered 
in DeviceOpenInstance API (I2C read errors, mutex...).

\note   To be performed once tmbslTDA18204_Open has been called.

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18204_HwInit(
    tmUnitSelect_t tUnit    /* I: Unit number */
)
{
	pTDA18204Object_t   pObj = Null;
	tmErrorCode_t       err = TM_OK;
	UInt8               ID_byte_1 = 0;
	UInt8               ID_byte_2 = 0;
    UInt8               majorRevision = 0;
    UInt8               minorRevision = 0;
	UInt32              counter = 10; /* Wait I2C ready */
	UInt8               uValue = 0;

	/* Get a driver instance */
	err = iTDA18204_GetInstance(tUnit, &pObj);

	_MUTEX_ACQUIRE(TDA18204)

		tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslTDA18204_HwInit(0x%08X)", tUnit);

	/* Reset Hw State */
	pObj->eHwState = TDA18204_HwState_InitNotDone;

	/* first check that I2C responds and retrieve ID */
	while(counter > 0)
	{
		/* patch : make a first read to allo I2C work properly */
		err = iTDA18204_Read(pObj, &gTDA18204_Reg_ID_1__IDENT_1, &ID_byte_1, Bus_NoWrite);

        if(err == TM_OK)
        {
            err = iTDA18204_ReadRegMap(pObj, gTDA18204_Reg_ID_1.Address, TDA18204_REG_DATA_LEN(gTDA18204_Reg_ID_1__IDENT_1, gTDA18204_Reg_ID_2));
            // this function might return an error but that does not mean the process
            // tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_ReadRegMap(0x%08X) failed.", tUnit));
        }

		if(err == TM_OK)
		{
			/* IC I2C responds */
			break;
		}

		if(counter)
		{
			/* Decrease the counter */
			counter--;

			/* Wait for a step of  1ms */
			err = iTDA18204_Wait(pObj, 1);
			tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
		}
	}

	/****************************************************/
	/* Retrieve ID                                      */
	/****************************************************/
	if(err == TM_OK)
	{
		err = iTDA18204_Read(pObj, &gTDA18204_Reg_ID_1__IDENT_1, &ID_byte_1, Bus_NoWrite);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", tUnit));
	}

	if(err == TM_OK)
	{
		err = iTDA18204_Read(pObj, &gTDA18204_Reg_ID_2__IDENT_2, &ID_byte_2, Bus_NoWrite);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", tUnit));
	}

	if(err == TM_OK)
	{
		/* Construct Identity */
		pObj->Identity = (UInt16)((ID_byte_1 << 8) | ID_byte_2);
	}

	/****************************************************/
	/* Retrieve HW revision                             */
	/****************************************************/
	if(err == TM_OK)
	{
		err = iTDA18204_Read(pObj, &gTDA18204_Reg_ID_3__MAJOR_REV, &majorRevision, Bus_NoWrite);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", tUnit));
	}

	if(err == TM_OK)
	{
		err = iTDA18204_Read(pObj, &gTDA18204_Reg_ID_3__MINOR_REV, &minorRevision, Bus_NoWrite);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", tUnit));
	}

	if(err == TM_OK)
	{
		/* Construct Revision */
		/* ES1   : majorRevision = 1   -    minorRevision = 0  => pObj->Revision = 0x10 */
		/* ES1MF : majorRevision = 1   -    minorRevision = 1  => pObj->Revision = 0x11 */
		/* ES2   : majorRevision = 2   -    minorRevision = 0  => pObj->Revision = 0x20 */
		pObj->Revision = (UInt8)((majorRevision << 4) | minorRevision);
    }

	/**********************/
	/*      TDA18204      */
	/**********************/
	if(pObj->Identity == 18204)
	{
		/****************************************************/
		/* Change POR default values                        */
		/****************************************************/
        /* Reset all flags */
        pObj->bAgc2Active = False;

        /* change POR default values */

        /****************************************************************/
        /* AGC3                                                         */
        /****************************************************************/
        if(err == TM_OK)
        {
            /* keep AGC3 switch off, but set its value to 11dB */
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC3_GAIN_WB__AGC3_GAIN, TDA18204_AGC3_GAIN_11_VALUE_3, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC3_NB_byte_3__AGC3_NB_Do_step, TDA18204_AGC3_NB_Do_step_2_044_VALUE_1, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC3_NB_byte_3__AGC3_NB_Up_step, TDA18204_AGC3_NB_Up_step_66_VALUE_2, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }

        /****************************************************************/
        /* AGK                                                          */
        /****************************************************************/
        if(err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGCK_byte_1__AGCK_Mode, TDA18204_AGCK_Mode_8_192_VALUE_2, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
        
        /****************************************************************/
        /* AGC2 NB                                                      */
        /****************************************************************/
        if(err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_NB_byte_1__AGC2_NB_Up_step, TDA18204_AGC2_NB_Up_step_66_VALUE_2, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_NB_byte_1__AGC2_NB_Do_step, TDA18204_AGC2_NB_Do_step_8_192_VALUE_0, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }

        /****************************************************************/
        /* AGC1                                                         */
        /****************************************************************/
        if(err == TM_OK)
        {
            /* Set AGC1 TOP I2C DN/UP */
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC1_byte_1__AGC1_Top, pObj->agc1TopDownUp, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* Set AGC1_Top_Strategy */
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC1_byte_1__AGC1_Top_Strategy, pObj->agc1TopStrategy, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* Set AGC1_Det_Speed */
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC1_byte_1__AGC1_Det_Speed, pObj->agc1DetSpeed, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* LNA Range */
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC1_byte_2__LNA_Range, TDA18204_LNA_Range_RANGE_MINUS8DB_PLUS22DB_VALUE_1, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* LNA Zin -> Standard */
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC1_byte_2__LNA_Zin, 0x1, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* Set time constant for the AGC1 switch control */
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC1_byte_4__AGC1_smooth_t_cst, pObj->agc1TimeCst, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }

        if(err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_IR_Cal_byte_3__IR_target, TDA18204_IR_target_61_ES1_MINUS_67_ES2_VALUE_3, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }

		/* ES2 specific */
		if ( pObj->Revision == 0x20)
		{
			if(err == TM_OK)
			{
				err = iTDA18204_Write(pObj, &gTDA18204_Reg_H3H5_byte__H3H5_Dcboost, 0x0, Bus_NoRead);
				tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
			}
		}

		/* ES1, ES1MF, ES2 */
		if(err == TM_OK)
		{
			err = iTDA18204_Write(pObj, &gTDA18204_Reg_IR_Mixer_byte_1__DC_Notch, 0x0, Bus_NoRead);
			tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
		}
		if(err == TM_OK)
		{
			err = iTDA18204_Write(pObj, &gTDA18204_Reg_Charge_pump_byte__ICP_low, 0x0, Bus_NoRead);
			tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
		}
		if(err == TM_OK)
		{
			err = iTDA18204_Write(pObj, &gTDA18204_Reg_Charge_pump_byte__ICP_select, TDA18204_ICP_select_100_VALUE_3, Bus_NoRead);
			tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
		}

		if(err == TM_OK)
		{
			err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC3_NB_byte_3__DET3_LowBW, 0x0, Bus_RW);
			tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
		}
		if(err == TM_OK)
		{
			err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_NB_byte_3__AGC2_Gain_Control_En, 0x0, Bus_NoRead);
			tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
		}

		/* Write the WB AGC2 TOP (write TOP UP before TOP DOWN) */
		/* Change default value of AGC2_G_UP : POR = 9 -> 16 */
		if(err == TM_OK)
		{
			err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_G_UP__AGC2_G_UP, pObj->agc2WBGud[TDA18204_AGC2_Gud_Up], Bus_NoRead);
			tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
		}
		/* Change default value of AGC2_G_DOWN : POR = 8 -> 15 */
		if(err == TM_OK)
		{
			err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_G_DOWN__AGC2_G_DOWN, pObj->agc2WBGud[TDA18204_AGC2_Gud_Down], Bus_NoRead);
			tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
		}

		/* Set time constant for the LT switch control */
		if(err == TM_OK)
		{
			err = iTDA18204_Write(pObj, &gTDA18204_Reg_LT_byte_2__LT_smooth_t_cst, pObj->ltTimeCst, Bus_NoRead);
			tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
		}

        /* set AGC2 WB gain change speed : AGC2_DOWN_STEP and AGC2_UP_STEP */
        if(err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_STEP__AGC2_UP_STEP, pObj->agc2UpStep, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_STEP__AGC2_DOWN_STEP, pObj->agc2DownStep, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }

		/* Read all bytes */
		err = iTDA18204_ReadRegMap(pObj, 0x00, TDA18204_REG_MAP_NB_BYTES);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_ReadRegMap(0x%08X) failed.", tUnit));

		/********************************/
		/* force DIG clock to INTERNAL  */
		/********************************/		
		/* Check if Dig_Clock is in INTERNAL_OSCILLATOR, if not set it */
		err = iTDA18204_Read(pObj, &gTDA18204_Reg_clock_ctrl_byte_1__Dig_Clock, &uValue, Bus_NoWrite);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", pObj->tUnitW));

		if ( (err == TM_OK) && (uValue != TDA18204_Dig_Clock_INTERNAL_OSCILLATOR_VALUE_0))
		{
			err = iTDA18204_Write(pObj, &gTDA18204_Reg_clock_ctrl_byte_1__Dig_Clock, TDA18204_Dig_Clock_INTERNAL_OSCILLATOR_VALUE_0, Bus_NoRead);
			tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
		}

		/*****************/
		/*  XTal config  */
		/*****************/	
		if(err == TM_OK)
		{
			/* Set XTAL_IN_CTRL */
			err = iTDA18204_Write(pObj, &gTDA18204_Reg_XTAL_IN_CTRL__XTAL_IN_CTRL, (UInt8)pObj->XTin, Bus_NoRead);
			tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
			/* Set XTAL_IN_CTRL */
			err = iTDA18204_Write(pObj, &gTDA18204_Reg_XTAL_OUT_CTRL__XTout, (UInt8)pObj->XTOut, Bus_NoRead);
			tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.",  pObj->tUnitW));
		}

		/*********************************************************************/
		/*  Patch : enable all IRQ to allows digital working without sub LO  */
		/*********************************************************************/		
		if(err == TM_OK)
		{
			err = iTDA18204_Write(pObj, &gTDA18204_Reg_IRQ_enable, 0xFF, Bus_NoRead);
			tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
		}

		/****************************************************/
		/* wake-up tuner                                    */
		/****************************************************/
		if(err == TM_OK)
		{
			/* Set power state TDA18204_PowerCalibrationNormalMode */
			err = iTDA18204_SetLLPowerState(pObj, TDA18204_PowerCalibrationNormalMode);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_SetLLPowerState(0x%08X, TDA18204_PowerNbNormalMode) failed.", tUnit));
		}
		if(err == TM_OK)
		{
			pObj->curPowerState = tmPowerOn;
		}

		if(err == TM_OK)
		{
			/* Inform that init phase has started */
			if(err == TM_OK)
			{
				pObj->eHwState = TDA18204_HwState_InitPending;
			}

			/****************************************************/
			/* Power Saving Mode                                */
			/****************************************************/
			if(err == TM_OK)
			{
				err = iTDA18204_PowerSavingMode(pObj, pObj->PowerSavingMode);
			}

			/****************************************************/
			/* Calibrations                                     */
			/****************************************************/
			/****************************************************/
			/* Xtal calibration                                 */
			/****************************************************/
			if ( (pObj->XTin == TDA18204_XTAL_IN_CTRL_XTAL_27MHZ_VALUE_7) || (pObj->XTin == TDA18204_XTAL_IN_CTRL_XTAL_16MHZ_VALUE_6) )
			{
				if(err == TM_OK)
				{
					err = iTDA18204_XtalCalibration(pObj);
				}
			}

			/****************************************************/
			/* RC calibration                                   */
			/****************************************************/
			if(err == TM_OK)
			{
				err = iTDA18204_RCCalibration(pObj);
			}

			/****************************************************/
			/* IR calibration                                   */
			/****************************************************/
			if(err == TM_OK)
			{
				err = iTDA18204_IRCalibration(pObj);
			}

			/****************************************************/
			/* FVCOmax calibration                              */
			/****************************************************/
			if(err == TM_OK)
			{
				err = iTDA18204_FVCOMaxCalibration(pObj);
			}

            if (err == TM_OK)
            {
                /* boost VCO OFF */
                err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_SAVING_MODE_1__BOOST_VCO, 0x0, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
            }

			if(err == TM_OK && pObj->eHwState == TDA18204_HwState_InitPending)
			{
				pObj->eHwState = TDA18204_HwState_InitDone;
			}
			else
			{
				pObj->eHwState = TDA18204_HwState_InitNotDone;
			}

			/****************************************************/
			/*  put Tuner in BufMTO/LTO Stand-By                */
			/****************************************************/
			if(err == TM_OK)
			{
				/* Set power state TDA18204_PowerBufMtoLtoStandby */
				err = iTDA18204_SetLLPowerState(pObj, TDA18204_PowerBufMtoLtoStandby);
				tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_SetLLPowerState(0x%08X, PowerNormalMode) failed.", tUnit));
			}
			if(err == TM_OK)
			{
				pObj->curPowerState = tmPowerStandby;
			}

		}
	}
	else if(pObj->Identity == 18202)
	{
		/* ... */
	}

	_MUTEX_RELEASE(TDA18204)

		return err;
}

/*=================================================================================================*/
/**
\brief  Gets the IF Frequency set in TDA18204 driver.

\param  tUnit  Concerned unit.
\param  pIF  Pointer to the object in which the IF Frequency in Hz must be stored.

\return TM_OK: Succeed.
\return TDA18204_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
\return TDA18204_ERR_BAD_PARAMETER: Parameters out of range.
\return Other optional return codes of the functions registered 
in DeviceOpenInstance API (I2C read errors, mutex...).

\note   To be performed once tmbslTDA18204_Open has been called.

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18204_GetIF(
    tmUnitSelect_t  tUnit,  /* I: Unit number */
    UInt32*         pIF     /* O: IF Frequency in hertz */
)
{
    pTDA18204Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = iTDA18204_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18204)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslTDA18204_GetIF(0x%08X)", tUnit);

    /* Test parameter(s) */
    if(   pObj->StandardMode<=TDA18204_StandardMode_Unknown
        || pObj->StandardMode>=TDA18204_StandardMode_Max
        || pObj->pStandard == Null
        || pIF == Null)
    {
        err = TDA18204_ERR_BAD_PARAMETER;
    }

    if(err == TM_OK)
    {
        *pIF = pObj->pStandard->uIF;
    }

    _MUTEX_RELEASE(TDA18204)

    return err;
}

/*=================================================================================================*/
/**
\brief  Gets the PLL lock status of the TDA18204.

\param  tUnit  Concerned unit.
\param  pLockStatus  Pointer to the object in which the lock status must be stored. 
					 Available values are:
						tmbslFrontEndStateUnknown
						tmbslFrontEndStateLocked
						tmbslFrontEndStateNotLocked
						tmbslFrontEndStateSearching

\return TM_OK: Succeed.
\return TDA18204_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
\return TDA18204_ERR_BAD_PARAMETER: Parameters out of range.
\return Other optional return codes of the functions registered 
in DeviceOpenInstance API (I2C read errors, mutex...).

\note   To be performed once tmbslTDA18204_Open has been called.

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18204_GetLockStatus(
    tmUnitSelect_t          tUnit,      /* I: Unit number */
    tmbslFrontEndState_t*   pLockStatus /* O: PLL Lock status */
)
{
	pTDA18204Object_t   pObj = Null;
	tmErrorCode_t       err = TM_OK;
	UInt8               uLO_Lock = 0;

    /* Get a driver instance */
    err = iTDA18204_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18204)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslTDA18204_GetLockStatus(0x%08X)", tUnit);

    if( pLockStatus == Null )
    {
        err = TDA18204_ERR_BAD_PARAMETER;
    }

    if(err == TM_OK)
    {
        err = iTDA18204_Read(pObj, &gTDA18204_Reg_POWER_STATUS__LO_Lock, &uLO_Lock, Bus_RW);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", pObj->tUnitW));

        if(err == TM_OK)
        {
            *pLockStatus =  (uLO_Lock)? tmbslFrontEndStateLocked : tmbslFrontEndStateNotLocked;
        }
        else
        {
            *pLockStatus = tmbslFrontEndStateUnknown;
        }
    }

    _MUTEX_RELEASE(TDA18204)

    return err;
}

/*=================================================================================================*/
/**
\brief  Calculate Gain

\param  tUnit  Concerned unit.
\param  uRF  RF frequency in hertz.
\param  pGain  Pointer to the object in which the Gain value in dB (x1000) must be stored.

\return TM_OK: Succeed.
\return TDA18204_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
\return TDA18204_ERR_BAD_PARAMETER: Parameters out of range.
\return Other optional return codes of the functions registered 
in DeviceOpenInstance API (I2C read errors, mutex...).

\note   To be performed once tmbslTDA18204_Open has been called.

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18204_CalculateGain(
    tmUnitSelect_t  tUnit,       /* I: Unit number */
    UInt32          uRF,         /* I: RF frequency in hertz */
    Int32*          pGain        /* O: Gain value in dB (x1000) */
)
{
    pTDA18204Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = iTDA18204_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18204)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslTDA18204_CalculateGain(0x%08X)", tUnit);

    /* set uRF in MHz for internal function */
    uRF = uRF/1000000;

    if(err == TM_OK)
    {
        err = iTDA18204_CalculateGain(pObj, uRF, pGain);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_CalculateGain(0x%08X) failed.", pObj->tUnitW));
    }

    _MUTEX_RELEASE(TDA18204)

    return err;
}

/*=================================================================================================*/
/**
\brief  Gets the RF input Power Level of the TDA18204 in dBµV. 
		The power level is returned on 4 digits (ex: 0375 correspond to 3.75dBmV).

\param  tUnit  Concerned unit.
\param  pPowerLevel  Pointer to the object in which the power level must be stored.

\return TM_OK: Succeed.
\return TDA18204_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
\return TDA18204_ERR_BAD_PARAMETER: Parameters out of range.
\return Other optional return codes of the functions registered 
in DeviceOpenInstance API (I2C read errors, mutex...).

\note   To be performed once tmbslTDA18204_Open has been called.

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18204_GetPowerLevel(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    Int32*          pPowerLevel /* O: Power Level in (x100) dBµVrms */
)
{
	pTDA18204Object_t   pObj = Null;
	tmErrorCode_t       err = TM_OK;
	UInt8				RSSIValue = 0;
    UInt8               uAGC2NBLoopOffCurrent = 0;
    UInt8               uAGC3NBLoopOffCurrent = 0;

	/* Get a driver instance */
	err = iTDA18204_GetInstance(tUnit, &pObj);

	_MUTEX_ACQUIRE(TDA18204)

		tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslTDA18204_GetPowerLevel(0x%08X)", tUnit);

	/* Test parameter(s) */
	if( pPowerLevel == Null ||
		pObj->StandardMode<=TDA18204_StandardMode_Unknown ||
		pObj->StandardMode>=TDA18204_StandardMode_Max ||
		pObj->pStandard == Null)
	{
		err = TDA18204_ERR_BAD_PARAMETER;
	}
    else
    {
       *pPowerLevel = 0;
    }

    /* only launch measurement if NB path ON */
	if(pObj->curLLPowerState == TDA18204_PowerNbNormalMode || pObj->curLLPowerState == TDA18204_PowerSciNbNormalMode )
    {
        /* before launching NB RSSI loop off all AGCs after having retrieve current value in order */
        /* to restore them at the end of current procedure */
        /* retrieve AGC2_NB_loop_off  */
        if(err == TM_OK)
        {
            err = iTDA18204_Read(pObj, &gTDA18204_Reg_AGC2_NB_byte_2__AGC2_NB_loop_off, &uAGC2NBLoopOffCurrent, Bus_NoWrite);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", pObj->tUnitW));
        }
        /* retrieve AGC3_NB_loop_off  */
        if(err == TM_OK)
        {
            err = iTDA18204_Read(pObj, &gTDA18204_Reg_AGC3_NB_byte_2__AGC3_NB_loop_off, &uAGC3NBLoopOffCurrent, Bus_NoWrite);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", pObj->tUnitW));
        }  
        /* loop off all AGCs  */
        if(err == TM_OK)
        {
            if (uAGC2NBLoopOffCurrent == 0)
            {
                err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_NB_byte_2__AGC2_NB_loop_off, 0x1, Bus_RW);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", pObj->tUnitW));
            }
        }
        if(err == TM_OK)
        {
            if (uAGC3NBLoopOffCurrent == 0)
            {
                err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC3_NB_byte_2__AGC3_NB_loop_off, 0x1, Bus_RW);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", pObj->tUnitW));
            }
        } 

        /* Get the RSSI value for the power level estimation*/
        err = iTDA18204_GetRSSI(pObj, &RSSIValue);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_GetRSSI(0x%08X) failed.", tUnit));

		if (err == TM_OK)
		{
			err =  iTDA18204_GetPowerLevel(pObj, RSSIValue, pPowerLevel);
			tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18204GetPower_Level(0x%08X) failed.", tUnit));
		}
		else
		{
			*pPowerLevel = 0;
		}

        /* loop on all AGCs  */
        if(err == TM_OK)
        {
            if (uAGC2NBLoopOffCurrent == 0)
            {
                err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_NB_byte_2__AGC2_NB_loop_off, 0x0, Bus_RW);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", pObj->tUnitW));
            }
        }
        if(err == TM_OK)
        {
            if (uAGC3NBLoopOffCurrent == 0)
            {
                err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC3_NB_byte_2__AGC3_NB_loop_off, 0x0, Bus_RW);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", pObj->tUnitW));
            }
        } 
	}
	_MUTEX_RELEASE(TDA18204)

    return err;
}


/*=================================================================================================*/
/**
\brief  Set the Tilt Equalizer selection.

\param  tUnit  Concerned unit.
\param  tTeqSelect  Tilt Equalizer selection (15dB, 10dB, 5dB, 3dB, 0dB, -5dB, -10dB).

\return TM_OK: Succeed.
\return TDA18204_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
\return TDA18204_ERR_BAD_PARAMETER: Parameters out of range.
\return Other optional return codes of the functions registered 
in DeviceOpenInstance API (I2C read errors, mutex...).

\note   To be performed once tmbslTDA18204_Open has been called.

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18204_SetTiltEqualizer(
    tmUnitSelect_t  tUnit,           /* I: Unit number */
    TDA18204TEQSelect_t  tTeqSelect  /* I: Tilt Equalizer selection */
)
{
    pTDA18204Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = iTDA18204_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18204)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslTDA18204_SetTiltEqualizer(0x%08X)", tUnit);

    if(err == TM_OK)
    {
        err = iTDA18204_SetTiltEqualizer(pObj, tTeqSelect);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_SetTiltEqualizer(0x%08X) failed.", pObj->tUnitW));
    }

    _MUTEX_RELEASE(TDA18204)

    return err;
}

/*=================================================================================================*/
/**
\brief  Activate/desactivate GPIO.

\param  tUnit  Concerned unit.
\param  uNumber  Gpio number ( N° 1 or 2 ).
\param  bActive  True : activate  False : desactivate.

\return TM_OK: Succeed.
\return TDA18204_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
\return TDA18204_ERR_BAD_PARAMETER: Parameters out of range.
\return Other optional return codes of the functions registered 
in DeviceOpenInstance API (I2C read errors, mutex...).

\note   To be performed once tmbslTDA18204_Open has been called.

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18204_SetGpio(
    tmUnitSelect_t          tUnit,           /* I: Unit number */
    UInt8                   uNumber,         /* I: Number of the GPIO to set */
    Bool                    bActive          /* I: GPIO enabled/disabled */
)
{
    pTDA18204Object_t           pObj = Null;
    tmErrorCode_t               err = TM_OK;

    /* Get a driver instance */
    err = iTDA18204_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18204)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslTDA18204_SetGpio(0x%08X)", tUnit);

    if(err == TM_OK)
    {
        switch (uNumber)
        {
            case 1:
                /* Set GPIO n°1 */
                err = iTDA18204_Write(pObj, &gTDA18204_Reg_GPIO_CTRL__GPIO1, (bActive==True)?1:0, Bus_RW);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
                break;

            default:
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmbslTDA18204_SetGpio(0x%08X) invalid GPIO number.", tUnit));
                break;
        }
    }

    _MUTEX_RELEASE(TDA18204)

    return err;
}

/*=================================================================================================*/
/**
\brief  Gives information if IRQ raised or not ( flag IRQ Status).

\param  tUnit  Concerned unit.
\param  pIRQ  Pointer to IRQ flag status.

\return TM_OK: Succeed.
\return TDA18204_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
\return TDA18204_ERR_BAD_PARAMETER: Parameters out of range.
\return Other optional return codes of the functions registered 
in DeviceOpenInstance API (I2C read errors, mutex...).

\note   To be performed once tmbslTDA18204_Open has been called.

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18204_GetIRQ(
    tmUnitSelect_t  tUnit  /* I: Unit number */,
    Bool*           pIRQ   /* O: IRQ triggered */
)
{
    pTDA18204Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;
    UInt8               IRQ_status = 0;

    /* Get a driver instance */
    err = iTDA18204_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18204)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslTDA18204_GetIRQ(0x%08X)", tUnit);

    if(pIRQ == Null)
    {
        err = TDA18204_ERR_BAD_PARAMETER;
    }

    if(err == TM_OK)
    {
        *pIRQ = 0;

        err = iTDA18204_Read(pObj, &gTDA18204_Reg_IRQ_status__IRQ_status, &IRQ_status, Bus_RW);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", pObj->tUnitW));

        if(err == TM_OK)
        {
            *pIRQ = IRQ_status;
        }
    }

    _MUTEX_RELEASE(TDA18204)

    return err;
}

/*=================================================================================================*/
/**
\brief  Waits IRQ to trigger and time out if not raised. This action is automatically done in 
		tmbslTDA18204_HwInit, tmbslTDA18204_SetRF and tmbslTDA18204_GetPowerLevel functions. No 
		need to do it on top of that.

\param  tUnit  Concerned unit.
\param  uTimeOut  Time-out in ms for IRQ to raise.
\param  uWaitStep  Time step to poll IRQ in ms.
\param  uIrqStatus  IRQs trigger to wait

\return TM_OK: Succeed.
\return TDA18204_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
\return TDA18204_ERR_BAD_PARAMETER: Parameters out of range.
\return Other optional return codes of the functions registered 
in DeviceOpenInstance API (I2C read errors, mutex...).

\note   To be performed once tmbslTDA18204_Open has been called.

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18204_WaitIRQ(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt32          uTimeOut,    /* I: timeOut for IRQ wait */
    UInt32          uWaitStep,   /* I: wait step */
    UInt8           uIrqStatus   /* I: IRQs to wait */
)
{
    pTDA18204Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = iTDA18204_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18204)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslTDA18204_WaitIRQ(0x%08X)", tUnit);

    err = iTDA18204_WaitIRQ(pObj, uTimeOut, uWaitStep, uIrqStatus);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_WaitIRQ(0x%08X) failed.", tUnit));

    _MUTEX_RELEASE(TDA18204)

    return err;
}

/*=================================================================================================*/
/**
\brief  Write into part or entire register ( to memory or Hardware ).

\param  tUnit  Concerned unit.
\param  pBitField  Pointer to wanted bitfield. The list of const 
			       TDA18204_BitField_t is defined in tmbslTDA18204_RegDef.c file.

\param  uData  data to write.
\param  eBusAccess  Enumeration defining the type of access:
					•	Bus_RW      Read & write in HardWare 
					•	Bus_NoRead  Read in Memory, Write in HardWare
					•	Bus_NoWrite Read in HardWare, Write in Memory
					•	Bus_None         Read & write in memory

\return TM_OK: Succeed.
\return TDA18204_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
\return TDA18204_ERR_BAD_PARAMETER: Parameters out of range.
\return Other optional return codes of the functions registered 
in DeviceOpenInstance API (I2C read errors, mutex...).

\note   To be performed once tmbslTDA18204_Open has been called.

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18204_Write(
    tmUnitSelect_t              tUnit,      /* I: Unit number */
    const TDA18204_BitField_t*  pBitField,  /* I: Bitfield structure */
    UInt8                       uData,      /* I: Data to write */
    tmbslFrontEndBusAccess_t    eBusAccess  /* I: Access to bus */
)
{
    pTDA18204Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = iTDA18204_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18204)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslTDA18204_Write(0x%08X)", tUnit);

    err = iTDA18204_Write(pObj, pBitField, uData, eBusAccess);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));

    _MUTEX_RELEASE(TDA18204)

    return err;
}

/*=================================================================================================*/
/**
\brief  Read into part or entire register ( from memory or Hardware ).

\param  tUnit  Concerned unit.
\param  pBitField  Pointer to wanted bitfield. The list of const,
			       TDA18204_BitField_t is defined in tmbslTDA18204_RegDef.c file.

\param  puData  Pointer to read data.
\param  eBusAccess  Enumeration defining the type of access:
					•	Bus_RW      Read & write in HardWare 
					•	Bus_NoRead  Read in Memory, Write in HardWare
					•	Bus_NoWrite Read in HardWare, Write in Memory
					•	Bus_None         Read & write in memory

\return TM_OK: Succeed.
\return TDA18204_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
\return TDA18204_ERR_BAD_PARAMETER: Parameters out of range.
\return Other optional return codes of the functions registered 
in DeviceOpenInstance API (I2C read errors, mutex...).

\note   To be performed once tmbslTDA18204_Open has been called.

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18204_Read(
    tmUnitSelect_t              tUnit,      /* I: Unit number */
    const TDA18204_BitField_t*  pBitField,  /* I: Bitfield structure */
    UInt8*                      puData,     /* I: Data to read */
    tmbslFrontEndBusAccess_t    eBusAccess  /* I: Access to bus */
)
{
    pTDA18204Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = iTDA18204_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18204)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslTDA18204_Read(0x%08X)", tUnit);

    err = iTDA18204_Read(pObj, pBitField, puData, eBusAccess);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", pObj->tUnitW));

    _MUTEX_RELEASE(TDA18204)

    return err;
}

/*=================================================================================================*/
/**
\brief  Configure the WB path of the TDA18204 to a specified RF frequency.

\param  tUnit  Concerned unit.
\param  uRF  Frequency to set in Hertz.

\return TM_OK Succeed.
\return TDA18204_ERR_ERR_NO_INSTANCES No instances available.
\return TDA18204_ERR_BAD_PARAMETER Parameter is not correctly filled.
\return error codes of the functions registered in tmbslTDA18204_Open (I2C read errors, mutex...).

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18204_ConfigureWB(
    tmUnitSelect_t  tUnit,  /*  I: Unit number */
    UInt32          uRF     /*  I: RF frequency in hertz */
)
{
    pTDA18204Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = iTDA18204_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18204)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslTDA18204_SetRF(0x%08X)", tUnit);

    if(err == TM_OK)
    {
        UInt8 Agc2_g_up = 0;
        UInt8 Agc2_g_down = 0;
        
        UInt8 uIndex = 0;

        /* find out the AGC2 TOP configuration in the table */
        while( (uRF >= pObj->agc2WBTop[uIndex].uRF) && (uIndex < TDA18204_INSTANCE_CUSTOM_AGC2WB_TOP_NB - 1) )
        {
            uIndex ++;
        }

        /* retrieve AGC2 TOP top to be programmed */
        Agc2_g_up = pObj->agc2WBTop[uIndex].AGC2_Gud_Up;
        Agc2_g_down = pObj->agc2WBTop[uIndex].AGC2_Gud_Down;

        if(err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_G_UP__AGC2_G_UP, Agc2_g_up, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }

        if(err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_G_DOWN__AGC2_G_DOWN, Agc2_g_down, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
    }

    _MUTEX_RELEASE(TDA18204)

    return err;
}
/*=================================================================================================*/
/**
\brief  returns number of bytes of driver structure instance in memory

\param  oSize  size in Bytes

\return TM_OK Succeed.

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18204_GetDriverSize(
    UInt16*          pSize     /*  pointer to the number of bytes of driver structure */
    )
{
    *pSize = sizeof(TDA18204Object_t);
    return TM_OK;
}
/*=================================================================================================*/
/**
\brief  Copy driver structure content in memory to pStruct destination

\param  tUnit  Concerned unit.

\return TM_OK Succeed.
\return TDA18204_ERR_ERR_NO_INSTANCES No instances available.

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18204_SaveDriver(
    tmUnitSelect_t  tUnit,  /*  I: Unit number */
    void*          pStruct     /*  pointer to driver structure to save */
)
{
    pTDA18204Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;

    /* Get a driver instance */
    err = iTDA18204_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18204)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslTDA18204_SetRF(0x%08X)", tUnit);

    if(err == TM_OK)
    {
       memcpy(pStruct, pObj, sizeof(TDA18204Object_t));
    }
    _MUTEX_RELEASE(TDA18204)

    return err;
}
/*=================================================================================================*/
/**
\brief  Copy  pStruct content into driver structure instance in memory 

\param  tUnit  Concerned unit.

\return TM_OK Succeed.
\return TDA18204_ERR_ERR_NO_INSTANCES No instances available.

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmbslTDA18204_RestoreDriver(
    tmUnitSelect_t  tUnit,  /*  I: Unit number */
    void*          pStruct     /*  pointer to driver structure to Restore */
)
{
    pTDA18204Object_t   pObj = Null;
    tmErrorCode_t       err = TM_OK;
    ptmbslFrontEndMutexHandle       pMutex;
    tmbslFrontEndIoFunc_t           sIo;
    tmbslFrontEndTimeFunc_t         sTime;
	tmbslFrontEndDebugFunc_t        sDebug;
	tmbslFrontEndMutexFunc_t        sMutex;

    /* Get a driver instance */
    err = iTDA18204_GetInstance(tUnit, &pObj);

    _MUTEX_ACQUIRE(TDA18204)

    tmDBGPRINTEx(DEBUGLVL_INOUT, "tmbslTDA18204_SetRF(0x%08X)", tUnit);

    if(err == TM_OK)
    {
        /* save data before restore*/
        pMutex = pObj->pMutex;
        sIo = pObj->sIo;
        sTime =  pObj->sTime;
        sDebug = pObj->sDebug;
        sMutex = pObj->sMutex;

        memcpy(pObj, pStruct, sizeof(TDA18204Object_t));

       /* restore saved data */
        pObj->pMutex =pMutex;
        pObj->sIo = sIo;
        pObj->sTime = sTime;
        pObj->sDebug = sDebug;
        pObj->sMutex = sMutex;
    }

    _MUTEX_RELEASE(TDA18204)

    return err;
}
/*============================================================================*/
/* Internal functions:                                                        */
/*============================================================================*/
/*============================================================================*/
/* FUNCTION:    iTDA18204_CheckHwState                                        */
/*                                                                            */
/* DESCRIPTION: Checks if Hw is ready to operate.                             */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18204_CheckHwState(
    pTDA18204Object_t       pObj,   /* I: Driver object */
    TDA18204HwStateCaller_t caller  /* I: Caller API */
)
{
	tmErrorCode_t   err = TM_OK;

	tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18204_CheckHwState(0x%08X)", pObj->tUnitW);

	switch(pObj->eHwState)
	{
	case TDA18204_HwState_InitNotDone:
		switch(caller)
		{
		case TDA18204_HwStateCaller_SetPower:
			break;

		default:
			err = TDA18204_ERR_NOT_INITIALIZED;
			break;
		}
		break;

	case TDA18204_HwState_InitDone:
		switch(caller)
		{
		case TDA18204_HwStateCaller_SetRF:
		case TDA18204_HwStateCaller_GetVCOmax:
		case TDA18204_HwStateCaller_GetIF:
		case TDA18204_HwStateCaller_GetLO:
		case TDA18204_HwStateCaller_GetVCO:

			err = TDA18204_ERR_STD_NOT_SET;
			break;

		default:
			break;
		}
		break;

	case TDA18204_HwState_SetStdDone:
		break;

	case TDA18204_HwState_SetRFDone:
		break;

	case TDA18204_HwState_InitPending:
		err = TDA18204_ERR_NOT_READY;
		break;            
	default:
		err = TDA18204_ERR_NOT_READY;
		break;
	}

	return err;
}


/*============================================================================*/
/* FUNCTION:    iTDA18204_FVCOMaxCalibration                                  */
/*                                                                            */
/* DESCRIPTION: Retrieve the FVCO max in Hz                                   */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18204_FVCOMaxCalibration(
    pTDA18204Object_t   pObj   /* I: Driver object */
)
{
	/* Initialize to error in case no valuable values are found */
	tmErrorCode_t err = TM_OK;
	UInt8  uValue = 0;
    UInt32 uValue32 = 0;

    /************************************/
    /*  Retrieve and store Temperature  */
    /************************************/
    err = iTDA18204_Write(pObj, &gTDA18204_Reg_THERMO_2__TM_ON, 0x1, Bus_NoRead);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    if(err == TM_OK)
    {
        err = iTDA18204_Read(pObj, &gTDA18204_Reg_THERMO_1__TM_D, &pObj->Temperature, Bus_NoWrite);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_THERMO_2__TM_ON, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /***************************************************************/
	/*  Check if register are ready to launch VCO max calibration  */
	/***************************************************************/
	err = iTDA18204_Read(pObj, &gTDA18204_Reg_POWER_DOWN_1__PD_REG2V8SYNT, &uValue, Bus_NoWrite);
	tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", pObj->tUnitW));
	if((err == TM_OK) && (uValue == 0))
	{
		err = iTDA18204_Read(pObj, &gTDA18204_Reg_XTAL_IN_CTRL__XTAL_IN_CTRL, &uValue, Bus_NoWrite);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", pObj->tUnitW));
	}
	else
	{
		err = TDA18204_ERR_NOT_READY;
	}
	if((err == TM_OK) && (uValue > 3))
	{
		err = iTDA18204_Read(pObj, &gTDA18204_Reg_POWER_DOWN_6__pd_pll, &uValue, Bus_NoWrite);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", pObj->tUnitW));
	}
	else
	{
		err = TDA18204_ERR_NOT_READY;
	}
	if((err == TM_OK) && (uValue == 0))
	{
		err = iTDA18204_Read(pObj, &gTDA18204_Reg_POWER_DOWN_6__pd_maindivcmos, &uValue, Bus_NoWrite);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", pObj->tUnitW));
	}
	else
	{
		err = TDA18204_ERR_NOT_READY;
	}	

	/*********************************************************/
	/*  Set IRQ_clear                                        */
	/*********************************************************/
	if((err == TM_OK) && (uValue == 0))
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_IRQ_clear, TDA18204_IRQ_Global|TDA18204_IRQ_MSM_LOCalc, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	else
	{
		err = TDA18204_ERR_NOT_READY;
	}

	/*********************************************************/
	/*  laucnh Fvcomax_calibration                           */
	/*********************************************************/
	if(err == TM_OK)
	{
		err = iTDA18204_SetMSM(pObj, TDA18204_MSM_FVCO_max_calc, True);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_SetMSM(0x%08X, TDA18204_MSM_FVCO_max_calc) failed.", pObj->tUnitW));
	}

	/*********************************************************/
	/*  wait for IRQ MSM_LOCalc_End                          */
	/*********************************************************/
	/* wait for 100 ms */
	if (err == TM_OK)
	{
		/* Wait for IRQ to trigger */
		err = iTDA18204_WaitIRQ(pObj, 100, 10, TDA18204_IRQ_MSM_LOCalc);
	}
    /****************************************************************/
    /*  patch ES1, ES1MF : as LOCalc_End doesn't occurs, we do as   */
    /*  if it occurs, then return OK and read registers             */
    /*  For ES2, wait for LOCalc_End occurs then read registers     */
    /****************************************************************/
    /* ES1, ES1MF specific */
    if ((pObj->Revision == 0x10) || (pObj->Revision == 0x11) || (pObj->Revision == 0x12) )
    {
        err =  TM_OK;

        /* restore some settings after FVCO Max Calibration */
        if ( (pObj->XTin == TDA18204_XTAL_IN_CTRL_BUFFER_IN_27MHZ_VALUE_5) || (pObj->XTin == TDA18204_XTAL_IN_CTRL_BUFFER_IN_16MHZ_VALUE_4) )
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_clock_ctrl_byte_1__Dig_Clock, TDA18204_Dig_Clock_INTERNAL_OSCILLATOR_VALUE_0, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
    }

    /* FVCO_max[7..0] */
	if(err == TM_OK)
	{
		err = iTDA18204_Read(pObj, &gTDA18204_Reg_FVCO_MAX_BYTE_3__FVCO_max_3, &uValue, Bus_NoWrite);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", pObj->tUnitW));
	}
	uValue32 = uValue;
	/* FVCO_max[15..8] */
	if(err == TM_OK)
	{
		err = iTDA18204_Read(pObj, &gTDA18204_Reg_FVCO_MAX_BYTE_2__FVCO_max_2, &uValue, Bus_NoWrite);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", pObj->tUnitW));
	}
	uValue32 += (uValue << 8);
	/* FVCO_max[23..16] */
	if(err == TM_OK)
	{
		err = iTDA18204_Read(pObj, &gTDA18204_Reg_FVCO_MAX_BYTE_1__FVCO_max_1, &uValue, Bus_NoWrite);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", pObj->tUnitW));
	}
	uValue32 += (uValue << 16);

	/* FVCOmax = FVCOmax (read value) - uVCOMaxMargin */
	pObj->VCOmax = (uValue32 - uVCOMaxMargin);
	//pObj->VCOmax = uValue32;

	return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18204_XtalCalibration                                     */
/*                                                                            */
/* DESCRIPTION: Launch the Xtal calibration                                   */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18204_XtalCalibration(
    pTDA18204Object_t   pObj   /* I: Driver object */
)
{
	tmErrorCode_t err = TM_OK;

	/* Set IRQ_clear */
	err = iTDA18204_Write(pObj, &gTDA18204_Reg_IRQ_clear, TDA18204_IRQ_Global|TDA18204_IRQ_XtalCal, Bus_NoRead);
	tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));

	/* Launch Xtal calibration */
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_MSM_byte_2__XtalCal_Launch,  0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}

	/* Wait IRQ */
	if(err == TM_OK)
	{
		/* State reached after 500 ms max */
		err = iTDA18204_WaitIRQ(pObj, 500, 10, TDA18204_IRQ_XtalCal);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_WaitIRQ(0x%08X) failed.", pObj->tUnitW));
	}

	return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18204_RCCalibration                                       */
/*                                                                            */
/* DESCRIPTION: Launch the RC calibration                                     */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18204_RCCalibration(
    pTDA18204Object_t   pObj   /* I: Driver object */
)
{
	tmErrorCode_t err = TM_OK;

	/* before launching RC calibration, in XTAL buffer in mode some specific settings must be done */
	if ( (pObj->XTin == TDA18204_XTAL_IN_CTRL_BUFFER_IN_27MHZ_VALUE_5) || (pObj->XTin == TDA18204_XTAL_IN_CTRL_BUFFER_IN_16MHZ_VALUE_4) )
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_LO_CHAIN_MUX_DIV2D__lochain_mux_div2d, TDA18204_lochain_mux_div2d_DIVIDE_BY_2_OR_3_MINUS_CALIBRATION_880MHZ_VALUE_54, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));

		/* program sublo */
		if(err == TM_OK)
		{
			/* patch calibration */
			err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_7__pd_sublo, 0x0, Bus_NoRead);
			tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
		}
		if(err == TM_OK)
		{
			err = iTDA18204_Write(pObj, &gTDA18204_Reg_SUBLO_PRESC__SUBLO_PRESC, 27, Bus_NoRead);
			tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
		}
	}

	/* Set IRQ_clear */
	err = iTDA18204_Write(pObj, &gTDA18204_Reg_IRQ_clear, TDA18204_IRQ_Global|TDA18204_IRQ_MSM_RCCal, Bus_NoRead);
	tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));

	/* Launch RC calibration */
	if(err == TM_OK)
	{
		err = iTDA18204_SetMSM(pObj, TDA18204_MSM_RC_Cal, True);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_SetMSM(0x%08X, TDA18204_MSM_RC_Cal) failed.", pObj->tUnitW));
	}

	/* Wait IRQ */
	if(err == TM_OK)
	{
		/* State reached after 500 ms max */
		err = iTDA18204_WaitIRQ(pObj, 500, 10, TDA18204_IRQ_MSM_RCCal);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_WaitIRQ(0x%08X) failed.", pObj->tUnitW));
	}

    /* ES1, ES1MF specific */
    if ((pObj->Revision == 0x10) || (pObj->Revision == 0x11) || (pObj->Revision == 0x12) )
    {
        /* restore some settings after RC calibration */
        if ( (pObj->XTin == TDA18204_XTAL_IN_CTRL_BUFFER_IN_27MHZ_VALUE_5) || (pObj->XTin == TDA18204_XTAL_IN_CTRL_BUFFER_IN_16MHZ_VALUE_4) )
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_LO_CHAIN_MUX_DIV2D__lochain_mux_div2d, TDA18204_lochain_mux_div2d_DIVIDE_BY_16_OR_24_MINUS_CALIBRATION_110MHZ_VALUE_43, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));

            if(err == TM_OK)
            {
                err = iTDA18204_Write(pObj, &gTDA18204_Reg_clock_ctrl_byte_1__Dig_Clock, TDA18204_Dig_Clock_INTERNAL_OSCILLATOR_VALUE_0, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
            }

            /* desactivate sublo */
            if(err == TM_OK)
            {
                /* patch calibration */
                err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_7__pd_sublo, 0x1, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
            }
            if(err == TM_OK)
            {
                err = iTDA18204_Write(pObj, &gTDA18204_Reg_SUBLO_PRESC__SUBLO_PRESC, 0x0, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
            }
        }
    }

        ///* restore some settings after RC calibration */
        //if ( (pObj->XTin == TDA18204_XTAL_IN_CTRL_BUFFER_IN_27MHZ_VALUE_5) || (pObj->XTin == TDA18204_XTAL_IN_CTRL_BUFFER_IN_16MHZ_VALUE_4) )
        //{
        //    err = iTDA18204_Write(pObj, &gTDA18204_Reg_LO_CHAIN_MUX_DIV2D__lochain_mux_div2d, TDA18204_lochain_mux_div2d_DIVIDE_BY_16_OR_24_MINUS_CALIBRATION_110MHZ_VALUE_43, Bus_NoRead);
        //    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));

        //    if(err == TM_OK)
        //    {
        //        err = iTDA18204_Write(pObj, &gTDA18204_Reg_clock_ctrl_byte_1__Dig_Clock, TDA18204_Dig_Clock_INTERNAL_OSCILLATOR_VALUE_0, Bus_NoRead);
        //        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        //    }

        //    /* desactivate sublo */
        //    if(err == TM_OK)
        //    {
        //        /* patch calibration */
        //        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_7__pd_sublo, 0x1, Bus_NoRead);
        //        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        //    }
        //    if(err == TM_OK)
        //    {
        //        err = iTDA18204_Write(pObj, &gTDA18204_Reg_SUBLO_PRESC__SUBLO_PRESC, 0x0, Bus_NoRead);
        //        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        //    }
        //}

	return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18204_IRCalibration                                       */
/*                                                                            */
/* DESCRIPTION: Launch the IR calibration                                     */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18204_IRCalibration(
    pTDA18204Object_t   pObj   /* I: Driver object */
)
{
    tmErrorCode_t err = TM_OK;

    /***** Image Rejection Calibration done at 110, 440 and 880 MHz, with VCO fixed at 3520 MHz *****/
    /**********************/
    /**     110 MHz      **/
    /**********************/
    /* set LPF_DCOffset_Corr to 1 */
    err = iTDA18204_Write(pObj, &gTDA18204_Reg_IF_AGC_byte__LPF_DCOffset_Corr, 0x1, Bus_NoRead);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));

    /* Enable High Pass */
    err = iTDA18204_Write(pObj, &gTDA18204_Reg_IF_Byte_2__IF_HP_Enable, 0x1, Bus_NoRead);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));

    /* Select Cuttoff Frequency to 1MHz  */
    err = iTDA18204_Write(pObj, &gTDA18204_Reg_IF_Byte_2__IF_HP_Fc, TDA18204_IF_HP_Fc_1_VALUE_1, Bus_NoRead);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));

    if(err == TM_OK)
    {
        /* set pd_calgen to 0 */
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_7__pd_calgen, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /* Diable polyphase filter for calibration (880MHz) */
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_calgen_byte_1__calgen_pph_sel, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /* set round division chain LO (110 MHz) + mux lochain_mux_div2d */
    /* bias du VCO On + LO Chain : power_down_5.pd_lochain  = 0x0 */
    /* mode IRCAL : power_down_5.pd_lochain_calmode  = 0x0 */
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5, 0x21, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_LO_CHAIN_MUX_DIV2D, TDA18204_lochain_mux_div2d_DIVIDE_BY_16_OR_24_MINUS_CALIBRATION_110MHZ_VALUE_43, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /* selects the low frequency of IRcal */
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_IR_Cal_byte_2__IR_FreqPtr, TDA18204_IR_FreqPtr_LOW_VALUE_1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /* Set IRQ_clear */
    err = iTDA18204_Write(pObj, &gTDA18204_Reg_IRQ_clear, TDA18204_IRQ_Global|TDA18204_IRQ_MSM_IRCAL, Bus_NoRead);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));

    /* Launch RC + IR calibration */
    if(err == TM_OK)
    {
        err = iTDA18204_SetMSM(pObj, TDA18204_MSM_IrCal, True);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_SetMSM(0x%08X, TDA18204_MSM_HwInit) failed.", pObj->tUnitW));
    }

    /* Wait IRQ */
    if(err == TM_OK)
    {
        /* State reached after 500 ms max */
        err = iTDA18204_WaitIRQ(pObj, 500, 10, TDA18204_IRQ_MSM_IRCAL);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_WaitIRQ(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************/
    /**     440 MHz      **/
    /**********************/
    /* set round division chain LO (440 MHz) + mux lochain_mux_div2d */
    /* bias du VCO On + LO Chain : power_down_5.pd_lochain  = 0x0 */
    /* mode IRCAL : power_down_5.pd_lochain_calmode  = 0x0 */
    /* if(err == TM_OK)
    {
    err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5, 0x2D, Bus_NoRead);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
    err = iTDA18204_Write(pObj, &gTDA18204_Reg_LO_CHAIN_MUX_DIV2D, TDA18204_lochain_mux_div2d_CALIBRATION_440MHZ_VALUE_46, Bus_NoRead);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    } */

    /* selects the mid frequency of IRcal */
    /* if(err == TM_OK)
    {
    err = iTDA18204_Write(pObj, &gTDA18204_Reg_IR_Cal_byte_2__IR_FreqPtr, TDA18204_IR_FreqPtr_MID_VALUE_2, Bus_NoRead);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    } */

    /* Set IRQ_clear */
    /* err = iTDA18204_Write(pObj, &gTDA18204_Reg_IRQ_clear, TDA18204_IRQ_Global|TDA18204_IRQ_MSM_IRCAL, Bus_NoRead);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW)); */

    /* Launch RC + IR calibration */
    /* if(err == TM_OK)
    {
    err = iTDA18204_SetMSM(pObj, TDA18204_MSM_IrCal, True);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_SetMSM(0x%08X, TDA18204_MSM_HwInit) failed.", pObj->tUnitW));
    } */

    /* Wait IRQ */
    /* if(err == TM_OK)
    { */
    /* State reached after 500 ms max */
    /* err = iTDA18204_WaitIRQ(pObj, 500, 10, TDA18204_IRQ_MSM_IRCAL);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_WaitIRQ(0x%08X) failed.", pObj->tUnitW));
    } */

    /**********************/
    /**     880 MHz      **/
    /**********************/
    /* Enable polyphase filter for calibration (880MHz) */
    /* if(err == TM_OK)
    {
    err = iTDA18204_Write(pObj, &gTDA18204_Reg_calgen_byte_1__calgen_pph_sel, 0x1, Bus_NoRead);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    } */

    /* set round division chain LO (880 MHz) + mux lochain_mux_div2d */
    /* bias du VCO On + LO Chain : power_down_5.pd_lochain  = 0x0 */
    /* mode IRCAL : power_down_5.pd_lochain_calmode  = 0x1 */
    /* if(err == TM_OK)
    {
    err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5, 0x6F, Bus_NoRead);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
    err = iTDA18204_Write(pObj, &gTDA18204_Reg_LO_CHAIN_MUX_DIV2D, TDA18204_lochain_mux_div2d_DIVIDE_BY_2_OR_3_MINUS_CALIBRATION_880MHZ_VALUE_54, Bus_NoRead);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
    err = iTDA18204_Write(pObj, &gTDA18204_Reg_calgen_byte_1__vco_IR_high, 0x1, Bus_NoRead);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    } */

    /* selects the high frequency of IRcal */
    /* if(err == TM_OK)
    {
    err = iTDA18204_Write(pObj, &gTDA18204_Reg_IR_Cal_byte_2__IR_FreqPtr, TDA18204_IR_FreqPtr_HIGH_VALUE_3, Bus_NoRead);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    } */

    /* Set IRQ_clear */
    /* err = iTDA18204_Write(pObj, &gTDA18204_Reg_IRQ_clear, TDA18204_IRQ_Global|TDA18204_IRQ_MSM_IRCAL, Bus_NoRead);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW)); */

    /* Launch RC + IR calibration */
    /* if(err == TM_OK)
    {
    err = iTDA18204_SetMSM(pObj, TDA18204_MSM_IrCal, True);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_SetMSM(0x%08X, TDA18204_MSM_HwInit) failed.", pObj->tUnitW));
    } */

    /* Wait IRQ */
    /* if(err == TM_OK)
    { */
    /* State reached after 500 ms max */
    /* err = iTDA18204_WaitIRQ(pObj, 500, 10, TDA18204_IRQ_MSM_IRCAL);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_WaitIRQ(0x%08X) failed.", pObj->tUnitW));
    } */

    /* reset LPF_DCOffset_Corr to 0 */
    /* if(err == TM_OK)
    {
    err = iTDA18204_Write(pObj, &gTDA18204_Reg_IF_AGC_byte__LPF_DCOffset_Corr, 0x0, Bus_NoRead);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    } */

    /* Disable High Pass */
    /* if(err == TM_OK)
    {
    err = iTDA18204_Write(pObj, &gTDA18204_Reg_IF_Byte_2__IF_HP_Enable, 0x0, Bus_NoRead);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    } */

    /* Disable polyphase filter for calibration (880MHz) */
    /* if(err == TM_OK)
    {
    err = iTDA18204_Write(pObj, &gTDA18204_Reg_calgen_byte_1__calgen_pph_sel, 0x0, Bus_NoRead);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    } */

    /* Disable vco_IR_high */
    /* if(err == TM_OK)
    {
    err = iTDA18204_Write(pObj, &gTDA18204_Reg_calgen_byte_1__vco_IR_high, 0x0, Bus_NoRead);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    } */

    /* ES1, ES1MF specific */
    if ((pObj->Revision == 0x10) || (pObj->Revision == 0x11) || (pObj->Revision == 0x12) )
    {
        /* restore some settings after IR calibration */
        if ( (pObj->XTin == TDA18204_XTAL_IN_CTRL_BUFFER_IN_27MHZ_VALUE_5) || (pObj->XTin == TDA18204_XTAL_IN_CTRL_BUFFER_IN_16MHZ_VALUE_4) )
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_clock_ctrl_byte_1__Dig_Clock, TDA18204_Dig_Clock_INTERNAL_OSCILLATOR_VALUE_0, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
    }


        ///* restore some settings after IR calibration */
        //if ( (pObj->XTin == TDA18204_XTAL_IN_CTRL_BUFFER_IN_27MHZ_VALUE_5) || (pObj->XTin == TDA18204_XTAL_IN_CTRL_BUFFER_IN_16MHZ_VALUE_4) )
        //{
        //    err = iTDA18204_Write(pObj, &gTDA18204_Reg_clock_ctrl_byte_1__Dig_Clock, TDA18204_Dig_Clock_INTERNAL_OSCILLATOR_VALUE_0, Bus_NoRead);
        //    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        //}


    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18204_CalculateNLO                                        */
/*                                                                            */
/* DESCRIPTION: Calculate the value of NLO                                    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18204_CalculateNLO(
    pTDA18204Object_t   pObj,		/* I: Driver object */
    UInt32				LO,			/* In KHz */
    UInt8*				NLO		    /* NLO_set  */
)
{
	tmErrorCode_t err = TM_OK;
	UInt8 uNLO = 0;

	if ( pObj->VCOmax >= ( LO*96) )
		uNLO = 96;
	else if ( pObj->VCOmax >= ( LO*64) )
		uNLO = 64;
	else if ( pObj->VCOmax >= ( LO*48) )
		uNLO = 48;
	else if ( pObj->VCOmax >= ( LO*32) )
		uNLO = 32;
	else if ( pObj->VCOmax >= ( LO*24) )
		uNLO = 24;
	else if ( pObj->VCOmax >= ( LO*16) )
		uNLO = 16;
	else if ( pObj->VCOmax >= ( LO*12) )
		uNLO = 12;
	else if ( pObj->VCOmax >= ( LO*8) )
		uNLO = 8;
	else if ( pObj->VCOmax >= ( LO*6) )
		uNLO = 6;
	else if ( pObj->VCOmax >= ( LO*4) )
		uNLO = 4;

	*NLO = uNLO;

	return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18204_FindXtalDiv1p5                                      */
/*                                                                            */
/* DESCRIPTION: Determine if Xtal is divide by 1.5                            */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18204_FindXtalDiv1p5(
    UInt32				VCO,			/* VCO In KHz */
    Bool*				Xtal_div_1p5	/* Xtal is divide by 1.5 */
)
{
	tmErrorCode_t err = TM_OK;
	UInt32 Fref = 27000; /* in KHz */
	Bool Div1p5 = False;

	/* The number  (VCO/Fref) has to be in the interval [128,255]. If we take in account the modulation, */
	/* and the architect requirement, it has to be in the interval [131:245] */
	/* To be implemented retrieve Fref in pobj */
	if ( VCO >= (131*Fref) )
		Div1p5 = False;
	else
		Div1p5 = True;

	*Xtal_div_1p5 = Div1p5;

	return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18204_SelectVCO                                           */
/*                                                                            */
/* DESCRIPTION: Determine if VCO high or Low must be choosen                  */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18204_SelectVCO(
    pTDA18204Object_t   pObj,			/* I: Driver object */
    UInt32				VCO,  			/* VCO In KHz */
    Bool*				VCOHighUsed  	/* is VCO high used or not */
)
{
    tmErrorCode_t err = TM_OK;
    UInt32 VCOcomp = 0;

    /* VCOcomp = FVCOmax(register value => so add uVCOMaxMargin)/1.3 */
    VCOcomp = ((pObj->VCOmax+uVCOMaxMargin)*100)/uVCOSwitch;

    /* common ES1, ES1MF, ES2 */
    if (VCO <= VCOcomp)
    {
        /**********************************/
        /****         VCO LOW          ****/
        /**********************************/
        *VCOHighUsed = False;

        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__PD_VCO_LOW, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        if(err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__PD_VCO_HIGH, 0x1, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__PD_VCO_EXT, 0x1, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }

        if ( VCO > (((pObj->VCOmax+uVCOMaxMargin)*100)/140) )
        {
            if(err == TM_OK)
            {
                err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_SAVING_MODE_5__PSM_VCO, TDA18204_PSM_VCO_4_5_LOW_OR_8_HIGH_VALUE_3, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
            }

            if( err == TM_OK )
            {
                err = iTDA18204_Write(pObj, &gTDA18204_Reg_Charge_pump_byte__ICP_select, TDA18204_ICP_select_150_VALUE_1, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
            }
            if(err == TM_OK)
            {
                err = iTDA18204_Write(pObj, &gTDA18204_Reg_Charge_pump_byte__ICP_low, 0x0, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
            }
        }
        else
        {
            if( err == TM_OK )
            {
                err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_SAVING_MODE_5__PSM_VCO, TDA18204_PSM_VCO_4_5_LOW_OR_8_HIGH_VALUE_3, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
            }
            if( err == TM_OK )
            {
                err = iTDA18204_Write(pObj, &gTDA18204_Reg_Charge_pump_byte__ICP_select, TDA18204_ICP_select_100_VALUE_3, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
            }
            if(err == TM_OK)
            {
                err = iTDA18204_Write(pObj, &gTDA18204_Reg_Charge_pump_byte__ICP_low, 0x0, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
            }
        }
    }
    else
    {
        /**********************************/
        /****         VCO HIGH         ****/
        /**********************************/
        *VCOHighUsed = True;

        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__PD_VCO_HIGH, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        if(err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__PD_VCO_LOW, 0x1, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__PD_VCO_EXT, 0x1, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }

        if ( (VCO <= pObj->VCOmax) && (VCO > (((pObj->VCOmax+uVCOMaxMargin)*100)/110)) )
        {
            if(err == TM_OK)
            {
                err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_SAVING_MODE_5__PSM_VCO, TDA18204_PSM_VCO_2_LOW_OR_5_HIGH_VALUE_0, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
            }
            if( err == TM_OK )
            {
                err = iTDA18204_Write(pObj, &gTDA18204_Reg_Charge_pump_byte__ICP_select, TDA18204_ICP_select_150_VALUE_1, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
            }
            if(err == TM_OK)
            {
                err = iTDA18204_Write(pObj, &gTDA18204_Reg_Charge_pump_byte__ICP_low, 0x0, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
            }
        }
        else if ( (VCO <= (((pObj->VCOmax+uVCOMaxMargin)*100)/110) ) && (VCO > (((pObj->VCOmax+uVCOMaxMargin)*100)/120)) )
        {
            if( err == TM_OK )
            {
                err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_SAVING_MODE_5__PSM_VCO, TDA18204_PSM_VCO_2_LOW_OR_5_HIGH_VALUE_0, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
            }
            if( err == TM_OK )
            {
                err = iTDA18204_Write(pObj, &gTDA18204_Reg_Charge_pump_byte__ICP_select, TDA18204_ICP_select_100_VALUE_3, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
            }
            if(err == TM_OK)
            {
                err = iTDA18204_Write(pObj, &gTDA18204_Reg_Charge_pump_byte__ICP_low, 0x0, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
            }
        }
        else
        {
            if(err == TM_OK)
            {
                err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_SAVING_MODE_5__PSM_VCO, TDA18204_PSM_VCO_2_LOW_OR_5_HIGH_VALUE_0, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
            }
            if(err == TM_OK)
            {
                err = iTDA18204_Write(pObj, &gTDA18204_Reg_Charge_pump_byte__ICP_low, 0x1, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
            }
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18204_SetSubLO                                            */
/*                                                                            */
/* DESCRIPTION: Calculate and program the Sub LO                             */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18204_SetSubLO(
    pTDA18204Object_t   pObj,		/* I: Driver object */
    UInt32				LO 			/* In KHz */
)
{
	tmErrorCode_t err = TM_OK;
	UInt8 NSub = 0;
	UInt8 SubLOPresc = 0;
	UInt8 SubLOBypass = 0;

	NSub = (UInt8) (LO/16000);

	if ( NSub > 55)
	{
		SubLOPresc = 27;
		SubLOBypass = 0;
	}
	if ((NSub <= 55) && (NSub > 49))
	{
		SubLOPresc = 20;
		SubLOBypass = 0;
	}
	if ((NSub <= 49) && (NSub > 44))
	{
		SubLOPresc = 15;
		SubLOBypass = 0;
	}
	if ((NSub <= 44) && (NSub > 40))
	{
		SubLOPresc = 10;
		SubLOBypass = 0;
	}
	if ((NSub <= 40) && (NSub > 36))
	{
		SubLOPresc = 6;
		SubLOBypass = 0;
	}
	if ((NSub <= 36) && (NSub > 34))
	{
		SubLOPresc = 3;
		SubLOBypass = 0;
	}
	if ((NSub <= 34) && (NSub > 31))
	{
		SubLOPresc = 3;
		SubLOBypass = 0;
	}
	if ((NSub <= 31) && (NSub > 34))
	{
		SubLOPresc = 1;
		SubLOBypass = 0;
	}
	if ((NSub <= 34) && (NSub > 28))
	{
		SubLOPresc = 29;
		SubLOBypass = 1;
	}
	if ((NSub <= 28) && (NSub > 24))
	{
		SubLOPresc = 20;
		SubLOBypass = 1;
	}
	if ((NSub <= 24) && (NSub > 22))
	{
		SubLOPresc = 14;
		SubLOBypass = 1;
	}
	if ((NSub <= 22) && (NSub > 20))
	{
		SubLOPresc = 10;
		SubLOBypass = 1;
	}
	if ((NSub <= 20) && (NSub > 18))
	{
		SubLOPresc = 6;
		SubLOBypass = 1;
	}
	if ((NSub <= 18) && (NSub > 17))
	{
		SubLOPresc = 4;
		SubLOBypass = 1;
	}
	if ((NSub <= 17) && (NSub > 15))
	{
		SubLOPresc = 0;
		SubLOBypass = 1;
	}
	if ((NSub <= 15) && (NSub > 13))
	{
		SubLOPresc = 29;
		SubLOBypass = 3;
	}
	if ((NSub <= 13) && (NSub > 12))
	{
		SubLOPresc = 20;
		SubLOBypass = 3;
	}
	if ((NSub <= 12) && (NSub > 11))
	{
		SubLOPresc = 16;
		SubLOBypass = 3;
	}
	if ((NSub <= 11) && (NSub > 10))
	{
		SubLOPresc = 12;
		SubLOBypass = 3;
	}	
	if ((NSub <= 10) && (NSub > 9))
	{
		SubLOPresc = 8;
		SubLOBypass = 3;
	}	
	if ((NSub <= 9) && (NSub > 8))
	{
		SubLOPresc = 4;
		SubLOBypass = 3;
	}	
	if ((NSub <= 8) && (NSub > 7))
	{
		SubLOPresc = 0;
		SubLOBypass = 3;
	}	
	if ((NSub <= 7) && (NSub > 6))
	{
		SubLOPresc = 24;
		SubLOBypass = 7;
	}	
	if ((NSub <= 6) && (NSub > 5))
	{
		SubLOPresc = 16;
		SubLOBypass = 7;
	}	
	if ((NSub <= 5) && (NSub > 4))
	{
		SubLOPresc = 8;
		SubLOBypass = 7;
	}	
	if ((NSub <= 4) && (NSub > 3))
	{
		SubLOPresc = 0;
		SubLOBypass = 7;
	}	
	if (NSub <= 3)
	{
		SubLOPresc = 24;
		SubLOBypass = 15;
	}

	/*****************************/
	/* program sub LO registers  */
	/*****************************/
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_7__pd_sublo, 0x0, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
    if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_SUBLO_PRESC__SUBLO_PRESC, SubLOPresc, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_SUBLO_BYPASS__SUBLO_BYPASS, SubLOBypass, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_clock_ctrl_byte_1__Dig_Clock, TDA18204_Dig_Clock_SUBLO_VALUE_3, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}

	return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18204_CalculateNIntKInt                                   */
/*                                                                            */
/* DESCRIPTION: Calculate PLL N & K values                                    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18204_CalculateNIntKInt(
    UInt32				VCO,			/* VCO In KHz */
    Bool				Xtal_div_1p5,	/* Xtal is divide by 1.5 */
    UInt8*				NInt,			/* LO_Int */
    UInt32*				KInt			/* LO_Frac */
)
{
    tmErrorCode_t err = TM_OK;
	UInt8 i = 0;
	UInt32 LOFracMask = 0x400000; /* bit[22] = 1 */
    UInt32 Division_ratio_int = 0;
    UInt32 Division_ratio_frac_x100000000 = 0;
	UInt32 Division_ratio_frac_x1000000000 = 0;
    UInt8 LOInt = 0;
	UInt32 LOFrac = 0;
	UInt32 Fref = 27000; /* in KHz */

	/* Calculate N_K_Prog */
	if (Xtal_div_1p5 == True)
	{
		Division_ratio_int = (VCO * 3)/(Fref*2);
		Division_ratio_frac_x100000000 = ((VCO - (Division_ratio_int*Fref*2/3))*100000)/((Fref*2/3)/1000); /* Division_ratio_frac x 100000000 */
		//Division_ratio_int = VCO / Fref;
		//Division_ratio_frac_x100000000 = ((VCO - (Division_ratio_int*Fref))*100000)/(Fref/1000); /* Division_ratio_frac x 100000000 */
	}
	else
	{
		Division_ratio_int = VCO / Fref;
		Division_ratio_frac_x100000000 = ((VCO - (Division_ratio_int*Fref))*100000)/(Fref/1000); /* Division_ratio_frac x 100000000 */
	}

	Division_ratio_frac_x100000000 = Division_ratio_frac_x100000000 / 2;

	/* Division_ratio = 128 +(N,K) */
	Division_ratio_int = Division_ratio_int - 128;

	/*     avec :	0.25 <= K < 0.75	=>  N' = N */
	/*									=>  K'= K  */
	/*				K < 0.25			=>  N' = N */
	/*									=>  K'= K  */
	/*				K >= 0.75			=>  N'= N  */
	/*									=>  K' = K */
	if ( Division_ratio_frac_x100000000 < 25000000)
	{
		Division_ratio_int = Division_ratio_int - 1;
		Division_ratio_frac_x100000000 = Division_ratio_frac_x100000000 + 50000000;
	}
	else if ( Division_ratio_frac_x100000000 >= 75000000)
	{
		Division_ratio_int = Division_ratio_int + 1;
		Division_ratio_frac_x100000000 = Division_ratio_frac_x100000000 - 50000000;
	}

	Division_ratio_frac_x1000000000 = Division_ratio_frac_x100000000 * 10; /* Division_ratio_frac x 1000000000 to win in precision*/

	for (i=0; i<23; i++)
	{
		if (Division_ratio_frac_x1000000000 >= LOFracTab[i])
		{
			LOFrac = LOFrac | LOFracMask;
			Division_ratio_frac_x1000000000 -= LOFracTab[i];
		}
		LOFracMask = (LOFracMask >> 1);
	}
    LOInt = (UInt8)(Division_ratio_int & 0x7F);
    LOFrac = LOFrac & 0x7FFFFF;

	*NInt = LOInt;
    *KInt = (LOFrac | 1);
	
	return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18204_PowerSavingMode                                     */
/*                                                                            */
/* DESCRIPTION: set registers according  the  PowerSavingMode                 */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t 
iTDA18204_PowerSavingMode(
    pTDA18204Object_t   pObj,   /* I: Driver object */
    TDA18204PowerSavingMode_t  PowerSavingMode    /* I: Power Saving Mode */
)
{
	tmErrorCode_t   err = TM_OK;

	tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18204_PowerSavingMode(0x%08X)", pObj->tUnitW);

	if (PowerSavingMode == TDA18204PowerSavingMode_normal )
	{
		/* PowerSavingMode nominal */
		/* set PSM to 0, all PSM are set automatically by HW */
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_1__PSM, 0x0, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	else
	{
		/* PowerSavingMode low */
		/* set PSM to 1, all PSM must by set by SW */
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_1__PSM, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));

		if(err == TM_OK)
		{
			/* PSM_MTOBUF1 bit(s): 17mA PSM=1 state (0x2) */
			/* BOOST_VCO bit(s): 1: On */
			err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_SAVING_MODE_1,  0x12, Bus_NoRead);
			tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
		}
		if(err == TM_OK)
		{
			/* PSM_AGC1 bit(s): 21.8mA PSM=1 state (0x1) */
			/* PSM_AGC2 bit(s): 22.9mA PSM=1 state (0x2) */
			/* PSM_AGC3 bit(s): 26,3mA PSM=1 state (0x1) */
			err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_SAVING_MODE_2,  0x19, Bus_NoRead);
			tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
		}
		if(err == TM_OK)
		{
			/* PSM_AAF_BUF2 bit(s): 5.31mA, PSM=1 state (0x4)*/
			/* PSM_TEQ_BUF bit(s): 8.6mA PSM=1 state (0x1) */
			err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_SAVING_MODE_3,  0x41, Bus_NoRead);
			tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
		}
		if(err == TM_OK)
		{
			/* PSM_RFPPF bit(s): -4.2 mA PSM=1 state (0x1) */
			/* PSM_Mixer bit(s):  0 mA PSM=1 state (0x0) */
			/* PSM_Ifpoly bit(s): -4.1 mA PSM=1 state (0x3) */
			/* PSM_Lodriver bit(s): -3 mA PSM=1 state (0x1) */
			err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_SAVING_MODE_4,  0x4D, Bus_NoRead);
			tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
		}
        if(err == TM_OK)
        {
            /* PSM_lpf bit(s): -4.9 mA PSM=1 state (0x3) */
            /* PSM_hpf_ifn bit(s): -3.44 mA PSM=1 state (0x3) */
            /* PSM_H35 bit(s): 21.3 mA PSM0 state (0x0) */
            /* PSM_VCO bit(s): 2 mA (low) or 5 mA (high) PSM=1 state (0x0) */
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_SAVING_MODE_5,  0xF0, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
        /* Add H3H5 programmability in ES2, so reprogram PSM H3H5 */
        if(err == TM_OK)
        {
            err = iTDA18204_SetBoostH3H5(pObj, TDA18204BoostH3H5_Psm0);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_SetBoostH3H5(0x%08X) failed.", pObj->tUnitW));
        }
    }

	if(err == TM_OK)
	{
		pObj->PowerSavingMode = PowerSavingMode;
	}

	return err;
}


/*============================================================================*/
/* FUNCTION:    iTDA18204_SetMSM                                              */
/*                                                                            */
/* DESCRIPTION: Set the MSM bit(s).                                           */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18204_SetMSM(
    pTDA18204Object_t   pObj,   /* I: Driver object */
    UInt8               uValue, /* I: Item value */
    Bool                bLaunch /* I: Launch MSM */
)
{
    tmErrorCode_t   err = TM_OK;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18204_SetMSM(0x%08X)", pObj->tUnitW);

    /* Set state machine and Launch it */
    err = iTDA18204_Write(pObj, &gTDA18204_Reg_MSM_byte_1, uValue, Bus_None);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));

    if(err == TM_OK && bLaunch)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_MSM_byte_2__MSM_Launch, 0x01, Bus_None);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        err = iTDA18204_WriteRegMap(pObj, gTDA18204_Reg_MSM_byte_1.Address, bLaunch?0x02:0x01);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_WriteRegMap(0x%08X) failed.", pObj->tUnitW));
    }

	/* reset msm */
    if(err == TM_OK && bLaunch)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_MSM_byte_1, 0x00, Bus_None);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
	if(err == TM_OK && bLaunch)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_MSM_byte_2, 0x00, Bus_None);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    iTDA18204_WaitIRQ                                             */
/*                                                                            */
/* DESCRIPTION: Wait the IRQ to trigger                                       */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18204_WaitIRQ(
    pTDA18204Object_t   pObj,       /* I: Driver object */
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

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18204_WaitIRQ(0x%08X)", pObj->tUnitW);

    while(err == TM_OK && (counter--)>0)
    {
        err = iTDA18204_Read(pObj, &gTDA18204_Reg_IRQ_status__IRQ_status, &uIRQ, Bus_RW);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", pObj->tUnitW));

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
            err = iTDA18204_Read(pObj, &gTDA18204_Reg_IRQ_status, &uIRQStatus, Bus_RW);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", pObj->tUnitW));

            if((irqStatus & uIRQStatus) == irqStatus)
            {
                bIRQTriggered = True;
            }
        }

        if(counter)
        {
            err = iTDA18204_Wait(pObj, waitStep);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
        }
    }

    if(err == TM_OK && bIRQTriggered == False)
    {
        err = TDA18204_ERR_NOT_READY;
    }

    return err;
}

tmErrorCode_t iTDA18204_rf_mux_agc2_wb_smooth_switch (pTDA18204Object_t pObj, UInt8 bValue)
{
    tmErrorCode_t  err = TM_OK;
    UInt8          acg2_wb_smooth;

    /**********************************/
	/*  rf_mux_agc2_wb_smooth switch  */
	/**********************************/
	if(err == TM_OK)
	{
		err = iTDA18204_Read(pObj, &gTDA18204_Reg_rf_switch_smooth_state__rf_mux_agc2_wb_smooth_state, &acg2_wb_smooth, Bus_NoWrite);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", pObj->tUnitW));
	}

	if(err == TM_OK)
	{
		if(acg2_wb_smooth != bValue)
		{
			err = iTDA18204_Write(pObj, &gTDA18204_Reg_rf_switchs__rf_mux_acg2_wb_smooth_move, 0x1, Bus_NoRead);
			tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
		}
	}

    /* force to 0 for the map to be according to the HW state */
    if( err == TM_OK )
    {
	    err = iTDA18204_Write(pObj, &gTDA18204_Reg_rf_switchs__rf_mux_acg2_wb_smooth_move, 0x0, Bus_NoRead);
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18204_PowerTransition_x                                   */
/*                                                                            */
/* DESCRIPTION: following functions are the transition of the state machine   */
/*              used in iTDA18204_SetLLPowerState                             */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t iTDA18204_PowerTransition_Init (pTDA18204Object_t pObj)
{
    tmErrorCode_t   err = TM_OK;

    /********************************************/
    /* in any case force DIG clock to INTERNAL  */
    /********************************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_clock_ctrl_byte_1__Dig_Clock, TDA18204_Dig_Clock_INTERNAL_OSCILLATOR_VALUE_0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    return err;
}

tmErrorCode_t iTDA18204_PowerTransition_PowerFullStandbyWithGPIO (pTDA18204Object_t pObj)
{
    tmErrorCode_t   err = TM_OK;

    /**********************************/
    /*  MTO / LTO / AGC1              */
    /**********************************/
    err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_MTO_BUF1, 0x1, Bus_NoRead);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_LT, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_LT_byte_2__RFsw_LT_RFin, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /* AGC1 */
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_1__PD_AGC1, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    /* DETECT 1 */
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__AGC1_DIGCLK_ON, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_DETECT1, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /********************************************************/
    /*  WB path                                             */
    /********************************************************/
    /**********************************/
    /*  TEQ                           */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_TEQ_BUF, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    /**********************************/
    /*  AGC2 WB                       */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_1__PD_AGC2, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /* Switch off AGC2 loop */
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_SPEED__AGC2_LOOP_OFF, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_CONTROL__AGC2_EXT_CTRL, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__AGC2_DIGCLK_ON, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_DETECT2, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /*  AGC3 WB                       */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_1__PD_AGC3, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /*  AAF WB                        */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_AAF_BUF2, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    /**********************************/
    /*  STOB WB                       */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_STOB, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }


    /********************************************************/
    /*  NB path                                             */
    /********************************************************/
    /******************************************/
    /*  AGC2 NB is powered down in any mode   */
    /******************************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_rf_switchs__RF_mux_acg2_nb, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_1__PD_AGC2_NB, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__AGC2_NB_DIGCLK_ON, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_DETECT2_NB, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /*  rf_mux_agc2_wb_smooth switch  */
    /**********************************/
    if( err == TM_OK )
    {
        err = iTDA18204_rf_mux_agc2_wb_smooth_switch (pObj, 0x0);
    }

    /**********************************/
    /*  H3H5 NB                       */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_W_Filter_byte__W_Filter_Enable, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_H3H5, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /*  STOB NB                       */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_STOB_NB, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************************/
    /* IF stob part + RF polyphase + IF polyphase */
    /**********************************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_S2B_RFPPF_IFPPF, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /* MIXER IF + LO                  */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_MIX, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /***************************************************/
    /* GSK + Low Pass filter (used with AGC3) + IF AGC */
    /***************************************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_LPF_IFAGC, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__AGCK_NB_DIGCLK_ON, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__AGC3_NB_DIGCLK_ON, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_DETECT3_NB, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC3_NB_byte_2__Force_AGC3_NB_gain, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_IR_Cal_byte_1__IR_IF_LPF_1p5M, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /* High Pass filter               */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_IF_Byte_2__IF_HP_Enable, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /********************************************************/
    /*  LO part                                             */
    /********************************************************/
    /**********************************/
    /* Xtal                           */
    /**********************************/
    /*  first config XTal */
    if(err == TM_OK)
    {
        /* Set XTAL_IN_CTRL */
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_XTAL_IN_CTRL__XTAL_IN_CTRL, TDA18204_XTAL_IN_CTRL_NO_XTAL_NO_BUFFER_IN_VALUE_0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        /* Set XTAL_IN_CTRL */
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_XTAL_OUT_CTRL__XTout, (UInt8)pObj->XTOut, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.",  pObj->tUnitW));
        /* Set XTAL_IN_CTRL */
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_XTAL_OUT_CTRL__Xtout_Amp, (UInt8)pObj->XTOutAmp, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_7__pd_xtout, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    /* mux after div1p5 */
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_7__pd_xtal_mux, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    /* will be powered when setting RF */
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_6__pd_xtal_div1p5, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    /* used for synchro signal when setting TEQ in function of delay programmed */
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_7__pd_xtout_asym, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /* PLL                            */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_6__pd_pll, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    /* will be powered when setting RF */
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__PD_VCO_LOW, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__PD_VCO_HIGH, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__PD_VCO_EXT, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    /* will be powered when setting RF */
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_6__pd_maindivcmos, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        /* patch calibration pd_md_counter is inversed */
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_6__pd_md_counter, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /* LO Chain                       */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    /* will be powered when setting RF, so all are powered down */
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div1p5, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div2a1, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div2b, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div2c, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div2d, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div2e, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /* LO Chain                       */
    /**********************************/
    /* will be powered when setting RF, so powered down */
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        /* patch calibration */
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_7__pd_sublo, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /* IR Cal                         */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain_calmode, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_7__pd_calgen, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /********************************************************/
    /*  Others part                                         */
    /********************************************************/

    /**********************************/
    /* GPIO                           */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_GPIO, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /* Last power on regulators      */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_1__PD_REG2V8RF, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_1__PD_REG2V8IF, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_1__PD_REG2V8SYNT, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    return err;
}

tmErrorCode_t iTDA18204_PowerTransition_PowerBufMtoLtoStandby (pTDA18204Object_t pObj)
{
    tmErrorCode_t   err = TM_OK;

    /**********************************/
    /*  MTO / LTO / AGC1              */
    /**********************************/
    err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_MTO_BUF1, 0x0, Bus_NoRead);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_LT, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_LT_byte_2__RFsw_LT_RFin, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /* AGC1 */
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_1__PD_AGC1, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    /* DETECT 1 */
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__AGC1_DIGCLK_ON, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_DETECT1, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /********************************************************/
    /*  WB path                                             */
    /********************************************************/
    /**********************************/
    /*  TEQ                           */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_TEQ_BUF, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    /**********************************/
    /*  AGC2 WB                       */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_1__PD_AGC2, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /* internal/external loop selection */
    /* in standalone (without FST), use internal AGC2 loop by default */
    if(err == TM_OK)
    {
        err = iTDA18204_SetAgc2WB(pObj);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_SetAgc2WB(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /*  AGC3 WB                       */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_1__PD_AGC3, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /*  AAF WB                        */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_AAF_BUF2, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    /**********************************/
    /*  STOB WB                       */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_STOB, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }


    /********************************************************/
    /*  NB path                                             */
    /********************************************************/
    /******************************************/
    /*  AGC2 NB is powered down in any mode   */
    /******************************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_rf_switchs__RF_mux_acg2_nb, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_1__PD_AGC2_NB, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__AGC2_NB_DIGCLK_ON, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_DETECT2_NB, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /*  rf_mux_agc2_wb_smooth switch  */
    /**********************************/
    if( err == TM_OK )
    {
        err = iTDA18204_rf_mux_agc2_wb_smooth_switch (pObj, 0x0);
    }

    /**********************************/
    /*  H3H5 NB                       */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_W_Filter_byte__W_Filter_Enable, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_H3H5, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /*  STOB NB                       */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_STOB_NB, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************************/
    /* IF stob part + RF polyphase + IF polyphase */
    /**********************************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_S2B_RFPPF_IFPPF, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /* MIXER IF + LO                  */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_MIX, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /***************************************************/
    /* GSK + Low Pass filter (used with AGC3) + IF AGC */
    /***************************************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_LPF_IFAGC, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__AGCK_NB_DIGCLK_ON, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__AGC3_NB_DIGCLK_ON, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_DETECT3_NB, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC3_NB_byte_2__Force_AGC3_NB_gain, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_IR_Cal_byte_1__IR_IF_LPF_1p5M, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /* High Pass filter               */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_IF_Byte_2__IF_HP_Enable, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /********************************************************/
    /*  LO part                                             */
    /********************************************************/
    /**********************************/
    /* Xtal                           */
    /**********************************/
    /*  first config XTal */
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_7__pd_xtal_mux, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /* check if TDA18204 is used in system or in standalone, and set XTAL_IN_CTRL */
    if(err == TM_OK)
    {
        /* Set XTAL_IN_CTRL */
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_XTAL_IN_CTRL__XTAL_IN_CTRL, (UInt8)pObj->XTin, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_clock_ctrl_byte_1__Dig_Clock, TDA18204_Dig_Clock_XTAL_IN_OR_BUFFER_VALUE_1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        /* Set XTAL_IN_CTRL */
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_XTAL_OUT_CTRL__XTout, (UInt8)pObj->XTOut, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.",  pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        /* Set XTAL_IN_CTRL */
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_XTAL_OUT_CTRL__Xtout_Amp, (UInt8)pObj->XTOutAmp, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_7__pd_xtout, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    /* mux after div1p5 */
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    /* will be powered when setting RF */
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_6__pd_xtal_div1p5, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    /* used for synchro signal when setting TEQ in function of delay programmed */
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_7__pd_xtout_asym, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /* PLL                            */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_6__pd_pll, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    /* will be powered when setting RF */
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__PD_VCO_LOW, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__PD_VCO_HIGH, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__PD_VCO_EXT, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    /* will be powered when setting RF */
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_6__pd_maindivcmos, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        /* patch calibration pd_md_counter is inversed */
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_6__pd_md_counter, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /* LO Chain                       */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    /* will be powered when setting RF, so all are powered down */
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div1p5, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div2a1, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div2b, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div2c, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div2d, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div2e, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /* LO Chain                       */
    /**********************************/
    /* will be powered when setting RF, so powered down */
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        /* patch calibration */
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_7__pd_sublo, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /* IR Cal                         */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain_calmode, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_7__pd_calgen, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /********************************************************/
    /*  Others part                                         */
    /********************************************************/

    /**********************************/
    /* GPIO                           */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_GPIO, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /* Last power on regulators       */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_1__PD_REG2V8RF, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_1__PD_REG2V8IF, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_1__PD_REG2V8SYNT, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }			

    return err;
}

tmErrorCode_t iTDA18204_PowerTransition_PowerCalibrationNormalMode (pTDA18204Object_t pObj)
{
    tmErrorCode_t   err = TM_OK;

    /**********************************/
    /* First power on regulators      */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_1__PD_REG2V8RF, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_1__PD_REG2V8IF, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_1__PD_REG2V8SYNT, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /*  MTO / LTO / AGC1              */
    /**********************************/
    err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_MTO_BUF1, 0x1, Bus_NoRead);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_LT, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_LT_byte_2__RFsw_LT_RFin, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /* AGC1 */
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_1__PD_AGC1, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    /* DETECT 1 */
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__AGC1_DIGCLK_ON, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_DETECT1, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /********************************************************/
    /*  WB path                                             */
    /********************************************************/
    /**********************************/
    /*  TEQ                           */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_TEQ_BUF, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    /**********************************/
    /*  AGC2 WB                       */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_1__PD_AGC2, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /* Switch off AGC2 loop */
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_SPEED__AGC2_LOOP_OFF, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_CONTROL__AGC2_EXT_CTRL, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__AGC2_DIGCLK_ON, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_DETECT2, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /*  AGC3 WB                       */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_1__PD_AGC3, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /*  AAF WB                        */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_AAF_BUF2, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    /**********************************/
    /*  STOB WB                       */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_STOB, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /********************************************************/
    /*  NB path                                             */
    /********************************************************/
    /******************************************/
    /*  AGC2 NB is powered down in any mode   */
    /******************************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }

    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_rf_switchs__RF_mux_acg2_nb, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_1__PD_AGC2_NB, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__AGC2_NB_DIGCLK_ON, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_DETECT2_NB, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /*  rf_mux_agc2_wb_smooth switch  */
    /**********************************/
    if( err == TM_OK )
    {
        err = iTDA18204_rf_mux_agc2_wb_smooth_switch (pObj, 0x0);
    }

    /**********************************/
    /*  H3H5 NB                       */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_W_Filter_byte__W_Filter_Enable, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_H3H5, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /*  STOB NB                       */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_STOB_NB, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************************/
    /* IF stob part + RF polyphase + IF polyphase */
    /**********************************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_S2B_RFPPF_IFPPF, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /* MIXER IF + LO                  */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_MIX, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /***************************************************/
    /* GSK + Low Pass filter (used with AGC3) + IF AGC */
    /***************************************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_LPF_IFAGC, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__AGCK_NB_DIGCLK_ON, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__AGC3_NB_DIGCLK_ON, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_DETECT3_NB, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC3_NB_byte_2__Force_AGC3_NB_gain, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC3_NB_byte_2__AGC3_NB_GAIN, TDA18204_AGC3_NB_GAIN_15_VALUE_7, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_IR_Cal_byte_1__IR_IF_LPF_1p5M, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /* State machine                  */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_RSSI_byte_2__RSSI_Gv, TDA18204_RSSI_Gv_MINUS4_5_VALUE_3, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /* ES2 specific */
    if ( pObj->Revision == 0x20)
    {
        if(err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_IR_Cal_byte_2__IR_Step_Delta, TDA18204_IR_Step_Delta_7_VALUE_7, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
    }
    /* ES1, ES1MF specific */
    else
    {
        if(err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_IR_Cal_byte_2__IR_Step_Delta, TDA18204_IR_Step_Delta_5_VALUE_5, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
    }

    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_IR_Cal_byte_1__IR_Mixer_Atten, TDA18204_IR_Mixer_Atten_MAX_VALUE_7, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_RCCAL_OFFSET__RCCAL_OFFSET, TDA18204_RCCAL_OFFSET_0_VALUE_2, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_IF_Byte_1__IF_LPF_Offset, TDA18204_IF_LPF_Offset_12_VALUE_3, Bus_RW);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /* High Pass filter               */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_IF_Byte_2__IF_HP_Enable, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /********************************************************/
    /*  LO part                                             */
    /********************************************************/
    /**********************************/
    /* Xtal                           */
    /**********************************/
    /*  first config XTal */
    if(err == TM_OK)
    {
        /* Set XTAL_IN_CTRL */
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_XTAL_IN_CTRL__XTAL_IN_CTRL, (UInt8)pObj->XTin, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        /* Set XTAL_IN_CTRL */
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_XTAL_OUT_CTRL__XTout, (UInt8)pObj->XTOut, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.",  pObj->tUnitW));
        /* Set XTAL_IN_CTRL */
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_XTAL_OUT_CTRL__Xtout_Amp, (UInt8)pObj->XTOutAmp, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_7__pd_xtout, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    /* mux after div1p5 */
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_7__pd_xtal_mux, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    /* will be powered when setting RF */
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        /* patch calibration */
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_6__pd_xtal_div1p5, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    /* used for synchro signal when setting TEQ in function of delay programmed */
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_7__pd_xtout_asym, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /* PLL                            */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_6__pd_pll, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    /* will be powered when setting RF */
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__PD_VCO_LOW, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__PD_VCO_HIGH, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__PD_VCO_EXT, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    /* will be powered when setting RF */
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_6__pd_maindivcmos, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        /* patch calibration pd_md_counter is inversed */
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_6__pd_md_counter, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /* LO Chain                       */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    /* will be powered when setting RF or Calibration, so all are powered down */
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div1p5, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        /* patch calibration */
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div2a1, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div2b, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div2c, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div2d, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div2e, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /* LO Chain                       */
    /**********************************/
    /* will be powered when setting RF, so powered down */
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        /* patch calibration */
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_7__pd_sublo, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /* IR Cal                         */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain_calmode, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_7__pd_calgen, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /********************************************************/
    /*  Others part                                         */
    /********************************************************/
    /**********************************/
    /* GPIO                           */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_GPIO, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    return err;
}

tmErrorCode_t iTDA18204_PowerTransition_Standby (pTDA18204Object_t pObj)
{
	tmErrorCode_t   err = TM_OK;

    /**********************************/
    /* Standby                        */
    /**********************************/
	/**********************************/
	/*  MTO / LTO / AGC1              */
	/**********************************/
	err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_MTO_BUF1, 0x1, Bus_NoRead);
	tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_LT, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_LT_byte_2__RFsw_LT_RFin, 0x0, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}

	/* AGC1 */
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_1__PD_AGC1, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	/* DETECT 1 */
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__AGC1_DIGCLK_ON, 0x0, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_DETECT1, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}

	/********************************************************/
	/*  WB path                                             */
	/********************************************************/
	/**********************************/
	/*  TEQ                           */
	/**********************************/
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_TEQ_BUF, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	/**********************************/
	/*  AGC2 WB                       */
	/**********************************/
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_1__PD_AGC2, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}

    /* Switch off AGC2 loop */
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_SPEED__AGC2_LOOP_OFF, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_CONTROL__AGC2_EXT_CTRL, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__AGC2_DIGCLK_ON, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_DETECT2, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
	
	/**********************************/
	/*  AGC3 WB                       */
	/**********************************/
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_1__PD_AGC3, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}

	/**********************************/
	/*  AAF WB                        */
	/**********************************/
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_AAF_BUF2, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	/**********************************/
	/*  STOB WB                       */
	/**********************************/
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_STOB, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}


	/********************************************************/
	/*  NB path                                             */
	/********************************************************/
	/******************************************/
	/*  AGC2 NB is powered down in any mode   */
	/******************************************/
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}

	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_rf_switchs__RF_mux_acg2_nb, 0x0, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_1__PD_AGC2_NB, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__AGC2_NB_DIGCLK_ON, 0x0, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_DETECT2_NB, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}

	/**********************************/
	/*  rf_mux_agc2_wb_smooth switch  */
    /**********************************/
    if (err == TM_OK)
    {
        err = iTDA18204_rf_mux_agc2_wb_smooth_switch (pObj, 0x0);
    }

	/**********************************/
	/*  H3H5 NB                       */
	/**********************************/
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_W_Filter_byte__W_Filter_Enable, 0x0, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_H3H5, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}

	/**********************************/
	/*  STOB NB                       */
	/**********************************/
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_STOB_NB, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}

	/**********************************************/
	/* IF stob part + RF polyphase + IF polyphase */
	/**********************************************/
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_S2B_RFPPF_IFPPF, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}

	/**********************************/
	/* MIXER IF + LO                  */
	/**********************************/
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_MIX, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}

	/***************************************************/
	/* GSK + Low Pass filter (used with AGC3) + IF AGC */
	/***************************************************/
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_LPF_IFAGC, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__AGCK_NB_DIGCLK_ON, 0x0, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__AGC3_NB_DIGCLK_ON, 0x0, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_DETECT3_NB, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC3_NB_byte_2__Force_AGC3_NB_gain, 0x0, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_IR_Cal_byte_1__IR_IF_LPF_1p5M, 0x0, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}

	/**********************************/
	/* High Pass filter               */
	/**********************************/
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_IF_Byte_2__IF_HP_Enable, 0x0, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}

	/********************************************************/
	/*  LO part                                             */
	/********************************************************/
	/**********************************/
	/* Xtal                           */
	/**********************************/
	/*  first config XTal */
	if(err == TM_OK)
	{
		/* Set XTAL_IN_CTRL */
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_XTAL_IN_CTRL__XTAL_IN_CTRL, TDA18204_XTAL_IN_CTRL_NO_XTAL_NO_BUFFER_IN_VALUE_0, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
		/* Set XTAL_IN_CTRL */
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_XTAL_OUT_CTRL__XTout, (UInt8)pObj->XTOut, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.",  pObj->tUnitW));
		/* Set XTAL_IN_CTRL */
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_XTAL_OUT_CTRL__Xtout_Amp, (UInt8)pObj->XTOutAmp, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_7__pd_xtout, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	/* mux after div1p5 */
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_7__pd_xtal_mux, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	/* will be powered when setting RF */
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_6__pd_xtal_div1p5, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	/* used for synchro signal when setting TEQ in function of delay programmed */
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_7__pd_xtout_asym, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}

	/**********************************/
	/* PLL                            */
	/**********************************/
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_6__pd_pll, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	/* will be powered when setting RF */
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__PD_VCO_LOW, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__PD_VCO_HIGH, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__PD_VCO_EXT, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	/* will be powered when setting RF */
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_6__pd_maindivcmos, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		/* patch calibration pd_md_counter is inversed */
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_6__pd_md_counter, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}

	/**********************************/
	/* LO Chain                       */
	/**********************************/
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	/* will be powered when setting RF, so all are powered down */
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div1p5, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div2a1, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div2b, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div2c, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div2d, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div2e, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}

	/**********************************/
	/* LO Chain                       */
	/**********************************/
	/* will be powered when setting RF, so powered down */
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		/* patch calibration */
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_7__pd_sublo, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}

	/**********************************/
	/* IR Cal                         */
	/**********************************/
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain_calmode, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_7__pd_calgen, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}

	/********************************************************/
	/*  Others part                                         */
	/********************************************************/

	/**********************************/
	/* GPIO                           */
	/**********************************/
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_GPIO, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}

	/**********************************/
	/* Last power off regulators      */
	/**********************************/
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_1__PD_REG2V8RF, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_1__PD_REG2V8IF, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_1__PD_REG2V8SYNT, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}


    return err;
}

tmErrorCode_t iTDA18204_PowerTransition_WbOn (pTDA18204Object_t pObj)
{
	tmErrorCode_t   err = TM_OK;

    /********************************************************/
	/*  WB path                                             */
	/********************************************************/
	
	/**********************************/
	/* IR Cal                         */
	/**********************************/
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain_calmode, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_7__pd_calgen, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	
	/**********************************/
	/*  TEQ                           */
	/**********************************/
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_TEQ_BUF, 0x0, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	/**********************************/
	/*  AGC2 WB                       */
	/**********************************/
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_1__PD_AGC2, 0x0, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}

    /* internal/external loop selection */
    /* in standalone (without FST), use internal AGC2 loop by default */
    if(err == TM_OK)
    {
        err = iTDA18204_SetAgc2WB(pObj);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_SetAgc2WBs(0x%08X) failed.", pObj->tUnitW));
    }

	/**********************************/
	/*  AGC3 WB                       */
	/**********************************/
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		/* patch calibration */
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_1__PD_AGC3, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}

	/**********************************/
	/*  AAF WB                        */
	/**********************************/
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_AAF_BUF2, 0x0, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	/**********************************/
	/*  STOB WB                       */
	/**********************************/
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_STOB, 0x0, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}

	/**********************************/
	/*  rf_mux_agc2_wb_smooth switch  */
    /**********************************/
    if (err == TM_OK)
    {
        err = iTDA18204_rf_mux_agc2_wb_smooth_switch (pObj, 0x0);
    }

    return err;
}

tmErrorCode_t iTDA18204_PowerTransition_WbOff (pTDA18204Object_t pObj)
{
	tmErrorCode_t   err = TM_OK;

  	/********************************************************/
	/*  WB path                                             */
	/********************************************************/
	/**********************************/
	/*  TEQ                           */
	/**********************************/
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_TEQ_BUF, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	/**********************************/
	/*  AGC2 WB                       */
	/**********************************/
    if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_1__PD_AGC2, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}

    /* Switch off AGC2 loop */
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_SPEED__AGC2_LOOP_OFF, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_CONTROL__AGC2_EXT_CTRL, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__AGC2_DIGCLK_ON, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_DETECT2, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

	/**********************************/
	/*  AGC3 WB                       */
	/**********************************/
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		/* patch calibration */
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_1__PD_AGC3, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}

	/**********************************/
	/*  AAF WB                        */
	/**********************************/
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_AAF_BUF2, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	/**********************************/
	/*  STOB WB                       */
	/**********************************/
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_STOB, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}

	/**********************************/
	/*  rf_mux_agc2_wb_smooth switch  */
	/**********************************/
    if (err == TM_OK)
    {
        err = iTDA18204_rf_mux_agc2_wb_smooth_switch (pObj, 0x0);
    }
	
    return err;
}

tmErrorCode_t iTDA18204_PowerTransition_NbOn (pTDA18204Object_t pObj)
{
    tmErrorCode_t   err = TM_OK;
    UInt8           AGC2NBGainForced = 0;
    UInt8           AGC2NBGainInternal = 0;
    UInt8           Valread = 0;

    /**********************************/
    /* First power on regulators      */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_1__PD_REG2V8IF, 0x0, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_1__PD_REG2V8SYNT, 0x0, Bus_NoRead); /* 1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /********************************************************/
    /*  NB path                                             */
    /********************************************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__AGC2_NB_DIGCLK_ON, 0x1, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_rf_switchs__RF_mux_acg2_nb, 0x1, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    /* check if PowerTransition_NbOff performed  :  gain forced  */
    err  = iTDA18204_Read(pObj, &gTDA18204_Reg_AGC2_NB_byte_1__Force_AGC2_NB_gain, &Valread, Bus_None);
    if (Valread == 1)
    {
        /* get current forced gain */
        err  = iTDA18204_Read(pObj, &gTDA18204_Reg_AGC2_NB_byte_4__AGC2_NB_GAIN, &AGC2NBGainForced, Bus_None);

        /* get current internal AGC gain with free action */
        err  = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_NB_byte_1__Force_AGC2_NB_gain, 0x00, Bus_NoRead);
        if(err == TM_OK)
        {
            err  = iTDA18204_Read(pObj, &gTDA18204_Reg_AGC2_NB_Gain_status__AGC2_NB_GAIN_READ, &AGC2NBGainInternal, Bus_NoRead);
        }
        if(err == TM_OK)
        {
               err  = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_NB_byte_1__Force_AGC2_NB_gain, 0x01, Bus_NoRead);
        }
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_DETECT2_NB, 0x0, Bus_NoRead); /* 5mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_1__PD_AGC2_NB, 0x0, Bus_NoRead); /* 17mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    /* increase AGC2 NB forced gain up to internal gain to avoid uncor when gain released */
    while (err == TM_OK && AGC2NBGainForced < AGC2NBGainInternal )
    {
        /* step  per 0.2dB(1) up to  "free running" internal value */
        AGC2NBGainForced = AGC2NBGainForced +1;
        if(err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_NB_byte_4__AGC2_NB_GAIN, AGC2NBGainForced, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_NB_byte_1__Force_AGC2_NB_gain, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_NB_byte_2__AGC2_NB_loop_off, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /*  H3H5 NB                       */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_W_Filter_byte__W_Filter_Enable, 0x0, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_H3H5, 0x0, Bus_NoRead); /* 26mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /*  STOB NB                       */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_STOB_NB, 0x0, Bus_NoRead); /* 1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************************/
    /* IF stob part + RF polyphase + IF polyphase */
    /**********************************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_S2B_RFPPF_IFPPF, 0x0, Bus_NoRead); /* 36mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /* MIXER IF + LO                  */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_MIX, 0x0, Bus_NoRead); /* 26mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /***************************************************/
    /* GSK + Low Pass filter (used with AGC3) + IF AGC */
    /***************************************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_LPF_IFAGC, 0x0, Bus_NoRead); /* 29mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__AGCK_NB_DIGCLK_ON, 0x1, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__AGC3_NB_DIGCLK_ON, 0x1, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_DETECT3_NB, 0x0, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC3_NB_byte_2__Force_AGC3_NB_gain, 0x0, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_IR_Cal_byte_1__IR_IF_LPF_1p5M, 0x0, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /* High Pass filter               */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_IF_Byte_2__IF_HP_Enable, 0x0, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /********************************************************/
    /*  LO part                                             */
    /********************************************************/
    /**********************************/
    /* Xtal                           */
    /**********************************/
    /*  first config XTal */
    if(err == TM_OK)
    {
        /* Set XTAL_IN_CTRL */
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_XTAL_IN_CTRL__XTAL_IN_CTRL, (UInt8)pObj->XTin, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        /* Set XTAL_IN_CTRL */
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_XTAL_OUT_CTRL__XTout, (UInt8)pObj->XTOut, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.",  pObj->tUnitW));
        /* Set XTAL_IN_CTRL */
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_XTAL_OUT_CTRL__Xtout_Amp, (UInt8)pObj->XTOutAmp, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_7__pd_xtout, 0x1, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    /* mux after div1p5 */
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_7__pd_xtal_mux, 0x0, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    /* will be powered when setting RF */
    if(err == TM_OK)
    {
        /* patch calibration */
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_6__pd_xtal_div1p5, 0x0, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    /* used for synchro signal when setting TEQ in function of delay programmed */
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_7__pd_xtout_asym, 0x1, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /* PLL                            */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_6__pd_pll, 0x0, Bus_NoRead); /* 7mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    /* will be powered when setting RF */
    if(err == TM_OK)
    {
        /* patch calibration */
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__PD_VCO_LOW, 0x1, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__PD_VCO_HIGH, 0x1, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__PD_VCO_EXT, 0x1, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    /* will be powered when setting RF */
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_6__pd_maindivcmos, 0x0, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        /* patch calibration pd_md_counter is inversed */
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_6__pd_md_counter, 0x1, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /* LO Chain                       */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain, 0x0, Bus_NoRead); /* 5mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    /* will be powered when setting RF, so all are powered down */
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div1p5, 0x1, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        /* patch calibration */
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div2a1, 0x0, Bus_NoRead); /* 1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div2b, 0x1, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div2c, 0x1, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div2d, 0x1, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div2e, 0x1, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /* LO Chain                       */
    /**********************************/
    /* will be powered when setting RF, so powered down */
    if(err == TM_OK)
    {
        /* patch calibration */
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_7__pd_sublo, 0x1, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /* IR Cal                         */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain_calmode, 0x1, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_7__pd_calgen, 0x1, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /*  rf_mux_agc2_wb_smooth switch  */
    /**********************************/
    if( err == TM_OK )
    {
        err = iTDA18204_rf_mux_agc2_wb_smooth_switch (pObj, 0x0);
    }

    return err;
}

tmErrorCode_t iTDA18204_PowerTransition_NbOff (pTDA18204Object_t pObj)
{
    tmErrorCode_t   err = TM_OK;
    UInt8           uAGC2NBGain = 0;

    /********************************************************/
    /*  NB path                                             */
    /********************************************************/

    /* similar algorithm to iTDA18204_SetAgc2NB if AGC2 NB > 1dB */

    /* AGC2 loop off enable */
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_NB_byte_2__AGC2_NB_loop_off, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    /* retrieve AGC2_NB_GAIN_READ */
    if(err == TM_OK)
    {
        err = iTDA18204_Read(pObj, &gTDA18204_Reg_AGC2_NB_Gain_status__AGC2_NB_GAIN_READ, &uAGC2NBGain, Bus_NoWrite);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", pObj->tUnitW));
    }
    /* Force AGC2 NB GAIN to same free value */
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_NB_byte_4__AGC2_NB_GAIN, uAGC2NBGain, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_NB_byte_1__Force_AGC2_NB_gain, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    /* AGC2 NB < 1dB is required before switching off AGC2 NB */
    /* to avoid impacting the WB path due to a sudden impedance change */
    while (err == TM_OK && uAGC2NBGain > 65)
    {
        /* step down of 0.2dB(1) until  1dB(65) */
        uAGC2NBGain = uAGC2NBGain-1;

        /* Force AGC2 NB GAIN */
        if(err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_NB_byte_4__AGC2_NB_GAIN, uAGC2NBGain, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
    }

    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_rf_switchs__RF_mux_acg2_nb, 0x0, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_DETECT2_NB, 0x1, Bus_NoRead); /* 4mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_1__PD_AGC2_NB, 0x1, Bus_NoRead); /* 16mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__AGC2_NB_DIGCLK_ON, 0x0, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /*  H3H5 NB                       */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_W_Filter_byte__W_Filter_Enable, 0x0, Bus_NoRead); /* 9mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_H3H5, 0x1, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /*  STOB NB                       */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_STOB_NB, 0x1, Bus_NoRead); /* 5mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************************/
    /* IF stob part + RF polyphase + IF polyphase */
    /**********************************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_S2B_RFPPF_IFPPF, 0x1, Bus_NoRead); /* 30mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /* MIXER IF + LO                  */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_MIX, 0x1, Bus_NoRead); /* 27mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /***************************************************/
    /* GSK + Low Pass filter (used with AGC3) + IF AGC */
    /***************************************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_LPF_IFAGC, 0x1, Bus_NoRead); /* 29mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__AGCK_NB_DIGCLK_ON, 0x0, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__AGC3_NB_DIGCLK_ON, 0x0, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_DETECT3_NB, 0x1, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC3_NB_byte_2__Force_AGC3_NB_gain, 0x0, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_IR_Cal_byte_1__IR_IF_LPF_1p5M, 0x1, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /* High Pass filter               */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_IF_Byte_2__IF_HP_Enable, 0x0, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /********************************************************/
    /*  LO part                                             */
    /********************************************************/
    /**********************************/
    /* Xtal                           */
    /**********************************/
    /*  first config XTal */
    if(err == TM_OK)
    {
        /* Set XTAL_IN_CTRL */
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_XTAL_IN_CTRL__XTAL_IN_CTRL, (UInt8)pObj->XTin, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        /* Set XTAL_IN_CTRL */
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_XTAL_OUT_CTRL__XTout, (UInt8)pObj->XTOut, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.",  pObj->tUnitW));
        /* Set XTAL_IN_CTRL */
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_XTAL_OUT_CTRL__Xtout_Amp, (UInt8)pObj->XTOutAmp, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_7__pd_xtout, 0x1, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    /* mux after div1p5 */
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_7__pd_xtal_mux, 0x0, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    /* will be powered when setting RF */
    if(err == TM_OK)
    {
        /* patch calibration */
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_6__pd_xtal_div1p5, 0x0, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    /* used for synchro signal when setting TEQ in function of delay programmed */
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_7__pd_xtout_asym, 0x1, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /* PLL                            */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_6__pd_pll, 0x1, Bus_NoRead); /* 7mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    /* will be powered when setting RF */
    if(err == TM_OK)
    {
        /* patch calibration */
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__PD_VCO_LOW, 0x1, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__PD_VCO_HIGH, 0x1, Bus_NoRead); /* 10mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__PD_VCO_EXT, 0x1, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    /* will be powered when setting RF */
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_6__pd_maindivcmos, 0x1, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        /* patch calibration pd_md_counter is inversed */
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_6__pd_md_counter, 0x1, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /* LO Chain                       */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain, 0x1, Bus_NoRead); /* 6mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    /* will be powered when setting RF, so all are powered down */
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div1p5, 0x1, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        /* patch calibration */
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div2a1, 0x1, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div2b, 0x1, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div2c, 0x1, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div2d, 0x1, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain_div2e, 0x1, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /* LO Chain                       */
    /**********************************/
    /* will be powered when setting RF, so powered down */
    if(err == TM_OK)
    {
        /* patch calibration */
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_7__pd_sublo, 0x1, Bus_NoRead); /* 2mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /* IR Cal                         */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5__pd_lochain_calmode, 0x1, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_7__pd_calgen, 0x1, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /* Last power off regulators      */
    /**********************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_1__PD_REG2V8IF, 0x1, Bus_NoRead); /* <1mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_1__PD_REG2V8SYNT, 0x1, Bus_NoRead); /* 2mA */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /**********************************/
    /*  rf_mux_agc2_wb_smooth switch  */
    /**********************************/
    if( err == TM_OK )
    {
        err = iTDA18204_rf_mux_agc2_wb_smooth_switch (pObj, 0x0);
    }

    return err;
}

tmErrorCode_t iTDA18204_PowerTransition_CommonOn (pTDA18204Object_t pObj)
{
	tmErrorCode_t   err = TM_OK;
    
	/**********************************/
	/* First power on regulators      */
	/**********************************/
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_1__PD_REG2V8RF, 0x0, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_1__PD_REG2V8IF, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_1__PD_REG2V8SYNT, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}

	/**********************************/
	/*  MTO / LTO / AGC1              */
	/**********************************/
	err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_MTO_BUF1, 0x1, Bus_NoRead);
	tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_LT, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_LT_byte_2__RFsw_LT_RFin, 0x0, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}

	/* AGC1 */
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_1__PD_AGC1, 0x0, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	/* DETECT 1 */
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__AGC1_DIGCLK_ON, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_DETECT1, 0x0, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}

    /********************************************************/
	/*  Others part                                         */
	/********************************************************/
	/**********************************/
	/* GPIO                           */
	/**********************************/
	if(err == TM_OK)
	{
		err = iTDA18204_Wait(pObj, pObj->PowerDownSmoothDelay);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
	}
	if(err == TM_OK)
	{
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_4__PD_GPIO, 0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18204_SetLLPowerState                                     */
/*                                                                            */
/* DESCRIPTION: Sets the power state.                                         */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18204_SetLLPowerState(
                          pTDA18204Object_t       pObj,       /* I: Driver object */
                          TDA18204PowerState_t    powerState  /* I: Power state of TDA18204 */
                         )
{
    tmErrorCode_t   err = TM_OK;
    Bool bRepeatStateMachine;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18204_SetLLPowerState(0x%08X)", pObj->tUnitW);

    /* Check if Hw is ready to operate */
    err = iTDA18204_CheckHwState(pObj, TDA18204_HwStateCaller_SetPower);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_CheckHwState(0x%08X) failed.", pObj->tUnitW));

    if(err == TM_OK)
    {
        do{
            /* by default: DO NOT RECALL THE STATE MACHINE! */           
            bRepeatStateMachine = False;

            /* Handle the Power State machine */
            switch( pObj->curLLPowerState )
            {
            case TDA18204_PowerFullStandby:
                switch(powerState)
                {
                case TDA18204_PowerFullStandby:
                    /* nothing to be done */
                    break;
                case TDA18204_PowerFullStandbyWithGPIO:
                    err = iTDA18204_PowerTransition_Init( pObj );
                    if (err == TM_OK)
                    {
                        err = iTDA18204_PowerTransition_PowerFullStandbyWithGPIO( pObj );
                    }
                    break;
                case TDA18204_PowerBufMtoLtoStandby:
                    err = iTDA18204_PowerTransition_Init( pObj );
                    if (err == TM_OK)
                    {
                        err = iTDA18204_PowerTransition_PowerBufMtoLtoStandby( pObj );
                    }
                    break;
                case TDA18204_PowerCalibrationNormalMode:
                    err = iTDA18204_PowerTransition_Init( pObj );
                    if (err == TM_OK)
                    {
                        err = iTDA18204_PowerTransition_PowerCalibrationNormalMode( pObj );
                    }
                    break;

                case TDA18204_PowerSciNormalMode:
                    err = iTDA18204_PowerTransition_Init( pObj );
                    if (err == TM_OK)
                    {
                        err = iTDA18204_PowerTransition_CommonOn( pObj );
                    }
                    if (err == TM_OK)
                    {
                        err = iTDA18204_PowerTransition_WbOn( pObj );
                    }
                    break;
                case TDA18204_PowerNbNormalMode:
                    err = iTDA18204_PowerTransition_Init( pObj );
                    if (err == TM_OK)
                    {
                        err = iTDA18204_PowerTransition_CommonOn( pObj );
                    }
                    if (err == TM_OK)
                    {
                        err = iTDA18204_PowerTransition_NbOn( pObj );
                    }
                    break;
                case TDA18204_PowerSciNbNormalMode:
                    err = iTDA18204_PowerTransition_Init( pObj );
                    if (err == TM_OK)
                    {
                        err = iTDA18204_PowerTransition_CommonOn( pObj );
                    }
                    if (err == TM_OK)
                    {
                        err = iTDA18204_PowerTransition_WbOn( pObj );
                    }
                    if (err == TM_OK)
                    {
                        err = iTDA18204_PowerTransition_NbOn( pObj );
                    }
                    break;

                default:
                    err = TDA18204_ERR_BAD_PARAMETER;
                    break;
                }
                break;

            case TDA18204_PowerFullStandbyWithGPIO:
                switch(powerState)
                {
                case TDA18204_PowerFullStandby:
                    err = iTDA18204_PowerTransition_Init( pObj );
                    if (err == TM_OK)
                    {
                        err = iTDA18204_PowerTransition_Standby( pObj );
                    }
                    break;
                case TDA18204_PowerFullStandbyWithGPIO:
                    /* nothing to be done */
                    break;
                case TDA18204_PowerBufMtoLtoStandby:
                    err = iTDA18204_PowerTransition_Init( pObj );
                    if (err == TM_OK)
                    {
                        err = iTDA18204_PowerTransition_PowerBufMtoLtoStandby( pObj );
                    }
                    break;
                case TDA18204_PowerCalibrationNormalMode:
                    err = iTDA18204_PowerTransition_Init( pObj );
                    if (err == TM_OK)
                    {
                        err = iTDA18204_PowerTransition_PowerCalibrationNormalMode( pObj );
                    }
                    break;

                case TDA18204_PowerSciNormalMode:
                case TDA18204_PowerNbNormalMode:
                case TDA18204_PowerSciNbNormalMode:
                    err = iTDA18204_PowerTransition_Standby( pObj );
                    if (err == TM_OK)
                    {
                        /* change current state and recall the state machine in order to perform transition from Standby */
                        pObj->curLLPowerState = TDA18204_PowerFullStandby;
                        bRepeatStateMachine = True;
                    }
                    break;

                default:
                    err = TDA18204_ERR_BAD_PARAMETER;
                    break;
                }
                break;

            case TDA18204_PowerBufMtoLtoStandby:
                switch(powerState)
                {
                case TDA18204_PowerFullStandby:
                    err = iTDA18204_PowerTransition_Init( pObj );
                    if (err == TM_OK)
                    {
                        err = iTDA18204_PowerTransition_Standby( pObj );
                    }
                    break;
                case TDA18204_PowerFullStandbyWithGPIO:
                    err = iTDA18204_PowerTransition_Init( pObj );
                    if (err == TM_OK)
                    {
                        err = iTDA18204_PowerTransition_PowerFullStandbyWithGPIO( pObj );
                    }
                    break;
                case TDA18204_PowerBufMtoLtoStandby:
                    /* nothing to be done */
                    break;
                case TDA18204_PowerCalibrationNormalMode:
                    err = iTDA18204_PowerTransition_Init( pObj );
                    if (err == TM_OK)
                    {
                        err = iTDA18204_PowerTransition_PowerCalibrationNormalMode( pObj );
                    }
                    break;

                case TDA18204_PowerSciNormalMode:
                case TDA18204_PowerNbNormalMode:
                case TDA18204_PowerSciNbNormalMode:
                    err = iTDA18204_PowerTransition_Init( pObj );
                    if (err == TM_OK)
                    {
                        err = iTDA18204_PowerTransition_Standby( pObj );
                    }
                    if (err == TM_OK)
                    {
                        /* change current state and recall the state machine in order to perform transition from Standby */
                        pObj->curLLPowerState = TDA18204_PowerFullStandby;
                        bRepeatStateMachine = True;
                    }
                    break;

                default:
                    err = TDA18204_ERR_BAD_PARAMETER;
                    break;
                }
                break;

            case TDA18204_PowerCalibrationNormalMode:
                switch(powerState)
                {
                case TDA18204_PowerFullStandby:
                    err = iTDA18204_PowerTransition_Init( pObj );
                    if (err == TM_OK)
                    {
                        err = iTDA18204_PowerTransition_Standby( pObj );
                    }
                    break;
                case TDA18204_PowerFullStandbyWithGPIO:
                    err = iTDA18204_PowerTransition_Init( pObj );
                    if (err == TM_OK)
                    {
                        err = iTDA18204_PowerTransition_PowerFullStandbyWithGPIO( pObj );
                    }
                    break;
                case TDA18204_PowerBufMtoLtoStandby:
                    err = iTDA18204_PowerTransition_Init( pObj );
                    if (err == TM_OK)
                    {
                        err = iTDA18204_PowerTransition_PowerBufMtoLtoStandby( pObj );
                    }
                    break;
                case TDA18204_PowerCalibrationNormalMode:
                    /* nothing to be done */
                    break;

                case TDA18204_PowerSciNormalMode:
                case TDA18204_PowerNbNormalMode:
                case TDA18204_PowerSciNbNormalMode:
                    err = iTDA18204_PowerTransition_Standby( pObj );
                    if (err == TM_OK)
                    {
                        /* change current state and recall the state machine in order to perform transition from Standby */
                        pObj->curLLPowerState = TDA18204_PowerFullStandby;
                        bRepeatStateMachine = True;
                    }
                    break;

                default:
                    err = TDA18204_ERR_BAD_PARAMETER;
                    break;
                }
                break;

            case TDA18204_PowerSciNormalMode:
                switch(powerState)
                {
                case TDA18204_PowerFullStandby:
                    err = iTDA18204_PowerTransition_Init( pObj );
                    if (err == TM_OK)
                    {
                        err = iTDA18204_PowerTransition_Standby( pObj );
                    }
                    break;
                case TDA18204_PowerFullStandbyWithGPIO:
                    err = iTDA18204_PowerTransition_Init( pObj );
                    if (err == TM_OK)
                    {
                        err = iTDA18204_PowerTransition_PowerFullStandbyWithGPIO( pObj );
                    }
                    break;
                case TDA18204_PowerBufMtoLtoStandby:
                    err = iTDA18204_PowerTransition_Init( pObj );
                    if (err == TM_OK)
                    {
                        err = iTDA18204_PowerTransition_PowerBufMtoLtoStandby( pObj );
                    }
                    break;
                case TDA18204_PowerCalibrationNormalMode:
                    err = iTDA18204_PowerTransition_Init( pObj );
                    if (err == TM_OK)
                    {
                        err = iTDA18204_PowerTransition_PowerCalibrationNormalMode( pObj );
                    }
                    break;

                case TDA18204_PowerSciNormalMode:
                    /* nothing to be done */
                    break;
                case TDA18204_PowerNbNormalMode:
                    err = iTDA18204_PowerTransition_Init( pObj );
                    if (err == TM_OK)
                    {
                        err = iTDA18204_PowerTransition_WbOff( pObj );
                    }
                    if (err == TM_OK)
                    {
                        err = iTDA18204_PowerTransition_NbOn( pObj );
                    }
                    break;
                case TDA18204_PowerSciNbNormalMode:
                    err = iTDA18204_PowerTransition_Init( pObj );
                    if (err == TM_OK)
                    {
                        err = iTDA18204_PowerTransition_NbOn( pObj );
                    }
                    break;
                default:
                    err = TDA18204_ERR_BAD_PARAMETER;
                    break;
                }
                break;

            case TDA18204_PowerNbNormalMode:
                switch(powerState)
                {
                case TDA18204_PowerFullStandby:
                    err = iTDA18204_PowerTransition_Init( pObj );
                    if (err == TM_OK)
                    {
                        err = iTDA18204_PowerTransition_Standby( pObj );
                    }
                    break;
                case TDA18204_PowerFullStandbyWithGPIO:
                    err = iTDA18204_PowerTransition_Init( pObj );
                    if (err == TM_OK)
                    {
                        err = iTDA18204_PowerTransition_PowerFullStandbyWithGPIO( pObj );
                    }
                    break;
                case TDA18204_PowerBufMtoLtoStandby:
                    err = iTDA18204_PowerTransition_Init( pObj );
                    if (err == TM_OK)
                    {
                        err = iTDA18204_PowerTransition_PowerBufMtoLtoStandby( pObj );
                    }
                    break;
                case TDA18204_PowerCalibrationNormalMode:
                    err = iTDA18204_PowerTransition_Init( pObj );
                    if (err == TM_OK)
                    {
                        err = iTDA18204_PowerTransition_PowerCalibrationNormalMode( pObj );
                    }
                    break;

                case TDA18204_PowerSciNormalMode:
                    err = iTDA18204_PowerTransition_Init( pObj );
                    if (err == TM_OK)
                    {
                        err = iTDA18204_PowerTransition_NbOff( pObj );
                    }
                    if (err == TM_OK)
                    {
                        err = iTDA18204_PowerTransition_WbOn( pObj );
                    }
                    break;
                case TDA18204_PowerNbNormalMode:
                    /* nothing to be done */
                    break;
                case TDA18204_PowerSciNbNormalMode:
                    err = iTDA18204_PowerTransition_Init( pObj );
                    if (err == TM_OK)
                    {
                        err = iTDA18204_PowerTransition_WbOn( pObj );
                    }
                    break;
                default:
                    err = TDA18204_ERR_BAD_PARAMETER;
                    break;
                }
                break;

            case TDA18204_PowerSciNbNormalMode:
                switch(powerState)
                {
                case TDA18204_PowerFullStandby:
                    err = iTDA18204_PowerTransition_Init( pObj );
                    if (err == TM_OK)
                    {
                        err = iTDA18204_PowerTransition_Standby( pObj );
                    }
                    break;
                case TDA18204_PowerFullStandbyWithGPIO:
                    err = iTDA18204_PowerTransition_Init( pObj );
                    if (err == TM_OK)
                    {
                        err = iTDA18204_PowerTransition_PowerFullStandbyWithGPIO( pObj );
                    }
                    break;
                case TDA18204_PowerBufMtoLtoStandby:
                    err = iTDA18204_PowerTransition_Init( pObj );
                    if (err == TM_OK)
                    {
                        err = iTDA18204_PowerTransition_PowerBufMtoLtoStandby( pObj );
                    }
                    break;
                case TDA18204_PowerCalibrationNormalMode:
                    err = iTDA18204_PowerTransition_Init( pObj );
                    if (err == TM_OK)
                    {
                        err = iTDA18204_PowerTransition_PowerCalibrationNormalMode( pObj );
                    }
                    break;

                case TDA18204_PowerSciNormalMode:
                    err = iTDA18204_PowerTransition_Init( pObj );
                    if (err == TM_OK)
                    {
                        err = iTDA18204_PowerTransition_NbOff( pObj );
                    }
                    break;
                case TDA18204_PowerNbNormalMode:
                    err = iTDA18204_PowerTransition_Init( pObj );
                    if (err == TM_OK)
                    {
                        err = iTDA18204_PowerTransition_WbOff( pObj );
                    }
                    break;
                case TDA18204_PowerSciNbNormalMode:
                    /* nothing to be done */
                    break;
                default:
                    err = TDA18204_ERR_BAD_PARAMETER;
                    break;
                }
                break;

            case TDA18204_PowerMax:
                /* only after initialisation*/
                switch(powerState)
                {
                case TDA18204_PowerFullStandby:
                case TDA18204_PowerFullStandbyWithGPIO:
                case TDA18204_PowerBufMtoLtoStandby:
                    /* check that those transitions are not possible */
                    err =  TDA18204_ERR_BAD_PARAMETER;
                    break;
                case TDA18204_PowerCalibrationNormalMode:
                    err = iTDA18204_PowerTransition_PowerCalibrationNormalMode( pObj );
                    break;
                default:
                    err =  TDA18204_ERR_BAD_PARAMETER;
                    break;
                }
                break;
            default:
                /* invalid state */
                err =  TDA18204_ERR_BAD_PARAMETER;
                break;
            }

            /* in some case, it is required to run the state machine another time */
            /* (i.e. first go in Standby mode then perform the transion)          */
        } while( (bRepeatStateMachine == True) && (err == TM_OK));
    }

    if(err == TM_OK)
    {
        /* Store low-level power state in driver instance */
        pObj->curLLPowerState = powerState;

        /* Store power state in driver instance */
        switch(powerState)
        {
        case TDA18204_PowerFullStandby:
        case TDA18204_PowerFullStandbyWithGPIO:
        case TDA18204_PowerBufMtoLtoStandby:
        case TDA18204_PowerCalibrationNormalMode:
            pObj->curPowerState = tmPowerStandby;
            break;

        case TDA18204_PowerSciNormalMode:
        case TDA18204_PowerNbNormalMode:
        case TDA18204_PowerSciNbNormalMode:
            pObj->curPowerState = tmPowerOn;
            break;

        default:
            break;
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18204_SetRF:                                              */
/*                                                                            */
/* DESCRIPTION: Tunes to a RF.                                                */
/*                                                                            */
/* RETURN:      TM_OK                                                         */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18204_SetRF(
    pTDA18204Object_t   pObj    /* I: Driver object */
)
{
	tmErrorCode_t   err = TM_OK;
	UInt8  uValue = 0;
	UInt8  uTemperature = 0;
	UInt8  uDeltaTemperature = 0;

	tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18204_SetRF(0x%08X)", pObj->tUnitW);

	if(pObj->curPowerState != tmPowerOn)
	{

        if ((pObj->StandardMode == TDA18204_TUNER_QAM_6MHz) ||
            (pObj->StandardMode == TDA18204_TUNER_QAM_8MHz) ||
            (pObj->StandardMode == TDA18204_TUNER_OOB_2MHz))
		{
			/* Set power state ON */
			err = iTDA18204_SetLLPowerState(pObj, TDA18204_PowerSciNbNormalMode);
			tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_SetLLPowerState(0x%08X, TDA18204_PowerSciNbNormalMode) failed.", pObj->tUnitW));
		}
		else
		{
			/* Set RF has no sens if not in tuner mode */
			err = TDA18204_ERR_NOT_SUPPORTED;
		}
	}

	if(err == TM_OK)
	{
		pObj->curPowerState = tmPowerOn;

		/* Check if Dig_Clock is in XTAL_IN_OR_BUFFER */
		err = iTDA18204_Read(pObj, &gTDA18204_Reg_clock_ctrl_byte_1__Dig_Clock, &uValue, Bus_NoWrite);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", pObj->tUnitW));

		if ( (err == TM_OK) && (uValue != TDA18204_Dig_Clock_XTAL_IN_OR_BUFFER_VALUE_1))
		{
			err = iTDA18204_Write(pObj, &gTDA18204_Reg_clock_ctrl_byte_1__Dig_Clock, TDA18204_Dig_Clock_XTAL_IN_OR_BUFFER_VALUE_1, Bus_NoRead);
			tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
		}
	}

    /* If |Delta Temperature| > 15°, need to do a new FVCOmax calibration */
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_THERMO_2__TM_ON, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Read(pObj, &gTDA18204_Reg_THERMO_1__TM_D, &uTemperature, Bus_NoWrite);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_THERMO_2__TM_ON, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if (uTemperature >= pObj->Temperature)
    {
        uDeltaTemperature = uTemperature - pObj->Temperature;
    }
    else
    {
        uDeltaTemperature = pObj->Temperature - uTemperature;
    }
    if (uDeltaTemperature > 15)
    {
        /* relaunch calibration */
        err = iTDA18204_FVCOMaxCalibration(pObj);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_FVCOMaxCalibration failed.", pObj->tUnitW));
    }

	if(err == TM_OK)
	{
		/* Update RF Filter path value */
		err = iTDA18204_SetRFFilters(pObj);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_WriteRFFilters failed.", pObj->tUnitW));
	}

	if(err == TM_OK)
	{
		/* Set Local Oscillator synthesis */
		err = iTDA18204_SetLO(pObj);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_SetLO failed.", pObj->tUnitW));
	}



	return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18204_ResetAgc2WB                                         */
/*                                                                            */
/* DESCRIPTION: Resets AGC2 (internal reset)                                  */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18204_ResetAgc2WB(
    pTDA18204Object_t   pObj   /* I: Driver object */
)
{
    tmErrorCode_t   err = TM_OK;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18204_ResetAgc2WB(0x%08X)", pObj->tUnitW);

    /* Pass AGC2 in internal AGC strategy */
    /* set AGC2_EXT_CTRL to 0, because was set to 1 at POR */
    if (err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_CONTROL__AGC2_EXT_CTRL, 0x00, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /* set PD_DETECT2 to 0 */
    if (err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_DETECT2, 0x00, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /* Write the AGC2 TOP (write TOP UP before TOP DOWN) */
    /* Change default value of AGC2_G_UP : POR = 9 -> 16 */
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_G_UP__AGC2_G_UP, pObj->agc2WBGud[TDA18204_AGC2_Gud_Up], Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    /* Change default value of AGC2_G_DOWN : POR = 8 -> 15 */
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_G_DOWN__AGC2_G_DOWN, pObj->agc2WBGud[TDA18204_AGC2_Gud_Down], Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /* AGC2 Bypass START (respect the order) */
    /* set AGC2_DOWN_STEP and AGC2_UP_STEP in order to be equivalent to AGC2_BYPASS set to 1 */
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_STEP__AGC2_DOWN_STEP, TDA18204_AGC2_DOWN_STEP_0_04_VALUE_6, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_STEP__AGC2_UP_STEP, TDA18204_AGC2_UP_STEP_0_09_VALUE_11, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    
    /*  Start AGC2 clock : AGC2_DIGCLK_ON */
    if (err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__AGC2_DIGCLK_ON, 0x01, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /*  wait 100 ms */
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, 100);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnit));
    }

    /* AGC2 Bypass STOP (respect the order) */
    /* set AGC2_DOWN_STEP and AGC2_UP_STEP in order to be equivalent to AGC2_BYPASS set to 0 */
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_STEP__AGC2_UP_STEP, TDA18204_AGC2_UP_STEP_6_86_VALUE_5, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_STEP__AGC2_DOWN_STEP, TDA18204_AGC2_DOWN_STEP_3_42_VALUE_0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18204_SetAgc2WB                                           */
/*                                                                            */
/* DESCRIPTION: Configure AGC2 WB loop (external or internal)                 */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18204_SetAgc2WB(
    pTDA18204Object_t   pObj   /* I: Driver object */
)
{
    tmErrorCode_t   err = TM_OK;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18204_SetAgc2WB(0x%08X)", pObj->tUnitW);

    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_SPEED__AGC2_LOOP_OFF, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    switch(pObj->agc2WBCtrl)
    {
    /* AGC2 internal loop */
    case TDA18204_AGC2_WB_Ctrl_Internal:

        err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_CONTROL__AGC2_EXT_CTRL, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));

        if(err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__AGC2_DIGCLK_ON, 0x1, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }

        if(err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_DETECT2, 0x0, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
        break;

    /* AGC2 external loop */
    case TDA18204_AGC2_WB_Ctrl_External:

        err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_CONTROL__AGC2_EXT_CTRL, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));

        if(err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_3__AGC2_DIGCLK_ON, 0x0, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }

        if(err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_DETECT2, 0x1, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
        break;

    default:
        err = TDA18204_ERR_BAD_PARAMETER;
        break;
    }

	return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18204_SetAgc2NB                                           */
/*                                                                            */
/* DESCRIPTION: Enable/Disable AGC2 NB in a smooth way to avoid uncorrs       */
/*              On WB path                                                    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18204_SetAgc2NB(
    pTDA18204Object_t   pObj,   /* I: Driver object */
    Bool                Agc2NBHPEnable /* I: AGC2 NB high-pass enable */
)
{
    tmErrorCode_t   err = TM_OK;
    UInt8           uAGC2NBGainReadCurrent = 0;
    UInt8           uAGC2NBLoopOffCurrent = 0;
    UInt8           uAGC2NBGainForceCurrent = 0;
    UInt8           uAGC2NBGain = 0;
    Int32           AGC2NBGainX100 = 0; /* 100 x AGC2 NB gain (dB) */
    UInt8           uNbIteration = 0;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18204_SetAgc2NB(0x%08X)", pObj->tUnitW);

    /* retrieve AGC2_NB_GAIN_READ */
    if(err == TM_OK)
    {
        err = iTDA18204_Read(pObj, &gTDA18204_Reg_AGC2_NB_Gain_status__AGC2_NB_GAIN_READ, &uAGC2NBGainReadCurrent, Bus_NoWrite);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", pObj->tUnitW));
    }
    AGC2NBGainX100 = -1200 + 20 * uAGC2NBGainReadCurrent;

    /* AGC2 NB < 9dB is required before toggling AGC2 NB high-pass filter */
    /* to avoid impacting the WB path due to a sudden impedance change */
    if ( AGC2NBGainX100 > 900)
    {
        /* retrieve AGC2_NB_loop_off and Force_AGC2_NB_gain */
        if(err == TM_OK)
        {
            err = iTDA18204_Read(pObj, &gTDA18204_Reg_AGC2_NB_byte_2__AGC2_NB_loop_off, &uAGC2NBLoopOffCurrent, Bus_NoWrite);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            err = iTDA18204_Read(pObj, &gTDA18204_Reg_AGC2_NB_byte_1__Force_AGC2_NB_gain, &uAGC2NBGainForceCurrent, Bus_NoWrite);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", pObj->tUnitW));
        }

        /* AGC2 loop off enable */
        if(err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_NB_byte_2__AGC2_NB_loop_off, 0x1, Bus_RW);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }

        while ( AGC2NBGainX100 > 900 )
        {
            AGC2NBGainX100 -= 100;
            uNbIteration += 1;
            uAGC2NBGain = (UInt8)((AGC2NBGainX100/20)+60);

            /* Force AGC2 NB GAIN */
            if(err == TM_OK)
            {
                err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_NB_byte_4__AGC2_NB_GAIN, uAGC2NBGain, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
            }
            if(err == TM_OK)
            {
                err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_NB_byte_1__Force_AGC2_NB_gain, 0x1, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
            }
            if(err == TM_OK)
            {
                /* wait 1 ms */
                err = iTDA18204_Wait(pObj, 1);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
            }
        }

        /* AGC2_NB_HP_EN */
        if(err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_NB_byte_5__AGC2_NB_HP_EN, (UInt8)Agc2NBHPEnable, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }

        /* inversion loop to restore AGC2 NB Gain */
        while ( uNbIteration != 0 )
        {
            AGC2NBGainX100 += 100;
            uNbIteration -= 1;
            uAGC2NBGain = (UInt8)((AGC2NBGainX100/20)+60);

            /* Force AGC2 NB GAIN */
            if(err == TM_OK)
            {
                err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_NB_byte_4__AGC2_NB_GAIN, uAGC2NBGain, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
            }
            if(err == TM_OK)
            {
                err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_NB_byte_1__Force_AGC2_NB_gain, 0x1, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
            }
            if(err == TM_OK)
            {
                /* wait 1 ms */
                err = iTDA18204_Wait(pObj, 1);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
            }
        }
        /* restore AGC2_NB_loop_off and Force_AGC2_NB_gain  */
        if(err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_NB_byte_2__AGC2_NB_loop_off, uAGC2NBLoopOffCurrent, Bus_RW);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_NB_byte_1__Force_AGC2_NB_gain, uAGC2NBGainForceCurrent, Bus_RW);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
    }
    else
    {
        /* AGC2_NB_HP_EN */
        if (err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_NB_byte_5__AGC2_NB_HP_EN, (UInt8)Agc2NBHPEnable, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18204_SoftReset                                           */
/*                                                                            */
/* DESCRIPTION: Perform a soft reset in HW                                    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18204_SoftReset(
    pTDA18204Object_t   pObj   /* I: Driver object */
)
{
    tmErrorCode_t   err = TM_OK;
    UInt8 uRccal = 0;
    
    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18204_SoftReset(0x%08X)", pObj->tUnitW);

    /* read RCCAL status */
    err = iTDA18204_Read(pObj, &gTDA18204_Reg_RCCAL_READ__RCCAL,  &uRccal, Bus_NoWrite);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", pObj->tUnitW));
    
    /* Force SOFT_RESET, so now when writting 0x01 in SOFT_RESET it will be effective and executed */
    /* once the soft reset is done, FORCE_SOFT_RESET is set to 0x00 */
    if (err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWERDOWN__FORCE_SOFT_RESET,  0x01, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /* SOFT_RESET */
    if (err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWERDOWN__SOFT_RESET,  0x01, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /* FORCE_SOFT_RESET = 0x00 */
    if (err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWERDOWN__FORCE_SOFT_RESET,  0x00, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /* restore RCCAL status */
    if (err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_RCCAL_FORCE__RCCAL_FORCE,  0x01, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if (err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_RCCAL_FORCE__RCCAL_BYPASS_VALUE, uRccal, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18204_GetRSSI                                             */
/*                                                                            */
/* DESCRIPTION: Reads RSSI.                                                   */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18204_GetRSSI(
    pTDA18204Object_t   pObj,   /* I: Driver object */
    UInt8*              puValue /* O: RSSI value */
)
{
    tmErrorCode_t   err = TM_OK;
    UInt8 uValue = 0;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18204_GetRSSI(0x%08X)", pObj->tUnitW);

    *puValue = 0;


    /* Set IRQ_clear*/
    err = iTDA18204_Write(pObj, &gTDA18204_Reg_IRQ_clear, TDA18204_IRQ_Global|TDA18204_IRQ_GetPowerLevel, Bus_NoRead);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));

    /* patch for ES2 */
    if ( pObj->Revision == 0x20)
    {		
        /* retrieve Dig_Clock in order to restore it after iTDA18204_SetMSM() */
        if(err == TM_OK)
        {
            err = iTDA18204_Read(pObj, &gTDA18204_Reg_clock_ctrl_byte_1__Dig_Clock, &uValue, Bus_NoWrite);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", pObj->tUnitW));
        }

        if(err == TM_OK)
        {
            /* Set state machine and Launch it */
            err = iTDA18204_SetMSM(pObj, TDA18204_MSM_GetPowerLevel, True);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_SetMSM(0x%08X, TDA18204_MSM_GetPowerLevel) failed.", pObj->tUnitW));
        }

        if(err == TM_OK)
        {
            err = iTDA18204_WaitIRQ(pObj, 700, 1, TDA18204_IRQ_GetPowerLevel);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_WaitIRQ(0x%08X) failed.", pObj->tUnitW));
        }

        /* restore Dig_Clock */
        if (err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_clock_ctrl_byte_1__Dig_Clock, uValue, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
    }
    else
    {
        if(err == TM_OK)
        {
            /* Set state machine and Launch it */
            err = iTDA18204_SetMSM(pObj, TDA18204_MSM_GetPowerLevel, True);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_SetMSM(0x%08X, TDA18204_MSM_GetPowerLevel) failed.", pObj->tUnitW));
        }

        if(err == TM_OK)
        {
            err = iTDA18204_WaitIRQ(pObj, 700, 1, TDA18204_IRQ_GetPowerLevel);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_WaitIRQ(0x%08X) failed.", pObj->tUnitW));
        }
    }

    if(err == TM_OK)
    {
        /* Read Power_Level_byte */
        err = iTDA18204_Read(pObj, &gTDA18204_Reg_RSSI_byte_1__RSSI, puValue, Bus_RW);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", pObj->tUnitW));

        if(err != TM_OK)
        {
            err = TDA18204_ERR_HW_FAILED;
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18204_GetPowerLevel                                       */
/*                                                                            */
/* DESCRIPTION: Get the input PowerLevel from RSSI                            */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18204_GetPowerLevel(
    pTDA18204Object_t   pObj,     /* I: Driver object */
	UInt8           uRSSIValue,   /* I: RSSI value reading previously                */
	Int32*          pPowerLevel   /* O: Address of the variable to output item value */
)
{
    tmErrorCode_t   err = TM_OK;
	Int32           PowerLevel = 0; /* in (x100) dBµV */
    Int32           OffsetIFLevel = -970; /* in (x100) dBµV */

    Int32           DeltaAGC1NBGain = 0; /* in dB x 1000 */
    Int32           DeltaAGC2NBGain = 0; /* in dB x 1000 */
    Int32           DeltaAGC3NBGain = 0; /* in dB x 1000 */
    Int32           GainMaxReference = 0; /* in dB x 1000 */


    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18204_GetPowerLevel(0x%08X)", pObj->tUnitW);

    /*************************************/
    /*  RSSI contribution                */
    /*************************************/
	/* For 0 < RSSI[7:0] <= 203  => RSSI value= 109.5 - RSSI[7:0] *0.25 (in dBµV) */
	/* For RSSI[7:0] > 204     => RSSI value = 58.75dBµVrms (in dBµV) */
	if ( uRSSIValue > 204 )
	{
		PowerLevel = 10950;
	}
	else
	{
		PowerLevel = 5875 + uRSSIValue*25;
	}

    /***********************************************************/
    /*  initialize OffsetIFLevelin function of ES HW version   */
    /***********************************************************/
    /* ES2 specific */
    if ( pObj->Revision == 0x20)
    {
        OffsetIFLevel = -970;
    }
    else
    {
        OffsetIFLevel = -625;
    }

    /**********************************************************/
    /*  Retrieve reference Max Gain of NB path (in dB x 1000) */
    /**********************************************************/
    err = iTDA18204_GetRefMaxGainNB(pObj, &GainMaxReference);

    /*************************************/
    /*  AGC1 contribution (in dB x 1000) */
    /*************************************/
    if (err == TM_OK)
    {
        err = iTDA18204_LvlAgc1NBContribution(pObj, &DeltaAGC1NBGain);
    }

    /****************************************/
    /*  AGC2 NB contribution (in dB x 1000) */
    /****************************************/
    if (err == TM_OK)
    {
        err = iTDA18204_LvlAgc2NBContribution(pObj, &DeltaAGC2NBGain);
    }

    /****************************************/
    /*  AGC3 NB contribution (in dB x 1000) */
    /****************************************/
    if (err == TM_OK)
    {
        err = iTDA18204_LvlAgc3NBContribution(pObj, &DeltaAGC3NBGain);
    }

    //*pPowerLevel = PowerLevel + ((DeltaAGC1NBGain + DeltaAGC2NBGain + DeltaAGC3NBGain + DeltaRFFilterGain)/10);
    *pPowerLevel = PowerLevel + OffsetIFLevel - ((GainMaxReference + DeltaAGC1NBGain + DeltaAGC2NBGain + DeltaAGC3NBGain)/10);

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18204_CalculateGain                                       */
/*                                                                            */
/* DESCRIPTION: Calculate the overall Gain                                    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18204_CalculateGain(
    pTDA18204Object_t   pObj,        /* I: Driver object */
    UInt32              uRF,         /* I: RF frequency in MHz */
    Int32*              pGain        /* O: Gain value in dB x 1000 */
)
{
    tmErrorCode_t   err = TM_OK;
    Int32           DeltaAGC1Gain = 0;
    Int32           DeltaAGC2Gain = 0;
    Int32           TEQGain = 0;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18204_CalculateGain(0x%08X)", pObj->tUnitW);

    /* calculate AGC1 contribution in dB x 1000 */
    err = iTDA18204_LvlAgc1Contribution(pObj, uRF, &DeltaAGC1Gain);

    /* calculate AGC2 contribution in dB x 1000 */
    if (err == TM_OK)
    {
        err = iTDA18204_LvlAgc2Contribution(pObj, uRF, &DeltaAGC2Gain);
    }

    /* calculate TEQ contribution in dB x 1000 */
    if (err == TM_OK)
    {
        err = iTDA18204_LvlTeqContribution(pObj, uRF, &TEQGain);
    }

    *pGain = DeltaAGC1Gain + DeltaAGC2Gain + TEQGain;

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18204_LvlAgc1Contribution                                 */
/*                                                                            */
/* DESCRIPTION: AGC1 Contribution for iTDA18204_CalculateGain()               */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18204_LvlAgc1Contribution(
    pTDA18204Object_t   pObj,        /* I: Driver object */
    UInt32              uRF,         /* I: RF frequency in MHz */
    Int32*              pDeltaGain   /* O: Delta AGC1 Gain value in dB (x1000) */
)
{
    tmErrorCode_t   err = TM_OK;
    UInt32          uIndex = 0;
    UInt8           uAGC1GainRead = 0;
    UInt8           uAGC1GainForce = 0;
    Int32           Gain = 0;
	Int32           GainX3 = 0;
	Int32           GainX2 = 0;
	Int32           GainX1 = 0;
	Int32           GainX0 = 0;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18204_LvlAgc1Contribution(0x%08X)", pObj->tUnitW);

    /* retrieve AGC1_GAIN_READ */
    err = iTDA18204_Read(pObj, &gTDA18204_Reg_AGC1_byte_3__Force_AGC1_gain, &uAGC1GainForce, Bus_NoWrite);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", pObj->tUnitW));

    if (err == TM_OK)
    {
        /* if AGC1 Gain is forced, the real value should be read in AGC1_Gain */
        if ( uAGC1GainForce == 0x0 )
        {
            err = iTDA18204_Read(pObj, &gTDA18204_Reg_AGC1_LT_Gain_status__AGC1_gain_read, &uAGC1GainRead, Bus_NoWrite);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", pObj->tUnitW));
        }
        else
        {
            err = iTDA18204_Read(pObj, &gTDA18204_Reg_AGC1_byte_3__AGC1_Gain, &uAGC1GainRead, Bus_NoWrite);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", pObj->tUnitW));
        }
    }

    /* RSSI coefficients initialization, in function of HW version */
    /* ES2 specific */
    if ( pObj->Revision == 0x20)
    {
        if (err == TM_OK)
        {
            while ( (uAGC1GainRead > Lvl_AGC1_ES2[uIndex][TDA18204_Coeff_X_Code]) && (uIndex < TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC1_MAX) )
                uIndex = uIndex + 1;

            if (uIndex == TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC1_MAX)
            {
                uIndex = uIndex - 1;
            } 
        }

        /*   gain_AGC1 (dB)        = X3*RF^3/100000000000 + X2*RF^2/100000000 + X1*RF/1000000 + X0/100   */
        /*   gain_AGC1 (dB * 1000) = X3*RF^3/100000000 + X2*RF^2/100000 + X1*RF/10 + X0*10   */
        if (err == TM_OK)
        {    
            GainX3 = (Int32)((Int32)(uRF * uRF * uRF)/(Int32)10000);
            GainX3 = (Int32)(Lvl_AGC1_ES2[uIndex][TDA18204_Coeff_X3] * GainX3 );
            GainX2 = (Int32)(Lvl_AGC1_ES2[uIndex][TDA18204_Coeff_X2] * ((uRF * uRF )/10) );
            GainX1 = (Int32)((Int32)(Lvl_AGC1_ES2[uIndex][TDA18204_Coeff_X1] * uRF) * (Int32)10 );
            GainX0 = (Int32)( Lvl_AGC1_ES2[uIndex][TDA18204_Coeff_X0] * 100000 );
            Gain =   (Int32)( GainX3 + GainX2 + GainX1 + GainX0 );
            Gain =   Gain / (Int32)10000;
        }
    }
    else
    {
        if (err == TM_OK)
        {
            while ( (uAGC1GainRead > Lvl_AGC1_ES1[uIndex][TDA18204_Coeff_X_Code]) && (uIndex < TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC1_MAX) )
                uIndex = uIndex + 1;

            if (uIndex == TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC1_MAX)
            {
                uIndex = uIndex - 1;
            } 
        }

        /*   gain_AGC1 (dB)        = X3*RF^3/100000000000 + X2*RF^2/100000000 + X1*RF/1000000 + X0/100   */
        /*   gain_AGC1 (dB * 1000) = X3*RF^3/100000000 + X2*RF^2/100000 + X1*RF/10 + X0*10   */
        if (err == TM_OK)
        {    
            GainX3 = (Int32)((Int32)(uRF * uRF * uRF)/(Int32)10000);
            GainX3 = (Int32)(Lvl_AGC1_ES1[uIndex][TDA18204_Coeff_X3] * GainX3 );
            GainX2 = (Int32)(Lvl_AGC1_ES1[uIndex][TDA18204_Coeff_X2] * ((uRF * uRF )/10) );
            GainX1 = (Int32)((Int32)(Lvl_AGC1_ES1[uIndex][TDA18204_Coeff_X1] * uRF) * (Int32)10 );
            GainX0 = (Int32)( Lvl_AGC1_ES1[uIndex][TDA18204_Coeff_X0] * 100000 );
            Gain =   (Int32)( GainX3 + GainX2 + GainX1 + GainX0 );
            Gain =   Gain / (Int32)10000;
        }
    }

	*pDeltaGain = (Int32)Gain;

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18204_LvlAgc2Contribution                                 */
/*                                                                            */
/* DESCRIPTION: AGC2 Contribution for iTDA18204_CalculateGain()               */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18204_LvlAgc2Contribution(
    pTDA18204Object_t   pObj,        /* I: Driver object */
    UInt32              uRF,         /* I: RF frequency in MHz */
    Int32*              pDeltaGain   /* O: Delta AGC2 Gain value in dB (x1000) */
)
{
    tmErrorCode_t   err = TM_OK;
    UInt32          uIndex = 0;
    UInt8           uAGC2GainRead = 0;
    Int32           Gain = 0;
	Int32           GainX3 = 0;
	Int32           GainX2 = 0;
	Int32           GainX1 = 0;
	Int32           GainX0 = 0;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18204_LvlAgc2Contribution(0x%08X)", pObj->tUnitW);

    /* retrieve AGC2_GAIN_READ */
    err = iTDA18204_Read(pObj, &gTDA18204_Reg_AGC2_GAIN_STATUS__AGC2_GAIN_READ, &uAGC2GainRead, Bus_NoWrite);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", pObj->tUnitW));

    /* RSSI coefficients initialization, in function of HW version */
    /* ES2 specific */
    if ( pObj->Revision == 0x20)
    {
        if (err == TM_OK)
        {
            while ( (uAGC2GainRead > Lvl_AGC2WB_ES2[uIndex][TDA18204_Coeff_X_Code]) && (uIndex < TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC2_MAX) )
                uIndex = uIndex + 1;

            if (uIndex == TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC2_MAX)
            {
                uIndex = uIndex - 1;
            }
        }

        /*   gain_AGC2 (dB)        = X3*RF^3/100000000000 + X2*RF^2/100000000 + X1*RF/100000 + X0/100   */
        /*   gain_AGC2 (dB * 1000) = X3*RF^3/100000000 + X2*RF^2/100000 + X1*RF/100 + X0*10   */
        if (err == TM_OK)
        {    
            GainX3 = (Int32)((Int32)(uRF * uRF * uRF)/(Int32)10000);
            GainX3 = (Int32)(Lvl_AGC2WB_ES2[uIndex][TDA18204_Coeff_X3] * GainX3 );
            GainX2 = (Int32)(Lvl_AGC2WB_ES2[uIndex][TDA18204_Coeff_X2] * ((uRF * uRF )/10) );
            GainX1 = (Int32)((Int32)(Lvl_AGC2WB_ES2[uIndex][TDA18204_Coeff_X1] * uRF) * (Int32)100 );
            GainX0 = (Int32)( Lvl_AGC2WB_ES2[uIndex][TDA18204_Coeff_X0] * 100000 );
            Gain =   (Int32)( GainX3 + GainX2 + GainX1 + GainX0 );
            Gain =   Gain / (Int32)10000;
            Gain =   Gain - (Int32)((Lvl_AGC2WB_ES2[uIndex][TDA18204_Coeff_X_Code]- uAGC2GainRead)*200);
        }
    }
    else
    {
        if (err == TM_OK)
        {
            while ( (uAGC2GainRead > Lvl_AGC2WB_ES1[uIndex][TDA18204_Coeff_X_Code]) && (uIndex < TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC2_MAX) )
                uIndex = uIndex + 1;

            if (uIndex == TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC2_MAX)
            {
                uIndex = uIndex - 1;
            }
        }

        /*   gain_AGC2 (dB)        = X3*RF^3/100000000000 + X2*RF^2/100000000 + X1*RF/100000 + X0/100   */
        /*   gain_AGC2 (dB * 1000) = X3*RF^3/100000000 + X2*RF^2/100000 + X1*RF/100 + X0*10   */
        if (err == TM_OK)
        {    
            GainX3 = (Int32)((Int32)(uRF * uRF * uRF)/(Int32)10000);
            GainX3 = (Int32)(Lvl_AGC2WB_ES1[uIndex][TDA18204_Coeff_X3] * GainX3 );
            GainX2 = (Int32)(Lvl_AGC2WB_ES1[uIndex][TDA18204_Coeff_X2] * ((uRF * uRF )/10) );
            GainX1 = (Int32)((Int32)(Lvl_AGC2WB_ES1[uIndex][TDA18204_Coeff_X1] * uRF) * (Int32)100 );
            GainX0 = (Int32)( Lvl_AGC2WB_ES1[uIndex][TDA18204_Coeff_X0] * 100000 );
            Gain =   (Int32)( GainX3 + GainX2 + GainX1 + GainX0 );
            Gain =   Gain / (Int32)10000;
            Gain =   Gain - (Int32)((Lvl_AGC2WB_ES1[uIndex][TDA18204_Coeff_X_Code]- uAGC2GainRead)*200);
        }
    }

    *pDeltaGain = (Int32)Gain;

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18204_LvlTeqContribution                                  */
/*                                                                            */
/* DESCRIPTION: TEQ Contribution for iTDA18204_CalculateGain()                */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18204_LvlTeqContribution(
    pTDA18204Object_t   pObj,        /* I: Driver object */
    UInt32              uRF,         /* I: RF frequency in MHz */
    Int32*              pGain        /* O: TEQ Gain value in dB (x1000) */
)
{
	tmErrorCode_t   err = TM_OK;
	UInt32          uIndex = 0;
	UInt8           uTEQSelected = 0;
	Int32           Gain = 0;
	Int32           GainX3 = 0;
	Int32           GainX2 = 0;
	Int32           GainX1 = 0;
	Int32           GainX0 = 0;

	tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18204_LvlTeqContribution(0x%08X)", pObj->tUnitW);

	/* retrieve TEQ_GAIN */
	err = iTDA18204_Read(pObj, &gTDA18204_Reg_TEQ_SELECT__TEQ_SELECT, &uTEQSelected, Bus_NoWrite);
	tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", pObj->tUnitW));

    /* RSSI coefficients initialization, in function of HW version */
    /* ES2 specific */
    if ( pObj->Revision == 0x20)
    {
        if (err == TM_OK)
        {
            while ( (uTEQSelected > Lvl_TEQ_ES2[uIndex][TDA18204_Coeff_X_Code]) && (uIndex < TDA18204_INSTANCE_CUSTOM_COMMON_LVL_TEQ_MAX) )
                uIndex = uIndex + 1;

            if (uIndex == TDA18204_INSTANCE_CUSTOM_COMMON_LVL_TEQ_MAX)
            {
                uIndex = uIndex - 1;
            }
        }

        /*   gain_TEQ (dB)        = X3*RF^3/100000000000 + X2*RF^2/100000000 + X1*RF/100000 + X0/100   */
        /*   gain_TEQ (dB * 1000) = X3*RF^3/100000000 + X2*RF^2/100000 + X1*RF/100 + X0*10   */
        if (err == TM_OK)
        {    
            GainX3 = (Int32)((Int32)(uRF * uRF * uRF)/(Int32)10000);
            GainX3 = (Int32)(Lvl_TEQ_ES2[uIndex][TDA18204_Coeff_X3] * GainX3 );
            GainX2 = (Int32)((uRF * uRF )/10);
            GainX2 = (Int32)(Lvl_TEQ_ES2[uIndex][TDA18204_Coeff_X2] * GainX2);
            GainX1 = (Int32)((Int32)(Lvl_TEQ_ES2[uIndex][TDA18204_Coeff_X1] * uRF) * (Int32)100 );
            GainX0 = (Int32)( Lvl_TEQ_ES2[uIndex][TDA18204_Coeff_X0] * 100000 );
            Gain =   (Int32)( GainX3 + GainX2 + GainX1 + GainX0 );
            Gain =   Gain / (Int32)10000;
        }
    }
    else
    {
        if (err == TM_OK)
        {
            while ( (uTEQSelected > Lvl_TEQ_ES1[uIndex][TDA18204_Coeff_X_Code]) && (uIndex < TDA18204_INSTANCE_CUSTOM_COMMON_LVL_TEQ_MAX) )
                uIndex = uIndex + 1;

            if (uIndex == TDA18204_INSTANCE_CUSTOM_COMMON_LVL_TEQ_MAX)
            {
                uIndex = uIndex - 1;
            }
        }

        /*   gain_TEQ (dB)        = X3*RF^3/100000000000 + X2*RF^2/100000000 + X1*RF/100000 + X0/100   */
        /*   gain_TEQ (dB * 1000) = X3*RF^3/100000000 + X2*RF^2/100000 + X1*RF/100 + X0*10   */
        if (err == TM_OK)
        {    
            GainX3 = (Int32)((Int32)(uRF * uRF * uRF)/(Int32)10000);
            GainX3 = (Int32)(Lvl_TEQ_ES1[uIndex][TDA18204_Coeff_X3] * GainX3 );
            GainX2 = (Int32)((uRF * uRF )/10);
            GainX2 = (Int32)(Lvl_TEQ_ES1[uIndex][TDA18204_Coeff_X2] * GainX2);
            GainX1 = (Int32)((Int32)(Lvl_TEQ_ES1[uIndex][TDA18204_Coeff_X1] * uRF) * (Int32)100 );
            GainX0 = (Int32)( Lvl_TEQ_ES1[uIndex][TDA18204_Coeff_X0] * 100000 );
            Gain =   (Int32)( GainX3 + GainX2 + GainX1 + GainX0 );
            Gain =   Gain / (Int32)10000;
        }
    }

	*pGain = (Int32)Gain;

	return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18204_GetRefMaxGainNB                                     */
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
iTDA18204_GetRefMaxGainNB(
    pTDA18204Object_t   pObj,        /* I: Driver object */
    Int32*              pMaxGain     /* O: Max gain value in dB (x1000) */
)
{
    tmErrorCode_t   err = TM_OK;
    UInt32          uIndex = 0;
    Int32           MaxGain = 0;
    UInt32          uRF = 0; /* RF in MHz */


    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18204_GetRefMaxGainNB(0x%08X)", pObj->tUnitW);

    uRF = ( pObj->uRF / 1000000 );

    /* RSSI coefficients initialization, in function of HW version */
    /* ES2 specific */
    if ( pObj->Revision == 0x20)
    {
        while ( (uIndex < TDA18204_INSTANCE_CUSTOM_COMMON_LVL_REF_GAIN_MAX) && ((Int32)pObj->uRF > Lvl_MAX_GAIN_NB_ES2[uIndex][TDA18204_Coeff_Y_Code]) )
            uIndex = uIndex + 1;

        if (uIndex == TDA18204_INSTANCE_CUSTOM_COMMON_LVL_REF_GAIN_MAX)
        {
            uIndex = uIndex - 1;
        }

        /*   MaxGain (dB)        = X2*RF^2/10000000 + X1*RF/1000000 + X0/1000000   */
        /*   delta gain_AGC1_NB (dB * 1000) = ( X2*RF^2/10 + X1*RF + X0 )/1000   */
        /* X0 : Lvl_MAX_GAIN_NB_ES2[uIndex][TDA18204_Coeff_X0] */
        /* X1 : Lvl_MAX_GAIN_NB_ES2[uIndex][TDA18204_Coeff_X1] */
        /* X2 : Lvl_MAX_GAIN_NB_ES2[uIndex][TDA18204_Coeff_X2] */
        MaxGain = (Int32)(Lvl_MAX_GAIN_NB_ES2[uIndex][TDA18204_Coeff_X0] + (Int32)(Lvl_MAX_GAIN_NB_ES2[uIndex][TDA18204_Coeff_X1] * uRF ) + (Int32)(Lvl_MAX_GAIN_NB_ES2[uIndex][TDA18204_Coeff_X2] * uRF * uRF)/(Int32)10 );
        MaxGain = MaxGain / (Int32)1000;
    }
    else
    {
        while ( (uIndex < TDA18204_INSTANCE_CUSTOM_COMMON_LVL_REF_GAIN_MAX) && ((Int32)pObj->uRF > Lvl_MAX_GAIN_NB_ES1[uIndex][TDA18204_Coeff_Y_Code]) )
            uIndex = uIndex + 1;

        if (uIndex == TDA18204_INSTANCE_CUSTOM_COMMON_LVL_REF_GAIN_MAX)
        {
            uIndex = uIndex - 1;
        }

        /*   MaxGain (dB)        = X2*RF^2/10000000 + X1*RF/1000000 + X0/1000000   */
        /*   delta gain_AGC1_NB (dB * 1000) = ( X2*RF^2/10 + X1*RF + X0 )/1000   */
        /* X0 : Lvl_MAX_GAIN_NB_ES1[uIndex][TDA18204_Coeff_X0] */
        /* X1 : Lvl_MAX_GAIN_NB_ES1[uIndex][TDA18204_Coeff_X1] */
        /* X2 : Lvl_MAX_GAIN_NB_ES1[uIndex][TDA18204_Coeff_X2] */
        MaxGain = (Int32)(Lvl_MAX_GAIN_NB_ES1[uIndex][TDA18204_Coeff_X0] + (Int32)(Lvl_MAX_GAIN_NB_ES1[uIndex][TDA18204_Coeff_X1] * uRF ) + (Int32)(Lvl_MAX_GAIN_NB_ES1[uIndex][TDA18204_Coeff_X2] * uRF * uRF)/(Int32)10 );
        MaxGain = MaxGain / (Int32)1000;
    }

    *pMaxGain = (Int32)MaxGain;

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18204_LvlAgc1NBContribution                               */
/*                                                                            */
/* DESCRIPTION: AGC1 NB Contribution for iTDA18204_GetPowerLevel()            */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18204_LvlAgc1NBContribution(
    pTDA18204Object_t   pObj,        /* I: Driver object */
    Int32*              pDeltaGain   /* O: Delta AGC1 Gain value in dB (x1000) */
)
{
    tmErrorCode_t   err = TM_OK;
    UInt32          uIndex = 0;
    UInt8           uAGC1NBGainRead = 0;
    UInt8           uAGC1GainForce = 0;
    Int32           DeltaGain = 0;
    UInt32          uRF = 0; /* RF in MHz */

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18204_LvlAgc1NBContribution(0x%08X)", pObj->tUnitW);

    uRF = ( pObj->uRF / 1000000 );

    /* retrieve AGC1_NB_GAIN_READ */
    err = iTDA18204_Read(pObj, &gTDA18204_Reg_AGC1_byte_3__Force_AGC1_gain, &uAGC1GainForce, Bus_NoWrite);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", pObj->tUnitW));

    if (err == TM_OK)
    {
        /* if AGC1 Gain is forced, the real value should be read in AGC1_Gain */
        if ( uAGC1GainForce == 0x0 )
        {
            err = iTDA18204_Read(pObj, &gTDA18204_Reg_AGC1_LT_Gain_status__AGC1_gain_read, &uAGC1NBGainRead, Bus_NoWrite);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", pObj->tUnitW));
        }
        else
        {
            err = iTDA18204_Read(pObj, &gTDA18204_Reg_AGC1_byte_3__AGC1_Gain, &uAGC1NBGainRead, Bus_NoWrite);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", pObj->tUnitW));
        }
    }

    /* RSSI coefficients initialization, in function of HW version */
    /* ES2 specific */
    if ( pObj->Revision == 0x20)
    {
        if (err == TM_OK)
        {
            while ( (uAGC1NBGainRead > Lvl_AGC1NB_ES2[uIndex][TDA18204_Coeff_Y_Code]) && (uIndex < TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC1_MAX) )
                uIndex = uIndex + 1;

            if (uIndex == TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC1_MAX)
            {
                uIndex = uIndex - 1;
            }
        }

        /*   delta gain_AGC1_NB (dB)        = X2*RF^2/100000000 + X1*RF/10000000 + X0/10000000   */
        /*   delta gain_AGC1_NB (dB * 1000) = ( X2*RF^2/10 + X1*RF + X0 )/10000   */
        /* X0 : Lvl_AGC1NB_ES2[uIndex][TDA18204_Coeff_X0] */
        /* X1 : Lvl_AGC1NB_ES2[uIndex][TDA18204_Coeff_X1] */
        /* X2 : Lvl_AGC1NB_ES2[uIndex][TDA18204_Coeff_X2] */
        DeltaGain = (Int32)(Lvl_AGC1NB_ES2[uIndex][TDA18204_Coeff_X0] + (Int32)(Lvl_AGC1NB_ES2[uIndex][TDA18204_Coeff_X1] * uRF ) + (Int32)(Lvl_AGC1NB_ES2[uIndex][TDA18204_Coeff_X2] * uRF * uRF)/(Int32)10 );
        DeltaGain = DeltaGain / (Int32)10000;
    }
    else
    {
        if (err == TM_OK)
        {
            while ( (uAGC1NBGainRead > Lvl_AGC1NB_ES1[uIndex][TDA18204_Coeff_Y_Code]) && (uIndex < TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC1_MAX) )
                uIndex = uIndex + 1;

            if (uIndex == TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC1_MAX)
            {
                uIndex = uIndex - 1;
            }
        }

        /*   delta gain_AGC1_NB (dB)        = X2*RF^2/100000000 + X1*RF/10000000 + X0/10000000   */
        /*   delta gain_AGC1_NB (dB * 1000) = ( X2*RF^2/10 + X1*RF + X0 )/10000   */
        /* X0 : Lvl_AGC1NB_ES1[uIndex][TDA18204_Coeff_X0] */
        /* X1 : Lvl_AGC1NB_ES1[uIndex][TDA18204_Coeff_X1] */
        /* X2 : Lvl_AGC1NB_ES1[uIndex][TDA18204_Coeff_X2] */
        DeltaGain = (Int32)(Lvl_AGC1NB_ES1[uIndex][TDA18204_Coeff_X0] + (Int32)(Lvl_AGC1NB_ES1[uIndex][TDA18204_Coeff_X1] * uRF ) + (Int32)(Lvl_AGC1NB_ES1[uIndex][TDA18204_Coeff_X2] * uRF * uRF)/(Int32)10 );
        DeltaGain = DeltaGain / (Int32)10000;
    }

    *pDeltaGain = (Int32)DeltaGain;

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18204_LvlAgc2NBContribution                               */
/*                                                                            */
/* DESCRIPTION: AGC2 NB Contribution for iTDA18204_GetPowerLevel()            */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18204_LvlAgc2NBContribution(
    pTDA18204Object_t   pObj,        /* I: Driver object */
    Int32*              pDeltaGain   /* O: Delta AGC2 Gain value in dB (x1000) */
)
{
    tmErrorCode_t   err = TM_OK;
    UInt32          uIndex = 0;
    UInt8           uAGC2NBGainRead = 0;
    Int32           Gain = 0;
    Int32           GainX2 = 0;
    Int32           GainX1 = 0;
    Int32           GainX0 = 0;
    UInt32          uRF = 0; /* RF in MHz */

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18204_LvlAgc2NBContribution(0x%08X)", pObj->tUnitW);

    uRF = ( pObj->uRF / 1000000 );

    /* retrieve AGC2_NB_GAIN_READ */
    err = iTDA18204_Read(pObj, &gTDA18204_Reg_AGC2_NB_Gain_status__AGC2_NB_GAIN_READ, &uAGC2NBGainRead, Bus_NoWrite);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", pObj->tUnitW));

    /* RSSI coefficients initialization, in function of HW version */
    /* ES2 specific */
    if ( pObj->Revision == 0x20)
    {
        if (err == TM_OK)
        {
            while ( (uAGC2NBGainRead > Lvl_AGC2NB_ES2[uIndex][TDA18204_Coeff_Z_Code]) && (uIndex < TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC2_MAX) )
                uIndex = uIndex + 1;

            if (uIndex == TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC2_MAX)
            {
                uIndex = uIndex - 1;
            }

            /*   gain_AGC2 (dB * 1000) = (X2(polynom2)*RF^2 + X1(polynom2)*RF + X0(polynom2))/10000 + X0(polynom2)*uAGC2NBGainRead  */
            if(uRF <= 600)
            {
                GainX2 = (Int32)(Lvl_AGC2NB_ES2[uIndex][TDA18204_Coeff_Z3] * uRF * uRF );
                GainX1 = (Int32)(Lvl_AGC2NB_ES2[uIndex][TDA18204_Coeff_Z2] * uRF);
                GainX0 = (Int32) Lvl_AGC2NB_ES2[uIndex][TDA18204_Coeff_Z1];
            }
            else
            {
                GainX2 = (Int32)(Lvl_AGC2NB_ES2[uIndex][TDA18204_Coeff_Z6] * uRF * uRF );
                GainX1 = (Int32)(Lvl_AGC2NB_ES2[uIndex][TDA18204_Coeff_Z5] * uRF);
                GainX0 = (Int32) Lvl_AGC2NB_ES2[uIndex][TDA18204_Coeff_Z4];
            }

            Gain =   (Int32)( GainX2 + GainX1 + GainX0 + (Lvl_AGC2NB_ES2[uIndex][TDA18204_Coeff_Z0]*uAGC2NBGainRead) );
            Gain =   Gain / (Int32)10000;
        }
    }
    else
    {
        if (err == TM_OK)
        {
            while ( (uAGC2NBGainRead > Lvl_AGC2NB_ES1[uIndex][TDA18204_Coeff_Y_Code]) && (uIndex < TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC2_MAX) )
                uIndex = uIndex + 1;

            if (uIndex == TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC2_MAX)
            {
                uIndex = uIndex - 1;
            }
        }

        /*   delta gain_AGC2_NB (dB)        = X2*RF/100000 + X1*uAGC2NBGainRead/10000 + X0/10000   */
        /*   delta gain_AGC2_NB (dB * 1000) = (X2*RF/10 + X1*uAGC2NBGainRead + X0) / 10   */
        /* X0 : Lvl_AGC2NB_ES1[uIndex][TDA18204_Coeff_X0] */
        /* X1 : Lvl_AGC2NB_ES1[uIndex][TDA18204_Coeff_X1] */
        /* X2 : Lvl_AGC2NB_ES1[uIndex][TDA18204_Coeff_X2] */
        Gain = (Int32)(Lvl_AGC2NB_ES1[uIndex][TDA18204_Coeff_X0] + (Int32)(Lvl_AGC2NB_ES1[uIndex][TDA18204_Coeff_X1] * uAGC2NBGainRead ) + (Int32)(Lvl_AGC2NB_ES1[uIndex][TDA18204_Coeff_X2] * uRF )/(Int32)10 );
        Gain = Gain / (Int32)10;
    }

    *pDeltaGain = (Int32)Gain;

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18204_LvlAgc3NBContribution                               */
/*                                                                            */
/* DESCRIPTION: AGC3 NB Contribution for iTDA18204_GetPowerLevel()            */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18204_LvlAgc3NBContribution(
    pTDA18204Object_t   pObj,        /* I: Driver object */
    Int32*              pDeltaGain   /* O: Delta AGC3 Gain value in dB (x1000) */
)
{
    tmErrorCode_t   err = TM_OK;
    UInt32          uIndex = 0;
    UInt8           uAGC3NBGainRead = 0;
    Int32           DeltaGain = 0;
    UInt32          uRF = 0; /* RF in MHz */

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18204_LvlAgc3NBContribution(0x%08X)", pObj->tUnitW);

    uRF = ( pObj->uRF / 1000000 );

    /* retrieve AGC3_NB_GAIN_READ */
    err = iTDA18204_Read(pObj, &gTDA18204_Reg_AGC3_NB_gain_status__AGC3_NB_GAIN_READ, &uAGC3NBGainRead, Bus_NoWrite);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", pObj->tUnitW));

    /* RSSI coefficients initialization, in function of HW version */
    /* ES2 specific */
    if ( pObj->Revision == 0x20)
    {
        if (err == TM_OK)
        {
            while ( (uAGC3NBGainRead > Lvl_AGC3NB_ES2[uIndex][TDA18204_Coeff_Y_Code]) && (uIndex < TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC3_MAX) )
                uIndex = uIndex + 1;

            if (uIndex == TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC3_MAX)
            {
                uIndex = uIndex - 1;
            }
        }

        /*   delta gain_AGC3_NB (dB)        = X2*RF^2/100000000 + X1*RF/10000000 + X0/10000000   */
        /*   delta gain_AGC3_NB (dB * 1000) = ( X2*RF^2/10 + X1*RF + X0 )/10000   */
        /* X0 : Lvl_AGC3NB_ES2[uIndex][TDA18204_Coeff_X0] */
        /* X1 : Lvl_AGC3NB_ES2[uIndex][TDA18204_Coeff_X1] */
        /* X2 : Lvl_AGC3NB_ES2[uIndex][TDA18204_Coeff_X2] */
        DeltaGain = (Int32)(Lvl_AGC3NB_ES2[uIndex][TDA18204_Coeff_X0] + (Int32)(Lvl_AGC3NB_ES2[uIndex][TDA18204_Coeff_X1] * uRF ) + (Int32)(Lvl_AGC3NB_ES2[uIndex][TDA18204_Coeff_X2] * uRF * uRF)/(Int32)10 );
        DeltaGain = DeltaGain / (Int32)10000;
    }
    else
    {
        if (err == TM_OK)
        {
            while ( (uAGC3NBGainRead > Lvl_AGC3NB_ES1[uIndex][TDA18204_Coeff_Y_Code]) && (uIndex < TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC3_MAX) )
                uIndex = uIndex + 1;

            if (uIndex == TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC3_MAX)
            {
                uIndex = uIndex - 1;
            }
        }

        /*   delta gain_AGC3_NB (dB)        = X2*RF^2/100000000 + X1*RF/10000000 + X0/10000000   */
        /*   delta gain_AGC3_NB (dB * 1000) = ( X2*RF^2/10 + X1*RF + X0 )/10000   */
        /* X0 : Lvl_AGC3NB_ES1[uIndex][TDA18204_Coeff_X0] */
        /* X1 : Lvl_AGC3NB_ES1[uIndex][TDA18204_Coeff_X1] */
        /* X2 : Lvl_AGC3NB_ES1[uIndex][TDA18204_Coeff_X2] */
        DeltaGain = (Int32)(Lvl_AGC3NB_ES1[uIndex][TDA18204_Coeff_X0] + (Int32)(Lvl_AGC3NB_ES1[uIndex][TDA18204_Coeff_X1] * uRF ) + (Int32)(Lvl_AGC3NB_ES1[uIndex][TDA18204_Coeff_X2] * uRF * uRF)/(Int32)10 );
        DeltaGain = DeltaGain / (Int32)10000;
    }

    *pDeltaGain = (Int32)DeltaGain;

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18204_SetTiltEqualizer                                    */
/*                                                                            */
/* DESCRIPTION: Set the Tilt Equalizer selection.                             */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18204_SetTiltEqualizer(
    pTDA18204Object_t       pObj,        /* I: Driver object */
    TDA18204TEQSelect_t     teqSelect    /* I: Tilt Equalizer selection */
)
{
	tmErrorCode_t   err = TM_OK;

	tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18204_SetTiltEqualizer(0x%08X)", pObj->tUnitW);

	/* write TEQ_SELECT */
	err = iTDA18204_Write(pObj, &gTDA18204_Reg_TEQ_SELECT__TEQ_SELECT,  teqSelect, Bus_NoRead);
	tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));

	/* write TEQ_Launch */
	if (err == TM_OK)
	{    
		err = iTDA18204_Write(pObj, &gTDA18204_Reg_MSM_byte_2__TEQ_Launch,  0x1, Bus_NoRead);
		tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
	}

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18204_SetRFFilters                                        */
/*                                                                            */
/* DESCRIPTION: Set the RF filters                                            */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18204_SetRFFilters(
    pTDA18204Object_t       pObj          /* I: Driver object */
)
{
    tmErrorCode_t   err = TM_OK;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18204_SetRFFilters(0x%08X)", pObj->tUnitW);

    /* selects the IR_FreqPtr of IRcal to use for setting RF */
    /* ES1, ES1MF and ES2 : we take low value in any case */
    err = iTDA18204_Write(pObj, &gTDA18204_Reg_IR_Cal_byte_2__IR_FreqPtr, TDA18204_IR_FreqPtr_LOW_VALUE_1, Bus_NoRead);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));

    /* PR#460 : TOP AGC2_NB versus CSO/CTB */
    /* by default reinitialize BP filter to handle AGC2 TOP transition when filter 6 was selected before (ex 300 MHz) */
    /* and we want RF (ex 500 MHz) */
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_RF_Band_Pass_Filter_byte__RF_BPF, TDA18204_RF_BPF_BP_FILTER1_44MINUS73MHZ_VALUE_31, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /* Set H3H5 filter (RF band pass filter) */
    /*  RF in 40-62 MHz   : BP_Filter1 (44-73MHz)    */
    /*  RF in 62-87 MHz   : BP_Filter2 (62-103MHz)   */
    /*  RF in 87-123 MHz  : BP_Filter3 (87-145MHz)   */
    /*  RF in 123-173 MHz : BP_Filter4 (123-204MHz)  */
    /*                                              */
    /*		ES1 : RF in 173-244 MHz : BP_Filter5 (173-288MHz)  */
    /*		ES1 : RF > 244 MHz      : no BP Filter             */
    /*                                                        */
    /*		ES2 : RF in 173-244 MHz : BP_Filter5 (173-288MHz)  */
    /*		ES2 : RF in 244-320 MHz : BP_Filter6 (244-365MHz)  */
    /*		ES2 : RF > 320 MHz      : no BP Filter             */    

    if ( pObj->uRF <= TDA18204_BP_FILTER_1 )
    {
        if(err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_H3H5, 0x1, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_H3H5_byte__SWITCH_STAB_CAP, TDA18204_SWITCH_STAB_CAP_0_VALUE_0, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
    }
    else if ( pObj->uRF <= TDA18204_BP_FILTER_2 )
    {
        if(err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_RF_Band_Pass_Filter_byte__RF_BPF, TDA18204_RF_BPF_BP_FILTER1_44MINUS73MHZ_VALUE_31, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_H3H5, 0x0, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_H3H5_byte__SWITCH_STAB_CAP, TDA18204_SWITCH_STAB_CAP_1740_VALUE_31, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
    }
    else if ( pObj->uRF <= TDA18204_BP_FILTER_3 )
    {
        if(err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_RF_Band_Pass_Filter_byte__RF_BPF, TDA18204_RF_BPF_BP_FILTER2_62MINUS103MHZ_VALUE_30, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_H3H5, 0x0, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_H3H5_byte__SWITCH_STAB_CAP, TDA18204_SWITCH_STAB_CAP_380_VALUE_2, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
    }
    else if ( pObj->uRF <= TDA18204_BP_FILTER_4 )
    {
        if(err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_RF_Band_Pass_Filter_byte__RF_BPF, TDA18204_RF_BPF_BP_FILTER3_87MINUS145MHZ_VALUE_28, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_H3H5, 0x0, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_H3H5_byte__SWITCH_STAB_CAP, TDA18204_SWITCH_STAB_CAP_830_VALUE_28, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
    }
    else if ( pObj->uRF <= TDA18204_BP_FILTER_5 )
    {
        if(err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_RF_Band_Pass_Filter_byte__RF_BPF, TDA18204_RF_BPF_BP_FILTER4_123MINUS204MHZ_VALUE_24, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_H3H5, 0x0, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_H3H5_byte__SWITCH_STAB_CAP, TDA18204_SWITCH_STAB_CAP_330_VALUE_24, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
    }
    else
    {
        if ( pObj->uRF <= TDA18204_BP_FILTER_6 )
        {
            if(err == TM_OK)
            {
                err = iTDA18204_Write(pObj, &gTDA18204_Reg_RF_Band_Pass_Filter_byte__RF_BPF, TDA18204_RF_BPF_BP_FILTER5_173MINUS288MHZ_VALUE_16, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
            }
            if(err == TM_OK)
            {
                err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_H3H5, 0x0, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
            }
            if(err == TM_OK)
            {
                err = iTDA18204_Write(pObj, &gTDA18204_Reg_H3H5_byte__SWITCH_STAB_CAP, TDA18204_SWITCH_STAB_CAP_0_VALUE_0, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
            }
        }
        else
        {
            if(err == TM_OK)
            {
                err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_2__PD_H3H5, 0x1, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
            }
            if(err == TM_OK)
            {
                err = iTDA18204_Write(pObj, &gTDA18204_Reg_H3H5_byte__SWITCH_STAB_CAP, TDA18204_SWITCH_STAB_CAP_0_VALUE_0, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
            }
        }
    }

    /************************************/
    /***  set AGC2_NB_TOP UP and DOWN  **/
    /************************************/
    if( err == TM_OK )
    {
        err = iTDA18204_SetAGC2NBTOP(pObj);
    }

    /****************************************************/
    /*  AGC2 NB - HPF                                   */
    /****************************************************/
    if(err == TM_OK)
    {
        if ( pObj->uRF <= 600000000 )
        {
            if ( pObj->bAgc2Active == True )
            {
                /* enable AGC2_NB_HP_EN */
                err = iTDA18204_SetAgc2NB(pObj, False);
                pObj->bAgc2Active = False;
            }
        }
        else 
        {
            if ( pObj->bAgc2Active == False )
            {
                /* disable AGC2_NB_HP_EN */
                err = iTDA18204_SetAgc2NB(pObj, True);
                pObj->bAgc2Active = True;
            }
        }
    }

    /****************************************************/
    /*  WIFI                                            */
    /****************************************************/
    if ( ((pObj->uRF > 335360000) && (pObj->uRF <= 390656000)) || 
        ((pObj->uRF > 562688000) && (pObj->uRF <= 654848000)) )
    {
        if(err == TM_OK)
        {
            /* Select wireless filter frequency */
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_W_Filter_byte__W_Filter, TDA18204_W_Filter_WIRELESS_FILTER_NOTCH_1_7_MINUS_1_98GHZ_VALUE_3, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* Select wireless filter offset frequency */
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_W_Filter_byte__W_Filter_Offset, TDA18204_W_Filter_Offset_NOMINAL_VALUE_1, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* Enable wireless filter */
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_W_Filter_byte__W_Filter_Enable, 0x1, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
    }
    else if ( ((pObj->uRF > 390656000) && (pObj->uRF <= 430592000)) || 
        ((pObj->uRF > 654848000) && (pObj->uRF <= 720384000)) )
    {
        if(err == TM_OK)
        {
            /* Select wireless filter frequency */
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_W_Filter_byte__W_Filter, TDA18204_W_Filter_WIRELESS_FILTER_NOTCH_1_98_MINUS_2_17GHZ_VALUE_2, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* Select wireless filter offset frequency */
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_W_Filter_byte__W_Filter_Offset, TDA18204_W_Filter_Offset_MINUS4_VALUE_2, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* Enable wireless filter */
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_W_Filter_byte__W_Filter_Enable, 0x1, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
    }
    else if ( ((pObj->uRF > 473600000) && (pObj->uRF <= 535040000)) || 
        ((pObj->uRF > 793088000) && (pObj->uRF <= 865792000)) )
    {
        if(err == TM_OK)
        {
            /* Select wireless filter frequency */
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_W_Filter_byte__W_Filter, TDA18204_W_Filter_WIRELESS_FILTER_NOTCH_2_4_MINUS_2_6GHZ_VALUE_1, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* Select wireless filter offset frequency */
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_W_Filter_byte__W_Filter_Offset, TDA18204_W_Filter_Offset_NOMINAL_VALUE_1, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* Enable wireless filter */
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_W_Filter_byte__W_Filter_Enable, 0x1, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
    }
    else if ((pObj->uRF > 289280000) && (pObj->uRF <= 289280000))
    {
        if(err == TM_OK)
        {
            /* Select wireless filter frequency */
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_W_Filter_byte__W_Filter, TDA18204_W_Filter_WIRELESS_FILTER_NOTCH_2_4_MINUS_2_6GHZ_VALUE_1, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* Select wireless filter offset frequency */
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_W_Filter_byte__W_Filter_Offset, TDA18204_W_Filter_Offset_4_VALUE_0, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* Enable wireless filter */
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_W_Filter_byte__W_Filter_Enable, 0x1, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
    }
    else
    {
        if(err == TM_OK)
        {
            /* Select wireless filter frequency */
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_W_Filter_byte__W_Filter, TDA18204_W_Filter_WIRELESS_FILTER_NOTCH_2_4_MINUS_2_6GHZ_VALUE_1, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* Select wireless filter offset frequency */
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_W_Filter_byte__W_Filter_Offset, TDA18204_W_Filter_Offset_4_VALUE_0, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            /* Enable wireless filter */
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_W_Filter_byte__W_Filter_Enable, 0x0, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
    }

    /****************************************************/
    /*  S2D                                             */
    /****************************************************/
    if(err == TM_OK)
    {
        if (pObj->uRF > TDA18204_BP_FILTER_4)
        {
            /* S2D gain */
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_IR_Mixer_byte_1__S2D_Gain, TDA18204_S2D_Gain_6_VALUE_1, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
        else 
        {
            /* S2D gain */
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_IR_Mixer_byte_1__S2D_Gain, TDA18204_S2D_Gain_9_VALUE_2, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18204_SetLO                                              */
/*                                                                            */
/* DESCRIPTION: Set the PLL                                                   */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18204_SetLO(
    pTDA18204Object_t   pObj    /* I: Driver object */
)
{
    tmErrorCode_t err = TM_OK;
    /* LO in KHz */
    UInt32 LO = 0;
    /* VCO in KHz */
    UInt32 VCO = 0;

    UInt8 NLO = 0;
    UInt8 PowerDown5 = 0; /* register value */
    UInt8 MuxDiv2d = 0; /* register value */
    Bool  Xtal_div_1p5 = False; /* is Xtal divide by 1.5 ? */
    Bool  VCOHighUsed = False; 	/* is VCO high used or not */
    UInt8 N_int = 0;
    UInt32 K_int = 0;
    UInt8  uLO_Lock = 0;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18204_SetLO(0x%08X)", pObj->tUnitW);

    /* Calculate wanted LO = RF + IF (in KHz) */
    LO = (pObj->uRF + pObj->pStandard->uIF)/1000;

    /* FVCO_max calibration if not done */
    if (pObj->VCOmax == 0)
    {
        err = iTDA18204_FVCOMaxCalibration(pObj);
    }

    if (err == TM_OK)
    {
        /* Calculate the NLO (PostDiv) */
        err = iTDA18204_CalculateNLO(pObj, LO, &NLO);
    }

    /* Calculate the VCO target */
    VCO = LO * NLO; /* in KHz */

    /* is Xtal divide by 1.5 ? */
    if (err == TM_OK)
    {
        err = iTDA18204_FindXtalDiv1p5(VCO, &Xtal_div_1p5);
    }

    /* Calculate N & K values of the PLL */
    if (err == TM_OK)
    {
        err = iTDA18204_CalculateNIntKInt(VCO, Xtal_div_1p5, &N_int, &K_int);
    }

    /****************************************************/
    /*  NLO register settings                           */
    /****************************************************/
    if (err == TM_OK)
    {
        switch(NLO)
        {
        case 4:
            PowerDown5 = 0x2F;
            MuxDiv2d = TDA18204_lochain_mux_div2d_DIVIDE_BY_2_OR_3_MINUS_CALIBRATION_880MHZ_VALUE_54;
            break;
        case 6:
            PowerDown5 = 0x1F;
            MuxDiv2d = TDA18204_lochain_mux_div2d_DIVIDE_BY_2_OR_3_MINUS_CALIBRATION_880MHZ_VALUE_54;
            break;
        case 8:
            PowerDown5 = 0x27;
            MuxDiv2d = TDA18204_lochain_mux_div2d_DIVIDE_BY_4_OR_6_VALUE_53;
            break;
        case 12:
            PowerDown5 = 0x17;
            MuxDiv2d = TDA18204_lochain_mux_div2d_DIVIDE_BY_4_OR_6_VALUE_53;
            break;
        case 16:
            PowerDown5 = 0x23;
            MuxDiv2d = TDA18204_lochain_mux_div2d_DIVIDE_BY_8_OR_12_VALUE_51;
            break;
        case 24:
            PowerDown5 = 0x13;
            MuxDiv2d = TDA18204_lochain_mux_div2d_DIVIDE_BY_8_OR_12_VALUE_51;
            break;
        case 32:
            PowerDown5 = 0x21;
            MuxDiv2d = TDA18204_lochain_mux_div2d_DIVIDE_BY_16_OR_24_MINUS_CALIBRATION_110MHZ_VALUE_43;
            break;
        case 48:
            PowerDown5 = 0x11;
            MuxDiv2d = TDA18204_lochain_mux_div2d_DIVIDE_BY_16_OR_24_MINUS_CALIBRATION_110MHZ_VALUE_43;
            break;
        case 64:
            PowerDown5 = 0x20;
            MuxDiv2d = TDA18204_lochain_mux_div2d_DIVIDE_BY_32_OR_48_VALUE_27;
            break;
        case 96:
            PowerDown5 = 0x10;
            MuxDiv2d = TDA18204_lochain_mux_div2d_DIVIDE_BY_32_OR_48_VALUE_27;
            break;
        default:
            err = TDA18204_ERR_BAD_PARAMETER;
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_SetLO(0x%08X) *PostDiv value is wrong.", pObj->tUnitW));
            break;
        }
    }

    /* reset LO_Lock */
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_PLL_byte__PLL_lock_reset, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }

    /* bias du VCO On + LO Chain : power_down_5.pd_lochain  = 0x0 */
    /* mode normal (pas IRCAL Mode) : power_down_5.pd_lochain_calmode  = 0x1 */
    PowerDown5 = PowerDown5 | 0x40;

    /************************/
    /* LO chain programming */
    /************************/
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_5, PowerDown5, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_LO_CHAIN_MUX_DIV2D, MuxDiv2d, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }

    /* N' */
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_Sigma_delta_byte_1__LO_Int, (UInt8)(N_int & 0x7F), Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    } 

    /* K' */
    if(err == TM_OK)
    {
        /* force LO_Frac_0 to 0x00 */
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_Sigma_delta_byte_4__LO_Frac_0, 0x00, Bus_NoRead);
        /* err = iTDA18204_Write(pObj, &gTDA18204_Reg_Sigma_delta_byte_4__LO_Frac_0, (UInt8)(K_int & 0xFF), Bus_NoRead); */
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        /* Mask LO_Frac_1 with 0xF0 */
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_Sigma_delta_byte_3__LO_Frac_1, (UInt8)((K_int >> 8) & 0xF0), Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_Sigma_delta_byte_2__LO_Frac_2, (UInt8)((K_int >> 16) & 0x7F), Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }

    /******************************/
    /* PLL + VCO programming      */
    /******************************/
    /* Xtal divide by 1.5 */
    if(err == TM_OK)
    {
        if (Xtal_div_1p5 == False)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_6__pd_xtal_div1p5, 0x1, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
        else
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_DOWN_6__pd_xtal_div1p5, 0x0, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, 3);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }

    /* Determine which VCO to use and program it */
    if(err == TM_OK)
    {
        err = iTDA18204_SelectVCO(pObj, VCO, &VCOHighUsed);
    }

    /* Charge pump */
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_Charge_pump_byte__ICP_Force_Src, 0x1, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        /* wait 1 ms */
        err = iTDA18204_Wait(pObj, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_Charge_pump_byte__ICP_Force_Src, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /* reset LO_Lock */
    if(err == TM_OK)
    {
        err = iTDA18204_Write(pObj, &gTDA18204_Reg_PLL_byte__PLL_lock_reset, 0x0, Bus_NoRead);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    }

    /* wait for LO locked */ 
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, 10);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Read(pObj, &gTDA18204_Reg_POWER_STATUS__LO_Lock, &uLO_Lock, Bus_RW);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK) 
    {
        if (uLO_Lock != 1) 
        {
            /* wait and re-read LO_Lock */
            err = iTDA18204_Wait(pObj, 10);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
            if(err == TM_OK)
            {
                err = iTDA18204_Read(pObj, &gTDA18204_Reg_POWER_STATUS__LO_Lock, &uLO_Lock, Bus_RW);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Read(0x%08X) failed.", pObj->tUnitW));
            }
            if ((err != TM_OK) || (uLO_Lock != 1) )
            {
                // not sure to return an error if not lock TBD 
                //err = TDA18204_ERR_UNLOCK;
                err = TM_OK;
            }
        }
    }

    /*********************/
    /* program sub LO    */
    /*********************/
    if(err == TM_OK)
    {
        err = iTDA18204_SetSubLO(pObj, LO);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_SetSubLO(0x%08X) failed.", pObj->tUnitW));
    }
    if(err == TM_OK)
    {
        err = iTDA18204_Wait(pObj, 5);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Wait(0x%08X) failed.", pObj->tUnitW));
    }

    ///* reset LO_Lock */
    //if(err == TM_OK)
    //{
    //	err = iTDA18204_Write(pObj, &gTDA18204_Reg_PLL_byte__PLL_lock_reset, 0x0, Bus_NoRead);
    //	tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
    //}

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18204_SetBoostH3H5                                        */
/*                                                                            */
/* DESCRIPTION: Set the PSM(boost in case of ES2) for H3H5                    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18204_SetBoostH3H5(
    pTDA18204Object_t       pObj,        /* I: Driver object */
    TDA18204BoostH3H5_t     boostH3H5    /* I: PSM(boost in case of ES2) */
)
{
    tmErrorCode_t   err = TM_OK;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18204_SetTiltEqualizer(0x%08X)", pObj->tUnitW);

    /* ES2 specific */
    if ( pObj->Revision == 0x20)
    {
        switch(boostH3H5)
        {
        case TDA18204BoostH3H5_Psm0:
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_SAVING_MODE_5__PSM_H35, TDA18204_PSM_H35_21_3_VALUE_0, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
            if(err == TM_OK)
            {
                err = iTDA18204_Write(pObj, &gTDA18204_Reg_H3H5_byte__PSM_H35_EXTRA_BIT, 0x0, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
            }
            break;

        case TDA18204BoostH3H5_Psm1:
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_SAVING_MODE_5__PSM_H35, TDA18204_PSM_H35_19_4_VALUE_1, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));    
            if(err == TM_OK)
            {
                err = iTDA18204_Write(pObj, &gTDA18204_Reg_H3H5_byte__PSM_H35_EXTRA_BIT, 0x0, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
            }
            break;

        case TDA18204BoostH3H5_Psm2:
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_SAVING_MODE_5__PSM_H35, TDA18204_PSM_H35_17_4_VALUE_2, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));                      
            if(err == TM_OK)
            {
                err = iTDA18204_Write(pObj, &gTDA18204_Reg_H3H5_byte__PSM_H35_EXTRA_BIT, 0x0, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
            }
            break;

        case TDA18204BoostH3H5_Psm3:

            err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_SAVING_MODE_5__PSM_H35, TDA18204_PSM_H35_21_3_VALUE_0, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
            if(err == TM_OK)
            {
                err = iTDA18204_Write(pObj, &gTDA18204_Reg_H3H5_byte__PSM_H35_EXTRA_BIT, 0x1, Bus_NoRead);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
            }
            break;

        default:
            break;
        }
    }
    /* ES1, ES1MF specific */
    else
    {
        switch(boostH3H5)
        {
        case TDA18204BoostH3H5_Psm0:
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_SAVING_MODE_5__PSM_H35, TDA18204_PSM_H35_21_3_VALUE_0, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
            break;

        case TDA18204BoostH3H5_Psm1:
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_SAVING_MODE_5__PSM_H35, TDA18204_PSM_H35_19_4_VALUE_1, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
            break;

        case TDA18204BoostH3H5_Psm2:
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_SAVING_MODE_5__PSM_H35, TDA18204_PSM_H35_17_4_VALUE_2, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
            break;

        case TDA18204BoostH3H5_Psm3:
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_POWER_SAVING_MODE_5__PSM_H35, TDA18204_PSM_H35_15_4_VALUE_3, Bus_NoRead);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
            break;

        default:
            break;
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18204_SetAGC2NBTOP                                        */
/*                                                                            */
/* DESCRIPTION: Program AGC2 TOP                                              */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18204_SetAGC2NBTOP(
    pTDA18204Object_t       pObj        /* I: Driver object */
)
{
    tmErrorCode_t err = TM_OK;
    UInt8 AGC2_TOP_DN = 0;
    UInt8 AGC2_TOP_UP = 0;
    TDA18204_AGC2NBTOP_t eAGC2NBTop = TDA18204_AGC2NBTOP_LevelInvalid;

    if (( pObj->uRF > TDA18204_BP_FILTER_6 && pObj->uRF <= 600000000 ) || (pObj->uRF <= TDA18204_BP_FILTER_4) || (pObj->uRF > 793088000) )
    {
        eAGC2NBTop = TDA18204_AGC2NBTOP_LevelLow;

        AGC2_TOP_DN = pObj->pStandard->AGC2_TOP_DN_LOW;
        AGC2_TOP_UP = pObj->pStandard->AGC2_TOP_UP_LOW;
    }
    else
    {
        if( (pObj->uRF > 600000000) && (pObj->uRF <= 865792000) )
        {
            eAGC2NBTop = TDA18204_AGC2NBTOP_LevelHigh;

            AGC2_TOP_DN = pObj->pStandard->AGC2_TOP_DN_HIGH;
            AGC2_TOP_UP = pObj->pStandard->AGC2_TOP_UP_HIGH;
        } 
        else
        {
            /* new configuration to be put in the pObj */
            eAGC2NBTop = TDA18204_AGC2NBTOP_LevelMedium;

            AGC2_TOP_DN = pObj->pStandard->AGC2_TOP_DN_MEDIUM;
            AGC2_TOP_UP = pObj->pStandard->AGC2_TOP_UP_MEDIUM;
        }
    }

    /* test if write TOP UP or TOP DOWN first */
    /*
       Transitions are:
        Invalid(init) -> Low     = UP then DO
        Low           -> High    = DO then UP
        Low           -> Medium  = DO then UP
        High          -> Low     = UP then DO
        High          -> Medium  = UP then DO
        Medium        -> Low     = UP then DO
        Medium        -> High    = DO then UP
    */

    if( pObj->eAGC2NBTopCurrent > eAGC2NBTop )
    {
        /* UP first then DOwn */
        if(err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_NB_byte_3__AGC2_NB_TOP_UP, (107 - AGC2_TOP_UP), Bus_RW);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_NB_byte_2__AGC2_NB_TOP_DO, (107 - AGC2_TOP_DN), Bus_RW);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
    }
    else if( pObj->eAGC2NBTopCurrent < eAGC2NBTop )
    {
        /* DOwn first then UP */
        if(err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_NB_byte_2__AGC2_NB_TOP_DO, (107 - AGC2_TOP_DN), Bus_RW);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
        if(err == TM_OK)
        {
            err = iTDA18204_Write(pObj, &gTDA18204_Reg_AGC2_NB_byte_3__AGC2_NB_TOP_UP, (107 - AGC2_TOP_UP), Bus_RW);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "iTDA18204_Write(0x%08X) failed.", pObj->tUnitW));
        }
    }

    /* store current programmation for next one */
    pObj->eAGC2NBTopCurrent = eAGC2NBTop;

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18204_Write                                               */
/*                                                                            */
/* DESCRIPTION: Writes in TDA18204 hardware                                   */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18204_Write(
    pTDA18204Object_t           pObj,       /* I: Driver object */
    const TDA18204_BitField_t*  pBitField, /* I: Bitfield structure */
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

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18204_Write(0x%08X)", pObj->tUnitW);

    if(pBitField == Null)
    {
        err = TDA18204_ERR_BAD_PARAMETER;
    }

    if(err == TM_OK)
    {
        /* Set Register Address */
        RegAddr = pBitField->Address;

        if(RegAddr < TDA18204_REG_MAP_NB_BYTES)
        {
            pRegData = (UInt8 *)(&(pObj->RegMap)) + RegAddr;
        }
        else
        {
            pRegData = &RegData;
        }

        if( (eBusAccess&Bus_NoRead) == False && P_SIO_READ_VALID)
        {
            /* Read data from TDA18204 */
            err = P_SIO_READ(pObj->tUnitW, TDA18204_REG_ADD_SZ, &RegAddr, DataLen, pRegData);
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
                /* Write data to TDA18204 */
                err = P_SIO_WRITE(pObj->tUnitW, TDA18204_REG_ADD_SZ, &RegAddr, DataLen, pRegData);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "IO_Write(0x%08X, 1, 0x%02X, %d) failed.", pObj->tUnitW, RegAddr, DataLen));
            }
        }
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18204_Read                                                */
/*                                                                            */
/* DESCRIPTION: Reads in TDA18204 hardware                                    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18204_Read(
    pTDA18204Object_t           pObj,       /* I: Driver object */
    const TDA18204_BitField_t*  pBitField, /* I: Bitfield structure */
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

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18204_Read(0x%08X)", pObj->tUnitW);

    if(pBitField == Null)
    {
        err = TDA18204_ERR_BAD_PARAMETER;
    }

    if(err == TM_OK)
    {
        /* Set Register Address */
        RegAddr = pBitField->Address;

        if(RegAddr < TDA18204_REG_MAP_NB_BYTES)
        {
            pRegData = (UInt8 *)(&(pObj->RegMap)) + RegAddr;
        }
        else
        {
            pRegData = &RegData;
        }

        if( (eBusAccess&Bus_NoRead) == False && P_SIO_READ_VALID)
        {
            /* Read data from TDA18204 */
            err = P_SIO_READ(pObj->tUnitW, TDA18204_REG_ADD_SZ, &RegAddr, DataLen, pRegData);
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
/* FUNCTION:    iTDA18204_WriteRegMap                                         */
/*                                                                            */
/* DESCRIPTION: Writes driver RegMap cached data to TDA18204 hardware.        */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18204_WriteRegMap(
    pTDA18204Object_t   pObj,       /* I: Driver object */
    UInt8               uAddress,   /* I: Data to write */
    UInt32              uWriteLen   /* I: Number of data to write */
)
{
    tmErrorCode_t   err = TM_OK;
    pUInt8          pRegData = Null;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18204_WriteRegMap(0x%08X)", pObj->tUnitW);

    if( uAddress < TDA18204_REG_MAP_NB_BYTES &&
        (uAddress + uWriteLen) <= TDA18204_REG_MAP_NB_BYTES )
    {
        pRegData = (UInt8 *)(&(pObj->RegMap)) + uAddress;
    }
    else
    {
        err = TDA18204_ERR_BAD_PARAMETER;
    }

    if(err == TM_OK && P_SIO_WRITE_VALID)
    {
        /* Write data to TDA18204 */
        err = P_SIO_WRITE(pObj->tUnitW, TDA18204_REG_ADD_SZ, &uAddress, uWriteLen, pRegData);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "IO_Write(0x%08X, 1, 0x%02X, %d) failed.", pObj->tUnitW, uAddress, uWriteLen));
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18204_ReadRegMap                                          */
/*                                                                            */
/* DESCRIPTION: Reads driver RegMap cached data from TDA18204 hardware.       */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
iTDA18204_ReadRegMap(
    pTDA18204Object_t   pObj,       /* I: Driver object */
    UInt8               uAddress,   /* I: Data to read */
    UInt32              uReadLen    /* I: Number of data to read */
)
{
    tmErrorCode_t   err = TM_OK;
    pUInt8          pRegData = Null;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18204_ReadRegMap(0x%08X)", pObj->tUnitW);

    if( uAddress < TDA18204_REG_MAP_NB_BYTES &&
       (uAddress + uReadLen) <= TDA18204_REG_MAP_NB_BYTES )
    {
        pRegData = (UInt8 *)(&(pObj->RegMap)) + uAddress;
    }
    else
    {
        err = TDA18204_ERR_BAD_PARAMETER;
    }

    if(err == TM_OK && P_SIO_READ_VALID)
    {
        /* Read data from TDA18204 */
        err = P_SIO_READ(pObj->tUnitW, TDA18204_REG_ADD_SZ, &uAddress, uReadLen, pRegData);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "IO_Read(0x%08X, 1, 0x%02X, %d) failed.", pObj->tUnitW, uAddress, uReadLen));
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18204_Wait                                                */
/*                                                                            */
/* DESCRIPTION: Waits for requested time.                                     */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t 
iTDA18204_Wait(
    pTDA18204Object_t   pObj,   /* I: Driver object */
    UInt32              Time    /* I: time to wait for */
)
{
    tmErrorCode_t   err = TDA18204_ERR_NULL_CONTROLFUNC;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18204_Wait(0x%08X)", pObj->tUnitW);

    if(P_STIME_WAIT_VALID)
    {
        /* Wait Time ms */
        err = P_STIME_WAIT(pObj->tUnitW, Time);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TIME_Wait(0x%08X, %d) failed.", pObj->tUnitW, Time));
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18204_AllocInstance:                                      */
/*                                                                            */
/* DESCRIPTION: Allocates an instance.                                        */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
iTDA18204_AllocInstance(
    tmUnitSelect_t      tUnit,      /* I: Unit number */
    ppTDA18204Object_t  ppDrvObject /* I: Driver Object */
)
{
    tmErrorCode_t       err = TDA18204_ERR_ERR_NO_INSTANCES;
    pTDA18204Object_t   pObj = Null;
    UInt32              uLoopCounter = 0;

    /* Find a free instance */
    for (uLoopCounter = 0; uLoopCounter<TDA18204_MAX_UNITS; uLoopCounter++)
    {
        pObj = &gTDA18204Instance[uLoopCounter];
        if (pObj->init == False)
        {
            err = TM_OK;

            pObj->tUnit = tUnit;
            pObj->tUnitW = tUnit;

            err = iTDA18204_ResetInstance(pObj);

            if (pObj->Marker != TDA18204_INSTANCE_CUSTOM_MARKER)
            {
                err = TDA18204_ERR_NOT_INITIALIZED;
            }

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
/* FUNCTION:    iTDA18204_DeAllocInstance:                                    */
/*                                                                            */
/* DESCRIPTION: Deallocates an instance.                                      */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
iTDA18204_DeAllocInstance(
    pTDA18204Object_t  pDrvObject  /* I: Driver Object */
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
/* FUNCTION:    iTDA18204_GetInstance:                                        */
/*                                                                            */
/* DESCRIPTION: Gets an instance.                                             */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
iTDA18204_GetInstance(
    tmUnitSelect_t      tUnit,      /* I: Unit number   */
    ppTDA18204Object_t  ppDrvObject /* I: Driver Object */
)
{
    tmErrorCode_t       err = TDA18204_ERR_ERR_NO_INSTANCES;
    pTDA18204Object_t   pObj = Null;
    UInt32              uLoopCounter = 0;

    /* Get instance */
    for (uLoopCounter = 0; uLoopCounter<TDA18204_MAX_UNITS; uLoopCounter++)
    {
        pObj = &gTDA18204Instance[uLoopCounter];
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
/* FUNCTION:    iTDA18204_ResetInstance:                                      */
/*                                                                            */
/* DESCRIPTION: Resets an instance.                                           */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
iTDA18204_ResetInstance(
    pTDA18204Object_t  pDrvObject  /* I: Driver Object */
)
{
    tmErrorCode_t   err = TM_OK ;

    pDrvObject->curPowerState = TDA18204_INSTANCE_CUSTOM_CURPOWERSTATE_DEF;
    pDrvObject->curLLPowerState = TDA18204_INSTANCE_CUSTOM_CURLLPOWERSTATE_DEF;
    pDrvObject->eHwState = TDA18204_HwState_InitNotDone;

    // TBD if new elements added to pObj

    return err;
}

#ifdef _TVFE_IMPLEMENT_MUTEX

/*============================================================================*/
/* FUNCTION:    iTDA18204_MutexAcquire:                                       */
/*                                                                            */
/* DESCRIPTION: Acquires driver mutex.                                        */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
iTDA18204_MutexAcquire(
    pTDA18204Object_t   pObj,
    UInt32              timeOut
)
{
    tmErrorCode_t   err = TDA18204_ERR_NULL_CONTROLFUNC;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18204_MutexAcquire(0x%08X)", pObj->tUnitW);

    if(P_SMUTEX_ACQUIRE_VALID && P_MUTEX_VALID)
    {
        err = P_SMUTEX_ACQUIRE(P_MUTEX, timeOut);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "Mutex_Acquire(0x%08X, %d) failed.", pObj->tUnitW, timeOut));
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    iTDA18204_MutexRelease:                                       */
/*                                                                            */
/* DESCRIPTION: Releases driver mutex.                                        */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
iTDA18204_MutexRelease(
    pTDA18204Object_t   pObj
)
{
    tmErrorCode_t   err = TDA18204_ERR_NULL_CONTROLFUNC;

    tmDBGPRINTEx(DEBUGLVL_INOUT, "iTDA18204_MutexRelease(0x%08X)", pObj->tUnitW);

    if(P_SMUTEX_RELEASE_VALID && P_MUTEX_VALID)
    {
        err = P_SMUTEX_RELEASE(P_MUTEX);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "Mutex_Release(0x%08X) failed.", pObj->tUnitW));
    }

    return err;
}

#endif

