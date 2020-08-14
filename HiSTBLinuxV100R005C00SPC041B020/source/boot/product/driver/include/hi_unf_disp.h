/******************************************************************************
  Copyright (C), 2004-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
  File Name     : optm_disp.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2012/8/15
  Last Modified :
  Description   : header file for video output control
  Function List :
  History       :
  1.Date        :
  Author        :
  Modification  : Created file
******************************************************************************/

#ifndef __OPTM_DISP_H__
#define __OPTM_DISP_H__

#include "hi_type.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */


/**max DAC count*/
/**CNcomment:最大的DAC数目*/
#ifndef MAX_DAC_NUM
#define MAX_DAC_NUM (6)
#endif

/**enum define about DISPLAY channel*/
/**CNcomment:定义DISPLAY通道枚举*/
typedef enum hiUNF_DISP_E
{
    HI_UNF_DISPLAY0 = 0x0,  /**< DISPLAY0,Compatible with SD */ /**<CNcomment:高清DISPLAY0，兼容标清 */
    HI_UNF_DISPLAY1,         /**< DISPLAY1 *//**<CNcomment: 高清DISPLAY1 */
    HI_UNF_DISPLAY2,         /**< DISPLAY2 *//**<CNcomment: 高清DISPLAY2 */
    HI_UNF_DISPLAY_BUTT
}HI_UNF_DISP_E;

/**define the sigal type of DAC output*/
/**CNcomment:定义支持的DAC信号格式*/
typedef enum hiUNF_DISP_DAC_MODE_E
{
    HI_UNF_DISP_DAC_MODE_SILENCE = 0,               /**<No signal */ /**<CNcomment:无输出*/
    HI_UNF_DISP_DAC_MODE_CVBS,                      /**<CVBS output */ /**<CNcomment:CVBS输出*/
    HI_UNF_DISP_DAC_MODE_Y,                         /**<Y component signal output*//**<CNcomment:Y亮度分量输出*/
    HI_UNF_DISP_DAC_MODE_PB,                        /**<PB component signal output*//**<CNcomment:蓝色色差分量输出*/
    HI_UNF_DISP_DAC_MODE_PR,                        /**<PR componet signal output *//**<CNcomment:红色色差分量输出*/
    HI_UNF_DISP_DAC_MODE_R,                         /**< Red component signal output*//**<CNcomment:CNcomment:红色分量输出*/
    HI_UNF_DISP_DAC_MODE_G,                         /**<green component signal output*//**<CNcomment:绿色分量输出*/
    HI_UNF_DISP_DAC_MODE_G_NOSYNC,                  /**<green component signal output， without sync signal*//**<CNcomment:绿色分量输出, 无同步信号*/
    HI_UNF_DISP_DAC_MODE_B,                         /**<blue component signal output*//**<CNcomment:蓝色分量输出*/
    HI_UNF_DISP_DAC_MODE_SVIDEO_Y,                  /**<SVIDEO Y signal output*//**<CNcomment:SVIDEO_Y分量输出*/
    HI_UNF_DISP_DAC_MODE_SVIDEO_C,                  /**<SVIDEO C signal output *//**<CNcomment:SVIDEO_C分量输出*/

    HI_UNF_DISP_DAC_MODE_BUTT
}HI_UNF_DISP_DAC_MODE_E;

