
#ifndef __R836_H__
#define __R836_H__

extern HI_S32 r836_init_tuner(HI_U32 u32TunerPort);
extern HI_S32 r836_set_tuner(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_U32 puRF);
extern HI_VOID r836_tuner_resume (HI_U32 u32TunerPort);
extern HI_VOID r836_get_signal_strength(HI_U32 u32TunerPort, HI_U32 *pu32SignalStrength);
#endif

