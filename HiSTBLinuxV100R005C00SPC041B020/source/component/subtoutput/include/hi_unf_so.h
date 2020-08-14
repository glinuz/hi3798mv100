/**
 \file
 \brief Subtitle output Module
 \author Shenzhen Hisilicon Co., Ltd.
 \version 1.0
 \author
 \date 2010-03-10
 */

#ifndef __HI_UNF_SO_H__
#define __HI_UNF_SO_H__

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/*************************** Structure Definition ****************************/
/** \addtogroup     SUBTITLEOUT */
/** @{ */  /** <!-- 【SUBTITLEOUT】 */

/** Number of palette *//** CNcomment:调色板颜色个数 */
#define HI_UNF_SO_PALETTE_ENTRY (256)

/** Invalid pts value *//** CNcomment:无效的PTS值 */
#define HI_UNF_SO_NO_PTS        (-1)

/** The max node in SO *//** CNcomment:SO能缓存的最大节点数 */
#define HI_UNF_SO_MAX_NODE_NUM  (20)

/** The max buffer size *//** CNcomment:SO最大的缓存长度 */
#define HI_UNF_SO_MAX_BUFFER_SIZE (0x200000)


/** Subtitle type *//** CNcomment:字幕类型 */
typedef enum hiUNF_SO_SUBTITLE_TYPE_E
{
    HI_UNF_SUBTITLE_BITMAP = 0x0,  /**<Bmp subtitle *//**<CNcomment:图像字幕 */
    HI_UNF_SUBTITLE_TEXT,          /**<Text subtitle *//**<CNcomment:string字幕 */
    HI_UNF_SUBTITLE_ASS,           /**<Ass/ssa sbutitle *//**<CNcomment:ass字幕 */
    HI_UNF_SUBTITLE_BUTT
} HI_UNF_SO_SUBTITLE_TYPE_E;

/** Message type of subtitle *//** CNcomment:显示消息，对于teltext，含显示和擦除两个消息 */
typedef enum hiUNF_SO_DISP_MSG_TYPE_E
{
    HI_UNF_SO_DISP_MSG_NORM  = 0x0,     /**<Draw the subtitle *//**<CNcomment:正常显示数据 */
    HI_UNF_SO_DISP_MSG_ERASE,           /**<Clear the subtitle *//**<CNcomment:擦除 */
    HI_UNF_SO_DISP_MSG_END              /**<Message of drawing a page *//**<CNcomment:整页显示结束 */
} HI_UNF_SO_DISP_MSG_TYPE_E;

/** The parameter of clear region *//** CNcomment:擦除参数 */
typedef struct hiUNF_SO_CLEAR_PARAM_S
{
    HI_U32 x, y, w, h;     /**<Postion of clearing *//**<CNcomment:擦除位置和高宽信息 */
    HI_S64 s64ClearTime;   /**<Clear operation time,larger than or equal to s64NodePts+u32Duration*//**<CNcomment:擦除操作发生的时间,大于等于s64NodePts+u32Duration */
    HI_S64 s64NodePts;     /**<the present pts of sub *//**<CNcomment:字幕显示时间戳 */
    HI_U32 u32Duration;    /**<the present duration of sub *//**<CNcomment:字幕显示时间长度 */
} HI_UNF_SO_CLEAR_PARAM_S;

/** Color components  *//** CNcomment:颜色结构 */
typedef struct hiUNF_SO_COLOR_S
{
    HI_U8 u8Red;      /**<Red component *//**<CNcomment:R分量颜色值 */
    HI_U8 u8Green;    /**<Green component *//**<CNcomment:G分量颜色值 */
    HI_U8 u8Blue;     /**<Blue component *//**<CNcomment:B分量颜色值 */
    HI_U8 u8Alpha;    /**<Alpha component *//**<CNcomment:透明度，值为0为透明，0xFF为不透明 */
} HI_UNF_SO_COLOR_S;

/** Information of bitmap subtitle *//** CNcomment:图像字幕信息 */
typedef struct hiUNF_SO_GFX_S
{
    HI_S64 s64Pts;         /**<Start a display time, unit is Millisecond *//**<CNcomment:显示时间戳，单位ms */
    HI_U32 u32Duration;    /**<Duration of displaying, unit is Millisecond *//**<CNcomment:显示时长，单位ms */
    HI_U32 u32Len;         /**<Bytes of subtitle data *//**<CNcomment:数据长度，单位字节 */
    HI_U8  *pu8PixData;    /**<Data of subtitle *//**<CNcomment:图像数据 */

    HI_UNF_SO_DISP_MSG_TYPE_E enMsgType;                   /**<Type of display message *//**<CNcomment:显示消息类型 */
    HI_UNF_SO_COLOR_S stPalette[HI_UNF_SO_PALETTE_ENTRY];  /**<Palette *//**<CNcomment:调色板，ARGB8888 */
    HI_S32 s32BitWidth;    /**<Bits of Pix *//**<CNcomment:象素位宽 , 可以为 2,4,8位*/
    HI_U32 x, y, w, h;     /**<Position of display subtitle *//**<CNcomment:显示位置和高宽信息 */
    HI_U32 u32CanvasWidth;   /**<Display canvas width *//**<CNcomment:显示画布的宽度信息 */
    HI_U32 u32CanvasHeight;  /**<Display canvas height *//**<CNcomment:显示画布的高度信息 */
} HI_UNF_SO_GFX_S;

/** Infomation of text subtitle *//** CNcomment:文本字幕信息 */
typedef struct hiUNF_SO_TEXT_S
{
    HI_S64 s64Pts;              /**<Start a display time, unit is Millisecond *//**<CNcomment:显示时间戳，单位ms */
    HI_U32 u32Duration;         /**<Duration of displaying, unit is Millisecond *//**<CNcomment:显示时长，单位ms */
    HI_U32 u32Len;              /**<Bytes of subtitle data *//**<CNcomment:字幕数据长度，单位字节 */
    HI_U8 *pu8Data;             /**<Data of subtitle *//**<CNcomment:字幕数据 */

    HI_U32 x, y, w, h;          /**<Position of display subtitle *//**<CNcomment:显示位置和高宽信息 */
} HI_UNF_SO_TEXT_S;

/** Infomation of ass/ssa subtitle *//** CNcomment:ass字幕信息 */
typedef struct hiUNF_SO_ASS_S
{
    HI_S64 s64Pts;                /**<Start a display time, unit is Millisecond *//**<CNcomment:显示时间戳，单位ms */
    HI_U32 u32Duration;           /**<Duration of displaying, unit is Millisecond *//**<CNcomment:显示时长，单位ms */
    HI_U32 u32FrameLen;           /**<Bytes of subtitle data *//**<CNcomment:帧长度，单位字节 */
    HI_U8  *pu8EventData;         /**<Data of subtitle *//**<CNcomment:帧数据 */
    HI_U32 u32ExtradataSize;      /**<Length of extra data *//**<CNcomment:扩展数据长度，单位字节 */
    HI_U8  *pu8Extradata;         /**<Extra data *//**<CNcomment:扩展数据 */
} HI_UNF_SO_ASS_S;

/** Infomation of subtitle *//** CNcomment:字幕信息 */
typedef struct hiUNF_SO_SUBTITLE_INFO_S
{
    HI_UNF_SO_SUBTITLE_TYPE_E eType; /**<type *//**<CNcomment:字幕类型 */

    union
    {
        HI_UNF_SO_GFX_S  stGfx;      /**<Gfx subtitle *//**<CNcomment:图形字幕 */
        HI_UNF_SO_TEXT_S stText;     /**<Text subtitle *//**<CNcomment:文本字幕 */
        HI_UNF_SO_ASS_S  stAss;      /**<Ass subtitle *//**<CNcomment:ASS字幕 */
    } unSubtitleParam;
} HI_UNF_SO_SUBTITLE_INFO_S;

/**
\brief Call back funtion of getting current play time.
CNcomment:获取当前播放帧的时间戳，如果当前时间无效，ps64CurrentPts赋值为HI_UNF_SO_NO_PTS。CNend
\attention \n
None
\param[in] pUserData Userdata.CNcomment:注册传入的用户数据。CNend
\param[out] ps64CurrentPts Current play time. CNcomment:当前播放帧时间戳，单位ms。CNend

\retval ::HI_SUCCESS

\see \n
None
*/
typedef HI_S32 (*HI_UNF_SO_GETPTS_FN)(HI_VOID* pUserData, HI_S64 *ps64CurrentPts);

/**
\brief Callback funtion of drawing the subtitle. CNcomment:字幕画图回调函数。CNend
\attention \n
None
\param[in] pUserData Userdata. CNcomment:注册传入的用户数据。CNend
\param[in] pstInfo Information of subtitle. CNcomment:字幕信息。CNend
\param[in] pArg User data. CNcomment:扩展数据。CNend

\retval ::HI_SUCCESS

\see \n
None
*/
typedef HI_S32 (*HI_UNF_SO_ONDRAW_FN)(HI_VOID* pUserData, const HI_UNF_SO_SUBTITLE_INFO_S *pstInfo, HI_VOID *pArg);

/**
\brief Call back funtion of clearing the subtitle. CNcomment:字幕显示到时清除函数。CNend
\attention \n
None
\param[in] pUserData User data. CNcomment:注册传入的用户数据。CNend
\param[out] pArg Parameter of clearing the subtitle, Type is HI_UNF_SO_CLEAR_PARAM_S. CNcomment:擦除参数，指向HI_UNF_SO_CLEAR_PARAM_S结构。CNend

\retval ::HI_SUCCESS

\see \n
None
*/
typedef HI_S32 (*HI_UNF_SO_ONCLEAR_FN)(HI_VOID* pUserData, HI_VOID *pArg);

/** @} */  /** <!-- ==== Structure Definition End ==== */

/******************************* API Declaration *****************************/
/** \addtogroup      SUBTITLEOUT */
/** @{*/  /** <!-- [SUBTITLEOUT]*/

/**
\brief  Init SO module. CNcomment:subtitle output 模块初始化。CNend
\attention \n
None
\param None

\retval ::HI_SUCCESS Init success. CNcomment:初始化成功。CNend
\retval ::HI_FAILURE Operation fail. CNcomment:初始化失败。CNend

\see \n
None
*/
HI_S32 HI_UNF_SO_Init(HI_VOID);

/**
\brief  Deinit SO module. CNcomment:subtitle output 模块去初始化。CNend
\attention \n
None
\param None

\retval ::HI_SUCCESS  Deinit success. CNcomment:去初始化成功。CNend
\retval ::HI_FAILURE Operation fail. CNcomment:去初始化失败。CNend

\see \n
None
*/
HI_S32 HI_UNF_SO_DeInit(HI_VOID);

/**
\brief  Creates an SO instance. CNcomment:创建一个so实例。CNend
\attention \n
None
\param[out] phdl Handle of SO instance. CNcomment:播放器实例。CNend

\retval ::HI_SUCCESS  Create success. CNcomment:创建成功，句柄有效。CNend
\retval ::HI_FAILURE  Operation fail. CNcomment:创建失败。CNend

\see \n
None
*/
HI_S32 HI_UNF_SO_Create(HI_HANDLE *phdl);

/**
\brief  Destroy an SO instance. CNcomment:销毁一个so实例。CNend
\attention \n
None
\param[in] handle Handle of SO instance. CNcomment:so实例句柄。CNend

\retval ::HI_SUCCESS  Destroy success. CNcomment:销毁成功。CNend
\retval ::HI_FAILURE  Operation fail. CNcomment:销毁失败，参数非法。CNend

\see \n
None
*/
HI_S32 HI_UNF_SO_Destroy(HI_HANDLE handle);

/**
\brief  Setting offset time of the subtitle. CNcomment:设置字幕时间偏移。CNend
\attention \n
None
\param[in] handle Handle of SO instance. CNcomment:so实例句柄。CNend
\param[in] s64OffsetMs Offset of subtitle, unit is Millisecondes.CNcomment:字幕显示时间偏移值，单位ms。CNend

\retval ::HI_SUCCESS  Operation success. CNcomment:成功。CNend
\retval ::HI_FAILURE  Operation fail. CNcomment:失败。CNend

\see \n
None
*/
HI_S32 HI_UNF_SO_SetOffset(HI_HANDLE handle, HI_S64 s64OffsetMs);

/**
\brief  Register the callback function of getting current play time. CNcomment:注册时间戳获取回调函数，so通过获取当前播放时间戳同步字幕。CNend
\attention \n
None
\param[in] handle Handle of SO instance.CNcomment:so实例句柄。CNend
\param[in] pstCallback Callback function. CNcomment:回调函数参数。CNend
\param[in] pUserData User data. CNcomment:用户数据。CNend

\retval ::HI_SUCCESS  Register success. CNcomment:注册成功。CNend
\retval ::HI_FAILURE  Parameter invalid. CNcomment:注册失败。CNend

\see \n
None
*/
HI_S32 HI_UNF_SO_RegGetPtsCb(HI_HANDLE handle, HI_UNF_SO_GETPTS_FN pstCallback, HI_VOID* pUserData);

