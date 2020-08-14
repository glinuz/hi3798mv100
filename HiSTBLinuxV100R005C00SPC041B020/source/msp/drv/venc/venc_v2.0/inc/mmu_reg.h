//******************************************************************************
//  Copyright (C), 2007-2016, Hisilicon Technologies Co., Ltd.
//
//******************************************************************************
// File name     : c_union_define_mmu.h
// Version       : 2.0
// Author        : xxx
// Created       : 2016-02-04
// Last Modified : 
// Description   :  The C union definition file for the module mmu
// Function List : 
// History       : 
// 1 Date        : 
// Author        : xxx
// Modification  : Create file
//******************************************************************************

#ifndef __C_UNION_DEFINE_MMU_H__
#define __C_UNION_DEFINE_MMU_H__

/* Define the union U_VEDU_SMMU_SCR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    glb_bypass            : 1   ; /* [0]  */
        unsigned int    Reserved_2            : 2   ; /* [2..1]  */
        unsigned int    int_en                : 1   ; /* [3]  */
        unsigned int    Reserved_1            : 12  ; /* [15..4]  */
        unsigned int    ptw_pf                : 4   ; /* [19..16]  */
        unsigned int    Reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SMMU_SCR;

/* Define the union U_VEDU_SMMU_LP_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    auto_clk_gt_en        : 1   ; /* [0]  */
        unsigned int    Reserved_5            : 1   ; /* [1]  */
        unsigned int    smmu_idle             : 1   ; /* [2]  */
        unsigned int    Reserved_4            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SMMU_LP_CTRL;

/* Define the union U_VEDU_SMMU_INTMAS_S */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ints_tlbmiss_msk      : 1   ; /* [0]  */
        unsigned int    ints_ptw_trans_msk    : 1   ; /* [1]  */
        unsigned int    ints_tlbinvalid_rd_msk  : 1   ; /* [2]  */
        unsigned int    ints_tlbinvalid_wr_msk  : 1   ; /* [3]  */
        unsigned int    Reserved_7            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SMMU_INTMAS_S;

/* Define the union U_VEDU_SMMU_INTRAW_S */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ints_tlbmiss_raw      : 1   ; /* [0]  */
        unsigned int    ints_ptw_trans_raw    : 1   ; /* [1]  */
        unsigned int    ints_tlbinvalid_rd_raw  : 1   ; /* [2]  */
        unsigned int    ints_tlbinvalid_wr_raw  : 1   ; /* [3]  */
        unsigned int    Reserved_8            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SMMU_INTRAW_S;

/* Define the union U_VEDU_SMMU_INTSTAT_S */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ints_tlbmiss_stat     : 1   ; /* [0]  */
        unsigned int    ints_ptw_trans_stat   : 1   ; /* [1]  */
        unsigned int    ints_tlbinvalid_rd_stat  : 1   ; /* [2]  */
        unsigned int    ints_tlbinvalid_wr_stat  : 1   ; /* [3]  */
        unsigned int    Reserved_9            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SMMU_INTSTAT_S;

/* Define the union U_VEDU_SMMU_INTCLR_S */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ints_tlbmiss_clr      : 1   ; /* [0]  */
        unsigned int    ints_ptw_trans_clr    : 1   ; /* [1]  */
        unsigned int    ints_tlbinvalid_rd_clr  : 1   ; /* [2]  */
        unsigned int    ints_tlbinvalid_wr_clr  : 1   ; /* [3]  */
        unsigned int    Reserved_10           : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SMMU_INTCLR_S;

/* Define the union U_VEDU_SMMU_INTMASK_NS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intns_tlbmiss_msk     : 1   ; /* [0]  */
        unsigned int    intns_ptw_trans_msk   : 1   ; /* [1]  */
        unsigned int    intns_tlbinvalid_rd_msk  : 1   ; /* [2]  */
        unsigned int    intns_tlbinvalid_wr_msk  : 1   ; /* [3]  */
        unsigned int    Reserved_11           : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SMMU_INTMASK_NS;