typedef enum hiUNF_ENC_FMT_E
{
    HI_UNF_ENC_FMT_1080P_60 = 0,     /**<1080p 60 Hz*/
    HI_UNF_ENC_FMT_1080P_50,         /**<1080p 50 Hz*/
    HI_UNF_ENC_FMT_1080P_30,         /**<1080p 30 Hz*/
    HI_UNF_ENC_FMT_1080P_25,         /**<1080p 25 Hz*/
    HI_UNF_ENC_FMT_1080P_24,         /**<1080p 24 Hz*/

    HI_UNF_ENC_FMT_1080i_60,         /**<1080i 60 Hz*/
    HI_UNF_ENC_FMT_1080i_50,         /**<1080i 50 Hz*/

    HI_UNF_ENC_FMT_720P_60,          /**<720p 60 Hz*/
    HI_UNF_ENC_FMT_720P_50,          /**<720p 50 Hz */

    HI_UNF_ENC_FMT_576P_50,          /**<576p 50 Hz*/
    HI_UNF_ENC_FMT_480P_60,          /**<480p 60 Hz*/

    HI_UNF_ENC_FMT_PAL,              /* B D G H I PAL */
    HI_UNF_ENC_FMT_PAL_N,            /* (N)PAL        */
    HI_UNF_ENC_FMT_PAL_Nc,           /* (Nc)PAL       */

    HI_UNF_ENC_FMT_NTSC,             /* (M)NTSC       */
    HI_UNF_ENC_FMT_NTSC_J,           /* NTSC-J        */
    HI_UNF_ENC_FMT_NTSC_PAL_M,       /* (M)PAL        */

    HI_UNF_ENC_FMT_SECAM_SIN,        /**< SECAM_SIN*/
    HI_UNF_ENC_FMT_SECAM_COS,        /**< SECAM_COS*/

    HI_UNF_ENC_FMT_1080P_24_FRAME_PACKING,
    HI_UNF_ENC_FMT_720P_60_FRAME_PACKING,
    HI_UNF_ENC_FMT_720P_50_FRAME_PACKING,

    HI_UNF_ENC_FMT_861D_640X480_60,
    HI_UNF_ENC_FMT_VESA_800X600_60,
    HI_UNF_ENC_FMT_VESA_1024X768_60,
    HI_UNF_ENC_FMT_VESA_1280X720_60,
    HI_UNF_ENC_FMT_VESA_1280X800_60,
    HI_UNF_ENC_FMT_VESA_1280X1024_60,
    HI_UNF_ENC_FMT_VESA_1360X768_60,         //Rowe
    HI_UNF_ENC_FMT_VESA_1366X768_60,
    HI_UNF_ENC_FMT_VESA_1400X1050_60,        //Rowe
    HI_UNF_ENC_FMT_VESA_1440X900_60,
    HI_UNF_ENC_FMT_VESA_1440X900_60_RB,
    HI_UNF_ENC_FMT_VESA_1600X900_60_RB,
    HI_UNF_ENC_FMT_VESA_1600X1200_60,
    HI_UNF_ENC_FMT_VESA_1680X1050_60,
    HI_UNF_ENC_FMT_VESA_1680X1050_60_RB,  //
    HI_UNF_ENC_FMT_VESA_1920X1080_60,
    HI_UNF_ENC_FMT_VESA_1920X1200_60,
    HI_UNF_ENC_FMT_VESA_1920X1440_60,//
    HI_UNF_ENC_FMT_VESA_2048X1152_60,
    HI_UNF_ENC_FMT_VESA_2560X1440_60_RB,//
    HI_UNF_ENC_FMT_VESA_2560X1600_60_RB,//sequece:42

    HI_UNF_ENC_FMT_3840X2160_24 = 0x40,
    HI_UNF_ENC_FMT_3840X2160_25,
    HI_UNF_ENC_FMT_3840X2160_30,
    HI_UNF_ENC_FMT_3840X2160_50,
    HI_UNF_ENC_FMT_3840X2160_60,

    HI_UNF_ENC_FMT_4096X2160_24,
    HI_UNF_ENC_FMT_4096X2160_25,
    HI_UNF_ENC_FMT_4096X2160_30,
    HI_UNF_ENC_FMT_4096X2160_50,
    HI_UNF_ENC_FMT_4096X2160_60,

    HI_UNF_ENC_FMT_3840X2160_23_976,
    HI_UNF_ENC_FMT_3840X2160_29_97,
    HI_UNF_ENC_FMT_720P_59_94,
    HI_UNF_ENC_FMT_1080P_59_94,
    HI_UNF_ENC_FMT_1080P_29_97,
    HI_UNF_ENC_FMT_1080P_23_976,
    HI_UNF_ENC_FMT_1080i_59_94,
    HI_UNF_ENC_FMT_BUTT
}HI_UNF_ENC_FMT_E;

/**Defines the device aspect ratio.*/
/**CNcomment: 定义设备宽高比枚举*/
typedef enum hiUNF_DISP_ASPECT_RATIO_E
{
    HI_UNF_DISP_ASPECT_RATIO_AUTO,                              /**<aspect ratio as device Resolution*/ /**<CNcomment: 宽高比与设备分辨率一致*/
    HI_UNF_DISP_ASPECT_RATIO_4TO3,                              /**<4:3*/ /**<CNcomment: 4比3*/
    HI_UNF_DISP_ASPECT_RATIO_16TO9,                             /**<16:9*/ /**<CNcomment: 16比9*/
    HI_UNF_DISP_ASPECT_RATIO_221TO1,                            /**<221:100*/ /**<CNcomment: 221比100*/
    HI_UNF_DISP_ASPECT_RATIO_USER,                              /**<user define*/ /**<CNcomment: 用户定义*/

    HI_UNF_DISP_ASPECT_RATIO_BUTT
}HI_UNF_DISP_ASPECT_RATIO_E;


