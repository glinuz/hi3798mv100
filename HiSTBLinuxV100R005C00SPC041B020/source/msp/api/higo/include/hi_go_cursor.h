/**
 \file
 \brief Describes the header file of the HiGo.CNcomment:Cursor模块头文件 CNend
 \author Shenzhen Hisilicon Co., Ltd.
 \version draft
 \author 1.0
 \date 2008-5-16
 */
 
#ifndef _HI_GO_CURSOR_H
#define _HI_GO_CURSOR_H

#include "hi_go_comm.h"
#include "hi_go_gdev.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif  /*__cplusplus*/
#endif  /*__cplusplus*/

/*************************** Structure Definition ****************************/
/** \addtogroup      HIGO_CURSOR */
/** @{ */  /** <!-- [HIGO_CURSOR] */

typedef struct
{
    HI_HANDLE hCursor;    /**<The maximum resolution is 128x128, and the minimum resolution is 16x16 for the cursor picture. Both pixel alpha and colorkey are supported.*/
    						/**<CNcomment: 光标图片最大128*128,最小16*16像素, 支持像素alpha和colorkey */
    HI_U32    HotspotX;   /**<Horizontal coordinate of the cursor hot spot relative to the cursor picture*//**<CNcomment: 光标热点X坐标，相对于图片 */
    HI_U32    HotspotY;   /**<Vertical coordinate of the cursor+ hot spot relative to the cursor picture*//**<CNcomment: 光标热点Y坐标，相对于图片 */
} HIGO_CURSOR_INFO_S;

/**Status of the cursor layer*//** CNcomment: 光标层状态结构 */
typedef struct 
{
    HI_BOOL bShow;             /**<Whether to display the cursor layer.*//**<CNcomment: 光标层是否显示 */
} HIGO_CURSOR_STATUS_S;
/** @} */  /** <!-- ==== Structure Definition end ==== */

/******************************* API declaration *****************************/
/** \addtogroup      HIGO_CURSOR */
/** @{ */  /** <!-- [HIGO_CURSOR] */

/**
\brief Initializes the cursor module.CNcomment:光标初始化 CNend
\attention \n

\param N/A.CNcomment:无 CNend

\retval ::HI_SUCCESS Success.CNcomment:成功 CNend
\retval ::HI_FAILURE The display fails.CNcomment: 显示失败 CNend
\retval ::HIGO_ERR_INITFAILED
\retval ::HIGO_ERR_DEPEND_CURSOR

\see \n
::HI_GO_DeinitCursor
*/

HI_S32 HI_GO_InitCursor();

/**
\brief Deinitializes the cursor module.CNcomment:去初始化光标初始化 CNend
\attention \n

\param N/A.CNcomment:无 CNend

\retval ::HI_SUCCESS Success.CNcomment:成功 CNend
\retval ::HI_FAILURE The display fails.CNcomment:显示失败 CNend
\retval ::HIGO_ERR_NOTINIT

\see \n
::HI_GO_InitCursor
*/

HI_S32 HI_GO_DeinitCursor();

/**
\brief  Displays the cursor layer.CNcomment:显示光标 CNend
\attention \n
1. The cursor of the output layer is located in the middle of the screen by default.CNcomment:1 输出层的光标位置默认在屏幕中间 CNend
2. You need set the cursor information before the cursor layer is displayed.CNcomment:2.show光标的时候，需要首先设置光标的信息。CNend
\param[in] bVisible Visible status of the cursor layer. HI_TRUE: visible; HI_FASLE: invisible.CNcomment:光标层可见状态， HI_TRUE:可见， HI_FASLE:光标层不可见。CNend

\retval ::HI_SUCCESS Success.CNcomment:成功 CNend
\retval ::HI_FAILURE The display fails.CNcomment:显示失败 CNend
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_NOCURSORINF
\retval ::HIGO_ERR_DEPEND_CURSOR

\see \n
::HI_GO_GetCursorStatus
::HI_GO_SetCursorInfo
*/
HI_S32 HI_GO_ShowCursor(HI_BOOL bVisible);

/** 
\brief Obtains the current status of the cursor layer.CNcomment:获取光标层当前状态 CNend
\attention \n 
N/A.CNcomment:无 CNend

\param[out] pCursorStatus Pointer to the current status of the cursor layer. The value cannot be empty.CNcomment:光标层当前状态结构指针，不可为空 CNend

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_DEPEND_CURSOR

\see \n
::HI_GO_ShowLayer
*/
HI_S32 HI_GO_GetCursorStatus( HIGO_CURSOR_STATUS_S* pCursorStatus);

