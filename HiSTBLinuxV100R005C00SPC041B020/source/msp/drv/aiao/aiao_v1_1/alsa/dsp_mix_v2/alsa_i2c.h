#ifndef I2C_H_
#define I2C_H_
#include "hi_unf_ecs.h"
#include "hi_module.h"
#include "hi_debug.h"
//HI_ALSA_TLV320_SUPPORT

extern int init_i2c(void);
extern int remove_i2c(void);
extern void godbox_aic31_reset(void);
extern void i2c_pins_init(void);
extern void godbox_aic31_mute(int mute);

#endif //I2C_H_
