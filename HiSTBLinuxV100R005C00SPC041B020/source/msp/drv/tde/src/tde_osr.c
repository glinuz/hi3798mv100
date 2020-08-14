/******************************************************************************
*
* Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name           : tde_osr.c
Version             : Initial Draft
Author              :
Created             : 2015/07/11
Description         :
Function List       :
History             :
Date                            Author                  Modification
2015/07/11                     y00181162                         Created file
******************************************************************************/
#ifndef HI_BUILD_IN_BOOT
#include <linux/version.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <linux/interrupt.h>
#include <linux/poll.h>
#include <linux/workqueue.h>
#include <asm/io.h>
#include "tde_proc.h"
#include "tde_osilist.h"
#else
#include "tde_hal.h"
#include "tde_osilist.h"
#include "tde_adp.h"
#endif
#include "tde_config.h"
#include "hi_type.h"


#ifndef HI_BUILD_IN_BOOT
	#define MKSTR(exp) # exp
	#define MKMARCOTOSTR(exp) MKSTR(exp)
	char* g_pszTdeMmzName = NULL;
	module_param(g_pszTdeMmzName, charp, S_IRUGO);
	MODULE_PARM_DESC(g_pszTdeMmzName, "The mmz name for tde.");

	HI_U32 g_u32TdeTmpBuf = 0;
	module_param(g_u32TdeTmpBuf, uint, S_IRUGO);

	MODULE_PARM_DESC(g_u32TdeTmpBuf, "TDE Tmp buffer.");

	extern int tde_init_module_k(void);
	extern void tde_cleanup_module_k(void);
	extern int tde_open(struct inode *finode, struct file  *ffile);
	extern int tde_release(struct inode *finode, struct file  *ffile);

	extern long tde_ioctl(struct file  *ffile, unsigned int  cmd, unsigned long arg);
	#ifdef CONFIG_TDE_PM_ENABLE
	extern int tde_pm_suspend(PM_BASEDEV_S *pdev, pm_message_t state);
	extern int tde_pm_resume(PM_BASEDEV_S *pdev);
	#endif
	DECLARE_GFX_NODE("hi_tde",tde_open, tde_release, NULL, tde_ioctl,tde_ioctl,tde_pm_suspend, tde_pm_resume);
#else

int tde_osr_init(void)
{
    if (TdeHalInit(TDE_REG_BASEADDR) < 0){
        return -1;
    }
    TdeOsiListInit();
    TDE_TRACE(TDE_KERN_DEBUG, "init tde successful!\n");
    return 0;
}
#endif

#ifndef HI_BUILD_IN_BOOT
void tde_osr_deinit(void)
{
    TdeOsiListTerm();
    TdeHalRelease();
    return;
}
#endif


#ifndef HI_BUILD_IN_BOOT
#ifndef CONFIG_TDE_VERSION_DISABLE
STATIC HI_VOID HI_GFX_ShowVersionK(HIGFX_MODE_ID_E ModID)
{
#if !defined(CONFIG_GFX_COMM_VERSION_DISABLE) && !defined(CONFIG_GFX_COMM_DEBUG_DISABLE) && defined(MODULE)
    HI_CHAR MouleName[7][10] __attribute__((unused))= {"tde", "jpegdec", "jpegenc", "fb", "png", "higo", "gfx2d"};

    if (ModID >= HIGFX_BUTT_ID)
    {
        return;
    }

    GFX_Printk("Load hi_%s.ko success.\t\t(%s)\n", MouleName[ModID], VERSION_STRING);

#endif
}
#endif

HI_S32  TDE_DRV_ModInit(HI_VOID)
{
    int ret = 0;
#ifndef HI_MCE_SUPPORT
    ret = tde_init_module_k();
    if (0 != ret)
    {
        return -1;
    }
#endif
#ifdef CONFIG_TDE_PM_ENABLE
    /* register tde device */
    ret = HI_GFX_PM_Register();
    if(0!=ret)
    {
        return -1;
    }
#endif
#ifndef CONFIG_TDE_PROC_DISABLE
    {
       GFX_PROC_ITEM_S pProcItem = {tde_read_proc,tde_write_proc,NULL};
       HI_GFX_PROC_AddModule("tde", &pProcItem, NULL);
    }
#endif

#ifndef CONFIG_TDE_VERSION_DISABLE
   HI_GFX_ShowVersionK(HIGFX_TDE_ID);
#endif
    return 0;
}

HI_VOID  TDE_DRV_ModExit(HI_VOID)
{
#ifndef CONFIG_TDE_PROC_DISABLE
    HI_GFX_PROC_RemoveModule("tde");
#endif

#ifndef HI_MCE_SUPPORT
    tde_cleanup_module_k();
#endif
#ifdef CONFIG_TDE_PM_ENABLE
    /* cleanup_module is never called if registering failed */
    HI_GFX_PM_UnRegister();
#endif
}

#ifdef MODULE
module_init(TDE_DRV_ModInit);
module_exit(TDE_DRV_ModExit);
#endif

#ifndef HI_ADVCA_FUNCTION_RELEASE
MODULE_AUTHOR(AUTHOR);
MODULE_DESCRIPTION(DESCRIPTION);
MODULE_LICENSE(MPP_LICENSE);
MODULE_VERSION(TDE_VERSION);
#else
MODULE_AUTHOR("");
MODULE_DESCRIPTION("");
MODULE_LICENSE(MPP_LICENSE);
MODULE_VERSION("");
#endif

#endif
