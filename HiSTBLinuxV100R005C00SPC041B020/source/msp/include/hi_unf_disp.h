/******************************************************************************

   Copyright (C), 2001-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
   File Name     : hi_unf_disp.h
   Version       : Initial Draft
   Author        : Hisilicon multimedia software group
   Created       : 2009/12/15
   Description   :
   History       :
   1.Date        : 2009/12/15
    Author      : w58735
    Modification: Created file

 *******************************************************************************/


/**
 * \file
 **\brief : define DISPLAY module information
 * \brief supply infor about display.
 */

#ifndef  __HI_UNF_DISP_H__
#define  __HI_UNF_DISP_H__

#include "hi_unf_common.h"
#include "hi_unf_hdmi.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*************************** Structure Definition ****************************/
/** \addtogroup      DISPLAY */
/** @{ */  /** <!-- [DISPLAY] */

/**enum define about DISPLAY channel*/
/**CNcomment:定义DISPLAY通道枚举*/
typedef enum hiUNF_DISP_E
{
    HI_UNF_DISPLAY0 = 0x0,  /**< DISPLAY0,Compatible with SD */ /**<CNcomment:高清DISPLAY0，兼容标清 */
    HI_UNF_DISPLAY1,        /**< DISPLAY1 *//**<CNcomment: 高清DISPLAY1 */
    HI_UNF_DISPLAY2,        /**< DISPLAY2 *//**<CNcomment: 高清DISPLAY2 */
    HI_UNF_DISPLAY_BUTT
}HI_UNF_DISP_E;

/**max DAC count*/
/**CNcomment:最大的DAC数目*/
#define MAX_DAC_NUM ( 6 )

/**define the LCD data format*/
/**CNcomment:定义LCD的数据格式*/
typedef enum hiHI_UNF_DISP_INTF_DATA_FMT
{
    HI_UNF_DISP_INTF_DATA_FMT_YUV422   = 0,         /**<YUV422,data width is 16*//**<CNcomment:YUV422，位宽为16*/
    HI_UNF_DISP_INTF_DATA_FMT_RGB565   = 0x8,       /**<RGB565,data width is 16*//**<CNcomment:RGB565，位宽为16*/
    HI_UNF_DISP_INTF_DATA_FMT_RGB444   = 0xa,       /**<RGB444,data width is 16*//**<CNcomment:RGB444，位宽为16*/
    HI_UNF_DISP_INTF_DATA_FMT_RGB666   = 0xb,       /**<RGB666,data width is 24*//**<CNcomment:RGB666，位宽为24*/
    HI_UNF_DISP_INTF_DATA_FMT_RGB888   = 0xc,       /**<RGB888,data widht is 24*//**<CNcomment:RGB888，位宽为24*/
    HI_UNF_DISP_INTF_DATA_FMT_BUTT
}HI_UNF_DISP_INTF_DATA_FMT_E;

/**define LCD output data width*/
/**CNcomment:定义LCD输出的数据位宽*/
typedef enum hiUNF_DISP_INTF_DATA_WIDTH_E
{
    HI_UNF_DISP_INTF_DATA_WIDTH8 = 0,       /**<8 bits*//**<CNcomment:8位*/
    HI_UNF_DISP_INTF_DATA_WIDTH16,          /**<16 bits*//**<CNcomment:16位*/
    HI_UNF_DISP_INTF_DATA_WIDTH24,          /**<24 bits*//**<CNcomment:24位*/
    HI_UNF_DISP_INTF_DATA_WIDTH_BUTT
}HI_UNF_DISP_INTF_DATA_WIDTH_E;

/**define LCD timing */
/**CNcomment:定义LCD的时序参数*/
typedef struct hiUNF_DISP_TIMING_S
{
    HI_U32                        VFB;            /**<vertical front blank*//**<CNcomment:垂直前消隐*/
    HI_U32                        VBB;            /**<vertical back blank*//**<CNcomment:垂直后消隐*/
    HI_U32                        VACT;           /**<vertical active area*//**<CNcomment:垂直有效区*/
    HI_U32                        HFB;            /**<horizonal front blank*//**<CNcomment:水平前消隐*/
    HI_U32                        HBB;            /**<horizonal back blank*//**<CNcomment:水平后消隐*/
    HI_U32                        HACT;           /**<horizonal active area*/ /**<CNcomment:水平有效区*/
    HI_U32                        VPW;            /**<vertical sync pluse width*//**<CNcomment:垂直脉冲宽度*/
    HI_U32                        HPW;            /**<horizonal sync pluse width*/ /**<CNcomment:水平脉冲宽度*/
    HI_BOOL                       IDV;            /**< flag of data valid signal is needed flip*//**<CNcomment:有效数据信号是否翻转*/
    HI_BOOL                       IHS;            /**<flag of horizonal sync pluse is needed flip*//**<CNcomment:水平同步脉冲信号是否翻转*/
    HI_BOOL                       IVS;            /**<flag of vertical sync pluse is needed flip*//**<CNcomment:垂直同步脉冲信号是否翻转*/
    HI_BOOL                       ClockReversal;  /**<flag of clock is needed flip*//**<CNcomment:时钟是否翻转*/
    HI_UNF_DISP_INTF_DATA_WIDTH_E DataWidth;      /**<data width*/ /**<CNcomment:数据位宽*/
    HI_UNF_DISP_INTF_DATA_FMT_E   ItfFormat;      /**<data format.*//**<CNcomment:数据格式.*/

    HI_BOOL DitherEnable;                           /**< flag of is enable Dither*//**<CNcomment:数据格式.*/
    HI_U32  ClkPara0;                               /**<PLL  register SC_VPLL1FREQCTRL0  value *//**<CNcomment:PLL  SC_VPLL1FREQCTRL0  寄存器*/
    HI_U32  ClkPara1;                               /**<PLL  register SC_VPLL1FREQCTRL1 value*//**<CNcomment:PLL   SC_VPLL1FREQCTRL1寄存器*/

    HI_BOOL bInterlace;                             /**<progressive or interlace*//**<CNcomment:逐行或者隔行*/
    HI_U32  PixFreq;                                /**<pixel clock*//**<CNcomment:像素时钟*/
    HI_U32  VertFreq;                               /**<display rate*//**<CNcomment:刷新率*/
    HI_U32  AspectRatioW;                           /**<width of screen*//**<CNcomment:屏幕宽度*/
    HI_U32  AspectRatioH;                           /**<height of screen*//**<CNcomment:屏幕高度*/
    HI_BOOL bUseGamma;                              /**<gamma modulation*//**<CNcomment:伽马调节*/
    HI_U32  Reserve0;                               /**<reserved byte*//**<CNcomment:保留位*/
    HI_U32  Reserve1;                               /**<reserved byte*//**<CNcomment:保留位*/
} HI_UNF_DISP_TIMING_S;

/**define video and graphics layers */
/**CNcomment:视频和图形层*/
typedef enum hiUNF_DISP_LAYER_E
{
    HI_UNF_DISP_LAYER_VIDEO = 0,                   /**<video layer *//**<CNcomment:视频层*/
    HI_UNF_DISP_LAYER_GFX,                  /**<graphics layer *//**<CNcomment:图形层*/
    HI_UNF_DISP_LAYER_BUTT
}HI_UNF_DISP_LAYER_E;

/**bt1120 interface ID */
/**CNcomment:bt1120 接口ID  */
typedef enum hiUNF_DISP_BT1120_E
{
    HI_UNF_DISP_BT1120_0,    /**<BT1120 interface 0 *//**<CNcomment:BT1120接口0 */
    HI_UNF_DISP_BT1120_BUTT,
}HI_UNF_DISP_BT1120_E;

/**BT656 interface ID */
/**CNcomment:BT656 接口ID  */
typedef enum hiUNF_DISP_BT656_E
{
    HI_UNF_DISP_BT656_0,    /**<BT656 interface 0 *//**<CNcomment:BT656接口0 */
    HI_UNF_DISP_BT656_BUTT,
}HI_UNF_DISP_BT656_E;

/**LCD interface ID */
/**CNcomment:LCD 接口ID  */
typedef enum hiUNF_LCD_E
{
    HI_UNF_DISP_LCD_0,    /**<LCD interface 0 *//**<CNcomment:LCD接口0 */
    HI_UNF_DISP_LCD_BUTT,
}HI_UNF_DISP_LCD_E;

