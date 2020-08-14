/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : drv_pq_ext.h
  Version       : Initial Draft
  Author        : p00203646
  Created       : 2014/04/01
  Description   :

******************************************************************************/

#ifndef __DRV_PQ_EXT_V3_H__
#define __DRV_PQ_EXT_V3_H__

#include "hi_type.h"
#include "hi_drv_video.h"
//#include "hi_reg_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

//typedef S_CAS_REGS_TYPE PQ_VPSS_CFG_REG_S;
//typedef S_VPSSWB_REGS_TYPE PQ_VPSS_WBC_REG_S ;

/* display ID */
typedef enum hiPQ_DISPLAY_E
{
    HI_PQ_DISPLAY_0 = 0,
    HI_PQ_DISPLAY_1,
    HI_PQ_DISPLAY_2,
    HI_PQ_DISPLAY_BUTT
} HI_PQ_DISPLAY_E;

/* Layer ID */
typedef enum hiPQ_VDP_LAYER_VID_E
{
    PQ_VDP_LAYER_VID0  = 0, /* VDP_LAYER_V0 */
    PQ_VDP_LAYER_VID1  = 1, /* VDP_LAYER_V1 */
    PQ_VDP_LAYER_VID2  = 2, /* VDP_LAYER_V2 */
    PQ_VDP_LAYER_VID3  = 3, /* VDP_LAYER_V3 */
    PQ_VDP_LAYER_VID4  = 4, /* VDP_LAYER_V4 */

    PQ_VDP_LAYER_VID_BUTT
} HI_PQ_VDP_LAYER_VID_E;

/* VPSS视频信息 */
typedef struct hiHI_VPSS_PQ_INFO_S
{
    HI_U32              u32Width;           /* 图像宽度 */
    HI_U32              u32Height;          /* 图像高度 */
    HI_U32              u32FrameRate;       /* 输入帧率 */
    HI_BOOL             bInterlace;         /* 是否隔行信号 */
} HI_VPSS_PQ_INFO_S;

typedef struct hiHI_VDP_CHANNEL_TIMING_S
{
    HI_RECT_S  stFmtRect;
    HI_U32     u32RefreshRate;
    HI_BOOL    bProgressive;
} HI_VDP_CHANNEL_TIMING_S;
/* 算法绑定位置 */
typedef enum hiPQ_ALGIP_SEL_E
{
    HI_PQ_ALGIP_SEL_VDP = 0,   /* Alg Sel Vdp */
    HI_PQ_ALGIP_SEL_VPSS   ,   /* Alg Sel Vpss */

    HI_PQ_ALGIP_SEL__BUTT
} HI_PQ_ALGIP_SEL_E;


/*VDP视频信息 */
typedef struct hiHI_VDP_PQ_INFO_S
{
    HI_U32                      u32Width;           /* 图像宽度 */
    HI_U32                      u32Height;          /* 图像高度 */
    HI_U32                      u32FrameRate;       /* 输入帧率 */
    HI_BOOL                     bInterlace;         /* 是否隔行信号 */
    HI_U32                      u32OutWidth;        /* 输出图像宽度 */
    HI_U32                      u32OutHeight;       /* 输出图像高度 */
    HI_BOOL                     bSRState;           /* SR 开关 */
    HI_BOOL                     bIsogenyMode;       /* 是否同源模式*/    
    HI_BOOL                     bPartUpdate;        /* 是否部分更新参数*/
    HI_VDP_CHANNEL_TIMING_S     stChannelTiming[HI_PQ_DISPLAY_BUTT];
} HI_VDP_PQ_INFO_S;


/* PQ模块开关信息 */
typedef struct hiPQ_VPSS_MODULE_S
{
    HI_BOOL                     bFMD;         /* FMD开关 */
    HI_BOOL                     bTNR;         /* NR开关 */
    HI_BOOL                     bCCCL;        /* CCCL开关 */
} HI_PQ_VPSS_MODULE_S;

/* color space convert type */
typedef enum hiPQ_CSC_MODE_E
{
    HI_PQ_CSC_YUV2RGB_601 = 0      , /* YCbCr_601 L  -> RGB */
    HI_PQ_CSC_YUV2RGB_709          , /* YCbCr_709 L  -> RGB */
    HI_PQ_CSC_RGB2YUV_601          , /* RGB          -> YCbCr_601 L */
    HI_PQ_CSC_RGB2YUV_709          , /* RGB          -> YCbCr_709 L */
    HI_PQ_CSC_YUV2YUV_709_601      , /* YCbCr_709 L  -> YCbCr_601 L */
    HI_PQ_CSC_YUV2YUV_601_709      , /* YCbCr_601 L  -> YCbCr_709 L */
    HI_PQ_CSC_YUV2YUV              , /* YCbCr L      -> YCbCr L */

    HI_PQ_CSC_RGB2RGB_709_2020     , /* RGB_709      -> RGB_2020 */
    HI_PQ_CSC_RGB2YUV_2020_2020_L  , /* RGB_2020     -> YCbCr_2020 L */
    HI_PQ_CSC_YUV2RGB_2020_2020_L  , /* YCbCr_2020 L -> RGB_2020 */
    HI_PQ_CSC_RGB2RGB_2020_709     , /* RGB_2020     -> RGB_709 */
    HI_PQ_CSC_RGB2RGB_601_2020     , /* RGB_601      -> RGB_2020 */
    HI_PQ_CSC_RGB2RGB_2020_601     , /* RGB_2020     -> RGB_601 */

    HI_PQ_CSC_YUV2YUV_2020_601_L_L , /* YCbCr_2020 L -> YCbCr_601 L */
    HI_PQ_CSC_RGB2YUV_2020_601_L   , /* RGB_2020     -> YCbCr_601 L */

    HI_PQ_CSC_YUV2RGB_601_FULL     , /* YCbCr_601 F  -> RGB */
    HI_PQ_CSC_YUV2RGB_709_FULL     , /* YCbCr_709 F  -> RGB */
    HI_PQ_CSC_RGB2YUV_601_FULL     , /* RGB          -> YCbCr_601 F */
    HI_PQ_CSC_RGB2YUV_709_FULL     , /* RGB          -> YCbCr_709 F */
    HI_PQ_CSC_RGB2RGB              , /* RGB          -> RGB */
	HI_PQ_CSC_YUV2RGB_2020_709_L,
	HI_PQ_CSC_YUV2YUV_2020_709_L_L,

    HI_PQ_CSC_BUTT
} HI_PQ_CSC_MODE_E;

