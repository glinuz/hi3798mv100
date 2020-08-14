//******************************************************************************
//  Copyright (C), 2007-2016, Hisilicon Technologies Co., Ltd.
//
//******************************************************************************
// File name     : c_union_define_intra.h
// Version       : 2.0
// Author        : xxx
// Created       : 2016-02-04
// Last Modified : 
// Description   :  The C union definition file for the module intra
// Function List : 
// History       : 
// 1 Date        : 
// Author        : xxx
// Modification  : Create file
//******************************************************************************

#ifndef __C_UNION_DEFINE_INTRA_H__
#define __C_UNION_DEFINE_INTRA_H__

/* Define the union U_INTRA_INTER_AVAIL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intra_inter_avail     : 1   ; /* [0]  */
        unsigned int    Reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INTRA_INTER_AVAIL;

/* Define the union U_INTRA_SMOOTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intra_smooth          : 1   ; /* [0]  */
        unsigned int    Reserved_1            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INTRA_SMOOTH;

/* Define the union U_INTRA_DC_SHIFT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intra_dc_4shift       : 4   ; /* [3..0]  */
        unsigned int    intra_dc_8shift       : 4   ; /* [7..4]  */
        unsigned int    intra_dc_16shift      : 4   ; /* [11..8]  */
        unsigned int    intra_dc_4mode_en     : 1   ; /* [12]  */
        unsigned int    intra_dc_8mode_en     : 1   ; /* [13]  */
        unsigned int    intra_dc_16mode_en    : 1   ; /* [14]  */
        unsigned int    intra_dc_shift_en     : 1   ; /* [15]  */
        unsigned int    Reserved_2            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INTRA_DC_SHIFT;

/* Define the union U_INTRA_BIT_WEIGHT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intra_bit_weight      : 4   ; /* [3..0]  */
        unsigned int    Reserved_3            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INTRA_BIT_WEIGHT;

/* Define the union U_INTRA_DC_MADI */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intra_dc_4madi        : 8   ; /* [7..0]  */
        unsigned int    intra_dc_8madi        : 8   ; /* [15..8]  */
        unsigned int    intra_dc_16madi       : 8   ; /* [23..16]  */
        unsigned int    Reserved_4            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INTRA_DC_MADI;

/* Define the union U_INTRA_RDO_COST_OFFSET_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intra_cu4_rdo_cost_offset  : 16  ; /* [15..0]  */
        unsigned int    intra_cu8_rdo_cost_offset  : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INTRA_RDO_COST_OFFSET_1;

/* Define the union U_INTRA_NO_DC_COST_OFFSET_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intra_cu16_non_dc_mode_offset  : 16  ; /* [15..0]  */
        unsigned int    intra_cu32_non_dc_mode_offset  : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INTRA_NO_DC_COST_OFFSET_0;

/* Define the union U_INTRA_NO_DC_COST_OFFSET_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intra_cu4_non_dc_mode_offset  : 16  ; /* [15..0]  */
        unsigned int    intra_cu8_non_dc_mode_offset  : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INTRA_NO_DC_COST_OFFSET_1;

/* Define the union U_INTRA_CHNL4_ANG_0EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intra_cu4_mode0_en    : 1   ; /* [0]  */
        unsigned int    intra_cu4_mode1_en    : 1   ; /* [1]  */
        unsigned int    intra_cu4_mode2_en    : 1   ; /* [2]  */
        unsigned int    intra_cu4_mode3_en    : 1   ; /* [3]  */
        unsigned int    intra_cu4_mode4_en    : 1   ; /* [4]  */
        unsigned int    intra_cu4_mode5_en    : 1   ; /* [5]  */
        unsigned int    intra_cu4_mode6_en    : 1   ; /* [6]  */
        unsigned int    intra_cu4_mode7_en    : 1   ; /* [7]  */
        unsigned int    intra_cu4_mode8_en    : 1   ; /* [8]  */
        unsigned int    intra_cu4_mode9_en    : 1   ; /* [9]  */
        unsigned int    intra_cu4_mode10_en   : 1   ; /* [10]  */
        unsigned int    intra_cu4_mode11_en   : 1   ; /* [11]  */
        unsigned int    intra_cu4_mode12_en   : 1   ; /* [12]  */
        unsigned int    intra_cu4_mode13_en   : 1   ; /* [13]  */
        unsigned int    intra_cu4_mode14_en   : 1   ; /* [14]  */
        unsigned int    intra_cu4_mode15_en   : 1   ; /* [15]  */
        unsigned int    intra_cu4_mode16_en   : 1   ; /* [16]  */
        unsigned int    intra_cu4_mode17_en   : 1   ; /* [17]  */
        unsigned int    intra_cu4_mode18_en   : 1   ; /* [18]  */
        unsigned int    intra_cu4_mode19_en   : 1   ; /* [19]  */
        unsigned int    intra_cu4_mode20_en   : 1   ; /* [20]  */
        unsigned int    intra_cu4_mode21_en   : 1   ; /* [21]  */
        unsigned int    intra_cu4_mode22_en   : 1   ; /* [22]  */
        unsigned int    intra_cu4_mode23_en   : 1   ; /* [23]  */
        unsigned int    intra_cu4_mode24_en   : 1   ; /* [24]  */
        unsigned int    intra_cu4_mode25_en   : 1   ; /* [25]  */
        unsigned int    intra_cu4_mode26_en   : 1   ; /* [26]  */
        unsigned int    intra_cu4_mode27_en   : 1   ; /* [27]  */
        unsigned int    intra_cu4_mode28_en   : 1   ; /* [28]  */
        unsigned int    intra_cu4_mode29_en   : 1   ; /* [29]  */
        unsigned int    intra_cu4_mode30_en   : 1   ; /* [30]  */
        unsigned int    intra_cu4_mode31_en   : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INTRA_CHNL4_ANG_0EN;

