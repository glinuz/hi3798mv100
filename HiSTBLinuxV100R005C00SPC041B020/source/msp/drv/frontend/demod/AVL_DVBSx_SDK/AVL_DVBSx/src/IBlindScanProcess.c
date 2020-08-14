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


//$Revision: 1839 $ 
//$Date: 2010-12-15 23:17:24 -0400 (TUE, 15 Dec 2010) $
// 


#include "IBSP.h"
#include "avl_dvbsx.h"
#include "IBase.h"
#include "IRx.h"
#include "ITuner.h"
#include "IBlindScan.h"
#include "IBlindScanProcess.h"



AVL_DVBSx_ErrorCode AVL_DVBSx_IBlindScanProcess_LockChannel(struct AVL_DVBSx_Chip  *pAVL_DVBSxChip,struct AVL_Tuner *pAVL_Tuner,AVL_uint32 Freq_MHz, AVL_uint32 SymbolRate_Hz)
{
	struct AVL_DVBSx_Channel Channel;  
	AVL_DVBSx_ErrorCode r=AVL_DVBSx_EC_OK;
	AVL_uint16 uiLockStatus=0;
	AVL_uint32 uiTimeth; 
	AVL_uint16 uiCounter;
	AVL_uint16 i= 0;
	AVL_uint32 uiOpStatusTimeOut = 0;

	struct AVL_DVBSx_Chip * pAVLChip = pAVL_DVBSxChip;		
	struct AVL_Tuner * pTuner =pAVL_Tuner;	


	//Change the value defined by macro and reinitialize the tuner frequency  when we want to lock to a new channel.
	pTuner->m_uiFrequency_100kHz = Freq_MHz*10;     
	pTuner->m_uiSymbolRate_Hz = SymbolRate_Hz;
	AVL_DVBSx_ITuner_CalculateLPF((AVL_uint16)(SymbolRate_Hz/10000), pTuner);
	//pTuner->m_uiLPF_100kHz = 340;

	//Halt CPU to improve the tuner locking speed. It's the optional operation before locking tuner.
	r = AVL_DVBSx_IBase_SendRxOP(OP_RX_HALT, pAVLChip );
	if(AVL_DVBSx_EC_OK == r)
	{
		while(i++<20)
		{
			r = AVL_DVBSx_IBase_GetRxOPStatus(pAVLChip);
			if(AVL_DVBSx_EC_OK == r)
			{
				break;
			}
			else
			{
				//AVL_DVBSx_IBSP_Delay(10);
                tuner_mdelay(10);
			}
		}
	}
	else
	{
		return (r);
	}

	//This function should be called after initialization to lock the tuner.
	r = pTuner->m_pLockFunc(pTuner);
	if(AVL_DVBSx_EC_OK != r)
	{
		return (r);
	}
	//AVL_DVBSx_IBSP_Delay(150);		//Wait a while for tuner to lock in certain frequency.
    tuner_mdelay(150);

	//This function should be called to check the lock status of the tuner.
	r = pTuner->m_pGetLockStatusFunc(pTuner);
	if (AVL_DVBSx_EC_OK != r)
	{
		return (r);
	}

	Channel.m_uiSymbolRate_Hz = SymbolRate_Hz;      //Change the value defined by macro when we want to lock to a new channel.
	Channel.m_Flags = (CI_FLAG_IQ_NO_SWAPPED) << CI_FLAG_IQ_BIT;	//Normal IQ
	Channel.m_Flags |= (CI_FLAG_IQ_AUTO_BIT_AUTO) << CI_FLAG_IQ_AUTO_BIT;	//Enable automatic IQ swap detection
	Channel.m_Flags |= (CI_FLAG_DVBS2_UNDEF) << CI_FLAG_DVBS2_BIT;			//Enable automatic standard detection

	//This function should be called after lock tuner operation to  to the channel.
	r = AVL_DVBSx_IRx_LockChannel(&Channel, pAVLChip);  
	if (AVL_DVBSx_EC_OK != r)
	{
		return (r);
	}
	
	//GetRxOPStatus function should be called after lock channel to make sure the device has finished processing the lock command.
	do
	{
		r = AVL_DVBSx_IBase_GetRxOPStatus(pAVLChip);
		if(AVL_DVBSx_EC_OK == r)
		{
			break;
		}
		//AVL_DVBSx_IBSP_Delay(10);
        tuner_mdelay(10);
	}while(uiOpStatusTimeOut++ < 20);

	//Channel lock time increases when symbol rate decreases.USe max wait time based on different symbol rates.
	if(SymbolRate_Hz<5000000)
	{
		uiTimeth=5000*2;       //The max waiting time is 5000ms,considering the IQ swapped status the time should be doubled.
	}
	else if(SymbolRate_Hz<10000000)
	{
		uiTimeth=600*2;        //The max waiting time is 600ms,considering the IQ swapped status the time should be doubled.
	}
	else
	{
		uiTimeth=250*2;        //The max waiting time is 250ms,considering the IQ swapped status the time should be doubled.
	} 
	uiCounter=uiTimeth/10;
	do
	{
		//This function should be called to check the lock status of the demod.
		r = AVL_DVBSx_IRx_GetLockStatus(&uiLockStatus, pAVLChip);
		if ((AVL_DVBSx_EC_OK == r)&&(1 == uiLockStatus))
		{
			break;
		}
		AVL_DVBSx_IBSP_Delay(10);    //Wait 10ms for demod to lock the channel.
	}while(--uiCounter);
	if(0==uiCounter)
	{
		return (AVL_DVBSx_EC_TimeOut);
	}
	//printf("            Ch Freq:%dMHz R:%dHz locked\n",Freq_MHz,SymbolRate_Hz);
	return (r);
}

