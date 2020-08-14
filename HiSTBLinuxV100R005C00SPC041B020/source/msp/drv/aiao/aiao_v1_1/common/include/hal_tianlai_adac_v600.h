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
#if 0
typedef union
{
    // Define the struct bits
    struct
    {
        HI_U32 dacr_vol             : 7; // [6..0]
        HI_U32 reserved1     : 1; // [7]
        HI_U32 dacl_vol               : 7; // [14..8]
        HI_U32 reserved2           : 3; // [17..15]
        HI_U32 deemphasis_fs       : 2; // [19..18]
        HI_U32 dacr_deemph       : 1; // [20]
        HI_U32 dacl_deemph       : 1; // [21]
        HI_U32 dacr_path       : 1; // [22]
        HI_U32 dacl_path       : 1; // [23]
        HI_U32 popfreer       : 1; // [24]
        HI_U32 popfreel       : 1; // [25]
        HI_U32 fs       : 1; // [26]
        HI_U32 pd_vref       : 1; // [27]
        HI_U32 mute_dacr       : 1; // [28]
        HI_U32 mute_dacl       : 1; // [29]
        HI_U32 pd_dacr       : 1; // [30]
        HI_U32 pd_dacl       : 1; // [31]
        
    } bits;

    // Define an unsigned member
    HI_U32 u32;
} SC_PERI_TIANLAI_ADAC0;

#endif

/* Define the union U_PERI_TIANLA_ADAC0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dacr_vol              : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 1   ; /* [7]  */
        unsigned int    dacl_vol              : 7   ; /* [14..8]  */
        unsigned int    reserved_1            : 3   ; /* [17..15]  */
        unsigned int    deemphasis_fs         : 2   ; /* [19..18]  */
        unsigned int    dacr_deemph           : 1   ; /* [20]  */
        unsigned int    dacl_deemph           : 1   ; /* [21]  */
        unsigned int    dacr_path             : 1   ; /* [22]  */
        unsigned int    dacl_path             : 1   ; /* [23]  */
        unsigned int    reserved_2            : 4   ; /* [27..24]  */
        unsigned int    mute_dacr             : 1   ; /* [28]  */
        unsigned int    mute_dacl             : 1   ; /* [29]  */
        unsigned int    pd_dacr               : 1   ; /* [30]  */
        unsigned int    pd_dacl               : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} SC_PERI_TIANLAI_ADAC0;

#if 0
typedef union
{
    // Define the struct bits
    struct
    {
        HI_U32 reserved1             : 8; // [7..0]
        HI_U32 clkdgesel     : 1; // [8]
        HI_U32 clksel2               : 1; // [9]
        HI_U32 adj_refbf           : 2; // [11..10]    
        HI_U32 rst       : 1; // [12]
        HI_U32 adj_ctcm       : 1; // [13]      
        HI_U32 adj_dac       : 2; // [15..14]
        HI_U32 reserved2       : 3; // [18:16]
        HI_U32 sample_sel       : 3; // [21..19]
        HI_U32 data_bits       : 2; // [23..22]
        HI_U32 reserved3       : 1; // [24]
        HI_U32 mute_rate       : 2; // [26..25]
        HI_U32 dacvu       : 1; // [27]
        HI_U32 sunmuter       : 1; // [28]
        HI_U32 sunmutel       : 1; // [29]
        HI_U32 smuter       : 1; // [30]
        HI_U32 smutel       : 1; // [31]
    } bits;

    // Define an unsigned member
    HI_U32 u32;
} SC_PERI_TIANLAI_ADAC1;
#endif

/* Define the union U_PERI_TIANLA_ADAC1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pop_direct_r          : 1   ; /* [0]  */
        unsigned int    pop_adj_res           : 2   ; /* [2..1]  */
        unsigned int    pop_direct_l          : 1   ; /* [3]  */
        unsigned int    pop_adj_clk           : 2   ; /* [5..4]  */
        unsigned int    reserved_0            : 1   ; /* [6]  */
        unsigned int    verf_extmod           : 1   ; /* [7]  */
        unsigned int    reserved_1            : 11  ; /* [18..8]  */
        unsigned int    clksel                : 3   ; /* [21..19]  */
        unsigned int    data_bits             : 2   ; /* [23..22]  */
        unsigned int    reserved_2            : 8   ; /* [31..24]  */
    } bits;


    /* Define an unsigned member */
    unsigned int    u32;

} SC_PERI_TIANLAI_ADAC1;

typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ctrl_dffr             : 1   ; /* [0]  */
        unsigned int    ctrl_dffl             : 1   ; /* [1]  */
        unsigned int    pd_vref               : 1   ; /* [2]  */
        unsigned int    Pd_ctcm_ibias         : 1   ; /* [3]  */
        unsigned int    pd_ibias              : 1   ; /* [4]  */
        unsigned int    pd_dacr               : 1   ; /* [5]  */
        unsigned int    pd_dacl               : 1   ; /* [6]  */
        unsigned int    pd_ctcm               : 1   ; /* [7]  */
        unsigned int    adj_refbf             : 2   ; /* [9..8]  */
        unsigned int    adj_dac               : 2   ; /* [11..10]  */
        unsigned int    adj_ctcm              : 1   ; /* [12]  */
        unsigned int    RST                   : 1   ; /* [13]  */
        unsigned int    mute_dacr             : 1   ; /* [14]  */
        unsigned int    mute_dacl             : 1   ; /* [15]  */
        unsigned int    Td_sel                : 5   ; /* [20..16]  */
        unsigned int    fs                    : 1   ; /* [21]  */
        unsigned int    popfreer              : 1   ; /* [22]  */
        unsigned int    popfreel              : 1   ; /* [23]  */
        unsigned int    clkdgesel             : 1   ; /* [24]  */
        unsigned int    clksel                : 1   ; /* [25]  */
        unsigned int    Chop_dacvref_sel      : 2   ; /* [27..26]  */
        unsigned int    Chop_ctcm_sel         : 2   ; /* [29..28]  */
        unsigned int    Chop_bps_dacvref      : 1   ; /* [30]  */
        unsigned int    Chop_bps_ctcm         : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} SC_PERI_TIANLAI_ADAC2;




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
