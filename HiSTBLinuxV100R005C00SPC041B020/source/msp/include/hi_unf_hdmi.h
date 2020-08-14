
/******************************************************************************

  Copyright (C), 2001-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_unf_hdmi.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2009-12-10
  Description   :
  History       :
  1.Date        : 2009-12-10
    Author      : QuYaxin 46153
    Modification: Created file

*******************************************************************************/

#ifndef __HI_UNF_HDMI_H__
#define __HI_UNF_HDMI_H__

/* add include here */
#include "hi_unf_common.h"
#include "hi_unf_sound.h"
#include "hi_unf_edid.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


/*************************** Structure Definition ****************************/
/** \addtogroup      HDMI */
/** @{ */  /** <!-- [HDMI] */

/**the max infoframe length*//**CNcomment:最大信息帧长度 */
#define HI_UNF_HDMI_MAX_INFOFRAME_LEN   0X20

/**HDMI default value*//**CNcomment:HDMI 缺省值 */
#define HI_UNF_HDMI_DEFAULT_SETTING     0x00

/**HDMI interface ID *//**CNcomment:HDMI接口ID  */
typedef enum hiUNF_HDMI_ID_E
{
    HI_UNF_HDMI_ID_0         = 0,          /**<HDMI interface 0*/ /**<CNcomment:HDMI接口0 */
    HI_UNF_HDMI_ID_BUTT
} HI_UNF_HDMI_ID_E;

/**HDMI event type*//**CNcomment: HDMI事件类型 */
typedef enum hiUNF_HDMI_EVENT_TYPE_E
{
    HI_UNF_HDMI_EVENT_HOTPLUG = 0x10,       /**<HDMI HotPlug event type*//**<CNcomment:<HDMI热插拔事件 */
    HI_UNF_HDMI_EVENT_NO_PLUG,              /**<HDMI unplug event type*//**<CNcomment:HDMI Cable没有连接 事件 */
    HI_UNF_HDMI_EVENT_EDID_FAIL,            /**<HDMI read edid fail event type*//**<CNcomment:HDMI EDID读取失败事件 */
    HI_UNF_HDMI_EVENT_HDCP_FAIL,            /**<HDCP authentication fail event type *//**<CNcomment:HDCP验证失败事件 */
    HI_UNF_HDMI_EVENT_HDCP_SUCCESS,         /**<HDCP authentication succeed event type*//**<CNcomment:HDCP验证成功 */
    HI_UNF_HDMI_EVENT_RSEN_CONNECT,         /**<TMDS link is connected*//**<CNcomment:TMDS链接成功 */
    HI_UNF_HDMI_EVENT_RSEN_DISCONNECT,      /**<TMDS link is disconnected*//**<CNcomment:TMDS未链接 */
    HI_UNF_HDMI_EVENT_HDCP_USERSETTING,     /**<HDMI Reset *//**<CNcomment:HDCP 复位*/
    HI_UNF_HDMI_EVENT_BUTT
}HI_UNF_HDMI_EVENT_TYPE_E;

/*Video color space mode*//**CNcomment:视频颜色空间类型*/
typedef enum hiUNF_HDMI_VIDEO_MODE
{
    HI_UNF_HDMI_VIDEO_MODE_RGB444,          /**<RGB444 output mode*//**<CNcomment:RGB444输出模式 */
    HI_UNF_HDMI_VIDEO_MODE_YCBCR422,        /**<YCBCR422 output mode*//**<CNcomment:YCBCR422输出模式 */
    HI_UNF_HDMI_VIDEO_MODE_YCBCR444,        /**<YCBCR444 output mode*//**<CNcomment:YCBCR444输出模式 */
    HI_UNF_HDMI_VIDEO_MODE_YCBCR420,        /**<YCBCR420 output mode*//**<CNcomment:YCBCR420输出模式 */
    HI_UNF_HDMI_VIDEO_MODE_BUTT
}HI_UNF_HDMI_VIDEO_MODE_E;

/*HDMI Output Aspect Ratio*//**CNcomment:HDMI输出宽高比*/
typedef enum hiUNF_HDMI_ASPECT_RATIO_E
{
    HI_UNF_HDMI_ASPECT_RATIO_NO_DATA,        /**<Aspect Ratio unknown *//**<CNcomment:未知宽高比 */
    HI_UNF_HDMI_ASPECT_RATIO_4TO3,           /**<Aspect Ratio 4:3  *//**<CNcomment:宽高比4:3*/
    HI_UNF_HDMI_ASPECT_RATIO_16TO9,          /**<Aspect Ratio 16:9 *//**<CNcomment:宽高比16:9 */
    HI_UNF_HDMI_ASPECT_RATIO_64TO27,
	HI_UNF_HDMI_ASPECT_RATIO_256TO135,
    HI_UNF_HDMI_ASPECT_RATIO_FUTURE,
    HI_UNF_HDMI_ASPECT_RATIO_BUTT
}HI_UNF_HDMI_ASPECT_RATIO_E;


/**HDMI Deep color mode*//**CNcomment: HDMI 深色模式 */
typedef enum hiUNF_HDMI_DEEP_COLOR_E
{
    HI_UNF_HDMI_DEEP_COLOR_24BIT = 0x00,    /**<HDMI Deep color 24bit mode*//**<CNcomment:HDMI 24bit 深色模式  */
    HI_UNF_HDMI_DEEP_COLOR_30BIT,           /**<HDMI Deep color 30bit mode*//**<CNcomment:HDMI 30bit 深色模式  */
    HI_UNF_HDMI_DEEP_COLOR_36BIT,           /**<HDMI Deep color 36bit mode*//**<CNcomment:HDMI 36bit 深色模式  */
    HI_UNF_HDMI_DEEP_COLOR_OFF   = 0xff,
    HI_UNF_HDMI_DEEP_COLOR_BUTT
}HI_UNF_HDMI_DEEP_COLOR_E;

/**HDMI AVI infoframe BarInfo enum*//**CNcomment: HDMI AVI信息帧 BarInfo 枚举 */
typedef enum hiUNF_HDMI_BARINFO_E
{
   HDMI_BAR_INFO_NOT_VALID,                 /**<Bar Data not valid *//**<CNcomment:无效Bar数据  */
   HDMI_BAR_INFO_V,                         /**<Vertical bar data valid *//**<CNcomment:垂直Bar数据有效  */
   HDMI_BAR_INFO_H,                         /**<Horizental bar data valid *//**<CNcomment:水平bar数据有效  */
   HDMI_BAR_INFO_VH                         /**<Horizental and Vertical bar data valid *//**<CNcomment:水平垂直Bar数据同时有效 */
}HI_UNF_HDMI_BARINFO_E;

/**HDMI AVI infofram ScanInfo enum*//**CNcomment: HDMI AVI信息帧 ScanInfo 枚举 */
typedef enum hiUNF_HDMI_SCANINFO_E
{
    HDMI_SCAN_INFO_NO_DATA      = 0,        /**< No Scan information*//**<CNcomment:无扫描信息  */
    HDMI_SCAN_INFO_OVERSCANNED  = 1,        /**< Scan information, Overscanned (for television) *//**<CNcomment:扫描信息:全画面扫描  */
    HDMI_SCAN_INFO_UNDERSCANNED = 2,        /**< Scan information, Underscanned (for computer) *//**<CNcomment:扫描信息: 非全画面扫描  */
    HDMI_SCAN_INFO_FUTURE
}HI_UNF_HDMI_SCANINFO_E;

/**HDMI AVI InfoFrame picture scale enum*//**CNcomment: HDMI AVI信息帧 Picture scale 枚举 */
typedef enum hiUNF_HDMI_PICTURE_SCALING_E
{
    HDMI_PICTURE_NON_UNIFORM_SCALING,       /**< No Known, non-uniform picture scaling  *//**<CNcomment:统一图像坐标  */
    HDMI_PICTURE_SCALING_H,                 /**< Picture has been scaled horizentally *//**<CNcomment:图像水平坐标化  */
    HDMI_PICTURE_SCALING_V,                 /**< Picture has been scaled Vertically *//**<CNcomment:图像垂直坐标化  */
    HDMI_PICTURE_SCALING_HV                 /**< Picture has been scaled Horizentally and Vertically   *//**<CNcomment:图像水平垂直坐标化  */
} HI_UNF_HDMI_PICTURE_SCALING_E;

/**HDMI AVI InfoFrame colorimetry enum*//**CNcomment: HDMI AVI信息帧 色度空间 枚举 */
typedef enum hiUNF_HDMI_COLORSPACE_E
{
    HDMI_COLORIMETRY_NO_DATA,               /**<Colorimetry No Data option*//**<CNcomment:Colorimetry No Data选项 */
    HDMI_COLORIMETRY_ITU601,                /**<Colorimetry ITU601 option*//**<CNcomment:Colorimetry ITU601色度空间选项 */
    HDMI_COLORIMETRY_ITU709,                /**<Colorimetry ITU709 option*//**<CNcomment:Colorimetry ITU709色度空间选项 */
    HDMI_COLORIMETRY_EXTENDED,              /**<Colorimetry extended option*//**<CNcomment:Colorimetry 扩展选项 */
    HDMI_COLORIMETRY_XVYCC_601,             /**<Colorimetry xvYCC601 extened option*//**<CNcomment:Colorimetry xvYCC601扩展选项 */
    HDMI_COLORIMETRY_XVYCC_709,             /**<Colorimetry xvYCC709 extened option*//**<CNcomment:Colorimetry xvYCC709扩展选项 */
	HDMI_COLORIMETRY_S_YCC_601,             /**<Colorimetry S YCC 601 extened option*//**<CNcomment:Colorimetry S YCC 601扩展选项 */
	HDMI_COLORIMETRY_ADOBE_YCC_601,         /**<Colorimetry ADOBE YCC 601 extened option*//**<CNcomment:Colorimetry ADOBE YCC 601扩展选项 */
	HDMI_COLORIMETRY_ADOBE_RGB,             /**<Colorimetry ADOBE RGB extened option*//**<CNcomment:Colorimetry ADOBE RGB扩展选项 */
	HDMI_COLORIMETRY_2020_CONST_LUMINOUS,    /**<Colorimetry ITU2020 extened option*//**<CNcomment:Colorimetry ITU2020扩展选项:BT2020cYCC */
    HDMI_COLORIMETRY_2020_NON_CONST_LUMINOUS,/**<Colorimetry ITU2020 extened option*//**<CNcomment:Colorimetry ITU2020扩展选项:BT2020RGB或BT2020YCC*/
} HI_UNF_HDMI_COLORSPACE_E;

