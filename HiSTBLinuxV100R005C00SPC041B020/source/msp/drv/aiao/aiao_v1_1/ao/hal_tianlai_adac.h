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

// Define the union U_S40_TIANLAI_ADAC_CRG
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int adac_cken             : 1; // [0]
        unsigned int Reserved_0            : 3; // [3..1]
        unsigned int adac_srst_req         : 1; // [4]
        unsigned int Reserved_1            : 27; // [31..5]
    } bits;

    // Define an unsigned member
    unsigned int u32;
} U_S40_TIANLAI_ADAC_CRG;

/***************************************************************************
Description:
    define emum varible
***************************************************************************/

/*****************************************************************************
 Description  : ADAC API
*****************************************************************************/
HI_VOID ADAC_TIANLAI_Init(HI_U32 uSampelRate, HI_BOOL bResume);
HI_VOID ADAC_TIANLAI_DeInit(HI_BOOL bSuspend);




#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /* __AUD_TIANLAI_ADAC_HAL_H__ */
