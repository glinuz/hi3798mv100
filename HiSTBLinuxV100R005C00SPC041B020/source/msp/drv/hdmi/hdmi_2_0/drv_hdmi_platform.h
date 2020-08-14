#ifndef __DRV_HDMI_PLATFORM__
#define __DRV_HDMI_PLATFORM__

#include <linux/version.h>
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/delay.h>
#include <linux/poll.h>
#include <linux/interrupt.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/wait.h>
#include <linux/sched.h>

#if defined(CONFIG_HDMI_STB_SDK) 
#include "hi_debug.h"
#include "hi_module.h"
#include "hi_drv_module.h"
#include "hi_drv_dev.h"
#include "hi_drv_mmz.h"
#include "hi_drv_mem.h"
#include "drv_disp_ext.h"
#include "hi_drv_proc.h"
#include "hi_drv_cipher.h"
#include "drv_cipher_ext.h"
#include "drv_pdm_ext.h"
#include "hi_kernel_adapt.h"
#include "hi_drv_sys.h"
#include "hi_reg_common.h"
#elif defined (CONFIG_HDMI_BVT_SDK) 
#include <linux/init.h>
#include <linux/miscdevice.h>
#include "proc_ext.h"
#include "hi_debug.h"
#include "mm_ext.h"
#include "dev_ext.h"
#include "mod_ext.h"
#endif

#include "hi_type.h"
#include "drv_hdmi_proc.h"
#include "drv_hdmi_log.h"
#include "drv_hdmi_debug.h"

/***************************** Macro Definition ******************************/

/** \addtogroup     HDMI COMM */
/** @{ */  /** <!-- 【HDMI COMM】 */
                 
/** pm disable */
/** CNcomment:pm使能 CNend */
//#define  CONFIG_GFX_COMM_PM_DISABLE

#ifdef HI_ADVCA_FUNCTION_RELEASE
/** PROC disable */
/** CNcomment:proc使能 CNend */
#define  CONFIG_HDMI_PROC_DISABLE
/** LOG disable */
/** CNcomment:log使能 CNend */
#define  CONFIG_HDMI_DEBUG_DISABLE
#endif

/** register mammap operate */
/** CNcomment:寄存器映射操作 CNend */
#define HI_HDMI_REG_MAP(base, size)                  ioremap_nocache((base), (size))
/** register unmap operate */
/** CNcomment:寄存器逆映射操作 CNend */
#define HI_HDMI_REG_UNMAP(base)                      iounmap((HI_VOID*)(base))


#define HDMI_MMZ_ALIGN_BYTES        16

#define HDMI_MEMSET 	memset
#define HDMI_MEMCPY		memcpy
#define HDMI_MEMCMP		memcmp

#define HDMI_STRLEN(p)  ({p ? strlen(p) : 0 ;})
#define HDMI_STRNCPY(p1,p2,size)    if(p1 && p2 && size){strncpy(p1,p2,size);}

//#define HDMI_ARRAY_SIZE(a)			(sizeof(a)/sizeof(a[0]))

#define HDMI_SPIN				    spinlock_t
#define HDMI_INIT_SPIN_LOCK(x) 	    spin_lock_init(&x)
#define HDMI_SPIN_LOCK(x)	        spin_lock(&x)
#define HDMI_SPIN_UNLOCK(x)		    spin_unlock(&x)

#define HDMI_WAIT_QUEUE     	    wait_queue_head_t 
#define HDMI_INIT_WAIT_QUEUE(x)     init_waitqueue_head(&x)
#define HDMI_INIT_MUTEX(x)          sema_init(&x, 1)

#define HI_HDMI_DECLARE_MUTEX(x)    DEFINE_SEMAPHORE(x)

/** Structure of proc item */
/** CNcomment:proc相关函数操作 CNend */
typedef struct strHDMI_PROC_ITEM
{
    HI_S32 (*fnRead)(struct seq_file *, HI_VOID *);
    HI_S32 (*fnWrite)(struct file * file,  const char __user * buf, size_t count, loff_t *ppos);
    HI_S32 (*fnIoctl)(struct seq_file *, HI_U32 cmd, HI_U32 arg);
}HDMI_PROC_ITEM_S;


#if defined(CONFIG_HDMI_STB_SDK) || defined(CONFIG_HDMI_ANDROID_SDK) || defined(CONFIG_HDMI_TV_SDK)

