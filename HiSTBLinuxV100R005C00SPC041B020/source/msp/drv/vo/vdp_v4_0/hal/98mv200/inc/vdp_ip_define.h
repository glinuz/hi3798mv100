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

#ifndef _VDP_IP_DEFINE_H_
#define _VDP_IP_DEFINE_H_

#include "hi_type.h"
#include "vdp_define.h"
//-----------------------------------
//module define 
//-----------------------------------
#define VID_CROP_EN    1
#define VID_DCMP_EN    1
#define ZME_NON_LINEAR 1
#define VID_DRAW_EN    1
#define VID_SHARP_EN   1
#define VID_ZME2_EN    1
#define VID_VMX_EN     1
#define VID_ES_EN      0
#define MIXV_EN        1
#define MIXG_EN        1
#define WBC_CROP_EN    1
#define WBC_CSC_EN     1
#define CDS_EN         1
#define VP0_CSC_EN     0  //define csc0 & csc1 in one csc module
#define VP0_ACC_EN     0
#define GFX_CSC_EN     1
#define VP0_WCG_EN     0
#define SR_EN          1
#define SR_COEF_AXI    0  //sr coef axi
#define SR_COEF_APB    1
#define ZME_NON_LINEAR 1
#define VDP_WATERMARK  0
#define VMX_TC_MULTIPLY_100 0 // for vmx tc multipy 100
#define VID_BT2020_EN 0

#define HI3798MV200_SHARPEN_EN       0
#define HI3798MV200_ACC_EN                  0
#define HI3798MV200_ACM_EN                 1

#define VGA_CLIP       0

#define WBC_GP0_SEL    1 

#define DIMMING_EN     0

#define GFX_BT_TEST     0
#define ESL_TEST     0
#define SST_TEST     0
#define ST_TEST     0





//-----------------------------------
//OFFSET define, for driver
//-----------------------------------

#if 0
#define VID_OFFSET     0x800/4
#define VP_OFFSET      0x800/4
#define GFX_OFFSET     0x200/4
#define GP_OFFSET      0x800/4
#define MIX_OFFSET     0x100/4
#define CHN_OFFSET     0x400/4
#define INTF_OFFSET    0x100/4
#define WBC_OFFSET     0x400/4
#define VID_ZME_OFFSET 0xB200/4
#define SHARPEN_OFFSET 0x400/4
#define VID_VMX_OFFSET 0x100/4
#define VID_NXG_OFFSET 0x50/4

#define REGION_OFFSET  0x8
#else

#define VID_OFFSET     0x800/1
#define VP_OFFSET      0x800/1
#define GFX_OFFSET     0x200/1
#define GP_OFFSET      0x800/1
#define MIX_OFFSET     0x100/1
#define CHN_OFFSET     0x400/1
#define INTF_OFFSET    0x100/1
#define WBC_OFFSET     0x400/1
#define VID_ZME_OFFSET 0xB200/1
#define SHARPEN_OFFSET 0x400/1
#define VID_VMX_OFFSET 0x100/1
#define VID_NXG_OFFSET 0x50/1

#define REGION_OFFSET  0x20



#endif


//-----------------------------------
//Addr define, for driver
//-----------------------------------
#define VDP_BASE_ADDR     0xf8cc0000
#define VDP_AHB_BASE_ADDR     0x20900000
#define HDDATE_BASE_ADDR  (VDP_BASE_ADDR + 0xf000)
#define SDDATE_BASE_ADDR  (VDP_BASE_ADDR + 0xce00)
#define INT_REG_ADDR      (VDP_BASE_ADDR + 0x8)
#define INT_STA_ADDR      (VDP_BASE_ADDR + 0x4)

#define OFL_INT_REG_ADDR  (VDP_BASE_ADDR + 0x18)
#define OFL_INT_STA_ADDR  (VDP_BASE_ADDR + 0x14)

#define VDP_CLKMODE_ADDR     0x1fd00070
//-----------------------------------
//value define:
//-----------------------------------
#define MAX_VALUE      1023   //10bit:1023; 8bit:255; used in ifir_rm

#define  ZME_HPREC        (1<<20)
#define  ZME_VPREC        (1<<12)

#define  GZME_HPREC       (1<<20)
#define  GZME_VPREC       (1<<12)

#define LINK_NUM       4
#define LINK_WB_LENGTH 19*4
#define ACC_CNT_NUM    16

//acm lut size : 5x7x29 or 9x13x29
#define ACM_INDEX1     9
#define ACM_INDEX2     13 

