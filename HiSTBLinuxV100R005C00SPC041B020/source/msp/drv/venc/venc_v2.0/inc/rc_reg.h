//******************************************************************************
//  Copyright (C), 2007-2016, Hisilicon Technologies Co., Ltd.
//
//******************************************************************************
// File name     : c_union_define_rc.h
// Version       : 2.0
// Author        : xxx
// Created       : 2016-02-04
// Last Modified : 
// Description   :  The C union definition file for the module rc
// Function List : 
// History       : 
// 1 Date        : 
// Author        : xxx
// Modification  : Create file
//******************************************************************************

#ifndef __C_UNION_DEFINE_RC_H__
#define __C_UNION_DEFINE_RC_H__

/* Define the union U_RC_CU_QPG_RESTRAIN_QP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rc_qp_restrain_madi_thr  : 6   ; /* [5..0]  */
        unsigned int    Reserved_3            : 2   ; /* [7..6]  */
        unsigned int    rc_qp_restrain_mode   : 2   ; /* [9..8]  */
        unsigned int    rc_qp_restrain_en     : 1   ; /* [10]  */
        unsigned int    Reserved_2            : 1   ; /* [11]  */
        unsigned int    rc_qp_restrain_delta_blk32  : 4   ; /* [15..12]  */
        unsigned int    rc_lambda_qp_offset   : 5   ; /* [20..16]  */
        unsigned int    Reserved_1            : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_RC_CU_QPG_RESTRAIN_QP;

/* Define the union U_RC_CU_MIN_SAD_REG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rc_min_sad_level      : 16  ; /* [15..0]  */
        unsigned int    rc_low_min_sad_mode   : 2   ; /* [17..16]  */
        unsigned int    rc_high_min_sad_mode  : 2   ; /* [19..18]  */
        unsigned int    rc_qp_restrain_delta_blk16  : 4   ; /* [23..20]  */
        unsigned int    rc_min_sad_madi_en    : 1   ; /* [24]  */
        unsigned int    rc_rdo_lambda_choose_mode  : 2   ; /* [26..25]  */
        unsigned int    rc_lambda_inter_stredge_en  : 1   ; /* [27]  */
        unsigned int    rc_min_sad_qp_restrain_en  : 1   ; /* [28]  */
        unsigned int    Reserved_4            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_RC_CU_MIN_SAD_REG;

/* Define the union U_RC_QPG_SMART_REG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rc_smart_cu_level_qp_mode  : 1   ; /* [0]  */
        unsigned int    rc_smart_abs_qp_mode  : 1   ; /* [1]  */
        unsigned int    rc_smart_get_cu32_qp_mode  : 2   ; /* [3..2]  */
        unsigned int    rc_smart_get_cu64_qp_mode  : 2   ; /* [5..4]  */
        unsigned int    rc_qp_detlta_size_cu64  : 1   ; /* [6]  */
        unsigned int    Reserved_9            : 1   ; /* [7]  */
        unsigned int    rc_skin_min_qp        : 6   ; /* [13..8]  */
        unsigned int    Reserved_8            : 2   ; /* [15..14]  */
        unsigned int    Reserved_7            : 6   ; /* [21..16]  */
        unsigned int    Reserved_6            : 2   ; /* [23..22]  */
        unsigned int    rc_stredge_move_min_qp  : 6   ; /* [29..24]  */
        unsigned int    Reserved_5            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_RC_QPG_SMART_REG;

/* Define the union U_RC_SCENCE_MIN_QP0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rc_intra_det_min_qp   : 6   ; /* [5..0]  */
        unsigned int    Reserved_13           : 2   ; /* [7..6]  */
        unsigned int    rc_large_move_min_qp  : 6   ; /* [13..8]  */
        unsigned int    Reserved_12           : 2   ; /* [15..14]  */
        unsigned int    rc_stredge_min_qp     : 6   ; /* [21..16]  */
        unsigned int    Reserved_11           : 2   ; /* [23..22]  */
        unsigned int    rc_lowluma_min_qp     : 6   ; /* [29..24]  */
        unsigned int    Reserved_10           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_RC_SCENCE_MIN_QP0;

/* Define the union U_RC_SCENCE_MIN_QP1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rc_smooth_min_qp      : 6   ; /* [5..0]  */
        unsigned int    rc_row_target_bits    : 25  ; /* [30..6]  */
        unsigned int    Reserved_14           : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_RC_SCENCE_MIN_QP1;

