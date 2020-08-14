//**********************************************************************
//                                                                          
// Copyright(c)2008,Hisilicon Technologies Co.,Ltd                            
// All rights reserved.                                                     
//                                                                          
// File Name   : vdp_define.h
// Author      : s173141
// Data        : 2012/10/17
// Version     : v1.0
// Abstract    : header of vdp define
//                                                                         
// Modification history                                                    
//----------------------------------------------------------------------  
// Version       Data(yyyy/mm/dd)      name                                  
// Description                                                              
//                                                                          
//
//
//
//
//**********************************************************************

#ifndef _VDP_DEFINE_H_
#define _VDP_DEFINE_H_

#include "hi_type.h"
#include "hi_debug.h"

#ifdef __DISP_PLATFORM_BOOT__
#include <uboot.h>
#define printk printf
#else
#include "linux/kernel.h"
#endif

#define VDP_CLK_MODE_300MHZ     0
#define VDP_CLK_MODE_345_6MHZ   1
#define VDP_CLK_MODE_250MHZ     2
#define VDP_CLK_MODE_200MHZ     3

#define CLOCK_DIV_1  3
#define CLOCK_DIV_2  0
#define CLOCK_DIV_4  1

#define CLK_HDMI_VO_1_1  0
#define CLK_HDMI_VO_1_2  1

#define DISP_BASE_ADDRESS 0xff120000
#define DISP_INT_NUMBER   106
//-----------------------------------

//VOU寄存器列表物理基地址
#define OPTM_REGS_BASE_ADDR     0xff120000//0xf8cc0000
#define OPTM_REGS_SIZE          0x100000

#define HIMAX_REGS_BASE_ADDR    0xFF000000
#define HIMAX_REGS_SIZE         0x100000

//VOU系统时钟控制寄存器
//System control 寄存器
#define OPTM_REGS_VDP_SYS_SET 0xf8a220d8

//VOU PLL 寄存器
#define OPTM_REGS_HPLL01_SET 0xF8A22028
#define OPTM_REGS_HPLL02_SET 0xF8A2202C

typedef enum tagVDP_RM_LAYER_E
{
    VDP_RM_LAYER_VID0 = 0,
    VDP_RM_LAYER_VID1,
    VDP_RM_LAYER_VID2, 
    VDP_RM_LAYER_VID3, 
    VDP_RM_LAYER_VID4, 
    VDP_RM_LAYER_GFX0,   
    VDP_RM_LAYER_GFX1,   
    VDP_RM_LAYER_GFX2,   
    VDP_RM_LAYER_GFX3,   
    VDP_RM_LAYER_GFX4,   
    VDP_RM_LAYER_WBC_HD0,   
    VDP_RM_LAYER_WBC_GP0,   
    VDP_RM_LAYER_GP0,   
    VDP_RM_LAYER_GP1,   
    VDP_RM_LAYER_TT,   

} VDP_RM_LAYER_E;

typedef struct tagLAYER_SIGNED_DATA_S
{
    HI_S16 * s16YData;
    HI_S16 * s16UData;
    HI_S16 * s16VData;
    HI_S16 * s16AData;

    HI_U32    u32WthY;
    HI_U32    u32HgtY;
    HI_U32    u32WthC;
    HI_U32    u32HgtC;

} LAYER_SIGNED_DATA_S;



//-----------------------------------
//-----------------------------------
//debug define 
//-----------------------------------
#define PC_TEST        0
#define SYS_TEST       0
#define EDA_TEST       0
#define FPGA_TEST      0
#define TWOCHN_TEST    0
//-----------------------------------
//architecture define 
//-----------------------------------
#define VID_MAX        8
#define GFX_MAX        6
#define WBC_MAX        6
#define CHN_MAX        3
#define INTF_MAX       4
#define GP_MAX         2
#define VP_MAX         2
#define CBM_MAX        5 // = max_mix_layer_num + 1

#define SD0_EN         0

//layer define
#define VP0_EN         1
#define VP1_EN         0
#define VP2_EN         0
#define GP0_EN         1
#define GP1_EN         0
#define WBC_GP0_EN     1
#define WBC_DHD0_EN    1
#define INTF_EN        0

//multi region define
#define VID_REGION_NUM 16
#define OFL_INT        1
//-----------------------------------
//module define 
//-----------------------------------
#define VID_CROP_EN    1
#define VID_DCMP_EN    0
#define ZME_NON_LINEAR 1
#define VID_DRAW_EN    1
#define MIXV_EN        1
#define MIXG_EN        1
#define WBC_CROP_EN    1
#define WBC_CSC_EN     1
#define CDS_EN         1
#define VP0_CSC_EN     0  //define csc0 & csc1 in one csc module
#define VP0_ACC_EN     0
#define VP0_SR_EN      1  //
#define GFX_CSC_EN     1
#define SR_EN          1
#define ES_EN          1
#define ZME_NON_LINEAR 1

#define VGA_CLIP       0

#define WBC_GP0_SEL    1 

#define  FI_TEST  1

//-----------------------------------
//OFFSET define, for driver
//-----------------------------------
#define VID_OFFSET     0x800/4 
#define VP_OFFSET      0x800/4 
#define GFX_OFFSET     0x200/4 
#define GP_OFFSET      0x800/4 
#define MIX_OFFSET     0x100/4 
#define CHN_OFFSET     0x400/4 
#define INTF_OFFSET    0x100/4 
#define WBC_OFFSET     0x400/4 

#define REGION_OFFSET  0x8 

//-----------------------------------
//Addr define, for driver
//-----------------------------------
#define VDP_BASE_ADDR     0x1fd00000
#define HDDATE_BASE_ADDR  (VDP_BASE_ADDR + 0xf000)
#define SDDATE_BASE_ADDR  (VDP_BASE_ADDR + 0xf200)
#define INT_REG_ADDR      (VDP_BASE_ADDR + 0x8)
#define INT_STA_ADDR      (VDP_BASE_ADDR + 0x4)

#define OFL_INT_REG_ADDR  (VDP_BASE_ADDR + 0x18)
#define OFL_INT_STA_ADDR  (VDP_BASE_ADDR + 0x14)

//-----------------------------------
//value define:
//-----------------------------------
#define MAX_VALUE      1023   //10bit:1023; 8bit:255; used in ifir_rm
#define debug_width      1008 //1280//1920 //
#define  ZME_HPREC        (1<<20)
#define  ZME_VPREC        (1<<12)

#define  GZME_HPREC       (1<<12)
#define  GZME_VPREC       (1<<12)

#define LINK_NUM       4
#define LINK_WB_LENGTH 19*4
#define ACC_CNT_NUM    16
//acm lut size : 5x7x29 or 9x13x29
#define ACM_INDEX1     9
#define ACM_INDEX2     13 

#if FPGA_TEST
    #define W_2560P   2560
    #define H_2560P   1600
    #define W_1080P   1920
    #define H_1080P   1080
    #define W_720P    1280
    #define H_720P    720
    #define W_D1      720
    #define H_D1      480
    #define W_HVGA    480
    #define H_HVGA    320
    #define W_CIF     352
    #define H_CIF     288
    #define W_QCIF    176
    #define H_QCIF    144
    #define W_HC      128
    #define H_HC      128
#endif


//-----------------------------------
//System
//-----------------------------------
typedef enum 
{
    VDP_MASTER0 = 0,  
    VDP_MASTER1 = 1,  
    VDP_MASTER2 = 2,  
    VDP_MASTER3 = 3,  
    VDP_MASTER_BUTT
      
}VDP_MASTER_E;

typedef enum tagVDP_MASTER_ARB_MODE_E
{
    VDP_MASTER_ARB_MODE_ASK = 0 , 
    VDP_MASTER_ARB_MODE_GFX_PRE,
    VDP_MASTER_ARB_MODE_BUTT,
} VDP_MASTER_ARB_MODE_E;
typedef enum tagVDP_AXI_CMD_E
{
    VDP_AXI_CMD_RID0 = 0,
    VDP_AXI_CMD_RID1,
    VDP_AXI_CMD_WID0,

    VDP_AXI_CMD_RID0OTD,
    VDP_AXI_CMD_RID1OTD,
    VDP_AXI_CMD_WID0OTD,

    VDP_AXI_CMD_BUTT
} VDP_AXI_CMD_E;

typedef enum
{
    VDP_INT_CHN0 = 1,
    VDP_INT_CHN1    ,
    VDP_INT_CHN2    ,//DSD0
    VDP_INT_WBC_HD0 ,
    VDP_INT_WBC_GP0 ,
    VDP_INT_WBC_G0  ,
    VDP_INT_WBC_G4  ,
    VDP_INT_WBC_ME  ,
    VDP_INT_WBC_FI  ,
    VDP_INT_WBC_BMP ,
    VDP_INT_UT,
    WRAP_INT
} INT_VECTOR;

typedef enum tagVDP_INTMSK_E
{
    VDP_INTMSK_NONE        = 0,

    VDP_INTMSK_DHD0_VTTHD  = 0x1,
    VDP_INTMSK_DHD0_VTTHD2 = 0x2,
    VDP_INTMSK_DHD0_VTTHD3 = 0x4,
    VDP_INTMSK_DHD0_UFINT  = 0x8,

    VDP_INTMSK_DHD1_VTTHD  = 0x10,
    VDP_INTMSK_DHD1_VTTHD2 = 0x20,
    VDP_INTMSK_DHD1_VTTHD3 = 0x40,
    VDP_INTMSK_DHD1_UFINT  = 0x80,

    VDP_INTMSK_WBC_GP0_INT = 0x100,
    VDP_INTMSK_WBC_HD0_INT = 0x200,
    VDP_INTMSK_WBC_G0_INT  = 0x400,
    VDP_INTMSK_WBC_G4_INT  = 0x800,

    VDP_INTMSK_VDAC0_INT   = 0x1000,
    VDP_INTMSK_VDAC1_INT   = 0x2000,
    VDP_INTMSK_VDAC2_INT   = 0x4000,
    VDP_INTMSK_VDAC3_INT   = 0x8000,

    VDP_INTMSK_DSD0_VTTHD  = 0x10000,
    VDP_INTMSK_DSD0_UFINT  = 0x20000,

    VDP_INTMSK_VSDRRINT    = 0x100000,
    VDP_INTMSK_VADRRINT    = 0x200000,
    VDP_INTMSK_VHDRRINT    = 0x400000,
    VDP_INTMSK_G0RRINT     = 0x800000,

    VDP_INTMSK_G1RRINT     = 0x1000000,

    VDP_INTMSK_LOWDLY_INT  = 0x10000000,
    VDP_INTMSK_UTENDINT    = 0x40000000,
    VDP_INTMSK_BUSEERINT   = 0x80000000,
    
    HAL_INTMSK_ALL         = 0xffffffff

}VDP_INTMSK_E;