/* Define the union U_VEDU_SMMU_INTRAW_NS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intns_tlbmiss_raw     : 1   ; /* [0]  */
        unsigned int    intns_ptw_trans_raw   : 1   ; /* [1]  */
        unsigned int    intns_tlbinvalid_rd_raw  : 1   ; /* [2]  */
        unsigned int    intns_tlbinvalid_wr_raw  : 1   ; /* [3]  */
        unsigned int    Reserved_12           : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SMMU_INTRAW_NS;

/* Define the union U_VEDU_SMMU_INTSTAT_NS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intns_tlbmiss_stat    : 1   ; /* [0]  */
        unsigned int    intns_ptw_trans_stat  : 1   ; /* [1]  */
        unsigned int    intns_tlbinvalid_rd_stat  : 1   ; /* [2]  */
        unsigned int    intns_tlbinvalid_wr_stat  : 1   ; /* [3]  */
        unsigned int    Reserved_13           : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SMMU_INTSTAT_NS;

/* Define the union U_VEDU_SMMU_INTCLR_NS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intns_tlbmiss_clr     : 1   ; /* [0]  */
        unsigned int    intns_ptw_trans_clr   : 1   ; /* [1]  */
        unsigned int    intns_tlbinvalid_rd_clr  : 1   ; /* [2]  */
        unsigned int    intns_tlbinvalid_wr_clr  : 1   ; /* [3]  */
        unsigned int    Reserved_14           : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SMMU_INTCLR_NS;

/* Define the union U_VEDU_SMMU_FAULT_ID_PTW_S */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fault_index_id_s      : 16  ; /* [15..0]  */
        unsigned int    fault_strm_id_s       : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SMMU_FAULT_ID_PTW_S;

/* Define the union U_VEDU_SMMU_FAULT_ID_PTW_NS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fault_index_id_ns     : 16  ; /* [15..0]  */
        unsigned int    fault_strm_id_ns      : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SMMU_FAULT_ID_PTW_NS;

/* Define the union U_VEDU_SMMU_FAULT_PTW_NUM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fault_s_ptw_num       : 4   ; /* [3..0]  */
        unsigned int    fault_ns_ptw_num      : 4   ; /* [7..4]  */
        unsigned int    Reserved_20           : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SMMU_FAULT_PTW_NUM;

/* Define the union U_VEDU_SMMU_FAULT_ID_WR_S */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fault_index_id_wr_s   : 16  ; /* [15..0]  */
        unsigned int    fault_str_id_wr_s     : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SMMU_FAULT_ID_WR_S;

/* Define the union U_VEDU_SMMU_FAULT_ID_WR_NS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fault_index_id_wr_ns  : 16  ; /* [15..0]  */
        unsigned int    fault_str_id_wr_ns    : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SMMU_FAULT_ID_WR_NS;

/* Define the union U_VEDU_SMMU_FAULT_ID_RD_S */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fault_index_id_rd_s   : 16  ; /* [15..0]  */
        unsigned int    fault_str_id_rd_s     : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SMMU_FAULT_ID_RD_S;

/* Define the union U_VEDU_SMMU_FAULT_ID_RD_NS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fault_index_id_rd_ns  : 16  ; /* [15..0]  */
        unsigned int    fault_str_id_rd_ns    : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SMMU_FAULT_ID_RD_NS;

/* Define the union U_VEDU_SMMU_FAULT_TBU_INFO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fault_tlbmiss_err_s   : 2   ; /* [1..0]  */
        unsigned int    fault_tlbinvalid_err_s  : 2   ; /* [3..2]  */
        unsigned int    fault_tlbmiss_err_ns  : 2   ; /* [5..4]  */
        unsigned int    fault_tlbinvalid_err_ns  : 2   ; /* [7..6]  */
        unsigned int    fault_tbu_num         : 4   ; /* [11..8]  */
        unsigned int    Reserved_25           : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SMMU_FAULT_TBU_INFO;

