#ifndef __SI2147_H__
#define __SI2147_H__

extern HI_S32 si2147_set_tuner(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_U32 u32PuRF);
extern HI_VOID si2147_tuner_resume (HI_U32 u32TunerPort);
extern HI_S32 si2147_init_tuner(HI_U32 u32TunerPort);
extern HI_S32 si2147_get_signal_strength(HI_U32 u32TunerPort, HI_U32 *pu32SignalStrength);
#endif

