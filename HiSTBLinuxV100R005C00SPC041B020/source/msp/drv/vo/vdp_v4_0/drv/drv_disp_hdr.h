/******************************************************************************
  Copyright (C), 2001-2015, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
  File Name     : drv_disp_hdr.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2015-09-23
  Description   :structures define and functions declaration for processing Dolby HDR.
  History       :
  1.Date        : 2015-09-23
    Author      : q00293180
    Modification: Created file
*******************************************************************************/

#ifndef __DRV_DISP_HDR_H__
#define __DRV_DISP_HDR_H__

#include "hi_type.h"
#include "vdp_define.h"
#include "vdp_ip_define.h"
#include "vdp_fpga_define.h"
#include "KdmTypeFxp.h"
#include "vdp_hal_intf.h"

#ifndef __DISP_PLATFORM_BOOT__
#include "drv_hdmi_ext.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

//switcher composer mode define
#define DISABLE_EL_FLAG_OFF  0
#define DISABLE_EL_FLAG_ON   1

//update metadata buffer length
#define MAX_MD_BUF_LEN  255 //255 Bytes.

//metadata info send over HDMI
#define HEADER_MD_VER   0b000
#define HEADER_MD_TYPE  0b00

//DolbyVision source in HDR10 out infoFrame metadata
#define HDR10_METADATA_R_X                  0x8A48              //0.708/0.00002 = 35400
#define HDR10_METADATA_R_Y                  0x3908              //0.292/0.00002 = 14600
#define HDR10_METADATA_G_X                  0x2134              //0.170/0.00002 = 8500
#define HDR10_METADATA_G_Y                  0x9BAA              //0.797/0.00002 = 39850
#define HDR10_METADATA_B_X                  0x1996              //0.131/0.00002 = 6550
#define HDR10_METADATA_B_Y                  0x8FC               //0.046/0.00002 = 2300
#define HDR10_METADATA_WP_X                 0x3D13              //0.3127/0.00002 = 15635
#define HDR10_METADATA_WP_Y                 0x4042              //0.3290/0.00002 = 16450
#define HDR10_METADATA_DISP_MASTER_MAX      0x3E8               //1000nits 
#define HDR10_METADATA_DISP_MASTER_MIN      0x5                 //0.005nits 
#define HDR10_METADATA_CLL_MAX              0x0
#define HDR10_METADATA_FALL_MAX             0x0


//HLG source in HDR10 out infoFrame metadata
#define HLG_TO_HDR10_PRIMARIES0_X                  13250        
#define HLG_TO_HDR10_PRIMARIES0_Y                  34500        
#define HLG_TO_HDR10_PRIMARIES1_X                  7500        
#define HLG_TO_HDR10_PRIMARIES1_Y                  3000        
#define HLG_TO_HDR10_PRIMARIES2_X                  34000        
#define HLG_TO_HDR10_PRIMARIES2_Y                  16000         
#define HLG_TO_HDR10_WHITE_POINT_X                 15635        
#define HLG_TO_HDR10_WHITE_POINT_Y                 16450        
#define HLG_TO_HDR10_DISP_MASTER_LUMINANCE_MAX     1000         
#define HLG_TO_HDR10_DISP_MASTER_LUMINANCE_MIN     200           
#define HLG_TO_HDR10_CONTENT_LIGHT_LEVEL_MAX       1000
#define HLG_TO_HDR10_FRAME_AVERAGE_LIGHT_MAX         250


typedef enum hiDRV_DISP_WORKING_MODE_E
{
    HI_DRV_DISP_WORKING_MODE_SDR = 0,            /* 输入源为SDR源时将进入该工作模式 */
    HI_DRV_DISP_WORKING_MODE_DOLBY_HDR,          /* 输入源为Dolby源时将进入该工作模式 */
    HI_DRV_DISP_WORKING_MODE_LAST_GRAPHIC_DOLBY, /* 没有视频层，在DolbyVision输出模式下最后一帧DolbyVision图形输出；
                                                    该模式存在的原因是在退出DolbyVision输出模式时 ，source需要发送最后一帧标记*/
    HI_DRV_DISP_WORKING_MODE_DOLBY_HDR10,        /* 输入源为HDR10经过DOLBY HDR通路处理的工作模式 */
    HI_DRV_DISP_WORKING_MODE_HISI_HDR,           /* 输入源为HDR经过Hisi HDR通路处理的工作模式 */
    HI_DRV_DISP_WORKING_MODE_BUTT
}HI_DRV_DISP_WORKING_MODE_E;



