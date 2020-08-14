/*--------------------------------------------------------------------------------------------------------------------------*/
/*!!Warning: This is a key information asset of Huawei Tech Co.,Ltd                                                         */
/*CODEMARK:kOyQZYzjDpyGdBAEC2GaWinjiDDUykL9e8pckESWBbMVmSWkBuyJO01cTiy3TdzKxGk0oBQa
mSMf7J4FkTpfvzHyMxSEsfcbL/G0fFswaAahBMZqlWi0iYujNXpYhY1dEAFzsR1hztx27g8T
+4PchKl+YOrc1oUVdy2EOe6frxuwqdfcMsKI4Fwkt7iN7oz3Uffgjgr9PA8PN97D+OI2ZmI8
RayGpFVcZP2jwV373Es88+89ViuQe+xqRB+5/wxEME+GfkI/h2axUkiy/BNSVQ==*/
/*--------------------------------------------------------------------------------------------------------------------------*/
/***********************************************************************
*
* Copyright (c) 2014 HUAWEI - All Rights Reserved
*
* File     : $hevc_vfmw.h$
* Date     : $2014/07/01$
* Revision : $v1.0$
* Purpose  : Hevc decoder header file.
*
* Change History:
*
* Date                       Author                       Change
* ====                       ======                       ====== 
* 2014/03/01          l00214825 h00216273                Original.
* 2014/07/01                y00226912                 Modify to vfmw.
*
* Dependencies:
* Linux OS

************************************************************************/


#ifndef _HEVC_HEADER_
#define _HEVC_HEADER_