/**HDMI AVI InfoFrame RGB range enum*//**CNcomment: HDMI AVI信息帧 RGB色度范围 枚举 */
typedef enum hiUNF_HDMI_RGB_QUAN_RAGE_E
{
    HDMI_RGB_QUANTIZATION_DEFAULT_RANGE,    /**< Defaulr range, it depends on the video format *//**<CNcomment:默认色度范围，依赖于视频制式 */
    HDMI_RGB_QUANTIZATION_LIMITED_RANGE,    /**< Limited quantization range of 220 levels when receiving a CE video format*//**<CNcomment:受限色度范围16-234 */
    HDMI_RGB_QUANTIZATION_FULL_RANGE        /**< Full quantization range of 256 levels when receiving an IT video format*//**<CNcomment:全色度范围 0-255 */
} HI_UNF_HDMI_RGB_QUAN_RAGE_E;

/**HDMI AVI InfoFrame YCC quantization range enum *//**CNcomment:HDMI AVI信息帧 YCC色度范围 枚举 */
typedef enum hiUNF_HDMI_YCC_QUAN_RAGE_E
{
    HDMI_YCC_QUANTIZATION_LIMITED_RANGE,    /**< Limited quantization range of 220 levels when receiving a CE video format*//**<CNcomment:受限色度范围16-234 */
    HDMI_YCC_QUANTIZATION_FULL_RANGE        /**< Full quantization range of 256 levels when receiving an IT video format*//**<CNcomment:全色度范围 0-255 */
}HI_UNF_HDMI_YCC_QUAN_RAGE_E;

/**HDMI AVI InfoFrame AVI video content type enum*//**CNcomment:HDMI AVI信息帧 AVI视频内容的类型 枚举 */
typedef enum hiUNF_HDMI_CONTENT_TYPE_E
{
    HDMI_CONTNET_GRAPHIC,                   /**< Graphics type*//**<CNcomment:图像 */
    HDMI_CONTNET_PHOTO,                     /**< Photo type*//**<CNcomment:照片 */
    HDMI_CONTNET_CINEMA,                    /**< Cinema type*//**<CNcomment:电影院 */
    HDMI_CONTNET_GAME                       /**< Game type*//**<CNcomment:游戏 */
}HI_UNF_HDMI_CONTENT_TYPE_E;

/**HDMI Priority judgments strategy enum*//**CNcomment:HDMI 优先判断策略 枚举*/
typedef enum hiUNF_HDMI_DEFAULT_ACTION_E
{
    HI_UNF_HDMI_DEFAULT_ACTION_NULL,   /**<Default action null*//**<CNcomment:无默认优先策略 */
    HI_UNF_HDMI_DEFAULT_ACTION_HDMI,   /**<Default action HDMI*//**<CNcomment:优先判断HDMI */
    HI_UNF_HDMI_DEFAULT_ACTION_DVI,    /**<Default action DVI*//**<CNcomment:优先判断DVI */
    HI_UNF_HDMI_DEFAULT_ACTION_BUTT
}HI_UNF_HDMI_DEFAULT_ACTION_E;


/**The hotplug callback function interface */
/**CNcomment: 接口热插拔回调函数 */
typedef void (*HI_UNF_HDMI_CALLBACK)(HI_UNF_HDMI_EVENT_TYPE_E event, HI_VOID *pPrivateData);

/**HDMI Callback Struct*/
/**CNcomment: HDMI回调结构 */
typedef struct hiUNF_HDMI_CALLBACK_FUNC_S
{
    HI_UNF_HDMI_CALLBACK pfnHdmiEventCallback;  /**<callback function pointer*//**<CNcomment:回调函数指针*/
    HI_VOID             *pPrivateData;         /**<callback funtion param*//**<CNcomment:回调函数参数*/
 }HI_UNF_HDMI_CALLBACK_FUNC_S;

/**HDMI Open Param*/
/**CNcomment: HDMI打开参数 */
typedef struct hiUNF_HDMI_OPEN_PARA_S
{
    HI_UNF_HDMI_DEFAULT_ACTION_E enDefaultMode;/**<HDMI Priority judgments strategy enum*//**<CNcomment:HDMI 优先判断策略 枚举*/
}HI_UNF_HDMI_OPEN_PARA_S;

/**Current HDCP version enum*/
/**CNcomment:当前HDCP版本 枚举*/
typedef enum hiUNF_HDMI_HDCP_VERSION_E
{
    HI_UNF_HDMI_HDCP_VERSION_NONE,      /**<HDCP version, none HDCP*//**<CNcomment:无HDCP */
    HI_UNF_HDMI_HDCP_VERSION_HDCP14,    /**<HDCP version, HDCP1.4*//**<CNcomment:HDCP1.4 */
    HI_UNF_HDMI_HDCP_VERSION_HDCP22,    /**<HDCP version, HDCP2.2*//**<CNcomment:HDCP2.2 */
    HI_UNF_HDMI_HDCP_VERSION_BUTT
}HI_UNF_HDMI_HDCP_VERSION_E;

/**HDMI status*/
/**CNcomment: HDMI状态 */
typedef struct hiUNF_HDMI_STATUS_S
{
    HI_BOOL                 bConnected;             /**<The Device is connected or disconnected *//**<CNcomment:设备是否连接 */
    HI_BOOL                 bSinkPowerOn;           /**<The sink is PowerOn or not*//**<CNcomment:Sink设备是否上电 */
    HI_BOOL                 bAuthed;                /**<HDCP Authentication *//**<CNcomment:HDCP 是否握手完成 */
    HI_U8                   u8Bksv[5];              /**<Bksv of sink 40bits*//**<CNcomment:接收端的Bksv */
    HI_UNF_HDMI_HDCP_VERSION_E enHDCPVersion;       /**<Current HDCP version*//**<CNcomment:当前HDCP版本 */
}HI_UNF_HDMI_STATUS_S;

/**Current HDCP mode enum*/
/**CNcomment:HDCP模式 枚举*/
typedef enum hiUNF_HDMI_HDCP_MODE_E
{
    HI_UNF_HDMI_HDCP_MODE_AUTO,			/**<HDCP mode, HDCP auto*//**<CNcomment:HDCP自动模式，HDMI驱动根据Sink端能力自动选择认证版本 */
    HI_UNF_HDMI_HDCP_MODE_1_4,			/**<HDCP mode, HDCP1.4*//**<CNcomment:HDCP1.4 */
    HI_UNF_HDMI_HDCP_MODE_2_2,			/**<HDCP mode, HDCP2.2*//**<CNcomment:HDCP2.2 */
    HI_UNF_HDMI_HDCP_BUTT
}HI_UNF_HDMI_HDCP_MODE_E;

/**the config parameter of HDMI interface*/
/**CNcomment:HDMI 接口参数配置 */
typedef struct hiUNF_HDMI_ATTR_S
{
    HI_BOOL                 bEnableHdmi;         /**<force to HDMI or DVI,the value must set before HI_UNF_HDMI_Start or behind HI_UNF_HDMI_Stop*//**<CNcomment:是否强制HDMI,否则为DVI.该值必须在 HI_UNF_HDMI_Start之前或者HI_UNF_HDMI_Stop之后设置  */

    HI_BOOL                 bEnableVideo;        /**<parameter must set HI_TRUE,or the HDMI diver will force to set HI_TRUE*//**<CNcomment:必须是HI_TRUE, 如果是HI_FALSE:HDMI驱动会强制设置为HI_TRUE */

    HI_UNF_HDMI_VIDEO_MODE_E enVidOutMode;       /**<HDMI output vedio mode VIDEO_MODE_YCBCR,VIDEO_MODE_YCBCR444,VIDEO_MODE_YCBCR422,VIDEO_MODE_RGB444 *//**<CNcomment:HDMI输出视频模式，VIDEO_MODE_YCBCR444，VIDEO_MODE_YCBCR422，VIDEO_MODE_RGB444 */
    HI_UNF_HDMI_DEEP_COLOR_E enDeepColorMode;    /**<Deep Color output mode,defualt: HI_UNF_HDMI_DEEP_COLOR_24BIT *//**<CNcomment:DeepColor输出模式, 默认为HI_UNF_HDMI_DEEP_COLOR_24BIT */
    HI_BOOL                 bxvYCCMode;          /**<the xvYCC output mode,default:HI_FALSE*//**<CNcomment:< xvYCC输出模式，默认为HI_FALSE */

    HI_BOOL                 bEnableAudio;        /**<Enable flag of Audio*//**CNcomment:是否Enable音频 */

    HI_BOOL                 bEnableAviInfoFrame; /**<Enable flag of AVI InfoFrame,suggestion:enable *//**<CNcomment:是否使能 AVI InfoFrame，建议使能 */
    HI_BOOL                 bEnableAudInfoFrame; /**<Enable flag of Audio InfoFrame,suggestion:enable*//**<CNcomment:是否使能 AUDIO InfoFrame，建议使能 */
    HI_BOOL                 bEnableSpdInfoFrame; /**<Enable flag of SPD info frame,suggestion:disable*//**<CNcomment:是否使能 SPD InfoFrame， 建议关闭 */
    HI_BOOL                 bEnableMpegInfoFrame;/**<Enable flag of MPEG info frame,suggestion:disable*//**<CNcomment:是否使能 MPEG InfoFrame， 建议关闭 */

    HI_BOOL                 bHDCPEnable;         /**<0:HDCP disable mode,1:eable HDCP mode*//**<CNcomment:< 0:HDCP不激活，1:HDCP模式打开 */

    HI_BOOL                 bEnableVidModeAdapt; /**<Enable flag of vedio mode & DVI adapting case of user setting incorrect,default:HI_TRUE.When user have no any adapting strategy,suggestion HI_TRUE*//**<CNcomment:当用户设置的输出颜色空间与DVI 错误时是否使能 自动校正，不使能则用户设置错误时直接返回失败，默认使能 。建议当用户有自适应策略时关闭，否则使能*/
    HI_BOOL                 bEnableDeepClrAdapt; /**<Enable flag of deep color mode adapting case of user setting incorrect,default: HI_FALSE.When user have no any adapting strategy,suggestion HI_TRUE*//**<CNcomment:当用户设置的输出色深(位宽)错误时是否使能 自动校正，不使能则用户设置错误时直接返回失败，默认使能。建议当用户有自适应策略时关闭，否则使能*/
    HI_BOOL                 bAuthMode;           /**<Enable flag of authentication, suggestion:disable *//**<CNcomment:是否使能认证模式，建议默认不是能 */
	HI_UNF_HDMI_HDCP_MODE_E enHDCPMode;          /**<HDCP mode:HI_UNF_HDMI_HDCP_MODE_AUTO, HI_UNF_HDMI_HDCP_MODE_1_4,HI_UNF_HDMI_HDCP_MODE_2_2*//**<CNcomment:< HDCP等级，HI_UNF_HDMI_HDCP_MODE_AUTO(根据能力自动选择)，HI_UNF_HDMI_HDCP_MODE_1_4,HI_UNF_HDMI_HDCP_MODE_2_2 */
}HI_UNF_HDMI_ATTR_S;

