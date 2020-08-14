/**
 \file
 \brief Configuration of board
 \copyright Shenzhen Hisilicon Co., Ltd.
 \version draft
 \author x57522
 \date 2010-8-9
 */

#ifndef __COMMON_BOARDCFG_H__
#define __COMMON_BOARDCFG_H__

/* add include here */

#ifdef __cplusplus
extern "C" {
#endif

#define OTA_TUNER_ID              (HI_LOADER_TUNER_INDEX)

#if (0 == OTA_TUNER_ID)
#if defined(HI_DEMOD_USE_I2C)
#define LOADER_USE_I2C
#elif defined(HI_DEMOD_USE_GPIO)
#define LOADER_USE_GPIO
#define GET_TUNER_GPIOSCL()             __GET_TUNER0_GOIOSCL()
#define GET_TUNER_GPIOSDA()             __GET_TUNER0_GOIOSDA()
#endif
#define GET_TUNER_CONFIG(stTunerAttr)   __GET_TUNER0_CONFIG(stTunerAttr)
#define GET_TUNER_TSOUT_CONFIG(stTSOut) __GET_TUNER0_TSOUT_CONFIG(stTSOut)
#define GET_DEMUX_PORT()                __GET_DEMUX0_PORT()

#elif (1 == OTA_TUNER_ID)
#if defined(HI_DEMOD1_USE_I2C)
#define LOADER_USE_I2C
#elif defined(HI_DEMOD1_USE_GPIO)
#define LOADER_USE_GPIO
#define GET_TUNER_GPIOSCL()             __GET_TUNER_GPIOSCL(1)
#define GET_TUNER_GPIOSDA()             __GET_TUNER_GPIOSDA(1)

#endif
#define GET_TUNER_CONFIG(stTunerAttr)   __GET_TUNER_CONFIG(1,stTunerAttr)
#define GET_TUNER_TSOUT_CONFIG(stTSOut) __GET_TUNER_TSOUT_CONFIG(1,stTSOut)
#define GET_DEMUX_PORT()                __GET_DEMUX_PORT(1)


#elif (2 == OTA_TUNER_ID)
#if defined(HI_DEMOD2_USE_I2C)
#define LOADER_USE_I2C
#elif defined(HI_DEMOD2_USE_GPIO)
#define LOADER_USE_GPIO
#define GET_TUNER_GPIOSCL()             __GET_TUNER_GPIOSCL(2)
#define GET_TUNER_GPIOSDA()             __GET_TUNER_GPIOSDA(2)
#endif
#define GET_TUNER_CONFIG(stTunerAttr)   __GET_TUNER_CONFIG(2,stTunerAttr)
#define GET_TUNER_TSOUT_CONFIG(stTSOut) __GET_TUNER_TSOUT_CONFIG(2,stTSOut)
#define GET_DEMUX_PORT()                __GET_DEMUX_PORT(2)

#elif (3 == OTA_TUNER_ID)
#if defined(HI_DEMOD3_USE_I2C)
#define LOADER_USE_I2C
#elif defined(HI_DEMOD3_USE_GPIO)
#define LOADER_USE_GPIO
#define GET_TUNER_GPIOSCL()             __GET_TUNER_GPIOSCL(3)
#define GET_TUNER_GPIOSDA()             __GET_TUNER_GPIOSDA(3)
#endif
#define GET_TUNER_CONFIG(stTunerAttr)   __GET_TUNER_CONFIG(3,stTunerAttr)
#define GET_TUNER_TSOUT_CONFIG(stTSOut) __GET_TUNER_TSOUT_CONFIG(3,stTSOut)
#define GET_DEMUX_PORT()                __GET_DEMUX_PORT(3)
#else
#error "unexception cable tuner port config!"
#endif

/*single tuner use this brunch*/
#define __GET_TUNER0_CONFIG(stTunerAttr) \
    {\
        stTunerAttr.enSigType      = (HI_UNF_TUNER_SIG_TYPE_E)HI_TUNER_SIGNAL_TYPE; \
        stTunerAttr.enTunerDevType = (HI_UNF_TUNER_DEV_TYPE_E)HI_TUNER_TYPE; \
        stTunerAttr.u32TunerAddr   = HI_TUNER_DEV_ADDR; \
        stTunerAttr.enDemodDevType = (HI_UNF_DEMOD_DEV_TYPE_E)HI_DEMOD_TYPE; \
        stTunerAttr.u32DemodAddr   = HI_DEMOD_DEV_ADDR; \
        stTunerAttr.enOutputMode   = (HI_UNF_TUNER_OUPUT_MODE_E)HI_DEMOD_TS_MODE; \
        stTunerAttr.enI2cChannel   = HI_DEMOD_I2C_CHANNEL; \
        stTunerAttr.u32ResetGpioNo = HI_DEMOD_RESET_GPIO; \
    }

/*multi tuner ports use this brunch*/
#define __GET_TUNER_CONFIG(u32TunerId, stTunerAttr) \
    { \
        stTunerAttr.enSigType      = (HI_UNF_TUNER_SIG_TYPE_E)HI_TUNER##u32TunerId##_SIGNAL_TYPE; \
        stTunerAttr.enTunerDevType = (HI_UNF_TUNER_DEV_TYPE_E)HI_TUNER##u32TunerId##_TYPE; \
        stTunerAttr.u32TunerAddr   = HI_TUNER##u32TunerId##_DEV_ADDR; \
        stTunerAttr.enDemodDevType = (HI_UNF_DEMOD_DEV_TYPE_E)HI_DEMOD##u32TunerId##_TYPE; \
        stTunerAttr.u32DemodAddr   = HI_DEMOD##u32TunerId##_DEV_ADDR; \
        stTunerAttr.enOutputMode   = (HI_UNF_TUNER_OUPUT_MODE_E)HI_DEMOD##u32TunerId##_TS_MODE; \
        stTunerAttr.enI2cChannel   = HI_DEMOD##u32TunerId##_I2C_CHANNEL; \
        stTunerAttr.u32ResetGpioNo = HI_DEMOD##u32TunerId##_RESET_GPIO; \
    }

#if defined(LOADER_USE_GPIO)
/*single tuner use this brunch*/
#define __GET_TUNER0_GOIOSCL() HI_DEMOD_GPIO_SCL
#define __GET_TUNER0_GOIOSDA() HI_DEMOD_GPIO_SDA

/*multi tuner ports use this brunch*/
#define __GET_TUNER_GPIOSCL(u32TunerId) HI_DEMOD##u32TunerId##_GPIO_SCL
#define __GET_TUNER_GPIOSDA(u32TunerId) HI_DEMOD##u32TunerId##_GPIO_SDA
#endif

#define __GET_DEMUX0_PORT()  HI_DEMUX_PORT
#define __GET_DEMUX_PORT(u32TunerId)  HI_DEMUX##u32TunerId##_PORT

/*single tuner use this brunch*/
#define __GET_TUNER0_TSOUT_CONFIG(stTSOut) \
    { \
        stTSOut.enTSOutput[0] = HI_TUNER_OUTPUT_PIN0; \
        stTSOut.enTSOutput[1] = HI_TUNER_OUTPUT_PIN1; \
        stTSOut.enTSOutput[2] = HI_TUNER_OUTPUT_PIN2; \
        stTSOut.enTSOutput[3] = HI_TUNER_OUTPUT_PIN3; \
        stTSOut.enTSOutput[4] = HI_TUNER_OUTPUT_PIN4; \
        stTSOut.enTSOutput[5] = HI_TUNER_OUTPUT_PIN5; \
        stTSOut.enTSOutput[6] = HI_TUNER_OUTPUT_PIN6; \
        stTSOut.enTSOutput[7] = HI_TUNER_OUTPUT_PIN7; \
        stTSOut.enTSOutput[8] = HI_TUNER_OUTPUT_PIN8; \
        stTSOut.enTSOutput[9] = HI_TUNER_OUTPUT_PIN9; \
        stTSOut.enTSOutput[10] = HI_TUNER_OUTPUT_PIN10; \
    }

/*multi tuner ports use this brunch*/
#define __GET_TUNER_TSOUT_CONFIG(u32TunerId,stTSOut) \
    { \
        stTSOut.enTSOutput[0] = HI_TUNER##u32TunerId##_OUTPUT_PIN0; \
        stTSOut.enTSOutput[1] = HI_TUNER##u32TunerId##_OUTPUT_PIN1; \
        stTSOut.enTSOutput[2] = HI_TUNER##u32TunerId##_OUTPUT_PIN2; \
        stTSOut.enTSOutput[3] = HI_TUNER##u32TunerId##_OUTPUT_PIN3; \
        stTSOut.enTSOutput[4] = HI_TUNER##u32TunerId##_OUTPUT_PIN4; \
        stTSOut.enTSOutput[5] = HI_TUNER##u32TunerId##_OUTPUT_PIN5; \
        stTSOut.enTSOutput[6] = HI_TUNER##u32TunerId##_OUTPUT_PIN6; \
        stTSOut.enTSOutput[7] = HI_TUNER##u32TunerId##_OUTPUT_PIN7; \
        stTSOut.enTSOutput[8] = HI_TUNER##u32TunerId##_OUTPUT_PIN8; \
        stTSOut.enTSOutput[9] = HI_TUNER##u32TunerId##_OUTPUT_PIN9; \
        stTSOut.enTSOutput[10] = HI_TUNER##u32TunerId##_OUTPUT_PIN10; \
    }

#ifdef HI_LOADER_TUNER_SAT
#define __GET_SAT_TUNER0_CONFIG(stSatTunerAttr) \
    { \
        stSatTunerAttr.u32DemodClk       = HI_DEMOD_REF_CLOCK; \
        stSatTunerAttr.u16TunerMaxLPF    = HI_TUNER_MAX_LPF; \
        stSatTunerAttr.u16TunerI2CClk    = HI_TUNER_I2C_CLOCK; \
        stSatTunerAttr.enRFAGC           = HI_TUNER_RFAGC; \
        stSatTunerAttr.enIQSpectrum      = HI_TUNER_IQSPECTRUM; \
        stSatTunerAttr.enTSClkPolar      = HI_TUNER_TSCLK_POLAR; \
        stSatTunerAttr.enTSFormat        = HI_TUNER_TS_FORMAT; \
        stSatTunerAttr.enTSSerialPIN     = HI_TUNER_TS_SERIAL_PIN; \
        stSatTunerAttr.enDiSEqCWave      = HI_TUNER_DISEQCWAVE; \
        stSatTunerAttr.enLNBCtrlDev      = HI_LNBCTRL_DEV_TYPE; \
        stSatTunerAttr.u16LNBDevAddress  = HI_LNBCTRL_DEV_ADDR; \
    }

#define __GET_SAT_TUNER_CONFIG(u32TunerId,stSatTunerAttr) \
    { \
        stSatTunerAttr.u32DemodClk       = HI_DEMOD##u32TunerId##_REF_CLOCK; \
        stSatTunerAttr.u16TunerMaxLPF    = HI_TUNER##u32TunerId##_MAX_LPF; \
        stSatTunerAttr.u16TunerI2CClk    = HI_TUNER##u32TunerId##_I2C_CLOCK; \
        stSatTunerAttr.enRFAGC           = HI_TUNER##u32TunerId##_RFAGC; \
        stSatTunerAttr.enIQSpectrum      = HI_TUNER##u32TunerId##_IQSPECTRUM; \
        stSatTunerAttr.enTSClkPolar      = HI_TUNER##u32TunerId##_TSCLK_POLAR; \
        stSatTunerAttr.enTSFormat        = HI_TUNER##u32TunerId##_TS_FORMAT; \
        stSatTunerAttr.enTSSerialPIN     = HI_TUNER##u32TunerId##_TS_SERIAL_PIN; \
        stSatTunerAttr.enDiSEqCWave      = HI_TUNER##u32TunerId##_DISEQCWAVE; \
        stSatTunerAttr.enLNBCtrlDev      = HI_LNBCTRL##u32TunerId##_DEV_TYPE; \
        stSatTunerAttr.u16LNBDevAddress  = HI_LNBCTRL##u32TunerId##_DEV_ADDR; \
    }

#if (0 == OTA_TUNER_ID)
#define GET_SAT_TUNER_CONFIG(stSatTunerAttr) __GET_SAT_TUNER0_CONFIG(stSatTunerAttr)
#elif (1 == OTA_TUNER_ID)
#define GET_SAT_TUNER_CONFIG(stSatTunerAttr) __GET_SAT_TUNER_CONFIG(1,stSatTunerAttr)
#elif (2 == OTA_TUNER_ID)
#define GET_SAT_TUNER_CONFIG(stSatTunerAttr) __GET_SAT_TUNER_CONFIG(2,stSatTunerAttr)
#elif (3 ==  OTA_TUNER_ID)
#define GET_SAT_TUNER_CONFIG(stSatTunerAttr) __GET_SAT_TUNER_CONFIG(3,stSatTunerAttr)
#else
#error "unexception sat tuner port config!"
#endif

#endif

#if defined(HI_LOADER_TUNER_DVB_T) || defined(HI_LOADER_TUNER_DVB_T2)
#define __GET_TER_TUNER0_CONFIG(stTerTunerAttr) \
    { \
        stTerTunerAttr.u32DemodClk       = HI_TER_DEMOD_REF_CLOCK; \
        stTerTunerAttr.u32ResetGpioNo    = HI_DEMOD_RESET_GPIO; \
        stTerTunerAttr.u16TunerMaxLPF    = HI_TER_TUNER_MAX_LPF; \
        stTerTunerAttr.u16TunerI2CClk    = HI_TER_TUNER_I2C_CLOCK; \
        stTerTunerAttr.enRFAGC           = HI_TER_TUNER_RFAGC; \
        stTerTunerAttr.enIQSpectrum      = HI_TER_TUNER_IQSPECTRUM; \
        stTerTunerAttr.enTSClkPolar      = HI_TER_TUNER_TSCLK_POLAR; \
        stTerTunerAttr.enTSFormat        = HI_TER_TUNER_TS_FORMAT; \
        stTerTunerAttr.enTSSerialPIN     = HI_TER_TUNER_TS_SERIAL_PIN; \
        stTerTunerAttr.enTSSyncHead      = HI_TER_TUNER_TS_SYNC_HEAD; \
    }

#define __GET_TER_TUNER_CONFIG(u32TunerId,stTerTunerAttr) \
    { \
        stTerTunerAttr.u32DemodClk       = HI_TER_DEMOD##u32TunerId##_REF_CLOCK; \
        stTerTunerAttr.u32ResetGpioNo    = HI_DEMOD##u32TunerId##_RESET_GPIO; \
        stTerTunerAttr.u16TunerMaxLPF    = HI_TER_TUNER##u32TunerId##_MAX_LPF; \
        stTerTunerAttr.u16TunerI2CClk    = HI_TER_TUNER##u32TunerId##_I2C_CLOCK; \
        stTerTunerAttr.enRFAGC           = HI_TER_TUNER##u32TunerId##_RFAGC; \
        stTerTunerAttr.enIQSpectrum      = HI_TER_TUNER##u32TunerId##_IQSPECTRUM; \
        stTerTunerAttr.enTSClkPolar      = HI_TER_TUNER##u32TunerId##_TSCLK_POLAR; \
        stTerTunerAttr.enTSFormat        = HI_TER_TUNER##u32TunerId##_TS_FORMAT; \
        stTerTunerAttr.enTSSerialPIN     = HI_TER_TUNER##u32TunerId##_TS_SERIAL_PIN; \
        stTerTunerAttr.enTSSyncHead      = HI_TER_TUNER##u32TunerId##_TS_SYNC_HEAD; \
    }

#if (0 == OTA_TUNER_ID)
#define GET_TER_TUNER_CONFIG(stTerTunerAttr) __GET_TER_TUNER0_CONFIG(stTerTunerAttr)
#elif (1 == OTA_TUNER_ID)
#define GET_TER_TUNER_CONFIG(stTerTunerAttr) __GET_TER_TUNER_CONFIG(1,stTerTunerAttr)
#elif (2 == OTA_TUNER_ID)
#define GET_TER_TUNER_CONFIG(stTerTunerAttr) __GET_TER_TUNER_CONFIG(2,stTerTunerAttr)
#elif (3 ==  OTA_TUNER_ID)
#define GET_TER_TUNER_CONFIG(stTerTunerAttr) __GET_TER_TUNER_CONFIG(3,stTerTunerAttr)
#else
#error "unexception ter tuner port config!"
#endif

#endif

#ifdef __cplusplus
}
#endif
#endif /* __COMMON_BOARDCFG_H__ */