typedef enum tagVDP_OFL_INTMSK_E
{
    VDP_OFL_INTMSK_NONE         = 0,

    VDP_OFL_INTMSK_WBC_ALL_INT  = 0x80,
    VDP_OFL_INTMSK_WBC_GP0_INT  = 0x100,
    VDP_OFL_INTMSK_WBC_HD0_INT  = 0x200,
    VDP_OFL_INTMSK_WBC_G0_INT   = 0x400,
    VDP_OFL_INTMSK_WBC_G4_INT   = 0x800,
    VDP_OFL_INTMSK_WBC_ME_INT   = 0x1000,
    VDP_OFL_INTMSK_WBC_FI_INT   = 0x2000,
    VDP_OFL_INTMSK_WBC_BMP_INT  = 0x4000,

    HAL_OFL_INTMSK_ALL          = 0xffffffff

}VDP_OFL_INTMSK_E;
typedef enum tagVDP_INT_E
{
    VDP_INT_VTTHD  = 1,
    VDP_INT_VTTHD2 = 2,
    VDP_INT_VTTHD3 = 3,
    VDP_INT_BUTT 
}VDP_INT_E;

typedef enum tagVDP_LAYER_VID_E
{
    VDP_LAYER_VID0  = 0,
    VDP_LAYER_VID1  = 1,
    VDP_LAYER_VID2  = 2,
    VDP_LAYER_VID3  = 3,
    VDP_LAYER_VID4  = 4,
    VDP_LAYER_VID5  = 5,
    VDP_LAYER_VID6  = 6,//for FI pixel
    VDP_LAYER_VID7  = 7,//for FI info
    
    VDP_LAYER_VID_BUTT

} VDP_LAYER_VID_E;

typedef enum tagVDP_LAYER_GFX_E
{
    VDP_LAYER_GFX0  = 0,
    VDP_LAYER_GFX1  = 1,
    VDP_LAYER_GFX2  = 2,
    VDP_LAYER_GFX3  = 3,
    VDP_LAYER_GFX4  = 4,
    VDP_LAYER_GFX5  = 5,
    
    VDP_LAYER_GFX_BUTT

} VDP_LAYER_GFX_E;

typedef enum tagVDP_LAYER_MASTER_E
{
    VDP_LAYER_MASTER_V0  = 0,
    VDP_LAYER_MASTER_V1,
    VDP_LAYER_MASTER_V2,
    VDP_LAYER_MASTER_V3,
    VDP_LAYER_MASTER_V4,
    VDP_LAYER_MASTER_V5,
    VDP_LAYER_MASTER_V6,
    VDP_LAYER_MASTER_V7,
    VDP_LAYER_MASTER_V8,
    VDP_LAYER_MASTER_V9, 
    VDP_LAYER_MASTER_VHD_STR,
    VDP_LAYER_MASTER_VHD_STW,
    VDP_LAYER_MASTER_VAD_STR,
    VDP_LAYER_MASTER_VAD_STW,
    VDP_LAYER_MASTER_G0,
    VDP_LAYER_MASTER_G1,
    VDP_LAYER_MASTER_G2,
    VDP_LAYER_MASTER_G3,
    VDP_LAYER_MASTER_G4, 
    VDP_LAYER_MASTER_G5,
    VDP_LAYER_MASTER_G6,
    VDP_LAYER_MASTER_G7,
    VDP_LAYER_MASTER_WBC0,
    VDP_LAYER_MASTER_WBC1,
    VDP_LAYER_MASTER_WBC2,
    VDP_LAYER_MASTER_WBC3,
    VDP_LAYER_MASTER_WBC4,
    VDP_LAYER_MASTER_WBC5,
    VDP_LAYER_MASTER_WBC6,
    VDP_LAYER_MASTER_WBC7,  
    VDP_LAYER_MASTER_PARA,
    VDP_LAYER_MASTER_SDDATE,
    
    VDP_LAYER_MASTER_BUTT

} VDP_LAYER_MASTER_E;


typedef enum tagVDP_LAYER_VP_E
{
    VDP_LAYER_VP0   = 0,
    VDP_LAYER_VP1   = 1,
    VDP_LAYER_DHD0   = 2,
    
    VDP_LAYER_VP_BUTT

} VDP_LAYER_VP_E;


typedef enum tagVDP_LAYER_GP_E
{
    VDP_LAYER_GP0   = 0,
    VDP_LAYER_GP1   = 1,
    
    VDP_LAYER_GP_BUTT

} VDP_LAYER_GP_E;

typedef enum tagVDP_LAYER_WBC_E
{
    VDP_LAYER_WBC_HD0  = 0,
    VDP_LAYER_WBC_GP0  = 1,
    VDP_LAYER_WBC_G0   = 2,
    VDP_LAYER_WBC_G4   = 3,
    VDP_LAYER_WBC_ME   = 4,
    VDP_LAYER_WBC_FI   = 5,
    VDP_LAYER_WBC_BMP  = 6,

    VDP_LAYER_WBC_BUTT

} VDP_LAYER_WBC_E;

typedef enum tagVDP_CHN_E
{
    VDP_CHN_DHD0    = 0,
    VDP_CHN_DHD1    = 1,
    VDP_CHN_DSD0    = 2,
    VDP_CHN_WBC0    = 3,
    VDP_CHN_WBC1    = 4,
    VDP_CHN_WBC2    = 5,
    VDP_CHN_WBC3    = 6,
    VDP_CHN_NONE    = 7,
    VDP_CHN_BUTT

} VDP_CHN_E;

typedef enum tagVDP_DATA_RMODE_E
{
    VDP_RMODE_INTERFACE   = 0,
    VDP_RMODE_PROGRESSIVE = 1,
    VDP_RMODE_TOP         = 2,
    VDP_RMODE_BOTTOM      = 3,
    VDP_RMODE_BUTT        

} VDP_DATA_RMODE_E;

typedef enum tagVDP_PROC_FMT_E
{
    VDP_PROC_FMT_SP_422      = 0x0,
    VDP_PROC_FMT_SP_420      = 0x1,
    VDP_PROC_FMT_SP_444      = 0x2,//plannar,in YUV color domain
    VDP_PROC_FMT_RGB_888     = 0x3,//package,in RGB color domain
    VDP_PROC_FMT_RGB_444     = 0x4,//plannar,in RGB color domain

    VDP_PROC_FMT_BUTT        
    
}VDP_PROC_FMT_E;

typedef struct tagVDP_DISP_RECT_S
{
    HI_U32 u32SX;   // source horizontal start position
    HI_U32 u32SY;   // source vertical start position
    
    HI_U32 u32DXS;  // dispaly horizontal start position
    HI_U32 u32DYS;  // display vertical start position

    HI_U32 u32DXL;  // dispaly horizontal end position
    HI_U32 u32DYL;  // display vertical end position
    
    HI_U32 u32VX;   // video horizontal start position
    HI_U32 u32VY;   // video vertical start position

    HI_U32 u32VXL;  // video horizontal start position
    HI_U32 u32VYL;  // video vertical start position
    
    HI_U32 u32IWth; // input width
    HI_U32 u32IHgt; // input height
    HI_U32 u32OWth; // output width
    HI_U32 u32OHgt; // output height

} VDP_DISP_RECT_S;

typedef struct
{
    HI_U32 u32X;
    HI_U32 u32Y;

    HI_U32 u32Wth;
    HI_U32 u32Hgt;
    
} VDP_RECT_S;

//-----------------------------------
//video layer
//-----------------------------------
typedef enum tagVDP_DATA_WTH
{
    VDP_DATA_WTH_8  = 0,
    VDP_DATA_WTH_10 = 1,
    VDP_DATA_WTH_BUTT
}VDP_DATA_WTH;
typedef enum tagVDP_UVORDER_E
{
    VDP_UVORDER_U_FIRST = 0,
    VDP_UVORDER_V_FIRST = 1,
    VDP_UVORDER_BUTT
}VDP_UVORDER_E;

typedef enum tagVDP_VID_IFMT_E
{
    VDP_VID_IFMT_SP_400      = 0x1,
    VDP_VID_IFMT_SP_420      = 0x3,
    VDP_VID_IFMT_SP_422      = 0x4,
    VDP_VID_IFMT_SP_444      = 0x5,
    VDP_VID_IFMT_PKG_UYVY    = 0x9,
    VDP_VID_IFMT_PKG_YUYV    = 0xa,
    VDP_VID_IFMT_PKG_YVYU    = 0xb,

    VDP_VID_IFMT_BUTT        
    
}VDP_VID_IFMT_E;

typedef enum tagVDP_DRAW_MODE_E
{
    VDP_DRAW_MODE_1 = 0,
    VDP_DRAW_MODE_2 = 1,
    VDP_DRAW_MODE_4 = 2,
    VDP_DRAW_MODE_8 = 3,

    VDP_DRAW_MODE_BUTT
}VDP_DRAW_MODE_E;

typedef struct  
{
    HI_U32 u32Id;//layer id for eda test
    HI_U32 u32Enable;
    HI_U32 u32Drr;
    HI_U32 u32LossyEn;
}VDP_CMP_CFG_S;

typedef enum tagVDP_CMP_COMPONENT_E
{
    VDP_CMP_COMPONENT_Y  = 0,
    VDP_CMP_COMPONENT_C  = 1,
    VDP_CMP_COMPONENT_AR = 2,  
    VDP_CMP_COMPONENT_GB = 3,
    VDP_CMP_COMPONENT_YUV = 4,

    VDP_CMP_COMPONENT_BUTT
}VDP_CMP_COMPONENT_E;
 
typedef enum tagVDP_VID_PARA_E
{
    VDP_VID_PARA_ZME_HORL = 0,
    VDP_VID_PARA_ZME_HORC    ,
    VDP_VID_PARA_ZME_VERL    ,
    VDP_VID_PARA_ZME_VERC    ,

    VDP_VID_PARA_ZME_HOR     ,
    VDP_VID_PARA_ZME_VER     ,

    VDP_VID_PARA_BUTT
} VDP_VID_PARA_E;

