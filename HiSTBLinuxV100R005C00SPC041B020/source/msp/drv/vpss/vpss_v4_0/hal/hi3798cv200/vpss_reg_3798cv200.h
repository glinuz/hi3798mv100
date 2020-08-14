//****************************************************************************** 
// Copyright     :  Copyright (C) 2015, Hisilicon Technologies Co., Ltd.
// File name     :  c_union_define.h
// Author        :  x57490
// Version       :  1.0 
// Date          :  2015-04-30
// Description   :  Define all registers/tables for S40V200
// Others        :  Generated automatically by nManager V4.0 
// History       :  x57490 2015-04-30 Create file
//******************************************************************************
#ifndef __VPSS_REG_3798CV200_H__
#define __VPSS_REG_3798CV200_H__

#include "vpss_common.h"
#include "vpss_reg_struct.h"
#include "hi_drv_reg.h"
#include "hi_reg_common.h"

#define VPSS_BASE_ADDR  0xf8cb0000
#define VPSS0_BASE_ADDR  0xf8cb0000    //s40v200




HI_S32 VPSS_REG_RegWrite(volatile HI_U32 *a, HI_U32 b);
HI_U32 VPSS_REG_RegRead(volatile HI_U32* a);
HI_S32 VPSS_REG_ReSetCRG(HI_U32 u32Type);
HI_S32 VPSS_REG_SetClockEn(HI_BOOL bClkEn);
HI_S32 VPSS_REG_BaseRegInit(VPSS_REG_S **ppstPhyReg);
HI_S32 VPSS_REG_AppRegInit(VPSS_REG_S **ppstAppReg, HI_U8 * pu8VirAddr);
//HI_S32 VPSS_REG_SmmuInit(HI_U32 u32AppAddr);

/*配置结点重置*/
HI_S32 VPSS_REG_ResetAppReg(HI_U32 * pu32AppAddr,VPSS_REG_S *pstPqReg);
HI_S32 VPSS_REG_SetIntMask(HI_U32 * pu32AppAddr,HI_U32 u32Mask);
HI_S32 VPSS_REG_GetIntMask(HI_U32 * pu32AppAddr,HI_U32 *pu32Mask);
HI_S32 VPSS_REG_GetIntState(HI_U32 * pu32AppAddr,HI_U32 *pu32Int);
HI_S32 VPSS_REG_GetRawIntState(HI_U32 * pu32AppAddr,HI_U32 *pu32RawInt);
HI_S32 VPSS_REG_ClearIntState(HI_U32 * pu32AppAddr,HI_U32 u32Data);

/********************************/
HI_S32 VPSS_REG_SetBurstLen(HI_U32 * pu32AppAddr,REG_BURST_LEN_E eLen);
HI_S32 VPSS_REG_SetCkGtEn(HI_U32 * pu32AppAddr,HI_BOOL bEnable, HI_BOOL bCalcEnable);
HI_S32 VPSS_REG_SetTimeOut(HI_U32 * pu32AppAddr,HI_U32 u32Data);
HI_S32 VPSS_REG_SetSmmuIntEn(HI_U8 *pu32AppAddr,HI_BOOL bEnable);
HI_S32 VPSS_REG_ClearSmmuInt(HI_U8 *pu32AppAddr);
HI_S32 VPSS_REG_GetSmmuIntState(HI_U8 *pu32AppAddr,HI_U32 *pu32SecureState,HI_U32 *pu32NoSecureState);

#if 0
HI_S32 VPSS_REG_SetSmmuGlbBypass(HI_U32 * pu32AppAddr,HI_BOOL bEnable);
HI_S32 VPSS_REG_SetSmmuAutoClkGtEn(HI_U32 * pu32AppAddr,HI_BOOL bEnable);
#endif
HI_S32 VPSS_REG_StartLogic(HI_U32 u32AppAddr,HI_U32 *pu32PhyAddr);
HI_S32 VPSS_REG_CloseClock(HI_VOID);
HI_S32 VPSS_REG_OpenClock(HI_VOID);
HI_S32 VPSS_REG_GetClockState(HI_BOOL *pbOpened);
HI_S32 VPSS_REG_SetTNrEn(HI_U32 * pu32AppAddr,HI_BOOL bEnable);
HI_S32 VPSS_REG_SetTNrMadMode(HI_U32 * pu32AppAddr,HI_U32 u32Mode);
HI_S32 VPSS_REG_SetSNrEn(HI_U32 * pu32AppAddr,HI_BOOL bEnable);
HI_S32 VPSS_REG_SetIfmdEn(HI_U32 * pu32AppAddr,HI_BOOL bEnable);
HI_S32 VPSS_REG_SetIglbEn(HI_U32 * pu32AppAddr,HI_BOOL bEnable);
HI_S32 VPSS_REG_Set4PixEn(HI_U32 * pu32AppAddr,HI_BOOL bEnable);
HI_S32 VPSS_REG_SetProtEn(HI_U32 * pu32AppAddr,HI_U32 u32Prot);
HI_S32 VPSS_REG_SetRfrEn(HI_U32 * pu32AppAddr,HI_BOOL bEnable);
HI_VOID VPSS_REG_SetDetEn(HI_U32 * pu32AppAddr,HI_BOOL bEnable);
HI_VOID VPSS_REG_SetBlkDetEn(HI_U32 * pu32AppAddr,HI_BOOL bEnable);
HI_VOID VPSS_REG_SetPgbmEn(HI_U32 * pu32AppAddr,HI_BOOL bEnable);
HI_VOID VPSS_REG_SetVC1En(HI_U32 * pu32AppAddr,HI_BOOL bEnable);
HI_VOID VPSS_REG_SetMedsEn(HI_U32 * pu32AppAddr,HI_BOOL bEnable);
HI_VOID VPSS_REG_SetMcdiEn(HI_U32 * pu32AppAddr,HI_BOOL bEnable);
HI_S32 VPSS_REG_EnDei(HI_U32 * pu32AppAddr,HI_BOOL bEnDei);
HI_S32 VPSS_REG_SetChkSumEn(HI_U32 * pu32AppAddr,HI_U32 u32Chksum);
HI_S32 VPSS_REG_SetDcmpEn(HI_U32 * pu32AppAddr,HI_U32 u32En);
HI_S32 VPSS_REG_SetDcmpHeadAddr(HI_U32 * pu32AppAddr,HI_U32 u32YHeadaddr,HI_U32 u32CHeadaddr,HI_U32 u32Headsize);
HI_S32 VPSS_REG_SetDcmpHeadStride(HI_U32 * pu32AppAddr,HI_U32  u32HeadStride);
HI_S32 VPSS_REG_SetRfrPixBitw(HI_U32 * pu32AppAddr,HI_DRV_PIXEL_BITWIDTH_E ePixw);
HI_S32 VPSS_REG_SetRefNxtPixBitw(HI_U32 * pu32AppAddr,HI_DRV_PIXEL_BITWIDTH_E ePixw);
HI_S32 VPSS_REG_SetInPixBitw(HI_U32 * pu32AppAddr,HI_DRV_PIXEL_BITWIDTH_E ePixw);

