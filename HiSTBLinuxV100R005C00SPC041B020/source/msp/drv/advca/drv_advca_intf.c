/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_advca_intf.c
  Version       : Initial Draft
  Author        : Hisilicon hisecurity team
  Created       :
  Last Modified :
  Description   :
  Function List :
  History       :
******************************************************************************/
#include <linux/jiffies.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/delay.h>
#include <linux/slab.h>
#include <linux/poll.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/mm.h>
#include "hi_common.h"
#include "hi_module.h"
#include "hi_unf_advca.h"
#include "hi_drv_cipher.h"
#include "hi_drv_otp.h"
#include "hi_drv_sys.h"
#include "hi_drv_dev.h"
#include "hi_drv_proc.h"
#include "hi_drv_reg.h"
#include "drv_advca.h"
#include "drv_advca_ext.h"
#include "drv_otp_ext.h"
#include "drv_advca_ioctl.h"
#include "hi_drv_module.h"
#include "drv_advca_internal.h"
#include "drv_advca_external.h"
#include "runtime_module.h"

atomic_t g_CaRefCnt = ATOMIC_INIT(0);

#define MUTEX_MAX_NUMBER 2
#define MUTEX_NAME_MAX_LEN 100

typedef struct CA_Sema
{
    struct semaphore CaSem;
    HI_BOOL isUsed;
}CA_Sema_S;

typedef struct CA_Mutex
{
    struct mutex  ca_oplock;
    CA_Sema_S ca_sema[MUTEX_MAX_NUMBER];
}CA_Mutex_S;

CA_Mutex_S g_ca_mutex;

static UMAP_DEVICE_S caUmapDev;

extern HI_S32  DRV_ADVCA_ModeInit_0(HI_VOID);
extern HI_VOID DRV_ADVCA_ModeExit_0(HI_VOID);
extern int DRV_ADVCA_Open(struct inode *inode, struct file *filp);
extern int DRV_ADVCA_Release(struct inode *inode, struct file *filp);
extern OTP_EXPORT_FUNC_S *g_pOTPExportFunctionList;

#define CA_BASE     0x10000000

HI_S32 CA_Ioctl(struct inode *inode, struct file *file, unsigned int cmd, void* arg);

HI_S32 ca_atomic_read(HI_U32 *pu32Value)
{
    if (NULL == pu32Value)
    {
        HI_FATAL_CA("Error! Null pointer input in ca atomic read!\n");
        return HI_FAILURE;
    }

    *pu32Value = atomic_read(&g_CaRefCnt);

    return HI_SUCCESS;
}

HI_S32 ca_atomic_dec_return(HI_U32 *pu32Value)
{
    if (NULL == pu32Value)
    {
        HI_FATAL_CA("Error! Null pointer input in ca atomic read!\n");
        return HI_FAILURE;
    }

    *pu32Value = atomic_dec_return(&g_CaRefCnt);

    return HI_SUCCESS;
}

HI_S32 ca_atomic_inc_return(HI_U32 *pu32Value)
{
    if (NULL == pu32Value)
    {
        HI_FATAL_CA("Error! Null pointer input in ca atomic inc return!\n");
        return HI_FAILURE;
    }

    *pu32Value = atomic_inc_return(&g_CaRefCnt);
    return HI_SUCCESS;
}

