/******************************************************************************
*
* Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon), 
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name           : tde_osictl.h
Version             : Initial Draft
Author              : 
Created             : 2014/06/20
Description         : 
Function List       :                    
History             :
Date                        Author                  Modification
2014/06/20                                 Created file        
******************************************************************************/
#ifndef  __TDE_OSICTL_H__
#define  __TDE_OSICTL_H__

#include "hi_tde_type.h"
#include "tde_adp.h"
#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif /* __cplusplus */
#endif  /* __cplusplus */

/****************************************************************************/
/*                             TDE osi ctl functions define                 */
/****************************************************************************/

/*****************************************************************************
 Prototype       : TdeOsiOpen
 Description     : open TDE device
 Input           : I_VOID  **
 Output          : None
 Return Value    : 
 Global Variable   
    Read Only    : 
    Read & Write : 
  History         
  1.Date         : 2008/5/26
    Author       : wming
    Modification : Created function

*****************************************************************************/
HI_S32 TdeOsiOpen(HI_VOID);

/*****************************************************************************
 Prototype       : TdeOsiClose
 Description     : close TDE device
 Input           : I_VOID  **
 Output          : None
 Return Value    : 
 Global Variable   
    Read Only    : 
    Read & Write : 
  History         
  1.Date         : 2008/5/26
    Author       : wming
    Modification : Created function

*****************************************************************************/
HI_S32 TdeOsiClose(HI_VOID);

/*****************************************************************************
* Function:      TdeOsiReset
* Description:   reset all states, release all lists
* Input:         none
* Output:        none
* Return:        success / fail
* Others:        none
*****************************************************************************/
HI_VOID TdeOsiReset(HI_VOID);

/*****************************************************************************
* Function:      TdeOsiBeginJob
* Description:   get TDE task handle
* Input:         ps32Handle:get task handle pointer
* Output:        none
* Return:        success / error code
* Others:        none
*****************************************************************************/
HI_S32 TdeOsiBeginJob(TDE_HANDLE *ps32Handle);

/*****************************************************************************
* Function:      TdeOsiEndJob
* Description:   submit TDE task
* Input:         s32Handle: task handle
*                bBlock: if block
*                u32TimeOut: timeout value(unit by 10ms)
*                bSync: if synchronization or not of sumbitting operate
*                pFuncComplCB: callback function of completing operate
* Output:        none
* Return:        success / error code
* Others:        none
*****************************************************************************/
HI_S32 TdeOsiEndJob(TDE_HANDLE s32Handle, HI_BOOL bBlock, HI_U32 u32TimeOut, 
                    HI_BOOL bSync, TDE_FUNC_CB pFuncComplCB, HI_VOID *pFuncPara);

/*****************************************************************************
* Function:      TdeOsiCancelJob
* Description:   delete TDE task created, only effectivly callede before endjob
*                release software resource of list of its task
* Input:         s32Handle: task handle
* Output:        none
* Return:        success / error code
* Others:        none
*****************************************************************************/
HI_S32     TdeOsiCancelJob(TDE_HANDLE s32Handle);

/*****************************************************************************
* Function:      TdeOsiWaitForDone
* Description:   wait for completing of submiting TDE operate
* Input:         s32Handle: task handle
* Output:        none
* Return:        success / error code
* Others:        none
*****************************************************************************/
HI_S32 TdeOsiWaitForDone(TDE_HANDLE s32Handle, HI_U32 u32TimeOut);

/*****************************************************************************
* Function:      TdeOsiWaitAllDone
* Description:   wait for completion of all TDE operate
* Input:         none
* Output:        none
* Return:        success / fail
* Others:        none
*****************************************************************************/
HI_S32 TdeOsiWaitAllDone(HI_BOOL bSync);

/*****************************************************************************
* Function:      TdeOsiQuickCopy
* Description:   quick blit source to target bitmap,no any functional operate, the size of source and target bitmap is the same, pixel format is the same too
* Input:         s32Handle: task handle
*                pSrc: source bitmap info struct
*                pstSrcRect: source bitmap operate rect
*                pDst: target bitmap information struct
*                pstDstRect: target bitmap operate rect
* Output:        none
* Return:        success / error code
* Others:        none
*****************************************************************************/
HI_S32     TdeOsiQuickCopy(TDE_HANDLE s32Handle, TDE2_SURFACE_S* pstSrc, TDE2_RECT_S  *pstSrcRect,
                           TDE2_SURFACE_S* pstDst, TDE2_RECT_S *pstDstRect);

