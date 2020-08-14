#ifndef __HAL_VENC_H__
#define __HAL_VENC_H__

#include "hi_type.h"
#include "drv_venc_ratecontrol.h"

#ifdef __VENC_S40V200_CONFIG__
#include "drv_venc_reg_r006.h"
#endif

#ifdef __VENC_3716CV200_CONFIG__
#include "drv_venc_reg_r008.h"
#endif

#if ((defined __VENC_S5V100_CONFIG__)|(defined __VENC_98M_CONFIG__))
#include "drv_venc_reg_s5v100.h"
#endif

#if (defined __VENC_98CV200_CONFIG__)
#include "drv_venc_reg_98cv200.h"
#endif

#if (defined __VENC_98MV200_CONFIG__)
#include "drv_venc_reg_98mv200.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef __VENC_DPT_ONLY__
#define VEDU_REG_BASE_ADDR 0xff360000
#define VEDU_IRQ_ID 133
#else
#define VEDU_REG_BASE_ADDR 0xf8c80000
#define VEDU_IRQ_ID 133						/* 101+32 */
#endif

#ifdef HI_SMMU_SUPPORT
#define VEDU_SMMU_NS_IRQ_ID    133    /*101 + 32*/
#define VEDU_SMMU_S_IRQ_ID     133   /* 99 + 32*/
#endif

enum
{
	VEDU_H264	= 0,
	VEDU_JPGE   = 1,
	VEDU_H263	= 2,
	VEDU_MPEG4  = 3,
	VEDU_H265	= 4,
};

typedef struct
{
	HI_U8* pu8StartVirAddr;    /*VirAddr*/
	HI_U32 u32StartPhyAddr;    /*PhyAddr or SmmuAddr*/
	HI_U32 u32Size;
} VENC_BUFFER_S;

typedef struct
{
    HI_U32   Enable [8];    /* only used at H264  */
    HI_U32   AbsQpEn[8];
    HI_S32   Qp     [8];    /* -26 ~ 25 or 0 ~ 51 */
    HI_U32   Width  [8];    /* size in MB */
    HI_U32   Height [8];    /* size in MB */
    HI_U32   StartX [8];    /* size in MB */
    HI_U32   StartY [8];    /* size in MB */
    HI_U32   Keep   [8];

} VeduEfl_RoiAttr_S;

typedef struct
{
  HI_U32 cu_qp_delta_thresh[16];   
  HI_U32 qp_delta_level[16];
  HI_U32 qp_madi_switch_thr;

  HI_U32 large_move_qp_delta;
  HI_U32 strong_edge_qp_delta; 
  HI_U32 strong_edge_move_qp_delta;
  HI_U32 skin_qp_delta;
  HI_U32 intra_det_qp_delta;
  
  HI_U32 large_move_max_qp ;   
  HI_U32 strong_edge_max_qp ;
  HI_U32 move_edge_max_qp;
  HI_U32 skin_max_qp   ;
  HI_U32 intra_det_max_qp;


  HI_U32  h264_smooth_qp_delta ;
  HI_U32  h264_smooth_qp_delta1 ; 
  HI_U32  h264_smooth_max_qp ;
  HI_U32  cu32_delta_low; 
  HI_U32  cu32_delta_high; 
  
  HI_U32 qp_delta;   
  HI_U32 lambda[80];
  
  HI_U32 qp_restrain_madi_thr ;
  HI_U32 qp_restrain_mode ;
  HI_U32 qp_restrain_en ;
  HI_U32 qp_restrain_delta_blk32 ; 
  HI_U32 lambda_qp_offset;
  
  HI_U32 low_min_sad_en;
  HI_U32 high_min_sad_en;
  
  HI_U32 rc_h264_smooth_mb_en ;
  HI_U32 rc_low_luma_en ;
  HI_U32 rc_cu_qp_en;
  HI_U32 rc_row_qp_en;
  
  HI_U32 rc_cu_madi_en;
  HI_U32 rc_qp_restrain_en;
  HI_U32 move_scene_en;
  HI_U32 vcpi_bfg_roi_qp_en;
  HI_U32 strong_edge_move_en; 
  HI_U32 intra_det_qp_en;
    
  HI_U32 min_sad_thresh_low;
  HI_U32 min_sad_thresh_high;

  HI_U32 min_sad_level;
  HI_U32 low_min_sad_mode;
  HI_U32 high_min_sad_mode;
  HI_U32 qp_restrain_delta_blk16;
  HI_U32 min_sad_madi_en;
  HI_U32 min_sad_qp_restrain_en;
  HI_U32 rdo_lambda_choose_mode;
  HI_U32 lambda_inter_stredge_en;

  HI_U32 rd_smooth_mb_en;
  HI_U32 rd_min_sad_flag_en;
  HI_U32 wr_min_sad_flag_en;
  HI_U32  prev_min_sad_en;
  HI_U32  qp_detlta_size_cu64;
  HI_U32  smart_get_cu64_qp_mode;
  HI_U32  smart_get_cu32_qp_mode;
  HI_U32  smart_cu_level_qp_mode;
  HI_U32  rc_smart_abs_qp_mode;
  HI_U32  skin_min_qp;
  HI_U32  stredge_move_min_qp;
  HI_U32  intra_det_min_qp;
  HI_U32  large_move_min_qp;
  HI_U32  stredge_min_qp;
  HI_U32  lowluma_min_qp;
  HI_U32  smooth_min_qp;
  HI_U32  row_target_bits;

}VeduEfl_QpgAttr_S;

