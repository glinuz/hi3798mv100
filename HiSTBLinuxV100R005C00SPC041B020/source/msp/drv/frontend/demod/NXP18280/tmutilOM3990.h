/**
Copyright (C) 2010 NXP B.V., All Rights Reserved.
This source code and any compilation or derivative thereof is the proprietary
information of NXP B.V. and is confidential in nature. Under no circumstances
is this software to be  exposed to or placed under an Open Source License of
any type without the expressed written permission of NXP B.V.
*
* \file          tmutilOM3990.h
*
* \date          %modify_time%
*
* \author        JMLD
*
* \brief         Describe briefly the purpose of this file.
*
* REFERENCE DOCUMENTS :
*                tmutilOM3990_User_Guide.pdf
*
* \par Changelog
*  -2.0.0 Remove float variable from RSSI and calibration functions.
*     API change in tmutilOM3990GetRSSI for *RSSI variable: the float value (in dBuV) becomes an int value (in dBuV*100).
*  -0.1.1 fix RSSI correction for WB path
*  -0.1.0 API prototype changes: tUnit of devices
*  -0.1.0 source code refactoring for RSSI
*  -0.0.7 all FFT computations done here in tmutilOM3990_PerformFFT(), itmutilOM3990_CalculateFFT()
*  -0.0.6 remove useless function call to tmbslTDA18280_DeviceGetHwVersion
*  -0.0.5 Take into account product ID and different value of TDA18280_DIG_ADC_TARGET_RMS in power level calculation
*     Change comment because uOffsetPowerLevel is in dB not in dBuV, because it's a relative value, not a power
*     Add API tmutilOM3990_GetRssiCorrectionArray() because already use by PUMA5 API
*  -0.0.4 Remove warnings for sample code
*  -0.0.3 Update TDA18280 Power mode according to modification in driver
*  -0.0.2 Update all documentation for delivery of TDA18256 + TDA18204 to customers
*  -0.0.1 Initial version
*  -1.1.2 Take into account HW version of TDA18280 to apply or not 6dB offset in RSSI
*  -1.1.1 Update code after integration and test
*  -1.1.0 Modify DS calibration API to select BandWidth (API modified)
*  -1.0.1 test
*  -1.0.2 Add DownStream calibration for NB
*     Add FFT feature
*     Update RSSI for NB
*  -0.1.1 Add DownStream calibration for NB
*  -0.1.0 Add FFT feature
*     Update RSSI for NB
*  -1.0.0 Released version
*  -1.0.0 Release version
*  -1.0.0 Rework RSSI management
*  -0.0.9 Add crest factor at DigAGC output in power level calculation + remove diplexer lost
*  -0.0.8 Update documentation
*  -0.0.7 Implement RSSI calibration with NVRAM example +  Apply an offset of 6dB to AGC gain read
*  -0.0.6 Debug TEQ
*  -0.0.5 Fix RSSI issue and add a 9.5 dB offset (measured on OM3968 board).
*  -0.0.4 Adapt to new path gain type in 18280 API
*  -0.0.3 Update Tilt API + float for RSSI
*  -0.0.2 Adapt to 18280 API
*  -0.0.1 First version
*
* \par Version_Definition
*  VERSION_TAG:UTILOM3990_COMP_NUM.UTILOM3990_MAJOR_VER.UTILOM3990_MINOR_VER
*
*/


#ifndef tmutilOM3990_H
#define tmutilOM3990_H

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*/
/*                       INCLUDE FILES                                        */
/*============================================================================*/

/*============================================================================*/
/*                       MACRO DEFINITION                                     */
/*============================================================================*/
    /* Driver version definition */
#define UTILOM3990_COMP_NUM  2 /* Major protocol change - Specification change required */
#define UTILOM3990_MAJOR_VER 0 /* Minor protocol change - Specification change required */
#define UTILOM3990_MINOR_VER 0 /* Software update - No protocol change - No specification change required */


/* SW Error codes */
#define OM3990_ERR_BASE               (CID_COMP_TUNER | CID_LAYER_DEVLIB)
#define OM3990_ERR_COMP               (CID_COMP_TUNER | CID_LAYER_DEVLIB | TM_ERR_COMP_UNIQUE_START)

