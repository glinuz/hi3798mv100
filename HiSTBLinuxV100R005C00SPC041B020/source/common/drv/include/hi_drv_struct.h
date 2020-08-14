/******************************************************************************
Copyright (C), 2009-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : hi_struct.h
Version       : Initial Draft
Author        : Hisilicon
Created       : 2009/12/16
Last Modified :
Description   : Defines the common data type of the system.
                CNcomment: 系统共用的数据类型定义
Function List :
History       :
******************************************************************************/
/**
 * \file
 * \brief Describes the information about basic data types. CNcomment: 提供基本数据类型的相关信息
 */
#ifndef __DRV_STRUCT_EXT_H__
#define __DRV_STRUCT_EXT_H__

#include "hi_type.h"
#include "hi_module.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */


/*************************** Structure Definition ****************************/
/** \addtogroup      H_2_1_2 */
/** @{ */  /** <!-- 【Common data structure. CNcomment: 通用数据结构】 */

#define UMAP_NAME    "hi_"

#define HI_MOD_SYS                  "sys"
#define UMAP_DEVNAME_SYS            UMAP_NAME HI_MOD_SYS
#define UMAP_DEV_NUM_SYS            1
#define UMAP_MIN_MINOR_SYS          1
#define UMAP_MAX_MINOR_SYS          (UMAP_MIN_MINOR_SYS + UMAP_DEV_NUM_SYS - 1)


#define HI_MOD_LOG                  "log"
#define UMAP_DEVNAME_LOG            UMAP_NAME HI_MOD_LOG
#define UMAP_DEV_NUM_LOG            1
#define UMAP_MIN_MINOR_LOG          (UMAP_MAX_MINOR_SYS + 1)
#define UMAP_MAX_MINOR_LOG          (UMAP_MIN_MINOR_LOG + UMAP_DEV_NUM_LOG - 1)


#define HI_MOD_EVENT                "low_delay_statistics"
#define UMAP_DEVNAME_EVENT          UMAP_NAME HI_MOD_EVENT
#define UMAP_DEV_NUM_EVENT          1
#define UMAP_MIN_MINOR_EVENT        (UMAP_MAX_MINOR_LOG + 1)
#define UMAP_MAX_MINOR_EVENT        (UMAP_MIN_MINOR_EVENT + UMAP_DEV_NUM_EVENT - 1)

#define HI_MOD_STAT                 "stat"
#define UMAP_DEVNAME_STAT           UMAP_NAME HI_MOD_STAT
#define UMAP_DEV_NUM_STAT           1
#define UMAP_MIN_MINOR_STAT         (UMAP_MAX_MINOR_EVENT + 1)
#define UMAP_MAX_MINOR_STAT         (UMAP_MIN_MINOR_STAT + UMAP_DEV_NUM_STAT - 1)



#define HI_MOD_MEM      "mem"

#define HI_MOD_PROC     "proc"

/*End of CMPI */

/*Other modules*/
/**CNcomment:  其它模块 */
#define HI_MOD_CIPHER               "cipher"
#define UMAP_DEVNAME_CIPHER         UMAP_NAME HI_MOD_CIPHER
#define UMAP_DEV_NUM_CIPHER         1
#define UMAP_MIN_MINOR_CIPHER       (UMAP_MAX_MINOR_STAT + 1)
#define UMAP_MAX_MINOR_CIPHER       (UMAP_MIN_MINOR_CIPHER + UMAP_DEV_NUM_CIPHER - 1)

#define HI_MOD_DISP                 "disp"
#define UMAP_DEVNAME_DISP           UMAP_NAME HI_MOD_DISP
#define UMAP_DEV_NUM_DISP           1
#define UMAP_MIN_MINOR_DISP         (UMAP_MAX_MINOR_CIPHER + 1)
#define UMAP_MAX_MINOR_DISP         (UMAP_MIN_MINOR_DISP + UMAP_DEV_NUM_DISP - 1)

#define HI_MOD_VO                   "vo"
#define UMAP_DEVNAME_VO             UMAP_NAME HI_MOD_VO
#define UMAP_DEV_NUM_VO             1
#define UMAP_MIN_MINOR_VO           (UMAP_MAX_MINOR_DISP + 1)
#define UMAP_MAX_MINOR_VO           (UMAP_MIN_MINOR_VO + UMAP_DEV_NUM_VO - 1)