typedef struct
{
  HI_U32 l0_psw_adapt_en;

  HI_U32 l0_psw_thr0;
  HI_U32 l0_psw_thr1;
  HI_U32 l0_psw_thr2;

  HI_U32 tr_weightx_2;   
  HI_U32 tr_weightx_1;   
  HI_U32 tr_weightx_0;   
  HI_U32 tr_weighty_2;   
  HI_U32 tr_weighty_1;   
  HI_U32 tr_weighty_0;   
  HI_U32 sr_weight_2;    
  HI_U32 sr_weight_1;    
  HI_U32 sr_weight_0;

  HI_U32 skin_num;  
  HI_U32 skin_v_min_thr; 
  HI_U32 skin_v_max_thr;
  HI_U32 skin_u_min_thr; 
  HI_U32 skin_u_max_thr;

  HI_U32 pme_intra16_madi_thr;
  HI_U32 pme_intra32_madi_thr;
  
  HI_U32 pme_intra_lowpow_en;
  HI_U32 pme_iblk_pre_cost_thr_h264;
  HI_U32 pme_intrablk_det_cost_thr1;
  HI_U32 still_scene_thr;   
  HI_U32 high_edge_cnt;   
  HI_U32 high_edge_thr;   
 
  HI_U32 move_sad_thr ;
  HI_U32 move_scene_thr ;
  HI_U32 interstrongedge_madi_thr ;
  HI_U32 interdiff_max_min_madi_times ;
  HI_U32 interdiff_max_min_madi_abs ;
  HI_U32 cost_lamda_en ;
  HI_U32 mvp3median_en ;
  HI_U32 new_cost_en;   
  HI_U32 cost_lamda1 ;
  HI_U32 cost_lamda0 ;

  HI_U32 l0_win0_width;
  HI_U32 l0_win0_height;
  HI_U32 l0_win1_width;
  HI_U32 l0_win1_height;
  HI_U32 l0_win2_width;
  HI_U32 l0_win2_height;
  HI_U32 l0_win3_width;
  HI_U32 l0_win3_height;

  HI_U32 l0_cost_offset ;
  HI_U32 pme_intrablk_det_cost_thr0   ;
  HI_U32 pme_intrablk_det_mvx_thr     ;
  HI_U32 pme_intrablk_det_mvy_thr     ;
  HI_U32 pme_intrablk_det_mv_dif_thr0 ;
  HI_U32 pme_intrablk_det_mv_dif_thr1 ;

  HI_U32 pme_safe_line;
  HI_U32 pme_safe_line_val;

  HI_U32 pme_iblk_refresh_start_num;
  HI_U32 pme_iblk_refresh_num;
  HI_U32 pme_high_luma_thr;
  HI_U32 smooth_madi_thr;
  HI_U32 pme_inter_first;
  HI_U32 low_luma_thr     ;
  HI_U32 low_luma_madi_thr  ;
  HI_U32 madi_dif_thr      ;
  HI_U32 cur_madi_dif_thr  ;
  HI_U32 min_sad_thr_gain  ;
  HI_U32 min_sad_thr_offset;

}VeduEfl_PmeAttr_S;

