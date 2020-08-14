//******************************************************************************
//  Copyright (C), 2007-2016, Hisilicon Technologies Co., Ltd.
//
//******************************************************************************
// File name     : c_union_define_dblk.h
// Version       : 2.0
// Author        : xxx
// Created       : 2016-02-04
// Last Modified : 
// Description   :  The C union definition file for the module dblk
// Function List : 
// History       : 
// 1 Date        : 
// Author        : xxx
// Modification  : Create file
//******************************************************************************

#ifndef __C_UNION_DEFINE_DBLK_H__
#define __C_UNION_DEFINE_DBLK_H__

/* Define the union U_DBLK_FRAME_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    frame_cnt             : 16  ; /* [15..0]  */
        unsigned int    Reserved_1            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DBLK_FRAME_CNT;

/* Define the union U_DBLK_LCU_CNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lcu_cnt               : 18  ; /* [17..0]  */
        unsigned int    Reserved_2            : 14  ; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DBLK_LCU_CNT;

/* Define the union U_DBLK_DFX_FLAG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    db_sao_cft_ls_c       : 1   ; /* [0]  */
        unsigned int    db_sao_cft_us_c       : 1   ; /* [1]  */
        unsigned int    db_sao_cft_ls         : 1   ; /* [2]  */
        unsigned int    db_sao_cft_us         : 1   ; /* [3]  */
        unsigned int    reserved0             : 6   ; /* [9..4]  */
        unsigned int    fsm_error_c           : 1   ; /* [10]  */
        unsigned int    fsm_error             : 1   ; /* [11]  */
        unsigned int    Reserved_3            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_DBLK_DFX_FLAG;

//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile unsigned int           RESERVED0[32];
    volatile U_DBLK_FRAME_CNT       DBLK_FRAME_CNT;
    volatile U_DBLK_LCU_CNT         DBLK_LCU_CNT;
    volatile U_DBLK_DFX_FLAG        DBLK_DFX_FLAG;
    volatile unsigned int           RESERVED1[93];


} S_dblk_REGS_TYPE;

/* Declare the struct pointor of the module dblk */
extern volatile S_dblk_REGS_TYPE *gopdblkAllReg;

/* Declare the functions that set the member value */



#endif /* __C_UNION_DEFINE_DBLK_H__ */