typedef enum tagVDP_VP_PARA_E
{
    VDP_VP_PARA_ACM     = 0x1,//for coef read and para up
    VDP_VP_PARA_DCI_HBW = 0x2,//for coef read
    VDP_VP_PARA_DCI_DIV = 0x4,//for coef read
    VDP_VP_PARA_DCI_LUT = 0x8,//for coef read
    VDP_VP_PARA_ABC     = 0x9,//for para up
    VDP_VP_PARA_DCI     = 0x9,//for para up
    VDP_VP_PARA_SHARP   = 0x10,//for coef read and para up

    VDP_VP_PARA_BUTT
} VDP_VP_PARA_E;

//-----------------------------------
//graphics layer
//-----------------------------------
typedef enum tagVDP_GFX_IFMT_E
{
    VDP_GFX_IFMT_CLUT_1BPP   = 0x00,
    VDP_GFX_IFMT_CLUT_2BPP   = 0x10,
    VDP_GFX_IFMT_CLUT_4BPP   = 0x20,
    VDP_GFX_IFMT_CLUT_8BPP   = 0x30,

    VDP_GFX_IFMT_ACLUT_44    = 0x38,

    VDP_GFX_IFMT_RGB_444     = 0x40,
    VDP_GFX_IFMT_RGB_555     = 0x41,
    VDP_GFX_IFMT_RGB_565     = 0x42,

    VDP_GFX_IFMT_PKG_UYVY    = 0x43,
    VDP_GFX_IFMT_PKG_YUYV    = 0x44,
    VDP_GFX_IFMT_PKG_YVYU    = 0x45,

    VDP_GFX_IFMT_ACLUT_88    = 0x46,
    VDP_GFX_IFMT_ARGB_4444   = 0x48,
    VDP_GFX_IFMT_ARGB_1555   = 0x49,

    VDP_GFX_IFMT_RGB_888     = 0x50,//24bpp
    VDP_GFX_IFMT_YCBCR_888   = 0x51,//24bpp
    VDP_GFX_IFMT_ARGB_8565   = 0x5a,//24bpp

    VDP_GFX_IFMT_KRGB_888    = 0x60,
    VDP_GFX_IFMT_ARGB_8888   = 0x68,
    VDP_GFX_IFMT_AYCBCR_8888 = 0x69,

    VDP_GFX_IFMT_RGBA_4444   = 0xc8,
    VDP_GFX_IFMT_RGBA_5551   = 0xc9,
    VDP_GFX_IFMT_RGBA_5658   = 0xda,//24bpp
    VDP_GFX_IFMT_RGBA_8888   = 0xe8,
    VDP_GFX_IFMT_YCBCRA_8888 = 0xe9,
    VDP_GFX_IFMT_ABGR_8888   = 0xef,


    VDP_GFX_IFMT_BUTT        
    
}VDP_GFX_IFMT_E;
 
typedef enum tagVDP_GP_ORDER_E
{
    VDP_GP_ORDER_NULL     = 0x0,
    VDP_GP_ORDER_CSC      = 0x1,
    VDP_GP_ORDER_ZME      = 0x2,
    VDP_GP_ORDER_CSC_ZME  = 0x3,
    VDP_GP_ORDER_ZME_CSC  = 0x4,

    VDP_GP_ORDER_BUTT
} VDP_GP_ORDER_E;

typedef enum tagVDP_GFX_BITEXTEND_E
{
    VDP_GFX_BITEXTEND_1ST =   0,  
    VDP_GFX_BITEXTEND_2ND = 0x2,
    VDP_GFX_BITEXTEND_3RD = 0x3,

    VDP_GFX_BITEXTEND_BUTT
}VDP_GFX_BITEXTEND_E;

typedef struct
{
    HI_U32  uAData  ;
    HI_U32  uRData  ;
    HI_U32  uGData  ;
    HI_U32  uBData  ;
} VDP_GFX_LUT_DATA_S;

typedef struct tagVDP_GFX_CKEY_S
{
    HI_U32 u32Key_r_min;
    HI_U32 u32Key_g_min;
    HI_U32 u32Key_b_min;

    HI_U32 u32Key_r_max;
    HI_U32 u32Key_g_max;
    HI_U32 u32Key_b_max;    

    HI_U32 u32Key_r_msk;
    HI_U32 u32Key_g_msk;
    HI_U32 u32Key_b_msk;

    HI_U32 bKeyMode;

    HI_U32 u32KeyAlpha;

} VDP_GFX_CKEY_S;

typedef struct tagVDP_GFX_MASK_S
{
    HI_U32 u32Mask_r;
    HI_U32 u32Mask_g;
    HI_U32 u32Mask_b;

} VDP_GFX_MASK_S;

typedef enum tagVDP_GFX_PARA_E
{
    VDP_GFX_PARA_LUT = 0,
    
    VDP_GFX_PARA_BUTT
} VDP_GFX_PARA_E;

typedef enum tagVDP_GP_PARA_E
{
    VDP_GP_PARA_ZME_HORL = 0,
    VDP_GP_PARA_ZME_HORC  ,
    VDP_GP_PARA_ZME_VERL  ,
    VDP_GP_PARA_ZME_VERC  ,

    VDP_GP_GTI_PARA_ZME_HORL  ,
    VDP_GP_GTI_PARA_ZME_HORC  ,
    VDP_GP_GTI_PARA_ZME_VERL  ,
    VDP_GP_GTI_PARA_ZME_VERC  ,
    
    VDP_GP_PARA_ZME_HOR   ,
    VDP_GP_PARA_ZME_VER   ,
    
    VDP_GP_PARA_BUTT
} VDP_GP_PARA_E;

//-----------------------------------
//wbc layer
//-----------------------------------
typedef enum tagVDP_WBC_FMT_E
{
    VDP_WBC_OFMT_PKG_UYVY = 0,
    VDP_WBC_OFMT_PKG_YUYV = 1,
    VDP_WBC_OFMT_PKG_YVYU = 2,
    VDP_WBC_OFMT_ARGB8888 = 3,
    VDP_WBC_OFMT_SP420    = 4,
    VDP_WBC_OFMT_SP422    = 5,
    VDP_WBC_OFMT_SP400    = 7,
    
    VDP_WBC_OFMT_BUUT

}VDP_WBC_OFMT_E;

typedef enum tagVDP_WBC_DRR_E
{
    VDP_WBC_DRR_0     = 0,
    VDP_WBC_DRR_12P5  = 1,
    VDP_WBC_DRR_25    = 2,
    VDP_WBC_DRR_37P5  = 3,
    VDP_WBC_DRR_50    = 4,
    VDP_WBC_DRR_BUUT

}VDP_WBC_DRR_E;

typedef enum tagVDP_WBC_3D_MODE_E
{
    VDP_WBC_3D_MODE_TWOEYE = 0,
    VDP_WBC_3D_MODE_LEFTEYE = 2,
    VDP_WBC_3D_MODE_RIGHTEYE = 3,

    VDP_WBC_3D_MODE_BUTT
}VDP_WBC_3D_MODE_E;

typedef enum tagVDP_WBC_PARA_E
{
    VDP_WBC_PARA_ZME_HORL = 0,
    VDP_WBC_PARA_ZME_HORC  ,
    VDP_WBC_PARA_ZME_VERL  ,
    VDP_WBC_PARA_ZME_VERC  ,

    VDP_WBC_GTI_PARA_ZME_HORL  ,
    VDP_WBC_GTI_PARA_ZME_HORC  ,
    VDP_WBC_GTI_PARA_ZME_VERL  ,
    VDP_WBC_GTI_PARA_ZME_VERC  ,
    
    VDP_WBC_PARA_ZME_HOR   ,
    VDP_WBC_PARA_ZME_VER   ,
    
    VDP_WBC_PARA_BUTT
} VDP_WBC_PARA_E;

//-----------------------------------
//channel & interface
//-----------------------------------
typedef struct tagVDP_DISP_SYNCINFO_S
{
    HI_U32  bSynm;
    HI_U32  bIop;
    HI_U32  u32Intfb; 
    
    HI_U32  u32Vact ;
    HI_U32  u32Vbb;
    HI_U32  u32Vfb;

    HI_U32  u32Hact;
    HI_U32  u32Hbb;
    HI_U32  u32Hfb;

    HI_U32  u32Bvact;
    HI_U32  u32Bvbb;
    HI_U32  u32Bvfb;

    HI_U32  u32Hpw;
    HI_U32  u32Vpw;
    HI_U32  u32Hmid;

    HI_U32  bIdv;
    HI_U32  bIhs;
    HI_U32  bIvs;

} VDP_DISP_SYNCINFO_S;

typedef struct tagVDP_DISP_SYNCINV_S 
{
    HI_U32  u32FdInv;//Field
    HI_U32  u32VsInv; 
    HI_U32  u32HsInv; 
    HI_U32  u32DvInv; 

} VDP_DISP_SYNCINV_S;

typedef struct tagVDP_MIX_INFO_S
{
    HI_U32 bEnable;

    HI_U32 u32LayerAttr;//0:vp ; 1:gp

    HI_U32 u32GAlpha;
    HI_U32 u32BkgAlpha;

    HI_U32 u32XStart;
    HI_U32 u32YStart;

    HI_U32 u32FkPitch;
    HI_U32 u32BkPitch;

    HI_U32 u32Wth; // blend data width
    HI_U32 u32Hgt; // blend data height

    HI_U32 bPreMultEn;

    HI_U32 bPmSrcEn;
    HI_U32 bPmDstEn;


} VDP_MIX_INFO_S;

typedef enum tagVDP_DISP_MODE_E
{
    VDP_DISP_MODE_2D  = 0,
    VDP_DISP_MODE_SBS = 1,
    VDP_DISP_MODE_LBL = 2,
    VDP_DISP_MODE_FS  = 3,
    VDP_DISP_MODE_TAB = 4,
    VDP_DISP_MODE_FP  = 5,

    VDP_DISP_MODE_BUTT
}VDP_DISP_MODE_E;

typedef enum tagVDP_CBM_MIX_E
{
    VDP_CBM_MIXV0 = 0,
    VDP_CBM_MIXV1 = 1,
    VDP_CBM_MIXG0 = 2,
    VDP_CBM_MIXG1 = 3,
    VDP_CBM_MIX0  = 4,
    VDP_CBM_MIX1  = 5,
    VDP_CBM_MIX2  = 6,
    VDP_CBM_MIX3  = 7,
    VDP_CBM_MIX_BUTT 
}VDP_CBM_MIX_E;

