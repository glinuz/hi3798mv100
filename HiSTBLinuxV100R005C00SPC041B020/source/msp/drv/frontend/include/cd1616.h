#ifndef __CD1616_H__
#define __CD1616_H__

#include "hi_type.h"

extern HI_S32 cd1616_set_tuner(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_U32 u32PuRF);

extern HI_S32 cd1616_double_set_tuner(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_U32 u32PuRF);

#endif
