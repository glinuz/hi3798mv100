//******************************************************************************
//  Copyright (C), 2007-2014, Hisilicon Technologies Co., Ltd.
//
//******************************************************************************
// File name     : Reg_HEVC_H264.h
// Version       : 2.0
// Author        : l00214825
// Created       : 2014-10-28
// Last Modified : 
// Description   :  逻辑寄存器结构体
// Function List : 
// History       : 
// 1 Date        : 
// Author        : xxx
// Modification  : Create file
//******************************************************************************
#include "./inc/cabac_reg.h"
#include "./inc/curld_reg.h"
#include "./inc/dblk_reg.h"
#include "./inc/fme_reg.h"
#include "./inc/ime_reg.h"
#include "./inc/intra_reg.h"
#include "./inc/mrg_reg.h"
#include "./inc/nbi_reg.h"
#include "./inc/pack_reg.h"
#include "./inc/pmeld_reg.h"
//#include "./inc/pmest_reg.h"
#include "./inc/pme_reg.h"
#include "./inc/pmv_reg.h"
#include "./inc/qpg_reg.h"
#include "./inc/recst_reg.h"
#include "./inc/refld_reg.h"
#include "./inc/sao_reg.h"
#include "./inc/sel_reg.h"
#include "./inc/tqitq_reg.h"
#include "./inc/vcpi_reg.h"
#include "./inc/vctrl_reg.h"
#include "./inc/vlcst_reg.h"
#include "./inc/vlc_reg.h"
#include "./inc/rc_reg.h"
#include "./inc/env_reg.h"
#include "./inc/mmu_reg.h"

#ifndef __C_UNION_DEFINE_VEDU_REG_H__
#define __C_UNION_DEFINE_VEDU_REG_H__

/*-------------- Vcpi start  0x20620000 ---------------------*/ 

typedef struct
{ 
    /* Vcpi寄存器结构体   0x20620000  */
    volatile S_vcpi_REGS_TYPE   VEDU_VCPI_REGS;
    
    /* Vctrl寄存器结构体   0x20620200  */
    volatile S_vctrl_REGS_TYPE  VEDU_VCTRL_REGS;
    
    /* Curld寄存器结构体   0x20620400  */
    volatile S_curld_REGS_TYPE  VEDU_CURLD_REGS;
    
    /* Pme寄存器结构体    0x20620600  */
    volatile S_pme_REGS_TYPE  VEDU_PME_REGS;
    
    /* Refid寄存器结构体   0x20620800  */
    volatile S_refld_REGS_TYPE VEDU_REFLD_REGS;
    
    /* Ime寄存器结构体   0x20620a00  */
    volatile S_ime_REGS_TYPE VEDU_IME_REGS;
    
    /* Qpg寄存器结构体   0x20620c00  */
    volatile S_qpg_REGS_TYPE VEDU_QPG_REGS;
    
    /* Mrg寄存器结构体   0x20620e00  */
    volatile S_mrg_REGS_TYPE VEDU_MRG_REGS;
    
    /* Fme寄存器结构体   0x20621000  */
    volatile S_fme_REGS_TYPE VEDU_FME_REGS;
    
    /* Intra寄存器结构体   0x20621200  */
    volatile S_intra_REGS_TYPE VEDU_INTRA_REGS;
    
    /* Pmv寄存器结构体   0x20621400  */
    volatile S_pmv_REGS_TYPE VEDU_PMV_REGS;

    /* sel寄存器结构体   0x20621600  */
	volatile S_sel_REGS_TYPE  VEDU_SEL_REGS;     
       
    /*  Dblk寄存器结构体   0x20621800  */
       volatile S_dblk_REGS_TYPE  VEDU_DBLK_REGS;   
          
    /*  Sao寄存器结构体   0x20621a00  */
     volatile S_sao_REGS_TYPE  VEDU_SAO_REGS;
     
    /* Pack寄存器结构体   0x20621c00  */
 	volatile S_pack_REGS_TYPE  VEDU_PACK_REGS; 
    
    /* Cabac寄存器结构体   0x20621e00  */
    volatile S_cabac_REGS_TYPE  VEDU_CABAC_REGS;    
      
    /* vlc寄存器结构体   0x20622000  */
     volatile S_vlc_REGS_TYPE  VEDU_VLC_REGS;

    /* nbi 寄存器结构体   0x20622200  */
	volatile S_nbi_REGS_TYPE  VEDU_NBI_REGS;

    /* vlcst 寄存器结构体   0x20622400  */ 
	volatile S_vlcst_REGS_TYPE  VEDU_VLCST_REGS;
   
    /* tqitq寄存器结构体   0x20622600  */
	volatile S_tqitq_REGS_TYPE VEDU_TQITQ_REGS; 
     
    /* recst 保留 0x20622a00  */
	  volatile S_recst_REGS_TYPE  VEDU_RECST_REGS;

    /* pmeld 寄存器结构体   0x20622c00  */
	    volatile S_pmeld_REGS_TYPE  VEDU_PMELD_REGS;  
	
    /* pmest 寄存器结构体   0x20622e00  */       
     // volatile S_pmest_REGS_TYPE  VEDU_PMEST_REGS;
      volatile  S_rc_REGS_TYPE  VEDU_RC_REGS;  
	volatile S_env_REGS_TYPE   VEDU_ENV_REGS;  

	/* mmu 寄存器结构体   0x2062f000 */
	volatile S_mmu_REGS_TYPE   VEDU_MMU_REGS;  
	

}S_VEDU_REGS_TYPE;


