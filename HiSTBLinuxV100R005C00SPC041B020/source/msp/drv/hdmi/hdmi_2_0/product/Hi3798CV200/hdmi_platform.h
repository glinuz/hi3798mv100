/******************************************************************************

  Copyright (C), 2010-2026, Hisilicon. Co., Ltd.

 ******************************************************************************
 * File Name     : hdmi_platform.h
 * Version       : 1.0
 * Author        : Hisilicon multimedia software group
 * Created       : 2015/6/27
 * Description   : 
 * History       :
 * Date          : 2015/6/27
 * Author        : l00232354
 * Modification  :
 *******************************************************************************
 */
#ifndef __HDMI_PLATFORM_H__
#define __HDMI_PLATFORM_H__
#include "hi_type.h"
#include "hi_module.h"
#include "hi_debug.h"
#ifndef HDMI_BUILD_IN_BOOT
#include "hi_drv_mem.h"
#include <asm/io.h>
#endif
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

/******************************************************************************/
/* Chip type                                                                  */
/******************************************************************************/
#define HDMI_CHIP_TYPE_HI3798CV200


/******************************************************************************/
/* Macro type for hdmi module                                                 */
/******************************************************************************/
#define HDMI_CTRL_BASE_ADDR 0xf8ce0000

#if defined(CONFIG_HDMI_STB_SDK)
#define HDMI_COLOR_BAR_MASK            0x70000000
#define HDMI_COLOR_BAR_UPDATE_MASK     0x00000001
#define HDMI_COLOR_BAR_BASE            0xf8ccc000
#define PERI_HDMITX_CTRL_ADDR          0xf8a208b0
#define PERI_HDMITX_AVCTTPT            0xf8a80038
#define HDMI_AUDIO_SOURCE_SPDIF        (0x1<<0)
#define HDMI_AUDIO_SOURCE_I2S          (0x1<<1)
#define HDMI_AUDIO_SOURCE_DSD          (0x1<<2)
#define HDMI_AUDIO_SOURCE_HBRA         (0x1<<3)
#define HDMI_AUDIO_SOURCE_PARALLEL     (0x1<<4)

#define HDMI_CRG_BASE_ADDR             0xf8a20000

#define HDMITX_CTRL_ASCLK_SEL          (0x1<<14) 
#define HDMITX_CTRL_SRST_REQ           (0x1<<9)
#define HDMITX_CTRL_BUS_SRST_REQ       (0x1<<8)
#define HDMITX_CTRL_ID_CKSEL           (0x1<<6)
#define HDMITX_CTRL_OSCLK_SEL          (0x1<<5)  
#define HDMITX_CTRL_PIXELNX_CKSEL      (0x1<<4)
#elif defined(CONFIG_HDMI_BVT_SDK)
#define PERI_HDMITX_CTRL_ADDR          0x12120150
#define HDMI_AUDIO_SOURCE_SPDIF 	   (0x1<<6)
#define HDMI_AUDIO_SOURCE_I2S 		   (0x1<<7)
#define HDMI_AUDIO_SOURCE_DSD 		   (0x1<<8)
#define HDMI_AUDIO_SOURCE_HBRA 		   (0x1<<9)
#define HDMI_AUDIO_SOURCE_PARALLEL 	   (0x1<<10)

#define HDMI_CRG_BASE_ADDR  			0x1204003c
#define HDMITX_CTRL_PIXELNX_CKSEL		(0x1<<28)
#define HDMITX_CTRL_ID_CKSEL			(0x1<<24)
#define HDMITX_CTRL_ASCLK_SEL			(0x1<<18)
#define HDMITX_CTRL_OSCLK_SEL			(0x1<<17)
#define HDMITX_CTRL_SRST_REQ			(0x1<<13)
#define HDMITX_CTRL_BUS_SRST_REQ		(0x1<<12)
#define HDMITX_CTRL_PIXELNX_CKEN		(0x1<<8) 
#define HDMITX_CTRL_XCLK_CKEN  		    (0x1<<7)  
#define HDMITX_CTRL_PIXEL_CKEN			(0x1<<6) 
#define HDMITX_CTRL_AS_CKEN			    (0x1<<5)
#define HDMITX_CTRL_OS_CKEN			    (0x1<<4)
#define HDMITX_CTRL_MHL_CKEN			(0x1<<3)
#define HDMITX_CTRL_ID_CKEN			    (0x1<<2)
#define HDMITX_CTRL_CEC_CKEN			(0x1<<1)
#define HDMITX_CTRL_BUS_CKEN			(0x1<<0)
#define HDMI_COLOR_BAR_MASK             0x70000000
#define HDMI_COLOR_BAR_UPDATE_MASK      0x00000001
#define HDMI_COLOR_BAR_BASE             0x1302c000
#endif