typedef struct
{
    HI_U32 img_improve_en;
    HI_U32 static_en;
    HI_U32 highedge_en;
    HI_U32 skin_en;

    HI_U32 norm32_tr1_denois_max_num;
    HI_U32 norm32_coeff_protect_num;
    HI_U32 norm16_tr1_denois_max_num;
    HI_U32 norm16_coeff_protect_num;
    HI_U32 skin32_tr1_denois_max_num;
    HI_U32 skin32_coeff_protect_num;
    HI_U32 skin16_tr1_denois_max_num;
    HI_U32 skin16_coeff_protect_num;
    HI_U32 static32_tr1_denois_max_num;
    HI_U32 static32_coeff_protect_num;
    HI_U32 static16_tr1_denois_max_num;
    HI_U32 static16_coeff_protect_num;
    HI_U32 hedge32_tr1_denois_max_num;
    HI_U32 hedge32_coeff_protect_num;
    HI_U32 hedge16_tr1_denois_max_num;
    HI_U32 hedge16_coeff_protect_num;

    HI_U32 norm_intra_cu32_rdcost_offset;
    HI_U32 norm_intra_cu16_rdcost_offset;
    HI_U32 norm_intra_cu8_rdcost_offset;
    HI_U32 norm_intra_cu4_rdcost_offset;

    HI_U32 strmov_intra_cu32_rdcost_offset;
    HI_U32 strmov_intra_cu16_rdcost_offset;
    HI_U32 strmov_intra_cu8_rdcost_offset;
    HI_U32 strmov_intra_cu4_rdcost_offset;

    HI_U32 norm_mrg_cu64_rdcost_offset;
    HI_U32 norm_mrg_cu32_rdcost_offset;
    HI_U32 norm_mrg_cu16_rdcost_offset;
    HI_U32 norm_mrg_cu8_rdcost_offset;
    HI_U32 strmov_mrg_cu64_rdcost_offset;
    HI_U32 strmov_mrg_cu32_rdcost_offset;
    HI_U32 strmov_mrg_cu16_rdcost_offset;
    HI_U32 strmov_mrg_cu8_rdcost_offset;


    HI_U32 norm_fme_cu64_rdcost_offset;
    HI_U32 norm_fme_cu32_rdcost_offset;
    HI_U32 norm_fme_cu16_rdcost_offset;
    HI_U32 norm_fme_cu8_rdcost_offset;
    HI_U32 strmov_fme_cu64_rdcost_offset;
    HI_U32 strmov_fme_cu32_rdcost_offset;
    HI_U32 strmov_fme_cu16_rdcost_offset;
    HI_U32 strmov_fme_cu8_rdcost_offset;

    HI_U32 skin_intra_cu32_rdcost_offset;
    HI_U32 skin_intra_cu16_rdcost_offset;
    HI_U32 skin_intra_cu8_rdcost_offset ;
    HI_U32 skin_intra_cu4_rdcost_offset ;
    HI_U32 sobel_str_intra_cu32_rdcost_offset;
    HI_U32 sobel_str_intra_cu16_rdcost_offset;
    HI_U32 sobel_str_intra_cu8_rdcost_offset ;
    HI_U32 sobel_str_intra_cu4_rdcost_offset ;

    HI_U32 skin_mrg_cu64_rdcost_offset;
    HI_U32 skin_mrg_cu32_rdcost_offset;
    HI_U32 skin_mrg_cu16_rdcost_offset;
    HI_U32 skin_mrg_cu8_rdcost_offset ;
    HI_U32 sobel_str_mrg_cu64_rdcost_offset;
    HI_U32 sobel_str_mrg_cu32_rdcost_offset;
    HI_U32 sobel_str_mrg_cu16_rdcost_offset;
    HI_U32 sobel_str_mrg_cu8_rdcost_offset ;

    HI_U32 skin_fme_cu64_rdcost_offset;
    HI_U32 skin_fme_cu32_rdcost_offset;
    HI_U32 skin_fme_cu16_rdcost_offset;
    HI_U32 skin_fme_cu8_rdcost_offset ;
    HI_U32 sobel_str_fme_cu64_rdcost_offset;
    HI_U32 sobel_str_fme_cu32_rdcost_offset;
    HI_U32 sobel_str_fme_cu16_rdcost_offset;
    HI_U32 sobel_str_fme_cu8_rdcost_offset ;

    HI_U32 hedge_intra_cu32_rdcost_offset; 
    HI_U32 hedge_intra_cu16_rdcost_offset; 
    HI_U32 hedge_intra_cu8_rdcost_offset;   
    HI_U32 hedge_intra_cu4_rdcost_offset; 
    HI_U32 sobel_tex_intra_cu32_rdcost_offset; 
    HI_U32 sobel_tex_intra_cu16_rdcost_offset; 
    HI_U32 sobel_tex_intra_cu8_rdcost_offset;   
    HI_U32 sobel_tex_intra_cu4_rdcost_offset;  

    HI_U32 hedge_mrg_cu64_rdcost_offset;  
    HI_U32 hedge_mrg_cu32_rdcost_offset;  
    HI_U32 hedge_mrg_cu16_rdcost_offset;  
    HI_U32 hedge_mrg_cu8_rdcost_offset;    
    HI_U32 sobel_tex_mrg_cu64_rdcost_offset;  
    HI_U32 sobel_tex_mrg_cu32_rdcost_offset;  
    HI_U32 sobel_tex_mrg_cu16_rdcost_offset;  
    HI_U32 sobel_tex_mrg_cu8_rdcost_offset;  

    HI_U32 hedge_fme_cu64_rdcost_offset;  
    HI_U32 hedge_fme_cu32_rdcost_offset;  
    HI_U32 hedge_fme_cu16_rdcost_offset;  
    HI_U32 hedge_fme_cu8_rdcost_offset;
    HI_U32 sobel_tex_fme_cu64_rdcost_offset;  
    HI_U32 sobel_tex_fme_cu32_rdcost_offset;  
    HI_U32 sobel_tex_fme_cu16_rdcost_offset;  
    HI_U32 sobel_tex_fme_cu8_rdcost_offset;   

    HI_U32 roundingedgemovmechanism;
    HI_U32 roundingedgemovdegreethresh;
    HI_U32 roundingedgemovforcezeroresidthresh;
    HI_U32 roundingedgemovac32sum;
    HI_U32 roundingedgemovac16sum;
    HI_U32 roundingedgemovlowfreqacblk32;
    HI_U32 roundingedgemovlowfreqacblk16;
    HI_U32 roundingsobelstrmechanism;
    HI_U32 roundingsobelstrdegreethresh ;
    HI_U32 roundingsobelstrforcezeroresidthresh;
    HI_U32 roundingsobelstrac32sum;
    HI_U32 roundingsobelstrac16sum;
    HI_U32 roundingsobelstrlowfreqacblk32;
    HI_U32 roundingsobelstrlowfreqacblk16;
    HI_U32 roundingsobelweakmechanism;
    HI_U32 roundingsobelweakdegreethresh;
    HI_U32 roundingsobelweakforcezeroresidthresh;
    HI_U32 roundingsobelweakac32sum;
    HI_U32 roundingsobelweakac16sum;
    HI_U32 roundingsobelweaklowfreqacblk32;
    HI_U32 roundingsobelweaklowfreqacblk16;

    HI_U32 roundingoffset16x16;
    HI_U32 roundingoffset32x32;
    HI_U32 roundingskinoffset16x16;
    HI_U32 roundingskinoffset32x32;
    HI_U32 roundingstilloffset16x16;
    HI_U32 roundingstilloffset32x32;
    HI_U32 roundingedgeoffset16x16;
    HI_U32 roundingedgeoffset32x32;
    HI_U32 roundingedgemovoffset16x16;
    HI_U32 roundingedgemovoffset32x32;
    HI_U32 roundingsobelstroffset16x16;
    HI_U32 roundingsobelstroffset32x32;
    HI_U32 roundingsobelweakoffset16x16;
    HI_U32 roundingsobelweakoffset32x32;
    HI_U32 edgemov32_coeff_protect_num;
    HI_U32 edgemov32_tr1_denois_max_num;
    HI_U32 edgemov16_coeff_protect_num;
    HI_U32 edgemov16_tr1_denois_max_num;
    HI_U32 sobelstr32_coeff_protect_num;
    HI_U32 sobelstr32_tr1_denois_max_num;
    HI_U32 sobelstr16_coeff_protect_num;
    HI_U32 sobelstr16_tr1_denois_max_num;
    HI_U32 sobelweak32_coeff_protect_num;
    HI_U32 sobelweak32_tr1_denois_max_num;
    HI_U32 sobelweak16_coeff_protect_num;
    HI_U32 sobelweak16_tr1_denois_max_num;

    HI_U32 norm_isolate_ac_enable;
    HI_U32 norm_force_zero_cnt;
    HI_U32 norm_engsum_32;
    HI_U32 norm_engcnt_32;
    HI_U32 norm_engsum_16;
    HI_U32 norm_engcnt_16;
    HI_U32 skin_isolate_ac_enable;
    HI_U32 skin_force_zero_cnt;
    HI_U32 skin_engsum_32;
    HI_U32 skin_engcnt_32;
    HI_U32 skin_engsum_16;
    HI_U32 skin_engcnt_16;
    HI_U32 still_isolate_ac_enable;
    HI_U32 still_force_zero_cnt;
    HI_U32 still_engsum_32;
    HI_U32 still_engcnt_32;
    HI_U32 still_engsum_16;
    HI_U32 still_engcnt_16;
    HI_U32 stredge_isolate_ac_enable ;
    HI_U32 stredge_force_zero_cnt;
    HI_U32 stredge_engsum_32;
    HI_U32 stredge_engcnt_32;
    HI_U32 stredge_engsum_16;
    HI_U32 stredge_engcnt_16;
    HI_U32 edgemov_isolate_ac_enable;
    HI_U32 edgemov_force_zero_cnt;
    HI_U32 edgemov_engsum_32;
    HI_U32 edgemov_engcnt_32;
    HI_U32 edgemov_engsum_16;
    HI_U32 edgemov_engcnt_16;
    HI_U32 sobelstr_isolate_ac_enable;
    HI_U32 sobelstr_force_zero_cnt;
    HI_U32 sobelstr_engsum_32;
    HI_U32 sobelstr_engcnt_32;
    HI_U32 sobelstr_engsum_16;
    HI_U32 sobelstr_engcnt_16;
    HI_U32 sobelwk_isolate_ac_enable;
    HI_U32 sobelwk_force_zero_cnt;
    HI_U32 sobelwk_engsum_32;
    HI_U32 sobelwk_engcnt_32;
    HI_U32 sobelwk_engsum_16;
    HI_U32 sobelwk_engcnt_16;

    HI_U32  roundingMechanism   ;
    HI_U32  roundingDegreeThresh;
    HI_U32  roundingForceZeroResidThresh;
    HI_U32  roundingAC32sum;
    HI_U32  roundingAC16sum ;
    HI_U32  roundingLowFreqACBlk32;
    HI_U32  roundingLowFreqACBlk16;

    HI_U32  roundingSkinMechanism;
    HI_U32  roundingSkinDegreeThresh;
    HI_U32  roundingSkinForceZeroResidThresh;
    HI_U32  roundingSkinAC32sum;
    HI_U32  roundingSkinAC16sum;
    HI_U32  roundingSkinLowFreqACBlk32;
    HI_U32  roundingSkinLowFreqACBlk16;

    HI_U32  roundingStillMechanism;
    HI_U32  roundingStillDegreeThresh;
    HI_U32  roundingStillForceZeroResidThresh;
    HI_U32  roundingStillAC32sum;
    HI_U32  roundingStillAC16sum;
    HI_U32  roundingStillLowFreqACBlk32;
    HI_U32  roundingStillLowFreqACBlk16;

    HI_U32  roundingEdgeMechanism;
    HI_U32  roundingEdgeDegreeThresh;
    HI_U32  roundingEdgeForceZeroResidThresh;
    HI_U32  roundingEdgeAC32sum;
    HI_U32  roundingEdgeAC16sum;
    HI_U32  roundingEdgeLowFreqACBlk32;
    HI_U32  roundingEdgeLowFreqACBlk16;

    HI_U32 norm_sel_cu8_rd_offset;
    HI_U32 skin_sel_cu8_rd_offset;
    HI_U32 hedge_sel_cu8_rd_offset;
    HI_U32 strmov_sel_cu8_rd_offset;
    HI_U32 sobelstr_sel_cu8_rd_offset;
    HI_U32 sobeltex_sel_cu8_rd_offset;

    HI_U32 skin_inter_cu_rdcost_offset;
    HI_U32 hedge_inter_cu_rdcost_offset;
    HI_U32 skin_layer_rdcost_offset;

    HI_U32 sobelstr_inter_cu_rdcost_offset;
    HI_U32 sobeltex_inter_cu_rdcost_offset;
    HI_U32 hedge_layer_rdcost_offset;

    HI_U32 strmov_inter_cu_rdcost_offset;
    HI_U32 norm_inter_cu_rdcost_offset;
    HI_U32 strmov_layer_rdcost_offset;

    HI_U32 sobelstr_layer_rdcost_offset;
    HI_U32 sobeltex_layer_rdcost_offset;
    HI_U32 norm_layer_rdcost_offset;

    HI_U32 sel_intra4_8_abs_offset;
    HI_U32 sel_intra16_abs_offset;
    HI_U32 sel_inter8_16_abs_offset;
    HI_U32 sel_inter32_abs_offset;
    HI_U32 sel_layer32_abs_offset;

} VeduEfl_ImgImproveAttr_S;

