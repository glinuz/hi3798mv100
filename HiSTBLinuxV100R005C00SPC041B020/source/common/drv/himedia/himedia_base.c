#include <linux/module.h>
#include <linux/init.h>
#include <linux/dma-mapping.h>
#include <linux/bootmem.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/string.h>

//#include "hibase.h"
#include "himedia_base.h"



/*********************************************************************/
/**** himedia bus  ****/
/*********************************************************************/


static void himedia_bus_release(struct device *dev)
{
	return;
}

struct device himedia_bus = {
	.init_name	= "himediaBusDev",
	.release    = himedia_bus_release
};  /*top level bus, parent and bus member are both NULL*//*CNcomment:这是顶层总线，parent 和 bus 成员为 NULL*/

//EXPORT_SYMBOL_GPL(himedia_bus);



#if 1
static ssize_t modalias_show(struct device *dev, struct device_attribute *a,
							char *buf)
{
	PM_BASEDEV_S *pdev = TO_PM_BASEDEV(dev);
	int len = snprintf(buf, PAGE_SIZE, "himedia:%s\n", (char*)pdev->name);

	return (len >= PAGE_SIZE) ? (PAGE_SIZE - 1) : len;
}

static struct device_attribute himedia_dev_attrs[] = {
	__ATTR_RO(modalias),
	__ATTR_NULL,
};
#endif

static int himedia_match(struct device *dev, struct device_driver *drv)
{
	PM_BASEDEV_S *pdev = TO_PM_BASEDEV(dev);
	return (strncmp(pdev->name, drv->name, HIMEDIA_DEVICE_NAME_MAX_LEN+8) == 0);
}

static int himedia_uevent(struct device *dev, struct kobj_uevent_env *env)
{
	PM_BASEDEV_S	*pdev = TO_PM_BASEDEV(dev);
	add_uevent_var(env, "MODALIAS=himedia:%s", pdev->name);
	return 0;
}

#if 1 
//#ifdef CONFIG_PM_SLEEP

static int himedia_legacy_suspend(struct device *dev, pm_message_t mesg)
{
	int ret = 0;

	if (dev->driver && dev->driver->suspend)
		ret = dev->driver->suspend(dev, mesg);

	return ret;
}


static int himedia_legacy_suspend_late(struct device *dev, pm_message_t mesg)
{
	int ret = 0;
	PM_BASEDEV_S *pdev = TO_PM_BASEDEV(dev);
	PM_BASEDRV_S *pdrv = to_himedia_basedrv(dev->driver);

	if (dev->driver && pdrv->suspend_late)
		ret = pdrv->suspend_late(pdev, mesg);

	return ret;
}

static int himedia_legacy_resume_early(struct device *dev)
{
	int ret = 0;
	PM_BASEDEV_S *pdev = TO_PM_BASEDEV(dev);
	PM_BASEDRV_S *pdrv = to_himedia_basedrv(dev->driver);

	if (dev->driver && pdrv->resume_early)
		ret = pdrv->resume_early(pdev);

	return ret;
}

static int himedia_legacy_resume(struct device *dev)
{
	int ret = 0;

	if (dev->driver && dev->driver->resume)
		ret = dev->driver->resume(dev);

	return ret;
}

static int himedia_pm_prepare(struct device *dev)
{
	struct device_driver *drv = dev->driver;
	int ret = 0;

	if (drv && drv->pm && drv->pm->prepare)
		ret = drv->pm->prepare(dev);

	return ret;
}

static void himedia_pm_complete(struct device *dev)
{
	struct device_driver *drv = dev->driver;

	if (drv && drv->pm && drv->pm->complete)
		drv->pm->complete(dev);
}

//#ifdef CONFIG_SUSPEND
#if 1

static int himedia_pm_suspend(struct device *dev)
{
	int ret = 0;
	struct device_driver *drv = dev->driver;

	if (!drv)
		return 0;

	if (drv->pm) {
		if (drv->pm->suspend)
			ret = drv->pm->suspend(dev);
	} else {
		ret = himedia_legacy_suspend(dev, PMSG_SUSPEND);
	}

	return ret;
}

static int himedia_pm_suspend_noirq(struct device *dev)
{
	int ret = 0;
	struct device_driver *drv = dev->driver;

	if (!drv)
		return 0;

	if (drv->pm) {
		if (drv->pm->suspend_noirq)
			ret = drv->pm->suspend_noirq(dev);
	} else {
		ret = himedia_legacy_suspend_late(dev, PMSG_SUSPEND);
	}

	return ret;
}

