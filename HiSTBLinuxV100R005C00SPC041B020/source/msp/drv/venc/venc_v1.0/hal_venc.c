#include "hal_venc.h"
#include "drv_venc_efl.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif

HI_VOID VENC_HAL_ClrAllInt(S_VEDU_REGS_TYPE *pVeduReg)
{
    pVeduReg->VEDU_INTCLR.u32 = 0xFFFFFFFF;
}

HI_VOID VENC_HAL_DisableAllInt(S_VEDU_REGS_TYPE *pVeduReg)
{
    pVeduReg->VEDU_INTMASK.u32 = 0;
}

#ifdef HI_SMMU_SUPPORT
/******************************************************************************
Function   :
Description:
Calls      :
Input      :
Output     : read back IntStat & rate control register  for smmu
Return     :
Others     :
******************************************************************************/
HI_VOID VENC_HAL_ReadReg_Smmu( VeduEfl_EncPara_S* EncHandle )
{
    VeduEfl_EncPara_S* pEncPara = EncHandle;

    S_VEDU_REGS_TYPE *pAllReg = (S_VEDU_REGS_TYPE *)pEncPara->pRegBase;

    /* read IntStat & RC reg */
    pEncPara->intns_tlbmiss_stat      = pAllReg->SMMU_INTSTAT_NS.bits.intns_tlbmiss_stat;
    pEncPara->intns_ptw_trans_stat  = pAllReg->SMMU_INTSTAT_NS.bits.intns_ptw_trans_stat;
    pEncPara->intns_tlbinvalid_stat   = pAllReg->SMMU_INTSTAT_NS.bits.intns_tlbinvalid_stat;                       //新增  liminqi
    
    /* clear vedu int */
    pAllReg->SMMU_INTCLR_NS.u32 = 0xFFFFFFFF;
}

HI_S32 VENC_HAL_CfgSmmuReg( VeduEfl_EncPara_S* EncHandle)
{
   VeduEfl_EncPara_S* pEncPara = EncHandle;
   
   S_VEDU_REGS_TYPE  *pAllReg = (S_VEDU_REGS_TYPE  *)pEncPara->pRegBase;
   
   pAllReg->SMMU_SCB_TTBR   = pEncPara->u32SmmuPageBaseAddr;
   pAllReg->SMMU_CB_TTBR    = pEncPara->u32SmmuPageBaseAddr;
   pAllReg->SMMU_ERR_RDADDR = pEncPara->u32SmmuErrReadAddr;
   pAllReg->SMMU_ERR_WRADDR = pEncPara->u32SmmuErrWriteAddr;
   return HI_SUCCESS;
}
 
#endif


/******************************************************************************
Function   :
Description:
Calls      :
Input      :
Output     : read back IntStat & rate control register
Return     :
Others     :
******************************************************************************/
HI_VOID VENC_HAL_ReadReg( VeduEfl_EncPara_S* EncHandle )
{
    VeduEfl_EncPara_S* pEncPara = EncHandle;

    S_VEDU_REGS_TYPE *pAllReg = (S_VEDU_REGS_TYPE *)pEncPara->pRegBase;

    /* read IntStat & RC reg */
    pEncPara->VencEndOfPic = pAllReg->VEDU_INTSTAT.bits.VencEndOfPic;
    pEncPara->VencBufFull = pAllReg->VEDU_INTSTAT.bits.VencBufFull;
    pEncPara->VencPbitOverflow = pAllReg->VEDU_INTSTAT.bits.VencPbitOverflow;                          //新增  liminqi
    
    if (!pEncPara->VencBufFull && !pEncPara->VencPbitOverflow)
    {
        pEncPara->MeanQP        = pAllReg->VEDU_MEANQP.bits.meanQp;
        pEncPara->PicBits       = pAllReg->VEDU_PICBITS;                   //当前图像整帧实际编码bit数
        pEncPara->TotalMbhBits  = pAllReg->VEDU_TOTALMBHBITS;              //码率控制需要的整帧图像的mbheader的bit数
        pEncPara->TotalTxtBits  = pAllReg->VEDU_TOTALTXTBITS;              //码率控制需要的整帧图像的纹理bit数
        pEncPara->TotalTxtCost0 = pAllReg->VEDU_TOTALCOST0;                //码率控制需要的整帧图像的纹理cost低32bits
        pEncPara->TotalTxtCost1 = pAllReg->VEDU_TOTALCOST1;                //码率控制需要的整帧图像的纹理cost高32bits
///////////add by ckf77439

        pEncPara->stRc.NumIMBCurFrm =  pAllReg->VEDU_PICINFO1.u32
                                     + pAllReg->VEDU_PICINFO6.u32 
                                     + pAllReg->VEDU_PICINFO7.u32 
                                     + pAllReg->VEDU_PICINFO8.u32;

////////////add end
        pEncPara->Timer         = pAllReg->VEDU_TIMER;                     //
        pEncPara->IdleTimer     = pAllReg->VEDU_IDLE_TIMER;                //
#ifdef __VENC_S40V200_CONFIG__
        pEncPara->CrefldReqNum  = pAllReg->VEDU_CREFLD_MISSNUM;
        pEncPara->CrefldDatNum  = pAllReg->VEDU_CREFLD_HITNUM;
#endif        
	    //pEncPara->CfgErrIndex   = pAllReg->VEDU_CFGERR_INDEX;
	    pEncPara->IntraCloseCnt = pAllReg->VEDU_LOWPOW_CNT0;               //
	    pEncPara->IntpSearchCnt = pAllReg->VEDU_LOWPOW_CNT2;               //
	    pEncPara->FracCloseCnt  = pAllReg->VEDU_LOWPOW_CNT1;               //

		pEncPara->stStat.u32TotalPicBits += pEncPara->PicBits;
		if (pEncPara->stStat.u32TotalPicBits <= pEncPara->PicBits)
		{
		    pEncPara->stStat.u32TotalPicBits = pEncPara->PicBits;
			pEncPara->stStat.u32TotalEncodeNum = 1;
		}
		else
		{
		    pEncPara->stStat.u32TotalEncodeNum++;
		}
    }

    /* clear vedu int */
    //VENC_HAL_ClrAllInt(pAllReg);      /*pAllReg->VEDU_INTCLR.u32 = 0xFFFFFFFF;*/    
}

