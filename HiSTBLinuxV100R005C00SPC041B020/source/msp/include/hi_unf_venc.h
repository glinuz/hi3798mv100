/******************************************************************************

  Copyright (C), 2001-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_unf_venc.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/04/07
  Description   :
  History       :
  1.Date        : 2010/04/07
    Author      : j00131665
    Modification: Created file

*******************************************************************************/
/** 
 * \file
 * \brief Describes the information about video encoding (VENC). CNcomment: 提供VENC的相关信息 CNend
 */

#ifndef  __HI_UNF_VENC_H__
#define  __HI_UNF_VENC_H__

#include "hi_unf_common.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif

/********************************Struct Definition********************************/
/** \addtogroup      VENC */
/** @{ */  /** <!-- 【VENC】 */

/**H.264 NALU type*/
/**CNcomment: H.264NALU类型 */
typedef enum hiUNF_H264E_NALU_TYPE_E
{
    HI_UNF_H264E_NALU_PSLICE = 1,       /**<P slice NALU*/ 
    HI_UNF_H264E_NALU_ISLICE = 5,       /**<I slice NALU*/ 
    HI_UNF_H264E_NALU_SEI = 6,          /**<SEI NALU*/ 
    HI_UNF_H264E_NALU_SPS = 7,          /**<SPS NALU*/
    HI_UNF_H264E_NALU_PPS = 8,          /**<PPS NALU*/ 
    HI_UNF_H264E_NALU_BUTT
} HI_UNF_H264E_NALU_TYPE_E;

/**H.263 NALU type*/
/**CNcomment: H.263NALU类型 */
typedef enum hiUNF_H263E_PACK_TYPE_E
{
    HI_UNF_H263E_NALU_PSLICE = 1,       /**<P slice NALU*/
    HI_UNF_H263E_NALU_ISLICE = 5,       /**<I slice NALU*/
    HI_UNF_H263E_NALU_SEI = 6,          /**<SEI NALU*/
    HI_UNF_H263E_NALU_SPS = 7,          /**<SPS NALU*/
    HI_UNF_H263E_NALU_PPS = 8,          /**<PPS NALU*/
    HI_UNF_H263E_NALU_BUTT
} HI_UNF_H263E_PACK_TYPE_E;

/**MPEG4 package type*/
/**CNcomment: MPEG4打包类型 */
typedef enum hiUNF_MPEG4E_PACK_TYPE_E
{
    HI_UNF_MPEG4E_PACK_VO    = 1,          /**<VO package*/ /**<CNcomment: VO 包*/
    HI_UNF_MPEG4E_PACK_VOS   = 2,         /**<VOS package*/ /**<CNcomment: VOS 包*/
    HI_UNF_MPEG4E_PACK_VOL   = 3,         /**<VOL package*/ /**<CNcomment: VOL 包*/
    HI_UNF_MPEG4E_PACK_VOP   = 4,         /**<VOP package*/ /**<CNcomment: VOP 包*/
    HI_UNF_MPEG4E_PACK_SLICE = 5        /**<Slice package*/ /**<CNcomment: SLICE 包*/	
} HI_UNF_MPEG4E_PACK_TYPE_E;

/**HEVC NALU type*/
/**CNcomment: HEVC NALU类型 */
typedef enum hiUNF_HEVCE_NALU_TYPE_E
{
    HI_UNF_HEVCE_NALU_PSLICE = 1,       /**<P slice NALU*/ 
    HI_UNF_HEVCE_NALU_ISLICE = 19,       /**<I slice NALU*/ 
    HI_UNF_HEVCE_NALU_VPS = 32,          /**<VPS NALU*/ 
    HI_UNF_HEVCE_NALU_SPS = 33,          /**<SPS NALU*/
    HI_UNF_HEVCE_NALU_PPS = 34,          /**<PPS NALU*/ 
    HI_UNF_HEVCE_NALU_SEI = 39,          /**<SEI NALU*/ 
    HI_UNF_HEVCE_NALU_BUTT
} HI_UNF_HEVCE_NALU_TYPE_E;

