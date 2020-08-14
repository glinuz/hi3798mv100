//******************************************************************************
//  Copyright (C), 2007-2016, Hisilicon Technologies Co., Ltd.
//
//******************************************************************************
// File name     : c_union_define_fme.h
// Version       : 2.0
// Author        : xxx
// Created       : 2016-02-04
// Last Modified : 
// Description   :  The C union definition file for the module fme
// Function List : 
// History       : 
// 1 Date        : 
// Author        : xxx
// Modification  : Create file
//******************************************************************************

#ifndef __C_UNION_DEFINE_FME_H__
#define __C_UNION_DEFINE_FME_H__

/* Define the union U_FME_BIAS_COST0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    Reserved_1            : 16  ; /* [15..0]  */
        unsigned int    Reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_FME_BIAS_COST0;

/* Define the union U_FME_BIAS_COST1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    Reserved_3            : 16  ; /* [15..0]  */
        unsigned int    Reserved_2            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_FME_BIAS_COST1;

/* Define the union U_FME_PMV_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fme_pmv_pu8_cnt       : 7   ; /* [6..0]  */
        unsigned int    Reserved_7            : 1   ; /* [7]  */
        unsigned int    fme_pmv_pu16_cnt      : 8   ; /* [15..8]  */
        unsigned int    fme_pmv_pu32_cnt      : 5   ; /* [20..16]  */
        unsigned int    fme_pmv_pu64_cnt      : 1   ; /* [21]  */
        unsigned int    Reserved_6            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_FME_PMV_CNT;

/* Define the union U_FME_IMV_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fme_ime_pu8_cnt       : 7   ; /* [6..0]  */
        unsigned int    Reserved_9            : 1   ; /* [7]  */
        unsigned int    fme_ime_pu16_cnt      : 8   ; /* [15..8]  */
        unsigned int    fme_ime_pu32_cnt      : 5   ; /* [20..16]  */
        unsigned int    fme_ime_pu64_cnt      : 1   ; /* [21]  */
        unsigned int    Reserved_8            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_FME_IMV_CNT;

/* Define the union U_FME_FME_STA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fme_lcu_oper_cnt      : 16  ; /* [15..0]  */
        unsigned int    fme_lcu_oper_state    : 1   ; /* [16]  */
        unsigned int    Reserved_11           : 3   ; /* [19..17]  */
        unsigned int    fme_pu_ctrl_state     : 2   ; /* [21..20]  */
        unsigned int    fme_mv_ctrl_state     : 2   ; /* [23..22]  */
        unsigned int    fme_lcu_lpw_en        : 1   ; /* [24]  */
        unsigned int    Reserved_10           : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_FME_FME_STA;

//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile U_FME_BIAS_COST0       FME_BIAS_COST0;
    volatile U_FME_BIAS_COST1       FME_BIAS_COST1;
    volatile unsigned int           RESERVED0[6];
    volatile unsigned int           FME_GTCK_CNT;
    volatile unsigned int           RESERVED1[3];
    volatile U_FME_PMV_CNT          FME_PMV_CNT;
    volatile U_FME_IMV_CNT          FME_IMV_CNT;
    volatile U_FME_FME_STA          FME_FME_STA;
    volatile unsigned int           RESERVED2[113];


} S_fme_REGS_TYPE;

/* Declare the struct pointor of the module fme */
extern volatile S_fme_REGS_TYPE *gopfmeAllReg;

/* Declare the functions that set the member value */



#endif /* __C_UNION_DEFINE_FME_H__ */
