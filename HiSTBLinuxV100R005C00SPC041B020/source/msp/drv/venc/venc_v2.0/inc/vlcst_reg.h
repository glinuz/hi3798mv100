//******************************************************************************
//  Copyright (C), 2007-2016, Hisilicon Technologies Co., Ltd.
//
//******************************************************************************
// File name     : c_union_define_vlcst.h
// Version       : 2.0
// Author        : xxx
// Created       : 2016-02-04
// Last Modified : 
// Description   :  The C union definition file for the module vlcst
// Function List : 
// History       : 
// 1 Date        : 
// Author        : xxx
// Modification  : Create file
//******************************************************************************

#ifndef __C_UNION_DEFINE_VLCST_H__
#define __C_UNION_DEFINE_VLCST_H__

/* Define the union U_VLCST_PTBITS_EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vlcst_ptbits_en       : 1   ; /* [0]  */
        unsigned int    Reserved_2            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VLCST_PTBITS_EN;

/* Define the union U_VLCST_DESCRIPTOR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vlcst_idrind          : 1   ; /* [0]  */
        unsigned int    Reserved_4            : 15  ; /* [15..1]  */
        unsigned int    vlcst_chnid           : 8   ; /* [23..16]  */
        unsigned int    Reserved_3            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VLCST_DESCRIPTOR;

//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile unsigned int           RESERVED0;
    volatile unsigned int           VLCST_STRMBUFLEN;
    volatile unsigned int           RESERVED1[2];
    volatile U_VLCST_PTBITS_EN      VLCST_PTBITS_EN;
    volatile unsigned int           VLCST_PTBITS;
    volatile U_VLCST_DESCRIPTOR     VLCST_DESCRIPTOR;
    volatile unsigned int           VLCST_SLC_CFG0;
    volatile unsigned int           VLCST_SLC_CFG1;
    volatile unsigned int           VLCST_SLC_CFG2;
    volatile unsigned int           VLCST_SLC_CFG3;
    volatile unsigned int           RESERVED2[21];
    volatile unsigned int           VLCST_SLC_LEN_CNT;
    volatile unsigned int           VLCST_WPTR;
    volatile unsigned int           VLCST_RPTR;
    volatile unsigned int           RESERVED3[93];


} S_vlcst_REGS_TYPE;

/* Declare the struct pointor of the module vlcst */
extern volatile S_vlcst_REGS_TYPE *gopvlcstAllReg;

/* Declare the functions that set the member value */
int iSetVLCST_STRMBUFLENvlcst_strmbuflen(unsigned int uvlcst_strmbuflen);
int iSetVLCST_PTBITS_ENvlcst_ptbits_en(unsigned int uvlcst_ptbits_en);
int iSetVLCST_PTBITSvlcst_ptbits(unsigned int uvlcst_ptbits);
int iSetVLCST_DESCRIPTORvlcst_idrind(unsigned int uvlcst_idrind);
int iSetVLCST_DESCRIPTORvlcst_chnid(unsigned int uvlcst_chnid);
int iSetVLCST_SLC_CFG0vlcst_slc_cfg0(unsigned int uvlcst_slc_cfg0);
int iSetVLCST_SLC_CFG1vlcst_slc_cfg1(unsigned int uvlcst_slc_cfg1);
int iSetVLCST_SLC_CFG2vlcst_slc_cfg2(unsigned int uvlcst_slc_cfg2);
int iSetVLCST_SLC_CFG3vlcst_slc_cfg3(unsigned int uvlcst_slc_cfg3);



#endif /* __C_UNION_DEFINE_VLCST_H__ */
