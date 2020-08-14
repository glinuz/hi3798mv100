/*=================================================================================================*/
/**
Copyright (C) 2008 NXP B.V., All Rights Reserved.
This source code and any compilation or derivative thereof is the proprietary
information of NXP B.V. and is confidential in nature. Under no circumstances
is this software to be  exposed to or placed under an Open Source License of
any type without the expressed written permission of NXP B.V.
*
* \file          tmbslTDA18280.h
*                %version: CFR_FEAP#1 %
*
* \date          %date_modified%
*
* \brief         Describe briefly the purpose of this file.
*
* REFERENCE DOCUMENTS : TDA18280_Driver_User_Guide.pdf
*
* Detailled description may be added here.
*
* \section info Change Information
*
* \verbatim
Date          Modified by CRPRNr  TASKNr  Maintenance description
-------------|-----------|-------|-------|----------------------------------------------------------
-------------|-----------|-------|-------|----------------------------------------------------------
\endverbatim
*
*/
/*=================================================================================================*/

#pragma once
#ifndef _TMBSL_TDA18280_H
#define _TMBSL_TDA18280_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "tmbslTDA18280_InstanceCustom.h"

    /*=================================================================================================*/
    /* TDA18280 Error Codes                                                                            */
    /*=================================================================================================*/

    /** 
    * Macro to define the base starting point of TDA1%8280 SW driver errors.
    */
#define TDA18280_ERR_BASE                       (CID_COMP_TUNER | CID_LAYER_BSL)
    /** 
    * Macro to define the following error : The Tunit parameter passed in the API is not correct.
    */
#define TDA18280_ERR_BAD_UNIT_NUMBER            (TDA18280_ERR_BASE + TM_ERR_BAD_UNIT_NUMBER)
    /** 
    * Macro to define the following error : No instance available for this instance number.
    */
#define TDA18280_ERR_ERR_NO_INSTANCES           (TDA18280_ERR_BASE + TM_ERR_NO_INSTANCES)
    /** 
    * Macro to define the following error : This instance of the device has not been initialized previously.
    as required
    */
#define TDA18280_ERR_NOT_INITIALIZED            (TDA18280_ERR_BASE + TM_ERR_NOT_INITIALIZED)
    /** 
    * Macro to define the following error : This instance has already been used.
    */
#define TDA18280_ERR_ALREADY_SETUP              (TDA18280_ERR_BASE + TM_ERR_ALREADY_SETUP)
    /** 
    * Macro to define the following error : The Initialization procedure has failed.
    */
#define TDA18280_ERR_INIT_FAILED                (TDA18280_ERR_BASE + TM_ERR_INIT_FAILED)
    /** 
    * Macro to define the following error : The parameters passed in the API are not correct.
    */
#define TDA18280_ERR_BAD_PARAMETER              (TDA18280_ERR_BASE + TM_ERR_BAD_PARAMETER)
    /** 
    * Macro to define the following error : The called API is not supported by this device.
    */
#define TDA18280_ERR_NOT_SUPPORTED              (TDA18280_ERR_BASE + TM_ERR_NOT_SUPPORTED)
    /** 
    * Macro to define the following error : The provided pointer to the control function is not correct.
    */
#define TDA18280_ERR_NULL_CONTROLFUNC           (TDA18280_ERR_BASE + TM_ERR_NULL_CONTROLFUNC)
    /** 
    * Macro to define the following error : The module ID read from the device is not correct.
    */
#define TDA18280_ERR_BAD_MODULE_ID              (TDA18280_ERR_BASE + TM_ERR_BAD_MODULE_ID)
    /** 
    * Macro to define the following error : The module HW access was not wrong.
    */
