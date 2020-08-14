#ifndef __MVC_H__
#define __MVC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "basedef.h"
#include "vfmw.h"
#include "public.h"
#include "bitstream.h"
#include "vdm_hal.h"


/************************ se err check define *********************/
#define   MVC_SE_ERR_CHECK           \
    do                               \
    {                                \
        if (pMvcCtx->SeErrFlg)      \
        {                            \
            return MVC_ERR;         \
        }                            \
    } while(0)



#define   MVC_ERR_FS_OUT_AGAIN   (-3)
#define   MVC_NOTDEC             (-2)    //表示MVC_DEC_DecodePacket()里面有一个NAL没有解码就返回了，下次调用不用送码流了
#define   MVC_ERR                (-1)
#define   MVC_OK                  0
#define   MVC_ALREADY_OUT   1
#define   MVC_OUTPUT_FAIL    2

/********************* error information define *******************/
#define   MVC_SPS_ERR            -1
#define   MVC_SUBMVC_SPS_ERR         -1
#define   MVC_SE_ERR             -1
#define   MVC_PPS_ERR            -1
#define   MVC_VLC_CODE_ERR       -1
#define   MVC_FLC_ERR            -1
#define   MVC_IDR_ERR            -1
#define   MVC_SLICE_HEADER_ERR   -1
#define   MVC_MMCO_NOT_EQUAL     -1
#define   MVC_RECOVERPOINT_ERR   -1
#define   MVC_REDUNDANTPIC_ERR   -1
#define   MVC_PIC_INITIAL_ERR    -1
#define   MVC_INITDPB_ERR        -1
#define   MVC_OUTPUT_ERR         -1

/******************* some define of start contrl ******************/
#define   MVC_START_FRAME                         0

#define   MVC_MAX_BYTES_START                     (512*1024)    // 512*1024 bytes received, start VDM to dec
#define   MVC_GET_ONE_NALU_SIZE                   (4*1024) //SCD_SEG_BLANK_LEN           // 拼包的最小长度

/***************************** define *****************************/
#define   MVC_PIC_DECODING                        1
#define   MVC_EMPTY                               0

#define   MVC_BASELINE                            66
#define   MVC_MAINPROFILE                         77
#define   MVC_EXTENDED                            88
#define   MVC_HIGHPROFILE                         100      //!< YUV 4:2:0/8 "High"
#define   MVC_HIGH10P                             110      //!< YUV 4:2:0/10 "High 10"
#define   MVC_HIGH422P                            122      //!< YUV 4:2:2/10 "High 4:2:2"
#define   MVC_HIGH444P                            144      //!< YUV 4:4:4/12 "High 4:4:4"
#define   MVC_MULTIVIEWHIGH                       118      //MVC  YUV 4:2:0
#define   STEREOHIGH                          128     //MVC  YUV 4:2:0

// define para
#define   MVC_FRAME                               0
#define   MVC_TOP_FIELD                           1
#define   MVC_BOTTOM_FIELD                        2
#define   MVC_FIELD_PAIR                          3

#define   MVC_P_SLICE                             0
#define   MVC_B_SLICE                             1
#define   MVC_I_SLICE                             2

#define   MVC_MAX_DPB_LEN                         33
#define   MVC_MAX_LIST_SIZE                       33
#define   MVC_MAX_FRAME_STORE                     40   // Maximun vdm slot
#define   MVC_MAX_PMV_STORE                       18   // 16 for max dpb + 1 for dec_pic+ 1 for interview pic
#define   MVC_VO_HOLD_FRAME_NUM                   3    // vo hold the number of frames
#define   MVC_MAX_MMCO_LEN                        100
#define   MVC_MAX_NUM_VIEWS                       2     //最大支持view数

#define   MVC_NALU_TYPE_SLICE      1
#define   MVC_NALU_TYPE_DPA        2
#define   MVC_NALU_TYPE_DPB        3
#define   MVC_NALU_TYPE_DPC        4
#define   MVC_NALU_TYPE_IDR        5
#define   MVC_NALU_TYPE_SEI        6
#define   MVC_NALU_TYPE_SPS        7
#define   MVC_NALU_TYPE_PPS        8
#define   MVC_NALU_TYPE_AUD        9
#define   MVC_NALU_TYPE_EOSEQ     10
#define   MVC_NALU_TYPE_EOSTREAM  11
#define   MVC_NALU_TYPE_FILL      12
#define   MVC_NALU_TYPE_SPSEXT    13
#define   MVC_NALU_TYPE_PREFIX    14               //mvc,base view nal mvc info
#define   MVC_NALU_TYPE_SUBSPS    15               //mvc,non base view sequence header info
#define   MVC_NALU_TYPE_AUX       19
#define   MVC_NALU_TYPE_SLICEEXT  20             //mvc,non base view slice info
#define   MVC_NALU_TYPE_EOPIC     30              // end curr pic, followed by 0x4853504943454E44
#define   MVC_NALU_TYPE_TSTEND    31              // end test, start last pic


/*定义解码模式*/
#define  MVC_IMODE                2               // only dec I pic
#define  MVC_IPMODE               1               // dec I pic, p pic and ref b pic, no ref b pic not dec
#define  MVC_IPBMODE              0               // all pic dec


/*for image format clean bits*/
#define   MVC_CLEAN_FRAME_TYPE                    (~3)
#define   MVC_CLEAN_VIDEO_FORMAT                  (~(7<<5))
#define   MVC_CLEAN_SOURCE_FORMAT                 (~(3<<8))
#define   MVC_CLEAN_FIELD_VALID                   (~(3<<10))
#define   MVC_CLEAN_TOP_FILED_FIRST               (~(3<<12))
#define   MVC_CLEAN_ASPECT_RATIO                  (~(7<<14))