HI_S32 ca_down_interruptible(HI_U32 *pu32Value)
{
    struct semaphore *pCaSem = NULL;

    pCaSem = (struct semaphore*)pu32Value;
    if (NULL == pCaSem)
    {
        return HI_FAILURE;
    }

    if (down_interruptible(pCaSem))
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 ca_down_trylock(HI_U32 *pu32Value)
{
    HI_S32 s32Ret = 0;
    struct semaphore *pCaSem = NULL;

    pCaSem = (struct semaphore*)pu32Value;
    if (NULL == pCaSem)
    {
        return HI_FAILURE;
    }

    s32Ret = down_trylock(pCaSem);
    if (0 != s32Ret)
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_VOID ca_up(HI_U32 *pu32Value)
{
    struct semaphore *pCaSem = NULL;

    pCaSem = (struct semaphore*)pu32Value;
    if (NULL == pCaSem)
    {
        return ;
    }

    up(pCaSem);
    return ;
}

HI_VOID ca_initMutex(HI_U32 *pu32Value)
{
    struct semaphore *pCaSem = NULL;

    if (NULL == pu32Value)
    {
        HI_FATAL_CA("Invalid Input param, NULL pointer in ca_initMutex.\n");
        return;
    }

    pCaSem = (struct semaphore*)pu32Value;

    sema_init(pCaSem, 1);

    return;
}

HI_U32* ca_create_mutex(HI_VOID)
{
    HI_U8 i = 0;
    HI_U32 *pu32Value = NULL;

    mutex_lock(&g_ca_mutex.ca_oplock);
    for (i = 0; i < MUTEX_MAX_NUMBER; i++)
    {
        if (g_ca_mutex.ca_sema[i].isUsed == HI_FALSE)
        {
            break;
        }
    }

    if (i < MUTEX_MAX_NUMBER)
    {
        pu32Value = (HI_U32 *)&g_ca_mutex.ca_sema[i].CaSem;
        g_ca_mutex.ca_sema[i].isUsed = HI_TRUE;
    }
    else
    {
        HI_FATAL_CA("Error! fail to create ca mutex!\n");
        pu32Value = NULL;
    }
    mutex_unlock(&g_ca_mutex.ca_oplock);

    return pu32Value;
}

HI_VOID *ca_ioremap_nocache(HI_U32 u32Addr, HI_U32 u32Len)
{
    return ioremap_nocache(u32Addr, u32Len);
}

HI_VOID ca_iounmap(HI_VOID *pAddr)
{
    if (NULL == pAddr)
    {
        HI_ERR_CA("Invalid Input param\n");
        return;
    }
    iounmap(pAddr);
    return;
}

HI_VOID ca_msleep(HI_U32 u32Time)
{
    msleep(u32Time);
    return;
}

HI_S32 ca_snprintf(char * buf, HI_U32 size, const char * fmt, ...)
{
    HI_S32 retval;
    va_list ap;

    va_start(ap,fmt);
    retval = vsnprintf(buf, size, fmt,ap);
    va_end(ap);

    return retval;
}

HI_VOID CA_readReg(HI_U8 *addr, HI_U32 *pu32Result)
{
    if ((NULL == addr) || (NULL == pu32Result))
    {
        HI_ERR_CA("Invalid Input param\n");
        return;
    }
    HI_REG_READ32(addr, *pu32Result);
    return;
}

HI_VOID CA_writeReg(HI_U8 *addr, HI_U32 u32Result)
{
    if (NULL == addr)
    {
        HI_ERR_CA("Invalid Input param\n");
        return;
    }
    HI_REG_WRITE32(addr, u32Result);
    return;
}

HI_VOID CA_OTP_READ_REG(HI_U32 addr, HI_U32 *pu32Result)
{
    HI_U8 * pu8VirAddr = HI_NULL;

    if (NULL == pu32Result)
    {
        HI_ERR_OTP("Invalid Input param\n");
        return;
    }
    pu8VirAddr= (HI_U8*)ioremap_nocache(addr,0x10);
    if(HI_NULL == pu8VirAddr)
    {
        HI_ERR_OTP("ioremap_nocache map error\n");
        return;
    }
    HI_REG_READ32(pu8VirAddr, *pu32Result);
    iounmap(pu8VirAddr);
    return;
}

HI_VOID CA_OTP_WRITE_REG(HI_U32 addr, HI_U32 u32Result)
{
    HI_U8 * pu8VirAddr = HI_NULL;

    pu8VirAddr= (HI_U8*)ioremap_nocache(addr,0x10);
    if(HI_NULL == pu8VirAddr)
    {
        HI_ERR_OTP("ioremap_nocache map error\n");
        return;
    }
    HI_REG_READ32(pu8VirAddr, u32Result);
    iounmap(pu8VirAddr);
    return;
}

unsigned int ddr_wakeup_check_code[] =
{
    #include DDR_WAKEUP_SRC
};

extern int (* ca_pm_suspend)(HI_VOID *sramAddr, int length);

int locate_ca_pm_suspend(HI_VOID *sramAddr, int length)
{
    if (NULL == sramAddr)
    {
        HI_ERR_CA("Invalid Input param\n");
        return -1;
    }

    if (length <= sizeof (ddr_wakeup_check_code))
    {
        return -1;
    }

    memcpy(sramAddr, ddr_wakeup_check_code, sizeof (ddr_wakeup_check_code));

    return 0;
}

int ADVCA_PM_Resume(PM_BASEDEV_S *pdev)
{
#if    defined(CHIP_TYPE_hi3716mv300)   \
    || defined(CHIP_TYPE_hi3716mv310)   \
    || defined(CHIP_TYPE_hi3716mv320)   \
    || defined(CHIP_TYPE_hi3110ev500)   \
    || defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3719cv100)   \
    || defined(CHIP_TYPE_hi3716mv410)   \
    || defined(CHIP_TYPE_hi3716mv420)   \
    || defined(CHIP_TYPE_hi3798cv200)   \
    || defined(CHIP_TYPE_hi3798mv200) \
    || defined(CHIP_TYPE_hi3798mv200_a)
    Config_LPC_Setting();
#endif

    HI_PRINT("ADVCA resume OK\n");
    return HI_SUCCESS;
}


int ADVCA_PM_Suspend(PM_BASEDEV_S *pdev, pm_message_t state)
{
    HI_PRINT("ADVCA suspend OK\n");

    ca_pm_suspend = locate_ca_pm_suspend;

    return HI_SUCCESS;
}

static long DRV_ADVCA_Ioctl(struct file *ffile, unsigned int cmd, unsigned long arg)
{
    long ret;

    ret = (long)HI_DRV_UserCopy(ffile->f_dentry->d_inode, ffile, cmd, arg, CA_Ioctl);

    return ret;
}

static struct file_operations ca_fpops =
{
    .owner = THIS_MODULE,
    .open = DRV_ADVCA_Open,
    .release = DRV_ADVCA_Release,
    .unlocked_ioctl = DRV_ADVCA_Ioctl,
#ifdef CONFIG_COMPAT
    .compat_ioctl = DRV_ADVCA_Ioctl,
#endif
};

static PM_BASEOPS_S ca_drvops =
{
    .probe        = NULL,
    .remove       = NULL,
    .shutdown     = NULL,
    .prepare      = NULL,
    .complete     = NULL,
    .suspend      = ADVCA_PM_Suspend,
    .suspend_late = NULL,
    .resume_early = NULL,
    .resume       = ADVCA_PM_Resume,
};

/*****************************************************************************
 Prototype    :
 Description  : CA模块 proc 函数
 Input        : None
 Output       : None
 Return Value : None
*****************************************************************************/
extern HI_S32 DRV_ADVCA_GetChipVersion(HI_CHIP_TYPE_E *penChipType, HI_CHIP_VERSION_E *penChipVersion);


/*****************************************************************************
 Prototype    :
 Description  : CA模块 注册函数
 Input        : None
 Output       : None
 Return Value : None
*****************************************************************************/
HI_S32 ADVCA_DRV_ModeInit(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U8 i = 0;


    mutex_init(&g_ca_mutex.ca_oplock);

    for (i=0; i< MUTEX_MAX_NUMBER; i++)
    {
        g_ca_mutex.ca_sema[i].isUsed = HI_FALSE;
    }

    ret = DRV_ADVCA_ModeInit_0();
    if(ret != HI_SUCCESS)
    {
        return HI_FAILURE;
    }

    ca_snprintf(caUmapDev.devfs_name, sizeof(caUmapDev.devfs_name), UMAP_DEVNAME_CA);
    caUmapDev.minor  = UMAP_MIN_MINOR_CA;
    caUmapDev.owner  = THIS_MODULE;
    caUmapDev.fops   = &ca_fpops;
    caUmapDev.drvops = &ca_drvops;
    if (HI_DRV_DEV_Register(&caUmapDev) < 0)
    {
        HI_FATAL_CA("register CA failed.\n");
        goto err0;
    }


#if    defined(CHIP_TYPE_hi3716mv300)   \
    || defined(CHIP_TYPE_hi3716mv310)   \
    || defined(CHIP_TYPE_hi3716mv320)   \
    || defined(CHIP_TYPE_hi3110ev500)   \
    || defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3719cv100)   \
    || defined(CHIP_TYPE_hi3716mv410)   \
    || defined(CHIP_TYPE_hi3716mv420)   \
    || defined(CHIP_TYPE_hi3798cv200)   \
    || defined(CHIP_TYPE_hi3798mv200) \
    || defined(CHIP_TYPE_hi3798mv200_a)
    ret = RuntimeModule_Init();
    if (ret != HI_SUCCESS)
    {
        DRV_ADVCA_ModeExit_0();
        return HI_FAILURE;
    }
#endif

#ifdef MODULE
    HI_PRINT("Load hi_advca.ko success.\t(%s)\n", VERSION_STRING);
#endif

    return HI_SUCCESS;

err0:

    DRV_ADVCA_ModeExit_0();
    return HI_FAILURE;
}

HI_VOID ADVCA_DRV_ModeExit(HI_VOID)
{

    HI_DRV_DEV_UnRegister(&caUmapDev);

#if    defined(CHIP_TYPE_hi3716mv300)   \
    || defined(CHIP_TYPE_hi3716mv310)   \
    || defined(CHIP_TYPE_hi3716mv320)   \
    || defined(CHIP_TYPE_hi3110ev500)   \
    || defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3719cv100)   \
    || defined(CHIP_TYPE_hi3716mv410)   \
    || defined(CHIP_TYPE_hi3716mv420)   \
    || defined(CHIP_TYPE_hi3798cv200)   \
    || defined(CHIP_TYPE_hi3798mv200) \
    || defined(CHIP_TYPE_hi3798mv200_a)
    RuntimeModule_Exit();
#endif

    DRV_ADVCA_ModeExit_0();

    return;
}

#ifdef MODULE
module_init(ADVCA_DRV_ModeInit);
module_exit(ADVCA_DRV_ModeExit);
#endif

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("HISILICON");