static int himedia_pm_resume(struct device *dev)
{
	int ret = 0;
	struct device_driver *drv = dev->driver;

//	HI_PRINT("%s %s %d \n", __FILE__, __FUNCTION__, __LINE__);


	if (!drv)
		return 0;

	if (drv->pm) {
		if (drv->pm->resume)
			ret = drv->pm->resume(dev);
	} else {
		ret = himedia_legacy_resume(dev);
	}

	return ret;
}

static int himedia_pm_resume_noirq(struct device *dev)
{
	int ret = 0;
	struct device_driver *drv = dev->driver;

//	HI_PRINT("111 %s %s %d \n", __FILE__, __FUNCTION__, __LINE__);
	

	if (!drv)
		return 0;

	if (drv->pm) {
		if (drv->pm->resume_noirq)
			ret = drv->pm->resume_noirq(dev);
	} else {
		ret = himedia_legacy_resume_early(dev);
	}

	return ret;
}

#else /* !CONFIG_SUSPEND */

#define himedia_pm_suspend		    NULL
#define himedia_pm_resume		    NULL
#define himedia_pm_suspend_noirq	NULL
#define himedia_pm_resume_noirq	    NULL

#endif /* !CONFIG_SUSPEND */

//#ifdef  CONFIG_HIBERNATION
#if defined(CONFIG_PM_HIBERNATE) || defined(CONFIG_HISI_SNAPSHOT_BOOT)
static int himedia_pm_freeze(struct device *dev)
{
	int ret = 0;
	struct device_driver *drv = dev->driver;

	if (!drv)
		return 0;

	if (drv->pm) {
		if (drv->pm->freeze)
			ret = drv->pm->freeze(dev);
	} else {
		ret = himedia_legacy_suspend(dev, PMSG_FREEZE);
	}

	return ret;
}

static int himedia_pm_freeze_noirq(struct device *dev)
{
	int ret = 0;
	struct device_driver *drv = dev->driver;

	if (!drv)
		return 0;

	if (drv->pm) {
		if (drv->pm->freeze_noirq)
			ret = drv->pm->freeze_noirq(dev);
	} else {
		ret = himedia_legacy_suspend_late(dev, PMSG_FREEZE);
	}

	return ret;
}

static int himedia_pm_thaw(struct device *dev)
{
	int ret = 0;
	struct device_driver *drv = dev->driver;

	if (!drv)
		return 0;

	if (drv->pm) {
		if (drv->pm->thaw)
			ret = drv->pm->thaw(dev);
	} else {
		ret = himedia_legacy_resume(dev);
	}

	return ret;
}

static int himedia_pm_thaw_noirq(struct device *dev)
{
	int ret = 0;
	struct device_driver *drv = dev->driver;

	if (!drv)
		return 0;

	if (drv->pm) {
		if (drv->pm->thaw_noirq)
			ret = drv->pm->thaw_noirq(dev);
	} else {
		ret = himedia_legacy_resume_early(dev);
	}

	return ret;
}

static int himedia_pm_poweroff(struct device *dev)
{
	int ret = 0;
	struct device_driver *drv = dev->driver;

	if (!drv)
		return 0;

	if (drv->pm) {
		if (drv->pm->poweroff)
			ret = drv->pm->poweroff(dev);
	} else {
		ret = himedia_legacy_suspend(dev, PMSG_HIBERNATE);
	}

	return ret;
}

static int himedia_pm_poweroff_noirq(struct device *dev)
{
	int ret = 0;
	struct device_driver *drv = dev->driver;

	if (!drv)
		return 0;

	if (drv->pm) {
		if (drv->pm->poweroff_noirq)
			ret = drv->pm->poweroff_noirq(dev);
	} else {
		ret = himedia_legacy_suspend_late(dev, PMSG_HIBERNATE);
	}

	return ret;
}

static int himedia_pm_restore(struct device *dev)
{
	int ret = 0;
	struct device_driver *drv = dev->driver;

	if (!drv)
		return 0;

	if (drv->pm) {
		if (drv->pm->restore)
			ret = drv->pm->restore(dev);
	} else {
		ret = himedia_legacy_resume(dev);
	}

	return ret;
}

static int himedia_pm_restore_noirq(struct device *dev)
{
	int ret = 0;
	struct device_driver *drv = dev->driver;

	if (!drv)
		return 0;

	if (drv->pm) {
		if (drv->pm->restore_noirq)
			ret = drv->pm->restore_noirq(dev);
	} else {
		ret = himedia_legacy_resume_early(dev);
	}

	return ret;
}

#else /* !CONFIG_HIBERNATION */

