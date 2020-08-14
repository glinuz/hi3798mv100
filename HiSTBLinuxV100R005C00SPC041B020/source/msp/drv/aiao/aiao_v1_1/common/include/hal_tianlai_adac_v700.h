/***********************************************************************************
*              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName:
* Description: TianLai internal audio dac hal
*
* History:
* Version   Date         Author         DefectNum    Description
* main\1    2012-11-06   AudioGroup     NULL         Create this file.
***********************************************************************************/
#ifndef __AUD_TIANLAI_ADAC_HAL_H__
#define __AUD_TIANLAI_ADAC_HAL_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#include "hi_type.h"
#include "hi_unf_sound.h"

/***************************************************************************
Description:
    define const varible
***************************************************************************/

/* Define the union U_PERI_TIANLA_ADAC0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adac_dig_dacr_vol     : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 1   ; /* [7]  */
        unsigned int    adac_dig_dacl_vol     : 7   ; /* [14..8]  */
        unsigned int    reserved_1            : 3   ; /* [17..15]  */
        unsigned int    adac_dig_deemphasis_fs : 2   ; /* [19..18]  */
        unsigned int    adac_dig_dacr_deemph  : 1   ; /* [20]  */
        unsigned int    adac_dig_dacl_deemph  : 1   ; /* [21]  */
        unsigned int    adac_dig_dacr_path    : 1   ; /* [22]  */
        unsigned int    adac_dig_dacl_path    : 1   ; /* [23]  */
        unsigned int    reserved_2            : 4   ; /* [27..24]  */
        unsigned int    adac_dig_mute_dacr    : 1   ; /* [28]  */
        unsigned int    adac_dig_mute_dacl    : 1   ; /* [29]  */
        unsigned int    adac_dig_pd_dacr      : 1   ; /* [30]  */
        unsigned int    adac_dig_pd_dacl      : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} SC_PERI_TIANLA_ADAC0;

/* Define the union U_PERI_TIANLA_ADAC1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adac_adj_refbf        : 3   ; /* [2..0]  */
        unsigned int    adac_adj_ctcm         : 2   ; /* [4..3]  */
        unsigned int    adac_adj_dac          : 3   ; /* [7..5]  */
        unsigned int    reserved_0            : 11  ; /* [18..8]  */
        unsigned int    adac_clk_sel_sam      : 3   ; /* [21..19]  */
        unsigned int    adac_data_bits        : 2   ; /* [23..22]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} SC_PERI_TIANLA_ADAC1;


/* Define the union U_PERI_TIANLA_ADAC2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adac_fs               : 1   ; /* [0]  */
        unsigned int    adac_pd_vref          : 1   ; /* [1]  */
        unsigned int    adac_pdb_ctcm_ibias   : 1   ; /* [2]  */
        unsigned int    adac_pd_ctcm          : 1   ; /* [3]  */
        unsigned int    adac_pd_ibias         : 1   ; /* [4]  */
        unsigned int    adac_mute_lineout     : 1   ; /* [5]  */
        unsigned int    adac_pd_dacr          : 1   ; /* [6]  */
        unsigned int    adac_pd_dacl          : 1   ; /* [7]  */
        unsigned int    adac_pop_pu_pullb     : 1   ; /* [8]  */
        unsigned int    adac_pop_pullout_en   : 1   ; /* [9]  */
        unsigned int    adac_adj_dac_low_bias : 1   ; /* [10]  */
        unsigned int    adac_adj_cp           : 2   ; /* [12..11]  */
        unsigned int    adac_gain             : 3   ; /* [15..13]  */
        unsigned int    reserved_0            : 1   ; /* [16]  */
        unsigned int    adac_mute_sp_lineout  : 1   ; /* [17]  */
        unsigned int    adac_vref_extmod      : 1   ; /* [18]  */
        unsigned int    adac_td_sel           : 5   ; /* [23..19]  */
        unsigned int    adac_dac_lineout_chop_sel : 2   ; /* [25..24]  */
        unsigned int    adac_dac_vref_chop_sel : 2   ; /* [27..26]  */
        unsigned int    adac_ctcm_chop_sel    : 2   ; /* [29..28]  */
        unsigned int    adac_chop_bps_dacvref : 1   ; /* [30]  */
        unsigned int    adac_chop_bps_ctcm    : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} SC_PERI_TIANLA_ADAC2;

/* Define the union U_PERI_TIANLA_ADAC3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adac_chop_bps_lineout : 1   ; /* [0]  */
        unsigned int    adac_pop_spd_cfg      : 2   ; /* [2..1]  */
        unsigned int    adac_pop_pdm_delay_cfg : 2   ; /* [4..3]  */
        unsigned int    adac_rst_pop          : 1   ; /* [5]  */
        unsigned int    adac_pop_dis          : 1   ; /* [6]  */
        unsigned int    adac_pd_clkramp       : 1   ; /* [7]  */
        unsigned int    adac_pop_clk_sel      : 2   ; /* [9..8]  */
        unsigned int    adac_cp_dt_ctrl       : 2   ; /* [11..10]  */
        unsigned int    adac_cp_clk_sel       : 2   ; /* [13..12]  */
        unsigned int    adac_cp_en_soft       : 1   ; /* [14]  */
        unsigned int    adac_cp_pd            : 1   ; /* [15]  */
        unsigned int    adac_cp_buffer_pd     : 1   ; /* [16]  */
        unsigned int    adac_pd_lineout       : 1   ; /* [17]  */
        unsigned int    adac_msel             : 1   ; /* [18]  */
        unsigned int    reserved_0            : 2   ; /* [20..19]  */
        unsigned int    adac_rst              : 1   ; /* [21]  */
        unsigned int    adac_clkdgesel        : 1   ; /* [22]  */
        unsigned int    adac_clksel           : 1   ; /* [23]  */
        unsigned int    adac_ctrl7            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} SC_PERI_TIANLA_ADAC3;

// Define the union U_S40_TIANLAI_ADAC_CRG
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    adac_cken             : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    adac_srst_req         : 1   ; /* [4]  */
        unsigned int    reserved_1            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_S40_TIANLAI_ADAC_CRG;

/***************************************************************************
Description:
    define emum varible
***************************************************************************/

/*****************************************************************************
 Description  : ADAC API
*****************************************************************************/
#ifdef HI_SND_MUTECTL_SUPPORT
HI_VOID ADAC_FastPowerEnable(HI_BOOL bEnable);
#endif
HI_VOID ADAC_TIANLAI_Init(HI_U32 uSampelRate, HI_BOOL bResume);
HI_VOID ADAC_TIANLAI_DeInit(HI_BOOL bSuspend);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __AUD_TIANLAI_ADAC_HAL_H__ */
