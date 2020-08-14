#ifndef __MT_2081_H__
#define __MT_2081_H__


extern HI_S32 mt2081_set_tuner(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_U32 u32PuRF);
extern HI_VOID mt2081_tuner_resume(HI_U32 u32TunerPort);
extern HI_S32 mt2081_get_signal_strength(HI_U32 u32TunerPort, HI_U32 *pu32SignalStrength);
#endif