// vpss dbdect 
#define DBD_HYSTRROW_SIZE       4096
#define DBD_BORD_SIZE           4096
#define VDP_DBD_HYCOUTER_INFO  0x0000
#define VDP_DBD_BORDER_INFO    0x2000
#define VDP_DBD_HYSIZE_INFO    0x2200
#define VDP_DBD_HCSIZE_INFO    0x2204
#define VDP_DBD_VYSIZE_INFO    0x2208
#define VDP_DBD_MAX_INDEX      0x220C
#define VDP_DBD_HYSIZE_INDEX   0x2210
#define VDP_DBD_VYSIZE_INDEX   0x2230
#define VDP_DBD_HUSIZE_INDEX   0x2250
#define VDP_DBD_HVSIZE_INDEX   0x2270

///vpss tnr pglb
#define VDP_PGLB_INFO           0x2100

#define IABS(x) (((x) < 0) ? -(x) : (x))
#define CCLIP(x,max,min) (((x) < min) ? min : (((x) > max)?max:(x)))



#define HI_TYPE   1

#ifndef HI_TYPE
typedef            char    HI_S8;
typedef   unsigned short   HI_U16;
typedef            short   HI_S16;
typedef   unsigned int     HI_U32;
typedef   signed   int     HI_S32;

typedef   float            HI_FLOAT;
typedef   double           HI_DOUBLE;

typedef   void             HI_VOID;

typedef enum 
{
    HI_FALSE    = 0,
    HI_TRUE     = 1
} HI_BOOL;
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

typedef enum 
{
    VDP_EDGE_MODE_128  = 0,  
    VDP_EDGE_MODE_256  = 1,  
    VDP_EDGE_MODE_4096 = 2,  
    VDP_EDGE_MODE_BUTT
      
}VDP_AXI_EDGE_MODE_E;

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
    VDP_INT_WBC_VP0 ,
    VDP_INT_WBC_GP0 ,
    VDP_INT_WBC_G0  ,
    VDP_INT_WBC_G4  ,
    VDP_INT_WBC_ME  ,
    VDP_INT_WBC_FI  ,
    VDP_INT_WBC_BMP ,
    VDP_INT_HC_BMP ,
    VDP_INT_WBC_ALL ,
    VDP_INT_RM_CHN0 ,//for RM_Enable
    VDP_INT_RM_CHN1 ,//for RM_Enable
    VDP_INT_RM_CHN2 ,//for RM_Enable
    VDP_INT_UT,
    VDP_INT_WBC_TNR_REF,
    VDP_INT_WBC_TNR_MAD,
    VDP_INT_WBC_TNR_STT,
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

typedef enum tagVDP_LAYER_VP_E
{
    VDP_LAYER_VP0   = 0,
    VDP_LAYER_VP1   = 1,
    VDP_LAYER_DHD0   = 2,
    
    VDP_LAYER_VP_BUTT

} VDP_LAYER_VP_E;

typedef enum tagVDP_DCI_POS_E
{
    VDP_DCI_POS_VID  = 0,
    VDP_DCI_POS_VP   = 1,
    
    VDP_DCI_POS_BUTT

} VDP_DCI_POS_E;

typedef enum tagVDP_LAYER_GP_E
{
    VDP_LAYER_GP0   = 0,
    VDP_LAYER_GP1   = 1,
    
    VDP_LAYER_GP_BUTT

} VDP_LAYER_GP_E;

//-------------------------------------------- 
//add v3 zme used by v3 or wbc_vp 
typedef enum
{
    VDP_WBC_ZME_USE_V3         = 0x0,
    VDP_WBC_ZME_USE_WBC_VP     = 0x1,
    VDP_WBC_ZME_USE_BUTT
} VDP_WBC_ZME_USE_E;

//-------------------------------------------- 
//when wbc connect to VID VP:INPUT FMT
typedef enum
{
    VDP_WBC_VID_INFMT_SP422    = 0x0,
    VDP_WBC_VID_INFMT_SP420    = 0x1,
    VDP_WBC_VID_INFMT_SP444    = 0x2,
    VDP_WBC_VID_INFMT_SP400    = 0x3,

    VDP_WBC_VID_INFMT_BUTT
} VDP_WBC_VID_INFMT;


