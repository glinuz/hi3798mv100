/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon), 
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name           : unf_tde.c
Version             : Initial Draft
Author              : 
Created             : 2014/06/20
Description         : 
Function List       :                    
History             :
Date                        Author                  Modification
2014/06/20                  y00181162               Created file        
******************************************************************************/
#ifndef HI_BUILD_IN_BOOT
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/types.h>
#include "hi_drv_tde.h"
#else
#include "tde_osictl.h"
#endif
#include "hi_tde_api.h"
#include "hi_debug.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif  /* __cplusplus */
#endif  /* __cplusplus */
#ifndef HI_BUILD_IN_BOOT
#define TDE_CHECK_FD() \
    do {                                         \
        if (-1 == g_s32TdeFd)                    \
        {                                        \
            return HI_ERR_TDE_DEV_NOT_OPEN;      \
        }                                        \
    } while (0)

static const char *g_pszTdeDevName = "/dev/hi_tde";

static HI_S32 g_s32TdeFd = -1;              /* tde device handle */

static HI_S32 g_s32TDeRef = 0;

#else
#define TDE_CHECK_FD() \
    do {                                         \
        if (0 == g_s32TDeRef)                    \
        {                                        \
            return HI_ERR_TDE_DEV_NOT_OPEN;      \
        }                                        \
    } while (0)
    
static HI_S32 g_s32TDeRef = 0;
extern int  tde_osr_init(void);
#endif/** boot cut by y00181162 **/

#ifndef HI_BUILD_IN_BOOT/** boot cut by y00181162 **/
extern void tde_osr_deinit(void);
#endif/** boot cut by y00181162 **/


#ifndef HI_BUILD_IN_BOOT
static HI_VOID ConvertSurface(TDE2_IOC_SURFACE_S *stSrc,TDE2_SURFACE_S *pstDst)
{
    stSrc->bAlphaExt1555  = pstDst->bAlphaExt1555;
    stSrc->bAlphaMax255   = pstDst->bAlphaMax255;
    stSrc->bYCbCrClut     = pstDst->bYCbCrClut;
    stSrc->enColorFmt     = pstDst->enColorFmt;
    stSrc->u32ClutPhyAddr = (HI_U32)(unsigned long)pstDst->pu8ClutPhyAddr;
    stSrc->u32CbCrPhyAddr = pstDst->u32CbCrPhyAddr;
    stSrc->u32CbCrStride  = pstDst->u32CbCrStride;
    stSrc->u32Height      = pstDst->u32Height;
    stSrc->u32PhyAddr     = pstDst->u32PhyAddr;
    stSrc->u32Stride      = pstDst->u32Stride;
    stSrc->u32Width       = pstDst->u32Width;
    stSrc->u8Alpha0       = pstDst->u8Alpha0;
    stSrc->u8Alpha1       = pstDst->u8Alpha1;
}
#endif


HI_S32 HI_TDE2_Open(HI_VOID)
{
#ifndef HI_BUILD_IN_BOOT
    if (-1 != g_s32TdeFd)
    {
        g_s32TDeRef++;
        return HI_SUCCESS;
    }

    g_s32TdeFd = open(g_pszTdeDevName, O_RDWR, 0);
    if (g_s32TdeFd < 0)
    {
        return HI_ERR_TDE_DEV_OPEN_FAILED;
    }
#else
    if (g_s32TDeRef){
        g_s32TDeRef++;
        return HI_SUCCESS;
    }
    if (tde_osr_init() < 0){
        return HI_ERR_TDE_DEV_OPEN_FAILED;
    }
#endif
    g_s32TDeRef++;
    return HI_SUCCESS;
}


#ifndef HI_BUILD_IN_BOOT/** boot cut by y00181162 **/
HI_VOID HI_TDE2_Close(HI_VOID)
{
#ifndef HI_BUILD_IN_BOOT
   if (-1 == g_s32TdeFd)
    {
        return;
    }
    g_s32TDeRef--;
    
    if(g_s32TDeRef > 0)
    {
        return ;
    }
    else
    {
        g_s32TDeRef = 0;
    }
    
    close(g_s32TdeFd);

    g_s32TdeFd = -1;
#else
    if (g_s32TDeRef == 0){
        return;
    }
    
    g_s32TDeRef--;
    
    if(g_s32TDeRef > 0){
        return ;
    }
    tde_osr_deinit();
#endif
    return;
}
#endif/** boot cut by y00181162 **/


