/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_pwm_ioctl.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/10/25
  Description   :
  History       :
  1.Date        : 2013/10/25
    Author      : h00183514
    Modification: Created file

******************************************************************************/
#ifndef __DRV_PWM_IOCTL_H__
#define __DRV_PWM_IOCTL_H__

#include "hi_type.h"
#include "hi_debug.h"

#include "hi_unf_pwm.h"

#ifdef __cplusplus
#if __cplusplus
    extern "C"{
#endif
#endif

#define HI_FATAL_PWM(fmt...) \
            HI_FATAL_PRINT(HI_ID_PWM, fmt)

#define HI_ERR_PWM(fmt...) \
            HI_ERR_PRINT(HI_ID_PWM, fmt)

#define HI_WARN_PWM(fmt...) \
            HI_WARN_PRINT(HI_ID_PWM, fmt)

#define HI_INFO_PWM(fmt...) \
            HI_INFO_PRINT(HI_ID_PWM, fmt)

typedef struct tagPWM_REG_S
{
    HI_U32      u32Cfg;
    HI_U32      u32Ctrl;
    HI_U32      u32State;
}PWM_REG_S;

typedef struct tagPWM_ATTR_CMD_PARA_S
{
    HI_UNF_PWM_E        enPwm;
    HI_UNF_PWM_ATTR_S   stPwmAttr;
}PWM_ATTR_CMD_PARA_S;

typedef struct tagPWM_SIGNAL_CMD_PARA_S
{
    HI_UNF_PWM_E        enPwm;
    HI_U32              u32CarrierSigDurationUs;
    HI_U32              u32LowLevelSigDurationUs;
}PWM_SIGNAL_CMD_PARA_S;

typedef enum hiIOC_PWM_E
{
    IOC_PWM_GETATTR,
    IOC_PWM_SETATTR,
    IOC_PWM_SENDSIGNAL,

    IOC_PWM_BUTT
}IOC_PWM_E;


#define CMD_PWM_GETATTR              _IOWR(HI_ID_PWM, IOC_PWM_GETATTR, PWM_ATTR_CMD_PARA_S)
#define CMD_PWM_SETATTR              _IOWR(HI_ID_PWM, IOC_PWM_SETATTR, PWM_ATTR_CMD_PARA_S)
#define CMD_PWM_SENDSIGNAL           _IOWR(HI_ID_PWM, IOC_PWM_SENDSIGNAL, PWM_SIGNAL_CMD_PARA_S)



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif



