/**
 * This product contains one or more programs protected under international
 * and U.S. copyright laws as unpublished works.  They are confidential and
 * proprietary to Dolby Laboratories.  Their reproduction or disclosure, in
 * whole or in part, or the production of derivative works therefrom without
 * the express permission of Dolby Laboratories is prohibited.
 * Copyright 2011 - 2015 by Dolby Laboratories.
 * All rights reserved.
 *
 * @brief RPU common API.
 * @file rpu_api_common.h
 *
 * $Id$
 */

#ifndef _RPU_API_COMMON_H_
#define _RPU_API_COMMON_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "rpu_ext_config.h"

/*! @brief Macro to handle DLL import/export */
#ifdef LIBRPU_EXPORTS
/* export symbols */
#ifdef WIN32
#define LIBRPU_API __declspec(dllexport)
#else
#define LIBRPU_API __attribute__((visibility("default")))
#endif
#else
/* import symbols */
#ifdef WIN32
#ifdef LIBRPU_DLL
#define LIBRPU_API __declspec(dllimport)
#else
#define LIBRPU_API 
#endif
#else
#define LIBRPU_API
#endif
#endif /* LIBRPU_EXPORTS */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
#ifndef __cplusplus
#define inline                          __inline
#endif
#endif

/****************************************************************************
 * Defines
 ****************************************************************************/

#define VDR_RPU_SYNTAX_VERSION_MAJOR                    0           /**<@brief major syntax version */
#define VDR_RPU_SYNTAX_VERSION_MINOR                    1           /**<@brief minor syntax version */
#define VDR_RPU_SYNTAX_VERSION_PATCH                    2           /**<@brief patch syntax version */
#define RPU_FORMAT (0x07FF & ((VDR_RPU_SYNTAX_VERSION_MAJOR << 8) | (VDR_RPU_SYNTAX_VERSION_MINOR << 4) | VDR_RPU_SYNTAX_VERSION_PATCH))

#define VDR_RPU_API_VERSION_MAJOR                    0           /**<@brief major api version */
#define VDR_RPU_API_VERSION_MINOR                    1           /**<@brief minor api version */
#define VDR_RPU_API_VERSION_PATCH                    2           /**<@brief patch api version */

/*! \name Configurable define statements
*/
/*! \{ */
#define RPU_PRE_BUFFER_SIZE       1024 * 1024 /**< @brief */ /* must be a power of 2 */
#define MAX_BITSTREAM_SIZE          64 * 1024 /**< @brief */
#define VDR_RPU_NAL_TYPE                 0x19 /**< @brief */
#define NALU_STARTCODE_LENGTH               4 /**< @brief */
/*! \} */

#define MAX_RPU_ID                         15
#define MAX_LEVEL                          15
#define MAX_PROFILE                        15

#define MAX_POLYNOMIAL_ORDER                4
#define MAX_MMR_ORDER                       3
#define MAX_NUM_MAPPING_PIVOTS             16
#define MAX_NUM_NLQ_PIVOTS                 16
#define MAX_NUM_CONTENT_GAMMUT              8
#define MAX_NUM_VDR_DISPLAY_PRIMARIES       8
#define MAX_COMPONENT                       3
#define NUM_MMR_COEFF                       7
#define NUM_POW_COEFF                       5
#define MAPPING_LUT_SIZE                 1024
#define MAX_NUM_RESIDUAL_RESAMPLING_COEFF  17
#define MAX_NUM_CHROMA_FILTER_COEFF         8
#define DEFAULT_COEFFICIENT_LOG2_DENOM             23
#define DEFAULT_SPATIAL_FILTER_EXP_COEF_LOG2_DENOM 20


/*! \name Maximum number of partitions allowed for a certain level.
The decoder can allocate it's resources based on these define statements.
*/
/*! \{ */
#define MAX_NUM_X_PARTITIONS_L1          1 /**< @brief  @hideinitializer */
#define MAX_NUM_Y_PARTITIONS_L1          1 /**< @brief  @hideinitializer */
#define MAX_NUM_X_PARTITIONS_L2          4 /**< @brief  @hideinitializer */
#define MAX_NUM_Y_PARTITIONS_L2          4 /**< @brief  @hideinitializer */
#define MAX_NUM_X_PARTITIONS_L3          8 /**< @brief  @hideinitializer */
#define MAX_NUM_Y_PARTITIONS_L3          8 /**< @brief  @hideinitializer */
/*! \} */

/****************************************************************************
 * Error codes
 ****************************************************************************/
/** \name Error codes
These error codes are return values for all API functions.
*/

/*! \{ */
#define RPU_ERR_NONE                      0 /**< @brief no error                                  @hideinitializer */

