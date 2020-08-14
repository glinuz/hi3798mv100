/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name        : jpeg_hdec_rwreg.c
Version            : Initial Draft
Author            : y00181162
Created            : 2014/06/20
Description        : write data to register and read data from register
                  CNcomment: 读写寄存器 CNend\n
Function List     :


History           :
Date                Author                Modification
2014/06/20            y00181162            Created file
******************************************************************************/

/*********************************add include here******************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jpeg_hdec_adp.h"
#include "jpeg_hdec_rwreg.h"
#include "hi_jpeg_reg.h"

/***************************** Macro Definition ******************************/


/******************** to see which include file we want to use***************/



/*************************** Structure Definition ****************************/



/********************** Global Variable declaration **************************/

/******************************* API forward declarations *******************/

/******************************* API realization *****************************/


/*****************************************************************************
* func            : JPEG_HDEC_WriteReg
* description    : writd data to register
                  CNcomment:  写寄存器                                    CNend\n
* param[in]     : pJpegRegVirAddr  CNcomment:  寄存器映射上来的虚拟地址 CNend\n
* param[in]     : s32PhyOff        CNcomment:  偏移地址                 CNend\n
* param[in]     : s32Val           CNcomment:  要写的寄存器的值         CNend\n
* retval        : NA
* others:        : NA
*****************************************************************************/
HI_VOID JPEG_HDEC_WriteReg(volatile HI_CHAR *pJpegRegVirAddr, HI_S32 s32PhyOff, HI_S32 s32Val)
{

        volatile HI_S32  *ps32Addr = NULL;

        /**
        ** the phycial offset address can not larger than register length
        ** CNcomment: 偏移地址不能大于寄存器长度 CNend\n
        **/
        if(s32PhyOff < JPGD_REG_LENGTH){
            ps32Addr  = (volatile HI_S32*)(pJpegRegVirAddr + s32PhyOff); /*lint !e826 ignore by y00181162, because this is needed */
            *ps32Addr = s32Val;
        }

}

/*****************************************************************************
* func            : JPEG_HDEC_ReadReg
* description    : read data from register
                  CNcomment:  读寄存器的值                                    CNend\n
* param[in]     : pJpegRegVirAddr    CNcomment:    寄存器映射上来的虚拟地址    CNend\n
* param[in]     : s32PhyOff         CNcomment:    偏移地址                    CNend\n
* retval        : HI_SUCCESS        CNcomment:    成功                        CNend\n
* retval        : HI_FAILURE        CNcomment:    失败                        CNend\n
* others:        : NA
*****************************************************************************/
HI_S32 JPEG_HDEC_ReadReg(const volatile HI_CHAR *pJpegRegVirAddr, HI_S32 s32PhyOff)
{

        return s32PhyOff < JPGD_REG_LENGTH ?  *(volatile int *)(pJpegRegVirAddr + s32PhyOff) : HI_FAILURE; /*lint !e826 ignore by y00181162, because this is needed */

}

/*****************************************************************************
* func            : JPEG_HDEC_CpyData2Reg
* description    : copy the mem data to register
                  CNcomment:  将buf中的值写到寄存器中                    CNend\n
* param[in]     : pJpegRegVirAddr  CNcomment:  寄存器映射上来的虚拟地址 CNend\n
* param[in]     : pInMem           CNcomment:  要输出的buffer地址        CNend\n
* param[in]     : s32PhyOff        CNcomment:  偏移地址                 CNend\n
* param[in]     : u32Bytes           CNcomment:  要写的字节数             CNend\n
* retval        : NA
* others:        : NA
*****************************************************************************/
HI_VOID JPEG_HDEC_CpyData2Reg(volatile HI_CHAR *pJpegRegVirAddr,const HI_VOID *pInMem,HI_S32 s32PhyOff,HI_U32 u32Bytes)
{


        HI_U32 u32Cnt = 0;

        for(u32Cnt = 0; u32Cnt < u32Bytes; u32Cnt += 4){
            *(volatile int *)(pJpegRegVirAddr + s32PhyOff + u32Cnt)=*(int *)((char*)pInMem + u32Cnt);/*lint !e826 ignore by y00181162, because this is needed */
        }

}

/*****************************************************************************
* func            : JPEG_HDEC_CpyData2Buf
* description    : copy the register data to mem
                  CNcomment:  将寄存器中的值写到用户buffer中                CNend\n
* param[in]     : pJpegRegVirAddr      CNcomment:  寄存器映射上来的虚拟地址    CNend\n
* param[in]     : s32PhyOff           CNcomment:  偏移地址              CNend\n
* param[in]     : u32Bytes              CNcomment:  要输出的字节数      CNend\n
* param[out]    : pOutMem              CNcomment:  输出buffer地址      CNend\n
* retval        : HI_SUCCESS          CNcomment:  成功                  CNend\n
* retval        : HI_FAILURE          CNcomment:  失败                  CNend\n
* others:        : NA
*****************************************************************************/
HI_S32 JPEG_HDEC_CpyData2Buf(const volatile HI_CHAR *pJpegRegVirAddr,const HI_S32 s32PhyOff,const HI_U32 u32Bytes,HI_VOID *pOutMem)
{

        HI_U32 u32Cnt = 0;

        for(u32Cnt = 0; u32Cnt < u32Bytes; u32Cnt += 4){
            *(volatile int *)((char*)pOutMem + u32Cnt)=*(int *)(pJpegRegVirAddr + s32PhyOff + u32Cnt);/*lint !e826 ignore by y00181162, because this is needed */
        }
        return HI_SUCCESS;
}
