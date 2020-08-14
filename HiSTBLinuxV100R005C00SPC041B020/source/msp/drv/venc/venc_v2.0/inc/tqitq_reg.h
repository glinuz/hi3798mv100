//******************************************************************************
//  Copyright (C), 2007-2016, Hisilicon Technologies Co., Ltd.
//
//******************************************************************************
// File name     : c_union_define_tqitq.h
// Version       : 2.0
// Author        : xxx
// Created       : 2016-02-04
// Last Modified : 
// Description   :  The C union definition file for the module tqitq
// Function List : 
// History       : 
// 1 Date        : 
// Author        : xxx
// Modification  : Create file
//******************************************************************************

#ifndef __C_UNION_DEFINE_TQITQ_H__
#define __C_UNION_DEFINE_TQITQ_H__

/* Define the union U_TQITQ_DEADZONE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tqitq_deadzone_intra_slice  : 8   ; /* [7..0]  */
        unsigned int    tqitq_deadzone_inter_slice  : 8   ; /* [15..8]  */
        unsigned int    Reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_TQITQ_DEADZONE;

//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile U_TQITQ_DEADZONE       TQITQ_DEADZONE;
    volatile unsigned int           RESERVED0[3];
    volatile unsigned int           TQITQ_LCU_COUNT;
    volatile unsigned int           TQITQ_TU4GATE_ENCNT;
    volatile unsigned int           TQITQ_DST4GATE_ENCNT;
    volatile unsigned int           TQITQ_TU8GATE_ENCNT;
    volatile unsigned int           TQITQ_TU16GATE_ENCNT;
    volatile unsigned int           TQITQ_TU32GATE_ENCNT;
    volatile unsigned int           RESERVED1[246];


} S_tqitq_REGS_TYPE;

/* Declare the struct pointor of the module tqitq */
extern volatile S_tqitq_REGS_TYPE *goptqitqAllReg;

/* Declare the functions that set the member value */
int iSetTQITQ_DEADZONEtqitq_deadzone_intra_slice(unsigned int utqitq_deadzone_intra_slice);
int iSetTQITQ_DEADZONEtqitq_deadzone_inter_slice(unsigned int utqitq_deadzone_inter_slice);



#endif /* __C_UNION_DEFINE_TQITQ_H__ */
