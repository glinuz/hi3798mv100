/******************************************************************************
*              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: hi_gfx2d_api.h
* Description: Graphic 2D engine API declaration.
*
* History:
* Version   Date         Author     DefectNum    Description
* main\1    2013-10-23              NULL         Create this file.
*******************************************************************************/

#ifndef _HI_GFX2D_API_H_
#define _HI_GFX2D_API_H_

#include "hi_gfx2d_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif  /*__cplusplus*/
#endif  /*__cplusplus*/

/********************************* API declaration *****************************/

/** \addtogroup      GFX2D API */
/** @{ */  /** <!-- 【GFX2D API】 */

/**
\brief Open device. CNcomment:打开设备CNend
\attention \n
Call this interface to open gfx engine device before using it.\n
CNcomment:当要使用该文件提供的接口时,先调用本接口打开设备\n CNend

\param[in] enDevId Device id. CNcomment:设备号CNend
\retval 0 success. CNcomment:成功CNend
\retval ::HI_ERR_GFX2D_INVALID_DEVID Invalid device id. CNcomment:不正确的设备号CNend
\retval ::HI_ERR_GFX2D_DEV_PERMISSION Failed to access to device. CNcomment:访问设备失败CNend
\see \n
::HI_GFX2D_Close
 */
HI_S32 HI_GFX2D_Open(const HI_GFX2D_DEV_ID_E enDevId);

/**
\brief Close device. CNcomment:关闭设备CNend
\attention \n
Call this interface to close gfx engine device.\n
Keep calling HI_GFX2D_Open and HI_GFX2D_Close in pairs.\n
CNcomment:调用本接口关闭设备\n
始终保持HI_GFX2D_Open和HI_GFX2D_Close成对调用\n CNend

\param[in] enDevId Device id. CNcomment:设备号CNend
\retval 0 success. CNcomment:成功CNend
\see \n
::HI_GFX2D_Open
 */
HI_S32 HI_GFX2D_Close(const HI_GFX2D_DEV_ID_E enDevId);

