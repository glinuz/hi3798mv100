/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_unf_vp.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2011/03/25
  Description   :
  History       :
  1.Date        : 2011/03/25
    Author      :
    Modification: Created file

 *******************************************************************************/

/**
 * \file
 * \brief Describes the information about the video phone (VP) engine. CNcomment: 提供VP视频引擎的相关信息CNend
 */
#ifndef __HI_UNF_VP_H__
#define __HI_UNF_VP_H__
#include <netinet/in.h>
#include "hi_common.h"
#include "hi_unf_common.h"
#include "hi_unf_venc.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif

/*************************** Structure Definition ****************************/
/** \addtogroup      VP*/
/** @{ */  /** <!-- [VP] */

/** Video source mode */
/**CNcomment: 视频源模式*/
typedef enum hiUNF_VP_SOURCE_MODE_E
{
    HI_UNF_VP_SRC_MODE_USBCAM = 0,   /**<USB camera source mode *//**<CNcomment: USB 摄像头源模式*/
    HI_UNF_VP_SRC_MODE_VI,           /**<VI camera source mode *//**<CNcomment: VI源模式*/
    HI_UNF_VP_SRC_MODE_BUTT,
} HI_UNF_VP_SOURCE_MODE_E;

/**Video source attribute*/
/**CNcomment: 视频源属性 */
typedef struct hiUNF_VP_SOURCE_ATTR_S
{
    HI_HANDLE               hSrc;        /**<video source handle, can be set to VI handle or USB camera handle in v4l2*//**<CNcomment: 视频输入句柄，VI句柄或者v4l2中USB摄像头的句柄*/
    HI_UNF_VP_SOURCE_MODE_E enSrcMode;   /**<Video source mode *//**<CNcomment: 视频源模式*/
    HI_U32                  u32VIBufNum; /**<VI buffer number*//**<CNcomment:VI缓冲区个数*/
} HI_UNF_VP_SOURCE_ATTR_S;

/**Network attribute*/
/**CNcomment: 网络属性 */
typedef union hiUNF_VP_NET_ATTR_U
{
    struct sockaddr_in  stIpv4Attr;      /**<IPV4 attribute*//**<CNcomment:IPV4属性*/
    struct sockaddr_in6 stIpv6Attr;      /**<IPV6 attribute*//**<CNcomment:IPV6属性*/
} HI_UNF_VP_NET_ATTR_U;

/**RTP attribute*/
/**CNcomment: RTP属性*/
typedef struct hiUNF_VP_RTP_ATTR_S
{
    HI_U32 u32SplitSize;                       /**< split size, should be less than MTU size*//**<CNcomment:分片大小*/
    HI_U32 u32PayLoadType;                     /**< pay load type *//**<CNcomment:负载类型*/
    HI_U32 u32SortDepth;                       /**< sort in order when receive RTP packets, this para indicates sort depth *//**<CNcomment:排序深度*/
    HI_U32 u32RecvBufSize;                     /**< buffer size, usually 1500 bytes , should be bigger than u32SplitSize*//**<CNcomment:接收buf，应该大于分片大小*/
    HI_U32 u32RecvBufNum;                      /**< buffer number (total_size = u32BufSize * u32BufNum) *//**<CNcomment:接收buf个数*/
} HI_UNF_VP_RTP_ATTR_S;

