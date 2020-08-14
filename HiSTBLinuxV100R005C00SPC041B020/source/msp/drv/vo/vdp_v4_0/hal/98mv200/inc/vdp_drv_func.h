#ifndef __DRV_FUNC_H__
#define __DRV_FUNC_H__

#include "vdp_define.h"
#include "vdp_drv_ip_sharpen.h"

#if EDA_TEST
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
//#include "c_union_define.h"
#include "hi_reg_common.h"
#include "vmx.h"
#include "vdp_assert.h"
//for HDR composer
#include "rpu_ext_config.h"
#include "KdmTypeFxp.h"

#endif

#if FPGA_TEST
#include "vdp_fpga_define.h"
#endif

// #if VDP_C_COMPILE
// #include "vdp_reg.h"
// #endif

#define         MAX_WIN_NUM_PER_LAYER       16

#define CheckError(x) { if (x) { printf("\nErr @ (%s, %d)\n", __FILE__, __LINE__); \
                                                         exit(-__LINE__); } }

//add ACC ADDR
#define VDP_ACC_ADDR VDP_BASE_ADDR+0x6700 

//add by  h226871
typedef enum tagVDP_SR_SCALE_MODE_E
{
    VDP_SR_H1P1V1P1    = 0x0,
    VDP_SR_H1P1V1P2    ,
    VDP_SR_H1P2V1P1    ,
    VDP_SR_H1P2V1P2    ,

    VDP_VDP_SR__BUTT        
    
}VDP_SR_SCALE_MODE_E;


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
typedef enum tagVDP_REGIONMODE_E
{
    VDP_PIP_BOTTOM       = 0,
    VDP_PIP_TOP          = 1,
    VDP_PIP_LEFT         = 2,
    VDP_PIP_RIGHT        = 3,
    VDP_PIP_TOP_LEFT     = 4,
    VDP_PIP_TOP_RIGHT    = 5,
    VDP_PIP_BOTTOM_LEFT  = 6,
    VDP_PIP_BOTTOM_RIGHT = 7,
    VDP_POP_NORM         = 8,
    
    VDP_REGIONMODE_BUTT

} VDP_REGIONMODE_E;

typedef struct tagVDP_REGION_CFG_S
{
  VDP_REGIONMODE_E  u32VdpRegionMode;
  VDP_RECT_S        u32LargeVideoReso;
  VDP_RECT_S        u32SmallVideoReso;
  VDP_VID_IFMT_E    u32VideoFmt;
  HI_U32            u32LargeVideoLStr;
  HI_U32            u32LargeVideoCStr;
  HI_U32            u32LargeVideoLAddr;
  HI_U32            u32LargeVideoCAddr;
  HI_U32            u32SmallVideoLAddr;
  HI_U32            u32SmallVideoCAddr;
  HI_U32            u32SmallVideoLStr;
  HI_U32            u32SmallVideoCStr;

}VDP_REGION_CFG_S;




typedef enum tagVDP_SCALE_MODE_E
{
    NO_SCALE      = 0x0,  //video  = disp = mix
    UP_SCALE      = 0x1,  //video! = disp = mix mid
    DOWN_SCALE    = 0x2, //video! = disp = mix up
    
    VDP_SCALE_MODE_BUTT        
    
}VDP_SCALE_MODE_E;

//add begin  by h226871
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
//add end  by h226871
//layer pos gen out
typedef struct hiVDP_LAYER_POS_S 
{
    VDP_RECT_S stInReso;
    VDP_RECT_S stPos;
    VDP_RECT_S stPosMinReso;

	
    HI_U32     u32AlignXY;
    HI_U32     u32AlignWH;
    HI_U32     u32AlignX;
    HI_U32     u32AlignY;
	
    HI_U32     u32AlignW;
    HI_U32     u32AlignH;

}VDP_LAYER_POS_S;

//layer pos gen in 
typedef struct hiVDP_POS_GEN_CFG_S 
{
    VDP_RECT_S stReso;
    VDP_POS_MODE_E enPosMode;
    HI_U32 u32SclMode;
    HI_U32 u32AlignXY;
    HI_U32 u32AlignWH;

}VDP_POS_GEN_CFG_S;
	
typedef enum tagVDP_VP_DCI_MODE_E
{
    VDP_VP_DCI_TYP      = 0x0,
    VDP_VP_DCI_RAND     = 0x1,
    VDP_VP_DCI_MAX      = 0x2,
    VDP_VP_DCI_MIN      = 0x3,

    VDP_VP_DCI_CFG_BUTT        
    
}VDP_VP_DCI_MODE_E;

//read mode
typedef enum tagVDP_RM_READ_E
{
    VDP_ZME_READ    = 0,
    VDP_SHARPEN_READ = 1, 
    VDP_DCI_READ     = 2, 
    VDP_GAMMA_READ   = 3, 
    VDP_ACM_READ     = 4, 
    VDP_DIMMING_READ = 5, 
    VDP_VMX_READ     = 6, 
    VDP_CVM_READ     = 7, 
    VDP_VDM_READ     = 8, 
    VDP_GDM_DEGAMM   = 9, 
    VDP_GDM_GAMM     = 10, 
    VDP_MD_READ      = 11,
    VDP_ZME2_READ    = 12,    
    VDM_COEF_DEGAMMA = 13,
    VDM_COEF_TONEMAPPING = 14,
    VDM_COEF_GAMMA   = 15,
    GDM_COEF_TONEMAPPING = 16,
    GDM_COEF_GAMMA   = 17

} VDP_RM_READ_E ;