/*********************************VC1****************************************************/
HI_S32 VPSS_REG_SetVc1RangeEn(HI_U32 * pu32AppAddr,REG_FIELDPOS_E ePos,HI_BOOL bEnable);
HI_S32 VPSS_REG_SetVc1Profile(HI_U32 * pu32AppAddr,REG_FIELDPOS_E ePos,HI_U32 u32Profile);
HI_S32 VPSS_REG_SetVc1Map(HI_U32 * pu32AppAddr,REG_FIELDPOS_E ePos,HI_U32 u32MapY,HI_U32 u32MapC);
HI_S32 VPSS_REG_SetVc1MapFlag(HI_U32 * pu32AppAddr,REG_FIELDPOS_E ePos,HI_U32 u32YFlag,HI_U32 u32CFlag);
HI_S32 VPSS_REG_SetVc1RangeDfrmEn(HI_U32 * pu32AppAddr,REG_FIELDPOS_E ePos,HI_U32 u32EnVc1);

/*********************************VC1****************************************************/




/*********************************check sum****************************************************/
HI_S32 VPSS_REG_SetRch0SumSel(HI_U32 * pu32AppAddr,HI_U32 u32rch0sumsel);
HI_S32 VPSS_REG_SetRch1SumSel(HI_U32 * pu32AppAddr,HI_U32 u32rch1sumsel);
HI_S32 VPSS_REG_SetRch2SumSel(HI_U32 * pu32AppAddr,HI_U32 u32rch2sumsel);
HI_S32 VPSS_REG_SetRch3SumSel(HI_U32 * pu32AppAddr,HI_U32 u32rch3sumsel);
HI_S32 VPSS_REG_SetRinfoSumSel(HI_U32 * pu32AppAddr,HI_U32 u32rinfosumsel);
HI_S32 VPSS_REG_SetWinfoSumSel(HI_U32 * pu32AppAddr,HI_U32 u32winfosumsel);

/*********************************check sum****************************************************/

HI_S32 VPSS_REG_SetMadRaddr(HI_U32 * pu32AppAddr,HI_U32 u32MadRaddr);
HI_S32 VPSS_REG_SetMadWaddr(HI_U32 * pu32AppAddr,HI_U32 u32MadWaddr);
HI_S32 VPSS_REG_SetMadStride(HI_U32 * pu32AppAddr,HI_U32 u32MadStride);
HI_S32 VPSS_REG_SetRfrDither(HI_U32 * pu32AppAddr,HI_U32 u32DitherMode, HI_BOOL bDitherEn);
HI_S32 VPSS_REG_SetRfrYaddr(HI_U32 * pu32AppAddr,HI_U32 u32RfrYaddr);
HI_S32 VPSS_REG_SetRfrCaddr(HI_U32 * pu32AppAddr,HI_U32 u32RfrCaddr);
HI_S32 VPSS_REG_SetRfrStride(HI_U32 * pu32AppAddr,HI_U32 u32RfrYStride,HI_U32 u32RfrCStride);
HI_S32 VPSS_REG_SetVhd0Dither(HI_U32 * pu32AppAddr,HI_U32 u32DitherMode,HI_BOOL bDitherEn);
HI_S32 VPSS_REG_SetVhd0Mirror(HI_U32 * pu32AppAddr,HI_U32 u32Vhd0Mirror);
HI_S32 VPSS_REG_SetVhd0Flip(HI_U32 * pu32AppAddr,HI_U32 u32Vhd0Flip);
HI_S32 VPSS_REG_SetVhd0UVInvert(HI_U32 * pu32AppAddr,HI_U32 u32Vhd0uvivt);
HI_S32 VPSS_REG_SetVhd0PixW(HI_U32 *pu32AppAddr,HI_DRV_PIXEL_BITWIDTH_E ePixW);
HI_S32 VPSS_REG_SetLcmpEn(HI_U32 * pu32AppAddr,HI_U32 LcmpEnable);
HI_S32 VPSS_REG_SetRefWidth(HI_U32 * pu32AppAddr,HI_U32 RefWidth);
HI_S32 VPSS_REG_SetRefHight(HI_U32 * pu32AppAddr,HI_U32 RefHight);
HI_S32 VPSS_REG_SetNodeID(HI_U32 * pu32AppAddr,HI_U32 u32NodeID);
//HI_S32 VPSS_REG_SetAxiRID(HI_U32 * pu32AppAddr,REG_AXI_RID_CHN_E eChn, HI_U32 u32AxiRID);
HI_S32 VPSS_REG_SetSmmuBypass(HI_U32 * pu32AppAddr,REG_SMMU_CHN_E eChn, HI_BOOL bBypass);
HI_S32 VPSS_REG_SetRchSmmuBypass(HI_U32 * pu32AppAddr,HI_U32 u32Mask);
HI_S32 VPSS_REG_SetWchSmmuBypass(HI_U32 * pu32AppAddr,HI_U32 u32Mask);