/**Data type of the Encoder*/
/**CNcomment: 编码器数据类型 */
typedef union hiUNF_VENC_DATA_TYPE_U
{
    HI_UNF_H264E_NALU_TYPE_E   enH264EType;	/**<H.264 encoding data*/ /**<CNcomment: h264编码数据*/	
    HI_UNF_H263E_PACK_TYPE_E   enH263EType;	/**<H.263 encoding data*/ /**<CNcomment: h263编码数据*/	
    HI_UNF_MPEG4E_PACK_TYPE_E  enMPEG4EType;    /**<MPEG4 encoding data*/ /**<CNcomment: MPEG4编码数据*/
    HI_UNF_HEVCE_NALU_TYPE_E   enHEVCEType;     /**<HEVC encoding data*/ /**<CNcomment: HEVC编码数据*/
}HI_UNF_VENC_DATA_TYPE_U;

/*Output stream attributes structure*/
/**CNcomment: 输出码流属性结构体 */
typedef struct hiVENC_STREAM_S
{
    HI_U8                   *pu8Addr ;       /**<Stream virtual address*/ /**<CNcomment: 码流虚拟地址*/	
    HI_U32                  u32SlcLen;      /**<Stream length*/ /**<CNcomment: 码流长度*/		
    HI_U32                  u32PtsMs;        /**<Presentation time stamp (PTS), in ms*/ /**<CNcomment: 时间戳，单位是毫秒*/
    HI_BOOL                 bFrameEnd;       /**<Frame end or not*/ /**<CNcomment: 标识是否为帧结束*/
    HI_UNF_VENC_DATA_TYPE_U enDataType;      /**Encoding data type*/ /**<CNcomment: 编码数据类型*/
}HI_UNF_VENC_STREAM_S;