#define OM3990_ERR_BAD_UNIT_NUMBER       (OM3990_ERR_BASE + TM_ERR_BAD_UNIT_NUMBER)
#define OM3990_ERR_ERR_NO_INSTANCES      (OM3990_ERR_BASE + TM_ERR_NO_INSTANCES)
#define OM3990_ERR_NOT_INITIALIZED       (OM3990_ERR_BASE + TM_ERR_NOT_INITIALIZED)
#define OM3990_ERR_BAD_PARAMETER         (OM3990_ERR_BASE + TM_ERR_BAD_PARAMETER)
#define OM3990_ERR_NOT_SUPPORTED         (OM3990_ERR_BASE + TM_ERR_NOT_SUPPORTED)
#define OM3990_ERR_HW                    (OM3990_ERR_COMP + 0x0001)
#define OM3990_ERR_POWER_LEVEL_TOO_LOW   (OM3990_ERR_COMP + 0x0002)
#define OM3990_ERR_POWER_LEVEL_TOO_HIGH  (OM3990_ERR_COMP + 0x0003)

typedef struct _OM3990_TransfertFunctionADCTable
{
    UInt32 uX;  /* RF (MHz) */
    Int32 iY;   /* 100 x Transfert Function coefficients */
} OM3990_TransfertFunctionADCTable;

typedef struct _OM3990_TopDigAGCTable
{
    UInt32 uX;  /* TARGET (reg) */
    Int32 iY;   /* TopDigAGC (dBuV x100) */
} OM3990_TopDigAGCTable;

#define OM3990_TF_ADC_TABLE \
    /* RF   ,  Transfert Function coefficients */ \
{ \
    {1,   0 },\
    {68,  -1 },\
    {118, -2}, \
    {152, -3 }, \
    {180, -4 }, \
    {205, -5 }, \
    {226, -6 }, \
    {247, -7 }, \
    {264, -8 }, \
    {282, -9 }, \
    {298, -10}, \
    {313, -11}, \
    {328, -12}, \
    {342, -13}, \
    {356, -14}, \
    {369, -15}, \
    {382, -16}, \
    {394, -17}, \
    {406, -18}, \
    {418, -19}, \
    {429, -20}, \
    {440, -21}, \
    {451, -22}, \
    {462, -23}, \
    {472, -24}, \
    {482, -25}, \
    {492, -26}, \
    {502, -27}, \
    {512, -28}, \
    {521, -29}, \
    {531, -30}, \
    {540, -31}, \
    {549, -32}, \
    {558, -33}, \
    {567, -34}, \
    {576, -35}, \
    {584, -36}, \
    {593, -37}, \
    {601, -38}, \
    {609, -39}, \
    {618, -40}, \
    {626, -41}, \
    {634, -42}, \
    {642, -43}, \
    {650, -44}, \
    {657, -45}, \
    {665, -46}, \
    {673, -47}, \
    {680, -48}, \
    {688, -49}, \
    {695, -50}, \
    {703, -51}, \
    {710, -52}, \
    {717, -53}, \
    {725, -54}, \
    {732, -55}, \
    {739, -56}, \
    {746, -57}, \
    {753, -58}, \
    {760, -59}, \
    {767, -60}, \
    {774, -61}, \
    {781, -62}, \
    {787, -63}, \
    {794, -64}, \
    {801, -65}, \
    {807, -66}, \
    {814, -67}, \
    {821, -68}, \
    {827, -69}, \
    {834, -70}, \
    {840, -71}, \
    {847, -72}, \
    {853, -73}, \
    {859, -74}, \
    {866, -75}, \
    {872, -76}, \
    {878, -77}, \
    {884, -78}, \
    {891, -79}, \
    {897, -80}, \
    {903, -81}, \
    {909, -82}, \
    {915, -83}, \
    {921, -84}, \
    {927, -85}, \
    {933, -86}, \
    {939, -87}, \
    {945, -88}, \
    {951, -89}, \
    {957, -90}, \
    {963, -91}, \
    {969, -92}, \
    {975, -93}, \
    {981, -94}, \
    {987, -95}, \
    {992, -96}, \
    {999, -97}, \
    {0,0} \
}

