#ifndef AVL6381_DATA_TYPE_H
#define AVL6381_DATA_TYPE_H


typedef  char AVL6381_char;     		//< 8 bits signed char data type.
typedef  unsigned char AVL6381_uchar;   //< 8 bits unsigned char data type.

typedef  short AVL6381_int16;   		//< 16 bits signed char data type.
typedef  unsigned short AVL6381_uint16; //< 16 bits unsigned char data type.

typedef  int AVL6381_int32;     		//< 32 bits signed char data type.
typedef  unsigned int AVL6381_uint32;   //< 32 bits unsigned char data type.


typedef  char * AVL6381_pchar;  			//< pointer to a 8 bits signed char data type.
typedef  unsigned char * AVL6381_puchar; 	//< pointer to a 8 bits unsigned char data type.

typedef  short * AVL6381_pint16;    		//< pointer to a 16 bits signed char data type.
typedef  unsigned short * AVL6381_puint16;  //< pointer to a 16 bits unsigned char data type.

typedef  int * AVL6381_pint32;  			//< pointer to a 32 bits signed char data type.
typedef  unsigned int * AVL6381_puint32; 	//< pointer to a 32 bits unsigned char data type.

typedef long * AVL6381_pint64;
typedef unsigned int * AVL6381_Puint64;

typedef struct AVL6381_uint64
{
    AVL6381_uint32 m_HighWord;   // The most significant 32-bits of the unsigned 64-bit integer
    AVL6381_uint32 m_LowWord;    // The least significant 32-bits of the unsigned 64-bit integer
}AVL6381_uint64;

// Stores a signed 64-bit integer
typedef struct AVL6381_int64
{
    AVL6381_int32 m_HighWord;    // The most significant 32-bits of the signed 64-bit integer
    AVL6381_uint32 m_LowWord;    // The least significant 32-bits of the signed 64-bit integer
}AVL6381_int64;


typedef AVL6381_uint16 AVL6381_IIC_ErrorCode;
typedef AVL6381_uint16 AVL6381_ErrorCode;       // Defines the error code 

// The structure stores the data and flags associated with the Availink device.
struct AVL6381_Chip
{    
    AVL6381_uint32 m_I2CChn;       //< Device I2C Num. Added by l00185424, 2012/03/03
    AVL6381_uint16 m_SlaveAddr;    //< Device I2C slave address.
}; 

typedef enum AVL6381_RxInput
{
    ADC_IF_I = 0,           // = 0 The input signal is a real IF signal routed to the AVL6381 device I channel input.
    ADC_IF_Q = 1,           // = 1 The input signal is a real IF signal routed to the AVL6381 device Q channel input.
    ADC_BB_IQ = 2       	// = 2 The input signal is a complex baseband signal.
}AVL6381_RxInput;

typedef enum AVL6381_Clock_Config
{
    AVL6381_Xtal_30M = 0,
    AVL6381_Xtal_16M = 1,
    AVL6381_Xtal_24M = 2,
    AVL6381_Xtal_27M = 3
}AVL6381_Clock_Config;


typedef struct AVL6381_InputConfig
{
    AVL6381_uint16 usDemodI2CAddr;
    AVL6381_Clock_Config ueRefConfig;
    AVL6381_RxInput ueDTMBInputPath;
    AVL6381_RxInput ueDVBCInputPath;
    AVL6381_uint32 uiDTMBIFFreqHz;
    AVL6381_uint32 uiDVBCIFFreqHz;
    AVL6381_uint32 uiDTMBSymbolRateHz;
    AVL6381_uint32 uiDVBCSymbolRateHz;
}AVL6381_InputConfig;

// The MPEG output mode. The default value in the Availink device is \a AVL_DVBSx_MPM_Parallel
typedef enum AVL6381_MPEG_Mode
{
    AVL6381_MPEG_PARALLEL = 0,           // =0  Output parallel MPEG data
    AVL6381_MPEG_SERIAL = 1              // =1  Output serial MPEG data via D7.
}AVL6381_MPEG_Mode; 

typedef enum AVL6381_MPEG_ClockMode
{
    AVL6381_MPEG_FALLING = 0,   //  =0  MPEG data should be sampled on the falling edge of the clock.
    AVL6381_MPEG_RISING = 1     //  =1  MPEG data should be sampled on the rising edge of the clock.
}AVL6381_MPEG_ClockMode;

typedef enum AVL6381_MPEG_Continuous
{
    AVL6381_MPEG_CONTINUOUS_ENABLE  = 0,
    AVL6381_MPEG_CONTINUOUS_DISABLE = 1
}AVL6381_MPEG_Continuous;


typedef struct AVL6381_MPEGConfig
{
    AVL6381_MPEG_Mode enumSerialParallel;
    AVL6381_MPEG_ClockMode enumRisingFalling;
    AVL6381_MPEG_Continuous enumConti;
}AVL6381_MPEGConfig;

typedef struct _AVL6381_Config{
    AVL6381_uchar sStartupMode;
    AVL6381_InputConfig sInputConfig;
    AVL6381_MPEGConfig sMPEGConfig;
}AVL6381_Config;


#endif

