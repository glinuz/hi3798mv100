//******************************************************************************
//  Copyright (C), 2007-2016, Hisilicon Technologies Co., Ltd.
//
//******************************************************************************
// File name     : c_union_define_ime.h
// Version       : 2.0
// Author        : xxx
// Created       : 2016-02-04
// Last Modified : 
// Description   :  The C union definition file for the module ime
// Function List : 
// History       : 
// 1 Date        : 
// Author        : xxx
// Modification  : Create file
//******************************************************************************

#ifndef __C_UNION_DEFINE_IME_H__
#define __C_UNION_DEFINE_IME_H__

/* Define the union U_IME_INTER_MODE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    Reserved_2            : 1   ; /* [0]  */
        unsigned int    ime_inter8x8_en       : 1   ; /* [1]  */
        unsigned int    Reserved_1            : 6   ; /* [7..2]  */
        unsigned int    Reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_IME_INTER_MODE;

/* Define the union U_IME_RDOCFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ime_lambdaoff8        : 16  ; /* [15..0]  */
        unsigned int    ime_lambdaoff16       : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_IME_RDOCFG;

/* Define the union U_IME_FME_LPOW_THR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ime_lowpow_fme_thr0   : 6   ; /* [5..0]  */
        unsigned int    ime_lowpow_fme_thr1   : 6   ; /* [11..6]  */
        unsigned int    Reserved_4            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_IME_FME_LPOW_THR;

//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile U_IME_INTER_MODE       IME_INTER_MODE;
    volatile unsigned int           RESERVED0[7];
    volatile U_IME_RDOCFG           IME_RDOCFG;
    volatile U_IME_FME_LPOW_THR     IME_FME_LPOW_THR;
    volatile unsigned int           RESERVED1[118];


} S_ime_REGS_TYPE;

/* Declare the struct pointor of the module ime */
extern volatile S_ime_REGS_TYPE *gopimeAllReg;

/* Declare the functions that set the member value */
int iSetIME_INTER_MODEime_inter8x8_en(unsigned int uime_inter8x8_en);
int iSetIME_RDOCFGime_lambdaoff8(unsigned int uime_lambdaoff8);
int iSetIME_RDOCFGime_lambdaoff16(unsigned int uime_lambdaoff16);
int iSetIME_FME_LPOW_THRime_lowpow_fme_thr0(unsigned int uime_lowpow_fme_thr0);
int iSetIME_FME_LPOW_THRime_lowpow_fme_thr1(unsigned int uime_lowpow_fme_thr1);



#endif /* __C_UNION_DEFINE_IME_H__ */
