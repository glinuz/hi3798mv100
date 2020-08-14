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
// $Log: vdp_define.h,v $
//
//
//
//
//**********************************************************************

#ifndef _VDP_DEFINE_H_
#define _VDP_DEFINE_H_
#include "hi_type.h"


#define DISP_BASE_ADDRESS 0xf8cc0000UL
#define DISP_INT_NUMBER   122

#define VDP_VP   1
#define VDP_GP   1
// test switch
#define PC_TEST   0
#define SYS_TEST  0
#define EDA_TEST  0
#define FPGA_TEST 1

#define VID_MAX 5
#define GFX_MAX 5
#define WBC_MAX 4
#define CHN_MAX 2
#define GP_MAX  2
#define VP_MAX  2
#define CBM_MAX  (VP_MAX + GP_MAX)

#define VID_REGION_NUM 16

#define MAX_LAYER (VID_MAX + GFX_MAX)
#define MIX_PARA  8
#define WBCCK_DEBUG 1
#define MIX_DEBUG 1
#define ZME_DEBUG 1

#define VID_OFFSET 0x800/4 
#define VP_OFFSET  0x800/4 
#define GFX_OFFSET 0x800/4 
#define GP_OFFSET  0x800/4 
#define MIX_OFFSET 0x100/4 
#define CHN_OFFSET 0x400/4
#define SHARPEN_OFFSET 0x400/4

#define WBC_OFFSET 0x400/4 

#define REGION_OFFSET 0x8 
#define WBC_GP0_SEL 1 

#define MAX_VALUE 255

#define HI_TYPE   1
#define LOOPBREAK 5

#ifdef RERUN

#else

#define ENV_DEBUG 1

#endif

#if !(FPGA_TEST)
#define MyAssert(x) { if (!(x)) { printf("\nErr @ (%s, %d)\n", __FILE__, __LINE__); \
                                                         exit(-__LINE__); } }
#else
#define MyAssert(x) 
#endif


#define MAX_PICTURE_WIDTH  2600
#define MAX_PICTURE_HEIGHT 1600

#define   HI_SUCCESS          0
#define   HI_FAILURE          (-1)
#define   HI_POINT_NULL       (-2)
#define   HI_PARAM_ERRO       (-3)

#define   VDP_BASE_ADDR  0xf8cc0000
#define   HDDATE_BASE_ADDR  (VDP_BASE_ADDR + 0xf000)
#define   SDDATE_BASE_ADDR  (VDP_BASE_ADDR + 0xf200)
#define   INT_REG_ADDR  (VDP_BASE_ADDR + 0x8)
#define   INT_STA_ADDR  (VDP_BASE_ADDR + 0x4)

#define  ACK_REQ_ACCEPT   0
#define  ACK_MASTER_FULL  1
#define  ACK_SLAVE_FULL   2

#define  BUS_RD_WTH    32
 

#define VID_ZME_TAP_HL 8
#define VID_ZME_TAP_HC 4
#define VID_ZME_TAP_VL_VID0 6
#define VID_ZME_TAP_VL_VID3 4
#define VID_ZME_TAP_VC 4

#define VID_SR_ZME_TAP_HL 8
#define VID_SR_ZME_TAP_HC 8
#define VID_SR_ZME_TAP_VL 6
#define VID_SR_ZME_TAP_VC 6

#define VID_ZME_TAP_HLTI 8
#define VID_ZME_TAP_HCTI 4
#define VID_ZME_TAP_VLTI 2
#define VID_ZME_TAP_VCTI 2

#define GFX_ZME_TAP_HL 8
#define GFX_ZME_TAP_HC 8
#define GFX_ZME_TAP_VL 4
#define GFX_ZME_TAP_VC 4

#define GFX_ZME_TAP_HLTI 2
#define GFX_ZME_TAP_HCTI 2
#define GFX_ZME_TAP_VLTI 2
#define GFX_ZME_TAP_VCTI 2

#define WBC_HD_ZME_TAP_HL 8
#define WBC_HD_ZME_TAP_HC 4
#define WBC_HD_ZME_TAP_VL 4
#define WBC_HD_ZME_TAP_VC 4

#define VID_ZME_HPHA 32
#define VID_ZME_VPHA 32

#define GFX_ZME_HPHA 8
#define GFX_ZME_VPHA 16

#define  ZME_HPREC    (1<<20)
#define  ZME_VPREC    (1<<12)

#define  GZME_HPREC    (1<<20)
#define  GZME_VPREC    (1<<12)

#define IABS(x) (((x) < 0) ? -(x) : (x))
#define CCLIP(x,max,min) (((x) < min) ? min : (((x) > max)?max:(x)))

#ifdef DEBUG
    #define W_2560P   256
    #define H_2560P   160
    #define W_2160P   384
    #define H_2160P   216
    #define W_1080P   192
    #define H_1080P   128
    #define W_720P    128
    #define H_720P    72
    #define W_D1      72
    #define H_D1      48
    #define W_CIF     32
    #define H_CIF     32
#else //normal mode
    #define W_2560P   2560
    #define H_2560P   1600
    #define W_2160P   3840
    #define H_2160P   2160
    #define W_1080P   1920
    #define H_1080P   1080
    #define W_720P    1280
    #define H_720P    720
    #define W_D1      720
    #define H_D1      480
	#define W_576P	  720
	#define H_576P	  576
    #define W_576I	  720
    #define H_576I	  576
    #define W_CIF     352
    #define H_CIF     288
#endif

#define AXI_AVLD2ARDY_MIN  (VDP_BASE_ADDR+0xff00)
#define AXI_AVLD2ARDY_MAX  (VDP_BASE_ADDR+0xff04)
#define AXI_ARDY2AVLD_MIN  (VDP_BASE_ADDR+0xff08)
#define AXI_ARDY2AVLD_MAX  (VDP_BASE_ADDR+0xff0c)
#define AXI_FIRST_RVLD     (VDP_BASE_ADDR+0xff10)
#define AXI_RVLD           (VDP_BASE_ADDR+0xff14)
#define AXI_WVLD2RDY       (VDP_BASE_ADDR+0xff18)
#define LINK_NUM 4
#define LINK_WB_LENGTH 19*4
#define ACC_CNT_NUM 16
#define MULTI_AREA_NUM  16 //for muti_area  now 16 area

#define OPTM_AA_ACC_COEF_NUM 256
#define OPTM_AA_ACC_TABLE_SIZE 45
#define OPTM_AA_GMM_COEF_NUM 257*3


#ifndef HI_TYPE
typedef            char    HI_S8;
typedef   unsigned short   HI_U16;
typedef            short   HI_S16;
typedef   unsigned int     HI_U32;
typedef   signed   int     HI_S32;
typedef   unsigned char    HI_U8;
typedef   bool             HI_BOOL;
typedef   float            HI_FLOAT;
typedef   double           HI_DOUBLE;

typedef   void             HI_VOID;
#endif

typedef   HI_VOID*         HAL_VIDEO_HANDLE_S;
typedef   HI_VOID*         HAL_DISP_HANDLE_S;


#ifndef HI_TYPE
typedef enum 
{
    HI_FALSE    = 0,
    HI_TRUE     = 1
} HI_BOOL;
#endif


