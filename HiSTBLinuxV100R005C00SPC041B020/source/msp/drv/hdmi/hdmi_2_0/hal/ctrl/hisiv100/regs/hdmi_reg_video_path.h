// ****************************************************************************** 
// Copyright     :  Copyright (C) 2015, Hisilicon Technologies Co. Ltd.
// File name     :  video_path_reg_c_union_define.h
// Project line  :  Platform And Key Technologies Development
// Department    :  CAD Development Department
// Author        :  w00226427
// Version       :  1.0
// Date          :  Tester
// Description   :  The description of xxx project
// Others        :  Generated automatically by nManager V4.0.2.5 
// History       :  w00226427 2015/06/15 16:25:01 Create file
// ******************************************************************************

#ifndef __VIDEO_PATH_REG_C_UNION_DEFINE_H__
#define __VIDEO_PATH_REG_C_UNION_DEFINE_H__

/* Define the union U_TIMING_GEN_CTRL */
/* Define the union U_TIMING_GEN_CTRL,ATTR:,ADDR:800,INIT:00000000,MASK:3ff,CMMNT:Timing Generator øÿ÷∆ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reg_timing_gen_en : 1  ; /* [0] */
        unsigned int    reg_extmode       : 1  ; /* [1] */
        unsigned int    reg_timing_sel    : 6  ; /* [7:2] */
        unsigned int    reg_sync_polarity : 2  ; /* [9:8] */
        unsigned int    rsv_0             : 22  ; /* [31:10] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_TIMING_GEN_CTRL;

/* Define the union U_HSYNC_TIMING_CONFIG0 */
/* Define the union U_HSYNC_TIMING_CONFIG0,ATTR:,ADDR:804,INIT:00638359,MASK:3ffffff,CMMNT:ÀÆ∆Ω ±–Ú≈‰÷√ºƒ¥Ê∆˜0°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hsync_total_count       : 13  ; /* [12:0] */
        unsigned int    hsync_high_to_low_count : 13  ; /* [25:13] */
        unsigned int    rsv_1                   : 6  ; /* [31:26] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HSYNC_TIMING_CONFIG0;

/* Define the union U_HSYNC_TIMING_CONFIG1 */
/* Define the union U_HSYNC_TIMING_CONFIG1,ATTR:,ADDR:808,INIT:000202E0,MASK:3ffffff,CMMNT:ÀÆ∆Ω ±–Ú≈‰÷√ºƒ¥Ê∆˜1°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hsync_de_start_count : 13  ; /* [12:0] */
        unsigned int    hsync_de_end_count   : 13  ; /* [25:13] */
        unsigned int    rsv_2                : 6  ; /* [31:26] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HSYNC_TIMING_CONFIG1;

/* Define the union U_HSYNC_TIMING_CONFIG2 */
/* Define the union U_HSYNC_TIMING_CONFIG2,ATTR:,ADDR:80C,INIT:00000000,MASK:1fff,CMMNT:ÀÆ∆Ω ±–Ú≈‰÷√ºƒ¥Ê∆˜2°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hsync_low_to_high_count : 13  ; /* [12:0] */
        unsigned int    rsv_3                   : 19  ; /* [31:13] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_HSYNC_TIMING_CONFIG2;

/* Define the union U_VSYNC_TIMING_CONFIG0 */
/* Define the union U_VSYNC_TIMING_CONFIG0,ATTR:,ADDR:810,INIT:0000620C,MASK:ffffff,CMMNT:¥π÷± ±–Ú≈‰÷√ºƒ¥Ê∆˜0°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vsync_total_count       : 12  ; /* [11:0] */
        unsigned int    vsync_high_to_low_count : 12  ; /* [23:12] */
        unsigned int    rsv_4                   : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VSYNC_TIMING_CONFIG0;

/* Define the union U_VSYNC_TIMING_CONFIG1 */
/* Define the union U_VSYNC_TIMING_CONFIG1,ATTR:,ADDR:814,INIT:0000F1EF,MASK:ffffff,CMMNT:¥π÷± ±–Ú≈‰÷√ºƒ¥Ê∆˜1°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vsync_de_start_count : 12  ; /* [11:0] */
        unsigned int    vsync_de_end_count   : 12  ; /* [23:12] */
        unsigned int    rsv_5                : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VSYNC_TIMING_CONFIG1;

/* Define the union U_VSYNC_TIMING_CONFIG2 */
/* Define the union U_VSYNC_TIMING_CONFIG2,ATTR:,ADDR:818,INIT:00000000,MASK:fff,CMMNT:¥π÷± ±–Ú≈‰÷√ºƒ¥Ê∆˜2°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vsync_low_to_high_count : 12  ; /* [11:0] */
        unsigned int    rsv_6                   : 20  ; /* [31:12] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VSYNC_TIMING_CONFIG2;

/* Define the union U_PATTERN_GEN_CTRL */
/* Define the union U_PATTERN_GEN_CTRL,ATTR:,ADDR:840,INIT:00000000,MASK:7fff,CMMNT:enPattern Generatorøÿ÷∆ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tpg_enable          : 1  ; /* [0] */
        unsigned int    video_format        : 2  ; /* [2:1] */
        unsigned int    solid_pattern_en    : 1  ; /* [3] */
        unsigned int    colorbar_en         : 1  ; /* [4] */
        unsigned int    square_pattern_en   : 1  ; /* [5] */
        unsigned int    mask_pattern_en     : 3  ; /* [8:6] */
        unsigned int    replace_pattern_en  : 3  ; /* [11:9] */
        unsigned int    bar_pattern_extmode : 1  ; /* [12] */
        unsigned int    cbar_pattern_sel    : 2  ; /* [14:13] */
        unsigned int    mix_color_en        : 1  ; /* [15] */
        unsigned int    increase_en         : 1  ; /* [16] */
        unsigned int    rsv_7               : 15  ; /* [31:17] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PATTERN_GEN_CTRL;

/* Define the union U_SOLID_PATTERN_CONFIG */
/* Define the union U_SOLID_PATTERN_CONFIG,ATTR:,ADDR:844,INIT:1F8DC1F8,MASK:3fffffff,CMMNT:¥ø…´pattern≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    solid_pattern_cb : 10  ; /* [9:0] */
        unsigned int    solid_pattern_y  : 10  ; /* [19:10] */
        unsigned int    solid_pattern_cr : 10  ; /* [29:20] */
        unsigned int    rsv_8            : 2  ; /* [31:30] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_SOLID_PATTERN_CONFIG;

/* Define the union U_MASK_PATTERN_CONFIG */
/* Define the union U_MASK_PATTERN_CONFIG,ATTR:,ADDR:848,INIT:3FFFFFFF,MASK:3fffffff,CMMNT:mask pattern≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mask_pattern_cb : 10  ; /* [9:0] */
        unsigned int    mask_pattern_y  : 10  ; /* [19:10] */
        unsigned int    mask_pattern_cr : 10  ; /* [29:20] */
        unsigned int    rsv_9           : 2  ; /* [31:30] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MASK_PATTERN_CONFIG;

/* Define the union U_BAR_EXT_CONFIG */
/* Define the union U_BAR_EXT_CONFIG,ATTR:,ADDR:84C,INIT:0007805A,MASK:3ffffff,CMMNT:colorbar/square patternøÌ°¢∏ﬂ≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    colorbar_width : 12  ; /* [11:0] */
        unsigned int    square_height  : 12  ; /* [23:12] */
        unsigned int    rsv_10         : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_BAR_EXT_CONFIG;

