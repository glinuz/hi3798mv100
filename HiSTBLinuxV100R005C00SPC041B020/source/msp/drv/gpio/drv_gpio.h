#ifndef __DRV_GPIO_H__
#define __DRV_GPIO_H__

#include "hi_type.h"
#include "hi_drv_gpio.h"

#ifdef __cplusplus
#if __cplusplus
       extern "C"{
#endif
#endif

HI_S32 DRV_GPIO_QueryInt (GPIO_INT_S * pGpioIntValue);
HI_S32 DRV_GPIO_Open(struct inode *inode, struct file *filp);
HI_S32 DRV_GPIO_Close(struct inode *inode, struct file *filp);
HI_S32 DRV_GPIO_SetOutputType(HI_U32 u32GpioNo, HI_UNF_GPIO_OUTPUTTYPE_E  enOutputType);
HI_S32 DRV_GPIO_GetOutputType(HI_U32 u32GpioNo, HI_UNF_GPIO_OUTPUTTYPE_E  *penOutputType);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