/*****************************************************************************
* Function:      TdeOsiQuickFill
* Description:   quickly fill fixed value to target bitmap
* Input:         s32Handle: task handle
*                pDst: target bitmap info struct
*                pstDstRect: target bitmap operate rect
*                u32FillData: fill data, pixel format is the same as target bitmap format
* Output:        none
* Return:        success / error code
* Others:        none
*****************************************************************************/
HI_S32     TdeOsiQuickFill(TDE_HANDLE s32Handle, TDE2_SURFACE_S* pstDst, TDE2_RECT_S *pstDstRect,
                           HI_U32 u32FillData);

/*****************************************************************************
* Function:      TdeOsiQuickResize
* Description:   zoom the size of source bitmap to the size of target bitmap,
				  source and target are the same bitmap
* Input:         s32Handle: task handle
*                pSrc: source bitmap info struct
*                pstSrcRect: source bitmap operate rect
*                pDst: target bitmap info struct
*                pstDstRect: target bitmap operate rect
* Output:        none
* Return:        success / error code
* Others:        none
*****************************************************************************/
HI_S32     TdeOsiQuickResize(TDE_HANDLE s32Handle, TDE2_SURFACE_S* pstSrc, TDE2_RECT_S  *pstSrcRect,
                             TDE2_SURFACE_S* pstDst, TDE2_RECT_S  *pstDstRect);

/*****************************************************************************
* Function:      TdeOsiQuickFlicker
* Description:   deflicker source bitmap,output to target bitmap,
			     source and target can be the same bitmap
* Input:         s32Handle: task handle
*                pSrc: source bitmap info struct
*                pstSrcRect: source bitmap operate rect
*                pDst: target bitmap info struct
*                pstDstRect: target bitmap operate rect
* Output:        none
* Return:        success / error code
* Others:        none
*****************************************************************************/
HI_S32     TdeOsiQuickFlicker(TDE_HANDLE s32Handle, TDE2_SURFACE_S* pstSrc, TDE2_RECT_S  *pstSrcRect,
                                TDE2_SURFACE_S* pstDst, TDE2_RECT_S  *pstDstRect);




/*****************************************************************************
* Function:      TdeOsiBlit
* Description:   operate pstBackGround with pstForeGround, which result output to pDst, operate setting is in pOpt
* Input:         s32Handle:task handle
*                pstBackGround: background bitmap info struct
*                pstBackGroundRect: background bitmap operate rect
*                pstForeGround: foreground  bitmap infor struct
*                pstForeGroundRect: foreground bitmap operate rects
*                pstDst:  target bitmap infor struct
*                pstDstRect:  target bitmap operate rect
*                pOpt: operate parameter setting struct 
* Output:        none
* Return:        success / error code
* Others:        none
*****************************************************************************/
HI_S32     TdeOsiBlit(TDE_HANDLE s32Handle, TDE2_SURFACE_S* pstBackGround, TDE2_RECT_S  *pstBackGroundRect,
                          TDE2_SURFACE_S* pstForeGround, TDE2_RECT_S  *pstForeGroundRect, TDE2_SURFACE_S* pstDst,
                          TDE2_RECT_S  *pstDstRect, TDE2_OPT_S* pOpt);

/*****************************************************************************
* Function:      TdeOsiMbBlit
* Description:   MB blit interface
* Input:         s32Handle: task handle
*                pstMB: MB inforation struct
*                pstMbRect: Mb bitmap operate rect
*                pstDst: target bitmap info struct
*                pstDstRect: target bitmap operate rect
*                pstMbOpt:  operate parameter setting struct
* Output:        none
* Return:        >0: return task id of current operate ; <0: fail
* Others:        none
*****************************************************************************/
HI_S32     TdeOsiMbBlit(TDE_HANDLE s32Handle, TDE2_MB_S* pstMB, TDE2_RECT_S  *pstMbRect, 
                        TDE2_SURFACE_S* pstDst, TDE2_RECT_S  *pstDstRect, TDE2_MBOPT_S* pstMbOpt);