/* CSC Tuning Or No Tuning */
typedef enum hiPQ_CSC_TYPE_E
{
    HI_PQ_CSC_TUNING_V0 = 0  ,
    HI_PQ_CSC_TUNING_V1      ,
    HI_PQ_CSC_TUNING_V2      ,
    HI_PQ_CSC_TUNING_V3      ,
    HI_PQ_CSC_TUNING_V4      ,
    HI_PQ_CSC_TUNING_GP0     ,
    HI_PQ_CSC_TUNING_GP1     ,
    HI_PQ_CSC_NORMAL_SETTING ,

    HI_PQ_CSC_TYPE_BUUT
} HI_PQ_CSC_TYPE_E;

/* CSC Tuning Or No Tuning */
typedef struct hiPQ_CSC_CTRL_S
{
    HI_BOOL bCSCEn;
    HI_PQ_CSC_TYPE_E enCscType;
	HI_U32 u32CscPre;

} HI_PQ_CSC_CRTL_S;

#if 0
/*VDP CSC ID*/
typedef enum hiPQ_CSC_ID_E
{
    HI_PQ_VDP_CSC_V0 = 0 ,
    HI_PQ_VDP_CSC_V1     ,
    HI_PQ_VDP_CSC_V2     ,
    HI_PQ_VDP_CSC_V3     ,
    HI_PQ_VDP_CSC_V4     ,
    HI_PQ_VDP_CSC_GP0    ,
    HI_PQ_VDP_CSC_GP1    ,

    HI_PQ_VDP_CSC_BUTT
} HI_PQ_CSC_ID_E;
#endif

/* VDP CSC 属性 */
typedef struct  hiPQ_VDP_CSC_S
{
    HI_BOOL  bCSCEn;
    HI_PQ_CSC_MODE_E  enCscMode;
} HI_PQ_VDP_CSC_S;


/* 缩放YUV数据结构体444 422 420 */
typedef enum hiPQ_ZME_FORMAT_E
{
    HI_PQ_ALG_PIX_FORMAT_SP420 = 0,
    HI_PQ_ALG_PIX_FORMAT_SP422,
    HI_PQ_ALG_PIX_FORMAT_SP444,

    HI_PQ_ALG_PIX_FORMAT_BUTT,
} ZME_FORMAT_E;


/* CSC 矩阵系数结构 */
typedef struct  hiPQ_CSC_COEF_S
{
    HI_S32 csc_coef00;
    HI_S32 csc_coef01;
    HI_S32 csc_coef02;

    HI_S32 csc_coef10;
    HI_S32 csc_coef11;
    HI_S32 csc_coef12;

    HI_S32 csc_coef20;
    HI_S32 csc_coef21;
    HI_S32 csc_coef22;
} HI_PQ_CSC_COEF_S;

typedef struct  hiPQ_CSC_DCCOEF_S
{
    HI_S32 csc_in_dc0;
    HI_S32 csc_in_dc1;
    HI_S32 csc_in_dc2;

    HI_S32 csc_out_dc0;
    HI_S32 csc_out_dc1;
    HI_S32 csc_out_dc2;
} HI_PQ_CSC_DCCOEF_S;

/* DCI统计窗口 */
typedef struct  hiPQ_DCI_WIN_S
{
    HI_U16      u16HStar;
    HI_U16      u16HEnd;
    HI_U16      u16VStar;
    HI_U16      u16VEnd;
} HI_PQ_DCI_WIN_S;


typedef struct
{
    HI_S32 IsProgressiveSeq;
    HI_S32 IsProgressiveFrm;
    HI_S32 RealFrmRate;

} PQ_VDEC_INFO_S;

/* IFMD软算法所需驱动传入参数，VPSS->PQ */
typedef struct hi_PQ_IFMD_CALC_S
{
    HI_U32  u32HandleNo;
    HI_U32  u32WidthY;
    HI_U32  u32HeightY;
    HI_S32  s32FieldOrder;                 /* 顶底场序 顶场优先底场优先 */
    HI_S32  s32FieldMode;                  /* 顶底场标志 */
    HI_U32  u32FrameRate;
    HI_BOOL bPreInfo;                      /* DEI逻辑处理timeout，仍旧处理上一场 */
    PQ_VDEC_INFO_S stVdecInfo;
   // S_VPSSWB_REGS_TYPE* pstIfmdHardOutReg; /* 读取ifmd的状态寄存器 */

} HI_PQ_IFMD_CALC_S;

typedef struct
{
    HI_S32  die_reset;
    HI_S32  die_stop;
    HI_S32  dir_mch;
    HI_S32  die_out_sel;
    HI_S32  jud_en;
    //HI_S32  ChromaVertFltEn;
    HI_S32  s32FieldOrder;    /* 顶地场序 */
    HI_S32  s32SceneChanged;  /* 场景切换信息 */
    HI_S32  s32FilmType;      /* 电影模式 */
    HI_U32  u32KeyValue;      /* 关键帧 */
    HI_U32  u32EdgeSmoothEn;

} HI_PQ_IFMD_PLAYBACK_S;


