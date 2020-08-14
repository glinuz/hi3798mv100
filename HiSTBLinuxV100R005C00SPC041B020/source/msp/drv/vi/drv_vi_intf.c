/***********************************************************************************
*              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName   :  drv_vi_intf.c
* Description:
*
***********************************************************************************/

#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <asm/io.h>
#include <asm/uaccess.h>

#include "hi_drv_module.h"
#include "drv_vi.h"
#include "drv_vi_ext.h"
#include "hi_drv_vi.h"
#include "hi_error_mpi.h"
#include "hi_kernel_adapt.h"

HI_DECLARE_MUTEX(g_ViMutex);

static UMAP_DEVICE_S g_ViRegisterData;
static PM_BASEOPS_S g_ViDrvOps =
{
    .probe        = NULL,
    .remove       = NULL,
    .shutdown     = NULL,
    .prepare      = NULL,
    .complete     = NULL,
    .suspend      = VI_DRV_Suspend,
    .suspend_late = NULL,
    .resume_early = NULL,
    .resume       = VI_DRV_Resume,
};
static struct file_operations g_ViFileOps =
{
    .owner			= THIS_MODULE,
    .open			= VI_DRV_Open,
    .unlocked_ioctl = VI_DRV_Ioctl,
#ifdef CONFIG_COMPAT
    .compat_ioctl   = VI_DRV_Ioctl,
#endif
    .release		= VI_DRV_Close,
};
static VI_EXPORT_FUNC_S g_ViExtFuncs =
{
	.pfnVOAcqFrame      = (HI_VOID *)HI_VI_Chan_VOAcqFrame,
    .pfnVORlsFrame      = (HI_VOID *)HI_VI_Chan_VORlsFrame,
    .pfnVOSendWinInfo   = (HI_VOID *)HI_VI_Chan_VOChangeWinInfo,
    .pfnVencRlsFrame    = (HI_VOID *)HI_VI_Chan_VORlsFrame,
    .pfnDispSetCastAttr = (HI_VOID *)VI_DRV_ChangeVencInfo,

};
HI_S32 HI_VI_Chan_VOAcqFrame(HI_HANDLE hPort, HI_DRV_VIDEO_FRAME_S* pstVpssFrame)
{
    if ((HI_INVALID_HANDLE == hPort) || (HI_NULL == pstVpssFrame))
    {
        HI_ERR_VI("HI_VI_Chan_VOAcqFrame Bad param!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;

}

HI_S32 HI_VI_Chan_VORlsFrame(HI_HANDLE hPort, HI_DRV_VIDEO_FRAME_S* pstVpssFrame)
{
    HI_S32 s32Ret;
    if ((HI_INVALID_HANDLE == hPort) || (HI_NULL == pstVpssFrame))
    {
        HI_ERR_VI("HI_VI_Chan_VORlsFrame Bad param!\n");
        return HI_FAILURE;
    }
    s32Ret = VI_DRV_ReleaseImage(hPort,pstVpssFrame);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VI("call VI_DRV_ReleaseImage err!\n");
    }
    return s32Ret;
}


HI_S32 HI_VI_Chan_VOChangeWinInfo(HI_HANDLE hPort, HI_DRV_WIN_PRIV_INFO_S* pstWinInfo)
{
    HI_S32 s32Ret;
    if ((HI_INVALID_HANDLE == hPort) || (HI_NULL == pstWinInfo))
    {
        HI_ERR_VI("HI_VDEC_Chan_VOChangeWinInfo Bad param!\n");
        return HI_FAILURE;
    }
   //HI_ERR_VI("call VDEC_Chan_VOChangeWinInfo %x,%p\n",hPort,pstWinInfo);
    s32Ret = VI_DRV_ChangeWinInfo(hPort,pstWinInfo);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VI("call VDEC_Chan_VOChangeWinInfo err!\n");
    }
    return s32Ret;
}

HI_S32 HI_VI_Chan_DispSetCastAttr(HI_HANDLE hPort, HI_U32 u32Width, HI_U32 u32Height, HI_U32 u32FrmRate)