typedef enum tagVDP_LAYER_WBC_E
{
    VDP_LAYER_WBC_HD0        = 0,
    VDP_LAYER_WBC_GP0        = 1,
    VDP_LAYER_WBC_G0         = 2,
    VDP_LAYER_WBC_VP0        = 3,
    VDP_LAYER_WBC_G4         = 3,
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

typedef enum tagVDP_HZME_MODE_E
{
    VDP_HZME_MODE_COPY   = 0,
    VDP_HZME_MODE_FIR   = 1,
    VDP_HZME_MODE_BUTT

} VDP_HZME_MODE_E;


typedef enum tagVDP_PROC_FMT_E
{
    VDP_PROC_FMT_SP_422      = 0x0,
    VDP_PROC_FMT_SP_420      = 0x1,
    VDP_PROC_FMT_SP_444      = 0x2,//plannar,in YUV color domain
    VDP_PROC_FMT_RGB_888     = 0x3,//package,in RGB color domain
    VDP_PROC_FMT_RGB_444     = 0x4,//plannar,in RGB color domain

    VDP_PROC_FMT_BUTT        
    
}VDP_PROC_FMT_E;

//-------------------------------------------- 
/*
//add VID_VMX LayerID 
typedef enum
{
    VDP_LAYER_VMX_VID0      = 0x0,
    VDP_LAYER_VMX_VID1      = 0x1,
    VDP_LAYER_VMX_VP0       = 0x2,
    VDP_LAYER_VMX_BUTT
} VDP_LAYER_VMX_E;
//add VID_NXG LayerID 
typedef enum
{
    VDP_LAYER_NXG_VID0      = 0x0,
    VDP_LAYER_NXG_DHD0      = 0x0,
    VDP_LAYER_NXG_VID1      = 0x1,
    VDP_LAYER_NXG_DHD1      = 0x1,
    VDP_LAYER_NXG_VP0       = 0x2,
    VDP_LAYER_NXG_BUTT
} VDP_LAYER_NXG_E;

typedef struct tagVDP_NXG_CFG_S
{
	HI_U32 dwm_en;	
	HI_U32 debug_en;	
	HI_U32 frame_rate;	
	HI_U32 keyin;	
	HI_U32 payload_0;	
	HI_U32 payload_1;	
	HI_U32 * setting;	 
	HI_U32 video_yfpos;	
	HI_U32 video_xfpos;	
	HI_U32 video_ylpos;	
	HI_U32 video_xlpos;	
} VDP_NXG_CFG_S;
*/

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
    HI_U32 u32SX;
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
    VDP_DATA_WTH_10_CTS = 2,
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

typedef enum tagVDP_DRAW_PIXEL_MODE_E
{
    VDP_DRAW_PIXEL_MODE_1 = 0,
    VDP_DRAW_PIXEL_MODE_2 = 1,
    VDP_DRAW_PIXEL_MODE_4 = 2,
    VDP_DRAW_PIXEL_MODE_8 = 3,

    VDP_DRAW_PIXEL_MODE_BUTT
}VDP_DRAW_PIXEL_MODE_E;



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
    VDP_VID_PARA_ZME2_HOR     ,
    VDP_VID_PARA_ZME2_VER     ,    
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
    VDP_DISP_DIGFMT_2160P50    ,//3840x2160 
    VDP_DISP_DIGFMT_2160P60_HBI,//3840x2160 
    VDP_DISP_DIGFMT_2160P30    ,//3840x2160 
    VDP_DISP_DIGFMT_4096x2160P60,

    VDP_DISP_DIGFMT_4096x2160P30,

    VDP_DISP_DIGFMT_4096x2160P24,
    VDP_DISP_DIGFMT_2160P120   ,//3840x1080@120
    VDP_DISP_DIGFMT_TESTI      , 
    VDP_DISP_DIGFMT_TESTP      , 
    VDP_DISP_DIGFMT_TESTS      , 
    VDP_DISP_DIGFMT_TESTUT     , 
    VDP_DISP_DIGFMT_1080P_24_FP,
    VDP_DISP_DIGFMT_1080P30,
    VDP_DISP_DIGFMT_1080P_25,
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
    VDP_ZME_MODE_VERBL,
	VDP_ZME_MODE_VERBC,
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

	/*
    VDP_CSC_YUV2YUV     = 1,
    VDP_CSC_YUV2RGB_601    ,
    VDP_CSC_YUV2RGB_709    ,
    VDP_CSC_YUV2YUV_709_601,
    VDP_CSC_YUV2YUV_601_709,
    VDP_CSC_RGB2YUV_601    ,
    VDP_CSC_RGB2YUV_709    ,
	*/


    VDP_CSC_YUV2RGB_601 = 0      , /* YCbCr_601 L  -> RGB */
    VDP_CSC_YUV2RGB_709          , /* YCbCr_709 L  -> RGB */
    VDP_CSC_RGB2YUV_601          , /* RGB          -> YCbCr_601 L */
    VDP_CSC_RGB2YUV_709          , /* RGB          -> YCbCr_709 L */
    VDP_CSC_YUV2YUV_709_601      , /* YCbCr_709 L  -> YCbCr_601 L */
    VDP_CSC_YUV2YUV_601_709      , /* YCbCr_601 L  -> YCbCr_709 L */
    VDP_CSC_YUV2YUV              , /* YCbCr L      -> YCbCr L */

    VDP_CSC_RGB2RGB_709_2020     , /* RGB_709      -> RGB_2020 */
    VDP_CSC_RGB2YUV_2020_2020_L  , /* RGB_2020     -> YCbCr_2020 L */
    VDP_CSC_YUV2RGB_2020_2020_L  , /* YCbCr_2020 L -> RGB_2020 */
    VDP_CSC_RGB2RGB_2020_709     , /* RGB_2020     -> RGB_709 */
    VDP_CSC_RGB2RGB_601_2020     , /* RGB_601      -> RGB_2020 */
    VDP_CSC_RGB2RGB_2020_601     , /* RGB_2020     -> RGB_601 */

    VDP_CSC_YUV2YUV_2020_601_L_L , /* YCbCr_2020 L -> YCbCr_601 L */
    VDP_CSC_RGB2YUV_2020_601_L   , /* RGB_2020     -> YCbCr_601 L */

    VDP_CSC_YUV2RGB_601_FULL     , /* YCbCr_601 F  -> RGB */
    VDP_CSC_YUV2RGB_709_FULL     , /* YCbCr_709 F  -> RGB */
    VDP_CSC_RGB2YUV_601_FULL     , /* RGB          -> YCbCr_601 F */
    VDP_CSC_RGB2YUV_709_FULL     , /* RGB          -> YCbCr_709 F */
    VDP_CSC_RGB2RGB              , /* RGB          -> RGB */


	//for EDA test
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

typedef struct VDP_VDM_CSC_COEF
{
    HI_S16 s16M33YUV2RGB0[3][3]    ;
    HI_S16 s16M33YUV2RGB1[3][3]    ;
    HI_U16 u16ScaleYUV2RGB         ;
    HI_S32 s32InOffsetYUV2RGB0[3]  ;
    HI_S32 s32InOffsetYUV2RGB1[3]  ;
    HI_S32 s32OutOffsetYUV2RGB0[3] ;
    HI_S32 s32OutOffsetYUV2RGB1[3] ;
    HI_U16 u16ClipMinYUV2RGB       ;
    HI_U16 u16ClipMaxYUV2RGB       ;
} VDP_VDM_CSC_COEF_S;



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
	HI_S32	s32_m_BinMid[32];
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

#if VDP_WATERMARK
typedef struct tagVDPWATERMARK_SOFT_S
{
}VDPWATERMARK_SOFT_S
#endif

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
    VDP_LAYER_CONN_DHD0    = 0x0,
    VDP_LAYER_CONN_VPACSC  = 0x0,
    VDP_LAYER_CONN_VIDAZME = 0x1,
    VDP_LAYER_CONN_VIDBZME = 0x2,
    VDP_LAYER_CONN_DHD1 = 0x1,
    VDP_LAYER_CONN_DSD0 = 0x4,
    VDP_LAYER_CONN_VID0 = 0x8,
    VDP_LAYER_CONN_VID3 = 0x10,
    VDP_LAYER_CONN_VID4 = 0x20,
    //for S5V100 WBC_DHD0
    VDP_LAYER_CONN_CBM1 = 0x0,//except HC
    VDP_LAYER_CONN_GP = 0x3,//except HC
    //mod for HiFoneB02 WBC_DHD0
    VDP_LAYER_CONN_CBM2 = 0x0,
    VDP_LAYER_CONN_VP   = 0x2,
    VDP_LAYER_CONN_VPCSC= 0x0,
    VDP_LAYER_CONN_V0ZME= 0x2,

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
	HI_U32 outhstart;	  //12 bit, DCI作用区域
	HI_U32 outhend;		  //12 bit
	HI_U32 outvstart;	  //12 bit
	HI_U32 outvend;		  //12 bit

}VDP_DCI_POS_S;

