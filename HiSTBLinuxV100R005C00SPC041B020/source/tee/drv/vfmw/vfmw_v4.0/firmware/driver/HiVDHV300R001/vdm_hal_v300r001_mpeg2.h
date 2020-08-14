/*--------------------------------------------------------------------------------------------------------------------------*/
/*!!Warning: This is a key information asset of Huawei Tech Co.,Ltd                                                         */
/*CODEMARK:kOyQZYzjDpyGdBAEC2GaWinjiDDUykL9e8pckESWBbMVmSWkBuyJO01cTiy3TdzKxGk0oBQa
mSMf7J4FkTpfvzHyMxSEsfcbL/G0fFswaAZ8tsS4we+PBWC6a/UNlzCWIaw+Ujkv9NAY+as0
fg7WZB+aurbTb+grV7JG95WFd/ICKEiygQWHyyjsDM+vnh7UBhUvh0pTHAH2QfsYgyziLCkF
6+XDWVvOwcdxA3qz7eyCKfH/yCF4q7LSreB12NaTT+1zkErMaF/qIul99kmjHg==*/
/*--------------------------------------------------------------------------------------------------------------------------*/

/*********************************************************************** 
*
* Copyright (c) 2009 Hisilicon - All Rights Reserved
*
* File: $vdm_hal_mpeg2.h$
* Date: $2009/09/01$
* Revision: $v1.0$
* Purpose: HiVDHV100 IP MPEG2 VDM HAL Module.
*
* Change History:
*
* Date       Author            Change
* ======     ======            ======
* 20090901   q45134            Original
*
************************************************************************/

#ifndef __VDM_HAL_V300R001_MPEG2_H__
#define __VDM_HAL_V300R001_MPEG2_H__


#include "basedef.h"

//MPEG2下行消息池
typedef struct
{
    USIGN pic_width_in_mb:                      9;
    USIGN reserved0:                            7;
    USIGN pic_height_in_mb:                     9;
    USIGN mpeg1_flag:                           1;
    USIGN reserved1:                            6;
} MP2DNMSG_V300R001_D0;

typedef struct
{
    USIGN frame_pred_frame_dct:                 1;
    USIGN reserved0:                            7;
    USIGN picture_structure:                    2;
    USIGN second_field_flag:                    1;
    USIGN reserved1:                            5;
    USIGN concealment_motion_vectors:           1;
    USIGN reserved2:                            7;
    USIGN pic_coding_type:                      3;
    USIGN full_pel_forward_vector:              1;
    USIGN full_pel_backward_vector:             1;
    USIGN reserved3:                            3;
} MP2DNMSG_V300R001_D1;

typedef struct
{
    USIGN fcode_11:                             4;
    USIGN reserved0:                            4;
    USIGN fcode_10:                             4;
    USIGN reserved1:                            4;
    USIGN fcode_01:                             4;
    USIGN reserved2:                            4;
    USIGN fcode_00:                             4;
    USIGN reserved3:                            3;
    USIGN top_field_first:                      1;
} MP2DNMSG_V300R001_D2;                                 

typedef struct
{
    USIGN intra_dc_precision:                   2;
    USIGN reserved0:                            6;
    USIGN q_scale_type:                         1;
    USIGN reserved1:                            7;
    USIGN intra_vlc_format:                     1;
    USIGN reserved2:                            7;
    USIGN alternate_scan:                       1;
    USIGN reserved3:                            7;
} MP2DNMSG_V300R001_D3;

typedef struct
{
    USIGN bwd_address:                          32;
} MP2DNMSG_V300R001_D4;                                 

typedef struct
{
    USIGN fwd_address:                          32;
} MP2DNMSG_V300R001_D5;                                 

typedef struct
{
    USIGN rcn_address:                          32;
} MP2DNMSG_V300R001_D6;                                  

typedef struct
{  
    USIGN current_pmv_addr:                     32;
} MP2DNMSG_V300R001_D7;

typedef struct
{
    USIGN bit_stream_address_0:                 24;
    USIGN reserved:                             8;
} MP2DNMSG_V300R001_D8;

typedef struct
{
    USIGN bit_len_0:                            24;
    USIGN bit_offset_0:                         7;
    USIGN reserved:                             1;
} MP2DNMSG_V300R001_D9;

typedef struct
{
    USIGN bit_stream_address_1:                 24;
    USIGN reserved:                             8;
} MP2DNMSG_V300R001_D10;

typedef struct
{
    USIGN bit_len_1:                            24;
    USIGN bit_offset_1:                         7;
    USIGN reserved:                             1;
} MP2DNMSG_V300R001_D11;

