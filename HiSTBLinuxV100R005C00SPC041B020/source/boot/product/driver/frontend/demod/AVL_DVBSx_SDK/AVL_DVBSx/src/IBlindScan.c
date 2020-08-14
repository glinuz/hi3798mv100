/*
 *           Copyright 2010 Availink, Inc.
 *
 *  This software contains Availink proprietary information and
 *  its use and disclosure are restricted solely to the terms in
 *  the corresponding written license agreement. It shall not be 
 *  disclosed to anyone other than valid licensees without
 *  written permission of Availink, Inc.
 *
 */


//$Revision: 869 $ 
//$Date: 2009-07-07 10:00:53 -0400 (Tue, 07 Jul 2009) $
// 
#include "IBlindScan.h"
#include "IBase.h"
#include "IRx.h"
#include "II2C.h"
#include "ITuner.h"

AVL_DVBSx_ErrorCode AVL_DVBSx_IBlindScan_Scan(struct AVL_DVBSx_BlindScanPara * pBSPara, AVL_uint16 uiTunerLPF_100kHz, const struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r;
	AVL_uint16 uiCarrierFreq_100kHz;
	AVL_uint16 uiMinSymRate;
	enum AVL_DVBSx_FunctionalMode enumFunctionalMode;

	r = AVL_DVBSx_IBase_GetFunctionalMode(&enumFunctionalMode, pAVLChip);

	if(enumFunctionalMode == AVL_DVBSx_FunctMode_BlindScan)
	{
		r |= AVL_DVBSx_II2C_Write16(pAVLChip, rc_tuner_LPF_100kHz_addr, uiTunerLPF_100kHz);

		uiMinSymRate = pBSPara->m_uiMinSymRate_kHz - 200;		// give some tolerance

		if( uiMinSymRate < 800 )	   //Blind scan doesn't support symbol rate less then 1M, give 200K margin
		{
			uiMinSymRate = 800;
		}

		if( pBSPara->m_uiStartFreq_100kHz < pBSPara->m_uiStopFreq_100kHz )
		{
			if( AVL_DVBSx_EC_OK == r )
			{
				uiCarrierFreq_100kHz = ((pBSPara->m_uiStopFreq_100kHz)+(pBSPara->m_uiStartFreq_100kHz))>>1;
				r |= AVL_DVBSx_II2C_Write16(pAVLChip, rc_tuner_frequency_100kHz_addr, uiCarrierFreq_100kHz);
				r |= AVL_DVBSx_II2C_Write16(pAVLChip, rc_blind_scan_min_sym_rate_kHz_addr, uiMinSymRate);
				r |= AVL_DVBSx_II2C_Write16(pAVLChip, rc_blind_scan_max_sym_rate_kHz_addr, (pBSPara->m_uiMaxSymRate_kHz)+200);
				r |= AVL_DVBSx_II2C_Write16(pAVLChip, rc_blind_scan_start_freq_100kHz_addr, (pBSPara->m_uiStartFreq_100kHz));
				r |= AVL_DVBSx_II2C_Write16(pAVLChip, rc_blind_scan_end_freq_100kHz_addr, (pBSPara->m_uiStopFreq_100kHz));

				if( AVL_DVBSx_EC_OK == r )
				{
					r = AVL_DVBSx_IBase_SendRxOP(OP_RX_BLIND_SCAN, (struct AVL_DVBSx_Chip *)pAVLChip );
				}
			}
		}
		else
		{
			r = AVL_DVBSx_EC_GeneralFail;
		}
	}
	else
	{
		r = AVL_DVBSx_EC_GeneralFail;
	}

	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBlindScan_GetScanStatus(struct AVL_DVBSx_BlindScanInfo * pBSInfo, const struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;

	r |= AVL_DVBSx_II2C_Read16(pAVLChip, rs_blind_scan_progress_addr, &(pBSInfo->m_uiProgress));
	r |= AVL_DVBSx_II2C_Read16(pAVLChip, rs_blind_scan_channel_count_addr, &(pBSInfo->m_uiChannelCount));
	r |= AVL_DVBSx_II2C_Read16(pAVLChip, rc_blind_scan_start_freq_100kHz_addr, &(pBSInfo->m_uiNextStartFreq_100kHz));
	r |= AVL_DVBSx_II2C_Read16(pAVLChip, rs_blind_scan_error_code_addr, &(pBSInfo->m_uiResultCode));
	if( pBSInfo->m_uiProgress > 100 )
	{
		pBSInfo->m_uiProgress = 0;
	}
	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBlindScan_Cancel(struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r;
	enum AVL_DVBSx_FunctionalMode enumFunctionalMode;

	r = AVL_DVBSx_IBase_GetFunctionalMode(&enumFunctionalMode, pAVLChip);

	if(enumFunctionalMode == AVL_DVBSx_FunctMode_BlindScan)
	{
		r |= AVL_DVBSx_IBase_SendRxOP(OP_RX_HALT, pAVLChip );
	}
	else
	{
		r = AVL_DVBSx_EC_GeneralFail;
	}

	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBlindScan_ReadChannelInfo(AVL_uint16 uiStartIndex, AVL_puint16 pChannelCount, struct AVL_DVBSx_Channel * pChannel, const struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r;
	AVL_uint32 channel_addr;
	AVL_uint16 i1, i2;
	AVL_uint32 uiMinFreq;
	int iMinIdx;
	struct AVL_DVBSx_Channel sTempChannel;

	r = AVL_DVBSx_II2C_Read16(pAVLChip, rs_blind_scan_channel_count_addr, &i1);
	if( (uiStartIndex + (*pChannelCount)) > (i1) )
	{
		*pChannelCount = i1-uiStartIndex;
	}
	r |= AVL_DVBSx_II2C_Read16(pAVLChip, rc_blind_scan_channel_info_offset_addr, &i1);
	channel_addr = (sharemem_addr + i1) + uiStartIndex*sizeof(struct AVL_DVBSx_Channel);
	for( i1=0; i1<(*pChannelCount); i1++ )
	{
#if 1  //for some processors which can not read 12 bytes		
		//dump the channel information
		r |= AVL_DVBSx_II2C_Read32(pAVLChip, channel_addr, &(pChannel[i1].m_uiFrequency_kHz));
		channel_addr += 4;
		r |= AVL_DVBSx_II2C_Read32(pAVLChip, channel_addr, &(pChannel[i1].m_uiSymbolRate_Hz));
		channel_addr += 4;
		r |= AVL_DVBSx_II2C_Read32(pAVLChip, channel_addr, &(pChannel[i1].m_Flags));
		channel_addr += 4;
#endif		
	}

	// Sort the results
	for(i1=0; i1<(*pChannelCount); i1++)
	{
		iMinIdx = i1;
		uiMinFreq = pChannel[i1].m_uiFrequency_kHz;
		for(i2=(i1+1); i2<(*pChannelCount); i2++)
		{
			if(pChannel[i2].m_uiFrequency_kHz < uiMinFreq)
			{
				uiMinFreq = pChannel[i2].m_uiFrequency_kHz;
				iMinIdx = i2;
			}
		}
		sTempChannel = pChannel[iMinIdx];
		pChannel[iMinIdx] = pChannel[i1];
		pChannel[i1] = sTempChannel;
	}

	return(r);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBlindScan_Reset( const struct AVL_DVBSx_Chip * pAVLChip )
{
	AVL_DVBSx_ErrorCode r;

	r = AVL_DVBSx_II2C_Write16(pAVLChip, rc_blind_scan_reset_addr, 1);

	return(r);
}

