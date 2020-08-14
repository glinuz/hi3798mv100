
#ifndef __BOOT_HDMI_INTF_H__
#define __BOOT_HDMI_INTF_H__
#include <uboot.h>
#include "hi_type.h"
#include "hi_debug.h"
#include "drv_hdmi_common.h"
#include "hdmi_hal.h"
#include "hi_drv_disp.h"

#define HDMI_WARN(format...)    HI_WARN_PRINT(HI_ID_HDMI, format)
#define HDMI_ERR(format...)     HI_ERR_PRINT(HI_ID_HDMI, format)
#define HDMI_FATAL(format...)   HI_FATAL_PRINT(HI_ID_HDMI, format)
#define HDMI_INFO(format...)    HI_INFO_PRINT(HI_ID_HDMI, format)

#define HDMI_MEMSET         memset
#define HDMI_MEMCPY         memcpy

#undef offsetof
#define offsetof(__type, __member) ((unsigned int) &((__type *)0)->__member)

/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:	the pointer to the member.
 * @type:	the type of the container struct this is embedded in.
 * @member:	the name of the member within the struct.
 *
 */
#undef container_of
#define container_of(ptr, type, member) ({			\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type,member) );})

typedef struct 
{
    HI_U32             u32HdmiDevId;
    HI_BOOL            bYUV420Enable;
    HI_BOOL            bEmiEnable;
    HDMI_TMDS_MODE_E   enTmdsMode;
    HDMI_ATTR_S        stAttr;
    HDMI_INFOFRAME_S   stInfoFrame;
    HDMI_HAL_S*        pstHdmiHal;
}HDMI_DEVICE_S;

#endif//__BOOT_HDMI_INTF_H__