// used for communication between modules
typedef enum tagVDP_GFX_PARA_E
{
    VDP_GFX_PARA_LUT = 0,

    VDP_GFX_PARA_BUTT
} VDP_GFX_PARA_E;
typedef enum 
{
    VDP_MASTER0 = 0,  
    VDP_MASTER1 = 1,  
    VDP_MASTER_BUTT
      
}VDP_MASTER_E;
typedef enum tagVDP_UVORDER_E
{
    VDP_UVORDER_U_FIRST = 0,
    VDP_UVORDER_V_FIRST = 1,
    VDP_UVORDER_BUTT
}VDP_UVORDER_E;

typedef enum
{
    VDP_INT_CHN0 = 1,
    VDP_INT_CHN1    ,
    VDP_INT_WBC_HD0 ,
    VDP_INT_WBC_GP0 ,
    VDP_INT_WBC_G0 ,
    VDP_INT_WBC_G4 ,    
	VDP_INT_RM_CHN0 ,//for RM_Enable
    VDP_INT_RM_CHN1 ,//for RM_Enable
    VDP_INT_RM_CHN2 ,//for RM_Enable
    VDP_INT_UT,
    WRAP_INT
} INT_VECTOR;

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


typedef enum tagVDP_LAYER_VID_E
{
    VDP_LAYER_VID0  = 0,
    VDP_LAYER_VID1  = 1,
    VDP_LAYER_VID2  = 2,
    VDP_LAYER_VID3  = 3,
    VDP_LAYER_VID4  = 4,
    VDP_LAYER_VID5  = 5,
    
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

typedef enum tagVDP_RM_READ_E
{
    VDP_ZME_READ    = 0,
    VDP_SHARPEN_READ = 1, 
    VDP_DCI_READ     = 2, 
    VDP_GAMMA_READ   = 3, 
    VDP_ACM_READ     = 4, 

} VDP_RM_READ_E ;

typedef struct tagVDP_DCI_POS_S
{
    HI_U32 hstart;
    HI_U32 hend;
    HI_U32 vstart;
    HI_U32 vend;

}VDP_DCI_POS_S;

typedef struct tagVDP_DCI_PARA_S
{
	HI_U32  u32_dci_sel;

	HI_S32  s32_dci_en;
    HI_S32  s32_dci_demoen;
    HI_S32  s32_dci_demomode;
    HI_S32  s32_dci_demopos;

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

typedef enum tagVDP_LAYER_VP_E
{
    VDP_LAYER_VP0   = 0,
    VDP_LAYER_VP1   = 1,
    
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
    VDP_CHN_WBC0    = 2,
    VDP_CHN_WBC1    = 3,
    VDP_CHN_WBC2    = 4,
    VDP_CHN_WBC3    = 5,
    VDP_CHN_NONE    = 6,
    VDP_CHN_BUTT

} VDP_CHN_E;
typedef enum tagVDP_WBC_LOCATE_E
{
    VDP_WBC_DHD_LOCATE_CBM= 0,
    VDP_WBC_DHD_LOCATE_IPU= 1,
    VDP_WBC_DHD_LOCATE_BUTT

} VDP_WBC_LOCATE_E;

typedef enum tagVDP_DATA_WTH
{
    VDP_DATA_WTH_8  = 0,
    VDP_DATA_WTH_10 = 1,

    VDP_DATA_WTH_BUTT
}VDP_DATA_WTH;

typedef enum tagVDP_VID_IFMT_E
{
    VDP_VID_IFMT_SP_400      = 0x1,
    VDP_VID_IFMT_SP_420      = 0x3,
    VDP_VID_IFMT_SP_422      = 0x4,
    VDP_VID_IFMT_SP_444      = 0x5,
    VDP_VID_IFMT_SP_TILE     = 0x6,
    VDP_VID_IFMT_PKG_UYVY    = 0x9,
    VDP_VID_IFMT_PKG_YUYV    = 0xa,
    VDP_VID_IFMT_PKG_YVYU    = 0xb,

    VDP_VID_IFMT_BUTT        
    
}VDP_VID_IFMT_E;
 
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
    VDP_GFX_IFMT_ABGR_8888 = 0xef,

    VDP_GFX_IFMT_BUTT        
    
}VDP_GFX_IFMT_E;
 
typedef enum tagVDP_PROC_FMT_E
{
    VDP_PROC_FMT_SP_422      = 0x0,
    VDP_PROC_FMT_SP_420      = 0x1,
    VDP_PROC_FMT_SP_444      = 0x2,
    VDP_PROC_FMT_RGB_888     = 0x3,//package,in RGB color domain
    VDP_PROC_FMT_RGB_444     = 0x4,//plannar,in RGB color domain

    VDP_PROC_FMT_BUTT        
    
}VDP_PROC_FMT_E;

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

typedef enum tagVDP_DATA_RMODE_E
{
    VDP_RMODE_INTERFACE   = 0,
    VDP_RMODE_INTERLACE   = 0,
    VDP_RMODE_PROGRESSIVE = 1,
    VDP_RMODE_TOP         = 2,
    VDP_RMODE_BOTTOM      = 3,
    VDP_RMODE_BUTT        

} VDP_DATA_RMODE_E;


typedef enum tagVDP_INTMSK_E
{
    VDP_INTMSK_NONE      = 0,

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

    VDP_INTMSK_WBC2TEINT = 0x800,

    VDP_INTMSK_LNKTEINT  = 0x1000,

    VDP_INTMSK_WBC4INT   = 0x20000,
    VDP_INTMSK_WBC5INT   = 0x40000,
    VDP_INTMSK_NRWBCINT  = 0x80000,

    VDP_INTMSK_VSDRRINT  = 0x100000,
    VDP_INTMSK_VADRRINT  = 0x200000,
    VDP_INTMSK_VHDRRINT  = 0x400000,
    VDP_INTMSK_G0RRINT   = 0x800000,

    VDP_INTMSK_G1RRINT   = 0x1000000,

    VDP_INTMSK_LOWDLY_INT= 0x10000000,
    VDP_INTMSK_UTENDINT  = 0x40000000,
    VDP_INTMSK_BUSEERINT = 0x80000000,
    
    HAL_INTMSK_ALL       = 0xffffffff

}VDP_INTMSK_E;

typedef enum tagVDP_INT_E
{
    VDP_INT_VTTHD  = 1,
    VDP_INT_VTTHD2 = 2,
    VDP_INT_VTTHD3 = 3,
    VDP_INT_BUTT 
}VDP_INT_E;

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

typedef enum tagVDP_GP_ORDER_E
{
    VDP_GP_ORDER_NULL     = 0x0,
    VDP_GP_ORDER_CSC      = 0x1,
    VDP_GP_ORDER_ZME      = 0x2,
    VDP_GP_ORDER_CSC_ZME  = 0x3,
    VDP_GP_ORDER_ZME_CSC  = 0x4,

    VDP_GP_ORDER_BUTT
} VDP_GP_ORDER_E;


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


//-------------------------------------------- 
//add wbc connect to vidbzme,vidazme,vpacsc
typedef enum
{
    VDP_LAYER_CONN_DHD0          = 0x0,
    VDP_LAYER_CONN_VPACSC        = 0x0,
    VDP_LAYER_CONN_VIDAZME       = 0x1,
    VDP_LAYER_CONN_VIDBZME       = 0x2,
    VDP_LAYER_CONN_BUTT           
} VDP_LAYER_CONN_E;
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

    VDP_WBC_VID_INFMT_BUTT
} VDP_WBC_VID_INFMT;

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


