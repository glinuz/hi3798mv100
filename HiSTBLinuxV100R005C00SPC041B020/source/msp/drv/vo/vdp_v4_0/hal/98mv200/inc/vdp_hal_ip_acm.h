#ifndef __VDP_HAL_IP_ACM_H__
#define __VDP_HAL_IP_ACM_H__

//#include "c_union_define.h"
#include "hi_reg_common.h"
#include "vdp_define.h"


HI_VOID VDP_VID_SetAcmAcm3En     ( HI_U32 u32Data, HI_U32 acm3_en);
HI_VOID VDP_VID_SetAcmAcm2En     ( HI_U32 u32Data, HI_U32 acm2_en);
HI_VOID VDP_VID_SetAcmAcm1En     ( HI_U32 u32Data, HI_U32 acm1_en);
HI_VOID VDP_VID_SetAcmAcm0En     ( HI_U32 u32Data, HI_U32 acm0_en);
HI_VOID VDP_VID_SetAcmAcmSecBlk0 ( HI_U32 u32Data, HI_U32 acm0_sec_blk);
HI_VOID VDP_VID_SetAcmAcmFirBlk0 ( HI_U32 u32Data, HI_U32 acm0_fir_blk);
HI_VOID VDP_VID_SetAcmAcmDUOff0  ( HI_U32 u32Data, HI_U32 acm0_d_u_off);
HI_VOID VDP_VID_SetAcmAcmCUOff0  ( HI_U32 u32Data, HI_U32 acm0_c_u_off);
HI_VOID VDP_VID_SetAcmAcmBUOff0  ( HI_U32 u32Data, HI_U32 acm0_b_u_off);
HI_VOID VDP_VID_SetAcmAcmAUOff0  ( HI_U32 u32Data, HI_U32 acm0_a_u_off);
HI_VOID VDP_VID_SetAcmAcmTestEn  ( HI_U32 u32Data, HI_U32 acm_test_en);
HI_VOID VDP_VID_SetAcmAcmDbgMode ( HI_U32 u32Data, HI_U32 acm_dbg_mode);
HI_VOID VDP_VID_SetAcmAcmProtMode ( HI_U32 u32Data, HI_U32 bw_pro_mode);
HI_VOID VDP_VID_SetAcmAcmDVOff0  ( HI_U32 u32Data, HI_U32 acm0_d_v_off);
HI_VOID VDP_VID_SetAcmAcmCVOff0  ( HI_U32 u32Data, HI_U32 acm0_c_v_off);
HI_VOID VDP_VID_SetAcmAcmBVOff0  ( HI_U32 u32Data, HI_U32 acm0_b_v_off);
HI_VOID VDP_VID_SetAcmAcmAVOff0  ( HI_U32 u32Data, HI_U32 acm0_a_v_off);
HI_VOID VDP_VID_SetAcmAcmSecBlk1 ( HI_U32 u32Data, HI_U32 acm1_sec_blk);
HI_VOID VDP_VID_SetAcmAcmFirBlk1 ( HI_U32 u32Data, HI_U32 acm1_fir_blk);
HI_VOID VDP_VID_SetAcmAcmDUOff1  ( HI_U32 u32Data, HI_U32 acm1_d_u_off);
HI_VOID VDP_VID_SetAcmAcmCUOff1  ( HI_U32 u32Data, HI_U32 acm1_c_u_off);
HI_VOID VDP_VID_SetAcmAcmBUOff1  ( HI_U32 u32Data, HI_U32 acm1_b_u_off);
HI_VOID VDP_VID_SetAcmAcmAUOff1  ( HI_U32 u32Data, HI_U32 acm1_a_u_off);
HI_VOID VDP_VID_SetAcmAcmDVOff1  ( HI_U32 u32Data, HI_U32 acm1_d_v_off);
HI_VOID VDP_VID_SetAcmAcmCVOff1  ( HI_U32 u32Data, HI_U32 acm1_c_v_off);
HI_VOID VDP_VID_SetAcmAcmBVOff1  ( HI_U32 u32Data, HI_U32 acm1_b_v_off);
HI_VOID VDP_VID_SetAcmAcmAVOff1  ( HI_U32 u32Data, HI_U32 acm1_a_v_off);
HI_VOID VDP_VID_SetAcmAcmSecBlk2 ( HI_U32 u32Data, HI_U32 acm2_sec_blk);
HI_VOID VDP_VID_SetAcmAcmFirBlk2 ( HI_U32 u32Data, HI_U32 acm2_fir_blk);
HI_VOID VDP_VID_SetAcmAcmDUOff2  ( HI_U32 u32Data, HI_U32 acm2_d_u_off);
HI_VOID VDP_VID_SetAcmAcmCUOff2  ( HI_U32 u32Data, HI_U32 acm2_c_u_off);
HI_VOID VDP_VID_SetAcmAcmBUOff2  ( HI_U32 u32Data, HI_U32 acm2_b_u_off);
HI_VOID VDP_VID_SetAcmAcmAUOff2  ( HI_U32 u32Data, HI_U32 acm2_a_u_off);
HI_VOID VDP_VID_SetAcmAcmDVOff2  ( HI_U32 u32Data, HI_U32 acm2_d_v_off);
HI_VOID VDP_VID_SetAcmAcmCVOff2  ( HI_U32 u32Data, HI_U32 acm2_c_v_off);
HI_VOID VDP_VID_SetAcmAcmBVOff2  ( HI_U32 u32Data, HI_U32 acm2_b_v_off);
HI_VOID VDP_VID_SetAcmAcmAVOff2  ( HI_U32 u32Data, HI_U32 acm2_a_v_off);
HI_VOID VDP_VID_SetAcmAcmSecBlk3 ( HI_U32 u32Data, HI_U32 acm3_sec_blk);
HI_VOID VDP_VID_SetAcmAcmFirBlk3 ( HI_U32 u32Data, HI_U32 acm3_fir_blk);
HI_VOID VDP_VID_SetAcmAcmDUOff3  ( HI_U32 u32Data, HI_U32 acm3_d_u_off);
HI_VOID VDP_VID_SetAcmAcmCUOff3  ( HI_U32 u32Data, HI_U32 acm3_c_u_off);
HI_VOID VDP_VID_SetAcmAcmBUOff3  ( HI_U32 u32Data, HI_U32 acm3_b_u_off);
HI_VOID VDP_VID_SetAcmAcmAUOff3  ( HI_U32 u32Data, HI_U32 acm3_a_u_off);
HI_VOID VDP_VID_SetAcmAcmDVOff3  ( HI_U32 u32Data, HI_U32 acm3_d_v_off);
HI_VOID VDP_VID_SetAcmAcmCVOff3  ( HI_U32 u32Data, HI_U32 acm3_c_v_off);
HI_VOID VDP_VID_SetAcmAcmBVOff3  ( HI_U32 u32Data, HI_U32 acm3_b_v_off);
HI_VOID VDP_VID_SetAcmAcmAVOff3  ( HI_U32 u32Data, HI_U32 acm3_a_v_off);

#endif


