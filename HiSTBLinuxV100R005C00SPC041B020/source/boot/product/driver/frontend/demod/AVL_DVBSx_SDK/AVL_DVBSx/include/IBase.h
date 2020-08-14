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
/// @brief Declares functions for generic device level operations.
/// 
//$Revision: 985 $ 
//$Date: 2009-11-09 18:09:24 -0500 (Mon, 09 Nov 2009) $
// 
#ifndef IBase_h_h
	#define IBase_h_h

	#include "avl_dvbsx.h"
	#include "ITuner.h"
	#include "avl_dvbsx_globals.h"

	#ifdef AVL_CPLUSPLUS
extern "C" {
	#endif

	///@cond
	#define core_reset_b_reg			0x600000
	#define gpio_data_in_to_reg			0x6C0004
	#define gpio_data_reg_out			0x6C0008
	#define gpio_reg_enb				0x6C000C

	#define pll_clkr_map_addr			0x6C40C0
	#define pll_clkf_map_addr			0x6C4100
	#define pll_od_map_addr				0x6C4080
	#define pll_od2_map_addr			0x6C4140
	#define pll_od3_map_addr			0x6C4180
	#define pll_bwadj_map_addr			0x6C41C0
	#define pll_softvalue_en_map_addr	0x6C4200
	#define reset_register_addr			0x6C4000

	#define OP_RX_NOOP		    		0x00
	#define OP_RX_LD_DEFAULT    		0x01
	#define OP_RX_INIT_GO				0x02
	#define OP_RX_RESET_BERPER	 		0x03
	#define OP_RX_HALT					0x04
	#define OP_RX_SLEEP         		0x05
	#define OP_RX_WAKE          		0x06
	#define OP_RX_BLIND_SCAN    		0x08
	#define OP_RX_STDOUT_MODE   		0x09
	///@endcond

	///@cond
	#define AVL_DVBSx_API_VER_MAJOR	0x01
	#define AVL_DVBSx_API_VER_MINOR	0x00
	#define AVL_DVBSx_API_VER_BUILD	0x05

	/// @cond
	/// Configures the PLL.
	/// 
	/// @param pPLLConf  Pointer to the PLL configuration object
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip object for which the PLL is being configured.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the PLL configuration has been successfully sent to the Availink device. 
	/// Return ::AVL_DVBSx_I2C_Fail if there is an I2C communication problem
	/// @remarks This function is an internal SDK function. Availink recommends that users refrain from directly calling this function.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IBase_SetPLL(const struct AVL_DVBSx_PllConf * pPLLConf, struct AVL_DVBSx_Chip * pAVLChip );

	/// Downloads the firmware to the Availink device.
	/// 
	/// @param pInitialData Pointer to the buffer which contains the firmware data.
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip object for which the firmware is being downloaded.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the firmware download is successful. 	
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// @remarks This function is an internal SDK function. Availink recommends that users refrain from directly calling this function. 
	AVL_DVBSx_ErrorCode AVL_DVBSx_IBase_DownloadFirmware(AVL_puchar pFirmwareData,  const struct AVL_DVBSx_Chip * pAVLChip);

	///@endcond
	
	/// Checks the Availink device status to determine whether initialization is complete.
	/// 
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip object for which initialization status is being checked.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if device initialization is complete. 
	/// Return ::AVL_DVBSx_EC_GeneralFail if device initialization has failed.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IBase_GetStatus( struct AVL_DVBSx_Chip * pAVLChip );

	/// Retrieves the Availink device version information.
	/// 
	/// @param pVerInfo Pointer to an object in which to store the retrieved version information.  Refer to ::AVL_DVBSx_Ver_Info for more details.
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip object for which the version information is being retrieved.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the version information has been retrieved.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IBase_GetVersion( struct AVL_DVBSx_VerInfo * pVerInfo, const struct AVL_DVBSx_Chip * pAVLChip ) ;

	/// Initializes the Availink device. This function boots the device. 
	/// The user may call ::AVL_DVBSx_IBase_GetStatus to check the device initialization status.
	/// 
	/// @param pPLLConf  Pointer to the ::AVL_DVBSx_PLLConf object which contains the device PLL settings. 
	/// @param pInitialData Pointer to the buffer which contains the firmware patch data.
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip object which is being initialized.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the firmware and configuration information has been successfully downloaded to the Availink device.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IBase_Initialize( const struct AVL_DVBSx_PllConf * pPLLConf, AVL_puchar pInitialData,  struct AVL_DVBSx_Chip * pAVLChip ) ;

	/// Sends an operational command to the Availink device.
	/// 
	/// @param ucOpCmd  The OP_RX_xxx command being sent to the device.
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip to which the command is being sent.  
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the operational command is sent to the device.
	/// Return ::AVL_DVBSx_EC_Running if the command could not be sent because the device is still processing a previous command.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// @remarks This function is an internal SDK function. Availink recommends that users refrain from directly calling this function. 
	AVL_DVBSx_ErrorCode AVL_DVBSx_IBase_SendRxOP(AVL_uchar ucOpCmd, struct AVL_DVBSx_Chip * pAVLChip );

	/// Checks if the Availink device has finished processing the last operational command sent to it.
	/// 
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip for which the operational command status is being checked.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the device has finished processing the last command. 
	/// Return ::AVL_DVBSx_EC_Running if the device is still processing a previous command.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// @remarks This function is an internal SDK function. Availink recommends that users refrain from directly calling this function.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IBase_GetRxOPStatus(const struct AVL_DVBSx_Chip * pAVLChip );

	/// Halts the Availink device.  In halt mode the device does not perform any channel processing.
	/// 
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip which is being halted.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the operational command has been sent to the device.
	/// Return ::AVL_DVBSx_EC_Running if the command could not be sent because the device is still processing a previous command.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IBase_Halt( struct AVL_DVBSx_Chip * pAVLChip );

	/// Places the Availink device in sleep mode.  In sleep mode some device blocks are held in an idle state to reduce power consumption.
	/// 
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip which is being placed in sleep mode.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the operational command has been sent to the device.
	/// Return ::AVL_DVBSx_EC_Running if the command could not be sent because the device is still processing a previous command.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IBase_Sleep( struct AVL_DVBSx_Chip * pAVLChip );

	/// Wakes the Availink device from sleep mode.  Upon waking from sleep, the device enters the halt mode.
	/// 
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip which is being waken from sleep.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the operational command has been sent to the device.
	/// Return ::AVL_DVBSx_EC_Running if the command could not be sent because the device is still processing a previous command.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IBase_Wake( struct AVL_DVBSx_Chip * pAVLChip );

	/// Sets the device functional mode.  The device can be configured for either demod mode or blind scan mode.  
	/// 
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip for which the functional mode is being set.
	/// @param enumFunctionalMode The functional mode into which the device is being placed.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the functional mode has been set. 	
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IBase_SetFunctionalMode(const struct AVL_DVBSx_Chip * pAVLChip, enum AVL_DVBSx_FunctionalMode enumFunctionalMode);

	/// Checks the current device functional mode. The Availink device can operate in either demodulation mode or blind scan mode.
	/// 
	/// @param pFunctionalMode Pointer to the ::AVL_DVBSx_FunctionalMode object in which to store the retrieved functional mode.
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip for which the functional mode is being retrieved.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the functional mode has been retrieved. 		
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IBase_GetFunctionalMode(enum AVL_DVBSx_FunctionalMode * pFunctionalMode,  const struct AVL_DVBSx_Chip * pAVLChip);

	/// Configures the direction of the GPIO pins.
	/// 
	/// @param ucDir This is a bitmapped field in which each of the three LSBs controls the direction of a particular GPIO pin.  Setting a direction bit to a 
	/// one configures the respective pin as an input, and setting the bit to a zero configures the pin as an output.  Bit 0 (the LSB) corresponds to pin CS_0, 
	/// bit 1 corresponds to pin LNB_CNTRL_1, and bit 2 corresponds to pin GPIO_CLK.
	/// @param pAVLChip A pointer point to the ::AVL_DVBSx_Chip object for which GPIO pin direction is being configured.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the GPIO pin direction is configured.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IBase_SetGPIODir( AVL_uchar ucDir, const struct AVL_DVBSx_Chip * pAVLChip );

	/// Sets the voltage level of any GPIO pins that have been configured as outputs.
	/// 
	/// @param ucVal This is a bitmapped field in which each of the three LSBs controls the voltage level of a particular GPIO pin that has been configured as an 
	/// output.  Setting a value bit to a zero drives the corresponding pin low, while setting a value bit to a one, drives the corresponding pin high.
	/// Bit 0 (the LSB) corresponds to pin CS_0, bit 1 corresponds to pin LNB_CNTRL_1, and bit 2 corresponds to pin GPIO_CLK.
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip object for which the GPIO pin output voltages are being configured.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the GPIO output pin voltages have been configured.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// @remarks This function only controls the voltage of those GPIO pins that have been configured as outputs.  If a GPIO pin has been configured as an input,
	/// the function has no effect for that pin.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IBase_SetGPIOVal( AVL_uchar ucVal, const struct AVL_DVBSx_Chip * pAVLChip );

	/// Reads the voltage level of the GPIO pins.
	/// 
	/// @param pucVal Pointer to a variable in which to store the GPIO pin voltage levels.  This is a bitmapped field in which each of the three LSBs will 
	/// contain the voltage level of a particular GPIO pin.  A value bit of zero means that the corresponding pin is low, while a value bit of one, means that
	/// the corresponding pin is high.  Bit 0 (the LSB) corresponds to pin CS_0, bit 1 corresponds to pin LNB_CNTRL_1, and bit 2 corresponds to pin GPIO_CLK.
	/// The GPIO pin voltage level can be read regardless of whether the pin has been configured as an input or output.
	/// @param pAVLChip Pointer to the ::AVL_DVBSx_Chip object for which the GPIO pin voltage levels are being read.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the GPIO pin voltages have been read.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is a I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IBase_GetGPIOVal( AVL_puchar pucVal, const struct AVL_DVBSx_Chip * pAVLChip );

	/// Checks the Availink device is in sleep status or wake status.
	/// 
	/// @param pChipStatus Pointer to the AVL_DVBSx_Sleep_Wake_Status object in which to store the retrieved chip status.
	/// @param pAVLChip Pointer to the AVL_DVBSx_Chip for which the operational command status is being checked.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the device has finished processing the last command. 
	/// Return ::AVL_DVBSx_EC_Running if the device is still processing a previous command.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	AVL_DVBSx_ErrorCode AVL_DVBSx_IBase_GetChipStatus( enum AVL_DVBSx_Sleep_Wake_Status * pChipStatus, const struct AVL_DVBSx_Chip * pAVLChip );

	/// Configures the device to indicate whether the tuner inverts the received signal spectrum.
	/// 
	/// @param enumSpectrumPolarity Indicates whether the tuner being used inverts the received signal spectrum.  
	/// @param pAVLChip A pointer to the ::AVL_DVBSx_Chip object for which the spectrum polarity setting is being configured.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, 
	/// Return ::AVL_DVBSx_EC_OK if the spectrum polarity configuration is successful.
	/// Return ::AVL_DVBSx_EC_I2CFail if there is an I2C communication problem.
	/// @remarks 
	AVL_DVBSx_ErrorCode AVL_DVBSx_IBase_SetSpectrumPolarity( enum AVL_DVBSx_SpectrumPolarity enumSpectrumPolarity, const struct AVL_DVBSx_Chip * pAVLChip );


	#ifdef AVL_CPLUSPLUS
}
	#endif

#endif