/* Define the union U_CBAR_PATTERN_A0 */
/* Define the union U_CBAR_PATTERN_A0,ATTR:,ADDR:850,INIT:3C3FC3FC,MASK:3fffffff,CMMNT:Colorbar/square patternÀÆ∆Ω≤ Ãı1≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bar_pattern_a0 : 30  ; /* [29:0] */
        unsigned int    rsv_11         : 2  ; /* [31:30] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CBAR_PATTERN_A0;

/* Define the union U_CBAR_PATTERN_A1 */
/* Define the union U_CBAR_PATTERN_A1,ATTR:,ADDR:854,INIT:21C87000,MASK:3fffffff,CMMNT:Colorbar/square patternÀÆ∆Ω≤ Ãı2≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bar_pattern_a1 : 30  ; /* [29:0] */
        unsigned int    rsv_12         : 2  ; /* [31:30] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CBAR_PATTERN_A1;

/* Define the union U_CBAR_PATTERN_A2 */
/* Define the union U_CBAR_PATTERN_A2,ATTR:,ADDR:858,INIT:0008721C,MASK:3fffffff,CMMNT:Colorbar/square patternÀÆ∆Ω≤ Ãı3≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bar_pattern_a2 : 30  ; /* [29:0] */
        unsigned int    rsv_13         : 2  ; /* [31:30] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CBAR_PATTERN_A2;

/* Define the union U_CBAR_PATTERN_A3 */
/* Define the union U_CBAR_PATTERN_A3,ATTR:,ADDR:85C,INIT:00087000,MASK:3fffffff,CMMNT:Colorbar/square patternÀÆ∆Ω≤ Ãı4≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bar_pattern_a3 : 30  ; /* [29:0] */
        unsigned int    rsv_14         : 2  ; /* [31:30] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CBAR_PATTERN_A3;

/* Define the union U_CBAR_PATTERN_A4 */
/* Define the union U_CBAR_PATTERN_A4,ATTR:,ADDR:860,INIT:21C0021C,MASK:3fffffff,CMMNT:Colorbar/square patternÀÆ∆Ω≤ Ãı5≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bar_pattern_a4 : 30  ; /* [29:0] */
        unsigned int    rsv_15         : 2  ; /* [31:30] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CBAR_PATTERN_A4;

/* Define the union U_CBAR_PATTERN_A5 */
/* Define the union U_CBAR_PATTERN_A5,ATTR:,ADDR:864,INIT:21C00000,MASK:3fffffff,CMMNT:Colorbar/square patternÀÆ∆Ω≤ Ãı6≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bar_pattern_a5 : 30  ; /* [29:0] */
        unsigned int    rsv_16         : 2  ; /* [31:30] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CBAR_PATTERN_A5;

/* Define the union U_CBAR_PATTERN_A6 */
/* Define the union U_CBAR_PATTERN_A6,ATTR:,ADDR:868,INIT:0000021C,MASK:3fffffff,CMMNT:Colorbar/square patternÀÆ∆Ω≤ Ãı7≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bar_pattern_a6 : 30  ; /* [29:0] */
        unsigned int    rsv_17         : 2  ; /* [31:30] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CBAR_PATTERN_A6;

/* Define the union U_CBAR_PATTERN_A7 */
/* Define the union U_CBAR_PATTERN_A7,ATTR:,ADDR:86C,INIT:00000000,MASK:3fffffff,CMMNT:Colorbar/square patternÀÆ∆Ω≤ Ãı8≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bar_pattern_a7 : 30  ; /* [29:0] */
        unsigned int    rsv_18         : 2  ; /* [31:30] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CBAR_PATTERN_A7;

/* Define the union U_CBAR_PATTERN_B0 */
/* Define the union U_CBAR_PATTERN_B0,ATTR:,ADDR:870,INIT:00000000,MASK:3fffffff,CMMNT:Colorbar/square patternÀÆ∆Ω≤ Ãı1≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bar_pattern_b0 : 30  ; /* [29:0] */
        unsigned int    rsv_19         : 2  ; /* [31:30] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CBAR_PATTERN_B0;

/* Define the union U_CBAR_PATTERN_B1 */
/* Define the union U_CBAR_PATTERN_B1,ATTR:,ADDR:874,INIT:0000021C,MASK:3fffffff,CMMNT:Colorbar/square patternÀÆ∆Ω≤ Ãı2≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bar_pattern_b1 : 30  ; /* [29:0] */
        unsigned int    rsv_20         : 2  ; /* [31:30] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CBAR_PATTERN_B1;

/* Define the union U_CBAR_PATTERN_B2 */
/* Define the union U_CBAR_PATTERN_B2,ATTR:,ADDR:878,INIT:21C00000,MASK:3fffffff,CMMNT:Colorbar/square patternÀÆ∆Ω≤ Ãı3≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bar_pattern_b2 : 30  ; /* [29:0] */
        unsigned int    rsv_21         : 2  ; /* [31:30] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CBAR_PATTERN_B2;

/* Define the union U_CBAR_PATTERN_B3 */
/* Define the union U_CBAR_PATTERN_B3,ATTR:,ADDR:87C,INIT:21C0021C,MASK:3fffffff,CMMNT:Colorbar/square patternÀÆ∆Ω≤ Ãı4≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bar_pattern_b3 : 30  ; /* [29:0] */
        unsigned int    rsv_22         : 2  ; /* [31:30] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CBAR_PATTERN_B3;

/* Define the union U_CBAR_PATTERN_B4 */
/* Define the union U_CBAR_PATTERN_B4,ATTR:,ADDR:880,INIT:00087000,MASK:3fffffff,CMMNT:Colorbar/square patternÀÆ∆Ω≤ Ãı5≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bar_pattern_b4 : 30  ; /* [29:0] */
        unsigned int    rsv_23         : 2  ; /* [31:30] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CBAR_PATTERN_B4;

/* Define the union U_CBAR_PATTERN_B5 */
/* Define the union U_CBAR_PATTERN_B5,ATTR:,ADDR:884,INIT:0008721C,MASK:3fffffff,CMMNT:Colorbar/square patternÀÆ∆Ω≤ Ãı6≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bar_pattern_b5 : 30  ; /* [29:0] */
        unsigned int    rsv_24         : 2  ; /* [31:30] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CBAR_PATTERN_B5;

/* Define the union U_CBAR_PATTERN_B6 */
/* Define the union U_CBAR_PATTERN_B6,ATTR:,ADDR:888,INIT:21C87000,MASK:3fffffff,CMMNT:Colorbar/square patternÀÆ∆Ω≤ Ãı7≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bar_pattern_b6 : 30  ; /* [29:0] */
        unsigned int    rsv_25         : 2  ; /* [31:30] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CBAR_PATTERN_B6;

/* Define the union U_CBAR_PATTERN_B7 */
/* Define the union U_CBAR_PATTERN_B7,ATTR:,ADDR:88C,INIT:3C3FC3FC,MASK:3fffffff,CMMNT:Colorbar/square patternÀÆ∆Ω≤ Ãı8≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    bar_pattern_b7 : 30  ; /* [29:0] */
        unsigned int    rsv_26         : 2  ; /* [31:30] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CBAR_PATTERN_B7;

/* Define the union U_FORMAT_DET_CONFIG */
/* Define the union U_FORMAT_DET_CONFIG,ATTR:,ADDR:8B8,INIT:000000F0,MASK:ff,CMMNT:enFormat detector ≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sync_polarity_force  : 1  ; /* [0] */
        unsigned int    hsync_polarity_value : 1  ; /* [1] */
        unsigned int    vsync_polarity_value : 1  ; /* [2] */
        unsigned int    fdt_status_clear     : 1  ; /* [3] */
        unsigned int    pixel_cnt_threhold   : 4  ; /* [7:4] */
        unsigned int    rsv_27               : 24  ; /* [31:8] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_FORMAT_DET_CONFIG;

/* Define the union U_FDET_STATUS */
/* Define the union U_FDET_STATUS,ATTR:,ADDR:8BC,INIT:00000000,MASK:7,CMMNT:enFormat detector ◊¥Ã¨ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    interlaced     : 1  ; /* [0] */
        unsigned int    hsync_polarity : 1  ; /* [1] */
        unsigned int    vsync_polarity : 1  ; /* [2] */
        unsigned int    rsv_28         : 29  ; /* [31:3] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_FDET_STATUS;

/* Define the union U_FDET_HORI_RES */
/* Define the union U_FDET_HORI_RES,ATTR:,ADDR:8C0,INIT:00000000,MASK:3ffffff,CMMNT:enFormat detector ÀÆ∆Ω ±–Úºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hsync_active_cnt : 13  ; /* [12:0] */
        unsigned int    hsync_total_cnt  : 13  ; /* [25:13] */
        unsigned int    rsv_29           : 6  ; /* [31:26] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_FDET_HORI_RES;

/* Define the union U_FDET_VERT_RES */
/* Define the union U_FDET_VERT_RES,ATTR:,ADDR:8C4,INIT:00000000,MASK:3ffffff,CMMNT:enFormat detector ¥π÷± ±–Úºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vsync_active_cnt : 13  ; /* [12:0] */
        unsigned int    vsync_total_cnt  : 13  ; /* [25:13] */
        unsigned int    rsv_30           : 6  ; /* [31:26] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_FDET_VERT_RES;

/* Define the union U_DITHER_CONFIG */
/* Define the union U_DITHER_CONFIG,ATTR:,ADDR:8EC,INIT:00000001,MASK:3f,CMMNT:Dither/Randƒ£øÈ≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dither_rnd_byp      : 1  ; /* [0] */
        unsigned int    dither_mode         : 2  ; /* [2:1] */
        unsigned int    dither_rnd_en       : 1  ; /* [3] */
        unsigned int    dither_spatial_en   : 1  ; /* [4] */
        unsigned int    dither_spatial_dual : 1  ; /* [5] */
        unsigned int    rsv_31              : 26  ; /* [31:6] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DITHER_CONFIG;

/* Define the union U_CLIP_CTRL */
/* Define the union U_CLIP_CTRL,ATTR:,ADDR:8F8,INIT:00000001,MASK:3,CMMNT:Clipƒ£øÈ≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    range_clip_byp : 1  ; /* [0] */
        unsigned int    clip_rgb_mode  : 1  ; /* [1] */
        unsigned int    rsv_32         : 30  ; /* [31:2] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CLIP_CTRL;

/* Define the union U_CLIP_Y_CONFIG */
/* Define the union U_CLIP_Y_CONFIG,ATTR:,ADDR:8FC,INIT:00FFF000,MASK:ffffff,CMMNT:CLIP Y∑÷¡ø≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    clip_y_min : 12  ; /* [11:0] */
        unsigned int    clip_y_max : 12  ; /* [23:12] */
        unsigned int    rsv_33     : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CLIP_Y_CONFIG;