typedef struct tagVDP_DISP_SYNCINV_E 
{
    HI_U32  u32DatDinv;
    HI_U32  u32DatVinv;
    HI_U32  u32DatHinv;
    
    HI_U32  u32Hdfinv;
    HI_U32  u32Hdvinv; 
    HI_U32  u32Hdhinv;
    HI_U32  u32Hddinv;
    
    HI_U32  u32Vgavinv;
    HI_U32  u32Vgahinv;
    HI_U32  u32Vgadinv;
    
    HI_U32  u32Lcdvinv;
    HI_U32  u32Lcdhinv;
    HI_U32  u32Lcddinv;

} VDP_DISP_SYNCINV_E;

typedef struct
{
    HI_U32 u32X;
    HI_U32 u32Y;

    HI_U32 u32Wth;
    HI_U32 u32Hgt;
    
} VDP_RECT_S;


typedef struct
{
    HI_U32 acc_multi    ;
    HI_U32 thd_med_high ;
    HI_U32 thd_med_low  ;
    HI_U32 thd_high     ;
    HI_U32 thd_low      ;
} ACCTHD_S;




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

typedef enum tagVDP_VID_PARA_E
{
    VDP_VID_PARA_ZME_HOR = 0x0,
    VDP_VID_PARA_ZME_VER = 0x1,
	VDP_VID_PARA_ZME_VERL = 0x2,
    VDP_VID_PARA_ZME_VERC = 0x3,
    VDP_VID_PARA_ZME_HORL = 0x4,
    VDP_VID_PARA_ZME_HORC = 0x5,
    VDP_VID_PARA_SR_ZME_HOR  = 0x6,
    VDP_VID_PARA_SR_ZME_VER  = 0x7,
    VDP_VID_PARA_SR_ZME_HORL = 0x8,
    VDP_VID_PARA_SR_ZME_VERL = 0x9,
    VDP_VID_PARA_SR_ZME_HORC = 0x10,
    VDP_VID_PARA_SR_ZME_VERC = 0x11,

    VDP_VID_PARA_BUTT
} VDP_VID_PARA_E;

typedef struct 
{
    HI_S32 cvfir_coef00;
    HI_S32 cvfir_coef01;
    HI_S32 cvfir_coef02;
    HI_S32 cvfir_coef03;

    HI_S32 cvfir_coef10;
    HI_S32 cvfir_coef11;
    HI_S32 cvfir_coef12;
    HI_S32 cvfir_coef13;

} VDP_VID_ZME_CVFIR_COEF_S;

typedef enum tagVDP_GP_PARA_E
{
    VDP_GP_PARA_ZME_HOR = 0,
    VDP_GP_PARA_ZME_VER   ,
    
    VDP_GP_PARA_ZME_HORL  ,
    VDP_GP_PARA_ZME_HORC  ,
    VDP_GP_PARA_ZME_VERL  ,
    VDP_GP_PARA_ZME_VERC  ,

    VDP_GP_GTI_PARA_ZME_HORL  ,
    VDP_GP_GTI_PARA_ZME_HORC  ,
    VDP_GP_GTI_PARA_ZME_VERL  ,
    VDP_GP_GTI_PARA_ZME_VERC  ,
    
    VDP_GP_PARA_BUTT
} VDP_GP_PARA_E;

typedef enum tagVDP_WBC_PARA_E
{
    VDP_WBC_PARA_ZME_HOR = 0,
    VDP_WBC_PARA_ZME_VER   ,
    
    VDP_WBC_PARA_ZME_HORL  ,
    VDP_WBC_PARA_ZME_HORC  ,
    VDP_WBC_PARA_ZME_VERL  ,
    VDP_WBC_PARA_ZME_VERC  ,

    VDP_WBC_GTI_PARA_ZME_HORL  ,
    VDP_WBC_GTI_PARA_ZME_HORC  ,
    VDP_WBC_GTI_PARA_ZME_VERL  ,
    VDP_WBC_GTI_PARA_ZME_VERC  ,
    
    VDP_WBC_PARA_BUTT
} VDP_WBC_PARA_E;

typedef enum tagVDP_DISP_PARA_E
{
    VDP_DISP_PARA_GMM = 0x0,

    VDP_DISP_PARA_BUTT
} VDP_DISP_PARA_E;
//---------------------------------------
// Modules
//---------------------------------------

// bkg color patern fill
typedef struct tagVDP_BKG_S
{
    HI_U32 u32BkgY;
    HI_U32 u32BkgU;
    HI_U32 u32BkgV;

    HI_U32 u32BkgA;
    
    HI_U32 bBkType;

} VDP_BKG_S;

//SR
typedef enum tagVDP_SR_COEF_MODE_E
{
    VDP_SR_COEF_TYP  = 1,
    VDP_SR_COEF_MAX = 1,
    VDP_SR_COEF_MIN = 1,
    VDP_SR_COEF_RAND = 1,
   
    VDP_SR_COEF_BUTT
} VDP_SR_COEF_MODE_E;

typedef struct 
{
    HI_S32 sr_hlcoef00;
    HI_S32 sr_hlcoef01;
    HI_S32 sr_hlcoef02;

    HI_S32 sr_hlcoef03;
    HI_S32 sr_hlcoef04;
    HI_S32 sr_hlcoef05;

    HI_S32 sr_hlcoef06;
    HI_S32 sr_hlcoef07;
    HI_S32 sr_hlcoef10;

    HI_S32 sr_hlcoef11;
    HI_S32 sr_hlcoef12;
    HI_S32 sr_hlcoef13;

    HI_S32 sr_hlcoef14;
    HI_S32 sr_hlcoef15;
    HI_S32 sr_hlcoef16;

    HI_S32 sr_hlcoef17;
    HI_S32 sr_hccoef00;
    HI_S32 sr_hccoef01;

    HI_S32 sr_hccoef02;
    HI_S32 sr_hccoef03;
    HI_S32 sr_hccoef04;

    HI_S32 sr_hccoef05;
    HI_S32 sr_hccoef06;
    HI_S32 sr_hccoef07;

    HI_S32 sr_hccoef10;
    HI_S32 sr_hccoef11;
    HI_S32 sr_hccoef12;

    HI_S32 sr_hccoef13;
    HI_S32 sr_hccoef14;
    HI_S32 sr_hccoef15;

    HI_S32 sr_hccoef16;
    HI_S32 sr_hccoef17;

} VDP_SR_HORCOEF_S;

typedef struct 
{
    HI_S32 sr_vlcoef00;
    HI_S32 sr_vlcoef01;
    HI_S32 sr_vlcoef02;

    HI_S32 sr_vlcoef03;
    HI_S32 sr_vlcoef04;
    HI_S32 sr_vlcoef05;

    HI_S32 sr_vlcoef10;
    HI_S32 sr_vlcoef11;
    HI_S32 sr_vlcoef12;

    HI_S32 sr_vlcoef13;
    HI_S32 sr_vlcoef14;
    HI_S32 sr_vlcoef15;

    HI_S32 sr_vlcoef16;
    HI_S32 sr_vlcoef17;

    HI_S32 sr_vccoef00;
    HI_S32 sr_vccoef01;
    HI_S32 sr_vccoef02;

    HI_S32 sr_vccoef03;
    HI_S32 sr_vccoef04;
    HI_S32 sr_vccoef05;

    HI_S32 sr_vccoef10;
    HI_S32 sr_vccoef11;
    HI_S32 sr_vccoef12;

    HI_S32 sr_vccoef13;
    HI_S32 sr_vccoef14;
    HI_S32 sr_vccoef15;

}VDP_SR_VERCOEF_S;

