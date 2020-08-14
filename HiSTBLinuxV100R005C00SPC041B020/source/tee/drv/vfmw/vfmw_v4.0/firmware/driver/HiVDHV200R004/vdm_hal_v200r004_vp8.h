/*--------------------------------------------------------------------------------------------------------------------------*/
/*!!Warning: This is a key information asset of Huawei Tech Co.,Ltd                                                         */
/*CODEMARK:kOyQZYzjDpyGdBAEC2GaWinjiDDUykL9e8pckESWBbMVmSWkBuyJO01cTiy3TdzKxGk0oBQa
mSMf7J4FkTpfv/JzwTgKEHe/9N4yU5rqhEDaRH/mYdIijFrTqr/UaRBfB2ylm29ZtemC0p1r
w4HAPbvHkbR9nmyvaX/pXwkvzWTfph+Ek7O+pvI8ySoLRC9f6bj125UbfF41UpwNV2GU8Y1Y
diznsUBeMBLaYDBI6XnJFpWZbmH86rt6KWsjrphXQzOaq12MnODsY02lU85ujg==*/
/*--------------------------------------------------------------------------------------------------------------------------*/

/******************************************************************************

  版权所有 (C), 2001-2011, 华为技术有限公司

******************************************************************************
    文 件 名   : vdm_hal_vp8.h
    版 本 号   : 初稿
    作    者   : 
    生成日期   : 
    最近修改   :
    功能描述   : VDM300plus硬件抽象
         

  修改历史   :
    1.日    期 : 2011-09-17
    作    者   : 
    修改内容   : 

******************************************************************************/

#ifndef __VDM_HAL_V200R004_VP8_HERAER__
#define __VDM_HAL_V200R004_VP8_HERAER__


//PIC 下行消息组
typedef struct  {
    USIGN cur_frame_type:                        1;
    USIGN last_frame_type:                       2;
    USIGN reserved:                             29;
} VP8_V200R004_PICDNMSG_D0;

typedef struct  {
	USIGN use_bilinear_mc_filter:                1;
	USIGN full_pixel:                            1;
    USIGN reserved:                             30;
} VP8_V200R004_PICDNMSG_D1;

typedef struct  {
	USIGN pic_width_in_mb:                   	 9;
	USIGN reserved0:                             7;
	USIGN pic_height_in_mb:                  	 9;
	USIGN reserved1:                             7;
} VP8_V200R004_PICDNMSG_D2;

typedef struct  {
	USIGN prob_skip_false:                       8;
	USIGN mb_no_coeff_skip:                      1;
	USIGN multi_token_partition:                 2;
    USIGN reserved:                             21;
} VP8_V200R004_PICDNMSG_D3;

typedef struct  {
    USIGN segmentation_enable:                   1;
    USIGN update_mb_segmentation_map:            1;
    USIGN mb_segmentation_abs_delta:             1;
    USIGN mode_ref_lf_delta_enabled:             1;
    USIGN reserved:                             28;
} VP8_V200R004_PICDNMSG_D4;

typedef struct  {
    USIGN cur_filter_type:                      1;
    USIGN last_filter_type:                     2;
    USIGN filter_level:                         6;
    USIGN cur_sharpness_level:                  3;
    USIGN last_sharpness_level:                	3;	
    USIGN reserved:                             17;
} VP8_V200R004_PICDNMSG_D5;

typedef struct  {
    USIGN y1dc_sign:                             1;
    USIGN y1dc_absdelta_q:                       4;
    USIGN y2dc_sign:                             1;
    USIGN y2dc_absdelta_q:                       4;
    USIGN y2ac_sign:                             1;	
    USIGN y2ac_absdelta_q:                       4;
    USIGN uvdc_sign:                             1;
    USIGN uvdc_absdelta_q:                       4;
    USIGN uvac_sign:                             1;
    USIGN uvac_absdelta_q:                       4;
    USIGN reserved:                              7;
} VP8_V200R004_PICDNMSG_D6;

typedef struct {
	USIGN base_qindex:							 7;
	USIGN reserved:                              25;
} VP8_V200R004_PICDNMSG_D7;


typedef struct {
	USIGN value:                              	32;
} VP8_V200R004_PICDNMSG_D8;


typedef struct  {
    USIGN range:                                 8;
    USIGN reserved0:                             8;
    USIGN count:                                 6;
	USIGN reserved1:                            10;
} VP8_V200R004_PICDNMSG_D9;


typedef struct  {
    USIGN bit_len_0:                           25;
    USIGN bit_offset_0:                         7;
} VP8_V200R004_PICDNMSG_D16;