/**Defines the device aspect ratio struct.*/
/**CNcomment: 定义设备宽高比结构*/
typedef struct hiUNF_DISP_ASPECT_RATIO_S
{
    HI_UNF_DISP_ASPECT_RATIO_E          enDispAspectRatio;      /**<aspect ratio type of device*/ /**<CNcomment: 设备宽高比类型*/
    HI_U32                              u32UserAspectWidth;     /**<user define width of device*/ /**<CNcomment: 用户定义设备宽度*/
    HI_U32                              u32UserAspectHeight;    /**<user define height of device*/ /**<CNcomment: 用户定义设备高度*/
}HI_UNF_DISP_ASPECT_RATIO_S;

/********************************ENCODER STRUCT********************************/
/** define the enum of Macrovision output type*/
/** CNcomment:显示输出Macrovision模式枚举定义*/
typedef enum hiUNF_DISP_MACROVISION_MODE_E
{
    HI_UNF_DISP_MACROVISION_MODE_TYPE0,  /**<type 0 *//**<CNcomment:典型配置0 */
    HI_UNF_DISP_MACROVISION_MODE_TYPE1,  /**<type 1 *//**<CNcomment:典型配置1 */
    HI_UNF_DISP_MACROVISION_MODE_TYPE2,  /**<type 2  *//**<CNcomment:典型配置2 */
    HI_UNF_DISP_MACROVISION_MODE_TYPE3,  /**<type 3  *//**<CNcomment:典型配置3 */
    HI_UNF_DISP_MACROVISION_MODE_CUSTOM, /**<type of configure by user  *//**<CNcomment:用户自定义配置 */
    HI_UNF_DISP_MACROVISION_MODE_BUTT
} HI_UNF_DISP_MACROVISION_MODE_E;

/* CGMS type select */
/*CNcomment:CGMS 类型选择*/
typedef enum hiUNF_DISP_CGMS_TYPE_E
{
    HI_UNF_DISP_CGMS_TYPE_A = 0x00,     /* CGMS type  A*//*CNcomment:CGMS 类型A*/
    HI_UNF_DISP_CGMS_TYPE_B,                /* CGMS type  B*//*CNcomment:CGMS 类型B*/

    HI_UNF_DISP_CGMS_TYPE_BUTT
}HI_UNF_DISP_CGMS_TYPE_E;



/* definition of CGMS mode */
typedef enum hiUNF_DISP_CGMS_MODE_E
{
   HI_UNF_DISP_CGMS_MODE_COPY_FREELY    = 0,    /* copying is permitted without restriction */
   HI_UNF_DISP_CGMS_MODE_COPY_NO_MORE   = 0x01, /* No more copies are allowed (one generation copy has been made) */
   HI_UNF_DISP_CGMS_MODE_COPY_ONCE      = 0x02, /* One generation of copies may be made */
   HI_UNF_DISP_CGMS_MODE_COPY_NEVER     = 0x03, /* No copying is permitted */

   HI_UNF_DISP_CGMS_MODE_BUTT
}HI_UNF_DISP_CGMS_MODE_E;


/** definition of CGMS configuration */
typedef struct hiUNF_DISP_CGMS_CFG_S
{
    HI_BOOL                  bEnable;    /** HI_TRUE:CGMS is enabled; HI_FALSE:CGMS is disabled */
    HI_UNF_DISP_CGMS_TYPE_E  enType;     /** type-A or type-B or None(BUTT) */
    HI_UNF_DISP_CGMS_MODE_E  enMode;     /** CGMS mode. */

}HI_UNF_DISP_CGMS_CFG_S;

typedef enum hiUNF_HDMI_ID_E
{
    HI_UNF_HDMI_ID_0,
    HI_UNF_HDMI_ID_BUTT
}HI_UNF_HDMI_ID_E;

