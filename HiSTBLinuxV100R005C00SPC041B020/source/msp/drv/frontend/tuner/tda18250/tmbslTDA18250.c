/*
  Copyright (C) 2006-2009 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmbslTDA18250.c
 *
 *                3
 *
 * \date          %modify_time%
 *
 * \brief         Describe briefly the purpose of this file.
 *
 * REFERENCE DOCUMENTS :
 *                TDA18250_Driver_User_Guide.pdf
 *
 * Detailed description may be added here.
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
#include "tmddTDA18250.h"

#include "tmbslTDA18250.h"
#include "tmbslTDA18250InstanceCustom.h"
#include "tmbslTDA18250local.h"
#include "tmbslTDA18250Instance.h"

/*============================================================================*/
/* Types and defines:                                                         */
/*============================================================================*/

/*============================================================================*/
/* Global data:                                                               */
/*============================================================================*/
UInt8 uTDA18250TimeTransition[] = {0,2,3,5,9,18,36,224};
UInt16 uTDA18250TimeTransitionAGC1[] = {0,3,5,9,17,33,65,401};

static UInt32 uIrFreq[] = {TMBSL_TDA18250_IR_FREQ1, TMBSL_TDA18250_IR_FREQ2, TMBSL_TDA18250_IR_FREQ3};

static Int32 Lvl_RSSI[] = TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_Lvl_RSSI;

static Int32 Lvl_RfFilter[tmTDA18250_RfFilter_Index_Max][tmTDA18250_RfFilter_X_Max] = TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_Lvl_RfFilter;

static Int32 Lvl_AGC1_MasterMulti[tmTDA18250_Lvl_AGC1_Index_Max][tmTDA18250_AGC1_X_Max] = TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_Lvl_AGC1_MasterMulti;

static Int32 Lvl_AGC1[tmTDA18250_Lvl_AGC1_Index_Max][tmTDA18250_AGC1_X_Max] = TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_Lvl_AGC1;

static Int32 Lvl_AGC2b[tmTDA18250_Lvl_AGC2b_Index_Max][tmTDA18250_AGC2b_X_Max] = TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_Lvl_AGC2b;

static Int32 Lvl_Offset[tmTDA18250_Offset_X_Max] = TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_Lvl_Offset;

static UInt8 Lvl_Div10[tmTDA18250_XDIV10_Index_Max][tmTDA18250_XDIV10_X_Max] = TMBSL_TDA18250_INSTANCE_CUSTOM_COMMON_Lvl_XDIV10;

static tmTDA18250BlocksPath_t tmTDA18250BlocksPath[] = {
                                             {   /* tmTDA18250AnalogChannel */                                           \
                                                 tmddTDA18250_PD_NotActivated,                   /* pdAGC2a        */    \
                                                 tmddTDA18250_PD_Activated,                      /* pdAGC2b        */    \
                                                 tmddTDA18250_PD_NotActivated,                   /* PD_RFAGC_Det   */    \
                                                 tmddTDA18250_PD_Activated,                      /* pdAGC2_DETECT2 */    \
                                                 tmddTDA18250_PD_Activated,                      /* pdAGC2_DETECT3 */    \
                                                 tmddTDA18250_PD_Activated,                      /* pdAGC2_DETECT4 */    \
                                                 tmddTDA18250_PD_NotActivated,                   /* PD_RFAGC_Ifout */    \
                                                 0,                                              /* BUF1b_DCloopIH */    \
                                                 tmddTDA18250_PD_Activated                       /* PDBUF1b        */    \
                                             },                                                                          \
                                             {   /* tmTDA18250DigitalChannel */                                          \
                                                 tmddTDA18250_PD_Activated,                      /* pdAGC2a        */    \
                                                 tmddTDA18250_PD_NotActivated,                   /* pdAGC2b        */    \
                                                 tmddTDA18250_PD_Activated,                      /* PD_RFAGC_Det   */    \
                                                 tmddTDA18250_PD_NotActivated,                   /* pdAGC2_DETECT2 */    \
                                                 tmddTDA18250_PD_NotActivated,                   /* pdAGC2_DETECT3 */    \
                                                 tmddTDA18250_PD_NotActivated,                   /* pdAGC2_DETECT4 */    \
                                                 tmddTDA18250_PD_NotActivated,                   /* PD_RFAGC_Ifout */    \
                                                 1,                                              /* BUF1b_DCloopIH */    \
                                                 tmddTDA18250_PD_NotActivated                    /* PDBUF1b        */    \
                                             }                                                                           \
                                           };


/*============================================================================*/
/* Internal Prototypes:                                                       */
/*============================================================================*/

/*============================================================================*/
/* Exported functions:                                                        */
/*============================================================================*/

