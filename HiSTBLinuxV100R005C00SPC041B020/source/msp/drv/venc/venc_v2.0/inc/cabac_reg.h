//******************************************************************************
//  Copyright (C), 2007-2016, Hisilicon Technologies Co., Ltd.
//
//******************************************************************************
// File name     : c_union_define_cabac.h
// Version       : 2.0
// Author        : xxx
// Created       : 2016-02-04
// Last Modified : 
// Description   :  The C union definition file for the module cabac
// Function List : 
// History       : 
// 1 Date        : 
// Author        : xxx
// Modification  : Create file
//******************************************************************************

#ifndef __C_UNION_DEFINE_CABAC_H__
#define __C_UNION_DEFINE_CABAC_H__

/* Define the union U_CABAC_GLB_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    Reserved_1            : 8   ; /* [7..0]  */
        unsigned int    cabac_max_num_mergecand  : 3   ; /* [10..8]  */
        unsigned int    Reserved_0            : 5   ; /* [15..11]  */
        unsigned int    cabac_nal_unit_head   : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CABAC_GLB_CFG;

/* Define the union U_CABAC_SLCHDR_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cabac_slchdr_size_part1  : 5   ; /* [4..0]  */
        unsigned int    Reserved_2            : 11  ; /* [15..5]  */
        unsigned int    cabac_slchdr_size_part2  : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CABAC_SLCHDR_SIZE;

/* Define the union U_CABAC_SLCHDR_PART1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cabac_slchdr_part1    : 16  ; /* [15..0]  */
        unsigned int    Reserved_3            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CABAC_SLCHDR_PART1;

/* Define the union U_CABAC_SLCHDR_SIZE_I */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cabac_slchdr_size_part1_i  : 5   ; /* [4..0]  */
        unsigned int    Reserved_4            : 11  ; /* [15..5]  */
        unsigned int    cabac_slchdr_size_part2_i  : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CABAC_SLCHDR_SIZE_I;

/* Define the union U_CABAC_SLCHDR_PART1_I */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cabac_slchdr_part1_i  : 16  ; /* [15..0]  */
        unsigned int    Reserved_5            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CABAC_SLCHDR_PART1_I;

//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile U_CABAC_GLB_CFG        CABAC_GLB_CFG;
    volatile U_CABAC_SLCHDR_SIZE    CABAC_SLCHDR_SIZE;
    volatile U_CABAC_SLCHDR_PART1   CABAC_SLCHDR_PART1;
    volatile unsigned int           CABAC_SLCHDR_PART2_SEG1;
    volatile unsigned int           CABAC_SLCHDR_PART2_SEG2;
    volatile unsigned int           CABAC_SLCHDR_PART2_SEG3;
    volatile unsigned int           CABAC_SLCHDR_PART2_SEG4;
    volatile unsigned int           CABAC_SLCHDR_PART2_SEG5;
    volatile unsigned int           CABAC_SLCHDR_PART2_SEG6;
    volatile unsigned int           CABAC_SLCHDR_PART2_SEG7;
    volatile unsigned int           CABAC_SLCHDR_PART2_SEG8;
    volatile U_CABAC_SLCHDR_SIZE_I   CABAC_SLCHDR_SIZE_I;
    volatile U_CABAC_SLCHDR_PART1_I   CABAC_SLCHDR_PART1_I;
    volatile unsigned int           CABAC_SLCHDR_PART2_SEG1_I;
    volatile unsigned int           CABAC_SLCHDR_PART2_SEG2_I;
    volatile unsigned int           CABAC_SLCHDR_PART2_SEG3_I;
    volatile unsigned int           CABAC_SLCHDR_PART2_SEG4_I;
    volatile unsigned int           CABAC_SLCHDR_PART2_SEG5_I;
    volatile unsigned int           CABAC_SLCHDR_PART2_SEG6_I;
    volatile unsigned int           CABAC_SLCHDR_PART2_SEG7_I;
    volatile unsigned int           CABAC_SLCHDR_PART2_SEG8_I;
    volatile unsigned int           RESERVED0[11];
    volatile unsigned int           CABAC_PIC_STRMSIZE;
    volatile unsigned int           CABAC_PIC_BIN_NUM;
    volatile unsigned int           CABAC_PIC_HDRBIN_NUM;
    volatile unsigned int           CABAC_PIC_RESBIN_NUM;
    volatile unsigned int           RESERVED1[92];


} S_cabac_REGS_TYPE;