#ifdef CONFIG_HDMI_DEBUG_DISABLE
#define EDID_INFO(fmt...)      
#define EDID_WARN(fmt...)       
#define EDID_ERR(fmt...)       
#define EDID_FATAL(fmt...)        
#define HDMI_INFO(fmt...)        
#define HDMI_WARN(fmt...)        
#define HDMI_ERR(fmt...)        
#define HDMI_FATAL(fmt...)    
#elif defined(HDMI_DEBUG)
#define EDID_INFO(fmt...)      //HI_INFO_PRINT   (HI_ID_HDMI, fmt)
#define EDID_WARN(fmt...)      //HI_WARN_PRINT   (HI_ID_HDMI, fmt)    
#define EDID_ERR(fmt...)       //HI_ERR_PRINT    (HI_ID_HDMI, fmt)  
#define EDID_FATAL(fmt...)    // HI_FATAL_PRINT  (HI_ID_HDMI, fmt)    
#define HDMI_INFO(fmt...)      HI_INFO_PRINT   (HI_ID_HDMI, fmt); HDMI_LOG_INFO(fmt)
#define HDMI_WARN(fmt...)      HI_WARN_PRINT   (HI_ID_HDMI, fmt); HDMI_LOG_WARN(fmt)
#define HDMI_ERR(fmt...)       HI_ERR_PRINT    (HI_ID_HDMI, fmt); HDMI_LOG_ERR(fmt)
#define HDMI_FATAL(fmt...)     HI_FATAL_PRINT  (HI_ID_HDMI, fmt); HDMI_LOG_FATAL(fmt)
#else
#define EDID_INFO(fmt...)      //HI_INFO_PRINT   (HI_ID_HDMI, fmt)
#define EDID_WARN(fmt...)      //HI_WARN_PRINT   (HI_ID_HDMI, fmt)    
#define EDID_ERR(fmt...)       //HI_ERR_PRINT    (HI_ID_HDMI, fmt)  
#define EDID_FATAL(fmt...)    // HI_FATAL_PRINT  (HI_ID_HDMI, fmt)    
#define HDMI_INFO(fmt...)      HI_INFO_PRINT   (HI_ID_HDMI, fmt)
#define HDMI_WARN(fmt...)      HI_WARN_PRINT   (HI_ID_HDMI, fmt)
#define HDMI_ERR(fmt...)       HI_ERR_PRINT    (HI_ID_HDMI, fmt)
#define HDMI_FATAL(fmt...)     HI_FATAL_PRINT  (HI_ID_HDMI, fmt)
#endif