/* Define the union U_VEDU_SMMU_FAULT_TBU_DBG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    aw_valids             : 1   ; /* [0]  */
        unsigned int    aw_readys             : 1   ; /* [1]  */
        unsigned int    tbu_awreadym          : 1   ; /* [2]  */
        unsigned int    awch_stall_n          : 1   ; /* [3]  */
        unsigned int    write_command_counter  : 2   ; /* [5..4]  */
        unsigned int    ar_valids             : 1   ; /* [6]  */
        unsigned int    ar_readys             : 1   ; /* [7]  */
        unsigned int    tbu_arreadym          : 1   ; /* [8]  */
        unsigned int    arch_stall_n          : 1   ; /* [9]  */
        unsigned int    read_command_counter  : 2   ; /* [11..10]  */
        unsigned int    Reserved_26           : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SMMU_FAULT_TBU_DBG;

/* Define the union U_VEDU_SMMU_PREF_BUFFER_EMPTY */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pref_buffer_empty     : 1   ; /* [0]  */
        unsigned int    Reserved_27           : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SMMU_PREF_BUFFER_EMPTY;

/* Define the union U_VEDU_SMMU_PTWQ_IDLE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ptwq0_idle_state      : 1   ; /* [0]  */
        unsigned int    ptwq1_idle_state      : 1   ; /* [1]  */
        unsigned int    ptwq2_idle_state      : 1   ; /* [2]  */
        unsigned int    ptwq3_idle_state      : 1   ; /* [3]  */
        unsigned int    ptwq4_idle_state      : 1   ; /* [4]  */
        unsigned int    ptwq5_idle_state      : 1   ; /* [5]  */
        unsigned int    ptwq6_idle_state      : 1   ; /* [6]  */
        unsigned int    ptwq7_idle_state      : 1   ; /* [7]  */
        unsigned int    ptwq8_idle_state      : 1   ; /* [8]  */
        unsigned int    ptwq9_idle_state      : 1   ; /* [9]  */
        unsigned int    ptwq10_idle_state     : 1   ; /* [10]  */
        unsigned int    ptwq11_idle_state     : 1   ; /* [11]  */
        unsigned int    ptwq12_idle_state     : 1   ; /* [12]  */
        unsigned int    ptwq13_idle_state     : 1   ; /* [13]  */
        unsigned int    ptwq14_idle_state     : 1   ; /* [14]  */
        unsigned int    ptwq15_idle_state     : 1   ; /* [15]  */
        unsigned int    Reserved_28           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SMMU_PTWQ_IDLE;

/* Define the union U_VEDU_SMMU_RESET_STATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    smmu_rst_state        : 1   ; /* [0]  */
        unsigned int    Reserved_29           : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SMMU_RESET_STATE;

/* Define the union U_VEDU_SMMU_MASTER_DBG0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    last_miss_cnt_rd      : 10  ; /* [9..0]  */
        unsigned int    cur_miss_cnt_rd       : 10  ; /* [19..10]  */
        unsigned int    vld_debug_rd          : 4   ; /* [23..20]  */
        unsigned int    rdy_debug_rd          : 4   ; /* [27..24]  */
        unsigned int    in_out_cmd_cnt_rd     : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SMMU_MASTER_DBG0;

/* Define the union U_VEDU_SMMU_MASTER_DBG1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    last_miss_cnt_wr      : 10  ; /* [9..0]  */
        unsigned int    cur_miss_cnt_wr       : 10  ; /* [19..10]  */
        unsigned int    vld_debug_wr          : 4   ; /* [23..20]  */
        unsigned int    rdy_debug_wr          : 4   ; /* [27..24]  */
        unsigned int    in_out_cmd_cnt_wr     : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SMMU_MASTER_DBG1;