#define himedia_pm_freeze		    NULL
#define himedia_pm_thaw		        NULL
#define himedia_pm_poweroff		    NULL
#define himedia_pm_restore		    NULL
#define himedia_pm_freeze_noirq	    NULL
#define himedia_pm_thaw_noirq		NULL
#define himedia_pm_poweroff_noirq	NULL
#define himedia_pm_restore_noirq	NULL

#endif /* !CONFIG_HIBERNATION */

static struct dev_pm_ops himedia_dev_pm_ops = {
	.prepare        = himedia_pm_prepare,
	.complete       = himedia_pm_complete,
	.suspend        = himedia_pm_suspend,
	.resume         = himedia_pm_resume,
	.freeze         = himedia_pm_freeze,
	.thaw           = himedia_pm_thaw,
	.poweroff       = himedia_pm_poweroff,
	.restore        = himedia_pm_restore,
	.suspend_noirq  = himedia_pm_suspend_noirq,
	.resume_noirq   = himedia_pm_resume_noirq,
	.freeze_noirq   = himedia_pm_freeze_noirq,
	.thaw_noirq     = himedia_pm_thaw_noirq,
	.poweroff_noirq = himedia_pm_poweroff_noirq,
	.restore_noirq  = himedia_pm_restore_noirq,
};

#define HIMEDIA_PM_OPS_PTR	(&himedia_dev_pm_ops)

#else /* !CONFIG_PM_SLEEP */

#define HIMEDIA_PM_OPS_PTR	NULL

#endif /* !CONFIG_PM_SLEEP */


struct bus_type himedia_bus_type = {
	.name		= "himediaBus",
	.dev_attrs	= himedia_dev_attrs,
	.match		= himedia_match,
	.uevent		= himedia_uevent,
	.pm			= HIMEDIA_PM_OPS_PTR,
};
// EXPORT_SYMBOL_GPL(himedia_bus_type);


int himedia_bus_init(void)
{
	int ret;
	ret = device_register(&himedia_bus);
	if (ret)
		return ret;
	ret =  bus_register(&himedia_bus_type);
	if (ret)
		device_unregister(&himedia_bus);
	return ret;
}

void himedia_bus_exit(void)
{
	bus_unregister(&himedia_bus_type);
	device_unregister(&himedia_bus);
	return;
}


/*********************************************************************/
/***  himedia  base  device  ***/
/*********************************************************************/
int himedia_device_add(PM_BASEDEV_S *pdev)
{
	// 0
	if (!pdev)
		return -EINVAL;
	// 1
	if (!pdev->dev.parent)
		pdev->dev.parent = &himedia_bus;
	pdev->dev.bus = &himedia_bus_type;

	if (pdev->id != -1)
		dev_set_name(&pdev->dev, "%s.%d", pdev->name,  pdev->id);
	else
		dev_set_name(&pdev->dev, "%s",pdev->name);

#if 0
	HI_PRINT("Registering himedia device '%s'. Parent at %s\n",
	dev_name(&pdev->dev), dev_name(pdev->dev.parent));
#endif
	return device_add(&pdev->dev);
}
//EXPORT_SYMBOL_GPL(himedia_device_add);

void himedia_device_del(PM_BASEDEV_S *pdev)
{
	if (pdev) {
		device_del(&pdev->dev);
	}
	return;
}
//EXPORT_SYMBOL_GPL(himedia_device_del);


void himedia_device_put(PM_BASEDEV_S *pdev)
{
	if (pdev)
		put_device(&pdev->dev);
}
//EXPORT_SYMBOL_GPL(himedia_device_put);


struct himedia_devobj {
	PM_BASEDEV_S pdev;
	char name[1];
};


static void himedia_device_release(struct device *dev)
{
	struct himedia_devobj *pa = container_of(dev, struct himedia_devobj, 
								pdev.dev);
	kfree(pa);
	return;
}

PM_BASEDEV_S *himedia_device_alloc(const char *name, int id)
{
	int size ;
	struct himedia_devobj *pa;
	size = strlen(name) + 8;
	pa = kzalloc(sizeof(struct himedia_devobj) + size, GFP_KERNEL);
	if (pa) {
		snprintf(pa->name, size, "%s-base", name);
		//strncpy(pa->name, name, strlen(name)+1);
		pa->pdev.name = pa->name;
		pa->pdev.id   = id;
		device_initialize(&pa->pdev.dev);
		pa->pdev.dev.release = himedia_device_release;
	}
	return pa ? &pa->pdev : NULL;
}
//EXPORT_SYMBOL_GPL(himedia_device_alloc);


int himedia_device_register(PM_BASEDEV_S *pdev)
{
	device_initialize(&pdev->dev);
	return himedia_device_add(pdev);
}