//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile unsigned int           RESERVED0[116];
    volatile U_RC_CU_QPG_RESTRAIN_QP   RC_CU_QPG_RESTRAIN_QP;
    volatile unsigned int           RC_CU_MIN_SAD_THRESH_0;
    volatile unsigned int           RC_CU_MIN_SAD_THRESH_1;
    volatile U_RC_CU_MIN_SAD_REG    RC_CU_MIN_SAD_REG;
    volatile U_RC_QPG_SMART_REG     RC_QPG_SMART_REG;
    volatile U_RC_SCENCE_MIN_QP0    RC_SCENCE_MIN_QP0;
    volatile U_RC_SCENCE_MIN_QP1    RC_SCENCE_MIN_QP1;
    volatile unsigned int           RESERVED1[5];


} S_rc_REGS_TYPE;

/* Declare the struct pointor of the module rc */
extern volatile S_rc_REGS_TYPE *goprcAllReg;

/* Declare the functions that set the member value */
int iSetRC_CU_QPG_RESTRAIN_QPrc_qp_restrain_madi_thr(unsigned int urc_qp_restrain_madi_thr);
int iSetRC_CU_QPG_RESTRAIN_QPrc_qp_restrain_mode(unsigned int urc_qp_restrain_mode);
int iSetRC_CU_QPG_RESTRAIN_QPrc_qp_restrain_en(unsigned int urc_qp_restrain_en);
int iSetRC_CU_QPG_RESTRAIN_QPrc_qp_restrain_delta_blk32(unsigned int urc_qp_restrain_delta_blk32);
int iSetRC_CU_QPG_RESTRAIN_QPrc_lambda_qp_offset(unsigned int urc_lambda_qp_offset);
int iSetRC_CU_MIN_SAD_THRESH_0rc_min_sad_thresh_low(unsigned int urc_min_sad_thresh_low);
int iSetRC_CU_MIN_SAD_THRESH_1rc_min_sad_thresh_high(unsigned int urc_min_sad_thresh_high);
int iSetRC_CU_MIN_SAD_REGrc_min_sad_level(unsigned int urc_min_sad_level);
int iSetRC_CU_MIN_SAD_REGrc_low_min_sad_mode(unsigned int urc_low_min_sad_mode);
int iSetRC_CU_MIN_SAD_REGrc_high_min_sad_mode(unsigned int urc_high_min_sad_mode);
int iSetRC_CU_MIN_SAD_REGrc_qp_restrain_delta_blk16(unsigned int urc_qp_restrain_delta_blk16);
int iSetRC_CU_MIN_SAD_REGrc_min_sad_madi_en(unsigned int urc_min_sad_madi_en);
int iSetRC_CU_MIN_SAD_REGrc_rdo_lambda_choose_mode(unsigned int urc_rdo_lambda_choose_mode);
int iSetRC_CU_MIN_SAD_REGrc_lambda_inter_stredge_en(unsigned int urc_lambda_inter_stredge_en);
int iSetRC_CU_MIN_SAD_REGrc_min_sad_qp_restrain_en(unsigned int urc_min_sad_qp_restrain_en);
int iSetRC_QPG_SMART_REGrc_smart_cu_level_qp_mode(unsigned int urc_smart_cu_level_qp_mode);
int iSetRC_QPG_SMART_REGrc_smart_abs_qp_mode(unsigned int urc_smart_abs_qp_mode);
int iSetRC_QPG_SMART_REGrc_smart_get_cu32_qp_mode(unsigned int urc_smart_get_cu32_qp_mode);
int iSetRC_QPG_SMART_REGrc_smart_get_cu64_qp_mode(unsigned int urc_smart_get_cu64_qp_mode);
int iSetRC_QPG_SMART_REGrc_qp_detlta_size_cu64(unsigned int urc_qp_detlta_size_cu64);
int iSetRC_QPG_SMART_REGrc_skin_min_qp(unsigned int urc_skin_min_qp);
int iSetRC_QPG_SMART_REGrc_stredge_move_min_qp(unsigned int urc_stredge_move_min_qp);
int iSetRC_SCENCE_MIN_QP0rc_intra_det_min_qp(unsigned int urc_intra_det_min_qp);
int iSetRC_SCENCE_MIN_QP0rc_large_move_min_qp(unsigned int urc_large_move_min_qp);
int iSetRC_SCENCE_MIN_QP0rc_stredge_min_qp(unsigned int urc_stredge_min_qp);
int iSetRC_SCENCE_MIN_QP0rc_lowluma_min_qp(unsigned int urc_lowluma_min_qp);
int iSetRC_SCENCE_MIN_QP1rc_smooth_min_qp(unsigned int urc_smooth_min_qp);
int iSetRC_SCENCE_MIN_QP1rc_row_target_bits(unsigned int urc_row_target_bits);



#endif /* __C_UNION_DEFINE_RC_H__ */