// for mvc_ue_vld() and mvc_se_vld()
#define mvc_ue_vld(code, bits, val, zeros)            \
    do                                                \
    {                                                 \
        (bits) = ((zeros)<<1) + 1;                    \
        (val)  = ((code)>>(32-(bits))) - 1;           \
    } while(0)

#define mvc_se_vld(code, bits, val, zeros)            \
    do                                                \
    {                                                 \
        SINT32 tmp;                                   \
        (bits) = ((zeros)<<1) + 1;                    \
        tmp    = ((code)>>(32-(bits)))>>1;            \
        (val)  = ((code)>>(32-(bits)))&1?-1*tmp:tmp;  \
    } while(0)

/************************ structure define ************************/

typedef enum
{
    MVC_SEI_BUFFERING_PERIOD = 0,
    MVC_SEI_PIC_TIMING,
    MVC_SEI_PAN_SCAN_RECT,
    MVC_SEI_FILLER_PAYLOAD,
    MVC_SEI_USER_DATA_REGISTERED_ITU_T_T35,
    MVC_SEI_USER_DATA_UNREGISTERED,
    MVC_SEI_RECOVERY_POINT,
    MVC_SEI_DEC_REF_PIC_MARKING_REPETITION,
    MVC_SEI_SPARE_PIC,
    MVC_SEI_SCENE_INFO,
    MVC_SEI_SUB_SEQ_INFO,
    MVC_SEI_SUB_SEQ_LAYER_CHARACTERISTICS,
    MVC_SEI_SUB_SEQ_CHARACTERISTICS,
    MVC_SEI_FULL_FRAME_FREEZE,
    MVC_SEI_FULL_FRAME_FREEZE_RELEASE,
    MVC_SEI_FULL_FRAME_SNAPSHOT,
    MVC_SEI_PROGRESSIVE_REFINEMENT_SEGMENT_START,
    MVC_SEI_PROGRESSIVE_REFINEMENT_SEGMENT_END,
    MVC_SEI_MOTION_CONSTRAINED_SLICE_GROUP_SET,
    MVC_SEI_FILM_GRAIN_CHARACTERISTICS,
    MVC_SEI_DEBLOCKING_FILTER_DISPLAY_PREFERENCE,
    MVC_SEI_STEREO_VIDEO_INFO,
    MVC_SEI_FRAME_PACKING_ARRANGEMENT = 45,
    MVC_SEI_MAX_ELEMENTS  // number of maximum syntax elements
} MVC_SEI_type;

typedef struct
{
    UINT8  *alloc_stream_word_addr;
    UINT8  *align_stream_word_addr;
    UINT8  *tab_base_addr;
    UINT32  used_words;
} MVC_DEC_BitStreamBuffer;

typedef struct
{
    UINT8  *streamBuffer;       // [MAX_CODED_FRAME_SIZE], actual codebuffer for read bytes
    UINT32  bitstream_length;   // over codebuffer length, byte oriented, UVLC only
    UADDR   stream_phy_addr;    // physic addr of stream
    UINT32  valid_bitlen;       // valid bits when dec slice data
    UINT32  bitsoffset;         // offset in 32 bits when this segment stream start
    UINT32  bytespos;           // stream VDM will used start bytes ph addr, word addr aligned
    SINT32  StreamID;
} MVC_BITSTREAM_S;

typedef struct
{
    UINT8  is_valid;
    //UINT8  mvcinfo_flag;
    UINT8  non_idr_flag;
    UINT8  priority_id;
    UINT8  temporal_id;
    UINT8  anchor_pic_flag;
    UINT8  inter_view_flag;
    UINT8  reserved_one_bit;
    SINT32 view_id;
} MVC_NALUMVCEXT_S;

typedef struct
{
    UINT8   nal_used_segment;     // which segment using decode
    UINT8   is_valid;
    UINT8   nal_unit_type;
    UINT8   nal_integ;
    UINT8   nal_ref_idc;
    SINT8   svc_extension_flag;
    MVC_BITSTREAM_S  stream[2];
    UINT32  nal_bitoffset;
    UINT32  nal_segment;          // how many stream segment received in this nal
    UINT32  nal_trail_zero_bit_num;
    UINT32  forbidden_bit;
    UINT64  PTS;
} MVC_NALU_S;

typedef struct
{
    UINT8   cbr_flag[32];
    UINT8   bit_rate_scale;
    UINT8   cpb_size_scale;
    UINT32  cpb_cnt_minus1;
    UINT32  bit_rate_value_minus1[32];
    UINT32  cpb_size_value_minus1[32];
    UINT32  initial_cpb_removal_delay_length_minus1;
    UINT32  cpb_removal_delay_length_minus1;
    UINT32  dpb_output_delay_length_minus1;
    UINT32  time_offset_length;
} MVC_HRD_PARAMETERS_S;

typedef struct
{
    UINT8   aspect_ratio_info_present_flag;
    UINT8   overscan_info_present_flag;
    UINT8   overscan_appropriate_flag;
    UINT8   video_signal_type_present_flag;
    UINT8   video_full_range_flag;
    UINT8   colour_description_present_flag;
    UINT8   chroma_location_info_present_flag;
    UINT8   nal_hrd_parameters_present_flag;
    UINT8   timing_info_present_flag;
    UINT8   fixed_frame_rate_flag;
    UINT8   vcl_hrd_parameters_present_flag;
    UINT8   pic_struct_present_flag;
    UINT8   bitstream_restriction_flag;
    UINT8   motion_vectors_over_pic_boundaries_flag;
    UINT8   low_delay_hrd_flag;
    UINT8   aspect_ratio_idc;
    UINT32  sar_width;
    UINT32  sar_height;
    UINT32  aspect_ratio;
    UINT32  video_format;
    UINT32  colour_primaries;
    UINT32  transfer_characteristics;
    UINT32  matrix_coefficients;
    UINT32  chroma_sample_loc_type_top_field;
    UINT32  chroma_sample_loc_type_bottom_field;
    UINT32  num_units_in_tick;
    UINT32  time_scale;
    MVC_HRD_PARAMETERS_S  nal_hrd_parameters;
    MVC_HRD_PARAMETERS_S  vcl_hrd_parameters;
    UINT32  max_bytes_per_pic_denom;
    UINT32  max_bits_per_mb_denom;
    UINT32  log2_max_mv_length_vertical;
    UINT32  log2_max_mv_length_horizontal;
    UINT32  num_reorder_frames;
    UINT32  max_dec_frame_buffering;
} MVC_VUI_SEQ_PARAMETERS_S;