#define OM3990_TOP_DIG_AGC_TABLE \
    /* TARGET (reg),  TopDigAGC (dBuV x100) */ \
{ \
    {2047, 11556    },\
    {1626, 11456    },\
    {1292, 11356    },\
    {1026, 11256    },\
    {815,  11156    },\
    {647,  11056    },\
    {514,  10956    },\
    {408,  10856    },\
    {364,  10806    },\
    {324,  10756    },\
    {289,  10706    },\
    {257,  10655    },\
    {229,  10605    },\
    {204,  10555    },\
    {182,  10505    },\
    {162,  10454    },\
    {144,  10403    },\
    {129,  10356    },\
    {115,  10306    },\
    {102,  10254    },\
    {91,   10204    },\
    {81,   10153    },\
    {72,   10102    },\
    {64,   10051    },\
    {57,   10001    },\
    {51,   9953     },\
    {45,   9898     },\
    {40,   9847     },\
    {36,   9801     },\
    {32,   9750     },\
    {28,   9692     },\
    {25,   9643     },\
    {22,   9587     },\
    {20,   9546     },\
    {19,   9524     },\
    {18,   9500     },\
    {17,   9475     },\
    {16,   9449     },\
    {15,   9421     },\
    {14,   9391     },\
    {13,   9359     },\
    {12,   9324     },\
    {11,   9286     },\
    {10,   9245     },\
    {9,    9199     },\
    {8,    9148     },\
    {7,    9090     },\
    {6,    9023     },\
    {5,    8944     },\
    {4,    8847     },\
    {3,    8722     },\
    {2,    8546     },\
    {1,    8245     },\
    {0,    8245     } \
}

/* max NVRAM size is set to 20 samples */
#define OM3990_DEFAULT_NVRAM \
{ \
    0, 0, \
    0, 0, \
    0, 0, \
    0, 0, \
    0, 0, \
    0, 0, \
    0, 0, \
    0, 0, \
    0, 0, \
    0, 0, \
    0, 0, \
    0, 0, \
    0, 0, \
    0, 0, \
    0, 0, \
    0, 0, \
    0, 0, \
    0, 0, \
    0, 0, \
    0, 0, \
    0, 0 \
}

/* max NVRAM size is set to 20 samples */
#define OM3990_OFFSET_CALIBRATION \
{ \
    {0, 0, 0}, \
    {0, 0, 0}, \
    {0, 0, 0}, \
    {0, 0, 0}, \
    {0, 0, 0}, \
    {0, 0, 0}, \
    {0, 0, 0}, \
    {0, 0, 0}, \
    {0, 0, 0}, \
    {0, 0, 0}, \
    {0, 0, 0}, \
    {0, 0, 0}, \
    {0, 0, 0}, \
    {0, 0, 0}, \
    {0, 0, 0}, \
    {0, 0, 0}, \
    {0, 0, 0}, \
    {0, 0, 0}, \
    {0, 0, 0}, \
    {0, 0, 0} \
}

/*============================================================================*/
/*                       ENUM OR TYPE DEFINITION                              */
/*============================================================================*/
/** 
* System Calibration NVRAM callback functions.
*/
typedef struct _OM3990_SystemCalibrationIoFunc_t
{
    tmErrorCode_t   (*Write)(UInt32 WriteLen, UInt32* pData); /**< Write NVRAM function. */
    tmErrorCode_t   (*Wait)(UInt32 tms); /**< Wait t ms without blocking scheduler. */
} OM3990_SystemCalibrationIoFunc_t, *pOM3990_SystemCalibrationIoFunc_t;

/** 
* Init System Calibration NVRAM callback functions.
*/
typedef struct _OM3990_InitSystemCalibrationIoFunc_t
{
    tmErrorCode_t   (*Read)(UInt32 ReadLen, UInt32* pData); /**< Read NVRAM function. */
} OM3990_InitSystemCalibrationIoFunc_t, *pOM3990_InitSystemCalibrationIoFunc_t;

