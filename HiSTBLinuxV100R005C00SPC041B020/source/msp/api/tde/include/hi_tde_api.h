/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon), 
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name           : hi_tde_api.h
Version             : Initial Draft
Author              : 
Created             : 2014/06/20
Description         : 
Function List       :                    
History             :
Date                        Author                  Modification
2014/06/20                  y00181162               Created file        
******************************************************************************/
#ifndef  __HI_TDE_API_H__
#define  __HI_TDE_API_H__

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif /* __cplusplus */
#endif  /* __cplusplus */

#include "hi_type.h"
#include "hi_tde_type.h"

#define HI_TDE_Open HI_TDE2_Open
#define HI_TDE_Close HI_TDE2_Close
#define HI_TDE_BeginJob HI_TDE2_BeginJob

/****************************************************************************/
/*                             TDE2 API define                               */
/****************************************************************************/

/******************************* API Declaration *****************************/
/** \addtogroup      TDE */
/** @{ */  /** <!—[TDE] */

/** 
\brief Starts a two-dimensional engine 2 (TDE2) device. CNcomment:打开TDE2设备CNend
\attention \n
Before using the TDE, you must call this API to ensure that a TDE device is started.\n
A TDE device can be started repeatedly by using multiple processes.\n.
CNcomment:在进行TDE相关操作前应该首先调用此接口，保证TDE设备处于打开状态\n
TDE设备允许多进程重复打开\n  CNend
 
\param  N/A
\retval ::HI_SUCCESS Success. CNcomment:成功CNend
\retval ::HI_ERR_TDE_DEV_OPEN_FAILED Starting a TDE device fails. CNcomment:TDE设备打开失败CNend
\see \n
N/A
*/
HI_S32      HI_TDE2_Open(HI_VOID);


/** 
\brief Stops a TDE2 device. CNcomment:关闭TDE2设备CNend
\attention \n
HI_TDE2_Open must work with HI_TDE2_Close.\n
CNcomment:调用HI_TDE2_Open与HI_TDE2_Close的次数需要对应\n  CNend
\param  N/A
\retval N/A
\see \n
N/A
*/
HI_VOID     HI_TDE2_Close(HI_VOID);


/** 
\brief Creates a TDE task.
\attention N/A
\param  N/A
\retval If the return value is greater than 0, it indicates that the TDE task handle is valid. CNcomment:大于0：合法的TDE任务句柄CNend
\retval ::HI_ERR_TDE_DEV_NOT_OPEN  Calling this API fails because no TDE device is started. CNcomment:TDE设备未打开，API调用失败CNend
\retval ::HI_ERR_TDE_INVALID_HANDLE The handle of a TDE task is invalid. CNcomment:非法的TDE任务句柄CNend
\see \n
N/A
*/
TDE_HANDLE  HI_TDE2_BeginJob(HI_VOID);


/** 
\brief Submits an existing TDE task. CNcomment:提交已创建的TDE任务CNend
\attention \n
Before calling this API, you must call HI_TDE2_Open to start a TDE device and call HI_TDE2_BeginJob to obtain a valid task handle.\n
If the calling mode is set to block mode, and the API is returned due to timeout or interruption, the operation continues till it is complete even though the API called by the TDE is returned in advance.\n
The synchronization mode is not supported currently.\n
After a task is submitted, its handle becomes invalid. If the task is submitted again, the error code HI_ERR_TDE_INVALID_HANDLE is returned. \n.
CNcomment:在调用此接口前应保证调用HI_TDE2_Open打开TDE设备，并且调用HI_TDE2_BeginJob获得了有效的任务句柄\n
若设置为阻塞操作，函数超时返回或被中断返回时应该注意：此时TDE操作的API函数提前返回，但执行的操作仍会完成\n
暂不支持同步模式\n
提交任务后，此任务对应的handle会变为无效，再次提交会出现错误码HI_ERR_TDE_INVALID_HANDLE \n  CNend

\param[in] s32Handle  Handle of a TDE task. CNcomment:TDE任务句柄CNend
\param[in] bSync  Whether to submit a TDE task in synchronization mode. CNcomment:是否使用同步方式提交CNend
\param[in] bBlock  Block flag. CNcomment:阻塞标志CNend
\param[in] u32TimeOut  Timeout period 10 ms. CNcomment:超时时间，单位jiffies（10ms）CNend
\retval ::HI_SUCCESS  A task is submitted successfully in non-block mode. CNcomment:非阻塞任务提交成功CNend
\retval ::HI_ERR_TDE_INVALID_HANDLE  The input task handle is invalid. CNcomment:输入的Job handler无效CNend
\retval ::HI_ERR_TDE_JOB_TIMEOUT  A task is not completed due to timeout. CNcomment:任务超时未完成CNend
\see \n
N/A
*/
HI_S32      HI_TDE2_EndJob(TDE_HANDLE s32Handle, HI_BOOL bSync, HI_BOOL bBlock, HI_U32 u32TimeOut);


/** 
\brief Cancels an operation added in a TDE task. CNcomment:取消已经加入到TDE任务中的操作CNend
\attention \n
Before calling this API, you must call HI_TDE2_Open to start a TDE device and call HI_TDE2_BeginJob to obtain a valid task handle.\n
A submitted task cannot be cancelled.\n
No operations can be added to cancelled tasks, because the cancelled tasks are invalid.\n
This API is called when an error occurs during TDE operation configuration.\n
CNcomment:在调用此接口前应保证调用HI_TDE2_Open打开TDE设备，并且调用HI_TDE2_BeginJob获得了有效的任务句柄\n
已经提交的任务不能够再取消\n
取消后的任务不再有效，不能添加操作\n
此接口的应用场景：在配置TDE操作出错时使用\n  CNend

\param[in] s32Handle Handle of a TDE task. CNcomment:任务句柄CNend
\retval 0 success. CNcomment:成功CNend
\retval ::HI_ERR_TDE_DEV_NOT_OPEN  Calling this API fails because no TDE device is started. CNcomment:TDE设备未打开，API调用失败CNend
\retval ::HI_FAILURE  The specified task cannot be cancelled because it has been submitted. CNcomment:指定的任务已经提交无法取消CNend

\see \n
N/A
*/
HI_S32      HI_TDE2_CancelJob(TDE_HANDLE s32Handle);