/*****************************************************************************
* Function:      TdeOsiBitmapMaskRop
* Description:   Ropmask source with mask bitmap at first, and then ropmask source1 with middle bitmap
*                output result into target bitmap
* Input:         s32Handle: task handle
*                pstBackGround: background bitmap info struct
*                pstBackGroundRect: background bitmap operate rect
*                pstForeGround: foreground bitmap info struct
*                pstForeGroundRect: foreground bitmap operate rect
*                pstMask: bitmap info struct of  fake code operate 
*                pstMaskRect: bitmap operate rect of fake code operate
*                pstDst:  target bitmap info struct
*                pstDstRect: target bitmap operate rect
* Output:        none
* Return:        success / error code
* Others:        none
*****************************************************************************/
HI_S32     TdeOsiBitmapMaskRop(TDE_HANDLE s32Handle, 
                        TDE2_SURFACE_S* pstBackGround, TDE2_RECT_S  *pstBackGroundRect,
                        TDE2_SURFACE_S* pstForeGround, TDE2_RECT_S  *pstForGroundRect, 
                        TDE2_SURFACE_S* pstMask, TDE2_RECT_S  *pstMaskRect,
                        TDE2_SURFACE_S* pDst, TDE2_RECT_S  *pstDstRect,
                        TDE2_ROP_CODE_E enRopCode_Color, TDE2_ROP_CODE_E enRopCode_Alpha);

/*****************************************************************************
* Function:      TdeOsiBitmapMaskBlend
* Description:   blendmask source2 with mask bitmap at first, and then blend source1 with middle bitmap
*			     output result to target bitmap               
* Input:         s32Handle: task handle
*                pstBackGround: background bitmap info struct
*                pstBackGroundRect: background bitmap operate rect
*                pstForeGround: foreground bitmap info struct
*                pstForeGroundRect: foreground bitmap operate rect
*                pstMask: bitmap info struct of  fake code operate 
*                pstMaskRect: bitmap operate rect of fake code operate
*                pstDst:  target bitmap info struct
*                pstDstRect: target bitmap operate rect
*                u8Alpha:  alpha value of operate
* Output:        none
* Return:        success / error code
* Others:        none
*****************************************************************************/
HI_S32     TdeOsiBitmapMaskBlend(TDE_HANDLE s32Handle, TDE2_SURFACE_S* pstBackGround, TDE2_RECT_S  *pstBackGroundRect,
                                 TDE2_SURFACE_S* pstForeGround, TDE2_RECT_S  *pstForGroundRect, TDE2_SURFACE_S* pstMask,
                                 TDE2_RECT_S  *pstMaskRect, TDE2_SURFACE_S* pstDst, TDE2_RECT_S  *pstDstRect,
                                 HI_U8 u8Alpha, TDE2_ALUCMD_E enBlendMode);


/*****************************************************************************
* Function:      TdeOsiSolidDraw
* Description:   output result of operate single color value with source bitmap,operate setting is in pOpt
* Input:         s32Handle: task handle
*                pSrc: source bitmap information struct
*                pstDst: target bitmap info struct
*                pstFillColor: color info
*                pstOpt: operate parameter setting struct
* Output:        none
* Return:        success / error code
* Others:        none
*****************************************************************************/
HI_S32  TdeOsiSolidDraw(TDE_HANDLE s32Handle, TDE2_SURFACE_S* pstSrc, TDE2_RECT_S  *pstSrcRect, TDE2_SURFACE_S *pstDst,
                           TDE2_RECT_S  *pstDstRect, TDE2_FILLCOLOR_S *pstFillColor, TDE2_OPT_S *pstOpt);


/*****************************************************************************
* Function:      TdeOsiSetDeflickerLevel
* Description:   set deflicker level
* Input:         eDeflickerLevel: deflicker level
* Output:        none
* Return:        success / error code
* Others:        none
*****************************************************************************/
HI_S32 TdeOsiSetDeflickerLevel(TDE_DEFLICKER_LEVEL_E eDeflickerLevel);

/*****************************************************************************
* Function:      TdeOsiGetDeflickerLevel
* Description:   get deflicker level
* Input:         eDeflickerLevel: deflicker level
* Output:        none
* Return:        success / error code
* Others:        none
*****************************************************************************/
HI_S32 TdeOsiGetDeflickerLevel(TDE_DEFLICKER_LEVEL_E *pDeflickerLevel);

