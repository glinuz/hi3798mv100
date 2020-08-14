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
/// @brief Defines functions for initializing and controlling the I2C repeater.
/// @details The I2C repeater provides a dedicated I2C bus for tuner control. It is 
/// recommended that the functions in this interface be used to implement a tuner 
/// driver for the tuner being used. For customer convenience, Availink provides tested 
/// tuner drivers for a variety tuner devices.
/// 
//$Revision: 514 $ 
//$Date: 2008-08-12 17:51:32 -0400 (Tue, 12 Aug 2008) $
// 
#ifndef II2CRepeater_h_h
	#define II2CRepeater_h_h

	#include "avl_dvbsx.h"
	#include "avl_dvbsx_globals.h"

	#ifdef AVL_CPLUSPLUS
extern "C" {
	#endif

	///@cond

	#define I2CM_CMD_LENGTH   0x14
	#define I2CM_RSP_LENGTH   0x14

	#define OP_I2CM_NOOP      0x00
	#define OP_I2CM_INIT	  0x01
	#define OP_I2CM_WRITE     0x02
	#define OP_I2CM_READ      0x03

	///@endcond

	///@cond

	/// Sends an I2C repeater operational command to the Availink device. 
	/// 
	/// @param pBuff Pointer to the array which contains the operational command and its parameters.
	/// @param ucSize The number of command related bytes in the array to which pBuff points.
	/// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object to which an I2C repeater operational command is being sent.
	/// 
	/// Return ::AVL_DVBSx_EC_OK if the I2C repeater operational command has been sent to the Availink device.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// Return ::AVL_DVBSx_EC_Running if the command could not be sent to the Availink device because the device is still processing a previous command.
	AVL_DVBSx_ErrorCode AVL_DVBSx_II2CRepeater_SendOP(AVL_puchar pBuff, AVL_uchar ucSize, struct AVL_DVBSx_Chip * pAVLChip );

	///@endcond

	/// Initializes the I2C repeater.
	/// 
	/// @param I2CBusClock_kHz The clock speed of the I2C bus between the tuner and the Availink device.
	/// @param pAVLChip A pointer to a ::AVL_DVBSx_Chip object for which the I2C repeater is being initialized.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the initialize command has been sent to the Availink device.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// Return ::AVL_DVBSx_EC_Running if the initialize command could not be sent to the Availink device because the device is still processing a previous command.
	/// @remarks This function must be called before any other function in this interface. This is a nonblocking function.  Call ::AVL_DVBSx_II2CRepeater_GetOPStatus to determine if the I2C repeater is initialized.
	AVL_DVBSx_ErrorCode AVL_DVBSx_II2CRepeater_Initialize( AVL_uint16 I2CBusClock_kHz, struct AVL_DVBSx_Chip * pAVLChip );

	/// Reads data back from the tuner via the I2C repeater.  This function is used with tuners which insert a stop
	/// bit between messages.
	/// 
	/// @param ucSlaveAddr The slave address of the tuner device. Please note that the Availink device only supports a 7 bit slave address.
	/// @param pucBuff Pointer to a buffer in which to store the data read from the tuner.
	/// @param uiSize The number of bytes to read from the tuner.  The maximum value is 20.
	/// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object for which data is being read from the tuner.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the data has been read from the tuner.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// Return ::AVL_DVBSx_EC_Running if the read command could not be sent to the Availink device because the device is still processing  a previous command.
	/// Return ::AVL_DVBSx_EC_GeneralFail if \a uiSize is larger than 20.
	/// @remarks This function will trigger a I2C read operation. It is used with tuners which insert a stop bit between messages.  The read position (or device internal address) can be determined by calling ::AVL_DVBSx_II2CRepeater_SendData.
	AVL_DVBSx_ErrorCode AVL_DVBSx_II2CRepeater_ReadData(  AVL_uchar ucSlaveAddr, AVL_puchar pucBuff, AVL_uint16 uiSize, struct AVL_DVBSx_Chip * pAVLChip );

	/// Reads data back from the tuner via the I2C repeater.  This function is used with tuners which do not insert a stop
	/// bit between messages.
	/// 
	/// @param ucSlaveAddr The slave address of the tuner device. Please note that the Availink device only supports a 7 bit slave address.
	/// @param pucBuff Pointer to the buffer in which to store the read data.
	/// @param ucRegAddr The address of the register being read.
	/// @param uiSize The number of bytes to read from the tuner.  The maximum value is 20.
	/// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object for which data is being read from the tuner.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the data has been read from the tuner.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// Return ::AVL_DVBSx_EC_Running if the read command could not be sent to the Availink device because the device is still processing a previous command.
	/// Return ::AVL_DVBSx_EC_GeneralFail if \a uiSize is larger than 20.
	/// @remarks This function will trigger a I2C read operation. It is used with tuners which do not insert a stop bit between messages.
	AVL_DVBSx_ErrorCode AVL_DVBSx_II2CRepeater_ReadData_Multi(  AVL_uchar ucSlaveAddr, AVL_puchar pucBuff, AVL_uchar ucRegAddr, AVL_uint16 uiSize, struct AVL_DVBSx_Chip * pAVLChip );

	/// Writes data to the tuner via the I2C repeater.
	/// 
	/// @param ucSlaveAddr The slave address of the tuner device. Please note that the Availink device only supports a 7 bit slave address.
	/// @param ucBuff Pointer to the buffer which contains the data to be sent to the tuner.
	/// @param uiSize The number of bytes to be sent to the tuner.  The maximum value is 17.
	/// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object for which data is being sent to the tuner.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the send command has been sent to the Availink device.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// Return ::AVL_DVBSx_EC_Running if the send command could not be sent to the Availink device because the device is still processing a previous command.
	/// Return ::AVL_DVBSx_EC_GeneralFail if \a uiSize is larger than 17.
	/// @remarks The internal register address is buried in the buffer to which \a ucBuff points. This function is a nonblocking function.  Call ::AVL_DVBSx_II2CRepeater_GetOPStatus to determine if the write operation is complete.
	AVL_DVBSx_ErrorCode AVL_DVBSx_II2CRepeater_SendData(  AVL_uchar ucSlaveAddr, const AVL_puchar ucBuff, AVL_uint16 uiSize, struct AVL_DVBSx_Chip * pAVLChip );

	/// Checks if the last I2C repeater operation is finished.
	/// 
	/// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object for which I2C repeater operation status is being queried.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the last I2C repeater operation is complete.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// Return ::AVL_DVBSx_EC_Running if the Availink device is still processing the last I2C repeater operation.
	AVL_DVBSx_ErrorCode AVL_DVBSx_II2CRepeater_GetOPStatus(const struct AVL_DVBSx_Chip * pAVLChip );

	#ifdef AVL_CPLUSPLUS
}
	#endif

#endif
