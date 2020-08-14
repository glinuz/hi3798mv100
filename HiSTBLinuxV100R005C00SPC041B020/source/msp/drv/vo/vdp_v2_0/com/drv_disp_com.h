
/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : drv_disp_com.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/12/30
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#ifndef __DRV_DISP_COM_H__
#define __DRV_DISP_COM_H__

#include "hi_type.h"
#include "hi_drv_video.h"
#include "hi_drv_disp.h"
#include "drv_disp_version.h"
#include "drv_disp_osal.h"


#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */


/*CVBS  Video Buff Type */
#define  VIDEO_BUFF_CVBS_NONE            0
#define  VIDEO_BUFF_CVBS_FMS6141            1
#define  VIDEO_BUFF_CVBS_FMS6143a           2
#define  VIDEO_BUFF_CVBS_TT_FILTER           3
#define  VIDEO_BUFF_CVBS_TT_HIFONE           4

/*YPbPr  ideo Buff Type */
#define  VIDEO_BUFF_YPBPR_NONE          0
#define  VIDEO_BUFF_YPBPR_FMS6363		1
#define  VIDEO_BUFF_YPBPR_DIO2176       2
#define  VIDEO_BUFF_YPBPR_TT_FILTER      3
#define  VIDEO_BUFF_YPBPR_TT_HIFONE      4

/*DAC Type
    Only HISI_VOLTAGE   Support  DAC Detect
    Only SYNOPSYS   Need   Revise LumaNoLine By Gamma
*/

#if  (defined(CHIP_TYPE_hi3716cv200) || defined(CHIP_TYPE_hi3719cv100) || defined(CHIP_TYPE_hi3718cv100) ||  defined(CHIP_TYPE_hi3716mv400))

/*DAC type */
#define  DAC_TYPE_HISI_VOLTAGE
/* Video Buff Type!*/
#define  VIDEO_BUFF_CVBS     VIDEO_BUFF_CVBS_NONE
#define  VIDEO_BUFF_YPBPR  VIDEO_BUFF_YPBPR_NONE

#elif  (defined(CHIP_TYPE_hi3719mv100) || defined(CHIP_TYPE_hi3718mv100))

/*DAC type */
#define  DAC_TYPE_SYNOPSYS

#if defined(HI_VIDEO_BUFF_TYPE0)
    /*Custom Video Type  1*/
#define  VIDEO_BUFF_CVBS     VIDEO_BUFF_CVBS_FMS6143a
#define  VIDEO_BUFF_YPBPR  VIDEO_BUFF_YPBPR_FMS6363
#else
    /*Demo Video Type!*/
#define  VIDEO_BUFF_CVBS     VIDEO_BUFF_CVBS_FMS6141
#define  VIDEO_BUFF_YPBPR  VIDEO_BUFF_YPBPR_FMS6363
#endif

#elif  (defined(CHIP_TYPE_hi3798cv100) || defined(CHIP_TYPE_hi3796cv100))

/*DAC type */
#define   DAC_TYPE_SYNOPSYS_EC
/* Video Buff Type!*/
#define  VIDEO_BUFF_CVBS     VIDEO_BUFF_CVBS_FMS6141
#define  VIDEO_BUFF_YPBPR    VIDEO_BUFF_YPBPR_DIO2176


#elif  (defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100) )

/*DAC type */
#define   DAC_TYPE_HISI_TORNADO

/* Video Buff Type !*/
#define  VIDEO_BUFF_CVBS       VIDEO_BUFF_CVBS_TT_FILTER
#define  VIDEO_BUFF_YPBPR    VIDEO_BUFF_YPBPR_TT_FILTER

#elif  (defined(CHIP_TYPE_hi3798cv200_a) || defined(CHIP_TYPE_hi3798cv200_b))

/*DAC type */
#define   DAC_TYPE_HISI_TORNADO

/* Video Buff Type !*/
#define  VIDEO_BUFF_CVBS       VIDEO_BUFF_CVBS_TT_HIFONE
#define  VIDEO_BUFF_YPBPR      VIDEO_BUFF_YPBPR_TT_HIFONE

#else

/*DAC type */
#define   DAC_TYPE_HISI_TORNADO
/* Video Buff Type!*/
#define  VIDEO_BUFF_CVBS     VIDEO_BUFF_CVBS_FMS6141
#define  VIDEO_BUFF_YPBPR  VIDEO_BUFF_YPBPR_FMS6363

#endif
#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /*  __DRV_DISP_COMMON_H__  */