/**HDMI infoFrame type definition*/
/**CNcomment: HDMI 信息帧类型定义 */
typedef enum tagHI_UNF_HDMI_INFOFRAME_TYPE_E
{
    HI_INFOFRAME_TYPE_AVI,          /**<HDMI AVI InfoFrame type defintion*//**<CNcomment:HDMI AVI InfoFrame 类型定义 */
    HI_INFOFRAME_TYPE_SPD,          /**<HDMI SPD InfoFrame type defintion*//**<CNcomment:HDMI SPD InfoFrame 类型定义 */
    HI_INFOFRAME_TYPE_AUDIO,        /**<HDMI AUDIO InfoFrame type defintion*//**<CNcomment:HDMI AUDIO InfoFrame 类型定义 */
    HI_INFOFRAME_TYPE_MPEG,         /**<HDMI MPEG InfoFrame type defintion*//**<CNcomment:HDMI MPEG InfoFrame 类型定义 */
    HI_INFOFRAME_TYPE_VENDORSPEC,   /**<HDMI Specific InfoFrame type defintion*//**<CNcomment:HDMI Vendor Specific InfoFrame 类型定义 */
    HI_INFOFRAME_TYPE_BUTT
}HI_UNF_HDMI_INFOFRAME_TYPE_E;

/**HDMI AVI InfoFrame parameter struct,please reference EIA-CEA-861-D*/
/**CNcomment: HDMI AVI 信息帧参数结构, 请参考EIA-CEA-861-D */
typedef struct hiUNF_HDMI_AVI_INFOFRAME_VER2_S
{
    HI_UNF_ENC_FMT_E               enTimingMode;            /**<AVI video timing format*//**<CNcomment:AVI视频timing格式 */
    HI_UNF_HDMI_VIDEO_MODE_E       enOutputType;            /**<AVI video output color space*//**<CNcomment:AVI视频输出颜色格式 */
    HI_BOOL                        bActive_Infor_Present;   /**<AVI video Active_Infor_Present flag*//**<CNcomment:AVI视频Active_Infor_Present标志位 */
    HI_UNF_HDMI_BARINFO_E          enBarInfo;               /**<AVI video BarInfo type*//**<CNcomment:AVI视频BarInfo类型 */
    HI_UNF_HDMI_SCANINFO_E         enScanInfo;              /**<AVI video ScanInfo type*//**<CNcomment:AVI视频ScanInfo类型 */
    HI_UNF_HDMI_COLORSPACE_E       enColorimetry;           /**<AVI video Colorimetry type*//**<CNcomment:AVI视频Colorimetry类型 */
    HI_UNF_HDMI_ASPECT_RATIO_E     enAspectRatio;           /**<AVI video AspectRatio type*//**<CNcomment:AVI视频宽高比格式 */
    HI_UNF_HDMI_ASPECT_RATIO_E     enActiveAspectRatio;     /**<AVI video Active AspectRatio type*//**<CNcomment:AVI视频有效宽高比格式 */
    HI_UNF_HDMI_PICTURE_SCALING_E  enPictureScaling;        /**<AVI video picture scaling type*//**<CNcomment:AVI视频scaling格式 */
    HI_UNF_HDMI_RGB_QUAN_RAGE_E    enRGBQuantization;       /**<AVI video RGB Quantization*//**<CNcomment:AVI视频RGB色度范围 */
    HI_BOOL                        bIsITContent;            /**<AVI video ITContent flag*//**<CNcomment:AVI视频ITcontent */
    HI_U32                         u32PixelRepetition;      /**<AVI video Pixel Repetition flag*//**<CNcomment:AVI视频像素重传标志位 */

    HI_UNF_HDMI_CONTENT_TYPE_E     enContentType;           /**<AVI video content type*//**<CNcomment:AVI视频内容的类型 */
    HI_UNF_HDMI_YCC_QUAN_RAGE_E    enYCCQuantization;       /**<AVI video YCC Quantization*//**CNcomment:*< AVI视频YCC色度范围 */

    HI_U32                         u32LineNEndofTopBar;     /**<AVI video EndofTopBar coordinate,defualt:0 *//**<CNcomment:AVI视频EndofTopBar坐标，缺省为0 */
    HI_U32                         u32LineNStartofBotBar;   /**<AVI video StartofBotBar coordinate,defualt:0*//**<CNcomment:AVI视频StartofBotBar坐标，缺省为0 */
    HI_U32                         u32PixelNEndofLeftBar;   /**<AVI video EndofLeft coordinate,defualt:0*//**<CNcomment:AVI视频EndofLeft坐标，缺省为0 */
    HI_U32                         u32PixelNStartofRightBar;/**<AVI video StartofRightBar coordinate,defualt:0*//**<CNcomment:AVI视频StartofRightBar坐标，缺省为0 */
}HI_UNF_HDMI_AVI_INFOFRAME_VER2_S;

/**HDMI AUDIO InfoFrame parameter struct ,please reference EIA-CEA-861-D*/
/**CNcomment: HDMI 音频信息帧参数结构, 请参考EIA-CEA-861-D */
typedef struct hiUNF_HDMI_AUD_INFOFRAME_VER1_S
{
    HI_U32                            u32ChannelCount;          /**<audio frequency channel count*//**<CNcomment:音频 声道数 */
    HI_UNF_EDID_AUDIO_FORMAT_CODE_E   enCodingType;             /**<audio frequency coding type,default 0;Refer to Stream Header*//**<CNcomment:音频 编码类型，缺省为0：Refer to Stream Header */
    HI_U32                            u32SampleSize;            /**<audio frequency sample size,default 0,Refer to Stream Header*//**<CNcomment:音频 采样大小，缺省为0：Refer to Stream Header */
    HI_U32                            u32SamplingFrequency;     /**<audio frequency sampling frequency ,default 0,Refer to Stream Header*//**<CNcomment:音频 采样频率，缺省为0：Refer to Stream Header */
    HI_U32                            u32ChannelAlloc;          /**<audio frequency channel allocable ,default 0,Refer to Stream Header*//**<CNcomment:音频 声道分配，缺省为0：Refer to Stream Header */
    HI_U32                            u32LevelShift;            /**<audio frequency Levelshift ,default 0,Refer to Stream Header*//**<CNcomment:音频 Levelshift，缺省为0：Refer to Stream Header */
    HI_BOOL                           u32DownmixInhibit;        /**<audio frequency DownmixInhibit ,default 0,Refer to Stream Header*//**<CNcomment:音频 DownmixInhibit，缺省为0：Refer to Stream Header */
}HI_UNF_HDMI_AUD_INFOFRAME_VER1_S;

/**HDMI SPD InfoFrame parameter struct,please reference EIA-CEA-861-D*/
/**CNcomment: HDMI SPD信息帧参数结构 , 请参考EIA-CEA-861-D */
typedef struct hiUNF_HDMI_SPD_INFOFRAME_S
{
    HI_U8                          u8VendorName[8];          /**<vendor name*//**<CNcomment:卖方名称 */
    HI_U8                          u8ProductDescription[16]; /**<product Description*//**<CNcomment:产品描述符 */
}HI_UNF_HDMI_SPD_INFOFRAME_S;

/**HDMI Source HDMI MPEG InfoFrame parameter struct,please reference EIA-CEA-861-D*/
/**CNcomment: HDMI MPEG信息帧参数结构 , 请参考EIA-CEA-861-D */
typedef struct hiUNF_HDMI_MPEGSOURCE_INFOFRAME_S
{
    HI_U32                         u32MPEGBitRate;           /**<MPEG bit Rate*//**<CNcomment:MPEG位率 */
    HI_BOOL                        bIsFieldRepeated;         /**<FieldRepeater flag*//**<CNcomment:FieldRepeater标志位 */
}HI_UNF_HDMI_MPEGSOURCE_INFOFRAME_S;

/**HDMI Vendor Specific InfoFrame parameter struct,please reference EIA-CEA-861-D*/
/**CNcomment: HDMI VSIF信息帧 参数结构 , 请参考EIA-CEA-861-D */
typedef struct hiUNF_HDMI_VENDORSPEC_INFOFRAME_S
{
    HI_U32                         u32RegistrationId;       /**<Registration Id*//**<CNcomment:注册ID号 */
}HI_UNF_HDMI_VENDORSPEC_INFOFRAME_S;

/**HDMI InfoFrame unit struct*/
/**CNcomment: HDMI 信息帧联合定义体 */
typedef union hiUNF_HDMI_INFOFRAME_UNIT_U
{
    HI_UNF_HDMI_AVI_INFOFRAME_VER2_S   stAVIInfoFrame;      /**<AVI FrameInfo*//**<CNcomment:AVI信息帧 */
    HI_UNF_HDMI_AUD_INFOFRAME_VER1_S   stAUDInfoFrame;      /**<Audio FrameInfo*//**<CNcomment:AUD信息帧 */
    HI_UNF_HDMI_SPD_INFOFRAME_S        stSPDInfoFrame;      /**<SPD FrameInfo*//**<CNcomment:SPD信息帧 */
    HI_UNF_HDMI_MPEGSOURCE_INFOFRAME_S stMPEGSourceInfoFrame;/**<MPEGSource FrameInfo*//**<CNcomment:MPEGSource信息帧 */
    HI_UNF_HDMI_VENDORSPEC_INFOFRAME_S stVendorSpecInfoFrame;/**<VS FrameInfo*//**<CNcomment:VS信息帧 */
}HI_UNF_HMDI_INFORFRAME_UNIT_U;

/**HDMI InfoFrame struct */
/**CNcomment: HDMI 信息帧数据结构 */
typedef struct hiUNF_HDMI_INFOFRAME_S
{
    HI_UNF_HDMI_INFOFRAME_TYPE_E    enInfoFrameType;  /**<InfoFrame type*//**CNcomment:<InfoFrame类型 */
    HI_UNF_HMDI_INFORFRAME_UNIT_U   unInforUnit;      /**<InfoFrame unit data*//**CNcomment:<InfoFrame数据 */
}HI_UNF_HDMI_INFOFRAME_S;


/* CEC */
/** CEC interrelated Opcode:Please refer to CEC 15 Message Descriptions */
/** CNcomment: CEC 相关的操作码 */