typedef struct tagVDP_DCI_PARA_S
{
	HI_U32  u32_dci_sel;

	HI_S32  s32_dci_en;
    HI_S32  s32_dci_demoen;
    HI_S32  s32_dci_demomode;
    HI_S32  s32_dci_demopos;

	HI_S32 s32HIST_WGT_EN;

	HI_S32  s32BS_limit_THR;
	HI_S32  s32BS_limit_ABlend;

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
		
	HI_S32  s32OutHStart;	  //12 bit, DCI作用区域
	HI_S32  s32OutHEnd;		  //12 bit
	HI_S32  s32OutVStart;	  //12 bit
	HI_S32  s32OutVEnd;		  //12 bit

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
//	HI_S32  s32Histogram[2][BINNUM];
//	HI_S32  s32HistRShft[2][BINNUM];
	
 //	HI_S32  s32CurFrmNum;
//  HI_S32  s32Format;
//
//	HI_S32 *ps32BinWeight[METRICNUM];
//	HI_U8   u8WgtBitNum;

//	HI_S32  ps32AdjLUT[33], ps32AdjLUT_0[33], ps32AdjLUT_1[33], ps32AdjLUT_2[33];
//
//	HI_S32  s32divLUT[64];
    HI_S32  s32YLinear;
	HI_U8   u8EnManAlgBlend;	//1 bit
	HI_U8   u8ManAlgBlend;		//7 bit
	HI_U8   u8SplitMode; 		//2 bit, 0: manully set split point; 1: use default split type; 2: auto change between peak, mean and valley according to weights
	HI_U8   u8SplitType;		//2 bit, 0: Peak; 1: Mean; 2: Valley
	HI_U8   u8SplitPoint;		//8 bit
	HI_U16	u16ClipLimitL;		//10 bit
	HI_U16	u16ClipLimitR;		//10 bit
	HI_U8   u8HistMapBlend;		//5 bit
	HI_U8   u8FlatMode;			//2 bit, 0: dci with dynamic split point (same as other videos); 1: flat-> dci with fixed split point; 2: flat -> don't apply dci
	HI_U32  u32FlatThrsh;		// 1080p:200


	HI_U8   u8EnBlackStretch;	//1 bit
	HI_S32  s32BendingLevel;	//320 (10bit proc), 80 (8bit proc)
	HI_S32  s32MaxBSLevel;		//160 (10bit proc), 40 (8bit proc)
	HI_U32	u32MinBlackThrsh;	//8 bit
	HI_U8   u8MinBlackBlend;    //5 bit
	HI_S32  s32BSLOffset;		//6 bit (signed)
	HI_U8   u8BSLimitThrsh;		//7 bit
	HI_U8	u8BlackWgtThrsh;	//6 bit
	HI_S32  s32BSDelta[320]; 	//10 bit x 320,RW
	HI_S32  s32BSDeltaInv;    //102 for (1<<14)/160 || (1<<16)/640

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

    //HI_U32 gma_lin_coef_a_0;
    //HI_U32 gma_lin_coef_a_1;
    //HI_U32 gma_lin_coef_a_2;
    //HI_S32 gma_lin_coef_b_0;
    //HI_S32 gma_lin_coef_b_1;
    //HI_S32 gma_lin_coef_b_2;
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

typedef struct tagVDP_WCG_COEF_S
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

}VDP_WCG_COEF_S;

