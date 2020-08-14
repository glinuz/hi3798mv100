/*=================================================================================================*/
/**
Copyright (C) 2008 NXP B.V., All Rights Reserved.
This source code and any compilation or derivative thereof is the proprietary
information of NXP B.V. and is confidential in nature. Under no circumstances
is this software to be  exposed to or placed under an Open Source License of
any type without the expressed written permission of NXP B.V.
*
* \file          tmbslTDA18280_InstanceCustom.h
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
*
* \par Changelog
*  -0.0.1.18 Digital AGC optimization for CCI
*  -0.0.1.17 Add config for polarity of IRQ output pin
*  -0.0.1.16 fine tuning of digital AGC for paths to DAC
*     adjust allowed number of paths for each product
*     restrict OOB feature to compatible products
*  -0.0.1.15 Select extended SR recovery separately for each path
*  -0.0.1.14 Simplify product ES version management
*  -0.0.1.13 Fix base address of slow registers for DDC3 and DDC7
*  -0.0.1.12 Add TS configuration (MSB first, sync on 1 byte)
*  -0.0.1.11 ES2: add API for IRQ management (enable/disable/read status/clear status) (Puma5 API compatibility)
*  -0.0.1.10 Integration of new xml file for ES2
*  -0.0.1.9 Take into account TDA18280 ES2 xml for driver
*  -0.0.1.8 Rename TDA18280_DIG_ADC_TARGET_RMS by TDA18280_DIG_AGC_TARGET_RMS
*     In case of TDA18256 and 3 < path<=7, TDA18280_DIG_ADC_TARGET_RMS 0x40 -> 0x11
*  -0.0.1.7 Add configuration for TS clock mode (gated, permanent)
*  -0.0.1.6 Change default product type to Invalid
*  -0.0.1.5 Fix a regression in power consumption at HwInit
*  -0.0.1.4 Update all documentation for OM3990 delivery
*  -0.0.1.4 Update all documentation for delivery of TDA18256 + TDA18204 to customers ( add detection auto mode for product name)
*  -0.0.1.3 Decimation factor of DDC equals 12
*  -0.0.1.2 Change default internal OOB IF to 5MHz for proper lock
*  -0.0.1.1 debug RAM address fix (xml + RegDef)
*  -0.0.1.0 RegDef update
*  -0.0.0.4 Change in default features
*  -0.0.0.3 Use ZIF for CDP and OOB demod input
*  -0.0.0.2 fix PLL config (frequency 5.4GHz)
*  -0.0.0.1 Support DAC and channel stacker (TDA18256)
*  -0.0.0.1 Initial version
*
* \par Version_Definition
*  VERSION_TAG:TDA18280_SETTINGS_CUSTOMER_NUM.TDA18280_SETTINGS_PROJECT_NUM.TDA18280_SETTINGS_MAJOR_VER.TDA18280_SETTINGS_MINOR_VER
*/
/*=================================================================================================*/

#pragma once
#ifndef _TDA18280_INSTANCE_CUSTOM_H
#define _TDA18280_INSTANCE_CUSTOM_H

