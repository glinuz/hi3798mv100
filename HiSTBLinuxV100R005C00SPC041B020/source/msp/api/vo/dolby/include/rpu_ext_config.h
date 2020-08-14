/**
 * This product contains one or more programs protected under international
 * and U.S. copyright laws as unpublished works.  They are confidential and
 * proprietary to Dolby Laboratories.  Their reproduction or disclosure, in
 * whole or in part, or the production of derivative works therefrom without
 * the express permission of Dolby Laboratories is prohibited.
 * Copyright 2011 - 2015 by Dolby Laboratories.
 * All rights reserved.
 *
 * @brief common data structures.
 * @file rpu_ext_config.h
 *
 * $Id$
 */

#ifndef _RPU_EXT_CONFIG_H_
#define _RPU_EXT_CONFIG_H_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define MAX_PIVOT       9
#define MAX_POLY_ORDER  2

#define MAX_DM_EXT_BLOCKS 255
//  0x2F00 - 119

typedef struct rpu_ext_config_fixpt_base_s
{
    uint32_t  rpu_VDR_bit_depth;
    uint32_t  rpu_BL_bit_depth;
    uint32_t  rpu_EL_bit_depth;
    uint32_t  coefficient_log2_denom;
    uint32_t  num_pivots[3];
    uint32_t  pivot_value[3][MAX_PIVOT];
    uint32_t  mapping_idc[3];
    uint32_t  poly_order[3][MAX_PIVOT-1];
    int32_t   poly_coef_int[3][MAX_PIVOT-1][MAX_POLY_ORDER+1];
    uint32_t  poly_coef[3][MAX_PIVOT-1][MAX_POLY_ORDER+1];
    uint8_t   NLQ_method_idc;
    uint8_t   disable_residual_flag;
    uint8_t   reserved_8bit_0;
    uint8_t   reserved_8bit_1;
    uint32_t  NLQ_offset[3];
    int32_t   NLQ_coeff_int[3][3];
    uint32_t  NLQ_coeff[3][3];
} rpu_ext_config_fixpt_base_t;

typedef struct rpu_ext_config_fixpt_main_s
{
    uint32_t  rpu_VDR_bit_depth;
    uint32_t  rpu_BL_bit_depth;
    uint32_t  rpu_EL_bit_depth;
    uint32_t  coefficient_log2_denom;
    uint32_t  num_pivots[3];
    uint32_t  pivot_value[3][MAX_PIVOT];
    uint32_t  mapping_idc[3];
    uint32_t  poly_order[3][MAX_PIVOT-1];
    int32_t   poly_coef_int[3][MAX_PIVOT-1][MAX_POLY_ORDER+1];
    uint32_t  poly_coef[3][MAX_PIVOT-1][MAX_POLY_ORDER+1];
    uint32_t  MMR_order[2];
    int32_t   MMR_coef_int[2][22];
    uint32_t  MMR_coef[2][22];
    uint8_t   NLQ_method_idc;
    uint8_t   disable_residual_flag;
    uint8_t   el_spatial_resampling_filter_flag;
    uint8_t   reserved_8bit;
    uint32_t  NLQ_offset[3];
    int32_t   NLQ_coeff_int[3][3];
    uint32_t  NLQ_coeff[3][3];
    uint32_t  spatial_resampling_filter_flag;
    uint32_t  spatial_resampling_explicit_filter_flag;
    uint32_t  spatial_filter_exp_coef_log2_denom;
    uint32_t  spatial_resampling_mode_hor_idc;
    uint32_t  spatial_resampling_mode_ver_idc;
    uint32_t  spatial_resampling_filter_hor_idc[3];
    uint32_t  spatial_resampling_filter_ver_idc[3];
    uint32_t  spatial_resampling_luma_pivot[2];
    int32_t   spatial_filter_coeff_hor_int[3][8];
    uint32_t  spatial_filter_coeff_hor[3][8];
    int32_t   spatial_filter_coeff_ver_int[2][3][6];
    uint32_t  spatial_filter_coeff_ver[2][3][6];
} rpu_ext_config_fixpt_main_t;