typedef enum tagVDP_CBM_LAYER_E
{
    VDP_CBM_VP0 = 0x1,
    VDP_CBM_GP0 = 0x2,
    VDP_CBM_CBM  = 0x1,
    VDP_CBM_VID3 = 0x1,
    VDP_CBM_VID4 = 0x2,
    VDP_CBM_GP1  = 0x3,
    VDP_CBM_GFX4 = 0x3,
    VDP_CBM_GFX3 = 0x4,
    VDP_CBM_VP1 = 0x1,    
/* no use in S5V100
    //VDP_CBM_VID4= 4,


    VDP_CBM_VID0 = 0x1,
    VDP_CBM_VID1 = 0x9,//pip

    VDP_CBM_GFX0 = 0x2,
    VDP_CBM_GFX4 = 0x4,
    VDP_CBM_GFX2 = 0xa,//hc

    //DSD
    VDP_CBM_VID4 = 0x1,
    VDP_CBM_GFX1 = 0x2,
*/

    VDP_CBM_BUTT 
}VDP_CBM_LAYER_E;

typedef enum
{
    VDP_PRIO_MIXV0_VID0 = 0x1,
    VDP_PRIO_MIXV0_VID1 = 0x2,

    VDP_PRIO_MIXG0_GFX0 = 0x1,
    VDP_PRIO_MIXG0_GFX1 = 0x2,
    VDP_PRIO_MIXG0_GFX2 = 0x3,
    VDP_PRIO_MIXG0_GFX3 = 0x4,

    //DHD0 without VP/GP
    VDP_PRIO_CBM0_VID0  = 0x1,
    VDP_PRIO_CBM0_GFX0  = 0x2,
    VDP_PRIO_CBM0_VID1  = 0x9,//PIP
    VDP_PRIO_CBM0_GFX2  = 0xa,//HC

    //DHD0 with VP/GP
    VDP_PRIO_CBM0_VP0   = 0x1,
    VDP_PRIO_CBM0_GP0   = 0x2,

    //DHD1 without VP/GP
    VDP_PRIO_CBM1_VID3  = 0x1,
    VDP_PRIO_CBM1_VID4  = 0x2,
    VDP_PRIO_CBM1_GP1   = 0x3,
    VDP_PRIO_CBM1_GFX4  = 0x3,
    VDP_PRIO_CBM1_GFX3  = 0x4,//HC

/*no use in S5V100    
    //VDP_PRIO_CBM1_VID1  = 0x9,//PIP

    //DHD0 with GP
    //VDP_PRIO_CBM0_GP1   = 0x3,

    //DSD0
    VDP_PRIO_CBM2_VID4  = 0x1,
    VDP_PRIO_CBM2_GFX1  = 0x2,
    VDP_PRIO_CBM2_GFX2  = 0xa,//HC
*/

}VDP_PRIO_E;

typedef enum tagVDP_DISP_DIGFMT_E
{
    VDP_DISP_DIGFMT_PAL    = 0 , 
    VDP_DISP_DIGFMT_NTSC       , 
    VDP_DISP_DIGFMT_1080P50    , 
    VDP_DISP_DIGFMT_1080P60    , 
    VDP_DISP_DIGFMT_1080I50    , 
    VDP_DISP_DIGFMT_1080I60    , 
    VDP_DISP_DIGFMT_720P50     , 
    VDP_DISP_DIGFMT_720P60     , 
    VDP_DISP_DIGFMT_800x600    , 
    VDP_DISP_DIGFMT_576P       , 
    VDP_DISP_DIGFMT_576I       , 
    VDP_DISP_DIGFMT_480P       , 
    VDP_DISP_DIGFMT_480I       , 
    VDP_DISP_DIGFMT_2560P60    , 
    VDP_DISP_DIGFMT_TESTI      , 
    VDP_DISP_DIGFMT_TESTP      , 
    VDP_DISP_DIGFMT_TESTS      , 
    VDP_DISP_DIGFMT_TESTUT     , 
    VDP_DISP_DIGFMT_1366x768   ,     
  
    VDP_DISP_DIGFMT_BUTT
} VDP_DISP_DIGFMT_E;

typedef enum tagVDP_DISP_INTF_E
{
    VDP_DISP_INTF_HDMI   = 0,
    VDP_DISP_INTF_VGA    = 1,
    VDP_DISP_INTF_BT1120 = 2,
    VDP_DISP_INTF_DATE   = 3,
    VDP_DISP_INTF_SDDATE = 3,
    VDP_DISP_INTF_CVBS   = 3,
    VDP_DISP_INTF_LCD    = 4, 
    VDP_DISP_INTF_HDDATE = 5,
    VDP_DISP_INTF_BUTT   = 6,
} VDP_DISP_INTF_E;

typedef struct tagVDP_DISP_CLIP_S
{
    HI_U32   bClipEn;
    HI_U32   u32ClipLow_y;  
    HI_U32   u32ClipLow_cb;  
    HI_U32   u32ClipLow_cr;  
    
    HI_U32   u32ClipHigh_y;  
    HI_U32   u32ClipHigh_cb;  
    HI_U32   u32ClipHigh_cr;  

} VDP_DISP_CLIP_S;

typedef enum tagVDP_DISP_PARA_E
{
    VDP_DISP_PARA_GMM_R    = 0x0,//for coef read
    VDP_DISP_PARA_GMM_G    = 0x2,//for coef read
    VDP_DISP_PARA_GMM_B    = 0x4,//for coef read
    VDP_DISP_PARA_GMM      = 0x8,//for paraup

    VDP_DISP_PARA_ZME_HORL = 0x10,//for coef read and para up
    VDP_DISP_PARA_ZME_HORC = 0x20,//for coef read and para up
    VDP_DISP_PARA_ZME_VERL = 0x40,//for coef read and para up
    VDP_DISP_PARA_ZME_VERC = 0x80,//for coef read and para up

    VDP_DISP_PARA_ZME_HOR  = 0x81,//for para up
    VDP_DISP_PARA_ZME_VER  = 0x82,//for para up

    VDP_DISP_PARA_DIM_LUT = 0x100,//for coef read and para up
    VDP_DISP_PARA_DIM_HF  = 0x200,//for coef read and para up
    VDP_DISP_PARA_DIM_VF  = 0x400,//for coef read and para up

    VDP_DISP_PARA_BUTT
} VDP_DISP_PARA_E;

//-----------------------------------
//IP define
//-----------------------------------
typedef enum 
{
    VDP_ZME_MODE_HOR = 0,
    VDP_ZME_MODE_VER,

    VDP_ZME_MODE_HORL,  
    VDP_ZME_MODE_HORC,  
    VDP_ZME_MODE_VERL,
    VDP_ZME_MODE_VERC,

    VDP_ZME_MODE_ALPHA,
    VDP_ZME_MODE_ALPHAV,
    VDP_ZME_MODE_VERT,
    VDP_ZME_MODE_VERB,

    VDP_ZME_MODE_ALL,
    VDP_ZME_MODE_NONL,
    VDP_ZME_MODE_BUTT
      
}VDP_ZME_MODE_E;

typedef enum 
{
    VDP_TI_MODE_LUM = 0,  
    VDP_TI_MODE_CHM,  

    VDP_TI_MODE_ALL,
    VDP_TI_MODE_NON,
    VDP_TI_MODE_BUTT
      
}VDP_TI_MODE_E;

typedef enum tagVDP_ZME_ORDER_E
{
    VDP_ZME_ORDER_HV = 0x0,
    VDP_ZME_ORDER_VH = 0x1,

    VDP_ZME_ORDER_BUTT
} VDP_ZME_ORDER_E;

typedef enum tagVDP_DITHER_E
{
    VDP_DITHER_DROP_10   = 0,
    VDP_DITHER_TMP_10    = 1,
    VDP_DITHER_SPA_10    = 2,
    VDP_DITHER_TMP_SPA_8 = 3,
    VDP_DITHER_ROUND_10  = 4,
    VDP_DITHER_ROUND_8   = 5,
    VDP_DITHER_DISEN     = 6,
    VDP_DITHER_BUTT
} VDP_DITHER_E;

typedef struct
{
    HI_U32 dither_coef0;
    HI_U32 dither_coef1;
    HI_U32 dither_coef2;
    HI_U32 dither_coef3;

    HI_U32 dither_coef4;
    HI_U32 dither_coef5;
    HI_U32 dither_coef6;
    HI_U32 dither_coef7;
} VDP_DITHER_COEF_S;

typedef enum tagVDP_IFIRMODE_E
{
    VDP_IFIRMODE_DISEN = 0,
    VDP_IFIRMODE_COPY,
    VDP_IFIRMODE_DOUBLE,
    VDP_IFIRMODE_6TAPFIR,

    VDP_IFIRMODE_BUTT
}VDP_IFIRMODE_E;

typedef struct
{
    HI_U32 acc_multi    ;
    HI_U32 thd_med_high ;
    HI_U32 thd_med_low  ;
    HI_U32 thd_high     ;
    HI_U32 thd_low      ;
} ACCTHD_S;

typedef struct tagVDP_BKG_S
{
    HI_U32 u32BkgY;
    HI_U32 u32BkgU;
    HI_U32 u32BkgV;

    HI_U32 u32BkgA;
    
    HI_U32 bBkType;

} VDP_BKG_S;

typedef enum tagVDP_CSC_MODE_E
{
    /*
    VDP_CSC_YUV2RGB_601 = 0,
    VDP_CSC_YUV2RGB_709    ,
    VDP_CSC_RGB2YUV_601    ,
    VDP_CSC_RGB2YUV_709    ,
    VDP_CSC_YUV2YUV_709_601,
    VDP_CSC_YUV2YUV_601_709,
    VDP_CSC_YUV2YUV,
    VDP_CSC_YUV2YUV_MAX,
    VDP_CSC_YUV2YUV_MIN,
    VDP_CSC_YUV2YUV_RAND,
    */
    VDP_CSC_YUV2YUV     = 1,
    VDP_CSC_YUV2RGB_601    ,
    VDP_CSC_YUV2RGB_709    ,
    VDP_CSC_YUV2YUV_709_601,
    VDP_CSC_YUV2YUV_601_709,
    VDP_CSC_RGB2YUV_601    ,
    VDP_CSC_RGB2YUV_709    ,
    VDP_CSC_YUV2YUV_MAX    ,
    VDP_CSC_YUV2YUV_MIN    ,
    VDP_CSC_YUV2YUV_RAND   ,
   
    VDP_CSC_BUTT
} VDP_CSC_MODE_E;