typedef enum hiDRV_HDR_PLAY_MODE_E
{
    HI_DRV_HDR_PLAY_MODE_SDR = 0,             /* output sdr signal. */
    HI_DRV_HDR_PLAY_MODE_SDR_CERT,            /* output sdr cert signal. */        
    HI_DRV_HDR_PLAY_MODE_DOLBY,               /* output dolby signal. */
    HI_DRV_HDR_PLAY_MODE_DOLBY_LAST,          /* output last dolby frm signal. */
    HI_DRV_HDR_PLAY_MODE_HDR10,               /* output hdr10 signal(10bit444). */
    HI_DRV_HDR_PLAY_MODE_HDR10_CERT,          /* output hdr10 cert signal(10bit422) */

    HI_DRV_HDR_PLAY_MODE_HLG_ARIB,            /* output HLG_ARIB hdr signal */
    HI_DRV_HDR_PLAY_MODE_HLG_BBC,             /* output HLG_BBC hdr signal */ 
    HI_DRV_HDR_PLAY_MODE_TECHNICOLOR,         /* output Technicolor hdr signal */
    
    HI_DRV_HDR_PLAY_MODE_BUTT
}HI_DRV_HDR_PLAY_MODE_E;



//metadata assemble struct
typedef struct hiDRV_DOLBY_METADATA_ASSEMBLE_S
{
    rpu_ext_config_fixpt_main_t     stCompCfg;
    dm_metadata_t                   stDmCfg;
    DmKsFxp_t                       stDmKs;
}HI_DRV_DOLBY_METADATA_ASSEMBLE_S;

typedef enum hiDRV_DOLBY_COMPOSER_MODE_E
{
    HI_DRV_DOLBY_COMPOSER_MODE_FULL = 0,    /* BL与EL叠加composer模式 */
    HI_DRV_DOLBY_COMPOSER_MODE_EL_FREE,     /* 忽略EL，BL独过composer模式 */
    HI_DRV_DOLBY_COMPOSER_MODE_BUTT
}HI_DRV_DOLBY_COMPOSER_MODE_E;


typedef struct hiDRV_DOLBY_HDR_MD_HEADER_S
{
    HI_BOOL     bNoMd;      /* indicates that no Dolby Vision metadata is contained in the current Dolby Vision video frame transmission */
    HI_BOOL     bEos;       /* indicates that the immediate next video frame contains a signal that is not a Dolby Vision signal */
    HI_U8       u8MdVer;    /* metadata_version = 0b000: Current version */
    HI_U8       u8MdType;   /* metadata_type = 0b00: Dolby Vision metadata */
    HI_U32      u32MdId;    /* first byte of dm_metadata_base_t,indicates the ID number of the Dolby Vision video frame */
}HI_DRV_DOLBY_HDR_MD_HEADER_S;

#ifndef __DISP_PLATFORM_BOOT__

/************HDR hal start****************/
//HAL layer adapter structure.
typedef enum tagDRV_DISP_HDR_CFG_MODE_E
{
    DRV_DOVI_BC_IN_DOVI_YUV_OUT  = 0x0 , 
    DRV_DOVI_NBC_IN_DOVI_YUV_OUT       , 

    DRV_DOVI_BC_IN_HDR10_OUT_CERT      , 
    DRV_DOVI_NBC_IN_HDR10_OUT_CERT     , 
    DRV_HDR10_IN_HDR10_OUT_CERT        , 

    //for STB hdmi output, 444
    //VDP_HDR_SDR_IN_SDR_OUT       , //don't need this mode.SDR will go through normal traditional path
    DRV_DOVI_BC_IN_SDR_OUT             , 
    DRV_DOVI_NBC_IN_SDR_OUT            , 
    DRV_HDR10_IN_SDR_OUT               , 

    DRV_DOVI_BC_IN_DOVI_IPT_OUT        , 
    DRV_DOVI_NBC_IN_DOVI_IPT_OUT       , 

    //for certificate: 422 uyvy output
//    VDP_HDR_SDR_IN_SDR_OUT_CERT            , 
    DRV_DOVI_BC_IN_SDR_OUT_CERT        , 
    DRV_DOVI_NBC_IN_SDR_OUT_CERT       , 
    DRV_HDR10_IN_SDR_OUT_CERT          , 
//
//    //for FPGA test
//    VDP_HDR_DOVI_BC_IN_HDR10_OUT_FPGA      , 
//    VDP_HDR_DOVI_NBC_IN_HDR10_OUT_FPGA     , 
    DRV_DOVI_BC_IN_HDR10_OUT           , //HAL NEED TO ADD THIS NEW BRANCH
    DRV_DOVI_NBC_IN_HDR10_OUT          , //HAL NEED TO ADD THIS NEW BRANCH
    DRV_HDR10_IN_HDR10_OUT             , //USE HDR FPGA BRANCH

    DRV_DISP_HDR_CFG_MODE_BUTT        
    
}DRV_DISP_HDR_CFG_MODE_E;


