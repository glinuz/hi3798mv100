/******************************************************************************

 Copyright @ Hisilicon Technologies Co., Ltd. 1998-2015. All rights reserved.

 ******************************************************************************
  File Name     : vpss_cfgreg.h
  Version       : Initial Draft
  Author        : z00178013
  Created       : 2016/1/25
  Last Modified :
  Description   : vpss_cfgreg.c header file
  Function List :
  History       :
  1.Date        : 2016/1/25
    Author      : z00178013
    Modification: Created file

******************************************************************************/
#ifndef __VPSS_CFGREG_H__
#define __VPSS_CFGREG_H__
#include "hi_type.h"
#include "hi_reg_common.h"

/*----------------------------------------------*
 * external variables                           *
 *----------------------------------------------*/

/*----------------------------------------------*
 * external routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * internal routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * project-wide global variables                *
 *----------------------------------------------*/

/*----------------------------------------------*
 * module-wide global variables                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * constants                                    *
 *----------------------------------------------*/

/*----------------------------------------------*
 * macros                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * routines' implementations                    *
 *----------------------------------------------*/




#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

extern HI_VOID VPSS_DRV_ClrInt( S_VPSS_REGS_TYPE *pstVpssRegs, HI_U32 u32ClrInt );
extern HI_S32 VPSS_DRV_GetIntStatus( S_VPSS_REGS_TYPE *pstVpssRegs );
extern HI_S32 VPSS_DRV_GetRawIntStatus( S_VPSS_REGS_TYPE *pstVpssRegs );
extern HI_S32 VPSS_DRV_Get_SMMU_INTSTAT_S(S_VPSS_REGS_TYPE *pstVpssRegs, HI_U32 *pu32SecureState,HI_U32 *pu32NoSecureState);
extern HI_S32 VPSS_DRV_SetClockEn(HI_BOOL bClkEn);
extern HI_VOID VPSS_DRV_SetIntMask( S_VPSS_REGS_TYPE *pstVpssRegs, HI_U32 u32Mask );
extern HI_S32 VPSS_DRV_SetTimeOut(S_VPSS_REGS_TYPE *pstVpssRegs,HI_U32 u32Data);
extern HI_VOID VPSS_DRV_Set_bfield(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 bfield );
extern HI_VOID VPSS_DRV_Set_bfield_first(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 bfield_first );
extern HI_VOID VPSS_DRV_Set_bfield_mode(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 bfield_mode );
extern HI_VOID VPSS_DRV_Set_blk_det_en(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 blk_det_en );
extern HI_VOID VPSS_DRV_Set_chk_sum_en(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 chk_sum_en );
extern HI_VOID VPSS_DRV_Set_cur_vc1_en(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cur_vc1_en );
extern HI_VOID VPSS_DRV_Set_dei_en(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dei_en );
extern HI_VOID VPSS_DRV_Set_four_pix_en(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 four_pix_en );
extern HI_VOID VPSS_DRV_Set_ifmd_en(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ifmd_en );
extern HI_VOID VPSS_DRV_Set_igbm_en(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 igbm_en );
extern HI_VOID VPSS_DRV_Set_img_pro_mode(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 img_pro_mode );
extern HI_VOID VPSS_DRV_Set_mcdi_en(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 mcdi_en );
extern HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_01(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lut07 ,HI_U32 motion_edge_lut06 ,HI_U32 motion_edge_lut05 ,HI_U32 motion_edge_lut04 ,HI_U32 motion_edge_lut03 ,HI_U32 motion_edge_lut02 ,HI_U32 motion_edge_lut01 ,HI_U32 motion_edge_lut00 );
extern HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_02(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lut0f ,HI_U32 motion_edge_lut0e ,HI_U32 motion_edge_lut0d ,HI_U32 motion_edge_lut0c ,HI_U32 motion_edge_lut0b ,HI_U32 motion_edge_lut0a ,HI_U32 motion_edge_lut09 ,HI_U32 motion_edge_lut08 );
extern HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_03(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lut17 ,HI_U32 motion_edge_lut16 ,HI_U32 motion_edge_lut15 ,HI_U32 motion_edge_lut14 ,HI_U32 motion_edge_lut13 ,HI_U32 motion_edge_lut12 ,HI_U32 motion_edge_lut11 ,HI_U32 motion_edge_lut10 );
extern HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_04(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lut1f ,HI_U32 motion_edge_lut1e ,HI_U32 motion_edge_lut1d ,HI_U32 motion_edge_lut1c ,HI_U32 motion_edge_lut1b ,HI_U32 motion_edge_lut1a ,HI_U32 motion_edge_lut19 ,HI_U32 motion_edge_lut18 );
extern HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_05(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lut27 ,HI_U32 motion_edge_lut26 ,HI_U32 motion_edge_lut25 ,HI_U32 motion_edge_lut24 ,HI_U32 motion_edge_lut23 ,HI_U32 motion_edge_lut22 ,HI_U32 motion_edge_lut21 ,HI_U32 motion_edge_lut20 );
extern HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_06(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lut2f ,HI_U32 motion_edge_lut2e ,HI_U32 motion_edge_lut2d ,HI_U32 motion_edge_lut2c ,HI_U32 motion_edge_lut2b ,HI_U32 motion_edge_lut2a ,HI_U32 motion_edge_lut29 ,HI_U32 motion_edge_lut28 );
extern HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_07(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lut37 ,HI_U32 motion_edge_lut36 ,HI_U32 motion_edge_lut35 ,HI_U32 motion_edge_lut34 ,HI_U32 motion_edge_lut33 ,HI_U32 motion_edge_lut32 ,HI_U32 motion_edge_lut31 ,HI_U32 motion_edge_lut30 );
extern HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_08(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lut3f ,HI_U32 motion_edge_lut3e ,HI_U32 motion_edge_lut3d ,HI_U32 motion_edge_lut3c ,HI_U32 motion_edge_lut3b ,HI_U32 motion_edge_lut3a ,HI_U32 motion_edge_lut39 ,HI_U32 motion_edge_lut38 );
extern HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_09(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lut47 ,HI_U32 motion_edge_lut46 ,HI_U32 motion_edge_lut45 ,HI_U32 motion_edge_lut44 ,HI_U32 motion_edge_lut43 ,HI_U32 motion_edge_lut42 ,HI_U32 motion_edge_lut41 ,HI_U32 motion_edge_lut40 );
extern HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_10(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lut4f ,HI_U32 motion_edge_lut4e ,HI_U32 motion_edge_lut4d ,HI_U32 motion_edge_lut4c ,HI_U32 motion_edge_lut4b ,HI_U32 motion_edge_lut4a ,HI_U32 motion_edge_lut49 ,HI_U32 motion_edge_lut48 );
extern HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_11(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lut57 ,HI_U32 motion_edge_lut56 ,HI_U32 motion_edge_lut55 ,HI_U32 motion_edge_lut54 ,HI_U32 motion_edge_lut53 ,HI_U32 motion_edge_lut52 ,HI_U32 motion_edge_lut51 ,HI_U32 motion_edge_lut50 );
extern HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_12(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lut5f ,HI_U32 motion_edge_lut5e ,HI_U32 motion_edge_lut5d ,HI_U32 motion_edge_lut5c ,HI_U32 motion_edge_lut5b ,HI_U32 motion_edge_lut5a ,HI_U32 motion_edge_lut59 ,HI_U32 motion_edge_lut58 );
extern HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_13(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lut67 ,HI_U32 motion_edge_lut66 ,HI_U32 motion_edge_lut65 ,HI_U32 motion_edge_lut64 ,HI_U32 motion_edge_lut63 ,HI_U32 motion_edge_lut62 ,HI_U32 motion_edge_lut61 ,HI_U32 motion_edge_lut60 );
extern HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_14(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lut6f ,HI_U32 motion_edge_lut6e ,HI_U32 motion_edge_lut6d ,HI_U32 motion_edge_lut6c ,HI_U32 motion_edge_lut6b ,HI_U32 motion_edge_lut6a ,HI_U32 motion_edge_lut69 ,HI_U32 motion_edge_lut68 );
extern HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_15(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lut77 ,HI_U32 motion_edge_lut76 ,HI_U32 motion_edge_lut75 ,HI_U32 motion_edge_lut74 ,HI_U32 motion_edge_lut73 ,HI_U32 motion_edge_lut72 ,HI_U32 motion_edge_lut71 ,HI_U32 motion_edge_lut70 );
extern HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_16(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lut7f ,HI_U32 motion_edge_lut7e ,HI_U32 motion_edge_lut7d ,HI_U32 motion_edge_lut7c ,HI_U32 motion_edge_lut7b ,HI_U32 motion_edge_lut7a ,HI_U32 motion_edge_lut79 ,HI_U32 motion_edge_lut78 );
extern HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_17(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lut87 ,HI_U32 motion_edge_lut86 ,HI_U32 motion_edge_lut85 ,HI_U32 motion_edge_lut84 ,HI_U32 motion_edge_lut83 ,HI_U32 motion_edge_lut82 ,HI_U32 motion_edge_lut81 ,HI_U32 motion_edge_lut80 );
extern HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_18(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lut8f ,HI_U32 motion_edge_lut8e ,HI_U32 motion_edge_lut8d ,HI_U32 motion_edge_lut8c ,HI_U32 motion_edge_lut8b ,HI_U32 motion_edge_lut8a ,HI_U32 motion_edge_lut89 ,HI_U32 motion_edge_lut88 );
extern HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_19(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lut97 ,HI_U32 motion_edge_lut96 ,HI_U32 motion_edge_lut95 ,HI_U32 motion_edge_lut94 ,HI_U32 motion_edge_lut93 ,HI_U32 motion_edge_lut92 ,HI_U32 motion_edge_lut91 ,HI_U32 motion_edge_lut90 );
extern HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_20(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lut9f ,HI_U32 motion_edge_lut9e ,HI_U32 motion_edge_lut9d ,HI_U32 motion_edge_lut9c ,HI_U32 motion_edge_lut9b ,HI_U32 motion_edge_lut9a ,HI_U32 motion_edge_lut99 ,HI_U32 motion_edge_lut98 );
extern HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_21(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_luta7 ,HI_U32 motion_edge_luta6 ,HI_U32 motion_edge_luta5 ,HI_U32 motion_edge_luta4 ,HI_U32 motion_edge_luta3 ,HI_U32 motion_edge_luta2 ,HI_U32 motion_edge_luta1 ,HI_U32 motion_edge_luta0 );
extern HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_22(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lutaf ,HI_U32 motion_edge_lutae ,HI_U32 motion_edge_lutad ,HI_U32 motion_edge_lutac ,HI_U32 motion_edge_lutab ,HI_U32 motion_edge_lutaa ,HI_U32 motion_edge_luta9 ,HI_U32 motion_edge_luta8 );
extern HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_23(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lutb7 ,HI_U32 motion_edge_lutb6 ,HI_U32 motion_edge_lutb5 ,HI_U32 motion_edge_lutb4 ,HI_U32 motion_edge_lutb3 ,HI_U32 motion_edge_lutb2 ,HI_U32 motion_edge_lutb1 ,HI_U32 motion_edge_lutb0 );
extern HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_24(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lutbf ,HI_U32 motion_edge_lutbe ,HI_U32 motion_edge_lutbd ,HI_U32 motion_edge_lutbc ,HI_U32 motion_edge_lutbb ,HI_U32 motion_edge_lutba ,HI_U32 motion_edge_lutb9 ,HI_U32 motion_edge_lutb8 );
extern HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_25(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lutc7 ,HI_U32 motion_edge_lutc6 ,HI_U32 motion_edge_lutc5 ,HI_U32 motion_edge_lutc4 ,HI_U32 motion_edge_lutc3 ,HI_U32 motion_edge_lutc2 ,HI_U32 motion_edge_lutc1 ,HI_U32 motion_edge_lutc0 );
extern HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_26(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lutcf ,HI_U32 motion_edge_lutce ,HI_U32 motion_edge_lutcd ,HI_U32 motion_edge_lutcc ,HI_U32 motion_edge_lutcb ,HI_U32 motion_edge_lutca ,HI_U32 motion_edge_lutc9 ,HI_U32 motion_edge_lutc8 );
extern HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_27(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lutd7 ,HI_U32 motion_edge_lutd6 ,HI_U32 motion_edge_lutd5 ,HI_U32 motion_edge_lutd4 ,HI_U32 motion_edge_lutd3 ,HI_U32 motion_edge_lutd2 ,HI_U32 motion_edge_lutd1 ,HI_U32 motion_edge_lutd0 );
extern HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_28(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lutdf ,HI_U32 motion_edge_lutde ,HI_U32 motion_edge_lutdd ,HI_U32 motion_edge_lutdc ,HI_U32 motion_edge_lutdb ,HI_U32 motion_edge_lutda ,HI_U32 motion_edge_lutd9 ,HI_U32 motion_edge_lutd8 );
extern HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_29(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lute7 ,HI_U32 motion_edge_lute6 ,HI_U32 motion_edge_lute5 ,HI_U32 motion_edge_lute4 ,HI_U32 motion_edge_lute3 ,HI_U32 motion_edge_lute2 ,HI_U32 motion_edge_lute1 ,HI_U32 motion_edge_lute0 );
extern HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_30(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lutef ,HI_U32 motion_edge_lutee ,HI_U32 motion_edge_luted ,HI_U32 motion_edge_lutec ,HI_U32 motion_edge_luteb ,HI_U32 motion_edge_lutea ,HI_U32 motion_edge_lute9 ,HI_U32 motion_edge_lute8 );
extern HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_31(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lutf7 ,HI_U32 motion_edge_lutf6 ,HI_U32 motion_edge_lutf5 ,HI_U32 motion_edge_lutf4 ,HI_U32 motion_edge_lutf3 ,HI_U32 motion_edge_lutf2 ,HI_U32 motion_edge_lutf1 ,HI_U32 motion_edge_lutf0 );
extern HI_VOID VPSS_DRV_Set_MOTION_EDGE_LUT_32(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_edge_lutff ,HI_U32 motion_edge_lutfe ,HI_U32 motion_edge_lutfd ,HI_U32 motion_edge_lutfc ,HI_U32 motion_edge_lutfb ,HI_U32 motion_edge_lutfa ,HI_U32 motion_edge_lutf9 ,HI_U32 motion_edge_lutf8 );
extern HI_VOID VPSS_DRV_Set_nx1_vc1_en(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nx1_vc1_en );
extern HI_VOID VPSS_DRV_Set_nx2_vc1_en(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nx2_vc1_en );
extern HI_VOID VPSS_DRV_Set_prot(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 prot );
extern HI_VOID VPSS_DRV_Set_ref_vc1_en(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ref_vc1_en );
extern HI_VOID VPSS_DRV_Set_rfr_en(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 rfr_en );
extern HI_VOID VPSS_DRV_Set_rgme_en(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 rgme_en );
extern HI_VOID VPSS_DRV_Set_rotate_en(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 rotate_en );
extern HI_VOID VPSS_DRV_Set_SMMU_CB_TTBR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cb_ttbr );
extern HI_VOID VPSS_DRV_Set_SMMU_ERR_RDADDR_NS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 err_ns_rd_addr );
extern HI_VOID VPSS_DRV_Set_SMMU_ERR_RDADDR_S(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 err_s_rd_addr );
extern HI_VOID VPSS_DRV_Set_SMMU_ERR_WRADDR_NS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 err_ns_wr_addr );
extern HI_VOID VPSS_DRV_Set_SMMU_ERR_WRADDR_S(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 err_s_wr_addr );
extern HI_VOID VPSS_DRV_Set_SMMU_FAULT_ADDR_PTW_NS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 fault_addr_ptw_ns );
extern HI_VOID VPSS_DRV_Set_SMMU_FAULT_ADDR_PTW_S(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 fault_addr_ptw_s );
extern HI_VOID VPSS_DRV_Set_SMMU_FAULT_ADDR_RD_NS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 fault_addr_rd_ns );
extern HI_VOID VPSS_DRV_Set_SMMU_FAULT_ADDR_RD_S(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 fault_addr_rd_s );
extern HI_VOID VPSS_DRV_Set_SMMU_FAULT_ADDR_WR_NS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 fault_addr_wr_ns );
extern HI_VOID VPSS_DRV_Set_SMMU_FAULT_ADDR_WR_S(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 fault_addr_wr_s );
extern HI_VOID VPSS_DRV_Set_SMMU_FAULT_ID_PTW_NS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 fault_strm_id_ns ,HI_U32 
         fault_index_id_ns );
