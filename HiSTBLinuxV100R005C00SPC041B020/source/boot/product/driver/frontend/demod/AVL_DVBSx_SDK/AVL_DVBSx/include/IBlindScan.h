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
/// @brief Declares functions for blind scan.
/// 
/// @details The Availink device can store up to 120 detected carrier channels. 
/// If more than 120 channels are detected in the scan range, the function ::AVL_DVBSx_IBlindScan_GetScanStatus will 
/// set ::AVL_DVBSx_BlindScanPara.m_uiResultCode to 1.
/// The user can read blind scan results with the function ::AVL_DVBSx_IBlindScan_ReadChannelInfo. 
/// 
//$Revision: 985 $ 
//$Date: 2009-11-09 18:09:24 -0500 (Mon, 09 Nov 2009) $
// 
#ifndef IBlindScan_h_h
	#define IBlindScan_h_h

	#include "avl_dvbsx.h"
	#include "ITuner.h"
	#include "avl_dvbsx_globals.h"

	#ifdef AVL_CPLUSPLUS
extern "C" {
	#endif

	///@cond

	/// @endcond

	/// Stores the blind scan parameters which are passed to the ::AVL_DVBSx_IBlindScan_Scan function.
	struct AVL_DVBSx_BlindScanPara
	{
		AVL_uint16 m_uiStartFreq_100kHz;	///< The start scan frequency in units of 100kHz. The minimum value depends on the tuner specification. 
		AVL_uint16 m_uiStopFreq_100kHz;		///< The stop scan frequency in units of 100kHz. The maximum value depends on the tuner specification.
		AVL_uint16 m_uiMinSymRate_kHz;		///< The minimum symbol rate to be scanned in units of kHz. The minimum value is 1000 kHz.
		AVL_uint16 m_uiMaxSymRate_kHz;		///< The maximum symbol rate to be scanned in units of kHz. The maximum value is 45000 kHz.
	};

	/// Stores the blind scan status information.
	struct AVL_DVBSx_BlindScanInfo
	{
		AVL_uint16 m_uiProgress;				///< The percentage completion of the blind scan procedure. A value of 100 indicates that the blind scan is finished.
		AVL_uint16 m_uiChannelCount;			///< The number of channels detected thus far by the blind scan operation.  The Availink device can store up to 120 detected channels.
		AVL_uint16 m_uiNextStartFreq_100kHz;	///< The start frequency of the next scan in units of 100kHz.
		AVL_uint16 m_uiResultCode;				///< The result of the blind scan operation.  Possible values are:  0 - blind scan operation normal; 1 -- more than 120 channels have been detected.
	};

	/// Performs a blind scan operation. Call the function ::AVL_DVBSx_IBlindScan_GetScanStatus to check the status of the blind scan operation.
	/// 
	/// @param pBSPara  A pointer to the blind scan configuration parameters.
	/// @param uiTumerLPF_100kHz The tuner LPF bandwidth setting in units of 100 kHz.
	/// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object on which blind scan is being performed.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the scan command is successfully sent to the Availink device.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// Return ::AVL_DVBSx_EC_Running if the scan command could not be sent because the Availink device is still processing a previous command.
	/// Return ::AVL_DVBSx_EC_GeneralFail if the device is not in the blind scan functional mode or if the parameter \a pBSPara->m_uiStartFreq_100kHz is larger than the parameter \a pBSPara->m_uiStopFreq_100kHz
	AVL_DVBSx_ErrorCode AVL_DVBSx_IBlindScan_Scan(struct AVL_DVBSx_BlindScanPara * pBSPara, AVL_uint16 uiTunerLPF_100kHz, const struct AVL_DVBSx_Chip * pAVLChip );

	/// Queries the blind scan status.
	/// 
	/// @param pBSInfo  Pointer to the object in which the blind scan status is to be stored.
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip object for which the blind scan status is being queried.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the blind scan status has been retrieved.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IBlindScan_GetScanStatus(struct AVL_DVBSx_BlindScanInfo * pBSInfo, const struct AVL_DVBSx_Chip * pAVLChip );

	/// Cancels the current blind scan operation.
	/// 
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip object for which the blind scan operation is being canceled.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the Availink device has been commanded to cancel the blind scan operation.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// Return ::AVL_DVBSx_EC_Running if the blind scan could not be canceled because the device is still processing a previous command.
	/// @remarks This function sends a cancel command to the Availink device. The internal scan is not be canceled until the current scan section is finished. 
	/// Call ::AVL_DVBSx_IBase_GetRxOPStatus to determine when the scan cancellation is complete.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IBlindScan_Cancel(struct AVL_DVBSx_Chip * pAVLChip );

	/// Retrieves the blind scan results.
	/// 
	/// @param uiStartIndex  The blind scan results are stored in an array internal to the Availink device. This parameter tells the function the array index at which to retrieve the results.
	/// @param pChannelCount The number of channel results to be retrieved.  The function updates this value with the actual number of channel results that are reported.
	/// @param pChannel Pointer to an object in which the blind scan results are to be stored.
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip object for which the blind scan results are being retrieved.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the blind scan results have been retrieved.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// @remarks The scan results internal to the Availink device are overwritten after a subsequent call to the ::AVL_DVBSx_IBlindScan_Reset function. Be sure to read out all of the channel information before calling ::AVL_DVBSx_IBlindScan_Reset.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IBlindScan_ReadChannelInfo(AVL_uint16 uiStartIndex, AVL_puint16 pChannelCount, struct AVL_DVBSx_Channel * pChannel, const struct AVL_DVBSx_Chip * pAVLChip );

	/// Resets the Availink device internal blind scan results.
	/// 
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip object for which the internal blind scan results are being reset.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the reset operation is successful.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IBlindScan_Reset( const struct AVL_DVBSx_Chip * pAVLChip );

	#ifdef AVL_CPLUSPLUS
}
	#endif

#endif