/* General Protocol messages */

/**"Feature Abort" Used as a response to indicate that the device does not support the requested message type, or that it cannot execute it at the present time. */
/**CNcomment:"Feature Abort"消息表明设备不支持该信息的回复，或当前没法处理*/
#define CEC_OPCODE_FEATURE_ABORT                  0X00
/**"Abort" Message This message is reserved for testing purposes.*/
/**CNcomment:"Abort"消息是专为测试保留的*/
#define CEC_OPCODE_ABORT_MESSAGE                  0XFF

/* One Touch Play Feature*/

/**"Active Source" Used by a new source to indicate that it has started to transmit a stream OR used in response to a "Request Active Source"*/
/**CNcomment:"Active Source"消息说明源端设备正在发送一条码流或者回复"Request Active Source"消息*/
#define CEC_OPCODE_ACTIVE_SOURCE                  0X82
/**"Image View On" Sent by a source device to the TV whenever it enters the active state (alternatively it may send "Text View On").*/
/**CNcomment:当源端设备进入激活状态时，会给TV发送一次"Image View On"消息*/
#define CEC_OPCODE_IMAGE_VIEW_ON                  0X04
/**"Text View On" As "Image View On", but should also remove any text, menus and PIP windows from the TV's display.*/
/**CNcomment:"Text View On" 和"Image View On"指令相似，但同时会从TV画面上关闭文字，菜单和画中画窗口*/
#define CEC_OPCODE_TEXT_VIEW_ON                   0X0D

/* Routing Control Feature*/

/**"Inactive Source" Used by the currently active source to inform the TV that it has no video to be presented to the user, or is going into standby as the result of a local user command on the device. */
/**CNcomment: "Inactive Source"消息表明当前无码流播放或者因用户操作，设备要进入待机状态*/
#define CEC_OPCODE_INACTIVE_SOURCE                0X9D
/**"Request Active Source" Used by a new device to discover the status of the system.*/
/**CNcomment: "Request Active Source"消息用于通知系统添加了一个新设备*/
#define CEC_OPCODE_REQUEST_ACTIVE_SOURCE          0X85
/**"Routing Change" Sent by a CEC Switch when it is manually switched to inform all other devices on the network that the active route below the switch has changed. */
/**CNcomment: 当CEC转接器检测到子设备列表发生变化时，会通知所有活动子设备 "Routing Change"消息*/
#define CEC_OPCODE_ROUTING_CHANGE                 0X80
/**"Routing Information" Sent by a CEC Switch to indicate the active route below the switch.*/
/**CNcomment: CEC转接器发送"Routing Information"消息来检测子网络下活动子设备*/
#define CEC_OPCODE_ROUTING_INFORMATION            0X81
/**"Set Stream Path" Used by the TV to request a streaming path from the specified physical address.*/
/**CNcomment: TV从一个特定的设备上获取码流路径 */
#define CEC_OPCODE_SET_STREAM_PATH                0X86

/* Standby Feature*/

/**"Standby" Switches one or all devices into standby mode. Can be used as a broadcast message or be addressed to a specific device.See section CEC 13.3 for important notes on the use of this message */
/**CNcomment: "Standby"消息能使一个或多个设备进入待机状态。可用广播或者给特定的设备单独发送*/
#define CEC_OPCODE_STANDBY                        0X36

/* One Touch Record Feature*/

/**"Record Off" Requests a device to stop a recording. */
/**CNcomment: "Record Off"能使一个设备停止录像*/
#define CEC_OPCODE_RECORD_OFF                     0X0B
/**"Record On" Attempt to record the specified source. */
/**CNcomment: "Record On"消息用于尝试让一个特定的源录像*/
#define CEC_OPCODE_RECORD_ON                      0X09
/**"Record Status" Used by a Recording Device to inform the initiator of the message "Record On" about its status. */
/**CNcomment: 可录像设备发送"Record Status"消息和状态给发起"Record On"消息的的设备*/
#define CEC_OPCODE_RECORD_STATUS                  0X0A
/**"Record TV Screen" Request by the Recording Device to record the presently displayed source. */
/**CNcomment: "Record TV Screen"消息用于请求录像设备记录当前显示的资源*/
#define CEC_OPCODE_RECORD_TV_SCREEN               0X0F

/* Timer Programming Feature*/

/**"Clear Analogue Timer" Used to clear an Analogue timer block of a device. */
/**CNcomment: 清空模拟定时器设备*/
#define CEC_OPCODE_CLEAR_ANALOGUE_TIMER           0X33
/**"Clear Digital Timer" Used to clear a Digital timer block of a device. */
/**CNcomment: 清空数字定时器设备*/
#define CEC_OPCODE_CLEAR_DIGITAL_TIMER            0X99
/**"Clear External Timer" Used to clear an External timer block of a device. */
/**CNcomment: 清空外部定时器设备*/
#define CEC_OPCODE_CLEAR_EXTERNAL_TIMER           0XA1
/**"Set Analogue Timer" Used to set a single timer block on an Analogue Recording Device. */
/**CNcomment: 在模拟定时器设备上设置定时器*/
#define CEC_OPCODE_SET_ANALOGUE_TIMER             0X34
/**"Set Digital Timer" Used to set a single timer block on a Digital Recording Device. */
/**CNcomment: 在数字定时器设备上设置定时器*/
#define CEC_OPCODE_SET_DIGITAL_TIMER              0X97
/**"Set External Timer" Used to set a single timer block to record from an external device. */
/**CNcomment: 在外部定时器设备上设置定时器*/
#define CEC_OPCODE_SET_EXTERNAL_TIMER             0XA2
/**"Set Timer Program Title" Used to set the name of a program associated with a timer block. Sent directly after sending a "Set Analogue Timer" or "Set Digital Timer" message. The name is then associated with that timer block. */
/**CNcomment: 当发送完"Set Analogue Timer" 或"Set Digital Timer"消息后发送一个名称用于程序和定时器模块关联 */
#define CEC_OPCODE_SET_TIMER_PROGRAM_TITLE        0X67
/**"Timer Cleared Status" Used to give the status of a "Clear Analogue Timer", "Clear Digital Timer" or "Clear External Timer" message. */
/**CNcomment: "Timer Cleared Status"消息用于发送"Clear Analogue Timer"，"Clear Digital Timer"，"Clear External Timer"之后的状态*/
#define CEC_OPCODE_TIMER_CLEARED_STATUS           0X43
/**"Timer Status" Used to send timer status to the initiator of a "Set Timer" message. */
/**CNcomment: "Timer Status"消息用于给Set Timer"消息的发起者发送定时器状态*/
#define CEC_OPCODE_TIMER_STATUS                   0X35

/* System Information Feature*/

/**"CEC Version" Used to indicate the supported CEC version, in response to a "Get CEC Version" */
/**CNcomment: "CEC Version"消息用于发送设备上的CEC版本信息来回复"Get CEC Version"消息*/
#define CEC_OPCODE_CEC_VERSION                    0X9E
/**"Get CEC Version" Used by a device to enquire which version of CEC the target supports */
/**CNcomment: "Get CEC Version"用于一个设备获取从设备的CEC版本信息*/
#define CEC_OPCODE_GET_CEC_VERSION                0X9F
/**"Give Physical Address" A request to a device to return its physical address. */
/**CNcomment: 向一个设备请求获取该设备的物理地址*/
#define CEC_OPCODE_GIVE_PHYSICAL_ADDRESS          0X83
/**"Report Physical Address" Used to inform all other devices of the mapping between physical and logical address of the initiator.*/
/**CNcomment: 向同一网络下其他所有设备发送物理地址和逻辑地址*/
#define CEC_OPCODE_REPORT_PHYSICAL_ADDRESS        0X84
/**"Get Menu Language" Sent by a device capable of character generation (for OSD and Menus) to a TV in order to discover the currently selected Menu language.Also used by a TV during installation to discover the currently set menu language of other devices.*/
/**CNcomment: "Get Menu Language"用于获取Tv端能力集，用于替换当前的菜单语言类型；也可用于TV设备启动时，设置其它设备的菜单语言*/
#define CEC_OPCODE_GET_MENU_LANGUAGE              0X91
/**"Set Menu Language" Used by a TV or another device to indicate the menu language. */
/**CNcomment: "Set Menu Language"用于Tv或其他设备，设置菜单语言*/
#define CEC_OPCODE_SET_MENU_LANGUAGE              0X32

/*  Deck Control Feature*/

/**"Deck Control" Used to control a device's media functions. */
/**CNcomment: "Deck control"消息用于控制一个设备的多媒体功能*/
#define CEC_OPCODE_DECK_CONTROL                   0X42
/**"Deck Status" Used to provide a deck's status to the initiator of the "Give Deck Status" message. */
/**CNcomment: "Deck Status "消息用于回复"Give Deck Status"消息的发起者Deck的状态*/
#define CEC_OPCODE_DECK_STATUS                    0X1B
/**"Give Deck Status" Used to request the status of a device, regardless of whether or not it is the current active source. */
/**CNcomment: "Give Deck Status"消息请求获取目标设备的状态，而不管目标设备目前是不是激活的*/
#define CEC_OPCODE_GIVE_DECK_STATUS               0X1A
/**"Play" Used to control the playback behaviour of a source device. */
/**CNcomment: "Play"消息用于控制源设备播放*/
#define CEC_OPCODE_PLAY                           0X41

/* Tuner Control Feature*/

/**"Give Tuner Device Status" Used to request the status of a tuner device. */
/**CNcomment: "Give Tuner Device Status"用于获取电视调谐器的状态*/
#define CEC_OPCODE_GIVE_TUNER_DEVICE_STATUS       0X08
/**"Select Analogue Service" Directly selects an Analogue TV service */
/**CNcomment: "Select Analogue Service"消息用于直接选择一个模拟电视的服务项*/
#define CEC_OPCODE_SELECT_ANALOGUE_SERVICE        0X92
/**"Select Digital Service" Directly selects a Digital TV, Radio or Data Broadcast Service */
/**CNcomment: "Select Digital Service"消息用于直接选择一个数字电视的服务项*/
#define CEC_OPCODE_SELECT_DIGITAL_SERVICE         0X93
/**"Tuner Device Status" Use by a tuner device to provide its status to the initiator of the "Give Tuner Device Status" message. */
/**CNcomment: "Tuner Device Status"消息用于电视调谐器给" Give Tuner Device Status"消息的发起端回复*/
#define CEC_OPCODE_TUNER_DEVICE_STATUS            0X07
/**"Tuner Step Decrement" Used to tune to next lowest service in a tuner's service list. Can be used for PIP. */
/**CNcomment: "Tuner Step Decrement"消息用于把次低的服务项放入调谐器的服务列表，能用于画中画*/
#define CEC_OPCODE_TUNER_STEP_DECREMENT           0X06
/**"Tuner Step Increment" Used to tune to next highest service in a tuner's service list. Can be used for PIP. */
/**CNcomment: "Tuner Step Decrement"消息用于把次高的服务项放入调谐器的服务列表，能用于画中画*/
#define CEC_OPCODE_TUNER_STEP_INCREMENT           0X05