/**define the type of interface*/
/**CNcomment:定义接口类型*/
typedef enum  hiUNF_DISP_INTF_TYPE_E
{
    HI_UNF_DISP_INTF_TYPE_HDMI,     /**<HDMI interface type *//**<CNcomment:HDMI接口类型*/
    HI_UNF_DISP_INTF_TYPE_LCD,      /**<LCD interface type *//**<CNcomment:LCD接口类型*/
    HI_UNF_DISP_INTF_TYPE_BT1120,   /**<bt1120 digital interface type *//**<CNcomment:BT1120数字接口类型*/
    HI_UNF_DISP_INTF_TYPE_BT656,    /**<bt656 digital interface type*//**<CNcomment:BT656数字接口类型*/
    HI_UNF_DISP_INTF_TYPE_YPBPR,    /**<YPBPR interface type*//**<CNcomment:YPBPR接口类型*/
    HI_UNF_DISP_INTF_TYPE_RGB,      /**<RGB interface type*//**<CNcomment:RGB接口类型*/
    HI_UNF_DISP_INTF_TYPE_CVBS,     /**<CVBS interface type*//**<CNcomment:CVBS接口类型*/
    HI_UNF_DISP_INTF_TYPE_SVIDEO,   /**<SVIDEO interface type*//**<CNcomment:SVIDEO接口类型*/
    HI_UNF_DISP_INTF_TYPE_VGA,      /**<VGA interface type*//**<CNcomment:VGA接口类型*/
    HI_UNF_DISP_INTF_TYPE_BUTT
}HI_UNF_DISP_INTF_TYPE_E;

/**define the YPBPR type struct of interface*/
/**CNcomment:定义YPBPR接口结构*/
typedef struct  hiUNF_DISP_INTF_YPBPR_S
{
    HI_U8 u8DacY;             /**<DAC num of Y  *//**<CNcomment:Y分量dac端口号*/
    HI_U8 u8DacPb;            /**<DAC num of Pb  *//**<CNcomment:Pb分量dac端口号*/
    HI_U8 u8DacPr;            /**<DAC num of Pr  *//**<CNcomment:Pr分量dac端口号*/
}HI_UNF_DISP_INTF_YPBPR_S;

/**define the RGB type struct of interface*/
/**CNcomment:定义RGB接口结构*/
typedef struct  hiUNF_DISP_INTF_RGB_S
{
    HI_U8  u8DacR;            /**<DAC num of R  *//**<CNcomment:R分量dac端口号*/
    HI_U8  u8DacG;            /**<DAC num of G  *//**<CNcomment:G分量dac端口号*/
    HI_U8  u8DacB;            /**<DAC num of B  *//**<CNcomment:B分量dac端口号*/
    HI_BOOL bDacSync;         /**<G without sync signal  *//**<CNcomment:G分量不带同步信号*/
}HI_UNF_DISP_INTF_RGB_S;

/**define the VGA type struct of interface*/
/**CNcomment:定义VGA接口结构*/
typedef struct  hiUNF_DISP_INTF_VGA_S
{
    HI_U8  u8DacR;            /**<DAC num of R  *//**<CNcomment:R分量dac端口号*/
    HI_U8  u8DacG;            /**<DAC num of G  *//**<CNcomment:G分量dac端口号*/
    HI_U8  u8DacB;            /**<DAC num of B  *//**<CNcomment:B分量dac端口号*/
}HI_UNF_DISP_INTF_VGA_S;

/**define the CVBS type struct of interface*/
/**CNcomment:定义CVBS接口结构*/
typedef struct  hiUNF_DISP_INTF_CVBS_S
{
    HI_U8 u8Dac;              /**<DAC num of CVBS  *//**<CNcomment:CVBS端子dac端口号*/
}HI_UNF_DISP_INTF_CVBS_S;

/**define the SVIDEO type struct of interface*/
/**CNcomment:定义SVIDEO接口结构*/
typedef struct  hiUNF_DISP_INTF_SVIDEO_S
{
    HI_U8 u8DacY;             /**<DAC num of Y  *//**<CNcomment:Y分量dac端口号*/
    HI_U8 u8DacC;             /**<DAC num of C   *//**<CNcomment:C分量dac端口号*/
}HI_UNF_DISP_INTF_SVIDEO_S;

/**define display interface struct*/
/**CNcomment:定义显示接口结构*/
typedef struct  hiUNF_DISP_INTF_S
{
    HI_UNF_DISP_INTF_TYPE_E enIntfType;         /**<interface type *//**<CNcomment:接口类型*/
    union
    {
        HI_UNF_HDMI_ID_E        enHdmi;         /**<hdmi id *//**<CNcomment:HDMI 序号*/
        HI_UNF_DISP_BT1120_E    enBT1120;       /**<bt1120 id *//**<CNcomment:BT1120序号*/
        HI_UNF_DISP_BT656_E     enBT656;        /**<bt656 id *//**<CNcomment:BT656序号*/
        HI_UNF_DISP_LCD_E       enLcd;          /**<lcd id *//**<CNcomment:LCD序号*/

        HI_UNF_DISP_INTF_YPBPR_S    stYPbPr;    /**<intf config of YPBPR  *//**<CNcomment:YPBPR接口配置*/
        HI_UNF_DISP_INTF_RGB_S      stRGB;      /**<intf config of RGB  *//**<CNcomment:RGB接口配置*/
        HI_UNF_DISP_INTF_VGA_S      stVGA;      /**<intf config of VGA  *//**<CNcomment:VGA接口配置*/
        HI_UNF_DISP_INTF_CVBS_S     stCVBS;     /**<intf config of CVBS  *//**<CNcomment:CVBS接口配置*/
        HI_UNF_DISP_INTF_SVIDEO_S   stSVideo;   /**<intf config of SVIDEO  *//**<CNcomment:SVIDEO接口配置*/
    }unIntf;
}HI_UNF_DISP_INTF_S;


/**Defines the device aspect ratio.*/
/**CNcomment: 定义设备宽高比枚举*/
typedef enum hiUNF_DISP_ASPECT_RATIO_E
{
    HI_UNF_DISP_ASPECT_RATIO_AUTO,              /**<aspect ratio as device Resolution*//**<CNcomment: 宽高比与设备分辨率一致*/
    HI_UNF_DISP_ASPECT_RATIO_4TO3,              /**<4:3*//**<CNcomment: 4比3*/
    HI_UNF_DISP_ASPECT_RATIO_16TO9,             /**<16:9*//**<CNcomment: 16比9*/
    HI_UNF_DISP_ASPECT_RATIO_221TO1,            /**<221:100*//**<CNcomment: 221比100*/
    HI_UNF_DISP_ASPECT_RATIO_USER,              /**<user define*//**<CNcomment: 用户定义*/

    HI_UNF_DISP_ASPECT_RATIO_BUTT
}HI_UNF_DISP_ASPECT_RATIO_E;

/**Defines the device aspect ratio struct.*/
/**CNcomment: 定义设备宽高比结构*/
typedef struct hiUNF_DISP_ASPECT_RATIO_S
{
    HI_UNF_DISP_ASPECT_RATIO_E enDispAspectRatio;   /**<aspect ratio type of device*//**<CNcomment: 设备宽高比类型*/
    HI_U32                     u32UserAspectWidth;  /**<user define width of device*//**<CNcomment: 用户定义设备宽度*/
    HI_U32                     u32UserAspectHeight; /**<user define height of device*//**<CNcomment: 用户定义设备高度*/
}HI_UNF_DISP_ASPECT_RATIO_S;

/**Defines algorithmic  control struct of display device.*/
/**CNcomment: 定义显示设备算法控制结构*/
typedef struct hiUNF_DISP_ALG_CFG_S
{
    HI_BOOL bAccEnable;                             /**<acc alg*//**<CNcomment: acc算法*/
    HI_BOOL bSharpEnable;                           /**<sharp alg*//**<CNcomment: sharp算法*/
}HI_UNF_DISP_ALG_CFG_S;

/**Defines VBI type.*/
/**CNcomment: 定义VBI数据类型*/
typedef enum hiUNF_DISP_VBI_TYPE_E
{
    HI_UNF_DISP_VBI_TYPE_TTX = 0,                   /**<Teltext type*//**<CNcomment:teltext类型*/
    HI_UNF_DISP_VBI_TYPE_CC,                        /**<Closed caption/V-Chip type *//**<CNcomment: 隐藏字幕/V-Chip类型*/
    HI_UNF_DISP_VBI_TYPE_WSS,                       /**<Wide screen signal*//**<CNcomment:宽屏信令*/
    HI_UNF_DISP_VBI_TYPE_BUTT
} HI_UNF_DISP_VBI_TYPE_E;

/**Defines VBI config struct.*/
/**CNcomment: 定义VBI配置结构*/
typedef struct hiUNF_DISP_VBI_CFG_S
{
    HI_UNF_DISP_VBI_TYPE_E  enType;                  /**<VBI type*//**<CNcomment: VBI 类型*/
    HI_U32                  u32InBufferSize;        /**<VBI data(pes) buffer size,more than 4K,suggest 4K*//**<CNcomment:VBI pes 数据输入缓冲大小,不小于4K，推荐值4K*/
    HI_U32                  u32WorkBufferSize;      /**<VBI data buffer size used in driver,more than 2K,suggest 2K*//**<CNcomment:VBI解析后数据在驱动中接收缓冲大小不小于2K，推荐值2K*/
} HI_UNF_DISP_VBI_CFG_S;

