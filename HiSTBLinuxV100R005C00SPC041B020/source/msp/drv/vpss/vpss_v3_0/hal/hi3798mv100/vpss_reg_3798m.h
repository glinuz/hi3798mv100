#ifndef __VPSS_REG_3798M_H__
#define __VPSS_REG_3798M_H__

#include "vpss_common.h"
#include "vpss_reg_struct.h"
#include "hi_drv_reg.h"
#include "hi_reg_common.h"
#include "drv_pq_ext.h"

#define DEF_LOGIC_TIMEOUT  0xffffffff//0xe4e1c0

/*read/write reg*/
HI_S32 VPSS_REG_RegWrite(volatile HI_U32 *a, HI_U32 b);
HI_U32 VPSS_REG_RegRead(volatile HI_U32* a);

/*reset reg*/
HI_S32 VPSS_REG_ReSetCRG(HI_U32 u32Type);

HI_S32 VPSS_REG_BaseRegInit(VPSS_REG_S **ppstPhyReg);

HI_S32 VPSS_REG_AppRegInit(VPSS_REG_S **ppstAppReg,HI_U32 u32VirAddr);

HI_S32 VPSS_REG_ResetAppReg(HI_U32 u32AppAddr,VPSS_REG_S *pstPqReg);

/********************************/
HI_S32 VPSS_REG_SetIntMask(HI_U32 u32AppAddr,HI_U32 u32Mask);
HI_S32 VPSS_REG_GetIntMask(HI_U32 u32AppAddr,HI_U32 *pu32Mask);
HI_S32 VPSS_REG_GetIntState(HI_U32 u32AppAddr,HI_U32 *pu32Int);
HI_S32 VPSS_REG_GetRawIntState(HI_U32 u32AppAddr,HI_U32 *pu32RawInt);
HI_S32 VPSS_REG_ClearIntState(HI_U32 u32AppAddr,HI_U32 u32Data);
/********************************/


HI_S32 VPSS_REG_SetClockEn(HI_U32 u32Type, HI_BOOL bClkEn);
HI_S32 VPSS_REG_GetClockState(HI_BOOL *pbOpened);

HI_S32 VPSS_REG_SetMisCellaneous(HI_U32 u32AppAddr,HI_U32 u32Value);

HI_S32 VPSS_REG_SetTimeOut(HI_U32 u32AppAddr,HI_U32 u32Data);

HI_S32 VPSS_REG_StartLogic(HI_U32 u32AppAddr,HI_U32 u32PhyAddr);

HI_S32 VPSS_REG_EnPort(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_BOOL bEnable);

HI_S32 VPSS_REG_SetProtEn(HI_U32 u32AppAddr,HI_U32 u32Prot);

/********************************/

HI_S32 VPSS_REG_SetImgSize(HI_U32 u32AppAddr,HI_U32 u32Width,HI_U32 u32Height,HI_BOOL bProgressive);
HI_S32 VPSS_REG_SetImgAddr(HI_U32 u32AppAddr,REG_FIELDPOS_E ePos,HI_U32 u32Yaddr,HI_U32 u32Cbaddr, HI_U32 u32Craddr);
HI_S32 VPSS_REG_SetImgStride(HI_U32 u32AppAddr,REG_FIELDPOS_E ePos,HI_U32 u32YStride,HI_U32 u32CStride);
HI_S32 VPSS_REG_SetImgFormat(HI_U32 u32AppAddr,HI_DRV_PIX_FORMAT_E eFormat);
HI_S32 VPSS_REG_SetImgTile(HI_U32 u32AppAddr,REG_FIELDPOS_E ePos,HI_U32 EnTile);
HI_S32 VPSS_REG_SetImgOffset(HI_U32 u32AppAddr,REG_FIELDPOS_E ePos,HI_U32 u32HorOffset,HI_U32 u32VerOffset);
HI_S32 VPSS_REG_SetImgReadMod(HI_U32 u32AppAddr,HI_BOOL bField);
/********************************/

/********************************/
HI_S32 VPSS_REG_SetFrmSize(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32Height,HI_U32 u32Width);
HI_S32 VPSS_REG_SetFrmAddr(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32Yaddr,HI_U32 u32Caddr);
HI_S32 VPSS_REG_SetFrmStride(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32YStride,HI_U32 u32CStride);
HI_S32 VPSS_REG_SetFrmFormat(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_DRV_PIX_FORMAT_E eFormat);
HI_S32 VPSS_REG_SetPortUVInvert(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort, HI_BOOL bUVInvert);