TDE_HANDLE  HI_TDE2_BeginJob(HI_VOID)
{
    TDE_HANDLE s32Handle;

    TDE_CHECK_FD();
    #ifndef HI_BUILD_IN_BOOT
    if (ioctl(g_s32TdeFd, TDE_BEGIN_JOB, &s32Handle) < 0)
    {
        return HI_ERR_TDE_INVALID_HANDLE;
    }
    #else
    if (TdeOsiBeginJob(&s32Handle) < 0)
    {
        return HI_ERR_TDE_INVALID_HANDLE;
    }
    #endif
    return s32Handle;
}

HI_S32 HI_TDE2_Bitblit(TDE_HANDLE s32Handle, TDE2_SURFACE_S * pstBackGround, TDE2_RECT_S * pstBackGroundRect,
                       TDE2_SURFACE_S * pstForeGround, TDE2_RECT_S * pstForeGroundRect, TDE2_SURFACE_S * pstDst,
                       TDE2_RECT_S * pstDstRect,
                       TDE2_OPT_S * pstOpt)
{
    #ifndef HI_BUILD_IN_BOOT
    TDE_BITBLIT_CMD_S stBlitCmd = {0};

    TDE_CHECK_FD();

    stBlitCmd.s32Handle = s32Handle;

    if ((NULL == pstBackGround))
    {
        stBlitCmd.u32NullIndicator = (1 << 1);
    }
    else
    {
        ConvertSurface(&stBlitCmd.stBackGround,pstBackGround);
    }

    if (NULL == pstBackGroundRect)
    {
        stBlitCmd.u32NullIndicator |= (1 << 2);
    }
    else
    {
        memcpy(&stBlitCmd.stBackGroundRect, pstBackGroundRect, sizeof(TDE2_RECT_S));
    }

    if ((NULL == pstForeGround))
    {
        stBlitCmd.u32NullIndicator |= (1 << 3);
    }
    else
    {
        ConvertSurface(&stBlitCmd.stForeGround,pstForeGround);
    }

    if (NULL == pstForeGroundRect)
    {
        stBlitCmd.u32NullIndicator |= (1 << 4);
    }
    else
    {
        memcpy(&stBlitCmd.stForeGroundRect, pstForeGroundRect, sizeof(TDE2_RECT_S));
    }

    if ((NULL == pstDst))
    {
        return HI_ERR_TDE_NULL_PTR;
    }
    else
    {
        ConvertSurface(&stBlitCmd.stDst,pstDst);
    }

    if (NULL == pstDstRect)
    {
        return HI_ERR_TDE_NULL_PTR;
    }
    else
    {
        memcpy(&stBlitCmd.stDstRect, pstDstRect, sizeof(TDE2_RECT_S));
    }

    if (NULL == pstOpt)
    {
        stBlitCmd.u32NullIndicator |= (1 << 7);
    }
    else
    {
        memcpy(&stBlitCmd.stOpt, pstOpt, sizeof(TDE2_OPT_S));
    }

    return ioctl(g_s32TdeFd, TDE_BIT_BLIT, &stBlitCmd);
    #else
    return TdeOsiBlit(s32Handle, pstBackGround, pstBackGroundRect, pstForeGround, pstForeGroundRect, pstDst, pstDstRect, pstOpt);
    #endif
}

#ifndef HI_BUILD_IN_BOOT
HI_S32   HI_TDE2_QuickCopy(TDE_HANDLE s32Handle, TDE2_SURFACE_S* pstSrc, TDE2_RECT_S  *pstSrcRect,
                           TDE2_SURFACE_S* pstDst, TDE2_RECT_S *pstDstRect)
{
    TDE_QUICKCOPY_CMD_S stQuickCopy = {0};

    TDE_CHECK_FD();
    if ((NULL == pstSrc) || (NULL == pstSrcRect) || (NULL == pstDst) || (NULL == pstDstRect))
    {
        return HI_ERR_TDE_NULL_PTR;
    }


    stQuickCopy.s32Handle = s32Handle;    
    ConvertSurface(&stQuickCopy.stSrc,pstSrc);
    ConvertSurface(&stQuickCopy.stDst,pstDst);
    memcpy(&stQuickCopy.stSrcRect, pstSrcRect, sizeof(TDE2_RECT_S));
    memcpy(&stQuickCopy.stDstRect, pstDstRect, sizeof(TDE2_RECT_S));
    return ioctl(g_s32TdeFd, TDE_QUICK_COPY, &stQuickCopy);
}
#endif