typedef struct
{
    HI_S32  s32SceneChanged; /* 场景切换信息 */
    HI_S32  s32FilmType;     /* 电影模式 */
    HI_U32  u32KeyValue;     /* 关键帧 */
} HI_PQ_PFMD_PLAYBACK_S;

/* Brightness/Contrast/Hue/Saturation setting */
typedef struct hiPQ_PICTURE_SETTING_S
{
    HI_U16 u16Brightness;
    HI_U16 u16Contrast;
    HI_U16 u16Hue;
    HI_U16 u16Saturation;
} HI_PQ_PICTURE_SETTING_S;


typedef struct
{
    HI_U32    u32ZmeFrmWIn;      /* zme  input frame width  */
    HI_U32    u32ZmeFrmHIn;      /* zme  input frame height */
    HI_U32    u32ZmeFrmWOut;     /* zme output frame width  */
    HI_U32    u32ZmeFrmHOut;     /* zme output frame height */

    HI_U8     u8ZmeYCFmtIn;      /* video format for zme  input: 0-422; 1-420; 2-444 */
    HI_U8     u8ZmeYCFmtOut;     /* video format for zme Output: 0-422; 1-420; 2-444 */

    HI_BOOL   bZmeFrmFmtIn;      /* Frame format for zme  input: 0-field; 1-frame */
    HI_BOOL   bZmeFrmFmtOut;     /* Frame format for zme Output: 0-field; 1-frame */
    HI_BOOL   bZmeBFIn;          /* Input  field polar when input  is field format: 0-top field; 1-bottom field */
    HI_BOOL   bZmeBFOut;         /* Output field polar when Output is field format: 0-top field; 1-bottom field */

    HI_RECT_S stOriRect;
    HI_U32    u32InRate;         /* Vpss out Rate  RealRate*1000 */
    HI_U32    u32OutRate;        /* Disp Rate      RealRate*1000 */
    HI_BOOL   bDispProgressive;  /* 1:Progressive 0:Interlace */
    HI_U32    u32Fidelity;       /* rwzb info >0:is rwzb */
    /*
     1.OriRect
     2.InFrameRate
     3.OutRate
     4.Out I/P
     */
} HI_PQ_ZME_PARA_IN_S;


typedef enum hiPQ_ZME_LAYER_E
{
    HI_PQ_DISP_V0_LAYER_ZME = 0,
    HI_PQ_DISP_V1_LAYER_ZME    ,
    HI_PQ_DISP_V2_LAYER_ZME    ,
    HI_PQ_DISP_V3_LAYER_ZME    ,
    HI_PQ_DISP_V4_LAYER_ZME    ,
    HI_PQ_DISP_WBC0_LAYER_ZME  ,
    HI_PQ_DISP_SR_LAYER_ZME    ,
    HI_PQ_DISP_WBC_DHD_LAYER_ZME  ,

    HI_PQ_DISP_LAYER_ZME_BUTT
} HI_PQ_ZME_LAYER_E;


/* 备注:Port的概念跟HSCL是不同的，VPSS的缩放应该区分两个概念，
   缩放的类型以及缩放的未知(PORT) */
typedef enum hiPQ_VPSS_ZME_LAYER_E
{
    HI_PQ_VPSS_PORT0_LAYER_ZME = 0 ,
    HI_PQ_VPSS_PORT1_LAYER_ZME     ,
    HI_PQ_VPSS_PORT2_LAYER_ZME     ,
    HI_PQ_VPSS_HSCL_LAYER_ZME      ,

    HI_PQ_VPSS_LAYER_ZME_BUTT
} HI_PQ_VPSS_ZME_LAYER_E;


/* PORT缩放的窗口大小 */
typedef struct hiPQ_PORT_WIN_S
{
    HI_U32    u32Height;
    HI_U32    u32Width;
} HI_PQ_PORT_WIN_S;

/* ZME各Layer缩放窗口大小 */
typedef struct hiPQ_ZME_WIN_S
{
    HI_PQ_PORT_WIN_S    stPort0Win;
    HI_PQ_PORT_WIN_S    stPort1Win;
    HI_PQ_PORT_WIN_S    stPort2Win;
    HI_PQ_PORT_WIN_S    stHSCLWin;
} HI_PQ_ZME_WIN_S;
/* Vdec解码协议; 0: H265, 1: H264, 2: Mpeg2, 3: VP9, 4: mndet */
typedef enum hiPQ_VDEC_PROTOCOL_E
{
    HI_PQ_VDEC_PROTOCOL_H265 = 0,
    HI_PQ_VDEC_PROTOCOL_H264    ,
    HI_PQ_VDEC_PROTOCOL_MPEG2   ,
    HI_PQ_VDEC_PROTOCOL_VP9     ,
    HI_PQ_VDEC_PROTOCOL_MNDET   ,

    HI_PQ_VDEC_PROTOCOL_BUTT    ,    

} HI_PQ_VDEC_PROTOCOL_E;