HI_S32 VPSS_REG_EnPort(HI_U32 * pu32AppAddr,VPSS_REG_PORT_E ePort,HI_BOOL bEnable);

/********************************/
HI_S32 VPSS_REG_SetImgSize(HI_U32 * pu32AppAddr,HI_U32 u32Height,HI_U32 u32Width,HI_BOOL bProgressive);
HI_S32 VPSS_REG_SetImgAddr(HI_U32 * pu32AppAddr,REG_FIELDPOS_E ePos,HI_U32 u32Yaddr,HI_U32 u32Caddr);
HI_S32 VPSS_REG_SetImgAddrLB(HI_U32 * pu32AppAddr,HI_U32 u32Yaddr,HI_U32 u32Cbaddr);
HI_S32 VPSS_REG_SetImgStride(HI_U32 * pu32AppAddr,REG_FIELDPOS_E ePos,HI_U32 u32YStride,HI_U32 u32CStride);
HI_S32 VPSS_REG_SetImgStrideLB(HI_U32 * pu32AppAddr,HI_U32 u32YStride);
HI_S32 VPSS_REG_SetImgTile(HI_U32 * pu32AppAddr,REG_FIELDPOS_E ePos,HI_U32 EnTile);
HI_S32 VPSS_REG_SetImgOffset(HI_U32 * pu32AppAddr,REG_FIELDPOS_E ePos,HI_U32 u32HorOffset,HI_U32 u32VerOffset);
HI_S32 VPSS_REG_SetImgFormat(HI_U32 * pu32AppAddr,HI_DRV_PIX_FORMAT_E eFormat);
HI_S32 VPSS_REG_SetImgReadMod(HI_U32 * pu32AppAddr,HI_BOOL bField);
HI_S32 VPSS_REG_SetReadPixBitWidth(HI_U32 * pu32AppAddr,REG_CHANELPOS_E eChn,HI_DRV_PIXEL_BITWIDTH_E enBitWidth);
HI_S32 VPSS_REG_SetWbcPixBitWidth(HI_U32 * pu32AppAddr,HI_DRV_PIXEL_BITWIDTH_E enBitWidth);
/********************************/

/********************************/
HI_S32 VPSS_REG_SetFrmSize(HI_U32 * pu32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32Height,HI_U32 u32Width);
HI_S32 VPSS_REG_SetChkSumAddr(HI_U32 * pu32AppAddr,HI_U32 u32CKSUMaddr);
HI_S32 VPSS_REG_SetSttWrAddr(HI_U32 * pu32AppAddr,HI_U32 u32SttWrAddr);
HI_S32 VPSS_REG_SetFrmAddr(HI_U32 * pu32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32Yaddr,HI_U32 u32Caddr);
HI_S32 VPSS_REG_SetFrmStride(HI_U32 * pu32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32YStride,HI_U32 u32CStride);
HI_S32 VPSS_REG_SetFrmFormat(HI_U32 * pu32AppAddr,VPSS_REG_PORT_E ePort,HI_DRV_PIX_FORMAT_E eFormat);

/********************************/

/********************************/
HI_S32 VPSS_REG_SetZmeGlbEnable(HI_U32 * pu32AppAddr,HI_BOOL bEnable);
HI_S32 VPSS_REG_SetZmeEnable(HI_U32 * pu32AppAddr,VPSS_REG_PORT_E ePort,REG_ZME_MODE_E eMode,HI_BOOL bEnable);
HI_S32 VPSS_REG_SetZmeInSize(HI_U32 * pu32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32Height,HI_U32 u32Width);
HI_S32 VPSS_REG_SetZmeOutSize(HI_U32 * pu32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32Height,HI_U32 u32Width);
HI_S32 VPSS_REG_SetZmeFirEnable(HI_U32 * pu32AppAddr,VPSS_REG_PORT_E ePort,
                                    REG_ZME_MODE_E eMode,HI_BOOL bEnable);
HI_S32 VPSS_REG_SetZmeMidEnable(HI_U32 * pu32AppAddr,VPSS_REG_PORT_E ePort,
                                    REG_ZME_MODE_E eMode,HI_BOOL bEnable);
HI_S32 VPSS_REG_SetZmePhase(HI_U32 * pu32AppAddr,VPSS_REG_PORT_E ePort,
                                    REG_ZME_MODE_E eMode,HI_S32 s32Phase);
HI_S32 VPSS_REG_SetZmeRatio(HI_U32 * pu32AppAddr,VPSS_REG_PORT_E ePort,
                                    REG_ZME_MODE_E eMode,HI_U32 u32Ratio);