/*bt1120 interface ID *//**CNcomment:bt1120 接口ID  */
typedef enum hiUNF_DISP_BT1120_E
{
    HI_UNF_DISP_BT1120_0,                                   /**<BT1120 interface 0 *//**CNcomment:< BT1120接口0 */
    HI_UNF_DISP_BT1120_BUTT,
}HI_UNF_DISP_BT1120_E;

/*BT656 interface ID *//**CNcomment:BT656 接口ID  */
typedef enum hiUNF_DISP_BT656_E
{
    HI_UNF_DISP_BT656_0,                                    /**<BT656 interface 0 *//**CNcomment:< BT656接口0 */
    HI_UNF_DISP_BT656_BUTT,
}HI_UNF_DISP_BT656_E;

/*LCD interface ID *//**CNcomment:LCD 接口ID  */
typedef enum hiUNF_LCD_E
{
    HI_UNF_DISP_LCD_0,                                    /**<LCD interface 0 *//**CNcomment:< LCD接口0 */
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

/**define the VGA type struct of interface*/
/**CNcomment:定义VGA接口结构*/
typedef struct  hiUNF_DISP_INTF_VGA_S
{
    HI_U8  u8DacR;            /**<DAC num of R  *//**<CNcomment:R分量dac端口号*/
    HI_U8  u8DacG;            /**<DAC num of G  *//**<CNcomment:G分量dac端口号*/
    HI_U8  u8DacB;            /**<DAC num of B  *//**<CNcomment:B分量dac端口号*/
}HI_UNF_DISP_INTF_VGA_S;

/**define the YPBPR type struct of interface*/
/**CNcomment:定义YPBPR接口结构*/
typedef struct  hiUNF_DISP_INTF_YPBPR_S
{
    HI_U8 u8DacY;                               /**<DAC num of Y  *//**<CNcomment:Y分量dac端口号*/
    HI_U8 u8DacPb;                              /**<DAC num of Pb  *//**<CNcomment:Pb分量dac端口号*/
    HI_U8 u8DacPr;                              /**<DAC num of Pr  *//**<CNcomment:Pr分量dac端口号*/
}HI_UNF_DISP_INTF_YPBPR_S;

/**define the RGB type struct of interface*/
/**CNcomment:定义RGB接口结构*/
typedef struct  hiUNF_DISP_INTF_RGB_S
{
    HI_U8   u8DacR;                             /**<DAC num of R  *//**<CNcomment:R分量dac端口号*/
    HI_U8   u8DacG;                             /**<DAC num of G  *//**<CNcomment:G分量dac端口号*/
    HI_U8   u8DacB;                             /**<DAC num of B  *//**<CNcomment:B分量dac端口号*/
    HI_BOOL bDacSync;                           /**<G without sync signal  *//**<CNcomment:G分量不带同步信号*/
}HI_UNF_DISP_INTF_RGB_S;

/**define the CVBS type struct of interface*/
/**CNcomment:定义CVBS接口结构*/
typedef struct  hiUNF_DISP_INTF_CVBS_S
{
    HI_U8 u8Dac;                                /**<DAC num of CVBS  *//**<CNcomment:CVBS端子dac端口号*/
}HI_UNF_DISP_INTF_CVBS_S;


/**define the SVIDEO type struct of interface*/
/**CNcomment:定义SVIDEO接口结构*/
typedef struct  hiUNF_DISP_INTF_SVIDEO_S
{
    HI_U8 u8DacY;                               /**<DAC num of Y  *//**<CNcomment:Y分量dac端口号*/
    HI_U8 u8DacC;                               /**<DAC num of C  *//**<CNcomment:c分量dac端口号*/
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


typedef enum hiDISP_DAC_MODE_E
{
    DISP_DAC_MODE_SILENCE = 0,    /**<No signal */ /**<CNcomment:无信号*/
    DISP_DAC_MODE_CVBS,           /**<CVBS output */ /**<CNcomment:CVBS输出*/
    DISP_DAC_MODE_Y,              /**<Y component signal output*//**<CNcomment:Y亮度分量输出*/
    DISP_DAC_MODE_PB,             /**<PB component signal output*//**<CNcomment:蓝色色差分量输出*/
    DISP_DAC_MODE_PR,             /**<PR componet signal output *//**<CNcomment:红色色差分量输出*/
    DISP_DAC_MODE_R,              /**< Red component signal output*//**<CNcomment:CNcomment:红色分量输出*/
    DISP_DAC_MODE_G,              /**<green component signal output*//**<CNcomment:绿色分量输出*/
    DISP_DAC_MODE_B,              /**<blue component signal output*//**<CNcomment:蓝色分量输出*/
    DISP_DAC_MODE_SVIDEO_Y,       /**<SVIDEO Y signal output*//**<CNcomment:SVIDEO_Y分量输出*/
    DISP_DAC_MODE_SVIDEO_C,       /**<SVIDEO C signal output *//**<CNcomment:SVIDEO_C 分量输出*/
    DISP_DAC_MODE_HD_Y,           /**<HD Y component signal output*//**<CNcomment:高清Y亮度分量输出*/
    DISP_DAC_MODE_HD_PB,          /**<HD PB component signal output*//**<CNcomment:高清蓝色色度分量输出*/
    DISP_DAC_MODE_HD_PR,          /**< HD PR component signal output *//**<CNcomment:高清红色色度分量输出*/
    DISP_DAC_MODE_HD_R,           /**< HD red component signal output*/ /**<CNcomment:高清红色色度分量输出*/
    DISP_DAC_MODE_HD_G,           /**< HD green component signal output*//**<CNcomment:高清绿色色度分量输出*/
    DISP_DAC_MODE_HD_B,           /**< HD blue component signal output*//**<CNcomment:高清蓝色色度分量输出*/
    DISP_DAC_MODE_BUTT
}DISP_DAC_MODE_E;

/*the supported display interface.*//*CNcomment:定义支持的显示输出接口 */
typedef struct  hiDISP_INTERFACE_S
{
    HI_BOOL                      bScartEnable;
    HI_BOOL                      bBt1120Enable;
    HI_BOOL                      bBt656Enable;
    DISP_DAC_MODE_E       enDacMode[MAX_DAC_NUM];
}DISP_INTERFACE_S ;

/**define LCD output data width*/
/**CNcomment:定义LCD输出的数据位宽*/
typedef enum hiUNF_DISP_INTF_DATA_WIDTH_E
{
    HI_UNF_DISP_INTF_DATA_WIDTH8 = 0,       /**<8 bits*//**<CNcomment:8位*/
    HI_UNF_DISP_INTF_DATA_WIDTH16,          /**<16 bits*//**<CNcomment:16位*/
    HI_UNF_DISP_INTF_DATA_WIDTH24,          /**<24 bits*//**<CNcomment:24位*/
    HI_UNF_DISP_INTF_DATA_WIDTH_BUTT
}HI_UNF_DISP_INTF_DATA_WIDTH_E;


/**define the LCD data format*/
/**CNcomment:定义LCD的数据格式*/
typedef enum hiHI_UNF_DISP_INTF_DATA_FMT
{
    HI_UNF_DISP_INTF_DATA_FMT_YUV422 = 0,         /**<YUV422，data width is 16*//**<CNcomment:YUV422，位宽为16*/
    HI_UNF_DISP_INTF_DATA_FMT_RGB565 = 0x8,       /**<RGB565，data width is 16*//**<CNcomment:RGB565，位宽为16*/
    HI_UNF_DISP_INTF_DATA_FMT_RGB444 = 0xa,       /**<RGB444，data width is 16*//**<CNcomment:RGB444，位宽为16*/
    HI_UNF_DISP_INTF_DATA_FMT_RGB666 = 0xb,       /**<RGB666，data width is 24*//**<CNcomment:RGB666，位宽为24*/
    HI_UNF_DISP_INTF_DATA_FMT_RGB888 = 0xc,       /**<RGB888，data widht is 24*//**<CNcomment:RGB888，位宽为24*/
    HI_UNF_DISP_INTF_DATA_FMT_BUTT
}HI_UNF_DISP_INTF_DATA_FMT_E;

/**define the struct about color */
/**CNcomment:定义显示颜色的结构体 */
typedef struct  hiUNF_DISP_BG_COLOR_S
{
    HI_U8 u8Red ;            /**<red */ /**<CNcomment:红色分量*/
    HI_U8 u8Green ;          /**<green*//**<CNcomment:绿色分量*/
    HI_U8 u8Blue ;           /**<blue*//**<CNcomment:蓝色分量*/
} HI_UNF_DISP_BG_COLOR_S;


/**define display output range struct */
/**CNcomment:定义DISPLAY输出范围类型*/
typedef struct hiUNF_RECT_S
{
    HI_S32 x, y;
    HI_S32 w, h;
} HI_UNF_RECT_S;


/**define LCD timing */
/**CNcomment:定义LCD的时序参数*/
typedef struct hiUNF_DISP_TIMING_S
{
    HI_U32                    VFB;                 /**<vertical front blank*//**<CNcomment:垂直前消隐*/
    HI_U32                    VBB;                 /**<vertical back blank*//**<CNcomment:垂直后消隐*/
    HI_U32                    VACT;                /**<vertical active area*//**<CNcomment:垂直有效区*/
    HI_U32                    HFB;                 /**<horizonal front blank*//**<CNcomment:水平前消隐*/
    HI_U32                    HBB;                 /**<horizonal back blank*//**<CNcomment:水平后消隐*/
    HI_U32                    HACT;                /**<horizonal active area*/ /**<CNcomment:水平有效区*/
    HI_U32                    VPW;                 /**<vertical sync pluse width*//**<CNcomment:垂直脉冲宽度*/
    HI_U32                    HPW;                 /**<horizonal sync pluse width*/ /**<CNcomment:水平脉冲宽度*/
    HI_BOOL                   IDV;                 /**< flag of data valid signal is needed flip*//**<CNcomment:有效数据信号是否翻转*/
    HI_BOOL                   IHS;                 /**<flag of horizonal sync pluse is needed flip*//**<CNcomment:水平同步脉冲信号是否翻转*/
    HI_BOOL                   IVS;                 /**<flag of vertical sync pluse is needed flip*//**<CNcomment:垂直同步脉冲信号是否翻转*/
    HI_BOOL                   ClockReversal;       /**<flag of clock is needed flip*//**<CNcomment:时钟是否翻转*/
    HI_UNF_DISP_INTF_DATA_WIDTH_E DataWidth;           /**<data width*/ /**<CNcomment:数据位宽*/
    HI_UNF_DISP_INTF_DATA_FMT_E       ItfFormat;      /**<data format.*//**<CNcomment:数据格式.*/

    HI_BOOL                   DitherEnable;        /**< flag of is enable Dither*//**<CNcomment:数据格式.*/
    HI_U32                    ClkPara0;            /**<PLL  register SC_VPLL1FREQCTRL0  value *//**<CNcomment:PLL  SC_VPLL1FREQCTRL0  寄存器*/
    HI_U32                    ClkPara1;            /**<PLL  register SC_VPLL1FREQCTRL1 value*//**<CNcomment:PLL   SC_VPLL1FREQCTRL1寄存器*/

    HI_BOOL                   bInterlace;           /**<progressive or interlace*//**<CNcomment:逐行或者隔行*/
    HI_U32                    PixFreq;              /**<pixel clock*//**<CNcomment:像素时钟*/
    HI_U32                    VertFreq;             /**<display rate*//**<CNcomment:刷新率*/
    HI_U32                    AspectRatioW;         /**<width of screen*//**<CNcomment:屏幕宽度*/
    HI_U32                    AspectRatioH;         /**<height of screen*//**<CNcomment:屏幕高度*/
    HI_BOOL                   bUseGamma;            /**<gamma modulation*//**<CNcomment:伽马调节*/
    HI_U32                    Reserve0;             /**<reserved byte*//**<CNcomment:保留位*/
    HI_U32                    Reserve1;             /**<reserved byte*//**<CNcomment:保留位*/
} HI_UNF_DISP_TIMING_S;



/**define display margin stucture*/
/**CNcomment:定义显示空白区域结构体 */
typedef struct hiUNF_DISP_OFFSET_S
{
    HI_U32 u32Left;    /**<left offset *//**<CNcomment:左侧偏移*/
    HI_U32 u32Top;     /**<top offset *//**<CNcomment:上方偏移*/
    HI_U32 u32Right;   /**<right offset *//**<CNcomment:右侧偏移*/
    HI_U32 u32Bottom;  /**<bottom offset *//**<CNcomment:下方偏移*/
}HI_UNF_DISP_OFFSET_S;

HI_S32 HI_UNF_DISP_Init(HI_VOID);
HI_S32 HI_UNF_DISP_DeInit(HI_VOID);
HI_S32 HI_UNF_DISP_Open (HI_UNF_DISP_E enDisp) ;
HI_S32 HI_UNF_DISP_Close(HI_UNF_DISP_E enDisp);


#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif  /* __OPTM_DISP_H__ */