/*Coding channal attributes structure*/
/**CNcomment: 编码通道属性结构体 */
typedef struct hiUNF_VENC_CHN_ATTR_S
{
    HI_UNF_VCODEC_TYPE_E        enVencType;	      /**<Encoder type*/ /**<CNcomment: 编码器类型 */
    HI_UNF_VCODEC_CAP_LEVEL_E   enCapLevel;       /**<Encoder level*/ /**<CNcomment: 编码器编码能力 */
	HI_UNF_H264_PROFILE_E       enVencProfile;    /**<H264 Profile of Encoder,it is just valid for H264 Encoder*/ /**<CNcomment: 编码H264的档次，只在定义了H264协议编码器情况下才有效*/
    HI_U32                      u32Width;         /**<Width, 4-byte aligned. The width can be configured dynamically in the limit of encoder level.*/ 
                                                  /**<CNcomment: 宽度,4对齐,编码能力级范围内可支持动态设置 */
    HI_U32                      u32Height;        /**<Height, 4-byte aligned.The height can be configured dynamically in the limit of encoder level.*/ 
                                                  /**<CNcomment: 高度,4对齐,编码能力级范围内可支持动态设置 */
    HI_U32                      u32StrmBufSize;	  /**<Stream buffer size, should be larger than width*height*2 of the encoder level's resolving power.You'd better set larger streamBufferSize if you set larger target bitrate or Qlevel.*/ 
                                                  /**<CNcomment: 码流buffer大小,配置的码流buffer大小要>=编码能力级分辨率的宽*高*2，如果设置了较高的码率或JPEG图像质量参数，建议适当增大码流buffer大小。*/
    HI_U32                      u32RotationAngle; /**<Rotation angle. This parameter cannot be set.It must be set to 0.*/ /**<CNcomment: 旋转角度,无效设置,须配置成0*/
    HI_BOOL                     bSlcSplitEn;      /**<Slice split enable,it is just valid for H264 Encoder*/ /**<CNcomment: 是否使能分割slice,只在定义了H264协议编码器情况下才有效*/ 	
                                                  /**<CNcomment: 分割的大小，JPGE下以MCU为单位,H264或者MP4以字节为单位, H263不关心 */
    HI_U32                      u32TargetBitRate; /**<RC parameter for the VENC,it is just valid for H264 Encoder. It can be set dynamically.Some reference values as follows:above 5M for 1080P,above 3M for 720P, about 2M for D1*/ 
                                                   /**<CNcomment: Venc下是RC参数,仅在编码H264协议有效，可动态设置.部分参考值如下:1080P下5M以上，720P下3M以上，D1下2M左右*/
    HI_U32                      u32TargetFrmRate; /**<Target frame rate. It can be set dynamically.*/         /**<CNcomment: 目标帧率,可动态设置 */ 
    HI_U32                      u32InputFrmRate;  /**<Input frame rate. It can be set dynamically. The value of u32TargetFrmRate is less than or equal to the value of u32InputFrmRate.
	                                               ** Attention the if in the mode of attach source to encode the InputFrmRate which set by user is in vain.*/ 
                                                  /**<CNcomment: 输入帧率,可动态设置,u32TargetFrmRate <= u32InputFrmRate ,注意在绑定模式编码情况下用户配置的输入帧率将失效*/ 
    HI_U32                      u32Gop;			  /**<GOP size,it is just valid for H264 Encoder. It can be set dynamically.*/ /**<CNcomment: GOP大小,仅在编码H264协议有效，可动态设置 */ 	
    HI_U32                      u32MaxQp;         /**<The maximum quantization parameter,it is just valid for H264 Encoder. It can be set dynamically.*/    /**<CNcomment: 最大量化参数,仅在编码H264协议有效，可动态设置*/	
    HI_U32                      u32MinQp;         /**<The minimum quantization parameter,it is just valid for H264 Encoder. It can be set dynamically.*/    /**<CNcomment: 最小量化参数,仅在编码H264协议有效，可动态设置*/	
    HI_BOOL			            bQuickEncode;     /**<Quick Encode Mode enable*/ /**<CNcomment:是否使能快速编码模式*/
    HI_U8                       u8Priority;       /**<the Priority Level of the channal,should between 0 to the max num of channel minus 1.It can be set dynamically.*/     
                                                  /**<CNcomment: 编码通道的优先级属性，取值范围为0~最大通道数-1,可动态设置*/
    HI_U32                      u32Qlevel;        /**<The value ranges from 1 to 99 for the JPGE, it is just valid for JPEG Encoder.*/ /**<CNcomment: JPGE取值范围1-99，只在定义了JPEG编码器情况下才有效 */
    HI_U32                      u32DriftRateThr;  /**<Encoder bitrate fluctuation threshold.If u32DriftRateThr = 20 means the fluctuation of threshold is 20%*/ 
                                                  /**<CNcomment: 编码器码率波动阈值，如设置成20，表示波动阈值为20%*/ 	

    HI_U32                      u32SplitSize;     /**<Split size. The size is in the unit of minimum coded unit (MCU) for the JPEG format and byte for the MP4 format. The split size can be ignored in H.263 format.*/                                                   
}HI_UNF_VENC_CHN_ATTR_S;

/** @} */  /** <!-- ==== Struct Definition End ==== */


/********************************API declaration********************************/
/** \addtogroup      VENC */
/** @{ */  /** <!-- 【VENC】 */

/** 
\brief Initializes the video encoder. CNcomment: 初始化视频编码器 CNend
\attention \n
Before using the VENC, you must call this API. CNcomment: 调用VENC模块要求首先调用该接口 CNend
\param[in] N/A CNcomment: 无 CNend
\retval ::HI_SUCCESS Success CNcomment: 成功 CNend
\retval ::HI_ERR_VENC_DEV_NOT_EXIST No VENC device exists. CNcomment: 设备不存在 CNend
\retval ::HI_ERR_VENC_NOT_DEV_FILE The file is not a VENC file. CNcomment: 文件非设备 CNend
\retval ::HI_ERR_VENC_DEV_OPEN_ERR The VENC device fails to start. CNcomment: 打开设备失败 CNend
\see \n
N/A CNcomment: 无 CNend
*/
HI_S32 HI_UNF_VENC_Init(HI_VOID);