/* VPSS 驱动传入的信息 */
typedef struct hiPQ_WBC_INFO_S
{
    /* Common */
    HI_U32   u32HandleNo;
    HI_U32   u32Width;
    HI_U32   u32Height;
    HI_BOOL  bProgressive;              /* 隔逐行信息 */
//    S_VPSSWB_REGS_TYPE* pstVPSSWbcReg;  /* ifmd 、Globlal Motion and DB的回写信息 */
    /* GlobalMotion */
    HI_U32  u32Scd;
    HI_U32  stride;
    HI_VOID* pRGMV;

    /* IFMD */
    HI_S32   s32FieldOrder;              /* 顶底场序 顶场优先底场优先 */
    HI_S32   s32FieldMode;               /* 顶底场标志 */
    HI_U32   u32FrameRate;               /* 帧率 */
    HI_BOOL  bPreInfo;                   /* DEI逻辑处理timeout，仍旧处理上一场 */
    PQ_VDEC_INFO_S stVdecInfo;           /* VDEC 传递的隔逐行信息 */

    /* DNR */

} HI_PQ_WBC_INFO_S;


/* PQ Calc 传给VPSS驱动的信息 */
typedef struct hiPQ_CFG_INFO_S
{
    /* GlobalMotion */

    /* IFMD */
    HI_S32  die_reset;
    HI_S32  die_stop;
    HI_S32  dir_mch;
    HI_S32  die_out_sel;
    HI_S32  jud_en;
    HI_S32  ChromaVertFltEn;
    HI_S32  s32FieldOrder;    /* 顶地场序 */
    HI_S32  s32SceneChanged;  /* 场景切换信息 */
    HI_S32  s32FilmType;      /* 电影模式 */
    HI_U32  u32KeyValue;      /* 关键帧 */
    /* DNR */

} HI_PQ_CFG_INFO_S;

/* VPSS WBC 传递给De-Blocking运算必须的信息 */
typedef struct hiPQ_DB_CALC_INFO_S
{

    HI_U32 u32HandleNo;
    HI_U32 u32Width;
    HI_U32 u32Height;
   // S_VPSSWB_REGS_TYPE* pstDbCalcWbcReg;

} HI_PQ_DB_CALC_INFO_S;
typedef struct hiPQ_VDP_WBC_INFO_S
{
    /* Common */ 
    HI_PQ_VDP_LAYER_VID_E  enLayerId;          /* 层 */
    HI_U32                 u32BitDepth;        /* 通路处理位宽 */
    HI_RECT_S              stOriCoordinate;    /* Crop之前坐标 */
    HI_RECT_S              stCropCoordinate;   /* Crop之后坐标 */
    HI_PQ_ALGIP_SEL_E      enAlgIPSel;         /* 算法绑定位置 */
    //S_VPSSWB_REGS_TYPE*    pstVdpWbcReg;       /* ifmd、Globlal Motion and DB的回写信息 */

    /* DNR */
    HI_BOOL                bCropBeforeDB;      /* 0: 在DB之后Crop;1: 在DB 之前Crop */
    HI_U32                 u32SingleModeFlag;  /* 单场模式标识; 1:单场;0:非单场 */
    HI_U32                 u32ImageAvgQP;      /* 图像平均 QP */
    HI_U32                 u32VdhInfoAddr;     /* dbinfo 地址 */
    HI_U32                 u32VdhInfoStride;   /* 信息stride, 算法约定为(图像宽度+7)/8 */
    HI_U32                 u32ImageStruct;     /* 帧/场编码标识 */
    HI_PQ_VDEC_PROTOCOL_E  enVdecProtocol;     /* 解码协议 */

} HI_PQ_VDP_WBC_INFO_S;



typedef enum
{
    HI_PQ_ZME_COEF_1         = 0,
    HI_PQ_ZME_COEF_E1           ,
    HI_PQ_ZME_COEF_075          ,
    HI_PQ_ZME_COEF_05           ,
    HI_PQ_ZME_COEF_033          ,
    HI_PQ_ZME_COEF_025          ,
    HI_PQ_ZME_COEF_0            ,

    HI_PQ_ZME_COEF_RATIO_BUTT
} HI_PQ_ZME_COEF_RATIO_E;

typedef enum
{
    HI_PQ_ZME_COEF_TYPE_LH = 0,
    HI_PQ_ZME_COEF_TYPE_LV    ,
    HI_PQ_ZME_COEF_TYPE_CH    ,
    HI_PQ_ZME_COEF_TYPE_CV    ,

    HI_PQ_ZME_COEF_TYPE_BUTT
} HI_PQ_ZME_COEF_TYPE_E;

typedef enum
{
    HI_PQ_ZME_TAP_8T32P = 0,
    HI_PQ_ZME_TAP_6T32P    ,
    HI_PQ_ZME_TAP_4T32P    ,
    HI_PQ_ZME_TAP_2T32P    ,

    HI_PQ_ZME_TAP_BUTT
} HI_PQ_ZME_TAP_E;


/*********************internal struction define***************************/
typedef struct
{
    HI_U32    u32FrmWIn;      /* zme  input frame width  */
    HI_U32    u32FrmHIn;      /* zme  input frame height */
    HI_U32    u32FrmWOut;     /* zme output frame width  */
    HI_U32    u32FrmHOut;     /* zme output frame height */
    HI_U32    u32FmtIn;       /* video format for zme  input: 0-422; 1-420; 2-444 */
} HI_PQ_LAYER_STRATEGY_IN_S;

typedef struct
{
    HI_PQ_LAYER_STRATEGY_IN_S  stLayerInfo[2];
} HI_PQ_WBC_STRATEGY_IN_S;
/*********************internal struction define end***************************/


/*********************external struction define end***************************/
typedef union
{
    HI_PQ_LAYER_STRATEGY_IN_S stLayerStrategy;
    HI_PQ_WBC_STRATEGY_IN_S   stWbcStrategy;
} HI_PQ_ZME_STRATEGY_IN_U;

