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

#ifndef __DRV_AI_ALSA_FUNC_H__
#define __DRV_AI_ALSA_FUNC_H__

#include <linux/fs.h>

#include "hi_drv_ai.h"

int hi_ai_alsa_open(AI_DRV_Create_Param_S* pstAiParam, struct file* file);
int hi_ai_alsa_update_readptr(int handle, unsigned int* write_pos);
void hi_ai_alsa_query_readpos(int handle, unsigned int* pos);
int hi_ai_alsa_flush_buffer(int handle);
void hi_ai_alsa_query_writepos(int handle, HI_U32* pos);
int hi_ai_alsa_close(int handle, struct file* file);
int hi_ai_alsa_set_enable(int handle, HI_BOOL bEnable);
int hi_ai_alsa_get_attr(HI_UNF_AI_E enAiPort, HI_UNF_AI_ATTR_S* pstAiAttr);
int hi_ai_alsa_get_proc_func(AIAO_IsrFunc** pFunc);

#endif
