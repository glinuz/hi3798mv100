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

//----------------------------------------------------------------------
// Platform define, EDA and FPGA is difference
//----------------------------------------------------------------------
#define FPGA_TEST          1
#define VDP_CBB_FPGA       1
#define VDP_C_COMPILE      0

#define EDA_TEST           (!(FPGA_TEST || VDP_CBB_FPGA || VDP_C_COMPILE))

#if VDP_CBB_FPGA
//#include <linux/kernel.h>
#endif
//----------------------------------------------------------------------
// Test stage define
//----------------------------------------------------------------------
#define ESL_TEST           0
#define BT_TEST            0

//----------------------------------------------------------------------
// Project define
//----------------------------------------------------------------------
#define VDP_HI3798CV200    1     

#define VID_MAX        6
#define GFX_MAX        6
#define WBC_MAX        11
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
#define GP1_EN         1
#define WBC_GP0_EN     1
#define WBC_DHD0_EN    1
#define INTF_EN        0


#define OFL_INT        0

//----------------------------------------------------------------------
// Addr define
// should be modified in different project
//----------------------------------------------------------------------

#if EDA_TEST
  #define VDP_BASE_ADDR      0x1fd00000
#else
  #define VDP_BASE_ADDR      0xf8cc0000
#endif

#define VDP_AHB_BASE_ADDR     0x20900000
#define HDDATE_BASE_ADDR      (VDP_BASE_ADDR + 0xf000)
#define SDDATE_BASE_ADDR      (VDP_BASE_ADDR + 0xce00)
#define INT_REG_ADDR          (VDP_BASE_ADDR + 0x8)
#define INT_STA_ADDR          (VDP_BASE_ADDR + 0x4)

#define OFL_INT_REG_ADDR      (VDP_BASE_ADDR + 0x18)
#define OFL_INT_STA_ADDR      (VDP_BASE_ADDR + 0x14)

#define VDP_CHK_SUM           VDP_BASE_ADDR + 0xc5e0
#define ONE_CHK_CHN           (3+2*3)
#define MAX_CHK_CHN           (ONE_CHK_CHN*CHN_MAX)
#define MAX_CHK_FRM           10000

//vpss tnr/snr/dbm addr offset
#define VDP_DBM_ADDR          0xe000
#define VDP_TNR_ADDR          0xe800
#define VDP_TNR_CLUT_ADDR     0xeb00
#define VDP_SNR_ADDR          0xe500

//----------------------------------------------------------------------
// Offset define
//----------------------------------------------------------------------
#define VID_OFFSET            0x800 
#define VP_OFFSET             0x800
#define GFX_OFFSET            0x200
#define GP_OFFSET             0x800 
#define MIX_OFFSET            0x100
#define CHN_OFFSET            0x400 
#define INTF_OFFSET           0x100
#define WBC_OFFSET            0x400
#define VID_ZME_OFFSET        0xB200
#define SHARPEN_OFFSET        0x400
#define VID_VMX_OFFSET        0x100 
#define VID_NXG_OFFSET        0x50 

#define REGION_OFFSET         0x20 

//----------------------------------------------------------------------
// SPEC define
// should be modified in different project
//----------------------------------------------------------------------
#define VDP_HDR_MODE          1
#define VID_REGION_NUM        16

//acm lut size : 5x7x29 or 9x13x29
#define ACM_INDEX1     9
#define ACM_INDEX2     13 
//----------------------------------------------------------------------
// Module define
//----------------------------------------------------------------------
#define VID_CROP_EN          1
#define VID_DCMP_EN          1
#define ZME_NON_LINEAR       1
#define VID_DRAW_EN          1
#define VID_SHARP_EN         1
#define VID_ZME2_EN          1
#define VID_VMX_EN           1
#define VID_ES_EN            0
#define MIXV_EN              1
#define MIXG_EN              1
#define WBC_CROP_EN          1
#define WBC_CSC_EN           1
#define CDS_EN               1
#define VP0_CSC_EN           0  //define csc0 & csc1 in one csc module
#define VP0_ACC_EN           0
#define GFX_CSC_EN           1
#define VP0_WCG_EN           0
#define SR_EN                1
#define SR_COEF_AXI          0  //sr coef axi
#define SR_COEF_APB          1
#define ZME_NON_LINEAR       1
#define VDP_WATERMARK        0
#define VMX_TC_MULTIPLY_100  1 // for vmx tc multipy 100
#define VID_BT2020_EN        1
#define DIMMING_EN           0

#define VGA_CLIP             0

#define WBC_GP0_SEL          1 