typedef struct hiVDP_SCL_NONLINEAR_S
{ 
    HI_U32    u32StartStep;
    HI_U32    u32LZoneEndPos; 
    HI_U32    u32MZoneEndPos;
    HI_S32    s32LZone_delta;
    HI_S32    s32RZone_delta;    
    HI_BOOL   bEnable;
}VDP_SCL_NONLINEAR_S;

typedef enum tagVDP_DISP_DIMMING_MODE_E
{
    VDP_DISP_DIMMING_TYP0      = 0x0 ,
    VDP_DISP_DIMMING_TYP1      = 0x1 ,
    VDP_DISP_DIMMING_TYP2      = 0x2 ,
    VDP_DISP_DIMMING_TYP3      = 0x3 ,
    VDP_DISP_DIMMING_TYP4      = 0x4 ,
    VDP_DISP_DIMMING_TYP5      = 0x5 ,
    VDP_DISP_DIMMING_TYP6      = 0x6 ,
    VDP_DISP_DIMMING_TYP7      = 0x7 ,
    VDP_DISP_DIMMING_TYP8      = 0x8 ,
    VDP_DISP_DIMMING_TYP9      = 0x9 ,
    VDP_DISP_DIMMING_TYP10     = 0x10,
    VDP_DISP_DIMMING_TYP11     = 0x11,
    VDP_DISP_DIMMING_TYP12     = 0x12,
    VDP_DISP_DIMMING_TYP13     = 0x13,
    VDP_DISP_DIMMING_TYP14     = 0x14,
    VDP_DISP_DIMMING_TYP15     = 0x15,
    VDP_DISP_DIMMING_TYP16     = 0x16,
    VDP_DISP_DIMMING_TYP17     = 0x17,
    VDP_DISP_DIMMING_MAX       = 0x18,
    VDP_DISP_DIMMING_MIN       = 0x19,
    VDP_DISP_DIMMING_RAND      = 0x20,
    VDP_DISP_DIMMING_RAND1     = 0x21,
    VDP_DISP_DIMMING_2CHN      = 0x22,
    VDP_DISP_DIMMING_4k_13_TEST= 0x23,
    VDP_DISP_DIMMING_4k_31_TEST= 0x24,
    VDP_DISP_DIMMING_4k_01_TEST= 0x25,
    VDP_DISP_DIMMING_4k_02_TEST= 0x26,

    VDP_DISP_DIMMING_CFG_BUTT        
    
}VDP_DISP_DIMMING_MODE_E;
typedef enum tagVDP_VP_SR_MODE_E
{
    VDP_VP_SR_TYP    = 0x0,
    VDP_VP_SR_RAND     ,
    VDP_VP_SR_MAX      ,
    VDP_VP_SR_MIN      ,

    VDP_VP_SR_CFG_BUTT        
    
}VDP_VP_SR_MODE_E;
#if 0
typedef enum tagVDP_SR_COEF_MODE_E
{
    VDP_SR_COEF_TYP  = 1,
    VDP_SR_COEF_MAX = 1,
    VDP_SR_COEF_MIN = 1,
    VDP_SR_COEF_RAND = 1,
   
    VDP_SR_COEF_BUTT
} VDP_SR_COEF_MODE_E;
#endif
typedef enum tagVDP_V0_ES_MODE_E
{
    VDP_V0_ES_TYP_1    = 0x0,
    VDP_V0_ES_TYP_2    ,
    VDP_V0_ES_RAND     ,
    VDP_V0_ES_MAX      ,
    VDP_V0_ES_MIN      ,
    VDP_V0_ES_CFG_BUTT        
    
}VDP_V0_ES_MODE_E;

typedef enum tagVDP_TEST_MODE_E
{
    VDP_TEST_MODE_NORM = 0,
    VDP_TEST_MODE_RAND = 1,
    VDP_TEST_MODE_COMB = 2,

    VDP_TEST_MODE_BUTT
}VDP_TEST_MODE_E;


