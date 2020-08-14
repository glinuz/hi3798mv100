//******************************************************************************
//  Copyright (C), 2007-2016, Hisilicon Technologies Co., Ltd.
//
//******************************************************************************
// File name     : c_union_define_env.h
// Version       : 2.0
// Author        : xxx
// Created       : 2016-02-04
// Last Modified : 
// Description   :  The C union definition file for the module env
// Function List : 
// History       : 
// 1 Date        : 
// Author        : xxx
// Modification  : Create file
//******************************************************************************

#ifndef __C_UNION_DEFINE_ENV_H__
#define __C_UNION_DEFINE_ENV_H__

/* Define the union U_ENV_CHN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    env_chn_curr_id       : 4   ; /* [3..0]  */
        unsigned int    env_chn_goptype       : 4   ; /* [7..4]  */
        unsigned int    env_chn_bfrm_num      : 8   ; /* [15..8]  */
        unsigned int    Reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ENV_CHN;

//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile U_ENV_CHN              ENV_CHN;
    volatile unsigned int           RESERVED0[12287];


} S_env_REGS_TYPE;

/* Declare the struct pointor of the module env */
extern volatile S_env_REGS_TYPE *gopenvAllReg;

/* Declare the functions that set the member value */
int iSetENV_CHNenv_chn_curr_id(unsigned int uenv_chn_curr_id);
int iSetENV_CHNenv_chn_goptype(unsigned int uenv_chn_goptype);
int iSetENV_CHNenv_chn_bfrm_num(unsigned int uenv_chn_bfrm_num);



#endif /* __C_UNION_DEFINE_ENV_H__ */