/******************************************************************************
Function   :
Description: config vedu reg & start one frame encode.
Calls      :
Input      :
Output     :
Return     :
Others     :
******************************************************************************/
HI_VOID VENC_HAL_CfgReg( VeduEfl_EncPara_S* EncHandle )
{
    VeduEfl_EncPara_S* pEncPara = EncHandle;
    S_VEDU_REGS_TYPE  *pAllReg = (S_VEDU_REGS_TYPE  *)pEncPara->pRegBase;
    int i;
    
    {   //INTMASK
        U_VEDU_INTMASK D32;
        D32.u32 = 0;

        D32.bits.VencEndOfPicMask = 1;
        D32.bits.VencBufFullMask = 1;
        D32.bits.VencPbitOverflow = 1;
#if ((defined __VENC_3716CV200_CONFIG__)|(defined __VENC_S5V100_CONFIG__)|(defined __VENC_98M_CONFIG__)|(defined __VENC_98CV200_CONFIG__))
	    D32.bits.VeduSliceIntMask = pEncPara->LowDlyMod;
        D32.bits.VeduTimeoutMask  = 1;                        //for lowdelay
#endif
        pAllReg->VEDU_INTMASK.u32 = D32.u32;
    }
    {
        U_VEDU_MODE D32;
        D32.u32 = 0;
        D32.bits.vedsel       = 0;
        D32.bits.vencEn       = 1;
        D32.bits.lowDlyMod    = pEncPara->LowDlyMod;
        D32.bits.stdmod = pEncPara->Protocol;
#if ((defined __VENC_3716CV200_CONFIG__)|(defined __VENC_S5V100_CONFIG__)|(defined __VENC_98M_CONFIG__) | (defined __VENC_98CV200_CONFIG__))
        D32.bits.sliceIntEn   = pEncPara->LowDlyMod;
#endif
        D32.bits.timeEn       = pEncPara->TimeOutEn;                          
#ifdef __VENC_S40V200_CONFIG__
        D32.bits.accesslockEn = pEncPara->RegLockEn ;
#endif
#if ((defined __VENC_S5V100_CONFIG__)|(defined __VENC_98M_CONFIG__) | (defined __VENC_98CV200_CONFIG__))
        D32.bits.vcpi_csc_en  = (pEncPara->csc_mode?1:0);
#endif
        D32.bits.clkGateEn    = pEncPara->ClkGateEn;
        D32.bits.memClkGateEn = pEncPara->MemClkGateEn;                           //
        D32.bits.strFmt       = pEncPara->StoreFmt;                               //亮色度分量的存储格式 :0(semiplaner)
        D32.bits.packageSel   = pEncPara->PackageSel;
        D32.bits.osdEn        = pEncPara->OsdCfg.osd_en[0] | pEncPara->OsdCfg.osd_en[1] | pEncPara->OsdCfg.osd_en[2] | pEncPara->OsdCfg.osd_en[3] | 
                                pEncPara->OsdCfg.osd_en[4] | pEncPara->OsdCfg.osd_en[5] | pEncPara->OsdCfg.osd_en[6] | pEncPara->OsdCfg.osd_en[7] ;  

        pAllReg->VEDU_MODE.u32 = D32.u32;
    }

#if ((defined __VENC_S5V100_CONFIG__)|(defined __VENC_98M_CONFIG__) | (defined __VENC_98CV200_CONFIG__))
    //csc   //config it with the default value.
    {
    	U_VEDU_CSC_COEF0 D32;
    	D32.u32 = 0;
    	D32.bits.vcpi_csc00 = pEncPara->csc00;
    	D32.bits.vcpi_csc01 = pEncPara->csc01;
    	pAllReg->VEDU_CSC_COEF0.u32 = D32.u32;
    }
    {
    	U_VEDU_CSC_COEF1 D32;
    	D32.u32 = 0;
    	D32.bits.vcpi_csc02 = pEncPara->csc02;
    	D32.bits.vcpi_csc10 = pEncPara->csc10;
    	pAllReg->VEDU_CSC_COEF1.u32 = D32.u32;
    }
    {
    	U_VEDU_CSC_COEF2 D32;
    	D32.u32 = 0;
    	D32.bits.vcpi_csc11 = pEncPara->csc11;
    	D32.bits.vcpi_csc12 = pEncPara->csc12;
    	pAllReg->VEDU_CSC_COEF2.u32 = D32.u32;
    }
    {
    	U_VEDU_CSC_COEF3 D32;
    	D32.u32 = 0;
    	D32.bits.vcpi_csc20 = pEncPara->csc20;
    	D32.bits.vcpi_csc21 = pEncPara->csc21;
    	pAllReg->VEDU_CSC_COEF3.u32 = D32.u32;
    }
    {
    	U_VEDU_CSC_COEF4 D32;
    	D32.u32 = 0;
    	D32.bits.vcpi_csc22 = pEncPara->csc22;
    	pAllReg->VEDU_CSC_COEF4.u32 = D32.u32;
    }
    {
    	U_VEDU_CSC_INDC D32;
    	D32.u32 = 0;
    	D32.bits.vcpi_csc_indc0 = pEncPara->cscin0;
    	D32.bits.vcpi_csc_indc1 = pEncPara->cscin1;
    	D32.bits.vcpi_csc_indc2 = pEncPara->cscin2;
    	pAllReg->VEDU_CSC_INDC.u32 = D32.u32;
    }
    {
    	U_VEDU_CSC_OUTDC D32;
    	D32.u32 = 0;
    	D32.bits.vcpi_csc_outdc0 = pEncPara->cscout0;
    	D32.bits.vcpi_csc_outdc1 = pEncPara->cscout1;
    	D32.bits.vcpi_csc_outdc2 = pEncPara->cscout2;
    	pAllReg->VEDU_CSC_OUTDC.u32 = D32.u32;
    }
#endif

    {   //pic cfg0
        U_VEDU_PICFG0 D32;
        D32.u32 = 0;

        D32.bits.ptbitsEn        = pEncPara->PtBitsEn;                                                 
        D32.bits.cabacInitIdc    = pEncPara->CabacInitIdc;                                        
        D32.bits.cabacByteStuff  = pEncPara->CabacStuffEn;                                           
        D32.bits.ConstIntra      = pEncPara->ConstIntra;                                       
        D32.bits.Ipicture        = pEncPara->IntraPic;
        D32.bits.TransMode       = pEncPara->TransMode;                        
        D32.bits.numRefIndex     = pEncPara->NumRefIndex;                     
        D32.bits.NalRefIdc       = 3;
        D32.bits.EntropyEncMode  = pEncPara->H264CabacEn;                     
        D32.bits.dblkIdc         = pEncPara->DblkIdc;                         
        D32.bits.dblkAlpha       = pEncPara->DblkAlpha;                       
        D32.bits.dblkBeta        = pEncPara->DblkBeta;                        

        pAllReg->VEDU_PICFG0.u32 = D32.u32;
    }
    {
        //pic cfg1
        U_VEDU_PICFG1 D32;
        D32.u32 = 0;
        
        D32.bits.SliceSplit    = pEncPara->SlcSplitEn;
        D32.bits.SliceSpiltMod = pEncPara->SlcSplitMod;       
        D32.bits.SliceSize     = pEncPara->SplitSize;
        
        pAllReg->VEDU_PICFG1.u32 = D32.u32;
    }
    {   //pic cfg2
        U_VEDU_PICFG2 D32;
        D32.u32 = 0;
        
        D32.bits.IPCMPredEn          = pEncPara->IPCMPredEn;
        D32.bits.Intra4x4PredEn      = pEncPara->Intra4x4PredEn;
        D32.bits.Intra8x8PredEn      = pEncPara->Intra8x8PredEn;
        D32.bits.Intra16x16PredEn    = pEncPara->Intra16x16PredEn;
        D32.bits.intra4ReducedModeEn = pEncPara->I4ReducedModeEn;
		D32.bits.IntraLowPowEn       = pEncPara->IntraLowpowEn;
        
        pAllReg->VEDU_PICFG2.u32 = D32.u32;
    }
    {   //pic cfg3
        U_VEDU_PICFG3 D32;
        D32.u32 = 0;
        
        D32.bits.Inter8x8PredEn   = pEncPara->Inter8x8PredEn;
        D32.bits.Inter8x16PredEn  = pEncPara->Inter8x16PredEn;
        D32.bits.Inter16x8PredEn  = pEncPara->Inter16x8PredEn;
        D32.bits.Inter16x16PredEn = pEncPara->Inter16x16PredEn;
        D32.bits.pskEn            = pEncPara->PskipEn;
		D32.bits.fracLowPowEn     = pEncPara->fracLowpowEn;
		D32.bits.intpLowPowEn     = pEncPara->intpLowpowEn;
        D32.bits.extedgeEn        = pEncPara->ExtedgeEn;
        
        pAllReg->VEDU_PICFG3.u32 = D32.u32;
    }
	{   //pic cfg4
        U_VEDU_PICFG4 D32;
        D32.u32 = 0;
        
        D32.bits.vcpi_clip_en  = pEncPara->PixClipEn;
        
        pAllReg->VEDU_PICFG4.u32 = D32.u32;
    }
	{   //pic cfg5
        U_VEDU_PICFG5 D32;
        D32.u32 = 0;
        
        D32.bits.vcpi_luma_min = pEncPara->LumaClipMin;
		D32.bits.vcpi_luma_max = pEncPara->LumaClipMax;
		D32.bits.vcpi_chr_min  = pEncPara->ChrClipMin;
		D32.bits.vcpi_chr_max  = pEncPara->ChrClipMax;
		
        
        pAllReg->VEDU_PICFG5.u32 = D32.u32;
    }
    {   //mpeg4 fresh i mb
        U_VEDU_ENCUPDATE D32;
        D32.u32 = 0;
        
        D32.bits.iMBNum  = pEncPara->IMbNum;
        D32.bits.startMb = pEncPara->StartMb;
        
        pAllReg->VEDU_ENCUPDATE.u32 = D32.u32;
    }
    {   //image size
        U_VEDU_VEDIMGSIZE D32;
        D32.u32 = 0;
        
        D32.bits.ImgWidthInPixMinus1  = pEncPara->PicWidth  - 1;
        D32.bits.ImgHeightInPixMinus1 = pEncPara->PicHeight - 1;
        
        pAllReg->VEDU_VEDIMGSIZE.u32 = D32.u32;
    }
    
    //ptbit not sure
    pAllReg->VEDU_PTBITS = pEncPara->PtBits;

    //slice hdr
    pAllReg->VEDU_SLCHDRSTRM0 = pEncPara->SlcHdrStream [0];
    pAllReg->VEDU_SLCHDRSTRM1 = pEncPara->SlcHdrStream [1];
    pAllReg->VEDU_SLCHDRSTRM2 = pEncPara->SlcHdrStream [2];
    pAllReg->VEDU_SLCHDRSTRM3 = pEncPara->SlcHdrStream [3];
    pAllReg->VEDU_SLCHDRSTRM4 = pEncPara->ReorderStream[0];
    pAllReg->VEDU_SLCHDRSTRM5 = pEncPara->ReorderStream[1];
    pAllReg->VEDU_SLCHDRSTRM6 = pEncPara->MarkingStream[0];
    pAllReg->VEDU_SLCHDRSTRM7 = pEncPara->MarkingStream[1];
    {   
        U_VEDU_SLCHDRPARA D32;
        D32.u32 = 0;
        
        D32.bits.IDRind    = pEncPara->IntraPic;
        D32.bits.ParaWd    = ((pEncPara->SlcHdrBits >>  0) & 0xFF) - 1;
        D32.bits.ReorderWd = ((pEncPara->SlcHdrBits >>  8) & 0xFF) - 1; 
        D32.bits.MarkingWd = ((pEncPara->SlcHdrBits >> 16) & 0xFF) - 1;
        
        pAllReg->VEDU_SLCHDRPARA.u32 = D32.u32;
    }
    
    pAllReg->VEDU_PTS0 = pEncPara->PTS0;
    pAllReg->VEDU_PTS1 = pEncPara->PTS1;
    
    pAllReg->VEDU_TIMEOUT    = pEncPara->TimeOut;

#if (defined __VENC_98CV200_CONFIG__)
    {   //outstanding
        U_VEDU_OUTSTD D32;
        D32.u32 = 0;
        
        D32.bits.woutstdNum    = pEncPara->wOutStdNum;
        D32.bits.routstdNum     = pEncPara->rOutStdNum;
        
        pAllReg->VEDU_OUTSTD.u32 = D32.u32;
    }
#else
    pAllReg->VEDU_OUTSTD.u32 = pEncPara->OutStdNum;
#endif
    
    /* frame addr & stride */                            //  keypoint
    pAllReg->VEDU_SRCYADDR = pEncPara->SrcYAddr;
    pAllReg->VEDU_SRCCADDR = pEncPara->SrcCAddr;
#if ((defined __VENC_3716CV200_CONFIG__)|(defined __VENC_S5V100_CONFIG__)|(defined __VENC_98M_CONFIG__)| (defined __VENC_98CV200_CONFIG__))
    pAllReg->VEDU_SRCVADDR = pEncPara->SrcVAddr;
#endif
    pAllReg->VEDU_RCNYADDR = pEncPara->RcnYAddr[pEncPara->RcnIdx];
    pAllReg->VEDU_RCNCADDR = pEncPara->RcnCAddr[pEncPara->RcnIdx];
    pAllReg->VEDU_REFYADDR = pEncPara->RcnYAddr[!pEncPara->RcnIdx];
    pAllReg->VEDU_REFCADDR = pEncPara->RcnCAddr[!pEncPara->RcnIdx];

#if ((defined __VENC_3716CV200_CONFIG__)|(defined __VENC_S5V100_CONFIG__)|(defined __VENC_98M_CONFIG__) |(defined __VENC_98CV200_CONFIG__))
    {   
        U_VEDU_TUNL_READ_INTVL D32;
        D32.u32 = 0;
        
        D32.bits.tunlReadIntvl = pEncPara->tunlReadIntvl; 
        
        pAllReg->VEDU_TUNL_READ_INTVL.u32 = D32.u32;
    }
	pAllReg->VEDU_TUNLVELL_ADDR  = pEncPara->tunlcellAddr;
#endif

    {   
        U_VEDU_SSTRIDE D32;
        D32.u32 = 0;
        
        D32.bits.SrcYStride = pEncPara->SStrideY; 
        D32.bits.SrcCStride = pEncPara->SStrideC; 
        pAllReg->VEDU_SSTRIDE.u32 = D32.u32;
    }
    {   
        U_VEDU_RSTRIDE D32;
        D32.u32 = 0;
        
        D32.bits.RYStride = pEncPara->RStrideY;    //     
        D32.bits.RCStride = pEncPara->RStrideC;    //   
        pAllReg->VEDU_RSTRIDE.u32 = D32.u32;
    }
    
    /* output stream buffer */
    pAllReg->VEDU_STRMADDR       = pEncPara->StrmBufAddr;
    pAllReg->VEDU_SRPTRADDR      = pEncPara->StrmBufRpAddr;
    pAllReg->VEDU_SWPTRADDR      = pEncPara->StrmBufWpAddr;
    pAllReg->VEDU_STRMBUFLEN.u32 = pEncPara->StrmBufSize;

    //pAllReg->VEDU_TNBUFADDR   = pEncPara->TopNBufAddr;             //del
    //pAllReg->VEDU_IPIXBUFADDR = pEncPara->IpixBufAddr;             //del
    
    /* ME Config */
    {   
        U_VEDU_MECFG0 D32;
        D32.u32 = 0;
        
        D32.bits.HSWSize = pEncPara->HSWSize;
        D32.bits.VSWSize = pEncPara->VSWSize;             
		D32.bits.fracRealMvThr = pEncPara->fracRealMvThr;
        
        pAllReg->VEDU_MECFG0.u32 = D32.u32;
    }
    {   
        U_VEDU_MECFG1 D32;
        D32.u32 = 0;
        
        D32.bits.Rect0Mod = pEncPara->RectMod[0];
        D32.bits.Rect1Mod = pEncPara->RectMod[1];
        D32.bits.Rect2Mod = pEncPara->RectMod[2];
        D32.bits.Rect3Mod = pEncPara->RectMod[3];
        //D32.bits.Rect4Mod = pEncPara->RectMod[4];
        //D32.bits.Rect5Mod = pEncPara->RectMod[5];
        
        pAllReg->VEDU_MECFG1.u32 = D32.u32;
    }
    
    for(i = 0; i < 4; i++)
    {   
        U_VEDU_MERECTCFG D32;
        D32.u32 = 0;
        
        D32.bits.RectWidth  = pEncPara->RectWidth [i];
        D32.bits.RectHeight = pEncPara->RectHeight[i];
        D32.bits.RectHstep  = pEncPara->RectHstep [i];
        D32.bits.RectVstep  = pEncPara->RectVstep [i];
        
        pAllReg->VEDU_MERECTCFG[i].u32 = D32.u32;
    }
    
    {   
        U_VEDU_METHR0 D32;
        D32.u32 = 0;
        
#ifndef __VENC_98CV200_CONFIG__        
        D32.bits.SymcSTh1 = pEncPara->StartThr1;
#endif
        D32.bits.SymcSTh2 = pEncPara->StartThr2;
        
        pAllReg->VEDU_METHR0.u32 = D32.u32;
    }
    {   
        U_VEDU_METHR1 D32;
        D32.u32 = 0;
        
        D32.bits.IntraThr = pEncPara->IntraThr;
        
        pAllReg->VEDU_METHR1.u32 = D32.u32;
    }
    {   
        U_VEDU_MERDOCFG D32;
        D32.u32 = 0;
        
        D32.bits.LmdaOff16   = pEncPara->LmdaOff16;
        D32.bits.LmdaOff16x8 = pEncPara->LmdaOff1608;
        D32.bits.LmdaOff8x16 = pEncPara->LmdaOff0816;
        D32.bits.LmdaOff8    = pEncPara->LmdaOff8;
        
        pAllReg->VEDU_MERDOCFG.u32 = D32.u32;
    }
    {   
        U_VEDU_CREFLDMODE D32;
        D32.u32 = 0;
        
        D32.bits.crefldBur8En = pEncPara->CrefldBur8En;
        
        pAllReg->VEDU_CREFLDMODE.u32 = D32.u32;
    }
    {   
        U_VEDU_MDCFG D32;
        D32.u32 = 0;
        
        D32.bits.mdDelta = pEncPara->MdDelta;
        D32.bits.rdoEn   = 1;
        
        pAllReg->VEDU_MDCFG.u32 = D32.u32;
    }
    /* MDCFG */
    {
        ;
    }
    /* MCTF */
#ifdef __VENC_S40V200_CONFIG__
    {   
        U_VEDU_MCTFCFG0 D32;
        D32.u32 = 0;
        
        D32.bits.MctfStillEn      = pEncPara->MctfStillEn;
        D32.bits.MctfMovEn        = pEncPara->MctfMovEn;
        D32.bits.mctfStillMvThr   = pEncPara->mctfStillMvThr;
        D32.bits.mctfRealMvThr    = pEncPara->mctfRealMvThr;
        D32.bits.MctfStillCostThr = pEncPara->MctfStillCostThr;
        
        pAllReg->VEDU_MCTFCFG0.u32 = D32.u32;
    }
    {   
        U_VEDU_MCTFCFG1 D32;
        D32.u32 = 0;
        
        D32.bits.MctfLog2mctf    = pEncPara->MctfLog2mctf;
        D32.bits.MctfLumaDiffThr = pEncPara->MctfLumaDiffThr;
        D32.bits.MctfChrDiffThr  = pEncPara->MctfChrDiffThr;
        
        pAllReg->VEDU_MCTFCFG1.u32 = D32.u32;
    }
    {   
        U_VEDU_MCTFCFG2 D32;
        D32.u32 = 0;
        
        D32.bits.mctfChrDeltaThr = pEncPara->MctfChrDeltaThr;
        D32.bits.mctfStiMadiThr1 = pEncPara->MctfStiMadiThr1;
        D32.bits.mctfStiMadiThr2 = pEncPara->MctfStiMadiThr2;
        D32.bits.mctfMovMadiThr  = pEncPara->MctfMovMadiThr;
        
        pAllReg->VEDU_MCTFCFG2.u32 = D32.u32;
    }
    {   
        U_VEDU_MCTFCFG3 D32;
        D32.u32 = 0;
        
        D32.bits.mctfMovMad1_m = pEncPara->MctfMovMad1_m;
        D32.bits.mctfMovMad1_n = pEncPara->MctfMovMad1_n;
        D32.bits.mctfMovMad2_m = pEncPara->MctfMovMad2_m;
        D32.bits.mctfMovMad2_n = pEncPara->MctfMovMad2_n;
        D32.bits.mctfStiMad1_m = pEncPara->MctfStiMad1_m;
        D32.bits.mctfStiMad1_n = pEncPara->MctfStiMad1_n;
        D32.bits.mctfStiMad2_m = pEncPara->MctfStiMad2_m;
        D32.bits.mctfStiMad2_n = pEncPara->MctfStiMad2_n;
        
        pAllReg->VEDU_MCTFCFG3.u32 = D32.u32;
    }
#else
    {   
        U_VEDU_MCTFCFG0 D32;
        D32.u32 = 0;
        
        D32.bits.MctfStillEn      = pEncPara->MctfStillEn;
        D32.bits.MctfSmlmovEn     = pEncPara->MctfSmlmovEn;
        D32.bits.MctfBigmovEn     = pEncPara->MctfBigmovEn;
        D32.bits.mctfStillMvThr   = pEncPara->mctfStillMvThr;
        D32.bits.mctfRealMvThr    = pEncPara->mctfRealMvThr;
        D32.bits.MctfStillCostThr = pEncPara->MctfStillCostThr;
        
        pAllReg->VEDU_MCTFCFG0.u32 = D32.u32;
    }
    {   
        U_VEDU_MCTFCFG1 D32;
        D32.u32 = 0;
        
        D32.bits.MctfStiLumaOrialpha    = pEncPara->MctfStiLumaOrialpha     ;
        D32.bits.MctfSmlmovLumaOrialpha = pEncPara->MctfSmlmovLumaOrialpha  ;
        D32.bits.MctfBigmovLumaOrialpha = pEncPara->MctfBigmovLumaOrialpha  ;
        D32.bits.MctfChrOrialpha        = pEncPara->MctfChrOrialpha         ;
        
        pAllReg->VEDU_MCTFCFG1.u32 = D32.u32;
    }
    {   
        U_VEDU_MCTFCFG2 D32;
        D32.u32 = 0;
        
        D32.bits.mctfStiLumaDiffThr0 = pEncPara->mctfStiLumaDiffThr0 ;
        D32.bits.mctfStiLumaDiffThr1 = pEncPara->mctfStiLumaDiffThr1 ;
        D32.bits.mctfStiLumaDiffThr2 = pEncPara->mctfStiLumaDiffThr2 ;
        
        pAllReg->VEDU_MCTFCFG2.u32 = D32.u32;
    }
    {   
        U_VEDU_MCTFCFG3 D32;
        D32.u32 = 0;
        
        D32.bits.mctfSmlmovLumaDiffThr0 = pEncPara->mctfSmlmovLumaDiffThr0;
        D32.bits.mctfSmlmovLumaDiffThr1 = pEncPara->mctfSmlmovLumaDiffThr1;
        D32.bits.mctfSmlmovLumaDiffThr2 = pEncPara->mctfSmlmovLumaDiffThr2;
        
        pAllReg->VEDU_MCTFCFG3.u32 = D32.u32;
    }
    {   
        U_VEDU_MCTFCFG4 D32;
        D32.u32 = 0;

        D32.bits.mctfBigmovLumaDiffThr0 = pEncPara->mctfBigmovLumaDiffThr0;
        D32.bits.mctfBigmovLumaDiffThr1 = pEncPara->mctfBigmovLumaDiffThr1;
        D32.bits.mctfBigmovLumaDiffThr2 = pEncPara->mctfBigmovLumaDiffThr2;
        
        pAllReg->VEDU_MCTFCFG4.u32 = D32.u32;
    }    
    {   
        U_VEDU_MCTFCFG5 D32;
        D32.u32 = 0;

        D32.bits.mctfStiLumaDiffK0 = pEncPara->mctfStiLumaDiffK0;
        D32.bits.mctfStiLumaDiffK1 = pEncPara->mctfStiLumaDiffK1;
        D32.bits.mctfStiLumaDiffK2 = pEncPara->mctfStiLumaDiffK2;
        
        pAllReg->VEDU_MCTFCFG5.u32 = D32.u32;
    }    
    {   
        U_VEDU_MCTFCFG6 D32;
        D32.u32 = 0;

        D32.bits.mctfSmlmovLumaDiffK0 = pEncPara->mctfSmlmovLumaDiffK0;
        D32.bits.mctfSmlmovLumaDiffK1 = pEncPara->mctfSmlmovLumaDiffK1;
        D32.bits.mctfSmlmovLumaDiffK2 = pEncPara->mctfSmlmovLumaDiffK2;
        
        pAllReg->VEDU_MCTFCFG6.u32 = D32.u32;
    } 
    {   
        U_VEDU_MCTFCFG7 D32;
        D32.u32 = 0;

        D32.bits.mctfBigmovLumaDiffK0 = pEncPara->mctfBigmovLumaDiffK0;
        D32.bits.mctfBigmovLumaDiffK1 = pEncPara->mctfBigmovLumaDiffK1;
        D32.bits.mctfBigmovLumaDiffK2 = pEncPara->mctfBigmovLumaDiffK2;
        
        pAllReg->VEDU_MCTFCFG7.u32 = D32.u32;
    }  
    {   
        U_VEDU_MCTFCFG8 D32;
        D32.u32 = 0;

        D32.bits.mctfChrDiffThr0 = pEncPara->mctfChrDiffThr0;
        D32.bits.mctfChrDiffThr1 = pEncPara->mctfChrDiffThr1;
        D32.bits.mctfChrDiffThr2 = pEncPara->mctfChrDiffThr2;
        
        pAllReg->VEDU_MCTFCFG8.u32 = D32.u32;
    }        
    {   
        U_VEDU_MCTFCFG9 D32;
        D32.u32 = 0;

        D32.bits.mctfChrDiffK0 = pEncPara->mctfChrDiffK0;
        D32.bits.mctfChrDiffK1 = pEncPara->mctfChrDiffK1;
        D32.bits.mctfChrDiffK2 = pEncPara->mctfChrDiffK2;
        
        pAllReg->VEDU_MCTFCFG9.u32 = D32.u32;
    }        
    {   
        U_VEDU_MCTFCFG10 D32;
        D32.u32 = 0;

        D32.bits.mctfOriRatio       = pEncPara->mctfOriRatio;
        D32.bits.mctfStiMaxRatio    = pEncPara->mctfStiMaxRatio;
        D32.bits.mctfSmlmovMaxRatio = pEncPara->mctfSmlmovMaxRatio;
        D32.bits.mctfBigmovMaxRatio = pEncPara->mctfBigmovMaxRatio;
                
        pAllReg->VEDU_MCTFCFG10.u32 = D32.u32;
    }        
    {   
        U_VEDU_MCTFCFG11 D32;
        D32.u32 = 0;

        D32.bits.mctfStiMadiThr0 = pEncPara->mctfStiMadiThr0;
        D32.bits.mctfStiMadiThr1 = pEncPara->mctfStiMadiThr1;
        D32.bits.mctfStiMadiThr2 = pEncPara->mctfStiMadiThr2;
        
        pAllReg->VEDU_MCTFCFG11.u32 = D32.u32;
    }        
    {   
        U_VEDU_MCTFCFG12 D32;
        D32.u32 = 0;
        
        D32.bits.mctfSmlmovMadiThr0 = pEncPara->mctfSmlmovMadiThr0;
        D32.bits.mctfSmlmovMadiThr1 = pEncPara->mctfSmlmovMadiThr1;
        D32.bits.mctfSmlmovMadiThr2 = pEncPara->mctfSmlmovMadiThr2;
        
        pAllReg->VEDU_MCTFCFG12.u32 = D32.u32;
    }        
    {   
        U_VEDU_MCTFCFG13 D32;
        D32.u32 = 0;

        D32.bits.mctfBigmovMadiThr0 = pEncPara->mctfBigmovMadiThr0;
        D32.bits.mctfBigmovMadiThr1 = pEncPara->mctfBigmovMadiThr1;
        D32.bits.mctfBigmovMadiThr2 = pEncPara->mctfBigmovMadiThr2;
        
        pAllReg->VEDU_MCTFCFG13.u32 = D32.u32;
    }        
    {   
        U_VEDU_MCTFCFG14 D32;
        D32.u32 = 0;

        D32.bits.mctfStiMadiK0 = pEncPara->mctfStiMadiK0;
        D32.bits.mctfStiMadiK1 = pEncPara->mctfStiMadiK1;
        D32.bits.mctfStiMadiK2 = pEncPara->mctfStiMadiK2;
        
        pAllReg->VEDU_MCTFCFG14.u32 = D32.u32;
    }        
    {   
        U_VEDU_MCTFCFG15 D32;
        D32.u32 = 0;

        D32.bits.mctfSmlmovMadiK0 = pEncPara->mctfSmlmovMadiK0;
        D32.bits.mctfSmlmovMadiK1 = pEncPara->mctfSmlmovMadiK1;
        D32.bits.mctfSmlmovMadiK2 = pEncPara->mctfSmlmovMadiK2;
        
        pAllReg->VEDU_MCTFCFG15.u32 = D32.u32;
    }          
    {   
        U_VEDU_MCTFCFG16 D32;
        D32.u32 = 0;

        D32.bits.mctfBigmovMadiK0 = pEncPara->mctfBigmovMadiK0;
        D32.bits.mctfBigmovMadiK1 = pEncPara->mctfBigmovMadiK1;
        D32.bits.mctfBigmovMadiK2 = pEncPara->mctfBigmovMadiK2;
        
        pAllReg->VEDU_MCTFCFG16.u32 = D32.u32;
    }                      
#endif
    
    
    /* RC */
    {   
        U_VEDU_QPTHR D32;
        D32.u32 = 0;
        
        D32.bits.minQp       = pEncPara->MinQp;
        D32.bits.maxQp       = pEncPara->MaxQp;
        D32.bits.startQp     = pEncPara->StartQp;
        //D32.bits.startQpType = pEncPara->StartQpType;
        D32.bits.chromaQpOff = pEncPara->ChrQpOffset;
        
        pAllReg->VEDU_QPTHR.u32 = D32.u32;
    }
    {   
        U_VEDU_RC1 D32;
        D32.u32 = 0;
        
        D32.bits.rcQpDelta   = (pEncPara->IntraPic ? pEncPara->stRc.IQpDelta: pEncPara->stRc.PQpDelta);
        D32.bits.rcMadpDelta = -8;//0;
        
        pAllReg->VEDU_RC1.u32 = D32.u32;
    }
#if 0  //old version	
    if (pEncPara->IntraPic)                                         /*  调节图像质量 */
    {
        pAllReg->VEDU_QPDELTATHR0.u32 = 0x110f0e0c;   
        pAllReg->VEDU_QPDELTATHR1.u32 = 0x1b181613;
        pAllReg->VEDU_QPDELTATHR2.u32 = 0xffffffff;
    }
    else
    {
        pAllReg->VEDU_QPDELTATHR0.u32 = 0x09070707;
        pAllReg->VEDU_QPDELTATHR1.u32 = 0x17120e0b;
        pAllReg->VEDU_QPDELTATHR2.u32 = 0xffffff1c;
    }
#else
        pAllReg->VEDU_QPDELTATHR0.u32 = 0x09070707;
        pAllReg->VEDU_QPDELTATHR1.u32 = 0x19120e0c;
        pAllReg->VEDU_QPDELTATHR2.u32 = 0xffffffff;
#endif
    
    pAllReg->VEDU_TARGETBITS  = pEncPara->TargetBits;
    //pAllReg->VEDU_REFMBHBITS  = pEncPara->TotalMbhBits;
    //pAllReg->VEDU_REFTXTCOST0 = pEncPara->TotalTxtCost0;
    //pAllReg->VEDU_REFTXTCOST1 = pEncPara->TotalTxtCost1;
    
    /* mode lambda */
    for(i = 0; i < 20; i++)
    {   
        U_VEDU_MODLAMBDA D32;
        D32.u32 = 0;
        
        D32.bits.ModLambda0 = pEncPara->ModLambda[i*2+0];
        D32.bits.ModLambda1 = pEncPara->ModLambda[i*2+1];
        
        pAllReg->VEDU_MODLAMBDA[i].u32 = D32.u32;
    }
    
    /* scale list 8x8 */
    for(i = 0; i < 32; i++)
    {   
        U_VEDU_SCALE D32;
        D32.u32 = 0;
        
        D32.bits.scale0 = pEncPara->Scale8x8[i*4+0];
        D32.bits.scale1 = pEncPara->Scale8x8[i*4+1];
        D32.bits.scale2 = pEncPara->Scale8x8[i*4+2];
        D32.bits.scale3 = pEncPara->Scale8x8[i*4+3];
        
        pAllReg->VEDU_SCALE[i].u32 = D32.u32;
    }

    /* ROI */
    {   
        U_VEDU_ROICFG D32;
        D32.u32 = 0;
        
        D32.bits.region0En = pEncPara->RoiCfg.Enable[0];
        D32.bits.region1En = pEncPara->RoiCfg.Enable[1];
        D32.bits.region2En = pEncPara->RoiCfg.Enable[2];
        D32.bits.region3En = pEncPara->RoiCfg.Enable[3];
        D32.bits.region4En = pEncPara->RoiCfg.Enable[4];
        D32.bits.region5En = pEncPara->RoiCfg.Enable[5];
        D32.bits.region6En = pEncPara->RoiCfg.Enable[6];
        D32.bits.region7En = pEncPara->RoiCfg.Enable[7];
        
        D32.bits.bAbsQp0   = pEncPara->RoiCfg.AbsQpEn[0];
        D32.bits.bAbsQp1   = pEncPara->RoiCfg.AbsQpEn[1];
        D32.bits.bAbsQp2   = pEncPara->RoiCfg.AbsQpEn[2];
        D32.bits.bAbsQp3   = pEncPara->RoiCfg.AbsQpEn[3];
        D32.bits.bAbsQp4   = pEncPara->RoiCfg.AbsQpEn[4];
        D32.bits.bAbsQp5   = pEncPara->RoiCfg.AbsQpEn[5];
        D32.bits.bAbsQp6   = pEncPara->RoiCfg.AbsQpEn[6];
        D32.bits.bAbsQp7   = pEncPara->RoiCfg.AbsQpEn[7];
#ifndef __VENC_S40V200_CONFIG__
        D32.bits.region0Keep   = 0;//pEncPara->RoiCfg.Keep[0];
        D32.bits.region1Keep   = 0;//pEncPara->RoiCfg.Keep[1];
        D32.bits.region2Keep   = 0;//pEncPara->RoiCfg.Keep[2];
        D32.bits.region3Keep   = 0;//pEncPara->RoiCfg.Keep[3];
        D32.bits.region4Keep   = 0;//pEncPara->RoiCfg.Keep[4];
        D32.bits.region5Keep   = 0;//pEncPara->RoiCfg.Keep[5];
        D32.bits.region6Keep   = 0;//pEncPara->RoiCfg.Keep[6];
        D32.bits.region7Keep   = 0;//pEncPara->RoiCfg.Keep[7];
#endif        
        pAllReg->VEDU_ROICFG.u32 = D32.u32;
   //****************************************************************************//             
    }
    /* OSD */
    {   
        U_VPP_OSD_CFG D32;
        
        D32.bits.osdRgbfmt = pEncPara->OsdCfg.osd_rgbfm;
        
        D32.bits.osd0En = pEncPara->OsdCfg.osd_en[0];
        D32.bits.osd1En = pEncPara->OsdCfg.osd_en[1];
        D32.bits.osd2En = pEncPara->OsdCfg.osd_en[2];
        D32.bits.osd3En = pEncPara->OsdCfg.osd_en[3];
        D32.bits.osd4En = pEncPara->OsdCfg.osd_en[4];
        D32.bits.osd5En = pEncPara->OsdCfg.osd_en[5];
        D32.bits.osd6En = pEncPara->OsdCfg.osd_en[6];
        D32.bits.osd7En = pEncPara->OsdCfg.osd_en[7];
        
        D32.bits.osd0GlobalEn = pEncPara->OsdCfg.osd_global_en[0];
        D32.bits.osd1GlobalEn = pEncPara->OsdCfg.osd_global_en[1];
        D32.bits.osd2GlobalEn = pEncPara->OsdCfg.osd_global_en[2];
        D32.bits.osd3GlobalEn = pEncPara->OsdCfg.osd_global_en[3];
        D32.bits.osd4GlobalEn = pEncPara->OsdCfg.osd_global_en[4];
        D32.bits.osd5GlobalEn = pEncPara->OsdCfg.osd_global_en[5];
        D32.bits.osd6GlobalEn = pEncPara->OsdCfg.osd_global_en[6];
        D32.bits.osd7GlobalEn = pEncPara->OsdCfg.osd_global_en[7];
        
        pAllReg->VPP_OSD_CFG.u32 = D32.u32;
    }
#ifdef __VENC_98CV200_CONFIG__
    // add for MMU                                              
    {   //rrmax0                                                
        U_VEDU_RRMAX0 D32;                                      
        D32.u32 = 0;                                            
                                                                
        D32.bits.ch00_rrmax  = pEncPara->ch00_rrmax;         
        D32.bits.ch01_rrmax  = pEncPara->ch01_rrmax;  
        D32.bits.ch02_rrmax  = pEncPara->ch02_rrmax; 
        D32.bits.ch03_rrmax  = pEncPara->ch03_rrmax; 
                                                                
        pAllReg->VEDU_RRMAX0.u32 = D32.u32;                 
    }  
      
    {   //rrmax1                                                
        U_VEDU_RRMAX1 D32;                                      
        D32.u32 = 0;                                            
                                                                
        D32.bits.ch04_rrmax  = pEncPara->ch04_rrmax;         
        D32.bits.ch05_rrmax  = pEncPara->ch05_rrmax;  
        D32.bits.ch06_rrmax  = pEncPara->ch06_rrmax; 
        D32.bits.ch07_rrmax  = pEncPara->ch07_rrmax; 
                                                                
        pAllReg->VEDU_RRMAX1.u32 = D32.u32;                 
    }  
    
    {   //rrmax2                                                
        U_VEDU_RRMAX2 D32;                                      
        D32.u32 = 0;                                            
                                                                
        D32.bits.ch08_rrmax  = pEncPara->ch08_rrmax;         
        D32.bits.ch09_rrmax  = pEncPara->ch09_rrmax;  
        D32.bits.ch10_rrmax  = pEncPara->ch10_rrmax; 
        D32.bits.ch11_rrmax  = pEncPara->ch11_rrmax; 
                                                                
        pAllReg->VEDU_RRMAX2.u32 = D32.u32;                 
    }  
    
    {   //rrmax3                                                
        U_VEDU_RRMAX3 D32;                                      
        D32.u32 = 0;                                            
                                                                
        D32.bits.ch12_rrmax  = pEncPara->ch12_rrmax;         

        pAllReg->VEDU_RRMAX3.u32 = D32.u32;                 
    } 
    
    {   //wrmax0                                                
        U_VEDU_WRMAX0 D32;                                      
        D32.u32 = 0;                                            
                                                                
        D32.bits.ch00_wrmax  = pEncPara->ch00_wrmax;         
        D32.bits.ch01_wrmax  = pEncPara->ch01_wrmax;  
        D32.bits.ch02_wrmax  = pEncPara->ch02_wrmax; 
        D32.bits.ch03_wrmax  = pEncPara->ch03_wrmax; 
                                                                
        pAllReg->VEDU_RRMAX0.u32 = D32.u32;                 
    }  
      
    {   //wrmax1                                                
        U_VEDU_WRMAX1 D32;                                      
        D32.u32 = 0;                                            
                                                                
        D32.bits.ch04_wrmax  = pEncPara->ch04_wrmax;         
        D32.bits.ch05_wrmax  = pEncPara->ch05_wrmax;  
        D32.bits.ch06_wrmax  = pEncPara->ch06_wrmax; 
        D32.bits.ch07_wrmax  = pEncPara->ch07_wrmax; 
                                                                
        pAllReg->VEDU_RRMAX1.u32 = D32.u32;                 
    }  
    
    {   //wrmax2                                                
        U_VEDU_WRMAX2 D32;                                      
        D32.u32 = 0;                                            
                                                                
        D32.bits.ch08_wrmax  = pEncPara->ch08_wrmax;         
        D32.bits.ch09_wrmax  = pEncPara->ch09_wrmax;  
        D32.bits.ch10_wrmax  = pEncPara->ch10_wrmax; 
        D32.bits.ch11_wrmax  = pEncPara->ch11_wrmax; 
                                                                
        pAllReg->VEDU_RRMAX2.u32 = D32.u32;                 
    }  
    
    {   //wrmax3                                                
        U_VEDU_WRMAX3 D32;                                      
        D32.u32 = 0;                                            
                                                                
        D32.bits.ch12_wrmax  = pEncPara->ch12_wrmax; 
        D32.bits.ch13_wrmax  = pEncPara->ch13_wrmax; 
        D32.bits.ch14_wrmax  = pEncPara->ch14_wrmax; 
                                                      
        pAllReg->VEDU_RRMAX3.u32 = D32.u32;                 
    }    
    
    {   //tmax                                               
        U_VEDU_TMAX D32;                                      
        D32.u32 = 0;                                            
                                                                
        D32.bits.wtmax  = pEncPara->ch12_wrmax; 
        D32.bits.rtmax  = pEncPara->ch13_wrmax; 
                                                      
        pAllReg->VEDU_TMAX.u32 = D32.u32;                 
    }       
    
    {   //secure                                               
        U_VEDU_SECURE D32;                                      
        D32.u32 = 0;                                            
                                                                
        D32.bits.secure_en  = pEncPara->secure_en; 
                                                      
        pAllReg->VEDU_TMAX.u32 = D32.u32;                 
    }
      
    {   //chan_bypass                                               
        U_VEDU_CHN_BYPASS D32;                                      
        D32.u32 = 0;                                            
                                                                
        D32.bits.vcpi_srcy_bypass = pEncPara->vcpi_srcy_bypass;      
        D32.bits.vcpi_srcc_bypass = pEncPara->vcpi_srcc_bypass;      
        D32.bits.vcpi_srcv_bypass = pEncPara->vcpi_srcv_bypass;      
        D32.bits.vcpi_refy_bypass = pEncPara->vcpi_refy_bypass;      
        D32.bits.vcpi_refc_bypass = pEncPara->vcpi_refc_bypass;      
        D32.bits.vcpi_rcny_bypass = pEncPara->vcpi_rcny_bypass;      
        D32.bits.vcpi_rcnc_bypass = pEncPara->vcpi_rcnc_bypass;      
        D32.bits.vcpi_strm_bypass = pEncPara->vcpi_strm_bypass; 		
        
        pAllReg->VEDU_CHN_BYPASS.u32 = D32.u32;                 
    }                                                                            
  
    {   //smmu_scr                                              
        U_VEDU_SMMU_SCR D32;                                      
        D32.u32 = 0;                                            
                                                                
        D32.bits.glb_bypass   = pEncPara->glb_bypass;      
        D32.bits.int_en          = pEncPara->int_en;      
        D32.bits.ptw_pf          = 0;   //ptw的outstanding （0~15）
        
        pAllReg->SMMU_SCR.u32 = D32.u32;                 
    }    
    {   //smmu_ns_int_mask                                            
        U_VEDU_SMMU_INTMASK_NS D32;                                      
        D32.u32 = 0;                                            
                                                                
        D32.bits.intns_tlbmiss_msk             = 0;      
        D32.bits.intns_ptw_trans_msk         = 0;      
        D32.bits.intns_tlbinvalid_msk          = 0; 
        
        pAllReg->SMMU_INTMASK_NS.u32 = D32.u32;                 
    }    

#ifdef HI_SMMU_SUPPORT
     VENC_HAL_CfgSmmuReg(pEncPara);
#endif

#if 0	  
    {   //smmu_lp_ctrl                                              
        U_SMMU_LP_CTRL D32;                                      
        D32.u32 = 0;                                            
                                                                
        D32.bits.auto_clk_gt_en = pEncPara->auto_clk_gt_en;   
        																																		                                                   
        pAllReg->SMMU_LP_CTRL.u32 = D32.u32;                 
    } 
#endif
#endif  
    // start HW to encoder
    pAllReg->VEDU_START.u32 = 0;
    pAllReg->VEDU_START.u32 = 1;
}


#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif
