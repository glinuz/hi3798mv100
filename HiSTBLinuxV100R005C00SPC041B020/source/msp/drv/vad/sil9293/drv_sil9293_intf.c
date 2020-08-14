/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : drv_sil9293_intf.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/06/03
  Description   :
  History       :
  1.Date        : 2013/06/03
    Author      : j00169368
    Modification: Created file

******************************************************************************/

#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <asm/io.h>
#include <asm/uaccess.h>

#include "hi_drv_dev.h"
#include "hi_drv_proc.h"
#include "hi_drv_module.h"
#include "hi_module.h"
#include "hi_kernel_adapt.h"

#include "hi_drv_hdmirx.h"
#include "drv_sil9293.h"
#include "hi_osal.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif

#define strtoul  simple_strtoul   

extern HI_U32 gpioScl;
extern HI_U32 gpioSda;
extern HI_U32 gpioRst;
extern HI_U32 gpioInt;
extern bool bLowCi2ca;

HI_DECLARE_MUTEX(g_Sil9293Mutex);

static HI_S32 SIL9293_DRV_Suspend(PM_BASEDEV_S *pdev, pm_message_t state);
static HI_S32 SIL9293_DRV_Resume(PM_BASEDEV_S *pdev);
static HI_S32 SIL9293_DRV_Open(struct inode *finode, struct file *ffile);
static HI_S32 SIL9293_DRV_Close(struct inode *finode, struct file *ffile);
static long   SIL9293_DRV_Ioctl(struct file *ffile, unsigned int cmd, unsigned long arg);

static UMAP_DEVICE_S g_Sil9293RegisterData;
static PM_BASEOPS_S g_Sil9293DrvOps =
{
    .probe        = NULL,
    .remove       = NULL,
    .shutdown     = NULL,
    .prepare      = NULL,
    .complete     = NULL,
    .suspend      = SIL9293_DRV_Suspend,
    .suspend_late = NULL,
    .resume_early = NULL,
    .resume       = SIL9293_DRV_Resume,
};
static struct file_operations g_Sil9293FileOps =
{
    .owner			= THIS_MODULE,
    .open			= SIL9293_DRV_Open,
    .unlocked_ioctl = SIL9293_DRV_Ioctl,
    .release		= SIL9293_DRV_Close,
};

extern HDMIRX_CONTEXT_S g_stHdmirxCtx;

static HI_S32 SIL9293_DRV_Open(struct inode *finode, struct file *ffile)
{
    g_stHdmirxCtx.bOpened = HI_TRUE;

    return HI_SUCCESS;
}

static HI_S32 SIL9293_DRV_Close(struct inode *finode, struct file *ffile)
{
    if (HI_TRUE == g_stHdmirxCtx.bOpened)
    {
        HI_DRV_SIL9293_DisConnect();
    }

    return HI_SUCCESS;
}

HI_S32 SIL9293_DRV_Ioctl_k(struct inode *inode, struct file *file, unsigned int cmd, HI_VOID *arg)
{
    HI_S32 s32Ret;

    s32Ret = down_interruptible(&g_Sil9293Mutex);
    if (s32Ret)
    {
        HI_ERR_HDMIRX("%s: down_interruptible err!\n", __func__);
        return s32Ret;
    }

    switch (cmd)
    {
    case CMD_HDMIRX_CONNECT:
    {
        s32Ret = HI_DRV_SIL9293_Connect();
        break;
    }
    case CMD_HDMIRX_DIS_CONNECT:
    {
        HI_DRV_SIL9293_DisConnect();
        break;
    }
    case CMD_HDMIRX_SET_GPIO_PIN:
    {
        HI_HDMIRX_SET_GPIO_PIN_S *pstGpioPin;
        pstGpioPin = (HI_HDMIRX_SET_GPIO_PIN_S *)arg;
        s32Ret = HI_DRV_SIL9293_SetGpioPin(pstGpioPin);
        break;
    }
    case CMD_HDMIRX_GET_STATUS:
    {
        HI_SIG_STATUS_E *pstSigStatus;
        pstSigStatus = (HI_SIG_STATUS_E *)arg;
        s32Ret = HI_DRV_SIL9293_GetStatus(pstSigStatus);
        break;
    }
    case CMD_HDMIRX_GET_ATTR:
    {
        HI_HDMIRX_TIMING_INFO_S *pstTiming;
        pstTiming = (HI_HDMIRX_TIMING_INFO_S *)arg;
        s32Ret = HI_DRV_SIL9293_GetTiming(pstTiming);
        break;
    }
    default:
    {
        HI_ERR_HDMIRX("sil9293 cmd value err, cmd=%x\n", cmd);
        up(&g_Sil9293Mutex);
        return -ENOIOCTLCMD;
    }
    }

    up(&g_Sil9293Mutex);

    return s32Ret;
}

static long SIL9293_DRV_Ioctl(struct file *ffile, unsigned int cmd, unsigned long arg)
{
    long Ret;

    Ret = HI_DRV_UserCopy(ffile->f_dentry->d_inode, ffile, cmd, arg, SIL9293_DRV_Ioctl_k);
    return Ret;
}

static HI_S32 SIL9293_DRV_Suspend(PM_BASEDEV_S *pdev, pm_message_t state)
{
    HI_S32 s32Ret;
    
    s32Ret = down_interruptible(&g_Sil9293Mutex);
    if (s32Ret)
    {
        HI_ERR_HDMIRX("%s: down_interruptible err!\n", __func__);
        return s32Ret;
    }

    if (HI_TRUE == g_stHdmirxCtx.bOpened)
    {
        HI_DRV_SIL9293_DisConnect();

        g_stHdmirxCtx.bOpened = HI_TRUE;
       
    }
    HI_PRINT("SIL9293 Suspend OK\n");
    up(&g_Sil9293Mutex);

    return HI_SUCCESS;
}

static HI_S32 SIL9293_DRV_Resume(PM_BASEDEV_S *pdev)
{
    HI_S32 s32Ret;
    
    s32Ret = down_interruptible(&g_Sil9293Mutex);
    if (s32Ret)
    {
        HI_ERR_HDMIRX("%s: down_interruptible err!\n", __func__);
        return s32Ret;
    }
    if (HI_TRUE == g_stHdmirxCtx.bOpened)
    {
        HI_DRV_SIL9293_Connect();
    }
    
    HI_PRINT("SIL9293 Resume OK\n");
    up(&g_Sil9293Mutex);

    return HI_SUCCESS;
}
#if (1 == HI_PROC_SUPPORT)
static HI_S32 SIL9293_PROC_Read(struct seq_file *p, HI_VOID *v)
{
    if (HI_FALSE == g_stHdmirxCtx.bOpened)
    {
        HI_INFO_HDMIRX("Open first\n");
        return HI_SUCCESS;
    }
    else
    {
        return SIL9293_ProcShow(p);
    }
}
static HI_VOID SIL9293_DRV_ProcHelp(HI_VOID)
{
    HI_DRV_PROC_EchoHelper( "Read/Write register usage: echo [r/w] [DevAddr] [RegAddr] [data] >/proc/msp/sil9293 \n"
                            "Write sample:\n echo w 0x64 0x4a 0xea >/proc/msp/sil9293 \n\n"
                            "Read sample:\n echo r 0x64 0x4a >/proc/msp/sil9293 \n\n");
#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
    HI_DRV_PROC_EchoHelper( "Read/Write edid usage: echo edid [r/w] [file] >/proc/msp/sil9293 \n"
                            "Write sample:\n echo edid w /edid.bin >/proc/msp/sil9293 \n"
                            "Read sample:\n echo edid r >/proc/msp/sil9293 \n");
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
}

HI_S32 SIL9293_DRV_ParseProcPara(HI_CHAR *pProcPara,HI_CHAR **ppArg, HI_U32 count)
{
    HI_CHAR *pChar = HI_NULL;
    HI_U32 i;

    for(i=0;i<count;i++)
    {
        ppArg[i] = HI_NULL;        
    }
    
    if (strlen(pProcPara) == 0)
    {
        /* not fined arg1 and arg2, return failed */
        return HI_FAILURE;
    }
    
    pChar = pProcPara;
    
    for(i=0; i<count; i++)
    {
         while( (*pChar == ' ') && (*pChar != '\0') )
        {
            pChar++;
        }
       
        if (*pChar != '\0')
        {
             ppArg[i] = pChar;
        }
        else
        {
            return HI_FAILURE;
        }

        while( (*pChar != ' ') && (*pChar != '\0') )
        {
            pChar++;
        }

        if (*pChar == '\0')
        {
            return HI_SUCCESS;
        }
        *pChar = '\0';
        pChar = pChar + 1;
    }

    return HI_SUCCESS;
}


static HI_S32 SIL9293_PROC_Write(struct file * file,
                                 const char __user * buf, size_t count, loff_t *ppos)
{
    HI_CHAR  ProcPara[64]={0};
    HI_CHAR *pArg[5];
    HI_S32 Ret;
    HI_U32 u32Addr;
    HI_U8 u8Value;
    HI_U8 u8Drv;

    if (count >= 1 && count < 64 && buf != HI_NULL)
    {
        memset(ProcPara, 0, sizeof(ProcPara));
        if (copy_from_user(ProcPara, buf, count))
        {
            HI_ERR_HDMIRX("copy_from_user failed.\n");
            return HI_FAILURE;
        }

        ProcPara[count] = '\0';

        Ret = SIL9293_DRV_ParseProcPara(ProcPara,pArg,4);
        if (HI_SUCCESS != Ret)
        {
            SIL9293_DRV_ProcHelp();
            return HI_FAILURE;
        }

        if (HI_SUCCESS == HI_OSAL_Strncmp(pArg[0], "w", strlen("w")))
        {
            u8Drv = strtoul(pArg[1], NULL, 16);
            u32Addr = strtoul(pArg[2], NULL, 16);

           SIL9293_Read(u8Drv, u32Addr, &u8Value);
           
           HI_DRV_PROC_EchoHelper("write:[0x%02x]=0x%02x --> ",u32Addr, u8Value);
           
           u8Value = strtoul(pArg[3], NULL, 16);
           SIL9293_Write(u8Drv, u32Addr, u8Value);
           HI_DRV_PROC_EchoHelper("0x%02x\n", u8Value);
         }
        else if (HI_SUCCESS == HI_OSAL_Strncmp(pArg[0], "r", strlen("r")))
        {
            u8Drv = strtoul(pArg[1], NULL, 16);
            u32Addr = strtoul(pArg[2], NULL, 16);
            SIL9293_Read(u8Drv, u32Addr, &u8Value);

            HI_DRV_PROC_EchoHelper("read:[0x%02x]=0x%02x\n",u32Addr, u8Value);
        }
        else
        {
            SIL9293_DRV_ProcHelp();
        }
    }
    return count;
}

