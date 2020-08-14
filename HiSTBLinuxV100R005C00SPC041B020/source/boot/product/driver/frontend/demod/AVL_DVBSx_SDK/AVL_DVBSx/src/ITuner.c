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


//$Revision: 258 $ 
//$Date: 2008-02-06 14:05:26 -0500 (Wed, 06 Feb 2008) $
// 
#include "avl_dvbsx.h"
#include "ITuner.h"
#include "II2C.h"

AVL_DVBSx_ErrorCode AVL_DVBSx_ITuner_CalculateLPF(AVL_uint16 uiSymbolRate_10kHz, struct AVL_Tuner * pTuner)
{
	AVL_uint32 lpf = uiSymbolRate_10kHz;
	lpf *= 81;
	lpf /= 1000;
	lpf += 50;
	pTuner->m_uiLPF_100kHz = (AVL_uint16)lpf;
	return(AVL_DVBSx_EC_OK);
}