/*****************************************************************************
* Function:      HI_TDE2_QuickFill
* Description:   Fill quickly fixed value to target bitmap
* Input:         s32Handle: Task handle
*                pDst: Target bitmap info struct 
*                u32FillData: Fill information, pixel format accord with target bitmap
* Output:        none
* Return:        >0: current task id; <0: Failure
* Others:        none
*****************************************************************************/
HI_S32   HI_TDE2_QuickFill(TDE_HANDLE s32Handle, TDE2_SURFACE_S* pstDst, TDE2_RECT_S *pstDstRect,
                           HI_U32 u32FillData)
{
    TDE_CHECK_FD();
    if ((NULL == pstDst) || (NULL == pstDstRect))
    {
        return HI_ERR_TDE_NULL_PTR;
    }

    #ifndef HI_BUILD_IN_BOOT
    TDE_QUICKFILL_CMD_S stQuickFill = {0};
    ConvertSurface(&stQuickFill.stDst,pstDst);
    memcpy(&stQuickFill.stDstRect, pstDstRect, sizeof(TDE2_RECT_S));
    stQuickFill.s32Handle   = s32Handle;
    stQuickFill.u32FillData = u32FillData;

    return ioctl(g_s32TdeFd, TDE_QUICK_FILL, &stQuickFill);
    #else
     return TdeOsiQuickFill(s32Handle, pstDst, pstDstRect, u32FillData);
    #endif
}

/*****************************************************************************
* Function:      HI_TDE2_QuickResize
* Description:   Zoom source bitmap to the size fixed by target bitmap,source and target bitmap can be the same
* Input:         s32Handle: Task handle
*                pSrc: Source bitmap info struct
*                pDst: Target bitmap info struct
* Output:        None
* Return:        >0: current task id; <0: Faiure
* Others:        None
*****************************************************************************/
#ifndef HI_BUILD_IN_BOOT
HI_S32   HI_TDE2_QuickResize(TDE_HANDLE s32Handle, TDE2_SURFACE_S* pstSrc, TDE2_RECT_S  *pstSrcRect,
                             TDE2_SURFACE_S* pstDst, TDE2_RECT_S  *pstDstRect)
{
    TDE_QUICKRESIZE_CMD_S stQuickResize = {0};

    TDE_CHECK_FD();
    if ((NULL == pstSrc) || (NULL == pstSrcRect) || (NULL == pstDst) || (NULL == pstDstRect))
    {
        return HI_ERR_TDE_NULL_PTR;
    }

    stQuickResize.s32Handle = s32Handle;

    ConvertSurface(&stQuickResize.stSrc,pstSrc);
    ConvertSurface(&stQuickResize.stDst,pstDst);
    memcpy(&stQuickResize.stSrcRect, pstSrcRect, sizeof(TDE2_RECT_S));
    memcpy(&stQuickResize.stDstRect, pstDstRect, sizeof(TDE2_RECT_S));

    return ioctl(g_s32TdeFd, TDE_QUICK_RESIZE, &stQuickResize);
}

/*****************************************************************************
* Function:      HI_TDE2_QuickFlicker
* Description:   Deflicker source bitmap and export to target bitmap, source and target bitmap can be the same
* Input:         s32Handle: Task handle
*                pSrc: Source bitmap info struct
*                pDst: Target bitmap info struct
* Output:        None
* Return:        >0: Current Task id; <0: Failure
* Others:        None
*****************************************************************************/
HI_S32   HI_TDE2_QuickDeflicker(TDE_HANDLE s32Handle, TDE2_SURFACE_S* pstSrc, TDE2_RECT_S  *pstSrcRect,
                                TDE2_SURFACE_S* pstDst, TDE2_RECT_S  *pstDstRect)

{
    TDE_QUICKDEFLICKER_CMD_S stQuickDeflicker = {0};

    TDE_CHECK_FD();
    if ((NULL == pstSrc) || (NULL == pstSrcRect) || (NULL == pstDst) || (NULL == pstDstRect))
    {
        return HI_ERR_TDE_NULL_PTR;
    }

    stQuickDeflicker.s32Handle = s32Handle;
    ConvertSurface(&stQuickDeflicker.stSrc,pstSrc);
    ConvertSurface(&stQuickDeflicker.stDst,pstDst);
    memcpy(&stQuickDeflicker.stSrcRect, pstSrcRect, sizeof(TDE2_RECT_S));
    memcpy(&stQuickDeflicker.stDstRect, pstDstRect, sizeof(TDE2_RECT_S));

    return ioctl(g_s32TdeFd, TDE_QUICK_DEFLICKER, &stQuickDeflicker);
}