/********************************/

/********************************/
//HI_S32 VPSS_REG_SetZmeEnable(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,REG_ZME_MODE_E eMode,HI_BOOL bEnable);
//HI_S32 VPSS_REG_SetZmeInSize(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32Height,HI_U32 u32Width);
//HI_S32 VPSS_REG_SetZmeOutSize(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32Height,HI_U32 u32Width);
#if 0
HI_S32 VPSS_REG_SetZmeFirEnable(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,
                                    REG_ZME_MODE_E eMode,HI_BOOL bEnable);
#endif
#if 0
HI_S32 VPSS_REG_SetZmeMidEnable(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,
                                    REG_ZME_MODE_E eMode,HI_BOOL bEnable);
#endif
#if 0                                    
HI_S32 VPSS_REG_SetZmePhase(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,
                                    REG_ZME_MODE_E eMode,HI_S32 s32Phase);
#endif
#if 0
HI_S32 VPSS_REG_SetZmeRatio(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,
                                    REG_ZME_MODE_E eMode,HI_U32 u32Ratio);
#endif
//HI_S32 VPSS_REG_SetZmeHfirOrder(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_BOOL bVfirst);
//HI_S32 VPSS_REG_SetZmeInFmt(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_DRV_PIX_FORMAT_E eFormat);
//HI_S32 VPSS_REG_SetZmeOutFmt(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_DRV_PIX_FORMAT_E eFormat);
//HI_S32 VPSS_REG_SetZmeCoefAddr(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,
//                                    REG_ZME_MODE_E eMode,HI_U32 u32Addr);
/*LTI/CTI*/
HI_S32 VPSS_REG_SetLTIEn(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_BOOL  bEnLTI);
HI_S32 VPSS_REG_SetLTIDemo(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_BOOL  bEnDemoLTI);
HI_S32 VPSS_REG_SetLGainRatio(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_S32  s32Ratio);
HI_S32 VPSS_REG_SetLGainCoef(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_S8  *ps8Coef);
HI_S32 VPSS_REG_SetLMixingRatio(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_S32  s32Ratio);
HI_S32 VPSS_REG_SetLCoringThd(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_S32  s32Thd);
HI_S32 VPSS_REG_SetLSwing(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_S32  s32Under,HI_S32  s32Over);
HI_S32 VPSS_REG_SetLHpassCoef(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_S8  *ps8Coef);
HI_S32 VPSS_REG_SetLHfreqThd(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_S16  *ps16Thd);

HI_S32 VPSS_REG_SetCTIEn(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_BOOL  bEnCTI);
HI_S32 VPSS_REG_SetCTIDemo(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_BOOL  bEnDemoCTI);
HI_S32 VPSS_REG_SetCGainRatio(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_S32  s32Ratio);
HI_S32 VPSS_REG_SetCMixingRatio(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_S32  s32Ratio);
HI_S32 VPSS_REG_SetCCoringThd(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_S32  s32Thd);
HI_S32 VPSS_REG_SetCSwing(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_S32  s32Under,HI_S32  s32Over);
HI_S32 VPSS_REG_SetCHpassCoef(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_S8  *ps8Coef);


/********************************/

/********************************/
HI_VOID VPSS_REG_SetDetEn(HI_U32 u32AppAddr,HI_BOOL bEnable);
HI_VOID VPSS_REG_SetDetMode(HI_U32 u32AppAddr,HI_U32 u32Mode);
HI_VOID VPSS_REG_SetDetBlk(HI_U32 u32AppAddr,HI_U32 blk_id, HI_U32 *pu32Addr);
HI_VOID VPSS_REG_GetDetPixel(HI_U32 u32AppAddr,HI_U32 BlkNum, HI_U8* pstData);
/********************************/


