/******************************************************************************

  Copyright (C), 2013-2014, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : mpi_memdev.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/07/01
  Description   : Support /dev/hi_mem device.
  History       :
  1.Date        : 2013/07/01
    Author      : l00185424
    Modification: Created file

******************************************************************************/

#ifndef __MPI_MEMDEV_H__
#define __MPI_MEMDEV_H__

/******************************* Include Files *******************************/

/* add include here */
#include "hi_type.h"
#include "hi_module.h"
#include "hi_debug.h"
#include "hi_drv_memdev.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

/***************************** Macro Definition ******************************/

/*************************** Structure Definition ****************************/

/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/

HI_S32 MPI_MEMDEV_Init(HI_VOID);
HI_S32 MPI_MEMDEV_DeInit(HI_VOID);
HI_S32 MPI_MEMDEV_WriteRegister(HI_U32 u32RegAddr, HI_U32 u32Value);
HI_S32 MPI_MEMDEV_ReadRegister(HI_U32 u32RegAddr, HI_U32 *pu32Value);
HI_S32 MPI_MEMDEV_MapRegister(HI_U32 u32RegAddr, HI_U32 u32Length, HI_VOID *pVirAddr);
HI_S32 MPI_MEMDEV_UnmapRegister(HI_VOID *pVirAddr);


#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /* __MPI_MEMDEV_H__ */