/* Define the union U_CLIP_C_CONFIG */
/* Define the union U_CLIP_C_CONFIG,ATTR:,ADDR:900,INIT:00FFF000,MASK:ffffff,CMMNT:CLIP C∑÷¡ø≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    clip_c_min : 12  ; /* [11:0] */
        unsigned int    clip_c_max : 12  ; /* [23:12] */
        unsigned int    rsv_34     : 8  ; /* [31:24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CLIP_C_CONFIG;

/* Define the union U_PXL_CAP_CTRL */
/* Define the union U_PXL_CAP_CTRL,ATTR:,ADDR:90C,INIT:00000000,MASK:77,CMMNT:œÒÀÿ≤∂ªÒ≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    auto_trigger_en : 1  ; /* [0] */
        unsigned int    soft_trigger_en : 1  ; /* [1] */
        unsigned int    show_point_en   : 1  ; /* [2] */
        unsigned int    rsv_35          : 1  ; /* [3] */
        unsigned int    cap_stat_done   : 1  ; /* [4] */
        unsigned int    cap_stat_busy   : 1  ; /* [5] */
        unsigned int    cap_stat_error  : 1  ; /* [6] */
        unsigned int    rsv_36          : 25  ; /* [31:7] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PXL_CAP_CTRL;

/* Define the union U_PXL_CAP_POSITION */
/* Define the union U_PXL_CAP_POSITION,ATTR:,ADDR:910,INIT:00000000,MASK:3ffffff,CMMNT:œÒÀÿ≤∂ªÒŒª÷√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cap_pixel_position : 13  ; /* [12:0] */
        unsigned int    cap_line_position  : 13  ; /* [25:13] */
        unsigned int    rsv_37             : 6  ; /* [31:26] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PXL_CAP_POSITION;

/* Define the union U_CAP_Y_VALUE */
/* Define the union U_CAP_Y_VALUE,ATTR:,ADDR:914,INIT:00000000,MASK:fff,CMMNT:*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    capture_y_value : 12  ; /* [11:0] */
        unsigned int    rsv_38          : 20  ; /* [31:12] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CAP_Y_VALUE;

/* Define the union U_CAP_CB_VALUE */
/* Define the union U_CAP_CB_VALUE,ATTR:,ADDR:918,INIT:00000000,MASK:fff,CMMNT:*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    capture_cb_value : 12  ; /* [11:0] */
        unsigned int    rsv_39           : 20  ; /* [31:12] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CAP_CB_VALUE;

/* Define the union U_CAP_CR_VALUE */
/* Define the union U_CAP_CR_VALUE,ATTR:,ADDR:91C,INIT:00000000,MASK:fff,CMMNT:*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    capture_cr_value : 12  ; /* [11:0] */
        unsigned int    rsv_40           : 20  ; /* [31:12] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CAP_CR_VALUE;

/* Define the union U_MULTI_CSC_CTRL */
/* Define the union U_MULTI_CSC_CTRL,ATTR:,ADDR:934,INIT:00000000,MASK:ffffffff,CMMNT:Õ®”√CSC≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reg_csc_en          : 1  ; /* [0] */
        unsigned int    reg_csc_coef_ext    : 1  ; /* [1] */
        unsigned int    reg_csc_dither_en   : 1  ; /* [2] */
        unsigned int    reg_csc_saturate_en : 1  ; /* [3] */
        unsigned int    reg_csc_mode        : 8  ; /* [11:4] */
        unsigned int    rsv_41              : 20  ; /* [31:12] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MULTI_CSC_CTRL;

/* Define the union U_MULTI_CSC_COEFF11 */
/* Define the union U_MULTI_CSC_COEFF11,ATTR:,ADDR:938,INIT:00000000,MASK:ffffffff,CMMNT:Õ®”√CSCœµ ˝≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reg_csc_r1c1 : 16  ; /* [15:0] */
        unsigned int    rsv_42       : 16  ; /* [31:16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MULTI_CSC_COEFF11;

/* Define the union U_MULTI_CSC_COEFF12 */
/* Define the union U_MULTI_CSC_COEFF12,ATTR:,ADDR:93C,INIT:00000000,MASK:ffffffff,CMMNT:Õ®”√CSCœµ ˝≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reg_csc_r1c2 : 16  ; /* [15:0] */
        unsigned int    rsv_43       : 16  ; /* [31:16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MULTI_CSC_COEFF12;

/* Define the union U_MULTI_CSC_COEFF13 */
/* Define the union U_MULTI_CSC_COEFF13,ATTR:,ADDR:940,INIT:00000000,MASK:ffffffff,CMMNT:Õ®”√CSCœµ ˝≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reg_csc_r1c3 : 16  ; /* [15:0] */
        unsigned int    rsv_44       : 16  ; /* [31:16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MULTI_CSC_COEFF13;

/* Define the union U_MULTI_CSC_COEFF21 */
/* Define the union U_MULTI_CSC_COEFF21,ATTR:,ADDR:944,INIT:00000000,MASK:ffffffff,CMMNT:Õ®”√CSCœµ ˝≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reg_csc_r2c1 : 16  ; /* [15:0] */
        unsigned int    rsv_45       : 16  ; /* [31:16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MULTI_CSC_COEFF21;

/* Define the union U_MULTI_CSC_COEFF22 */
/* Define the union U_MULTI_CSC_COEFF22,ATTR:,ADDR:948,INIT:00000000,MASK:ffffffff,CMMNT:Õ®”√CSCœµ ˝≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reg_csc_r2c2 : 16  ; /* [15:0] */
        unsigned int    rsv_46       : 16  ; /* [31:16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MULTI_CSC_COEFF22;

/* Define the union U_MULTI_CSC_COEFF23 */
/* Define the union U_MULTI_CSC_COEFF23,ATTR:,ADDR:94C,INIT:00000000,MASK:ffffffff,CMMNT:Õ®”√CSCœµ ˝≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reg_csc_r2c3 : 16  ; /* [15:0] */
        unsigned int    rsv_47       : 16  ; /* [31:16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MULTI_CSC_COEFF23;

/* Define the union U_MULTI_CSC_COEFF31 */
/* Define the union U_MULTI_CSC_COEFF31,ATTR:,ADDR:950,INIT:00000000,MASK:ffffffff,CMMNT:Õ®”√CSCœµ ˝≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reg_csc_r3c1 : 16  ; /* [15:0] */
        unsigned int    rsv_48       : 16  ; /* [31:16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MULTI_CSC_COEFF31;

/* Define the union U_MULTI_CSC_COEFF32 */
/* Define the union U_MULTI_CSC_COEFF32,ATTR:,ADDR:954,INIT:00000000,MASK:ffffffff,CMMNT:Õ®”√CSCœµ ˝≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reg_csc_r3c2 : 16  ; /* [15:0] */
        unsigned int    rsv_49       : 16  ; /* [31:16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MULTI_CSC_COEFF32;

/* Define the union U_MULTI_CSC_COEFF33 */
/* Define the union U_MULTI_CSC_COEFF33,ATTR:,ADDR:958,INIT:00000000,MASK:ffffffff,CMMNT:Õ®”√CSCœµ ˝≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reg_csc_r3c3 : 16  ; /* [15:0] */
        unsigned int    rsv_50       : 16  ; /* [31:16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MULTI_CSC_COEFF33;

/* Define the union U_MULTI_CSC_INOFFSET_Y */
/* Define the union U_MULTI_CSC_INOFFSET_Y,ATTR:,ADDR:95C,INIT:00000000,MASK:ffffffff,CMMNT:Õ®”√CSC ‰»Î∆´÷√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reg_csc_inoffset_y : 13  ; /* [12:0] */
        unsigned int    rsv_51             : 19  ; /* [31:13] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MULTI_CSC_INOFFSET_Y;

/* Define the union U_MULTI_CSC_INOFFSET_CB */
/* Define the union U_MULTI_CSC_INOFFSET_CB,ATTR:,ADDR:960,INIT:00000000,MASK:ffffffff,CMMNT:Õ®”√CSC ‰»Î∆´÷√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reg_csc_inoffset_cb : 13  ; /* [12:0] */
        unsigned int    rsv_52              : 19  ; /* [31:13] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MULTI_CSC_INOFFSET_CB;

/* Define the union U_MULTI_CSC_INOFFSET_CR */
/* Define the union U_MULTI_CSC_INOFFSET_CR,ATTR:,ADDR:964,INIT:00000000,MASK:ffffffff,CMMNT:Õ®”√CSC ‰»Î∆´÷√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reg_csc_inoffset_cr : 13  ; /* [12:0] */
        unsigned int    rsv_53              : 19  ; /* [31:13] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MULTI_CSC_INOFFSET_CR;

/* Define the union U_MULTI_CSC_OUTOFFSET_Y */
/* Define the union U_MULTI_CSC_OUTOFFSET_Y,ATTR:,ADDR:968,INIT:00000000,MASK:ffffffff,CMMNT:Õ®”√CSC ‰≥ˆ∆´÷√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reg_csc_outoffset_y : 13  ; /* [12:0] */
        unsigned int    rsv_54              : 19  ; /* [31:13] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MULTI_CSC_OUTOFFSET_Y;

/* Define the union U_MULTI_CSC_OUTOFFSET_CB */
/* Define the union U_MULTI_CSC_OUTOFFSET_CB,ATTR:,ADDR:96C,INIT:00000000,MASK:ffffffff,CMMNT:Õ®”√CSC ‰≥ˆ∆´÷√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reg_csc_outoffset_cb : 13  ; /* [12:0] */
        unsigned int    rsv_55               : 19  ; /* [31:13] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MULTI_CSC_OUTOFFSET_CB;

/* Define the union U_MULTI_CSC_OUTOFFSET_CR */
/* Define the union U_MULTI_CSC_OUTOFFSET_CR,ATTR:,ADDR:970,INIT:00000000,MASK:ffffffff,CMMNT:Õ®”√CSC ‰≥ˆ∆´÷√≈‰÷√ºƒ¥Ê∆˜°£*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reg_csc_outoffset_cr : 13  ; /* [12:0] */
        unsigned int    rsv_56               : 19  ; /* [31:13] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MULTI_CSC_OUTOFFSET_CR;

/* Define the union U_VIDEO_DWSM_CTRL */
/* Define the union U_VIDEO_DWSM_CTRL,ATTR:,ADDR:980,INIT:00000008,MASK:ffffffff,CMMNT:±£¡Ùºƒ¥Ê∆˜*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reg_dwsm_hori_en   : 1  ; /* [0] */
        unsigned int    reg_hori_filter_en : 1  ; /* [1] */
        unsigned int    reg_dwsm_vert_en   : 1  ; /* [2] */
        unsigned int    reg_dwsm_vert_byp  : 1  ; /* [3] */
        unsigned int    reg_vert_cbcr_sel  : 1  ; /* [4] */
        unsigned int    rsv_57             : 27  ; /* [31:5] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VIDEO_DWSM_CTRL;

/* Define the union U_DATA_ALIGN_CTRL */
/* Define the union U_DATA_ALIGN_CTRL,ATTR:,ADDR:98C,INIT:00000000,MASK:ffffffff,CMMNT:*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reg_demux_420_en     : 1  ; /* [0] */
        unsigned int    reg_ddr_en           : 1  ; /* [1] */
        unsigned int    reg_yc_mux_en        : 1  ; /* [2] */
        unsigned int    reg_blank_replace_en : 1  ; /* [3] */
        unsigned int    reg_pixel_rate       : 2  ; /* [5:4] */
        unsigned int    reg_ddr_polarity     : 1  ; /* [6] */
        unsigned int    reg_yc_mux_polarity  : 1  ; /* [7] */
        unsigned int    reg_cbcr_order       : 1  ; /* [8] */
        unsigned int    reg_demux_cb_or_cr   : 1  ; /* [9] */
        unsigned int    reg_pxl_div_en       : 1  ; /* [10] */
        unsigned int    rsv_58               : 21  ; /* [31:11] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DATA_ALIGN_CTRL;

/* Define the union U_BLANK_DATA_Y */
/* Define the union U_BLANK_DATA_Y,ATTR:,ADDR:990,INIT:00000000,MASK:ffffffff,CMMNT:*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reg_blank_y : 12  ; /* [11:0] */
        unsigned int    rsv_59      : 20  ; /* [31:12] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_BLANK_DATA_Y;

/* Define the union U_BLANK_DATA_CB */
/* Define the union U_BLANK_DATA_CB,ATTR:,ADDR:994,INIT:00000000,MASK:ffffffff,CMMNT:*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reg_blank_cb : 12  ; /* [11:0] */
        unsigned int    rsv_60       : 20  ; /* [31:12] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_BLANK_DATA_CB;

/* Define the union U_BLANK_DATA_CR */
/* Define the union U_BLANK_DATA_CR,ATTR:,ADDR:998,INIT:00000000,MASK:ffffffff,CMMNT:*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reg_blank_cr : 12  ; /* [11:0] */
        unsigned int    rsv_61       : 20  ; /* [31:12] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_BLANK_DATA_CR;

/* Define the union U_VIDEO_DMUX_CTRL */
/* Define the union U_VIDEO_DMUX_CTRL,ATTR:,ADDR:9A8,INIT:00000088,MASK:ffffffff,CMMNT:*/
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reg_vmux_y_sel  : 3  ; /* [2:0] */
        unsigned int    reg_vmux_cb_sel : 3  ; /* [5:3] */
        unsigned int    reg_vmux_cr_sel : 3  ; /* [8:6] */
        unsigned int    reg_bitmask_y   : 2  ; /* [10:9] */
        unsigned int    reg_bitmask_cb  : 2  ; /* [12:11] */
        unsigned int    reg_bitmask_cr  : 2  ; /* [14:13] */
        unsigned int    reg_bitrev_en   : 3  ; /* [17:15] */
        unsigned int    reg_datamask_en : 3  ; /* [20:18] */
        unsigned int    reg_syncmask_en : 4  ; /* [24:21] */
        unsigned int    reg_inver_sync  : 4  ; /* [28:25] */
        unsigned int    rsv_62          : 3  ; /* [31:29] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VIDEO_DMUX_CTRL;


//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile U_TIMING_GEN_CTRL      TIMING_GEN_CTRL      ; /* 800 */
    volatile U_HSYNC_TIMING_CONFIG0 HSYNC_TIMING_CONFIG0 ; /* 804 */
    volatile U_HSYNC_TIMING_CONFIG1 HSYNC_TIMING_CONFIG1 ; /* 808 */
    volatile U_HSYNC_TIMING_CONFIG2 HSYNC_TIMING_CONFIG2 ; /* 80C */
    volatile U_VSYNC_TIMING_CONFIG0 VSYNC_TIMING_CONFIG0 ; /* 810 */
    volatile U_VSYNC_TIMING_CONFIG1 VSYNC_TIMING_CONFIG1 ; /* 814 */
    volatile U_VSYNC_TIMING_CONFIG2 VSYNC_TIMING_CONFIG2 ; /* 818 */
    unsigned int                                  Reserved_0[9];
    volatile U_PATTERN_GEN_CTRL     PATTERN_GEN_CTRL     ; /* 840 */
    volatile U_SOLID_PATTERN_CONFIG SOLID_PATTERN_CONFIG ; /* 844 */
    volatile U_MASK_PATTERN_CONFIG  MASK_PATTERN_CONFIG  ; /* 848 */
    volatile U_BAR_EXT_CONFIG       BAR_EXT_CONFIG       ; /* 84C */
    volatile U_CBAR_PATTERN_A0      CBAR_PATTERN_A0      ; /* 850 */
    volatile U_CBAR_PATTERN_A1      CBAR_PATTERN_A1      ; /* 854 */
    volatile U_CBAR_PATTERN_A2      CBAR_PATTERN_A2      ; /* 858 */
    volatile U_CBAR_PATTERN_A3      CBAR_PATTERN_A3      ; /* 85C */
    volatile U_CBAR_PATTERN_A4      CBAR_PATTERN_A4      ; /* 860 */
    volatile U_CBAR_PATTERN_A5      CBAR_PATTERN_A5      ; /* 864 */
    volatile U_CBAR_PATTERN_A6      CBAR_PATTERN_A6      ; /* 868 */
    volatile U_CBAR_PATTERN_A7      CBAR_PATTERN_A7      ; /* 86C */
    volatile U_CBAR_PATTERN_B0      CBAR_PATTERN_B0      ; /* 870 */
    volatile U_CBAR_PATTERN_B1      CBAR_PATTERN_B1      ; /* 874 */
    volatile U_CBAR_PATTERN_B2      CBAR_PATTERN_B2      ; /* 878 */
    volatile U_CBAR_PATTERN_B3      CBAR_PATTERN_B3      ; /* 87C */
    volatile U_CBAR_PATTERN_B4      CBAR_PATTERN_B4      ; /* 880 */
    volatile U_CBAR_PATTERN_B5      CBAR_PATTERN_B5      ; /* 884 */
    volatile U_CBAR_PATTERN_B6      CBAR_PATTERN_B6      ; /* 888 */
    volatile U_CBAR_PATTERN_B7      CBAR_PATTERN_B7      ; /* 88C */
	    unsigned int                                  Reserved_1[10];
    volatile U_FORMAT_DET_CONFIG    FORMAT_DET_CONFIG    ; /* 8B8 */
    volatile U_FDET_STATUS          FDET_STATUS          ; /* 8BC */
    volatile U_FDET_HORI_RES        FDET_HORI_RES        ; /* 8C0 */
    volatile U_FDET_VERT_RES        FDET_VERT_RES        ; /* 8C4 */
	    unsigned int                                  Reserved_2[9];
    volatile U_DITHER_CONFIG        DITHER_CONFIG        ; /* 8EC */
	    unsigned int                                  Reserved_3[2];
    volatile U_CLIP_CTRL            CLIP_CTRL            ; /* 8F8 */
    volatile U_CLIP_Y_CONFIG        CLIP_Y_CONFIG        ; /* 8FC */
    volatile U_CLIP_C_CONFIG        CLIP_C_CONFIG        ; /* 900 */
	    unsigned int                                  Reserved_4[2];
    volatile U_PXL_CAP_CTRL         PXL_CAP_CTRL         ; /* 90C */
    volatile U_PXL_CAP_POSITION     PXL_CAP_POSITION     ; /* 910 */
    volatile U_CAP_Y_VALUE          CAP_Y_VALUE          ; /* 914 */
    volatile U_CAP_CB_VALUE         CAP_CB_VALUE         ; /* 918 */
    volatile U_CAP_CR_VALUE         CAP_CR_VALUE         ; /* 91C */
        unsigned int                                  Reserved_5[5];/* 920-930 */
    volatile U_MULTI_CSC_CTRL         MULTI_CSC_CTRL         ; /* 934 */
    volatile U_MULTI_CSC_COEFF11      MULTI_CSC_COEFF11      ; /* 938 */
    volatile U_MULTI_CSC_COEFF12      MULTI_CSC_COEFF12      ; /* 93C */
    volatile U_MULTI_CSC_COEFF13      MULTI_CSC_COEFF13      ; /* 940 */
    volatile U_MULTI_CSC_COEFF21      MULTI_CSC_COEFF21      ; /* 944 */
    volatile U_MULTI_CSC_COEFF22      MULTI_CSC_COEFF22      ; /* 948 */
    volatile U_MULTI_CSC_COEFF23      MULTI_CSC_COEFF23      ; /* 94C */
    volatile U_MULTI_CSC_COEFF31      MULTI_CSC_COEFF31      ; /* 950 */
    volatile U_MULTI_CSC_COEFF32      MULTI_CSC_COEFF32      ; /* 954 */
    volatile U_MULTI_CSC_COEFF33      MULTI_CSC_COEFF33      ; /* 958 */
    volatile U_MULTI_CSC_INOFFSET_Y   MULTI_CSC_INOFFSET_Y   ; /* 95C */
    volatile U_MULTI_CSC_INOFFSET_CB  MULTI_CSC_INOFFSET_CB  ; /* 960 */
    volatile U_MULTI_CSC_INOFFSET_CR  MULTI_CSC_INOFFSET_CR  ; /* 964 */
    volatile U_MULTI_CSC_OUTOFFSET_Y  MULTI_CSC_OUTOFFSET_Y  ; /* 968 */
    volatile U_MULTI_CSC_OUTOFFSET_CB MULTI_CSC_OUTOFFSET_CB ; /* 96C */
    volatile U_MULTI_CSC_OUTOFFSET_CR MULTI_CSC_OUTOFFSET_CR ; /* 970 */
          unsigned int                                  Reserved_6[3];/* 974-97c */
    volatile U_VIDEO_DWSM_CTRL        VIDEO_DWSM_CTRL        ; /* 980 */
	   unsigned int                                  Reserved_7[2];/* 984-988 */
    volatile U_DATA_ALIGN_CTRL        DATA_ALIGN_CTRL        ; /* 98C */
    volatile U_BLANK_DATA_Y           BLANK_DATA_Y           ; /* 990 */
    volatile U_BLANK_DATA_CB          BLANK_DATA_CB          ; /* 994 */
    volatile U_BLANK_DATA_CR          BLANK_DATA_CR          ; /* 998 */
	   unsigned int                                  Reserved_8[3];/* 99C-9A4 */
    volatile U_VIDEO_DMUX_CTRL        VIDEO_DMUX_CTRL        ; /* 9A8 */

} S_video_path_reg_REGS_TYPE;

