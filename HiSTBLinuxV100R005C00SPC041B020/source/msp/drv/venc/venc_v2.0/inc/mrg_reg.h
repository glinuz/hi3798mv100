//******************************************************************************
//  Copyright (C), 2007-2016, Hisilicon Technologies Co., Ltd.
//
//******************************************************************************
// File name     : c_union_define_mrg.h
// Version       : 2.0
// Author        : xxx
// Created       : 2016-02-04
// Last Modified : 
// Description   :  The C union definition file for the module mrg
// Function List : 
// History       : 
// 1 Date        : 
// Author        : xxx
// Modification  : Create file
//******************************************************************************

#ifndef __C_UNION_DEFINE_MRG_H__
#define __C_UNION_DEFINE_MRG_H__

/* Define the union U_MRG_PMV_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mrg_pmv_pu8_cnt       : 7   ; /* [6..0]  */
        unsigned int    Reserved_7            : 1   ; /* [7]  */
        unsigned int    mrg_pmv_pu16_cnt      : 8   ; /* [15..8]  */
        unsigned int    mrg_pmv_pu32_cnt      : 5   ; /* [20..16]  */
        unsigned int    mrg_pmv_pu64_cnt      : 1   ; /* [21]  */
        unsigned int    Reserved_6            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MRG_PMV_CNT;

/* Define the union U_MRG_FMV_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mrg_fme_pu8_cnt       : 7   ; /* [6..0]  */
        unsigned int    Reserved_9            : 1   ; /* [7]  */
        unsigned int    mrg_fme_pu16_cnt      : 8   ; /* [15..8]  */
        unsigned int    mrg_fme_pu32_cnt      : 5   ; /* [20..16]  */
        unsigned int    mrg_fme_pu64_cnt      : 1   ; /* [21]  */
        unsigned int    Reserved_8            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MRG_FMV_CNT;

/* Define the union U_MRG_LCU_STA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mrg_lcu_oper_cnt      : 16  ; /* [15..0]  */
        unsigned int    mrg_lcu_oper_state    : 1   ; /* [16]  */
        unsigned int    Reserved_10           : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MRG_LCU_STA;

/* Define the union U_MRG_LCU_INFO_STA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mrg_lcu_roikeep_en    : 16  ; /* [15..0]  */
        unsigned int    mrg_cu_info           : 2   ; /* [17..16]  */
        unsigned int    mrg_layer_info        : 2   ; /* [19..18]  */
        unsigned int    mrg_lcu_roikeep       : 1   ; /* [20]  */
        unsigned int    mrg_lcu_lpw_en        : 1   ; /* [21]  */
        unsigned int    Reserved_11           : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MRG_LCU_INFO_STA;

//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile unsigned int           RESERVED3;
    volatile unsigned int           RESERVED1;
    volatile unsigned int           RESERVED4;
    volatile unsigned int           RESERVED5;
    volatile unsigned int           RESERVED2[4];
    volatile unsigned int           MRG_INT_GTCK_CNT;
    volatile unsigned int           MRG_FRAC_GTCK_CNT;
    volatile unsigned int           RESERVED7[2];
    volatile unsigned int           MRG_PU0_INFO;
    volatile unsigned int           MRG_PU1_INFO;
    volatile unsigned int           MRG_PU2_INFO;
    volatile unsigned int           MRG_PU3_INFO;
    volatile U_MRG_PMV_CNT          MRG_PMV_CNT;
    volatile U_MRG_FMV_CNT          MRG_FMV_CNT;
    volatile U_MRG_LCU_STA          MRG_LCU_STA;
    volatile U_MRG_LCU_INFO_STA     MRG_LCU_INFO_STA;
    volatile unsigned int           RESERVED9[108];


} S_mrg_REGS_TYPE;

/* Declare the struct pointor of the module mrg */
extern volatile S_mrg_REGS_TYPE *gopmrgAllReg;

/* Declare the functions that set the member value */



#endif /* __C_UNION_DEFINE_MRG_H__ */