{
    HI_S32 s32Ret;
    if (HI_INVALID_HANDLE == hPort)
    {
        HI_ERR_VI("HI_VI_Chan_DispSetCastAttr Bad param!\n");
        return HI_FAILURE;
    }
    HI_ERR_VI("call HI_VI_Chan_DispSetCastAttr %x,\n",hPort);
    
    s32Ret = VI_DRV_ChangeVencInfo(hPort, u32Width, u32Height, u32FrmRate);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VI("call VI_DRV_ChangeVencInfo err!\n");
    }
    return s32Ret;
}

HI_S32 VI_DRV_Open(struct inode *finode, struct file *ffile)
{
    return HI_SUCCESS;
}

HI_S32 VI_DRV_Close(struct inode *finode, struct file *ffile)
{
    HI_S32 Ret;

    Ret = down_interruptible(&g_ViMutex);
    if (Ret)
    {
        HI_ERR_VI("%s: down_interruptible err!\n", __func__);
        return Ret;
    }

    VI_DRV_DeInitParam(ffile);

    up(&g_ViMutex);

    return HI_SUCCESS;
}

HI_S32 VI_DRV_Ioctl_k(struct inode *inode, struct file *file, unsigned int cmd, HI_VOID *arg)
{
    HI_S32 Ret;

    Ret = down_interruptible(&g_ViMutex);
    if (Ret)
    {
        HI_ERR_VI("%s: down_interruptible err !\n", __func__);
        return Ret;
    }

    switch (cmd)
    {
    case CMD_VI_CREATE:
    {
        VI_CREATE_S *pstCreate;
        pstCreate = (VI_CREATE_S *)arg;
        Ret = HI_DRV_VI_Create(pstCreate, file);
        break;
    }
    case CMD_VI_DESTROY:
    {
        HI_HANDLE *hVi;
        hVi = (HI_HANDLE *)arg;
        Ret = HI_DRV_VI_Destroy(*hVi);
        break;
    }
    case CMD_VI_SET_ATTR:
    {
        VI_ATTR_S *pstAttr;
        pstAttr = (VI_ATTR_S*)arg;
        Ret = HI_DRV_VI_SetAttr(pstAttr->hVi, &pstAttr->stAttr);
        break;
    }
    case CMD_VI_SET_BUF:
    {
        VI_BUF_ATTR_S *pstAttr;
        pstAttr = (VI_BUF_ATTR_S*)arg;
        Ret = HI_DRV_VI_SetExtBuf(pstAttr->hVi, &pstAttr->stBufAttr);
        break;
    }
    case CMD_VI_CREATE_VPSS_PORT:
    {
        VI_VPSS_PORT_S *pstVpssPort;
        pstVpssPort = (VI_VPSS_PORT_S *)arg;
        Ret = HI_DRV_VI_CreateVpssPort(pstVpssPort->hVi, pstVpssPort);
        break;
    }
    case CMD_VI_DESTROY_VPSS_PORT:
    {
        VI_VPSS_PORT_S *pstVpssPort;
        pstVpssPort = (VI_VPSS_PORT_S *)arg;
        Ret = HI_DRV_VI_DestroyVpssPort(pstVpssPort->hVi, pstVpssPort);
        break;
    }
    case CMD_VI_START:
    {
        HI_HANDLE *hVi;
        hVi = (HI_HANDLE *)arg;
        Ret = HI_DRV_VI_Start(*hVi);
        break;
    }
    case CMD_VI_STOP:
    {
        HI_HANDLE *hVi;
        hVi = (HI_HANDLE *)arg;
        Ret = HI_DRV_VI_Stop(*hVi);
        break;
    }
    case CMD_VI_Q_FRAME:
    {
        VI_FRAME_S *pViFrame;
        pViFrame = (VI_FRAME_S *)arg;
        Ret = HI_DRV_VI_QueueFrame(pViFrame->hVi, &(pViFrame->stViFrame));
        break;
    }
    case CMD_VI_DQ_FRAME:
    {
        VI_FRAME_S *pViFrame;
        pViFrame = (VI_FRAME_S *)arg;
        Ret = HI_DRV_VI_DequeueFrame(pViFrame->hVi, &(pViFrame->stViFrame));
        break;
    }
    case CMD_VI_ACQUIRE_FRAME:
    {
        VI_FRAME_S *pViFrame;
        pViFrame = (VI_FRAME_S *)arg;
        Ret = HI_DRV_VI_UsrAcquireFrame(pViFrame->hVi, &(pViFrame->stViFrame), pViFrame->u32TimeoutMs);
        break;
    }
    case CMD_VI_RELEASE_FRAME:
    {
        VI_FRAME_S *pViFrame;
        pViFrame = (VI_FRAME_S *)arg;
        Ret = HI_DRV_VI_UsrReleaseFrame(pViFrame->hVi, &(pViFrame->stViFrame));
        break;
    }
    default:
    {
        HI_ERR_VI("vi cmd value err, cmd=%x\n", cmd);
        Ret = HI_ERR_VI_NOT_SUPPORT;
        break;
    }
    }

    up(&g_ViMutex);
    return Ret;
}

