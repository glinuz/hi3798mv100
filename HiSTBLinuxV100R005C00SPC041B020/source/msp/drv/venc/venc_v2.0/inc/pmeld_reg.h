//******************************************************************************
//  Copyright (C), 2007-2016, Hisilicon Technologies Co., Ltd.
//
//******************************************************************************
// File name     : c_union_define_pmeld.h
// Version       : 2.0
// Author        : xxx
// Created       : 2016-02-04
// Last Modified : 
// Description   :  The C union definition file for the module pmeld
// Function List : 
// History       : 
// 1 Date        : 
// Author        : xxx
// Modification  : Create file
//******************************************************************************

#ifndef __C_UNION_DEFINE_PMELD_H__
#define __C_UNION_DEFINE_PMELD_H__

//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile unsigned int           RESERVED0[128];


} S_pmeld_REGS_TYPE;

/* Declare the struct pointor of the module pmeld */
extern volatile S_pmeld_REGS_TYPE *goppmeldAllReg;

/* Declare the functions that set the member value */



#endif /* __C_UNION_DEFINE_PMELD_H__ */
