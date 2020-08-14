/******************************************************************************
 Copyright (C), 2009-2014, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
 File Name     : drv_demux_ext.h
 Version       : Initial Draft
 Author        : Hisilicon multimedia software group
 Created       : 2012/08/24
 Description   :
******************************************************************************/

#ifndef __DRV_DEMUX_EXT_H__
#define __DRV_DEMUX_EXT_H__

#include "hi_type.h"
#include "hi_drv_dev.h"
#include "hi_drv_demux.h"

typedef HI_S32  (*FN_DEMUX_AcquireEs)(HI_HANDLE, DMX_Stream_S*);
typedef HI_S32  (*FN_DEMUX_ReleaseEs)(HI_HANDLE, DMX_Stream_S*);
typedef HI_S32  (*FN_DEMUX_Suspend)(PM_BASEDEV_S *himd, pm_message_t state);
typedef HI_S32  (*FN_DEMUX_Resume)(PM_BASEDEV_S *himd);

typedef struct
{
    FN_DEMUX_AcquireEs      pfnDmxAcquireEs;
    FN_DEMUX_ReleaseEs      pfnDmxReleaseEs;
    FN_DEMUX_Suspend        pfnDmxSuspend;
    FN_DEMUX_Resume         pfnDmxResume;
} DEMUX_EXPORT_FUNC_S;

HI_S32  DMX_DRV_ModInit(HI_VOID);
HI_VOID DMX_DRV_ModExit(HI_VOID);

#endif

