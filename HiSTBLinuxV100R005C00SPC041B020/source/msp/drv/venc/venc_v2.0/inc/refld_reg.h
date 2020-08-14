//******************************************************************************
//  Copyright (C), 2007-2016, Hisilicon Technologies Co., Ltd.
//
//******************************************************************************
// File name     : c_union_define_refld.h
// Version       : 2.0
// Author        : xxx
// Created       : 2016-02-04
// Last Modified : 
// Description   :  The C union definition file for the module refld
// Function List : 
// History       : 
// 1 Date        : 
// Author        : xxx
// Modification  : Create file
//******************************************************************************

#ifndef __C_UNION_DEFINE_REFLD_H__
#define __C_UNION_DEFINE_REFLD_H__

/* Define the union U_REFLD_CMD_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    Reserved_1            : 20  ; /* [19..0]  */
        unsigned int    Reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_REFLD_CMD_CFG;

/* Define the union U_REFLD_LOAD_WINSIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    refld_ref0_width      : 5   ; /* [4..0]  */
        unsigned int    refld_ref0_height     : 5   ; /* [9..5]  */
        unsigned int    refld_ref1_width      : 5   ; /* [14..10]  */
        unsigned int    refld_ref1_height     : 5   ; /* [19..15]  */
        unsigned int    refld_col2gray_en     : 1   ; /* [20]  */
        unsigned int    Reserved_2            : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_REFLD_LOAD_WINSIZE;

/* Define the union U_REFLD_FETCH_STATE0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    poll_curr_st          : 4   ; /* [3..0]  */
        unsigned int    icmd_info_empty       : 1   ; /* [4]  */
        unsigned int    dcmp_state            : 1   ; /* [5]  */
        unsigned int    swblk_luma            : 1   ; /* [6]  */
        unsigned int    swblk_refidx          : 1   ; /* [7]  */
        unsigned int    swblk_intra_flag      : 1   ; /* [8]  */
        unsigned int    dcmp_head_empty       : 1   ; /* [9]  */
        unsigned int    fetch_fifo_empty      : 1   ; /* [10]  */
        unsigned int    fetch_fifo_afull      : 1   ; /* [11]  */
        unsigned int    req_fifo_empty        : 1   ; /* [12]  */
        unsigned int    req_fifo_afull        : 1   ; /* [13]  */
        unsigned int    ocmd_fifo_empty       : 1   ; /* [14]  */
        unsigned int    ocmd_fifo_full        : 1   ; /* [15]  */
        unsigned int    dmov_fifo_full        : 1   ; /* [16]  */
        unsigned int    poll_hit              : 1   ; /* [17]  */
        unsigned int    locktab_rdy           : 1   ; /* [18]  */
        unsigned int    lock_way_cnt          : 4   ; /* [22..19]  */
        unsigned int    cache_wr_stg          : 1   ; /* [23]  */
        unsigned int    Reserved_3            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_REFLD_FETCH_STATE0;

/* Define the union U_REFLD_DMOV_STATE0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dmov_luma_lcuy        : 7   ; /* [6..0]  */
        unsigned int    dmov_luma_lcux        : 7   ; /* [13..7]  */
        unsigned int    dmov_chma_lcuy        : 7   ; /* [20..14]  */
        unsigned int    dmov_chma_lcux        : 7   ; /* [27..21]  */
        unsigned int    blk_end_cnt           : 2   ; /* [29..28]  */
        unsigned int    dmov_fbuf_rrq         : 1   ; /* [30]  */
        unsigned int    dmov_fbuf_rac         : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_REFLD_DMOV_STATE0;

/* Define the union U_REFLD_DMOV_STATE1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    luma_over_cnt         : 16  ; /* [15..0]  */
        unsigned int    chma_over_cnt         : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_REFLD_DMOV_STATE1;

/* Define the union U_REFLD_HEAD_STATE0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    refld_head_curr_lcuy  : 10  ; /* [9..0]  */
        unsigned int    refld_head_data_enough  : 1   ; /* [10]  */
        unsigned int    refld_head_data_refresh  : 1   ; /* [11]  */
        unsigned int    refld_head_curr_lcux  : 8   ; /* [19..12]  */
        unsigned int    head_curr_width       : 7   ; /* [26..20]  */
        unsigned int    head_info_os_empty    : 1   ; /* [27]  */
        unsigned int    head_info_os_full     : 1   ; /* [28]  */
        unsigned int    head_info_ld_empty    : 1   ; /* [29]  */
        unsigned int    Reserved_4            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_REFLD_HEAD_STATE0;

/* Define the union U_REFLD_HEAD_STATE1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    refld_head_vctrl_lcuy  : 9   ; /* [8..0]  */
        unsigned int    refld_head_vctrl_lcux  : 9   ; /* [17..9]  */
        unsigned int    Reserved_5            : 2   ; /* [19..18]  */
        unsigned int    refld_head_sw_height  : 7   ; /* [26..20]  */
        unsigned int    head_info_ld_cnt      : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_REFLD_HEAD_STATE1;

/* Define the union U_REFLD_DCMP_DATA_STATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dcmp_sgmt0_error      : 1   ; /* [0]  */
        unsigned int    dcmp_sgmt0_consume    : 1   ; /* [1]  */
        unsigned int    dcmp_sgmt0_forgive    : 1   ; /* [2]  */
        unsigned int    dcmp_sgmt1_error      : 1   ; /* [3]  */
        unsigned int    dcmp_sgmt1_consume    : 1   ; /* [4]  */
        unsigned int    dcmp_sgmt1_forgive    : 1   ; /* [5]  */
        unsigned int    refld2dcmp_data_empty  : 1   ; /* [6]  */
        unsigned int    dcmp_obuf_rdy         : 4   ; /* [10..7]  */
        unsigned int    dcmp_obuf_blkr_rptr   : 2   ; /* [12..11]  */
        unsigned int    dcmp_obuf0_blk_wptr   : 1   ; /* [13]  */
        unsigned int    dcmp_obuf1_blk_wptr   : 1   ; /* [14]  */
        unsigned int    dcmp_obuf_dvld        : 1   ; /* [15]  */
        unsigned int    dcmp2refld_obuf_emptyn  : 1   ; /* [16]  */
        unsigned int    Reserved_6            : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_REFLD_DCMP_DATA_STATE;

//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile U_REFLD_CMD_CFG        REFLD_CMD_CFG;
    volatile U_REFLD_LOAD_WINSIZE   REFLD_LOAD_WINSIZE;
    volatile U_REFLD_FETCH_STATE0   REFLD_FETCH_STATE0;
    volatile U_REFLD_DMOV_STATE0    REFLD_DMOV_STATE0;
    volatile U_REFLD_DMOV_STATE1    REFLD_DMOV_STATE1;
    volatile unsigned int           REFLD_CACHE_HIT_CNT;
    volatile unsigned int           REFLD_CACHE_MISS_CNT;
    volatile unsigned int           REFLD_DATA_REQ_CNT;
    volatile unsigned int           REFLD_HEAD_REQ_CNT;
    volatile U_REFLD_HEAD_STATE0    REFLD_HEAD_STATE0;
    volatile U_REFLD_HEAD_STATE1    REFLD_HEAD_STATE1;
    volatile U_REFLD_DCMP_DATA_STATE   REFLD_DCMP_DATA_STATE;
    volatile unsigned int           RESERVED2[116];


} S_refld_REGS_TYPE;

/* Declare the struct pointor of the module refld */
extern volatile S_refld_REGS_TYPE *goprefldAllReg;

/* Declare the functions that set the member value */



#endif /* __C_UNION_DEFINE_REFLD_H__ */
