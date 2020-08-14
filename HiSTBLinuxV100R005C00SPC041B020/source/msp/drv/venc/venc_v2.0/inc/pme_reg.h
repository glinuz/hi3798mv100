//******************************************************************************
//  Copyright (C), 2007-2016, Hisilicon Technologies Co., Ltd.
//
//******************************************************************************
// File name     : c_union_define_pme.h
// Version       : 2.0
// Author        : xxx
// Created       : 2016-02-04
// Last Modified : 
// Description   :  The C union definition file for the module pme
// Function List : 
// History       : 
// 1 Date        : 
// Author        : xxx
// Modification  : Create file
//******************************************************************************

#ifndef __C_UNION_DEFINE_PME_H__
#define __C_UNION_DEFINE_PME_H__

/* Define the union U_PME_SW_EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_l0_psw_adapt_en   : 1   ; /* [0]  */
        unsigned int    Reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PME_SW_EN;

/* Define the union U_PME_SW_TR0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_l0_psw_thr0       : 16  ; /* [15..0]  */
        unsigned int    Reserved_1            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PME_SW_TR0;

/* Define the union U_PME_SW_TR1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_l0_psw_thr1       : 16  ; /* [15..0]  */
        unsigned int    Reserved_2            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PME_SW_TR1;

/* Define the union U_PME_SW_TR2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_l0_psw_thr2       : 16  ; /* [15..0]  */
        unsigned int    Reserved_3            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PME_SW_TR2;

/* Define the union U_PME_TR_WEIGHTX */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_tr_weightx_0      : 9   ; /* [8..0]  */
        unsigned int    pme_tr_weightx_1      : 9   ; /* [17..9]  */
        unsigned int    pme_tr_weightx_2      : 9   ; /* [26..18]  */
        unsigned int    Reserved_5            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PME_TR_WEIGHTX;

/* Define the union U_PME_TR_WEIGHTY */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_tr_weighty_0      : 8   ; /* [7..0]  */
        unsigned int    pme_tr_weighty_1      : 8   ; /* [15..8]  */
        unsigned int    pme_tr_weighty_2      : 8   ; /* [23..16]  */
        unsigned int    Reserved_6            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PME_TR_WEIGHTY;

/* Define the union U_PME_SR_WEIGHT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_sr_weight_0       : 4   ; /* [3..0]  */
        unsigned int    pme_sr_weight_1       : 4   ; /* [7..4]  */
        unsigned int    pme_sr_weight_2       : 4   ; /* [11..8]  */
        unsigned int    pme_pskip_strongedge_madi_thr  : 8   ; /* [19..12]  */
        unsigned int    pme_pskip_strongedge_madi_times  : 8   ; /* [27..20]  */
        unsigned int    Reserved_7            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PME_SR_WEIGHT;

/* Define the union U_PME_INTRABLK_DET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_intrablk_det_cost_thr0  : 16  ; /* [15..0]  */
        unsigned int    pme_pskip_mvy_consistency_thr  : 8   ; /* [23..16]  */
        unsigned int    pme_pskip_mvx_consistency_thr  : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PME_INTRABLK_DET;

/* Define the union U_PME_INTRABLK_DET_THR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_intrablk_det_mv_dif_thr1  : 8   ; /* [7..0]  */
        unsigned int    pme_intrablk_det_mv_dif_thr0  : 8   ; /* [15..8]  */
        unsigned int    pme_intrablk_det_mvy_thr  : 8   ; /* [23..16]  */
        unsigned int    pme_intrablk_det_mvx_thr  : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PME_INTRABLK_DET_THR;

/* Define the union U_PME_SKIN_THR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_skin_u_max_thr    : 8   ; /* [7..0]  */
        unsigned int    pme_skin_u_min_thr    : 8   ; /* [15..8]  */
        unsigned int    pme_skin_v_max_thr    : 8   ; /* [23..16]  */
        unsigned int    pme_skin_v_min_thr    : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PME_SKIN_THR;

