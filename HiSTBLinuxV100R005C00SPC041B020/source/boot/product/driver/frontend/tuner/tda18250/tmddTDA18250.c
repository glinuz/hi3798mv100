/*
  Copyright (C) 2006-2009 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmddTDA18250.c
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

#include "tmddTDA18250local.h"
#include "tmddTDA18250Instance.h"

/*============================================================================*/
/* Types and defines:                                                         */
/*============================================================================*/

/*============================================================================*/
/* Global data:                                                               */
/*============================================================================*/
UInt32 tmddTDA18250I2CMapAddress; 

UInt8  utmddTDA18250_UNUSED_0x1B_1;

/*============================================================================*/
/* Internal Prototypes:                                                       */
/*============================================================================*/

/*============================================================================*/
/* Exported functions:                                                        */
/*============================================================================*/


/*============================================================================*/
/* FUNCTION:    tmddTDA18250Init:                                             */
/*                                                                            */
/* DESCRIPTION: Initializes driver setup environment.                         */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250Init
(
    tmUnitSelect_t              tUnit,      /* I: Unit number      */
    tmbslFrontEndDependency_t*  psSrvFunc   /* I: setup parameters */
)
{
    ptmddTDA18250Object_t   pObj = Null;
    tmErrorCode_t           err  = TM_OK;


    /* Test parameters */
    if (psSrvFunc == Null)
    {
        err = ddTDA18250_ERR_BAD_PARAMETER;
    }

    if (err == TM_OK)
    {
        /* Get a driver instance */
        err = ddTDA18250GetInstance(tUnit, &pObj);
    }

    /* Check driver instance state */
    if (err == TM_OK || err == ddTDA18250_ERR_NOT_INITIALIZED)
    {
        if (pObj != Null && pObj->init == True)
        {
            err = ddTDA18250_ERR_NOT_INITIALIZED;
        }
        else 
        {
            if (pObj == Null)
            {
                /* Try to allocate an instance of the driver */
                err = ddTDA18250AllocInstance(tUnit, &pObj);
                if (err != TM_OK || pObj == Null)
                {
                    err = ddTDA18250_ERR_NOT_INITIALIZED;        
                }
            }

            if (err == TM_OK)
            {
                /* initialize the Object by default values */
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

                tmddTDA18250I2CMapAddress = (UInt32)&pObj->I2CMap;
                
                pObj->init = True;
            }
        }
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250DeInit:                                           */
/*                                                                            */
/* DESCRIPTION: Cleans driver setup environment.                              */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t 
tmddTDA18250DeInit
(
    tmUnitSelect_t  tUnit   /* I: Unit number */
)
{
    tmErrorCode_t           err = TM_OK;
    ptmddTDA18250Object_t   pObj = Null;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    tmDBGPRINTEx(DEBUGLVL_BLAB, "tmddTDA18250DeInit(0x%08X)", tUnit);

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);

        if (pObj->sMutex.DeInit != Null)
        {
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

    err = ddTDA18250DeAllocInstance(tUnit);

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetSWVersion:                                     */
/*                                                                            */
/* DESCRIPTION: Return the version of this device.                            */
/*                                                                            */
/* RETURN:      TM_OK                                                         */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetSWVersion
(
    ptmSWVersion_t  pSWVersion  /* O: Receives SW Version */
)
{
    pSWVersion->compatibilityNr = TMDD_TDA18250_COMP_NUM;
    pSWVersion->majorVersionNr  = TMDD_TDA18250_MAJOR_VER;
    pSWVersion->minorVersionNr  = TMDD_TDA18250_MINOR_VER;

    return TM_OK;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetPowerState:                                    */
/*                                                                            */
/* DESCRIPTION: Set the power state of this device.                           */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetPowerState
(
    tmUnitSelect_t            tUnit,        /* I: Unit number                */
    tmddTDA18250PowerState_t  powerState    /* I: Power state of this device */
)
{
    ptmddTDA18250Object_t   pObj = Null;
    tmErrorCode_t           err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));
    
    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set power state */
        switch (powerState)
        {
            case tmddTDA18250_PowerNormalMode:                    
                pObj->I2CMap.uBx17.bF.SM = 0;
                
                break;

            case tmddTDA18250_PowerStandby:
            case tmddTDA18250_PowerStandbyWithLoopThroughOnAndWithXtalOn:
                pObj->I2CMap.uBx17.bF.SM = 1;
                break;
                
            default:
                /* power state not supported */
                err = ddTDA18250_ERR_NOT_SUPPORTED;
                break;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Write(pObj, TMDD_TDA18250_0x17_Power_Down_byte_1, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        }

        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetPowerState:                                    */
/*                                                                            */
/* DESCRIPTION: Sets the standard mode.                                       */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetPowerState
(
    tmUnitSelect_t            tUnit,        /* I: Unit number                */
    ptmddTDA18250PowerState_t pPowerState   /* O: Power state of this device */
)
{
    ptmddTDA18250Object_t   pObj = Null;
    tmErrorCode_t           err  = TM_OK;
    UInt8                   uPowerState = 0;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* get power state */
        err = ddTDA18250Read(pObj, TMDD_TDA18250_0x17_Power_Down_byte_1, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        
        if (err == TM_OK)
        {
            uPowerState = pObj->I2CMap.uBx17.bF.SM;
        }
        else
        {
            uPowerState = 0x00;
        }
        
        if (err == TM_OK)
        {
            if (uPowerState == 0)
                *pPowerState = tmddTDA18250_PowerNormalMode;
            else
                *pPowerState = tmddTDA18250_PowerStandby;
        }

        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250Write                                             */
/*                                                                            */
/* DESCRIPTION: This function writes I2C data in the Tuner.                   */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250Write
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                  */
    UInt32          uIndex,     /* I: Start index to write         */
    UInt32          uNbBytes,   /* I: Number of bytes to write     */
    UInt8*          puBytes     /* I: Pointer on an array of bytes */
)
{
    ptmddTDA18250Object_t   pObj = Null;
    tmErrorCode_t           err  = TM_OK;
    UInt32                  uCounter;
    UInt8*                  pI2CMap = Null;

    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* pI2CMap initialization */
        pI2CMap = &(pObj->I2CMap.uBx00.ID_byte) + uIndex;

        /* save the values written in the Tuner */
        for (uCounter = 0; uCounter < uNbBytes; uCounter++)
        {
            *pI2CMap = puBytes[uCounter];
            pI2CMap ++;
        }

        /* write in the Tuner */
        err = ddTDA18250Write(pObj,(UInt8)(uIndex),(UInt8)(uNbBytes));
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));

        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250Read                                              */
/*                                                                            */
/* DESCRIPTION: Read in the Tuner.                                            */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250Read
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                  */
    UInt32          uIndex,     /* I: Start index to write         */
    UInt32          uNbBytes,   /* I: Number of bytes to read      */
    UInt8*          puBytes     /* I: Pointer on an array of bytes */
)
{

    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;
    UInt32                       uCounter = 0;
    UInt8*                       pI2CMap = Null;


    /* test the parameters */
    if (uNbBytes > TDA18250_I2C_MAP_NB_BYTES)
        err = ddTDA18250_ERR_BAD_UNIT_NUMBER;

    if (err == TM_OK)
    {
        /* Get Instance Object */
        err = ddTDA18250GetInstance(tUnit, &pObj);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));
    }

    if (err == TM_OK)
    {
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* pI2CMap initialization */
        pI2CMap = &(pObj->I2CMap.uBx00.ID_byte) + uIndex;

        /* read from the Tuner */
        err = ddTDA18250Read(pObj,(UInt8)(uIndex),(UInt8)(uNbBytes));

        /* copy readen values to puBytes */
        for (uCounter = 0; uCounter < uNbBytes; uCounter++)
        {
            *puBytes = (*pI2CMap);
            pI2CMap ++;
            puBytes ++;
        }

        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250Reset:                                            */
/*                                                                            */
/* DESCRIPTION: Resets the TDA18250.                                          */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250Reset
(
    tmUnitSelect_t          tUnit,               /* I: Unit number      */
    Bool                    clock_mode_auto,     /* I: clock mode       */
    tmddTDA18250ResetAGC_t  resetAGC             /* I: reset or not AGC */
)
{   
    ptmddTDA18250Object_t   pObj = Null;
    tmErrorCode_t           err = TM_OK;

    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }
        
    if (err == TM_OK)
    {

        /* write all initialization bytes */
        pObj->I2CMap.uBx05.Read_byte_5         = 0x00;
        pObj->I2CMap.uBx06.Read_byte_6         = 0x00;
        pObj->I2CMap.uBx07.PSM_byte_1          = 0xC0;
        
        pObj->I2CMap.uBx08.Main_Divider_byte_1 = 0x00;
        pObj->I2CMap.uBx09.Main_Divider_byte_2 = 0x40;
        pObj->I2CMap.uBx0A.Main_Divider_byte_3 = 0x00;
        pObj->I2CMap.uBx0B.Main_Divider_byte_4 = 0x00;
        pObj->I2CMap.uBx0C.Main_Divider_byte_5 = 0x00;
        pObj->I2CMap.uBx0D.Main_Divider_byte_6 = 0x00;
        pObj->I2CMap.uBx0E.Main_Divider_byte_7 = 0x84;
        pObj->I2CMap.uBx0F.Main_Divider_byte_8 = 0x09;
        
        pObj->I2CMap.uBx10.Cal_Divider_byte_1  = 0x00; 
        pObj->I2CMap.uBx11.Cal_Divider_byte_2  = 0x13; 
        pObj->I2CMap.uBx12.Cal_Divider_byte_3  = 0x00; 
        pObj->I2CMap.uBx13.Cal_Divider_byte_4  = 0x00; 
        pObj->I2CMap.uBx14.Cal_Divider_byte_5  = 0x01; 
        pObj->I2CMap.uBx15.Cal_Divider_byte_6  = 0x84; 
        pObj->I2CMap.uBx16.Cal_Divider_byte_7  = 0x09;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x05_Read_byte_5, 18);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));

        if (err == TM_OK)
        {
            pObj->I2CMap.uBx1A.IF_Byte_1         = 0x8E;

            pObj->I2CMap.uBx1B.IF_Byte_2         = 0x5D;
            utmddTDA18250_UNUSED_0x1B_1          = pObj->I2CMap.uBx1B.bF.UNUSED_0x1B_1;
            
            pObj->I2CMap.uBx1C.AGC2b_byte        = 0x9F;
            
            pObj->I2CMap.uBx1D.PSM_byte_2        = 0x01;
            pObj->I2CMap.uBx1E.PSM_byte_3        = 0x00;

            err = ddTDA18250Write(pObj, TMDD_TDA18250_0x1A_IF_Byte_1, 5);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        }
        
        if ( (err == TM_OK) && (resetAGC == tmddTDA18250_WithResetAGC) )
        {
            pObj->I2CMap.uBx1F.PSM_byte_4        = 0x00;
            
            pObj->I2CMap.uBx20.AGC1_byte_1       = 0x00;
            pObj->I2CMap.uBx21.AGC1_byte_2       = 0x80;
            pObj->I2CMap.uBx22.AGC1_byte_3       = 0x0C;
            
            pObj->I2CMap.uBx23.AGC2_byte_1       = 0x00;
            pObj->I2CMap.uBx24.AGC2_byte_2       = 0x0F;
            
            err = ddTDA18250Write(pObj, TMDD_TDA18250_0x1F_PSM_byte_4, 6);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            pObj->I2CMap.uBx25.Analog_AGC_byte   = 0x28;
            pObj->I2CMap.uBx26.RC_byte           = 0xE0;
            pObj->I2CMap.uBx27.RSSI_byte         = 0xDE;
            
            pObj->I2CMap.uBx28.IR_CAL_byte_1     = 0xA7;
            pObj->I2CMap.uBx29.IR_CAL_byte_2     = 0x40;
            pObj->I2CMap.uBx2A.IR_CAL_byte_3     = 0x40;
            pObj->I2CMap.uBx2B.IR_CAL_byte_4     = 0x00;
            
            if (clock_mode_auto)
            {
                pObj->I2CMap.uBx2C.RF_Cal_byte_1     = 0x30;
            }
            else
            {
                pObj->I2CMap.uBx2C.RF_Cal_byte_1     = 0xF0;
            }
            pObj->I2CMap.uBx2D.RF_Cal_byte_2     = 0x90;
            pObj->I2CMap.uBx2E.RF_Cal_byte_3     = 0x80;
            pObj->I2CMap.uBx2F.RF_Cal_byte_4     = 0x00;
            pObj->I2CMap.uBx30.RF_Cal_byte_5     = 0x36;
            pObj->I2CMap.uBx31.RF_Cal_byte_6     = 0x00;
            pObj->I2CMap.uBx32.RF_Cal_byte_7     = 0x2A;
            pObj->I2CMap.uBx33.RF_Cal_byte_8     = 0x00;
            pObj->I2CMap.uBx34.RF_Cal_byte_9     = 0x00;
            pObj->I2CMap.uBx35.RF_Cal_byte_10    = 0x00;
            
            pObj->I2CMap.uBx36.RF_Cal_RAM_byte_1 = 0x00;
            pObj->I2CMap.uBx37.RF_Cal_RAM_byte_2 = 0x00;
            
            pObj->I2CMap.uBx38.Debug_byte        = 0x00;
            pObj->I2CMap.uBx39.Fmax_byte_1       = 0xF6;
            pObj->I2CMap.uBx3A.Fmax_byte_2       = 0xF6;
        
            pObj->I2CMap.uBx3B.RCCAL_delay       = 0x20;
            pObj->I2CMap.uBx3C.AGC1_smooth       = 0x00;
            pObj->I2CMap.uBx3D.LT_smooth         = 0x00;
            pObj->I2CMap.uBx3E.RF_MUX_test       = 0x00;
            pObj->I2CMap.uBx3F.DTA_Filter        = 0x00;
            pObj->I2CMap.uBx40.AGC1bis           = 0xB8;    /* modified according to flow chart V6 */
            
            pObj->I2CMap.uBx41.IF_byte_1         = 0x00;
            pObj->I2CMap.uBx42.IF_byte_2         = 0xA0;
            
            pObj->I2CMap.uBx43.RFSW_SMOOTH       = 0x00;
            pObj->I2CMap.uBx44.RFSW_NOTCH        = 0x00;

            err = ddTDA18250Write(pObj, TMDD_TDA18250_0x25_Analog_AGC_byte, 32);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250ResetXtoutMTO:                                    */
/*                                                                            */
/* DESCRIPTION: Resets the XTout & MTO versus device type.                    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250ResetXtoutMTO
(
    tmUnitSelect_t           tUnit,      /* I: Unit number */
    tmddTDA18250DeviceType_t deviceType  /* I: device type */
)
{   
    ptmddTDA18250Object_t   pObj = Null;
    tmErrorCode_t           err = TM_OK;

    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }
        
    if (err == TM_OK)
    {
        /* manage XTout & MTO versus device type */
        switch (deviceType)
        {
            case tmddTDA18250_DeviceTypeMasterMulti:
                pObj->I2CMap.uBx17.Power_Down_byte_1 = 0x80;
                pObj->I2CMap.uBx18.Power_Down_byte_2 = 0x63;
                pObj->I2CMap.uBx19.Xtout_Byte        = 0x0A;
                break;

            case tmddTDA18250_DeviceTypeMasterMultiWoXTout:
                pObj->I2CMap.uBx17.Power_Down_byte_1   = 0x80;  
                pObj->I2CMap.uBx18.Power_Down_byte_2   = 0x63;  
                pObj->I2CMap.uBx19.Xtout_Byte          = 0x01;
                break;

            case tmddTDA18250_DeviceTypeMasterSingleWithLTWithXTout:
                pObj->I2CMap.uBx17.Power_Down_byte_1 = 0x80;
                pObj->I2CMap.uBx18.Power_Down_byte_2 = 0x63;
                pObj->I2CMap.uBx19.Xtout_Byte        = 0x0A;
                break;

            case tmddTDA18250_DeviceTypeMasterSingleWithLTWoXTout:
                pObj->I2CMap.uBx17.Power_Down_byte_1   = 0x80;  
                pObj->I2CMap.uBx18.Power_Down_byte_2   = 0x63;  
                pObj->I2CMap.uBx19.Xtout_Byte          = 0x01;
                break;

            case tmddTDA18250_DeviceTypeMasterSingleWoLTWithXTout:
                pObj->I2CMap.uBx17.Power_Down_byte_1   = 0xC0;  
                pObj->I2CMap.uBx18.Power_Down_byte_2   = 0x23;  
                pObj->I2CMap.uBx19.Xtout_Byte          = 0x0A;
                break;

            case tmddTDA18250_DeviceTypeMasterSingleWoLTWoXTout:
                pObj->I2CMap.uBx17.Power_Down_byte_1   = 0xC0;  
                pObj->I2CMap.uBx18.Power_Down_byte_2   = 0x23;  
                pObj->I2CMap.uBx19.Xtout_Byte          = 0x01;
                break;

            case tmddTDA18250_DeviceTypeSlaveWithXTout:
                pObj->I2CMap.uBx17.Power_Down_byte_1   = 0xC0;
                pObj->I2CMap.uBx18.Power_Down_byte_2   = 0x27;
                pObj->I2CMap.uBx19.Xtout_Byte          = 0x0A;
                break;

            case tmddTDA18250_DeviceTypeSlaveWithLTWoXTout:
                pObj->I2CMap.uBx17.Power_Down_byte_1   = 0x80;
                pObj->I2CMap.uBx18.Power_Down_byte_2   = 0x63;
                pObj->I2CMap.uBx19.Xtout_Byte          = 0x01;
                break;

            case tmddTDA18250_DeviceTypeSlave:
                pObj->I2CMap.uBx17.Power_Down_byte_1   = 0xC0;  
                pObj->I2CMap.uBx18.Power_Down_byte_2   = 0x27;  
                pObj->I2CMap.uBx19.Xtout_Byte          = 0x01;
                break;

            default:
                err = ddTDA18250_ERR_BAD_PARAMETER;
                break;
        }
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x17_Power_Down_byte_1, 3);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250WriteDataRAM:                                     */
/*                                                                            */
/* DESCRIPTION: writes data in RAM.                                           */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250WriteDataRAM
(
    tmUnitSelect_t tUnit,       /* I: Unit number */
    UInt8          uAddRAM,     /* I: Address RAM */
    UInt8          uDataRAM     /* I: Data RAM    */
)
{   
    ptmddTDA18250Object_t   pObj = Null;
    tmErrorCode_t           err = TM_OK;

    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }
        
    if (err == TM_OK)
    {
        
        if (err == TM_OK)
        {
            /* RAM write mode selected */
            pObj->I2CMap.uBx36.bF.WRI2CRAM = 1;
            
            err = ddTDA18250Write(pObj, TMDD_TDA18250_0x36_RF_Cal_RAM_byte_1, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            pObj->I2CMap.uBx37.bF.AddRAM = uAddRAM;
            
            err = ddTDA18250Write(pObj, TMDD_TDA18250_0x37_RF_Cal_RAM_byte_2, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            pObj->I2CMap.uBx06.bF.DataRAM = uDataRAM;
            
            err = ddTDA18250Write(pObj, TMDD_TDA18250_0x06_Read_byte_6, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        }
        if (err == TM_OK)
        {
            /* validate the sended data on the RAM */
            pObj->I2CMap.uBx36.bF.ModeI2CRAM = 1;
            
            err = ddTDA18250Write(pObj, TMDD_TDA18250_0x36_RF_Cal_RAM_byte_1, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetLOFreq                                         */
/*                                                                            */
/* DESCRIPTION: set the MAIN fractional PLL settings.                         */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       Freq_prog_En previously set to 1                              */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetLOFreq
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                       */
    UInt32          uLO         /* I: local oscillator frequency in Hz  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;

    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        uLO /= 1000;

        /* update & write LO_Frac_Auto [31..0] */
        pObj->I2CMap.uBx0A.LO_Frac_31_to_24    = (UInt8)(uLO >> 12);
        pObj->I2CMap.uBx0B.LO_Frac_23_to_16    = (UInt8)(uLO >> 4);
        pObj->I2CMap.uBx0C.bF.LO_Frac_15_to_12 = (UInt8)(uLO); 
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x0A_Main_Divider_byte_3, 3);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));

        if (err == TM_OK)
        {
            /* set value */
            pObj->I2CMap.uBx0F.bF.Freq_prog_Start = 1;
            
            err = ddTDA18250Write(pObj, TMDD_TDA18250_0x0F_Main_Divider_byte_8, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));

            /* wait 5ms */
            err = ddTDA18250Wait(pObj, 5);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Wait(0x%08X) failed.", tUnit));
            
            /* Hardware is set to 0 automatically */
            pObj->I2CMap.uBx0F.bF.Freq_prog_Start = 0x00;
        }

        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetLOFreq                                         */
/*                                                                            */
/* DESCRIPTION: get the MAIN fractional PLL settings.                         */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetLOFreq
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                       */
    UInt32          *puLO       /* O: local oscillator frequency in Hz  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;

    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* read LO_Frac_Auto [31..0] */
        *puLO  = pObj->I2CMap.uBx0A.LO_Frac_31_to_24 << 12;
        *puLO += pObj->I2CMap.uBx0B.LO_Frac_23_to_16 << 4;
        *puLO += pObj->I2CMap.uBx0C.bF.LO_Frac_15_to_12;
        *puLO *= 1000;

        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetIdentity:                                      */
/*                                                                            */
/* DESCRIPTION: Get the Identity bit(s) status.                               */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetIdentity
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t   pObj = Null;
    tmErrorCode_t           err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x00_ID_byte, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = pObj->I2CMap.uBx00.ID_byte;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetDieTemperature:                                */
/*                                                                            */
/* DESCRIPTION: Get the die temperature measurement.                          */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetDieTemperature
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t   pObj = Null;
    tmErrorCode_t           err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            /* switch thermometer on */
            pObj->I2CMap.uBx1D.bF.TM_ON = 1;
            err = ddTDA18250Write(pObj, TMDD_TDA18250_0x1D_PSM_byte_2, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));

            if(err == TM_OK)
            {
                err = ddTDA18250Read(pObj, TMDD_TDA18250_0x01_Read_byte_1, 1);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
            }

            if(err == TM_OK)
            {
                /* get value */
                *puValue = pObj->I2CMap.uBx01.bF.TM_D;

                /* switch thermometer off */
                pObj->I2CMap.uBx1D.bF.TM_ON = 0;
                err = ddTDA18250Write(pObj, TMDD_TDA18250_0x1D_PSM_byte_2, 1);
                tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
            }
        }
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetRSSI_Alarm                                     */
/*                                                                            */
/* DESCRIPTION: Get if the power level is out of range.                       */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       Read only                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetRSSI_Alarm
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x01_Read_byte_1, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = pObj->I2CMap.uBx01.bF.RSSI_Alarm;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetLO_Lock                                        */
/*                                                                            */
/* DESCRIPTION: Get the LO synthesizer lock at the programmed frequency.      */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       Read only                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetLO_Lock
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x01_Read_byte_1, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = pObj->I2CMap.uBx01.bF.LO_Lock;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetRSSI                                           */
/*                                                                            */
/* DESCRIPTION: Set the Power level value.                                    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetRSSI
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx02.RSSI = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x02_Read_byte_2, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetRSSI                                           */
/*                                                                            */
/* DESCRIPTION: Get the Power level value.                                    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetRSSI
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;

    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x00_ID_byte, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x02_Read_byte_2, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            if (pObj->I2CMap.uBx00.ID_byte == 0xD2)
            {

                *puValue = ((pObj->I2CMap.uBx02.bF.RSSI_7 << 7) |
                            (pObj->I2CMap.uBx02.bF.RSSI_6 << 6) |
                            (pObj->I2CMap.uBx02.bF.RSSI_5 << 5) |
                            (pObj->I2CMap.uBx02.bF.RSSI_4 << 4) |
                            (pObj->I2CMap.uBx02.bF.RSSI_3 << 3) |
                            (pObj->I2CMap.uBx02.bF.RSSI_2 << 2) |
                            (pObj->I2CMap.uBx02.bF.RSSI_1 << 1) |
                            (pObj->I2CMap.uBx02.bF.RSSI_0 ));
            }
            else if ( (pObj->I2CMap.uBx00.ID_byte == 0xD3) || (pObj->I2CMap.uBx00.ID_byte == 0xD9) )
            {
                *puValue = ((pObj->I2CMap.uBx02.bF.RSSI_0 << 7) |
                            (pObj->I2CMap.uBx02.bF.RSSI_1 << 6) |
                            (pObj->I2CMap.uBx02.bF.RSSI_2 << 5) |
                            (pObj->I2CMap.uBx02.bF.RSSI_3 << 4) |
                            (pObj->I2CMap.uBx02.bF.RSSI_4 << 3) |
                            (pObj->I2CMap.uBx02.bF.RSSI_5 << 2) |
                            (pObj->I2CMap.uBx02.bF.RSSI_6 << 1) |
                            (pObj->I2CMap.uBx02.bF.RSSI_7 << 0 ));
            }
            else
            {
                *puValue = ((pObj->I2CMap.uBx02.bF.RSSI_2 << 7) |
                            (pObj->I2CMap.uBx02.bF.RSSI_1 << 6) |
                            (pObj->I2CMap.uBx02.bF.RSSI_0 << 5) |
                            (pObj->I2CMap.uBx02.bF.RSSI_3 << 4) |
                            (pObj->I2CMap.uBx02.bF.RSSI_4 << 3) |
                            (pObj->I2CMap.uBx02.bF.RSSI_5 << 2) |
                            (pObj->I2CMap.uBx02.bF.RSSI_6 << 1) |
                            (pObj->I2CMap.uBx02.bF.RSSI_7 << 0 ));
            }
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetAGC2b_Gain                                     */
/*                                                                            */
/* DESCRIPTION: Set the Gain programming or gain value of AGC2b.              */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       Gain=(decimal value)*0.2-12 dB                                */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetAGC2b_Gain
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx03.AGC2 = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x03_Read_byte_3, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetAGC2b_Gain                                     */
/*                                                                            */
/* DESCRIPTION: Get the Gain programming or gain value of AGC2b.              */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       Gain=(decimal value)*0.2-12 dB                                */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetAGC2b_Gain
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x03_Read_byte_3, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = pObj->I2CMap.uBx03.AGC2;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetAGC1_Gain                                      */
/*                                                                            */
/* DESCRIPTION: Set the Gain programming or gain value of AGC1.               */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetAGC1_Gain
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx04.bF.AGC1 = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x04_Read_byte_4, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetAGC1_Gain                                      */
/*                                                                            */
/* DESCRIPTION: Get the Gain programming or gain value of AGC1.               */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetAGC1_Gain
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x04_Read_byte_4, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = pObj->I2CMap.uBx04.bF.AGC1;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetFlagMSM_OK                                     */
/*                                                                            */
/* DESCRIPTION: Get the FlagMSM_OK bit status.                                */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       Indicates if MSM is correctly launched                        */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetFlagMSM_OK
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x05_Read_byte_5, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = pObj->I2CMap.uBx05.bF.FlagMSM_OK;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetLaunchMSM                                      */
/*                                                                            */
/* DESCRIPTION: Launch the msm with the selected hc_mode.                     */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       msm: Main State Machine                                       */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetLaunchMSM
(
    tmUnitSelect_t  tUnit       /* I: Unit number */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx05.bF.launch_msm = 1;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x05_Read_byte_5, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Hardware is set to 0 automatically */
        pObj->I2CMap.uBx05.bF.launch_msm = 0;

        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetResetFlags                                     */
/*                                                                            */
/* DESCRIPTION: Reset all the flags.                                          */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetResetFlags
(
    tmUnitSelect_t  tUnit       /* I: Unit number */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx05.bF.FlagClear = 1;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x05_Read_byte_5, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        if (err == TM_OK)
        {
            /* set value */
            pObj->I2CMap.uBx05.bF.FlagClear = 0;
            
            err = ddTDA18250Write(pObj, TMDD_TDA18250_0x05_Read_byte_5, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetResetMSM                                       */
/*                                                                            */
/* DESCRIPTION: Reset the main state machine.                                 */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetResetMSM
(
    tmUnitSelect_t  tUnit       /* I: Unit number */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Reset Main State Machine */
        pObj->I2CMap.uBx05.bF.msm_rst = 1;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x05_Read_byte_5, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));

        if (err == TM_OK)
        {
            pObj->I2CMap.uBx05.bF.msm_rst = 0;
            
            err = ddTDA18250Write(pObj, TMDD_TDA18250_0x05_Read_byte_5, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        }

        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetDataRAM                                        */
/*                                                                            */
/* DESCRIPTION: write data in the RAM.                                        */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       Word used to write data in the RAM                            */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetDataRAM
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx06.bF.DataRAM = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x06_Read_byte_6, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetFreq_prog_Start                                */
/*                                                                            */
/* DESCRIPTION: start the LO calculation.                                     */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       returns to 0 at the end. Always 0 in read mode                */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetFreq_prog_Start
(
    tmUnitSelect_t  tUnit       /* I: Unit number */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx0F.bF.Freq_prog_Start = 1;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x0F_Main_Divider_byte_8, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));

        /* wait 5ms */
        err = ddTDA18250Wait(pObj, 5);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Wait(0x%08X) failed.", tUnit));
        
        /* Hardware is set to 0 automatically */
        pObj->I2CMap.uBx0F.bF.Freq_prog_Start = 0x00;

        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetCAL_Int                                        */
/*                                                                            */
/* DESCRIPTION: set the CAL programmable main divider bits.                   */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetCAL_Int
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx11.CAL_Int = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x11_Cal_Divider_byte_2, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetSM                                             */
/*                                                                            */
/* DESCRIPTION: Set the AGC2 to IF output.                                    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       When 1, AGC2 to IF output is off, synthetizer off             */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetSM
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx17.bF.SM = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x17_Power_Down_byte_1, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetSM                                             */
/*                                                                            */
/* DESCRIPTION: Get the AGC2 to IF output.                                    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       When 1, AGC2 to IF output is off, synthetizer off             */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetSM
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x17_Power_Down_byte_1, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = pObj->I2CMap.uBx17.bF.SM;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetPD_RFAGC_Ifout                                 */
/*                                                                            */
/* DESCRIPTION: Set the Power down of the circuit from the AGC2 output to     */
/*              the IF .                                                      */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetPD_RFAGC_Ifout
(
    tmUnitSelect_t    tUnit,   /* I: Unit number      */
    tmddTDA18250_PD_t uPd      /* I: Power Down value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx17.bF.PD_RFAGC_Ifout = uPd;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x17_Power_Down_byte_1, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetpdAGC1b                                        */
/*                                                                            */
/* DESCRIPTION: Set the AGC1b.                                                */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       AGC1b is the low ohmic input impedance LNA                    */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetpdAGC1b
(
    tmUnitSelect_t    tUnit,   /* I: Unit number      */
    tmddTDA18250_PD_t uPd      /* I: Power Down value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx17.bF.pdAGC1b = uPd;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x17_Power_Down_byte_1, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetpdAGC2b                                        */
/*                                                                            */
/* DESCRIPTION: Set the AGC2b Power down.                                     */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetpdAGC2b
(
    tmUnitSelect_t    tUnit,   /* I: Unit number      */
    tmddTDA18250_PD_t uPd      /* I: Power Down value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx18.bF.pdAGC2b = uPd;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x18_Power_Down_byte_2, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetPD_RFAGC_Det                                   */
/*                                                                            */
/* DESCRIPTION: Set the Detector placed at Rfagc output.                      */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetPD_RFAGC_Det
(
    tmUnitSelect_t    tUnit,   /* I: Unit number      */
    tmddTDA18250_PD_t uPd      /* I: Power Down value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx18.bF.PD_RFAGC_Det = uPd;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x18_Power_Down_byte_2, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetPDBUF1b                                        */
/*                                                                            */
/* DESCRIPTION: Set the BUF1b power.                                          */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       BUF1b is the buffer between the AGC1b out and the digital path*/
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetPDBUF1b
(
    tmUnitSelect_t    tUnit,   /* I: Unit number      */
    tmddTDA18250_PD_t uPd      /* I: Power Down value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx18.bF.PDBUF1b = uPd;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x18_Power_Down_byte_2, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetRFSW_MTO_LT_RFin                               */
/*                                                                            */
/* DESCRIPTION: Get the LT & MTO connection.                                  */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetRFSW_MTO_LT_RFin
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x18_Power_Down_byte_2, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = pObj->I2CMap.uBx18.bF.RFSW_MTO_LT_RFin;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250Sethc_mode                                        */
/*                                                                            */
/* DESCRIPTION: Set the chip state.                                           */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       0000: normal, 0001: IRcal wanted, 0010: IRcal image,          */
/*              0011: RFcal POR, 0100: RFcal AV, 0101: Softreset              */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250Sethc_mode
(
    tmUnitSelect_t       tUnit,      /* I: Unit number   */
    tmddTDA18250HcMode_t uHc_mode    /* I: Hc mode value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx19.bF.hc_mode = uHc_mode;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x19_Xtout_Byte, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetBP_Filter                                      */
/*                                                                            */
/* DESCRIPTION: Set the filter.                                               */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       RF band-pass(H3/H5)/all-pass/high-pass(AGC2b) filter selection*/
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetBP_Filter
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx1A.bF.BP_Filter = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x1A_IF_Byte_1, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetBP_Filter                                      */
/*                                                                            */
/* DESCRIPTION: Get the filter.                                               */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       RF band-pass(H3/H5)/all-pass/high-pass(AGC2b) filter selection*/
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetBP_Filter
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x1A_IF_Byte_1, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = pObj->I2CMap.uBx1A.bF.BP_Filter;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetIF_level                                       */
/*                                                                            */
/* DESCRIPTION: Set the IF output level.                                      */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetIF_level
(
    tmUnitSelect_t          tUnit,      /* I: Unit number    */
    tmddTDA18250IF_Level_t  uLevel      /* I: IF Level value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;

    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx1A.bF.IF_level = uLevel;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x1A_IF_Byte_1, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetAGC_Low_BW                                     */
/*                                                                            */
/* DESCRIPTION: Set the AGC2 fast attack settings (UP path speed).            */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetAGC_Low_BW
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx1B.bF.AGC_Low_BW = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x1B_IF_Byte_2, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetpdAGC2a                                        */
/*                                                                            */
/* DESCRIPTION: Set the Power down of the continuous AGC (RFAGC).             */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetpdAGC2a
(
    tmUnitSelect_t    tUnit,   /* I: Unit number      */
    tmddTDA18250_PD_t uPd      /* I: Power Down value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx1B.bF.pdAGC2a = uPd;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x1B_IF_Byte_2, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetRFAGC_OpenLoop                                 */
/*                                                                            */
/* DESCRIPTION: Set the Power down of the continuous AGC (RFAGC).             */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetRFAGC_OpenLoop
(
    tmUnitSelect_t    tUnit,   /* I: Unit number      */
    tmddTDA18250_PD_t uPd      /* I: Power Down value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx1B.bF.RFAGC_OpenLoop = uPd;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x1B_IF_Byte_2, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetAGC2b_byte                                     */
/*                                                                            */
/* DESCRIPTION: Set the AGC2b_byte value.                                     */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetAGC2b_byte
(
    tmUnitSelect_t tUnit,      /* I: Unit number      */
    UInt8          uValue      /* I: Power Down value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx1C.AGC2b_byte = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x1C_AGC2b_byte, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetAGC2b_byte                                     */
/*                                                                            */
/* DESCRIPTION: Get the AGC2b_byte value                                      */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetAGC2b_byte
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x1C_AGC2b_byte, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = pObj->I2CMap.uBx1C.AGC2b_byte;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetpdAGC2_DETECT2                                 */
/*                                                                            */
/* DESCRIPTION: Set the detector before the band pass filter.                 */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetpdAGC2_DETECT2
(
    tmUnitSelect_t    tUnit,   /* I: Unit number      */
    tmddTDA18250_PD_t uPd      /* I: Power Down value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx1C.bF.pdAGC2_DETECT2 = uPd;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x1C_AGC2b_byte, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetpdAGC2_DETECT3                                 */
/*                                                                            */
/* DESCRIPTION: Set the detector after the band pass filter.                  */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetpdAGC2_DETECT3
(
    tmUnitSelect_t    tUnit,   /* I: Unit number      */
    tmddTDA18250_PD_t uPd      /* I: Power Down value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx1C.bF.pdAGC2_DETECT3 = uPd;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x1C_AGC2b_byte, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetpdAGC2_DETECT4                                 */
/*                                                                            */
/* DESCRIPTION: Set the detector in the after the IF filter.                  */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetpdAGC2_DETECT4
(
    tmUnitSelect_t    tUnit,   /* I: Unit number      */
    tmddTDA18250_PD_t uPd      /* I: Power Down value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx1C.bF.pdAGC2_DETECT4 = uPd;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x1C_AGC2b_byte, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetAGC_On                                         */
/*                                                                            */
/* DESCRIPTION: Set the clock of AGC digital control (1: normal mode)         */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetAGC_On
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx1C.bF.AGC_On = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x1C_AGC2b_byte, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250Setpulse_up_auto                                  */
/*                                                                            */
/* DESCRIPTION: Set the pulse up auto                                         */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       If 0, takes the value of Pulse_up_width                       */
/*              If 1, sets the following default values, depending hc_mode:   */
/*              When 0 => pulse_up_width = "11"; -- 2 ms                      */
/*              When 1 => pulse_up_width = "01"; -- 125 us                    */
/*              When 2 => pulse_up_width = "01"; -- 125 us                    */
/*              When 3 => pulse_up_width = "00"; -- 16 us                     */
/*              When 4 => pulse_up_width = "00"; -- 16 us                     */
/*              When others => pulse_up_width = "00"; -- 16 us                */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250Setpulse_up_auto
(
    tmUnitSelect_t       tUnit,      /* I: Unit number */
    tmddTDA18250_State_t uState      /* I: State value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx1C.bF.pulse_up_auto = uState;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x1C_AGC2b_byte, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250Getpulse_up_auto                                  */
/*                                                                            */
/* DESCRIPTION: Get the pulse up auto                                         */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       If 0, takes the value of Pulse_up_width                       */
/*              If 1, sets the following default values, depending hc_mode:   */
/*              When 0 => pulse_up_width = "11"; -- 2 ms                      */
/*              When 1 => pulse_up_width = "01"; -- 125 us                    */
/*              When 2 => pulse_up_width = "01"; -- 125 us                    */
/*              When 3 => pulse_up_width = "00"; -- 16 us                     */
/*              When 4 => pulse_up_width = "00"; -- 16 us                     */
/*              When others => pulse_up_width = "00"; -- 16 us                */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250Getpulse_up_auto
(
    tmUnitSelect_t        tUnit,   /* I: Unit number    */
    tmddTDA18250_State_t* puState  /* O: State returned */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puState == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x1C_AGC2b_byte, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puState = (tmddTDA18250_State_t)pObj->I2CMap.uBx1C.bF.pulse_up_auto;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetPSM_StoB                                       */
/*                                                                            */
/* DESCRIPTION: Set the current consumption reduction of the Stob.            */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       Not used for PSM but for clipping opt                         */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetPSM_StoB
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx1E.bF.PSM_StoB = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x1E_PSM_byte_3, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetSTOB_DC                                        */
/*                                                                            */
/* DESCRIPTION: Set the Input voltage STOB (ES3c / ES3b).                     */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       00:1.87V(nom.)/1.87V                                          */
/*              10: 2.06V / 2.06V                                             */
/*              01: 1.78V / 1.96V                                             */
/*              11 :2.05V / 1.78V                                             */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetSTOB_DC
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx1E.bF.STOB_DC = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x1E_PSM_byte_3, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetAGC1_au_ptr                                    */
/*                                                                            */
/* DESCRIPTION: Set the Select Au/Ad for gains.                               */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       0,3,4.5,6,7.5,9,10.5,12,13.5 (Au)                             */
/*              or 3,4.5,6,7.5,9,10.5,12,13.5,15 (Ad)                         */
/*              in write mode for AGC1                                        */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetAGC1_au_ptr
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx1F.bF.AGC1_au_ptr = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x1F_PSM_byte_4, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetAGC1_aud_sel                                   */
/*                                                                            */
/* DESCRIPTION: Set the Select Au or Ad in write mode for AGC1.               */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetAGC1_aud_sel
(
    tmUnitSelect_t     tUnit,   /* I: Unit number       */
    tmddTDA18250_Aud_t uAud     /* I: Up/Down selection */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx1F.bF.AGC1_aud_sel = uAud;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x1F_PSM_byte_4, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetAGC1_aud                                       */
/*                                                                            */
/* DESCRIPTION: Set the Value of Au/Ad to write for AGC1.                     */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetAGC1_aud
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx20.bF.AGC1_aud = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x20_AGC1_byte_1, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetAGC1_Gup_sel                                   */
/*                                                                            */
/* DESCRIPTION: Set UP or DOWN root to be programmed for AGC1.                */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetAGC1_Gup_sel
(
    tmUnitSelect_t     tUnit,   /* I: Unit number       */
    tmddTDA18250_Gud_t uGud     /* I: Up/Down selection */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx20.bF.AGC1_Gup_sel = uGud;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x20_AGC1_byte_1, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetAGC2_Gup_sel                                   */
/*                                                                            */
/* DESCRIPTION: Set UP or DOWN root to be programmed for AGC2.                */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetAGC2_Gup_sel
(
    tmUnitSelect_t     tUnit,   /* I: Unit number       */
    tmddTDA18250_Gud_t uGud     /* I: Up/Down selection */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx20.bF.AGC2_Gup_sel = uGud;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x20_AGC1_byte_1, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetAGC2_Gud_ptr_sel                               */
/*                                                                            */
/* DESCRIPTION: Set the RAM of detect 2,3 or 4 in the AGC2 control.           */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       00: XX, 01: detector 2, 10: detector 3, 11: detector 4        */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetAGC2_Gud_ptr_sel
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx20.bF.AGC2_Gud_ptr_sel = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x20_AGC1_byte_1, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetAGC1_Gud                                       */
/*                                                                            */
/* DESCRIPTION: Set the TOP UP/DO root for AGC1.                              */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetAGC1_Gud
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx21.bF.AGC1_Gud = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x21_AGC1_byte_2, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetRFagc_fast                                     */
/*                                                                            */
/* DESCRIPTION: Set the Rfagc_fast in normal mode.                            */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       If 1, RFagc_fast=pulse_up in normal mode                      */
/*              If 0, Rfagc_fast=0 in normal mode                             */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetRFagc_fast
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx21.bF.RFagc_fast = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x21_AGC1_byte_2, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetAGC1_Do_step                                   */
/*                                                                            */
/* DESCRIPTION: Set the AGC1 fast attack settings (DO path speed).            */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetAGC1_Do_step
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx22.bF.AGC1_Do_step = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x22_AGC1_byte_3, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetAGC1_bypass                                    */
/*                                                                            */
/* DESCRIPTION: Set the AGC1 time constant to be 0.                           */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       Depends on AGC1Speed_Up/Do.                                   */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetAGC1_bypass
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx22.bF.AGC1_bypass = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x22_AGC1_byte_3, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetAGC1_loop_off                                  */
/*                                                                            */
/* DESCRIPTION: Set the AGC1 digital loop (no more update of the gains).      */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetAGC1_loop_off
(
    tmUnitSelect_t       tUnit,      /* I: Unit number */
    tmddTDA18250_State_t uState      /* I: State value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx22.bF.AGC1_loop_off = uState;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x22_AGC1_byte_3, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetAGC2_Gud_valid                                 */
/*                                                                            */
/* DESCRIPTION: Set the AGC2 gain up or down is store in RAM.                 */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       This bit is automatically reseted by the hardware             */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetAGC2_Gud_valid
(
    tmUnitSelect_t       tUnit,      /* I: Unit number */
    tmddTDA18250_State_t uState      /* I: State value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx22.bF.AGC2_Gud_valid = uState;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x22_AGC1_byte_3, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Hardware is set to 0 automatically */
        pObj->I2CMap.uBx22.bF.AGC2_Gud_valid = 0;

        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetAGC2_Gud                                       */
/*                                                                            */
/* DESCRIPTION: Set the TOP UP/DO root for AGC2.                              */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetAGC2_Gud
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx23.bF.AGC2_Gud = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x23_AGC2_byte_1, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetAGC1_aud_valid                                 */
/*                                                                            */
/* DESCRIPTION: Set the AGC1 top Au0 or Au3 or Au4,5 or ? or Au13,5 or Ad3    */
/*              or ? Ad15 is store in RAM.                                    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       This bit is automatically reseted by the hardware             */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetAGC1_aud_valid
(
    tmUnitSelect_t       tUnit,      /* I: Unit number */
    tmddTDA18250_State_t uState      /* I: State value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx23.bF.AGC1_aud_valid = uState;

        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x23_AGC2_byte_1, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Hardware is set to 0 automatically */
        pObj->I2CMap.uBx23.bF.AGC1_aud_valid = 0;

        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetAGC1_Gud_valid                                 */
/*                                                                            */
/* DESCRIPTION: Set the AGC1 gain up or down is store in RAM.                 */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       This bit is automatically reseted by the hardware             */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetAGC1_Gud_valid
(
    tmUnitSelect_t       tUnit,      /* I: Unit number */
    tmddTDA18250_State_t uState      /* I: State value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx23.bF.AGC1_Gud_valid = uState;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x23_AGC2_byte_1, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Hardware is set to 0 automatically */
        pObj->I2CMap.uBx23.bF.AGC1_Gud_valid = 0;

        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetAGC2_bypass                                    */
/*                                                                            */
/* DESCRIPTION: Set the AGC2 time constant to be 0.                           */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       Depends on AGC2Speed_Up/Do.                                   */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetAGC2_bypass
(
    tmUnitSelect_t       tUnit,      /* I: Unit number */
    tmddTDA18250_State_t uState      /* I: State value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx24.bF.AGC2_bypass = uState;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x24_AGC2_byte_2, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetAGC2_gain_Force                                */
/*                                                                            */
/* DESCRIPTION: Set the AGC2 gain, if 1 agc2_gain=AGC2 else agc2_gain_int.    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetAGC2_gain_Force
(
    tmUnitSelect_t       tUnit,      /* I: Unit number */
    tmddTDA18250_State_t uState      /* I: State value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx24.bF.AGC2_gain_Force = uState;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x24_AGC2_byte_2, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetRFAGC_Top                                      */
/*                                                                            */
/* DESCRIPTION: Set the detection level at low pass first stage output.       */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       000: 92dBm? 111: 102dBm (step2dBm)                             */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetRFAGC_Top
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx25.bF.RFAGC_Top = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x25_Analog_AGC_byte, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetRSSI_Byte                                      */
/*                                                                            */
/* DESCRIPTION: Set the RSSI_Byte value.                                      */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetRSSI_Byte
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx27.RSSI_byte = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x27_RSSI_byte, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetRSSI_Byte                                      */
/*                                                                            */
/* DESCRIPTION: Get the RSSI_Byte value.                                      */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetRSSI_Byte
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x27_RSSI_byte, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = pObj->I2CMap.uBx27.RSSI_byte;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetIR_Freq                                        */
/*                                                                            */
/* DESCRIPTION: Set the Frequency of IR calibration.                          */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       00=>reserved, 01=>low, 10=>mid , 11=>high.                    */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetIR_Freq
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx29.bF.IR_Freq = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x29_IR_CAL_byte_2, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetBUF1b_DCloopIH                                 */
/*                                                                            */
/* DESCRIPTION: Set the DC correction loop of buffer1b.                       */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       default value: 1.                                             */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetBUF1b_DCloopIH
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx29.bF.BUF1b_DCloopIH = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x29_IR_CAL_byte_2, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetSTOB_gain                                      */
/*                                                                            */
/* DESCRIPTION: Set the STOB gain.                                            */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       If 1, STOB gain~3dB. If 0, as 0dB.                            */
/*              Use only in ES3_D2 and ES3_D3.                                */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetSTOB_gain
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx29.bF.STOB_gain = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x29_IR_CAL_byte_2, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetSTOB_gain                                      */
/*                                                                            */
/* DESCRIPTION: Get the STOB gain.                                            */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       If 1, STOB gain~3dB. If 0, as 0dB.                            */
/*              Use only in ES3_D2 and ES3_D3.                                */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetSTOB_gain
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x29_IR_CAL_byte_2, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = pObj->I2CMap.uBx29.bF.STOB_gain;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetRFcal_modeOFF                                  */
/*                                                                            */
/* DESCRIPTION: Switch off the entire filter switch.                          */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       Must be set to 1 for RFcal.                                   */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetRFcal_modeOFF
(
    tmUnitSelect_t                  tUnit,      /* I: Unit number */
    tmddTDA18250_SW_RFcal_modeOFF_t uSwitch     /* I: State value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx2D.bF.RFcal_modeOFF = uSwitch;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x2D_RF_Cal_byte_2, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetRefineAV                                       */
/*                                                                            */
/* DESCRIPTION: Set the bypass of part of calAV in order to only do           */
/*              Refine procedure.                                             */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetRefineAV
(
    tmUnitSelect_t       tUnit,      /* I: Unit number */
    tmddTDA18250_State_t uState      /* I: State value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx2D.bF.RefineAV = uState;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x2D_RF_Cal_byte_2, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetSel_3dB                                        */
/*                                                                            */
/* DESCRIPTION: Set the AGCK gain.                                            */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetSel_3dB
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx2D.bF.Sel_3dB = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x2D_RF_Cal_byte_2, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250Setcapw_filter_sel                                */
/*                                                                            */
/* DESCRIPTION: Set the the manual testing of the RF_filter using LF_nHF,     */
/*              capw_gain, capw_cad, capw_ctank.                              */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250Setcapw_filter_sel
(
    tmUnitSelect_t       tUnit,      /* I: Unit number */
    tmddTDA18250_State_t uState      /* I: State value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx2D.bF.capw_filter_sel = uState;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x2D_RF_Cal_byte_2, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetFilterIndex                                    */
/*                                                                            */
/* DESCRIPTION: Get the index filter.                                         */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       If Indx_filter_sel=0 -> give the index of the optimum filter  */
/*              after the calibration                                         */
/*              If Indx_filter_sel=1 -> set the wanted index filter           */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetFilterIndex
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x2E_RF_Cal_byte_3, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = pObj->I2CMap.uBx2E.bF.FilterIndex;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetLF_nHF                                         */
/*                                                                            */
/* DESCRIPTION: Set the LF_nHF.                                               */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       If capw_filter_sel=0 ? reading LF_nHF tells wich filter is    */
/*              used (LF or HF band)                                          */
/*              If capw_filter_sel=1 ? writing LF_nHF allow to choose the     */
/*              wanted filter band                                            */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetLF_nHF
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx2E.bF.LF_nHF = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x2E_RF_Cal_byte_3, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetFreqCut                                        */
/*                                                                            */
/* DESCRIPTION: Set the Middle frequency of the filter overlap                */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetFreqCut
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx30.bF.FreqCut = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x30_RF_Cal_byte_5, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetFreqCut                                        */
/*                                                                            */
/* DESCRIPTION: Get the Middle frequency of the filter overlap                */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       FreCut_sel=1 => FreqCut[6:0]=FreCut_réel - 128                */
/*              FreCut_sel=0 => FreqCut[6:0]=FreCut_cal/2                     */
/*              (Default value in mode manuel: 182MHz)                        */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetFreqCut
(
    tmUnitSelect_t  tUnit,      /* I: Unit number    */
    UInt32*         puValue     /* O: FreqCut in MHz */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x30_RF_Cal_byte_5, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = pObj->I2CMap.uBx30.bF.FreqCut * 2000000;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetFreqCut_sel                                    */
/*                                                                            */
/* DESCRIPTION: Set the FreqCut selection                                     */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       If 0, FreqCut calculation made by the chip                    */
/*              If 1, manual mode. FreqCut setted by FreqCut[6:0]             */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetFreqCut_sel
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx30.bF.FreqCut_sel = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x30_RF_Cal_byte_5, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetNmeasAV                                        */
/*                                                                            */
/* DESCRIPTION: Set the Measurement number made around the theoretical filter */
/*              index at each calAV (-> 2*NmeasAV+1 measurement done)         */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetNmeasAV
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx32.bF.NmeasAV = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x32_RF_Cal_byte_7, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetNmeasAV                                        */
/*                                                                            */
/* DESCRIPTION: Get the Measurement number made around the theoretical filter */
/*              index at each calAV (-> 2*NmeasAV+1 measurement done)         */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetNmeasAV
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x32_RF_Cal_byte_7, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = pObj->I2CMap.uBx32.bF.NmeasAV;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetGAIN_threshold                                 */
/*                                                                            */
/* DESCRIPTION: Set the Minimum power difference needed between 2 RSSI        */
/*              measurements to be seen by the Rfcal (AV or POR)              */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       000 -> 0, 111 -> 1.75dB,  (Step 0.25)                         */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetGAIN_threshold
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx32.bF.GAIN_threshold = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x32_RF_Cal_byte_7, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250Setcapw_gain                                      */
/*                                                                            */
/* DESCRIPTION: Set the set cap_gain of both filter in manual mode.           */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       depending on LF_nHF.                                          */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250Setcapw_gain
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx33.bF.capw_gain = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x33_RF_Cal_byte_8, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250Setcapw_cad                                       */
/*                                                                            */
/* DESCRIPTION: Set the set capw_cad of both filter in manual mode.           */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       depending on LF_nHF.                                          */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250Setcapw_cad
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx34.bF.capw_cad = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x34_RF_Cal_byte_9, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250Setcapw_ctank                                     */
/*                                                                            */
/* DESCRIPTION: Set the set capw_ctank of both filter in manual mode.         */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       depending on LF_nHF.                                          */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250Setcapw_ctank
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx35.bF.capw_ctank = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x35_RF_Cal_byte_10, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetWRI2CRAM                                       */
/*                                                                            */
/* DESCRIPTION: Set the RAM write mode.                                       */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       If 1: RAM is in write mode, if 0 RAM is in read mode.         */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetWRI2CRAM
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx36.bF.WRI2CRAM = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x36_RF_Cal_RAM_byte_1, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetModeI2CRAM                                     */
/*                                                                            */
/* DESCRIPTION: Set the validation for sended data on the RAM.                */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetModeI2CRAM
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx36.bF.ModeI2CRAM = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x36_RF_Cal_RAM_byte_1, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetSelRAM                                         */
/*                                                                            */
/* DESCRIPTION: Set the wanted RAM of the RFcal part.                         */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetSelRAM
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx36.bF.SelRAM = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x36_RF_Cal_RAM_byte_1, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetAddRAM                                         */
/*                                                                            */
/* DESCRIPTION: Set the address of the RAM.                                   */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetAddRAM
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx37.bF.AddRAM = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x37_RF_Cal_RAM_byte_2, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetDTA_NotMOCA                                    */
/*                                                                            */
/* DESCRIPTION: Set the filter output which is connected to AGC1bis.          */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       '0': Output of MOCA filter is connected to AGC1bis            */
/*              '1': Output of DTA filter is connected to AGC1bis             */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetDTA_NotMOCA
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx3B.bF.DTA_NotMOCA = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x3B_RCCAL_delay, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetAGC1_smooth_t_cst                              */
/*                                                                            */
/* DESCRIPTION: Set the time constant factor for the AGC1 smooth control.     */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       000 : no smooth transition      100 : 8                       */
/*              001: 1                          101 : 16                      */
/*              010: 2                          110 : 32                      */
/*              011: 4                          111 : 200                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetAGC1_smooth_t_cst
(
    tmUnitSelect_t          tUnit,      /* I: Unit number    */
    tmddTDA18250_TimeCst_t  uTimeCst    /* I: Time Cst value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx3C.bF.AGC1_smooth_t_cst = uTimeCst;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x3C_AGC1_smooth, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250_SetRFSW_T_CST                                    */
/*                                                                            */
/* DESCRIPTION: Set the time constant factor for the RFSW_xx                  */
/*              (Analog, HP and LP) control.                                  */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       000 : no smooth transition      100 : 8                       */
/*              001: 1                          101 : 16                      */
/*              010: 2                          110 : 32                      */
/*              011: 4                          111 : 200                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250_SetRFSW_T_CST
(
    tmUnitSelect_t          tUnit,      /* I: Unit number    */
    tmddTDA18250_TimeCst_t  uTimeCst    /* I: Time Cst value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx3C.bF.RFSW_T_CST = uTimeCst;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x3C_AGC1_smooth, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetLT_smooth_t_cst                                */
/*                                                                            */
/* DESCRIPTION: Set the time constant factor for the LT smooth control.       */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       000 : no smooth transition      100 : 8                       */
/*              001: 1                          101 : 16                      */
/*              010: 2                          110 : 32                      */
/*              011: 4                          111 : 200                     */
/*              In LT test  mode, LT_cmd[3:2] = LT_T_CST[1:0]                 */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetLT_smooth_t_cst
(
    tmUnitSelect_t          tUnit,      /* I: Unit number    */
    tmddTDA18250_TimeCst_t  uTimeCst    /* I: Time Cst value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx3D.bF.LT_smooth_t_cst = uTimeCst;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x3D_LT_smooth, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetLT_smooth_offset                               */
/*                                                                            */
/* DESCRIPTION: Set the time to the LT transition in normal mode.             */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       00: no offset, 01: 1step, 10: 2steps, 11: 3steps              */
/*              In LT test mode, LT_cmd[1:0] = LT_OFFSET[1:0]                 */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetLT_smooth_offset
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx3D.bF.LT_smooth_offset = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x3D_LT_smooth, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetHP_MTO_T_CST                                   */
/*                                                                            */
/* DESCRIPTION: Set the time constant factor for the HP_MTO smooth control.   */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       000 : no smooth transition      100 : 8                       */ 
/*              001: 1                          101 : 16                      */
/*              010: 2                          110 : 32                      */
/*              011: 4                          111 : 200                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetHP_MTO_T_CST
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t                err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx41.bF.HP_MTO_T_CST = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x41_IF_byte_1, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetHP_MTO_T_CST                                   */
/*                                                                            */
/* DESCRIPTION: Get the time constant factor for the HP_MTO smooth control.   */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       000 : no smooth transition      100 : 8                       */ 
/*              001: 1                          101 : 16                      */
/*              010: 2                          110 : 32                      */
/*              011: 4                          111 : 200                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetHP_MTO_T_CST
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t                err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x41_IF_byte_1, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = pObj->I2CMap.uBx41.bF.HP_MTO_T_CST;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetInd_Div2                                       */
/*                                                                            */
/* DESCRIPTION: Set the bit for DTA/MOCA filter.                              */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetInd_Div2
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx3D.bF.Ind_Div2 = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x3D_LT_smooth, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250Setshift_GD_offset                                */
/*                                                                            */
/* DESCRIPTION: Set the tuner mode.                                           */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       0 GD in single tuner mode                                     */
/*              1 GD in multi tuner mode                                      */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250Setshift_GD_offset
(
    tmUnitSelect_t       tUnit,      /* I: Unit number */
    tmddTDA18250_State_t uState      /* I: State value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx41.bF.shift_GD_offset = uState;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x41_IF_byte_1, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetRFSW_ANALOG                                    */
/*                                                                            */
/* DESCRIPTION: Set the analog path switch position.                          */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       If the smooth is disabled, then the software provides the     */
/*              ON/OFF information, else the hardware provides the ON/OFF     */
/*              information                                                   */
/*              '0': switch on analog path is OFF                             */
/*              '1': switch on analog path is ON                              */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetRFSW_ANALOG
(
    tmUnitSelect_t            tUnit,        /* I: Unit number */
    tmddTDA18250SwitchState_t uSwitchState  /* I: Switch State value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx43.bF.RFSW_ANALOG = uSwitchState;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x43_RFSW_SMOOTH, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetRFSW_ANALOG                                    */
/*                                                                            */
/* DESCRIPTION: Get the analog path switch position.                          */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       If the smooth is disabled, then the software provides the     */
/*              ON/OFF information, else the hardware provides the ON/OFF     */
/*              information                                                   */
/*              '0': switch on analog path is OFF                             */
/*              '1': switch on analog path is ON                              */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetRFSW_ANALOG
(
    tmUnitSelect_t             tUnit,         /* I: Unit number           */
    tmddTDA18250SwitchState_t* puSwitchState  /* O: Switch State returned */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puSwitchState == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x43_RFSW_SMOOTH, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puSwitchState = (tmddTDA18250SwitchState_t)pObj->I2CMap.uBx43.bF.RFSW_ANALOG;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetRFSW_ANALOG_MOVE                               */
/*                                                                            */
/* DESCRIPTION: Set the smooth on the analog path.                            */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       1: smooth on going on the analog path                         */
/*              0: smooth done                                                */
/*              This bit is automatically reset by the hardware when the      */
/*              smooth on the switch is finished                              */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetRFSW_ANALOG_MOVE
(
    tmUnitSelect_t       tUnit,      /* I: Unit number */
    tmddTDA18250_State_t uState      /* I: State value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx43.bF.RFSW_ANALOG_MOVE = uState;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x43_RFSW_SMOOTH, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetRFSW_LP                                        */
/*                                                                            */
/* DESCRIPTION: Set the low pass filter ON/OFF information if the smooth      */
/*              is disabled.                                                  */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       0: low pass filter is OFF                                     */
/*              1: low pass filter is ON                                      */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetRFSW_LP
(
    tmUnitSelect_t            tUnit,        /* I: Unit number        */
    tmddTDA18250SwitchState_t uSwitchState  /* I: Switch State value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx3E.bF.RFSW_LP = uSwitchState;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x3E_RF_MUX_test, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetRFSW_LP                                        */
/*                                                                            */
/* DESCRIPTION: Get the low pass filter ON/OFF information if the smooth      */
/*              is disabled.                                                  */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       0: low pass filter is OFF                                     */
/*              1: low pass filter is ON                                      */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetRFSW_LP
(
    tmUnitSelect_t             tUnit,         /* I: Unit number           */
    tmddTDA18250SwitchState_t* puSwitchState  /* O: Switch State returned */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puSwitchState == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x3E_RF_MUX_test, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puSwitchState = (tmddTDA18250SwitchState_t)pObj->I2CMap.uBx3E.bF.RFSW_LP;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetRFSW_LP_MOVE                                   */
/*                                                                            */
/* DESCRIPTION: Set the smooth on going on the low pass filter.               */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       1: smooth on going on the low pass filter (write by soft)     */
/*              0: smooth done (write by hardware part)                       */
/*              This bit is automatically reset by the hardware when the      */
/*              smooth on the switch is finished                              */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetRFSW_LP_MOVE
(
    tmUnitSelect_t       tUnit,      /* I: Unit number */
    tmddTDA18250_State_t uState      /* I: State value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx3E.bF.RFSW_LP_MOVE = uState;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x3E_RF_MUX_test, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetDTA_bypass                                     */
/*                                                                            */
/* DESCRIPTION: Set the MOCA/DTA filter.                                      */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       If set to'1', the MOCA/DTA filter and the high-pass filter    */
/*              are bypassed and the signal goes directly from buffer1b       */
/*              output to AGC2b input                                         */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetDTA_bypass
(
    tmUnitSelect_t            tUnit,        /* I: Unit number        */
    tmddTDA18250SwitchState_t uSwitchState  /* I: Switch State value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx3F.bF.DTA_bypass = uSwitchState;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x3F_DTA_Filter, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetBP_Ind1                                        */
/*                                                                            */
/* DESCRIPTION: Set the BP_Ind1 for MOCA/DTA filter.                          */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetBP_Ind1
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx3F.bF.BP_Ind1 = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x3F_DTA_Filter, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetCap_HP_Left                                    */
/*                                                                            */
/* DESCRIPTION: Set the Cap_HP_Left for MOCA/DTA filter.                      */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetCap_HP_Left
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx3F.bF.Cap_HP_Left = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x3F_DTA_Filter, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetCap_HP_Right                                   */
/*                                                                            */
/* DESCRIPTION: Set the Cap_HP_Right for MOCA/DTA filter.                     */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetCap_HP_Right
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx3F.bF.Cap_HP_Right = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x3F_DTA_Filter, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetAgc1bis_gain                                   */
/*                                                                            */
/* DESCRIPTION: Set the Gain programming of AGC1bis.                          */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       000 -> 3 dB     001    -> 6 dB                                */
/*              010 -> 9 dB     011    ->15 dB                                */
/*              100 -> 12 d     Others ->15 dB                                */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetAgc1bis_gain
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx40.bF.Agc1bis_gain = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x40_AGC1bis, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250Setpd_AGC1bis                                     */
/*                                                                            */
/* DESCRIPTION: Set the power down of AGC1bis.                                */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       AGC1bis is the amplifier located after the MOCA/DTA filter    */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250Setpd_AGC1bis
(
    tmUnitSelect_t    tUnit,   /* I: Unit number      */
    tmddTDA18250_PD_t uPd      /* I: Power Down value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx40.bF.pd_AGC1bis = uPd;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x40_AGC1bis, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250Setnotch_select                                   */
/*                                                                            */
/* DESCRIPTION: Set the notch selection.                                      */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       00 no notch         01 GD correction                          */
/*              10 digital notch    11 no notch                               */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250Setnotch_select
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx41.bF.notch_select = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x41_IF_byte_1, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetLF_freq                                        */
/*                                                                            */
/* DESCRIPTION: Set the LF filter settings.                                   */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       0000 no LF filter                                             */
/*              0001 -> 1001 LF filters selection                             */
/*              1010 -> 1111 no LF filter                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetLF_freq
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx42.bF.LF_freq = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x42_IF_byte_2, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetShift_LF_rccal                                 */
/*                                                                            */
/* DESCRIPTION: Set the Shift_LF_rccal bit.                                   */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       0 : LF_freq[3:0] from I²C is not shifted by RC cal results    */
/*              1 : LF_freq[3:0] from I²C is shifted by RC cal results        */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetShift_LF_rccal
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx42.bF.Shift_LF_rccal = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x42_IF_byte_2, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetPD_hpf                                         */
/*                                                                            */
/* DESCRIPTION: Set the PD_hpf bit status.                                    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetPD_hpf
(
    tmUnitSelect_t    tUnit,   /* I: Unit number      */
    tmddTDA18250_PD_t uPd      /* I: Power Down value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx42.bF.PD_hpf = uPd;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x42_IF_byte_2, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250Sethpf_freq                                       */
/*                                                                            */
/* DESCRIPTION: Set the HPF cutfrequency.                                     */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       00 0.4M,     01 0.85M,        10 1M,       11 1.5M            */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250Sethpf_freq
(
    tmUnitSelect_t         tUnit,     /* I: Unit number    */
    tmddTDA18250HPF_Freq_t uHPF_Freq  /* I: HPF_Freq value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx42.bF.hpf_freq = uHPF_Freq;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x42_IF_byte_2, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetRFSW_HP                                        */
/*                                                                            */
/* DESCRIPTION: Set the HP digital path switch position.                      */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       If the smooth is disabled, then the software provides the     */
/*              ON/OFF information, else the hardware provides the ON/OFF     */
/*              information                                                   */
/*              '0': switch on HP digital path is OFF                         */
/*              '1': switch on HP digital path is ON                          */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetRFSW_HP
(
    tmUnitSelect_t            tUnit,        /* I: Unit number        */
    tmddTDA18250SwitchState_t uSwitchState  /* I: Switch State value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx43.bF.RFSW_HP = uSwitchState;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x43_RFSW_SMOOTH, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetRFSW_HP                                        */
/*                                                                            */
/* DESCRIPTION: Get the HP digital path switch position.                      */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       If the smooth is disabled, then the software provides the     */
/*              ON/OFF information, else the hardware provides the ON/OFF     */
/*              information                                                   */
/*              '0': switch on HP digital path is OFF                         */
/*              '1': switch on HP digital path is ON                          */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetRFSW_HP
(
    tmUnitSelect_t             tUnit,         /* I: Unit number           */
    tmddTDA18250SwitchState_t* puSwitchState  /* O: Switch State returned */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puSwitchState == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x43_RFSW_SMOOTH, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puSwitchState = (tmddTDA18250SwitchState_t)pObj->I2CMap.uBx43.bF.RFSW_HP;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetRFSW_HP_MOVE                                   */
/*                                                                            */
/* DESCRIPTION: Set the HP digital path switch position in smooth mode.       */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetRFSW_HP_MOVE
(
    tmUnitSelect_t       tUnit,      /* I: Unit number */
    tmddTDA18250_State_t uState      /* I: State value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx43.bF.RFSW_HP_MOVE = uState;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x43_RFSW_SMOOTH, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetCap_HP_Mid                                     */
/*                                                                            */
/* DESCRIPTION: Set the Cap_HP_Mid bit status.                                */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       Used for DTA/MOCA filter                                      */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetCap_HP_Mid
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx44.bF.Cap_HP_Mid = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x44_RFSW_NOTCH, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetCap_L10n                                       */
/*                                                                            */
/* DESCRIPTION: Set the Cap_L10n bit status.                                  */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       Used for DTA/MOCA filter                                      */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetCap_L10n
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx44.bF.Cap_L10n = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x44_RFSW_NOTCH, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetCap_L6n                                        */
/*                                                                            */
/* DESCRIPTION: Set the Cap_L6n bit status.                                   */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       Used for DTA/MOCA filter                                      */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetCap_L6n
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx44.bF.Cap_L6n = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x44_RFSW_NOTCH, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetBP_Ind2                                        */
/*                                                                            */
/* DESCRIPTION: Set the BP_Ind2 bit status.                                   */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       Used for DTA/MOCA filter                                      */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetBP_Ind2
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx44.bF.BP_Ind2 = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x44_RFSW_NOTCH, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250_Setrfcal_clock_en                                */
/*                                                                            */
/* DESCRIPTION: Set the 16MHz on RFcal.                                       */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250_Setrfcal_clock_en
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx2D.bF.rfcal_clock_en = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x2D_RF_Cal_byte_2, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250_Getrfcal_clock_en                                */
/*                                                                            */
/* DESCRIPTION: Get the 16MHz on RFcal.                                       */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250_Getrfcal_clock_en
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x2D_RF_Cal_byte_2, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = pObj->I2CMap.uBx2D.bF.rfcal_clock_en;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250_SetAGC1_Gain                                     */
/*                                                                            */
/* DESCRIPTION: Set the Gain programming or gain value of AGC1.               */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250_SetAGC1_Gain
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    tmddTDA18250_AGC1_Gain_t           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx04.bF.AGC1 = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x04_Read_byte_4, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250_GetAGC1_Gain                                     */
/*                                                                            */
/* DESCRIPTION: Get the Gain programming or gain value of AGC1.               */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250_GetAGC1_Gain
(
    tmUnitSelect_t             tUnit,      /* I: Unit number                                  */
    tmddTDA18250_AGC1_Gain_t*  puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x04_Read_byte_4, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = (tmddTDA18250_AGC1_Gain_t)pObj->I2CMap.uBx04.bF.AGC1;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}



/*============================================================================*/
/* FUNCTION:    tmddTDA18250_SetAGC1_gain_Force                               */
/*                                                                            */
/* DESCRIPTION: Set the AGC1 gain, if 1 agc1_gain=AGC1 else agc1_gain_int.    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250_SetAGC1_gain_Force
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    tmddTDA18250_AGC_Mode_t           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx24.bF.AGC1_gain_Force = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x24_AGC2_byte_2, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250_GetAGC1_gain_Force                               */
/*                                                                            */
/* DESCRIPTION: Get the AGC1 gain, if 1 agc1_gain=AGC1 else agc1_gain_int.    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250_GetAGC1_gain_Force
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    tmddTDA18250_AGC_Mode_t*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x24_AGC2_byte_2, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = (tmddTDA18250_AGC_Mode_t)pObj->I2CMap.uBx24.bF.AGC1_gain_Force;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetMainPLL                                        */
/*                                                                            */
/* DESCRIPTION: get the MAIN fractional PLL settings in manual mode.          */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       Freq_prog_En previously set to 0                              */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetMainPLL
(
    tmUnitSelect_t tUnit,         /* I: Unit number                      */
    UInt32         uLO,           /* I: local oscillator frequency in Hz */
    UInt8*         puPreScaler,   /* O: PreScaler                        */
    UInt8*         puPostDiv      /* O: PostDiv                          */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;
    UInt32                       uCounter;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if ((puPreScaler == Null)||(puPostDiv== Null))
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            /* search for MAIN_Post_Div corresponding to uLO */
            uCounter = 0;
            do uCounter ++;
            while (uLO > pObj->Config.MAIN_PLL_Map[uCounter - 1].uLO_Max && uCounter < TDA18250_MAIN_PLL_MAP_NB_ROWS);

            /* get MAIN_PLL_Map */
            *puPreScaler = (pObj->Config.MAIN_PLL_Map[uCounter - 1].uPost_Div)&0x07;
            *puPreScaler += 8;

            *puPostDiv = (pObj->Config.MAIN_PLL_Map[uCounter - 1].uPost_Div)&0x70;

            *puPostDiv = *puPostDiv >> 4;
            *puPostDiv = 1 << (*puPostDiv - 1);

        }

        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }
    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetMainPLL                                        */
/*                                                                            */
/* DESCRIPTION: set the MAIN fractional PLL settings in manual mode.          */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       Freq_prog_En previously set to 0                              */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetMainPLL
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                       */
    UInt32          uLO         /* I: local oscillator frequency in Hz  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;
    UInt32                       uDiv;
    UInt32                       uCounter;

    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        if (err == TM_OK)
        {
            /* search for MAIN_Post_Div corresponding to uLO */
            uCounter = 0;
            do uCounter ++;
            while (uLO > pObj->Config.MAIN_PLL_Map[uCounter - 1].uLO_Max && uCounter < TDA18250_MAIN_PLL_MAP_NB_ROWS);
            pObj->I2CMap.uBx08.Main_Divider_byte_1 = ((UInt8)pObj->Config.MAIN_PLL_Map[uCounter - 1].uPost_Div);

            /* calculate MAIN_Div */
            uDiv = (((UInt32)(pObj->Config.MAIN_PLL_Map[uCounter - 1].uDiv) * (uLO / 1000)) << 7) / 125;
            pObj->I2CMap.uBx09.bF.LO_Int = (UInt8)(uDiv >> 16) & 0x7F;
            
            pObj->I2CMap.uBx0A.LO_Frac_31_to_24 = (UInt8)(uDiv >> 13) & 0x07;
            pObj->I2CMap.uBx0B.LO_Frac_23_to_16 = (UInt8)(uDiv >> 5);
            pObj->I2CMap.uBx0C.bF.LO_Frac_15_to_12 = (UInt8)(uDiv);

            err = ddTDA18250Write(pObj, TMDD_TDA18250_0x08_Main_Divider_byte_1, 5);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* set value */
            pObj->I2CMap.uBx0F.bF.Freq_prog_Start = 1;
            
            err = ddTDA18250Write(pObj, TMDD_TDA18250_0x0F_Main_Divider_byte_8, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));

            /* wait 5ms */
            err = ddTDA18250Wait(pObj, 5);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Wait(0x%08X) failed.", tUnit));
            
            /* Hardware is set to 0 automatically */
            pObj->I2CMap.uBx0F.bF.Freq_prog_Start = 0x00;
        }
        
    /* Release driver mutex */
    (void)ddTDA18250MutexRelease(pObj);
    }
    
    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetFreq_prog_En                                   */
/*                                                                            */
/* DESCRIPTION: set the internal calculation of the LO.                       */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       If 1, enable the internal calculation of the LO               */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetFreq_prog_En
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;

    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx07.bF.Freq_prog_En = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x07_PSM_byte_1, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
    /* Release driver mutex */
    (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetFreq_prog_En                                   */
/*                                                                            */
/* DESCRIPTION: Get the internal calculation of the LO.                       */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetFreq_prog_En
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;

    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x07_PSM_byte_1, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = pObj->I2CMap.uBx07.bF.Freq_prog_En;
        }
        
    /* Release driver mutex */
    (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetLP_Fc                                          */
/*                                                                            */
/* DESCRIPTION: Set the Low-pass filter cut-off frequency.                    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetLP_Fc
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;

    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx1B.bF.LP_Fc = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x1B_IF_Byte_2, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetLP_Fc                                          */
/*                                                                            */
/* DESCRIPTION: Get the Low-pass filter cut-off frequency.                    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetLP_Fc
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;

    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x1B_IF_Byte_2, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = pObj->I2CMap.uBx1B.bF.LP_Fc;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetHP_MTO3_MOVE                                   */
/*                                                                            */
/* DESCRIPTION: Set the smooth on going on the MTO.                           */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       1: smooth on going on the low pass filter (write by soft)     */
/*              0: smooth done (write by hardware part)                       */
/*              This bit is automatically reset by the hardware when the      */
/*              smooth on the switch is finished                              */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetHP_MTO3_MOVE
(
    tmUnitSelect_t       tUnit,      /* I: Unit number */
    tmddTDA18250_State_t uState      /* I: State value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;

    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx43.bF.HP_MTO3_MOVE = uState;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x43_RFSW_SMOOTH, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetHP_MTO2_MOVE                                   */
/*                                                                            */
/* DESCRIPTION: Set the smooth on going on the MTO.                           */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       1: smooth on going on the low pass filter (write by soft)     */
/*              0: smooth done (write by hardware part)                       */
/*              This bit is automatically reset by the hardware when the      */
/*              smooth on the switch is finished                              */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetHP_MTO2_MOVE
(
    tmUnitSelect_t       tUnit,      /* I: Unit number */
    tmddTDA18250_State_t uState      /* I: State value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;

    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx43.bF.HP_MTO2_MOVE = uState;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x43_RFSW_SMOOTH, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetHP_MTO2                                        */
/*                                                                            */
/* DESCRIPTION: Set the MTO2 high pass switch position.                       */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       If the smooth is disabled, then the software provides the     */
/*              ON/OFF information, else the hardware provides the ON/OFF     */
/*              information                                                   */
/*              0: MTO2 high pass is OFF                                      */
/*              1: MTO2 high pass is ON                                       */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetHP_MTO2
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;

    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx43.bF.HP_MTO2 = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x43_RFSW_SMOOTH, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetHP_MTO2                                        */
/*                                                                            */
/* DESCRIPTION: Get the MTO2 high pass switch position.                       */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       If the smooth is disabled, then the software provides the     */
/*              ON/OFF information, else the hardware provides the ON/OFF     */
/*              information                                                   */
/*              0: MTO2 high pass is OFF                                      */
/*              1: MTO2 high pass is ON                                       */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetHP_MTO2
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;

    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x43_RFSW_SMOOTH, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = pObj->I2CMap.uBx43.bF.HP_MTO2;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetHP_MTO3                                        */
/*                                                                            */
/* DESCRIPTION: Set the MTO2 high pass switch position.                       */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       If the smooth is disabled, then the software provides the     */
/*              ON/OFF information, else the hardware provides the ON/OFF     */
/*              information                                                   */
/*              0: MTO3 high pass is OFF                                      */
/*              1: MTO3 high pass is ON                                       */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetHP_MTO3
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;

    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx43.bF.HP_MTO3 = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x43_RFSW_SMOOTH, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetHP_MTO3                                        */
/*                                                                            */
/* DESCRIPTION: Get the MTO2 high pass switch position.                       */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       If the smooth is disabled, then the software provides the     */
/*              ON/OFF information, else the hardware provides the ON/OFF     */
/*              information                                                   */
/*              0: MTO3 high pass is OFF                                      */
/*              1: MTO3 high pass is ON                                       */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetHP_MTO3
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;

    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x43_RFSW_SMOOTH, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = pObj->I2CMap.uBx43.bF.HP_MTO3;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetHP_MTO2_MOVE                                   */
/*                                                                            */
/* DESCRIPTION: Get if smooth on going on the MTO2 high pass.                 */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       1: smooth on going on the MTO2 high pass                      */
/*              0: smooth done                                                */
/*              This bit is automatically reset by the hardware when the      */
/*              smooth on the switch is finished                              */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetHP_MTO2_MOVE
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;

    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x43_RFSW_SMOOTH, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = pObj->I2CMap.uBx43.bF.HP_MTO2_MOVE;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetHP_MTO3_MOVE                                   */
/*                                                                            */
/* DESCRIPTION: Get if smooth on going on the MTO3 high pass.                 */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       1: smooth on going on the MTO3 high pass                      */
/*              0: smooth done                                                */
/*              This bit is automatically reset by the hardware when the      */
/*              smooth on the switch is finished                              */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetHP_MTO3_MOVE
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;

    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x43_RFSW_SMOOTH, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = pObj->I2CMap.uBx43.bF.HP_MTO3_MOVE;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmddTDA18250Getagc1_smooth_transition_on                      */
/*                                                                            */
/* DESCRIPTION: Get if a smooth AGC1 transition is in progress.               */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       1 = transition running                                        */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250Getagc1_smooth_transition_on
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x3D_LT_smooth, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = pObj->I2CMap.uBx3D.bF.agc1_smooth_transition_on;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetAGC1_loop_off                                  */
/*                                                                            */
/* DESCRIPTION: Get the AGC1 digital loop (no more update of the gains).      */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetAGC1_loop_off
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x22_AGC1_byte_3, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = pObj->I2CMap.uBx22.bF.AGC1_loop_off;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetAGC2_loop_off                                  */
/*                                                                            */
/* DESCRIPTION: Set the AGC2 digital loop (no more update of the gains).      */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetAGC2_loop_off
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx24.bF.AGC2_loop_off = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x24_AGC2_byte_2, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetAGC2_loop_off                                  */
/*                                                                            */
/* DESCRIPTION: Get the AGC2 digital loop (no more update of the gains).      */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetAGC2_loop_off
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t       err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x24_AGC2_byte_2, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = pObj->I2CMap.uBx24.bF.AGC2_loop_off;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetAGC2_Speed                                     */
/*                                                                            */
/* DESCRIPTION: Set the clock of the agc2 digital control (62.5k ? 250k).     */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       Not recommended to chose Fclock> 62.5kHz                      */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetAGC2_Speed
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t       err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx21.bF.AGC2_Speed = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x21_AGC1_byte_2, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetAGC2_Speed                                     */
/*                                                                            */
/* DESCRIPTION: Get the clock of the agc2 digital control (62.5k ? 250k).     */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       Not recommended to chose Fclock> 62.5kHz                      */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetAGC2_Speed
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t       err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x21_AGC1_byte_2, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = pObj->I2CMap.uBx21.bF.AGC2_Speed;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetAGC2_Do_step                                   */
/*                                                                            */
/* DESCRIPTION: Set the AGC2 fast attack settings (DO path speed).            */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetAGC2_Do_step
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx24.bF.AGC2_Do_step = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x24_AGC2_byte_2, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetAGC2_Do_step                                   */
/*                                                                            */
/* DESCRIPTION: Get the AGC2 fast attack settings (DO path speed).            */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetAGC2_Do_step
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x24_AGC2_byte_2, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = pObj->I2CMap.uBx24.bF.AGC2_Do_step;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetAGC2_Up_step                                   */
/*                                                                            */
/* DESCRIPTION: Set the RFagc open loop mode.                                 */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetAGC2_Up_step
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx1B.bF.AGC2_Up_step = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x1B_IF_Byte_2, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetAGC2_Up_step                                   */
/*                                                                            */
/* DESCRIPTION: Get the RFagc open loop mode.                                 */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetAGC2_Up_step
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x1B_IF_Byte_2, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = pObj->I2CMap.uBx1B.bF.AGC2_Up_step;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetAGC2_Speed_Up                                  */
/*                                                                            */
/* DESCRIPTION: Set the UP path time constant while bypassing.                */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetAGC2_Speed_Up
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t       err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx24.bF.AGC2_Speed_Up = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x24_AGC2_byte_2, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetAGC2_Speed_Up                                  */
/*                                                                            */
/* DESCRIPTION: Get the UP path time constant while bypassing.                */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetAGC2_Speed_Up
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t       err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x24_AGC2_byte_2, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = pObj->I2CMap.uBx24.bF.AGC2_Speed_Up;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetRC_GD_sign                                     */
/*                                                                            */
/* DESCRIPTION: Set the shift sign.                                           */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       '0': positive shift positif (1 shift by  right)               */
/*              '1': negative shift(0 shift by left)                          */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetRC_GD_sign
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t                err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx3B.bF.RC_GD_sign = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x3B_RCCAL_delay, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetRC_GD_offset                                   */
/*                                                                            */
/* DESCRIPTION: Set the shift sign.                                           */
/*                                                                            */
/* DESCRIPTION: Get the Unsigned value of BPCal_GD shifting.                  */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetRC_GD_offset
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t                err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx3B.bF.RC_GD_offset = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x3B_RCCAL_delay, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetRC_Cal_3_to_0                                  */
/*                                                                            */
/* DESCRIPTION: Get the RC cal read code after calibration.                   */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetRC_ReadCal_3_to_0
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x26_RC_byte, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = pObj->I2CMap.uBx26.bF.RC_ReadCal_3_to_0;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetRC_Cal_4                                       */
/*                                                                            */
/* DESCRIPTION: Get the MSB RC cal read code after calibration.               */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       (concatenate this with RC_ReadCal(3:0])                       */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetRC_ReadCal_4
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x27_RSSI_byte, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = pObj->I2CMap.uBx27.bF.RC_ReadCal_4;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetXtOut                                          */
/*                                                                            */
/* DESCRIPTION: Set the Xtout mode.                                           */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       0000 Xtal OFF (and so Xtout)    0110 Square 4MHz              */
/*              0001 Xtout OFF                  0111 Sine 200mV               */
/*              0010 Square 16MHz               1000 Sine 400mV               */
/*              0011 Square Fdiv of main PLL    1001 Sine 800mV               */
/*              0100 Square Fdiv of CAL PLL     1010 Sine 1200mV              */
/*              0101 Square 8MHz                                              */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetXtOut
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t                err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx19.bF.XtOut = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x19_Xtout_Byte, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetXtOut                                          */
/*                                                                            */
/* DESCRIPTION: Get the Xtout mode.                                           */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       0000 Xtal OFF (and so Xtout)    0110 Square 4MHz              */
/*              0001 Xtout OFF                  0111 Sine 200mV               */
/*              0010 Square 16MHz               1000 Sine 400mV               */
/*              0011 Square Fdiv of main PLL    1001 Sine 800mV               */
/*              0100 Square Fdiv of CAL PLL     1010 Sine 1200mV              */
/*              0101 Square 8MHz                                              */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetXtOut
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t                err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x19_Xtout_Byte, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = pObj->I2CMap.uBx19.bF.XtOut;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetpdLT                                           */
/*                                                                            */
/* DESCRIPTION: Set the LT ouput in HZ mode.                                  */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetpdLT
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t                err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx17.bF.pdLT = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x17_Power_Down_byte_1, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetpdLT                                           */
/*                                                                            */
/* DESCRIPTION: Get the LT ouput.                                             */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetpdLT
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t                err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x17_Power_Down_byte_1, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = pObj->I2CMap.uBx17.bF.pdLT;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetpdAGC1b                                        */
/*                                                                            */
/* DESCRIPTION: Get the AGC1b.                                                */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       AGC1b is the low ohmic input impedance LNA                    */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetpdAGC1b 
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t                err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x17_Power_Down_byte_1, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = pObj->I2CMap.uBx17.bF.pdAGC1b;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetpdAGC2b                                        */
/*                                                                            */
/* DESCRIPTION: Get the AGC2b Power down status.                              */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetpdAGC2b
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t                err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x18_Power_Down_byte_2, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = pObj->I2CMap.uBx18.bF.pdAGC2b;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetManual_LT                                      */
/*                                                                            */
/* DESCRIPTION: Set the AGC1 and LT gain settings.                            */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetManual_LT
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t                err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx20.bF.Manual_LT = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x20_AGC1_byte_1, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetLoopThrough_Gain                               */
/*                                                                            */
/* DESCRIPTION: Set the Gain programming of LoopThrough.                      */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetLoopThrough_Gain
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t                err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx04.bF.LT = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x04_Read_byte_4, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetLoopThrough_Gain                               */
/*                                                                            */
/* DESCRIPTION: Get the Gain programming of LoopThrough.                      */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetLoopThrough_Gain
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t                err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x04_Read_byte_4, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = pObj->I2CMap.uBx04.bF.LT;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}




/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetManual_LT                                      */
/*                                                                            */
/* DESCRIPTION: Get the AGC1 and LT gain settings.                            */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetManual_LT
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t                err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x20_AGC1_byte_1, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = pObj->I2CMap.uBx20.bF.Manual_LT;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250Setshift                                          */
/*                                                                            */
/* DESCRIPTION: Set the High-pass frequency selection in AGC2b.               */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250Setshift
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t                err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx2A.bF.shift = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x2A_IR_CAL_byte_3, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250Getshift                                          */
/*                                                                            */
/* DESCRIPTION: Get the High-pass frequency selection in AGC2b.               */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250Getshift
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t                err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x2A_IR_CAL_byte_3, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = pObj->I2CMap.uBx2A.bF.shift;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmddTDA18250Gethpf_freql                                      */
/*                                                                            */
/* DESCRIPTION: Get the HPF cutfrequency.                                     */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       00 0.4M,     01 0.85M,        10 1M,       11 1.5M            */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250Gethpf_freq
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t                err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x42_IF_byte_2, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = pObj->I2CMap.uBx42.bF.hpf_freq;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetPD_hpf                                         */
/*                                                                            */
/* DESCRIPTION: Get the PD_hpf bit status.                                    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetPD_hpf
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t                err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x42_IF_byte_2, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = pObj->I2CMap.uBx42.bF.PD_hpf;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetIF_level                                */
/*                                                                            */
/* DESCRIPTION: Get the IF output level.                                      */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetIF_level
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t                err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x1A_IF_Byte_1, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = pObj->I2CMap.uBx1A.bF.IF_level;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetRSSI_Sense_OK                                  */
/*                                                                            */
/* DESCRIPTION: Get the RSSI_Sense_OK bit status.                             */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       Equal to zero during the RSSI measurement (for debug)         */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetRSSI_Sense_OK
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t                err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x05_Read_byte_5, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = pObj->I2CMap.uBx05.bF.RSSI_Sense_OK;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetPSM_Buf1b                                      */
/*                                                                            */
/* DESCRIPTION: Reduce the current consumption of the Buf1b                   */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetPSM_Buf1b
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx40.bF.PSM_Buf1b = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x40_AGC1bis, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetPD_LO_Synthe                                   */
/*                                                                            */
/* DESCRIPTION: Switch the main synthe off                                    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetPD_LO_Synthe
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx17.bF.PD_LO_Synthe = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x17_Power_Down_byte_1, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetpdDETECT1                                      */
/*                                                                            */
/* DESCRIPTION: Set the Detector placed at AGC1a/b output.                    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetpdDETECT1
(
    tmUnitSelect_t    tUnit,    /* I: Unit number      */
    tmddTDA18250_PD_t uPdDetect /* I: Power Down detect value */
)
{
    ptmddTDA18250Object_t pObj = Null;
    tmErrorCode_t         err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx18.bF.pdDETECT1 = uPdDetect;

        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x18_Power_Down_byte_2, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}

/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetRC_LPF_sign                                   */
/*                                                                            */
/* DESCRIPTION: Set the shift sign.                                           */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       '0': positive shift positif (1 shift by  right)               */
/*              '1': negative shift(0 shift by left)                          */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetRC_LPF_sign
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t        pObj = Null;
    tmErrorCode_t                err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx3B.bF.RC_LPF_sign = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x3B_RCCAL_delay, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetRC_LPF_sign                                   */
/*                                                                            */
/* DESCRIPTION: Get the shift sign.                                           */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:       '0': positive shift positif (1 shift by  right)               */
/*              '1': negative shift(0 shift by left)                          */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetRC_LPF_sign
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t        pObj = Null;
    tmErrorCode_t                err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x3B_RCCAL_delay, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = pObj->I2CMap.uBx3B.bF.RC_LPF_sign;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250SetRC_LPF_offset                                 */
/*                                                                            */
/* DESCRIPTION: Set the Unsigned value of BPCal_LPF shifting.                 */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250SetRC_LPF_offset
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           uValue      /* I: item value  */
)
{
    ptmddTDA18250Object_t        pObj = Null;
    tmErrorCode_t                err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* set value */
        pObj->I2CMap.uBx3B.bF.RC_LPF_offset = uValue;
        
        err = ddTDA18250Write(pObj, TMDD_TDA18250_0x3B_RCCAL_delay, 1);
        tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Write(0x%08X) failed.", tUnit));
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    tmddTDA18250GetRC_LPF_offset                                 */
/*                                                                            */
/* DESCRIPTION: Get the Unsigned value of BPCal_LPF shifting.                 */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
tmddTDA18250GetRC_LPF_offset
(
    tmUnitSelect_t  tUnit,      /* I: Unit number                                  */
    UInt8*          puValue     /* O: Address of the variable to output item value */
)
{
    ptmddTDA18250Object_t        pObj = Null;
    tmErrorCode_t                err  = TM_OK;


    /* Get a driver instance */
    err = ddTDA18250GetInstance(tUnit, &pObj);
    tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250GetInstance(0x%08X) failed.", tUnit));

    if (err == TM_OK)
    {
        /* Try to acquire driver mutex */
        err = ddTDA18250MutexAcquire(pObj, ddTDA18250_MUTEX_TIMEOUT);
    }

    if (err == TM_OK)
    {
        /* Test parameters */
        if (puValue == Null)
        {
            err = ddTDA18250_ERR_BAD_PARAMETER;
        }

        if (err == TM_OK)
        {
            err = ddTDA18250Read(pObj, TMDD_TDA18250_0x3B_RCCAL_delay, 1);
            tmASSERTExT(err, TM_OK, (DEBUGLVL_ERROR, "ddTDA18250Read(0x%08X) failed.", tUnit));
        }
        
        if (err == TM_OK)
        {
            /* get value */
            *puValue = pObj->I2CMap.uBx3B.bF.RC_LPF_offset;
        }
        
        /* Release driver mutex */
        (void)ddTDA18250MutexRelease(pObj);
    }

    return err;
}

/*============================================================================*/
/* Internal functions:                                                        */
/*============================================================================*/


/*============================================================================*/
/* FUNCTION:    ddTDA18250Write:                                              */
/*                                                                            */
/* DESCRIPTION: This function writes I2C data in the Tuner.                   */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t 
ddTDA18250Write
(
    ptmddTDA18250Object_t   pObj,           /* I: Driver object */
    UInt8                   uSubAddress,    /* I: sub address */
    UInt8                   uNbData         /* I: nb of data */
)
{
    tmErrorCode_t   err = TM_OK;
    UInt8*          pI2CMap = Null;

    /* pI2CMap initialization */
    pI2CMap = &(pObj->I2CMap.uBx00.ID_byte) + uSubAddress;

    err = POBJ_SRVFUNC_SIO.Write(pObj->tUnitW, 1, &uSubAddress, uNbData, pI2CMap);

    return err;
}


/*============================================================================*/
/* FUNCTION:    ddTDA18250Read:                                               */
/*                                                                            */
/* DESCRIPTION: This function reads I2C data in the Tuner.                    */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t 
ddTDA18250Read
(
    ptmddTDA18250Object_t   pObj,           /* I: Driver object */
    UInt8                   uSubAddress,    /* I: sub address   */
    UInt8                   uNbData         /* I: nb of data    */
)
{
    tmErrorCode_t   err = TM_OK;
    UInt8*          pI2CMap = Null;

    /* pRegister initialization */
    pI2CMap = &(pObj->I2CMap.uBx00.ID_byte) + uSubAddress;

    /* read data from the Tuner */
    err = POBJ_SRVFUNC_SIO.Read(pObj->tUnitW, 1, &uSubAddress, uNbData, pI2CMap);

    if ( uSubAddress==TMDD_TDA18250_0x1B_IF_Byte_2 )
    {
        pObj->I2CMap.uBx1B.bF.UNUSED_0x1B_1 = utmddTDA18250_UNUSED_0x1B_1;
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    ddTDA18250Wait:                                               */
/*                                                                            */
/* DESCRIPTION: This function waits for requested time.                       */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t 
ddTDA18250Wait
(
    ptmddTDA18250Object_t   pObj,   /* I: Driver object    */
    UInt32                  Time    /* I: time to wait for */
)
{
    tmErrorCode_t   err  = TM_OK;

    /* wait Time ms */
    err = POBJ_SRVFUNC_STIME.Wait(pObj->tUnit, Time);

    return err;
}


/*============================================================================*/
/* FUNCTION:    ddTDA18250MutexAcquire:                                       */
/*                                                                            */
/* DESCRIPTION: Mutex Acquisition.                                            */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
ddTDA18250MutexAcquire
(
    ptmddTDA18250Object_t   pObj,
    UInt32                  timeOut
)
{
    tmErrorCode_t   err = TM_OK;

    if (pObj->sMutex.Acquire != Null && pObj->pMutex != Null)
    {
        err = pObj->sMutex.Acquire(pObj->pMutex, timeOut);
    }

    return err;
}


/*============================================================================*/
/* FUNCTION:    ddTDA18250MutexRelease:                                       */
/*                                                                            */
/* DESCRIPTION: Mutex release.                                                */
/*                                                                            */
/* RETURN:      TM_OK if no error                                             */
/*                                                                            */
/* NOTES:                                                                     */
/*============================================================================*/
tmErrorCode_t
ddTDA18250MutexRelease
(
    ptmddTDA18250Object_t   pObj  /* I: Driver object */
)
{
    tmErrorCode_t   err = TM_OK;

    if (pObj->sMutex.Release != Null && pObj->pMutex != Null)
    {
        err = pObj->sMutex.Release(pObj->pMutex);
    }

    return err;
}

