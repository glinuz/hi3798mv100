/***********************************************************************************
*              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: hi_tuner.c
* Description:
*
* History:
* Version   Date             Author       DefectNum    Description
* main\1    2007-08-03   w54542      NULL            Create this file.
* main\1    2007-11-10   w54542      NULL            modify this file.
***********************************************************************************/
//#include <linux/config.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <asm/atomic.h>
#include <linux/miscdevice.h>
#include <asm/io.h>
#include <linux/kthread.h>
#include <linux/delay.h>
//#include <linux/himedia.h>

#include "hi_type.h"
//#include "hi_i2c.h"
#include "drv_i2c_ext.h"
#include "drv_gpioi2c_ext.h"
#include "hi_debug.h"

#include "hi_drv_tuner.h"
#include "drv_tuner_ext.h"
#include "drv_demod.h"
#include "drv_tuner_ioctl.h"

#include "hi_unf_i2c.h"

#include "hi_drv_dev.h"
#include "hi_drv_proc.h"
#include "hi_drv_module.h"
//#include "common_dev.h"
//#include "common_proc.h"
//#include "common_mem.h"
//#include "common_stat.h"
//#include "hi_common_id.h"
//#include "common_module_drv.h"

extern HI_S32 hi_tuner_open(struct inode *inode, struct file *filp);
extern HI_S32 hi_tuner_release(struct inode *inode, struct file *filp);
extern long hi_tuner_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);
#if (1 == HI_PROC_SUPPORT)
    extern HI_S32 tuner_proc_read(struct seq_file *p, HI_VOID *v);
    extern HI_S32 tuner_proc_read_reg(struct seq_file *p, HI_VOID *v);
    extern HI_S32 tuner_proc_read_diseqc(struct seq_file *p, HI_VOID *v);
	extern HI_S32 tuner_proc_write(struct file * file,
                                 const char __user * buffer, size_t count, 
                                 loff_t *ppos);
#endif
extern HI_S32 tuner_resume(PM_BASEDEV_S *pdev);
extern HI_S32 tuner_suspend (PM_BASEDEV_S *pdev, pm_message_t state);

#if defined(CHIP_TYPE_hi3716cv200es) || defined(CHIP_TYPE_hi3716cv200) || defined(CHIP_TYPE_hi3716mv400)\
    || defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)
#if (1 == HI_PROC_SUPPORT)
#ifndef HI_ADVCA_FUNCTION_RELEASE
    extern HI_S32 qam_ram_collect_data(struct file * file, const char __user * buf, size_t count, loff_t *ppos);
    extern HI_S32 adc_collect_data(struct file * file, const char __user * buf, size_t count, loff_t *ppos);
#else
    HI_S32 qam_ram_collect_data(struct file * file, const char __user * buf, size_t count, loff_t *ppos)
    {
        return HI_SUCCESS;
    }
    HI_S32 adc_collect_data(struct file * file, const char __user * buf, size_t count, loff_t *ppos)
    {
        return HI_SUCCESS;
    }
#endif
#endif
#endif

static struct file_operations hi_tuner_fops =
{
    .owner = THIS_MODULE,
    .unlocked_ioctl = hi_tuner_ioctl,
#ifdef CONFIG_COMPAT
    .compat_ioctl = hi_tuner_ioctl,
#endif
    .open  = hi_tuner_open,
    .release = hi_tuner_release,
};

static PM_BASEOPS_S tuner_baseOps =
{
    .probe  = NULL,
    .remove = NULL,
    .shutdown = NULL,
    .prepare  = NULL,
    .complete = NULL,
    .suspend   = tuner_suspend,
    .suspend_late = NULL,
    .resume_early = NULL,
    .resume  = tuner_resume
};

static UMAP_DEVICE_S  hi_tuner_dev =
{
    .devfs_name = UMAP_DEVNAME_TUNER,
    .minor  = UMAP_MIN_MINOR_TUNER,
    .owner  = THIS_MODULE,
    .fops   = &hi_tuner_fops,
    .drvops = &tuner_baseOps
};

#if defined(CHIP_TYPE_hi3716cv200es) || defined(CHIP_TYPE_hi3716cv200) || defined(CHIP_TYPE_hi3716mv400)\
    || defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)
HI_S32 qam_ram_collect(struct seq_file * p, HI_VOID * v)
{
     return HI_SUCCESS;
}

HI_S32 adc_ram_collect(struct seq_file * p, HI_VOID * v)
{
     return HI_SUCCESS;
}

#endif

HI_S32 TUNER_DRV_ModInit(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;
#if (1 == HI_PROC_SUPPORT)
    DRV_PROC_ITEM_S *tuner_proc = NULL;
    DRV_PROC_ITEM_S *tuner_reg_proc = NULL;

#if defined(CHIP_TYPE_hi3716cv200es) || defined(CHIP_TYPE_hi3716cv200) || defined(CHIP_TYPE_hi3716mv400)\
    || defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)   
	DRV_PROC_ITEM_S *ram_collect_proc = NULL;
	DRV_PROC_ITEM_S *tuner_qtc_proc = NULL;
