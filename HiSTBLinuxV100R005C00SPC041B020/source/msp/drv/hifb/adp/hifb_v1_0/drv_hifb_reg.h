/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name       : drv_hifb_reg.h
Version         : Initial Draft
Author          :
Created         : 2016/02/02
Description     :
Function List   :


History         :
Date                Author                Modification
2016/02/02            y00181162              Created file
******************************************************************************/
#ifndef __DRV_HIFB_REG_H__
#define __DRV_HIFB_REG_H__


/*********************************add include here******************************/
#include "drv_hifb_config.h"


#ifdef __cplusplus
#if __cplusplus
   extern "C" {
#endif
#endif /* __cplusplus */



/*****************************************************************************/

#ifdef CONFIG_HIFB_USE_COMM_REGH

#include "hi_reg_common.h"

#else
/***************************** Macro Definition ******************************/


/*************************** Structure Definition ****************************/
// Define the union U_VOCTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    m0_arb_mode           : 4   ; // [3..0]
        unsigned int    Reserved_0            : 25  ; // [28..4]
        unsigned int    g3_dhd1_sel           : 1   ; // [29]
        unsigned int    chk_sum_en            : 1   ; // [30]
        unsigned int    vo_ck_gt_en           : 1   ; // [31]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VOCTRL;

// Define the union U_VOINTSTA
typedef union
{
    // Define the struct bits
    struct
    {

        unsigned int    dhd0vtthd1_int        : 1   ; // [0]
        unsigned int    dhd0vtthd2_int        : 1   ; // [1]
        unsigned int    dhd0vtthd3_int        : 1   ; // [2]
        unsigned int    dhd0uf_int            : 1   ; // [3]
        unsigned int    dhd1vtthd1_int        : 1   ; // [4]
        unsigned int    dhd1vtthd2_int        : 1   ; // [5]
        unsigned int    dhd1vtthd3_int        : 1   ; // [6]
        unsigned int    dhd1uf_int            : 1   ; // [7]
        unsigned int    gwbc0_vte_int         : 1   ; // [8]
        unsigned int    dwbc0_vte_int         : 1   ; // [9]
        unsigned int    g0wbc_vte_int         : 1   ; // [10]
        unsigned int    g4wbc_vte_int         : 1   ; // [11]
        unsigned int    Reserved_2            : 4   ; // [15..12]
        unsigned int    v0rr_int              : 1   ; // [16]
        unsigned int    v1rr_int              : 1   ; // [17]
        unsigned int    v2rr_int              : 1   ; // [18]
        unsigned int    v3rr_int              : 1   ; // [19]
        unsigned int    v4rr_int              : 1   ; // [20]
        unsigned int    v5rr_int              : 1   ; // [21]
        unsigned int    g0rr_int              : 1   ; // [22]
        unsigned int    g1rr_int              : 1   ; // [23]
        unsigned int    g2rr_int              : 1   ; // [24]
        unsigned int    g3rr_int              : 1   ; // [25]
        unsigned int    g4rr_int              : 1   ; // [26]
        unsigned int    g5rr_int              : 1   ; // [27]
        unsigned int    wbcdhd_partfns_int    : 1   ; // [28]
        unsigned int    ut_end_int            : 1   ; // [29]
        unsigned int    m0_be_int             : 1   ; // [30]
        unsigned int    Reserved_1            : 1   ; // [31]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VOINTSTA;

// Define the union U_VOMSKINTSTA
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    dhd0vtthd1_clr        : 1   ; // [0]
        unsigned int    dhd0vtthd2_clr        : 1   ; // [1]
        unsigned int    dhd0vtthd3_clr        : 1   ; // [2]
        unsigned int    dhd0uf_clr            : 1   ; // [3]
        unsigned int    dhd1vtthd1_clr        : 1   ; // [4]
        unsigned int    dhd1vtthd2_clr        : 1   ; // [5]
        unsigned int    dhd1vtthd3_clr        : 1   ; // [6]
        unsigned int    dhd1uf_clr            : 1   ; // [7]
        unsigned int    gwbc0_vte_clr         : 1   ; // [8]
        unsigned int    dwbc0_vte_clr         : 1   ; // [9]
        unsigned int    g0wbc_vte_clr         : 1   ; // [10]
        unsigned int    g4wbc_vte_clr         : 1   ; // [11]
        unsigned int    Reserved_4            : 4   ; // [15..12]
        unsigned int    v0rr_clr              : 1   ; // [16]
        unsigned int    v1rr_clr              : 1   ; // [17]
        unsigned int    v2rr_clr              : 1   ; // [18]
        unsigned int    v3rr_clr              : 1   ; // [19]
        unsigned int    v4rr_clr              : 1   ; // [20]
        unsigned int    v5rr_clr              : 1   ; // [21]
        unsigned int    g0rr_clr              : 1   ; // [22]
        unsigned int    g1rr_clr              : 1   ; // [23]
        unsigned int    g2rr_clr              : 1   ; // [24]
        unsigned int    g3rr_clr              : 1   ; // [25]
        unsigned int    g4rr_clr              : 1   ; // [26]
        unsigned int    g5rr_clr              : 1   ; // [27]
        unsigned int    wbcdhd_partfns_clr    : 1   ; // [28]
        unsigned int    ut_end_clr            : 1   ; // [29]
        unsigned int    m0_be_clr             : 1   ; // [30]
        unsigned int    Reserved_3            : 1   ; // [31]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VOMSKINTSTA;

// Define the union U_VDPVERSION1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int vdpversion1             : 32  ; // [31..0]
    } bits;

    // Define an unsigned member
        unsigned int    u32;

} U_VDPVERSION1;
// Define the union U_VDPVERSION2
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int vdpversion2             : 32  ; // [31..0]
    } bits;

    // Define an unsigned member
        unsigned int    u32;

} U_VDPVERSION2;