#define HI_MOD_VPSS                 "vpss"
#define UMAP_DEVNAME_VPSS           UMAP_NAME HI_MOD_VPSS
#define UMAP_DEV_NUM_VPSS           1
#define UMAP_MIN_MINOR_VPSS         (UMAP_MAX_MINOR_VO + 1)
#define UMAP_MAX_MINOR_VPSS         (UMAP_MIN_MINOR_VPSS + UMAP_DEV_NUM_VPSS - 1)

#define HI_MOD_VDEC                 "vdec"
#define UMAP_DEVNAME_VDEC           UMAP_NAME HI_MOD_VDEC
#define UMAP_DEV_NUM_VDEC           2
#define UMAP_MIN_MINOR_VDEC         (UMAP_MAX_MINOR_VPSS + 1)
#define UMAP_MAX_MINOR_VDEC         (UMAP_MIN_MINOR_VDEC + UMAP_DEV_NUM_VDEC - 1)

#define HI_MOD_AO                   "ao"
#define UMAP_DEVNAME_AO             UMAP_NAME HI_MOD_AO
#define UMAP_DEV_NUM_AO             2
#define UMAP_MIN_MINOR_AO           (UMAP_MAX_MINOR_VDEC + 1)
#define UMAP_MAX_MINOR_AO           (UMAP_MIN_MINOR_AO + UMAP_DEV_NUM_AO - 1)

#define HI_MOD_AI                   "ai"
#define UMAP_DEVNAME_AI             UMAP_NAME HI_MOD_AI
#define UMAP_DEV_NUM_AI             2
#define UMAP_MIN_MINOR_AI           (UMAP_MAX_MINOR_AO + 1)
#define UMAP_MAX_MINOR_AI           (UMAP_MIN_MINOR_AI + UMAP_DEV_NUM_AI - 1)

#define HI_MOD_AIAO                 "aiao"
#define UMAP_DEVNAME_AIAO           UMAP_NAME HI_MOD_AIAO
#define UMAP_DEV_NUM_AIAO           2
#define UMAP_MIN_MINOR_AIAO         (UMAP_MAX_MINOR_AI + 1)
#define UMAP_MAX_MINOR_AIAO         (UMAP_MIN_MINOR_AIAO + UMAP_DEV_NUM_AIAO - 1)

#define HI_MOD_AENC                 "aenc"
#define UMAP_DEVNAME_AENC           UMAP_NAME HI_MOD_AENC
#define UMAP_DEV_NUM_AENC           2
#define UMAP_MIN_MINOR_AENC         (UMAP_MAX_MINOR_AIAO + 1)
#define UMAP_MAX_MINOR_AENC         (UMAP_MIN_MINOR_AENC + UMAP_DEV_NUM_AENC - 1)

#define HI_MOD_AFLT                 "aflt"
#define UMAP_DEVNAME_AFLT           UMAP_NAME HI_MOD_AFLT
#define UMAP_DEV_NUM_AFLT           2
#define UMAP_MIN_MINOR_AFLT         (UMAP_MAX_MINOR_AENC + 1)
#define UMAP_MAX_MINOR_AFLT         (UMAP_MIN_MINOR_AFLT + UMAP_DEV_NUM_AFLT - 1)

#define HI_MOD_ADSP                 "adsp"
#define UMAP_DEVNAME_ADSP           UMAP_NAME HI_MOD_ADSP
#define UMAP_DEV_NUM_ADSP           2
#define UMAP_MIN_MINOR_ADSP         (UMAP_MAX_MINOR_AFLT + 1)
#define UMAP_MAX_MINOR_ADSP         (UMAP_MIN_MINOR_ADSP + UMAP_DEV_NUM_ADSP - 1)

#define HI_MOD_ADEC                 "adec"
#define UMAP_DEVNAME_ADEC           UMAP_NAME HI_MOD_ADEC
#define UMAP_DEV_NUM_ADEC           2
#define UMAP_MIN_MINOR_ADEC         (UMAP_MAX_MINOR_ADSP + 1)
#define UMAP_MAX_MINOR_ADEC         (UMAP_MIN_MINOR_ADEC + UMAP_DEV_NUM_ADEC - 1)

#define HI_MOD_DEMUX                "demux"
#define UMAP_DEVNAME_DEMUX          UMAP_NAME HI_MOD_DEMUX
#define UMAP_DEV_NUM_DEMUX          1
#define UMAP_MIN_MINOR_DEMUX        (UMAP_MAX_MINOR_ADEC + 1)
#define UMAP_MAX_MINOR_DEMUX        (UMAP_MIN_MINOR_DEMUX + UMAP_DEV_NUM_DEMUX - 1)