#if 0
typedef struct
{
    USIGN intra_quantiser_matrix_even:          8;
    USIGN non_Intra_quantiser_matrix_even:      8;
    USIGN intra_quantiser_matrix_odd:           8;
    USIGN non_Intra_quantiser_matrix_odd:       8;
} MP2DNMSG_V300R001_D16_47;
#else
typedef struct
{
    USIGN intra_quantiser_matrix_0:          8;
    USIGN intra_quantiser_matrix_1:      	 8;
    USIGN intra_quantiser_matrix_2:          8;
    USIGN intra_quantiser_matrix_3:        	 8;
} MP2DNMSG_V300R001_D16_31;
typedef struct
{
    USIGN non_intra_quantiser_matrix_0:          8;
    USIGN non_intra_quantiser_matrix_1:      	 8;
    USIGN non_intra_quantiser_matrix_2:          8;
    USIGN non_intra_quantiser_matrix_3:        	 8;
} MP2DNMSG_V300R001_D32_47;
#endif

typedef struct
{
    USIGN pmv_top_addr:                         32;
} MP2DNMSG_V300R001_D48;

typedef struct
{
    USIGN first_slc_dnmsg_addr:                32;
} MP2DNMSG_V300R001_D63;

//MPEG2上行消息池
typedef struct
{
    USIGN slice_start_mb_num:                   16;
    USIGN slice_end_mb_num:                     16;
} MP2UPMSG_V300R001_D1;

//MPEG2 SLICE 
typedef struct  {                              
    USIGN bit_len_0:                            24;
    USIGN bit_offset_0:                         7;
    USIGN reserved:                             1;
}MP2SLCDNMSG_V300R001_D0;                                  

typedef struct  {
    USIGN bit_stream_address_0:               24;
    USIGN reserved:                             8;
}MP2SLCDNMSG_V300R001_D1;                                  

typedef struct  {                              
    USIGN bit_len_1:                            24;
    USIGN bit_offset_1:                         7;
    USIGN reserved:                             1;
}MP2SLCDNMSG_V300R001_D2;  

typedef struct  {                              
    USIGN bit_stream_address_1:               24;
    USIGN reserved:                             8;
}MP2SLCDNMSG_V300R001_D3;  

typedef struct  {                              
    USIGN quantiser_scale_code:               6;
    USIGN intra_slice:                         1;
    USIGN reserved:                            25;
}MP2SLCDNMSG_V300R001_D4;                                 

typedef struct  {                              
    USIGN slice_start_mbn:                     20;
    USIGN reserved:                             12;
}MP2SLCDNMSG_V300R001_D5;                               

typedef struct  {                              
    USIGN slice_end_mbn:                       20;
    USIGN reserved:                             12;
}MP2SLCDNMSG_V300R001_D6;                               

typedef struct  {                              
    USIGN next_slice_para_addr:               32;
}MP2SLCDNMSG_V300R001_D7;    


//MPEG2消息池结构体
typedef struct
{
    MP2DNMSG_V300R001_D0     *d0;
    MP2DNMSG_V300R001_D1     *d1;
    MP2DNMSG_V300R001_D2     *d2;
    MP2DNMSG_V300R001_D3     *d3;
    MP2DNMSG_V300R001_D4     *d4;
    MP2DNMSG_V300R001_D5     *d5;
    MP2DNMSG_V300R001_D6     *d6;
    MP2DNMSG_V300R001_D7     *d7;
    MP2DNMSG_V300R001_D8     *d8;
    MP2DNMSG_V300R001_D9     *d9;
    MP2DNMSG_V300R001_D10    *d10;
    MP2DNMSG_V300R001_D11    *d11;
	MP2DNMSG_V300R001_D16_31 *dmatrix_intra[32];
	MP2DNMSG_V300R001_D32_47 *dmatrix_non_intra[32];
    MP2DNMSG_V300R001_D48    *d48;
    MP2DNMSG_V300R001_D63    *d63;
} MP2_V300R001_DNMSGINF_S;

typedef struct
{
    MP2UPMSG_V300R001_D1     *d1[MAX_UP_MSG_SLICE_NUM];
} MP2_V200R003_UPMSGINF_S;


#ifdef __cplusplus
extern "C" {
#endif

SINT32 MP2HAL_V300R001_InitHal( SINT32 CodeTabAddr);
SINT32 MP2HAL_V300R001_StartDec( MP2_DEC_PARAM_S *pMp2DecParam, SINT32 VdhId );

#ifdef __cplusplus
}
#endif


#endif //__VDM_HAL_MPEG2_H__