/* common error codes */
#define RPU_ERR_OUT_OF_MEMORY            -1  /**< @brief there is not enough memory available for buffer allocation
                                                         this can only happen during the initialization stage        @hideinitializer */
#define RPU_ERR_INVALID_DATA             -2  /**< @brief a invalid bitstream was found                               @hideinitializer */
#define RPU_ERR_NOT_SUPPORTED            -3  /**< @brief a currently not supported feature                           @hideinitializer */
#define RPU_ERR_INVALID_LEVEL            -4  /**< @brief the rpu stream violates the level limitations               @hideinitializer */
#define RPU_ERR_INVALID_PROFILE          -5  /**< @brief the rpu stream violates the profile limitations             @hideinitializer */
#define RPU_ERR_INVALID_SPATIAL_FILTER   -6  /**< @brief the rpu stream violates the spatial filter limitations      @hideinitializer */
#define RPU_ERR_INVALID_ARGUMENT         -7  /**< @brief thrown by a function called with invalid arguments          @hideinitializer */
#define RPU_ERR_INTERNAL                 -8  /**< @brief unspecified brief error                                     @hideinitializer */
#define RPU_ERR_VERSION                  -9  /**< @brief wrong version number                                        @hideinitializer */
#define RPU_ERR_INVALID_DM_EXT_SIZE     -10  /**< @brief invalid DM metadata extension size                          @hideinitializer */
#define RPU_ERR_INVALID_DM_EXT_LEVEL    -11  /**< @brief DM metadata extension can be 1 or 2 only                    @hideinitializer */
#define RPU_ERR_OUTPUT_BUFFER_TOO_SMALL -12  /**< @brief Output buffer is too small                                  @hideinitializer */
#define RPU_ERR_SH_NOT_MATCHING_PREV_SH -13  /**< @brief RPU sequence header not matching previous sequence header   @hideinitializer */

/* rpu writer error codes */

/* rpu decoder error codes */
#define RPU_ERR_NEED_MORE_DATA           -20  /**< @brief the decoder doesn't have enough data to output a complete rpu_data unit
                                                   thrown by rpu_decoder_get_output()                                      @hideinitializer */
#define RPU_ERR_END_OF_STREAM            -21  /**< @brief the last rpu_data unit was sent   (thrown by rpu_decoder_get_output())  @hideinitializer */
#define RPU_ERR_INPUT_BUFFER_FULL        -22  /**< @brief the brief nalu input buffer is full.
                                                   this happens if the caller pushes data via rpu_decoder_push_data
                                                   to many times before he calls rpu_decoder_get_output()                  @hideinitializer */
/*! \} */

/****************************************************************************
 * Enumerations
 ****************************************************************************/

/*!
    @brief RPU profile enumeration.
*/
typedef enum
{
    RPU_BASE_PROFILE = 0, /**< @brief base profile @hideinitializer */
    RPU_MAIN_PROFILE = 1, /**< @brief main profile @hideinitializer */
    RPU_HIGH_PROFILE = 2  /**< @brief high profile @hideinitializer */
} rpu_profile_e;

/*!
    @brief RPU level enumeration.
*/
typedef enum
{
    RPU_LEVEL_1 = 0, /**< @brief level 1 @hideinitializer */
    RPU_LEVEL_2 = 1, /**< @brief level 2 @hideinitializer */
    RPU_LEVEL_3 = 2  /**< @brief level 3 @hideinitializer */
} rpu_level_e;

typedef enum
{
    RPU_CF_420 = 0,
    RPU_CF_422 = 1,
    RPU_CF_444 = 2
} rpu_chroma_format_e;

typedef enum
{
    COEF_DATA_TYPE_FIXED = 0,
    COEF_DATA_TYPE_FLOAT = 1
} rpu_coefficient_data_type_e;

typedef enum
{
    /* YCbCr */
    RPU_CHAN_Y           = 0,
    RPU_CHAN_Cb          = 1,
    RPU_CHAN_Cr          = 2,

    /* GBR */
    RPU_CHAN_G           = 0,
    RPU_CHAN_B           = 1,
    RPU_CHAN_R           = 2
} rpu_color_channel_e;

typedef enum
{
    RPU_NON_NORMALIZED = 0,  /* Non-normalized */
                             /* Input: [ 0, (2^BL_bit_depth-1) ] */
                             /* Output: [ 0, (2^vdr_bit_depth_minus8+8-1) ] */
    RPU_NORMALIZED     = 1   /* Normalized */
                             /* Input: [ 0, 1 ) */
                             /* Output: [ 0, 1 ) */
                             /* 2..3    Reserved */
} vdr_rpu_normalization_e;