/*****************************************************************************
* Function:      TdeOsiSetAlphaThreshold
* Description:   set alpha threshold value
* Input:         none
* Output:        none
* Return:        success / error code
* Others:        none
*****************************************************************************/
HI_S32 TdeOsiSetAlphaThresholdValue(HI_U8 u8ThresholdValue);

/*****************************************************************************
* Function:      TdeOsiGetAlphaThreshold
* Description:   get alpha threshold value
* Input:         none
* Output:        pu8ThresholdValue: alpha threshold valu
* Return:        success / error code
* Others:        none
*****************************************************************************/
HI_S32 TdeOsiGetAlphaThresholdValue(HI_U8 * pu8ThresholdValue);

/*****************************************************************************
* Function:      TdeOsiSetAlphaThresholdState
* Description:   set alpha threshold switch
* Input:         bEnAlphaThreshold:if open alpha threshold flag or not
* Output:        none
* Return:        success / error code
* Others:        none
*****************************************************************************/
HI_S32 TdeOsiSetAlphaThresholdState(HI_BOOL bEnAlphaThreshold);

/*****************************************************************************
* Function:      TdeOsiGetAlphaThresholdState
* Description:   get alpha threshold switch
* Input:         bEnAlphaThreshold:if open alpha threshold flag or not
* Output:        none
* Return:        success / error code
* Others:        none
*****************************************************************************/
HI_S32 TdeOsiGetAlphaThresholdState(HI_BOOL *pbEnAlphaThreshold);
/*****************************************************************************
* Function:      TdeOsiPatternFill
* Description:   pattern fill
* Input:         none
* Output:        s32Handle:task handle
                 pstBackGround:background bitmap infor
                 pstBackGroundRect:background operate rectangle
                 pstForeGround:foreground bitmap info
                 pstForeGroundRect:foreground bitmap operate rectangle
                 pstDst:target bitmap information
                 pstDstRect:target operate rectangle
                 pstOpt:pattern fill operate option
* Return:        success/error code
* Others:        pattern width is 256 at most, height is not litmited
*****************************************************************************/

HI_S32 TdeOsiPatternFill(TDE_HANDLE s32Handle, TDE2_SURFACE_S * pstBackGround, 
    TDE2_RECT_S * pstBackGroundRect, TDE2_SURFACE_S * pstForeGround, 
    TDE2_RECT_S * pstForeGroundRect, TDE2_SURFACE_S * pstDst, 
    TDE2_RECT_S * pstDstRect, TDE2_PATTERN_FILL_OPT_S *pstOpt);

/*****************************************************************************
* Function:      TdeOsiEnableRegionDeflicker
* Description:   enable/unabel to local deflicker 
* Output:        bRegionDeflicker:deflick enable flag
* Return:        success/error code
* Others:        none
*****************************************************************************/
HI_S32 TdeOsiEnableRegionDeflicker(HI_BOOL bRegionDeflicker);

HI_S32 TdeCalScaleRect(const TDE2_RECT_S* pstSrcRect, const TDE2_RECT_S* pstDstRect,
                                TDE2_RECT_S* pstRectInSrc, TDE2_RECT_S* pstRectInDst);

HI_S32 TdeOsiQuickCopyEx(TDE_HANDLE s32Handle,   TDE2_SURFACE_S* pstSrc, TDE2_RECT_S  *pstSrcRect,
                         TDE2_SURFACE_S* pstDst, TDE2_RECT_S *pstDstRect, HI_BOOL bMMZForSrc,HI_BOOL bMMZForDst);
#ifdef CONFIG_TDE_BLIT_EX  
HI_S32 TdeOsiSingleBlitEx(TDE_HANDLE s32Handle,   TDE2_SURFACE_S* pstSrc, TDE2_RECT_S  *pstSrcRect,
                         TDE2_SURFACE_S* pstDst, TDE2_RECT_S *pstDstRect, TDE2_OPT_S* pstOpt,HI_BOOL bMMZForSrc,HI_BOOL bMMZForDst);
#endif
#ifdef __cplusplus
 #if __cplusplus
}
 #endif /* __cplusplus */
#endif  /* __cplusplus */

#endif  /* _TDE_OSICTL_H_ */