/* Define the union U_VEDU_SMMU_MASTER_DBG2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    last_double_miss_cnt_rd  : 8   ; /* [7..0]  */
        unsigned int    cur_double_miss_cnt_rd  : 8   ; /* [15..8]  */
        unsigned int    last_double_upd_cnt_rd  : 8   ; /* [23..16]  */
        unsigned int    cur_double_upd_cnt_rd  : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SMMU_MASTER_DBG2;

/* Define the union U_VEDU_SMMU_MASTER_DBG3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    last_double_miss_cnt_wr  : 8   ; /* [7..0]  */
        unsigned int    cur_double_miss_cnt_wr  : 8   ; /* [15..8]  */
        unsigned int    last_double_upd_cnt_wr  : 7   ; /* [22..16]  */
        unsigned int    cur_double_upd_cnt_wr  : 7   ; /* [29..23]  */
        unsigned int    mst_fsm_cur           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SMMU_MASTER_DBG3;

/* Define the union U_VEDU_SMMU_MASTER_DBG4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cur_sel1_chn_miss_cnt_rd  : 16  ; /* [15..0]  */
        unsigned int    last_sel1_chn_miss_cnt_rd  : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SMMU_MASTER_DBG4;

/* Define the union U_VEDU_SMMU_MASTER_DBG5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cur_sel2_chn_miss_cnt_rd  : 16  ; /* [15..0]  */
        unsigned int    last_sel2_chn_miss_cnt_rd  : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SMMU_MASTER_DBG5;

/* Define the union U_VEDU_SMMU_MASTER_DBG6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cur_sel1_chn_miss_cnt_wr  : 16  ; /* [15..0]  */
        unsigned int    last_sel1_chn_miss_cnt_wr  : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SMMU_MASTER_DBG6;

/* Define the union U_VEDU_SMMU_MASTER_DBG7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cur_sel2_chn_miss_cnt_wr  : 16  ; /* [15..0]  */
        unsigned int    last_sel2_chn_miss_cnt_wr  : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SMMU_MASTER_DBG7;

/* Define the union U_VEDU_SMMU_MASTER_DBG8 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sel2_chn_wr           : 8   ; /* [7..0]  */
        unsigned int    sel1_chn_wr           : 8   ; /* [15..8]  */
        unsigned int    sel2_chn_rd           : 8   ; /* [23..16]  */
        unsigned int    sel1_chn_rd           : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SMMU_MASTER_DBG8;