//----------------------------------------------------------------------
// 
//----------------------------------------------------------------------
//low cost cmp for S5V200
#define LOW_COST_CMP          1
#define VDP_TILE_DCMP         1
#define VDP_LINE_HUFF_DCMP    1
#define VDP_LINE_HUFF_CMP     1

#define VID_PIXEL_COPY        0

#define VID_WBC_TNR_CMP       1

#define OSDCMPTODUT           1

#define GCVM_COEF_A_SCALE         5
#define GDM_CSC_ALPHA_WIDTH       8
#define GDM_CSC_ALPHA_MAX_VALUE   ((1<<GDM_CSC_ALPHA_WIDTH)-1)
#define GDM_GMM_LUT_SIZE          64

//----------------------------------------------------------------------
// function define
//----------------------------------------------------------------------
#if EDA_TEST
#define VDP_ASSERT(expr) \
    if (!(expr)) { \
        cout << endl; \
        cout << "Assert Error at:" << endl; \
        cout << ">File name:" << __FILE__ << endl; \
        cout << ">Function :" << __FUNCTION__ << endl; \
        cout << dec << ">Line No. :" << __LINE__ << endl; \
        cout << ">Condition:" << #expr << endl; \
        exit(-1);\
    }
#else
#define VDP_ASSERT(expr) 
#endif

#ifdef __DISP_PLATFORM_BOOT__
  #ifndef HI_MINIBOOT_SUPPORT 
     #include <common.h>
  #else
     #include <stdio.h>
  #endif
  #define VDP_PRINT   printf
#else

#if (EDA_TEST || VDP_C_COMPILE)
    #include <stdio.h>
    #define VDP_PRINT   printf
  #else
    #define VDP_PRINT   printk
  #endif

#endif//_DISP_PLATFORM_BOOT_

#if FPGA_TEST
    #define W_4096    4096
    #define W_2160P   3840
    #define H_2160P   2160
    #define W_1600P   2560
    #define H_1600P   1600
    #define W_1080P   1920
    #define H_1080P   1080
    #define W_720P    1280
    #define H_720P    720
    #define W_D1      720
    #define H_D1      480
    #define W_576P    720
    #define H_576P    576
    #define W_480P    720
    #define H_480P    480
    #define W_HVGA    480
    #define H_HVGA    320
    #define W_CIF     352
    #define H_CIF     288
    #define W_QCIF    176
    #define H_QCIF    144
    #define W_HC      128
    #define H_HC      128
#endif

//----------------------------------------------------------------------
// struct/enum define
//----------------------------------------------------------------------

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

typedef enum 
{
    VDP_EDGE_MODE_128  = 0,  
    VDP_EDGE_MODE_256  = 1,  
    VDP_EDGE_MODE_4096 = 2,  
    VDP_EDGE_MODE_BUTT
      
}VDP_AXI_EDGE_MODE_E;

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
    VDP_INTMSK_WBC_VP0_INT = 0x800,

    VDP_INTMSK_REF_INT   = 0x2000,
    VDP_INTMSK_MAD_INT   = 0x4000,
    VDP_INTMSK_STT_INT   = 0x80000,

    VDP_INTMSK_VDAC0_INT   = 0x100000,
    VDP_INTMSK_VDAC1_INT   = 0x200000,
    VDP_INTMSK_VDAC2_INT   = 0x400000,
    VDP_INTMSK_VDAC3_INT   = 0x800000,

    VDP_INTMSK_DSD0_VTTHD  = 0x10000,
    VDP_INTMSK_DSD0_UFINT  = 0x20000,

    VDP_INTMSK_VSDRRINT    = 0x100000,
    VDP_INTMSK_VADRRINT    = 0x200000,
    VDP_INTMSK_VHDRRINT    = 0x400000,
    VDP_INTMSK_G0RRINT     = 0x800000,

    VDP_INTMSK_G1RRINT     = 0x1000000,

    VDP_INTMSK_LOWDLY_INT  = 0x20000000,
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
    VDP_OFL_INTMSK_HC_BMP_INT   = 0x6000,

    HAL_OFL_INTMSK_ALL          = 0xffffffff

}VDP_OFL_INTMSK_E;