typedef struct
{
    UINT8   constrained_set0_flag;                 // u(1)
    UINT8   constrained_set1_flag;                 // u(1)
    UINT8   constrained_set2_flag;                 // u(1)
    UINT8   constrained_set3_flag;                 // u(1)
    UINT8   constrained_set4_flag;                 // u(1)
    UINT8   constrained_set5_flag;                 // u(1)
    SINT8   seq_scaling_list_present_flag[12];     // u(1)
    UINT8   delta_pic_order_always_zero_flag;      // u(1)
    UINT8   gaps_in_frame_num_value_allowed_flag;  // u(1)
    UINT8   frame_mbs_only_flag;                   // u(1)
    UINT8   mb_adaptive_frame_field_flag;          // u(1)
    UINT8   direct_8x8_inference_flag;             // u(1)
    UINT8   frame_cropping_flag;                   // u(1)
    UINT8   vui_parameters_present_flag;           // u(1)
    UINT8   is_valid;                              // the sps content of this sps_id is geted or not
    UINT8   is_refreshed;                          // 是否更新，初始值为0
    UINT8   seq_scaling_matrix_present_flag;

    MVC_VUI_SEQ_PARAMETERS_S  vui_seq_parameters;
    UINT32  profile_idc;                           // u(8)
    UINT32  level_idc;                             // u(8)
    UINT32  seq_parameter_set_id;                  // ue(v)
    UINT32  chroma_format_idc;
    SINT32  ScalingList4x4[6][16];                 // se(v)
    SINT32  ScalingList8x8[6][64];                 // se(v)
    UINT32  qmatrix4x4[6][4];                      // 16 个bytes拼在4words中
    UINT32  qmatrix8x8[2][16];                     // 64 个bytes拼在16words中
    UINT32  log2_max_frame_num_minus4;             // ue(v)
    UINT32  pic_order_cnt_type;                    // ue(v)
    UINT32  log2_max_pic_order_cnt_lsb_minus4;     // ue(v)
    SINT32  offset_for_non_ref_pic;                // se(v)
    SINT32  offset_for_top_to_bottom_field;        // se(v)
    UINT32  num_ref_frames_in_pic_order_cnt_cycle; // ue(v)
    SINT32  offset_for_ref_frame[256];             // se(v)
    UINT32  num_ref_frames;                        // ue(v)
    UINT32  pic_width_in_mbs_minus1;               // ue(v)
    UINT32  pic_height_in_map_units_minus1;        // ue(v)
    UINT32  frame_crop_left_offset;                // ue(v)
    UINT32  frame_crop_right_offset;               // ue(v)
    UINT32  frame_crop_top_offset;                 // ue(v)
    UINT32  frame_crop_bottom_offset;              // ue(v)
    UINT32  dpb_size_plus1;                        // calculate from level_idc
    UINT32  disp_width;
    UINT32  disp_height;
    UINT32  disp_center_x;
    UINT32  disp_center_y;
} MVC_SPS_S;


typedef struct
{
    UINT8   vui_mvc_temporal_id[64];
    UINT8   vui_mvc_low_delay_hrd_flag[64];
    UINT8   vui_mvc_fixed_frame_rate_flag[64];
    UINT8   vui_mvc_pic_struct_present_flag[64];
    UINT8   vui_mvc_timing_info_present_flag[64];
    UINT8   vui_mvc_nal_hrd_parameters_present_flag[64];
    UINT8   vui_mvc_vcl_hrd_parameters_present_flag[64];
    UINT32  vui_mvc_num_ops_minus1;
    UINT32  vui_mvc_num_target_output_views_minus1[64];
    UINT32  vui_mvc_view_id[64][64];
    UINT32  vui_mvc_num_units_in_tick[64];
    UINT32  vui_mvc_time_scale[64];
    MVC_HRD_PARAMETERS_S  mvc_nal_hrd_parameters[64];
    MVC_HRD_PARAMETERS_S  mvc_vcl_hrd_parameters[64];
} MVCVUI_SUBSPS_S;


typedef struct
{
    UINT8  is_valid;
    UINT8   bit_equal_to_one;
    UINT32  num_views_minus1;
    SINT32  view_id[MVC_MAX_NUM_VIEWS];
    UINT32  num_anchor_refs[2][MVC_MAX_NUM_VIEWS];
    UINT32  anchor_ref[2][MVC_MAX_NUM_VIEWS][MVC_MAX_NUM_VIEWS - 1];
    UINT32  num_non_anchor_refs[2][MVC_MAX_NUM_VIEWS];
    UINT32  non_anchor_ref[2][MVC_MAX_NUM_VIEWS][MVC_MAX_NUM_VIEWS - 1];
    UINT32  num_level_values_signalled_minus1;
    UINT32  level_idc[16];
    UINT32  num_applicable_ops_minus1[16];
    UINT32  applicable_op_temporal_id[16][64];
    UINT32  applicable_op_num_target_views_minus1[16][64];
    UINT32  applicable_op_target_view_id[16][64][64];
    UINT32  applicable_op_num_views_minus1[16][64];
    UINT32  mvc_vui_parameters_present_flag;

    MVC_SPS_S sps;
    MVCVUI_SUBSPS_S mvcvui_subseq_parameters;

} MVC_SUBSPS_S;