typedef struct
{
    HI_U32 CABAC_GLB_CFG;
    HI_U32 CABAC_SLCHDR_SIZE;
    HI_U32 CABAC_SLCHDR_PART1;
    HI_U32 CABAC_SLCHDR_PART2_SEG1;

    HI_U32 CABAC_SLCHDR_PART2_SEG2;
    HI_U32 CABAC_SLCHDR_PART2_SEG3;
    HI_U32 CABAC_SLCHDR_PART2_SEG4;
    HI_U32 CABAC_SLCHDR_PART2_SEG5;

    HI_U32 CABAC_SLCHDR_PART2_SEG6;
    HI_U32 CABAC_SLCHDR_PART2_SEG7;
    HI_U32 CABAC_SLCHDR_PART2_SEG8;
    HI_U32 CABAC_SLCHDR_SIZE_I;

    HI_U32 CABAC_SLCHDR_PART1_I;
    HI_U32 CABAC_SLCHDR_PART2_SEG1_I;
    HI_U32 CABAC_SLCHDR_PART2_SEG2_I;
    HI_U32 CABAC_SLCHDR_PART2_SEG3_I;

    HI_U32 CABAC_SLCHDR_PART2_SEG4_I;
    HI_U32 CABAC_SLCHDR_PART2_SEG5_I;
    HI_U32 CABAC_SLCHDR_PART2_SEG6_I;
    HI_U32 CABAC_SLCHDR_PART2_SEG7_I;

    HI_U32 CABAC_SLCHDR_PART2_SEG8_I;
    HI_U32 reserved[11];
}S_CABAC_SLICE_HEAD_TYPE;

typedef struct
{
    HI_U32 VLC_SLCHDRSTRM0;
    HI_U32 VLC_SLCHDRSTRM1;
    HI_U32 VLC_SLCHDRSTRM2;
    HI_U32 VLC_SLCHDRSTRM3;
	
    HI_U32 VLC_REORDERSTRM0;
    HI_U32 VLC_REORDERSTRM1;
    HI_U32 VLC_MARKINGSTRM0;
    HI_U32 VLC_MARKINGSTRM1;
	
    HI_U32 VLC_SLCHDRPARA;
    HI_U32 VLC_SVC;
    HI_U32 VLC_SLCHDRSTRM0_I;
    HI_U32 VLC_SLCHDRSTRM1_I;
	
    HI_U32 VLC_SLCHDRSTRM2_I;
    HI_U32 VLC_SLCHDRSTRM3_I;
    HI_U32 VLC_REORDERSTRM0_I;
    HI_U32 VLC_REORDERSTRM1_I;
	
    HI_U32 VLC_MARKINGSTRM0_I;
    HI_U32 VLC_MARKINGSTRM1_I;
    HI_U32 VLC_SLCHDRPARA_I;
    HI_U32 reserved[13];
}S_VLC_SLICE_HEAD_TYPE;