typedef struct 
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

} VDP_CSC_COEF_S;

typedef struct
{
    HI_S32 csc_in_dc0;
    HI_S32 csc_in_dc1;
    HI_S32 csc_in_dc2;

    HI_S32 csc_out_dc0;
    HI_S32 csc_out_dc1;
    HI_S32 csc_out_dc2;
} VDP_CSC_DC_COEF_S;

typedef struct 
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

    HI_S32 csc_in_dc0;
    HI_S32 csc_in_dc1;
    HI_S32 csc_in_dc2;

    HI_S32 csc_out_dc0;
    HI_S32 csc_out_dc1;
    HI_S32 csc_out_dc2;
} VDP_CSC_CFG_S;

typedef struct tagVDP_ABC_PARA_S
{

    HI_S32 s32_m_abc_en; 
    HI_S32 s32_m_abc_demoen; 
    HI_S32 s32_m_AutoMan[2], s32_m_AdjWrite[2];         
    HI_S32 s32_m_UpdateEn[2];

    HI_S32  s32_m_CorrParam[2];
    HI_S32  s32_m_ReverseSign[2];
    HI_S32  s32_m_NegateValue[2];
    HI_S32  s32_m_InputAdjSet[2];   

    HI_S32  HbwCoef1[2][32];
    HI_S32  HbwCoef2[2][32];//BinWeighting
    
    HI_S32  s32_m_pLmax_1[1024], s32_m_pLmax_2[1024];

    HI_S32 s32_m_NumLevel, s32_m_BitLevel;

    HI_S32 s32_m_NumDiv;

    HI_S32  s32_m_NumBin;
    HI_S32  s32_m_BinUniform;
    HI_S32  s32_m_BinMid[32];
    HI_S32  s32_m_BinPosLow, s32_m_BinPosHigh, s32_m_BinNegLow, s32_m_BinNegHigh;

    HI_S32  s32_m_BrigThres[2], s32_m_DarkThres[2], s32_m_ShiftCtrl[2];

    HI_S32 *s32_m_BinWeighting[2];
    HI_S32 s32_m_BrightH[2], s32_m_BrightL[2], s32_m_DarkL[2], s32_m_DarkH[2], s32_m_AdjForward[2], s32_m_AdjReturn[2];
    HI_S32 s32_m_BinMid_LUT_Num,s32_m_pLmax_1_LUT_Num[2],s32_m_BinWeighting_LUT_Num[2];


    /*to get the histogram of s32CbCrSta_Y*Y+s32CbCrSta_Cb*Cb+s32CbCrSta_Cr*Cr+s32CbCrSta_Offset, */
    HI_S32 s32CbCrSta_en, s32CbCrSta_Y,s32CbCrSta_Cb, s32CbCrSta_Cr, s32CbCrSta_Offset;

    /*to compensate CbCr while Y been changed*/
    HI_S32 s32CbCrComp_en;
    HI_S32 s32CbCrGain_CbCr_THR;
    
    HI_S32 s32CbCrGain1_Y_THR[8]; 
    HI_S32 s32CbCrGain1[9]      ;
    
    HI_S32 s32CbCrGain2_Y_THR[8]; 
    HI_S32 s32CbCrGain2[9]      ; 
    /*new add for divide when CbCr compensation 20120220*/
    HI_S32 s32divLUT[64];
    HI_S32 s32divLUT_Num;
    HI_S32 s32divLUT_BitNum;

    /*new add for limited data(16-235) or full data(0-255) */
    HI_S32 s32Stretch;
    HI_S32 s32ClipRange;
    HI_S32 s32CorrParam_ClipHigh_pos;
    HI_S32 s32CorrParam_ClipHigh_neg;
    HI_S32 s32CorrParam_ClipLow_pos;
    HI_S32 s32CorrParam_ClipLow_neg;

} VDP_ABC_PARA_S;

typedef enum tagVDP_ABC_UNIT_E
{
    VDP_ABC_UNIT_LUM      = 1,
    VDP_ABC_UNIT_CONTRAST = 2,
    VDP_ABC_UNIT_BUTT
}VDP_ABC_UNIT_E;

typedef enum tagVDP_ABC_AUTOMAN_E
{
    VDP_ABC_AUTOMAN_HW = 0,
    VDP_ABC_AUTOMAN_SW = 1,
    VDP_ABC_AUTOMAN_BUTT
}VDP_ABC_AUTOMAN_E;

typedef enum tagVDP_ABC_SHIFT_E
{
    VDP_ABC_SHIFT_7BIT     = 0,//right shift 7bit 
    VDP_ABC_SHIFT_8BIT     = 1,//right shift 8bit 
    VDP_ABC_SHIFT_9BIT     = 2,//right shift 9bit 
    VDP_ABC_SHIFT_10BIT    = 3,//right shift 10bit 
    VDP_ABC_SHIFT_BUTT
}VDP_ABC_SHIFT_E;

typedef struct 
{
    HI_U32 abc_hist_brig_thd;
    HI_U32 abc_hist_dark_thd;

}VDP_ABC_HIST_THD_S;

typedef struct 
{
    HI_U32 abc_brig_thd_low;
    HI_U32 abc_brig_thd_high;
    HI_U32 abc_dark_thd_low;
    HI_U32 abc_dark_thd_high;

}VDP_ABC_THD_S;

typedef struct 
{
    HI_U32 abc_adj_up;
    HI_U32 abc_adj_dn;

}VDP_ABC_PARA_ADJ_S;

typedef struct 
{
    HI_U32 hue_adj_6;
    HI_U32 hue_adj_5;
    HI_U32 hue_adj_4;
    HI_U32 hue_adj_3;
    HI_U32 hue_adj_2;
    HI_U32 hue_adj_1;
    HI_U32 hue_adj_0;

}VDP_ACM_HUE_ADJ_S;

typedef struct 
{
    HI_U32 sat_adj_6;
    HI_U32 sat_adj_5;
    HI_U32 sat_adj_4;
    HI_U32 sat_adj_3;
    HI_U32 sat_adj_2;
    HI_U32 sat_adj_1;
    HI_U32 sat_adj_0;

}VDP_ACM_SAT_ADJ_S;

typedef enum tagVDP_DISP_INTF_OFMT_E
{
    VDP_DISP_INTF_OFMT_RGB888 = 0 , 
    VDP_DISP_INTF_OFMT_YUV422  ,
    VDP_DISP_INTF_OFMT_BUTT  ,
} VDP_DISP_INTF_OFMT_E;

typedef enum
{
    VDP_LAYER_CONN_DHD0 = 0x0,
    VDP_LAYER_CONN_DHD1 = 0x1,
    VDP_LAYER_CONN_DSD0 = 0x4,
    VDP_LAYER_CONN_VID0 = 0x8,
    VDP_LAYER_CONN_VID3 = 0x10,
    VDP_LAYER_CONN_VID4 = 0x20,

    VDP_LAYER_CONN_BUTT = 0,
} VDP_LAYER_CONN_E;

typedef enum tagVDP_CHKSUM_E
{
    VDP_CHKSUM_A = 0,
    VDP_CHKSUM_Y = 1,
    VDP_CHKSUM_U = 2,
    VDP_CHKSUM_V = 3,
    
    VDP_CHKSUM_BUTT
} VDP_CHKSUM_E;

typedef enum tagVDP_CHKSUM_INTF_E
{
    //the value must the same as VDP_DISP_INTF_E
    VDP_CHKSUM_HDMI   = 0,
    VDP_CHKSUM_VGA    = 1,
    VDP_CHKSUM_BT1120 = 2,
    VDP_CHKSUM_CVBS   = 3,
    
    VDP_CHKSUM_INTF_BUTT
} VDP_CHKSUM_INTF_E;

//for Dci
typedef enum tagVDP_DCI_SHIFT_E
{
    SHIFT_6BIT     = 0,//right shift 6bit 
    SHIFT_7BIT     = 1,//right shift 7bit 
    SHIFT_8BIT     = 2,//right shift 8bit 
    SHIFT_9BIT     = 3,//right shift 9bit 
    SHIFT_BUTT
}VDP_DCI_SHIFT_E;

typedef enum tagVDP_DCI_RANGE_E
{
    IN_RANGE,
    OUT_RANGE,
    RANGE_BUTT
}VDP_DCI_RANGE_E;

typedef struct tagVDP_DCI_POS_S
{
    HI_U32 hstart;
    HI_U32 hend;
    HI_U32 vstart;
    HI_U32 vend;

}VDP_DCI_POS_S;

typedef struct tagVDP_DCI_PARA_S
{
    HI_S32  s32_dci_en;
    HI_S32  s32_dci_demoen;

    HI_S32  s32EnSceneChange;
    HI_S32  s32EnManAdj[3];
    HI_S32  s32CbCrComp_en;
    HI_S32  s32CbCrSta_en;
    HI_S32  s32InputFRange;   //  input data range: 1/0 = full range/limited range
    HI_S32  s32OutputFRange;  // output data range: 1/0 = full range/limited range 
    HI_S32  s32HistRShftBit;
    HI_S32  s32HistLPFEn;
    HI_S32  s32EnSWcfg;
    
    HI_S32  s32HStart;
    HI_S32  s32HEnd;
    HI_S32  s32VStart;
    HI_S32  s32VEnd;
    
    HI_S32  s32CbCrSta_Y,s32CbCrSta_Cb, s32CbCrSta_Cr,  s32CbCrSta_Offset;

    HI_S32  s32HistCoring[3];

    HI_S32  s32MetricAlpha[3];
    HI_S32  s32HistABlend;
    HI_S32  s32OrgABlend;

    HI_S32  s32ManAdjWgt[3];

    HI_S32 s32AdjGain_ClipL[3];
    HI_S32 s32AdjGain_ClipH[3];

    HI_S32 s32GlbGain0,s32GlbGain1,s32GlbGain2;

    HI_S32  s32CbCrGainPos_Ythrsh[7];
    HI_S32  s32CbCrGainPos[9];

    HI_S32  s32CbCrGainPos_Slp[8];

    HI_S32  s32CbCrGainNeg_Ythrsh[7];

    HI_S32  s32CbCrGainNeg[9];

    HI_S32  s32CbCrGainNeg_Slp[8];

    HI_S32  ps32AdjLUT_SW[33];
//  HI_S32  s32Histogram[2][BINNUM];
//  HI_S32  s32HistRShft[2][BINNUM];
    
 // HI_S32  s32CurFrmNum;
//  HI_S32  s32Format;
//
//  HI_S32 *ps32BinWeight[METRICNUM];
//  HI_U8   u8WgtBitNum;

//  HI_S32  ps32AdjLUT[33], ps32AdjLUT_0[33], ps32AdjLUT_1[33], ps32AdjLUT_2[33];
//
//  HI_S32  s32divLUT[64];

}VDP_DCI_PARA_S;