/* Vendor Specific Command*/

/**"Device Vendor ID" Reports the vendor ID of this device. */
/**CNcomment: "Device Vendor ID"消息用于报告此设备的供应商ID*/
#define CEC_OPCODE_DEVICE_VENDOR_ID               0X87
/**"Give Device Vendor ID" Requests the Vendor ID from a device. */
/**CNcomment: "Give Device Vendor ID"消息用于请求此设备的供应商ID*/
#define CEC_OPCODE_GIVE_DEVICE_VENDOR_ID          0X8C
/**"Vendor Command" Allows vendor specific commands to be sent between two devices. */
/**CNcomment: "Vendor Command"用于两个设备之间互相发送供应商定制消息*/
#define CEC_OPCODE_VENDOR_COMMAND                 0X89
/**"Vendor Command With ID" Allows vendor specific commands to be sent between two devices or broadcast. */
/**CNcomment: "Vendor Command With ID"用于两个设备之间或广播形式发送供应商定制消息*/
#define CEC_OPCODE_VENDOR_COMMAND_WITH_ID         0XA0
/**"Vendor Remote Button Down" Indicates that a remote control button has been depressed. */
/**CNcomment: "Vendor Remote Button Down"消息表示，遥控器有按键按下来*/
#define CEC_OPCODE_VENDOR_REMOTE_BUTTON_DOWN      0X8A
/**"Vendor Remote Button Up" Indicates that a remote control button (the last button pressed indicated by the Vendor Remote Button Down message) has been released. */
/**CNcomment: "Vendor Remote Button Up"消息表示 "Vendor Remote Button Down"消息最后一次表示过的按键被释放了*/
#define CEC_OPCODE_VENDOR_REMOTE_BUTTON_UP        0X8B

/* OSD Display Feature*/

/**"Set OSD String" Used to send a text message to output on a TV. */
/**CNcomment: "Set OSD String"消息可以发送一段文字消息让它显示在电视机上*/
#define CEC_OPCODE_SET_OSD_STRING                 0X64
/**"Give OSD Name" Used to request the preferred OSD name of a device for use in menus associated with that device. */
/**CNcomment:请求首选的关联设备菜单显示的名字 */
#define CEC_OPCODE_GIVE_OSD_NAME                  0X46
/**"Set OSD Name" Used to set the preferred OSD name of a device for use in menus associated with that device. */
/**CNcomment:设置首选关联设备菜单显示的名字 */
#define CEC_OPCODE_SET_OSD_NAME                   0X47

/* Device Menu Control Feature*/

/**"Menu Request" A request from the TV for a device to show/remove a menu or to query if a device is currently showing a menu. */
/**CNcomment: "Menu Request"消息用于电视请求一个设备显示/关闭菜单或询问该设备当前有没有菜单显示*/
#define CEC_OPCODE_MENU_REQUEST                   0X8D
/**"Menu Status" Used to indicate to the TV that the device is showing/has removed a menu and requests the remote control keys to be passed though. */
/**CNcomment: "Menu Status"消息用于回复电视端当前正显示/移除了菜单，并透传遥控器指令*/
#define CEC_OPCODE_MENU_STATUS                    0X8E
/**"User Control Pressed" Used to indicate that the user pressed a remote control button or switched from one remote control button to another. */
/**CNcomment: "User Control Pressed"表示用户按了一个遥控器按键或从某一个按键换到另一个按键.也可以用于用户间接发起的指令*/
#define CEC_OPCODE_USER_CONTROL_PRESSED           0X44
/**"User Control Released" Indicates that user released a remote control button (the last one indicated by the "User Control Pressed" message) */
/**CNcomment: "User Control Released"表示用户释放了"User Control Released"消息指定的遥控器按键.也可以用于用户间接发起的指令*/
#define CEC_OPCODE_USER_CONTROL_RELEASED          0X45

/* Power Status Feature*/

/**"Give Device Power Status" Used to determine the current power status of a target device */
/**CNcomment: "Give Device Power Status"消息用于获取目标设备当前的电源状态*/
#define CEC_OPCODE_GIVE_DEVICE_POWER_STATUS       0X8F
/**"Report Power Status" Used to inform a requesting device of the current power status */
/**CNcomment: "Report Power Status"消息用于发送当前的电源状态回复请求设备*/
#define CEC_OPCODE_REPORT_POWER_STATUS            0X90

/* System Audio Control Feature*/

/**"Give Audio Status" Requests an amplifier to send its volume and mute status */
/**CNcomment: "Give Audio Status"消息请求获取扩音器的音量和静音状态*/
#define CEC_OPCODE_GIVE_AUDIO_STATUS              0X71
/**"Give System Audio Mode Status" Requests the status of the System Audio Mode */
/**CNcomment: "Give System Audio Mode Status"消息请求系统音频的状态*/
#define CEC_OPCODE_GIVE_SYSTEM_AUDIO_MODE_STATUS  0x7D
/**"Report Audio Status" Reports an amplifier's volume and mute status */
/**CNcomment: "Report Audio Status"消息用于发送扩音器的音量和静音状态*/
#define CEC_OPCODE_REPORT_AUDIO_STATUS            0X7A
/**"Set System Audio Mode" Turns the System Audio Mode On or Off. */
/**CNcomment: "Set System Audio Mode"消息用于打开/关闭系统音频功能*/
#define CEC_OPCODE_SET_SYSTEM_AUDIO_MODE          0X72
/**"System Audio Mode Request" A device implementing System Audio Control and which has volume control RC buttons (eg TV or STB) requests to use System Audio Mode to the amplifier */
/**CNcomment: 使用音量控制按钮(stb或者tv)需要扬声器进入 System Audio Mode*/
#define CEC_OPCODE_SYSTEM_AUDIO_MODE_REQUEST      0X70
/**"System Audio Mode Status" Reports the current status of the System Audio Mode */
/**CNcomment: "System Audio Mode Status"消息用于发送系统音频当前的状态*/
#define CEC_OPCODE_SYSTEM_AUDIO_MODE_STATUS       0X7E

/* Audio Rate Control Feature*/

/**"Set Audio Rate" Used to control audio rate from Source Device. */
/**CNcomment: "Set Audio Rate"消息用于控制源端设备的音频采样率*/
#define CEC_OPCODE_SET_AUDIO_RATE                 0X9A

/**POLL message have no opcode, So, we just use this value */
/**CNcomment: "POLL"消息没有操作码，我们仅仅只是使用该值*/
#define CEC_OPCODE_POLLING_MESSAGE                0XFE

/**HDMI CEC logical address,please reference  HDMI specification 1.4a*/
/**CNcomment: HDMI CEC逻辑地址，请参考HDMI 1.4a协议 */
typedef enum hiUNF_CEC_LOGICALADD_S
{
    HI_UNF_CEC_LOGICALADD_TV               = 0X00, /**<TV*//**CNcomment:<电视 */
    HI_UNF_CEC_LOGICALADD_RECORDDEV_1      = 0X01, /**<Record device 1*//**CNcomment:<录像机 1 */
    HI_UNF_CEC_LOGICALADD_RECORDDEV_2      = 0X02, /**<Record device 2*//**CNcomment:<录像机 2 */
    HI_UNF_CEC_LOGICALADD_TUNER_1          = 0X03, /**<Tuner 1*//**CNcomment:<高频头 1*/
    HI_UNF_CEC_LOGICALADD_PLAYDEV_1        = 0X04, /**<play device 1*//**CNcomment:<播放设备 1 */
    HI_UNF_CEC_LOGICALADD_AUDIOSYSTEM      = 0X05, /**<audio system*//**CNcomment:<音频系统 */
    HI_UNF_CEC_LOGICALADD_TUNER_2          = 0X06, /**<tuner 2*//**CNcomment:<高频头 2 */
    HI_UNF_CEC_LOGICALADD_TUNER_3          = 0X07, /**<tuner 3*//**CNcomment:<高频头 3 */
    HI_UNF_CEC_LOGICALADD_PLAYDEV_2        = 0X08, /**<play device 2*//**CNcomment:<播放设备 2 */
    HI_UNF_CEC_LOGICALADD_RECORDDEV_3      = 0X09, /**<Record device 3*//**CNcomment:<录像机 3 */
    HI_UNF_CEC_LOGICALADD_TUNER_4          = 0X0A, /**<tuner 4*//**CNcomment:<高频头 4 */
    HI_UNF_CEC_LOGICALADD_PLAYDEV_3        = 0X0B, /**<play device 3*//**CNcomment:<播放设备 3 */
    HI_UNF_CEC_LOGICALADD_RESERVED_1       = 0X0C, /**<reserved 1*//**CNcomment:<保留项 1 */
    HI_UNF_CEC_LOGICALADD_RESERVED_2       = 0X0D, /**<reserved 2*//**CNcomment:<保留项 2 */
    HI_UNF_CEC_LOGICALADD_SPECIALUSE       = 0X0E, /**<special use*//**CNcomment:<特殊用途 */
    HI_UNF_CEC_LOGICALADD_BROADCAST        = 0X0F, /**<broadcast*//**CNcomment:<广播 */
    HI_UNF_CEC_LOGICALADD_BUTT
}HI_UNF_CEC_LOGICALADD_S;

/**HDMI CEC command type,please reference  HDMI specification 1.4a*/
/**CNcomment: HDMI CEC命令类型，请参考HDMI 1.4a协议 */
typedef enum hiUNF_CEC_CMDTYPE_E
{
    HI_UNF_CEC_STRUCTCOMMAND,                    /**<CEC struct command*//**<CNcomment:CEC 结构命令 */
    HI_UNF_CEC_RAWCOMMAND,                       /**<CEC raw command*//**<CNcomment:CEC 原始命令 */
    HI_UNF_CEC_BUTT
}HI_UNF_CEC_CMDTYPE_E;

/**HDMI CEC Raw Data struct,please reference  HDMI specification 1.4a*/
/**CNcomment: HDMI CEC原始数据结构体，请参考HDMI 1.4a协议 */
typedef struct hiUNF_CEC_RAWDATA_S
{
    HI_U8                              u8Length;  /**<CEC raw data lengh*//**<CNcomment:cec 有效参数个数 */
    HI_U8                              u8Data[15];     /**<CEC raw data*//**<CNcomment:CEC 参数结构体 */
}HI_UNF_CEC_RAWDATA_S;

