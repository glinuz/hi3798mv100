/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name        : jpeg_hdec_rwreg.h
Version            : Initial Draft
Author            :
Created            : 2014/06/20
Description        : the write and read about the register realize in this file
                  CNcomment: 读写寄存器操作在该文件中实现 CNend\n
Function List     :


History           :
Date                Author                Modification
2014/06/20            y00181162              Created file
******************************************************************************/

#ifndef __JPEG_HDEC_RWREG_H__
#define __JPEG_HDEC_RWREG_H__


/*********************************add include here******************************/
#include  "hi_jpeg_config.h"
#include  "hi_type.h"


/*****************************************************************************/


#ifdef __cplusplus
#if __cplusplus
   extern "C" {
#endif
#endif /* __cplusplus */



    /***************************** Macro Definition ******************************/
    /** \addtogroup      JPEG RWREG MACRO */
    /** @{ */  /** <!-- 【JPEG RWREG MACRO】 */


     /** @} */    /*! <!-- Macro Definition end */


     /*************************** Enum Definition ****************************/

    /** \addtogroup      JPEG RWREG ENUM */
    /** @{ */  /** <!-- 【JPEG RWREG ENUM】 */



    /** @} */  /*! <!-- enum Definition end */

    /*************************** Structure Definition ****************************/

    /** \addtogroup      JPEG RWREG STRUCTURE */
    /** @{ */  /** <!-- 【JPEG RWREG STRUCTURE】 */

    /** @} */  /*! <!-- Structure Definition end */


    /********************** Global Variable declaration **************************/

    /******************************* API declaration *****************************/

    /** \addtogroup      JPEG RWREG API */
    /** @{ */  /** <!-- 【JPEG RWREG API】 */


    /*****************************************************************************
    * func            : JPEG_HDEC_WriteReg
    * description    : writd data to register
                      CNcomment:  写寄存器                                    CNend\n
    * param[in]     : pJpegRegVirAddr  CNcomment:  寄存器映射上来的虚拟地址    CNend\n
    * param[in]     : s32PhyOff        CNcomment:  偏移地址                 CNend\n
    * param[in]     : s32Val           CNcomment:  要写的寄存器的值         CNend\n
    * retval        : NA
    * others:        : NA
    *****************************************************************************/
    HI_VOID JPEG_HDEC_WriteReg(volatile HI_CHAR *pJpegRegVirAddr, const HI_S32 s32PhyOff, const HI_S32 s32Val);

    /*****************************************************************************
    * func            : JPEG_HDEC_ReadReg
    * description    : read data from register
                      CNcomment:  读寄存器的值                                    CNend\n
    * param[out]    : pJpegRegVirAddr   CNcomment:  寄存器映射上来的虚拟地址    CNend\n
    * param[in]     : s32PhyOff         CNcomment:  偏移地址                    CNend\n
    * retval        : HI_SUCCESS        CNcomment:  成功                        CNend\n
    * retval        : HI_FAILURE        CNcomment:  失败                        CNend\n
    * others:        : NA
    *****************************************************************************/
    HI_S32 JPEG_HDEC_ReadReg(const volatile HI_CHAR *pJpegRegVirAddr, const HI_S32 s32PhyOff);

    /*****************************************************************************
    * func            : JPEG_HDEC_CpyData2Reg
    * description    : copy the mem data to register
                      CNcomment:  将buf中的值写到寄存器中                    CNend\n
    * param[in]     : pJpegRegVirAddr  CNcomment:  寄存器映射上来的虚拟地址    CNend\n
    * param[in]     : pInMem           CNcomment:  要输出的buffer地址        CNend\n
    * param[in]     : s32PhyOff        CNcomment:  偏移地址                    CNend\n
    * param[in]     : u32Bytes         CNcomment:  要写的字节数              CNend\n
    * retval        : NA
    * others:        : NA
    *****************************************************************************/
    HI_VOID JPEG_HDEC_CpyData2Reg(volatile HI_CHAR *pJpegRegVirAddr,const HI_VOID *pInMem,const HI_S32 s32PhyOff,const HI_U32 u32Bytes);


    /*****************************************************************************
    * func            : JPEG_HDEC_CpyData2Buf
    * description    : copy the register data to mem
                      CNcomment:  将寄存器中的值写到用户buffer中                CNend\n
    * param[in]     : pJpegRegVirAddr     CNcomment:  寄存器映射上来的虚拟地址    CNend\n
    * param[in]     : s32PhyOff           CNcomment:  偏移地址              CNend\n
    * param[in]     : u32Bytes            CNcomment:  要输出的字节数      CNend\n
    * param[out]    : pOutMem             CNcomment:  输出buffer地址      CNend\n
    * retval        : HI_SUCCESS          CNcomment:  成功                  CNend\n
    * retval        : HI_FAILURE          CNcomment:  失败                  CNend\n
    * others:        : NA
    *****************************************************************************/
    HI_S32 JPEG_HDEC_CpyData2Buf(const volatile HI_CHAR *pJpegRegVirAddr,const HI_S32 s32PhyOff,const HI_U32 u32Bytes,HI_VOID *pOutMem);


    /** @} */  /*! <!-- API declaration end */

    /****************************************************************************/



#ifdef __cplusplus

#if __cplusplus

}
#endif
#endif /* __cplusplus */

#endif /* __JPEG_HDEC_RWREG_H__*/