/** 
\brief Waits for the completion of a specified TDE task. CNcomment:等待指定的任务完成CNend
\attention \n
 As a block interface, this API is blocked until the specified task is complete.\n
 CNcomment:此接口为阻塞接口，会阻塞等待指定的任务完成\n  CNend
 
\param[in] s32Handle Handle of a TDE task. CNcomment:任务句柄CNend
\retval 0  The specified TDE task is not completed. CNcomment:指定的TDE任务未完成CNend
\retval ::HI_ERR_TDE_DEV_NOT_OPEN  Calling this API fails because no TDE device is started. CNcomment:TDE设备未打开，API调用失败CNend
\retval ::HI_ERR_TDE_INVALID_HANDLE  The handle of a TDE task is invalid. CNcomment:非法的任务句柄CNend
\retval ::HI_ERR_TDE_QUERY_TIMEOUT  The specified task is not completed due to timeout. CNcomment:指定的任务超时未完成CNend
\retval ::HI_ERR_TDE_UNSUPPORTED_OPERATION  The operation is not supported. CNcomment:不支持的操作CNend
\retval ::HI_ERR_TDE_INTERRUPT Blocked job was interrupted.CNcomment:阻塞的TDE任务被打断CNend
\see \n
N/A
*/
HI_S32      HI_TDE2_WaitForDone(TDE_HANDLE s32Handle);


/** 
\brief Waits for the completion of all TDE tasks. CNcomment:等待TDE的所有任务完成CNend
\attention \n
As a block interface, this API is blocked until all TDE tasks are complete.\n
CNcomment:此接口为阻塞接口，会阻塞等待所有的TDE任务完成\n  CNend
\param  N/A
\retval 0  The specified TDE task is not completed. CNcomment:指定的TDE任务未完成CNend
\retval ::HI_ERR_TDE_DEV_NOT_OPEN  Calling this API fails because no TDE device is started. CNcomment:TDE设备未打开，API调用失败CNend
\retval ::HI_ERR_TDE_UNSUPPORTED_OPERATION  The operation is not supported. CNcomment:不支持的操作CNend
\retval ::HI_ERR_TDE_INTERRUPT Blocked job was interrupted.CNcomment:阻塞的TDE任务被打断CNend
\see \n
N/A
*/
HI_S32 HI_TDE2_WaitAllDone(HI_VOID);


/** 
\brief Resets all statuses of the TDE. CNcomment:复位TDE所有状态CNend
\attention \n
This API is used to reset software and hardware if a timeout error occurs due to the inconsistency of the software and hardware for standby wakeup.\n
CNcomment:此接口一般用于待机唤醒软硬件不匹配时出现超时错误时调用，用于复位软硬件\n  CNend
\param  N/A
\retval 0  TDE reset successfully. CNcomment:TDE复位成功CNend
\retval ::HI_ERR_TDE_DEV_NOT_OPEN  Calling this API fails because no TDE device is started. CNcomment:TDE设备未打开，API调用失败CNend
\see \n
N/A
*/
HI_S32 HI_TDE2_Reset(HI_VOID);


/** 
\brief Adds the fast copy operation to a TDE task. CNcomment:向任务中添加快速拷贝操作CNend
\attention \n
HI_TDE2_QuickCopy adopts the direct memory access (DMA) transfer technology; therefore, HI_TDE2_QuickCopy is superior to HI_TDE2_Bitblit in transfer.
Because the format conversion is not supported during fast copying, the formats of the source bitmap and the target bitmap must be the same.\n
In addition, scaling is not supported during fast copying. If the operating area size of the source bitmap is different from that of the target bitmap, the copy and transfer operations are performed based on the minimum width and height.\n
The specified operating area and the specified bitmap must have a public area; otherwise, an error is returned. This requirement is applicable to other operations.\n
If the pixel format of a bitmap is greater than or equal to a byte, the base address and stride of the bitmap format must be aligned based on the pixel format. If the pixel format of a bitmap is smaller than a byte, the base address and stride of the bitmap must be aligned based on byte. This requirement is applicable to other operations.\n
If the pixel format of a bitmap is smaller than a byte, the horizontal start point and width of the bitmap must be aligned based on pixel.\n
The horizontal start point and width of the YCbCr422 bitmap must be even numbers. This requirement is applicable to other operations.\n
CNcomment:此接口使用的是直接DMA搬移，因此性能优于HI_TDE2_Bitblit搬移\n
快速拷贝操作不支持格式转换，源位图和目标位图格式必须一致\n
快速拷贝不支持缩放功能，因此如果源和目的的操作区域尺寸不一致，则按照两者最小的宽高进行拷贝搬移\n
指定的操作区域要和指定的位图有公共区域，否则会返回错误；其他操作均有此要求\n
像素格式大于等于Byte的位图格式的基地址和位图的Stride必须按照像素格式对齐，像素格式不足Byte的位图格式的基地址和Stride需要按照Byte对齐；其他操作均有此要求\n
像素格式不足Byte的位图格式的水平起始位置和宽度必须按照像素对齐\n
YCbCr422格式的位图的水平起始位置和宽度必须为偶数；其他操作均有此要求\n  CNend

\param[in] s32Handle  Handle of a TDE task. CNcomment:TDE任务句柄CNend
\param[in] pstSrc  Source bitmap. CNcomment:源位图CNend
\param[in] pstSrcRect  Operating area of the source bitmap. CNcomment:源位图操作区域CNend
\param[in] pstDst  Target bitmap. CNcomment:目标位图CNend
\param[in] pstDstRect  Operating area of the target bitmap. CNcomment:目标位图操作区域CNend
\retval 0 success. CNcomment:成功CNend
\retval ::HI_ERR_TDE_DEV_NOT_OPEN  Calling this API fails because no TDE device is started. CNcomment:TDE设备未打开，API调用失败CNend
\retval ::HI_ERR_TDE_NULL_PTR  The parameter contains a null pointer. CNcomment:参数中有空指针错误CNend
\retval ::HI_ERR_TDE_INVALID_HANDLE  The handle of a TDE task is invalid. CNcomment:非法的任务句柄CNend
\retval ::HI_ERR_TDE_INVALID_PARA  The parameter is invalid. CNcomment:无效的参数设置CNend
\retval ::HI_ERR_TDE_NO_MEM  Adding an operation fails due to insufficient memory. CNcomment:内存不足，无法添加操作CNend
\retval ::HI_FAILURE  A system error or an unknown error occurs.CNcomment:系统错误或未知错误CNend
\see \n
N/A
*/
HI_S32      HI_TDE2_QuickCopy(TDE_HANDLE s32Handle, TDE2_SURFACE_S* pstSrc, TDE2_RECT_S  *pstSrcRect,
                              TDE2_SURFACE_S* pstDst, TDE2_RECT_S *pstDstRect);