/**
\brief  Register the callback function of drawing subtitle.
CNcomment:注册字幕绘制清除回调函数，如果设置了该函数，则so使用该函数实现字幕输出，字幕显示时长到达后，so调用清除函数通知清除。CNend
\attention \n
None
\param[in] handle Handle of SO instance. CNcomment:so实例句柄。CNend
\param[in] pfnOnDraw Callback function of drawing subtitle. CNcomment:绘制函数。CNend
\param[in] pfnOnClear Callback function of clearing subtitle. CNcomment:清除函数。CNend
\param[in] pUserData User data. CNcomment:用户数据。CNend

\retval ::HI_SUCCESS  Register success. CNcomment:注册成功。CNend
\retval ::HI_FAILURE Parameter invalid. CNcomment:注册失败。CNend

\see \n
None
*/
HI_S32 HI_UNF_SO_RegOnDrawCb(HI_HANDLE handle, HI_UNF_SO_ONDRAW_FN pfnOnDraw,
                                          HI_UNF_SO_ONCLEAR_FN pfnOnClear, HI_VOID* pUserData);

/**
\brief  Set the surface of drawing subtitle, if you do not set the ondraw callback function, SO painting subtitles with the surface.
CNcomment:设置字幕绘制画布句柄，如果没有设置ondraw回调函数，则so使用设置的画布句柄输出字幕。CNend
\attention \n
This feature is not implemented. CNcomment:该功能未实现。CNend
\param[in] handle Handle of SO instance.CNcomment:so实例句柄。CNend
\param[in] hSurfaceHandle Handle of surface. CNcomment:画布句柄。CNend

\retval ::HI_SUCCESS Success of setting surface. CNcomment:设置成功。CNend
\retval ::HI_FAILURE  Operation invalid. CNcomment:设置失败。CNend

\see \n
None
*/
HI_S32 HI_UNF_SO_SetDrawSurface(HI_HANDLE handle, HI_HANDLE hSurfaceHandle);

/**
\brief  Set the font of drawing subtitle.
CNcomment:设置字幕显示字体，该设置仅对设置画布输出字幕的方式起作用。CNend
\attention \n
This feature is not implemented. CNcomment:该功能未实现。CNend
\param[in] handle Handle of SO instance.CNcomment:so实例句柄。CNend
\param[in] hFont Handle of font instance.CNcomment:创建的字体句柄。CNend

\retval ::HI_SUCCESS  Success.CNcomment:设置成功。CNend
\retval ::HI_FAILURE  Operation invalid. CNcomment:设置失败。CNend

\see \n
None
*/
HI_S32 HI_UNF_SO_SetFont(HI_HANDLE handle, HI_HANDLE hFont);

/**
\brief Set the color of drawing text subtitle.
CNcomment:设置字幕显示颜色，该设置仅对设置画布输出字幕的方式起作用。CNend
\attention \n
This feature is not implemented. CNcomment:该功能未实现。CNend
\param[in] handle Handle of SO instance. CNcomment:so实例句柄。CNend
\param[in] u32Color Value of the color. CNcomment:颜色值。CNend

\retval ::HI_SUCCESS  Success. CNcomment:设置成功。CNend
\retval ::HI_FAILURE  Operation invalid. CNcomment:设置失败。CNend

\see \n
None
*/
HI_S32 HI_UNF_SO_SetColor(HI_HANDLE handle, HI_U32 u32Color);

/**
\brief Set the position of display subtitle.
CNcomment:设置字幕显示坐标，该设置仅对设置画布输出字幕的方式起作用。CNend
\attention \n
This feature is not implemented. CNcomment:该功能未实现。CNend
\param[in] handle Handle of SO instance. CNcomment:so实例句柄。CNend
\param[in] u32x Value of x coordinate.CNcomment:x坐标。CNend
\param[in] u32y Value of y coordinate. CNcomment:y坐标。CNend

\retval ::HI_SUCCESS  Success. CNcomment:设置成功。CNend
\retval ::HI_FAILURE  Parameter invalid. CNcomment:设置失败。CNend

\see \n
None
*/
HI_S32 HI_UNF_SO_SetPos(HI_HANDLE handle, HI_U32 u32x, HI_U32 u32y);

