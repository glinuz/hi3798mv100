/******************************************************************************
*              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: gfx2d_os.c
* Description: gfx2d os adapter layer
*
* History:
* Version   Date         Author     DefectNum    Description
* main\1    2013-10-23              NULL         Create this file.
*******************************************************************************/

#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#include "hi_drv_dev.h"
#include "hi_gfx2d_type.h"
#include "hi_gfx_comm_k.h"
#include "gfx2d_ctl.h"
#include "drv_gfx2d_ioctl.h"
#include "gfx2d_config.h"
#include "gfx2d_hal.h"
#include "hi_gfx2d_errcode.h"
#include "hi_gfx_sys_k.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif  /*__cplusplus*/
#endif  /*__cplusplus*/

#define GFX2D_MODULENAME "hi_gfx2d"
#define GFX2D_UINT_MULTIPLY_OVERFLOW(a, b) (UINT_MAX / (a) < (b))

static HI_U32 mem = 0;

module_param(mem, uint, S_IRUGO);

#ifndef GFX2D_PROC_UNSUPPORT
HI_S32 GFX2D_OS_ReadProc(struct seq_file *p, HI_VOID *v)
{
    (HI_VOID)GFX2D_CTL_ReadProc(p, v);

    return HI_SUCCESS;
}

HI_S32 GFX2D_OS_WriteProc(struct file *file, const char __user *buf,
                          size_t count, loff_t *ppos)
{
    (HI_VOID)GFX2D_CONFIG_WriteProc(file, buf, count, ppos);

    (HI_VOID)GFX2D_CTL_WriteProc(file, buf, count, ppos);

    return HI_SUCCESS;
}
#endif

HI_S32 GFX2D_OS_Open(struct inode *finode, struct file *ffile)
{
    return GFX2D_CTL_Open();
}

HI_S32 GFX2D_OS_Release(struct inode *finode, struct file *ffile)
{
    return GFX2D_CTL_Close();
}

long GFX2D_OS_Ioctl(struct file  *ffile, unsigned int  cmd, unsigned long arg)
{
    void __user *argp = (void __user *)arg;

    if (0 == arg)
    {
       return -EFAULT;
    }

    if (GFX2D_CTL_CheckOpen() == HI_FALSE)
    {
        return -EFAULT;
    }

    switch(cmd)
    {
        case GFX2D_COMPOSE:
        {
            HI_S32 s32Ret = HI_SUCCESS;
            GFX2D_COMPOSE_CMD_S stCompose;
            GFX2D_COMPOSE_CMD_HAL_S stHalCompose;
            HI_U32 u32Size;

            HI_GFX_Memset(&stCompose, 0x0, sizeof(GFX2D_COMPOSE_CMD_S));
            HI_GFX_Memset(&stHalCompose, 0x0, sizeof(GFX2D_COMPOSE_CMD_HAL_S));

            if (copy_from_user(&stCompose, argp, sizeof(GFX2D_COMPOSE_CMD_S)))
            {
                return -EFAULT;
            }

            if (stCompose.enDevId >= HI_GFX2D_DEV_ID_BUTT)
            {
                return HI_ERR_GFX2D_INVALID_DEVID;
            }

            if (0 == stCompose.stComposeList.u32ComposeCnt)
            {
        return HI_ERR_GFX2D_INVALID_COMPOSECNT;
        }

            if (GFX2D_UINT_MULTIPLY_OVERFLOW(stCompose.stComposeList.u32ComposeCnt, sizeof(HI_GFX2D_COMPOSE_S)))
            {
                return HI_ERR_GFX2D_INVALID_COMPOSECNT;
            }

        u32Size = sizeof(HI_GFX2D_COMPOSE_S) * stCompose.stComposeList.u32ComposeCnt;

            stHalCompose.stComposeList.pstCompose = HI_GFX_KMALLOC(HIGFX_GFX2D_ID, u32Size, GFP_KERNEL);
            if (stHalCompose.stComposeList.pstCompose == NULL)
            {
                return HI_ERR_GFX2D_NO_MEM;
            }
            stHalCompose.enDevId        = stCompose.enDevId;
            stHalCompose.pstDst         = &(stCompose.stDst);
            stHalCompose.bSync          = stCompose.bSync;
            stHalCompose.u32Timeout     = stCompose.u32Timeout;

            if (copy_from_user(stHalCompose.stComposeList.pstCompose, (HI_VOID *)(unsigned long)stCompose.stComposeList.u64Compose,
                               u32Size))
            {
                HI_GFX_KFREE(HIGFX_GFX2D_ID, stHalCompose.stComposeList.pstCompose);
                return -EFAULT;
            }

            stHalCompose.stComposeList.u32ComposeCnt = stCompose.stComposeList.u32ComposeCnt;
            stHalCompose.stComposeList.u32BgColor    = stCompose.stComposeList.u32BgColor;

            s32Ret = GFX2D_CTL_Compose(stHalCompose.enDevId, &stHalCompose.stComposeList, stHalCompose.pstDst, stHalCompose.bSync, stHalCompose.u32Timeout);

            if (copy_to_user((HI_VOID *)(unsigned long)stCompose.stComposeList.u64Compose, stHalCompose.stComposeList.pstCompose,
                             u32Size))
            {
                HI_GFX_KFREE(HIGFX_GFX2D_ID, stHalCompose.stComposeList.pstCompose);
                return -EFAULT;
            }

            stCompose.stDst.s32ReleaseFenceFd = stHalCompose.pstDst->s32ReleaseFenceFd;

            if (copy_to_user((HI_VOID *)argp, (HI_VOID *)&stCompose, sizeof(GFX2D_COMPOSE_CMD_S)))
            {
                HI_GFX_KFREE(HIGFX_GFX2D_ID, stHalCompose.stComposeList.pstCompose);
                return -EFAULT;
            }

            HI_GFX_KFREE(HIGFX_GFX2D_ID, stHalCompose.stComposeList.pstCompose);

            return s32Ret;

        }
        case GFX2D_WAITALLDONE:
        {
            GFX2D_WAITALLDONE_CMD_S stWaitAllDone;

            if (copy_from_user(&stWaitAllDone, argp, sizeof(GFX2D_WAITALLDONE_CMD_S)))
            {
                return -EFAULT;
            }

            if (stWaitAllDone.enDevId >= HI_GFX2D_DEV_ID_BUTT)
            {
                return HI_ERR_GFX2D_INVALID_DEVID;
            }

            return GFX2D_CTL_WaitAllDone(stWaitAllDone.enDevId, stWaitAllDone.u32Timeout);
        }
        default:
            return -ENOIOCTLCMD;
    }
    return HI_SUCCESS;
}

