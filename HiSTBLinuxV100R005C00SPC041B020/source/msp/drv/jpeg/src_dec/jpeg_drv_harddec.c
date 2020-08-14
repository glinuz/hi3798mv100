/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name        : jpeg_drv_hardedec.c
Version          : Initial Draft
Author           : y00181162
Created          : 2016/05/09
Description      :
Function List    :


History          :
Date                Author                Modification
2016/05/09          y00181162            Created file
******************************************************************************/


/***************************** add include here*********************************/
#ifndef HI_BUILD_IN_BOOT
    #include <linux/module.h>
    #include <linux/init.h>
    #include <linux/moduleparam.h>
    #include <linux/sched.h>
    #include <linux/kernel.h>
    #include <linux/types.h>
    #include <linux/fs.h>
    #include <linux/miscdevice.h>
    #include <linux/interrupt.h>
    #include <linux/ioport.h>
    #include <linux/ioctl.h>
    #include <linux/delay.h>
    #include <linux/device.h>
    #include <linux/errno.h>
    #include <linux/spinlock.h>
    #include <linux/mm.h>
    #include <linux/stddef.h>
    #include <linux/fcntl.h>
    #include <linux/slab.h>
    #include <asm/atomic.h>
    #include <asm/bitops.h>
    #include <asm/io.h>
    #include <asm/uaccess.h>
    #include <asm/pgtable.h>
    #include "jpeg_drv_suspend.h"
    #include "jpeg_drv_osr.h"
    #include "jpeg_drv_mem.h"
#endif



#include "jpeg_drv_dec.h"
#include "hi_jpeg_reg.h"
#include "jpeg_drv_hal.h"
#include "jpeg_drv_error.h"
#include "jpeg_drv_parse.h"

/***************************** Macro Definition ********************************/

/***************************** Structure Definition ****************************/

/***************************** Global Variable declaration *********************/


/***************************** API forward declarations ************************/
static inline HI_S32 JPEG_DRV_StartDec(HI_VOID);

#ifdef HI_BUILD_IN_BOOT
static inline HI_VOID JPEG_DRV_QuerytIntStatus(JPG_INTTYPE_E *pIntType);
#endif

extern HI_S32 DRV_JPEG_SetPara(HI_JPEG_DECINFO_S *pstDecInfo,IMAG_INFO_S *pstImgInfo);
extern HI_S32 DRV_JPEG_SendStream(HI_U32 DecHandle,HI_S32 *ps32Offset,HI_BOOL *pbReachEOF,IMAG_INFO_S *pstImgInfo);

/***************************** API realization *********************************/

HI_S32 JPEG_DRV_HardDec(HI_JPEG_DECINFO_S *stDecInfo,IMAG_INFO_S *pstImgInfo)
{
    HI_S32 s32Ret = HI_FAILURE;
#ifdef CONFIG_JPEG_OUTPUT_LUPIXSUM
    HI_U32 u32RegistLuaPixSum0 = 0;
    HI_U64 u64RegistLuaPixSum1 = 0;
#endif

    if ( (NULL == stDecInfo) || (NULL == pstImgInfo)){
        JPEG_TRACE("the dec info is null\n");
        return HI_ERR_JPEG_PARA_NULL;
    }

    if (0 == stDecInfo->stInMsg.u32SavePhy){
        JPEG_TRACE("the input ddr is null\n");
        return HI_ERR_JPEG_PARA_NULL;
    }

    if (0 == stDecInfo->stOutMsg.u32OutPhy[0]){
        JPEG_TRACE("the output ddr is null\n");
        return HI_ERR_JPEG_PARA_NULL;
    }

    s32Ret = DRV_JPEG_SetPara(stDecInfo,pstImgInfo);
    if (HI_SUCCESS != s32Ret){
        JPEG_TRACE("DRV_JPEG_SetPara failure\n");
        return s32Ret;
    }


    if (HI_TRUE == stDecInfo->stInMsg.bUserPhyMem){
        s32Ret = JPEG_DRV_StartDec();
    }
    if (HI_SUCCESS != s32Ret){
        JPEG_TRACE("start decode failure\n");
        return s32Ret;
    }

#ifdef CONFIG_JPEG_OUTPUT_LUPIXSUM
    if (HI_TRUE == stDecInfo->stOutMsg.bLuPixSum){
        u32RegistLuaPixSum0  = (HI_U32)jpeg_reg_read(JPGD_REG_LPIXSUM0);
        u64RegistLuaPixSum1  = (HI_U64)(jpeg_reg_read(JPGD_REG_LPIXSUM1) & 0xf);
        pstImgInfo->u64LuPixValue = (HI_U64)((u64RegistLuaPixSum1 << 32) | u32RegistLuaPixSum0);
        stDecInfo->stOutMsg.u64LuPixValue = pstImgInfo->u64LuPixValue;
    }
#endif

    return HI_SUCCESS;
}



static inline HI_S32 JPEG_DRV_StartDec(HI_VOID)
{
#ifndef HI_BUILD_IN_BOOT
     HI_S32 s32Ret = HI_SUCCESS;
#endif
    JPG_GETINTTYPE_S IntInfo = {0};

    jpeg_reg_write(JPGD_REG_RESUME, 0x2);

#ifdef CONFIG_JPEG_4KDDR_DISABLE
    jpeg_reg_write(JPGD_REG_START, 0x5);
#else
    jpeg_reg_write(JPGD_REG_START, 0x1);
#endif

#ifndef HI_BUILD_IN_BOOT
     IntInfo.IntType = JPG_INTTYPE_NONE;
     IntInfo.TimeOut = 5000;

     s32Ret = JPEG_DRV_GetIntStatusValue(&IntInfo);
     if (HI_FAILURE == s32Ret){
         return HI_FAILURE;
     }

     if (JPG_INTTYPE_FINISH == IntInfo.IntType){
        return HI_SUCCESS;
     }else if (JPG_INTTYPE_CONTINUE == IntInfo.IntType){
        JPEG_TRACE("dec continue\n");
        return HI_FAILURE;
     }else{
        JPEG_TRACE("dec failure\n");
        return HI_FAILURE;
     }
 #else
     while(1){
        IntInfo.IntType = JPG_INTTYPE_NONE;
        JPEG_DRV_QuerytIntStatus(&(IntInfo.IntType));
        if (JPG_INTTYPE_FINISH == IntInfo.IntType){
            return HI_SUCCESS;
         }else if (JPG_INTTYPE_CONTINUE == IntInfo.IntType || JPG_INTTYPE_ERROR == IntInfo.IntType){
            return HI_FAILURE;
         }else{
            continue;
         }
     }
 #endif

     return HI_SUCCESS;
}


#ifdef HI_BUILD_IN_BOOT
static inline HI_VOID JPEG_DRV_QuerytIntStatus(JPG_INTTYPE_E *pIntType)
{
    HI_U32 IntType = 0;

    DRV_JPEG_GetIntStatus(&IntType);
    DRV_JPEG_ClearIntStatus(IntType);

    if (IntType & 0x1){
        *pIntType = JPG_INTTYPE_FINISH;
    }else if (IntType & 0x2){
        *pIntType = JPG_INTTYPE_ERROR;
    }else if (IntType & 0x4){
        *pIntType = JPG_INTTYPE_CONTINUE;
    }else{
        *pIntType = JPG_INTTYPE_NONE;
    }
    return;
}
#endif