typedef enum
{
    MAPPING_POLYNOMIAL = 0,  /* Nth order polynomial mapping (N>=1) */
    MAPPING_MMR        = 1,  /* MMR mapping */
    MAPPING_POWER      = 2,  /* Power function mapping */
    MAPPING_LUT        = 3   /* Look-up table mapping */
                             /* 4..15 reserved */
} mapping_idc_e;

typedef enum
{
    MAPPING_CS_YCBCR  = 0,   /* YCbCr */
    MAPPING_CS_RGB    = 1    /* RGB */
                             /* 2..15 Reserved */
} mapping_color_space_e;

typedef enum
{
    NLQ_LINEAR_DZ      = 0,  /* Linear with dead zone quantization */
    NLQ_MU_LAW         = 1,  /* mu-law non-linear quantization */
                          /* 4..7 reserved */
} nlq_method_idc_e;

typedef enum
{
    CHROMA_FILTER_F0      =    0,
    CHROMA_FILTER_F1      =    1,
    CHROMA_FILTER_F2      =    2,
    CHROMA_FILTER_F3      =    3,
    CHROMA_FILTER_F4      =    4,
    CHROMA_FILTER_F5      =    5,
    CHROMA_FILTER_F6      =    6,
    CHROMA_FILTER_F7      =    7,
                           /*  8..13   reserved */
    CHROMA_FILTER_1D_EXP  =   14,
    CHROMA_FILTER_2D_EXP  =   15
} chroma_resampling_filter_idc_e;

typedef enum
{
    EL_CHROMA_FILTER_F0      = 0,
                           /* 1..6 reserved */
    EL_CHROMA_FILTER_CC_EXP  = 7
} residual_resampling_filter_idc_e;

typedef enum
{
    SPATIAL_RESAMPLING_FILTER_H_DOUBLE_F0   = 0,
                           /* 1..6 reserved */
    SPATIAL_RESAMPLING_FILTER_H_DOUBLE_EXP  = 7
} spatial_resampling_filter_hor_idc_e;

typedef enum
{
    SPATIAL_RESAMPLING_FILTER_V_DOUBLE_F0   = 0,
    SPATIAL_RESAMPLING_FILTER_V_DOUBLE_F1   = 1,
                           /* 2..6 reserved */
    SPATIAL_RESAMPLING_FILTER_V_DOUBLE_EXP  = 7
} spatial_resampling_filter_ver_idc_e;

typedef enum
{
    SPATIAL_RESAMPLING_MODE_H_BYPASS      = 0,
    SPATIAL_RESAMPLING_MODE_H_PROGRESSIVE = 1
    /* 2…7 reserved */
} spatial_resampling_mode_hor_idc_e;

typedef enum
{
    SPATIAL_RESAMPLING_MODE_V_BYPASS      = 0,
    SPATIAL_RESAMPLING_MODE_V_PROGRESSIVE = 1,
    SPATIAL_RESAMPLING_MODE_V_TOP         = 2,
    SPATIAL_RESAMPLING_MODE_V_BOTTOM      = 3
    /* 4…7 reserved */
} spatial_resampling_mode_ver_idc_e;

typedef enum
{
    SPATIAL_FILTER_F0      = 0,
                           /* 1..6 reserved */
    SPATIAL_FILTER_EXP     = 7
} spatial_resampling_filter_idc_e;

typedef enum
{
    PROCESSING_NULL   = 0
            /* 1..15 reserved */
} processing_idc_e;

typedef enum
{
    SIGNAL_CS_YCBCR  = 0,       /* YCbCr */
    SIGNAL_CS_RGB    = 1,       /* RGB */
    SIGNAL_CS_IPT    = 2        /* IPT */
                                /* 3 Reserved */
} signal_color_space_e;

typedef enum
{
    SIGNAL_RANGE_NARROW  = 0,   /* Narrow range defined in SMPTE 2084 A.7 */
    SIGNAL_RANGE_FULL    = 1,   /* Full range defined in SMPTE 2084 A.2 */
    SIGNAL_RANGE_SDI     = 2    /* SDI range defined in SMPTE 2084 A.5 */
                                /* 3 Reserved */
} signal_range_e;


/****************************************************************************
 * Type definitions
 ****************************************************************************/

typedef struct rpu_context_s rpu_context_t;