/********************************/
/*DEI*/
HI_S32 VPSS_REG_EnDei(HI_U32 u32AppAddr,HI_BOOL bEnDei);
HI_S32 VPSS_REG_RstDei(HI_U32 u32AppAddr,HI_BOOL bRst,HI_U32 u32RstValue);
HI_S32 VPSS_REG_SetDeiTopFirst(HI_U32 u32AppAddr,HI_BOOL bTopFirst);
HI_S32 VPSS_REG_SetDeiFieldMode(HI_U32 u32AppAddr,HI_BOOL bBottom);
HI_S32 VPSS_REG_SetDeiAddr(HI_U32 u32AppAddr,REG_FIELDPOS_E eField,HI_U32 u32YAddr,HI_U32 u32CAddr, HI_U32 u32CrAddr);
HI_S32 VPSS_REG_SetDeiStride(HI_U32 u32AppAddr,REG_FIELDPOS_E eField,HI_U32 u32YStride,HI_U32 u32CStride);
HI_S32 VPSS_REG_SetModeEn(HI_U32 u32AppAddr,REG_DIE_MODE_E eDieMode,HI_BOOL bEnMode);
HI_S32 VPSS_REG_SetOutSel(HI_U32 u32AppAddr,REG_DIE_MODE_E eDieMode,HI_BOOL bEnMode);
HI_S32 VPSS_REG_SetMode(HI_U32 u32AppAddr,REG_DIE_MODE_E eDieMode,HI_U32  u32Mode);
HI_S32 VPSS_REG_SetStInfo(HI_U32 u32AppAddr,HI_BOOL bStop)
;
HI_S32 VPSS_REG_SetMfMax(HI_U32 u32AppAddr,REG_DIE_MODE_E eDieMode,HI_BOOL bMax);
HI_S32 VPSS_REG_SetLuSceSdfMax(HI_U32 u32AppAddr,HI_BOOL bMax);
HI_S32 VPSS_REG_SetRecModeEn(HI_U32 u32AppAddr,HI_BOOL bEnMode);
HI_S32 VPSS_REG_SetMotionIIrEn(HI_U32 u32AppAddr,HI_BOOL bEnIIr);
HI_S32 VPSS_REG_SetMotionSmoothEn(HI_U32 u32AppAddr,HI_BOOL bEnSmooth);
HI_S32 VPSS_REG_SetMotionBlendEn(HI_U32 u32AppAddr,HI_BOOL bEnBlend);
HI_S32 VPSS_REG_SetChromeOffset(HI_U32 u32AppAddr,HI_U8 u8Offset);
HI_S32 VPSS_REG_SetMinIntern(HI_U32 u32AppAddr,HI_U32 u32MinIntern);
HI_S32 VPSS_REG_SetInternVer(HI_U32 u32AppAddr,HI_U32 u32InternVer);
HI_S32 VPSS_REG_SetRangeScale(HI_U32 u32AppAddr,HI_U32 u32Scale);
HI_S32 VPSS_REG_SetCK1(HI_U32 u32AppAddr,HI_U32 u32Gain,HI_U32 u32Range,HI_U32 u32Max);
HI_S32 VPSS_REG_SetCK2(HI_U32 u32AppAddr,HI_U32 u32Gain,HI_U32 u32Range,HI_U32 u32Max);
HI_S32 VPSS_REG_SetDIR(HI_U32 u32AppAddr,HI_S32 s32MultDir[15]);
HI_S32 VPSS_REG_SetCcEn(HI_U32 u32AppAddr,HI_BOOL bEnCc);
HI_S32 VPSS_REG_SetCcOffset(HI_U32 u32AppAddr,HI_S32 s32Offset);
HI_S32 VPSS_REG_SetCcDetMax(HI_U32 u32AppAddr,HI_S32 s32Max);
HI_S32 VPSS_REG_SetCcDetThd(HI_U32 u32AppAddr,HI_S32 s32Thd);
HI_S32 VPSS_REG_SetSimiMax(HI_U32 u32AppAddr,HI_S32 s32SimiMax);
HI_S32 VPSS_REG_SetSimiThd(HI_U32 u32AppAddr,HI_S32 s32SimiThd);
HI_S32 VPSS_REG_SetDetBlend(HI_U32 u32AppAddr,HI_S32 s32DetBlend);
HI_S32 VPSS_REG_SetMaxXChroma(HI_U32 u32AppAddr,HI_S32 s32Max);
HI_S32 VPSS_REG_SetIntpSclRat(HI_U32 u32AppAddr,HI_S32 s32Rat[15]);
HI_S32 VPSS_REG_SetStrenThd(HI_U32 u32AppAddr,HI_S32 s32Thd);
HI_S32 VPSS_REG_SetDirThd(HI_U32 u32AppAddr,HI_S32 s32Thd);
HI_S32 VPSS_REG_SetBcGain(HI_U32 u32AppAddr,HI_S32 s32BcGain);