typedef enum tagVDP_SR_CONN_E
{
    VDP_SR_CONN_NULL = 0x0,
    VDP_SR_CONN_DHD0 ,
    VDP_SR_CONN_VP0 ,
    VDP_SR_CONN_GP0 ,
    VDP_SR_CONN_V0  ,
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
    HI_S16   s16GainPos;/*(u,9,7):Max 3.0 */
    HI_S16   s16GainNeg;
    //-------------------------------------------- 
    //change
    HI_U8    u8Coringthr;	
    HI_U8    u8Coringzero;	
    HI_U8    u8Coringratio;	
    //-------------------------------------------- 
    HI_U8    u8OverThrsh;
    HI_U8    u8UnderThrsh;
    HI_U8    u8MixRatio;
    HI_U8    u8WinSize;
    /***limit***/    
    HI_U16    u16LimitRatio;  
    HI_U16    u16LimitPos0 ;   
    HI_U16    u16LimitPos1 ;   
    HI_S16    s16LimitNeg0 ;   
    HI_S16    s16LimitNeg1 ;   
    HI_U16    u16BoundPos  ;    
    HI_S16    s16BoundNeg  ;

    HI_BOOL  bEnable;
} VDP_SHARP_HIGH_FREQ_S;

typedef struct tagVDP_SHARP_GROUP_S
{
    VDP_SHARP_HIGH_FREQ_S stSharpU1;//unit 1

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
#if 0
typedef enum tagVDP_SR_MUX_E
{
    VDP_SR_AND_ZME_EN = 0x0,
    VDP_SR_AND_ZME_EN_L ,
    VDP_SR_AND_ZME_EN_R ,
    VDP_SR_ONLY_ZME_EN ,
    VDP_SR_ZME_EN_BUTT
} VDP_SR_MUX_E;

typedef enum tagVDP_SR_DEMO_E
{
    VDP_SR_DEMO_EN = 0x0,
    VDP_SR_DEMO_EN_L ,
    VDP_SR_DEMO_EN_R ,
    VDP_SR_DEMO_EN_BUTT
} VDP_SR_DEMO_E;
#endif

typedef enum tagVDP_WBC_LOCATE_E
{
    VDP_WBC_LOCATE_CBM1 = 0x0,
    VDP_WBC_LOCATE_CBM2 = 0x1,
    VDP_WBC_LOCATE_E_BUTT
} VDP_WBC_LOCATE_E;

#if 0
typedef enum tagVDP_SR_SERVE_MODE_E
{
    VDP_SR_2IN_2OUT = 0x0,
    VDP_SR_1IN_2OUT ,
    VDP_SR_1IN_1OUT ,
    VDP_SR_SERVE_MODE_BUTT
} VDP_SR_SERVE_MODE_E;
#endif

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

typedef enum tagVDP_SR_MODE_E
{
    VDP_SR_MODE_ALL = 0x0  , 
    VDP_SR_MODE_ONLY_ZME   , 
    VDP_SR_MODE_DEMO_LEFT  , 
    VDP_SR_MODE_DEMO_RIGHT , 
    VDP_SR_MODE_BUTT
} VDP_SR_MODE_E;

typedef struct tagVDP_SR_PARA_INFO
{
    VDP_SR_SHARP_MODE_E hsh_nodir   ; 
    VDP_PROC_FMT_E      ifmt        ; 
    HI_U32              sr_en       ; 
    VDP_SR_SHARP_E      sharp_en    ; 
    HI_U32              smooth_en   ; 
    HI_U32              ringdt_en   ;
    HI_U32              skindt_en   ;
    HI_U32              plus_en     ; 
    HI_U32              mid_en      ; 
    HI_U32              DispMode    ; 
    VDP_SR_MODE_E       sr_mode     ;
}VDP_SR_PARA_INFO;

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
    HI_U32 sharp_coring            ; 
    HI_U32 sharp_gain              ; 
    HI_U32 over_thrsh              ; 
    HI_U32 under_thrsh             ; 
    HI_U32 plus_ctrl               ; 
    HI_U32 grd_hvnear              ;
    HI_U32 clip_data               ;
    HI_U32 y_exp_val               ;
    HI_U32 u_exp_val               ;
    HI_U32 v_exp_val               ;
    HI_U32 y_thres_l               ;
    HI_U32 y_thres_h               ;
    HI_S32 y_slop                  ;
    HI_U32 u_thres_l               ;
    HI_U32 u_thres_h               ;
    HI_S32 u_slop                  ;
    HI_U32 v_thres_l               ;
    HI_U32 v_thres_h               ;
    HI_S32 v_slop                  ;
    HI_U32 grad_flat_thr           ;
    HI_U32 grad_flat_thr2          ;
    HI_U32 grad_thres_l            ;
    HI_U32 grad_thres_h            ;
    HI_U32 grad_slop               ;
    HI_U32 sharp_thres_l           ;
    HI_U32 sharp_thres_h           ;
    HI_U32 sharp_low_ctrl_l        ;
    HI_U32 sharp_low_ctrl_h        ;
    HI_U32 sharp_low_start         ;
    HI_U32 sharp_low_slop          ;
    HI_U32 sharp_high_ctrl_l       ;
    HI_U32 sharp_high_ctrl_h       ;
    HI_U32 sharp_high_start        ;
    HI_S32 sharp_high_slop         ;
    HI_U32 ring_thres_l            ;
    HI_U32 ring_thres_h            ;
    HI_U32 skin_thres_l            ;
    HI_U32 skin_thres_h            ;
    HI_U32 sharp_high_ctrl         ;
    HI_U32 strengh                 ;
    HI_U32 nolinear_ctrl           ;
    HI_U32 edge_factor_ctrl        ;
    HI_U32 shoot_thrsh             ;
    HI_U32 dir_hv_ctrl             ;
    HI_U32 dir_dg_ctrl             ;
    
    HI_U32 zme_en                  ; 
    HI_U32 hoffset                  ; 
    HI_U32 voffset                  ; 
    HI_U32 fir_en                  ; 
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
	char	BLCInputFileBMP[FILE_NAME_SIZE];
	char	BLCOutputFileBMP[FILE_NAME_SIZE];
	HI_S32 s32ColorSpaceIn;
	HI_S32 s32ColorSpaceOut;

	int m_bOutputEN;
	int m_bAutoSettingEN;			//自动配, 1/zonesize,  1/height;
	int m_LEDNUMH;					//horz zone number
	int m_LEDNUMV;					//vert zone number
	int m_BYPASS_LCD;				//如果 = 1, 输出图像值 .
	int m_BYPASS_LED;				//如果=1 输出最大值.
	int m_DISABLE_2D;				//如果=1 输出最大值.
	int m_SEL_UHD;				    //如果=1 输出最大值.
	int m_MODE_INIT_LED;			//0D时初始化与计算的进行blengding
	int m_LEDDRIVE_MAX;
	int m_DIV_HEIGHT;				//1/height
	int m_DIV_WIDTH;				//1/width
	int m_DIV_SEGHEIGHT;			//1/zonesize
	int m_DIV_SEGWIDTH;				//1/zonesize
	int m_LEDSEGH;					//每个zonesize
	int m_LEDSEGV;					//每个zonesize
	int m_COL_STARTLED;				//第一个zone的中心位置.
	int m_ROW_STARTLED;				//第一个zone的中心位置.
	int m_LEDDSTH;					//两个zone的中心点距离.  overlap   zoneSize 允许有overlap
	int m_LEDDSTV;					//v两个zone的中心点距离.
	int m_LPF_MODE;
	int m_GAIN_LED2D;				//4095 与算出来每个zone LED 值得blend              固定
	int m_GAIN_BLDMAX2D;			//算出来每个zone LED值 与每个zone的最大值进行blending  固定
	int m_BITS_B_HIST;
	int m_BITS_W_HIST;
	int m_THR_MD0;
	int m_THR_MD2;
	int m_COR_DSTATLW;
	int m_COR_DSTATMD;
	int m_COR_DSTATHG;
	int m_K_DSTAT0D;				//scene change
	int m_GAIN_LED0D;				//与4095 blending的增益
	int m_GAIN_LED0D_TF;			//时间域滤波
	int m_SC0D_SW;					// scene change传出去给软件。
	int m_LED0D_SW;					//scene change传出去给软件。
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

	
}VDP_DISP_DIMMING_CFG_S;
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