typedef enum tagVDP_VP_DCI_ADJ_NUM_E
{
    VDP_VP_DCI_ADJ0    = 0x0,
    VDP_VP_DCI_ADJ1    = 0x1,
    VDP_VP_DCI_ADJ2    = 0x2,

    VDP_VP_DCI_ADJ_BUTT        
    
}VDP_VP_DCI_ADJ_NUM_E;

typedef enum tagVDP_VP_DCI_CTRL_MODE_E
{
    VDP_VP_DCI_CTRL_AUTO    = 0x0,
    VDP_VP_DCI_CTRL_MAN     = 0x1,

    VDP_VP_DCI_CTRL_MODE_BUTT        
    
}VDP_VP_DCI_CTRL_MODE_E;
typedef struct tagVDP_DHD_WCG_COEF_S
{
    HI_U32 u32WcgEn;
    HI_U32 csc2_scale_sel;
    HI_U32 csc2_r_r_coef;
    HI_U32 csc2_r_g_coef;
    HI_U32 csc2_r_b_coef;
    HI_U32 csc2_g_r_coef;
    HI_U32 csc2_g_g_coef;
    HI_U32 csc2_g_b_coef;
    HI_U32 csc2_b_r_coef;
    HI_U32 csc2_b_g_coef;
    HI_U32 csc2_b_b_coef;

    HI_U32 gma_lin_neg_scl ; 
    HI_U32 gma_lin_init_slp; 
    HI_U32 gma_lin_thre_0  ; 
    HI_U32 gma_lin_thre_1  ; 
    HI_U32 gma_lin_thre_2  ; 

    HI_S32 gma_lin_coef_a_0;
    HI_S32 gma_lin_coef_a_1;
    HI_S32 gma_lin_coef_a_2;
    HI_U32 gma_lin_coef_b_0;
    HI_U32 gma_lin_coef_b_1;
    HI_U32 gma_lin_coef_b_2;
    HI_U32 gma_lin_coef_c_0;
    HI_U32 gma_lin_coef_c_1;
    HI_U32 gma_lin_coef_c_2;

    HI_U32 lin_gma_thre_0  ;
    HI_U32 lin_gma_thre_1  ;
    HI_U32 lin_gma_thre_2  ;

    HI_U32 lin_gma_coef_a_0;
    HI_U32 lin_gma_coef_a_1;
    HI_U32 lin_gma_coef_a_2;
    HI_U32 lin_gma_coef_b_0;
    HI_U32 lin_gma_coef_b_1;
    HI_U32 lin_gma_coef_b_2;
    HI_S32 lin_gma_coef_c_0;
    HI_S32 lin_gma_coef_c_1;
    HI_S32 lin_gma_coef_c_2;
}VDP_DHD_WCG_COEF_S;
typedef struct tagVDP_VP_WCG_COEF_S
{
    HI_U32 u32WcgEn;
    HI_U32 csc2_scale_sel;
    HI_U32 csc2_r_r_coef;
    //HI_S32 csc2_r_g_coef;
    //HI_S32 csc2_r_b_coef;
    //HI_S32 csc2_g_r_coef;
    HI_U32 csc2_r_g_coef;
    HI_U32 csc2_r_b_coef;
    HI_U32 csc2_g_r_coef;
    HI_U32 csc2_g_g_coef;
    //HI_S32 csc2_g_b_coef;
    //HI_S32 csc2_b_r_coef;
    //HI_S32 csc2_b_g_coef;
    HI_U32 csc2_g_b_coef;
    HI_U32 csc2_b_r_coef;
    HI_U32 csc2_b_g_coef;
    HI_U32 csc2_b_b_coef;

    HI_U32 gma_lin_neg_scl ; 
    HI_U32 gma_lin_init_slp; 
    HI_U32 gma_lin_thre_0  ; 
    HI_U32 gma_lin_thre_1  ; 
    HI_U32 gma_lin_thre_2  ; 
    HI_S32 gma_lin_coef_a_0;
    HI_S32 gma_lin_coef_a_1;
    HI_S32 gma_lin_coef_a_2;
    HI_U32 gma_lin_coef_b_0;
    HI_U32 gma_lin_coef_b_1;
    HI_U32 gma_lin_coef_b_2;
    HI_U32 gma_lin_coef_c_0;
    HI_U32 gma_lin_coef_c_1;
    HI_U32 gma_lin_coef_c_2;

    HI_U32 lin_gma_thre_0  ;
    HI_U32 lin_gma_thre_1  ;
    HI_U32 lin_gma_thre_2  ;

    HI_U32 lin_gma_coef_a_0;
    HI_U32 lin_gma_coef_a_1;
    HI_U32 lin_gma_coef_a_2;
    HI_U32 lin_gma_coef_b_0;
    HI_U32 lin_gma_coef_b_1;
    HI_U32 lin_gma_coef_b_2;
    HI_S32 lin_gma_coef_c_0;
    HI_S32 lin_gma_coef_c_1;
    HI_S32 lin_gma_coef_c_2;

}VDP_VP_WCG_COEF_S;
typedef enum tagVDP_SR_CONN_E
{
    VDP_SR_CONN_VP0  = 0x2031,
    VDP_SR_CONN_DHD0 = 0x1230,
    VDP_SR_CONN_BUTT
} VDP_SR_CONN_E;
typedef enum tagVDP_LVDS_FORMAT_E
{
    VDP_LVDS_FORMAT_VESA   = 0x0,
    VDP_LVDS_FORMAT_JEIDA  = 0x1,
    VDP_LVDS_FORMAT_FP     = 0x2,
    VDP_LVDS_FORMAT_UD     = 0x3,
    VDP_LVDS_FORMAT_BUTT
} VDP_LVDS_FORMAT_E;

typedef enum tagVDP_LVDS_COMP_SWAP_E
{
    VDP_LVDS_COMP_SWAP_RGB = 0x0,
    VDP_LVDS_COMP_SWAP_RBG = 0x1,
    VDP_LVDS_COMP_SWAP_BGR = 0x2,
    VDP_LVDS_COMP_SWAP_BRG = 0x3,
    VDP_LVDS_COMP_SWAP_GRB = 0x4,
    VDP_LVDS_COMP_SWAP_GBR = 0x5,
    VDP_LVDS_COMP_SWAP_BUTT
} VDP_LVDS_COMP_SWAP_E;

typedef enum tagVDP_LVDS_TEST_MODE_E
{
    VDP_LVDS_TEST_MODE_ZERO  = 0x0,
    VDP_LVDS_TEST_MODE_ONE   = 0x1,
    VDP_LVDS_TEST_MODE_INV   = 0x2,
    VDP_LVDS_TEST_MODE_GRAY  = 0x3,
    VDP_LVDS_TEST_MODE_PRBS7 = 0x4,
    VDP_LVDS_TEST_MODE_BUTT
} VDP_LVDS_TEST_MODE_E;

typedef enum tagVDP_LVDS_INV_E
{
    VDP_LVDS_INV_DATA  = 0x0,
    VDP_LVDS_INV_HSYNC = 0x1,
    VDP_LVDS_INV_VSYNC = 0x2,
    VDP_LVDS_INV_DE    = 0x3,
    VDP_LVDS_INV_RESV  = 0x4,
    VDP_LVDS_INV_BUTT
} VDP_LVDS_INV_E;

typedef enum tagVDP_LVDS_MODE_E
{
    VDP_LVDS_MODE_NULL    = 0x0,
    VDP_LVDS_MODE_SINGLE  = 0x1,
    VDP_LVDS_MODE_DUAL    = 0x3,
    VDP_LVDS_MODE_QUAD    = 0xf,
    VDP_LVDS_MODE_BUTT
} VDP_LVDS_MODE_E;
//---------------------------------
//Sharpen BEGIN
//---------------------------------
//sharpen
typedef struct tagVDP_SHARP_HIGH_FREQ_S
{
    HI_S16   s16Tmp3;
    HI_S16   s16Tmp2;
    HI_S16   s16Tmp1;
    HI_S16   s16Tmp0;
    HI_S16   s16GainPos;/*(s,10,8):Max 3.0 */
    HI_S16   s16GainNeg;
    HI_U8    u8Coring;  
    HI_U8    u8OverThrsh;
    HI_U8    u8UnderThrsh;
    HI_U8    u8MixRatio;
    HI_U8    u8WinSize;
    HI_U16   u16Grd0;/*10bit*/
    HI_U16   u16Grd1;
    HI_U16   u16Grd2;
    HI_U16   u16Grd3;
    HI_S16   s16GainMax;
    HI_S16   s16GainMinL;
    HI_S16   s16GainMinH;
    HI_S16   s16Slp1;/*(s,10,8)*/
    HI_S16   s16Slp2;
    HI_BOOL  bEnable;
    HI_BOOL  bEnAdpGain;
    HI_BOOL  bEnAdpShoot;
} VDP_SHARP_HIGH_FREQ_S;

typedef struct tagVDP_SHARP_GROUP_S
{
    VDP_SHARP_HIGH_FREQ_S stSharpU1;//unit 1
    VDP_SHARP_HIGH_FREQ_S stSharpU2;//unit 2
    VDP_SHARP_HIGH_FREQ_S stSharpU3;//unit 3
    VDP_SHARP_HIGH_FREQ_S stSharpU4;//unit 4
} VDP_SHARP_GROUP_S;

typedef enum tagVDP_SHARP_DIR_E
{
    VDP_SHARP_DIR_H   = 0x0,//horizontal
    VDP_SHARP_DIR_V   = 0x1,//vertical
    VDP_SHARP_DIR_D   = 0x2,//diagonal
    VDP_SHARP_DIR_BUTT          
} VDP_SHARP_DIR_E;

typedef struct tagVDP_SHARP_CFG_S
{
    HI_U32 bSharpenEn;
} VDP_SHARP_CFG_S;

//---------------------------------
//sharpen END
//---------------------------------