/* Define the union U_PME_INTRA_LOWPOW */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_intra16_madi_thr  : 8   ; /* [7..0]  */
        unsigned int    pme_intra32_madi_thr  : 8   ; /* [15..8]  */
        unsigned int    pme_intra_lowpow_en   : 1   ; /* [16]  */
        unsigned int    pme_inter_first       : 1   ; /* [17]  */
        unsigned int    Reserved_8            : 14  ; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PME_INTRA_LOWPOW;

/* Define the union U_PME_IBLK_COST_THR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_iblk_pre_cost_thr_h264  : 16  ; /* [15..0]  */
        unsigned int    pme_intrablk_det_cost_thr1  : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PME_IBLK_COST_THR;

/* Define the union U_PME_HIGH_EDGE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_skin_num          : 9   ; /* [8..0]  */
        unsigned int    pme_high_edge_thr     : 8   ; /* [16..9]  */
        unsigned int    pme_high_edge_cnt     : 5   ; /* [21..17]  */
        unsigned int    pme_still_scene_thr   : 9   ; /* [30..22]  */
        unsigned int    Reserved_9            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PME_HIGH_EDGE;

/* Define the union U_PME_LARGE_MOVE_THR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_move_scene_thr    : 8   ; /* [7..0]  */
        unsigned int    pme_move_sad_thr      : 14  ; /* [21..8]  */
        unsigned int    Reserved_11           : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PME_LARGE_MOVE_THR;

/* Define the union U_PME_INTER_STRONG_EDGE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_interdiff_max_min_madi_abs  : 8   ; /* [7..0]  */
        unsigned int    pme_interdiff_max_min_madi_times  : 8   ; /* [15..8]  */
        unsigned int    pme_interstrongedge_madi_thr  : 8   ; /* [23..16]  */
        unsigned int    Reserved_12           : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PME_INTER_STRONG_EDGE;

/* Define the union U_PME_NEW_COST */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_cost_lamda0       : 4   ; /* [3..0]  */
        unsigned int    pme_cost_lamda1       : 4   ; /* [7..4]  */
        unsigned int    Reserved_16           : 4   ; /* [11..8]  */
        unsigned int    pme_new_cost_en       : 2   ; /* [13..12]  */
        unsigned int    Reserved_15           : 2   ; /* [15..14]  */
        unsigned int    Reserved_14           : 1   ; /* [16]  */
        unsigned int    pme_mvp3median_en     : 1   ; /* [17]  */
        unsigned int    Reserved_13           : 14  ; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PME_NEW_COST;

/* Define the union U_PME_WINDOW_SIZE0_L0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_l0_win0_width     : 10  ; /* [9..0]  */
        unsigned int    Reserved_18           : 6   ; /* [15..10]  */
        unsigned int    pme_l0_win0_height    : 9   ; /* [24..16]  */
        unsigned int    Reserved_17           : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PME_WINDOW_SIZE0_L0;

/* Define the union U_PME_WINDOW_SIZE1_L0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_l0_win1_width     : 10  ; /* [9..0]  */
        unsigned int    Reserved_20           : 6   ; /* [15..10]  */
        unsigned int    pme_l0_win1_height    : 9   ; /* [24..16]  */
        unsigned int    Reserved_19           : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PME_WINDOW_SIZE1_L0;

/* Define the union U_PME_WINDOW_SIZE2_L0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_l0_win2_width     : 10  ; /* [9..0]  */
        unsigned int    Reserved_22           : 6   ; /* [15..10]  */
        unsigned int    pme_l0_win2_height    : 9   ; /* [24..16]  */
        unsigned int    Reserved_21           : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PME_WINDOW_SIZE2_L0;

/* Define the union U_PME_WINDOW_SIZE3_L0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_l0_win3_width     : 10  ; /* [9..0]  */
        unsigned int    Reserved_24           : 6   ; /* [15..10]  */
        unsigned int    pme_l0_win3_height    : 9   ; /* [24..16]  */
        unsigned int    Reserved_23           : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PME_WINDOW_SIZE3_L0;