/**
\brief Moves the cursor to a specified position on a specified screen.CNcomment:移动光标到指定屏幕指定位置 CNend
\attention \n
The cursor position can be changed only when the cursor status is show.CNcomment:必须在cursor为show的情况下才能改变光标位置 CNend

\param[in] x Horizontal coordinate (in pixel) of the cursor on the screen relative to the canvas surface of a 
graphics layer.CNcomment: 屏幕X坐标,单位像素, 相对于图层canvas surface CNend
\param[in] y Vertical coordinate (in pixel) of the cursor on the screen relative to the canvas surface of a graphics 
layer.CNcomment:屏幕Y坐标,单位像素, 相对于图层canvas surface CNend

\retval ::HIGO_ERR_NOTINIT
\retval ::HI_SUCCESS Success.CNcomment:成功 CNend
\retval ::HI_FAILURE The display fails.CNcomment:显示失败 CNend
\retval ::HIGO_ERR_DEPEND_CURSOR 

\see \n
*/
HI_S32 HI_GO_SetCursorPos(HI_S32 x, HI_S32 y);

/**
\brief Obtains the coordinates of the cursor on the screen.CNcomment:取得光标的屏幕坐标 CNend
\attention \n
N/A.CNcomment:无 CNend
\param[out] pX Horizontal coordinate (in pixel) of the cursor on the screen.CNcomment:光标的屏幕X坐标,单位像素 CNend
\param[out] pY Vertical coordinate (in pixel) of the cursor on the screen.CNcomment:光标的屏幕Y坐标,单位像素 CNend

\retval ::HI_SUCCESS Success.CNcomment:成功 CNend
\retval ::HI_FAILURE The display fails.CNcomment:显示失败 CNend
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_DEPEND_CURSOR

\see \n
*/
HI_S32 HI_GO_GetCursorPos(HI_S32* pX, HI_S32* pY);

/**
\brief Sets the hot spot of the displayed cursor picture.CNcomment:设置光标的显示图片的热点 CNend
\attention \n
The minimum resolution is 16x16, and the maximum resolution is 128x128 for the cursor picture. Both colorkey and 
alpha are supported. CNcomment:图片最小16x16,最大128x128像素，支持colorkey, alpha。 CNend
\param[in] pCursorInfo Information about the cursor picture and coordinates of the hot spot.CNcomment:光标图片，热点坐标信息 CNend

\retval ::HI_SUCCESS Success.CNcomment:成功 CNend
\retval ::HI_FAILURE The display fails.CNcomment:显示失败 CNend
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVPARAM
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_INVSURFACESIZE
\retval ::HIGO_ERR_INVPIXELFMT
\retval ::HIGO_ERR_INVHOTSPOT
\retval ::HIGO_ERR_DEPEND_CURSOR

\see \n
*/
HI_S32 HI_GO_SetCursorInfo(const HIGO_CURSOR_INFO_S* pCursorInfo);

/**
\brief Obtains the information about the displayed cursor picture.CNcomment:获取光标的显示图片信息 CNend
\attention \n
\param[in] pCursorInfo Information about the cursor picture and coordinates of the hot spot.CNcomment:光标图片，热点坐标信息 CNend

\retval ::HI_SUCCESS Success.CNcomment:成功 CNend
\retval ::HI_FAILURE The display fails.CNcomment:显示失败 CNend
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_DEPEND_CURSOR

\see \n
*/
HI_S32 HI_GO_GetCursorInfo(HIGO_CURSOR_INFO_S* pCursorInfo);

/**
\brief Attaches the cursor to one or more graphics layers.CNcomment:将光标绑定到图层上，支持一个光标绑定到多个图层上 CNend
\attention \n
The graphics layers to which the cursor is attached must be opened.CNcomment:必须将光标绑定到一个已经open的图层上。CNend
\param[in] hLayer CNcomment:图层ID CNend

\retval ::HI_SUCCESS Layer ID.CNcomment:成功 CNend
\retval ::HI_FAILURE Success.CNcomment:显示失败 CNend
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_INVLAYERID
\retval ::HIGO_ERR_NOTOPEN
\retval ::HIGO_ERR_DEPEND_CURSOR

\see \n
*/
HI_S32  HI_GO_AttchCursorToLayer(HI_HANDLE hLayer);

/**
\brief Detaches the cursor from graphics layers.CNcomment:将光标解绑定图层 CNend
\attention \n
\param[in] hLayer Layer ID.CNcomment:图层ID CNend

\retval ::HI_SUCCESS Success.CNcomment:成功 CNend
\retval ::HI_FAILURE The display fails.CNcomment:显示失败 CNend
\retval ::HIGO_ERR_INVLAYERID
\retval ::HIGO_ERR_DEPEND_CURSOR

\see \n
*/
HI_S32  HI_GO_DetachCursorFromLayer(HI_HANDLE hLayer);
/** @} */  /** <!-- ==== API declaration end ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif  /*__cplusplus*/
#endif  /*__cplusplus*/

#endif