// Define the union U_G0_CTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ifmt                  : 8   ; // [7..0]
        unsigned int    bitext                : 2   ; // [9..8]
        unsigned int    Reserved_252          : 14  ; // [23..10]
        unsigned int    flip_en               : 1   ; // [24]
        unsigned int    dcmp_en               : 1   ; // [25]
        unsigned int    read_mode             : 1   ; // [26]
        unsigned int    upd_mode              : 1   ; // [27]
        unsigned int    mute_en               : 1   ; // [28]
        unsigned int    Reserved_251          : 1   ; // [29]
        unsigned int    nosec_flag            : 1   ; // [30]
        unsigned int    surface_en            : 1   ; // [31]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_G0_CTRL;

// Define the union U_G0_UPD
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    regup                 : 1   ; // [0]
        unsigned int    Reserved_254          : 31  ; // [31..1]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_G0_UPD;

// Define the union U_G0_ADDR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int surface_addr            : 32  ; // [31..0]
    } bits;

    // Define an unsigned member
        unsigned int    u32;

} U_G0_ADDR;
// Define the union U_G0_NADDR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int surface_addr            : 32  ; // [31..0]
    } bits;

    // Define an unsigned member
        unsigned int    u32;

} U_G0_NADDR;
// Define the union U_G0_STRIDE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    surface_stride        : 16  ; // [15..0]
        unsigned int    Reserved_257          : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_G0_STRIDE;

// Define the union U_G0_IRESO
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    iw                    : 12  ; // [11..0]
        unsigned int    ih                    : 12  ; // [23..12]
        unsigned int    Reserved_258          : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_G0_IRESO;

// Define the union U_G0_SFPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    src_xfpos             : 7   ; // [6..0]
        unsigned int    Reserved_259          : 25  ; // [31..7]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_G0_SFPOS;

// Define the union U_G0_CBMPARA
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    galpha                : 8   ; // [7..0]
        unsigned int    palpha_range          : 1   ; // [8]
        unsigned int    Reserved_261          : 1   ; // [9]
        unsigned int    vedge_p               : 1   ; // [10]
        unsigned int    hedge_p               : 1   ; // [11]
        unsigned int    palpha_en             : 1   ; // [12]
        unsigned int    premult_en            : 1   ; // [13]
        unsigned int    key_en                : 1   ; // [14]
        unsigned int    key_mode              : 1   ; // [15]
        unsigned int    Reserved_260          : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_G0_CBMPARA;

// Define the union U_G0_CKEYMAX
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    keyb_max              : 8   ; // [7..0]
        unsigned int    keyg_max              : 8   ; // [15..8]
        unsigned int    keyr_max              : 8   ; // [23..16]
        unsigned int    va0                   : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_G0_CKEYMAX;

// Define the union U_G0_CKEYMIN
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    keyb_min              : 8   ; // [7..0]
        unsigned int    keyg_min              : 8   ; // [15..8]
        unsigned int    keyr_min              : 8   ; // [23..16]
        unsigned int    va1                   : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_G0_CKEYMIN;

// Define the union U_G0_CMASK
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    kmsk_b                : 8   ; // [7..0]
        unsigned int    kmsk_g                : 8   ; // [15..8]
        unsigned int    kmsk_r                : 8   ; // [23..16]
        unsigned int    Reserved_263          : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_G0_CMASK;

// Define the union U_G0_PARAADDR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int surface_addr            : 32  ; // [31..0]
    } bits;

    // Define an unsigned member
        unsigned int    u32;

} U_G0_PARAADDR;
// Define the union U_G0_PARAUP
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    gdc_paraup            : 1   ; // [0]
        unsigned int    Reserved_264          : 31  ; // [31..1]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_G0_PARAUP;

// Define the union U_G0_DCMP_ADDR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int surface_addr            : 32  ; // [31..0]
    } bits;

    // Define an unsigned member
        unsigned int    u32;

} U_G0_DCMP_ADDR;
// Define the union U_G0_DCMP_NADDR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int surface_addr            : 32  ; // [31..0]
    } bits;

    // Define an unsigned member
        unsigned int    u32;

} U_G0_DCMP_NADDR;
// Define the union U_G0_DCMP_OFFSET
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int offset                  : 32  ; // [31..0]
    } bits;

    // Define an unsigned member
        unsigned int    u32;

} U_G0_DCMP_OFFSET;
// Define the union U_G0_DFPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    disp_xfpos            : 12  ; // [11..0]
        unsigned int    disp_yfpos            : 12  ; // [23..12]
        unsigned int    Reserved_266          : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_G0_DFPOS;

// Define the union U_G0_DLPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    disp_xlpos            : 12  ; // [11..0]
        unsigned int    disp_ylpos            : 12  ; // [23..12]
        unsigned int    Reserved_268          : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_G0_DLPOS;

// Define the union U_G0_VFPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_xfpos           : 12  ; // [11..0]
        unsigned int    video_yfpos           : 12  ; // [23..12]
        unsigned int    Reserved_269          : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_G0_VFPOS;

// Define the union U_G0_VLPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_xlpos           : 12  ; // [11..0]
        unsigned int    video_ylpos           : 12  ; // [23..12]
        unsigned int    Reserved_270          : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_G0_VLPOS;

// Define the union U_G0_BK
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vbk_cr                : 10  ; // [9..0]
        unsigned int    vbk_cb                : 10  ; // [19..10]
        unsigned int    vbk_y                 : 10  ; // [29..20]
        unsigned int    Reserved_271          : 2   ; // [31..30]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_G0_BK;

// Define the union U_G0_ALPHA
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vbk_alpha             : 8   ; // [7..0]
        unsigned int    Reserved_272          : 24  ; // [31..8]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_G0_ALPHA;

// Define the union U_G0_DOF_CTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_273          : 31  ; // [30..0]
        unsigned int    dof_en                : 1   ; // [31]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_G0_DOF_CTRL;

// Define the union U_G0_DOF_STEP
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    left_step             : 8   ; // [7..0]
        unsigned int    right_step            : 8   ; // [15..8]
        unsigned int    Reserved_275          : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_G0_DOF_STEP;