#ifndef HDMI_BUILD_IN_BOOT
#define HDMI_VMALLOC(size)     HI_VMALLOC(HI_ID_HDMI, size, GFP_KERNEL)
#define HDMI_KMALLOC(size)     HI_KMALLOC(HI_ID_HDMI, size, GFP_KERNEL)

#define HDMI_KFREE(viraddr)    HI_KFREE(HI_ID_HDMI, viraddr)
#define HDMI_VFREE(viraddr)    HI_VFREE(HI_ID_HDMI, viraddr)

#define HDMI_IO_MAP(phys_addr, len) (ioremap_nocache(phys_addr, len))
#define HDMI_IO_UNMAP(vitural_addr) (iounmap(vitural_addr))

#else

#define HDMI_VMALLOC(size)     vmalloc(size, GFP_KERNEL)
#define HDMI_KMALLOC(size)     kmalloc(size, GFP_KERNEL)

#define HDMI_KFREE(viraddr)    kfree(viraddr)
#define HDMI_VFREE(viraddr)    vfree(viraddr)
#define HDMI_IO_MAP(phys_addr, len) (phys_addr)
#define HDMI_IO_UNMAP(virt_addr) 

#endif
/******************************************************************************/
/* Data Structures                                                            */
/******************************************************************************/

/******************************************************************************/
/* Gloabl Data                                                                */
/******************************************************************************/
 
/******************************************************************************/
/* Public Interface                                                           */
/******************************************************************************/
#define HDMI_MMZ_BUFFER_S       MMZ_BUFFER_S
#define DEVNAME_HDMI            UMAP_DEVNAME_HDMI

#ifdef CONFIG_COMPAT
#define DECLARE_HDMI_NODE(hdmi_name, hdmi_open, hdmi_release, hdmi_mmap, hdmi_ioctl, hdmi_suspend, hdmi_resume) \
static struct file_operations s_stHdmiFileOps =\
{\
    .owner   = THIS_MODULE,      \
    .unlocked_ioctl = hdmi_ioctl,\
    .compat_ioctl = hdmi_ioctl,  \
    .open    = hdmi_open,        \
    .release = hdmi_release,     \
    .mmap    = hdmi_mmap         \
};\
static PM_BASEOPS_S s_stHdmiDrvOps = \
{\
    .suspend      = hdmi_suspend,\
    .resume       = hdmi_resume  \
};\
static UMAP_DEVICE_S s_stHdmiDev = \
{\
    .minor      = UMAP_MIN_MINOR_HDMI,\
    .owner      = THIS_MODULE,\
    .fops       = &s_stHdmiFileOps,\
    .drvops     = &s_stHdmiDrvOps \
};
#else
#define DECLARE_HDMI_NODE(hdmi_name, hdmi_open, hdmi_release, hdmi_mmap, hdmi_ioctl, hdmi_suspend, hdmi_resume) \
static struct file_operations s_stHdmiFileOps =\
{\
    .owner   = THIS_MODULE,      \
    .unlocked_ioctl = hdmi_ioctl,\
    .open    = hdmi_open,        \
    .release = hdmi_release,     \
    .mmap    = hdmi_mmap         \
};\
static PM_BASEOPS_S s_stHdmiDrvOps = \
{\
    .suspend      = hdmi_suspend,\
    .resume       = hdmi_resume  \
};\
static UMAP_DEVICE_S s_stHdmiDev = \
{\
    .minor      = UMAP_MIN_MINOR_HDMI,\
    .owner      = THIS_MODULE,\
    .fops       = &s_stHdmiFileOps,\
    .drvops     = &s_stHdmiDrvOps \
};
#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

