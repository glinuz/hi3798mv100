/**
 * Copyright (C) 2005 Koninklijke Philips Electronics N.V., All Rights Reserved.
 * This source code and any compilation or derivative thereof is the proprietary
 * information of Koninklijke Philips Electronics N.V. and is confidential in
 * nature. Under no circumstances is this software to be  exposed to or placed
 * under an Open Source License of any type without the expressed written
 * permission of Koninklijke Philips Electronics N.V.
 *
 * \file          tmbslOOBIp_cfg.c
 *
 * \date          26-Nov-2007
 *
 * \brief         -
 *
 * REFERENCE DOCUMENTS :
 *
 * Detailled description may be added here.
 *
 * \section info Change Information
 *
 * \verbatim
   Date          Modified by CRPRNr  TASKNr  Maintenance description
   -------------|-----------|-------|-------|-----------------------------------
   26-July-2010 | A.TANT    |       |       | Creation
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


#include "tmbslOOBIp.h"
#include "tmbslOOBIplocal.h"

#include "tmbslOOBIpInstance.h"

#include "tmbslOOBIp_cfg.h"


/*============================================================================*/
/*                       MACRO DEFINITION                                     */
/*============================================================================*/
#define TMBSLOOBIP_CFG_COMPATIBILITY_NB 2

#if TMBSLOOBIP_CFG_VERSION != TMBSLOOBIP_CFG_COMPATIBILITY_NB
    #error "Wrong cfg files version !!!!"
#endif

/*============================================================================*/
/*                   PUBLIC FUNCTIONS DEFINITIONS                             */
/*============================================================================*/

/*============================================================================*/
/* tmbslOOBIp__CFG_GetConfig                                                   */
/*============================================================================*/
tmbslOOBIp_Cfg_Err_t
tmbslOOBIp_CFG_GetConfig( tmbslOOBIp_Cfg_t* pConfig )
{
    if (pConfig != (void*)0)
    {
        pConfig->CompatibilityNb_U          = TMBSLOOBIP_CFG_COMPATIBILITY_NB;
        pConfig->uOOBIpStartAddress         = TMBSLOOBIP_START_ADDRESS;
        return tmbslOOBIp_Cfg_NoError_E;
    }
    else
    {
        return tmbslOOBIp_Cfg_InvalidValue_E;
    }
}

/*============================================================================*/
/*                     END OF FILE                                            */
/*============================================================================*/