/** 
\brief Rapidly fills in the target bitmap with a fixed value. CNcomment:快速将固定值填充到目的位图CNend
\attention \n
The data to be filled in must be specified based on the format of the target bitmap.\n
CNcomment:填充的数据值需要按照目标位图的格式指定\n  CNend
\param[in] s32Handle  Handle of a TDE task. CNcomment:TDE任务句柄CNend
\param[in] pstDst  Target bitmap. CNcomment:目标位图CNend
\param[in] pstDstRect  Operating area of the target bitmap. CNcomment:目标位图操作区域CNend
\param[in] u32FillData  Data to be filled in. CNcomment:填充值CNend
\retval 0 success. CNcomment:成功CNend
\retval ::HI_ERR_TDE_DEV_NOT_OPEN  Calling this API fails because no TDE device is started. CNcomment:TDE设备未打开，API调用失败CNend
\retval ::HI_ERR_TDE_NULL_PTR  The parameter contains a null pointer. CNcomment:参数中有空指针错误CNend
\retval ::HI_ERR_TDE_INVALID_HANDLE  The handle of a TDE task is invalid. CNcomment:非法的任务句柄CNend
\retval ::HI_ERR_TDE_INVALID_PARA  The parameter is invalid. CNcomment:无效的参数设置CNend
\retval ::HI_ERR_TDE_NO_MEM  Adding an operation fails due to insufficient memory. CNcomment:内存不足，无法添加操作CNend
\retval ::HI_FAILURE  A system error or an unknown error occurs. CNcomment:系统错误或未知错误CNend
\see \n
N/A
*/
HI_S32      HI_TDE2_QuickFill(TDE_HANDLE s32Handle, TDE2_SURFACE_S* pstDst, TDE2_RECT_S *pstDstRect,
                              HI_U32 u32FillData);


/** 
\brief Scales the source bitmap based on a specified size of the target bitmap. The source bitmap and target bitmap can be the same.
CNcomment:将源位图缩放为目标位图指定大小，源和目标可为同一位图CNend
\attention \n
The maximum minification is smaller than 256, there is no limit on magnification,but the max size of the bitmap is 4095*4095\n
You can scale a bitmap that serves as both source bitmap and target bitmap. If the memories for storing the source bitmap and target bitmap overlap, the bitmaps cannot be scaled.\n
CNcomment:缩小倍数小于256倍，放大倍数则没有限制，但最大尺寸不能超过4095*4095 \n
缩放时源位图和目标位图可以为同一位图，但不能为内存有重叠的不同位图\n  CNend

\param[in] s32Handle  Handle of a TDE task. CNcomment:TDE任务句柄CNend
\param[in] pstSrc  Source bitmap. CNcomment:源位图CNend
\param[in] pstSrcRect  Operating area of the source bitmap. CNcomment:源位图操作区域CNend
\param[in] pstDst  Target bitmap. CNcomment:目标位图CNend
\param[in] pstDstRect  Operating area of the target bitmap. CNcomment:目标位图操作区域CNend
\retval 0 success. CNcomment:成功CNend
\retval ::HI_ERR_TDE_DEV_NOT_OPEN  Calling this API fails because no TDE device is started. CNcomment:TDE设备未打开，API调用失败CNend
\retval ::HI_ERR_TDE_NULL_PTR  The parameter contains a null pointer. CNcomment:参数中有空指针错误CNend
\retval ::HI_ERR_TDE_INVALID_HANDLE  The handle of a TDE task is invalid. CNcomment:非法的任务句柄CNend
\retval ::HI_ERR_TDE_INVALID_PARA  The parameter is invalid. CNcomment:无效的参数设置CNend
\retval ::HI_ERR_TDE_NO_MEM  Adding an operation fails due to insufficient memory. CNcomment:内存不足，无法添加操作CNend
\retval ::HI_ERR_TDE_NOT_ALIGNED  The start address or stride of the bitmap is not aligned based on pixel. CNcomment:位图起始地址或Stride没有按照像素对齐CNend
\retval ::HI_ERR_TDE_MINIFICATION  The start address or stride of the bitmap is not aligned based on pixel. CNcomment:缩小倍数过大CNend
\retval ::HI_ERR_TDE_NOT_ALIGNED  The start address of the color look-up table (CLUT) is not 4-byte aligned. CNcomment:Clut表的起始地址没有按照4byte对齐CNend
\retval ::HI_ERR_TDE_UNSUPPORTED_OPERATION  The operation is not supported. CNcomment:不支持的操作配置CNend
\retval ::HI_FAILURE  A system error or an unknown error occurs. CNcomment:系统错误或未知错误CNend
\see \n
N/A
*/
HI_S32      HI_TDE2_QuickResize(TDE_HANDLE s32Handle, TDE2_SURFACE_S* pstSrc, TDE2_RECT_S  *pstSrcRect,
                                TDE2_SURFACE_S* pstDst, TDE2_RECT_S  *pstDstRect);


