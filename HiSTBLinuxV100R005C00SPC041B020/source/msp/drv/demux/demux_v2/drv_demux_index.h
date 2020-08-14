
/***********************************************************************
*
* Copyright (c) 2006 Hisilicon - All Rights Reserved
*
* File: $pvr_index.h$
* Date: $2009/4/3$
* Revision: $v1.0$
* Purpose: make video stream index info for trick play.
*          support multi video standards
*
* Change History:
*
* Date             Author            Change
* ====             ======            ======
* 2009/4/3         z56361            Original
*
* Dependencies:
*
************************************************************************/


#ifndef __FIDX_EXT_HEADER__
#define __FIDX_EXT_HEADER__

#include "hi_type.h"

#include "hi_unf_demux.h"

/************************************************************************/
/* constant                                                             */
/************************************************************************/
#define  FIDX_VERSION  20090804

#define  FIDX_OK       0
#define  FIDX_ERR      -1

#ifndef UINT8
#define UINT8   HI_U8
#define SINT8   HI_S8
#define UINT16  HI_U16
#define SINT16  HI_S16
#define UINT32  HI_U32
#define SINT32  HI_S32
#define UINT64  HI_U64
#define SINT64  HI_S64
#define VOID    HI_VOID
#define ULONG   HI_SIZE_T
#endif

// define the profiles
#define H265_MAIN_PROFILE
#define HM11_SYNTAX                   (1)

#if defined(H265_MAIN_PROFILE)
#define HEVC_MAX_VIDEO_PARAM_SET_ID   (16)
#define HEVC_MAX_SEQ_PARAM_SET_ID     (16)
#define HEVC_MAX_PIC_PARAM_SET_ID     (64)
#define HEVC_MAX_DPB_SIZE             (5)
#define HEVC_MAX_LONG_TERM_PICS       (5) 
#define HEVC_MAX_DELTA_POC            (5)
#endif

#define HEVC_MAX_DPB_PIC_BUF          (6)   
#define HEVC_MAX_SLICE_NUM            (512)

#define HEVC_MAX_LSB_NUM              (33)
#define HEVC_MAX_TEMPLAYER            (8)     
#define HEVC_MAX_INT                  (2147483647)    
#define HEVC_MAX_UINT                 (0xFFFFFFFFU)    

#define HEVC_FALSE		              (0)
#define HEVC_TRUE		              (1)

#if HM11_SYNTAX
#define HEVC_MAX_VPS_OP_SETS_PLUS1    (1024)
#define HEVC_MAX_VPS_NUH_RESERVED_ZERO_LAYER_ID_PLUS1  (1)
#endif

#define HEVC_MAX_FRAME_STORE                  (20)    
#define HEVC_MAX_DPB_NUM                             (17)  
#define HEVC_MAX_NUM_REF_PICS                        (16)  
#define HEVC_MAX_SLOT_NUM                            (512)
#define HEVC_MAX_TILE_COLUMNS                        (64)
#define HEVC_MAX_TILE_ROWS                           (64)
#define HEVC_LOGIC_TILE_COLUMNS_LIMIT                (20)    		 
#define HEVC_LOGIC_TILE_ROWS_LIMIT                   (22)

#define HEVC_MAX_CU_DEPTH     (7)    
#define HEVC_MAX_CU_SIZE        (1<<(HEVC_MAX_CU_DEPTH))   

#define HEVC_SCALING_LIST_NUM                    (6)        
#define HEVC_SCALING_LIST_DC                      (16)    
#define HEVC_SCALING_LIST_START_VALUE   (8)    
#define HEVC_MAX_MATRIX_COEF_NUM           (64)    
#define HEVC_MAX_LIST_SIZE                           (16+1)

#define HEVC_SINT8_MAX_VAL	           (127)
#define HEVC_SINT8_MIN_VAL	           (-128)
#define HEVC_SINT13_MAX_VAL	           (4095)
#define HEVC_SINT13_MIN_VAL	           (-4096)