/**HDMI CEC user Interface Command Opcode,please reference  HDMI specification 1.4a*/
/**CNcomment: HDMI CEC 用户接口操作指令，请参考HDMI 1.4a协议 */
typedef enum hiUNF_CEC_UICMD_E
{
    HI_UNF_CEC_UICMD_SELECT                      = 0x00,
    HI_UNF_CEC_UICMD_UP                          = 0x01,
    HI_UNF_CEC_UICMD_DOWN                        = 0x02,
    HI_UNF_CEC_UICMD_LEFT                        = 0x03,
    HI_UNF_CEC_UICMD_RIGHT                       = 0x04,
    HI_UNF_CEC_UICMD_RIGHT_UP                    = 0x05,
    HI_UNF_CEC_UICMD_RIGHT_DOWN                  = 0x06,
    HI_UNF_CEC_UICMD_LEFT_UP                     = 0x07,
    HI_UNF_CEC_UICMD_LEFT_DOWN                   = 0x08,
    HI_UNF_CEC_UICMD_ROOT_MENU                   = 0x09,
    HI_UNF_CEC_UICMD_SETUP_MENU                  = 0x0A,
    HI_UNF_CEC_UICMD_CONTENTS_MENU               = 0x0B,
    HI_UNF_CEC_UICMD_FAVORITE_MENU               = 0x0C,
    HI_UNF_CEC_UICMD_EXIT                        = 0x0D,
    HI_UNF_CEC_UICMD_NUM_0                       = 0x20,
    HI_UNF_CEC_UICMD_NUM_1                       = 0x21,
    HI_UNF_CEC_UICMD_NUM_2                       = 0x22,
    HI_UNF_CEC_UICMD_NUM_3                       = 0x23,
    HI_UNF_CEC_UICMD_NUM_4                       = 0x24,
    HI_UNF_CEC_UICMD_NUM_5                       = 0x25,
    HI_UNF_CEC_UICMD_NUM_6                       = 0x26,
    HI_UNF_CEC_UICMD_NUM_7                       = 0x27,
    HI_UNF_CEC_UICMD_NUM_8                       = 0x28,
    HI_UNF_CEC_UICMD_NUM_9                       = 0x29,
    HI_UNF_CEC_UICMD_DOT                         = 0x2A,
    HI_UNF_CEC_UICMD_ENTER                       = 0x2B,
    HI_UNF_CEC_UICMD_CLEAR                       = 0x2C,
    HI_UNF_CEC_UICMD_NEXT_FAVORITE               = 0x2F,
    HI_UNF_CEC_UICMD_CHANNEL_UP                  = 0x30,
    HI_UNF_CEC_UICMD_CHANNEL_DOWN                = 0x31,
    HI_UNF_CEC_UICMD_PREVIOUS_CHANNEL            = 0x32,
    HI_UNF_CEC_UICMD_SOUND_SELECT                = 0x33,
    HI_UNF_CEC_UICMD_INPUT_SELECT                = 0x34,
    HI_UNF_CEC_UICMD_DISPLAY_INFORMATION         = 0x35,
    HI_UNF_CEC_UICMD_HELP                        = 0x36,
    HI_UNF_CEC_UICMD_PAGE_UP                     = 0x37,
    HI_UNF_CEC_UICMD_PAGE_DOWN                   = 0x38,
    HI_UNF_CEC_UICMD_POWER                       = 0x40,
    HI_UNF_CEC_UICMD_VOLUME_UP                   = 0x41,
    HI_UNF_CEC_UICMD_VOLUME_DOWN                 = 0x42,
    HI_UNF_CEC_UICMD_MUTE                        = 0x43,
    HI_UNF_CEC_UICMD_PLAY                        = 0x44,
    HI_UNF_CEC_UICMD_STOP                        = 0x45,
    HI_UNF_CEC_UICMD_PAUSE                       = 0x46,
    HI_UNF_CEC_UICMD_RECORD                      = 0x47,
    HI_UNF_CEC_UICMD_REWIND                      = 0x48,
    HI_UNF_CEC_UICMD_FAST_FORWARD                = 0x49,
    HI_UNF_CEC_UICMD_EJECT                       = 0x4A,
    HI_UNF_CEC_UICMD_FORWARD                     = 0x4B,
    HI_UNF_CEC_UICMD_BACKWARD                    = 0x4C,
    HI_UNF_CEC_UICMD_STOP_RECORD                 = 0x4D,
    HI_UNF_CEC_UICMD_PAUSE_RECORD                = 0x4E,
    HI_UNF_CEC_UICMD_ANGLE                       = 0x50,
    HI_UNF_CEC_UICMD_SUBPICTURE                  = 0x51,
    HI_UNF_CEC_UICMD_VIDEO_ON_DEMAND             = 0x52,
    HI_UNF_CEC_UICMD_ELECTRONIC_PROGRAM_GUIDE    = 0x53,
    HI_UNF_CEC_UICMD_TIMER_PROGRAMMING           = 0x54,
    HI_UNF_CEC_UICMD_INITIAL_CONFIGURATION       = 0x55,
    HI_UNF_CEC_UICMD_PLAY_FUNCTION               = 0x60,
    HI_UNF_CEC_UICMD_PAUSE_PLAY_FUNCTION         = 0x61,
    HI_UNF_CEC_UICMD_RECORD_FUNCTION             = 0x62,
    HI_UNF_CEC_UICMD_PAUSE_RECORD_FUNCTION       = 0x63,
    HI_UNF_CEC_UICMD_STOP_FUNCTION               = 0x64,
    HI_UNF_CEC_UICMD_MUTE_FUNCTION               = 0x65,
    HI_UNF_CEC_UICMD_RESTORE_VOLUME_FUNCTION     = 0x66,
    HI_UNF_CEC_UICMD_TUNE_FUNCTION               = 0x67,
    HI_UNF_CEC_UICMD_SELECT_MEDIA_FUNCTION       = 0x68,
    HI_UNF_CEC_UICMD_SELECT_AV_INPUT_FUNCTION    = 0x69,
    HI_UNF_CEC_UICMD_SELECT_AUDIO_INPUT_FUNCTION = 0x6A,
    HI_UNF_CEC_UICMD_POWER_TOGGLE_FUNCTION       = 0x6B,
    HI_UNF_CEC_UICMD_POWER_OFF_FUNCTION          = 0x6C,
    HI_UNF_CEC_UICMD_POWER_ON_FUNCTION           = 0x6D,
    HI_UNF_CEC_UICMD_F1_BLUE                     = 0x71,
    HI_UNF_CEC_UICMD_F2_RED                      = 0x72,
    HI_UNF_CEC_UICMD_F3_GREEN                    = 0x73,
    HI_UNF_CEC_UICMD_F4_YELLOW                   = 0x74,
    HI_UNF_CEC_UICMD_F5                          = 0x75,
    HI_UNF_CEC_UICMD_DATA                        = 0x76
}HI_UNF_CEC_UICMD_E;

/**HDMI CEC operand command,please reference  HDMI specification 1.4a*/
/**CNcomment: HDMI CEC 操作指令，请参考HDMI 1.4a协议 */
typedef union hiUNF_CEC_Operand_t
{
    HI_UNF_CEC_RAWDATA_S               stRawData;   /**<CEC raw date*//**<CNcomment:CEC 原始命令 */
    HI_UNF_CEC_UICMD_E                 stUIOpcode;  /**<CEC user interface command*//**<CNcomment:CEC用户自定义操作 */
}HI_UNF_CEC_Operand_t;

/**HDMI CEC struct command*/
/**CNcomment: HDMI CEC 命令结构 */
typedef struct hiUNF_HDMI_CEC_CMD_S
{
    HI_UNF_CEC_LOGICALADD_S  enSrcAdd;     /**<logical address of source *//**<CNcomment:源端设备地址 */
    HI_UNF_CEC_LOGICALADD_S  enDstAdd;     /**<logical address of destination*//**<CNcomment:目标设备地址 */
    HI_U8                    u8Opcode;     /**<opration code*//**<CNcomment:操作码*/
    HI_UNF_CEC_Operand_t     unOperand;    /**<operand*//**<CNcomment:操作数*/
}HI_UNF_HDMI_CEC_CMD_S;

/**HDMI CEC status struct*/
/**CNcomment: HDMI CEC 状态结构 */
typedef struct hiUNF_HDMI_CEC_STATUS_S
{
    HI_BOOL bEnable;                                 /**<the flag of CEC work,HI_TRUE:CEC work enable,HI_FALSE:CEC no work ,other parameter no effect*/ /**<CNcomment:CEC 正常工作标记位，如果为HI_TRUE,CEC可以正常工作，如果为HI_FASLE,CEC的其他参数无效，并且不能工作 */
    HI_U8   u8PhysicalAddr[4];                       /**<CEC physics address*/ /**<CNcomment:CEC 物理地址 */
    HI_U8   u8LogicalAddr;                           /**<CEC logic address,defualt 0x03*//**<CNcomment:CEC 逻辑地址，默认为：0x03. */
    HI_U8   u8Network[HI_UNF_CEC_LOGICALADD_BUTT];   /**<CEC network struct ,1:the device can response CEC command*/ /**<CNcomment:CEC 构建的网路结构，为1表示该设备能够响应CEC命令 */
}HI_UNF_HDMI_CEC_STATUS_S;

/**HDMI CEC Regcallback param struct */
/**CNcomment: HDMI CEC 回调函数参数结构 */
typedef HI_VOID (*HI_UNF_HDMI_CECCALLBACK)(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_CEC_CMD_S *pstCECCmd, HI_VOID *pData);


/**HDMI HDCP key struct*/
/**CNcomment: HDMI HDCP 密钥结构 */
typedef struct hiUNF_HDMI_LOAD_KEY_S
{
    HI_U8 *pu8InputEncryptedKey;            /**<Encrypted key pointer *//**<CNcomment:加密的密钥数组头地址 */
    HI_U32 u32KeyLength;  					/**<Encrypted key length*//**<CNcomment:加密的密钥长度 */
}HI_UNF_HDMI_LOAD_KEY_S;

/**HDMI Delay struct*/
/**CNcomment: HDMI 延时结构 */
typedef struct hiUNF_HDMI_DELAY_S
{
    HI_U32  u32MuteDelay;           /**<delay for avmute *//**<CNcomment:avmute的延时 */
    HI_U32  u32FmtDelay;  			/**<delay for setformat *//**<CNcomment:设置制式的延时 */
    HI_BOOL bForceFmtDelay;         /**<force setformat delay mode *//**<CNcomment:切换制式强制延时模式 */
    HI_BOOL bForceMuteDelay;        /**<force avmute delay mode *//**<CNcomment:mute强制延时模式 */
}HI_UNF_HDMI_DELAY_S;


