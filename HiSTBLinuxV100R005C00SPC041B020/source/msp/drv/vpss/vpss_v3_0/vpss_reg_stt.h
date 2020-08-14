//******************************************************************************
//  Copyright (C), 2007-2014, Hisilicon Technologies Co., Ltd.
//
//******************************************************************************
// File name     : c_union_define.h
// Version       : 2.0
// Author        : x57490
// Created       : 2014-08-27
// Last Modified : 
// Description   :  The C union definition file for the module CAS
// Function List : 
// History       : 
// 1 Date        : 
// Author        : x57490
// Modification  : Create file
//******************************************************************************

#ifndef __VPSS_REG_STT_H__
#define __VPSS_REG_STT_H__

#include "./hi3798cv200_a/hi_reg_vpss_wbc.h"

#if 0
// Define the union U_STR_DET_VIDBLK0TOL0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    blk_pix0              : 8   ; // [7..0] 
        unsigned int    blk_pix1              : 8   ; // [15..8] 
        unsigned int    blk_pix2              : 8   ; // [23..16] 
        unsigned int    blk_pix3              : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_STR_DET_VIDBLK0TOL0;

// Define the union U_STR_DET_VIDBLK0TOL1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    blk_pix4              : 8   ; // [7..0] 
        unsigned int    blk_pix5              : 8   ; // [15..8] 
        unsigned int    blk_pix6              : 8   ; // [23..16] 
        unsigned int    blk_pix7              : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_STR_DET_VIDBLK0TOL1;

// Define the union U_STR_DET_VIDBLK1TOL0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    blk_pix0              : 8   ; // [7..0] 
        unsigned int    blk_pix1              : 8   ; // [15..8] 
        unsigned int    blk_pix2              : 8   ; // [23..16] 
        unsigned int    blk_pix3              : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_STR_DET_VIDBLK1TOL0;

// Define the union U_STR_DET_VIDBLK1TOL1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    blk_pix4              : 8   ; // [7..0] 
        unsigned int    blk_pix5              : 8   ; // [15..8] 
        unsigned int    blk_pix6              : 8   ; // [23..16] 
        unsigned int    blk_pix7              : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_STR_DET_VIDBLK1TOL1;

// Define the union U_STR_DET_VIDBLK2TOL0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    blk_pix0              : 8   ; // [7..0] 
        unsigned int    blk_pix1              : 8   ; // [15..8] 
        unsigned int    blk_pix2              : 8   ; // [23..16] 
        unsigned int    blk_pix3              : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_STR_DET_VIDBLK2TOL0;

// Define the union U_STR_DET_VIDBLK2TOL1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    blk_pix4              : 8   ; // [7..0] 
        unsigned int    blk_pix5              : 8   ; // [15..8] 
        unsigned int    blk_pix6              : 8   ; // [23..16] 
        unsigned int    blk_pix7              : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_STR_DET_VIDBLK2TOL1;

// Define the union U_STR_DET_VIDBLK3TOL0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    blk_pix0              : 8   ; // [7..0] 
        unsigned int    blk_pix1              : 8   ; // [15..8] 
        unsigned int    blk_pix2              : 8   ; // [23..16] 
        unsigned int    blk_pix3              : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_STR_DET_VIDBLK3TOL0;

// Define the union U_STR_DET_VIDBLK3TOL1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    blk_pix4              : 8   ; // [7..0] 
        unsigned int    blk_pix5              : 8   ; // [15..8] 
        unsigned int    blk_pix6              : 8   ; // [23..16] 
        unsigned int    blk_pix7              : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_STR_DET_VIDBLK3TOL1;

// Define the union U_STR_DET_VIDBLK4TOL0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    blk_pix0              : 8   ; // [7..0] 
        unsigned int    blk_pix1              : 8   ; // [15..8] 
        unsigned int    blk_pix2              : 8   ; // [23..16] 
        unsigned int    blk_pix3              : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_STR_DET_VIDBLK4TOL0;

// Define the union U_STR_DET_VIDBLK4TOL1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    blk_pix4              : 8   ; // [7..0] 
        unsigned int    blk_pix5              : 8   ; // [15..8] 
        unsigned int    blk_pix6              : 8   ; // [23..16] 
        unsigned int    blk_pix7              : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_STR_DET_VIDBLK4TOL1;