//---------------------------------
//SR BEGIN
//---------------------------------
typedef enum tagVDP_SR_MUX_E
{
    VDP_SR_AND_ZME_EN = 0x0,
    VDP_SR_AND_ZME_EN_L ,
    VDP_SR_AND_ZME_EN_R ,
    VDP_SR_ONLY_ZME_EN ,
    VDP_SR_ZME_EN_BUTT
} VDP_SR_MUX_E;

typedef enum tagVDP_WBC_LOCATE_E
{
    VDP_WBC_LOCATE_CBM1 = 0x0,
    VDP_WBC_LOCATE_CBM2 = 0x1,
    VDP_WBC_LOCATE_E_BUTT
} VDP_WBC_LOCATE_E;

typedef enum tagVDP_SR_DEMO_E
{
    VDP_SR_DEMO_EN = 0x0,
    VDP_SR_DEMO_EN_L ,
    VDP_SR_DEMO_EN_R ,
    VDP_SR_DEMO_EN_BUTT
} VDP_SR_DEMO_E;

typedef enum tagVDP_SR_SERVE_MODE_E
{
    VDP_SR_2IN_2OUT = 0x0,
    VDP_SR_1IN_2OUT ,
    VDP_SR_1IN_1OUT ,
    VDP_SR_SERVE_MODE_BUTT
} VDP_SR_SERVE_MODE_E;

typedef enum hiSR_DEMO_MODE_E
{
    SR_DEMO_EN = 0x0,
    SR_DEMO_EN_L ,
    SR_DEMO_EN_R ,
    SR_DEMO_EN_BUTT
} SR_DEMO_MODE_E;

typedef enum tagVDP_SR_SHARP_MODE_E
{
    VDP_SR_SHARP_MODE_DETAIL = 0x0,
    VDP_SR_SHARP_MODE_TRAD ,
    VDP_SR_SHARP_MODE_BUTT
} VDP_SR_SHARP_MODE_E;

typedef enum tagVDP_SR_E
{
    VDP_SR_UENABLE = 0x0,
    VDP_SR_ENABLE ,
    VDP_SR_BUTT
} VDP_SR_E;

typedef enum tagVDP_SR_SHARP_E
{
    VDP_SR_SHARP_UENABLE = 0x0,
    VDP_SR_SHARP_R_ENABLE ,
    VDP_SR_SHARP_L_ENABLE ,
    VDP_SR_SHARP_ENABLE ,
    VDP_SR_SHARP_BUTT
} VDP_SR_SHARP_E;

typedef enum tagVDP_SR_SMOOTH_E
{
    VDP_SR_SMOOTH_UENABLE = 0x0,
    VDP_SR_SMOOTH_ENABLE ,
    VDP_SR_SMOOTH_BUTT
} VDP_SR_SMOOTH_E;

typedef enum tagVDP_SR_PLUS_E
{
    VDP_SR_PLUS_UENABLE = 0x0,
    VDP_SR_PLUS_ENABLE ,
    VDP_SR_PLUS_BUTT
} VDP_SR_PLUS_E;

typedef struct tagVDP_SR_PARA_INFO
{
    VDP_SR_MUX_E        sr_zme_en  ; 
    //VDP_SR_SERVE_MODE_E serve_mode ; 
    VDP_SR_SHARP_MODE_E hsh_nodir  ; 
    //VDP_PROC_FMT_E      ifmt       ; 
    //VDP_SR_E            sr_en      ; 
    VDP_SR_SHARP_E      sharp_en   ; 
    VDP_SR_SMOOTH_E     smooth_en  ; 
    VDP_SR_PLUS_E       plus_en    ; 
}VDP_SR_PARA_INFO;

typedef enum tagVDP_VP_SR_MODE_E
{
    VDP_VP_SR_TYP_1    = 0x0,
    VDP_VP_SR_TYP_2    ,
    VDP_VP_SR_TYP_3    ,
    VDP_VP_SR_TYP_4    ,
    VDP_VP_SR_RAND     ,
    VDP_VP_SR_MAX      ,
    VDP_VP_SR_MIN      ,

    VDP_VP_SR_CFG_BUTT        
}VDP_VP_SR_MODE_E;


typedef struct tagVDP_SR_IORESO
{
    HI_U32 iw  ;
    HI_U32 ih  ;
    HI_U32 ow  ;
    HI_U32 oh  ;
}VDP_SR_IORESO;

typedef struct tagVDP_SR_PARA_S
{
    VDP_SR_PARA_INFO  SrParaInfo   ;

    HI_U32 hfir_order              ; 
    HI_U32 hratio                  ; 
    HI_U32 vratio                  ; 
    HI_U32 grd_sub_ratio           ; 
    HI_S32 grd_thrl                ; 
    HI_S32 grd_thrh                ; 
    HI_U32 grd_thrh_subl           ; 
    HI_U32 grd_add_ctrl            ; 
    HI_U32 disc_detail_gain        ; 
    HI_U32 ensc_detail_gain        ; 
    HI_U32 sharp_coring            ; 
    HI_U32 sharp_gain              ; 
    HI_U32 over_thrsh              ; 
    HI_U32 under_thrsh             ; 
    HI_U32 plus_ctrl               ; 
    HI_U32 zme_en                  ; 
    HI_U32 offset                  ; 
    HI_U32 fir_en                  ; 
    HI_U32 mid_en                  ; 
    HI_U32 srzme_hladdr            ; 
    HI_U32 srzme_hcaddr            ; 
    HI_U32 srzme_vladdr            ; 
    HI_U32 srzme_vcaddr            ; 

}VDP_SR_PARA_S;


//---------------------------------
//SR END
//---------------------------------
//---------------------------------
//DIMMING  BEGIN
//---------------------------------

typedef enum{
    SCL1_TAP12 = 12,
    SCL1_TAP8 = 8,
    SCL1_TAP7 = 7,
    SCL1_TAP6 = 6,
    SCL1_TAP4 = 4,
    SCL1_TAP2 = 2
}PIC_SCL_TAP_E1;
typedef enum{
    SCL1_PHASE512 = 9,
    SCL1_PHASE128 = 7,
    SCL1_PHASE64 = 6,
    SCL1_PHASE32 = 5,
    SCL1_PHASE16 = 4,
    SCL1_PHASE8 = 3,
}PIC_SCL_PHASE_E1;

typedef struct tagVDP_DISP_DIMMING_ZMEV
{
    HI_S32  s32WidthIn;/*（horz/vert,lum/chrom）input width*/
    HI_S32  s32HeightIn;/*（horz/vert,lum/chrom）input height*/
    HI_S32  s32WidthOut;/*（horz/vert,lum/chrom）output width*/
    HI_S32  s32HeightOut;/*（horz/vert,lum/chrom）output height*/
    HI_U32  u32StrideIn;/*（horz/vert,lum/chrom）input stride*/
    HI_U32  u32StrideOut;/*（horz/vert,lum/chrom）output stride*/
    HI_S32  *ps32Coeff;/*（horz/vert,lum/chrom）coefficients*/
    PIC_SCL_TAP_E1    enTap;/*（horz/vert,lum/chrom）tap*/
    PIC_SCL_PHASE_E1  enPhaseShft;
    HI_U32  u32Step;/*（horz/vert,lum/chrom) step*/
    HI_DOUBLE  dPhase;/*（horz/vert,lum/chrom）initial phase*/
    HI_BOOL bEnMedian;/*（horz/vert,lum/chrom）Median enable:HI_TRUE(1):enable,HI_FALSE(0):disable*/
    HI_BOOL bEnScale;/*(horz/vert,lum/chrom) scale enable:HI_TRUE(1):enable,HI_FALSE(0):disable*/
    HI_BOOL bEnFilter;

} VDP_DISP_DIMMING_ZMEV;

typedef struct tagVDP_DISP_DIMMING_ZMEH
{
    HI_S32  s32WidthIn;/*（horz/vert,lum/chrom）input width*/
    HI_S32  s32HeightIn;/*（horz/vert,lum/chrom）input height*/
    HI_S32  s32WidthOut;/*（horz/vert,lum/chrom）output width*/
    HI_S32  s32HeightOut;/*（horz/vert,lum/chrom）output height*/
    HI_U32  u32StrideIn;/*（horz/vert,lum/chrom）input stride*/
    HI_U32  u32StrideOut;/*（horz/vert,lum/chrom）output stride*/
    HI_S32  *ps32Coeff;/*（horz/vert,lum/chrom）coefficients*/
    PIC_SCL_TAP_E1    enTap;/*（horz/vert,lum/chrom）tap*/
    PIC_SCL_PHASE_E1  enPhaseShft;
    HI_U32  u32Step;/*（horz/vert,lum/chrom) step*/
    HI_DOUBLE  dPhase;/*（horz/vert,lum/chrom）initial phase*/
    HI_BOOL bEnMedian;/*（horz/vert,lum/chrom）Median enable:HI_TRUE(1):enable,HI_FALSE(0):disable*/
    HI_BOOL bEnScale;/*(horz/vert,lum/chrom) scale enable:HI_TRUE(1):enable,HI_FALSE(0):disable*/
    HI_BOOL bEnFilter;
} VDP_DISP_DIMMING_ZMEH;
typedef struct tagVDP_DISP_DIMMING_THD_S
{
    HI_U32 u32CoreLow;
    HI_U32 u32CoreMid;
    HI_U32 u32CoreHigh;

    HI_U32 u32X0Low;
    HI_U32 u32X1Low;
    HI_U32 u32X2Low;
    HI_U32 u32X0Mid;
    HI_U32 u32X1Mid;
    HI_U32 u32X2Mid;
    HI_U32 u32X0High;
    HI_U32 u32X1High;
    HI_U32 u32X2High;

    HI_U32 u32G0Low;
    HI_U32 u32G1Low;
    HI_U32 u32G2Low;
    HI_U32 u32G3Low;
    HI_U32 u32G0Mid;
    HI_U32 u32G1Mid;
    HI_U32 u32G2Mid;
    HI_U32 u32G3Mid;
    HI_U32 u32G0High;
    HI_U32 u32G1High;
    HI_U32 u32G2High;
    HI_U32 u32G3High;

    HI_U32 u32K0Low;
    HI_U32 u32K1Low;
    HI_U32 u32K2Low;
    HI_U32 u32K3Low;
    HI_U32 u32K0Mid;
    HI_U32 u32K1Mid;
    HI_U32 u32K2Mid;
    HI_U32 u32K3Mid;
    HI_U32 u32K0High;
    HI_U32 u32K1High;
    HI_U32 u32K2High;
    HI_U32 u32K3High;

} VDP_DISP_DIMMING_THD_S;
typedef struct tagVDP_DISP_DIMMING_FIR_COEF_S
{
    HI_U32 u32HFirTap0;
    HI_U32 u32HFirTap1;
    HI_U32 u32HFirTap2;
    HI_U32 u32HFirTap3;
    HI_U32 u32HFirTap4;

    HI_U32 u32VFirTap0;
    HI_U32 u32VFirTap1;
    HI_U32 u32VFirTap2;
    HI_U32 u32VFirTap3;
    HI_U32 u32VFirTap4;

} VDP_DISP_DIMMING_FIR_COEF_S;

