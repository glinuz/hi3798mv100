
#ifndef __TDA18250_H__
#define __TDA18250_H__

extern HI_S32 tda18250_set_tuner(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_U32 u32PuRF);
extern HI_VOID tda18250_tuner_resume (HI_U32 u32TunerPort);
extern HI_S32 tda18250_init_tuner(HI_U32 u32TunerPort);
extern HI_S32 tda18250_get_signal_strength(HI_U32 u32TunerPort, HI_U32 *pu32SignalStrength);
#endif