/**Video engine attribute*/
/**CNcomment: 视频引擎属性 */
typedef struct hiUNF_VP_ATTR_S
{
    HI_UNF_VENC_CHN_ATTR_S stVencAttr;        /**<VENC attribute*//**<CNcomment: 视频编码器属性*/
    HI_UNF_VCODEC_TYPE_E   enDecType;         /**<VDEC attribute*//**<CNcomment: 视频解码器属性*/
    HI_BOOL                bIsIpv4;           /**<whether use IPV4 attribute, use 1 to demonstrate IPV4, use 0 to demonstrate IPV6 *//**<CNcomment: 是否使用IPV4参数，1表示使用IPV4，0表示使用IPV6*/
    HI_UNF_VP_NET_ATTR_U   stLocalNetAttr;    /**<Local network attribute*//**<CNcomment: 本地网络属性*/
    HI_UNF_VP_NET_ATTR_U   stRemoteNetAttr;   /**<Remote network attribute*//**<CNcomment: 远端网络属性*/
    HI_UNF_VP_RTP_ATTR_S   stRtpAttr;         /**<RTP attribute. Generally, We highly recommend that you don't modify it.*//**<CNcomment: RTP属性，一般不建议修改*/
} HI_UNF_VP_ATTR_S;

/**Remote mode*/
/**CNcomment: 远端模式*/
typedef enum hiUNF_VP_REMOTE_MODE_E
{
    HI_UNF_VP_RMT_MODE_SEND = 0x1,         /**<Send local video to remote mode*//**<CNcomment: 发送本地视频到远端模式*/
    HI_UNF_VP_RMT_MODE_RECV = 0x2,         /**<Receive remote video mode*//**<CNcomment: 接收远端视频模式*/
    HI_UNF_VP_RMT_MODE_BUTT = 0x4,
} HI_UNF_VP_REMOTE_MODE_E;

/**RTCP configure of video engine*/
/**CNcomment: 视频引擎RTCP配置信息 */
typedef struct hiUNF_VP_RTCP_CFG_S
{
    HI_U32 u32EnableFlag;     /**<Enable RTCP function: 0--disable, 1--enable*//**<CNcomment: RTCP统计使能: 0--不使能, 1--使能*/
    HI_U32 u32SendInterval;   /**<Send interval of RTCP report, [500, 65535]ms, must be integer of 500ms*//**<CNcomment: RTCP报告发送间隔, [500, 65535] ms , 必须为500ms的整数倍*/
} HI_UNF_VP_RTCP_CFG_S;

/**RTCP statistics information*/
/**CNcomment: RTCP 统计信息*/
typedef struct hiUNF_VP_RTCP_STATIC_INFO_S
{
    HI_U32 u32TotalLostPacket;     /**<Total lost packet, this value may reset to 0 when network connection become abnormal *//**<CNcomment: 数据丢包统计，网络连接异常时，改数据可能清零*/
    HI_U32 u32LossFraction;        /**<Lost rate(%)*//**<CNcomment: 丢包率(%) */
    HI_U32 u32Jitter;              /**<Jitter(ms)*//**<CNcomment: 平均抖动(ms) */
    HI_U32 u32Delay;               /**<Delay(ms)*//**<CNcomment: 平均时延(ms) */
} HI_UNF_VP_RTCP_STATIC_S;

/**Statistic information of network*/
/**CNcomment: 视频引擎网络统计信息 */
typedef struct hiUNF_VP_NET_STA_S
{
    HI_U32                  u32SendPacketNum; /**<Total send packet number*//**<CNcomment: 发送包总数*/
    HI_U32                  u32SendBytes;     /**<Total send bytes*//**<CNcomment: 发送字节数*/
    HI_U32                  u32RecvPacketNum; /**<Total recv packet number*//**<CNcomment: 接收包总数*/
    HI_U32                  u32RecvBytes;      /**<Total recv bytes*//**<CNcomment: 接收字节数*/
    HI_UNF_VP_RTCP_STATIC_S stSendInfo;       /**<Send information*//**<CNcomment: 发送统计*/
    HI_UNF_VP_RTCP_STATIC_S stRecvInfo;       /**<Reveive information*//**<CNcomment: 接收统计*/
} HI_UNF_VP_NET_STA_S;

/** @} */  /** <!-- ==== Structure Definition end ==== */

/******************************* API declaration *****************************/
/** \addtogroup      VP */
/** @{ */  /** <!-- 【VP】 */

