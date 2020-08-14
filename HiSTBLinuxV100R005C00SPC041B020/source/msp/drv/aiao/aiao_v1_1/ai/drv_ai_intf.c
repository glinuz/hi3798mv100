/*******************************************************************************
 *              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: drv_ai_intf.c
 * Description: aiao interface of module.
 *
 * History:
 * Version   Date         Author     DefectNum    Description
 * main\1
 ********************************************************************************/

#include <asm/setup.h>
#include <linux/interrupt.h>

#include "hi_type.h"
#include "hi_drv_struct.h"
#include "hi_drv_dev.h"
#include "hi_drv_proc.h"
#include "hi_drv_stat.h"

#include "hi_module.h"
#include "hi_drv_mmz.h"
#include "hi_drv_sys.h"
#include "hi_drv_module.h"
#include "hi_drv_mem.h"
#include "hi_error_mpi.h"

#include "hi_drv_ai.h"
#include "drv_ai_private.h"

/**************************** global variables ****************************/
static UMAP_DEVICE_S g_stAIDev;
static atomic_t g_AIModInitFlag = ATOMIC_INIT(0);

static struct file_operations AI_DRV_Fops =
{
    .owner            = THIS_MODULE,
    .open             = AI_DRV_Open,
    .unlocked_ioctl   = AI_DRV_Ioctl,
    .release          = AI_DRV_Release,
#ifdef CONFIG_COMPAT
    .compat_ioctl     = AI_DRV_Ioctl,
#endif
};

static PM_BASEOPS_S ai_drvops =
{
    .probe        = NULL,
    .remove       = NULL,
    .shutdown     = NULL,
    .prepare      = NULL,
    .complete     = NULL,
    .suspend      = AI_DRV_Suspend,
    .suspend_late = NULL,
    .resume_early = NULL,
    .resume       = AI_DRV_Resume,
};

#if (1 == HI_PROC_SUPPORT)
static AI_REGISTER_PARAM_S s_stProcParam =
{
    .pfnReadProc  = AI_DRV_ReadProc,
    .pfnWriteProc = AI_DRV_WriteProc,
};
#endif

static HI_S32  AIRegisterDevice(HI_VOID)
{
    snprintf(g_stAIDev.devfs_name, sizeof(g_stAIDev.devfs_name), UMAP_DEVNAME_AI);
    g_stAIDev.fops  = &AI_DRV_Fops;
    g_stAIDev.minor = UMAP_MIN_MINOR_AI;
    g_stAIDev.owner  = THIS_MODULE;
    g_stAIDev.drvops = &ai_drvops;

    if (HI_DRV_DEV_Register(&g_stAIDev) < 0)
    {
        HI_FATAL_AI("Unable to register ai dev\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 AIUnregisterDevice(HI_VOID)
{
    HI_DRV_DEV_UnRegister(&g_stAIDev);
    return HI_SUCCESS;
}

HI_S32 AI_DRV_ModInit(HI_VOID)
{
    HI_S32 Ret;

    Ret = AI_DRV_Init();
    if (HI_SUCCESS != Ret)
    {
        HI_FATAL_AI("Init ai drv fail!\n");
        return HI_FAILURE;
    }

#if (1 == HI_PROC_SUPPORT)
    Ret = AI_DRV_RegisterProc(&s_stProcParam);
    if (HI_SUCCESS != Ret)
    {
        HI_FATAL_AI("Reg proc fail!\n");
        return HI_FAILURE;
    }
#endif

    Ret = AIRegisterDevice();
    if (HI_SUCCESS != Ret)
    {
        HI_FATAL_AI("Unable to register ai dev\n");
        return HI_FAILURE;
    }

    atomic_inc(&g_AIModInitFlag);

    return HI_SUCCESS;
}

HI_VOID AI_DRV_ModExit(HI_VOID)
{
    AIUnregisterDevice();

    AI_DRV_Exit();

#if (1 == HI_PROC_SUPPORT)
    AI_DRV_UnregisterProc();
#endif
    atomic_dec(&g_AIModInitFlag);
}