// Define the union U_GP0_CTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    mux1_sel              : 2   ; // [1..0]
        unsigned int    Reserved_280          : 2   ; // [3..2]
        unsigned int    mux2_sel              : 2   ; // [5..4]
        unsigned int    Reserved_279          : 2   ; // [7..6]
        unsigned int    aout_sel              : 2   ; // [9..8]
        unsigned int    Reserved_278          : 2   ; // [11..10]
        unsigned int    bout_sel              : 2   ; // [13..12]
        unsigned int    Reserved_277          : 12  ; // [25..14]
        unsigned int    ffc_sel               : 1   ; // [26]
        unsigned int    Reserved_276          : 3   ; // [29..27]
        unsigned int    mute_en               : 1   ; // [30]
        unsigned int    read_mode             : 1   ; // [31]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_CTRL;

// Define the union U_GP0_UPD
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    regup                 : 1   ; // [0]
        unsigned int    Reserved_282          : 31  ; // [31..1]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_UPD;

// Define the union U_GP0_ORESO
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ow                    : 12  ; // [11..0]
        unsigned int    oh                    : 12  ; // [23..12]
        unsigned int    Reserved_283          : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_ORESO;

// Define the union U_GP0_IRESO
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    iw                    : 12  ; // [11..0]
        unsigned int    ih                    : 12  ; // [23..12]
        unsigned int    Reserved_284          : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_IRESO;

// Define the union U_GP0_HCOEFAD
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int coef_addr               : 32  ; // [31..0]
    } bits;

    // Define an unsigned member
        unsigned int    u32;

} U_GP0_HCOEFAD;
// Define the union U_GP0_VCOEFAD
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int coef_addr               : 32  ; // [31..0]
    } bits;

    // Define an unsigned member
        unsigned int    u32;

} U_GP0_VCOEFAD;
// Define the union U_GP0_PARAUP
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    gp0_hcoef_upd         : 1   ; // [0]
        unsigned int    gp0_vcoef_upd         : 1   ; // [1]
        unsigned int    Reserved_285          : 30  ; // [31..2]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_PARAUP;

// Define the union U_GP0_GALPHA
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    galpha                : 8   ; // [7..0]
        unsigned int    Reserved_286          : 24  ; // [31..8]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_GALPHA;

// Define the union U_GP0_DFPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    disp_xfpos            : 12  ; // [11..0]
        unsigned int    disp_yfpos            : 12  ; // [23..12]
        unsigned int    Reserved_288          : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_DFPOS;

// Define the union U_GP0_DLPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    disp_xlpos            : 12  ; // [11..0]
        unsigned int    disp_ylpos            : 12  ; // [23..12]
        unsigned int    Reserved_290          : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_DLPOS;

// Define the union U_GP0_VFPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_xfpos           : 12  ; // [11..0]
        unsigned int    video_yfpos           : 12  ; // [23..12]
        unsigned int    Reserved_291          : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_VFPOS;

// Define the union U_GP0_VLPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    video_xlpos           : 12  ; // [11..0]
        unsigned int    video_ylpos           : 12  ; // [23..12]
        unsigned int    Reserved_292          : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_VLPOS;

// Define the union U_GP0_BK
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vbk_cr                : 10  ; // [9..0]
        unsigned int    vbk_cb                : 10  ; // [19..10]
        unsigned int    vbk_y                 : 10  ; // [29..20]
        unsigned int    Reserved_293          : 2   ; // [31..30]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_BK;

// Define the union U_GP0_ALPHA
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vbk_alpha             : 8   ; // [7..0]
        unsigned int    Reserved_294          : 24  ; // [31..8]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_ALPHA;

// Define the union U_GP0_CSC_IDC
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscidc0               : 11  ; // [10..0]
        unsigned int    cscidc1               : 11  ; // [21..11]
        unsigned int    csc_en                : 1   ; // [22]
        unsigned int    Reserved_295          : 9   ; // [31..23]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_CSC_IDC;

// Define the union U_GP0_CSC_ODC
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscodc0               : 11  ; // [10..0]
        unsigned int    cscodc1               : 11  ; // [21..11]
        unsigned int    csc_sign_mode         : 1   ; // [22]
        unsigned int    Reserved_297          : 9   ; // [31..23]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_CSC_ODC;

// Define the union U_GP0_CSC_IODC
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscidc2               : 11  ; // [10..0]
        unsigned int    cscodc2               : 11  ; // [21..11]
        unsigned int    Reserved_298          : 10  ; // [31..22]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_CSC_IODC;

// Define the union U_GP0_CSC_P0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscp00                : 15  ; // [14..0]
        unsigned int    Reserved_300          : 1   ; // [15]
        unsigned int    cscp01                : 15  ; // [30..16]
        unsigned int    Reserved_299          : 1   ; // [31]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_CSC_P0;

// Define the union U_GP0_CSC_P1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscp02                : 15  ; // [14..0]
        unsigned int    Reserved_302          : 1   ; // [15]
        unsigned int    cscp10                : 15  ; // [30..16]
        unsigned int    Reserved_301          : 1   ; // [31]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_CSC_P1;

// Define the union U_GP0_CSC_P2
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscp11                : 15  ; // [14..0]
        unsigned int    Reserved_304          : 1   ; // [15]
        unsigned int    cscp12                : 15  ; // [30..16]
        unsigned int    Reserved_303          : 1   ; // [31]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_CSC_P2;

// Define the union U_GP0_CSC_P3
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscp20                : 15  ; // [14..0]
        unsigned int    Reserved_306          : 1   ; // [15]
        unsigned int    cscp21                : 15  ; // [30..16]
        unsigned int    Reserved_305          : 1   ; // [31]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_CSC_P3;

// Define the union U_GP0_CSC_P4
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cscp22                : 15  ; // [14..0]
        unsigned int    Reserved_307          : 17  ; // [31..15]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_CSC_P4;

