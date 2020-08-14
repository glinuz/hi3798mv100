/*
  Copyright (C) 2010 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmbslHCDP_Reg.h
 *
 *                %version: 2 %
 *
 * \date          %modify_time%
 *
 * \author        Alexandre TANT
 *
 * \brief         Describe briefly the purpose of this file.
 *
 * REFERENCE DOCUMENTS :
 *
 *
 * TVFE SW Arch V4 Template: Author Christophe CAZETTES
 *
 * \section info Change Information
 *
*/

/* File generated automatically from register description file */

#pragma once
#ifndef _TMBSL_HCDP_REG_H
#define _TMBSL_HCDP_REG_H

#ifdef __cplusplus
extern "C" {
#endif  /*__cplusplus*/

/*============================================================================*/
/* Registers definitions:                                                     */
/*============================================================================*/

typedef enum
{
    HCDPRegCdpVersion = 0,
        
    HCDPRegQamMode,
    
    HCDPRegSpectralInversion,
    
    HCDPRegFecMode,
    HCDPRegNxbRsMpegSyncSel,
    
    HCDPRegAdcClkPol,
    HCDPRegZeroAdcData,
    HCDPRegAdc2Comp,
    HCDPRegAdc4TestMode,
    
//    HCDPRegClkGenDclkDivSel,
    HCDPRegClkGenShutDclk,
    HCDPRegClkGenShutTclk,
    HCDPRegClkGenShutNxa,
    HCDPRegClkGenNxaClkFreeRun,
    HCDPRegClkGenRclkFreeRun,
    HCDPRegClkGenPrclkFreeRun,
    HCDPRegClkGenCsmSerClkFreeRun,
    HCDPRegClkGenCsmSerClkShut,
    HCDPRegClkGenReset,
    
//    HCDPRegClkGenDclkPhase,
    HCDPRegAdcFifoFreqWord,
    
    HCDPRegCdpSoftRst,
    HCDPRegCdpMicroRst,
    HCDPRegCdpFullRst,
    HCDPRegDemodSoftRst,
    HCDPRegDemodMicroRst,
    HCDPRegFecSoftRst,
    HCDPRegFecMicroRst,
    HCDPRegCsmSoftRst,
    HCDPRegCsmMicroRst,

    HCDPRegMiMinWrWait,
    HCDPRegMiMaxWrWait,

    HCDPRegMiMinRdWait,
    HCDPRegMiMaxRdWait,

    HCDPRegDemodWrDisable,
    HCDPRegFecWrDisable,
    HCDPRegTimeoutWrFlag,
    HCDPRegTimeoutRdFlag,

    HCDPRegAsxDackCtrl,

    HCDPRegGpioOut,
    HCDPRegGpioIn,
    
    HCDPRegSerTestDisable,
    HCDPRegSerTestErr0,

    HCDPRegSerGenCtrl,

    HCDPRegSerGenStatus,

    HCDPRegClkSmtrSoftRst,
    HCDPRegClkSmtrMicroRst,
    HCDPRegClkSmtrSoftRstSync,
    HCDPRegClkSmtrMicroRstSync,

    HCDPRegClkSmtrSyncWordPuncMode,
    HCDPRegClkSmtrSyncWordOvrDis,
    HCDPRegClkSmtrTeiSetDis,

    HCDPRegClkSmtrOutClkParPol,
    HCDPRegClkSmtrOutClkSerPol,
    HCDPRegClkSmtrOutClkParSel,
    HCDPRegClkSmtrInClkPol,

    HCDPRegClkSmtrDfailBeforeStart,
    HCDPRegClkSmtrDfailBeforeSync,
    HCDPRegClkSmtrDataBeforeStart,
    HCDPRegClkSmtrDataBeforeSync,
    HCDPRegClkSmtrDataWhileDfail,

    HCDPRegClkSmtrOutClkAfterStart,
    HCDPRegClkSmtrOutClkAfterSync,
    HCDPRegClkSmtrOutClkStopEn,
    HCDPRegClkSmtrValidAfterStart,
    HCDPRegClkSmtrValidAfterSync,
    HCDPRegClkSmtrValidStopEn,
    HCDPRegClkSmtrUseDclkDsync,

    HCDPRegClkSmtrFifoAutoMode,
    HCDPRegClkSmtrDdaStepSel,

    HCDPRegClkSmtrManualModulo,

    HCDPRegClkSmtrAutoModulo,
    HCDPRegClkSmtrManualNumerator,

    HCDPRegClkSmtrAutoNumerator,

    HCDPRegClkSmtrFifoEmptyCnt,
    HCDPRegClkSmtrFifoFullCnt,

    HCDPRegClkSmtrFifoMinCapacity,
    HCDPRegClkSmtrFifoMaxCapacity,

    HCDPRegClkSmtrFifoKickDownCnt,
    HCDPRegClkSmtrFifoKickUpCnt,

    HCDPRegPnBerMode,
    HCDPRegPnBerDemodMode,
    HCDPRegPnBerWinSel,
    HCDPRegPnBerExtWinSel,
    HCDPRegPnBerInfiniteMode,
    HCDPRegPnBerScaleResult,
    HCDPRegPnBerHeaderMode,
    
    HCDPRegPnBerBreakSel,
    HCDPRegPnBerBreakEn,

    HCDPRegPnBerStart,
    HCDPRegPnBerRestart,
    HCDPRegPnBerReset,

    HCDPRegPnBerIdle,
    HCDPRegPnBerLock,
    HCDPRegPnBerSeq,
    HCDPRegPnBerPol,

    HCDPRegPnBerReadyCount,
    HCDPRegPnBerResult,

    HCDPRegPnBerBreakCount,
    HCDPRegPnBerViolationCount,
    HCDPRegPnBerErrorCount,

    HCDPRegCheckCrcMode,

    HCDPRegCheckCrcValue,

    HCDPRegCheckCrcResult,

    HCDPRegCheckPidMode,

    HCDPRegCheckPidValue,

    HCDPRegCheckPidResult,

    HCDPRegCheckCcErrorCnt,
    HCDPRegCheckCcResult,

    HCDPRegClkSmtrUseCombDout,
    HCDPRegClkSmtrClrRegDout,

    HCDPRegRsDecodWinMode,
    HCDPRegRsDecodCsmMode,

    HCDPRegRsDecodClear,
    HCDPRegRsDecodFreeze,

    HCDPRegRsDecodUncorrectableCwHigh,
    HCDPRegRsDecodUncorrectableCwLow,

    HCDPRegRsDecodSymErrsHigh,
    HCDPRegRsDecodSymErrsLow,

    HCDPRegRsDecodBitErrsHigh,
    HCDPRegRsDecodBitErrsLow,

    HCDPRegRsDecodUnerroredCwHigh,
    HCDPRegRsDecodUnerroredCwLow,

    HCDPRegRsDecodCorrectedCwHigh,
    HCDPRegRsDecodCorrectedCwLow,

    HCDPRegRsDecodUseCombDout,
    HCDPRegRsDecodClrRegDout,

    HCDPRegRsDecodCodewordCount,
    HCDPRegRsDecodWindowCount,

    HCDPRegNxbTrNormWindow,

    HCDPRegNxbTrNormThr,

    HCDPRegNxbTrNormCountI,
    HCDPRegNxbTrNormCountQ,

    HCDPRegNxbTrSync,

    HCDPRegNxbTrManualSync,
    HCDPRegNxbTrAdvanceSync,

    HCDPRegNxbTrNormCntSel,
    HCDPRegNxbTrTclkDly,

    HCDPRegNxbTrNormCntReadyI,
    HCDPRegNxbTrNormCntReadyQ,

    HCDPRegNxbTrSkipCnt,
    HCDPRegNxbTrSkipCntClr,

    HCDPRegNxbTrNormCntIpeak,
    HCDPRegNxbTrNormCntQpeak,

    HCDPRegNxbTrNormPeakClr,

    HCDPRegNxbTrRclkGapCnt,
    HCDPRegNxbTrSymWidthCnt,

    HCDPRegNxbTrRclkGapClr,

    HCDPRegNxbFsyncIntoSyncThr,
    
    HCDPRegNxbFsyncIntoSyncWin,

    HCDPRegNxbFsyncOosThr,
    
    HCDPRegNxbFsyncOosWin,

    HCDPRegNxbFsyncRsOosDis,
    HCDPRegNxbFsyncMajOosDis,
    HCDPRegNxbFsyncSmOosDis,

    HCDPRegNxbFsyncSmCntClr,

    HCDPRegNxbFsyncInSync,
    HCDPRegNxbFsyncTimingSync,
    HCDPRegNxbFsyncSearch,
    HCDPRegNxbFsyncChangeState,
    HCDPRegNxbFsyncActual_IJ_Value,
    HCDPRegNxbFsyncActual_IJ_Quality,
    HCDPRegNxbFsync_IJ_ChangeCount,

    HCDPRegNxbFsyncCurr_IJ_Value,
    HCDPRegNxbFsyncMaj_IJ_Value,
    HCDPRegNxbFsyncMaj_IJ_Quality,

    HCDPRegNxbFsyncManual_IJ_Value,
    HCDPRegNxbFsyncManualDeintMode,
    HCDPRegNxbFsyncIJ_ModifyDis,

    HCDPRegNxbFsyncMajThr,

    HCDPRegNxbFsyncRclkDly,

    HCDPRegNxbDeintBypass,
    
    HCDPRegNxbDeintEnhancedDis,
    
    HCDPRegNxbDerandBypass,

    HCDPRegNxbDeintActualMode,

    HCDPRegNxbRsMpegCrcFailOnly,
    HCDPRegNxbRsMpegRsFailOnly,
    HCDPRegNxbRsMpegKeepCrc,

    HCDPRegNxbRsMpegPrclkDly,
    
    HCDPRegNxbRsMpegPrclkOnMode,

    HCDPRegNxbRsDecErrCorDis,
    HCDPRegNxbRsDecFailDis,

    HCDPRegNxbRsMpegOosDis,

    HCDPRegNxbRsMpegFailThr,

    HCDPRegNxbRsMpegOosThr,

    HCDPRegNxbRsMpegSync,
    HCDPRegNxbRsMpegFullSync,
    HCDPRegNxbRsMpegCrcSync,
    HCDPRegNxbRsMpegOos,
    HCDPRegNxbRsMpegSyncLost,
    HCDPRegNxbRsMpegDsync,
    HCDPRegNxbRsMpegTsHeaderTPCheck,
    HCDPRegNxbRsMpegTsHeaderPUSCheck,
    HCDPRegNxbRsMpegTsHeaderTEICheck,
    HCDPRegNxbRsMpegTsHeaderTPInvPol,
    HCDPRegNxbRsMpegTsHeaderPUSInvPol,
    HCDPRegNxbRsMpegTsHeaderTEIInvPol,

    HCDPRegNxbRsMpegMemHistThrSel,
    HCDPRegNxbRsMpegLocalReset,
    HCDPRegNxbRsMpegResyncEn,
    HCDPRegNxbRsMpegTrkQualityThrSel,
    HCDPRegNxbRsMpegUseMem2Sync,
    HCDPRegNxbRsUseMemAlways,
    HCDPRegNxbRsMpegEnNormMem,
    HCDPRegNxbRsMpegFreezePtr,
    HCDPRegNxbRsMpegDfailCntMode,
    HCDPRegNxbRsMpegSwAltSyncPos,
    HCDPRegNxbRsMpegIgnorePid,
    HCDPRegNxbRsMpegOosResetEn,

    HCDPRegNxbTrManualSpecInv,
    HCDPRegNxbTrSpecInvState,

    //HCDPRegNxbTrTstMbistEn,
    //HCDPRegNxbTrTstMbistStart,
    //HCDPRegNxbTrTstZeroMem,
    //HCDPRegNxbTrTstMbistDone,
    //HCDPRegNxbTrTstMbistFail,
    //
    //HCDPRegNxbTrMbistFailAddr,
    //HCDPRegNxbTrMbistFailBit,
    //
    //HCDPRegNxbTrMbistMinAddr,
    //HCDPRegNxbTrMbistRetentionMsb,

    HCDPRegNxbTrSoftReset,
    HCDPRegNxbTrMicroReset,

    HCDPRegNxbTrTstZeroMetrics,
    HCDPRegNxbTrTstAdvTbMemDis,
    HCDPRegNxbTrTstAcsNoSat,

    HCDPRegNxbTrUseCombDout,
    HCDPRegNxbTrClrRegDout,

    HCDPRegNxbFsyncTstKCnt,
    HCDPRegNxbFsyncTstNCnt,

    HCDPRegNxbFsyncTstSmState,

    HCDPRegNxbFsyncTstSmFailCnt,
    HCDPRegNxbFsyncTstSmBreakCnt,

    HCDPRegNxbFsyncSoftReset,
    HCDPRegNxbFsyncMicroReset,

    HCDPRegNxbFsyncUseCombDout,
    HCDPRegNxbFsyncClrRegDout,

    //HCDPRegNxbDeintTstMbistEn,
    //HCDPRegNxbDeintTstMbistStart,
    //HCDPRegNxbDeintTstMbistZeroMem,
    //HCDPRegNxbDeintTstMbistRetentionMsb,

    //HCDPRegNxbDeintTstMbistFail,
    //HCDPRegNxbDeintTstMbistDone,
    //HCDPRegNxbDeintTstMbistInprogress,

    HCDPRegNxbDeintSoftReset,
    HCDPRegNxbDeintMicroReset,

    HCDPRegNxbDeintUseCombDout,
    HCDPRegNxbDeintClrRegDout,

    HCDPRegNxbRsMpegTrackQuality,
    HCDPRegNxbRsMpegDfailCnt,

    HCDPRegNxbRsMpegIntoTrkCnt,
    HCDPRegNxbRsMpegSyncTrkState,

    HCDPRegNxbRsMpegRsDoutFail,

    HCDPRegNxbRsMpegSoftReset,
    HCDPRegNxbRsMpegMicroReset,
    HCDPRegNxbRsMpegSoftSync,
    HCDPRegNxbRsMpegMicroSync,

    //HCDPRegNxbRsMbistEnable,
    //HCDPRegNxbRsMbistStart,
    //HCDPRegNxbRsMbistZeroMem,
    //HCDPRegNxbRsMbistDone,
    //HCDPRegNxbRsMbistFail,

    //HCDPRegNxbRsMbistFailAddr,
    //HCDPRegNxbRsMbistFailBit,

    //HCDPRegNxbRsMbistRetentionMsb,
    //HCDPRegNxbRsMbistMinAddr,

    //HCDPRegNxbCrcMbistEnable,
    //HCDPRegNxbCrcMbistStart,
    //HCDPRegNxbCrcMbistZeroMem,
    //HCDPRegNxbCrcMbistDone,
    //HCDPRegNxbCrcMbistFail,

    //HCDPRegNxbCrcMbistFailAddr,
    //HCDPRegNxbCrcMbistFailBit,

    //HCDPRegNxbCrcMbistRetentionMsb,
    //HCDPRegNxbCrcMbistMinAddr,
    //
    //HCDPRegNxbSyncMbistEnable,
    //HCDPRegNxbSyncMbistStart,
    //HCDPRegNxbSyncMbistZeroMem,
    //HCDPRegNxbSyncMbistDone,
    //HCDPRegNxbSyncMbistFail,

    //HCDPRegNxbSyncMbistFailAddr,
    //HCDPRegNxbSyncMbistFailBit,

    //HCDPRegNxbSyncMbistRetentionMsb,
    //HCDPRegNxbSyncMbistMinAddr,

    HCDPRegNxbRsMpegBestPtr,

    HCDPRegNxbRsMpegBestStatusPtr,
    HCDPRegNxbRsMpegBestStatusCnt,

    HCDPRegNxbRsMpegS2BestStatusPtr,
    HCDPRegNxbRsMpegS2BestStatusCnt,

    HCDPRegNxbRsMpegCounter,
    
    HCDPRegNxbRsMpegCrcFailCnt,
    HCDPRegNxbRsMpegDfailCnt4Test,

    HCDPRegNxbRsMpegClrStateCnt,

    HCDPRegNxbRsMpegUseCombDout,
    HCDPRegNxbRsMpegClrRegDout,

    HCDPRegNxaDerandBypass,

    HCDPRegNxaRsErrCorDis,
    HCDPRegNxaRsDecFailDis,

    HCDPRegNxaOosDis,
    HCDPRegNxaForceOos,

    HCDPRegNxaSyncWordThr,

    HCDPRegNxaDfailThr,

    HCDPRegNxaDeintBypass,

    HCDPRegNxaRsSync,

    HCDPRegNxaStatus, //the whole register of 0x110
    HCDPRegNxaRsReady,
    HCDPRegNxaSync,
    HCDPRegNxaDerandState,
    HCDPRegNxaDerandFailCnt,

    HCDPRegNxaUseNxaSync,

    HCDPRegNxaTstTpGenEnable,

    HCDPRegNxaTstClkDly,

    //HCDPRegNxaMbistEnable,
    //HCDPRegNxaMbistStart,
    //HCDPRegNxaMbistZeroMem,
    //HCDPRegNxaMbistMemSelect,

    //HCDPRegNxaMbistDone,
    //HCDPRegNxaMbistFail,

    //HCDPRegNxaMbistFailAddr,
    //HCDPRegNxaMbistFailBit,

    //HCDPRegNxaMbistMinAddr,
    //HCDPRegNxaMbistRetentionMsb,

    HCDPRegNxaSoftReset,
    HCDPRegNxaMicroReset,

    HCDPRegNxaUseCombDout,
    HCDPRegNxaClearRegDout,

    HCDPRegFFTScaleStage1,
    HCDPRegFFTScaleStage2,
    HCDPRegFFTScaleStage3,
    HCDPRegFFTScaleStage4,
    HCDPRegFFTScaleStage5,
    HCDPRegFFTScaleStage6,
    HCDPRegFFTInitOneTapFilters,
    HCDPRegFFTSymEnPhase,

    HCDPRegFFTSoftReset,
    HCDPRegFFTMicroReset,

    HCDPRegFFTReadAddress,

    HCDPRegFFTDataRead,
    HCDPRegFFTActualAddress,
    HCDPRegFFTDataReady,

    //HCDPRegFFTMbistEnable,
    //HCDPRegFFTMbistStart,
    //HCDPRegFFTMbistZeroMem,
    //HCDPRegFFTMbistDone,
    //HCDPRegFFTMbistFail,
    //HCDPRegFFTMbistMemSelect,

    //HCDPRegFFTMbistFailAddr,
    //HCDPRegFFTMbistFailBit,

    //HCDPRegFFTMbistMinAddr,
    //HCDPRegFFTMbistRetentionMsb,

    HCDPRegFFTUseCombDout,
    HCDPRegFFTClearRegDout,

    HCDPRegTimersResetPulse,
    HCDPRegTimersPausePulse,
    HCDPRegTimersResumePulse,

    HCDPRegTimersMode,
    HCDPRegTimersXmode,
    HCDPRegTimersSmode,
    HCDPRegTimersUseCombDout,
    HCDPRegTimersClrRegDout,

    HCDPRegTimersLockInd,
    HCDPRegTimersWrapInd,
    HCDPRegTimersBreakAfterLockInd,
    HCDPRegTimerslockAfterBreakInd,

    HCDPRegTimer0,
    
    HCDPRegTimer1,
    
    HCDPRegTimer2,
    
    HCDPRegTimer3,

    HCDPRegTimersTicDivider,

    HCDPRegAagcFreeze0,
    
    HCDPRegAagcEmThr0,
    
    HCDPRegAagcAcc0,

    HCDPRegAagcBw0,

    HCDPRegAagcSdPol0,

    HCDPRegAagcSdEn0,

    HCDPRegAagcSdRate0,

    HCDPRegAagcClr,

    HCDPRegAagcSigLevel,

    HCDPRegAagcSatLevel,

    HCDPRegAagcPreSatLevel,

    HCDPRegAagcSatEmThr,

    HCDPRegAagcPreSatEmThr,

    HCDPRegAagcFreeze1,

    HCDPRegAagcEmThLow1,
    
    HCDPRegAagcEmThHigh1,

    HCDPRegAagcAcc1,

    HCDPRegAagcBw1,

    HCDPRegAagcSdPol1,

    HCDPRegAagcSdEn1,

    HCDPRegAagcSdRate1,

    HCDPRegAagcEmThLow2,

    HCDPRegAagcEmThHigh2,

    HCDPRegAagcFreeze2,

    HCDPRegAagcAcc2,

    HCDPRegAagcBw2,

    HCDPRegAagcSdPol2,

    HCDPRegAagcSdEn2,

    HCDPRegAagcSdRate2,

    HCDPRegAagcLockVal,

    HCDPRegAagcLockThLow,

    HCDPRegAagcLockThHigh,

    HCDPRegAagcIfSdSw,
    HCDPRegAagcIfAttnSdSw,
    HCDPRegAagcRfSdSw,
    HCDPRegAagcSidewinder,

    HCDPRegAagcSdAdc,

    HCDPRegAagcSatPreSatSwap,

    HCDPRegFedrBypass,

    //HCDPRegDerotateFreq1Low,

    //HCDPRegDerotateFreq1High,

    HCDPRegDerotateFreqLow,

    HCDPRegDerotateFreqHigh,
    

    HCDPRegAdcFifoFreqAccu,


    HCDPRegAdcIfcZeroAdcData,

    HCDPRegAdcStaggeredSampling,

    HCDPRegAdcIfcFifoRdPtr,

    HCDPRegAdcIfcFifoRdErr,

    //HCDPRegAdcInScale,

    HCDPRegAdcOffsetBinaryIn,

    HCDPRegPdfBypass,

    HCDPRegPdfGain,

    HCDPRegDagcBypass,
    
    HCDPRegDagcDisable,

    HCDPRegDagcAcc,

    HCDPRegDagcOutGain,

    HCDPRegDagcBandwidth,

    HCDPRegDagcThreshold,

    HCDPRegDagcPosSatInd,

    HCDPRegMfBypass,

    HCDPRegMfCoeffSel,

    HCDPRegStlBlindTrMode,

    HCDPRegStlDecNoptMode,

    HCDPRegStlDisable,

    HCDPRegStlInMode,

    HCDPRegStlEmMode,

    HCDPRegStlDdEmGain,

    HCDPRegStlDaEmGain,

    HCDPRegStlAcqDirGain,
    HCDPRegStlAcqIndGain,

    HCDPRegStlTrkDirGain,
    HCDPRegStlTrkIndGain,

    HCDPRegStlOctaveScale,

    HCDPRegStlEmAutoSw,

    HCDPRegSymRateLow,

    HCDPRegSymRateHigh,

    HCDPRegStlAcc,

    HCDPRegStlClk2UtlPol,

    HCDPRegStlClk2UtlDiv2,

    HCDPRegCnstlSymDelay,

    HCDPRegStlZeroDirectPath,

    HCDPRegStlGadEmSel,

    HCDPRegStlGadEmGain,

    HCDPRegStlSweepStep,
    HCDPRegStlSweepStepTime,

    HCDPRegStlSweepMin,

    HCDPRegStlSweepMax,

    HCDPRegStlSweepTimer,

    HCDPRegStlLockThr,

    HCDPRegStlFadeThr,

    HCDPRegStlLockBW,

    HCDPRegStlLockAcc,

    HCDPRegStlLockPol,

    HCDPRegCMseI,

    HCDPRegCMseQ,

    HCDPRegCMseBW,

    HCDPRegCMseDecI,
    HCDPRegCMseCompI,
    HCDPRegCMseAbsI,
    HCDPRegCMseSignI,

    HCDPRegCMseDecQ,
    HCDPRegCMseCompQ,
    HCDPRegCMseAbsQ,
    HCDPRegCMseSignQ,

    HCDPRegUMseLow,
    HCDPRegUMseHigh,

    HCDPRegUMseBW,

    HCDPRegMseLockThr,
    HCDPRegMseFadeThr,

    HCDPRegMseSlicerSel,
    HCDPRegEnEqCt2Stl,
    HCDPRegEnBedr2Eq,

    HCDPRegCagcBypass,
    
    HCDPRegCagcDisable,
    
    HCDPRegCagcAcc,

    HCDPRegCagcOutGain,

    HCDPRegCagcBandwidth,

    HCDPRegCagcBlindThr,

    HCDPRegCagcEmAutoMode,

    HCDPRegCagcEmDecMode,

    HCDPRegCagcEmBlindMode,

    HCDPRegCagcSatResetEn,

    HCDPRegCtlDisable,

    HCDPRegCtlInMode,

    HCDPRegCtlEmMode,

    HCDPRegCtlDdEmGain,

    HCDPRegCtlDaEmGain,

    HCDPRegCtlAcqGain,
    HCDPRegCtlAcqDirGain,
    HCDPRegCtlAcqFractGain,
    HCDPRegCtlAcqIndGain,

    HCDPRegCtlTrkGain,
    HCDPRegCtlTrkDirGain,
    HCDPRegCtlTrkFractGain,
    HCDPRegCtlTrkIndGain,

    HCDPRegCtlOctaveScale,
    
    HCDPRegCtlEmAutoSw,

    HCDPRegCtlAcc,

    HCDPRegCtlZeroDirectPath,

    HCDPRegAfcInMode,

    HCDPRegAfcPhaseThr,

    HCDPRegAfcEnergyThr,

    HCDPRegAfcDisable,

    HCDPRegAfcForce,

    HCDPRegCtlPhaseEmSel,

    HCDPRegCtlFreqEmSel,

    HCDPRegAfcUseInnerCircle,

    HCDPRegAfcAccelerate,

    HCDPRegSctlDisable,

    HCDPRegSctlAcqGain,
    HCDPRegSctlAcqDirGain,
    HCDPRegSctlAcqFractGain,
    HCDPRegSctlAcqIndGain,

    HCDPRegSctlTrkGain,
    HCDPRegSctlTrkDirGain,
    HCDPRegSctlTrkFractGain,
    HCDPRegSctlTrkIndGain,

    HCDPRegSctlVcoBypass,
    
    HCDPRegSctlAcc,
    
    HCDPRegSctlZeroDirectPath,

    HCDPRegBedrBypass,

    HCDPRegBedrPhaseGain,

    HCDPRegBedrTrkOnly,

    HCDPRegSctlMagGain1,
    
    HCDPRegSctlMagGain2,
    
    HCDPRegCtlMagGain1,
    
    HCDPRegCtlMagGain2,

    HCDPRegSctlIncGain,

    HCDPRegSctlDecGain,

    HCDPRegCtlIncGain,

    HCDPRegCtlDecGain,

    HCDPRegSctlLockEmGain,

    HCDPRegCtlLockEmGain,

    HCDPRegSctlLockAcc,
    
    HCDPRegCtlLockAcc,
    
    HCDPRegSctlNearLockThr,
    
    HCDPRegSctlNearFadeThr,

    HCDPRegDemodSctlLockMask,
    HCDPRegDemodSctlNearLockMask,
    HCDPRegDemodCtlLockMask,
    HCDPRegDemodStlLockMask,
    HCDPRegDemodMseLockMask,
    HCDPRegDemodAagcLockMask,
    HCDPRegDemodDagcSatMask,
    HCDPRegDemodCagcSatMask,

    HCDPRegLockStatus,  /*16bits of  0x330*/
    HCDPRegLockInd,
    HCDPRegSctlLockInd,
    HCDPRegSctlNearLockInd,
    HCDPRegCtlLockInd,
    HCDPRegStlLockInd,
    HCDPRegMseLockInd,
    HCDPRegAagcLockFlag,
    HCDPRegDagcSatInd,
    HCDPRegCagcSatInd,
    HCDPRegLock2UtlInd,
    HCDPRegDemodLockInd,

    HCDPRegSctlLockMask,
    HCDPRegSctlNearLockMask,
    HCDPRegCtlLockMask,
    HCDPRegStlLockMask,    
    HCDPRegMseLockMask,
    HCDPRegAagcLockMask,
    HCDPRegDagcSatMask,
    HCDPRegCagcSatMask,

    HCDPRegForceLock,

    HCDPRegForceNoLock,

    HCDPRegSctlLockMask2Utl,
    HCDPRegSctlNearLockMask2Utl,
    HCDPRegStlLockMask2Utl,

    HCDPRegForceLock2Utl,
    HCDPRegForceNoLock2Utl,

    HCDPRegDemodVersion,

    HCDPRegConstellationIsReal,    

    HCDPRegConstellationData,

    HCDPRegConstellationCtrl,

    HCDPRegDemodOutRotate,

    HCDPRegDemodOutDifferential,

    HCDPRegDemodOutEuroMapping,

    HCDPRegDemodReset,
    HCDPRegDemodSoftReset,
    HCDPRegDemodMicroReset,
    HCDPRegDagcAccReset,
    HCDPRegCagcAccReset,
    HCDPRegStlAccReset,

    HCDPRegCtlAccReset,
    HCDPRegSctlAccReset,
    HCDPRegStlLockAccReset,
    HCDPRegCtlLockAccReset,
    HCDPRegSctlLockAccReset,
    HCDPRegMseLockReset,

    HCDPRegDemodAssociateDisable,

    HCDPRegEqBypass,

    HCDPRegEqDisable,

    HCDPRegEqReset,
    HCDPRegEqTspaceMode,
    HCDPRegEqFreezeMode,
    
    HCDPRegEqCenterTapGain,

    HCDPRegEqFbScale,

    HCDPRegEqEfScale,

    HCDPRegEqBlindThrA,

    HCDPRegEqBlindThrB,

    HCDPRegEqBlindThrC,

    HCDPRegEqBlindThrD,

    HCDPRegEqAcqAdaptMode,

    HCDPRegEqTrkAdaptMode,

    HCDPRegEqAdaptModeAutoSw,

    HCDPRegEqAdaptGainAutoSw,

    HCDPRegEqAcqFfAdaptGain,
    HCDPRegEqAcqFfHsymAdaptGain,
    HCDPRegEqAcqFbAdaptGain,
    HCDPRegEqAcqEfAdaptGain,

    HCDPRegEqTrkFfAdaptGain,
    HCDPRegEqTrkFfHsymAdaptGain,
    HCDPRegEqTrkFbAdaptGain,
    HCDPRegEqTrkEfAdaptGain,

    HCDPRegEqHsymLmsWeight,

    HCDPRegEqAcqHsymFreeze,

    HCDPRegEqBlindThrScma,

    HCDPRegEqFfLeakageEnable,

    HCDPRegEqFfLeakageAlpha,

    HCDPRegEqFbLeakageEnable,

    HCDPRegEqFbLeakageAlpha,

    HCDPRegEqEfDisable,

    HCDPRegEqEfLeakageEnable,

    HCDPRegEqEfLeakageAlpha,

    HCDPRegEqAdaptFreeze0,

    HCDPRegEqAdaptFreeze1,

    HCDPRegEqAdaptFreeze2,

    HCDPRegEqAdaptFreeze3,

    HCDPRegEqDfePwr,

    HCDPRegEqEfePwr,

    HCDPRegEqTapPowerStart,
    HCDPRegEqTapPowerReady,

    HCDPRegEqCoefNumber,
    HCDPRegEqCoefIsReal,
    HCDPRegEqCoefIsHigh,
    HCDPRegEqCoefWrValid,
    HCDPRegEqCoefRdValid,

    HCDPRegEqCoefData,

    HCDPRegClk16ChangeEn,
    HCDPRegClk16FindValueDisable,

    HCDPRegClk16Value,
    HCDPRegClk16ValueGain,

    HCDPRegClk16Phase,
    HCDPRegClk16Freq,
    HCDPRegClk16KickCount,

    HCDPRegDemodTestEn,

    HCDPRegAdcTestMode,

    HCDPRegDemodLogicCrcDone,

    HCDPRegDemodLogicCrcIData,
    HCDPRegDemodLogicCrcQData,

    HCDPRegCtlOpenLoop,
    HCDPRegStlOpenLoop,
    HCDPRegCagcOpenLoop,
    HCDPRegSctlOpenLoop,

    HCDPRegPdfSym2MinGap,
    HCDPRegPdfSym2MaxGap,

    HCDPRegDemodOutSpecInvState,
    HCDPRegDemodOutNxaSync,
    HCDPRegDemodOutLock,
    HCDPRegDemodOutManualSpecInv,

    //HCDPRegEqCmemMbistEnable,
    //HCDPRegEqCmemMbistStart,
    //HCDPRegEqCmemMbistMemSelect,

    //HCDPRegEqCmemMbistMinAddr,
    //HCDPRegEqCmemMbistRetentionMsb,
    //HCDPRegEqCmemMbistZeroMem,

    //HCDPRegEqCmemMbistDone,
    //HCDPRegEqCmemMbistFail,
    //HCDPRegEqCmemMbistFailAddr,
    //HCDPRegEqCmemMbistFailBit,

    //HCDPRegEqSmemMbistEnable,
    //HCDPRegEqSmemMbistStart,
    //HCDPRegEqSmemMbistMemSelect,

    //HCDPRegEqSmemMbistMinAddr,
    //HCDPRegEqSmemMbistRetentionMsb,
    //HCDPRegEqSmemMbistZeroMem,

    //HCDPRegEqSmemMbistDone,
    //HCDPRegEqSmemMbistFail,
    //HCDPRegEqSmemMbistFailAddr,
    //HCDPRegEqSmemMbistFailBit,

    HCDPRegDemodWriteError,
    HCDPRegDemodReadError

}HCDPRegisterIndex_t;

typedef struct                  /* REGISTER.  This struct will be used to control access to */
{                               /* register mapped items in the various demods */
  char    *uRegName;             /*   asciz name of register */
  HCDPRegisterIndex_t eID;     /*   register simplification ID */
  UInt16  uAddress;        /*   address from register map (hex) */
  UInt16  uMask;   /*the bits mask of the register*/
  UInt16  uShift;  /*bit shift count from the right */
}HCDPRegisterMap;

#define StrIp(s)       (s)             /* allow driver to contain register names */

static const HCDPRegisterMap sRegisterMap[] = 
{
   /* regname                                uID                                  uAddr   uMask   uShift */
    { StrIp("CdpVersion"),                   HCDPRegCdpVersion,                   0x0000, 0xFFFF, 0x0 }, /* Reg(0): CdpVersion bits [15 downto 0] */
    { StrIp("QamMode"),                      HCDPRegQamMode,                      0x0001, 0x000F, 0x0 }, /* Reg(1): QamMode bits [3 downto 0] */
    { StrIp("SpectralInversion"),            HCDPRegSpectralInversion,            0x0002, 0x0001, 0x0 }, /* Reg(2): SpectralInversion bit [0] */
    { StrIp("FecMode"),                      HCDPRegFecMode,                      0x0003, 0x0001, 0x0 }, /* Reg(3): FecMode bit [0] */
    { StrIp("NxbRsMpegSyncSel"),             HCDPRegNxbRsMpegSyncSel,             0x0003, 0x0002, 0x1 }, /* Reg(3): FecMode bit [1] */
    { StrIp("AdcClkPol"),                    HCDPRegAdcClkPol,                    0x0004, 0x0003, 0x0 }, /* Reg(4): AdcControl bits [1 downto 0] */
    { StrIp("ZeroAdcData"),                  HCDPRegZeroAdcData,                  0x0004, 0x0010, 0x4 }, /* Reg(4): AdcControl bit [4] */
    { StrIp("Adc2Comp"),                     HCDPRegAdc2Comp,                     0x0004, 0x0020, 0x5 }, /* Reg(4): AdcControl bit [5] */
    { StrIp("Adc4TestMode"),                 HCDPRegAdc4TestMode,                 0x0004, 0x0100, 0x8 }, /* Reg(4): AdcControl bit [8] */
    //{ StrIp("ClkGenDclkDivSel"),             HCDPRegClkGenDclkDivSel,             0x0005, 0x0003, 0x0 }, /* Reg(5): ClkGenCtrl bits [1 downto 0] */
    { StrIp("ClkGenShutDclk"),               HCDPRegClkGenShutDclk,               0x0005, 0x0010, 0x4 }, /* Reg(5): ClkGenCtrl bit [4] */
    { StrIp("ClkGenShutTclk"),               HCDPRegClkGenShutTclk,               0x0005, 0x0020, 0x5 }, /* Reg(5): ClkGenCtrl bit [5] */
    { StrIp("ClkGenShutNxa"),                HCDPRegClkGenShutNxa,                0x0005, 0x0040, 0x6 }, /* Reg(5): ClkGenCtrl bit [6] */
    { StrIp("ClkGenNxaClkFreeRun"),          HCDPRegClkGenNxaClkFreeRun,          0x0005, 0x0100, 0x8 }, /* Reg(5): ClkGenCtrl bit [8] */
    { StrIp("ClkGenRclkFreeRun"),            HCDPRegClkGenRclkFreeRun,            0x0005, 0x0200, 0x9 }, /* Reg(5): ClkGenCtrl bit [9] */
    { StrIp("ClkGenPrclkFreeRun"),           HCDPRegClkGenPrclkFreeRun,           0x0005, 0x0400, 0xA }, /* Reg(5): ClkGenCtrl bit [10] */
    { StrIp("ClkGenCsmSerClkFreeRun"),       HCDPRegClkGenCsmSerClkFreeRun,       0x0005, 0x0800, 0xB }, /* Reg(5): ClkGenCtrl bit [11] */
    { StrIp("ClkGenCsmSerClkShut"),          HCDPRegClkGenCsmSerClkShut,          0x0005, 0x1000, 0xC }, /* Reg(5): ClkGenCtrl bit [12] */
    { StrIp("ClkGenReset"),                  HCDPRegClkGenReset,                  0x0005, 0x8000, 0xF }, /* Reg(5): ClkGenCtrl bit [15] */
    //{ StrIp("ClkGenDclkPhase"),              HCDPRegClkGenDclkPhase,              0x0006, 0x0007, 0x0 }, /* Reg(6): ClkGenStatus bits [2 downto 0] */
    { StrIp("AdcFifoFreqWord"),              HCDPRegAdcFifoFreqWord,              0x0006, 0xffff, 0x0 }, /* Reg(6): AdcFifoFreqWord bits [15 downto 0] */
    { StrIp("CdpSoftRst"),                   HCDPRegCdpSoftRst,                   0x0007, 0x0001, 0x0 }, /* Reg(7): CdpReset bit [0] */
    { StrIp("CdpMicroRst"),                  HCDPRegCdpMicroRst,                  0x0007, 0x0002, 0x1 }, /* Reg(7): CdpReset bit [1] */
    { StrIp("CdpFullRst"),                   HCDPRegCdpFullRst,                   0x0007, 0x0004, 0x2 }, /* Reg(7): CdpReset bit [2] */
    { StrIp("DemodSoftRst"),                 HCDPRegDemodSoftRst,                 0x0007, 0x0010, 0x4 }, /* Reg(7): CdpReset bit [4] */
    { StrIp("DemodMicroRst"),                HCDPRegDemodMicroRst,                0x0007, 0x0020, 0x5 }, /* Reg(7): CdpReset bit [5] */
    { StrIp("FecSoftRst"),                   HCDPRegFecSoftRst,                   0x0007, 0x0100, 0x8 }, /* Reg(7): CdpReset bit [8] */
    { StrIp("FecMicroRst"),                  HCDPRegFecMicroRst,                  0x0007, 0x0200, 0x9 }, /* Reg(7): CdpReset bit [9] */
    { StrIp("CsmSoftRst"),                   HCDPRegCsmSoftRst,                   0x0007, 0x1000, 0xC }, /* Reg(7): CdpReset bit [12] */
    { StrIp("CsmMicroRst"),                  HCDPRegCsmMicroRst,                  0x0007, 0x2000, 0xD }, /* Reg(7): CdpReset bit [13] */
    { StrIp("MiMinWrWait"),                  HCDPRegMiMinWrWait,                  0x0008, 0x00FF, 0x0 }, /* Reg(8): MiWrWaitCtrl bits [7 downto 0] */
    { StrIp("MiMaxWrWait"),                  HCDPRegMiMaxWrWait,                  0x0008, 0xFF00, 0x8 }, /* Reg(8): MiWrWaitCtrl bits [15 downto 8] */
    { StrIp("MiMinRdWait"),                  HCDPRegMiMinRdWait,                  0x0009, 0x00FF, 0x0 }, /* Reg(9): MiRdWaitCtrl bits [7 downto 0] */
    { StrIp("MiMaxRdWait"),                  HCDPRegMiMaxRdWait,                  0x0009, 0xFF00, 0x8 }, /* Reg(9): MiRdWaitCtrl bits [15 downto 8] */
    { StrIp("DemodWrDisable"),               HCDPRegDemodWrDisable,               0x000A, 0x0001, 0x0 }, /* Reg(10): MiRdWrCtrl bit [0] */
    { StrIp("FecWrDisable"),                 HCDPRegFecWrDisable,                 0x000A, 0x0002, 0x1 }, /* Reg(10): MiRdWrCtrl bit [1] */
    { StrIp("TimeoutWrFlag"),                HCDPRegTimeoutWrFlag,                0x000A, 0x0010, 0x4 }, /* Reg(10): MiRdWrCtrl bit [4] */
    { StrIp("TimeoutRdFlag"),                HCDPRegTimeoutRdFlag,                0x000A, 0x0020, 0x5 }, /* Reg(10): MiRdWrCtrl bit [5] */
    { StrIp("AsxDackCtrl"),                  HCDPRegAsxDackCtrl,                  0x000B, 0x0001, 0x0 }, /* Reg(11): AsxDackCtrl bit [0] */
    { StrIp("GpioOut"),                      HCDPRegGpioOut,                      0x000C, 0x00FF, 0x0 }, /* Reg(12): GpioInOutl bits [7 downto 0] */
    { StrIp("GpioIn"),                       HCDPRegGpioIn,                       0x000C, 0xFF00, 0x8 }, /* Reg(12): GpioInOutl bits [15 downto 8] */
    { StrIp("SerTestDisable"),               HCDPRegSerTestDisable,               0x000D, 0x0001, 0x0 }, /* Reg(13): SerTestCtrl bit [0] */
    { StrIp("SerTestErr0"),                  HCDPRegSerTestErr0,                  0x000D, 0x0002, 0x1 }, /* Reg(13): SerTestCtrl bit [1] */
    { StrIp("SerGenCtrl"),                   HCDPRegSerGenCtrl,                   0x000E, 0xFFFF, 0x0 }, /* Reg(14): SerGenCtrl bits [15 downto 0] */
    { StrIp("SerGenStatus"),                 HCDPRegSerGenStatus,                 0x000F, 0xFFFF, 0x0 }, /* Reg(15): SerGenStatus bits [15 downto 0] */
    { StrIp("ClkSmtrSoftRst"),               HCDPRegClkSmtrSoftRst,               0x0010, 0x0001, 0x0 }, /* Reg(16): ClkSmtrReset bit [0] */
    { StrIp("ClkSmtrMicroRst"),              HCDPRegClkSmtrMicroRst,              0x0010, 0x0002, 0x1 }, /* Reg(16): ClkSmtrReset bit [1] */
    { StrIp("ClkSmtrSoftRstSync"),           HCDPRegClkSmtrSoftRstSync,           0x0010, 0x0004, 0x2 }, /* Reg(16): ClkSmtrReset bit [2] */
    { StrIp("ClkSmtrMicroRstSync"),          HCDPRegClkSmtrMicroRstSync,          0x0010, 0x0008, 0x3 }, /* Reg(16): ClkSmtrReset bit [3] */
    { StrIp("ClkSmtrSyncWordPuncMode"),      HCDPRegClkSmtrSyncWordPuncMode,      0x0011, 0x0001, 0x0 }, /* Reg(17): ClkSmtrOutCtrl bit [0] */
    { StrIp("ClkSmtrSyncWordOvrDis"),        HCDPRegClkSmtrSyncWordOvrDis,        0x0011, 0x0002, 0x1 }, /* Reg(17): ClkSmtrOutCtrl bit [1] */
    { StrIp("ClkSmtrTeiSetDis"),             HCDPRegClkSmtrTeiSetDis,             0x0011, 0x0004, 0x2 }, /* Reg(17): ClkSmtrOutCtrl bit [2] */
    { StrIp("ClkSmtrOutClkParPol"),          HCDPRegClkSmtrOutClkParPol,          0x0012, 0x0001, 0x0 }, /* Reg(18): ClkSmtrClkCtrl bit [0] */
    { StrIp("ClkSmtrOutClkSerPol"),          HCDPRegClkSmtrOutClkSerPol,          0x0012, 0x0002, 0x1 }, /* Reg(18): ClkSmtrClkCtrl bit [1] */
    { StrIp("ClkSmtrOutClkParSel"),          HCDPRegClkSmtrOutClkParSel,          0x0012, 0x0030, 0x4 }, /* Reg(18): ClkSmtrClkCtrl bits [5 downto 4] */
    { StrIp("ClkSmtrInClkPol"),              HCDPRegClkSmtrInClkPol,              0x0012, 0x0100, 0x8 }, /* Reg(18): ClkSmtrClkCtrl bit [8] */
    { StrIp("ClkSmtrDfailBeforeStart"),      HCDPRegClkSmtrDfailBeforeStart,      0x0013, 0x0003, 0x0 }, /* Reg(19): ClkSmtrInitialCtrl1 bits [1 downto 0] */
    { StrIp("ClkSmtrDfailBeforeSync"),       HCDPRegClkSmtrDfailBeforeSync,       0x0013, 0x000C, 0x2 }, /* Reg(19): ClkSmtrInitialCtrl1 bits [3 downto 2] */
    { StrIp("ClkSmtrDataBeforeStart"),       HCDPRegClkSmtrDataBeforeStart,       0x0013, 0x0030, 0x4 }, /* Reg(19): ClkSmtrInitialCtrl1 bits [5 downto 4] */
    { StrIp("ClkSmtrDataBeforeSync"),        HCDPRegClkSmtrDataBeforeSync,        0x0013, 0x00C0, 0x6 }, /* Reg(19): ClkSmtrInitialCtrl1 bits [7 downto 6] */
    { StrIp("ClkSmtrDataWhileDfail"),        HCDPRegClkSmtrDataWhileDfail,        0x0013, 0x0300, 0x8 }, /* Reg(19): ClkSmtrInitialCtrl1 bits [9 downto 8] */
    { StrIp("ClkSmtrOutClkAfterStart"),      HCDPRegClkSmtrOutClkAfterStart,      0x0014, 0x0001, 0x0 }, /* Reg(20): ClkSmtrInitialCtrl2 bit [0] */
    { StrIp("ClkSmtrOutClkAfterSync"),       HCDPRegClkSmtrOutClkAfterSync,       0x0014, 0x0002, 0x1 }, /* Reg(20): ClkSmtrInitialCtrl2 bit [1] */
    { StrIp("ClkSmtrOutClkStopEn"),          HCDPRegClkSmtrOutClkStopEn,          0x0014, 0x0004, 0x2 }, /* Reg(20): ClkSmtrInitialCtrl2 bit [2] */
    { StrIp("ClkSmtrValidAfterStart"),       HCDPRegClkSmtrValidAfterStart,       0x0014, 0x0010, 0x4 }, /* Reg(20): ClkSmtrInitialCtrl2 bit [4] */
    { StrIp("ClkSmtrValidAfterSync"),        HCDPRegClkSmtrValidAfterSync,        0x0014, 0x0020, 0x5 }, /* Reg(20): ClkSmtrInitialCtrl2 bit [5] */
    { StrIp("ClkSmtrValidStopEn"),           HCDPRegClkSmtrValidStopEn,           0x0014, 0x0040, 0x6 }, /* Reg(20): ClkSmtrInitialCtrl2 bit [6] */
    { StrIp("ClkSmtrUseDclkDsync"),          HCDPRegClkSmtrUseDclkDsync,          0x0014, 0x0080, 0x7 }, /* Reg(20): ClkSmtrInitialCtrl2 bit [7] */
    { StrIp("ClkSmtrFifoAutoMode"),          HCDPRegClkSmtrFifoAutoMode,          0x0015, 0x003F, 0x0 }, /* Reg(21): ClkSmtrFifoCtrl bits [5 downto 0] */
    { StrIp("ClkSmtrDdaStepSel"),            HCDPRegClkSmtrDdaStepSel,            0x0015, 0x0700, 0x8 }, /* Reg(21): ClkSmtrFifoCtrl bits [10 downto 8] */
    { StrIp("ClkSmtrManualModulo"),          HCDPRegClkSmtrManualModulo,          0x0016, 0xFFFF, 0x0 }, /* Reg(22): ClkSmtrManualModulo bits [15 downto 0] */
    { StrIp("ClkSmtrAutoModulo"),            HCDPRegClkSmtrAutoModulo,            0x0017, 0xFFFF, 0x0 }, /* Reg(23): ClkSmtrAutoModulo bits [15 downto 0] */
    { StrIp("ClkSmtrManualNumerator"),       HCDPRegClkSmtrManualNumerator,       0x0018, 0xFFFF, 0x0 }, /* Reg(24): ClkSmtrManualNumerator bits [15 downto 0] */
    { StrIp("ClkSmtrAutoNumerator"),         HCDPRegClkSmtrAutoNumerator,         0x0019, 0xFFFF, 0x0 }, /* Reg(25): ClkSmtrAutoNumerator bits [15 downto 0] */
    { StrIp("ClkSmtrFifoEmptyCnt"),          HCDPRegClkSmtrFifoEmptyCnt,          0x001D, 0x000F, 0x0 }, /* Reg(29): ClkSmtrFifoStatusCnt bits [3 downto 0] */
    { StrIp("ClkSmtrFifoFullCnt"),           HCDPRegClkSmtrFifoFullCnt,           0x001D, 0x00F0, 0x4 }, /* Reg(29): ClkSmtrFifoStatusCnt bits [7 downto 4] */
    { StrIp("ClkSmtrFifoMinCapacity"),       HCDPRegClkSmtrFifoMinCapacity,       0x001E, 0x001F, 0x0 }, /* Reg(30): ClkSmtrFifoCapacity bits [4 downto 0] */
    { StrIp("ClkSmtrFifoMaxCapacity"),       HCDPRegClkSmtrFifoMaxCapacity,       0x001E, 0x1F00, 0x8 }, /* Reg(30): ClkSmtrFifoCapacity bits [12 downto 8] */
    { StrIp("ClkSmtrFifoKickDownCnt"),       HCDPRegClkSmtrFifoKickDownCnt,       0x001F, 0x00FF, 0x0 }, /* Reg(31): ClkSmtrFifoKickCnt bits [7 downto 0] */
    { StrIp("ClkSmtrFifoKickUpCnt"),         HCDPRegClkSmtrFifoKickUpCnt,         0x001F, 0xFE00, 0x9 }, /* Reg(31): ClkSmtrFifoKickCnt bits [15 downto 9] */
    { StrIp("PnBerMode"),                    HCDPRegPnBerMode,                    0x0030, 0x0007, 0x0 }, /* Reg(48): PnBerModeCtrl bits [2 downto 0] */
    { StrIp("PnBerDemodMode"),               HCDPRegPnBerDemodMode,               0x0030, 0x0008, 0x3 }, /* Reg(48): PnBerModeCtrl bit [3] */
    { StrIp("PnBerWinSel"),                  HCDPRegPnBerWinSel,                  0x0030, 0x00F0, 0x4 }, /* Reg(48): PnBerModeCtrl bits [7 downto 4] */
    { StrIp("PnBerExtWinSel"),               HCDPRegPnBerExtWinSel,               0x0030, 0x0300, 0x8 }, /* Reg(48): PnBerModeCtrl bits [9 downto 8] */
    { StrIp("PnBerInfiniteMode"),            HCDPRegPnBerInfiniteMode,            0x0030, 0x1000, 0xC }, /* Reg(48): PnBerModeCtrl bit [12] */
    { StrIp("PnBerScaleResult"),             HCDPRegPnBerScaleResult,             0x0030, 0x2000, 0xD }, /* Reg(48): PnBerModeCtrl bit [13] */
    { StrIp("PnBerHeaderMode"),              HCDPRegPnBerHeaderMode,              0x0030, 0x4000, 0xE }, /* Reg(48): PnBerModeCtrl bit [14] */
    { StrIp("PnBerBreakSel"),                HCDPRegPnBerBreakSel,                0x0031, 0x0003, 0x0 }, /* Reg(49): PnBerBreakCtrl bits [1 downto 0] */
    { StrIp("PnBerBreakEn"),                 HCDPRegPnBerBreakEn,                 0x0031, 0x0010, 0x4 }, /* Reg(49): PnBerBreakCtrl bit [4] */
    { StrIp("PnBerStart"),                   HCDPRegPnBerStart,                   0x0032, 0x0001, 0x0 }, /* Reg(50): PnBerStartCtrl bit [0] */
    { StrIp("PnBerRestart"),                 HCDPRegPnBerRestart,                 0x0032, 0x0002, 0x1 }, /* Reg(50): PnBerStartCtrl bit [1] */
    { StrIp("PnBerReset"),                   HCDPRegPnBerReset,                   0x0032, 0x0010, 0x4 }, /* Reg(50): PnBerStartCtrl bit [4] */
    { StrIp("PnBerIdle"),                    HCDPRegPnBerIdle,                    0x0033, 0x0001, 0x0 }, /* Reg(51): PnBerStatus bit [0] */
    { StrIp("PnBerLock"),                    HCDPRegPnBerLock,                    0x0033, 0x0002, 0x1 }, /* Reg(51): PnBerStatus bit [1] */
    { StrIp("PnBerSeq"),                     HCDPRegPnBerSeq,                     0x0033, 0x0004, 0x2 }, /* Reg(51): PnBerStatus bit [2] */
    { StrIp("PnBerPol"),                     HCDPRegPnBerPol,                     0x0033, 0x0008, 0x3 }, /* Reg(51): PnBerStatus bit [3] */
    { StrIp("PnBerReadyCount"),              HCDPRegPnBerReadyCount,              0x0034, 0x000F, 0x0 }, /* Reg(52): PnBerReadyCount bits [3 downto 0] */
    { StrIp("PnBerResult"),                  HCDPRegPnBerResult,                  0x0035, 0xFFFF, 0x0 }, /* Reg(53): PnBerResult bits [15 downto 0] */
    { StrIp("PnBerBreakCount"),              HCDPRegPnBerBreakCount,              0x0036, 0x000F, 0x0 }, /* Reg(54): PnBerErrorStatus bits [3 downto 0] */
    { StrIp("PnBerViolationCount"),          HCDPRegPnBerViolationCount,          0x0036, 0x00F0, 0x4 }, /* Reg(54): PnBerErrorStatus bits [7 downto 4] */
    { StrIp("PnBerErrorCount"),              HCDPRegPnBerErrorCount,              0x0036, 0xFF00, 0x8 }, /* Reg(54): PnBerErrorStatus bits [15 downto 8] */
    { StrIp("CheckCrcMode"),                 HCDPRegCheckCrcMode,                 0x0037, 0x0003, 0x0 }, /* Reg(55): CheckCrcMode bits [1 downto 0] */
    { StrIp("CheckCrcValue"),                HCDPRegCheckCrcValue,                0x0038, 0x0001, 0x0 }, /* Reg(56): CheckCrcValue bit [0] */
    { StrIp("CheckCrcResult"),               HCDPRegCheckCrcResult,               0x0039, 0x0001, 0x0 }, /* Reg(57): CheckCrcResult bit [0] */
    { StrIp("CheckPidMode"),                 HCDPRegCheckPidMode,                 0x003A, 0x0003, 0x0 }, /* Reg(58): CheckPidMode bits [1 downto 0] */
    { StrIp("CheckPidValue"),                HCDPRegCheckPidValue,                0x003B, 0x1FFF, 0x0 }, /* Reg(59): CheckPidValue bits [12 downto 0] */
    { StrIp("CheckPidResult"),               HCDPRegCheckPidResult,               0x003C, 0x1FFF, 0x0 }, /* Reg(60): CheckPidResult bits [12 downto 0] */
    { StrIp("CheckCcErrorCnt"),              HCDPRegCheckCcErrorCnt,              0x003D, 0x00FF, 0x0 }, /* Reg(61): CheckCcStatus bits [7 downto 0] */
    { StrIp("CheckCcResult"),                HCDPRegCheckCcResult,                0x003D, 0x0F00, 0x8 }, /* Reg(61): CheckCcStatus bits [11 downto 8] */
    { StrIp("ClkSmtrUseCombDout"),           HCDPRegClkSmtrUseCombDout,           0x003F, 0x0001, 0x0 }, /* Reg(63): ClkSmtrDoutCtrl bit [0] */
    { StrIp("ClkSmtrClrRegDout"),            HCDPRegClkSmtrClrRegDout,            0x003F, 0x0002, 0x1 }, /* Reg(63): ClkSmtrDoutCtrl bit [1] */
    { StrIp("RsDecodWinMode"),               HCDPRegRsDecodWinMode,               0x004F, 0x0007, 0x0 }, /* Reg(79): RsDecodMode bits [2 downto 0] */
    { StrIp("RsDecodCsmMode"),               HCDPRegRsDecodCsmMode,               0x004F, 0x0010, 0x4 }, /* Reg(79): RsDecodMode bit [4] */
    { StrIp("RsDecodClear"),                 HCDPRegRsDecodClear,                 0x0050, 0x0001, 0x0 }, /* Reg(80): RsDecodCtrl bit [0] */
    { StrIp("RsDecodFreeze"),                HCDPRegRsDecodFreeze,                0x0050, 0x0002, 0x1 }, /* Reg(80): RsDecodCtrl bit [1] */
    { StrIp("RsDecodUncorrectableCwHigh"),   HCDPRegRsDecodUncorrectableCwHigh,   0x0051, 0xFFFF, 0x0 }, /* Reg(81): RsDecodUnerroredCwHigh bits [15 downto 0] */
    { StrIp("DecodUncorrectableCwLow"),      HCDPRegRsDecodUncorrectableCwLow,    0x0052, 0xFFFF, 0x0 }, /* Reg(82): RsDecodUnerroredCwLow bits [15 downto 0] */
    { StrIp("RsDecodSymErrsHigh"),           HCDPRegRsDecodSymErrsHigh,           0x0053, 0xFFFF, 0x0 }, /* Reg(83): RsDecodSymErrsHigh bits [15 downto 0] */
    { StrIp("RsDecodSymErrsLow"),            HCDPRegRsDecodSymErrsLow,            0x0054, 0xFFFF, 0x0 }, /* Reg(84): RsDecodSymErrsLow bits [15 downto 0] */
    { StrIp("RsDecodBitErrsHigh"),           HCDPRegRsDecodBitErrsHigh,           0x0055, 0xFFFF, 0x0 }, /* Reg(85): RsDecodBitErrsHigh bits [15 downto 0] */
    { StrIp("RsDecodBitErrsLow"),            HCDPRegRsDecodBitErrsLow,            0x0056, 0xFFFF, 0x0 }, /* Reg(86): RsDecodBitErrsLow bits [15 downto 0] */
    { StrIp("RsDecodUnerroredCwHigh"),       HCDPRegRsDecodUnerroredCwHigh,       0x0057, 0xFFFF, 0x0 }, /* Reg(87): RsDecodUnerroredCwHigh bits [15 downto 0] */
    { StrIp("RsDecodUnerroredCwLow"),        HCDPRegRsDecodUnerroredCwLow,        0x0058, 0xFFFF, 0x0 }, /* Reg(88): RsDecodUnerroredCwLow bits [15 downto 0] */
    { StrIp("RsDecodCorrectedCwHigh"),       HCDPRegRsDecodCorrectedCwHigh,       0x0059, 0xFFFF, 0x0 }, /* Reg(89): RsDecodCorrectedCwHigh bits [15 downto 0] */
    { StrIp("RsDecodCorrectedCwLow"),        HCDPRegRsDecodCorrectedCwLow,        0x005A, 0xFFFF, 0x0 }, /* Reg(90): RsDecodCorrectedCwLow bits [15 downto 0] */
    { StrIp("RsDecodUseCombDout"),           HCDPRegRsDecodUseCombDout,           0x005B, 0x0001, 0x0 }, /* Reg(91): RsDecodDoutCtrl bit [0] */
    { StrIp("RsDecodClrRegDout"),            HCDPRegRsDecodClrRegDout,            0x005B, 0x0002, 0x1 }, /* Reg(91): RsDecodDoutCtrl bit [1] */
    { StrIp("RsDecodCodewordCount"),         HCDPRegRsDecodCodewordCount,         0x005C, 0x00FF, 0x0 }, /* Reg(92): RsDecodCount bits [7 downto 0] */
    { StrIp("RsDecodWindowCount"),           HCDPRegRsDecodWindowCount,           0x005C, 0xFF00, 0x8 }, /* Reg(92): RsDecodCount bits [15 downto 8] */
    { StrIp("NxbTrNormWindow"),              HCDPRegNxbTrNormWindow,              0x0060, 0x00FF, 0x0 }, /* Reg(96): NxbTrNormWindow bits [7 downto 0] */
    { StrIp("NxbTrNormThr"),                 HCDPRegNxbTrNormThr,                 0x0061, 0x00FF, 0x0 }, /* Reg(97): NxbTrNormThr bits [7 downto 0] */
    { StrIp("NxbTrNormCountI"),              HCDPRegNxbTrNormCountI,              0x0062, 0x00FF, 0x0 }, /* Reg(98): NxbTrNormCount bits [7 downto 0] */
    { StrIp("NxbTrNormCountQ"),              HCDPRegNxbTrNormCountQ,              0x0062, 0xFF00, 0x8 }, /* Reg(98): NxbTrNormCount bits [15 downto 8] */
    { StrIp("NxbTrSync"),                    HCDPRegNxbTrSync,                    0x0063, 0x0001, 0x0 }, /* Reg(99): NxbTrSync bit [0] */
    { StrIp("NxbTrManualSync"),              HCDPRegNxbTrManualSync,              0x0064, 0x0001, 0x0 }, /* Reg(100): NxbTrManualSyncCtrl bit [0] */
    { StrIp("NxbTrAdvanceSync"),             HCDPRegNxbTrAdvanceSync,             0x0064, 0x0002, 0x1 }, /* Reg(100): NxbTrManualSyncCtrl bit [1] */
    { StrIp("NxbTrNormCntSel"),              HCDPRegNxbTrNormCntSel,              0x0065, 0x0003, 0x0 }, /* Reg(101): NxbTrNormCntSel bits [1 downto 0] */
    { StrIp("NxbTrTclkDly"),                 HCDPRegNxbTrTclkDly,                 0x0065, 0x0010, 0x4 }, /* Reg(101): NxbTrNormCntSel bit [4] */
    { StrIp("NxbTrNormCntReadyI"),           HCDPRegNxbTrNormCntReadyI,           0x0066, 0x00FF, 0x0 }, /* Reg(102): NxbTrNormCntReady bits [7 downto 0] */
    { StrIp("NxbTrNormCntReadyQ"),           HCDPRegNxbTrNormCntReadyQ,           0x0066, 0xFF00, 0x8 }, /* Reg(102): NxbTrNormCntReady bits [15 downto 8] */
    { StrIp("NxbTrSkipCnt"),                 HCDPRegNxbTrSkipCnt,                 0x0067, 0x0FFF, 0x0 }, /* Reg(103): NxbTrSkipCnt bits [11 downto 0] */
    { StrIp("NxbTrSkipCntClr"),              HCDPRegNxbTrSkipCntClr,              0x0068, 0x0001, 0x0 }, /* Reg(104): NxbTrSkipCntClr bit [0] */
    { StrIp("NxbTrNormCntIpeak"),            HCDPRegNxbTrNormCntIpeak,            0x0069, 0x00FF, 0x0 }, /* Reg(105): NxbTrNormCntPeak bits [7 downto 0] */
    { StrIp("NxbTrNormCntQpeak"),            HCDPRegNxbTrNormCntQpeak,            0x0069, 0xFF00, 0x8 }, /* Reg(105): NxbTrNormCntPeak bits [15 downto 8] */
    { StrIp("NxbTrNormPeakClr"),             HCDPRegNxbTrNormPeakClr,             0x006A, 0x0001, 0x0 }, /* Reg(106): NxbTrNormPeakClr bit [0] */
    { StrIp("NxbTrRclkGapCnt"),              HCDPRegNxbTrRclkGapCnt,              0x006B, 0x00FF, 0x0 }, /* Reg(107): NxbTrTstStatus bits [7 downto 0] */
    { StrIp("NxbTrSymWidthCnt"),             HCDPRegNxbTrSymWidthCnt,             0x006B, 0xFF00, 0x8 }, /* Reg(107): NxbTrTstStatus bits [15 downto 8] */
    { StrIp("NxbTrRclkGapClr"),              HCDPRegNxbTrRclkGapClr,              0x006C, 0x0001, 0x0 }, /* Reg(108): NxbTrRclkGapClr bit [0] */
    { StrIp("NxbFsyncIntoSyncThr"),          HCDPRegNxbFsyncIntoSyncThr,          0x0070, 0x000F, 0x0 }, /* Reg(112): NxbFsyncIntoSyncThr bits [3 downto 0] */
    { StrIp("NxbFsyncIntoSyncWin"),          HCDPRegNxbFsyncIntoSyncWin,          0x0071, 0x000F, 0x0 }, /* Reg(113): NxbFsyncIntoSyncWin bits [3 downto 0] */
    { StrIp("NxbFsyncOosThr"),               HCDPRegNxbFsyncOosThr,               0x0072, 0x000F, 0x0 }, /* Reg(114): NxbFsyncOosThr bits [3 downto 0] */
    { StrIp("NxbFsyncOosWin"),               HCDPRegNxbFsyncOosWin,               0x0073, 0x000F, 0x0 }, /* Reg(115): NxbFsyncOosWin bits [3 downto 0] */
    { StrIp("NxbFsyncRsOosDis"),             HCDPRegNxbFsyncRsOosDis,             0x0074, 0x0001, 0x0 }, /* Reg(116): NxbFsyncOosDis bit [0] */
    { StrIp("NxbFsyncMajOosDis"),            HCDPRegNxbFsyncMajOosDis,            0x0074, 0x0002, 0x1 }, /* Reg(116): NxbFsyncOosDis bit [1] */
    { StrIp("NxbFsyncSmOosDis"),             HCDPRegNxbFsyncSmOosDis,             0x0074, 0x0010, 0x4 }, /* Reg(116): NxbFsyncOosDis bit [4] */
    { StrIp("NxbFsyncSmCntClr"),             HCDPRegNxbFsyncSmCntClr,             0x0075, 0x0001, 0x0 }, /* Reg(117): NxbFsyncSmCntClr bit [0] */
    { StrIp("NxbFsyncInSync"),               HCDPRegNxbFsyncInSync,               0x0076, 0x0001, 0x0 }, /* Reg(118): NxbFsyncStatus1 bit [0] */
    { StrIp("NxbFsyncTimingSync"),           HCDPRegNxbFsyncTimingSync,           0x0076, 0x0002, 0x1 }, /* Reg(118): NxbFsyncStatus1 bit [1] */
    { StrIp("NxbFsyncSearch"),               HCDPRegNxbFsyncSearch,               0x0076, 0x0004, 0x2 }, /* Reg(118): NxbFsyncStatus1 bit [2] */
    { StrIp("NxbFsyncChangeState"),          HCDPRegNxbFsyncChangeState,          0x0076, 0x0008, 0x3 }, /* Reg(118): NxbFsyncStatus1 bit [3] */
    { StrIp("NxbFsyncActual_IJ_Value"),      HCDPRegNxbFsyncActual_IJ_Value,      0x0076, 0x00F0, 0x4 }, /* Reg(118): NxbFsyncStatus1 bits [7 downto 4] */
    { StrIp("NxbFsyncActual_IJ_Quality"),    HCDPRegNxbFsyncActual_IJ_Quality,    0x0076, 0x0700, 0x8 }, /* Reg(118): NxbFsyncStatus1 bits [10 downto 8] */
    { StrIp("NxbFsync_IJ_ChangeCount"),      HCDPRegNxbFsync_IJ_ChangeCount,      0x0076, 0x7000, 0xC }, /* Reg(118): NxbFsyncStatus1 bits [14 downto 12] */
    { StrIp("NxbFsyncCurr_IJ_Value"),        HCDPRegNxbFsyncCurr_IJ_Value,        0x0077, 0x000F, 0x0 }, /* Reg(119): NxbFsyncStatus2 bits [3 downto 0] */
    { StrIp("NxbFsyncMaj_IJ_Value"),         HCDPRegNxbFsyncMaj_IJ_Value,         0x0077, 0x00F0, 0x4 }, /* Reg(119): NxbFsyncStatus2 bits [7 downto 4] */
    { StrIp("NxbFsyncMaj_IJ_Quality"),       HCDPRegNxbFsyncMaj_IJ_Quality,       0x0077, 0x0700, 0x8 }, /* Reg(119): NxbFsyncStatus2 bits [10 downto 8] */
    { StrIp("NxbFsyncManual_IJ_Value"),      HCDPRegNxbFsyncManual_IJ_Value,      0x0078, 0x000F, 0x0 }, /* Reg(120): NxbFsyncDeintModeSel bits [3 downto 0] */
    { StrIp("NxbFsyncManualDeintMode"),      HCDPRegNxbFsyncManualDeintMode,      0x0078, 0x0010, 0x4 }, /* Reg(120): NxbFsyncDeintModeSel bit [4] */
    { StrIp("NxbFsyncIJ_ModifyDis"),         HCDPRegNxbFsyncIJ_ModifyDis,         0x0078, 0x0020, 0x5 }, /* Reg(120): NxbFsyncDeintModeSel bit [5] */
    { StrIp("NxbFsyncMajThr"),               HCDPRegNxbFsyncMajThr,               0x0079, 0x0007, 0x0 }, /* Reg(121): NxbDeintMajThr bits [2 downto 0] */
    { StrIp("NxbFsyncRclkDly"),              HCDPRegNxbFsyncRclkDly,              0x007A, 0x0001, 0x0 }, /* Reg(122): NxbFsyncRclkDly bit [0] */
    { StrIp("NxbDeintBypass"),               HCDPRegNxbDeintBypass,               0x0080, 0x0001, 0x0 }, /* Reg(128): NxbDeintBypass bit [0] */
    { StrIp("NxbDeintEnhancedDis"),          HCDPRegNxbDeintEnhancedDis,          0x0081, 0x0001, 0x0 }, /* Reg(129): NxbDeintEnhancedDis bit [0] */
    { StrIp("NxbDerandBypass"),              HCDPRegNxbDerandBypass,              0x0082, 0x0001, 0x0 }, /* Reg(130): NxbDerandBypass bit [0] */
    { StrIp("NxbDeintActualMode"),           HCDPRegNxbDeintActualMode,           0x0083, 0x000F, 0x0 }, /* Reg(131): NxbDeintActualMode bits [3 downto 0] */
    { StrIp("NxbRsMpegCrcFailOnly"),         HCDPRegNxbRsMpegCrcFailOnly,         0x0090, 0x0001, 0x0 }, /* Reg(144): NxbRsMpegFailCtrl bit [0] */
    { StrIp("NxbRsMpegRsFailOnly"),          HCDPRegNxbRsMpegRsFailOnly,          0x0090, 0x0002, 0x1 }, /* Reg(144): NxbRsMpegFailCtrl bit [1] */
    { StrIp("NxbRsMpegKeepCrc"),             HCDPRegNxbRsMpegKeepCrc,             0x0090, 0x0004, 0x2 }, /* Reg(144): NxbRsMpegFailCtrl bit [2] */
    { StrIp("NxbRsMpegPrclkDly"),            HCDPRegNxbRsMpegPrclkDly,            0x0091, 0x0001, 0x0 }, /* Reg(145): NxbRsMpegPrclkDly bit [0] */
    { StrIp("NxbRsMpegPrclkOnMode"),         HCDPRegNxbRsMpegPrclkOnMode,         0x0092, 0x0001, 0x0 }, /* Reg(146): NxbRsMpegPrclkOnMode bit [0] */
    { StrIp("NxbRsDecErrCorDis"),            HCDPRegNxbRsDecErrCorDis,            0x0093, 0x0001, 0x0 }, /* Reg(147): NxbRsDecErrCorCtrl bit [0] */
    { StrIp("NxbRsDecFailDis"),              HCDPRegNxbRsDecFailDis,              0x0093, 0x0002, 0x1 }, /* Reg(147): NxbRsDecErrCorCtrl bit [1] */
    { StrIp("NxbRsMpegOosDis"),              HCDPRegNxbRsMpegOosDis,              0x0094, 0x0001, 0x0 }, /* Reg(148): NxbRsMpegOosDis bit [0] */
    { StrIp("NxbRsMpegFailThr"),             HCDPRegNxbRsMpegFailThr,             0x009C, 0x000F, 0x0 }, /* Reg(156): NxbRsMpegFailThr bits [3 downto 0] */
    { StrIp("NxbRsMpegOosThr"),              HCDPRegNxbRsMpegOosThr,              0x009D, 0x000F, 0x0 }, /* Reg(157): NxbRsMpegOosThr bits [3 downto 0] */
    { StrIp("NxbRsMpegSync"),                HCDPRegNxbRsMpegSync,                0x00A0, 0xFFFF, 0x0 }, /* Reg(160): NxbRsMpegSync bit [15 downto 0] */
    { StrIp("NxbRsMpegFullSync"),            HCDPRegNxbRsMpegFullSync,            0x00A0, 0x0001, 0x0 }, /* Reg(160): NxbRsMpegSync bit [0] */
    { StrIp("NxbRsMpegCrcSync"),             HCDPRegNxbRsMpegCrcSync,             0x00A0, 0x0002, 0x1 }, /* Reg(160): NxbRsMpegSync bit [1] */
    { StrIp("NxbRsMpegOos"),                 HCDPRegNxbRsMpegOos,                 0x00A0, 0x0004, 0x2 }, /* Reg(160): NxbRsMpegSync bit [2] */
    { StrIp("NxbRsMpegSyncLost"),            HCDPRegNxbRsMpegSyncLost,            0x00A0, 0x0008, 0x3 }, /* Reg(160): NxbRsMpegSync bit [3] */
    { StrIp("NxbRsMpegDsync"),               HCDPRegNxbRsMpegDsync,               0x00A0, 0x0010, 0x4 }, /* Reg(160): NxbRsMpegSync bit [4] */
    { StrIp("NxbRsMpegTsHeaderTPCheck"),     HCDPRegNxbRsMpegTsHeaderTPCheck,     0x00A0, 0x0080, 0x7 }, /* Reg(160): NxbRsMpegSync bit [7] */
    { StrIp("NxbRsMpegTsHeaderPUSCheck"),    HCDPRegNxbRsMpegTsHeaderPUSCheck,    0x00A0, 0x0100, 0x8 }, /* Reg(160): NxbRsMpegSync bit [8] */
    { StrIp("NxbRsMpegTsHeaderTEICheck"),    HCDPRegNxbRsMpegTsHeaderTEICheck,    0x00A0, 0x0200, 0x9 }, /* Reg(160): NxbRsMpegSync bit [9] */
    { StrIp("NxbRsMpegTsHeaderTPInvPol"),    HCDPRegNxbRsMpegTsHeaderTPInvPol,    0x00A0, 0x0400, 0xA }, /* Reg(160): NxbRsMpegSync bit [10] */
    { StrIp("NxbRsMpegTsHeaderPUSInvPol"),   HCDPRegNxbRsMpegTsHeaderPUSInvPol,   0x00A0, 0x0800, 0xB }, /* Reg(160): NxbRsMpegSync bit [11] */
    { StrIp("NxbRsMpegTsHeaderTEIInvPol"),   HCDPRegNxbRsMpegTsHeaderTEIInvPol,   0x00A0, 0x1000, 0xC }, /* Reg(160): NxbRsMpegSync bit [12] */
    { StrIp("NxbRsMpegMemHistThrSel"),       HCDPRegNxbRsMpegMemHistThrSel,       0x00A6, 0x0003, 0x0 }, /* Reg(166): NxbRsMpegSyncCtrl bits [1 downto 0] */
    { StrIp("NxbRsMpegLocalReset"),          HCDPRegNxbRsMpegLocalReset,          0x00A6, 0x0004, 0x2 }, /* Reg(166): NxbRsMpegSyncCtrl bit [2] */
    { StrIp("NxbRsMpegResyncEn"),            HCDPRegNxbRsMpegResyncEn,            0x00A6, 0x0008, 0x3 }, /* Reg(166): NxbRsMpegSyncCtrl bit [3] */
    { StrIp("NxbRsMpegTrkQualityThrSel"),    HCDPRegNxbRsMpegTrkQualityThrSel,    0x00A6, 0x00F0, 0x4 }, /* Reg(166): NxbRsMpegSyncCtrl bits [7 downto 4] */
    { StrIp("NxbRsMpegUseMem2Sync"),         HCDPRegNxbRsMpegUseMem2Sync,         0x00A6, 0x0100, 0x8 }, /* Reg(166): NxbRsMpegSyncCtrl bit [8] */
    { StrIp("NxbRsUseMemAlways"),            HCDPRegNxbRsUseMemAlways,            0x00A6, 0x0200, 0x9 }, /* Reg(166): NxbRsMpegSyncCtrl bit [9] */
    { StrIp("NxbRsMpegEnNormMem"),           HCDPRegNxbRsMpegEnNormMem,           0x00A6, 0x0400, 0xA }, /* Reg(166): NxbRsMpegSyncCtrl bit [10] */
    { StrIp("NxbRsMpegFreezePtr"),           HCDPRegNxbRsMpegFreezePtr,           0x00A6, 0x0800, 0xB }, /* Reg(166): NxbRsMpegSyncCtrl bit [11] */
    { StrIp("NxbRsMpegDfailCntMode"),        HCDPRegNxbRsMpegDfailCntMode,        0x00A6, 0x1000, 0xC }, /* Reg(166): NxbRsMpegSyncCtrl bit [12] */
    { StrIp("NxbRsMpegSwAltSyncPos"),        HCDPRegNxbRsMpegSwAltSyncPos,        0x00A6, 0x2000, 0xD }, /* Reg(166): NxbRsMpegSyncCtrl bit [13] */
    { StrIp("NxbRsMpegIgnorePid"),           HCDPRegNxbRsMpegIgnorePid,           0x00A6, 0x4000, 0xE }, /* Reg(166): NxbRsMpegSyncCtrl bit [14] */
    { StrIp("NxbRsMpegOosResetEn"),          HCDPRegNxbRsMpegOosResetEn,          0x00A6, 0x8000, 0xF }, /* Reg(166): NxbRsMpegSyncCtrl bit [15] */
    { StrIp("NxbTrManualSpecInv"),           HCDPRegNxbTrManualSpecInv,           0x00B0, 0x0001, 0x0 }, /* Reg(176): NxbTrSpecInv bit [0] */
    { StrIp("NxbTrSpecInvState"),            HCDPRegNxbTrSpecInvState,            0x00B0, 0x0010, 0x4 }, /* Reg(176): NxbTrSpecInv bit [4] */
    //{ StrIp("NxbTrTstMbistEn"),              HCDPRegNxbTrTstMbistEn,              0x00B9, 0x0001, 0x0 }, /* Reg(185): NxbTrMbist bit [0] */
    //{ StrIp("NxbTrTstMbistStart"),           HCDPRegNxbTrTstMbistStart,           0x00B9, 0x0002, 0x1 }, /* Reg(185): NxbTrMbist bit [1] */
    //{ StrIp("NxbTrTstZeroMem"),              HCDPRegNxbTrTstZeroMem,              0x00B9, 0x0004, 0x2 }, /* Reg(185): NxbTrMbist bit [2] */
    //{ StrIp("NxbTrTstMbistDone"),            HCDPRegNxbTrTstMbistDone,            0x00B9, 0x0010, 0x4 }, /* Reg(185): NxbTrMbist bit [4] */
    //{ StrIp("NxbTrTstMbistFail"),            HCDPRegNxbTrTstMbistFail,            0x00B9, 0x0020, 0x5 }, /* Reg(185): NxbTrMbist bit [5] */
    //{ StrIp("NxbTrMbistFailAddr"),           HCDPRegNxbTrMbistFailAddr,           0x00BA, 0x007F, 0x0 }, /* Reg(186): NxbTrMbistStatus bits [6 downto 0] */
    //{ StrIp("NxbTrMbistFailBit"),            HCDPRegNxbTrMbistFailBit,            0x00BA, 0xFF00, 0x8 }, /* Reg(186): NxbTrMbistStatus bits [15 downto 8] */
    //{ StrIp("NxbTrMbistMinAddr"),            HCDPRegNxbTrMbistMinAddr,            0x00BB, 0x007F, 0x0 }, /* Reg(187): NxbTrMbistCtrl2 bits [6 downto 0] */
    //{ StrIp("NxbTrMbistRetentionMsb"),       HCDPRegNxbTrMbistRetentionMsb,       0x00BB, 0x0F00, 0x8 }, /* Reg(187): NxbTrMbistCtrl2 bits [11 downto 8] */
    { StrIp("NxbTrSoftReset"),               HCDPRegNxbTrSoftReset,               0x00BC, 0x0001, 0x0 }, /* Reg(188): NxbTrReset bit [0] */
    { StrIp("NxbTrMicroReset"),              HCDPRegNxbTrMicroReset,              0x00BC, 0x0002, 0x1 }, /* Reg(188): NxbTrReset bit [1] */
    { StrIp("NxbTrTstZeroMetrics"),          HCDPRegNxbTrTstZeroMetrics,          0x00BD, 0x0001, 0x0 }, /* Reg(189): NxbTrTstControl2 bit [0] */
    { StrIp("NxbTrTstAdvTbMemDis"),          HCDPRegNxbTrTstAdvTbMemDis,          0x00BD, 0x0002, 0x1 }, /* Reg(189): NxbTrTstControl2 bit [1] */
    { StrIp("NxbTrTstAcsNoSat"),             HCDPRegNxbTrTstAcsNoSat,             0x00BD, 0x0004, 0x2 }, /* Reg(189): NxbTrTstControl2 bit [2] */
    { StrIp("NxbTrUseCombDout"),             HCDPRegNxbTrUseCombDout,             0x00BE, 0x0001, 0x0 }, /* Reg(190): NxbTrDoutCtrl bit [0] */
    { StrIp("NxbTrClrRegDout"),              HCDPRegNxbTrClrRegDout,              0x00BE, 0x0002, 0x1 }, /* Reg(190): NxbTrDoutCtrl bit [1] */
    { StrIp("NxbFsyncTstKCnt"),              HCDPRegNxbFsyncTstKCnt,              0x00C2, 0x000F, 0x0 }, /* Reg(194): NxbFsyncTstCnt bits [3 downto 0] */
    { StrIp("NxbFsyncTstNCnt"),              HCDPRegNxbFsyncTstNCnt,              0x00C2, 0x00F0, 0x4 }, /* Reg(194): NxbFsyncTstCnt bits [7 downto 4] */
    { StrIp("NxbFsyncTstSmState"),           HCDPRegNxbFsyncTstSmState,           0x00C3, 0x0003, 0x0 }, /* Reg(195): NxbFsyncTstSmState bits [1 downto 0] */
    { StrIp("NxbFsyncTstSmFailCnt"),         HCDPRegNxbFsyncTstSmFailCnt,         0x00C4, 0x00FF, 0x0 }, /* Reg(196): NxbFsyncTstSmCnt bits [7 downto 0] */
    { StrIp("NxbFsyncTstSmBreakCnt"),        HCDPRegNxbFsyncTstSmBreakCnt,        0x00C4, 0xFF00, 0x8 }, /* Reg(196): NxbFsyncTstSmCnt bits [15 downto 8] */
    { StrIp("NxbFsyncSoftReset"),            HCDPRegNxbFsyncSoftReset,            0x00C5, 0x0001, 0x0 }, /* Reg(197): NxbFsyncReset bit [0] */
    { StrIp("NxbFsyncMicroReset"),           HCDPRegNxbFsyncMicroReset,           0x00C5, 0x0002, 0x1 }, /* Reg(197): NxbFsyncReset bit [1] */
    { StrIp("NxbFsyncUseCombDout"),          HCDPRegNxbFsyncUseCombDout,          0x00C6, 0x0001, 0x0 }, /* Reg(198): NxbFsyncDoutCtrl bit [0] */
    { StrIp("NxbFsyncClrRegDout"),           HCDPRegNxbFsyncClrRegDout,           0x00C6, 0x0002, 0x1 }, /* Reg(198): NxbFsyncDoutCtrl bit [1] */
    //{ StrIp("NxbDeintTstMbistEn"),           HCDPRegNxbDeintTstMbistEn,           0x00D1, 0x0001, 0x0 }, /* Reg(209): NxbDeintMbistCtrl bit [0] */
    //{ StrIp("NxbDeintTstMbistStart"),        HCDPRegNxbDeintTstMbistStart,        0x00D1, 0x0002, 0x1 }, /* Reg(209): NxbDeintMbistCtrl bit [1] */
    //{ StrIp("NxbDeintTstMbistZeroMem"),      HCDPRegNxbDeintTstMbistZeroMem,      0x00D1, 0x0010, 0x4 }, /* Reg(209): NxbDeintMbistCtrl bit [4] */
    //{ StrIp("NxbDeintTstMbistRetentionMsb"), HCDPRegNxbDeintTstMbistRetentionMsb, 0x00D1, 0x0F00, 0x8 }, /* Reg(209): NxbDeintMbistCtrl bits [11 downto 8] */
    //{ StrIp("NxbDeintTstMbistFail"),         HCDPRegNxbDeintTstMbistFail,         0x00D2, 0x0001, 0x0 }, /* Reg(210): NxbDeintMbistStatus bit [0] */
    //{ StrIp("NxbDeintTstMbistDone"),         HCDPRegNxbDeintTstMbistDone,         0x00D2, 0x0002, 0x1 }, /* Reg(210): NxbDeintMbistStatus bit [1] */
    //{ StrIp("NxbDeintTstMbistInprogress"),   HCDPRegNxbDeintTstMbistInprogress,   0x00D2, 0x0004, 0x2 }, /* Reg(210): NxbDeintMbistStatus bit [2] */
    { StrIp("NxbDeintSoftReset"),            HCDPRegNxbDeintSoftReset,            0x00D4, 0x0001, 0x0 }, /* Reg(212): NxbDeintReset bit [0] */
    { StrIp("NxbDeintMicroReset"),           HCDPRegNxbDeintMicroReset,           0x00D4, 0x0002, 0x1 }, /* Reg(212): NxbDeintReset bit [1] */
    { StrIp("NxbDeintUseCombDout"),          HCDPRegNxbDeintUseCombDout,          0x00D5, 0x0001, 0x0 }, /* Reg(213): NxbDeintDoutCtrl bit [0] */
    { StrIp("NxbDeintClrRegDout"),           HCDPRegNxbDeintClrRegDout,           0x00D5, 0x0002, 0x1 }, /* Reg(213): NxbDeintDoutCtrl bit [1] */
    { StrIp("NxbRsMpegTrackQuality"),        HCDPRegNxbRsMpegTrackQuality,        0x00E7, 0x001F, 0x0 }, /* Reg(231): NxbRsMpegStatus1 bits [4 downto 0] */
    { StrIp("NxbRsMpegDfailCnt"),            HCDPRegNxbRsMpegDfailCnt,            0x00E7, 0x1F00, 0x8 }, /* Reg(231): NxbRsMpegStatus1 bits [12 downto 8] */
    { StrIp("NxbRsMpegIntoTrkCnt"),          HCDPRegNxbRsMpegIntoTrkCnt,          0x00E8, 0x00FF, 0x0 }, /* Reg(232): NxbRsMpegStatus2 bits [7 downto 0] */
    { StrIp("NxbRsMpegSyncTrkState"),        HCDPRegNxbRsMpegSyncTrkState,        0x00E8, 0x0700, 0x8 }, /* Reg(232): NxbRsMpegStatus2 bits [10 downto 8] */
    { StrIp("NxbRsMpegRsDoutFail"),          HCDPRegNxbRsMpegRsDoutFail,          0x00E9, 0x00FF, 0x0 }, /* Reg(233): NxbRsMpegRsDoutFail bits [7 downto 0] */
    { StrIp("NxbRsMpegSoftReset"),           HCDPRegNxbRsMpegSoftReset,           0x00EE, 0x0001, 0x0 }, /* Reg(238): NxbRsMpegReset bit [0] */
    { StrIp("NxbRsMpegMicroReset"),          HCDPRegNxbRsMpegMicroReset,          0x00EE, 0x0002, 0x1 }, /* Reg(238): NxbRsMpegReset bit [1] */
    { StrIp("NxbRsMpegSoftSync"),            HCDPRegNxbRsMpegSoftSync,            0x00EE, 0x0004, 0x2 }, /* Reg(238): NxbRsMpegReset bit [2] */
    { StrIp("NxbRsMpegMicroSync"),           HCDPRegNxbRsMpegMicroSync,           0x00EE, 0x0008, 0x3 }, /* Reg(238): NxbRsMpegReset bit [3] */
    //{ StrIp("NxbRsMbistEnable"),             HCDPRegNxbRsMbistEnable,             0x00EF, 0x0001, 0x0 }, /* Reg(239): NxbRsMpegRsMbistCtrl1 bit [0] */
    //{ StrIp("NxbRsMbistStart"),              HCDPRegNxbRsMbistStart,              0x00EF, 0x0002, 0x1 }, /* Reg(239): NxbRsMpegRsMbistCtrl1 bit [1] */
    //{ StrIp("NxbRsMbistZeroMem"),            HCDPRegNxbRsMbistZeroMem,            0x00EF, 0x0004, 0x2 }, /* Reg(239): NxbRsMpegRsMbistCtrl1 bit [2] */
    //{ StrIp("NxbRsMbistDone"),               HCDPRegNxbRsMbistDone,               0x00EF, 0x0008, 0x3 }, /* Reg(239): NxbRsMpegRsMbistCtrl1 bit [3] */
    //{ StrIp("NxbRsMbistFail"),               HCDPRegNxbRsMbistFail,               0x00EF, 0x0010, 0x4 }, /* Reg(239): NxbRsMpegRsMbistCtrl1 bit [4] */
    //{ StrIp("NxbRsMbistFailAddr"),           HCDPRegNxbRsMbistFailAddr,           0x00F0, 0x00FF, 0x0 }, /* Reg(240): NxbRsMpegRsMbistStatus bits [7 downto 0] */
    //{ StrIp("NxbRsMbistFailBit"),            HCDPRegNxbRsMbistFailBit,            0x00F0, 0x0F00, 0x8 }, /* Reg(240): NxbRsMpegRsMbistStatus bits [11 downto 8] */
    //{ StrIp("NxbRsMbistRetentionMsb"),       HCDPRegNxbRsMbistRetentionMsb,       0x00F1, 0x000F, 0x0 }, /* Reg(241): NxbRsMpegRsMbistCtrl2 bits [3 downto 0] */
    //{ StrIp("NxbRsMbistMinAddr"),            HCDPRegNxbRsMbistMinAddr,            0x00F1, 0x0FF0, 0x4 }, /* Reg(241): NxbRsMpegRsMbistCtrl2 bits [11 downto 4] */
    //{ StrIp("NxbCrcMbistEnable"),            HCDPRegNxbCrcMbistEnable,            0x00F2, 0x0001, 0x0 }, /* Reg(242): NxbRsMpegCrcMbistCtrl1 bit [0] */
    //{ StrIp("NxbCrcMbistStart"),             HCDPRegNxbCrcMbistStart,             0x00F2, 0x0002, 0x1 }, /* Reg(242): NxbRsMpegCrcMbistCtrl1 bit [1] */
    //{ StrIp("NxbCrcMbistZeroMem"),           HCDPRegNxbCrcMbistZeroMem,           0x00F2, 0x0004, 0x2 }, /* Reg(242): NxbRsMpegCrcMbistCtrl1 bit [2] */
    //{ StrIp("NxbCrcMbistDone"),              HCDPRegNxbCrcMbistDone,              0x00F2, 0x0008, 0x3 }, /* Reg(242): NxbRsMpegCrcMbistCtrl1 bit [3] */
    //{ StrIp("NxbCrcMbistFail"),              HCDPRegNxbCrcMbistFail,              0x00F2, 0x0010, 0x4 }, /* Reg(242): NxbRsMpegCrcMbistCtrl1 bit [4] */
    //{ StrIp("NxbCrcMbistFailAddr"),          HCDPRegNxbCrcMbistFailAddr,          0x00F3, 0x01F0, 0x4 }, /* Reg(243): NxbRsMpegCrcMbistStatus bits [8 downto 4] */
    //{ StrIp("NxbCrcMbistFailBit"),           HCDPRegNxbCrcMbistFailBit,           0x00F3, 0x1E00, 0x9 }, /* Reg(243): NxbRsMpegCrcMbistStatus bits [12 downto 9] */
    //{ StrIp("NxbCrcMbistRetentionMsb"),      HCDPRegNxbCrcMbistRetentionMsb,      0x00F4, 0x000F, 0x0 }, /* Reg(244): NxbRsMpegCrcMbistCtrl2 bits [3 downto 0] */
    //{ StrIp("NxbCrcMbistMinAddr"),           HCDPRegNxbCrcMbistMinAddr,           0x00F4, 0x1FF0, 0x4 }, /* Reg(244): NxbRsMpegCrcMbistCtrl2 bits [12 downto 4] */
    //{ StrIp("NxbSyncMbistEnable"),           HCDPRegNxbSyncMbistEnable,           0x00F5, 0x0001, 0x0 }, /* Reg(245): NxbRsMpegSyncMbistCtrl1 bit [0] */
    //{ StrIp("NxbSyncMbistStart"),            HCDPRegNxbSyncMbistStart,            0x00F5, 0x0002, 0x1 }, /* Reg(245): NxbRsMpegSyncMbistCtrl1 bit [1] */
    //{ StrIp("NxbSyncMbistZeroMem"),          HCDPRegNxbSyncMbistZeroMem,          0x00F5, 0x0004, 0x2 }, /* Reg(245): NxbRsMpegSyncMbistCtrl1 bit [2] */
    //{ StrIp("NxbSyncMbistDone"),             HCDPRegNxbSyncMbistDone,             0x00F5, 0x0008, 0x3 }, /* Reg(245): NxbRsMpegSyncMbistCtrl1 bit [3] */
    //{ StrIp("NxbSyncMbistFail"),             HCDPRegNxbSyncMbistFail,             0x00F5, 0x0010, 0x4 }, /* Reg(245): NxbRsMpegSyncMbistCtrl1 bit [4] */
    //{ StrIp("NxbSyncMbistFailAddr"),         HCDPRegNxbSyncMbistFailAddr,         0x00F6, 0x00FF, 0x0 }, /* Reg(246): NxbRsMpegSyncMbistStatus bits [7 downto 0] */
    //{ StrIp("NxbSyncMbistFailBit"),          HCDPRegNxbSyncMbistFailBit,          0x00F6, 0x3F00, 0x8 }, /* Reg(246): NxbRsMpegSyncMbistStatus bits [13 downto 8] */
    //{ StrIp("NxbSyncMbistRetentionMsb"),     HCDPRegNxbSyncMbistRetentionMsb,     0x00F7, 0x000F, 0x0 }, /* Reg(247): NxbRsMpegSyncMbistCtrl2 bits [3 downto 0] */
    //{ StrIp("NxbSyncMbistMinAddr"),          HCDPRegNxbSyncMbistMinAddr,          0x00F7, 0x0FF0, 0x4 }, /* Reg(247): NxbRsMpegSyncMbistCtrl2 bits [11 downto 4] */
    { StrIp("NxbRsMpegBestPtr"),             HCDPRegNxbRsMpegBestPtr,             0x00F8, 0x0FFF, 0x0 }, /* Reg(248): NxbRsMpegBestPtr bits [11 downto 0] */
    { StrIp("NxbRsMpegBestStatusPtr"),       HCDPRegNxbRsMpegBestStatusPtr,       0x00FA, 0x0FFF, 0x0 }, /* Reg(250): NxbRsMpegBestStatus bits [11 downto 0] */
    { StrIp("NxbRsMpegBestStatusCnt"),       HCDPRegNxbRsMpegBestStatusCnt,       0x00FA, 0xF000, 0xC }, /* Reg(250): NxbRsMpegBestStatus bits [15 downto 12] */
    { StrIp("NxbRsMpegS2BestStatusPtr"),     HCDPRegNxbRsMpegS2BestStatusPtr,     0x00FB, 0x0FFF, 0x0 }, /* Reg(251): NxbRsMpegS2BestStatus bits [11 downto 0] */
    { StrIp("NxbRsMpegS2BestStatusCnt"),     HCDPRegNxbRsMpegS2BestStatusCnt,     0x00FB, 0xF000, 0xC }, /* Reg(251): NxbRsMpegS2BestStatus bits [15 downto 12] */
    { StrIp("NxbRsMpegCounter"),             HCDPRegNxbRsMpegCounter,             0x00FC, 0x00FF, 0x0 }, /* Reg(252): NxbRsMpegCounter bits [7 downto 0] */
    { StrIp("NxbRsMpegCrcFailCnt"),          HCDPRegNxbRsMpegCrcFailCnt,          0x00FD, 0x00FF, 0x0 }, /* Reg(253): NxbRsMpegFailCnt bits [7 downto 0] */
    { StrIp("NxbRsMpegDfailCnt4Test"),       HCDPRegNxbRsMpegDfailCnt4Test,       0x00FD, 0xFF00, 0x8 }, /* Reg(253): NxbRsMpegFailCnt bits [15 downto 8] */
    { StrIp("NxbRsMpegClrStateCnt"),         HCDPRegNxbRsMpegClrStateCnt,         0x00FE, 0xFFFF, 0x0 }, /* Reg(254): NxbRsMpegClrStateCnt bits [15 downto 0] */
    { StrIp("NxbRsMpegUseCombDout"),         HCDPRegNxbRsMpegUseCombDout,         0x00FF, 0x0001, 0x0 }, /* Reg(255): NxbRsMpegDoutCtrl bit [0] */
    { StrIp("NxbRsMpegClrRegDout"),          HCDPRegNxbRsMpegClrRegDout,          0x00FF, 0x0002, 0x1 }, /* Reg(255): NxbRsMpegDoutCtrl bit [1] */
    { StrIp("NxaDerandBypass"),              HCDPRegNxaDerandBypass,              0x0100, 0x0001, 0x0 }, /* Reg(256): NxaDerandBypass bit [0] */
    { StrIp("NxaRsErrCorDis"),               HCDPRegNxaRsErrCorDis,               0x0103, 0x0001, 0x0 }, /* Reg(259): NxaRsCorCtrl bit [0] */
    { StrIp("NxaRsDecFailDis"),              HCDPRegNxaRsDecFailDis,              0x0103, 0x0002, 0x1 }, /* Reg(259): NxaRsCorCtrl bit [1] */
    { StrIp("NxaOosDis"),                    HCDPRegNxaOosDis,                    0x0104, 0x0001, 0x0 }, /* Reg(260): NxaSyncCtrl bit [0] */
    { StrIp("NxaForceOos"),                  HCDPRegNxaForceOos,                  0x0104, 0x0002, 0x1 }, /* Reg(260): NxaSyncCtrl bit [1] */
    { StrIp("NxaSyncWordThr"),               HCDPRegNxaSyncWordThr,               0x010C, 0x0007, 0x0 }, /* Reg(268): NxaSyncWordThr bits [2 downto 0] */
    { StrIp("NxaDfailThr"),                  HCDPRegNxaDfailThr,                  0x010D, 0x000F, 0x0 }, /* Reg(269): NxaDfailThr bits [3 downto 0] */
    { StrIp("NxaDeintBypass"),               HCDPRegNxaDeintBypass,               0x010E, 0x0001, 0x0 }, /* Reg(270): NxaDeintBypass bit [0] */
    { StrIp("NxaRsSync"),                    HCDPRegNxaRsSync,                    0x010F, 0x0001, 0x0 }, /* Reg(271): NxaRsSync bit [0] */
    { StrIp("NxaRsStatus"),                  HCDPRegNxaStatus,                    0x0110, 0xFFFF, 0x0 }, /* Reg(272): NxaStatus bit [0:15] */
    { StrIp("NxaRsReady"),                   HCDPRegNxaRsReady,                   0x0110, 0x0001, 0x0 }, /* Reg(272): NxaStatus bit [0] */
    { StrIp("NxaSync"),                      HCDPRegNxaSync,                      0x0110, 0x0002, 0x1 }, /* Reg(272): NxaStatus bit [1] */
    { StrIp("NxaDerandState"),               HCDPRegNxaDerandState,               0x0110, 0x0030, 0x4 }, /* Reg(272): NxaStatus bits [5 downto 4] */
    { StrIp("NxaDerandFailCnt"),             HCDPRegNxaDerandFailCnt,             0x0110, 0x0700, 0x8 }, /* Reg(272): NxaStatus bits [10 downto 8] */
    { StrIp("NxaUseNxaSync"),                HCDPRegNxaUseNxaSync,                0x0111, 0x0001, 0x0 }, /* Reg(273): NxaUseNxaSync bit [0] */
    { StrIp("NxaTstTpGenEnable"),            HCDPRegNxaTstTpGenEnable,            0x0132, 0x0001, 0x0 }, /* Reg(306): NxaTstTpGenEnable bit [0] */
    { StrIp("NxaTstClkDly"),                 HCDPRegNxaTstClkDly,                 0x0133, 0x0001, 0x0 }, /* Reg(307): NxaTstClkDly bit [0] */
    //{ StrIp("NxaMbistEnable"),               HCDPRegNxaMbistEnable,               0x0134, 0x0001, 0x0 }, /* Reg(308): NxaMbistCtrl1 bit [0] */
    //{ StrIp("NxaMbistStart"),                HCDPRegNxaMbistStart,                0x0134, 0x0002, 0x1 }, /* Reg(308): NxaMbistCtrl1 bit [1] */
    //{ StrIp("NxaMbistZeroMem"),              HCDPRegNxaMbistZeroMem,              0x0134, 0x0010, 0x4 }, /* Reg(308): NxaMbistCtrl1 bit [4] */
    //{ StrIp("NxaMbistMemSelect"),            HCDPRegNxaMbistMemSelect,            0x0134, 0x00C0, 0x6 }, /* Reg(308): NxaMbistCtrl1 bits [7 downto 6] */
    //{ StrIp("NxaMbistDone"),                 HCDPRegNxaMbistDone,                 0x0135, 0x0001, 0x0 }, /* Reg(309): NxaMbistStatus1 bit [0] */
    //{ StrIp("NxaMbistFail"),                 HCDPRegNxaMbistFail,                 0x0135, 0x0002, 0x1 }, /* Reg(309): NxaMbistStatus1 bit [1] */
    //{ StrIp("NxaMbistFailAddr"),             HCDPRegNxaMbistFailAddr,             0x0138, 0x07FF, 0x0 }, /* Reg(312): NxaMbistStatus2 bits [10 downto 0] */
    //{ StrIp("NxaMbistFailBit"),              HCDPRegNxaMbistFailBit,              0x0138, 0xF800, 0xB }, /* Reg(312): NxaMbistStatus2 bits [15 downto 11] */
    //{ StrIp("NxaMbistMinAddr"),              HCDPRegNxaMbistMinAddr,              0x0139, 0x07FF, 0x0 }, /* Reg(313): NxaMbistCtrl2 bits [10 downto 0] */
    //{ StrIp("NxaMbistRetentionMsb"),         HCDPRegNxaMbistRetentionMsb,         0x0139, 0x7800, 0xB }, /* Reg(313): NxaMbistCtrl2 bits [14 downto 11] */
    { StrIp("NxaSoftReset"),                 HCDPRegNxaSoftReset,                 0x013D, 0x0001, 0x0 }, /* Reg(317): NxaReset bit [0] */
    { StrIp("NxaMicroReset"),                HCDPRegNxaMicroReset,                0x013D, 0x0002, 0x1 }, /* Reg(317): NxaReset bit [1] */
    { StrIp("NxaUseCombDout"),               HCDPRegNxaUseCombDout,               0x013E, 0x0001, 0x0 }, /* Reg(318): NxaDoutCtrl bit [0] */
    { StrIp("NxaClearRegDout"),              HCDPRegNxaClearRegDout,              0x013E, 0x0002, 0x1 }, /* Reg(318): NxaDoutCtrl bit [1] */
    { StrIp("FFTScaleStage1"),               HCDPRegFFTScaleStage1,               0x01C0, 0x0001, 0x0 }, /* Reg(448): FFTControl bit [0] */
    { StrIp("FFTScaleStage2"),               HCDPRegFFTScaleStage2,               0x01C0, 0x0002, 0x1 }, /* Reg(448): FFTControl bit [1] */
    { StrIp("FFTScaleStage3"),               HCDPRegFFTScaleStage3,               0x01C0, 0x0004, 0x2 }, /* Reg(448): FFTControl bit [2] */
    { StrIp("FFTScaleStage4"),               HCDPRegFFTScaleStage4,               0x01C0, 0x0008, 0x3 }, /* Reg(448): FFTControl bit [3] */
    { StrIp("FFTScaleStage5"),               HCDPRegFFTScaleStage5,               0x01C0, 0x0010, 0x4 }, /* Reg(448): FFTControl bit [4] */
    { StrIp("FFTScaleStage6"),               HCDPRegFFTScaleStage6,               0x01C0, 0x0020, 0x5 }, /* Reg(448): FFTControl bit [5] */
    { StrIp("FFTInitOneTapFilters"),         HCDPRegFFTInitOneTapFilters,         0x01C0, 0x0040, 0x6 }, /* Reg(448): FFTControl bit [6] */
    { StrIp("FFTSymEnPhase"),                HCDPRegFFTSymEnPhase,                0x01C0, 0x0080, 0x7 }, /* Reg(448): FFTControl bit [7] */
    { StrIp("FFTSoftReset"),                 HCDPRegFFTSoftReset,                 0x01C1, 0x0001, 0x0 }, /* Reg(449): FFTReset bit [0] */
    { StrIp("FFTMicroReset"),                HCDPRegFFTMicroReset,                0x01C1, 0x0002, 0x1 }, /* Reg(449): FFTReset bit [1] */
    { StrIp("FFTReadAddress"),               HCDPRegFFTReadAddress,               0x01C2, 0x003F, 0x0 }, /* Reg(450): FFTReadAddress bits [5 downto 0] */
    { StrIp("FFTDataRead"),                  HCDPRegFFTDataRead,                  0x01C3, 0x00FF, 0x0 }, /* Reg(451): FFTReadValue bits [7 downto 0] */
    { StrIp("FFTActualAddress"),             HCDPRegFFTActualAddress,             0x01C3, 0x3F00, 0x8 }, /* Reg(451): FFTReadValue bits [13 downto 8] */
    { StrIp("FFTDataReady"),                 HCDPRegFFTDataReady,                 0x01C3, 0x4000, 0xE }, /* Reg(451): FFTReadValue bit [14] */
    //{ StrIp("FFTMbistEnable"),               HCDPRegFFTMbistEnable,               0x01C4, 0x0001, 0x0 }, /* Reg(452): FFTMbistCtrl1 bit [0] */
    //{ StrIp("FFTMbistStart"),                HCDPRegFFTMbistStart,                0x01C4, 0x0002, 0x1 }, /* Reg(452): FFTMbistCtrl1 bit [1] */
    //{ StrIp("FFTMbistZeroMem"),              HCDPRegFFTMbistZeroMem,              0x01C4, 0x0004, 0x2 }, /* Reg(452): FFTMbistCtrl1 bit [2] */
    //{ StrIp("FFTMbistDone"),                 HCDPRegFFTMbistDone,                 0x01C4, 0x0008, 0x3 }, /* Reg(452): FFTMbistCtrl1 bit [3] */
    //{ StrIp("FFTMbistFail"),                 HCDPRegFFTMbistFail,                 0x01C4, 0x0010, 0x4 }, /* Reg(452): FFTMbistCtrl1 bit [4] */
    //{ StrIp("FFTMbistMemSelect"),            HCDPRegFFTMbistMemSelect,            0x01C4, 0x0060, 0x5 }, /* Reg(452): FFTMbistCtrl1 bits [6 downto 5] */
    //{ StrIp("FFTMbistFailAddr"),             HCDPRegFFTMbistFailAddr,             0x01C5, 0x003F, 0x0 }, /* Reg(453): FFTMbistStatus bits [5 downto 0] */
    //{ StrIp("FFTMbistFailBit"),              HCDPRegFFTMbistFailBit,              0x01C5, 0x3F00, 0x8 }, /* Reg(453): FFTMbistStatus bits [13 downto 8] */
    //{ StrIp("FFTMbistMinAddr"),              HCDPRegFFTMbistMinAddr,              0x01C6, 0x003F, 0x0 }, /* Reg(454): FFTMbistCtrl2 bits [5 downto 0] */
    //{ StrIp("FFTMbistRetentionMsb"),         HCDPRegFFTMbistRetentionMsb,         0x01C6, 0x0F00, 0x8 }, /* Reg(454): FFTMbistCtrl2 bits [11 downto 8] */
    { StrIp("FFTUseCombDout"),               HCDPRegFFTUseCombDout,               0x01C7, 0x0001, 0x0 }, /* Reg(455): FFTDoutCtrl bit [0] */
    { StrIp("FFTClearRegDout"),              HCDPRegFFTClearRegDout,              0x01C7, 0x0002, 0x1 }, /* Reg(455): FFTDoutCtrl bit [1] */
    { StrIp("TimersResetPulse"),             HCDPRegTimersResetPulse,             0x01E0, 0x000F, 0x0 }, /* Reg(480): TimersPulse bits [3 downto 0] */
    { StrIp("TimersPausePulse"),             HCDPRegTimersPausePulse,             0x01E0, 0x00F0, 0x4 }, /* Reg(480): TimersPulse bits [7 downto 4] */
    { StrIp("TimersResumePulse"),            HCDPRegTimersResumePulse,            0x01E0, 0x0F00, 0x8 }, /* Reg(480): TimersPulse bits [11 downto 8] */
    { StrIp("TimersMode"),                   HCDPRegTimersMode,                   0x01E1, 0x000F, 0x0 }, /* Reg(481): TimersCtrl bits [3 downto 0] */
    { StrIp("TimersXmode"),                  HCDPRegTimersXmode,                  0x01E1, 0x00F0, 0x4 }, /* Reg(481): TimersCtrl bits [7 downto 4] */
    { StrIp("TimersSmode"),                  HCDPRegTimersSmode,                  0x01E1, 0x0F00, 0x8 }, /* Reg(481): TimersCtrl bits [11 downto 8] */
    { StrIp("TimersUseCombDout"),            HCDPRegTimersUseCombDout,            0x01E1, 0x1000, 0xC }, /* Reg(481): TimersCtrl bit [12] */
    { StrIp("TimersClrRegDout"),             HCDPRegTimersClrRegDout,             0x01E1, 0x2000, 0xD }, /* Reg(481): TimersCtrl bit [13] */
    { StrIp("TimersLockInd"),                HCDPRegTimersLockInd,                0x01E2, 0x000F, 0x0 }, /* Reg(482): TimersStatus bits [3 downto 0] */
    { StrIp("TimersWrapInd"),                HCDPRegTimersWrapInd,                0x01E2, 0x00F0, 0x4 }, /* Reg(482): TimersStatus bits [7 downto 4] */
    { StrIp("TimersBreakAfterLockInd"),      HCDPRegTimersBreakAfterLockInd,      0x01E2, 0x0F00, 0x8 }, /* Reg(482): TimersStatus bits [11 downto 8] */
    { StrIp("TimerslockAfterBreakInd"),      HCDPRegTimerslockAfterBreakInd,      0x01E2, 0xF000, 0xC }, /* Reg(482): TimersStatus bits [15 downto 12] */
    { StrIp("Timer0"),                       HCDPRegTimer0,                       0x01E3, 0xFFFF, 0x0 }, /* Reg(483): Timer0 bits [15 downto 0] */
    { StrIp("Timer1"),                       HCDPRegTimer1,                       0x01E4, 0xFFFF, 0x0 }, /* Reg(484): Timer1 bits [15 downto 0] */
    { StrIp("Timer2"),                       HCDPRegTimer2,                       0x01E5, 0xFFFF, 0x0 }, /* Reg(485): Timer2 bits [15 downto 0] */
    { StrIp("Timer3"),                       HCDPRegTimer3,                       0x01E6, 0xFFFF, 0x0 }, /* Reg(486): Timer3 bits [15 downto 0] */
    { StrIp("TimersTicDivider"),             HCDPRegTimersTicDivider,             0x01E7, 0xFFFF, 0x0 }, /* Reg(487): TimersTicDivider bits [15 downto 0] */
    { StrIp("AagcFreeze0"),                  HCDPRegAagcFreeze0,                  0x0200, 0x0001, 0x0 }, /* Reg(512): AagcFreeze0 bit [0] */
    { StrIp("AagcEmThr0"),                   HCDPRegAagcEmThr0,                   0x0201, 0x003F, 0x0 }, /* Reg(513): AagcEmThr0 bits [5 downto 0] */
    { StrIp("AagcAcc0"),                     HCDPRegAagcAcc0,                     0x0202, 0x03FF, 0x0 }, /* Reg(514): AagcAcc0 bits [9 downto 0] */
    { StrIp("AagcBw0"),                      HCDPRegAagcBw0,                      0x0203, 0x000F, 0x0 }, /* Reg(515): AagcBw0 bits [3 downto 0] */
    { StrIp("AagcSdPol0"),                   HCDPRegAagcSdPol0,                   0x0204, 0x0001, 0x0 }, /* Reg(516): AagcSdPol0 bit [0] */
    { StrIp("AagcSdEn0"),                    HCDPRegAagcSdEn0,                    0x0205, 0x0001, 0x0 }, /* Reg(517): AagcSdEn0 bit [0] */
    { StrIp("AagcSdRate0"),                  HCDPRegAagcSdRate0,                  0x0206, 0x0003, 0x0 }, /* Reg(518): AagcSdRate0 bits [1 downto 0] */
    { StrIp("AagcClr"),                      HCDPRegAagcClr,                      0x0207, 0x0001, 0x0 }, /* Reg(519): AagcClr bit [0] */
    { StrIp("AagcSigLevel"),                 HCDPRegAagcSigLevel,                 0x0208, 0x03FF, 0x0 }, /* Reg(520): AagcSigLevel bits [9 downto 0] */
    { StrIp("AagcSatLevel"),                 HCDPRegAagcSatLevel,                 0x0209, 0x003F, 0x0 }, /* Reg(521): AagcSatLevel bits [5 downto 0] */
    { StrIp("AagcPreSatLevel"),              HCDPRegAagcPreSatLevel,              0x020A, 0x003F, 0x0 }, /* Reg(522): AagcPreSatLevel bits [5 downto 0] */
    { StrIp("AagcSatEmThr"),                 HCDPRegAagcSatEmThr,                 0x020B, 0x003F, 0x0 }, /* Reg(523): AagcSatEmThr bits [5 downto 0] */
    { StrIp("AagcPreSatEmThr"),              HCDPRegAagcPreSatEmThr,              0x020C, 0x003F, 0x0 }, /* Reg(524): AagcPreSatEmThr bits [5 downto 0] */
    { StrIp("AagcFreeze1"),                  HCDPRegAagcFreeze1,                  0x020F, 0x0001, 0x0 }, /* Reg(527): AagcFreeze1 bit [0] */
    { StrIp("AagcEmThLow1"),                 HCDPRegAagcEmThLow1,                 0x0210, 0x03FF, 0x0 }, /* Reg(528): AagcEmThLow1 bits [9 downto 0] */
    { StrIp("AagcEmThHigh1"),                HCDPRegAagcEmThHigh1,                0x0211, 0x03FF, 0x0 }, /* Reg(529): AagcEmThHigh1 bits [9 downto 0] */
    { StrIp("AagcAcc1"),                     HCDPRegAagcAcc1,                     0x0212, 0x03FF, 0x0 }, /* Reg(530): AagcAcc1 bits [9 downto 0] */
    { StrIp("AagcBw1"),                      HCDPRegAagcBw1,                      0x0213, 0x000F, 0x0 }, /* Reg(531): AagcBw1 bits [3 downto 0] */
    { StrIp("AagcSdPol1"),                   HCDPRegAagcSdPol1,                   0x0214, 0x0001, 0x0 }, /* Reg(532): AagcSdPol1 bit [0] */
    { StrIp("AagcSdEn1"),                    HCDPRegAagcSdEn1,                    0x0215, 0x0001, 0x0 }, /* Reg(533): AagcSdEn1 bit [0] */
    { StrIp("AagcSdRate1"),                  HCDPRegAagcSdRate1,                  0x0216, 0x0003, 0x0 }, /* Reg(534): AagcSdRate1 bits [1 downto 0] */
    { StrIp("AagcEmThLow2"),                 HCDPRegAagcEmThLow2,                 0x0217, 0x03FF, 0x0 }, /* Reg(535): AagcEmThLow2 bits [9 downto 0] */
    { StrIp("AagcEmThHigh2"),                HCDPRegAagcEmThHigh2,                0x0218, 0x03FF, 0x0 }, /* Reg(536): AagcEmThHigh2 bits [9 downto 0] */
    { StrIp("AagcFreeze2"),                  HCDPRegAagcFreeze2,                  0x0219, 0x0001, 0x0 }, /* Reg(537): AagcFreeze2 bit [0] */
    { StrIp("AagcAcc2"),                     HCDPRegAagcAcc2,                     0x021A, 0x03FF, 0x0 }, /* Reg(538): AagcAcc2 bits [9 downto 0] */
    { StrIp("AagcBw2"),                      HCDPRegAagcBw2,                      0x021B, 0x000F, 0x0 }, /* Reg(539): AagcBw2 bits [3 downto 0] */
    { StrIp("AagcSdPol2"),                   HCDPRegAagcSdPol2,                   0x021C, 0x0001, 0x0 }, /* Reg(540): AagcSdPol2 bit [0] */
    { StrIp("AagcSdEn2"),                    HCDPRegAagcSdEn2,                    0x021D, 0x0001, 0x0 }, /* Reg(541): AagcSdEn2 bit [0] */
    { StrIp("AagcSdRate2"),                  HCDPRegAagcSdRate2,                  0x021E, 0x0003, 0x0 }, /* Reg(542): AagcSdRate2 bits [1 downto 0] */
    { StrIp("AagcLockVal"),                  HCDPRegAagcLockVal,                  0x021F, 0x003F, 0x0 }, /* Reg(543): AagcLockVal bits [5 downto 0] */
    { StrIp("AagcLockThLow"),                HCDPRegAagcLockThLow,                0x0220, 0x003F, 0x0 }, /* Reg(544): AagcLockThLow bits [5 downto 0] */
    { StrIp("AagcLockThHigh"),               HCDPRegAagcLockThHigh,               0x0221, 0x003F, 0x0 }, /* Reg(545): AagcLockThHigh bits [5 downto 0] */
    { StrIp("AagcIfSdSw"),                   HCDPRegAagcIfSdSw,                   0x0222, 0x0003, 0x0 }, /* Reg(546): AagcOutputCtrl bits [1 downto 0] */
    { StrIp("AagcIfAttnSdSw"),               HCDPRegAagcIfAttnSdSw,               0x0222, 0x0030, 0x4 }, /* Reg(546): AagcOutputCtrl bits [5 downto 4] */
    { StrIp("AagcRfSdSw"),                   HCDPRegAagcRfSdSw,                   0x0222, 0x0300, 0x8 }, /* Reg(546): AagcOutputCtrl bits [9 downto 8] */
    { StrIp("AagcSidewinder"),               HCDPRegAagcSidewinder,               0x0223, 0x0001, 0x0 }, /* Reg(547): Register bit [0] */
    { StrIp("AagcSdAdc"),                    HCDPRegAagcSdAdc,                    0x0225, 0x0001, 0x0 }, /* Reg(549): AagcSdAdc bit [0] */
    { StrIp("AagcSatPreSatSwap"),            HCDPRegAagcSatPreSatSwap,            0x0226, 0x0001, 0x0 }, /* Reg(550): AagcSatPreSatSwap bit [0] */
    { StrIp("FedrBypass"),                   HCDPRegFedrBypass,                   0x0230, 0x0001, 0x0 }, /* Reg(560): FedrBypass bit [0] */
    //{ StrIp("DerotateFreq1Low"),             HCDPRegDerotateFreq1Low,             0x0231, 0xFFFF, 0x0 }, /* Reg(561): DerotateFreqLow1 bits [15 downto 0] */
    //{ StrIp("DerotateFreq1"),                HCDPRegDerotateFreq1High,            0x0232, 0x00FF, 0x0 }, /* Reg(562): DerotateFreqHigh1 bits [7 downto 0] */
    { StrIp("DerotateFreq"),                 HCDPRegDerotateFreqLow,              0x0233, 0xFFFF, 0x0 }, /* Reg(563): DerotateFreqLow2 bits [15 downto 0] */
    { StrIp("DerotateFreq"),                 HCDPRegDerotateFreqHigh,             0x0234, 0x00FF, 0x0 }, /* Reg(564): DerotateFreqHigh2 bits [7 downto 0] */
    { StrIp("AdcFifoFreqAccu"),              HCDPRegAdcFifoFreqAccu,              0x0235, 0xFFFF, 0x0 }, /* Reg(565): AdcFifoFreqAccu bit [15 downto 0] */
    { StrIp("AdcIfcZeroAdcData"),            HCDPRegAdcIfcZeroAdcData,            0x0238, 0x0001, 0x0 }, /* Reg(568): AdcIfcZeroAdcData bit [0] */
    { StrIp("AdcStaggeredSampling"),         HCDPRegAdcStaggeredSampling,         0x0239, 0x0003, 0x0 }, /* Reg(569): AdcIfcAutoAlignMode bits [1 downto 0] */
    { StrIp("AdcIfcFifoRdPtr"),              HCDPRegAdcIfcFifoRdPtr,              0x023A, 0x0003, 0x0 }, /* Reg(570): AdcIfcFifoStatus bits [9 downto 8] */
    { StrIp("AdcIfcFifoRdErr"),              HCDPRegAdcIfcFifoRdErr,              0x023A, 0x0300, 0x8 }, /* Reg(570): AdcIfcFifoStatus bits [7 downto 4] */
    //{ StrIp("AdcInScale"),                   HCDPRegAdcInScale,                   0x023B, 0x0001, 0x0 }, /* Reg(571): AdcIfcInScale bit [0] */
    { StrIp("AdcOffsetBinaryIn"),            HCDPRegAdcOffsetBinaryIn,            0x023C, 0x0001, 0x0 }, /* Reg(572): AdcIfcOffsetBinaryIn bit [0] */
    { StrIp("PdfBypass"),                    HCDPRegPdfBypass,                    0x0240, 0x0001, 0x0 }, /* Reg(576): PdfBypass bit [0] */
    { StrIp("PdfGain"),                      HCDPRegPdfGain,                      0x0241, 0x0003, 0x0 }, /* Reg(577): PdfGain bits [1 downto 0] */
    { StrIp("DagcBypass"),                   HCDPRegDagcBypass,                   0x0250, 0x0001, 0x0 }, /* Reg(592): DagcBypass bit [0] */
    { StrIp("DagcDisable"),                  HCDPRegDagcDisable,                  0x0251, 0x0001, 0x0 }, /* Reg(593): DagcDisable bit [0] */
    { StrIp("DagcAcc"),                      HCDPRegDagcAcc,                      0x0252, 0x00FF, 0x0 }, /* Reg(594): DagcAcc bits [7 downto 0] */
    { StrIp("DagcOutGain"),                  HCDPRegDagcOutGain,                  0x0253, 0x0007, 0x0 }, /* Reg(595): DagcOutGain bits [2 downto 0] */
    { StrIp("DagcBandwidth"),                HCDPRegDagcBandwidth,                0x0254, 0x0007, 0x0 }, /* Reg(596): DagcBandwidth bits [2 downto 0] */
    { StrIp("DagcThreshold"),                HCDPRegDagcThreshold,                0x0255, 0x00FF, 0x0 }, /* Reg(597): DagcThreshold bits [7 downto 0] */
    { StrIp("DagcPosSatInd"),                HCDPRegDagcPosSatInd,                0x0256, 0x0001, 0x0 }, /* Reg(598): DagcPosSatInd bit [0] */
    { StrIp("MfBypass"),                     HCDPRegMfBypass,                     0x0260, 0x0003, 0x0 }, /* Reg(608): MfBypass bits [1 downto 0] */
    { StrIp("MfCoeffSel"),                   HCDPRegMfCoeffSel,                   0x0261, 0x0007, 0x0 }, /* Reg(609): MfCoeffSel bits [2 downto 0] */
    { StrIp("StlBlindTrMode"),               HCDPRegStlBlindTrMode,               0x0270, 0x0001, 0x0 }, /* Reg(624): StlBlindTrMode bit [0] */
    { StrIp("StlDecNoptMode"),               HCDPRegStlDecNoptMode,               0x0271, 0x0001, 0x0 }, /* Reg(625): StlDecNoptMode bit [0] */
    { StrIp("StlDisable"),                   HCDPRegStlDisable,                   0x0272, 0x0001, 0x0 }, /* Reg(626): StlDisable bit [0] */
    { StrIp("StlInMode"),                    HCDPRegStlInMode,                    0x0273, 0x0001, 0x0 }, /* Reg(627): StlInMode bit [0] */
    { StrIp("StlEmMode"),                    HCDPRegStlEmMode,                    0x0274, 0x0001, 0x0 }, /* Reg(628): StlEmMode bit [0] */
    { StrIp("StlDdEmGain"),                  HCDPRegStlDdEmGain,                  0x0275, 0x0003, 0x0 }, /* Reg(629): StlDdEmGain bits [1 downto 0] */
    { StrIp("StlDaEmGain"),                  HCDPRegStlDaEmGain,                  0x0276, 0x0003, 0x0 }, /* Reg(630): StlDaEmGain bits [1 downto 0] */
    { StrIp("StlAcqDirGain"),                HCDPRegStlAcqDirGain,                0x0277, 0x0007, 0x0 }, /* Reg(631): StlAcqGain bits [2 downto 0] */
    { StrIp("StlAcqIndGain"),                HCDPRegStlAcqIndGain,                0x0277, 0x0070, 0x4 }, /* Reg(631): StlAcqGain bits [6 downto 4] */
    { StrIp("StlTrkDirGain"),                HCDPRegStlTrkDirGain,                0x0278, 0x0007, 0x0 }, /* Reg(632): StlTrkGain bits [2 downto 0] */
    { StrIp("StlTrkIndGain"),                HCDPRegStlTrkIndGain,                0x0278, 0x0070, 0x4 }, /* Reg(632): StlTrkGain bits [6 downto 4] */
    { StrIp("StlOctaveScale"),               HCDPRegStlOctaveScale,               0x0279, 0x0001, 0x0 }, /* Reg(633): StlOctaveScale bit [0] */
    { StrIp("StlEmAutoSw"),                  HCDPRegStlEmAutoSw,                  0x027A, 0x0001, 0x0 }, /* Reg(634): StlEmAutoSw bit [0] */
    { StrIp("SymRateLow"),                   HCDPRegSymRateLow,                   0x027C, 0xFFFF, 0x0 }, /* Reg(636): SymRateLow bits [15 downto 0] */
    { StrIp("SymRateHigh"),                  HCDPRegSymRateHigh,                  0x027D, 0x00FF, 0x0 }, /* Reg(637): SymRateHigh bits [7 downto 0] */
    { StrIp("StlAcc"),                       HCDPRegStlAcc,                       0x027E, 0x3FFF, 0x0 }, /* Reg(638): StlAcc bits [13 downto 0] */
    { StrIp("StlClk2UtlPol"),                HCDPRegStlClk2UtlPol,                0x027F, 0x0001, 0x0 }, /* Reg(639): StlClk2UtlPol bit [0] */
    { StrIp("StlClk2UtlDiv2"),               HCDPRegStlClk2UtlDiv2,               0x0280, 0x0001, 0x0 }, /* Reg(640): StlClk2UtlDiv2 bit [0] */
    { StrIp("CnstlSymDelay"),                HCDPRegCnstlSymDelay,                0x0281, 0x0001, 0x0 }, /* Reg(641): CnstlSymDelay bit [0] */
    { StrIp("StlZeroDirectPath"),            HCDPRegStlZeroDirectPath,            0x0282, 0x0001, 0x0 }, /* Reg(642): StlZeroDirectPath bit [0] */
    { StrIp("StlGadEmSel"),                  HCDPRegStlGadEmSel,                  0x0283, 0x0001, 0x0 }, /* Reg(643): StlGadEmSel bit [0] */
    { StrIp("StlGadEmGain"),                 HCDPRegStlGadEmGain,                 0x0284, 0x0007, 0x0 }, /* Reg(644): StlGadEmGain bits [2 downto 0] */
    { StrIp("StlSweepStep"),                 HCDPRegStlSweepStep,                 0x0285, 0xFF00, 0x8 }, /* Reg(645): StlSweepStep bits [15 downto 8] */
    { StrIp("StlSweepStepTime"),             HCDPRegStlSweepStepTime,             0x0285, 0x00FF, 0x0 }, /* Reg(645): StlSweepStepTime bits [7 downto 0] */
    { StrIp("StlSweepMin"),                  HCDPRegStlSweepMin,                  0x0286, 0xFFFF, 0x0 }, /* Reg(646): StlSweepMin bits [15 downto 0] */
    { StrIp("StlSweepMax"),                  HCDPRegStlSweepMax,                  0x0287, 0xFFFF, 0x0 }, /* Reg(647): StlSweepMax bits [15 downto 0] */
    { StrIp("StlSweepTimer"),                HCDPRegStlSweepTimer,                0x0288, 0xFFFF, 0x0 }, /* Reg(648): StlSweepTimer bits [15 downto 0] */
    { StrIp("StlLockThr"),                   HCDPRegStlLockThr,                   0x0290, 0x007F, 0x0 }, /* Reg(656): StlLockThr bits [6 downto 0] */
    { StrIp("StlFadeThr"),                   HCDPRegStlFadeThr,                   0x0291, 0x007F, 0x0 }, /* Reg(657): StlFadeThr bits [6 downto 0] */
    { StrIp("StlLockBW"),                    HCDPRegStlLockBW,                    0x0292, 0x0003, 0x0 }, /* Reg(658): StlLockBW bits [1 downto 0] */
    { StrIp("StlLockAcc"),                   HCDPRegStlLockAcc,                   0x0293, 0x007F, 0x0 }, /* Reg(659): StlLockAcc bits [6 downto 0] */
    { StrIp("StlLockPol"),                   HCDPRegStlLockPol,                   0x0294, 0x0001, 0x0 }, /* Reg(660): StlLockPol bit [0] */
    { StrIp("CMseI"),                        HCDPRegCMseI,                        0x02A2, 0xFFFF, 0x0 }, /* Reg(674): CMseI bits [15 downto 0] */
    { StrIp("CMseQ"),                        HCDPRegCMseQ,                        0x02A3, 0xFFFF, 0x0 }, /* Reg(675): CMseQ bits [15 downto 0] */
    { StrIp("CMseBW"),                       HCDPRegCMseBW,                       0x02A4, 0x0003, 0x0 }, /* Reg(676): CMseBW bits [1 downto 0] */
    { StrIp("CMseDecI"),                     HCDPRegCMseDecI,                     0x02A5, 0x003F, 0x0 }, /* Reg(677): CMseControlI bits [5 downto 0] */
    { StrIp("CMseCompI"),                    HCDPRegCMseCompI,                    0x02A5, 0x00C0, 0x6 }, /* Reg(677): CMseControlI bits [7 downto 6] */
    { StrIp("CMseAbsI"),                     HCDPRegCMseAbsI,                     0x02A5, 0x0100, 0x8 }, /* Reg(677): CMseControlI bit [8] */
    { StrIp("CMseSignI"),                    HCDPRegCMseSignI,                    0x02A5, 0x0200, 0x9 }, /* Reg(677): CMseControlI bit [9] */
    { StrIp("CMseDecQ"),                     HCDPRegCMseDecQ,                     0x02A6, 0x003F, 0x0 }, /* Reg(678): CMseControlQ bits [5 downto 0] */
    { StrIp("CMseCompQ"),                    HCDPRegCMseCompQ,                    0x02A6, 0x00C0, 0x6 }, /* Reg(678): CMseControlQ bits [7 downto 6] */
    { StrIp("CMseAbsQ"),                     HCDPRegCMseAbsQ,                     0x02A6, 0x0100, 0x8 }, /* Reg(678): CMseControlQ bit [8] */
    { StrIp("CMseSignQ"),                    HCDPRegCMseSignQ,                    0x02A6, 0x0200, 0x9 }, /* Reg(678): CMseControlQ bit [9] */
    { StrIp("UMseLow"),                      HCDPRegUMseLow,                      0x02A7, 0xFFFF, 0x0 }, /* Reg(679): UMseLow bits [15 downto 0] */
    { StrIp("UMseHigh"),                     HCDPRegUMseHigh,                     0x02A8, 0x000F, 0x0 }, /* Reg(680): UMseHigh bits [3 downto 0] */
    { StrIp("UMseBW"),                       HCDPRegUMseBW,                       0x02A9, 0x0003, 0x0 }, /* Reg(681): UMseBW bits [1 downto 0] */
    { StrIp("MseLockThr"),                   HCDPRegMseLockThr,                   0x02B0, 0xFFFF, 0x0 }, /* Reg(688): MseLockThr bits [15 downto 0] */
    { StrIp("MseFadeThr"),                   HCDPRegMseFadeThr,                   0x02B1, 0xFFFF, 0x0 }, /* Reg(689): MseFadeThr bits [15 downto 0] */
    { StrIp("MseSlicerSel"),                 HCDPRegMseSlicerSel,                 0x02C2, 0x0003, 0x0 }, /* Reg(706): MseSlicerSel bits [1 downto 0] */
    { StrIp("EnEqCt2Stl"),                   HCDPRegEnEqCt2Stl,                   0x02C3, 0x0001, 0x0 }, /* Reg(707): EnEqCt2Stl bit [0] */
    { StrIp("EnBedr2Eq"),                    HCDPRegEnBedr2Eq,                    0x02C4, 0x0001, 0x0 }, /* Reg(708): EnBedr2Eq bit [0] */
    { StrIp("CagcBypass"),                   HCDPRegCagcBypass,                   0x02D0, 0x0001, 0x0 }, /* Reg(720): CagcBypass bit [0] */
    { StrIp("CagcDisable"),                  HCDPRegCagcDisable,                  0x02D1, 0x0001, 0x0 }, /* Reg(721): CagcDisable bit [0] */
    { StrIp("CagcAcc"),                      HCDPRegCagcAcc,                      0x02D2, 0x03FF, 0x0 }, /* Reg(722): CagcAcc bits [9 downto 0] */
    { StrIp("CagcOutGain"),                  HCDPRegCagcOutGain,                  0x02D3, 0x0007, 0x0 }, /* Reg(723): CagcOutGain bits [2 downto 0] */
    { StrIp("CagcBandwidth"),                HCDPRegCagcBandwidth,                0x02D4, 0x0007, 0x0 }, /* Reg(724): CagcBandwidth bits [2 downto 0] */
    { StrIp("CagcBlindThr"),                 HCDPRegCagcBlindThr,                 0x02D5, 0x00FF, 0x0 }, /* Reg(725): CagcBlindThr bits [7 downto 0] */
    { StrIp("CagcEmAutoMode"),               HCDPRegCagcEmAutoMode,               0x02D6, 0x0001, 0x0 }, /* Reg(726): CagcEmAutoMode bit [0] */
    { StrIp("CagcEmDecMode"),                HCDPRegCagcEmDecMode,                0x02D7, 0x0001, 0x0 }, /* Reg(727): CagcEmDecMode bit [0] */
    { StrIp("CagcEmBlindMode"),              HCDPRegCagcEmBlindMode,              0x02D8, 0x0001, 0x0 }, /* Reg(728): CagcEmBlindMode bit [0] */
    { StrIp("CagcSatResetEn"),               HCDPRegCagcSatResetEn,               0x02D9, 0x0001, 0x0 }, /* Reg(729): CagcSatResetEn bit [0] */
    { StrIp("CtlDisable"),                   HCDPRegCtlDisable,                   0x02E0, 0x0001, 0x0 }, /* Reg(736): CtlDisable bit [0] */
    { StrIp("CtlInMode"),                    HCDPRegCtlInMode,                    0x02E1, 0x0001, 0x0 }, /* Reg(737): CtlInMode bit [0] */
    { StrIp("CtlEmMode"),                    HCDPRegCtlEmMode,                    0x02E2, 0x0001, 0x0 }, /* Reg(738): CtlEmMode bit [0] */
    { StrIp("CtlDdEmGain"),                  HCDPRegCtlDdEmGain,                  0x02E3, 0x0003, 0x0 }, /* Reg(739): CtlDdEmGain bits [1 downto 0] */
    { StrIp("CtlDaEmGain"),                  HCDPRegCtlDaEmGain,                  0x02E4, 0x0003, 0x0 }, /* Reg(740): CtlDaEmGain bits [1 downto 0] */
    { StrIp("CtlAcqGain"),                   HCDPRegCtlAcqGain,                   0x02E5, 0x03FF, 0x0 }, /* Reg(741): CtlAcqGain bits [9 downto 0] */
    { StrIp("CtlAcqDirGain"),                HCDPRegCtlAcqDirGain,                0x02E5, 0x000F, 0x0 }, /* Reg(741): CtlAcqGain bits [3 downto 0] */
    { StrIp("CtlAcqFractGain"),              HCDPRegCtlAcqFractGain,              0x02E5, 0x0030, 0x4 }, /* Reg(741): CtlAcqGain bits [5 downto 4] */
    { StrIp("CtlAcqIndGain"),                HCDPRegCtlAcqIndGain,                0x02E5, 0x03C0, 0x6 }, /* Reg(741): CtlAcqGain bits [9 downto 6] */
    { StrIp("CtlTrkGain"),                   HCDPRegCtlTrkGain,                   0x02E6, 0x03FF, 0x0 }, /* Reg(742): CtlTrkGain bits [9 downto 0] */
    { StrIp("CtlTrkDirGain"),                HCDPRegCtlTrkDirGain,                0x02E6, 0x000F, 0x0 }, /* Reg(742): CtlTrkGain bits [3 downto 0] */
    { StrIp("CtlTrkFractGain"),              HCDPRegCtlTrkFractGain,              0x02E6, 0x0030, 0x4 }, /* Reg(742): CtlTrkGain bits [5 downto 4] */
    { StrIp("CtlTrkIndGain"),                HCDPRegCtlTrkIndGain,                0x02E6, 0x03C0, 0x6 }, /* Reg(742): CtlTrkGain bits [9 downto 6] */
    { StrIp("CtlOctaveScale"),               HCDPRegCtlOctaveScale,               0x02E7, 0x0001, 0x0 }, /* Reg(743): CtlOctaveScale bit [0] */
    { StrIp("CtlEmAutoSw"),                  HCDPRegCtlEmAutoSw,                  0x02E8, 0x0001, 0x0 }, /* Reg(744): CtlEmAutoSw bit [0] */
    { StrIp("CtlAcc"),                       HCDPRegCtlAcc,                       0x02E9, 0xFFFF, 0x0 }, /* Reg(745): CtlAcc bits [15 downto 0] */
    { StrIp("CtlZeroDirectPath"),            HCDPRegCtlZeroDirectPath,            0x02EB, 0x0001, 0x0 }, /* Reg(747): CtlZeroDirectPath bit [0] */
    { StrIp("AfcInMode"),                    HCDPRegAfcInMode,                    0x02F0, 0x0001, 0x0 }, /* Reg(752): AfcInMode bit [0] */
    { StrIp("AfcPhaseThr"),                  HCDPRegAfcPhaseThr,                  0x02F1, 0x00FF, 0x0 }, /* Reg(753): AfcPhaseThr bits [7 downto 0] */
    { StrIp("AfcEnergyThr"),                 HCDPRegAfcEnergyThr,                 0x02F2, 0x03FF, 0x0 }, /* Reg(754): AfcEnergyThr bits [9 downto 0] */
    { StrIp("AfcDisable"),                   HCDPRegAfcDisable,                   0x02F3, 0x0001, 0x0 }, /* Reg(755): AfcDisable bit [0] */
    { StrIp("AfcForce"),                     HCDPRegAfcForce,                     0x02F4, 0x0001, 0x0 }, /* Reg(756): AfcForce bit [0] */
    { StrIp("CtlPhaseEmSel"),                HCDPRegCtlPhaseEmSel,                0x02F5, 0x0001, 0x0 }, /* Reg(757): CtlPhaseEmSel bit [0] */
    { StrIp("CtlFreqEmSel"),                 HCDPRegCtlFreqEmSel,                 0x02F6, 0x0001, 0x0 }, /* Reg(758): CtlFreqEmSel bit [0] */
    { StrIp("AfcUseInnerCircle"),            HCDPRegAfcUseInnerCircle,            0x02F7, 0x0001, 0x0 }, /* Reg(759): AfcUseInnerCircle bit [0] */
    { StrIp("AfcAccelerate"),                HCDPRegAfcAccelerate,                0x02F8, 0x0003, 0x0 }, /* Reg(760): AfcAccelerate bits [1 downto 0] */
    { StrIp("SctlDisable"),                  HCDPRegSctlDisable,                  0x0300, 0x0001, 0x0 }, /* Reg(768): SctlDisable bit [0] */
    { StrIp("SctlAcqGain"),                  HCDPRegSctlAcqDirGain,               0x0301, 0x03F7, 0x0 }, /* Reg(769): SctlAcqGain bits [9 downto 4, 2 downto 0] */
    { StrIp("SctlAcqDirGain"),               HCDPRegSctlAcqDirGain,               0x0301, 0x0007, 0x0 }, /* Reg(769): SctlAcqGain bits [2 downto 0] */
    { StrIp("SctlAcqFractGain"),             HCDPRegSctlAcqFractGain,             0x0301, 0x0030, 0x4 }, /* Reg(769): SctlAcqGain bits [5 downto 4] */
    { StrIp("SctlAcqIndGain"),               HCDPRegSctlAcqIndGain,               0x0301, 0x03C0, 0x6 }, /* Reg(769): SctlAcqGain bits [9 downto 6] */
    { StrIp("SctlTrkGain"),                  HCDPRegSctlTrkGain,                  0x0302, 0x03F7, 0x0 }, /* Reg(770): SctlTrkGain bits [9 downto 4, 2 downto 0] */
    { StrIp("SctlTrkDirGain"),               HCDPRegSctlTrkDirGain,               0x0302, 0x0007, 0x0 }, /* Reg(770): SctlTrkGain bits [2 downto 0] */
    { StrIp("SctlTrkFractGain"),             HCDPRegSctlTrkFractGain,             0x0302, 0x0030, 0x4 }, /* Reg(770): SctlTrkGain bits [5 downto 4] */
    { StrIp("SctlTrkIndGain"),               HCDPRegSctlTrkIndGain,               0x0302, 0x03C0, 0x6 }, /* Reg(770): SctlTrkGain bits [9 downto 6] */
    { StrIp("SctlVcoBypass"),                HCDPRegSctlVcoBypass,                0x0304, 0x0001, 0x0 }, /* Reg(772): SctlVcoBypass bit [0] */
    { StrIp("SctlAcc"),                      HCDPRegSctlAcc,                      0x0305, 0x03FF, 0x0 }, /* Reg(773): SctlAcc bits [9 downto 0] */
    { StrIp("SctlZeroDirectPath"),           HCDPRegSctlZeroDirectPath,           0x0306, 0x0001, 0x0 }, /* Reg(774): SctlZeroDirectPath bit [0] */
    { StrIp("BedrBypass"),                   HCDPRegBedrBypass,                   0x0310, 0x0001, 0x0 }, /* Reg(784): BedrBypass bit [0] */
    { StrIp("BedrPhaseGain"),                HCDPRegBedrPhaseGain,                0x0311, 0x0003, 0x0 }, /* Reg(785): BedrPhaseGain bits [1 downto 0] */
    { StrIp("BedrTrkOnly"),                  HCDPRegBedrTrkOnly,                  0x0312, 0x0001, 0x0 }, /* Reg(786): BedrTrkOnly bit [0] */
    { StrIp("SctlMagGain1"),                 HCDPRegSctlMagGain1,                 0x0320, 0x0003, 0x0 }, /* Reg(800): SctlMagGain1 bits [1 downto 0] */
    { StrIp("SctlMagGain2"),                 HCDPRegSctlMagGain2,                 0x0321, 0x0003, 0x0 }, /* Reg(801): SctlMagGain2 bits [1 downto 0] */
    { StrIp("CtlMagGain1"),                  HCDPRegCtlMagGain1,                  0x0322, 0x0003, 0x0 }, /* Reg(802): CtlMagGain1 bits [1 downto 0] */
    { StrIp("CtlMagGain2"),                  HCDPRegCtlMagGain2,                  0x0323, 0x0003, 0x0 }, /* Reg(803): CtlMagGain2 bits [1 downto 0] */
    { StrIp("SctlIncGain"),                  HCDPRegSctlIncGain,                  0x0324, 0x0003, 0x0 }, /* Reg(804): SctlIncGain bits [1 downto 0] */
    { StrIp("SctlDecGain"),                  HCDPRegSctlDecGain,                  0x0325, 0x0003, 0x0 }, /* Reg(805): SctlDecGain bits [1 downto 0] */
    { StrIp("CtlIncGain"),                   HCDPRegCtlIncGain,                   0x0326, 0x0003, 0x0 }, /* Reg(806): CtlIncGain bits [1 downto 0] */
    { StrIp("CtlDecGain"),                   HCDPRegCtlDecGain,                   0x0327, 0x0003, 0x0 }, /* Reg(807): CtlDecGain bits [1 downto 0] */
    { StrIp("SctlLockEmGain"),               HCDPRegSctlLockEmGain,               0x0328, 0x0003, 0x0 }, /* Reg(808): SctlLockEmGain bits [1 downto 0] */
    { StrIp("CtlLockEmGain"),                HCDPRegCtlLockEmGain,                0x0329, 0x0003, 0x0 }, /* Reg(809): CtlLockEmGain bits [1 downto 0] */
    { StrIp("SctlLockAcc"),                  HCDPRegSctlLockAcc,                  0x032A, 0x1FFF, 0x0 }, /* Reg(810): SctlLockAcc bits [12 downto 0] */
    { StrIp("CtlLockAcc"),                   HCDPRegCtlLockAcc,                   0x032B, 0x1FFF, 0x0 }, /* Reg(811): CtlLockAcc bits [12 downto 0] */
    { StrIp("SctlNearLockThr"),              HCDPRegSctlNearLockThr,              0x032C, 0x00FF, 0x0 }, /* Reg(812): SctlNearLockThr bits [7 downto 0] */
    { StrIp("SctlNearFadeThr"),              HCDPRegSctlNearFadeThr,              0x032D, 0x00FF, 0x0 }, /* Reg(813): SctlNearFadeThr bits [7 downto 0] */
    { StrIp("DemodSctlLockMask"),            HCDPRegDemodSctlLockMask,            0x032F, 0x0002, 0x1 }, /* Reg(815): DemodLockMask bit [1] */
    { StrIp("DemodSctlNearLockMask"),        HCDPRegDemodSctlNearLockMask,        0x032F, 0x0004, 0x2 }, /* Reg(815): DemodLockMask bit [2] */
    { StrIp("DemodCtlLockMask"),             HCDPRegDemodCtlLockMask,             0x032F, 0x0008, 0x3 }, /* Reg(815): DemodLockMask bit [3] */
    { StrIp("DemodStlLockMask"),             HCDPRegDemodStlLockMask,             0x032F, 0x0010, 0x4 }, /* Reg(815): DemodLockMask bit [4] */
    { StrIp("DemodMseLockMask"),             HCDPRegDemodMseLockMask,             0x032F, 0x0020, 0x5 }, /* Reg(815): DemodLockMask bit [5] */
    { StrIp("DemodAagcLockMask"),            HCDPRegDemodAagcLockMask,            0x032F, 0x0040, 0x6 }, /* Reg(815): DemodLockMask bit [6] */
    { StrIp("DemodDagcSatMask"),             HCDPRegDemodDagcSatMask,             0x032F, 0x0200, 0x9 }, /* Reg(815): DemodLockMask bit [9] */
    { StrIp("DemodCagcSatMask"),             HCDPRegDemodCagcSatMask,             0x032F, 0x0400, 0xA }, /* Reg(815): DemodLockMask bit [10] */
    { StrIp("LockStatus"),                   HCDPRegLockStatus,                   0x0330, 0xFFFF, 0x0 }, /* Reg(816): LockStatus bit [15 downto 0] */
    { StrIp("LockInd"),                      HCDPRegLockInd,                      0x0330, 0x0001, 0x0 }, /* Reg(816): LockStatus bit [0] */
    { StrIp("SctlLockInd"),                  HCDPRegSctlLockInd,                  0x0330, 0x0002, 0x1 }, /* Reg(816): LockStatus bit [1] */
    { StrIp("SctlNearLockInd"),              HCDPRegSctlNearLockInd,              0x0330, 0x0004, 0x2 }, /* Reg(816): LockStatus bit [2] */
    { StrIp("CtlLockInd"),                   HCDPRegCtlLockInd,                   0x0330, 0x0008, 0x3 }, /* Reg(816): LockStatus bit [3] */
    { StrIp("StlLockInd"),                   HCDPRegStlLockInd,                   0x0330, 0x0010, 0x4 }, /* Reg(816): LockStatus bit [4] */
    { StrIp("MseLockInd"),                   HCDPRegMseLockInd,                   0x0330, 0x0020, 0x5 }, /* Reg(816): LockStatus bit [5] */
    { StrIp("AagcLockFlag"),                 HCDPRegAagcLockFlag,                 0x0330, 0x0040, 0x6 }, /* Reg(816): LockStatus bit [6] */
    { StrIp("DagcSatInd"),                   HCDPRegDagcSatInd,                   0x0330, 0x0200, 0x9 }, /* Reg(816): LockStatus bit [9] */
    { StrIp("CagcSatInd"),                   HCDPRegCagcSatInd,                   0x0330, 0x0400, 0xA }, /* Reg(816): LockStatus bit [10] */
    { StrIp("Lock2UtlInd"),                  HCDPRegLock2UtlInd,                  0x0330, 0x0800, 0xB }, /* Reg(816): LockStatus bit [11] */
    { StrIp("DemodLockInd"),                 HCDPRegDemodLockInd,                 0x0330, 0x1000, 0xC }, /* Reg(816): LockStatus bit [12] */
    { StrIp("SctlLockMask"),                 HCDPRegSctlLockMask,                 0x0331, 0x0002, 0x1 }, /* Reg(817): LockMask bit [1] */
    { StrIp("SctlNearLockMask"),             HCDPRegSctlNearLockMask,             0x0331, 0x0004, 0x2 }, /* Reg(817): LockMask bit [2] */
    { StrIp("CtlLockMask"),                  HCDPRegCtlLockMask,                  0x0331, 0x0008, 0x3 }, /* Reg(817): LockMask bit [3] */
    { StrIp("StlLockMask"),                  HCDPRegStlLockMask,                  0x0331, 0x0010, 0x4 }, /* Reg(817): LockMask bit [4] */
    { StrIp("MseLockMask"),                  HCDPRegMseLockMask,                  0x0331, 0x0020, 0x5 }, /* Reg(817): LockMask bit [5] */
    { StrIp("AagcLockMask"),                 HCDPRegAagcLockMask,                 0x0331, 0x0040, 0x6 }, /* Reg(817): LockMask bit [6] */
    { StrIp("DagcSatMask"),                  HCDPRegDagcSatMask,                  0x0331, 0x0200, 0x9 }, /* Reg(817): LockMask bit [9] */
    { StrIp("CagcSatMask"),                  HCDPRegCagcSatMask,                  0x0331, 0x0400, 0xA }, /* Reg(817): LockMask bit [10] */
    { StrIp("ForceLock"),                    HCDPRegForceLock,                    0x0332, 0x0001, 0x0 }, /* Reg(818): ForceLock bit [0] */
    { StrIp("ForceNoLock"),                  HCDPRegForceNoLock,                  0x0333, 0x0001, 0x0 }, /* Reg(819): ForceNoLock bit [0] */
    { StrIp("SctlLockMask2Utl"),             HCDPRegSctlLockMask2Utl,             0x0334, 0x0002, 0x1 }, /* Reg(820): LockMask2Utl bit [1] */
    { StrIp("SctlNearLockMask2Utl"),         HCDPRegSctlNearLockMask2Utl,         0x0334, 0x0004, 0x2 }, /* Reg(820): LockMask2Utl bit [2] */
    { StrIp("StlLockMask2Utl"),              HCDPRegStlLockMask2Utl,              0x0334, 0x0010, 0x4 }, /* Reg(820): LockMask2Utl bit [4] */
    { StrIp("ForceLock2Utl"),                HCDPRegForceLock2Utl,                0x0335, 0x0001, 0x0 }, /* Reg(821): ForceLock2Utl bit [0] */
    { StrIp("ForceNoLock2Utl"),              HCDPRegForceNoLock2Utl,              0x0336, 0x0001, 0x0 }, /* Reg(822): ForceNoLock2Utl bit [0] */
    { StrIp("DemodVersion"),                 HCDPRegDemodVersion,                 0x0340, 0xFFFF, 0x0 }, /* Reg(832): DemodVersion bits [15 downto 0] */
    { StrIp("ConstellationIsReal"),          HCDPRegConstellationIsReal,          0x0341, 0x0001, 0x0 }, /* Reg(833): Constellation bit [0] */
    { StrIp("ConstellationData"),            HCDPRegConstellationData,            0x0341, 0xFFFF, 0x0 }, /* Reg(833): Constellation bits [15 downto 1] */
    { StrIp("ConstellationCtrl"),            HCDPRegConstellationCtrl,            0x0342, 0x0007, 0x0 }, /* Reg(834): ConstellationCtrl bits [2 downto 0] */
    { StrIp("DemodOutRotate"),               HCDPRegDemodOutRotate,               0x0343, 0x0003, 0x0 }, /* Reg(835): DemodOutRotate bits [1 downto 0] */
    { StrIp("DemodOutDifferential"),         HCDPRegDemodOutDifferential,         0x0344, 0x0001, 0x0 }, /* Reg(836): DemodOutDifferential bit [0] */
    { StrIp("DemodOutEuroMapping"),          HCDPRegDemodOutEuroMapping,          0x0345, 0x0001, 0x0 }, /* Reg(837): DemodOutEuroMapping bit [0] */
    { StrIp("DemodReset"),                   HCDPRegDemodReset,                   0x346,  0xFFFF, 0x0},
    { StrIp("DemodSoftReset"),               HCDPRegDemodSoftReset,               0x0346, 0x0001, 0x0 }, /* Reg(838): DemodReset bit [0] */
    { StrIp("DemodMicroReset"),              HCDPRegDemodMicroReset,              0x0346, 0x0002, 0x1 }, /* Reg(838): DemodReset bit [1] */
    { StrIp("DagcAccReset"),                 HCDPRegDagcAccReset,                 0x0346, 0x0010, 0x4 }, /* Reg(838): DemodReset bit [4] */
    { StrIp("CagcAccReset"),                 HCDPRegCagcAccReset,                 0x0346, 0x0020, 0x5 }, /* Reg(838): DemodReset bit [5] */
    { StrIp("StlAccReset"),                  HCDPRegStlAccReset,                  0x0346, 0x0040, 0x6 }, /* Reg(838): DemodReset bit [6] */
    { StrIp("CtlAccReset"),                  HCDPRegCtlAccReset,                  0x0346, 0x0080, 0x7 }, /* Reg(838): DemodReset bit [7] */
    { StrIp("SctlAccReset"),                 HCDPRegSctlAccReset,                 0x0346, 0x0100, 0x8 }, /* Reg(838): DemodReset bit [8] */
    { StrIp("StlLockAccReset"),              HCDPRegStlLockAccReset,              0x0346, 0x0400, 0xA }, /* Reg(838): DemodReset bit [10] */
    { StrIp("CtlLockAccReset"),              HCDPRegCtlLockAccReset,              0x0346, 0x0800, 0xB }, /* Reg(838): DemodReset bit [11] */
    { StrIp("SctlLockAccReset"),             HCDPRegSctlLockAccReset,             0x0346, 0x1000, 0xC }, /* Reg(838): DemodReset bit [12] */
    { StrIp("MseLockReset"),                 HCDPRegMseLockReset,                 0x0346, 0x2000, 0xD }, /* Reg(838): DemodReset bit [13] */
    { StrIp("DemodAssociateDisable"),        HCDPRegDemodAssociateDisable,        0x0348, 0x0001, 0x0 }, /* Reg(840): DemodAssociateDisable bit [0] */
    { StrIp("EqBypass"),                     HCDPRegEqBypass,                     0x0350, 0x0003, 0x0 }, /* Reg(848): EqBypass bits [1 downto 0] */
    { StrIp("EqDisable"),                    HCDPRegEqDisable,                    0x0351, 0x0001, 0x0 }, /* Reg(849): EqDisable bit [0] */
    { StrIp("EqReset"),                      HCDPRegEqReset,                      0x0352, 0x0001, 0x0 }, /* Reg(850): EqReset bit [0] */
    { StrIp("EqTspaceMode"),                 HCDPRegEqTspaceMode,                 0x0352, 0x0010, 0x4 }, /* Reg(850): EqReset bit [4] */
    { StrIp("EqFreezeMode"),                 HCDPRegEqFreezeMode,                 0x0352, 0x0100, 0x8 }, /* Reg(850): EqReset bit [8] */
    { StrIp("EqCenterTapGain"),              HCDPRegEqCenterTapGain,              0x0353, 0x0001, 0x0 }, /* Reg(851): EqCenterTapGain bit [0] */
    { StrIp("EqFbScale"),                    HCDPRegEqFbScale,                    0x0354, 0x0007, 0x0 }, /* Reg(852): EqFbScale bits [2 downto 0] */
    { StrIp("EqEfScale"),                    HCDPRegEqEfScale,                    0x0355, 0x0007, 0x0 }, /* Reg(853): EqEfScale bits [2 downto 0] */
    { StrIp("EqBlindThrA"),                  HCDPRegEqBlindThrA,                  0x0356, 0x03FF, 0x0 }, /* Reg(854): EqBlindThrA bits [9 downto 0] */
    { StrIp("EqBlindThrB"),                  HCDPRegEqBlindThrB,                  0x0357, 0x03FF, 0x0 }, /* Reg(855): EqBlindThrB bits [9 downto 0] */
    { StrIp("EqBlindThrC"),                  HCDPRegEqBlindThrC,                  0x0358, 0x03FF, 0x0 }, /* Reg(856): EqBlindThrC bits [9 downto 0] */
    { StrIp("EqBlindThrD"),                  HCDPRegEqBlindThrD,                  0x0359, 0x03FF, 0x0 }, /* Reg(857): EqBlindThrD bits [9 downto 0] */
    { StrIp("EqAcqAdaptMode"),               HCDPRegEqAcqAdaptMode,               0x035A, 0x0003, 0x0 }, /* Reg(858): EqAcqAdaptMode bits [1 downto 0] */
    { StrIp("EqTrkAdaptMode"),               HCDPRegEqTrkAdaptMode,               0x035B, 0x0003, 0x0 }, /* Reg(859): EqTrkAdaptMode bits [1 downto 0] */
    { StrIp("EqAdaptModeAutoSw"),            HCDPRegEqAdaptModeAutoSw,            0x035C, 0x0001, 0x0 }, /* Reg(860): EqAdaptModeAutoSw bit [0] */
    { StrIp("EqAdaptGainAutoSw"),            HCDPRegEqAdaptGainAutoSw,            0x035D, 0x0001, 0x0 }, /* Reg(861): EqAdaptGainAutoSw bit [0] */
    { StrIp("EqAcqFfAdaptGain"),             HCDPRegEqAcqFfAdaptGain,             0x035E, 0x000F, 0x0 }, /* Reg(862): EqAcqAdaptGain bits [3 downto 0] */
    { StrIp("EqAcqFfHsymAdaptGain"),         HCDPRegEqAcqFfHsymAdaptGain,         0x035E, 0x00F0, 0x4 }, /* Reg(862): EqAcqAdaptGain bits [7 downto 4] */
    { StrIp("EqAcqFbAdaptGain"),             HCDPRegEqAcqFbAdaptGain,             0x035E, 0x0F00, 0x8 }, /* Reg(862): EqAcqAdaptGain bits [11 downto 8] */
    { StrIp("EqAcqEfAdaptGain"),             HCDPRegEqAcqEfAdaptGain,             0x035E, 0xF000, 0xC }, /* Reg(862): EqAcqAdaptGain bits [15 downto 12] */
    { StrIp("EqTrkFfAdaptGain"),             HCDPRegEqTrkFfAdaptGain,             0x035F, 0x000F, 0x0 }, /* Reg(863): EqTrkAdaptGain bits [3 downto 0] */
    { StrIp("EqTrkFfHsymAdaptGain"),         HCDPRegEqTrkFfHsymAdaptGain,         0x035F, 0x00F0, 0x4 }, /* Reg(863): EqTrkAdaptGain bits [7 downto 4] */
    { StrIp("EqTrkFbAdaptGain"),             HCDPRegEqTrkFbAdaptGain,             0x035F, 0x0F00, 0x8 }, /* Reg(863): EqTrkAdaptGain bits [11 downto 8] */
    { StrIp("EqTrkEfAdaptGain"),             HCDPRegEqTrkEfAdaptGain,             0x035F, 0xF000, 0xC }, /* Reg(863): EqTrkAdaptGain bits [15 downto 12] */
    { StrIp("EqHsymLmsWeight"),              HCDPRegEqHsymLmsWeight,              0x0360, 0x0001, 0x0 }, /* Reg(864): EqHsymLmsWeight bit [0] */
    { StrIp("EqAcqHsymFreeze"),              HCDPRegEqAcqHsymFreeze,              0x0361, 0x0001, 0x0 }, /* Reg(865): EqAcqHsymFreeze bit [0] */
    { StrIp("EqBlindThrScma"),               HCDPRegEqBlindThrScma,               0x0362, 0x03FF, 0x0 }, /* Reg(866): EqBlindThrScma bits [9 downto 0] */
    { StrIp("EqFfLeakageEnable"),            HCDPRegEqFfLeakageEnable,            0x0364, 0x0001, 0x0 }, /* Reg(868): EqFfLeakageEnable bit [0] */
    { StrIp("EqFfLeakageAlpha"),             HCDPRegEqFfLeakageAlpha,             0x0365, 0x000F, 0x0 }, /* Reg(869): EqFfLeakageAlpha bits [3 downto 0] */
    { StrIp("EqFbLeakageEnable"),            HCDPRegEqFbLeakageEnable,            0x0366, 0x0001, 0x0 }, /* Reg(870): EqFbLeakageEnable bit [0] */
    { StrIp("EqFbLeakageAlpha"),             HCDPRegEqFbLeakageAlpha,             0x0367, 0x000F, 0x0 }, /* Reg(871): EqFbLeakageAlpha bits [3 downto 0] */
    { StrIp("EqEfDisable"),                  HCDPRegEqEfDisable,                  0x0368, 0x0001, 0x0 }, /* Reg(872): EqEfDisable bit [0] */
    { StrIp("EqEfLeakageEnable"),            HCDPRegEqEfLeakageEnable,            0x0369, 0x0001, 0x0 }, /* Reg(873): EqEfLeakageEnable bit [0] */
    { StrIp("EqEfLeakageAlpha"),             HCDPRegEqEfLeakageAlpha,             0x036A, 0x000F, 0x0 }, /* Reg(874): EqEfLeakageAlpha bits [3 downto 0] */
    { StrIp("EqAdaptFreeze0"),               HCDPRegEqAdaptFreeze0,               0x036B, 0xFFFF, 0x0 }, /* Reg(875): EqAdaptFreeze0 bits [15 downto 0] */
    { StrIp("EqAdaptFreeze1"),               HCDPRegEqAdaptFreeze1,               0x036C, 0xFFFF, 0x0 }, /* Reg(876): EqAdaptFreeze1 bits [15 downto 0] */
    { StrIp("EqAdaptFreeze2"),               HCDPRegEqAdaptFreeze2,               0x036D, 0xFFFF, 0x0 }, /* Reg(877): EqAdaptFreeze2 bits [15 downto 0] */
    { StrIp("EqAdaptFreeze3"),               HCDPRegEqAdaptFreeze3,               0x036E, 0xFFFF, 0x0 }, /* Reg(878): EqAdaptFreeze3 bits [15 downto 0] */
    { StrIp("EqDfePwr"),                     HCDPRegEqDfePwr,                     0x036F, 0xFFFF, 0x0 }, /* Reg(879): EqDfePwr bits [15 downto 0] */
    { StrIp("EqEfePwr"),                     HCDPRegEqEfePwr,                     0x0370, 0xFFFF, 0x0 }, /* Reg(880): EqEfePwr bits [15 downto 0] */
    { StrIp("EqTapPowerStart"),              HCDPRegEqTapPowerStart,              0x0371, 0x0001, 0x0 }, /* Reg(881): EqTapPower bit [0] */
    { StrIp("EqTapPowerReady"),              HCDPRegEqTapPowerReady,              0x0371, 0x0002, 0x1 }, /* Reg(881): EqTapPower bit [1] */
    { StrIp("EqCoefNumber"),                 HCDPRegEqCoefNumber,                 0x0373, 0x003F, 0x0 }, /* Reg(883): EqCoefControl bits [5 downto 0] */
    { StrIp("EqCoefIsReal"),                 HCDPRegEqCoefIsReal,                 0x0373, 0x0040, 0x6 }, /* Reg(883): EqCoefControl bit [6] */
    { StrIp("EqCoefIsHigh"),                 HCDPRegEqCoefIsHigh,                 0x0373, 0x0080, 0x7 }, /* Reg(883): EqCoefControl bit [7] */
    { StrIp("EqCoefWrValid"),                HCDPRegEqCoefWrValid,                0x0373, 0x4000, 0xE }, /* Reg(883): EqCoefControl bit [14] */
    { StrIp("EqCoefRdValid"),                HCDPRegEqCoefRdValid,                0x0373, 0x8000, 0xF }, /* Reg(883): EqCoefControl bit [15] */
    { StrIp("EqCoefData"),                   HCDPRegEqCoefData,                   0x0374, 0x1FFF, 0x0 }, /* Reg(884): EqCoefData bits [12 downto 0] */
    { StrIp("Clk16ChangeEn"),                HCDPRegClk16ChangeEn,                0x03E0, 0x0001, 0x0 }, /* Reg(992): Clk16Control bit [0] */
    { StrIp("Clk16FindValueDisable"),        HCDPRegClk16FindValueDisable,        0x03E0, 0x0002, 0x1 }, /* Reg(992): Clk16Control bit [1] */
    { StrIp("Clk16Value"),                   HCDPRegClk16Value,                   0x03E1, 0x03FF, 0x0 }, /* Reg(993): Clk16Value bits [9 downto 0] */
    { StrIp("Clk16ValueGain"),               HCDPRegClk16ValueGain,               0x03E1, 0x1000, 0xC }, /* Reg(993): Clk16Value bit [12] */
    { StrIp("Clk16Phase"),                   HCDPRegClk16Phase,                   0x03E2, 0x003F, 0x0 }, /* Reg(994): Clk16Status bits [5 downto 0] */
    { StrIp("Clk16Freq"),                    HCDPRegClk16Freq,                    0x03E2, 0x0F00, 0x8 }, /* Reg(994): Clk16Status bits [11 downto 8] */
    { StrIp("Clk16KickCount"),               HCDPRegClk16KickCount,               0x03E2, 0x3000, 0xC }, /* Reg(994): Clk16Status bits [13 downto 12] */
    { StrIp("DemodTestEn"),                  HCDPRegDemodTestEn,                  0x03F0, 0x0001, 0x0 }, /* Reg(1008): DemodTestEn bit [0] */
    { StrIp("AdcTestMode"),                  HCDPRegAdcTestMode,                  0x03F1, 0x0003, 0x0 }, /* Reg(1009): AdcTestMode bits [1 downto 0] */
    { StrIp("DemodLogicCrcDone"),            HCDPRegDemodLogicCrcDone,            0x03F2, 0x0001, 0x0 }, /* Reg(1010): DemodLogicCrcDone bit [0] */
    { StrIp("DemodLogicCrcIData"),           HCDPRegDemodLogicCrcIData,           0x03F3, 0x00FF, 0x0 }, /* Reg(1011): DemodLogicCrcData bits [15 downto 0] */
    { StrIp("DemodLogicCrcQData"),           HCDPRegDemodLogicCrcQData,           0x03F3, 0xFF00, 0x8 }, /* Reg(1011): DemodLogicCrcData bits [15 downto 0] */
    { StrIp("CtlOpenLoop"),                  HCDPRegCtlOpenLoop,                  0x03F4, 0x0001, 0x0 }, /* Reg(1012): DemodOpenLoopCtrl bit [0] */
    { StrIp("StlOpenLoop"),                  HCDPRegStlOpenLoop,                  0x03F4, 0x0010, 0x4 }, /* Reg(1012): DemodOpenLoopCtrl bit [4] */
    { StrIp("CagcOpenLoop"),                 HCDPRegCagcOpenLoop,                 0x03F4, 0x0100, 0x8 }, /* Reg(1012): DemodOpenLoopCtrl bit [8] */
    { StrIp("SctlOpenLoop"),                 HCDPRegSctlOpenLoop,                 0x03F4, 0x1000, 0xC }, /* Reg(1012): DemodOpenLoopCtrl bit [12] */
    { StrIp("PdfSym2MinGap"),                HCDPRegPdfSym2MinGap,                0x03F5, 0x000F, 0x0 }, /* Reg(1013): PdfSym2Gap bits [3 downto 0] */
    { StrIp("PdfSym2MaxGap"),                HCDPRegPdfSym2MaxGap,                0x03F5, 0x00F0, 0x4 }, /* Reg(1013): PdfSym2Gap bits [7 downto 4] */
    { StrIp("DemodOutSpecInvState"),         HCDPRegDemodOutSpecInvState,         0x03F6, 0x0001, 0x0 }, /* Reg(1014): DemodOutSpecInvCtrl bit [0] */
    { StrIp("DemodOutNxaSync"),              HCDPRegDemodOutNxaSync,              0x03F6, 0x0002, 0x1 }, /* Reg(1014): DemodOutSpecInvCtrl bit [1] */
    { StrIp("DemodOutLock"),                 HCDPRegDemodOutLock,                 0x03F6, 0x0004, 0x2 }, /* Reg(1014): DemodOutSpecInvCtrl bit [2] */
    { StrIp("DemodOutManualSpecInv"),        HCDPRegDemodOutManualSpecInv,        0x03F6, 0x0010, 0x4 }, /* Reg(1014): DemodOutSpecInvCtrl bit [4] */
    //{ StrIp("EqCmemMbistEnable"),            HCDPRegEqCmemMbistEnable,            0x03F8, 0x0001, 0x0 }, /* Reg(1016): EqCmemMbistCtrl1 bit [0] */
    //{ StrIp("EqCmemMbistStart"),             HCDPRegEqCmemMbistStart,             0x03F8, 0x0010, 0x4 }, /* Reg(1016): EqCmemMbistCtrl1 bit [4] */
    //{ StrIp("EqCmemMbistMemSelect"),         HCDPRegEqCmemMbistMemSelect,         0x03F8, 0x0700, 0x8 }, /* Reg(1016): EqCmemMbistCtrl1 bits [10 downto 8] */
    //{ StrIp("EqCmemMbistMinAddr"),           HCDPRegEqCmemMbistMinAddr,           0x03F9, 0x000F, 0x0 }, /* Reg(1017): EqCmemMbistCtrl2 bits [3 downto 0] */
    //{ StrIp("EqCmemMbistRetentionMsb"),      HCDPRegEqCmemMbistRetentionMsb,      0x03F9, 0x00F0, 0x4 }, /* Reg(1017): EqCmemMbistCtrl2 bits [7 downto 4] */
    //{ StrIp("EqCmemMbistZeroMem"),           HCDPRegEqCmemMbistZeroMem,           0x03F9, 0x0100, 0x8 }, /* Reg(1017): EqCmemMbistCtrl2 bit [8] */
    //{ StrIp("EqCmemMbistDone"),              HCDPRegEqCmemMbistDone,              0x03FA, 0x0001, 0x0 }, /* Reg(1018): EqCmemMbistStatus bit [0] */
    //{ StrIp("EqCmemMbistFail"),              HCDPRegEqCmemMbistFail,              0x03FA, 0x0002, 0x1 }, /* Reg(1018): EqCmemMbistStatus bit [1] */
    //{ StrIp("EqCmemMbistFailAddr"),          HCDPRegEqCmemMbistFailAddr,          0x03FA, 0x00F0, 0x4 }, /* Reg(1018): EqCmemMbistStatus bits [7 downto 4] */
    //{ StrIp("EqCmemMbistFailBit"),           HCDPRegEqCmemMbistFailBit,           0x03FA, 0x7F00, 0x8 }, /* Reg(1018): EqCmemMbistStatus bits [14 downto 8] */
    //{ StrIp("EqSmemMbistEnable"),            HCDPRegEqSmemMbistEnable,            0x03FB, 0x0001, 0x0 }, /* Reg(1019): EqSmemMbistCtrl1 bit [0] */
    //{ StrIp("EqSmemMbistStart"),             HCDPRegEqSmemMbistStart,             0x03FB, 0x0010, 0x4 }, /* Reg(1019): EqSmemMbistCtrl1 bit [4] */
    //{ StrIp("EqSmemMbistMemSelect"),         HCDPRegEqSmemMbistMemSelect,         0x03FB, 0x0300, 0x8 }, /* Reg(1019): EqSmemMbistCtrl1 bits [9 downto 8] */
    //{ StrIp("EqSmemMbistMinAddr"),           HCDPRegEqSmemMbistMinAddr,           0x03FC, 0x000F, 0x0 }, /* Reg(1020): EqSmemMbistCtrl2 bits [3 downto 0] */
    //{ StrIp("EqSmemMbistRetentionMsb"),      HCDPRegEqSmemMbistRetentionMsb,      0x03FC, 0x00F0, 0x4 }, /* Reg(1020): EqSmemMbistCtrl2 bits [7 downto 4] */
    //{ StrIp("EqSmemMbistZeroMem"),           HCDPRegEqSmemMbistZeroMem,           0x03FC, 0x0100, 0x8 }, /* Reg(1020): EqSmemMbistCtrl2 bit [8] */
    //{ StrIp("EqSmemMbistDone"),              HCDPRegEqSmemMbistDone,              0x03FD, 0x0001, 0x0 }, /* Reg(1021): EqSmemMbistStatus bit [0] */
    //{ StrIp("EqSmemMbistFail"),              HCDPRegEqSmemMbistFail,              0x03FD, 0x0002, 0x1 }, /* Reg(1021): EqSmemMbistStatus bit [1] */
    //{ StrIp("EqSmemMbistFailAddr"),          HCDPRegEqSmemMbistFailAddr,          0x03FD, 0x00F0, 0x4 }, /* Reg(1021): EqSmemMbistStatus bits [7 downto 4] */
    //{ StrIp("EqSmemMbistFailBit"),           HCDPRegEqSmemMbistFailBit,           0x03FD, 0x3F00, 0x8 }, /* Reg(1021): EqSmemMbistStatus bits [13 downto 8] */
    { StrIp("DemodWriteError"),              HCDPRegDemodWriteError,              0x03FF, 0x0001, 0x0 }, /* Reg(1023): DemodError bit [0] */
    { StrIp("DemodReadError"),               HCDPRegDemodReadError,               0x03FF, 0x0002, 0x1 }, /* Reg(1023): DemodError bit [1] */
};

#ifdef __cplusplus
}
#endif  /*__cplusplus*/

#endif  /*_TMBSL_HCDP_REG_H*/