/** 
\brief Deinitializes the video encoder. CNcomment: 去初始化视频编码器 CNend
\attention \n
N/A CNcomment: 无 CNend
\param[in] N/A CNcomment: 无 CNend
\retval ::HI_SUCCESS Success CNcomment: 成功 CNend
\retval ::HI_ERR_VENC_DEV_CLOSE_ERR The video encoder fails to stop. CNcomment: 关闭编码器失败 CNend
\see \n
N/A CNcomment: 无  CNend
*/
HI_S32 HI_UNF_VENC_DeInit(HI_VOID);


/** 
\brief Obtains the default attributes of a VENC channel. CNcomment: 获取编码通道默认属性 CNend
\attention \n
By default, the encoding size is D1, encoding format is H.264, and a frame is a slice.
CNcomment: 默认D1编码，H.264格式，一帧为一个Slice CNend
\param[out] pstAttr Pointer to the attributes of a VENC channel. CNcomment: pstAttr 指向编码通道属性的指针 CNend
\retval ::HI_SUCCESS Success CNcomment: 成功 CNend
\retval ::HI_ERR_VENC_NULL_PTR The input pointer parameter is null. CNcomment: 输入指针参数为空指针 CNend
\see \n
N/A CNcomment: 无 CNend
*/
HI_S32 HI_UNF_VENC_GetDefaultAttr(HI_UNF_VENC_CHN_ATTR_S *pstAttr);


/** 
\brief Creates a VENC channel. CNcomment: 创建视频编码通道 CNend
\attention \n
You must create a VENC channel before video encoding. 
CNcomment: 进行视频编码要求首先创建编码通道 CNend
\param[in] pstAttr Pointer to the attributes of a VENC channel. CNcomment: pstAttr 指向编码通道属性的指针 CNend
\param[out] phVenc Pointer to the handle of a VENC channel. CNcomment: phVenc 指向编码通道句柄的指针 CNend
\retval ::HI_SUCCESS Success CNcomment: 成功  CNend
\retval ::HI_ERR_VENC_NULL_PTR The input pointer parameter is null. CNcomment: 输入指针参数有空指针 CNend
\retval ::HI_ERR_VENC_CREATE_ERR The number of VENC channels exceeds the limit. CNcomment: 编码通道数已满 CNend
\retval ::HI_ERR_VENC_INVALID_PARA The channel attributes are incorrect. CNcomment: 通道属性设置错误 CNend
\retval ::HI_ERR_VENC_NO_INIT The video encoder is not initialized. CNcomment: 编码器未初始化 CNend
\see \n
N/A
CNcomment: 无  CNend
*/
HI_S32 HI_UNF_VENC_Create(HI_HANDLE *phVenc,const HI_UNF_VENC_CHN_ATTR_S *pstAttr);


/** 
\brief Destroys a VENC channel. CNcomment: 销毁视频编码通道 CNend
\attention \n
\param[in] hVenc Handle of a VENC channel. CNcomment: CNcomment: hVenc 编码通道句柄 CNend
\retval ::HI_SUCCESS Success CNcomment: 成功 CNend
\retval ::HI_ERR_VENC_CHN_NOT_EXIST No handle exists. CNcomment: 句柄不存在 CNend
\retval ::HI_ERR_VENC_NO_INIT The video encoder is not initialized. CNcomment: 编码器未初始化 CNend
\see \n
N/A CNcomment: 无  CNend
*/
HI_S32 HI_UNF_VENC_Destroy(HI_HANDLE hVenc);


/** 
\brief Attaches a VENC channel to the video source. CNcomment: 绑定编码通道到视频源 CNend
\attention \n
You must call this API before performing encoding and obtaining streams. CNcomment: 开始编码和获取码流之前需要首先调用该接口 CNend
\param[in] hVenc Handle of a VENC channel CNcomment: hVenc 编码通道句柄 CNend
\param[in] hSrc Data source handle CNcomment: hSrc 视频源句柄 CNend
\retval ::HI_SUCCESS Success CNcomment: 成功 CNend
\retval ::HI_ERR_VENC_CHN_NOT_EXIST No VENC channel handle exists. CNcomment: 编码通道句柄不存在 CNend
\retval ::HI_ERR_VENC_INVALID_PARA The video source is invalid. CNcomment: 视频源错误  CNend
\retval ::HI_ERR_VENC_NO_INIT The video encoder is not initialized. CNcomment: 编码器未初始化 CNend
\see \n
N/A CNcomment: 无 CNend
*/
HI_S32 HI_UNF_VENC_AttachInput(HI_HANDLE hVenc,HI_HANDLE hSrc);


