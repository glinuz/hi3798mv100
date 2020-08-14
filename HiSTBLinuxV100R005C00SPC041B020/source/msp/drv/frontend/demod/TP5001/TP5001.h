
#ifndef __TP5001_H__
#define __TP5001_H__
#include "TP_TYPE.h"
//#include "RDA5812.h"
#include "RDA5815.h"
//#include "RDA5815M.h"
//#include "av2020.h"
//#include "S305.h"
//#include "sharp6306.h"
//#include "sharp6903.h"
//#include "IIC.h"
#ifdef _USE_TP5001_CHIP_

#define SERIES_PORT 1
 
#define SHARP_6306_TUNER		1
#define RDA_5812_TUNER			2
#define SHARP_6903_TUNER		3
#define S305_TUNER				4
#define AV2020_TUNER			5
#define RDA_5815_TUNER			6
#define RDA_5815M_TUNER			7

// ==========================================================================================================
// Define  & Marco
// 
// ---------------------------------------------------------------------------------------------------------
// Error Code:
// 

//#define TP5001_DEVADDR		0x18//0x0c//0x18

#define TP_SUCCESS										0
#define TP_IIC_WRERR									1
#define TP_IIC_RDERR									2
#define TP_IIC_WR_TOO_LONG								3
#define TP_IIC_RD_TOO_LONG								4
#define TP_SET_TUNER_ERR								5
#define TP_NOT_SUPPORT									6
#define TP_TUNER_IIC_WR_TOO_LONG						7
#define TP_TUNER_IIC_TIME_OUT							8
#define TP_NOT_LOCK										9
#define TP_PARA_ERR										10
#define TP_TUNER_ID_ERR									11
#define TP_ERROR_DIV0									12
#define TP_IIC_IDLE_ERR                                 13
#define TP_CHIP_ID_ERROR                                14

// ---------------------------------------------------------------------------------------------------------
typedef enum tag_TP_RFAGCPola 
{
	RA_Normal,
	RA_Invert
}TP_RFAGCPola;

typedef enum tag_TP_IQPola 
{
	Normal,
	Invert
}TP_IQPola;

typedef enum tag_TP_SPI_edge
{
	positive_edge,		// ????????Ч
	negitive_edge
}TP_SPI_edge;

typedef enum tag_TP_SPI_port_type
{
	series_port,		// \u017d???
	para_port,			// ????
	out_disable         // \u017e???
}TP_SPI_port_type;

// ---------------------------------------------------------------------------------------------------------
// Parameter