/**
 \brief Initializes the VP module. CNcomment: 初始化VP模块CNend
 \attention \n
Call this application programming interface (API) before creating VP by calling ::HI_UNF_VP_Create.
CNcomment: 在调用::HI_UNF_VP_Create接口创建引擎之前，调用本接口CNend
 \param N/A CNcomment: 无CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功CNend
 \retval ::HI_ERR_VP_DEV_NOT_EXIST The VP device does not exist. CNcomment: 设备不存在CNend
 \retval ::HI_ERR_VP_DEV_OPEN_ERR The VP device fails to be started. CNcomment: 设备打开错误CNend
 \see \n
N/A CNcomment: 无CNend
 */
HI_S32 HI_UNF_VP_Init(HI_VOID);

/**
 \brief Deinitialize the VP module. CNcomment: 去初始化VP模块CNend
 \attention \n
Call this API after destroying all VPs by calling ::HI_UNF_VP_DestroyChannel.
CNcomment: 在调用::HI_UNF_VP_Destroy接口销毁VP后，调用本接口CNend
 \param N/A CNcomment: 无CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功CNend
 \retval ::HI_ERR_VP_NOT_INIT The VP is not initialized. CNcomment: VP未初始化CNend
 \see \n
N/A CNcomment: 无CNend
 */
HI_S32 HI_UNF_VP_DeInit(HI_VOID);

/**
 \brief Obtain the default parameter values of VP. CNcomment: 获取VP缺省参数CNend
 \attention \n
Call this API before creating VP by calling ::HI_UNF_VP_Create. This enables you to set parameters conveniently.
CNcomment: 在调用::HI_UNF_VP_Create接口创建VP之前，调用本接口，方便填写参数CNend
 \param[in] N/A CNcomment: 无CNend
 \param[out] pstDefAttr Pointer to the default VP attributes. For details, see :HI_UNF_VP_ATTR_S. CNcomment: 指针类型，VP默认属性，参见:HI_UNF_VP_ATTR_S CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功CNend
 \retval ::HI_ERR_VP_NULL_PTR The pointer is null. CNcomment: 空指针CNend
 \see \n
N/A CNcomment: 无CNend
 */

HI_S32 HI_UNF_VP_GetDefaultAttr(HI_UNF_VP_ATTR_S *pstDefAttr);

/**
 \brief Create VP. CNcomment: 创建VP  CNend
 \attention \n
N/A CNcomment: 无CNend
 \param[in]  pstAttr parameter CNcomment: VP参数CNend
 \param[out] phVP Pointer to the VP handle. CNcomment: 指针类型，VP句柄CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功CNend
 \retval ::HI_ERR_VP_NULL_PTR The pointer is null. CNcomment: 空指针CNend
 \retval ::HI_ERR_VP_NOT_SUPPORT The input parameter is invalid. CNcomment: 输入参数非法CNend
 \retval ::HI_ERR_VP_NOT_SUPPORT The mode is not supported. CNcomment: 不支持模式CNend
 \see \n
N/A CNcomment: 无CNend
 */
HI_S32 HI_UNF_VP_Create(const HI_UNF_VP_ATTR_S *pstAttr, HI_HANDLE *phVP);

/**
 \brief Destroy VP. CNcomment: 销毁VP  CNend
 \attention \n
N/A CNcomment: 无CNend
 \param[in] hVP VP handle CNcomment: VP句柄CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功CNend
 \retval ::HI_ERR_VP_NOT_INIT The VP module is not initialized. CNcomment: VP未初始化CNend
 \retval ::HI_ERR_VP_INVALID_HANDLE The VP handle is invalid. CNcomment: 非法的VP句柄CNend
 \retval Other values: A fails to be destroyed. CNcomment: 其它 销毁失败CNend
 \see \n
N/A CNcomment: 无CNend
 */
HI_S32 HI_UNF_VP_Destroy(HI_HANDLE hVP);