#ifdef __cplusplus
extern "C"
{
#endif

/*============================================================================*/
/* Types and defines:                                                         */
/*============================================================================*/

/*==========================================================*/
/* Driver settings version definition:                      */
/*==========================================================*/
/** 
* SW Settings Customer Identification Number.
*/
#define TDA18280_SETTINGS_CUSTOMER_NUM 0
/** 
* SW Settings Project Identification Number.
*/
#define TDA18280_SETTINGS_PROJECT_NUM  0
/** 
* SW Settings Major Version.
*/
#define TDA18280_SETTINGS_MAJOR_VER    1
/** 
* SW Settings Minor Version.
*/
#define TDA18280_SETTINGS_MINOR_VER    18

/*==========================================================*/
/*General definitions:                                      */
/*==========================================================*/

/** 
* Maximum number of instances supported by the driver.
*/
#define TDA18280_MAX_UNITS                      (2)

/** 
* PLL configuration.
*/
#define TDA18280_PLL_CONFIG     \
    {   \
    tmTDA18280_PLL1_5400MHz,                /* pll1Freq, PLL1 frequency */ \
    tmTDA18280_LOChainMode8,                /* LOChainMode , 1, 3, 4, 5 or 8*/ \
    tmTDA18280_deciDDC12,                   /* deciDDC, Decimation factor of DDC: 12 or 16.*/ \
    tmTDA18280_DivNBDDC_4,                  /* DivNBDDC, Auxiliary DDC clock divider definition (NBDDCfreq=(FreqWBADC/16)/(DivNBDDC).*/\
    tmTDA18280_NBADCSampleFreq56MHz25       /* Auxiliary ADC Sample frequency. */\
    }

/**
* TDA1828x supported features.
*/
#define TDA18280_FEATURE \
    { \
        0,                                  /* uMaxIndexSupported */ \
        False,                              /* bOOBEnabled */ \
        False,                              /* bDACEnabled */ \
    }

/**
* Wideband RF AGC configuration.
*/
#define TDA18280_RFAGC_CONFIG \
    { \
    tmTDA18280_RfAgcCtrl_Enabled,   /* eRfAgcCtrl, RF AGC enabled */ \
    tmTDA18280_RfAgcMode_RMS         /* eRfAgcMode, mode RMS only */ \
    }

/**
* Interrupt pin (IRQ) configuration.
*/
#define TDA18280_IRQ_CONFIG \
    { \
    tmTDA18280_IrqMode_Enabled,         /* eIrqMode, IRQ pin enabled */ \
    tmTDA18280_IrqPolarity_ActiveHigh   /* eIrqPolarity, active high */ \
    }

/** 
* Path selector.
*/
typedef enum _tmTDA18280_Path_t {
    tmTDA18280_Path_0=0,   /**< Select path number 0.*/
    tmTDA18280_Path_1,     /**< Select path number 1.*/
    tmTDA18280_Path_2,     /**< Select path number 2.*/
    tmTDA18280_Path_3,     /**< Select path number 3.*/
    tmTDA18280_Path_4,     /**< Select path number 4.*/
    tmTDA18280_Path_5,     /**< Select path number 5.*/
    tmTDA18280_Path_6,     /**< Select path number 6.*/
    tmTDA18280_Path_7,     /**< Select path number 7.*/
    tmTDA18280_Path_Aux,   /**< Select auxiliary path.*/
    tmTDA18280_Path_Max
} tmTDA18280_Path_t, *ptmTDA18280_Path_t;


/*======================================================================================================*/
/*======================================================================================================*/
/* Custom Driver Instance Parameters:                                                                   */
/*======================================================================================================*/
/*======================================================================================================*/

/*==========================================================*/
/*Power settings:                                           */
/*==========================================================*/

/** 
* Marker to check the custom parameters are well initialized for each instance.
*/
#define TDA18280_INSTANCE_CUSTOM_MARKER  0xABCD

/** 
* IF value used for demod: Zero-IF
*/
#define TDA18280_DEMOD_IF  0
#define TDA18280_OOB_IF  5000000
#define TDA18280_DEMOD_INPUT_SAMPLE_FREQ  56250000 /* sampling frequency of demod input signal = 56.25MHz */

/** 
* Default value of path.
*/
#define TDA18280_INSTANCE_PATH \
    {                                  \
    tmTDA18280_OFF,            /**< tPathStatus: Path Status ON (1) or OFF (0). */\
    642000000,                 /**< uRF,           if PathStatus=ON, RF frequency to use for this path in hertz. Not Applicable if tPathNumber = tmTDA18280_Path_Aux.*/\
    tmTDA18280_8MHz,           /**< tBW,           if PathStatus=ON, BandWidth to use for this path 6000000 or 8000000 in hertz. */\
    tmTDA18280_ON,             /**< tGainAutoMode, if PathStatus=ON, 0: Gain Auto mode OFF, 1:  Auto mode ON.*/\
    0,                         /**< uGain,         if PathStatus=ON, Gain for this path in dB. Read only if GainAutoMode ON, Else ReadWrite.*/\
    tmFrontEndFECModeAnnexA,   /**< eFECMode,      if PathStatus=ON, Demod Configuration FEC mode */ \
    tmFrontEndSpecInvAuto,     /**< eSI,           if PathStatus=ON, Demod Configuration Spectral inversion */ \
    tmFrontEndModulationQam64, /**< eMod,          if PathStatus=ON, Demod Configuration Modulation */ \
    6875000,                   /**< uSR,           if PathStatus=ON, Demod Configuration Symbol Rate in Hz */ \
    }

/** 
* Default value of auxiliary path.
*/
#define TDA18280_INSTANCE_PATH_AUX \
    {                                  \
    tmTDA18280_OFF,            /**< tPathStatus: Path Status ON (1) or OFF (0). */\
    4000000,                   /**< uRF,           if PathStatus=ON, RF frequency to use for this path in hertz. Not Applicable if tPathNumber = tmTDA18280_Path_Aux.*/\
    tmTDA18280_8MHz,           /**< tBW,           if PathStatus=ON, BandWidth to use for this path 6000000 or 8000000 in hertz. */\
    tmTDA18280_ON,             /**< tGainAutoMode, if PathStatus=ON, 0: Gain Auto mode OFF, 1:  Auto mode ON.*/\
    0,                         /**< uGain,         if PathStatus=ON, Gain for this path in dB. Read only if GainAutoMode ON, Else ReadWrite.*/\
    tmFrontEndFECModeAnnexA,   /**< eFECMode,      if PathStatus=ON, Demod Configuration FEC mode */ \
    tmFrontEndSpecInvAuto,     /**< eSI,           if PathStatus=ON, Demod Configuration Spectral inversion */ \
    tmFrontEndModulationQam64, /**< eMod,          if PathStatus=ON, Demod Configuration Modulation */ \
    6875000,                   /**< uSR,           if PathStatus=ON, Demod Configuration Symbol Rate in Hz */ \
    }

/**
* Custom path configuration (default).
*/
#define TDA18280_INSTANCE_CUSTOM_PATH_DEFAULT TDA18280_INSTANCE_CUSTOM_PATH_GATED_CLK

/**
* Custom path configuration (TS gated clock).
*/
#define TDA18280_INSTANCE_CUSTOM_PATH_GATED_CLK \
    { \
    tmTDA18280_ConstellationSourceUnknown,  /**< sConstSource: */ \
    tmTDA18280_TsClkGated,                  /**< eTsClkMode: TS clock mode configuration */ \
    tmTDA18280_TsDataMsbFirst,              /**< eTsDataOrder: TS data MSB/LSB order configuration */ \
    tmTDA18280_TsSyncFirstByte,             /**< eTsSyncLength: TS sync length configuration */ \
    tmTDA18280_ExtendSymbolRateModeEnable700ppm, /**< eExtendSRMode: Extend Timing Recovery  */ \
    } \

/**
* Custom path configuration (TS permanent clock).
*/
#define TDA18280_INSTANCE_CUSTOM_PATH_PERMANENT_CLK \
    { \
    tmTDA18280_ConstellationSourceUnknown,  /**< sConstSource: */ \
    tmTDA18280_TsClkPermanent,              /**< eTsClkMode: TS clock mode configuration */ \
    tmTDA18280_TsDataMsbFirst,              /**< eTsDataOrder: TS data MSB/LSB order configuration */ \
    tmTDA18280_TsSyncFirstByte,             /**< eTsSyncLength: TS sync length configuration */ \
    tmTDA18280_ExtendSymbolRateModeEnable700ppm, /**< eExtendSRMode: Extend Timing Recovery  */ \
    } \

/** 
* Default value of NB ADC configuration.
*/
#define TDA18280_INSTANCE_CUSTOM_NB_ADC_FAST_SEL tmTDA18280_NBADCFastSel_fast

/** 
* Default value of I2CGATE configuration.
*/
#define TDA18280_INSTANCE_CUSTOM_I2CGATE \
    {                                  \
    tmTDA18280_OFF,         /**< tI2CGateActivation; 0: OFF Gate inactive, 1: ON Gate active*/   \
    7                       /**< tI2CGateSCIAdd; If Gate active, address of the SCI (TDA18204) from 0 to 7*/  \
    }

/** 
* Default value of TEQ input parameters.
*/
#define TDA18280_INSTANCE_CUSTOM_TEQ_INPUT \
    {                                  \
    False,                             /**< bUseInputParameter; Use Input Parameters.*/\
    0,                                 /**< pTiltFrequencyPlan; Tilt Frequency Plan.*/\
    0,                                 /**< pTiltChannelPlan; Tilt Channel Plan.*/\
    tmTDA18280_TiltEqualizer0dB,       /**< TDA18280_DefaultTilt;Default Tilt value.*/\
    False,                             /**< bTDA18280_RejectionEnabled;Rejection Enabled.*/\
    (tmTDA18280_Path_Max-1),           /**< uTDA18280_NbChannels;Nb of channels used in parallel for retrieving Gain */\
    10,                                /**< uTDA18280_MinNbChannelsNotEmpty;Nb minimum of not empty channels to apply Tilt algorithm */\
    10,                                /**< uTDA18280_StepAverage;Nb of values for average calculation into step detection */\
    50,                                /**< uTDA18280_SlopeThreshold;SlopeThreshold (x10).*/\
    7,                                 /**< uTDA18280_StepThreshold;StepThreshold (x10) in dB.*/\
    10,                                /**< uTDA18280_CorrelThreshold;100 x Correlation threshold */\
    465,                               /**< uTDA18280_EmptyThreshold;EmptyThreshold (x10) in dB. */\
    50,                                /**< uTDA18280_FrequencyPercentThreshold;FrequencyPercentThreshold in % */\
    70,                                /**< uTDA18280_StepPercentThreshold;StepPercentThreshold in % */\
    40,                                /**< uTDA18280_StepFreqRange;StepFreqRange in MHz */\
    10,                                /**< uTDA18280_Wait;Wait in ms */\
    tmTDA18280_6MHz                    /**< uTDA18280_eTDA18280_BandWidth; 8MHz or 6MHz */\
    }

/** 
* Default value of TEQ output parameters.
*/
#define TDA18280_INSTANCE_CUSTOM_TEQ_OUTPUT \
    {                                  \
    tmTDA18280_TiltEqualizer15dB,     /**< TDA18280_CalculatedTilt;           CalculatedTilt.*/\
    0,                                /**< iTDA18280_Slope;                   Slope (x10).*/\
    0,                                /**< uTDA18280_RSquare;                 RSquare.*/\
    0,                                /**< uTDA18280_T0Square;                T0Square.*/\
    0,                                /**< uTDA18280_MaxGain;                 MaxGain.*/\
    0,                                /**< uTDA18280_FreqMaxGain;             FreqMaxGain.*/\
    0,                                /**< uTDA18280_MinGain;                 MinGain.*/\
    0,                                /**< uTDA18280_FreqMinGain;             FreqMinGain.*/\
    0,                                /**< uTDA18280_NbEmptyChannel;          NbEmptyChannel.*/\
    False,                            /**< bTDA18280_BlockOfChannelDetected;  BlockOfChannelDetected.*/\
    156,                              /**< uTDA18280_LastFreqIndex;           LastFreqIndex.*/\
    True,                             /**< bTDA18280_FirstTimeTEQHandle;      FirstTimeTEQHandle.*/\
    False,                            /**< bTDA18280_StepDetected;            StepDetected.*/\
    0,                                /**< uTDA18280_StepDetectedFreq;        StepDetectedFreq.*/\
    0,                                /**< iTDA18280_StepDetectedValue;       StepDetectedValue.*/\
    0                                 /**< pFreqGain;                         Gain.*/\
}

/** 
* Tilt Equalizer Table size.
*/
#define TDA18280_TEQ_ARRAY_SIZE 158 

/** 
* Default Tilt Equalizer frequency plan.
*/
#define TDA18280_DEFAULT_FREQUENCY_PLAN { \
57, \
63, \
69, \
79, \
85, \
93, \
99, \
105, \
111, \
117, \
123, \
129, \
135, \
141, \
147, \
153, \
159, \
165, \
171, \
177, \
183, \
189, \
195, \
201, \
207, \
213, \
219, \
225, \
231, \
237, \
243, \
249, \
255, \
261, \
267, \
273, \
279, \
285, \
291, \
297, \
303, \
309, \
315, \
321, \
327, \
333, \
339, \
345, \
351, \
357, \
363, \
369, \
375, \
381, \
387, \
393, \
399, \
405, \
411, \
417, \
423, \
429, \
435, \
441, \
447, \
453, \
459, \
465, \
471, \
477, \
483, \
489, \
495, \
501, \
507, \
513, \
519, \
525, \
531, \
537, \
543, \
549, \
555, \
561, \
567, \
573, \
579, \
585, \
591, \
597, \
603, \
609, \
615, \
621, \
627, \
633, \
639, \
645, \
651, \
657, \
663, \
669, \
675, \
681, \
687, \
693, \
699, \
705, \
711, \
717, \
723, \
729, \
735, \
741, \
747, \
753, \
759, \
765, \
771, \
777, \
783, \
789, \
795, \
801, \
807, \
813, \
819, \
825, \
831, \
837, \
843, \
849, \
855, \
861, \
867, \
873, \
879, \
885, \
891, \
897, \
903, \
909, \
915, \
921, \
927, \
933, \
939, \
945, \
951, \
957, \
963, \
969, \
975, \
981, \
987, \
993, \
999, \
0 \
}

