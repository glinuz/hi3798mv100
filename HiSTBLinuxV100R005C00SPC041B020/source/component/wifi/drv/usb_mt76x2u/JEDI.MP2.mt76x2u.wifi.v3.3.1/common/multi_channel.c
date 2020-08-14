#include	"rt_config.h"

//iversonmcc

VOID Start_MCC(RTMP_ADAPTER *pAd)
{
#if defined(RT_CFG80211_SUPPORT) && defined(CONFIG_AP_SUPPORT)
	PMULTISSID_STRUCT pMbss = &pAd->ApCfg.MBSSID[MAIN_MBSSID];
	PAPCLI_STRUCT pApCliEntry = pApCliEntry = &pAd->ApCfg.ApCliTab[MAIN_MBSSID];

	struct wifi_dev *p2p_wdev = &pMbss->wdev;
	UINT32 tssi_slope_offset_1st	 =0;
	UINT32 tssi_slope_offset_2nd	 =0;
	ULONG	Value;
	ULONG	TimeTillTbtt;
	ULONG	temp;
	// 0 :STA , 1 : AP, 2 : GC, 3 : GO 
	INT role=2; //means GC role


	RTMP_CHIP_CAP *cap = &pAd->chipCap;

	DBGPRINT(RT_DEBUG_ERROR,("start mcc !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"));
	/*because Start_time will channel channel 2to channel 1 , we put GO in Channel 2*/
	pAd->Mlme.bStartMcc = TRUE;

 	pAd->Mlme.channel_1st_bw = pAd->StaCfg.wdev.bw;
	if (pAd->Mlme.channel_1st_bw == HT_BW_20)
		pAd->Mlme.channel_1st_channel = pAd->StaCfg.wdev.channel;
	else
		pAd->Mlme.channel_1st_channel = pAd->StaCfg.wdev.CentralChannel;


       if (pAd->Mlme.channel_1st_bw == BW_40) {
		if (pAd->StaCfg.wdev.CentralChannel > pAd->StaCfg.wdev.channel) {
				pAd->Mlme.channel_1st_ext = EXTCHA_ABOVE;
	 	} 
		else if (pAd->StaCfg.wdev.CentralChannel < pAd->StaCfg.wdev.channel){
				pAd->Mlme.channel_1st_ext = EXTCHA_BELOW;
		}
       }

//	pAd->CommonCfg.Channel = pAd->Mlme.channel_1st_channel;
//	pAd->Mlme.channel_1st_channel = pAd->MlmeAux.InfraChannel ;
	if (RTMP_CFG80211_VIF_P2P_GO_ON(pAd))
	{
		p2p_wdev = &pMbss->wdev;	
		
	 	pAd->Mlme.channel_2nd_bw = p2p_wdev->bw;
		if (pAd->Mlme.channel_2nd_bw == HT_BW_20)
			pAd->Mlme.channel_2nd_channel = p2p_wdev->channel;
		else
			pAd->Mlme.channel_2nd_channel = p2p_wdev->CentralChannel;

	       if (pAd->Mlme.channel_2nd_bw == BW_40) {
			if (p2p_wdev->CentralChannel > p2p_wdev->channel) {
				pAd->Mlme.channel_2nd_ext = EXTCHA_ABOVE;
		 	} 
			else if (p2p_wdev->CentralChannel <  p2p_wdev->channel){
				pAd->Mlme.channel_2nd_ext = EXTCHA_BELOW;
			}
       		}
	}
	else if (RTMP_CFG80211_VIF_P2P_CLI_ON(pAd))
	{

		p2p_wdev = &(pApCliEntry->wdev);
		
	 	pAd->Mlme.channel_2nd_bw = p2p_wdev->bw;
		if (pAd->Mlme.channel_2nd_bw == HT_BW_20)
			pAd->Mlme.channel_2nd_channel = p2p_wdev->channel;
		else
			pAd->Mlme.channel_2nd_channel = p2p_wdev->CentralChannel;


	       if (pAd->Mlme.channel_2nd_bw == BW_40) {
			if (p2p_wdev->CentralChannel > p2p_wdev->channel) {
				pAd->Mlme.channel_2nd_ext = EXTCHA_ABOVE;
		 	} 
			else if (p2p_wdev->CentralChannel <  p2p_wdev->channel){
				pAd->Mlme.channel_2nd_ext = EXTCHA_BELOW;
			}
		}
	}
		

	DBGPRINT(RT_DEBUG_ERROR,("pAd->Mlme.channel_1st_channel %d pAd->Mlme.channel_1st_staytime %d\n",pAd->Mlme.channel_1st_channel,pAd->Mlme.channel_1st_staytime));
	DBGPRINT(RT_DEBUG_ERROR,("pAd->Mlme.channel_1st_bw %d pAd->Mlme.channel_1st_ext %d \n",pAd->Mlme.channel_1st_bw,pAd->Mlme.channel_1st_ext));
	DBGPRINT(RT_DEBUG_ERROR,("pAd->Mlme.channel_2nd_channel %d pAd->Mlme.channel_2nd_staytime %d\n",pAd->Mlme.channel_2nd_channel,pAd->Mlme.channel_2nd_staytime));
	DBGPRINT(RT_DEBUG_ERROR,("pAd->Mlme.channel_2nd_bw %d pAd->Mlme.channel_2nd_ext %d \n",pAd->Mlme.channel_2nd_bw,pAd->Mlme.channel_2nd_ext));
	DBGPRINT(RT_DEBUG_ERROR,("pAd->Mlme.switch_idle_time %d\n",pAd->Mlme.switch_idle_time));

	if (pAd->Mlme.channel_1st_channel <= 14) {
		tssi_slope_offset_1st &= ~TSSI_PARAM2_SLOPE0_MASK;
		tssi_slope_offset_1st |= TSSI_PARAM2_SLOPE0(cap->tssi_0_slope_g_band);
		tssi_slope_offset_1st &= ~TSSI_PARAM2_SLOPE1_MASK;
		tssi_slope_offset_1st |= TSSI_PARAM2_SLOPE1(cap->tssi_1_slope_g_band);
		tssi_slope_offset_1st &= ~TSSI_PARAM2_OFFSET0_MASK;
		tssi_slope_offset_1st |= TSSI_PARAM2_OFFSET0(cap->tssi_0_offset_g_band);
		tssi_slope_offset_1st &= ~TSSI_PARAM2_OFFSET1_MASK;
		tssi_slope_offset_1st |= TSSI_PARAM2_OFFSET1(cap->tssi_1_offset_g_band);
	} else {
		tssi_slope_offset_1st &= ~TSSI_PARAM2_SLOPE0_MASK;
		tssi_slope_offset_1st |= TSSI_PARAM2_SLOPE0(cap->tssi_0_slope_a_band[get_chl_grp(pAd->Mlme.channel_1st_channel)]);
		tssi_slope_offset_1st &= ~TSSI_PARAM2_SLOPE1_MASK;
		tssi_slope_offset_1st |= TSSI_PARAM2_SLOPE1(cap->tssi_1_slope_a_band[get_chl_grp(pAd->Mlme.channel_1st_channel)]);
		tssi_slope_offset_1st &= ~TSSI_PARAM2_OFFSET0_MASK;
		tssi_slope_offset_1st |= TSSI_PARAM2_OFFSET0(cap->tssi_0_offset_a_band[get_chl_grp(pAd->Mlme.channel_1st_channel)]);
		tssi_slope_offset_1st &= ~TSSI_PARAM2_OFFSET1_MASK;
		tssi_slope_offset_1st |= TSSI_PARAM2_OFFSET1(cap->tssi_1_offset_a_band[get_chl_grp(pAd->Mlme.channel_1st_channel)]);
	}

	//apcli1
	if (pAd->Mlme.channel_2nd_channel <= 14) {
		tssi_slope_offset_2nd &= ~TSSI_PARAM2_SLOPE0_MASK;
		tssi_slope_offset_2nd |= TSSI_PARAM2_SLOPE0(cap->tssi_0_slope_g_band);
		tssi_slope_offset_2nd &= ~TSSI_PARAM2_SLOPE1_MASK;
		tssi_slope_offset_2nd |= TSSI_PARAM2_SLOPE1(cap->tssi_1_slope_g_band);
		tssi_slope_offset_2nd &= ~TSSI_PARAM2_OFFSET0_MASK;
		tssi_slope_offset_2nd |= TSSI_PARAM2_OFFSET0(cap->tssi_0_offset_g_band);
		tssi_slope_offset_2nd &= ~TSSI_PARAM2_OFFSET1_MASK;
		tssi_slope_offset_2nd |= TSSI_PARAM2_OFFSET1(cap->tssi_1_offset_g_band);
	} else {
		tssi_slope_offset_2nd &= ~TSSI_PARAM2_SLOPE0_MASK;
		tssi_slope_offset_2nd |= TSSI_PARAM2_SLOPE0(cap->tssi_0_slope_a_band[get_chl_grp(pAd->Mlme.channel_2nd_channel)]);
		tssi_slope_offset_2nd &= ~TSSI_PARAM2_SLOPE1_MASK;
		tssi_slope_offset_2nd |= TSSI_PARAM2_SLOPE1(cap->tssi_1_slope_a_band[get_chl_grp(pAd->Mlme.channel_2nd_channel)]);
		tssi_slope_offset_2nd &= ~TSSI_PARAM2_OFFSET0_MASK;
		tssi_slope_offset_2nd |= TSSI_PARAM2_OFFSET0(cap->tssi_0_offset_a_band[get_chl_grp(pAd->Mlme.channel_2nd_channel)]);
		tssi_slope_offset_2nd &= ~TSSI_PARAM2_OFFSET1_MASK;
		tssi_slope_offset_2nd |= TSSI_PARAM2_OFFSET1(cap->tssi_1_offset_a_band[get_chl_grp(pAd->Mlme.channel_2nd_channel)]);
	}	


	pAd->chipCap.tssi_enable = FALSE; //let host don't  do tssi	

	RTMP_IO_READ32(pAd, TBTT_TIMER, &TimeTillTbtt);
	TimeTillTbtt = TimeTillTbtt&0x1ffff;
	DBGPRINT(RT_DEBUG_ERROR,("   .!!!!TimeTillTbtt =  %ld  \n", TimeTillTbtt));
	
	RTMP_IO_READ32(pAd, TSF_TIMER_DW0, &Value);
	DBGPRINT(RT_DEBUG_ERROR,("   .!!!!Current Tsf LSB = = %ld \n",  Value));
	temp = TimeTillTbtt*64+Value;
	DBGPRINT(RT_DEBUG_ERROR,("   .!!!!Tsf LSB + TimeTillTbtt= %ld \n", temp));
	/* Wait five beacon 204800 for 2 beacon interval.   */
	pAd->GONoASchedule.StartTime = Value + TimeTillTbtt*64 + 204800 + (pAd->Mlme.channel_2nd_staytime*512);

	pAd->GONoASchedule.Duration=(pAd->Mlme.channel_1st_staytime + pAd->Mlme.switch_idle_time) *1024;
	pAd->GONoASchedule.Interval= ((pAd->Mlme.channel_2nd_staytime + pAd->Mlme.switch_idle_time) *1024) + (pAd->GONoASchedule.Duration);
	DBGPRINT(RT_DEBUG_ERROR,("pAd->GONoASchedule.Duration %ld \n",pAd->GONoASchedule.Duration));
	DBGPRINT(RT_DEBUG_ERROR,("pAd->GONoASchedule.Interval %ld \n",pAd->GONoASchedule.Interval));

	/*count 200 , noa count will always 200 , not to 0*/
	pAd->GONoASchedule.Count = 200; /*wait 4 beacon + (interval * 4)*/ 


	if (RTMP_CFG80211_VIF_P2P_GO_ON(pAd))
	{
		role =3; //means GO
	}
	else // GC dont' care NOA start time
	{
		pAd->GONoASchedule.StartTime = 0 ;
	}

	andes_mcc_start(pAd, pAd->Mlme.channel_1st_channel, pAd->Mlme.channel_2nd_channel, pAd->Mlme.channel_1st_bw,pAd->Mlme.channel_2nd_bw,
	pAd->Mlme.channel_1st_ext,pAd->Mlme.channel_2nd_ext,0,role, pAd->Mlme.channel_1st_staytime ,pAd->Mlme.channel_2nd_staytime ,
	pAd->Mlme.switch_idle_time,1,tssi_slope_offset_1st, tssi_slope_offset_2nd,pAd->GONoASchedule.StartTime);
#endif /* defined(RT_CFG80211_SUPPORT) && defined(CONFIG_AP_SUPPORT) */
}