typedef struct dm_metadata_base_s
{
    /* signal attributes */
    unsigned char dm_metadata_id; // affected_dm_metadata_id<<4|current_dm_metadata_id
    unsigned char scene_refresh_flag;
    unsigned char YCCtoRGB_coef0_hi;
    unsigned char YCCtoRGB_coef0_lo;
    unsigned char YCCtoRGB_coef1_hi;
    unsigned char YCCtoRGB_coef1_lo;
    unsigned char YCCtoRGB_coef2_hi;
    unsigned char YCCtoRGB_coef2_lo;
    unsigned char YCCtoRGB_coef3_hi;
    unsigned char YCCtoRGB_coef3_lo;
    unsigned char YCCtoRGB_coef4_hi;
    unsigned char YCCtoRGB_coef4_lo;
    unsigned char YCCtoRGB_coef5_hi;
    unsigned char YCCtoRGB_coef5_lo;
    unsigned char YCCtoRGB_coef6_hi;
    unsigned char YCCtoRGB_coef6_lo;
    unsigned char YCCtoRGB_coef7_hi;
    unsigned char YCCtoRGB_coef7_lo;
    unsigned char YCCtoRGB_coef8_hi;
    unsigned char YCCtoRGB_coef8_lo;
    unsigned char YCCtoRGB_offset0_byte3;
    unsigned char YCCtoRGB_offset0_byte2;
    unsigned char YCCtoRGB_offset0_byte1;
    unsigned char YCCtoRGB_offset0_byte0;
    unsigned char YCCtoRGB_offset1_byte3;
    unsigned char YCCtoRGB_offset1_byte2;
    unsigned char YCCtoRGB_offset1_byte1;
    unsigned char YCCtoRGB_offset1_byte0;
    unsigned char YCCtoRGB_offset2_byte3;
    unsigned char YCCtoRGB_offset2_byte2;
    unsigned char YCCtoRGB_offset2_byte1;
    unsigned char YCCtoRGB_offset2_byte0;
    unsigned char RGBtoLMS_coef0_hi;
    unsigned char RGBtoLMS_coef0_lo;
    unsigned char RGBtoLMS_coef1_hi;
    unsigned char RGBtoLMS_coef1_lo;
    unsigned char RGBtoLMS_coef2_hi;
    unsigned char RGBtoLMS_coef2_lo;
    unsigned char RGBtoLMS_coef3_hi;
    unsigned char RGBtoLMS_coef3_lo;
    unsigned char RGBtoLMS_coef4_hi;
    unsigned char RGBtoLMS_coef4_lo;
    unsigned char RGBtoLMS_coef5_hi;
    unsigned char RGBtoLMS_coef5_lo;
    unsigned char RGBtoLMS_coef6_hi;
    unsigned char RGBtoLMS_coef6_lo;
    unsigned char RGBtoLMS_coef7_hi;
    unsigned char RGBtoLMS_coef7_lo;
    unsigned char RGBtoLMS_coef8_hi;
    unsigned char RGBtoLMS_coef8_lo;
    unsigned char signal_eotf_hi;
    unsigned char signal_eotf_lo;
    unsigned char signal_eotf_param0_hi;
    unsigned char signal_eotf_param0_lo;
    unsigned char signal_eotf_param1_hi;
    unsigned char signal_eotf_param1_lo;
    unsigned char signal_eotf_param2_byte3;
    unsigned char signal_eotf_param2_byte2;
    unsigned char signal_eotf_param2_byte1;
    unsigned char signal_eotf_param2_byte0;
    unsigned char signal_bit_depth;
    unsigned char signal_color_space;
    unsigned char signal_chroma_format;
    unsigned char signal_full_range_flag;
    /* source display attributes */
    unsigned char source_min_PQ_hi;
    unsigned char source_min_PQ_lo;
    unsigned char source_max_PQ_hi;
    unsigned char source_max_PQ_lo;
    unsigned char source_diagonal_hi;
    unsigned char source_diagonal_lo;
    /* extended metadata */
    unsigned char num_ext_blocks;
} dm_metadata_base_t;

typedef struct ext_level_1_s
{
    unsigned char min_PQ_hi;
    unsigned char min_PQ_lo;
    unsigned char max_PQ_hi;
    unsigned char max_PQ_lo;
    unsigned char avg_PQ_hi;
    unsigned char avg_PQ_lo;
} ext_level_1_t;

typedef struct ext_level_2_s
{
    unsigned char target_max_PQ_hi       ;
    unsigned char target_max_PQ_lo       ;
    unsigned char trim_slope_hi          ;
    unsigned char trim_slope_lo          ;
    unsigned char trim_offset_hi         ;
    unsigned char trim_offset_lo         ;
    unsigned char trim_power_hi          ;
    unsigned char trim_power_lo          ;
    unsigned char trim_chroma_weight_hi  ;
    unsigned char trim_chroma_weight_lo  ;
    unsigned char trim_saturation_gain_hi;
    unsigned char trim_saturation_gain_lo;
    unsigned char ms_weight_hi           ;
    unsigned char ms_weight_lo           ;
} ext_level_2_t;

typedef struct ext_level_4_s
{
    unsigned char anchor_PQ_hi       ;
    unsigned char anchor_PQ_lo       ;
    unsigned char anchor_power_hi    ;
    unsigned char anchor_power_lo    ;
} ext_level_4_t;

typedef struct ext_level_5_s
{
    unsigned char active_area_left_offset_hi  ;
    unsigned char active_area_left_offset_lo  ;
    unsigned char active_area_right_offset_hi ;
    unsigned char active_area_right_offset_lo ;
    unsigned char active_area_top_offset_hi   ;
    unsigned char active_area_top_offset_lo   ;
    unsigned char active_area_bottom_offset_hi;
    unsigned char active_area_bottom_offset_lo;
} ext_level_5_t;

typedef struct dm_metadata_ext_s
{
    unsigned char ext_block_length_byte3;
    unsigned char ext_block_length_byte2;
    unsigned char ext_block_length_byte1;
    unsigned char ext_block_length_byte0;
    unsigned char ext_block_level;
    union {
        ext_level_1_t level_1;
        ext_level_2_t level_2;
        ext_level_4_t level_4;
        ext_level_5_t level_5;
    } l;
} dm_metadata_ext_t;

typedef struct dm_metadata_s {
    dm_metadata_base_t base;
    dm_metadata_ext_t ext[MAX_DM_EXT_BLOCKS];
} dm_metadata_t;

#ifdef __cplusplus
}
#endif

#endif /* _RPU_EXT_CONFIG_H_ */
