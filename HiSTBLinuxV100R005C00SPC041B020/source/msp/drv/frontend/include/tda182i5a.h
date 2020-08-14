
#ifndef __TDA182I5A_H__
#define __TDA182I5A_H__

extern HI_S32 tda182i5a_set_tuner (HI_U32 u32TunerPort, HI_U32 u32PuRF, HI_U32 u32BandWidth);
extern HI_VOID tda182i5a_tuner_resume (HI_U32 u32TunerPort);
extern HI_S32 tda182i5a_init_tuner(HI_U32 u32TunerPort);
extern HI_S32 tda182i5a_get_signal_strength(HI_U32 u32TunerPort, HI_U32 *pu32SignalStrength);
#endif