#ifdef __cplusplus
extern "C" {
#endif

#include "basedef.h"
#include "vfmw.h"
#include "public.h"
#include "bitstream.h"
#include "vdm_hal.h"


// define the profiles
#define H265_MAIN_PROFILE
#define HM11_SYNTAX                   (1)
#define HM9_NALU_TYPES                (1)

#define HEVC_NOTDEC                   (-2)

#define HEVC_STREAM_END_CODE1         (0x48535049)
#define HEVC_STREAM_END_CODE2         (0x43454E44)

// profile related constant
#if defined(H265_MAIN_PROFILE)
#define HEVC_MAX_VIDEO_PARAM_SET_ID   (16)
#define HEVC_MAX_SEQ_PARAM_SET_ID     (16)
#define HEVC_MAX_PIC_PARAM_SET_ID     (64)
#define HEVC_MAX_DPB_SIZE             (5)
#define HEVC_MAX_LONG_TERM_PICS       (5) 
#define HEVC_MAX_DELTA_POC            (5)
#endif

#define HEVC_MAX_DPB_PIC_BUF          (6)             //参考协议文档A.4
#define HEVC_MAX_SLICE_NUM            (512)
#define HEVC_MAX_ENTRY_POINT          (100)

#define HEVC_MAX_LSB_NUM              (33)
#define HEVC_MAX_TEMPLAYER            (8)             //max number of temporal layer
#define HEVC_MAX_CPB_CNT              (32)            //Upper bound of (cpb_cnt_minus1 + 1)
#define HEVC_MAX_NUM_PICS_IN_SOP      (1024)
#define HEVC_MAX_NESTING_NUM_LAYER    (64)
#define HEVC_MAX_CHROMA_FORMAT_IDC    (3)
#define HEVC_MAX_INT                  (2147483647)    //max value of signed 32-bit integer
#define HEVC_MAX_UINT                 (0xFFFFFFFFU)   //max. value of unsigned 32-bit integer

#define HEVC_FALSE		              (0)
#define HEVC_TRUE		              (1)

#define HEVC_STREAM_END_PPS_ID        (191)

#if HM11_SYNTAX
#define HEVC_MAX_VPS_OP_SETS_PLUS1    (1024)
#define HEVC_MAX_VPS_NUH_RESERVED_ZERO_LAYER_ID_PLUS1  (1)
#endif

//macros
#define HEVC_MAX_CU_DEPTH              (7)                           // log2(LCUSize)
#define HEVC_MAX_CU_SIZE               (1<<(HEVC_MAX_CU_DEPTH))      // maximum allowable size of CU
#define HEVC_MIN_PU_SIZE               (4)

#define HEVC_SCALING_LIST_NUM          (6)           //< list number for quantization matrix
#define HEVC_SCALING_LIST_DC           (16)          //< default DC value
#define HEVC_SCALING_LIST_START_VALUE  (8)           //< start value for dpcm mode
#define HEVC_MAX_MATRIX_COEF_NUM       (64)          //< max coefficient number for quantization matrix

#define HEVC_MAX_LIST_SIZE             (16+1)

#define HEVC_MAX_FRAME_STORE           (20)          // Maximun Frame Store Num
#define HEVC_MAX_PMV_STORE             (20)          // Maximun PMV   Store Num

#define HEVC_MAX_PIX_WIDTH             (4096)
#define HEVC_MAX_PIX_HEIGHT            (2304)

#define HEVC_SINT8_MAX_VAL	           (127)
#define HEVC_SINT8_MIN_VAL	           (-128)
#define HEVC_SINT13_MAX_VAL	           (4095)
#define HEVC_SINT13_MIN_VAL	           (-4096)

#define HEVC_I_SLICE                   (2)
#define HEVC_P_SLICE                   (1)
#define HEVC_B_SLICE                   (0)

#define HEVC_I_FRAME                   (0)
#define HEVC_P_FRAME                   (1)
#define HEVC_B_FRAME                   (2)

/********************* error information define *******************/
#define   HEVC_ERR_FS_OUT_AGAIN        (-3)
#define   HEVC_INIT_DECODER_ERR        (-1)
#define   HEVC_DEC_ERR                 (-1)
#define   HEVC_DEC_NORMAL              (0)
#define   HEVC_OK				       (0)
#define   HEVC_PROFILE_ERR             (-1
#define   HEVC_LEVEL_ERR               (-1)
#define   HEVC_VHB_MEM_ERR             (-5)
#define   HEVC_DPB_ERR                 (-3)
#define   HEVC_FS_ALLOC_ERR            (-4)
#define   HEVC_VHB_MEM_ERR             (-5)
#define   HEVC_OUT_FLAG                (-6)
#define   HEVC_APC_ERR                 (-7)
#define   HEVC_BS_OVERFLOW_ERR         (-8)
#define   HEVC_DEC_OCCUPY              (99)
#define   HEVC_VPS_ERR                 (-1)
#define   HEVC_SPS_ERR                 (-1)
#define   HEVC_SE_ERR                  (-1)
#define   HEVC_PPS_ERR                 (-1)
#define   HEVC_VLC_CODE_ERR            (-1)
#define   HEVC_FLC_ERR                 (-1)
#define   HEVC_IDR_ERR                 (-1)
#define   HEVC_SLICE_HEADER_ERR        (-1)
#define   HEVC_MMCO_NOT_EQUAL          (-1)
#define   HEVC_RECOVERPOINT_ERR        (-1)
#define   HEVC_REDUNDANTPIC_ERR        (-1)
#define   HEVC_PIC_INITIAL_ERR         (-1)
#define   HEVC_INITDPB_ERR             (-1)
#define   HEVC_OUTPUT_ERR              (-1)

#define   HEVC_PIC_DECODING            (1)
#define   HEVC_PIC_EMPTY               (0)

// MERGE
#define   MRG_MAX_NUM_CANDS            (5)

/*for image format clean bits*/
#define   HEVC_CLEAN_FRAME_TYPE        (~3)
#define   HEVC_CLEAN_VIDEO_FORMAT      (~(7<<5))
#define   HEVC_CLEAN_SOURCE_FORMAT     (~(3<<8))
#define   HEVC_CLEAN_FIELD_VALID       (~(3<<10))
#define   HEVC_CLEAN_TOP_FILED_FIRST   (~(3<<12))
#define   HEVC_CLEAN_ASPECT_RATIO      (~(7<<14))

/******************* some define of start contrl ******************/
/*小于这个值需要拼包，否则可能码流解析不完整*/
#define HEVC_GET_ONE_NALU_SIZE         (4*1024)
#define HEVC_START_FRAME               (0)

/************************ structure define ************************/
typedef struct
{
    /************** VAHB *************/
    SINT32     total_image_num;       // 总共分配的图像个数
    SINT32     total_pmvblk_num;      // 总共分配的PMV块个数
    
    PHYADDR    vahb_phy_addr;         // VAHB物理地址，1024对齐
    UINT8*     vahb_vir_addr;         // VAHB物理地址，1024对齐

    //sed_top_addr
    PHYADDR    sed_top_phy_addr;
    UINT8*     sed_top_vir_addr;

    //pmv_top_addr
    PHYADDR    pmv_top_phy_addr;
    UINT8*     pmv_top_vir_addr;

    //rcn_top_addr
    PHYADDR    rcn_top_phy_addr;
    UINT8*     rcn_top_vir_addr;
    
    //ppfd_addr
    PHYADDR    ppfd_phy_addr;
    UINT8*     ppfd_vir_addr;
    
    //cabac m/n
    PHYADDR    mn_phy_addr;           
    UINT8*     mn_vir_addr;

    //tile_segment_info_addr
    PHYADDR    tile_segment_info_phy_addr;
    UINT8*     tile_segment_info_vir_addr;

    //dblk_addr
    PHYADDR    dblk_left_phy_addr;
    UINT8*     dblk_left_vir_addr;
    PHYADDR    dblk_top_phy_addr;
    UINT8*     dblk_top_vir_addr;

    //sao_addr
    PHYADDR    sao_left_phy_addr;
    UINT8*     sao_left_vir_addr;
    PHYADDR    sao_top_phy_addr;
    UINT8*     sao_top_vir_addr;
    
    //up msg slot
    PHYADDR    upmsgslot_phy_addr;
    UINT8*     upmsgslot_vir_addr;
        
    //消息池
    PHYADDR    msgpool_phy_addr;
    UINT8*     msgpool_vir_addr;
    
    //图像存储空间
    UINT32     image_stride;
    PHYADDR    image_phy_addr[HEVC_MAX_FRAME_STORE];
    UINT16*    image_vir_addr[HEVC_MAX_FRAME_STORE ];
    
    //PMV存储空间
    UINT32     half_pmvblk_offset;
    PHYADDR    pmvblk_phy_addr[HEVC_MAX_PMV_STORE];
    UINT8*     pmvblk_vir_addr[HEVC_MAX_PMV_STORE];
    
} HEVC_MEMINFO;

typedef struct
{
    UINT8   general_tier_flag;
    UINT8   general_profile_compatibility_flag[32];
    UINT8   general_progressive_source_flag;
    UINT8   general_interlaced_source_flag;
    UINT8   general_non_packed_constraint_flag;
    UINT8   general_frame_only_constraint_flag;
    UINT8   sub_layer_profile_present_flag[6];
    UINT8   sub_layer_level_present_flag[6];
    UINT8   sub_layer_profile_compatibility_flag[6][32];
    UINT8   sub_layer_tier_flag[6];
    UINT8   sub_layer_progressive_source_flag[6];
    UINT8   sub_layer_interlaced_source_flag[6];
    UINT8   sub_layer_non_packed_constraint_flag[6];
    UINT8   sub_layer_frame_only_constraint_flag[6];
    SINT32  general_profile_idc;
    SINT32  general_profile_space;
    SINT32  general_reserved_zero_44bits;
    SINT32  general_level_idc;
    SINT32  sub_layer_profile_space[6];
    SINT32  sub_layer_profile_idc[6];
    SINT32  sub_layer_reserved_zero_44bits[6];
    SINT32  sub_layer_level_idc[6];
}HEVC_PROFILE_TIER_LEVEL_S;

/// coefficient scanning type used in ACS
typedef enum 
{
    SCAN_ZIGZAG = 0,      ///< typical zigzag scan
    SCAN_HOR,             ///< horizontal first scan
    SCAN_VER,              ///< vertical first scan
    SCAN_DIAG              ///< up-right diagonal scan
}HEVC_COEFF_SCAN_TYPE;

/// reference list index
typedef enum 
{
    REF_PIC_LIST_0 = 0,   ///< reference list 0
    REF_PIC_LIST_1 = 1,   ///< reference list 1
    REF_PIC_LIST_C = 2,   ///< combined reference list for uni-prediction in B-Slices
    REF_PIC_LIST_X = 100  ///< special mark
}HEVC_REF_PIC_LIST;

typedef enum 
{
    SCALING_LIST_4x4 = 0,
    SCALING_LIST_8x8,
    SCALING_LIST_16x16,
    SCALING_LIST_32x32,
    SCALING_LIST_SIZE_NUM
}HEVC_SCALING_LIST_SIZE;

typedef struct
{
	UINT8	 useTransformSkip;	 //!< transform skipping flag for setting default scaling matrix for 4x4
	UINT8	 useDefaultScalingMatrixFlag [SCALING_LIST_SIZE_NUM][HEVC_SCALING_LIST_NUM]; //!< UseDefaultScalingMatrixFlag
	UINT8	 scaling_list_pred_mode_flag[SCALING_LIST_SIZE_NUM][HEVC_SCALING_LIST_NUM];//!< flag for using default matrix
	SINT32   scaling_list_dc_coef_minus8;
	SINT32   scaling_list_delta_coef;
	SINT32   scalingListDC[SCALING_LIST_SIZE_NUM][HEVC_SCALING_LIST_NUM]; //!< the DC value of the matrix coefficient for 16x16
	UINT32   refMatrixId[SCALING_LIST_SIZE_NUM][HEVC_SCALING_LIST_NUM]; //!< RefMatrixID
	UINT32   predMatrixId[SCALING_LIST_SIZE_NUM][HEVC_SCALING_LIST_NUM]; //!< reference list index
	SINT32	 scalingListCoef[SCALING_LIST_SIZE_NUM][HEVC_SCALING_LIST_NUM][HEVC_MAX_MATRIX_COEF_NUM]; //!< quantization matrix
    UINT32   scaling_list_pred_matrix_id_delta[SCALING_LIST_SIZE_NUM][HEVC_SCALING_LIST_NUM];
}HEVC_SCALING_LIST;

typedef struct
{
    UINT32  inter_ref_pic_set_prediction_flag;
	UINT32  delta_idx;
	UINT32  delta_rps_sign;
	UINT32  abs_delta_rps;
	UINT32  num_negative_pics;
	UINT32  num_positive_pics;
	UINT32  num_of_pics;
	UINT32  numRefIdc;
	UINT32  num_of_longterm_pics;
	UINT32  refIdc[HEVC_MAX_NUM_REF_PICS + 1];
	UINT32  used_flag[HEVC_MAX_NUM_REF_PICS + 1];
    SINT32  delta_poc[HEVC_MAX_NUM_REF_PICS + 1];
    SINT32  poc[HEVC_MAX_NUM_REF_PICS];
}HEVC_SHORT_TERM_RPSET_S;

typedef struct
{
    UINT8   ref_pic_list_modification_flag_l0;
	UINT8   ref_pic_list_modification_flag_l1;
	UINT32  list_entry_l0[32];
	UINT32  list_entry_l1[32];
}HEVC_REF_PIC_LISTS_MODDIFICATION;

typedef struct
{
    SINT32 luma_log2_weight_denom;
	SINT32 delta_chroma_log2_weight_denom;
    SINT32 luma_weight_l0_flag[HEVC_MAX_NUM_REF_PICS];
    SINT32 chroma_weight_l0_flag[HEVC_MAX_NUM_REF_PICS];
    SINT32 delta_luma_weight_l0[HEVC_MAX_NUM_REF_PICS];
    SINT32 luma_offset_l0[HEVC_MAX_NUM_REF_PICS];
    SINT32 delta_chroma_weight_l0[HEVC_MAX_NUM_REF_PICS][2];
    SINT32 delta_chroma_offset_l0[HEVC_MAX_NUM_REF_PICS][2];

    //HEVC_B_SLICE
    SINT32 luma_weight_l1_flag[HEVC_MAX_NUM_REF_PICS];
    SINT32 chroma_weight_l1_flag[HEVC_MAX_NUM_REF_PICS];
    SINT32 delta_luma_weight_l1[HEVC_MAX_NUM_REF_PICS];
    SINT32 luma_offset_l1[HEVC_MAX_NUM_REF_PICS];
    SINT32 delta_chroma_weight_l1[HEVC_MAX_NUM_REF_PICS][2];
    SINT32 delta_chroma_offset_l1[HEVC_MAX_NUM_REF_PICS][2];

    // inferred value
    SINT32 chroma_log2_weight_denom;
    SINT32 LumaWeightL0[HEVC_MAX_NUM_REF_PICS];
    SINT32 ChromaWeightL0[HEVC_MAX_NUM_REF_PICS][2];
    SINT32 ChromaOffsetL0[HEVC_MAX_NUM_REF_PICS][2];
    SINT32 LumaWeightL1[HEVC_MAX_NUM_REF_PICS];
    SINT32 ChromaWeightL1[HEVC_MAX_NUM_REF_PICS][2];
    SINT32 ChromaOffsetL1[HEVC_MAX_NUM_REF_PICS][2];
}HEVC_PRED_WEIGHT_TABLE;

typedef struct 
{
    // Explicit weighted prediction parameters parsed in slice header,
    // or Implicit weighted prediction parameters (8 bits depth values).
    UINT8   bPresentFlag;
    UINT32  uiLog2WeightDenom;
    SINT32  iWeight;
    SINT32  iOffset;
    
    // Weighted prediction scaling values built from above parameters (bitdepth scaled):
    SINT32  w, o, offset, shift, round;
} HEVC_WPSCALINGPARAM;

typedef struct
{
    UINT8  fixed_pic_rate_general_flag;
    UINT8  fixed_pic_rate_within_cvs_flag;
    UINT8  low_delay_hrd_flag;
    UINT32 elemental_duration_in_tc_minus1;
    UINT32 cpb_cnt_minus1;
    UINT32 bit_rate_value_minus1[HEVC_MAX_CPB_CNT][2];
    UINT32 cpb_size_value_minus1[HEVC_MAX_CPB_CNT][2];
    UINT32 cpb_size_du_value_minus1[HEVC_MAX_CPB_CNT][2];
    UINT32 bit_rate_du_value_minus1[HEVC_MAX_CPB_CNT][2];
    UINT32 cbr_flag[HEVC_MAX_CPB_CNT][2];    
} HEVC_HRD_SUBLAYER_SET_S;

typedef struct
{
    UINT8  nal_hrd_parameters_present_flag;
    UINT8  vcl_hrd_parameters_present_flag;
    UINT8  sub_pic_hrd_params_present_flag;
    UINT8  sub_pic_cpb_params_in_pic_timing_sei_flag;
    UINT32 tick_divisor_minus2;
    UINT32 du_cpb_removal_delay_increment_length_minus1;
    UINT32 dpb_output_delay_du_length_minus1;
    UINT32 bit_rate_scale;
    UINT32 cpb_size_scale;
    UINT32 cpb_size_du_scale;
    UINT32 initial_cpb_removal_delay_length_minus1;
    UINT32 au_cpb_removal_delay_length_minus1;
    UINT32 dpb_output_delay_length_minus1;
    HEVC_HRD_SUBLAYER_SET_S hrd_sublayer[HEVC_MAX_TEMPLAYER];
}HEVC_HRD_PARAM_SET_S;

typedef struct
{
    UINT8  valid;
    UINT8  vps_temporal_id_nesting_flag;
    UINT8  is_refresh;
    UINT8  vps_extension_flag;
    UINT8  vps_extension_data_flag;
    UINT8  vps_poc_proportional_to_timing_flag;
    UINT8  vps_timing_info_present_flag;
    UINT8  vps_sub_layer_ordering_info_present_flag;
    
    SINT32 video_parameter_set_id;
    SINT32 vps_reserved_three_2bits;
    SINT32 vps_max_layers_minus1;
    SINT32 vps_max_sub_layers_minus1;
    SINT32 vps_reserved_0xffff_16bits;
    SINT32 vps_max_layer_id;
    SINT32 vps_num_layer_sets_minus1;
    SINT32 vps_num_units_in_tick;
    SINT32 vps_time_scale;
    SINT32 vps_num_ticks_poc_diff_one_minus1;
    SINT32 vps_num_hrd_parameters;
    SINT32 vps_max_dec_pic_buffering[HEVC_MAX_TEMPLAYER];
    SINT32 vps_num_reorder_pics[HEVC_MAX_TEMPLAYER];
    SINT32 vps_max_latency_increase[HEVC_MAX_TEMPLAYER];
    SINT32 layer_id_included_flag[HEVC_MAX_VPS_OP_SETS_PLUS1][HEVC_MAX_VPS_NUH_RESERVED_ZERO_LAYER_ID_PLUS1];
    SINT32 hrd_layer_set_idx[HEVC_MAX_VPS_OP_SETS_PLUS1];
    SINT32 cprms_present_flag[HEVC_MAX_VPS_OP_SETS_PLUS1];
    
    HEVC_PROFILE_TIER_LEVEL_S profile_tier_level;
    HEVC_HRD_PARAM_SET_S hrd_parameters;
}HEVC_VIDEO_PARAM_SET_S;

typedef struct
{
    UINT8  aspect_ratio_info_present_flag;
    UINT8  overscan_info_present_flag;
    UINT8  overscan_appropriate_flag;
    UINT8  video_signal_type_present_flag;
    UINT8  video_full_range_flag;
    UINT8  colour_description_present_flag;
    UINT8  chroma_loc_info_present_flag;
    UINT8  neutral_chroma_indication_flag;
    UINT8  field_seq_flag;
    UINT8  frame_field_info_present_flag;
    UINT8  default_display_window_flag;
    UINT8  vui_timing_info_present_flag;
    UINT8  vui_poc_proportional_to_timing_flag;
    UINT8  vui_hrd_parameters_present_flag;
    UINT8  bitstream_restriction_flag;
    UINT8  tiles_fixed_structure_flag;
    UINT8  motion_vectors_over_pic_boundaries_flag;
    UINT8  restricted_ref_pic_lists_flag;
    
    UINT32 aspect_ratio_idc;
    UINT32 sar_width;
    UINT32 sar_height;
    UINT32 video_format;
    UINT32 colour_primaries;
    UINT32 transfer_characteristics;
    UINT32 matrix_coeffs;
    UINT32 chroma_sample_loc_type_top_field;
    UINT32 chroma_sample_loc_type_bottom_field;
    UINT32 def_disp_win_left_offset;
    UINT32 def_disp_win_right_offset;
    UINT32 def_disp_win_top_offset;
    UINT32 def_disp_win_bottom_offset;
    UINT32 vui_num_units_in_tick;
    UINT32 vui_time_scale;
    UINT32 vui_num_ticks_poc_diff_one_minus1;
    UINT32 min_spatial_segmentation_idc;
    UINT32 max_bytes_per_pic_denom;
    UINT32 max_bits_per_min_cu_denom;
    UINT32 log2_max_mv_length_horizontal;
    UINT32 log2_max_mv_length_vertical;
    
    HEVC_HRD_PARAM_SET_S hrd_parameters;
}HEVC_VUI_PARAM_SET_S;


typedef struct 
{
    UINT8  is_refresh;
    UINT8  valid;
    UINT8  dpbsize_changed;
    UINT8  sps_temporal_id_nesting_flag;
    UINT8  sps_sub_layer_ordering_info_present_flag;
    UINT8  restricted_ref_pic_lists_flag;
    UINT8  scaling_list_enabled_flag;
    UINT8  sps_scaling_list_data_present_flag;
    UINT8  amp_enabled_flag;
    UINT8  sample_adaptive_offset_enabled_flag;
    UINT8  pcm_loop_filter_disable_flag;
    UINT8  long_term_ref_pics_present_flag;
    UINT8  sps_temporal_mvp_enable_flag;
    UINT8  sps_strong_intra_smoothing_enable_flag;
    UINT8  vui_parameters_present_flag;
    UINT8  sps_extension_flag;
    UINT8  sps_extension_data_flag;
    UINT8  used_by_curr_pic_lt_sps_flag[HEVC_MAX_LSB_NUM];
    
    UINT32 video_parameter_set_id;
    UINT32 sps_max_sub_layers_minus1;
    UINT32 seq_parameter_set_id;
    UINT32 chroma_format_idc;
    UINT32 separate_colour_plane_flag;
    UINT32 pic_width_in_luma_samples;
    UINT32 pic_height_in_luma_samples;
    UINT32 conformance_window_flag;
    UINT32 conf_win_left_offset;
    UINT32 conf_win_right_offset;
    UINT32 conf_win_top_offset;
    UINT32 conf_win_bottom_offset;
    UINT32 bit_depth_luma;
    UINT32 qp_bdOffset_y;
    UINT32 bit_depth_chroma;
    UINT32 qp_bdOffset_c;
    UINT32 pcm_enabled_flag;
    UINT32 pcm_bit_depth_luma;
    UINT32 pcm_bit_depth_chroma;
    UINT32 max_pic_order_cnt_lsb;
    UINT32 bits_for_poc;
    UINT32 log2_min_luma_coding_block_size_minus3;
    UINT32 log2_min_cb_sizeY;
    UINT32 log2_diff_max_min_luma_coding_block_size;
    UINT32 log2_ctb_sizeY;
    UINT32 min_cb_sizeY;
    UINT32 ctb_sizeY;
    UINT32 max_cu_width;
    UINT32 max_cu_height;
    UINT32 log2_min_transform_block_size_minus2;
    UINT32 quadtree_tu_log2_min_size;
    UINT32 log2_diff_max_min_transform_block_size;
    UINT32 quadtree_tu_log2_max_size;
    UINT32 log2_min_pcm_coding_block_size_minus3;
    UINT32 pcm_log2_min_size;
    UINT32 log2_diff_max_min_pcm_coding_block_size;
    UINT32 pcm_log2_max_size;
    UINT32 max_transform_hierarchy_depth_inter;
    UINT32 quadtree_tu_max_depth_inter;
    UINT32 max_transform_hierarchy_depth_intra;
    UINT32 quadtree_tu_max_depth_intra;
    UINT32 max_cu_depth;
    UINT32 min_tr_depth;
    UINT32 max_tr_depth;
    UINT32 num_short_term_ref_pic_sets;
    UINT32 num_long_term_ref_pic_sps;
    UINT32 dpb_size;
    UINT32 ctb_num_width;   // PicWidthInCtbsY 
    UINT32 ctb_num_height;  // PicHeightInCtbsY
    UINT32 lt_ref_pic_poc_lsb_sps[HEVC_MAX_LSB_NUM];
    UINT32 max_dec_pic_buffering[HEVC_MAX_TEMPLAYER];
    UINT32 num_reorder_pics[HEVC_MAX_TEMPLAYER];
    UINT32 max_latency_increase[HEVC_MAX_TEMPLAYER];
    
    HEVC_PROFILE_TIER_LEVEL_S profile_tier_level;
    HEVC_SCALING_LIST scaling_list;
    HEVC_VUI_PARAM_SET_S vui_parameters;
    HEVC_SHORT_TERM_RPSET_S short_term_ref_pic_set[64];
} HEVC_SEQ_PARAM_SET_S;

typedef enum
{
    BUFFERING_PERIOD                      = 0,
    PICTURE_TIMING                        = 1,
    PAN_SCAN_RECT                         = 2,
    FILLER_PAYLOAD                        = 3,
    USER_DATA_REGISTERED_ITU_T_T35        = 4,
    USER_DATA_UNREGISTERED                = 5,
    RECOVERY_POINT                        = 6,
    SCENE_INFO                            = 9,
    FULL_FRAME_SNAPSHOT                   = 15,
    PROGRESSIVE_REFINEMENT_SEGMENT_START  = 16,
    PROGRESSIVE_REFINEMENT_SEGMENT_END    = 17,
    FILM_GRAIN_CHARACTERISTICS            = 19,
    POST_FILTER_HINT                      = 22,
    TONE_MAPPING_INFO                     = 23,
    FRAME_PACKING                         = 45,
    DISPLAY_ORIENTATION                   = 47,
    SOP_DESCRIPTION                       = 128,
    ACTIVE_PARAMETER_SETS                 = 129,
    DECODING_UNIT_INFO                    = 130,
    TEMPORAL_LEVEL0_INDEX                 = 131,
    DECODED_PICTURE_HASH                  = 132,
    SCALABLE_NESTING                      = 133,
    REGION_REFRESH_INFO                   = 134,        
}HEVC_SEI_PAYLOADTYPE;

typedef struct
{
    UINT32 initial_cpb_removal_delay;
    UINT32 initial_cpb_removal_offset;
    UINT32 initial_alt_cpb_removal_delay;
    UINT32 initial_alt_cpb_removal_offset;
}HEVC_SEI_BUF_PERIOD_INITIAL_S;

typedef struct
{
    UINT8  irap_cpb_params_present_flag;
    UINT8  concatenation_flag;
    UINT32 bp_seq_parameter_set_id;
    UINT32 cpb_delay_offset;
    UINT32 dpb_delay_offset;
    UINT32 au_cpb_removal_delay_delta_minus1;
    HEVC_SEI_BUF_PERIOD_INITIAL_S nal_vcl_inf[HEVC_MAX_CPB_CNT][2];
}HEVC_SEI_BUF_PERIOD_S;

typedef struct
{
    UINT8  duplicate_flag;
    UINT8  du_common_cpb_removal_delay_flag;
    UINT32 pic_struct;
    UINT32 source_scan_type;
    UINT32 au_cpb_removal_delay_minus1;
    UINT32 pic_dpb_output_delay;
    UINT32 pic_dpb_output_du_delay;
    UINT32 num_decoding_units_minus1;
    UINT32 du_common_cpb_removal_delay_increment_minus1;
    UINT32 num_nalus_in_du_minus1[256];	
    UINT32 du_cpb_removal_delay_increment_minus1[256];
}HEVC_SEI_PIC_TIMMING_S;

typedef struct
{
    UINT8 *user_data_payload_byte;  
    UINT8  uuid_iso_iec_11578[16];
    UINT32 userDataLength;      
}HEVC_SEI_USER_DATA_UNREG_S;

typedef struct
{
    UINT8  self_contained_cvs_flag;
    UINT8  no_parameter_set_update_flag;
    SINT32 active_video_parameter_set_id;
    SINT32 num_sps_ids_minus1;
    SINT32 active_seq_parameter_set_id[16];
}HEVC_SEI_ACTIVE_PARAMETER_SET_S;

typedef struct
{
    UINT8  dpb_output_du_delay_present_flag;
    SINT32 decoding_unit_idx;
    SINT32 du_spt_cpb_removal_delay_increment;
    SINT32 pic_spt_dpb_output_du_delay;        
}HEVC_SEI_DECODEING_UNIT_INFO_S;

typedef struct
{
    UINT8  exact_match_flag;
    UINT8  broken_link_flag;
    SINT32 recovery_poc_cnt;
}HEVC_SEI_RECOVERY_POINT_S;

typedef struct
{
    UINT8  frame_packing_arrangement_cancel_flag;
    UINT8  quincunx_sampling_flag;
    UINT8  spatial_flipping_flag;
    UINT8  frame0_flipped_flag;
    UINT8  field_views_flag;
    UINT8  current_frame_is_frame0_flag;
    UINT8  frame0_self_contained_flag;
    UINT8  frame1_self_contained_flag;
    UINT8  frame_packing_arrangement_persistence_flag;
    UINT8  upsampled_aspect_ratio_flag;   
    UINT32 frame_packing_arrangement_id;
    UINT32 frame_packing_arrangement_type;
    UINT32 content_interpretation_type;
    UINT32 frame0_grid_position_x;
    UINT32 frame0_grid_position_y;
    UINT32 frame1_grid_position_x;
    UINT32 frame1_grid_position_y;
    UINT32 frame_packing_arrangement_reserved_byte;    
}HEVC_SEI_FRAME_PACKING_ARRANGEMENT_S;

typedef struct
{
    UINT8  hor_flip;
    UINT8  ver_flip;
    UINT8  display_orientation_cancel_flag;
    UINT8  display_orientation_persistence_flag;
    UINT32 anticlockwise_rotation;
}HEVC_SEI_DISPLAY_ORIENTATION_S;

typedef struct
{
    UINT32 temporal_sub_layer_zero_idx;
    UINT32 irap_pic_id;
}HEVC_SEI_TEMPORAL_SUB_LAYER_S;

typedef struct
{
    UINT8 refreshed_region_flag;
}HEVC_SEI_REGION_REFRESH_INFO_S;   

typedef struct
{
    UINT8  tone_map_cancel_flag;
    UINT8  tone_map_persistence_flag;
    UINT8  exposure_compensation_value_sign_flag;
    UINT32 tone_map_id;
    UINT32 coded_data_bit_depth;
    UINT32 target_bit_depth;
    UINT32 tone_map_model_id;
    UINT32 min_value;
    UINT32 max_value;
    UINT32 sigmoid_midpoint;
    UINT32 sigmoid_width;
    UINT32 camera_iso_speed_idc;
    UINT32 camera_iso_speed_value;
    UINT32 exposure_index_idc;
    UINT32 exposure_index_value;
    UINT32 exposure_compensation_value_numerator;
    UINT32 exposure_compensation_value_denom_idc;
    UINT32 ref_screen_luminance_white;
    UINT32 extended_range_white_level;
    UINT32 nominal_black_level_code_value;
    UINT32 nominal_white_level_code_value;
    UINT32 extended_white_level_code_value;
    UINT32 num_pivots;
    UINT32 coded_pivot_value[256];
    UINT32 target_pivot_value[256];
    UINT32 start_of_coded_interval[257];
}HEVC_SEI_TONE_MAPPING_INFO_S;

typedef struct
{
    UINT32 sop_seq_parameter_set_id;
    UINT32 num_entries_in_sop_minus1;
    UINT32 sop_vcl_nut[HEVC_MAX_NUM_PICS_IN_SOP];
    UINT32 sop_temporal_id[HEVC_MAX_NUM_PICS_IN_SOP];
    UINT32 sop_short_term_rps_idx[HEVC_MAX_NUM_PICS_IN_SOP];
    SINT32 sop_poc_delta[HEVC_MAX_NUM_PICS_IN_SOP];
}HEVC_SEI_SOP_INFO_S;

typedef enum
{
    MD5,
    CRC,
    CHECKSUM,
    RESERVED,
}HEVC_SEI_Method;


typedef struct
{
    UINT8  digest[3][16];
    UINT32 hash_type;
    HEVC_SEI_Method method;
}HEVC_SEI_DECODED_PICTURE_HASH_S;

typedef struct
{
    UINT8  bitstream_subset_flag;
    UINT8  nesting_op_flag;
    UINT8  default_op_flag;
    UINT8  all_layers_flag;
    UINT8  callerOwnsSEIs; 
    UINT32 nesting_num_ops_minus1;
    UINT32 nesting_max_temporal_id_plus1[HEVC_MAX_TEMPLAYER];
    UINT32 nesting_op_idx[HEVC_MAX_NESTING_NUM_LAYER];
    UINT32 nesting_no_op_max_temporal_id_plus1;
    UINT32 nesting_num_layers_minus1;
    UINT32 nesting_layer_id[HEVC_MAX_NESTING_NUM_LAYER];
}HEVC_SEI_SCALABLE_NESTING_S;

typedef struct
{
    UINT32 last_payload_type_byte;
    UINT32 last_payload_size_byte;
    UINT32 reserved_payload_extension_data;
    UINT32 payload_bit_equal_to_one;
    UINT32 payload_bit_equal_to_zero;
    HEVC_SEI_PAYLOADTYPE payloadtype;
    HEVC_SEI_BUF_PERIOD_S buf_period;
    HEVC_SEI_PIC_TIMMING_S pic_timming;
    HEVC_SEI_USER_DATA_UNREG_S user_data_unreg;
    HEVC_SEI_ACTIVE_PARAMETER_SET_S active_param_sets;
    HEVC_SEI_DECODEING_UNIT_INFO_S decoding_unit_info;
    HEVC_SEI_RECOVERY_POINT_S recovery_point;
    HEVC_SEI_FRAME_PACKING_ARRANGEMENT_S frame_packing;
    HEVC_SEI_DISPLAY_ORIENTATION_S display_orientation;
    HEVC_SEI_TEMPORAL_SUB_LAYER_S temporal_sub_layer;
    HEVC_SEI_REGION_REFRESH_INFO_S region_refresh_info;
    HEVC_SEI_TONE_MAPPING_INFO_S tone_mapping_info;
    HEVC_SEI_SOP_INFO_S sop_info;
    HEVC_SEI_SCALABLE_NESTING_S scalable_nesting;  
    HEVC_SEI_DECODED_PICTURE_HASH_S decoded_picture_hash; 
}HEVC_SEI_PARAM_SET_S;

typedef struct
{
    UINT8  valid;
    UINT8  is_refresh;
    UINT8  dependent_slice_segments_enabled_flag;
    UINT8  sign_data_hiding_flag;
    UINT8  output_flag_present_flag;
    UINT8  cabac_init_present_flag;
    UINT8  constrained_intra_pred_flag;
    UINT8  transform_skip_enabled_flag;
    UINT8  cu_qp_delta_enabled_flag;
    UINT8  pic_slice_chroma_qp_offsets_present_flag;
    UINT8  weighted_pred_flag;
    UINT8  weighted_bipred_flag;
    UINT8  transquant_bypass_enable_flag;
    UINT8  tiles_enabled_flag;
    UINT8  entropy_coding_sync_enabled_flag;
    UINT8  uniform_spacing_flag;
    UINT8  loop_filter_across_tiles_enabled_flag;
    UINT8  loop_filter_across_slices_enabled_flag;
    UINT8  deblocking_filter_control_present_flag;
    UINT8  deblocking_filter_override_enabled_flag;
    UINT8  pic_disable_deblocking_filter_flag;
    UINT8  pic_scaling_list_data_present_flag;
    UINT8  lists_modification_present_flag;
    UINT8  slice_segment_header_extension_present_flag;
    UINT8  pps_extension_flag;
    UINT8  pps_extension_data_flag;
    
    SINT32 pic_parameter_set_id;
    SINT32 seq_parameter_set_id;
    UINT32 num_extra_slice_header_bits;
    SINT32 num_ref_idx_l0_default_active;
    SINT32 num_ref_idx_l1_default_active;
    SINT32 pic_init_qp;
    SINT32 diff_cu_qp_delta_depth;
    SINT32 max_cu_qp_delta_depth;
    SINT32 pic_cb_qp_offset;
    SINT32 pic_cr_qp_offset;
    SINT32 num_tile_columns;
    SINT32 num_tile_rows;
    SINT32 pps_beta_offset_div2;
    SINT32 pps_tc_offset_div2;
    SINT32 log2_parallel_merge_level;
    SINT32 log2_min_cu_qp_delta_size;
    SINT32 column_width[HEVC_MAX_TILE_COLUMNS];
    SINT32 row_height[HEVC_MAX_TILE_ROWS];
    SINT32 column_bd[HEVC_MAX_TILE_COLUMNS];
    SINT32 row_bd[HEVC_MAX_TILE_ROWS];
    
    HEVC_SCALING_LIST  scaling_list;
} HEVC_PIC_PARAM_SET_S;

typedef enum
{
    NOT_NEW_PIC = 0,
    IS_NEW_PIC,
    IS_SKIP_PIC,
}HEVC_PIC_TYPE;

typedef struct
{
    UINT8  pic_output_flag;
    UINT8  first_slice_segment_in_pic_flag;
    UINT8  no_output_of_prior_pics_flag;
    UINT8  dependent_slice_segment_flag;
    UINT8  short_term_ref_pic_set_sps_flag;
    UINT8  slice_sao_luma_flag;
    UINT8  slice_sao_chroma_flag;
    UINT8  slice_temporal_mvp_enable_flag;
    UINT8  num_ref_idx_active_override_flag;
    UINT8  mvd_l1_zero_flag;
    UINT8  cabac_init_flag;
    UINT8  collocated_from_l0_flag;
    UINT8  deblocking_filter_override_flag;
    UINT8  slice_disable_deblocking_filter_flag;
    UINT8  slice_loop_filter_across_slices_enabled_flag;
    UINT8  used_by_curr_pic_lt_flag[HEVC_MAX_NUM_REF_PICS];
    UINT8  delta_poc_msb_present_flag[HEVC_MAX_NUM_REF_PICS];
    
    SINT32 poc;
    SINT32 prev_poc;
    SINT32 slice_qp;
    SINT32 slice_type;
    SINT32 offset_len;
    UINT32 nuh_temporal_id;
    UINT32 nal_unit_type;
    SINT32 pic_parameter_set_id;
    SINT32 slice_segment_address;
    SINT32 colour_plane_id;
    SINT32 pic_order_cnt_lsb;
    SINT32 short_term_ref_pic_set_idx;
    UINT32 num_long_term_sps;
    UINT32 num_long_term_pics;
    UINT32 num_ref_idx_l0_active;
    UINT32 num_ref_idx_l1_active;
    SINT32 collocated_ref_idx;
    SINT32 max_num_merge_cand;
    SINT32 slice_qp_delta;
    SINT32 slice_cb_qp_offset;
    SINT32 slice_cr_qp_offset;
    SINT32 slice_beta_offset_div2;
    SINT32 slice_tc_offset_div2;
    UINT32 num_entry_point_offsets;
    UINT32 slice_segment_header_extension_length;
    UINT32 slice_segment_header_extension_data_byte;
    UINT32 dependent_slice_curstart_cuaddr;
    UINT32 dependent_slice_curend_cuaddr;
    UINT32 listXsize[2];
    UINT32 num_ref_idx[3];
    SINT32 entry_point_offset[256];
    SINT32 lt_idx_sps[HEVC_MAX_NUM_REF_PICS];
    SINT32 poc_lsb_lt[HEVC_MAX_NUM_REF_PICS];
    SINT32 delta_poc_msb_cycle_lt[HEVC_MAX_NUM_REF_PICS];
    SINT32 pocLsbLt[HEVC_MAX_NUM_REF_PICS];
    SINT32 deltaPocMSBCycleLT[HEVC_MAX_NUM_REF_PICS];
    SINT32 CheckLTMSB[HEVC_MAX_NUM_REF_PICS];
    
    HEVC_PIC_TYPE new_pic_type;
    HEVC_PRED_WEIGHT_TABLE pred_weight_table;
    HEVC_SHORT_TERM_RPSET_S short_term_ref_pic_set;
    HEVC_REF_PIC_LISTS_MODDIFICATION ref_pic_lists_modification;
}HEVC_SLICE_SEGMENT_HEADER;


typedef struct
{
    SINT32  poc;
    UINT32  err_level;
    UINT32  pic_type;
    UINT32  structure;     // 0: frame, 1: top_field, 2: bottom_field, 3: field pair
    UINT32  is_long_term;  // 0: not long term ref,  1: long term ref  
    UINT32  is_short_term; // 0: not short term ref, 1: short term ref. BUT one pic cann't long and short.   
    SINT32  long_term_pic_num;
    SINT32  pic_num; 
    
    struct  HEVC_FRAMESTORE_S *frame_store;
} HEVC_STORABLEPIC_S;

typedef enum
{
    FIRST_FRAME_OUT_INIT = 0,
    FIRST_FRAME_WAIT_OUT,
    FIRST_FRAME_ALREADY_OUT,
} HEVC_FO_STATE_E;

typedef enum
{
    FS_NOT_USED = 0,
    FS_IN_DPB,
    FS_DISPLAY_ONLY,
} HEVC_FS_STATE_E;

typedef struct HEVC_FRAMESTORE_S
{      
    UINT8   non_existing;              // 0: real frame, 1: fake frame from creat lost ref pic
    UINT8   is_reference;              // 0: not ref,    1: for ref 
    UINT8   is_displayed;              // 0: not insert, 1: already insert in vo queue
    HEVC_FS_STATE_E  eFrameStoreState; // frame store state
	HEVC_FO_STATE_E  eFirstFrameState; // first frame fast out state
    SINT32  poc;
    UINT32  pic_type;
    UINT32  err_level;
    UINT32  pmv_address_idc; 
    UINT32  apc_idc;
	SINT32  logic_fs_id;    
	
    HEVC_STORABLEPIC_S  frame;
    IMAGE   fs_image; 
} HEVC_FRAMESTORE_S;

typedef struct
{
    UINT8   is_ref_idc;
    UINT8   state;   
    UINT32  nal_unit_type;
    SINT32  thispoc;
    UINT32  pic_type;
    UINT32  pic_width;
    UINT32  pic_height;
    UINT32  pic_ctbs;  
    UINT32  start_count;  
    UINT32  stream_base_addr;
    UINT32  pmv_address_idc; 
	UINT32  err_level;
    
    HEVC_FRAMESTORE_S *frame_store; 
    IMAGE   fs_image; 
} HEVC_CURRPIC_S;

typedef struct
{
    UINT32  size;
    UINT32  used_size;
    UINT32  is_used[HEVC_APC_SIZE];                       
    UINT32  idc[HEVC_APC_SIZE];
    SINT32  poc[HEVC_APC_SIZE];
} HEVC_APC_S;

typedef struct
{
    UINT32  size;
    UINT32  used_size;
    UINT32  max_long_term_pic_idx;
    UINT32  ltref_frames_in_buffer;
    UINT32  negative_ref_frames_in_buffer;
    UINT32  positive_ref_frames_in_buffer;
    HEVC_FRAMESTORE_S *fs[HEVC_MAX_DPB_NUM];
    HEVC_FRAMESTORE_S *fs_negative_ref[HEVC_MAX_DPB_NUM];
    HEVC_FRAMESTORE_S *fs_positive_ref[HEVC_MAX_DPB_NUM];
    HEVC_FRAMESTORE_S *fs_ltref[HEVC_MAX_DPB_NUM];
} HEVC_DPB_S;

typedef struct
{
    UINT8  *streamBuffer;       // [MAX_CODED_FRAME_SIZE], actual codebuffer for read bytes
    UINT32  bitstream_length;   // over codebuffer length, byte oriented, UVLC only
    UINT32  stream_phy_addr;    // physic addr of stream    unit: byte
    UINT32  valid_bitlen;       // valid pBs when dec slice data    unit: bit
    UINT32  bitsoffset;         // offset in 128 pBs when this segment stream start    unit: bit
    UINT32  bytespos;           // stream VDH will used start bytes ph addr, 4words/16bytes addr aligned     unit: byte
    SINT32  StreamID;			// stream seg id for release used
} HEVC_BITSTREAM_S;

typedef struct
{
	UINT8   dsp_check_pic_over_flag;
    UINT8   nal_used_segment;     // which segment using decode
    UINT32  nal_bitoffset;
    UINT32  nal_segment;          // how many stream segment received in this nal
    UINT32  nal_trail_zero_bit_num; 
    UINT32  nal_unit_type;
    UINT32  nal_ref_idc;
    UINT32  forbidden_zero_bit;
	UINT32  nuh_reserved_zero_6bits;
	UINT32  nuh_temporal_id;
    UINT32  is_valid;
    UINT32  nal_integ;

    HEVC_BITSTREAM_S  stream[2];
} HEVC_NALU_S;

typedef struct 
{      
    UINT32       slot_addr_ph;
    UINT32       first_mb_in_slice; // is valid when slice slot
    UINT32       slice_type;
    SINT32       picid_refidx0;     // 当前slice在list0中第一个元素对应的帧存槽位号，负数无效，P slice repair
    SINT32       picid_nearpoc;     // 当前slice在dpb中找到的poc距离最近的帧存的槽位号，负数无效，I slice repair
    
    UINT32      *slot_addr_vir;
    HEVC_NALU_S *slice_nal; 
} HEVC_MSGSLOT_S;

enum NalUnitType
{
    NAL_UNIT_CODED_SLICE_TRAIL_N = 0,  // 0
    NAL_UNIT_CODED_SLICE_TRAIL_R,      // 1
    
    NAL_UNIT_CODED_SLICE_TSA_N,        // 2
    NAL_UNIT_CODED_SLICE_TLA_R,        // 3    // Current name in the spec: TSA_R
    
    NAL_UNIT_CODED_SLICE_STSA_N,       // 4
    NAL_UNIT_CODED_SLICE_STSA_R,       // 5
    
    NAL_UNIT_CODED_SLICE_RADL_N,       // 6
    NAL_UNIT_CODED_SLICE_RADL_R,       // 7    // Current name in the spec: RADL_R
    
    NAL_UNIT_CODED_SLICE_RASL_N,       // 8
    NAL_UNIT_CODED_SLICE_RASL_R,       // 9    // Current name in the spec: RASL_R
    
    NAL_UNIT_RESERVED_VCL_N10,
    NAL_UNIT_RESERVED_VCL_R11,
    NAL_UNIT_RESERVED_VCL_N12,
    NAL_UNIT_RESERVED_VCL_R13,
    NAL_UNIT_RESERVED_VCL_N14,
    NAL_UNIT_RESERVED_VCL_R15,
    
    NAL_UNIT_CODED_SLICE_BLA_W_LP,     // 16   // Current name in the spec: BLA_W_LP
    NAL_UNIT_CODED_SLICE_BLA_W_RADL,   // 17   // Current name in the spec: BLA_W_DLP
    NAL_UNIT_CODED_SLICE_BLA_N_LP,     // 18
    NAL_UNIT_CODED_SLICE_IDR_W_RADL,   // 19   // Current name in the spec: IDR_W_DLP
    NAL_UNIT_CODED_SLICE_IDR_N_LP,     // 20
    NAL_UNIT_CODED_SLICE_CRA,          // 21
    NAL_UNIT_RESERVED_IRAP_VCL22,
    NAL_UNIT_RESERVED_IRAP_VCL23,
    
    NAL_UNIT_RESERVED_VCL24,
    NAL_UNIT_RESERVED_VCL25,
    NAL_UNIT_RESERVED_VCL26,
    NAL_UNIT_RESERVED_VCL27,
    NAL_UNIT_RESERVED_VCL28,
    NAL_UNIT_RESERVED_VCL29,
    NAL_UNIT_RESERVED_VCL30, 
    NAL_UNIT_RESERVED_VCL31,
    
    NAL_UNIT_VPS,                      // 32
    NAL_UNIT_SPS,                      // 33
    NAL_UNIT_PPS,                      // 34
    NAL_UNIT_ACCESS_UNIT_DELIMITER,    // 35
    NAL_UNIT_EOS,                      // 36
    NAL_UNIT_EOB,                      // 37
    NAL_UNIT_FILLER_DATA,              // 38
    NAL_UNIT_PREFIX_SEI,               // 39 Prefix SEI
    NAL_UNIT_SUFFIX_SEI,               // 40 Suffix SEI
    NAL_UNIT_RESERVED_NVCL41,
    NAL_UNIT_RESERVED_NVCL42,
    NAL_UNIT_RESERVED_NVCL43,
    NAL_UNIT_RESERVED_NVCL44,
    NAL_UNIT_RESERVED_NVCL45,
    NAL_UNIT_RESERVED_NVCL46,
    NAL_UNIT_RESERVED_NVCL47,
    
    NAL_UNIT_EOPIC,                    // NAL_UNIT_UNSPECIFIED_48, 自定义一帧图像结束特殊nal类型
    NAL_UNIT_EOSTREAM,                 // NAL_UNIT_UNSPECIFIED_49, 自定义码流结束特殊nal类型
    NAL_UNIT_UNSPECIFIED_50,
    NAL_UNIT_UNSPECIFIED_51,
    NAL_UNIT_UNSPECIFIED_52,
    NAL_UNIT_UNSPECIFIED_53,
    NAL_UNIT_UNSPECIFIED_54,
    NAL_UNIT_UNSPECIFIED_55,
    NAL_UNIT_UNSPECIFIED_56,
    NAL_UNIT_UNSPECIFIED_57,
    NAL_UNIT_UNSPECIFIED_58,
    NAL_UNIT_UNSPECIFIED_59,
    NAL_UNIT_UNSPECIFIED_60,
    NAL_UNIT_UNSPECIFIED_61,
    NAL_UNIT_UNSPECIFIED_62,
    NAL_UNIT_UNSPECIFIED_63,
    NAL_UNIT_INVALID,
};

typedef struct
{
    UINT8  uniform_spacing_flag;
    SINT32 num_tile_rows_minus1;
    SINT32 num_tile_columns_minus1;
    SINT32 row_height_minus1[HEVC_MAX_TILE_ROWS];
    SINT32 column_width_minus1[HEVC_MAX_TILE_COLUMNS];
}HEVC_TILE_INFO_S;

typedef struct
{
    SINT32 pic_width_in_luma_sample;
    SINT32 pic_height_in_luma_sample;
    SINT32 CtbSizeYLog2;
    SINT32 MinTbSizeLog2;
    SINT32 PicWidthInCtb;
    SINT32 PicHeithtInCtb;
}HEVC_PIC_SIZE_INFO_S;

typedef struct
{
    UINT8   last_pack_in_nal;
    UINT8   pack_is_pic_over_flag;
    UINT8  *p_stream;
    UINT32  strm_phy_addr;
    SINT32  stream_len;
    SINT32  StreamID;
    UINT64  pts;
} HEVC_STREAM_PACKET;

typedef enum
{
    FIRST_FRAME_INIT = 0,
    FIRST_FRAME_PROCESSED,
} HEVC_FIRST_FRAME_STATE_E;

typedef union
{
    HEVC_VIDEO_PARAM_SET_S VpsTmp;
    HEVC_SEQ_PARAM_SET_S   SpsTmp;
    HEVC_PIC_PARAM_SET_S   PpsTmp;
} HEVC_TMP_PARAM_SET_S;

typedef struct
{
    UINT8   bNewSequence;
    UINT8   bNoOutputOfPriorPicsFlag;
    UINT8   bNoRaslOutputFlag;
    UINT8   bPPicFound;
    UINT8   bHaveNalNotDecoded;
	UINT8   bIsInsertEOPICNal;          //是否是代码插入eopicnal还是真正码流里存在naltype=30的nal
    UINT8   IsStreamEndFlag;
    UINT8   PrevRAPIsBLA;
    SINT8   aucConvertToBit[HEVC_MAX_CU_SIZE+1];

    SINT32  MaxSliceNum;
    SINT32  MaxVpsNum;
    SINT32  MaxSpsNum;
    SINT32  MaxPpsNum;
    UINT32  AllowStartDec;
    UINT32  CurrMsgSlotNum;
    UINT32  TotalFsNum;                 // change while g_DPB.size change
    UINT32  TotalPmvNum;                // change while g_DPB.size change
    SINT32  PrevSlicePoc;
    SINT32  LastSlicePoc;
    UINT32  BitDepthY;
    UINT32  BitDepthC;
    SINT32  PocRandomAccess;
    SINT32  PocCRA ;
	UINT32  NumSlicePara;
	SINT32  LastDisplayPoc;
    UINT32  TotalSliceNum;
    UINT32  TotalPicNum;
    UINT32  MaxBytesReceived;
    UINT32  uiAddCuDepth;
    UINT32  AdvancedCfg;
    UINT32  RefErrorThr;
    UINT32  OutErrorThr;
    SINT32  ChanID;
	UINT32  OldDecMode;
	UINT32  prev_poc;
    UINT32  NewPicCounter;
    UINT32  PicOutCounter;
	SINT32  TotalFrameNum;
	SINT32  OutputFrameNum;
	UINT32  prev_pic_parameter_set_id;  //用于判断是否是新的一帧图像,特别是第一个slice被丢弃(或者解错)的情况下
    
    UINT32  scalingListSize[4]; 
    UINT32  scalingListSizeX[4];
    UINT32  sigLastScanCG32x32[64];
    UINT32  qmatrix[4][6][16];          // 4 types:4x4 8x8 16x16 32x32, 对于4x4，16个bytes拼在4words中，其余64个bytes拼在16words中
    UINT32  PmvStoreUsedFlag[HEVC_MAX_PMV_STORE]; // pmv store memory allocate statistic 
    UINT32  scalingListNum[SCALING_LIST_SIZE_NUM];
	UINT32  auiSigLastScan[4][HEVC_MAX_CU_DEPTH][128*128];
    UINT32  scalingListDC[SCALING_LIST_SIZE_NUM][HEVC_SCALING_LIST_NUM]; 
    UINT32  IsLongTerm[HEVC_MAX_SLICE_NUM][2][HEVC_MAX_NUM_REF_PICS];
    UINT32  ApcList[HEVC_MAX_SLICE_NUM][2][HEVC_MAX_NUM_REF_PICS];
    
    BS                        *pBS;
    HEVC_VIDEO_PARAM_SET_S    *pVPS;
    HEVC_SEQ_PARAM_SET_S      *pSPS;
    HEVC_PIC_PARAM_SET_S      *pPPS;
    SYNTAX_EXTRA_DATA_S       *pstExtraData;
	HEVC_DEC_PIC_PARAM_S      *pHevcDecParam;
    HEVC_NALU_S               *pCurrNal;
    HEVC_STORABLEPIC_S        *pListX[2][HEVC_MAX_LIST_SIZE];
    
    BS                         Bs;
    HEVC_DPB_S                 DPB;
    HEVC_APC_S                 APC;
    HEVC_MEMINFO               HevcMemInfo;
    HEVC_TMP_PARAM_SET_S       TmpParam;
    HEVC_SEI_PARAM_SET_S       SEI;
    HEVC_VIDEO_PARAM_SET_S     CurrVPS;
    HEVC_SEQ_PARAM_SET_S       CurrSPS;
    HEVC_SEQ_PARAM_SET_S       SEISPS;
    HEVC_PIC_PARAM_SET_S       CurrPPS;
    HEVC_CURRPIC_S             CurrPic;
    HEVC_SLICE_SEGMENT_HEADER  CurrSlice;
	HEVC_SLICE_SEGMENT_HEADER  IndependentSlice;
    IMAGE_VO_QUEUE             ImageQue;
    HEVC_FIRST_FRAME_STATE_E   FirstFrameState;
    HEVC_DEC_PIC_PARAM_S       DecPicPara;
	HEVC_DEC_SLICE_PARAM_S    *pDecSlicePara; 
    HEVC_NALU_S                NalArray[HEVC_MAX_SLOT_NUM+1]; 
    PHYADDR                    Apc2MvAddr[HEVC_MAX_SLICE_NUM][HEVC_APC_SIZE];
    HEVC_SLICE_SEGMENT_HEADER  Slice[HEVC_MAX_SLICE_NUM];
    HEVC_MSGSLOT_S             MsgSlot[HEVC_MAX_SLOT_NUM];
    HEVC_FRAMESTORE_S          FrameStore[HEVC_MAX_FRAME_STORE];
    HEVC_WPSCALINGPARAM        WeightPredTable[2][HEVC_MAX_NUM_REF_PICS][3]; // [list0 or list1][refIdx][0:Y, 1:U, 2:V]
    UINT32     u32SCDWrapAround; //l00273086
    HEVC_NALU_S* pLastNal;//l00273086
}HEVC_CTX_S;


/*************************** Function Declare *************************/
SINT32 HEVCDEC_Init(HEVC_CTX_S *pHevcCtx,SYNTAX_EXTRA_DATA_S *pstExtraData);
VOID   HEVC_InitDecPara(HEVC_CTX_S *pHevcCtx);
SINT32 HEVC_ClearAll(HEVC_CTX_S *pHevcCtx,SINT32 flag);
VOID   HEVC_InitScalingOrderTable(HEVC_CTX_S *pHevcCtx);
VOID   HEVC_InitSigLastScan(HEVC_CTX_S *pHevcCtx, UINT32* pBuffZ, UINT32* pBuffH, UINT32* pBuffV, UINT32* pBuffD, SINT32 iWidth, SINT32 iHeight, SINT32 iDepth);
SINT32 *HEVC_GetScalingListDefaultAddress(UINT32 sizeId, UINT32 matrixId);
SINT32 HEVCDEC_DecodePacket(HEVC_CTX_S *pHevcCtx,HEVC_STREAM_PACKET *pPacket);
SINT32 HEVC_DecOneNal(HEVC_CTX_S *pHevcCtx);
SINT32 HEVC_DecVPS(HEVC_CTX_S *pHevcCtx);
SINT32 HEVC_DecSPS(HEVC_CTX_S *pHevcCtx);
SINT32 HEVC_DecPPS(HEVC_CTX_S *pHevcCtx);
SINT32 HEVC_DecSEI(HEVC_CTX_S *pHevcCtx);
SINT32 HEVC_DecSlice(HEVC_CTX_S *pHevcCtx);
SINT32 HEVC_DecSliceSegmentHeader(HEVC_CTX_S *pHevcCtx, UINT32 IsDecSlice);
SINT32 HEVC_DecVDM(HEVC_CTX_S *pHevcCtx);
SINT32 HEVC_VpsSpsPpsCheck(HEVC_CTX_S *pHevcCtx);
SINT32 HEVC_IsNewPic(HEVC_CTX_S *pHevcCtx);
SINT32 HEVC_DecPTL(HEVC_CTX_S *pHevcCtx,HEVC_PROFILE_TIER_LEVEL_S *pPTL, SINT32 ProfilePresentFlag, SINT32 MaxNumSubLayersMinus1);
SINT32 HEVC_DecScalingListData(HEVC_CTX_S *pHevcCtx,HEVC_SCALING_LIST *pScalingList);
SINT32 HEVC_DecShortTermRefPicSet(HEVC_CTX_S *pHevcCtx,HEVC_SEQ_PARAM_SET_S *pSPS, HEVC_SHORT_TERM_RPSET_S *pShortTermRpset, UINT32 idx);
SINT32 HEVC_DecRefPicListsModification(HEVC_CTX_S *pHevcCtx,HEVC_SLICE_SEGMENT_HEADER *pSlice, HEVC_REF_PIC_LISTS_MODDIFICATION *pRpListModification, SINT32 numRpsCurrTempList);
SINT32 HEVC_DecPredWeightTable(HEVC_CTX_S *pHevcCtx);
SINT32 HEVC_DecHrdParam(HEVC_CTX_S *pHevcCtx,HEVC_HRD_PARAM_SET_S *pHrd, UINT8 commonInfPresentFlag, UINT32 maxNumSubLayersMinus1);
SINT32 HEVC_DecVuiParam(HEVC_CTX_S *pHevcCtx,HEVC_VUI_PARAM_SET_S *pVui, HEVC_SEQ_PARAM_SET_S *pSps);
SINT32 HEVC_MoreRbspData(HEVC_CTX_S *pHevcCtx);
SINT32 HEVC_More_Rbsp_Data(HEVC_CTX_S *pHevcCtx);
SINT32 HEVC_WritePicMsg(HEVC_CTX_S *pHevcCtx);
SINT32 HEVC_WriteSliceMsg(HEVC_CTX_S *pHevcCtx);
UINT32 *HEVC_GetVirAddr(UINT32 Dn, UINT32 *pbase_vir_addr);
UINT32 HEVC_GetPhyAddr(UINT32 Dn, UINT32 base_phy_addr);
SINT32 HEVC_InitPic(HEVC_CTX_S *pHevcCtx);
SINT32 HEVC_InitDPB(HEVC_CTX_S *pHevcCtx,UINT32 ReRangeFlag);
SINT32 HEVC_StorePicInDPB(HEVC_CTX_S *pHevcCtx, LUMA_INFO_S *pLumaInfo);
SINT32 HEVC_DecList(HEVC_CTX_S *pHevcCtx);
SINT32 HEVC_ArrangeVahbMem(HEVC_CTX_S *pHevcCtx,UINT32 ImgWidth, UINT32 ImgHeight, UINT32 ImgWidthInCtb, UINT32 ImgHeightInCtb, UINT32 ImgNum, UINT32 PmvBlkNum);
SINT32 HEVC_AllocFrameStore(HEVC_CTX_S *pHevcCtx);
SINT32 HEVC_GetReRangeFlag(HEVC_CTX_S *pHevcCtx,SINT32 OldWidth, SINT32 OldHeight, SINT32 OldLog2CtbSizeY);
VOID   HEVC_WaitVo(HEVC_CTX_S *pHevcCtx);
VOID   HEVC_GetBackPicFromVOQueue(HEVC_CTX_S *pHevcCtx);
VOID HEVC_RemoveDummyFrame(HEVC_CTX_S *pHevcCtx);
SINT32 HEVC_CheckThatAllRefPicsAreAvailable(HEVC_CTX_S *pHevcCtx, SINT32* piLostPoc);
SINT32 HEVC_CreateLostPicture(HEVC_CTX_S *pHevcCtx, SINT32 iLostPoc);
VOID HEVC_ApplyReferencePictureSet(HEVC_CTX_S *pHevcCtx);
VOID HEVC_RemoveApcStore(HEVC_CTX_S *pHevcCtx, UINT32 pos);
VOID HEVC_RemoveUnusedApcStore(HEVC_CTX_S *pHevcCtx);
UINT32 HEVC_RemoveUnUsedFrameStore(HEVC_CTX_S *pHevcCtx);
VOID   HEVC_RemoveFrameStoreOutDPB(HEVC_CTX_S *pHevcCtx,UINT32 pos);
VOID   HEVC_GetMinPOC(HEVC_CTX_S *pHevcCtx,SINT32 *poc, UINT32 *pos);
SINT32 HEVC_OutputFrmFromDPB(HEVC_CTX_S *pHevcCtx);
SINT32 HEVC_FlushOutputFrmFromDPB(HEVC_CTX_S *pHevcCtx);
SINT32 HEVC_EmptyFrmFromDPB(HEVC_CTX_S *pHevcCtx);
SINT32 HEVC_OutputFrmToVO(HEVC_CTX_S *pHevcCtx,HEVC_FRAMESTORE_S *pFrameStore);
SINT32 HEVC_InsertFrmInDPB(HEVC_CTX_S *pHevcCtx,UINT32 pos, HEVC_CURRPIC_S *pCurrPic);
SINT32 HEVC_GetAPC(HEVC_CTX_S *pHevcCtx,HEVC_FRAMESTORE_S *pFrameStore);
VOID   HEVC_UpdateReflist(HEVC_CTX_S *pHevcCtx);
VOID   HEVC_UpdateLTReflist(HEVC_CTX_S *pHevcCtx);
SINT32 HEVC_InitListX(HEVC_CTX_S *pHevcCtx);
VOID   HEVC_FindMinRefIdx(HEVC_CTX_S *pHevcCtx);
SINT32 HEVC_SetScalingList(HEVC_CTX_S *pHevcCtx);
VOID   HEVC_SetDefaultScalingList(HEVC_CTX_S *pHevcCtx);
VOID   HEVC_CalScalingList(HEVC_CTX_S *pHevcCtx,SINT32 *pScalingListCoef, UINT32 ListSize, UINT32 MatrixId);
SINT32 HEVC_SeiMessage(HEVC_CTX_S *pHevcCtx,HEVC_SEI_PARAM_SET_S *pSEI, HEVC_SEQ_PARAM_SET_S *pSPS);
SINT32 HEVC_Sei_Payload(HEVC_CTX_S *pHevcCtx,HEVC_SEI_PARAM_SET_S *pSEI, HEVC_SEQ_PARAM_SET_S *pSPS, UINT32 payloadType, UINT32 payloadSize);
UINT32 HEVC_Sei_More_Data_In_Payload( HEVC_CTX_S *pHevcCtx,UINT32 payloadSize);
UINT32 HEVC_Sei_Payload_Extension_Present( HEVC_CTX_S *pHevcCtx,UINT32 payloadSize);
SINT32 HEVC_Sei_Decoded_Picture_Hash( HEVC_CTX_S *pHevcCtx,HEVC_SEI_DECODED_PICTURE_HASH_S *pDec);
SINT32 HEVC_Sei_Scalable_Nesting(HEVC_CTX_S *pHevcCtx,HEVC_SEI_PARAM_SET_S *pSEI, HEVC_SEQ_PARAM_SET_S *pSPS, UINT32 payloadSize);
SINT32 HEVC_Sei_Structure_Of_Pictures_Info(HEVC_CTX_S *pHevcCtx,HEVC_SEI_SOP_INFO_S *pSop, UINT32 payloadSize);
SINT32 HEVC_Sei_Tone_Mapping_Info(HEVC_CTX_S *pHevcCtx,HEVC_SEI_TONE_MAPPING_INFO_S *pTon);
SINT32 HEVC_Sei_Region_Refresh_Info(HEVC_CTX_S *pHevcCtx,HEVC_SEI_REGION_REFRESH_INFO_S *pReg);
SINT32 HEVC_Sei_Temporal_Sub_Layer_Zero_Index(HEVC_CTX_S *pHevcCtx,HEVC_SEI_TEMPORAL_SUB_LAYER_S *pTem);
SINT32 HEVC_Sei_Display_Orientation(HEVC_CTX_S *pHevcCtx,HEVC_SEI_DISPLAY_ORIENTATION_S *pDis);
SINT32 HEVC_Sei_Frame_Packing_Arrangement(HEVC_CTX_S *pHevcCtx,HEVC_SEI_FRAME_PACKING_ARRANGEMENT_S *pFrm);
SINT32 HEVC_Sei_Recovery_Point(HEVC_CTX_S *pHevcCtx,HEVC_SEI_RECOVERY_POINT_S *pRec);
SINT32 HEVC_Sei_Dec_Unit_Info(HEVC_CTX_S *pHevcCtx,HEVC_SEI_DECODEING_UNIT_INFO_S *pDec, HEVC_HRD_PARAM_SET_S *pHrd);
SINT32 HEVC_Sei_Active_Param_sets(HEVC_CTX_S *pHevcCtx,HEVC_SEI_ACTIVE_PARAMETER_SET_S *pAct);
SINT32 HEVC_Sei_Buf_Period(HEVC_CTX_S *pHevcCtx,HEVC_SEI_BUF_PERIOD_S *pBuf, HEVC_HRD_PARAM_SET_S *pHrd);
SINT32 HEVC_Sei_Pic_Timming(HEVC_CTX_S *pHevcCtx,HEVC_SEI_PIC_TIMMING_S *pPic, HEVC_VUI_PARAM_SET_S *pVui);
SINT32 HEVC_Sei_User_Data_Unregistered(HEVC_CTX_S *pHevcCtx,HEVC_SEI_USER_DATA_UNREG_S *pData, UINT32 payloadSize);
VOID   HEVC_CalcStreamBits(HEVC_CTX_S *pHevcCtx);
SINT32 HEVC_StreamAddr_ReW(HEVC_CTX_S *pHevcCtx,UINT32 new_stream_base_addr, UINT32 old_stream_base_addr);
SINT32 HEVC_ReadByteAlignment(HEVC_CTX_S *pHevcCtx);
UINT8  HEVC_GetRapPicFlag(HEVC_CTX_S *pHevcCtx);
UINT8  HEVC_GetIdrPicFlag(HEVC_CTX_S *pHevcCtx);
SINT32 HEVC_FindTrailZeros(HEVC_CTX_S *pHevcCtx);
SINT32 HEVC_ReceivePacket(HEVC_CTX_S *pHevcCtx,HEVC_STREAM_PACKET *pPacket);
VOID   HEVC_CombinePacket(HEVC_CTX_S *pHevcCtx);
VOID   HEVC_ReleaseNAL(SINT32 ChanID,HEVC_NALU_S *p_nal);
SINT32 HEVC_CheckFrameStore(HEVC_CTX_S *pHevcCtx, HEVC_FRAMESTORE_S *pFrameStore);
VOID   HEVC_GetImagePara(HEVC_CTX_S *pHevcCtx,HEVC_FRAMESTORE_S *pFrameStore);
SINT32 HEVC_DECRecycleImage(HEVC_CTX_S *pHevcCtx,UINT32 PicID);
SINT32 HEVC_CheckOutputImage(HEVC_CTX_S *pHevcCtx,UINT32 PicID);
VOID   Hevc_InitDecBuffers(HEVC_CTX_S *pHevcCtx);
UINT32 HEVC_u_v(BS *pBs,SINT32 v, SINT8 *Name);
UINT32 HEVC_f_v(BS *pBs,SINT32 v, SINT8 *Name);
UINT32 HEVC_ue_v(BS *pBs, SINT8 *Name);
SINT32 HEVC_se_v(BS *pBs, SINT8 *Name);
SINT32 HEVCDEC_GetImageBuffer( HEVC_CTX_S *pHevcCtx );
SINT32 HEVC_IsNewPicNal( SINT32 *pNewPicDetector, UINT8 *pu8 );
VOID   HEVCDEC_Exit(VOID);
UINT32 HEVCDEC_VDMPostProc( HEVC_CTX_S *pHevcCtx, SINT32 ErrRatio, SINT32 CalledByISR,LUMA_INFO_S *pLumaInfo,SINT32 ModuleLowlyEnable,SINT32 VdhId);
SINT32 HEVC_IsOutDPB(HEVC_CTX_S *pHevcCtx,HEVC_FRAMESTORE_S  *pFrameStore);
SINT32 HEVC_ClearCurrNal(HEVC_CTX_S *pHevcCtx);
SINT32 HEVC_ClearDPB(HEVC_CTX_S *pHevcCtx);
UINT32 HEVC_FrameStoreRefFlg(const HEVC_FRAMESTORE_S *pFS);
SINT32 HEVC_ClearAllSlice(HEVC_CTX_S *pHevcCtx);
SINT32 HEVC_ClearCurrSlice(HEVC_CTX_S *pHevcCtx);
SINT32 HEVC_ClearAllNal(HEVC_CTX_S *pHevcCtx);
SINT32 HEVC_ClearCurrPic(HEVC_CTX_S *pHevcCtx);
SINT32 HEVC_FlushDPB(HEVC_CTX_S *pHevcCtx);
VOID   HEVC_UnMarkFrameStoreRef(HEVC_CTX_S *pHevcCtx, HEVC_FRAMESTORE_S *pFrameStore);
SINT32 HEVC_OutputOneFrmFromDPB(HEVC_CTX_S *pHevcCtx, UINT32 pos);
SINT32 HEVC_HaveSliceToDec(HEVC_CTX_S *pHevcCtx);
SINT32 HEVC_InquireSliceProperty(HEVC_CTX_S *pHevcCtx, SINT32 *IsNewPic, SINT32 *IsSizeChanged);
SINT32 HEVC_GetPicStreamSize(HEVC_CTX_S *pHevcCtx);
SINT32 HEVCDEC_GetRemainImg(HEVC_CTX_S *pHevcCtx);
SINT32 HEVCDEC_GetImageWidth(HEVC_CTX_S *pHevcCtx);
SINT32 HEVCDEC_GetImageHeight(HEVC_CTX_S *pHevcCtx);
#ifdef __cplusplus
}
#endif

#endif 
