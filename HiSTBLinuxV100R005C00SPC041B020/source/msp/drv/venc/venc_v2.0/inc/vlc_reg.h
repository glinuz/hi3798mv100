//******************************************************************************
//  Copyright (C), 2007-2016, Hisilicon Technologies Co., Ltd.
//
//******************************************************************************
// File name     : c_union_define_vlc.h
// Version       : 2.0
// Author        : xxx
// Created       : 2016-02-04
// Last Modified : 
// Description   :  The C union definition file for the module vlc
// Function List : 
// History       : 
// 1 Date        : 
// Author        : xxx
// Modification  : Create file
//******************************************************************************

#ifndef __C_UNION_DEFINE_VLC_H__
#define __C_UNION_DEFINE_VLC_H__

/* Define the union U_VLC_SLCHDRPARA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vlc_markingbit        : 6   ; /* [5..0]  */
        unsigned int    reserved1             : 2   ; /* [7..6]  */
        unsigned int    vlc_reorderbit        : 6   ; /* [13..8]  */
        unsigned int    reserved0             : 2   ; /* [15..14]  */
        unsigned int    vlc_parabit           : 7   ; /* [22..16]  */
        unsigned int    Reserved_1            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VLC_SLCHDRPARA;

/* Define the union U_VLC_SVC */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vlc_svc_en            : 1   ; /* [0]  */
        unsigned int    vlc_svc_strm          : 24  ; /* [24..1]  */
        unsigned int    Reserved_2            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VLC_SVC;

/* Define the union U_VLC_SLCHDRPARA_I */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vlc_markingbit_i      : 6   ; /* [5..0]  */
        unsigned int    reserved1             : 2   ; /* [7..6]  */
        unsigned int    vlc_reorderbit_i      : 6   ; /* [13..8]  */
        unsigned int    reserved0             : 2   ; /* [15..14]  */
        unsigned int    vlc_parabit_i         : 7   ; /* [22..16]  */
        unsigned int    Reserved_3            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VLC_SLCHDRPARA_I;

/* Define the union U_VLC_CABAC_BIT_OVER_FLOW */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vlc_cabac_bitout_overflow  : 25  ; /* [24..0]  */
        unsigned int    Reserved_5            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VLC_CABAC_BIT_OVER_FLOW;

//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile unsigned int           RESERVED0;
    volatile unsigned int           VLC_SLCHDRSTRM0;
    volatile unsigned int           VLC_SLCHDRSTRM1;
    volatile unsigned int           VLC_SLCHDRSTRM2;
    volatile unsigned int           VLC_SLCHDRSTRM3;
    volatile unsigned int           VLC_REORDERSTRM0;
    volatile unsigned int           VLC_REORDERSTRM1;
    volatile unsigned int           VLC_MARKINGSTRM0;
    volatile unsigned int           VLC_MARKINGSTRM1;
    volatile U_VLC_SLCHDRPARA       VLC_SLCHDRPARA;
    volatile U_VLC_SVC              VLC_SVC;
    volatile unsigned int           VLC_SLCHDRSTRM0_I;
    volatile unsigned int           VLC_SLCHDRSTRM1_I;
    volatile unsigned int           VLC_SLCHDRSTRM2_I;
    volatile unsigned int           VLC_SLCHDRSTRM3_I;
    volatile unsigned int           VLC_REORDERSTRM0_I;
    volatile unsigned int           VLC_REORDERSTRM1_I;
    volatile unsigned int           VLC_MARKINGSTRM0_I;
    volatile unsigned int           VLC_MARKINGSTRM1_I;
    volatile U_VLC_SLCHDRPARA_I     VLC_SLCHDRPARA_I;
    volatile unsigned int           RESERVED1[12];
    volatile U_VLC_CABAC_BIT_OVER_FLOW   VLC_CABAC_BIT_OVER_FLOW;
    volatile unsigned int           VLC_PIC_TOTAL_BITS;
    volatile unsigned int           VLC_CABAC_PIC_HRD_BITS;
    volatile unsigned int           VLC_CABAC_PIC_RES_BITS;
    volatile unsigned int           RESERVED2[92];


} S_vlc_REGS_TYPE;

/* Declare the struct pointor of the module vlc */
extern volatile S_vlc_REGS_TYPE *gopvlcAllReg;

/* Declare the functions that set the member value */
int iSetVLC_SLCHDRSTRM0vlc_slchdrstrm0(unsigned int uvlc_slchdrstrm0);
int iSetVLC_SLCHDRSTRM1vlc_slchdrstrm1(unsigned int uvlc_slchdrstrm1);
int iSetVLC_SLCHDRSTRM2vlc_slchdrstrm2(unsigned int uvlc_slchdrstrm2);
int iSetVLC_SLCHDRSTRM3vlc_slchdrstrm3(unsigned int uvlc_slchdrstrm3);
int iSetVLC_REORDERSTRM0vlc_reorderstrm0(unsigned int uvlc_reorderstrm0);
int iSetVLC_REORDERSTRM1vlc_reorderstrm1(unsigned int uvlc_reorderstrm1);
int iSetVLC_MARKINGSTRM0vlc_markingstrm0(unsigned int uvlc_markingstrm0);
int iSetVLC_MARKINGSTRM1vlc_markingstrm1(unsigned int uvlc_markingstrm1);
int iSetVLC_SLCHDRPARAvlc_markingbit(unsigned int uvlc_markingbit);
int iSetVLC_SLCHDRPARAvlc_reorderbit(unsigned int uvlc_reorderbit);
int iSetVLC_SLCHDRPARAvlc_parabit(unsigned int uvlc_parabit);
int iSetVLC_SVCvlc_svc_en(unsigned int uvlc_svc_en);
int iSetVLC_SVCvlc_svc_strm(unsigned int uvlc_svc_strm);
int iSetVLC_SLCHDRSTRM0_Ivlc_slchdrstrm0_i(unsigned int uvlc_slchdrstrm0_i);
int iSetVLC_SLCHDRSTRM1_Ivlc_slchdrstrm1_i(unsigned int uvlc_slchdrstrm1_i);
int iSetVLC_SLCHDRSTRM2_Ivlc_slchdrstrm2_i(unsigned int uvlc_slchdrstrm2_i);
int iSetVLC_SLCHDRSTRM3_Ivlc_slchdrstrm3_i(unsigned int uvlc_slchdrstrm3_i);
int iSetVLC_REORDERSTRM0_Ivlc_reorderstrm0_i(unsigned int uvlc_reorderstrm0_i);
int iSetVLC_REORDERSTRM1_Ivlc_reorderstrm1_i(unsigned int uvlc_reorderstrm1_i);
int iSetVLC_MARKINGSTRM0_Ivlc_markingstrm0_i(unsigned int uvlc_markingstrm0_i);
int iSetVLC_MARKINGSTRM1_Ivlc_markingstrm1_i(unsigned int uvlc_markingstrm1_i);
int iSetVLC_SLCHDRPARA_Ivlc_markingbit_i(unsigned int uvlc_markingbit_i);
int iSetVLC_SLCHDRPARA_Ivlc_reorderbit_i(unsigned int uvlc_reorderbit_i);
int iSetVLC_SLCHDRPARA_Ivlc_parabit_i(unsigned int uvlc_parabit_i);



#endif /* __C_UNION_DEFINE_VLC_H__ */