/*****************************************************************************
* Function:      HI_TDE2_SolidDraw
* Description:   Operate src1 with src2 and export the result to pDst,set operation in pOpt
*                If src bitmap is mb format, it just support single source, which say is only set either pSrc1 or pSrc2
* Input:         s32Handle: Task handle
*                pSrc: source1 bitmap info struct 
*                pstDst: target bitmap information struct 
*                pstFillColor:  target bitmap info struct
*                pstOpt:  operate argument setting struct
* Output:        none
* Return:        >0: return task id of current operate; <0: fail
* Others:        none
*****************************************************************************/
HI_S32   HI_TDE2_SolidDraw(TDE_HANDLE s32Handle, TDE2_SURFACE_S* pstForeGround, TDE2_RECT_S  *pstForeGroundRect,
                           TDE2_SURFACE_S *pstDst,
                           TDE2_RECT_S  *pstDstRect, TDE2_FILLCOLOR_S *pstFillColor,
                           TDE2_OPT_S *pstOpt)
{
    TDE_SOLIDDRAW_CMD_S stSolidDraw = {0};

    TDE_CHECK_FD();

    stSolidDraw.s32Handle = s32Handle;

    if ((NULL == pstForeGround))
    {
        stSolidDraw.u32NullIndicator = (1 << 1);
    }
    else
    {
        ConvertSurface(&stSolidDraw.stForeGround,pstForeGround);
    }

    if (NULL == pstForeGroundRect)
    {
        stSolidDraw.u32NullIndicator |= (1 << 2);
    }
    else
    {
        memcpy(&stSolidDraw.stForeGroundRect, pstForeGroundRect, sizeof(TDE2_RECT_S));
    }

    if ((NULL == pstDst))
    {
        return HI_ERR_TDE_NULL_PTR;
    }
    else
    {
        ConvertSurface(&stSolidDraw.stDst,pstDst);
    }

    if (NULL == pstDstRect)
    {
        return HI_ERR_TDE_NULL_PTR;
    }
    else
    {
        memcpy(&stSolidDraw.stDstRect, pstDstRect, sizeof(TDE2_RECT_S));
    }

    if (NULL == pstFillColor)
    {
        stSolidDraw.u32NullIndicator |= (1 << 5);
    }
    else
    {
        memcpy(&stSolidDraw.stFillColor, pstFillColor, sizeof(TDE2_FILLCOLOR_S));
    }

    if (NULL == pstOpt)
    {
        stSolidDraw.u32NullIndicator |= (1 << 6);
    }
    else
    {
        memcpy(&stSolidDraw.stOpt, pstOpt, sizeof(TDE2_OPT_S));
    }

    return ioctl(g_s32TdeFd, TDE_SOLID_DRAW, &stSolidDraw);
}
#endif

/*****************************************************************************
* Function:      HI_TDE2_MbBlit
* Description:   MB blit interface
* Input:         s32Handle: task handle
*                pY:    brightness info struct 
*                pCbCr: chroma information struct
*                pDst:  target bitmap inforamtion struct
*                pMbOpt: operate argument setting struct
* Output:        none
* Return:        >0: return task id of current operate; <0: fail
* Others:        none
*****************************************************************************/
HI_S32      HI_TDE2_MbBlit(TDE_HANDLE s32Handle, TDE2_MB_S* pstMB, TDE2_RECT_S  *pstMbRect, TDE2_SURFACE_S* pstDst,
                           TDE2_RECT_S  *pstDstRect,
                           TDE2_MBOPT_S* pstMbOpt)
{
    TDE_CHECK_FD();

    if ((NULL == pstMB) || (NULL == pstDst) || (NULL == pstMbOpt))
    {
        return HI_ERR_TDE_NULL_PTR;
    }
    #ifndef HI_BUILD_IN_BOOT
    TDE_MBBITBLT_CMD_S stMbBlit = {0};
    stMbBlit.s32Handle = s32Handle;
    memcpy(&stMbBlit.stMB, pstMB, sizeof(TDE2_MB_S));
    memcpy(&stMbBlit.stMbRect, pstMbRect, sizeof(TDE2_RECT_S));
    ConvertSurface(&stMbBlit.stDst,pstDst);
    memcpy(&stMbBlit.stDstRect, pstDstRect, sizeof(TDE2_RECT_S));
    memcpy(&stMbBlit.stMbOpt, pstMbOpt, sizeof(TDE2_MBOPT_S));

    return ioctl(g_s32TdeFd, TDE_MB_BITBLT, &stMbBlit);
    #else
    return TdeOsiMbBlit(s32Handle, pstMB, pstMbRect, pstDst, pstDstRect, pstMbOpt);
    #endif
}

