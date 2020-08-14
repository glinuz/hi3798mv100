/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_otp_common.h
  Version       : Initial Draft
  Author        : Hisilicon hisecurity team
  Created       : 
  Last Modified :
  Description   : 
  Function List :
  History       :
******************************************************************************/
#ifndef __DRV_OTP_COMMON_H__
#define __DRV_OTP_COMMON_H__

#include "hi_common.h"
#include "hi_type.h"
#ifdef SDK_OTP_ARCH_VERSION_V3
#include "drv_otp_ioctl.h"
#else
#include "priv_otp.h"
#endif

typedef enum
{
    OTP_VERSION_100,
    OTP_VERSION_200,
    OTP_VERSION_BUTT
}OTP_VERSION_E;

HI_VOID otp_write_reg(HI_U32 addr,  HI_U32 val);
HI_U32  otp_read_reg(HI_U32 addr);
HI_VOID otp_wait(HI_U32 us);


#endif  /* __DRV_OTP_COMMON_H__ */

