#ifndef __HI_UNF_SUBT_H__
#define __HI_UNF_SUBT_H__

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus*/

/*************************** Structure Definition ****************************/
/** \addtogroup      SUBTITLE */
/** @{ */  /** <!-- 【SUBTITLE】 */

/** Invalid handle in subtitle *//** CNcomment:无效的字幕句柄 */
#define SUBT_INVALID_HANDLE   (0x0)
/** the max item(language) in Subtitling descriptor *//** CNcomment:最大的字幕项(语言)个数 */
#define SUBT_ITEM_MAX_NUM     (32)
/** the max subtitle instance *//** CNcomment:最大的字幕模块实例 */
#define SUBT_INSTANCE_MAX_NUM (8)

/** Defines the subtitle data type  *//** CNcomment: 定义字幕数据类型 */
typedef enum hiUNF_SUBT_DATA_TYPE_E
{
    HI_UNF_SUBT_DVB  = 0x1, /**<DVB subtitle *//**<CNcomment: DVB字幕 */
    HI_UNF_SUBT_SCTE = 0x2, /**<SCTE subtitle *//**<CNcomment: SCTE字幕 */
    HI_UNF_SUBT_BUTT
}HI_UNF_SUBT_DATA_TYPE_E;

/** Defines the type used to code the subtitle *//** CNcomment:定义字幕编码类型枚举 */
typedef enum hiUNF_SUBT_TYPE_E
{
    HI_UNF_SUBT_TYPE_BITMAP = 0, /**<Coding of bitmap *//**<CNcomment:位图编码 */
    HI_UNF_SUBT_TYPE_TEXT,       /**<Coded as a string of characters *//**<CNcomment:文本编码 */
    HI_UNF_SUBT_TYPE_BUTT
}HI_UNF_SUBT_TYPE_E;

/** Defines the status of the subtitling page *//** CNcomment:定义字幕页状态枚举 */
typedef enum hiUNF_SUBT_PAGE_STATE_E
{
    HI_UNF_SUBT_PAGE_NORMAL_CASE        = 0,    /**<Page update, use previous page instance to display *//**<CNcomment:局部更新 */
    HI_UNF_SUBT_PAGE_ACQUISITION_POINT,         /**<Page refresh, use next page instance to display *//**<CNcomment:全屏刷新 */
    HI_UNF_SUBT_PAGE_MODE_CHANGE,               /**<New page, needed to display the new page *//**<CNcomment:刷新整个字幕页 */
    HI_UNF_SUBT_PAGE_BUTT
}HI_UNF_SUBT_PAGE_STATE_E;

/** Defines the data of the subtitle output *//** CNcomment:定义字幕输出数据结构体 */
typedef struct hiUNF_SUBT_DATA_S
{
    HI_UNF_SUBT_TYPE_E enDataType;        /**<The type used to code the subtitle *//**<CNcomment:字幕编码类型 */
    HI_UNF_SUBT_PAGE_STATE_E enPageState; /**<The status of the subtitling page *//**<CNcomment:字幕页状态 */
    HI_U32             u32x;              /**<The horizontal address of subtitling page *//**<CNcomment:x坐标 */
    HI_U32             u32y;              /**<The vertical address of subtitling page *//**<CNcomment:y坐标 */
    HI_U32             u32w;              /**<The horizontal length of subtitling page *//**<CNcomment:宽度 */
    HI_U32             u32h;              /**<The vertical length of subtitling page *//**<CNcomment:高度 */
    HI_U32             u32BitWidth;       /**<Bits in pixel-code *//**<CNcomment:位宽 */
    HI_U32             u32PTS;            /**<Presentation time stamp *//**<CNcomment:时间戳 */
    HI_U32             u32Duration;       /**<The period, expressed in ms, after which a page instance is no longer valid *//**<CNcomment:持续时间，单位是ms */
    HI_U32             u32PaletteItem;    /**<Pixels of palette *//**<CNcomment:调色板长度 */
    HI_VOID*           pvPalette;         /**<Palette data *//**<CNcomment:调色板数据 */
    HI_U32             u32DataLen;        /**<Subtitling page data length *//**<CNcomment:字幕数据长度 */
    HI_U8*             pu8SubtData;       /**<Subtitling page data *//**<CNcomment:字幕数据 */
    HI_U32             u32DisplayWidth;   /**<Display canvas width *//**<CNcomment:显示画布的宽度 */
    HI_U32             u32DisplayHeight;  /**<Display canvas height *//**<CNcomment:显示画布的高度 */
}HI_UNF_SUBT_DATA_S;

/** Defines the item of the subtitling content *//** CNcomment:定义字幕服务项结构体 */
typedef struct hiUNF_SUBT_ITEM_S
{
    HI_U32 u32SubtPID;      /**<The pid for playing subtitle *//**<CNcomment:字幕pid */
    HI_U16 u16PageID;       /**<The Subtitle page id *//**<CNcomment:字幕页id */
    HI_U16 u16AncillaryID;  /**<The Subtitle ancillary id *//**<CNcomment:字幕辅助页id */
}HI_UNF_SUBT_ITEM_S;

/** Defines the callback function which output the subtitling data *//** CNcomment:定义输出字幕数据的回调函数 */
typedef HI_S32 (*HI_UNF_SUBT_CALLBACK_FN)(HI_VOID* pUserData, HI_UNF_SUBT_DATA_S *pstData);

/** Defines the callback function which get current pts *//** CNcomment:定义获取当前时间戳的回调函数 */
typedef HI_S32 (*HI_UNF_SUBT_GETPTS_FN)(HI_VOID* pUserData, HI_S64 *ps64Pts);

/** Defines the parameter of subtitle instance *//** CNcomment:定义字幕模块参数结构体 */
typedef struct hiUNF_SUBT_PARAM_S
{
    HI_UNF_SUBT_ITEM_S astItems[SUBT_ITEM_MAX_NUM]; /**<The item of the subtitling content *//**<CNcomment:字幕内容 */
    HI_U8  u8SubtItemNum;                           /**<Amount of subtitling item *//**<CNcomment:字幕内容个数 */
    HI_UNF_SUBT_CALLBACK_FN pfnCallback;            /**<Callback function in which output subtitling page data *//**<CNcomment:回调函数，用来输出解码后的字幕数据 */
    HI_VOID* pUserData;                             /**<User data used in callback function *//**<CNcomment:用户数据，只用在回调函数里面 */
    HI_UNF_SUBT_DATA_TYPE_E enDataType;             /**<subtitle data type in subt module *//**<CNcomment:在subt模块中用的字幕数据类型 */
}HI_UNF_SUBT_PARAM_S;

/** @} */  /** <!-- ==== Structure Definition end ==== */

/******************************* API Declaration *****************************/
/** \addtogroup      SUBTITLE */
/** @{*/  /** <!-- [SUBTITLE] */

/**
\brief Initialize subtitle module. CNcomment:初始化字幕模块。CNend
\attention \n
none. CNcomment:无。CNend
\retval ::HI_SUCCESS initialize success. CNcomment:初始化成功。CNend
\retval ::HI_FAILURE initialize failure. CNcomment:初始化失败。CNend
\see \n
none. CNcomment:无。CNend
*/
HI_S32 HI_UNF_SUBT_Init(HI_VOID);

/**
\brief DeInitialize subtitle module. CNcomment:去初始化字幕模块。CNend
\attention \n
none. CNcomment:无。CNend
\retval ::HI_SUCCESS deinitialize success. CNcomment:去初始化成功。CNend
\retval ::HI_FAILURE deinitialize failure. CNcomment:去初始化失败。CNend
\see \n
none. CNcomment:无。CNend
*/
 HI_S32 HI_UNF_SUBT_DeInit(HI_VOID);

/**
\brief Create subtitle module. CNcomment:创建字幕模块。CNend
\attention \n
none. CNcomment:无。CNend
\param[in]  pstSubtParam  parameters used in created subtitle. CNcomment:参数值。CNend
\param[out] phSubt        subtitle handle. CNcomment:字幕句柄。CNend
\retval ::HI_SUCCESS create success. CNcomment:创建成功。CNend
\retval ::HI_FAILURE create failure. CNcomment:创建失败。CNend
\see \n
none. CNcomment:无。CNend
*/
HI_S32 HI_UNF_SUBT_Create(HI_UNF_SUBT_PARAM_S *pstSubtParam, HI_HANDLE *phSubt);