/*****************************************************************************
* Function:      HI_TDE2_EndJob
* Description:   submit TDE2 task
* Input:         s32Handle: task handle
*                bSync: if synchronization
*                bBlock: if block
*                u32TimeOut: timeout value(unit by 10ms)
* Output:        none
* Return:        success/fail
* Others:        none
*****************************************************************************/
HI_S32   HI_TDE2_EndJob(TDE_HANDLE s32Handle, HI_BOOL bSync, HI_BOOL bBlock, HI_U32 u32TimeOut)
{
    TDE_CHECK_FD();
    #ifndef HI_BUILD_IN_BOOT
    TDE_ENDJOB_CMD_S stEndJob;
    /* Disable sync function */
    bSync = HI_FALSE;

    stEndJob.s32Handle = s32Handle;
    stEndJob.bSync  = bSync;
    stEndJob.bBlock = bBlock;
    stEndJob.u32TimeOut = u32TimeOut;

    return ioctl(g_s32TdeFd, TDE_END_JOB, &stEndJob);
    #else
    return TdeOsiEndJob(s32Handle, bBlock, u32TimeOut, bSync, HI_NULL, HI_NULL);
    #endif
}

/*****************************************************************************
* Function:      HI_TDE2_WaitForDone
* Description:   wait for completion of sumit TDE2 operate
* Input:         s32Handle: task handle
* Output:        none
* Return:        true: assigned task completed  / false: assigned task not completed 
* Others:        none
*****************************************************************************/
#ifndef HI_BUILD_IN_BOOT
HI_S32 HI_TDE2_WaitForDone(TDE_HANDLE s32Handle)
{
    TDE_CHECK_FD();
    
    return ioctl(g_s32TdeFd, TDE_WAITFORDONE, &s32Handle);
}

/*****************************************************************************
* Function:      HI_TDE2_WaitAllDone
* Description:   wait for all TDE operate if all be done
* Input:         none
* Output:        none
* Return:        success / fail
* Others:        none
*****************************************************************************/
HI_S32 HI_TDE2_WaitAllDone()
{
    TDE_CHECK_FD();

    return ioctl(g_s32TdeFd, TDE_WAITALLDONE);
}

/*****************************************************************************
* Function:      HI_TDE2_Reset
* Description:   reset TDE all states
* Input:         none
* Output:        none
* Return:        success / fail
* Others:        none
*****************************************************************************/
HI_S32 HI_TDE2_Reset(HI_VOID)
{
    TDE_CHECK_FD();

    return ioctl(g_s32TdeFd, TDE_RESET);
}

/*****************************************************************************
* Function:      HI_TDE2_CancelJob
* Description:   delete TDE2 tasks created, effective called before endjob
* Input:         s32Handle: task handle
* Output:        none
* Return:        success / fail
* Others:        none
*****************************************************************************/
HI_S32   HI_TDE2_CancelJob(TDE_HANDLE s32Handle)
{
    TDE_CHECK_FD();

    return ioctl(g_s32TdeFd, TDE_CANCEL_JOB, &s32Handle);
}