/** 
\brief Adds the anti-flicker operation to a TDE task. CNcomment:向任务中添加抗闪烁操作 CNend
\attention \n
The anti-flicker operation supports vertical filtering only.\n
The anti-flicker operation can be performed on the source bitmap and target bitmap that are stored in the same memory.\n
If the sizes of the specified input area and output area are different, scaling is performed.\n
CNcomment:抗闪烁只按垂直方向进行滤波\n
抗闪烁的源和目的位图可以在同一块内存\n
如果指定的输入区域与输出不一致，则会进行缩放处理\n  CNend
 	
\param[in] s32Handle  Handle of a TDE task. CNcomment:TDE任务句柄CNend
\param[in] pstSrc  Source bitmap. CNcomment:源位图CNend
\param[in] pstSrcRect  Operating area of the source bitmap. CNcomment:源位图操作区域CNend
\param[in] pstDst  Target bitmap. CNcomment:目标位图CNend
\param[in] pstDstRect  Operating area of the target bitmap. CNcomment:目标位图操作区域CNend
\retval 0 success. CNcomment:成功CNend
\retval ::HI_ERR_TDE_DEV_NOT_OPEN  Calling this API fails because no TDE device is started. CNcomment:TDE设备未打开，API调用失败CNend
\retval ::HI_ERR_TDE_NULL_PTR  The parameter contains a null pointer. CNcomment:参数中有空指针错误CNend
\retval ::HI_ERR_TDE_INVALID_HANDLE  The handle of a TDE task is invalid. CNcomment:非法的任务句柄CNend
\retval ::HI_ERR_TDE_INVALID_PARA  The parameter is invalid. CNcomment:无效的参数设置CNend
\retval ::HI_ERR_TDE_NO_MEM  Adding an operation fails due to insufficient memory. CNcomment:内存不足，无法添加操作CNend
\retval ::HI_ERR_TDE_NOT_ALIGNED  The start address or stride of the bitmap is not aligned based on pixel. CNcomment:位图起始地址或Stride没有按照像素对齐CNend
\retval ::HI_ERR_TDE_UNSUPPORTED_OPERATION  The operation is not supported. CNcomment:不支持的操作配置CNend
\retval ::HI_ERR_TDE_MINIFICATION  The minification is too large. CNcomment:缩小倍数过大CNend
\retval ::HI_FAILURE  A system error or an unknown error occurs. 系统错误或未知错误CNend
\see \n
N/A
*/
HI_S32      HI_TDE2_QuickDeflicker(TDE_HANDLE s32Handle, TDE2_SURFACE_S* pstSrc, TDE2_RECT_S  *pstSrcRect,
                                   TDE2_SURFACE_S* pstDst, TDE2_RECT_S  *pstDstRect);



/** 
\brief Adds the transfer operation (with additional functions available for the raster or macroblock bitmap) to a TDE task.
CNcomment:向任务中添加对光栅/宏块位图进行有附加功能的搬移操作CNend
\attention \n
Before calling this API, you must call HI_TDE2_Open to start a TDE device and call HI_TDE2_BeginJob to obtain a valid task handle.\n
If the sizes of the foreground bitmap and target bitmap are different, scaling is performed based on the preset scaling area. If no scaling area is set, the clip and transfer operations are performed based on the minimum value of the public area.\n
The value of the global alpha, Alpha0, or Alpha1 ranges from 0 to 255.\n
The background bitmap and the target bitmap can be the same.\n
If only the single-source transfer operation with additional functions,you can set the structure information and the pointer of the operating area of the background or the background bitmap to null,but the pointer to the operation option type can't be null,and it doesn't support ColorKey and Blend.\n
Scaling is not supported if the mirror function is enabled.\n
If only the single-source transfer operation (for example, performing the ROP and reverse operations on the source bitmap only) is required, you can set the structure information and the pointer of the operating area of the background or the background bitmap to null.\n
Scaling is not supported if the mirror function is enabled.\n
The clip operation can be performed only when the clipping area overlaps the operating area; otherwise, an error occurs.\n
When the color extension (from CLUT to ARGB/AYCbCr) is performed for the first time, the CLUT Reload flag function must be enabled.\n
During a simple transfer operation (the operation does not contain additional functions and the pixel format of source and target bitmaps are the same), you can set the pointer to the operation option type to null. Then fast transfer is implemented.\n
The pixel format of the target bitmap cannot be the macroblock format.\n
CNcomment:在调用此接口前应保证调用HI_TDE2_Open打开TDE设备，并且调用HI_TDE2_BeginJob获得了有效的任务句柄\n
当前景源位图与目标位图尺寸不一致时，如果设置了缩放则按照设定的区域进行缩放，否则按照设置公共区域的最小值进行裁减搬移\n
Global Alpha和Alplh0、Alpha1的设置值统一按照[0,255]的范围进行设置\n
背景位图可以与目标位图为同一位图\n
当只需要使用单源搬移操作且带附加功能时，可以将背景或背景位图的结构信息和操作区域结构指针设置为空，操作选项结构体指针不可为空。不支持单源Blend和ColorKey功能\n
不能够在启用镜像的同时进行缩放\n
作Clip操作时，裁减区域必须与操作区域有公共交集，否则会返回错误\n
在第一次作颜色扩展操作（源为Clut格式，目的为ARGB/AYCbCr格式）时，需要打开Clut Reload标记\n
作简单的搬移操作(不带附加功能，且源和目标的象素格式一致)时，可以将操作选项结构体指针设置为空,这时会进行快速搬移\n
不支持目标象素格式为宏块格式\n  CNend

\param[in] s32Handle  Handle of a TDE task. CNcomment:TDE任务句柄CNend
\param[in] pstBackGround  Background bitmap. CNcomment:背景位图CNend
\param[in] pstBackGroundRect  Operating area of the background bitmap. CNcomment:背景位图操作区域CNend
\param[in] pstForeGround  Foreground bitmap. CNcomment:前景位图CNend
\param[in] pstForeGroundRect  Operating area of the foreground bitmap. CNcomment:前景位图操作区域CNend
\param[in] pstDst  Target bitmap. CNcomment:目标位图CNend
\param[in] pstDstRect  Operating area of the target bitmap. CNcomment:目标位图操作区域CNend
\param[in] pstOpt  Structure for setting operation parameters. CNcomment:运算参数设置结构CNend
\retval 0 success. CNcomment:成功CNend
\retval ::HI_ERR_TDE_DEV_NOT_OPEN  Calling this API fails because no TDE device is started. CNcomment:TDE设备未打开，API调用失败CNend
\retval ::HI_ERR_TDE_NULL_PTR  The parameter contains a null pointer. CNcomment:参数中有空指针错误CNend
\retval ::HI_ERR_TDE_INVALID_HANDLE  The handle of a TDE task is invalid. CNcomment:非法的任务句柄CNend
\retval ::HI_ERR_TDE_INVALID_PARA  The parameter is invalid. CNcomment:无效的参数设置CNend
\retval ::HI_ERR_TDE_NO_MEM  Adding an operation fails due to insufficient memory. CNcomment:内存不足，无法添加操作CNend
\retval ::HI_ERR_TDE_NOT_ALIGNED  The start address of the CLUT is not 4-byte aligned. CNcomment:Clut表的起始地址没有按照4byte对齐CNend
\retval ::HI_ERR_TDE_MINIFICATION  The minification is too large. CNcomment:缩小倍数过大CNend
\retval ::HI_ERR_TDE_UNSUPPORTED_OPERATION  The bitmap does not support this operation. CNcomment:位图的格式操作不支持此操作CNend
\retval ::HI_ERR_TDE_CLIP_AREA  The operating area does not overlap the clipping area, and the displayed information is not updated. CNcomment:操作区域与Clip区域没有交集，显示不会有更新CNend
\retval ::HI_FAILURE  A system error or an unknown error occurs. CNcomment:系统错误或未知错误
\see \n
N/A
*/
HI_S32      HI_TDE2_Bitblit(TDE_HANDLE s32Handle, TDE2_SURFACE_S* pstBackGround, TDE2_RECT_S  *pstBackGroundRect,
                            TDE2_SURFACE_S* pstForeGround, TDE2_RECT_S  *pstForeGroundRect, TDE2_SURFACE_S* pstDst,
                            TDE2_RECT_S  *pstDstRect, TDE2_OPT_S* pstOpt);


