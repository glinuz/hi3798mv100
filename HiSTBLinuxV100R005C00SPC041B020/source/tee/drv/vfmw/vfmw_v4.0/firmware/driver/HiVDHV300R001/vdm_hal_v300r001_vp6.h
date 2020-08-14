/*--------------------------------------------------------------------------------------------------------------------------*/
/*!!Warning: This is a key information asset of Huawei Tech Co.,Ltd                                                         */
/*CODEMARK:kOyQZYzjDpyGdBAEC2GaWinjiDDUykL9e8pckESWBbMVmSWkBuyJO01cTiy3TdzKxGk0oBQa
mSMf7J4FkTpfvzHyMxSEsfcbL/G0fFswaAZ8tsS4we+PBWC6a/UNlzCWIaw+Ujkv9NAY+as0
fg7WZLpvZ4VPqIbjb0KpG6XdFat4tnTLtqIN9Iw+iFxWCcAtxTiudoiQzmq6nYvUMZGWHooh
Q1hyB359Xl8YcVfqu+PSgPHJwS6TCJH5Oly/clrpitZ6IsBdfJ7mq8SOoA5iMg==*/
/*--------------------------------------------------------------------------------------------------------------------------*/

/******************************************************************************

  版权所有 (C), 2001-2011, 华为技术有限公司

******************************************************************************
    文 件 名   : 
    版 本 号   : 
    作    者   : 
    生成日期   : 
    最近修改   :
    功能描述   : 
         

  修改历史   :
    1.日    期 : 
    作    者   : 
    修改内容   : 

******************************************************************************/
#ifndef __VDM_HAL_V300R001_VP6_H__
#define __VDM_HAL_V300R001_VP6_H__


#include "basedef.h"
#include "mem_manage.h"

#if 1
//// REG & MSG START
//寄存器组
typedef struct  {
    USIGN dec_start:                            1;
    USIGN reserved:                             31;
} VP6_V300R001_VDH_START;

typedef struct  {
    USIGN repair_start:                         1;
    USIGN reserved:                             31;
} VP6_V300R001_VDH_REPAIR;

typedef struct  {
    USIGN mbamt_to_dec:                         16;
    USIGN reserved:                             14; 
    USIGN load_qmatrix_flag:                    1;   
    USIGN repair_en:                            1;    
} VP6_V300R001_BASIC_CFG0;

typedef struct  {
    USIGN video_standard:                       4;
    USIGN ddr_stride:                           10;
    USIGN fst_slc_grp:                          1;
    USIGN mv_output_en:                         1; 
    USIGN max_slcgrp_num:                       12;
    USIGN reserved:                             1;
    USIGN dblk_1d_en:                           1;
    USIGN compress_en:                            1;
    USIGN ppfd_en:                              1;
} VP6_V300R001_BASIC_CFG1;

typedef struct  {
    USIGN av_msg_addr:                          32;
} VP6_V300R001_AVM_ADDR;

typedef struct  {
    USIGN va_msg_addr:                          32;
} VP6_V300R001_VAM_ADDR;

typedef struct  {
    USIGN stream_base_addr:                     32;
} VP6_V300R001_STREAM_BASE_ADDR;

typedef struct  {
    USIGN intrs_vdh_dec_over:                   1;
    USIGN intrs_vdh_dec_err:                    1;
    USIGN intrs_rpr_dec_over:                   1;
    USIGN reserved:                             4;
    USIGN version_id:                           8;
} VP6_V300R001_VDH_STATE;
#endif

//PIC 下行消息组
typedef struct  {
    USIGN pic_coding_type:                      1;
    //USIGN RefreshGoldenFrame:                   1;
    USIGN reserved:                             31;
} VP6_V300R001_DWNMSG_D0;

typedef struct  {
    USIGN pic_width_in_mb:                      9;
    USIGN reserved0:                            7;
    USIGN pic_height_in_mb:                     9;
    USIGN reserved1:                            7;
} VP6_V300R001_DWNMSG_D1;

typedef struct  {

    USIGN Vp3VersionNo:                         5;
    USIGN VpProfile:                            2;
    USIGN MultiStream:                          1;
    USIGN UseHuffman:                           1;
    USIGN UseLoopFilter:                        1;
    //USIGN ScalingMode:                          2;
    USIGN reserved:                             22;
} VP6_V300R001_DWNMSG_D2;

