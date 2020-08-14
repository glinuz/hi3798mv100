/******************************************************************************

  Copyright (C), 2013-2014, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : hi_drv_memdev.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/07/01
  Description   : Support /dev/hi_mem device.
  History       :
  1.Date        : 2013/07/01
    Author      : l00185424
    Modification: Created file

******************************************************************************/

#ifndef __HI_DRV_MEMDEV_H__
#define __HI_DRV_MEMDEV_H__

/******************************* Include Files *******************************/

/* add include here */
#include "hi_type.h"
#include "hi_module.h"
#include "hi_debug.h"
#include "hi_drv_struct.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

/***************************** Macro Definition ******************************/

#define UMAP_DEVNAME_MEMDEV           UMAP_NAME HI_MOD_MEM

#define HI_FATAL_MEMDEV(fmt...)   HI_FATAL_PRINT(HI_ID_MEMDEV, fmt)
#define HI_ERR_MEMDEV(fmt...)     HI_ERR_PRINT(HI_ID_MEMDEV, fmt)
#define HI_WARN_MEMDEV(fmt...)    HI_WARN_PRINT(HI_ID_MEMDEV, fmt)
#define HI_INFO_MEMDEV(fmt...)    HI_INFO_PRINT(HI_ID_MEMDEV, fmt)


/*************************** Structure Definition ****************************/

/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/

HI_S32 MEMDEV_DRV_ModInit(HI_VOID);
HI_S32 MEMDEV_DRV_ModExit(HI_VOID);


#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /* __HI_DRV_MEMDEV_H__ */