extern HI_VOID VPSS_DRV_Set_SMMU_FAULT_ID_PTW_S(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 fault_strm_id_s ,HI_U32 
         fault_index_id_s );
extern HI_VOID VPSS_DRV_Set_SMMU_FAULT_ID_RD_NS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 fault_str_id_rd_ns ,HI_U32 
         fault_index_id_rd_ns );
extern HI_VOID VPSS_DRV_Set_SMMU_FAULT_ID_RD_S(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 fault_str_id_rd_s ,HI_U32 
         fault_index_id_rd_s );
extern HI_VOID VPSS_DRV_Set_SMMU_FAULT_ID_WR_NS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 fault_str_id_wr_ns ,HI_U32 
         fault_index_id_wr_ns );
extern HI_VOID VPSS_DRV_Set_SMMU_FAULT_ID_WR_S(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 fault_str_id_wr_s ,HI_U32 
         fault_index_id_wr_s );
extern HI_VOID VPSS_DRV_Set_SMMU_FAULT_PTW_NUM(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 fault_ns_ptw_num ,HI_U32 
         fault_s_ptw_num );
extern HI_VOID VPSS_DRV_Set_SMMU_FAULT_TBU_DBG(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 read_command_counter ,HI_U32 
         arch_stall_n ,HI_U32 tbu_arreadym ,HI_U32 ar_readys ,HI_U32 ar_valids ,HI_U32 write_command_counter ,HI_U32 
         awch_stall_n ,HI_U32 tbu_awreadym ,HI_U32 aw_readys ,HI_U32 aw_valids );
extern HI_VOID VPSS_DRV_Set_SMMU_FAULT_TBU_INFO(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 fault_tbu_num ,HI_U32 
         fault_tlbinvalid_err_ns ,HI_U32 fault_tlbmiss_err_ns ,HI_U32 fault_tlbinvalid_err_s ,HI_U32 fault_tlbmiss_err_s );
extern HI_VOID VPSS_DRV_Set_SMMU_FAULT_TLB_RD_NS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 fault_tlb_rd_ns );
extern HI_VOID VPSS_DRV_Set_SMMU_FAULT_TLB_RD_S(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 fault_tlb_rd_s );
extern HI_VOID VPSS_DRV_Set_SMMU_FAULT_TLB_WR_NS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 fault_tlb_wr_ns );
extern HI_VOID VPSS_DRV_Set_SMMU_FAULT_TLB_WR_S(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 fault_tlb_wr_s );
extern HI_VOID VPSS_DRV_Set_SMMU_INTCLR_NS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 intns_tlbinvalid_wr_clr ,HI_U32 
         intns_tlbinvalid_rd_clr ,HI_U32 intns_ptw_trans_clr ,HI_U32 intns_tlbmiss_clr );
extern HI_VOID VPSS_DRV_Set_SMMU_INTCLR_S(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ints_tlbinvalid_wr_clr ,HI_U32 
         ints_tlbinvalid_rd_clr ,HI_U32 ints_ptw_trans_clr ,HI_U32 ints_tlbmiss_clr );
extern HI_VOID VPSS_DRV_Set_SMMU_INTMASK_NS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 intns_tlbinvalid_wr_msk ,HI_U32 
         intns_tlbinvalid_rd_msk ,HI_U32 intns_ptw_trans_msk ,HI_U32 intns_tlbmiss_msk );
extern HI_VOID VPSS_DRV_Set_SMMU_INTMASK_S(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ints_tlbinvalid_wr_msk ,HI_U32 
         ints_tlbinvalid_rd_msk ,HI_U32 ints_ptw_trans_msk ,HI_U32 ints_tlbmiss_msk );
extern HI_VOID VPSS_DRV_Set_SMMU_INTRAW_NS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 intns_tlbinvalid_wr_raw ,HI_U32 
         intns_tlbinvalid_rd_raw ,HI_U32 intns_ptw_trans_raw ,HI_U32 intns_tlbmiss_raw );
extern HI_VOID VPSS_DRV_Set_SMMU_INTRAW_S(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ints_tlbinvalid_wr_raw ,HI_U32 
         ints_tlbinvalid_rd_raw ,HI_U32 ints_ptw_trans_raw ,HI_U32 ints_tlbmiss_raw );
extern HI_VOID VPSS_DRV_Set_SMMU_INTSTAT_NS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 intns_tlbinvalid_wr_stat ,HI_U32 
         intns_tlbinvalid_rd_stat ,HI_U32 intns_ptw_trans_stat ,HI_U32 intns_tlbmiss_stat );
extern HI_VOID VPSS_DRV_Set_SMMU_INTSTAT_S(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ints_tlbinvalid_wr_stat ,HI_U32 
         ints_tlbinvalid_rd_stat ,HI_U32 ints_ptw_trans_stat ,HI_U32 ints_tlbmiss_stat );
extern HI_VOID VPSS_DRV_Set_SMMU_LP_CTRL(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 auto_clk_gt_en );
extern HI_VOID VPSS_DRV_Set_SMMU_MASTER_DBG0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 in_out_cmd_cnt_rd ,HI_U32 rdy_debug_rd ,
         HI_U32 vld_debug_rd ,HI_U32 cur_miss_cnt_rd ,HI_U32 last_miss_cnt_rd );
extern HI_VOID VPSS_DRV_Set_SMMU_MASTER_DBG1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 in_out_cmd_cnt_wr ,HI_U32 rdy_debug_wr ,
         HI_U32 vld_debug_wr ,HI_U32 cur_miss_cnt_wr ,HI_U32 last_miss_cnt_wr );
extern HI_VOID VPSS_DRV_Set_SMMU_MASTER_DBG2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cur_double_upd_cnt_rd ,HI_U32 
         last_double_upd_cnt_rd ,HI_U32 cur_double_miss_cnt_rd ,HI_U32 last_double_miss_cnt_rd );
extern HI_VOID VPSS_DRV_Set_SMMU_MASTER_DBG3(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 mst_fsm_cur ,HI_U32 cur_double_upd_cnt_wr 
         ,HI_U32 last_double_upd_cnt_wr ,HI_U32 cur_double_miss_cnt_wr ,HI_U32 last_double_miss_cnt_wr );
extern HI_VOID VPSS_DRV_Set_SMMU_MASTER_DBG4(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cur_sel1_chn_miss_cnt_rd ,HI_U32 
         last_sel1_chn_miss_cnt_rd );
extern HI_VOID VPSS_DRV_Set_SMMU_MASTER_DBG5(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cur_sel2_chn_miss_cnt_rd ,HI_U32 
         last_sel2_chn_miss_cnt_rd );
extern HI_VOID VPSS_DRV_Set_SMMU_MASTER_DBG6(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cur_sel1_chn_miss_cnt_wr ,HI_U32 
         last_sel1_chn_miss_cnt_wr );
extern HI_VOID VPSS_DRV_Set_SMMU_MASTER_DBG7(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cur_sel2_chn_miss_cnt_wr ,HI_U32 
         last_sel2_chn_miss_cnt_wr );
extern HI_VOID VPSS_DRV_Set_SMMU_MASTER_DBG8(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 sel1_chn_rd ,HI_U32 sel2_chn_rd ,HI_U32 
         sel1_chn_wr ,HI_U32 sel2_chn_wr );
extern HI_VOID VPSS_DRV_Set_SMMU_PREF_BUFFER_EMPTY(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 pref_buffer_empty );
extern HI_VOID VPSS_DRV_Set_SMMU_PTWQ_IDLE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ptwq15_idle_state ,HI_U32 ptwq14_idle_state 
         ,HI_U32 ptwq13_idle_state ,HI_U32 ptwq12_idle_state ,HI_U32 ptwq11_idle_state ,HI_U32 ptwq10_idle_state ,HI_U32 
         ptwq9_idle_state ,HI_U32 ptwq8_idle_state ,HI_U32 ptwq7_idle_state ,HI_U32 ptwq6_idle_state ,HI_U32 ptwq5_idle_state ,
         HI_U32 ptwq4_idle_state ,HI_U32 ptwq3_idle_state ,HI_U32 ptwq2_idle_state ,HI_U32 ptwq1_idle_state ,HI_U32 
         ptwq0_idle_state );
extern HI_VOID VPSS_DRV_Set_SMMU_RESET_STATE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 smmu_rst_state );
extern HI_VOID VPSS_DRV_Set_SMMU_SCB_TTBR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 scb_ttbr );
extern HI_VOID VPSS_DRV_Set_SMMU_SCR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ptw_pf ,HI_U32 int_en ,HI_U32 glb_bypass );
extern HI_VOID VPSS_DRV_Set_snr_en(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 snr_en );
extern HI_VOID VPSS_DRV_Set_tnr_en(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 tnr_en );
extern HI_VOID VPSS_DRV_Set_vhd0_crop_en(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_crop_en );
extern HI_VOID VPSS_DRV_Set_vhd0_en(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_en );
extern HI_VOID VPSS_DRV_Set_vhd0_flip(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_flip);
extern HI_VOID VPSS_DRV_Set_vhd0_lba_en(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_lba_en );
extern HI_VOID VPSS_DRV_Set_vhd0_mirror(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_mirror);
extern HI_VOID VPSS_DRV_Set_vhd0_pre_vfir_en(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_pre_vfir_en );
extern HI_VOID VPSS_DRV_Set_vhd0_uv_invert(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_uv_invert);
extern HI_VOID VPSS_DRV_Set_VPSS_BLKMVCURADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 blkmv_cur_addr );
extern HI_VOID VPSS_DRV_Set_VPSS_BLKMVNX1ADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 blkmv_nx1_addr );
extern HI_VOID VPSS_DRV_Set_VPSS_BLKMVNX1STRIDE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 blkmv_nx1_stride );
extern HI_VOID VPSS_DRV_Set_VPSS_BLKMVNX2ADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 blkmv_nx2_addr );
extern HI_VOID VPSS_DRV_Set_VPSS_BLKMVNX2STRIDE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 blkmv_nx2_stride );
extern HI_VOID VPSS_DRV_Set_VPSS_BLKMVREFADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 blkmv_ref_addr );
extern HI_VOID VPSS_DRV_Set_VPSS_BLKMVSTRIDE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 blkmv_cur_stride ,HI_U32 blkmv_ref_stride );
extern HI_VOID VPSS_DRV_Set_VPSS_BUS_CTRL(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 b128_split_en ,HI_U32 b256_split_en );
extern HI_VOID VPSS_DRV_Set_VPSS_CHK_SUM_SEL(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 winfo_sum_sel ,HI_U32 rinfo_sum_sel ,HI_U32 rch3_sum_sel ,HI_U32 rch2_sum_sel ,HI_U32 rch1_sum_sel ,HI_U32 rch0_sum_sel );
extern HI_VOID VPSS_DRV_Set_VPSS_CHK_SUM_W_ADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 chk_sum_w_addr );
extern HI_VOID VPSS_DRV_Set_VPSS_CMD_CNT_CHN(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmd_cnt_rd_chn ,HI_U32 cmdlen_cnt_rd_chn ,HI_U32 cmd_cnt_wr_chn ,HI_U32 cmdlen_cnt_wr_chn );
extern HI_VOID VPSS_DRV_Set_VPSS_CTRL(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 bfield ,HI_U32 bfield_mode ,HI_U32 bfield_first ,HI_U32 img_pro_mode ,HI_U32 igbm_en ,HI_U32 ifmd_en ,HI_U32 prot ,HI_U32 rotate_en ,HI_U32 four_pix_en ,HI_U32 rfr_en ,HI_U32 tnr_en ,HI_U32 snr_en ,HI_U32 ref_vc1_en ,HI_U32 cur_vc1_en ,HI_U32 blk_det_en ,HI_U32 nx1_vc1_en ,HI_U32 rgme_en ,HI_U32 nx2_vc1_en ,HI_U32 mcdi_en ,HI_U32 dei_en ,HI_U32 chk_sum_en ,HI_U32 vhd0_en );
extern HI_VOID VPSS_DRV_Set_VPSS_CTRL2(S_VPSS_REGS_TYPE *pstVpssRegs );
extern HI_VOID VPSS_DRV_Set_VPSS_CTRL3(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_lba_en ,HI_U32 vhd0_crop_en ,HI_U32 vhd0_pre_vfir_en ,HI_U32 zme_vhd0_en );
extern HI_VOID VPSS_DRV_Set_VPSS_CUECADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cuec_addr );
extern HI_VOID VPSS_DRV_Set_VPSS_CUESTRIDE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cuec_stride ,HI_U32 cuey_stride );
extern HI_VOID VPSS_DRV_Set_VPSS_CUEYADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cuey_addr );
extern HI_VOID VPSS_DRV_Set_VPSS_CUE_CTRL(S_VPSS_REGS_TYPE *pstVpssRegs );
extern HI_VOID VPSS_DRV_Set_VPSS_CURCADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 curc_addr );
extern HI_VOID VPSS_DRV_Set_VPSS_CURSTRIDE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 curc_stride ,HI_U32 cury_stride );
extern HI_VOID VPSS_DRV_Set_VPSS_CURYADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cury_addr );
extern HI_VOID VPSS_DRV_Set_VPSS_CUR_CTRL(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cur_ver_offset ,HI_U32 cur_hor_offset ,HI_U32 cur_tile_format );
extern HI_VOID VPSS_DRV_Set_VPSS_CUR_VC1_CTRL0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cur_vc1_rangedfrm ,HI_U32 cur_vc1_profile ,HI_U32 cur_vc1_mapyflg ,HI_U32 cur_vc1_mapcflg ,HI_U32 cur_vc1_mapy ,HI_U32 cur_vc1_mapc );
extern HI_VOID VPSS_DRV_Set_VPSS_DBD_BLKSIZE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dbd_hy_max_blk_size ,HI_U32 dbd_min_blk_size );
extern HI_VOID VPSS_DRV_Set_VPSS_DBD_FLAT(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dbd_thr_flat );
extern HI_VOID VPSS_DRV_Set_VPSS_DBD_HYRDX8BINLUT0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dbd_hy_rdx8bin_lut1 ,HI_U32 dbd_hy_rdx8bin_lut0 );
extern HI_VOID VPSS_DRV_Set_VPSS_DBD_HYRDX8BINLUT1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dbd_hy_rdx8bin_lut3 ,HI_U32 dbd_hy_rdx8bin_lut2 );
extern HI_VOID VPSS_DRV_Set_VPSS_DBD_HYRDX8BINLUT2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dbd_hy_rdx8bin_lut5 ,HI_U32 dbd_hy_rdx8bin_lut4 );
extern HI_VOID VPSS_DRV_Set_VPSS_DBD_HYRDX8BINLUT3(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dbd_hy_rdx8bin_lut6 );
extern HI_VOID VPSS_DRV_Set_VPSS_DBD_LUTWGT0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dbd_det_lut_wgt7 ,HI_U32 dbd_det_lut_wgt6 ,HI_U32 dbd_det_lut_wgt5 ,HI_U32 dbd_det_lut_wgt4 ,HI_U32 dbd_det_lut_wgt3 ,HI_U32 dbd_det_lut_wgt2 ,HI_U32 dbd_det_lut_wgt1 ,HI_U32 dbd_det_lut_wgt0 );
extern HI_VOID VPSS_DRV_Set_VPSS_DBD_LUTWGT1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dbd_det_lut_wgt15 ,HI_U32 dbd_det_lut_wgt14 ,HI_U32 dbd_det_lut_wgt13 ,HI_U32 dbd_det_lut_wgt12 ,HI_U32 dbd_det_lut_wgt11 ,HI_U32 dbd_det_lut_wgt10 ,HI_U32 dbd_det_lut_wgt9 ,HI_U32 dbd_det_lut_wgt8 );
extern HI_VOID VPSS_DRV_Set_VPSS_DBD_RTNBDTXT(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dbd_hy_rtn_bd_txt );
extern HI_VOID VPSS_DRV_Set_VPSS_DBD_THDEDGE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dbd_hy_thd_edge );
extern HI_VOID VPSS_DRV_Set_VPSS_DBD_THDTXT(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dbd_hy_thd_txt );
extern HI_VOID VPSS_DRV_Set_VPSS_DBD_TSTBLKNUM(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dbd_hy_tst_blk_num );
extern HI_VOID VPSS_DRV_Set_VPSS_DBM_DEMO(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dbm_demo_pos_x );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_BLK(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 db_lum_h_blk_size );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_0 );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_1 );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG10(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_10 );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG11(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_11 );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG12(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_12 );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG13(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_13 );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG14(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_14 );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG15(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_15 );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG16(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_16 );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG17(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_17 );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG18(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_18 );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG19(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_19 );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_2 );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG20(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_20 );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG21(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_21 );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG22(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_22 );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG23(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_23 );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG24(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_24 );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG25(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_25 );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG26(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_26 );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG27(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_27 );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG28(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_28 );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG29(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_29 );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG3(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_3 );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG30(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_30 );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG31(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_31 );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG4(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_4 );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG5(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_5 );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG6(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_6 );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG7(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_7 );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG8(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_8 );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_BORD_FLAG9(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vpss_db_bord_flag_9 );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_CTRL(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dbm_out_mode ,HI_U32 det_hy_en ,HI_U32 dbm_demo_mode ,HI_U32 dbm_demo_en ,HI_U32 db_lum_hor_en ,HI_U32 nr_en ,HI_U32 dm_en ,HI_U32 db_en );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_CTRL_db(S_VPSS_REGS_TYPE *pstVpssRegs , HI_U32 db_en );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_CTRL_dm(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dm_en );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_CTRL_nr(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nr_en );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_CTRS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 db_max_lum_hor_db_dist ,HI_U32 db_ctrst_adj_gain2 ,HI_U32 db_ctrst_adj_gain1 ,HI_U32 db_dir_smooth_mode ,HI_U32 db_ctrst_adj_core );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_DIR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 db_ctrst_thresh ,HI_U32 db_grad_sub_ratio );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_LHHF(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 db_lum_hor_hf_var_gain2 ,HI_U32 db_lum_hor_hf_var_gain1 ,HI_U32 db_lum_hor_hf_var_core ,HI_U32 db_lum_hor_hf_diff_gain2 ,HI_U32 db_lum_hor_hf_diff_gain1 ,HI_U32 db_lum_hor_hf_diff_core );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_LUT0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 db_lum_hor_delta_lut_p7 ,HI_U32 db_lum_hor_delta_lut_p6 ,HI_U32 db_lum_hor_delta_lut_p5 ,HI_U32 db_lum_hor_delta_lut_p4 ,HI_U32 db_lum_hor_delta_lut_p3 ,HI_U32 db_lum_hor_delta_lut_p2 ,HI_U32 db_lum_hor_delta_lut_p1 ,HI_U32 db_lum_hor_delta_lut_p0 );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_LUT1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 db_lum_hor_delta_lut_p15 ,HI_U32 db_lum_hor_delta_lut_p14 ,HI_U32 db_lum_hor_delta_lut_p13 ,HI_U32 db_lum_hor_delta_lut_p12 ,HI_U32 db_lum_hor_delta_lut_p11 ,HI_U32 db_lum_hor_delta_lut_p10 ,HI_U32 db_lum_hor_delta_lut_p9 ,HI_U32 db_lum_hor_delta_lut_p8 );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_LUT2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 db_lum_h_str_fade_lut_p9 ,HI_U32 db_lum_h_str_fade_lut_p8 ,HI_U32 db_lum_h_str_fade_lut_p7 ,HI_U32 db_lum_h_str_fade_lut_p6 ,HI_U32 db_lum_h_str_fade_lut_p5 ,HI_U32 db_lum_h_str_fade_lut_p4 ,HI_U32 db_lum_h_str_fade_lut_p3 ,HI_U32 db_lum_h_str_fade_lut_p2 ,HI_U32 db_lum_h_str_fade_lut_p1 ,HI_U32 db_lum_h_str_fade_lut_p0 );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_LUT3(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 db_lum_h_str_fade_lut_p11 ,HI_U32 db_lum_h_str_fade_lut_p10 );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_LUT4(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 db_dir_str_gain_lut_p7 ,HI_U32 db_dir_str_gain_lut_p6 ,HI_U32 db_dir_str_gain_lut_p5 ,HI_U32 db_dir_str_gain_lut_p4 ,HI_U32 db_dir_str_gain_lut_p3 ,HI_U32 db_dir_str_gain_lut_p2 ,HI_U32 db_dir_str_gain_lut_p1 ,HI_U32 db_dir_str_gain_lut_p0 );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_LUT5(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 db_dir_str_lut_p7 ,HI_U32 db_dir_str_lut_p6 ,HI_U32 db_dir_str_lut_p5 ,HI_U32 db_dir_str_lut_p4 ,HI_U32 db_dir_str_lut_p3 ,HI_U32 db_dir_str_lut_p2 ,HI_U32 db_dir_str_lut_p1 ,HI_U32 db_dir_str_lut_p0 );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_LUT6(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 db_dir_str_lut_p15 ,HI_U32 db_dir_str_lut_p14 ,HI_U32 db_dir_str_lut_p13 ,HI_U32 db_dir_str_lut_p12 ,HI_U32 db_dir_str_lut_p11 ,HI_U32 db_dir_str_lut_p10 ,HI_U32 db_dir_str_lut_p9 ,HI_U32 db_dir_str_lut_p8 );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_LVHF(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 db_lum_hor_bord_adj_gain ,HI_U32 db_lum_hor_adj_gain );
extern HI_VOID VPSS_DRV_Set_VPSS_DB_RATIO(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 db_lum_hor_txt_win_size ,HI_U32 db_global_db_strenth_lum ,HI_U32 db_lum_hor_filter_sel ,HI_U32 db_lum_hor_scale_ratio );
extern HI_VOID VPSS_DRV_Set_VPSS_DEI_ADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dei_cfg_addr );
extern HI_VOID VPSS_DRV_Set_VPSS_DIECHECK1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 bc1_gain ,HI_U32 bc1_autodz_gain ,HI_U32 bc1_max_dz );
extern HI_VOID VPSS_DRV_Set_VPSS_DIECHECK2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 bc2_gain ,HI_U32 bc2_autodz_gain ,HI_U32 bc2_max_dz );
extern HI_VOID VPSS_DRV_Set_VPSS_DIECOMBCHK0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 comb_chk_upper_limit ,HI_U32 comb_chk_lower_limit ,HI_U32 comb_chk_min_vthd ,HI_U32 comb_chk_min_hthd );
extern HI_VOID VPSS_DRV_Set_VPSS_DIECOMBCHK1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 comb_chk_en ,HI_U32 comb_chk_md_thd ,HI_U32 comb_chk_edge_thd );
extern HI_VOID VPSS_DRV_Set_VPSS_DIECTRL(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 die_out_sel_l ,HI_U32 die_out_sel_c ,HI_U32 die_l_mode ,HI_U32 die_c_mode ,HI_U32 mc_only ,HI_U32 ma_only ,HI_U32 edge_smooth_en ,HI_U32 mchdir_l ,HI_U32 mchdir_c ,HI_U32 die_rst ,HI_U32 stinfo_stop ,HI_U32 edge_smooth_ratio );
extern HI_VOID VPSS_DRV_Set_VPSS_DIECTRL_lc_mode(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 die_l_mode ,HI_U32 die_c_mode );
extern HI_VOID VPSS_DRV_Set_VPSS_DIECTRL_outsel(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 die_out_sel_l ,HI_U32 die_out_sel_c );
extern HI_VOID VPSS_DRV_Set_VPSS_DIEDIR0_3(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dir3_scale ,HI_U32 dir2_scale ,HI_U32 dir1_scale ,HI_U32 dir0_scale );
extern HI_VOID VPSS_DRV_Set_VPSS_DIEDIR12_14(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dir14_scale ,HI_U32 dir13_scale ,HI_U32 dir12_scale );
extern HI_VOID VPSS_DRV_Set_VPSS_DIEDIR4_7(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dir7_scale ,HI_U32 dir6_scale ,HI_U32 dir5_scale ,HI_U32 dir4_scale );
extern HI_VOID VPSS_DRV_Set_VPSS_DIEDIR8_11(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dir11_scale ,HI_U32 dir10_scale ,HI_U32 dir9_scale ,HI_U32 dir8_scale );
extern HI_VOID VPSS_DRV_Set_VPSS_DIEDIRTHD(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 strength_thd ,HI_U32 hor_edge_en ,HI_U32 edge_mode ,HI_U32 dir_thd ,HI_U32 bc_gain );
extern HI_VOID VPSS_DRV_Set_VPSS_DIEEDGEFORMC(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 inter_diff_thd2 ,HI_U32 edge_scale ,HI_U32 edge_coring );
extern HI_VOID VPSS_DRV_Set_VPSS_DIEEDGENORM0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 edge_norm_1 ,HI_U32 edge_norm_0 );
extern HI_VOID VPSS_DRV_Set_VPSS_DIEEDGENORM1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 edge_norm_3 ,HI_U32 edge_norm_2 );
extern HI_VOID VPSS_DRV_Set_VPSS_DIEEDGENORM2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 mc_strength_k3 ,HI_U32 edge_norm_5 ,HI_U32 edge_norm_4 );
extern HI_VOID VPSS_DRV_Set_VPSS_DIEEDGENORM3(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 mc_strength_g3 ,HI_U32 edge_norm_7 ,HI_U32 edge_norm_6 );
extern HI_VOID VPSS_DRV_Set_VPSS_DIEEDGENORM4(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 inter_diff_thd0 ,HI_U32 edge_norm_9 ,HI_U32 edge_norm_8 );
extern HI_VOID VPSS_DRV_Set_VPSS_DIEEDGENORM5(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 inter_diff_thd1 ,HI_U32 edge_norm_11 ,HI_U32 edge_norm_10 );
extern HI_VOID VPSS_DRV_Set_VPSS_DIEFLDMTN(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 long_motion_shf ,HI_U32 fld_motion_wnd_mode ,HI_U32 fld_motion_gain ,HI_U32 fld_motion_curve_slope ,HI_U32 fld_motion_thd_high ,HI_U32 fld_motion_thd_low );
extern HI_VOID VPSS_DRV_Set_VPSS_DIEFRMFLDBLENDRAT(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 frame_field_blend_ratio4 ,HI_U32 frame_field_blend_ratio3 ,HI_U32 frame_field_blend_ratio2 ,HI_U32 frame_field_blend_ratio1 );
extern HI_VOID VPSS_DRV_Set_VPSS_DIEFRMFLDBLENDSLP(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 frame_field_blend_ratio0 ,HI_U32 frame_field_blend_slope3 ,HI_U32 frame_field_blend_slope2 ,HI_U32 frame_field_blend_slope1 ,HI_U32 frame_field_blend_slope0 );
extern HI_VOID VPSS_DRV_Set_VPSS_DIEFRMFLDBLENDTHD0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 frame_field_blend_thd3 ,HI_U32 frame_field_blend_thd2 ,HI_U32 frame_field_blend_thd1 ,HI_U32 frame_field_blend_thd0 );
extern HI_VOID VPSS_DRV_Set_VPSS_DIEFRMFLDBLENDTHD1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 frame_field_blend_ratio_max ,HI_U32 frame_field_blend_ratio_min ,HI_U32 frame_field_blend_thd5 ,HI_U32 frame_field_blend_thd4 );
extern HI_VOID VPSS_DRV_Set_VPSS_DIEFRMMTNSMTHRAT(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 frame_motion_smooth_ratio4 ,HI_U32 frame_motion_smooth_ratio3 ,HI_U32 frame_motion_smooth_ratio2 ,HI_U32 frame_motion_smooth_ratio1 );
extern HI_VOID VPSS_DRV_Set_VPSS_DIEFRMMTNSMTHSLP(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 frame_motion_smooth_ratio0 ,HI_U32 frame_motion_smooth_slope3 ,HI_U32 frame_motion_smooth_slope2 ,HI_U32 frame_motion_smooth_slope1 ,HI_U32 frame_motion_smooth_slope0 );
extern HI_VOID VPSS_DRV_Set_VPSS_DIEFRMMTNSMTHTHD0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 frame_motion_smooth_thd3 ,HI_U32 frame_motion_smooth_thd2 ,HI_U32 frame_motion_smooth_thd1 ,HI_U32 frame_motion_smooth_thd0 );
extern HI_VOID VPSS_DRV_Set_VPSS_DIEFRMMTNSMTHTHD1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 frame_motion_smooth_ratio_max ,HI_U32 frame_motion_smooth_ratio_min ,HI_U32 frame_motion_smooth_thd5 ,HI_U32 frame_motion_smooth_thd4 );
extern HI_VOID VPSS_DRV_Set_VPSS_DIEFUSION0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 k_c_mcbld ,HI_U32 k_c_mcw ,HI_U32 k_y_mcbld ,HI_U32 k_y_mcw );
extern HI_VOID VPSS_DRV_Set_VPSS_DIEFUSION1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 g0_mcw_adj ,HI_U32 k0_mcw_adj ,HI_U32 x0_mcw_adj );
extern HI_VOID VPSS_DRV_Set_VPSS_DIEFUSION2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 k1_mcbld ,HI_U32 k0_mcbld ,HI_U32 x0_mcbld ,HI_U32 k1_mcw_adj );
extern HI_VOID VPSS_DRV_Set_VPSS_DIEFUSION3(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 mc_lai_bldmode ,HI_U32 k_curw_mcbld ,HI_U32 g0_mcbld );
extern HI_VOID VPSS_DRV_Set_VPSS_DIEHISMODE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 his_motion_info_write_mode ,HI_U32 his_motion_write_mode ,HI_U32 his_motion_using_mode ,HI_U32 his_motion_en ,HI_U32 pre_info_en ,HI_U32 ppre_info_en ,HI_U32 rec_mode_frm_motion_step_1 ,HI_U32 rec_mode_frm_motion_step_0 ,HI_U32 rec_mode_fld_motion_step_1 ,HI_U32 rec_mode_fld_motion_step_0 );
extern HI_VOID VPSS_DRV_Set_VPSS_DIEHISMODE_pre_info_en(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 pre_info_en );
extern HI_VOID VPSS_DRV_Set_VPSS_DIEINTEN(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 min_north_strength ,HI_U32 dir_ratio_north );
extern HI_VOID VPSS_DRV_Set_VPSS_DIEINTPSCL0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 intp_scale_ratio_8 ,HI_U32 intp_scale_ratio_7 ,HI_U32 intp_scale_ratio_6 ,HI_U32 intp_scale_ratio_5 ,HI_U32 intp_scale_ratio_4 ,HI_U32 intp_scale_ratio_3 ,HI_U32 intp_scale_ratio_2 ,HI_U32 intp_scale_ratio_1 );
extern HI_VOID VPSS_DRV_Set_VPSS_DIEINTPSCL1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 intp_scale_ratio_15 ,HI_U32 intp_scale_ratio_14 ,HI_U32 intp_scale_ratio_13 ,HI_U32 intp_scale_ratio_12 ,HI_U32 intp_scale_ratio_11 ,HI_U32 intp_scale_ratio_10 ,HI_U32 intp_scale_ratio_9 );
extern HI_VOID VPSS_DRV_Set_VPSS_DIEJITMTN(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 fld_motion_coring ,HI_U32 jitter_coring ,HI_U32 jitter_gain );
extern HI_VOID VPSS_DRV_Set_VPSS_DIEKMAG1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 khoredge ,HI_U32 kmagv_2 ,HI_U32 kmagv_1 ,HI_U32 kmagh_2 ,HI_U32 kmagh_1 );
extern HI_VOID VPSS_DRV_Set_VPSS_DIEKMAG2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 gain_framemotion ,HI_U32 motion_limt_2 ,HI_U32 motion_limt_1 ,HI_U32 frame_mag_en ,HI_U32 gain_horedge );
extern HI_VOID VPSS_DRV_Set_VPSS_DIELMA2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 chroma_mf_offset ,HI_U32 dei_st_rst_value ,HI_U32 rec_mode_en ,HI_U32 chroma_mf_max ,HI_U32 luma_mf_max ,HI_U32 motion_iir_en ,HI_U32 luma_scesdf_max ,HI_U32 frame_motion_smooth_en ,HI_U32 recmode_frmfld_blend_mode );
extern HI_VOID VPSS_DRV_Set_VPSS_DIEMAGBMTHD0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ma_gbm_thd0 ,HI_U32 ma_gbm_thd1 );
extern HI_VOID VPSS_DRV_Set_VPSS_DIEMAGBMTHD1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ma_gbm_thd2 ,HI_U32 ma_gbm_thd3 );
extern HI_VOID VPSS_DRV_Set_VPSS_DIEMCGBMCOEF0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 mtfilten_gmd ,HI_U32 mtth3_gmd ,HI_U32 mtth2_gmd ,HI_U32 mtth1_gmd ,HI_U32 mtth0_gmd );
extern HI_VOID VPSS_DRV_Set_VPSS_DIEMCGBMCOEF1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 k_mag_gmd ,HI_U32 k_difh_gmd ,HI_U32 k_maxmag_gmd );
extern HI_VOID VPSS_DRV_Set_VPSS_DIEMCSTRENGTH0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 mc_strength_g0 ,HI_U32 mc_strength_k2 ,HI_U32 mc_strength_k1 ,HI_U32 mc_strength_k0 );
extern HI_VOID VPSS_DRV_Set_VPSS_DIEMCSTRENGTH1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 mc_strength_maxg ,HI_U32 mc_strength_ming ,HI_U32 mc_strength_g2 ,HI_U32 mc_strength_g1 );
extern HI_VOID VPSS_DRV_Set_VPSS_DIEMORFLT(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 med_blend_en ,HI_U32 mor_flt_en ,HI_U32 mor_flt_size ,HI_U32 mor_flt_thd );
extern HI_VOID VPSS_DRV_Set_VPSS_DIEMTNADJ(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_adjust_gain_chr ,HI_U32 motion_adjust_coring ,HI_U32 motion_adjust_gain );
extern HI_VOID VPSS_DRV_Set_VPSS_DIEMTNDIFFTHD0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_diff_thd_3 ,HI_U32 motion_diff_thd_2 ,HI_U32 motion_diff_thd_1 ,HI_U32 motion_diff_thd_0 );
extern HI_VOID VPSS_DRV_Set_VPSS_DIEMTNDIFFTHD1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 max_motion_iir_ratio ,HI_U32 min_motion_iir_ratio ,HI_U32 motion_diff_thd_5 ,HI_U32 motion_diff_thd_4 );
extern HI_VOID VPSS_DRV_Set_VPSS_DIEMTNIIRRAT(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_iir_curve_ratio_4 ,HI_U32 motion_iir_curve_ratio_3 ,HI_U32 motion_iir_curve_ratio_2 ,HI_U32 motion_iir_curve_ratio_1 );
extern HI_VOID VPSS_DRV_Set_VPSS_DIEMTNIIRSLP(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motion_iir_curve_ratio_0 ,HI_U32 motion_iir_curve_slope_3 ,HI_U32 motion_iir_curve_slope_2 ,HI_U32 motion_iir_curve_slope_1 ,HI_U32 motion_iir_curve_slope_0 );
extern HI_VOID VPSS_DRV_Set_VPSS_DIESCALE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 range_scale );
extern HI_VOID VPSS_DRV_Set_VPSS_DIESTA(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 c_height_cnt ,HI_U32 l_height_cnt ,HI_U32 cur_cstate ,HI_U32 cur_state );
extern HI_VOID VPSS_DRV_Set_VPSS_DM_DIR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dm_opp_ang_ctrst_div ,HI_U32 dm_opp_ang_ctrst_t ,HI_U32 dm_ctrst_thresh ,HI_U32 dm_grad_sub_ratio );
extern HI_VOID VPSS_DRV_Set_VPSS_DM_DIRC(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dm_mmf_limit_en ,HI_U32 dm_mndir_opp_ctrst_t );
extern HI_VOID VPSS_DRV_Set_VPSS_DM_DIRO(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dm_csw_trsnt_st_10 );
extern HI_VOID VPSS_DRV_Set_VPSS_DM_EDGE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dm_global_str ,HI_U32 dm_init_val_step ,HI_U32 dm_mmf_set );
extern HI_VOID VPSS_DRV_Set_VPSS_DM_LSW(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dm_lsw_ratio ,HI_U32 dm_csw_trsnt_st ,HI_U32 dm_lw_ctrst_t_10 ,HI_U32 dm_lw_ctrst_t );
extern HI_VOID VPSS_DRV_Set_VPSS_DM_LUT0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dm_sw_wht_lut_p3 ,HI_U32 dm_sw_wht_lut_p2 ,HI_U32 dm_sw_wht_lut_p1 ,HI_U32 dm_sw_wht_lut_p0 );
extern HI_VOID VPSS_DRV_Set_VPSS_DM_LUT1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dm_limit_t_10 ,HI_U32 dm_limit_t ,HI_U32 dm_sw_wht_lut_p4 );
extern HI_VOID VPSS_DRV_Set_VPSS_DM_LUT10(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dm_trans_band_lut_p29 ,HI_U32 dm_trans_band_lut_p28 ,HI_U32 dm_trans_band_lut_p27 ,HI_U32 dm_trans_band_lut_p26 ,HI_U32 dm_trans_band_lut_p25 );
extern HI_VOID VPSS_DRV_Set_VPSS_DM_LUT11(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dm_trans_band_lut_p30 );
extern HI_VOID VPSS_DRV_Set_VPSS_DM_LUT2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dm_dir_str_gain_lut_p7 ,HI_U32 dm_dir_str_gain_lut_p6 ,HI_U32 dm_dir_str_gain_lut_p5 ,HI_U32 dm_dir_str_gain_lut_p4 ,HI_U32 dm_dir_str_gain_lut_p3 ,HI_U32 dm_dir_str_gain_lut_p2 ,HI_U32 dm_dir_str_gain_lut_p1 ,HI_U32 dm_dir_str_gain_lut_p0 );
extern HI_VOID VPSS_DRV_Set_VPSS_DM_LUT3(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dm_dir_str_lut_p7 ,HI_U32 dm_dir_str_lut_p6 ,HI_U32 dm_dir_str_lut_p5 ,HI_U32 dm_dir_str_lut_p4 ,HI_U32 dm_dir_str_lut_p3 ,HI_U32 dm_dir_str_lut_p2 ,HI_U32 dm_dir_str_lut_p1 ,HI_U32 dm_dir_str_lut_p0 );
extern HI_VOID VPSS_DRV_Set_VPSS_DM_LUT4(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dm_dir_str_lut_p15 ,HI_U32 dm_dir_str_lut_p14 ,HI_U32 dm_dir_str_lut_p13 ,HI_U32 dm_dir_str_lut_p12 ,HI_U32 dm_dir_str_lut_p11 ,HI_U32 dm_dir_str_lut_p10 ,HI_U32 dm_dir_str_lut_p9 ,HI_U32 dm_dir_str_lut_p8 );
extern HI_VOID VPSS_DRV_Set_VPSS_DM_LUT5(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dm_trans_band_lut_p4 ,HI_U32 dm_trans_band_lut_p3 ,HI_U32 dm_trans_band_lut_p2 ,HI_U32 dm_trans_band_lut_p1 ,HI_U32 dm_trans_band_lut_p0 );
extern HI_VOID VPSS_DRV_Set_VPSS_DM_LUT6(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dm_trans_band_lut_p9 ,HI_U32 dm_trans_band_lut_p8 ,HI_U32 dm_trans_band_lut_p7 ,HI_U32 dm_trans_band_lut_p6 ,HI_U32 dm_trans_band_lut_p5 );
extern HI_VOID VPSS_DRV_Set_VPSS_DM_LUT7(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dm_trans_band_lut_p14 ,HI_U32 dm_trans_band_lut_p13 ,HI_U32 dm_trans_band_lut_p12 ,HI_U32 dm_trans_band_lut_p11 ,HI_U32 dm_trans_band_lut_p10 );
extern HI_VOID VPSS_DRV_Set_VPSS_DM_LUT8(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dm_trans_band_lut_p19 ,HI_U32 dm_trans_band_lut_p18 ,HI_U32 dm_trans_band_lut_p17 ,HI_U32 dm_trans_band_lut_p16 ,HI_U32 dm_trans_band_lut_p15 );
extern HI_VOID VPSS_DRV_Set_VPSS_DM_LUT9(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dm_trans_band_lut_p24 ,HI_U32 dm_trans_band_lut_p23 ,HI_U32 dm_trans_band_lut_p22 ,HI_U32 dm_trans_band_lut_p21 ,HI_U32 dm_trans_band_lut_p20 );
extern HI_VOID VPSS_DRV_Set_VPSS_DM_MMFLR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dm_csw_trsnt_lt ,HI_U32 dm_mmf_lr_10 ,HI_U32 dm_mmf_lr );
extern HI_VOID VPSS_DRV_Set_VPSS_DM_MMFSR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dm_csw_trsnt_lt_10 ,HI_U32 dm_mmf_sr_10 ,HI_U32 dm_mmf_sr );
extern HI_VOID VPSS_DRV_Set_VPSS_DM_THR0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dm_dir_blend_str ,HI_U32 dm_lim_res_blend_str2 ,HI_U32 dm_lim_res_blend_str1 ,HI_U32 dm_limit_lsw_ratio );
extern HI_VOID VPSS_DRV_Set_VPSS_FTCONFIG(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 node_rst_en );
extern HI_VOID VPSS_DRV_Set_VPSS_IMGSIZE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 imgheight ,HI_U32 imgwidth );
extern HI_VOID VPSS_DRV_Set_VPSS_INTMASK(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 bus_r_err_mask ,HI_U32 dcmp_err_mask ,HI_U32 vhd0_tunl_mask ,HI_U32 eof_end_mask ,HI_U32 bus_w_err_mask ,HI_U32 timeout_mask ,HI_U32 eof_mask );
extern HI_VOID VPSS_DRV_Set_VPSS_IN_CTRL(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cts_bit_sel ,HI_U32 cts_en ,HI_U32 snr_mad_upfir_en ,HI_U32 uv_invert ,HI_U32 rfr_pix_bitw ,HI_U32 ref_nxt_pix_bitw ,HI_U32 in_pix_bitw ,HI_U32 in_format );
extern HI_VOID VPSS_DRV_Set_VPSS_IN_CTRL_CTS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cts_bit_sel ,HI_U32 cts_en );
extern HI_VOID VPSS_DRV_Set_VPSS_IN_CTRL_nx2(S_VPSS_REGS_TYPE *pstVpssRegs , HI_U32 in_pix_bitw ,HI_U32 in_format );
extern HI_VOID VPSS_DRV_Set_VPSS_IN_CTRL_ref(S_VPSS_REGS_TYPE *pstVpssRegs , HI_U32 ref_nxt_pix_bitw );
extern HI_VOID VPSS_DRV_Set_VPSS_IN_CTRL_rfr_bitw(S_VPSS_REGS_TYPE *pstVpssRegs , HI_U32 rfr_pix_bitw );
extern HI_VOID VPSS_DRV_Set_VPSS_MACCFG(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 mac_ch_prio );
extern HI_VOID VPSS_DRV_Set_VPSS_MADSTRIDE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 mad_stride );
extern HI_VOID VPSS_DRV_Set_VPSS_MAD_RADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 mad_raddr );
extern HI_VOID VPSS_DRV_Set_VPSS_MAD_WADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 mad_waddr );
extern HI_VOID VPSS_DRV_Set_VPSS_MCDI_DEMO(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 demo_border ,HI_U32 demo_mode_r ,HI_U32 demo_mode_l ,HI_U32 demo_en );
extern HI_VOID VPSS_DRV_Set_VPSS_MCDI_DLT0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 th_blkmvx_mvdlt ,HI_U32 th_rgmvx_mvdlt ,HI_U32 th_ls_mvdlt ,HI_U32 th_vblkdist_mvdlt ,HI_U32 th_hblkdist_mvdlt );
extern HI_VOID VPSS_DRV_Set_VPSS_MCDI_DLT1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 k_sadcore_mvdlt ,HI_U32 th_mag_mvdlt ,HI_U32 g_mag_mvdlt ,HI_U32 thl_sad_mvdlt ,HI_U32 thh_sad_mvdlt );
extern HI_VOID VPSS_DRV_Set_VPSS_MCDI_LCDINEW0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 mcendc ,HI_U32 mcendr ,HI_U32 mcstartc ,HI_U32 mcstartr );
extern HI_VOID VPSS_DRV_Set_VPSS_MCDI_LCDINEW1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 movegain ,HI_U32 movecorig ,HI_U32 movethdl ,HI_U32 movethdh );
extern HI_VOID VPSS_DRV_Set_VPSS_MCDI_MC0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 mclpf_mode ,HI_U32 k_pxlmag_mcw ,HI_U32 x_pxlmag_mcw ,HI_U32 rs_pxlmag_mcw ,HI_U32 gain_mclpfh ,HI_U32 gain_dn_mclpfv ,HI_U32 gain_up_mclpfv );
extern HI_VOID VPSS_DRV_Set_VPSS_MCDI_MC1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 g_pxlmag_mcw );
extern HI_VOID VPSS_DRV_Set_VPSS_MCDI_MC2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 k_c_vertw ,HI_U32 k_y_vertw );
extern HI_VOID VPSS_DRV_Set_VPSS_MCDI_MC3(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 k_fstmt_mc ,HI_U32 x_fstmt_mc ,HI_U32 k1_mv_mc ,HI_U32 x0_mv_mc ,HI_U32 bdv_mcpos ,HI_U32 bdh_mcpos ,HI_U32 k_delta );
extern HI_VOID VPSS_DRV_Set_VPSS_MCDI_MC4(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 k_hfcore_mc ,HI_U32 x_hfcore_mc ,HI_U32 g_slmt_mc ,HI_U32 k_slmt_mc ,HI_U32 x_slmt_mc ,HI_U32 g_fstmt_mc );
extern HI_VOID VPSS_DRV_Set_VPSS_MCDI_MC5(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 r0_mc ,HI_U32 c0_mc ,HI_U32 g_hfcore_mc );
extern HI_VOID VPSS_DRV_Set_VPSS_MCDI_MC6(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 mcmvrange ,HI_U32 r1_mc ,HI_U32 c1_mc );
extern HI_VOID VPSS_DRV_Set_VPSS_MCDI_MC7(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 k_frcount_mc ,HI_U32 x_frcount_mc ,HI_U32 scenechange_mc );
extern HI_VOID VPSS_DRV_Set_VPSS_MCDI_NUMT(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 mc_numt_blden ,HI_U32 numt_gain ,HI_U32 numt_coring ,HI_U32 numt_lpf_en );
extern HI_VOID VPSS_DRV_Set_VPSS_MCDI_RGDIFY(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 k_rgdifycore ,HI_U32 g_rgdifycore ,HI_U32 core_rgdify ,HI_U32 lmt_rgdify );
extern HI_VOID VPSS_DRV_Set_VPSS_MCDI_RGLS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 k_sad_rgls ,HI_U32 th_mag_rgls ,HI_U32 th_sad_rgls ,HI_U32 k_sadcore_rgmv );
extern HI_VOID VPSS_DRV_Set_VPSS_MCDI_RGMAG0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 core_mag_rg ,HI_U32 lmt_mag_rg );
extern HI_VOID VPSS_DRV_Set_VPSS_MCDI_RGMAG1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 core_mv_rgmvls ,HI_U32 k_mv_rgmvls ,HI_U32 core_mag_rgmvls ,HI_U32 k_mag_rgmvls ,HI_U32 th_mvadj_rgmvls ,HI_U32 en_mvadj_rgmvls );
extern HI_VOID VPSS_DRV_Set_VPSS_MCDI_RGMCW0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 k_rglsw ,HI_U32 k_simimvw ,HI_U32 gh_core_simimv ,HI_U32 gl_core_simimv ,HI_U32 k_core_simimv );
extern HI_VOID VPSS_DRV_Set_VPSS_MCDI_RGMCW1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 k_core_vsaddif ,HI_U32 k_rgsadadj_mcw ,HI_U32 core_rgsadadj_mcw ,HI_U32 k_mvy_mcw ,HI_U32 core_mvy_mcw ,HI_U32 rgtb_en_mcw );
extern HI_VOID VPSS_DRV_Set_VPSS_MCDI_RGMCW2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 core_rgmag_mcw ,HI_U32 mode_rgysad_mcw ,HI_U32 k_vsaddifw ,HI_U32 gh_core_vsad_dif ,HI_U32 gl_core_vsaddif );
extern HI_VOID VPSS_DRV_Set_VPSS_MCDI_RGMCW3(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 g0_rgmag_mcw ,HI_U32 k0_rgmag_mcw ,HI_U32 x0_rgmag_mcw );
extern HI_VOID VPSS_DRV_Set_VPSS_MCDI_RGMCW4(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 x0_rgsad_mcw ,HI_U32 core_rgsad_mcw ,HI_U32 k1_rgmag_mcw );
extern HI_VOID VPSS_DRV_Set_VPSS_MCDI_RGMCW5(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 k1_rgsad_mcw ,HI_U32 g0_rgsad_mcw ,HI_U32 k0_rgsad_mcw );
extern HI_VOID VPSS_DRV_Set_VPSS_MCDI_RGMCW6(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 k_rgsad_mcw ,HI_U32 x_rgsad_mcw ,HI_U32 k0_smrg_mcw ,HI_U32 x0_smrg_mcw );
extern HI_VOID VPSS_DRV_Set_VPSS_MCDI_RGMV(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 thmag_rgmv ,HI_U32 th_saddif_rgmv ,HI_U32 th_0mvsad_rgmv );
extern HI_VOID VPSS_DRV_Set_VPSS_MCDI_RGSAD(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 coef_sadlpf ,HI_U32 kmv_rgsad ,HI_U32 k_tpdif_rgsad ,HI_U32 g_tpdif_rgsad );
extern HI_VOID VPSS_DRV_Set_VPSS_MCDI_SEL(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 force_mven ,HI_U32 force_mvx );
extern HI_VOID VPSS_DRV_Set_VPSS_MCDI_TPMCW0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 k1_tpmvdist_mcw ,HI_U32 g0_tpmvdist_mcw ,HI_U32 k0_tpmvdist_mcw ,HI_U32 x0_tpmvdist_mcw );
extern HI_VOID VPSS_DRV_Set_VPSS_MCDI_TPMCW1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 k_core_tpmvdist_mcw ,HI_U32 b_core_tpmvdist_mcw ,HI_U32 k_avgmvdist_mcw ,HI_U32 k_minmvdist_mcw );
extern HI_VOID VPSS_DRV_Set_VPSS_MCDI_VERTWEIGHT0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 b_hvdif_dw ,HI_U32 b_bhvdif_dw ,HI_U32 k_bhvdif_dw ,HI_U32 core_bhvdif_dw ,HI_U32 gain_lpf_dw ,HI_U32 k_max_hvdif_dw );
extern HI_VOID VPSS_DRV_Set_VPSS_MCDI_VERTWEIGHT1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 b_mv_dw ,HI_U32 core_mv_dw ,HI_U32 k_difv_dw ,HI_U32 core_hvdif_dw );
extern HI_VOID VPSS_DRV_Set_VPSS_MCDI_VERTWEIGHT2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 k1_hvdif_dw ,HI_U32 g0_hvdif_dw ,HI_U32 k0_hvdif_dw ,HI_U32 x0_hvdif_dw );
extern HI_VOID VPSS_DRV_Set_VPSS_MCDI_VERTWEIGHT3(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 k1_mv_dw ,HI_U32 g0_mv_dw ,HI_U32 k0_mv_dw ,HI_U32 x0_mv_dw );
extern HI_VOID VPSS_DRV_Set_VPSS_MCDI_VERTWEIGHT4(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 k1_mt_dw ,HI_U32 g0_mt_dw ,HI_U32 k0_mt_dw ,HI_U32 x0_mt_dw );
extern HI_VOID VPSS_DRV_Set_VPSS_MCDI_VERTWEIGHT5(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 b_mt_dw ,HI_U32 k1_mv_mt ,HI_U32 x0_mv_mt ,HI_U32 g0_mv_mt );
extern HI_VOID VPSS_DRV_Set_VPSS_MCDI_WNDMCW0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 k_tbdif_mcw ,HI_U32 k0_max_mag_mcw ,HI_U32 k1_max_mag_mcw ,HI_U32 k_max_dif_mcw ,HI_U32 k_max_core_mcw ,HI_U32 k_difvcore_mcw ,HI_U32 k_difhcore_mcw );
extern HI_VOID VPSS_DRV_Set_VPSS_MCDI_WNDMCW1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 k1_mag_wnd_mcw ,HI_U32 g0_mag_wnd_mcw ,HI_U32 k0_mag_wnd_mcw ,HI_U32 x0_mag_wnd_mcw ,HI_U32 k_tbmag_mcw );
extern HI_VOID VPSS_DRV_Set_VPSS_MCDI_WNDMCW2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 g0_sad_wnd_mcw ,HI_U32 k0_sad_wnd_mcw ,HI_U32 x0_sad_wnd_mcw ,HI_U32 g1_mag_wnd_mcw );
extern HI_VOID VPSS_DRV_Set_VPSS_MCDI_WNDMCW3(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 g1_sad_wnd_mcw ,HI_U32 k1_sad_wnd_mcw );
extern HI_VOID VPSS_DRV_Set_VPSS_MISCELLANEOUS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 burst_len_cfg ,HI_U32 ck_gt_en_calc ,HI_U32 ck_gt_en ,HI_U32 init_timer ,HI_U32 woutstanding ,HI_U32 routstanding );
extern HI_VOID VPSS_DRV_Set_VPSS_MISCELLANEOUS_OUTSTANDING(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 woutstanding ,HI_U32 routstanding );
extern HI_VOID VPSS_DRV_Set_VPSS_MISCELLANEOUS_BURSTLENGTH(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 u32BurstLength);
extern HI_VOID VPSS_DRV_Set_VPSS_NODEID(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nodeid );
extern HI_VOID VPSS_DRV_Set_VPSS_NX1_VC1_CTRL0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nx1_vc1_rangedfrm ,HI_U32 nx1_vc1_profile ,HI_U32 nx1_vc1_mapyflg ,HI_U32 nx1_vc1_mapcflg ,HI_U32 nx1_vc1_mapy ,HI_U32 nx1_vc1_mapc );
extern HI_VOID VPSS_DRV_Set_VPSS_NX2C_HEAD_ADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nx2c_top_head_addr );
extern HI_VOID VPSS_DRV_Set_VPSS_NX2C_LB_ADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nx2clb_addr );
extern HI_VOID VPSS_DRV_Set_VPSS_NX2Y_HEAD_ADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nx2y_top_head_addr );
extern HI_VOID VPSS_DRV_Set_VPSS_NX2Y_LB_ADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nx2ylb_addr );
extern HI_VOID VPSS_DRV_Set_VPSS_NX2_HEAD_STRIDE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nx2_head_stride );
extern HI_VOID VPSS_DRV_Set_VPSS_NX2_LB_STRIDE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nx2lb_stride );
extern HI_VOID VPSS_DRV_Set_VPSS_NX2_TUNL_ADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nx2_tunl_addr );
extern HI_VOID VPSS_DRV_Set_VPSS_NX2_TUNL_CTRL(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nx2_tunl_rd_interval ,HI_U32 nx2_tunl_en );
extern HI_VOID VPSS_DRV_Set_VPSS_NX2_VC1_CTRL0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nx2_vc1_rangedfrm ,HI_U32 nx2_vc1_profile ,HI_U32 nx2_vc1_mapyflg ,HI_U32 nx2_vc1_mapcflg ,HI_U32 nx2_vc1_mapy ,HI_U32 nx2_vc1_mapc );
extern HI_VOID VPSS_DRV_Set_VPSS_NXT1CADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nxt1c_addr );
extern HI_VOID VPSS_DRV_Set_VPSS_NXT1STRIDE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nxt1c_stride ,HI_U32 nxt1y_stride );
extern HI_VOID VPSS_DRV_Set_VPSS_NXT1YADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nxt1y_addr );
extern HI_VOID VPSS_DRV_Set_VPSS_NXT1_CTRL(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nxt1_ver_offset ,HI_U32 nxt1_hor_offset ,HI_U32 nxt1_tile_format );
extern HI_VOID VPSS_DRV_Set_VPSS_NXT1_CTRL_nxt1_tile_format(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nxt1_tile_format );
extern HI_VOID VPSS_DRV_Set_VPSS_NXT1_CTRL_offset(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nxt1_ver_offset ,HI_U32 nxt1_hor_offset );
extern HI_VOID VPSS_DRV_Set_VPSS_NXT2CADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nxt2c_addr );
extern HI_VOID VPSS_DRV_Set_VPSS_NXT2STRIDE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nxt2c_stride ,HI_U32 nxt2y_stride );
extern HI_VOID VPSS_DRV_Set_VPSS_NXT2YADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nxt2y_addr );
extern HI_VOID VPSS_DRV_Set_VPSS_NXT2_CTRL(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nxt2_dcmp_en ,HI_U32 nxt2_ver_offset ,HI_U32 nxt2_hor_offset ,HI_U32 nxt2_tile_format );
extern HI_VOID VPSS_DRV_Set_VPSS_NXT2_CTRL_decmp(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nxt2_dcmp_en );
extern HI_VOID VPSS_DRV_Set_VPSS_NXT2_CTRL_offset(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 nxt2_ver_offset ,HI_U32 nxt2_hor_offset );
extern HI_VOID VPSS_DRV_Set_VPSS_NXT2_CTRL_tile(S_VPSS_REGS_TYPE *pstVpssRegs , HI_U32 nxt2_tile_format );
extern HI_VOID VPSS_DRV_Set_VPSS_PDBLKTHD(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 diff_movblk_thd );
extern HI_VOID VPSS_DRV_Set_VPSS_PDCTRL(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 lasi_mode ,HI_U32 bitsmov2r );
extern HI_VOID VPSS_DRV_Set_VPSS_PDITDIFFVTHD(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 itdiff_vthd3 ,HI_U32 itdiff_vthd2 ,HI_U32 itdiff_vthd1 ,HI_U32 itdiff_vthd0 );
extern HI_VOID VPSS_DRV_Set_VPSS_PDLASIMODE0COEF(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 lasi_txt_coring ,HI_U32 lasi_txt_alpha );
extern HI_VOID VPSS_DRV_Set_VPSS_PDLASIMODE1COEF(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 lasi_txt_thd3 ,HI_U32 lasi_txt_thd2 ,HI_U32 lasi_txt_thd1 ,HI_U32 lasi_txt_thd0 );
extern HI_VOID VPSS_DRV_Set_VPSS_PDLASITHD(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 lasi_mov_thd ,HI_U32 lasi_edge_thd ,HI_U32 lasi_coring_thd );
extern HI_VOID VPSS_DRV_Set_VPSS_PDPCCCORING(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 coring_blk ,HI_U32 coring_norm ,HI_U32 coring_tkr );
extern HI_VOID VPSS_DRV_Set_VPSS_PDPCCHTHD(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 pcc_hthd );
extern HI_VOID VPSS_DRV_Set_VPSS_PDPCCMOV(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 mov_coring_norm ,HI_U32 mov_coring_tkr ,HI_U32 mov_coring_blk );
extern HI_VOID VPSS_DRV_Set_VPSS_PDPCCVTHD(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 pcc_vthd3 ,HI_U32 pcc_vthd2 ,HI_U32 pcc_vthd1 ,HI_U32 pcc_vthd0 );
extern HI_VOID VPSS_DRV_Set_VPSS_PDPHISTTHD1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 hist_thd3 ,HI_U32 hist_thd2 ,HI_U32 hist_thd1 ,HI_U32 hist_thd0 );
extern HI_VOID VPSS_DRV_Set_VPSS_PDREGION(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 region1_y_stt ,HI_U32 region1_y_end );
extern HI_VOID VPSS_DRV_Set_VPSS_PDUMTHD(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 um_thd2 ,HI_U32 um_thd1 ,HI_U32 um_thd0 );
extern HI_VOID VPSS_DRV_Set_VPSS_PNEXT(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 *pu32PhyAddr);
extern HI_VOID VPSS_DRV_Set_VPSS_PRJCURSTRIDE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 prjh_cur_stride ,HI_U32 prjv_cur_stride );
extern HI_VOID VPSS_DRV_Set_VPSS_PRJCURSTRIDE_H(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 prjh_cur_stride);
extern HI_VOID VPSS_DRV_Set_VPSS_PRJCURSTRIDE_V(S_VPSS_REGS_TYPE *pstVpssRegs , HI_U32 prjv_cur_stride );
extern HI_VOID VPSS_DRV_Set_VPSS_PRJHCURADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 prjh_cur_addr );
extern HI_VOID VPSS_DRV_Set_VPSS_PRJHNX2ADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 prjh_nx2_addr );
extern HI_VOID VPSS_DRV_Set_VPSS_PRJNX2STRIDE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 prjh_nx2_stride ,HI_U32 prjv_nx2_stride );
extern HI_VOID VPSS_DRV_Set_VPSS_PRJNX2STRIDE_H(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 prjh_nx2_stride );
extern HI_VOID VPSS_DRV_Set_VPSS_PRJNX2STRIDE_V(S_VPSS_REGS_TYPE *pstVpssRegs, HI_U32 prjv_nx2_stride );
extern HI_VOID VPSS_DRV_Set_VPSS_PRJVCURADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 prjv_cur_addr );
extern HI_VOID VPSS_DRV_Set_VPSS_PRJVNX2ADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 prjv_nx2_addr );
extern HI_VOID VPSS_DRV_Set_VPSS_RCH_BYPASS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 rch_transr_bypass ,HI_U32 rch_nx2c_tile_2bit_bypass ,HI_U32 rch_nx2y_tile_2bit_bypass ,HI_U32 rch_nx2c_head_bypass ,HI_U32 rch_nx2y_head_bypass ,HI_U32 rch_tunl_bypass ,HI_U32 rch_reec_bypass ,HI_U32 rch_reey_bypass ,HI_U32 rch_rgmv_cur_bypass ,HI_U32 rch_rgmv_nx1_bypass ,HI_U32 rch_prjv_cur_bypass ,HI_U32 rch_blkmv_ref_bypass ,HI_U32 rch_blkmv_cur_bypass ,HI_U32 rch_blkmv_nx1_bypass ,HI_U32 rch_prjh_cur_bypass ,HI_U32 rch_rst_bypass ,HI_U32 rch_srmd_bypass ,HI_U32 rch_trmd_bypass ,HI_U32 rch_nx2c_bypass ,HI_U32 rch_nx2y_bypass ,HI_U32 rch_nx1c_bypass ,HI_U32 rch_nx1y_bypass ,HI_U32 rch_refc_bypass ,HI_U32 rch_refy_bypass ,HI_U32 rch_curc_bypass ,HI_U32 rch_cury_bypass );
extern HI_VOID VPSS_DRV_Set_VPSS_RCH_BYPASS_FLAG(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 u32Flag);
extern HI_VOID VPSS_DRV_Set_VPSS_REECADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 reec_addr );
extern HI_VOID VPSS_DRV_Set_VPSS_REESTRIDE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 reec_stride ,HI_U32 reey_stride );
extern HI_VOID VPSS_DRV_Set_VPSS_REEYADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 reey_addr );
extern HI_VOID VPSS_DRV_Set_VPSS_REE_CTRL(S_VPSS_REGS_TYPE *pstVpssRegs );
extern HI_VOID VPSS_DRV_Set_VPSS_REFCADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 refc_addr );
extern HI_VOID VPSS_DRV_Set_VPSS_REFSIZE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 refheight ,HI_U32 refwidth );
extern HI_VOID VPSS_DRV_Set_VPSS_REFSTRIDE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 refc_stride ,HI_U32 refy_stride );
extern HI_VOID VPSS_DRV_Set_VPSS_REFYADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 refy_addr );
extern HI_VOID VPSS_DRV_Set_VPSS_REF_CTRL(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ref_ver_offset ,HI_U32 ref_hor_offset ,HI_U32 ref_tile_format );
extern HI_VOID VPSS_DRV_Set_VPSS_REF_VC1_CTRL0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ref_vc1_rangedfrm ,HI_U32 ref_vc1_profile ,HI_U32 ref_vc1_mapyflg ,HI_U32 ref_vc1_mapcflg ,HI_U32 ref_vc1_mapy ,HI_U32 ref_vc1_mapc );
extern HI_VOID VPSS_DRV_Set_VPSS_RFRCADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 rfrc_addr );
extern HI_VOID VPSS_DRV_Set_VPSS_RFRCTRL(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 rfr_dither_mode ,HI_U32 rfr_dither_en );
extern HI_VOID VPSS_DRV_Set_VPSS_RFRSTRIDE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 rfrc_stride ,HI_U32 rfry_stride );
extern HI_VOID VPSS_DRV_Set_VPSS_RFRYADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 rfry_addr );
extern HI_VOID VPSS_DRV_Set_VPSS_RGMVCURADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 rgmv_cur_addr );
extern HI_VOID VPSS_DRV_Set_VPSS_RGMVNX1ADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 rgmv_nx1_addr );
extern HI_VOID VPSS_DRV_Set_VPSS_RGMVNX2ADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 rgmv_nx2_addr );
extern HI_VOID VPSS_DRV_Set_VPSS_RGMVNX2STRIDE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 rgmv_nx2_stride );
extern HI_VOID VPSS_DRV_Set_VPSS_RGMVSTRIDE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 rgmv_nx1_stride ,HI_U32 rgmv_cur_stride );
extern HI_VOID VPSS_DRV_Set_VPSS_RTT_CFG(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 rotate_angle );
extern HI_VOID VPSS_DRV_Set_VPSS_SNR_ADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 snr_cfg_addr );
extern HI_VOID VPSS_DRV_Set_VPSS_SNR_CBCR_W1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cbcr_weight1 ,HI_U32 cr_end1 ,HI_U32 cb_end1 ,HI_U32 cr_begin1 ,HI_U32 cb_begin1 );
extern HI_VOID VPSS_DRV_Set_VPSS_SNR_CBCR_W2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cbcr_weight2 ,HI_U32 cr_end2 ,HI_U32 cb_end2 ,HI_U32 cr_begin2 ,HI_U32 cb_begin2 );
extern HI_VOID VPSS_DRV_Set_VPSS_SNR_EDGE_MEANK1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 edgemeank4 ,HI_U32 edgemeank3 ,HI_U32 edgemeank2 ,HI_U32 edgemeank1 );
extern HI_VOID VPSS_DRV_Set_VPSS_SNR_EDGE_MEANK2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 edgemeank8 ,HI_U32 edgemeank7 ,HI_U32 edgemeank6 ,HI_U32 edgemeank5 );
extern HI_VOID VPSS_DRV_Set_VPSS_SNR_EDGE_MEANTH1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 edgemeanth3 ,HI_U32 edgemeanth2 ,HI_U32 edgemeanth1 );
extern HI_VOID VPSS_DRV_Set_VPSS_SNR_EDGE_MEANTH2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 edgemeanth6 ,HI_U32 edgemeanth5 ,HI_U32 edgemeanth4 );
extern HI_VOID VPSS_DRV_Set_VPSS_SNR_EDGE_MEANTH3(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 edgemeanth8 ,HI_U32 edgemeanth7 );
extern HI_VOID VPSS_DRV_Set_VPSS_SNR_EDGE_RATIORANGE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 edgeoriratio ,HI_U32 edgeminratio ,HI_U32 edgemaxratio );
extern HI_VOID VPSS_DRV_Set_VPSS_SNR_EDGE_RATIO_1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ratio3 ,HI_U32 ratio2 ,HI_U32 ratio1 );
extern HI_VOID VPSS_DRV_Set_VPSS_SNR_EDGE_RATIO_2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ratio6 ,HI_U32 ratio5 ,HI_U32 ratio4 );
extern HI_VOID VPSS_DRV_Set_VPSS_SNR_EDGE_RATIO_3(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ratio8 ,HI_U32 ratio7 );
extern HI_VOID VPSS_DRV_Set_VPSS_SNR_EDGE_STR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 edge3 ,HI_U32 edge2 ,HI_U32 edge1 );
extern HI_VOID VPSS_DRV_Set_VPSS_SNR_EDGE_STRK(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 edgestrk3 ,HI_U32 edgestrk2 ,HI_U32 edgestrk1 );
extern HI_VOID VPSS_DRV_Set_VPSS_SNR_EDGE_STRRANGE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 edgeoristrength ,HI_U32 edgeminstrength ,HI_U32 edgemaxstrength );
extern HI_VOID VPSS_DRV_Set_VPSS_SNR_EDGE_STRTH(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 edgestrth3 ,HI_U32 edgestrth2 ,HI_U32 edgestrth1 );
extern HI_VOID VPSS_DRV_Set_VPSS_SNR_ENABLE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 test_en ,HI_U32 edgeprefilteren ,HI_U32 colorweighten ,HI_U32 motionedgeweighten ,HI_U32 cnr2den ,HI_U32 ynr2den );
extern HI_VOID VPSS_DRV_Set_VPSS_SNR_MAD_RADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 snr_mad_raddr );
extern HI_VOID VPSS_DRV_Set_VPSS_SNR_MT(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motionalpha );
extern HI_VOID VPSS_DRV_Set_VPSS_SNR_SCHANGE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 scenechange_mode2_en ,HI_U32 scenechange_bldcore ,HI_U32 scenechange_bldk ,HI_U32 scenechange_mode1_en ,HI_U32 scenechange_en ,HI_U32 scenechange_info );
extern HI_VOID VPSS_DRV_Set_VPSS_SNR_SCHANGE_TH(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 scenechangeth );
extern HI_VOID VPSS_DRV_Set_VPSS_SNR_STR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 csnrstr ,HI_U32 ysnrstr );
extern HI_VOID VPSS_DRV_Set_VPSS_SNR_STR_ADJUST(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cstradjust ,HI_U32 ystradjust );
extern HI_VOID VPSS_DRV_Set_VPSS_SNR_STR_OFFSET(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 stroffset3 ,HI_U32 stroffset2 ,HI_U32 stroffset1 ,HI_U32 stroffset0 );
extern HI_VOID VPSS_DRV_Set_VPSS_SNR_TEST_COLOR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 test_color_cr ,HI_U32 test_color_cb );
extern HI_VOID VPSS_DRV_Set_VPSS_SNR_WIN(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 c2dwinheight ,HI_U32 c2dwinwidth ,HI_U32 y2dwinheight ,HI_U32 y2dwinwidth );
extern HI_VOID VPSS_DRV_Set_VPSS_START(S_VPSS_REGS_TYPE *pstVpssRegs);
extern HI_VOID VPSS_DRV_Set_VPSS_STRADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 st_raddr );
extern HI_VOID VPSS_DRV_Set_VPSS_STSTRIDE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 st_stride );
extern HI_VOID VPSS_DRV_Set_VPSS_STT_W_ADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 stt_w_addr );
extern HI_VOID VPSS_DRV_Set_VPSS_STWADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 st_waddr );
extern HI_VOID VPSS_DRV_Set_VPSS_TESTPATTERN(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 pat_bkgrnd ,HI_U32 pat_dis_width ,HI_U32 pat_angle ,HI_U32 test_pat_en );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_ADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 tnr_cfg_addr );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_CBLEND(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cblendingymax ,HI_U32 cblendingxmax ,HI_U32 cblendingymin ,HI_U32 cblendingxmin );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_CBLENDING_LUT0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cblendingalphalut3 ,HI_U32 cblendingalphalut2 ,HI_U32 cblendingalphalut1 ,HI_U32 cblendingalphalut0 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_CBLENDING_LUT1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cblendingalphalut7 ,HI_U32 cblendingalphalut6 ,HI_U32 cblendingalphalut5 ,HI_U32 cblendingalphalut4 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_CBLENDING_LUT2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cblendingalphalut11 ,HI_U32 cblendingalphalut10 ,HI_U32 cblendingalphalut9 ,HI_U32 cblendingalphalut8 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_CBLENDING_LUT3(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cblendingalphalut15 ,HI_U32 cblendingalphalut14 ,HI_U32 cblendingalphalut13 ,HI_U32 cblendingalphalut12 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_CBLENDING_LUT4(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cblendingalphalut19 ,HI_U32 cblendingalphalut18 ,HI_U32 cblendingalphalut17 ,HI_U32 cblendingalphalut16 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_CBLENDING_LUT5(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cblendingalphalut23 ,HI_U32 cblendingalphalut22 ,HI_U32 cblendingalphalut21 ,HI_U32 cblendingalphalut20 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_CBLENDING_LUT6(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cblendingalphalut27 ,HI_U32 cblendingalphalut26 ,HI_U32 cblendingalphalut25 ,HI_U32 cblendingalphalut24 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_CBLENDING_LUT7(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cblendingalphalut31 ,HI_U32 cblendingalphalut30 ,HI_U32 cblendingalphalut29 ,HI_U32 cblendingalphalut28 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_CFG(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cfg_cmdcore ,HI_U32 cfg_cmdgain ,HI_U32 cfg_ymdcore ,HI_U32 cfg_ymdgain );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut3 ,HI_U32 cmotionstrlut2 ,HI_U32 cmotionstrlut1 ,HI_U32 cmotionstrlut0 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut7 ,HI_U32 cmotionstrlut6 ,HI_U32 cmotionstrlut5 ,HI_U32 cmotionstrlut4 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT10(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut43 ,HI_U32 cmotionstrlut42 ,HI_U32 cmotionstrlut41 ,HI_U32 cmotionstrlut40 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT11(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut47 ,HI_U32 cmotionstrlut46 ,HI_U32 cmotionstrlut45 ,HI_U32 cmotionstrlut44 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT12(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut51 ,HI_U32 cmotionstrlut50 ,HI_U32 cmotionstrlut49 ,HI_U32 cmotionstrlut48 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT13(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut55 ,HI_U32 cmotionstrlut54 ,HI_U32 cmotionstrlut53 ,HI_U32 cmotionstrlut52 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT14(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut59 ,HI_U32 cmotionstrlut58 ,HI_U32 cmotionstrlut57 ,HI_U32 cmotionstrlut56 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT15(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut63 ,HI_U32 cmotionstrlut62 ,HI_U32 cmotionstrlut61 ,HI_U32 cmotionstrlut60 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT16(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut67 ,HI_U32 cmotionstrlut66 ,HI_U32 cmotionstrlut65 ,HI_U32 cmotionstrlut64 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT17(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut71 ,HI_U32 cmotionstrlut70 ,HI_U32 cmotionstrlut69 ,HI_U32 cmotionstrlut68 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT18(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut75 ,HI_U32 cmotionstrlut74 ,HI_U32 cmotionstrlut73 ,HI_U32 cmotionstrlut72 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT19(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut79 ,HI_U32 cmotionstrlut78 ,HI_U32 cmotionstrlut77 ,HI_U32 cmotionstrlut76 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut11 ,HI_U32 cmotionstrlut10 ,HI_U32 cmotionstrlut9 ,HI_U32 cmotionstrlut8 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT20(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut83 ,HI_U32 cmotionstrlut82 ,HI_U32 cmotionstrlut81 ,HI_U32 cmotionstrlut80 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT21(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut87 ,HI_U32 cmotionstrlut86 ,HI_U32 cmotionstrlut85 ,HI_U32 cmotionstrlut84 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT22(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut91 ,HI_U32 cmotionstrlut90 ,HI_U32 cmotionstrlut89 ,HI_U32 cmotionstrlut88 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT23(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut95 ,HI_U32 cmotionstrlut94 ,HI_U32 cmotionstrlut93 ,HI_U32 cmotionstrlut92 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT24(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut99 ,HI_U32 cmotionstrlut98 ,HI_U32 cmotionstrlut97 ,HI_U32 cmotionstrlut96 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT25(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut103 ,HI_U32 cmotionstrlut102 ,HI_U32 cmotionstrlut101 ,HI_U32 cmotionstrlut100 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT26(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut107 ,HI_U32 cmotionstrlut106 ,HI_U32 cmotionstrlut105 ,HI_U32 cmotionstrlut104 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT27(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut111 ,HI_U32 cmotionstrlut110 ,HI_U32 cmotionstrlut109 ,HI_U32 cmotionstrlut108 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT28(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut115 ,HI_U32 cmotionstrlut114 ,HI_U32 cmotionstrlut113 ,HI_U32 cmotionstrlut112 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT29(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut119 ,HI_U32 cmotionstrlut118 ,HI_U32 cmotionstrlut117 ,HI_U32 cmotionstrlut116 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT3(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut15 ,HI_U32 cmotionstrlut14 ,HI_U32 cmotionstrlut13 ,HI_U32 cmotionstrlut12 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT30(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut123 ,HI_U32 cmotionstrlut122 ,HI_U32 cmotionstrlut121 ,HI_U32 cmotionstrlut120 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT31(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut127 ,HI_U32 cmotionstrlut126 ,HI_U32 cmotionstrlut125 ,HI_U32 cmotionstrlut124 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT4(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut19 ,HI_U32 cmotionstrlut18 ,HI_U32 cmotionstrlut17 ,HI_U32 cmotionstrlut16 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT5(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut23 ,HI_U32 cmotionstrlut22 ,HI_U32 cmotionstrlut21 ,HI_U32 cmotionstrlut20 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT6(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut27 ,HI_U32 cmotionstrlut26 ,HI_U32 cmotionstrlut25 ,HI_U32 cmotionstrlut24 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT7(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut31 ,HI_U32 cmotionstrlut30 ,HI_U32 cmotionstrlut29 ,HI_U32 cmotionstrlut28 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT8(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut35 ,HI_U32 cmotionstrlut34 ,HI_U32 cmotionstrlut33 ,HI_U32 cmotionstrlut32 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_CMOTIONSTR_LUT9(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotionstrlut39 ,HI_U32 cmotionstrlut38 ,HI_U32 cmotionstrlut37 ,HI_U32 cmotionstrlut36 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_CONTRL(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 marketcoor ,HI_U32 marketmode ,HI_U32 marketmodeen ,HI_U32 sharemotionen ,HI_U32 mcnr_bypass_en ,HI_U32 cnren ,HI_U32 ynren ,HI_U32 nren );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_DTBLEND(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 dtblendingymax ,HI_U32 dtblendingxmax ,HI_U32 dtblendingymin ,HI_U32 dtblendingxmin );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_FILR_MODE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cnonrrange ,HI_U32 ynonrrange ,HI_U32 cmotionmode ,HI_U32 ymotionmode );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_FILR_MODE_ymmode(S_VPSS_REGS_TYPE *pstVpssRegs , HI_U32 ymotionmode );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_FLAT_INFO(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 flatinfoymax ,HI_U32 flatinfoxmax ,HI_U32 flatinfoymin ,HI_U32 flatinfoxmin );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_MAMC_CBCR_UPEN(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cbcrweight2 ,HI_U32 cbcrweight1 ,HI_U32 cbcrupdateen );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_MAMC_CBCR_UPTHD1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 crend1 ,HI_U32 cbend1 ,HI_U32 crbegin1 ,HI_U32 cbbegin1 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_MAMC_CBCR_UPTHD2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 crend2 ,HI_U32 cbend2 ,HI_U32 crbegin2 ,HI_U32 cbbegin2 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_MAMC_MAP(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmotioncore ,HI_U32 cmotiongain ,HI_U32 ymotioncore ,HI_U32 ymotiongain ,HI_U32 motionmappingen );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_MAMC_MCADJ(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmclpfmode ,HI_U32 ymclpfmode ,HI_U32 cmcadjen ,HI_U32 ymcadjen );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_MAMC_PRE_MAD(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 motionmergeratio ,HI_U32 premotionalpha ,HI_U32 premotionmergemode );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_MAMC_SALTUS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 saltuslevel ,HI_U32 saltusdecten );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_MAMD_CTRL(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 alpha2 ,HI_U32 alpha1 ,HI_U32 cmotionlpfmode ,HI_U32 ymotionlpfmode ,HI_U32 cmotioncalcmode ,HI_U32 ymotioncalcmode ,HI_U32 meancadjshift ,HI_U32 meanyadjshift ,HI_U32 mean_cmotion_adj_en ,HI_U32 mean_ymotion_adj_en ,HI_U32 mdprelpfen );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_MAMD_GAIN(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 cmdcore ,HI_U32 cmdgain ,HI_U32 ymdcore ,HI_U32 ymdgain );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_MAME_CTRL(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 gm_adj ,HI_U32 mvlpfmode ,HI_U32 mvrefmode ,HI_U32 motionestimateen );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_MAME_CTRL_mcTnr(S_VPSS_REGS_TYPE *pstVpssRegs , HI_U32 bEnable );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_MAME_GAIN(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 mag_pnl_core_xmv ,HI_U32 mag_pnl_gain_xmv ,HI_U32 mag_pnl_core_0mv ,HI_U32 mag_pnl_gain_0mv );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_MAME_MV_THD(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 adj_xmv_max ,HI_U32 adj_0mv_max ,HI_U32 adj_xmv_min ,HI_U32 adj_0mv_min ,HI_U32 adj_mv_min ,HI_U32 adj_mv_max );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_MAME_STD_CORE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 std_pnl_core_xmv ,HI_U32 std_pnl_gain_xmv ,HI_U32 std_pnl_core_0mv ,HI_U32 std_pnl_gain_0mv ,HI_U32 std_core_xmv ,HI_U32 std_core_0mv );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_MAME_STD_GAIN(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 std_pnl_core ,HI_U32 std_pnl_gain );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_MAND_CTRL(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 blkdiffthd ,HI_U32 noisedetecten );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_MAND_THD(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 flatthdmax ,HI_U32 flatthdmed ,HI_U32 flatthdmin );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_MEAN_MOTION_LUT0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 mean_motion_ratio3 ,HI_U32 mean_motion_ratio2 ,HI_U32 mean_motion_ratio1 ,HI_U32 mean_motion_ratio0 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_MEAN_MOTION_LUT1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 mean_motion_ratio7 ,HI_U32 mean_motion_ratio6 ,HI_U32 mean_motion_ratio5 ,HI_U32 mean_motion_ratio4 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_MEAN_MOTION_LUT2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 mean_motion_ratio11 ,HI_U32 mean_motion_ratio10 ,HI_U32 mean_motion_ratio9 ,HI_U32 mean_motion_ratio8 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_MEAN_MOTION_LUT3(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 mean_motion_ratio15 ,HI_U32 mean_motion_ratio14 ,HI_U32 mean_motion_ratio13 ,HI_U32 mean_motion_ratio12 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_SCENE_CHANGE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 scenechangeinfo ,HI_U32 scenechangeen );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_YBLEND(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 yblendingymax ,HI_U32 yblendingxmax ,HI_U32 yblendingymin ,HI_U32 yblendingxmin );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_YBLENDING_LUT0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 yblendingalphalut3 ,HI_U32 yblendingalphalut2 ,HI_U32 yblendingalphalut1 ,HI_U32 yblendingalphalut0 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_YBLENDING_LUT1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 yblendingalphalut7 ,HI_U32 yblendingalphalut6 ,HI_U32 yblendingalphalut5 ,HI_U32 yblendingalphalut4 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_YBLENDING_LUT2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 yblendingalphalut11 ,HI_U32 yblendingalphalut10 ,HI_U32 yblendingalphalut9 ,HI_U32 yblendingalphalut8 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_YBLENDING_LUT3(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 yblendingalphalut15 ,HI_U32 yblendingalphalut14 ,HI_U32 yblendingalphalut13 ,HI_U32 yblendingalphalut12 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_YBLENDING_LUT4(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 yblendingalphalut19 ,HI_U32 yblendingalphalut18 ,HI_U32 yblendingalphalut17 ,HI_U32 yblendingalphalut16 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_YBLENDING_LUT5(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 yblendingalphalut23 ,HI_U32 yblendingalphalut22 ,HI_U32 yblendingalphalut21 ,HI_U32 yblendingalphalut20 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_YBLENDING_LUT6(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 yblendingalphalut27 ,HI_U32 yblendingalphalut26 ,HI_U32 yblendingalphalut25 ,HI_U32 yblendingalphalut24 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_YBLENDING_LUT7(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 yblendingalphalut31 ,HI_U32 yblendingalphalut30 ,HI_U32 yblendingalphalut29 ,HI_U32 yblendingalphalut28 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut3 ,HI_U32 ymotionstrlut2 ,HI_U32 ymotionstrlut1 ,HI_U32 ymotionstrlut0 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT1(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut7 ,HI_U32 ymotionstrlut6 ,HI_U32 ymotionstrlut5 ,HI_U32 ymotionstrlut4 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT10(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut43 ,HI_U32 ymotionstrlut42 ,HI_U32 ymotionstrlut41 ,HI_U32 ymotionstrlut40 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT11(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut47 ,HI_U32 ymotionstrlut46 ,HI_U32 ymotionstrlut45 ,HI_U32 ymotionstrlut44 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT12(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut51 ,HI_U32 ymotionstrlut50 ,HI_U32 ymotionstrlut49 ,HI_U32 ymotionstrlut48 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT13(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut55 ,HI_U32 ymotionstrlut54 ,HI_U32 ymotionstrlut53 ,HI_U32 ymotionstrlut52 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT14(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut59 ,HI_U32 ymotionstrlut58 ,HI_U32 ymotionstrlut57 ,HI_U32 ymotionstrlut56 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT15(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut63 ,HI_U32 ymotionstrlut62 ,HI_U32 ymotionstrlut61 ,HI_U32 ymotionstrlut60 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT16(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut67 ,HI_U32 ymotionstrlut66 ,HI_U32 ymotionstrlut65 ,HI_U32 ymotionstrlut64 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT17(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut71 ,HI_U32 ymotionstrlut70 ,HI_U32 ymotionstrlut69 ,HI_U32 ymotionstrlut68 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT18(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut75 ,HI_U32 ymotionstrlut74 ,HI_U32 ymotionstrlut73 ,HI_U32 ymotionstrlut72 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT19(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut79 ,HI_U32 ymotionstrlut78 ,HI_U32 ymotionstrlut77 ,HI_U32 ymotionstrlut76 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT2(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut11 ,HI_U32 ymotionstrlut10 ,HI_U32 ymotionstrlut9 ,HI_U32 ymotionstrlut8 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT20(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut83 ,HI_U32 ymotionstrlut82 ,HI_U32 ymotionstrlut81 ,HI_U32 ymotionstrlut80 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT21(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut87 ,HI_U32 ymotionstrlut86 ,HI_U32 ymotionstrlut85 ,HI_U32 ymotionstrlut84 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT22(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut91 ,HI_U32 ymotionstrlut90 ,HI_U32 ymotionstrlut89 ,HI_U32 ymotionstrlut88 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT23(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut95 ,HI_U32 ymotionstrlut94 ,HI_U32 ymotionstrlut93 ,HI_U32 ymotionstrlut92 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT24(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut99 ,HI_U32 ymotionstrlut98 ,HI_U32 ymotionstrlut97 ,HI_U32 ymotionstrlut96 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT25(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut103 ,HI_U32 ymotionstrlut102 ,HI_U32 ymotionstrlut101 ,HI_U32 ymotionstrlut100 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT26(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut107 ,HI_U32 ymotionstrlut106 ,HI_U32 ymotionstrlut105 ,HI_U32 ymotionstrlut104 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT27(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut111 ,HI_U32 ymotionstrlut110 ,HI_U32 ymotionstrlut109 ,HI_U32 ymotionstrlut108 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT28(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut115 ,HI_U32 ymotionstrlut114 ,HI_U32 ymotionstrlut113 ,HI_U32 ymotionstrlut112 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT29(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut119 ,HI_U32 ymotionstrlut118 ,HI_U32 ymotionstrlut117 ,HI_U32 ymotionstrlut116 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT3(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut15 ,HI_U32 ymotionstrlut14 ,HI_U32 ymotionstrlut13 ,HI_U32 ymotionstrlut12 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT30(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut123 ,HI_U32 ymotionstrlut122 ,HI_U32 ymotionstrlut121 ,HI_U32 ymotionstrlut120 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT31(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut127 ,HI_U32 ymotionstrlut126 ,HI_U32 ymotionstrlut125 ,HI_U32 ymotionstrlut124 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT4(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut19 ,HI_U32 ymotionstrlut18 ,HI_U32 ymotionstrlut17 ,HI_U32 ymotionstrlut16 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT5(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut23 ,HI_U32 ymotionstrlut22 ,HI_U32 ymotionstrlut21 ,HI_U32 ymotionstrlut20 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT6(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut27 ,HI_U32 ymotionstrlut26 ,HI_U32 ymotionstrlut25 ,HI_U32 ymotionstrlut24 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT7(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut31 ,HI_U32 ymotionstrlut30 ,HI_U32 ymotionstrlut29 ,HI_U32 ymotionstrlut28 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT8(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut35 ,HI_U32 ymotionstrlut34 ,HI_U32 ymotionstrlut33 ,HI_U32 ymotionstrlut32 );
extern HI_VOID VPSS_DRV_Set_VPSS_TNR_YMOTIONSTR_LUT9(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 ymotionstrlut39 ,HI_U32 ymotionstrlut38 ,HI_U32 ymotionstrlut37 ,HI_U32 ymotionstrlut36 );
extern HI_VOID VPSS_DRV_Set_VPSS_TRANS_DES_ADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 trans_des_addr );
extern HI_VOID VPSS_DRV_Set_VPSS_TRANS_INFO(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 trans_en ,HI_U32 trans_size );
extern HI_VOID VPSS_DRV_Set_VPSS_TRANS_SRC_ADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 trans_src_addr );
extern HI_VOID VPSS_DRV_Set_VPSS_VHD0CADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0c_addr );
extern HI_VOID VPSS_DRV_Set_VPSS_VHD0CROP_POS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_crop_y ,HI_U32 vhd0_crop_x );
extern HI_VOID VPSS_DRV_Set_VPSS_VHD0CROP_SIZE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_crop_height ,HI_U32 vhd0_crop_width );
extern HI_VOID VPSS_DRV_Set_VPSS_VHD0CTRL(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_dither_mode ,HI_U32 vhd0_lcmp_en ,HI_U32 vhd0_uv_invert ,HI_U32 vhd0_dither_en ,HI_U32 vhd0_pix_bitw ,HI_U32 vhd0_flip ,HI_U32 vhd0_mirror ,HI_U32 vhd0_format );
extern HI_VOID VPSS_DRV_Set_VPSS_VHD0CTRL_bitw(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_pix_bitw );
extern HI_VOID VPSS_DRV_Set_VPSS_VHD0CTRL_dither(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_dither_mode, HI_U32 vhd0_dither_en );
extern HI_VOID VPSS_DRV_Set_VPSS_VHD0CTRL_format(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_format );
extern HI_VOID VPSS_DRV_Set_VPSS_VHD0LBA_BK(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_vbk_cr ,HI_U32 vhd0_vbk_cb ,HI_U32 vhd0_vbk_y );
extern HI_VOID VPSS_DRV_Set_VPSS_VHD0LBA_DSIZE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_lba_height ,HI_U32 vhd0_lba_width );
extern HI_VOID VPSS_DRV_Set_VPSS_VHD0LBA_VFPOS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_lba_yfpos ,HI_U32 vhd0_lba_xfpos );
extern HI_VOID VPSS_DRV_Set_VPSS_VHD0SIZE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_height ,HI_U32 vhd0_width );
extern HI_VOID VPSS_DRV_Set_VPSS_VHD0STRIDE(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0c_stride ,HI_U32 vhd0y_stride );
extern HI_VOID VPSS_DRV_Set_VPSS_VHD0YADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0y_addr );
extern HI_VOID VPSS_DRV_Set_VPSS_VHD0_HCOFFSET(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 hor_coffset );
extern HI_VOID VPSS_DRV_Set_VPSS_VHD0_HLOFFSET(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 hor_loffset );
extern HI_VOID VPSS_DRV_Set_VPSS_VHD0_HSP(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 hlmsc_en ,HI_U32 hchmsc_en ,HI_U32 hlmid_en ,HI_U32 hchmid_en ,HI_U32 hlfir_en ,HI_U32 hchfir_en ,HI_U32 hfir_order ,HI_U32 hratio );
extern HI_VOID VPSS_DRV_Set_VPSS_VHD0_TUNL_ADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_tunl_addr );
extern HI_VOID VPSS_DRV_Set_VPSS_VHD0_TUNL_CTRL(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_tunl_en ,HI_U32 vhd0_tunl_mode ,HI_U32 vhd0_tunl_finish_line );
extern HI_VOID VPSS_DRV_Set_VPSS_VHD0_VOFFSET(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vluma_offset ,HI_U32 vchroma_offset );
extern HI_VOID VPSS_DRV_Set_VPSS_VHD0_VSP(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vlmsc_en ,HI_U32 vchmsc_en ,HI_U32 vlmid_en ,HI_U32 vchmid_en ,HI_U32 vlfir_en ,HI_U32 vchfir_en ,HI_U32 zme_out_fmt );
extern HI_VOID VPSS_DRV_Set_VPSS_VHD0_VSR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vratio );
extern HI_VOID VPSS_DRV_Set_VPSS_VHD0_ZMEORESO(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_zme_oh ,HI_U32 vhd0_zme_ow );
extern HI_VOID VPSS_DRV_Set_VPSS_VHD0_ZME_CHADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_scl_ch );
extern HI_VOID VPSS_DRV_Set_VPSS_VHD0_ZME_CVADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_scl_cv );
extern HI_VOID VPSS_DRV_Set_VPSS_VHD0_ZME_LHADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_scl_lh );
extern HI_VOID VPSS_DRV_Set_VPSS_VHD0_ZME_LVADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_scl_lv );
extern HI_VOID VPSS_DRV_Set_VPSS_VPZME_CFG0(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 vhd0_pre_vfir_mode );
extern HI_VOID VPSS_DRV_Set_VPSS_WCH_BYPASS(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 wch_tunl_bypass ,HI_U32 wch_transw_bypass ,HI_U32 wch_cuec_bypass ,HI_U32 wch_cuey_bypass ,HI_U32 wch_rgmvnx2_bypass ,HI_U32 wch_prjvnx2_bypass ,HI_U32 wch_blkmvnx2_bypass ,HI_U32 wch_prjhnx2_bypass ,HI_U32 wch_stt_bypass ,HI_U32 wch_twmd_bypass ,HI_U32 wch_wst_bypass ,HI_U32 wch_rfrc_bypass ,HI_U32 wch_rfry_bypass ,HI_U32 wch_vhd0c_bypass ,HI_U32 wch_vhd0y_bypass );
extern HI_VOID VPSS_DRV_Set_VPSS_WCH_BYPASS_FLAG(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 u32Flag);
extern HI_VOID VPSS_DRV_Set_VPSS_ZME_ADDR(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 zme_cfg_addr );
extern HI_VOID VPSS_DRV_Set_zme_vhd0_en(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 zme_vhd0_en );
extern HI_U32 VPSS_REG_RegRead(volatile HI_U32 *a);
extern HI_S32 VPSS_REG_RegWrite(volatile HI_U32 *a, HI_U32 b);
extern HI_S32 VPSS_REG_ResetAppReg(S_VPSS_REGS_TYPE *pstVpssRegs, VPSS_REG_S *pstPqReg);
HI_VOID VPSS_DRV_Set_VPSS_DIELMA2_mf(S_VPSS_REGS_TYPE *pstVpssRegs ,HI_U32 chroma_mf_max ,HI_U32 luma_mf_max );


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __VPSS_CFGREG_H__ */