HI_S32 VPSS_REG_EnHorEdge(HI_U32 u32AppAddr,HI_BOOL bEdgeEn);
HI_S32 VPSS_REG_EnEdgeMode(HI_U32 u32AppAddr,HI_BOOL bEdgeModeEn);
HI_S32 VPSS_REG_SetMotionCoring(HI_U32 u32AppAddr,HI_S32 s32Coring);
HI_S32 VPSS_REG_SetJitterCoring(HI_U32 u32AppAddr,HI_S32 s32Coring);
HI_S32 VPSS_REG_SetJitterGain(HI_U32 u32AppAddr,HI_S32 s32Gain);

HI_S32 VPSS_REG_SetMotionSlope(HI_U32 u32AppAddr,HI_S32 s32Slope);
HI_S32 VPSS_REG_SetMotionGain(HI_U32 u32AppAddr,HI_S32 s32Gain);
HI_S32 VPSS_REG_SetMotionHThd(HI_U32 u32AppAddr,HI_S32 s32HThd);
HI_S32 VPSS_REG_SetMotionLThd(HI_U32 u32AppAddr,HI_S32 s32LThd);

HI_S32 VPSS_REG_SetMotionDiffThd(HI_U32 u32AppAddr,HI_S32 s32Thd[8]);
HI_S32 VPSS_REG_SetMotionIIrSlope(HI_U32 u32AppAddr,HI_S32 *ps32Slope);
HI_S32 VPSS_REG_SetMotionIIrRatio(HI_U32 u32AppAddr,HI_S32 *ps32Ratio);
HI_S32 VPSS_REG_SetMotionIIrRatioMaxMin(HI_U32 u32AppAddr,HI_S32 s32Max,HI_S32 s32Min);

HI_S32 VPSS_REG_SetSmoothDiffThd(HI_U32 u32AppAddr,HI_S32 *ps32Thd);
HI_S32 VPSS_REG_SetSmoothSlope(HI_U32 u32AppAddr,HI_S32 *ps32Slope);
HI_S32 VPSS_REG_SetSmoothRatio(HI_U32 u32AppAddr,HI_S32 *ps32Ratio);
HI_S32 VPSS_REG_SetSmoothRatioMaxMin(HI_U32 u32AppAddr,HI_S32 s32Max,HI_S32 s32Min);


HI_S32 VPSS_REG_SetBlendDiffThd(HI_U32 u32AppAddr,HI_S32 *ps32Thd);
HI_S32 VPSS_REG_SetBlendSlope(HI_U32 u32AppAddr,HI_S32 *ps32Slope);
HI_S32 VPSS_REG_SetBlendRatio(HI_U32 u32AppAddr,HI_S32 *ps32Ratio);
HI_S32 VPSS_REG_SetBlendRatioMaxMin(HI_U32 u32AppAddr,HI_S32 s32Max,HI_S32 s32Min);

HI_S32 VPSS_REG_SetMotionAdj(HI_U32 u32AppAddr,HI_S32 s32Gain,HI_S32 s32Chom,HI_S32 s32Coring);
HI_S32 VPSS_REG_SetMotionThd(HI_U32 u32AppAddr,HI_S32 s32Small,HI_S32 s32Large);

