/**
  Copyright (C) 2008 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmbslOOBIp.h
 *                %version: CFR_FEAP#4 %
 *
 * \date          %date_modified%
 *
 * \brief         Describe briefly the purpose of this file.
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
   9-JULY-2007  | A.TANT    |       |       | CREATION OF OOBIp ARCHITECTURE 2.0.0
   -------------|-----------|-------|-------|-----------------------------------
                |           |       |       |
   -------------|-----------|-------|-------|-----------------------------------
   \endverbatim
 *
*/

#ifndef _TMBSLOOBIP_H //-----------------
#define _TMBSLOOBIP_H

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*/
/*                       INCLUDE FILES                                        */
/*============================================================================*/


/*============================================================================*/
/*                       MACRO DEFINITION                                     */
/*============================================================================*/
#define TMBSL_OOBIP_NBREG        100

/* SW Error codes */
#define OOBIP_ERR_BASE               (CID_COMP_DEMOD | CID_LAYER_BSL)
#define OOBIP_ERR_COMP               (CID_COMP_DEMOD | CID_LAYER_BSL | TM_ERR_COMP_UNIQUE_START)

#define OOBIP_ERR_BAD_UNIT_NUMBER    (OOBIP_ERR_BASE + TM_ERR_BAD_UNIT_NUMBER)
#define OOBIP_ERR_NOT_INITIALIZED    (OOBIP_ERR_BASE + TM_ERR_NOT_INITIALIZED)
#define OOBIP_ERR_INIT_FAILED        (OOBIP_ERR_BASE + TM_ERR_INIT_FAILED)
#define OOBIP_ERR_BAD_PARAMETER      (OOBIP_ERR_BASE + TM_ERR_BAD_PARAMETER)
#define OOBIP_ERR_NOT_SUPPORTED      (OOBIP_ERR_BASE + TM_ERR_NOT_SUPPORTED)
#define OOBIP_ERR_HW_FAILED          (OOBIP_ERR_COMP + 0x0001)
#define OOBIP_ERR_NOT_READY          (OOBIP_ERR_COMP + 0x0002)
#define OOBIP_ERR_BAD_CRC            (OOBIP_ERR_COMP + 0x0003)
#define OOBIP_ERR_BAD_VERSION        (OOBIP_ERR_COMP + 0x0004)

    typedef enum _OOBIp_HwSampleVersion_t
    {
        OOBIP_HwSampleVersion_Unknown = 0,
        OOBIP_HwSampleVersion_ES1,
        OOBIP_HwSampleVersion_ES2,
        OOBIP_HwSampleVersion_Max
    }OOBIp_HwSampleVersion_t;
    
    typedef enum _OOBIP_PODIfcMode_t
    {
        OOBIP_PODIfcModeUnknown = 0,
        OOBIP_PODIfcModeDisabled,
        OOBIP_PODIfcModeEnabled,
        OOBIP_PODIfcModeMax
    } OOBIP_PODIfcMode_t;

    typedef enum _OOBIP_Polarity_t
    {
        OOBIP_PolarityUnknown = 0,
        OOBIP_PolarityNormal,
        OOBIP_PolarityInversed,
        OOBIP_PolarityMax
    } OOBIP_Polarity_t;

    typedef struct  _OOBIP_LockInd_t
    {
        Bool bCarrierLock;
        Bool bDemodLock;
        Bool bFECLock;
        UInt8 bLockAcc;
        UInt8 bSyncQuality;
    }OOBIP_LockInd_t;

    typedef struct  _OOBIP_AGCInd_t
    {
        Bool bAAGCSat;
        Bool bDAGCSat;
        Bool bFAGCSat;
        UInt16 bCTLAcc;
        UInt8 bBTLAcc;
        Int16 bRFSDLevel;
        UInt8 bAAGCAcc;
        UInt8 bDAGCAcc;
        UInt8 bFAGCAcc;
    }OOBIP_AGCInd_t;

/*============================================================================*/
/* Types and defines:                                                         */
/*============================================================================*/

/**
    \brief  Create an instance of the OOBIp demodulator by setting its parameters

    \param  demodUnit Demodulator unit number
    \param  psSrvFunc Structure containing the Hardware Access, the Time and the Debug functions.
              
    \return TM_OK Succeed
    \return OOBIP_ERR_NOT_INITIALIZED Error while initialisation
    \return OOBIP_ERR_BAD_PARAMETER Parameter is not correctly filled

    \note   No hardware access is done in this function
    \note   Setup parameters \n Especially:
    \note        -the hardware address of I2C registers table, \n
    \note        -the pointers of system functions,\n
    \note        -the pointers of tuner function,\n
    \note        -the tuner unit number.

    \sa     NA
 */
