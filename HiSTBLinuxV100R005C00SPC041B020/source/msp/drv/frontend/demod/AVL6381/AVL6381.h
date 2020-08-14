#ifndef AVL6381_H
#define AVL6381_H

#include <linux/delay.h>
#include <linux/module.h>
#include <linux/init.h>

#include "drv_tuner_ext.h"
#include "hi_drv_i2c.h"
#include "hi_type.h"
#include "drv_demod.h"
#include "hi_kernel_adapt.h"
#include "drv_tuner_ioctl.h"

#include "AVL6381_API.h"
#include "AVL6381_Internal.h"
#include "AVL6381_iic_rw.h"
#include "AVL6381_data_type.h"


struct AVL6381_Tuner
{            
	AVL6381_uint16 m_uiSlaveAddress;        //< The Tuner slave address. It is the write address of the tuner device. In particular it is an 8-bit address, with the LSB set to zero. The Availink device does not support 10-bit I2C addresses. 
    AVL6381_uint16 m_uiI2CBusClock_kHz;     //< The clock speed of the I2C bus that is dedicated to tuner control.  The units are kHz.
    AVL6381_uint16 m_uiFrequency_100kHz;    //< The tuned frequency in units of 100kHz.
    AVL6381_uint16 m_uiLPF_100kHz;          //< The lowpass filter bandwidth of the tuner.
    AVL6381_uint32 m_uiSymbolRate_Hz;       //< The symbol rate of the incoming channel.
    void * m_pParameters;                   //< A pointer to the tuner's customized parameters baseband gain, etc.
    struct AVL6381_Chip * m_pAVLChip; 		//< A pointer to the Availink device connected to the Tuner.
};

// Defines the pin on which the Availink device outputs the MPEG data when the MPEG interface has been configured to operate 
// in serial mode.
enum AVL6381_MpegSerialPin
{
    AVL6381_MPSP_DATA0 = 0, 		//< = 0 Serial data is output on pin MPEG_DATA_0
    AVL6381_MPSP_DATA7 = 1 			//< = 1 Serial data is output on pin MPEG_DATA_7
};

// The MPEG output clock polarity. The clock polarity should be configured to meet the back end device's requirement.
// The default value in the Availink device is \a AVL_DVBSx_MPCP_Rising
enum AVL6381_MpegClockPolarity
{
    AVL6381_MPCP_Falling = 0,     	//<  = 0  The MPEG data is valid on the falling edge of the clock.
    AVL6381_MPCP_Rising = 1      	//<  = 1  The MPEG data is valid on the rising edge of the clock.
};


// The MPEG output format. The default value in the Availink device is \a AVL_DVBSx_MPF_TS
enum AVL6381_MpegFormat
{
    AVL6381_MPF_TS = 0,       		//< = 0  Transport stream format.
    AVL6381_MPF_TSP = 1       		//< = 1  Transport stream plus parity format.
};

// Defines the polarity of the RF AGC control signal.   The polarity of the RF AGC control signal must be
// configured to match that required by the tuner.
// The default value in the Availink device is \a AVL_DVBSx_RA_Invert
enum AVL6381_RfagcPola
{
    AVL6381_RA_Normal = 0,    		//< = 0  Normal polarization. This setting is used for a tuner whose gain increases with increased AGC voltage.
    AVL6381_RA_Invert = 1     		//< = 1  Inverted polarization. The default value. Most tuners fall into this category.  This setting is used for a tuner whose gain decreases with increased AGC voltage.
};

// Defines the device spectrum polarity setting. 
enum AVL6381_SpectrumPolarity
{
    AVL6381_Spectrum_Normal = 0,  	//< = 0 The received signal spectrum is not inverted.
    AVL6381_Spectrum_Invert = 1  	//< = 1 The received signal spectrum is inverted.
};


#endif