/** define VBI information structure*/
/**CNcomment: 定义VBI信息数据结构*/
typedef struct hiUNF_DISP_VBI_DATA_S
{
    HI_UNF_DISP_VBI_TYPE_E  enType;                  /**<VBI type*//**<CNcomment: VBI 类型*/
    HI_U8                   *pu8DataAddr;           /**<Vbi data buffer virtual address*//**<CNcomment:Vbi数据用户虚拟地址*/
    HI_U32                  u32DataLen;             /**<Vbi data lenght*//**<CNcomment:Vbi数据长度*/
} HI_UNF_DISP_VBI_DATA_S;

/**Defines the MAX buffer number.*/
/** CNcomment:定义最大的buffer 分配数*/
#define HI_DISP_CAST_BUFFER_MAX_NUMBER ( 16 )

/**define CAST config */
/**CNcomment:定义屏幕投影配置*/
typedef struct hiUNF_DISP_CAST_ATTR_S
{
    HI_UNF_VIDEO_FORMAT_E enFormat;                                        /**<the output video format.*//**<CNcomment:定义输出视频格式*/
    HI_U32                u32Width;                                       /**<the output video width.*//**<CNcomment:定义输出视频的宽*/
    HI_U32                u32Height;                                      /**<the output video height.*//**<CNcomment:定义输出视频的高*/
    HI_BOOL               bLowDelay;                                      /**<work at low delay mode.*//**<CNcomment:启用低延迟模式*/
    HI_U32                u32BufNum;                                      /**<the buffer number.*//**<CNcomment:定义分配buffer 数*/
    HI_BOOL               bUserAlloc;                                     /**<whether is user alloc memory*//**<CNcomment:定义是否用户分配内存*/
    HI_U32                u32BufSize;                                     /**<each the buffer size.*//**<CNcomment:定义用户分配每个buffer 大小*/
    HI_U32                u32BufStride;                                   /**<the horizonal stride.*//**<CNcomment:定义行对齐*/
    HI_U32                u32BufPhyAddr[HI_DISP_CAST_BUFFER_MAX_NUMBER];  /**<the buffer physics address.*//**<CNcomment:分配内存buffer 成员的物理地址*/
    HI_BOOL               bCrop;                                          /**<whether enable crop .*//**<CNcomment:定义是否进行crop*/
    HI_UNF_CROP_RECT_S    stCropRect;                                     /**<the crop wise .*//**<CNcomment:定义crop 范围*/
} HI_UNF_DISP_CAST_ATTR_S;

/**define color setting */
/**CNcomment:定义色彩设置*/
typedef struct hiUNF_DISP_COLOR_SETTING_S
{
    HI_BOOL bGammaEnable;         /**<whether Gamma enable.*//**<CNcomment:定义Gamma 使能*/
    HI_BOOL bUseCustGammaTable;   /**<whether use custom GammaTable.*//**<CNcomment:是否使用用户Gamma表*/
    HI_BOOL bColorCorrectEnable;  /**<whether Color Correct.*//**<CNcomment:是否色彩校正*/
    HI_S32  s32ColorTemp;         /**<color Temp.*//**<CNcomment:色温*/
    HI_U32  u32Reserve;           /**<Reserve.*//**<CNcomment:保留*/
    HI_VOID *pPrivate;            /**<Private.*//**<CNcomment:私有体指针*/
}HI_UNF_DISP_COLOR_SETTING_S;

/**define the struct about color */
/**CNcomment:定义显示颜色的结构体 */
typedef struct  hiUNF_DISP_BG_COLOR_S
{
    HI_U8 u8Red;                  /**<red *//**<CNcomment:红色分量*/
    HI_U8 u8Green;                /**<green*//**<CNcomment:绿色分量*/
    HI_U8 u8Blue;                 /**<blue*//**<CNcomment:蓝色分量*/
} HI_UNF_DISP_BG_COLOR_S;

/**define the struct about isogeny attributes */
/**CNcomment:定义同源属性 */
typedef struct  hiUNF_DISP_ISOGENY_ATTR_S
{
    HI_UNF_DISP_E          enDisp; /**<channel id*//**<CNcomment: 通道号*/
    HI_UNF_ENC_FMT_E       enFormat;/**<format type*//**<CNcomment:制式类型*/
}HI_UNF_DISP_ISOGENY_ATTR_S ;


/********************************ENCODER STRUCT********************************/
/** define the enum of Macrovision output type*/
/** CNcomment:显示输出Macrovision模式枚举定义*/
typedef enum hiUNF_DISP_MACROVISION_MODE_E
{
    HI_UNF_DISP_MACROVISION_MODE_TYPE0,     /**<type 0 *//**<CNcomment:典型配置0 */
    HI_UNF_DISP_MACROVISION_MODE_TYPE1,     /**<type 1 *//**<CNcomment:典型配置1 */
    HI_UNF_DISP_MACROVISION_MODE_TYPE2,     /**<type 2 *//**<CNcomment:典型配置2 */
    HI_UNF_DISP_MACROVISION_MODE_TYPE3,     /**<type 3 *//**<CNcomment:典型配置3 */
    HI_UNF_DISP_MACROVISION_MODE_CUSTOM,    /**<type of configure by user *//**<CNcomment:用户自定义配置 */
    HI_UNF_DISP_MACROVISION_MODE_BUTT
} HI_UNF_DISP_MACROVISION_MODE_E;

/** CGMS type select */
/**CNcomment:CGMS 类型选择*/
typedef enum hiUNF_DISP_CGMS_TYPE_E
{
    HI_UNF_DISP_CGMS_TYPE_A = 0x00,     /**<CGMS type  A*//**<CNcomment:CGMS 类型A*/
    HI_UNF_DISP_CGMS_TYPE_B,            /**<CGMS type  B*//**<CNcomment:CGMS 类型B*/

    HI_UNF_DISP_CGMS_TYPE_BUTT
}HI_UNF_DISP_CGMS_TYPE_E;

/** definition of CGMS mode */
/**CNcomment:定义CGMS 模式*/
typedef enum hiUNF_DISP_CGMS_MODE_E
{
    HI_UNF_DISP_CGMS_MODE_COPY_FREELY  = 0,     /**<copying is permitted without restriction *//**<CNcomment:无限制拷贝*/
    HI_UNF_DISP_CGMS_MODE_COPY_NO_MORE = 0x01,  /**<No more copies are allowed (one generation copy has been made)*//**<CNcomment:拷贝一次后不允许再被拷贝*/
    HI_UNF_DISP_CGMS_MODE_COPY_ONCE    = 0x02,  /**<One generation of copies may be made *//**<CNcomment:仅允许拷贝一次*/
    HI_UNF_DISP_CGMS_MODE_COPY_NEVER   = 0x03,  /**<No copying is permitted *//**<CNcomment:不允许拷贝*/

    HI_UNF_DISP_CGMS_MODE_BUTT
}HI_UNF_DISP_CGMS_MODE_E;

/** definition of CGMS configuration */
typedef struct hiUNF_DISP_CGMS_CFG_S
{
    HI_BOOL                 bEnable;            /**<HI_TRUE:CGMS is enabled; HI_FALSE:CGMS is disabled *//**<CNcomment:CGMS 使能选项*/
    HI_UNF_DISP_CGMS_TYPE_E enType;             /**<type-A or type-B or None(BUTT) *//**<CNcomment:CGMS 类型*/
    HI_UNF_DISP_CGMS_MODE_E enMode;             /**<CGMS mode. *//**<CNcomment:CGMS 模式*/
}HI_UNF_DISP_CGMS_CFG_S;

/**define WSS information structure*/
/**CNcomment:定义图文信息数据结构 */
typedef struct hiUNF_DISP_WSS_DATA_S
{
    HI_BOOL bEnable;                            /**<WSS configure enable HI_TRUE: enable,HI_FALSE: disnable*//**<CNcomment:WSS配置使能。HI_TRUE：使能；HI_FALSE：禁止*/
    HI_U16  u16Data;                            /**<Wss data */ /**<CNcomment:Wss数据*/
}HI_UNF_DISP_WSS_DATA_S;