//-------------------
// CSC
//-------------------
typedef enum tagVDP_CSC_MODE_E
{
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

//-------------------
// CSC
//-------------------
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
	// the compensation is separated into 8 subsections, (Gains&Thresholds)
	
    //HI_S32 s32CbCrGain1_0,s32CbCrGain1_1,s32CbCrGain1_2,s32CbCrGain1_3,s32CbCrGain1_4,s32CbCrGain1_5,s32CbCrGain1_6,s32CbCrGain1_7,s32CbCrGain1_8;
	//HI_S32 s32CbCrGain1_Y_THR1,s32CbCrGain1_Y_THR2,s32CbCrGain1_Y_THR3,s32CbCrGain1_Y_THR4,s32CbCrGain1_Y_THR5,s32CbCrGain1_Y_THR6,s32CbCrGain1_Y_THR7,s32CbCrGain1_Y_THR8;
	//HI_S32 s32CbCrGain2_0,s32CbCrGain2_1,s32CbCrGain2_2,s32CbCrGain2_3,s32CbCrGain2_4,s32CbCrGain2_5,s32CbCrGain2_6,s32CbCrGain2_7,s32CbCrGain2_8;
	//HI_S32 s32CbCrGain2_Y_THR1,s32CbCrGain2_Y_THR2,s32CbCrGain2_Y_THR3,s32CbCrGain2_Y_THR4,s32CbCrGain2_Y_THR5,s32CbCrGain2_Y_THR6,s32CbCrGain2_Y_THR7,s32CbCrGain2_Y_THR8;
    
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

//-------------------
// ACM
//-------------------
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

//for rm
typedef struct 
{
	HI_U32  u32IWth;
	HI_U32  u32IHgt;

	HI_U32  u32OWth;
	HI_U32  u32OHgt;

	HI_S32 s32StartFrm;
	HI_S32 s32EndFrm;

	HI_U32  u32BitDepthIn; /* data bit depth of the input YUV file :8/10 bit*/
	HI_U32  u32BitDepthOut;/*data bit depth of the output YUV file :8/10 bit*/
	HI_U32  u32BitDepthPrc;/*data bit depth of processing*/

	HI_S32 s32En2DSR;
	HI_S32 s32EnSharp;
	HI_S32 s32SharpGain;
	HI_S32 s32GradSubRatio;
	HI_S32 s32GradAddCtrl;
	HI_S32 s32GrdThrL;
	HI_S32 s32GrdThrH;
	HI_S32 s32GrdThrHsubL;

	HI_S32 s32UnderThrsh;
	HI_S32 s32OverThrsh;
	HI_S32 s32SharpCoring;

	HI_S32 s32DISCdetailGain;
	HI_S32 s32EnhSCdetailGain;
	HI_S32 s32EnEnhshnodir;
	HI_S32 s32EnPlusCtrl;

	HI_S32 s32EnHorzSmooth;
	HI_S32 s32PlusCtrl;

	HI_S32 s32UpsclRatioH;
	HI_S32 s32UpsclRatioV;

    HI_U32 u32SrDispStat;
    HI_U32 u32SrDispMode;
    HI_U32 u32DispMode;
    HI_U32 u32ZmeInFmt;

} VDP_SR_CFG_S;

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

typedef enum tagVDP_IFIRMODE_E
{
    VDP_IFIRMODE_DISEN = 0,
    VDP_IFIRMODE_COPY,
    VDP_IFIRMODE_DOUBLE,
    VDP_IFIRMODE_6TAPFIR,

    VDP_IFIRMODE_BUTT
}VDP_IFIRMODE_E;

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

typedef enum tagVDP_WBC_3D_MODE_E
{
    VDP_WBC_3D_MODE_TWOEYE = 0,
    VDP_WBC_3D_MODE_LEFTEYE = 2,
    VDP_WBC_3D_MODE_RIGHTEYE = 3,

    VDP_WBC_3D_MODE_BUTT
}VDP_WBC_3D_MODE_E;

typedef enum tagVDP_VP_PARA_E
{
    VDP_VP_PARA_ACM     = 0x1,//for coef read and para up
    VDP_VP_PARA_DCI_HBW = 0x2,//for coef read
    VDP_VP_PARA_DCI_DIV = 0x4,//for coef read
    VDP_VP_PARA_DCI_LUT = 0x8,//for coef read
    VDP_VP_PARA_BUTT
} VDP_VP_PARA_E;
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
    VDP_CMP_COMPONENT_YUV  = 4,
    VDP_CMP_COMPONENT_A    = 5,
    VDP_CMP_COMPONENT_R    = 6,
    VDP_CMP_COMPONENT_G    = 7,
    VDP_CMP_COMPONENT_B    = 8,

    VDP_CMP_COMPONENT_BUTT
}VDP_CMP_COMPONENT_E;


//-------------------
// vou graphic layer data extend mode
//-------------------
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



typedef enum tagVDP_CBM_MIX_E
{
    VDP_CBM_MIXV0 = 0,
    VDP_CBM_MIXV1 = 1,
    VDP_CBM_MIXG0 = 2,
    VDP_CBM_MIXG1 = 3,
    VDP_CBM_MIX0  = 4,
    VDP_CBM_MIX1  = 5,

    VDP_CBM_MIX_BUTT 
}VDP_CBM_MIX_E;

typedef enum tagSDATE_SYSMD_E
{
    SDATE_SYSMD_525 = 0,
    SDATE_SYSMD_625,
    
    SDATE_SYSMD_NULL
}SDATE_SYSMD_E;

typedef enum tagSDATE_MDUMD_t
{
    SDATE_MDUMD_525      = 0x0,
    SDATE_MDUMD_M_NTSC    = 0x0001,
    SDATE_MDUMD_NTSC_J    = 0x0010,
    SDATE_MDUMD_M_PAL     = 0x0100,
    SDATE_MDUMD_PAL_60     = 0x1000,
    SDATE_MDUMD_NTSC_443     = 0x1001,    

    SDATE_MDUMD_625      = 0x10000,
    SDATE_MDUMD_BDGHI_PAL = 0x10001,
    SDATE_MDUMD_N_PAL     = 0x10010,
    SDATE_MDUMD_NC_PAL    = 0x10100,
    SDATE_MDUMD_SECAM    = 0x10100,

    SDATE_MDUMD_NULL
}SDATE_MDUMD_t;

typedef enum tagSDATE_SYNCMODE_E
{
    SDATE_SYNCMOD_ONE = 0,
    SDATE_SYNCMOD_THREE,
    
    SDATE_SYNCMOD_NULL

}SDATE_SYNCMODE_E;

typedef enum tagSDATE_VANBASE_E
{
    SDATE_VANBASE_ON = 0,
    SDATE_VANBASE_OF,
    
    SDATE_VANBASE_NULL
}SDATE_VANBASE_E;

typedef enum tagSDATE_LINELEN_E
{
    SDATE_LINELEN_BT601 = 0,
    SDATE_LINELEN_BT470, 

    SDATE_LINELEN_NULL 
}SDATE_LINELEN_E;

typedef enum tagSDATE_AGCAMP_E
{
    SDATE_AGCAMP_ISIDE = 0,
    SDATE_AGCAMP_OSIDE,
    
    SDATE_AGCAMP_NULL
}SDATE_AGCAMP_E;

