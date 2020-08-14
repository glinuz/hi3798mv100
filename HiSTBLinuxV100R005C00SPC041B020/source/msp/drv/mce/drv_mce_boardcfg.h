#ifndef __DRV_MCE_BOARDCFG_H__
#define __DRV_MCE_BOARDCFG_H__

#include "hi_type.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TUNER_USE    HI_TUNER0_ID

#if (HI_TUNER_SIGNAL_TYPE == 2)
#define GET_SAT_TUNER_CONFIG(stSatTunerAttr) \
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
#endif

#if    (HI_TUNER_SIGNAL_TYPE == 4) \
    || (HI_TUNER_SIGNAL_TYPE == 8) \
    || (HI_TUNER_SIGNAL_TYPE == 16)
#define GET_TER_TUNER_CONFIG(stTerTunerAttr) \
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
#endif

#if    (HI_TUNER_SIGNAL_TYPE == 1) \
    || (HI_TUNER_SIGNAL_TYPE == 128)
#define GET_CAB_TUNER_CONFIG(stCabTunerAttr) \
{ \
    stCabTunerAttr.u32DemodClk       = HI_CAB_DEMOD_REF_CLOCK; \
    stCabTunerAttr.enTSSerialPIN     = HI_CAB_TUNER_TS_SERIAL_PIN; \
}
#endif

#ifdef HI_TUNER_OUTPUT_PIN0
#define GET_TUNER_TSOUT_CONFIG(stTSOut) \
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
#endif

#define DEFAULT_DVB_PORT (HI_DEMUX_PORT)

#if (HI_TUNER_SIGNAL_TYPE == 1)
#define GET_CONNECT_PARA(stConnectPara) \
{ \
    stConnectPara.enSigType = HI_UNF_TUNER_SIG_TYPE_CAB; \
    stConnectPara.unConnectPara.stCab.bReverse = 0; \
    stConnectPara.unConnectPara.stCab.u32Freq = 610000; \
    stConnectPara.unConnectPara.stCab.u32SymbolRate = 6875000; \
    stConnectPara.unConnectPara.stCab.enModType = HI_UNF_MOD_TYPE_QAM_64; \
}
#elif (HI_TUNER_SIGNAL_TYPE == 2)
#define GET_CONNECT_PARA(stConnectPara) \
{ \
    stConnectPara.enSigType = HI_UNF_TUNER_SIG_TYPE_SAT; \
    stConnectPara.unConnectPara.stSat.u32Freq = 3840000; \
    stConnectPara.unConnectPara.stSat.u32SymbolRate = 27500000; \
    stConnectPara.unConnectPara.stSat.enPolar = HI_UNF_TUNER_FE_POLARIZATION_H; \
}
#elif (HI_TUNER_SIGNAL_TYPE == 4)
#define GET_CONNECT_PARA(stConnectPara) \
{ \
    stConnectPara.enSigType = HI_UNF_TUNER_SIG_TYPE_DVB_T; \
    stConnectPara.unConnectPara.stTer.bReverse = 0; \
    stConnectPara.unConnectPara.stTer.u32Freq = 682000; \
    stConnectPara.unConnectPara.stTer.u32BandWidth = 8000; \
}
#endif


#ifdef __cplusplus
}
#endif

#endif