// Define the union U_STR_DET_VIDBLK5TOL0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    blk_pix0              : 8   ; // [7..0] 
        unsigned int    blk_pix1              : 8   ; // [15..8] 
        unsigned int    blk_pix2              : 8   ; // [23..16] 
        unsigned int    blk_pix3              : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_STR_DET_VIDBLK5TOL0;

// Define the union U_STR_DET_VIDBLK5TOL1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    blk_pix4              : 8   ; // [7..0] 
        unsigned int    blk_pix5              : 8   ; // [15..8] 
        unsigned int    blk_pix6              : 8   ; // [23..16] 
        unsigned int    blk_pix7              : 8   ; // [31..24] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_STR_DET_VIDBLK5TOL1;

// Define the union U_DB_HY_SIZE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hy_size               : 7   ; // [6..0] 
        unsigned int    Reserved_7            : 9   ; // [15..7] 
        unsigned int    hy_counter            : 12  ; // [27..16] 
        unsigned int    Reserved_6            : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DB_HY_SIZE;

// Define the union U_DB_HC_SIZE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hc_size               : 7   ; // [6..0] 
        unsigned int    Reserved_10           : 9   ; // [15..7] 
        unsigned int    hc_counter            : 12  ; // [27..16] 
        unsigned int    Reserved_9            : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DB_HC_SIZE;

// Define the union U_DB_VY_SIZE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vy_size               : 7   ; // [6..0] 
        unsigned int    Reserved_12           : 9   ; // [15..7] 
        unsigned int    vy_counter            : 12  ; // [27..16] 
        unsigned int    Reserved_11           : 4   ; // [31..28] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DB_VY_SIZE;