HI_S32 VPSS_REG_SetZmeHfirOrder(HI_U32 * pu32AppAddr,VPSS_REG_PORT_E ePort,HI_BOOL bVfirst);
HI_S32 VPSS_REG_SetZmeInFmt(HI_U32 * pu32AppAddr,VPSS_REG_PORT_E ePort,HI_DRV_PIX_FORMAT_E eFormat);
HI_S32 VPSS_REG_SetZmeOutFmt(HI_U32 * pu32AppAddr,VPSS_REG_PORT_E ePort,HI_DRV_PIX_FORMAT_E eFormat);
HI_S32 VPSS_REG_SetZmeCoefAddr(HI_U32 * pu32AppAddr,VPSS_REG_PORT_E ePort,
                                    REG_ZME_MODE_E eMode,HI_U32 u32Addr);
                                    
/********************************/

/********************************/
HI_VOID VPSS_REG_SetDetMode(HI_U32 * pu32AppAddr,HI_U32 u32Mode);
HI_VOID VPSS_REG_SetDetBlk(HI_U32 * pu32AppAddr,HI_U32 blk_id, HI_U32 *pu32Addr);

/********************************/


/*************************************************************************************************/
/*DEI*/
HI_S32 VPSS_REG_RstDei(HI_U32 * pu32AppAddr,HI_BOOL bRst);//,HI_U32 u32RstValue)
HI_S32 VPSS_REG_SetDeiTopFirst(HI_U32 * pu32AppAddr,HI_BOOL bTopFirst);
HI_S32 VPSS_REG_SetDeiFieldMode(HI_U32 * pu32AppAddr,HI_BOOL bBottom);
HI_S32 VPSS_REG_SetDeiAddr(HI_U32 * pu32AppAddr,REG_FIELDPOS_E eField,HI_U32 u32YAddr,HI_U32 u32CAddr);
HI_S32 VPSS_REG_SetDeiStride(HI_U32 * pu32AppAddr,REG_FIELDPOS_E eField,HI_U32 u32YStride,HI_U32 u32CStride);
HI_S32 VPSS_REG_SetModeEn(HI_U32 * pu32AppAddr,REG_DIE_MODE_E eDieMode,HI_BOOL bEnMode);
HI_S32 VPSS_REG_SetOutSel(HI_U32 * pu32AppAddr,REG_DIE_MODE_E eDieMode,HI_BOOL bEnMode);
HI_S32 VPSS_REG_SetMode(HI_U32 * pu32AppAddr,REG_DIE_MODE_E eDieMode,HI_U32  u32Mode);
HI_S32 VPSS_REG_SetStInfo(HI_U32 * pu32AppAddr,HI_BOOL bStop);
HI_S32 VPSS_REG_SetMfMax(HI_U32 * pu32AppAddr,REG_DIE_MODE_E eDieMode,HI_BOOL bMax);
HI_S32 VPSS_REG_SetLuSceSdfMax(HI_U32 * pu32AppAddr,HI_BOOL bMax);
HI_S32 VPSS_REG_SetRecModeEn(HI_U32 * pu32AppAddr,HI_BOOL bEnMode);
HI_S32 VPSS_REG_SetMotionIIrEn(HI_U32 * pu32AppAddr,HI_BOOL bEnIIr);
HI_S32 VPSS_REG_SetMotionSmoothEn(HI_U32 * pu32AppAddr,HI_BOOL bEnSmooth);
HI_S32 VPSS_REG_SetMotionBlendEn(HI_U32 * pu32AppAddr,HI_BOOL bEnBlend);
HI_S32 VPSS_REG_SetChromeOffset(HI_U32 * pu32AppAddr,HI_U8 u8Offset);
HI_S32 VPSS_REG_SetMinIntern(HI_U32 * pu32AppAddr,HI_U32 u32MinIntern);
HI_S32 VPSS_REG_SetInternVer(HI_U32 * pu32AppAddr,HI_U32 u32InternVer);
HI_S32 VPSS_REG_SetRangeScale(HI_U32 * pu32AppAddr,HI_U32 u32Scale);
HI_S32 VPSS_REG_SetCK1(HI_U32 * pu32AppAddr,HI_U32 u32Gain,HI_U32 u32Range,HI_U32 u32Max);
HI_S32 VPSS_REG_SetCK2(HI_U32 * pu32AppAddr,HI_U32 u32Gain,HI_U32 u32Range,HI_U32 u32Max);
HI_S32 VPSS_REG_SetDIR(HI_U32 * pu32AppAddr,HI_S32 *ps32MultDir);
HI_S32 VPSS_REG_SetIntpSclRat(HI_U32 * pu32AppAddr,HI_S32 *ps32Rat);
HI_S32 VPSS_REG_SetStrenThd(HI_U32 * pu32AppAddr,HI_S32 s32Thd);
HI_S32 VPSS_REG_SetDirThd(HI_U32 * pu32AppAddr,HI_S32 s32Thd);
HI_S32 VPSS_REG_SetBcGain(HI_U32 * pu32AppAddr,HI_S32 s32BcGain);
HI_S32 VPSS_REG_EnHorEdge(HI_U32 * pu32AppAddr,HI_BOOL bEdgeEn);
HI_S32 VPSS_REG_EnEdgeMode(HI_U32 * pu32AppAddr,HI_BOOL bEdgeModeEn);
HI_S32 VPSS_REG_SetJitterCoring(HI_U32 * pu32AppAddr,HI_S32 s32Coring);
HI_S32 VPSS_REG_SetJitterGain(HI_U32 * pu32AppAddr,HI_S32 s32Gain);
HI_S32 VPSS_REG_SetMotionSlope(HI_U32 * pu32AppAddr,HI_S32 s32Slope);
HI_S32 VPSS_REG_SetMotionCoring(HI_U32 * pu32AppAddr,HI_S32 s32Coring);
HI_S32 VPSS_REG_SetMotionGain(HI_U32 * pu32AppAddr,HI_S32 s32Gain);
HI_S32 VPSS_REG_SetMotionHThd(HI_U32 * pu32AppAddr,HI_S32 s32HThd);
HI_S32 VPSS_REG_SetMotionLThd(HI_U32 * pu32AppAddr,HI_S32 s32LThd);
HI_S32 VPSS_REG_SetMotionDiffThd(HI_U32 * pu32AppAddr,HI_S32 *ps32Thd);
HI_S32 VPSS_REG_SetMotionIIrSlope(HI_U32 * pu32AppAddr,HI_S32 *ps32Slope);
HI_S32 VPSS_REG_SetMotionIIrRatio(HI_U32 * pu32AppAddr,HI_S32 *ps32Ratio);
HI_S32 VPSS_REG_SetMotionIIrRatioMaxMin(HI_U32 * pu32AppAddr,HI_S32 s32Max,HI_S32 s32Min);
//////////////////////
HI_S32 VPSS_REG_SetSmoothDiffThd(HI_U32 * pu32AppAddr,HI_S32 *ps32Thd);
HI_S32 VPSS_REG_SetSmoothSlope(HI_U32 * pu32AppAddr,HI_S32 *ps32Slope);
HI_S32 VPSS_REG_SetSmoothRatio(HI_U32 * pu32AppAddr,HI_S32 *ps32Ratio);
HI_S32 VPSS_REG_SetSmoothRatioMaxMin(HI_U32 * pu32AppAddr,HI_S32 s32Max,HI_S32 s32Min);
///////////////////////
HI_S32 VPSS_REG_SetBlendDiffThd(HI_U32 * pu32AppAddr,HI_S32 *ps32Thd);
HI_S32 VPSS_REG_SetBlendSlope(HI_U32 * pu32AppAddr,HI_S32 *ps32Slope);
HI_S32 VPSS_REG_SetBlendRatio(HI_U32 * pu32AppAddr,HI_S32 *ps32Ratio);
HI_S32 VPSS_REG_SetBlendRatioMaxMin(HI_U32 * pu32AppAddr,HI_S32 s32Max,HI_S32 s32Min);
///////////////////////
HI_S32 VPSS_REG_SetRecFldStep(HI_U32 * pu32AppAddr,HI_S32 *ps32Step);
HI_S32 VPSS_REG_SetRecFrmStep(HI_U32 * pu32AppAddr,HI_S32 *ps32Step);
HI_S32 VPSS_REG_SetHisEn(HI_U32 * pu32AppAddr,HI_BOOL bEnHis);
HI_S32 VPSS_REG_SetHisInfoWrMode(HI_U32 * pu32AppAddr,HI_BOOL bHisMtnInfoWrMd);
HI_S32 VPSS_REG_SetHisWrMode(HI_U32 * pu32AppAddr,HI_BOOL bHisMtnWrMd);
HI_S32 VPSS_REG_SetHisUseMode(HI_U32 * pu32AppAddr,HI_BOOL bHisMtnUseMd);
HI_S32 VPSS_REG_SetMorFlt(HI_U32 * pu32AppAddr,HI_BOOL bEnflt,HI_S8 s8FltSize,HI_S8 s8FltThd);
HI_S32 VPSS_REG_SetMotionAdj(HI_U32 * pu32AppAddr,HI_S32 s32Gain,HI_S32 s32Chom,HI_S32 s32Coring);
HI_S32 VPSS_REG_SetMotionThd(HI_U32 * pu32AppAddr,HI_S32 s32Small,HI_S32 s32Large);
HI_S32 VPSS_REG_SetBlendEn(HI_U32 * pu32AppAddr,HI_BOOL bEnBlend);
HI_S32 VPSS_REG_SetHisPreEn(HI_U32 * pu32AppAddr,HI_BOOL bEnPre);
HI_S32 VPSS_REG_SetHisPpreEn(HI_U32 * pu32AppAddr,HI_BOOL bEnPpre);
HI_S32 VPSS_REG_SetCombLimit(HI_U32 * pu32AppAddr,HI_S32  s32UpLimit,HI_S32  s32LowLimit);
HI_S32 VPSS_REG_SetCombThd(HI_U32 * pu32AppAddr,HI_S32  s32Hthd,HI_S32  s32Vthd);
HI_S32 VPSS_REG_SetCombEn(HI_U32 * pu32AppAddr,HI_BOOL bEnComb);
HI_S32 VPSS_REG_SetCombMdThd(HI_U32 * pu32AppAddr,HI_S32 s32MdThd);
HI_S32 VPSS_REG_SetCombEdgeThd(HI_U32 * pu32AppAddr,HI_S32 s32EdgeThd);
HI_S32 VPSS_REG_SetStWrAddr(HI_U32 * pu32AppAddr,HI_U32 u32Addr);
HI_S32 VPSS_REG_SetStRdAddr(HI_U32 * pu32AppAddr,HI_U32 u32Addr);
HI_S32 VPSS_REG_SetStStride(HI_U32 * pu32AppAddr,HI_U32 u32Stride);
HI_S32 VPSS_REG_SetDeiParaAddr(HI_U32 * pu32AppAddr,HI_U32 u32ParaPhyAddr);
HI_S32 VPSS_REG_SetDBMAddr(HI_U32 * pu32AppAddr,HI_U32 u32PhyAddr);
HI_S32 VPSS_REG_GetPdIchd(HI_U32 * pu32AppAddr,HI_S32 *pstData);
HI_S32 VPSS_REG_SetOutstanding(HI_U32 * pu32AppAddr,HI_U32 ROuts, HI_U32 WOuts);
HI_S32 VPSS_REG_SetAxiID(HI_U32 * pu32AppAddr,REG_AXI_CHN_E eChn, HI_U32 u32AxiID);

/**FMD */
HI_S32 VPSS_REG_SetDeiEdgeSmoothRatio(HI_U32 * pu32AppAddr,HI_S8 u8Data);
HI_S32 VPSS_REG_SetDeiStillBlkThd(HI_U32 * pu32AppAddr,HI_S8 u8Data);
HI_S32 VPSS_REG_SetDeiHistThd(HI_U32 * pu32AppAddr,HI_S8 *pu8Data);
HI_S32 VPSS_REG_SetDeiUmThd(HI_U32 * pu32AppAddr,HI_S8 *pu8Data);
HI_S32 VPSS_REG_SetDeiCoring(HI_U32 * pu32AppAddr,HI_S8 s8CorBlk,HI_S8 s8CorNorm,HI_S8 s8CorTkr);
HI_S32 VPSS_REG_SetDeiMovCoring(HI_U32 * pu32AppAddr,HI_S8 s8Blk,HI_S8 s8Norm,HI_S8 s8Tkr);
HI_S32 VPSS_REG_SetDeiPccHThd(HI_U32 * pu32AppAddr,HI_S8 s8Data);
HI_S32 VPSS_REG_SetDeiPccVThd(HI_U32 * pu32AppAddr,HI_S8 *ps8Data);
HI_S32 VPSS_REG_SetDeiItDiff(HI_U32 * pu32AppAddr,HI_S8 *ps8Data);
HI_S32 VPSS_REG_SetDeiLasiCtrl(HI_U32 * pu32AppAddr,HI_S8 s8Thr,HI_S8 s8EdgeThr,HI_S8 s8lasiThr);
HI_S32 VPSS_REG_SetDeiPdBitMove(HI_U32 * pu32AppAddr,HI_S32  s32Data);
HI_S32 VPSS_REG_SetDeiDirMch(HI_U32 * pu32AppAddr,HI_BOOL  bNext);
HI_S32 VPSS_REG_SetEdgeSmooth(HI_U32 * pu32AppAddr,HI_BOOL  bEdgeSmooth);
/*************************************************************************************************/
HI_S32 VPSS_REG_SetUVConvertEn(HI_U32 * pu32AppAddr,HI_U32 u32EnUV);
HI_S32 VPSS_REG_SetDbmEn(HI_U32 * pu32AppAddr,HI_BOOL  bEnDBM);
HI_S32 VPSS_REG_SetDbmOutMode(HI_U32 * pu32AppAddr,HI_U32  u32Mode);
HI_S32 VPSS_REG_SetDsEn(HI_U32 * pu32AppAddr,HI_BOOL  bEnDS);
HI_S32 VPSS_REG_SetDrEn(HI_U32 * pu32AppAddr,HI_BOOL  bEnDR);
HI_S32 VPSS_REG_SetDbmDemoEn(HI_U32 * pu32AppAddr,HI_BOOL  bEnDBM);
HI_S32 VPSS_REG_SetDbDetectEn(HI_U32 * pu32AppAddr,REG_DBDET_MODE_E eDbDetMode,HI_BOOL  bEnDbDetect);
HI_S32 VPSS_REG_SetDbEn(HI_U32 * pu32AppAddr,REG_DB_MODE_E eDbMode,HI_BOOL  bEnDb);
HI_S32 VPSS_REG_SetDmEn(HI_U32 * pu32AppAddr,HI_BOOL  bEnDm);
HI_S32 VPSS_REG_SetLBAEn(HI_U32 * pu32AppAddr,VPSS_REG_PORT_E ePort,HI_BOOL bEnLba);
HI_S32 VPSS_REG_SetLBABg(HI_U32 * pu32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32Color,HI_U32 u32Alpha);
HI_S32 VPSS_REG_SetLbaDispSize(HI_U32 * pu32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32Height,HI_U32 u32Width);
HI_S32 VPSS_REG_SetLbaVfPos(HI_U32 * pu32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32XFPos,HI_U32 u32YFPos);
HI_S32 VPSS_REG_SetOutCropVidPos(HI_U32 * pu32AppAddr,VPSS_REG_PORT_E ePort,
                            HI_U32 u32XFPos,HI_U32 u32YFPos,
                            HI_U32 u32Height,HI_U32 u32Width);
HI_S32 VPSS_REG_SetTunlEn(HI_U32 * pu32AppAddr,VPSS_REG_PORT_E ePort,HI_BOOL  bEnTunl);
HI_S32 VPSS_REG_SetTunlFinishLine(HI_U32 * pu32AppAddr,VPSS_REG_PORT_E ePort,HI_S32  s32FinishLine);
HI_S32 VPSS_REG_SetTunlMode(HI_U32 * pu32AppAddr,VPSS_REG_PORT_E ePort, REG_TUNLPOS_E  s32TunlMode);
HI_S32 VPSS_REG_SetTunlAddr(HI_U32 * pu32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32TunlAddr);
HI_S32 VPSS_REG_SetAddr(HI_U32 * pu32AppAddr,VPSS_REG_ADDR_E eType,HI_U32 u32Addr);
HI_S32 VPSS_REG_SetCurTunlAddr(HI_U32 * pu32AppAddr,HI_U32 u32TunlAddr);
HI_S32 VPSS_REG_SetCurTunlEn(HI_U32 * pu32AppAddr,HI_BOOL u32CurTunlEn);
HI_S32 VPSS_REG_SetCurTunlInterval(HI_U32 * pu32AppAddr,REG_FRAMEPOS_E ePort,HI_S32  s32CurTunlInterval);
HI_S32 VPSS_REG_SetInCropPos(HI_U32 * pu32AppAddr,HI_U32 u32InCropY,HI_U32 u32InCropX);
HI_S32 VPSS_REG_SetInCropEn(HI_U32 * pu32AppAddr,HI_BOOL bEnable);
HI_S32 VPSS_REG_SetInCropMode(HI_U32 * pu32AppAddr,HI_BOOL bInCropMode);
HI_S32 VPSS_REG_SetInCropSize(HI_U32 * pu32AppAddr,HI_U32 u32InCropHeight,HI_U32 u32InCropWidth);
/****PORT CROP*****/
HI_S32 VPSS_REG_SetPortCropPos(HI_U32 * pu32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32CropY,HI_U32 u32CropX);
HI_S32 VPSS_REG_SetPortCropSize(HI_U32 * pu32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32CropHeight,HI_U32 u32CropWidth);
HI_S32 VPSS_REG_SetPortCropEn(HI_U32 * pu32AppAddr,HI_BOOL bEnable);
HI_S32 VPSS_REG_SetPortMirrorEn(HI_U32 * pu32AppAddr,VPSS_REG_PORT_E ePort, HI_BOOL bMirrorEn);
HI_S32 VPSS_REG_SetPortFlipEn(HI_U32 * pu32AppAddr,VPSS_REG_PORT_E ePort, HI_BOOL bFlipEn);
HI_S32 VPSS_REG_SetPreHfirEn(HI_U32 * pu32AppAddr,HI_BOOL bHfirEn);
HI_S32 VPSS_REG_SetPreHfirMode(HI_U32 * pu32AppAddr, HI_U32 u32HfirMode);
HI_S32 VPSS_REG_SetPreVfirEn(HI_U32 * pu32AppAddr,HI_BOOL bVfirEn);
HI_S32 VPSS_REG_SetPreVfirMode(HI_U32 * pu32AppAddr,HI_U32 u32VfirMode);
HI_S32 VPSS_REG_SetRotation(HI_U32 * pu32AppAddr,HI_U32 u32Angle, HI_U32 u32ProMode);
HI_S32 VPSS_REG_GetPFCNT(HI_U32 * pu32AppAddr,HI_U32 *pu32Int);
HI_S32 VPSS_REG_SetFidelity(HI_U32 *pu32AppVAddr,HI_BOOL bEnFidelity);
HI_S32 VPSS_REG_SetStMode(HI_U32 *pu32AppVAddr,HI_BOOL bLumaMax,HI_BOOL bChromaMax);
HI_S32 VPSS_REG_SetPreZme(HI_U32 *pu32AppVAddr,
                            VPSS_REG_PREZME_E enHor,VPSS_REG_PREZME_E enVer);