/**define display 3D mode stucture*/
/**CNcomment:定义显示模式结构体 */
typedef enum hiUNF_DISP_3D_E
{
    HI_UNF_DISP_3D_NONE = 0,
    HI_UNF_DISP_3D_FRAME_PACKING,                   /**<3d type:Frame Packing*//**<CNcomment:3d 模式:帧封装*/
    HI_UNF_DISP_3D_SIDE_BY_SIDE_HALF,               /**<3d type:Side by side half*//**<CNcomment:3d 模式:并排式 左右半边*/
    HI_UNF_DISP_3D_TOP_AND_BOTTOM,                  /**<3d type:Top and Bottom*//**<CNcomment:3d 模式:上下模式*/
    HI_UNF_DISP_3D_FIELD_ALTERNATIVE,               /**<3d type:Field alternative*//**<CNcomment:3d 模式:场交错*/
    HI_UNF_DISP_3D_LINE_ALTERNATIVE,                /**<3d type:Field alternative*//**<CNcomment:3d 模式:行交错*/
    HI_UNF_DISP_3D_SIDE_BY_SIDE_FULL,               /**<3d type:Side by side full*//**<CNcomment:3d 模式:并排式 左右全场*/
    HI_UNF_DISP_3D_L_DEPTH,                         /**<3d type:L+depth*//**<CNcomment:3d 模式:L+DEPTH*/
    HI_UNF_DISP_3D_L_DEPTH_GRAPHICS_GRAPHICS_DEPTH, /**<3d type:L+depth+Graphics+Graphics-depth*//**<CNcomment:3d 模式:L+depth+Graphics+Graphics-depth*/
    HI_UNF_DISP_3D_BUTT
}HI_UNF_DISP_3D_E;

/**define display margin stucture*/
/**CNcomment:定义显示空白区域结构体 */
typedef struct hiUNF_DISP_OFFSET_S
{
    HI_U32 u32Left;    /**<left offset *//**<CNcomment:左侧偏移*/
    HI_U32 u32Top;     /**<top offset *//**<CNcomment:上方偏移*/
    HI_U32 u32Right;   /**<right offset *//**<CNcomment:右侧偏移*/
    HI_U32 u32Bottom;  /**<bottom offset *//**<CNcomment:下方偏移*/
}HI_UNF_DISP_OFFSET_S;

typedef enum hiUNF_DISP_HDR_TYPE_E
{
    HI_UNF_DISP_HDR_TYPE_NONE = 0,      /* 普通信号显示BT601 BT709 */
    HI_UNF_DISP_HDR_TYPE_DOLBY,         /* DolbyVision信号显示 */
    HI_UNF_DISP_HDR_TYPE_HDR10,         /* 标准HDR信号显示 */
    HI_UNF_DISP_HDR_TYPE_BUTT
} HI_UNF_DISP_HDR_TYPE_E;


/** @} */  /** <!-- ==== Structure Definition end ==== */

/******************************* API declaration *****************************/
/** \addtogroup      DISPLAY */
/** @{ */  /** <!-- [DISPLAY] */