/* Define the union U_INTRA_CHNL4_ANG_1EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intra_cu4_mode32_en   : 1   ; /* [0]  */
        unsigned int    intra_cu4_mode33_en   : 1   ; /* [1]  */
        unsigned int    intra_cu4_mode34_en   : 1   ; /* [2]  */
        unsigned int    Reserved_5            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INTRA_CHNL4_ANG_1EN;

/* Define the union U_INTRA_CHNL8_ANG_0EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intra_cu8_mode0_en    : 1   ; /* [0]  */
        unsigned int    intra_cu8_mode1_en    : 1   ; /* [1]  */
        unsigned int    intra_cu8_mode2_en    : 1   ; /* [2]  */
        unsigned int    intra_cu8_mode3_en    : 1   ; /* [3]  */
        unsigned int    intra_cu8_mode4_en    : 1   ; /* [4]  */
        unsigned int    intra_cu8_mode5_en    : 1   ; /* [5]  */
        unsigned int    intra_cu8_mode6_en    : 1   ; /* [6]  */
        unsigned int    intra_cu8_mode7_en    : 1   ; /* [7]  */
        unsigned int    intra_cu8_mode8_en    : 1   ; /* [8]  */
        unsigned int    intra_cu8_mode9_en    : 1   ; /* [9]  */
        unsigned int    intra_cu8_mode10_en   : 1   ; /* [10]  */
        unsigned int    intra_cu8_mode11_en   : 1   ; /* [11]  */
        unsigned int    intra_cu8_mode12_en   : 1   ; /* [12]  */
        unsigned int    intra_cu8_mode13_en   : 1   ; /* [13]  */
        unsigned int    intra_cu8_mode14_en   : 1   ; /* [14]  */
        unsigned int    intra_cu8_mode15_en   : 1   ; /* [15]  */
        unsigned int    intra_cu8_mode16_en   : 1   ; /* [16]  */
        unsigned int    intra_cu8_mode17_en   : 1   ; /* [17]  */
        unsigned int    intra_cu8_mode18_en   : 1   ; /* [18]  */
        unsigned int    intra_cu8_mode19_en   : 1   ; /* [19]  */
        unsigned int    intra_cu8_mode20_en   : 1   ; /* [20]  */
        unsigned int    intra_cu8_mode21_en   : 1   ; /* [21]  */
        unsigned int    intra_cu8_mode22_en   : 1   ; /* [22]  */
        unsigned int    intra_cu8_mode23_en   : 1   ; /* [23]  */
        unsigned int    intra_cu8_mode24_en   : 1   ; /* [24]  */
        unsigned int    intra_cu8_mode25_en   : 1   ; /* [25]  */
        unsigned int    intra_cu8_mode26_en   : 1   ; /* [26]  */
        unsigned int    intra_cu8_mode27_en   : 1   ; /* [27]  */
        unsigned int    intra_cu8_mode28_en   : 1   ; /* [28]  */
        unsigned int    intra_cu8_mode29_en   : 1   ; /* [29]  */
        unsigned int    intra_cu8_mode30_en   : 1   ; /* [30]  */
        unsigned int    intra_cu8_mode31_en   : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INTRA_CHNL8_ANG_0EN;

/* Define the union U_INTRA_CHNL8_ANG_1EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intra_cu8_mode32_en   : 1   ; /* [0]  */
        unsigned int    intra_cu8_mode33_en   : 1   ; /* [1]  */
        unsigned int    intra_cu8_mode34_en   : 1   ; /* [2]  */
        unsigned int    Reserved_6            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INTRA_CHNL8_ANG_1EN;

/* Define the union U_INTRA_CHNL16_ANG_0EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intra_cu16_mode0_en   : 1   ; /* [0]  */
        unsigned int    intra_cu16_mode1_en   : 1   ; /* [1]  */
        unsigned int    intra_cu16_mode2_en   : 1   ; /* [2]  */
        unsigned int    intra_cu16_mode3_en   : 1   ; /* [3]  */
        unsigned int    intra_cu16_mode4_en   : 1   ; /* [4]  */
        unsigned int    intra_cu16_mode5_en   : 1   ; /* [5]  */
        unsigned int    intra_cu16_mode6_en   : 1   ; /* [6]  */
        unsigned int    intra_cu16_mode7_en   : 1   ; /* [7]  */
        unsigned int    intra_cu16_mode8_en   : 1   ; /* [8]  */
        unsigned int    intra_cu16_mode9_en   : 1   ; /* [9]  */
        unsigned int    intra_cu16_mode10_en  : 1   ; /* [10]  */
        unsigned int    intra_cu16_mode11_en  : 1   ; /* [11]  */
        unsigned int    intra_cu16_mode12_en  : 1   ; /* [12]  */
        unsigned int    intra_cu16_mode13_en  : 1   ; /* [13]  */
        unsigned int    intra_cu16_mode14_en  : 1   ; /* [14]  */
        unsigned int    intra_cu16_mode15_en  : 1   ; /* [15]  */
        unsigned int    intra_cu16_mode16_en  : 1   ; /* [16]  */
        unsigned int    intra_cu16_mode17_en  : 1   ; /* [17]  */
        unsigned int    intra_cu16_mode18_en  : 1   ; /* [18]  */
        unsigned int    intra_cu16_mode19_en  : 1   ; /* [19]  */
        unsigned int    intra_cu16_mode20_en  : 1   ; /* [20]  */
        unsigned int    intra_cu16_mode21_en  : 1   ; /* [21]  */
        unsigned int    intra_cu16_mode22_en  : 1   ; /* [22]  */
        unsigned int    intra_cu16_mode23_en  : 1   ; /* [23]  */
        unsigned int    intra_cu16_mode24_en  : 1   ; /* [24]  */
        unsigned int    intra_cu16_mode25_en  : 1   ; /* [25]  */
        unsigned int    intra_cu16_mode26_en  : 1   ; /* [26]  */
        unsigned int    intra_cu16_mode27_en  : 1   ; /* [27]  */
        unsigned int    intra_cu16_mode28_en  : 1   ; /* [28]  */
        unsigned int    intra_cu16_mode29_en  : 1   ; /* [29]  */
        unsigned int    intra_cu16_mode30_en  : 1   ; /* [30]  */
        unsigned int    intra_cu16_mode31_en  : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INTRA_CHNL16_ANG_0EN;