#define HI_MOD_JPGD                 "jpgd"
#define UMAP_DEVNAME_JPGD           UMAP_NAME HI_MOD_JPGD
#define UMAP_DEV_NUM_JPGD           1
#define UMAP_MIN_MINOR_JPGD         (UMAP_MAX_MINOR_DEMUX + 1)
#define UMAP_MAX_MINOR_JPGD         (UMAP_MIN_MINOR_JPGD + UMAP_DEV_NUM_JPGD - 1)

#define HI_MOD_SYNC                 "sync"
#define UMAP_DEVNAME_SYNC           UMAP_NAME HI_MOD_SYNC
#define UMAP_DEV_NUM_SYNC           1
#define UMAP_MIN_MINOR_SYNC         (UMAP_MAX_MINOR_JPGD + 1)
#define UMAP_MAX_MINOR_SYNC         (UMAP_MIN_MINOR_SYNC + UMAP_DEV_NUM_SYNC - 1)

#define HI_MOD_AVPLAY               "avplay"
#define UMAP_DEVNAME_AVPLAY         UMAP_NAME HI_MOD_AVPLAY
#define UMAP_DEV_NUM_AVPLAY         1
#define UMAP_MIN_MINOR_AVPLAY       (UMAP_MAX_MINOR_SYNC + 1)
#define UMAP_MAX_MINOR_AVPLAY       (UMAP_MIN_MINOR_AVPLAY + UMAP_DEV_NUM_AVPLAY - 1)

#define HI_MOD_PVR                  "pvr"
#define UMAP_DEVNAME_PVR            UMAP_NAME HI_MOD_PVR
#define UMAP_DEV_NUM_PVR            1
#define UMAP_MIN_MINOR_PVR          (UMAP_MAX_MINOR_AVPLAY + 1)
#define UMAP_MAX_MINOR_PVR          (UMAP_MIN_MINOR_PVR + UMAP_DEV_NUM_PVR - 1)

#define HI_MOD_HDMI                 "hdmi"
#define UMAP_DEVNAME_HDMI           UMAP_NAME HI_MOD_HDMI
#define UMAP_DEV_NUM_HDMI           1
#define UMAP_MIN_MINOR_HDMI         (UMAP_MAX_MINOR_PVR + 1)
#define UMAP_MAX_MINOR_HDMI         (UMAP_MIN_MINOR_HDMI + UMAP_DEV_NUM_HDMI - 1)

#define HI_MOD_VI                   "vi"
#define UMAP_DEVNAME_VI             UMAP_NAME HI_MOD_VI
#define UMAP_DEV_NUM_VI             1
#define UMAP_MIN_MINOR_VI           (UMAP_MAX_MINOR_HDMI + 1)
#define UMAP_MAX_MINOR_VI           (UMAP_MIN_MINOR_VI + UMAP_DEV_NUM_VI - 1)

#define HI_MOD_VENC                 "venc"
#define UMAP_DEVNAME_VENC           UMAP_NAME HI_MOD_VENC
#define UMAP_DEV_NUM_VENC           2
#define UMAP_MIN_MINOR_VENC         (UMAP_MAX_MINOR_VI + 1)
#define UMAP_MAX_MINOR_VENC         (UMAP_MIN_MINOR_VENC + UMAP_DEV_NUM_VENC - 1)

#define HI_MOD_SIO_AI               "sio_ai"
#define UMAP_DEVNAME_SIO_AI         UMAP_NAME HI_MOD_SIO_AI
#define UMAP_DEV_NUM_SIO_AI         2
#define UMAP_MIN_MINOR_SIO_AI       (UMAP_MAX_MINOR_VENC + 1)
#define UMAP_MAX_MINOR_SIO_AI       (UMAP_MIN_MINOR_SIO_AI + UMAP_DEV_NUM_SIO_AI - 1)

#define HI_MOD_SIO_AO               "sio_ao"
#define UMAP_DEVNAME_SIO_AO         UMAP_NAME HI_MOD_SIO_AO
#define UMAP_DEV_NUM_SIO_AO         2
#define UMAP_MIN_MINOR_SIO_AO       (UMAP_MAX_MINOR_SIO_AI + 1)
#define UMAP_MAX_MINOR_SIO_AO       (UMAP_MIN_MINOR_SIO_AO + UMAP_DEV_NUM_SIO_AO - 1)