/** 
* NVRAM Header.
*/
typedef struct _OM3990_SystemCalibrationNVRAMHeader_t
{
    UInt32  uVersion;   /**< Version of the NVRAM storage. */
    UInt32  uNbPoints;  /**< Number of samples store in the NVRAM. */
}
OM3990_SystemCalibrationNVRAMHeader_t, *pOM3990_SystemCalibrationNVRAMHeader_t;

/** 
* NVRAM Offset sample.
*/
typedef struct _OM3990_SystemCalibrationNVRAMSample_t
{
    UInt32  uRF;                  /**< RF frequency in hertz.  */
    Int32   uOffsetPowerLevel;    /**< Wide Band:   Power level Offset (x100) at the RF input, in dBμV */
    Int32   uOffsetPowerLevelNB;  /**< Narrow Band: Power level Offset (x100) at the RF input, in dBμV */
}
OM3990_SystemCalibrationNVRAMSample_t, *pOM3990_SystemCalibrationNVRAMSample_t;

/** 
* NVRAM
*/
typedef struct _OM3990_SystemCalibrationNVRAM_t
{
    OM3990_SystemCalibrationNVRAMHeader_t  tHeader;  /**< NVRAM Header.  */
    OM3990_SystemCalibrationNVRAMSample_t* pSample;  /**< Pointer to samples. */
}
OM3990_SystemCalibrationNVRAM_t, *pOM3990_SystemCalibrationNVRAM_t;

/** 
* Calibration Reference Point Definition.
*/
typedef struct _OM3990_RefPoint_t
{
    UInt32          uRF;        /**< RF frequency in Hertz.  */
    //float 
    UInt32 sRefLevel;  /**< Power level at the RF input of the box in DBμV. */
}
OM3990_RefPoint_t, *pOM3990_RefPoint_t;

/** 
* Calibration Reference Vector Definition.
*/
typedef struct _OM3990_RefVector_t
{
    UInt8                uNbPoints;     /**< Number of points in the vector. */
    OM3990_RefPoint_t*  tRefPoints;    /**< Pointer to the points.   */
}
OM3990_RefVector_t, *pOM3990_RefVector_t;

/** 
* Calibration Reference Unit Definition.
*/
typedef enum _OM3990_RefVectorPowerUnit_t
{
    OM3990_RefVectorPowerUnit_dBuV = 0,
    OM3990_RefVectorPowerUnit_dBmV = 60,  
}
OM3990_RefVectorPowerUnit_t, *pOM3990_RefVectorPowerUnit_t;

/** 
* BW selection.
*/
typedef enum _OM3990_BW_t {
    OM3990_6MHz,     /**< W = 6 MHz, in Hertz.*/
    OM3990_8MHz,     /**< W = 8 MHz, in Hertz.*/
} OM3990_BW_t, *pOM3990_BW_t;

/*============================================================================*/
/*                       EXTERN DATA DEFINITION                               */
/*============================================================================*/



/*============================================================================*/
/*                       EXTERN FUNCTION PROTOTYPES                           */
/*============================================================================*/

/*=================================================================================================*/
/**
\brief  Perform the Initialization of System calibration: Retrieve offset from NVRAM

\return TM_OK: Succeed.
\return OM3990_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
\return OM3990_ERR_BAD_PARAMETER: Parameters out of range.
\return Other optional return codes of the functions registered 
in DeviceOpenInstance API (I2C read errors, mutex...).

\note  

\sa     NA
*/
#if 0

/*=================================================================================================*/
tmErrorCode_t
    tmutilOM3990_InitSystemCalibration(
    tmUnitSelect_t              tUnitTDA18204,      /**< I: TDA18204 Unit number. */
    tmUnitSelect_t              tUnitTDA18280,      /**< I: TDA18280 Unit number. */
    OM3990_InitSystemCalibrationIoFunc_t* pInitSystemCalibrationFunc /**< I: Pointer to callback functions. */
    );

