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


///
/// @file 
/// @brief The ITuner interface
/// @details The ITuner interface supports dynamic tuner switch during run time. It is the user's responsibility to to ensure that the function pointers in the tuner data structure are not NULL.
/// 
//$Revision: 355 $ 
//$Date: 2008-04-21 16:12:24 -0400 (Mon, 21 Apr 2008) $
// 
#ifndef ITuner_h_h
	#define ITuner_h_h

	#include "avl_dvbsx.h"
	#include "avl_dvbsx_globals.h"

	#ifdef AVL_CPLUSPLUS
extern "C" {
	#endif

	/// @cond

	/// @endcond

	/// The Tuner data structure
	/// 
	struct AVL_Tuner
	{
		AVL_uint16 m_uiSlaveAddress;			///< The Tuner slave address. It is the write address of the tuner device. In particular it is an 8-bit address, with the LSB set to zero. The Availink device does not support 10-bit I2C addresses. 
		AVL_uint16 m_uiI2CBusClock_kHz;		///< The clock speed of the I2C bus that is dedicated to tuner control.  The units are kHz.
		AVL_uint16 m_uiFrequency_100kHz;	///< The tuned frequency in units of 100kHz.
		AVL_uint16 m_uiLPF_100kHz;				///< The lowpass filter bandwidth of the tuner.
		AVL_uint32 m_uiSymbolRate_Hz;			///< The symbol rate of the incoming channel.
		void * m_pParameters;							///< A pointer to the tuner's customized parameters baseband gain, etc.
		struct AVL_DVBSx_Chip * m_pAVLChip;	///< A pointer to the Availink device connected to the Tuner.
		AVL_DVBSx_ErrorCode (* m_pInitializeFunc)(struct AVL_Tuner *);	 	///< A pointer to the tuner initialization function.
		AVL_DVBSx_ErrorCode (* m_pGetLockStatusFunc)(struct AVL_Tuner *); ///< A pointer to the tuner GetLockStatus function.
		AVL_DVBSx_ErrorCode (* m_pDumpDataFunc)(AVL_puchar ucpData, AVL_puchar ucpSize, struct AVL_Tuner *); 	///< A pointer to the DumpData function. This function is optional; it is used to dump debug information.
		AVL_DVBSx_ErrorCode (* m_pLockFunc)(struct AVL_Tuner *);					///< A pointer to the tuner Lock function.
	    AVL_uint16 m_tunerNum;
	};

	/// This function calculates the tuner lowpass filter bandwidth based on the symbol rate of the received signal.
	/// 	
	/// @param uiSymbolRate_10kHz  The symbol rate of the received signal in units of 10kHz.
	/// @param pTuner  A pointer to the tuner object for which the lowpass filter bandwidth is being computed. The function updates the member m_uiLPF_100kHz of this object.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the tuner lowpass filter bandwidth member of the tuner object has been updated.
	/// This function is provided as an example of how to set the lowpass filter bandwidth in a manner that is proportional to the symbol rate.  The user may use their own calculation instead.
	AVL_DVBSx_ErrorCode AVL_DVBSx_ITuner_CalculateLPF(AVL_uint16 uiSymbolRate_10kHz,struct AVL_Tuner * pTuner);

	#ifdef AVL_CPLUSPLUS
}
	#endif
#endif