typedef enum hiPQ_PREZME_HOR_MUL_E
{
    HI_PQ_PREZME_HOR_DISABLE = 0,
    HI_PQ_PREZME_HOR_2X         ,
    HI_PQ_PREZME_HOR_4X         ,
    HI_PQ_PREZME_HOR_8X         ,

    HI_PQ_PREZME_HOR_BUTT
} HI_PQ_PREZME_HOR_MUL_E;

typedef enum hiPQ_PREZME_VER_MUL_E
{
    HI_PQ_PREZME_VER_DISABLE = 0,
    HI_PQ_PREZME_VER_2X         ,
    HI_PQ_PREZME_VER_4X         ,
    HI_PQ_PREZME_VER_8X         ,

    HI_PQ_PREZME_VER_BUTT
} HI_PQ_PREZME_VER_MUL_E;

typedef enum  hiPQ_WBC_POINT_SEL_E
{
    HI_PQ_WBC_POINT_VP  = 0,
    HI_PQ_WBC_POINT_V0,

    HI_PQ_WBC_POINT_BUTT
} HI_PQ_WBC_POINT_SEL_E;

typedef enum  hiPQ_ZME_BIND_POSITION_E
{
    HI_PQ_ZME_BIND_WBC  = 0,
    HI_PQ_ZME_BIND_V3,

    HI_PQ_ZME_BIND_BUTT
} HI_PQ_ZME_BIND_POSITION_E;

typedef  struct
{
    HI_U32   u32FrmInWidth;
    HI_U32   u32FrmInHeight;

    HI_U32   u32FrmOutWidth;
    HI_U32   u32FrmOutHeight;

    HI_U32   u32PixInFmt;       /* video format for zme  input: 0-422; 1-420; 2-444 */
    HI_U32   u32PixOutFmt;      /* video format for zme  input: 0-422; 1-420; 2-444 */

    HI_BOOL  bInFmt;     /* Frame format for zme Output: 0-field; 1-frame */
    HI_BOOL  bOutFmt;    /* Frame format for zme Output: 0-field; 1-frame */
} HI_PQ_IMAGE_FMT_S;

/*V0*/
typedef  struct
{
    HI_BOOL   bReadFmtIn;      /* Frame format for zme  input: 0-field; 1-frame */
    HI_BOOL   bReadBFIn;       /* Input field polar when input is field format: 0-top field; 1-bottom field */
    HI_BOOL   bOpenP2I;

    HI_PQ_PREZME_HOR_MUL_E  eHScalerMode;  /* Horizontal PreScaler */
    HI_PQ_PREZME_VER_MUL_E  eVScalerMode;  /* Vertical PreScaler */

    HI_PQ_IMAGE_FMT_S   stZmeFmt[3];
    HI_U32 u32ZmeNum;
} HI_PQ_LAYER_STRATEGY_OUT_S;

/* WBC VP*/
typedef  struct
{
    HI_BOOL   bReadFmtIn;      /* Frame format for zme  input: 0-field; 1-frame */
    HI_BOOL   bReadBFIn;       /* Input field polar when input is field format: 0-top field; 1-bottom field */
    HI_BOOL   bOpenP2I;

    HI_PQ_PREZME_HOR_MUL_E    eHScalerMode;        /* ????? */
    HI_PQ_WBC_POINT_SEL_E     enVpPoint;
    HI_PQ_ZME_BIND_POSITION_E eZmePosition;

    HI_PQ_IMAGE_FMT_S stZme;
} HI_PQ_WBC_VP_STRATEGY_OUT_S;

typedef  struct
{
    /* WBC DHD*/
    HI_PQ_PREZME_HOR_MUL_E  eHScalerMode;
    HI_BOOL  bInWbcVpZmeFmt;     /* Frame format for zme Output: 0-field; 1-frame */
    HI_PQ_IMAGE_FMT_S stZme;
} HI_PQ_WBC_DHD0_STRATEGY_OUT_S;

typedef union
{
    HI_PQ_LAYER_STRATEGY_OUT_S    stStrategyV0;
    HI_PQ_WBC_VP_STRATEGY_OUT_S   stStrategyWbcVp;
    HI_PQ_WBC_DHD0_STRATEGY_OUT_S stStrategyWbcDhd;
} HI_PQ_ZME_STRATEGY_OUT_U;
//-------------gfx csc ----------------------//

typedef enum hiPQ_GFX_COLOR_SPACE_E
{
    VDP_GFX_CS_UNKNOWN = 0,

    VDP_GFX_CS_BT601_YUV_LIMITED,  /* BT.601 */
    VDP_GFX_CS_BT601_YUV_FULL,
    VDP_GFX_CS_BT601_RGB_LIMITED,
    VDP_GFX_CS_BT601_RGB_FULL,

    VDP_GFX_CS_BT709_YUV_LIMITED,  /* BT.709 */
    VDP_GFX_CS_BT709_YUV_FULL,
    VDP_GFX_CS_BT709_RGB_LIMITED,
    VDP_GFX_CS_BT709_RGB_FULL,

    VDP_GFX_CS_BT2020_YUV_LIMITED, /* BT.2020 */
    VDP_GFX_CS_BT2020_RGB_FULL,

    VDP_GFX_CS_BUTT
} HI_VDP_GFX_COLOR_SPACE_E;

typedef struct hiVDP_GFX_CSC_MODE_S
{
    HI_VDP_GFX_COLOR_SPACE_E eInputCS;  /* input  color space type, should be set carefully according to the application situation. */
    HI_VDP_GFX_COLOR_SPACE_E eOutputCS; /* output color space type, should be set carefully according to the application situation. */

} HI_VDP_GFX_CSC_MODE_S;

typedef struct
{
	HI_U32  u32GammaCoef[64]; 
	HI_U32  u32DeGammaCoef[64]; 
	
}HI_VDP_GAMM_PARA_S;





