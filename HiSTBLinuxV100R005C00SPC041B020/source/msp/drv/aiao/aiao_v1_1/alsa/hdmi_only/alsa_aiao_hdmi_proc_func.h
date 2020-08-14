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

#ifndef __DRV_AIAO_HDMI_ALSA_PROC_H__
#define __DRV_AIAO_HDMI_ALSA_PROC_H__

#include <sound/core.h>
#include "hi_type.h"
#include "alsa_aiao_hdmi_comm.h"

int hiaudio_ao_hdmi_proc_init(void * card, const char * name, struct hihdmiaudio_data *had);


void hiaudio_hdmi_proc_cleanup(void *p);


#endif