#elif defined(CONFIG_HDMI_BVT_SDK)
#define PROC_PRINT(arg...) seq_printf(arg)
#define VERSION_STRING ("[HDMI] Version:["MPP_VERSION"] Build Time:["__DATE__", "__TIME__"]")
#define HI_TRACE_HDMI(level, fmt...)      \
do{\
        HI_TRACE(level,HI_ID_HDMI,"[Func]:%s [Line]:%d [Info]:",__FUNCTION__, __LINE__);\
     HI_TRACE(level,HI_ID_HDMI,##fmt);\
} while(0)

#ifdef CONFIG_HDMI_DEBUG_DISABLE 
#define EDID_INFO(fmt...)      
#define EDID_WARN(fmt...)       
#define EDID_ERR(fmt...)       
#define EDID_FATAL(fmt...)        
#define HDMI_INFO(fmt...)        
#define HDMI_WARN(fmt...)        
#define HDMI_ERR(fmt...)        
#define HDMI_FATAL(fmt...)  
#elif defined(HDMI_DEBUG)
#define EDID_INFO(fmt...)     //HI_INFO_PRINT   (HI_ID_HDMI, fmt)
#define EDID_WARN(fmt...)     //HI_WARN_PRINT   (HI_ID_HDMI, fmt)    
#define EDID_ERR(fmt...)      //HI_ERR_PRINT    (HI_ID_HDMI, fmt)  
#define EDID_FATAL(fmt...)    //HI_FATAL_PRINT  (HI_ID_HDMI, fmt)    
#define HDMI_INFO(fmt...)     HI_TRACE_HDMI(HI_DBG_INFO, fmt); HDMI_LOG_INFO(fmt)
#define HDMI_WARN(fmt...)     HI_TRACE_HDMI(HI_DBG_WARN, fmt); HDMI_LOG_WARN(fmt)
#define HDMI_ERR(fmt...)      HI_TRACE_HDMI(HI_DBG_ERR, fmt);  HDMI_LOG_ERR(fmt)
#define HDMI_FATAL(fmt...)    HI_TRACE_HDMI(HI_DBG_EMERG, fmt); HDMI_LOG_FATAL(fmt)
#else
#define EDID_INFO(fmt...)     HI_TRACE_HDMI(HI_DBG_INFO, fmt)     
#define EDID_WARN(fmt...)     HI_TRACE_HDMI(HI_DBG_WARN, fmt) 
#define EDID_ERR(fmt...)      HI_TRACE_HDMI(HI_DBG_ERR, fmt) 
#define EDID_FATAL(fmt...)    HI_TRACE_HDMI(HI_DBG_EMERG, fmt)
#define HDMI_FATAL(fmt...)    HI_TRACE_HDMI(HI_DBG_EMERG, fmt)
#define HDMI_ERR(fmt...)      HI_TRACE_HDMI(HI_DBG_ERR, fmt)
#define HDMI_WARN(fmt...)     HI_TRACE_HDMI(HI_DBG_WARN, fmt)
#define HDMI_INFO(fmt...)     HI_TRACE_HDMI(HI_DBG_INFO, fmt)
#endif

#define DEVNAME_HDMI UMAP_DEVNAME_HDMI_BASE
#define DECLARE_HDMI_NODE(hdmi_name, hdmi_open, hdmi_release, hdmi_mmap, hdmi_ioctl, hdmi_suspend, hdmi_resume) \
static struct file_operations s_stHdmiFileOps =\
{\
    .owner   = THIS_MODULE,      \
    .unlocked_ioctl = hdmi_ioctl,\
    .open    = hdmi_open,        \
    .release = hdmi_release,     \
    .mmap    = hdmi_mmap         \
};\
static struct himedia_ops s_stHdmiDrvOps = \
{ \
    .pm_freeze          = hdmi_suspend,\
    .pm_restore         = hdmi_resume,\
};\
static UMAP_DEVICE_S s_stHdmiDev = \
{\
    .minor      = UMAP_HDMI_MINOR_BASE,\
    .owner      = THIS_MODULE,\
    .fops       = &s_stHdmiFileOps,\
    .drvops = &s_stHdmiDrvOps,\
};\


#else

#endif


#if defined(CONFIG_HDMI_STB_SDK) || defined(CONFIG_HDMI_ANDROID_SDK) || defined(CONFIG_HDMI_TV_SDK)

static inline HI_S32 HI_HDMI_PROC_AddModule(HI_CHAR * pEntry_name, HDMI_PROC_ITEM_S* pProcItem, HI_VOID *pData)
{
    #ifndef  CONFIG_HDMI_PROC_DISABLE
        DRV_PROC_EX_S stProcItem;
        stProcItem.fnIoctl =  pProcItem->fnIoctl;
        stProcItem.fnRead = pProcItem->fnRead;
        stProcItem.fnWrite= pProcItem->fnWrite;
        HI_DRV_PROC_AddModule(pEntry_name, &stProcItem, pData);
    #endif
    return 0;

}

static inline HI_VOID HI_HDMI_PROC_RemoveModule(HI_CHAR *pEntry_name)
{
    #ifndef  CONFIG_HDMI_PROC_DISABLE
      HI_DRV_PROC_RemoveModule(pEntry_name);
    #endif
}

static inline HI_S32 HI_HDMI_MODULE_Register(HI_U32 u32ModuleID, const HI_CHAR * pszModuleName, HI_VOID *pData)
{
    return HI_DRV_MODULE_Register(u32ModuleID, pszModuleName, pData);
}

static inline HI_S32 HI_HDMI_MODULE_UnRegister(HI_U32 u32ModuleID)
{
    return HI_DRV_MODULE_UnRegister(u32ModuleID);
}

#define HI_HDMI_DEV_Register(HdmiDev) HI_DRV_DEV_Register(&HdmiDev) 

#define HI_HDMI_DEV_UnRegister(HdmiDev) HI_DRV_DEV_UnRegister(&HdmiDev)

#define HI_HDMI_UserCopy(file, cmd, arg, HdmiIoctl) HI_DRV_UserCopy(file->f_dentry->d_inode, file, cmd, arg, HdmiIoctl)

#elif defined(CONFIG_HDMI_BVT_SDK)

static inline HI_S32 HI_HDMI_PROC_AddModule(HI_CHAR * pEntry_name, HDMI_PROC_ITEM_S* pProcItem, HI_VOID *pData)
{
#ifndef  CONFIG_HDMI_PROC_DISABLE
        CMPI_PROC_ITEM_S stProcItem;
        CMPI_PROC_ITEM_S* pstProcItem = HI_NULL;
		if (!pProcItem)
	    {
	        return HI_FAILURE;
	    }
        stProcItem.entry = NULL;
        stProcItem.pData = NULL;
        stProcItem.read = pProcItem->fnRead;
        stProcItem.write = pProcItem->fnWrite;
        pstProcItem = CMPI_CreateProc(pEntry_name,stProcItem.read, NULL);
		if (!pstProcItem)
        {
            return HI_FAILURE;
        }
    return HI_SUCCESS;
#else

    return HI_SUCCESS;
#endif	
}

static inline HI_VOID HI_HDMI_PROC_RemoveModule(HI_CHAR *pEntry_name)
{
    #ifndef  CONFIG_HDMI_PROC_DISABLE
      CMPI_RemoveProc(pEntry_name);
	return ;
#else
    #endif
}

#define HI_HDMI_MODULE_Register(u32ModuleID,pszModuleName,pData) CMPI_RegisterModule(&s_stHmdiModule);


#define HI_GFX_MODULE_UnRegister(u32ModuleID)  CMPI_UnRegisterModule(u32ModuleID);

#define HI_HDMI_DEV_Register(HdmiDev) CMPI_RegisterDevice(&HdmiDev) 

#define HI_HDMI_DEV_UnRegister(HdmiDev) CMPI_UnRegisterDevice(&HdmiDev)


#define HI_HDMI_UserCopy(file, cmd, arg, HdmiIoctl) CMPI_UserCopy(file, cmd, arg, HdmiIoctl)

#else


#endif

#endif // __DRV_HDMI_PLATFORM__
