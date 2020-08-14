#ifndef __FV_THREADS_H__
#define __FV_THREADS_H__

#include "fv_config.h"

void fv_eth_detect(void);
void fv_storage_detect(int usb_nr, int sd_nr);
void fv_tuner_detect(int howmany, struct fv_config_tuner *para);

void *ir_thread(void *arg);
void *key_thread(void *arg);
void *led_thread(void *arg);
#ifdef HI_BUILD_WITH_KEYLED
int create_keyled_threads();
void destory_keyled_threads();
#else
static inline int create_keyled_threads()
{
    return 0;
}
static inline void destory_keyled_threads()
{
    return ;
}
#endif


void stop_led_thread(void);
void stop_ir_thread(void);
void stop_key_thread(void);

void fv_detect_ddr(void);
void fv_detect_flash(void);

void fv_show_versions(void);
void fv_show_name(void);
void fv_show_factory_reset(void);

void isTsFileExist(int exist);
void getTsFileState(int *exist);
int fv_tsplay_enter(char tsfile[]);
int fv_tsplay_getVolume(unsigned int *u32Volume);
int fv_tsplay_setVolume(unsigned int u32Volume);
int fv_tsplay_exit();

#endif