/* Define the union U_INTRA_CHNL16_ANG_1EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intra_cu16_mode32_en  : 1   ; /* [0]  */
        unsigned int    intra_cu16_mode33_en  : 1   ; /* [1]  */
        unsigned int    intra_cu16_mode34_en  : 1   ; /* [2]  */
        unsigned int    Reserved_7            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INTRA_CHNL16_ANG_1EN;

/* Define the union U_INTRA_CHNL32_ANG_0EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intra_cu32_mode0_en   : 1   ; /* [0]  */
        unsigned int    intra_cu32_mode1_en   : 1   ; /* [1]  */
        unsigned int    intra_cu32_mode2_en   : 1   ; /* [2]  */
        unsigned int    intra_cu32_mode3_en   : 1   ; /* [3]  */
        unsigned int    intra_cu32_mode4_en   : 1   ; /* [4]  */
        unsigned int    intra_cu32_mode5_en   : 1   ; /* [5]  */
        unsigned int    intra_cu32_mode6_en   : 1   ; /* [6]  */
        unsigned int    intra_cu32_mode7_en   : 1   ; /* [7]  */
        unsigned int    intra_cu32_mode8_en   : 1   ; /* [8]  */
        unsigned int    intra_cu32_mode9_en   : 1   ; /* [9]  */
        unsigned int    intra_cu32_mode10_en  : 1   ; /* [10]  */
        unsigned int    intra_cu32_mode11_en  : 1   ; /* [11]  */
        unsigned int    intra_cu32_mode12_en  : 1   ; /* [12]  */
        unsigned int    intra_cu32_mode13_en  : 1   ; /* [13]  */
        unsigned int    intra_cu32_mode14_en  : 1   ; /* [14]  */
        unsigned int    intra_cu32_mode15_en  : 1   ; /* [15]  */
        unsigned int    intra_cu32_mode16_en  : 1   ; /* [16]  */
        unsigned int    intra_cu32_mode17_en  : 1   ; /* [17]  */
        unsigned int    intra_cu32_mode18_en  : 1   ; /* [18]  */
        unsigned int    intra_cu32_mode19_en  : 1   ; /* [19]  */
        unsigned int    intra_cu32_mode20_en  : 1   ; /* [20]  */
        unsigned int    intra_cu32_mode21_en  : 1   ; /* [21]  */
        unsigned int    intra_cu32_mode22_en  : 1   ; /* [22]  */
        unsigned int    intra_cu32_mode23_en  : 1   ; /* [23]  */
        unsigned int    intra_cu32_mode24_en  : 1   ; /* [24]  */
        unsigned int    intra_cu32_mode25_en  : 1   ; /* [25]  */
        unsigned int    intra_cu32_mode26_en  : 1   ; /* [26]  */
        unsigned int    intra_cu32_mode27_en  : 1   ; /* [27]  */
        unsigned int    intra_cu32_mode28_en  : 1   ; /* [28]  */
        unsigned int    intra_cu32_mode29_en  : 1   ; /* [29]  */
        unsigned int    intra_cu32_mode30_en  : 1   ; /* [30]  */
        unsigned int    intra_cu32_mode31_en  : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INTRA_CHNL32_ANG_0EN;

/* Define the union U_INTRA_CHNL32_ANG_1EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intra_cu32_mode32_en  : 1   ; /* [0]  */
        unsigned int    intra_cu32_mode33_en  : 1   ; /* [1]  */
        unsigned int    intra_cu32_mode34_en  : 1   ; /* [2]  */
        unsigned int    Reserved_8            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INTRA_CHNL32_ANG_1EN;

/* Define the union U_INTRA_DFX_FRM_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dfx_frm_cnt           : 16  ; /* [15..0]  */
        unsigned int    Reserved_9            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INTRA_DFX_FRM_CNT;

/* Define the union U_INTRA_DFX_LCU_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dfx_lcu_cnt           : 16  ; /* [15..0]  */
        unsigned int    Reserved_10           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INTRA_DFX_LCU_CNT;

/* Define the union U_INTRA_DFX_LCU_END_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dfx_lcu_end_cnt       : 16  ; /* [15..0]  */
        unsigned int    Reserved_11           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INTRA_DFX_LCU_END_CNT;

/* Define the union U_INTRA_DFX_LCU_CYCLE_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dfx_lcu_cycle_cnt     : 16  ; /* [15..0]  */
        unsigned int    Reserved_12           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INTRA_DFX_LCU_CYCLE_CNT;

/* Define the union U_INTRA_DFX_NBI_RD_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dfx_nbi_rd_cnt        : 16  ; /* [15..0]  */
        unsigned int    Reserved_13           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INTRA_DFX_NBI_RD_CNT;

/* Define the union U_INTRA_DFX_NBI_WR_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dfx_nbi_wr_cnt        : 16  ; /* [15..0]  */
        unsigned int    Reserved_14           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INTRA_DFX_NBI_WR_CNT;

/* Define the union U_INTRA_DFX_CU8_DONE_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dfx_cu8_done_cnt      : 16  ; /* [15..0]  */
        unsigned int    Reserved_16           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INTRA_DFX_CU8_DONE_CNT;

/* Define the union U_INTRA_DFX_CU16_DONE_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dfx_cu16_done_cnt     : 16  ; /* [15..0]  */
        unsigned int    Reserved_17           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INTRA_DFX_CU16_DONE_CNT;

/* Define the union U_INTRA_DFX_CU32_DONE_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dfx_cu32_done_cnt     : 16  ; /* [15..0]  */
        unsigned int    Reserved_18           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INTRA_DFX_CU32_DONE_CNT;