#define HEVC_I_SLICE                   (2)
#define HEVC_P_SLICE                   (1)
#define HEVC_B_SLICE                   (0)
#define HEVC_ERR_SLICE                   (3)

#define HEVC_I_FRAME                   (0)
#define HEVC_P_FRAME                   (1)
#define HEVC_B_FRAME                   (2)
#define HEVC_IDR_FRAME                   (3)
#define HEVC_ERR_FRAME                 (4)

#define   HEVC_PIC_DECODING            (1)
#define   HEVC_PIC_EMPTY               (0)
#define   MRG_MAX_NUM_CANDS            (5)

#define   HEVC_DEC_ERR                 (-1)
#define   HEVC_DEC_NORMAL              (0)

typedef struct
{
    UINT8   sub_layer_profile_present_flag[6];
    UINT8   sub_layer_level_present_flag[6];
    SINT32  general_level_idc;
}HEVC_PROFILE_TIER_LEVEL_S;

typedef enum 
{
    SCAN_ZIGZAG = 0,  
    SCAN_HOR,   
    SCAN_VER,   
    SCAN_DIAG  
}HEVC_COEFF_SCAN_TYPE;

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
    UINT8	 useTransformSkip;	  
    UINT8	 useDefaultScalingMatrixFlag [SCALING_LIST_SIZE_NUM][HEVC_SCALING_LIST_NUM];  
    UINT8	 scaling_list_pred_mode_flag[SCALING_LIST_SIZE_NUM][HEVC_SCALING_LIST_NUM]; 
    SINT32   scaling_list_dc_coef_minus8;
    SINT32   scaling_list_delta_coef;
    SINT32   scalingListDC[SCALING_LIST_SIZE_NUM][HEVC_SCALING_LIST_NUM];  
    UINT32   refMatrixId[SCALING_LIST_SIZE_NUM][HEVC_SCALING_LIST_NUM];  
    UINT32   predMatrixId[SCALING_LIST_SIZE_NUM][HEVC_SCALING_LIST_NUM];  
    SINT32	  scalingListCoef[SCALING_LIST_SIZE_NUM][HEVC_SCALING_LIST_NUM][HEVC_MAX_MATRIX_COEF_NUM];  
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
    SINT32 luma_weight_l0_flag[HEVC_MAX_NUM_REF_PICS];
    SINT32 chroma_weight_l0_flag[HEVC_MAX_NUM_REF_PICS];
    SINT32 luma_weight_l1_flag[HEVC_MAX_NUM_REF_PICS];
    SINT32 chroma_weight_l1_flag[HEVC_MAX_NUM_REF_PICS];
}HEVC_PRED_WEIGHT_TABLE;

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
}HEVC_VIDEO_PARAM_SET_S;

typedef struct 
{
    UINT8  is_refresh;
    UINT8  valid;
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
    UINT32 ctb_num_width;  
    UINT32 ctb_num_height;  
    UINT32 lt_ref_pic_poc_lsb_sps[HEVC_MAX_LSB_NUM];
    UINT32 max_dec_pic_buffering[HEVC_MAX_TEMPLAYER];
    UINT32 num_reorder_pics[HEVC_MAX_TEMPLAYER];
    UINT32 max_latency_increase[HEVC_MAX_TEMPLAYER];
    
    HEVC_PROFILE_TIER_LEVEL_S profile_tier_level;
    HEVC_SCALING_LIST scaling_list;
    HEVC_SHORT_TERM_RPSET_S short_term_ref_pic_set[64];
} HEVC_SEQ_PARAM_SET_S;

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
    HEVC_SHORT_TERM_RPSET_S short_term_ref_pic_set;
    HEVC_REF_PIC_LISTS_MODDIFICATION ref_pic_lists_modification;
}HEVC_SLICE_SEGMENT_HEADER;

typedef struct
{
    SINT32  poc;
    UINT32  pic_type;
    UINT32  is_long_term;  
    UINT32  is_short_term; 
    
    struct  HEVC_FRAMESTORE_S *frame_store;
} HEVC_STORABLEPIC_S;