/*****************************************************************************
* Function:      HI_TDE2_BitmapMaskRop
* Description:   Ropmask source2 with mask bitmap at first, Ropmask source1 with middle bitmap at then
*                output result into target bitmap.
* Input:         s32Handle: task handle
*                pstBackGround: background bitmap information struct
*                pstBackGroundRect: background bitmap operate rect
*                pstForeGround: foreground bitmap information struct
*                pstForeGroundRect: foreground bitmap operate rect
*                pstMask: bitmap info struct of doing mask operate
*                pstMaskRect: bitmap operate rect of  mask operate
*                pstDst:  target bitmap information struct
*                pstDstRect:  target bitmap operate rect
* Output:        none
* Return:        >0: return task id of current operate; <0: fail
* Others:        none
*****************************************************************************/
HI_S32      HI_TDE2_BitmapMaskRop(TDE_HANDLE s32Handle, 
                                  TDE2_SURFACE_S* pstBackGround, TDE2_RECT_S  *pstBackGroundRect,
                                  TDE2_SURFACE_S* pstForeGround, TDE2_RECT_S  *pstForeGroundRect,
                                  TDE2_SURFACE_S* pstMask, TDE2_RECT_S  *pstMaskRect, 
                                  TDE2_SURFACE_S* pstDst, TDE2_RECT_S  *pstDstRect,
                                  TDE2_ROP_CODE_E enRopCode_Color, TDE2_ROP_CODE_E enRopCode_Alpha)
{
    TDE_BITMAP_MASKROP_CMD_S stBitmapMaskRop;

    TDE_CHECK_FD();
    if ((NULL == pstBackGround) || (NULL == pstBackGroundRect) || (NULL == pstForeGround) || (NULL == pstForeGroundRect)
        || (NULL == pstMask) || (NULL == pstMaskRect) || (NULL == pstDst) || (NULL == pstDstRect))
    {
        return HI_ERR_TDE_NULL_PTR;
    }

    stBitmapMaskRop.s32Handle = s32Handle;
    stBitmapMaskRop.enRopCode_Alpha = enRopCode_Alpha;
    stBitmapMaskRop.enRopCode_Color = enRopCode_Color;
    ConvertSurface(&stBitmapMaskRop.stBackGround,pstBackGround);
    ConvertSurface(&stBitmapMaskRop.stForeGround,pstForeGround);
    ConvertSurface(&stBitmapMaskRop.stMask,pstMask);
    ConvertSurface(&stBitmapMaskRop.stDst,pstDst);
    memcpy(&stBitmapMaskRop.stBackGroundRect, pstBackGroundRect, sizeof(TDE2_RECT_S));
    memcpy(&stBitmapMaskRop.stForeGroundRect, pstForeGroundRect, sizeof(TDE2_RECT_S));
    memcpy(&stBitmapMaskRop.stMaskRect, pstMaskRect, sizeof(TDE2_RECT_S));
    memcpy(&stBitmapMaskRop.stDstRect, pstDstRect, sizeof(TDE2_RECT_S));

    return ioctl(g_s32TdeFd, TDE_BITMAP_MASKROP, &stBitmapMaskRop);
}

/*****************************************************************************
* Function:      HI_TDE2_BitmapMaskBlend
* Description:   blendmask source2 with mask bitmap at first, and then blend source1 with middle bitmap
*                put output result into target bitmap
* Input:         s32Handle: task handle
*                pstBackGround: background bitmap information struct
*                pstBackGroundRect: background bitamp operate rect
*                pstForeGround: foreground bitmap information struct
*                pstForeGroundRect: foreground bitamp operate rect
*                pstMask:  bitmap info struct of doing mask operate
*                pstMaskRect: bitmap operate rect of  mask operate
*                pstDst:  target bitmap information struct
*                pstDstRect:  target bitmap operate rect
*                u8Alpha:  alpha value is operated
* Output:        none
* Return:        >0: return task id of current operate; <0: fail
* Others:        none
*****************************************************************************/
HI_S32   HI_TDE2_BitmapMaskBlend(TDE_HANDLE s32Handle, 
                                 TDE2_SURFACE_S* pstBackGround, TDE2_RECT_S  *pstBackGroundRect,
                                 TDE2_SURFACE_S* pstForeGround, TDE2_RECT_S  *pstForeGroundRect,
                                 TDE2_SURFACE_S* pstMask, TDE2_RECT_S  *pstMaskRect, 
                                 TDE2_SURFACE_S* pstDst, TDE2_RECT_S  *pstDstRect,
                                 HI_U8 u8Alpha, TDE2_ALUCMD_E enBlendMode)
{
    TDE_BITMAP_MASKBLEND_CMD_S stBitmapMaskBlend;

    TDE_CHECK_FD();
    if ((NULL == pstBackGround) || (NULL == pstBackGroundRect) || (NULL == pstForeGround) || (NULL == pstForeGroundRect)
        || (NULL == pstMask) || (NULL == pstMaskRect) || (NULL == pstDst) || (NULL == pstDstRect))
    {
        return HI_ERR_TDE_NULL_PTR;
    }

    if (TDE2_ALUCMD_BLEND != enBlendMode)
    {
        return HI_ERR_TDE_INVALID_PARA;
    }

    stBitmapMaskBlend.s32Handle = s32Handle;
    stBitmapMaskBlend.u8Alpha = u8Alpha;
    stBitmapMaskBlend.enBlendMode = enBlendMode;

    ConvertSurface(&stBitmapMaskBlend.stBackGround,pstBackGround);
    ConvertSurface(&stBitmapMaskBlend.stForeGround,pstForeGround);
    ConvertSurface(&stBitmapMaskBlend.stMask,pstMask);
    ConvertSurface(&stBitmapMaskBlend.stDst,pstDst);
    
    memcpy(&stBitmapMaskBlend.stBackGroundRect, pstBackGroundRect, sizeof(TDE2_RECT_S));
    memcpy(&stBitmapMaskBlend.stForeGroundRect, pstForeGroundRect, sizeof(TDE2_RECT_S));
    memcpy(&stBitmapMaskBlend.stMaskRect, pstMaskRect, sizeof(TDE2_RECT_S));
    memcpy(&stBitmapMaskBlend.stDstRect, pstDstRect, sizeof(TDE2_RECT_S));

    return ioctl(g_s32TdeFd, TDE_BITMAP_MASKBLEND, &stBitmapMaskBlend);
}