/*!
    rpu_init_flags_t
    @brief Flags used for the encoder and decoder initialization.\n
*/
typedef struct rpu_init_flags_s
{
    rpu_profile_e profile;              /**< @details This flag is used to minimize the memory allocation
                                              of the encoder or decoder. After initializing the encoder/decoder
                                              will only be able to process streams with a RPU profile smaller
                                              than the one specified. */
    rpu_level_e level;                  /**< @details This flag is used to minimize the memory allocation
                                              of the encoder or decoder. After initializing the encoder/decoder
                                              will only be able to process streams with a RPU level smaller
                                              than the one specified. */
    int convert_to_float;               /**< @details Convert the rpu coefficients to floating point if they are in fixed point.
                                              This flag only takes effect on the rpu_decoder.
                                              If the flag is set to 0 no conversion is done.
                                              If this flag is set to 1 and coefficient_data_type is equal to 0
                                              all coefficients are converted from fixed point to floating point
                                              before passed to the application. The conversion is based on the
                                              coefficient log2 denom for the current coefficient. */
    int convert_to_fxp;                 /**< @details Convert the rpu coefficients to fixed point if they are in floating point.
                                              This flag only takes effect on the rpu_decoder.
                                              If the flag is set to 0 no conversion is done.
                                              If this flag is set to 1 and coefficient_data_type is equal to 1 all
                                              coefficients are converted from floating point to fixed point before
                                              passed to the application. The conversion uses the maximum allowed
                                              coefficient log2denom and sets this value accordingly.
                                              */
    int allow_changing_sequence_header; /**< @details The RPU specification doesn't allow the sequence header
                                              to change in the same sequence. If this flag is set to 1 the
                                              sequence header is allowed to change for every frame. This flag should
                                              only be set for debugging purposes. */

} rpu_init_flags_t;

typedef struct el_chroma_residual_resampling_s
{
    int32_t el_chroma_resampling_filter_cc_exp_coef[ 2 ][ 2 ][ MAX_NUM_RESIDUAL_RESAMPLING_COEFF ];
} el_chroma_residual_resampling_t;

typedef struct nlq_linear_dz_s
{
    uint32_t linear_deadzone_slope_int;
    union {
        uint32_t linear_deadzone_slope;
        float linear_deadzone_slope_float;
    } us;
    uint32_t linear_deadzone_threshold_int;
    union {
        uint32_t linear_deadzone_threshold;
        float linear_deadzone_threshold_float;
    } ut;
} nlq_linear_dz_t;

typedef struct nlq_mu_law_s
{
    uint16_t mu_law_levels;
    uint32_t mu_law_mu;
} nlq_mu_law_t;

typedef struct rpu_data_nlq_param_s
{
    uint16_t nlq_offset;
    uint32_t vdr_in_max_int;
    union {
        uint32_t vdr_in_max;
        float vdr_in_max_float;
    } uv;
    union {
        nlq_linear_dz_t  nlq_linear_dz;
        nlq_mu_law_t     nlq_mu_law;
    } up;
} rpu_data_nlq_param_t;

typedef struct rpu_data_nlq_s
{
    uint8_t nlq_param_pred_flag[MAX_NUM_NLQ_PIVOTS-1][3];
    rpu_data_nlq_param_t rpu_data_nlq_param[MAX_NUM_NLQ_PIVOTS-1][3];
    uint8_t diff_pred_part_idx_nlq[MAX_NUM_NLQ_PIVOTS-1][3]; /* = diff_pred_part_idx_nlq_minus1 + 1 */
} rpu_data_nlq_t;

typedef struct rpu_data_spatial_resampling_filter_exp_s
{
    int32_t  spatial_resampling_filter_exp_coef_int[2][2][3][8];
    union {
        uint32_t spatial_resampling_filter_exp_coef[2][2][3][8];
        float    spatial_resampling_filter_exp_coef_float[2][2][3][8];
    } u;
} rpu_data_spatial_resampling_filter_exp_t;

typedef struct rpu_data_spatial_resampling_s
{
    union {
        uint32_t spatial_resampling_pivot[2];
        float spatial_resampling_pivot_float[2];
    } u;
    rpu_data_spatial_resampling_filter_exp_t
        rpu_data_spatial_resampling_filter_exp;
} rpu_data_spatial_resampling_t;

typedef struct rpu_data_chroma_resampling_filter_1D_exp_coef_s
{
    int32_t chroma_filter_1D_exp_coef[MAX_NUM_CHROMA_FILTER_COEFF];
} rpu_data_chroma_resampling_filter_1D_exp_coef_t;

typedef struct rpu_data_chroma_resampling_filter_1D_exp_s
{
    uint8_t chroma_resampling_filter_1D_pred_flag;
    rpu_data_chroma_resampling_filter_1D_exp_coef_t
        rpu_data_chroma_resampling_filter_1D_exp_coef;
    uint8_t diff_pred_part_idx_chroma_resampling_filter_1D; /* = diff_pred_part_idx_chroma_resampling_filter_1D_minus1 + 1 */
} rpu_data_chroma_resampling_filter_1D_exp_t;