/** 
\brief Detaches a VENC channel from the video source. CNcomment: 解绑定视频源 CNend
\attention \n
You must stop encoding before calling this API.
CNcomment: 调用该接口需要首先停止编码 CNend
\param[in] hVenc Handle of a VENC channel CNcomment: hVenc 编码通道句柄 CNend
\retval ::HI_SUCCESS Success CNcomment: 成功  CNend
\retval ::HI_ERR_VENC_CHN_NOT_EXIST No VENC channel handle exists. CNcomment: 编码通道句柄不存在 CNend
\retval ::HI_ERR_VENC_INVALID_PARA The video source is invalid. CNcomment: 视频源错误 CNend
\retval ::HI_ERR_VENC_NO_INIT The video encoder is not initialized. CNcomment: 编码器未初始化 CNend
\retval ::HI_ERR_VENC_CHN_INVALID_STAT Invalid state which the video encoder does not stop encoding. CNcomment: 异常状态,编码器未停止编码 CNend
\retval ::HI_ERR_VENC_CHN_NO_ATTACH The VENC channel doesn't attach any source. CNcomment: 编码通道没有绑定任何源 CNend
\see \n
N/A CNcomment: 无  CNend
*/
HI_S32 HI_UNF_VENC_DetachInput(HI_HANDLE hVencChn);


/** 
\brief Starts to perform encoding. CNcomment: 开始编码 CNend
\attention \n
You must initialize the video encoder, create a VENC channel, and attach the channel to the video source before calling this API.
CNcomment: 调用该接口需要首先初始化编码器，创建编码通道，绑定视频源 CNend
\param[in] hVenc Handle of a VENC channel CNcomment: hVenc 编码通道句柄 CNend
\retval ::HI_SUCCESS Success CNcomment: 成功 CNend
\retval ::HI_ERR_VENC_CHN_NOT_EXIST No VENC channel handle exists. CNcomment: 编码通道句柄不存在 CNend
\retval ::HI_ERR_VENC_NO_INIT The video encoder is not initialized. CNcomment: 编码器未初始化 CNend
\see \n
N/A CNcomment: 无 CNend
*/
HI_S32 HI_UNF_VENC_Start(HI_HANDLE hVenc);


/** 
\brief Stops encoding. CNcomment: 停止编码 CNend
\attention \n
You must initialize the video encoder, create a VENC channel, attach the channel to the video source, and start to perform encoding before calling this API.
CNcomment: 调用该接口需要首先初始化编码器，创建编码通道，绑定视频源，已经开始编码 CNend
\param[in] hVenc Handle of a VENC channel CNcomment: hVenc 编码通道句柄 CNend
\retval ::HI_SUCCESS Success CNcomment: 成功 CNend
\retval ::HI_ERR_VENC_CHN_NOT_EXIST No VENC channel handle exists. CNcomment: 编码通道句柄不存在 CNend
\retval ::HI_ERR_VENC_NO_INIT The video encoder is not initialized. CNcomment: 编码器未初始化 CNend
\see \n
N/A CNcomment: 无 CNend
*/
HI_S32 HI_UNF_VENC_Stop(HI_HANDLE hVenc);

