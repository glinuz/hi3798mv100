/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_drv_ci.h
  Version       : Initial Draft
  Author        : Hisilicon hisecurity team
  Created       : 
  Last Modified :
  Description   : 
  Function List :
  History       :
******************************************************************************/

#ifndef __HI_DRV_CI_H__
#define __HI_DRV_CI_H__
#include "hi_debug.h"

#ifdef __cplusplus
extern "C" {
#endif

#define HI_FATAL_CI(fmt...) \
    HI_FATAL_PRINT(HI_ID_CI, fmt)

#define HI_ERR_CI(fmt...) \
    HI_ERR_PRINT(HI_ID_CI, fmt)

#define HI_WARN_CI(fmt...) \
    HI_WARN_PRINT(HI_ID_CI, fmt)

#define HI_INFO_CI(fmt...) \
    HI_INFO_PRINT(HI_ID_CI, fmt)

#define CHECK_CIPORT_VALID(enCIPort) \
    { \
        if (enCIPort >= HI_UNF_CI_PORT_BUTT) \
        { \
            HI_ERR_CI("Invalid CI Port:%d.\n", enCIPort); \
            return HI_ERR_CI_INVALID_PARA; \
        } \
    }

#define CHECK_PCCD_VALID(enCardId) \
    { \
        if (enCardId >= HI_UNF_CI_PCCD_BUTT) \
        { \
            HI_ERR_CI("Invalid Card Id:%d.\n", enCardId); \
            return HI_ERR_CI_INVALID_PARA; \
        } \
    }

#define HI_KMALLOC_CI(size, flags)      HI_KMALLOC(HI_ID_CI, size, flags)
#define HI_KFREE_CI(addr)               HI_KFREE(HI_ID_CI, addr)
#define HI_VMALLOC_CI(size)             HI_VMALLOC(HI_ID_CI, size)
#define HI_VFREE_CI(addr)               HI_VFREE(HI_ID_CI, addr)

#define MODULE_CI "HI_CI"

#define CI_PCCD_IORESET_TIMEOUT (300)   /* 3s */
#define CI_PCCD_RW_TIMEOUT (30)         /* 300ms */
#define CI_PCCD_NEGBUFFER_TIMEOUT (300) /* 300ms */

#define CI_TIME_10MS (10)           /* 10ms */
#define CI_TIME_1MS (1)             /* 1ms */
#define CI_DELAY_AFTER_WRITE_US (5) /* us */

#define DATA_REG (0)
#define COM_STAT_REG (1)
#define SIZE_L_REG (2)
#define SIZE_H_REG (3)

#define CI_PCCD_MIN_BUFFERSIZE (16)
#define CI_PCCD_MAX_BUFFERSIZE (1024)


#ifdef __cplusplus
}
#endif
#endif