typedef struct tagDRV_DISP_HDR_FRAME_ADDR_S
{
	/* Y address*/
    HI_U32  u32PhyAddr_YHead; /* only for compress format */
    HI_U32  u32PhyAddr_Y;
    HI_U64  vir_addr_y;
    HI_U32  u32Stride_Y;

    /* C OR Cb address*/
    HI_U32  u32PhyAddr_CHead;
    HI_U32  u32PhyAddr_C;
    HI_U64  vir_addr_c;
    HI_U32  u32Stride_C;

    /* Cr address*/
    HI_U32  u32PhyAddr_CrHead;
    HI_U32  u32PhyAddr_Cr;
    HI_U32  u32Stride_Cr;

	HI_U32  u32Head_Stride;
    HI_U32  u32Head_Size;
}DRV_DISP_HDR_FRAME_ADDR_S;

typedef struct hiDRV_DISP_HDR_FRAME_INFO_S
{
    HI_DRV_PIXEL_BITWIDTH_E   enBitWidth;   //0:8bit; 1:10bit
    HI_DRV_PIX_FORMAT_E       enPixFormat;   //pixel format

    HI_BOOL                   bDcmpEn;
    HI_BOOL                   bSecurity;
    /*0 means left eye, 1 means right eye.*/
    DRV_DISP_HDR_FRAME_ADDR_S stAddress[2];

	/*low 2 bit, 0 means left eye, 1 means rigth eye.*/
	DRV_DISP_HDR_FRAME_ADDR_S stAddress_lb[2];

} HI_DRV_DISP_HDR_FRAME_INFO_S;

typedef struct hiDRV_DISP_DOLBY_HDR_CFG_S
{
    DRV_DISP_HDR_CFG_MODE_E         enHdrMode;
    HI_BOOL                         bMuteEn;   //whether mute the video channel.
    HI_BOOL                         bGfxEn;
    HI_BOOL                         bSmmu;
    HI_BOOL                         bELValid;  //0 --> EL invalid,indicate no el frame; 1 --> EL valid,indicate has el frame.
    HI_DRV_DISP_HDR_FRAME_INFO_S    stBLFrame;
    HI_DRV_DISP_HDR_FRAME_INFO_S    stELFrame;
 	HI_RECT_S                       stBLResoIn;      //BL input Reso
 	HI_RECT_S                       stBLResoOut;     //BL Output Reso
 	HI_RECT_S                       stELResoIn;      //EL input Reso
 	HI_RECT_S                       stELResoOut;     //EL Output Reso
 	HI_BOOL                         bSdFmt;
	HI_BOOL                         bSecureEn;
	CBB_DISPLAY_INFO_S              stDispInfo;

} HI_DRV_DISP_DOLBY_HDR_CFG_S;

typedef enum tagDRV_DISP_HISI_HDR_CFG_MODE_E
{
    DRV_HISI_HDR10_IN_HDR10_OUT = 0X0,
    DRV_HISI_HDR10_IN_SDR_OUT, 
    DRV_HISI_HDR10_IN_HLG_OUT,

    DRV_HISI_HLG_IN_HDR10_OUT,
    DRV_HISI_HLG_IN_SDR_OUT,
    DRV_HISI_HLG_IN_HLG_OUT,
    
    DRV_HISI_SLF_IN_HDR10_OUT,
    DRV_HISI_SLF_IN_SDR_OUT,
        
    DRV_HISI_TECHNI_IN_HDR10_OUT,
    DRV_HISI_TECHNI_IN_SDR_OUT,

    
    DRV_HISI_HDR_CFG_MODE_BUTT
    
}DRV_DISP_HISI_HDR_CFG_MODE_E;