/**
 \brief Attach input source of VP. CNcomment: 绑定视频输入源CNend
 \attention \n
 \param[in] hVP VP handle CNcomment: VP 句柄CNend
 \param[in] stSrcAttr Video input attribute. CNcomment:视频输入通道属性CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功CNend
 \retval ::HI_ERR_VP_INVALID_PARA The input parameter is invalid. CNcomment: 无效的输入参数CNend
 \retval Other values: Failed to attach input source of VP. CNcomment: 绑定视频输入源失败CNend
 \see \n
N/A CNcomment: 无CNend
 */
HI_S32 HI_UNF_VP_AttachSource(HI_HANDLE hVP, HI_UNF_VP_SOURCE_ATTR_S *pstSrcAttr);

/**
 \brief Attach local window of VP. CNcomment: 绑定本地视频窗口CNend
 \attention \n
 \param[in] hVP VP handle CNcomment: VP 句柄CNend
 \param[in] hLocalWin Local window handle. CNcomment:本地视频窗口句柄CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功CNend
 \retval ::HI_ERR_VP_INVALID_PARA The input parameter is invalid. CNcomment: 无效的输入参数CNend
 \retval Other values: Failed to attach local window of VP. CNcomment: 绑定本地视频窗口失败CNend
 \see \n
N/A CNcomment: 无CNend
 */
HI_S32 HI_UNF_VP_AttachLocalWin(HI_HANDLE hVP, HI_HANDLE hLocalWin);

/**
 \brief Attach remote window of VP. CNcomment: 绑定远端视频窗口CNend
 \attention \n
 \param[in] hVP VP handle CNcomment: VP 句柄CNend
 \param[in] hRemoteWin Remote window handle. CNcomment:远端视频窗口句柄CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功CNend
 \retval ::HI_ERR_VP_INVALID_PARA The input parameter is invalid. CNcomment: 无效的输入参数CNend
 \retval Other values: Failed to attach remote window of VP. CNcomment: 绑定远端视频窗口失败CNend
 \see \n
N/A CNcomment: 无CNend
 */
HI_S32 HI_UNF_VP_AttachRemoteWin(HI_HANDLE hVP, HI_HANDLE hRemoteWin);

/**
 \brief Detach input source of VP. CNcomment: 去绑定视频输入源CNend
 \attention \n
 \param[in] hVP VP handle CNcomment: VP 句柄CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功CNend
 \retval ::HI_ERR_VP_INVALID_PARA The input parameter is invalid. CNcomment: 无效的输入参数CNend
 \retval Other values: Failed to detach input source of VP. CNcomment: 去绑定视频输入源失败CNend
 \see \n
N/A CNcomment: 无CNend
 */
HI_S32 HI_UNF_VP_DetachSource(HI_HANDLE hVP);

/**
 \brief Detach local window of VP. CNcomment: 去绑定本地视频窗口CNend
 \attention \n
 \param[in] hVP VP handle CNcomment: VP 句柄CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功CNend
 \retval ::HI_ERR_VP_INVALID_PARA The input parameter is invalid. CNcomment: 无效的输入参数CNend
 \retval Other values: Failed to detach local window of VP. CNcomment: 去绑定本地视频窗口失败CNend
 \see \n
N/A CNcomment: 无CNend
 */
HI_S32 HI_UNF_VP_DetachLocalWin(HI_HANDLE hVP);

/**
 \brief Detach remote window of VP. CNcomment: 去绑定远端视频窗口CNend
 \attention \n
 \param[in] hVP VP handle CNcomment: VP句柄CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功CNend
 \retval ::HI_ERR_VP_INVALID_PARA The input parameter is invalid. CNcomment: 无效的输入参数CNend
 \retval Other values: Failed to detach remote window of VP. CNcomment: 去绑定远端视频窗口失败CNend
 \see \n
N/A CNcomment: 无CNend
 */
HI_S32 HI_UNF_VP_DetachRemoteWin(HI_HANDLE hVP);