/**
\brief Destroy subtitle module. CNcomment:销毁字幕模块。CNend
\attention \n
none. CNcomment:无。CNend
\param[in]  hSubt         subtitle handle. CNcomment:字幕句柄。CNend
\retval ::HI_SUCCESS destroy success. CNcomment:销毁成功。CNend
\retval ::HI_FAILURE destroy failure. CNcomment:销毁失败。CNend
\see \n
none. CNcomment:无。CNend
*/
HI_S32 HI_UNF_SUBT_Destroy(HI_HANDLE hSubt);

/**
\brief Select one subtitle content to output. CNcomment:切换字幕内容。CNend
\attention \n
none. CNcomment:无。CNend
\param[in]  hSubt         subtitle handle. CNcomment:字幕句柄。CNend
\param[in]  pstSubtItem   subtitle item. CNcomment:字幕内容项。CNend
\retval ::HI_SUCCESS switching success. CNcomment:切换成功。CNend
\retval ::HI_FAILURE switching failure. CNcomment:切换失败。CNend
\see \n
none. CNcomment:无。CNend
*/
HI_S32 HI_UNF_SUBT_SwitchContent(HI_HANDLE hSubt, HI_UNF_SUBT_ITEM_S *pstSubtItem);

/**
\brief Reset subtitle module. CNcomment:复位字幕模块。CNend
\attention \n
none. CNcomment:无。CNend
\param[in]  hSubt         subtitle handle. CNcomment:字幕句柄。CNend
\retval ::HI_SUCCESS reset success. CNcomment:复位成功。CNend
\retval ::HI_FAILURE reset failure. CNcomment:复位失败。CNend
\see \n
none. CNcomment:无。CNend
*/
HI_S32 HI_UNF_SUBT_Reset(HI_HANDLE hSubt);

/**
\brief Update subtitle module. CNcomment:更新字幕模块。CNend
\attention \n
none. CNcomment:无。CNend
\param[in]  hSubt         subtitle handle. CNcomment:字幕句柄。CNend
\param[in]  pstSubtParam  the new subtitle content. CNcomment:新的字幕内容信息。CNend
\retval ::HI_SUCCESS update success. CNcomment:更新成功。CNend
\retval ::HI_FAILURE update failure. CNcomment:更新失败。CNend
\see \n
none. CNcomment:无。CNend
*/
HI_S32 HI_UNF_SUBT_Update(HI_HANDLE hSubt, HI_UNF_SUBT_PARAM_S *pstSubtParam);

/**
\brief Inject DVB subtitle stream to decoder. CNcomment:注入字幕数据到解码器。CNend
\attention \n
Used the PES packet syntax for carriage of DVB subtitles. CNcomment:DVB字幕传输使用PES格式。CNend
\param[in]  hSubt         subtitle handle. CNcomment:字幕句柄。CNend
\param[in]  u32SubtPID    the pid of subtitle stream. CNcomment:字幕流pid。CNend
\param[in]  pu8Data       subtitle stream data. CNcomment:字幕数据。CNend
\param[in]  u32DataSize   the size of subtitle stream data. CNcomment:字幕数据长度。CNend
\retval ::HI_SUCCESS inject success. CNcomment:注入成功。CNend
\retval ::HI_FAILURE inject failure. CNcomment:注入失败。CNend
\see \n
none. CNcomment:无。CNend
*/
HI_S32 HI_UNF_SUBT_InjectData(HI_HANDLE hSubt, HI_U32 u32SubtPID, HI_U8* pu8Data, HI_U32 u32DataSize);

/**
\brief Register the callback function geted current pts.
CNcomment:注册获取当前时间戳的回调函数。CNend
\attention \n
none. CNcomment:无。CNend
\param[in]  hSubt         subtitle handle. CNcomment:字幕句柄。CNend
\param[in]  pfnGetPts     callback funtion which geted current pts. CNcomment:获取当前时间戳的回调函数。CNend
\param[in]  pUserData   userdata which used in callback funtion. CNcomment:回调函数的用户数据。CNend
\retval   ::HI_SUCCESS    success. CNcomment:成功。CNend
\retval   ::HI_FAILURE    failure. CNcomment:失败。CNend
\see \n
none. CNcomment: 无。CNend
*/
HI_S32 HI_UNF_SUBT_RegGetPtsCb(HI_HANDLE hSubt, HI_UNF_SUBT_GETPTS_FN pfnGetPts, HI_VOID* pUserData);

/** @} */  /** <!-- ==== API declaration end ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus*/

#endif
