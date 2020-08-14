/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : common_log.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2008/02/29
  Description   :
  History       :
  1.Date        : 2008/02/29
    Author      : c42025
    Modification: Created file, we can adjust our log level as do as HI_PRINT, eg
    command line in shell :
    [pub@hi3511mpp]$echo "venc = 2" > /proc/msp/log
    will change log levle of module "VENC" to 2, then, all message with greate
    levle than "2" will be suppressed.
  2.Date        : 2009/12/17
    Author      : jianglei
    Modification: add the control of output way, it can output log info to network by add log buffer

******************************************************************************/
#include <linux/seq_file.h>
#include "hi_type.h"
#include "hi_debug.h"
#include "hi_osal.h"
#include "hi_drv_dev.h"
#include "hi_drv_module.h"
#include "hi_drv_mmz.h"
#include "drv_log.h"
#include "hi_drv_proc.h"
#include "hi_drv_log.h"
#include "drv_log_ioctl.h"


static UMAP_DEVICE_S    g_srtuDbgDev;

HI_S32 CMPI_LOG_Ioctl(struct inode *inode, struct file *file,
                             HI_U32 cmd, HI_VOID *arg)
{
    switch (cmd)
    {
        case UMAP_CMPI_LOG_INIT:
        {
            HI_U32 *pu32PhyAddr = (HI_U32*)arg;

            /*pass the address of config information to user-state to share config information*/
            HI_DRV_LOG_ConfgBufAddr(pu32PhyAddr);

            return HI_SUCCESS;
        }
        case UMAP_CMPI_LOG_EXIT:
        {
            return HI_SUCCESS;
        }
#if defined(LOG_NETWORK_SUPPORT) || defined(LOG_UDISK_SUPPORT)
        case UMAP_CMPI_LOG_READ_LOG:
        {
            LOG_BUF_READ_S *pPara = (LOG_BUF_READ_S*)arg;

            return HI_DRV_LOG_BufferRead(pPara->pMsgAddr, pPara->BufLen, &pPara->CopyedLen, HI_FALSE);
        }
        case UMAP_CMPI_LOG_WRITE_LOG:
        {
            LOG_BUF_WRITE_S *pPara = (LOG_BUF_WRITE_S*)arg;

            return HI_DRV_LOG_BufferWrite(pPara->pMsgAddr, pPara->MsgLen, MSG_FROM_USER);
        }
#endif
        case UMAP_CMPI_LOG_SET_PATH:
        {
            LOG_PATH_S *pstPath = (LOG_PATH_S *)arg;

            return HI_DRV_LOG_SetPath(pstPath->pszPath, pstPath->u32PathLen);
        }
        case UMAP_CMPI_LOG_SET_STORE_PATH:
        {
            STORE_PATH_S *pstPath = (STORE_PATH_S *)arg;

            return HI_DRV_LOG_SetStorePath(pstPath->pszPath, pstPath->u32PathLen);
        }
        default:
            return HI_FAILURE;
    }

    UNUSED(file);
    UNUSED(inode);
}

static long LOG_DRV_Ioctl(struct file *file, HI_U32 cmd, unsigned long arg)
{
    return (long)HI_DRV_UserCopy(file->f_dentry->d_inode, file, cmd, arg, CMPI_LOG_Ioctl);
}

