/**
  Copyright (C) 2008 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmbslOOBIp_Instance.c
 *                %version: CFR_FEAP#2 %
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
   26-Nov-2007  | V.VRIGNAUD|       |       | COMPATIBILITY WITH NXPFE
                | C.CAZETTES|       |       | 
   -------------|-----------|-------|-------|-----------------------------------
                |           |       |       |
   -------------|-----------|-------|-------|-----------------------------------
   \endverbatim
 *
*/

#include "tmNxTypes.h"
#include "tmCompId.h"
#include "tmFrontEnd.h"
#include "tmbslFrontEndTypes.h"

#include "tmbslOOBIp.h"
#include "tmbslOOBIplocal.h"

#include "tmbslOOBIpInstance.h"

/*----------------------------------------------------------------------------*/
/* Global data:                                                               */
/*----------------------------------------------------------------------------*/
tmOOBIpObject_t gOOBIpInstance[] = 
{
    {0},{0},{0},{0}
};

/*============================================================================*/
/* FUNCTION:    tmbslOOBIp_AllocInstance:                                   */
/*                                                                            */
/* DESCRIPTION: allocate new instance                                         */
/*                                                                            */
/* RETURN:                                                                    */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
OOBIpAllocInstance
(
    tmUnitSelect_t       tUnit,         /* I: Unit number   */
    tmOOBIpObject_t **ppDrvObject    /* I: Device Object */
)
{ 
    tmErrorCode_t       err = OOBIP_ERR_BAD_UNIT_NUMBER;
    tmOOBIpObject_t    *pObj = Null;
    UInt32              uLoopCounter = 0;    

    /* Find a free instance */
    for(uLoopCounter = 0; uLoopCounter<OOBIP_MAX_UNITS; uLoopCounter++)
    {
        pObj = &gOOBIpInstance[uLoopCounter];
        if(pObj->sApiFlags.iInit == False)
        {
            pObj->tUnit = tUnit;
            
            *ppDrvObject = pObj;
            err = TM_OK;
            break;
        }
    }

    /* return value */
    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslOOBIp_DeAllocInstance:                                 */
/*                                                                            */
/* DESCRIPTION: deallocate instance                                           */
/*                                                                            */
/* RETURN:      always TM_OK                                                  */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
OOBIpDeAllocInstance
(
    tmUnitSelect_t  tUnit    /* I: Unit number */
)
{     
    tmErrorCode_t       err = OOBIP_ERR_BAD_UNIT_NUMBER;
    tmOOBIpObject_t    *pObj = Null;
 
    /* check input parameters */
    err = OOBIpGetInstance(tUnit, &pObj);

    /* check driver state */
    if (err == TM_OK)
    {
        if (pObj == Null || pObj->sApiFlags.iInit == False)
        {
            err = OOBIP_ERR_NOT_INITIALIZED;
        }
    }

    if ((err == TM_OK) && (pObj != Null)) 
    {
        pObj->sApiFlags.iInit = False;
    }

    /* return value */
    return err;
}

/*============================================================================*/
/* FUNCTION:    tmbslOOBIp_GetInstance:                                     */
/*                                                                            */
/* DESCRIPTION: get the instance                                              */
/*                                                                            */
/* RETURN:      always True                                                   */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
OOBIpGetInstance 
(
    tmUnitSelect_t       tUnit,         /* I: Unit number   */
    tmOOBIpObject_t    **ppDrvObject    /* I: Device Object */
)
{     
    tmErrorCode_t       err = OOBIP_ERR_NOT_INITIALIZED;
    tmOOBIpObject_t *pObj = Null;
    UInt32              uLoopCounter = 0;    

    /* get instance */
    for(uLoopCounter = 0; uLoopCounter<OOBIP_MAX_UNITS; uLoopCounter++)
    {
        pObj = &gOOBIpInstance[uLoopCounter];
        if(pObj->sApiFlags.iInit == True && pObj->tUnit == tUnit)
        {
            *ppDrvObject = pObj;
            err = TM_OK;
            break;
        }
    }

    /* return value */
    return err;
}