HI_S32 HI_TDE2_SetDeflickerLevel(TDE_DEFLICKER_LEVEL_E enDeflickerLevel)
{
    TDE_CHECK_FD();
    if (TDE_DEFLICKER_BUTT <= enDeflickerLevel)
    {
        return HI_ERR_TDE_INVALID_PARA;
    }


    return ioctl(g_s32TdeFd, TDE_SET_DEFLICKERLEVEL, &enDeflickerLevel);
}

HI_S32 HI_TDE2_GetDeflickerLevel(TDE_DEFLICKER_LEVEL_E *pDeflickerLevel)
{
    TDE_CHECK_FD();
    if (HI_NULL == pDeflickerLevel)
    {
        return HI_ERR_TDE_NULL_PTR;
    }
    

    return ioctl(g_s32TdeFd, TDE_GET_DEFLICKERLEVEL, pDeflickerLevel);
}
#endif

HI_S32 HI_TDE2_SetAlphaThresholdValue(HI_U8 u8ThresholdValue)
{
    TDE_CHECK_FD();
    #ifndef HI_BUILD_IN_BOOT
    return ioctl(g_s32TdeFd, TDE_SET_ALPHATHRESHOLD_VALUE, &u8ThresholdValue);
    #else
    return TdeOsiSetAlphaThresholdValue(u8ThresholdValue);
    #endif
}

#ifndef HI_BUILD_IN_BOOT
HI_S32 HI_TDE2_GetAlphaThresholdValue(HI_U8 * pu8ThresholdValue)
{
    TDE_CHECK_FD();
    if (HI_NULL == pu8ThresholdValue)
    {
        return HI_ERR_TDE_NULL_PTR;
    }
    
    return ioctl(g_s32TdeFd, TDE_GET_ALPHATHRESHOLD_VALUE, pu8ThresholdValue);
}
#endif

HI_S32 HI_TDE2_SetAlphaThresholdState(HI_BOOL bEnAlphaThreshold)
{
    TDE_CHECK_FD();
    #ifndef HI_BUILD_IN_BOOT
    return ioctl(g_s32TdeFd, TDE_SET_ALPHATHRESHOLD_STATE, &bEnAlphaThreshold);
    #else
    return TdeOsiSetAlphaThresholdState(bEnAlphaThreshold);
    #endif
}

#ifndef HI_BUILD_IN_BOOT
HI_S32 HI_TDE2_GetAlphaThresholdState(HI_BOOL *p_bEnAlphaThreshold)
{
    TDE_CHECK_FD();

    if (HI_NULL == p_bEnAlphaThreshold)
    {
        return HI_ERR_TDE_NULL_PTR;
    }

    return ioctl(g_s32TdeFd, TDE_GET_ALPHATHRESHOLD_STATE, p_bEnAlphaThreshold);
}

