/******************************************************************************
 Copyright (C), 2009-2014, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
 File Name     : drv_descrambler.h
 Version       : Initial Draft
 Author        : Hisilicon multimedia software group
 Created       : 2013/04/16
 Description   :
******************************************************************************/

#ifndef __DRV_DESCRAMBLER_H__
#define __DRV_DESCRAMBLER_H__

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef enum
{
    DMX_KEY_TYPE_EVEN   = 0,
    DMX_KEY_TYPE_ODD    = 1
} DMX_KEY_TYPE_E;

typedef enum
{
    DMX_KEY_CW = 0,
    DMX_KEY_IV = 1
} DMX_CW_TYPE;

HI_S32  DMXKeyIoctl(struct file *file, HI_U32 cmd, HI_VOID *arg);
HI_VOID DmxDestroyAllDescrambler(Dmx_Set_S * DmxSet, struct file * file);

#ifdef HI_DEMUX_PROC_SUPPORT
HI_S32  DMXKeyProcRead(struct seq_file *p, HI_VOID *v);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif  /* __DRV_DESCRAMBLER_H__ */