/**
\brief Submit sync compose task. CNcomment:提交同步叠加任务CNend
\attention \n
Task submitted by this interface is sync.Returning success indicates the task is completed.If it returns timeout,the task will finish in future time.\n
The interface supports composing seven layers at most.\n
Six layers of them can support such color fmts:ARGB8888、ABGR8888、KRGB888、KBGR888、ARGB1555、RGB565、RGB888、BGR888、YUYV422、YVYU422、UYVY422、YYUV422、VYUY422、VUYY422、SEMIPLANAR420UV、SEMIPLANAR420VU、SEMIPLANAR422UV_H、SEMIPLANAR422VU_H、SEMIPLANAR422UV_V、SEMIPLANAR422VU_V、PLANAR400、PLANAR420、PLANAR411、PLANAR410、PLANAR422H、PLANAR422V、PLANAR444.\n
Lowest resolution supported is 1*1,highest is 2560*1600.\n
The other layer can support more color fmts:ARGB8888、ABGR8888、KRGB888、KBGR888、ARGB1555、RGB565、RGB888、BGR888
Lowest resolution supported by this layer is 32*32,highest is 2560*1600.\n
This layer only supportes mem type surface.
In addition,this layer supports zoom.Horizontal deflate ratio shoule be less than 256, vertical should be less than 16.There is no limit to magnify ratio.\n
Output fmt supported by this interface is:ARGB8888、ABGR8888、ARGB1555、RGB888、BGR888、SEMIPLANAR420UV.\n
If the output fmt is ARGB8888,setting the compression of dst surface will enable compression.In this case,mem size and stride of dst surface should be get by calling HI_GFX2D_GetCmpSurfaceStride and HI_GFX2D_GetCmpSurfaceSize,and width of dst surface should be even.\n
If the output fmt is SEMIPLANAR420UV, setting the histogram addr of dst surface will enable histogram stat.Size of histogram mem is 64byte.
Stride of surface should be 16 byte aligned.
Colorkey and ROP are both not supported.

CNcomment:该接口提交的是同步任务,返回成功即表示提交的任务完成,若返回超时,则任务会在将来的某个时间完成，不会丢弃\n
最多可以支持7层叠加\n
其中六层支持的输入格式为:ARGB8888、ABGR8888、KRGB888、KBGR888、ARGB1555、RGB565、RGB888、BGR888\n
支持的输入分辨率最小为1*1,最大2560*1600\n
另外一个层支持的输入格式为:\n
ARGB8888、ABGR8888、KRGB888、KBGR888、ARGB1555、RGB565、RGB888、BGR888\n
YUYV422、YVYU422、UYVY422、YYUV422、VYUY422、VUYY422\n
SEMIPLANAR420UV、SEMIPLANAR420VU、SEMIPLANAR422UV_H、SEMIPLANAR422VU_H、SEMIPLANAR422UV_V、SEMIPLANAR422VU_V\n
PLANAR400、PLANAR420、PLANAR411、PLANAR410、PLANAR422H、PLANAR422V、PLANAR444\n
支持的输入分辨率最小为32*32,最大2560*1600\n
该层只支持mem类型的输入surface\n
该层支持缩放:水平最大缩小倍数为256(不含)，垂直最大缩小倍数为16(不含)，放大无限制\n
支持的输出格式为:\n
ARGB8888、ABGR8888、ARGB1555、RGB888、BGR888、SEMIPLANAR420UV\n
在输出ARGB8888格式时，可以支持对输出数据进行压缩。此时，输出surface的stride和需要的内存\n
大小与非压缩时有差异，需要通过HI_GFX2D_GetCmpSurfaceStride和HI_GFX2D_GetCmpSurfaceSize来获取\n
输出压缩时，要求输出的宽度为偶数\n
在输出SEMIPLANAR420UV时，支持获取输出数据Y分量的直方图。若需要获取直方图信息，需要在输出surface\n
中指定64byte大小内存的首地址\n
输入、输出的stride要求16byte对齐\n
不支持colorkey功能\n
不支持ROP功能\n
不支持输入surface为压缩类型的surface\n
对于Color类型的surface,其格式固定为ARGB8888,color值按照ARGB8888来设置\n
超时值设置为0，会一直等待直到任务完成\nCNend

\param[in]  enDevId Device id. CNcomment:设备号CNend
\param[in]  pstComposeList Input surface list. CNcomment:输入surface链表CNend
\param[in]  pstDst Dst surface. CNcomment:目标surfaceCNend
\param[in]  u32Timeout Timeout value,measured in ms. CNcomment:超时值，毫秒为单位CNend

\retval 0 Success. CNcomment:成功CNend
\retval ::HI_ERR_GFX2D_INVALID_DEVID Invalid device id. CNcomment:不正确的设备号CNend
\retval ::HI_ERR_GFX2D_DEV_NOT_OPEN Device not opened. CNcomment:设备未打开CNend
\retval ::HI_ERR_GFX2D_NULL_PTR Null pointer param. CNcomment:空指针参数CNend
\retval ::HI_ERR_GFX2D_INVALID_COMPOSECNT Invalid input surface count. CNcomment:不正确的输入surface个数CNend
\retval ::HI_ERR_GFX2D_INVALID_SURFACE_TYPE Invalid surface type. CNcomment:不正确的surface类型CNend
\retval ::HI_ERR_GFX2D_INVALID_SURFACE_RESO Invalid surface resolution. CNcomment:不正确的surface分辨率CNend
\retval ::HI_ERR_GFX2D_INVALID_SURFACE_FMT Invalid surface color format. CNcomment:不正确的surface像素格式CNend
\retval ::HI_ERR_GFX2D_INVALID_SURFACE_ADDR Invalid surface addr. CNcomment:不正确的surface地址CNend
\retval ::HI_ERR_GFX2D_INVALID_SURFACE_STRIDE Invalid surface stride. CNcomment:不正确的surface行间距CNend
\retval ::HI_ERR_GFX2D_INVALID_SURFACE_CMPTYPE Invalid surface cmpression type. CNcomment:不正确的压缩类型CNend
\retval ::HI_ERR_GFX2D_INVALID_RECT Invalid operation rect. CNcomment:不正确的操作矩形CNend
\retval ::HI_ERR_GFX2D_INVALID_RESIZE_FILTERMODE Invalid filter mode. CNcomment:不正确的滤波模式CNend
\retval ::HI_ERR_GFX2D_INVALID_RESIZE_RATIO Invalid resize ratio. CNcomment:不正确的缩放比CNend
\retval ::HI_ERR_GFX2D_INVALID_CLIP_MODE Invalid clip mode. CNcomment:不正确的剪切模式CNend
\retval ::HI_ERR_GFX2D_INVALID_CLIP_RECT Invalid clip rect. CNcomment:不正确的剪切矩形CNend
\retval ::HI_ERR_GFX2D_UNSUPPORT Unsupport operation. CNcomment:不支持的操作CNend
\retval ::HI_ERR_GFX2D_NO_MEM Mem is not enough. CNcomment:内存不足CNend
\retval ::HI_ERR_GFX2D_TIMEOUT Task is not finished in appointed time. CNcomment:任务未在指定的超时时间内完成CNend

 \see \n
::HI_GFX2D_ComposeAsync
 */
HI_S32 HI_GFX2D_ComposeSync(const HI_GFX2D_DEV_ID_E enDevId,
                            HI_GFX2D_COMPOSE_LIST_S *pstComposeList,
                            HI_GFX2D_SURFACE_S *pstDst,
                            const HI_U32 u32Timeout);