#define HI_MOD_DMA                  "dma"
#define UMAP_DEVNAME_DMA            UMAP_NAME HI_MOD_DMA
#define UMAP_DEV_NUM_DMA            1
#define UMAP_MIN_MINOR_DMA          (UMAP_MAX_MINOR_SIO_AO + 1)
#define UMAP_MAX_MINOR_DMA          (UMAP_MIN_MINOR_DMA + UMAP_DEV_NUM_DMA - 1)

#define HI_MOD_AUDADP               "audadp"
#define UMAP_DEVNAME_AUDADP         UMAP_NAME HI_MOD_AUDADP
#define UMAP_DEV_NUM_AUDADP         1
#define UMAP_MIN_MINOR_AUDADP       (UMAP_MAX_MINOR_DMA + 1)
#define UMAP_MAX_MINOR_AUDADP       (UMAP_MIN_MINOR_AUDADP + UMAP_DEV_NUM_AUDADP - 1)

#define HI_MOD_HME                  "hme"
#define UMAP_DEVNAME_HME            UMAP_NAME HI_MOD_HME
#define UMAP_DEV_NUM_HME            1
#define UMAP_MIN_MINOR_HME          (UMAP_MAX_MINOR_AUDADP + 1)
#define UMAP_MAX_MINOR_HME          (UMAP_MIN_MINOR_HME + UMAP_DEV_NUM_HME - 1)

#define HI_MOD_NDPT                 "ndpt"
#define UMAP_DEVNAME_NDPT           UMAP_NAME HI_MOD_NDPT
#define UMAP_DEV_NUM_NDPT           1
#define UMAP_MIN_MINOR_NDPT         (UMAP_MAX_MINOR_HME + 1)
#define UMAP_MAX_MINOR_NDPT         (UMAP_MIN_MINOR_NDPT + UMAP_DEV_NUM_NDPT - 1)

#define HI_MOD_VP                   "vp"
#define UMAP_DEVNAME_VP             UMAP_NAME HI_MOD_VP
#define UMAP_DEV_NUM_VP             1
#define UMAP_MIN_MINOR_VP           (UMAP_MAX_MINOR_NDPT + 1)
#define UMAP_MAX_MINOR_VP           (UMAP_MIN_MINOR_VP + UMAP_DEV_NUM_VP - 1)

/*                 ECS                */
#define HI_MOD_IR                   "ir"
#define UMAP_DEVNAME_IR             UMAP_NAME HI_MOD_IR
#define UMAP_DEV_NUM_IR             1
#define UMAP_MIN_MINOR_IR           (UMAP_MAX_MINOR_VP + 1)
#define UMAP_MAX_MINOR_IR           (UMAP_MIN_MINOR_IR + UMAP_DEV_NUM_IR - 1)

#define HI_MOD_KEYLED               "keyled"
#define UMAP_DEVNAME_KEYLED         UMAP_NAME HI_MOD_KEYLED
#define UMAP_DEV_NUM_KEYLED         1
#define UMAP_MIN_MINOR_KEYLED       (UMAP_MAX_MINOR_IR + 1)
#define UMAP_MAX_MINOR_KEYLED       (UMAP_MIN_MINOR_KEYLED + UMAP_DEV_NUM_KEYLED - 1)

#define HI_MOD_I2C                  "i2c"
#define UMAP_DEVNAME_I2C            UMAP_NAME HI_MOD_I2C
#define UMAP_DEV_NUM_I2C            1
#define UMAP_MIN_MINOR_I2C          (UMAP_MAX_MINOR_KEYLED + 1)
#define UMAP_MAX_MINOR_I2C          (UMAP_MIN_MINOR_I2C + UMAP_DEV_NUM_I2C - 1)

#define HI_MOD_TUNER                "tuner"
#define UMAP_DEVNAME_TUNER          UMAP_NAME HI_MOD_TUNER
#define UMAP_DEV_NUM_TUNER          1
#define UMAP_MIN_MINOR_TUNER        (UMAP_MAX_MINOR_I2C + 1)
#define UMAP_MAX_MINOR_TUNER        (UMAP_MIN_MINOR_TUNER + UMAP_DEV_NUM_TUNER - 1)