/** 
\brief Adds the fill and transfer operations (with additional functions available for the raster bitmap) to a TDE task. After addition, the functions of drawing a point, drawing a line, filling in a color block on the surface, and filling in a memory can be implemented.
CNcomment:向任务中添加对光栅位图进行有附加操作的填充搬移操作。实现在surface上画点、画线、色块填充或内存填充等功能CNend
\attention \n
Before calling this API, you must call HI_TDE2_Open to start a TDE device and call HI_TDE2_BeginJob to obtain a valid task handle.\n
When the foreground bitmap is null, the functions of this API are the same as those of HI_TDE2_QuickFil.\n
During solid drawing, the foreground bitmap supports the colorkey operation only.\n
To draw a rectangle, a vertical line, or a horizontal line by calling HI_TDE2_SolidDraw, you need to set the width and height of the filled rectangle. For example, drawing a vertical line is to draw a rectangle with the width of one pixel.\n
CNcomment:在调用此接口前应保证调用HI_TDE2_Open打开TDE设备，并且调用HI_TDE2_BeginJob获得了有效的任务句柄\n
当前景位图为NULL时，该接口和HI_TDE2_QuickFill实现的功能一样\n
在Solid Draw时，只能对前景做Color Key\n
Solid Draw绘制矩形或者水平/垂直直线的方法是通过设置填充矩形的宽/高来完成。例如：垂直直线就是绘制宽度为1像素的矩形\n  CNend
 	 	 	 	
\param[in] s32Handle  Handle of a TDE task. CNcomment:TDE任务句柄CNend
\param[in] pstForeGround  Foreground bitmap. CNcomment:前景位图CNend
\param[in] pstForeGroundRect  Operating area of the foreground bitmap. CNcomment:前景位图的操作区域CNend
\param[in] pstDst  Target bitmap. CNcomment:目标位图CNend
\param[in] pstDstRect  Operating area of the target bitmap. CNcomment:目标位图的操作区域CNend
\param[in] pstFillColor  Structure of fill colors. CNcomment:填充色结构体CNend
\param[in] pstOpt  Structure of operation attributes. CNcomment:操作属性结构体CNend
\retval 0 success. CNcomment:成功CNend
\retval ::HI_ERR_TDE_DEV_NOT_OPEN  Calling this API fails because no TDE device is started. CNcomment:TDE设备未打开，API调用失败CNend
\retval ::HI_ERR_TDE_NULL_PTR  The parameter contains a null pointer. CNcomment:参数中有空指针错误CNend
\retval ::HI_ERR_TDE_INVALID_HANDLE  The handle of a TDE task is invalid. CNcomment:非法的任务句柄CNend
\retval ::HI_ERR_TDE_INVALID_PARA  The parameter is invalid. CNcomment:无效的参数设置CNend
\retval ::HI_ERR_TDE_NO_MEM  Adding an operation fails due to insufficient memory. CNcomment:内存不足，无法添加操作CNend
\retval ::HI_ERR_TDE_NOT_ALIGNED  The start address of the CLUT is not 4-byte aligned. CNcomment:Clut表的起始地址没有按照4byte对齐CNend
\retval ::HI_ERR_TDE_MINIFICATION  The minification is too large. CNcomment:缩小倍数过大CNend
\retval ::HI_ERR_TDE_UNSUPPORTED_OPERATION  The bitmap does not support this operation. CNcomment:位图的格式操作不支持此操作CNend
\retval ::HI_ERR_TDE_CLIP_AREA  The operating area does not overlap the clipping area, and the displayed information is not updated. CNcomment:操作区域与Clip区域没有交集，显示不会有更新CNend
\retval ::HI_FAILURE  A system error or an unknown error occurs. CNcomment:系统错误或未知错误CNend
\see \n
N/A
*/
HI_S32      HI_TDE2_SolidDraw(TDE_HANDLE s32Handle, TDE2_SURFACE_S* pstForeGround, TDE2_RECT_S  *pstForeGroundRect,
                              TDE2_SURFACE_S *pstDst,
                              TDE2_RECT_S  *pstDstRect, TDE2_FILLCOLOR_S *pstFillColor,
                              TDE2_OPT_S *pstOpt);