AVL_uint16 AVL_DVBSx_IBlindScanProcess_JudgeChannels(const struct AVL_DVBSx_Channel *pTemp, const struct AVL_DVBSx_Channel *pValid)
{
	AVL_uint16 flag = 0;

	if(pTemp->m_uiSymbolRate_Hz >= FILTER_SYMBOL_RATE_20MHZ)
	{
		if( (AVL_abssub(pValid->m_uiFrequency_kHz , pTemp->m_uiFrequency_kHz)<SAME_HIGH_SR_FREQUENCY_DEVIANCE_KHZ) && (AVL_abssub(pValid->m_uiSymbolRate_Hz , pTemp->m_uiSymbolRate_Hz)<SAME_HIGH_SR_DEVIANCE_HZ) )
		{
			flag = 1;
		}	
	}
	else if(pTemp->m_uiSymbolRate_Hz >= FILTER_SYMBOL_RATE_10MHZ)
	{
		if( (AVL_abssub(pValid->m_uiFrequency_kHz , pTemp->m_uiFrequency_kHz)<SAME_LOW_SR_FREQUENCY_DEVIANCE_KHZ) && (AVL_abssub(pValid->m_uiSymbolRate_Hz , pTemp->m_uiSymbolRate_Hz)<SAME_MIDDLE_SR_DEVIANCE_HZ) )
		{
			flag = 1;
		}	
	}
	else  //less than 10MHz
	{
		if( (AVL_abssub(pValid->m_uiFrequency_kHz , pTemp->m_uiFrequency_kHz)<SAME_LOW_SR_FREQUENCY_DEVIANCE_KHZ) && (AVL_abssub(pValid->m_uiSymbolRate_Hz , pTemp->m_uiSymbolRate_Hz)<SAME_LOW_SR_DEVIANCE_HZ) )
		{
			flag = 1;
		}	
	}

	if(AVL_abssub(pValid->m_uiFrequency_kHz , pTemp->m_uiFrequency_kHz) < ((pValid->m_uiSymbolRate_Hz +  pTemp->m_uiSymbolRate_Hz)/2000))
	{
		flag = 1;
	}

	return flag;
}

void AVL_DVBSx_IBlindScanProcess_ChannelsQsort(struct AVL_DVBSx_Channel *Ch_list_valid, AVL_uint16 original_channels_count,AVL_uint16 new_add_channels_count)
{
	struct AVL_DVBSx_Channel Ch_temp;
	AVL_uint16 i =0;
	AVL_uint16 index =0; // index of the newly added channel.
	if(Ch_list_valid == AVL_NULL)
		return;
	for(i=0; i< new_add_channels_count;i++)
	{
		index = original_channels_count + i;    
		while(index > 0)
		{
			if(Ch_list_valid[index].m_uiFrequency_kHz >= Ch_list_valid[index-1].m_uiFrequency_kHz)
			{
				break;  //No need to qsort for this index. it is OK.
			}
			else
			{
				Ch_temp = Ch_list_valid[index-1];
				Ch_list_valid[index-1] = Ch_list_valid[index] ;
				Ch_list_valid[index] = Ch_temp;
				index--;
			}
		}
	}
}