/**
 \brief Obtains the configuration of VP. CNcomment: 获取VP 配置CNend
 \attention \n
 \param[in] hVP VP handle CNcomment: VP 句柄CNend
 \param[out] pstAttr Pointer to VP attributes. For details, see :HI_UNF_VP_ATTR_S. CNcomment: 指针类型，VP 属性，参见:HI_UNF_VP_ATTR_S  CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功CNend
 \retval ::HI_ERR_VP_INVALID_PARA The input parameter is invalid. CNcomment: 无效的输入参数CNend
 \retval ::HI_ERR_VP_NULL_PTR The pointer is null. CNcomment: 空指针CNend
 \retval Other values:Fail to obtain attributes. CNcomment: 获取通道属性失败CNend
 \see \n
N/A CNcomment: 无CNend
 */

HI_S32 HI_UNF_VP_GetAttr(HI_HANDLE hVP, HI_UNF_VP_ATTR_S *pstAttr);

/**
 \brief Modifies the configuration of VP. CNcomment: 修改VP配置CNend
 \attention \n
You are advised to obtain the current configuration by calling HI_UNF_VP_GetChAttr(), and modify parameter values as required.
CNcomment: 建议先调用HI_UNF_VP_GetChAttr()获取当前配置，再对参数做必要修改CNend
 \param[in] hVP VP handle CNcomment: VP 句柄CNend
 \param[in] pstAttr Pointer to VP attributes. For details, see :HI_UNF_VP_ATTR_S. CNcomment: 指针类型，VP 属性，参见:HI_UNF_VP_ATTR_S  CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功CNend
 \retval ::HI_ERR_VP_INVALID_PARA The input parameter is invalid. CNcomment: 无效的输入参数CNend
 \retval ::HI_ERR_VP_NULL_PTR The pointer is null. CNcomment: 空指针CNend
 \retval Other values: Fail to modify attributes . CNcomment: 修改属性失败CNend
 \see \n
N/A CNcomment:无CNend
 */
HI_S32 HI_UNF_VP_SetAttr(HI_HANDLE hVP, const HI_UNF_VP_ATTR_S *pstAttr);

/**
 \brief Starts VP. CNcomment: VP开始运行CNend
 \attention \n
After setting parameters including the codec, video input (VI), and video output (VO) parameters, you can start the video engine by calling this API to transmit or receive data over the Internet and display the local or remote video output.
CNcomment: 在参数准备好后，包括编解码器、VI/VO设置正确后，调用此接口可以启动视频引擎的运行，实现网络收发、显示本地、远端视频输出CNend
 \param[in] hVP VP handle CNcomment: VP 句柄CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功CNend
 \retval ::HI_ERR_VP_INVALID_PARA The input parameter is invalid. CNcomment: 无效的输入参数CNend
 \retval ::HI_ERR_VP_NULL_PTR The pointer is null. CNcomment: 空指针CNend
 \retval Other values: VP fails to be started. CNcomment:  VP开始运行失败CNend
 \see \n
N/A CNcomment: 无CNend
 */
HI_S32 HI_UNF_VP_Start(HI_HANDLE hVP);

/**
 \brief Stops VP. CNcomment: VP停止运行CNend
 \attention \n
After VP is started successfully, you can stop it by calling this API to freeze the screen or stop transmitting or receiving data over Internet. You can resume the VP by calling HI_UNF_VP_Start.
CNcomment: 在VP正常启动运行后，可以通过此接口暂停运行，使画面冻结、网络收发暂停。可以调用HI_UNF_VP_Start恢复运行CNend
 \param[in] hVP VP handle CNcomment: VP 句柄CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功CNend
 \retval ::HI_ERR_VP_INVALID_PARA The input parameter is invalid. CNcomment: 无效的输入参数CNend
 \retval ::HI_ERR_VP_NULL_PTR The pointer is null. CNcomment: 空指针CNend
 \retval Other values: VP fails to be started. CNcomment: VP开始运行失败CNend
 \see \n
N/A CNcomment: 无CNend
 */
