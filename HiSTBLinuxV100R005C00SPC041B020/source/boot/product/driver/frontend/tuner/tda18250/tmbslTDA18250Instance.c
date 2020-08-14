/*
  Copyright (C) 2006-2009 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmbslTDA18250Instance.c
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
#include "tmUnitParams.h"
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
/* Global data:                                                               */
/*============================================================================*/

/* Driver instance */
tmTDA18250Object_t gTDA18250Instance[] = 
{
    {
        (tmUnitSelect_t)(-1),                                   /* tUnit      */
        (tmUnitSelect_t)(-1),                                   /* tUnit temporary */
        Null,                                                   /* pMutex     */
        False,                                                  /* init (instance initialization default) */
        {                                                       /* sRWFunc    */
            Null,
            Null
        },
        {                                                       /* sTime      */
            Null,
            Null
        },
        {                                                       /* sDebug     */
            Null
        },
        {                                                       /* sMutex     */
            Null,
            Null,
            Null,
            Null
        },
        0,                                                      /* RF         */
        0,                                                      /* freqCut    */
        False,                                                  /* ResetCalledPreviously */
        tmTDA18250_PowerTransitionMode_Smooth,                  /* ePowerTransitionMode */
        TMBSL_TDA18250_INSTANCE_CUSTOM_0
    },
    {
        (tmUnitSelect_t)(-1),                                   /* tUnit      */
        (tmUnitSelect_t)(-1),                                   /* tUnit temporary */
        Null,                                                   /* pMutex     */
        False,                                                  /* init (instance initialization default) */
        {                                                       /* sRWFunc    */
            Null,
            Null
        },
        {                                                       /* sTime      */
            Null,
            Null
        },
        {                                                       /* sDebug     */
            Null
        },
        {                                                       /* sMutex     */
            Null,
            Null,
            Null,
            Null
        },
        0,                                                      /* RF         */
        0,                                                      /* freqCut    */
        False,                                                  /* ResetCalledPreviously */
        tmTDA18250_PowerTransitionMode_Smooth,                  /* ePowerTransitionMode */
        TMBSL_TDA18250_INSTANCE_CUSTOM_1
    },
    {
        (tmUnitSelect_t)(-1),                                   /* tUnit      */
        (tmUnitSelect_t)(-1),                                   /* tUnit temporary */
        Null,                                                   /* pMutex     */
        False,                                                  /* init (instance initialization default) */
        {                                                       /* sRWFunc    */
            Null,
            Null
        },
        {                                                       /* sTime      */
            Null,
            Null
        },
        {                                                       /* sDebug     */
            Null
        },
        {                                                       /* sMutex     */
            Null,
            Null,
            Null,
            Null
        },
        0,                                                      /* RF         */
        0,                                                      /* freqCut    */
        False,                                                  /* ResetCalledPreviously */
        tmTDA18250_PowerTransitionMode_Smooth,                  /* ePowerTransitionMode */
        TMBSL_TDA18250_INSTANCE_CUSTOM_2
    },
    {
        (tmUnitSelect_t)(-1),                                   /* tUnit      */
        (tmUnitSelect_t)(-1),                                   /* tUnit temporary */
        Null,                                                   /* pMutex     */
        False,                                                  /* init (instance initialization default) */
        {                                                       /* sRWFunc    */
            Null,
            Null
        },
        {                                                       /* sTime      */
            Null,
            Null
        },
        {                                                       /* sDebug     */
            Null
        },
        {                                                       /* sMutex     */
            Null,
            Null,
            Null,
            Null
        },
        0,                                                      /* RF         */
        0,                                                      /* freqCut    */
        False,                                                  /* ResetCalledPreviously */
        tmTDA18250_PowerTransitionMode_Smooth,                  /* ePowerTransitionMode */
        TMBSL_TDA18250_INSTANCE_CUSTOM_3
    }
};


/*============================================================================*/
/* Internal functions:                                                        */
/*============================================================================*/

/*============================================================================*/
/* FUNCTION:    TDA18250AllocInstance:                                        */
/*                                                                            */
/* DESCRIPTION: Allocates an instance.                                        */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
TDA18250AllocInstance
(
    tmUnitSelect_t          tUnit,      /* I: Unit number   */
    pptmTDA18250Object_t    ppDrvObject /* I: Device Object */
)
{ 
    tmErrorCode_t       err = TDA18250_ERR_BAD_UNIT_NUMBER;
    ptmTDA18250Object_t pObj = Null;
    UInt32              uLoopCounter = 0;    

    /* Find a free instance */
    for (uLoopCounter = 0; uLoopCounter<TDA18250_MAX_UNITS; uLoopCounter++)
    {
        pObj = &gTDA18250Instance[uLoopCounter];
        if (pObj->init == False)
        {
            pObj->tUnit = tUnit;
            pObj->tUnitW = tUnit;

            *ppDrvObject = pObj;
            err = TM_OK;
            break;
        }
    }

    /* return value */
    return err;
}

/*============================================================================*/
/* FUNCTION:    TDA18250DeAllocInstance:                                      */
/*                                                                            */
/* DESCRIPTION: Deallocates an instance.                                      */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
TDA18250DeAllocInstance
(
    tmUnitSelect_t  tUnit   /* I: Unit number */
)
{     
    tmErrorCode_t       err = TDA18250_ERR_BAD_UNIT_NUMBER;
    ptmTDA18250Object_t pObj = Null;

    /* check input parameters */
    err = TDA18250GetInstance(tUnit, &pObj);

    /* check driver state */
    if (err == TM_OK)
    {
        if (pObj == Null || pObj->init == False)
        {
            err = TDA18250_ERR_NOT_INITIALIZED;
        }
    }

    if ((err == TM_OK) && (pObj != Null)) 
    {
        pObj->init = False;
    }

    /* return value */
    return err;
}


/*============================================================================*/
/* FUNCTION:    TDA18250GetInstance:                                          */
/*                                                                            */
/* DESCRIPTION: Gets an instance.                                             */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
TDA18250GetInstance
(
    tmUnitSelect_t          tUnit,      /* I: Unit number   */
    pptmTDA18250Object_t    ppDrvObject /* I: Device Object */
)
{     
    tmErrorCode_t       err = TDA18250_ERR_NOT_INITIALIZED;
    ptmTDA18250Object_t pObj = Null;
    UInt32              uLoopCounter = 0;    

    /* get instance */
    for (uLoopCounter = 0; uLoopCounter<TDA18250_MAX_UNITS; uLoopCounter++)
    {
        pObj = &gTDA18250Instance[uLoopCounter];
        if (pObj->init == True && pObj->tUnit == GET_INDEX_TYPE_TUNIT(tUnit))
        {
            
            *ppDrvObject = pObj;
            err = TM_OK;
            break;
        }
    }

    /* return value */
    return err;
}