/* Define the union U_PME_COST_OFFSET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_l0_cost_offset    : 16  ; /* [15..0]  */
        unsigned int    Reserved_26           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PME_COST_OFFSET;

/* Define the union U_PME_SAFE_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_safe_line         : 13  ; /* [12..0]  */
        unsigned int    pme_safe_line_val     : 1   ; /* [13]  */
        unsigned int    Reserved_27           : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PME_SAFE_CFG;

/* Define the union U_PME_IBLK_REFRESH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_iblk_refresh_start_num  : 18  ; /* [17..0]  */
        unsigned int    Reserved_28           : 14  ; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PME_IBLK_REFRESH;

/* Define the union U_PME_IBLK_REFRESH_NUM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_iblk_refresh_num  : 18  ; /* [17..0]  */
        unsigned int    Reserved_29           : 14  ; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PME_IBLK_REFRESH_NUM;

/* Define the union U_PME_QPG_RC_THR0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    Reserved_31           : 16  ; /* [15..0]  */
        unsigned int    pme_madi_dif_thr      : 7   ; /* [22..16]  */
        unsigned int    pme_cur_madi_dif_thr  : 7   ; /* [29..23]  */
        unsigned int    Reserved_30           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PME_QPG_RC_THR0;

/* Define the union U_PME_LOW_LUMA_THR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_low_luma_thr      : 8   ; /* [7..0]  */
        unsigned int    pme_low_luma_madi_thr  : 8   ; /* [15..8]  */
        unsigned int    pme_high_luma_thr     : 8   ; /* [23..16]  */
        unsigned int    Reserved_32           : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PME_LOW_LUMA_THR;

/* Define the union U_PME_QPG_RC_THR1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_min_sad_thr_offset  : 8   ; /* [7..0]  */
        unsigned int    pme_min_sad_thr_gain  : 4   ; /* [11..8]  */
        unsigned int    pme_smooth_madi_thr   : 8   ; /* [19..12]  */
        unsigned int    pme_min_sad_thr_offset_cur  : 8   ; /* [27..20]  */
        unsigned int    pme_min_sad_thr_gain_cur  : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PME_QPG_RC_THR1;

/* Define the union U_PME_PBLK_PRE1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_pblk_pre_mv_dif_thr1  : 8   ; /* [7..0]  */
        unsigned int    pme_pblk_pre_mv_dif_thr0  : 8   ; /* [15..8]  */
        unsigned int    pme_pblk_pre_mv_dif_cost_thr  : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PME_PBLK_PRE1;

/* Define the union U_PME_MADI_SUM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_madi_sum          : 25  ; /* [24..0]  */
        unsigned int    Reserved_33           : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PME_MADI_SUM;

/* Define the union U_PME_MADP_SUM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_madp_sum          : 25  ; /* [24..0]  */
        unsigned int    Reserved_34           : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PME_MADP_SUM;

/* Define the union U_PME_MADI_NUM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_madi_num          : 17  ; /* [16..0]  */
        unsigned int    Reserved_35           : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PME_MADI_NUM;

/* Define the union U_PME_MADP_NUM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_madp_num          : 17  ; /* [16..0]  */
        unsigned int    Reserved_36           : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PME_MADP_NUM;

