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
/// @brief Declares the functions for blind scan process operations.
/// @details The IBlindScanProcess interface provides a simple approach for performing a blind scan process operation, it also like a middle ware level code. 
///
//$Revision: 1839 $ 
//$Date: 2010-12-15 23:17:24 -0400 (TUE, 15 Dec 2010) $
// 



#ifndef BlindScanProcess_h_h
    #define BlindScanProcess_h_h

#include "avl_dvbsx.h"
#include "IBlindScan.h"

    #ifdef AVL_CPLUSPLUS
extern "C" {
	#endif


// Channel buffer size 
#define MAXIMUM_CHANNELS_COUNT 258

//Defines required for duplicate channel filtration
#define FILTER_SYMBOL_RATE_20MHZ 20000000
#define FILTER_SYMBOL_RATE_10MHZ 10000000

#define SAME_HIGH_SR_FREQUENCY_DEVIANCE_KHZ 5000	//5MHz
#define SAME_LOW_SR_FREQUENCY_DEVIANCE_KHZ  3000	//3MHz

#define SAME_HIGH_SR_DEVIANCE_HZ    100000			//100Ksps
#define SAME_MIDDLE_SR_DEVIANCE_HZ  50000			//50Ksps
#define SAME_LOW_SR_DEVIANCE_HZ     25000			//25Ksps

enum AVL_DVBSx_BlindscanProcessStatus
{
	AVL_DVBSx_BS_Status_Stop = 0,			///< = 0 Indicates that the blind scan process has stopped.
	AVL_DVBSx_BS_Status_Scanning = 1,		///< = 1 Indicates that the blind scan process is still scanning.
	AVL_DVBSx_BS_Status_Locked = 2			///< = 2 Indicates that the blind scan process has locked to one channel.
};

struct AVL_DVBSx_BlindScanProcessData
{
	AVL_uint16 m_uiChannelCount;			///< Indicates the number of scanned out channel.
	AVL_uint16 m_uiLockedChannelIndex;		///< Indicates index of the currently locked channel in the channel_buf array. It starts from 1 while channel_buf array's index starts from 0.
	AVL_uint16 m_uiLastFoundCHcount;		///< Indicates the channel count of last scan step found.
	AVL_uint16 m_uiNextStartFreq_100KHz;	///< Indicates the next step scan frequency. When a scan is over, store the next start frequency in this parameter for next step's scan.
	AVL_uint16 m_uiScanStopFlag;			///< Indicates the completion status of blind scan process over a specified range.
	AVL_uint16 m_uiLockFlag[MAXIMUM_CHANNELS_COUNT];				///< Indicates the lock status of each channel found in the scan process. 1 indicates has locked, 0 indicates unlock.
	struct AVL_DVBSx_Channel channel_buf[MAXIMUM_CHANNELS_COUNT];   ///< Buffer used to store the scanned channels' results.
	struct AVL_DVBSx_BlindScanPara bsPara;			///< Stores parameters required by the blind scan such as start and stop frequencies and symbol rates. Should be set by the user.
	enum AVL_DVBSx_BlindscanProcessStatus m_eBSStatusFlag;	///< Indicates the status of process in terms of scan or lock.  Should be set by the user.
	AVL_uint16 m_uiTuner_MaximumLPF_100KHz ;		///< Stores the maximum LPF value of the  tuner in units of 100kHz. Should be set by the user. Recommended value for Sharp tuner is 340.
	AVL_uint16 m_uiTuner_FlatLPF_100KHz ;			///< Stores the LPF value of the tuner over which its response is flat in units of 100kHz. Should be set by the user. Recommended value for Sharp tuner is 320.
};




/// Locks to a channel using at a specified frequency and symbol rate. 
/// 
/// @param pAVLChip A pointer to the AVL_DVBSx_Chip object on which the lock operation is being performed. 
/// @param pTuner  A pointer to the tuner object to perform tuner lock operations.
/// @param Freq_MHz The frequency parameter needed by the Availink device to lock to the input signal.
/// @param SymbolRate_Hz The symbol rate parameter needed by the Availink device to lock to the input signal.
/// @return ::AVL_DVBSx_ErrorCode, 
/// Return ::AVL_DVBSx_EC_OK if the device has successfully locked to the channel.	
/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
/// @remarks This function is an internal SDK function. Availink recommends that users refrain from calling this function directly. 
AVL_DVBSx_ErrorCode AVL_DVBSx_IBlindScanProcess_LockChannel(struct AVL_DVBSx_Chip  *pAVL_DVBSxChip, struct AVL_Tuner *pAVL_Tuner, AVL_uint32 Freq_MHz, AVL_uint32 SymbolRate_Hz);

/// Determines if the two channel structures passed to it as arguments are duplicates.
/// 
/// @param pTemp Pointer to the first channel structure to be compared.
/// @param pValid Pointer to the second channel structure to be compared.
/// 
/// @return ::AVL_uint16, 
/// Return ::1 if the two channels are same. 	
/// Return ::0 if the two channels are different.
/// @remarks This function is an internal SDK function. Availink recommends that users refrain from calling this function directly. 
AVL_uint16 AVL_DVBSx_IBlindScanProcess_JudgeChannels(const struct AVL_DVBSx_Channel *pTemp, const struct AVL_DVBSx_Channel *pValid);

/// Sorts a list of channels in the descending order of channel frequency. 
/// 
/// @param Ch_list_valid Pointer to the channel list to be sorted.
/// @param original_channels_count The count of the channels already sorted in the list. 
/// @param new_add_channels_count The count of newer channels to be sorted, the starting index for the sort is defined by the new_add_channels_count parameter. 
/// @return ::void, 
/// @remarks This function is an internal SDK function. Availink recommends that users refrain from calling this function directly. 
void AVL_DVBSx_IBlindScanProcess_ChannelsQsort(struct AVL_DVBSx_Channel *Ch_list_valid, AVL_uint16 original_channels_count,AVL_uint16 new_add_channels_count);

/// Stores the new channels found in the scan and filters out the duplicate ones.
/// 
/// @param Ch_list_valid Pointer to the channel list to store the new channels.
/// @param uiValid_ChCount The count of the channels already stored in the list pointed by  Ch_list_valid parameter.
/// @param maximum_channels_count The maximum count of channels that can be stored in the list pointed by Ch_list_valid parameter.
/// @param Ch_list_Temp Pointer to a temporary channel list to store the new channels.
/// @param uiChCount_Found The count of the channels stored in the list pointed by Ch_list_Temp parameter.
/// 
/// @return ::void, 
/// @remarks This function is an internal SDK function. Availink recommends that users refrain from calling this function directly . 
void AVL_DVBSx_IBlindScanProcess_ChannelsFilter(struct AVL_DVBSx_Channel *Ch_list_valid, AVL_uint16 *uiValid_ChCount, AVL_uint16 maximum_channels_count, struct AVL_DVBSx_Channel *Ch_list_Temp, AVL_uint16 uiChCount_Found);

/// Calculates the progress of blind scan process based on current scan step's start frequency.
/// 
/// @param bsProgress Pointer to the progress variable that gets updated based on next scan step's start frequency.
/// @param CurrentFreq Current scan step's start frequency used to calculate the new progress.
/// @param bsData  A pointer to a structure that stores all the required information to manage blind scan process. The fucntion gets the blind scan frequency range from this structure.
/// @return ::AVL_DVBSx_ErrorCode, 
/// Return ::AVL_DVBSx_EC_OK if update progress value is successful. 	
/// Return ::AVL_DVBSx_EC_GeneralFail if the update results in some failure.
/// @remarks This function is an internal SDK function. Availink recommends that users refrain from calling this function directly. 
AVL_DVBSx_ErrorCode AVL_DVBSx_IBlindScanProcess_UpdateProgress(AVL_uint16 *bsProgress, AVL_uint16 CurrentFreq, struct AVL_DVBSx_BlindScanProcessData *bsData);

/// Reads the channels found during a particular scan from the firmware and stores them to bsData structure. 
/// 
/// @param pAVLChip A pointer to the AVL_DVBSx_Chip object on which read channel operation is being performed.
/// @param bsData A pointer to a structure that stores all the required information to manage blind scan process.
/// @param Channels_Count The count of the new channels found in the scan.
///
/// @return ::AVL_DVBSx_ErrorCode, 
/// Return ::AVL_DVBSx_EC_OK if the read channel operation is successful. 	
/// Return ::Not AVL_DVBSx_EC_OK if read channel operation from the device results in a failure.
/// @remarks This function is an internal SDK function. Availink recommends that users refrain from calling this function directly. 
AVL_DVBSx_ErrorCode AVL_DVBSx_IBlindScanProcess_ReadChannels(struct AVL_DVBSx_Chip * pAVLChip, struct AVL_DVBSx_BlindScanProcessData *bsData, AVL_uint16 Channels_Count);

/// Performs a blind scan operation over a scan step size.
/// 
/// @param pAVLChip A pointer to the AVL_DVBSx_Chip object on which blind scan is being performed.
/// @param pTuner   A pointer to the tuner object to perform tuner operations such as lock before doing each blind scan and lock channel operation.
/// @param bsData A pointer to a structure that stores all the required information to manage blind scan process. 
/// 
/// @return ::AVL_DVBSx_ErrorCode, 
/// Return ::AVL_DVBSx_EC_OK if the operation is successful. 	
/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
/// Return ::AVL_DVBSx_EC_GeneralFail if the operation results in some chip level failure.
/// @remarks This function is an internal SDK function. Availink recommends that users refrain from calling this function directly. 
AVL_DVBSx_ErrorCode AVL_DVBSx_IBlindScanProcess_ScanStart(struct AVL_DVBSx_Chip *pAVLChip, struct AVL_Tuner *pTuner, struct AVL_DVBSx_BlindScanProcessData *bsData);

/// Starts blind scan process. Initializes the bsData structure data for blind scan.
/// 
/// @param pAVLChip A pointer to the AVL_DVBSx_Chip object on which blind scan process is being performed.
/// @param pTuner  A pointer to the tuner object to perform tuner operations such as lock before starting the blind scan.
/// @param bsData A pointer to a structure that stores all the required information to manage blind scan process.
/// @param pBSPara  A pointer to a structure that stores blind scan configuration parameters.
/// @param uiTuner_MaximumLPF_100KHz The tuner's maximum LPF bandwidth setting in units of 100 kHz.
/// @param uiTuner_FlatLPF_100KHz The tuner's flat LPF bandwidth setting in units of 100 kHz.
///
/// @return ::AVL_DVBSx_ErrorCode, 
/// Return ::AVL_DVBSx_EC_OK if the scan process is successfully started.
/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
/// Return ::AVL_DVBSx_EC_Running if the blind scan command could not be sent because the Availink device is still processing a previous command.
/// Return ::AVL_DVBSx_EC_GeneralFail if the device is not in the blind scan functional mode or if the parameter pBSPara->m_uiStartFreq_100kHz is greater than the parameter pBSPara->m_uiStopFreq_100kHz
AVL_DVBSx_ErrorCode AVL_DVBSx_IBlindScanProcess_Start(struct AVL_DVBSx_Chip * pAVLChip, struct AVL_Tuner * pTuner,struct AVL_DVBSx_BlindScanProcessData *bsData, struct AVL_DVBSx_BlindScanPara bsPara, AVL_uint16 uiTuner_MaximumLPF_100KHz,AVL_uint16 uiTuner_FlatLPF_100KHz);

/// Performs incremental blind scan over the entire specified range, reports its progress and locks to all the channels found during each incremental scan. 
/// 
/// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object on which blind scan is being performed.
/// @param pTuner   A pointer to the tuner object to perform tuner operations such as lock before doing each blind scan and lock channel operation.
/// @param bsData A pointer to a structure that stores all the required information to manage blind scan process. It is same as bsData parameter of AVL_DVBSx_IBlindScanProcess_Start function.
/// @param bsProgress A pointer to a value used to indicate the blind scan progress.
///
/// @return ::AVL_DVBSx_BlindscanProcessStatus, 
/// Return ::AVL_DVBSx_BS_Status_Stop if the process was completed or reported some problem.
/// Return ::AVL_DVBSx_BS_Status_Scanning the process is scanning.
/// Return ::AVL_DVBSx_BS_Status_Locked the process has locked to a channel.
enum AVL_DVBSx_BlindscanProcessStatus AVL_DVBSx_IBlindScanProcess_ScanLock(struct AVL_DVBSx_Chip * pAVLChip, struct AVL_Tuner * pTuner, struct AVL_DVBSx_BlindScanProcessData *bsData, AVL_uint16 *bsProgress);

/// Stops blind scan process. Sets a flag to cause AVL_DVBSx_IBlindScanProcess_ScanLock function return AVL_DVBSx_BS_Status_Stop.
/// 
/// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object on which blind scan is being stopped.
/// @param bsData A pointer to a structure that stores all the required information to manage blind scan process. It is same as bsData parameter of AVL_DVBSx_IBlindScanProcess_Start and AVL_DVBSx_IBlindScanProcess_ScanLock functions.
///
/// @return ::AVL_DVBSx_ErrorCode, 
/// Return ::AVL_DVBSx_EC_OK.
AVL_DVBSx_ErrorCode AVL_DVBSx_IBlindScanProcess_Stop(struct AVL_DVBSx_Chip * pAVLChip, struct AVL_DVBSx_BlindScanProcessData *bsData);


	#ifdef AVL_CPLUSPLUS
}
	#endif

#endif