/**
\brief  Getting the number in the butitle queue.
CNcomment:获取缓冲buffer中未显示的字幕数据个数。CNend
\attention \n
None
\param[in] handle Handle of SO instance. CNcomment:so实例句柄。CNend
\param[out] pu32SubNum Number of subtitles in queue. CNcomment:缓冲字幕个数。CNend

\retval ::HI_SUCCESS Success. CNcomment:成功。CNend
\retval ::HI_FAILURE Operation fail. CNcomment:失败。CNend

\see \n
None
*/
HI_S32 HI_UNF_SO_GetSubNumInBuff(HI_HANDLE handle, HI_U32 *pu32SubNum);

/**
\brief Clear the subtitles in queue.
CNcomment:清除字幕缓冲中的数据，字幕切换时，需调用该接口清除so字幕缓存。CNend
\attention \n
None
\param[in] handle Handle of SO instance. CNcomment:so实例句柄。CNend

\retval ::HI_SUCCESS Success. CNcomment:成功。CNend
\retval ::HI_FAILURE Operation invalid. CNcomment:失败。CNend

\see \n
None
*/
HI_S32 HI_UNF_SO_ResetSubBuf(HI_HANDLE handle);

/**
\brief  Clear the subtitles in queue by PTS. If PTS is 0, the same as HI_UNF_SO_ResetSubBuf.
CNcomment:清除字幕缓冲中的数据,小于指定PTS的字幕缓冲被清除.如果指定PTS值为0,这个接口等效于HI_UNF_SO_ResetSubBuf。CNend
\attention \n
None
\param[in] handle Handle of SO instance. CNcomment:so实例句柄。CNend

\retval ::HI_SUCCESS Success. CNcomment:成功。CNend
\retval ::HI_FAILURE Operation invalid. CNcomment:失败。CNend

\see \n
None
*/
HI_S32 HI_UNF_SO_ResetSubBuf_ByPts(HI_HANDLE handle, HI_S64 s64Pts);

/**
\brief  Send subtitles to SO queue. If the data size and the number is larger than the value set by the user, then the transmission fails.
CNcomment:发送字幕数据给so，如果缓冲size和个数都超过用户设置的值，则发送失败。CNend
\attention \n
None
\param[in] handle Handle of SO instance. CNcomment:so实例句柄。CNend
\param[in] pstSubInfo Data of subtitle. CNcomment:字幕信息。CNend
\param[in] u32TimeOut Timeout of sending subtitle. CNcomment:发送超时时间，单位ms。CNend

\retval ::HI_SUCCESS  Success. CNcomment:发送成功。CNend
\retval ::HI_FAILURE  Operation fail.CNcomment:发送失败。CNend

\see \n
None
*/
HI_S32 HI_UNF_SO_SendData(HI_HANDLE handle, const HI_UNF_SO_SUBTITLE_INFO_S *pstSubInfo, HI_U32 u32TimeOutMs);

/**
\brief  Get current subt data in SO queue.
CNcomment:获取SO 队列中，当前正在处理的字幕数据。CNend
\attention \n
None
\param[in] handle Handle of SO instance. CNcomment:so实例句柄。CNend
\param[in] pstSubInfo Data of subtitle. CNcomment:字幕信息。CNend

\retval ::HI_SUCCESS  Success. CNcomment:发送成功。CNend
\retval ::HI_FAILURE  Operation fail.CNcomment:发送失败。CNend

\see \n
None
*/
HI_S32 HI_UNF_SO_GetCurData(HI_HANDLE handle, HI_UNF_SO_SUBTITLE_INFO_S *pstSubInfo);

/**
\brief  Setting max interval time of the subtitle. 
CNcomment:设置字幕最大的时间偏差。CNend 
\attention \n 
None 
\param[in] handle Handle of SO instance. CNcomment:so实例句柄。CNend 
\param[in] u32IntervalMs max interval of subtitle, unit is Millisecondes.
CNcomment:字幕最大的时间偏差值，单位ms。CNend 

\retval ::HI_SUCCESS  Operation success. CNcomment:成功。CNend 
\retval ::HI_FAILURE  Operation fail. CNcomment:失败。CNend 

\see \n 
None 
*/ 
HI_S32 HI_UNF_SO_SetMaxInterval(HI_HANDLE handle, HI_U32 u32IntervalMs ); 

/** @} */  /** <!-- ==== API Declaration End ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_UNF_SO_H__ */