typedef struct
{
    HI_VDP_GFX_COLOR_SPACE_E eInputCS;  /* input  color space type, should be set carefully according to the application situation. */
    HI_VDP_GFX_COLOR_SPACE_E eOutputCS; /* output color space type, should be set carefully according to the application situation. */
    HI_BOOL bIsBGRIn;

    HI_U32  u32Bright;      /* bright adjust value,range[0,100],default setting 50; */
    HI_U32  u32Contrst;     /* contrast adjust value,range[0,100],default setting 50; */
    HI_U32  u32Hue;         /* hue adjust value,range[0,100],default setting 50; */
    HI_U32  u32Satur;       /* saturation adjust value,range[0,100],default setting 50; */
    HI_U32  u32Kr;          /* red component gain adjust value for color temperature adjust,range[0,100],default setting 50; */
    HI_U32  u32Kg;          /* green component gain adjust value for color temperature adjust,range[0,100],default setting 50; */
    HI_U32  u32Kb;          /* blue component gain adjust value for color temperature adjust,range[0,100],default setting 50; */
} HI_VDP_GFX_CSC_PARA_S;


typedef struct
{
    HI_U32 u32ZmeFrmWIn;    /* zme input frame width   */
    HI_U32 u32ZmeFrmHIn;    /* zme input frame height  */
    HI_U32 u32ZmeFrmWOut;   /* zme output frame width  */
    HI_U32 u32ZmeFrmHOut;   /* zme output frame height */

    HI_BOOL bZmeFrmFmtIn;   /* Frame format for zme input:  0-field; 1-frame */
    HI_BOOL bZmeFrmFmtOut;  /* Frame format for zme Output: 0-field; 1-frame */

    HI_BOOL bDeFlicker;
    HI_BOOL bSlvGp;         /* 0-no isogeny; 1-isogeny */
} HI_VDP_GFX_ZME_PARA_S;

//----------------gfx csc end-------------------//

//----------------add by hyx for zme phase ---------//
#define ALG_V_HZME_PRECISION 1048576
#define ALG_V_VZME_PRECISION 4096




//typedef HI_S32 (*FN_PQ_UpdateVpssPQ)(HI_U32 u32HandleNo, HI_VPSS_PQ_INFO_S* pstTimingInfo, PQ_VPSS_CFG_REG_S* pstVPSSReg, PQ_VPSS_WBC_REG_S* pstWbcReg, HI_PQ_VPSS_MODULE_S* pstPQModule);
//typedef HI_S32 (*FN_PQ_UpdateVdpPQ)(HI_U32 u32DisplayId, HI_VDP_PQ_INFO_S* pstTimingInfo, S_VDP_REGS_TYPE* pstVDPReg);
typedef HI_S32 (*FN_PQ_UpdateVdpCSC)(HI_PQ_CSC_TYPE_E enCscTypeId, HI_PQ_VDP_CSC_S* pstCscMode);
typedef HI_S32 (*FN_PQ_UpdateDCIWin)(HI_PQ_DCI_WIN_S* pstWin, HI_BOOL bDciEnable);
typedef HI_S32 (*FN_PQ_SetZme)(HI_PQ_ZME_LAYER_E u32LayerId, HI_PQ_ZME_PARA_IN_S* pstZmeDrvPara, HI_BOOL  bFirEnable);
//typedef HI_S32 (*FN_PQ_SetVpssZme)(HI_PQ_VPSS_ZME_LAYER_E e32LayerId, S_CAS_REGS_TYPE* pstReg, HI_PQ_ZME_PARA_IN_S* pstZmeDrvPara, HI_BOOL  bFirEnable);
typedef HI_S32 (*FN_PQ_GetCSCCoef)(HI_PQ_CSC_CRTL_S* pstCscCtrl, HI_PQ_CSC_MODE_E enCscMode, HI_PQ_CSC_COEF_S* pstCscCoef, HI_PQ_CSC_DCCOEF_S* pstCscDcCoef);
typedef HI_S32 (*FN_PQ_Get8BitCSCCoef)(HI_PQ_CSC_MODE_E  enCSCMode, HI_PQ_CSC_COEF_S* pstCSCCoef, HI_PQ_CSC_DCCOEF_S* pstCSCDCCoef);
typedef HI_S32 (*FN_PQ_DBCalcCfg)(HI_PQ_DB_CALC_INFO_S* pstDbCalcInfo);
typedef HI_S32 (*FN_PQ_GetWbcInfo)(HI_PQ_WBC_INFO_S* pstVpssWbcInfo);
typedef HI_S32 (*FN_PQ_SetAlgCalcCfg)(HI_PQ_WBC_INFO_S* pstVpssWbcInfo, HI_PQ_CFG_INFO_S* pstVpssCfgInfo);
typedef HI_S32 (*FN_PQ_GetSetting)(HI_PQ_PICTURE_SETTING_S* pstPictureSetting);
typedef HI_S32 (*FN_PQ_SetSetting)(HI_PQ_PICTURE_SETTING_S* pstPictureSetting);
typedef HI_S32 (*FN_PQ_GetVdpZmeCoef)(HI_PQ_ZME_COEF_RATIO_E enCoefRatio, HI_PQ_ZME_COEF_TYPE_E enCoefType, HI_PQ_ZME_TAP_E enZmeTap, HI_S16* ps16Coef);
typedef HI_S32 (*FN_PQ_GetVdpZmeStrategy) (HI_PQ_ZME_LAYER_E enLayerId, HI_PQ_ZME_STRATEGY_IN_U* pstZmeIn, HI_PQ_ZME_STRATEGY_OUT_U* pstZmeOut);