typedef struct tagVDP_LAYER_CFG_S
{
    HI_U32                  u32LayerEn          ; 
    VDP_LAYER_VID_E         enVidLayer          ; 
    HI_U32                  enInFmt             ; 
    VDP_RECT_S              stInReso            ; //it needn't config!
    VDP_RECT_S              stOutReso           ; //it needn't config!
    VDP_RECT_S              stVidPos            ; //it needn't config!
    VDP_RECT_S              stDispPos           ; //it needn't config!
    HI_U32                  u32CropEn           ; 
    VDP_RECT_S				stCrop                        ; //it needn't config!
    HI_U32                  u32YAddr            ; //it needn't config!
    HI_U32                  u32CAddr            ; //it needn't config!
    HI_U32                  u32YStride          ; //it needn't config!
    HI_U32                  u32YAddrR           ; //it needn't config!
    HI_U32                  u32CAddrR           ; //it needn't config!
    HI_U32                  u32CStride          ; //it needn't config!
    VDP_DATA_WTH            enDataWidth         ; 
    HI_U32                  u32gAlpha           ; 
    VDP_DATA_RMODE_E        enReadMode          ; 
    HI_U32                  u32DrawMode         ; 
    HI_BOOL                 bUvorder            ; 
    HI_U32                  u32DcmpEn           ; //
    HI_U32                  u32TileDcmpEn       ; //
    HI_U32                  u32CmpMode          ;  
    HI_U32                  u32PixelLoss        ;
    HI_U32                  u32AlphaLoss        ;

    HI_U32                  u32LHeadAddr        ; 
    HI_U32                  u32CHeadAddr        ; 
    HI_U32                  u32Size             ; 
    HI_U32                  u32HorZmeEn         ; //it needn't config!
    HI_U32                  u32VerZmeEn         ; //it needn't config!
    HI_U32                  u32HFirEn           ; //it needn't config!
    HI_U32                  u32VFirEn           ; //it needn't config!
    HI_U32                  u32HorZme2En        ; //it needn't config!
    HI_U32                  u32VerZme2En        ; //it needn't config!
    HI_U32                  u32Zme2HFirEn       ; //it needn't config!
    HI_U32                  u32Zme2VFirEn       ; //it needn't config!
    HI_U32                  u32Zme2HMidEn       ; //it needn't config!
    HI_U32                  u32Zme2VMidEn       ; //it needn't config!
    HI_U32                  u32Zme2ShootCtrlEn  ; //it needn't config!
    HI_U32                  u32Zme2ShootCtrlMode; //it needn't config!
    HI_U32                  u32Zme2HVOrder      ; //it needn't config!
    HI_BOOL                 bSharpenEn          ; 
    VDP_VID_SHARPEN_MODE_E  enSharpMode         ; 
    HI_U32                  u32SharpenPos       ; 
    HI_BOOL                 bSharpenDebugEn     ; 
    HI_U32                  u32SharpenDebugMode ; 
    VDP_IFIRMODE_E			stIfirMode          ; 
    HI_BOOL					bDciEn              ; 
    HI_BOOL                 bMute               ; 
    VDP_BKG_S				stVidMuteBkg        ; //it needn't config!
    VDP_BKG_S				stVidBkg            ; //it needn't config!
    VDP_BKG_S				stBkg               ; 
    HI_U32                  region_en           ; 
    HI_U32                  u32MultiModeEn      ; //it needn't config!
    HI_U32                  u32AllRegionDisable ; //it needn't config!

    //vp
    HI_BOOL                 bCscEnable          ; 
    VDP_CSC_MODE_E          enCscMode           ; 
    VDP_CSC_COEF_S          st_csc_coef         ; //it needn't config!
    VDP_CSC_DC_COEF_S       st_csc_dc_coef      ; //it needn't config!
    HI_BOOL					bAcmEn              ; 
    HI_BOOL					bAccEn              ; 
    HI_BOOL					bAcmDebug           ; 
    HI_BOOL					bAccDebug           ; 
    HI_BOOL					bPreReadEn          ; 
    HI_U32                  u32VidAlpha         ; 
    HI_BOOL					bDofEn              ; 
    HI_S32					s32LeftStp          ; 
    HI_S32					s32RightStp         ; 

    VDP_SR_PARA_INFO		SrParaInfo          ; //it needn't config!
    VDP_VP_SR_MODE_E		enSrMode            ; //it needn't config!
    VDP_SR_SCALE_MODE_E     enSrScaleMd         ; //it needn't config!
    VDP_SR_SCALE_MODE_E     enSrScaleMode       ; //it needn't config!
    VDP_SR_IORESO			stIOReso            ; //it needn't config!
    HI_U32					SrMode              ; //it needn't config!
    VDP_SR_IORESO			stSrIOReso          ; //it needn't config!
    HI_U32                  u32LumLosslessEn    ; //it needn't config!
    HI_U32                  u32ChmLosslessEn    ; //it needn't config!
    HI_BOOL					bCmpEn              ; 
    HI_U32					u32HeadSize         ; 
    HI_U32                  u32DataWidth        ; 
    HI_U32                  u32LossEn           ; //it needn't config!


    HI_U32                  u32RegionNumX       ; //it needn't config!
    HI_U32                  u32RegionNumY       ; //it needn't config!
    //axi  //it needn't config!
    HI_U32					u32PreReadEn        ; 
    HI_U32					u32ReqCtrl          ; 
    HI_U32					u32BurstLenth       ; 
    HI_U32					u32MultiIdEn        ; 
    HI_U32					u32OutStandingRd0   ; 
    HI_U32					u32OutStandingRd1   ; 
    HI_U32					u32OutStandingWr    ; 

    //mmu test
    HI_U32                  u32VidRegionSmmuLumBypass[16];
    HI_U32                  u32VidRegionSmmuChromBypass[16];

    //gfx
    VDP_LAYER_GFX_E         enGfxLayer;
    HI_BOOL                 bIsLossEn;
    HI_U32					u32BitExtend; 
    HI_U32					u32PalphaEn; 
    HI_U32					u32GfxGalpha; 
    HI_U32					u32PreMultEn; 
    HI_U32					u32DePreMultEn; 
    HI_U32					u32UpdMode; 
    HI_U32					u32KeyEn; 
    VDP_GFX_CKEY_S          stKey;
    VDP_GFX_MASK_S          stMsk;
    HI_U32					u32GfxPalphaRange; 

    //wbc
    VDP_WBC_3D_MODE_E       enThreeDMode;
    VDP_WBC_OFMT_E          enOutFmt;
	VDP_LAYER_CONN_E        enConnPoint;
	VDP_LAYER_CONN_E        enWbcVpConnPoint;
	HI_U32                  u32CdsEn;
    
    VDP_LAYER_CONN_E        u32LayerConn; 
    VDP_TEST_MODE_E         enTestMode;

    VDP_DISP_MODE_E         enDispMode;
    HI_U32                  u32CbmIop;
    HI_U32                  u32IntfWidth;
    HI_U32                  u32IntfHght ;
    HI_BOOL                 bIop;

    VDP_RM_LAYER_E          enRmLayer;
    HI_U32                  num;
    HI_U32                  u32MmuGlbBypassEn;
}VDP_LAYER_CFG_S;