/* Declare the struct pointor of the module cabac */
extern volatile S_cabac_REGS_TYPE *gopcabacAllReg;

/* Declare the functions that set the member value */
int iSetCABAC_GLB_CFGcabac_max_num_mergecand(unsigned int ucabac_max_num_mergecand);
int iSetCABAC_GLB_CFGcabac_nal_unit_head(unsigned int ucabac_nal_unit_head);
int iSetCABAC_SLCHDR_SIZEcabac_slchdr_size_part1(unsigned int ucabac_slchdr_size_part1);
int iSetCABAC_SLCHDR_SIZEcabac_slchdr_size_part2(unsigned int ucabac_slchdr_size_part2);
int iSetCABAC_SLCHDR_PART1cabac_slchdr_part1(unsigned int ucabac_slchdr_part1);
int iSetCABAC_SLCHDR_PART2_SEG1cabac_slchdr_part2_seg1(unsigned int ucabac_slchdr_part2_seg1);
int iSetCABAC_SLCHDR_PART2_SEG2cabac_slchdr_part2_seg2(unsigned int ucabac_slchdr_part2_seg2);
int iSetCABAC_SLCHDR_PART2_SEG3cabac_slchdr_part2_seg3(unsigned int ucabac_slchdr_part2_seg3);
int iSetCABAC_SLCHDR_PART2_SEG4cabac_slchdr_part2_seg4(unsigned int ucabac_slchdr_part2_seg4);
int iSetCABAC_SLCHDR_PART2_SEG5cabac_slchdr_part2_seg5(unsigned int ucabac_slchdr_part2_seg5);
int iSetCABAC_SLCHDR_PART2_SEG6cabac_slchdr_part2_seg6(unsigned int ucabac_slchdr_part2_seg6);
int iSetCABAC_SLCHDR_PART2_SEG7cabac_slchdr_part2_seg7(unsigned int ucabac_slchdr_part2_seg7);
int iSetCABAC_SLCHDR_PART2_SEG8cabac_slchdr_part2_seg8(unsigned int ucabac_slchdr_part2_seg8);
int iSetCABAC_SLCHDR_SIZE_Icabac_slchdr_size_part1_i(unsigned int ucabac_slchdr_size_part1_i);
int iSetCABAC_SLCHDR_SIZE_Icabac_slchdr_size_part2_i(unsigned int ucabac_slchdr_size_part2_i);
int iSetCABAC_SLCHDR_PART1_Icabac_slchdr_part1_i(unsigned int ucabac_slchdr_part1_i);
int iSetCABAC_SLCHDR_PART2_SEG1_Icabac_slchdr_part2_seg1_i(unsigned int ucabac_slchdr_part2_seg1_i);
int iSetCABAC_SLCHDR_PART2_SEG2_Icabac_slchdr_part2_seg2_i(unsigned int ucabac_slchdr_part2_seg2_i);
int iSetCABAC_SLCHDR_PART2_SEG3_Icabac_slchdr_part2_seg3_i(unsigned int ucabac_slchdr_part2_seg3_i);
int iSetCABAC_SLCHDR_PART2_SEG4_Icabac_slchdr_part2_seg4_i(unsigned int ucabac_slchdr_part2_seg4_i);
int iSetCABAC_SLCHDR_PART2_SEG5_Icabac_slchdr_part2_seg5_i(unsigned int ucabac_slchdr_part2_seg5_i);
int iSetCABAC_SLCHDR_PART2_SEG6_Icabac_slchdr_part2_seg6_i(unsigned int ucabac_slchdr_part2_seg6_i);
int iSetCABAC_SLCHDR_PART2_SEG7_Icabac_slchdr_part2_seg7_i(unsigned int ucabac_slchdr_part2_seg7_i);
int iSetCABAC_SLCHDR_PART2_SEG8_Icabac_slchdr_part2_seg8_i(unsigned int ucabac_slchdr_part2_seg8_i);



#endif /* __C_UNION_DEFINE_CABAC_H__ */