/** 
\brief Obtains VENC streams. CNcomment: 获取视频编码码流 CNend
\attention \n
You must attach a VENC channel to the video source and start to perform encoding before calling this API.
The block time (in ms) is configurable. If u32TimeOutMs is set to 0, the block time is 0; if u32TimeOutMs is set to 0xFFFFFFFF, it indicates infinite wait. \n
If the block time is reached but no data is received, ::HI_ERR_VENC_BUF_EMPTY is returned.\n
If the wait time is 0 and there is no data, ::HI_ERR_VENC_BUF_EMPTY is returned.
CNcomment: 调用该接口需要首先绑定视频源，开始编码
阻塞时间可以设置，时间单位为毫秒，设置为0不等待，设置为0xffffffff一直等待。\n
若超过阻塞时间，还没有数据到达，则返回::HI_ERR_VENC_BUF_EMPTY\n
如果等待时间为0，且没有码流数据，则返回::HI_ERR_VENC_BUF_EMPTY\n CNend
\param[in] hVenc Handle of a VENC channel CNcomment: hVenc 编码通道句柄 CNend
\param[out] pstStream Pointer to the structure for storing streams CNcomment: pstStream 存放码流结构的指针 CNend
\param[in] u32TimeoutMs: Wait timeout, count in ms CNcomment: u32TimeoutMs：等待超时时间，单位ms CNend
\retval ::HI_SUCCESS Success CNcomment: 成功 CNend
\retval ::HI_ERR_VENC_CHN_NOT_EXIST No VENC channel handle exists. CNcomment: 编码通道句柄不存在 CNend
\retval ::HI_ERR_VENC_NO_INIT The video encoder is not initialized. CNcomment: 编码器未初始化 CNend
\retval ::HI_ERR_VENC_NULL_PTR The stream pointer is null. CNcomment: 码流指针为空 CNend
\retval ::HI_ERR_VENC_BUF_EMPTY Streams fail to be obtained. CNcomment: 获取码流失败 CNend
\see \n
N/A CNcomment: 无 CNend
*/
HI_S32 HI_UNF_VENC_AcquireStream(HI_HANDLE hVenc,HI_UNF_VENC_STREAM_S *pstStream, HI_U32 u32TimeoutMs);


/** 
\brief Releases VENC streams. CNcomment: 释放视频编码码流 CNend
\attention \n
You must obtain streams and keep the streams unchanged before calling this API.
The stream which you get first should be release first.
CNcomment: 调用该接口需要首先获取码流，且不能改变码流内容。
先获取的码流需要先释放。 CNend
\param[in] hVenc Handle of a VENC channel CNcomment: hVenc 编码通道句柄 CNend
\param[in] pstStream Pointer to the structure for storing streams CNcomment: pstStream 存放码流结构的指针 CNend
\retval ::HI_SUCCESS Success CNcomment: 成功 CNend
\retval ::HI_ERR_VENC_CHN_NOT_EXIST No VENC channel handle exists. CNcomment: 编码通道句柄不存在 CNend
\retval ::HI_ERR_VENC_NO_INIT The video encoder is not initialized. CNcomment: 编码器未初始化 CNend
\retval ::HI_ERR_VENC_NULL_PTR The stream pointer is null. CNcomment: 码流指针为空 CNend
\retval ::HI_ERR_VENC_CHN_RELEASE_ERR Streams fail to be released. CNcomment: 释放码流失败 CNend
\see \n
N/A CNcomment: 无 CNend
*/
HI_S32 HI_UNF_VENC_ReleaseStream(HI_HANDLE hVenc, const HI_UNF_VENC_STREAM_S *pstStream);