typedef struct
{
    HI_U32 vcpi_ipcm_en;
    HI_U32 vcpi_intra_cu_en;
    HI_U32 vcpi_trans_mode;
    HI_U32 vcpi_entropy_mode;

    HI_U32 vcpi_sao_chroma;
    HI_U32 vcpi_sao_luma;

    HI_U32 max_num_mergecand;    
    HI_U32 tmv_en; 

    HI_U32 pmv_poc[6];

}VeduEfl_BaseAttr_S;

typedef struct
{
    HI_U8  SlcSplitMod;       /* 0 or 1, byte or mb line ,just select 1*/
    VENC_BUFFER_S DdrRegCfgBuf;

    HI_U32* pRegBase;
    /*para*/
    HI_U32 Protocol;
    HI_U16 SlcSplitEn;
    HI_U32 SplitSize;
    HI_U32 StrmBufAddr;
    HI_U32 StrmBufRpAddr; /* phy addr for hardware */
    HI_U32 StrmBufWpAddr;
    HI_U32 StrmBufSize;
    /* frame buffer parameter */
    HI_U32 SStrideY;
    HI_U32 SStrideC;
    HI_U32 YuvStoreType;           /* 0, semiplannar; 1, package; 2,planer */
    HI_U32 PackageSel;

    HI_U32 SrcYAddr;
    HI_U32 SrcCAddr;
    HI_U32 SrcVAddr;                  //just for input of planner

    HI_U32 RcnYAddr[2];
    HI_U32 RcnCAddr[2];
    HI_U32 RcnYAddr_head[2];
    HI_U32 RcnCAddr_head[2];
    HI_U32 TmvAddr[2];
    HI_U32 PmeAddr[2];
    HI_U32 PmeInfoAddr[3];

    HI_U32 SlcHdrBits;         /* 8bit_0 | mark | reorder | slchdr */
    HI_U32 SlcHdrBits_i;//not use
    HI_U32 SlcHdrStream_i [4];
    HI_U32 ReorderStream_i[2];
    HI_U32 MarkingStream_i[2];

    HI_U32 CABAC_SLCHDR_PART2_SEG[8];
    HI_U32 CABAC_SLCHDR_PART2_SEG_I[8];//nt
    HI_U32 slchdr_part1;
    HI_U32 slchdr_part1_i;//nt
    HI_U32 slchdr_size_part1;
    HI_U32 slchdr_size_part1_i; //nt
    HI_U32 slchdr_size_part2;
    HI_U32 slchdr_size_part2_i; //nt

    HI_U32  PTS0;
    HI_U32  PTS1;

#if 1

    HI_U32  RStrideY;
    HI_U32  RStrideC;
    HI_U32  RHStrideY;
    HI_U32  RHStrideC;
#endif

    HI_U32 SlcHdrStream [4];
    HI_U32 ReorderStream[2];
    HI_U32 MarkingStream[2];

    HI_S32 vcpi_srcy_bypass;
    HI_S32 vcpi_srcv_bypass;
    HI_S32 vcpi_refy_bypass;
    HI_S32 vcpi_refc_bypass;
    HI_S32 vcpi_strm_bypass;

#ifdef   __VENC_98MV200_CONFIG__  //add for 98mv200
    HI_S32 vcpi_srcu_bypass;           //read reg
    HI_S32 vcpi_pmeinfold1_bypass;
    HI_S32 vcpi_pmeinfold0_bypass;
    HI_S32 vcpi_refch_bypass;
    HI_S32 vcpi_refyh_bypass;
    HI_S32 vcpi_pmeld_bypass;
    HI_S32 vcpi_nbild_bypass;

    HI_S32 vcpi_pmeinfost_bypass;   //write reg
    HI_S32 vcpi_recc_bypass;
    HI_S32 vcpi_recy_bypass;
    HI_S32 vcpi_recch_bypass;
    HI_S32 vcpi_recyh_bypass;
    HI_S32 vcpi_pmest_bypass;
    HI_S32 vcpi_nbist_bypass;
#endif
    HI_S32 glb_bypass;
    //for smmu
    HI_S32 int_en;
    HI_U32 tunlReadIntvl;
    HI_U32 tunlcellAddr;
    HI_S32  ClkGateEn;
    HI_S32  MemClkGateEn;
    HI_S32  TimeOutEn;
    HI_S32  PtBitsEn;
    HI_U32  TimeOut;
    HI_S32  PtBits;

    //add for 98cv200
    HI_S32  wOutStdNum;
    HI_S32  rOutStdNum;
    //add end

    VeduEfl_RoiAttr_S RoiCfg;
#if (defined __VENC_98MV200_CONFIG__)
    VeduEfl_ImgImproveAttr_S    ImgImproveCfg;
    VeduEfl_QpgAttr_S           QpgCfg;
    VeduEfl_BaseAttr_S          BaseCfg;
    VeduEfl_PmeAttr_S           PmeCfg;
#endif

#ifdef HI_SMMU_SUPPORT
    HI_U32 u32SmmuPageBaseAddr;
    HI_U32 u32SmmuErrWriteAddr;
    HI_U32 u32SmmuErrReadAddr;
#endif

    HI_U32 VencEndOfPic;
    HI_U32 VencBufFull;
    HI_U32 VencPbitOverflow;
    HI_U32 MeanQP;

    HI_S32 ints_tlbmiss_stat;          //¡ã2¨¨??D??¡Á¡ä¨¬?
    HI_S32 ints_ptw_trans_stat;
    HI_S32 ints_tlbinvalid_rd_stat;
    HI_S32 ints_tlbinvalid_wr_stat;

    HI_S32 intns_tlbmiss_stat;       //¡¤?¡ã2¨¨??D??¡Á¡ä¨¬?
    HI_S32 intns_ptw_trans_stat;
    HI_S32 intns_tlbinvalid_rd_stat;
    HI_S32 intns_tlbinvalid_wr_stat;

    HI_S32 ints_tlbmiss_msk;           //¡ã2¨¨??¡ä?D???¨¢¡À?
    HI_S32 ints_ptw_trans_msk;
    HI_S32 ints_tlbinvalid_msk;

    HI_S32 ints_tlbmiss_raw;            //¡ã2¨¨??¡ä?D??
    HI_S32 ints_ptw_trans_raw;
    HI_S32 ints_tlbinvalid_raw;

    HI_S32 ints_tlbmiss_clr;           //¡ã2¨¨?2¨¤???D??
    HI_S32 ints_ptw_trans_clr;
    HI_S32 ints_tlbinvalid_clr;

    HI_S32 intns_tlbmiss_msk     ;      //¡¤?¡ã2¨¨??¡ä?D???¨¢¡À?
    HI_S32 intns_ptw_trans_msk   ;
    HI_S32 intns_tlbinvalid_msk  ;

    HI_S32 intns_tlbmiss_raw     ;      //¡¤? ¡ã2¨¨??¡ä?D??
    HI_S32 intns_ptw_trans_raw   ;
    HI_S32 intns_tlbinvalid_raw  ;

    HI_S32 intns_tlbmiss_clr     ;        //¡¤? ¡ã2¨¨?2¨¤???D??
    HI_S32 intns_ptw_trans_clr   ;
    HI_S32 intns_tlbinvalid_clr  ;

    HI_S32 fault_index_id        ;
    HI_S32 fault_strm_id         ;
    HI_S32 fault_index_id_wr     ;
    HI_S32 fault_str_id_wr       ;
    HI_S32 fault_index_id_rd     ;
    HI_S32 fault_str_id_rd       ;
    HI_S32 fault_tlbmiss_err     ;
    HI_S32 fault_tlbinvalid_err  ;
    HI_S32 fault_ptw             ;
    HI_S32 fault_tbu             ;
    HI_S32 fault_ptw_num         ;
    HI_S32 fault_tbu_num         ;

} Vedu_Hal_S;

typedef unsigned long VEDU_LOCK_FLAG;

HI_VOID VENC_HAL_ClrAllInt( S_VEDU_REGS_TYPE* pVeduReg );
HI_VOID VENC_HAL_DisableAllInt( S_VEDU_REGS_TYPE* pVeduReg );
HI_VOID VENC_HAL_ReadReg(Vedu_Hal_S *pstHal, VeduEfl_Rc_S *pstRc, VeduEfl_StatInfo_S *pstStat);
HI_VOID VENC_HAL_CfgReg(Vedu_Hal_S *pstHal, VeduEfl_Rc_S *pstRc);
HI_VOID VENC_HAL_ReadReg_Smmu(Vedu_Hal_S *pstHal);
HI_VOID VENC_HAL_DDRCfgReg(Vedu_Hal_S *pstHal, VeduEfl_Rc_S *pstRc);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif //__HAL_VENC_H__