/** 
* Default Tilt Equalizer channel plan (last element must be 0xFF).
*/
#define TDA18280_DEFAULT_CHANNEL_PLAN { \
0, \
1, \
2, \
3, \
4, \
5, \
6, \
7, \
0xFF \
}

/** 
* Default value of complete customer defined parameter structure.
*/
#define TDA18280_INSTANCE_CUSTOM_0 \
    tmTDA18280_PowerMax,                    /* curPowerState: Current power settings */ \
    tmTDA18280_HwState_InitNotDone,         /* eHwState: Hardware state */ \
    TDA18280_PLL_CONFIG,                    /* PllCfg: PLL configuration.*/ \
    TDA18280_RFAGC_CONFIG,                  /* RfAgcCfg: Wideband RF AGC configuration (mixed signal AGC).*/\
    tmTDA18280_ProductTypeTDA18284,          /* eProductType: undefined product type.*/ \
    tmTDA18280_EsVersionUnknown,            /* EsVersion: undefined HW version.*/ \
    TDA18280_FEATURE,                       /* tFeature: TDA18280 supported features.*/ \
    {                                       /* Path[tmTDA18280_Path_Max], Path configuration values for each path.*/ \
    TDA18280_INSTANCE_PATH,                 /* Path 0 */ \
    TDA18280_INSTANCE_PATH,                 /* Path 1 */ \
    TDA18280_INSTANCE_PATH,                 /* Path 2 */ \
    TDA18280_INSTANCE_PATH,                 /* Path 3 */ \
    TDA18280_INSTANCE_PATH,                 /* Path 4 */ \
    TDA18280_INSTANCE_PATH,                 /* Path 5 */ \
    TDA18280_INSTANCE_PATH,                 /* Path 6 */ \
    TDA18280_INSTANCE_PATH,                 /* Path 7 */ \
    TDA18280_INSTANCE_PATH_AUX              /* Path auxiliary */ \
    },                                      \
    {                                       /* PathCurrentState */ \
    tmTDA18280_OFF,                         /* Path 0 */ \
    tmTDA18280_OFF,                         /* Path 1 */ \
    tmTDA18280_OFF,                         /* Path 2 */ \
    tmTDA18280_OFF,                         /* Path 3 */ \
    tmTDA18280_OFF,                         /* Path 4 */ \
    tmTDA18280_OFF,                         /* Path 5 */ \
    tmTDA18280_OFF,                         /* Path 6 */ \
    tmTDA18280_OFF,                         /* Path 7 */ \
    tmTDA18280_OFF                          /* Path auxiliary */ \
    },                                      \
    TDA18280_INSTANCE_CUSTOM_NB_ADC_FAST_SEL, /**< Auxiliary ADC clock selection.*/\
    TDA18280_INSTANCE_CUSTOM_I2CGATE,         /**< I2CGate configuration.*/\
    TDA18280_INSTANCE_CUSTOM_TEQ_INPUT,       /**< Default value of TEQ input parameters.*/\
    TDA18280_INSTANCE_CUSTOM_TEQ_OUTPUT,      /**< Default value of TEQ output parameters.*/\
    tmFrontEndFECModeAnnexA,                  /**< Default Value for FEC Mode (demod).*/ \
    tmTDA18280_OOBFECMode_SCTE_55_1,          /**< Default Value for OOB FEC Mode.*/ \
    tmTDA18280_Path_Max,                      /**< Default Value for OOB input.*/ \
    TDA18280_IRQ_CONFIG,                      /**< IrqCfg: IRQ pin configuration.*/ \
    { \
    TDA18280_INSTANCE_CUSTOM_PATH_DEFAULT, /* Path 0 */ \
    TDA18280_INSTANCE_CUSTOM_PATH_DEFAULT, /* Path 1 */ \
    TDA18280_INSTANCE_CUSTOM_PATH_DEFAULT, /* Path 2 */ \
    TDA18280_INSTANCE_CUSTOM_PATH_DEFAULT, /* Path 3 */ \
    TDA18280_INSTANCE_CUSTOM_PATH_DEFAULT, /* Path 4 */ \
    TDA18280_INSTANCE_CUSTOM_PATH_DEFAULT, /* Path 5 */ \
    TDA18280_INSTANCE_CUSTOM_PATH_DEFAULT, /* Path 6 */ \
    TDA18280_INSTANCE_CUSTOM_PATH_DEFAULT, /* Path 7 */ \
    TDA18280_INSTANCE_CUSTOM_PATH_DEFAULT, /* Path auxiliary */ \
    }, \
    TDA18280_INSTANCE_CUSTOM_MARKER