/* Declare the struct pointor of the module video_path_reg */
//extern volatile S_video_path_reg_REGS_TYPE *gopvideo_path_regAllReg;
int HDMI_TX_S_video_path_reg_REGS_TYPE_Init(void);
int HDMI_TX_S_video_path_reg_REGS_TYPE_DeInit(void);

/* Declare the functions that set the member value */
int HDMI_TIMING_GEN_CTRL_reg_timing_gen_enSet(unsigned int ureg_timing_gen_en);
int HDMI_TIMING_GEN_CTRL_reg_timing_gen_enGet(HI_VOID);
int HDMI_TIMING_GEN_CTRL_reg_extmodeSet(unsigned int ureg_extmode);
int HDMI_TIMING_GEN_CTRL_reg_extmodeGet(HI_VOID);
int HDMI_TIMING_GEN_CTRL_reg_timing_selSet(unsigned int ureg_timing_sel);
int HDMI_TIMING_GEN_CTRL_reg_timing_selGet(HI_VOID);
int HDMI_TIMING_GEN_CTRL_reg_sync_polaritySet(unsigned int ureg_sync_polarity);
int HDMI_TIMING_GEN_CTRL_reg_sync_polarityGet(HI_VOID);
int HDMI_HSYNC_TIMING_CONFIG0_hsync_total_countSet(unsigned int uhsync_total_count);
int HDMI_HSYNC_TIMING_CONFIG0_hsync_total_countGet(HI_VOID);
int HDMI_HSYNC_TIMING_CONFIG0_hsync_high_to_low_countSet(unsigned int uhsync_high_to_low_count);
int HDMI_HSYNC_TIMING_CONFIG0_hsync_high_to_low_countGet(HI_VOID);
int HDMI_HSYNC_TIMING_CONFIG1_hsync_de_start_countSet(unsigned int uhsync_de_start_count);
int HDMI_HSYNC_TIMING_CONFIG1_hsync_de_start_countGet(HI_VOID);
int HDMI_HSYNC_TIMING_CONFIG1_hsync_de_end_countSet(unsigned int uhsync_de_end_count);
int HDMI_HSYNC_TIMING_CONFIG1_hsync_de_end_countGet(HI_VOID);
int HDMI_HSYNC_TIMING_CONFIG2_hsync_low_to_high_countSet(unsigned int uhsync_low_to_high_count);
int HDMI_HSYNC_TIMING_CONFIG2_hsync_low_to_high_countGet(HI_VOID);
int HDMI_VSYNC_TIMING_CONFIG0_vsync_total_countSet(unsigned int uvsync_total_count);
int HDMI_VSYNC_TIMING_CONFIG0_vsync_total_countGet(HI_VOID);
int HDMI_VSYNC_TIMING_CONFIG0_vsync_high_to_low_countSet(unsigned int uvsync_high_to_low_count);
int HDMI_VSYNC_TIMING_CONFIG0_vsync_high_to_low_countGet(HI_VOID);
int HDMI_VSYNC_TIMING_CONFIG1_vsync_de_start_countSet(unsigned int uvsync_de_start_count);
int HDMI_VSYNC_TIMING_CONFIG1_vsync_de_start_countGet(HI_VOID);
int HDMI_VSYNC_TIMING_CONFIG1_vsync_de_end_countSet(unsigned int uvsync_de_end_count);
int HDMI_VSYNC_TIMING_CONFIG1_vsync_de_end_countGet(HI_VOID);
int HDMI_VSYNC_TIMING_CONFIG2_vsync_low_to_high_countSet(unsigned int uvsync_low_to_high_count);
int HDMI_VSYNC_TIMING_CONFIG2_vsync_low_to_high_countGet(HI_VOID);
int HDMI_PATTERN_GEN_CTRL_tpg_enableSet(unsigned int utpg_enable);
int HDMI_PATTERN_GEN_CTRL_tpg_enableGet(HI_VOID);
int HDMI_PATTERN_GEN_CTRL_video_formatSet(unsigned int uvideo_format);
int HDMI_PATTERN_GEN_CTRL_video_formatGet(HI_VOID);
int HDMI_PATTERN_GEN_CTRL_solid_pattern_enSet(unsigned int usolid_pattern_en);
int HDMI_PATTERN_GEN_CTRL_solid_pattern_enGet(HI_VOID);
int HDMI_PATTERN_GEN_CTRL_colorbar_enSet(unsigned int ucolorbar_en);
int HDMI_PATTERN_GEN_CTRL_colorbar_enGet(HI_VOID);
int HDMI_PATTERN_GEN_CTRL_square_pattern_enSet(unsigned int usquare_pattern_en);
int HDMI_PATTERN_GEN_CTRL_square_pattern_enGet(HI_VOID);
int HDMI_PATTERN_GEN_CTRL_mask_pattern_enSet(unsigned int umask_pattern_en);
int HDMI_PATTERN_GEN_CTRL_mask_pattern_enGet(HI_VOID);
int HDMI_PATTERN_GEN_CTRL_replace_pattern_enSet(unsigned int ureplace_pattern_en);
int HDMI_PATTERN_GEN_CTRL_replace_pattern_enGet(HI_VOID);
int HDMI_PATTERN_GEN_CTRL_bar_pattern_extmodeSet(unsigned int ubar_pattern_extmode);
int HDMI_PATTERN_GEN_CTRL_bar_pattern_extmodeGet(HI_VOID);
int HDMI_PATTERN_GEN_CTRL_cbar_pattern_selSet(unsigned int ucbar_pattern_sel);
int HDMI_PATTERN_GEN_CTRL_cbar_pattern_selGet(HI_VOID);
int HDMI_PATTERN_GEN_CTRL_mix_color_enSet(unsigned int umix_color_en);
int HDMI_PATTERN_GEN_CTRL_mix_color_enGet(HI_VOID);
int HDMI_PATTERN_GEN_CTRL_increase_enSet(unsigned int uincrease_en);
int HDMI_PATTERN_GEN_CTRL_increase_enGet(HI_VOID);
int HDMI_SOLID_PATTERN_CONFIG_solid_pattern_cbSet(unsigned int usolid_pattern_cb);
int HDMI_SOLID_PATTERN_CONFIG_solid_pattern_cbGet(HI_VOID);
int HDMI_SOLID_PATTERN_CONFIG_solid_pattern_ySet(unsigned int usolid_pattern_y);
int HDMI_SOLID_PATTERN_CONFIG_solid_pattern_yGet(HI_VOID);
int HDMI_SOLID_PATTERN_CONFIG_solid_pattern_crSet(unsigned int usolid_pattern_cr);
int HDMI_SOLID_PATTERN_CONFIG_solid_pattern_crGet(HI_VOID);
int HDMI_MASK_PATTERN_CONFIG_mask_pattern_cbSet(unsigned int umask_pattern_cb);
int HDMI_MASK_PATTERN_CONFIG_mask_pattern_cbGet(HI_VOID);
int HDMI_MASK_PATTERN_CONFIG_mask_pattern_ySet(unsigned int umask_pattern_y);
int HDMI_MASK_PATTERN_CONFIG_mask_pattern_yGet(HI_VOID);
int HDMI_MASK_PATTERN_CONFIG_mask_pattern_crSet(unsigned int umask_pattern_cr);
int HDMI_MASK_PATTERN_CONFIG_mask_pattern_crGet(HI_VOID);
int HDMI_BAR_EXT_CONFIG_colorbar_widthSet(unsigned int ucolorbar_width);
int HDMI_BAR_EXT_CONFIG_colorbar_widthGet(HI_VOID);
int HDMI_BAR_EXT_CONFIG_square_heightSet(unsigned int usquare_height);
int HDMI_BAR_EXT_CONFIG_square_heightGet(HI_VOID);
int HDMI_CBAR_PATTERN_A0_bar_pattern_a0Set(unsigned int ubar_pattern_a0);
int HDMI_CBAR_PATTERN_A0_bar_pattern_a0Get(HI_VOID);
int HDMI_CBAR_PATTERN_A1_bar_pattern_a1Set(unsigned int ubar_pattern_a1);
int HDMI_CBAR_PATTERN_A1_bar_pattern_a1Get(HI_VOID);
int HDMI_CBAR_PATTERN_A2_bar_pattern_a2Set(unsigned int ubar_pattern_a2);
int HDMI_CBAR_PATTERN_A2_bar_pattern_a2Get(HI_VOID);
int HDMI_CBAR_PATTERN_A3_bar_pattern_a3Set(unsigned int ubar_pattern_a3);
int HDMI_CBAR_PATTERN_A3_bar_pattern_a3Get(HI_VOID);
int HDMI_CBAR_PATTERN_A4_bar_pattern_a4Set(unsigned int ubar_pattern_a4);
int HDMI_CBAR_PATTERN_A4_bar_pattern_a4Get(HI_VOID);
int HDMI_CBAR_PATTERN_A5_bar_pattern_a5Set(unsigned int ubar_pattern_a5);
int HDMI_CBAR_PATTERN_A5_bar_pattern_a5Get(HI_VOID);
int HDMI_CBAR_PATTERN_A6_bar_pattern_a6Set(unsigned int ubar_pattern_a6);
int HDMI_CBAR_PATTERN_A6_bar_pattern_a6Get(HI_VOID);
int HDMI_CBAR_PATTERN_A7_bar_pattern_a7Set(unsigned int ubar_pattern_a7);
int HDMI_CBAR_PATTERN_A7_bar_pattern_a7Get(HI_VOID);
int HDMI_CBAR_PATTERN_B0_bar_pattern_b0Set(unsigned int ubar_pattern_b0);
int HDMI_CBAR_PATTERN_B0_bar_pattern_b0Get(HI_VOID);
int HDMI_CBAR_PATTERN_B1_bar_pattern_b1Set(unsigned int ubar_pattern_b1);
int HDMI_CBAR_PATTERN_B1_bar_pattern_b1Get(HI_VOID);
int HDMI_CBAR_PATTERN_B2_bar_pattern_b2Set(unsigned int ubar_pattern_b2);
int HDMI_CBAR_PATTERN_B2_bar_pattern_b2Get(HI_VOID);
int HDMI_CBAR_PATTERN_B3_bar_pattern_b3Set(unsigned int ubar_pattern_b3);
int HDMI_CBAR_PATTERN_B3_bar_pattern_b3Get(HI_VOID);
int HDMI_CBAR_PATTERN_B4_bar_pattern_b4Set(unsigned int ubar_pattern_b4);
int HDMI_CBAR_PATTERN_B4_bar_pattern_b4Get(HI_VOID);
int HDMI_CBAR_PATTERN_B5_bar_pattern_b5Set(unsigned int ubar_pattern_b5);
int HDMI_CBAR_PATTERN_B5_bar_pattern_b5Get(HI_VOID);
int HDMI_CBAR_PATTERN_B6_bar_pattern_b6Set(unsigned int ubar_pattern_b6);
int HDMI_CBAR_PATTERN_B6_bar_pattern_b6Get(HI_VOID);
int HDMI_CBAR_PATTERN_B7_bar_pattern_b7Set(unsigned int ubar_pattern_b7);
int HDMI_CBAR_PATTERN_B7_bar_pattern_b7Get(HI_VOID);
int HDMI_FORMAT_DET_CONFIG_sync_polarity_forceSet(unsigned int usync_polarity_force);
int HDMI_FORMAT_DET_CONFIG_sync_polarity_forceGet(HI_VOID);
int HDMI_FORMAT_DET_CONFIG_hsync_polarity_valueSet(unsigned int uhsync_polarity_value);
int HDMI_FORMAT_DET_CONFIG_hsync_polarity_valueGet(HI_VOID);
int HDMI_FORMAT_DET_CONFIG_vsync_polarity_valueSet(unsigned int uvsync_polarity_value);
int HDMI_FORMAT_DET_CONFIG_vsync_polarity_valueGet(HI_VOID);
int HDMI_FORMAT_DET_CONFIG_fdt_status_clearSet(unsigned int ufdt_status_clear);
int HDMI_FORMAT_DET_CONFIG_fdt_status_clearGet(HI_VOID);
int HDMI_FORMAT_DET_CONFIG_pixel_cnt_threholdSet(unsigned int upixel_cnt_threhold);
int HDMI_FORMAT_DET_CONFIG_pixel_cnt_threholdGet(HI_VOID);
int HDMI_FDET_STATUS_interlacedSet(unsigned int uinterlaced);
int HDMI_FDET_STATUS_interlacedGet(HI_VOID);
int HDMI_FDET_STATUS_hsync_polaritySet(unsigned int uhsync_polarity);
int HDMI_FDET_STATUS_hsync_polarityGet(HI_VOID);
int HDMI_FDET_STATUS_vsync_polaritySet(unsigned int uvsync_polarity);
int HDMI_FDET_STATUS_vsync_polarityGet(HI_VOID);
int HDMI_FDET_HORI_RES_hsync_active_cntSet(unsigned int uhsync_active_cnt);
int HDMI_FDET_HORI_RES_hsync_active_cntGet(HI_VOID);
int HDMI_FDET_HORI_RES_hsync_total_cntSet(unsigned int uhsync_total_cnt);
int HDMI_FDET_HORI_RES_hsync_total_cntGet(HI_VOID);
int HDMI_FDET_VERT_RES_vsync_active_cntSet(unsigned int uvsync_active_cnt);
int HDMI_FDET_VERT_RES_vsync_active_cntGet(HI_VOID);
int HDMI_FDET_VERT_RES_vsync_total_cntSet(unsigned int uvsync_total_cnt);
int HDMI_FDET_VERT_RES_vsync_total_cntGet(HI_VOID);
int HDMI_DITHER_CONFIG_dither_rnd_bypSet(unsigned int udither_rnd_byp);
int HDMI_DITHER_CONFIG_dither_rnd_bypGet(HI_VOID);
int HDMI_DITHER_CONFIG_dither_modeSet(unsigned int udither_mode);
int HDMI_DITHER_CONFIG_dither_modeGet(HI_VOID);
int HDMI_DITHER_CONFIG_dither_rnd_enSet(unsigned int udither_rnd_en);
int HDMI_DITHER_CONFIG_dither_rnd_enGet(HI_VOID);
int HDMI_DITHER_CONFIG_dither_spatial_enSet(unsigned int udither_spatial_en);
int HDMI_DITHER_CONFIG_dither_spatial_enGet(HI_VOID);
int HDMI_DITHER_CONFIG_dither_spatial_dualSet(unsigned int udither_spatial_dual);
int HDMI_DITHER_CONFIG_dither_spatial_dualGet(HI_VOID);
int HDMI_CLIP_CTRL_range_clip_bypSet(unsigned int urange_clip_byp);
int HDMI_CLIP_CTRL_range_clip_bypGet(HI_VOID);
int HDMI_CLIP_CTRL_clip_rgb_modeSet(unsigned int uclip_rgb_mode);
int HDMI_CLIP_CTRL_clip_rgb_modeGet(HI_VOID);
int HDMI_CLIP_Y_CONFIG_clip_y_minSet(unsigned int uclip_y_min);
int HDMI_CLIP_Y_CONFIG_clip_y_minGet(HI_VOID);
int HDMI_CLIP_Y_CONFIG_clip_y_maxSet(unsigned int uclip_y_max);
int HDMI_CLIP_Y_CONFIG_clip_y_maxGet(HI_VOID);
int HDMI_CLIP_C_CONFIG_clip_c_minSet(unsigned int uclip_c_min);
int HDMI_CLIP_C_CONFIG_clip_c_minGet(HI_VOID);
int HDMI_CLIP_C_CONFIG_clip_c_maxSet(unsigned int uclip_c_max);
int HDMI_CLIP_C_CONFIG_clip_c_maxGet(HI_VOID);
int HDMI_PXL_CAP_CTRL_auto_trigger_enSet(unsigned int uauto_trigger_en);
int HDMI_PXL_CAP_CTRL_auto_trigger_enGet(HI_VOID);
int HDMI_PXL_CAP_CTRL_soft_trigger_enSet(unsigned int usoft_trigger_en);
int HDMI_PXL_CAP_CTRL_soft_trigger_enGet(HI_VOID);
int HDMI_PXL_CAP_CTRL_show_point_enSet(unsigned int ushow_point_en);
int HDMI_PXL_CAP_CTRL_show_point_enGet(HI_VOID);
int HDMI_PXL_CAP_CTRL_cap_stat_doneSet(unsigned int ucap_stat_done);
int HDMI_PXL_CAP_CTRL_cap_stat_doneGet(HI_VOID);
int HDMI_PXL_CAP_CTRL_cap_stat_busySet(unsigned int ucap_stat_busy);
int HDMI_PXL_CAP_CTRL_cap_stat_busyGet(HI_VOID);
int HDMI_PXL_CAP_CTRL_cap_stat_errorSet(unsigned int ucap_stat_error);
int HDMI_PXL_CAP_CTRL_cap_stat_errorGet(HI_VOID);
int HDMI_PXL_CAP_POSITION_cap_pixel_positionSet(unsigned int ucap_pixel_position);
int HDMI_PXL_CAP_POSITION_cap_pixel_positionGet(HI_VOID);
int HDMI_PXL_CAP_POSITION_cap_line_positionSet(unsigned int ucap_line_position);
int HDMI_PXL_CAP_POSITION_cap_line_positionGet(HI_VOID);
int HDMI_CAP_Y_VALUE_capture_y_valueSet(unsigned int ucapture_y_value);
int HDMI_CAP_Y_VALUE_capture_y_valueGet(HI_VOID);
int HDMI_CAP_CB_VALUE_capture_cb_valueSet(unsigned int ucapture_cb_value);
int HDMI_CAP_CB_VALUE_capture_cb_valueGet(HI_VOID);
int HDMI_CAP_CR_VALUE_capture_cr_valueSet(unsigned int ucapture_cr_value);
int HDMI_CAP_CR_VALUE_capture_cr_valueGet(HI_VOID);
int HDMI_MULTI_CSC_CTRL_reg_csc_enSet(unsigned int ureg_csc_en);
int HDMI_MULTI_CSC_CTRL_reg_csc_enGet(HI_VOID);
int HDMI_MULTI_CSC_CTRL_reg_csc_coef_extSet(unsigned int ureg_csc_coef_ext);
int HDMI_MULTI_CSC_CTRL_reg_csc_coef_extGet(HI_VOID);
int HDMI_MULTI_CSC_CTRL_reg_csc_dither_enSet(unsigned int ureg_csc_dither_en);
int HDMI_MULTI_CSC_CTRL_reg_csc_dither_enGet(HI_VOID);
int HDMI_MULTI_CSC_CTRL_reg_csc_saturate_enSet(unsigned int ureg_csc_saturate_en);
int HDMI_MULTI_CSC_CTRL_reg_csc_saturate_enGet(HI_VOID);
int HDMI_MULTI_CSC_CTRL_reg_csc_modeSet(unsigned int ureg_csc_mode);
int HDMI_MULTI_CSC_CTRL_reg_csc_modeGet(HI_VOID);
int HDMI_MULTI_CSC_COEFF11_reg_csc_r1c1Set(unsigned int ureg_csc_r1c1);
int HDMI_MULTI_CSC_COEFF11_reg_csc_r1c1Get(HI_VOID);
int HDMI_MULTI_CSC_COEFF12_reg_csc_r1c2Set(unsigned int ureg_csc_r1c2);
int HDMI_MULTI_CSC_COEFF12_reg_csc_r1c2Get(HI_VOID);
int HDMI_MULTI_CSC_COEFF13_reg_csc_r1c3Set(unsigned int ureg_csc_r1c3);
int HDMI_MULTI_CSC_COEFF13_reg_csc_r1c3Get(HI_VOID);
int HDMI_MULTI_CSC_COEFF21_reg_csc_r2c1Set(unsigned int ureg_csc_r2c1);
int HDMI_MULTI_CSC_COEFF21_reg_csc_r2c1Get(HI_VOID);
int HDMI_MULTI_CSC_COEFF22_reg_csc_r2c2Set(unsigned int ureg_csc_r2c2);
int HDMI_MULTI_CSC_COEFF22_reg_csc_r2c2Get(HI_VOID);
int HDMI_MULTI_CSC_COEFF23_reg_csc_r2c3Set(unsigned int ureg_csc_r2c3);
int HDMI_MULTI_CSC_COEFF23_reg_csc_r2c3Get(HI_VOID);
int HDMI_MULTI_CSC_COEFF31_reg_csc_r3c1Set(unsigned int ureg_csc_r3c1);
int HDMI_MULTI_CSC_COEFF31_reg_csc_r3c1Get(HI_VOID);
int HDMI_MULTI_CSC_COEFF32_reg_csc_r3c2Set(unsigned int ureg_csc_r3c2);
int HDMI_MULTI_CSC_COEFF32_reg_csc_r3c2Get(HI_VOID);
int HDMI_MULTI_CSC_COEFF33_reg_csc_r3c3Set(unsigned int ureg_csc_r3c3);
int HDMI_MULTI_CSC_COEFF33_reg_csc_r3c3Get(HI_VOID);
int HDMI_MULTI_CSC_INOFFSET_Y_reg_csc_inoffset_ySet(unsigned int ureg_csc_inoffset_y);
int HDMI_MULTI_CSC_INOFFSET_Y_reg_csc_inoffset_yGet(HI_VOID);
int HDMI_MULTI_CSC_INOFFSET_CB_reg_csc_inoffset_cbSet(unsigned int ureg_csc_inoffset_cb);
int HDMI_MULTI_CSC_INOFFSET_CB_reg_csc_inoffset_cbGet(HI_VOID);
int HDMI_MULTI_CSC_INOFFSET_CR_reg_csc_inoffset_crSet(unsigned int ureg_csc_inoffset_cr);
int HDMI_MULTI_CSC_INOFFSET_CR_reg_csc_inoffset_crGet(HI_VOID);
int HDMI_MULTI_CSC_OUTOFFSET_Y_reg_csc_outoffset_ySet(unsigned int ureg_csc_outoffset_y);
int HDMI_MULTI_CSC_OUTOFFSET_Y_reg_csc_outoffset_yGet(HI_VOID);
int HDMI_MULTI_CSC_OUTOFFSET_CB_reg_csc_outoffset_cbSet(unsigned int ureg_csc_outoffset_cb);
int HDMI_MULTI_CSC_OUTOFFSET_CB_reg_csc_outoffset_cbGet(HI_VOID);
int HDMI_MULTI_CSC_OUTOFFSET_CR_reg_csc_outoffset_crSet(unsigned int ureg_csc_outoffset_cr);
int HDMI_MULTI_CSC_OUTOFFSET_CR_reg_csc_outoffset_crGet(HI_VOID);
int HDMI_VIDEO_DWSM_CTRL_reg_dwsm_hori_enSet(unsigned int ureg_dwsm_hori_en);
int HDMI_VIDEO_DWSM_CTRL_reg_dwsm_hori_enGet(HI_VOID);
int HDMI_VIDEO_DWSM_CTRL_reg_hori_filter_enSet(unsigned int ureg_hori_filter_en);
int HDMI_VIDEO_DWSM_CTRL_reg_hori_filter_enGet(HI_VOID);
int HDMI_VIDEO_DWSM_CTRL_reg_dwsm_vert_enSet(unsigned int ureg_dwsm_vert_en);
int HDMI_VIDEO_DWSM_CTRL_reg_dwsm_vert_enGet(HI_VOID);
int HDMI_VIDEO_DWSM_CTRL_reg_dwsm_vert_bypSet(unsigned int ureg_dwsm_vert_byp);
int HDMI_VIDEO_DWSM_CTRL_reg_dwsm_vert_bypGet(HI_VOID);
int HDMI_VIDEO_DWSM_CTRL_reg_vert_cbcr_selSet(unsigned int ureg_vert_cbcr_sel);
int HDMI_VIDEO_DWSM_CTRL_reg_vert_cbcr_selGet(HI_VOID);
int HDMI_DATA_ALIGN_CTRL_reg_demux_420_enSet(unsigned int ureg_demux_420_en);
int HDMI_DATA_ALIGN_CTRL_reg_demux_420_enGet(HI_VOID);
int HDMI_DATA_ALIGN_CTRL_reg_ddr_enSet(unsigned int ureg_ddr_en);
int HDMI_DATA_ALIGN_CTRL_reg_ddr_enGet(HI_VOID);
int HDMI_DATA_ALIGN_CTRL_reg_yc_mux_enSet(unsigned int ureg_yc_mux_en);
int HDMI_DATA_ALIGN_CTRL_reg_yc_mux_enGet(HI_VOID);
int HDMI_DATA_ALIGN_CTRL_reg_blank_replace_enSet(unsigned int ureg_blank_replace_en);
int HDMI_DATA_ALIGN_CTRL_reg_blank_replace_enGet(HI_VOID);
int HDMI_DATA_ALIGN_CTRL_reg_pixel_rateSet(unsigned int ureg_pixel_rate);
int HDMI_DATA_ALIGN_CTRL_reg_pixel_rateGet(HI_VOID);
int HDMI_DATA_ALIGN_CTRL_reg_ddr_polaritySet(unsigned int ureg_ddr_polarity);
int HDMI_DATA_ALIGN_CTRL_reg_ddr_polarityGet(HI_VOID);
int HDMI_DATA_ALIGN_CTRL_reg_yc_mux_polaritySet(unsigned int ureg_yc_mux_polarity);
int HDMI_DATA_ALIGN_CTRL_reg_yc_mux_polarityGet(HI_VOID);
int HDMI_DATA_ALIGN_CTRL_reg_cbcr_orderSet(unsigned int ureg_cbcr_order);
int HDMI_DATA_ALIGN_CTRL_reg_cbcr_orderGet(HI_VOID);
int HDMI_DATA_ALIGN_CTRL_reg_demux_cb_or_crSet(unsigned int ureg_demux_cb_or_cr);
int HDMI_DATA_ALIGN_CTRL_reg_demux_cb_or_crGet(HI_VOID);
int HDMI_DATA_ALIGN_CTRL_reg_pxl_div_enSet(unsigned int ureg_pxl_div_en);
int HDMI_DATA_ALIGN_CTRL_reg_pxl_div_enGet(HI_VOID);
int HDMI_BLANK_DATA_Y_reg_blank_ySet(unsigned int ureg_blank_y);
int HDMI_BLANK_DATA_Y_reg_blank_yGet(HI_VOID);
int HDMI_BLANK_DATA_CB_reg_blank_cbSet(unsigned int ureg_blank_cb);
int HDMI_BLANK_DATA_CB_reg_blank_cbGet(HI_VOID);
int HDMI_BLANK_DATA_CR_reg_blank_crSet(unsigned int ureg_blank_cr);
int HDMI_BLANK_DATA_CR_reg_blank_crGet(HI_VOID);
int HDMI_VIDEO_DMUX_CTRL_reg_vmux_y_selSet(unsigned int ureg_vmux_y_sel);
int HDMI_VIDEO_DMUX_CTRL_reg_vmux_y_selGet(HI_VOID);
int HDMI_VIDEO_DMUX_CTRL_reg_vmux_cb_selSet(unsigned int ureg_vmux_cb_sel);
int HDMI_VIDEO_DMUX_CTRL_reg_vmux_cb_selGet(HI_VOID);
int HDMI_VIDEO_DMUX_CTRL_reg_vmux_cr_selSet(unsigned int ureg_vmux_cr_sel);
int HDMI_VIDEO_DMUX_CTRL_reg_vmux_cr_selGet(HI_VOID);
int HDMI_VIDEO_DMUX_CTRL_reg_bitmask_ySet(unsigned int ureg_bitmask_y);
int HDMI_VIDEO_DMUX_CTRL_reg_bitmask_yGet(HI_VOID);
int HDMI_VIDEO_DMUX_CTRL_reg_bitmask_cbSet(unsigned int ureg_bitmask_cb);
int HDMI_VIDEO_DMUX_CTRL_reg_bitmask_cbGet(HI_VOID);
int HDMI_VIDEO_DMUX_CTRL_reg_bitmask_crSet(unsigned int ureg_bitmask_cr);
int HDMI_VIDEO_DMUX_CTRL_reg_bitmask_crGet(HI_VOID);
int HDMI_VIDEO_DMUX_CTRL_reg_bitrev_enSet(unsigned int ureg_bitrev_en);
int HDMI_VIDEO_DMUX_CTRL_reg_bitrev_enGet(HI_VOID);
int HDMI_VIDEO_DMUX_CTRL_reg_datamask_enSet(unsigned int ureg_datamask_en);
int HDMI_VIDEO_DMUX_CTRL_reg_datamask_enGet(HI_VOID);
int HDMI_VIDEO_DMUX_CTRL_reg_syncmask_enSet(unsigned int ureg_syncmask_en);
int HDMI_VIDEO_DMUX_CTRL_reg_syncmask_enGet(HI_VOID);
int HDMI_VIDEO_DMUX_CTRL_reg_inver_syncSet(unsigned int ureg_inver_sync);
int HDMI_VIDEO_DMUX_CTRL_reg_inver_syncGet(HI_VOID);
#endif // __VIDEO_PATH_REG_C_UNION_DEFINE_H__