HI_S32 HI_TDE2_PatternFill(TDE_HANDLE s32Handle, 
    TDE2_SURFACE_S * pstBackGround, TDE2_RECT_S * pstBackGroundRect, 
    TDE2_SURFACE_S * pstForeGround, TDE2_RECT_S * pstForeGroundRect, 
    TDE2_SURFACE_S * pstDst, TDE2_RECT_S * pstDstRect, 
    TDE2_PATTERN_FILL_OPT_S *pstOpt)
{
    TDE_PATTERN_FILL_CMD_S stPatternFillCmd = {0};

    TDE_CHECK_FD();
    
    stPatternFillCmd.s32Handle = s32Handle;

    if (HI_NULL == pstBackGround)
    {
        stPatternFillCmd.u32NullIndicator |= (0x1 << 1);
    }
    else
    {
        ConvertSurface(&stPatternFillCmd.stBackGround,pstBackGround);
    }

    if (HI_NULL == pstBackGroundRect)
    {
        stPatternFillCmd.u32NullIndicator |= (0x1 << 2);
    }
    else
    {
        memcpy(&stPatternFillCmd.stBackGroundRect, pstBackGroundRect, sizeof(TDE2_RECT_S));
    }
    if (HI_NULL == pstForeGround)
    {
        stPatternFillCmd.u32NullIndicator |= (0x1 << 3);
    }
    else
    {
        ConvertSurface(&stPatternFillCmd.stForeGround,pstForeGround);
    }

    if (HI_NULL == pstForeGroundRect)
    {
        stPatternFillCmd.u32NullIndicator |= (0x1 << 4);
    }
    else
    {
        memcpy(&stPatternFillCmd.stForeGroundRect, pstForeGroundRect, sizeof(TDE2_RECT_S));
    }
    if (HI_NULL == pstDst)
    {
        stPatternFillCmd.u32NullIndicator |= (0x1 << 5);
    }
    else
    {
        ConvertSurface(&stPatternFillCmd.stDst,pstDst);
    }

    if (HI_NULL == pstDstRect)
    {
        stPatternFillCmd.u32NullIndicator |= (0x1 << 6);
    }
    else
    {
        memcpy(&stPatternFillCmd.stDstRect, pstDstRect, sizeof(TDE2_RECT_S));
    }

    if (HI_NULL == pstOpt)
    {
        stPatternFillCmd.u32NullIndicator |= (0x1 << 7);
    }
    else
    {
        memcpy(&stPatternFillCmd.stOpt, pstOpt, sizeof(TDE2_PATTERN_FILL_OPT_S));
    }

    return ioctl(g_s32TdeFd, TDE_PATTERN_FILL, &stPatternFillCmd);
}

HI_S32 HI_TDE2_EnableRegionDeflicker(HI_BOOL bRegionDeflicker)
{
    return ioctl(g_s32TdeFd, TDE_ENABLE_REGIONDEFLICKER, &bRegionDeflicker);
}

HI_S32 HI_TDE2_MultiBlending(TDE_HANDLE s32Handle, TDE_SURFACE_LIST_S *pstSurfaceList)
{
    HI_S32 i = 0;

    if(HI_ERR_TDE_INVALID_HANDLE == s32Handle)
    {
        return HI_FAILURE;
    }

    if (NULL == pstSurfaceList)
    {
        return HI_FAILURE;
    }

    HI_S32 blitnum = pstSurfaceList->u32SurfaceNum;
    TDE2_SURFACE_S dstSur = {0};
    memcpy(&dstSur, pstSurfaceList->pDstSurface, sizeof(TDE2_SURFACE_S));

    for (i = 0; i < blitnum;  i++)
    {
        TDE2_SURFACE_S srcSur = {0};
        TDE2_RECT_S srcRect = {0};
        TDE2_RECT_S dstRect = {0};
        TDE2_OPT_S stOpt;

        memset(&stOpt, 0, sizeof(TDE2_OPT_S));
        memcpy(&srcSur, &(pstSurfaceList->pstComposor[i].stSrcSurface), sizeof(TDE2_SURFACE_S));
        memcpy(&srcRect, &(pstSurfaceList->pstComposor[i].stInRect), sizeof(TDE2_RECT_S));
        memcpy(&dstRect, &(pstSurfaceList->pstComposor[i].stOutRect), sizeof(TDE2_RECT_S));
        memcpy(&stOpt, &(pstSurfaceList->pstComposor[i].stOpt), sizeof(TDE2_OPT_S));

        HI_S32 s32Ret = HI_TDE2_Bitblit(s32Handle, &dstSur, &dstRect, &srcSur, &srcRect,
                &dstSur, &dstRect, &stOpt);

        if(HI_SUCCESS != s32Ret)
        {
            HI_TDE2_CancelJob(s32Handle);
            return s32Ret;
        }
    }

    return HI_SUCCESS;
}
#endif
#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __cplusplus */

