/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_drv_cimax+.h
  Version       : Initial Draft
  Author        : advca
  Created       : 2013/04/12
  Description   : 
  History       :
  1.Date        : 2013/04/12
    Author      : z00213260
    Modification: Created file

******************************************************************************/

#ifndef __HI_DRV_CIMAXPLUS_H__
#define __HI_DRV_CIMAXPLUS_H__

#ifdef __cplusplus
extern "C" {
#endif


#define HI_FATAL_CIMAXPLUS(fmt...) \
    HI_FATAL_PRINT(HI_ID_CIMAXPLUS, fmt)

#define HI_ERR_CIMAXPLUS(fmt...) \
    HI_ERR_PRINT(HI_ID_CIMAXPLUS, fmt)

#define HI_WARN_CIMAXPLUS(fmt...) \
    HI_WARN_PRINT(HI_ID_CIMAXPLUS, fmt)

#define HI_INFO_CIMAXPLUS(fmt...) \
    HI_INFO_PRINT(HI_ID_CIMAXPLUS, fmt)

#define HI_KMALLOC_CIMAXPLUS(size, flags)      HI_KMALLOC(HI_ID_CIMAXPLUS, size, flags)
#define HI_KFREE_CIMAXPLUS(addr)               HI_KFREE(HI_ID_CIMAXPLUS, addr)
#define HI_VMALLOC_CIMAXPLUS(size)             HI_VMALLOC(HI_ID_CIMAXPLUS, size)
#define HI_VFREE_CIMAXPLUS(addr)               HI_VFREE(HI_ID_CIMAXPLUS, addr)

#define MODULE_CIMAXPLUS "HI_CIMAXPLUS"


#ifdef __cplusplus
}
#endif

#endif