typedef struct  {
    USIGN PredictionFilterAlpha:                5;
    USIGN PredictionFilterMvSizeThresh:         3;
    USIGN PredictionFilterVarThresh:            10;
    USIGN PredictionFilterMode:                 2;
    USIGN reserved:                             12;
} VP6_V300R001_DWNMSG_D3;

typedef struct  {
    USIGN FLimit:                               4;
    USIGN DcQuant:                              8;
    USIGN AcQuant:                              9;
    USIGN reserved:                             11;
} VP6_V300R001_DWNMSG_D4;

typedef struct  {
    USIGN value:                                32;
} VP6_V300R001_DWNMSG_D8;

typedef struct  {
    USIGN range:                                8;
    USIGN reserved0:                            8;
    USIGN count:                                4;
    USIGN reserved1:                            12;
} VP6_V300R001_DWNMSG_D9;

typedef struct  {
    USIGN bit0_len_0:                           25;
    USIGN bit0_offset_0:                        7;
} VP6_V300R001_DWNMSG_D16;

typedef struct  {
    USIGN bit0_stream_addr_0:                   24;
    USIGN reserved:                             8;
} VP6_V300R001_DWNMSG_D17;

typedef struct  {
    USIGN bit0_len_1:                           25;
    USIGN bit0_offset_1:                        7;
} VP6_V300R001_DWNMSG_D18;

typedef struct  {
    USIGN bit0_stream_addr_1:                   24;
    USIGN reserved:                             8;
} VP6_V300R001_DWNMSG_D19;

typedef struct  {
    USIGN bit1_len_0:                           25;
    USIGN bit1_offset_0:                        7;
} VP6_V300R001_DWNMSG_D20;

typedef struct  {
    USIGN bit1_stream_addr_0:                   24;
    USIGN reserved:                             8;
} VP6_V300R001_DWNMSG_D21;

typedef struct  {
    USIGN bit1_len_1:                           25;
    USIGN bit1_offset_1:                        7;
} VP6_V300R001_DWNMSG_D22;

typedef struct  {
    USIGN bit1_stream_addr_1:                   24;
    USIGN reserved:                             8;
} VP6_V300R001_DWNMSG_D23;

typedef struct  {
    USIGN curr_rcn_addr:                        32;
} VP6_V300R001_DWNMSG_D32;                                 

typedef struct  {
    USIGN golden_ref_addr:                      32;
} VP6_V300R001_DWNMSG_D34;

typedef struct  {
    USIGN last_ref_addr:                        32;
} VP6_V300R001_DWNMSG_D35;

typedef struct  {
    USIGN sed_top_addr:                         32;
} VP6_V300R001_DWNMSG_D36;

typedef struct  {
    USIGN pmv_top_addr:                         32;
} VP6_V300R001_DWNMSG_D37;

typedef struct  {
    USIGN rcn_top_addr:                         32;
} VP6_V300R001_DWNMSG_D38;

typedef struct  {
   USIGN tab_addr:                              32;
} VP6_V300R001_DWNMSG_D39;

//上行消息组
typedef struct  {
    USIGN error_mb_num:                         16;
    USIGN error_flag:                           1;
    USIGN endofvop_flag:                        1;
    USIGN pic_coding_type:                      2;
    USIGN reserved:                             1;    
    USIGN error_mode:                           11;
} VP6_V300R001_UPMSG_D0;

typedef struct  {
    USIGN slice_start_mb_num:                   16;
    USIGN slice_end_mb_num:                     16;
} VP6_V300R001_UPMSG_D1;

#ifdef __cplusplus
extern "C" {
#endif

SINT32 VP6HAL_V300R001_InitHal(SINT32 CodeTabAddr);
//SINT32 VP6HAL_V200R003_InitHal(SINT32 CodeTabAddr);
SINT32 VP6HAL_V300R001_StartDec(VP6_DEC_PARAM_S *pVp6sDecParam, SINT32 VdhId);
SINT32 VP6HAL_V300R001_CfgReg(VP6_DEC_PARAM_S *pVp6DecParam, VDMHAL_HWMEM_S *pHwMem , SINT32 VdhId);
SINT32 VP6HAL_V300R001_CfgDnMsg(VP6_DEC_PARAM_S *pVp6DecParam, VDMHAL_HWMEM_S *pHwMem , SINT32 VdhId);

#ifdef __cplusplus
}
#endif

#endif //__VDM_HAL_AVS_H__