/**
\brief show sdk version. CNcomment:提交异步叠加任务\n CNend
\attention \n
\n
CNcomment:该接口提交的是异步任务，返回成功不代表任务完成其他可参考接口HI_GFX2D_ComposeSync CNend\n

\param[in]  enDevId Device id. CNcomment:设备号CNend
\param[in]  pstComposeList Input surface list. CNcomment:输入surface链表CNend
\param[in]  pstDst Dst surface. CNcomment:目标surfaceCNend

\retval 0 Success. CNcomment:成功CNend
\retval ::HI_ERR_GFX2D_INVALID_DEVID Invalid device id. CNcomment:不正确的设备号CNend
\retval ::HI_ERR_GFX2D_DEV_NOT_OPEN Device not opened. CNcomment:设备未打开CNend
\retval ::HI_ERR_GFX2D_NULL_PTR Null pointer param. CNcomment:空指针参数CNend
\retval ::HI_ERR_GFX2D_INVALID_COMPOSECNT Invalid input surface count. CNcomment:不正确的输入surface个数CNend
\retval ::HI_ERR_GFX2D_INVALID_SURFACE_TYPE Invalid surface type. CNcomment:不正确的surface类型CNend
\retval ::HI_ERR_GFX2D_INVALID_SURFACE_RESO Invalid surface resolution. CNcomment:不正确的surface分辨率CNend
\retval ::HI_ERR_GFX2D_INVALID_SURFACE_FMT Invalid surface color format. CNcomment:不正确的surface像素格式CNend
\retval ::HI_ERR_GFX2D_INVALID_SURFACE_ADDR Invalid surface addr. CNcomment:不正确的surface地址CNend
\retval ::HI_ERR_GFX2D_INVALID_SURFACE_STRIDE Invalid surface stride. CNcomment:不正确的surface行间距CNend
\retval ::HI_ERR_GFX2D_INVALID_SURFACE_CMPTYPE Invalid surface cmpression type. CNcomment:不正确的压缩类型CNend
\retval ::HI_ERR_GFX2D_INVALID_RECT Invalid operation rect. CNcomment:不正确的操作矩形CNend
\retval ::HI_ERR_GFX2D_INVALID_RESIZE_FILTERMODE Invalid filter mode. CNcomment:不正确的滤波模式CNend
\retval ::HI_ERR_GFX2D_INVALID_RESIZE_RATIO Invalid resize ratio. CNcomment:不正确的缩放比CNend
\retval ::HI_ERR_GFX2D_INVALID_CLIP_MODE Invalid clip mode. CNcomment:不正确的剪切模式CNend
\retval ::HI_ERR_GFX2D_INVALID_CLIP_RECT Invalid clip rect. CNcomment:不正确的剪切矩形CNend
\retval ::HI_ERR_GFX2D_UNSUPPORT Unsupport operation. CNcomment:不支持的操作CNend
\retval ::HI_ERR_GFX2D_NO_MEM Mem is not enough. CNcomment:内存不足CNend

 \see \n
::HI_GFX_ComposeSync
 */
HI_S32 HI_GFX2D_ComposeAsync(const HI_GFX2D_DEV_ID_E enDevId,
                             HI_GFX2D_COMPOSE_LIST_S *pstComposeList,
                             HI_GFX2D_SURFACE_S *pstDst);

/**
\brief Wait all tasks submitted finished. CNcomment:等待所有任务完成CNend
\attention \n
If u32Timeout is zero,the interface will wait forever until tasks are finished.\n
CNcomment:当u32Timeout的值为0时，会一直等待直到任务完成\n CNend
\param[in]  enDevId Device id. CNcomment:设备号CNend
\param[in]  u32TimeOut Timeout value,measured in ms. CNcomment:超时值,单位为毫秒CNend
\retval 0 Success. CNcomment:成功CNend
\retval ::HI_ERR_GFX2D_INVALID_DEVID Invalid device id. CNcomment:不正确的设备号CNend
\retval ::HI_ERR_GFX2D_DEV_NOT_OPEN Device not opened. CNcomment:设备未打开CNend
\retval ::HI_ERR_GFX2D_TIMEOUT Tasks are not finished in appointed time. CNcomment:任务未在指定的超时时间内完成CNend
\see N/A
*/
HI_S32 HI_GFX2D_WaitAllDone(const HI_GFX2D_DEV_ID_E enDevId, const HI_U32 u32TimeOut);


#ifdef __cplusplus
#if __cplusplus
}
#endif  /*__cplusplus*/
#endif  /*__cplusplus*/


#endif  /*_HI_GFX2D_API_H_*/
