//******************************************************************************
//  Copyright (C), 2007-2016, Hisilicon Technologies Co., Ltd.
//
//******************************************************************************
// File name     : c_union_define_pack.h
// Version       : 2.0
// Author        : xxx
// Created       : 2016-02-04
// Last Modified : 
// Description   :  The C union definition file for the module pack
// Function List : 
// History       : 
// 1 Date        : 
// Author        : xxx
// Modification  : Create file
//******************************************************************************

#ifndef __C_UNION_DEFINE_PACK_H__
#define __C_UNION_DEFINE_PACK_H__

//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile unsigned int           PACK_SYNTAX_CONFIG;
    volatile unsigned int           PACK_CU_PARAMETER;
    volatile unsigned int           PACK_PCM_PARAMETER;
    volatile unsigned int           PACK_TF_SKIP_FLAG;
    volatile unsigned int           RESERVED0[124];


} S_pack_REGS_TYPE;

/* Declare the struct pointor of the module pack */
extern volatile S_pack_REGS_TYPE *goppackAllReg;

/* Declare the functions that set the member value */



#endif /* __C_UNION_DEFINE_PACK_H__ */