/** 
\brief Sets the encoding channel attributes dynamically. CNcomment: 动态设置编码通道属性 CNend
\attention \n
You must create a VENC channel before calling this API.
This API just can change these attributes as follows:input framerate,target output framerate,target output bitrate,gop,
max/min QP,channel priority, whether use QuickEncode mode and even the encode width and height within the limits of the encoder level.
Moreover,even though you can't change the attributes of encoder level,encoder type,Stream buffer size,Rotation angle,Slice split enable and so on,
you should still assign the real value to them at that time.
It's better that you should call HI_UNF_VENC_GetAttr() to obtain the attributes of a VENC channel before calling this API.
CNcomment: 调用该接口需要首先创建编码通道 .该接接口只能动态的设置以下通道属性:输入帧率，目标输出帧率，目标输出码率，GOP值，最大/最小量化参数，
通道优先级， 快速编码使能，甚至在编码能力级范围下动态改变编码宽高。此外，即使该接口不能动态改变编码能力级，编码协议，码流buffer大小，
旋转角度，分割编码使能等属性，在调用该接口时你还是要给这些通道属性赋上当时的实际值。最好在调用该接口之前调用HI_UNF_VENC_GetAttr()接口获取到当时的通道属性 CNend
\param[in] Venc Handle of a VENC channel CNcomment: hVenc 编码通道句柄 CNend
\param[in] pstAttr Pointer to the attributes of a VENC channel CNcomment: pstAttr 存放编码通道属性的指针 CNend
\retval ::HI_SUCCESS Success CNcomment: 成功 CNend
\retval ::HI_ERR_VENC_CHN_NOT_EXIST No VENC channel handle exists. CNcomment: 编码通道句柄不存在 CNend
\retval ::HI_ERR_VENC_INVALID_PARA The channel attributes are incorrect. CNcomment: 通道属性设置错误 CNend
\retval ::HI_ERR_VENC_NO_INIT The video encoder is not initialized. CNcomment: 编码器未初始化 CNend
\retval ::HI_ERR_VENC_NULL_PTR The pointer is null. CNcomment: 指针为空 CNend
\retval ::HI_ERR_VENC_NOT_SUPPORT This change of attributes dosen't be supported. CNcomment: 不支持该属性动态配置 CNend
\see \n
N/A CNcomment: 无 CNend
*/
HI_S32 HI_UNF_VENC_SetAttr(HI_HANDLE hVenc,const HI_UNF_VENC_CHN_ATTR_S *pstAttr);


/** 
\brief Obtains the attributes of a VENC channel. CNcomment: 获取编码通道属性 CNend
\attention \n
You must create a VENC channel before calling this API.
CNcomment: 调用该接口需要首先创建编码通道 CNend
\param[in] hVenc Handle of a VENC channel CNcomment: hVenc 编码通道句柄 CNend
\param[out] pstAttr Encoding channel attribute CNcomment: pstAttr 编码通道属性 CNend
\retval ::HI_SUCCESS Success CNcomment: 成功 CNend
\retval ::HI_ERR_VENC_CHN_NOT_EXIST No VENC channel handle exists. CNcomment: 编码通道句柄不存在 CNend
\retval ::HI_ERR_VENC_NO_INIT The video encoder is not initialized. CNcomment: 编码器未初始化 CNend
\retval ::HI_ERR_VENC_NULL_PTR The pointer is null. CNcomment: 指针为空 CNend
\see \n 
N/A CNcomment: 无 CNend
*/
HI_S32 HI_UNF_VENC_GetAttr(HI_HANDLE hVenc, HI_UNF_VENC_CHN_ATTR_S *pstAttr);


/** 
\brief Applies for I frames. CNcomment: 请求I帧 CNend
\attention \n
The video encoder encodes an I frame as soon as possible after you call this API.\n
You can call this API when you set up a video call or fix errors.\n
If you call this API repeatedly in a short period, I frames may be not generated each time.\n
CNcomment: 调用该接口后，编码器会尽快编码出一个I帧来.\n
此接口一般用于可视电话场景的通话建立和错误恢复.\n
此接口只是"尽最大能力"尽快编码出一个I帧来，如果在短时间内多次调用此接口，那么不能保证每次调用都能对应编码出一个I帧来.\n CNend
\param[in] hVencChn Handle of a VENC channel CNcomment: hVencChn 编码通道句柄 CNend
\retval ::HI_SUCCESS Success CNcomment: 成功 CNend
\retval ::HI_ERR_VENC_CHN_NOT_EXIST No VENC channel handle exists. CNcomment: 编码通道句柄不存在 CNend
\retval ::HI_ERR_VENC_NO_INIT The video encoder is not initialized. CNcomment: 编码器未初始化 CNend
\see \n
N/A CNcomment: 无 CNend
*/
HI_S32 HI_UNF_VENC_RequestIFrame(HI_HANDLE hVencChn);