/*============================================================================*/
/* FUNCTION:    tmbslTDA18250Init:                                            */
/*                                                                            */
/* DESCRIPTION: Initializes driver setup environment.                         */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmbslTDA18250Init
(
    tmUnitSelect_t              tUnit,      /* I: Unit number      */
    tmbslFrontEndDependency_t*  psSrvFunc   /* I: setup parameters */
)
{
    ptmTDA18250Object_t pObj = Null;
    tmErrorCode_t       err  = TM_OK;

    
    /* Test parameters */
    if (psSrvFunc == Null)
    {
        err = TDA18250_ERR_BAD_PARAMETER;
    }

    if (err == TM_OK)
    {
        /* Get a driver instance */
        err = TDA18250GetInstance(tUnit, &pObj);
    }

    /* Check driver instance state */
    if (err == TM_OK || err == TDA18250_ERR_NOT_INITIALIZED)
    {
        if (pObj != Null && pObj->init == True)
        {
            err = TDA18250_ERR_NOT_INITIALIZED;
        }
        else 
        {
            if (pObj == Null)
            {
                /* Try to allocate an instance of the driver */
                err = TDA18250AllocInstance(tUnit, &pObj);
                if (err != TM_OK || pObj == Null)
                {
                    err = TDA18250_ERR_NOT_INITIALIZED;        
                }
            }

            if (err == TM_OK)
            {
                /* Initialize the driver object */
                pObj->sRWFunc = psSrvFunc->sIo;
                pObj->sTime = psSrvFunc->sTime;
                pObj->sDebug = psSrvFunc->sDebug;

                if (   psSrvFunc->sMutex.Init != Null
                    && psSrvFunc->sMutex.DeInit != Null
                    && psSrvFunc->sMutex.Acquire != Null
                    && psSrvFunc->sMutex.Release != Null)
                {
                    pObj->sMutex = psSrvFunc->sMutex;

                    err = pObj->sMutex.Init(&pObj->pMutex);
                }

                pObj->init = True;

                /* Initialize low level driver setup environment */
                err = tmddTDA18250Init(tUnit, psSrvFunc);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmTDA18250Init(0x%08X) failed.", pObj->tUnit));

            }
        }
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmbslTDA18250DeInit:                                          */
/*                                                                            */
/* DESCRIPTION: Cleans driver setup environment.                              */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t 
tmbslTDA18250DeInit
(
    tmUnitSelect_t  tUnit   /* I: Unit number */
)
{
    ptmTDA18250Object_t pObj = Null;
    tmErrorCode_t       err  = TM_OK;


    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = TDA18250MutexAcquire(pObj, TDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Clean low level driver setup environment */
        err = tmddTDA18250DeInit(tUnit);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250DeInit(0x%08X) failed.", pObj->tUnit));

        /* Release driver mutex */
        (void)TDA18250MutexRelease(pObj);

        if (pObj->sMutex.DeInit != Null)
        {
            /* Clean driver mutex environment */
            if (pObj->pMutex != Null)
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

    err = TDA18250DeAllocInstance(tUnit);

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmbslTDA18250GetSWVersion:                                    */
/*                                                                            */
/* DESCRIPTION: Gets driver version.                                          */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmbslTDA18250GetSWVersion
(
    ptmSWVersion_t  pSWVersion  /* I: Receives SW Version */
)
{
    tmErrorCode_t       err = TDA18250_ERR_NOT_INITIALIZED;
    
    err = tmddTDA18250GetSWVersion(pSWVersion);
    
    return err;
}


/*============================================================================*/
/* FUNCTION:    tmbslTDA18250_GetSWSettingsVersion:                           */
/*                                                                            */
/* DESCRIPTION: Returns the version of the driver settings.                   */
/*                                                                            */
/* RETURN:      TM_OK                                                         */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmbslTDA18250_GetSWSettingsVersion
(
    ptmSWSettingsVersion_t pSWSettingsVersion   /* O: Receives SW Settings Version */
)
{
    pSWSettingsVersion->customerNr      = TMBSL_TDA18250_SETTINGS_CUSTOMER_NUM;
    pSWSettingsVersion->projectNr       = TMBSL_TDA18250_SETTINGS_PROJECT_NUM;
    pSWSettingsVersion->majorVersionNr  = TMBSL_TDA18250_SETTINGS_MAJOR_VER;
    pSWSettingsVersion->minorVersionNr  = TMBSL_TDA18250_SETTINGS_MINOR_VER;

    return TM_OK;
}

/*============================================================================*/
/* FUNCTION:    tmbslTDA18250SetPowerTransitionMode                          */
/*                                                                            */
/* DESCRIPTION: Sets the power state mode                                     */
/*              Normal or smooth                                              */
/*              Default is smooth                                             */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmbslTDA18250SetPowerTransitionMode
(
    tmUnitSelect_t                    tUnit,      /* I: Unit number                   */
    tmTDA18250PowerTransitionMode_t  ePowerTransitionMode  /* I: Power state mode              */
)
{
    ptmTDA18250Object_t pObj = Null;
    tmErrorCode_t           err  = TM_OK;

    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    /* check parameter */
    if (err == TM_OK)
    {
        if( ePowerTransitionMode >= tmTDA18250_PowerTransitionMode_Invalid )
        {
            err = TDA18250_ERR_BAD_PARAMETER;
        }
    }

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = TDA18250MutexAcquire(pObj, TDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* change power state mode */
        pObj->ePowerTransitionMode = ePowerTransitionMode;

        /* Release driver mutex */
        (void)TDA18250MutexRelease(pObj);
    }

    return err;
}



/*============================================================================*/
/* FUNCTION:    tmbslTDA18250SetPowerState:                                   */
/*                                                                            */
/* DESCRIPTION: Sets the power state.                                         */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmbslTDA18250SetPowerState
(
    tmUnitSelect_t          tUnit,      /* I: Unit number                */
    tmTDA18250PowerState_t  powerState  /* I: Power state of this device */
)
{
    ptmTDA18250Object_t pObj = Null;
    tmErrorCode_t       err  = TM_OK;
    UInt8               AGC2_Gain;
    tmddTDA18250_PD_t   ePdDetect;
    
    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = TDA18250MutexAcquire(pObj, TDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        if (powerState==tmTDA18250_PowerNormalMode)
        {
            /* Standby Mode --> Normal Mode */
            /* 1. SM= 0                     */
            /* Buf1b_DCloopIH = 1           */
            /* Close switch in smooth       */

            if (err == TM_OK)
            {
                /* Call tmddTDA18250SetPowerState */
                err = tmddTDA18250SetPowerState(tUnit, (tmddTDA18250PowerState_t)powerState);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetPowerState(0x08X) failed.", pObj->tUnit));
            }

            // new STBY actions
            if (err == TM_OK)
            {
                err = tmddTDA18250SetPDBUF1b(tUnit, tmddTDA18250_PD_NotActivated);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetPDBUF1b(0x08X) failed.", pObj->tUnit));
            }
            
            if (err == TM_OK)
            {
                err = tmddTDA18250SetPD_RFAGC_Ifout(tUnit, tmddTDA18250_PD_NotActivated);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetPD_RFAGC_Ifout(0x08X) failed.", pObj->tUnit));
            }

            if (err == TM_OK)
            {
                /* Set AGC2b Power down */        
                err = tmddTDA18250SetpdAGC2b(tUnit, tmddTDA18250_PD_NotActivated);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetpdAGC2b(0x08X) failed.", pObj->tUnit));
            }
            
            if (err == TM_OK)
            {
                /* Switch the main synthe off */        
                err = tmddTDA18250SetPD_LO_Synthe(tUnit, tmddTDA18250_PD_NotActivated);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetPD_LO_Synthe(0x08X) failed.", pObj->tUnit));
            }
            
            if (err == TM_OK)
            {
                /* Set pdAGC2_DETECT4 Power down */
                err = tmddTDA18250SetpdAGC2_DETECT4(tUnit, tmddTDA18250_PD_NotActivated);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetpdAGC2_DETECT4(0x08X) failed.", pObj->tUnit));
            }
            
            if (err == TM_OK)
            {
                /* Set pdAGC2_DETECT3 Power down */
                err = tmddTDA18250SetpdAGC2_DETECT3(tUnit, tmddTDA18250_PD_NotActivated);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetpdAGC2_DETECT3(0x08X) failed.", pObj->tUnit));
            }

            if (err == TM_OK)
            {
                /* Set pdAGC2_DETECT2 Power down */
                err = tmddTDA18250SetpdAGC2_DETECT2(tUnit, tmddTDA18250_PD_NotActivated);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetpdAGC2_DETECT2(0x08X) failed.", pObj->tUnit));
            }

            /* configure pdDetect1 */
            if (pObj->AGC1Mode!=tmddTDA18250_AGC1_Gain_Free)
            {
                ePdDetect = tmddTDA18250_PD_Activated;
            }
            else
            {
                ePdDetect = tmddTDA18250_PD_NotActivated;
            }

            if (err == TM_OK)
            {
                err = tmddTDA18250SetpdDETECT1(tUnit, ePdDetect);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetpdDETECT1(0x08X) failed.", pObj->tUnit));
            }

            if (err == TM_OK)
            {
                /* Set Digital IF HPF activation */
                err = tmddTDA18250SetPD_hpf(tUnit, tmddTDA18250_PD_NotActivated);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetPD_hpf(0x08X) failed.", pObj->tUnit));
            }
            
            if (err == TM_OK)
            {
                /* AGC2 frozen */
                err = tmddTDA18250SetAGC2_gain_Force(tUnit, tmddTDA18250_OFF);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC2_gain_Force(0x%08X) failed.", tUnit));
            }

            // new STBY actions
            if (err == TM_OK)
            {
                err = tmddTDA18250SetpdAGC1b(tUnit, tmddTDA18250_PD_NotActivated);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetpdAGC1b(0x08X) failed.", pObj->tUnit));
            }
            if (err == TM_OK)
            {
                err = tmddTDA18250SetPDBUF1b(tUnit, tmddTDA18250_PD_NotActivated);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetPDBUF1b(0x08X) failed.", pObj->tUnit));
            }
            
            if ((err == TM_OK) && (pObj->deviceType<tmTDA18250_DeviceTypeSlave) && (pObj->StandardMode>=tmTDA18250_DIG_6MHz))
            {
                /* DCloopIH = 1 */
                err = tmddTDA18250SetBUF1b_DCloopIH (tUnit, 1);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetBUF1b_DCloopIH(0x%08X) failed.", tUnit));
            }

            /* Close the current switch on Master */
            if ((err == TM_OK) && (pObj->deviceType<tmTDA18250_DeviceTypeSlave))
            {
                /* stop AGC1 loop */
                if (err == TM_OK)
                {
                    err = TDA18250SetAGC1_loop_off(tUnit, tmTDA18250_ON);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250SetAGC1_loop_off(0x%08X) failed.", tUnit));
                }

                if (err == TM_OK)
                {
                    err = TDA18250SetSwitchClose (tUnit, pObj->BandIndex);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250SetSwitchClose(0x%08X) failed.", tUnit));
                }
                
                if (err == TM_OK)
                {
                    /* Set AGC1b Power down */        
                    err = tmddTDA18250Setpd_AGC1bis(tUnit, tmddTDA18250_PD_Activated);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250Setpd_AGC1bis(0x08X) failed.", pObj->tUnit));
                }

                /* restart AGC1 loop */
                if (err == TM_OK)
                {
                    err = TDA18250SetAGC1_loop_off(tUnit, tmTDA18250_OFF);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250SetAGC1_loop_off(0x%08X) failed.", tUnit));
                }
            }
        } 
        else
        {
            /* Normal Mode --> Standby Mode */
            /* Open switch in smooth        */
            /* Buf1b_DCloopIH = 0           */
            /* SM= 1                        */

            /* Open the current switch on Master */
            if ((err == TM_OK) && (pObj->deviceType<tmTDA18250_DeviceTypeSlave))
            {
                /* stop AGC1 loop */
                if (err == TM_OK)
                {
                    err = TDA18250SetAGC1_loop_off(tUnit, tmTDA18250_ON);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250SetAGC1_loop_off(0x%08X) failed.", tUnit));
                }
                if (err == TM_OK)
                {
                    err = TDA18250SetSwitchOpen (tUnit);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250SetSwitchOpen(0x%08X) failed.", tUnit));
                }
                /* restart AGC1 loop */
                if (err == TM_OK)
                {
                    err = TDA18250SetAGC1_loop_off(tUnit, tmTDA18250_OFF);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250SetAGC1_loop_off(0x%08X) failed.", tUnit));
                }
            }

            if ((err == TM_OK) && (pObj->deviceType<tmTDA18250_DeviceTypeSlave) && (pObj->StandardMode>=tmTDA18250_DIG_6MHz))
            {
                /* DCloopIH = 0 */
                err = tmddTDA18250SetBUF1b_DCloopIH (tUnit, 0);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetBUF1b_DCloopIH(0x%08X) failed.", tUnit));
            }


            if (err == TM_OK)
            {
                err = tmddTDA18250SetpdAGC1b(tUnit, tmddTDA18250_PD_Activated);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetpdAGC1b(0x08X) failed.", pObj->tUnit));
            }
            if (err == TM_OK)
            {
                err = tmddTDA18250SetPDBUF1b(tUnit, tmddTDA18250_PD_Activated);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetPDBUF1b(0x08X) failed.", pObj->tUnit));
            }

            // new STBY actions
            /* Get and Set back the same gain to refresh internal Tuner memory */
            if (err == TM_OK)
            {
                err = tmddTDA18250GetAGC2b_Gain(tUnit, &AGC2_Gain);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250GetAGC2b_Gain(0x%08X) failed.", tUnit));
            }

            if (err == TM_OK)
            {
                err = tmddTDA18250SetAGC2b_Gain(tUnit, AGC2_Gain);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC2b_Gain(0x%08X) failed.", tUnit));
            }

            if (err == TM_OK)
            {
                /* AGC2 frozen */
                err = tmddTDA18250SetAGC2_gain_Force(tUnit, tmddTDA18250_ON);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC2_gain_Force(0x%08X) failed.", tUnit));
            }

            if (err == TM_OK)
            {
                err = TDA18250WriteAGC2To0dB(tUnit, tmTDA18250_AGC2Step3dB_ON);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250WriteAGC2To0dB(0x%08X) failed.", tUnit));
            }

            if( pObj->ePowerTransitionMode == tmTDA18250_PowerTransitionMode_Smooth )
            {
                if (err == TM_OK)
                {
                    /* Set Digital IF HPF activation */
                    err = tmddTDA18250SetPD_hpf(tUnit, tmddTDA18250_PD_Activated);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetPD_hpf(0x08X) failed.", pObj->tUnit));
                }

                if (err == TM_OK)
                {
                    /* Detector placed at AGC1a/b output */
                    err = tmddTDA18250SetpdDETECT1(tUnit, tmddTDA18250_PD_Activated);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetpdDETECT1(0x08X) failed.", pObj->tUnit));
                }

                if (err == TM_OK)
                {
                    /* Set pdAGC2_DETECT2 Power down */
                    err = tmddTDA18250SetpdAGC2_DETECT2(tUnit, tmddTDA18250_PD_Activated);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetpdAGC2_DETECT2(0x08X) failed.", pObj->tUnit));
                }

                if (err == TM_OK)
                {
                    /* Set pdAGC2_DETECT3 Power down */
                    err = tmddTDA18250SetpdAGC2_DETECT3(tUnit, tmddTDA18250_PD_Activated);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetpdAGC2_DETECT3(0x08X) failed.", pObj->tUnit));
                }

                if (err == TM_OK)
                {
                    /* Set pdAGC2_DETECT4 Power down */
                    err = tmddTDA18250SetpdAGC2_DETECT4(tUnit, tmddTDA18250_PD_Activated);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetpdAGC2_DETECT4(0x08X) failed.", pObj->tUnit));
                }

                if (err == TM_OK)
                {
                    /* Switch the main synthe off */        
                    err = tmddTDA18250SetPD_LO_Synthe(tUnit, tmddTDA18250_PD_Activated);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetPD_LO_Synthe(0x08X) failed.", pObj->tUnit));
                }

                if (err == TM_OK)
                {
                    /* Set AGC1b Power down */        
                    err = tmddTDA18250Setpd_AGC1bis(tUnit, tmddTDA18250_PD_Activated);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250Setpd_AGC1bis(0x08X) failed.", pObj->tUnit));
                }

                if (err == TM_OK)
                {
                    /* Set AGC2b Power down */        
                    err = tmddTDA18250SetpdAGC2b(tUnit, tmddTDA18250_PD_Activated);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetpdAGC2b(0x08X) failed.", pObj->tUnit));
                }

                if (err == TM_OK)
                {
                    err = tmddTDA18250SetPD_RFAGC_Ifout(tUnit, tmddTDA18250_PD_Activated);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetPD_RFAGC_Ifout(0x08X) failed.", pObj->tUnit));
                }

                if (err == TM_OK)
                {
                    err = tmddTDA18250SetPDBUF1b(tUnit, tmddTDA18250_PD_Activated);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetPDBUF1b(0x08X) failed.", pObj->tUnit));
                }
                // new STBY actions
            }

            if (err == TM_OK)
            {
                /* Call tmddTDA18250SetPowerState */
                err = tmddTDA18250SetPowerState(tUnit, (tmddTDA18250PowerState_t)powerState);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetPowerState(0x08X) failed.", pObj->tUnit));
            }

        }

        if (err == TM_OK)
        {
            /* store power state */
            pObj->curPowerState = powerState;
        }

        /* Release driver mutex */
        (void)TDA18250MutexRelease(pObj);
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslTDA18250GetPowerState:                                   */
/*                                                                            */
/* DESCRIPTION: Gets the power state.                                         */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmbslTDA18250GetPowerState
(
    tmUnitSelect_t          tUnit,          /* I: Unit number                */
    tmTDA18250PowerState_t  *pPowerState    /* O: Power state of this device */
)
{
    ptmTDA18250Object_t pObj = Null;
    tmErrorCode_t       err  = TM_OK;


    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = TDA18250MutexAcquire(pObj, TDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (pPowerState == Null)
        {
            err = TDA18250_ERR_BAD_PARAMETER;
        }
        
        if (err == TM_OK)
        {
            /* get power state stored in driver instance */
            *pPowerState = pObj->curPowerState;
        }

        /* Release driver mutex */
        (void)TDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmbslTDA18250Reset:                                           */
/*                                                                            */
/* DESCRIPTION: Resets the TDA18250.                                          */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmbslTDA18250Reset
(
    tmUnitSelect_t  tUnit   /* I: Unit number */
)
{
    ptmTDA18250Object_t pObj = Null;
    tmErrorCode_t       err = TM_OK;


    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = TDA18250MutexAcquire(pObj, TDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        err = TDA18250Reset(tUnit);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250Reset(0x%08X) failed.", tUnit));
 
        if (err == TM_OK)
        {
            /* Writting done all the time after reset */
            pObj->ResetCalledPreviously = True;
        }
        
        /* Release driver mutex */
        (void)TDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmbslTDA18250SetStandardMode:                                 */
/*                                                                            */
/* DESCRIPTION: Sets the standard mode.                                       */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       must be called before SetRF if standard mode changed          */
/*============================================================================*/
tmErrorCode_t
tmbslTDA18250SetStandardMode
(
    tmUnitSelect_t              tUnit,          /* I: Unit number                  */
    tmTDA18250StandardMode_t    StandardMode    /* I: Standard mode of this device */
)
{
    ptmTDA18250Object_t pObj = Null;
    tmErrorCode_t       err  = TM_OK;


    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = TDA18250MutexAcquire(pObj, TDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* store standard mode in driver instance */
        pObj->StandardMode = StandardMode;

        err = TDA18250SetStandardMode(tUnit);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250SetStandardMode(0x%08X) failed.", tUnit));

        /* Release driver mutex */
        (void)TDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmbslTDA18250GetStandardMode:                                 */
/*                                                                            */
/* DESCRIPTION: Gets the standard mode.                                       */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmbslTDA18250GetStandardMode
(
    tmUnitSelect_t              tUnit,          /* I: Unit number                  */
    tmTDA18250StandardMode_t    *pStandardMode  /* O: Standard mode of this device */
)
{
    ptmTDA18250Object_t pObj = Null;
    tmErrorCode_t       err  = TM_OK;


    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = TDA18250MutexAcquire(pObj, TDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (pStandardMode == Null)
        {
            err = TDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            /* Get standard mode stored in driver instance */
            *pStandardMode = pObj->StandardMode;
        }

        /* Release driver mutex */
        (void)TDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmbslTDA18250SetRf:                                           */
/*                                                                            */
/* DESCRIPTION: Sets the RF.                                                  */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmbslTDA18250SetRf
(
    tmUnitSelect_t  tUnit,  /* I: Unit number           */
    UInt32          uRF     /* I: RF frequency in hertz */
)
{    
    ptmTDA18250Object_t   pObj = Null;
    tmErrorCode_t         err  = TM_OK;
    UInt32                uLO = 0;
    UInt8                 uIR_Freq = 0;
    tmTDA18250BandIndex_t uNewBandIndex = tmTDA18250_IND_DIG_Band_1;
    UInt8                 AGC2_Gain = 0;

    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = TDA18250MutexAcquire(pObj, TDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        pObj->uRF = uRF;

        if (err == TM_OK)
        {
            if (err == TM_OK)
            {
                err = tmddTDA18250SetLaunchMSM(tUnit);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetLaunchMSM(0x%08X) failed.", tUnit));
            }

            if (err == TM_OK)
            {
                uLO = uRF + pObj->Std_Array[pObj->StandardMode].IF;
                err = tmddTDA18250SetLOFreq(tUnit, uLO);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetLOFreq(0x%08X) failed.", tUnit));
            }

            if (err == TM_OK)
            {
                /* Set the IR factors according to RF frequency   */
                /* IR_Freq selection: 01=>low, 10=>mid , 11=>high */
                if (uRF <= 200000000)
                {
                    uIR_Freq = 1;            
                }
                else if ((uRF > 200000000) && (uRF <= 600000000))
                {
                    uIR_Freq = 2;
                }
                else if (uRF > 600000000)
                {
                    uIR_Freq = 3;        
                }
                err = tmddTDA18250SetIR_Freq(tUnit, uIR_Freq);   
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetIR_Freq(0x%08X) failed.", tUnit));
            }

            if (err == TM_OK)
            {
                /* Search the new Hybrid Band used */
                err = TDA18250RF2BandIndex(tUnit, uRF, &uNewBandIndex);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250RF2BandIndex(0x%08X) failed.", tUnit));

                if (pObj->BandParam[uNewBandIndex].ChannelType == tmTDA18250DigitalChannel)
                {
                    /* Get and Set back the same gain to refresh internal Tuner memory */
                    if (err == TM_OK)
                    {
                        err = tmddTDA18250GetAGC2b_Gain(tUnit, &AGC2_Gain);
                        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250GetAGC2b_Gain(0x%08X) failed.", tUnit));
                    }
                    if (err == TM_OK)
                    {
                        err = tmddTDA18250SetAGC2b_Gain(tUnit, AGC2_Gain);
                        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC2b_Gain(0x%08X) failed.", tUnit));
                    }  
                    if (err == TM_OK)
                    {
                        /* Set the new Digital Path */
                        if ((uNewBandIndex != pObj->BandIndex) || (pObj->ResetCalledPreviously == True))
                        {
                            err = TDA18250SetDigitalPath(tUnit, uNewBandIndex);
                            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250SetDigitalPath(0x%08X) failed.", tUnit));
                        }
                    }
                }
            }

        }

        if (err == TM_OK)
        {
            /* Writting done all the time after reset */
            pObj->ResetCalledPreviously = False;
        }

        /* Release driver mutex */
        (void)TDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmbslTDA18250GetRf:                                           */
/*                                                                            */
/* DESCRIPTION: Gets the RF.                                                  */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmbslTDA18250GetRf
(
    tmUnitSelect_t  tUnit,  /* I: Unit number           */
    UInt32*         puRF    /* O: RF frequency in hertz */
)
{
    ptmTDA18250Object_t pObj = Null;
    tmErrorCode_t       err  = TM_OK;


    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = TDA18250MutexAcquire(pObj, TDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puRF == Null)
        {
            err = TDA18250_ERR_BAD_PARAMETER;
        }
        
        if (err == TM_OK)
        {
            /* Get RF */
            *puRF = pObj->uRF;
        }

        /* Release driver mutex */
        (void)TDA18250MutexRelease(pObj);
    }
    return err;
}


/*============================================================================*/
/* FUNCTION:    tmTDA18251ReadLO:                                             */
/*                                                                            */
/* DESCRIPTION: Read Lo.                                                      */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmbslTDA18250ReadLO
(
    tmUnitSelect_t  tUnit,  /* I: Unit number                         */
    UInt32          *puLO   /* O: local oscillator frequency in hertz */
)
{
    ptmTDA18250Object_t pObj = Null;
    tmErrorCode_t       err = TM_OK;
    UInt8               uValue;

    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = TDA18250MutexAcquire(pObj, TDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puLO == Null)
        {
            err = TDA18250_ERR_BAD_PARAMETER;
        }
        if (err == TM_OK)
        {
            err = tmddTDA18250GetFreq_prog_En(tUnit,&uValue);
        }
        if (err == TM_OK)
        {
            if (uValue == 1)
            {
                if (err == TM_OK)
                {
                    err = tmddTDA18250GetLOFreq(tUnit,puLO);
                }
            }
            else
            {
                *puLO = pObj->uRF + pObj->Std_Array[pObj->StandardMode].IF;
            }
        }
        /* Release driver mutex */
        (void)TDA18250MutexRelease(pObj);
    }
    return err;
}


/*============================================================================*/
/* FUNCTION:    tmbslTDA18250GetIF:                                           */
/*                                                                            */
/* DESCRIPTION: Gets the TDA18250 IF.                                         */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmbslTDA18250GetIF
(
    tmUnitSelect_t  tUnit,  /* I: Unit number           */
    UInt32*         puIF    /* O: IF Frequency in hertz */
)
{
    ptmTDA18250Object_t pObj = Null;
    tmErrorCode_t       err = TM_OK;


    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = TDA18250MutexAcquire(pObj, TDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puIF == Null)
        {
            err = TDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            /* Get IF */
            *puIF = pObj->Std_Array[pObj->StandardMode].IF;
        }

        /* Release driver mutex */
        (void)TDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmbslTDA18250GetLockStatus:                                   */
/*                                                                            */
/* DESCRIPTION: Gets the Lock Status.                                         */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmbslTDA18250GetLockStatus
(
    tmUnitSelect_t          tUnit,      /* I: Unit number     */
    tmbslFrontEndState_t*   pLockStatus /* O: PLL Lock status */
)
{
    ptmTDA18250Object_t pObj = Null;
    tmErrorCode_t       err = TM_OK;
    UInt8               uValueLO = 0;


    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = TDA18250MutexAcquire(pObj, TDA18250_MUTEX_TIMEOUT);
    }
    
    if (err == TM_OK)
    {
        /* Test parameters */
        if (pLockStatus == Null)
        {
            err = TDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            /* Call tmddTDA18250GetLO_Lock */
            err =  tmddTDA18250GetLO_Lock(tUnit, &uValueLO);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250GetLO_Lock(0x%08X) failed.", tUnit));
        }
    
        if (err == TM_OK)
        {
            *pLockStatus = (uValueLO)? tmbslFrontEndStateLocked : tmbslFrontEndStateNotLocked;
        }

        /* Release driver mutex */
        (void)TDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmbslTDA18250GetPowerLevel:                                   */
/*                                                                            */
/* DESCRIPTION: Gets the Power Level.                                         */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmbslTDA18250GetPowerLevel
(
    tmUnitSelect_t  tUnit,      /* I: Unit number         */
    Int32*          pPowerLevel /* O: Power Level in dBmV */
)
{
    ptmTDA18250Object_t pObj = Null;
    tmErrorCode_t       err = TM_OK;
    UInt8               uRSSIValue;

    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = TDA18250MutexAcquire(pObj, TDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Get the RSSI value for the power level estimation*/
        err =  TDA18250GetRSSI(tUnit, &uRSSIValue);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetRSSI(0x%08X) failed.", tUnit));

        if (err == TM_OK)
        {
            err =  TDA18250GetPower_Level(tUnit, pObj->Std_Array[pObj->StandardMode].IF, uRSSIValue, pPowerLevel);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetPower_Level(0x%08X) failed.", tUnit));
        }
        else
        {
            *pPowerLevel = 0;
        }
            
        /* Release driver mutex */
        (void)TDA18250MutexRelease(pObj);
    }
    return err;
}


/*============================================================================*/
/* FUNCTION:    tmbslTDA18250ResetAGC:                                        */
/*                                                                            */
/* DESCRIPTION: Resets the TDA18250 AGC.                                      */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmbslTDA18250ResetAGC
(
    tmUnitSelect_t  tUnit   /* I: Unit number */
)
{
    ptmTDA18250Object_t pObj = Null;
    tmErrorCode_t       err = TM_OK;


    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = TDA18250MutexAcquire(pObj, TDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        err = TDA18250ResetAGC(tUnit);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250Reset(0x%08X) failed.", tUnit));
 
        /* Release driver mutex */
        (void)TDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmbslTDA18250SetIfLevel:                                      */
/*                                                                            */
/* DESCRIPTION: Sets the TDA18250 IF Level.                                   */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmbslTDA18250SetIfLevel
(
    tmUnitSelect_t         tUnit,     /* I: Unit number */
    tmddTDA18250IF_Level_t uIF_Level  /* I: IF Level    */
)
{
    ptmTDA18250Object_t pObj = Null;
    tmErrorCode_t       err = TM_OK;


    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = TDA18250MutexAcquire(pObj, TDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        err = tmddTDA18250SetIF_level(tUnit, uIF_Level);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetIF_level(0x%08X) failed.", tUnit));


        if (err == TM_OK)
        {
            pObj->uIF_Level = uIF_Level;
        }

        /* Release driver mutex */
        (void)TDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* Internal functions:                                                        */
/*============================================================================*/


/*============================================================================*/
/* FUNCTION:    TDA18250Reset:                                                */
/*                                                                            */
/* DESCRIPTION: Resets the TDA18250.                                          */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
TDA18250Reset
(
    tmUnitSelect_t  tUnit     /* I: Unit number */
)
{   
    ptmTDA18250Object_t   pObj = Null;
    tmErrorCode_t         err = TM_OK;
    UInt8                 uIdentity ;

    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));


    if (err == TM_OK)
    {
        /***** Write all initialization bytes *****/
        err = tmddTDA18250Reset(tUnit, pObj->clock_mode_auto, tmddTDA18250_WithResetAGC);

        if (err == TM_OK)
        {
            err = tmddTDA18250ResetXtoutMTO(tUnit, (tmddTDA18250DeviceType_t)pObj->deviceType);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250ResetXtoutMTO(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = tmddTDA18250SetIF_level(tUnit, pObj->uIF_Level);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetIF_level(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = tmddTDA18250SetResetMSM(tUnit);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetResetMSM(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = tmddTDA18250SetLaunchMSM(tUnit);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetLaunchMSM(0x%08X) failed.", tUnit));
        }

        /***** Image Rejection Calibration *****/
        if (err == TM_OK)
        {
            err = TDA18250IRCAL_InitRestore(tUnit, tmTDA18250_INIT);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250IRCAL_Init(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = TDA18250IRCalibration(tUnit);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250IRCalibration(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = TDA18250IRCAL_InitRestore(tUnit, tmTDA18250_RESTORE);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250IRCAL_Init(0x%08X) failed.", tUnit));
        }

        /* Get HW Identity */
        if (err == TM_OK)
        {
            err = tmddTDA18250GetIdentity(tUnit, &uIdentity);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250GetIdentity(0x%08X) failed.", tUnit));
        }

        /***** Reset AGC *****/
        if (err == TM_OK)
        {
            TDA18250ResetAGC(tUnit);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250ResetAGC(0x%08X) failed.", tUnit));
        }
        
        /***** Write default LO *****/
        if (err == TM_OK)
        {
            tmddTDA18250SetLOFreq(tUnit, TMBSL_TDA18250_DEFAULT_LO);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetLOFreq(0x%08X) failed.", tUnit));
            pObj->uRF = TMBSL_TDA18250_DEFAULT_LO - pObj->Std_Array[pObj->StandardMode].IF ;
        }
        
        /***** Write default smooth offset between RF and Loop throught *****/
        if (err == TM_OK)
        {
            tmddTDA18250SetLT_smooth_offset(tUnit, 0);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetLT_smooth_offset(0x%08X) failed.", tUnit));
        }

        /* switches initialization: time constant equal zero previously to ensure direct access */
        if (err == TM_OK)
        {
            err = TDA18250SwitchDirectAccess(tUnit, tmTDA18250_DTA_bypass, tmddTDA18250_SW_CLOSE);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250SwitchDirectAccess(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = TDA18250SwitchDirectAccess(tUnit, tmTDA18250_RFSW_ANALOG, tmddTDA18250_SW_OPEN);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250SwitchDirectAccess(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = TDA18250SwitchDirectAccess(tUnit, tmTDA18250_RFSW_LP, tmddTDA18250_SW_OPEN);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250SwitchDirectAccess(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = TDA18250SwitchDirectAccess(tUnit, tmTDA18250_RFSW_HP, tmddTDA18250_SW_OPEN);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250SwitchDirectAccess(0x%08X) failed.", tUnit));
        }
        
        /* Switch smooth time constant setting */
        if (err == TM_OK)
        {
            err = tmddTDA18250_SetRFSW_T_CST(tUnit, pObj->Switch_TimeCst);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250_SetRFSW_T_CST(0x%08X) failed.", tUnit));
        }

        /* default band is DIGITAL 6MHz */
        if (err == TM_OK)
        {
            pObj->BandIndex = tmTDA18250_IND_DIG_Band_6;
        }

        if (err == TM_OK)
        {
            err= tmddTDA18250SetHP_MTO_T_CST(tUnit, pObj->MTO_TimeCst);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetHP_MTO_T_CST(0x%08X) failed.", tUnit));
        }

        if (err == TM_OK)
        {
            err= tmddTDA18250Setnotch_select(tUnit, 01);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250Setnotch_select(0x%08X) failed.", tUnit));
        }

    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    TDA18250IRCalibration:                                        */
/*                                                                            */
/* DESCRIPTION: Image Rejection calibration.                                  */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
TDA18250IRCalibration
(
    tmUnitSelect_t tUnit        /* I: Unit number */
)
{   
    ptmTDA18250Object_t   pObj = Null;
    tmErrorCode_t         err = TM_OK;
    UInt8                 FreqCpt = 0;

    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));
        
    if (err == TM_OK)
    {
        /***** Image Rejection Calibration *****/
        for (FreqCpt=0; FreqCpt<3; FreqCpt++)
        {
            if (err == TM_OK)
            {
                err = TDA18250SignalCalibration(tUnit, FreqCpt+1, uIrFreq[FreqCpt]);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250SignalCalibration(0x%08X) failed.", tUnit));
            }
            if (err == TM_OK)
            {
                err = TDA18250ImageCalibration(tUnit);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250ImageCalibration(0x%08X) failed.", tUnit));
            }
        }
        
        if (err == TM_OK)
        {
            err = tmddTDA18250Sethc_mode(tUnit, tmddTDA18250_HcModeNormal);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250Sethc_mode(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = tmddTDA18250SetLaunchMSM(tUnit);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetLaunchMSM(0x%08X) failed.", tUnit));
        }
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    TDA18250IRCAL_InitRestore:                                    */
/*                                                                            */
/* DESCRIPTION: IC Init before / Restore after calibration.                   */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       Analog configuration by default in Reset()                    */
/*============================================================================*/
tmErrorCode_t
TDA18250IRCAL_InitRestore
(
    tmUnitSelect_t     tUnit,       /* I: Unit number */
    tmTDA18250_Step_t  uIRStep      /* I: IRCAL Step  */
)
{   
    ptmTDA18250Object_t             pObj = Null;
    tmErrorCode_t                   err = TM_OK;
    UInt8                           uPower_Down_byte_2 = 0xAF;
    UInt8                           uAGC2b_byte = 0x8F;
    tmddTDA18250_SW_RFcal_modeOFF_t uSwitch = tmddTDA18250_RFcal_modeOFF_Max;
    tmddTDA18250_PD_t               uPD = tmddTDA18250_PD_Max;

    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));
        
    if (err == TM_OK)
    {
        if (uIRStep == tmTDA18250_INIT)
        {
            uSwitch            = tmddTDA18250_RFcal_modeOFF_OPEN;
            uPD                = tmddTDA18250_PD_Activated;
            uPower_Down_byte_2 = 0xAF;
            uAGC2b_byte        = 0x8F;
        }
        if (uIRStep == tmTDA18250_RESTORE)
        {
            uSwitch            = tmddTDA18250_RFcal_modeOFF_CLOSE;
            uPD                = tmddTDA18250_PD_NotActivated;
            switch (pObj->deviceType)
            {
                case tmTDA18250_DeviceTypeMasterMultiWoXTout:
                case tmTDA18250_DeviceTypeMasterMulti:
                case tmTDA18250_DeviceTypeMasterSingleWithLTWithXTout:
                case tmTDA18250_DeviceTypeMasterSingleWithLTWoXTout:
                case tmTDA18250_DeviceTypeSlaveWithLTWoXTout:
                    uPower_Down_byte_2 = 0x63;
                    break;
                case tmTDA18250_DeviceTypeMasterSingleWoLTWithXTout:
                case tmTDA18250_DeviceTypeMasterSingleWoLTWoXTout:
                case tmTDA18250_DeviceTypeSlaveWithXTout:
                case tmTDA18250_DeviceTypeSlave:
                    uPower_Down_byte_2 = 0x23;
                    break;
                default:
                    err = TDA18250_ERR_BAD_PARAMETER;
                    break;
            }
            uAGC2b_byte = 0x9F;
        }

        if (err == TM_OK)
        {
            /* Switch off/on the entire filter switch */
            err = tmddTDA18250SetRFcal_modeOFF(tUnit, uSwitch);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetRFcal_modeOFF(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            /* Set Power down of the continuous AGC */
            err = tmddTDA18250SetpdAGC2a(tUnit, uPD);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetpdAGC2a(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            /* Set Power down of the continuous AGC1b */
            err = tmddTDA18250SetpdAGC1b(tUnit, uPD);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetpdAGC1b(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = tmddTDA18250Write(tUnit, 0x18, 1, &uPower_Down_byte_2);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250Write(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = tmddTDA18250Write(tUnit, 0x1C, 1, &uAGC2b_byte);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250Write(0x%08X) failed.", tUnit));
        }

    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    TDA18250SignalCalibration:                                    */
/*                                                                            */
/* DESCRIPTION: Signal Calibration.                                           */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
TDA18250SignalCalibration
(
    tmUnitSelect_t tUnit,       /* I: Unit number */
    UInt8          uIR_Freq,    /* I: Freq step   */
    UInt32         uLO          /* I: LO          */
)
{   
    ptmTDA18250Object_t   pObj = Null;
    tmErrorCode_t         err = TM_OK;
    UInt8                 FlagMSM = 0;
    UInt32                ulTimeout = 0;
  
    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));
        
    if (err == TM_OK)
    {
        /* IR_Freq selection: 01=>low, 10=>mid , 11=>high */
        err = tmddTDA18250SetIR_Freq(tUnit, uIR_Freq);   

        if (err == TM_OK)
        {
            err = tmddTDA18250Sethc_mode(tUnit, tmddTDA18250_HcModeIRcalWanted);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250Sethc_mode(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = tmddTDA18250SetCAL_Int(tUnit, 0x13);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetCAL_Int(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = tmddTDA18250SetLOFreq(tUnit, uLO);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetLOFreq(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = TDA18250Wait(pObj, 5);  /* wait 5ms */
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250Wait(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = tmddTDA18250SetResetFlags(tUnit);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetResetFlags(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = tmddTDA18250SetLaunchMSM(tUnit);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetLaunchMSM(0x%08X) failed.", tUnit));
        }

        if (err == TM_OK)
        {
            ulTimeout = 0;

            do
            {
                err = TDA18250Wait(pObj, TDA18250_SIGNAL_CALIB_WAIT);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250Wait(0x%08X) failed.", tUnit));

                /* retrive calibration status */        
                if( err == TM_OK )
                {        
                    err = tmddTDA18250GetFlagMSM_OK(tUnit, &FlagMSM);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250GetFlagMSM_OK(0x%08X) failed.", tUnit));
                }

                ulTimeout ++;

            } while( (FlagMSM != 1) && (ulTimeout < TDA18250_SIGNAL_CALIB_TIMEOUT) && (err == TM_OK) );

            /* check whether timeout occurs */    
            if( (FlagMSM != 1) || (err != TM_OK) ) 
            {
                err = TDA18250_ERR_HW_FAILED;
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250SignalCalibration: FlagMSM_OK(0x%08X) failed.", tUnit));
            }
        }
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    TDA18250ImageCalibration:                                     */
/*                                                                            */
/* DESCRIPTION: Image Calibration.                                            */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
TDA18250ImageCalibration
(
    tmUnitSelect_t tUnit     /* I: Unit number */
)
{   
    ptmTDA18250Object_t   pObj = Null;
    tmErrorCode_t         err = TM_OK;
    UInt8                 FlagMSM = 0;
    UInt32                ulTimeout = 0;

    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));
        
    if (err == TM_OK)
    {
        err = tmddTDA18250SetCAL_Int(tUnit, 0xED);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetCAL_Int(0x%08X) failed.", tUnit));

        if (err == TM_OK)
        {
            err = tmddTDA18250SetFreq_prog_Start(tUnit);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetFreq_prog_Start(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = TDA18250Wait(pObj, 5);  /* wait 5ms */
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250Wait(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = tmddTDA18250Sethc_mode(tUnit, tmddTDA18250_HcModeIRcalImage);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250Sethc_mode(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = tmddTDA18250SetResetFlags(tUnit);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetResetFlags(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = tmddTDA18250SetLaunchMSM(tUnit);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetLaunchMSM(0x%08X) failed.", tUnit));
        }

        if (err == TM_OK)
        {
            ulTimeout = 0;

            do
            {
                err = TDA18250Wait(pObj, TDA18250_IMAGE_CALIB_WAIT);  
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250Wait(0x%08X) failed.", tUnit));

                /* retrive calibration status */        
                if( err == TM_OK )
                {        
                    err = tmddTDA18250GetFlagMSM_OK(tUnit, &FlagMSM);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250GetFlagMSM_OK(0x%08X) failed.", tUnit));
                }

                ulTimeout ++;

            } while( (FlagMSM != 1) && (ulTimeout < TDA18250_IMAGE_CALIB_TIMEOUT) && (err == TM_OK) );

            /* check whether timeout occurs */    
            if( (FlagMSM != 1) || (err != TM_OK) ) 
            {
                err = TDA18250_ERR_HW_FAILED;
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250ImageCalibration: FlagMSM_OK(0x%08X) failed.", tUnit));
            }
        }
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    TDA18250ResetAGC:                                             */
/*                                                                            */
/* DESCRIPTION: resets AGC1 & AGC2 or AGC1 only.                              */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
TDA18250ResetAGC
(
    tmUnitSelect_t   tUnit      /* I: Unit number */
)
{   
    ptmTDA18250Object_t   pObj = Null;
    tmErrorCode_t         err = TM_OK;

    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if( err == TM_OK )
    {
        /* Check AGC1 gain forced (master/slave) */
        if( pObj->AGC1Mode == tmddTDA18250_AGC1_Gain_Free )
        {
            /* master case */

            /* initialization */
            if (err == TM_OK)
            {
                err = tmddTDA18250SetAGC1_smooth_t_cst(tUnit, tmddTDA18250_TCST_0);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC1_smooth_t_cst(0x%08X) failed.", tUnit));
            }
            if (err == TM_OK)
            {
                err = tmddTDA18250SetAGC1_Do_step(tUnit, 0);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC1_Do_step(0x%08X) failed.", tUnit));
            }
            if (err == TM_OK)
            {
                err = tmddTDA18250SetAGC1_bypass(tUnit, tmddTDA18250_ON);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC1_bypass(0x%08X) failed.", tUnit));
            }

            /* Start AGC clock */
            if (err == TM_OK)
            {
                err = tmddTDA18250SetAGC_On(tUnit, 1);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC_On(0x%08X) failed.", tUnit));
            }

            /* AGC TOP writting */
            if (err == TM_OK)
            {
                err = TDA18250WriteAGC1_TOP(tUnit);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250WriteAGC1_TOP(0x%08X) failed.", tUnit));
            }

#ifdef AGC1_WAIT
            if (err == TM_OK)
            {
                err = TDA18250Wait(pObj, 100);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250Wait(0x%08X) failed.", tUnit));
            }
#endif

            if (err == TM_OK)
            {
                err = tmddTDA18250SetAGC1_Do_step(tUnit, 3);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC1_Do_step(0x%08X) failed.", tUnit));
            }

            /* Reset AGC */
            if (err == TM_OK)
            {
                err = tmddTDA18250SetAGC1_bypass(tUnit, tmddTDA18250_OFF);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC1_bypass(0x%08X) failed.", tUnit));
            }
            if ((err == TM_OK) && (pObj->Std_Array[pObj->StandardMode].ChannelType == tmTDA18250DigitalChannel) )
            {
                err = TDA18250SetAGC2_Bypass(tUnit);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250SetAGC2_Bypass(0x%08X) failed.", tUnit));
            }

#ifdef AGC1_WAIT
            if (err == TM_OK)
            {
                err = TDA18250Wait(pObj, 520);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250Wait(0x%08X) failed.", tUnit));
            }
#endif

            /* AGC1 smooth transition time */
            if (err == TM_OK)
            {
                err = tmddTDA18250SetAGC1_smooth_t_cst(tUnit, pObj->AGC1_TimeCst);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC1_smooth_t_cst(0x%08X) failed.", tUnit));
            }

            /* Loop Through smooth transition time */
            if (err == TM_OK)
            {
                err = tmddTDA18250SetLT_smooth_t_cst(tUnit, pObj->LT_TimeCst);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetLT_smooth_t_cst(0x%08X) failed.", tUnit));
            }

            /* configure pdDetect1 */
            if( (err == TM_OK) && (pObj->AGC1Mode!=tmddTDA18250_AGC1_Gain_Free) )
            {
                err = tmddTDA18250SetpdDETECT1( tUnit, tmddTDA18250_PD_NotActivated );
            }
        }
        else
        {
            /* slave case */
#ifdef AGC1_WAIT
            err = tmddTDA18250_SetAGC1_gain_Force(tUnit,tmddTDA18250_AGC_Mode_forced);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250_SetAGC1_gain_Force(0x%08X) failed.", tUnit));
#endif

            if (err == TM_OK)
            {
                err = tmddTDA18250_SetAGC1_Gain(tUnit,pObj->AGC1Mode);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250_SetAGC1_gain(0x%08X) failed.", tUnit));
            }

            /* configure pdDetect1 */
            if( err == TM_OK )
            {
                err = tmddTDA18250SetpdDETECT1(tUnit, tmddTDA18250_PD_Activated);
            }
        }
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    TDA18250WriteAGC1_TOP:                                        */
/*                                                                            */
/* DESCRIPTION: Writes AGC1 TOP values from the table AGC1GainCoeff.          */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
TDA18250WriteAGC1_TOP
(
    tmUnitSelect_t   tUnit      /* I: Unit number */
)
{   
    ptmTDA18250Object_t   pObj = Null;
    tmErrorCode_t         err = TM_OK;
    UInt8                 uAudAdd;

    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Up Gain selection, programming and validation */
        err = tmddTDA18250SetAGC1_Gup_sel(tUnit, tmddTDA18250_GUD_UP);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC1_Gup_sel(0x%08X) failed.", tUnit));
        
        if (err == TM_OK)
        {
            err = tmddTDA18250SetAGC1_Gud(tUnit, pObj->AGC1_Gud[tmTDA18250_AGC1_Gud_Up]);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC1_Gud(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = tmddTDA18250SetAGC1_Gud_valid(tUnit, tmddTDA18250_ON);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC1_Gud_valid(0x%08X) failed.", tUnit));
        }
        
        /* Down Gain selection, programming and validation */
        if (err == TM_OK)
        {
            err = tmddTDA18250SetAGC1_Gup_sel(tUnit, tmddTDA18250_GUD_DOWN);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC1_Gup_sel(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = tmddTDA18250SetAGC1_Gud(tUnit, pObj->AGC1_Gud[tmTDA18250_AGC1_Gud_Down]);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC1_Gud(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = tmddTDA18250SetAGC1_Gud_valid(tUnit, tmddTDA18250_ON);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC1_Gud_valid(0x%08X) failed.", tUnit));
        }
        
        /* Up Gain coeff selection */
        if (err == TM_OK)
        {
            err = tmddTDA18250SetAGC1_aud_sel(tUnit, tmddTDA18250_AUD_UP);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC1_aud_sel(0x%08X) failed.", tUnit));
        }
        /* Up Gain coeff programming and validation */
        if (err == TM_OK)
        {
            for (uAudAdd = tmTDA18250_AGC1_Aup_Minus2dB; uAudAdd<=tmTDA18250_AGC1_Aup_16_125dB; uAudAdd++)
            {
                err = TDA18250WriteAGC1_UpDownGain(tUnit, uAudAdd, pObj->AGC1_Aup[uAudAdd]);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250WriteAGC1_UpDownGain(0x%08X) failed.", tUnit));
            }
        }
        
        /* Down Gain coeff selection */
        if (err == TM_OK)
        {
            err = tmddTDA18250SetAGC1_aud_sel(tUnit, tmddTDA18250_AUD_DOWN);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC1_aud_sel(0x%08X) failed.", tUnit));
        }
        /* Down Gain coeff programming and validation */
        if (err == TM_OK)
        {
            for (uAudAdd = tmTDA18250_AGC1_Ado_2dB; uAudAdd<=tmTDA18250_AGC1_Ado_18dB; uAudAdd++)
            {
                err = TDA18250WriteAGC1_UpDownGain(tUnit, uAudAdd, pObj->AGC1_Ado[uAudAdd]);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250WriteAGC1_UpDownGain(0x%08X) failed.", tUnit));
            }
        }
        
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    TDA18250WriteAGC1_UpDownGain:                                 */
/*                                                                            */
/* DESCRIPTION: Writes AGC1 Up/Down gains.                                    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       Up/Down previously set                                        */
/*============================================================================*/
tmErrorCode_t
TDA18250WriteAGC1_UpDownGain
(
    tmUnitSelect_t   tUnit,    /* I: Unit number     */
    UInt8            uAudAdd,  /* I: Up/Down address */
    UInt8            uAudGain  /* I: Up/Down gain    */
)
{   
    ptmTDA18250Object_t   pObj = Null;
    tmErrorCode_t         err = TM_OK;

    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Gain coeff selection, programming and validation */
        if (err == TM_OK)
        {
            err = tmddTDA18250SetAGC1_au_ptr(tUnit, uAudAdd);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC1_au_ptr(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = tmddTDA18250SetAGC1_aud(tUnit, uAudGain);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC1_aud(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = tmddTDA18250SetAGC1_aud_valid(tUnit, tmddTDA18250_ON);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC1_aud_valid(0x%08X) failed.", tUnit));
        }
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    TDA18250WriteAGC2_TOP:                                        */
/*                                                                            */
/* DESCRIPTION: Writes AGC2 TOP values from the table AGC2_Gain in            */
/*              tmTDA18250 regarding the BP filter index.                     */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
TDA18250WriteAGC2_TOP
(
    tmUnitSelect_t   tUnit,    /* I: Unit number     */
    UInt8            uBPIndex  /* I: BP filter index */
)
{   
    ptmTDA18250Object_t   pObj = Null;
    tmErrorCode_t         err = TM_OK;
    UInt8                 uGudAdd;

    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* No Up/Down gain validation */
        err = tmddTDA18250SetAGC2_Gud_valid(tUnit, tmddTDA18250_OFF);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC2_Gud_valid(0x%08X) failed.", tUnit));

        /* Up Gain selection */
        if (err == TM_OK)   
        {
            err = tmddTDA18250SetAGC2_Gup_sel(tUnit, tmddTDA18250_GUD_UP);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC2_Gup_sel(0x%08X) failed.", tUnit));
        }
        
        /* Detector programming and validation */
        /* remark: Up Gain = Down Gain + 1     */
        if (err == TM_OK)
        {
            for (uGudAdd = tmTDA18250_AGC2_Detector2; uGudAdd<=tmTDA18250_AGC2_Detector4; uGudAdd++)
            {
                err = TDA18250WriteAGC2_UpDownGain(tUnit, uGudAdd, pObj->AGC2_GdownDetector[uBPIndex][uGudAdd-1]+1);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250WriteAGC2_UpDownGain(0x%08X) failed.", tUnit));
            }
        }
        
        /* Down Gain selection */
        if (err == TM_OK)   
        {
            err = tmddTDA18250SetAGC2_Gup_sel(tUnit, tmddTDA18250_GUD_DOWN);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC2_Gup_sel(0x%08X) failed.", tUnit));
        }
        
        /* Detector programming and validation */
        if (err == TM_OK)
        {
            for (uGudAdd = tmTDA18250_AGC2_Detector2; uGudAdd<=tmTDA18250_AGC2_Detector4; uGudAdd++)
            {
                err = TDA18250WriteAGC2_UpDownGain(tUnit, uGudAdd, pObj->AGC2_GdownDetector[uBPIndex][uGudAdd-1]);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250WriteAGC2_UpDownGain(0x%08X) failed.", tUnit));
            }
        }
        
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    TDA18250WriteAGC2_UpDownGain:                                 */
/*                                                                            */
/* DESCRIPTION: Writes AGC2 Up/Down gains.                                    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       Up / Down previously set                                      */
/*============================================================================*/
tmErrorCode_t
TDA18250WriteAGC2_UpDownGain
(
    tmUnitSelect_t   tUnit,    /* I: Unit number     */
    UInt8            uGudAdd,  /* I: Up/Down address */
    UInt8            uGudGain  /* I: Up/Down gain    */
)
{   
    ptmTDA18250Object_t   pObj = Null;
    tmErrorCode_t         err = TM_OK;

    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* 2, 3 or 4 selection in the AGC2 control detection, programming and validation */
        if (err == TM_OK)
        {
            err = tmddTDA18250SetAGC2_Gud_ptr_sel(tUnit, uGudAdd);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC2_Gud_ptr_sel(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = tmddTDA18250SetAGC2_Gud(tUnit, uGudGain);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC2_Gud(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = tmddTDA18250SetAGC2_Gud_valid(tUnit, tmddTDA18250_ON);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC2_Gud_valid(0x%08X) failed.", tUnit));
        }
        
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    TDA18250CalAV:                                                */
/*                                                                            */
/* DESCRIPTION: RF tracking filter AV calibration.                            */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
TDA18250CalAV
(
    tmUnitSelect_t tUnit    /* I: Unit number */
)
{   
    ptmTDA18250Object_t   pObj = Null;
    tmErrorCode_t         err = TM_OK;
    UInt8                 uWait;

    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {

        err = tmddTDA18250SetRSSI_Byte(tUnit, 0xDE);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetRSSI_Byte(0x%08X) failed.", tUnit));
        
        if (err == TM_OK)
        {
            err = tmddTDA18250SetCAL_Int(tUnit, 0x13);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetCAL_Int(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = tmddTDA18250SetResetFlags(tUnit);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetResetFlags(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = tmddTDA18250Sethc_mode(tUnit, tmddTDA18250_HcModeRFcalAV);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250Sethc_mode(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = tmddTDA18250SetLaunchMSM(tUnit);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetLaunchMSM(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = tmddTDA18250GetNmeasAV(tUnit, &uWait);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250GetNmeasAV(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            uWait = 1 + 2*uWait;
            err = TDA18250Wait(pObj, uWait);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250Wait(0x%08X) failed.", tUnit));
        }

        if (err == TM_OK)
        {
            err = tmddTDA18250_Setrfcal_clock_en(tUnit, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250Setrfcal_clock_en(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = tmddTDA18250_Setrfcal_clock_en(tUnit, 0);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250Setrfcal_clock_en(0x%08X) failed.", tUnit));
        }
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    TDA18250SetSwitchOpen:                                        */
/*                                                                            */
/* DESCRIPTION: Opens the current switch.                                     */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       RFSW_ANALOG always in smooth access                           */
/*              DTA_bypass always in direct access                            */
/*============================================================================*/
tmErrorCode_t
TDA18250SetSwitchOpen
(
    tmUnitSelect_t  tUnit   /* I: Unit number */
)
{
    ptmTDA18250Object_t       pObj = Null;
    tmErrorCode_t             err  = TM_OK;
    tmTDA18250_SwitchSmooth_t eSwitchSmooth;


    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Switch smooth management */
        if (pObj->Switch_TimeCst == tmddTDA18250_TCST_0)
            eSwitchSmooth = tmTDA18250_SW_SMOOTH_OFF;
        else
            eSwitchSmooth = tmTDA18250_SW_SMOOTH_ON;
        
        /* Open the switch in the current Band */
        if (pObj->BandParam[pObj->BandIndex].Switch == tmTDA18250_DTA_bypass)
        {
            err = TDA18250SwitchDirectAccess(tUnit, tmTDA18250_DTA_bypass, tmddTDA18250_SW_OPEN);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250SwitchSmoothAccess(0x%08X) failed.", tUnit));
        }
        else
        {
            switch (eSwitchSmooth)
            {   
                case tmTDA18250_SW_SMOOTH_OFF:
                    err = TDA18250SwitchDirectAccess(tUnit, pObj->BandParam[pObj->BandIndex].Switch, tmddTDA18250_SW_OPEN);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250SwitchDirectAccess(0x%08X) failed.", tUnit));
                    break;
                    
                case tmTDA18250_SW_SMOOTH_ON:
                    err = TDA18250SwitchSmoothAccess(tUnit, pObj->BandParam[pObj->BandIndex].Switch, tmddTDA18250_SW_OPEN);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250SwitchSmoothAccess(0x%08X) failed.", tUnit));
                    break;
                default:
                    break;
            }
        }

        /* save the switch state of the current band */
        if (err == TM_OK)
        {
            pObj->BandParam[pObj->BandIndex].SwitchState = tmddTDA18250_SW_OPEN;
        }
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    TDA18250SetSwitchClose:                                       */
/*                                                                            */
/* DESCRIPTION: Closes the new switch.                                        */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       RFSW_ANALOG always in smooth access                           */
/*              DTA_bypass always in direct access                            */
/*============================================================================*/
tmErrorCode_t
TDA18250SetSwitchClose
(
    tmUnitSelect_t            tUnit,         /* I: Unit number */
    tmTDA18250BandIndex_t     uNewBandIndex  /* I: Band Index  */
)
{
    ptmTDA18250Object_t       pObj = Null;
    tmErrorCode_t             err  = TM_OK;
    tmTDA18250_SwitchSmooth_t eSwitchSmooth;

    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Switch smooth management */
        if (pObj->Switch_TimeCst == tmddTDA18250_TCST_0)
            eSwitchSmooth = tmTDA18250_SW_SMOOTH_OFF;
        else
            eSwitchSmooth = tmTDA18250_SW_SMOOTH_ON;

        /* Close the new switch */
        if (pObj->BandParam[uNewBandIndex].Switch == tmTDA18250_DTA_bypass)
        {
            err = TDA18250SwitchDirectAccess(tUnit, pObj->BandParam[uNewBandIndex].Switch, tmddTDA18250_SW_CLOSE);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250SwitchSmoothAccess(0x%08X) failed.", tUnit));
        }
        else
        {
            switch (eSwitchSmooth)
            {   
                case tmTDA18250_SW_SMOOTH_OFF:
                    err = TDA18250SwitchDirectAccess(tUnit, pObj->BandParam[uNewBandIndex].Switch, tmddTDA18250_SW_CLOSE);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250SwitchDirectAccess(0x%08X) failed.", tUnit));
                    break;
                    
                case tmTDA18250_SW_SMOOTH_ON:
                    err = TDA18250SwitchSmoothAccess(tUnit, pObj->BandParam[uNewBandIndex].Switch, tmddTDA18250_SW_CLOSE);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250SwitchSmoothAccess(0x%08X) failed.", tUnit));
                    break;
               default:
                    break;
            }
        }

        /* save the new RF Band used with the switch state associated */
        if (err == TM_OK)
        {
            pObj->BandParam[uNewBandIndex].SwitchState = tmddTDA18250_SW_CLOSE;
        }
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    TDA18250SwitchDirectAccess:                                   */
/*                                                                            */
/* DESCRIPTION: Opens the current switch in direct access.                    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
TDA18250SwitchDirectAccess
(
    tmUnitSelect_t            tUnit,       /* I: Unit number  */
    tmTDA18250Switch_t        Switch,      /* I: Switch Name  */
    tmddTDA18250SwitchState_t SwitchState  /* I: Switch State */
)
{
    ptmTDA18250Object_t pObj = Null;
    tmErrorCode_t       err  = TM_OK;


    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {        
        /* direct access */
        if (err == TM_OK)
        {
            switch (Switch)
            {
                case tmTDA18250_RFSW_ANALOG:
                    err = tmddTDA18250SetRFSW_ANALOG(tUnit, SwitchState);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetRFSW_ANALOG(0x%08X) failed.", tUnit));
                    break;
                    
                case tmTDA18250_RFSW_LP:
                    err = tmddTDA18250SetRFSW_LP(tUnit, SwitchState);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetRFSW_LP(0x%08X) failed.", tUnit));
                    break;
                    
                case tmTDA18250_RFSW_HP:
                    err = tmddTDA18250SetRFSW_HP(tUnit, SwitchState);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetRFSW_HP(0x%08X) failed.", tUnit));
                    break;
                    
                case tmTDA18250_DTA_bypass:
                    err = tmddTDA18250SetDTA_bypass(tUnit, SwitchState);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetDTA_bypass(0x%08X) failed.", tUnit));
                    break;
                    
                default:
                    err = ddTDA18250_ERR_NOT_SUPPORTED;
            }
        }

    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    TDA18250SwitchSmoothAccess:                                   */
/*                                                                            */
/* DESCRIPTION: Opens the current switch in direct access.                    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
TDA18250SwitchSmoothAccess
(
    tmUnitSelect_t            tUnit,       /* I: Unit number  */
    tmTDA18250Switch_t        Switch,      /* I: Switch Name  */
    tmddTDA18250SwitchState_t SwitchState  /* I: Switch State */
)
{
    ptmTDA18250Object_t       pObj = Null;
    tmErrorCode_t             err  = TM_OK;
    tmddTDA18250SwitchState_t RdSwitchState;
    UInt8                     uState;
    UInt8                     uCpt;

    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /*  Check smooth access */
        switch (Switch)
        {
            case tmTDA18250_RFSW_ANALOG:
                err = tmddTDA18250GetRFSW_ANALOG(tUnit, &RdSwitchState);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250GetRFSW_ANALOG(0x%08X) failed.", tUnit));
                break;
                
            case tmTDA18250_RFSW_LP:
                err = tmddTDA18250GetRFSW_LP(tUnit, &RdSwitchState);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250GetRFSW_LP(0x%08X) failed.", tUnit));
                break;
                
            case tmTDA18250_RFSW_HP:
                err = tmddTDA18250GetRFSW_HP(tUnit, &RdSwitchState);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250GetRFSW_HP(0x%08X) failed.", tUnit));
                break;
                
            case tmTDA18250_DTA_bypass:
            default:
                err = TDA18250_ERR_NOT_SUPPORTED;
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "DTA_bypass Not supported(0x%08X) failed.", tUnit));
                break;
        }

        /* check switch state value readden */
        if (err == TM_OK)
        {
            if (RdSwitchState == SwitchState)
            {
                return TM_OK;
            }
        }
    }

    if (err == TM_OK)
    {
        /*  Check smooth state machine is available */
        /* AGC1 loop ON or OFF*/
        if (err == TM_OK)
        {
            err = tmddTDA18250GetAGC1_loop_off(tUnit, &uState);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250GetAGC1_loop_off(0x%08X) failed.", tUnit));
        }

        if (err == TM_OK)
        {
            err = TDA18250SetAGC1_loop_off(tUnit, tmTDA18250_ON);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250SetAGC1_loop_off(0x%08X) failed.", tUnit));
        }
    }

    if (err == TM_OK)
    {        
        /* smooth access */
        switch (Switch)
        {
            case tmTDA18250_RFSW_ANALOG:
                err = tmddTDA18250SetRFSW_ANALOG_MOVE(tUnit, tmddTDA18250_ON);                    
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetRFSW_ANALOG_MOVE(0x%08X) failed.", tUnit));
                break;
                
            case tmTDA18250_RFSW_LP:
                err = tmddTDA18250SetRFSW_LP_MOVE(tUnit, tmddTDA18250_ON);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetRFSW_LP_MOVE(0x%08X) failed.", tUnit));
                break;
                
            case tmTDA18250_RFSW_HP:
                err = tmddTDA18250SetRFSW_HP_MOVE(tUnit, tmddTDA18250_ON);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetRFSW_HP_MOVE(0x%08X) failed.", tUnit));
                break;
                
            case tmTDA18250_DTA_bypass:
                err = TDA18250_ERR_NOT_SUPPORTED;
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "DTA_bypass not supported in smooth mode(0x%08X) failed.", tUnit));
                break;
                
            default:
                err = ddTDA18250_ERR_NOT_SUPPORTED;
        }
        
        if (err == TM_OK)
        {
            uCpt = 0;
            RdSwitchState = (SwitchState == tmddTDA18250_SW_OPEN) ? tmddTDA18250_SW_CLOSE : tmddTDA18250_SW_OPEN;
            while ((RdSwitchState != SwitchState) && uCpt<5)
            {
                /* wait Time constant set in custom file */
                if (err == TM_OK)
                {
                    err = TDA18250Wait(pObj, uTDA18250TimeTransition[pObj->Switch_TimeCst]);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250Wait(0x%08X) failed.", tUnit));
                }

                if (err == TM_OK)
                {
                    /* smooth access */
                    switch (Switch)
                    {
                        case tmTDA18250_RFSW_ANALOG:
                            err = tmddTDA18250GetRFSW_ANALOG(tUnit, &RdSwitchState);
                            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250GetRFSW_ANALOG(0x%08X) failed.", tUnit));
                            break;
                            
                        case tmTDA18250_RFSW_LP:
                            err = tmddTDA18250GetRFSW_LP(tUnit, &RdSwitchState);
                            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250GetRFSW_LP(0x%08X) failed.", tUnit));
                            break;
                            
                        case tmTDA18250_RFSW_HP:
                            err = tmddTDA18250GetRFSW_HP(tUnit, &RdSwitchState);
                            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250GetRFSW_HP(0x%08X) failed.", tUnit));
                            break;
                            
                        case tmTDA18250_DTA_bypass:
                        default:
                            err = TDA18250_ERR_NOT_SUPPORTED;
                            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "DTA_bypass Not supported(0x%08X) failed.", tUnit));
                            break;
                    }
                }
                uCpt++;
            }

        }

        /* Put back loop off at its original value  */
        if ((err == TM_OK)&&(uState==tmTDA18250_OFF))
        {
            err = TDA18250SetAGC1_loop_off(tUnit, tmTDA18250_OFF);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250SetAGC1_loop_off(0x%08X) failed.", tUnit));
        }

        /* check switch state value readden */
        if (err == TM_OK)
        {
            if (RdSwitchState != SwitchState)
            {
                err = TDA18250_ERR_NOT_READY;
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "RdSwitchState != WrSwitchState(0x%08X) failed.", tUnit));
            }
            else
                err = TM_OK;
        }
        
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    TDA18250SetMTOOpen:                                           */
/*                                                                            */
/* DESCRIPTION: Opens the MTO.                                                */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
TDA18250SetMTOOpen
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           MTOindex    /* MTO number     */
)
{
    ptmTDA18250Object_t       pObj = Null;
    tmErrorCode_t             err  = TM_OK;

    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* MTO smooth management */
        if (pObj->MTO_TimeCst == tmddTDA18250_TCST_0)
        {
            err = TDA18250MTODirectAccess(tUnit, MTOindex, tmddTDA18250_SW_OPEN);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250MTODirectAccess(0x%08X) failed.", tUnit));
        }
        else
        {
            err = TDA18250MTOSmoothAccess(tUnit, MTOindex, tmddTDA18250_SW_OPEN);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250MTOSmoothAccess(0x%08X) failed.", tUnit));
        }
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    TDA18250SetMTOClose:                                          */
/*                                                                            */
/* DESCRIPTION: Closes the MTO.                                               */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
TDA18250SetMTOClose
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           MTOindex    /* I: MTO number  */
)
{
    ptmTDA18250Object_t       pObj = Null;
    tmErrorCode_t             err  = TM_OK;

    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* MTO smooth management */
        if (pObj->MTO_TimeCst == tmddTDA18250_TCST_0)
        {
            err = TDA18250MTODirectAccess(tUnit, MTOindex, tmddTDA18250_SW_CLOSE);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250MTODirectAccess(0x%08X) failed.", tUnit));
        }
        else
        {              
            err = TDA18250MTOSmoothAccess(tUnit, MTOindex, tmddTDA18250_SW_CLOSE);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250MTOSmoothAccess(0x%08X) failed.", tUnit));
        }
    }
    return err;
}


/*============================================================================*/
/* FUNCTION:    TDA18250MTODirectAccess:                                      */
/*                                                                            */
/* DESCRIPTION: Opens the current MTO in direct access.                       */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
TDA18250MTODirectAccess
(
    tmUnitSelect_t              tUnit,      /* I: Unit number */
    UInt8                       MTOindex,   /* I: MTO number  */
    tmddTDA18250SwitchState_t   MTOState    /* I: MTO State   */
)
{
    ptmTDA18250Object_t pObj = Null;
    tmErrorCode_t       err  = TM_OK;


    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {        
        /* direct access */
        if (err == TM_OK)
        {
            switch (MTOindex)
            {
                case 02:
                    err = tmddTDA18250SetHP_MTO2(tUnit, MTOState);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetHP_MTO2(0x%08X) failed.", tUnit));
                    break;
                    
                case 03:
                    err = tmddTDA18250SetHP_MTO3(tUnit, MTOState);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetHP_MTO3 (0x%08X) failed.", tUnit));
                    break;
                    
                default:
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "Bad MTO index (0x%08X)", MTOindex));
                    err = ddTDA18250_ERR_NOT_SUPPORTED;
            }
        }

    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    TDA18250MTOSmoothAccess:                                      */
/*                                                                            */
/* DESCRIPTION: Opens the current MTO in direct access.                       */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
TDA18250MTOSmoothAccess
(
    tmUnitSelect_t            tUnit,        /* I: Unit number */
    UInt8                     MTOindex,     /* I: MTO number  */
    tmddTDA18250SwitchState_t MTOState      /* I: MTO State   */
)
{
    ptmTDA18250Object_t       pObj = Null;
    tmErrorCode_t             err  = TM_OK;
    UInt8                     RdMTOState;
    UInt8                     uState;
    UInt8                     uCpt;

    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /*  Check smooth access */
        switch (MTOindex)
        {
            case 02:
                err = tmddTDA18250GetHP_MTO2(tUnit, &RdMTOState);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250GetHP_MTO2(0x%08X) failed.", tUnit));
                break;
                
            case 03:
                err = tmddTDA18250GetHP_MTO3(tUnit, &RdMTOState);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250GetHP_MTO3(0x%08X) failed.", tUnit));
                break;
                
            default:
                err = TDA18250_ERR_NOT_SUPPORTED;
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "Bad MTO index (0x%08X)", MTOindex));
                break;
        }

        /* check MTO state value readden */
        if (err == TM_OK)
        {
            if ((tmddTDA18250SwitchState_t)RdMTOState == MTOState)
            {
                return TM_OK;
            }
        }
    }

    if (err == TM_OK)
    {
        /*  Check smooth state machine is available */
        /* AGC1 loop ON or OFF*/
        if (err == TM_OK)
        {
            err = tmddTDA18250GetAGC1_loop_off(tUnit, &uState);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250GetAGC1_loop_off(0x%08X) failed.", tUnit));
        }

        if (err == TM_OK)
        {
            err = TDA18250SetAGC1_loop_off(tUnit, tmTDA18250_ON);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250SetAGC1_loop_off(0x%08X) failed.", tUnit));
        }
    }

    if (err == TM_OK)
    {        
       
        /* smooth access */
        switch (MTOindex)
        {
            case 02:
                err = tmddTDA18250SetHP_MTO2_MOVE(tUnit, tmddTDA18250_ON);                    
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetHP_MTO2_MOVE(0x%08X) failed.", tUnit));
                break;
                
            case 03:
                err = tmddTDA18250SetHP_MTO3_MOVE(tUnit, tmddTDA18250_ON);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetHP_MTO3_MOVE(0x%08X) failed.", tUnit));
                break;
                
            default:
                err = ddTDA18250_ERR_NOT_SUPPORTED;
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "Bad MTO index (0x%08X)", MTOindex));
        }
        
        if (err == TM_OK)
        {
            uCpt = 0;
            RdMTOState = (MTOState == tmddTDA18250_SW_OPEN) ? tmddTDA18250_SW_CLOSE : tmddTDA18250_SW_OPEN;
            while (((tmddTDA18250SwitchState_t)RdMTOState != MTOState) && uCpt<5)
            {
                /* wait Time constant set in custom file */
                if (err == TM_OK)
                {
                    err = TDA18250Wait(pObj, uTDA18250TimeTransition[pObj->MTO_TimeCst]);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250Wait(0x%08X) failed.", tUnit));
                }

                if (err == TM_OK)
                {
                    /* smooth access */
                    switch (MTOindex)
                    {
                        case 02:
                            err = tmddTDA18250GetHP_MTO2(tUnit, &RdMTOState);
                            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250GetRFSW_ANALOG(0x%08X) failed.", tUnit));
                            break;
                            
                        case 03:
                            err = tmddTDA18250GetHP_MTO3(tUnit, &RdMTOState);
                            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250GetRFSW_LP(0x%08X) failed.", tUnit));
                            break;
                            
                        default:
                            err = TDA18250_ERR_NOT_SUPPORTED;
                            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "Bad MTO index (0x%08X)", MTOindex));
                            break;
                    }
                }
                uCpt++;
            }
        }

        /* Put back loop off at its original value  */
        if ((err == TM_OK)&&(uState==tmTDA18250_OFF))
        {
            err = TDA18250SetAGC1_loop_off(tUnit, tmTDA18250_OFF);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250SetAGC1_loop_off(0x%08X) failed.", tUnit));
        }

        /* check MTO state value readden */
        if (err == TM_OK)
        {
            if ((tmddTDA18250SwitchState_t)RdMTOState != MTOState)
            {
                err = TDA18250_ERR_NOT_READY;
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "RdMTOState != WrMTOState(0x%08X) failed.", tUnit));
            }
            else
                err = TM_OK;
        }
        
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    TDA18250SetDigitalPath:                                       */
/*                                                                            */
/* DESCRIPTION: Digital Path configuration.                                   */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       pObj->BandIndex = previous index in this step                 */
/*              pObj->BandIndex will replaced by NewBandIndex at the end of   */
/*              the path setting                                              */
/*============================================================================*/
tmErrorCode_t
TDA18250SetDigitalPath
(
    tmUnitSelect_t        tUnit,        /* I: Unit number */
    tmTDA18250BandIndex_t NewBandIndex  /* I: Band Index  */
)
{   
    ptmTDA18250Object_t   pObj = Null;
    tmErrorCode_t         err = TM_OK;
    Bool                  bCloseSwitchCdt = 0;

    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {

        if (err == TM_OK)
        {
            /* AGC2_TOP writting only if BP_Filter changed or after Reset */
            if ( (pObj->BandParam[NewBandIndex].BP_Filter != pObj->BandParam[pObj->BandIndex].BP_Filter)
                 ||
                 (pObj->ResetCalledPreviously == True)
               )
            {
                err = TDA18250WriteAGC2_TOP(tUnit, pObj->BandParam[NewBandIndex].BP_Filter);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250WriteAGC2_TOP(0x%08X) failed.", tUnit));
            }
        }
        if (err == TM_OK)
        {
            /* stop AGC1 loop */
            err = TDA18250SetAGC1_loop_off(tUnit, tmTDA18250_ON);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250SetAGC1_loop_off(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            /* AGC2 frozen */
            err = tmddTDA18250SetAGC2_gain_Force(tUnit, tmddTDA18250_ON);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC2_gain_Force(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            /* AGC2 management if current path is tmTDA18250_DTA_bypass */
            if (pObj->BandParam[pObj->BandIndex].Switch == tmTDA18250_DTA_bypass)
            {
                err = TDA18250WriteAGC2To0dB(tUnit, tmTDA18250_AGC2Step3dB_ON);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250WriteAGC2To0dB(0x%08X) failed.", tUnit));
            }
        }
        if (err == TM_OK)
        {
            /* Open switch to prevent interference regarding condition */
            if (
                 (pObj->BandParam[NewBandIndex].Switch != tmTDA18250_DTA_bypass)
                  ||
                 ( 
                    (pObj->BandParam[NewBandIndex].Switch == tmTDA18250_DTA_bypass)
                    &&
                    (pObj->BandParam[pObj->BandIndex].Switch != tmTDA18250_DTA_bypass)
                 )
               )
            {
                err = TDA18250SetSwitchOpen(tUnit);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250SetSwitchOpen(0x%08X) failed.", tUnit));

                if (err == TM_OK)
                {
                        bCloseSwitchCdt = 1;
                }
            }
        }
        if (err == TM_OK)
        {
            /* Blocks path updated after Reset */
            if (pObj->ResetCalledPreviously == True)
            {
                err = TDA18250SetBlocksPath(tUnit, tmTDA18250DigitalChannel);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250SetBlocksPath(0x%08X) failed.", tUnit));
            }
        }
        if (err == TM_OK)
        {
            /* Update RF Filter path value */
            err = TDA18250WriteRF_Filters(tUnit, NewBandIndex);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250WriteRF_Filters(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            /* AGC2 management */
            err = TDA18250WriteAGC2To0dB(tUnit, tmTDA18250_AGC2Step3dB_OFF);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250WriteAGC2To0dB(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            /* Close new switch previously openned regarding condition */
            if (bCloseSwitchCdt == 1)
            {
                err = TDA18250SetSwitchClose(tUnit, NewBandIndex);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250SetSwitchClose(0x%08X) failed.", tUnit));
            }
        }
        if (err == TM_OK)
        {
            /* start AGC1 loop */
            err = TDA18250SetAGC1_loop_off(tUnit, tmTDA18250_OFF);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250SetAGC1_loop_off(0x%08X) failed.", tUnit));
        }
                

        if (err == TM_OK)
        {
            /* AGC2 free */
            err = tmddTDA18250SetAGC2_gain_Force(tUnit, tmddTDA18250_OFF);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC2_gain_Force(0x%08X) failed.", tUnit));
        }

        if (err == TM_OK)
        {
            TDA18250SetAGC2_Bypass(tUnit);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250SetAGC2_Bypass(0x%08X) failed.", tUnit));
        }

        if (err == TM_OK)
        {
            /* Band Index updated */
            pObj->BandIndex = NewBandIndex;
        }
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    TDA18250RF2BandIndex:                                         */
/*                                                                            */
/* DESCRIPTION: converts RF to Band index.                                    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
TDA18250RF2BandIndex
(
    tmUnitSelect_t        tUnit,       /* I: Unit number           */
    UInt32                uRF,         /* I: RF frequency in hertz */
    tmTDA18250BandIndex_t *pBandIndex  /* O: Band Index            */
)
{
    ptmTDA18250Object_t   pObj = Null;
    tmErrorCode_t         err  = TM_OK;
    tmTDA18250BandIndex_t uNewBandIndex = tmTDA18250_IND_Band_Max;

    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        uNewBandIndex = tmTDA18250_IND_DIG_Band_1;
        
        /* Search the current Hybrid Band used */
        while ( (uRF > pObj->BandParam[uNewBandIndex].RfMax) && (uNewBandIndex < tmTDA18250_IND_Band_Max) )
            uNewBandIndex = (tmTDA18250BandIndex_t)(uNewBandIndex + 1);

        *pBandIndex = uNewBandIndex;
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    TDA18250WriteAGC2ToxdB:                                       */
/*                                                                            */
/* DESCRIPTION: Increases AGC2.                                               */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
TDA18250WriteAGC2ToxdB
(
    tmUnitSelect_t          tUnit,        /* I: Unit number */
    UInt8                   AGC2_Gain     /* I: Gain        */
)
{
    ptmTDA18250Object_t   pObj = Null;
    tmErrorCode_t         err  = TM_OK;
    UInt8                 uAGC2_Gain;

    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Increase AGC2 by step of 3dB */
        err = tmddTDA18250GetAGC2b_Gain(tUnit, &uAGC2_Gain);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC2b_Gain(0x%08X) failed.", tUnit));

        while ( (uAGC2_Gain < AGC2_Gain) && (err == TM_OK))
        {
            if (AGC2_Gain - uAGC2_Gain > 15) 
            {
                uAGC2_Gain += 15;
            }
            else
            {
                uAGC2_Gain = AGC2_Gain;
            }

            err = tmddTDA18250SetAGC2b_Gain(tUnit, uAGC2_Gain);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC2b_Gain(0x%08X) failed.", tUnit));
            
            if (err == TM_OK)
            {
                err = TDA18250Wait(pObj, 0);  /* wait */
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250Wait(0x%08X) failed.", tUnit));
            }
        }
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    TDA18250WriteAGC2To0dB:                                       */
/*                                                                            */
/* DESCRIPTION: Decreases AGC2 to 0dB if positive.                            */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
TDA18250WriteAGC2To0dB
(
    tmUnitSelect_t          tUnit,        /* I: Unit number        */
    tmTDA18250_AGC2Step3dB  eAGC2Step3dB  /* I: Switch Smooth used */
)
{
    ptmTDA18250Object_t   pObj = Null;
    tmErrorCode_t         err  = TM_OK;
    UInt8                 uAGC2_Gain;
    UInt8                 uStepCpt;

    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        if (eAGC2Step3dB == tmTDA18250_AGC2Step3dB_ON)
        {
            err = tmddTDA18250GetAGC2b_Gain(tUnit, &uAGC2_Gain);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250GetAGC2b_Gain(0x%08X) failed.", tUnit));

            /* Decrease AGC2 by step of 3dB */
            uStepCpt = 0;
            while ( (uAGC2_Gain > 60) && (err == TM_OK) )
            {
                uAGC2_Gain -= 15;
                err = tmddTDA18250SetAGC2b_Gain(tUnit, uAGC2_Gain);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC2b_Gain(0x%08X) failed.", tUnit));

                if (err == TM_OK)
                {
                    err = TDA18250Wait(pObj, 0);  /* wait */
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250Wait(0x%08X) failed.", tUnit));
                }
                if (err == TM_OK)
                {
                    uStepCpt++;       
                }
            }
        }
        
        if (eAGC2Step3dB == tmTDA18250_AGC2Step3dB_OFF)
        {
            /* Set AGC2 directly to 0dB */
            err = tmddTDA18250SetAGC2b_Gain(tUnit, 60);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC2b_Gain(0x%08X) failed.", tUnit));
        }
                
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    TDA18250WriteRF_Filters:                                      */
/*                                                                            */
/* DESCRIPTION: Writes specific Digital data regarding the Band Index.        */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       writting data done only if previous different from current    */
/*============================================================================*/
tmErrorCode_t
TDA18250WriteRF_Filters
(
    tmUnitSelect_t         tUnit,         /* I: Unit number */
    tmTDA18250BandIndex_t  uNewBandIndex  /* I: Band Index  */
)
{
    ptmTDA18250Object_t   pObj = Null;
    tmErrorCode_t         err  = TM_OK;

    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Set the data only only if previous different from current */
        if ( (pObj->BandParam[uNewBandIndex].Ind_Div2 != pObj->BandParam[pObj->BandIndex].Ind_Div2)
              || (pObj->ResetCalledPreviously == True) )
        {
            err = tmddTDA18250SetInd_Div2(tUnit, pObj->BandParam[uNewBandIndex].Ind_Div2);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetInd_Div2(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            if ( (pObj->BandParam[uNewBandIndex].Cap_HP_Left != pObj->BandParam[pObj->BandIndex].Cap_HP_Left)
              || (pObj->ResetCalledPreviously == True) )
            {
                err = tmddTDA18250SetCap_HP_Left(tUnit, pObj->BandParam[uNewBandIndex].Cap_HP_Left);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetCap_HP_Left(0x%08X) failed.", tUnit));
            }
        }
        if (err == TM_OK)
        {
            if ( (pObj->BandParam[uNewBandIndex].Cap_HP_Mid != pObj->BandParam[pObj->BandIndex].Cap_HP_Mid)
              || (pObj->ResetCalledPreviously == True) )
            {
                err = tmddTDA18250SetCap_HP_Mid(tUnit, pObj->BandParam[uNewBandIndex].Cap_HP_Mid);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetCap_HP_Mid(0x%08X) failed.", tUnit));
            }
        }
        if (err == TM_OK)
        {
            if ( (pObj->BandParam[uNewBandIndex].Cap_HP_Right != pObj->BandParam[pObj->BandIndex].Cap_HP_Right)
              || (pObj->ResetCalledPreviously == True) )
            {
                err = tmddTDA18250SetCap_HP_Right(tUnit, pObj->BandParam[uNewBandIndex].Cap_HP_Right);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetCap_HP_Right(0x%08X) failed.", tUnit));
            }
        }
        if (err == TM_OK)
        {
            if ( (pObj->BandParam[uNewBandIndex].BP_Ind1 != pObj->BandParam[pObj->BandIndex].BP_Ind1)
              || (pObj->ResetCalledPreviously == True) )
            {
                err = tmddTDA18250SetBP_Ind1(tUnit, pObj->BandParam[uNewBandIndex].BP_Ind1);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetBP_Ind1(0x%08X) failed.", tUnit));
            }
        }
        if (err == TM_OK)
        {
            if ( (pObj->BandParam[uNewBandIndex].BP_Ind2 != pObj->BandParam[pObj->BandIndex].BP_Ind2)
              || (pObj->ResetCalledPreviously == True) )
            {
                err = tmddTDA18250SetBP_Ind2(tUnit, pObj->BandParam[uNewBandIndex].BP_Ind2);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetBP_Ind2(0x%08X) failed.", tUnit));
            }
        }
        if (err == TM_OK)
        {
            if ( (pObj->BandParam[uNewBandIndex].Cap_L6n != pObj->BandParam[pObj->BandIndex].Cap_L6n)
              || (pObj->ResetCalledPreviously == True) )
            {
                err = tmddTDA18250SetCap_L6n(tUnit, pObj->BandParam[uNewBandIndex].Cap_L6n);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetCap_L6n(0x%08X) failed.", tUnit));
            }
        }
        if (err == TM_OK)
        {
            if ( (pObj->BandParam[uNewBandIndex].Cap_L10n != pObj->BandParam[pObj->BandIndex].Cap_L10n)
              || (pObj->ResetCalledPreviously == True) )
            {
                err = tmddTDA18250SetCap_L10n(tUnit, pObj->BandParam[uNewBandIndex].Cap_L10n);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetCap_L10n(0x%08X) failed.", tUnit));
            }
        }
        if (err == TM_OK)
        {
            if ( (pObj->BandParam[uNewBandIndex].DTA_notMOCA != pObj->BandParam[pObj->BandIndex].DTA_notMOCA)
              || (pObj->ResetCalledPreviously == True) )
            {
                err = tmddTDA18250SetDTA_NotMOCA(tUnit, pObj->BandParam[uNewBandIndex].DTA_notMOCA);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetDTA_NotMOCA(0x%08X) failed.", tUnit));
            }
        }
        if (err == TM_OK)
        {
            if ( (pObj->BandParam[uNewBandIndex].pd_AGC1bis != pObj->BandParam[pObj->BandIndex].pd_AGC1bis)
              || (pObj->ResetCalledPreviously == True) )
            {
                err = tmddTDA18250Setpd_AGC1bis(tUnit, pObj->BandParam[uNewBandIndex].pd_AGC1bis);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250Setpd_AGC1bis(0x%08X) failed.", tUnit));
            }
        }
        if (err == TM_OK)
        {
            if ( (pObj->BandParam[uNewBandIndex].AGC1bis_gain != pObj->BandParam[pObj->BandIndex].AGC1bis_gain)
              || (pObj->ResetCalledPreviously == True) )
            {
                err = tmddTDA18250SetAgc1bis_gain(tUnit, pObj->BandParam[uNewBandIndex].AGC1bis_gain);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAgc1bis_gain(0x%08X) failed.", tUnit));
            }
        }
        if (err == TM_OK)
        {
            if ( (pObj->BandParam[uNewBandIndex].BP_Filter != pObj->BandParam[pObj->BandIndex].BP_Filter)
              || (pObj->ResetCalledPreviously == True) )
            {
                err = tmddTDA18250SetBP_Filter(tUnit, pObj->BandParam[uNewBandIndex].BP_Filter);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetBP_Filter(0x%08X) failed.", tUnit));
            }
        }
        if (err == TM_OK)
        {
            if ( (pObj->BandParam[uNewBandIndex].Stob_Gain != pObj->BandParam[pObj->BandIndex].Stob_Gain)
                 || (pObj->ResetCalledPreviously == True) )
            {
                err = tmddTDA18250SetSTOB_gain(tUnit, pObj->BandParam[uNewBandIndex].Stob_Gain);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetSTOB_gain(0x%08X) failed.", tUnit));
            }
        }
        if (err == TM_OK)
        {
            if ( (pObj->BandParam[uNewBandIndex].Stob_DC != pObj->BandParam[pObj->BandIndex].Stob_DC)
                 || (pObj->ResetCalledPreviously == True) )
            {
                err = tmddTDA18250SetSTOB_DC(tUnit, pObj->BandParam[uNewBandIndex].Stob_DC);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetSTOB_DC(0x%08X) failed.", tUnit));
            }
        }
        if (err == TM_OK)
        {
            if ( (pObj->BandParam[uNewBandIndex].PSM_Stob != pObj->BandParam[pObj->BandIndex].PSM_Stob)
                 || (pObj->ResetCalledPreviously == True) )
            {
                err = tmddTDA18250SetPSM_StoB(tUnit, pObj->BandParam[uNewBandIndex].PSM_Stob);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetPSM_StoB(0x%08X) failed.", tUnit));
            }
        }
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    TDA18250SetStandardMode:                                      */
/*                                                                            */
/* DESCRIPTION: Sets the standard mode.                                       */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       called before SetRF                                           */
/*============================================================================*/
tmErrorCode_t
TDA18250SetStandardMode
(
    tmUnitSelect_t tUnit    /* I: Unit number */
)
{
    ptmTDA18250Object_t pObj = Null;
    tmErrorCode_t       err  = TM_OK;


    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        if (err == TM_OK)
        {
            /* Switch off the entire filter switch */
            err = tmddTDA18250SetRFcal_modeOFF(tUnit, pObj->Std_Array[pObj->StandardMode].RFcal_modeOFF);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetRFcal_modeOFF(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = tmddTDA18250SetPD_hpf(tUnit, pObj->Std_Array[pObj->StandardMode].PD_hpf);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetPD_hpf(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = tmddTDA18250Sethpf_freq(tUnit, (tmddTDA18250HPF_Freq_t)pObj->Std_Array[pObj->StandardMode].IF_hpf);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250Sethpf_freq(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = tmddTDA18250Setnotch_select(tUnit, pObj->Std_Array[pObj->StandardMode].notch_select);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250Setnotch_select(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = tmddTDA18250SetLP_Fc(tUnit, pObj->Std_Array[pObj->StandardMode].LP_Fc);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetLP_Fc(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = tmddTDA18250SetRC_LPF_offset(tUnit, pObj->Std_Array[pObj->StandardMode].RC_LPFC_Offset);
        }
        if (err == TM_OK)
        {
            err = tmddTDA18250SetRC_LPF_sign(tUnit, pObj->Std_Array[pObj->StandardMode].RC_LPFC_Sign);
        }
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    TDA18250SetRFAGC_Top:                                         */
/*                                                                            */
/* DESCRIPTION: writes RFAGC_Top.                                             */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
TDA18250SetRFAGC_Top
(
    tmUnitSelect_t  tUnit   /* I: Unit number  */
)
{
    ptmTDA18250Object_t pObj = Null;
    tmErrorCode_t       err  = TM_OK;
    UInt8               uRFAGC_TopIndex;

    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Search the current RFAGC_Top Band used */
        uRFAGC_TopIndex = 0;
        while ( (pObj->uRF > pObj->RFAGC_Top[uRFAGC_TopIndex].RfMax) )
        {
            uRFAGC_TopIndex = (tmTDA18250BandIndex_t)(uRFAGC_TopIndex + 1);
            if(uRFAGC_TopIndex >= tmTDA18250_IND_RFAGC_Top_Max)
            {
                break;
            }
        }


        if (uRFAGC_TopIndex == 6)
        {
            err = TM_ERR_BAD_PARAMETER;
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "RF value parameter (0x%08X) failed.", tUnit));
        }
        else
        {
            err = tmddTDA18250SetRFAGC_Top(tUnit, pObj->RFAGC_Top[uRFAGC_TopIndex].uRFAGC_Top);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetRFAGC_Top(0x%08X) failed.", tUnit));
        }
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    TDA18250SetBlocksPath:                                        */
/*                                                                            */
/* DESCRIPTION: Activates or not the blocks regarding the channel type.       */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       called in SetRF if channel type changed after switch setting  */
/*============================================================================*/
tmErrorCode_t
TDA18250SetBlocksPath
(
    tmUnitSelect_t          tUnit,       /* I: Unit number  */
    tmTDA18250ChannelType_t ChannelType  /* I: Channel type */
)
{
    ptmTDA18250Object_t    pObj = Null;
    tmErrorCode_t          err  = TM_OK;


    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Set Power down of the continuous AGC */
        err = tmddTDA18250SetpdAGC2a(tUnit, tmTDA18250BlocksPath[ChannelType].pdAGC2a);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetpdAGC2a(0x%08X) failed.", tUnit));
 
        if (err == TM_OK)
        {
            /* Set AGC2b Power down */
            err = tmddTDA18250SetpdAGC2b(tUnit, tmTDA18250BlocksPath[ChannelType].pdAGC2b);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetpdAGC2b(0x%08X) failed.", tUnit));
        }
 
        if (err == TM_OK)
        {
            /* Set PD_RFAGC_Det Power down */
            err = tmddTDA18250SetPD_RFAGC_Det(tUnit, tmTDA18250BlocksPath[ChannelType].PD_RFAGC_Det);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetPD_RFAGC_Det(0x%08X) failed.", tUnit));
        }
 
        if (err == TM_OK)
        {
            /* Set pdAGC2_DETECT2 Power down */
            err = tmddTDA18250SetpdAGC2_DETECT2(tUnit, tmTDA18250BlocksPath[ChannelType].pdAGC2_DETECT2);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetpdAGC2_DETECT2(0x%08X) failed.", tUnit));
        }
 
        if (err == TM_OK)
        {
            /* Set pdAGC2_DETECT3 Power down */
            err = tmddTDA18250SetpdAGC2_DETECT3(tUnit, tmTDA18250BlocksPath[ChannelType].pdAGC2_DETECT3);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetpdAGC2_DETECT3(0x%08X) failed.", tUnit));
        }
  
        if (err == TM_OK)
        {
            /* Set AGC2b Power down */
            err = tmddTDA18250SetpdAGC2_DETECT4(tUnit, tmTDA18250BlocksPath[ChannelType].pdAGC2_DETECT4);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetpdAGC2_DETECT4(0x%08X) failed.", tUnit));
        }
 
        if (err == TM_OK)
        {
            /* Set the Power down circuit from the AGC2 output to the IF */
            err = tmddTDA18250SetPD_RFAGC_Ifout(tUnit, tmTDA18250BlocksPath[ChannelType].PD_RFAGC_Ifout);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetPD_RFAGC_Ifout(0x%08X) failed.", tUnit));
        }
 
        if (err == TM_OK)
        {
            err = tmddTDA18250SetBUF1b_DCloopIH(tUnit, tmTDA18250BlocksPath[ChannelType].BUF1b_DCloopIH);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetBUF1b_DCloopIH(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = tmddTDA18250SetPDBUF1b(tUnit, tmTDA18250BlocksPath[ChannelType].PDBUF1b);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetPDBUF1b(0x%08X) failed.", tUnit));
        }
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    TDA18250GetRSSI:                                              */
/*                                                                            */
/* DESCRIPTION: Launch and read the RSSI.                                     */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
TDA18250GetRSSI
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8*          pRSSI       /* O: RSSI value  */
)
{   
    ptmTDA18250Object_t pObj = Null;
    tmErrorCode_t       err = TM_OK;
    UInt8               bTempRSSI_byte = 0;
    UInt8               FlagMSM = 0;
    UInt8               FlagRSSI_Alarm = 0;

    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));
    
    if (err == TM_OK)
    {
        *pRSSI = 0;

        /* Store RSSI_byte */
        err = tmddTDA18250GetRSSI_Byte(tUnit, &bTempRSSI_byte);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250GetRSSI_Byte(0x%08X) failed.", tUnit));

        if (err == TM_OK)
        {
            err = tmddTDA18250SetRSSI_Byte(tUnit, 0xC9);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetRSSI_Byte(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = tmddTDA18250SetResetFlags(tUnit);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetResetFlags(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = tmddTDA18250Sethc_mode(tUnit, tmddTDA18250_HcModeRSSI);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250Sethc_mode(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = tmddTDA18250SetLaunchMSM(tUnit);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetLaunchMSM(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = TDA18250Wait(pObj, 15);  /* wait 15ms */
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250Wait(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = tmddTDA18250Sethc_mode(tUnit, tmddTDA18250_HcModeNormal);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250Sethc_mode(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = tmddTDA18250SetRSSI_Byte(tUnit, bTempRSSI_byte);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetRSSI_Byte(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            err = tmddTDA18250GetFlagMSM_OK(tUnit, &FlagMSM);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250GetFlagMSM_OK(0x%08X) failed.", tUnit));

            /* Flag MSM test */
            if (FlagMSM != 1)
            {
                err = TDA18250_ERR_HW_FAILED;
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetRSSI: FlagMSM_OK(0x%08X) failed.", tUnit));
            }
        }
        if (err == TM_OK)
        {
            err = tmddTDA18250GetRSSI_Alarm(tUnit, &FlagRSSI_Alarm);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250GetRSSI_Alarm(0x%08X) failed.", tUnit));

            /* Flag MSM test */
            if (FlagRSSI_Alarm == 1)
            {
                err = TDA18250_ERR_HW_FAILED;
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "FlagRSSI_Alarm(0x%08X) failed.", tUnit));
            }
        }
        if (err == TM_OK)
        {
            err = tmddTDA18250GetRSSI(tUnit, pRSSI);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250GetRSSI(0x%08X) failed.", tUnit));
        }
        else
        {
            *pRSSI = 0;
        }
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    TDA18250CalcPow:                                              */
/*                                                                            */
/* DESCRIPTION: pow calculation.                                              */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
Int32
TDA18250CalcPow
(
    Int32 uX,     /* I: X value  */
    Int32 uY      /* I: Y value  */
)
{
    UInt8 cpt;
    Int32 dVal;

    dVal = 1;
    for (cpt = 0; cpt<uY; cpt++)
        dVal *= uX;
    
    return dVal;
}


/*============================================================================*/
/* FUNCTION:    TDA18250LvlAgc2Contribution:                                  */
/*                                                                            */
/* DESCRIPTION: AGC1 Contribution for TDA18250GetPower_Level().               */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
TDA18250LvlAgc2Contribution
(
    UInt8       uAGC2b,         /* I: AGC2 gain          */
    UInt32      uRfFreq,        /* I: RF frequency (MHz) */
    Int32*      pPowerLevel     /* I/O: PowerLevel       */
)
{
    tmErrorCode_t   err = TM_OK;
    Int32           AGC2b = 0;
    UInt32          uIndex = 0;
    
    /* Search the current AGC2b index */
    while ( (uAGC2b > Lvl_AGC2b[uIndex][tmTDA18250_AGC2b_Code]) )
    {
        uIndex = uIndex + 1;
        if (uIndex == tmTDA18250_Lvl_AGC2b_Index_Max)
        {
            break;
        }
    }

    if (uIndex == tmTDA18250_Lvl_AGC2b_Index_Max)
    {
        err = TDA18250_ERR_BAD_UNIT_NUMBER;
    }

    if (err == TM_OK)
    {
        /* only one division for the intermediate sum */
        AGC2b = Lvl_AGC2b[uIndex][tmTDA18250_AGC2b_X0];
        AGC2b += uAGC2b * Lvl_AGC2b[uIndex][tmTDA18250_AGC2b_X1];
        AGC2b /= TDA18250CalcPow(10, Lvl_Div10[tmTDA18250_XDIV10_AGC2][tmTDA18250_XDIV10_X0] );
        
        AGC2b += ( uRfFreq * Lvl_AGC2b[uIndex][tmTDA18250_AGC2b_X2] ) / ( TDA18250CalcPow(10, Lvl_Div10[tmTDA18250_XDIV10_AGC2][tmTDA18250_XDIV10_X2]) );

        *pPowerLevel -= AGC2b / (  TDA18250CalcPow(10, Lvl_Div10[tmTDA18250_XDIV10_AGC2][tmTDA18250_XDIV10_SUM]) );
    }
            
    return err;
}


/*============================================================================*/
/* FUNCTION:    TDA18250LvlAgc1Contribution:                                  */
/*                                                                            */
/* DESCRIPTION: AGC1 Contribution for TDA18250GetPower_Level().               */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
TDA18250LvlAgc1Contribution
(
    tmddTDA18250_AGC1_Gain_t uAGC1,         /* I: AGC1 gain          */
    UInt32                   uRF,           /* I: RF frequency (MHz) */
    Int32*                   pPowerLevel    /* I/O: PowerLevel       */
)
{
    tmErrorCode_t   err = TM_OK;
    Int32           Agc1 = 0;
    UInt32          uIndex = 0;
    UInt8           uCpt = 0;
    
    while ( (uAGC1 > Lvl_AGC1[uIndex][tmTDA18250_AGC1_Code]) && (uIndex < tmTDA18250_Lvl_AGC1_Index_Max) )
    {
        uIndex = uIndex + 1;
        if (uIndex == tmTDA18250_Lvl_AGC1_Index_Max)
        {
            break;
        }
    }

    if (uIndex == tmTDA18250_Lvl_AGC1_Index_Max)
    {
        err = TDA18250_ERR_BAD_UNIT_NUMBER;
    }

    if (err == TM_OK)
    {
        for (uCpt=tmTDA18250_AGC1_X0; uCpt<=tmTDA18250_AGC1_X2; uCpt++)
        {
           Agc1 += ( TDA18250CalcPow(uRF, uCpt) * Lvl_AGC1[uIndex][uCpt] ) / ( TDA18250CalcPow(10, Lvl_Div10[tmTDA18250_XDIV10_AGC1][uCpt]) );
        }
        
        *pPowerLevel -= Agc1 / ( TDA18250CalcPow(10, Lvl_Div10[tmTDA18250_XDIV10_AGC1][tmTDA18250_XDIV10_SUM]) );
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    TDA18250LvlAgc1MasterMultiContribution:                       */
/*                                                                            */
/* DESCRIPTION: AGC1 Contribution for TDA18250GetPower_Level() for Master     */
/*              in Multi solution.                                            */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
TDA18250LvlAgc1MasterMultiContribution
(
    tmddTDA18250_AGC1_Gain_t uAGC1,         /* I: AGC1 gain          */
    UInt32                   uRF,           /* I: RF frequency (MHz) */
    Int32*                   pPowerLevel    /* I/O: PowerLevel       */
)
{
    tmErrorCode_t   err  = TM_OK;
    Int32           Agc1Multi = 0;
    UInt32          uIndex = 0;
    UInt8           uCpt = 0;
    
    while ( (uAGC1 > Lvl_AGC1_MasterMulti[uIndex][tmTDA18250_AGC1_Code]) && (uIndex < tmTDA18250_Lvl_AGC1_Index_Max) )
    {
        uIndex = uIndex + 1;
        if (uIndex == tmTDA18250_Lvl_AGC1_Index_Max)
        {
            break;
        }
    }

    if (uIndex == tmTDA18250_Lvl_AGC1_Index_Max)
    {
        err = TDA18250_ERR_BAD_UNIT_NUMBER;
    }

    if (err == TM_OK)
    {
        for (uCpt=tmTDA18250_AGC1_X0; uCpt<=tmTDA18250_AGC1_X2; uCpt++)
        {
            Agc1Multi += ( TDA18250CalcPow(uRF, uCpt) * Lvl_AGC1_MasterMulti[uIndex][uCpt] ) / ( TDA18250CalcPow(10, Lvl_Div10[tmTDA18250_XDIV10_AGC1Multi][uCpt]) );
        }
        
        *pPowerLevel -= Agc1Multi / ( TDA18250CalcPow(10, Lvl_Div10[tmTDA18250_XDIV10_AGC1Multi][tmTDA18250_XDIV10_SUM]) );
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    TDA18250GetPower_Level                                        */
/*                                                                            */
/* DESCRIPTION: Calculate the RF input level in dBmV.                         */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
TDA18250GetPower_Level
(
   tmUnitSelect_t  tUnit,        /* I: Unit number                                  */
   UInt32          IF,           /* I: Intermediate Frequency                       */
   UInt8           uRSSIValue,   /* I: RSSI value reading previously                */
   Int32*          pRFEVAL       /* O: Address of the variable to output item value */
)
{
    ptmTDA18250Object_t      pObj = Null;
    tmErrorCode_t            err  = TM_OK;
    tmddTDA18250_AGC1_Gain_t uAGC1_Current = tmddTDA18250_AGC1_Gain_Fixed_Minus2dB;
    tmddTDA18250_AGC1_Gain_t uAGC1_Master = tmddTDA18250_AGC1_Gain_Fixed_Minus2dB;
    tmTDA18250_AGC1_Type_t   AGC1_Type = tmTDA18250_AGC1_Slave;
    UInt32                   uLO = 0;
    tmTDA18250BandIndex_t    uIndex = tmTDA18250_IND_DIG_Band_1;
    UInt8                    uAGC2b = 0;
    UInt8                    uCpt = 0;
    UInt8                    uLT_RFin = 0;
    UInt32                   uRfFreq = 0; 
    Int32                    PowerLevel = (Int32)uRSSIValue;
    Int32                    TempPowerLevel = 0;

    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));
    
    if (err == TM_OK)
    {

        err = tmddTDA18250GetLOFreq(tUnit,&uLO);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250GetLOFreq(0x%08X) failed.", tUnit));

        uRfFreq = uLO - IF;

        /* Band recovery */
        if (err == TM_OK)
        {
            /* Search the new Hybrid Band used */
            err = TDA18250RF2BandIndex(tUnit, uRfFreq, &uIndex);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250RF2BandIndex(0x%08X) failed.", tUnit));
        }

        /* RF frequency in MHz for calculation */
        uRfFreq /= 1000000;

        if (err == TM_OK)
        {
            if (PowerLevel > 203)
                err = TM_ERR_BAD_PARAMETER;
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "RSSI value parameter(0x%08X) failed.", tUnit));
        }
        
        /* if Master
                Function_rssi = (rssi_code * a + b) - (RFfilter + agc2 + agc1m)
           else
                Function_rssi = (rssi_code * a + b) - (RFfilter + agc2 + agc1s + agc1m + offset_slave)
        */
        
        /* ----- RSSI contribution ----- */
        if (err == TM_OK)
        {
            PowerLevel = Lvl_RSSI[tmTDA18250_RSSI_X0] + Lvl_RSSI[tmTDA18250_RSSI_X1] * uRSSIValue;
        }
        
        /* ----- RfFilter contribution ----- */
        if (err == TM_OK)
        {
            for (uCpt=tmTDA18250_RfFilter_X0; uCpt<=tmTDA18250_RfFilter_X2; uCpt++)
            {
                TempPowerLevel += TDA18250CalcPow(uRfFreq, uCpt) * Lvl_RfFilter[uIndex][uCpt] / ( TDA18250CalcPow(10, Lvl_Div10[tmTDA18250_DIV10_RF][uCpt]) );
            }
            PowerLevel -= TempPowerLevel / ( TDA18250CalcPow(10, Lvl_Div10[tmTDA18250_DIV10_RF][tmTDA18250_XDIV10_SUM]) );
        }

        /* ----- AGC2b contribution ----- */
        if (err == TM_OK)
        {
            err = tmddTDA18250GetAGC2b_Gain(tUnit, &uAGC2b);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250GetAGC2b_Gain(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            err = TDA18250LvlAgc2Contribution(uAGC2b, uRfFreq, &PowerLevel);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250LvlAgc2Contribution(0x%08X) failed.", tUnit));
        }

        /* ----- AGC1 contribution ----- */
        if (err == TM_OK)
        {
            err = tmddTDA18250_GetAGC1_Gain(tUnit, &uAGC1_Current);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250_GetAGC1_Gain(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* Master/Slave detection  */
            if ( pObj->deviceType < tmTDA18250_DeviceTypeSlave )
            {
                /* Master Single/Multi configuration detection */
                err = tmddTDA18250GetRFSW_MTO_LT_RFin(tUnit, &uLT_RFin);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250GetRFSW_MTO_LT_RFin(0x%08X) failed.", tUnit));
                
                if (err == TM_OK)
                {
                    if (uLT_RFin)
                    {
                        AGC1_Type = tmTDA18250_AGC1_MasterMulti;
                    }
                    else
                    {
                        AGC1_Type = tmTDA18250_AGC1_MasterSingle;
                    }
                }
            }
            else
            {
                /* Slave configuration need the AGC1 Master contribution */
                err = tmddTDA18250_GetAGC1_Gain((tUnit&0xF0)+TDA18250_MASTER_UNIT, &uAGC1_Master);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250_GetAGC1_Gain(0x%08X) failed.", tUnit));
                AGC1_Type = tmTDA18250_AGC1_Slave;
            }
        }

        if (err == TM_OK)
        {
            switch(AGC1_Type)
            {
                case tmTDA18250_AGC1_MasterSingle:
                    err = TDA18250LvlAgc1Contribution(uAGC1_Current, uRfFreq, &PowerLevel);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250LvlAgc1Contribution(0x%08X) failed.", tUnit));

                    break;

                case tmTDA18250_AGC1_MasterMulti:
                    err = TDA18250LvlAgc1MasterMultiContribution(uAGC1_Current, uRfFreq, &PowerLevel);                    
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250LvlAgc1MasterMultiContribution(0x%08X) failed.", tUnit));
                    break;

                case tmTDA18250_AGC1_Slave:
                    /* Slave AGC1 contribution */
                    err = TDA18250LvlAgc1Contribution(uAGC1_Current, uRfFreq, &PowerLevel);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250LvlAgc1Contribution(0x%08X) failed.", tUnit));

                    if (err == TM_OK)
                    {
                        /* Master contribution */
                        err = TDA18250LvlAgc1MasterMultiContribution(uAGC1_Master, uRfFreq, &PowerLevel);
                        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250LvlAgc1MasterMultiContribution(0x%08X) failed.", tUnit));
                    }
                    
                    break;
                default:
                    break;
            }
            
        }

        /* ----- Offset contribution ----- */
        if (err == TM_OK)
        {
            if (AGC1_Type == tmTDA18250_AGC1_Slave)
            {
                TempPowerLevel = 0;
                for (uCpt=tmTDA18250_Offset_X0; uCpt<=tmTDA18250_Offset_X1; uCpt++)
                {
                    TempPowerLevel += ( TDA18250CalcPow(uRfFreq, uCpt) * Lvl_Offset[uCpt] )  / ( TDA18250CalcPow(10, Lvl_Div10[tmTDA18250_XDIV10_OffsetSlave][uCpt]) );
                }
                PowerLevel -= TempPowerLevel / ( TDA18250CalcPow(10, Lvl_Div10[tmTDA18250_XDIV10_OffsetSlave][tmTDA18250_XDIV10_SUM]) );
            }
        }
        /* return dBmV */
        if (err == TM_OK)
        {
            *pRFEVAL = PowerLevel;
        }
        else
        {
            *pRFEVAL = 0;
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetPower_Level(0x%08X) failed.", tUnit));
        }

    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    TDA182504SetAGC1_loop_off                                     */
/*                                                                            */
/* DESCRIPTION: Sets the loop off                                             */
/*              and check availability of Smooth state machine                */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
TDA18250SetAGC1_loop_off
(
    tmUnitSelect_t          tUnit,      /* I: Unit number */
    tmTDA18250_State_t      uState      /* I: State value */
)
{
    ptmTDA18250Object_t pObj = Null;
    tmErrorCode_t       err  = TM_OK;
    UInt8              uValue = 0;
    UInt8              uCpt ;


    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* AGC1 loop ON or OFF*/
        if (err == TM_OK)
        {
            err = tmddTDA18250SetAGC1_loop_off(tUnit, (tmddTDA18250_State_t)uState);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC1_loop_off(0x%08X) failed.", tUnit));
        }

        if (uState==tmTDA18250_ON)
        {
            /* Check Smooth ON bit to be sure loop off command is acknowlegded */
            /* and Smooth state machine is free */
            if (err == TM_OK)
            {
                err = tmddTDA18250Getagc1_smooth_transition_on(tUnit,&uValue);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250Getagc1_smooth_transition_on(0x%08X) failed.", tUnit));
            }

            uCpt = 0;
            //while ((uValue!=0)&&(uCpt<=2))
            while ((uValue!=0)&&(uCpt<=20))
            {
                if (err == TM_OK)
                {
                    err = TDA18250Wait(pObj, uTDA18250TimeTransitionAGC1[(pObj->AGC1_TimeCst)]);  /* wait */
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250Wait(0x%08X) failed.", tUnit));
                }
                if (err == TM_OK)
                {
                    err = tmddTDA18250Getagc1_smooth_transition_on(tUnit,&uValue);
                    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250Getagc1_smooth_transition_on(0x%08X) failed.", tUnit));
                }
                uCpt++;
            }
            if (uValue!=0) 
            {
                err = TDA18250_ERR_NOT_READY;
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "Smooth state mchine not ready in time !", tUnit));
            }

        }
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    TDA18250SetAGC2_Bypass                                        */
/*                                                                            */
/* DESCRIPTION: Sets the AGC2 Bypass                                          */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
TDA18250SetAGC2_Bypass
(
    tmUnitSelect_t  tUnit   /* I: Unit number */
)
{
    ptmTDA18250Object_t   pObj = Null;
    tmErrorCode_t         err = TM_OK;
    UInt8                 uIdentity ;

    /* Get a driver instance */
    err = TDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Get HW Identity */
        err = tmddTDA18250GetIdentity(tUnit, &uIdentity);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250GetIdentity(0x%08X) failed.", tUnit));

        if ( ( pObj->deviceType >= tmTDA18250_DeviceTypeSlave ) && (uIdentity != 0xD9) )
        {
            /* Execute for Slave device only and ID!= 0xD9 */
            
            if (err == TM_OK)
            {
                /* AGC2 fast reset begin */
                err = tmddTDA18250SetAGC2_bypass(tUnit, tmddTDA18250_ON);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC2_bypass(0x%08X) failed.", tUnit));
            }
            if (err == TM_OK)
            {
                err = TDA18250Wait(pObj, 7);  /* wait 7ms */
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250Wait(0x%08X) failed.", tUnit));
            }
            if (err == TM_OK)
            {
                /* AGC2 fast reset end */
                err = tmddTDA18250SetAGC2_bypass(tUnit, tmddTDA18250_OFF);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC2_bypass(0x%08X) failed.", tUnit));
            }
        }

        if (uIdentity == 0xD9)
        {
            /* Execute for Master and Slave if ID=0xD9 */

            /* slow bypass mode on Up step, nominal mode on Down step with fast response */
            if (err == TM_OK)
            {
                err = tmddTDA18250SetAGC2_loop_off(tUnit, tmddTDA18250_ON);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC2_loop_off(0x%08X) failed.", tUnit));
            }
            if (err == TM_OK)
            {
                err = tmddTDA18250SetAGC2_Up_step(tUnit, 0x00);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC2_Up_step(0x%08X) failed.", tUnit));
            }
            {
                err = tmddTDA18250SetAGC2_Do_step(tUnit, 0x03);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC2_Do_step(0x%08X) failed.", tUnit));
            }
            if (err == TM_OK)
            {
                err = tmddTDA18250SetAGC2_loop_off(tUnit, tmddTDA18250_OFF);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC2_loop_off(0x%08X) failed.", tUnit));
            }
            if (err == TM_OK)
            {
                err = tmddTDA18250SetAGC2_Speed_Up(tUnit, 0x00);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC2_Speed_Up(0x%08X) failed.", tUnit));
            }
            if (err == TM_OK)
            {
                /* AGC2 fast reset begin */
                err = tmddTDA18250SetAGC2_bypass(tUnit, tmddTDA18250_ON);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC2_bypass(0x%08X) failed.", tUnit));
            }
            
            if (err == TM_OK)
            {
                err = TDA18250Wait(pObj, 20);  /* wait 20ms */
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "TDA18250Wait(0x%08X) failed.", tUnit));
            }

            /* return to nominal mode for up and down time constant */
            if (err == TM_OK)
            {
                err = tmddTDA18250SetAGC2_loop_off(tUnit, tmddTDA18250_ON);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC2_loop_off(0x%08X) failed.", tUnit));
            }
            if (err == TM_OK)
            {
                err = tmddTDA18250SetAGC2_Up_step(tUnit, 0x00);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC2_Up_step(0x%08X) failed.", tUnit));
            }
            if (err == TM_OK)
            {
                err = tmddTDA18250SetAGC2_Do_step(tUnit, 0x00);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC2_Do_step(0x%08X) failed.", tUnit));
            }
            if (err == TM_OK)
            {
                err = tmddTDA18250SetAGC2_loop_off(tUnit, tmddTDA18250_OFF);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC2_loop_off(0x%08X) failed.", tUnit));
            }
            if (err == TM_OK)
            {
                /* AGC2 fast reset end */
                err = tmddTDA18250SetAGC2_bypass(tUnit, tmddTDA18250_OFF);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "tmddTDA18250SetAGC2_bypass(0x%08X) failed.", tUnit));
            }
        }
    }
    
    return err;
}


/*============================================================================*/
/* FUNCTION:    TDA18250Wait:                                                 */
/*                                                                            */
/* DESCRIPTION: Waits for the requested time.                                 */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t 
TDA18250Wait
(
    ptmTDA18250Object_t pObj,   /* I: Driver object    */
    UInt32              wTime   /* I: time to wait for */
)
{
    tmErrorCode_t err  = TM_OK;


    /* wait wTime ms */
    err = POBJ_SRVFUNC_STIME.Wait (pObj->tUnit, wTime);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "Wait(0x%08X) failed.", pObj->tUnit));

    return err;
}


/*============================================================================*/
/* FUNCTION:    TDA18250MutexAcquire:                                         */
/*                                                                            */
/* DESCRIPTION: Mutex Acquisition.                                            */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
TDA18250MutexAcquire
(
    ptmTDA18250Object_t    pObj,    /* I: Driver object */
    UInt32                 timeOut  /* I: Timeout       */
)
{
    tmErrorCode_t err = TM_OK;


    if (pObj->sMutex.Acquire != Null && pObj->pMutex != Null)
    {
        err = pObj->sMutex.Acquire(pObj->pMutex, timeOut);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "MutexAcquire(0x%08X) failed.", pObj->tUnit));
    }
    
    return err;
}


/*============================================================================*/
/* FUNCTION:    TDA18250MutexRelease:                                         */
/*                                                                            */
/* DESCRIPTION: Mutex release.                                                */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
TDA18250MutexRelease
(
    ptmTDA18250Object_t    pObj  /* I: Driver object */
)
{
    tmErrorCode_t err = TM_OK;


    if (pObj->sMutex.Release != Null && pObj->pMutex != Null)
    {
        err = pObj->sMutex.Release(pObj->pMutex);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "MutexRelease(0x%08X) failed.", pObj->tUnit));
    }
    
    return err;
}
