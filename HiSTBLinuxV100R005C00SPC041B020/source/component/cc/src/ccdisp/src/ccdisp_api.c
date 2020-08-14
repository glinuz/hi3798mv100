/*******************************************************************************
*                       Include files
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cc_debug.h"
#include "hi_unf_cc.h"
#include "ccdisp_def.h"
#include "ccdisp_api.h"
#include <pthread.h>

#define CCDISP_MUTEX_LOCK(x)  \
    do{\
        int ret = pthread_mutex_lock(x);\
        if(ret != 0){\
            HI_ERR_CC("CC call pthread_mutex_lock(DISP) failure,ret = 0x%x\n",ret);\
        }\
    }while(0)

#define CCDISP_MUTEX_UNLOCK(x)  \
    do{\
        int ret = pthread_mutex_unlock(x);\
        if(ret != 0){\
            HI_ERR_CC("CC call pthread_mutex_unlock(DISP) failure, ret = 0x%x\n",ret);\
        }\
    }while(0)

/*****************************************************************************
*                       Macro Definitions
*****************************************************************************/
#define INC(x) if(x < 0) { x = 0;}
#define COLOR2ARGB(a, c) do { \
    (a).u8Alpha = ((c) >> 24) & 0xff; \
    (a).u8Red   = ((c) >> 16) & 0xff; \
    (a).u8Green = ((c) >> 8 ) & 0xff; \
    (a).u8Blue  =  (c)        & 0xff; \
    } while (0)
#define ARGB(a,rgb) (0x00000000==s_u32Opacity[a])||(0x00000000==rgb)?\
        0x00000000:s_u32Opacity[a] |(rgb & 0x00ffffff);

#define ARGB2COLOR(a,c) c=((a).u8Alpha << 24)|((a).u8Red << 16)|((a).u8Green << 8)|((a).u8Blue)

/*****************************************************************************
*                       Type Definitions
*****************************************************************************/
#define INTERLINE_HEIGTH 5
#define MAX_CC_COLUMNS_4B3 32
#define MAX_CC_COLUMNS_16B9 42
/*******************************************************************************
*                       Extern Data Definition
*******************************************************************************/

/*******************************************************************************
*                       Static Data Definition
*******************************************************************************/

                               /*default,   solid,      flash, translucent, transparent */
static HI_U32 s_u32Opacity[5]={0x00000000,0xff000000,0xff000000,0x7f000000,0x00000000};

static CCDisp_Screen_S s_astCCDispScreen[1];
static HI_UNF_CC_RECT_S s_astWindowRect[8];
static CCDISP_FlashText_S * s_astCCDispFlashText[8];
static pthread_mutex_t flashMutex=PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t drawMutex = PTHREAD_MUTEX_INITIALIZER;

/*******************************************************************************
*                       Static Function Prototype
*******************************************************************************/

static CCDisp_Window_S* CCDISP_GetWindowByID(CCDISP_WINDOW_ID_E enWinID)
{
    HI_U32 u32WinID = (HI_U32)enWinID;

    if (u32WinID >= CCDISP_MAX_WINDOWS)
    {
        return HI_NULL;
    }

    return &(s_astCCDispScreen[0].astWindow[u32WinID]);
}