//---------------------------------
//for EC of S5V100
//---------------------------------
typedef enum tagVDP_GP_OUTMODE_E
{
    VDP_GP_OUTMODE_NORMAL   = 0x0,// sr 1in_2out or FHD intf: single channel; else : dual channel; 
    VDP_GP_OUTMODE_2IN_1OUT = 0x1,// online/offline GP 4k in 2k*2k out
    VDP_GP_OUTMODE_BUTT
} VDP_GP_OUTMODE_E;

//---------------------------------
//TNR BEGIN
//---------------------------------
typedef struct tagVDP_TNR_PARA_INFO
{
    HI_U32             tnr_en      ;
    HI_U32             tnr_pgbm_en ;
    HI_U32             tnr_y_bypass;
    HI_U32             tnr_c_bypass;
    HI_U32             market_en   ;
    HI_U32             market_mode ;
    HI_U32             market_coordinate ;
    HI_U32             tnr_c_en    ;
}VDP_TNR_PARA_INFO;

//---------------------------------
//TNR END
//---------------------------------

//---------------------------------
//SNR BEGIN
//---------------------------------
typedef struct tagVDP_SNR_PARA_INFO
{
    HI_U32             snr_en           ;
    HI_U32             lumasnren        ;
    HI_U32             chromasnren      ;
    HI_U32             debugmodeen      ;
    HI_U32             debugmode        ;
    HI_U32             demomodeen       ;
    HI_U32             demomodelr       ;
    HI_U32             demomodecoor       ;
    HI_U32             diredgesmoothen  ;
    HI_U32             motionedgeen     ;
}VDP_SNR_PARA_INFO;