typedef enum tagSDATE_TTLINE_E
{
    SDATE_TT_OL_07 = 0x00010000,
    SDATE_TT_OL_08 = 0x00020000,
    SDATE_TT_OL_09 = 0x00040000,
    SDATE_TT_OL_10 = 0x00080000,
                               
    SDATE_TT_OL_11 = 0x00100000,
    SDATE_TT_OL_12 = 0x00200000,
    SDATE_TT_OL_13 = 0x00400000,
    SDATE_TT_OL_14 = 0x00800000,
                               
    SDATE_TT_OL_15 = 0x01000000,
    SDATE_TT_OL_16 = 0x02000000,
    SDATE_TT_OL_17 = 0x04000000,
    SDATE_TT_OL_18 = 0x08000000,
                               
    SDATE_TT_OL_19 = 0x10000000,
    SDATE_TT_OL_20 = 0x20000000,
    SDATE_TT_OL_21 = 0x40000000,
    SDATE_TT_OL_22 = 0x80000000,
                               
    SDATE_TT_EL_07 = 0x00000001,
    SDATE_TT_EL_08 = 0x00000002,
    SDATE_TT_EL_09 = 0x00000004,
    SDATE_TT_EL_10 = 0x00000008,
                               
    SDATE_TT_EL_11 = 0x00000010,
    SDATE_TT_EL_12 = 0x00000020,
    SDATE_TT_EL_13 = 0x00000040,
    SDATE_TT_EL_14 = 0x00000080,
                               
    SDATE_TT_EL_15 = 0x00000100,
    SDATE_TT_EL_16 = 0x00000200,
    SDATE_TT_EL_17 = 0x00000400,
    SDATE_TT_EL_18 = 0x00000800,
                               
    SDATE_TT_EL_19 = 0x00001000,
    SDATE_TT_EL_20 = 0x00002000,
    SDATE_TT_EL_21 = 0x00004000,
    SDATE_TT_EL_22 = 0x00008000,
	
    
    SDATE_TTLINE_ALL,
    SDATE_TT_ALL,
    SDATE_TT_NULL  
}SDATE_TTLINE_E;


typedef enum tagSDATE_FIELD_E
{
    SDATE_ODD_FIELD = 0,
    SDATE_EVEN_FIELD,
}SDATE_FIELD_E;

typedef enum tagSDATE_MV_E
{
    SDATE_MV_N0 = 0,
    SDATE_MV_N1 , 
    SDATE_MV_N2 ,
    SDATE_MV_N3 ,
    SDATE_MV_N4 ,
    SDATE_MV_N5 ,
    SDATE_MV_N6 ,
    SDATE_MV_N7 ,
    SDATE_MV_N8 ,
    SDATE_MV_N9 ,
    SDATE_MV_N10,
    SDATE_MV_N11,
    SDATE_MV_N12,
    SDATE_MV_N13,
    SDATE_MV_N14,
    SDATE_MV_N15,
    SDATE_MV_N16,
    SDATE_MV_N17,
    SDATE_MV_N18,
    SDATE_MV_N19,
    SDATE_MV_N20,
    SDATE_MV_N21,
    SDATE_MV_N22,

    SDATE_MV_NULL
}SDATE_MV_E;

typedef enum tagSDATE_VPS_DATA_E
{
    SDATE_VPS_DATA_0 = 0,
    SDATE_VPS_DATA_1,
    SDATE_VPS_DATA_2,
    SDATE_VPS_DATA_3,

    SDATE_VPS_DATA_NULL    
}SDATE_VPS_DATA_E;

typedef enum tagSDATE_LUMNT_E
{
    SDATE_LUMNT_1 = 0,
    SDATE_LUMNT_2,
    SDATE_LUMNT_3,
    SDATE_LUMNT_4,
    SDATE_LUMNT_5,
    SDATE_LUMNT_6,
    SDATE_LUMNT_7,
    SDATE_LUMNT_8,
    SDATE_LUMNT_9,
    SDATE_LUMNT_10,
    SDATE_LUMNT_11,
    SDATE_LUMNT_12,
    SDATE_LUMNT_13,
    SDATE_LUMNT_14,
    SDATE_LUMNT_15,
    SDATE_LUMNT_16,
    SDATE_LUMNT_17,
    SDATE_LUMNT_18,

    SDATE_LUMNT_NULL
}SDATE_LUMNT_E;

typedef enum tagSDATE_LUMOS_E
{
    SDATE_LUMOS_TAP1 = 0,
    SDATE_LUMOS_TAP2,		

    SDATE_LUMOS_NULL    
}SDATE_LUMOS_E;

typedef enum tagSDATE_OUTMODE_E
{
    SDATE_OUTMODE_COMP_Y = 0,
    SDATE_OUTMODE_COMP_PB,
    SDATE_OUTMODE_COMP_PR,    
    
    SDATE_OUTMODE_SVID_Y,
    SDATE_OUTMODE_SVID_C, 

    SDATE_OUTMODE_CVBS,

    SDATE_OUTMODE_NULL
}SDATE_OUTMODE_E;

typedef enum tagSDATE_DACMODE_E
{
    SDATE_DACMODE_DAC0 = 0,
    SDATE_DACMODE_DAC1 = 1,
    SDATE_DACMODE_DAC2 = 2,
    SDATE_DACMODE_DAC3 = 3,
    SDATE_DACMODE_DAC4 = 4,
    SDATE_DACMODE_DAC5 = 5,
    
    SDATE_DACMODE_NULL
    
}SDATE_DACMODE_E;

typedef enum tagHDATE_SYNCMODE_E
{
    HDATE_SYNCMODE_HSYNC_IN,
    HDATE_SYNCMODE_HSYNC_OUT,
    HDATE_SYNCMODE_VSYNC_IN,
    HDATE_SYNCMODE_VSYNC_OUT,
    HDATE_SYNCMODE_FID_IN ,
    HDATE_SYNCMODE_FID_OUT,

    HDATE_SYNCMODE_NULL	
}HDATE_SYNCMODE_E;

typedef enum tagHDATE_SYNCCHN_E
{
    HDATE_SYNCCHN_B = 1,
    HDATE_SYNCCHN_G = 2,    
    HDATE_SYNCCHN_R = 4,

    HDATE_SYNCCHN_RGB     = 0x10,
    HDATE_SYNCCHN_YPBPR = 0x11,
    HDATE_SYNCCHN_VGA     = 0x12,

    HDATE_SYNCCHN_NULL
    
}HDATE_SYNCCHN_E;

typedef enum tagHDATE_VIDFMT_E
{
    HDATE_VIDFMT_480P_SMPTE293M = 0,
    HDATE_VIDFMT_480P_BT1358 = 0,		
    
    HDATE_VIDFMT_576P_BT1358 = 1,		
    
    HDATE_VIDFMT_720P_EIA770 = 2,
    HDATE_VIDFMT_720P_SMPTE296M = 2,

    HDATE_VIDFMT_1080P_SMPTE274M = 3,
    HDATE_VIDFMT_1080P_EIA770 = 3,
    HDATE_VIDFMT_1080P_SMPTE295M = 5,

    HDATE_VIDFMT_1080I_SMPTE274M = 4,
    HDATE_VIDFMT_1080I_EIA770 = 4,
    HDATE_VIDFMT_1080I_SMPTE295M = 6,
    HDATE_VIDFMT_1080I_AS4933 = 8,

    HDATE_VIDFMT_1152I_AS4933 = 7,
    
    HDATE_VIDFMT_NULL
    
}HDATE_VIDFMT_E;