static HI_S32 CCDISP_CheckRect(HI_UNF_CC_RECT_S stMasterRect, HI_UNF_CC_RECT_S stSlaveRect)
{
    if ((stSlaveRect.x < stMasterRect.x-1)
        || (stSlaveRect.x + stSlaveRect.width > stMasterRect.x + stMasterRect.width+2)
        || (stSlaveRect.y < stMasterRect.y-1)
        || (stSlaveRect.y + stSlaveRect.height > stMasterRect.y + stMasterRect.height+2))
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/*******************************************************************************
*                       Export Function Definition
*******************************************************************************/


/*************************************************************************************************
* Description: Initialize the osd system for cc and create the screen with the transparent color*
*              as screen background color.                                                      *
* Arguments:   none                                                                             *
* Return Value: 0 success. Other value when failed(such as size or position isn't correct).     *
* Note:      In the future application. There are may be more than one cc screens in the system.*
*************************************************************************************************/
HI_S32 CCDISP_Init(CCDISP_INIT_PARAM_S *pstInitParam)
{
    if (HI_NULL == pstInitParam)
    {
        HI_ERR_CC("param is error\n");
        return HI_FAILURE;
    }

    memset(&s_astCCDispScreen[0], 0, sizeof(CCDisp_Screen_S));
    s_astCCDispScreen[0].stOpt = pstInitParam->stOpt;

    return HI_SUCCESS;
}

/*************************************************************************************************
 * Description: Deinitialize the cc screen according to the sid. And destroy the osd system if   *
 *              necessary.                                                                       *
 * Arguments: sid: Screen identifier.                                                            *
 * Return value: o success, other value that indicate the error when failed.                     *
 *************************************************************************************************/
HI_S32 CCDISP_DeInit(void)
{
    memset(&s_astCCDispScreen[0], 0, sizeof(CCDisp_Screen_S));
    return HI_SUCCESS;
}


/*************************************************************************************************
 * Description: create the screen with the transparent color as screen background color.         *
 * Arguments: sid:   Screen identifier.                                                          *
 *            p_param: cc screen parameters.                                                     *
 *            list_id: timer list u8Id                                                             *
 * Return Value: 0 success. Other value when failed(such as size or position isn't correct).     *
 * Note:      In the future application. There are may be more than one cc screens in the system.*
 *            after the screen is created the default bVisible attribute is invisible.            *
 *************************************************************************************************/
HI_S32 CCDISP_Screen_SetSize(HI_UNF_CC_RECT_S stScreenRect)
{
    CCDisp_Screen_S *pstScreen = &s_astCCDispScreen[0];

    if (((stScreenRect.x + stScreenRect.width) > CCDISP_SCREEN_MAX_WIDTH)
        || ((stScreenRect.y + stScreenRect.height) > CCDISP_SCREEN_MAX_HEIGHT))
    {
        HI_ERR_CC("stScreenRect(%d %d %d %d) is invalid !\n", stScreenRect.x, stScreenRect.y,
                  stScreenRect.width, stScreenRect.height);
        return HI_FAILURE;
    }

    pstScreen->stScreenRect = stScreenRect;
    pstScreen->u32ScreenColor = HI_UNF_CC_COLOR_BLACK;
    pstScreen->enScreenOpacity = HI_UNF_CC_OPACITY_TRANSPARENT;

    pstScreen->stClipArea.x = pstScreen->stScreenRect.width /10;
    pstScreen->stClipArea.y = pstScreen->stScreenRect.height /10;
    pstScreen->stClipArea.width  = (pstScreen->stScreenRect.width * 8) /10;
    pstScreen->stClipArea.height = (pstScreen->stScreenRect.height * 8) /10;

    return HI_SUCCESS;
}

/*************************************************************************************************
 * Description: Set clip area of screen.                                                                 *
 * Arguments: sid:  screen identifier.                                                           *
 * Return value: 0 success, other value that indicate the error when failed.                     *
 *************************************************************************************************/
HI_S32 CCDISP_Screen_SetClipArea(HI_UNF_CC_RECT_S stClipArea)
{
    CCDisp_Screen_S *pstScreen = &s_astCCDispScreen[0];

    if (HI_FAILURE == CCDISP_CheckRect(pstScreen->stScreenRect, stClipArea))
    {
        HI_ERR_CC("stClipArea(%d %d %d %d) is invalid !\n", stClipArea.x, stClipArea.y,
                  stClipArea.width, stClipArea.height);
    }

    pstScreen->stClipArea = stClipArea;

    return HI_SUCCESS;
}

/*************************************************************************************************
 * Description: disable clip.                                                                    *
 * Arguments: sid: screen identifier.                                                            *
 * Return value: 0 success, other value that indicate the error when failed.                     *
 *************************************************************************************************/
HI_S32 CCDISP_Screen_DisableClip(void)
{
    CCDisp_Screen_S *pstScreen = &s_astCCDispScreen[0];

    /* clip area is the same as screen rect */
    pstScreen->stClipArea = pstScreen->stScreenRect;

    return HI_SUCCESS;
}

HI_S32 CCDISP_Screen_GetColor(HI_U32 *pu32Color, HI_UNF_CC_OPACITY_E *penOpacity)
{
    CCDisp_Screen_S *pstScreen = &s_astCCDispScreen[0];

    if (pu32Color)
    {
        *pu32Color = pstScreen->u32ScreenColor;
    }
    if (penOpacity)
    {
        *penOpacity = pstScreen->enScreenOpacity;
    }

    return HI_SUCCESS;
}

/*************************************************************************************************
 * Description:  Set screen color.                                                                *
 * Arguments:  sid:  screen identifier.                                                          *
 * Return value:  0 success, other value that indicate the error when failed.                    *
 *************************************************************************************************/
HI_S32 CCDISP_Screen_SetColor(HI_U32 u32Color, HI_UNF_CC_OPACITY_E enOpacity)
{
    CCDisp_Screen_S *pstScreen = &s_astCCDispScreen[0];

    pstScreen->u32ScreenColor = u32Color;
    pstScreen->enScreenOpacity = enOpacity;

    return HI_SUCCESS;
}

HI_S32 CCDISP_Screen_GetColorByRGB(HI_UNF_CC_COLOR_S *pstColor)
{
    CCDisp_Screen_S *pstScreen = &s_astCCDispScreen[0];

    if (pstColor)
    {
        *pstColor = pstScreen->stScreenColor;
    }

    return HI_SUCCESS;
}

HI_S32 CCDISP_Screen_SetColorByRGB(HI_UNF_CC_COLOR_S stColor)
{
    CCDisp_Screen_S *pstScreen = &s_astCCDispScreen[0];

    pstScreen->stScreenColor= stColor;

    return HI_SUCCESS;
}

HI_S32 CCDISP_Screen_Clear(void)
{
    CCDisp_Screen_S *pstScreen = &s_astCCDispScreen[0];
    HI_U32 u32RGB = pstScreen->u32ScreenColor;
    HI_UNF_CC_OPACITY_E enOpacity = pstScreen->enScreenOpacity;
    HI_U32 u32Color = 0;
    HI_UNF_CC_DISPLAY_PARAM_S stDispParam;

    DISP_TRACE("color:0x%x,opa:%s\n",u32RGB,DBG_OPA_OUT[enOpacity]);

    stDispParam.enOpt = HI_UNF_CC_OPT_FILLRECT;
    stDispParam.stRect = pstScreen->stScreenRect;
    stDispParam.u32DisplayWidth = pstScreen->stScreenRect.width;
    stDispParam.u32DisplayHeight = pstScreen->stScreenRect.height;
    u32Color=ARGB(enOpacity,u32RGB);
    COLOR2ARGB(stDispParam.unDispParam.stFillRect.stColor, u32Color);

    if (pstScreen->stOpt.pfnDisplay)
    {
        CCDISP_MUTEX_LOCK(&drawMutex);
        (HI_VOID)pstScreen->stOpt.pfnDisplay(pstScreen->stOpt.pUserData, &stDispParam);
        CCDISP_MUTEX_UNLOCK(&drawMutex);
    }

    return HI_SUCCESS;
}

/*************************************************************************************************
 * Description:  Create a window.                                                                *
 * Arguments:  sid:  screen identifier.                                                          *
 * Return value:  0 success, other value that indicate the error when failed.                    *
 *************************************************************************************************/
HI_S32 CCDISP_Window_Create(CCDISP_WINDOW_ID_E enWinID, HI_UNF_CC_RECT_S stWinRect)
{
    CCDisp_Screen_S *pstScreen = &s_astCCDispScreen[0];
    CCDisp_Window_S *pstWindow = CCDISP_GetWindowByID(enWinID);

    if(HI_NULL == pstWindow )
    {
        HI_ERR_CC("enWinID : %d is invalid !\n", enWinID);
        return HI_FAILURE;
    }

    if (HI_FAILURE == CCDISP_CheckRect(pstScreen->stClipArea, stWinRect))
    {
        HI_ERR_CC("stWinRect(%d %d %d %d) is invalid ! screen(%d %d %d %d)\n", stWinRect.x, stWinRect.y,
                  stWinRect.width, stWinRect.height, pstScreen->stClipArea.x, pstScreen->stClipArea.y,
                  pstScreen->stClipArea.width, pstScreen->stClipArea.height);
    }

    memset(pstWindow, 0, sizeof(CCDisp_Window_S));
    pstWindow->enWinID            = enWinID;
    pstWindow->bVisible           = HI_TRUE;
    pstWindow->stWinRect          = stWinRect;
    pstWindow->u32WinColor         = HI_UNF_CC_COLOR_BLACK;
    pstWindow->enWinOpacity       = HI_UNF_CC_OPACITY_TRANSPARENT;
    pstWindow->stText.u32FgColor   = HI_UNF_CC_COLOR_WHITE;
    pstWindow->stText.enFgOpacity = HI_UNF_CC_OPACITY_SOLID;
    pstWindow->stText.u32BgColor   = HI_UNF_CC_COLOR_BLACK;
    pstWindow->stText.enBgOpacity = HI_UNF_CC_OPACITY_SOLID;
    pstWindow->stText.enFontName  = HI_UNF_CC_FN_DEFAULT;
    pstWindow->stText.enFontStyle = HI_UNF_CC_FONTSTYLE_NORMAL;
    pstWindow->stText.enFontSize  = HI_UNF_CC_FONTSIZE_STANDARD;

    return HI_SUCCESS;
}

/*************************************************************************************************
 * Description:  Create a window.                                                                *
 * Arguments:  sid:  screen identifier.                                                          *
 * Return value:  0 success, other value that indicate the error when failed.                    *
 *************************************************************************************************/
HI_S32 CCDISP_Window_Destroy(CCDISP_WINDOW_ID_E enWinID)
{
    CCDisp_Window_S *pstWindow = CCDISP_GetWindowByID(enWinID);

    if(HI_NULL == pstWindow)
    {
        HI_ERR_CC("enWinID : %d is invalid !\n", enWinID);
        return HI_FAILURE;
    }

    memset(pstWindow, 0, sizeof(CCDisp_Window_S));
    pstWindow->bVisible = HI_FALSE;

    return HI_SUCCESS;
}


HI_S32 CCDISP_Window_Show(CCDISP_WINDOW_ID_E enWinID)
{
    CCDisp_Window_S *pstWindow = CCDISP_GetWindowByID(enWinID);

    if(HI_NULL == pstWindow)
    {
        HI_ERR_CC("enWinID : %d is invalid !\n", enWinID);
        return HI_FAILURE;
    }

    pstWindow->bVisible = HI_TRUE;

    return HI_SUCCESS;
}

HI_S32 CCDISP_Window_Hide(CCDISP_WINDOW_ID_E enWinID)
{
    CCDisp_Window_S *pstWindow = CCDISP_GetWindowByID(enWinID);

    if(HI_NULL == pstWindow)
    {
        HI_ERR_CC("enWinID : %d is invalid !\n", enWinID);
        return HI_FAILURE;
    }

    pstWindow->bVisible = HI_FALSE;

    return HI_SUCCESS;
}

HI_S32 CCDISP_Window_GetSize(CCDISP_WINDOW_ID_E enWinID, HI_UNF_CC_RECT_S *pstWinRect)
{
    CCDisp_Window_S *pstWindow = CCDISP_GetWindowByID(enWinID);

    if(HI_NULL == pstWindow)
    {
        HI_ERR_CC("enWinID : %d is invalid !\n", enWinID);
        return HI_FAILURE;
    }

    if (pstWinRect)
    {
        *pstWinRect = pstWindow->stWinRect;
    }

    return HI_SUCCESS;
}

HI_S32 CCDISP_Window_SetSize(CCDISP_WINDOW_ID_E enWinID, HI_UNF_CC_RECT_S stWinRect)
{
    CCDisp_Screen_S *pstScreen = &s_astCCDispScreen[0];
    CCDisp_Window_S *pstWindow = CCDISP_GetWindowByID(enWinID);

    if(HI_NULL == pstWindow)
    {
        HI_ERR_CC("enWinID : %d is invalid !\n", enWinID);
        return HI_FAILURE;
    }

    if (HI_FAILURE == CCDISP_CheckRect(pstScreen->stClipArea, stWinRect))
    {
        HI_ERR_CC("stWinRect(%d %d %d %d) is invalid !\n", stWinRect.x, stWinRect.y,
                  stWinRect.width, stWinRect.height);
    }

    pstWindow->stWinRect = stWinRect;

    return HI_SUCCESS;
}

HI_S32 CCDISP_Window_GetColor(CCDISP_WINDOW_ID_E enWinID, HI_U32*pu32WinColor, HI_UNF_CC_OPACITY_E *penWinOpacity)
{
    CCDisp_Window_S *pstWindow = CCDISP_GetWindowByID(enWinID);

    if(HI_NULL == pstWindow)
    {
        HI_ERR_CC("enWinID : %d is invalid !\n", enWinID);
        return HI_FAILURE;
    }

    if (pu32WinColor)
    {
        *pu32WinColor = pstWindow->u32WinColor;
    }

    if (penWinOpacity)
    {
        *penWinOpacity = pstWindow->enWinOpacity;
    }

    return HI_SUCCESS;
}

/*************************************************************************************************
 * Description: Set the dtv708 window color and opacity of the cc display.                       *
 * Arguments: sid: screen identifier.                                                            *
 *              color: the window color.                                                         *
 *              Opacity: the window opacity.                                                     *
 * Return value: 0 success, other value that indicate the error when failed.                     *
 * Note:        When rollup the bottom area will be filled with this color      DTV708 SPECIFIC  *
 *************************************************************************************************/
HI_S32 CCDISP_Window_SetColor(CCDISP_WINDOW_ID_E enWinID, HI_U32 u32WinColor, HI_UNF_CC_OPACITY_E enWinOpacity)
{
    CCDisp_Window_S *pstWindow = CCDISP_GetWindowByID(enWinID);

    if(HI_NULL == pstWindow)
    {
        HI_ERR_CC("enWinID : %d is invalid !\n", enWinID);
        return HI_FAILURE;
    }

    pstWindow->u32WinColor = u32WinColor;
    pstWindow->enWinOpacity = enWinOpacity;

    return HI_SUCCESS;
}

HI_S32 CCDISP_Window_GetColorByRGB(CCDISP_WINDOW_ID_E enWinID, HI_UNF_CC_COLOR_S *pstWinColor)
{
    CCDisp_Window_S *pstWindow = CCDISP_GetWindowByID(enWinID);

    if(HI_NULL == pstWindow)
    {
        HI_ERR_CC("enWinID : %d is invalid !\n", enWinID);
        return HI_FAILURE;
    }

    if (pstWinColor)
    {
        *pstWinColor = pstWindow->stWinColor;
    }

    return HI_SUCCESS;
}

HI_S32 CCDISP_Window_SetColorByRGB(CCDISP_WINDOW_ID_E enWinID, HI_UNF_CC_COLOR_S stWinColor)
{
    CCDisp_Window_S *pstWindow = CCDISP_GetWindowByID(enWinID);

    if(HI_NULL == pstWindow)
    {
        HI_ERR_CC("enWinID : %d is invalid !\n", enWinID);
        return HI_FAILURE;
    }

    pstWindow->stWinColor = stWinColor;

    return HI_SUCCESS;
}

HI_S32 CCDISP_Window_Clear(CCDISP_WINDOW_ID_E enWinID)
{
    CCDisp_Screen_S *pstScreen = &s_astCCDispScreen[0];
    CCDisp_Window_S *pstWindow = CCDISP_GetWindowByID(enWinID);
    HI_U32 u32RGB = HI_UNF_CC_COLOR_DEFAULT;
    HI_UNF_CC_OPACITY_E enOpacity = HI_UNF_CC_OPACITY_DEFAULT;
    HI_U32 u32Color = 0;
    HI_UNF_CC_DISPLAY_PARAM_S stDispParam;

    if(HI_NULL == pstWindow)
    {
        HI_ERR_CC("enWinID : %d is invalid !\n", enWinID);
        return HI_FAILURE;
    }

    u32RGB = pstWindow->u32WinColor;
    enOpacity = pstWindow->enWinOpacity;

    if( HI_FALSE == pstWindow->bVisible )
    {
        return HI_SUCCESS;
    }

    stDispParam.enOpt = HI_UNF_CC_OPT_FILLRECT;
    stDispParam.stRect = pstWindow->stWinRect;
    stDispParam.u32DisplayWidth = pstScreen->stScreenRect.width;
    stDispParam.u32DisplayHeight = pstScreen->stScreenRect.height;
    u32Color = ARGB(enOpacity,u32RGB);
    COLOR2ARGB(stDispParam.unDispParam.stFillRect.stColor, u32Color);

    if (pstScreen->stOpt.pfnDisplay)
    {
        CCDISP_MUTEX_LOCK(&drawMutex);
        (HI_VOID)pstScreen->stOpt.pfnDisplay(pstScreen->stOpt.pUserData, &stDispParam);
        CCDISP_MUTEX_UNLOCK(&drawMutex);
    }

    return HI_SUCCESS;
}

/*************************************************************************************************
* Description:  Fill the rectangle with the system foreground color.                            *
* Arguments:  sid: screen identifier.                                                           *
*          isClear: is this function called to clear the area                                   *
*                 if this variable is 1. we will use the screen color to fill                   *
*                 0 means to use the win foreground color to fill                            *
*                 2 means fill rect with background color                                       *
*                 3 means fill rect with text foreground color                                  *
*          bOffScreen: if the rectangle draws on off screen buffer                              *
*           rect: The rectangle that to be filled with system foreground color.                  *
*           flag: 1    means    608                                  *
*                 2    means    708                                            *
*                 3    means    arib                                          *
* Return value:  0 success, other value that indicate the error when failed.                    *
*************************************************************************************************/
HI_S32 CCDISP_Window_FillRect(CCDISP_WINDOW_ID_E enWinID, HI_U32 u32RGB,
                              HI_UNF_CC_OPACITY_E enOpacity, HI_UNF_CC_RECT_S stRect,HI_U32 flag)
{
    HI_U32 u32Color = 0;
    HI_UNF_CC_COLOR_S stColor;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_UNF_CC_RECT_S stFillRect = stRect ;

    u32Color =ARGB(enOpacity,u32RGB);
    COLOR2ARGB(stColor, u32Color);

    s32Ret = CCDISP_Window_FillRectByRGB(enWinID,stColor,stFillRect,flag);
    
    return s32Ret;
}

HI_S32 CCDISP_Window_FillRectByRGB(CCDISP_WINDOW_ID_E enWinID, HI_UNF_CC_COLOR_S stColor, HI_UNF_CC_RECT_S stRect,HI_U32 flag)
{
    CCDisp_Screen_S *pstScreen = &s_astCCDispScreen[0];
    CCDisp_Window_S *pstWindow = CCDISP_GetWindowByID(enWinID);
    HI_UNF_CC_DISPLAY_PARAM_S stDispParam;

    if(HI_NULL == pstWindow)
    {
        HI_ERR_CC("enWinID : %d is invalid !\n", enWinID);
        return HI_FAILURE;
    }

    if( HI_FALSE == pstWindow->bVisible )
    {
        return HI_SUCCESS;
    }
    if (0 == stRect.width || 0 == stRect.height)
    {
        return HI_SUCCESS;
    }

    DISP_TRACE("FillRect, rect[%d %d %d %d], Color : [%d %d %d %d]\n",
               stRect.x,stRect.y,stRect.width,stRect.height,stColor.u8Alpha,stColor.u8Red,stColor.u8Green,stColor.u8Blue);

    if (HI_FAILURE == CCDISP_CheckRect(pstWindow->stWinRect, stRect))
    {
        HI_ERR_CC("window rect is (%d %d %d %d)\n", pstWindow->stWinRect.x, pstWindow->stWinRect.y,
                  pstWindow->stWinRect.width, pstWindow->stWinRect.height);
        HI_ERR_CC("stRect(%d %d %d %d) is invalid !\n", stRect.x, stRect.y,
                  stRect.width, stRect.height);
    }

    //record 708 window rect
    if(2 == flag)
    {
        s_astWindowRect[enWinID] = stRect;
    }
    stDispParam.enOpt = HI_UNF_CC_OPT_FILLRECT;
    stDispParam.stRect = stRect;
    stDispParam.u32DisplayWidth = pstScreen->stScreenRect.width;
    stDispParam.u32DisplayHeight = pstScreen->stScreenRect.height;
    stDispParam.unDispParam.stFillRect.stColor = stColor;

    if (pstScreen->stOpt.pfnDisplay)
    {
        CCDISP_MUTEX_LOCK(&drawMutex);
        (HI_VOID)pstScreen->stOpt.pfnDisplay(pstScreen->stOpt.pUserData, &stDispParam);
        CCDISP_MUTEX_UNLOCK(&drawMutex);
    }

    return HI_SUCCESS;
}

/*************************************************************************************************
 * Description:  Block move the memory of screen.                                                *
 * Arguments:  sid: screen identifier.                                                           *
 *              dstx: destination x coordinate.                                                  *
 *              u16DstY: destination y coordinate.                                                  *
 *              width: the width of block move area.                                             *
 *              Height: the height of block move area.                                           *
 *              Srcx: the source x coordinate of block move.                                     *
 *              Srcy: the source y coordinate of block move.                                     *
 * Return value: 0 success, other value that indicate the error when failed.                     *
 * Note:  Please take care of the source destination overlap case. Because our svp-ex chip have  *
 * bugs when the block move area is overlapped when the moving direction is downward.            *
 *************************************************************************************************/
HI_S32 CCDISP_Window_BlockMove(CCDISP_WINDOW_ID_E enWinID, HI_S32 s32DstX, HI_S32 s32DstY,
                               HI_S32 s32Width, HI_S32 s32Height, HI_S32 s32SrcX, HI_S32 s32SrcY)
{
    CCDisp_Screen_S *pstScreen = &s_astCCDispScreen[0];
    CCDisp_Window_S *pstWindow = CCDISP_GetWindowByID(enWinID);
    HI_UNF_CC_RECT_S stSrcRect, stDstRect;

    if(HI_NULL == pstWindow)
    {
        HI_ERR_CC("enWinID : %d is invalid !\n", enWinID);
        return HI_FAILURE;
    }

    if( HI_FALSE == pstWindow->bVisible )
    {
        return HI_SUCCESS;
    }

    INC(s32SrcX);
    INC(s32SrcY);
    INC(s32DstX);
    INC(s32DstY);

    if((s32SrcX + s32Width - 1) > pstWindow->stWinRect.width)
    {
        s32Width = pstWindow->stWinRect.width - s32SrcX;
    }
    if((s32DstY + s32Height - 1) > pstWindow->stWinRect.height)
    {
        s32Height = pstWindow->stWinRect.height - s32DstY;
    }

    if((s32DstX + s32Width - 1) > pstWindow->stWinRect.width)
    {
        s32DstX = pstWindow->stWinRect.width - s32Width;
    }
    if((s32DstY + s32Height - 1) > pstWindow->stWinRect.height)
    {
        s32DstY = pstWindow->stWinRect.height - s32Height;
    }

    stSrcRect.x = s32SrcX;
    stSrcRect.y = s32SrcY;
    stSrcRect.width= s32Width;
    stSrcRect.height= s32Height;

    stDstRect.x = s32DstX;
    stDstRect.y = s32DstY;
    stDstRect.width = s32Width;
    stDstRect.height= s32Height;

    if (pstScreen->stOpt.pfnBlit)
    {
        (HI_VOID)pstScreen->stOpt.pfnBlit(pstScreen->stOpt.pUserData, &stSrcRect, &stDstRect);
    }

    return HI_SUCCESS;
}


/*************************************************************************************************
* Description: rollup the specified area with the speed.
* Arguments: sid: screen identifier.
*              Rect: rollup area.
*              Speed: rollup speed.
* Return value: 0 success, other value that indicate the error when failed.
* Note:    Currently we implement the rollup by continuous block move the area. So the rollup
*         speed parameter indicate the offset step between two times. We will define the speed
*      five level. This function should map the speed to block move offset or real rollup speed.
*************************************************************************************************/
HI_S32 CCDISP_Window_Rollup(CCDISP_WINDOW_ID_E enWinID, HI_U16 x, HI_U16 width,
                            HI_U16 u16StartY, HI_U16 u16EndY, HI_U16 u16Offset, CCDISP_ROLLSPEED_E enSpeed)
{
    HI_S32 s32Ret = HI_SUCCESS;
    CCDisp_Screen_S *pstScreen = &s_astCCDispScreen[0];
    CCDisp_Window_S *pstWindow = CCDISP_GetWindowByID(enWinID);

    HI_S32 s32RollupStep = 0,s32RollupOffset = 0,s32ScrollStep = 0,s32ScrollHeight= 0;
    HI_U16 u16DstY = 0,u16SrcY= 0;
    HI_UNF_CC_RECT_S stSrcRect, stDstRect;
    HI_U32 u32Color = 0;
    HI_UNF_CC_DISPLAY_PARAM_S stDispParam;

    if(HI_NULL == pstWindow)
    {
        HI_ERR_CC("enWinID : %d is invalid !\n", enWinID);
        return HI_FAILURE;
    }

    if( HI_FALSE == pstWindow->bVisible )
    {
        return HI_SUCCESS;
    }

    /*here we should add a function to convert the speed into the rollup step*/

    s32RollupOffset = u16Offset;
    s32ScrollHeight = u16EndY - u16StartY;
    u16DstY = u16StartY;
    s32RollupStep = s32RollupOffset/(enSpeed + 1);

    stDispParam.enOpt = HI_UNF_CC_OPT_FILLRECT;
    stDispParam.u32DisplayWidth = pstScreen->stScreenRect.width;
    stDispParam.u32DisplayHeight = pstScreen->stScreenRect.height;
    u32Color = ARGB(pstWindow->enWinOpacity,pstWindow->u32WinColor);
    COLOR2ARGB(stDispParam.unDispParam.stFillRect.stColor, u32Color);

    while(s32RollupOffset > 0)
    {
        s32ScrollStep = (s32RollupOffset > s32RollupStep) ? s32RollupStep : s32RollupOffset;
        s32RollupOffset -= s32ScrollStep;
        u16SrcY = u16StartY + s32ScrollStep;
        s32ScrollHeight -= s32ScrollStep;

        stSrcRect.x = x;
        stSrcRect.y = u16SrcY;
        stSrcRect.width = width;
        stSrcRect.height = s32ScrollHeight;


        stDstRect.x = x;
        stDstRect.y = u16DstY;
        stDstRect.width = width;
        stDstRect.height = s32ScrollHeight;

        if(pstScreen->stOpt.pfnBlit)
        {
            s32Ret = pstScreen->stOpt.pfnBlit(pstScreen->stOpt.pUserData, &stSrcRect, &stDstRect);
        }

        if(pstScreen->stOpt.pfnDisplay)
        {
            stDstRect.x = x;
            stDstRect.y = u16DstY+s32ScrollHeight;
            stDstRect.width = width;
            stDstRect.height = s32ScrollStep;
            stDispParam.stRect = stDstRect;
            CCDISP_MUTEX_LOCK(&drawMutex);
            s32Ret |= pstScreen->stOpt.pfnDisplay(pstScreen->stOpt.pUserData, &stDispParam);
            CCDISP_MUTEX_UNLOCK(&drawMutex);
        }
        //u16SrcY = u16DstY;
    }

    return s32Ret;
}

/*************************************************************************************************
* Description: Get the text size of string.                                                     *
* Arguments: sid: screen u8Id.                                                                    *
*              Str: string pointer.                                                             *
*              Length: string length.                                                           *
*              pWidth: pointer to return the string width.                                      *
*              pHeight: pointer to return the string height.                                    *
* Return value: 0 success, other value that indicate the error when failed.                     *
* Note:                                                                                         *
*        This function is specific for the dtv708                                               *
*************************************************************************************************/
HI_S32 CCDISP_Text_GetSize(CCDISP_WINDOW_ID_E enWinID, HI_U16 *pu16Str, HI_S32 s32Length,
                           HI_S32 *ps32Width, HI_S32 *ps32Height)
{
    HI_S32 s32Ret = HI_SUCCESS;
    CCDisp_Screen_S *pstScreen = &s_astCCDispScreen[0];
    CCDisp_Window_S *pstWindow = CCDISP_GetWindowByID(enWinID);
    HI_S32 s32Width = 0, s32Height = 0;

    if(HI_NULL == pstWindow)
    {
        HI_ERR_CC("enWinID : %d is invalid !\n", enWinID);
        return HI_FAILURE;
    }

    if(pstScreen->stOpt.pfnGetTextSize)
    {
        s32Ret = pstScreen->stOpt.pfnGetTextSize(pstScreen->stOpt.pUserData, pu16Str, s32Length, &s32Width, &s32Height);
    }
    if (ps32Width)
    {
        *ps32Width = s32Width;
    }
    if (ps32Height)
    {
        *ps32Height = s32Height;
    }

    return s32Ret;
}

/*************************************************************************************************
 * Description: Draw the str on the cc screen at the coordinate(x,y).                            *
 * Arguments: sid: screen identifier.                                                            *
 *              X: x coordinate.                                                                 *
 *              Y: y coordinate.                                                                 *
 *            Str: pointer to the string to draw.(Unicode)                                       *
 *         Length: closed caption length.                                                        *
 *           Flag: reserved.                                                                     *
 *          bOffScreen: if the rectangle draws on off screen buffer                              *
 *          bIgnoreLeftBG: if the left background is not needed                                  *
 *          bTailingSpace: if the tailing space needs to draw or not                             *
 *          bDrawBackground: if the background needs to draw or not                              *
 *          flag : 1 means 608                                                                   *
 *                 2 means 708                                                                   *
 *                 3 means arib                                                                  *
 * Return value: 0 success, other value that indicate the error when failed.                     *
 *************************************************************************************************/
HI_S32 CCDISP_Text_Draw(CCDISP_WINDOW_ID_E enWinID, HI_S32 x, HI_S32 y,
                        HI_U16* pu16Str, HI_S32 s32StrLen, HI_S32* ps32DrawWidth, HI_U32 flag)
{
    HI_S32 s32Ret = HI_SUCCESS;
    CCDisp_Screen_S *pstScreen = &s_astCCDispScreen[0];
    CCDisp_Window_S *pstWindow = CCDISP_GetWindowByID(enWinID);
    HI_S32 s32Width = 0, s32Height = 0;
    HI_UNF_CC_RECT_S stRect;
    HI_U32 u32FgColor = 0, u32BgColor = 0,u32EdgeColor=0;
    HI_UNF_CC_DISPLAY_PARAM_S stDispParam;

    if(HI_NULL == pstWindow)
    {
        HI_ERR_CC("enWinID : %d is invalid !\n", enWinID);
        return HI_FAILURE;
    }
    if( HI_FALSE == pstWindow->bVisible )
    {
        return HI_SUCCESS;
    }
    if(HI_NULL == pu16Str || 0 == s32StrLen)
    {
        HI_ERR_CC("parameter is error\n");
        return HI_FAILURE;
    }

    DISP_TRACE("TextDraw, xy : %d %d, s32Length : %d\n", x,y,s32StrLen);

    if(pstScreen->stOpt.pfnGetTextSize && pstScreen->stOpt.pfnDisplay)
    {
        HI_FLOAT scale;
        HI_U8 rowNum;
        do
        {
            s32Ret = pstScreen->stOpt.pfnGetTextSize(pstScreen->stOpt.pUserData, pu16Str, s32StrLen, &s32Width, &s32Height);
            if (HI_FAILURE == s32Ret)
            {
                HI_ERR_CC("failed to get text size !\n");
                return HI_FAILURE;
            }

            stRect.x = x;
            stRect.y = y;
            stRect.width = s32Width;
            stRect.height = s32Height;

            //if 708,adjust the rect according to the fontsize
            if (2 == flag)
            {

                if (HI_UNF_CC_FONTSIZE_SMALL == pstWindow->stText.enFontSize )
                {
                    scale = 1.0 * MAX_CC_COLUMNS_4B3 / MAX_CC_COLUMNS_16B9;
                }
                else if (HI_UNF_CC_FONTSIZE_LARGE == pstWindow->stText.enFontSize )
                {
                    scale = 1.0 * MAX_CC_COLUMNS_16B9 / MAX_CC_COLUMNS_4B3;
                }
                else
                {
                    scale = 1.0;
                }

                if (1.0 != scale)
                {
                    rowNum = (y - s_astWindowRect[enWinID].y - INTERLINE_HEIGTH) / (s32Height + INTERLINE_HEIGTH);
                    stRect.width = s32Width * scale;
                    stRect.height = s32Height * scale;
                    stRect.x = x;
                    stRect.y = s_astWindowRect[enWinID].y + rowNum * (stRect.height + INTERLINE_HEIGTH) + INTERLINE_HEIGTH;
                }
            }

            s32Ret = CCDISP_CheckRect(pstWindow->stWinRect, stRect);
            if (HI_FAILURE == s32Ret)
            {
                HI_WARN_CC("text(%d %d %d %d) is over range (%d %d %d %d) !\n", stRect.x, stRect.y, stRect.width, stRect.height,
                          pstWindow->stWinRect.x,pstWindow->stWinRect.y,pstWindow->stWinRect.width,pstWindow->stWinRect.height);
                //return HI_FAILURE;

                s32StrLen--;
            }

        }while (HI_FAILURE == s32Ret && s32StrLen);

        stDispParam.enOpt = HI_UNF_CC_OPT_DRAWTEXT;
        stDispParam.stRect = stRect;
        stDispParam.u32DisplayWidth = pstScreen->stScreenRect.width;
        stDispParam.u32DisplayHeight = pstScreen->stScreenRect.height;
        stDispParam.unDispParam.stText.pu16Text = pu16Str;
        stDispParam.unDispParam.stText.u8TextLen = s32StrLen;
        
		if(3 == flag)
		{
              ARGB2COLOR(pstWindow->stText.stFgColor, pstWindow->stText.u32FgColor);
              u32FgColor = ARGB(pstWindow->stText.enFgOpacity,pstWindow->stText.u32FgColor);
              COLOR2ARGB(stDispParam.unDispParam.stText.stFgColor, u32FgColor);

              ARGB2COLOR(pstWindow->stText.stBgColor, pstWindow->stText.u32BgColor);
              u32BgColor = ARGB(pstWindow->stText.enBgOpacity,pstWindow->stText.u32BgColor);
              COLOR2ARGB(stDispParam.unDispParam.stText.stBgColor, u32BgColor);

              ARGB2COLOR(pstWindow->stText.stEdgeColor, pstWindow->stText.u32EdgeColor);
              u32EdgeColor=ARGB(pstWindow->stText.enEdgeOpacity,pstWindow->stText.u32EdgeColor);
              COLOR2ARGB(stDispParam.unDispParam.stText.stEdgeColor, u32EdgeColor);
		}
		else
		{
              u32FgColor = ARGB(pstWindow->stText.enFgOpacity,pstWindow->stText.u32FgColor);
              COLOR2ARGB(stDispParam.unDispParam.stText.stFgColor, u32FgColor);

              u32BgColor = ARGB(pstWindow->stText.enBgOpacity,pstWindow->stText.u32BgColor);
              COLOR2ARGB(stDispParam.unDispParam.stText.stBgColor, u32BgColor);

              u32EdgeColor=ARGB(pstWindow->stText.enEdgeOpacity,pstWindow->stText.u32EdgeColor);
              COLOR2ARGB(stDispParam.unDispParam.stText.stEdgeColor, u32EdgeColor);
		
		}
		
        //u8Justify ?
        //u8WordWrap ?
        stDispParam.unDispParam.stText.enFontStyle = pstWindow->stText.enFontStyle;
        stDispParam.unDispParam.stText.enFontSize = pstWindow->stText.enFontSize;
        //now arib do not support large font,fix later
        if(3==flag)
        {
                if (HI_UNF_CC_FONTSIZE_LARGE == pstWindow->stText.enFontSize )
                {
                    stDispParam.unDispParam.stText.enFontSize = HI_UNF_CC_FONTSIZE_STANDARD;
                }
        }
        stDispParam.unDispParam.stText.enEdgetype = pstWindow->stText.enEdgeType;


        //record flash char,just support 708
        if ((2 == flag)&&(pstWindow->stText.enFgOpacity == HI_UNF_CC_OPACITY_FLASH || pstWindow->stText.enBgOpacity == HI_UNF_CC_OPACITY_FLASH))
        {
            CCDISP_FlashText_S* temp = HI_NULL;
            CCDISP_FlashText_S* text = (CCDISP_FlashText_S*)malloc(sizeof(CCDISP_FlashText_S));
            HI_U16 * pu16buf = (HI_U16*)malloc(2*s32StrLen);
            if (HI_NULL == pu16buf)
            {
                HI_ERR_CC("malloc pu16buf failed\n");
                if(HI_NULL!=text)
                {
                    free(text);
                    text = HI_NULL;
                }
            }
            else
            {
                if (HI_NULL == text)
                {
                    free(pu16buf);
                    pu16buf = HI_NULL;
                    HI_ERR_CC("malloc text failed\n");
                }
                else
                {
                    memcpy(pu16buf, pu16Str, s32StrLen * 2);
                    text->pu16Text = pu16buf;
                    text->s32TextLen = s32StrLen;
                    text->stRect = stRect;
                    text->u32DisplayWidth = pstScreen->stScreenRect.width;
                    text->u32DisplayHeight = pstScreen->stScreenRect.height;
                    text->u32FgColor = u32FgColor;
                    text->u32BgColor = u32BgColor;
                    text->enFontSize = pstWindow->stText.enFontSize;
                    if (pstWindow->stText.enFgOpacity == HI_UNF_CC_OPACITY_FLASH)
                    {
                        text->bFgFlash = HI_TRUE;
                    }
                    else
                    {
                        text->bFgFlash = HI_FALSE;
                    }
                    text->next = HI_NULL;

                    //head insert
                    CCDISP_MUTEX_LOCK(&flashMutex);
                    temp = s_astCCDispFlashText[enWinID];
                    s_astCCDispFlashText[enWinID] = text;
                    s_astCCDispFlashText[enWinID]->next = temp;
                    CCDISP_MUTEX_UNLOCK(&flashMutex);
                }
            }
        }

        CCDISP_MUTEX_LOCK(&drawMutex);
        s32Ret = pstScreen->stOpt.pfnDisplay(pstScreen->stOpt.pUserData, &stDispParam);
        CCDISP_MUTEX_UNLOCK(&drawMutex);
        if(HI_SUCCESS == s32Ret && ps32DrawWidth != HI_NULL)
        {
            *ps32DrawWidth = s32Width;
        }
    }

    return s32Ret;
}

HI_S32 CCDISP_Text_DeleteCharFlash(CCDISP_WINDOW_ID_E enWinID)
{
    //delete flash word
    CCDISP_MUTEX_LOCK(&flashMutex);
    CCDISP_FlashText_S * flashText_A=  s_astCCDispFlashText[enWinID];
    CCDISP_FlashText_S * flashText_B = HI_NULL;
    s_astCCDispFlashText[enWinID]  = HI_NULL;
    while(flashText_A != HI_NULL)
    {
        flashText_B = flashText_A;
        flashText_A = flashText_B->next;
        free(flashText_B->pu16Text);
        free(flashText_B);
        flashText_B = HI_NULL;
    }
    CCDISP_MUTEX_UNLOCK(&flashMutex);
    return HI_SUCCESS;
}

HI_S32 _DtvCC_CharFlash(CCDISP_FlashText_S * text,HI_U32 flag)
{
    CCDisp_Screen_S *pstScreen = &s_astCCDispScreen[0];
    HI_UNF_CC_DISPLAY_PARAM_S stDispParam;
    
    stDispParam.stRect = text->stRect;
    stDispParam.u32DisplayWidth = text->u32DisplayWidth;
    stDispParam.u32DisplayHeight = text->u32DisplayHeight;
    
    if( (flag == 0) && (text->bFgFlash))
    {   
         COLOR2ARGB(stDispParam.unDispParam.stFillRect.stColor,text->u32BgColor);
         stDispParam.enOpt = HI_UNF_CC_OPT_FILLRECT;
         CCDISP_MUTEX_LOCK(&drawMutex);
         pstScreen->stOpt.pfnDisplay(pstScreen->stOpt.pUserData, &stDispParam);
         CCDISP_MUTEX_UNLOCK(&drawMutex);
         return HI_SUCCESS;
    }
    else
    {
        COLOR2ARGB(stDispParam.unDispParam.stText.stFgColor,text->u32FgColor);
    }
    if((flag == 0) && (!text->bFgFlash))
    {
         COLOR2ARGB(stDispParam.unDispParam.stText.stBgColor,text->u32FgColor);
    }
    else
    {
         COLOR2ARGB(stDispParam.unDispParam.stText.stBgColor,text->u32BgColor);
    }

    stDispParam.enOpt = HI_UNF_CC_OPT_DRAWTEXT;
    stDispParam.unDispParam.stText.pu16Text = text->pu16Text;
    stDispParam.unDispParam.stText.u8TextLen = text->s32TextLen;
    stDispParam.unDispParam.stText.enFontSize = text->enFontSize;
    
    CCDISP_MUTEX_LOCK(&drawMutex);
    pstScreen->stOpt.pfnDisplay(pstScreen->stOpt.pUserData, &stDispParam);
    CCDISP_MUTEX_UNLOCK(&drawMutex);
    return HI_SUCCESS;
}

HI_S32 CCDISP_Text_CharFlash(HI_VOID)
{
    HI_U32 winCount;
    static HI_U32 count=0;
    CCDISP_MUTEX_LOCK(&flashMutex);
    for( winCount = 0 ; winCount < 8 ; winCount++ )
    {
        if(HI_NULL==s_astCCDispFlashText[winCount])
        {
             continue;
        }
        else
        {
            CCDISP_FlashText_S * flashText = s_astCCDispFlashText[winCount];
            while( flashText != HI_NULL )
            {
                _DtvCC_CharFlash(flashText,count%2);
                flashText = flashText->next;
            }
            count++;
        }
    }
    CCDISP_MUTEX_UNLOCK(&flashMutex);
    return HI_SUCCESS;
}


/*************************************************************************************************
 * Description: Set the text edge type.                                                          *
 * Arguments: sid: screen identifier.                                                            *
 *              Type: cc text edge type.                                                         *
 * Return value: 0 success, other value that indicate the error when failed.                     *
 *************************************************************************************************/
HI_S32 CCDISP_Text_SetEdgeType(CCDISP_WINDOW_ID_E enWinID, HI_UNF_CC_EdgeType_E enEdgeType)
{
    CCDisp_Window_S *pstWindow = CCDISP_GetWindowByID(enWinID);

    if(HI_NULL == pstWindow)
    {
        HI_ERR_CC("enWinID : %d is invalid !\n", enWinID);
        return HI_FAILURE;
    }

    pstWindow->stText.enEdgeType = enEdgeType;

    return HI_SUCCESS;
}

/*************************************************************************************************
 * Description: Set cc text edge color when edge type isn¡¯t CCENONE.                            *
 * Arguments: sid: screen identifier.                                                            *
 *              edgeColor: edge color of cc.                                                     *
 *              opacity  : edge color opacity. normaly SOLID                                     *
 * Return value: 0 success, other value that indicate the error when failed.                     *
 *************************************************************************************************/
HI_S32 CCDISP_Text_SetEdgeColor(CCDISP_WINDOW_ID_E enWinID, HI_U32 u32EdgeColor, HI_UNF_CC_OPACITY_E enEdgeOpacity)
{
    CCDisp_Window_S *pstWindow = CCDISP_GetWindowByID(enWinID);

    if(HI_NULL == pstWindow)
    {
        HI_ERR_CC("enWinID : %d is invalid !\n", enWinID);
        return HI_FAILURE;
    }

    pstWindow->stText.u32EdgeColor = u32EdgeColor;
    pstWindow->stText.enEdgeOpacity = enEdgeOpacity;

    return HI_SUCCESS;
}

HI_S32 CCDISP_Text_SetEdgeColorByRGB(CCDISP_WINDOW_ID_E enWinID, HI_UNF_CC_COLOR_S stEdgeColor)
{
    CCDisp_Window_S *pstWindow = CCDISP_GetWindowByID(enWinID);

    if(HI_NULL == pstWindow)
    {
        HI_ERR_CC("enWinID : %d is invalid !\n", enWinID);
        return HI_FAILURE;
    }

    pstWindow->stText.stEdgeColor = stEdgeColor;

    return HI_SUCCESS;
}

/*************************************************************************************************
 * Description: Set cc text underline attribute.                                                 *
 * Arguments: sid: screen identifier.                                                            *
 *              isUnderline: the cc text underline attribute.                                    *
 * Return value: 0 success, other value that indicate the error when failed.                     *
 *************************************************************************************************/
HI_S32 CCDISP_Text_SetUnderLine(CCDISP_WINDOW_ID_E enWinID, HI_BOOL bUnderline)
{
    CCDisp_Window_S *pstWindow = CCDISP_GetWindowByID(enWinID);

    if(HI_NULL == pstWindow)
    {
        HI_ERR_CC("enWinID : %d is invalid !\n", enWinID);
        return HI_FAILURE;
    }

    if (bUnderline)
    {
        if ((HI_UNF_CC_FONTSTYLE_ITALIC == pstWindow->stText.enFontStyle)||(HI_UNF_CC_FONTSTYLE_ITALIC_UNDERLINE == pstWindow->stText.enFontStyle))
        {
            pstWindow->stText.enFontStyle = HI_UNF_CC_FONTSTYLE_ITALIC_UNDERLINE;
        }
        else
        {
            pstWindow->stText.enFontStyle = HI_UNF_CC_FONTSTYLE_UNDERLINE;
        }
    }
    else
    {
        if ((HI_UNF_CC_FONTSTYLE_ITALIC == pstWindow->stText.enFontStyle)||(HI_UNF_CC_FONTSTYLE_ITALIC_UNDERLINE == pstWindow->stText.enFontStyle))
        {
            pstWindow->stText.enFontStyle = HI_UNF_CC_FONTSTYLE_ITALIC;
        }
        else
        {
            pstWindow->stText.enFontStyle = HI_UNF_CC_FONTSTYLE_NORMAL;
        }
    }

    return HI_SUCCESS;
}

/*************************************************************************************************
* Description: Set cc text italic attribute.                                                    *
* Arguments: sid: screen identifier.                                                            *
*              isItalic: is cc text use italic font.                                            *
* Return value: 0 success, other value that indicate the error when failed.                     *
*************************************************************************************************/
HI_S32 CCDISP_Text_SetItalic(CCDISP_WINDOW_ID_E enWinID, HI_BOOL bItalic)
{
    CCDisp_Window_S *pstWindow = CCDISP_GetWindowByID(enWinID);

    if(HI_NULL == pstWindow)
    {
        HI_ERR_CC("enWinID : %d is invalid !\n", enWinID);
        return HI_FAILURE;
    }

    if (bItalic)
    {
        if ((HI_UNF_CC_FONTSTYLE_UNDERLINE == pstWindow->stText.enFontStyle)||(HI_UNF_CC_FONTSTYLE_ITALIC_UNDERLINE == pstWindow->stText.enFontStyle))
        {
            pstWindow->stText.enFontStyle = HI_UNF_CC_FONTSTYLE_ITALIC_UNDERLINE;
        }
        else
        {
            pstWindow->stText.enFontStyle = HI_UNF_CC_FONTSTYLE_ITALIC;
        }
    }
    else
    {
        if ((HI_UNF_CC_FONTSTYLE_UNDERLINE == pstWindow->stText.enFontStyle)||(HI_UNF_CC_FONTSTYLE_ITALIC_UNDERLINE == pstWindow->stText.enFontStyle))
        {
            pstWindow->stText.enFontStyle = HI_UNF_CC_FONTSTYLE_UNDERLINE;
        }
        else
        {
            pstWindow->stText.enFontStyle = HI_UNF_CC_FONTSTYLE_NORMAL;
        }
    }

    return HI_SUCCESS;
}

/*************************************************************************************************
 * Description: Set cc text flash attribute. specific for cc 608                                 *
 *              The flash attributes is specific to cc text. The background flash is set by color*
 *              the text flash also can be set be color. add this function to be compatible with *
 *              the old code.                                                                    *
 * Arguments: sid: screen identifier.                                                            *
 *              isFlash: is cc text flash.                                                       *
 * Return value: 0 success, other value that indicate the error when failed.                     *
 *************************************************************************************************/
HI_S32 CCDISP_Text_SetFlash(CCDISP_WINDOW_ID_E enWinID, HI_BOOL bFlash)
{
    CCDisp_Window_S *pstWindow = CCDISP_GetWindowByID(enWinID);

    if(HI_NULL == pstWindow)
    {
        HI_ERR_CC("enWinID : %d is invalid !\n", enWinID);
        return HI_FAILURE;
    }

    if (HI_TRUE == bFlash)
    {
        pstWindow->stText.enFgOpacity = HI_UNF_CC_OPACITY_FLASH;
    }

    return HI_SUCCESS;
}

/*************************************************************************************************
 * Description: Set the current font of cc display.                                              *
 * Arguments: sid: screen identifier.                                                            *
 *              Font: the cc display font.                                                       *
 *              ucCCType: 0 CC608, 1 CC708                                                       *
 * Return value: 0 success, other value that indicate the error when failed.                     *
 *************************************************************************************************/
HI_S32 CCDISP_Text_SetFontName(CCDISP_WINDOW_ID_E enWinID, HI_UNF_CC_FONTNAME_E enFontName)
{
    CCDisp_Window_S *pstWindow = CCDISP_GetWindowByID(enWinID);

    if(HI_NULL == pstWindow)
    {
        HI_ERR_CC("enWinID : %d is invalid !\n", enWinID);
        return HI_FAILURE;
    }

    pstWindow->stText.enFontName = enFontName;

    return HI_SUCCESS;
}

/*************************************************************************************************
* Description: Set the current font size.                                                        *
* Arguments: sid: screen identifier.                                                             *
*               Size: CC font size.                                                              *
* Return value: 0 success, other value that indicate the error when failed.                      *
* Note:     The matched real font size of CCFontSize is calculated on cc display initialization  *
*           according to the cc screen size.                                                     *
**************************************************************************************************/
HI_S32 CCDISP_Text_SetFontSize(CCDISP_WINDOW_ID_E enWinID, HI_UNF_CC_FONTSIZE_E enFontSize)
{
    CCDisp_Window_S *pstWindow = CCDISP_GetWindowByID(enWinID);

    if(HI_NULL == pstWindow)
    {
        HI_ERR_CC("enWinID : %d is invalid !\n", enWinID);
        return HI_FAILURE;
    }

    pstWindow->stText.enFontSize = enFontSize;

    return HI_SUCCESS;
}

/*************************************************************************************************
 * Description: Set the background color and opacity of the cc display.                          *
 * Arguments: sid: screen identifier.                                                            *
 *              color: the background color.                                                     *
 *              Opacity: the background opacity.                                                 *
 * Return value: 0 success, other value that indicate the error when failed.                     *
 *************************************************************************************************/
HI_S32 CCDISP_Text_GetFGColor(CCDISP_WINDOW_ID_E enWinID, HI_U32 *pu32FgColor, HI_UNF_CC_OPACITY_E *penFgOpacity)
{
    CCDisp_Window_S *pstWindow = CCDISP_GetWindowByID(enWinID);

    if(HI_NULL == pstWindow)
    {
        HI_ERR_CC("enWinID : %d is invalid !\n", enWinID);
        return HI_FAILURE;
    }

    if (pu32FgColor)
    {
        *pu32FgColor = pstWindow->stText.u32FgColor;
    }
    if (penFgOpacity)
    {
        *penFgOpacity = pstWindow->stText.enFgOpacity;
    }

    return HI_SUCCESS;
}

/*************************************************************************************************
* Description: Set the foreground color and opacity of cc display.                              *
* Arguments: sid: screen identifier.                                                            *
*            color: if over than 0x40 it's standard color. Less than 0x3f real dtv708 color    *
*  Opacity: the opacity of the set color.  The two elements combined together is the real color.*
* Return value: 0 success, other value that indicate the error when failed.                     *
*************************************************************************************************/
HI_S32 CCDISP_Text_SetFGColor(CCDISP_WINDOW_ID_E enWinID, HI_U32 u32FgColor, HI_UNF_CC_OPACITY_E enFgOpacity)
{
    CCDisp_Window_S *pstWindow = CCDISP_GetWindowByID(enWinID);

    if(HI_NULL == pstWindow)
    {
        HI_ERR_CC("enWinID : %d is invalid !\n", enWinID);
        return HI_FAILURE;
    }

    pstWindow->stText.u32FgColor = u32FgColor;
    pstWindow->stText.enFgOpacity = enFgOpacity;

    return HI_SUCCESS;
}


/*************************************************************************************************
 * Description: Get the current background color and opacity of the cc display.                  *
 * Arguments: sid: screen identifier.                                                            *
 *              color: the background color.                                                     *
 *              Opacity: the background opacity.                                                 *
 * Return value: 0 success, other value that indicate the error when failed.                     *
 *************************************************************************************************/
HI_S32 CCDISP_Text_GetBGColor(CCDISP_WINDOW_ID_E enWinID, HI_U32 *pu32BgColor, HI_UNF_CC_OPACITY_E *penBgOpacity)
{
    CCDisp_Window_S *pstWindow = CCDISP_GetWindowByID(enWinID);

    if(HI_NULL == pstWindow)
    {
        HI_ERR_CC("enWinID : %d is invalid !\n", enWinID);
        return HI_FAILURE;
    }

    if (pu32BgColor)
    {
        *pu32BgColor = pstWindow->stText.u32BgColor;
    }
    if (penBgOpacity)
    {
        *penBgOpacity = pstWindow->stText.enBgOpacity;
    }

    return HI_SUCCESS;
}

HI_S32 CCDISP_Text_SetBGColor(CCDISP_WINDOW_ID_E enWinID, HI_U32 u32BgColor, HI_UNF_CC_OPACITY_E enBgOpacity)
{
    CCDisp_Window_S *pstWindow = CCDISP_GetWindowByID(enWinID);

    if(HI_NULL == pstWindow)
    {
        HI_ERR_CC("enWinID : %d is invalid !\n", enWinID);
        return HI_FAILURE;
    }

    pstWindow->stText.u32BgColor = u32BgColor;
    pstWindow->stText.enBgOpacity = enBgOpacity;

    return HI_SUCCESS;
}

HI_S32 CCDISP_Text_GetFGColorByRGB(CCDISP_WINDOW_ID_E enWinID, HI_UNF_CC_COLOR_S *pstFgColor)
{
    CCDisp_Window_S *pstWindow = CCDISP_GetWindowByID(enWinID);

    if(HI_NULL == pstWindow)
    {
        HI_ERR_CC("enWinID : %d is invalid !\n", enWinID);
        return HI_FAILURE;
    }

    if (pstFgColor)
    {
        *pstFgColor = pstWindow->stText.stFgColor;
    }

    return HI_SUCCESS;
}

HI_S32 CCDISP_Text_SetFGColorByRGB(CCDISP_WINDOW_ID_E enWinID, HI_UNF_CC_COLOR_S stFgColor)
{
    CCDisp_Window_S *pstWindow = CCDISP_GetWindowByID(enWinID);

    if(HI_NULL == pstWindow)
    {
        HI_ERR_CC("enWinID : %d is invalid !\n", enWinID);
        return HI_FAILURE;
    }

    pstWindow->stText.stFgColor = stFgColor;

    return HI_SUCCESS;
}

HI_S32 CCDISP_Text_GetBGColorByRGB(CCDISP_WINDOW_ID_E enWinID, HI_UNF_CC_COLOR_S *pstBgColor)
{
    CCDisp_Window_S *pstWindow = CCDISP_GetWindowByID(enWinID);

    if(HI_NULL == pstWindow)
    {
        HI_ERR_CC("enWinID : %d is invalid !\n", enWinID);
        return HI_FAILURE;
    }

    if (pstBgColor)
    {
        *pstBgColor = pstWindow->stText.stBgColor;
    }

    return HI_SUCCESS;
}

HI_S32 CCDISP_Text_SetBGColorByRGB(CCDISP_WINDOW_ID_E enWinID, HI_UNF_CC_COLOR_S stBgColor)
{
    CCDisp_Window_S *pstWindow = CCDISP_GetWindowByID(enWinID);

    if(HI_NULL == pstWindow)
    {
        HI_ERR_CC("enWinID : %d is invalid !\n", enWinID);
        return HI_FAILURE;
    }

    pstWindow->stText.stBgColor = stBgColor;

    return HI_SUCCESS;
}

HI_S32 CCDISP_Bitmap_Draw(CCDISP_WINDOW_ID_E enWinID, HI_UNF_CC_RECT_S stRect, HI_UNF_CC_BITMAP_S *pstBitmap)
{
    CCDisp_Screen_S *pstScreen = &s_astCCDispScreen[0];
    HI_UNF_CC_DISPLAY_PARAM_S stCCdispalyParm;

    if (HI_NULL == pstBitmap)
    {
        HI_ERR_CC("bitmap data is error  !\n");
        return HI_FAILURE;
    }

    if (pstScreen->stOpt.pfnDisplay)
    {
        stCCdispalyParm.enOpt = HI_UNF_CC_OPT_DRAWBITMAP;
        stCCdispalyParm.stRect = stRect;
        stCCdispalyParm.u32DisplayWidth = pstScreen->stScreenRect.width;
        stCCdispalyParm.u32DisplayHeight = pstScreen->stScreenRect.height;
        stCCdispalyParm.unDispParam.stBitmap = *pstBitmap;
        (HI_VOID)pstScreen->stOpt.pfnDisplay(pstScreen->stOpt.pUserData, &stCCdispalyParm);
    }

    return HI_SUCCESS;
}

HI_S32 CCDISP_SendVbiData(HI_UNF_CC_VBI_DADA_S *pstVbiDataField1,HI_UNF_CC_VBI_DADA_S *pstVbiDataField2)
{
    CCDisp_Screen_S *pstScreen = &s_astCCDispScreen[0];

    if (HI_NULL == pstVbiDataField1 ||HI_NULL == pstVbiDataField2 )
    {
        HI_ERR_CC("vbi data is error  !\n");
        return HI_FAILURE;
    }

    if (pstScreen->stOpt.pfnVBIOutput)
    {
        (HI_VOID)pstScreen->stOpt.pfnVBIOutput(pstScreen->stOpt.pUserData, pstVbiDataField1,pstVbiDataField2);
    }

    return HI_SUCCESS;
}

HI_S32 CCDISP_GetCurPts(HI_S64 *ps64CurPts)
{
    CCDisp_Screen_S *pstScreen = &s_astCCDispScreen[0];
    if (HI_NULL == ps64CurPts)
    {
        HI_ERR_CC("Param null  !\n");
        return HI_FAILURE;
    }
    if (pstScreen->stOpt.pfnGetPts)
    {
       (HI_VOID)pstScreen->stOpt.pfnGetPts(pstScreen->stOpt.pUserData, ps64CurPts);
    }
    return HI_SUCCESS;
}

HI_S32 CCDISP_OutputXDSData(HI_U8 u8XDSClass, HI_U8 u8XDSPacketType, HI_U8 *pu8Data, HI_U8 u8Length)
{
    CCDisp_Screen_S *pstScreen = &s_astCCDispScreen[0];

    if (HI_NULL == pu8Data || 0 == u8Length)
    {
        HI_ERR_CC("XDS data is error  !\n");
        return HI_FAILURE;
    }

    if (pstScreen->stOpt.pfnXDSOutput)
    {
        (HI_VOID)pstScreen->stOpt.pfnXDSOutput(u8XDSClass,u8XDSPacketType,pu8Data,u8Length);
    }

    return HI_SUCCESS;
}