/**
   \brief Initiallization DISP module.CNcomment:初始化DISP模块 CNend
   \attention \n
   Please call this API function, before call anyother API of DISP module.
   CNcomment:调用DISP模块其它接口前要求首先调用本接口 CNend
   \param  none.CNcomment:无 CNend
   \retval ::HI_SUCCESS  operation success.CNcomment:成功 CNend
   \retval ::HI_ERR_DISP_DEV_NOT_EXIST DISP device not exist.CNcomment:设备不存在 CNend
   \retval ::HI_ERR_DISP_NOT_DEV_FILE  DISP  not device file .CNcomment:非设备 CNend
   \retval ::HI_ERR_DISP_DEV_OPEN_ERR  DISP  open fail.CNcomment:打开失败 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_Init( HI_VOID );


/**
   \brief deinital.CNcomment:去初始化DISP模块 CNend
   \attention \n
   please call API HI_UNF_DISP_Close and open all the DISP device, before call this API.
   CNcomment:在调用::HI_UNF_DISP_Close接口关闭所有打开的DISP后调用本接口 CNend
   \param none.CNcomment:无 CNend
   \retval ::HI_SUCCESS success.CNcomment:成功 CNend
   \retval ::HI_ERR_DISP_DEV_CLOSE_ERR  DISP close fail.CNcomment:关闭失败 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_DeInit( HI_VOID );


/**
   \brief Attach one DISP channel to anoter.CNcomment:将两个DISP通道绑定 CNend
   \attention \n
   Please finish the attach operation before the DISP channel has been open, and currently we only support HD channel attach to SD channel.
   CNcomment:目前支持将高清DISP绑定到标清DISP通道上，绑定在打开DISP通道前必须完成绑定操作。 CNend
   \param[in] enDstDisp   Destination DISP channel.CNcomment:目标DISP通道号 CNend
   \param[in] enSrcDisp   source DISP channel.CNcomment:  源DISP通道号 CNend
   \retval ::HI_SUCCESS  operation success.CNcomment:成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT  display not be initialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA  invalid input parameter.CNcomment:输入参数非法 CNend
   \retval ::HI_ERR_DISP_INVALID_OPT   invalid opeation.CNcomment:操作非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_Attach( HI_UNF_DISP_E enDstDisp, HI_UNF_DISP_E enSrcDisp );


/**
   \brief dettach DISP.CNcomment:将两个DISP通道解绑定 CNend
   \attention \n
   should  close the DISP channels, before do detach operation.
   CNcomment:只有在关闭两个DISP通道后才能进行解绑定操作。 CNend
   \param[in] enDstDisp   Destination DISP channel.CNcomment:目标DISP通道号 CNend
   \param[in] enSrcDisp    source DISP channel.CNcomment: 源DISP通道号 CNend
   \retval ::HI_SUCCESS  success.CNcomment:成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT   DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA  invalid input parameter.CNcomment:输入参数非法 CNend
   \retval ::HI_ERR_DISP_INVALID_OPT   invalid operation.CNcomment:操作非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_Detach( HI_UNF_DISP_E enDstDisp, HI_UNF_DISP_E enSrcDisp );


/**
   \brief open DISP channel.CNcomment:打开指定DISP通道 CNend
   \attention \n
   Please config the valid parameters before open DISP.
   CNcomment:在打开DISP之前，先完成对其的参数设置，避免画面闪烁 CNend
   \param[in] enDisp   DISP channel ID, please reference the define of HI_UNF_DISP_E.CNcomment:DISP通道号，请参见::HI_UNF_DISP_E CNend
   \retval ::HI_SUCCESS success.CNcomment:成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT   DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA  invalid input parameter.CNcomment:输入参数非法 CNend
   \retval ::HI_ERR_DISP_CREATE_ERR    DISP create fail.CNcomment:DISP创建失败 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_Open( HI_UNF_DISP_E enDisp );


/**
   \brief  coase DISP channel.CNcomment:关闭指定DISP CNend
   \attention \n
   none.
   CNcomment:无 CNend
   \param[in] enDisp DISP channel ID.CNcomment:DISP通道号 CNend
   \retval ::HI_SUCCESS success. CNcomment:成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA invalid input parameter.CNcomment:输入参数非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_Close( HI_UNF_DISP_E enDisp );


/**
   \brief  set DISP interface parameter.CNcomment:设置DISP的接口参数 CNend
   \attention \n
   The API is uesed for set  interface attach .If the interface has been attached,must be detach it ,or return HI_ERR_DISP_INVALID_OPT;\n
   There are 4 DAC provided by SOC 3712 v300. suggest, DAC 0/1/2 used by HD channel, DAC3 used by SD channel CVBS. \n
   There are 6 DAC provided by SOC 3716 v100/v200/v300. suggest, DAC 0/1/2 used by HD channel, DAC3/4/5 used by SD channel.
   CNcomment:该API 用来设置输出接口的绑定关系，如果设置已被绑定过的接口，则需要先进行Detach操作\n
   解除绑定关系，否则返回错误HI_ERR_DISP_INVALID_OPT;\n
   3712 v300 芯片共有4个DAC，推荐0，1，2用作高清输出，3用作标清CVBS输出。\n
   3716 v100/v200/v300 芯片共有6个DAC，推荐0，1，2用作高清输出，3，4，5用作标清输出。CNend
   \param[in] enDisp      DISP channel ID.CNcomment:DISP通道号 CNend
   \param[in] u32IntfNum  DISP intf num of DISP channel.CNcomment:DISP通道上的接口数目 CNend
   \param[in] pstIntf  DISP intf para.CNcomment:DISP接口参数 CNend
   \retval ::HI_SUCCESS   success.CNcomment:成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT   DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_NULL_PTR      Input pointer is NULL.CNcomment:输入指针为空 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA  invalid input parameter.CNcomment:输入参数非法 CNend
   \retval ::HI_ERR_DISP_INVALID_OPT   invalid operation.CNcomment:操作非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_AttachIntf( HI_UNF_DISP_E enDisp, HI_UNF_DISP_INTF_S *pstIntf, HI_U32 u32IntfNum );


/**
   \brief cancel DISP interface parameter.CNcomment:取消DISP的接口参数 CNend
   \attention \n
   The API is uesed to  detach  interface.
   CNcomment:这个API 用来解除接口的绑定关系。 CNend
   \param[in] enDisp        DISP channel ID.CNcomment:DISP通道号 CNend
   \param[in] u32IntfNum  DISP interface number of DISP channel.CNcomment:DISP通道上的接口数目 CNend
   \param[in] pstIntf  DISP interface para.CNcomment:DISP接口参数 CNend
   \retval ::HI_SUCCESS   success. CNcomment:成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT   DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_NULL_PTR        Input pointer is NULL.CNcomment:输入指针为空 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA  invalid input parameter.CNcomment:输入参数非法 CNend
   \retval ::HI_ERR_DISP_INVALID_OPT   invalid operation.CNcomment:操作非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_DetachIntf( HI_UNF_DISP_E enDisp, HI_UNF_DISP_INTF_S *pstIntf, HI_U32 u32IntfNum );


/**
   \brief set DISP output format.CNcomment:设置DISP的制式 CNend
   \attention \n
   for HD DISP channel please set HD display format, and for SD DISP channel please set SD display format.\n
   for scenario of HD,SD use same source, only support format which frame rate is 50Hz or 60Hz.
   CNcomment:对高清的DISP，只能设置高清的制式；对标清的DISP，只能设置标清的制式。\n
   在同源显示场景下，暂不支持刷新率非50Hz/60Hz的制式，如：暂不支持1080P24/1080P25/1080P30 CNend
   \param[in] enDisp               DISP channel ID.CNcomment:DISP通道号 CNend
   \param[in] enEncodingFormat    DISP format.CNcomment:DISP的制式 CNend
   \retval ::HI_SUCCESS  success.CNcomment:成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \retval ::HI_ERR_DISP_INVALID_OPT    invalid operation.CNcomment:操作非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_SetFormat( HI_UNF_DISP_E enDisp, HI_UNF_ENC_FMT_E enEncodingFormat );


/**
   \brief get DISP format.CNcomment:获取DISP的制式 CNend
   \attention \n
   none.CNcomment:无 CNend
   \param[in] enDisp               DISP channel ID.CNcomment:DISP通道号 CNend
   \param[in] penEncodingFormat    poiner of DISP format.DCNcomment:ISP的制式指针 CNend
   \retval ::HI_SUCCESS  success.CNcomment:成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \retval ::HI_ERR_DISP_INVALID_OPT    invalid operation.CNcomment:操作非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_GetFormat( HI_UNF_DISP_E enDisp, HI_UNF_ENC_FMT_E *penEncodingFormat );


/**
   \brief set user define LCD clock parameter.CNcomment:设置用户定义的LCD时序参数 CNend
   \attention \n
   only 3716 v100 v200 is  supported LCD.
   CNcomment:只有3716 v100 v200 支持LCD CNend
   \param[in] enDisp               DISP channel ID.CNcomment:DISP通道号 CNend
   \param[in] pstTiming    poiner of DISP format.CNcomment:LCD时序参数 CNend
   \retval ::HI_SUCCESS  success.CNcomment:成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \retval ::HI_ERR_DISP_INVALID_OPT    invalid operation.CNcomment:操作非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_SetCustomTiming( HI_UNF_DISP_E enDisp, HI_UNF_DISP_TIMING_S *pstTiming );


/**
   \brief  get LCD parameter which had been config.CNcomment:获取DISP的LCD参数 CNend
   \attention \n
   only 3716 v100 v200 is  supported LCD.
   CNcomment:只有3716 v100 v200 支持LCD CNend
   \param[in] enDisp          DISP channel ID.CNcomment:DISP通道号 CNend
   \param[in] pstTiming      pointer of LCD paramter.CNcomment:指针类型，LCD参数 CNend
   \retval ::HI_SUCCESS success.CNcomment: 成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \retval ::HI_ERR_DISP_INVALID_OPT    invalid operation.CNcomment:操作非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_GetCustomTiming( HI_UNF_DISP_E enDisp, HI_UNF_DISP_TIMING_S *pstTiming );


/**
   \brief   set DISP layers Z order.CNcomment:设置DISP上叠加层的Z序 CNend
   \attention \n
   only 3716 v100 v200 is  supported LCD.
   CNcomment:只有3716 v100 v200 支持LCD CNend
   \param[in] enDisp          DISP channel ID.CNcomment: DISP通道号 CNend
   \param[in] enLayer        layer ID which needed overlay to the DISP channel.CNcomment: DISP上的叠加层 CNend
   \param[in] enZFlag        the way of overlay.CNcomment:Z序调节方式 CNend
   \retval ::HI_SUCCESS  success.CNcomment:成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \retval ::HI_ERR_DISP_INVALID_OPT    invalid operation.CNcomment:操作非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_SetLayerZorder( HI_UNF_DISP_E enDisp, HI_UNF_DISP_LAYER_E enLayer, HI_LAYER_ZORDER_E enZFlag );


/**
   \brief  get DISP overlay z order.CNcomment:获取DISP上叠加层的Z序 CNend
   \attention \n
   In the case of HD atach to SD. the Z order configuration of HD will auto sync to SD, also the configuration of SD will auto sync to HD.
   CNcomment:在高标清绑定情况下，高清DISP的设置会自动同步到标清DISP；标清DISP的设置会自动同步到高清DISP。 CNend
   \param[in] enDisp            DISP channel ID.CNcomment:DISP通道号 CNend
   \param[in] enLayer           CNcomment:DISP上的叠加层 CNend
   \param[in] pu32Zorder        CNcomment:叠加层Z序数值，数值越大优先级越高 CNend
   \retval ::HI_SUCCESS success.CNcomment: 成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \retval ::HI_ERR_DISP_INVALID_OPT    invalid operation.CNcomment:操作非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_GetLayerZorder( HI_UNF_DISP_E enDisp, HI_UNF_DISP_LAYER_E enLayer, HI_U32 *pu32Zorder );


/**
   \brief set DISP back ground color.CNcomment:设置DISP背景色 CNend
   \attention \n
   none.
   CNcomment:无 CNend
   \param[in] enDisp       CNcomment:DISP通道号 CNend
   \param[in] pstBgColor   CNcomment:指针类型，待配置的显示输出背景色。请参见::HI_UNF_BG_COLOR_S CNend
   \retval ::HI_SUCCESS  success.CNcomment:成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:操作非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_SetBgColor( HI_UNF_DISP_E enDisp, const HI_UNF_DISP_BG_COLOR_S *pstBgColor );


/**
   \brief  set DISP back ground color.CNcomment:获取DISP背景色 CNend
   \attention \n
   In the case of HD atach to SD. the Z order configuration of HD will auto sync to SD, also the configuration of SD will auto sync to HD.
   CNcomment:在高标清绑定情况下，高清DISP的设置会自动同步到标清DISP；标清DISP的设置会自动同步到高清DISP。 CNend
   \param[in] enDisp          DISP channel ID.CNcomment:DISP通道号 CNend
   \param[out] pstBgColor   pointer of back ground color.CNcomment: 指针类型，显示输出背景色 CNend
   \retval ::HI_SUCCESS  success.CNcomment:成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:操作非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_GetBgColor( HI_UNF_DISP_E enDisp, HI_UNF_DISP_BG_COLOR_S *pstBgColor );


/**
   \brief  set DISP brightness.CNcomment:设置DISP亮度 CNend
   \attention \n
   if the value seted more than 100, we clip it to 100.CNcomment:大于100的值按100处理 CNend
   \param[in] enDisp            DISP channel ID.CNcomment:DISP通道号 CNend
   \param[in] u32Brightness   brightness value. the range is 0~100, 0 means the min brightness value.
   CNcomment:待设置的显示输出亮度值。取值范围为0～100。0：最小亮度；100：最大亮度 CNend
   \retval ::HI_SUCCESS  success.CNcomment:成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:操作非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_SetBrightness( HI_UNF_DISP_E enDisp, HI_U32 u32Brightness );


/**
   \brief   get the DISP brightness.CNcomment:获取DISP亮度 CNend
   \attention \n
   In the case of HD atach to SD. the Z order configuration of HD will auto sync to SD, also the configuration of SD will auto sync to HD.
   CNcomment:查询的默认亮度值为50。\n
   在高标清绑定情况下，高清DISP的设置会自动同步到标清DISP；标清DISP的设置会自动同步到高清DISP。 CNend
   \param[in] enDisp              DISP channel ID.CNcomment:DISP通道号 CNend
   \param[out] pu32Brightness    pointer of brightness. CNcomment:指针类型，显示输出亮度值 CNend
   \retval ::HI_SUCCESS  success.CNcomment:成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:操作非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_GetBrightness( HI_UNF_DISP_E enDisp, HI_U32 *pu32Brightness );


/**
   \brief  set DISP contrast value.CNcomment:设置DISP对比度 CNend
   \attention \n
   If the value is more than 100, we clip it to 100.
   CNcomment:大于100的值按100处理 CNend
   \param[in] enDisp          DISP channel ID.CNcomment:DISP通道号 CNend
   \param[in] u32Contrast    contrast value. the range is 0~100, 0 means the min contrast value. \n
   CNcomment:待设置的显示输出对比度值。取值范围为0～100。0：最小对比度；100：最大对比度 CNend
   \retval ::HI_SUCCESS  success.CNcomment:成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:操作非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_SetContrast( HI_UNF_DISP_E enDisp, HI_U32 u32Contrast );


/**
   \brief  get DISP contrast value.CNcomment:获取DISP对比度 CNend
   \attention \n
   In the case of HD atach to SD. the Z order configuration of HD will auto sync to SD, also the configuration of SD will auto sync to HD.\n
   CNcomment:查询的默认对比度值为50。\n
   在高标清绑定情况下，高清DISP的设置会自动同步到标清DISP；标清DISP的设置会自动同步到高清DISP。 CNend
   \param[in] enDisp           DISP channel ID.CNcomment:DISP通道号 CNend
   \param[out] pu32Contrast   pointer of contrast. CNcomment:指针类型，显示输出对比度值 CNend
   \retval ::HI_SUCCESS  success.CNcomment:成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:操作非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_GetContrast( HI_UNF_DISP_E enDisp, HI_U32 *pu32Contrast );


/**
   \brief   set DISP saturation.CNcomment:设置DISP饱和度 CNend
   \attention \n
   If the value is more than 100, we clip it to 100.
   CNcomment:大于100的值按100处理 CNend
   \param[in] enDisp             DISP channel ID.CNcomment:DISP通道号 CNend
   \param[in] u32Saturation      saturation value. the range is 0~100, 0 means the min saturation value. \n
   CNcomment:待设置的显示输出饱和度值。取值范围为0～100。0：最小饱和度；100：最大饱和度 CNend
   \retval ::HI_SUCCESS CNcomment: success.成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:操作非法 CNend
   \see \n
   none.CNcomment:无 CNend
*/
HI_S32 HI_UNF_DISP_SetSaturation( HI_UNF_DISP_E enDisp, HI_U32 u32Saturation );