typedef struct
{
    UINT8   entropy_coding_mode_flag;                 // u(1)
    UINT8   pic_order_present_flag;                   // u(1)
    UINT8   weighted_pred_flag;                        // u(1)
    UINT8   deblocking_filter_control_present_flag; // u(1)
    UINT8   constrained_intra_pred_flag;             // u(1)
    SINT8   pic_scaling_list_present_flag[12];       // u(1)
    UINT8   redundant_pic_cnt_present_flag;          // u(1)
    UINT8   pic_scaling_matrix_present_flag;
    UINT8   is_valid;                               // the Pps content of this Pps_id is geted or not
    UINT8   is_refreshed;
    UINT32  pic_parameter_set_id;                   // ue(v)
    UINT32  seq_parameter_set_id;                   // ue(v)
    UINT32  num_slice_groups_minus1;                // ue(v)
    UINT32  num_ref_idx_l0_active_minus1;           // ue(v)
    UINT32  num_ref_idx_l1_active_minus1;           // ue(v)
    UINT32  weighted_bipred_idc;                    // u(2)
    SINT32  pic_init_qp_minus26;                    // se(v)
    SINT32  chroma_qp_index_offset;                 // se(v)
    SINT32  second_chroma_qp_index_offset;
    UINT32  transform_8x8_mode_flag;
    SINT32  ScalingList4x4[6][16];                  // se(v)
    SINT32  ScalingList8x8[6][64];                  // se(v)
    SINT32  UseDefaultScalingMatrix4x4Flag[6];
    SINT32  UseDefaultScalingMatrix8x8Flag[2];
    UINT32  qmatrix4x4[6][4];                       // 16 个bytes拼在4words中
    UINT32  qmatrix8x8[2][16];                      // 64 个bytes拼在16words中
} MVC_PPS_S;

typedef struct
{
    UINT8   field_pic_flag;
    UINT8   bottom_field_flag;
    UINT8   nal_unit_type;
    UINT8   nal_ref_idc;
    UINT8   anchor_pic_flag;
    UINT8   inter_view_flag;
    UINT32  seq_parameter_set_id;
    UINT32  pic_parameter_set_id;
    UINT32  frame_num;
    UINT32  idr_pic_id;
    UINT32  pic_oder_cnt_lsb;
    SINT32  delta_pic_oder_cnt_bottom;
    SINT32  delta_pic_order_cnt[2];
    UINT32  first_mb_in_slice;
    SINT32  view_id;
} MVC_PREVSLICE_S;

typedef struct
{
    UINT8   slice_type;
    UINT8   field_pic_flag;
    UINT8   bottom_field_flag;
    UINT8   nal_unit_type;
    UINT8   MbaffFrameFlag;
    UINT8   new_pic_flag;
    UINT8   direct_spatial_mv_pred_flag;
    UINT8   apply_weights_flag;
    UINT8   nal_ref_idc;
    UINT8   anchor_pic_flag;
    UINT8   inter_view_flag;
    UINT8   mvcinfo_flag;
    SINT8   svc_extension_flag;
    UINT32  pic_parameter_set_id;
    UINT32  prev_frame_num[MVC_MAX_NUM_VIEWS];
    UINT32  frame_num;
    UINT32  len;
    UINT32  valid_bitlen;
    UINT32  bit_num;
    UINT32  idr_pic_id;
    UINT32  listXsize[2];
    UINT32  first_mb_in_slice;
    UINT32  num_ref_idx_l0_active_minus1;
    UINT32  num_ref_idx_l1_active_minus1;
    UINT32  luma_log2_weight_denom;
    UINT32  chroma_log2_weight_denom;
    SINT32  wp_weight_l0_y[32];
    SINT32  wp_weight_l0_u[32];
    SINT32  wp_weight_l0_v[32];
    SINT32  wp_weight_l1_y[32];
    SINT32  wp_weight_l1_u[32];
    SINT32  wp_weight_l1_v[32];
    SINT32  wp_offset_l0_y[32];
    SINT32  wp_offset_l0_u[32];
    SINT32  wp_offset_l0_v[32];
    SINT32  wp_offset_l1_y[32];
    SINT32  wp_offset_l1_u[32];
    SINT32  wp_offset_l1_v[32];
    UINT32  cabac_init_idc;
    SINT32  slice_qp_delta;
    UINT32  disable_deblocking_filter_idc;
    SINT32  slice_alpha_c0_offset_div2;
    SINT32  slice_beta_offset_div2;
    SINT32  picid_refidx0; // 当前slice在list0中第一个元素对应的帧存槽位号，负数无效
    SINT32  picid_nearpoc; // 当前slice在dpb中找到的poc距离最近的帧存的槽位号，负数无效

    SINT32  view_id;
    UINT32  voidx;
    UINT32  interviewlistXsize[2];
} MVC_SLICE_S;

typedef struct
{
    SINT8   last_has_mmco_5;
    SINT8   last_pic_bottom_field;
    /* for POC mode 0:*/
    UINT32  pic_order_cnt_lsb;
    SINT32  delta_pic_order_cnt_bottom;
    SINT32  PrevPicOrderCntMsb;
    UINT32  PrevPicOrderCntLsb;
    SINT32  PicOrderCntMsb;
    SINT32  toppoc;
    SINT32  bottompoc;
    SINT32  framepoc;
    SINT32  ThisPOC;
    /* for POC mode 1:*/
    SINT32  delta_pic_order_cnt[2];
    UINT32  AbsFrameNum;
    UINT32  PreviousFrameNum;
    UINT32  frame_num;
    UINT32  FrameNumOffset;
    SINT32  PrevFrameNumOffset;
    /* for POC mode 2:*/
    SINT32  FrameNumInPicOrderCntCycle;
    SINT32  PicOrderCntCycleCnt;
    SINT32  ExpectedPicOrderCnt;
    SINT32  ExpectedDeltaPerPicOrderCntCycle;
} MVC_POC_S;

