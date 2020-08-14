/******************************************************************************

  Copyright (C), 2001-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_unf_vi.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/03/26
  Description   :
  History       :
  1.Date        : 2010/03/26
    Author      : j00131665
    Modification: Created file

 *******************************************************************************/

/**
 * \file
 * \brief Describes the information about video input (VI). CNcomment: 提供VI的相关信息 CNend
 */

#ifndef  __HI_UNF_VI_H__
#define  __HI_UNF_VI_H__

#include "hi_type.h"
#include "hi_unf_common.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif

/********************************Macro Definition********************************/
/** \addtogroup      VI */
/** @{ */  /** <!-- 【VI】 */
/**Maximum external buffer number*/
/**CNcomment: VI管理外部缓冲区最大个数*/
#define MAX_VI_EXT_BUF_NUM (16)

/** @} */  /** <!-- ==== Macro Definition end ==== */

/********************************Struct Definition********************************/
/** \addtogroup      VI */
/** @{ */  /** <!-- 【VI】 */

/**Input port */
/**CNcomment: 输入端口 */
typedef enum hiUNF_VI_E
{
    HI_UNF_VI_PORT0 = 0,      /**<Port 0 *//**<CNcomment: 端口0*/
    HI_UNF_VI_PORT1,          /**<Port 1 *//**<CNcomment: 端口1*/
    HI_UNF_VI_BUTT
} HI_UNF_VI_E;

/**Input interface mode */
/**CNcomment: 输入接口模式 */
typedef enum hiUNF_VI_INPUT_MODE_E
{
    HI_UNF_VI_MODE_BT656_576I = 0,  /**<ITU-R BT.656 YUV4:2:2 standard, 576I *//**<CNcomment: ITU-R BT.656 YUV4:2:2 标准，576I */
    HI_UNF_VI_MODE_BT656_480I,      /**<ITU-R BT.656 YUV4:2:2 standard, 480I *//**<CNcomment: ITU-R BT.656 YUV4:2:2 标准，480I */
    HI_UNF_VI_MODE_BT601_576I,      /**<ITU-R BT.601 YUV4:2:2 standard, 576I *//**<CNcomment: ITU-R BT.601 YUV4:2:2 标准 */
    HI_UNF_VI_MODE_BT601_480I,      /**<ITU-R BT.601 YUV4:2:2 standard, 480I *//**<CNcomment: ITU-R BT.601 YUV4:2:2 标准 */
    HI_UNF_VI_MODE_BT1120_640X480P, /**<ITU-R BT.1120 YUV4:2:2 standard, 480P *//**<CNcomment: ITU-R BT.1120 YUV4:2:2 标准，640x480P */
    HI_UNF_VI_MODE_BT1120_480P,     /**<ITU-R BT.1120 YUV4:2:2 standard, 480P *//**<CNcomment: ITU-R BT.1120 YUV4:2:2 标准，720x480P */
    HI_UNF_VI_MODE_BT1120_576P,     /**<ITU-R BT.1120 YUV4:2:2 standard, 576P *//**<CNcomment: ITU-R BT.1120 YUV4:2:2 标准，720x576P */
    HI_UNF_VI_MODE_BT1120_720P_50,  /**<ITU-R BT.1120 YUV4:2:2 standard, 720P_50 *//**<CNcomment: ITU-R BT.1120 YUV4:2:2 标准，720P_50 */
    HI_UNF_VI_MODE_BT1120_720P_60,  /**<ITU-R BT.1120 YUV4:2:2 standard, 720P_60 *//**<CNcomment: ITU-R BT.1120 YUV4:2:2 标准，720P_60 */
    HI_UNF_VI_MODE_BT1120_1080I_50, /**<ITU-R BT.1120 YUV4:2:2 standard, 1080I_50 *//**<CNcomment: ITU-R BT.1120 YUV4:2:2 标准，1080I_50 */
    HI_UNF_VI_MODE_BT1120_1080I_60, /**<ITU-R BT.1120 YUV4:2:2 standard, 1080I_60 *//**<CNcomment: ITU-R BT.1120 YUV4:2:2 标准，1080I_60 */
    HI_UNF_VI_MODE_BT1120_1080P_25, /**<ITU-R BT.1120 YUV4:2:2 standard, 1080P *//**<CNcomment: ITU-R BT.1120 YUV4:2:2 标准，1080P_25 */
    HI_UNF_VI_MODE_BT1120_1080P_30, /**<ITU-R BT.1120 YUV4:2:2 standard, 1080P *//**<CNcomment: ITU-R BT.1120 YUV4:2:2 标准，1080P_30 */
    HI_UNF_VI_MODE_BT1120_1080P_50, /**<ITU-R BT.1120 YUV4:2:2 standard, 1080P *//**<CNcomment: ITU-R BT.1120 YUV4:2:2 标准，1080P_50 */
    HI_UNF_VI_MODE_BT1120_1080P_60, /**<ITU-R BT.1120 YUV4:2:2 standard, 1080P *//**<CNcomment: ITU-R BT.1120 YUV4:2:2 标准，1080P_60 */  
    HI_UNF_VI_MODE_DIGITAL_CAMERA_480P_30,    /**<Digital camera. The data format can be RGB or YCbCr,480P_30*//**<CNcomment:数字摄像头，数据可能是RGB或者YCbCr，480P_30 */
    HI_UNF_VI_MODE_DIGITAL_CAMERA_576P_30,    /**<Digital camera. The data format can be RGB or YCbCr,576P_30 *//**<CNcomment:数字摄像头，数据可能是RGB或者YCbCr，576P_30*/
    HI_UNF_VI_MODE_DIGITAL_CAMERA_720P_30,    /**<Digital camera. The data format can be RGB or YCbCr,720P_30 *//**<CNcomment:数字摄像头，数据可能是RGB或者YCbCr，720P_30 */
    HI_UNF_VI_MODE_DIGITAL_CAMERA_1080P_30,   /**<Digital camera. The data format can be RGB or YCbCr,1080P_30 *//**<CNcomment:数字摄像头，数据可能是RGB或者YCbCr ，1080P_30*/
    HI_UNF_VI_MODE_HDMIRX,              /**HDMI INPUT *//**<CNcomment: ITU-R YUV4:4:4 标准 逐行信号*/  
    HI_UNF_VI_MODE_BUTT
} HI_UNF_VI_INPUT_MODE_E;

