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
/// @brief Defines data structures, enumerations, and global macros for the AVL_DVBSx device.
/// 
//$Revision: 914 $ 
//$Date: 2009-07-30 09:59:11 -0400 (Thu, 30 Jul 2009) $
// 
#ifndef AVL_DVBSx_h_h
	#define AVL_DVBSx_h_h

	#include "avl.h"

	#ifdef AVL_CPLUSPLUS
extern "C" {
	#endif

	// The CI_FLAG_... macros below pertain to the m_Flags member of the AVL_DVBSx_Channel structure.
	#define CI_FLAG_IQ_BIT							0x00000000  /// The index of the bit which controls the IQ swap setting
	#define CI_FLAG_IQ_BIT_MASK						0x00000001  /// A bit mask for accessing the IQ swap bit
	#define CI_FLAG_IQ_NO_SWAPPED					0x00000000	/// Setting the CI_FLAG_IQ_BIT to this value means that the I and Q signals are not swapped
	#define CI_FLAG_IQ_SWAPPED						0x00000001	/// Setting the CI_FLAG_IQ_BIT to this value means that the I and Q signals are swapped

	#define CI_FLAG_DVBS2_BIT						0x00000002	/// The start index of the bit which controls the standard setting
	#define CI_FLAG_DVBS2_BIT_MASK					0x0000001c	/// A bit mask for accessing the standard setting 
	#define CI_FLAG_DVBS 							0x00000000	/// The value which represents the DVB-S standard
	#define CI_FLAG_DVBS2							0x00000001	/// The value which represents the DVB-S2 standard
	#define CI_FLAG_DVBS2_UNDEF						0x00000004  /// The value which indicates that the standard is not known

	#define CI_FLAG_IQ_AUTO_BIT						0x00000005	/// The index of the bit which controls the automatic IQ swap setting
	#define CI_FLAG_IQ_AUTO_BIT_MASK				0x00000020  /// A bit mask for accessing the automatic IQ swap bit
	#define CI_FLAG_IQ_AUTO_BIT_OFF					0x00000000  /// Setting the CI_FLAG_AUTO_BIT to this value disables automatic IQ swap
	#define CI_FLAG_IQ_AUTO_BIT_AUTO				0x00000001  /// Setting the CI_FLAG_AUTO_BIT to this value enables automatic IQ swap

	#define CI_FLAG_LOCK_MODE_BIT					0x00000006	/// The index of the bit which controls the channel lock mode
	#define CI_FLAG_LOCK_MODE_BIT_MASK				0x00000040	/// A bit mask for accessing the channel lock mode

	#define CI_FLAG_ADAPTIVE_POWER_SAVE_BIT			0x00000007	/// The index of the bit which controls whether adaptive power save mode is enabled
	#define CI_FLAG_ADAPTIVE_POWER_SAVE_BIT_MASK	0x00000080	/// A bit mask for accessing the adaptive power save mode bit

	typedef AVL_uint16 AVL_DVBSx_ErrorCode;		///< Defines the return code for Availink device operations. \sa AVL_DVBSx_EC_OK, AVL_DVBSx_EC_GeneralFail, AVL_DVBSx_EC_I2CFail, AVL_DVBSx_EC_TimeOut, AVL_DVBSx_EC_Running, AVL_DVBSx_EC_MemoryRunout, AVL_DVBSx_EC_BSP_ERROR1, AVL_DVBSx_EC_BSP_ERROR2

	#define AVL_DVBSx_EC_OK					0	///< There is no error.
	#define AVL_DVBSx_EC_GeneralFail		1	///< Some general failure happened.
	#define AVL_DVBSx_EC_I2CFail			2	///< I2C bus failed.
	#define AVL_DVBSx_EC_TimeOut			4	///< Operation failed in a given time period
	#define AVL_DVBSx_EC_Running			8	///< The Availink device is still processing a previous command.
	#define AVL_DVBSx_EC_InSleepMode		16	///< The requested receiver command could not be sent because the Availink device is in sleep mode.
	#define AVL_DVBSx_EC_MemoryRunout		32	///< The Availink device has insufficient memory to complete the current operation.
	#define AVL_DVBSx_EC_BSP_ERROR1			64	///< User defined error code for reporting BSP operation failure.
	#define AVL_DVBSx_EC_BSP_ERROR2			128	///< User defined error code for reporting BSP operation failure.

	#define AVL_DVBSx_SA_0					0x0C ///< Availink device slave address 0.  Up to two Availink devices are supported on one I2C bus.  
	#define AVL_DVBSx_SA_1					0x0D ///< Availink device slave address 1.  Up to two Availink devices are supported on one I2C bus.

	///
	/// Represents the code rate. The Availink device can automatically detect the code rate of the input signal.
	enum AVL_DVBSx_FecRate
	{
		RX_DVBS_1_2 = 0,		///< = 0 Code rate 1/2
		RX_DVBS_2_3 = 1,		///< = 1 Code rate 2/3
		RX_DVBS_3_4 = 2,		///< = 2 Code rate 3/4
		RX_DVBS_5_6 = 3,		///< = 3 Code rate 5/6
		RX_DVBS_6_7 = 4,		///< = 4 Code rate 6/7
		RX_DVBS_7_8 = 5,		///< = 5 Code rate 7/8
		RX_DVBS2_1_4 = 6,		///< = 6 Code rate 1/4
		RX_DVBS2_1_3 = 7,		///< = 7 Code rate 1/3
		RX_DVBS2_2_5 = 8,		///< = 8 Code rate 2/5
		RX_DVBS2_1_2 = 9,		///< = 9 Code rate 1/2
		RX_DVBS2_3_5 = 10,		///< = 10 Code rate 3/5
		RX_DVBS2_2_3 = 11,		///< = 11 Code rate 2/3
		RX_DVBS2_3_4 = 12,		///< = 12 Code rate 3/4
		RX_DVBS2_4_5 = 13,		///< = 13 Code rate 4/5
		RX_DVBS2_5_6 = 14,		///< = 14 Code rate 5/6
		RX_DVBS2_8_9 = 15,		///< = 15 Code rate 8/9
		RX_DVBS2_9_10 = 16		///< = 16 Code rate 9/10
	};

	///
	/// Represents the Pilot mode of the signal. The Availink device can automatically detect the Pilot mode of the received signal.
	enum AVL_DVBSx_Pilot
	{
		RX_Pilot_off = 0,		///< = 0 Pilot off
		RX_Pilot_on = 1			///< = 1 Pilot on
	};

	///
	/// Represents the channel lock mode.
	enum AVL_DVBSx_LockMode
	{
		AVL_DVBSx_LOCK_MODE_FIXED = 0,		///< = 0 Fixed lock mode
		AVL_DVBSx_LOCK_MODE_ADAPTIVE = 1	///< = 1 Adaptive lock mode
	};

	///
	/// Represents the modulation mode. The Availink device can automatically detect the modulation mode of the received signal.
	enum AVL_DVBSx_ModulationMode
	{
		AVL_DVBSx_MM_QPSK = 0,		///< = 0 QPSK
		AVL_DVBSx_MM_8PSK = 1,		///< = 1 8-PSK
		AVL_DVBSx_MM_16APSK = 2,	///< = 2 16-APSK
		AVL_DVBSx_MM_32APSK = 3		///< = 3 32-APSK
	};

	///
	/// The roll off of the received signal. The Availink device can automatically detect this value from the received signal.
	enum AVL_DVBSx_RollOff
	{
		AVL_DVBSx_RO_20 = 0,		///<  = 0 Rolloff is 0.20
		AVL_DVBSx_RO_25 = 1,		///<  = 1 Rolloff is 0.25
		AVL_DVBSx_RO_35 = 2			///<  = 2 Rolloff is 0.35
	};

	///
	/// The MPEG output format. The default value in the Availink device is \a AVL_DVBSx_MPF_TS
	enum AVL_DVBSx_MpegFormat
	{
		AVL_DVBSx_MPF_TS = 0,		///< = 0  Transport stream format.
		AVL_DVBSx_MPF_TSP = 1		///< = 1  Transport stream plus parity format.
	};

	///
	/// The MPEG output mode. The default value in the Availink device is \a AVL_DVBSx_MPM_Parallel
	enum AVL_DVBSx_MpegMode
	{
		AVL_DVBSx_MPM_Parallel = 0,		///< = 0  Output MPEG data in parallel mode
		AVL_DVBSx_MPM_Serial = 1		///< = 0  Output MPEG data in serial mode
	};

	///
	/// The MPEG output clock polarity. The clock polarity should be configured to meet the back end device's requirement.
	/// The default value in the Availink device is \a AVL_DVBSx_MPCP_Rising
	enum AVL_DVBSx_MpegClockPolarity
	{
		AVL_DVBSx_MPCP_Falling = 0,		///<  = 0  The MPEG data is valid on the falling edge of the clock.
		AVL_DVBSx_MPCP_Rising = 1		///<  = 1  The MPEG data is valid on the rising edge of the clock.
	};

	///
	/// Internal MPEG pull-down resistor control. Used to enable or disable the internal MPEG interface pull-down resistors.
	/// The default value in AVL_DVBSx is \a AVL_DVBSx_MPPD_Enable
	enum AVL_DVBSx_MpegPulldown
	{
		AVL_DVBSx_MPPD_Disable = 0,		///< = 0 The internal MPEG interface pull-down resistors are disabled.
		AVL_DVBSx_MPPD_Enable = 1		///< = 1 The internal MPEG interface pull-down resistors are enabled.
	};

	///
	/// Defines the output bit order of the MPEG data bytes.  The meaning differs depending on whether the MPEG interface 
	/// is configured to output data in serial mode or parallel mode as follows:
	/// Serial Mode: Normal - The device outputs the MSB of each byte first and the LSB of each byte last.
	//               Invert - The device outputs the LSB of each byte first and the MSB of each byte last.
	/// Parallel Mode: Normal - The device outputs the MSB of each byte on pin MPEG_DATA_7 and the LSB of each byte on pin MPEG_DATA_0.
	///                Invert - The device outputs the LSB of each byte on pin MPEG_DATA_7 and the MSB of each byte on pin MPEG_DATA_0.
	enum AVL_DVBSx_MpegBitOrder
	{
		AVL_DVBSx_MPBO_Normal = 0,		///< = 0 Normal output bit order
		AVL_DVBSx_MPBO_Invert = 1		///< = 1 Inverted output bit order
	};

	///
	/// Defines the pin on which the Availink device outputs the MPEG data when the MPEG interface has been configured to operate 
	/// in serial mode.
	enum AVL_DVBSx_MpegSerialPin
	{
		AVL_DVBSx_MPSP_DATA0 = 0,		///< = 0 Serial data is output on pin MPEG_DATA_0
		AVL_DVBSx_MPSP_DATA7 = 1		///< = 1 Serial data is output on pin MPEG_DATA_7
	};

	///
	/// Defines the polarity of the MPEG data valid signal when the MPEG interface is configured to operate in TSP mode.
	enum AVL_DVBSx_MpegValidPolarity
	{
		AVL_DVBSx_MPVP_Normal = 0,		///< = 0 The MPEG data valid signal is high during the payload data and low during the parity bytes.
		AVL_DVBSx_MPVP_Invert = 1		///< = 1 The MPEG data valid signal is low during the payload data and high during the parity bytes.
	};

	///
	/// Defines the polarity of the MPEG error signal.
	enum AVL_DVBSx_MpegErrorPolarity
	{
		AVL_DVBSx_MPEP_Normal = 0,		///< = 0 The MPEG error signal is high during the payload of a packet which contains uncorrectable error(s).
		AVL_DVBSx_MPEP_Invert = 1		///< = 1 The MPEG error signal is low during the payload of a packet which contains uncorrectable error(s).
	};

	///
	/// Defines the polarity of the RF AGC control signal.   The polarity of the RF AGC control signal must be
	/// configured to match that required by the tuner.
	/// The default value in the Availink device is \a AVL_DVBSx_RA_Invert
	enum AVL_DVBSx_RfagcPola
	{
		AVL_DVBSx_RA_Normal = 0,	///< = 0  Normal polarization. This setting is used for a tuner whose gain increases with increased AGC voltage.
		AVL_DVBSx_RA_Invert = 1		///< = 1  Inverted polarization. The default value. Most tuners fall into this category.  This setting is used for a tuner whose gain decreases with increased AGC voltage.
	};

	///
	/// Defines the device functional mode.
	enum AVL_DVBSx_FunctionalMode
	{
		AVL_DVBSx_FunctMode_Demod = 0,	    ///< = 0 The device is in demod mode.
		AVL_DVBSx_FunctMode_BlindScan = 1	///< = 1 The device is in blind scan mode
	};

	/// 
	/// Defines the device spectrum polarity setting. 
	enum AVL_DVBSx_SpectrumPolarity
	{
		AVL_DVBSx_Spectrum_Normal = 0,		///< = 0 The received signal spectrum is not inverted.
		AVL_DVBSx_Spectrum_Invert = 1		///< = 1 The received signal spectrum is inverted.
	};

	///
	/// Defines the sleep-wake status of the Availink device
	enum AVL_DVBSx_Sleep_Wake_Status
	{
		AVL_DVBSx_Sleep_Mode = 0,
		AVL_DVBSx_Wake_Mode = 1
	};

	/// Stores the MPEG interface configuration parameters that typically need to be configured for the user application.
	struct AVL_DVBSx_MpegInfo
	{
		enum AVL_DVBSx_MpegFormat m_MpegFormat;		///< The MPEG output format (TS or TSP)
		enum AVL_DVBSx_MpegMode m_MpegMode;			///< The MPEG output mode (parallel or serial)
		enum AVL_DVBSx_MpegClockPolarity m_MpegClockPolarity; ///< The polarity of the MPEG clock signal
	} ;

    /// Stores the properties which characterize the received signal
	struct AVL_DVBSx_SignalInfo
	{
		enum AVL_DVBSx_Pilot m_pilot;				///< The pilot mode. \sa ::AVL_DVBSx_Pilot.
		enum AVL_DVBSx_FecRate m_coderate;			///< The code rate. \sa ::AVL_DVBSx_FecRate.
		enum AVL_DVBSx_ModulationMode m_modulation;	///< The modulation mode. \sa ::AVL_DVBSx_ModulationMode.
		enum AVL_DVBSx_RollOff m_rolloff;			///< The roll-off (excess bandwidth). \sa ::AVL_DVBSx_RollOff.
	};

    /// Stores the carrier channel parameters.
	struct AVL_DVBSx_Channel
	{
		AVL_uint32 m_uiFrequency_kHz;		///< The channel carrier frequency in units of kHz. 
		AVL_uint32 m_uiSymbolRate_Hz;		///< The symbol rate in units of Hz. 
		AVL_uint32 m_Flags;					///< Contains bit-mapped fields which store additional channel configuration information.
	};

    /// The structure stores the data and flags associated with the Availink device.
	struct AVL_DVBSx_Chip
	{
		AVL_uint32 m_I2CChn;                ///< Device I2C Num. Added by l00185424, 2012/03/03
		AVL_uint16 m_SlaveAddr;				///< Device I2C slave address.
		AVL_uint16 m_uiBusId;				///< Bus identifier.
		AVL_uint16 m_StdBuffIndex;			///< The internal stdout port buffer index.
		AVL_uint16 m_DemodFrequency_10kHz;	///< The internal demod clock frequency corresponding to the current PLL configuration in units of 10kHz.
		AVL_uint16 m_FecFrequency_10kHz;	///< The FEC clock frequency corresponding to the current PLL configuration in units of 10kHz.
		AVL_uint16 m_MpegFrequency_10kHz;	///< The MPEG clock corresponding to the current PLL configuration in units of 10 kHZ.
		AVL_semaphore m_semRx;				///< A semaphore used to protect the receiver command channel.
		AVL_semaphore m_semI2CRepeater;		///< A semaphore used to protect the I2C repeater channel.
		AVL_semaphore m_semI2CRepeater_r;	///< A semaphore used to protect the I2C repeater channel data reading.
		AVL_semaphore m_semDiseqc;			///< A semaphore used to protect the DiSEqC operations.
	};

	/// Configures the Availink device's PLL. The SDK provides an array of PLL configurations in IBSP.h. The user may remove unused elements in that array to reduce the SDK footprint. Availink advises users to refrain from changing the PLL setup values themselves. Please contact Availink if there is a need to support a PLL configuration not already provided.
	struct AVL_DVBSx_PllConf
	{
		AVL_uint16 m_uiClkf;					///< Feedback clock divider
		AVL_uint16 m_uiClkr;					///< Reference clock divider
		AVL_uint16 m_uiPllod;					///< PLL output divider
		AVL_uint16 m_uiPllod2;					///< PLL output divider 2
		AVL_uint16 m_uiPllod3;					///< PLL output divider 3
		AVL_uint16 m_RefFrequency_kHz;			///< Reference clock frequency in units of kHz
		AVL_uint16 m_DmodFrequency_10kHz;		///< Demod clock frequency in units of 10kHz
		AVL_uint16 m_FecFrequency_10kHz;		///< FEC clock frequency in units of 10 kHz
		AVL_uint16 m_MpegFrequency_10kHz;		///< MPEG clock frequency in units of 10 kHz
	} ;

	/// The structure for storing the version information associated with the Availink device and its associated software. \sa AVL_VerInfo
	struct AVL_DVBSx_VerInfo
	{
		struct AVL_VerInfo m_Chip;		///< Hardware version. Should be 1.0.2.
		struct AVL_VerInfo m_API;		///< SDK version.
		struct AVL_VerInfo m_Patch;		///< The version of the firmware patch.
	} ;

	/// Initializes an ::AVL_DVBSx_Chip object. The user must first define an object of type ::AVL_DVBSx_Chip, and then pass the address of the object to this function. This function initializes semaphores for the object and sets the I2C slave address.
	/// 
	/// @param pAVL_DVBSx_ChipObject  A pointer to the ::AVL_DVBSx_Chip object.
	/// @param uiSlaveAddress  The slave address for the Availink device.
	/// 
	/// @return ::AVL_DVBSx_ErrorCode, Return ::AVL_DVBSx_EC_OK if the initialization is successful.
	/// @remarks This function initializes the semaphores without any checking. It is the user's responsibility to make sure that each object is initialized only once. 
	AVL_DVBSx_ErrorCode Init_AVL_DVBSx_ChipObject(struct AVL_DVBSx_Chip * pAVL_DVBSx_ChipObject, AVL_uint16 uiSlaveAddress);

	/// Declaration of the global PLL configuration array variable. The variable must be defined in IBSP.c
	/// 
	extern const struct AVL_DVBSx_PllConf pll_conf[];
	extern const AVL_uint16 pll_array_size;

	#ifdef AVL_CPLUSPLUS
}
	#endif
#endif
