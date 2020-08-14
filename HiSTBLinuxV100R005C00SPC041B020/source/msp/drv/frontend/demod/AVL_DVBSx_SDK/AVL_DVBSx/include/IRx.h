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
/// @brief Declares functions for the receiver component.
/// @details Although the user may call these functions from multiple threads, Availink recommends that all
/// "control" functions be called from a single thread to avoid confusion. 
/// 
//$Revision: 991 $ 
//$Date: 2009-11-19 16:21:06 -0500 (Thu, 19 Nov 2009) $
// 
#ifndef IRx_h_h
	#define IRx_h_h

	#include "avl_dvbsx.h"
	#include "avl_dvbsx_globals.h"

	#ifdef AVL_CPLUSPLUS
extern "C" {
	#endif

	/// @cond
	#define rx_aagc_gain								0x0040004C
    #define rx_Rolloff_addr								0x00400030
	#define scatter_data_addr							0x00001AD8
	#define rc_mpeg_bus_tri_enb							0x006C0028
	#define rc_mpeg_bus_pe								0x006C0038
	#define rc_rfagc_tri_enb							0x006C002C
	#define rc_spare_rw_reg_1							0x006C0044
	#define rp_mpeg_config_addr							0x2652
	#define rs_imb_status_addr							0x00400058
	#define rp_phase_imb_addr							0x2650
	#define rp_amp_imb_addr								0x264E
	#define rs_cust_chip_id_addr						0x006C0034
	#define rc_eq_out_iq_swap_addr						0x00402028

	//raptor merge for blind scan fixes to be able to control the tuner step size
	#define rp_tuner_factor_addr                        0x0000264C
	#define rp_hsym_acq_th_addr							0x0000264A
	#define rp_hsym_cd_th_addr							0x00002648	

    #define rc_mpeg_bus_cntrl_addr				        0x400800
    #define rc_cntns_pkt_para_rate_frac_n_addr	        0x400898
    #define rc_cntns_pkt_para_rate_frac_d_addr	        0x40089C
    #define rc_pkt_seri_rate_frac_n_addr		        0x400818
    #define rc_pkt_seri_rate_frac_d_addr		        0x40081C


	#define  rc_pkt_err_count_addr						0x400810
	#define  rc_pkt_count_addr							0x400814
	#define  rs_total_words_addr						0x400430
	#define  rs_total_uncorrected_words_addr			0x400434


	/// @endcond

	/// Initializes the demodulator.
	/// 
    /// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object for which the demodulator is being initialized.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the demodulator has been initialized.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// @remarks This function must be called first before all other functions declared in this interface.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_Initialize(const struct AVL_DVBSx_Chip * pAVLChip );

	/// Configures the Availink device with the polarity of the RF AGC. This value should be set according to the tuner's requirement as specified by the tuner datasheet.
	/// 
	/// @param enumAGCPola The polarity of the RF AGC. Refer to ::AVL_DVBSx_RfagcPola.
    /// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object which is being configured with the tuner RF AGC polarity.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the tuner RF AGC polarity has been configured.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_SetRFAGCPola( enum AVL_DVBSx_RfagcPola enumAGCPola, const struct AVL_DVBSx_Chip * pAVLChip );

	/// Configures the MPEG output mode.
	/// 
	/// @param pMpegMode A pointer to an object which contains the MPEG output mode configuration. 
    /// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object for which the MPEG output mode is being configured.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the MPEG output mode has been configured.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_SetMpegMode( const struct AVL_DVBSx_MpegInfo * pMpegMode, const struct AVL_DVBSx_Chip * pAVLChip );

	/// Gets the RF signal level.
	/// 
	/// @param puiRFSignalLevel Pointer to a variable in which to store signal level.  The signal level value ranges from 0 to 65535, with zero corresponding to the weakest signal level value and 65535 corresponding to the strongest signal level value.
	/// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object for which the signal level is being retrieved.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the signal level has been retrieved.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// @remarks This function actually reads the RF AGC value. The corresponding signal power in dBm varies depending on the tuner. The user can derive the relationship from the Tuner datasheet or through measurement.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_GetSignalLevel(AVL_puint16 puiRFSignalLevel , const struct AVL_DVBSx_Chip * pAVLChip );

	/// Sets the carrier frequency sweep range for subsequent LockChannel calls. Use of this function is optional.  The default value is 500 (+/-5MHz). This function is typically used to narrow the sweep range in cases where there are multiple narrow bandwidth signals within the default range and there is the possibility that the wrong signal will be locked.  If multiple signals are detected within the specified sweep range, the Availink device will lock to the strongest one.
	/// 
	/// @param uiFreqSweepRange_10kHz Half of the frequency sweep range in units of 10kHz. The maximum value is 500 (sweep from -5MHz to +5MHz).
    /// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object for which the frequency sweep range is being set.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the frequency sweep range is successfully configured.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// @remarks The input parameter uiFreqSweepRange_10kHz specifies half of the actual sweep range. The whole range will be [-uiFreqSweepRange_10kHz, +uiFreqSweepRange_10kHz]. Note that this sweep range must be large enough to accommodate the RF path frequency uncertainty.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_SetFreqSweepRange(AVL_uint16 uiFreqSweepRange_10kHz, const struct AVL_DVBSx_Chip * pAVLChip );

	/// Configures the m_Flags member of the AVL_DVBSx_Channel object with the desired lock mode.  On boot-up the default channel lock mode is AVL_DVBSx_LOCK_MODE_FIXED.
	/// 
	/// @param psChannel  Pointer to the channel object for which to configure the lock mode.
	/// @param enumChannelLockMode The desired lock mode.  Refer to ::AVL_DVBSx_LockMode. 
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK after the m_Flags member of the psChannel input parameter is configured with the desired lock mode.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_SetChannelLockMode( struct AVL_DVBSx_Channel * psChannel, enum AVL_DVBSx_LockMode enumChannelLockMode );

	/// Locks to a channel using the parameters specified in \a psChannel.
	/// 
	/// @param psChannel  Holds the channel related parameters needed by the Availink device to lock to the input signal.
	/// @param pAVLChip A pointer to the AVL_DVBSx_Chip object for which the lock operation is being performed. 
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the lock parameters and command are successfully sent to the device.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// Return ::AVL_DVBSx_EC_Running if the lock command could not be sent to the device because the device is still processing a previous command.
	/// @remarks Calling this function commands the Availink device to lock to a particular channel. Use the function 
	/// GetLockStatus to determine if the device has successfully locked to the channel. The channel lock operation is 
	/// performed in either fixed mode or adaptive mode depending on how the psChannel input parameter has been configured.  
	/// Please see the function SetChannelLockMode for more details regarding how to configure the channel object for either mode.  This function can perform automatic IQ swap if \a psChannel->m_Flags has the CI_FLAG_IQ_AUTO_BIT bit set.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_LockChannel( struct AVL_DVBSx_Channel * psChannel, struct AVL_DVBSx_Chip * pAVLChip );

	/// Checks if the Availink device has locked to the channel.
	/// 
	/// @param puiLockStatus Pointer to a variable in which to store the channel lock status (0 - Not locked 1 - Locked).
	/// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object for which the lock status is being queried.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the channel lock status has been retrieved.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// @remarks This function should be called after the function ::AVL_DVBSx_IRx_LockChannel is called.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_GetLockStatus( AVL_puint16 puiLockStatus, const struct AVL_DVBSx_Chip * pAVLChip );

	/// Resets the BER and PER accumulation statistics.
	/// 
	/// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object for which the BER and PER statistics are being reset.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if command to reset the BER and PER statistics has been sent to the Availink device.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// Return ::AVL_DVBSx_EC_Running if the command could not be sent to the Availink device because the device is still processing a previous command.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_ResetErrorStat( struct AVL_DVBSx_Chip * pAVLChip );

	/// Retrieves the scatter data from the Availink device. 
	/// 
	/// @param ucpData  Pointer to an array in which to store the scatter data. The buffer size MUST be >= 2*(*puiSize) because there are two bytes per set of IQ data. The first (*puiSize) bytes in the buffer are I values, and the following (*puiSize) bytes are Q values.
	/// @param puiSize  The number of IQ pairs to be read.  The function updates this value with the number of IQ pairs actually retrieved.
	/// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object for which the scatter data is being retrieved.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the scatter data has been retrieved.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// Return ::AVL_DVBSx_EC_Running if the scatter data is not ready for retrieval.
	/// @remarks Normally, the size of the scatter data will be 132 sets of IQ. i.e. 264 bytes.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_GetScatterData( AVL_puchar ucpData, AVL_puint16 puiSize, const struct AVL_DVBSx_Chip * pAVLChip );

	/// Reads the current SNR estimate.
	/// 
	/// @param puiSNR_db Pointer to a variable in which to store the estimated SNR value.  The SNR value is scaled by 100. For example, a reported SNR value of 2578 means the SNR value is 25.78 db.
	/// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object for which estimated SNR is being read.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the SNR has been retrieved.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// @remarks It takes a few seconds for the device to calculate a stable SNR value after FEC lock. The function returns an SNR value of 0 before a stable SNR value is calculated.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_GetSNR( AVL_puint32 puiSNR_db, const struct AVL_DVBSx_Chip * pAVLChip );

	/// Reads the current PER calculation result.
	/// 
	/// @param puiPER Pointer to a variable in which to store the PER value. The value is scaled by 1e+9. For example, if the reported value is 123456, the PER value is 0.000123456.
	/// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object for which the PER is being retrieved.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the PER has been retrieved.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// @remarks It takes a few seconds for the device to calculates a stable PER value after FEC lock. The function reports a PER of 0 until a stable PER is calculated.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_GetPER( AVL_puint32 puiPER, const struct AVL_DVBSx_Chip * pAVLChip );

	/// Reads the current BER calculation result.
	/// 
	/// @param puiBER Pointer to a variable in which to store the current BER value. The value is scaled by 1e+9. For example, a BER reading of 123456 means the that the BER is 0.000123456.
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip object for which the BER is being read.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the BER has been retrieved.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// @remarks This function is provided for laboratory BER testing.  The function requires the use of a transmitter which can generate either an LFSR15 or LFSR23 data stream.  
	AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_GetBER( AVL_puint32 puiBER, const struct AVL_DVBSx_Chip * pAVLChip );

	/// Reads the current DVBS BER estimate.
	/// 
	/// @param puiBER A pointer to a variable in which to store the estimated DVBS BER value. The value is scaled by 1e+9. For example, a BER reading of 123456 means the estimated BER is 0.000123456.
	/// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object for which estimated DVBS BER is being read.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the BER has been retrieved.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// Return ::AVL_DVBSx_EC_GeneralFail if the Availink device is not locked to a DVBS signal.
	/// @remarks This function reports and estimated BER when a DVB-S signal is received.  The function does not require the input of a known test pattern to the receiver.  It can estimate BER for on-air DVB-S signals. 
	AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_GetDVBSBER( AVL_puint32 puiBER, const struct AVL_DVBSx_Chip * pAVLChip );

	/// Resets the DVB-S BER estimate.
	/// 
	/// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object for which the DVB-S BER estimate is being reset.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the DVB-S BER estimate has been reset.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_ResetDVBSBER( const struct AVL_DVBSx_Chip * pAVLChip );

	/// Gets the RF carrier frequency offset. Typically this is used to adjust the tuner frequency.
	/// 
	/// @param piRFOffset_100kHz Pointer to a variable in which to store the RF carrier frequency offset in units of 100 kHz. 
	/// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object for which the RF carrier frequency offset is being read.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the RF carrier frequency offset has been read.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// @remarks This function may be called after the Availink device has locked to the input signal. 
	AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_GetRFOffset( AVL_pint16 piRFOffset_100kHz, const struct AVL_DVBSx_Chip * pAVLChip );

	/// Gets the current locked signal information.
	/// 
	/// @param pSignalInfo Pointer to an object in which to store the detected signal information. 
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip object for which the signal information is being retrieved.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the signal information has been retrieved.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// @remarks This function may be called after the Availink device has locked to the input signal.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_GetSignalInfo( struct AVL_DVBSx_SignalInfo * pSignalInfo, const struct AVL_DVBSx_Chip * pAVLChip );

	/// Allows the user to enable or disable dish pointing mode.
	/// 
	/// @param ucMode Indicates whether dish pointing mode is being enabled or disabled (0 - Disable dish pointing mode, 1 - Enable dish pointing mode).  
	/// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object for which dish pointing mode is being configured.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if dish pointing mode has been configured.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// Return ::AVL_DVBSx_EC_GeneralFail if the Availink device is not in demodulator mode.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_SetDishPointingMode( AVL_uchar ucMode, const struct AVL_DVBSx_Chip *pAVLChip );

	/// Configures the Availink device to drive the MPEG output interface.
	/// 
	/// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object which is to drive the MPEG output interface.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the device has been successfully configured to drive the MPEG output interface.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_DriveMpegOutput( const struct AVL_DVBSx_Chip * pAVLChip );

	/// Configures the Availink device to release the MPEG output interface, placing it in a high impedance state.  This allows the MPEG bus to be driven by other devices which may be sharing it with the Availink device.
	/// 
	/// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object which is to release the MPEG output interface.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the device has been successfully configured to release the MPEG output interface.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_ReleaseMpegOutput( const struct AVL_DVBSx_Chip * pAVLChip );

	/// Enables MPEG persistent clock mode.  In this mode, once the Availink device is commanded to lock to a channel, it persistently outputs an MPEG clock signal,
	/// regardless of whether the demod is locked or not.  When the demod is not locked, the MPEG valid and data signals are held low.  When the demod is locked, the received data
	/// is output.
	/// 
	/// @param uiMpegDataClkFreq_10kHz The desired MPEG output byte clock frequency during periods in which the demod is not locked.  Units of 10 kHz.
	/// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object for which MPEG persistent clock mode is being enabled.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if MPEG persistent clock mode is successfully enabled.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_EnableMpegPersistentClockMode( AVL_uint16 uiMpegDataClkFreq_10kHz, const struct AVL_DVBSx_Chip *pAVLChip );

	/// Disables MPEG persistent clock mode.  If MPEG persistent clock mode is disabled, then the Availink device outputs an MPEG clock signal only when it is locked to a channel.
	/// 
	/// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object for which MPEG persistent clock mode is being disabled.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if MPEG persistent clock mode is successfully disabled.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_DisableMpegPersistentClockMode( const struct AVL_DVBSx_Chip *pAVLChip );

	/// Enable MPEG manual clock mode.  In this mode, the Availink device always outputs an MPEG clock signal.
	/// 
	/// @param uiMpegDataClkFreq_10kHz The desired MPEG continuous output byte clock frequency.
	/// @param enumMpegMode The mode in which the MPEG output interface is being operated (parallel or serial).
	/// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object for which MPEG persistent clock mode is being disabled.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if MPEG persistent clock mode is successfully disabled.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
    AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_EnableMpegManualClockFrequency(AVL_uint16 uiMpegDataClkFreq_10kHz, enum AVL_DVBSx_MpegMode enumMpegMode, const struct AVL_DVBSx_Chip * pAVLChip);

	/// Disables MPEG manual clock mode.  If MPEG manual clock is disabled, MPEG output clock in automatic mode. Then the Availink device outputs an MPEG clock signal only when it is locked to a channel.
	/// 
	/// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object for which MPEG persistent clock mode is being disabled.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if MPEG persistent clock mode is successfully disabled.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
    AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_DisableMpegManualClockFrequency(const struct AVL_DVBSx_Chip * pAVLChip);

	/// Enables or disables the internal MPEG interface pull-down resistors.  
	/// 
	/// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object for which the internal MPEG interface resistors are being configured.
	/// @param enumPulldownState Indicates whether the internal pull-down resistors should be enabled or disabled.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the internal MPEG pull-down resistors have been configured.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_SetMpegPulldown( const struct AVL_DVBSx_Chip *pAVLChip, enum AVL_DVBSx_MpegPulldown enumPulldownState );

	/// Configures the Availink device to output the RF AGC signal.
	/// 
	/// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object which is to output an RF AGC signal.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the device has been successfully configured to output an RF AGC signal.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_DriveRFAGC( const struct AVL_DVBSx_Chip * pAVLChip );

	/// Configures the Availink device to release the RF AGC signal output.
	/// 
	/// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object which is to release the RF AGC signal output.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the device has been successfully configured to release the RF AGC signal.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_ReleaseRFAGC( const struct AVL_DVBSx_Chip * pAVLChip );

	/// Reads the IQ imbalance of the received signal.
	/// 
	/// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object for which the IQ imbalance of the received signal is being read.
	/// @param piAmplitude A pointer to a variable in which to store the amplitude imbalance in units of dB.  The value is scaled by 100.  For example, a value of 300 means an amplitude imbalance of 3 dB.
	/// @param piPhase A pointer to a variable in which to store the phase imbalance in units of degrees.  The value is scaled by 100.  For example, a value of 200 means a phase imbalance of 2 degrees.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the IQ imbalance has been retrieved from the Availink device.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_GetIQ_Imbalance( const struct AVL_DVBSx_Chip * pAVLChip, AVL_pint16 piAmplitude, AVL_pint16 piPhase );

	/// Reads the Availink device ID.  Availink can customize the device ID to meet the needs of individual customers.  
	/// 
	/// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object for which the device ID is being read.
	/// @param puiDeviceID A pointer to a variable in which to store the device ID.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the device ID has been read.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_GetDeviceID( const struct AVL_DVBSx_Chip * pAVLChip, AVL_puint32 puiDeviceID);

	/// Configures the output bit order of the MPEG data.  The user can set the bit order to either normal or invert.
	/// The meaning differs depending on whether the MPEG interface is configured to output data in serial mode or
	/// parallel mode as follows:
	/// Serial mode (Normal - Output the most significant bit of each byte first, Invert - Output the least significant 
	/// bit of each byte first.)
	/// Parallel mode (Normal - The MSB of each byte is output on pin 7 and the LSB on pin 0, Invert - The MSB of each 
	/// byte is output on pin 0 and the LSB on pin 7.)
	/// 
	/// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object for which the MPEG bit order is being configured.
	/// @param enumMpegMode The mode in which the MPEG output interface is being operated (parallel or serial).
	/// @param enumMpegBitOrder The desired MPEG output bit order.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the MPEG output bit order has been configured.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_SetMpegBitOrder( const struct AVL_DVBSx_Chip * pAVLChip, enum AVL_DVBSx_MpegMode enumMpegMode, enum AVL_DVBSx_MpegBitOrder enumMpegBitOrder );

	/// Selects the pin on which MPEG data is output in serial mode.  The serial data can be output on pin MPEG_DATA_7
	/// or pin MPEG_DATA_0.
	/// 
	/// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object for which the output data pin is being selected.
	/// @param enumSerialPin The pin on which to output the MPEG data in serial mode.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the MPEG output pin has been configured for serial mode.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_SetMpegSerialPin( const struct AVL_DVBSx_Chip * pAVLChip, enum AVL_DVBSx_MpegSerialPin enumSerialPin );

	/// Configures the polarity of the MPEG valid signal when the MPEG interface is configured for TSP mode.
	/// If the MPEG Interface is configured for TS mode, calling this function has no effect.
	/// 
	/// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object for which the polarity of the MPEG valid signal is being configured.
	/// @param enumValidPolarity The polarity of the MPEG valid signal in TSP mode (Normal - The valid signal is held low during the 
	/// parity bytes, Invert - The valid signal is held high during the parity bytes).
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the polarity of the MPEG valid signal has been configured.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_SetMpegValidPolarity( const struct AVL_DVBSx_Chip * pAVLChip, enum AVL_DVBSx_MpegValidPolarity enumValidPolarity );

	/// Configures the polarity of the MPEG error signal.  This function allows the user to configure the Availink device with the
	/// the error signal polarity that should be used when the demod is not locked to the channel as well as the error signal polarity
	/// that should be used when the demod is locked to the channel.
	/// 
	/// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object for which the polarity of the MPEG error signal is being configured.
	/// @param enumErrorLockPolarity The MPEG error signal polarity to be used during periods in which the Availink device is locked to a 
	/// signal (Normal - The MPEG error signal is high when there is an MPEG error, Invert - The MPEG signal is low when there is an MPEG error).
	/// @param enumErrorUnlockPolarity The MPEG error signal polarity to be used during periods in which the Availink device is not locked to a 
	/// signal (Normal - MPEG error signal is low when the demod is not locked, Invert - MPEG error signal is high when the demod is not locked.)
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the polarity of the MPEG error signal has been configured.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_SetMpegErrorPolarity(const struct AVL_DVBSx_Chip * pAVLChip, enum AVL_DVBSx_MpegErrorPolarity enumErrorLockPolarity, enum AVL_DVBSx_MpegErrorPolarity enumErrorUnlockPolarity);

	/// Reads the IQ swap status of the received signal.  This function is useful if the user enabled automatic IQ swap
	/// when locking to the channel.  After channel lock has been achieved, the user may call the GetIQ_Swap function
	/// to determine whether the I and Q signals had to be swapped for the received signal.
	/// 
	/// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object for which the IQ swap status status is being retrieved.
	/// @param puiIQ_Swap Pointer to a variable in which to store the IQ swap status (0 - Not swapped, 1 - Swapped).
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the IQ swap status has been retrieved.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_GetIQ_Swap( const struct AVL_DVBSx_Chip * pAVLChip, AVL_puint16 uiIQ_Swap );

	/// Reads the current word error rate of Reed-Solomon decoder.
	/// 
	/// @param puiRSErr Pointer to a variable in which to store the word error rate value.The value is scaled by 1e+9. For example, if the reported value is 123456, the word error rate value is 0.000123456.
	/// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object for which the word error rate is being retrieved.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the word error rate has been retrieved.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_GetRSError( AVL_puint32  puiRSErr, const struct AVL_DVBSx_Chip * pAVLChip );

	/// Reads back the value of total error packets at current time.
	/// 
	/// @param puiErrPacket Pointer to a variable in which to store the error packets value. 
	/// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object for which the error packets is being retrieved.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the error packets value has been retrieved.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IRx_GetErrPacket( AVL_puint32 puiErrPacket, const struct AVL_DVBSx_Chip * pAVLChip );

    #ifdef AVL_CPLUSPLUS
}
	#endif

#endif