typedef enum tagVDP_MULTI_MODE_E
{
    VDP_MULTI_GAP_EQ_WTH     = 0,
    VDP_MULTI_GAP_NOEQ_WTH   = 1,
    VDP_MULTI_NOGAP_EQ_WTH   = 2,
    VDP_MULTI_NOGAP_NOEQ_WTH = 3,
    VDP_MULTI_RAND           = 4,
    VDP_MULTI_MODE_RAND      = 5,
    
    VDP_MULTI_BUTT

} VDP_MULTI_MODE_E;

typedef struct
{
    HI_U32 u32IntfIop;
    HI_U32 u32HorNum;
    HI_U32 u32VerNum;
    HI_U32 u32Wth;
    HI_U32 u32Hgt;
    VDP_MULTI_MODE_E enMultiMode;

    HI_U32 u32_laddr   ;  
    HI_U32 u32_caddr   ; 
    HI_U32 u32YStride  ;
    HI_U32 u32CStride  ;
    HI_U32 enInFmt;

} VDP_MULTI_CFG_S;

typedef enum tagVDP_V0_USING_MODE_E
{
    VDP_V0_USING_CONST_0    = 0x0,
    VDP_V0_USING_CONST_1         ,
    VDP_V0_USING_RAND_BY_TYPE    ,//pick one cfg in 16 cfg type, only three cfg type make using = 1
    VDP_V0_USING_RAND_BY_USING   ,//50% using =0 50% using =1
    VDP_V0_USING_SET_TYPE        ,
    VDP_V0_USING_SET_USING       , 
    VDP_V0_USING_WRONG_USING        
    
}VDP_V0_USING_MODE_E;

typedef enum tagVDP_V0_TNR_MODE_E
{
    VDP_V0_TNR_TYP_1    = 0x0,
    VDP_V0_TNR_ZERO     ,
    VDP_V0_TNR_RAND     ,
    VDP_V0_TNR_MAX      ,
    VDP_V0_TNR_MIN      ,
    VDP_V0_TNR_CFG_BUTT        
    
}VDP_V0_TNR_MODE_E;

typedef enum tagVDP_V0_SNR_MODE_E
{
    VDP_V0_SNR_TYP_1    = 0x0,
    VDP_V0_SNR_TYP_2    = 0x0,
    VDP_V0_SNR_ZERO     ,
    VDP_V0_SNR_RAND     ,
    VDP_V0_SNR_MAX      ,
    VDP_V0_SNR_MIN      ,
    VDP_V0_SNR_CFG_BUTT        
    
}VDP_V0_SNR_MODE_E;

typedef enum tagVDP_V0_DBM_MODE_E
{
    VDP_V0_DBM_TYP_1    = 0x0,
    VDP_V0_DBM_STREAM   ,
    VDP_V0_DBM_RAND     ,
    VDP_V0_DBM_MAX      ,
    VDP_V0_DBM_MIN      ,
    VDP_V0_DBM_CFG_BUTT        
    
}VDP_V0_DBM_MODE_E;


typedef enum tagVDP_V0_DB_DETECT_MODE_E
{
    VDP_V0_DB_DETECT_TYP_1    = 0x0,
    VDP_V0_DB_DETECT_STREAM   ,
    VDP_V0_DB_DETECT_RAND     ,
    VDP_V0_DB_DETECT_MAX      ,
    VDP_V0_DB_DETECT_MIN      ,
    VDP_V0_DB_DETECT_CFG_BUTT        
    
}VDP_V0_DB_DETECT_MODE_E;

typedef enum 
{
    VDP_HDR_COMPOSER_CFG_TYP = 0,
    VDP_HDR_COMPOSER_CFG_RAND ,
    VDP_HDR_COMPOSER_CFG_RAND_COEF ,
    VDP_HDR_COMPOSER_CFG_MAX  ,
    VDP_HDR_COMPOSER_CFG_MIN  ,
    VDP_HDR_COMPOSER_CFG_BUTT
}VDP_HDR_COMPOSER_CFG_E;