extern tmErrorCode_t
tmbslOOBIp_Init
(
    tmUnitSelect_t            demodUnit,
    tmbslFrontEndDependency_t *psSrvFunc
);

/**
    \brief  Destroy an instance of the OOBIp demodulator

    \param  demodUnit Demodulator unit number
    
    \return TM_OK Succeed
    \return OOBIP_ERR_NOT_INITIALIZED This unit is not initialised
    \return OOBIP_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA10025_Open)
             
    \note   NA

    \sa     NA
 */
extern tmErrorCode_t 
tmbslOOBIp_DeInit 
(
    tmUnitSelect_t demodUnit
);

/**
    \brief  Get version of this device 

    \param  pSWVersion Pointer to the software version received

    \return TM_OK Succeed
             
    \note   Values defined in the tm10024local.h file

    \sa     NA
 */
extern tmErrorCode_t   
tmbslOOBIp_GetSWVersion 
(
    ptmSWVersion_t pSWVersion
);

/**
    \brief  Program the symbol rate

    \param  demodUnit Demodulator unit number
    \param  uSR Symbol rate value expressed in bauds 

    \return TM_OK Succeed
    \return OOBIP_ERR_NOT_INITIALIZED This unit is not initialised
    \return error codes of the functions registered in tmbslTDA10025_Open
    \return OOBIP_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA10025_Open)
             
    \note   NA

    \sa     tmbslOOBIp_GetSR
    \sa     tmbslOOBIp_GetConfiguredSR
 */
extern tmErrorCode_t
tmbslOOBIp_SetSR 
(
    tmUnitSelect_t demodUnit,
    UInt32         uSR
);

/**
    \brief  Get the current symbol rate

    \param  demodUnit Demodulator unit number
    \param  puCS Pointer on symbol rate expressed in baud

    \return TM_OK Succeed
    \return OOBIP_ERR_NOT_INITIALIZED This unit is not initialised
    \return error codes of the functions registered in tmbslTDA10025_Open
    \return OOBIP_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA10025_Open)
             
    \note   returns the SR and not the value programmed by tmbslOOBIp_SetSR

    \sa     tmbslOOBIp_SetSR
    \sa     tmbslOOBIp_GetConfiguredSR
 */
extern tmErrorCode_t
tmbslOOBIp_GetSR 
(
    tmUnitSelect_t demodUnit,
    UInt32         *puCS
);

/**
    \brief  Get the configured symbol rate

    \param  demodUnit Demodulator unit number
    \param  puCS Pointer on symbol rate expressed in baud

    \return TM_OK Succeed
    \return OOBIP_ERR_NOT_INITIALIZED This unit is not initialised
    \return error codes of the functions registered in tmbslTDA10025_Open
    \return OOBIP_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA10025_Open)
             
    \note   returns the value programmed by tmbslOOBIp_SetSR

    \sa     tmbslOOBIp_SetSR
    \sa     tmbslOOBIp_GetSR
 */
extern tmErrorCode_t
tmbslOOBIp_GetConfiguredSR 
(
    tmUnitSelect_t demodUnit,
    UInt32         *puCS
);

/**
    \brief  Program the spectral inversion

    \param  demodUnit Demodulator unit number
    \param  eSI Spectral Inversion

    \return TM_OK Succeed
    \return OOBIP_ERR_NOT_INITIALIZED This unit is not initialised
    \return error codes of the functions registered in tmbslTDA10025_Open
    \return OOBIP_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA10025_Open)

    \note   NA

    \sa     NA
 */
extern tmErrorCode_t
tmbslOOBIp_SetSI 
(
    tmUnitSelect_t      demodUnit,
    tmFrontEndSpecInv_t eSI
);

/**
    \brief  Read the spectral inversion

    \param  demodUnit Demodulator unit number
    \param  peSI Pointer on the Spectral Inversion

    \return TM_OK Succeed
    \return OOBIP_ERR_NOT_INITIALIZED This unit is not initialised
    \return error codes of the functions registered in tmbslTDA10025_Open
    \return OOBIP_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA10025_Open)
             
    \note   NA

    \sa     NA
 */