/**VI frame buffer management mode */
/**CNcomment: VI帧缓存管理方式 */
typedef enum hiUNF_VI_BUF_MGMT_E
{
    HI_UNF_VI_BUF_ALLOC = 0,  /**<VI alloc buffer *//**<CNcomment: VI申请帧BUF */
    HI_UNF_VI_BUF_MMAP,       /**<User alloc frame buffer, then maps it to VI *//**<CNcomment: 用户申请帧BUF，并映射给VI */
    HI_UNF_VI_BUF_BUTT
} HI_UNF_VI_BUF_MGMT_E;

/**VI frame buffer struct */
/**CNcomment: VI帧缓存结构体 */
typedef struct hiUNF_VI_BUFF_ATTR_S
{
    HI_VOID *pUsrVirAddr[MAX_VI_EXT_BUF_NUM];  /**<User virtual address *//**<CNcomment: 用户态虚拟地址*/
    HI_U32 u32PhyAddr[MAX_VI_EXT_BUF_NUM];     /**<Physical address *//**<CNcomment: 物理地址*/
    HI_U32 u32BufNum;                          /**<Buffer number *//**<CNcomment: 缓冲区个数*/
    HI_U32 u32Stride;                          /**<Stride of external frame buffer *//**<CNcomment:外部帧存的stride*/
} HI_UNF_VI_BUFFER_ATTR_S;