/*=================================================================================================*/
/**
\brief  Perform the System calibration: compute and store offset 
for each of the reference points (Frequency, Reference_level) given, done only once in Factory phase.

\return TM_OK: Succeed.
\return OM3990_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
\return OM3990_ERR_BAD_PARAMETER: Parameters out of range.
\return Other optional return codes of the functions registered 
in DeviceOpenInstance API (I2C read errors, mutex...).

\note  To be performed once HwInit functions has been called. Only once in Factory phase.

\sa     NA
*/
/*=================================================================================================*/		
tmErrorCode_t
    tmutilOM3990_SystemCalibration(
    tmUnitSelect_t              tUnitTDA18204,      /**< I: TDA18204 Unit number. */
    tmUnitSelect_t              tUnitTDA18280,      /**< I: TDA18280 Unit number. */
    OM3990_SystemCalibrationIoFunc_t* pSystemCalibrationFunc, /**< I: Pointer to callback functions. */
    OM3990_RefVector_t*               ptRefVector, /**< I: Pointer to the vector containing the reference points. */
    OM3990_RefVectorPowerUnit_t       ePowerUnit,  /**< I: Power level unit of reference vector. */
    OM3990_BW_t                       eBW          /**< I: BandWidth used for calibration */
    );
#endif
/*=================================================================================================*/
/**
\brief  Perform the RSSI measurement: combine access to TDA18204/TDA18280.

\return TM_OK: Succeed.
\return OM3990_ERR_ERR_NO_INSTANCES: No instance allocated for this unit number.
\return OM3990_ERR_BAD_PARAMETER: Parameters out of range.
\return Other optional return codes of the functions registered 
in DeviceOpenInstance API (I2C read errors, mutex...).

\note   To be performed once HwInit functions has been called.

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmutilOM3990GetRSSI(
    tmUnitSelect_t              tUnitTDA18204,      /**< I: TDA18204 Unit number. */
    tmUnitSelect_t              tUnitTDA18280,      /**< I: TDA18280 Unit number. */
    UInt8           path,  /**< I: Path to compute the RSSI on. */
    Int32             *RSSI  /**< O: Pointer to the RSSI in dBµV*100. */
);


#if 0
/*=================================================================================================*/
/**
\brief  Retrieve the correction array applied on the RSSI measurement (used for debugging purpose).

\return TM_OK: Succeed.
\return OM3990_ERR_BAD_PARAMETER: the array size does not match the size of data stored.
\return Other optional return codes of the functions registered 
in DeviceOpenInstance API (I2C read errors, mutex...).

\note This function read the current correction array used, then read the correction array stored in NVRAM 
and apply it as the the current correction array. This allow to apply corection stored in NVRAM if it is not
already done.

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmutilOM3990_GetRssiCorrectionArray(
    OM3990_SystemCalibrationNVRAMSample_t **pOffsetCalibration,
    OM3990_SystemCalibrationNVRAMSample_t **pOffsetCalibrationNVRAM,
    OM3990_InitSystemCalibrationIoFunc_t* pInitSystemCalibrationFunc
);

void 
itmutilOM3990_CalculateFFT(
        short int dir, long SampleNb, double *x, double *y
        );
/*=================================================================================================*/
/**
\brief  Perform the FFT.

\return TM_OK: Succeed.
\return OM3990_ERR_BAD_PARAMETER: the array size does not match the size of data stored.
\return Other optional return codes of the functions registered 
in DeviceOpenInstance API (I2C read errors, mutex...).

\note   NA

\sa     NA
*/
/*=================================================================================================*/
tmErrorCode_t
tmutilOM3990_PerformFFT(
    tmUnitSelect_t              tUnitTDA18204,      /**< I: TDA18204 Unit number. */
    tmUnitSelect_t              tUnitTDA18280,      /**< I: TDA18280 Unit number. */
    UInt16 nbSamples,                   /**< I: number of samples (max = 0x4000). */
    UInt16 nbCaptures,                  /**< I: number of FFT used for average calculation. */
    double *fft,                        /**< O: pointer on the array containing the FFT (contains nbSamples/2 elements). */
    ptmTDA18280_FFTResults_t fftResult, /**< O: FFT feature. */
    double *fftCorrected /* in dBuV */
);
#endif
#ifdef __cplusplus
}
#endif

#endif /* tmutilOM3990_H */
/*============================================================================*/
/*                            END OF FILE                                     */
/*============================================================================*/