#ifdef CONFIG_COMPAT
HI_S32 CMPI_Compat_LOG_Ioctl(struct inode *inode, struct file *file,
                             HI_U32 cmd, HI_VOID *arg)
{
    switch(cmd)
    {
#if defined(LOG_NETWORK_SUPPORT) || defined(LOG_UDISK_SUPPORT)
        case UMAP_CMPI_LOG_Compat_READ_LOG:
        {
            LOG_Compat_BUF_READ_S *pPara = (LOG_Compat_BUF_READ_S*)arg;

            return HI_DRV_LOG_BufferRead((HI_U8*)compat_ptr(pPara->pMsgAddr), pPara->BufLen, &pPara->CopyedLen, HI_FALSE);
        }
        case UMAP_CMPI_LOG_Compat_WRITE_LOG:
        {
            LOG_Compat_BUF_WRITE_S *pPara = (LOG_Compat_BUF_WRITE_S*)arg;

            return HI_DRV_LOG_BufferWrite((HI_U8*)compat_ptr(pPara->pMsgAddr), pPara->MsgLen, MSG_FROM_USER);
        }
#endif
        case UMAP_CMPI_LOG_Compat_SET_PATH:
        {
            LOG_Compat_PATH_S *pstPath = (LOG_Compat_PATH_S *)arg;

            return HI_DRV_LOG_SetPath((HI_CHAR*)compat_ptr(pstPath->pszPath), pstPath->u32PathLen);
        }
        case UMAP_CMPI_LOG_Compat_SET_STORE_PATH:
        {
            STORE_Compat_PATH_S *pstPath = (STORE_Compat_PATH_S *)arg;

            return HI_DRV_LOG_SetStorePath((HI_CHAR*)compat_ptr(pstPath->pszPath), pstPath->u32PathLen);
        }
        default:
            return CMPI_LOG_Ioctl(inode, file, cmd, arg);
    }

    UNUSED(file);
    UNUSED(inode);
}

static long LOG_DRV_Compat_Ioctl(struct file *file, HI_U32 cmd, unsigned long arg)
{
    return (long)HI_DRV_UserCopy(file->f_dentry->d_inode, file, cmd, arg, CMPI_Compat_LOG_Ioctl);
}
#endif

static HI_S32 LOG_DRV_Release(struct inode * inode, struct file * file)
{
    return 0;
}

static HI_S32 LOG_DRV_Open(struct inode * inode, struct file * file)
{
    return 0;
}

static struct file_operations DRV_log_Fops=
{
    .owner = THIS_MODULE,
    .open = LOG_DRV_Open,
    .unlocked_ioctl = LOG_DRV_Ioctl,
#ifdef CONFIG_COMPAT
    .compat_ioctl = LOG_DRV_Compat_Ioctl,
#endif
    .release = LOG_DRV_Release,
};

HI_S32 HI_DRV_LOG_Init(HI_VOID)
{
    HI_S32 ret;
    DRV_PROC_ITEM_S *item = NULL;

    ret = HI_DRV_MODULE_Register(HI_ID_LOG, "HI_LOG", HI_NULL);
    if (HI_SUCCESS != ret)
    {
        return -1;
    }

    HI_OSAL_Snprintf(g_srtuDbgDev.devfs_name, sizeof(g_srtuDbgDev.devfs_name), "%s", UMAP_DEVNAME_LOG);
    g_srtuDbgDev.fops = &DRV_log_Fops;
    g_srtuDbgDev.minor = UMAP_MIN_MINOR_LOG;
    g_srtuDbgDev.owner  = THIS_MODULE;
    g_srtuDbgDev.drvops = NULL;
    if (HI_DRV_DEV_Register(&g_srtuDbgDev) < 0)
    {
        HI_ERR_LOG( "Unable to register dbg dev\n");
        return -1;
    }

    item = HI_DRV_PROC_AddModule(HI_MOD_LOG, NULL, NULL);
    if (!item)
    {
        HI_DRV_DEV_UnRegister(&g_srtuDbgDev);
        return -1;
    }
    item->read = HI_DRV_LOG_ProcRead;
    item->write = HI_DRV_LOG_ProcWrite;

    return 0;
}

HI_VOID HI_DRV_LOG_Exit(HI_VOID)
{
    HI_DRV_PROC_RemoveModule(HI_MOD_LOG);
    HI_DRV_DEV_UnRegister(&g_srtuDbgDev);
    HI_DRV_MODULE_UnRegister(HI_ID_LOG);
    return;
}

//module_param(LogBufSize, int, S_IRUGO);