#define HI_MOD_SCI                  "sci"
#define UMAP_DEVNAME_SCI            UMAP_NAME HI_MOD_SCI
#define UMAP_DEV_NUM_SCI            1
#define UMAP_MIN_MINOR_SCI          (UMAP_MAX_MINOR_TUNER + 1)
#define UMAP_MAX_MINOR_SCI          (UMAP_MIN_MINOR_SCI + UMAP_DEV_NUM_SCI - 1)

#define HI_MOD_PM                   "pm"
#define UMAP_DEVNAME_PM             UMAP_NAME HI_MOD_PM
#define UMAP_DEV_NUM_PM             1
#define UMAP_MIN_MINOR_PM           (UMAP_MAX_MINOR_SCI + 1)
#define UMAP_MAX_MINOR_PM           (UMAP_MIN_MINOR_PM + UMAP_DEV_NUM_PM - 1)

#define HI_MOD_CA                   "ca"
#define UMAP_DEVNAME_CA             UMAP_NAME HI_MOD_CA
#define UMAP_DEV_NUM_CA             1
#define UMAP_MIN_MINOR_CA           (UMAP_MAX_MINOR_PM + 1)
#define UMAP_MAX_MINOR_CA           (UMAP_MIN_MINOR_CA + UMAP_DEV_NUM_CA - 1)

#define HI_MOD_OTP                  "otp"
#define UMAP_DEVNAME_OTP            UMAP_NAME HI_MOD_OTP
#define UMAP_DEV_NUM_OTP            1
#define UMAP_MIN_MINOR_OTP          (UMAP_MAX_MINOR_CA + 1)
#define UMAP_MAX_MINOR_OTP          (UMAP_MIN_MINOR_OTP + UMAP_DEV_NUM_OTP - 1)

#define HI_MOD_SVDEC                "svdec"
#define UMAP_DEVNAME_SVDEC          UMAP_NAME HI_MOD_SVDEC
#define UMAP_DEV_NUM_SVDEC          1
#define UMAP_MIN_MINOR_SVDEC        (UMAP_MAX_MINOR_OTP + 1)
#define UMAP_MAX_MINOR_SVDEC        (UMAP_MIN_MINOR_SVDEC + UMAP_DEV_NUM_SVDEC - 1)

#define HI_MOD_SETHDCP              "sethdcp"
#define UMAP_DEVNAME_SETHDCP        UMAP_NAME HI_MOD_SETHDCP
#define UMAP_DEV_NUM_SETHDCP        1
#define UMAP_MIN_MINOR_SETHDCP      (UMAP_MAX_MINOR_SVDEC + 1)
#define UMAP_MAX_MINOR_SETHDCP      (UMAP_MIN_MINOR_SETHDCP + UMAP_DEV_NUM_SETHDCP - 1)

#define HI_MOD_CI                   "ci"
#define UMAP_DEVNAME_CI             UMAP_NAME HI_MOD_CI
#define UMAP_DEV_NUM_CI             1
#define UMAP_MIN_MINOR_CI           (UMAP_MAX_MINOR_SETHDCP + 1)
#define UMAP_MAX_MINOR_CI           (UMAP_MIN_MINOR_CI + UMAP_DEV_NUM_CI - 1)

#define HI_MOD_SUBT                 "subt"
#define UMAP_DEVNAME_SUBT           UMAP_NAME HI_MOD_SUBT
#define UMAP_DEV_NUM_SUBT           1
#define UMAP_MIN_MINOR_SUBT         (UMAP_MAX_MINOR_CI+ 1)
#define UMAP_MAX_MINOR_SUBT         (UMAP_MIN_MINOR_SUBT + UMAP_DEV_NUM_SUBT - 1)

#define HI_MOD_MEM2                 "memmgr"
#define UMAP_DEVNAME_MEM2           UMAP_NAME HI_MOD_MEM2
#define UMAP_DEV_NUM_MEM2           1
#define UMAP_MIN_MINOR_MEM2         (UMAP_MAX_MINOR_SUBT + 1)
#define UMAP_MAX_MINOR_MEM2         (UMAP_MIN_MINOR_MEM2 + UMAP_DEV_NUM_MEM2 - 1)

#define HI_MOD_MODULE               "module"
#define UMAP_DEVNAME_MODULE         UMAP_NAME HI_MOD_MODULE
#define UMAP_DEV_NUM_MODULE         1
#define UMAP_MIN_MINOR_MODULE       (UMAP_MAX_MINOR_MEM2 + 1)
#define UMAP_MAX_MINOR_MODULE       (UMAP_MIN_MINOR_MODULE + UMAP_DEV_NUM_MODULE - 1)