/*
channel : 0 ==> don't care
		1 ==> park at channel 0 (means infra channel)
		2 ==> park at channel 1 (means GO/GC channel)

*/
VOID Stop_MCC(RTMP_ADAPTER *pAd,INT channel)
{
#if defined(RT_CFG80211_SUPPORT) && defined(CONFIG_AP_SUPPORT)
		PMULTISSID_STRUCT pMbss = &pAd->ApCfg.MBSSID[MAIN_MBSSID];
		PAPCLI_STRUCT pApCliEntry = pApCliEntry = &pAd->ApCfg.ApCliTab[MAIN_MBSSID];
		struct wifi_dev *p2p_wdev = &pMbss->wdev;

		pAd->Mlme.bStartMcc = FALSE;
		pAd->chipCap.tssi_enable = TRUE; //let host don't  do tssi	

		printk("Stop_MCC swtich to channel %d \n",channel);
		andes_mcc_stop(pAd, channel, 0, 0, 0); //park at p2p GO| CLI Cannel

		if (channel == 1 )
		{
			AsicSwitchChannel(pAd, pAd->StaCfg.wdev.CentralChannel, FALSE);
			bbp_set_bw(pAd, pAd->StaCfg.wdev.bw);
		}
		else if(channel == 2 )
		{
			if (RTMP_CFG80211_VIF_P2P_GO_ON(pAd))
				p2p_wdev = &pMbss->wdev;	
			else if (RTMP_CFG80211_VIF_P2P_CLI_ON(pAd))
				p2p_wdev = &(pApCliEntry->wdev);


			AsicSwitchChannel(pAd, p2p_wdev->CentralChannel, FALSE);
			bbp_set_bw(pAd, p2p_wdev->bw);

		}
		else
		{
			AsicSwitchChannel(pAd, pAd->StaCfg.wdev.CentralChannel, FALSE);
			bbp_set_bw(pAd, pAd->StaCfg.wdev.bw);
		}
#endif /* defined(RT_CFG80211_SUPPORT) && defined(CONFIG_AP_SUPPORT) */
}