typedef struct
{
    UINT8   ref_pic_list_reordering_flag_l0;
    UINT8   ref_pic_list_reordering_flag_l1;
    UINT32  reordering_of_pic_nums_idc_l0[33];
    UINT32  reordering_of_pic_nums_idc_l1[33];
    UINT32  abs_diff_pic_num_minus1_l0[33];
    UINT32  abs_diff_pic_num_minus1_l1[33];
    UINT32  long_term_pic_idx_l0[32];
    UINT32  long_term_pic_idx_l1[32];
    UINT32  abs_diff_view_idx_minus1_l0[33];
    UINT32  abs_diff_view_idx_minus1_l1[33];
} MVC_REORDER_S;

typedef struct
{
    UINT32  memory_management_control_operation;
    UINT32  difference_of_pic_nums_minus1;
    UINT32  long_term_pic_num;
    UINT32  long_term_frame_idx;
    UINT32  max_long_term_frame_idx_plus1;
} MVC_MMCO_S;

typedef struct
{
    UINT8   is_valid;
    UINT8   no_output_of_prior_pics_flag;
    UINT8   long_term_reference_flag;
    UINT8   adaptive_ref_pic_marking_mode_flag;
    MVC_MMCO_S  MMCO[MVC_MAX_MMCO_LEN];
} MVC_MARK_S;

typedef struct
{
    SINT8  frame_packing_dec_ok;
    SINT8  frame_packing_arrangement_cancel_flag;
    SINT8  frame_packing_arrangement_extension_flag;
    SINT8  quincunx_sampling_flag;
    SINT8  spatial_flipping_flag;
    SINT8  frame0_flipped_flag;
    SINT8  field_views_flag;
    SINT8  current_frame_is_frame0_flag;
    SINT8  frame0_self_contained_flag;
    SINT8  frame1_self_contained_flag;
    SINT8  frame0_grid_position_x;
    SINT8  frame0_grid_position_y;
    SINT8  frame1_grid_position_x;
    SINT8  frame1_grid_position_y;
    SINT32 frame_packing_arrangement_type;
    SINT32 content_interpretation_type;
    SINT32 frame_packing_arrangement_id;
    SINT32 frame_packing_arrangement_reserved_byte;
    SINT32 frame_packing_arrangement_repetition_period;
} MVC_FRAME_PACKING_ARRANGEMENT_S;

typedef struct
{
    UINT8  pic_struct;
    SINT32 cpb_removal_delay;
    SINT32 dpb_output_delay;
    /* 后面还有许多语法元素，暂时用不到，不列了 */
} MVC_PIC_TIMING_SEI_S;

typedef struct
{
    UINT8   structure;     // 0: MVC_FRAME, 1: MVC_TOP_FIELD, 2: bottom_field, 3: field pair
    UINT8   is_long_term;  // 0: not long term ref, 1: long term ref
    UINT8   is_short_term; // 0: not short term ref, 1: short term ref. BUT one pic cann't long and short.
    UINT8   pic_type;
    struct  MVC_FRAMESTORE_S *frame_store;
    SINT32  long_term_pic_num;
    SINT32  pic_num;
    SINT32  poc;
    SINT32  MVC_DecPOC;    //mvc规避由于last_has_mmco_5而将base view poc信息重置而找不到interview ref pic

    UINT32  err_level;
    /* BEGIN: Added by y62639, 2010/9/27 */
    SINT32  MaxQp;
    SINT32  MinQp;
    /* END:   Added by y62639, 2010/9/27 */
} MVC_STORABLEPIC_S;

typedef enum
{
    MVC_FO_STATE_NA,
    MVC_FO_STATE_FAST_OUTPUT,
    MVC_FO_STATE_NORMAL_ORDER_OUTPUT,
    MVC_FO_STATE_BUTT,
} MVC_FO_STATE_E;

typedef struct MVC_FRAMESTORE_S
{
    UINT8   pic_type;
    UINT8   non_existing;   // 1: 该fs中图像不存在, 0: 存在
    UINT8   is_used;        // 0: MVC_EMPTY, 1: top, 2: bottom, 3: both fields (or MVC_FRAME)--output
    UINT8   is_reference;   // 0: not used for ref, 1: top used, 2: bottom used,
    UINT8   mode_skip_flag; // 0: all dec, 1: top not dec, 2: bottom not dec, 3: both fields (or MVC_FRAME) not dec
    UINT8   is_in_dpb;       // 已经出了DPB, 0: already out from dpb, 1: still in dpb
    SINT8   logic_fs_id;    /*  */
    UINT8   is_displayed;    // VO 是否显示，0: not display; 1: displayed
    SINT8   as_first_out;    //mvc为了假装第一帧快速输出，同时可以判断IDRFOUND的置位时机
    SINT8   svc_extension_flag;
    UINT8   anchor_pic_flag[2];
    UINT8   inter_view_flag[2];
    MVC_FO_STATE_E  eFoState;  // 处理第一帧快速输出时使用

    UINT32  frame_num;
    SINT32  frame_num_wrap;
    UINT32  long_term_frame_idx;
    SINT32  poc;
    SINT32  MVC_DecPOC;           //mvc规避由于last_has_mmco_5而将base view poc信息重置而找不到interview ref pic
    UINT32  MbaffFrameFlag;
    UINT32  err_level;
    UINT32  pmv_address_idc;
    UINT32  apc_idc;
    SINT32  view_id;
    UINT32  voidx;
    UINT32  dpb_cnt;

    IMAGE              fs_image;
    MVC_STORABLEPIC_S  frame;
    MVC_STORABLEPIC_S  top_field;
    MVC_STORABLEPIC_S  bottom_field;
} MVC_FRAMESTORE_S;

