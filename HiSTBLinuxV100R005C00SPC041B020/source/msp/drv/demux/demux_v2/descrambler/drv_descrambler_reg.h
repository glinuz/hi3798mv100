/******************************************************************************
 Copyright (C), 2009-2014, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
 File Name     : drv_descrambler_reg.h
 Version       : Initial Draft
 Author        : Hisilicon multimedia software group
 Created       : 2013/04/16
 Description   :
******************************************************************************/

#ifndef __DRV_DESCRAMBLER_REG_H__
#define __DRV_DESCRAMBLER_REG_H__

#include "hi_type.h"

#define DMX_CW_CTRL0                    (0x3B70)    /* cw order change reg0 */
#define DMX_CW_CTRL2                    (0x3B78)    /* cw order change reg2 */

#ifdef DMX_SECURE_KEY_SUPPORT
/*
 * cpu worked in secure mode if no tee deployed.
 * secure cpu can only operate secure cw registers.
 * non secure cpu can only operate non secure cw registers.
 */
#ifdef DMX_TEE_SUPPORT
#define CW_SET                          (0x3B80)    /* CW configuration control register */
#define DMX_CW_DATA                     (0x3B84)    /* CW configuration register */
#else
#define CW_SECURE_FLAG                  (0x3E10)   /* secure CW set flag */
#define CW_SET                          (0x3E00)    /* secure CW configuration control register */
#define DMX_CW_DATA                     (0x3E04)    /* secure CW configuration register */
#endif

#else

#define CW_SET                          (0x3B80)    /* CW configuration control register */
#define DMX_CW_DATA                     (0x3B84)    /* CW configuration register */

#endif

#define CA_INFO0                        (0x3B90)    /* hardware OTP CA set [31:0] */

typedef union
{
    struct
    {
        HI_U32  hardonly_csa2       : 1;    // [0]
        HI_U32  hardonly_spe        : 1;    // [1]
        HI_U32  hardonly_des        : 1;    // [2]
        HI_U32  hardonly_novel      : 1;    // [3]
        HI_U32  hardonly_csa3       : 1;    // [4]
        HI_U32  hardonly_others     : 1;    // [5]
        HI_U32  hardonly_aes_cbc    : 1;    // [6] 
        HI_U32  reserved0           : 1;    // [7]
        HI_U32  dis_csa2            : 1;    // [8]
        HI_U32  dis_spe             : 1;    // [9]
        HI_U32  dis_des             : 1;    // [10]
        HI_U32  dis_novel           : 1;    // [11]
        HI_U32  dis_csa3            : 1;    // [12]
        HI_U32  dis_others          : 1;    // [13]
        HI_U32  dis_tdes            : 1;    // [14]
        HI_U32  reserved2           : 15;   // [29:15]
        HI_U32  ca_tsout_dis        : 1;    // [30]
        HI_U32  cainfo_dbg_dis      : 1;    // [31]
    } bits;

    HI_U32 value;
} U_CA_INFO0;

#define DMX_CA_DBG_0                    (0x3B94)    /* hardware OTP CA set [63:32] */
#define CA_ENTROPY                      (0x3BA8)    /* reserved register. Entropy decrease register */
#define DMX_CW_SET(DmxId)               (0x3BC0 + ((DmxId) << 2))       /* Set demux(x)'s CW table id */
#define CHAN_CW_TAB_ID(ChanId)          (0x3BE0 + (ChanId) / 16 * 4)    /* Set channel's CW table id, 2bit for each channel */

#define DMX_CW_VALUE(Idx, flag, half)   (0x3E00+16*(Idx) + 8 *(flag) +4*(half))  /* soft-way CW word set register */

#endif  // __DRV_DESCRAMBLER_REG_H__