//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile U_VEDU_SMMU_SCR             SMMU_SCR;
    volatile unsigned int           RESERVED0;
    volatile U_VEDU_SMMU_LP_CTRL         SMMU_LP_CTRL;
    volatile unsigned int           RESERVED1;
    volatile U_VEDU_SMMU_INTMAS_S        SMMU_INTMAS_S;
    volatile U_VEDU_SMMU_INTRAW_S        SMMU_INTRAW_S;
    volatile U_VEDU_SMMU_INTSTAT_S       SMMU_INTSTAT_S;
    volatile U_VEDU_SMMU_INTCLR_S        SMMU_INTCLR_S;
    volatile U_VEDU_SMMU_INTMASK_NS      SMMU_INTMASK_NS;
    volatile U_VEDU_SMMU_INTRAW_NS       SMMU_INTRAW_NS;
    volatile U_VEDU_SMMU_INTSTAT_NS      SMMU_INTSTAT_NS;
    volatile U_VEDU_SMMU_INTCLR_NS       SMMU_INTCLR_NS;
    volatile unsigned int           RESERVED2[118];
    volatile unsigned int           SMMU_SCB_TTBR;
    volatile unsigned int           SMMU_CB_TTBR;
    volatile unsigned int           RESERVED3[56];
    volatile unsigned int           SMMU_ERR_RDADDR_S;
    volatile unsigned int           SMMU_ERR_WRADDR_S;
    volatile unsigned int           RESERVED4[3];
    volatile unsigned int           SMMU_ERR_RDADDR_NS;
    volatile unsigned int           SMMU_ERR_WRADDR_NS;
    volatile unsigned int           RESERVED5;
    volatile unsigned int           SMMU_FAULT_ADDR_PTW_S;
    volatile U_VEDU_SMMU_FAULT_ID_PTW_S   SMMU_FAULT_ID_PTW_S;
    volatile unsigned int           RESERVED6[2];
    volatile unsigned int           SMMU_FAULT_ADDR_PTW_NS;
    volatile U_VEDU_SMMU_FAULT_ID_PTW_NS   SMMU_FAULT_ID_PTW_NS;
    volatile U_VEDU_SMMU_FAULT_PTW_NUM   SMMU_FAULT_PTW_NUM;
    volatile unsigned int           RESERVED7;
    volatile unsigned int           SMMU_FAULT_ADDR_WR_S;
    volatile unsigned int           SMMU_FAULT_TLB_WR_S;
    volatile U_VEDU_SMMU_FAULT_ID_WR_S   SMMU_FAULT_ID_WR_S;
    volatile unsigned int           RESERVED8;
    volatile unsigned int           SMMU_FAULT_ADDR_WR_NS;
    volatile unsigned int           SMMU_FAULT_TLB_WR_NS;
    volatile U_VEDU_SMMU_FAULT_ID_WR_NS   SMMU_FAULT_ID_WR_NS;
    volatile unsigned int           RESERVED9;
    volatile unsigned int           SMMU_FAULT_ADDR_RD_S;
    volatile unsigned int           SMMU_FAULT_TLB_RD_S;
    volatile U_VEDU_SMMU_FAULT_ID_RD_S   SMMU_FAULT_ID_RD_S;
    volatile unsigned int           RESERVED10;
    volatile unsigned int           SMMU_FAULT_ADDR_RD_NS;
    volatile unsigned int           SMMU_FAULT_TLB_RD_NS;
    volatile U_VEDU_SMMU_FAULT_ID_RD_NS   SMMU_FAULT_ID_RD_NS;
    volatile U_VEDU_SMMU_FAULT_TBU_INFO   SMMU_FAULT_TBU_INFO;
    volatile U_VEDU_SMMU_FAULT_TBU_DBG   SMMU_FAULT_TBU_DBG;
    volatile U_VEDU_SMMU_PREF_BUFFER_EMPTY   SMMU_PREF_BUFFER_EMPTY;
    volatile U_VEDU_SMMU_PTWQ_IDLE       SMMU_PTWQ_IDLE;
    volatile U_VEDU_SMMU_RESET_STATE     SMMU_RESET_STATE;
    volatile U_VEDU_SMMU_MASTER_DBG0     SMMU_MASTER_DBG0;
    volatile U_VEDU_SMMU_MASTER_DBG1     SMMU_MASTER_DBG1;
    volatile U_VEDU_SMMU_MASTER_DBG2     SMMU_MASTER_DBG2;
    volatile U_VEDU_SMMU_MASTER_DBG3     SMMU_MASTER_DBG3;
    volatile unsigned int           RESERVED11[100];
    volatile U_VEDU_SMMU_MASTER_DBG4     SMMU_MASTER_DBG4;
    volatile U_VEDU_SMMU_MASTER_DBG5     SMMU_MASTER_DBG5;
    volatile U_VEDU_SMMU_MASTER_DBG6     SMMU_MASTER_DBG6;
    volatile U_VEDU_SMMU_MASTER_DBG7     SMMU_MASTER_DBG7;
    volatile U_VEDU_SMMU_MASTER_DBG8     SMMU_MASTER_DBG8;
    volatile unsigned int           RESERVED12[76];
   //add by c00346155
   // volatile U_VEDU_CHN_BYPASS
   
   //add end

} S_mmu_REGS_TYPE;