#define TDA18280_ERR_HW_FAILED                  (TDA18280_ERR_BASE + 0x0001)

    /*==========================================================*/
    /*Clocking definitions:                                     */
    /*==========================================================*/

    /*=================================================================================================*/
    /* Types and defines:                                                                              */
    /*=================================================================================================*/

    /** 
    * ON/OFF selection.
    */
    typedef enum _tmTDA18280_ONOFF_t {
        tmTDA18280_OFF,     /**< The feature is OFF.*/
        tmTDA18280_ON       /**< The feature is ON.*/
    } tmTDA18280_ONOFF_t, *ptmTDA18280_ONOFF_t;

    /** 
    * Rx Equalizer CTLE Gain.
    */
    typedef enum _tmTDA18280_EqGain_t {
        tmTDA18280_EqGain_0dB=0,     /**< 0dB peak */
        tmTDA18280_EqGain_1_5dB=2,   /**< 1.5dB peak */
        tmTDA18280_EqGain_3dB=6,     /**< 3 dB peak */
        tmTDA18280_EqGain_4_5dB=9,   /**< 4.5dB peak */
        tmTDA18280_EqGain_5dB=0xb    /**< 5 dB peak */
    } tmTDA18280_EqGain_t, *ptmTDA18280_EqGain_t;


    /** 
    * Read Debug RAM mode (WB Adc or NB Adc).
    */
    typedef enum _tmTDA18280_DebugRamMode_t {
        tmTDA18280_DebugRamMode_StoreWbAdc = 0,
        tmTDA18280_DebugRamMode_StoreNbAdc
    } tmTDA18280_DebugRamMode_t, *ptmTDA18280_DebugRamMode_t;

    /** 
    * BW selection.
    */
    typedef enum _tmTDA18280_BW_t {
        tmTDA18280_2MHz=2000000,     /**< BW = 2 MHz, in Hertz (only available on ES2).*/
        tmTDA18280_5MHz=5000000,     /**< BW = 5 MHz, in Hertz (only available on ES2).*/
        tmTDA18280_6MHz=6000000,     /**< BW = 6 MHz, in Hertz.*/
        tmTDA18280_7MHz=7000000,     /**< BW = 7 MHz, in Hertz (only available on ES2).*/
        tmTDA18280_8MHz=8000000,     /**< BW = 8 MHz, in Hertz.*/
    } tmTDA18280_BW_t, *ptmTDA18280_BW_t;

    /** 
    * Frequency selection of PLL1.
    */
    typedef enum _tmTDA18280_SelPLL1FreqMHz_t {
        tmTDA18280_PLL1_5184MHz=5184,         /**< Frequency of PLL 1 at 5184 in MHz.*/
        tmTDA18280_PLL1_5400MHz=5400,         /**< Frequency of PLL 1 at 5400 in MHz.*/
        tmTDA18280_PLL1_7776MHz=7776,         /**< Frequency of PLL 1 at 7776 in MHz.*/
        tmTDA18280_PLL1_8100MHz=8100          /**< Frequency of PLL 1 at 8100 in MHz.*/
    } tmTDA18280_SelPLL1FreqMHz_t, *ptmTDA18280_SelPLL1FreqMHz_t;

    /** 
    * LO chain initialisation mode.
    */
    typedef enum _tmTDA18280_LOChainMode_t {
        tmTDA18280_LOChainMode1 = 1,  /**<             US_div = 4 / SRDes_div = 12 / Test ADC Clk / Current  = 10 */ 
        tmTDA18280_LOChainMode3 = 3,  /**<             US_div = 5 / SRDes_div = 8  / Test US Clk  / Current  = 11 */ 
        tmTDA18280_LOChainMode4 = 4,  /**<             US_div = 6 / SRDes_div = 8  / Test SD Clk  / Current  = 01 */ 
        tmTDA18280_LOChainMode5 = 5,  /**< Div3 Mode / US_div = 8 / SRDes_div = 12 / Test SD Clk  / Current  = 00 */   
        tmTDA18280_LOChainMode8 = 8,  /**< Div3 Mode / US_div = 10/ SRDes_div = 8  / Test SD Clk  / Current  = 10 */
        tmTDA18280_LOChainMode9 = 9   /**< 6GHZ test mode/  US_div = 6 / SRDes_div = 6  / Test SD Clk  / Current  = 01 */ 
    } tmTDA18280_LOChainMode_t, *ptmTDA18280_LOChainMode_t;

    /** 
    * Decimation factor of DDC.
    */
    typedef enum _tmTDA18280_deciDDC_t {
        tmTDA18280_deciDDC12 = 12,  /**< Decimation factor of DDC = 12.*/ 
        tmTDA18280_deciDDC16 = 16   /**< Decimation factor of DDC = 16.*/ 
    } tmTDA18280_deciDDC_t, *ptmTDA18280_deciDDC_t;

    /** 
    * Power States Definition.
    */
    typedef enum _tmTDA18280_PowerState_t {
        tmTDA18280_PowerWBOnly = 0,      /**<  WideBand path On (NarrowBand path Off). */
        tmTDA18280_PowerWBAndNB,         /**<  WideBand and NarrowBand paths On. */
        tmTDA18280_PowerNBOnly,          /**<  NarrowBand/Auxiliary path On (WideBand path Off). */
        tmTDA18280_PowerStandby,         /**<  All paths in Power-Down (except digital & oscillator). */
        tmTDA18280_PowerMax
    } tmTDA18280_PowerState_t, *ptmTDA18280_PowerState_t;

    /** 
    * Tilt Equalizer Definition.
    */
    typedef enum _tmTDA18280_TiltEqualizer_t {
        tmTDA18280_TiltEqualizer15dB = 0,            /**<  +15dB. */
        tmTDA18280_TiltEqualizer10dB,                /**<  +10dB. */
        tmTDA18280_TiltEqualizer5dB,                 /**<   +5dB. */
        tmTDA18280_TiltEqualizer3dB,                 /**<  +3dB.*/
        tmTDA18280_TiltEqualizer0dB,                 /**<   0dB. */
        tmTDA18280_TiltEqualizerMinus5dB,            /**<  -5dB. */
        tmTDA18280_TiltEqualizerMinus10dB            /**<  -10dB. */
    } tmTDA18280_TiltEqualizer_t, *ptmTDA18280_TiltEqualizer_t;

    /** 
    * TS Clock Configuration.
    */
    typedef enum _tmTDA18280_TsClkMode_t
    {
        tmTDA18280_TsClkGated = 0,
        tmTDA18280_TsClkPermanent
    } tmTDA18280_TsClkMode_t;

    /** 
    * TS Data Configuration.
    */
    typedef enum _tmTDA18280_TsDataOrder_t
    {
        tmTDA18280_TsDataMsbFirst = 0,
        tmTDA18280_TsDataLsbFirst
    } tmTDA18280_TsDataOrder_t;

    /** 
    * TS Sync Configuration.
    */
    typedef enum _tmTDA18280_TsSyncLength_t
    {
        tmTDA18280_TsSyncFirstByte = 0, /* long TS sync */
        tmTDA18280_TsSyncFirstBit       /* short TS sync */
    } tmTDA18280_TsSyncLength_t;

    /** 
    * Path configuration Definition.
    */
    typedef struct _tmTDA18280_PathCfg_t
    {
        tmTDA18280_ONOFF_t     tPathStatus;        /**< Path Status ON (1) or OFF (0). */
        UInt32                 uRF;                /**< if PathStatus=ON, RF frequency to use for this path in hertz. If tPathNumber is tmTDA18280_Path_Aux, this will match with the IF of the tuner.*/
        tmTDA18280_BW_t        tBW;                /**< if PathStatus=ON, BandWidth to use for this path 6000000 or 8000000 in hertz. */
        tmTDA18280_ONOFF_t     tGainAutoMode;      /**< if PathStatus=ON, 0: Auto mode OFF, 1:  Auto mode ON.*/
        UInt32                 uGainMultBy10;      /**< if PathStatus=ON, Gain for this path in dB. Read only if GainAutoMode ON, Else ReadWrite.*/
        tmFrontEndFECMode_t    eFECMode;           /**< if PathStatus=ON, Demod Configuration FEC mode */
        tmFrontEndSpecInv_t    eSI;                /**< if PathStatus=ON, Demod Configuration Spectral inversion */
        tmFrontEndModulation_t eMod;               /**< if PathStatus=ON, Demod Configuration Modulation */
        UInt32                 uSR;                /**< if PathStatus=ON, Demod Configuration Symbol Rate in Hz */
    } tmTDA18280_PathCfg_t, *ptmTDA18280_PathCfg_t;

    /** 
    * I2C gate configuration.
    */
    typedef struct _tmTDA18280_I2CGate_t
    {
        tmTDA18280_ONOFF_t  tI2CGateActivation;     /**< 0: OFF Gate inactive, 1: ON Gate active*/      
        UInt8  tI2CGateSCIAdd;                      /**< If Gate active, address of the SCI from 0 to 7*/      
    } tmTDA18280_I2CGate_t, *ptmTDA18280_I2CGate_t;

    /** 
    * Tilt Equalizer output measures.
    */
    typedef struct _tmTDA18280_TiltMeasure_t
    {
        UInt32 Frequency;                   /**< Frequency in MHz */
        UInt32 Gain;                        /**< Gain (x10) in dB */
        UInt32 GainAfterRejection;          /**< GainAfterRejection (x10) in dB */
        UInt32 GainCorrected;               /**< GainCorrected (x10) in dB */
    } tmTDA18280_TiltMeasure_t, *ptmTDA18280_TiltMeasure_t, **pptmTDA18280_TiltMeasure_t;

    /** 
    * Tilt Equalizer Input parameters.
    */
    typedef struct _tmTDA18280_TiltInputParameters_t
    {
        Bool                            bUseInputParameter;           /**< bUseInputParameter.*/
        UInt32*                         pTiltFrequencyPlan;           /**< Tilt frequency plan.*/
        UInt8*                          pTiltChannelPlan;             /**< Tilt channel plan.*/
        tmTDA18280_TiltEqualizer_t      TDA18280_DefaultTilt;         /**< Default Tilt value.*/
        Bool                            bTDA18280_RejectionEnabled;   /**< RejectionEnabled.*/
        UInt8                           uTDA18280_NbChannels;         /**< Nb of channels used in parallel for retrieving Gain */
        UInt8                           uTDA18280_MinNbChannelsNotEmpty; /**< Nb minimum of not empty channels to apply Tilt algorithm */
        UInt32                          uTDA18280_StepAverage;        /**< Nb of values for average calculation into step detection */
        Int32                           uTDA18280_SlopeThreshold;     /**< SlopeThreshold.*/
        UInt32                          uTDA18280_StepThreshold;      /**< StepThreshold in dB.*/
        UInt32                          uTDA18280_CorrelThreshold;    /**< 100 x Correlation threshold */
        UInt32                          uTDA18280_EmptyThreshold;            /**< EmptyThreshold : Gain (x 10) in dB */
        UInt32                          uTDA18280_FrequencyPercentThreshold; /**< FrequencyPercentThreshold in % */
        UInt32                          uTDA18280_StepPercentThreshold;      /**< StepPercentThreshold in % */
        UInt32                          uTDA18280_StepFreqRange;             /**< StepFreqRange in MHz */
        UInt32                          uTDA18280_Wait;                      /**< Wait in ms */
        tmTDA18280_BW_t                 eTDA18280_BandWidth;                 /**< Tuner Band Width */
    } tmTDA18280_TiltInputParameters_t, *ptmTDA18280_TiltInputParameters_t, **pptmTDA18280_TiltInputParameters_t;

    /** 
    * Tilt Equalizer Output parameters.
    */
    typedef struct _tmTDA18280_TiltOutputParameters_t
    {
        tmTDA18280_TiltEqualizer_t      TDA18280_CalculatedTilt;            /**< CalculatedTilt.*/
        Int32                           iTDA18280_Slope;                    /**< Slope.*/
        UInt32                          uTDA18280_RSquare;                  /**< RSquare.*/
        UInt32                          uTDA18280_T0Square;                 /**< T0Square.*/
        UInt32                          uTDA18280_MaxGain;                  /**< MaxGain.*/
        UInt32                          uTDA18280_FreqMaxGain;              /**< FreqMaxGain.*/
        UInt32                          uTDA18280_MinGain;                  /**< MinGain.*/
        UInt32                          uTDA18280_FreqMinGain;              /**< FreqMinGain.*/
        UInt32                          uTDA18280_NbEmptyChannel;           /**< NbEmptyChannel.*/
        Bool                            bTDA18280_BlockOfChannelDetected;   /**< BlockOfChannelDetected.*/
        UInt32                          uTDA18280_LastFreqIndex;            /**< LastFreqIndex.*/
        Bool                            bTDA18280_FirstTimeTEQHandle;       /**< FirstTimeTEQHandle.*/
        Bool                            bTDA18280_StepDetected;             /**< StepDetected.*/
        UInt32                          uTDA18280_StepDetectedFreq;         /**< StepDetectedFreq.*/
        Int32                           iTDA18280_StepDetectedValue;        /**< StepDetectedValue.*/
        tmTDA18280_TiltMeasure_t*       pFreqGain;                          /**< Gain.*/
    } tmTDA18280_TiltOutputParameters_t, *ptmTDA18280_TiltOutputParameters_t, **pptmTDA18280_TiltOutputParameters_t;

    
    /** 
    * HW product type.
    */
    typedef enum _tmTDA18280_ProductType_t
    {
        tmTDA18280_ProductTypeTDA18280 = 0,   /**< TDA18280: 8*TS */
        tmTDA18280_ProductTypeTDA18284,       /**< TDA18284: 4*TS */
        tmTDA18280_ProductTypeTDA18285,       /**< TDA18285: 4*TS + 1*OOB */
        tmTDA18280_ProductTypeTDA18286,       /**< TDA18286: 6*TS */
        tmTDA18280_ProductTypeTDA18287,       /**< TDA18287: 6*TS + 1*OOB */
        tmTDA18280_ProductTypeTDA18256,       /**< TDA18256: 4*TS + 1*DAC (4 channels) */
        tmTDA18280_ProductTypeInvalid         /**< invalid */
    }
    tmTDA18280_ProductType_t, *ptmTDA18280_ProductType_t;

    /** 
    * HW version.
    */
    typedef enum _tmTDA18280_EsVersion_t
    {
        tmTDA18280_EsVersionUnknown = 0, /**< default value */
        tmTDA18280_EsVersion1 = 1,       /**< ES1 */
        tmTDA18280_EsVersion2 = 2,       /**< ES2 */
        tmTDA18280_EsVersionInvalid      /**< invalid */
    }
    tmTDA18280_EsVersion_t, *ptmTDA18280_EsVersion_t;

    /** 
    * AGC mode.
    */
    typedef enum _tmTDA18280_AgcMode_t {
        tmTDA18280_AgcMode_Normal,     /**< Normal.*/
        tmTDA18280_AgcMode_Fast        /**< Fast mode for calibration purpose.*/
    } 
    tmTDA18280_AgcMode_t, *ptmTDA18280_AgcMode_t;

    /** 
    * FFT features.
    */
    typedef struct _tmTDA18280_FFTResults_t
    {
        double minfft;                /**< minimum value of the FFT. */
        double maxfft;                /**< maximum value of the FFT. */
        UInt32 freqAdc;               /**< ADC frequency in MHz*/
        UInt32 SamplingDelta;         /**< Delta between 2 samples of the FFT array in Hz. */
    } tmTDA18280_FFTResults_t, *ptmTDA18280_FFTResults_t;

    /**
    * TDA18280 HCDP BER Window
    */
    typedef enum _tmTDA18280_BERWindow_t
    {
        tmTDA18280_HCDPBERWindowUnknown = 0,
        tmTDA18280_HCDPBERWindow1E5, /**< 1e5 bits. */
        tmTDA18280_HCDPBERWindow1E6, /**< 1e6 bits. */
        tmTDA18280_HCDPBERWindow1E7, /**< 1e7 bits. */
        tmTDA18280_HCDPBERWindow1E8, /**< 1e8 bits. */
        tmTDA18280_HCDPBERWindowMax
    }tmTDA18280_BERWindow_t;

    /**
    * TDA18280 HCDP Interleaver Mode
    */
    typedef enum _tmTDA18280_InterleaverMode_t
    {
        /*Nxa*/
        tmTDA18280_HCDPInterleaverMode_12_17 = 0,
        /*Nxb*/
        tmTDA18280_HCDPInterleaverMode_128_1_A,
        tmTDA18280_HCDPInterleaverMode_128_1_B,
        tmTDA18280_HCDPInterleaverMode_128_2,
        tmTDA18280_HCDPInterleaverMode_128_3,
        tmTDA18280_HCDPInterleaverMode_128_4,
        tmTDA18280_HCDPInterleaverMode_64_2,
        tmTDA18280_HCDPInterleaverMode_32_4,
        tmTDA18280_HCDPInterleaverMode_16_8,
        tmTDA18280_HCDPInterleaverMode_8_16,
        tmTDA18280_HCDPInterleaverMode_NA
    }tmTDA18280_InterleaverMode_t;

    /**
    * TDA18280 OOB AGC indicators
    */
    typedef struct  _tmTDA18280_OOBLockInd_t
    {
        Bool bCarrierLock;
        Bool bDemodLock;
        Bool bFECLock;
        UInt8 bLockAcc;
        UInt8 bSyncQuality;
    }tmTDA18280_OOBLockInd_t;

    /**
    * TDA18280 OOB AGC indicators
    */
    typedef struct  _tmTDA18280_OOBIPAGCInd_t
    {
        UInt16 bCTLAcc;
        UInt8 bBTLAcc;
        Int16 bRFSDLevel;
        UInt8 bDAGCAcc;
        UInt8 bFAGCAcc;
    }tmTDA18280_OOBIPAGCInd_t;

    /**
    * TDA18280 Features
    */
    typedef struct _tmTDA18280_Feature_t
    {
        UInt8 uMaxIndexSupported; /**< indicate Max PATH & CDP number. */
        Bool bOOBSupported;    /**< indicate that OOB is supported.*/
        Bool bDACSupported;    /**< indicate that DAC is supported (replace last 4 demod (4 to 7)).*/
    } tmTDA18280_Feature_t;

    /**
    * TDA18280 OOB FEC mode
    */
    typedef enum _tmTDA18280_OOBFECMode_t {
        tmTDA18280_OOBFECMode_SCTE_55_1,     /**< SCTE_55_1.*/
        tmTDA18280_OOBFECMode_SCTE_55_2,     /**< SCTE_55_2.*/
        tmTDA18280_OOBFECMode_POD,           /**< POD for CableCard (no FEC).*/
        tmTDA18280_OOBFECMode_Max
    } tmTDA18280_OOBFECMode_t, *ptmTDA18280_OOBFECMode_t;

    /**
    * TDA18280 OOB input mode
    */
    typedef enum _tmTDA18280_OobInput_t {
        tmTDA18280_OobInput_None,   /**< OOB not used.*/
        tmTDA18280_OobInput_WB,     /**< OOB used with WB path.*/
        tmTDA18280_OobInput_NB      /**< OOB used with Aux path.*/
    } tmTDA18280_OobInput_t, *ptmTDA18280_OobInput_t;

    /**
    * TDA18280 Lock Indicator
    */
    typedef struct _TDA18280_LockInd_t
    {
        Bool bCAGCSat;
        Bool bDAGCSat;
        Bool bStl;
        Bool bCtl;
        Bool bSctl;
        Bool bDemod;
        Bool bFEC;
        UInt16 uDemodLockTime;
        UInt16 uFecLockTime;
    }TDA18280_LockInd_t;

    /** 
    * Demod settings
    */
    typedef enum _tmTDA18280_ConstellationSource_t
    {
        tmTDA18280_ConstellationSourceUnknown = 0,
        tmTDA18280_ConstellationSourceADC,
        tmTDA18280_ConstellationSourceFEDR,
        tmTDA18280_ConstellationSourcePDF,
        tmTDA18280_ConstellationSourceDAGC,
        tmTDA18280_ConstellationSourceMF,
        tmTDA18280_ConstellationSourceCAGC,
        tmTDA18280_ConstellationSourceEqualizer,
        tmTDA18280_ConstellationSourceBEDR,
        tmTDA18280_ConstellationSourceMax
    }tmTDA18280_ConstellationSource_t;

    typedef enum _tmTDA18280_ExtendSymbolRateMode_t
    {
        tmTDA18280_ExtendSymbolRateModeDisable,
        tmTDA18280_ExtendSymbolRateModeEnable700ppm,
        tmTDA18280_ExtendSymbolRateModeEnable1500ppm, /* costly in lock time, till 1 sec */
        tmTDA18280_ExtendSymbolRateModeInvalid
    }tmTDA18280_ExtendSymbolRateMode_t;

    typedef enum _tmTDA18280_PNBERWindow
    {
        tmTDA18280_PNBERWindow_Unknown = 0,
        tmTDA18280_PNBERWindow_1E5,         /*1e5 bits*/
        tmTDA18280_PNBERWindow_1E6,         /*1e6 bits*/
        tmTDA18280_PNBERWindow_1E7,         /*1e7 bits*/
        tmTDA18280_PNBERWindow_1E8,         /*1e8 bits*/
        tmTDA18280_PNBERWindow_1E9,         /*1e9 bits*/
        tmTDA18280_PNBERWindow_1E10,        /*1e10 bits*/
        tmTDA18280_PNBERWindow_1E11,        /*1e11 bits*/
        tmTDA18280_PNBERWindow_1E12,        /*1e12 bits*/
        tmTDA18280_PNBERWindow_1E13,        /*1e13 bits*/
        tmTDA18280_PNBERWindow_Invalid      /*1e5 bits*/
    } tmTDA18280_PNBERWindow_t;

    /**
    * Extra path configuration
    */
    typedef struct _tmTDA18280_PathCfgFixed_t
    {
        tmTDA18280_ConstellationSource_t  sConstSource;
        tmTDA18280_TsClkMode_t            eTsClkMode;
        tmTDA18280_TsDataOrder_t          eTsDataOrder;
        tmTDA18280_TsSyncLength_t         eTsSyncLength;
        tmTDA18280_ExtendSymbolRateMode_t eExtendSRMode;        /**< Extend Timing Recovery */
    } tmTDA18280_PathCfgFixed_t;

    /** 
    * DAC Source.
    */
    typedef enum _tmTDA18280_UpmixerUpsampling_t {
        tmTDA18280_UpmixerUpsamplingBy12,         /**< upsampling by 12, sample rate = 13.5MS/s. */
        tmTDA18280_UpmixerUpsamplingBy16,         /**< upsampling by 16, sample rate = 10.54MS/s. */
        tmTDA18280_UpmixerUpsamplingInvalid
    } tmTDA18280_UpmixerUpsampling_t, *p_tmTDA18280_UpmixerUpsampling_t;

    /** 
    * DAC Configuration.
    */
    typedef struct _tmTDA18280_DACConfiguration_t
    {
        tmTDA18280_ONOFF_t              eEnable;               /**< Enable/Disable DAC.*/
        UInt32                          ulInternalRefResistor; /**< internal ref resistor.*/
        UInt32                          ulFineTuning;          /**< Fine tuning.*/
        UInt32                          ulCoarseTuning;        /**< Coarse tuning.*/
    }
    tmTDA18280_DACConfiguration_t, *ptmTDA18280_DACConfiguration_t;