typedef struct
{
    UINT8   state;
    UINT8   field_pair_flag;
    UINT8   non_existing;  // 0：当前图像存在, 1：当前图像不存在. 主要用于Frame_num gap的情况下.
    UINT8   structure;     // 0: MVC_FRAME, 1: MVC_TOP_FIELD, 2: bottom_field, 3: reserved
    UINT8   is_long_term;  // 0: not long term ref,  1: long term ref
    UINT8   is_short_term; // 0: not short term ref, 1: short term ref. BUT one pic cann't long and short.
    UINT8   nal_unit_type;
    UINT8   mode_skip_flag;
    UINT8   entropy_coding_mode_flag;
    UINT8   MbaffFrameFlag;
    //    UINT8   FrameStoreFlag;
    UINT8   anchor_pic_flag;
    UINT8   inter_view_flag;
    UINT8   pic_type;
    UINT8   is_idr_pic;

    IMAGE   fs_image;

    MVC_FRAMESTORE_S *frame_store;
    MVC_MARK_S       *pMarkPara;

    UINT32  frame_num;
    UINT32  long_term_frame_idx;
    UINT32  nal_ref_idc;
    SINT32  thispoc;
    SINT32  framepoc;
    SINT32  toppoc;
    SINT32  bottompoc;
    SINT32  decthispoc;    //mvc规避由于last_has_mmco_5而将base view poc信息重置而找不到interview ref pic
    SINT32  decframepoc;
    SINT32  dectoppoc;
    SINT32  decbottompoc;

    UINT32  err_level;
    UINT32  pic_width_in_mb;
    UINT32  pic_height_in_mb;
    UINT32  frame_height_in_mb;
    UINT32  pic_mbs;
    UADDR   stream_base_addr;
    UINT32  pmv_address_idc;
    SINT32  view_id;
    UINT32  voidx;
} MVC_CURRPIC_S;

typedef struct
{
    UINT8   structure;    // 图像结构, 0: MVC_FRAME, 1: top field, 2:bottom field, 3: rsv
    UINT8   FrameStoreFlag;
    UINT8   pic_type;
    UINT32  nal_ref_idc;
    UINT32  frame_num;
    UINT32  pic_counter;
    UINT32  image_id;
    UINT32  Dpb_pos;
    SINT32  view_id;

} MVC_PREVPIC_S;

typedef struct
{
    UINT8   is_used[16];
    UINT32  idc[16];
    UINT32  poc[2][16];
    UINT32  size;
    UINT32  used_size;
    UINT32  RefIdx[32];
} MVC_APC_S;

typedef struct
{
    MVC_FRAMESTORE_S *fs[16];
    MVC_FRAMESTORE_S *fs_ref[16];
    MVC_FRAMESTORE_S *fs_ltref[16];
    UINT32  size;
    UINT32  used_size;
    UINT32  ref_frames_in_buffer;
    UINT32  ltref_frames_in_buffer;
    UINT32  max_long_term_pic_idx;
} MVC_DPB_S;

typedef struct
{
    SINT32 poc;
    SINT32 view_id;
    UINT32 voidx;
    UINT32 logic_fs_id;
    IMAGE  *pImage;
} MVC_FSINFO_S;
typedef struct
{
    UINT32 logic_fs_id;
    IMAGE  *pImage;
} MVC_OUTVIEW_S;

typedef struct
{
    UINT32 is_used[16];
    UINT32 is_judge[16];

    UINT32 size;
    MVC_FSINFO_S fsinfo[16];

    SINT32 curr_min_poc;
    UINT32 num_views;
    UINT32 view_count;
    MVC_OUTVIEW_S   view_buffer[MVC_MAX_NUM_VIEWS];
} MVC_OUTFRMBUFFER_S;

typedef struct
{
    MVC_NALU_S *pSliceNal;
} MVC_SLCINFO_S;  //Slice信息, 主要是记录未释放的码流

typedef struct
{
    UINT8   last_pack_in_nal;
    UINT8   *p_stream;
    UADDR   strm_phy_addr;
    SINT32  stream_len;
    SINT32  StreamID;
    UINT64  pts;
} MVC_STREAM_PACKET;

typedef enum
{
    MVC_FIRST_DISP_INIT = 0,
    MVC_FIRST_DISP_UNPAIRED,
    MVC_FIRST_DISP_OUTPUT
} MVC_FIRSTDISPSTATE_E;