#ifdef   __cplusplus
    extern   "C" 
    {
#endif

// ---------------------------------------------------------------------------------------------------------
// ==========================================================================================================


// ==========================================================================================================
// External Function
// 

// ---------------------------------------------------------------------------------------------------------
// Function Name: TP_init
// Description: TP?\u0178Ƭ?????\u0152??
// Return:
//		TP_SUCCESS: ?????ɹ\u0160
//
TP_UINT8 TP_init(TP_UINT8 tuner_type);
// ---------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------
// Function Name: TP_deinit
// Description: TP?\u0178Ƭ???\u017d???\u0152??
// Return:
//		TP_SUCCESS: ?????ɹ\u0160
//
TP_UINT8 TP_deinit(void);
// ---------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------
// Function Name: TP_get_version
// Description: TP?\u0178Ƭ???õ\u0153???\u0178??
// Output:
//		pVersion:  
// Return:
//		TP_SUCCESS: ?????ɹ\u0160
//
TP_UINT8 TP_get_version(TP_INT8 * pVersion);
// ---------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------
// Function Name: TP_set_rf_tuner
// Description: TP?\u0178Ƭ??????Tuner
// Input:
//		frequency:  Ƶ??
// Return:
//		TP_SUCCESS: ?????ɹ\u0160
//
TP_UINT8 TP_set_rf_tuner(TP_UINT32 frequency,TP_UINT32 symbol_rate);
// ---------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------
// Function Name: TP_set_rf_pola
// Description: TP?\u0178Ƭ??????\u0152???
// Input:
//		Pola: \u0152??ԣ?Normal: ????(VH_SEL??\u0153?Ϊ??)??Invert: ?\u017d??(VH_SEL??\u0153?Ϊ\u017e?)
// Return:
//		TP_SUCCESS: ???\u0161
//
TP_UINT8 TP_set_rf_pola(TP_RFAGCPola Pola); 
// ---------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------
// Function Name: TP_set_iq_switch
// Description: TP?\u0178Ƭ??????IQ?\u017d??
// Input:
//		iq_pola: IQ??\u0152???   Normal:????   Invert: ?\u017d??
// Return:
//		TP_SUCCESS: ?????ɹ\u0160
//
TP_UINT8 TP_set_iq_switch(TP_IQPola iq_pola);
// ---------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------
// Function Name: TP_set_symbol_rate
// Description: TP?\u0178Ƭ?????÷?????
// Input:
//		symbol_rate:  ??????
// Return:
//		TP_SUCCESS: ?????ɹ\u0160 
//
TP_UINT8 TP_set_symbol_rate(TP_UINT32 symbol_rate);
// ---------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------
// Function Name: TP_ts_interface
// Description: TP?\u0178Ƭ??????AGC????
// Input:
//		active_edge: positive_edge, ??????     negitive_edge, ??\u0153???
//		port_type: series_port, \u017d???      para_port, ????
//		clock_div: SPIʱ?ӷ?Ƶ????,    SPI_CLK = 380 / (clock_div * 4)
// Return:
//		TP_SUCCESS: ?????ɹ\u0160
//
TP_UINT8 TP_set_ts_interface(TP_SPI_edge active_edge, TP_SPI_port_type port_type, TP_UINT8 clock_div);
// ---------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------
// Function Name: TP_get_signal_quality
// Description: TP?\u0178Ƭ???õ\u0153?ź???
// Output:
//		p_quality_percent:  ?? 0: 0%  --   100: 100%
// Return:
//		TP_SUCCESS: ?????ɹ\u0160
//
TP_UINT8 TP_get_signal_quality(TP_UINT8 * p_quality_percent);
// ---------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------
// Function Name: TP_get_signal_quality BD
// Description: TP?\u0178Ƭ???õ\u0153?ź???
// Output:
//		p_quality_DB:  ?? DB
// Return:
//		TP_SUCCESS: ?????ɹ\u0160
//
TP_UINT8 TP_get_signal_quality_DB(TP_UINT8 * p_quality_DB);
// ---------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------
// Function Name: TP_get_signal_strength
// Description: TP?\u0178Ƭ???õ\u0153?ź?ǿ??
// Output:
//		p_signal_strength:  ?ź?ǿ??
// Return:
//		TP_SUCCESS: ?????ɹ\u0160
//
TP_UINT8 TP_get_signal_strength(TP_UINT8 * p_signal_strength);
// ---------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------
// Function Name: TP_get_lock_status
// Description: TP?\u0178Ƭ???õ\u0153???\u0161?\u017d̬
// Return:
//		TP_SUCCESS: ???\u0161
//		TP_NOT_LOCK: û?????\u0161
//
TP_UINT8 TP_get_lock_status(void);
// ---------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------
// Function Name: TP_get_freq_offset
// Description: TP?\u0178Ƭ???õ\u0153Ƶƫֵ
// Output:
//		p_freq_offset: Ƶ??ƫ??ֵ????λ(KHz)
// Return:
//		TP_SUCCESS: ?????ɹ\u0160
//
TP_UINT8 TP_get_freq_offset(TP_INT32 * p_freq_offset);
// ---------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------
// Function Name: TP_set_sleep
// Description: TP?\u0178Ƭ\u0153???˯??ģ?\u0153
// Return:
//		TP_SUCCESS: ?????ɹ\u0160
//
TP_UINT8 TP_set_sleep(void);
// ---------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------
// Function Name: TP_soft_reset
// Description: TP?\u0178Ƭ??\u017e\u017dλ
// Return:
//		TP_SUCCESS: ?????ɹ\u0160
//
TP_UINT8 TP_soft_reset(void);
// ---------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------
// Function Name: TP_SendDiseqcCommand
// Description: TP????Diseqc????
// Paramenter:
//			length: input, ????Diseqc?????ĳ\u20ac??
//          pCommands: input, ????Diseqc????????\u0178?
//          pReturnLength: Input/Output \u0153?????\u0178ݵĳ\u20ac??
//          pReturnBuffer: \u0153?????\u0178ݵ?buffer
// Return:
//		TP_SUCCESS: ?????ɹ\u0160
//
TP_UINT8 TP_SendDiseqcCommand(TP_UINT8  length, 
                                 TP_UINT8* pCommands, 
                                 TP_UINT8* pReturnLength, 
                                 TP_UINT8* pReturnBuffer);
// ---------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------
// Function Name: TP_get_statistic_ber_bler
// Description: TP5001?\u0178Ƭ???õ\u0153BER??BLERͳ\u0152?ֵ
// Return:
//		TP_SUCCESS: ?????ɹ\u0160
//
TP_UINT8 TP_get_statistic_ber_bler(TP_UINT64 *pBer_total, TP_UINT64 *pBer_error, TP_UINT64 *pBler_total, TP_UINT64 *pBler_error);
// ---------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------
// Function Name: TP_clear_statistic_ber_bler
// Description: TP5001?\u0178Ƭ??????BER??BLERͳ\u0152?ֵ
// Return:
//		TP_SUCCESS: ?????ɹ\u0160
//
TP_UINT8 TP_clear_statistic_ber_bler(TP_UINT8 type);
// ---------------------------------------------------------------------------------------------------------

TP_UINT8 TP_Standby(void);
TP_UINT8 TP_Resume(void);



TP_UINT8 TP_iic_tuner_write(TP_UINT8 dev_addr, TP_UINT8 * value, TP_UINT32 length);
TP_UINT8 TP_iic_tuner_read(TP_UINT8 dev_addr, TP_UINT8 reg_addr, TP_UINT8 * value, TP_UINT32 length);
                                 

TP_UINT8 TP_iic_write(TP_UINT8 device_address, TP_UINT16 register_address, TP_UINT8 * value_buffer, TP_UINT32 length);

TP_UINT8 TP_iic_read(TP_UINT8 device_address, TP_UINT16 register_address, TP_UINT8 * value_buffer, TP_UINT32 length);

void TP_Delay( TP_UINT32 uiMS );

void TP_Reset(void);

#ifdef   __cplusplus
    }
#endif
#endif
#endif //_USE_TP5001_CHIP_