extern tmErrorCode_t
tmbslOOBIp_GetSI 
(
    tmUnitSelect_t      demodUnit,
    tmFrontEndSpecInv_t *peSI
);

/**
    \brief  Program the modulation

    \param  demodUnit Demodulator unit number
    \param  eMod Modulation type

    \return TM_OK Succeed
    \return OOBIP_ERR_NOT_INITIALIZED This unit is not initialised
    \return error codes of the functions registered in tmbslTDA10025_Open
    \return OOBIP_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA10025_Open)
             
    \note   only BPSK and QPSK are allowed

    \sa     NA
 */
extern tmErrorCode_t
tmbslOOBIp_SetMod 
(
    tmUnitSelect_t         demodUnit,
    tmFrontEndModulation_t eMod
);

/**
    \brief  Get the current modulation 

    \param  demodUnit Demodulator unit number
    \param  peMod Pointer on modulation type

    \return TM_OK Succeed
    \return OOBIP_ERR_NOT_INITIALIZED This unit is not initialised
    \return error codes of the functions registered in tmbslTDA10025_Open
    \return OOBIP_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA10025_Open)
             
    \note   NA

    \sa     NA
 */
extern tmErrorCode_t
tmbslOOBIp_GetMod 
(
    tmUnitSelect_t         demodUnit,
    tmFrontEndModulation_t *peMod
);

/**
    \brief  Calculate the signal to noise ratio in dB 

    \param  demodUnit Demodulator unit number
    \param  pbEsNo Pointer on channel SNR

    \return TM_OK Succeed
    \return OOBIP_ERR_NOT_INITIALIZED This unit is not initialised
    \return error codes of the functions registered in tmbslTDA10025_Open
    \return OOBIP_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA10025_Open)
                     
    \note   NA

    \sa     NA
 */
extern tmErrorCode_t
tmbslOOBIp_GetEsNo 
(
    tmUnitSelect_t demodUnit,
    UInt8         *pbEsNo
);

/**
    \brief  Get the Lock Status 

    \param  demodUnit Demodulator unit number
    \param  peLockStatus Pointer on lock status

    \return TM_OK Succeed
    \return OOBIP_ERR_NOT_INITIALIZED This unit is not initialised
    \return error codes of the functions registered in tmbslTDA10025_Open
    \return OOBIP_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA10025_Open)
                     
    \note   NA

    \sa     NA
 */
extern tmErrorCode_t
tmbslOOBIp_GetLockStatus
(
    tmUnitSelect_t       demodUnit,
    tmbslFrontEndState_t *peLockStatus
);

/**
    \brief  Get the HW ES version

    \param  demodUnit Demodulator unit number
    \param  peHwSample

    \return TM_OK Succeed
    \return OOBIP_ERR_NOT_INITIALIZED This unit is not initialised
    \return error codes of the functions registered in tmbslTDA10025_Open
    \return OOBIP_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA10025_Open)
                     
    \note   NA

    \sa     NA
 */
extern tmErrorCode_t
tmbslOOBIp_GetHwSample
(
    tmUnitSelect_t tUnit,
    OOBIp_HwSampleVersion_t *peHwSample
);

/**
    \brief   

    \param  demodUnit Demodulator unit number

    \return TM_OK Succeed
    \return OOBIP_ERR_NOT_INITIALIZED This unit is not initialised
    \return error codes of the functions registered in tmbslTDA10025_Open
    \return OOBIP_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA10025_Open)
                     
    \note   NA

    \sa     NA
 */
extern tmErrorCode_t
tmbslOOBIp_Reset
(
    tmUnitSelect_t tUnit
);

/**
    \brief   

    \param  demodUnit Demodulator unit number
    \param  psAGCInd

    \return TM_OK Succeed
    \return OOBIP_ERR_NOT_INITIALIZED This unit is not initialised
    \return error codes of the functions registered in tmbslTDA10025_Open
    \return OOBIP_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA10025_Open)
                     
    \note   NA

    \sa     NA
 */
extern tmErrorCode_t
tmbslOOBIp_GetAGCInd
(
    tmUnitSelect_t  tUnit,
    OOBIP_AGCInd_t *psAGCInd
);

/**
    \brief   

    \param  demodUnit Demodulator unit number
    \param  uIF

    \return TM_OK Succeed
    \return OOBIP_ERR_NOT_INITIALIZED This unit is not initialised
    \return error codes of the functions registered in tmbslTDA10025_Open
    \return OOBIP_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA10025_Open)
                     
    \note   NA

    \sa     NA
 */