typedef struct
{
    UINT8    StartMode;  // 0:Normal Mode, 1: Flush Mode
    UINT8    IsStreamEndFlag;
    UINT8    Init_Subsps_Flag;
    UINT8    CurrMarkIdx;
    UINT8    AllowStartDec;   // 0: find recover point, when recover point found and decode right this flag =1;
    UINT8    HaveNalNotDecoded;
    UINT8    IDRFound;          // 0: to find I/IDR, 1: I/IDR found
    UINT8    PPicFound;
    UINT8    OldDecMode;
    UINT8    IModeOutFlag;
    UINT8    SeErrFlg;
    UINT8    PrevPicIsIDR;

    SINT32   CurWidthInMb;
    SINT32   CurHeightInMb;
    UINT32   TotalSUBSPS;
    UINT32   CurrSPSID;  //SPS index number
    UINT32   CurrSUBSPSID; //SUBSPS index number
    SINT32   MaxSliceNum;
    SINT32   MaxSpsNum;
    SINT32   MaxPpsNum;
    UINT32   CurrDPBSize;
    UINT32   TotalFsNum;      // change while g_DPB.size change
    UINT32   TotalPmvNum;     // change while g_DPB.size change
    UINT32   frame_rate;
    UINT32   TotalUsrDatNum;
    UINT32   SliceParaNum;
    UINT32   TotalNal;
    UINT32   TotalSPS ;
    UINT32   TotalPPS;
    UINT32   TotalSlice;
    UINT32   SeqCnt;
    UINT32   TotalPicNum;
    UINT32   TotalFrameNum;
    UINT32   TotalFieldNum;
    UINT32   SliceNumInPic;
    UINT32   MaxBytesReceived;
    UINT32   advanced_cfg;
    UINT32   ref_error_thr;
    UINT32   out_error_thr;
    SINT32   ChanID;
    UINT32   PrevStorePicCounter;
    UINT32   PicCounter;
    UINT32   NewPicCounter;
    UINT32   PicOutCounter;
    SINT32   numTotalFrame;
    SINT32   numOutputFrame;
    UINT32   PmvStoreUsedFlag[MVC_MAX_PMV_STORE]; // pmv store memory allocate statistic

    MVC_FIRSTDISPSTATE_E     FirstFrameDisplayed;

    SYNTAX_EXTRA_DATA_S     *pstExtraData;
    MVC_DEC_PIC_PARAM_S     *pMvcDecParam;
    MVC_NALU_S              *pCurrNal;
    MVC_SPS_S               *pCurrSPS; //may be from SPS or from SUBSPS.sps
    MVC_SPS_S               *pSPS;       //needed in decoding slice head info
    MVC_PPS_S               *pPPS;
    MVC_SPS_S               *SPS;
    MVC_PPS_S               *PPS;
    MVC_STORABLEPIC_S       *pListX[2][MVC_MAX_LIST_SIZE];
    MVC_FRAMESTORE_S        *pDirectOutBuf;
    MVC_FRAMESTORE_S        *pLastFieldBuf;
    VDEC_USRDAT_S           *pUsrDatArray[4];
    MVC_DEC_SLICE_PARAM_S   *DecSlicePara;

    BS                       BS;  // for bstream.c中的调用之用
    IMAGE_VO_QUEUE           ImageQue;
    MVC_NALU_S               NalArray[MAX_SLICE_SLOT_NUM + 1];
    MVC_NALUMVCEXT_S         CurrNalMvcInfo;
    MVC_SUBSPS_S             SUBSPS[32];
    MVC_SUBSPS_S             CurrSUBSPS;
    MVC_FRAMESTORE_S         InterviewListX[2][MVC_MAX_NUM_VIEWS];
    MVC_SPS_S                CurrSPS;
    MVC_PPS_S                CurrPPS;
    MVC_POC_S                CurrPOC;
    MVC_REORDER_S            CurrReorder;
    MVC_SLICE_S              CurrSlice;
    MVC_PREVSLICE_S          PrevSlice;
    MVC_MARK_S               Mark[2];
    MVC_FRAME_PACKING_ARRANGEMENT_S  FramePackSEI;
    MVC_PIC_TIMING_SEI_S     PicTimingSEI;
    MVC_DPB_S                DPB;
    MVC_APC_S                APC;
    MVC_FRAMESTORE_S         FrameStore[MVC_MAX_FRAME_STORE];
    MVC_CURRPIC_S            CurrPic;
    MVC_CURRPIC_S            LastPic;
    MVC_PREVPIC_S            PrevPic;
    MVC_SLCINFO_S            SlcInfo[MAX_SLICE_SLOT_NUM];     //Slice信息, 主要是记录未释放的码流
    MVC_DEC_PIC_PARAM_S      DecPicPara;
    UINT32     u32SCDWrapAround; //l00273086
    MVC_NALU_S *pLastNal;//l00273086
    UINT32     FakeFrmHeightInMb;
    UINT32     FakeFrmWidthInMb;
} MVC_CTX_S;


/*************************** func declare *************************/
SINT32 MVC_FindTrailZeros(MVC_CTX_S *pMvcCtx);
SINT32 MVC_RecivePacket(MVC_CTX_S *pMvcCtx, MVC_STREAM_PACKET *pPacket);
VOID   MVC_CombinePacket(MVC_CTX_S *pMvcCtx);
SINT32 MVC_FindNaluArraySlot(MVC_CTX_S *pMvcCtx);
SINT32 MVC_DecOneNal(MVC_CTX_S *pMvcCtx, UINT32 NalMode);
SINT32 MVC_DecSPS(MVC_CTX_S *pMvcCtx);
SINT32 MVC_ProcessSPS(MVC_CTX_S *pMvcCtx, MVC_SPS_S *pSPS);
SINT32 mvc_vui_parameters(MVC_CTX_S *pMvcCtx, MVC_VUI_SEQ_PARAMETERS_S *pVUI);
SINT32 MVC_SPSEqual(MVC_SPS_S *pSPS1, MVC_SPS_S *pSPS2);
SINT32 MVC_DecPPS(MVC_CTX_S *pMvcCtx);
SINT32 MVC_ProcessPPS(MVC_CTX_S *pMvcCtx, MVC_PPS_S *pPPS);
SINT32 MVC_PPSEqual(MVC_PPS_S *pPPS1, MVC_PPS_S *pPPS2);
SINT32 MVC_DecSEI(MVC_CTX_S *pMvcCtx);
SINT32 MVC_SliceCheck(MVC_CTX_S *pMvcCtx);
SINT32 MVC_DecSlice(MVC_CTX_S *pMvcCtx);
SINT32 MVC_ProcessSliceHeaderFirstPart(MVC_CTX_S *pMvcCtx);
SINT32 MVC_ProcessSliceHeaderSecondPart(MVC_CTX_S *pMvcCtx);
SINT32 mvc_ref_pic_list_reordering(MVC_CTX_S *pMvcCtx);
SINT32 mvc_pred_weight_table(MVC_CTX_S *pMvcCtx);
SINT32 mvc_dec_ref_pic_marking(MVC_CTX_S *pMvcCtx);
SINT32 MVC_DecMMCO(MVC_CTX_S *pMvcCtx, UINT32 save);
SINT32 MVC_PPSSPSCheck(MVC_CTX_S *pMvcCtx);
SINT32 MVC_PPSSPSCheckTmpId(MVC_CTX_S *pMvcCtx, UINT32 tmp_pic_parameter_set_id);
UINT32 MVC_IsNewPic(MVC_CTX_S *pMvcCtx);
VOID   MVC_PicTypeStatistic(MVC_CTX_S *pMvcCtx);
VOID   MVC_CalcPicNum(MVC_CTX_S *pMvcCtx);
VOID   MVC_CalcStreamBits(MVC_CTX_S *pMvcCtx);
VOID   MVC_ExitSlice(MVC_CTX_S *pMvcCtx);
SINT32 MVC_DecList(MVC_CTX_S *pMvcCtx);
SINT32 MVC_WritePicMsg(MVC_CTX_S *pMvcCtx);
VOID   MVC_WriteSliceMsg(MVC_CTX_S *pMvcCtx);
SINT32 MVC_DecList(MVC_CTX_S *pMvcCtx);
SINT32 MVC_DecVDM(MVC_CTX_S *pMvcCtx);
VOID   MVC_InitOldSlice(MVC_CTX_S *pMvcCtx);
SINT32 MVC_InitListX(MVC_CTX_S *pMvcCtx);
VOID   MVC_ReorderListX(MVC_CTX_S *pMvcCtx);
SINT32 MVC_RepairList(MVC_CTX_S *pMvcCtx);
SINT32 MVC_StorePicInDpb(MVC_CTX_S *pMvcCtx);
SINT32 MVC_InitPic(MVC_CTX_S *pMvcCtx);
VOID   MVC_ReleaseNAL(SINT32 ChannelId, MVC_NALU_S *p_nal);
VOID   MVC_FlushDecoder(MVC_CTX_S *pMvcCtx);
SINT32 MVC_IsNewPicNal( SINT32 *pNewPicDetector, UINT8 *pu8 );
SINT32 MVC_IsRefListWrong(MVC_CTX_S *pMvcCtx);
SINT32 MVC_FlushDPB(MVC_CTX_S *pMvcCtx, SINT32 CurrViewId);
SINT32 MVC_InitDPB(MVC_CTX_S *pMvcCtx);

SINT32 MVC_DEC_Init(MVC_CTX_S *pMvcCtx, SYNTAX_EXTRA_DATA_S *pstExtraData);
VOID   MVC_DEC_Destroy(MVC_CTX_S *pMvcCtx);
SINT32 MVC_DEC_DecodePacket(MVC_CTX_S *pMvcCtx, MVC_STREAM_PACKET *pPacket);
SINT32 MVC_DEC_RecycleImage(MVC_CTX_S *pMvcCtx, UINT32 PicID);
UINT32 MVC_DEC_VDMPostProc( MVC_CTX_S *pMvcCtx, SINT32 ErrRatio, SINT32 CalledByISR );
SINT32 MVC_DEC_GetImageBuffer( MVC_CTX_S *pMvcCtx );
SINT32 MVC_DEC_GetRemainImg(MVC_CTX_S *pMvcCtx);
SINT32 MVC_ClearCurrPic(MVC_CTX_S *pMvcCtx);
SINT32 MVC_DecSubSPS(MVC_CTX_S *pMvcCtx);
SINT32 MVC_ProcessSUBSPSMvcExt(MVC_CTX_S *pMvcCtx, MVC_SUBSPS_S *pSubsps);
SINT32 MVC_ProcessSUBSPSMvcVuiExt(MVC_CTX_S *pMvcCtx, MVC_SUBSPS_S *pSubsps);
SINT32 MVC_FreeMvcExtMem(MVC_SUBSPS_S *pSubsps);
SINT32 MVC_FreeMvcVuiExtMem(MVCVUI_SUBSPS_S *pMvc_vui_parameter);
VOID    mvc_combine_scalinglist(UINT32 *scalingList_Org, UINT32 *scalingList, SINT32 sizeOfScalingList );
SINT32  mvc_assign_quant_params(MVC_SPS_S *pSPS, MVC_PPS_S *pPPS);
SINT32  MVC_GetBaseViewId(MVC_CTX_S *pMvcCtx);
SINT32 MVC_GetVOIdx(SINT32 *ref_view_id, UINT32 num_views, SINT32 CurrViewId);
SINT32  mvc_is_view_id_in_ref_view_list(SINT32 view_id, SINT32 *ref_view_id, UINT32 num_ref_views);
VOID MVC_GenPiclistfromFrmlist_Interview(UINT32 currStrcture, MVC_FRAMESTORE_S *fs_list,
        UINT32 list_idx, MVC_STORABLEPIC_S **list, UINT32 *list_size);
VOID mvc_append_interview_list(MVC_CTX_S *pMvcCtx, UINT32 listidx, 	MVC_FRAMESTORE_S *list, UINT32 *listXsize);
SINT32 MVC_GetMaxViewIdx (MVC_CTX_S *pMvcCtx, SINT32 CurrViewId, UINT32 anchor_pic_flag, int listidx);
MVC_STORABLEPIC_S  *mvc_get_inter_view_pic(MVC_CTX_S *pMvcCtx, SINT32 targetViewID, SINT32 currPOC, UINT32 listidx);
VOID mvc_reorder_interview(MVC_CTX_S *pMvcCtx, UINT32 num_ref_idx_lX_active_minus1, UINT32 *refIdxLX, SINT32 targetViewID, SINT32 currpoc, UINT32 listidx);
VOID MVC_ReorderRefPiclist(MVC_CTX_S *pMvcCtx, UINT32 listidx, UINT32 num_ref_idx_lX_active_minus1);
VOID MVC_ReorderListX(MVC_CTX_S *pMvcCtx);
SINT32 MVC_NonLongTermRefFlg(const MVC_FRAMESTORE_S *pFS);
SINT32 MVC_RoundLog2 (SINT32 iValue);


#ifdef __cplusplus
}
#endif


#endif  // __MVC_H__