typedef enum
{
    FS_NOT_USED = 0,
    FS_IN_DPB,
    FS_DISPLAY_ONLY,
} HEVC_FS_STATE_E;

typedef struct HEVC_FRAMESTORE_S
{      
    UINT8   non_existing;    
    UINT8   is_reference;  
    UINT8   is_displayed;   
    HEVC_FS_STATE_E  eFrameStoreState; 
    SINT32  poc;
    UINT32  pic_type;	
    HEVC_STORABLEPIC_S  frame;
} HEVC_FRAMESTORE_S;

typedef struct
{
    UINT8   is_ref_idc;
    UINT8   state;   
    UINT32  nal_unit_type;
    SINT32  thispoc;
    UINT32  pic_type;
    HEVC_FRAMESTORE_S *frame_store; 
} HEVC_CURRPIC_S;

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
    UINT8   dsp_check_pic_over_flag;
    UINT8   nal_used_segment;  
    UINT32  nal_bitoffset;
    UINT32  nal_segment;  
    UINT32  nal_trail_zero_bit_num; 
    UINT32  nal_unit_type;
    UINT32  nal_ref_idc;
    UINT32  forbidden_zero_bit;
    UINT32  nuh_reserved_zero_6bits;
    UINT32  nuh_temporal_id;
    UINT32  is_valid;
    UINT32  nal_integ;
} HEVC_NALU_S;

enum NalUnitType
{
    NAL_UNIT_CODED_SLICE_TRAIL_N = 0,  // 0
    NAL_UNIT_CODED_SLICE_TRAIL_R,      // 1
    
    NAL_UNIT_CODED_SLICE_TSA_N,        // 2
    NAL_UNIT_CODED_SLICE_TLA_R,        // 3  
    
    NAL_UNIT_CODED_SLICE_STSA_N,       // 4
    NAL_UNIT_CODED_SLICE_STSA_R,       // 5
    
    NAL_UNIT_CODED_SLICE_RADL_N,       // 6
    NAL_UNIT_CODED_SLICE_RADL_R,       // 7   
    
    NAL_UNIT_CODED_SLICE_RASL_N,       // 8
    NAL_UNIT_CODED_SLICE_RASL_R,       // 9   
    
    NAL_UNIT_RESERVED_VCL_N10,
    NAL_UNIT_RESERVED_VCL_R11,
    NAL_UNIT_RESERVED_VCL_N12,
    NAL_UNIT_RESERVED_VCL_R13,
    NAL_UNIT_RESERVED_VCL_N14,
    NAL_UNIT_RESERVED_VCL_R15,
    
    NAL_UNIT_CODED_SLICE_BLA_W_LP,     // 16   
    NAL_UNIT_CODED_SLICE_BLA_W_RADL,   // 17  
    NAL_UNIT_CODED_SLICE_BLA_N_LP,     // 18
    NAL_UNIT_CODED_SLICE_IDR_W_RADL,   // 19  
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
    