extern tmErrorCode_t
tmbslOOBIp_SetIF
(
    tmUnitSelect_t tUnit,
    UInt32         uIF
);

/**
    \brief   

    \param  demodUnit Demodulator unit number
    \param  puIFOffset

    \return TM_OK Succeed
    \return OOBIP_ERR_NOT_INITIALIZED This unit is not initialised
    \return error codes of the functions registered in tmbslTDA10025_Open
    \return OOBIP_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA10025_Open)
                     
    \note   NA

    \sa     NA
 */
extern tmErrorCode_t
tmbslOOBIp_GetIFOffset
(
    tmUnitSelect_t tUnit,
    Int32         *puIFOffset
);

/**
    \brief   

    \param  demodUnit Demodulator unit number
    \param  puIF

    \return TM_OK Succeed
    \return OOBIP_ERR_NOT_INITIALIZED This unit is not initialised
    \return error codes of the functions registered in tmbslTDA10025_Open
    \return OOBIP_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA10025_Open)
                     
    \note   NA

    \sa     NA
 */
extern tmErrorCode_t
tmbslOOBIp_GetConfiguredIF
(
    tmUnitSelect_t tUnit,
    UInt32         *puIF
);

/**
    \brief   

    \param  demodUnit Demodulator unit number
    \param  eMode

    \return TM_OK Succeed
    \return OOBIP_ERR_NOT_INITIALIZED This unit is not initialised
    \return error codes of the functions registered in tmbslTDA10025_Open
    \return OOBIP_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA10025_Open)
                     
    \note   NA

    \sa     NA
 */
extern tmErrorCode_t
tmbslOOBIp_SetFECMode
(
    tmUnitSelect_t         tUnit,
    tmFrontEndFECMode_t    eMode
);

/**
    \brief   

    \param  demodUnit Demodulator unit number
    \param  peMode

    \return TM_OK Succeed
    \return OOBIP_ERR_NOT_INITIALIZED This unit is not initialised
    \return error codes of the functions registered in tmbslTDA10025_Open
    \return OOBIP_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA10025_Open)
                     
    \note   NA

    \sa     NA
 */
extern tmErrorCode_t
tmbslOOBIp_GetFECMode
(
    tmUnitSelect_t       tUnit,
    tmFrontEndFECMode_t *peMode
);

/**
    \brief   

    \param  demodUnit Demodulator unit number

    \return TM_OK Succeed
    \return OOBIP_ERR_NOT_INITIALIZED This unit is not initialised
    \return error codes of the functions registered in tmbslTDA10025_Open
    \return OOBIP_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA10025_Open)
                     
    \note   NA

    \sa     NA
 */
extern tmErrorCode_t
tmbslOOBIp_StartLock
(
    tmUnitSelect_t tUnit
);

/**
    \brief   

    \param  demodUnit Demodulator unit number
    \param  psLockInd

    \return TM_OK Succeed
    \return OOBIP_ERR_NOT_INITIALIZED This unit is not initialised
    \return error codes of the functions registered in tmbslTDA10025_Open
    \return OOBIP_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA10025_Open)
                     
    \note   NA

    \sa     NA
 */
extern tmErrorCode_t
tmbslOOBIp_GetLockInd
(
    tmUnitSelect_t       tUnit,
    OOBIP_LockInd_t     *psLockInd
);

/**
    \brief   

    \param  demodUnit Demodulator unit number
    \param  psMER

    \return TM_OK Succeed
    \return OOBIP_ERR_NOT_INITIALIZED This unit is not initialised
    \return error codes of the functions registered in tmbslTDA10025_Open
    \return OOBIP_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA10025_Open)
                     
    \note   MER in dB is 10*Math::Log10(dMER) 
            with dMER = (double)sMER.lInteger/sMER.uDivider;
            if sMER.uDivider == 0, MER in dB is 30dB

    \sa     NA
 */
extern tmErrorCode_t
tmbslOOBIp_GetMER
(
    tmUnitSelect_t        tUnit,
    tmFrontEndFracNb32_t *psMER
);

/**
    \brief   Get corrected and uncorrected packets number

    \param  demodUnit Demodulator unit number
    \param  puUncors Pointer to the number of uncorrected packets 
    \param  puCorrected Pointer to the number of corrected packets

    \return TM_OK Succeed
    \return OOBIP_ERR_NOT_INITIALIZED This unit is not initialised
    \return error codes of the functions registered in tmbslTDA10025_Open
    \return OOBIP_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA10025_Open)
                     
    \note   NA

    \sa     NA
 */
