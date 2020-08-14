/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_stat_ioctl.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2008/12/17
  Description   :
  History       :
  1.Date        : 2008/12/17
    Author      : z42136
    Modification: Created file

******************************************************************************/

#ifndef __DRV_STAT_IOCTL_H__
#define __DRV_STAT_IOCTL_H__

#include "hi_debug.h"
#include "hi_drv_stat.h"

typedef struct
{
    STAT_EVENT_E  enEvent;
    HI_U32        Value;
}STAT_EVENT_S;


#define UMAPC_CMPI_STAT_REGISTER            _IOWR (HI_ID_STAT, 101, HI_U32)
#define UMAPC_CMPI_STAT_UNREGISTER          _IOW (HI_ID_STAT,  102, HI_U32)

#define UMAPC_CMPI_STAT_RESETALL            _IO (HI_ID_STAT,   103)

#define UMAPC_CMPI_STAT_EVENT               _IOW (HI_ID_STAT,  104, STAT_EVENT_S)
#define UMAPC_CMPI_STAT_GETTICK             _IOR (HI_ID_STAT,  105, HI_U32)

#define UMAPC_CMPI_STAT_LD_EVENT            _IOW(HI_ID_STAT, 106, HI_LD_Event_S)

#endif /* __DRV_STAT_IOCTL_H__ */

