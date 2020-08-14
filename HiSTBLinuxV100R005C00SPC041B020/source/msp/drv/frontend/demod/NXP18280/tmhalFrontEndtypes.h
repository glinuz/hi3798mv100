/**
 * Copyright (C) 2005 Koninklijke Philips Electronics N.V., All Rights Reserved.
 * This source code and any compilation or derivative thereof is the proprietary
 * information of Koninklijke Philips Electronics N.V. and is confidential in
 * nature. Under no circumstances is this software to be  exposed to or placed
 * under an Open Source License of any type without the expressed written
 * permission of Koninklijke Philips Electronics N.V.
 *
 * \file          tmhalFrontEndtypes.h
 *                %version: 28 %
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
    27-Mar-2008 | B.GUILLOT | 13122 | 23472 | Integrate with tmbslDvbtIp.
   -------------|-----------|-------|-------|-----------------------------------
   19-May-2005  | B.GUILLOT |       | 6884  |
   \endverbatim
 *
*/


#ifndef TMHALFRONTENDTYPES_H
#define TMHALFRONTENDTYPES_H

/*============================================================================*/
/*                       INCLUDE FILES                                        */
/*============================================================================*/
#include "tmNxTypes.h"

#include "tmFrontEnd.h"
#include "tmbslFrontEndTypes.h"

#ifdef TMBSLFRONTEND_CFG_ITEM_MAKE
#undef TMBSLFRONTEND_CFG_ITEM_MAKE
#endif
#define TMBSLFRONTEND_CFG_ITEM_MAKE(item)  tmhalFrontEnd##item
#include"tmbslFrontEndCfgItem.h" 