/** 
\brief Input frame to VENC. CNcomment: 向编码器送帧 CNend
\attention \n
You must create a VENC channel before calling this API.
If the venc channel has already attach the source,::HI_ERR_VENC_CHN_INVALID_STAT is returned.
This API should be matched with the API: HI_UNF_VENC_DequeueFrame.
HI_UNF_VENC_QueueFrame is used to send frame data to Encoder for user and HI_UNF_VENC_DequeueFrame is used to call the Encoder 
to receive the frame data which has already be processed.
CNcomment: 调用该接口需要首先创建编码通道.
如果编码通道已经绑定源，会返回HI_ERR_VENC_CHN_INVALID_STAT 无效操作 
该接口需要与 HI_UNF_VENC_DequeueFrame 接口配对使用。
HI_UNF_VENC_QueueFrame接口用于为用户提供一个接口向编码器送帧供编码，HI_UNF_VENC_DequeueFrame接口是编码器归还已经处理的帧 CNend
\param[in] hVenc Handle of a VENC channel CNcomment: hVenc 编码通道句柄 CNend
\param[in] pstFrameinfo Frame information struct CNcomment: 帧信息属性 CNend
\retval ::HI_SUCCESS                      Success CNcomment: 成功 CNend
\retval ::HI_ERR_VENC_CHN_NOT_EXIST       No VENC channel handle exists. CNcomment: 编码通道句柄不存在 CNend
\retval ::HI_ERR_VENC_NULL_PTR            The pointer is null. CNcomment: 指针为空 CNend
\retval ::HI_ERR_VENC_NO_INIT             The video encoder is not initialized. CNcomment: 编码器未初始化 CNend
\retval ::HI_ERR_VENC_CHN_INVALID_STAT    Invalid state which the VENC channel has already attach source. CNcomment: 异常状态，编码器通道已经绑定源 CNend
\see \n
N/A CNcomment: 无 CNend
*/
HI_S32 HI_UNF_VENC_QueueFrame(HI_HANDLE hVenc, HI_UNF_VIDEO_FRAME_INFO_S *pstFrameinfo);


/** 
\brief release frame from VENC. CNcomment: 向编码器还帧 CNend
\attention \n
You must create a VENC channel before calling this API.
If the venc channel has already attach the source,::HI_ERR_VENC_CHN_INVALID_STAT is returned.
Refer to the attention of the API: HI_UNF_VENC_QueueFrame.
If HI_UNF_VENC_DequeueFrame return HI_SUCCESS, it means the corresponding frame buffer could be set free by user. 
CNcomment: 调用该接口需要首先创建编码通道.
如果编码通道已经绑定源，会返回HI_ERR_VENC_CHN_INVALID_STAT 无效操作 
注意事项参照 HI_UNF_VENC_QueueFrame接口。如果HI_UNF_VENC_DequeueFrame接口返回成功，意味着对应的帧存用户可以释放 CNend
\param[in] hVenc Handle of a VENC channel CNcomment: hVenc 编码通道句柄 CNend
\param[out] pstFrameinfo Frame information struct CNcomment: 帧信息属性 CNend
\retval ::HI_SUCCESS                       Success CNcomment: 成功 CNend
\retval ::HI_ERR_VENC_CHN_NOT_EXIST        No VENC channel handle exists. CNcomment: 编码通道句柄不存在 CNend
\retval ::HI_ERR_VENC_NULL_PTR             The pointer is null. CNcomment: 指针为空 CNend
\retval ::HI_ERR_VENC_NO_INIT              The video encoder is not initialized. CNcomment: 编码器未初始化 CNend
\retval ::HI_ERR_VENC_CHN_INVALID_STAT     Invalid state which the VENC channel has already attach source. CNcomment: 异常状态，编码器通道已经绑定源 CNend
\see \n
N/A CNcomment: 无 CNend
*/
HI_S32 HI_UNF_VENC_DequeueFrame(HI_HANDLE hVenc, HI_UNF_VIDEO_FRAME_INFO_S *pstFrameinfo);

/** @} */  /** <!-- ==== API Declaration End ==== */
#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif

#endif//__HI_UNF_VENC_H__