#define  FILE_NAME_SIZE 255
typedef struct tagVDP_DISP_DIMMING_CFG_S
{
    char    BLCInputFileBMP[FILE_NAME_SIZE];
    char    BLCOutputFileBMP[FILE_NAME_SIZE];
    HI_S32 s32ColorSpaceIn;
    HI_S32 s32ColorSpaceOut;

    int m_bOutputEN;
    int m_bAutoSettingEN;           //自动配, 1/zonesize,  1/height;
    int m_LEDNUMH;                  //horz zone number
    int m_LEDNUMV;                  //vert zone number
    int m_BYPASS_LCD;               //如果 = 1, 输出图像值 .
    int m_BYPASS_LED;               //如果=1 输出最大值.
    int m_DISABLE_2D;               //如果=1 输出最大值.
    int m_SEL_UHD;                  //如果=1 输出最大值.
    int m_MODE_INIT_LED;            //0D时初始化与计算的进行blengding
    int m_LEDDRIVE_MAX;
    int m_DIV_HEIGHT;               //1/height
    int m_DIV_WIDTH;                //1/width
    int m_DIV_SEGHEIGHT;            //1/zonesize
    int m_DIV_SEGWIDTH;             //1/zonesize
    int m_LEDSEGH;                  //每个zonesize
    int m_LEDSEGV;                  //每个zonesize
    int m_COL_STARTLED;             //第一个zone的中心位置.
    int m_ROW_STARTLED;             //第一个zone的中心位置.
    int m_LEDDSTH;                  //两个zone的中心点距离.  overlap   zoneSize 允许有overlap
    int m_LEDDSTV;                  //v两个zone的中心点距离.
    int m_LPF_MODE;
    int m_GAIN_LED2D;               //4095 与算出来每个zone LED 值得blend              固定
    int m_GAIN_BLDMAX2D;            //算出来每个zone LED值 与每个zone的最大值进行blending  固定
    int m_BITS_B_HIST;
    int m_BITS_W_HIST;
    int m_THR_MD0;
    int m_THR_MD2;
    int m_COR_DSTATLW;
    int m_COR_DSTATMD;
    int m_COR_DSTATHG;
    int m_K_DSTAT0D;                //scene change
    int m_GAIN_LED0D;               //与4095 blending的增益
    int m_GAIN_LED0D_TF;            //时间域滤波
    int m_SC0D_SW;                  // scene change传出去给软件。
    int m_LED0D_SW;                 //scene change传出去给软件。
    int m_LED0D_CORE_LW;
    int m_LED0D_CORE_MD;
    int m_LED0D_CORE_HG;
    int m_LED0D_X0_LW;
    int m_LED0D_X1_LW;
    int m_LED0D_X2_LW;
    int m_LED0D_K0_LW;
    int m_LED0D_K1_LW;
    int m_LED0D_K2_LW;
    int m_LED0D_K3_LW;
    int m_LED0D_G0_LW;
    int m_LED0D_G1_LW;
    int m_LED0D_G2_LW;
    int m_LED0D_G3_LW;
    int m_LED0D_X0_MD;
    int m_LED0D_X1_MD;
    int m_LED0D_X2_MD;
    int m_LED0D_K0_MD;
    int m_LED0D_K1_MD;
    int m_LED0D_K2_MD;
    int m_LED0D_K3_MD;
    int m_LED0D_G0_MD;
    int m_LED0D_G1_MD;
    int m_LED0D_G2_MD;
    int m_LED0D_G3_MD;
    int m_LED0D_X0_HG;
    int m_LED0D_X1_HG;
    int m_LED0D_X2_HG;
    int m_LED0D_K0_HG;
    int m_LED0D_K1_HG;
    int m_LED0D_K2_HG;
    int m_LED0D_K3_HG;
    int m_LED0D_G0_HG;
    int m_LED0D_G1_HG;
    int m_LED0D_G2_HG;
    int m_LED0D_G3_HG;
    int m_LED2D_CORE_LW;
    int m_LED2D_CORE_MD;
    int m_LED2D_CORE_HG;
    int m_LED2D_X0_LW;
    int m_LED2D_X1_LW;
    int m_LED2D_X2_LW;
    int m_LED2D_K0_LW;
    int m_LED2D_K1_LW;
    int m_LED2D_K2_LW;
    int m_LED2D_K3_LW;
    int m_LED2D_G0_LW;
    int m_LED2D_G1_LW;
    int m_LED2D_G2_LW;
    int m_LED2D_G3_LW;
    int m_LED2D_X0_MD;
    int m_LED2D_X1_MD;
    int m_LED2D_X2_MD;
    int m_LED2D_K0_MD;
    int m_LED2D_K1_MD;
    int m_LED2D_K2_MD;
    int m_LED2D_K3_MD;
    int m_LED2D_G0_MD;
    int m_LED2D_G1_MD;
    int m_LED2D_G2_MD;
    int m_LED2D_G3_MD;
    int m_LED2D_X0_HG;
    int m_LED2D_X1_HG;
    int m_LED2D_X2_HG;
    int m_LED2D_K0_HG;
    int m_LED2D_K1_HG;
    int m_LED2D_K2_HG;
    int m_LED2D_K3_HG;
    int m_LED2D_G0_HG;
    int m_LED2D_G1_HG;
    int m_LED2D_G2_HG;
    int m_LED2D_G3_HG;
    int m_CORE_LED0D;
    int m_K_LED0D;
    int m_GAIN_BLD0D2D; 

    int m_GAIN_LPFH;    
    int m_GAIN_LPFV;
    int m_CORE_LED2D;
    int m_K_LED2D;
    int m_GAIN_TF;

    int m_Gain_Final_2D;
    int m_Offset_Final_2D;
    int m_Clip_btm_Final_2D;
    int m_Clip_up_Final_2D;
    
    int m_Gain_Final_0D;
    int m_Offset_Final_0D;
    int m_Clip_btm_Final_0D;
    int m_Clip_up_Final_0D;
    
    int m_COEF0_LPFH;
    int m_COEF1_LPFH;
    int m_COEF2_LPFH;
    int m_COEF3_LPFH;
    int m_COEF4_LPFH;
    int m_COEF0_LPFV;
    int m_COEF1_LPFV;
    int m_COEF2_LPFV;
    int m_COEF3_LPFV;
    int m_COEF4_LPFV;   
    int m_LCDCOMPGAIN;
    int m_LCDCOMPGAIN_2D;
    int m_LCDG_CORE_LED0D;
    int m_LCDG_TH_LED0D;
    int m_LCDG_K_LED0D;
    int m_LCDG_CORE_LED2D;
    int m_LCDG_K_LED2D; 
    int m_K_LEDPANEL;
    int m_MAX_LEDPANEL;
    int m_K_LEDOUT;
    HI_DOUBLE  dPhaseh;
    HI_BOOL bEnMedianh;
    HI_BOOL bEnScaleh;
    HI_BOOL bEnFilterh;
    HI_DOUBLE  dPhasev;
    HI_BOOL bEnMedianv;
    HI_BOOL bEnScalev;
    HI_BOOL bEnFilterv;

} VDP_DISP_DIMMING_CFG_S;
//---------------------------------
//DIMMING END
//---------------------------------


//---------------------------------
//ES BEGIN
//---------------------------------

typedef struct tagVDP_ES_PARA_INFO
{
    HI_U32            es_en                ;
    HI_U32            mk_en                ;
    HI_U32            mk_mode              ;
    HI_U32            blend_mode           ;
    HI_U32            edge_strength_mode   ;
    HI_U32            alter_dir_en         ;
    HI_U32            check2_en            ;
    HI_U32            check1_en            ;
    HI_U32            dir_mode             ;
    HI_U32            interlaced_proc      ;
}VDP_ES_PARA_INFO;

typedef struct tagVDP_ES_PARA_S
{
    VDP_ES_PARA_INFO  EsParaInfo           ;
    HI_U32            check1_deadzone      ;
    HI_U32            check1_autodz_gain   ;
    HI_U32            check1_gain          ;
    HI_U32            check1_dir_diff_gain ;
    HI_U32            check2_deadzone      ;
    HI_U32            check2_autodz_gain   ;
    HI_U32            check2_gain          ;
    HI_U32            check_gain           ;
    HI_U32            ratio_limit_scale    ;
    HI_S32            ver_min_strength     ;
    HI_U32            edge_smooth_gain     ;
}VDP_ES_PARA_S;

//---------------------------------
//ES END
//---------------------------------

typedef struct tagVDP_CSC_INFO_S
{   
    VDP_CSC_MODE_E          cscMode;
    HI_U32                  u32Bright;
    HI_U32                  u32Contrast;
    HI_U32                  u32Saturation;
    HI_U32                  u32Hue;
    HI_U32                  u32Kr;
    HI_U32                  u32Kg;
    HI_U32                  u32Kb;
} VDP_CSC_INFO_S;

typedef enum 
{
    VDP_DISP_COEFMODE_HOR  = 0,  
    VDP_DISP_COEFMODE_VER,
    VDP_DISP_COEFMODE_LUT,
    VDP_DISP_COEFMODE_GAM,
    VDP_DISP_COEFMODE_ACC,
    VDP_DISP_COEFMODE_ABC,
    VDP_DISP_COEFMODE_ACM,
    VDP_DISP_COEFMODE_NR,
    VDP_DISP_COEFMODE_SHARP,
    VDP_DISP_COEFMODE_DIM,
    VDP_DISP_COEFMODE_DIMZMEH,
    VDP_DISP_COEFMODE_DIMZMEV,

    VDP_DISP_COEFMODE_ALL
}VDP_DISP_COEFMODE_E;


typedef enum tagVDP_IP_SEL_E
{
    VDP_IP_SEL_BYPASS = 0,
    VDP_IP_SEL_CONNECT_VP,
    VDP_IP_SEL_CONNECT_DHD,

    VDP_IP_SEL_BUTT,
}VDP_IP_SEL_E;

#endif