typedef struct hiDRV_DISP_HISI_HDR_CFG_S
{
    DRV_DISP_HISI_HDR_CFG_MODE_E    enHdrMode;
    HI_BOOL                         bMuteEn;   //whether mute the video channel.
    HI_BOOL                         bGfxEn;
    HI_BOOL                         bSmmu;
    HI_DRV_DISP_HDR_FRAME_INFO_S    stFrame;
    HI_DRV_COLOR_SPACE_E            enInColorSpace; 
    HI_DRV_COLOR_SPACE_E            enOutColorSpace;
 	HI_RECT_S                       stResoIn;   //input Reso
 	HI_RECT_S                       stResoOut;  //output Reso
 	HI_BOOL                         bSdFmt;
	HI_BOOL                         bSecureEn;
	CBB_DISPLAY_INFO_S              stDispInfo;
}HI_DRV_DISP_HISI_HDR_CFG_S;

/*********************HDR hal end**********************************/
/**************************************PLL PARAMETRE END**********************************************/
typedef struct hiDRV_DISP_DOLBY_HDR_PROC_INFO_S
{
    HI_DRV_DISP_DOLBY_HDR_CFG_S   stDolbyHdrCfg;
    HI_BOOL                       bELVaild;
    rpu_ext_config_fixpt_main_t   stCompCfg;
    dm_metadata_t                 stDmCfg;
    DmKsFxp_t                     stDmKs;
}HI_DRV_DISP_DOLBY_HDR_PROC_INFO_S;

typedef struct hiDRV_DISP_HISI_HDR_PROC_INFO_S
{
    HI_DRV_DISP_HISI_HDR_CFG_S    stHisiHdrCfg;
}HI_DRV_DISP_HISI_HDR_PROC_INFO_S;

typedef struct hiDRV_DISP_HDR_PROC_INFO_S
{
    HI_DRV_DISP_WORKING_MODE_E          enWorkMode;
    HI_DRV_HDR_PLAY_MODE_E              enHdrPlayMode;
    HI_DRV_DISP_DOLBY_HDR_PROC_INFO_S   stDolbyHdrProcInfo;
    HI_DRV_DISP_HISI_HDR_PROC_INFO_S    stHisiHdrProcInfo;
}HI_DRV_DISP_HDR_PROC_INFO_S;

typedef struct hiDRV_DISP_DOLBY_HDR_PLAY_INFO_S
{
    HI_DRV_DISP_DOLBY_HDR_CFG_S         stLastHdrCfg;               /* keep hdr config in this interrupt for mute. */
    HI_DRV_DOLBY_METADATA_ASSEMBLE_S    stAssemMd;                  /* store metadata that config in this interrupt.*/
    HI_U8                               u8MdBuf[MAX_MD_BUF_LEN];    /* store update metadata that config in this interrupt.*/
    HI_U32                              u32UpMdLen;                 /* the length of update metadata */

}HI_DRV_DISP_DOLBY_HDR_PLAY_INFO_S;

typedef struct hiDRV_DISP_HISI_HDR_PLAY_INFO_S
{
    HI_DRV_DISP_HISI_HDR_CFG_S          stLastHdrCfg;               /* keep hdr config in this interrupt */
}HI_DRV_DISP_HISI_HDR_PLAY_INFO_S;

typedef struct hiDRV_DISP_PLAY_INFO_S
{
    HI_DRV_DISP_WORKING_MODE_E          enLastWorkMode;             /* workmode in last interrupt*/
    HI_DRV_HDR_PLAY_MODE_E              enLastPlayMode;             /* playmode in last interrupt*/
    HI_DRV_DISP_DOLBY_HDR_PLAY_INFO_S   stDolbyHdrInfo;             /* dolby hdr play info */
    HI_DRV_DISP_HISI_HDR_PLAY_INFO_S    stHisiHdrinfo;              /* Hisi hdr play info */
    HI_DRV_HDMI_HDR_ATTR_S              stLastHdmiHdrAttr;          /* store last hdmi hdr attr.*/
}HI_DRV_DISP_PLAY_INFO_S;//保存本次HDR处理相关信息，播放/工作模式决策以及仅有图形层时需要这些信息

/* this function use for hdr process, it's useless in boot. */
extern HI_S32 DispGetHdmiExportFuncPtr(HDMI_EXPORT_FUNC_S ** pstHdmiFunc);
#endif 

#ifdef VDP_DOLBY_HDR_SUPPORT
/*Dolby output type should match progressive format,otherwise return false.*/
HI_BOOL DISP_OutputTypeAndFormatMatch(HI_DRV_DISP_OUT_TYPE_E enOutputType, HI_DRV_DISP_FMT_E  enFormat);
#else
#define DISP_OutputTypeAndFormatMatch(a,b) HI_TRUE
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