typedef enum tagVDP_INT_E
{
    VDP_INT_VTTHD  = 1,
    VDP_INT_VTTHD2    ,
    VDP_INT_VTTHD3    ,
    VDP_INT_VTTHD4    ,
    VDP_INT_VTTHD5    ,
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
    VDP_LAYER_WBC_HD0        = 0,
    VDP_LAYER_WBC_GP0        = 1,
    VDP_LAYER_WBC_G0         = 2,
    VDP_LAYER_WBC_VP0        = 3,
    VDP_LAYER_WBC_G4         = 4,
    VDP_LAYER_WBC_ME         = 5,
    VDP_LAYER_WBC_FI         = 6,
    VDP_LAYER_WBC_BMP        = 7,
    VDP_LAYER_HC_BMP         = 8,
    VDP_LAYER_WBC_TNR_REF    = 9,
    VDP_LAYER_WBC_TNR_MAD    = 10,
    VDP_LAYER_WBC_TNR_STT3   = 11,
    VDP_LAYER_WBC_DBM_STT1   = 12,
    VDP_LAYER_WBC_DBM_STT2   = 13,

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

//when wbc connect to VID VP:INPUT FMT
typedef enum
{
    VDP_WBC_VID_INFMT_SP422    = 0x0,
    VDP_WBC_VID_INFMT_SP420    = 0x1,
    VDP_WBC_VID_INFMT_SP444    = 0x2,
    VDP_WBC_VID_INFMT_SP400    = 0x3,

    VDP_WBC_VID_INFMT_BUTT
} VDP_WBC_VID_INFMT;


typedef enum tagVDP_V0_FMT_E
{
    VDP_V0_FMT_420    = 0,
    VDP_V0_FMT_422    = 1,
    VDP_V0_FMT_444    = 2,
    VDP_V0_FMT_BUTT
} VDP_V0_FMT_E;

typedef enum tagVDP_DATA_RMODE_E
{
    VDP_RMODE_INTERFACE   = 0,
    VDP_RMODE_INTERLACE   = 0,
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

typedef struct
{
    HI_U32 u32SX;
    HI_U32 u32X;
    HI_U32 u32Y;
    
    HI_U32 u32Wth;
    HI_U32 u32Hgt;
    
} VDP_RECT_S;

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


//-----------------------------------
//video layer
//-----------------------------------
typedef enum tagVDP_DATA_WTH
{
    VDP_DATA_WTH_8  = 0,
    VDP_DATA_WTH_10 = 1,
    VDP_DATA_WTH_BUTT
}VDP_DATA_WTH;

typedef enum tagVDP_REQ_LENGHT
{
    VDP_REQ_LENGHT_16 = 0,
    VDP_REQ_LENGHT_8  = 1,
    VDP_REQ_LENGHT_4  = 2,
    VDP_REQ_LENGHT_BUTT
}VDP_REQ_LENGHT;

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
    VDP_VID_IFMT_SP_TILE     = 0x6,
    VDP_VID_IFMT_SP_TILE_64  = 0x7,
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

typedef enum tagVDP_CMP_COMPONENT_E
{
    VDP_CMP_COMPONENT_Y    = 0,
    VDP_CMP_COMPONENT_C    = 1,
    VDP_CMP_COMPONENT_AR   = 2,  
    VDP_CMP_COMPONENT_GB   = 3,
    VDP_CMP_COMPONENT_YUV  = 4,
    VDP_CMP_COMPONENT_A    = 5,
    VDP_CMP_COMPONENT_R    = 6,
    VDP_CMP_COMPONENT_G    = 7,
    VDP_CMP_COMPONENT_B    = 8,
    VDP_CMP_COMPONENT_ARGB = 9,

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
	VDP_VID_PARA_DCI		 ,
	VDP_VID_PARA_TNR	     ,
	VDP_VID_PARA_TNR_LUT	 ,
	VDP_VID_PARA_SNR		 ,
	VDP_VID_PARA_DBM		 ,
	VDP_VID_PARA_VMX		 ,

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
	VDP_VP_PARA_VMX	    ,

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

    VDP_GP_PARA_HDR_DEGMM    ,
    VDP_GP_PARA_HDR_GMM    ,
    
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

typedef enum tagVDP_WBC_WLEN_E
{
    VDP_WBC_BURST_16 = 0,
    VDP_WBC_BURST_8  = 1,
    VDP_WBC_BURST_4  = 2,
    
    VDP_WBC_BURST_BUUT

}VDP_WBC_WLEN_E;

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
    //MIX0-DHD0
    VDP_CBM_VP0  = 0x1,
    VDP_CBM_GP0  = 0x2,

    //MIX1-MIX_4K
    VDP_CBM_CBM  = 0x1,
    //VDP_CBM_GFX3 = 0x2,

    //MIX2-DSD0
    VDP_CBM_VID3 = 0x1,
    VDP_CBM_VID4 = 0x2,
    VDP_CBM_GP1  = 0x3,
    VDP_CBM_GFX4 = 0x3,
    VDP_CBM_GFX3 = 0x4,

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
    //zreo means no layer driver;
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

	//DHD1
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
    VDP_DISP_DIGFMT_1600P60    ,//2560x1600
    VDP_DISP_DIGFMT_2160P60    ,//3840x2160 
    VDP_DISP_DIGFMT_2160P60_HBI,//3840x2160 
    VDP_DISP_DIGFMT_2160P30    ,//3840x2160 
    VDP_DISP_DIGFMT_4096x2160P60,
    VDP_DISP_DIGFMT_2160P120   ,//3840x1080@120
    VDP_DISP_DIGFMT_TESTI      , 
    VDP_DISP_DIGFMT_TESTP      , 
    VDP_DISP_DIGFMT_TESTS      , 
    VDP_DISP_DIGFMT_TESTUT     , 
    VDP_DISP_DIGFMT_1080P_24_FP,
    VDP_DISP_DIGFMT_1080P30,  
    VDP_DISP_DIGFMT_1080P_24,
    VDP_DISP_DIGFMT_BUTT
} VDP_DISP_DIGFMT_E;

typedef enum tagVDP_DISP_INTF_E
{
    VDP_DISP_INTF_HDMI   = 0,
    VDP_DISP_INTF_VGA    = 1,
    VDP_DISP_INTF_BT1120 = 2,
    //VDP_DISP_INTF_DATE   = 3,
    VDP_DISP_INTF_SDDATE = 3,
    VDP_DISP_INTF_CVBS   = 3,
    VDP_DISP_INTF_LCD    = 4, 
    VDP_DISP_INTF_HDDATE = 5,
    VDP_DISP_INTF_BUTT   = 6,
} VDP_DISP_INTF_E;

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

typedef struct tagVDP_BKG_S
{
    HI_U32 u32BkgY;
    HI_U32 u32BkgU;
    HI_U32 u32BkgV;

    HI_U32 u32BkgA;
    
    HI_U32 bBkType;

} VDP_BKG_S;
//add for mute bk
typedef struct tagVDP_MUTE_BK_S
{
    HI_U32 u32MuteBkgA;
    HI_U32 u32MuteBkgY;
    HI_U32 u32MuteBkgU;
    HI_U32 u32MuteBkgV;

} VDP_MUTE_BK_S;

typedef struct tagVDP_VP_MUTE_BK_S
{
    HI_U32 u32VpMuteBkgA;
    HI_U32 u32VpMuteBkgY;
    HI_U32 u32VpMuteBkgU;
    HI_U32 u32VpMuteBkgV;

} VDP_VP_MUTE_BK_S;

typedef enum tagVDP_DISP_INTF_OFMT_E
{
    VDP_DISP_INTF_OFMT_RGB888 = 0 , 
    VDP_DISP_INTF_OFMT_YUV422  ,
    VDP_DISP_INTF_OFMT_BUTT  ,
} VDP_DISP_INTF_OFMT_E;

typedef enum
{
    //VDP_LAYER_CONN_DHD0    = 0x0,
    VDP_LAYER_CONN_VPACSC  = 0x0,
    VDP_LAYER_CONN_VIDAZME = 0x1,
    VDP_LAYER_CONN_VIDBZME = 0x2,
    //VDP_LAYER_CONN_DHD1 = 0x1,
    //VDP_LAYER_CONN_DSD0 = 0x4,
    //VDP_LAYER_CONN_VID0 = 0x8,
    //VDP_LAYER_CONN_VID3 = 0x10,
    //VDP_LAYER_CONN_VID4 = 0x20,
    //for S5V100 WBC_DHD0
    //VDP_LAYER_CONN_CBM1 = 0x0,//except HC
    //mod for HiFoneB02 WBC_DHD0
    //VDP_LAYER_CONN_CBM2 = 0x0,
    //VDP_LAYER_CONN_VP   = 0x2,
    //VDP_LAYER_CONN_VPCSC= 0x0,
    //VDP_LAYER_CONN_V0ZME= 0x2,

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
    VDP_CHKSUM_DHD0   = 0,
    VDP_CHKSUM_DHD1   = 1,
    VDP_CHKSUM_HDMI   = 2,
    VDP_CHKSUM_VGA    = 3,
    VDP_CHKSUM_BT1120 = 4,
    VDP_CHKSUM_CVBS   = 5,
    
    VDP_CHKSUM_INTF_BUTT
} VDP_CHKSUM_INTF_E;

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

//add v3 zme used by v3 or wbc_vp 
typedef enum
{
    vdp_wbc_zme_use_v3         = 0x0,
    vdp_wbc_zme_use_wbc_vp     = 0x1,
    VDP_WBC_ZME_USE_BUTT
} VDP_WBC_ZME_USE_E;


#endif//_VDP_DEFINE_H_