HI_S32 HI_UNF_VP_Stop(HI_HANDLE hVP);

/**
 \brief Previews the local video over VP. CNcomment: VP开始预览本地视频CNend
 \attention \n
After the VI and hLocalWin parameters are set, you can preview the local video by calling this API.
CNcomment: 在VI和hLocalWin准备好之后，开始预览本地视频CNend
 \param[in] hVP VP handle CNcomment: VP 句柄CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功CNend
 \retval ::HI_ERR_VP_INVALID_PARA The input parameter is invalid. CNcomment: 无效的输入参数CNend
 \retval ::HI_ERR_VP_NULL_PTR The pointer is null. CNcomment: 空指针CNend
 \retval Other values: VP fails to be started. CNcomment: VP开始运行失败CNend
 \see \n
N/A CNcomment: 无CNend
 */
HI_S32 HI_UNF_VP_StartPreView(HI_HANDLE hVP);

/**
 \brief Stops previewing the local video over VP. CNcomment: VP预览本地视频CNend
 \attention \n
When a local video is previewed over VP, you can stop previewing the video by calling this API.
CNcomment: 在VP预览本地视频后，调用停止预览CNend
 \param[in] hVP VP handle CNcomment: VP 句柄CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功CNend
 \retval ::HI_ERR_VP_INVALID_PARA The input parameter is invalid. CNcomment: 无效的输入参数CNend
 \retval ::HI_ERR_VP_NULL_PTR The pointer is null. CNcomment: 空指针CNend
 \retval Other values: VP fails to be started. CNcomment: 其它  VP开始运行失败CNend
 \see \n
N/A CNcomment: 无CNend
 */
HI_S32 HI_UNF_VP_StopPreView(HI_HANDLE hVP);

/**
 \brief Starts remote VP. CNcomment: VP启动远端  CNend
 \attention \n
Starts remote VP, this interface enables vp start sending local video to remote, or receiving remote video.
CNcomment: 启动远端，可以单独启动发送本地视频到远端，或接收远端视频  CNend
 \param[in] hVP VP handle CNcomment: VP 句柄  CNend
 \param[in] enRmtMode remote mode CNcomment: 远端模式  CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功  CNend
 \retval ::HI_ERR_VP_INVALID_PARA The input parameter is invalid. CNcomment: 无效的输入参数  CNend
 \retval ::HI_ERR_VP_NULL_PTR The pointer is null. CNcomment: 空指针  CNend
 \retval Other values: VP fails to be started. CNcomment: VP运行失败  CNend
 \see \n
N/A CNcomment: 无  CNend
 */
HI_S32 HI_UNF_VP_StartRemote(HI_HANDLE hVP, HI_UNF_VP_REMOTE_MODE_E enRmtMode);

/**
 \brief Stops remote VP. CNcomment: VP停止远端  CNend
 \attention \n
Stops remote VP, this interface enables vp stop sending local video to remote, or receiving remote video.
CNcomment: 停止远端，可以单独停止发送本地视频到远端，或停止接收远端视频  CNend
 \param[in] hVP VP handle CNcomment: VP 句柄  CNend
 \param[in] enRmtMode remote mode CNcomment: 远端模式  CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功  CNend
 \retval ::HI_ERR_VP_INVALID_PARA The input parameter is invalid. CNcomment: 无效的输入参数  CNend
 \retval ::HI_ERR_VP_NULL_PTR The pointer is null. CNcomment: 空指针  CNend
 \retval Other values: VP fails to be started. CNcomment: VP停止失败  CNend
 \see \n
N/A CNcomment: 无  CNend
 */
HI_S32 HI_UNF_VP_StopRemote(HI_HANDLE hVP, HI_UNF_VP_REMOTE_MODE_E enRmtMode);