#define TDA18280_NO_IRQ                  0x00000000

    /** 
    * TSMF Mode.
    */
    typedef enum _tmTDA18280_TsmfMode_t
    {
        tmTDA18280_TsmfModeDisabled,
        tmTDA18280_TsmfModeForced,
        tmTDA18280_TsmfModeAuto,
        tmTDA18280_TsmfModeInvalid
    }tmTDA18280_TsmfMode_t, *ptmTDA18280_TsmfMode_t;

    /** 
    * TSMF Status.
    */
    typedef enum _tmTDA18280_TsmfStatusError_t
    {
        tmTDA18280_TsmfStatusNoError,
        tmTDA18280_TsmfStatusError,
        tmTDA18280_TsmfStatusErrorInvalid
    }tmTDA18280_TsmfStatusError_t, *ptmTDA18280_TsmfStatusError_t;

    /** 
    * TSMF Emergency.
    */
    typedef enum _tmTDA18280_TsmfEmergency_t
    {
        tmTDA18280_TsmfEmergencyInactive,
        tmTDA18280_TsmfEmergencyActive,
        tmTDA18280_TsmfEmergencyNA,
    }tmTDA18280_TsmfEmergency_t;

    /** 
    * TSMF Receive Status.
    */
    typedef enum _tmTDA18280_TsmfReceiveStatus_t
    {
        tmTDA18280_TsmfReceiveStatusGood,
        tmTDA18280_TsmfReceiveStatusMedium,
        tmTDA18280_TsmfReceiveStatusBad,
        tmTDA18280_TsmfReceiveStatusNA
    }tmTDA18280_TsmfReceiveStatus_t, *ptmTDA18280_TsmfReceiveStatus_t;

    /** 
    * TSMF TS Status.
    */
    typedef enum _tmTDA18280_TsmfTsStatus_t
    {
        tmTDA18280_TsmfTsStatusValid,
        tmTDA18280_TsmfTsStatusInvalid
    }tmTDA18280_TsmfTsStatus_t, *ptmTDA18280_TsmfTsStatus_t;

    /** 
    * TSMF Lock Status.
    */
    typedef enum _tmTDA18280_TsmfStatusLock_t
    {
        tmTDA18280_TsmfStatusNotLocked,
        tmTDA18280_TsmfStatusLocked,
        tmTDA18280_TsmfStatusLockInvalid
    }tmTDA18280_TsmfStatusLock_t, *ptmTDA18280_TsmfStatusLock_t;

    /** 
    * TSMF Status.
    */
    typedef struct _tmTDA18280_TsmfStatus_t
    {
        tmTDA18280_TsmfStatusLock_t           eLockStatus;
        tmTDA18280_TsmfStatusError_t          eError;
        tmTDA18280_TsmfEmergency_t            eEmergency;
        tmTDA18280_TsmfReceiveStatus_t        eCurrentReceiveStatus;
        UInt16                              uVersionNumber;
        UInt16                              uSelectedTsId;
        UInt16                              uSelectedOnId;
    }tmTDA18280_TsmfStatus_t, *ptmTDA18280_TsmfStatus_t;

    /** 
    * TSMF Available TS.
    */
    typedef struct _tmTDA18280_TsmfAvailableTs_t
    {
        UInt16                       TsId;
        UInt16                       OnId;
        tmTDA18280_TsmfTsStatus_t      eTsStatus;
        tmTDA18280_TsmfReceiveStatus_t eRcvStatus;
    }tmTDA18280_TsmfAvailableTs_t, *ptmTDA18280_TsmfAvailableTs_t;

    typedef enum _tmTDA18280_typeIrq_t {
        tmTDA18280_typeIrq_none,
        tmTDA18280_typeIrq_Lock,
        tmTDA18280_typeIrq_UnLock
    } tmTDA18280_typeIrq_t, *ptmTDA18280_typeIrq_t;

    typedef enum _TDA18280_ExtendSymbolRateMode_t
    {
        TDA18280_ExtendSymbolRateModeDisable,
        TDA18280_ExtendSymbolRateModeEnable700ppm,
        TDA18280_ExtendSymbolRateModeEnable1500ppm, /* costly in lock time, till 1 sec */
        TDA18280_ExtendSymbolRateModeInvalid
    }TDA18280_ExtendSymbolRateMode_t;

    /*=================================================================================================*/
    /* Exported functions:                                                                             */
    /*=================================================================================================*/

    /*=================================================================================================*/
    /**
    \brief  Create an instance of the TDA18280 device : inititalize the memory 
    reserved for the specified unit number with the specified data structure 
    containing pointer to external functions.

    \return TM_OK: Succeed.
    \return TDA18280_ERR_BAD_PARAMETER: Pointer to external functions is not correct.
    \return TDA18280_ERR_ALREADY_SETUP: This instance has already been open previously.
    \return TDA18280_ERR_ERR_NO_INSTANCES: No Memory prepared for this instance.
    \return TDA18280_ERR_NOT_INITIALIZED: The memory prepared is not correct (Marker issue).
    \return Other optional return codes of the functions registered 
    in DeviceOpenInstance API (Mutex...).

    \note   No hardware access is done in this function.

    \sa     NA
    */
    /*=================================================================================================*/
    tmErrorCode_t
        tmbslTDA18280_DeviceOpenInstance(
        tmUnitSelect_t                    tUnit,      /**< I: Unit number. */
        const tmbslFrontEndDependency_t*  ptSrvFunc   /**< I: Structure containing the Hardware Access, the Time and the Debug functions.*/
        );

    /*=================================================================================================*/
    /**
    \brief  Close the instance of the TDA18280 device: Desinititalize the memory 
    reserved for the specified unit number.

    \return TM_OK: Succeed.
    \return TDA18280_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
    \return Other optional return codes of the functions registered
    in DeviceOpenInstance API (Mutex...).

    \note   NA

    \sa     NA
    */
    /*=================================================================================================*/
    tmErrorCode_t
        tmbslTDA18280_DeviceCloseInstance(
        tmUnitSelect_t              tUnit   /**< I: Unit number. */
        );

    /*=================================================================================================*/
    /**
    \brief  Get the software version of the TDA18280 device driver.

    \return TM_OK: Succeed.

    \note   ptmSWVersion_t is defined in the tmnxtypes.h file.

    \sa     NA
    */
    /*=================================================================================================*/
    tmErrorCode_t
        tmbslTDA18280_DeviceGetSWVersion(
        tmSWVersion_t*              ptSWVersion  /**< O: Receives SW Version. */
        );

    /*=================================================================================================*/
    /**
    \brief  Get the version of the TDA18280 device configuration file.

    \return TM_OK: Succeed.

    \note   ptmSWSettingsVersion_t is defined in the tmnxtypes.h file.

    \sa     NA
    */
    /*=================================================================================================*/
    tmErrorCode_t
        tmbslTDA18280_DeviceGetSWSettingsVersion(
        tmSWSettingsVersion_t*      ptSWSettingsVersion   /**< O: Receives SW configuration file version. */
        );

    /*=================================================================================================*/
    /**
    \brief  Perform the hardware Initialization of the device.

    \return TM_OK: Succeed.
    \return TDA18280_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
    \return TDA18280_ERR_BAD_PARAMETER: Parameters out of range.
    \return Other optional return codes of the functions registered 
    in DeviceOpenInstance API (I2C read errors, mutex...).

    \note   To be performed once tmbslTDA18280_DeviceOpenInstance has been called.

    \sa     NA
    */
    /*=================================================================================================*/
    tmErrorCode_t
        tmbslTDA18280_DeviceHwInit(
        tmUnitSelect_t              tUnit    /**< I: Unit number. */
        );

    /*=================================================================================================*/
    /**
    \brief  Set the power state of the device.

    \return TM_OK: Succeed.
    \return TDA18280_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
    \return TDA18280_ERR_BAD_PARAMETER: Parameters out of range.
    \return Other optional return codes of the functions registered 
    in DeviceOpenInstance API (I2C read errors, mutex...).

    \note   NA

    \sa     NA
    */
    /*=================================================================================================*/
    tmErrorCode_t
        tmbslTDA18280_DeviceSetPowerState(
        tmUnitSelect_t              tUnit,      /**< I: Unit number. */
        tmTDA18280_PowerState_t     tpowerState /**< I: Power state (tmPowerOn or tmPowerStandby. */
        );

    /*=================================================================================================*/
    /**
    \brief  Get the power state of the device.

    \return TM_OK: Succeed.
    \return TDA18280_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
    \return TDA18280_ERR_BAD_PARAMETER: Parameters out of range.
    \return Other optional return codes of the functions registered 
    in DeviceOpenInstance API (I2C read errors, mutex...).

    \note   NA

    \sa     NA
    */
    /*=================================================================================================*/
    tmErrorCode_t
        tmbslTDA18280_DeviceGetPowerState(
        tmUnitSelect_t              tUnit,          /**< I: Unit number. */
        tmTDA18280_PowerState_t*    ptPowerState    /**< O: receives the Power state (tmPowerOn or tmPowerStandby). */
        );

    /*=================================================================================================*/
    /**
    \brief  Get Product name

    \return TM_OK: Succeed.
    \return TDA18280_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
    \return TDA18280_ERR_BAD_PARAMETER: Parameters out of range.
    \return Other optional return codes of the functions registered 
    in DeviceOpenInstance API (I2C read errors, mutex...).

    \note   .

    \sa     NA
    */
    /*=================================================================================================*/
    tmErrorCode_t
        tmbslTDA18280_DeviceGetProductName(
        tmUnitSelect_t       tUnit,
        tmTDA18280_ProductType_t* pProductName
        );

    /*=================================================================================================*/
    /**
    \brief  Set the demod FEC mode.

    \return TM_OK: Succeed.
    \return TDA18280_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
    \return TDA18280_ERR_BAD_PARAMETER: Parameters out of range.
    \return Other optional return codes of the functions registered 
    in DeviceOpenInstance API (I2C read errors, mutex...).

    \note   NA

    \sa     NA
    */
    /*=================================================================================================*/
    tmErrorCode_t
        tmbslTDA18280_DeviceSetFECMode(
        tmUnitSelect_t              tUnit,          /**< I: Unit number. */
        tmFrontEndFECMode_t         eFECMode        /**< I: FEC mode */
        );

    /*=================================================================================================*/
    /**
    \brief  Get the demod FEC mode.

    \return TM_OK: Succeed.
    \return TDA18280_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
    \return TDA18280_ERR_BAD_PARAMETER: Parameters out of range.
    \return Other optional return codes of the functions registered 
    in DeviceOpenInstance API (I2C read errors, mutex...).

    \note   NA

    \sa     NA
    */
    /*=================================================================================================*/
    tmErrorCode_t
        tmbslTDA18280_DeviceGetFECMode(
        tmUnitSelect_t              tUnit,          /**< I: Unit number. */
        tmFrontEndFECMode_t*        peFECMode       /**< O: FEC mode */
        );

    /*=================================================================================================*/
    /**
    \brief  Set the OOB FEC mode.

    \return TM_OK: Succeed.
    \return TDA18280_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
    \return TDA18280_ERR_BAD_PARAMETER: Parameters out of range.
    \return Other optional return codes of the functions registered 
    in DeviceOpenInstance API (I2C read errors, mutex...).

    \note   NA

    \sa     NA
    */
    /*=================================================================================================*/
    tmErrorCode_t
        tmbslTDA18280_DeviceSetOOBFECMode(
        tmUnitSelect_t              tUnit,          /**< I: Unit number. */
        tmTDA18280_OOBFECMode_t     eFECMode        /**< I: FEC mode */
        );

    /*=================================================================================================*/
    /**
    \brief  Get the OOB FEC mode.

    \return TM_OK: Succeed.
    \return TDA18280_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
    \return TDA18280_ERR_BAD_PARAMETER: Parameters out of range.
    \return Other optional return codes of the functions registered 
    in DeviceOpenInstance API (I2C read errors, mutex...).

    \note   NA

    \sa     NA
    */
    /*=================================================================================================*/
    tmErrorCode_t
        tmbslTDA18280_DeviceGetOOBFECMode(
        tmUnitSelect_t              tUnit,          /**< I: Unit number. */
        tmTDA18280_OOBFECMode_t*    peFECMode       /**< O: FEC mode */
        );

    /*=================================================================================================*/
    /**
    \brief  Clear, Enable or Disable IRQ lock or unlock of the specified path 

    \return TM_OK: Succeed.
    \return TDA18280_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
    \return TDA18280_ERR_BAD_PARAMETER: Parameters out of range.
    \return Other optional return codes of the functions registered 
    in DeviceOpenInstance API (I2C read errors, mutex...).

    \note   NA

    \sa     NA
    */
    /*=================================================================================================*/
    tmErrorCode_t
        tmbslTDA18280_PathIrqSet(
        tmUnitSelect_t              tUnit,          /**< I: Unit number. */
        tmTDA18280_Path_t           tPathNumber,    /**< Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).*/
        tmTDA18280_typeIrq_t        TypeIRQ         /**< I: Lock / unlock */
        );

    /*=================================================================================================*/
    /**
    \brief  Read IRQ lock or unlock of the specified path 

    \return TM_OK: Succeed.
    \return TDA18280_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
    \return TDA18280_ERR_BAD_PARAMETER: Parameters out of range.
    \return Other optional return codes of the functions registered 
    in DeviceOpenInstance API (I2C read errors, mutex...).

    \note   NA

    \sa     NA
    */
    /*=================================================================================================*/
    tmErrorCode_t
        tmbslTDA18280_PathIrqRead(
        tmUnitSelect_t              tUnit,          /**< I: Unit number. */
        tmTDA18280_Path_t           tPathNumber,    /**< Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).*/
        tmTDA18280_typeIrq_t        TypeIRQ,        /**< I: Lock / unlock */
        Bool*                       bIrq            /**< O: return True if <TypeIRQ> else False */
        );

    /*=================================================================================================*/
    /**
    \brief  Perform the configuration of the specified path of the device.

    \return TM_OK: Succeed.
    \return TDA18280_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
    \return TDA18280_ERR_BAD_PARAMETER: Parameters out of range.
    \return TDA18280_ERR_NOT_SUPPORTED: indicates that the function can not be called in current state
    \return Other optional return codes of the functions registered 
    in DeviceOpenInstance API (I2C read errors, mutex...).

    \note   To be performed once tmbslTDA18280_DeviceHwInit has been called.

    \sa     NA
    */
    /*=================================================================================================*/
    tmErrorCode_t
        tmbslTDA18280_PathSet(
        tmUnitSelect_t              tUnit,      /**< I: Unit number. */
        tmTDA18280_Path_t           tPathNumber,/**< Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).*/
        const tmTDA18280_PathCfg_t* ptPathCfg   /**< I: Pointer to Path Configuration structure. */                 
        );

    /*=================================================================================================*/
    /**
    \brief  Read the configuration of the specified path of the device.

    \return TM_OK: Succeed.
    \return TDA18280_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
    \return TDA18280_ERR_BAD_PARAMETER: Parameters out of range.
    \return Other optional return codes of the functions registered 
    in DeviceOpenInstance API (I2C read errors, mutex...).

    \note   To be performed once tmbslTDA18280_DeviceHwInit has been called.

    \sa     NA
    */
    /*=================================================================================================*/
    tmErrorCode_t
        tmbslTDA18280_PathGet(
        tmUnitSelect_t              tUnit,      /**< I: Unit number. */
        tmTDA18280_Path_t           tPathNumber,/**< Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).*/
        tmTDA18280_PathCfg_t*       ptPathCfg   /**< I: Pointer to Path Configuration structure. */                 
        );

    /*=================================================================================================*/
    /**
    \brief  Write the I2C gate configuration of the device.

    \return TM_OK: Succeed.
    \return TDA18280_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
    \return TDA18280_ERR_BAD_PARAMETER: Parameters out of range.
    \return Other optional return codes of the functions registered 
    in DeviceOpenInstance API (I2C read errors, mutex...).

    \note   NA

    \sa     NA
    */
    /*=================================================================================================*/
    tmErrorCode_t
        tmbslTDA18280_DeviceI2CGate_SetCfg(
        tmUnitSelect_t              tUnit,          /**< I: Unit number. */
        tmTDA18280_I2CGate_t*       pI2CGateCfg     /**< I: Configuration of the Gate. */
        );

    /*=================================================================================================*/
    /**
    \brief  Read the I2C gate configuration of the device.

    \return TM_OK: Succeed.
    \return TDA18280_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
    \return TDA18280_ERR_BAD_PARAMETER: Parameters out of range.
    \return Other optional return codes of the functions registered 
    in DeviceOpenInstance API (I2C read errors, mutex...).

    \note   NA

    \sa     NA
    */
    /*=================================================================================================*/
    tmErrorCode_t
        tmbslTDA18280_DeviceI2CGate_GetCfg(
        tmUnitSelect_t              tUnit,          /**< I: Unit number. */
        tmTDA18280_I2CGate_t*       pI2CGateCfg     /**< O: Configuration of the Gate. */
        );

    /*=================================================================================================*/
    /**
    \brief  Get Tilt Equalizer Estimation.

    \return TM_OK: Succeed.
    \return TDA18280_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
    \return TDA18280_ERR_BAD_PARAMETER: Parameters out of range.
    \return Other optional return codes of the functions registered 
    in DeviceOpenInstance API (I2C read errors, mutex...).

    \note   NA

    \sa     NA
    */
    /*=================================================================================================*/
    tmErrorCode_t
        tmbslTDA18280_DeviceGetTiltEqualizerEstimation(
        tmUnitSelect_t                     tUnit,           /**< I: Unit number. */
        tmTDA18280_TiltInputParameters_t*  pTiltInputParameters, /**< I: Tilt Inptut Data. */
        tmTDA18280_TiltOutputParameters_t* pTiltParameters       /**< O: Tilt Output Data. */
        );

    /*=================================================================================================*/
    /**
    \brief  Read Debug Ram of the device.

    \return TM_OK: Succeed.
    \return TDA18280_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
    \return TDA18280_ERR_BAD_PARAMETER: Parameters out of range.
    \return Other optional return codes of the functions registered 
    in DeviceOpenInstance API (I2C read errors, mutex...).

    \note   RAMsize should be <= 0x4000

    \sa     NA
    */
    /*=================================================================================================*/
    tmErrorCode_t
        tmbslTDA18280_DeviceReadDebugRam(
        tmUnitSelect_t  tUnit,           /**< I: Unit number. */
        tmTDA18280_DebugRamMode_t eMode, /**< I: Debug Ram Read Mode. */
        UInt16          RAMsize,         /**< I: Size of the RAM. */
        Int16 *         pData            /**< O: Pointer to data. */
        );


    /*=================================================================================================*/
    /**
    \brief  Write a register of the device.

    \return TM_OK: Succeed.
    \return TDA18280_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
    \return TDA18280_ERR_BAD_PARAMETER: Parameters out of range.
    \return Other optional return codes of the functions registered 
    in DeviceOpenInstance API (I2C read errors, mutex...).

    \note   NA

    \sa     NA
    */
    /*=================================================================================================*/
    tmErrorCode_t
        tmbslTDA18280_DeviceWriteRegister(
        tmUnitSelect_t              tUnit,          /**< I: Unit number. */
        UInt16                      Base_Address,   /**< I: base address of the register. */
        UInt16                      Address,        /**< I: address of the register. */
#ifdef PRINT_I2C
        Char Str[50],                               /**< I: Debug String of register. */
#endif
        UInt16                      uData           /**< I: data to write in the register. */
        );

    /*=================================================================================================*/
    /**
    \brief  Read a register of the device.

    \return TM_OK: Succeed.
    \return TDA18280_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
    \return TDA18280_ERR_BAD_PARAMETER: Parameters out of range.
    \return Other optional return codes of the functions registered 
    in DeviceOpenInstance API (I2C read errors, mutex...).

    \note   NA

    \sa     NA
    */
    /*=================================================================================================*/
    tmErrorCode_t
        tmbslTDA18280_DeviceReadRegister(
        tmUnitSelect_t              tUnit,          /**< I: Unit number. */
        UInt16                      Base_Address,   /**< I: base address of the register. */
        UInt16                      Address,        /**< I: address of the register. */
#ifdef PRINT_I2C
        Char Str[50],                               /**< I: Debug String of register. */
#endif
        UInt16*                     puData          /**< O: Receives the read Data. */
        );

    /*=================================================================================================*/
    /**
    \brief  Get the temperature of TDA18280 component

    \param  tUnit   Device unit number
    \param  plTemp  Pointer to the device temperature in tenth of dB (value_dB*10)

    \return TM_OK: Succeed.
    \return TDA18280_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
    \return TDA18280_ERR_BAD_PARAMETER: Parameters out of range.
    \return Other optional return codes of the functions registered 
    in DeviceOpenInstance API (I2C read errors, mutex...).

    \note   NA

    \sa     NA
    */
    /*=================================================================================================*/
    tmErrorCode_t 
        tmbslTDA18280_GetTemperature( 
        tmUnitSelect_t                  tUnit,        /**< I: Unit number */
        Int32                          *plTemp        /**< O: Temperature */
        );

    /*=================================================================================================*/
    /**
    \brief  Capture data from debug ram and perform FFT calculation.

    \return TM_OK: Succeed.
    \return TDA18280_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
    \return TDA18280_ERR_BAD_PARAMETER: Parameters out of range.
    \return Other optional return codes of the functions registered 
    in DeviceOpenInstance API (I2C read errors, mutex...).

    \note   This might take some time.

    \sa     NA
    */
    /*=================================================================================================*/
    tmErrorCode_t
        tmbslTDA18280_PerformFFT(
        tmUnitSelect_t tUnit,              /**< I: Unit number. */
        UInt16 nbSamples,                  /**< I: number of samples (max = 0x4000). */
        UInt16 nbCaptures,                 /**< I: number of FFT used for average calculation. */
        double *fft,                       /**< O: pointer on the array containing the FFT (contains nbSamples/2 elements). */
        ptmTDA18280_FFTResults_t fftResult /**< O: FFT feature. */
        );

    /*=================================================================================================*/
    /**
    \brief  Get CDP or OOB demodulator lock status

    \param  tUnit         Demodulator unit number
    \param  tPathNumber   Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).
    \param  peLockStatus  Pointer to the lock status (Searching, Locked or NotLocked)

    \return TM_OK Succeed
    \return TDA18280_ERR_NOT_INITIALIZED This unit is not initialised
    \return error codes of the functions registered in tmbslTDA18280_Open
    \return TDA18280_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA18280_Open)

    \note   NA

    \sa     NA
    */
    /*=================================================================================================*/
    tmErrorCode_t
        tmbslTDA18280_PathGetDemodLockStatus
        (
        tmUnitSelect_t        tUnit,        /**< I: Unit number. */
        tmTDA18280_Path_t     tPathNumber,  /**< I: Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).*/
        tmbslFrontEndState_t *peLockStatus  /**< O: Demod lock status. */
        );

    /*=================================================================================================*/
    /**
    \brief  Get CDP or OOB demodulator BER value

    \param  tUnit         Demodulator unit number
    \param  tPathNumber   Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).
    \param  psBER         Pointer to BER value
    \param  puUncors      Pointer to Uncor value

    \return TM_OK Succeed
    \return TDA18280_ERR_NOT_INITIALIZED This unit is not initialised
    \return error codes of the functions registered in tmbslTDA18280_Open
    \return TDA18280_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA18280_Open)

    \note   NA

    \sa     NA
    */
    /*=================================================================================================*/
    tmErrorCode_t
        tmbslTDA18280_PathGetDemodBER
        (
        tmUnitSelect_t        tUnit,        /**< I: Unit number. */
        tmTDA18280_Path_t     tPathNumber,  /**< I: Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).*/
        tmFrontEndFracNb32_t *psBER,        /**< O: Bit Error Ratio value. */
        UInt32               *puUncors      /**< O: Uncorrected packets value. */
        );

    /*=================================================================================================*/
    /**
    \brief  Get the BER computation window

    \param  tUnit         Demodulator unit number
    \param  tPathNumber   Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).
    \param  peBERWindow   Pointer to the BER computation window

    \return TM_OK Succeed
    \return TDA18280_ERR_NOT_INITIALIZED This unit is not initialised
    \return error codes of the functions registered in tmbslTDA18280_Open
    \return TDA18280_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA18280_Open)

    \note   NA

    \sa     NA
    */
    /*=================================================================================================*/
    tmErrorCode_t
        tmbslTDA18280_PathGetDemodBERWindow
        (
        tmUnitSelect_t          tUnit,        /**< I: Unit number. */
        tmTDA18280_Path_t       tPathNumber,  /**< I: Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).*/
        tmTDA18280_BERWindow_t *peBERWindow   /**< O: Size of the BER computation window.*/
        );

    /*=================================================================================================*/
    /**
    \brief  Get IF Offset value

    \param  tUnit         Demodulator unit number
    \param  tPathNumber   Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).
    \param  pIFOffset     Pointer to IF Offset value (in Hz)

    \return TM_OK Succeed
    \return TDA18280_ERR_NOT_INITIALIZED This unit is not initialised
    \return error codes of the functions registered in tmbslTDA18280_Open
    \return TDA18280_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA18280_Open)

    \note   NA

    \sa     NA
    */
    /*=================================================================================================*/
    tmErrorCode_t
        tmbslTDA18280_PathGetDemodIFOffset
        (
        tmUnitSelect_t        tUnit,        /**< I: Unit number. */
        tmTDA18280_Path_t     tPathNumber,  /**< I: Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).*/
        Int32                *pIFOffset     /**< O: IF offset value (in Hz). */
        );

    /*=================================================================================================*/
    /**
    \brief  Get Symbol Rate value

    \param  tUnit         Demodulator unit number
    \param  tPathNumber   Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).
    \param  puSR          Pointer to Symbol Rate value

    \return TM_OK Succeed
    \return TDA18280_ERR_NOT_INITIALIZED This unit is not initialised
    \return error codes of the functions registered in tmbslTDA18280_Open
    \return TDA18280_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA18280_Open)

    \note   NA

    \sa     NA
    */
    /*=================================================================================================*/
    tmErrorCode_t
        tmbslTDA18280_PathGetDemodSR
        (
        tmUnitSelect_t        tUnit,        /**< I: Unit number. */
        tmTDA18280_Path_t     tPathNumber,  /**< I: Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).*/
        UInt32                *puSR         /**< O: Symbol Rate value. */
        );

    /*=================================================================================================*/
    /**
    \brief  Get Spectral Inversion value

    \param  tUnit         Demodulator unit number
    \param  tPathNumber   Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).
    \param  peSI          Pointer to Spectral Inversion value

    \return TM_OK Succeed
    \return TDA18280_ERR_NOT_INITIALIZED This unit is not initialised
    \return error codes of the functions registered in tmbslTDA18280_Open
    \return TDA18280_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA18280_Open)

    \note   NA

    \sa     NA
    */
    /*=================================================================================================*/
    tmErrorCode_t
        tmbslTDA18280_PathGetDemodSI
        (
        tmUnitSelect_t        tUnit,        /**< I: Unit number. */
        tmTDA18280_Path_t     tPathNumber,  /**< I: Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).*/
        tmFrontEndSpecInv_t   *peSI         /**< O: Spectral Inversion value. */
        );

    /*=================================================================================================*/
    /**
    \brief  Gets the actual interleaver mode value

    \param  tUnit         Demodulator unit number
    \param  tPathNumber   Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).
    \param  peSI          Pointer to interleaver mode value

    \return TM_OK Succeed
    \return TDA18280_ERR_NOT_INITIALIZED This unit is not initialised
    \return error codes of the functions registered in tmbslTDA18280_Open
    \return TDA18280_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA18280_Open)

    \note   NA

    \sa     NA
    */
    /*=================================================================================================*/
    tmErrorCode_t
        tmbslTDA18280_PathGetDemodInterleaverMode
        (
        tmUnitSelect_t        tUnit,        /**< I: Unit number. */
        tmTDA18280_Path_t     tPathNumber,  /**< I: Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).*/
        tmTDA18280_InterleaverMode_t *peInterleaverMode  /**< O: Pointer to interleaver mode.*/
        );

    /*=================================================================================================*/
    /**
    \brief  Get the channel EsNo in dB (or SNR, Signal to Noise Ratio).

    \param  tUnit         Demodulator unit number
    \param  tPathNumber   Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).
    \param  pEsno         Pointer to channel EsNo value

    \return TM_OK Succeed
    \return TDA18280_ERR_NOT_INITIALIZED This unit is not initialised
    \return error codes of the functions registered in tmbslTDA18280_Open
    \return TDA18280_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA18280_Open)

    \note   NA

    \sa     NA
    */
    /*=================================================================================================*/
    tmErrorCode_t
        tmbslTDA18280_PathGetDemodChannelEsNo
        (
        tmUnitSelect_t        tUnit,        /**< I: Unit number. */
        tmTDA18280_Path_t     tPathNumber,  /**< I: Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).*/
        tmFrontEndFracNb32_t *pEsno         /**< O: Demod channel EsNo. */
        );

    /*=================================================================================================*/
    /**
    \brief  Gets the OOB Uncors

    \param  tUnit         Out Of Band unit number
    \param  puUncors      Pointer to Uncors value
    \param  puCorrected   Pointer to Corrected value

    \return TM_OK Succeed
    \return TDA18280_ERR_NOT_INITIALIZED This unit is not initialised
    \return error codes of the functions registered in tmbslTDA18280_Open
    \return TDA18280_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA18280_Open)

    \note   NA

    \sa     NA
    */
    /*=================================================================================================*/
    tmErrorCode_t
        tmbslTDA18280_DeviceGetOOBUncors
        (
        tmUnitSelect_t            tUnit,        /**< I: Unit number. */
        UInt32                   *puUncors,     /**< O: OOB Uncors. */
        UInt32                   *puCorrected   /**< O: OOB Corrected. */
        );

    /*=================================================================================================*/
    /**
    \brief  Gets the OOB modulation error rate 

    \param  tUnit         Demodulator unit number
    \param  psMER         Pointer to OOB MER value

    \return TM_OK Succeed
    \return TDA18280_ERR_NOT_INITIALIZED This unit is not initialised
    \return error codes of the functions registered in tmbslTDA18280_Open
    \return TDA18280_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA18280_Open)

    \note   NA

    \sa     NA
    */
    /*=================================================================================================*/
    tmErrorCode_t
        tmbslTDA18280_DeviceGetOOBMER
        (
        tmUnitSelect_t            tUnit,        /**< I: Unit number. */
        tmFrontEndFracNb32_t     *psMER         /**< O: OOB MER. */
        );

    /*=================================================================================================*/
    /**
    \brief  Clear uncor

    \param  tUnit         Demodulator unit number
    \param  tPathNumber   Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).

    \return TM_OK Succeed
    \return TDA18280_ERR_NOT_INITIALIZED This unit is not initialised
    \return error codes of the functions registered in tmbslTDA18280_Open
    \return TDA18280_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA18280_Open)

    \note   NA

    \sa     NA
    */
    /*=================================================================================================*/
    tmErrorCode_t
        tmbslTDA18280_PathClearDemodUncor
        (
        tmUnitSelect_t        tUnit,        /**< I: Unit number. */
        tmTDA18280_Path_t     tPathNumber   /**< I: Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).*/
        );

    /*=================================================================================================*/
    /**
    \brief  Get driver features

    \param  tUnit        Demodulator unit number
    \param  psFeature    TDA18280 Features

    \return TM_OK Succeed
    \return TDA18280_ERR_NOT_INITIALIZED This unit is not initialised
    \return error codes of the functions registered in tmbslTDA18280_Open
    \return TDA18280_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA18280_Open)

    \note   NA

    \sa     NA
    */
    /*=================================================================================================*/
    tmErrorCode_t
        tmbslTDA18280_GetFeatures
        (
        tmUnitSelect_t        tUnit,       /**< I: Unit number. */
        tmTDA18280_Feature_t *ptFeature    /**< O: TDA18280 Features.*/
        );

    /*=================================================================================================*/
    /**
    \brief  Set OOB input mode

    \param  tUnit        Demodulator unit number
    \param  eOobInput    OOB input (none, Wide Band or Narrow Band)

    \return TM_OK Succeed
    \return TDA18280_ERR_NOT_INITIALIZED This unit is not initialised
    \return error codes of the functions registered in tmbslTDA18280_Open
    \return TDA18280_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA18280_Open)

    \note   NA

    \sa     NA
    */
    /*=================================================================================================*/
     tmErrorCode_t
         tmbslTDA18280_DeviceSetOOBInput
         ( 
         tmUnitSelect_t        tUnit,        /**< I: Unit number. */
         tmTDA18280_OobInput_t eOobInput     /**< I: OOB input. */
         );

     /*=================================================================================================*/
     /**
     \brief  Get OOB input mode

     \param  tUnit        Demodulator unit number
     \param  peOobInput   Pointer to OOB input (none, Wide Band or Narrow Band)

     \return TM_OK Succeed
     \return TDA18280_ERR_NOT_INITIALIZED This unit is not initialised
     \return error codes of the functions registered in tmbslTDA18280_Open
     \return TDA18280_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA18280_Open)

     \note   NA

     \sa     NA
     */
     /*=================================================================================================*/
     tmErrorCode_t
         tmbslTDA18280_DeviceGetOOBInput
         ( 
         tmUnitSelect_t        tUnit,          /**< I: Unit number. */
         tmTDA18280_OobInput_t *peOobInput     /**< O: OOB output. */
         );

     /*=================================================================================================*/
     /**
     \brief  Set the TSMF mode.

     \param  tUnit         Demodulator unit number
     \param  tPathNumber   Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).
     \param  eTsmfMode     TSMF mode (Disable, Forced or Auto)

     \return TM_OK Succeed
     \return TDA18280_ERR_NOT_INITIALIZED This unit is not initialised
     \return error codes of the functions registered in tmbslTDA18280_Open
     \return TDA18280_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA18280_Open)

     \note   NA

     \sa     NA
     */
     /*=================================================================================================*/
     tmErrorCode_t
         tmbslTDA18280_PathSetDemodTsmfMode
         (
         tmUnitSelect_t        tUnit,        /**< I: Unit number. */
         tmTDA18280_Path_t     tPathNumber,  /**< I: Path selector. */
         tmTDA18280_TsmfMode_t eTsmfMode     /**< I: TSMF mode. */
         );

     /*=================================================================================================*/
     /**
     \brief  Get the TSMF mode.

     \param  tUnit         Demodulator unit number
     \param  tPathNumber   Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).
     \param  peTsmfMode    Pointer to TSMF mode (Disable, Forced or Auto)

     \return TM_OK Succeed
     \return TDA18280_ERR_NOT_INITIALIZED This unit is not initialised
     \return error codes of the functions registered in tmbslTDA18280_Open
     \return TDA18280_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA18280_Open)

     \note   NA

     \sa     NA
     */
     /*=================================================================================================*/
     tmErrorCode_t
         tmbslTDA18280_PathGetDemodTsmfMode
         (
         tmUnitSelect_t         tUnit,        /**< I: Unit number. */
         tmTDA18280_Path_t      tPathNumber,  /**< I: Path selector. */
         tmTDA18280_TsmfMode_t* peTsmfMode     /**< O: Pointer to TSMF mode. */
         );

     /*=================================================================================================*/
     /**
     \brief  Get the TSMF status.

     \param  tUnit         Demodulator unit number
     \param  tPathNumber   Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).
     \param  psTsmfStatus  Pointer to TSMF status.

     \return TM_OK Succeed
     \return TDA18280_ERR_NOT_INITIALIZED This unit is not initialised
     \return error codes of the functions registered in tmbslTDA18280_Open
     \return TDA18280_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA18280_Open)

     \note   NA

     \sa     NA
     */
     /*=================================================================================================*/
     tmErrorCode_t
         tmbslTDA18280_PathGetDemodTsmfStatus
         (
         tmUnitSelect_t         tUnit,        /**< I: Unit number. */
         tmTDA18280_Path_t      tPathNumber,  /**< I: Path selector. */
         tmTDA18280_TsmfStatus_t* psTsmfStatus  /**< O: Pointer to TSMF status. */
         );


     /*=================================================================================================*/
     /**
     \brief  Get the available TS IDs

     \param  tUnit                  Demodulator unit number
     \param  tPathNumber            Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).
     \param  peTsmfTsmfAvailableTs  Pointer to available TS IDs.

     \return TM_OK Succeed
     \return TDA18280_ERR_NOT_INITIALIZED This unit is not initialised
     \return error codes of the functions registered in tmbslTDA18280_Open
     \return TDA18280_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA18280_Open)

     \note   NA

     \sa     NA
     */
     /*=================================================================================================*/
     tmErrorCode_t
         tmbslTDA18280_PathGetDemodTsmfAvailableTsId
         (
         tmUnitSelect_t         tUnit,        /**< I: Unit number. */
         tmTDA18280_Path_t      tPathNumber,  /**< I: Path selector. */
         tmTDA18280_TsmfAvailableTs_t* peTsmfTsmfAvailableTs  /**< O: Pointer to available TS IDs. */
         );


     /*=================================================================================================*/
     /**
     \brief  Select one of the available TS IDs.

     \param  tUnit         Demodulator unit number
     \param  tPathNumber   Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).
     \param  uTsId         Index of the selected TsId (between 1 and 15)

     \return TM_OK Succeed
     \return TDA18280_ERR_NOT_INITIALIZED This unit is not initialised
     \return error codes of the functions registered in tmbslTDA18280_Open
     \return TDA18280_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA18280_Open)

     \note   NA

     \sa     NA
     */
     /*=================================================================================================*/
     tmErrorCode_t
         tmbslTDA18280_PathSelectDemodTsmfTsId
         (
         tmUnitSelect_t        tUnit,        /**< I: Unit number. */
         tmTDA18280_Path_t     tPathNumber,  /**< I: Path selector. */
         UInt32                uTsId         /**< I: TS Id. */
         );

    /*=================================================================================================*/
    /**
    \brief  Set Extend Symbol Rate Range

    \param  tUnit         Demodulator unit number
    \param  tPathNumber   Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).
    \param  eMode         Extended Symbol Rate Range

    \return TM_OK Succeed
    \return TDA18280_ERR_NOT_INITIALIZED This unit is not initialised
    \return error codes of the functions registered in tmbslTDA18280_Open
    \return TDA18280_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA18280_Open)

    \note   NA

    \sa     NA
    */
    /*=================================================================================================*/
    tmErrorCode_t
        tmbslTDA18280_PathSetDemodExtendSymbolRateRange
        (
        tmUnitSelect_t                   tUnit,        /**< I: Unit number. */
        tmTDA18280_Path_t                tPathNumber,  /**< I: Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).*/
        TDA18280_ExtendSymbolRateMode_t  eMode         /**< I: Extended Symbol Rate Range. */
        );

    /*=================================================================================================*/
    /**
    \brief  Get Extend Symbol Rate Range

    \param  tUnit         Demodulator unit number
    \param  tPathNumber   Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).
    \param  peMode        Extended Symbol Rate Range

    \return TM_OK Succeed
    \return TDA18280_ERR_NOT_INITIALIZED This unit is not initialised
    \return error codes of the functions registered in tmbslTDA18280_Open
    \return TDA18280_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA18280_Open)

    \note   NA

    \sa     NA
    */
    /*=================================================================================================*/
    tmErrorCode_t
        tmbslTDA18280_PathGetDemodExtendSymbolRateRange
        (
        tmUnitSelect_t                   tUnit,          /**< I: Unit number. */
        tmTDA18280_Path_t                tPathNumber,    /**< I: Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).*/
        TDA18280_ExtendSymbolRateMode_t *peMode          /**< 0: Extended Symbol Rate Range. */
        );

    /*=================================================================================================*/
    /**
    \brief  Change some settings after demod lock

    \param  tUnit         Demodulator unit number
    \param  tPathNumber   Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).

    \return TM_OK Succeed
    \return TDA18280_ERR_NOT_INITIALIZED This unit is not initialised
    \return error codes of the functions registered in tmbslTDA18280_Open
    \return TDA18280_ERR_NULL_CONTROLFUNC External functions are not properly registered (tmbslTDA18280_Open)

    \note   NA

    \sa     NA
    */
    /*=================================================================================================*/
    tmErrorCode_t
        tmbslTDA18280_PathSetAfterDemodLock
        (
        tmUnitSelect_t                   tUnit,        /**< I: Unit number. */
        tmTDA18280_Path_t                tPathNumber   /**< I: Path selector. From 0 to (TDA18280_NB_PATH-1), including Auxiliary path (tmTDA18280_Path_Aux).*/
        );

#ifdef __cplusplus
}
#endif

#endif /* _TMBSL_TDA18280_H */

