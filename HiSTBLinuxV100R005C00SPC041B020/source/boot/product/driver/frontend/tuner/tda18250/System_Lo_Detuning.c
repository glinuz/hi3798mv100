/*
  Copyright (C) 2006-2009 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          System_Lo_Detuning.c
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
#include "tmUnitParams.h"

/*============================================================================*/
/* Project include files:                                                     */
/*============================================================================*/
#include "tmddTDA18250.h"
#include "tmbslTDA18250.h"
#include <tmbslTDA18250InstanceCustom.h>
#include "tmbslTDA18250local.h"
#include "System_Lo_Detuning.h"


/*============================================================================*/
/* FUNCTION:    tmLO_Detuning:                                                */
/*                                                                            */
/* DESCRIPTION: Calculate the shift of each LO of all Tuners                  */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t tmSystem_LO_Detuning
(
    tmUnitSelect_t          TunerUnit,      /* I: Tuner unit number                   */
    tmTDA18250ChannelType_t ChannelType,    /* I: Channel type                        */
    UInt32                  *uLO            /* I: local oscillator frequency in hertz */
)
{    
    tmErrorCode_t err = TM_OK;
    UInt32        uCounter;
    UInt32        uIndex;
    Int32         VCOs[TDA18250_MAX_UNITS];
    UInt32        uLoTemp;
    UInt8         uPrescTemp;
    UInt8         uPostDivTemp;
    Bool          bShiftFreq;

    uIndex = (UNIT_PATH_INDEX_GET(TunerUnit) + 1)%TDA18250_MAX_UNITS;

    if (ChannelType == tmTDA18250AnalogChannel)
        *uLO += 40000;

    /**************************************************************************/
    /*                  Calculate VCOs of other Tuners                        */
    /**************************************************************************/
    for (uCounter=1; uCounter < TDA18250_MAX_UNITS; uCounter++)
    {

        /* Retrieve Tuner Lo */
        err = tmbslTDA18250ReadLO(UNIT_PATH_TYPE_SET(uIndex,UNIT_PATH_TYPE_GET(TunerUnit)), &uLoTemp);

        if (err == TM_OK)
        {
            /* Retrieve Prescaler and PostDiv */
            err = tmddTDA18250GetMainPLL(UNIT_PATH_TYPE_SET(uIndex,UNIT_PATH_TYPE_GET(TunerUnit)), uLoTemp, &uPrescTemp, &uPostDivTemp);

            if (err == TM_OK)
            {
                /* Calculate and store VCO (in kHz) */
                VCOs[uCounter] = (uLoTemp/1000) * uPrescTemp * uPostDivTemp;
            }
        }

        
        uIndex = (uIndex + 1)%TDA18250_MAX_UNITS;
    }

    do
    {
        /**********************************************************************/
        /*                 Calculate VCOs of current Tuner                    */
        /**********************************************************************/
        /* Retrieve Prescaler and PostDiv */
        err = tmddTDA18250GetMainPLL(TunerUnit, *uLO, &uPrescTemp, &uPostDivTemp);

        if (err == TM_OK)
        {
            /* Calculate and store VCO (in kHz) */
            VCOs[0] = ((*uLO)/1000) * uPrescTemp * uPostDivTemp;

            bShiftFreq = False;

            /* Search if we may encounter VCO pulling troubles */
            for(uCounter = 1; uCounter < TDA18250_MAX_UNITS; uCounter++)
            {
                if( abs( VCOs[0] - VCOs[uCounter] ) < 300)
                {
                    /* We need to shift frequency */
                    bShiftFreq = True;
                    *uLO += 5000;
                    break;
                }
            }
        }
        
    }
    while ((bShiftFreq) && (err == TM_OK));

    return err;
}