typedef struct rpu_data_chroma_resampling_filter_2D_exp_coef_s
{
    int32_t chroma_filter_2D_exp_coef[MAX_NUM_CHROMA_FILTER_COEFF][MAX_NUM_CHROMA_FILTER_COEFF];
} rpu_data_chroma_resampling_filter_2D_exp_coef_t;

typedef struct rpu_data_chroma_resampling_filter_2D_exp_s
{
    uint8_t chroma_resampling_filter_2D_pred_flag;
    rpu_data_chroma_resampling_filter_2D_exp_coef_t
        rpu_data_chroma_resampling_filter_2D_exp_coef[2];
    uint8_t diff_pred_part_idx_chroma_resampling_filter_2D; /* = diff_pred_part_idx_chroma_resampling_filter_2D_minus1 + 1 */
} rpu_data_chroma_resampling_filter_2D_exp_t;

typedef struct rpu_data_chroma_resampling_s
{
    rpu_data_chroma_resampling_filter_2D_exp_t
        rpu_data_chroma_resampling_filter_2D_exp[2];
    rpu_data_chroma_resampling_filter_1D_exp_t
        rpu_data_chroma_resampling_filter_1D_exp[2][2];
} rpu_data_chroma_resampling_t;

typedef struct rpu_data_mapping_param_poly_s
{
    uint8_t poly_order;  /* = poly_order_minus1 + 1 */
    uint8_t linear_interp_flag ;
    /* Linear interpolation */
    int32_t pred_linear_interp_value_int;
    union {
        uint32_t pred_linear_interp_value;
        float pred_linear_interp_value_float;
    } ul;
    /* Polynomial */
    int32_t poly_coef_int[MAX_POLYNOMIAL_ORDER+1];
    union {
        uint32_t poly_coef[MAX_POLYNOMIAL_ORDER+1];
        float poly_coef_float[MAX_POLYNOMIAL_ORDER+1];
    } uc;
} rpu_data_mapping_param_poly_t;

typedef struct rpu_data_mapping_param_MMR_s
{
    uint8_t mmr_order;    /* = mmr_order_minus1 + 1 */
    int32_t mmr_constant_int;
    union {
        uint32_t mmr_constant;
        float mmr_constant_float;
    } u1;
    int32_t mmr_coef_int[MAX_MMR_ORDER][NUM_MMR_COEFF];
    union {
        uint32_t mmr_coef[MAX_MMR_ORDER][NUM_MMR_COEFF];
        float mmr_coef_float[MAX_MMR_ORDER][NUM_MMR_COEFF];
    } uc;
} rpu_data_mapping_param_MMR_t;

typedef struct rpu_data_mapping_param_pow_s
{
    int32_t pow_coef_int[NUM_POW_COEFF];
    union {
        uint32_t pow_coef[NUM_POW_COEFF];
        float pow_coef_float[NUM_POW_COEFF];
    } u;
} rpu_data_mapping_param_pow_t;

typedef struct rpu_data_mapping_param_LUT_s
{
    int32_t pred_lut_value_int[MAPPING_LUT_SIZE];
    union {
        uint32_t pred_lut_value[MAPPING_LUT_SIZE];
        float pred_lut_value_float[MAPPING_LUT_SIZE];
    } u;
} rpu_data_mapping_param_LUT_t;

typedef struct rpu_data_mapping_s
{
    uint8_t mapping_idc[3][MAX_NUM_MAPPING_PIVOTS - 1];
    uint8_t mapping_param_pred_flag[3][MAX_NUM_MAPPING_PIVOTS - 1];
    union {
        rpu_data_mapping_param_poly_t rpu_data_mapping_param_poly[3][MAX_NUM_MAPPING_PIVOTS - 1];
        rpu_data_mapping_param_MMR_t  rpu_data_mapping_param_MMR[3][MAX_NUM_MAPPING_PIVOTS - 1];
        rpu_data_mapping_param_pow_t  rpu_data_mapping_param_pow[3][MAX_NUM_MAPPING_PIVOTS - 1];
        rpu_data_mapping_param_LUT_t  rpu_data_mapping_param_LUT[3];
    } u;
    uint8_t diff_pred_part_idx_mapping[3][MAX_NUM_MAPPING_PIVOTS - 1]; /* diff_pred_part_idx_mapping_minus1 + 1 */
} rpu_data_mapping_t;

