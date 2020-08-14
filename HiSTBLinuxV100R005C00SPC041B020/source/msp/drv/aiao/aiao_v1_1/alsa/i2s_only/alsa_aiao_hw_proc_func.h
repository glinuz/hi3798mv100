/*******************************************************************************
 *              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: drv_aiao_alsa_func.h
 * Description: drv aiao alsa func h
 *
 * History:
 * Version   Date         Author     DefectNum    Description
 * main\1    
 ********************************************************************************/

#ifndef __DRV_AIAO_I2S_ALSA_PROC_H__
#define __DRV_AIAO_I2SALSA_PROC_H__

#include <sound/core.h>
#include "hi_type.h"
#include "alsa_aiao_hw_comm.h"

int hiaudio_ao_hw_proc_init(void * card, const char * name, struct hii2saudio_data *had);
int hiaudio_ai_hw_proc_init(void * card, const char * name, struct hii2saudio_data *had);


void hiaudio_hw_proc_cleanup(void *p);


#endif