/* Declare the struct pointor of the module mmu */
extern volatile S_mmu_REGS_TYPE *gopmmuAllReg;

/* Declare the functions that set the member value */
int iSetSMMU_SCRglb_bypass(unsigned int uglb_bypass);
int iSetSMMU_SCRint_en(unsigned int uint_en);
int iSetSMMU_SCRptw_pf(unsigned int uptw_pf);
int iSetSMMU_LP_CTRLauto_clk_gt_en(unsigned int uauto_clk_gt_en);
int iSetSMMU_INTMAS_Sints_tlbmiss_msk(unsigned int uints_tlbmiss_msk);
int iSetSMMU_INTMAS_Sints_ptw_trans_msk(unsigned int uints_ptw_trans_msk);
int iSetSMMU_INTMAS_Sints_tlbinvalid_rd_msk(unsigned int uints_tlbinvalid_rd_msk);
int iSetSMMU_INTMAS_Sints_tlbinvalid_wr_msk(unsigned int uints_tlbinvalid_wr_msk);
int iSetSMMU_INTCLR_Sints_tlbmiss_clr(unsigned int uints_tlbmiss_clr);
int iSetSMMU_INTCLR_Sints_ptw_trans_clr(unsigned int uints_ptw_trans_clr);
int iSetSMMU_INTCLR_Sints_tlbinvalid_rd_clr(unsigned int uints_tlbinvalid_rd_clr);
int iSetSMMU_INTCLR_Sints_tlbinvalid_wr_clr(unsigned int uints_tlbinvalid_wr_clr);
int iSetSMMU_INTMASK_NSintns_tlbmiss_msk(unsigned int uintns_tlbmiss_msk);
int iSetSMMU_INTMASK_NSintns_ptw_trans_msk(unsigned int uintns_ptw_trans_msk);
int iSetSMMU_INTMASK_NSintns_tlbinvalid_rd_msk(unsigned int uintns_tlbinvalid_rd_msk);
int iSetSMMU_INTMASK_NSintns_tlbinvalid_wr_msk(unsigned int uintns_tlbinvalid_wr_msk);
int iSetSMMU_INTCLR_NSintns_tlbmiss_clr(unsigned int uintns_tlbmiss_clr);
int iSetSMMU_INTCLR_NSintns_ptw_trans_clr(unsigned int uintns_ptw_trans_clr);
int iSetSMMU_INTCLR_NSintns_tlbinvalid_rd_clr(unsigned int uintns_tlbinvalid_rd_clr);
int iSetSMMU_INTCLR_NSintns_tlbinvalid_wr_clr(unsigned int uintns_tlbinvalid_wr_clr);
int iSetSMMU_SCB_TTBRscb_ttbr(unsigned int uscb_ttbr);
int iSetSMMU_CB_TTBRcb_ttbr(unsigned int ucb_ttbr);
int iSetSMMU_ERR_RDADDR_Serr_s_rd_addr(unsigned int uerr_s_rd_addr);
int iSetSMMU_ERR_WRADDR_Serr_s_wr_addr(unsigned int uerr_s_wr_addr);
int iSetSMMU_ERR_RDADDR_NSerr_ns_rd_addr(unsigned int uerr_ns_rd_addr);
int iSetSMMU_ERR_WRADDR_NSerr_ns_wr_addr(unsigned int uerr_ns_wr_addr);
int iSetSMMU_MASTER_DBG8sel2_chn_wr(unsigned int usel2_chn_wr);
int iSetSMMU_MASTER_DBG8sel1_chn_wr(unsigned int usel1_chn_wr);
int iSetSMMU_MASTER_DBG8sel2_chn_rd(unsigned int usel2_chn_rd);
int iSetSMMU_MASTER_DBG8sel1_chn_rd(unsigned int usel1_chn_rd);



#endif /* __C_UNION_DEFINE_MMU_H__ */