/* Define the union U_INTRA_DFX_CU64_DONE_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dfx_cu64_done_cnt     : 16  ; /* [15..0]  */
        unsigned int    Reserved_19           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INTRA_DFX_CU64_DONE_CNT;

/* Define the union U_INTRA_DFX_CU8_RDO_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dfx_cu8_rdo_cnt       : 16  ; /* [15..0]  */
        unsigned int    Reserved_20           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INTRA_DFX_CU8_RDO_CNT;

/* Define the union U_INTRA_DFX_CU16_RDO_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dfx_cu16_rdo_cnt      : 16  ; /* [15..0]  */
        unsigned int    Reserved_21           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INTRA_DFX_CU16_RDO_CNT;

/* Define the union U_INTRA_DFX_CU32_RDO_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dfx_cu32_rdo_cnt      : 16  ; /* [15..0]  */
        unsigned int    Reserved_22           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INTRA_DFX_CU32_RDO_CNT;

/* Define the union U_INTRA_CU_EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intra_cu_en           : 4   ; /* [3..0]  */
        unsigned int    Reserved_23           : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INTRA_CU_EN;

/* Define the union U_INTRA_DFX_CU8_FSM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dfx_cu8_fsm           : 5   ; /* [4..0]  */
        unsigned int    Reserved_24           : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INTRA_DFX_CU8_FSM;

/* Define the union U_INTRA_DFX_CU16_FSM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dfx_cu16_fsm          : 5   ; /* [4..0]  */
        unsigned int    Reserved_25           : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INTRA_DFX_CU16_FSM;

/* Define the union U_INTRA_DFX_CU32_FSM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dfx_cu32_fsm          : 5   ; /* [4..0]  */
        unsigned int    Reserved_26           : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INTRA_DFX_CU32_FSM;

/* Define the union U_INTRA_DFX_PCM_PWR__TYPE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dfx_pcm               : 1   ; /* [0]  */
        unsigned int    Reserved_30           : 3   ; /* [3..1]  */
        unsigned int    dfx_lpw               : 1   ; /* [4]  */
        unsigned int    Reserved_29           : 3   ; /* [7..5]  */
        unsigned int    dfx_frm_type          : 2   ; /* [9..8]  */
        unsigned int    Reserved_28           : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INTRA_DFX_PCM_PWR__TYPE;

/* Define the union U_INTRA_DFX_HEAD_BIT_WEIGHT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dfx_head_bit_weight   : 4   ; /* [3..0]  */
        unsigned int    Reserved_31           : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INTRA_DFX_HEAD_BIT_WEIGHT;

/* Define the union U_INTRA_DFX_RDO_WEIGHT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dfx_rdo_weight        : 16  ; /* [15..0]  */
        unsigned int    Reserved_32           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INTRA_DFX_RDO_WEIGHT;

/* Define the union U_INTRA_DFX_BLK_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dfx_cu4_idx           : 8   ; /* [7..0]  */
        unsigned int    dfx_cu8_idx           : 6   ; /* [13..8]  */
        unsigned int    Reserved_35           : 2   ; /* [15..14]  */
        unsigned int    dfx_cu16_idx          : 4   ; /* [19..16]  */
        unsigned int    Reserved_34           : 4   ; /* [23..20]  */
        unsigned int    dfx_cu32_idx          : 2   ; /* [25..24]  */
        unsigned int    Reserved_33           : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_INTRA_DFX_BLK_CNT;

//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile U_INTRA_INTER_AVAIL    INTRA_INTER_AVAIL;
    volatile U_INTRA_SMOOTH         INTRA_SMOOTH;
    volatile U_INTRA_DC_SHIFT       INTRA_DC_SHIFT;
    volatile U_INTRA_BIT_WEIGHT     INTRA_BIT_WEIGHT;
    volatile U_INTRA_DC_MADI        INTRA_DC_MADI;
    volatile U_INTRA_RDO_COST_OFFSET_1   INTRA_RDO_COST_OFFSET_1;
    volatile U_INTRA_NO_DC_COST_OFFSET_0   INTRA_NO_DC_COST_OFFSET_0;
    volatile U_INTRA_NO_DC_COST_OFFSET_1   INTRA_NO_DC_COST_OFFSET_1;
    volatile U_INTRA_CHNL4_ANG_0EN   INTRA_CHNL4_ANG_0EN;
    volatile U_INTRA_CHNL4_ANG_1EN   INTRA_CHNL4_ANG_1EN;
    volatile U_INTRA_CHNL8_ANG_0EN   INTRA_CHNL8_ANG_0EN;
    volatile U_INTRA_CHNL8_ANG_1EN   INTRA_CHNL8_ANG_1EN;
    volatile U_INTRA_CHNL16_ANG_0EN   INTRA_CHNL16_ANG_0EN;
    volatile U_INTRA_CHNL16_ANG_1EN   INTRA_CHNL16_ANG_1EN;
    volatile U_INTRA_CHNL32_ANG_0EN   INTRA_CHNL32_ANG_0EN;
    volatile U_INTRA_CHNL32_ANG_1EN   INTRA_CHNL32_ANG_1EN;
    volatile U_INTRA_DFX_FRM_CNT    INTRA_DFX_FRM_CNT;
    volatile U_INTRA_DFX_LCU_CNT    INTRA_DFX_LCU_CNT;
    volatile U_INTRA_DFX_LCU_END_CNT   INTRA_DFX_LCU_END_CNT;
    volatile U_INTRA_DFX_LCU_CYCLE_CNT   INTRA_DFX_LCU_CYCLE_CNT;
    volatile U_INTRA_DFX_NBI_RD_CNT   INTRA_DFX_NBI_RD_CNT;
    volatile U_INTRA_DFX_NBI_WR_CNT   INTRA_DFX_NBI_WR_CNT;
    volatile unsigned int           RESERVED2[2];
    volatile U_INTRA_DFX_CU8_DONE_CNT   INTRA_DFX_CU8_DONE_CNT;
    volatile U_INTRA_DFX_CU16_DONE_CNT   INTRA_DFX_CU16_DONE_CNT;
    volatile U_INTRA_DFX_CU32_DONE_CNT   INTRA_DFX_CU32_DONE_CNT;
    volatile U_INTRA_DFX_CU64_DONE_CNT   INTRA_DFX_CU64_DONE_CNT;
    volatile U_INTRA_DFX_CU8_RDO_CNT   INTRA_DFX_CU8_RDO_CNT;
    volatile U_INTRA_DFX_CU16_RDO_CNT   INTRA_DFX_CU16_RDO_CNT;
    volatile U_INTRA_DFX_CU32_RDO_CNT   INTRA_DFX_CU32_RDO_CNT;
    volatile U_INTRA_CU_EN          INTRA_CU_EN;
    volatile U_INTRA_DFX_CU8_FSM    INTRA_DFX_CU8_FSM;
    volatile U_INTRA_DFX_CU16_FSM   INTRA_DFX_CU16_FSM;
    volatile U_INTRA_DFX_CU32_FSM   INTRA_DFX_CU32_FSM;
    volatile unsigned int           RESERVED4;
    volatile U_INTRA_DFX_PCM_PWR__TYPE   INTRA_DFX_PCM_PWR__TYPE;
    volatile U_INTRA_DFX_HEAD_BIT_WEIGHT   INTRA_DFX_HEAD_BIT_WEIGHT;
    volatile U_INTRA_DFX_RDO_WEIGHT   INTRA_DFX_RDO_WEIGHT;
    volatile U_INTRA_DFX_BLK_CNT    INTRA_DFX_BLK_CNT;
    volatile unsigned int           RESERVED5[88];


} S_intra_REGS_TYPE;

