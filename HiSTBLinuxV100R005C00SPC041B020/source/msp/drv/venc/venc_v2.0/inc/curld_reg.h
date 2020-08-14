//******************************************************************************
//  Copyright (C), 2007-2016, Hisilicon Technologies Co., Ltd.
//
//******************************************************************************
// File name     : c_union_define_curld.h
// Version       : 2.0
// Author        : xxx
// Created       : 2016-02-04
// Last Modified : 
// Description   :  The C union definition file for the module curld
// Function List : 
// History       : 
// 1 Date        : 
// Author        : xxx
// Modification  : Create file
//******************************************************************************

#ifndef __C_UNION_DEFINE_CURLD_H__
#define __C_UNION_DEFINE_CURLD_H__

/* Define the union U_CURLD_GCFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    Reserved_5            : 8   ; /* [7..0]  */
        unsigned int    Reserved_4            : 6   ; /* [13..8]  */
        unsigned int    curld_col2gray_en     : 1   ; /* [14]  */
        unsigned int    curld_clip_en         : 1   ; /* [15]  */
        unsigned int    Reserved_3            : 4   ; /* [19..16]  */
        unsigned int    curld_read_interval   : 8   ; /* [27..20]  */
        unsigned int    curld_lowdly_en       : 1   ; /* [28]  */
        unsigned int    Reserved_2            : 1   ; /* [29]  */
        unsigned int    Reserved_1            : 1   ; /* [30]  */
        unsigned int    Reserved_0            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CURLD_GCFG;

/* Define the union U_CURLD_CLIP_THX */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    curld_clip_luma_min   : 8   ; /* [7..0]  */
        unsigned int    curld_clip_luma_max   : 8   ; /* [15..8]  */
        unsigned int    curld_clip_chrm_min   : 8   ; /* [23..16]  */
        unsigned int    curld_clip_chrm_max   : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CURLD_CLIP_THX;

/* Define the union U_CURLD_CMPLD_STATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    curld_sgmt_mode_err   : 1   ; /* [0]  */
        unsigned int    curld_de_cmp_wrong    : 1   ; /* [1]  */
        unsigned int    Reserved_9            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CURLD_CMPLD_STATE;

//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile U_CURLD_GCFG           CURLD_GCFG;
    volatile unsigned int           RESERVED0[22];
    volatile unsigned int           RESERVED1;
    volatile U_CURLD_CLIP_THX       CURLD_CLIP_THX;
    volatile unsigned int           RESERVED2[18];
    volatile U_CURLD_CMPLD_STATE    CURLD_CMPLD_STATE;
    volatile unsigned int           RESERVED3[84];


} S_curld_REGS_TYPE;

/* Declare the struct pointor of the module curld */
extern volatile S_curld_REGS_TYPE *gopcurldAllReg;

/* Declare the functions that set the member value */
int iSetCURLD_GCFGcurld_col2gray_en(unsigned int ucurld_col2gray_en);
int iSetCURLD_GCFGcurld_clip_en(unsigned int ucurld_clip_en);
int iSetCURLD_GCFGcurld_read_interval(unsigned int ucurld_read_interval);
int iSetCURLD_GCFGcurld_lowdly_en(unsigned int ucurld_lowdly_en);
int iSetCURLD_CLIP_THXcurld_clip_luma_min(unsigned int ucurld_clip_luma_min);
int iSetCURLD_CLIP_THXcurld_clip_luma_max(unsigned int ucurld_clip_luma_max);
int iSetCURLD_CLIP_THXcurld_clip_chrm_min(unsigned int ucurld_clip_chrm_min);
int iSetCURLD_CLIP_THXcurld_clip_chrm_max(unsigned int ucurld_clip_chrm_max);



#endif /* __C_UNION_DEFINE_CURLD_H__ */
