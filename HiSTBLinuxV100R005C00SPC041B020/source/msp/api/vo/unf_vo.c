/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_unf_vo.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2009/12/16
  Description   :
  History       :
  1.Date        : 2009/12/16
    Author      : w58735
    Modification: Created file

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include "hi_mpi_win.h"
#include "hi_mpi_vi.h"
#include "hi_error_mpi.h"
#include "mpi_disp_tran.h"
#include "hi_mpi_avplay.h"

HI_S32 HI_UNF_VO_Init(HI_UNF_VO_DEV_MODE_E enDevMode)
{
    if (HI_UNF_VO_DEV_MODE_BUTT <= enDevMode)
    {
        HI_FATAL_WIN("Invalid mode!\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    return HI_MPI_WIN_Init();
}

HI_S32 HI_UNF_VO_DeInit(HI_VOID)
{
    return HI_MPI_WIN_DeInit();
}

HI_S32 VO_ConvertWinAttrToMPI(HI_UNF_WINDOW_ATTR_S* pUnfAttr, HI_DRV_WIN_ATTR_S* pstMpiAttr)
{
    if (!pstMpiAttr)
    {
        HI_ERR_WIN("para pstMpiAttr is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }
    if (!pUnfAttr)
    {
        HI_ERR_WIN("para pUnfAttr is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    /* conver unf parameter to driver parameters */
    memset(pstMpiAttr, 0, sizeof(HI_DRV_WIN_ATTR_S));

    Transfer_DispID(&pUnfAttr->enDisp, &pstMpiAttr->enDisp, HI_TRUE);

    pstMpiAttr->bVirtual = pUnfAttr->bVirtual;

    if (HI_UNF_VO_ASPECT_CVRS_BUTT <= pUnfAttr->stWinAspectAttr.enAspectCvrs)
    {
        HI_ERR_WIN("para enAspectCvrs is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }


    if (pUnfAttr->stWinAspectAttr.bUserDefAspectRatio)
    {
        pstMpiAttr->stCustmAR.u32ARw = pUnfAttr->stWinAspectAttr.u32UserAspectWidth;
        pstMpiAttr->stCustmAR.u32ARh = pUnfAttr->stWinAspectAttr.u32UserAspectHeight;
    }
    else
    {
        pstMpiAttr->stCustmAR.u32ARw = 0;
        pstMpiAttr->stCustmAR.u32ARh = 0;

    }
    Transfe_ARConvert(&pUnfAttr->stWinAspectAttr.enAspectCvrs, &pstMpiAttr->enARCvrs, HI_TRUE);


    pstMpiAttr->bUseCropRect = pUnfAttr->bUseCropRect;
    pstMpiAttr->stCropRect.u32LeftOffset = pUnfAttr->stCropRect.u32LeftOffset;
    pstMpiAttr->stCropRect.u32TopOffset  = pUnfAttr->stCropRect.u32TopOffset;
    pstMpiAttr->stCropRect.u32RightOffset  = pUnfAttr->stCropRect.u32RightOffset;
    pstMpiAttr->stCropRect.u32BottomOffset = pUnfAttr->stCropRect.u32BottomOffset;

    pstMpiAttr->stInRect = pUnfAttr->stInputRect;
    pstMpiAttr->stOutRect = pUnfAttr->stOutputRect;

    if (pUnfAttr->bVirtual)
    {
        Transfer_VideoFormat(&pUnfAttr->enVideoFormat, &pstMpiAttr->enDataFormat, HI_TRUE);
        if (pstMpiAttr->enDataFormat != HI_DRV_PIX_FMT_NV21
           && pstMpiAttr->enDataFormat != HI_DRV_PIX_FMT_NV61_2X1)
        {
            HI_ERR_WIN("para enVideoFormat can't be supported.\n");
            return HI_ERR_VO_INVALID_PARA;
        }
    }

    return HI_SUCCESS;
}

HI_S32 VO_ConvertWinAttrToUNF(HI_DRV_WIN_ATTR_S* pstMpiAttr, HI_UNF_WINDOW_ATTR_S* pUnfAttr)
{
    if (!pstMpiAttr)
    {
        HI_ERR_WIN("para pstMpiAttr is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }
    if (!pUnfAttr)
    {
        HI_ERR_WIN("para pUnfAttr is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    /* conver unf parameter to driver parameters */
    memset(pUnfAttr, 0, sizeof(HI_UNF_WINDOW_ATTR_S));
    Transfer_DispID(&pUnfAttr->enDisp, &pstMpiAttr->enDisp, HI_FALSE);

    pUnfAttr->bVirtual = pstMpiAttr->bVirtual;
#if 0
    if (pstMpiAttr->stCustmAR.u8ARw && pstMpiAttr->stCustmAR.u8ARh)
    {
        pUnfAttr->stWinAspectAttr.bUserDefAspectRatio = HI_TRUE;
    }

    pUnfAttr->stWinAspectAttr.u32UserAspectWidth  = pstMpiAttr->stCustmAR.u8ARw;
    pUnfAttr->stWinAspectAttr.u32UserAspectHeight = pstMpiAttr->stCustmAR.u8ARh;

    Transfe_ARConvert(&pUnfAttr->stWinAspectAttr.enAspectCvrs, &pstMpiAttr->enARCvrs, HI_FALSE);
    pUnfAttr->stWinAspectAttr.enAspectCvrs = (HI_UNF_VO_ASPECT_CVRS_E)pstMpiAttr->enARCvrs;
#else

    if (pstMpiAttr->stCustmAR.u32ARw != 0 && pstMpiAttr->stCustmAR.u32ARh != 0)
    {
        pUnfAttr->stWinAspectAttr.bUserDefAspectRatio = HI_TRUE;
        pUnfAttr->stWinAspectAttr.u32UserAspectWidth  = pstMpiAttr->stCustmAR.u32ARw;
        pUnfAttr->stWinAspectAttr.u32UserAspectHeight = pstMpiAttr->stCustmAR.u32ARh;
    }
    else
    {
        pUnfAttr->stWinAspectAttr.bUserDefAspectRatio = HI_FALSE;
        pUnfAttr->stWinAspectAttr.u32UserAspectWidth  = pstMpiAttr->stCustmAR.u32ARw;
        pUnfAttr->stWinAspectAttr.u32UserAspectHeight = pstMpiAttr->stCustmAR.u32ARh;

    }
    Transfe_ARConvert(&pUnfAttr->stWinAspectAttr.enAspectCvrs, &pstMpiAttr->enARCvrs, HI_FALSE);

#endif

    pUnfAttr->bUseCropRect = pstMpiAttr->bUseCropRect ;
    pUnfAttr->stCropRect.u32LeftOffset   = pstMpiAttr->stCropRect.u32LeftOffset;
    pUnfAttr->stCropRect.u32TopOffset    = pstMpiAttr->stCropRect.u32TopOffset;
    pUnfAttr->stCropRect.u32RightOffset  = pstMpiAttr->stCropRect.u32RightOffset;
    pUnfAttr->stCropRect.u32BottomOffset = pstMpiAttr->stCropRect.u32BottomOffset;

    pUnfAttr->stInputRect = pstMpiAttr->stInRect;

    pUnfAttr->stOutputRect = pstMpiAttr->stOutRect;

    if (pstMpiAttr->bVirtual)
    {
        Transfer_VideoFormat(&pUnfAttr->enVideoFormat, &pstMpiAttr->enDataFormat, HI_FALSE);
    }

    return HI_SUCCESS;
}


HI_S32 HI_UNF_VO_CreateWindow(const HI_UNF_WINDOW_ATTR_S* pWinAttr, HI_HANDLE* phWindow)
{
    HI_DRV_WIN_ATTR_S stMpiAttr;
    HI_S32 s32Ret = HI_SUCCESS;

    if (!pWinAttr)
    {
        HI_ERR_WIN("para pWinAttr is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    if (!phWindow)
    {
        HI_ERR_WIN("para phWindow is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    memset((HI_VOID*)&stMpiAttr, 0, sizeof(HI_DRV_WIN_ATTR_S));

    s32Ret = VO_ConvertWinAttrToMPI((HI_UNF_WINDOW_ATTR_S*)pWinAttr, &stMpiAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_WIN("para pWinAttr is err.\n");
        return s32Ret;
    }

    s32Ret = HI_MPI_WIN_Create(&stMpiAttr, phWindow);
    return s32Ret;
}

HI_S32 HI_UNF_VO_CreateWindowExt(const HI_UNF_WINDOW_ATTR_S* pWinAttr,
                                  HI_HANDLE *phWindow,
                                  HI_BOOL bVirtScreen)
{
    HI_DRV_WIN_ATTR_S stMpiAttr;
    HI_S32 s32Ret = HI_SUCCESS;

    if (!pWinAttr)
    {
        HI_ERR_WIN("para pWinAttr is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    if (!phWindow)
    {
        HI_ERR_WIN("para phWindow is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    memset((HI_VOID*)&stMpiAttr, 0, sizeof(HI_DRV_WIN_ATTR_S));
    
    s32Ret = VO_ConvertWinAttrToMPI((HI_UNF_WINDOW_ATTR_S*)pWinAttr, &stMpiAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_WIN("para pWinAttr is err.\n");
        return s32Ret;
    }

    s32Ret = HI_MPI_WIN_Create_Ext(&stMpiAttr, phWindow, bVirtScreen);

    return s32Ret;
}


HI_S32 HI_UNF_VO_DestroyWindow(HI_HANDLE hWindow)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_WIN_Destroy(hWindow);

    return s32Ret;
}

HI_S32 HI_UNF_VO_SetWindowEnable(HI_HANDLE hWindow, HI_BOOL bEnable)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_WIN_SetEnable(hWindow, bEnable);

    return s32Ret;
}

HI_S32 HI_UNF_VO_GetWindowEnable(HI_HANDLE hWindow, HI_BOOL* pbEnable)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_WIN_GetEnable(hWindow, pbEnable);

    return s32Ret;
}

HI_S32 HI_UNF_VO_SetWindowAttr(HI_HANDLE hWindow, const HI_UNF_WINDOW_ATTR_S* pWinAttr)
{
    HI_DRV_WIN_ATTR_S stMpiAttr;
    HI_S32 s32Ret = HI_SUCCESS;

    if (!pWinAttr)
    {
        HI_ERR_WIN("para pWinAttr is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    memset((HI_VOID*)&stMpiAttr, 0, sizeof(HI_DRV_WIN_ATTR_S));
    s32Ret = VO_ConvertWinAttrToMPI((HI_UNF_WINDOW_ATTR_S*)pWinAttr, &stMpiAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_WIN("para pWinAttr is err.\n");
        return s32Ret;
    }

    s32Ret = HI_MPI_WIN_SetAttr(hWindow, &stMpiAttr);

    return s32Ret;
}

HI_S32 HI_UNF_VO_GetWindowAttr(HI_HANDLE hWindow, HI_UNF_WINDOW_ATTR_S* pWinAttr)
{
    HI_DRV_WIN_ATTR_S stMpiAttr;
    HI_S32 s32Ret = HI_SUCCESS;

    if (!pWinAttr)
    {
        HI_ERR_WIN("para pWinAttr is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    memset((HI_VOID*)&stMpiAttr, 0, sizeof(HI_DRV_WIN_ATTR_S));

    s32Ret = HI_MPI_WIN_GetAttr(hWindow, &stMpiAttr);
    if (!s32Ret)
    {
        VO_ConvertWinAttrToUNF(&stMpiAttr, pWinAttr);
    }

    return s32Ret;
}

HI_S32 HI_UNF_VO_AcquireFrame(HI_HANDLE hWindow, HI_UNF_VIDEO_FRAME_INFO_S* pstFrameinfo, HI_U32 u32TimeoutMs)
{
    HI_DRV_VIDEO_FRAME_S stMpi;
    HI_S32  s32Ret = HI_FAILURE;
    HI_U32  u32StartTime = 0;
    HI_U32  u32EndTime = 0;

    if (!pstFrameinfo)
    {
        HI_ERR_WIN("para pstFrameinfo is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    if (HI_SUCCESS != HI_SYS_GetTimeStampMs(&u32StartTime))
    {
        HI_ERR_WIN("GetTimeStampMs Failed\n");
        return HI_ERR_VO_OPERATION_DENIED;
    }
    
    memset((HI_VOID*)&stMpi, 0, sizeof(HI_DRV_VIDEO_FRAME_S));
    
    do
    {
        s32Ret = HI_MPI_WIN_AcquireFrame(hWindow, &stMpi);
        if (HI_SUCCESS == s32Ret)
        {
            Transfer_Frame(pstFrameinfo, &stMpi, HI_FALSE);
            return HI_SUCCESS;
        }

        (HI_VOID)usleep(1 * 1000);
        
        if (HI_SUCCESS != HI_SYS_GetTimeStampMs(&u32EndTime))
        {
            HI_ERR_WIN("GetTimeStampMs Failed\n");
            return s32Ret;
        }
    } while ((u32EndTime - u32StartTime) <= u32TimeoutMs);

    return s32Ret;
}

HI_S32 HI_UNF_VO_ReleaseFrame(HI_HANDLE hWindow, HI_UNF_VIDEO_FRAME_INFO_S* pstFrameinfo)
{
    HI_DRV_VIDEO_FRAME_S stMpi;
    HI_S32 s32Ret;

    if (!pstFrameinfo)
    {
        HI_ERR_WIN("para pstFrameinfo is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    s32Ret = Transfer_Frame(pstFrameinfo, &stMpi, HI_TRUE);
    if (HI_SUCCESS == s32Ret)
    {
        s32Ret = HI_MPI_WIN_ReleaseFrame(hWindow, &stMpi);
    }

    return s32Ret;
}


HI_S32 HI_UNF_VO_SetWindowZorder(HI_HANDLE hWindow, HI_LAYER_ZORDER_E enZFlag)
{
    HI_DRV_DISP_ZORDER_E enZorder;
    HI_S32 s32Ret;

    if (enZFlag >= HI_LAYER_ZORDER_BUTT)
    {
        HI_ERR_WIN("Invalid zorder parameter!\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    Transfe_ZOrder(&enZFlag, &enZorder, HI_TRUE);

    s32Ret = HI_MPI_WIN_SetZorder(hWindow, enZorder);

    return s32Ret;
}

HI_S32 HI_UNF_VO_GetWindowZorder(HI_HANDLE hWindow, HI_U32* pu32Zorder)
{
    HI_S32 s32Ret;
    if (!pu32Zorder)
    {
        HI_ERR_WIN("para pu32Zorder is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }
    s32Ret = HI_MPI_WIN_GetZorder(hWindow, pu32Zorder);

    return s32Ret;
}

HI_S32 HI_UNF_VO_AttachWindow(HI_HANDLE hWindow, HI_HANDLE hSrc)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U8 u8Handle = (hSrc >> 16) & 0xFF;

    if(HI_ID_AVPLAY == u8Handle)
    {
        s32Ret = HI_MPI_AVPLAY_AttachWindow(hSrc, hWindow);
    }
#ifdef HI_VI_SUPPORT
    else if(HI_ID_VI == u8Handle)
    {
        s32Ret = HI_MPI_VI_Attach(hSrc, hWindow);
    }
#endif
    else
    {
        HI_ERR_WIN("invalid handle!\n");
        s32Ret = HI_ERR_VO_INVALID_PARA;
    }

    if (s32Ret)
    {
        HI_ERR_WIN("Vo AttachWindow failed!\n");
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_VO_DetachWindow(HI_HANDLE hWindow, HI_HANDLE hSrc)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U8 u8Handle = (hSrc >> 16) & 0xFF;

    if(HI_ID_AVPLAY == u8Handle)
    {
        s32Ret = HI_MPI_AVPLAY_DetachWindow(hSrc, hWindow);
    }
#ifdef HI_VI_SUPPORT
    else if(HI_ID_VI == u8Handle)
    {
        s32Ret = HI_MPI_VI_Detach(hSrc, hWindow);
    }
#endif
    else
    {
        HI_ERR_WIN("invalid handle!\n");
        s32Ret = HI_ERR_VO_INVALID_PARA;
    }

    if (s32Ret)
    {
       HI_ERR_WIN("Vo AVPLAY_DettachWindow failed!\n");
       return s32Ret;
    }

    return HI_SUCCESS;
}

#if 0
HI_S32 HI_UNF_VO_SetFrostGlassLevel(HI_HANDLE hWindow, HI_UNF_WINDOW_FROST_E enWinFrostMode)
{
    HI_S32 s32Ret = HI_FAILURE;
	HI_DRV_WINDOW_FROST_E eDrvFrostMode = HI_UNF_WINDOW_FROST_CLOSE;
	
    if (HI_UNF_WINDOW_FROST_BUTT <= enWinFrostMode)
    {
        return HI_ERR_VO_INVALID_PARA;
    }

	(HI_VOID)Transfe_FrostMode(&enWinFrostMode, &eDrvFrostMode, HI_TRUE);
    s32Ret = HI_MPI_WIN_SetFrostGlassLevel(hWindow, eDrvFrostMode);
	
    return s32Ret;
}

HI_S32 HI_UNF_VO_GetFrostGlassLevel(HI_HANDLE hWindow, HI_UNF_WINDOW_FROST_E *penWinFrostMode)
{
    HI_S32 s32Ret = HI_FAILURE;
	HI_DRV_WINDOW_FROST_E eDrvFrostMode = HI_UNF_WINDOW_FROST_CLOSE;
	
    if (HI_NULL == penWinFrostMode)
    {
        return HI_ERR_VO_NULL_PTR;
    }
	
    s32Ret = HI_MPI_WIN_GetFrostGlassLevel(hWindow, &eDrvFrostMode);
	if (HI_SUCCESS == s32Ret)
	{
		(HI_VOID)Transfe_FrostMode(penWinFrostMode, &eDrvFrostMode, HI_FALSE);
	}
	
    return s32Ret;
}
#endif

HI_S32 HI_UNF_VO_FreezeWindow(HI_HANDLE hWindow, HI_BOOL bEnable, HI_UNF_WINDOW_FREEZE_MODE_E enWinFreezeMode)
{
    HI_DRV_WIN_SWITCH_E eFrzMode;
    HI_S32 s32Ret;

    Transfe_SwitchMode(&enWinFreezeMode, &eFrzMode, HI_TRUE);
    s32Ret = HI_MPI_WIN_Freeze(hWindow, bEnable, eFrzMode);

    return s32Ret;
}

HI_S32 HI_UNF_VO_GetWindowFreezeStatus(HI_HANDLE hWindow, HI_BOOL *pbEnable, HI_UNF_WINDOW_FREEZE_MODE_E *penWinFreezeMode)
{
    HI_DRV_WIN_SWITCH_E eFrzMode;
    HI_S32 s32Ret;

    if ((HI_NULL == pbEnable) || (HI_NULL == penWinFreezeMode))
    {
        HI_ERR_WIN("para is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    s32Ret = HI_MPI_WIN_GetFreezeStat(hWindow, pbEnable, &eFrzMode);
    if (s32Ret == HI_SUCCESS)
    {
        Transfe_SwitchMode(penWinFreezeMode, &eFrzMode, HI_FALSE);
    }

    return s32Ret;
}


HI_S32 HI_UNF_VO_SetWindowFieldMode(HI_HANDLE hWindow, HI_BOOL bEnable)
{
    return HI_ERR_VO_WIN_UNSUPPORT;
}

HI_S32 HI_UNF_VO_GetWindowFieldMode(HI_HANDLE hWindow, HI_BOOL *pbEnable)
{
    return HI_ERR_VO_WIN_UNSUPPORT;
}

HI_S32 HI_UNF_VO_ResetWindow(HI_HANDLE hWindow, HI_UNF_WINDOW_FREEZE_MODE_E enWinFreezeMode)
{
    HI_DRV_WIN_SWITCH_E eRstMode;
    HI_S32 s32Ret;

    if (enWinFreezeMode >= HI_UNF_WINDOW_FREEZE_MODE_BUTT)
    {
        HI_ERR_WIN("para enWinFreezeMode is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }

    Transfe_SwitchMode(&enWinFreezeMode, &eRstMode, HI_TRUE);
    s32Ret = HI_MPI_WIN_Reset(hWindow, eRstMode);

    return s32Ret;
}


HI_S32 HI_UNF_VO_AttachExternBuffer(HI_HANDLE hWindow, HI_UNF_BUFFER_ATTR_S* pstBufAttr)
{
    HI_DRV_VIDEO_BUFFER_POOL_S stBufPool;
    HI_S32 s32Ret;

    memset(&stBufPool, 0, sizeof(HI_DRV_VIDEO_BUFFER_POOL_S));
    if (!pstBufAttr)
    {
        HI_ERR_WIN("para pstBufAttr is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }
    Transfer_BufferPool(pstBufAttr, &stBufPool, HI_TRUE);

    s32Ret = HI_MPI_WIN_SetExtBuffer(hWindow, &stBufPool);

    return s32Ret;
}

HI_S32 HI_UNF_VO_SetQuickOutputEnable(HI_HANDLE hWindow, HI_BOOL bQuickOutputEnable)
{
    HI_S32 s32Ret;

    if ((HI_TRUE != bQuickOutputEnable) && (HI_FALSE != bQuickOutputEnable))
    {
        HI_ERR_WIN("para bQuickOutputEnable is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }
    s32Ret = HI_MPI_WIN_SetQuickOutput(hWindow, bQuickOutputEnable);
    return s32Ret;
}

HI_S32 HI_UNF_VO_GetQuickOutputStatus(HI_HANDLE hWindow, HI_BOOL *pbQuickOutputEnable)
{
    HI_S32 s32Ret;

    if (!pbQuickOutputEnable)
    {
        HI_ERR_WIN("param null ptr.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    s32Ret = HI_MPI_WIN_GetQuickOutputStatus(hWindow, pbQuickOutputEnable);
    return s32Ret;
}

HI_S32 HI_UNF_VO_CapturePicture(HI_HANDLE hWindow, HI_UNF_VIDEO_FRAME_INFO_S* pstCapPicture)
{
    HI_DRV_VIDEO_FRAME_S stMpi;
    HI_S32 s32Ret = HI_SUCCESS;

    if (!pstCapPicture)
    {
        HI_ERR_WIN("para pstCapPicture is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    memset((void*)&stMpi, 0, sizeof(HI_DRV_VIDEO_FRAME_S));
    s32Ret = HI_MPI_WIN_CapturePicture(hWindow, &stMpi);
    if (s32Ret == HI_SUCCESS)
    {
        Transfer_Frame(pstCapPicture, &stMpi, HI_FALSE);
    }

    return s32Ret;
}

HI_S32 HI_UNF_VO_CapturePictureRelease(HI_HANDLE hWindow, HI_UNF_VIDEO_FRAME_INFO_S* pstCapPicture)
{
    HI_DRV_VIDEO_FRAME_S stMpi;
    HI_S32 s32Ret = HI_SUCCESS;

    if (!pstCapPicture)
    {
        HI_ERR_WIN("para pstCapPicture is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    s32Ret = Transfer_Frame(pstCapPicture, &stMpi, HI_TRUE);
    if (HI_SUCCESS == s32Ret)
    {
        s32Ret = HI_MPI_WIN_CapturePictureRelease(hWindow, &stMpi);
    }

    return s32Ret;
}

HI_S32 HI_UNF_VO_SetRotation(HI_HANDLE hWindow, HI_UNF_VO_ROTATION_E enRotation)
{
    HI_DRV_ROT_ANGLE_E eRot = HI_DRV_ROT_ANGLE_BUTT;
    HI_S32 s32Ret;

    s32Ret = Transfe_Rotate(&enRotation, &eRot, HI_TRUE);
    if (s32Ret)
    {
        HI_ERR_WIN("para enRotation is invalid.\n");
        return HI_ERR_VO_INVALID_PARA;
    }
    s32Ret = HI_MPI_WIN_SetRotation(hWindow, eRot);
    return s32Ret;
}

HI_S32 HI_UNF_VO_GetRotation(HI_HANDLE hWindow, HI_UNF_VO_ROTATION_E* penRotation)
{
    HI_DRV_ROT_ANGLE_E eRot;
    HI_S32 s32Ret;
    if (!penRotation)
    {
        HI_ERR_WIN("para penRotation is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }
    s32Ret = HI_MPI_WIN_GetRotation(hWindow, &eRot);
    if (!s32Ret)
    {
        Transfe_Rotate(penRotation, &eRot, HI_FALSE);
    }

    return s32Ret;
}

HI_S32 HI_UNF_VO_SetFlip(HI_HANDLE hWindow, HI_BOOL bHoriFlip, HI_BOOL bVertFlip)
{
    HI_S32 s32Ret;
    s32Ret = HI_MPI_WIN_SetFlip(hWindow, bHoriFlip, bVertFlip);
    return s32Ret;
}

HI_S32 HI_UNF_VO_GetFlip(HI_HANDLE hWindow, HI_BOOL* pbHoriFlip, HI_BOOL* pbVertFlip)
{
    HI_S32 s32Ret;

    if ((!pbVertFlip)  || (!pbHoriFlip))
    {
        HI_ERR_WIN("para is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }
    s32Ret = HI_MPI_WIN_GetFlip(hWindow, pbHoriFlip, pbVertFlip);
    return s32Ret;
}

HI_S32 HI_UNF_VO_SetStereoDetpth(HI_HANDLE hWindow, HI_S32 s32Depth)
{
    return HI_ERR_VO_WIN_UNSUPPORT;
}

HI_S32 HI_UNF_VO_GetStereoDetpth(HI_HANDLE hWindow, HI_S32 *ps32Depth)
{
    return HI_ERR_VO_WIN_UNSUPPORT;
}

HI_S32 HI_UNF_VO_QueueFrame(HI_HANDLE hWindow, HI_UNF_VIDEO_FRAME_INFO_S *pstFrameinfo)
{
    HI_DRV_VIDEO_FRAME_S stMpi;
    HI_S32 s32Ret  = HI_FAILURE;

    if (!pstFrameinfo)
    {
        HI_ERR_WIN("para is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    /*before trans, do  memset first.*/
    memset((HI_VOID*)&stMpi, 0, sizeof(HI_DRV_VIDEO_FRAME_S));

    s32Ret = Transfer_Frame(pstFrameinfo, &stMpi, HI_TRUE);
    if (s32Ret == HI_SUCCESS)
    {
        s32Ret = HI_MPI_WIN_QueueFrame(hWindow, &stMpi);
    }

    return s32Ret;
}

HI_S32 HI_UNF_VO_DeQueueFrame(HI_HANDLE hWindow, HI_UNF_VIDEO_FRAME_INFO_S *pstFrameinfo, HI_U32 u32TimeoutMs)
{
    HI_DRV_VIDEO_FRAME_S stMpi;
    HI_S32  s32Ret  = HI_FAILURE;
    HI_U32  u32StartTime = 0;
    HI_U32  u32EndTime = 0;

    if (!pstFrameinfo)
    {
        HI_ERR_WIN("para is null.\n");
        return HI_ERR_VO_NULL_PTR;
    }

    if (HI_SUCCESS != HI_SYS_GetTimeStampMs(&u32StartTime))
    {
        HI_ERR_WIN("GetTimeStampMs Failed\n");
        return HI_FAILURE;
    }
    
    memset((HI_VOID*)&stMpi, 0, sizeof(HI_DRV_VIDEO_FRAME_S));
    
    do
    {
        s32Ret = HI_MPI_WIN_DequeueFrame(hWindow, &stMpi);
        if (HI_SUCCESS == s32Ret)
        {
            s32Ret = Transfer_Frame(pstFrameinfo, &stMpi, HI_FALSE);
            if (HI_SUCCESS == s32Ret)
            {
                return HI_SUCCESS;
            }
            else
            {
                HI_FATAL_WIN("trans frame infor failed!\n");
                return HI_ERR_VO_FRAME_INFO_ERROR;
            }
        }

        (HI_VOID)usleep(1 * 1000);

        if (HI_SUCCESS != HI_SYS_GetTimeStampMs(&u32EndTime))
        {
            HI_ERR_WIN("GetTimeStampMs Failed\n");
            return HI_FAILURE;
        }
    } while ((u32EndTime - u32StartTime) <= u32TimeoutMs);

    return s32Ret;
}

