/******************************************************************************

  Copyright (C), 2012-2050, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : pq_hal_dei.c
  Version       : Initial Draft
  Author        : y00220178
  Created       : 2013/11/20
  Description   :

******************************************************************************/
#include "hi_type.h"
#include "pq_hal_dei.h"
#include "hi_drv_mmz.h"
#include "hi_drv_mem.h"


#if 0
/*--------------------------------------------------------------------------------*
Function        : HI_S32 PQ_HAL_SetDEIParam(DEI_PARA_S *pstDeiPara)
Description     :设置DEI寄存器值
Calls           :
Data Accessed   :
Data Updated    :
Input           : pstDeiPara    DEI寄存器配置参数
Output          :NA
Return          :NA
Others          :NA
*--------------------------------------------------------------------------------*/
HI_S32 PQ_HAL_SetDeiRegist(HI_U32 u32HandleNo, DEI_PARAMETER_S *pstDeiPara)
{
    VPSS_REG_S *pstVpssVirReg = NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_DIECTRL.bits.edge_smooth_ratio = pstDeiPara->u32EdgeSmoothRatio;
    pstVpssVirReg->VPSS_DIECTRL.bits.stinfo_stop = pstDeiPara->bStinfoStop;
    pstVpssVirReg->VPSS_DIECTRL.bits.die_rst = pstDeiPara->bDieRst;
    pstVpssVirReg->VPSS_DIECTRL.bits.mchdir_c = pstDeiPara->u8DirMchC;
    pstVpssVirReg->VPSS_DIECTRL.bits.mchdir_l = pstDeiPara->u8DirMchL;
    pstVpssVirReg->VPSS_DIECTRL.bits.edge_smooth_en = pstDeiPara->bEdgeSmoothEn;
    pstVpssVirReg->VPSS_DIECTRL.bits.die_c_mode = pstDeiPara->u8DeiChmmode;
    pstVpssVirReg->VPSS_DIECTRL.bits.die_l_mode = pstDeiPara->u8DeiLmmode;
    pstVpssVirReg->VPSS_DIECTRL.bits.die_out_sel_c = pstDeiPara->bDeiOutSelChm;
    pstVpssVirReg->VPSS_DIECTRL.bits.die_out_sel_l = pstDeiPara->bDeiOutSelLum;

    pstVpssVirReg->VPSS_DIELMA2.bits.recmode_frmfld_blend_mode = pstDeiPara->u8FrmFldBlendMode;
    pstVpssVirReg->VPSS_DIELMA2.bits.frame_motion_smooth_en  = pstDeiPara->bFrameMotionSmoothEn;
    pstVpssVirReg->VPSS_DIELMA2.bits.luma_scesdf_max = pstDeiPara->u32SCDUsingMax;
    pstVpssVirReg->VPSS_DIELMA2.bits.motion_iir_en = pstDeiPara->bMotionIIREn;
    pstVpssVirReg->VPSS_DIELMA2.bits.luma_mf_max = pstDeiPara->bLumaMfMax;
    pstVpssVirReg->VPSS_DIELMA2.bits.chroma_mf_max = pstDeiPara->bChmaMfMax;
    pstVpssVirReg->VPSS_DIELMA2.bits.rec_mode_en = pstDeiPara->bRecModeEn;
    pstVpssVirReg->VPSS_DIELMA2.bits.chroma_mf_offset = pstDeiPara->u16ChromMaOffset;

    pstVpssVirReg->VPSS_DIEINTEN.bits.dir_ratio_north = pstDeiPara->u32DirRatioVer;
    pstVpssVirReg->VPSS_DIEINTEN.bits.min_north_strength = pstDeiPara->s32MinVerStrength;

    pstVpssVirReg->VPSS_DIESCALE.bits.range_scale = pstDeiPara->u32ScaleRange;

    pstVpssVirReg->VPSS_DIECHECK1.bits.bc1_max_dz = pstDeiPara->u32Bc1MaxDz;
    pstVpssVirReg->VPSS_DIECHECK1.bits.bc1_autodz_gain = pstDeiPara->u32Bc1AutoDzGain;
    pstVpssVirReg->VPSS_DIECHECK1.bits.bc1_gain = pstDeiPara->u32Bc1Gain;

    pstVpssVirReg->VPSS_DIECHECK2.bits.bc2_max_dz = pstDeiPara->u32Bc2MaxDz;
    pstVpssVirReg->VPSS_DIECHECK2.bits.bc2_autodz_gain = pstDeiPara->u32Bc2AutoDzGain;
    pstVpssVirReg->VPSS_DIECHECK2.bits.bc2_gain = pstDeiPara->u32Bc2Gain;

    pstVpssVirReg->VPSS_DIEDIR0_3.bits.dir0_scale = pstDeiPara->au8DirRatio[0];
    pstVpssVirReg->VPSS_DIEDIR0_3.bits.dir1_scale = pstDeiPara->au8DirRatio[1];
    pstVpssVirReg->VPSS_DIEDIR0_3.bits.dir2_scale = pstDeiPara->au8DirRatio[2];
    pstVpssVirReg->VPSS_DIEDIR0_3.bits.dir3_scale = pstDeiPara->au8DirRatio[3];

    pstVpssVirReg->VPSS_DIEDIR4_7.bits.dir4_scale = pstDeiPara->au8DirRatio[4];
    pstVpssVirReg->VPSS_DIEDIR4_7.bits.dir5_scale = pstDeiPara->au8DirRatio[5];
    pstVpssVirReg->VPSS_DIEDIR4_7.bits.dir6_scale = pstDeiPara->au8DirRatio[6];
    pstVpssVirReg->VPSS_DIEDIR4_7.bits.dir7_scale = pstDeiPara->au8DirRatio[7];

    pstVpssVirReg->VPSS_DIEDIR8_11.bits.dir8_scale  = pstDeiPara->au8DirRatio[8];
    pstVpssVirReg->VPSS_DIEDIR8_11.bits.dir9_scale  = pstDeiPara->au8DirRatio[9];
    pstVpssVirReg->VPSS_DIEDIR8_11.bits.dir10_scale = pstDeiPara->au8DirRatio[10];
    pstVpssVirReg->VPSS_DIEDIR8_11.bits.dir11_scale = pstDeiPara->au8DirRatio[11];

    pstVpssVirReg->VPSS_DIEDIR12_14.bits.dir12_scale = pstDeiPara->au8DirRatio[12];
    pstVpssVirReg->VPSS_DIEDIR12_14.bits.dir13_scale = pstDeiPara->au8DirRatio[13];
    pstVpssVirReg->VPSS_DIEDIR12_14.bits.dir14_scale = pstDeiPara->au8DirRatio[14];

    //驱动负责更新每场的运动信息，多余的寄存器，无需配置
    //pstVpssVirReg->VPSS_DIESTPPREADDR.bits.die_ppre_addr
    //pstVpssVirReg->VPSS_DIESTPREADDR.bits.die_pre_addr

    pstVpssVirReg->VPSS_DIEINTPSCL0.bits.intp_scale_ratio_1 = pstDeiPara->au8IntpScaleRatio[0];
    pstVpssVirReg->VPSS_DIEINTPSCL0.bits.intp_scale_ratio_2 = pstDeiPara->au8IntpScaleRatio[1];
    pstVpssVirReg->VPSS_DIEINTPSCL0.bits.intp_scale_ratio_3 = pstDeiPara->au8IntpScaleRatio[2];
    pstVpssVirReg->VPSS_DIEINTPSCL0.bits.intp_scale_ratio_4 = pstDeiPara->au8IntpScaleRatio[3];
    pstVpssVirReg->VPSS_DIEINTPSCL0.bits.intp_scale_ratio_5 = pstDeiPara->au8IntpScaleRatio[4];
    pstVpssVirReg->VPSS_DIEINTPSCL0.bits.intp_scale_ratio_6 = pstDeiPara->au8IntpScaleRatio[5];
    pstVpssVirReg->VPSS_DIEINTPSCL0.bits.intp_scale_ratio_7 = pstDeiPara->au8IntpScaleRatio[6];
    pstVpssVirReg->VPSS_DIEINTPSCL0.bits.intp_scale_ratio_8 = pstDeiPara->au8IntpScaleRatio[7];

    pstVpssVirReg->VPSS_DIEINTPSCL1.bits.intp_scale_ratio_9  = pstDeiPara->au8IntpScaleRatio[8];
    pstVpssVirReg->VPSS_DIEINTPSCL1.bits.intp_scale_ratio_10 = pstDeiPara->au8IntpScaleRatio[9];
    pstVpssVirReg->VPSS_DIEINTPSCL1.bits.intp_scale_ratio_11 = pstDeiPara->au8IntpScaleRatio[10];
    pstVpssVirReg->VPSS_DIEINTPSCL1.bits.intp_scale_ratio_12 = pstDeiPara->au8IntpScaleRatio[11];
    pstVpssVirReg->VPSS_DIEINTPSCL1.bits.intp_scale_ratio_13 = pstDeiPara->au8IntpScaleRatio[12];
    pstVpssVirReg->VPSS_DIEINTPSCL1.bits.intp_scale_ratio_14 = pstDeiPara->au8IntpScaleRatio[13];
    pstVpssVirReg->VPSS_DIEINTPSCL1.bits.intp_scale_ratio_15 = pstDeiPara->au8IntpScaleRatio[14];

    pstVpssVirReg->VPSS_DIEDIRTHD.bits.bc_gain = pstDeiPara->u32BcGain;
    pstVpssVirReg->VPSS_DIEDIRTHD.bits.dir_thd = pstDeiPara->u32DirThd;
    pstVpssVirReg->VPSS_DIEDIRTHD.bits.edge_mode = pstDeiPara->u8EdgeMode;
    pstVpssVirReg->VPSS_DIEDIRTHD.bits.hor_edge_en = pstDeiPara->u8HorEdgeEn;
    pstVpssVirReg->VPSS_DIEDIRTHD.bits.strength_thd = pstDeiPara->s32StrengthThd;

    pstVpssVirReg->VPSS_DIEJITMTN.bits.jitter_gain = pstDeiPara->u16JitterGain;
    pstVpssVirReg->VPSS_DIEJITMTN.bits.jitter_coring = pstDeiPara->u16JitterCoring;
    pstVpssVirReg->VPSS_DIEJITMTN.bits.fld_motion_coring = pstDeiPara->u16FldMotionCoring;

    pstVpssVirReg->VPSS_DIEFLDMTN.bits.fld_motion_thd_low = pstDeiPara->u16FldMotionThdLow;
    pstVpssVirReg->VPSS_DIEFLDMTN.bits.fld_motion_thd_high = pstDeiPara->u16FldMotionThdHigh;
    pstVpssVirReg->VPSS_DIEFLDMTN.bits.fld_motion_curve_slope = pstDeiPara->s8FldMotionCurveSlope;
    pstVpssVirReg->VPSS_DIEFLDMTN.bits.fld_motion_gain = pstDeiPara->u8FldMotionGain;

    pstVpssVirReg->VPSS_DIEMTNDIFFTHD0.bits.motion_diff_thd_0 = pstDeiPara->au16MotionDiffThd[0];
    pstVpssVirReg->VPSS_DIEMTNDIFFTHD0.bits.motion_diff_thd_1 = pstDeiPara->au16MotionDiffThd[1];
    pstVpssVirReg->VPSS_DIEMTNDIFFTHD0.bits.motion_diff_thd_2 = pstDeiPara->au16MotionDiffThd[2];
    pstVpssVirReg->VPSS_DIEMTNDIFFTHD0.bits.motion_diff_thd_3 = pstDeiPara->au16MotionDiffThd[3];

    pstVpssVirReg->VPSS_DIEMTNDIFFTHD1.bits.motion_diff_thd_4 = pstDeiPara->au16MotionDiffThd[4];
    pstVpssVirReg->VPSS_DIEMTNDIFFTHD1.bits.motion_diff_thd_5 = pstDeiPara->au16MotionDiffThd[5];
    pstVpssVirReg->VPSS_DIEMTNDIFFTHD1.bits.min_motion_iir_ratio = pstDeiPara->u16MinMotionIIrRatio;
    pstVpssVirReg->VPSS_DIEMTNDIFFTHD1.bits.max_motion_iir_ratio = pstDeiPara->u16MaxMotionIIrRatio;

    pstVpssVirReg->VPSS_DIEMTNIIRSLP.bits.motion_iir_curve_slope_0 = pstDeiPara->as16MotionIIRCurveSlope[0];
    pstVpssVirReg->VPSS_DIEMTNIIRSLP.bits.motion_iir_curve_slope_1 = pstDeiPara->as16MotionIIRCurveSlope[1];
    pstVpssVirReg->VPSS_DIEMTNIIRSLP.bits.motion_iir_curve_slope_2 = pstDeiPara->as16MotionIIRCurveSlope[2];
    pstVpssVirReg->VPSS_DIEMTNIIRSLP.bits.motion_iir_curve_slope_3 = pstDeiPara->as16MotionIIRCurveSlope[3];
    pstVpssVirReg->VPSS_DIEMTNIIRSLP.bits.motion_iir_curve_ratio_0 = pstDeiPara->au16MotionIIRRatio[0];

    pstVpssVirReg->VPSS_DIEMTNIIRRAT.bits.motion_iir_curve_ratio_1 = pstDeiPara->au16MotionIIRRatio[1];
    pstVpssVirReg->VPSS_DIEMTNIIRRAT.bits.motion_iir_curve_ratio_2 = pstDeiPara->au16MotionIIRRatio[2];
    pstVpssVirReg->VPSS_DIEMTNIIRRAT.bits.motion_iir_curve_ratio_3 = pstDeiPara->au16MotionIIRRatio[3];
    pstVpssVirReg->VPSS_DIEMTNIIRRAT.bits.motion_iir_curve_ratio_4 = pstDeiPara->au16MotionIIRRatio[4];

    pstVpssVirReg->VPSS_DIEHISMODE.bits.rec_mode_fld_motion_step_0 = pstDeiPara->au8RecModeFldMotionStep[0];
    pstVpssVirReg->VPSS_DIEHISMODE.bits.rec_mode_fld_motion_step_1 = pstDeiPara->au8RecModeFldMotionStep[1];
    pstVpssVirReg->VPSS_DIEHISMODE.bits.rec_mode_frm_motion_step_0 = pstDeiPara->au8RecModeFrmMotionStep[0];
    pstVpssVirReg->VPSS_DIEHISMODE.bits.rec_mode_frm_motion_step_1 =  pstDeiPara->au8RecModeFrmMotionStep[1];
    pstVpssVirReg->VPSS_DIEHISMODE.bits.ppre_info_en = pstDeiPara->bPpreInfoEn;
    pstVpssVirReg->VPSS_DIEHISMODE.bits.pre_info_en = pstDeiPara->bPreInfoEn;
    pstVpssVirReg->VPSS_DIEHISMODE.bits.his_motion_en = pstDeiPara->bHisMotionEn;
    pstVpssVirReg->VPSS_DIEHISMODE.bits.his_motion_using_mode = pstDeiPara->bHisMotionUsingMode;
    pstVpssVirReg->VPSS_DIEHISMODE.bits.his_motion_write_mode = pstDeiPara->bHisMotionWriteMode;
    pstVpssVirReg->VPSS_DIEHISMODE.bits.his_motion_info_write_mode = pstDeiPara->bMotionInfoWriteMode;

    pstVpssVirReg->VPSS_DIEMORFLT.bits.mor_flt_thd = pstDeiPara->u8MorFltThd;
    pstVpssVirReg->VPSS_DIEMORFLT.bits.mor_flt_size = pstDeiPara->u8MorFltSize;
    pstVpssVirReg->VPSS_DIEMORFLT.bits.mor_flt_en = pstDeiPara->bMorFltEn;
    pstVpssVirReg->VPSS_DIEMORFLT.bits.med_blend_en = pstDeiPara->bMedBlendEn;

    pstVpssVirReg->VPSS_DIECOMBCHK0.bits.comb_chk_min_hthd = pstDeiPara->u16CombChkMinHThd;
    pstVpssVirReg->VPSS_DIECOMBCHK0.bits.comb_chk_min_vthd = pstDeiPara->u16CombChkMinVThd;
    pstVpssVirReg->VPSS_DIECOMBCHK0.bits.comb_chk_lower_limit = pstDeiPara->u16CombChkLowerLimit;
    pstVpssVirReg->VPSS_DIECOMBCHK0.bits.comb_chk_upper_limit = pstDeiPara->u16CombChkUpperLimit;

    pstVpssVirReg->VPSS_DIECOMBCHK1.bits.comb_chk_edge_thd = pstDeiPara->u16CombChkEdgeThd;
    pstVpssVirReg->VPSS_DIECOMBCHK1.bits.comb_chk_md_thd = pstDeiPara->u16CombChkMdThd;
    pstVpssVirReg->VPSS_DIECOMBCHK1.bits.comb_chk_en = pstDeiPara->bCombChkEn;

    pstVpssVirReg->VPSS_DIEFRMMTNSMTHTHD0.bits.frame_motion_smooth_thd0 = pstDeiPara->au16FrameMotionSmoothThd[0];
    pstVpssVirReg->VPSS_DIEFRMMTNSMTHTHD0.bits.frame_motion_smooth_thd1 = pstDeiPara->au16FrameMotionSmoothThd[1];
    pstVpssVirReg->VPSS_DIEFRMMTNSMTHTHD0.bits.frame_motion_smooth_thd2 = pstDeiPara->au16FrameMotionSmoothThd[2];
    pstVpssVirReg->VPSS_DIEFRMMTNSMTHTHD0.bits.frame_motion_smooth_thd3 = pstDeiPara->au16FrameMotionSmoothThd[3];

    pstVpssVirReg->VPSS_DIEFRMMTNSMTHTHD1.bits.frame_motion_smooth_thd4 = pstDeiPara->au16FrameMotionSmoothThd[4];
    pstVpssVirReg->VPSS_DIEFRMMTNSMTHTHD1.bits.frame_motion_smooth_thd5 = pstDeiPara->au16FrameMotionSmoothThd[5];
    pstVpssVirReg->VPSS_DIEFRMMTNSMTHTHD1.bits.frame_motion_smooth_ratio_min = pstDeiPara->u16MinFrameMotionSmoothRatio;
    pstVpssVirReg->VPSS_DIEFRMMTNSMTHTHD1.bits.frame_motion_smooth_ratio_max = pstDeiPara->u16MaxFrameMotionSmoothRatio;

    pstVpssVirReg->VPSS_DIEFRMMTNSMTHSLP.bits.frame_motion_smooth_slope0 = pstDeiPara->as16FrameMotionSmoothSlope[0];
    pstVpssVirReg->VPSS_DIEFRMMTNSMTHSLP.bits.frame_motion_smooth_slope1 = pstDeiPara->as16FrameMotionSmoothSlope[1];
    pstVpssVirReg->VPSS_DIEFRMMTNSMTHSLP.bits.frame_motion_smooth_slope2 = pstDeiPara->as16FrameMotionSmoothSlope[2];
    pstVpssVirReg->VPSS_DIEFRMMTNSMTHSLP.bits.frame_motion_smooth_slope3 = pstDeiPara->as16FrameMotionSmoothSlope[3];
    pstVpssVirReg->VPSS_DIEFRMMTNSMTHSLP.bits.frame_motion_smooth_ratio0 = pstDeiPara->au16FrameMotionSmoothRatio[0];

    pstVpssVirReg->VPSS_DIEFRMMTNSMTHRAT.bits.frame_motion_smooth_ratio1 = pstDeiPara->au16FrameMotionSmoothRatio[1];
    pstVpssVirReg->VPSS_DIEFRMMTNSMTHRAT.bits.frame_motion_smooth_ratio2 = pstDeiPara->au16FrameMotionSmoothRatio[2];
    pstVpssVirReg->VPSS_DIEFRMMTNSMTHRAT.bits.frame_motion_smooth_ratio3 = pstDeiPara->au16FrameMotionSmoothRatio[3];
    pstVpssVirReg->VPSS_DIEFRMMTNSMTHRAT.bits.frame_motion_smooth_ratio4 = pstDeiPara->au16FrameMotionSmoothRatio[4];

    pstVpssVirReg->VPSS_DIEFRMFLDBLENDTHD0.bits.frame_field_blend_thd0 = pstDeiPara->au16FrmFldBlendThd[0];
    pstVpssVirReg->VPSS_DIEFRMFLDBLENDTHD0.bits.frame_field_blend_thd1 = pstDeiPara->au16FrmFldBlendThd[1];
    pstVpssVirReg->VPSS_DIEFRMFLDBLENDTHD0.bits.frame_field_blend_thd2 = pstDeiPara->au16FrmFldBlendThd[2];
    pstVpssVirReg->VPSS_DIEFRMFLDBLENDTHD0.bits.frame_field_blend_thd3 = pstDeiPara->au16FrmFldBlendThd[3];

    pstVpssVirReg->VPSS_DIEFRMFLDBLENDTHD1.bits.frame_field_blend_thd4 = pstDeiPara->au16FrmFldBlendThd[4];
    pstVpssVirReg->VPSS_DIEFRMFLDBLENDTHD1.bits.frame_field_blend_thd5 = pstDeiPara->au16FrmFldBlendThd[5];
    pstVpssVirReg->VPSS_DIEFRMFLDBLENDTHD1.bits.frame_field_blend_ratio_min = pstDeiPara->u16MinFrmFldBlendRatio;
    pstVpssVirReg->VPSS_DIEFRMFLDBLENDTHD1.bits.frame_field_blend_ratio_max = pstDeiPara->u16MaxFrmFldBlendRatio;

    pstVpssVirReg->VPSS_DIEFRMFLDBLENDSLP.bits.frame_field_blend_slope0 = pstDeiPara->as16FrmFldBlendSlope[0];
    pstVpssVirReg->VPSS_DIEFRMFLDBLENDSLP.bits.frame_field_blend_slope1 = pstDeiPara->as16FrmFldBlendSlope[1];
    pstVpssVirReg->VPSS_DIEFRMFLDBLENDSLP.bits.frame_field_blend_slope2 = pstDeiPara->as16FrmFldBlendSlope[2];
    pstVpssVirReg->VPSS_DIEFRMFLDBLENDSLP.bits.frame_field_blend_slope3 = pstDeiPara->as16FrmFldBlendSlope[3];
    pstVpssVirReg->VPSS_DIEFRMFLDBLENDSLP.bits.frame_field_blend_ratio0 = pstDeiPara->au16FrmFldBlendRatio[0];

    pstVpssVirReg->VPSS_DIEFRMFLDBLENDRAT.bits.frame_field_blend_ratio1 = pstDeiPara->au16FrmFldBlendRatio[1];
    pstVpssVirReg->VPSS_DIEFRMFLDBLENDRAT.bits.frame_field_blend_ratio2 = pstDeiPara->au16FrmFldBlendRatio[2];
    pstVpssVirReg->VPSS_DIEFRMFLDBLENDRAT.bits.frame_field_blend_ratio3 = pstDeiPara->au16FrmFldBlendRatio[3];
    pstVpssVirReg->VPSS_DIEFRMFLDBLENDRAT.bits.frame_field_blend_ratio4 = pstDeiPara->au16FrmFldBlendRatio[4];

    pstVpssVirReg->VPSS_DIEMTNADJ.bits.motion_adjust_gain = pstDeiPara->u16AdjustGain;
    pstVpssVirReg->VPSS_DIEMTNADJ.bits.motion_adjust_coring = pstDeiPara->u16AdjustCoring;
    pstVpssVirReg->VPSS_DIEMTNADJ.bits.motion_adjust_gain_chr = pstDeiPara->u16AdjustGainChroma;

    //pstVpssVirReg->VPSS_DIEGLBMTNTHD.bits.small_motion_thd = pstDeiPara->u32SmallmotionThd; /* 5. AV500已经删除 */
    //pstVpssVirReg->VPSS_DIEGLBMTNTHD.bits.large_motion_thd = pstDeiPara->u32LargemotionThd;

    pstVpssVirReg->VPSS_CTRL.bits.igbm_en = pstDeiPara->bIgblEn;

    //add by z214841 2015.7.9
    pstVpssVirReg->VPSS_MCDI_NUMT.bits.numt_lpf_en = pstDeiPara->numt_lpf_en;
    pstVpssVirReg->VPSS_MCDI_NUMT.bits.numt_gain = pstDeiPara->numt_gain;
    pstVpssVirReg->VPSS_MCDI_NUMT.bits.numt_coring = pstDeiPara->numt_coring;

#if 1
    //MC 基础参数
    pstVpssVirReg->VPSS_DIECTRL.bits.mc_only = pstDeiPara->mcdeiparams.MC_ONLY;
    pstVpssVirReg->VPSS_DIECTRL.bits.ma_only = pstDeiPara->mcdeiparams.MA_ONLY;

    pstVpssVirReg->VPSS_CTRL.bits.meds_en = pstDeiPara->mcdeiparams.MEDS_EN; /* 下采样使能信号 */

    pstVpssVirReg->VPSS_MCDI_RGMAG0.bits.core_mag_rg = pstDeiPara->mcdeiparams.CORE_MAG_RG;
    pstVpssVirReg->VPSS_MCDI_RGMAG0.bits.lmt_mag_rg = pstDeiPara->mcdeiparams.LMT_MAG_RG;

    pstVpssVirReg->VPSS_MCDI_RGDIFY.bits.k_rgdifycore = pstDeiPara->mcdeiparams.K_RGDIFYCORE;
    pstVpssVirReg->VPSS_MCDI_RGDIFY.bits.g_rgdifycore = pstDeiPara->mcdeiparams.G_RGDIFYCORE;
    pstVpssVirReg->VPSS_MCDI_RGDIFY.bits.core_rgdify = pstDeiPara->mcdeiparams.CORE_RGDIFY;
    pstVpssVirReg->VPSS_MCDI_RGDIFY.bits.lmt_rgdify  = pstDeiPara->mcdeiparams.LMT_RGDIFY ;

    pstVpssVirReg->VPSS_MCDI_RGSAD.bits.coef_sadlpf = pstDeiPara->mcdeiparams.COEF_SADLPF;
    pstVpssVirReg->VPSS_MCDI_RGSAD.bits.kmv_rgsad = pstDeiPara->mcdeiparams.KMV_RGSAD;
    pstVpssVirReg->VPSS_MCDI_RGSAD.bits.k_tpdif_rgsad = pstDeiPara->mcdeiparams.K_TPDIF_RGSAD;
    pstVpssVirReg->VPSS_MCDI_RGSAD.bits.g_tpdif_rgsad = pstDeiPara->mcdeiparams.G_TPDIF_RGSAD;

    pstVpssVirReg->VPSS_MCDI_RGMV.bits.thmag_rgmv = pstDeiPara->mcdeiparams.THMAG_RGMV;
    pstVpssVirReg->VPSS_MCDI_RGMV.bits.th_saddif_rgmv = pstDeiPara->mcdeiparams.TH_SADDIF_RGMV;
    pstVpssVirReg->VPSS_MCDI_RGMV.bits.th_0mvsad_rgmv = pstDeiPara->mcdeiparams.TH_0MVSAD_RGMV;

    pstVpssVirReg->VPSS_MCDI_RGMAG1.bits.core_mv_rgmvls = pstDeiPara->mcdeiparams.CORE_MV_RGMVLS;
    pstVpssVirReg->VPSS_MCDI_RGMAG1.bits.k_mv_rgmvls = pstDeiPara->mcdeiparams.K_MV_RGMVLS;
    pstVpssVirReg->VPSS_MCDI_RGMAG1.bits.core_mag_rgmvls = pstDeiPara->mcdeiparams.CORE_MAG_RGMVLS;
    pstVpssVirReg->VPSS_MCDI_RGMAG1.bits.k_mag_rgmvls = pstDeiPara->mcdeiparams.K_MAG_RGMVLS;
    pstVpssVirReg->VPSS_MCDI_RGMAG1.bits.th_mvadj_rgmvls = pstDeiPara->mcdeiparams.TH_MVADJ_RGMVLS;
    pstVpssVirReg->VPSS_MCDI_RGMAG1.bits.en_mvadj_rgmvls = pstDeiPara->mcdeiparams.EN_MVADJ_RGMVLS;

    //  pstVpssVirReg->VPSS_MCDI_BLKH.bits.blkmvstartc = pstDeiPara->mcdeiparams.BLKMVSTARTC;
    //  pstVpssVirReg->VPSS_MCDI_BLKH.bits.blkmvendc = pstDeiPara->mcdeiparams.BLKMVENDC;

    //  pstVpssVirReg->VPSS_MCDI_CAN_MV.bits.th_zeroblkmv = pstDeiPara->mcdeiparams.TH_ZEROBLKMV;
    //  pstVpssVirReg->VPSS_MCDI_CAN_MV.bits.ksad_rgmv = pstDeiPara->mcdeiparams.KSAD_RGMV;
    //  pstVpssVirReg->VPSS_MCDI_CAN_MV.bits.addsad_neibrgmv = pstDeiPara->mcdeiparams.ADDSAD_NEIBRGMV;

    //  pstVpssVirReg->VPSS_MCDI_MV_REF.bits.core_mvx_refsad = pstDeiPara->mcdeiparams.CORE_MVX_REFSAD;
    //  pstVpssVirReg->VPSS_MCDI_MV_REF.bits.k_mvx_refsad = pstDeiPara->mcdeiparams.K_MVX_REFSAD;
    //  pstVpssVirReg->VPSS_MCDI_MV_REF.bits.min_mvx_refsad = pstDeiPara->mcdeiparams.MIN_MVX_REFSAD;
    //  pstVpssVirReg->VPSS_MCDI_MV_REF.bits.max_mvx_refsad = pstDeiPara->mcdeiparams.MAX_MVX_REFSAD;

    //  pstVpssVirReg->VPSS_MCDI_BLK_SAD0.bits.max_hfcore = pstDeiPara->mcdeiparams.MAX_HFCORE;
    //  pstVpssVirReg->VPSS_MCDI_BLK_SAD0.bits.k_maxhfcore = pstDeiPara->mcdeiparams.K_MAXHFCORE;
    //  pstVpssVirReg->VPSS_MCDI_BLK_SAD0.bits.k_hfxcore = pstDeiPara->mcdeiparams.K_HFXCORE;
    //  pstVpssVirReg->VPSS_MCDI_BLK_SAD0.bits.k_hfycore = pstDeiPara->mcdeiparams.K_HFYCORE;

    //  pstVpssVirReg->VPSS_MCDI_BLK_SAD1.bits.core_mag_blk = pstDeiPara->mcdeiparams.CORE_MAG_BLK;
    //  pstVpssVirReg->VPSS_MCDI_BLK_SAD1.bits.lmt_mag_blk = pstDeiPara->mcdeiparams.LMT_MAG_BLK;
    //  pstVpssVirReg->VPSS_MCDI_BLK_SAD1.bits.diflmt_sad_blk = pstDeiPara->mcdeiparams.DIFLMT_SAD_BLK;

    //  pstVpssVirReg->VPSS_MCDI_K_PNL0.bits.core_mv_kpnl = pstDeiPara->mcdeiparams.CORE_MV_KPNL;
    //  pstVpssVirReg->VPSS_MCDI_K_PNL0.bits.k_mv_kpnl = pstDeiPara->mcdeiparams.K_MV_KPNL;
    //  pstVpssVirReg->VPSS_MCDI_K_PNL0.bits.core_ksaddif_kpnl = pstDeiPara->mcdeiparams.CORE_KSADDIF_KPNL;
    //  pstVpssVirReg->VPSS_MCDI_K_PNL0.bits.k_ksaddif_kpnl = pstDeiPara->mcdeiparams.K_KSADDIF_KPNL;

    //  pstVpssVirReg->VPSS_MCDI_K_PNL1.bits.g_ksaddif_kpnl = pstDeiPara->mcdeiparams.G_KSADDIF_KPNL;
    //  pstVpssVirReg->VPSS_MCDI_K_PNL1.bits.core_rgmag_kpnl = pstDeiPara->mcdeiparams.CORE_RGMAG_KPNL;
    //  pstVpssVirReg->VPSS_MCDI_K_PNL1.bits.k_rgmag_kpnl = pstDeiPara->mcdeiparams.K_RGMAG_KPNL;
    //  pstVpssVirReg->VPSS_MCDI_K_PNL1.bits.th_rgsad_kpnl = pstDeiPara->mcdeiparams.TH_RGSAD_KPNL;

    //  pstVpssVirReg->VPSS_MCDI_K_PNL2.bits.k_rgsad_kpnl = pstDeiPara->mcdeiparams.K_RGSAD_KPNL;
    //  pstVpssVirReg->VPSS_MCDI_K_PNL2.bits.core_rgls_kpnl = pstDeiPara->mcdeiparams.CORE_RGLS_KPNL;
    //  pstVpssVirReg->VPSS_MCDI_K_PNL2.bits.k_w_selrgmv = pstDeiPara->mcdeiparams.K_W_SELRGMV;
    //  pstVpssVirReg->VPSS_MCDI_K_PNL2.bits.th_ratio_kpnl = pstDeiPara->mcdeiparams.TH_RATIO_KPNL;

    //  pstVpssVirReg->VPSS_MCDI_K_PNL3.bits.k_ratio_kpnl = pstDeiPara->mcdeiparams.K_RATIO_KPNL;
    //  pstVpssVirReg->VPSS_MCDI_K_PNL3.bits.g_ratio_kpnl = pstDeiPara->mcdeiparams.G_RATIO_KPNL;
    //  pstVpssVirReg->VPSS_MCDI_K_PNL3.bits.k_pnl_sad = pstDeiPara->mcdeiparams.K_PNL_SAD;
    //  pstVpssVirReg->VPSS_MCDI_K_PNL3.bits.g_pnl_sad = pstDeiPara->mcdeiparams.G_PNL_SAD;

    //  pstVpssVirReg->VPSS_MCDI_K_PNL_ADJ.bits.x_rgmv_kpnl = pstDeiPara->mcdeiparams.X_RGMV_KPNL;
    //  pstVpssVirReg->VPSS_MCDI_K_PNL_ADJ.bits.k_rgmv_kpnl = pstDeiPara->mcdeiparams.K_RGMV_KPNL;
    //  pstVpssVirReg->VPSS_MCDI_K_PNL_ADJ.bits.x_difpnl_kpnl = pstDeiPara->mcdeiparams.X_DIFPNL_KPNL;
    //  pstVpssVirReg->VPSS_MCDI_K_PNL_ADJ.bits.k_difpnl_kpnl = pstDeiPara->mcdeiparams.K_DIFPNL_KPNL;
    //  pstVpssVirReg->VPSS_MCDI_K_PNL_ADJ.bits.g_difpnl_kpnl = pstDeiPara->mcdeiparams.G_DIFPNL_KPNL;

    //  pstVpssVirReg->VPSS_MCDI_VALUE_PNL.bits.core_mvdst_pnl = pstDeiPara->mcdeiparams.CORE_MVDST_PNL;
    //  pstVpssVirReg->VPSS_MCDI_VALUE_PNL.bits.k_mvpnl = pstDeiPara->mcdeiparams.K_MVPNL;
    //  pstVpssVirReg->VPSS_MCDI_VALUE_PNL.bits.g_mvpnl = pstDeiPara->mcdeiparams.G_MVPNL;

    //  pstVpssVirReg->VPSS_MCDI_SP_PNL0.bits.k_sppnl = pstDeiPara->mcdeiparams.K_SPPNL;
    //  pstVpssVirReg->VPSS_MCDI_SP_PNL0.bits.g_sppnl = pstDeiPara->mcdeiparams.G_SPPNL;
    //  pstVpssVirReg->VPSS_MCDI_SP_PNL0.bits.core_wmag_sppnl = pstDeiPara->mcdeiparams.CORE_WMAG_SPPNL;
    //  pstVpssVirReg->VPSS_MCDI_SP_PNL0.bits.k_wmag_sppnl = pstDeiPara->mcdeiparams.K_WMAG_SPPNL;

    //  pstVpssVirReg->VPSS_MCDI_SP_PNL1.bits.b_wmag_sppnl = pstDeiPara->mcdeiparams.B_WMAG_SPPNL;
    //  pstVpssVirReg->VPSS_MCDI_SP_PNL1.bits.th_wsad_sppnl = pstDeiPara->mcdeiparams.TH_WSAD_SPPNL;
    //  pstVpssVirReg->VPSS_MCDI_SP_PNL1.bits.k_wsad_sppnl = pstDeiPara->mcdeiparams.K_WSAD_SPPNL;
    //  pstVpssVirReg->VPSS_MCDI_SP_PNL1.bits.b_wsad_sppnl = pstDeiPara->mcdeiparams.B_WSAD_SPPNL;

    //  pstVpssVirReg->VPSS_MCDI_TP_PNL0.bits.core_wmag_tppnl = pstDeiPara->mcdeiparams.CORE_WMAG_TPPNL;
    //  pstVpssVirReg->VPSS_MCDI_TP_PNL0.bits.k_wmag_tppnl = pstDeiPara->mcdeiparams.K_WMAG_TPPNL;
    //  pstVpssVirReg->VPSS_MCDI_TP_PNL0.bits.k_tppnl = pstDeiPara->mcdeiparams.K_TPPNL;
    //  pstVpssVirReg->VPSS_MCDI_TP_PNL0.bits.g_tppnl = pstDeiPara->mcdeiparams.G_TPPNL;

    //  pstVpssVirReg->VPSS_MCDI_TP_PNL1.bits.b_wmag_tppnl = pstDeiPara->mcdeiparams.B_WMAG_TPPNL;
    //  pstVpssVirReg->VPSS_MCDI_TP_PNL1.bits.th_wsad_tppnl = pstDeiPara->mcdeiparams.TH_WSAD_TPPNL;
    //  pstVpssVirReg->VPSS_MCDI_TP_PNL1.bits.k_wsad_tppnl = pstDeiPara->mcdeiparams.K_WSAD_TPPNL;
    //  pstVpssVirReg->VPSS_MCDI_TP_PNL1.bits.b_wsad_tppnl = pstDeiPara->mcdeiparams.B_WSAD_TPPNL;

    //  pstVpssVirReg->VPSS_MCDI_RGMV_PNL0.bits.core_sad_rgpnl = pstDeiPara->mcdeiparams.CORE_SAD_RGPNL;
    //  pstVpssVirReg->VPSS_MCDI_RGMV_PNL0.bits.k_sad_rgpnl = pstDeiPara->mcdeiparams.K_SAD_RGPNL;
    //  pstVpssVirReg->VPSS_MCDI_RGMV_PNL0.bits.g_ls_rgpnl = pstDeiPara->mcdeiparams.G_LS_RGPNL;
    //  pstVpssVirReg->VPSS_MCDI_RGMV_PNL0.bits.th_mag_rgpnl = pstDeiPara->mcdeiparams.TH_MAG_RGPNL;

    //  pstVpssVirReg->VPSS_MCDI_RGMV_PNL1.bits.g_sad_rgpnl = pstDeiPara->mcdeiparams.G_SAD_RGPNL;
    //  pstVpssVirReg->VPSS_MCDI_RGMV_PNL1.bits.k_mag_rgpnl = pstDeiPara->mcdeiparams.K_MAG_RGPNL;
    //  pstVpssVirReg->VPSS_MCDI_RGMV_PNL1.bits.g_mag_rgpnl = pstDeiPara->mcdeiparams.G_MAG_RGPNL;
    //  pstVpssVirReg->VPSS_MCDI_RGMV_PNL1.bits.th_ls_rgpnl = pstDeiPara->mcdeiparams.TH_LS_RGPNL;
    //  pstVpssVirReg->VPSS_MCDI_RGMV_PNL1.bits.k_ls_rgpnl = pstDeiPara->mcdeiparams.K_LS_RGPNL;

    //  pstVpssVirReg->VPSS_MCDI_MAG_PNL0.bits.core_blkmag_magpnl = pstDeiPara->mcdeiparams.CORE_BLKMAG_MAGPNL;
    //  pstVpssVirReg->VPSS_MCDI_MAG_PNL0.bits.k_magpnl = pstDeiPara->mcdeiparams.K_MAGPNL;
    //  pstVpssVirReg->VPSS_MCDI_MAG_PNL0.bits.g_magpnl = pstDeiPara->mcdeiparams.G_MAGPNL;
    //  pstVpssVirReg->VPSS_MCDI_MAG_PNL0.bits.core_rgmag_magpnl = pstDeiPara->mcdeiparams.CORE_RGMAG_MAGPNL;

    //  pstVpssVirReg->VPSS_MCDI_MAG_PNL1.bits.k_rgmag_magpnl = pstDeiPara->mcdeiparams.K_RGMAG_MAGPNL;
    //  pstVpssVirReg->VPSS_MCDI_MAG_PNL1.bits.th_rgsad_magpnl = pstDeiPara->mcdeiparams.TH_RGSAD_MAGPNL;
    //  pstVpssVirReg->VPSS_MCDI_MAG_PNL1.bits.k_rgsad_magpnl = pstDeiPara->mcdeiparams.K_RGSAD_MAGPNL;
    //  pstVpssVirReg->VPSS_MCDI_MAG_PNL1.bits.core_rgls_magpnl = pstDeiPara->mcdeiparams.CORE_RGLS_MAGPNL;

    //  pstVpssVirReg->VPSS_MCDI_MAG_PNL2.bits.k_rgls_magpnl = pstDeiPara->mcdeiparams.K_RGLS_MAGPNL;
    //  pstVpssVirReg->VPSS_MCDI_MAG_PNL2.bits.k_rgmv_magpnl = pstDeiPara->mcdeiparams.K_RGMV_MAGPNL;
    //  pstVpssVirReg->VPSS_MCDI_MAG_PNL2.bits.g_rgmv_magpnl= pstDeiPara->mcdeiparams.G_RGMV_MAGPNL;

    //  pstVpssVirReg->VPSS_MCDI_ZEROMV_PNL0.bits.core_rgmvdif_0mvpnl = pstDeiPara->mcdeiparams.CORE_RGMVDIF_0MVPNL;
    //  pstVpssVirReg->VPSS_MCDI_ZEROMV_PNL0.bits.k_rgmvdif_0mvpnl = pstDeiPara->mcdeiparams.K_RGMVDIF_0MVPNL;
    //  pstVpssVirReg->VPSS_MCDI_ZEROMV_PNL0.bits.core_rgmag_0mvpnl = pstDeiPara->mcdeiparams.CORE_RGMAG_0MVPNL;
    //  pstVpssVirReg->VPSS_MCDI_ZEROMV_PNL0.bits.k_rgmag_0mvpnl = pstDeiPara->mcdeiparams.K_RGMAG_0MVPNL;

    //  pstVpssVirReg->VPSS_MCDI_ZEROMV_PNL1.bits.core_rgls_0mvpnl = pstDeiPara->mcdeiparams.CORE_RGLS_0MVPNL;
    //  pstVpssVirReg->VPSS_MCDI_ZEROMV_PNL1.bits.k_rgls_0mvpnl = pstDeiPara->mcdeiparams.K_RGLS_0MVPNL;
    //  pstVpssVirReg->VPSS_MCDI_ZEROMV_PNL1.bits.k_rg_0mvpnl = pstDeiPara->mcdeiparams.K_RG_0MVPNL;
    //  pstVpssVirReg->VPSS_MCDI_ZEROMV_PNL1.bits.g_rg_0mvpnl = pstDeiPara->mcdeiparams.G_RG_0MVPNL;

    pstVpssVirReg->VPSS_MCDI_SEL.bits.force_mven = pstDeiPara->mcdeiparams.FORCE_MVEN;
    pstVpssVirReg->VPSS_MCDI_SEL.bits.force_mvx = pstDeiPara->mcdeiparams.FORCE_MVX;
    //  pstVpssVirReg->VPSS_MCDI_SEL.bits.en_mvdilation = pstDeiPara->mcdeiparams.EN_MVDILATION;

    //  pstVpssVirReg->VPSS_MCDI_BLK_POSTPRO0.bits.th0_mvdist_mvspchk = pstDeiPara->mcdeiparams.TH0_MVDIST_MVSPCHK;
    //  pstVpssVirReg->VPSS_MCDI_BLK_POSTPRO0.bits.th1_mvdist_mvspchk = pstDeiPara->mcdeiparams.TH1_MVDIST_MVSPCHK;
    //  pstVpssVirReg->VPSS_MCDI_BLK_POSTPRO0.bits.th_mag_mvspchk = pstDeiPara->mcdeiparams.TH_MAG_MVSPCHK;
    //  pstVpssVirReg->VPSS_MCDI_BLK_POSTPRO0.bits.th_wrg_mvspchk = pstDeiPara->mcdeiparams.TH_WRG_MVSPCHK;

    //  pstVpssVirReg->VPSS_MCDI_BLK_POSTPRO1.bits.th_sad_mvspchk = pstDeiPara->mcdeiparams.TH_SAD_MVSPCHK;
    //  pstVpssVirReg->VPSS_MCDI_BLK_POSTPRO1.bits.th0_mvdist_mvspdlt = pstDeiPara->mcdeiparams.TH0_MVDIST_MVSPDLT;
    //  pstVpssVirReg->VPSS_MCDI_BLK_POSTPRO1.bits.th1_mvdist_mvspdlt = pstDeiPara->mcdeiparams.TH1_MVDIST_MVSPDLT;
    //  pstVpssVirReg->VPSS_MCDI_BLK_POSTPRO1.bits.th_mag_mvspdlt = pstDeiPara->mcdeiparams.TH_MAG_MVSPDLT;

    //  pstVpssVirReg->VPSS_MCDI_BLK_POSTPRO2.bits.th_wrg_mvspdlt = pstDeiPara->mcdeiparams.TH_WRG_MVSPDLT;
    //  pstVpssVirReg->VPSS_MCDI_BLK_POSTPRO2.bits.th_sad_mvspdlt = pstDeiPara->mcdeiparams.TH_SAD_MVSPDLT;
    //  pstVpssVirReg->VPSS_MCDI_BLK_POSTPRO2.bits.thl_mag_mvmedfilt = pstDeiPara->mcdeiparams.THL_MAG_MVMEDFILT;
    //  pstVpssVirReg->VPSS_MCDI_BLK_POSTPRO2.bits.thh_mag_mvmedfilt = pstDeiPara->mcdeiparams.THH_MAG_MVMEDFILT;

    //  pstVpssVirReg->VPSS_MCDI_SELMCMV0.bits.g_sad_selmcmv = pstDeiPara->mcdeiparams.G_SAD_SELMCMV;
    //  pstVpssVirReg->VPSS_MCDI_SELMCMV0.bits.k_sad_selmcmv = pstDeiPara->mcdeiparams.K_SAD_SELMCMV;
    //  pstVpssVirReg->VPSS_MCDI_SELMCMV0.bits.g_mag_selmcmv = pstDeiPara->mcdeiparams.G_MAG_SELMCMV;
    //  pstVpssVirReg->VPSS_MCDI_SELMCMV0.bits.k_mag_selmcmv = pstDeiPara->mcdeiparams.K_MAG_SELMCMV;
    //  pstVpssVirReg->VPSS_MCDI_SELMCMV0.bits.k_maxmag_selmcmv = pstDeiPara->mcdeiparams.K_MAXMAG_SELMCMV;
    //  pstVpssVirReg->VPSS_MCDI_SELMCMV0.bits.k_core_simimv = pstDeiPara->mcdeiparams.K_CORE_SIMIMV;
    //  pstVpssVirReg->VPSS_MCDI_SELMCMV0.bits.gl_core_simimv = pstDeiPara->mcdeiparams.GL_CORE_SIMIMV;

    //  pstVpssVirReg->VPSS_MCDI_SELMCMV1.bits.gh_core_simimv = pstDeiPara->mcdeiparams.GH_CORE_SIMIMV;
    //  pstVpssVirReg->VPSS_MCDI_SELMCMV1.bits.k_simimvw = pstDeiPara->mcdeiparams.K_SIMIMVW;
    //  pstVpssVirReg->VPSS_MCDI_SELMCMV1.bits.k_curmag_mcw = pstDeiPara->mcdeiparams.K_CURMAG_MCW;

    pstVpssVirReg->VPSS_DIEFUSION3.bits.k_curw_mcbld = pstDeiPara->mcdeiparams.K_CURW_MCBLD;

    //  pstVpssVirReg->VPSS_MCDI_RG_MCW0.bits.b_blksad_mcw = pstDeiPara->mcdeiparams.B_BLKSAD_MCW;
    //  pstVpssVirReg->VPSS_MCDI_RG_MCW0.bits.core_rgsad_mcw = pstDeiPara->mcdeiparams.CORE_RGSAD_MCW;
    //  pstVpssVirReg->VPSS_MCDI_RG_MCW0.bits.g0_rgmag_mcw = pstDeiPara->mcdeiparams.G0_RGMAG_MCW;
    //  pstVpssVirReg->VPSS_MCDI_RG_MCW0.bits.core_rgmag_mcw = pstDeiPara->mcdeiparams.CORE_RGMAG_MCW;

    //  pstVpssVirReg->VPSS_MCDI_RG_MCW1.bits.core_mvy_mcw = pstDeiPara->mcdeiparams.CORE_MVY_MCW;
    //  pstVpssVirReg->VPSS_MCDI_RG_MCW1.bits.k_mvy_mcw = pstDeiPara->mcdeiparams.K_MVY_MCW;
    //  pstVpssVirReg->VPSS_MCDI_RG_MCW1.bits.core_rgsadadj_mcw = pstDeiPara->mcdeiparams.CORE_RGSADADJ_MCW;
    //  pstVpssVirReg->VPSS_MCDI_RG_MCW1.bits.k_rgsadadj_mcw = pstDeiPara->mcdeiparams.K_RGSADADJ_MCW;
    //  pstVpssVirReg->VPSS_MCDI_RG_MCW1.bits.k_core_vsaddif = pstDeiPara->mcdeiparams.K_CORE_VSADDIF;
    //  pstVpssVirReg->VPSS_MCDI_RG_MCW1.bits.gl_core_vsaddif = pstDeiPara->mcdeiparams.GL_CORE_VSADDIF;

    //  pstVpssVirReg->VPSS_MCDI_RG_MCW2.bits.gh_core_vsaddif = pstDeiPara->mcdeiparams.GH_CORE_VSADDIF;
    //  pstVpssVirReg->VPSS_MCDI_RG_MCW2.bits.k_vsaddifw = pstDeiPara->mcdeiparams.K_VSADDIFW;
    //  pstVpssVirReg->VPSS_MCDI_RG_MCW2.bits.mode_rgysad_mcw = pstDeiPara->mcdeiparams.MODE_RGYSAD_MCW;
    //  pstVpssVirReg->VPSS_MCDI_RG_MCW2.bits.x0_rgmag_mcw = pstDeiPara->mcdeiparams.X0_RGMAG_MCW;
    //  pstVpssVirReg->VPSS_MCDI_RG_MCW2.bits.k0_rgmag_mcw = pstDeiPara->mcdeiparams.K0_RGMAG_MCW;

    //  pstVpssVirReg->VPSS_MCDI_RG_MCW3.bits.k1_rgmag_mcw = pstDeiPara->mcdeiparams.K1_RGMAG_MCW;
    //  pstVpssVirReg->VPSS_MCDI_RG_MCW3.bits.x0_rgsad_mcw = pstDeiPara->mcdeiparams.X0_RGSAD_MCW;
    //  pstVpssVirReg->VPSS_MCDI_RG_MCW3.bits.k0_rgsad_mcw = pstDeiPara->mcdeiparams.K0_RGSAD_MCW;

    //  pstVpssVirReg->VPSS_MCDI_RG_MCW4.bits.g0_rgsad_mcw = pstDeiPara->mcdeiparams.G0_RGSAD_MCW;
    //  pstVpssVirReg->VPSS_MCDI_RG_MCW4.bits.k1_rgsad_mcw = pstDeiPara->mcdeiparams.K1_RGSAD_MCW;
    //  pstVpssVirReg->VPSS_MCDI_RG_MCW4.bits.k_rglsw = pstDeiPara->mcdeiparams.K_RGLSW;

    //  pstVpssVirReg->VPSS_MCDI_BLK_MCW0.bits.k_mag_blksadcore_mcw = pstDeiPara->mcdeiparams.K_MAG_BLKSADCORE_MCW;
    //  pstVpssVirReg->VPSS_MCDI_BLK_MCW0.bits.g_mag_blksadcore_mcw = pstDeiPara->mcdeiparams.G_MAG_BLKSADCORE_MCW;
    //  pstVpssVirReg->VPSS_MCDI_BLK_MCW0.bits.x0_mag_blk_mcw = pstDeiPara->mcdeiparams.X0_MAG_BLK_MCW;
    //  pstVpssVirReg->VPSS_MCDI_BLK_MCW0.bits.k0_mag_blk_mcw = pstDeiPara->mcdeiparams.K0_MAG_BLK_MCW;
    //  pstVpssVirReg->VPSS_MCDI_BLK_MCW0.bits.g0_mag_blk_mcw = pstDeiPara->mcdeiparams.G0_MAG_BLK_MCW;

    //  pstVpssVirReg->VPSS_MCDI_BLK_MCW1.bits.k1_mag_blk_mcw = pstDeiPara->mcdeiparams.K1_MAG_BLK_MCW;
    //  pstVpssVirReg->VPSS_MCDI_BLK_MCW1.bits.g1_mag_blk_mcw = pstDeiPara->mcdeiparams.G1_MAG_BLK_MCW;
    //  pstVpssVirReg->VPSS_MCDI_BLK_MCW1.bits.g1_sad_blk_mcw = pstDeiPara->mcdeiparams.G1_SAD_BLK_MCW;
    //  pstVpssVirReg->VPSS_MCDI_BLK_MCW1.bits.k_avgblkmcw = pstDeiPara->mcdeiparams.K_AVGBLKMCW;

    //  pstVpssVirReg->VPSS_MCDI_BLK_MCW2.bits.x0_sad_blk_mcw = pstDeiPara->mcdeiparams.X0_SAD_BLK_MCW;
    //  pstVpssVirReg->VPSS_MCDI_BLK_MCW2.bits.k0_sad_blk_mcw = pstDeiPara->mcdeiparams.K0_SAD_BLK_MCW;
    //  pstVpssVirReg->VPSS_MCDI_BLK_MCW2.bits.g0_sad_blk_mcw = pstDeiPara->mcdeiparams.G0_SAD_BLK_MCW;
    //  pstVpssVirReg->VPSS_MCDI_BLK_MCW2.bits.k1_sad_blk_mcw = pstDeiPara->mcdeiparams.K1_SAD_BLK_MCW;

    //  pstVpssVirReg->VPSS_MCDI_BLK_MCW3.bits.k_curblkmcw = pstDeiPara->mcdeiparams.K_CURBLKMCW;
    //  pstVpssVirReg->VPSS_MCDI_BLK_MCW3.bits.g_curblkmcw = pstDeiPara->mcdeiparams.G_CURBLKMCW;

    //  pstVpssVirReg->VPSS_MCDI_TPMVWEIGHT0.bits.x0_tpmvdist_mcw = pstDeiPara->mcdeiparams.X0_TPMVDIST_MCW;
    //  pstVpssVirReg->VPSS_MCDI_TPMVWEIGHT0.bits.g0_tpmvdist_mcw = pstDeiPara->mcdeiparams.G0_TPMVDIST_MCW;
    //  pstVpssVirReg->VPSS_MCDI_TPMVWEIGHT0.bits.k_minmvdist_mcw = pstDeiPara->mcdeiparams.K_MINMVDIST_MCW;
    //  pstVpssVirReg->VPSS_MCDI_TPMVWEIGHT0.bits.k_avgmvdist_mcw = pstDeiPara->mcdeiparams.K_AVGMVDIST_MCW;

    //  pstVpssVirReg->VPSS_MCDI_TPMVWEIGHT1.bits.k0_tpmvdist_mcw = pstDeiPara->mcdeiparams.K0_TPMVDIST_MCW;
    //  pstVpssVirReg->VPSS_MCDI_TPMVWEIGHT1.bits.k1_tpmvdist_mcw = pstDeiPara->mcdeiparams.K1_TPMVDIST_MCW;
    //  pstVpssVirReg->VPSS_MCDI_TPMVWEIGHT1.bits.b_core_tpmvdist_mcw = pstDeiPara->mcdeiparams.B_CORE_TPMVDIST_MCW;
    //  pstVpssVirReg->VPSS_MCDI_TPMVWEIGHT1.bits.k_core_tpmvdist_mcw = pstDeiPara->mcdeiparams.K_CORE_TPMVDIST_MCW;

    //  pstVpssVirReg->VPSS_MCDI_WNDMC0.bits.k_difhcore_mcw = pstDeiPara->mcdeiparams.K_DIFHCORE_MCW;
    //  pstVpssVirReg->VPSS_MCDI_WNDMC0.bits.k_sub_hcore_mcw = pstDeiPara->mcdeiparams.K_SUB_HCORE_MCW;
    //  pstVpssVirReg->VPSS_MCDI_WNDMC0.bits.k_difvcore_mcw = pstDeiPara->mcdeiparams.K_DIFVCORE_MCW;
    //  pstVpssVirReg->VPSS_MCDI_WNDMC0.bits.k_max_core_mcw = pstDeiPara->mcdeiparams.K_MAX_CORE_MCW;
    //  pstVpssVirReg->VPSS_MCDI_WNDMC0.bits.k_max_dif_mcw = pstDeiPara->mcdeiparams.K_MAX_DIF_MCW;

    //  pstVpssVirReg->VPSS_MCDI_WNDMC1.bits.k1_max_mag_mcw = pstDeiPara->mcdeiparams.K1_MAX_MAG_MCW;
    //  pstVpssVirReg->VPSS_MCDI_WNDMC1.bits.k0_max_mag_mcw = pstDeiPara->mcdeiparams.K0_MAX_MAG_MCW;
    //  pstVpssVirReg->VPSS_MCDI_WNDMC1.bits.k_tbdif_mcw = pstDeiPara->mcdeiparams.K_TBDIF_MCW;
    //  pstVpssVirReg->VPSS_MCDI_WNDMC1.bits.k_tbmag_mcw = pstDeiPara->mcdeiparams.K_TBMAG_MCW;
    //  pstVpssVirReg->VPSS_MCDI_WNDMC1.bits.k_blksad_wnd_mcw = pstDeiPara->mcdeiparams.K_BLKSAD_WND_MCW;
    //  pstVpssVirReg->VPSS_MCDI_WNDMC1.bits.k_mag_wndsadcore_mcw = pstDeiPara->mcdeiparams.K_MAG_WNDSADCORE_MCW;
    //  pstVpssVirReg->VPSS_MCDI_WNDMC1.bits.g_mag_wndsadcore_mcw = pstDeiPara->mcdeiparams.G_MAG_WNDSADCORE_MCW;

    //  pstVpssVirReg->VPSS_MCDI_WNDMC2.bits.x0_mag_wnd_mcw = pstDeiPara->mcdeiparams.X0_MAG_WND_MCW;
    //  pstVpssVirReg->VPSS_MCDI_WNDMC2.bits.k0_mag_wnd_mcw = pstDeiPara->mcdeiparams.K0_MAG_WND_MCW;
    //  pstVpssVirReg->VPSS_MCDI_WNDMC2.bits.g0_mag_wnd_mcw = pstDeiPara->mcdeiparams.G0_MAG_WND_MCW;
    //  pstVpssVirReg->VPSS_MCDI_WNDMC2.bits.k1_mag_wnd_mcw = pstDeiPara->mcdeiparams.K1_MAG_WND_MCW;

    //  pstVpssVirReg->VPSS_MCDI_WNDMC4.bits.g1_mag_wnd_mcw = pstDeiPara->mcdeiparams.G1_MAG_WND_MCW;
    //  pstVpssVirReg->VPSS_MCDI_WNDMC4.bits.g1_sad_wnd_mcw = pstDeiPara->mcdeiparams.G1_SAD_WND_MCW;

    //  pstVpssVirReg->VPSS_MCDI_WNDMC3.bits.x0_sad_wnd_mcw = pstDeiPara->mcdeiparams.X0_SAD_WND_MCW;
    //  pstVpssVirReg->VPSS_MCDI_WNDMC3.bits.k0_sad_wnd_mcw = pstDeiPara->mcdeiparams.K0_SAD_WND_MCW;
    //  pstVpssVirReg->VPSS_MCDI_WNDMC3.bits.g0_sad_wnd_mcw = pstDeiPara->mcdeiparams.G0_SAD_WND_MCW;
    //  pstVpssVirReg->VPSS_MCDI_WNDMC3.bits.k1_sad_wnd_mcw = pstDeiPara->mcdeiparams.K1_SAD_WND_MCW;

    //  pstVpssVirReg->VPSS_MCDI_BLKMAGSADADJ0.bits.k_magadd_data_mcw = pstDeiPara->mcdeiparams.K_MAGADD_DATA_MCW;
    //  pstVpssVirReg->VPSS_MCDI_BLKMAGSADADJ0.bits.k_sadcore_data_mcw = pstDeiPara->mcdeiparams.K_SADCORE_DATA_MCW;
    //  pstVpssVirReg->VPSS_MCDI_BLKMAGSADADJ0.bits.g_blkmagcore_mcw = pstDeiPara->mcdeiparams.G_BLKMAGCORE_MCW;
    //  pstVpssVirReg->VPSS_MCDI_BLKMAGSADADJ0.bits.g1_blksadcore_mcw = pstDeiPara->mcdeiparams.G1_BLKSADCORE_MCW;

    //  pstVpssVirReg->VPSS_MCDI_BLKMAGSADADJ2.bits.x0_k_blkmagcore_mcw = pstDeiPara->mcdeiparams.X0_K_BLKMAGCORE_MCW;
    //  pstVpssVirReg->VPSS_MCDI_BLKMAGSADADJ2.bits.g0_k_blkmagcore_mcw = pstDeiPara->mcdeiparams.G0_K_BLKMAGCORE_MCW;
    //  pstVpssVirReg->VPSS_MCDI_BLKMAGSADADJ2.bits.k0_k_blkmagcore_mcw = pstDeiPara->mcdeiparams.K0_K_BLKMAGCORE_MCW;
    //  pstVpssVirReg->VPSS_MCDI_BLKMAGSADADJ2.bits.g1_k_blkmagcore_mcw = pstDeiPara->mcdeiparams.G1_K_BLKMAGCORE_MCW;

    //  pstVpssVirReg->VPSS_MCDI_BLKMAGSADADJ3.bits.x0_blksadcore_mcw = pstDeiPara->mcdeiparams.X0_BLKSADCORE_MCW;
    //  pstVpssVirReg->VPSS_MCDI_BLKMAGSADADJ3.bits.k0_blksadcore_mcw = pstDeiPara->mcdeiparams.K0_BLKSADCORE_MCW;
    //  pstVpssVirReg->VPSS_MCDI_BLKMAGSADADJ3.bits.g0_blksadcore_mcw = pstDeiPara->mcdeiparams.G0_BLKSADCORE_MCW;
    //  pstVpssVirReg->VPSS_MCDI_BLKMAGSADADJ3.bits.k1_blksadcore_mcw = pstDeiPara->mcdeiparams.K1_BLKSADCORE_MCW;

    pstVpssVirReg->VPSS_MCDI_VERTWEIGHT0.bits.k_max_hvdif_dw = pstDeiPara->mcdeiparams.K_MAX_HVDIF_DW;
    pstVpssVirReg->VPSS_MCDI_VERTWEIGHT0.bits.gain_lpf_dw = pstDeiPara->mcdeiparams.GAIN_LPF_DW;
    pstVpssVirReg->VPSS_MCDI_VERTWEIGHT0.bits.core_bhvdif_dw = pstDeiPara->mcdeiparams.CORE_BHVDIF_DW;
    pstVpssVirReg->VPSS_MCDI_VERTWEIGHT0.bits.k_bhvdif_dw = pstDeiPara->mcdeiparams.K_BHVDIF_DW;
    pstVpssVirReg->VPSS_MCDI_VERTWEIGHT0.bits.b_bhvdif_dw = pstDeiPara->mcdeiparams.B_BHVDIF_DW;
    pstVpssVirReg->VPSS_MCDI_VERTWEIGHT0.bits.b_hvdif_dw = pstDeiPara->mcdeiparams.B_HVDIF_DW;

    pstVpssVirReg->VPSS_MCDI_VERTWEIGHT1.bits.core_hvdif_dw = pstDeiPara->mcdeiparams.CORE_HVDIF_DW;
    pstVpssVirReg->VPSS_MCDI_VERTWEIGHT1.bits.k_difv_dw = pstDeiPara->mcdeiparams.K_DIFV_DW;
    pstVpssVirReg->VPSS_MCDI_VERTWEIGHT1.bits.core_mv_dw = pstDeiPara->mcdeiparams.CORE_MV_DW;
    pstVpssVirReg->VPSS_MCDI_VERTWEIGHT1.bits.b_mv_dw = pstDeiPara->mcdeiparams.B_MV_DW;

    pstVpssVirReg->VPSS_MCDI_VERTWEIGHT2.bits.x0_hvdif_dw = pstDeiPara->mcdeiparams.X0_HVDIF_DW;
    pstVpssVirReg->VPSS_MCDI_VERTWEIGHT2.bits.k0_hvdif_dw = pstDeiPara->mcdeiparams.K0_HVDIF_DW;
    pstVpssVirReg->VPSS_MCDI_VERTWEIGHT2.bits.g0_hvdif_dw = pstDeiPara->mcdeiparams.G0_HVDIF_DW;
    pstVpssVirReg->VPSS_MCDI_VERTWEIGHT2.bits.k1_hvdif_dw = pstDeiPara->mcdeiparams.K1_HVDIF_DW;

    pstVpssVirReg->VPSS_MCDI_VERTWEIGHT3.bits.x0_mv_dw = pstDeiPara->mcdeiparams.X0_MV_DW;
    pstVpssVirReg->VPSS_MCDI_VERTWEIGHT3.bits.k0_mv_dw = pstDeiPara->mcdeiparams.K0_MV_DW;
    pstVpssVirReg->VPSS_MCDI_VERTWEIGHT3.bits.g0_mv_dw = pstDeiPara->mcdeiparams.G0_MV_DW;
    pstVpssVirReg->VPSS_MCDI_VERTWEIGHT3.bits.k1_mv_dw = pstDeiPara->mcdeiparams.K1_MV_DW;

    pstVpssVirReg->VPSS_MCDI_VERTWEIGHT4.bits.x0_mt_dw = pstDeiPara->mcdeiparams.X0_MT_DW;
    pstVpssVirReg->VPSS_MCDI_VERTWEIGHT4.bits.k0_mt_dw = pstDeiPara->mcdeiparams.K0_MT_DW;
    pstVpssVirReg->VPSS_MCDI_VERTWEIGHT4.bits.g0_mt_dw = pstDeiPara->mcdeiparams.G0_MT_DW;
    pstVpssVirReg->VPSS_MCDI_VERTWEIGHT4.bits.k1_mt_dw = pstDeiPara->mcdeiparams.K1_MT_DW;

    pstVpssVirReg->VPSS_MCDI_VERTWEIGHT5.bits.g0_mv_mt = pstDeiPara->mcdeiparams.G0_MV_MT;
    pstVpssVirReg->VPSS_MCDI_VERTWEIGHT5.bits.x0_mv_mt = pstDeiPara->mcdeiparams.X0_MV_MT;
    pstVpssVirReg->VPSS_MCDI_VERTWEIGHT5.bits.k1_mv_mt = pstDeiPara->mcdeiparams.K1_MV_MT;
    pstVpssVirReg->VPSS_MCDI_VERTWEIGHT5.bits.b_mt_dw = pstDeiPara->mcdeiparams.B_MT_DW;

    pstVpssVirReg->VPSS_MCDI_MC0.bits.gain_up_mclpfv = pstDeiPara->mcdeiparams.GAIN_UP_MCLPFV;
    pstVpssVirReg->VPSS_MCDI_MC0.bits.gain_dn_mclpfv = pstDeiPara->mcdeiparams.GAIN_DN_MCLPFV;
    pstVpssVirReg->VPSS_MCDI_MC0.bits.gain_mclpfh = pstDeiPara->mcdeiparams.GAIN_MCLPFH;
    pstVpssVirReg->VPSS_MCDI_MC0.bits.rs_pxlmag_mcw = pstDeiPara->mcdeiparams.RS_PXLMAG_MCW;
    pstVpssVirReg->VPSS_MCDI_MC0.bits.x_pxlmag_mcw = pstDeiPara->mcdeiparams.X_PXLMAG_MCW;
    pstVpssVirReg->VPSS_MCDI_MC0.bits.k_pxlmag_mcw = pstDeiPara->mcdeiparams.K_PXLMAG_MCW;

    pstVpssVirReg->VPSS_DIEFUSION3.bits.mc_lai_bldmode = pstDeiPara->mcdeiparams.Mc_LAI_BLDMode;
    pstVpssVirReg->VPSS_DIEFUSION3.bits.g0_mcbld = pstDeiPara->mcdeiparams.G0_MCBLD;

    pstVpssVirReg->VPSS_DIEFUSION0.bits.k_y_mcw = pstDeiPara->mcdeiparams.nK_MCW_Y;
    pstVpssVirReg->VPSS_DIEFUSION0.bits.k_y_mcbld = pstDeiPara->mcdeiparams.nK_MCBLD_Y;
    pstVpssVirReg->VPSS_DIEFUSION0.bits.k_c_mcw = pstDeiPara->mcdeiparams.nK_MCW_C;
    pstVpssVirReg->VPSS_DIEFUSION0.bits.k_c_mcbld = pstDeiPara->mcdeiparams.nK_MCBLD_C;

    pstVpssVirReg->VPSS_DIEFUSION1.bits.x0_mcw_adj = pstDeiPara->mcdeiparams.X0_MCW_ADJ;
    pstVpssVirReg->VPSS_DIEFUSION1.bits.k0_mcw_adj = pstDeiPara->mcdeiparams.K0_MCW_ADJ;
    pstVpssVirReg->VPSS_DIEFUSION1.bits.g0_mcw_adj = pstDeiPara->mcdeiparams.G0_MCW_ADJ;

    pstVpssVirReg->VPSS_DIEFUSION2.bits.k1_mcw_adj = pstDeiPara->mcdeiparams.K1_MCW_ADJ;
    pstVpssVirReg->VPSS_DIEFUSION2.bits.x0_mcbld = pstDeiPara->mcdeiparams.X0_MCBLD;
    pstVpssVirReg->VPSS_DIEFUSION2.bits.k0_mcbld = pstDeiPara->mcdeiparams.K0_MCBLD;
    pstVpssVirReg->VPSS_DIEFUSION2.bits.k1_mcbld = pstDeiPara->mcdeiparams.K1_MCBLD;

    pstVpssVirReg->VPSS_MCDI_MC2.bits.k_y_vertw = pstDeiPara->mcdeiparams.K_Y_VERTW;
    pstVpssVirReg->VPSS_MCDI_MC2.bits.k_c_vertw = pstDeiPara->mcdeiparams.K_C_VERTW;
    //  pstVpssVirReg->VPSS_MCDI_MC2.bits.k_delta = pstDeiPara->mcdeiparams.K_DELTA;
    //  pstVpssVirReg->VPSS_MCDI_MC2.bits.k_hfcore_gmd_mc = pstDeiPara->mcdeiparams.K_HFCORE_GMD_MC;

    //  pstVpssVirReg->VPSS_MCDI_MC3.bits.k_avg_delta = pstDeiPara->mcdeiparams.K_AVG_DELTA;
    //  pstVpssVirReg->VPSS_MCDI_MC3.bits.x0_mv_delta = pstDeiPara->mcdeiparams.X0_MV_DELTA;
    //  pstVpssVirReg->VPSS_MCDI_MC3.bits.k1_mv_delta = pstDeiPara->mcdeiparams.K1_MV_DELTA;
    pstVpssVirReg->VPSS_MCDI_MC3.bits.bdh_mcpos = pstDeiPara->mcdeiparams.BDH_MCPOS;
    pstVpssVirReg->VPSS_MCDI_MC3.bits.bdv_mcpos = pstDeiPara->mcdeiparams.BDV_MCPOS;

    //  pstVpssVirReg->VPSS_MCDI_MC1.bits.x0_mv_gmd_mc = pstDeiPara->mcdeiparams.X0_MV_GMD_MC;
    //  pstVpssVirReg->VPSS_MCDI_MC1.bits.k1_mv_gmd_mc = pstDeiPara->mcdeiparams.K1_MV_GMD_MC;
    //  pstVpssVirReg->VPSS_MCDI_MC1.bits.dec_hfcore_gmd_mc = pstDeiPara->mcdeiparams.DEC_HFCORE_GMD_MC;
    //  pstVpssVirReg->VPSS_MCDI_MC1.bits.inc_hfcore_gmd_mc = pstDeiPara->mcdeiparams.INC_HFCORE_GMD_MC;

    pstVpssVirReg->VPSS_DIEMCGBMCOEF0.bits.mtfilten_gmd = pstDeiPara->mcdeiparams.MTFILTEN_GMD;
    pstVpssVirReg->VPSS_DIEMCGBMCOEF0.bits.mtth0_gmd = pstDeiPara->mcdeiparams.MTTH0_GMD;
    pstVpssVirReg->VPSS_DIEMCGBMCOEF0.bits.mtth1_gmd = pstDeiPara->mcdeiparams.MTTH1_GMD;
    pstVpssVirReg->VPSS_DIEMCGBMCOEF0.bits.mtth2_gmd = pstDeiPara->mcdeiparams.MTTH2_GMD;
    pstVpssVirReg->VPSS_DIEMCGBMCOEF0.bits.mtth3_gmd = pstDeiPara->mcdeiparams.MTTH3_GMD;

    pstVpssVirReg->VPSS_DIEMCGBMCOEF1.bits.k_maxmag_gmd = pstDeiPara->mcdeiparams.K_MAXMAG_GMD;
    pstVpssVirReg->VPSS_DIEMCGBMCOEF1.bits.k_difh_gmd = pstDeiPara->mcdeiparams.K_DIFH_GMD;
    pstVpssVirReg->VPSS_DIEMCGBMCOEF1.bits.k_mag_gmd = pstDeiPara->mcdeiparams.K_MAG_GMD;

    pstVpssVirReg->VPSS_MCDI_MC0.bits.mclpf_mode = pstDeiPara->mcdeiparams.MCLPF_MODE;

    //add by z214841 2015.7.8
    pstVpssVirReg->VPSS_MCDI_RGLS.bits.th_sad_rgls    = pstDeiPara->mcdeiparams.TH_SAD_RGLS;
    pstVpssVirReg->VPSS_MCDI_RGLS.bits.th_mag_rgls    = pstDeiPara->mcdeiparams.TH_MAG_RGLS;
    pstVpssVirReg->VPSS_MCDI_RGLS.bits.k_sad_rgls     = pstDeiPara->mcdeiparams.K_SAD_RGLS;
    pstVpssVirReg->VPSS_MCDI_RGLS.bits.k_sadcore_rgmv = pstDeiPara->mcdeiparams.K_SADCORE_RGMV;

    pstVpssVirReg->VPSS_MCDI_DLT0.bits.th_hblkdist_mvdlt = pstDeiPara->mcdeiparams.TH_HBLKDIST_MVDLT;
    pstVpssVirReg->VPSS_MCDI_DLT0.bits.th_vblkdist_mvdlt = pstDeiPara->mcdeiparams.TH_VBLKDIST_MVDLT;
    pstVpssVirReg->VPSS_MCDI_DLT0.bits.th_ls_mvdlt       = pstDeiPara->mcdeiparams.TH_LS_MVDLT;
    pstVpssVirReg->VPSS_MCDI_DLT0.bits.th_rgmvx_mvdlt    = pstDeiPara->mcdeiparams.TH_RGMVX_MVDLT;
    pstVpssVirReg->VPSS_MCDI_DLT0.bits.th_blkmvx_mvdlt   = pstDeiPara->mcdeiparams.TH_BLKMVX_MVDLT;

    pstVpssVirReg->VPSS_MCDI_DLT1.bits.thh_sad_mvdlt   = pstDeiPara->mcdeiparams.THH_SAD_MVDLT;
    pstVpssVirReg->VPSS_MCDI_DLT1.bits.thl_sad_mvdlt   = pstDeiPara->mcdeiparams.THL_SAD_MVDLT;
    pstVpssVirReg->VPSS_MCDI_DLT1.bits.g_mag_mvdlt     = pstDeiPara->mcdeiparams.G_MAG_MVDLT;
    pstVpssVirReg->VPSS_MCDI_DLT1.bits.th_mag_mvdlt    = pstDeiPara->mcdeiparams.TH_MAG_MVDLT;
    pstVpssVirReg->VPSS_MCDI_DLT1.bits.k_sadcore_mvdlt = pstDeiPara->mcdeiparams.K_SADCORE_MVDLT;

    pstVpssVirReg->VPSS_MCDI_RGMCW0.bits.k_core_simimv  = pstDeiPara->mcdeiparams.K_CORE_SIMIMV;
    pstVpssVirReg->VPSS_MCDI_RGMCW0.bits.gl_core_simimv = pstDeiPara->mcdeiparams.GL_CORE_SIMIMV;
    pstVpssVirReg->VPSS_MCDI_RGMCW0.bits.gh_core_simimv = pstDeiPara->mcdeiparams.GH_CORE_SIMIMV;
    pstVpssVirReg->VPSS_MCDI_RGMCW0.bits.k_simimvw = pstDeiPara->mcdeiparams.K_SIMIMVW;
    pstVpssVirReg->VPSS_MCDI_RGMCW0.bits.k_rglsw = pstDeiPara->mcdeiparams.K_RGLSW;

    pstVpssVirReg->VPSS_MCDI_RGMCW1.bits.core_mvy_mcw = pstDeiPara->mcdeiparams.CORE_MVY_MCW;
    pstVpssVirReg->VPSS_MCDI_RGMCW1.bits.k_mvy_mcw = pstDeiPara->mcdeiparams.K_MVY_MCW;
    pstVpssVirReg->VPSS_MCDI_RGMCW1.bits.core_rgsadadj_mcw = pstDeiPara->mcdeiparams.CORE_RGSADADJ_MCW;
    pstVpssVirReg->VPSS_MCDI_RGMCW1.bits.k_rgsadadj_mcw = pstDeiPara->mcdeiparams.K_RGSADADJ_MCW;
    pstVpssVirReg->VPSS_MCDI_RGMCW1.bits.k_core_vsaddif = pstDeiPara->mcdeiparams.K_CORE_VSADDIF;
    pstVpssVirReg->VPSS_MCDI_RGMCW1.bits.rgtb_en_mcw = pstDeiPara->mcdeiparams.RGTB_EN_MCW;

    pstVpssVirReg->VPSS_MCDI_RGMCW2.bits.gl_core_vsaddif = pstDeiPara->mcdeiparams.GL_CORE_VSADDIF;
    pstVpssVirReg->VPSS_MCDI_RGMCW2.bits.gh_core_vsad_dif = pstDeiPara->mcdeiparams.GH_CORE_VSADDIF;
    pstVpssVirReg->VPSS_MCDI_RGMCW2.bits.k_vsaddifw = pstDeiPara->mcdeiparams.K_VSADDIFW;
    pstVpssVirReg->VPSS_MCDI_RGMCW2.bits.mode_rgysad_mcw = pstDeiPara->mcdeiparams.MODE_RGYSAD_MCW;
    pstVpssVirReg->VPSS_MCDI_RGMCW2.bits.core_rgmag_mcw = pstDeiPara->mcdeiparams.CORE_RGMAG_MCW;

    pstVpssVirReg->VPSS_MCDI_RGMCW3.bits.x0_rgmag_mcw = pstDeiPara->mcdeiparams.X0_RGMAG_MCW;
    pstVpssVirReg->VPSS_MCDI_RGMCW3.bits.k0_rgmag_mcw = pstDeiPara->mcdeiparams.K0_RGMAG_MCW;
    pstVpssVirReg->VPSS_MCDI_RGMCW3.bits.g0_rgmag_mcw = pstDeiPara->mcdeiparams.G0_RGMAG_MCW;

    pstVpssVirReg->VPSS_MCDI_RGMCW4.bits.k1_rgmag_mcw = pstDeiPara->mcdeiparams.K1_RGMAG_MCW;
    pstVpssVirReg->VPSS_MCDI_RGMCW4.bits.core_rgsad_mcw = pstDeiPara->mcdeiparams.CORE_RGSAD_MCW;
    pstVpssVirReg->VPSS_MCDI_RGMCW4.bits.x0_rgsad_mcw = pstDeiPara->mcdeiparams.X0_RGSAD_MCW;

    pstVpssVirReg->VPSS_MCDI_RGMCW5.bits.k0_rgsad_mcw = pstDeiPara->mcdeiparams.K0_RGSAD_MCW;
    pstVpssVirReg->VPSS_MCDI_RGMCW5.bits.g0_rgsad_mcw = pstDeiPara->mcdeiparams.G0_RGSAD_MCW;
    pstVpssVirReg->VPSS_MCDI_RGMCW5.bits.k1_rgsad_mcw = pstDeiPara->mcdeiparams.K1_RGSAD_MCW;

    pstVpssVirReg->VPSS_MCDI_RGMCW6.bits.x0_smrg_mcw = pstDeiPara->mcdeiparams.X0_SMRG_MCW;
    pstVpssVirReg->VPSS_MCDI_RGMCW6.bits.k0_smrg_mcw = pstDeiPara->mcdeiparams.K0_SMRG_MCW;
    pstVpssVirReg->VPSS_MCDI_RGMCW6.bits.x_rgsad_mcw = pstDeiPara->mcdeiparams.X_RGSAD_MCW;
    pstVpssVirReg->VPSS_MCDI_RGMCW6.bits.k_rgsad_mcw = pstDeiPara->mcdeiparams.K_RGSAD_MCW;

    pstVpssVirReg->VPSS_MCDI_TPMCW0.bits.x0_tpmvdist_mcw = pstDeiPara->mcdeiparams.X0_TPMVDIST_MCW;
    pstVpssVirReg->VPSS_MCDI_TPMCW0.bits.k0_tpmvdist_mcw = pstDeiPara->mcdeiparams.K0_TPMVDIST_MCW;
    pstVpssVirReg->VPSS_MCDI_TPMCW0.bits.g0_tpmvdist_mcw = pstDeiPara->mcdeiparams.G0_TPMVDIST_MCW;
    pstVpssVirReg->VPSS_MCDI_TPMCW0.bits.k1_tpmvdist_mcw = pstDeiPara->mcdeiparams.K1_TPMVDIST_MCW;

    pstVpssVirReg->VPSS_MCDI_TPMCW1.bits.k_minmvdist_mcw = pstDeiPara->mcdeiparams.K_MINMVDIST_MCW;
    pstVpssVirReg->VPSS_MCDI_TPMCW1.bits.k_avgmvdist_mcw = pstDeiPara->mcdeiparams.K_AVGMVDIST_MCW;
    pstVpssVirReg->VPSS_MCDI_TPMCW1.bits.b_core_tpmvdist_mcw = pstDeiPara->mcdeiparams.B_CORE_TPMVDIST_MCW;
    pstVpssVirReg->VPSS_MCDI_TPMCW1.bits.k_core_tpmvdist_mcw = pstDeiPara->mcdeiparams.K_CORE_TPMVDIST_MCW;

    pstVpssVirReg->VPSS_MCDI_WNDMCW0.bits.k_difhcore_mcw = pstDeiPara->mcdeiparams.K_DIFHCORE_MCW;
    pstVpssVirReg->VPSS_MCDI_WNDMCW0.bits.k_difvcore_mcw = pstDeiPara->mcdeiparams.K_DIFVCORE_MCW;
    pstVpssVirReg->VPSS_MCDI_WNDMCW0.bits.k_max_core_mcw = pstDeiPara->mcdeiparams.K_MAX_CORE_MCW;
    pstVpssVirReg->VPSS_MCDI_WNDMCW0.bits.k_max_dif_mcw = pstDeiPara->mcdeiparams.K_MAX_DIF_MCW;
    pstVpssVirReg->VPSS_MCDI_WNDMCW0.bits.k1_max_mag_mcw = pstDeiPara->mcdeiparams.K1_MAX_MAG_MCW;
    pstVpssVirReg->VPSS_MCDI_WNDMCW0.bits.k0_max_mag_mcw = pstDeiPara->mcdeiparams.K0_MAX_MAG_MCW;
    pstVpssVirReg->VPSS_MCDI_WNDMCW0.bits.k_tbdif_mcw = pstDeiPara->mcdeiparams.K_TBDIF_MCW;

    pstVpssVirReg->VPSS_MCDI_WNDMCW1.bits.k_tbmag_mcw = pstDeiPara->mcdeiparams.K_TBMAG_MCW;
    pstVpssVirReg->VPSS_MCDI_WNDMCW1.bits.x0_mag_wnd_mcw = pstDeiPara->mcdeiparams.X0_MAG_WND_MCW;
    pstVpssVirReg->VPSS_MCDI_WNDMCW1.bits.k0_mag_wnd_mcw = pstDeiPara->mcdeiparams.K0_MAG_WND_MCW;
    pstVpssVirReg->VPSS_MCDI_WNDMCW1.bits.g0_mag_wnd_mcw = pstDeiPara->mcdeiparams.G0_MAG_WND_MCW;
    pstVpssVirReg->VPSS_MCDI_WNDMCW1.bits.k1_mag_wnd_mcw = pstDeiPara->mcdeiparams.K1_MAG_WND_MCW;

    pstVpssVirReg->VPSS_MCDI_WNDMCW2.bits.g1_mag_wnd_mcw = pstDeiPara->mcdeiparams.G1_MAG_WND_MCW;
    pstVpssVirReg->VPSS_MCDI_WNDMCW2.bits.x0_sad_wnd_mcw = pstDeiPara->mcdeiparams.X0_SAD_WND_MCW;
    pstVpssVirReg->VPSS_MCDI_WNDMCW2.bits.k0_sad_wnd_mcw = pstDeiPara->mcdeiparams.K0_SAD_WND_MCW;
    pstVpssVirReg->VPSS_MCDI_WNDMCW2.bits.g0_sad_wnd_mcw = pstDeiPara->mcdeiparams.G0_SAD_WND_MCW;

    pstVpssVirReg->VPSS_MCDI_WNDMCW3.bits.k1_sad_wnd_mcw = pstDeiPara->mcdeiparams.K1_SAD_WND_MCW;
    pstVpssVirReg->VPSS_MCDI_WNDMCW3.bits.g1_sad_wnd_mcw = pstDeiPara->mcdeiparams.G1_SAD_WND_MCW;

    pstVpssVirReg->VPSS_MCDI_MC1.bits.g_pxlmag_mcw = pstDeiPara->mcdeiparams.G_PXLMAG_MCW;

    pstVpssVirReg->VPSS_DIEFUSION0.bits.k_y_mcw = pstDeiPara->mcdeiparams.K_Y_MCW;
    pstVpssVirReg->VPSS_DIEFUSION0.bits.k_c_mcw = pstDeiPara->mcdeiparams.K_C_MCW;

    pstVpssVirReg->VPSS_MCDI_MC3.bits.k_delta = pstDeiPara->mcdeiparams.K_DELTA;
    pstVpssVirReg->VPSS_MCDI_MC3.bits.x0_mv_mc = pstDeiPara->mcdeiparams.X0_MV_MC;
    pstVpssVirReg->VPSS_MCDI_MC3.bits.k1_mv_mc = pstDeiPara->mcdeiparams.K1_MV_MC;

    pstVpssVirReg->VPSS_MCDI_MC5.bits.c0_mc = pstDeiPara->mcdeiparams.C0_MC;
    pstVpssVirReg->VPSS_MCDI_MC5.bits.r0_mc = pstDeiPara->mcdeiparams.R0_MC;

    pstVpssVirReg->VPSS_MCDI_MC6.bits.c1_mc = pstDeiPara->mcdeiparams.C1_MC;
    pstVpssVirReg->VPSS_MCDI_MC6.bits.r1_mc = pstDeiPara->mcdeiparams.R1_MC;
    pstVpssVirReg->VPSS_MCDI_MC6.bits.mcmvrange = pstDeiPara->mcdeiparams.MCMVRange;

    pstVpssVirReg->VPSS_MCDI_MC7.bits.scenechange_mc = pstDeiPara->mcdeiparams.SCENECHANGE_MC;
    pstVpssVirReg->VPSS_MCDI_MC7.bits.x_frcount_mc = pstDeiPara->mcdeiparams.X_FRCOUNT_MC;
    pstVpssVirReg->VPSS_MCDI_MC7.bits.k_frcount_mc = pstDeiPara->mcdeiparams.K_FRCOUNT_MC;

    pstVpssVirReg->VPSS_DIEFUSION3.bits.mc_lai_bldmode = pstDeiPara->mcdeiparams.MC_LAI_BLDMODE;

    pstVpssVirReg->VPSS_MCDI_NUMT.bits.mc_numt_blden = pstDeiPara->mcdeiparams.MC_NUMT_BLDEN;

    pstVpssVirReg->VPSS_MCDI_LCDINEW0.bits.mcstartr = pstDeiPara->mcdeiparams.McStartR;
    pstVpssVirReg->VPSS_MCDI_LCDINEW0.bits.mcstartc = pstDeiPara->mcdeiparams.McStartC;
    pstVpssVirReg->VPSS_MCDI_LCDINEW0.bits.mcendr = pstDeiPara->mcdeiparams.McEndR;
    pstVpssVirReg->VPSS_MCDI_LCDINEW0.bits.mcendc = pstDeiPara->mcdeiparams.McEndC;

    pstVpssVirReg->VPSS_DIEEDGENORM2.bits.mc_strength_k3 = pstDeiPara->mcdeibaseParams.stINTERPINFO.MC_STRENGTH_K3;
    pstVpssVirReg->VPSS_DIEEDGENORM3.bits.mc_strength_g3 = pstDeiPara->mcdeibaseParams.stINTERPINFO.MC_STRENGTH_G3;

    //  pstVpssVirReg->VPSS_MCDI_WNDMC4.bits.ass_vertlpf_on = pstDeiPara->mcdeiparams.Ass_VertLPFON;
    //  pstVpssVirReg->VPSS_MCDI_WNDMC4.bits.wnd_max_on = pstDeiPara->mcdeiparams.Wnd_MaxOn;

    //  pstVpssVirReg->VPSS_MCDI_BLKV.bits.blkmvstartr = pstDeiPara->mcdeiparams.McStartR;
    //  pstVpssVirReg->VPSS_MCDI_BLKV.bits.blkmvendr = pstDeiPara->mcdeiparams.McEndR;

    //  pstVpssVirReg->VPSS_MCDI_BLKH.bits.blkmvstartc = pstDeiPara->mcdeiparams.McStartC;
    //  pstVpssVirReg->VPSS_MCDI_BLKH.bits.blkmvendc = pstDeiPara->mcdeiparams.McEndC;

    /* 新增MA参数 */
    pstVpssVirReg->VPSS_DIEFLDMTN.bits.long_motion_shf     = pstDeiPara->mcdeiparams.blong_motion_shf;
    pstVpssVirReg->VPSS_DIEFLDMTN.bits.fld_motion_wnd_mode = pstDeiPara->mcdeiparams.bfld_motion_wnd_mode;

    /* 其他MC 参数 */
    /* 1. GmdForMC_S, 全部为重新定义寄存器, 不再重复设置
       2. InterpInfo_S
       */
    pstVpssVirReg->VPSS_DIEEDGENORM0.bits.edge_norm_0 = pstDeiPara->mcdeibaseParams.stINTERPINFO.EDGE_NORM[0];
    pstVpssVirReg->VPSS_DIEEDGENORM0.bits.edge_norm_1 = pstDeiPara->mcdeibaseParams.stINTERPINFO.EDGE_NORM[1];

    pstVpssVirReg->VPSS_DIEEDGENORM1.bits.edge_norm_2 = pstDeiPara->mcdeibaseParams.stINTERPINFO.EDGE_NORM[2];
    pstVpssVirReg->VPSS_DIEEDGENORM1.bits.edge_norm_3 = pstDeiPara->mcdeibaseParams.stINTERPINFO.EDGE_NORM[3];

    pstVpssVirReg->VPSS_DIEEDGENORM2.bits.edge_norm_4 = pstDeiPara->mcdeibaseParams.stINTERPINFO.EDGE_NORM[4];
    pstVpssVirReg->VPSS_DIEEDGENORM2.bits.edge_norm_5 = pstDeiPara->mcdeibaseParams.stINTERPINFO.EDGE_NORM[5];
    //pstVpssVirReg->VPSS_DIEEDGENORM2.bits.mc_stregth_k3 = pstDeiPara->mcdeibaseParams.stINTERPINFO.MC_STRENGTH_K3;

    pstVpssVirReg->VPSS_DIEEDGENORM3.bits.edge_norm_6 = pstDeiPara->mcdeibaseParams.stINTERPINFO.EDGE_NORM[6];
    pstVpssVirReg->VPSS_DIEEDGENORM3.bits.edge_norm_7 = pstDeiPara->mcdeibaseParams.stINTERPINFO.EDGE_NORM[7];
    //pstVpssVirReg->VPSS_DIEEDGENORM3.bits.mc_stregth_g3 = pstDeiPara->mcdeibaseParams.stINTERPINFO.MC_STRENGTH_G3;

    pstVpssVirReg->VPSS_DIEEDGENORM4.bits.edge_norm_8 = pstDeiPara->mcdeibaseParams.stINTERPINFO.EDGE_NORM[8];
    pstVpssVirReg->VPSS_DIEEDGENORM4.bits.edge_norm_9 = pstDeiPara->mcdeibaseParams.stINTERPINFO.EDGE_NORM[9];
    pstVpssVirReg->VPSS_DIEEDGENORM4.bits.inter_diff_thd0 = pstDeiPara->mcdeibaseParams.stINTERPINFO.INTER_DIFF_THD0;

    pstVpssVirReg->VPSS_DIEEDGENORM5.bits.edge_norm_10 = pstDeiPara->mcdeibaseParams.stINTERPINFO.EDGE_NORM[10];
    pstVpssVirReg->VPSS_DIEEDGENORM5.bits.edge_norm_11 = pstDeiPara->mcdeibaseParams.stINTERPINFO.EDGE_NORM[11];
    pstVpssVirReg->VPSS_DIEEDGENORM5.bits.inter_diff_thd1 = pstDeiPara->mcdeibaseParams.stINTERPINFO.INTER_DIFF_THD1;

    pstVpssVirReg->VPSS_DIEEDGEFORMC.bits.inter_diff_thd2 = pstDeiPara->mcdeibaseParams.stINTERPINFO.INTER_DIFF_THD2;
    pstVpssVirReg->VPSS_DIEEDGEFORMC.bits.edge_coring     = pstDeiPara->mcdeibaseParams.stINTERPINFO.MC_EDGE_CORING;
    pstVpssVirReg->VPSS_DIEEDGEFORMC.bits.edge_scale      = pstDeiPara->mcdeibaseParams.stINTERPINFO.MC_EDGE_SCALE;

    pstVpssVirReg->VPSS_DIEMCSTRENGTH0.bits.mc_strength_k0 = pstDeiPara->mcdeibaseParams.stINTERPINFO.MC_STRENGTH_K0;
    pstVpssVirReg->VPSS_DIEMCSTRENGTH0.bits.mc_strength_k1 = pstDeiPara->mcdeibaseParams.stINTERPINFO.MC_STRENGTH_K1;
    pstVpssVirReg->VPSS_DIEMCSTRENGTH0.bits.mc_strength_k2 = pstDeiPara->mcdeibaseParams.stINTERPINFO.MC_STRENGTH_K2;
    pstVpssVirReg->VPSS_DIEMCSTRENGTH0.bits.mc_strength_g0 = pstDeiPara->mcdeibaseParams.stINTERPINFO.MC_STRENGTH_G0;

    pstVpssVirReg->VPSS_DIEMCSTRENGTH1.bits.mc_strength_g1 = pstDeiPara->mcdeibaseParams.stINTERPINFO.MC_STRENGTH_G1;
    pstVpssVirReg->VPSS_DIEMCSTRENGTH1.bits.mc_strength_g2 = pstDeiPara->mcdeibaseParams.stINTERPINFO.MC_STRENGTH_G2;
    pstVpssVirReg->VPSS_DIEMCSTRENGTH1.bits.mc_strength_maxg = pstDeiPara->mcdeibaseParams.stINTERPINFO.MC_STRENGTH_MAXG;
    pstVpssVirReg->VPSS_DIEMCSTRENGTH1.bits.mc_strength_ming = pstDeiPara->mcdeibaseParams.stINTERPINFO.MC_STRENGTH_MING;

    /* 参数尝试 , 解决拉丝问题 */
    //pstVpssVirReg->VPSS_MCDI_MC4.bits.mc_end_col   = 0; //255;
    //pstVpssVirReg->VPSS_MCDI_MC4.bits.mc_start_col = 0; //255;
    //pstVpssVirReg->VPSS_MCDI_MC4.bits.mc_end_row   = 0; //255;
    //pstVpssVirReg->VPSS_MCDI_MC4.bits.mc_start_row = 0; //255;
#endif

#if 0
    pstVpssVirReg->VPSS_DIEGLBMTNTHD.bits.small_motion_thd = pstDeiPara->u32SmallmotionThd; /* 5. AV500已经删除 */
    pstVpssVirReg->VPSS_DIEGLBMTNTHD.bits.large_motion_thd = pstDeiPara->u32LargemotionThd;
    pstVpssVirReg->VPSS_CTRL.bits.iglb_en = pstDeiPara->bIgblEn;
#endif
#if 1
    pstVpssVirReg->VPSS_DIEMAGBMTHD0.bits.ma_gbm_thd0 = pstDeiPara->ma_gbm_thd0;
    pstVpssVirReg->VPSS_DIEMAGBMTHD0.bits.ma_gbm_thd1 = pstDeiPara->ma_gbm_thd1;
    pstVpssVirReg->VPSS_DIEMAGBMTHD1.bits.ma_gbm_thd2 = pstDeiPara->ma_gbm_thd2;
    pstVpssVirReg->VPSS_DIEMAGBMTHD1.bits.ma_gbm_thd3 = pstDeiPara->ma_gbm_thd3;

#endif

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_GetMotionHistReg(MOTION_INPUT_S *pstMotionInput, MotionHist_S *pstMotionHist)
{
    PQ_CHECK_NULL_PTR(pstMotionInput);

    pstMotionHist->MOTION_HIST[0]  = pstMotionInput->pstMotionReg->iglb_mtn_bin0.u32;
    pstMotionHist->MOTION_HIST[1]  = pstMotionInput->pstMotionReg->iglb_mtn_bin1.u32;
    pstMotionHist->MOTION_HIST[2]  = pstMotionInput->pstMotionReg->iglb_mtn_bin2.u32;
    pstMotionHist->MOTION_HIST[3]  = pstMotionInput->pstMotionReg->iglb_mtn_bin3.u32;
    pstMotionHist->MOTION_HIST[4]  = pstMotionInput->pstMotionReg->iglb_mtn_bin4.u32;
    pstMotionHist->MOTION_HIST[5]  = pstMotionInput->pstMotionReg->iglb_mtn_bin5.u32;
    pstMotionHist->MOTION_HIST[6]  = pstMotionInput->pstMotionReg->iglb_mtn_bin6.u32;
    pstMotionHist->MOTION_HIST[7]  = pstMotionInput->pstMotionReg->iglb_mtn_bin7.u32;
    pstMotionHist->MOTION_HIST[8]  = pstMotionInput->pstMotionReg->iglb_mtn_bin8.u32;
    pstMotionHist->MOTION_HIST[9]  = pstMotionInput->pstMotionReg->iglb_mtn_bin9.u32;
    pstMotionHist->MOTION_HIST[10] = pstMotionInput->pstMotionReg->iglb_mtn_bin10.u32;
    pstMotionHist->MOTION_HIST[11] = pstMotionInput->pstMotionReg->iglb_mtn_bin11.u32;
    pstMotionHist->MOTION_HIST[12] = pstMotionInput->pstMotionReg->iglb_mtn_bin12.u32;
    pstMotionHist->MOTION_HIST[13] = pstMotionInput->pstMotionReg->iglb_mtn_bin13.u32;
    pstMotionHist->MOTION_HIST[14] = pstMotionInput->pstMotionReg->iglb_mtn_bin14.u32;
    pstMotionHist->MOTION_HIST[15] = pstMotionInput->pstMotionReg->iglb_mtn_bin15.u32;
    pstMotionHist->MOTION_HIST[16] = pstMotionInput->pstMotionReg->iglb_mtn_bin16.u32;
    pstMotionHist->MOTION_HIST[17] = pstMotionInput->pstMotionReg->iglb_mtn_bin17.u32;
    pstMotionHist->MOTION_HIST[18] = pstMotionInput->pstMotionReg->iglb_mtn_bin18.u32;
    pstMotionHist->MOTION_HIST[19] = pstMotionInput->pstMotionReg->iglb_mtn_bin19.u32;
    pstMotionHist->MOTION_HIST[20] = pstMotionInput->pstMotionReg->iglb_mtn_bin20.u32;
    pstMotionHist->MOTION_HIST[21] = pstMotionInput->pstMotionReg->iglb_mtn_bin21.u32;
    pstMotionHist->MOTION_HIST[22] = pstMotionInput->pstMotionReg->iglb_mtn_bin22.u32;
    pstMotionHist->MOTION_HIST[23] = pstMotionInput->pstMotionReg->iglb_mtn_bin23.u32;
    pstMotionHist->MOTION_HIST[24] = pstMotionInput->pstMotionReg->iglb_mtn_bin24.u32;
    pstMotionHist->MOTION_HIST[25] = pstMotionInput->pstMotionReg->iglb_mtn_bin25.u32;
    pstMotionHist->MOTION_HIST[26] = pstMotionInput->pstMotionReg->iglb_mtn_bin26.u32;
    pstMotionHist->MOTION_HIST[27] = pstMotionInput->pstMotionReg->iglb_mtn_bin27.u32;
    pstMotionHist->MOTION_HIST[28] = pstMotionInput->pstMotionReg->iglb_mtn_bin28.u32;
    pstMotionHist->MOTION_HIST[29] = pstMotionInput->pstMotionReg->iglb_mtn_bin29.u32;
    pstMotionHist->MOTION_HIST[30] = pstMotionInput->pstMotionReg->iglb_mtn_bin30.u32;
    pstMotionHist->MOTION_HIST[31] = pstMotionInput->pstMotionReg->iglb_mtn_bin31.u32;

    pstMotionHist->SMALL_MOTION_NUM = pstMotionInput->pstMotionReg->iglb_mtn_num0.u32;
    pstMotionHist->SMALL_MOTION_SUM = pstMotionInput->pstMotionReg->iglb_mtn_sum0.u32;
    pstMotionHist->MIDDL_MOTION_NUM = pstMotionInput->pstMotionReg->iglb_mtn_num1.u32
                                      + pstMotionInput->pstMotionReg->iglb_mtn_num2.u32
                                      + pstMotionInput->pstMotionReg->iglb_mtn_num3.u32;
    pstMotionHist->MIDDL_MOTION_SUM = pstMotionInput->pstMotionReg->iglb_mtn_sum1.u32
                                      + pstMotionInput->pstMotionReg->iglb_mtn_sum2.u32
                                      + pstMotionInput->pstMotionReg->iglb_mtn_sum3.u32;
    pstMotionHist->LARGE_MOTION_NUM = pstMotionInput->pstMotionReg->iglb_mtn_num4.u32;
    pstMotionHist->LARGE_MOTION_SUM = pstMotionInput->pstMotionReg->iglb_mtn_sum4.u32;

    return HI_SUCCESS;
}
#endif

/* 根据globalmotion 计算出的运动信息调整gain值 */
HI_S32 PQ_HAL_SetDeiAdjustGain(HI_U32 u32HandleNo, HI_U32 u32AdjustGain)
{
    VPSS_REG_S *pstVpssVirReg = NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_DIEMTNADJ.bits.motion_adjust_gain = u32AdjustGain;
    return HI_SUCCESS;
}

/* 获取隔行运动信息interlace globa motion开关状态 */
HI_BOOL PQ_HAL_GetInterlaceMotionEn(HI_U32 u32HandleNo)
{
    HI_BOOL bIGlbEn;
    S_CAS_REGS_TYPE *pstVpssVirReg = NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    bIGlbEn = pstVpssVirReg->VPSS_CTRL.bits.igbm_en;

    return bIGlbEn;
}

HI_S32 PQ_HAL_SetInterlaceMotionEn(HI_U32 u32HandleNo, HI_BOOL bOnOff)
{
    S_CAS_REGS_TYPE *pstVpssVirReg = NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    /* Logic limit: When dei close, can not open igbm_en; So PQ do not control */
    pstVpssVirReg->VPSS_CTRL.bits.igbm_en = bOnOff;

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetSoftReg(HI_U32 u32HandleNo, HI_Mc_DeiParam_S *pstMcParm, HI_DEI_Variable_S *pstDeiVar)
{
    VPSS_REG_S *pstVpssVirReg = NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);
#if 0
    //pstVpssVirReg->VPSS_MCDI_MC2.bits.k_hfcore_gmd_mc = pstMcParm->K_HFCORE_GMD_MC;
    pstVpssVirReg->VPSS_DIECTRL.bits.mc_only = pstMcParm->MC_ONLY;
    pstVpssVirReg->VPSS_DIECTRL.bits.ma_only = pstMcParm->MA_ONLY;
#endif
    //pstVpssVirReg->VPSS_DIECTRL.bits.mc_only = 1;
    //pstVpssVirReg->VPSS_DIECTRL.bits.ma_only = 1;

    pstVpssVirReg->VPSS_DIEFUSION0.bits.k_y_mcw   = pstMcParm->nK_MCW_Y;
    pstVpssVirReg->VPSS_DIEFUSION0.bits.k_y_mcbld = pstMcParm->nK_MCW_C;
    pstVpssVirReg->VPSS_DIEFUSION0.bits.k_c_mcw   = pstMcParm->nK_MCBLD_Y;
    pstVpssVirReg->VPSS_DIEFUSION0.bits.k_c_mcbld = pstMcParm->nK_MCBLD_C;
    pstVpssVirReg->VPSS_MCDI_MC2.bits.k_c_vertw   = pstMcParm->K_C_VERTW;

    pstVpssVirReg->VPSS_DIEMTNADJ.bits.motion_adjust_gain = pstDeiVar->stMDINFO.ADJUST_GAIN;
    pstVpssVirReg->VPSS_DIEEDGEFORMC.bits.edge_coring     = pstDeiVar->stINTERPINFO.MC_EDGE_CORING;
    pstVpssVirReg->VPSS_DIEEDGEFORMC.bits.edge_scale      = pstDeiVar->stINTERPINFO.MC_EDGE_SCALE;

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_UpdateBlkRect(HI_U32 u32HandleNo, HI_U32 u32Width, HI_U32 u32Height)
{
    VPSS_REG_S *pstVpssVirReg = NULL;

    /*HI_U32 blkmvendc, blkmvstartc;

    blkmvstartc = 0;

    if (u32Width > 960)
    {
        blkmvendc = u32Width / 2 - 1;
    }
    else
    {
        blkmvendc = u32Width - 1;
    }

      pstVpssVirReg->VPSS_MCDI_BLKH.bits.blkmvendc   = blkmvendc;
      pstVpssVirReg->VPSS_MCDI_BLKH.bits.blkmvstartc = blkmvstartc;
    */

    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetMotionCompensateEn(HI_U32 u32HandleNo, HI_BOOL bMcEnable)
{
    VPSS_REG_S *pstVpssVirReg = NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_DIECTRL.bits.ma_only = 1 - bMcEnable;

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_EnableDeiDemo(HI_U32 u32HandleNo, HI_BOOL bOnOff)
{
    S_CAS_REGS_TYPE *pstVpssVirReg = HI_NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_MCDI_DEMO.bits.demo_en = bOnOff;

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_GetMaDeiEn(HI_U32 u32HandleNo, HI_BOOL *pbOnOff)
{
    S_CAS_REGS_TYPE *pstVpssVirReg = NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    *pbOnOff = pstVpssVirReg->VPSS_CTRL.bits.dei_en;

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_GetMcDeiEn(HI_U32 u32HandleNo, HI_BOOL *pbOnOff)
{
    S_CAS_REGS_TYPE *pstVpssVirReg = NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    *pbOnOff = pstVpssVirReg->VPSS_CTRL.bits.mcdi_en;

    return HI_SUCCESS;
}


HI_S32 PQ_HAL_SetDeiDemoMode(HI_U32 u32HandleNo, DEI_DEMO_MODE_E enMode)
{
    S_CAS_REGS_TYPE *pstVpssVirReg = HI_NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    if (DEI_DEMO_ENABLE_L == enMode) /* Enable_L: L-McEnable; R-McDisable */
    {
        pstVpssVirReg->VPSS_MCDI_DEMO.bits.demo_mode_l = MC_DIOUTPUT;
        pstVpssVirReg->VPSS_MCDI_DEMO.bits.demo_mode_r = MC_DIDISABLE;
    }
    else /* Enable_R: R-McEnable; L-McDisable */
    {
        pstVpssVirReg->VPSS_MCDI_DEMO.bits.demo_mode_l = MC_DIDISABLE;
        pstVpssVirReg->VPSS_MCDI_DEMO.bits.demo_mode_r = MC_DIOUTPUT;
    }

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetDeiDemoModeCoor(HI_U32 u32HandleNo, HI_U32 u32XPos)
{
    S_CAS_REGS_TYPE *pstVpssVirReg = HI_NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_MCDI_DEMO.bits.demo_border = u32XPos;

    return HI_SUCCESS;
}