/** 
* DIGITAL AGC
*/

#define TDA18280_DIG_AGC_K 12 // default value is 12
#define TDA18280_DIG_AGC_ACCU_WINDOW_SEL_NORMAL 1
#define TDA18280_DIG_AGC_ERROR_WINDOW_SEL_NORMAL 1
#define TDA18280_DIG_AGC_TH_H_NORMAL 0x001
#define TDA18280_DIG_AGC_TH_L_NORMAL 0xFFF
#define TDA18280_DIG_AGC_ACCU_WINDOW_SEL_FAST 0
#define TDA18280_DIG_AGC_ERROR_WINDOW_SEL_FAST 0
#define TDA18280_DIG_AGC_TH_UL_DISABLE 0x800 /* disable CableCut */

/* DIGITAL AGC FOR DEMOD */
#define TDA18280_DIG_AGC_TARGET_RMS 0x35 // value is 53 -> 0x35
#define TDA18280_DIG_AGC_TH_VH_NORMAL 0x06A /* value is +106 (4.77dB) -> 0x06A */
#define TDA18280_DIG_AGC_TH_VL_NORMAL 0xF9A /* value is -106 = -TDA18280_DIG_AGC_TH_VH_NORMAL -> 0xF9A */
#define TDA18280_DIG_AGC_TH_VH_FAST 0x01F /* threshold very high at +2dB: 31 -> 0x01F */
#define TDA18280_DIG_AGC_TH_VL_FAST 0xFE1 /* threshold very low at -2dB: -31 -> 0xFE1 */
/* Cable Cut Ultra Low threshold must be set to -7dB vs target value (0x35) */
/* 10*log10((TARGET+UL)/TARGET) = -6.83 with UL = -42 (0xFD6)*/
#define TDA18280_DIG_AGC_TH_UL 0xFD6

