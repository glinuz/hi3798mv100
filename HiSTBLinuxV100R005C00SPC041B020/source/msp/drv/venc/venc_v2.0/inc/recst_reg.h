//******************************************************************************
//  Copyright (C), 2007-2016, Hisilicon Technologies Co., Ltd.
//
//******************************************************************************
// File name     : c_union_define_recst.h
// Version       : 2.0
// Author        : xxx
// Created       : 2016-02-04
// Last Modified : 
// Description   :  The C union definition file for the module recst
// Function List : 
// History       : 
// 1 Date        : 
// Author        : xxx
// Modification  : Create file
//******************************************************************************

#ifndef __C_UNION_DEFINE_RECST_H__
#define __C_UNION_DEFINE_RECST_H__

//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile unsigned int           RESERVED0[128];


} S_recst_REGS_TYPE;

/* Declare the struct pointor of the module recst */
extern volatile S_recst_REGS_TYPE *goprecstAllReg;

/* Declare the functions that set the member value */



#endif /* __C_UNION_DEFINE_RECST_H__ */