HI_S32 VPSS_REG_SetRecFldStep(HI_U32 u32AppAddr,HI_S32 s32Step[2]);
HI_S32 VPSS_REG_SetRecFrmStep(HI_U32 u32AppAddr,HI_S32 s32Step[2]);
HI_S32 VPSS_REG_SetHisEn(HI_U32 u32AppAddr,HI_BOOL bEnHis);
HI_S32 VPSS_REG_SetHisInfoWrMode(HI_U32 u32AppAddr,HI_BOOL bHisMtnInfoWrMd);
HI_S32 VPSS_REG_SetHisUseMode(HI_U32 u32AppAddr,HI_BOOL bHisMtnUseMd);
HI_S32 VPSS_REG_SetHisWrMode(HI_U32 u32AppAddr,HI_BOOL bHisMtnWrMd);
HI_S32 VPSS_REG_SetHisUseMode(HI_U32 u32AppAddr,HI_BOOL bHisMtnUseMd);
HI_S32 VPSS_REG_SetHisPreEn(HI_U32 u32AppAddr,HI_BOOL bEnPre);
HI_S32 VPSS_REG_SetHisPpreEn(HI_U32 u32AppAddr,HI_BOOL bEnPpre);
HI_S32 VPSS_REG_SetMorFlt(HI_U32 u32AppAddr,HI_BOOL bEnflt,HI_S8 s8FltSize,HI_S8 s8FltThd);
HI_S32 VPSS_REG_SetBlendEn(HI_U32 u32AppAddr,HI_BOOL bEnBlend);

HI_S32 VPSS_REG_SetCombLimit(HI_U32 u32AppAddr,HI_S32  s32UpLimit,HI_S32  s32DownLimit);
HI_S32 VPSS_REG_SetCombThd(HI_U32 u32AppAddr,HI_S32  s32Hthd,HI_S32  s32Vthd);
HI_S32 VPSS_REG_SetCombEn(HI_U32 u32AppAddr,HI_BOOL bEnComb);
HI_S32 VPSS_REG_SetCombMdThd(HI_U32 u32AppAddr,HI_S32 s32MdThd);
HI_S32 VPSS_REG_SetCombEdgeThd(HI_U32 u32AppAddr,HI_S32 s32EdgeThd);

HI_S32 VPSS_REG_SetStWrAddr(HI_U32 u32AppAddr,HI_U32 u32Addr);
HI_S32 VPSS_REG_SetStRdAddr(HI_U32 u32AppAddr,HI_U32 u32Addr);

HI_S32 VPSS_REG_SetStStride(HI_U32 u32AppAddr,HI_U32 u32Stride);
HI_S32 VPSS_REG_SetDeiParaAddr(HI_U32 u32AppAddr,HI_U32 u32ParaAddr);

/********************************/

/********************************/
HI_S32 VPSS_REG_SetVc1En(HI_U32 u32AppAddr,HI_U32 u32EnVc1);
HI_S32 VPSS_REG_SetVc1Profile(HI_U32 u32AppAddr,REG_FRAMEPOS_E ePos,HI_U32 u32Profile);
HI_S32 VPSS_REG_SetVc1Map(HI_U32 u32AppAddr,REG_FRAMEPOS_E ePos,HI_U32 u32MapY,HI_U32 u32MapC,
                            HI_U32 u32BMapY,HI_U32 u32BMapC);
HI_S32 VPSS_REG_SetVc1MapFlag(HI_U32 u32AppAddr,REG_FRAMEPOS_E ePos,HI_U32 u32YFlag,HI_U32 u32CFlag,
                            HI_U32 u32BYFlag,HI_U32 u32BCFlag);
HI_S32 VPSS_REG_SetVc1RangeEn(HI_U32 u32AppAddr,REG_FRAMEPOS_E ePos,HI_U32 u32EnVc1);
                         
/********************************/

HI_S32 VPSS_REG_SetUVConvertEn(HI_U32 u32AppAddr,HI_U32 u32EnUV);

HI_S32 VPSS_REG_GetHisBin(HI_U32 u32AppAddr,HI_S32 *pstData);

HI_S32 VPSS_REG_GetItDiff(HI_U32 u32AppAddr,HI_S32 *pstData);

HI_S32 VPSS_REG_GetPdMatch(HI_U32 u32AppAddr,
                            HI_S32 *ps32Match0,HI_S32 *ps32UnMatch0,
                            HI_S32 *ps32Match1,HI_S32 *ps32UnMatch1);
                            
HI_S32 VPSS_REG_GetLasiCnt(HI_U32 u32AppAddr,
                            HI_S32 *ps32Cnt14,HI_S32 *ps32Cnt32,
                            HI_S32 *ps32Cnt34);


HI_S32 VPSS_REG_GetPdIchd(HI_U32 u32AppAddr,HI_S32 *pstData);

HI_S32 VPSS_REG_GetBlkSad(HI_U32 u32AppAddr,HI_S32 *pstData);