/*!
    @struct vdr_rpu_data_payload_t
    @brief Data structure for the rpu payloads.
    @details This data structure is derived from vdr_rpu_data_payload in the "Dolby Consumer VDR Decoder Specification". It also contains offsets to the coefficients of all payloads. Each payload contains the payload data and the coefficients for one partition.
*/
typedef struct vdr_rpu_data_payload_s
{
/** \name Payload data as defined in the "Dolby Consumer VDR Decoder Specification".
*/
/*! \{ */
    rpu_data_mapping_t             rpu_data_mapping; /**< @brief */
    rpu_data_chroma_resampling_t   rpu_data_chroma_resampling; /**< @brief */
    rpu_data_spatial_resampling_t  rpu_data_spatial_resampling; /**< @brief */
    rpu_data_nlq_t                 rpu_data_nlq; /**< @brief */
    el_chroma_residual_resampling_t el_chroma_residual_resampling; /**< @brief */
/*! \} */
/** \name Offsets from the payload base address to the actual coefficients for each partition.*/
/*! \{ */
    int32_t mapping_coef_offset[3][MAX_NUM_MAPPING_PIVOTS-1]; /**< @brief */
    int32_t chroma_resampling_filter_1D_exp_coef_offset[2][2]; /**< @brief */
    int32_t chroma_resampling_filter_2D_exp_coef_offset[2][2]; /**< @brief */
    int32_t nlq_coef_offset[MAX_NUM_NLQ_PIVOTS-1][3]; /**< @brief */
    int32_t el_chroma_resampling_filter_cc_exp_coef_offset[2][2]; /**< @brief */
/*! \} */
} vdr_rpu_data_payload_t;


typedef struct ext_block_payload_level_1_s
{
    uint16_t min_PQ;
    uint16_t max_PQ;
    uint16_t avg_PQ;
} ext_block_payload_level_1_t;

typedef struct ext_block_payload_level_2_s
{
    uint16_t target_max_PQ       ;
    uint16_t trim_slope          ;
    uint16_t trim_offset         ;
    uint16_t trim_power          ;
    uint16_t trim_chroma_weight  ;
    uint16_t trim_saturation_gain;
    int16_t  ms_weight           ;
} ext_block_payload_level_2_t;

typedef struct ext_block_payload_level_4_s
{
    uint16_t anchor_PQ       ;
    uint16_t anchor_power    ;
} ext_block_payload_level_4_t;

typedef struct ext_block_payload_level_5_s
{
    uint16_t active_area_left_offset   ;
    uint16_t active_area_right_offset  ;
    uint16_t active_area_top_offset    ;
    uint16_t active_area_bottom_offset ;
} ext_block_payload_level_5_t;

typedef struct ext_content_adaptive_metadata_s
{
    uint32_t ext_block_length;
    uint8_t ext_block_level;
    union {
        ext_block_payload_level_1_t level_1;
        ext_block_payload_level_2_t level_2;
        ext_block_payload_level_4_t level_4;
        ext_block_payload_level_5_t level_5;
    } l;
} ext_content_adaptive_metadata_t;


typedef struct vdr_dm_data_payload_s
{
    uint8_t  affected_dm_metadata_id;
    uint8_t  current_dm_metadata_id;
    uint8_t  scene_refresh_flag;
    int16_t  YCCtoRGB_coef0;
    int16_t  YCCtoRGB_coef1;
    int16_t  YCCtoRGB_coef2;
    int16_t  YCCtoRGB_coef3;
    int16_t  YCCtoRGB_coef4;
    int16_t  YCCtoRGB_coef5;
    int16_t  YCCtoRGB_coef6;
    int16_t  YCCtoRGB_coef7;
    int16_t  YCCtoRGB_coef8;
    uint32_t YCCtoRGB_offset0;
    uint32_t YCCtoRGB_offset1;
    uint32_t YCCtoRGB_offset2;
    int16_t  RGBtoLMS_coef0;
    int16_t  RGBtoLMS_coef1;
    int16_t  RGBtoLMS_coef2;
    int16_t  RGBtoLMS_coef3;
    int16_t  RGBtoLMS_coef4;
    int16_t  RGBtoLMS_coef5;
    int16_t  RGBtoLMS_coef6;
    int16_t  RGBtoLMS_coef7;
    int16_t  RGBtoLMS_coef8;
    uint16_t signal_eotf;
    uint16_t signal_eotf_param0;
    uint16_t signal_eotf_param1;
    uint32_t signal_eotf_param2;
    uint8_t  signal_bit_depth;
    uint8_t  signal_color_space;
    uint8_t  signal_chroma_format;
    uint8_t  signal_full_range_flag;
    uint16_t source_min_PQ;
    uint16_t source_max_PQ;
    uint16_t source_diagonal;
    uint8_t  num_ext_blocks;
    ext_content_adaptive_metadata_t ext_block[255];
} vdr_dm_data_payload_t;