typedef enum 
{
    VDP_BT2020_CFG_TYP = 0,
    VDP_BT2020_CFG_RAND ,
    VDP_BT2020_CFG_MAX  ,
    VDP_BT2020_CFG_MIN  ,
    VDP_BT2020_CFG_BUTT
}VDP_BT2020_CFG_E;
typedef enum 
{
    VDP_HDR_DM_CFG_TYP = 0,
    VDP_HDR_DM_CFG_RAND ,
    VDP_HDR_DM_CFG_RAND_COEF ,
    VDP_HDR_DM_CFG_RAND_YUV2RGB ,
    VDP_HDR_DM_CFG_RAND_RGB2LMS ,
    VDP_HDR_DM_CFG_RAND_LMS2IPT ,
    VDP_HDR_DM_CFG_RAND_IPT2LMS ,
    VDP_HDR_DM_CFG_RAND_LMS2RGB ,
    VDP_HDR_DM_CFG_RAND_RGB2YUV ,
    VDP_HDR_DM_CFG_MAX  ,
    VDP_HDR_DM_CFG_MIN  ,
    VDP_HDR_DM_CFG_ZERO ,
    VDP_HDR_DM_CFG_12BIT_YUV2RGB,
    VDP_HDR_DM_CFG_14BIT_YUV2RGB,
    VDP_HDR_DM_CFG_BUTT
}VDP_HDR_DM_CFG_E;


typedef enum 
{
    VDP_HDR_CLR_SPACE_601= 0,
    VDP_HDR_CLR_SPACE_709 ,
    VDP_HDR_CLR_SPACE_2020 ,
    VDP_HDR_CLR_SPACE_BUTT

}VDP_HDR_CLR_SPACE_E;

typedef enum 
{
    VDP_HD_DIV1_SD_DIV1= 0,
    VDP_HD_DIV1_SD_DIV2 ,
    VDP_HD_DIV1_SD_DIV4 ,
    VDP_HD_DIV2_SD_DIV2 ,
    VDP_HD_DIV2_SD_DIV4 ,
    VDP_HD_DIV4_SD_DIV4 ,
    VDP_CLK_CLR_SPACE_BUTT
}VDP_CLK_CFG_E;

#if VID_BT2020_EN
typedef enum 
{
    VDP_BT2020_TYP = 0,
    VDP_BT2020_RAND ,
    VDP_BT2020_MAX  ,
    VDP_BT2020_MIN  ,
    VDP_BT2020_BUTT
}VDP_BT2020_MODE_E;
typedef enum 
{
    VDP_HDR_DM_TYP = 0,
    VDP_HDR_DM_RAND ,
    VDP_HDR_DM_RAND_COEF ,
    VDP_HDR_DM_RAND_YUV2RGB ,
    VDP_HDR_DM_RAND_RGB2LMS ,
    VDP_HDR_DM_RAND_LMS2IPT ,
    VDP_HDR_DM_RAND_IPT2LMS ,
    VDP_HDR_DM_RAND_LMS2RGB ,
    VDP_HDR_DM_RAND_RGB2YUV ,
    VDP_HDR_DM_MAX  ,
    VDP_HDR_DM_MIN  ,
    VDP_HDR_DM_ZERO ,
    VDP_HDR_DM_12BIT_YUV2RGB,
    VDP_HDR_DM_14BIT_YUV2RGB,
    VDP_HDR_DM_BUTT
}VDP_HDR_DM_MODE_E;


//typedef enum 
//{
//    VDP_CLR_SPACE_601= 0,
//    VDP_CLR_SPACE_709 ,
//    VDP_CLR_SPACE_2020 ,
//    VDP_CLR_SPACE_BUTT
//
//}VDP_CLR_SPACE_E;


//typedef struct
//{
//	VDP_CLR_SPACE_E   en_v0_space;
//	VDP_CLR_SPACE_E   en_v1_space;
//	VDP_CLR_SPACE_E   en_out_space;
//	VDP_BT2020_MODE_E en_bt2020_mode;
//
//}VDP_BT2020_MODE_CFG_T;
#endif
//typedef struct
//{
//	VDP_HDR_CLR_SPACE_E   en_v0_space;
//	VDP_HDR_CLR_SPACE_E   en_v1_space;
//	VDP_HDR_CLR_SPACE_E   en_out_space;
//	VDP_BT2020_CFG_E en_bt2020_mode;
//
//}VDP_BT2020_MODE_CFG_T;



HI_VOID VDP_FUNC_SetDciMode(VDP_LAYER_VP_E  enLayer, VDP_VP_DCI_MODE_E DciMode, HI_U32 w, HI_U32 h);
HI_VOID VDP_FUNC_SetDciModeIop(VDP_LAYER_VP_E  enLayer, VDP_VP_DCI_MODE_E DciMode, HI_U32 iop, HI_U32 w, HI_U32 h);

//HI_VOID VDP_FUNC_SetSrCoefMode (HI_U32 u32Data, VDP_SR_COEF_MODE_E enSrMode);
HI_VOID VDP_FUNC_SetSrMode(VDP_SR_PARA_INFO SrParaInfo, 
                           VDP_VP_SR_MODE_E SrMode, 
                           VDP_SR_IORESO IOReso);

HI_VOID VDP_FUNC_SetEsMode(VDP_ES_PARA_INFO EsParaInfo, VDP_V0_ES_MODE_E EsMode);

//HI_VOID VDP_FUNC_SetAcmMode(VDP_TEST_MODE_E enAcmMode);