//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile U_PME_SW_EN            PME_SW_EN;
    volatile U_PME_SW_TR0           PME_SW_TR0;
    volatile U_PME_SW_TR1           PME_SW_TR1;
    volatile U_PME_SW_TR2           PME_SW_TR2;
    volatile unsigned int           RESERVED3;
    volatile U_PME_TR_WEIGHTX       PME_TR_WEIGHTX;
    volatile U_PME_TR_WEIGHTY       PME_TR_WEIGHTY;
    volatile U_PME_SR_WEIGHT        PME_SR_WEIGHT;
    volatile U_PME_INTRABLK_DET     PME_INTRABLK_DET;
    volatile U_PME_INTRABLK_DET_THR   PME_INTRABLK_DET_THR;
    volatile unsigned int           PME_IBLK_LCU_NUM;
    volatile unsigned int           PME_PBLK_LCU_NUM;
    volatile U_PME_SKIN_THR         PME_SKIN_THR;
    volatile U_PME_INTRA_LOWPOW     PME_INTRA_LOWPOW;
    volatile U_PME_IBLK_COST_THR    PME_IBLK_COST_THR;
    volatile U_PME_HIGH_EDGE        PME_HIGH_EDGE;
    volatile unsigned int           RESERVED0[33];
    volatile U_PME_LARGE_MOVE_THR   PME_LARGE_MOVE_THR;
    volatile U_PME_INTER_STRONG_EDGE   PME_INTER_STRONG_EDGE;
    volatile U_PME_NEW_COST         PME_NEW_COST;
    volatile U_PME_WINDOW_SIZE0_L0   PME_WINDOW_SIZE0_L0;
    volatile U_PME_WINDOW_SIZE1_L0   PME_WINDOW_SIZE1_L0;
    volatile U_PME_WINDOW_SIZE2_L0   PME_WINDOW_SIZE2_L0;
    volatile U_PME_WINDOW_SIZE3_L0   PME_WINDOW_SIZE3_L0;
    volatile unsigned int           RESERVED2[4];
    volatile U_PME_COST_OFFSET      PME_COST_OFFSET;
    volatile U_PME_SAFE_CFG         PME_SAFE_CFG;
    volatile U_PME_IBLK_REFRESH     PME_IBLK_REFRESH;
    volatile U_PME_IBLK_REFRESH_NUM   PME_IBLK_REFRESH_NUM;
    volatile U_PME_QPG_RC_THR0      PME_QPG_RC_THR0;
    volatile U_PME_LOW_LUMA_THR     PME_LOW_LUMA_THR;
    volatile U_PME_QPG_RC_THR1      PME_QPG_RC_THR1;
    volatile U_PME_PBLK_PRE1        PME_PBLK_PRE1;
    volatile U_PME_MADI_SUM         PME_MADI_SUM;
    volatile U_PME_MADP_SUM         PME_MADP_SUM;
    volatile U_PME_MADI_NUM         PME_MADI_NUM;
    volatile U_PME_MADP_NUM         PME_MADP_NUM;
    volatile unsigned int           RESERVED1[56];


} S_pme_REGS_TYPE;

/* Declare the struct pointor of the module pme */
extern volatile S_pme_REGS_TYPE *goppmeAllReg;