/* DIGITAL AGC FOR DAC (TDA18256 and 4 <= path <= 7) */
#define TDA18280_DIG_AGC_TARGET_RMS_FOR_DAC 0x15 // optimized to 0x15 (iso 0x40); 4.8dB below TDA18280_DIG_AGC_TARGET_RMS (in theory 7.5dB)
#define TDA18280_DIG_AGC_TH_VH_NORMAL_FOR_DAC 0x02A /* value is +42 (4.77dB) -> 0x02A */
#define TDA18280_DIG_AGC_TH_VL_NORMAL_FOR_DAC 0xFD5 /* value is -42 = -TDA18280_DIG_AGC_TH_VH_NORMAL_FOR_DAC -> 0xFD5 */
#define TDA18280_DIG_AGC_TH_VH_FAST_FOR_DAC 0x00D /* threshold very high at +2dB: 13 -> 0x00D */
#define TDA18280_DIG_AGC_TH_VL_FAST_FOR_DAC 0xFF8 /* threshold very low at -2dB: -8 -> 0xFF8 */
/* Cable Cut Ultra Low threshold must be set to -7dB vs target value (0x15) */
/* 10*log10((TARGET+UL)/TARGET) = -7.2dB with UL = -17 (0xFEF)*/
#define TDA18280_DIG_AGC_TH_UL_FOR_DAC 0xFEF

/** 
* RSSI
*/
#define TDA18280_RSSI_CORRECTION 1200 /* dB x100 */

#ifdef __cplusplus
}
#endif

#endif /* _TDA18280_INSTANCE_CUSTOM_H */