//modify by z214841 for 3798mv100        
HI_S32 VPSS_REG_GetPccData(HI_U32 u32AppAddr,HI_S32 *ps32FFWD,
                             HI_S32 *ps32FWD,HI_S32 *ps32BWD,HI_S32 *ps32CRSS,
                             /*HI_S32 *ps32PW,*/HI_S32 *ps32FWDTKR,HI_S32 *ps32WDTKR);


HI_S32 VPSS_REG_SetDeiEdgeSmoothRatio(HI_U32 u32AppAddr,HI_S8 u8Data);
HI_S32 VPSS_REG_SetDeiStillBlkThd(HI_U32 u32AppAddr,HI_S8 u8Data);
HI_S32 VPSS_REG_SetDeiHistThd(HI_U32 u32AppAddr,HI_S8 *pu8Data);
HI_S32 VPSS_REG_SetDeiUmThd(HI_U32 u32AppAddr,HI_S8 *pu8Data);
HI_S32 VPSS_REG_SetDeiCoring(HI_U32 u32AppAddr,HI_S8 s8CorBlk,HI_S8 s8CorNorm,HI_S8 s8CorTkr);
HI_S32 VPSS_REG_SetDeiMovCoring(HI_U32 u32AppAddr,HI_S8 s8Blk,HI_S8 s8Norm,HI_S8 s8Tkr);
HI_S32 VPSS_REG_SetDeiPccHThd(HI_U32 u32AppAddr,HI_S8 s8Data);
HI_S32 VPSS_REG_SetDeiPccVThd(HI_U32 u32AppAddr,HI_S8 *ps8Data);
HI_S32 VPSS_REG_SetDeiItDiff(HI_U32 u32AppAddr,HI_S8 *ps8Data);
HI_S32 VPSS_REG_SetDeiLasiCtrl(HI_U32 u32AppAddr,HI_S8 s8Thr,HI_S8 s8EdgeThr,HI_S8 s8lasiThr);
HI_S32 VPSS_REG_SetDeiPdBitMove(HI_U32 u32AppAddr,HI_S32  s32Data);
HI_S32 VPSS_REG_SetDeiDirMch(HI_U32 u32AppAddr,HI_BOOL  bNext);
HI_S32 VPSS_REG_SetEdgeSmooth(HI_U32 u32AppAddr,HI_BOOL  bEdgeSmooth);



/**************************/



/*DR*/
HI_S32 VPSS_REG_SetDREn(HI_U32 u32AppAddr,HI_BOOL  bEnDR);
HI_S32 VPSS_REG_SetDRDemoEn(HI_U32 u32AppAddr,HI_BOOL  bEnDRDemo);
HI_S32 VPSS_REG_SetDRPara(HI_U32 u32AppAddr,HI_S32  s32FlatThd,HI_S32  s32SimiThd,
                            HI_S32 s32AlphaScale,HI_S32 s32BetaScale);
/*DB*/                          
HI_S32 VPSS_REG_SetDBEn(HI_U32 u32AppAddr,HI_BOOL  bEnDB);
HI_S32 VPSS_REG_SetDBVH(HI_U32 u32AppAddr,HI_BOOL  bEnVert, HI_BOOL bEnHor);
HI_S32 VPSS_REG_SetEdgeThd(HI_U32 u32AppAddr,HI_S32  s32Thd);
//HI_S32 VPSS_REG_SetVerProg(HI_U32 u32AppAddr,HI_BOOL  bProg);
HI_S32 VPSS_REG_SetThrGrad(HI_U32 u32AppAddr,HI_BOOL  bgrad);
HI_S32 VPSS_REG_SetTextEn(HI_U32 u32AppAddr,HI_BOOL  btexten);
HI_S32 VPSS_REG_SetWeakFlt(HI_U32 u32AppAddr,HI_BOOL  bWeak);
HI_S32 VPSS_REG_SetMaxDiff(HI_U32 u32AppAddr,HI_S32  s32VerMax,HI_S32  s32HorMax);
HI_S32 VPSS_REG_SetLeastDiff(HI_U32 u32AppAddr,HI_S32  s32VerLeast,HI_S32  s32HorLeast);
HI_S32 VPSS_REG_SetScale(HI_U32 u32AppAddr,HI_S32  s32Alpha,HI_S32  s32Beta);
HI_S32 VPSS_REG_SetSmoothThd(HI_U32 u32AppAddr,HI_S32  s32Thd);
HI_S32 VPSS_REG_SetQpThd(HI_U32 u32AppAddr,HI_S32  s32Thd);
HI_S32 VPSS_REG_SetPicestQp(HI_U32 u32AppAddr,HI_S32  s32Picest);
HI_S32 VPSS_REG_SetDnrInfo(HI_U32 u32AppAddr,HI_U32  u32Rcnt,HI_U32  u32Bcnt,HI_U32  u32MaxGrad,HI_U32  u32Cntrst8);







