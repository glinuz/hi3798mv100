/*******************************************************************************
*             Copyright 2009 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: drv_demux.h
* Description: demux function wrap
*
* History:
* Version   Date              Author    DefectNum       Description
* main\1    2009-12-24        jianglei  NULL            create this file.
*******************************************************************************/
#ifndef __DRV_DEMUX_H__
#define __DRV_DEMUX_H__

#include "hi_drv_mmz.h"
#include "hi_unf_demux.h"
#include "drv_demux_define.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/***************************** Macro Definition ******************************/

/*************************** Structure Definition ****************************/

/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/

#ifdef HI_DEMUX_PROC_SUPPORT
/* control function entry by proc file system*/
typedef enum
{
    DMX_DEBUG_CMD_SAVE_ES       = 1,
    DMX_DEBUG_CMD_SAVE_ALLTS,
    DMX_DEBUG_CMD_SAVE_IPTS,
    DMX_DEBUG_CMD_SAVE_DMXTS
} DMX_DEBUG_CMD;

typedef enum
{
    DMX_DEBUG_CMD_STOP        = 0,
    DMX_DEBUG_CMD_START       = 1,
} DMX_DEBUG_CMD_CTRl;



HI_S32  DMX_OsrDebugCtrl(HI_U32 cmd,DMX_DEBUG_CMD_CTRl cmdctrl,HI_U32 param);

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif  // __DRV_DEMUX_H__

