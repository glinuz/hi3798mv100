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
/// @brief Defines the BSP functions which the user needs to implement. 
/// @details These BSP functions are called by SDK API functions. This file also defines some hardware related macros which also need to be 
/// customized by the user according to their hardware platform. Most of the functions declared here should 
/// NOT be directly called by the user's applications explicitly.  There are two exceptions.  These are ::AVL_DVBSx_IBSP_Initialize and ::AVL_DVBSx_IBSP_Dispose.
///
//$Revision: 355 $ 
//$Date: 2008-04-21 16:12:24 -0400 (Mon, 21 Apr 2008) $
// 
#ifndef IBSP_h_h
	#define IBSP_h_h

	#include "avl_dvbsx.h"
    #include "drv_tuner_ioctl.h"

	#ifdef AVL_CPLUSPLUS
extern "C" {
	#endif

	#define MAX_II2C_READ_SIZE	64		///< The maximum number of bytes the back end chip can handle in a single I2C read operation. This value must >= 2.
	#define MAX_II2C_Write_SIZE	64		///< The maximum number of bytes the back end chip can handle in a single I2C write operation. This value must >= 8.


	/// Performs initialization for BSP operations.
	/// 
	/// @remarks This function should never called inside the SDK. The user can redeclare this function to any prototype.
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK Returned by the function stub provided with the SDK. 
	AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_Initialize(void);

	/// Destroys all resources allocated in AVL_DVBSx_IBSP_Initialize and other BSP operations.
	/// 
	/// @remarks This function should never called inside the SDK. The user can redeclare this function to any prototype.
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK Returned by the function stub provided with the SDK. 
	AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_Dispose(void);

	/// Implements a delay in units of milliseconds.
	/// 
	/// @param uiMS: The delay period in units of milliseconds.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK Returned by the function stub provided with the SDK. 
	AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_Delay( AVL_uint32 uiMS );

	/// Performs an I2C read operation.
	/// 
	/// @param pAVLChip Pointer to the Availink device for which the read operation is being performed. 
	/// @param pucBuff Pointer to a buffer in which to place the read data.
	/// @param puiSize The number of bytes to be read. The function updates this value with the number of bytes actually read. If there is an error, the function sets this value to 0.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the read operation is successful. 
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// @remarks This function should perform a direct I2C read operation without first writing the device internal address. The Availink SDK automatically handles writing the device internal address prior to performing the read operation.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_I2CRead( const struct AVL_DVBSx_Chip * pAVLChip,  AVL_puchar pucBuff, AVL_puint16 puiSize );

	/// Performs an I2C write operation.
	/// 
	/// @param pAVLChip Pointer to the Availink device for which the write operation is being performed.
	/// @param pucBuff Pointer to a buffer which contains the data to be written.
	/// @param puiSize The number of bytes to be written. The function updates this value with the number of bytes actually written. If there is an error, the function sets this value to 0.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the write operation is successful. 
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_I2CWrite( const struct AVL_DVBSx_Chip * pAVLChip,  AVL_puchar pucBuff, AVL_puint16 puiSize );

	/// Initializes a semaphore object.
	/// 
	/// @param pSemaphore A pointer to the ::AVL_semaphore object to be initialized.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK Returned by the function stub provided with the SDK. 
	/// @remarks All of the semaphore objects should be initialized with 1 as maximum count and the initialized state should be signaled. In particular, after initialization, the first query should succeed.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_InitSemaphore( AVL_psemaphore pSemaphore ); 

	/// Queries the semaphore. If the semaphore is held by another thread, the function should be blocked until the semaphore is available.
	/// 
	/// @param pSemaphore A pointer to the ::AVL_semaphore object being queried.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK Returned by the function stub provided with the SDK. 
	AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_WaitSemaphore( AVL_psemaphore pSemaphore );         

	/// Releases the semaphore so that it is available.
	/// 
	/// @param pSemaphore A pointer to the ::AVL_semaphore object which is being released.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK Returned by the function stub provided with the SDK. 
	AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_ReleaseSemaphore( AVL_psemaphore pSemaphore );

	#ifdef AVL_CPLUSPLUS
}
	#endif
#endif