HI_S32 VPSS_REG_SetTnrAddr(HI_U32 * pu32AppAddr,HI_U32 u32TNRAddr,HI_U32 u32CLUTAddr);
HI_S32 VPSS_REG_SetSnrAddr(HI_U32 * pu32AppAddr,HI_U32 u32SNRAddr);
HI_S32 VPSS_REG_SetMktCoordinate(HI_U32 * pu32AppAddr,HI_U32 u32MktCoordinate);
HI_S32 VPSS_REG_SetDbmDemoXPos(HI_U32 * pu32AppAddr,HI_U32 u32Xpos);
HI_S32 VPSS_REG_SetTnr(HI_U32 *pu32AppVAddr,HI_BOOL bDemoEn);
HI_S32 VPSS_REG_SetLBADispPos(HI_U32 * pu32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32XFPos,HI_U32 u32YFPos,
                            HI_U32 u32Height,HI_U32 u32Width);
HI_S32 VPSS_REG_SetLBAVidPos(HI_U32 * pu32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32XFPos,HI_U32 u32YFPos,
                            HI_U32 u32Height,HI_U32 u32Width);
HI_S32 VPSS_REG_SetPortUVInvert(HI_U32 * pu32AppAddr,VPSS_REG_PORT_E ePort, HI_BOOL bUVInvert);
//======================READ STT VALUE=================================
HI_S32 VPSS_REG_AppSttRegInit(S_STT_REGS_TYPE **ppstAppReg,HI_U32* pu32VirAddr);
HI_VOID VPSS_REG_GetDetPixel(HI_U32 * pu32AppAddr,HI_U32 BlkNum, HI_U8* pstData);
/********************************/
HI_S32 VPSS_REG_GetHisBin(HI_U32 * pu32AppAddr,HI_S32 *pstData);
HI_S32 VPSS_REG_GetItDiff(HI_U32 * pu32AppAddr,HI_S32 *pstData);
//TODO!!!  这里只有区域0 的，没有区域1 的
HI_S32 VPSS_REG_GetPccData(HI_U32 * pu32AppAddr,HI_S32 *ps32FFWD,
                             HI_S32 *ps32FWD,HI_S32 *ps32BWD,HI_S32 *ps32CRSS,
                             /*HI_S32 *ps32PW,*/HI_S32 *ps32FWDTKR,HI_S32 *ps32WDTKR);