/*LBOX*/
HI_S32 VPSS_REG_SetLBAEn(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_BOOL bEnLba);
HI_S32 VPSS_REG_SetLBADispPos(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32XFPos,HI_U32 u32YFPos,
                            HI_U32 u32Height,HI_U32 u32Width);
HI_S32 VPSS_REG_SetLBAVidPos(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32XFPos,HI_U32 u32YFPos,
                            HI_U32 u32Height,HI_U32 u32Width);

/********************************/
HI_S32 VPSS_REG_SetHsclEnable(HI_U32 u32AppAddr,REG_ZME_MODE_E eMode,HI_BOOL bEnable);

HI_S32 VPSS_REG_SetHsclOutSize(HI_U32 u32AppAddr,HI_U32 u32Width);

HI_S32 VPSS_REG_SetHsclFirEnable(HI_U32 u32AppAddr,REG_ZME_MODE_E eMode,HI_BOOL bEnable);
HI_S32 VPSS_REG_SetHsclMidEnable(HI_U32 u32AppAddr,REG_ZME_MODE_E eMode,HI_BOOL bEnable);
HI_S32 VPSS_REG_SetHsclPhase(HI_U32 u32AppAddr,REG_ZME_MODE_E eMode,HI_S32 s32Phase);                                    
HI_S32 VPSS_REG_SetHsclRatio(HI_U32 u32AppAddr,HI_U32 u32Ratio);         
HI_S32 VPSS_REG_SetHsclCoefAddr(HI_U32 u32AppAddr,REG_ZME_MODE_E eMode,HI_U32 u32Addr);                                    
                                    
/********************************/

HI_S32 VPSS_REG_SetVhdCmpEn(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_BOOL  bEnVhdCmp);
HI_S32 VPSS_REG_SetVhdCmpAddr(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort, HI_U32 u32YHeadaddr,HI_U32 u32CHeadaddr);
HI_S32 VPSS_REG_SetVhdCmpDrr(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort, HI_U32 u32VhdCmpDrr);
HI_S32 VPSS_REG_SetVhdCmpLossyEn(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort, HI_BOOL  bEnVhdCmpLossy);
HI_S32 VPSS_REG_SetDcmpEn(HI_U32 u32AppAddr,HI_BOOL  bEnDcmp);
HI_S32 VPSS_REG_SetDcmpHeadAddr(HI_U32 u32AppAddr,REG_FIELDPOS_E ePos,HI_U32 u32YHeadaddr,HI_U32 u32CHeadaddr);
HI_S32 VPSS_REG_SetTunlEn(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_BOOL  bEnTunl);
HI_S32 VPSS_REG_SetTunlFinishLine(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_S32  s32FinishLine);
HI_S32 VPSS_REG_SetTunlMode(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort, REG_TUNLPOS_E  s32TunlMode);
HI_S32 VPSS_REG_SetTunlAddr(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32TunlAddr);
HI_S32 VPSS_REG_SetCurTunlAddr(HI_U32 u32AppAddr,REG_FRAMEPOS_E  ePort,HI_U32 u32TunlAddr);
HI_S32 VPSS_REG_SetCurTunlEn(HI_U32 u32AppAddr,HI_BOOL u32CurTunlEn);
HI_S32 VPSS_REG_SetCurTunlInterval(HI_U32 u32AppAddr,REG_FRAMEPOS_E ePort,HI_S32  s32CurTunlInterval);
HI_S32 VPSS_REG_SetCscEn(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_BOOL  bEnCSC);
HI_S32 VPSS_REG_SetCscIdc(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32CscIdc0,HI_U32 u32CscIdc1,HI_U32 u32CscIdc2);
HI_S32 VPSS_REG_SetCscOdc(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32CscOdc0,HI_U32 u32CscOdc1,HI_U32 u32CscOdc2);
HI_S32 VPSS_REG_SetCscP00(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32CscP00);
HI_S32 VPSS_REG_SetCscP01(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32CscP01);
HI_S32 VPSS_REG_SetCscP02(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32CscP02);
HI_S32 VPSS_REG_SetCscP10(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32CscP10);
HI_S32 VPSS_REG_SetCscP11(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32CscP11);
HI_S32 VPSS_REG_SetCscP12(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32CscP12);
HI_S32 VPSS_REG_SetCscP20(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32CscP20);
HI_S32 VPSS_REG_SetCscP21(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32CscP21);
HI_S32 VPSS_REG_SetCscP22(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32CscP22);
#if 1
HI_S32 VPSS_REG_SetInCropPos(HI_U32 u32AppAddr,HI_U32 u32InCropY,HI_U32 u32InCropX);
HI_S32 VPSS_REG_SetInCropEn(HI_U32 u32AppAddr,HI_BOOL bInCropEn);
HI_S32 VPSS_REG_SetInCropMode(HI_U32 u32AppAddr,HI_BOOL bInCropMode);
HI_S32 VPSS_REG_SetInCropSize(HI_U32 u32AppAddr,HI_U32 u32InCropHeight,HI_U32 u32InCropWidth);
HI_S32 VPSS_REG_SetPortCropPos(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32CropY,HI_U32 u32CropX);
HI_S32 VPSS_REG_SetPortCropSize(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32CropHeight,HI_U32 u32CropWidth);
HI_S32 VPSS_REG_SetPortCropEn(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_BOOL bPortCropEn);
#endif