// Define the union U_GP0_ZME_HSP
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hratio                : 16  ; // [15..0]
        unsigned int    Reserved_310          : 3   ; // [18..16]
        unsigned int    hfir_order            : 1   ; // [19]
        unsigned int    Reserved_309          : 3   ; // [22..20]
        unsigned int    hafir_en              : 1   ; // [23]
        unsigned int    hfir_en               : 1   ; // [24]
        unsigned int    Reserved_308          : 3   ; // [27..25]
        unsigned int    hchmid_en             : 1   ; // [28]
        unsigned int    hlmid_en              : 1   ; // [29]
        unsigned int    hamid_en              : 1   ; // [30]
        unsigned int    hsc_en                : 1   ; // [31]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_ZME_HSP;

// Define the union U_GP0_ZME_HOFFSET
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    hor_coffset           : 16  ; // [15..0]
        unsigned int    hor_loffset           : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_ZME_HOFFSET;

// Define the union U_GP0_ZME_VSP
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_312          : 23  ; // [22..0]
        unsigned int    vafir_en              : 1   ; // [23]
        unsigned int    vfir_en               : 1   ; // [24]
        unsigned int    Reserved_311          : 2   ; // [26..25]
        unsigned int    vsc_luma_tap          : 1   ; // [27]
        unsigned int    vchmid_en             : 1   ; // [28]
        unsigned int    vlmid_en              : 1   ; // [29]
        unsigned int    vamid_en              : 1   ; // [30]
        unsigned int    vsc_en                : 1   ; // [31]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_ZME_VSP;

// Define the union U_GP0_ZME_VSR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vratio                : 16  ; // [15..0]
        unsigned int    Reserved_313          : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_ZME_VSR;

// Define the union U_GP0_ZME_VOFFSET
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vbtm_offset           : 16  ; // [15..0]
        unsigned int    vtp_offset            : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_ZME_VOFFSET;

// Define the union U_GP0_ZME_LTICTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    lhpass_coef4          : 8   ; // [7..0]
        unsigned int    lmixing_ratio         : 8   ; // [15..8]
        unsigned int    lgain_ratio           : 12  ; // [27..16]
        unsigned int    Reserved_314          : 3   ; // [30..28]
        unsigned int    lti_en                : 1   ; // [31]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_ZME_LTICTRL;

// Define the union U_GP0_ZME_LHPASSCOEF
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    lhpass_coef0          : 8   ; // [7..0]
        unsigned int    lhpass_coef1          : 8   ; // [15..8]
        unsigned int    lhpass_coef2          : 8   ; // [23..16]
        unsigned int    lhpass_coef3          : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_ZME_LHPASSCOEF;

// Define the union U_GP0_ZME_LTITHD
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    lover_swing           : 10  ; // [9..0]
        unsigned int    lunder_swing          : 10  ; // [19..10]
        unsigned int    lcoring_thd           : 12  ; // [31..20]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_ZME_LTITHD;

// Define the union U_GP0_ZME_LHFREQTHD
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    lhfreq_thd0           : 16  ; // [15..0]
        unsigned int    lhfreq_thd1           : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_ZME_LHFREQTHD;

// Define the union U_GP0_ZME_LGAINCOEF
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    lgain_coef0           : 8   ; // [7..0]
        unsigned int    lgain_coef1           : 8   ; // [15..8]
        unsigned int    lgain_coef2           : 8   ; // [23..16]
        unsigned int    Reserved_317          : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_ZME_LGAINCOEF;

// Define the union U_GP0_ZME_CTICTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_319          : 8   ; // [7..0]
        unsigned int    cmixing_ratio         : 8   ; // [15..8]
        unsigned int    cgain_ratio           : 12  ; // [27..16]
        unsigned int    Reserved_318          : 3   ; // [30..28]
        unsigned int    cti_en                : 1   ; // [31]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_ZME_CTICTRL;

// Define the union U_GP0_ZME_CHPASSCOEF
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    chpass_coef0          : 8   ; // [7..0]
        unsigned int    chpass_coef1          : 8   ; // [15..8]
        unsigned int    chpass_coef2          : 8   ; // [23..16]
        unsigned int    Reserved_321          : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_ZME_CHPASSCOEF;

// Define the union U_GP0_ZME_CTITHD
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cover_swing           : 10  ; // [9..0]
        unsigned int    cunder_swing          : 10  ; // [19..10]
        unsigned int    ccoring_thd           : 12  ; // [31..20]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_GP0_ZME_CTITHD;

// Define the union U_WBC_G0_CTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    req_interval          : 10  ; // [9..0]
        unsigned int    auto_stop_en          : 1   ; // [10]
        unsigned int    Reserved_323          : 15  ; // [25..11]
        unsigned int    format_out            : 2   ; // [27..26]
        unsigned int    Reserved_322          : 3   ; // [30..28]
        unsigned int    wbc_en                : 1   ; // [31]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_G0_CTRL;

// Define the union U_WBC_G0_UPD
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    regup                 : 1   ; // [0]
        unsigned int    Reserved_325          : 31  ; // [31..1]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_G0_UPD;

// Define the union U_WBC_G0_CMP
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cmp_lossy_en          : 1   ; // [0]
        unsigned int    Reserved_327          : 3   ; // [3..1]
        unsigned int    cmp_drr               : 4   ; // [7..4]
        unsigned int    Reserved_326          : 23  ; // [30..8]
        unsigned int    cmp_en                : 1   ; // [31]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_G0_CMP;

// Define the union U_WBC_G0_AR_ADDR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int wbcaddr                 : 32  ; // [31..0]
    } bits;

    // Define an unsigned member
        unsigned int    u32;

} U_WBC_G0_AR_ADDR;
// Define the union U_WBC_G0_GB_ADDR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int wbccaddr                : 32  ; // [31..0]
    } bits;

    // Define an unsigned member
        unsigned int    u32;

} U_WBC_G0_GB_ADDR;
// Define the union U_WBC_G0_STRIDE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    wbcstride             : 16  ; // [15..0]
        unsigned int    Reserved_329          : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_G0_STRIDE;