HI_S32 VPSS_REG_GetPdMatch(HI_U32 * pu32AppAddr,
                            HI_S32 *ps32Match0,HI_S32 *ps32UnMatch0,
                            HI_S32 *ps32Match1,HI_S32 *ps32UnMatch1);
//TODO!!!  这里只有区域0 的，没有区域1 的
HI_S32 VPSS_REG_GetLasiCnt(HI_U32 * pu32AppAddr,
                            HI_S32 *ps32Cnt14,HI_S32 *ps32Cnt32,
                            HI_S32 *ps32Cnt34);
HI_S32 VPSS_REG_GetBlkSad(HI_U32 * pu32AppAddr,HI_S32 *pstData);
HI_S32 VPSS_REG_GetDbHyCnt(HI_U32 * pu32AppAddr,HI_U32 *pu32DbHyCnt);
HI_S32 VPSS_REG_GetDbBorder(HI_U32 * pu32AppAddr,HI_U32 *pu32DbBorder);
HI_S32 VPSS_REG_GetDbHySizeHyCnt(HI_U32 * pu32AppAddr,HI_U32 *pu32DbHySizeHyCounter);
HI_S32 VPSS_REG_GetDbHySizeHySize(HI_U32 * pu32AppAddr,HI_U32 *pu32DbHySizeHySize);
HI_S32 VPSS_REG_GetDbHcSizeHcCnt(HI_U32 * pu32AppAddr,HI_U32 *pu32DbHcSizeHcCounter);
HI_S32 VPSS_REG_GetDbHcSizeHcSize(HI_U32 * pu32AppAddr,HI_U32 *pu32DbHcSizeHcSize);
HI_S32 VPSS_REG_GetDbVySizeVyCnt(HI_U32 * pu32AppAddr,HI_U32 *pu32DbVySizeVyCounter);
HI_S32 VPSS_REG_GetDbVySizeVySize(HI_U32 * pu32AppAddr,HI_U32 *pu32DbVySizeVySize);


HI_S32 VPSS_REG_SetReeAddr(HI_U32 * pu32AppAddr,HI_U32 u32Yaddr,HI_U32 u32Caddr,HI_U32 u32YStr,HI_U32 u32CStr);
HI_S32 VPSS_REG_SetPrjCurAddr(HI_U32 * pu32AppAddr,HI_U32 u32Vaddr,HI_U32 u32Haddr,HI_U32 u32VStr,HI_U32 u32HStr);
HI_S32 VPSS_REG_SetRgmvAddr(HI_U32 * pu32AppAddr,HI_U32 u32Curaddr,HI_U32 u32Nx1addr,HI_U32 u32CurStr,HI_U32 u32Nx1Str);
HI_S32 VPSS_REG_SetBlkmvAddr(HI_U32 * pu32AppAddr,HI_U32 u32Curaddr,HI_U32 u32Refaddr,HI_U32 u32CurStr,HI_U32 u32RefStr);
HI_S32 VPSS_REG_SetCueAddr(HI_U32 * pu32AppAddr,HI_U32 u32Yaddr,HI_U32 u32Caddr,HI_U32 u32YStr,HI_U32 u32CStr);
HI_S32 VPSS_REG_SetPrjNx2Addr(HI_U32 * pu32AppAddr,HI_U32 u32Vaddr,HI_U32 u32Haddr,HI_U32 u32VStr,HI_U32 u32HStr);
HI_S32 VPSS_REG_SetRgmvNx2Addr(HI_U32 * pu32AppAddr,HI_U32 u32addr,HI_U32 u32Str);
HI_S32 VPSS_REG_SetBlkmvNx1Addr(HI_U32 * pu32AppAddr,HI_U32 u32addr,HI_U32 u32Str);
HI_S32 VPSS_REG_SetBlkmvNx1Addr(HI_U32 * pu32AppAddr,HI_U32 u32addr,HI_U32 u32Str);
HI_S32 VPSS_REG_SetBlkmvNx2Addr(HI_U32 * pu32AppAddr,HI_U32 u32addr,HI_U32 u32Str);
HI_S32 VPSS_REG_SetSNRMadRaddr(HI_U32 * pu32AppAddr,HI_U32 u32MadRaddr);
HI_S32 VPSS_REG_SetNx2Craddr(HI_U32 * pu32AppAddr,HI_U32 u32Craddr);


HI_S32 VPSS_REG_SetDieMaOnly(HI_U32 * pu32AppAddr,HI_BOOL bEn);
HI_S32 VPSS_REG_SetDieFldMotionShf(HI_U32 * pu32AppAddr,HI_U32 u32Mode);
HI_S32 VPSS_REG_SetDieLongMotionShf(HI_U32 * pu32AppAddr,HI_U32 u32Mode);

HI_S32 VPSS_REG_SetSmmuAddr(HI_U8 * pu32PhyAddr,HI_U32 u32PageAddr,HI_U32 u32ErrReadAddr,HI_U32 u32ErrWriteAddr);
HI_S32 VPSS_REG_SetTransMeta(HI_U32 *pu32AppAddr,
                                HI_U32 u32SrcAddr,
                                HI_U32 u32DstAddr,
                                HI_U32 u32Size);
                                
HI_S32 VPSS_REG_SetGlobalBypass(HI_U8* pu32PhyAddr,HI_BOOL bBypass);                               
#endif /* __C_UNION_DEFINE_VPSS_MV410_H__ */