/**Vvideo 3D to 2D transfer mode */
/**CNcomment: 3D播放模式 */
typedef enum hiUNF_VI_3DT2D_MODE_E 
{
    HI_UNF_VI_3DT2D_OFF = 0,        /**< Keep normal left and right eye 3D format *//**<CNcomment: 保留3D格式的左右眼正常播放图像 */
    HI_UNF_VI_3DT2D_L,              /**< Keep normal left eye 3D format*//**<CNcomment: 保留3D格式的左眼图像 */
    HI_UNF_VI_3DT2D_R,              /**< Keep normal right eye 3D format*//**<CNcomment: 保留3D格式的右眼图像 */
    
    HI_UNF_VI_3DT2D_BUTT,
} HI_UNF_VI_3DT2D_MODE_E;


/**VI attributes */
/**CNcomment: VI属性 */
typedef struct hiUNF_VI_ATTR_S
{
    HI_BOOL                           bVirtual;             /**<Whether to create virtual VI, only u32BufNum is valid in virtual VI mode *//**<CNcomment: 是否创建虚拟VI，虚拟VI下只有u32BufNum参数有效*/
    HI_UNF_VI_INPUT_MODE_E            enInputMode;          /**<Video input mode *//**<CNcomment: 视频输入模式 */
    HI_RECT_S                         stInputRect;          /**<Crop region of a window *//**<CNcomment: 窗口的裁减区域 */
    HI_UNF_VIDEO_FORMAT_E             enVideoFormat;        /**<Output Video format *//**<CNcomment: 视频格式 */
    HI_UNF_VI_BUF_MGMT_E              enBufMgmtMode;        /**<VI frame buffer management mode *//**<CNcomment: VI帧缓存管理方式 */
    HI_U32                            u32BufNum;            /**<Number of available buffers *//**<CNcomment: 可用的帧buf的个数 */
    
    HI_BOOL                           bInterlace;           /**<Intput video interlace or progressive *//**<CNcomment: 逐行隔行信息 */
    HI_U32                            u32Width;             /**<Intput video width *//**<CNcomment: 源图像一帧的宽度,3D格式为单眼宽高 */
    HI_U32                            u32Height;            /**<Intput video feight *//**<CNcomment: 源图像一帧的高度，即隔行信号是两场的高度，逐行信号是一帧的高度，3D格式为单眼宽高 */
    HI_U32                            u32Vblank;	        /**<Intput video field blanking width, used 3d FB*//**<CNcomment: 场消隐区宽度，用于处理3D格式FP */
    HI_U32                            u32FrameRate;         /**<Intput video intput frequency *//**<CNcomment: 输入帧率 [24Hz,85] */
    HI_UNF_VIDEO_FRAME_PACKING_TYPE_E en3dFmt;              /**<Intput video format 2D or3D *//**<CNcomment: 2D/3D 格式 */
    HI_UNF_VIDEO_FORMAT_E             enPixFmt;             /**<Intput pixel format *//**<CNcomment: 输入像素格式 RGB444,YUV444/422 */ 
    HI_UNF_PIXEL_BITWIDTH_E           enSrcBitWidth;        /**<Intput pixel Width *//**<CNcomment: 输入位宽 8/10/12BIT */
    HI_UNF_OVERSAMPLE_MODE_E          enOverSample;         /**<Intput Over Sample format *//**<CNcomment: 过采样模式 */ 
    HI_UNF_COLOR_SPACE_E              enColorSpace;         /**<Intput Color Space *//**<CNcomment: 色彩空间 */
    HI_UNF_VI_3DT2D_MODE_E            enDst3DT2DMode;       /**<video 3D to 2D mode *//**<CNcomment: 静态,3D播放模式，由绑定模块(VENC/VDP)确定 */  
    HI_UNF_PIXEL_BITWIDTH_E           enDstBitWidth;        /**<output pixel Width *//**<CNcomment: 静态,输入为输入位宽为8bit则8bit输出，大于8bit则10bit输出；或则由绑定模块确认输出帧率 */ 
} HI_UNF_VI_ATTR_S;
/** @} */  /** <!-- ==== Struct Definition End ==== */

/********************************API Declaration********************************/
/** \addtogroup      VI */
/** @{ */  /** <!-- 【VI】 */