#define HI_MOD_MLOG                 "mlog"
#define UMAP_DEVNAME_MLOG           UMAP_NAME HI_MOD_MLOG
#define UMAP_DEV_NUM_MLOG           1
#define UMAP_MIN_MINOR_MLOG         (UMAP_MAX_MINOR_MODULE + 1)
#define UMAP_MAX_MINOR_MLOG         (UMAP_MIN_MINOR_MLOG + UMAP_DEV_NUM_MLOG - 1)

#define HI_MOD_MCE                  "mce"
#define UMAP_DEVNAME_MCE            UMAP_NAME HI_MOD_MCE
#define UMAP_DEV_NUM_MCE            1
#define UMAP_MIN_MINOR_MCE          (UMAP_MAX_MINOR_MLOG + 1)
#define UMAP_MAX_MINOR_MCE          (UMAP_MIN_MINOR_MCE + UMAP_DEV_NUM_MCE - 1)

#define HI_MOD_PDM                  "pdm"
#define UMAP_DEVNAME_PDM            UMAP_NAME HI_MOD_PDM
#define UMAP_DEV_NUM_PDM            1
#define UMAP_MIN_MINOR_PDM          (UMAP_MAX_MINOR_MCE + 1)
#define UMAP_MAX_MINOR_PDM          (UMAP_MIN_MINOR_PDM + UMAP_DEV_NUM_PDM - 1)

#define UMAP_DEV_NUM_TDE            1
#define UMAP_MIN_MINOR_TDE          (UMAP_MAX_MINOR_PDM + 1)
#define UMAP_MAX_MINOR_TDE          (UMAP_MIN_MINOR_TDE + UMAP_DEV_NUM_TDE - 1)

#define UMAP_DEV_NUM_HIFB           1
#define UMAP_MIN_MINOR_HIFB         (UMAP_MAX_MINOR_TDE + 1)
#define UMAP_MAX_MINOR_HIFB         (UMAP_MIN_MINOR_HIFB + UMAP_DEV_NUM_HIFB - 1)

#define UMAP_DEV_NUM_PNG            2
#define UMAP_MIN_MINOR_PNG          (UMAP_MAX_MINOR_HIFB + 1)
#define UMAP_MAX_MINOR_PNG          (UMAP_MIN_MINOR_PNG + UMAP_DEV_NUM_PNG - 1)

#define UMAP_DEV_NUM_JPEG           1
#define UMAP_MIN_MINOR_JPEG         (UMAP_MAX_MINOR_PNG + 1)
#define UMAP_MAX_MINOR_JPEG         (UMAP_MIN_MINOR_JPEG + UMAP_DEV_NUM_JPEG - 1)

#define UMAP_DEV_NUM_JPGE           1
#define UMAP_MIN_MINOR_JPGE         (UMAP_MAX_MINOR_JPEG + 1)
#define UMAP_MAX_MINOR_JPGE         (UMAP_MIN_MINOR_JPGE + UMAP_DEV_NUM_JPGE - 1)

#define HI_MOD_WDG                  "wdg"
#define UMAP_DEVNAME_WDG            UMAP_NAME HI_MOD_WDG
#define UMAP_DEV_NUM_WDG            1
#define UMAP_MIN_MINOR_WDG          (UMAP_MAX_MINOR_JPGE + 1)
#define UMAP_MAX_MINOR_WDG          (UMAP_MIN_MINOR_WDG + UMAP_DEV_NUM_WDG - 1)

#define HI_MOD_GPIO                 "gpio"
#define UMAP_DEVNAME_GPIO           UMAP_NAME HI_MOD_GPIO
#define UMAP_DEV_NUM_GPIO           1
#define UMAP_MIN_MINOR_GPIO         (UMAP_MAX_MINOR_WDG + 1)
#define UMAP_MAX_MINOR_GPIO         (UMAP_MIN_MINOR_GPIO + UMAP_DEV_NUM_GPIO - 1)

#define HI_MOD_SIL9293              "sil9293"
#define UMAP_DEVNAME_SIL9293        UMAP_NAME HI_MOD_SIL9293
#define UMAP_DEV_NUM_SIL9293        1
#define UMAP_MIN_MINOR_SIL9293      (UMAP_MAX_MINOR_GPIO + 1)
#define UMAP_MAX_MINOR_SIL9293      (UMAP_MIN_MINOR_SIL9293 + UMAP_DEV_NUM_SIL9293 - 1)