#if 0
typedef struct tagPQ_EXPORT_FUNC_S
{
  //  FN_PQ_UpdateVpssPQ            pfnPQ_UpdateVpssPQ;
//    FN_PQ_UpdateVdpPQ             pfnPQ_UpdateVdpPQ;
    FN_PQ_UpdateVdpCSC            pfnPQ_UpdateVdpCSC;
    FN_PQ_UpdateDCIWin            pfnPQ_UpdateDCIWin;
    FN_PQ_SetZme                  pfnPQ_SetZme;
   // FN_PQ_SetVpssZme              pfnPQ_SetVpssZme;
    FN_PQ_GetCSCCoef              pfnPQ_GetCSCCoef;
    FN_PQ_Get8BitCSCCoef          pfnPQ_Get8BitCSCCoef;
    FN_PQ_DBCalcCfg               pfnPQ_DBCalcCfg;
    FN_PQ_GetWbcInfo              pfnPQ_GetWbcInfo;
    FN_PQ_SetAlgCalcCfg           pfnPQ_SetAlgCalcCfg;
    FN_PQ_GetSetting              pfnPQ_GetHDPictureSetting;
    FN_PQ_GetSetting              pfnPQ_GetSDPictureSetting;
    FN_PQ_SetSetting              pfnPQ_SetHDPictureSetting;
    FN_PQ_SetSetting              pfnPQ_SetSDPictureSetting;
    FN_PQ_GetSetting              pfnPQ_GetHDVideoSetting;
    FN_PQ_GetSetting              pfnPQ_GetSDVideoSetting;
    FN_PQ_SetSetting              pfnPQ_SetHDVideoSetting;
    FN_PQ_SetSetting              pfnPQ_SetSDVideoSetting;
    FN_PQ_GetVdpZmeCoef           pfnPQ_GetVdpZmeCoef;
    FN_PQ_GetVdpZmeStrategy       pfnPQ_GetVdpZmeStrategy;
} PQ_EXPORT_FUNC_S;
#endif

extern HI_S32 PQ_DRV_ModInit(HI_VOID);

extern HI_VOID PQ_DRV_ModuleExit(HI_VOID);

extern HI_S32 HI_DRV_PQ_Init(HI_CHAR* pszPath);

extern HI_S32 HI_DRV_PQ_DeInit(HI_VOID);


/**
\brief Timming变化后进行VPSS PQ参数更新
\attention \n
无

\param[in] *u32HandleNo:VPSS通道号
\param[in] *pstTimingInfo: Timming Info
\param[in] *pstVPSSReg:VPSS 虚拟寄存器地址，如果为NULL，则表示销毁该路地址,
一个handleNo对应一个地址，若地址发生变化，才会重新赋初始值，否则不赋初始值。
\param[out] *pstPQModule:PQ传给驱动的开关参数

\retval ::HI_SUCCESS

*/

//extern HI_S32 DRV_PQ_UpdateVpssPQ(HI_U32 u32HandleNo, HI_VPSS_PQ_INFO_S* pstTimingInfo, PQ_VPSS_CFG_REG_S* pstVPSSReg, PQ_VPSS_WBC_REG_S* pstWbcReg, HI_PQ_VPSS_MODULE_S* pstPQModule);

/**
 \brief Timming变化后进行VDP PQ参数更新
 \attention \n
无

 \param[in] u32DisplayId
 \param[in] pstTimingInfo: Timming Info
 \param[in] *pstVDPReg:VDP 虚拟寄存器地址

 \retval ::HI_SUCCESS

 */

//extern HI_S32 DRV_PQ_UpdateVdpPQ(HI_U32 u32DisplayId, HI_VDP_PQ_INFO_S* pstTimingInfo, S_VDP_REGS_TYPE* pstVDPReg);

/**
 \brief 设置VDP各通道的CSC
 \attention \n
无

 \param[in] enDisplayId:
 \param[in] pstCscMode: 色彩空间

 \retval ::HI_SUCCESS

 */
extern HI_S32 DRV_PQ_UpdateVdpCSC(HI_PQ_CSC_TYPE_E enCscTypeId, HI_PQ_VDP_CSC_S* pstCscMode);

/**
 \brief 更新DCI直方图统计窗口
 \attention \n
无

 \param[in] *pstWin;

 \retval ::HI_SUCCESS

 */
extern HI_S32 DRV_PQ_UpdateDCIWin(HI_PQ_DCI_WIN_S* pstWin, HI_BOOL bDciEnable);

/**
 \brief 设置ZME
 \attention \n
无

 \param[in] u32LayerId:
 \param[in] pstZmeDrvPara:
 \param[in] bFirEnable

 \retval ::HI_SUCCESS

 */
extern HI_S32 DRV_PQ_SetZme(HI_PQ_ZME_LAYER_E e32LayerId, HI_PQ_ZME_PARA_IN_S* pstZmeDrvPara, HI_BOOL bFirEnable);

/**
 \brief 设置ZME
 \attention \n
无

 \param[in] u32LayerId:
 \param[in] pstZmeDrvPara:
 \param[in] bFirEnable

 \retval ::HI_SUCCESS

 */
//extern HI_S32 DRV_PQ_SetVpssZme(HI_PQ_VPSS_ZME_LAYER_E e32LayerId, S_CAS_REGS_TYPE* pstReg, HI_PQ_ZME_PARA_IN_S* pstZmeDrvPara, HI_BOOL bFirEnable);

/**
 \brief 获取CSC系数
 \attention \n
无

 \param[in] enCSCMode:
 \param[out] pstCSCCoef:
 \param[out] pstCSCDCCoef:

 \retval ::HI_SUCCESS

 */