/**
 \brief Configs RTCP over VP. CNcomment: VP配置RTCP  CNend
 \attention \n
When VP has been created, you can config RTCP to enable and config RTCP sending frequency by calling this API.
CNcomment: 在创建VP后，调用该接口配置RTCP，可以使能以及调节RTCP发包频率CNend
 \param[in] hVP VP handle CNcomment: VP句柄CNend
 \param[out] pstRtcpCfg Pointer to config RTCP information, refer to HI_UNF_VP_RTCP_CFG_S. CNcomment: 指针类型，VP RTCP配置信息，参见:HI_UNF_VP_RTCP_CFG_S  CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功CNend
 \retval ::HI_ERR_VP_INVALID_PARA The input parameter is invalid. CNcomment: 无效的输入参数CNend
 \retval ::HI_ERR_VP_NULL_PTR The pointer is null. CNcomment: 空指针CNend
 \retval Other value CNcomment: 其它CNend
 \see \n
N/A CNcomment: 无CNend
 */
HI_S32 HI_UNF_VP_ConfigRtcp(HI_HANDLE hVP, HI_UNF_VP_RTCP_CFG_S *pstRtcpCfg);

/**
 \brief Gets network statistics information over VP. CNcomment: VP获取网络统计信息CNend
 \attention \n
When VP has been started, you can get network statistics information to config parameter of video encoder by calling this API.
CNcomment: 在运行VP后，调用该接口获取网络统计信息以调整编码器参数CNend
 \param[in] hVP VP handle CNcomment: VP句柄CNend
 \param[out] pstNetStatics Pointer to network statistics information of VP, refer to HI_UNF_VP_NET_STA_S. CNcomment: 指针类型，VP网络统计信息，参见:HI_UNF_VP_NET_STA_S  CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功CNend
 \retval ::HI_ERR_VP_INVALID_PARA The input parameter is invalid. CNcomment: 无效的输入参数CNend
 \retval ::HI_ERR_VP_NULL_PTR The pointer is null. CNcomment: 空指针CNend
 \retval Other value CNcomment: 其它CNend
 \see \n
N/A CNcomment: 无CNend
 */
HI_S32 HI_UNF_VP_GetNetStatics(HI_HANDLE hVP, HI_UNF_VP_NET_STA_S *pstNetStatics);

/**
 \brief Gets VENC handle. CNcomment: 获取VENC句柄  CNend
 \attention \n
 \param[in] hVP VP handle CNcomment: VP句柄  CNend
 \param[out] phVenc Pointer to VENC handle. CNcomment: 指针类型，VENC句柄  CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功  CNend
 \retval ::HI_ERR_VP_INVALID_PARA The input parameter is invalid. CNcomment: 无效的输入参数  CNend
 \retval ::HI_ERR_VP_NULL_PTR The pointer is null. CNcomment: 空指针  CNend
 \retval Other value CNcomment: 其它  CNend
 \see \n
N/A CNcomment: 无  CNend
 */
HI_S32 HI_UNF_VP_GetVencHandle(HI_HANDLE hVp, HI_HANDLE *phVenc);

/**
 \brief Gets AVPLAY handle that play remote video. CNcomment: 获取播放远端视频的AVPLAY句柄  CNend
 \attention \n
 \param[in] hVP VP handle CNcomment: VP句柄  CNend
 \param[out] phAvplay Pointer to AVPLAY handle. CNcomment: 指针类型，AVPLAY句柄  CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功  CNend
 \retval ::HI_ERR_VP_INVALID_PARA The input parameter is invalid. CNcomment: 无效的输入参数  CNend
 \retval ::HI_ERR_VP_NULL_PTR The pointer is null. CNcomment: 空指针  CNend
 \retval Other value CNcomment: 其它  CNend
 \see \n
N/A CNcomment: 无  CNend
 */
HI_S32 HI_UNF_VP_GetAvplayHandle(HI_HANDLE hVp, HI_HANDLE *phAvplay);

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif

#endif