void AVL_DVBSx_IBlindScanProcess_ChannelsFilter(struct AVL_DVBSx_Channel *Ch_list_valid, AVL_uint16 *uiValid_ChCount,AVL_uint16 maximum_channels_count, struct AVL_DVBSx_Channel *Ch_list_Temp, AVL_uint16 uiChCount_Found)
{
	AVL_uchar i,j,flag;
	AVL_uint16 uiCount =*uiValid_ChCount;
	struct AVL_DVBSx_Channel *pTemp;
	struct AVL_DVBSx_Channel *pValid;
	AVL_uint32 uiSymbolRate_Hz;
	AVL_uint32 ui_SR_offset;

	for(i=0;i< uiChCount_Found;i++)
	{
		pTemp = &Ch_list_Temp[i];
		flag =0;
		for(j=0;j<*uiValid_ChCount;j++)
		{
			pValid = &Ch_list_valid[j];
			if(AVL_DVBSx_IBlindScanProcess_JudgeChannels(pTemp,pValid))
			{
				flag = 1;
				break;
			}  
		}
		if(0 == flag)
		{
			Ch_list_valid[uiCount].m_Flags = pTemp->m_Flags;
			Ch_list_valid[uiCount].m_uiSymbolRate_Hz = pTemp->m_uiSymbolRate_Hz;
			Ch_list_valid[uiCount].m_uiFrequency_kHz = pTemp->m_uiFrequency_kHz;

			//adjust symbol rate offset
			uiSymbolRate_Hz = Ch_list_valid[uiCount].m_uiSymbolRate_Hz;
			ui_SR_offset = ((uiSymbolRate_Hz%10000)>5000)?(10000-(uiSymbolRate_Hz%10000)):(uiSymbolRate_Hz%10000);
			if( ((uiSymbolRate_Hz>10000000) && (ui_SR_offset<4000)) || ((uiSymbolRate_Hz>5000000) && (ui_SR_offset<2000))  )
				uiSymbolRate_Hz =  (uiSymbolRate_Hz%10000<5000)?(uiSymbolRate_Hz-ui_SR_offset):(uiSymbolRate_Hz+ui_SR_offset);

			ui_SR_offset = ((uiSymbolRate_Hz%1000)>500)?(1000-(uiSymbolRate_Hz%1000)):(uiSymbolRate_Hz%1000);
			if( (uiSymbolRate_Hz<5000000) && (ui_SR_offset< 500))
				uiSymbolRate_Hz =  (uiSymbolRate_Hz%1000<500)?(uiSymbolRate_Hz-ui_SR_offset):(uiSymbolRate_Hz+ui_SR_offset);

			Ch_list_valid[uiCount].m_uiSymbolRate_Hz =1000*(uiSymbolRate_Hz/1000);

			uiCount++;
			if(uiCount >= maximum_channels_count)
			{
				break;
			}
		}		
	}

	*uiValid_ChCount = uiCount;
}


