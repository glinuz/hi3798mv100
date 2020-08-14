/**
  Copyright (C) 2010 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmbslTDA18204.h
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

#ifndef _TMBSL_TDA18204_H
#define _TMBSL_TDA18204_H

#ifdef __cplusplus
extern "C"
{
#endif

/*============================================================================*/
/* TDA18204 Error Codes                                                       */
/*============================================================================*/

#define TDA18204_ERR_BASE                       (CID_COMP_TUNER | CID_LAYER_BSL)
#define TDA18204_ERR_COMP                       (CID_COMP_TUNER | CID_LAYER_BSL | TM_ERR_COMP_UNIQUE_START)

#define TDA18204_ERR_BAD_UNIT_NUMBER            (TDA18204_ERR_BASE + TM_ERR_BAD_UNIT_NUMBER)
#define TDA18204_ERR_ERR_NO_INSTANCES           (TDA18204_ERR_BASE + TM_ERR_NO_INSTANCES)
#define TDA18204_ERR_NOT_INITIALIZED            (TDA18204_ERR_BASE + TM_ERR_NOT_INITIALIZED)
#define TDA18204_ERR_ALREADY_SETUP              (TDA18204_ERR_BASE + TM_ERR_ALREADY_SETUP)
#define TDA18204_ERR_INIT_FAILED                (TDA18204_ERR_BASE + TM_ERR_INIT_FAILED)
#define TDA18204_ERR_BAD_PARAMETER              (TDA18204_ERR_BASE + TM_ERR_BAD_PARAMETER)
#define TDA18204_ERR_NOT_SUPPORTED              (TDA18204_ERR_BASE + TM_ERR_NOT_SUPPORTED)
#define TDA18204_ERR_NULL_CONTROLFUNC           (TDA18204_ERR_BASE + TM_ERR_NULL_CONTROLFUNC)
#define TDA18204_ERR_UNLOCK                     (TDA18204_ERR_BASE + TM_ERR_UNLOCK)
#define TDA18204_ERR_HW_FAILED                  (TDA18204_ERR_COMP + 0x0001)
#define TDA18204_ERR_NOT_READY                  (TDA18204_ERR_COMP + 0x0002)
#define TDA18204_ERR_BAD_VERSION                (TDA18204_ERR_COMP + 0x0003)
#define TDA18204_ERR_STD_NOT_SET                (TDA18204_ERR_COMP + 0x0004)
#define TDA18204_ERR_RF_NOT_SET                 (TDA18204_ERR_COMP + 0x0005)

/*============================================================================*/
/* Types and defines:                                                         */
/*============================================================================*/

typedef enum _TDA18204PowerState_t {
    TDA18204_PowerFullStandby = 1,			/* All blocks in Power Down (except digital, reg1V8 & oscillator) */
    TDA18204_PowerFullStandbyWithGPIO,		/* All blocks in Power Down (except digital, reg1V8, GPIOs & oscillator) */
    TDA18204_PowerBufMtoLtoStandby,			/* All blocks in Power Down (Full Stand-by + LNA1 + Buf1 On) */
    TDA18204_PowerCalibrationNormalMode,	/* All blocks in Power-Down (except calibrations, used for HW init) */
	TDA18204_PowerSciNormalMode,			/* All blocks in Power-Down (except SCI Path) */
	TDA18204_PowerNbNormalMode,				/* All blocks in Power-Down (except NB Path) */
	TDA18204_PowerSciNbNormalMode,			/* All blocks on (except Ircal  path) */
    TDA18204_PowerMax
} TDA18204PowerState_t, *pTDA18204PowerState_t;

typedef enum _TDA18204EsVersion_t {
    TDA18204_EsVersion1 = 0,	        /* ES1 */
    TDA18204_EsVersion2 = 1,	        /* ES2 */
    TDA18204_EsVersionMax
} TDA18204EsVersion_t, *pTDA18204EsVersion_t;

typedef enum _TDA18204Agc1GainForce_t {
    TDA18204_Agc1GainForceOff              = 0, /*  OFF  */
    TDA18204_Agc1GainForceOn               = 1, /*  ON  */
    TDA18204_Agc1GainForceMax
} TDA18204Agc1GainForce_t, *pTDA18204Agc1GainForce_t;

typedef enum _TDA18204LTGainForce_t {
    TDA18204_LTGainForceOff              = 0, /*  OFF  */
    TDA18204_LTGainForceOn               = 1, /*  ON  */
    TDA18204_LTGainForceMax
} TDA18204LTGainForce_t, *pTDA18204LTGainForce_t;

typedef enum _TDA18204TEQSelect_t {
    TDA18204_TEQSelect15dB = 0,            /* +15dB */
    TDA18204_TEQSelect10dB,                /* +10dB */
    TDA18204_TEQSelect5dB,                 /*  +5dB */
    TDA18204_TEQSelect3dB,                 /*  +3dB */
    TDA18204_TEQSelect0dB,                 /*   0dB */
    TDA18204_TEQSelectMinus5dB,            /*  -5dB */
    TDA18204_TEQSelectMinus10dB            /* -10dB */
} TDA18204TEQSelect_t, *pTDA18204TEQSelect_t;

typedef enum _TDA18204StandardMode_t {
    TDA18204_StandardMode_Unknown = 0,                  /* Unknown standard */
    TDA18204_TUNER_QAM_6MHz,                            /* Mode Tuner, Digital TV QAM 6MHz */
    TDA18204_TUNER_QAM_8MHz,                            /* Mode Tuner, Digital TV QAM 8MHz */
    TDA18204_TUNER_OOB_2MHz,                            /* Mode Tuner, Digital TV OOB 2MHz */
    TDA18204_StandardMode_Max
} TDA18204StandardMode_t, *pTDA18204StandardMode_t;

/* Register Bit-Field Definition */
typedef struct _TDA18204_BitField_t
{
    UInt8   Address;
    UInt8   PositionInBits;
    UInt8   WidthInBits;
    UInt8   Attributes;
}
TDA18204_BitField_t, *pTDA18204_BitField_t;

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

/*=================================================================================================*/
/**
    \brief  Sets the TDA18204 standard mode preset.

    \param  tUnit  Concerned unit.
    \param  tStandardMode  Standard mode to be set. Possible values are:
							TDA18204_QAM_6MHz
							TDA18204_QAM_8MHz

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
);

/*=================================================================================================*/
/**
\brief  Gets the TDA18204 standard mode preset.

\param  tUnit  Concerned unit.
\param  pStandardMode  Pointer to the object in which the standard mode preset must be stored. 
						Possible return values are the same as tmbslTDA18204_SetStandardMode.

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
    tmUnitSelect_t            tUnit,           /*  I: Unit number */
    TDA18204StandardMode_t*   pStandardMode    /*  O: Standard mode of this device */
);

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
);

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
);

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
);

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
);

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
);

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
);

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
    Int32*          pPowerLevel /* O: Power Level in (x100) dBµV */
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
    );

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
);

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
);

#ifdef __cplusplus
}
#endif

#endif /* _TMBSL_TDA18204_H */