#endif
#endif
    /* pre */
#ifndef HI_MCE_SUPPORT
    ret = HI_DRV_TUNER_Init();
    if (HI_SUCCESS != ret)
    {
        HI_ERR_TUNER("HI_DRV_TUNER_Init err! \n");
        return ret;
    }
#endif

    /* misc register tuner */
    ret = HI_DRV_DEV_Register(&hi_tuner_dev);
    if (HI_SUCCESS != ret)
    {
        HI_INFO_TUNER("hi tuner(tuner and QAM) register failed!\n");
        ret = -EFAULT;
        goto err0;
    }
#if (1 == HI_PROC_SUPPORT)
    /*proc*/
    tuner_proc = HI_DRV_PROC_AddModule("tuner", NULL, NULL);
    if (! tuner_proc)
    {
        HI_INFO_TUNER("add proc tuner module failed\n");
        ret = HI_FAILURE;
        goto err1;
    }
    tuner_proc->read = tuner_proc_read;
    tuner_proc->write = tuner_proc_write;

    tuner_reg_proc = HI_DRV_PROC_AddModule("tuner_reg", NULL, NULL);
    if (! tuner_reg_proc)
    {
        HI_INFO_TUNER("add proc tuner_reg module failed\n");
        ret = HI_FAILURE;
        goto err1;
    }
    tuner_reg_proc->read = tuner_proc_read_reg;

    tuner_reg_proc = HI_DRV_PROC_AddModule("tuner_diseqc", NULL, NULL);
    if (! tuner_reg_proc)
    {
        HI_INFO_TUNER("add proc tuner_diseqc module failed\n");
        ret = HI_FAILURE;
        goto err1;
    }
    tuner_reg_proc->read = tuner_proc_read_diseqc;

#if defined(CHIP_TYPE_hi3716cv200es) || defined(CHIP_TYPE_hi3716cv200) || defined(CHIP_TYPE_hi3716mv400)\
    || defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)
    
	//DRV_PROC_ITEM_S *ram_collect_proc = NULL;

    ram_collect_proc = HI_DRV_PROC_AddModule("qam_ram_collect", NULL, NULL);
    if (! ram_collect_proc)
    {
        HI_FATAL_TUNER("add proc qam_ram_collect module failed\n");
        ret = HI_FAILURE;
        goto err1;
    }
    ram_collect_proc->read = qam_ram_collect;
    ram_collect_proc->write = qam_ram_collect_data;
	
    //ADC_QTC
	//DRV_PROC_ITEM_S *tuner_qtc_proc = NULL;

    tuner_qtc_proc = HI_DRV_PROC_AddModule("tuner_qtc", NULL, NULL);
    if (! tuner_qtc_proc)
    {
        HI_FATAL_TUNER("add proc tuner_qtc module failed\n");
        ret = HI_FAILURE;
        goto err1;
    }
    tuner_qtc_proc->read = adc_ram_collect;
    tuner_qtc_proc->write = adc_collect_data;
#endif
#endif
   
#ifdef MODULE
    HI_PRINT("Load hi_tuner.ko success.\t(%s)\n", VERSION_STRING);
#endif

    return HI_SUCCESS;

#if (1 == HI_PROC_SUPPORT)
err1:
#endif
    HI_DRV_MODULE_UnRegister(HI_ID_TUNER);
    HI_DRV_DEV_UnRegister(&hi_tuner_dev);
err0:
#ifndef HI_MCE_SUPPORT
    HI_DRV_TUNER_DeInit();
#endif
    return ret;
}

HI_VOID TUNER_DRV_ModExit(HI_VOID)
{
#if defined(CHIP_TYPE_hi3716cv200es) || defined(CHIP_TYPE_hi3716cv200) || defined(CHIP_TYPE_hi3716mv400)\
    || defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)
    HI_DRV_PROC_RemoveModule("qam_ram_collect");
	HI_DRV_PROC_RemoveModule("tuner_qtc");
#endif

    HI_DRV_PROC_RemoveModule("tuner");
    HI_DRV_PROC_RemoveModule("tuner_reg");
	HI_DRV_PROC_RemoveModule("tuner_diseqc");
    HI_DRV_MODULE_UnRegister(HI_ID_TUNER);
    HI_DRV_DEV_UnRegister(&hi_tuner_dev);
#ifndef HI_MCE_SUPPORT
    HI_DRV_TUNER_DeInit();
#endif
    //HI_I2C_Close();

    return;
}

#ifdef MODULE
module_init(TUNER_DRV_ModInit);
module_exit(TUNER_DRV_ModExit);
#endif

#ifndef HI_ADVCA_FUNCTION_RELEASE
MODULE_AUTHOR("Digital Media Team ,Hisilicon crop ");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("tuner interface for Hi3716 Solution");
#else
MODULE_AUTHOR("");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("");
#endif