extern HI_S32 DRV_PQ_GetCSCCoef(HI_PQ_CSC_CRTL_S* pstCscCtrl, HI_PQ_CSC_MODE_E enCscMode, HI_PQ_CSC_COEF_S* pstCscCoef, HI_PQ_CSC_DCCOEF_S* pstCscDcCoef);

/**
 \brief 获取8bit CSC系数
 \attention \n
无

 \param[in] enCSCMode:
 \param[out] pstCSCCoef:
 \param[out] pstCSCDCCoef:

 \retval ::HI_SUCCESS

 */
extern HI_S32 DRV_PQ_Get8BitCSCCoef(HI_PQ_CSC_MODE_E enCSCMode, HI_PQ_CSC_COEF_S* pstCSCCoef, HI_PQ_CSC_DCCOEF_S* pstCSCDCCoef);


/**
 \brief 设置算法运算完之后的寄存器
 \attention \n
无

 \param[in]  *pstWbcInfo
 \param[out]
 \retval ::HI_SUCCESS

 */
extern HI_S32 DRV_PQ_SetAlgCalcCfg(HI_PQ_WBC_INFO_S* pstVpssWbcInfo, HI_PQ_CFG_INFO_S* pstVpssCfgInfo);

/**
 \brief 获取WbcInfo信息
 \attention \n
无

 \param[in]  HI_PQ_WBC_INFO_S* pstWbcInfo

 \retval ::HI_SUCCESS

 */
extern HI_S32 DRV_PQ_GetWbcInfo(HI_PQ_WBC_INFO_S* pstVpssWbcInfo);


/**
 \brief 获取图形高清亮度/对比度/色调/饱和度
 \attention \n
无

 \param[out] u32Hue   亮度/对比度/色调/饱和度

 \retval ::HI_SUCCESS

 */
extern HI_S32 DRV_PQ_GetHDPictureSetting(HI_PQ_PICTURE_SETTING_S* pstPictureSetting);

/**
 \brief 获取图形标清亮度/对比度/色调/饱和度
 \attention \n
无

 \param[out] u32Hue   亮度/对比度/色调/饱和度

 \retval ::HI_SUCCESS

 */
extern HI_S32 DRV_PQ_GetSDPictureSetting(HI_PQ_PICTURE_SETTING_S* pstPictureSetting);


/**
 \brief 设置图形高清亮度/对比度/色调/饱和度
 \attention \n
无

 \param[in] u32Hue   亮度/对比度/色调/饱和度

 \retval ::HI_SUCCESS

 */
extern HI_S32 DRV_PQ_SetHDPictureSetting(HI_PQ_PICTURE_SETTING_S* pstPictureSetting);

/**
 \brief 设置图形标清亮度/对比度/色调/饱和度
 \attention \n
无

 \param[in] u32Hue   亮度/对比度/色调/饱和度

 \retval ::HI_SUCCESS

 */
extern HI_S32 DRV_PQ_SetSDPictureSetting(HI_PQ_PICTURE_SETTING_S* pstPictureSetting);

/**
 \brief 获取视频高清亮度/对比度/色调/饱和度
 \attention \n
无

 \param[out] u32Hue   亮度/对比度/色调/饱和度

 \retval ::HI_SUCCESS

 */
extern HI_S32 DRV_PQ_GetHDVideoSetting(HI_PQ_PICTURE_SETTING_S* pstPictureSetting);

/**
 \brief 获取视频标清亮度/对比度/色调/饱和度
 \attention \n
无

 \param[out] u32Hue   亮度/对比度/色调/饱和度

 \retval ::HI_SUCCESS

 */
extern HI_S32 DRV_PQ_GetSDVideoSetting(HI_PQ_PICTURE_SETTING_S* pstPictureSetting);


/**
 \brief 设置视频高清亮度/对比度/色调/饱和度
 \attention \n
无

 \param[in] u32Hue   亮度/对比度/色调/饱和度

 \retval ::HI_SUCCESS

 */
extern HI_S32 DRV_PQ_SetHDVideoSetting(HI_PQ_PICTURE_SETTING_S* pstPictureSetting);

/**
 \brief 设置视频标清亮度/对比度/色调/饱和度
无

 \param[in] u32Hue   亮度/对比度/色调/饱和度

 \retval ::HI_SUCCESS

 */
extern HI_S32 DRV_PQ_SetSDVideoSetting(HI_PQ_PICTURE_SETTING_S* pstPictureSetting);



/**
 \brief VPSS ZME二级缩放校验;
 \attention \n
无

 \param[in]
 u32InWitdh: 输入宽; u32InHeigh: 输入高; stZmeWin: 各层输出宽高;

 \param[out]
 pu32OutWitdh 输出宽; pu32OutHeigh 输出高;

 \retval :
HI_SUCCESS : 需要二级缩放
HI_FAILURE : 不需要二级缩放

 */
extern HI_S32 DRV_PQ_ZME_2L_Check(HI_U32 u32InWitdh, HI_U32 u32InHeigh, HI_PQ_ZME_WIN_S stZmeWin, HI_U32* pu32OutWitdh, HI_U32* pu32OutHeigh);

extern HI_S32 DRV_PQ_GetVdpZmeCoef(HI_PQ_ZME_COEF_RATIO_E enCoefRatio, HI_PQ_ZME_COEF_TYPE_E enCoefType, HI_PQ_ZME_TAP_E enZmeTap, HI_U8* ps16Coef);

extern HI_S32 DRV_PQ_GetVdpZmeStrategy (HI_PQ_ZME_LAYER_E enLayerId, HI_PQ_ZME_STRATEGY_IN_U* pstZmeIn, HI_PQ_ZME_STRATEGY_OUT_U* pstZmeOut);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* End of #ifndef __DRV_PQ_EXT_H__ */