HI_S32 GFX2D_OS_PmSuspend(PM_BASEDEV_S *pstDev, pm_message_t state)
{
    GFX2D_CTL_WaitAllDone(HI_GFX2D_DEV_ID_0, 0);

    HI_PRINT("GFX2D suspend ok\n");

    return HI_SUCCESS;
}

HI_S32 GFX2D_OS_PmResume(PM_BASEDEV_S *pstDev)
{
    HI_S32 s32Ret;

    s32Ret = GFX2D_CTL_Resume();

    HI_PRINT("GFX2D resume %s\n", (s32Ret == HI_SUCCESS) ? "OK":"NOK");

    return HI_SUCCESS;
}

DECLARE_GFX_NODE(GFX2D_MODULENAME,GFX2D_OS_Open, GFX2D_OS_Release, NULL, GFX2D_OS_Ioctl,\
                 GFX2D_OS_Ioctl,GFX2D_OS_PmSuspend, GFX2D_OS_PmResume);


static HI_VOID HI_GFX_ShowVersionK(HI_VOID)
{
#if !defined(CONFIG_GFX_COMM_VERSION_DISABLE) && !defined(CONFIG_GFX_COMM_DEBUG_DISABLE) && defined(MODULE)
    GFX_Printk("Load hi_gfx2d.ko success.\t(%s)\n", VERSION_STRING);
    return;
#endif
}


HI_S32 GFX2D_OS_ModInit(HI_VOID)
{
    HI_S32 s32Ret;
#ifndef GFX2D_PROC_UNSUPPORT
    GFX_PROC_ITEM_S stProcItem =
    {
        .fnRead = GFX2D_OS_ReadProc,
        .fnWrite = GFX2D_OS_WriteProc,
        .fnIoctl = NULL,
    };
#endif

    s32Ret = HI_GFX_MODULE_Register(HIGFX_GFX2D_ID, "HI_GFX2D", NULL);
    if (HI_SUCCESS != s32Ret)
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "Register module failed!\n");
        return HI_FAILURE;
    }

    s32Ret = GFX2D_CONFIG_SetMemSize(mem);
    if (HI_SUCCESS != s32Ret)
    {
        HI_GFX_MODULE_UnRegister(HIGFX_GFX2D_ID);
        return HI_FAILURE;
    }

    s32Ret = GFX2D_CTL_Init();
    if (HI_SUCCESS != s32Ret)
    {
        HI_GFX_MODULE_UnRegister(HIGFX_GFX2D_ID);
        return HI_FAILURE;
    }

    s32Ret = HI_GFX_PM_Register();
    if (HI_SUCCESS != s32Ret)
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "Register module failed!\n");
        (HI_VOID)GFX2D_CTL_Deinit();
        HI_GFX_MODULE_UnRegister(HIGFX_GFX2D_ID);
        return HI_FAILURE;
    }

#ifndef GFX2D_PROC_UNSUPPORT
    s32Ret = HI_GFX_PROC_AddModule(GFX2D_MODULENAME, &stProcItem, NULL);
    if (HI_SUCCESS != s32Ret)
    {
        HI_GFX_COMM_LOG_WARNING(HIGFX_GFX2D_ID, "Create proc failed!\n");
    }
#endif

    HI_GFX_ShowVersionK();

    return HI_SUCCESS;
}

HI_VOID GFX2D_OS_ModExit(HI_VOID)
{
#ifndef GFX2D_PROC_UNSUPPORT
    HI_GFX_PROC_RemoveModule(GFX2D_MODULENAME);
#endif

    (HI_VOID)HI_GFX_PM_UnRegister();

    (HI_VOID)GFX2D_CTL_Deinit();

    HI_GFX_MODULE_UnRegister(HIGFX_GFX2D_ID);

    return;
}

#ifdef MODULE
module_init(GFX2D_OS_ModInit);
module_exit(GFX2D_OS_ModExit);
#endif

MODULE_AUTHOR("Digital Media Team, Hisilicon crop.");
MODULE_DESCRIPTION("Hisilicon GFX2D Device driver for 3716CV200");
MODULE_LICENSE("GPL");
MODULE_VERSION("V1.0.0.0");

#ifdef __cplusplus
#if __cplusplus
}
#endif  /*__cplusplus*/
#endif  /*__cplusplus*/