void himedia_device_unregister(PM_BASEDEV_S *pdev)
{
	himedia_device_del(pdev);
	himedia_device_put(pdev);
	return;
}





/*********************************************************************/
/***  himedia  base  driver  ***/
/*********************************************************************/

static int himedia_drv_probe(struct device *dev)
{
	PM_BASEDRV_S *pdrv = to_himedia_basedrv(dev->driver);
	PM_BASEDEV_S *pdev = TO_PM_BASEDEV(dev);

	return pdrv->probe(pdev);
}

static int himedia_drv_remove(struct device *dev)
{
	PM_BASEDRV_S *pdrv = to_himedia_basedrv(dev->driver);
	PM_BASEDEV_S *pdev = TO_PM_BASEDEV(dev);

	return pdrv->remove(pdev);
}

static void himedia_drv_shutdown(struct device *dev)
{
	PM_BASEDRV_S *pdrv = to_himedia_basedrv(dev->driver);
	PM_BASEDEV_S *pdev = TO_PM_BASEDEV(dev);

	pdrv->shutdown(pdev);
	return;
}

static int himedia_drv_suspend(struct device *dev, pm_message_t state)
{
	PM_BASEDRV_S *pdrv = to_himedia_basedrv(dev->driver);
	PM_BASEDEV_S *pdev = TO_PM_BASEDEV(dev);

	return pdrv->suspend(pdev, state);
}

static int himedia_drv_resume(struct device *dev)
{
	PM_BASEDRV_S *pdrv = to_himedia_basedrv(dev->driver);
	PM_BASEDEV_S *pdev = TO_PM_BASEDEV(dev);

	return pdrv->resume(pdev);
}


int himedia_driver_register(PM_BASEDRV_S *drv)
{
	drv->driver.bus = &himedia_bus_type;
	if (drv->probe)
		drv->driver.probe    = himedia_drv_probe;
	if (drv->remove)
		drv->driver.remove   = himedia_drv_remove;
	if (drv->shutdown)
		drv->driver.shutdown = himedia_drv_shutdown;
	if (drv->suspend)
		drv->driver.suspend  = himedia_drv_suspend;
	if (drv->resume)
		drv->driver.resume   = himedia_drv_resume;
	return driver_register(&drv->driver);
}
//EXPORT_SYMBOL_GPL(himedia_driver_register0);


void himedia_driver_unregister(PM_BASEDRV_S *drv)
{
	driver_unregister(&drv->driver);
}
//EXPORT_SYMBOL_GPL(himedia_driver_unregister0);


struct himedia_drvobj {
	PM_BASEDRV_S pdrv;
	char name[1];
};



PM_BASEDRV_S *himedia_driver_alloc(const char *name, struct module *owner, 
								PM_BASEOPS_S *ops)
{
	int size;
	struct himedia_drvobj *pa;
	size = strlen(name) + 8;
	pa = kzalloc(sizeof(struct himedia_drvobj) + size, GFP_KERNEL);
	if (pa) {
		// 0
		snprintf(pa->name, size, "%s-base", name);
		//strncpy(pa->name, name, strlen(name)+1);
		pa->pdrv.driver.name  = pa->name;
		pa->pdrv.driver.owner = owner;
		// 1
		if (ops && ops->probe) {
			pa->pdrv.probe = ops->probe;
		}else{
			pa->pdrv.probe = NULL;
		}

		if (ops && ops->remove) {
			pa->pdrv.remove = ops->remove;
		}else{
			pa->pdrv.remove = NULL;
		}

		if (ops && ops->shutdown) {
			pa->pdrv.shutdown = ops->shutdown;
		}else {
			pa->pdrv.shutdown = NULL;
		}

		if (ops && ops->suspend) {
			pa->pdrv.suspend = ops->suspend;
		}else{
			pa->pdrv.suspend = NULL;
		}

		if (ops && ops->resume) {
			pa->pdrv.resume = ops->resume;
		}else{
			pa->pdrv.resume = NULL;
		}
		
		if (ops && ops->suspend_late){
			pa->pdrv.suspend_late = ops->suspend_late;
		}else{
			pa->pdrv.suspend_late = NULL;
		}
		
		if (ops && ops->resume_early){
			pa->pdrv.resume_early = ops->resume_early;
		}else{
			pa->pdrv.resume_early = NULL;
		}
	}
	return pa ? &pa->pdrv : NULL;
}

void himedia_driver_release(PM_BASEDRV_S *drv)
{
	struct himedia_drvobj *pa = container_of(drv, struct himedia_drvobj, pdrv);
	kfree(pa);
	return;
}