#define HI_MOD_PQ                   "pq"
#define UMAP_DEVNAME_PQ             UMAP_NAME HI_MOD_PQ
#define UMAP_DEV_NUM_PQ             1
#define UMAP_MIN_MINOR_PQ           (UMAP_MAX_MINOR_SIL9293 + 1)
#define UMAP_MAX_MINOR_PQ           (UMAP_MIN_MINOR_PQ + UMAP_DEV_NUM_PQ - 1)

#define HI_MOD_PWM                  "pwm"
#define UMAP_DEVNAME_PWM            UMAP_NAME HI_MOD_PWM
#define UMAP_DEV_NUM_PWM            1
#define UMAP_MIN_MINOR_PWM          (UMAP_MAX_MINOR_PQ + 1)
#define UMAP_MAX_MINOR_PWM          (UMAP_MIN_MINOR_PWM + UMAP_DEV_NUM_PWM - 1)

#define HI_MOD_SIF                  "sif"
#define UMAP_DEVNAME_SIF            UMAP_NAME HI_MOD_SIF
#define UMAP_DEV_NUM_SIF            1
#define UMAP_MIN_MINOR_SIF          (UMAP_MAX_MINOR_PWM + 1)
#define UMAP_MAX_MINOR_SIF          (UMAP_MIN_MINOR_SIF + UMAP_DEV_NUM_SIF - 1)

#define HI_MOD_AMP                  "amp"
#define UMAP_DEVNAME_AMP            UMAP_NAME HI_MOD_AMP
#define UMAP_DEV_NUM_AMP            1
#define UMAP_MIN_MINOR_AMP          (UMAP_MAX_MINOR_SIF + 1)
#define UMAP_MAX_MINOR_AMP          (UMAP_MIN_MINOR_AMP + UMAP_DEV_NUM_AMP - 1)

#define HI_MOD_VPU                  "vpu"
#define UMAP_DEVNAME_VPU            UMAP_NAME HI_MOD_VPU
#define UMAP_DEV_NUM_VPU            1
#define UMAP_MIN_MINOR_VPU          (UMAP_MAX_MINOR_AMP + 1)
#define UMAP_MAX_MINOR_VPU          (UMAP_MIN_MINOR_VPU + UMAP_DEV_NUM_VPU - 1)

#define HI_MOD_SPI                  "spi"
#define UMAP_DEVNAME_SPI            UMAP_NAME HI_MOD_SPI
#define UMAP_DEV_NUM_SPI            1
#define UMAP_MIN_MINOR_SPI          (UMAP_MAX_MINOR_VPU + 1)
#define UMAP_MAX_MINOR_SPI          (UMAP_MIN_MINOR_SPI + UMAP_DEV_NUM_SPI - 1)

#define HI_MOD_HDMIRX                  "hdmirx"
#define UMAP_DEVNAME_HDMIRX            UMAP_NAME HI_MOD_HDMIRX
#define UMAP_DEV_NUM_HDMIRX            1
#define UMAP_MIN_MINOR_HDMIRX          (UMAP_MAX_MINOR_SPI + 1)
#define UMAP_MAX_MINOR_HDMIRX          (UMAP_MIN_MINOR_HDMIRX + UMAP_DEV_NUM_HDMIRX -1)

#define UMAP_DEV_NUM_TOTAL          UMAP_MAX_MINOR_HDMIRX

/** @} */  /** <!-- ==== Structure Definition End ==== */

#define IOC_TYPE_DEMUX          'I'
#define IOC_TYPE_MEM            'J'
#define IOC_TYPE_RELEASE        'K'
#define IOC_TYPE_EVENT          'L'
#define IOC_TYPE_SYS            'M'
#define IOC_TYPE_STAT           'N'
#define IOC_TYPE_MCC            'O'
#define IOC_TYPE_TDE            'P'

#define IOC_TYPE_CIPHER         'Q'
#define IOC_TYPE_AVPLAY         'R'

#define IOC_TYPE_VI             'S'
#define IOC_TYPE_VENC           'T'
#define IOC_TYPE_CA             'U'
#define IOC_TYPE_OTP            'V'

#define IOC_TYPE_VP             'W'
#define IOC_TYPE_SIF            'X'


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  /* __HI_STRUCT_H__ */

