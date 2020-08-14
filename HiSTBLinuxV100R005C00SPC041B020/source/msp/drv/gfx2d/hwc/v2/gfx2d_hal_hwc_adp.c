/******************************************************************************
*              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: gfx2d_hal_hwc_adp.c
* Description: hwc adp code
*
* History:
* Version   Date         Author     DefectNum    Description
* main\1    2013-10-23              NULL         Create this file.
*******************************************************************************/
#include "gfx2d_hal_hwc_adp.h"
#include "hi_gfx_sys_k.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif  /*__cplusplus*/
#endif  /*__cplusplus*/

static HWC_CAPABILITY_S gs_stCapability =
{
    HWC_MAX_LAYERS,
    6,
    1,
#if HWC_VERSION == 0x500
    {0x00c043d0, 0x7efc},
#else
    {0x00c043d0, 0xfe7efc},
#endif
    {0x0c040c0, 0x200},
    1,
    0xffff,
    16,
    1,
#if HWC_VERSION == 0x400 /* hi3798cv200 */
    2560,
#elif HWC_VERSION == 0x300 /*hi3798cv100 hi3796cv100*/
    3840,
#else
    2560,
#endif
    1,
#if HWC_VERSION == 0x400
    1600,
#elif HWC_VERSION == 0x300
    2160,
#else
    1600,
#endif
    32,
    32,
    16,
    16,
    2,
    {0x400000, 0x0},
#if HWC_VERSION >= 0x400
    HI_TRUE,
    HI_TRUE,
    2,
    1,
#else
    HI_FALSE,
    HI_FALSE,
    1,
    2,
#endif
};

HI_S32 HWC_ADP_GetCapability(HWC_CAPABILITY_S *pstCapability)
{
    HI_GFX_Memcpy(pstCapability, &gs_stCapability, sizeof(HWC_CAPABILITY_S));

    return HI_SUCCESS;
}
#ifdef __cplusplus
#if __cplusplus
}
#endif  /*__cplusplus*/
#endif  /*__cplusplus*/