/* Declare the functions that set the member value */
int iSetPME_SW_ENpme_l0_psw_adapt_en(unsigned int upme_l0_psw_adapt_en);
int iSetPME_SW_TR0pme_l0_psw_thr0(unsigned int upme_l0_psw_thr0);
int iSetPME_SW_TR1pme_l0_psw_thr1(unsigned int upme_l0_psw_thr1);
int iSetPME_SW_TR2pme_l0_psw_thr2(unsigned int upme_l0_psw_thr2);
int iSetPME_TR_WEIGHTXpme_tr_weightx_0(unsigned int upme_tr_weightx_0);
int iSetPME_TR_WEIGHTXpme_tr_weightx_1(unsigned int upme_tr_weightx_1);
int iSetPME_TR_WEIGHTXpme_tr_weightx_2(unsigned int upme_tr_weightx_2);
int iSetPME_TR_WEIGHTYpme_tr_weighty_0(unsigned int upme_tr_weighty_0);
int iSetPME_TR_WEIGHTYpme_tr_weighty_1(unsigned int upme_tr_weighty_1);
int iSetPME_TR_WEIGHTYpme_tr_weighty_2(unsigned int upme_tr_weighty_2);
int iSetPME_SR_WEIGHTpme_sr_weight_0(unsigned int upme_sr_weight_0);
int iSetPME_SR_WEIGHTpme_sr_weight_1(unsigned int upme_sr_weight_1);
int iSetPME_SR_WEIGHTpme_sr_weight_2(unsigned int upme_sr_weight_2);
int iSetPME_SR_WEIGHTpme_pskip_strongedge_madi_thr(unsigned int upme_pskip_strongedge_madi_thr);
int iSetPME_SR_WEIGHTpme_pskip_strongedge_madi_times(unsigned int upme_pskip_strongedge_madi_times);
int iSetPME_INTRABLK_DETpme_intrablk_det_cost_thr0(unsigned int upme_intrablk_det_cost_thr0);
int iSetPME_INTRABLK_DETpme_pskip_mvy_consistency_thr(unsigned int upme_pskip_mvy_consistency_thr);
int iSetPME_INTRABLK_DETpme_pskip_mvx_consistency_thr(unsigned int upme_pskip_mvx_consistency_thr);
int iSetPME_INTRABLK_DET_THRpme_intrablk_det_mv_dif_thr1(unsigned int upme_intrablk_det_mv_dif_thr1);
int iSetPME_INTRABLK_DET_THRpme_intrablk_det_mv_dif_thr0(unsigned int upme_intrablk_det_mv_dif_thr0);
int iSetPME_INTRABLK_DET_THRpme_intrablk_det_mvy_thr(unsigned int upme_intrablk_det_mvy_thr);
int iSetPME_INTRABLK_DET_THRpme_intrablk_det_mvx_thr(unsigned int upme_intrablk_det_mvx_thr);
int iSetPME_SKIN_THRpme_skin_u_max_thr(unsigned int upme_skin_u_max_thr);
int iSetPME_SKIN_THRpme_skin_u_min_thr(unsigned int upme_skin_u_min_thr);
int iSetPME_SKIN_THRpme_skin_v_max_thr(unsigned int upme_skin_v_max_thr);
int iSetPME_SKIN_THRpme_skin_v_min_thr(unsigned int upme_skin_v_min_thr);
int iSetPME_INTRA_LOWPOWpme_intra16_madi_thr(unsigned int upme_intra16_madi_thr);
int iSetPME_INTRA_LOWPOWpme_intra32_madi_thr(unsigned int upme_intra32_madi_thr);
int iSetPME_INTRA_LOWPOWpme_intra_lowpow_en(unsigned int upme_intra_lowpow_en);
int iSetPME_INTRA_LOWPOWpme_inter_first(unsigned int upme_inter_first);
int iSetPME_IBLK_COST_THRpme_iblk_pre_cost_thr_h264(unsigned int upme_iblk_pre_cost_thr_h264);
int iSetPME_IBLK_COST_THRpme_intrablk_det_cost_thr1(unsigned int upme_intrablk_det_cost_thr1);
int iSetPME_HIGH_EDGEpme_skin_num(unsigned int upme_skin_num);
int iSetPME_HIGH_EDGEpme_high_edge_thr(unsigned int upme_high_edge_thr);
int iSetPME_HIGH_EDGEpme_high_edge_cnt(unsigned int upme_high_edge_cnt);
int iSetPME_HIGH_EDGEpme_still_scene_thr(unsigned int upme_still_scene_thr);
int iSetPME_LARGE_MOVE_THRpme_move_scene_thr(unsigned int upme_move_scene_thr);
int iSetPME_LARGE_MOVE_THRpme_move_sad_thr(unsigned int upme_move_sad_thr);
int iSetPME_INTER_STRONG_EDGEpme_interdiff_max_min_madi_abs(unsigned int upme_interdiff_max_min_madi_abs);
int iSetPME_INTER_STRONG_EDGEpme_interdiff_max_min_madi_times(unsigned int upme_interdiff_max_min_madi_times);
int iSetPME_INTER_STRONG_EDGEpme_interstrongedge_madi_thr(unsigned int upme_interstrongedge_madi_thr);
int iSetPME_NEW_COSTpme_cost_lamda0(unsigned int upme_cost_lamda0);
int iSetPME_NEW_COSTpme_cost_lamda1(unsigned int upme_cost_lamda1);
int iSetPME_NEW_COSTpme_new_cost_en(unsigned int upme_new_cost_en);
int iSetPME_NEW_COSTpme_mvp3median_en(unsigned int upme_mvp3median_en);
int iSetPME_WINDOW_SIZE0_L0pme_l0_win0_width(unsigned int upme_l0_win0_width);
int iSetPME_WINDOW_SIZE0_L0pme_l0_win0_height(unsigned int upme_l0_win0_height);
int iSetPME_WINDOW_SIZE1_L0pme_l0_win1_width(unsigned int upme_l0_win1_width);
int iSetPME_WINDOW_SIZE1_L0pme_l0_win1_height(unsigned int upme_l0_win1_height);
int iSetPME_WINDOW_SIZE2_L0pme_l0_win2_width(unsigned int upme_l0_win2_width);
int iSetPME_WINDOW_SIZE2_L0pme_l0_win2_height(unsigned int upme_l0_win2_height);
int iSetPME_WINDOW_SIZE3_L0pme_l0_win3_width(unsigned int upme_l0_win3_width);
int iSetPME_WINDOW_SIZE3_L0pme_l0_win3_height(unsigned int upme_l0_win3_height);
int iSetPME_COST_OFFSETpme_l0_cost_offset(unsigned int upme_l0_cost_offset);
int iSetPME_SAFE_CFGpme_safe_line(unsigned int upme_safe_line);
int iSetPME_SAFE_CFGpme_safe_line_val(unsigned int upme_safe_line_val);
int iSetPME_IBLK_REFRESHpme_iblk_refresh_start_num(unsigned int upme_iblk_refresh_start_num);
int iSetPME_IBLK_REFRESH_NUMpme_iblk_refresh_num(unsigned int upme_iblk_refresh_num);
int iSetPME_QPG_RC_THR0pme_madi_dif_thr(unsigned int upme_madi_dif_thr);
int iSetPME_QPG_RC_THR0pme_cur_madi_dif_thr(unsigned int upme_cur_madi_dif_thr);
int iSetPME_LOW_LUMA_THRpme_low_luma_thr(unsigned int upme_low_luma_thr);
int iSetPME_LOW_LUMA_THRpme_low_luma_madi_thr(unsigned int upme_low_luma_madi_thr);
int iSetPME_LOW_LUMA_THRpme_high_luma_thr(unsigned int upme_high_luma_thr);
int iSetPME_QPG_RC_THR1pme_min_sad_thr_offset(unsigned int upme_min_sad_thr_offset);
int iSetPME_QPG_RC_THR1pme_min_sad_thr_gain(unsigned int upme_min_sad_thr_gain);
int iSetPME_QPG_RC_THR1pme_smooth_madi_thr(unsigned int upme_smooth_madi_thr);
int iSetPME_QPG_RC_THR1pme_min_sad_thr_offset_cur(unsigned int upme_min_sad_thr_offset_cur);
int iSetPME_QPG_RC_THR1pme_min_sad_thr_gain_cur(unsigned int upme_min_sad_thr_gain_cur);
int iSetPME_PBLK_PRE1pme_pblk_pre_mv_dif_thr1(unsigned int upme_pblk_pre_mv_dif_thr1);
int iSetPME_PBLK_PRE1pme_pblk_pre_mv_dif_thr0(unsigned int upme_pblk_pre_mv_dif_thr0);
int iSetPME_PBLK_PRE1pme_pblk_pre_mv_dif_cost_thr(unsigned int upme_pblk_pre_mv_dif_cost_thr);



#endif /* __C_UNION_DEFINE_PME_H__ */
