/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name        : jpeg_drv_proc.h
Version            : Initial Draft
Author            :
Created            : 2015/02/02
Description        : the proc information define in this file
                  CNcomment: proc 相关的信息都在这里 CNend\n
Function List     :


History           :
Date                Author                Modification
2015/02/02            y00181162              Created file
******************************************************************************/

#ifndef __JPEG_DRV_PROC_H__
#define __JPEG_DRV_PROC_H__


#ifndef HI_BUILD_IN_BOOT

/*********************************add include here******************************/
#include "hi_jpeg_config.h"


#ifdef CONFIG_JPEG_PROC_ENABLE

#include <linux/seq_file.h>
#include "hi_drv_jpeg.h"


/*****************************************************************************/

#ifdef __cplusplus
#if __cplusplus
   extern "C" {
#endif
#endif /* __cplusplus */


   /***************************** Macro Definition ******************************/

   #define PROC_JPEG_ENTRY_NAME            "jpeg"

    /*************************** Structure Definition ****************************/


    /********************** Global Variable declaration **************************/


    /******************************* API declaration *****************************/

    /*****************************************************************************
    * Function     : JPEG_Proc_GetStruct
    * Description  : get the proc struct information
    * param[in]    : ppstProcInfo
    * retval       : NA
    *****************************************************************************/
    HI_VOID JPEG_Proc_GetStruct(HI_JPEG_PROC_INFO_S **ppstProcInfo);


    /*****************************************************************************
    * Function     : JPEG_Proc_init
    * Description  :
    * param[in]    : NA
    * retval       : NA
    *****************************************************************************/
    HI_VOID JPEG_Proc_init(HI_VOID);


    /*****************************************************************************
    * Function     : JPEG_Proc_Cleanup
    * Description  :
    * param[in]    : NA
    * retval       : NA
    *****************************************************************************/
    HI_VOID JPEG_Proc_Cleanup(HI_VOID);


    /*****************************************************************************
    * Function     : JPEG_Proc_IsOpen
    * Description  :
    * param[in]    : NA
    * retval       : NA
    *****************************************************************************/
    HI_BOOL JPEG_Proc_IsOpen(HI_VOID);


    /*****************************************************************************
    * Function     : JPEG_Get_Proc_Status
    * Description  :
    * param[in]    : pbProcStatus
    * retval       : NA
    *****************************************************************************/
    HI_VOID JPEG_Get_Proc_Status(HI_BOOL* pbProcStatus);


    /****************************************************************************/



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif/**CONFIG_JPEG_PROC_ENABLE**/

#endif/**HI_BUILD_IN_BOOT**/

#endif /** use the proc information -DCONFIG_JPEG_PROC_ENABLE **/
