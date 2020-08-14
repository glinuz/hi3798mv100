/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_drv_pvr.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2009/12/21
  Description   :
  History       :
  1.Date        : 2009/12/21
    Author      : w58735
    Modification: Created file

******************************************************************************/
#ifndef __HI_DRV_PVR_H__
#define __HI_DRV_PVR_H__

#include "hi_unf_pvr.h"
#include "hi_mpi_pvr.h"

/* definition of max play channel */
#define PVR_PLAY_MAX_CHN_NUM           5 

/* definition of max record channel */
#if     defined(CHIP_TYPE_hi3716mv410)\
    ||  defined(CHIP_TYPE_hi3716mv420)
#define PVR_REC_MAX_CHN_NUM            32
#else
#define PVR_REC_MAX_CHN_NUM            10 
#endif
#define PVR_REC_START_NUM               PVR_PLAY_MAX_CHN_NUM

#define CMD_PVR_INIT_PLAY               _IOR(HI_ID_PVR, 0x01, HI_U32)
#define CMD_PVR_CREATE_PLAY_CHN         _IOR(HI_ID_PVR, 0x02, HI_U32)
#define CMD_PVR_DESTROY_PLAY_CHN        _IOW(HI_ID_PVR, 0x03, HI_U32)
#define CMD_PVR_GET_DECODE_ABILITY      _IOR(HI_ID_PVR, 0x04, HI_U32)
#define CMD_PVR_ACQUIRE_DECODE_ABILITY  _IOW(HI_ID_PVR, 0x05, HI_U32)
#define CMD_PVR_RELEASE_DECODE_ABILITY  _IOW(HI_ID_PVR, 0x06, HI_U32)

#define CMD_PVR_INIT_REC                _IOR(HI_ID_PVR, 0x11, HI_U32)
#define CMD_PVR_CREATE_REC_CHN          _IOR(HI_ID_PVR, 0x12, HI_U32)
#define CMD_PVR_DESTROY_REC_CHN         _IOW(HI_ID_PVR, 0x13, HI_U32)

#if   defined(CHIP_TYPE_hi3716mv310)    \
    ||defined(CHIP_TYPE_hi3716mv300)    \
    ||defined(CHIP_TYPE_hi3110ev500)    \
    ||defined(CHIP_TYPE_hi3712)
#define PVR_PLAY_DECODE_ABILITY (35*1920*1080)
#elif defined(CHIP_TYPE_hi3716h)        \
    ||defined(CHIP_TYPE_hi3716cv100)    \
    ||defined(CHIP_TYPE_hi3720)
#define PVR_PLAY_DECODE_ABILITY (40*1920*1080)
#elif defined(CHIP_TYPE_hi3719mv100)    \
    ||defined(CHIP_TYPE_hi3719cv100)    \
    ||defined(CHIP_TYPE_hi3718mv100)    \
    ||defined(CHIP_TYPE_hi3718cv100)    \
    ||defined(CHIP_TYPE_hi3716cv200)    \
    ||defined(CHIP_TYPE_hi3716mv420)    \
    ||defined(CHIP_TYPE_hi3716mv400)    \
    ||defined(CHIP_TYPE_hi3716mv410)
#define PVR_PLAY_DECODE_ABILITY (65*1920*1080)
#elif defined(CHIP_TYPE_hi3796mv100)    \
    ||defined(CHIP_TYPE_hi3716dv100)    \
    ||defined(CHIP_TYPE_hi3796cv100)    \
    ||defined(CHIP_TYPE_hi3798mv100)    \
    ||defined(CHIP_TYPE_hi3798mv200)    \
    ||defined(CHIP_TYPE_hi3798mv200_a)  \
    ||defined(CHIP_TYPE_hi3798cv100)    \
    ||defined(CHIP_TYPE_hi3798cv200) 
#define PVR_PLAY_DECODE_ABILITY (120*1920*1080)
#else
#define PVR_PLAY_DECODE_ABILITY (30*1920*1080)
#endif

#endif
