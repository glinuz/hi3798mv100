//******************************************************************************
//  Copyright (C), 2007-2016, Hisilicon Technologies Co., Ltd.
//
//******************************************************************************
// File name     : c_union_define_nbi.h
// Version       : 2.0
// Author        : xxx
// Created       : 2016-02-04
// Last Modified : 
// Description   :  The C union definition file for the module nbi
// Function List : 
// History       : 
// 1 Date        : 
// Author        : xxx
// Modification  : Create file
//******************************************************************************

#ifndef __C_UNION_DEFINE_NBI_H__
#define __C_UNION_DEFINE_NBI_H__

/* Define the union U_NBI_STATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ld_fifo_empty         : 1   ; /* [0]  */
        unsigned int    ld_fifo_full          : 1   ; /* [1]  */
        unsigned int    ld_fsm_state          : 2   ; /* [3..2]  */
        unsigned int    st_fifo_empty         : 1   ; /* [4]  */
        unsigned int    st_fifo_full          : 1   ; /* [5]  */
        unsigned int    st_fsm_state          : 2   ; /* [7..6]  */
        unsigned int    tmv_wr_en             : 1   ; /* [8]  */
        unsigned int    tmv_rd_en             : 1   ; /* [9]  */
        unsigned int    Reserved_1            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_NBI_STATE;

/* Define the union U_NBI_STATE_UP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fsm_state_up          : 16  ; /* [15..0]  */
        unsigned int    Reserved_2            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_NBI_STATE_UP;

/* Define the union U_NBI_LCU_COUNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lcu_cnt               : 16  ; /* [15..0]  */
        unsigned int    Reserved_4            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_NBI_LCU_COUNT;

/* Define the union U_NBI_TILE_COUNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tile_cnt              : 16  ; /* [15..0]  */
        unsigned int    Reserved_5            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_NBI_TILE_COUNT;

/* Define the union U_NBI_FRAME_COUNT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    frame_cnt             : 16  ; /* [15..0]  */
        unsigned int    Reserved_6            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_NBI_FRAME_COUNT;

//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile unsigned int           RESERVED0[4];
    volatile U_NBI_STATE            NBI_STATE;
    volatile U_NBI_STATE_UP         NBI_STATE_UP;
    volatile unsigned int           RESERVED2[2];
    volatile U_NBI_LCU_COUNT        NBI_LCU_COUNT;
    volatile U_NBI_TILE_COUNT       NBI_TILE_COUNT;
    volatile U_NBI_FRAME_COUNT      NBI_FRAME_COUNT;
    volatile unsigned int           RESERVED1[117];


} S_nbi_REGS_TYPE;

/* Declare the struct pointor of the module nbi */
extern volatile S_nbi_REGS_TYPE *gopnbiAllReg;

/* Declare the functions that set the member value */



#endif /* __C_UNION_DEFINE_NBI_H__ */