typedef struct vdr_rpu_sequence_header_s
{
    uint8_t  chroma_sample_loc_type;
    uint8_t  chroma_resampling_explicit_filter_flag;
    uint8_t  coefficient_data_type;
    uint8_t  coefficient_log2_denom;
    uint8_t  spatial_filter_exp_coef_log2_denom; /* = spatial_filter_exp_coef_log2_denom_minus6 + 6  */
    uint8_t  chroma_filter_exp_coef_log2_denom;  /* = chroma_filter_exp_coef_log2_denom_minus6 + 6   */
    uint8_t  vdr_rpu_normalized_idc;
    uint8_t  BL_video_full_range_flag;
    uint8_t  BL_bit_depth;
    uint8_t  EL_bit_depth;
    uint8_t  vdr_bit_depth;
    uint8_t  spatial_resampling_filter_flag;
    uint8_t  spatial_resampling_explicit_filter_flag;
    uint8_t  reserved_zero_3bits;
    uint8_t  el_spatial_resampling_filter_flag;
    uint8_t  disable_residual_flag;
    uint8_t  el_chroma_filter_exp_coef_log2_denom; /* = el_chroma_filter_exp_coef_log2_denom_minus10 + 10 */
} vdr_rpu_sequence_header_t;

/*!
    @struct rpu_data_header_t
    @brief For details of this data structure and sub data structures see "Dolby Consumer VDR Decoder Specification."
*/
typedef struct rpu_data_header_s
{
    uint8_t  rpu_type;
    uint16_t rpu_format;
    uint8_t  vdr_rpu_profile;
    uint8_t  vdr_rpu_level;
    uint8_t  vdr_seq_info_present_flag;
    vdr_rpu_sequence_header_t rpu_sequence_header;
    uint8_t  vdr_dm_metadata_present_flag;
    uint8_t  use_prev_vdr_rpu_flag;
    uint8_t  prev_vdr_rpu_id;
    uint8_t  vdr_rpu_id;
    uint8_t  mapping_color_space;
    uint8_t  mapping_chroma_format_idc;
    uint8_t  chroma_resampling_filter_idc;
    uint8_t  chroma_filter_1D_exp_length_ver;         /* = (chroma_filter_1D_exp_length_ver_div2_minus2 + 2) * 2   */
    uint8_t  chroma_filter_1D_exp_length_hor;         /* = (chroma_filter_1D_exp_length_hor_div2_minus2 + 2) * 2   */
    uint8_t  chroma_filter_2D_exp_phase0_length_ver;  /* = chroma_filter_2D_exp_phase0_length_ver_minus1  + 1      */
    uint8_t  chroma_filter_2D_exp_phase0_length_hor;  /* = chroma_filter_2D_exp_phase0_length_hor_minus1  + 1      */
    uint8_t  chroma_filter_2D_exp_phase1_length_ver;  /* = chroma_filter_2D_exp_phase1_length_ver_minus1  + 1      */
    uint8_t  chroma_filter_2D_exp_phase1_length_hor;  /* = chroma_filter_2D_exp_phase1_length_hor_minus1  + 1      */
    uint8_t  num_pivots[3];  /* = num_pivots_minus2 + 2 */
    uint16_t pred_pivot_value[3][MAX_NUM_MAPPING_PIVOTS ];
    uint16_t pivot_value[3][MAX_NUM_MAPPING_PIVOTS];  /* derived values from pred_pivot_value */
    uint8_t  spatial_resampling_mode_hor_idc;
    uint8_t  spatial_resampling_filter_hor_idc[3];
    uint8_t  spatial_resampling_mode_ver_idc;
    uint8_t  spatial_resampling_filter_ver_idc[3];
    uint8_t  nlq_method_idc;
    uint8_t  nlq_num_pivots;   /*  = nlq_num_pivots_minus2 + 2 */
    uint16_t nlq_pred_pivot_value[MAX_NUM_NLQ_PIVOTS ];
    uint16_t nlq_pivot_value[MAX_NUM_NLQ_PIVOTS];   /* derived values from nlq_pred_pivot_value */
    uint8_t  el_chroma_resampling_filter_idc;
    uint8_t  num_x_partitions;  /* = num_x_partitions_minus1 + 1 */
    uint8_t  num_y_partitions;  /* = num_y_partitions_minus1 + 1 */
    uint8_t  overlapped_prediction_method;
} rpu_data_header_t;

/*!
    @struct rpu_data_t
    @brief This is the main rpu_data structure used as an input for the RPU encoder and as an output for the RPU decoder.
    @details This data structure is derived from the Dolby Consumer VDR Decoder Specification.
*/
typedef struct rpu_data_s
{
    rpu_data_header_t *rpu_data_header; /**< @brief Pointer to the rpu header. */
    vdr_rpu_data_payload_t *vdr_rpu_data_payload; /**< @brief Pointer to the rpu payload */
    vdr_dm_data_payload_t *vdr_dm_data_payload; /**< @brief Pointer to DM metadata */
} rpu_data_t;