typedef enum tagHDATE_CSCMODE_E
{
    HDATE_CSCMODE_YCBCR_YPBPR = 0,
    HDATE_CSCMODE_YCBCR_RGB709 = 1,
    HDATE_CSCMODE_YCBCR_RGB601 = 2,    
    HDATE_CSCMODE_YCBCR_RGB240 = 3,

    HDATE_CSCMODE_CUSTOM = 4,
	
    HDATE_CSCMODE_NULL
    
}HDATE_CSCMODE_E;

typedef enum tagHDATE_OUTCHN_E
{
    HDATE_OUTCHN_VID1 = 0,
    HDATE_OUTCHN_VID2,
    HDATE_OUTCHN_VID3,
    HDATE_OUTCHN_VSYNC,
    HDATE_OUTCHN_HSYNC,
    HDATE_OUTCHN_FID,

    HDATE_OUTCHN_NULL
    
}HDATE_OUTCHN_E;


typedef enum tagHDATE_SRCMODE_E
{
    HDATE_SRCMODE_SD = 0,
    HDATE_SRCMODE_HD,

    HDATE_SRCMODE_NULL
    
}HDATE_SRCMODE_E;

typedef enum tagHDATE_OSMODE_E
{
    HDATE_OSMODE_1X_CPY = 0,
    HDATE_OSMODE_1X_FLT,
    HDATE_OSMODE_2X_FLT,
    HDATE_OSMODE_4X_FLT,    

    HDATE_OSMODE_NULL,
    
}HDATE_OSMODE_E;

typedef enum tagHDATE_CGMS_DATA_E
{
    HDATE_CGMSA_DATA = 0,
    HDATE_CGMSB_HEAD,
    HDATE_CGMSB_DATA1,
    HDATE_CGMSB_DATA2,
    HDATE_CGMSB_DATA3,
    HDATE_CGMSB_DATA4,

    HDATE_CGMS_TYPEA = 0,
    HDATE_CGMS_TYPEB ,
    
    HDATE_CGMS_DATA_NULL,
    
}HDATE_CGMS_DATA_E;
typedef enum tagVDP_CBM_LAYER_E
{
    VDP_CBM_VP0 = 0,
    VDP_CBM_GP0 = 1,
    VDP_CBM_VP1 = 2,
    VDP_CBM_VID3 = 2,    
    VDP_CBM_GP1 = 3,
    VDP_CBM_VID4 = 4,    
    VDP_CBM_GFX3= 5,
    
    VDP_CBM_BUTT 
}VDP_CBM_LAYER_E;

typedef enum 
{
    VDP_DISP_COEFMODE_HOR  = 0,  
    VDP_DISP_COEFMODE_VER,
    VDP_DISP_COEFMODE_LUT,
    VDP_DISP_COEFMODE_GAM,
    VDP_DISP_COEFMODE_ACC,
    VDP_DISP_COEFMODE_DCI,
    VDP_DISP_COEFMODE_ABC,
    VDP_DISP_COEFMODE_ACM,
    VDP_DISP_COEFMODE_NR,
    VDP_DISP_COEFMODE_SHARP,
    VDP_DISP_COEFMODE_DIM,
    VDP_DISP_COEFMODE_DIMZMEH,
    VDP_DISP_COEFMODE_DIMZMEV,

    VDP_DISP_COEFMODE_ALL
}VDP_DISP_COEFMODE_E;

typedef enum tagRM_FFINFO_E
{
    RM_FFINFO_TOP,
    RM_FFINFO_BTM,
    RM_FFINFO_FRM,
    RM_FFINFO_BT1,

    RM_FFINFO_BUTT
}RM_FFINFO_E;


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


typedef struct tagRequest
{
    HI_S32  id;
    HI_VOID *buf;
    HI_U32  addr;
    HI_S32  size;
    HI_S32  count;
    HI_S32  mode;
    HI_U32    wr;  /// 0: read; 1: write
} Request;


typedef enum tagVDP_DISP_DIGFMT_E
{
    VDP_DISP_DIGFMT_PAL    = 0  , 
    VDP_DISP_DIGFMT_NTSC,
    VDP_DISP_DIGFMT_720P,
    VDP_DISP_DIGFMT_1080P, 
    VDP_DISP_DIGFMT_1080I,
    VDP_DISP_DIGFMT_1080P50, 
    VDP_DISP_DIGFMT_1080P60 , 
    VDP_DISP_DIGFMT_1080I50, 
    VDP_DISP_DIGFMT_1080I60, 
    VDP_DISP_DIGFMT_720P50 , 
    VDP_DISP_DIGFMT_720P60,
    VDP_DISP_DIGFMT_720P_60, 
    VDP_DISP_DIGFMT_800x600, 
    VDP_DISP_DIGFMT_1920x1200,
    VDP_DISP_DIGFMT_576P, 
    VDP_DISP_DIGFMT_576I, 
    VDP_DISP_DIGFMT_480P, 
    VDP_DISP_DIGFMT_480I, 
    VDP_DISP_DIGFMT_2160P30,//3840x2160
    VDP_DISP_DIGFMT_2560P60, 
    VDP_DISP_DIGFMT_TESTI, 
    VDP_DISP_DIGFMT_TESTP, 
    VDP_DISP_DIGFMT_TESTS, 
    VDP_DISP_DIGFMT_TESTUT, 
    VDP_DISP_DIGFMT_1080P30,
    VDP_DISP_DIGFMT_1280x1024,
    VDP_DISP_DIGFMT_1024x768 ,
    VDP_DISP_DIGFMT_1080P_24_FP,    
    VDP_DISP_DIGFMT_1080I_50_FP,
    VDP_DISP_DIGFMT_2560P,
    VDP_DISP_DIGFMT_640x480_72 ,
    VDP_DISP_DIGFMT_1366x768,
    VDP_DISP_DIGFMT_BUTT
} VDP_DISP_DIGFMT_E;

//-----------------------------------
//define of EDA
//-----------------------------------
#if 0  //old define
typedef enum tagVDP_RM_DUMP_E
{
    VDP_RM_DUMP_ARGB = 0,
    VDP_RM_DUMP_YUV     ,
    VDP_RM_DUMP_Y       ,
    VDP_RM_DUMP_C       ,
    VDP_RM_DUMP_BUTT     
}VDP_RM_DUMP_E;
#endif

typedef enum tagVDP_RM_DUMP_E
{
    VDP_RM_DUMP_ARGB = 0,
    VDP_RM_DUMP_YUV     ,
    VDP_RM_DUMP_YUV_L     ,// Y0U0V0 Y1U1V1 Y2U2V2 ...
    VDP_RM_DUMP_YUV_R     ,// Y0U0V0 Y1U1V1 Y2U2V2 ...
    VDP_RM_DUMP_Y       ,
    VDP_RM_DUMP_C       ,
    VDP_RM_DUMP_YY      ,
    VDP_RM_DUMP_UV      ,//2 pixel per cycle, U0V0 U1V1 ...
    VDP_RM_DUMP_YY_L      ,//2 pixel per cycle, Y0Y1 Y2Y3 ...
    VDP_RM_DUMP_YY_R      ,//2 pixel per cycle, Y0Y1 Y2Y3 ...
    VDP_RM_DUMP_UV_L      ,//2 pixel per cycle, U0V0 U1V1 ...
    VDP_RM_DUMP_UV_R      ,//2 pixel per cycle, U0V0 U1V1 ...
    VDP_RM_DUMP_BUTT     
}VDP_RM_DUMP_E;
typedef enum tagVDP_RM_LAYER_E
{
    VDP_RM_LAYER_VID0 = 0,
    VDP_RM_LAYER_VID1,
    VDP_RM_LAYER_VID2, 
    VDP_RM_LAYER_VID3, 
    VDP_RM_LAYER_VID4, 
      VDP_RM_LAYER_VP0,
    VDP_RM_LAYER_GFX0,   
    VDP_RM_LAYER_GFX1,   
    VDP_RM_LAYER_GFX2,   
    VDP_RM_LAYER_GFX3,   
    VDP_RM_LAYER_GFX4,   
    VDP_RM_LAYER_WBC_HD0,   
    VDP_RM_LAYER_WBC_GP0,   
    VDP_RM_LAYER_GP0,   
    VDP_RM_LAYER_GP1,   
    VDP_RM_LAYER_DHD0,   
    VDP_RM_LAYER_TT,   

} VDP_RM_LAYER_E;

typedef struct tagVDP_BFM_INFO_S
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

    HI_U32 u32CbarEn;
    HI_U32 u32IntfFmt;
    HI_U32 u32BfmMode;
    HI_U32 u32BitWth;

    HI_U32  bFp;

}VDP_BFM_INFO_S;


typedef struct tagVDP_BFM_OUTDATA_S
{
    HI_U16 *Y;
    HI_U16 *Cb;
    HI_U16 *Cr;
    
    HI_U32 Length;

    HI_U8  Field;
    HI_U8  Dataformat;
    HI_U8  Datafmt;
}VDP_BFM_OUTDATA_S;

typedef struct tagVDP_WBCK_INFO_S
{
    VDP_LAYER_WBC_E  enWbcId;
    RM_FFINFO_E      enFfInfo; 
}VDP_WBCK_INFO_S;

typedef enum
{
    ID_DRV_MODULE,
    ID_CK_MODULE,
    ID_BFM_MODULE,
    ID_WBC_MODULE,
    ID_DAC_MODULE
}MODULE_ID;

typedef struct
{
    //VDP_RM_COEF_MODE_E enCoefMd;

    HI_U32 u32CusMd;

    HI_U32 u32HorTap;
    HI_U32 u32VerTap;
    
    HI_U32 u32HorPha;
    HI_U32 u32VerPha;

    HI_U32 u32HorAddr;
    HI_U32 u32VerAddr;

    HI_S32 * sp32HorCoef;
    HI_S32 * sp32VerCoef;
    
} VDP_STI_ZMECOEF_S;

typedef enum tagVDP_RM_COEF_MODE_E
{
    VDP_RM_COEF_MODE_TYP = 0x0,
    VDP_RM_COEF_MODE_RAN = 0x1,
    VDP_RM_COEF_MODE_MIN = 0x2,
    VDP_RM_COEF_MODE_MAX = 0x3,
    VDP_RM_COEF_MODE_ZRO = 0x4,
    VDP_RM_COEF_MODE_CUS = 0x5,
    VDP_RM_COEF_MODE_BUTT
} VDP_RM_COEF_MODE_E;


typedef struct
{
    VDP_DATA_RMODE_E    enLumRdMode;
    VDP_DATA_RMODE_E    enChmRdMode;
    HI_U32              bFlipEn;
    HI_U32              u32DrawMode;

} VDP_FRW_INFO_S;

typedef enum tagVDP_DISP_INTF_E
{
    VDP_DISP_INTF_LCD = 0 , 
    VDP_DISP_INTF_BT1120  ,
    VDP_DISP_INTF_HDMI    ,
    VDP_DISP_INTF_VGA     ,
    VDP_DISP_INTF_HDDATE  ,
    VDP_DISP_INTF_SDDATE  ,
    VDP_DISP_INTF_DATE    ,
    VDP_DISP_INTF_CVBS    ,
    VDP_DISP_INTF_BUTT    ,
} VDP_DISP_INTF_E;

typedef enum tagVDP_DISP_INTF_OFMT_E
{
    VDP_DISP_INTF_OFMT_RGB888 = 0 , 
    VDP_DISP_INTF_OFMT_YUV422  ,
    VDP_DISP_INTF_OFMT_BUTT  ,
} VDP_DISP_INTF_OFMT_E;

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


typedef enum tagDISP_VENC_E
{
    DISP_VENC_HDATE0 = 0,

    DISP_VENC_SDATE0,
    DISP_VENC_VGA0,
    DISP_VENC_MAX
}DISP_VENC_E;

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
typedef enum tagVDP_CONNECT_E
{
    VDP_CONNECT_G3_DHD0 = 0, 
    VDP_CONNECT_G3_DHD1    ,

    VDP_CONNECT_BUTT       ,
} VDP_CONNECT_E;

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
    VDP_CHKSUM_YPBPR  = 6,
    
    VDP_CHKSUM_INTF_BUTT
} VDP_CHKSUM_INTF_E;

//---------------------------------
//SR BEGIN
//---------------------------------
typedef enum tagVDP_SR_MUX_E
{
    VDP_SR_AND_ZME_EN = 0x0,
    VDP_SR_AND_ZME_EN_L,
    VDP_SR_AND_ZME_EN_R,
    VDP_SR_ONLY_ZME_EN,
    VDP_SR_ZME_EN_BUTT
} VDP_SR_MUX_E;

typedef enum tagVDP_SR_DEMO_E
{
    VDP_SR_DEMO_EN = 0x0,
    VDP_SR_DEMO_EN_L,
    VDP_SR_DEMO_EN_R,
    VDP_SR_DEMO_EN_BUTT
} VDP_SR_DEMO_E;

typedef enum tagVDP_SR_DISP_STAT_E
{
    VDP_SR_DISP_ZME_SR = 0x0,
    VDP_SR_DISP_ZME,
    VDP_SR_DISP_DEMO_LEFT,
    VDP_SR_DISP_DEMO_RIGHT,
    VDP_SR_DISP_BUTT
} VDP_SR_DISP_STAT_E;

typedef enum tagVDP_SR_SHARP_MODE_E
{
    VDP_SR_SHARP_MODE_DETAIL = 0x0,
    VDP_SR_SHARP_MODE_TRAD,
    VDP_SR_SHARP_MODE_BUTT
} VDP_SR_SHARP_MODE_E;

typedef enum tagVDP_SR_E
{
    VDP_SR_UENABLE = 0x0,
    VDP_SR_ENABLE,
    VDP_SR_BUTT
} VDP_SR_E;

typedef enum tagVDP_SR_SHARP_E
{
    VDP_SR_SHARP_UENABLE = 0x0,
    //VDP_SR_SHARP_R_ENABLE,
    //VDP_SR_SHARP_L_ENABLE,
    VDP_SR_SHARP_ENABLE,
    VDP_SR_SHARP_BUTT
} VDP_SR_SHARP_E;

typedef enum tagVDP_SR_SMOOTH_E
{
    VDP_SR_SMOOTH_UENABLE = 0x0,
    VDP_SR_SMOOTH_ENABLE, 
    VDP_SR_SMOOTH_BUTT
} VDP_SR_SMOOTH_E;

