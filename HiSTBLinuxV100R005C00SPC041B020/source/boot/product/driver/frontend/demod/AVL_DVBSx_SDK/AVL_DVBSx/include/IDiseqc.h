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
/// @brief Declares the functions for DiSEqC operations.
/// @details There are some limitations of the DiSEqC operation. First, it is a half duplex bus. You cannot send and receive data simultaneously. Second, the maximum size of each transmission is 8 bytes.
/// The DiSEqC interface can operate in different modes such as modulation mode, tone mode and continuous mode. There are functions corresponding to these modes.  If the user changes the operating mode by calling functions
/// which belong to another mode, the previous operation will be cancelled if it has not finished yet.
///
//$Revision: 355 $ 
//$Date: 2008-04-21 16:12:24 -0400 (Mon, 21 Apr 2008) $
// 
#ifndef IDiseqc_h_h
	#define IDiseqc_h_h

	#include "avl_dvbsx.h"

	#ifdef AVL_CPLUSPLUS
extern "C" {
	#endif

	///@cond

	#define diseqc_tx_cntrl_addr			0x00700000
	#define diseqc_tone_frac_n_addr			0x00700004
	#define diseqc_tone_frac_d_addr			0x00700008
	#define diseqc_tx_st_addr				0x0070000c
	#define diseqc_rx_parity_addr			0x00700010
	#define diseqc_rx_msg_tim_addr			0x00700014
	#define diseqc_rx_st_addr				0x00700018
	#define diseqc_rx_cntrl_addr			0x0070001c
	#define diseqc_srst_addr				0x00700020
	#define diseqc_bit_time_addr			0x00700024
	#define diseqc_samp_frac_n_addr			0x00700028
	#define diseqc_samp_frac_d_addr			0x0070002c
	#define diseqc_bit_decode_range_addr	0x00700030
	#define diseqc_rx_fifo_map_addr			0x00700040
	#define diseqc_tx_fifo_map_addr			0x00700080

	///@endcond

	///
	/// When transmitting data in Tone0 or Tone1 mode, there is a gap between two tones. This enumeration defines the gap length.
	enum AVL_DVBSx_Diseqc_TxGap
	{
		AVL_DVBSx_DTXG_15ms = 0,		///< = 0  The gap is 15 ms.
		AVL_DVBSx_DTXG_20ms = 1,		///< = 1  The gap is 20 ms.
		AVL_DVBSx_DTXG_25ms = 2,		///< = 2  The gap is 25 ms.
		AVL_DVBSx_DTXG_30ms = 3			///< = 3  The gap is 30 ms.
	};

	///
	/// Defines the transmit mode.
	enum AVL_DVBSx_Diseqc_TxMode
	{
		AVL_DVBSx_DTM_Modulation = 0,			///< = 0  Use modulation mode.
		AVL_DVBSx_DTM_Tone0 = 1,				///< = 1  Send out tone 0.
		AVL_DVBSx_DTM_Tone1 = 2,				///< = 2  Send out tone 1.
		AVL_DVBSx_DTM_Continuous = 3			///< = 3  Continuously send out pulses.
	};

	///
	/// Configures the DiSEqC output waveform mode.
	enum AVL_DVBSx_Diseqc_WaveFormMode
	{
		AVL_DVBSx_DWM_Normal = 0,			///< = 0  Normal waveform mode
		AVL_DVBSx_DWM_Envelope = 1			///< = 1  Envelope waveform mode
	};

	///
	/// After data is transmitted to the DiSEqC device, the DiSEqC device may return some data. 
	/// This enumeration controls the amount of time for which the Availink device will open the DiSEqC input FIFO to receive the data. Data that 
	/// is received outside of this time frame is abandoned.
	enum AVL_DVBSx_Diseqc_RxTime
	{
		AVL_DVBSx_DRT_150ms = 0,		///< = 0  Wait 150 ms for receive data and then close the input FIFO.
		AVL_DVBSx_DRT_170ms = 1,		///< = 1  Wait 170 ms for receive data and then close the input FIFO.
		AVL_DVBSx_DRT_190ms = 2,		///< = 2  Wait 190 ms for receive data and then close the input FIFO.
		AVL_DVBSx_DRT_210ms = 3			///< = 3  Wait 210 ms for receive data and then close the input FIFO.
	};

	/// Stores the DiSEqC configuration parameters.
	/// 
	struct AVL_DVBSx_Diseqc_Para
	{
		AVL_uint16 m_ToneFrequency_kHz;						///< The DiSEqC bus speed in units of kHz. Normally, it is 22kHz. 
		enum AVL_DVBSx_Diseqc_TxGap m_TXGap;				///< Transmit gap
		enum AVL_DVBSx_Diseqc_WaveFormMode m_TxWaveForm;	///< Transmit waveform format
		enum AVL_DVBSx_Diseqc_RxTime m_RxTimeout;			///< Receive time frame window
		enum AVL_DVBSx_Diseqc_WaveFormMode m_RxWaveForm;	///< Receive waveform format
	};

	/// Stores the DiSEqC transmitter status.
	/// 
	struct AVL_DVBSx_Diseqc_TxStatus
	{
		AVL_uchar m_TxDone;				///< Indicates whether the transmission is complete (1 - transmission is finished, 0 - transmission is still in progress).
		AVL_uchar m_TxFifoCount;		///< The number of bytes remaining in the transmit FIFO
	};

	/// Stores the DiSEqC receiver status
	/// 
	struct AVL_DVBSx_Diseqc_RxStatus
	{
		AVL_uchar m_RxFifoCount;		///< The number of bytes in the DiSEqC receive FIFO.
		AVL_uchar m_RxFifoParChk;		///< The parity check result of the received data. This is a bit-mapped field in which each bit represents the parity check result for each each byte in the receive FIFO.  The upper bits without corresponding data are undefined. If a bit is 1, the corresponding byte in the FIFO has good parity. For example, if three bytes are in the FIFO, and the parity check value is 0x03 (value of bit 2 is zero), then the first and the second bytes in the receive FIFO are good. The third byte had bad parity. 
		AVL_uchar m_RxDone;				///< 1 if the receiver window is turned off, 0 if it is still in receiving state.
	};

	/// Initializes the DiSEqC component using the configuration parameters in \a pDiseqcPara.
	/// 
	/// @param pDiseqcPara  Pointer to the DiSEqC configuration parameters.
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip object for which the DiSEqC interface is being initialized.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the DiSEqC interface has been initialized.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IDiseqc_Initialize( const struct AVL_DVBSx_Diseqc_Para * pDiseqcPara, struct AVL_DVBSx_Chip * pAVLChip );

	/// Reads data from the DiSEqC input FIFO.
	/// 
	/// @param pucBuff Pointer to a buffer in which the read data should be stored.
	/// @param pucSize The number of bytes to read from the FIFO. The maximum value is 8.  The function updates this parameter to indicate the number of bytes that have actually been read.
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip object for which the DiSEqC input FIFO is being read.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the DiSEqC input FIFO has been read. 
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// Return ::AVL_DVBSx_EC_GeneralFail if the DiSEqC interface is not in modulation mode or if the DiSEqC interface is still receiving the data.
	/// @remarks Availink recommends that the user call ::AVL_DVBSx_IDiseqc_GetRxStatus before calling this function. 
	AVL_DVBSx_ErrorCode AVL_DVBSx_IDiseqc_ReadModulationData( AVL_puchar pucBuff, AVL_puchar pucSize, struct AVL_DVBSx_Chip * pAVLChip );

	/// Sends data to the DiSEqC bus. 
	/// 
	/// @param pucBuff Pointer to an array which contains the data to be sent to the DiSEqC bus.
	/// @param ucSize The number of bytes to be sent. The maximum is 8.
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip object for which data is to be sent to the DiSEqC bus.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the data has been sent
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// Return ::AVL_DVBSx_EC_Running if it is not safe to switch the DiSEqC mode because the last transmission is not complete yet.
	/// Return ::AVL_DVBSx_EC_MemoryRunout if \aucSize is larger than 8.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IDiseqc_SendModulationData( const AVL_puchar pucBuff, AVL_uchar ucSize, struct AVL_DVBSx_Chip * pAVLChip );

	/// Checks the current status of the DiSEqC transmitter.
	/// 
	/// @param pTxStatus  Pointer to an object in which to store the DiSEqC transmitter status.
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip object for which the transmit status is being checked.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the transmit status has been retrieved.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IDiseqc_GetTxStatus( struct AVL_DVBSx_Diseqc_TxStatus * pTxStatus, struct AVL_DVBSx_Chip * pAVLChip );

	/// Checks the current status of the DiSEqC receiver.
	/// 
	/// @param pRxStatus  Pointer to an object in which to store the DiSEqC receiver status information.
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip object for which the receiver status is being checked.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the receiver status has been retrieved.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// Return ::AVL_DVBSx_EC_GeneralFail if the DiSEqC component is not in modulation mode.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IDiseqc_GetRxStatus( struct AVL_DVBSx_Diseqc_RxStatus * pRxStatus, struct AVL_DVBSx_Chip * pAVLChip );

	/// Sets the output level of pin LNB_CNTRL_0.  This pin is typically used to control LNB polarization.
	/// 
	/// @param uiOut Controls the level of pin LNB_CNTRL_0 (0 - Set output level low, 1 - Set output level high).
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip object for which the output level of pin LNB_CNTRL_0 is being set.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the pin level has been configured.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IDiseqc_SetLNBOut( AVL_uchar uiOut, struct AVL_DVBSx_Chip * pAVLChip );    

	/// Gets the output level of pin LNB_CNTRL_0.
	/// 
	/// @param puiOut Pointer to a variable in which to store the pin output level (0 - Output level is low, 1 - Output level is high).
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip object for which the pin output level is being read.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the pin level has been read.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IDiseqc_GetLNBOut( AVL_puchar puiOut, struct AVL_DVBSx_Chip * pAVLChip );

	/// Configures the device to transmit the DiSEqC Tone.
	/// 
	/// @param ucTone  Configures the tone to be transmitted (0 - Tone_0, 1 - Tone_1).
	/// @param ucCount  The number of tones to be transmitted.  The maximum value is 8.
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip object which is being configured to transmit the tones.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the device has been configured to transmit the DiSEqC tones.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// Return ::AVL_DVBSx_EC_MemoryRunout if \aucCount is larger than 8.
	/// Return ::AVL_DVBSx_EC_Running if it is not safe to switch the DiSEqC mode because the last transmission is not complete.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IDiseqc_SendTone( AVL_uchar ucTone, AVL_uchar ucCount, struct AVL_DVBSx_Chip * pAVLChip );

	/// Configures the device to output a continuous 22kHz DiSEqC waveform.
	/// 
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip object which is being configured to output the waveform.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the device has been configured to transmit the waveform.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// Return ::AVL_DVBSx_EC_Running if it is not safe to switch the DiSEqC mode because the last transmission is not complete.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IDiseqc_StartContinuous (struct AVL_DVBSx_Chip * pAVLChip );

	/// Configures the device to stop outputting a continuous  22kHz DiSEqC waveform.
	/// 
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip object which is being configured to stop outputting the waveform.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the device has been configured to stop transmitting the waveform.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IDiseqc_StopContinuous (struct AVL_DVBSx_Chip * pAVLChip );

	/// Sets the output level of pin LNB_CNTRL_1.  This pin is also can used to control LNB polarization.
	/// 
	/// @param uiOut Controls the level of pin LNB_CNTRL_1 (0 - Set output level low, 1 - Set output level high).
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip object for which the output level of pin LNB_CNTRL_1 is being set.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the pin level has been configured.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IDiseqc_SetLNB1Out( AVL_uchar uiOut, struct AVL_DVBSx_Chip * pAVLChip );    

	/// Gets the output level of pin LNB_CNTRL_1.
	/// 
	/// @param puiOut Pointer to a variable in which to store the pin output level (0 - Output level is low, 1 - Output level is high).
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip object for which the pin output level is being read.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the pin level has been read.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IDiseqc_GetLNB1Out( AVL_puchar puiOut, struct AVL_DVBSx_Chip * pAVLChip );

	#ifdef AVL_CPLUSPLUS
}
	#endif

#endif