HI_S32 VPSS_REG_SetPortMirrorEn(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort, HI_BOOL bMirrorEn);
HI_S32 VPSS_REG_SetPortFlipEn(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort, HI_BOOL bFlipEn);
HI_S32 VPSS_REG_SetPreHfirEn(HI_U32 u32AppAddr, HI_BOOL bHfirEn);
HI_S32 VPSS_REG_SetPreHfirMode(HI_U32 u32AppAddr, HI_U32 u32HfirMode);
HI_S32 VPSS_REG_SetPreVfirEn(HI_U32 u32AppAddr,HI_BOOL bVfirEn);
HI_S32 VPSS_REG_SetPreVfirMode(HI_U32 u32AppAddr, HI_U32 u32VfirMode);
//HI_S32 VPSS_REG_SetRotation(HI_U32 u32AppAddr,HI_U32 u32Angle);
HI_S32 VPSS_REG_SetRotation(HI_U32 u32AppAddr,HI_U32 u32Angle, HI_U32 u32ProMode);

HI_S32 VPSS_REG_GetReg(HI_U32 u32AppAddr,HI_U32 *pu32Int);

HI_S32 VPSS_REG_SetLBABg(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32Color,HI_U32 u32Alpha);
HI_S32 VPSS_REG_SetLBADispPos(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32XFPos,HI_U32 u32YFPos,
                            HI_U32 u32Height,HI_U32 u32Width);
HI_S32 VPSS_REG_SetLBAVidPos(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32XFPos,HI_U32 u32YFPos,
                            HI_U32 u32Height,HI_U32 u32Width);
HI_S32 VPSS_REG_SetOutCropVidPos(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32XFPos,HI_U32 u32YFPos,
                            HI_U32 u32Height,HI_U32 u32Width);
HI_S32 VPSS_REG_SetFidelity(HI_U32 u32AppVAddr,HI_BOOL bEnFidelity); 

HI_S32 VPSS_REG_SetStMode(HI_U32 u32AppVAddr,HI_BOOL bLumaMax,HI_BOOL bChromaMax);

HI_S32 VPSS_REG_SetPreZme(HI_U32 u32AppVAddr,
                            VPSS_REG_PREZME_E enHor,VPSS_REG_PREZME_E enVer);
HI_S32 VPSS_REG_SetPortZmeEn(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort, HI_BOOL bPortZmeEn);
                            

HI_S32 VPSS_REG_SetProtEn(HI_U32 u32AppAddr,HI_BOOL  bSecure);
#endif
