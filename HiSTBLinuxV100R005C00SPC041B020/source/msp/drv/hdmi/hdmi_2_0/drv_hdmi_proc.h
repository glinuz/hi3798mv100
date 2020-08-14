/******************************************************************************

  Copyright (C), 2014-2024, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_hdmi_proc.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2015/02/01
  Description   :
  History       :
  Date          : 2015/02/01
  Author        : t00273561
  Modification  :
*******************************************************************************/
#ifndef __DRV_HDMI_PROC_H__
#define __DRV_HDMI_PROC_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
#include "hi_type.h"
#include "drv_hdmi_platform.h"

HI_S32 DRV_HDMI_ReadProc(struct seq_file *pFile, HI_VOID *pArg);

HI_S32 DRV_HDMI_VOReadProc(struct seq_file *pFile, HI_VOID *pArg);

HI_S32 DRV_HDMI_AOReadProc(struct seq_file *pFile, HI_VOID *pArg);

HI_S32 DRV_HDMI_SinkReadProc(struct seq_file *pFile, HI_VOID *pArg);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif


#endif  /* __DRV_HDMI_PROC_H__ */  