struct rpu_encoder_ctx_s;
struct rpu_decoder_ctx_s;

/****************************************************************************
 * Function definitions
 ****************************************************************************/

/*!
    @brief
    Dumping RPU header data structure.
    @param [in]  rpu_data
    pointer to the RPU data structure
*/
LIBRPU_API void rpu_header_dump(rpu_data_t* rpu_data);
/*!
    @brief
    Dumping RPU payload data structure.
    @details Only one context needs to be passed. The other context must be NULL.
    @param [in]  rpu_data
    pointer to the RPU data structure
    @param [in]  rec
    pointer to the rpu encoder context
    @param [in]  rdc
    pointer to the rpu decoder context
*/
LIBRPU_API int rpu_payload_dump(rpu_data_t* rpu_data, struct rpu_encoder_ctx_s* rec, struct rpu_decoder_ctx_s* rdc);

/*!
    @brief
    Dumping DM metadata data structure.
    @param [in]  rpu_data
    pointer to the RPU data structure
*/
LIBRPU_API void vdr_dm_data_payload_dump(rpu_data_t* rpu_data);

/*! @brief
    Read DM metadata to buffer from *.md file
    @param[in]  fp_md
    Pointer to *.md file
    @param[out] pdm_md
    Pointer to dm_metadata_t structure
*/
LIBRPU_API int read_dm_data_payload(dm_metadata_t *pdm_md, FILE *fp_md);

/*!
    @brief
    Convert floating point coefficient to fixed point coefficient.
    @param [in]   coeff
    The input floating point coefficient
    @param [in]   coefficient_log2_denom
    Number of fractional bits in fixed point coefficient
    @param [out]  coeff_int
    pointer to integer part of fixed point coefficient
    @param [out]  coeff_frac
    pointer to fractional part of fixed point coefficient
*/

LIBRPU_API int convert_coeff_float_to_fixed_point(float coeff, int coefficient_log2_denom, int32_t* coeff_int, uint32_t* coeff_frac);

/*! @brief
    Convert fixed point coefficient to floating point coefficient.
    @param[in]  coefficient_log2_denom
    Number of fractional bits in fixed point coefficient
    @param[in]  coeff_int
    The integer part of fixed point coefficient
    @param[in]  coeff_frac
    The fractional part of fixed point coefficient
    @param[out] coeff
    Pointer to the output floating point coefficient
*/
LIBRPU_API int convert_coeff_fixed_point_to_float(int coefficient_log2_denom, int32_t coeff_int, uint32_t coeff_frac, float* coeff);

/*!
    @brief
    Convert DM RPU payload to DM byte structure.
    @param [in]   dm_rpu
    pointer to the DM RPU data structure
    @param [out]  dm_bstruct
    pointer to the DM metadata structure
*/
LIBRPU_API void dm_rpu_payload_2_struct(vdr_dm_data_payload_t* dm_rpu, dm_metadata_t* dm_bstruct);

/*!
    @brief
    Convert DM byte structure to DM RPU payload.
    @param [in]   dm_struct
    pointer to the DM metadata structure
    @param [out]  rpu_payload
    pointer to the DM RPU data structure
*/
LIBRPU_API void dm_struct_2_rpu_payload(vdr_dm_data_payload_t* rpu_payload, dm_metadata_t* dm_struct);

/*!
    @brief
    Convert DM RPU payload to DM byte sequence
    @param [in]   dm_rpu
    pointer to the DM RPU data structure
    @param [out]  DM byte sequence buffer pointer
    @param [out]  DM byte sequence length pointer
    pointer to the DM metadata structure
*/
LIBRPU_API int dm_rpu_payload_2_byte_sequence(vdr_dm_data_payload_t* dm_rpu, char* dm_byte_sequence, int* length);

/*!
    @brief
    Validate DM payload.
    @param [in]  dmp
    pointer to the DM RPU data structure
    @param [in]  frame
    current frame number
    @return
        @li =0     no error
        @li <0     error code

*/
LIBRPU_API int validate_dm_payload(vdr_dm_data_payload_t* dmp, int frame);

/*!
    @brief
    Validate composer config.
    @param [in]   efrc
    pointer to the composr config structure
    @param [in]  rpu_payload
    current frame number
    @return
        @li =0     no error
        @li <0     error code

*/
LIBRPU_API int validate_composer_config(rpu_ext_config_fixpt_main_t* efrc, int frame);

/*!
    @brief
    Converts an error code to a meaningful string.
    @param [in]  error_code
    error code number
    @return error code string

*/
LIBRPU_API const char *rpu_error_code_2_str(int error_code);

#ifdef __cplusplus
}
#endif

#endif /* _RPU_API_COMMON_H_ */