//---------------------------------
//SNR END
//---------------------------------

//---------------------------------
//DBM BEGIN
//---------------------------------
typedef struct tagVDP_DBM_PARA_INFO
{
    HI_U32             in_width      ;
    HI_U32             in_height     ;
    HI_U32             ifmt          ;
    HI_U32             dbm_en        ;
    HI_U32             db_en         ;
    HI_U32             dm_en         ;
    HI_U32             ds_en         ;
    HI_U32             dr_en         ;
    HI_U32             dbm_out_mode  ;
    HI_U32             db_lum_ver_en ;
    HI_U32             db_lum_hor_en ;
    HI_U32             db_chr_ver_en ;
    HI_U32             db_chr_hor_en ;
    HI_U32             market_en     ;
    HI_U32             market_mode   ;
    HI_U32             l_h_bs        ;  // just for the stream mode
    HI_U32             l_v_bs        ;  // just for the stream mode
    HI_U32             c_h_bs        ;  // just for the stream mode

    HI_U32             dbm_protocol         ;  
    HI_U32             db_vdh_trans_info_en ;  
    HI_U32             db_det_en            ;  
    HI_U32             db_blk_mode          ;  
    HI_U32             vdh_db_width         ;  
    HI_U32             vdh_db_height        ;  
    HI_U32             db_stt_offset_x      ;  
    HI_U32             db_stt_offset_y      ;  
    HI_U32             db_end_offset_x      ;  
    HI_U32             db_end_offset_y      ;  
}VDP_DBM_PARA_INFO;

//---------------------------------
//DBM END
//---------------------------------