// Define the union U_WBC_G0_OFFSET
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int wbcoffset               : 32  ; // [31..0]
    } bits;

    // Define an unsigned member
        unsigned int    u32;

} U_WBC_G0_OFFSET;
// Define the union U_WBC_G0_ORESO
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ow                    : 12  ; // [11..0]
        unsigned int    oh                    : 12  ; // [23..12]
        unsigned int    Reserved_330          : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_G0_ORESO;

// Define the union U_WBC_G0_FCROP
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    wfcrop                : 12  ; // [11..0]
        unsigned int    hfcrop                : 12  ; // [23..12]
        unsigned int    Reserved_331          : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_G0_FCROP;

// Define the union U_WBC_G0_LCROP
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    wlcrop                : 12  ; // [11..0]
        unsigned int    hlcrop                : 12  ; // [23..12]
        unsigned int    Reserved_332          : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_G0_LCROP;

// Define the union U_WBC_GP0_CTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    req_interval          : 10  ; // [9..0]
        unsigned int    auto_stop_en          : 1   ; // [10]
        unsigned int    Reserved_335          : 1   ; // [11]
        unsigned int    wbc_vtthd_mode        : 1   ; // [12]
        unsigned int    Reserved_334          : 5   ; // [17..13]
        unsigned int    three_d_mode          : 2   ; // [19..18]
        unsigned int    Reserved_333          : 4   ; // [23..20]
        unsigned int    format_out            : 4   ; // [27..24]
        unsigned int    mode_out              : 2   ; // [29..28]
        unsigned int    uv_order              : 1   ; // [30]
        unsigned int    wbc_en                : 1   ; // [31]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_GP0_CTRL;

// Define the union U_WBC_GP0_UPD
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    regup                 : 1   ; // [0]
        unsigned int    Reserved_337          : 31  ; // [31..1]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_GP0_UPD;

// Define the union U_WBC_GP0_YADDR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int wbcaddr                 : 32  ; // [31..0]
    } bits;

    // Define an unsigned member
        unsigned int    u32;

} U_WBC_GP0_YADDR;
// Define the union U_WBC_GP0_CADDR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int wbccaddr                : 32  ; // [31..0]
    } bits;

    // Define an unsigned member
        unsigned int    u32;

} U_WBC_GP0_CADDR;
// Define the union U_WBC_GP0_STRIDE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    wbclstride            : 16  ; // [15..0]
        unsigned int    wbccstride            : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_GP0_STRIDE;

// Define the union U_WBC_GP0_ORESO
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ow                    : 12  ; // [11..0]
        unsigned int    oh                    : 12  ; // [23..12]
        unsigned int    Reserved_339          : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_GP0_ORESO;

// Define the union U_WBC_GP0_FCROP
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    wfcrop                : 12  ; // [11..0]
        unsigned int    hfcrop                : 12  ; // [23..12]
        unsigned int    Reserved_341          : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_GP0_FCROP;

// Define the union U_WBC_GP0_LCROP
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    wlcrop                : 12  ; // [11..0]
        unsigned int    hlcrop                : 12  ; // [23..12]
        unsigned int    Reserved_342          : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_GP0_LCROP;

// Define the union U_WBC_GP0_DITHER_CTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_343          : 29  ; // [28..0]
        unsigned int    dither_md             : 3   ; // [31..29]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_WBC_GP0_DITHER_CTRL;

// Define the union U_MIXG0_BKG
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    mixer_bkgcr           : 10  ; // [9..0]
        unsigned int    mixer_bkgcb           : 10  ; // [19..10]
        unsigned int    mixer_bkgy            : 10  ; // [29..20]
        unsigned int    Reserved_406          : 2   ; // [31..30]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_MIXG0_BKG;

// Define the union U_MIXG0_BKALPHA
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    mixer_alpha           : 8   ; // [7..0]
        unsigned int    Reserved_408          : 24  ; // [31..8]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_MIXG0_BKALPHA;

// Define the union U_MIXG0_MIX
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    mixer_prio0           : 4   ; // [3..0]
        unsigned int    mixer_prio1           : 4   ; // [7..4]
        unsigned int    mixer_prio2           : 4   ; // [11..8]
        unsigned int    mixer_prio3           : 4   ; // [15..12]
        unsigned int    Reserved_409          : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_MIXG0_MIX;

// Define the union U_CBM_BKG1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cbm_bkgcr1            : 10  ; // [9..0]
        unsigned int    cbm_bkgcb1            : 10  ; // [19..10]
        unsigned int    cbm_bkgy1             : 10  ; // [29..20]
        unsigned int    Reserved_410          : 2   ; // [31..30]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_CBM_BKG1;

// Define the union U_CBM_MIX1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    mixer_prio0           : 4   ; // [3..0]
        unsigned int    mixer_prio1           : 4   ; // [7..4]
        unsigned int    Reserved_412          : 24  ; // [31..8]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_CBM_MIX1;

// Define the union U_CBM_BKG2
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    cbm_bkgcr2            : 10  ; // [9..0]
        unsigned int    cbm_bkgcb2            : 10  ; // [19..10]
        unsigned int    cbm_bkgy2             : 10  ; // [29..20]
        unsigned int    Reserved_414          : 2   ; // [31..30]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_CBM_BKG2;

// Define the union U_CBM_MIX2
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    mixer_prio0           : 4   ; // [3..0]
        unsigned int    mixer_prio1           : 4   ; // [7..4]
        unsigned int    mixer_prio2           : 4   ; // [11..8]
        unsigned int    mixer_prio3           : 4   ; // [15..12]
        unsigned int    Reserved_416          : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_CBM_MIX2;

// Define the union U_CBM_ATTR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    sur_attr0             : 1   ; // [0]
        unsigned int    sur_attr1             : 1   ; // [1]
        unsigned int    sur_attr2             : 1   ; // [2]
        unsigned int    sur_attr3             : 1   ; // [3]
        unsigned int    sur_attr4             : 1   ; // [4]
        unsigned int    sur_attr5             : 1   ; // [5]
        unsigned int    Reserved_418          : 26  ; // [31..6]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_CBM_ATTR;