long VI_DRV_Ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    long Ret;

    Ret = (long)HI_DRV_UserCopy(file->f_dentry->d_inode, file, cmd, arg, VI_DRV_Ioctl_k);
    return Ret;
}

HI_S32 VI_DRV_Suspend(PM_BASEDEV_S *pdev, pm_message_t state)
{
    HI_S32 Ret;

    Ret = down_interruptible(&g_ViMutex);
    if (Ret)
    {
        HI_ERR_VI("%s: down_interruptible err !\n", __func__);
        return Ret;
    }

    HI_DRV_VI_Suspend();
    up(&g_ViMutex);

    return HI_SUCCESS;
}

HI_S32 VI_DRV_Resume(PM_BASEDEV_S *pdev)
{
    HI_S32 Ret;

    Ret = down_interruptible(&g_ViMutex);
    if (Ret)
    {
        HI_ERR_VI("%s: down_interruptible err !\n", __func__);
        return Ret;
    }

    HI_DRV_VI_Resume();

    up(&g_ViMutex);
    return HI_SUCCESS;
}

HI_S32 VI_DRV_ModInit(HI_VOID)
{
    HI_S32 Ret;

    Ret = HI_OSAL_Snprintf(g_ViRegisterData.devfs_name, sizeof(g_ViRegisterData.devfs_name),
                           "%s", UMAP_DEVNAME_VI);
    if (0 == Ret)
    {
        HI_ERR_VI("HI_OSAL_Snprintf failed\n");
        return HI_FAILURE;
    }

    g_ViRegisterData.fops   = &g_ViFileOps;
    g_ViRegisterData.minor  = UMAP_MIN_MINOR_VI;
    g_ViRegisterData.owner  = THIS_MODULE;
    g_ViRegisterData.drvops = &g_ViDrvOps;
    if (HI_DRV_DEV_Register(&g_ViRegisterData) < 0)
    {
        HI_FATAL_VI("register VI failed.\n");
        return HI_FAILURE;
    }

    Ret = HI_DRV_MODULE_Register(HI_ID_VI, "HI_VI", (HI_VOID*)&g_ViExtFuncs);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_VI("HI_DRV_MODULE_Register failed, mode ID = 0x%08X\n", HI_ID_VI);
        HI_DRV_DEV_UnRegister(&g_ViRegisterData);
        return HI_FAILURE;
    }

    VI_DRV_InitParam();

#ifdef MODULE
    HI_PRINT("Load hi_vi.ko success.\t\t(%s)\n", VERSION_STRING);
#endif

    return HI_SUCCESS;
}

HI_VOID VI_DRV_ModExit(HI_VOID)
{
    HI_DRV_MODULE_UnRegister(HI_ID_VI);
    HI_DRV_DEV_UnRegister(&g_ViRegisterData);

    return;
}

#ifdef MODULE
module_init(VI_DRV_ModInit);
module_exit(VI_DRV_ModExit);
#endif

MODULE_AUTHOR("HISILICON");
MODULE_LICENSE("GPL");
