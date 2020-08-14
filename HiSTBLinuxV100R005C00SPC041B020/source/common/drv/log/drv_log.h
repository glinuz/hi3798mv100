/****************************************************************************
 *             Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: common_log.h
 * Description:
 *    Implement all function for outputting debug information.
 *
 * History:
 * Version   Date         Author     DefectNum    Description
 * 1.1       2006-03-24   l48554     NULL         Create
 ****************************************************************************/
#ifndef __DRV_LOG_EXT_H__
#define __DRV_LOG_EXT_H__
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/major.h>
#include <asm/types.h>
#include <linux/fs.h>
#include <asm/atomic.h>

#include "hi_debug.h"
#include "drv_log_ioctl.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* End of #ifdef __cplusplus */

#define DRV_LOG_DEVICE_NAME "sys/log"

#define MSG_FROM_USER   0
#define MSG_FROM_KERNEL 1


typedef struct hiLOG_BUFFER_INFO_S
{
    HI_U32 u32StartAddrPhy; /*start physic address*/	/*CNcomment:缓冲区起始物理地址*/
    HI_U8 *pu8StartAddrVir; /*start virtual address*/	/*CNcomment:缓冲区起始虚拟地址*/
    HI_U32 u32BufSize;      /*buffer size*/				/*CNcomment:缓冲区大小*/
    HI_U32 u32WriteAddr;    /*write offset*/			/*CNcomment:写地址偏移*/
    HI_U32 u32ReadAddr;     /*read offset*/				/*CNcomment:读地址偏移*/
    HI_U32 u32ResetFlag;    /*reset count*/				/*CNcomment:复位次数*/
    HI_U32 u32WriteCount;   /*write count*/				/*CNcomment:写入次数*/
    wait_queue_head_t wqNoData;    /*no wait queque*/	/*CNcomment:没有数据等待队列*/
}LOG_BUFFER_INFO_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /*__CMPI_LOG_H__*/