/**
 \brief Initializes the video input unit (VIU). CNcomment: 初始化VI CNend
 \attention \n
Before calling the VIU, you must call this application programming interface (API).
CNcomment: 调用VI模块要求首先调用该接口 CNend
 \param[in] N/A CNcomment: 无 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend
 \retval ::HI_ERR_VI_DEV_NOT_EXIST No VI device exists. CNcomment: 设备不存在 CNend
 \retval ::HI_ERR_VI_NOT_DEV_FILE The file is not a VI file. CNcomment: 文件非设备 CNend
 \retval ::HI_ERR_VI_DEV_OPENED The VI device fails to be started. CNcomment: 打开设备失败 CNend
 \see \n
N/A CNcomment: 无 CNend
 */
HI_S32 HI_UNF_VI_Init(HI_VOID);

/**
 \brief Deinitializes the VIU. CNcomment: 去初始化VI CNend
 \attention \n
 \param[in] N/A CNcomment: 无 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend
 \see \n
N/A CNcomment: 无 CNend
 */
HI_S32 HI_UNF_VI_DeInit(HI_VOID);

/**
 \brief Obtains the attributes of a VI port. CNcomment: 获取VI端口默认属性 CNend
 \attention \n
By default, port 0 is used, half D1 and BT656 formats are used, the data of the odd field is collected, and the semi-planar YUV4:2:0 data is output.
CNcomment: 默认Port=0, HALF-D1, BT656，采集偶场，输出SemiPlanner YUV4:2:0格式数据 CNend
 \param[out] pstAttr Pointer to VI attributes CNcomment: VI属性指针 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend
 \retval ::HI_ERR_VI_NULL_PTR The pointer is null. CNcomment: 空指针 CNend
 \see \n
N/A CNcomment: 无 CNend
 */
HI_S32 HI_UNF_VI_GetDefaultAttr(HI_UNF_VI_ATTR_S *pstAttr);

/**
 \brief Set the attributes of a VI port. CNcomment: 设置VI Port属性 CNend
 \attention \n
This reserved API is combined with HI_UNF_VI_Create, and cannot be called dynamically.
CNcomment: 预留接口，与Create合一，不可动态调用 CNend
 \param[in] hVi VI handle CNcomment: VI句柄 CNend
 \param[in] pstAttr  Pointer to VI attributes CNcomment: VI属性指针 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend
 \retval ::HI_ERR_VI_NULL_PTR The pointer is null. CNcomment: 空指针 CNend
 \retval ::HI_ERR_VI_CHN_NOT_EXIST The handle is incorrect. CNcomment: 句柄错误 CNend
 \retval ::HI_ERR_VI_CHN_INIT_BUF_ERR An error occurs when the buffer is initialized. CNcomment: 初始化buffer错误 CNend
 \retval ::HI_ERR_VI_INVALID_PARA The attribute parameters are incorrect. CNcomment: 属性参数错误 CNend
 \see \n
N/A CNcomment: 无 CNend
 */
HI_S32 HI_UNF_VI_SetAttr(HI_HANDLE hVI, HI_UNF_VI_ATTR_S *pstAttr);

/**
 \brief Obtains the attributes of a VI port. CNcomment: 获取VI Port属性 CNend
 \attention \n
 \param[in] hVi VI handle CNcomment: VI句柄 CNend
 \param[out] pstAttr Pointer to VI attributes CNcomment: VI属性指针 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend
 \retval ::HI_ERR_VI_NULL_PTR The pointer is null. CNcomment: 空指针 CNend
 \retval ::HI_ERR_VI_CHN_NOT_EXIST The handle is incorrect. CNcomment: 句柄错误 CNend
 \see \n
N/A CNcomment: 无 CNend
 */
HI_S32 HI_UNF_VI_GetAttr(HI_HANDLE hVI, HI_UNF_VI_ATTR_S *pstAttr);