//==============================================================================
// Define the global struct
typedef struct
{
    volatile unsigned int           VPSS_PDFRMITDIFF;
    volatile unsigned int           VPSS_PDSTLBLKSAD[16];
    volatile unsigned int           VPSS_PDHISTBIN[4];
    volatile unsigned int           VPSS_PDUMMATCH0;
    volatile unsigned int           VPSS_PDUMNOMATCH0;
    volatile unsigned int           VPSS_PDUMMATCH1;
    volatile unsigned int           VPSS_PDUMNOMATCH1;
    volatile unsigned int           VPSS_PDPCCFFWD0;
    volatile unsigned int           VPSS_PDPCCFWD0;
    volatile unsigned int           VPSS_PDPCCBWD0;
    volatile unsigned int           VPSS_PDPCCCRSS0;
    volatile unsigned int           VPSS_PDPCCFWDTKR0;
    volatile unsigned int           VPSS_PDPCCBWDTKR0;
    volatile unsigned int           VPSS_PDPCCFFWD1;
    volatile unsigned int           VPSS_PDPCCFWD1;
    volatile unsigned int           VPSS_PDPCCBWD1;
    volatile unsigned int           VPSS_PDPCCCRSS1;
    volatile unsigned int           VPSS_PDPCCFWDTKR1;
    volatile unsigned int           VPSS_PDPCCBWDTKR1;
    volatile unsigned int           VPSS_PDLASICNT140;
    volatile unsigned int           VPSS_PDLASICNT320;
    volatile unsigned int           VPSS_PDLASICNT340;
    volatile unsigned int           VPSS_PDLASICNT141;
    volatile unsigned int           VPSS_PDLASICNT321;
    volatile unsigned int           VPSS_PDLASICNT341;
    volatile unsigned int           VPSS_PDICHD;
    volatile unsigned int           Reserved_2[20];
    volatile unsigned int           IGLB_MTN_BIN0;
    volatile unsigned int           IGLB_MTN_BIN1;
    volatile unsigned int           IGLB_MTN_BIN2;
    volatile unsigned int           IGLB_MTN_BIN3;
    volatile unsigned int           IGLB_MTN_BIN4;
    volatile unsigned int           IGLB_MTN_BIN5;
    volatile unsigned int           IGLB_MTN_BIN6;
    volatile unsigned int           IGLB_MTN_BIN7;
    volatile unsigned int           IGLB_MTN_BIN8;
    volatile unsigned int           IGLB_MTN_BIN9;
    volatile unsigned int           IGLB_MTN_BIN10;
    volatile unsigned int           IGLB_MTN_BIN11;
    volatile unsigned int           IGLB_MTN_BIN12;
    volatile unsigned int           IGLB_MTN_BIN13;
    volatile unsigned int           IGLB_MTN_BIN14;
    volatile unsigned int           IGLB_MTN_BIN15;
    volatile unsigned int           IGLB_MTN_BIN16;
    volatile unsigned int           IGLB_MTN_BIN17;
    volatile unsigned int           IGLB_MTN_BIN18;
    volatile unsigned int           IGLB_MTN_BIN19;
    volatile unsigned int           IGLB_MTN_BIN20;
    volatile unsigned int           IGLB_MTN_BIN21;
    volatile unsigned int           IGLB_MTN_BIN22;
    volatile unsigned int           IGLB_MTN_BIN23;
    volatile unsigned int           IGLB_MTN_BIN24;
    volatile unsigned int           IGLB_MTN_BIN25;
    volatile unsigned int           IGLB_MTN_BIN26;
    volatile unsigned int           IGLB_MTN_BIN27;
    volatile unsigned int           IGLB_MTN_BIN28;
    volatile unsigned int           IGLB_MTN_BIN29;
    volatile unsigned int           IGLB_MTN_BIN30;
    volatile unsigned int           IGLB_MTN_BIN31;
    volatile unsigned int           IGLB_MTN_NUM0;
    volatile unsigned int           IGLB_MTN_SUM0;
    volatile unsigned int           IGLB_MTN_NUM1;
    volatile unsigned int           IGLB_MTN_SUM1;
    volatile unsigned int           IGLB_MTN_NUM2;
    volatile unsigned int           IGLB_MTN_SUM2;
    volatile unsigned int           IGLB_MTN_NUM3;
    volatile unsigned int           IGLB_MTN_SUM3;
    volatile unsigned int           IGLB_MTN_NUM4;
    volatile unsigned int           IGLB_MTN_SUM4;
    volatile unsigned int           Reserved_3[22];
    volatile U_STR_DET_VIDBLK0TOL0   STR_DET_VIDBLK0TOL0;
    volatile U_STR_DET_VIDBLK0TOL1   STR_DET_VIDBLK0TOL1;
    volatile U_STR_DET_VIDBLK1TOL0   STR_DET_VIDBLK1TOL0;
    volatile U_STR_DET_VIDBLK1TOL1   STR_DET_VIDBLK1TOL1;
    volatile U_STR_DET_VIDBLK2TOL0   STR_DET_VIDBLK2TOL0;
    volatile U_STR_DET_VIDBLK2TOL1   STR_DET_VIDBLK2TOL1;
    volatile U_STR_DET_VIDBLK3TOL0   STR_DET_VIDBLK3TOL0;
    volatile U_STR_DET_VIDBLK3TOL1   STR_DET_VIDBLK3TOL1;
    volatile U_STR_DET_VIDBLK4TOL0   STR_DET_VIDBLK4TOL0;
    volatile U_STR_DET_VIDBLK4TOL1   STR_DET_VIDBLK4TOL1;
    volatile U_STR_DET_VIDBLK5TOL0   STR_DET_VIDBLK5TOL0;
    volatile U_STR_DET_VIDBLK5TOL1   STR_DET_VIDBLK5TOL1;
    volatile unsigned int           Reserved_4[52];
    volatile unsigned int           DB_HY_COUNTER[1920];
//    volatile unsigned int           Reserved_5[1919];
    volatile unsigned int           DB_BORDER[128];
//    volatile unsigned int           Reserved_8[127];
    volatile U_DB_HY_SIZE           DB_HY_SIZE;
    volatile U_DB_HC_SIZE           DB_HC_SIZE;
    volatile U_DB_VY_SIZE           DB_VY_SIZE;


} S_STT_REGS_TYPE;

// Declare the struct pointor of the module CAS
//extern volatile S_STT_REGS_TYPE *gopCASAllReg;

// Declare the functions that set the member value


#endif

#endif // __VPSS_REG_STT_H__