extern tmErrorCode_t
tmbslOOBIp_GetUncors
(
    tmUnitSelect_t        tUnit,
    UInt32               *puUncors,
    UInt32               *puCorrected
);

/**
    \brief  Reset uncor counter

    \param  demodUnit Demodulator unit number

    \return TM_OK Succeed
    \return OOBIP_ERR_NOT_INITIALIZED This unit is not initialised
    \return error codes of the functions registered in tmbslTDA10025_Open
    \return OOBIP_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA10025_Open)
                     
    \note   NA

    \sa     NA
 */
extern tmErrorCode_t
tmbslOOBIp_ClearUncors
(
    tmUnitSelect_t       tUnit
);

/**
    \brief  Get IQ  

    \param  demodUnit Demodulator unit number
    \param  psI
    \param  psQ

    \return TM_OK Succeed
    \return OOBIP_ERR_NOT_INITIALIZED This unit is not initialised
    \return error codes of the functions registered in tmbslTDA10025_Open
    \return OOBIP_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA10025_Open)
                     
    \note   NA

    \sa     NA
 */
extern tmErrorCode_t
tmbslOOBIp_GetIQ
(
    tmUnitSelect_t        tUnit,
    tmFrontEndFracNb8_t  *psI,
    tmFrontEndFracNb8_t  *psQ
);

/**
    \brief  Set Clocks

    \param  demodUnit Demodulator unit number
    \param  uSamplingClock

    \return TM_OK Succeed
    \return OOBIP_ERR_NOT_INITIALIZED This unit is not initialised
    \return error codes of the functions registered in tmbslTDA10025_Open
    \return OOBIP_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA10025_Open)
                     
    \note   NA

    \sa     NA
 */
extern tmErrorCode_t
tmbslOOBIp_SetClocks
(
    tmUnitSelect_t       tUnit,
    UInt32               uSamplingClock
);

/**
    \brief  Get clocks

    \param  demodUnit Demodulator unit number
    \param  puSamplingClock

    \return TM_OK Succeed
    \return OOBIP_ERR_NOT_INITIALIZED This unit is not initialised
    \return error codes of the functions registered in tmbslTDA10025_Open
    \return OOBIP_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA10025_Open)
                     
    \note   NA

    \sa     NA
 */
extern tmErrorCode_t
tmbslOOBIp_GetClocks
(
    tmUnitSelect_t       tUnit,
    UInt32               *puSamplingClock
);

/**
    \brief  set POD interface configuration

    \param  demodUnit Demodulator unit number
    \param  ePODIfcMode

    \return TM_OK Succeed
    \return OOBIP_ERR_NOT_INITIALIZED This unit is not initialised
    \return error codes of the functions registered in tmbslTDA10025_Open
    \return OOBIP_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA10025_Open)
                     
    \note   NA

    \sa     NA
 */
extern tmErrorCode_t
tmbslOOBIp_SetPODInterfaceMode
(
    tmUnitSelect_t     tUnit,
    OOBIP_PODIfcMode_t ePODIfcMode
);

/**
    \brief  Get POD interface configuration

    \param  demodUnit Demodulator unit number
    \param  pePODIfcMode

    \return TM_OK Succeed
    \return OOBIP_ERR_NOT_INITIALIZED This unit is not initialised
    \return error codes of the functions registered in tmbslTDA10025_Open
    \return OOBIP_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA10025_Open)
                     
    \note   NA

    \sa     NA
 */
extern tmErrorCode_t
tmbslOOBIp_GetPODInterfaceMode
(
    tmUnitSelect_t      tUnit,
    OOBIP_PODIfcMode_t *pePODIfcMode
);

/**
    \brief  Set the address offset matching with the version of the IP 

    \param  demodUnit Demodulator unit number
    \param  uStartAddress

    \return TM_OK Succeed
    \return OOBIP_ERR_NOT_INITIALIZED This unit is not initialised
    \return error codes of the functions registered in tmbslTDA10025_Open
    \return OOBIP_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA10025_Open)
                     
    \note   it is not mandatory to call this function, it depends on the application

    \sa     NA
 */
extern tmErrorCode_t
tmbslOOBIp_SetStartAddressOffset
(
    tmUnitSelect_t tUnit,
    UInt16         uStartAddress
);

#ifdef __cplusplus
}
#endif

#endif // TM<MODULE>_H //---------------