HI_S32 SIL9293_PROC_Create(HI_VOID)
{
    HI_S32 Ret;
    DRV_PROC_ITEM_S *pProcItem;
    HI_CHAR ProcName[12];

    Ret = HI_OSAL_Snprintf(ProcName, sizeof(ProcName), "sil9293");
    if (0 == Ret)
    {
        HI_ERR_HDMIRX("HI_OSAL_Snprintf failed!\n");
        return HI_FAILURE;
    }

    pProcItem = HI_DRV_PROC_AddModule(ProcName, HI_NULL, HI_NULL);
    if (!pProcItem)
    {
        HI_ERR_HDMIRX("add %s proc failed.\n", ProcName);
        return HI_FAILURE;
    }

    pProcItem->read  = SIL9293_PROC_Read;
    pProcItem->write = SIL9293_PROC_Write;

    return HI_SUCCESS;
}

HI_S32 SIL9293_PROC_Destroy(HI_VOID)
{
    HI_S32 Ret;
    HI_CHAR ProcName[12];

    memset(ProcName, 0, sizeof(ProcName));

    Ret = HI_OSAL_Snprintf(ProcName, sizeof(ProcName), "sil9293");
    if (0 == Ret)
    {
        HI_ERR_HDMIRX("HI_OSAL_Snprintf failed!\n");
        return HI_FAILURE;
    }

    HI_DRV_PROC_RemoveModule(ProcName);
    return HI_SUCCESS;
}
#endif

HI_S32 SIL9293_DRV_ModInit(HI_VOID)
{
    HI_S32 Ret;

    Ret = HI_OSAL_Snprintf(g_Sil9293RegisterData.devfs_name, sizeof(g_Sil9293RegisterData.devfs_name),
                           "%s", UMAP_DEVNAME_SIL9293);
    if (0 == Ret)
    {
        HI_ERR_HDMIRX("HI_OSAL_Snprintf failed\n");
        return HI_FAILURE;
    }

    g_Sil9293RegisterData.fops   = &g_Sil9293FileOps;
    g_Sil9293RegisterData.minor  = UMAP_MIN_MINOR_SIL9293;
    g_Sil9293RegisterData.owner  = THIS_MODULE;
    g_Sil9293RegisterData.drvops = &g_Sil9293DrvOps;
    if (HI_DRV_DEV_Register(&g_Sil9293RegisterData) < 0)
    {
        HI_ERR_HDMIRX("register SIL9293 failed.\n");
        goto ERR0;
    }

    Ret = HI_DRV_MODULE_Register(HI_ID_SIL9293, "HI_SIL9293", HI_NULL);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_HDMIRX("HI_DRV_MODULE_Register failed, mode ID = 0x%08X\n", HI_ID_SIL9293);
        goto ERR1;
    }
#if (1 == HI_PROC_SUPPORT)
    Ret = SIL9293_PROC_Create();
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_HDMIRX("SIL9293_PROC_Create failed.\n");
        HI_DRV_MODULE_UnRegister(HI_ID_SIL9293);
        HI_DRV_DEV_UnRegister(&g_Sil9293RegisterData);
        return HI_FAILURE;
    }
#endif

#ifdef MODULE
    HI_PRINT("Load hi_sil9293.ko success.\t(%s)\n", VERSION_STRING);
#endif

    return HI_SUCCESS;


    HI_DRV_MODULE_UnRegister(HI_ID_SIL9293);
ERR1:
    HI_DRV_DEV_UnRegister(&g_Sil9293RegisterData);
ERR0:
    return HI_FAILURE;
}

HI_VOID SIL9293_DRV_ModExit(HI_VOID)
{
#if (1 == HI_PROC_SUPPORT)
    SIL9293_PROC_Destroy();
#endif
    HI_DRV_MODULE_UnRegister(HI_ID_SIL9293);
    HI_DRV_DEV_UnRegister(&g_Sil9293RegisterData);
}

module_param(gpioScl, int, S_IRUGO);
module_param(gpioSda, int, S_IRUGO);
module_param(gpioRst, int, S_IRUGO);
module_param(gpioInt, int, S_IRUGO);
module_param(bLowCi2ca, bool, S_IRUGO);

#ifdef MODULE
module_init(SIL9293_DRV_ModInit);
module_exit(SIL9293_DRV_ModExit);
#endif

MODULE_AUTHOR("HISILICON");
MODULE_LICENSE("GPL");

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif
