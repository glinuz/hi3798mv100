/*
Copyright (C) 2010 NXP B.V., All Rights Reserved.
This source code and any compilation or derivative thereof is the proprietary
information of NXP B.V. and is confidential in nature. Under no circumstances
is this software to be  exposed to or placed under an Open Source License of
any type without the expressed written permission of NXP B.V.
*
* \file          tmbslTDA18280_Local.h
*
*                %version: 1 %
*
* \date          %modify_time%
*
* \author        LC
*
* \brief         Describe briefly the purpose of this file.
*
* REFERENCE DOCUMENTS :
*                TDA18280_Driver_User_Guide.pdf
*
* \par Changelog
*  -1.1.4 Lower limit for extended SR mode changed from 4Msym/s to 3Msym/s
*     Various optimizations for CCI (CTL loop mainly)
*  -1.1.3 Improve robustness to driver re-instanciation
*  -1.1.2 Simplify DeviceHwInit function
*     Avoid I2C reads in BlockDAC_GetCfg when DAC is disabled
*     2MHz filter (OOB only) not supported due to internal limitations (for ES1 and ES2)
*     Fix frequency offset estimation when offset is almost zero
*     Some fixes in TSMF functions
*  -1.1.1 IRQ functions not supported in ES1 samples
*     Add support for polarity of IRQ output pin
*  -1.1.0 reduce the number of wait loops in iTDA18280_BlockADCCalibration
*     skip useless read of digital DDC gain when tuning a wide band path
*     add support to avoid 64-bit divisions in PathConfig and PathAuxConfig
*     remove TDA18288 product name
*     remove tmTDA18280_ProductTypeAuto
*     remove math.h library include from the driver (FFT): math.h, tmbslTDA18280_PerformFFT, iTDA18280_CalculateFFT are moved to tmutilOM3990 component
*     fine tuning of digital AGC for paths to DAC
*     replace hard-coded check "tmTDA18280_ProductTypeTDA18256" test by feature "DACSupported" in driver
*     adjust allowed number of paths for each product
*     restrict OOB feature to compatible products
*  -1.3.14 Fix demod lock issue for 128QAM in symbol rate range 4.1-4.7Msym/s
*     Fix max array index in GetTemperature
*  -1.3.13 add API to select extended timing recovery
*     EqFfLeakageAlpha adapt for lock issue with short echo at 0.2us
*  -1.3.12 Remove float in Cordic programming
*     Improve accuracy of channel center frequency for WB
*  -1.3.11 new IRQ APIs ( PathIrqSet & PathIrqRead )
*  -1.3.10 Remove double type from iTDA18280_BlockPLL1CheckLock
*     Remove useless function iTDA18280_UtilInt
*  -1.3.9 Proper TS interface reset when NB is disabled
*     Prevent bad parameters for PathSet and PathGet
*  -1.3.8 Fix demod reset (wrong settings) when path is already enabled
*  -1.3.7 Fix PathGet when demodulator is disabled
*  -1.3.6 driver cleaning for Embedded C compiler
*  -1.3.5 Move DeviceGetHwVersion function
*  -1.3.4 Fix maximum DDC gain allowed in manual (47.9dB)
*  -1.3.3 Force reset of CDP/OOB after disabling the clock of demod blocks
*  -1.3.2 Add support for one shot and continuous mode for debugRAM
*  -1.3.1 Change settings for the DAC (Rint = 5, Coarse = 1, Fine = 10) to use a current of 4mA full scale.
*  -1.3.0 Fix support of new ES2 filters (2MHz, 5MHz and 7MHz) in WB and NB
*  -1.2.13 Support TS configuration for MSB/LSB first and short/long sync
*  -1.2.12 Disabled outputs are now pulled-down (instead of pulled-up)
*  -1.2.11 ES2: add API for IRQ management (enable/disable/read status/clear status) (Puma5 API compatibility)
*     Change settings for the DAC (Rint = 16, Coarse = 2, Fine = 0)
*     Add new filters for ES2 (5MHz, 7MHz and 2MHz for OOB path)
*     Integration of new xml file for ES2
*  -1.2.10 Set tmbslTDA18280_DeviceSetProductName() in tmbslTDA18280_Advanced
*     Move APIs tmbslTDA18280_DeviceDACSet() and tmbslTDA18280_DeviceDACGet() in tmbslTDA18280_Advanced
*     Add new filters for ES2 (5MHz, 7MHz and 2MHz for OOB path)
*     Change settings for the DAC (Rint = 16, Coarse = 2, Fine = 0)
*  -1.2.9 Take into account ES2 xml
*     Set tda18280_reg_dbg_ram and tda18280_reg_noclone registers from RO->RW (delivery of second xml package from DV)
*     Properly update path config in iTDA18280_PathDemodStartup()
*     Add POD feature for OOB
*  -1.2.8 In path setting UI of TDA18256, when programming path 4-7, frequency and BW are systematically resetted to default value when we do a read
*     Potential cast problem for wanted_frequency in the function iTDA18280_BlockPLL1CheckLock()
*     In case of TDA18256 and 3 < path <= 7, TDA18280_DIG_ADC_TARGET_RMS 0x40 -> 0x11
*     Rename TDA18280_DIG_ADC_TARGET_RMS by TDA18280_DIG_AGC_TARGET_RMS
*     Set tmbslTDA18280_DeviceSetProductName() and tmbslTDA18280_DeviceGetProductName() in tmbslTDA18280 because it is used in tmutilOM3990
*     Add 2 new APIs tmbslTDA18280_DeviceDACSet() and tmbslTDA18280_DeviceDACGet() in tmbslTDA18280 driver
*     Remove unusefull parameter eUpmixerUpsampling in DAC cfg structure
*     Remove comment containing Debug DAC
*     Update iTDA18280_BlockDAC_GetCfg()
*     Rework iTDA18280_PathDdcStartup() and iTDA18280_PathDdcStop() by configuring correctly DAC inside
*     Add protection in tmbslTDA18280_DeviceSetProductName() in order to force customer application to call tmbslTDA18280_DeviceHwInit() after product name set
*     Add new API tmbslTDA18280_PathGetDemodInterleaverMode() to get the interleaver mode
*  -1.2.7 Remove warnings for sample code
*  -1.2.6 In path get function, doesn't retrieve demod informations if product=TDA18256 and 3 < path <= 7 (ex gain = 0)
*  -1.2.5 Improve the smooth WB ADC switch on (add reset of sequencers)
*  -1.2.4 Support TS clock mode (gated, permanent)
*  -1.2.3 Functions [Set/Get]ProductName remove from API
*  -1.2.2 Add APIs tmbslTDA18280_DeviceSetProductName() and tmbslTDA18280_DeviceGetProductName()
*  -1.2.1 Fix CDP frequency offset acquisition
*  -1.2.0 Consolidate clock on/off order and reset triggering
*  -1.1.4 Support smooth WB ADC switch on and off
*  -1.1.3 Prevent reading digital AGC for disabled paths
*  -1.1.2 Roll-back progressive enabling of WB ADC; add reset of RFIF and RFDSP
*  -1.1.1 Fix pad enabling for common TS clock of TDA18256 version
*  -1.1.0 Support new power states (WBOnly, WBAndNB, NBOnly), manage smoothly the state transitions and optimize the power consumption.
*  -1.0.10 Code cleaning: RF AGC management
*  -1.0.9 fix NB residual BER through NB ADC clock inversion
*     Update all documentation for delivery of TDA18256 + TDA18204 to customers
*  -1.0.8 Fix the TS7 input switch (interchange between OOB and CDP7)
*  -1.0.7 Disable AUX AGC output and powerdown NB ADC when NB path is in stand-by
*  -1.0.6 Update tmTDA18280_PNBERWindow_t enum
*  -1.0.5 Fix wrong I2C read after reading debug ram registers with disabled clock
*  -1.0.4 Add StartLock function
*  -1.0.3 Update frequencies of NB DDC
*  -1.0.2 Replace EmergencyMode with NBOnly
*     Allow OOB lock in programing oob_freq_shift registers, and changing internal IF to 5MHz
*     Add new APIs tmbslTDA18280_PathGetDemodSR() and tmbslTDA18280_PathGetDemodSI()
*  -1.0.1 Allow OOB lock in programing oob_freq_shift registers
*  -1.0.0 Replace EmergencyMode with NBOnly
*  -0.1.17 TDA18280 : Put some demod APIs in Advanced mode
*  -0.1.16 OM18280 and OM3990 initial delivery version after debug with ES
*  -0.1.15 Improve TS index management
*  -0.1.14 add variable type for TS output selection
*  -0.1.13 fix bug in disabling auxiliary path (which matches TS2)
*  -0.1.12 Fix for OOB (I2C read/write, associated path)
*  -0.1.11 Roll-back useless permutation of CDP 5 and 7
*  -0.1.10 Variable type cleaning for path/DDC/CDP/TS numbers
*  -0.1.9 improve On to Off path transition
*  -0.1.8 Fix DAC enabling with pd_dac_debug_vih
*  -0.1.7 Reset and COR_FREQ fixed
*  -0.1.6 Set default values in PathGet. Add low pass filter programming for WB.
*  -0.1.5 Fix OOB I2C read/write, improve log messages
*  -0.1.4 Fix I2C read/write for CDP
*  -0.1.3 Fix DeviceHwInit for demods (clocks and reset)
*  -0.1.2 Remove references to Serdes and HSIO
*  -0.1.1 Skip I2C read in GetCdpIndex
*  -0.1.0 Support DAC and channel stacker (TDA18256)
*  -0.0.1 Initial version
*
* \par Version_Definition
*  VERSION_TAG:TDA18280_COMP_NUM.TDA18280_MAJOR_VER.TDA18280_MINOR_VER
*
*/

#pragma once
#ifndef _TMBSL_TDA18280_LOCAL_H
#define _TMBSL_TDA18280_LOCAL_H

#ifdef __cplusplus
extern "C"
{
#endif

/*============================================================================*/
/* Types and defines:                                                         */
/*============================================================================*/
/* Driver version definition */
#define TDA18280_COMP_NUM  1 /* Major protocol change - Specification change required */
#define TDA18280_MAJOR_VER 1 /* Minor protocol change - Specification change required */
#define TDA18280_MINOR_VER 4 /* Software update - No protocol change - No specification change required */

#ifdef PRINT_I2C
    /** 
    * Macro to expand all Bitfield definition constants.
    */
#define BF(x) x##_##Base_Address, x##_##Address, (Char*)#x, x##_##PositionInBits, x##_##WidthInBits
    /** 
    * Macro to expand all register definition constants.
    */
#define REG(x) x##_##Base_Address, x##_##Address, (Char*)#x
    /** 
    * Macro to expand all Bitfield definition constants with an offset on the Base address.
    */
#define BFBO(x,o) x##_##Base_Address+o*0x200, x##_##Address, (Char*)#x,x##_##PositionInBits, x##_##WidthInBits
    /** 
    * Macro to expand all Bitfield definition constants with an offset on the address.
    */
#define BFAO(x,o) x##_##Base_Address, x##_##Address+o, (Char*)#x, x##_##PositionInBits, x##_##WidthInBits
    /** 
    * Macro to expand all Bitfield definition constants with an offset on the Base address and an offset on the address.
    */
#define BFBOAO(x,bo, ao) x##_##Base_Address+bo*0x200, x##_##Address+ao, (Char*)#x, x##_##PositionInBits, x##_##WidthInBits
    /** 
    * Macro to expand all Bitfield definition constants with an offset on the address and an offset on the bit position.
    */
#define BFAOBO(x,ao, bo) x##_##Base_Address, x##_##Address+ao, (Char*)#x, x##_##PositionInBits+bo, x##_##WidthInBits
    /** 
    * Macro to expand all register definition constants with an offset on the Base address.
    */
#define REGBO(x,o) x##_##Base_Address+o*0x200, x##_##Address, (Char*)#x
    /** 
    * Macro to expand all register definition constants with an offset on the address.
    */
#define REGAO(x,o) x##_##Base_Address, x##_##Address+o, (Char*)#x
    /** 
    * Macro to expand all register definition constants with an offset on the address and an offset on the Base address.
    */
#define REGBOAO(x,bo, ao) x##_##Base_Address+bo*0x200, x##_##Address+ao, (Char*)#x
#else
    /** 
    * Macro to expand all Bitfield definition constants.
    */
#define BF(x) x##_##Base_Address, x##_##Address, x##_##PositionInBits, x##_##WidthInBits
    /** 
    * Macro to expand all register definition constants.
    */
#define REG(x) x##_##Base_Address, x##_##Address
    /** 
    * Macro to expand all Bitfield definition constants with an offset on the Base address.
    */
#define BFBO(x,o) x##_##Base_Address+o*0x200, x##_##Address, x##_##PositionInBits, x##_##WidthInBits
    /** 
    * Macro to expand all Bitfield definition constants with an offset on the address.
    */
#define BFAO(x,o) x##_##Base_Address, x##_##Address+o, x##_##PositionInBits, x##_##WidthInBits
    /** 
    * Macro to expand all Bitfield definition constants with an offset on the Base address and an offset on the address.
    */
#define BFBOAO(x,bo, ao) x##_##Base_Address+bo*0x200, x##_##Address+ao, x##_##PositionInBits, x##_##WidthInBits
    /** 
    * Macro to expand all Bitfield definition constants with an offset on the address.
    */
#define BFAOBO(x,ao, bo) x##_##Base_Address, x##_##Address+ao, x##_##PositionInBits+bo, x##_##WidthInBits
    /** 
    * Macro to expand all register definition constants with an offset on the Base address.
    */
#define REGBO(x,o) x##_##Base_Address+o*0x200, x##_##Address
    /** 
    * Macro to expand all register definition constants with an offset on the address.
    */
#define REGAO(x,o) x##_##Base_Address, x##_##Address+o
    /** 
    * Macro to expand all register definition constants with an offset on the address and an offset on the Base address.
    */
#define REGBOAO(x,bo, ao) x##_##Base_Address+bo*0x200, x##_##Address+ao
#endif

    /** 
    * Generic values related to the HW architecture.
    */
#define TDA18280_NB_PATH 9 // tmTDA18280_Path_Max
#define TDA18280_NB_CDP 8 // number of demod
#define TDA18280_AUX_CDP_PATH tmTDA18280_Path_2 // index of the CDP used with Aux path
#define TDA18280_OOB_DDC_PATH tmTDA18280_Path_7 // index of the DDC used with OOB (AUX can also be used)
#define TDA18280_TSMF_AVAILABLE_TS_NUMBER 15

    /** 
    * CPU Endianness.
    */
#define TDA18280_CPU_LITTLEENDIAN 0 /**<  Little Endian */
#define TDA18280_CPU_BIGENDIAN 1 /**< Big Endian */

    /** 
    * Maximum gain read used for TEQ (x10 dB)
    */
#define TDA18280_TEQ_MAX_GAIN       480

    /** 
    * Masks for manual sequencing of individual pon (PON_ENABLES register)
    */
#define PON_INT_DFT_MASK 0xF81C /* bits to enable first in PON_ENABLES */
#define PON_INT_DEL1_MASK 0x02E0 /* bits to enable second in PON_ENABLES */
#define PON_INT_DEL2_MASK 0x0503 /* bits to enable third in PON_ENABLES */

    /** 
    * Xtal Oscillator Frequency in Hz.
    */
#define TDA18280_XTAL_FREQ  27000000 /**< Xtal Oscillator Frequency in Hz.*/

    /** 
    * VCO calculation factor = (2*27e6/128) / 1e6
    */
#define TDA18280_VCO_CALCULATION_FACTOR_x1E6 421875 // TDA18280_VCO_CALCULATION_FACTOR = 0.421875

    /** 
    * 32 bit maximum value.
    */
#define MAX_32_BITS 0xFFFFFFFF

    /** 
    * Absolute value macro.
    */
#define ABS(a) ((a)>=0?(a):-(a))

    /* Instance macros */
#define P_OBJ_VALID                             (pObj != Null)

    /* I/O Functions macros */
#define P_SIO                                   pObj->sIo
#define P_SIO_READ                              P_SIO.Read
#define P_SIO_WRITE                             P_SIO.Write
#define P_SIO_READ_VALID                        (P_OBJ_VALID && (P_SIO_READ != Null))
#define P_SIO_WRITE_VALID                       (P_OBJ_VALID && (P_SIO_WRITE != Null))

    /* Time Functions macros */
#define P_STIME                                 pObj->sTime
#define P_STIME_WAIT                            P_STIME.Wait
#define P_STIME_WAIT_VALID                      (P_OBJ_VALID && (P_STIME_WAIT != Null))

    /* Debug Functions macros */
#define P_SDEBUG                                pObj->sDebug
#define P_DBGPRINTEx                            P_SDEBUG.Print
#define P_DBGPRINTVALID                         (P_OBJ_VALID && (P_DBGPRINTEx != Null))

    /* Mutex Functions macros */
#define P_SMUTEX                                pObj->sMutex
#ifdef _TVFE_SW_ARCH_V4
#define P_SMUTEX_OPEN                           P_SMUTEX.Open
#define P_SMUTEX_CLOSE                          P_SMUTEX.Close
#else
#define P_SMUTEX_OPEN                           P_SMUTEX.Init
#define P_SMUTEX_CLOSE                          P_SMUTEX.DeInit
#endif
#define P_SMUTEX_ACQUIRE                        P_SMUTEX.Acquire
#define P_SMUTEX_RELEASE                        P_SMUTEX.Release

#define P_SMUTEX_OPEN_VALID                     (P_OBJ_VALID && (P_SMUTEX_OPEN != Null))
#define P_SMUTEX_CLOSE_VALID                    (P_OBJ_VALID && (P_SMUTEX_CLOSE != Null))
#define P_SMUTEX_ACQUIRE_VALID                  (P_OBJ_VALID && (P_SMUTEX_ACQUIRE != Null))
#define P_SMUTEX_RELEASE_VALID                  (P_OBJ_VALID && (P_SMUTEX_RELEASE != Null))

    /* Driver Mutex macros */
#define TDA18280_MUTEX_TIMEOUT                  (5000)
#define P_MUTEX                                 pObj->pMutex
#define P_MUTEX_VALID                           (P_MUTEX != Null)

#ifdef _TVFE_IMPLEMENT_MUTEX
#define _MUTEX_ACQUIRE(_NAME) \
    if(err == TM_OK) \
    { \
    /* Try to acquire driver mutex */ \
    err = i##_NAME##_MutexAcquire(pObj, _NAME##_MUTEX_TIMEOUT); \
} \
    if(err == TM_OK) \
    {

#define _MUTEX_RELEASE(_NAME) \
    (void)i##_NAME##_MutexRelease(pObj); \
}
#else
#define _MUTEX_ACQUIRE(_NAME) \
    if(err == TM_OK) \
    {

#define _MUTEX_RELEASE(_NAME) \
}
#endif

/* mask used for CDP instances */
#define ADDITIONNAL_INFO_MASK                  (0x007C0000)
#define ADDITIONNAL_INFO_POS                   (18)

#define ADDITIONNAL_INFO_GET(_tUnIt)          (((_tUnIt)&ADDITIONNAL_INFO_MASK) >> ADDITIONNAL_INFO_POS)
#define ADDITIONNAL_INFO_VAL(_val)            (((_val)<<ADDITIONNAL_INFO_POS)&ADDITIONNAL_INFO_MASK)
#define ADDITIONNAL_INFO_SET(_tUnIt, _val)    ( ((_tUnIt)&~ADDITIONNAL_INFO_MASK) | ADDITIONNAL_INFO_POS(_val) )
#define ADDITIONNAL_INFO_VAL_GET(_val)        (ADDITIONNAL_INFO_VAL(ADDITIONNAL_INFO_GET(_val)))

/* mask used for TSMF registers */
#define TDA18280_BYPASS                  0x3000
#define TDA18280_BYPASS_BYPASS_MSK       0x1
#define TDA18280_BYPASS_BYPASS_DISABLE   0x0
#define TDA18280_BYPASS_BYPASS_ENABLE    0x1

#define TDA18280_MODE                    0x3002
#define TDA18280_MODE_BYPASS_MSK         0xF
#define TDA18280_MODE_BYPASS_DISABLE     0x0
#define TDA18280_MODE_BYPASS_ENABLE      0x1
#define TDA18280_MODE_BYPASS_AUTOMATIC   0x2

#define TDA18280_REG0x05                 0x3008
#define TDA18280_REG0x05_M_LOCK_MSK      0x01
#define TDA18280_REG0x05_M_LOCK_BIT      0
#define TDA18280_REG0x05_VER_MSK         0x0E
#define TDA18280_REG0x05_VER_BIT         1         
#define TDA18280_REG0x05_CRE_ST_MSK      0x30
#define TDA18280_REG0x05_CRE_ST_BIT      4
#define TDA18280_REG0x05_EMERGENCY_MSK   0x40
#define TDA18280_REG0x05_EMERGENCY_BIT   6
#define TDA18280_REG0x05_ERROR_MSK       0x80
#define TDA18280_REG0x05_ERROR_BIT       7

#define TDA18280_TS_ID                   0x3004
#define TDA18280_ON_ID                   0x3006

#define TDA18280_TS_STATUS               0x300A
#define TDA18280_RE_STATUS_MSB           0x300C
#define TDA18280_RE_STATUS_LSB           0x300E

#define BIT_MSK                          0x01

#define TDA18280_TS_ID_OFFSET            0x4
#define TDA18280_ON_ID_OFFSET            0x4

#define TDA18280_TS_ID1                  0x3010
#define TDA18280_ON_ID1                  0x3012

#define TDA18280_TS_ID2                  0x3014
#define TDA18280_ON_ID2                  0x3016

#define TDA18280_TS_ID3                  0x3018
#define TDA18280_ON_ID3                  0x301A

#define TDA18280_TS_ID4                  0x301C
#define TDA18280_ON_ID4                  0x301E

#define TDA18280_TS_ID5                  0x3020
#define TDA18280_ON_ID5                  0x3022

#define TDA18280_TS_ID6                  0x3024
#define TDA18280_ON_ID6                  0x3026

#define TDA18280_TS_ID7                  0x3028
#define TDA18280_ON_ID7                  0x302A

#define TDA18280_TS_ID8                  0x302C
#define TDA18280_ON_ID8                  0x302E

#define TDA18280_TS_ID9                  0x3030
#define TDA18280_ON_ID9                  0x3032

#define TDA18280_TS_ID10                 0x3034
#define TDA18280_ON_ID10                 0x3036

#define TDA18280_TS_ID11                 0x3038
#define TDA18280_ON_ID11                 0x303A

#define TDA18280_TS_ID12                 0x303C
#define TDA18280_ON_ID12                 0x303E

#define TDA18280_TS_ID13                 0x3040
#define TDA18280_ON_ID13                 0x3042

#define TDA18280_TS_ID14                 0x3044
#define TDA18280_ON_ID14                 0x3046

#define TDA18280_TS_ID15                 0x3048
#define TDA18280_ON_ID15                 0x304A

#define RAM_SIZE 16384
#define STATUS_TIMEOUT 40 // arbitrary value

    /** 
    * Auxiliary DDC clock divider definition (NBDDCfreq=(FreqWBADC/16)/(DivNBDDC).
    */
    typedef enum _TDA18280_DivNBDDC_t
    {
        tmTDA18280_DivNBDDC_3 = 0,   /**< Auxiliary DDC clock divider = 3.*/
        tmTDA18280_DivNBDDC_4        /**< Auxiliary DDC clock divider = 4.*/
    }
    TDA18280_DivNBDDC_t, *pTDA18280_DivNBDDC_t;

    /** 
    * Auxiliary ADC clock selection.
    */
    typedef enum _TDA18280_NBADCFastSel_t
    {
        tmTDA18280_NBADCFastSel_slow = 0,   /**< Auxiliary ADC slow clock.*/
        tmTDA18280_NBADCFastSel_fast        /**< Auxiliary ADC fast (*2) clock.*/
    }
    TDA18280_NBADCFastSel_t, *pTDA18280_NBADCFastSel_t;

    /**
    * NB ADC sample frequency
    */
    typedef enum _TDA18280_NBADCSampleFreq_t {
        tmTDA18280_NBADCSampleFreq28MHz125,
        tmTDA18280_NBADCSampleFreq56MHz25,
        tmTDA18280_NBADCSampleFreqInvalid
    } 
    TDA18280_NBADCSampleFreq_t, *p_tmTDA18280_NBADCSampleFreq_t;

    /* TDA18280 Driver State Machine */
    typedef enum _tmTDA18280_HwState_t {
        tmTDA18280_HwState_Unknown = 0,   /* Hw State Unknown */
        tmTDA18280_HwState_InitNotDone,   /* Hw Init Not Done */
        tmTDA18280_HwState_InitPending,   /* Hw Init Pending */
        tmTDA18280_HwState_InitDone,      /* Hw Init Done */
        tmTDA18280_HwState_Max
    } tmTDA18280_HwState_t, *ptmTDA18280_HwState_t;

    /** 
    * Temperature
    */
    typedef enum _tmTDA18280_TemperatureSource_t {
        tmTDA18280_TemperatureSourceHS,      /**< HS */
        tmTDA18280_TemperatureSourceLO,      /**< LO */
        tmTDA18280_TemperatureSourceRFADC,   /**< RF ADC */
        tmTDA18280_TemperatureSourceInvalid
    } tmTDA18280_TemperatureSource_t, *p_tmTDA18280_TemperatureSource_t;

    /**
    * Debug RAM
    */
    typedef enum _tmTDA18280_DebugRamState_t
    {
        tmTDA18280_DebugRamDisabled,
        tmTDA18280_DebugRamEnabled
    } tmTDA18280_DebugRamState_t, *ptmTDA18280_DebugRamState_t;

    typedef enum _tmTDA18280_DebugRamCtrlMode_t
    {
        tmTDA18280_DebugRamCtrlMode_Reset = 0,
        tmTDA18280_DebugRamCtrlMode_Stuff_Rfdsp = 1,
        tmTDA18280_DebugRamCtrlMode_Store_Adc = 2,
        tmTDA18280_DebugRamCtrlMode_Store_Adc_Ds = 3, /* ADC after downsampling */
        tmTDA18280_DebugRamCtrlMode_Store_Nb_Adc = 4,
        tmTDA18280_DebugRamCtrlMode_Dump_Ram = 5,
        tmTDA18280_DebugRamCtrlMode_Fill_Ram = 6,
        tmTDA18280_DebugRamCtrlMode_I2c_Rd = 7,
        tmTDA18280_DebugRamCtrlMode_I2c_Wr = 8,
        tmTDA18280_DebugRamCtrlMode_Stuff_Demod = 9,
        tmTDA18280_DebugRamCtrlMode_Store_Ddc = 10,
        tmTDA18280_DebugRamCtrlMode_Store_Nb_Ddc = 11
    } tmTDA18280_DebugRamCtrlMode_t, *ptmTDA18280_DebugRamCtrlMode_t;

        typedef enum _tmTDA18280_DebugRamCtrlOneShot_t
    {
        tmTDA18280_DebugRamCtrlOneShot_Continuous = 0,
        tmTDA18280_DebugRamCtrlOneShot_OneShot = 1
    } tmTDA18280_DebugRamCtrlOneShot_t, *ptmTDA18280_DebugRamCtrlOneShot_t;

    /*
    * RF AGG configs
    */
    typedef enum  _tmTDA18280_RfAgcCtrl_t
    {
        tmTDA18280_RfAgcCtrl_Disabled = 0,
        tmTDA18280_RfAgcCtrl_Enabled = 1,
        tmTDA18280_RfAgcCtrl_Max = 2
    } tmTDA18280_RfAgcCtrl_t, *ptmTDA18280_RfAgcCtrl_t;

    typedef enum  _tmTDA18280_RfAgcMode_t
    {
        tmTDA18280_RfAgcMode_RMS = 0,
        tmTDA18280_RfAgcMode_Peak = 1,
        tmTDA18280_RfAgcMode_RMSAndPeak = 2
    } tmTDA18280_RfAgcMode_t, *ptmTDA18280_RfAgcMode_t;

    /*
    * IRQ configs
    */
    typedef enum  _tmTDA18280_IrqMode_t
    {
        tmTDA18280_IrqMode_Disabled = 0,
        tmTDA18280_IrqMode_Enabled = 1
    } tmTDA18280_IrqMode_t, *ptmTDA18280_IrqMode_t;

    typedef enum  _tmTDA18280_IrqPolarity_t
    {
        tmTDA18280_IrqPolarity_ActiveHigh = 0,
        tmTDA18280_IrqPolarity_ActiveLow = 1
    } tmTDA18280_IrqPolarity_t, *ptmTDA18280_IrqPolarity_t;

    /** 
    * PLL configuration Definition.
    */
    typedef struct _tmTDA18280_PllCfg_t
    {
        tmTDA18280_SelPLL1FreqMHz_t     pll1Freq;    /**<  pll1 configuration */
        tmTDA18280_LOChainMode_t        LOChainMode; /**<  LOChainMode , 1, 3, 4, 5 or 8.*/  
        tmTDA18280_deciDDC_t            deciDDC;     /**<  Decimation factor of DDC. */
        TDA18280_DivNBDDC_t             DivNBDDC;    /**< Auxiliary DDC clock divider definition (NBDDCfreq=(FreqWBADC/16)/(DivNBDDC).*/
        TDA18280_NBADCSampleFreq_t      NBADCSampleFreq;  /**< Auxiliary ADC Sample frequency. */
    }
    tmTDA18280_PllCfg_t, *ptmTDA18280_PllCfg_t;

    typedef struct _tmTDA18280_LogTable_t
    {
        UInt32 uX;
        UInt32 uLogX;  /*100*log(x)*/
    } tmTDA18280_LogTable_t;
    
    /** 
    * Wideband RF AGC config (mixed signal AGC)
    */
    typedef struct _tmTDA18280_RfAgcCfg_t
    {
        tmTDA18280_RfAgcCtrl_t  eRfAgcCtrl;
        tmTDA18280_RfAgcMode_t  eRfAgcMode;
    }
    tmTDA18280_RfAgcCfg_t, *ptmTDA18280_RfAgcCfg_t;

    /** 
    * Interrupt pin (IRQ) configuration
    */
    typedef struct _tmTDA18280_IrqCfg_t
    {
        tmTDA18280_IrqMode_t      eIrqMode;
        tmTDA18280_IrqPolarity_t  eIrqPolarity;
    }
    tmTDA18280_IrqCfg_t, *ptmTDA18280_IrqCfg_t;

    /** 
    * Demodulator CDP/OOB selection.
    */
    typedef enum _tmTDA18280_Demod_t {
        tmTDA18280_Demod_CDP_0=0,   /**< Select CDP 0.*/
        tmTDA18280_Demod_CDP_1,     /**< Select CDP 1.*/
        tmTDA18280_Demod_CDP_2,     /**< Select CDP 2.*/
        tmTDA18280_Demod_CDP_3,     /**< Select CDP 3.*/
        tmTDA18280_Demod_CDP_4,     /**< Select CDP 4.*/
        tmTDA18280_Demod_CDP_5,     /**< Select CDP 5.*/
        tmTDA18280_Demod_CDP_6,     /**< Select CDP 6.*/
        tmTDA18280_Demod_CDP_7,     /**< Select CDP 7.*/
        tmTDA18280_Demod_OOB,       /**< Select OOB.*/
        tmTDA18280_Demod_None,      /**< Select no demod.*/
        tmTDA18280_Demod_Max
    } tmTDA18280_Demod_t, *ptmTDA18280_Demod_t;

    /** 
    * TS output selection.
    */
    typedef enum _tmTDA18280_Ts_t {
        tmTDA18280_Ts_0=0,   /**< Select TS output 0.*/
        tmTDA18280_Ts_1,     /**< Select TS output 1.*/
        tmTDA18280_Ts_2,     /**< Select TS output 2.*/
        tmTDA18280_Ts_3,     /**< Select TS output 3.*/
        tmTDA18280_Ts_4,     /**< Select TS output 4.*/
        tmTDA18280_Ts_5,     /**< Select TS output 5.*/
        tmTDA18280_Ts_6,     /**< Select TS output 6.*/
        tmTDA18280_Ts_7,     /**< Select TS output 7.*/
        tmTDA18280_Ts_Max
    } tmTDA18280_Ts_t, *ptmTDA18280_Ts_t;

#define TDA18280_LOG_TABLE \
    /* X   ,  1000*LogX */ \
    {1,0}, \
    {2,301}, \
    {3,477}, \
    {4,602}, \
    {5,698}, \
    {6,778}, \
    {7,845}, \
    {8,903}, \
    {9,954}, \
    {10,1000}, \
    {11,1041}, \
    {12,1079}, \
    {13,1113}, \
    {14,1146}, \
    {15,1176}, \
    {16,1204}, \
    {17,1230}, \
    {18,1255}, \
    {19,1278}, \
    {20,1301}, \
    {21,1322}, \
    {22,1342}, \
    {23,1361}, \
    {24,1380}, \
    {25,1397}, \
    {26,1414}, \
    {27,1431}, \
    {28,1447}, \
    {29,1462}, \
    {30,1477}, \
    {31,1491}, \
    {32,1505}, \
    {33,1518}, \
    {34,1531}, \
    {35,1544}, \
    {36,1556}, \
    {37,1568}, \
    {38,1579}, \
    {39,1591}, \
    {41,1612}, \
    {42,1623}, \
    {43,1633}, \
    {45,1653}, \
    {46,1662}, \
    {48,1681}, \
    {49,1690}, \
    {51,1707}, \
    {52,1716}, \
    {54,1732}, \
    {56,1748}, \
    {58,1763}, \
    {60,1778}, \
    {62,1792}, \
    {63,1799}, \
    {66,1819}, \
    {68,1832}, \
    {70,1845}, \
    {72,1857}, \
    {74,1869}, \
    {77,1886}, \
    {79,1897}, \
    {82,1913}, \
    {84,1924}, \
    {87,1939}, \
    {90,1954}, \
    {93,1968}, \
    {96,1982}, \
    {99,1995}, \
    {102,2008}, \
    {105,2021}, \
    {109,2037}, \
    {112,2049}, \
    {116,2064}, \
    {120,2079}, \
    {123,2089}, \
    {127,2103}, \
    {132,2120}, \
    {136,2133}, \
    {140,2146}, \
    {145,2161}, \
    {149,2173}, \
    {154,2187}, \
    {159,2201}, \
    {164,2214}, \
    {169,2227}, \
    {175,2243}, \
    {180,2255}, \
    {186,2269}, \
    {192,2283}, \
    {198,2296}, \
    {205,2311}, \
    {211,2324}, \
    {218,2338}, \
    {225,2352}, \
    {232,2365}, \
    {240,2380}, \
    {247,2392}, \
    {255,2406}, \
    {264,2421}, \
    {272,2434}, \
    {281,2448}, \
    {290,2462}, \
    {299,2475}, \
    {309,2489}, \
    {318,2502}, \
    {329,2517}, \
    {339,2530}, \
    {350,2544}, \
    {361,2557}, \
    {373,2571}, \
    {385,2585}, \
    {397,2598}, \
    {410,2612}, \
    {423,2626}, \
    {437,2640}, \
    {451,2804}, \
    {465,2667}, \
    {480,2681}, \
    {495,2694}, \
    {511,2708}, \
    {528,2722}, \
    {544,2735}, \
    {562,2749}, \
    {580,2763}, \
    {598,2776}, \
    {618,2790}, \
    {637,2804}, \
    {658,2818}, \
    {679,2831}, \
    {701,2845}, \
    {723,2859}, \
    {746,2872}, \
    {770,2886}, \
    {795,2900}, \
    {820,2913}, \
    {846,2927}, \
    {874,2941}, \
    {901,2954}, \
    {930,2968}, \
    {960,2982}, \
    {991,2996}, \
    {1023,3009}, \
    {1055,3023}, \
    {1089,3037}, \
    {1124,3050}, \
    {1160,3064}, \
    {1197,3078}, \
    {1235,3091}, \
    {1275,3105}, \
    {1316,3119}, \
    {1358,3132}, \
    {1401,3146}, \
    {1446,3160}, \
    {1493,3174}, \
    {1540,3187}, \
    {1590,3201}, \
    {1641,3215}, \
    {1693,3228}, \
    {1747,3242}, \
    {1803,3255}, \
    {1861,3269}, \
    {1920,3283}, \
    {1982,3297}, \
    {2045,3310}, \
    {2111,3324}, \
    {2178,3338}, \
    {2248,3351}, \
    {2320,3365}, \
    {2394,3379}, \
    {2471,3392}, \
    {2550,3406}, \
    {2632,3420}, \
    {2716,3433}, \
    {2803,3447}, \
    {2892,3461}, \
    {2985,3474}, \
    {3081,3488}, \
    {3179,3502}, \
    {3281,3516}, \
    {3386,3529}, \
    {3494,3543}, \
    {3606,3557}, \
    {3722,3570}, \
    {3841,3584}, \
    {3964,3598}, \
    {4090,3611}, \
    {4221,3625}, \
    {4356,3639}, \
    {4496,3652}, \
    {4640,3666}, \
    {4788,3680}, \
    {4941,3693}, \
    {5100,3707}, \
    {5263,3721}, \
    {5431,3734}, \
    {5605,3748}, \
    {5784,3762}, \
    {5970,3775}, \
    {6161,3789}, \
    {6358,3803}, \
    {6561,3816}, \
    {6771,3830}, \
    {6988,3844}, \
    {7212,3858}, \
    {7442,3871}, \
    {7680,3885}, \
    {7926,3899}, \
    {8180,3912}, \
    {8442,3926}, \
    {8712,3940}, \
    {8991,3953}, \
    {9278,3967}, \
    {9575,3981}, \
    {9882,3994}, \
    {10198,4008}, \
    {10524,4022}, \
    {10861,4035}, \
    {11209,4049}, \
    {11567,4063}, \
    {11938,4076}, \
    {12320,4090}, \
    {12714,4104}, \
    {13121,4117}, \
    {13541,4131}, \
    {13974,4145}, \
    {14421,4158}, \
    {14883,4172}, \
    {15359,4186}, \
    {15850,4200}, \
    {16357,4213}, \
    {16881,4227}, \
    {17421,4241}, \
    {17979,4254}, \
    {18554,4268}, \
    {19148,4282}, \
    {19760,4295}, \
    {20393,4309}, \
    {21045,4323}, \
    {21719,4336}, \
    {22414,4350}, \
    {23131,4364}, \
    {23871,4377}, \
    {24635,4391}, \
    {25424,4405}, \
    {26237,4418}, \
    {27077,4432}, \
    {27943,4446}, \
    {28837,4459}, \
    {29760,4473}, \
    {30713,4487}, \
    {31695,4500}, \
    {32710,4514}, \
    {33756,4528}, \
    {34837,4542}, \
    {35951,4555}, \
    {37102,4569}, \
    {38289,4583}, \
    {39514,4596}, \
    {40779,4610}, \
    {42084,4624}, \
    {43431,4637}, \
    {44820,4651}, \
    {46255,4665}, \
    {47735,4678}, \
    {49262,4692}, \
    {50839,4706}, \
    {52466,4719}, \
    {54145,4733}, \
    {55877,4747}, \
    {57665,4760}, \
    {59511,4774}, \
    {61415,4788}, \
    {63380,4801}, \
    {65408,4815}, \
    {67501,4829}, \
    {69662,4842}, \
    {71891,4856}, \
    {74191,4870}, \
    {76565,4884}, \
    {79015,4897}, \
    {81544,4911}, \
    {84153,4925}, \
    {86846,4938}, \
    {89625,4952}, \
    {92493,4966}, \
    {95453,4979}, \
    {98508,4993}, \
    {0,0}

    /** 
    * instance structure
    */
    typedef struct _tmTDA18280_Object_t
    {
        tmUnitSelect_t                  tUnit;
        tmUnitSelect_t                  tUnitW;
        ptmbslFrontEndMutexHandle       pMutex;
        Bool                            init;
        tmbslFrontEndIoFunc_t           sIo;
        tmbslFrontEndTimeFunc_t         sTime;
        tmbslFrontEndDebugFunc_t        sDebug;
        tmbslFrontEndMutexFunc_t        sMutex;
        /* Device specific part: */
        tmTDA18280_PowerState_t         curPowerState;
        tmTDA18280_HwState_t            eHwState;
        tmTDA18280_PllCfg_t             PllCfg;
        tmTDA18280_RfAgcCfg_t           RfAgcCfg;               /**< Wideband RF AGC config (mixed signal AGC).*/
        tmTDA18280_ProductType_t        eProductType;           /**< Product type.*/ 
        tmTDA18280_EsVersion_t          EsVersion;              /**< Hardware version.*/ 
        tmTDA18280_Feature_t            tFeature;
        /* Path dependant part: */
        tmTDA18280_PathCfg_t            Path[tmTDA18280_Path_Max]; /**< Path configuration values for each path. (programmed value)*/
        tmTDA18280_ONOFF_t              PathCurrentState[tmTDA18280_Path_Max]; /**< indicate if the path is started or not.*/
        /* NarrowBand */
        TDA18280_NBADCFastSel_t         NBADCFastSel;           /**< Auxiliary ADC clock selection.*/
        /* Miscellaneous part: */
        tmTDA18280_I2CGate_t            I2CGateCfg;             /**< I2CGate configuration.*/
        tmTDA18280_TiltInputParameters_t  TiltInputParameters;  /**< TILT measure parameters INPUT */
        tmTDA18280_TiltOutputParameters_t TiltOutputParameters; /**< TILT measure parameters Output */
        tmFrontEndFECMode_t             eFECMode;               /**< FEC mode configuration */
        tmTDA18280_OOBFECMode_t         eOOBFECMode;            /**< OOB FEC mode configuration */
        tmTDA18280_Path_t               eOOBInput;              /**< input selection of OOB demod (max means none) */
        tmTDA18280_IrqCfg_t             IrqCfg;                 /**< Interrupt pin (IRQ) configuration */
        /* CDP configuration */
        tmTDA18280_PathCfgFixed_t       PathCfgFixed[tmTDA18280_Path_Max]; /**< Extra path configuration */
        int                             Marker;                 /**< Marker to check coherency of initializing constant structure in tmbslTDA18280_InstanceCustom.h. Should be eaqual to TDA18280_INSTANCE_CUSTOM_MARKER.*/\
    } tmTDA18280_Object_t, *ptmTDA18280_Object_t, **pptmTDA18280_Object_t;

    /** 
    * PathSet AGC mode
    */
    typedef enum _TDA18280_PathSetAGCMode_t
    {
        tmTDA18280_PathSetAGCMode_Normal = 0,   /**< In this mode, wait the AGC gain to be stable.*/
        tmTDA18280_PathSetAGCMode_Calibration   /**< For calibration, do not wait. At the end, the AGC are still in fast mode.*/
    }
    TDA18280_PathSetAGCMode_t, *pTDA18280_PathSetAGCMode_t;

    /*============================================================================*/
    /* Internal Prototypes:                                                       */
    /*============================================================================*/
    extern tmErrorCode_t
        iTDA18280_DevicePowerStateSet(ptmTDA18280_Object_t pObj, tmTDA18280_PowerState_t powerState);

    extern tmErrorCode_t
        iTDA18280_DevicePowerStateGet(ptmTDA18280_Object_t pObj, tmTDA18280_PowerState_t *pePowerState );

    extern tmErrorCode_t
        iTDA18280_PathDdcClockCheck(ptmTDA18280_Object_t pObj, tmTDA18280_Path_t Idx_path, UInt8* pClock_OK);

    extern tmErrorCode_t
        iTDA18280_PathDdcStartup(ptmTDA18280_Object_t pObj, tmTDA18280_Path_t PathNumber);

    extern tmErrorCode_t
        iTDA18280_PathDdcStop(ptmTDA18280_Object_t pObj, tmTDA18280_Path_t PathNumber);

    extern tmErrorCode_t
        iTDA18280_PathConfig(
        ptmTDA18280_Object_t    pObj,       /* I: Driver object */
        UInt8   Idx_ddc,
        const tmTDA18280_PathCfg_t* ptPathCfg,
        TDA18280_PathSetAGCMode_t eMode
        );

    extern tmErrorCode_t
        iTDA18280_PathAuxConfig(
        ptmTDA18280_Object_t    pObj,       /* I: Driver object */
        const tmTDA18280_PathCfg_t* ptPathCfg
        );

    extern tmErrorCode_t
        iTDA18280_PathDemodClockCheck(ptmTDA18280_Object_t pObj, tmTDA18280_Demod_t DemodIndex, UInt8* pClock_OK);

    tmErrorCode_t
        iTDA18280_PathDemodStartup(
        ptmTDA18280_Object_t  pObj,             /* I: Driver object */
        tmTDA18280_Path_t     Idx_path,         /* I: Path index */
        const tmTDA18280_PathCfg_t*  ptPathCfg  /* I: Path configuration */
        );

    tmErrorCode_t
        iTDA18280_PathDemodStop(
        ptmTDA18280_Object_t  pObj,     /* I: Driver object */
        tmTDA18280_Path_t     Idx_path  /* I: Path index */
        );

    tmErrorCode_t
        iTDA18280_GetDemodTsmfAvailableTsId(
        ptmTDA18280_Object_t   pObj,          /* I: Driver object */
        tmTDA18280_Demod_t     DemodIndex,    /* I: Demod index */
        tmTDA18280_TsmfAvailableTs_t* psTsmfAvailableTs  /* O: Pointer to available TS IDs */
        );

    tmErrorCode_t
        iTDA18280_GetDemodTsmfStatus(
        ptmTDA18280_Object_t   pObj,          /* I: Driver object */
        tmTDA18280_Demod_t     DemodIndex,    /* I: Demod index */
        tmTDA18280_TsmfStatus_t* psTsmfStatus /* O: Pointer to TSMF status */
        );

    tmErrorCode_t
        iTDA18280_GetDemodTsmfMode(
        ptmTDA18280_Object_t   pObj,        /* I: Driver object */
        tmTDA18280_Demod_t     DemodIndex,  /* I: Demod index */
        tmTDA18280_TsmfMode_t* peMode       /* O: Pointer to TSMF mode */
        );

    tmErrorCode_t
        iTDA18280_SetDemodTsmfMode(
        ptmTDA18280_Object_t   pObj,        /* I: Driver object */
        tmTDA18280_Demod_t     DemodIndex,  /* I: Demod index */
        tmTDA18280_TsmfMode_t  eTsmfMode    /* I: TSMF mode */
        );

    tmErrorCode_t
        iTDA18280_PathTsOutputStartup(
        ptmTDA18280_Object_t  pObj,     /* I: Driver object */
        tmTDA18280_Path_t     Idx_path  /* I: Path index */
        );

    tmErrorCode_t
        iTDA18280_PathTsOutputStop(
        ptmTDA18280_Object_t  pObj,     /* I: Driver object */
        tmTDA18280_Path_t     Idx_path  /* I: Path index */
        );

    extern tmErrorCode_t
        iTDA18280_NbAdcConfiguration(
        ptmTDA18280_Object_t    pObj,       /* I: Driver object */
        tmTDA18280_ONOFF_t eStatus
        );

    extern tmErrorCode_t
        iTDA18280_BlockCrossSwitchProg(
        ptmTDA18280_Object_t    pObj,       /* I: Driver object */
        UInt16  Idx_ddc,
        UInt16 cfg_cross_switch
        );

    extern tmErrorCode_t
        iTDA18280_DeviceHwInit(ptmTDA18280_Object_t pObj);

    extern tmErrorCode_t
        iTDA18280_PathReadGain(ptmTDA18280_Object_t pObj, tmTDA18280_Path_t PathNumber);

    extern tmErrorCode_t
        iTDA18280_I2CWriteBitField(ptmTDA18280_Object_t pObj, 
        UInt16 Base_Address, 
        UInt16 Address, 
#ifdef PRINT_I2C
        Char Str[50], 
#endif
        UInt8 PositionInBits, 
        UInt8 WidthInBit, 
        UInt16 uData);

    extern tmErrorCode_t
        iTDA18280_I2CReadBitField(ptmTDA18280_Object_t pObj, 
        UInt16 Base_Address, 
        UInt16 Address, 
#ifdef PRINT_I2C
        Char Str[50], 
#endif
        UInt8 PositionInBits, 
        UInt8 WidthInBit, 
        UInt16* puData);

    extern tmErrorCode_t
        iTDA18280_I2CWriteRegister(ptmTDA18280_Object_t pObj, 
        UInt16 Base_Address, 
        UInt16 Address, 
#ifdef PRINT_I2C
        Char Str[50], 
#endif
        UInt16 RegData);

    extern tmErrorCode_t
        iTDA18280_I2CReadRegister(ptmTDA18280_Object_t pObj, 
        UInt16 Base_Address, 
        UInt16 Address, 
#ifdef PRINT_I2C
        Char Str[50], 
#endif
        UInt32 uReadLen, 
        UInt16* pRegData);

    extern tmErrorCode_t
        iTDA18280_UtilWait(ptmTDA18280_Object_t pObj, UInt32 Time);

    extern int iTDA18280_UtilSignExtension(int nbbits, int value);

    extern tmErrorCode_t 
        iTDA18280_TEQMeasureIFGain(ptmTDA18280_Object_t pObj);

    extern tmErrorCode_t 
        iTDA18280_TEQDetectRejection(ptmTDA18280_Object_t pObj);

    extern tmErrorCode_t 
        iTDA18280_TEQDetectBlockOfChannel(ptmTDA18280_Object_t pObj);

    extern tmErrorCode_t 
        iTDA18280_TEQHandleAnalogDigitalStep(ptmTDA18280_Object_t pObj);

    extern tmErrorCode_t 
        iTDA18280_TEQLeastSquareAnalysis(ptmTDA18280_Object_t pObj);

    extern tmErrorCode_t 
        iTDA18280_TEQCalculateT0Square(ptmTDA18280_Object_t pObj);

    extern tmErrorCode_t 
        iTDA18280_TEQGetClosestSlope(ptmTDA18280_Object_t pObj);

    extern tmErrorCode_t
        iTDA18280_BlockPLL1Startup(ptmTDA18280_Object_t       pObj);

    extern tmErrorCode_t
        iTDA18280_BlockCGUAndResetStopWBNB(ptmTDA18280_Object_t pObj);

    extern tmErrorCode_t
        iTDA18280_BlockCGUAndResetInitWBNB(ptmTDA18280_Object_t pObj);

    extern tmErrorCode_t
        iTDA18280_BlockCGUAndResetStartupWB(ptmTDA18280_Object_t pObj);

    extern tmErrorCode_t
        iTDA18280_BlockCGUAndResetStopWB(ptmTDA18280_Object_t pObj);

    extern tmErrorCode_t
        iTDA18280_BlockLOChainConfigAndStartup(ptmTDA18280_Object_t pObj, tmTDA18280_LOChainMode_t LOChainMode);

    extern tmErrorCode_t
        iTDA18280_BlockLOChainStop(ptmTDA18280_Object_t pObjp);

    extern tmErrorCode_t
        iTDA18280_BlockPLL1CheckLock(
        ptmTDA18280_Object_t       pObj,
        UInt16 max_delay_ms,
        UInt32 wanted_freq_MHz
        );

    extern tmErrorCode_t
        iTDA18280_BlockADCStop(ptmTDA18280_Object_t pObj);

    extern tmErrorCode_t
        iTDA18280_BlockADCStartup(ptmTDA18280_Object_t pObj);

    extern tmErrorCode_t
        iTDA18280_BlockADCCalibration(ptmTDA18280_Object_t pObj);

    extern tmErrorCode_t 
        iTDA18280_BlockDAC_SetCfg( 
        ptmTDA18280_Object_t pObj, 
        tmTDA18280_DACConfiguration_t* ptConfiguration
        );

    extern tmErrorCode_t 
        iTDA18280_BlockDAC_GetCfg( 
        ptmTDA18280_Object_t pObj, 
        tmTDA18280_DACConfiguration_t* ptConfiguration
        );

    extern tmErrorCode_t
        iTDA18280_BlockI2CGate_SetCfg(
        ptmTDA18280_Object_t pObj,
        tmTDA18280_I2CGate_t*       pI2CGateCfg      /**< I: Configuration of the Gate. */
        );

    extern tmErrorCode_t
        iTDA18280_BlockI2CGate_GetCfg(
        ptmTDA18280_Object_t pObj
        );

    extern tmErrorCode_t 
        iTDA18280_GetTemperatureMeasurement(ptmTDA18280_Object_t pObj, tmTDA18280_TemperatureSource_t eTempSource, Int32 *pulTemp);

    extern tmErrorCode_t 
        iTDA18280_BlockPLL1ProgramTempLoopPrecharge(ptmTDA18280_Object_t pObj);

    extern tmErrorCode_t
        iTDA18280_SetStateDebugRam(ptmTDA18280_Object_t pObj, tmTDA18280_DebugRamState_t eState);

    extern tmErrorCode_t
        iTDA18280_GetStateDebugRam(ptmTDA18280_Object_t pObj, ptmTDA18280_DebugRamState_t peState);

    extern tmErrorCode_t
        iTDA18280_SetConfigAndAcquireDebugRam(ptmTDA18280_Object_t pObj, tmTDA18280_DebugRamCtrlMode_t eMode, tmTDA18280_DebugRamCtrlOneShot_t eOneShot, UInt16 uNbSamples);

    extern tmErrorCode_t 
        iTDA18280_DeviceReadDebugRam(ptmTDA18280_Object_t pObj, tmTDA18280_DebugRamMode_t eMode, UInt16 RAMsize, Int16 * pData);
    
    extern UInt32
        iTDA18280_Log( UInt32 ulValue );

    tmErrorCode_t
        iTDA18280_DeviceAllocInstance(tmUnitSelect_t tUnit, pptmTDA18280_Object_t ppDrvObject);

    tmErrorCode_t
        iTDA18280_DeviceDeAllocInstance(ptmTDA18280_Object_t pDrvObject);

    tmErrorCode_t
        iTDA18280_DeviceGetInstance(tmUnitSelect_t tUnit, pptmTDA18280_Object_t ppDrvObject);

    tmErrorCode_t
        iTDA18280_DeviceResetInstance(ptmTDA18280_Object_t pDrvObject);

    tmErrorCode_t
        iTDA18280_DeviceGetHWVersion(ptmTDA18280_Object_t pObj);

    tmErrorCode_t
        iTDA18280_ConfigureIOs(
        ptmTDA18280_Object_t    pObj
        );

    tmErrorCode_t
        iTDA18280_IrqConfiguration(
        ptmTDA18280_Object_t    pObj
        );

    tmErrorCode_t 
        iTDA18280_BlockDebugRamRead(
        ptmTDA18280_Object_t pObj, UInt16 RAMsize, Int16 *pData
        );

    tmErrorCode_t
        iTDA18280_SetDigAgcMode(
        ptmTDA18280_Object_t pObj,
        UInt8 Idx_ddc,
        tmTDA18280_AgcMode_t eMode
        );

    tmErrorCode_t
        iTDA18280_EnableWbRfAgc(
        ptmTDA18280_Object_t pObj,
        Bool bState
        );

   tmErrorCode_t
        iTDA18280_PathSet(
        ptmTDA18280_Object_t          pObj,
        tmTDA18280_Path_t             tPathNumber,
        const tmTDA18280_PathCfg_t*   ptPathCfg,
        TDA18280_PathSetAGCMode_t     eMode
        );

    tmErrorCode_t
        iTDA18280_PathGet(
        ptmTDA18280_Object_t         pObj,
        tmTDA18280_Path_t            tPathNumber,
        tmTDA18280_PathCfg_t*        ptPathCfg
        );

    extern tmErrorCode_t 
        iTDA18280_CdpIQPreset(
        ptmTDA18280_Object_t             pObj,
        tmTDA18280_Demod_t               uCdpIndex,
        tmTDA18280_ConstellationSource_t eSource
        );

    tmErrorCode_t
        iTDA18280_CdpOobHwInit(
        ptmTDA18280_Object_t              pObj
        );

    tmErrorCode_t
        iTDA18280_CdpHwInit(
        ptmTDA18280_Object_t              pObj,
        UInt32                            uCdpIndex
        );

    tmErrorCode_t
        iTDA18280_CdpConfig(
        ptmTDA18280_Object_t         pObj,
        tmTDA18280_Path_t            tPathNumber,
        const tmTDA18280_PathCfg_t*  ptPathCfg
        );

    tmErrorCode_t
        iTDA18280_OobHwInit(
        ptmTDA18280_Object_t              pObj
        );

    tmErrorCode_t
        iTDA18280_OobConfig(
        ptmTDA18280_Object_t         pObj,
        const tmTDA18280_PathCfg_t*  ptPathCfg
        );

#ifdef _TVFE_IMPLEMENT_MUTEX
    extern tmErrorCode_t iTDA18280_MutexAcquire(ptmTDA18280_Object_t pObj, UInt32 timeOut);
    extern tmErrorCode_t iTDA18280_MutexRelease(ptmTDA18280_Object_t pObj);
#endif

    extern tmErrorCode_t iTDA18280_GetDemodIndex( ptmTDA18280_Object_t pObj, tmTDA18280_Path_t PathIndex, tmTDA18280_Demod_t *pDemodIndex);
    tmErrorCode_t iTDA18280_GetTsIndex( ptmTDA18280_Object_t pObj, tmTDA18280_Path_t PathIndex, tmTDA18280_Ts_t *pTsIndex);
    tmErrorCode_t iTDA18280_GetPathIndexFromDemod( ptmTDA18280_Object_t pObj, tmTDA18280_Demod_t DemodIndex, tmTDA18280_Path_t *pPathIndex);
    tmErrorCode_t iTDA18280_GetPathIndexFromTs( ptmTDA18280_Object_t pObj, tmTDA18280_Ts_t TsIndex, tmTDA18280_Path_t *pPathIndex);

#ifdef __cplusplus
}
#endif

#endif /* _TMBSL_TDA18280_LOCAL_H */