/** 
\brief Adds the transfer operation (with additional functions available for the macroblock bitmap) to a TDE task. That is, the luminance macroblock data and the chrominance macroblock data are combined into raster data. During the combination, the scaling, anti-flicker, and clip operations can be performed concurrently.
CNcomment:向任务中添加对宏块位图进行有附加功能的搬移操作。将亮度和色度宏块数据合并成光栅格式，可以伴随缩放、抗闪烁、Clip处理CNend
\attention \n
Before calling this API, you must call HI_TDE2_Open to start a TDE device and call HI_TDE2_BeginJob to obtain a valid task handle.\n
For the YCbCr422 macroblock, the horizontal coordination of the start point and the width of the operating area must be an even number.\n
The pixel format of the target bitmap cannot be the macroblock format.\n
CNcomment:在调用此接口前应保证调用HI_TDE2_Open打开TDE设备，并且调用HI_TDE2_BeginJob获得了有效的任务句柄\n
对于YCbCr422格式的宏块，操作区域起始点水平坐标以及宽度必须是偶数\n
不支持目标象素格式为宏块格式\n  CNend
 	 	 	
\param[in] s32Handle  Handle of a TDE task. CNcomment:TDE任务句柄CNend
\param[in] pstMB  Surface of a macroblock. CNcomment:宏块surface  CNend
\param[in] pstMbRect  Operating area of a macroblock. CNcomment:宏块操作区域CNend
\param[in] pstDst  Target bitmap. CNcomment:目标位图CNend
\param[in] pstDstRect  Operating area of the target bitmap. CNcomment:目标操作区域CNend
\param[in] pstMbOpt  Operation attributes of a macroblock. CNcomment:宏块操作属性CNend
\retval 0 success. CNcomment:成功CNend
\retval ::HI_ERR_TDE_DEV_NOT_OPEN  Calling this API fails because no TDE device is started. CNcomment:TDE设备未打开，API调用失败CNend
\retval ::HI_ERR_TDE_NULL_PTR  The parameter contains a null pointer. CNcomment:参数中有空指针错误CNend
\retval ::HI_ERR_TDE_INVALID_HANDLE  The handle of a TDE task is invalid. CNcomment:非法的任务句柄CNend
\retval ::HI_ERR_TDE_INVALID_PARA  The parameter is invalid. CNcomment:无效的参数设置CNend
\retval ::HI_ERR_TDE_NO_MEM  Adding an operation fails due to insufficient memory. CNcomment:内存不足，无法添加操作CNend
\retval ::HI_ERR_TDE_MINIFICATION  The minification is too large because its maximum value is 255. CNcomment:缩小倍数超出限制（最大为缩小255倍）CNend
\retval ::HI_ERR_TDE_UNSUPPORTED_OPERATION  The bitmap does not support this operation. CNcomment:位图的格式操作不支持此操作CNend
\retval ::HI_ERR_TDE_CLIP_AREA  The operating area does not overlap the clipping area, and the displayed information is not updated. CNcomment:操作区域与Clip区域没有交集，显示不会有更新CNend
\retval ::HI_FAILURE  A system error or an unknown error occurs. CNcomment:系统错误或未知错误CNend
\see \n
N/A
*/
HI_S32      HI_TDE2_MbBlit(TDE_HANDLE s32Handle, TDE2_MB_S* pstMB, TDE2_RECT_S  *pstMbRect, TDE2_SURFACE_S* pstDst, TDE2_RECT_S  *pstDstRect,
                           TDE2_MBOPT_S* pstMbOpt);


/** 
\brief Adds the mask raster of operation (ROP) (available for the raster bitmap) to a TDE task. That is, the ROP operation is performed on the foreground bitmap and the background bitmap based on the mask bitmap.
CNcomment:向任务中添加对光栅位图进行Mask Rop搬移操作。根据Mask位图实现前景位图和背景位图ROP的效果CNend
\attention \n
Before calling this API, you must call HI_TDE2_Open to start a TDE device and call HI_TDE2_BeginJob to obtain a valid task handle.\n
The sizes of the operating areas of the mask bitmap, foreground bitmap, and background bitmap must be the same.\n
The mask bitmap must be in A1 format.\n
The target bitmap and foreground bitmap must be in the same color space.\n
CNcomment:在调用此接口前应保证调用HI_TDE2_Open打开TDE设备，并且调用HI_TDE2_BeginJob获得了有效的任务句柄\n  CNend
三个位图的操作区域大小必须一致\n
Mask位图必须是A1格式的位图\n
目标位图和前景位图必须位于同一颜色空间\n  CNend
 	 	 	 	
\param[in] s32Handle  Handle of a TDE task. CNcomment:TDE任务句柄CNend
\param[in] pstBackGround  Background bitmap. CNcomment:背景位图CNend
\param[in] pstBackGroundRect  Operating area of the background bitmap. CNcomment:背景位图操作区域CNend
\param[in] pstForeGround  Foreground bitmap. CNcomment:前景位图CNend
\param[in] pstForeGroundRect  Operating area of the foreground bitmap. CNcomment:前景位图操作区域CNend
\param[in] pstMask  Mask bitmap. CNcomment:Mask位图CNend
\param[in] pstMaskRect  Operating area of the mask bitmap. CNcomment:Mask位图操作区域CNend
\param[in] pstDst  Target bitmap. CNcomment:目标位图CNend
\param[in] pstDstRect  Operating area of the target bitmap. CNcomment:目标位图操作区域CNend
\param[in] enRopCode_Color  ROP code of a color component. CNcomment:颜色分量的ROP运算码CNend
\param[in] enRopCode_Alpha  ROP code of the alpha component. CNcomment:Alpha分量的ROP运算码CNend
\retval 0 success. CNcomment:成功CNend
\retval ::HI_ERR_TDE_DEV_NOT_OPEN  Calling this API fails because no TDE device is started. CNcomment:TDE设备未打开，API调用失败CNend
\retval ::HI_ERR_TDE_NULL_PTR  The parameter contains a null pointer. CNcomment:参数中有空指针错误CNend
\retval ::HI_ERR_TDE_INVALID_HANDLE  The handle of a TDE task is invalid. CNcomment:非法的任务句柄CNend
\retval ::HI_ERR_TDE_INVALID_PARA  The parameter is invalid. CNcomment:无效的参数设置CNend
\retval ::HI_ERR_TDE_NO_MEM  Adding an operation fails due to insufficient memory. CNcomment:内存不足，无法添加操作CNend
\retval ::HI_FAILURE  A system error or an unknown error occurs. CNcomment:系统错误或未知错误CNend
\see \n
N/A
*/
HI_S32      HI_TDE2_BitmapMaskRop(TDE_HANDLE s32Handle, 
                                  TDE2_SURFACE_S* pstBackGround, TDE2_RECT_S  *pstBackGroundRect,
                                  TDE2_SURFACE_S* pstForeGround, TDE2_RECT_S  *pstForeGroundRect,
                                  TDE2_SURFACE_S* pstMask, TDE2_RECT_S  *pstMaskRect, 
                                  TDE2_SURFACE_S* pstDst, TDE2_RECT_S  *pstDstRect,
                                  TDE2_ROP_CODE_E enRopCode_Color, TDE2_ROP_CODE_E enRopCode_Alpha);


