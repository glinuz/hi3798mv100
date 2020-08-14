/*--------------------------------------------------------------------------------------------------------------------------*/
/*!!Warning: This is a key information asset of Huawei Tech Co.,Ltd                                                         */
/*CODEMARK:kOyQZYzjDpyGdBAEC2GaWinjiDDUykL9e8pckESWBbMVmSWkBuyJO01cTiy3TdzKxGk0oBQa
mSMf7J4FkTpfvzHyMxSEsfcbL/G0fFswaAZ8tsS4we+PBWC6a/UNlzCWIaw+Ujkv9NAY+as0
fg7WZMMifKMkczCId4VHIXMDidrLRP2/mf8L+Uf8Vs8OtXXOA/0mYo7x4kZJIoKpRg9STY5U
+XYAw9SEjAq6J0BNphYHg6UfaT2dsX3PbIOe1oe8gm4oUXGEnTzBEH6HvTZYgw==*/
/*--------------------------------------------------------------------------------------------------------------------------*/

/******************************************************************************

  版权所有 (C), 2001-2011, 华为技术有限公司

******************************************************************************
    文 件 名   : vdm_hal_v200r003_avs.h
    版 本 号   : 初稿
    作    者   : 
    生成日期   : 
    最近修改   :
    功能描述   : VDM300plus硬件抽象
         

  修改历史   :
    1.日    期 : 2009-03-04
    作    者   : 
    修改内容   : 

******************************************************************************/

#ifndef __VDM_HAL_V300R001_AVS_H__
#define __VDM_HAL_V300R001_AVS_H__


#include "basedef.h"
#include "mem_manage.h"
#include "syn_cmn.h"

/*AVS下行消息池定义*/
typedef struct
{                  
    UINT32 pic_width_in_mb:          9;
    UINT32 rsv0:                     7;
    UINT32 pic_height_in_mb:         9;
    UINT32 chroma_format:          2;
    UINT32 sample_precision:          2;
	UINT32 rsv1:                     3;
} AVS_V300R001_DWNMSG_D0;   

typedef struct
{                    
    UINT32 progressive_frame:        1;
    UINT32 picture_structure:        2;
    UINT32 pic_coding_type:          2;
    UINT32 top_field_first:          1;
    UINT32 repeat_first_field:       1;
    UINT32 fix_qp:                   1;
    UINT32 pic_qp:                   6;
    UINT32 skip_mode_flag:           1;
    UINT32 ad_pred_mode_disable:     1;
    UINT32 pic_ref_flag:             1;
    UINT32 no_fwd_ref_flag:          1;
    UINT32 rsv:                      14;
} AVS_V300R001_DWNMSG_D1;   

typedef struct
{                  
    UINT32 loop_flt_disable:         1;
    UINT32 loop_flt_para_flag:       1;
    UINT32 alpha_c_offset:           5;
    UINT32 beta_offset:              5;
    UINT32 rsv:                      20;
} AVS_V300R001_DWNMSG_D2;

typedef struct
{
    UINT32 fwd_top_field_first_0:  1;
    UINT32 fwd_picture_structure_0:  2;
    UINT32 fwd_top_field_first_1:  1;
    UINT32 fwd_picture_structure_1:  2;
    UINT32 bwd_top_field_first:  1;
    UINT32 bwd_picture_structure:  2;
    UINT32 rsv:                      23;
} AVS_V300R001_DWNMSG_D3;   

typedef struct
{                  
    UINT32 bwd_address:            32;
} AVS_V300R001_DWNMSG_D4;  

typedef struct
{                  
    UINT32 fwd_address_0:            32;
} AVS_V300R001_DWNMSG_D5;   

typedef struct
{                  
    UINT32 fwd_address_1:            32;
} AVS_V300R001_DWNMSG_D6;   

typedef struct
{                  
    UINT32 rcn_address:              32;
} AVS_V300R001_DWNMSG_D7;   

typedef struct
{                  
    UINT32 col_pic_type:             1;
    UINT32 rsv0:                     23;
    UINT32 col_pic_struct:           2;
    UINT32 rsv1:                     6;
} AVS_V300R001_DWNMSG_D8;

typedef struct
{                  
    UINT32 bit_stream_address_0:     24;
    UINT32 rsv:                      8;
} AVS_V300R001_DWNMSG_D9;

typedef struct
{                  
    UINT32 bit_len_0:                24;
    UINT32 bit_offset_0:             7;
    UINT32 rsv:                      1;
} AVS_V300R001_DWNMSG_D10;

typedef struct
{                  
    UINT32 bit_stream_address_1:     24;
    UINT32 rsv:                      8;
} AVS_V300R001_DWNMSG_D11;

typedef struct
{                  
#if 1
    UINT32 weighting_quant_flag:           1;
	UINT32 chroma_quant_param_disable:    1;
	UINT32 chroma_quant_param_delta_v:    6;
	UINT32 chroma_quant_param_delta_u:    6;
	UINT32 aec_enable:                      1;
	UINT32 p_filed_enhanced_flag:          1;
	UINT32 b_filed_enhanced_flag:          1;
	UINT32 rsv:                             15;
#endif
#if 0
    UINT32 bit_len_1:                24;
    UINT32 bit_offset_1:             7;
    UINT32 rsv:                      1;
#endif
} AVS_V300R001_DWNMSG_D12;

typedef struct
{
    UINT32 pmv_top_addr:         32;
} AVS_V300R001_DWNMSG_D13;

typedef struct
{
    UINT32 topmb_intra_addr:         32;
} AVS_V300R001_DWNMSG_D14;

typedef struct
{
    UINT32 colpic_mv_addr:           32;
} AVS_V300R001_DWNMSG_D16;

typedef struct
{
    UINT32 pmv_cfg_table:           32;
} AVS_V300R001_DWNMSG_D17_48;

typedef struct
{
    UINT32 pmv_colmb_addr:           32;
} AVS_V300R001_DWNMSG_D49;
typedef struct
{
    UINT32 dblk_top_addr:         32;
} AVS_V300R001_DWNMSG_D50; 
typedef struct
{
    UINT32 sed_top_addr:         32;
} AVS_V300R001_DWNMSG_D51; 


typedef struct
{
	SINT32 bit_stream_addr_0;
	SINT32 bit_offset_0;
	SINT32 bit_len_0;
	SINT32 bit_stream_addr_1;
	SINT32 bit_offset_1;
	SINT32 bit_len_1;	
	SINT32 slice_start_mbn;
	SINT32 slice_end_mbn;
	SINT32 next_slice_para_addr;	
} AVS_V300R001_SLCINFO_S;


#ifdef __cplusplus
extern "C" {
#endif

SINT32 AVSHAL_V300R001_InitHal(SINT32 CodeTabAddr);
SINT32 AVSHAL_V300R001_StartDec(AVS_DEC_PARAM_S *pAvsDecParam, SINT32 VdhId);

#ifdef __cplusplus
}
#endif


#endif //__VDM_HAL_AVS_H__
