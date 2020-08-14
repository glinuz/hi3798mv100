/******************************************************************************
  Copyright (C), 2001-2015, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
  File Name     : drv_vdp_hdr_com.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2015-09-23
  Description   : this file keeps win and disp shared structures and functions.
  History       :
  1.Date        : 2015-09-23
    Author      : q00293180
    Modification: Created file
*******************************************************************************/

#include <linux/kernel.h>
#include <linux/string.h>

//common structure and functions for window and display.
#include "drv_vdp_hdr_com.h"


#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */


static WINDOW_DOLBY_REFERENCE_INFO_S stWinDoviRefInfo;

HI_VOID ResetWinDoviRefInfo(HI_VOID)
{
    stWinDoviRefInfo.u32HdrWinHandle = HI_INVALID_HANDLE;
    stWinDoviRefInfo.u32HDDispWinNum = 0;
    stWinDoviRefInfo.pCurrentFrame = HI_NULL;
    stWinDoviRefInfo.pNextFrame    = HI_NULL;
    stWinDoviRefInfo.bHdrWinStatus = HI_FALSE;
    memset(&stWinDoviRefInfo.stWinOutRect,0,sizeof(HI_RECT_S));
    return ;
}

HI_VOID ClearWinDoviRefInfo(HI_DRV_WINDOW_MODE_E eHdrMode)
{
    stWinDoviRefInfo.u32HdrWinHandle = HI_INVALID_HANDLE;
    stWinDoviRefInfo.pCurrentFrame = HI_NULL;
    stWinDoviRefInfo.pNextFrame    = HI_NULL;
    stWinDoviRefInfo.bHdrWinStatus = HI_FALSE;
    memset(&stWinDoviRefInfo.stWinOutRect, 0, sizeof(HI_RECT_S));

    return ;
}


HI_VOID SetHdrWinHandle(HI_U32 u32WinIndex)
{
    stWinDoviRefInfo.u32HdrWinHandle = u32WinIndex;
    return ;
}

HI_VOID GetHdrWinHandle(HI_U32 *pu32WinHandle)
{
    *pu32WinHandle = stWinDoviRefInfo.u32HdrWinHandle;
    return ;
}


HI_VOID SetHdrWinStatus(HI_BOOL bEnable)
{
    stWinDoviRefInfo.bHdrWinStatus = bEnable;
    return ;
}


HI_VOID SetHDRPathStatus(HI_BOOL bEnable)
{
    stWinDoviRefInfo.bHDRPathClose = bEnable;
    return ;
}

HI_VOID GetHDRPathStatus(HI_BOOL *pbEnable)
{
    *pbEnable = stWinDoviRefInfo.bHDRPathClose;
    return ;
}


HI_VOID UpdateHDWinNum(HI_U32  u32HDWinNum)
{
    stWinDoviRefInfo.u32HDDispWinNum = u32HDWinNum;
    return ;
}

HI_VOID SetWinDoviRefInfo(HI_U32                   u32HDWinNum,
                         HI_DRV_VIDEO_FRAME_S     *pstCurrentFrm,
                         HI_DRV_VIDEO_FRAME_S     *pstNextFrm,
                         HI_RECT_S                stOutRect)
{
    //NOTE:here needn't to check null prt!!
    
    stWinDoviRefInfo.u32HDDispWinNum = u32HDWinNum;

    //update current frame ptr.
    stWinDoviRefInfo.pCurrentFrame = pstCurrentFrm;
    //update next frame ptr.
    stWinDoviRefInfo.pNextFrame    = pstNextFrm;
    stWinDoviRefInfo.stWinOutRect = stOutRect;
    
    return ;
}


HI_VOID GetWinDoviRefInfo(WINDOW_DOLBY_REFERENCE_INFO_S *pstWinDoviRefInfo)
{
    //pointer valid check.
    pstWinDoviRefInfo->u32HdrWinHandle = stWinDoviRefInfo.u32HdrWinHandle;
    pstWinDoviRefInfo->u32HDDispWinNum = stWinDoviRefInfo.u32HDDispWinNum;
    pstWinDoviRefInfo->pCurrentFrame = stWinDoviRefInfo.pCurrentFrame;
    pstWinDoviRefInfo->pNextFrame    = stWinDoviRefInfo.pNextFrame;
    pstWinDoviRefInfo->stWinOutRect  = stWinDoviRefInfo.stWinOutRect;
    pstWinDoviRefInfo->bHdrWinStatus = stWinDoviRefInfo.bHdrWinStatus;
    return ;
}




#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */


