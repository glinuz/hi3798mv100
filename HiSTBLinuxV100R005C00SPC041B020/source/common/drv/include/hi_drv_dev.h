/****************************************************************************
 *             Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: hi_drv_dev.h
 * Description:
 *    Implement all function for add UMAP device into OS.
 * 
 * History:
 * Version   Date         Author     DefectNum    Description
 * 1.1       2006-03-24   l48554     NULL         Create
 ****************************************************************************/
#ifndef __HI_DRV_DEV_H__
#define __HI_DRV_DEV_H__

#include <linux/module.h>
#include <linux/device.h>
#include <linux/major.h>
#include <asm/types.h>
#include <linux/fs.h>
#include <asm/atomic.h>

#include "hi_type.h"
#include "hi_debug.h"
#include "hi_drv_struct.h"

#ifdef __cplusplus
extern "C"
{
#endif /* End of #ifdef __cplusplus */

/** @addtogroup H_DEV */
/** @{ */

#define HIMEDIA_DEVICE_NAME_MAX_LEN 64

#define HIMEDIA_DEVICE_MAJOR 218
#define HIMEDIA_DYNAMIC_MINOR 255

typedef struct tagPM_BASEDEV_S{
	HI_S32		id;
	const HI_S8	* name;
	struct device	dev;
}PM_BASEDEV_S;

#define TO_PM_BASEDEV(x) container_of((x), PM_BASEDEV_S, dev)

typedef struct tagPM_BASEOPS_S {
	HI_S32  (*probe)(PM_BASEDEV_S *);
	HI_S32  (*remove)(PM_BASEDEV_S *);
	HI_VOID (*shutdown)(PM_BASEDEV_S *);
	HI_S32  (*prepare)(PM_BASEDEV_S *);
	HI_VOID (*complete)(PM_BASEDEV_S *);
	HI_S32  (*suspend)(PM_BASEDEV_S *, pm_message_t state);
	HI_S32  (*suspend_late)(PM_BASEDEV_S *, pm_message_t state);
	HI_S32  (*resume_early)(PM_BASEDEV_S *);
	HI_S32  (*resume)(PM_BASEDEV_S *);
}PM_BASEOPS_S;


typedef struct tagPM_BASEDRV_S {
	HI_S32  (*probe) (PM_BASEDEV_S *);
	HI_S32  (*remove)(PM_BASEDEV_S *);
	HI_VOID (*shutdown)(PM_BASEDEV_S *);
	HI_S32  (*suspend)(PM_BASEDEV_S *, pm_message_t state);
	HI_S32  (*suspend_late)(PM_BASEDEV_S *, pm_message_t state);
	HI_S32  (*resume_early)(PM_BASEDEV_S *);
	HI_S32  (*resume)(PM_BASEDEV_S *);
	struct device_driver driver;
}PM_BASEDRV_S;

#define to_himedia_basedrv(drv)	container_of((drv), PM_BASEDRV_S, driver)


typedef struct tagPM_DEVICE_S  {
	HI_S32 minor;
	const HI_S8 *name;
	struct module *owner;
	const struct file_operations *app_ops;
	PM_BASEOPS_S *base_ops;
	struct list_head list;
	struct device *app_device;
	PM_BASEDEV_S *base_device;
	PM_BASEDRV_S *base_driver;
}PM_DEVICE_S;

	
typedef struct hiUMAP_DEVICE_S
{
    HI_CHAR devfs_name[HIMEDIA_DEVICE_NAME_MAX_LEN];     /* devfs */
    HI_S32  minor;
	struct module *owner;
    struct file_operations *fops;
	PM_BASEOPS_S *drvops;
}UMAP_DEVICE_S, *PTR_UMAP_DEVICE_S;

HI_S32  DRV_PM_ModInit(HI_VOID);
HI_VOID DRV_PM_ModExit(HI_VOID);

HI_S32  HI_DRV_DEV_KInit(HI_VOID);
HI_VOID HI_DRV_DEV_KExit(HI_VOID);

HI_S32  HI_DRV_DEV_Register(UMAP_DEVICE_S *umapd);
HI_VOID HI_DRV_DEV_UnRegister(UMAP_DEVICE_S *umapd);

HI_S32 HI_DRV_PM_Register(PM_DEVICE_S *);
HI_S32 HI_DRV_PM_UnRegister(PM_DEVICE_S *);

extern HI_S32 HI_DRV_UserCopy(struct inode *inode, struct file *file,
           HI_U32 cmd, unsigned long arg,
           HI_S32 (*func)(struct inode *inode, struct file *file,
               HI_U32 cmd, HI_VOID *arg));
/** @} */

#define HI_FATAL_DEV(fmt...) \
            HI_TRACE(HI_LOG_LEVEL_FATAL, HI_ID_LOG, fmt)
#define HI_ERR_DEV(fmt...) \
            HI_TRACE(HI_LOG_LEVEL_ERROR, HI_ID_LOG, fmt)
#define HI_WARN_DEV(fmt...) \
            HI_TRACE(HI_LOG_LEVEL_WARNING, HI_ID_LOG, fmt)
#define HI_INFO_DEV(fmt...) \
            HI_TRACE(HI_LOG_LEVEL_INFO, HI_ID_LOG, fmt)

#ifdef __cplusplus
}
#endif /* End of #ifdef __cplusplus */


#endif /* __HI_DRV_DEV_H__ */