// Define the union U_DHD0_CTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    regup                 : 1   ; // [0]
        unsigned int    disp_mode             : 3   ; // [3..1]
        unsigned int    iop                   : 1   ; // [4]
        unsigned int    Reserved_422          : 7   ; // [11..5]
        unsigned int    gmm_mode              : 1   ; // [12]
        unsigned int    gmm_en                : 1   ; // [13]
        unsigned int    hdmi_mode             : 1   ; // [14]
        unsigned int    Reserved_421          : 3   ; // [17..15]
        unsigned int    sin_en                : 1   ; // [18]
        unsigned int    Reserved_420          : 1   ; // [19]
        unsigned int    fpga_lmt_width        : 7   ; // [26..20]
        unsigned int    fpga_lmt_en           : 1   ; // [27]
        unsigned int    p2i_en                : 1   ; // [28]
        unsigned int    cbar_sel              : 1   ; // [29]
        unsigned int    cbar_en               : 1   ; // [30]
        unsigned int    intf_en               : 1   ; // [31]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DHD0_CTRL;

//==============================================================================
// Define the global struct
typedef struct
{
    U_VOCTRL             VOCTRL;
    U_VOINTSTA           VOINTSTA;
    U_VOMSKINTSTA        VOMSKINTSTA;
    unsigned int          Clip_0[1];
    U_VDPVERSION1        VDPVERSION1;
    U_VDPVERSION2        VDPVERSION2;
    unsigned int         Reserved_0[2];
    unsigned int          Clip_1;
    unsigned int         Reserved_1[4];
    unsigned int         Clip_2[5];
    unsigned int         Reserved_2[2];
    unsigned int          Clip_3;
    unsigned int         Reserved_3[43];
    unsigned int         Clip_4[4];
    unsigned int         Reserved_4[4];
    unsigned int          Clip_5;
    unsigned int         Reserved_5[3];
    unsigned int         Clip_6[6];
    unsigned int         Reserved_6[110];
    unsigned int         Clip_7[2];
    unsigned int         Reserved_7[62];
    unsigned int          Clip_8;
    unsigned int         Reserved_8[3];
    unsigned int         Clip_9[2];
    unsigned int         Reserved_9;
    unsigned int          Clip_10;
    unsigned int         Reserved_10[8];
    unsigned int          Clip_11;
    unsigned int         Reserved_11[19];
    unsigned int         Clip_12[2];
    unsigned int         Reserved_12[10];
    unsigned int          Clip_13;
    unsigned int         Reserved_13[11];
    unsigned int         Clip_14[2];
    unsigned int         Reserved_14[194];
    unsigned int         Clip_15[2];
    unsigned int         Reserved_15[6];
    unsigned int          Clip_16;
    unsigned int         Reserved_16;
    unsigned int         Clip_17[2];
    unsigned int         Reserved_17[2];
    unsigned int          Clip_18;
    unsigned int         Reserved_18;
    unsigned int          Clip_19[3];
    unsigned int         Reserved_19;
    unsigned int          Clip_20[10];
    unsigned int         Reserved_20[2];
    unsigned int          Clip_21[8];
    unsigned int         Reserved_21[8];
    unsigned int          Clip_22[3];
    unsigned int         Reserved_22[3];
    unsigned int          Clip_23[4];
    unsigned int         Reserved_23[38];
    unsigned int          Clip_24[4];
    unsigned int         Reserved_24[28];
    unsigned int          Clip_25[6];
    unsigned int         Reserved_25[2];
    unsigned int          Clip_26[6];
    unsigned int         Reserved_26[2];
    unsigned int          Clip_27[6];
    unsigned int         Reserved_27[2];
    unsigned int          Clip_28[6];
    unsigned int         Reserved_28[2];
    unsigned int          Clip_29[6];
    unsigned int         Reserved_29[2];
    unsigned int          Clip_30[6];
    unsigned int         Reserved_30[2];
    unsigned int          Clip_31[6];
    unsigned int         Reserved_31[2];
    unsigned int          Clip_32[6];
    unsigned int         Reserved_32[2];
    unsigned int          Clip_33[6];
    unsigned int         Reserved_33[2];
    unsigned int          Clip_34[6];
    unsigned int         Reserved_34[2];
    unsigned int          Clip_35[6];
    unsigned int         Reserved_35[2];
    unsigned int          Clip_36[6];
    unsigned int         Reserved_36[2];
    unsigned int          Clip_37[6];
    unsigned int         Reserved_37[2];
    unsigned int          Clip_38[6];
    unsigned int         Reserved_38[2];
    unsigned int          Clip_39[6];
    unsigned int         Reserved_39[2];
    unsigned int         Clip_40[6];
    unsigned int         Reserved_40[130];
    unsigned int          Clip_41[2];
    unsigned int         Reserved_41[10];
    unsigned int          Clip_42;
    unsigned int         Reserved_42[3];
    unsigned int         Clip_43[2];
    unsigned int         Reserved_43[46];
    unsigned int          Clip_44;
    unsigned int         Reserved_44;
    unsigned int          Clip_45;
    unsigned int         Reserved_45[5];
    unsigned int          Clip_46[6];
    unsigned int         Reserved_46[3122];
    unsigned int          Clip_47[2];
    unsigned int         Reserved_47[2];
    unsigned int          Clip_48[3];
    unsigned int         Reserved_48;
    unsigned int          Clip_49;
    unsigned int         Reserved_49[31];
    unsigned int         Clip_50[2];
    unsigned int         Reserved_50[22];
    unsigned int         Clip_51[2];
    unsigned int         Reserved_51[2];
    unsigned int          Clip_52[3];
    unsigned int         Reserved_52;
    unsigned int          Clip_53[3];
    unsigned int         Reserved_53;
    unsigned int          Clip_54[3];
    unsigned int         Reserved_54;
    unsigned int          Clip_55[3];
    unsigned int         Reserved_55;
    unsigned int          Clip_56[3];
    unsigned int         Reserved_56;
    unsigned int          Clip_57[6];
    unsigned int         Reserved_57[6];
    unsigned int         Clip_58[2];
    unsigned int         Reserved_58[26];
    unsigned int         Clip_59[6];
    unsigned int         Reserved_59[58];
    unsigned int         Clip_60[16];
    unsigned int         Reserved_60[1840];
    U_G0_CTRL            G0_CTRL;
    U_G0_UPD             G0_UPD;
    unsigned int         Reserved_61[2];
    U_G0_ADDR            G0_ADDR;
    unsigned int         Reserved_62;
    U_G0_NADDR           G0_NADDR;
    U_G0_STRIDE          G0_STRIDE;
    U_G0_IRESO           G0_IRESO;
    U_G0_SFPOS           G0_SFPOS;
    unsigned int         Reserved_63[2];
    U_G0_CBMPARA         G0_CBMPARA;
    U_G0_CKEYMAX         G0_CKEYMAX;
    U_G0_CKEYMIN         G0_CKEYMIN;
    U_G0_CMASK           G0_CMASK;
    U_G0_PARAADDR        G0_PARAADDR;
    U_G0_PARAUP          G0_PARAUP;
    unsigned int         Reserved_64[2];
    U_G0_DCMP_ADDR       G0_DCMP_ADDR;
    U_G0_DCMP_NADDR      G0_DCMP_NADDR;
    U_G0_DCMP_OFFSET     G0_DCMP_OFFSET;
    unsigned int         Reserved_65[9];
    U_G0_DFPOS           G0_DFPOS;
    U_G0_DLPOS           G0_DLPOS;
    U_G0_VFPOS           G0_VFPOS;
    U_G0_VLPOS           G0_VLPOS;
    U_G0_BK              G0_BK;
    U_G0_ALPHA           G0_ALPHA;
    unsigned int         Reserved_66[2];
    U_G0_DOF_CTRL        G0_DOF_CTRL;
    U_G0_DOF_STEP        G0_DOF_STEP;
    unsigned int         Reserved_67[3030];
    U_GP0_CTRL           GP0_CTRL;
    U_GP0_UPD            GP0_UPD;
    U_GP0_ORESO          GP0_ORESO;
    U_GP0_IRESO          GP0_IRESO;
    U_GP0_HCOEFAD        GP0_HCOEFAD;
    U_GP0_VCOEFAD        GP0_VCOEFAD;
    U_GP0_PARAUP         GP0_PARAUP;
    unsigned int         Reserved_68;
    U_GP0_GALPHA         GP0_GALPHA;
    unsigned int         Reserved_69[55];
    U_GP0_DFPOS          GP0_DFPOS;
    U_GP0_DLPOS          GP0_DLPOS;
    U_GP0_VFPOS          GP0_VFPOS;
    U_GP0_VLPOS          GP0_VLPOS;
    U_GP0_BK             GP0_BK;
    U_GP0_ALPHA          GP0_ALPHA;
    unsigned int         Reserved_70[2];
    U_GP0_CSC_IDC        GP0_CSC_IDC;
    U_GP0_CSC_ODC        GP0_CSC_ODC;
    U_GP0_CSC_IODC       GP0_CSC_IODC;
    U_GP0_CSC_P0         GP0_CSC_P0;
    U_GP0_CSC_P1         GP0_CSC_P1;
    U_GP0_CSC_P2         GP0_CSC_P2;
    U_GP0_CSC_P3         GP0_CSC_P3;
    U_GP0_CSC_P4         GP0_CSC_P4;
    U_GP0_ZME_HSP        GP0_ZME_HSP;
    U_GP0_ZME_HOFFSET    GP0_ZME_HOFFSET;
    U_GP0_ZME_VSP        GP0_ZME_VSP;
    U_GP0_ZME_VSR        GP0_ZME_VSR;
    U_GP0_ZME_VOFFSET    GP0_ZME_VOFFSET;
    unsigned int         Reserved_71[3];
    U_GP0_ZME_LTICTRL    GP0_ZME_LTICTRL;
    U_GP0_ZME_LHPASSCOEF GP0_ZME_LHPASSCOEF;
    U_GP0_ZME_LTITHD     GP0_ZME_LTITHD;
    unsigned int         Reserved_72;
    U_GP0_ZME_LHFREQTHD  GP0_ZME_LHFREQTHD;
    U_GP0_ZME_LGAINCOEF  GP0_ZME_LGAINCOEF;
    unsigned int         Reserved_73[2];
    U_GP0_ZME_CTICTRL    GP0_ZME_CTICTRL;
    U_GP0_ZME_CHPASSCOEF GP0_ZME_CHPASSCOEF;
    U_GP0_ZME_CTITHD     GP0_ZME_CTITHD;
    unsigned int         Reserved_74[925];
    U_WBC_G0_CTRL        WBC_G0_CTRL;
    U_WBC_G0_UPD         WBC_G0_UPD;
    U_WBC_G0_CMP         WBC_G0_CMP;
    unsigned int         Reserved_75;
    U_WBC_G0_AR_ADDR     WBC_G0_AR_ADDR;
    U_WBC_G0_GB_ADDR     WBC_G0_GB_ADDR;
    U_WBC_G0_STRIDE      WBC_G0_STRIDE;
    U_WBC_G0_OFFSET      WBC_G0_OFFSET;
    U_WBC_G0_ORESO       WBC_G0_ORESO;
    U_WBC_G0_FCROP       WBC_G0_FCROP;
    U_WBC_G0_LCROP       WBC_G0_LCROP;
    unsigned int         Reserved_76[501];
    U_WBC_GP0_CTRL       WBC_GP0_CTRL;
    U_WBC_GP0_UPD        WBC_GP0_UPD;
    unsigned int         Reserved_77[2];
    U_WBC_GP0_YADDR      WBC_GP0_YADDR;
    U_WBC_GP0_CADDR      WBC_GP0_CADDR;
    U_WBC_GP0_STRIDE     WBC_GP0_STRIDE;
    unsigned int         Reserved_78;
    U_WBC_GP0_ORESO      WBC_GP0_ORESO;
    U_WBC_GP0_FCROP      WBC_GP0_FCROP;
    U_WBC_GP0_LCROP      WBC_GP0_LCROP;
    unsigned int         Reserved_79[53];
    U_WBC_GP0_DITHER_CTRL WBC_GP0_DITHER_CTRL;
    unsigned int          Clip_80;
    unsigned int          Clip_81;
    unsigned int         Reserved_80[189];
    unsigned int         Clip_82[2];
    unsigned int         Reserved_81[2];
    unsigned int          Clip_83[3];
    unsigned int         Reserved_82;
    unsigned int          Clip_84[3];
    unsigned int         Reserved_83;
    unsigned int          Clip_85[3];
    unsigned int         Reserved_84;
    unsigned int          Clip_86;
    unsigned int         Reserved_85[3];
    unsigned int          Clip_87[4];
    unsigned int         Reserved_86[16];
    unsigned int         Clip_88[16];
    unsigned int         Reserved_87[4];
    unsigned int         Clip_89[2];
    unsigned int         Reserved_88[2];
    unsigned int         Clip_90[3];
    unsigned int         Reserved_89[109];
    unsigned int         Clip_91[3];
    unsigned int         Reserved_90[3];
    unsigned int         Clip_92[4];
    unsigned int         Reserved_91[6];
     unsigned int          Clip_93[7];
    unsigned int         Reserved_92[57];
    unsigned int          Clip_94;
    unsigned int         Reserved_93;
    unsigned int          Clip_95;
    unsigned int         Reserved_94[125];
    U_MIXG0_BKG          MIXG0_BKG;
    U_MIXG0_BKALPHA      MIXG0_BKALPHA;
    U_MIXG0_MIX          MIXG0_MIX;
    unsigned int         Reserved_95[125];
    U_CBM_BKG1           CBM_BKG1;
    unsigned int         Reserved_96;
    U_CBM_MIX1           CBM_MIX1;
    unsigned int         Reserved_97[5];
    U_CBM_BKG2           CBM_BKG2;
    unsigned int         Reserved_98;
    U_CBM_MIX2           CBM_MIX2;
    unsigned int         Reserved_99[5];
    U_CBM_ATTR           CBM_ATTR;
    unsigned int         Reserved_100[751];
    U_DHD0_CTRL          DHD0_CTRL;
    unsigned int          Clip_96[8];
    unsigned int         Reserved_101[2];
    unsigned int          Clip_97;
    unsigned int         Reserved_102;
    unsigned int          Clip_98;
    unsigned int         Reserved_103[2];
    unsigned int          Clip_99[11];
    unsigned int         Reserved_104;
    unsigned int          Clip_100[3];
    unsigned int         Reserved_105;
    unsigned int          Clip_101[10];
    unsigned int         Reserved_106[2];
    unsigned int          Clip_102;
    unsigned int         Reserved_107[3];
    unsigned int          Clip_103;
    unsigned int         Reserved_108[3];
    unsigned int          Clip_104[5];
    unsigned int         Reserved_109[2];
       unsigned int          Clip_105[2];
    unsigned int         Reserved_110;
    unsigned int         Clip_106[2];
    unsigned int         Reserved_111[3008];
    unsigned int          Clip_107[7];
    unsigned int         Reserved_112[3];
    unsigned int          Clip_108[11];
    unsigned int         Reserved_113[67];
    unsigned int          Clip_109[31];
    unsigned int         Reserved_114;
    unsigned int         Clip_110[3];
    unsigned int         Reserved_115;
    unsigned int          Clip_111[7];
    unsigned int         Reserved_116[5];
    unsigned int          Clip_112[8];
} S_VOU_CV200_REGS_TYPE;
#endif