HI_S32 VDP_VID_GetNonLnrSclInfo(HI_U32 u32ZoneMWidthIn,
		                        HI_U32 u32ZoneMWidthOut,
		                        HI_U32 u32DeltaConfig,
		                        HI_U32 u32WidthIn,
							   	HI_U32 u32WidthOut,
								VDP_SCL_NONLINEAR_S *pstLumaNLnr,
								VDP_SCL_NONLINEAR_S *pstChomNLnr);

HI_VOID VDP_FUNC_SetVdpUsing(VDP_V0_USING_MODE_E UsingMode, double clk_ppc_t, HI_U32 using_cfg);
HI_VOID VDP_FUNC_SetTnrMode(VDP_TNR_PARA_INFO TnrParaInfo, VDP_V0_TNR_MODE_E TnrMode);
HI_VOID VDP_FUNC_SetSnrMode(VDP_SNR_PARA_INFO SnrParaInfo, VDP_V0_SNR_MODE_E SnrMode);
HI_VOID VDP_FUNC_SetDbmMode(VDP_DBM_PARA_INFO DbmParaInfo, VDP_V0_DBM_MODE_E DbmMode);
HI_VOID VDP_FUNC_SetDbDetectMode(VDP_DB_DETECT_PARA_INFO DbDetectParaInfo, VDP_V0_DB_DETECT_MODE_E DbDetectMode);

typedef enum tagVDP_WCG_MODE_E
{
    VDP_WCG_MODE_TYP      = 0x0,
    VDP_WCG_MODE_RAND     = 0x1,
    VDP_WCG_MODE_MAX      = 0x2,
    VDP_WCG_MODE_MIN      = 0x3,

    VDP_WCG_MODE_BUTT        
    
}VDP_WCG_MODE_E;

HI_VOID VDP_FUNC_SetWcgGmaLinThr   (VDP_WCG_COEF_S *stWcgCoef, VDP_WCG_MODE_E WcgMode);
HI_VOID VDP_FUNC_SetWcgGmaLincoef  (VDP_WCG_COEF_S *stWcgCoef, VDP_WCG_MODE_E WcgMode);
HI_VOID VDP_FUNC_SetWcgLinGmaThr   (VDP_WCG_COEF_S *stWcgCoef, VDP_WCG_MODE_E WcgMode);
HI_VOID VDP_FUNC_SetWcgLinGmaCoef  (VDP_WCG_COEF_S *stWcgCoef, VDP_WCG_MODE_E WcgMode);
HI_VOID VDP_FUNC_SetWcgCscCoef     (VDP_WCG_COEF_S *stWcgCoef, VDP_WCG_MODE_E WcgMode);
HI_VOID VDP_FUNC_SetDimmingMode(VDP_CHN_E  enLayer, VDP_DISP_DIMMING_MODE_E  DimmingMode, HI_U32 w, HI_U32 h);


#if EDA_TEST
HI_VOID VDP_FUNC_CfgInit        (VDP_LAYER_CFG_S * pstCfg, HI_U32 u32Wth, HI_U32 u32Hgt);
HI_VOID VDP_FUNC_SetVid            (HI_U32 u32LayerId, VDP_LAYER_CFG_S * pstCfg);
HI_VOID VDP_FUNC_SetVp             (HI_U32 u32LayerId, VDP_LAYER_CFG_S * pstCfg);
HI_VOID VDP_FUNC_SetGfx            (HI_U32 u32LayerId, VDP_LAYER_CFG_S * pstCfg);
HI_VOID VDP_FUNC_SetGp             (HI_U32 u32LayerId, VDP_LAYER_CFG_S * pstCfg);
HI_VOID VDP_FUNC_SetWbc            (HI_U32 u32LayerId, VDP_LAYER_CFG_S * pstCfg);
HI_VOID VDP_FUNC_SetReadCoef       (VDP_RM_LAYER_E enLayer,VDP_RM_READ_E ReadMode);
HI_VOID VDP_FUNC_SetDhd            (HI_U32 u32LayerId);
#endif
HI_VOID VDP_FUNC_SetMultiMode      (VDP_LAYER_VID_E enLayer, VDP_MULTI_CFG_S stMultiCfg);
HI_VOID VDP_FUNC_LayerPosGen       (VDP_POS_GEN_CFG_S stPosGenCfg, VDP_LAYER_POS_S * stLayerPos);
HI_VOID VDP_SetRegionCfg           (VDP_REGION_CFG_S  stRegionCfg);

//add by h226871
HI_VOID VDP_FUNC_SetVidCfg(HI_U32 u32LayerId, VDP_LAYER_CFG_S *pstCfg);
HI_VOID VDP_FUNC_SetVidComb( VDP_LAYER_CFG_S *st_vid_cfg,VDP_LAYER_CFG_S *st_vp0_cfg);
HI_VOID VDP_FUNC_SetVpRandCfg( VDP_LAYER_CFG_S  *stVidInfo,VDP_LAYER_CFG_S *st_vp0_cfg);
HI_VOID VDP_FUNC_SetVpComb(HI_U32 u32LayerId, VDP_LAYER_CFG_S *pstCfg);