    NAL_UNIT_EOPIC,                    // NAL_UNIT_UNSPECIFIED_48, 
    NAL_UNIT_EOSTREAM,                 // NAL_UNIT_UNSPECIFIED_49, 
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

typedef union
{
    HEVC_VIDEO_PARAM_SET_S VpsTmp;
    HEVC_SEQ_PARAM_SET_S   SpsTmp;
    HEVC_PIC_PARAM_SET_S   PpsTmp;
} HEVC_TMP_PARAM_SET_S;

typedef struct _tag_BITSTREAM{
    UINT8  *pHead;
    UINT8  *pTail;
    UINT32  Bufa;
    UINT32  Bufb;
    SINT32  BsLen;
    SINT32  BufPos;
    SINT32  TotalPos;
    SINT32  HistoryPos;
}BS, *LPBS;

typedef struct
{
    UINT8   bNewSequence;
    UINT8   bNoOutputOfPriorPicsFlag;
    UINT8   bNoRaslOutputFlag;
    UINT8   PrevRAPIsBLA;
    SINT8   aucConvertToBit[HEVC_MAX_CU_SIZE+1];
    
    SINT32  MaxVpsNum;
    SINT32  MaxSpsNum;
    SINT32  MaxPpsNum;
    UINT32  AllowStartDec;
    SINT32  PrevSlicePoc;
    SINT32  LastSlicePoc;
    UINT32  BitDepthY;
    UINT32  BitDepthC;
    SINT32  PocRandomAccess;
    SINT32  PocCRA ;
    SINT32  LastDisplayPoc;
    UINT32  TotalSliceNum;
    UINT32  uiAddCuDepth;
    UINT32  prev_poc;
    UINT32  prev_pic_parameter_set_id;  
    UINT32  scalingListSize[4]; 
    UINT32  scalingListSizeX[4];
    UINT32  sigLastScanCG32x32[64];
    UINT32  scalingListNum[SCALING_LIST_SIZE_NUM];
    UINT32  auiSigLastScan[4][HEVC_MAX_CU_DEPTH][128*128];

    SINT32 HevcFrmCnt;
    SINT32 HevcFrmPoc;
    SINT32 HevcFrmType;
    SINT32 HevcRefNum;
    SINT32 HevcRefPoc[16];

    SINT32 Dmx_HevcFrmCnt;
    SINT32 Dmx_HevcFrmPoc;
    SINT32 Dmx_HevcFrmType;
    SINT32 Dmx_HevcRefNum;
    SINT32 Dmx_HevcRefPoc[16];
    
    BS  *pBS;
    HEVC_VIDEO_PARAM_SET_S    pVPS[16];
    HEVC_SEQ_PARAM_SET_S      pSPS[16];
    HEVC_PIC_PARAM_SET_S      pPPS[64];
    HEVC_NALU_S               *pCurrNal;
    HEVC_STORABLEPIC_S        *pListX[2][HEVC_MAX_LIST_SIZE];
    
    BS   Bs;
    HEVC_DPB_S  DPB;
    HEVC_TMP_PARAM_SET_S       TmpParam;
    HEVC_VIDEO_PARAM_SET_S     CurrVPS;
    HEVC_SEQ_PARAM_SET_S       CurrSPS;
    HEVC_SEQ_PARAM_SET_S       SEISPS;
    HEVC_PIC_PARAM_SET_S       CurrPPS;
    HEVC_CURRPIC_S             CurrPic;
    HEVC_SLICE_SEGMENT_HEADER  CurrSlice;
    HEVC_NALU_S                NalArray; 
    HEVC_FRAMESTORE_S          FrameStore[HEVC_MAX_FRAME_STORE];
}HEVC_CTX_S;

SINT32 HEVC_Init(HEVC_CTX_S *pHevcCtx);
//VOID   HEVC_InitDecPara(HEVC_CTX_S *pHevcCtx);
//VOID   HEVC_InitScalingOrderTable(HEVC_CTX_S *pHevcCtx);
//VOID   HEVC_InitSigLastScan(HEVC_CTX_S *pHevcCtx, UINT32* pBuffZ, UINT32* pBuffH, UINT32* pBuffV, UINT32* pBuffD, SINT32 iWidth, SINT32 iHeight, SINT32 iDepth);
//SINT32 *HEVC_GetScalingListDefaultAddress(UINT32 sizeId, UINT32 matrixId);
//SINT32 HEVC_DecVPS(HEVC_CTX_S *pHevcCtx);
//SINT32 HEVC_DecSPS(HEVC_CTX_S *pHevcCtx);
//SINT32 HEVC_DecPPS(HEVC_CTX_S *pHevcCtx);
//SINT32 HEVC_DecSlice(HEVC_CTX_S *pHevcCtx);
//SINT32 HEVC_DecSliceSegmentHeader(HEVC_CTX_S *pHevcCtx, UINT32 IsDecSlice);
//SINT32 HEVC_CountRef(HEVC_CTX_S *pHevcCtx);
//SINT32 HEVC_IsNewPic(HEVC_CTX_S *pHevcCtx);
//SINT32 HEVC_DecPTL(HEVC_CTX_S *pHevcCtx,HEVC_PROFILE_TIER_LEVEL_S *pPTL, SINT32 ProfilePresentFlag, SINT32 MaxNumSubLayersMinus1);
//SINT32 HEVC_DecScalingListData(HEVC_CTX_S *pHevcCtx,HEVC_SCALING_LIST *pScalingList);
//SINT32 HEVC_DecShortTermRefPicSet(HEVC_CTX_S *pHevcCtx,HEVC_SEQ_PARAM_SET_S *pSPS, HEVC_SHORT_TERM_RPSET_S *pShortTermRpset, UINT32 idx);
//SINT32 HEVC_DecRefPicListsModification(HEVC_CTX_S *pHevcCtx,HEVC_SLICE_SEGMENT_HEADER *pSlice, HEVC_REF_PIC_LISTS_MODDIFICATION *pRpListModification, SINT32 numRpsCurrTempList);
//SINT32 HEVC_DecPredWeightTable(HEVC_CTX_S *pHevcCtx);
//SINT32 HEVC_WritePicMsg(HEVC_CTX_S *pHevcCtx);
//SINT32 HEVC_WriteSliceMsg(HEVC_CTX_S *pHevcCtx);
//SINT32 HEVC_InitPic(HEVC_CTX_S *pHevcCtx);
//SINT32 HEVC_StorePicInDPB(HEVC_CTX_S *pHevcCtx);
//SINT32 HEVC_DecList(HEVC_CTX_S *pHevcCtx);
//SINT32 HEVC_AllocFrameStore(HEVC_CTX_S *pHevcCtx);
//SINT32 HEVC_CheckThatAllRefPicsAreAvailable(HEVC_CTX_S *pHevcCtx, SINT32* piLostPoc);
//VOID HEVC_ApplyReferencePictureSet(HEVC_CTX_S *pHevcCtx);
//UINT32 HEVC_RemoveUnUsedFrameStore(HEVC_CTX_S *pHevcCtx);
//VOID   HEVC_RemoveFrameStoreOutDPB(HEVC_CTX_S *pHevcCtx,UINT32 pos);
//SINT32 HEVC_InsertFrmInDPB(HEVC_CTX_S *pHevcCtx,UINT32 pos, HEVC_CURRPIC_S *pCurrPic);
//VOID   HEVC_UpdateReflist(HEVC_CTX_S *pHevcCtx);
//VOID   HEVC_UpdateLTReflist(HEVC_CTX_S *pHevcCtx);
//SINT32 HEVC_InitListX(HEVC_CTX_S *pHevcCtx);
//UINT8  HEVC_GetRapPicFlag(HEVC_CTX_S *pHevcCtx);
//UINT8  HEVC_GetIdrPicFlag(HEVC_CTX_S *pHevcCtx);
//VOID   Hevc_InitDecBuffers(HEVC_CTX_S *pHevcCtx);
//UINT32 HEVC_u_v(BS *pBs,SINT32 v, SINT8 *Name);
//UINT32 HEVC_f_v(BS *pBs,SINT32 v, SINT8 *Name);
//UINT32 HEVC_ue_v(BS *pBs, SINT8 *Name);
//SINT32 HEVC_se_v(BS *pBs, SINT8 *Name);


/************************************************************************/
/* struct & enum                                                        */
/************************************************************************/
/* video standard type */
typedef enum hi_VIDSTD_E
{
	VIDSTD_MPEG2,
	VIDSTD_MPEG4,
	VIDSTD_AVS,
    VIDSTD_H264,
    VIDSTD_VC1,
    VIDSTD_H263,
    VIDSTD_DIVX3,
	VIDSTD_AUDIO_PES,
	VIDSTD_HEVC,
    VIDSTD_BUTT
}VIDSTD_E;

/* stream data type */
typedef enum hi_STRM_TYPE_E
{
    STRM_TYPE_ES = 0,
	STRM_TYPE_PES,
	STRM_TYPE_BUTT
}STRM_TYPE_E;

/* frame type */
typedef enum hi_FIDX_FRAME_TYPE_E
{
    FIDX_FRAME_TYPE_UNKNOWN = 0,
    FIDX_FRAME_TYPE_I,
	FIDX_FRAME_TYPE_P,
	FIDX_FRAME_TYPE_B,
	FIDX_FRAME_TYPE_IDR,
	FIDX_FRAME_TYPE_PESH,
    FIDX_FRAME_TYPE_BUTT,
    FIDX_FRAME_TYPE_ERR,
	
} FIDX_FRAME_TYPE_E;

/* descriptor for frame information */
typedef struct hiFRAME_POS_S
{
	FIDX_FRAME_TYPE_E  eFrameType;
    HI_U32        u32PrePTS;
	HI_U32        u32PTS;
	HI_S64        s64GlobalOffset;
	HI_S32        s32OffsetInPacket;
	HI_S32        s32PacketCount;
	HI_S32        s32FrameSize;
	HI_U32        u32Reservd;

    /* hevc private */
    HI_S32        s32CurPoc;
    HI_S32        s32RefNum;
    HI_S32        s32RefPoc[16];
}FRAME_POS_S;

/* pvr index's SCD descriptor                                               */
typedef struct hiFINDEX_SCD_S
{
    HI_U8   u8IndexType;             /* type of index(pts,sc,pause,ts) */
    HI_U8   u8StartCode;             /* type of start code, 1Byte after 000001 */
    HI_U16  u16Reservd;    
    HI_U32  u32PtsMs;
	
    HI_S64  s64GlobalOffset;        /* start code offset in global buffer */
    HI_U8   au8DataAfterSC[8];      /* 1~8 Byte next to SC */
    HI_U32  u32ExtraSCDataSize;     /* extra data size */
    HI_U8   *pu8ExtraSCData;        /* save extra more data */
    HI_U32  u32ExtraSCDataPhyAddr;  /* extra data phy addr */
} FINDEX_SCD_S;

/*!***********************************************************************/
/*! interface functions definition                                       */
/*!***********************************************************************/

/*!***********************************************************************
@brief
	global init, clear residual information, and register call back.
 ************************************************************************/
HI_VOID FIDX_Init(HI_VOID (*OutputFramePosition)(HI_U32 *Param, FRAME_POS_S *pstScInfo));
HI_VOID FIDX_DeInit(HI_VOID);

/*!***********************************************************************
@brief
    open an instance
@param	
	VidStandard:  video standard type
@return
    if success return the instance ID, between 0 and (FIDX_MAX_CTX_NUM-1)
	otherwise return -1
 ************************************************************************/
HI_S32  FIDX_OpenInstance(VIDSTD_E VidStandard, STRM_TYPE_E StrmType, HI_U32 *Param);

/************************************************************************
@brief
    close specified instance
@param
	InstIdx:  the ID of the instance to be closed
 ************************************************************************/
HI_S32  FIDX_CloseInstance( HI_S32 InstIdx );


/*!***********************************************************************
@brief
    feed start code to FIDX.
    there are 2 method to feed necessary information to FIDX:
	1. feed stream directly. Call FIDX_MakeFrameIndex()
	2. feed start code. In this method, the start code must be scanned outside,
	   This call this function to create index.
 ************************************************************************/
HI_VOID  FIDX_FeedStartCode(
    HI_S32 InstIdx,                    /*! instance ID */
    const FINDEX_SCD_S *pstSC );      /* SCD descriptor */

/*!***********************************************************************
@brief
    to see if the SC is usful for index making
@return
    if the SC is usful return FIDX_OK, otherwise return FIDX_ERR
 ************************************************************************/
HI_S32  FIDX_IsSCUseful(HI_S32 InstIdx, HI_U8 u8StartCode);

#endif