/** 
\brief Adds the mask blend operation (available for the raster bitmap) to a TDE task. That is, the foreground bitmap and the background bitmap are blending with the mask bitmap based on the mask bitmap.
CNcomment:向任务中添加对光栅位图进行Mask Blend搬移操作。根据Mask位图实现前景位图和背景位图带Mask位图的叠加效果CNend
\attention \n
Before calling this API, you must call HI_TDE2_Open to start a TDE device and call HI_TDE2_BeginJob to obtain a valid task handle.\n
The target bitmap and background bitmap must be in the same color space.\n
If the foreground bitmap contains the premultiplied data, the premultiplied mode must be selected during alpha blending; otherwise, the non-premultiplied mode is selected.
The parameter enBlendMode cannot be set to TDE2_ALUCMD_ROP.\n
CNcomment:在调用此接口前应保证调用HI_TDE2_Open打开TDE设备，并且调用HI_TDE2_BeginJob获得了有效的任务句柄\n
目标位图和背景位图必须位于同一颜色空间\n
如果前景的位图是预乘了的数据，Alpha叠加模式应该选择预乘模式；否则选择非预乘模式\n
enBlendMode不能选择TDE2_ALUCMD_ROP模式\n  CNend

\param[in] s32Handle   Handle of a TDE task. CNcomment:TDE任务句柄CNend
\param[in] pstBackGround  Background bitmap. CNcomment:背景位图CNend
\param[in] pstBackGroundRect  Operating area of the background bitmap. CNcomment:背景位图操作区域CNend
\param[in] pstForeGround  Foreground bitmap. CNcomment:前景位图
\param[in] pstForeGroundRect  Operating area of the foreground bitmap.CNcomment: 前景位图操作区域CNend
\param[in] pstMask  Mask bitmap. CNcomment:Mask位图CNend
\param[in] pstMaskRect  Operating area of the mask bitmap. CNcomment:Mask位图操作区域CNend
\param[in] pstDst  Target bitmap. CNcomment:目标位图
\param[in] pstDstRect  Operating area of the target bitmap. CNcomment:目标位图操作区域CNend
\param[in] u8Alpha  Global alpha value during alpha blending. CNcomment:Alpha叠加时的全局Alpha值CNend
\param[in] enBlendMode  Alpha blending mode. CNcomment:Alpha叠加模式选择CNend
\retval 0 success. CNcomment:成功CNend
\retval ::HI_ERR_TDE_DEV_NOT_OPEN  Calling this API fails because no TDE device is started. CNcomment:TDE设备未打开，API调用失败CNend
\retval ::HI_ERR_TDE_NULL_PTR  The parameter contains a null pointer. CNcomment:参数中有空指针错误CNend
\retval ::HI_ERR_TDE_INVALID_HANDLE  The handle of a TDE task is invalid. CNcomment:非法的任务句柄CNend
\retval ::HI_ERR_TDE_INVALID_PARA  The parameter is invalid. CNcomment:无效的参数设置
\retval ::HI_ERR_TDE_NO_MEM  Adding an operation fails due to insufficient memory. CNcomment:内存不足，无法添加操作CNend
\retval ::HI_FAILURE  A system error or an unknown error occurs. CNcomment:系统错误或未知错误CNend
\see \n
N/A
*/
HI_S32      HI_TDE2_BitmapMaskBlend(TDE_HANDLE s32Handle, 
                                    TDE2_SURFACE_S* pstBackGround, TDE2_RECT_S  *pstBackGroundRect,
                                    TDE2_SURFACE_S* pstForeGround, TDE2_RECT_S  *pstForeGroundRect,
                                    TDE2_SURFACE_S* pstMask, TDE2_RECT_S  *pstMaskRect,
                                    TDE2_SURFACE_S* pstDst, TDE2_RECT_S  *pstDstRect,
                                    HI_U8 u8Alpha, TDE2_ALUCMD_E enBlendMode);


/** 
\brief  Adds the rotation operation to a TDE task. CNcomment:向任务中添加旋转操作CNend
\attention \n
The pixel formats of the source bitmap and target bitmap must be the same, and the format cannot be YUV, CLUT1, or CLUT4.\n
The parameters s32DstPointX and s32DstPointY are set to 0 by default. Because the two parameters are not used at present, they can be set to any values.\n
CNcomment:源和目标的像素格式必须相同，并且不能是YUV、CLUT1和CLUT4格式\n
参数s32DstPointX和s32DstPointY默认为0，目前这两个参数没有用到，可以输入任何值\n  CNend

\param[in] s32Handle  Handle of a TDE task. CNcomment:TDE任务句柄CNend
\param[in] pstSrc  Source bitmap.CNcomment: 源位图CNend
\param[in] pstSrcRect  Operating area of the source bitmap. CNcomment:源位图操作区域CNend
\param[in] pstDst  Target bitmap. CNcomment:目标位图CNend
\param[in] s32DstPointX  Horizontal coordinate of the start point of the operating area in the target bitmap. CNcomment:目标位图操作区域的起始点横坐标CNend
\param[in] s32DstPointY  Vertical coordinate of the start point of the operating area in the target bitmap. CNcomment:目标位图操作区域的起始点纵坐标CNend
\param[in] enRotateAngle  Rotate angle. CNcomment:旋转的角度CNend
\retval 0 success. CNcomment:成功CNend
\retval ::HI_ERR_TDE_DEV_NOT_OPEN  Calling this API fails because no TDE device is started. CNcomment:TDE设备未打开，API调用失败CNend
\retval ::HI_ERR_TDE_INVALID_HANDLE  The handle of a TDE task is invalid. CNcomment:非法的任务句柄CNend
\retval ::HI_ERR_TDE_NULL_PTR  The parameter contains a null pointer. CNcomment:参数中有空指针错误CNend
\retval ::HI_ERR_TDE_UNSUPPORTED_OPERATION  The operation is not supported. CNcomment:不支持的操作CNend
\retval ::HI_ERR_TDE_INVALID_PARA  The parameter is invalid. CNcomment:无效的参数设置CNend
\see \n
N/A
*/
HI_S32 HI_TDE2_Rotate(TDE_HANDLE s32Handle, 
                    TDE2_SURFACE_S *pstSrc, TDE2_RECT_S *pstSrcRect, 
                    TDE2_SURFACE_S *pstDst, HI_S32 s32DstPointX,  HI_S32 s32DstPointY,
                    TDE_ROTATE_ANGLE_E enRotateAngle);