/* Declare the struct pointor of the module intra */
extern volatile S_intra_REGS_TYPE *gopintraAllReg;

/* Declare the functions that set the member value */
int iSetINTRA_INTER_AVAILintra_inter_avail(unsigned int uintra_inter_avail);
int iSetINTRA_SMOOTHintra_smooth(unsigned int uintra_smooth);
int iSetINTRA_DC_SHIFTintra_dc_4shift(unsigned int uintra_dc_4shift);
int iSetINTRA_DC_SHIFTintra_dc_8shift(unsigned int uintra_dc_8shift);
int iSetINTRA_DC_SHIFTintra_dc_16shift(unsigned int uintra_dc_16shift);
int iSetINTRA_DC_SHIFTintra_dc_4mode_en(unsigned int uintra_dc_4mode_en);
int iSetINTRA_DC_SHIFTintra_dc_8mode_en(unsigned int uintra_dc_8mode_en);
int iSetINTRA_DC_SHIFTintra_dc_16mode_en(unsigned int uintra_dc_16mode_en);
int iSetINTRA_DC_SHIFTintra_dc_shift_en(unsigned int uintra_dc_shift_en);
int iSetINTRA_BIT_WEIGHTintra_bit_weight(unsigned int uintra_bit_weight);
int iSetINTRA_DC_MADIintra_dc_4madi(unsigned int uintra_dc_4madi);
int iSetINTRA_DC_MADIintra_dc_8madi(unsigned int uintra_dc_8madi);
int iSetINTRA_DC_MADIintra_dc_16madi(unsigned int uintra_dc_16madi);
int iSetINTRA_RDO_COST_OFFSET_1intra_cu4_rdo_cost_offset(unsigned int uintra_cu4_rdo_cost_offset);
int iSetINTRA_RDO_COST_OFFSET_1intra_cu8_rdo_cost_offset(unsigned int uintra_cu8_rdo_cost_offset);
int iSetINTRA_NO_DC_COST_OFFSET_0intra_cu16_non_dc_mode_offset(unsigned int uintra_cu16_non_dc_mode_offset);
int iSetINTRA_NO_DC_COST_OFFSET_0intra_cu32_non_dc_mode_offset(unsigned int uintra_cu32_non_dc_mode_offset);
int iSetINTRA_NO_DC_COST_OFFSET_1intra_cu4_non_dc_mode_offset(unsigned int uintra_cu4_non_dc_mode_offset);
int iSetINTRA_NO_DC_COST_OFFSET_1intra_cu8_non_dc_mode_offset(unsigned int uintra_cu8_non_dc_mode_offset);
int iSetINTRA_CHNL4_ANG_0ENintra_cu4_mode0_en(unsigned int uintra_cu4_mode0_en);
int iSetINTRA_CHNL4_ANG_0ENintra_cu4_mode1_en(unsigned int uintra_cu4_mode1_en);
int iSetINTRA_CHNL4_ANG_0ENintra_cu4_mode2_en(unsigned int uintra_cu4_mode2_en);
int iSetINTRA_CHNL4_ANG_0ENintra_cu4_mode3_en(unsigned int uintra_cu4_mode3_en);
int iSetINTRA_CHNL4_ANG_0ENintra_cu4_mode4_en(unsigned int uintra_cu4_mode4_en);
int iSetINTRA_CHNL4_ANG_0ENintra_cu4_mode5_en(unsigned int uintra_cu4_mode5_en);
int iSetINTRA_CHNL4_ANG_0ENintra_cu4_mode6_en(unsigned int uintra_cu4_mode6_en);
int iSetINTRA_CHNL4_ANG_0ENintra_cu4_mode7_en(unsigned int uintra_cu4_mode7_en);
int iSetINTRA_CHNL4_ANG_0ENintra_cu4_mode8_en(unsigned int uintra_cu4_mode8_en);
int iSetINTRA_CHNL4_ANG_0ENintra_cu4_mode9_en(unsigned int uintra_cu4_mode9_en);
int iSetINTRA_CHNL4_ANG_0ENintra_cu4_mode10_en(unsigned int uintra_cu4_mode10_en);
int iSetINTRA_CHNL4_ANG_0ENintra_cu4_mode11_en(unsigned int uintra_cu4_mode11_en);
int iSetINTRA_CHNL4_ANG_0ENintra_cu4_mode12_en(unsigned int uintra_cu4_mode12_en);
int iSetINTRA_CHNL4_ANG_0ENintra_cu4_mode13_en(unsigned int uintra_cu4_mode13_en);
int iSetINTRA_CHNL4_ANG_0ENintra_cu4_mode14_en(unsigned int uintra_cu4_mode14_en);
int iSetINTRA_CHNL4_ANG_0ENintra_cu4_mode15_en(unsigned int uintra_cu4_mode15_en);
int iSetINTRA_CHNL4_ANG_0ENintra_cu4_mode16_en(unsigned int uintra_cu4_mode16_en);
int iSetINTRA_CHNL4_ANG_0ENintra_cu4_mode17_en(unsigned int uintra_cu4_mode17_en);
int iSetINTRA_CHNL4_ANG_0ENintra_cu4_mode18_en(unsigned int uintra_cu4_mode18_en);
int iSetINTRA_CHNL4_ANG_0ENintra_cu4_mode19_en(unsigned int uintra_cu4_mode19_en);
int iSetINTRA_CHNL4_ANG_0ENintra_cu4_mode20_en(unsigned int uintra_cu4_mode20_en);
int iSetINTRA_CHNL4_ANG_0ENintra_cu4_mode21_en(unsigned int uintra_cu4_mode21_en);
int iSetINTRA_CHNL4_ANG_0ENintra_cu4_mode22_en(unsigned int uintra_cu4_mode22_en);
int iSetINTRA_CHNL4_ANG_0ENintra_cu4_mode23_en(unsigned int uintra_cu4_mode23_en);
int iSetINTRA_CHNL4_ANG_0ENintra_cu4_mode24_en(unsigned int uintra_cu4_mode24_en);
int iSetINTRA_CHNL4_ANG_0ENintra_cu4_mode25_en(unsigned int uintra_cu4_mode25_en);
int iSetINTRA_CHNL4_ANG_0ENintra_cu4_mode26_en(unsigned int uintra_cu4_mode26_en);
int iSetINTRA_CHNL4_ANG_0ENintra_cu4_mode27_en(unsigned int uintra_cu4_mode27_en);
int iSetINTRA_CHNL4_ANG_0ENintra_cu4_mode28_en(unsigned int uintra_cu4_mode28_en);
int iSetINTRA_CHNL4_ANG_0ENintra_cu4_mode29_en(unsigned int uintra_cu4_mode29_en);
int iSetINTRA_CHNL4_ANG_0ENintra_cu4_mode30_en(unsigned int uintra_cu4_mode30_en);
int iSetINTRA_CHNL4_ANG_0ENintra_cu4_mode31_en(unsigned int uintra_cu4_mode31_en);
int iSetINTRA_CHNL4_ANG_1ENintra_cu4_mode32_en(unsigned int uintra_cu4_mode32_en);
int iSetINTRA_CHNL4_ANG_1ENintra_cu4_mode33_en(unsigned int uintra_cu4_mode33_en);
int iSetINTRA_CHNL4_ANG_1ENintra_cu4_mode34_en(unsigned int uintra_cu4_mode34_en);
int iSetINTRA_CHNL8_ANG_0ENintra_cu8_mode0_en(unsigned int uintra_cu8_mode0_en);
int iSetINTRA_CHNL8_ANG_0ENintra_cu8_mode1_en(unsigned int uintra_cu8_mode1_en);
int iSetINTRA_CHNL8_ANG_0ENintra_cu8_mode2_en(unsigned int uintra_cu8_mode2_en);
int iSetINTRA_CHNL8_ANG_0ENintra_cu8_mode3_en(unsigned int uintra_cu8_mode3_en);
int iSetINTRA_CHNL8_ANG_0ENintra_cu8_mode4_en(unsigned int uintra_cu8_mode4_en);
int iSetINTRA_CHNL8_ANG_0ENintra_cu8_mode5_en(unsigned int uintra_cu8_mode5_en);
int iSetINTRA_CHNL8_ANG_0ENintra_cu8_mode6_en(unsigned int uintra_cu8_mode6_en);
int iSetINTRA_CHNL8_ANG_0ENintra_cu8_mode7_en(unsigned int uintra_cu8_mode7_en);
int iSetINTRA_CHNL8_ANG_0ENintra_cu8_mode8_en(unsigned int uintra_cu8_mode8_en);
int iSetINTRA_CHNL8_ANG_0ENintra_cu8_mode9_en(unsigned int uintra_cu8_mode9_en);
int iSetINTRA_CHNL8_ANG_0ENintra_cu8_mode10_en(unsigned int uintra_cu8_mode10_en);
int iSetINTRA_CHNL8_ANG_0ENintra_cu8_mode11_en(unsigned int uintra_cu8_mode11_en);
int iSetINTRA_CHNL8_ANG_0ENintra_cu8_mode12_en(unsigned int uintra_cu8_mode12_en);
int iSetINTRA_CHNL8_ANG_0ENintra_cu8_mode13_en(unsigned int uintra_cu8_mode13_en);
int iSetINTRA_CHNL8_ANG_0ENintra_cu8_mode14_en(unsigned int uintra_cu8_mode14_en);
int iSetINTRA_CHNL8_ANG_0ENintra_cu8_mode15_en(unsigned int uintra_cu8_mode15_en);
int iSetINTRA_CHNL8_ANG_0ENintra_cu8_mode16_en(unsigned int uintra_cu8_mode16_en);
int iSetINTRA_CHNL8_ANG_0ENintra_cu8_mode17_en(unsigned int uintra_cu8_mode17_en);
int iSetINTRA_CHNL8_ANG_0ENintra_cu8_mode18_en(unsigned int uintra_cu8_mode18_en);
int iSetINTRA_CHNL8_ANG_0ENintra_cu8_mode19_en(unsigned int uintra_cu8_mode19_en);
int iSetINTRA_CHNL8_ANG_0ENintra_cu8_mode20_en(unsigned int uintra_cu8_mode20_en);
int iSetINTRA_CHNL8_ANG_0ENintra_cu8_mode21_en(unsigned int uintra_cu8_mode21_en);
int iSetINTRA_CHNL8_ANG_0ENintra_cu8_mode22_en(unsigned int uintra_cu8_mode22_en);
int iSetINTRA_CHNL8_ANG_0ENintra_cu8_mode23_en(unsigned int uintra_cu8_mode23_en);
int iSetINTRA_CHNL8_ANG_0ENintra_cu8_mode24_en(unsigned int uintra_cu8_mode24_en);
int iSetINTRA_CHNL8_ANG_0ENintra_cu8_mode25_en(unsigned int uintra_cu8_mode25_en);
int iSetINTRA_CHNL8_ANG_0ENintra_cu8_mode26_en(unsigned int uintra_cu8_mode26_en);
int iSetINTRA_CHNL8_ANG_0ENintra_cu8_mode27_en(unsigned int uintra_cu8_mode27_en);
int iSetINTRA_CHNL8_ANG_0ENintra_cu8_mode28_en(unsigned int uintra_cu8_mode28_en);
int iSetINTRA_CHNL8_ANG_0ENintra_cu8_mode29_en(unsigned int uintra_cu8_mode29_en);
int iSetINTRA_CHNL8_ANG_0ENintra_cu8_mode30_en(unsigned int uintra_cu8_mode30_en);
int iSetINTRA_CHNL8_ANG_0ENintra_cu8_mode31_en(unsigned int uintra_cu8_mode31_en);
int iSetINTRA_CHNL8_ANG_1ENintra_cu8_mode32_en(unsigned int uintra_cu8_mode32_en);
int iSetINTRA_CHNL8_ANG_1ENintra_cu8_mode33_en(unsigned int uintra_cu8_mode33_en);
int iSetINTRA_CHNL8_ANG_1ENintra_cu8_mode34_en(unsigned int uintra_cu8_mode34_en);
int iSetINTRA_CHNL16_ANG_0ENintra_cu16_mode0_en(unsigned int uintra_cu16_mode0_en);
int iSetINTRA_CHNL16_ANG_0ENintra_cu16_mode1_en(unsigned int uintra_cu16_mode1_en);
int iSetINTRA_CHNL16_ANG_0ENintra_cu16_mode2_en(unsigned int uintra_cu16_mode2_en);
int iSetINTRA_CHNL16_ANG_0ENintra_cu16_mode3_en(unsigned int uintra_cu16_mode3_en);
int iSetINTRA_CHNL16_ANG_0ENintra_cu16_mode4_en(unsigned int uintra_cu16_mode4_en);
int iSetINTRA_CHNL16_ANG_0ENintra_cu16_mode5_en(unsigned int uintra_cu16_mode5_en);
int iSetINTRA_CHNL16_ANG_0ENintra_cu16_mode6_en(unsigned int uintra_cu16_mode6_en);
int iSetINTRA_CHNL16_ANG_0ENintra_cu16_mode7_en(unsigned int uintra_cu16_mode7_en);
int iSetINTRA_CHNL16_ANG_0ENintra_cu16_mode8_en(unsigned int uintra_cu16_mode8_en);
int iSetINTRA_CHNL16_ANG_0ENintra_cu16_mode9_en(unsigned int uintra_cu16_mode9_en);
int iSetINTRA_CHNL16_ANG_0ENintra_cu16_mode10_en(unsigned int uintra_cu16_mode10_en);
int iSetINTRA_CHNL16_ANG_0ENintra_cu16_mode11_en(unsigned int uintra_cu16_mode11_en);
int iSetINTRA_CHNL16_ANG_0ENintra_cu16_mode12_en(unsigned int uintra_cu16_mode12_en);
int iSetINTRA_CHNL16_ANG_0ENintra_cu16_mode13_en(unsigned int uintra_cu16_mode13_en);
int iSetINTRA_CHNL16_ANG_0ENintra_cu16_mode14_en(unsigned int uintra_cu16_mode14_en);
int iSetINTRA_CHNL16_ANG_0ENintra_cu16_mode15_en(unsigned int uintra_cu16_mode15_en);
int iSetINTRA_CHNL16_ANG_0ENintra_cu16_mode16_en(unsigned int uintra_cu16_mode16_en);
int iSetINTRA_CHNL16_ANG_0ENintra_cu16_mode17_en(unsigned int uintra_cu16_mode17_en);
int iSetINTRA_CHNL16_ANG_0ENintra_cu16_mode18_en(unsigned int uintra_cu16_mode18_en);
int iSetINTRA_CHNL16_ANG_0ENintra_cu16_mode19_en(unsigned int uintra_cu16_mode19_en);
int iSetINTRA_CHNL16_ANG_0ENintra_cu16_mode20_en(unsigned int uintra_cu16_mode20_en);
int iSetINTRA_CHNL16_ANG_0ENintra_cu16_mode21_en(unsigned int uintra_cu16_mode21_en);
int iSetINTRA_CHNL16_ANG_0ENintra_cu16_mode22_en(unsigned int uintra_cu16_mode22_en);
int iSetINTRA_CHNL16_ANG_0ENintra_cu16_mode23_en(unsigned int uintra_cu16_mode23_en);
int iSetINTRA_CHNL16_ANG_0ENintra_cu16_mode24_en(unsigned int uintra_cu16_mode24_en);
int iSetINTRA_CHNL16_ANG_0ENintra_cu16_mode25_en(unsigned int uintra_cu16_mode25_en);
int iSetINTRA_CHNL16_ANG_0ENintra_cu16_mode26_en(unsigned int uintra_cu16_mode26_en);
int iSetINTRA_CHNL16_ANG_0ENintra_cu16_mode27_en(unsigned int uintra_cu16_mode27_en);
int iSetINTRA_CHNL16_ANG_0ENintra_cu16_mode28_en(unsigned int uintra_cu16_mode28_en);
int iSetINTRA_CHNL16_ANG_0ENintra_cu16_mode29_en(unsigned int uintra_cu16_mode29_en);
int iSetINTRA_CHNL16_ANG_0ENintra_cu16_mode30_en(unsigned int uintra_cu16_mode30_en);
int iSetINTRA_CHNL16_ANG_0ENintra_cu16_mode31_en(unsigned int uintra_cu16_mode31_en);
int iSetINTRA_CHNL16_ANG_1ENintra_cu16_mode32_en(unsigned int uintra_cu16_mode32_en);
int iSetINTRA_CHNL16_ANG_1ENintra_cu16_mode33_en(unsigned int uintra_cu16_mode33_en);
int iSetINTRA_CHNL16_ANG_1ENintra_cu16_mode34_en(unsigned int uintra_cu16_mode34_en);
int iSetINTRA_CHNL32_ANG_0ENintra_cu32_mode0_en(unsigned int uintra_cu32_mode0_en);
int iSetINTRA_CHNL32_ANG_0ENintra_cu32_mode1_en(unsigned int uintra_cu32_mode1_en);
int iSetINTRA_CHNL32_ANG_0ENintra_cu32_mode2_en(unsigned int uintra_cu32_mode2_en);
int iSetINTRA_CHNL32_ANG_0ENintra_cu32_mode3_en(unsigned int uintra_cu32_mode3_en);
int iSetINTRA_CHNL32_ANG_0ENintra_cu32_mode4_en(unsigned int uintra_cu32_mode4_en);
int iSetINTRA_CHNL32_ANG_0ENintra_cu32_mode5_en(unsigned int uintra_cu32_mode5_en);
int iSetINTRA_CHNL32_ANG_0ENintra_cu32_mode6_en(unsigned int uintra_cu32_mode6_en);
int iSetINTRA_CHNL32_ANG_0ENintra_cu32_mode7_en(unsigned int uintra_cu32_mode7_en);
int iSetINTRA_CHNL32_ANG_0ENintra_cu32_mode8_en(unsigned int uintra_cu32_mode8_en);
int iSetINTRA_CHNL32_ANG_0ENintra_cu32_mode9_en(unsigned int uintra_cu32_mode9_en);
int iSetINTRA_CHNL32_ANG_0ENintra_cu32_mode10_en(unsigned int uintra_cu32_mode10_en);
int iSetINTRA_CHNL32_ANG_0ENintra_cu32_mode11_en(unsigned int uintra_cu32_mode11_en);
int iSetINTRA_CHNL32_ANG_0ENintra_cu32_mode12_en(unsigned int uintra_cu32_mode12_en);
int iSetINTRA_CHNL32_ANG_0ENintra_cu32_mode13_en(unsigned int uintra_cu32_mode13_en);
int iSetINTRA_CHNL32_ANG_0ENintra_cu32_mode14_en(unsigned int uintra_cu32_mode14_en);
int iSetINTRA_CHNL32_ANG_0ENintra_cu32_mode15_en(unsigned int uintra_cu32_mode15_en);
int iSetINTRA_CHNL32_ANG_0ENintra_cu32_mode16_en(unsigned int uintra_cu32_mode16_en);
int iSetINTRA_CHNL32_ANG_0ENintra_cu32_mode17_en(unsigned int uintra_cu32_mode17_en);
int iSetINTRA_CHNL32_ANG_0ENintra_cu32_mode18_en(unsigned int uintra_cu32_mode18_en);
int iSetINTRA_CHNL32_ANG_0ENintra_cu32_mode19_en(unsigned int uintra_cu32_mode19_en);
int iSetINTRA_CHNL32_ANG_0ENintra_cu32_mode20_en(unsigned int uintra_cu32_mode20_en);
int iSetINTRA_CHNL32_ANG_0ENintra_cu32_mode21_en(unsigned int uintra_cu32_mode21_en);
int iSetINTRA_CHNL32_ANG_0ENintra_cu32_mode22_en(unsigned int uintra_cu32_mode22_en);
int iSetINTRA_CHNL32_ANG_0ENintra_cu32_mode23_en(unsigned int uintra_cu32_mode23_en);
int iSetINTRA_CHNL32_ANG_0ENintra_cu32_mode24_en(unsigned int uintra_cu32_mode24_en);
int iSetINTRA_CHNL32_ANG_0ENintra_cu32_mode25_en(unsigned int uintra_cu32_mode25_en);
int iSetINTRA_CHNL32_ANG_0ENintra_cu32_mode26_en(unsigned int uintra_cu32_mode26_en);
int iSetINTRA_CHNL32_ANG_0ENintra_cu32_mode27_en(unsigned int uintra_cu32_mode27_en);
int iSetINTRA_CHNL32_ANG_0ENintra_cu32_mode28_en(unsigned int uintra_cu32_mode28_en);
int iSetINTRA_CHNL32_ANG_0ENintra_cu32_mode29_en(unsigned int uintra_cu32_mode29_en);
int iSetINTRA_CHNL32_ANG_0ENintra_cu32_mode30_en(unsigned int uintra_cu32_mode30_en);
int iSetINTRA_CHNL32_ANG_0ENintra_cu32_mode31_en(unsigned int uintra_cu32_mode31_en);
int iSetINTRA_CHNL32_ANG_1ENintra_cu32_mode32_en(unsigned int uintra_cu32_mode32_en);
int iSetINTRA_CHNL32_ANG_1ENintra_cu32_mode33_en(unsigned int uintra_cu32_mode33_en);
int iSetINTRA_CHNL32_ANG_1ENintra_cu32_mode34_en(unsigned int uintra_cu32_mode34_en);



#endif /* __C_UNION_DEFINE_INTRA_H__ */