typedef struct  {
    USIGN bit_stream_addr_0:                   24;
    USIGN reserved:                             8;
} VP8_V200R004_PICDNMSG_D17;

typedef struct  {
    USIGN bit_len_1:                           25;
    USIGN bit_offset_1:                        7;
} VP8_V200R004_PICDNMSG_D18;

typedef struct  {
    USIGN bit_stream_addr_1:                   24;
    USIGN reserved:                             8;
} VP8_V200R004_PICDNMSG_D19;

typedef struct  {
	USIGN segment_feature_dat00:                8;
	USIGN segment_feature_dat01:                8;
	USIGN segment_feature_dat02:                8;
	USIGN segment_feature_dat03:                8;
} VP8_V200R004_PICDNMSG_D20;


typedef struct  {
	USIGN segment_feature_dat10:                8;
	USIGN segment_feature_dat11:                8;
	USIGN segment_feature_dat12:                8;
	USIGN segment_feature_dat13:                8;
} VP8_V200R004_PICDNMSG_D21;

typedef struct  {
	USIGN ref_lf_delta0:                		8;
	USIGN ref_lf_delta1:                		8;
	USIGN ref_lf_delta2:                		8;
	USIGN ref_lf_delta3:                		8;
} VP8_V200R004_PICDNMSG_D22;

typedef struct  {
	USIGN mode_lf_delta0:                		8;
	USIGN mode_lf_delta1:                		8;
	USIGN mode_lf_delta2:                		8;
	USIGN mode_lf_delta3:                		8;
} VP8_V200R004_PICDNMSG_D23;

typedef struct  {
    USIGN  ref_frame_sign_bias_0:   			8;
    USIGN  ref_frame_sign_bias_1:   			8;
    USIGN  ref_frame_sign_bias_2:   			8;
    USIGN  ref_frame_sign_bias_3:   			8;
} VP8_V200R004_PICDNMSG_D24;

typedef struct  {
    USIGN curr_rcn_addr:                        32;
} VP8_V200R004_PICDNMSG_D32;                                 

typedef struct  {
    USIGN alt_ref_addr:                         32;
} VP8_V200R004_PICDNMSG_D33;

typedef struct  {
    USIGN golden_ref_addr:                      32;
} VP8_V200R004_PICDNMSG_D34;

typedef struct  {
    USIGN last_ref_addr:                        32;
} VP8_V200R004_PICDNMSG_D35;

typedef struct  {
    USIGN sed_top_addr:                         32;
} VP8_V200R004_PICDNMSG_D36;

typedef struct  {
    USIGN pmv_top_addr:                         32;
} VP8_V200R004_PICDNMSG_D37;

typedef struct  {
    USIGN rcn_top_addr:                         32;
} VP8_V200R004_PICDNMSG_D38;

typedef struct  {
    USIGN tab_addr:                             32;
} VP8_V200R004_PICDNMSG_D39;

typedef struct {
	USIGN dblk_top_addr:                        32;
} VP8_V200R004_PICDNMSG_D40;


/*typedef struct {
	USIGN dblk_top_addr:                        32;
} VP8_V200R003_PICDNMSG_D59;*/

typedef struct  {
    USIGN bitn_len:                          	25;
    USIGN bitn_offset:                        	7;
} VP8_V200R004_BITSTREAM_LEN_OFFSET;

typedef struct  {
    USIGN bitn_stream_addr:                  	24;
    USIGN reserved:                             8;
} VP8_V200R004_BITSTREAM_ADDR;


//上行消息组
typedef struct  {
    USIGN error_mb_num:                         16;
    USIGN error_flag:                           1;
    USIGN endofvop_flag:                        1;
    USIGN pic_coding_type:                      2;
    USIGN reserved:                             1;    
    USIGN error_mode:                           11;
} VP8_V200R004_UPMSG_D0;

typedef struct  {
    USIGN slice_start_mb_num:                   16;
    USIGN slice_end_mb_num:                     16;
} VP8_V200R004_UPMSG_D1;



#ifdef __cplusplus
extern "C" {
#endif

SINT32 VP8HAL_V200R004_InitHal(SINT32 CodeTabAddr);
SINT32 VP8HAL_V200R004_CfgReg(VP8_DEC_PARAM_S *pVp8DecParam, VDMHAL_HWMEM_S *pHwMem , SINT32 VdhId);
SINT32 VP8HAL_V200R004_StartDec(VP8_DEC_PARAM_S *pVp8DecParam, SINT32 VdhId);
SINT32 VP8HAL_V200R004_CfgDnMsg(VP8_DEC_PARAM_S *pVp8DecParam, VDMHAL_HWMEM_S *pHwMem, SINT32 VdhId);
#ifdef __cplusplus
}
#endif


#endif 