/********defeine MDDRC register***********/
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    mem_mode              : 1   ; // [0]
        unsigned int    mem_comb              : 2   ; // [2..1]
        unsigned int    Reserved              : 29  ; // [31..3]
    } bits;

    // Define an unsigned member
    unsigned int    u32;
}U_DDR_MODE;




typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    apg_gt_en              : 1   ; // [0]
        unsigned int    muxcmd_gt_en           : 1   ; // [1]
        unsigned int    detaddr_gt_en          : 1   ; // [2]
        unsigned int    Reserved               : 29  ; // [31..3]
    } bits;

    // Define an unsigned member
    unsigned int    u32;
}U_CLK_CFG;



typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ddr_retention_ctrl    : 1   ; // [0]
        unsigned int    Reserved              : 31  ; // [31..1]
    } bits;

    // Define an unsigned member
    unsigned int    u32;
}U_DDR_RETENTION_CTRL;


// Define the union U_AWADDR_SRVLNC_START
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int awaddr_srvlnc_start                 : 32  ; // [31..0]
    } bits;

    // Define an unsigned member
        unsigned int    u32;

} U_AWADDR_SRVLNC_START;

// Define the union U_AWADDR_SRVLNC_END
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int awaddr_srvlnc_end                 : 32  ; // [31..0]
    } bits;

    // Define an unsigned member
        unsigned int    u32;

} U_AWADDR_SRVLNC_END;


typedef struct
{
    U_DDR_MODE               ddr_mode;
    U_CLK_CFG                clk_cfg;
    U_DDR_RETENTION_CTRL     ddr_retention_ctrl;
    unsigned int             reserved_1[61];
    U_AWADDR_SRVLNC_START    awaddr_srvlnc_start;
    U_AWADDR_SRVLNC_END      awaddr_srvlnc_end;
    unsigned int             reserved_2[62];
    unsigned int             awaddr_srvlnc_status;
}OPTM_MDDRC_REGS_S;

/********************** Global Variable declaration **************************/


/******************************* API declaration *****************************/

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