#ifdef __cplusplus
extern "C"
{
#endif



/*============================================================================*/
/*                       MACRO DEFINITION                                     */
/*============================================================================*/
#define TMBSL_ERR_TUNER_BASE	            0x00
#define TMBSL_DEMOD_BER_NOT_READY           0x7fffffff

#define TMBSL_MAX_CHANNEL_FRONTEND          0x08

#ifndef NULL
	#define NULL (void *)0
#endif

/*============================================================================*/
/*                       ENUM OR TYPE DEFINITION                              */
/*============================================================================*/

/*** SDE types ***/    
/* standard*/
#define tmhalFrontEndStandard_t tmFrontEndStandard_t
#define tmhalFrontEndStandardDSS tmFrontEndStandardDSS
#define tmhalFrontEndStandardDVBS tmFrontEndStandardDVBS
#define tmhalFrontEndStandardBSD tmFrontEndStandardBSD
#define tmhalFrontEndStandardDVBT tmFrontEndStandardDVBT
#define tmhalFrontEndStandardDVBC tmFrontEndStandardDVBC
#define tmhalFrontEndStandardDVBH tmFrontEndStandardDVBH
#define tmhalFrontEndStandardMax tmFrontEndStandardMax

/* spectral inversion*/
#define tmhalFrontEndSpectrumAuto tmFrontEndSpecInvAuto
#define tmhalFrontEndSpectrumNormal tmFrontEndSpecInvOff
#define tmhalFrontEndSpectrumInverted tmFrontEndSpecInvOn
#define tmhalFrontEndSpectralInvMax tmFrontEndSpecInvMax
#define tmhalFrontEndSpectrum_t tmFrontEndSpecInv_t

/* modulation*/
#define tmhalFrontEndModulationAuto tmFrontEndModulationAuto
#define tmhalFrontEndModulationBpsk tmFrontEndModulationBpsk
#define tmhalFrontEndModulationQpsk tmFrontEndModulationQpsk
#define tmhalFrontEndModulationQam4 tmFrontEndModulationQam4
#define tmhalFrontEndModulationPsk8 tmFrontEndModulationPsk8
#define tmhalFrontEndModulationQam16 tmFrontEndModulationQam16
#define tmhalFrontEndModulationQam32 tmFrontEndModulationQam32
#define tmhalFrontEndModulationQam64 tmFrontEndModulationQam64
#define tmhalFrontEndModulationQam128 tmFrontEndModulationQam128
#define tmhalFrontEndModulationQam256 tmFrontEndModulationQam256
#define tmhalFrontEndModulationQam512 tmFrontEndModulationQam512
#define tmhalFrontEndModulationQam1024 tmFrontEndModulationQam1024
#define tmhalFrontEndModulationMax tmFrontEndModulationMax
#define tmhalFrontEndModulation_t tmFrontEndModulation_t

/* viterbi rate*/
#define tmhalFrontEndDepuncRateAuto tmFrontEndCodeRateAuto
#define tmhalFrontEndDepuncRate12 tmFrontEndCodeRate_1_2
#define tmhalFrontEndDepuncRate23 tmFrontEndCodeRate_2_3
#define tmhalFrontEndDepuncRate34 tmFrontEndCodeRate_3_4
#define tmhalFrontEndDepuncRate45 tmFrontEndCodeRate_4_5
#define tmhalFrontEndDepuncRate56 tmFrontEndCodeRate_5_6
#define tmhalFrontEndDepuncRate67 tmFrontEndCodeRate_6_7
#define tmhalFrontEndDepuncRate78 tmFrontEndCodeRate_7_8
#define tmhalFrontEndDepuncRate89 tmFrontEndCodeRate_8_9
#define tmhalFrontEndDepuncRateMax tmFrontEndCodeRateMax
#define tmhalFrontEndDepuncRate_t tmFrontEndCodeRate_t

/* guard interval*/
#define tmhalFrontEndGuardIntervalAuto tmFrontEndGIAuto
#define tmhalFrontEndGuardInterval32 tmFrontEndGI_1_32
#define tmhalFrontEndGuardInterval16 tmFrontEndGI_1_16
#define tmhalFrontEndGuardInterval8 tmFrontEndGI_1_8
#define tmhalFrontEndGuardInterval4 tmFrontEndGI_1_4
#define tmhalFrontEndGuardIntervalMax tmFrontEndGIMax
#define tmhalFrontEndGuardInterval_t tmFrontEndGI_t

/* fast Fourrier transform size*/
#define tmhalFrontEndFftAuto tmFrontEndFftAuto
#define tmhalFrontEndFft2K tmFrontEndFft2K
#define tmhalFrontEndFft8K tmFrontEndFft8K
#define tmhalFrontEndFft4K tmFrontEndFft4K
#define tmhalFrontEndFftMax tmFrontEndFftMax
#define tmhalFrontEndFft_t tmFrontEndFft_t

/* hierarchy*/
#define tmhalFrontEndHierarchyAuto tmFrontEndHierAuto
#define tmhalFrontEndHierarchyNo tmFrontEndHierNo
#define tmhalFrontEndHierarchyAlpha1 tmFrontEndHierAlpha1
#define tmhalFrontEndHierarchyAlpha2 tmFrontEndHierAlpha2
#define tmhalFrontEndHierarchyAlpha4 tmFrontEndHierAlpha4
#define tmhalFrontEndHierarchyMax tmFrontEndHierMax
#define tmhalFrontEndHierarchy_t tmFrontEndHier_t

/* priority*/
#define tmhalFrontEndPriorityAuto tmFrontEndPriorAuto
#define tmhalFrontEndPriorityHigh tmFrontEndPriorHigh
#define tmhalFrontEndPriorityLow tmFrontEndPriorLow
#define tmhalFrontEndPriorityMax tmFrontEndPriorMax
#define tmhalFrontEndPriority_t tmFrontEndPrior_t

/*** tmbslFrontEnd types ***/
/* Status of the carrier lock */

#define tmhalFrontEndCarrierStatus_t tmbslFrontEndState_t
#define tmhalFrontEndCarrierUnknow tmbslFrontEndStateUnknown
#define tmhalFrontEndCarrierLock tmbslFrontEndStateLocked
#define tmhalFrontEndCarrierNoLock tmbslFrontEndStateNotLocked
#define tmhalFrontEndCarrierMax tmbslFrontEndStateMax


//#define tmhalFrontEndCarrierStatus_t tmbslFrontEndCarrierStatus_t
//#define tmhalFrontEndCarrierUnknow tmbslFrontEndCarrierUnknow
//#define tmhalFrontEndCarrierLock tmbslFrontEndCarrierLocked
//#define tmhalFrontEndCarrierNoLock tmbslFrontEndCarrierNotLocked
//#define tmhalFrontEndCarrierMax tmbslFrontEndCarrierMax





/* turbo code*/
typedef enum _tmhalFrontEndTurboCode_t {
    tmhalFrontEndTurboCodeDisabled = 0,
    tmhalFrontEndTurboCode1,
    tmhalFrontEndTurboCode2,
    tmhalFrontEndTurboCode3,
    tmhalFrontEndTurboCode4,
    tmhalFrontEndTurboCodeMax
} tmhalFrontEndTurboCode_t, *ptmhalFrontEndTurboCode_t;

/* tone burst*/
typedef enum _tmhalFrontEndToneBurst_t {
    tmhalFrontEndToneBurstDisabled = 0,
    tmhalFrontEndToneBurstSatA,
    tmhalFrontEndToneBurstSatB,
    tmhalFrontEndToneBurstMax
} tmhalFrontEndToneBurst_t, *ptmhalFrontEndToneBurst_t;

/* frequency offset*/
typedef enum _tmhalFrontEndRfOffset_t {
    tmhalFrontEndRfOffsetAuto = 0,
    tmhalFrontEndRfOffsetNull,
    tmhalFrontEndRfOffsetPlus125,
    tmhalFrontEndRfOffsetMinus125,
    tmhalFrontEndRfOffsetPlus166,
    tmhalFrontEndRfOffsetMinus166,
    tmhalFrontEndRfOffsetPlus333,
    tmhalFrontEndRfOffsetMinus333,
    tmhalFrontEndRfOffsetPlus500,
    tmhalFrontEndRfOffsetMinus500,
    tmhalFrontEndRfOffsetMax
} tmhalFrontEndRfOffset_t, *ptmhalFrontEndRfOffset_t;

/* status of the manager function*/
typedef enum _tmhalFrontEndManagerStatus_t {
    tmhalFrontEndManagerStatusCarrierOk = 0,
    tmhalFrontEndManagerStatusNoCarrier,
    tmhalFrontEndManagerStatusNotCompleted,
    tmhalFrontEndManagerStatusMax
} tmhalFrontEndManagerStatus_t, *ptmhalFrontEndManagerStatus_t;

/* Fine (but slow) autoscan or normal autoscan*/
typedef enum _tmhalFrontEndAutoScanMode_t
{
    tmhalFrontEndAutoScanNone = 0, 
    tmhalFrontEndAutoScanAverage,
    tmhalFrontEndAutoScanFine
} tmhalFrontEndAutoScanMode_t;

typedef enum _tmhalFrontEndBandType_t
{
    tmhalFrontEndBandTypeDefault = 0,
    tmhalFrontEndBandTypeSECAM_L,
    tmhalFrontEndBandTypeMax
} tmhalFrontEndBandType_t;

typedef enum _tmhalFrontEndSatPolarVoltage_t
{
    /** 0V for Analog swith, 0 volts is used     */   	
    tmhalFrontEndSat0V_E,      	
    /** 14V for the vertical polarisation usually     */
    tmhalFrontEndSat14V_E,   
    /** 18V for the horizontal polarisation usually   */              
    tmhalFrontEndSat18V_E                    
}tmhalFrontEndSatPolarVoltage_t;

typedef enum _tmhalFrontEndTunerMode_t
{
    tmhalFrontEndTunerAnalogMode_E,
    tmhalFrontEndTunerDigitalMode_E,
    tmhalFrontEndTunerModeMax_E
}tmhalFrontEndTunerMode_t;

/* Pio control */
typedef enum _tmhalFrontEndPioCtrl_t
{
      /** Pio as Input */
      tmhalFrontEndPioAsInput = 0,
      /** Pio as output */
      tmhalFrontEndPioAsOutput,
      tmhalFrontEndPioAsInOutMax
} tmhalFrontEndPioCtrl_t;




/* Structure contains flags for scan */
typedef struct _tmdlflags_t
{
	unsigned int iScan;
	unsigned int iTune;
	unsigned int iStartAlgo;
	unsigned int iStateOffset;
	unsigned int iAutoOffset;
	unsigned int iStateSI;
	unsigned int ChannelRegistered;
	unsigned int uStopFrequency;
	unsigned int iRfMax;
	unsigned int iRfMin;
	unsigned int uRf;
	tmhalFrontEndRfOffset_t iOffset;
} tmdlflags_t,*ptmdlflags_t;

typedef enum    _tmStandardMode_t
{
	tmStandardMN = 1,
	tmStandardB = 2,
	tmStandardGH = 4,
	tmStandardI = 8,
	tmStandardDK = 16,
	tmStandardL = 32,
	tmStandardLL = 64,
	tmStandardFM = 128,
}   tmStandardMode_t, *ptmStandardMode_t;

typedef struct _tmdlflagsScanAnalog_t
{
	unsigned int uF_Step;
	unsigned int uF_IF;
	unsigned int uF_Search;
	unsigned int uF_LO;
	unsigned int uF_Fine;
	unsigned int uF_Store;
	unsigned int iScan;
	unsigned int iTune;
	unsigned int iStartAlgo;
	unsigned int ChannelRegistered;
	unsigned int uStopFrequency;
	unsigned int iRfMax;
	unsigned int iRfMin;
	unsigned int uRf;
	tmhalFrontEndRfOffset_t iOffset;
} tmdlflagsScanAnalog_t,*ptmdlflagsScanAnalog_t;

/*typedef enum _tmcfgIndex_t {
		BOARD = 0,
		STANDARD = 1,
		FEINIT = 100,
		tmI2C_SWITCH=101,
		IDENTITY = 102,
		GETNBOFUNIT = 200,
		BANDWIDTH = 300,
		RFBAND = 302,
		RF,
		ID,
		POR,
		MAINLOCK,
		PLRFAGC = 310,
		POWERSTATE = 312,
		PLRUN = 350,
		TMVALUE,
		PLMODE,
		PLAGC1,
		PLAGC2,
		RFCALOK,
		IRCALOK,
		BPFILTER,
		GAINTAPER,
		LPFC,
		FMRFN,
		XTOUT,
		IFLEVEL,
		EXTENDEDREG,
		CALPOSTDIV,
		CALDIV,
		MAINPOSTDIV,
		MAINDIV,
		MAPNAME,
		MAPCOLUMN,
		MAPROW,
		MAPVALUE,
		READINIT,
		RFCAL_TCOMP,
		STATUSBYTE = 380,
		PROTOCOLWAIT = 382,
		STANDARDMODE = 400,
		tmDEMOD_STATUS=402,
		tmAFC,
		tmH_LOCK,
		IF = 517,
		PROTOCOLSTEP,
	} tmcfgIndex_t;*/





/* system function*/
typedef struct _tmhalFrontEndBslSystemFunc_t
{
    UInt32	(*SY_GetTickTime)	(void);
    UInt32	(*SY_GetTickPeriod)	(void);
    Bool    (*SY_TimerStart)    (UInt32 FeInstance_U, UInt32 Time_U);
    Bool    (*SY_TimerWait)         (UInt32 FeInstance_U);
    UInt32	(*SY_GetCommErrors)	(void);
    Bool	(*SY_Write)			(UInt32  uAddress,
                                 UInt32  uSubAddress,
                                 UInt32  uNbData,
                                 UInt32* pDataBuff);
    Bool	(*SY_WriteBit)		(UInt32  uAddress,
                                 UInt32  uSubAddress,
                                 UInt32  uMaskValue,
                                 UInt32  uValue);
    Bool	(*SY_Read)			(UInt32  uAddress,
                                 UInt32  uSubAddress,
                                 UInt32  uNbData,
                                 UInt32* pDataBuff);
    Bool	(*SY_SetDeviceAdd)		(UInt32 DeviceId,
    								 UInt32 NewAdd);
    Bool	(*SY_SetErrorEnable)	(Bool ErrorEnable);
    Bool	(*SY_SetUrt)			(void);
    Bool	(*SY_GetMatLog)			(char deviceName[],int WriteOrRead,UInt8 TabDevice[25][150]);
} tmhalFrontEndBslSystemFunc_t, *ptmhalFrontEndBslSystemFunc_t;


typedef struct _tmhalFrontEndbslSrvDebugFunc_t
{
    void        (*print)(const char*, ...);
} tmhalFrontEndbslSrvDebugFunc_t;


typedef struct _tmhalFrontEndbslSrvTimeFunc_t
{
    UInt32      (*Get)(void);           /* return current time value in ms */
    UInt32      (*Wait)(UInt32 t);      /* wait t ms without blocking sceduler, warning this function
                                           don't schedule others frontend instance */
} tmhalFrontEndbslSrvTimeFunc_t;


typedef struct _tmhalFrontEndbslSrvMathFunc_t
{
    Float32     (*log10)(Float32 x);
    Float32     (*pow)(Float32 x, Int32 y);
} tmhalFrontEndbslSrvMathFunc_t;

typedef struct _tmhalFrontEndbslSrvDemodTunerFunc_t
{
    UInt32      (*Init)(tmUnitSelect_t TunerUnit, ...);
//    UInt32      (*Read)(tmUnitSelect_t FrontEndUnit, UInt8 uSubAddress, UInt32 uNbData, UInt8* pDataBuff);
//    UInt32      (*Write)(tmUnitSelect_t FrontEndUnit, UInt8 uSubAddress, UInt32 uNbData, UInt8* pDataBuff, Bool bIndex);
    UInt32      (*SetRf)(tmUnitSelect_t FrontEndUnit, UInt32 uRf);
    UInt32      (*GetRf)(tmUnitSelect_t FrontEndUnit, UInt32* puRf);
    UInt32      (*SetPowerState)(tmUnitSelect_t FrontEndUnit, tmPowerState_t ePowerState);
    UInt32      (*SetCfg)(tmUnitSelect_t FrontEndUnit, tmhalFrontEndConfig_t uItemId, UInt32 uValue);
    UInt32      (*GetCfg)(tmUnitSelect_t FrontEndUnit, tmhalFrontEndConfig_t uItemId, UInt32* puValue);
    UInt32      (*GetRssi)(tmUnitSelect_t FrontEndUnit, Int8* pRssi, UInt32* puAgc1, UInt32* puAgc2, UInt32* puAdc);
} tmhalFrontEndbslSrvDemodTunerFunc_t;

typedef struct _tmhalFrontEndbslSrvDemodIoFunc_t
{
    UInt32      (*Read)(tmUnitSelect_t FrontEndUnit, UInt8 uSubAddress, UInt32 uNbData, UInt8* pDataBuff);
    UInt32      (*Read32)(tmUnitSelect_t FrontEndUnit, UInt32 uSubAddress, UInt32 uNbData, UInt32* pDataBuff);
    UInt32      (*Write)(tmUnitSelect_t FrontEndUnit, UInt8 uSubAddress, UInt32 uNbData, UInt8* pDataBuff);
    UInt32      (*Write32)(tmUnitSelect_t FrontEndUnit, UInt32 uSubAddress, UInt32 uNbData, UInt32* pDataBuff);
    UInt32      (*WriteBit)(tmUnitSelect_t FrontEndUnit, UInt8 uSubAddress, UInt8 uMask, UInt8 uData);
} tmhalFrontEndbslSrvDemodIoFunc_t;

typedef struct _tmhalFrontEndbslSrvTunerIoFunc_t
{
    UInt32      (*Read)(tmUnitSelect_t FrontEndUnit, UInt8 uSubAddress, UInt32 uNbData, UInt8* pDataBuff);
    UInt32      (*Read_Raw)(tmUnitSelect_t FrontEndUnit, UInt32 uNbData, UInt8* pDataBuff);
    UInt32      (*Write)(tmUnitSelect_t FrontEndUnit, UInt8 uSubAddress, UInt32 uNbData, UInt8* pDataBuff);
    UInt32      (*Write_Raw)(tmUnitSelect_t FrontEndUnit, UInt32 uNbData, UInt8* pDataBuff);
} tmhalFrontEndbslSrvTunerIoFunc_t;

typedef struct _tmhalFrontEndbslSrvDemodFunc_t
{
    tmhalFrontEndbslSrvDemodIoFunc_t    sDemod;
    tmhalFrontEndbslSrvDemodTunerFunc_t sTuner;
    tmhalFrontEndbslSrvTimeFunc_t       sTime;
    tmhalFrontEndbslSrvMathFunc_t       sMath;
    tmhalFrontEndbslSrvDebugFunc_t      sDebug;
} tmhalFrontEndbslSrvDemodFunc_t;

#define tmhalFrontEndbslSrvFunc_t   tmhalFrontEndbslSrvDemodFunc_t

typedef struct _tmhalFrontEndbslSrvTunerFunc_t
{
    tmhalFrontEndbslSrvTunerIoFunc_t    sTuner;
    tmhalFrontEndbslSrvTimeFunc_t       sTime;
    tmhalFrontEndbslSrvDebugFunc_t      sDebug;
} tmhalFrontEndbslSrvTunerFunc_t;

typedef struct _tmhalFrontEndAntennaDiv_t
{
    UInt32              nbSlave;
    tmUnitSelect_t      SlaveId[TMBSL_MAX_CHANNEL_FRONTEND];
} tmhalFrontEndAntennaDiv_t;

typedef struct _tmbslFrontEndDemod2TunerDependency_t
{
    UInt32 (*Init)(tmUnitSelect_t TunerUnit, ...);
    UInt32 (*SetRf)(tmUnitSelect_t FrontEndUnit, UInt32 uRf);
    UInt32 (*GetRf)(tmUnitSelect_t FrontEndUnit, UInt32* puRf);
    UInt32 (*SetPowerState)(tmUnitSelect_t FrontEndUnit, tmPowerState_t ePowerState);
    UInt32 (*SetCfg)(tmUnitSelect_t FrontEndUnit, UInt32 uItemId, UInt32 uValue);
    UInt32 (*GetCfg)(tmUnitSelect_t FrontEndUnit, UInt32 uItemId, UInt32* puValue);
    UInt32 (*GetRssi)(tmUnitSelect_t FrontEndUnit, Int8* pRssi, UInt32* puAgc1, UInt32* puAgc2, UInt32* puAdc);
}
tmbslFrontEndDemod2TunerDependency_t;

#ifdef __cplusplus
}
#endif

#endif /* TMHALFRONTENDTYPES_H */

/*============================================================================*/
/*                            END OF FILE                                     */
/*============================================================================*/