HI_VOID VDP_FUNC_SetGpRandCfg( VDP_LAYER_CFG_S *st_gfx_cfg,VDP_LAYER_CFG_S *st_gp0_cfg);
HI_VOID VDP_FUNC_SetGfxCombCfg( VDP_LAYER_CFG_S *st_g0_cfg,VDP_LAYER_CFG_S *st_gp0_cfg);

HI_VOID VDP_SetWbcVp_VpAcsc_ppi(VDP_DISP_MODE_E setdisp_mode,VDP_RECT_S dhd0_rect);
HI_VOID VDP_SetWbcVp_Vidbzme_ppp(VDP_DISP_MODE_E setdisp_mode,VDP_RECT_S dhd0_rect);
HI_VOID VDP_SetWbcVp_Vidbzme_ppi(VDP_DISP_MODE_E setdisp_mode,VDP_RECT_S dhd0_rect);
HI_VOID VDP_SetWbcVp_VpAcsc_ppp(VDP_DISP_MODE_E setdisp_mode,VDP_RECT_S dhd0_rect);
HI_VOID VDP_SetWbcVp_Vidbzme_pii(VDP_DISP_MODE_E setdisp_mode,VDP_RECT_S dhd0_rect);
HI_VOID VDP_SetWbcVp_Vidbzme_pip(VDP_DISP_MODE_E setdisp_mode,VDP_RECT_S dhd0_rect);
HI_VOID VDP_SetWbcVp_VpAcsc_iii(VDP_DISP_MODE_E setdisp_mode,VDP_RECT_S dhd0_rect);
HI_VOID VDP_SetWbcVp_Vidbzme_iii(VDP_DISP_MODE_E setdisp_mode,VDP_RECT_S dhd0_rect);

//#if VID_VMX_EN
//HI_VOID VDP_FUNC_SetVmxMode (VDP_LAYER_VMX_E enLayer, VDP_VMX_MODE_E VmxMode, HI_U32 iw, HI_U32 ih, VDP_VMX_CFG_S * vmx_cfg_in);
//#endif
//
//#if VID_NXG_EN
//HI_VOID VDP_FUNC_SetNxgMode(HI_U32 enLayer, VDP_NXG_NXG_MODE_E NxgMode, HI_U32 iw, HI_U32 ih, VDP_NXG_CFG_S stCfg) ;
//#endif

////HI_VOID VDP_FUNC_HdrComposerMdsParse(rpu_ext_config_fixpt_main_t* comp_cfg, const char *file_metadata, HI_U32 frm_cnt);
////HI_VOID VDP_FUNC_SetComposerMode(VDP_HDR_COMPOSER_CFG_E ComposerMode, rpu_ext_config_fixpt_main_t* comp_cfg);
////
////HI_VOID VDP_FUNC_HdrDmMdsParse(DmKsFxp_t * pKs, const char *file_metadata, HI_U32 frm_cnt);
////HI_VOID VDP_FUNC_SetDmMode(VDP_HDR_DM_CFG_E DmMode, DmKsFxp_t * pKs);
////HI_VOID VDP_FUNC_SetBt2020Mode(HI_U32 enLayer, VDP_BT2020_MODE_CFG_T*);
////
////HI_U32 VDP_FUNC_GetHdrHdmiMdLen(HI_U32 frm_num, const char *FileName);
////
////HI_U32 VDP_FUNC_HdrHdmiMdParse(HI_U8 *buf, HI_U32 size, const char *FileName);
////HI_U32 VDP_FUNC_HdrHdmiHeadParse(HI_U32 frm_num, const char *FileName);

/*
typedef enum tagVDP_HDR_CFG_MODE_E
{
    VDP_HDR_DOVI_BW_IN_DOVI_YUV_OUT  = 0x0 , 
    VDP_HDR_DOVI_NBW_IN_DOVI_YUV_OUT       , 

    VDP_HDR_DOVI_BW_IN_HDR10_OUT           , 
    VDP_HDR_DOVI_NBW_IN_HDR10_OUT          , 
    VDP_HDR_HDR10_IN_HDR10_OUT             , 

	//for STB hdmi output, 444
    VDP_HDR_SDR_IN_SDR_OUT                 , 
    VDP_HDR_DOVI_BW_IN_SDR_OUT             , 
    VDP_HDR_DOVI_NBW_IN_SDR_OUT            , 
    VDP_HDR_HDR10_IN_SDR_OUT               , 

    VDP_HDR_DOVI_BW_IN_DOVI_IPT_OUT        , 
    VDP_HDR_DOVI_NBW_IN_DOVI_IPT_OUT       , 

	//for certificate: 422 uyvy output
    VDP_HDR_SDR_IN_SDR_OUT_CERT            , 
    VDP_HDR_DOVI_BW_IN_SDR_OUT_CERT        , 
    VDP_HDR_DOVI_NBW_IN_SDR_OUT_CERT       , 
    VDP_HDR_HDR10_IN_SDR_OUT_CERT          , 

	//for FPGA test
    VDP_HDR_DOVI_BW_IN_HDR10_OUT_FPGA      , 
    VDP_HDR_DOVI_NBW_IN_HDR10_OUT_FPGA     , 
    VDP_HDR_HDR10_IN_HDR10_OUT_FPGA        , 

    VDP_HDR_CFG_MODE_BUTT        
    
}VDP_HDR_CFG_MODE_E;


typedef struct
{
	VDP_HDR_CFG_MODE_E enHdrMode;
	VDP_DATA_WTH u32DataWidth;
	VDP_RECT_S stVidReso;
	VDP_RECT_S stBlReso;
	VDP_RECT_S stElReso;
	VDP_RECT_S stGfxReso;

	HI_U32 u32GfxEn;
	HI_BOOL bElEn;
	HI_BOOL bMuteEn;
	//HI_U32 u32CoefAddr[5];

} VDP_HDR_CFG_S;
*/