typedef union
{
    S_CABAC_SLICE_HEAD_TYPE  CABAC_SLICE_HEAD;
    S_VLC_SLICE_HEAD_TYPE    VLC_SLICE_HEAD;
} U_SLICE_HEAD_TYPE;
typedef struct
{
/*************************************************/
/********每帧必须配置的寄存器，384+128=512字节*******/
/***************************************************************************************************************************;*********/
//0x0000 ~ 0x0180
HI_U32 VCPI_PICSIZE_PIX;
HI_U32 VCPI_STRFMT;
HI_U32 CURLD_GCFG;
//HI_U32 VCPI_PIC_STRONG_EN;
HI_U32 CURLD_CLIP_THX ;

HI_U32 VCPI_SW_L0_SIZE		;
HI_U32 VCPI_MV_LOAD;
HI_U32 REFLD_LOAD_WINSIZE;
HI_U32 TQITQ_DEADZONE;

HI_U32 VCPI_I_SLC_INSERT;//插入Islc的开关和slcidx
HI_U32 PME_SAFE_CFG;			//安全线
HI_U32 PME_IBLK_REFRESH;
HI_U32 PME_IBLK_REFRESH_NUM;

HI_U32 VCPI_NBI_MVST_ADDR;
HI_U32 VCPI_NBI_MVLD_ADDR;
HI_U32 VCPI_STRMADDR;
HI_U32 VCPI_SWPTRADDR;

HI_U32 VCPI_SRPTRADDR;
HI_U32 VLCST_STRMBUFLEN;
HI_U32 VCPI_VLC_CONFIG;
HI_U32 VLCST_DESCRIPTOR;

HI_U32 VLCST_SLC_CFG0;
HI_U32 VLCST_SLC_CFG1;
HI_U32 VLCST_SLC_CFG2;
HI_U32 VLCST_SLC_CFG3;

HI_U32 PMV_POC_0;
HI_U32 PMV_POC_1;
HI_U32 PMV_POC_2;
HI_U32 PMV_POC_3;

HI_U32 PMV_POC_4;
HI_U32 PMV_POC_5;
HI_U32 PMV_TMV_EN;
HI_U32 VCPI_REF_FLAG;

HI_U32 VCPI_TUNLCELL_ADDR;
HI_U32 VCPI_SRC_YADDR;
HI_U32 VCPI_SRC_CADDR;
HI_U32 VCPI_SRC_VADDR;

HI_U32 VCPI_YH_ADDR;
HI_U32 VCPI_CH_ADDR;
HI_U32 VCPI_STRIDE;
//HI_U32 VCPI_HEAD_STRIDE;
HI_U32 VCPI_PMEINFO_ST_ADDR ;

HI_U32 VCPI_REC_YADDR;
HI_U32 VCPI_REC_CADDR;
HI_U32 VCPI_REC_STRIDE;
HI_U32 VCPI_REC_YH_ADDR;

HI_U32 VCPI_REC_CH_ADDR;
HI_U32 VCPI_REC_HEAD_STRIDE;
HI_U32 VCPI_REFY_L0_ADDR	;
HI_U32 VCPI_REFC_L0_ADDR;

HI_U32 VCPI_REF_L0_STRIDE;
HI_U32 VCPI_REFYH_L0_ADDR;
HI_U32 VCPI_REFCH_L0_ADDR;
HI_U32 VCPI_REFH_L0_STRIDE;

HI_U32 VCPI_PMELD_L0_ADDR;
HI_U32 VCPI_PMEST_ADDR;
HI_U32 VCPI_PMEINFO_LD0_ADDR;
HI_U32 VCPI_PMEINFO_LD1_ADDR;
//HI_U32 reserved0[1];

HI_U32 VCPI_SW_L1_SIZE		;
HI_U32 VCPI_REFY_L1_ADDR;
HI_U32 VCPI_REFC_L1_ADDR;
HI_U32 VCPI_REF_L1_STRIDE;

HI_U32 VCPI_REFYH_L1_ADDR;
HI_U32 VCPI_REFCH_L1_ADDR;
HI_U32 VCPI_REFH_L1_STRIDE;
HI_U32 VCPI_PMELD_L1_ADDR;

//0x0180 ~ 0x0200
U_SLICE_HEAD_TYPE SLICE_HEAD; // del or not ?

HI_U32 reserved1[32];
/***********************************************************************************/
/**************很少改变的寄存器,256字节,需要外部设置参数才会修改的寄存器*****************/
/***********************************************************************************/
//0x0200 ~ 0x0300
HI_U32 VCPI_TILE_SIZE;
HI_U32 VCPI_SLICE;
HI_U32 VCPI_DBLKCFG;
HI_U32 VCPI_CROSS_TILE_SLC;

HI_U32 VCPI_INTRA_INTER_CU_EN;
HI_U32 VCPI_PRE_JUDGE_EXT_EN;
HI_U32 VCPI_PRE_JUDGE_COST_THR;
HI_U32 VCPI_PME_PARAM;

HI_U32 VCPI_IBLK_PRE_MV_THR;
HI_U32 IME_INTER_MODE;
HI_U32 IME_RDOCFG;
HI_U32 IME_FME_LPOW_THR;

// changed from here

HI_U32 MRG_FORCE_ZERO_EN;
HI_U32 INTRA_INTER_AVAIL;
HI_U32 INTRA_SMOOTH;
HI_U32 INTRA_DC_SHIFT;

HI_U32 INTRA_BIT_WEIGHT;
HI_U32 INTRA_DC_MADI;
HI_U32 INTRA_RDO_COST_OFFSET_1;
HI_U32 INTRA_NO_DC_COST_OFFSET_0;

HI_U32 INTRA_NO_DC_COST_OFFSET_1;
HI_U32 INTRA_CHNL4_ANG_0EN;
HI_U32 INTRA_CHNL4_ANG_1EN;
HI_U32 INTRA_CHNL8_ANG_0EN;

HI_U32 INTRA_CHNL8_ANG_1EN;
HI_U32 INTRA_CHNL16_ANG_0EN;
HI_U32 INTRA_CHNL16_ANG_1EN;
HI_U32 INTRA_CHNL32_ANG_0EN;

HI_U32 INTRA_CHNL32_ANG_1EN;
HI_U32 PACK_CU_PARAMETER;
HI_U32 PACK_PCM_PARAMETER;
HI_U32 PACK_TF_SKIP_FLAG;
HI_U32 reserved2[32];

/***********************************************************************************/
/********************OSD和ROI信息，384字节，外部设置参数才会修改的寄存器**************/
/***********************************************************************************/
//0x0300 ~ 0x0480
HI_U32 VCPI_OSD_ENABLE;
HI_U32 VCPI_OSD_POS[8];
HI_U32 VCPI_OSD_SIZE[8];
HI_U32 VCPI_OSD_LAYERID;

HI_U32 VCPI_OSD_QP0;
HI_U32 VCPI_OSD_QP1;
HI_U32 CURLD_OSD01_ALPHA;
HI_U32 CURLD_OSD23_ALPHA;

HI_U32 CURLD_OSD45_ALPHA;
HI_U32 CURLD_OSD67_ALPHA;
HI_U32 CURLD_OSD_GALPHA0;
HI_U32 CURLD_OSD_GALPHA1;

HI_U32 CURLD_OSD_ADDR[8];
HI_U32 CURLD_OSD_STRIDE[8];
HI_U32 reserved3[22];

HI_U32 VCTRL_ROI_CFG0;
HI_U32 VCTRL_ROI_CFG1;
HI_U32 VCTRL_ROI_CFG2;
HI_U32 VCTRL_ROI_SIZE[8];
HI_U32 VCTRL_ROI_START[8];
HI_U32 reserved4[1];

HI_U32 QPG_QP_LAMBDA_CTRL_REG0[16];
HI_U32 reserved5[28];
HI_U32 QPG_QP_LAMBDA_CTRL_REG1[64];

/***********************************************************************************/
/***************Lamda，IP、B帧可能不同，所以可能是经常改变的寄存器,以后无需增加***********/
/****************行级码控QP和Bits,开启功能后会每帧配置,总共768字节**********************/
/***********************************************************************************/
//0x0480 ~ 0x0780

/***********************************************************************************/
/************************量化表，不经常改变的寄存器，768字节***************************/
/***********************************************************************************/
//0x0780 ~ 0x0a80
/*
HI_U32 IntraLuma4x4	[4];
HI_U32 IntraChroma4x4	[4];
HI_U32 InterLuma4x4	[4];
HI_U32 InterChroma4x4	[4];
HI_U32 IntraLuma8x8	[16];
HI_U32 IntraChroma8x8	[16];
HI_U32 InterLuma8x8	[16];
HI_U32 InterChroma8x8	[16];
HI_U32 IntraLuma16	[16];
HI_U32 IntraChroma16	[16];
HI_U32 InterLuma16	[16];
HI_U32 InterChroma16	[16];
HI_U32 IntraLuma32	[16];
HI_U32 InterLuma32	[16];
HI_U32 TQITQ_DC0;
HI_U32 TQITQ_DC1;
HI_U32 reserved6[14];
*/
/***********************************************************************************/
/************************算法相关系数，256字节，调节完毕后会固化************************/
/***********************************************************************************/
//0x0a80 ~ 0x0b80
HI_U32 VCTRL_NORM_TR32X32_COEFF_DENOISE;
HI_U32 VCTRL_NORM_TR16X16_COEFF_DENOISE;
HI_U32 VCTRL_SKIN_TR32X32_COEFF_DENOISE;
HI_U32 VCTRL_SKIN_TR16X16_COEFF_DENOISE;

HI_U32 VCTRL_STATIC_TR32X32_COEFF_DENOISE;
HI_U32 VCTRL_STATIC_TR16X16_COEFF_DENOISE;
HI_U32 VCTRL_HEDGE_TR32X32_COEFF_DENOISE;
HI_U32 VCTRL_HEDGE_TR16X16_COEFF_DENOISE;

HI_U32 VCTRL_NORMAL_INTRA_RDO_FACTOR;
HI_U32 VCTRL_NORMAL_MRG_RDO_FACTOR;
HI_U32 VCTRL_NORMAL_FME_RDO_FACTOR;
HI_U32 VCTRL_SKIN_REGION_INTRA_RDO_FACTOR;

HI_U32 VCTRL_SKIN_REGION_MRG_RDO_FACTOR;
HI_U32 VCTRL_SKIN_REGION_FME_RDO_FACTOR;
HI_U32 VCTRL_HIGH_EDGE_INTRA_RDO_FACTOR;
HI_U32 VCTRL_HIGH_EDGE_MRG_RDO_FACTOR;

HI_U32 VCTRL_HIGH_EDGE_FME_RDO_FACTOR;
HI_U32 VCTRL_EDGE_MOV_COEFF_DENOISE   ;
HI_U32 VCTRL_SOBEL_STRONG_COEFF_DENOISE;
HI_U32 VCTRL_SOBEL_WEAK_COEFF_DENOISE;

HI_U32 VCTRL_NORM_COEFF_DENOISE;
HI_U32 VCTRL_SKIN_COEFF_DENOISE;
HI_U32 VCTRL_STILL_COEFF_DENOISE;
HI_U32 VCTRL_EDGE_COEFF_DENOISE;

HI_U32 VCTRL_OFFSET_COEFF_DENOISE;
HI_U32 VCTRL_NEW_OFFSET_COEFF_DENOISE;
HI_U32 VCTRL_EDGEMOV_TR32X32_COEFF_DENOISE;
HI_U32 VCTRL_EDGEMOV_TR16X16_COEFF_DENOISE;

HI_U32 VCTRL_SOBELSTR_TR32X32_COEFF_DENOISE;
HI_U32 VCTRL_SOBELSTR_TR16X16_COEFF_DENOISE;
HI_U32 VCTRL_SOBELWEAK_TR32X32_COEFF_DENOISE;
HI_U32 VCTRL_SOBELWEAK_TR16X16_COEFF_DENOISE;
	
HI_U32 VCTRL_SEL_INTRA_RDO_FACTOR;           //下面++++++++++新增加进来的寄存器
HI_U32 VCTRL_SEL_INTER_RDO_FACTOR_0;
HI_U32 VCTRL_SEL_INTER_RDO_FACTOR_1;
HI_U32 VCTRL_SEL_INTER_RDO_FACTOR_2;

HI_U32 VCTRL_SEL_LAYER_RDO_FACTOR;
HI_U32 VCTRL_NORM_ENG_DENOISE;
HI_U32 VCTRL_SKIN_ENG_DENOISE;
HI_U32 VCTRL_STILL_ENG_DENOISE;

HI_U32 VCTRL_STREDGE_ENG_DENOISE;
HI_U32 VCTRL_EDGEMOV_ENG_DENOISE;
HI_U32 VCTRL_SOBELSTR_ENG_DENOISE;
HI_U32 VCTRL_SOBELWK_ENG_DENOISE;

HI_U32 FME_BIAS_COST0;
HI_U32 FME_BIAS_COST1;
HI_U32 MRG_BIAS_COST0;
HI_U32 MRG_BIAS_COST1;
            
HI_U32 SEL_INTRA_ABS_OFFSET0;
HI_U32 SEL_INTRA_ABS_OFFSET1;
HI_U32 SEL_INTER_ABS_OFFSET0;
HI_U32 SEL_INTER_ABS_OFFSET1;

HI_U32 SEL_OFFSET_STRENGTH;
HI_U32 SEL_CU32_DC_AC_TH_OFFSET;
HI_U32 SEL_CU32_QP_TH;
HI_U32 SEL_RES_DC_AC_THR;

HI_U32 VCPI_PINTRA_THRESH0;
HI_U32 VCPI_PINTRA_THRESH1;
HI_U32 VCPI_PINTRA_THRESH2;
HI_U32 reserved6[5];

/***********************************************************************************/
/************************算法相关系数，384字节，调节完毕后会固化************************/
/***********************************************************************************/
//0x0b80 ~ 0x0d00
HI_U32 PME_LARGE_MOVE_THR;
HI_U32 PME_INTER_STRONG_EDGE;
HI_U32 PME_NEW_COST;
HI_U32 PME_COST_OFFSET;

HI_U32 PME_WINDOW_SIZE[1][4];

HI_U32 PME_SW_EN;//req11:arlen=16;addr=32'd2944
HI_U32 PME_SW_TR0;
HI_U32 PME_SW_TR1;
HI_U32 PME_SW_TR2;

HI_U32 PME_SKIP_PRE;
HI_U32 PME_TR_WEIGHTX;
HI_U32 PME_TR_WEIGHTY;
HI_U32 PME_SR_WEIGHT;

HI_U32 PME_INTRABLK_DET;
HI_U32 PME_INTRABLK_DET_THR;
HI_U32 reserved7[2];

HI_U32 PME_SKIN_THR;
HI_U32 PME_INTRA_LOWPOW;
HI_U32 PME_IBLK_COST_THR;
HI_U32 PME_HIGH_EDGE;

HI_U32 reserved8[12];

}S_VEDU_DDRCFG_TYPE;


#endif /* __C_UNION_DEFINE_VCPI_H__ */