/**
   \brief   get DISP saturation.CNcomment:获取DISP饱和度 CNend
   \attention \n
   In the case of HD atach to SD. the Z order configuration of HD will auto sync to SD, also the configuration of SD will auto sync to HD.\n
   CNcomment:查询的默认饱和度值为50。\n
   在高标清绑定情况下，高清DISP的设置会自动同步到标清DISP；标清DISP的设置会自动同步到高清DISP。 CNend
   \param[in] enDisp                 DISP channel ID.CNcomment:DISP通道号 CNend
   \param[out] pu32Saturation    pointer of saturation. CNcomment:指针类型，显示输出饱和度值 CNend
   \retval ::HI_SUCCESS CNcomment: success.成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:操作非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_GetSaturation( HI_UNF_DISP_E enDisp, HI_U32 *pu32Saturation );


/**
   \brief  set DISP hueplus.CNcomment:设置DISP色调 CNend
   \attention \n
   none.CNcomment:无 CNend
   \param[in] enDisp              DISP channel ID.CNcomment:DISP通道号 CNend
   \param[in] u32HuePlus       hueplus value. the range is 0~100, 0 means the min hueplus value. \n
   CNcomment:显示输出色调增益值。范围为0～100。0：表示最小色调增益；100：表示最大色调增益 CNend
   \retval ::HI_SUCCESS CNcomment: success.成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:操作非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_SetHuePlus( HI_UNF_DISP_E enDisp, HI_U32 u32HuePlus );


/**
   \brief get DISP hueplus.CNcomment:获取DISP色调 CNend
   \attention \n
   In the case of HD atach to SD. the Z order configuration of HD will auto sync to SD, also the configuration of SD will auto sync to HD.\n
   CNcomment:查询的默认色调值为50。\n
   在高标清绑定情况下，高清DISP的设置会自动同步到标清DISP；标清DISP的设置会自动同步到高清DISP。 CNend
   \param[in] enDisp          DISP channel ID.CNcomment: DISP通道号 CNend
   \param[out] pu32HuePlus     pointer of hueplus. CNcomment:指针类型，显示输出色调增益值 CNend
   \retval ::HI_SUCCESS CNcomment: success.成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:操作非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_GetHuePlus( HI_UNF_DISP_E enDisp, HI_U32 *pu32HuePlus );


/**
   \brief  set Wss.CNcomment:直接设置WSS（Wide Screen Singnalling）数据 CNend
   \attention \n
   none. CNcomment:无 CNend
   \param[in] enDisp           DISP channel ID.CNcomment:DISP通道号 CNend
   \param[in] pstWssData        pointer of Wss data description structure.CNcomment:指针类型，指向WSS数据的指针 CNend
   \retval ::HI_SUCCESS CNcomment: success.成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:操作非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_SetWss( HI_UNF_DISP_E enDisp, const HI_UNF_DISP_WSS_DATA_S *pstWssData );


/**
   \brief  set Macrovision mode.CNcomment:设置Macrovision模式 CNend
   \attention \n
   none.CNcomment:无 CNend
   \param[in] enDisp          DISP channel ID.CNcomment:DISP通道号 CNend
   \param[in] enMode        Macrovision mode.  CNcomment:Macrovision模式，请参见::HI_UNF_MACROVISION_MODE_E CNend
   \param[in] pData          pointer of Macrovision control data CNcomment: 指针类型，自定义的Macrovision控制数据 CNend
   \retval ::HI_SUCCESS CNcomment: success.成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:操作非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_SetMacrovision( HI_UNF_DISP_E enDisp, HI_UNF_DISP_MACROVISION_MODE_E enMode, const HI_VOID *pData );


/**
   \brief get Macrovision mode.CNcomment: 获取Macrovision模式 CNend
   \attention \n
   none. CNcomment:无 CNend
   \param[in] enDisp      DISP channel ID.CNcomment:DISP通道号 CNend
   \param[out] penMode    pointer of Macrovision mode. CNcomment:指针类型，MACROVISION模式 CNend
   \param[out] pData      a data pointer only valid whenpenMode=HI_MACROVISION_MODE_CUSTOM.\n
   CNcomment:指针类型，仅当penMode=HI_MACROVISION_MODE_CUSTOM时有效 CNend
   \retval ::HI_SUCCESS CNcomment: success.成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:操作非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_GetMacrovision( HI_UNF_DISP_E enDisp, HI_UNF_DISP_MACROVISION_MODE_E *penMode, const HI_VOID *pData );


/**
   \brief   set CGMS data.CNcomment:设置CGMS（Copy Generation Management System）数据 CNend
   \attention \n
   none. CNcomment:无 CNend
   \param[in] enDisp                   DISP channel ID.CNcomment:DISP通道号 CNend
   \param[in] pstCgmsCfg             pointer of CGMS configuration  CNcomment:指针类型，指向CGMS配置数据的指针 CNend
   \retval ::HI_SUCCESS CNcomment: success.成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:操作非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_SetCgms( HI_UNF_DISP_E enDisp, const HI_UNF_DISP_CGMS_CFG_S *pstCgmsCfg );


/**
   \brief chip do or not support Macrovision.CNcomment: 获取芯片是否支持Macrovision CNend
   \attention \n
   none.CNcomment:无 CNend
   \param[in] enDisp      DISP channel ID.CNcomment:DISP通道号 CNend
   \param[out] pbSupport    pointer of Macrovision flag. CNcomment:指针类型，Macrovision 标志 CNend
   \retval ::HI_SUCCESS CNcomment: success.成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:操作非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_GetMacrovisionSupport( HI_UNF_DISP_E enDisp, HI_BOOL *pbSupport );

/**
   \brief set aspect ratio attribute of display device.CNcomment: 设置设备宽高比 CNend
   \attention \n
   If set AspectRatio USER mode ,must be set u32UserAspectWidth and u32UserAspectHeight \n
   (theWidth/Height range is 1/64~64,if set zero means using current screen AspectRatio.) \n
   If not AspectRatio USER mode ,it will be set value by enumeration means;In this mode u32UserAspectWidth and u32UserAspectHeight are invalid ;\n
   If set AspectRatio AUTO mode , use current screen AspectRatio;
   CNcomment:如果设置USER模式则需设置u32UserAspectWidth 和u32UserAspectHeight的值，\n
   赋值范围为两者比值1/64~64,如果设置0则表示使用当前屏幕的宽高比；\n
   非USER模式则按照对应的比值起效u32UserAspectWidth 和u32UserAspectHeight的值无效\n
   其中设置AUTO模式则设备宽高比值自动设置为当前屏幕的宽高比。 CNend
   \param[in] enDisp      DISP channel ID.CNcomment:DISP通道号 CNend
   \param[in] pstDispAspectRatio    pointer of aspect ratio attribute of display device . CNcomment:指针类型，显示设备宽高比信息 CNend
   \retval ::HI_SUCCESS CNcomment: success.成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:操作非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_SetAspectRatio( HI_UNF_DISP_E enDisp, HI_UNF_DISP_ASPECT_RATIO_S *pstDispAspectRatio );


/**
   \brief get aspect ratio attribute of display device.CNcomment: 获得设备宽高比属性 CNend
   \attention \n
   none.
   CNcomment:无 CNend
   \param[in] enDisp      DISP channel ID.CNcomment:DISP通道号 CNend
   \param[out] pstDispAspectRatio    pointer of aspect ratio attribute of display device . CNcomment:指针类型，显示设备宽高比信息 CNend
   \retval ::HI_SUCCESS CNcomment: success.成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:操作非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_GetAspectRatio( HI_UNF_DISP_E enDisp, HI_UNF_DISP_ASPECT_RATIO_S *pstDispAspectRatio );


/**
   \brief set algorithmic attribute of display device.CNcomment: 设置设备算法属性 CNend
   \attention \n
   none. CNcomment:无 CNend
   \param[in] enDisp      DISP channel ID.CNcomment:DISP通道号 CNend
   \param[in] pstAlg    pointer of algorithmic attribute of display device . CNcomment:指针类型，显示设备算法属性 CNend
   \retval ::HI_SUCCESS CNcomment: success.成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:操作非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_SetAlgCfg( HI_UNF_DISP_E enDisp, HI_UNF_DISP_ALG_CFG_S *pstAlg );


/**
   \brief get algorithmic status of display device.CNcomment: 获取设备算法属性 CNend
   \attention \n
   none. CNcomment:无 CNend
   \param[in] enDisp      DISP channel ID.CNcomment:DISP通道号 CNend
   \param[out] pstAlg    pointer of algorithmic attribute of display device . CNcomment:指针类型，显示设备算法属性 CNend
   \retval ::HI_SUCCESS CNcomment: success.成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:操作非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_GetAlgCfg( HI_UNF_DISP_E enDisp, HI_UNF_DISP_ALG_CFG_S *pstAlg );


/**
   \brief create VBI data channel.CNcomment: 创建VBI数据通道 CNend
   \attention \n
   Each VBI type can not  created  repeatedly,if created more than one time,it will be return HI_ERR_DISP_CREATE_ERR;
   CNcomment:每种VBI类型只能创建一次，多次创建则返回HI_ERR_DISP_CREATE_ERR CNend
   \param[in] enDisp      DISP channel ID.CNcomment:DISP通道号 CNend
   \param[in] pstCfg    pointer of parameter of VBI channel . CNcomment:指针类型，VBI通道参数 CNend
   \param[out] phVbi    pointer of VBI handle . CNcomment:指针类型，VBI通道句柄 CNend
   \retval ::HI_SUCCESS CNcomment: success.成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:操作非法 CNend
   \retval ::HI_ERR_DISP_CREATE_ERR    invalid operation. CNcomment:创建失败 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_CreateVBI( HI_UNF_DISP_E enDisp, HI_UNF_DISP_VBI_CFG_S *pstCfg, HI_HANDLE *phVbi );


/**
   \brief destroy VBI data channel.CNcomment: 销毁VBI数据通道 CNend
   \attention \n
   none.
   CNcomment:无 CNend
   \param[in] hVbi      VBI handle ID.CNcomment:VBI 通道句柄 CNend
   \retval ::HI_SUCCESS CNcomment: success.成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:操作非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_DestroyVBI( HI_HANDLE hVbi );


/**
   \brief send data to vbi channel.CNcomment: 发送数据到VBI通道 CNend
   \attention \n
   After  call HI_UNF_DISP_CreateVBI creat VBI handle sucess,you can call API HI_UNF_DISP_SendVBIData send VBI data .
   CNcomment:调用HI_UNF_DISP_CreateVBI 创建VBI通道成功后，才能调用HI_UNF_DISP_SendVBIData  发送数据 CNend
   \param[in] hVbi      VBI handle ID.CNcomment:VBI 通路句柄 CNend
   \param[in] pstVbiData      pointer of VBI data.CNcomment:VBI数据指针 CNend
   \retval ::HI_SUCCESS CNcomment: success.成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:操作非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_SendVBIData( HI_HANDLE hVbi, HI_UNF_DISP_VBI_DATA_S *pstVbiData );

/**
   \brief Get default attributes of screen share channel.CNcomment: 获取屏幕投影通道的默认属性 CNend
   \attention \n
   none. CNcomment:无 CNend
   \param[in] enDisp      display channel.CNcomment:显示通路 CNend
   \param[out]pstAttr     pointer of parameter .CNcomment:指针,配置参数 CNend
   \retval ::HI_SUCCESS CNcomment: success.成功 CNend
   \retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_GetDefaultCastAttr(HI_UNF_DISP_E enDisp,HI_UNF_DISP_CAST_ATTR_S *pstAttr);


/**
   \brief create screen share channel.CNcomment: 创建屏幕投影通道 CNend
   \attention \n
   none. CNcomment:无 CNend
   \param[in] enDisp      display channel.CNcomment:显示通路 CNend
   \param[in] pstAttr      pointer of parameter .CNcomment:指针,配置参数 CNend
   \param[out] phCast      handle of screen share .CNcomment:屏幕投影句柄 CNend
   \retval ::HI_SUCCESS CNcomment: success.成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:操作非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_CreateCast( HI_UNF_DISP_E enDisp, HI_UNF_DISP_CAST_ATTR_S * pstAttr, HI_HANDLE *phCast );


/**
   \brief destroy screen share channel.CNcomment: 销毁屏幕投影通道 CNend
   \attention \n
   none. CNcomment:无 CNend
   \param[in] phCast      handle of screen share .CNcomment:屏幕投影句柄 CNend
   \retval ::HI_SUCCESS CNcomment: success.成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:操作非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_DestroyCast( HI_HANDLE hCast );


/**
   \brief enable screen share.CNcomment: 使能屏幕投影功能 CNend
   \attention \n
   none. CNcomment:无 CNend
   \param[in] phCast      handle of screen share .CNcomment:屏幕投影句柄 CNend
   \param[in] bEnable      enable screen share .CNcomment:使能屏幕投影 CNend
   \retval ::HI_SUCCESS CNcomment: success.成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:操作非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_SetCastEnable( HI_HANDLE hCast, HI_BOOL bEnable );


/**
   \brief get enable flag of screen share.CNcomment: 获取屏幕投影是否使能 CNend
   \attention \n
   none. CNcomment:无 CNend
   \param[in] phCast      handle of screen share .CNcomment:屏幕投影句柄 CNend
   \param[out] bEnable     flag .CNcomment:标志 CNend
   \retval ::HI_SUCCESS CNcomment: success.成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:操作非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_GetCastEnable( HI_HANDLE hCast, HI_BOOL *pbEnable );


/**
   \brief get frame info of screen share.CNcomment: 获取屏幕投影帧信息 CNend
   \attention \n
   none. CNcomment:无 CNend
   \param[in] hCast      handle of screen share .CNcomment:屏幕投影句柄 CNend
   \param[out] pstFrameInfo        frame info.CNcomment:帧信息 CNend
   \param[in] u32TimeoutMs wait time in ms.CNcomment:等待时间，毫秒为单位 CNend
   \retval ::HI_SUCCESS CNcomment: success.成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:操作非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_AcquireCastFrame(HI_HANDLE hCast, HI_UNF_VIDEO_FRAME_INFO_S *pstFrameInfo, HI_U32 u32TimeoutMs);


/**
   \brief release frame info of screen share.CNcomment: 释放屏幕投影帧信息 CNend
   \attention \n
   none. CNcomment:无 CNend
   \param[in] hCast      handle of screen share .CNcomment:屏幕投影句柄 CNend
   \param[in] pstFrameInfo     frame info.CNcomment:帧信息 CNend
   \retval ::HI_SUCCESS CNcomment: success.成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:操作非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_ReleaseCastFrame( HI_HANDLE hCast, HI_UNF_VIDEO_FRAME_INFO_S *pstFrameInfo );


/**
   \brief professional color modulation.CNcomment: 专业色彩调节 CNend
   \attention \n
   none. CNcomment:无 CNend
   \param[in] enDisp      display channel.CNcomment:显示通路 CNend
   \param[in] pstCS      struct of color modulation.CNcomment:色彩调节结构 CNend
   \retval ::HI_SUCCESS CNcomment: success.成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:操作非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_SetColor( HI_UNF_DISP_E enDisp, HI_UNF_DISP_COLOR_SETTING_S *pstCS );


/**
   \brief get color modulation parameter.CNcomment: 获取色彩调节信息 CNend
   \attention \n
   none. CNcomment:无 CNend
   \param[in] enDisp      display channel.CNcomment:显示通路 CNend
   \param[in] pstCS      struct of color modulation.CNcomment:色彩调节结构 CNend
   \retval ::HI_SUCCESS CNcomment: success.成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:操作非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_GetColor( HI_UNF_DISP_E enDisp, HI_UNF_DISP_COLOR_SETTING_S *pstCS );


/**
   \Snapshot truncation screen connection, intercepts the screen complete picture;CNcomment:截取屏幕完整画面; CNend
   \attention \n
   none. CNcomment:无 CNend
   \param[in] enDisp      display channel.CNcomment:显示通路 CNend
   \param[in] pstFrameInfo     frame info.CNcomment:帧信息 CNend
   \retval ::HI_SUCCESS CNcomment: success.成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:操作非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_AcquireSnapshot(HI_UNF_DISP_E enDisp, HI_UNF_VIDEO_FRAME_INFO_S * pstFrameInfo);

/**
   \Release Snapshot picture;CNcomment:释放截屏画面; CNend
   \attention \n
   none. CNcomment:无 CNend
   \param[in] enDisp      display channel.CNcomment:显示通路 CNend
   \param[in] pstFrameInfo     frame info.CNcomment:帧信息 CNend
   \retval ::HI_SUCCESS CNcomment: success.成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:操作非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_ReleaseSnapshot(HI_UNF_DISP_E enDisp, HI_UNF_VIDEO_FRAME_INFO_S * pstFrameInfo);


/**
   \brief get color modulation parameter.CNcomment:设置DISP的默认属性 CNend
   \attention \n
   none. CNcomment:无 CNend
   \param[in] enDisp      display channel.CNcomment:显示通路 CNend
   \retval ::HI_SUCCESS CNcomment: success.成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:输入指针为空 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:操作非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_SetDefaultPara( HI_UNF_DISP_E enDisp);


/**
   \brief set DISP 3D format.CNcomment:设置DISP的3D制式 CNend
   \attention \n
   none. CNcomment:无 CNend
   \param[in] enDisp           DISP channel ID.CNcomment:DISP通道号 CNend
   \param[in] en3D             DISP 3D mode.CNcomment:DISP 3D模式 CNend
   \param[in] enEncodingFormat DISP format.CNcomment:DISP的制式 CNend
   \retval ::HI_SUCCESS  success.CNcomment:成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_Set3DMode(HI_UNF_DISP_E enDisp, HI_UNF_DISP_3D_E en3D, HI_UNF_ENC_FMT_E enEncFormat);

/**
   \brief get DISP 3D format.CNcomment:查询DISP的3D制式 CNend
   \attention \n
   none. CNcomment:无 CNend
   \param[in] enDisp           DISP channel ID.CNcomment:DISP通道号 CNend
   \param[out] pen3D           DISP 3D mode.CNcomment:DISP 3D模式 CNend
   \param[out] penEncFormat    DISP format.CNcomment:DISP的制式 CNend
   \retval ::HI_SUCCESS  success.CNcomment:成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_Get3DMode(HI_UNF_DISP_E enDisp, HI_UNF_DISP_3D_E *pen3D, HI_UNF_ENC_FMT_E *penEncFormat);

/**
   \brief set right eye first for 3D output.CNcomment:设置3D输出右眼优先 CNend
   \attention \n
   Only take effect in 3D output mode. CNcomment: 仅在3D输出时有效 CNend
   \param[in] enDisp     DISP channel ID.CNcomment:DISP通道号 CNend
   \param[in] bEnable   Right-Eye-First.CNcomment:右眼优先 CNend
   \retval ::HI_SUCCESS  success.CNcomment:成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_SetRightEyeFirst(HI_UNF_DISP_E enDisp, HI_BOOL bEnable);

/**
   \brief set virtual screen size of display.CNcomment:设置显示通道的虚拟屏幕大小 CNend
   \attention \n
   if not set ,default value is 1280*720. CNcomment: 如果不设置此函数，默认的虚拟屏幕大小是720p. CNend
   \param[in] enDisp   DISP channel ID.CNcomment:DISP通道号 CNend
   \param[in] u32Width  virtual screen width, the range is [480, 3840].CNcomment:虚拟屏幕宽度，取值范围为[480, 3840]. CNend
   \param[in] u32Height  virtual screen height, the range is [480, 3840].CNcomment:虚拟屏幕高度，取值范围为[480, 3840]. CNend
   \retval ::HI_SUCCESS  success.CNcomment:成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_SetVirtualScreen(HI_UNF_DISP_E enDisp, HI_U32 u32Width, HI_U32 u32Height);

/**
   \brief get virtual screen size of display.CNcomment:获取显示通道的虚拟屏幕大小 CNend
   \attention \n
   none. CNcomment: 无 CNend
   \param[in] enDisp   DISP channel ID.CNcomment:DISP通道号 CNend
   \param[out] u32Width  virtual screen width, the range is [480, 3840].CNcomment:虚拟屏幕宽度，取值范围为[480, 3840]. CNend
   \param[out] u32Height  virtual screen height, the range is [480, 3840].CNcomment:虚拟屏幕高度，取值范围为[480, 3840]. CNend
   \retval ::HI_SUCCESS  success.CNcomment:成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_GetVirtualScreen(HI_UNF_DISP_E enDisp, HI_U32 *u32Width, HI_U32 *u32Height);


/**
   \brief set offset of display area in real screen.CNcomment:设置显示区域在实体屏幕上的偏移量,为物理像素点 CNend
   \attention \n
   must be 4 pixels aligned,if not set, the default value is 0. CNcomment: offset 值必须是4对齐,如果不设置默认的值为0. CNend
   \param[in] enDisp   DISP channel ID.CNcomment:DISP通道号 CNend
   \param[in] pstOffset  offset in pixel, the range is [0, 200].CNcomment:偏移像素数目，取值范围为[0, 200]. CNend
   \retval ::HI_SUCCESS  success.CNcomment:成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_SetScreenOffset(HI_UNF_DISP_E enDisp, HI_UNF_DISP_OFFSET_S *pstOffset);

/**
   \brief get offset of display area in real screen.CNcomment:获取显示区域在实体屏幕上的偏移量,为物理像素点 CNend
   \attention \n
   none. CNcomment: 无 CNend
   \param[in] enDisp   DISP channel ID.CNcomment:DISP通道号 CNend
   \param[out] pstOffset  offset in pixel, the range is [0, 200].CNcomment:偏移像素数目，取值范围为[0, 200]. CNend
   \retval ::HI_SUCCESS  success.CNcomment:成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_GetScreenOffset(HI_UNF_DISP_E enDisp, HI_UNF_DISP_OFFSET_S *pstOffset);

/**
   \brief set isogeny attributes.CNcomment: 设置同源属性 CNend
   \attention \n
   none. CNcomment: 无 CNend
   \param[in] pstIsogeny isogenous attributes.CNcomment:同源模式下，要设置的属性信息 CNend
   \param[in] u32ChannelNum  channel counts to be set in isogenous mode.CNcomment:同源模式下，要设置属性的通道个数 CNend
   \retval ::HI_SUCCESS  success.CNcomment:成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_SetIsogenyAttr(const HI_UNF_DISP_ISOGENY_ATTR_S *pstIsogeny, const HI_U32 u32ChannelNum);

/**
   \brief set display output type.CNcomment: 设置显示输出类型 CNend
   \attention \n
   none. CNcomment: 无 CNend
   \param[in] enDisp   DISP channel ID.CNcomment:DISP通道号 CNend
   \param[in] enHDRType   DISP type.CNcomment:DISP类型CNend
   \retval ::HI_SUCCESS  success.CNcomment:成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_SetHDRType(HI_UNF_DISP_E enDisp, const HI_UNF_DISP_HDR_TYPE_E enHDRType);

/**
   \brief Get display output type.CNcomment: 获取显示输出类型 CNend
   \attention \n
   none. CNcomment: 无 CNend
   \param[in] enDisp   DISP channel ID.CNcomment:DISP通道号 CNend
   \param[in] penHDRType   DISP type.CNcomment:DISP类型CNend
   \retval ::HI_SUCCESS  success.CNcomment:成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_GetHDRType(HI_UNF_DISP_E enDisp, HI_UNF_DISP_HDR_TYPE_E *penHDRType);

/**
   \brief set display output type.CNcomment: 设置显示输出是否支持BT2020 CNend
   \attention \n
   none. CNcomment: 无 CNend
   \param[in] enDisp   DISP channel ID.CNcomment:DISP通道号 CNend
   \param[in] bEnable   BT2020  support.CNcomment:DISP类型CNend
   \retval ::HI_SUCCESS  success.CNcomment:成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_SetBT2020Enable(HI_UNF_DISP_E enDisp, HI_BOOL bEnable);

/**
   \brief Get display output type.CNcomment: 获取显示输出是否支持BT2020 CNend
   \attention \n
   none. CNcomment: 无 CNend
   \param[in] enDisp   DISP channel ID.CNcomment:DISP通道号 CNend
   \param[in] pbEnable   BT2020  support.CNcomment:DISP类型CNend
   \retval ::HI_SUCCESS  success.CNcomment:成功 CNend
   \retval ::HI_ERR_DISP_NO_INIT    DISP uninitialization.CNcomment:DISP未初始化 CNend
   \retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:输入参数非法 CNend
   \see \n
   none.CNcomment:无 CNend
 */
HI_S32 HI_UNF_DISP_GetBT2020Enable(HI_UNF_DISP_E enDisp, HI_BOOL *pbEnable);


/** @} */  /** <!-- ==== API declaration end ==== */
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif
/************************************** The End Of File **************************************/