/** @} */  /** <!-- ==== Structure Definition end ==== */

/******************************* API declaration *****************************/
/** \addtogroup      HDMI */
/** @{ */  /** <!-- [HDMI] */

/**
\brief the whole initialization of the hdmi. CNcomment:HDMI接口驱动软件初始化 CNend
\attention  this func should be called before vo_init and after disp_init. CNcomment:必须在DISP驱动Setup之后和VO驱动Setup之前打开 CNend
\param CNcomment:无 CNend
\retval HI_SUCCESS  success. CNcomment:成功 CNend
\retval please refer to the err code definitino of mpi.CNcomment:请参考MPI错误码 CNend
\see \n
CNcomment:无 CNend
*/
HI_S32 HI_UNF_HDMI_Init(HI_VOID);


/**
\brief deinit the hdmi. CNcomment:HDMI接口去初始化 CNend
\attention  this must be called after vo exited and before disp  exited . CNcomment:必须在VO驱动Exit之后和DISP驱动Exit之前打开 CNend
\param CNcomment:无 CNend
\retval HI_SUCCESS      success.CNcomment:成功 CNend
\retval please refer to the err code definitino of mpi.CNcomment:请参考MPI错误码 CNend
\see \n
CNcomment:无 CNend
*/
HI_S32 HI_UNF_HDMI_DeInit(HI_VOID);


/**
\brief create hdmi device. CNcomment:创建HDMI设备 CNend
\attention \n
\param[in] enHdmi  hdmi device id. CNcomment:要打开的HDMI设备 CNend
\param[in] pstOpenPara  When get GetSinkCapability failed,defalut set sink device to DVI/HDMI mode.CNcomment:未获取到对端能力集时，默认把对端当成(DVI/HDMI)设备 CNend
\retval HI_SUCCESS     success.  CNcomment:成功 CNend
\retval please refer to the err code definitino of mpi.CNcomment:请参考MPI错误码 CNend
\see \n
CNcomment:无 CNend
*/
HI_S32 HI_UNF_HDMI_Open(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_OPEN_PARA_S *pstOpenPara);

/**
\brief close the handler created by  HI_UNF_HDMI_Open. CNcomment:销毁由HI_UNF_HDMI_Open创建的句柄 CNend
\attention \n
\param[in] enHdmi  hdmi device id. CNcomment:HDMI设备ID CNend
\retval HI_SUCCESS     success.  CNcomment:成功 CNend
\retval please refer to the err code definitino of mpi.CNcomment:请参考MPI错误码 CNend
\see \n
CNcomment:无 CNend
*/
HI_S32 HI_UNF_HDMI_Close(HI_UNF_HDMI_ID_E enHdmi);

/**
\brief get current hdmi status. CNcomment:获取hdmi当前的状态 CNend
\attention \n
\param[in] enHdmi  hdmi device id. CNcomment:HDMI设备ID CNend
\param[out] pHdmiStatus hdmi cuttent status. CNcomment:HDMI当前状态 CNend
\retval HI_SUCCESS     success.  CNcomment:成功 CNend
\retval please refer to the err code definitino of mpi.CNcomment:请参考MPI错误码 CNend
\see \n
CNcomment:无 CNend
*/
HI_S32 HI_UNF_HDMI_GetStatus(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_STATUS_S *pHdmiStatus);

/**
\brief to get the capability of sink connect to stbbox. CNcomment:查询获取HDMI Sink设备的能力集 CNend
\attention \n
\param[in] enHdmi   hdmi device id.CNcomment:HDMI设备ID CNend
\param[out] pCapability  the capability of the sink .CNcomment:SINK能力集 CNend
\retval HI_SUCCESS       success.  CNcomment:成功 CNend
\retval please refer to the err code definitino of mpi. CNcomment:请参考MPI错误码 CNend
\see ::HI_UNF_EDID_BASE_INFO_S\n
CNcomment:无 CNend
*/
HI_S32 HI_UNF_HDMI_GetSinkCapability(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_EDID_BASE_INFO_S *pCapability);

/**
\brief set the attr of given hdmi interface. CNcomment:设置HDMI接口属性。 CNend
\attention \n
\param[in] enHdmi      hdmi device id.CNcomment:HDMI设备ID CNend
\param[in] pstAttr     the attr of given hdmi interface.CNcomment:HDMI接口属性 CNend
\retval HI_SUCCESS     success.  CNcomment:成功 CNend
\retval please refer to the err code definitino of mpi.CNcomment:请参考MPI错误码 CNend
\see ::HI_UNF_HDMI_ATTR_S\n
CNcomment:无 CNend
*/
HI_S32 HI_UNF_HDMI_SetAttr(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_ATTR_S *pstAttr);


/**
\brief get the current attr of the give hdmi interface. CNcomment:查询HDMI接口当前属性 CNend
\attention \n
\param[in] enHdmi hdmi device id.CNcomment: HDMI设备ID CNend
\param[out] pstAttr     the attr of given hdmi interface. CNcomment:HDMI接口属性 CNend
\retval HI_SUCCESS      success. CNcomment:成功 CNend
\retval please refer to the err code definitino of mpi.CNcomment:请参考MPI错误码 CNend
\see HI_UNF_HDMI_ATTR_S\n
CNcomment:无 CNend
*/
HI_S32 HI_UNF_HDMI_GetAttr(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_ATTR_S *pstAttr);


/**
\brief get the cec working status. CNcomment:获取CEC状态 CNend
\attention  user can get the cec working status through this func. CNcomment:客户可以通过该接口获取CEC工作状态\n CNend
\param[in] enHdmi       hdmi device id.CNcomment:HDMI设备ID CNend
\param[in] pStatus     get cec working status. CNcomment:CEC 状态 CNend
\retval HI_SUCCESS     success. CNcomment:成功 CNend
\retval please refer to the err code definitino of mpi.CNcomment:请参考MPI错误码 CNend
\see ::HI_UNF_HDMI_CEC_CMD_S\n
CNcomment:无 CNend
*/
HI_S32 HI_UNF_HDMI_CECStatus(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_CEC_STATUS_S  *pStatus);


/**
\brief send the cec data. CNcomment:发送CEC 命令 CNend
\attention \n
\param[in] enHdmi   hdmi device id. CNcomment:HDMI设备ID CNend
\param[in] pCECCmd      the cec cmd data.CNcomment:Cec Command 内容 CNend
\retval HI_SUCCESS     success. CNcomment:成功 CNend
\retval please refer to the err code definitino of mpi.CNcomment:请参考MPI错误码 CNend
\see ::HI_UNF_HDMI_CEC_CMD_S\n
CNcomment:无 CNend
*/
HI_S32 HI_UNF_HDMI_SetCECCommand(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_CEC_CMD_S  *pCECCmd);


/**
\brief get the cec data received. CNcomment:接收的CEC 命令 CNend
\attention \n
\param[in] enHdmi   hdmi device id. CNcomment:HDMI设备ID CNend
\param[in] pCECCmd    the cec cmd data. CNcomment:Cec Command 内容 CNend
\param[in] timeout    timeout for getting cec cmd, unit: 10ms;
\retval HI_SUCCESS      success. CNcomment:成功 CNend
\retval please refer to the err code definitino of mpi.CNcomment:请参考MPI错误码 CNend
\see ::HI_UNF_HDMI_CEC_CMD_S\n
CNcomment:无 CNend
*/
HI_S32 HI_UNF_HDMI_GetCECCommand(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_CEC_CMD_S  *pCECCmd, HI_U32 timeout);

/**
\brief register CEC callback function. CNcomment:注册获取CEC回调函数 CNend
\attention \n
\param[in] enHdmi   hdmi device id. CNcomment:HDMI设备ID CNend
\param[in] pCECCallback    the cec callback handle. CNcomment:回调函数句柄 CNend
\retval HI_SUCCESS      success. CNcomment:成功 CNend
\retval please refer to the err code definitino of mpi.CNcomment:请参考MPI错误码 CNend
\see ::HI_UNF_HDMI_CEC_CMD_S\n
CNcomment:无 CNend
*/
HI_S32 HI_UNF_HDMI_RegCECCallBackFunc(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_CECCALLBACK pCECCallback);

/**
\brief unregister CEC callback function. CNcomment:反注册获取CEC回调函数 CNend
\attention \n
\param[in] enHdmi   hdmi device id. CNcomment:HDMI设备ID CNend
\param[in] pCECCallback  the cec callback handle. CNcomment:回调函数句柄 CNend
\retval HI_SUCCESS      success. CNcomment:成功 CNend
\retval please refer to the err code definitino of mpi.CNcomment:请参考MPI错误码 CNend
\see ::HI_UNF_HDMI_CEC_CMD_S\n
CNcomment:无 CNend
*/
HI_S32 HI_UNF_HDMI_UnRegCECCallBackFunc(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_CECCALLBACK pCECCallback);

/**
\brief enable the cec func. CNcomment:CEC 使能打开 CNend
\attention \n
\param[in] enHdmi   hdmi device id. CNcomment:HDMI设备ID CNend
\retval HI_SUCCESS     success. CNcomment:成功 CNend
\retval please refer to the err code definitino of mpi.CNcomment:请参考MPI错误码 CNend
\see \n
CNcomment:无 CNend
*/
HI_S32 HI_UNF_HDMI_CEC_Enable(HI_UNF_HDMI_ID_E enHdmi);

/**
\brief disable the cec func. CNcomment:CEC 使能关闭 CNend
\attention \n
\param[in] enHdmi   hdmi device id. CNcomment:HDMI设备ID CNend
\retval HI_SUCCESS      success. CNcomment:成功 CNend
\retval please refer to the err code definitino of mpi.CNcomment:请参考MPI错误码 CNend
\see \n
CNcomment:无 CNend
*/
HI_S32 HI_UNF_HDMI_CEC_Disable(HI_UNF_HDMI_ID_E enHdmi);

/**
\brief  set and send infoframe. CNcomment:设置并发送InfoFrame CNend
\attention \n
\param[in] enHdmi       hdmi device id.CNcomment:HDMI设备ID CNend
\param[in] pstInfoFrame the inforframe content.CNcomment:InfoFrame内容 CNend
\retval HI_SUCCESS      success. CNcomment:成功 CNend
\retval please refer to the err code definitino of mpi.CNcomment:请参考MPI错误码 CNend
\see \n
CNcomment:无 CNend
*/
HI_S32 HI_UNF_HDMI_SetInfoFrame(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_INFOFRAME_S *pstInfoFrame);