typedef enum tagVDP_HDR_GDM_MODE_E
{
    VDP_HDR_GDM_TYP = 0,
    VDP_HDR_GDM_RAND ,
    VDP_HDR_GDM_RAND_COEF ,
    VDP_HDR_GDM_ZERO ,
    VDP_HDR_GDM_RAND_YUV2RGB ,
    VDP_HDR_GDM_RAND_RGB2LMS ,
    VDP_HDR_GDM_RAND_LMS2IPT ,
    VDP_HDR_GDM_RAND_IPT2LMS ,
    VDP_HDR_GDM_RAND_LMS2RGB ,
    VDP_HDR_GDM_RAND_RGB2YUV ,
    VDP_HDR_GDM_MAX  ,
    VDP_HDR_GDM_MIN  ,
    VDP_HDR_GDM_BUTT
}VDP_HDR_GDM_MODE_E;

typedef enum tagVDP_HDR_CFG_MODE_E
{
    VDP_HDR_DOVI_BC_IN_DOVI_YUV_OUT  = 0x0 , 
    VDP_HDR_DOVI_NBC_IN_DOVI_YUV_OUT       , 

    VDP_HDR_DOVI_BC_IN_HDR10_OUT           , 
    VDP_HDR_DOVI_NBC_IN_HDR10_OUT          , 
    VDP_HDR_HDR10_IN_HDR10_OUT             , 

	//for STB hdmi output, 444
    VDP_HDR_SDR_IN_SDR_OUT                 , 
    VDP_HDR_DOVI_BC_IN_SDR_OUT             , 
    VDP_HDR_DOVI_NBC_IN_SDR_OUT            , 
    VDP_HDR_HDR10_IN_SDR_OUT               , 

    VDP_HDR_DOVI_BC_IN_DOVI_IPT_OUT        , 
    VDP_HDR_DOVI_NBC_IN_DOVI_IPT_OUT       , 

	//for certificate: 422 uyvy output
    VDP_HDR_SDR_IN_SDR_OUT_CERT            , 
    VDP_HDR_DOVI_BC_IN_SDR_OUT_CERT        , 
    VDP_HDR_DOVI_NBC_IN_SDR_OUT_CERT       , 
    VDP_HDR_HDR10_IN_SDR_OUT_CERT          , 

	//for FPGA test
    VDP_HDR_DOVI_BC_IN_HDR10_OUT_FPGA      , 
    VDP_HDR_DOVI_NBC_IN_HDR10_OUT_FPGA     , 
    VDP_HDR_HDR10_IN_HDR10_OUT_FPGA        , 

    VDP_HDR_CFG_MODE_BUTT        
    
}VDP_HDR_CFG_MODE_E;


typedef struct
{
	VDP_HDR_CFG_MODE_E enHdrMode;
	VDP_DATA_WTH u32DataWidth;//0:8bit; 1:10bit
	VDP_RECT_S stVidReso;//vid input reso
	VDP_RECT_S stBlReso;//vid zme output reso
	VDP_RECT_S stElReso;//el input reso
	VDP_RECT_S stGfxReso;//gfx input reso

	HI_BOOL bMuteEn;
	HI_BOOL bGfxEn;
	//HI_U32 u32CoefAddr[5];

} VDP_HDR_CFG_S;
#if VDP_HDR_MODE 
HI_VOID VDP_FUNC_SetGdmMode(VDP_HDR_GDM_MODE_E DmMode);
#endif
HI_U32 VDP_FUNC_SetHdrMode(VDP_HDR_CFG_S *stHdrCfg, HI_U32 u32FrmNum, const char *pFilePath);
HI_VOID VDP_FUNC_SetClkMode(VDP_CLK_CFG_E ClkMode);

//// HI_S32 VDP_DRV_SetHdrMode         ( VDP_HDR_CFG_S *stHdrCfg, rpu_ext_config_fixpt_main_t *pstCompCfg, DmKsFxp_t *pstDmKs);
//// HI_S32 VDP_DRV_SetHdrEdaCfg       ( VDP_HDR_CFG_S *stHdrCfg);
//// HI_S32 VDP_DRV_SetComposer        ( rpu_ext_config_fixpt_main_t* pstCompCfg);
//// HI_S32 VDP_DRV_SetVdm             ( DmKsFxp_t* pstDmKs);
//// HI_S32 VDP_DRV_SetHdrHdmiMetadata ( HI_U8 * pu8HeadData, HI_U32 u32MdLen, HI_U8 * pu8MdBuf);
#if VID_BT2020_EN
//HI_VOID VDP_FUNC_SetBt2020Mode(HI_U32 enLayer, VDP_BT2020_MODE_CFG_T * ModeCfg);
#endif

#endif