//---------------------------------
//DB DETECT BEGIN
//---------------------------------
typedef struct tagVDP_DB_DETECT_PARA_INFO
{
    HI_U32             in_width         ;
    HI_U32             in_height        ;
    HI_U32             dbm_en           ;
    HI_U32             det_size_en      ;
    HI_U32             det_hy_en        ;
    HI_U32             det_hc_en        ;
    HI_U32             det_vy_en        ;
    HI_U32             hy_tst_blk_num   ; // just for the stream mode
    HI_U32             hc_tst_blk_num   ; // just for the stream mode
    HI_U32             vy_tst_blk_num   ; // just for the stream mode
    HI_U32             min_blk_size     ; // just for the stream mode
    HI_U32             hy_max_blk_size  ; // just for the stream mode
    HI_U32             hc_max_blk_size  ; // just for the stream mode
    HI_U32             vy_max_blk_size  ; // just for the stream mode
}VDP_DB_DETECT_PARA_INFO;

//---------------------------------
//DB DETECT END
//---------------------------------


//ZME2 coef
typedef struct tagVDP_VID_ZME2_HCOEF_S
{
    HI_S32  zme2_hlcoef00; 
    HI_S32  zme2_hlcoef01; 
    HI_S32  zme2_hlcoef02; 
    HI_S32  zme2_hlcoef03; 
    HI_S32  zme2_hlcoef04; 
    HI_S32  zme2_hlcoef05; 
    HI_S32  zme2_hlcoef06; 
    HI_S32  zme2_hlcoef07; 
    HI_S32  zme2_hlcoef10; 
    HI_S32  zme2_hlcoef11; 
    HI_S32  zme2_hlcoef12; 
    HI_S32  zme2_hlcoef13; 
    HI_S32  zme2_hlcoef14; 
    HI_S32  zme2_hlcoef15; 
    HI_S32  zme2_hlcoef16; 
    HI_S32  zme2_hlcoef17; 

    HI_S32  zme2_hccoef00; 
    HI_S32  zme2_hccoef01; 
    HI_S32  zme2_hccoef02; 
    HI_S32  zme2_hccoef03; 
    HI_S32  zme2_hccoef10; 
    HI_S32  zme2_hccoef11; 
    HI_S32  zme2_hccoef12; 
    HI_S32  zme2_hccoef13;

}VDP_VID_ZME2_HCOEF_S;

typedef struct tagVDP_VID_ZME2_VCOEF_S
{
    HI_S32  zme2_vlcoef00; 
    HI_S32  zme2_vlcoef01; 
    HI_S32  zme2_vlcoef02; 
    HI_S32  zme2_vlcoef03; 
    HI_S32  zme2_vlcoef04; 
    HI_S32  zme2_vlcoef05; 
    HI_S32  zme2_vlcoef10; 
    HI_S32  zme2_vlcoef11; 
    HI_S32  zme2_vlcoef12; 
    HI_S32  zme2_vlcoef13; 
    HI_S32  zme2_vlcoef14; 
    HI_S32  zme2_vlcoef15; 

    HI_S32  zme2_vccoef00; 
    HI_S32  zme2_vccoef01; 
    HI_S32  zme2_vccoef02; 
    HI_S32  zme2_vccoef03; 
    HI_S32  zme2_vccoef10; 
    HI_S32  zme2_vccoef11; 
    HI_S32  zme2_vccoef12; 
    HI_S32  zme2_vccoef13;

}VDP_VID_ZME2_VCOEF_S;

typedef struct tagVDP_VID_CVFIR_VCOEF_S
{

    HI_S32  cvfir_vccoef00; 
    HI_S32  cvfir_vccoef01; 
    HI_S32  cvfir_vccoef02; 
    HI_S32  cvfir_vccoef03; 
    HI_S32  cvfir_vccoef10; 
    HI_S32  cvfir_vccoef11; 
    HI_S32  cvfir_vccoef12; 
    HI_S32  cvfir_vccoef13;

}VDP_VID_CVFIR_VCOEF_S;


#if 0 //VID_VMX_EN
// used for communication between modules
typedef enum tagVDP_VMX_MODE_E
{
    VDP_VMX_TYP = 0,
    VDP_VMX_4K_SMALL_SYMBOLS ,
    VDP_VMX_MAXIMUM_COVERAGE ,
    VDP_VMX_NORM ,
    VDP_VMX_RAND_MULTIPLY_THRESHOLD , //all settings rand but :watermark is on, multiply is 100, threshold value use default value
    VDP_VMX_RAND_THRESHOLD ,    //all seetings rand but :watermark is on, threshold value use default value
    VDP_VMX_RAND ,              //all seetings rand include watermark on or not
    VDP_VMX_MAX  ,
    VDP_VMX_MIN  ,
    VDP_VMX_BUTT
}VDP_VMX_MODE_E;

typedef enum tagVDP_NXG_NXG_MODE_E
{
    VDP_NXG_NXG_TYP  = 0 , 
    VDP_NXG_NXG_NORM = 1 , 

    VDP_NXG_NXG_BUTT
}VDP_NXG_NXG_MODE_E;

#endif

#endif