/**
\brief get the infoframe infor. CNcomment:获取正在发送的InfoFrame的信息 CNend
\attention \n
\param[in] enHdmi       hdmi device id. CNcomment:HDMI设备ID CNend
\param[in] enInfoFrameType the info frame type such as avi or audio or gcp etc. CNcomment:InfoFrame类型 CNend
\param[out] pstInfoFrame   the inforframe content.CNcomment:InfoFrame内容 CNend
\retval HI_SUCCESS      success.CNcomment:成功 CNend
\retval please refer to the err code definitino of mpi.CNcomment:请参考MPI错误码 CNend
\see \n
CNcomment:无 CNend
*/
HI_S32 HI_UNF_HDMI_GetInfoFrame(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_INFOFRAME_TYPE_E enInfoFrameType, HI_UNF_HDMI_INFOFRAME_S *pstInfoFrame);



/**
\brief  start the hdmi works. CNcomment:HDMI开始 CNend
\attention \n
this should be called after HI_UNF_HDMI_SetAttr.
CNcomment:启动HDMI接口。一般在HI_UNF_HDMI_SetAttr之后调用 CNend
\param[in] enHdmi   hdmi device id.CNcomment:HDMI设备ID CNend
\retval HI_SUCCESS     success. CNcomment:成功 CNend
\retval please refer to the err code definitino of mpi.CNcomment:请参考MPI错误码 CNend
\see \n
CNcomment:无 CNend
*/
HI_S32 HI_UNF_HDMI_Start(HI_UNF_HDMI_ID_E enHdmi);

/**
\brief stop  the hdmi. CNcomment:hdmi停止 CNend
\attention \n
this func should be called  when hdmi plug out.
CNcomment:当HDMI线被拔除后，HDMI回调函数应该调用该函数。 CNend
\param[in] enHdmi  hdmi device id. CNcomment:HDMI设备ID CNend
\retval HI_SUCCESS     success. CNcomment:成功 CNend
\retval please refer to the err code definitino of mpi.CNcomment:请参考MPI错误码 CNend
\see \n
CNcomment:无 CNend
*/
HI_S32 HI_UNF_HDMI_Stop(HI_UNF_HDMI_ID_E enHdmi);

/**
\brief set the deep color mode. CNcomment:设置HDMI DeepColor模式 CNend
\attention \n
\param[in] enHdmi   hdmi device id. CNcomment:HDMI设备ID CNend
\param[in] enDeepColor deep color mode,please refer to the HI_UNF_HDMI_DEEP_COLOR_E definiton.CNcomment:DeepColor模式，请参考::HI_UNF_HDMI_DEEP_COLOR_E  CNend
\retval HI_SUCCESS     success. CNcomment:成功 CNend
\retval please refer to the err code definitino of mpi.CNcomment:请参考MPI错误码 CNend
\see \n
CNcomment:无 CNend
*/
HI_S32 HI_UNF_HDMI_SetDeepColor(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_DEEP_COLOR_E enDeepColor);

/**
\brief switch the xvycc on or off. CNcomment:设置HDMI xvYCC 模式 CNend
\attention \n
\param[in] enHdmi   hdmi device id.CNcomment:HDMI设备ID CNend
\param[in] bEnalbe   whether to enable xvycc mode or not .CNcomment:是否使能xvYCC模式 CNend
\retval HI_SUCCESS    success.  CNcomment:成功 CNend
\retval please refer to the err code definitino of mpi.CNcomment:请参考MPI错误码 CNend
\see \n
CNcomment:无 CNend
*/
HI_S32 HI_UNF_HDMI_SetxvYCCMode(HI_UNF_HDMI_ID_E enHdmi, HI_BOOL bEnalbe);

/**
\brief switch the mute of av  on or off. CNcomment:设置HDMI AV mute 模式 CNend
\attention \n
\param[in] enHdmi   hdmi device id.CNcomment:HDMI设备ID CNend
\param[in] bAvMute   whether to mute the av.CNcomment:是否mute AV CNend
\retval HI_SUCCESS     success. CNcomment:成功 CNend
\retval please refer to the err code definitino of mpi.CNcomment:请参考MPI错误码 CNend
\see \n
CNcomment:无 CNend
*/
HI_S32 HI_UNF_HDMI_SetAVMute(HI_UNF_HDMI_ID_E enHdmi, HI_BOOL bAvMute);

/**
\brief get the edid information forcelly. CNcomment:强制获取EDID原始数据,该接口为特定平台需要获取原始EDID的接口 CNend
\attention \n
\param[in] enHdmi   hdmi device id.CNcomment:HDMI设备ID CNend
\param[in] *u8Edid  the buffer allocated externally, buffer size must be 512. CNcomment:获取原始EDID数据buffer。EDID版本不同，读出来的数据不同，取上限buffer 大小为512,用户需自己分配好; CNend
\param[in] *u32EdidLength  the data lenth of  original edid. CNcomment:获取原始EDID数据长度 CNend
\retval HI_SUCCESS   success.   CNcomment:成功 CNend
\retval please refer to the err code definitino of mpi.CNcomment:请参考MPI错误码 CNend
\see \n
CNcomment:无 CNend
*/
HI_S32 HI_UNF_HDMI_Force_GetEDID(HI_UNF_HDMI_ID_E enHdmi, HI_U8 *u8Edid, HI_U32 *u32EdidLength);


/**
\brief register callback function. CNcomment:注册回调函数 CNend
\attention \n
this func should be called before HI_UNF_HDMI_Open and after HI_UNF_HDMI_Init
because HI_UNF_HDMI_Open will trigger hotplug event.
And this function can only register one function
If call this func two times,then the 2nd callback function will cover 1st one.
CNcomment:建议在HI_UNF_HDMI_Init之后和HI_UNF_HDMI_Open之前调用 \n
因为Open时如果连着接收端设备上会触发一次hotplug消息 \n
该函数只能注册一个回调函数，第二次注册的回调函数会覆盖前面的回调函数 CNend
\param CNcomment:无 CNend
\param[in] enHdmi   hdmi device id.CNcomment:HDMI设备ID CNend
\param[in] *HI_UNF_HDMI_CALLBACK_FUNC_S  callback function CNcomment:回调函数 CNend
\retval HI_SUCCESS   success.   CNcomment:成功 CNend
\retval please refer to the err code definitino of mpi.CNcomment:请参考MPI错误码 CNend
\see \n
CNcomment:无 CNend
*/
HI_S32 HI_UNF_HDMI_RegCallbackFunc(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_CALLBACK_FUNC_S *pstCallbackFunc);


/**
\brief unregister CallbackFunc. CNcomment:注销回调函数 CNend
\attention \n
this func should be called before HI_UNF_HDMI_DeInit and after HI_UNF_HDMI_Close
CNcomment:建议在HI_UNF_HDMI_Close之后和HI_UNF_HDMI_DeInit之前调用 CNend
\param[in] enHdmi   hdmi device id.CNcomment:HDMI设备ID CNend
\param[in] *HI_UNF_HDMI_CALLBACK_FUNC_S  callback function CNcomment:回调函数 CNend
\retval HI_SUCCESS   success.   CNcomment:成功 CNend
\retval please refer to the err code definitino of mpi.CNcomment:请参考MPI错误码 CNend
\see \n
CNcomment:无 CNend
*/
HI_S32 HI_UNF_HDMI_UnRegCallbackFunc(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_CALLBACK_FUNC_S *pstCallbackFunc);

/**
\brief Load HDCP key. CNcomment:导入hdcpkey CNend
\attention \n
\param[in] enHdmi   hdmi device id.CNcomment:HDMI设备ID CNend
\param[in] *pstLoadKey  key struct length and point CNcomment:key 的结构 长度和指针 CNend
\retval HI_SUCCESS   success.   CNcomment:成功 CNend
\retval please refer to the err code definitino of mpi.CNcomment:请参考MPI错误码 CNend
\see \n
CNcomment:无 CNend
*/
HI_S32 HI_UNF_HDMI_LoadHDCPKey(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_LOAD_KEY_S *pstLoadKey);

/**
\brief get the edid information forcelly. CNcomment:从EEPROM获取EDID数据 CNend
\attention \n
Difference with two interface for get EDID
HI_UNF_HDMI_Force_GetEDID : Reread EDID from Sink,and write it to EEPROM
HI_UNF_HDMI_ReadEDID : read edid from EEPROM,not Real-time read from Sink
CNcomment:两个获取EDID的接口差别
HI_UNF_HDMI_Force_GetEDID 是重新从接收端读取Edid，并写到EEPROM
HI_UNF_HDMI_ReadEDID 是从EEPROM读取Edid，而不是实时的接收端EDID CNend
\param[in] *pstLoadKey  key struct length and point CNcomment:key 的结构 长度和指针 CNend
\retval HI_SUCCESS   success.   CNcomment:成功 CNend
\retval please refer to the err code definitino of mpi.CNcomment:请参考MPI错误码 CNend
\see \n
CNcomment:无 CNend
*/
HI_S32 HI_UNF_HDMI_ReadEDID(HI_U8 *u8Edid, HI_U32 *u32EdidLength);

/**
\brief Get HDMI runtime delay. CNcomment:获取hdmi运行时延时 CNend
\attention \n
CNend
\param[in] *pstDelay  delay struct delay time and mode CNcomment:延时结构体 延时的时长和模式 CNend
\retval HI_SUCCESS   success.   CNcomment:成功 CNend
\retval please refer to the err code definitino of mpi.CNcomment:请参考MPI错误码 CNend
\see \n
CNcomment:无 CNend
*/
HI_S32 HI_UNF_HDMI_GetDelay(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_DELAY_S *pstDelay);

/**
\brief Set HDMI runtime delay. CNcomment:设置hdmi运行时延时 CNend
\attention \n
if not use this interface, then use hdmi inner delay
CNcomment:默认情况下调用该接口前会使用内部延时,调用后根据delay模式使用延时 CNend
\param[in] *pstDelay  delay struct delay time and mode CNcomment:延时结构体 延时的时长和模式 CNend
\retval HI_SUCCESS   success.   CNcomment:成功 CNend
\retval please refer to the err code definitino of mpi.CNcomment:请参考MPI错误码 CNend
\see \n
CNcomment:无 CNend
*/
HI_S32 HI_UNF_HDMI_SetDelay(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_DELAY_S *pstDelay);

/** @} */  /** <!-- ==== API declaration end ==== */
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_UNF_HDMI_H__ */

