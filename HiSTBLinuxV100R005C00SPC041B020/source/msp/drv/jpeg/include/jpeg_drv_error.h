/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name        : jpeg_drv_error.h
Version            : Initial Draft
Author            :
Created            : 2016/02/02
Description        :
Function List     :


History           :
Date                Author                Modification
2016/02/02            y00181162              Created file
******************************************************************************/
#ifndef __JPEG_DRV_ERROR_H__
#define __JPEG_DRV_ERROR_H__


/*********************************add include here******************************/
#ifndef HI_BUILD_IN_BOOT
    #include <linux/types.h>
    #include <linux/fs.h>
    #include <linux/interrupt.h>
    #include <linux/ioctl.h>
    #include <linux/delay.h>
    #include <linux/errno.h>
    #include <linux/mm.h>
    #include <linux/fcntl.h>
    #include <linux/slab.h>
    #include <asm/atomic.h>
    #include <asm/io.h>
#endif


#include "hi_type.h"

/*****************************************************************************/


#ifdef __cplusplus
#if __cplusplus
   extern "C" {
#endif
#endif /* __cplusplus */



         /***************************** Macro Definition ******************************/
         /** \addtogroup      JPEG */
         /** @{ */    /** <!--[JPEG]*/
        #define JPEG_DEF_ERR(errid)  ((HI_S32)((0xB0002000) | ((HI_U32)errid)))

         /** @} */    /** <!-- ==== Macro Definition end ==== */

         /*************************** Enum Definition ****************************/
        /** \addtogroup   JPEG */
        /** @{ */    /** <!--[JPEG]*/
        typedef enum tagJPEG_ERR_E
        {
            JPEG_ERR_PARA_NULL    = 0,
            JPEG_ERR_NOT_JPEG,
            JPEG_ERR_UNSUPPORT,
            JPEG_ERR_SOFN_EXIT,
            JPEG_ERR_SOS_EXIT,
            JPEG_ERR_PARSE_DQT,
            JPEG_ERR_PARSE_DHT,
            JPEG_ERR_DATA_SUPPORT,
            JPEG_ERR_SETPARA,
            JPEG_ERR_DRI,
            JPEG_ERR_BUTT
        } JPEG_ERR_E;

        /** CNcomment:²ÎÊý¿ÕÖ¸Õë£¬´íÎóÂëÎª 0xB0002000 */
        #define HI_ERR_JPEG_PARA_NULL       JPEG_DEF_ERR(JPEG_ERR_PARA_NULL)

        /** CNcomment:²»ÊÇjpegÍ¼Æ¬£¬´íÎóÂëÎª 0xB0002001 */
        #define HI_ERR_JPEG_NOT_JPEG        JPEG_DEF_ERR(JPEG_ERR_NOT_JPEG)

        /** CNcomment:²»Ö§³Ö£¬´íÎóÂëÎª 0xB0002002 */
        #define HI_ERR_JPEG_UNSUPPORT       JPEG_DEF_ERR(JPEG_ERR_UNSUPPORT)

        /** CNcomment:½âÎöÉ¨Ãè²ãÊ§°Ü£¬´íÎóÂëÎª 0xB0002003 */
        #define HI_ERR_JPEG_SOFN_EXIT       JPEG_DEF_ERR(JPEG_ERR_SOFN_EXIT)

        /** CNcomment:½âÎöÖ¡²ãÊ§°Ü£¬  ´íÎóÂëÎª 0xB0002004 */
        #define HI_ERR_JPEG_SOS_EXIT       JPEG_DEF_ERR(JPEG_ERR_SOS_EXIT)

        /** CNcomment:½âÎöÁ¿»¯±íÊ§°Ü£¬  ´íÎóÂëÎª 0xB0002005 */
        #define HI_ERR_JPEG_PARSE_DQT       JPEG_DEF_ERR(JPEG_ERR_PARSE_DQT)

        /** CNcomment:½âÎö¹þ·òÂü±í£¬  ´íÎóÂëÎª 0xB0002006 */
        #define HI_ERR_JPEG_PARSE_DHT       JPEG_DEF_ERR(JPEG_ERR_PARSE_DHT)

        /** CNcomment:Í¼Æ¬Ã»ÓÐÍêÕûµÄÂëÁ÷Êý¾Ý£¬  ´íÎóÂëÎª 0xB0002007 */
        #define HI_ERR_JPEG_DATA_SUPPORT    JPEG_DEF_ERR(JPEG_ERR_DATA_SUPPORT)

        /** CNcomment:ÉèÖÃ²ÎÊý´íÎó£¬  ´íÎóÂëÎª 0xB0002008 */
        #define HI_ERR_JPEG_SERPARA          JPEG_DEF_ERR(JPEG_ERR_SETPARA)

        /** CNcomment:DRI½âÎö´íÎó£¬  ´íÎóÂëÎª 0xB0002009 */
        #define HI_ERR_JPEG_DRI              JPEG_DEF_ERR(JPEG_ERR_DRI)

        /** @} */  /** <!-- ==== enum Definition end ==== */

        /*************************** Structure Definition ****************************/

        /** \addtogroup   JPEG */
        /** @{ */    /** <!--[JPEG]*/


        /** @} */  /** <!-- ==== Structure Definition End ==== */

        /********************** Global Variable declaration **************************/

        /******************************* API declaration *****************************/

        /** \addtogroup   JPEG */
        /** @{ */    /** <!--[JPEG]*/

        /** @} */  /** <!-- ==== API Declaration End ==== */

        /****************************************************************************/



#ifdef __cplusplus

#if __cplusplus

    }
#endif
#endif /* __cplusplus */

#endif /* __JPEG_DRV_ERROR_H__*/