/**
 \brief Enables a VI port to create a VI handle. CNcomment: 打开VI Port，创建Vi句柄 CNend
 \attention \n
Before calling the VIU, you must call this API.\n
Unless the virtual VI is used, the port ID must correspond to the hardware. That is, the hardware and software must use the same port\n
CNcomment: 调用VI 模块需要首先调用该接口\n
           除非使用虚拟VI, 否则PortID需要与硬件对应,硬件上使用了哪个Port,那么软件也必须使用哪个Port.\n CNend
 \param[in] enViPort VI port CNcomment: VI端口 CNend
 \param[in] pstAttr Pointer to VI attributes CNcomment: VI属性指针 CNend
 \param[out] phandle hVi Pointer to the VI handle CNcomment: VI句柄指针 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend
 \retval ::HI_ERR_VI_NULL_PTR The pointer is null. CNcomment: 空指针 CNend
 \retval ::HI_ERR_VI_CHN_INIT_BUF_ERR An error occurs when the buffer is initialized. CNcomment: 初始化buffer错误 CNend
 \retval ::HI_ERR_VI_INVALID_PARA The attribute parameters are incorrect. CNcomment: 属性参数错误 CNend
 \see \n
N/A CNcomment: 无 CNend
 */

HI_S32 HI_UNF_VI_Create(HI_UNF_VI_E enViPort, HI_UNF_VI_ATTR_S *pstAttr, HI_HANDLE *phandle);

/**
 \brief Disable a VI port to destroy the handle. CNcomment: 关闭VI Port，销毁句柄 CNend
 \attention \n
 \param[in] hVi VI handle CNcomment: VI句柄 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend
 \retval ::HI_ERR_VI_CHN_NOT_EXIST The handle is incorrect. CNcomment: 句柄错误 CNend
 \see \n
N/A CNcomment: 无 CNend
 */
HI_S32 HI_UNF_VI_Destroy(HI_HANDLE hVI);

/**
 \brief Start VI. CNcomment: 启动VI，销毁句柄 CNend
 \attention \n
 \param[in] hVI VI handle CNcomment: VI句柄 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend
 \retval ::HI_ERR_VI_CHN_NOT_EXIST The handle is incorrect. CNcomment: 句柄错误 CNend
 \see \n
N/A CNcomment: 无 CNend
 */
HI_S32 HI_UNF_VI_Start(HI_HANDLE hVI);

/**
 \brief Stop VI. CNcomment: 停止VI，销毁句柄 CNend
 \attention \n
 \param[in] hVI VI handle CNcomment: VI句柄 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend
 \retval ::HI_ERR_VI_CHN_NOT_EXIST The handle is incorrect. CNcomment: 句柄错误 CNend
 \see \n
N/A CNcomment: 无 CNend
 */
HI_S32 HI_UNF_VI_Stop(HI_HANDLE hVI);

/**
 \brief Set external buffer which will be managed by VI. CNcomment: 设置外部缓冲区，由VI管理 CNend
 \attention \n
 The interface is only valid in real VI scene, and ::HI_UNF_VI_BUF_MMAP mode should be set.
 CNcomment: 用户只能在实体VI场景下，设置为::HI_UNF_VI_BUF_MMAP模式时，才能使用此接口\n CNend
 \param[in] hVI VI handle CNcomment: VI句柄 CNend
 \param[in] pstBufAttr Buffer attributes CNcomment: 缓冲区属性 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend
 \retval ::HI_ERR_VI_CHN_NOT_EXIST The handle is incorrect. CNcomment: 句柄错误 CNend
 \see \n
N/A CNcomment: 无 CNend
 */
HI_S32 HI_UNF_VI_SetExternBuffer(HI_HANDLE hVI, HI_UNF_VI_BUFFER_ATTR_S* pstBufAttr);