/** 
\brief Sets the anti-flicker level. CNcomment:设置抗闪烁级别CNend
\attention \n
N/A
\param[in] enDeflickerLevel  Anti-flicker level. CNcomment:抗闪烁系数级别CNend
\retval 0 success. CNcomment:成功CNend
\retval ::

\see none
*/
HI_S32 HI_TDE2_SetDeflickerLevel(TDE_DEFLICKER_LEVEL_E enDeflickerLevel);


/** 
\brief Obtains the anti-flicker level.CNcomment: 获取抗闪烁级别CNend
\attention \n
N/A
\param[in] pDeflickerLevel  Anti-flicker level. CNcomment:抗闪烁级别CNend
\retval 0 success. CNcomment:成功CNend
\retval ::

\see \n
N/A
*/
HI_S32 HI_TDE2_GetDeflickerLevel(TDE_DEFLICKER_LEVEL_E *pDeflickerLevel);


/** 
\brief Sets the alpha judgment threshold. CNcomment:设置alpha判决阈值CNend
\attention \n
N/A
\param[in] u8ThresholdValue  Judgment threshold. CNcomment:判决阈值CNend
\retval 0 success. CNcomment:成功CNend
\retval ::

\see \n
N/A
*/
HI_S32 HI_TDE2_SetAlphaThresholdValue(HI_U8 u8ThresholdValue);


/** 
\brief Obtains the alpha judgment threshold. CNcomment:获取alpha判决阈值CNend
\attention \n
N/A
\param[in] pu8ThresholdValue  Judgment threshold. CNcomment:判决阈值CNend
\retval 0 success. CNcomment:成功CNend
\retval ::

\see \n
N/A
*/
HI_S32 HI_TDE2_GetAlphaThresholdValue(HI_U8 *pu8ThresholdValue);


/** 
\brief Enables or disables the alpha judgment. CNcomment:设置alpha判决开关CNend
\attention  N/A
\param[in] bEnAlphaThreshold  Whether to enable the alpha judgment. CNcomment:alpha判决开关CNend
\retval 0 success. CNcomment:成功CNend
\retval ::
\see \n
N/A
*/
HI_S32 HI_TDE2_SetAlphaThresholdState(HI_BOOL bEnAlphaThreshold);


/** 
\brief Obtains the enable status of the alpha judgment. CNcomment:获取alpha判决开关CNend
\attention N/A
\param[in]  p_bEnAlphaThreshold  Whether to enable the alpha judgment. CNcomment:alpha判决开关CNend
\retval 0 success. CNcomment:成功CNend
\retval ::

\see \n
N/A
*/
HI_S32 HI_TDE2_GetAlphaThresholdState(HI_BOOL * p_bEnAlphaThreshold);


/** 
\brief Fills in the pattern. CNcomment:模式填充CNend
\attention N/A
\param[in] s32Handle  Handle of a TDE task. CNcomment:任务句柄CNend
\param[in] pstBackGround  Background bitmap. CNcomment:背景位图信息CNend
\param[in] pstBackGroundRect  Operating rectangle of the background bitmap. CNcomment:背景操作矩形CNend
\param[in] pstForeGround  Foreground bitmap. CNcomment:前景位图信息CNend
\param[in] pstForeGroundRect  Operating rectangle of the foreground bitmap. CNcomment:前景位操作矩形CNend
\param[in] pstDst  Target bitmap. CNcomment:目标位图信息CNend
\param[in] pstDstRect  Operating rectangle of the target bitmap. CNcomment:目标操作矩形CNend
\param[in] pstOpt  Options for pattern filling. CNcomment:模式填充操作选项CNend
\retval 0 success. CNcomment:成功CNend
\retval ::

\see \n
N/A
*/
HI_S32 HI_TDE2_PatternFill(TDE_HANDLE s32Handle, TDE2_SURFACE_S *pstBackGround, 
                           TDE2_RECT_S *pstBackGroundRect, TDE2_SURFACE_S *pstForeGround,
                           TDE2_RECT_S *pstForeGroundRect, TDE2_SURFACE_S *pstDst,
                           TDE2_RECT_S *pstDstRect, TDE2_PATTERN_FILL_OPT_S *pstOpt);

/** 
\brief Enables or disables the regional anti-flicker function. CNcomment:使能/去使能局部抗闪烁CNend
\attention N/A
\param[in] bRegionDeflicker  Enable/disable flag. CNcomment:使能/去使能标志CNend
\retval 0 success. CNcomment:成功CNend
\retval ::

\see \n
N/A
*/
HI_S32 HI_TDE2_EnableRegionDeflicker(HI_BOOL bRegionDeflicker);

/**
\brief: multi blend surface by surface list
\param[in]: composed surface list
\retval: 0 success
**/
HI_S32 HI_TDE2_MultiBlending(TDE_HANDLE s32Handle, TDE_SURFACE_LIST_S *pstSurfaceList);

/** @} */  /** <!-- ==== API declaration end ==== */
#ifdef __cplusplus
 #if __cplusplus
}
 #endif /* __cplusplus */
#endif  /* __cplusplus */

#endif  /* _HI_API_TDE2_H_ */