typedef enum tagVDP_SR_PLUS_E
{
    VDP_SR_PLUS_UENABLE = 0x0,
    VDP_SR_PLUS_ENABLE,
    VDP_SR_PLUS_BUTT
} VDP_SR_PLUS_E;

typedef struct tagVDP_SR_PARA_INFO
{
    HI_U32  u32IWth; 
    HI_U32  u32IHgt; 

    HI_U32  u32OWth; 
    HI_U32  u32OHgt; 

    HI_U32  sr_en; 
    HI_U32  zme_en; 
    HI_U32  smooth_en; 
    HI_U32  plus_en; 
    HI_U32  mid_en; 
    HI_U32  DispMode; 

    VDP_SR_DISP_STAT_E  disp_state; 

    VDP_PROC_FMT_E  ifmt; 
    VDP_SR_SHARP_E  sharp_en; 

    //VDP_SR_DISP_STAT_E  sr_disp_stat; 
    VDP_SR_SHARP_MODE_E hsh_nodir; 

}VDP_SR_PARA_INFO;

typedef struct tagVDP_SR_IORESO
{
    HI_U32 iw;
    HI_U32 ih;
    HI_U32 ow;
    HI_U32 oh;
}VDP_SR_IORESO;

typedef struct tagVDP_SR_PARA_S
{
    VDP_SR_PARA_INFO  SrParaInfo;

    HI_U32 hfir_order; 
    HI_U32 hratio; 
    HI_U32 vratio; 
    HI_U32 grd_sub_ratio; 
    HI_S32 grd_thrl; 
    HI_S32 grd_thrh; 
    HI_U32 grd_thrh_subl; 
    HI_U32 grd_add_ctrl; 
    HI_U32 disc_detail_gain; 
    HI_U32 ensc_detail_gain; 
    HI_U32 sharp_coring; 
    HI_U32 sharp_gain; 
    HI_U32 over_thrsh; 
    HI_U32 under_thrsh; 
    HI_U32 plus_ctrl; 
    HI_U32 offset; 
    HI_U32 fir_en; 
    HI_U32 zme_en; 
    HI_U32 srzme_hladdr; 
    HI_U32 srzme_hcaddr; 
    HI_U32 srzme_vladdr; 
    HI_U32 srzme_vcaddr; 

}VDP_SR_PARA_S;


//---------------------------------
//SR END
//---------------------------------
//---------------------------------
//Sharpen BEGIN
//---------------------------------
//sharpen
typedef struct tagVDP_SHARP_HIGH_FREQ_S
{
    HI_S16   s16Tmp5;
    HI_S16   s16Tmp4;
    HI_S16   s16Tmp3;
    HI_S16   s16Tmp2;
    HI_S16   s16Tmp1;
    HI_S16   s16Tmp0;
    HI_S16   s16GainPos;/*(s,10,8):Max 3.0 */
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
    HI_U8    u8bGain;
    HI_U16   u16Grd0;/*10bit*/
    HI_U16   u16Grd1;
    HI_U16   u16Grd2;
    HI_U16   u16Grd3;
    /***limit***/    
    HI_U16    u16LimitRatio;  
    HI_U16    u16LimitPos0 ;   
    HI_U16    u16LimitPos1 ;   
    HI_S16    s16LimitNeg0 ;   
    HI_S16    s16LimitNeg1 ;   
    HI_U16    u16BoundPos  ;    
    HI_S16    s16BoundNeg  ;

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
    VDP_SHARP_HIGH_FREQ_S stSharpU5;//unit 5 add S5V200L
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

typedef enum tagVDP_VID_SHARPEN_MODE_E
{
    VDP_VID_SHARPEN_TYP      = 0x0,
    VDP_VID_SHARPEN_MAX      = 0x1,
    VDP_VID_SHARPEN_MIN      = 0x2,
    VDP_VID_SHARPEN_RAND     = 0x3,
    VDP_VID_SHARPEN_ZERO     = 0x4,
    VDP_VID_SHARPEN_ONE      = 0x5,

    VDP_VID_SHARPEN_CFG_BUTT        
    
}VDP_VID_SHARPEN_MODE_E;
//---------------------------------
//sharpen END
//---------------------------------

//add by h226871
typedef struct hiVDP_LAYER_RESO_S 
{
	VDP_RECT_S				stVdp_rect_in; 
	VDP_RECT_S				stVdp_rect_out; 
	VDP_RECT_S				stVdp_rect_vid; 
	VDP_RECT_S				stVdp_rect_disp; 
    VDP_RECT_S 				stPosMinReso;
    HI_U32   				u32AlignWH;
    HI_U32     				u32AlignXY;
    HI_U32   				u32AlignW;
    HI_U32   				u32AlignH;
    HI_U32     				u32AlignX;
    HI_U32     				u32AlignY;

}VDP_LAYER_RESO_S;

typedef enum tagVDP_POS_MODE_E
{
    VDP_POS_MODE_FULL         = 0x0,  //video  = disp = mix
    VDP_POS_MODE_CENTER       = 0x1,  //video! = disp = mix mid

    VDP_POS_MODE_TOP          = 0x2,  //video! = disp = mix up
    VDP_POS_MODE_BOTTOM       = 0x3,  //video! = disp = mix down
    VDP_POS_MODE_LEFT         = 0x4,  //video! = disp = mix left
    VDP_POS_MODE_RIGHT        = 0x5,  //video! = disp = mix right

    VDP_POS_MODE_TOP_LEFT     = 0x6,  //video! = disp = mix up
    VDP_POS_MODE_BOTTOM_LEFT  = 0x7,  //video! = disp = mix down
    VDP_POS_MODE_TOP_RIGHT    = 0x8,  //video! = disp = mix left
    VDP_POS_MODE_BOTTOM_RIGHT = 0x9, //video! = disp = mix right
    
    VDP_POS_MODE_BUTT        
    
}VDP_POS_MODE_E;

//layer pos gen in 
typedef struct hiVDP_POS_GEN_CFG_S 
{
    VDP_RECT_S stReso;
    VDP_POS_MODE_E enPosMode;
    HI_U32 u32SclMode;
    HI_U32 u32AlignXY;
    HI_U32 u32AlignWH;
	
    HI_U32 u32AlignX;
    HI_U32 u32AlignY;
    HI_U32 u32AlignW;
    HI_U32 u32AlignH;

}VDP_POS_GEN_CFG_S;


typedef enum tagVDP_TEST_MODE_E
{
    VDP_TEST_MODE_NORM = 0,
    VDP_TEST_MODE_RAND = 1,
    VDP_TEST_MODE_COMB = 2,

    VDP_TEST_MODE_BUTT
}VDP_TEST_MODE_E;

//layer pos gen out
typedef struct hiVDP_LAYER_POS_S 
{
    VDP_RECT_S stInReso;
    VDP_RECT_S stPos;
    VDP_RECT_S stPosMinReso;
	
    HI_U32     u32AlignWH;
    HI_U32     u32AlignXY;
	
    HI_U32     u32AlignX;
    HI_U32     u32AlignY;
    HI_U32     u32AlignW;
    HI_U32     u32AlignH;

}VDP_LAYER_POS_S;

typedef enum tagVDP_SCALE_MODE_E
{
    NO_SCALE      = 0x0,  //video  = disp = mix
    UP_SCALE      = 0x1,  //video! = disp = mix mid
    DOWN_SCALE    = 0x2, //video! = disp = mix up
    
    VDP_SCALE_MODE_BUTT        
    
}VDP_SCALE_MODE_E;
#endif