/**
 \brief Obtains the address of a frame buffer over a VI port.
 CNcomment:获取虚拟VI Port 一帧图像Buffer CNend
 \attention \n
 The interface can only be used in virual VI scene\n
CNcomment: 只有在虚拟VI场景下，才可使用此接口还帧\n CNend
 \param[in] hVI VI handle CNcomment: VI句柄 CNend
 \param[out] pViBuf Pointer to the VI buffer CNcomment: VI buffer指针 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend
 \retval ::HI_ERR_VI_CHN_NOT_EXIST The handle is incorrect. CNcomment: 句柄错误 CNend
 \retval ::HI_ERR_VI_NULL_PTR The pointer is null. CNcomment: 空指针 CNend
 \see \n
N/A CNcomment: 无 CNend
 */
HI_S32 HI_UNF_VI_DequeueFrame(HI_HANDLE hVI, HI_UNF_VIDEO_FRAME_INFO_S *pFrameInfo);

/**
 \brief Releases the VI frame buffer for the driver.
 CNcomment: 发送虚拟VI Port 一帧图像Buffer，驱动虚拟Port工作 CNend
 \attention
 The interface can only be used in virual VI scene\n
CNcomment: 只有在虚拟VI场景下，才可使用此接口送帧\n CNend
 \param[in] hVI VI handle CNcomment: VI句柄 CNend
 \param[in] pViBuf Pointer to the VI buffer CNcomment: VI buffer指针 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend
 \retval ::HI_ERR_VI_CHN_NOT_EXIST The handle is incorrect. CNcomment: 句柄错误 CNend
 \retval ::HI_ERR_VI_NULL_PTR The pointer is null. CNcomment: 空指针 CNend
 \see \n
N/A CNcomment: 无 CNend
 */
HI_S32 HI_UNF_VI_QueueFrame(HI_HANDLE hVI, HI_UNF_VIDEO_FRAME_INFO_S *pFrameInfo);

/**
 \brief Obtains the address of a YUV picture. CNcomment: 获取一帧YUV 图像地址 CNend
 \attention \n
You do not need to call this API generally. It is recommended that you bind the VIU to the video encoder (VENC) or video output unit (VOU) to implement the encoding function or VIVO loopback function.
CNcomment: 一般情况下，用户不需要调用此接口，推荐直接将VI与VENC或者VO进行绑定以便实现编码或者VIVO环回功能。 CNend
 \param[in] hVi VI handle CNcomment: VI句柄 CNend
 \param[out] pViBuf Pointer to the VI buffer CNcomment: VI buffer指针 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend
 \retval ::HI_ERR_VI_CHN_NOT_EXIST The handle is incorrect. CNcomment: 句柄错误 CNend
 \retval ::HI_ERR_VI_NULL_PTR The pointer is null. CNcomment: 空指针 CNend
 \see \n
N/A CNcomment: 无 CNend
 */
HI_S32 HI_UNF_VI_AcquireFrame(HI_HANDLE hVI, HI_UNF_VIDEO_FRAME_INFO_S *pFrameInfo, HI_U32 u32TimeoutMs);

/**
 \brief Releases the frame buffer for the VIU after the VENC or VOU obtains a YUV picture.
CNcomment: 用户归还一帧YUV 图像地址 CNend
 \attention \n
You do not need to call this API generally. It is recommended that you bind the VIU to the VENC or VOU to implement the encoding function or VIVO loopback function.
CNcomment: 一般情况下，用户不需要调用此接口，推荐直接将VI与VENC或者VO进行绑定以便实现编码或者VIVO环回功能。 CNend
 \param[in] hVi VI handle CNcomment: VI句柄 CNend
 \param[in] pViBuf Pointer to the VI buffer CNcomment: VI buffer指针 CNend
 \retval ::HI_SUCCESS Success CNcomment: 成功 CNend
 \retval ::HI_ERR_VI_CHN_NOT_EXIST The handle is incorrect. CNcomment: 句柄错误 CNend
 \retval ::HI_ERR_VI_NULL_PTR The pointer is null. CNcomment: 空指针 CNend
 \see \n
N/A CNcomment: 无 CNend
 */
HI_S32 HI_UNF_VI_ReleaseFrame(HI_HANDLE hVI, const HI_UNF_VIDEO_FRAME_INFO_S *pFrameInfo);

/** @} */  /** <!-- ====API Declaration End ==== */

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif
#endif