AVL_DVBSx_ErrorCode AVL_DVBSx_IBlindScanProcess_UpdateProgress(AVL_uint16 *bsProgress,AVL_uint16 CurrentFreq, struct AVL_DVBSx_BlindScanProcessData *bsData)
{
	AVL_uint16 Progress_Temp;
	AVL_uint32 Caculate_Temp;
	if(bsProgress == AVL_NULL || bsData == AVL_NULL)
		return AVL_DVBSx_EC_GeneralFail;

	if(CurrentFreq < bsData->bsPara.m_uiStartFreq_100kHz)
		return AVL_DVBSx_EC_GeneralFail;

	Caculate_Temp = (AVL_uint32)(CurrentFreq - bsData->bsPara.m_uiStartFreq_100kHz) *100 ;
	Progress_Temp = (AVL_uint16)(Caculate_Temp/(bsData->bsPara.m_uiStopFreq_100kHz -bsData->bsPara.m_uiStartFreq_100kHz));
	if(Progress_Temp <=100)
	{
		*bsProgress = Progress_Temp;
		return AVL_DVBSx_EC_OK;
	}
	return AVL_DVBSx_EC_GeneralFail;
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBlindScanProcess_ReadChannels(struct AVL_DVBSx_Chip * pAVLChip,struct AVL_DVBSx_BlindScanProcessData *bsData,AVL_uint16 Channels_Count)
{
	AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
	AVL_uint16 start_index = 0;
	AVL_uint16 read_count = 0;
	AVL_uint16 channels_count_temp;
	AVL_uint16 new_channels_count;

	struct AVL_DVBSx_Channel channels_Temp[10];

	while(Channels_Count)
	{
		if(Channels_Count > 10)
		{
			read_count = 10;
			Channels_Count -= read_count;	
		}
		else
		{
			read_count = Channels_Count;
			Channels_Count = 0;
		}

		r = AVL_DVBSx_IBlindScan_ReadChannelInfo(start_index, &read_count, channels_Temp, pAVLChip);
		start_index += read_count;
		if(r == AVL_DVBSx_EC_OK)
		{
			channels_count_temp = bsData->m_uiChannelCount;
			AVL_DVBSx_IBlindScanProcess_ChannelsFilter(bsData->channel_buf, &channels_count_temp, MAXIMUM_CHANNELS_COUNT, channels_Temp ,read_count);  //Will update bsData->m_uiChannelCount.
			new_channels_count = channels_count_temp - bsData->m_uiChannelCount;
			AVL_DVBSx_IBlindScanProcess_ChannelsQsort(bsData->channel_buf,bsData->m_uiChannelCount,new_channels_count);
			bsData->m_uiLastFoundCHcount = new_channels_count;
		}
		else
		{
			break;
		}
	}

	return r;  
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBlindScanProcess_ScanStart(struct AVL_DVBSx_Chip * pAVLChip, struct AVL_Tuner * pTuner,struct AVL_DVBSx_BlindScanProcessData *bsData)
{
	AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
	struct AVL_DVBSx_BlindScanPara bsPara;
	AVL_uint16 Start_Freq_100KHz;	

	r = AVL_DVBSx_IBase_SetFunctionalMode(pAVLChip, AVL_DVBSx_FunctMode_BlindScan);	

	if( bsData->m_uiNextStartFreq_100KHz < bsData->bsPara.m_uiStartFreq_100kHz)
		Start_Freq_100KHz = bsData->bsPara.m_uiStartFreq_100kHz;
	else
		Start_Freq_100KHz = bsData->m_uiNextStartFreq_100KHz;

	bsPara.m_uiStartFreq_100kHz = Start_Freq_100KHz;
	bsPara.m_uiStopFreq_100kHz = bsData->bsPara.m_uiStopFreq_100kHz;
	bsPara.m_uiMaxSymRate_kHz = bsData->bsPara.m_uiMaxSymRate_kHz;
	bsPara.m_uiMinSymRate_kHz = bsData->bsPara.m_uiMinSymRate_kHz;

	r |= AVL_DVBSx_IBlindScan_Reset(pAVLChip);

	if(bsPara.m_uiStartFreq_100kHz >= bsPara.m_uiStopFreq_100kHz)
	{	
		bsData->m_eBSStatusFlag = AVL_DVBSx_BS_Status_Stop;
		return AVL_DVBSx_EC_GeneralFail;  //Do not need to do scan
	}

	bsPara.m_uiStopFreq_100kHz = bsPara.m_uiStartFreq_100kHz + bsData->m_uiTuner_FlatLPF_100KHz*2;  //Use flat LPF of 320 to suit all the tuners.

	if(bsPara.m_uiStopFreq_100kHz >= bsData->bsPara.m_uiStopFreq_100kHz)
	{
		bsPara.m_uiStopFreq_100kHz = bsData->bsPara.m_uiStopFreq_100kHz;  //This sentence can be removed, if the user wishes to scan the TS at the Stop_Freq.
		bsData->m_uiScanStopFlag  = 1 ;//Used as  a flag to inform blind scan is over.
	}

	//Setup the tuner before scan
	pTuner->m_uiFrequency_100kHz = (bsPara.m_uiStartFreq_100kHz + bsPara.m_uiStopFreq_100kHz)/2; 
	pTuner->m_uiLPF_100kHz = bsData->m_uiTuner_MaximumLPF_100KHz; 
	pTuner->m_uiSymbolRate_Hz = 30000000; //Default use 30Msps replace to unidentified value.

	r = pTuner->m_pLockFunc(pTuner);   //Lock the tuner.

	AVL_DVBSx_IBSP_Delay(100);		//wait a while for tuner to lock in certain frequency.
	r|= pTuner->m_pGetLockStatusFunc(pTuner);	 //Check the lock status of the tuner.
	if (AVL_DVBSx_EC_OK != r)		 
	{
		//printf("Tuner can not lock to %d MHz !!!\n", pTuner->m_uiFrequency_100kHz/10 );
		return AVL_DVBSx_EC_GeneralFail;
	}
	//printf("Tuner has lock to %d MHz !!!\n", pTuner->m_uiFrequency_100kHz/10 );

	//This function should be called after tuner lock to perform a blind scan.
	r |= AVL_DVBSx_IBlindScan_Scan(&bsPara,bsData->m_uiTuner_MaximumLPF_100KHz, pAVLChip);

	return r;
}

//Prepare and start the blind scan process
AVL_DVBSx_ErrorCode AVL_DVBSx_IBlindScanProcess_Start(struct AVL_DVBSx_Chip * pAVLChip, struct AVL_Tuner * pTuner,struct AVL_DVBSx_BlindScanProcessData *bsData, struct AVL_DVBSx_BlindScanPara bsPara, AVL_uint16 uiTuner_MaximumLPF_100KHz,AVL_uint16 uiTuner_FlatLPF_100KHz)
{
	AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
	AVL_int16 i;

	if(pAVLChip == AVL_NULL || pTuner == AVL_NULL || 
		bsPara.m_uiMaxSymRate_kHz < bsPara.m_uiMinSymRate_kHz || bsPara.m_uiStopFreq_100kHz < bsPara.m_uiStartFreq_100kHz)
	{
		return AVL_DVBSx_EC_GeneralFail;
	}

	//Save the configure parameter
	bsData->bsPara = bsPara;  //Save the blind scan range.
	bsData->m_eBSStatusFlag = AVL_DVBSx_BS_Status_Scanning;  //Show is scanning 
	bsData->m_uiTuner_MaximumLPF_100KHz = uiTuner_MaximumLPF_100KHz;
	bsData->m_uiTuner_FlatLPF_100KHz = uiTuner_FlatLPF_100KHz;

	//Increase the frequency rang for blind scan out the channel at boundary of frequency range.
	if(bsData->bsPara.m_uiStartFreq_100kHz >= bsData->m_uiTuner_FlatLPF_100KHz)
	{
		bsData->bsPara.m_uiStartFreq_100kHz -= bsData->m_uiTuner_FlatLPF_100KHz;
	}
	bsData->bsPara.m_uiStopFreq_100kHz += bsData->m_uiTuner_FlatLPF_100KHz;
 
	bsData->m_uiChannelCount = 0;
	bsData->m_uiLockedChannelIndex = 0; 
	bsData->m_uiLastFoundCHcount = 0;
	bsData->m_uiScanStopFlag = 0;
	bsData->m_uiNextStartFreq_100KHz = 0;
	for(i =0; i< MAXIMUM_CHANNELS_COUNT; i++)
	{
		bsData->m_uiLockFlag[i] = 0;
	}
	
	r = AVL_DVBSx_IBlindScanProcess_ScanStart(pAVLChip, pTuner, bsData);

	return r;
}

//Update the blind scan progress and continue the blind scan process
enum AVL_DVBSx_BlindscanProcessStatus AVL_DVBSx_IBlindScanProcess_ScanLock(struct AVL_DVBSx_Chip * pAVLChip, struct AVL_Tuner * pTuner,struct AVL_DVBSx_BlindScanProcessData *bsData,AVL_uint16 *bsProgress)
{
	AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
	struct AVL_DVBSx_BlindScanInfo bsInfo;

	if(pAVLChip == AVL_NULL || pTuner == AVL_NULL || bsData == AVL_NULL || bsProgress == AVL_NULL)
	{
		return AVL_DVBSx_BS_Status_Stop;
	}

	if(bsData->m_eBSStatusFlag == AVL_DVBSx_BS_Status_Stop)
	{
		return AVL_DVBSx_BS_Status_Stop;
	}


	if(bsData->m_eBSStatusFlag == AVL_DVBSx_BS_Status_Scanning)  //If it is scanning
	{
		r = AVL_DVBSx_IBlindScan_GetScanStatus(&bsInfo, pAVLChip);  //Query the internal blind scan procedure information.
		if(AVL_DVBSx_EC_OK == r)
		{
			AVL_DVBSx_IBlindScanProcess_UpdateProgress(bsProgress, bsInfo.m_uiNextStartFreq_100kHz,bsData);

			if(bsInfo.m_uiProgress == 100)
			{

				bsData->m_uiNextStartFreq_100KHz = bsInfo.m_uiNextStartFreq_100kHz;

				if(bsData->m_uiLastFoundCHcount>0)
				{
					bsData->m_uiChannelCount += bsData->m_uiLastFoundCHcount;
					bsData->m_uiLastFoundCHcount = 0;
				}

				if(bsInfo.m_uiChannelCount > 0)
				{
					r = AVL_DVBSx_IBlindScanProcess_ReadChannels(pAVLChip, bsData,bsInfo.m_uiChannelCount);	
				}

				bsData->m_eBSStatusFlag = AVL_DVBSx_BS_Status_Locked;  
				

				if(bsData->m_uiScanStopFlag)  //It is the last blind scan step.
				{
					bsData->m_uiChannelCount += bsData->m_uiLastFoundCHcount;
					bsData->m_uiLastFoundCHcount = 0;
				}
			}
		}
		else
		{
			bsData->m_eBSStatusFlag =AVL_DVBSx_BS_Status_Stop;
			return AVL_DVBSx_BS_Status_Stop;
		}
	}

	if(bsData->m_eBSStatusFlag == AVL_DVBSx_BS_Status_Locked)    //Locked status, there are no channels to lock goto next scan
	{

		while(bsData->m_uiLockedChannelIndex < bsData->m_uiChannelCount)   //If there are channels remaining to be locked
		{
			r = AVL_DVBSx_IBase_SetFunctionalMode(pAVLChip, AVL_DVBSx_FunctMode_Demod);	
			r |= AVL_DVBSx_IBlindScanProcess_LockChannel(pAVLChip,pTuner, bsData->channel_buf[bsData->m_uiLockedChannelIndex].m_uiFrequency_kHz/1000, bsData->channel_buf[bsData->m_uiLockedChannelIndex].m_uiSymbolRate_Hz);
			if(r == AVL_DVBSx_EC_OK)
			{
			bsData->m_uiLockFlag[bsData->m_uiLockedChannelIndex] = 1;
			}
			bsData->m_uiLockedChannelIndex++;
			if(r == AVL_DVBSx_EC_OK)        //Channel lock
			{
				bsData->m_eBSStatusFlag = AVL_DVBSx_BS_Status_Locked;
				return AVL_DVBSx_BS_Status_Locked;
			}	
		}

		if(!bsData->m_uiScanStopFlag)  //Go to next  scan
		{
			r = AVL_DVBSx_IBlindScanProcess_ScanStart(pAVLChip, pTuner, bsData);
			if(r ==  AVL_DVBSx_EC_OK)
			{
				bsData->m_eBSStatusFlag = AVL_DVBSx_BS_Status_Scanning;
			}
			else
			{
				bsData->m_eBSStatusFlag = AVL_DVBSx_BS_Status_Stop;
			}

		}
		else   //Stop blind scan process
		{
			bsData->m_eBSStatusFlag =AVL_DVBSx_BS_Status_Stop;
			return AVL_DVBSx_BS_Status_Stop;
		}
	}

	return AVL_DVBSx_BS_Status_Scanning;
}

//Stops the blind scan
AVL_DVBSx_ErrorCode AVL_DVBSx_IBlindScanProcess_Stop(struct AVL_DVBSx_Chip * pAVLChip, struct AVL_DVBSx_BlindScanProcessData *bsData)
{
	AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
	if(bsData == AVL_NULL)
	{
		return AVL_DVBSx_EC_GeneralFail;
	}

	//This function should be called after blind scan to placed device into demodulator mode.
	r = AVL_DVBSx_IBase_SetFunctionalMode(pAVLChip, AVL_DVBSx_FunctMode_Demod);
	bsData->m_eBSStatusFlag = AVL_DVBSx_BS_Status_Stop;
	return r;
}

