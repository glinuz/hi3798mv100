/* **************************************************** */
/*!
   @file	MN_DMD_common.h
   @brief	Panasonic Demodulator Driver
			(private common functions & declarations)
   @author	R.Mori
   @date	2011/6/30
   @param
		(c)	Panasonic
   */
/* **************************************************** */


#ifndef MN8847X_DMD_COMMON_H
#define MN8847X_DMD_COMMON_H

#include "MN_DMD_driver.h"

#ifdef __cplusplus
extern "C" {
#endif
#include <linux/kernel.h>

#include "drv_tuner_ext.h"
#include "drv_demod.h"

/* **************************************************** */
/* Welcome ! Pls. add I2C api to control tuner and DMD in < MN_I2C.c >>*/
/* **************************************************** */
/* **************************************************** */
/* Demod type setting *//* < User define 1 > */
/* **************************************************** */
#define DEMOD_TYPE DMD_TYPE_MN88473 //DMD_TYPE_MN88472

// 1-1 SoC communicate with Tuner directly(SDA&SCL) or by Demod(MSDA&MSCL)
#define TUNER_I2C_MODE TUNER_CONTROL_BY_DEMOD//TUNER_CONTROL_BY_DEMOD

// 1-2 Tuner I2C control method by Demod 
#if(TUNER_I2C_MODE == TUNER_CONTROL_BY_DEMOD)
 #define DEMOD_CONTROL_TUNER_I2C_MODE TCB_I2C_FORMAT_REPEAT_MODE //MN88472 only support repeator mode
  #if (DEMOD_TYPE == DMD_TYPE_MN88473)
     #undef DEMOD_CONTROL_TUNER_I2C_MODE
    #define DEMOD_CONTROL_TUNER_I2C_MODE TCB_I2C_FORMAT_REPEAT_MODE//TCB_I2C_FORMAT_BYPASS_MODE //TCB_I2C_FORMAT_BYPASS_MODE
  #endif
#endif

/* **************************************************** */
/* Tuner type setting *//* < User define 2 > */
/* **************************************************** */
#define TUNER_TYPE MXL603//NXP18275//NXP18250B//MXL603

//Demod ask Tuner to output IF in range, for better performance
#if (TUNER_TYPE == MXL603)||(TUNER_TYPE == NMI120)
#define DMD_ADC_INPUT_RANGE DMD_ADC_INPUT_RANGE_DIFFERENTIAL_1_0Vpp
#elif (TUNER_TYPE == RDA5880H)||(TUNER_TYPE == NXP18275)
#define DMD_ADC_INPUT_RANGE DMD_ADC_INPUT_RANGE_DIFFERENTIAL_1_5Vpp
#else
//default setting
#define DMD_ADC_INPUT_RANGE DMD_ADC_INPUT_RANGE_DIFFERENTIAL_1_0Vpp
#endif


/* **************************************************** */
/* XTAL_IF setting *//* < User define 3 > */
/* **************************************************** */
//#define XTAL_FREQ DMD_XTAL20480KHZ_IF5MHZ //Recommended by Panasonic, for customer in China
//#define XTAL_FREQ DMD_XTAL20500KHZ_IF5MHZ //for MN88472 only
//#define XTAL_FREQ DMD_XTAL24000KHZ_IF5MHZ //XTAL shared by Mxl603 and MN8847x
#define XTAL_FREQ DMD_XTAL24000KHZ_IF5MHZ//for MN88473 only

//The ADC sampling frequency
#if (DEMOD_TYPE == DMD_TYPE_MN88472) //Unit : Khz
  #if (XTAL_FREQ == DMD_XTAL20480KHZ_IF5MHZ)
  #define DMD_ADCK_FREQ 20480
  #elif (XTAL_FREQ == DMD_XTAL20500KHZ_IF5MHZ)
  #define DMD_ADCK_FREQ 20500
  #elif (XTAL_FREQ == DMD_XTAL24000KHZ_IF5MHZ) 
  #define DMD_ADCK_FREQ 20500
  #else
  #endif
#elif (DEMOD_TYPE == DMD_TYPE_MN88473) //Unit : Khz
  #if (XTAL_FREQ == DMD_XTAL20480KHZ_IF5MHZ)
  #define DMD_ADCK_FREQ 24746
  #elif (XTAL_FREQ == DMD_XTAL25000KHZ_IF5MHZ)
  #define DMD_ADCK_FREQ 25000
  #elif (XTAL_FREQ == DMD_XTAL24000KHZ_IF5MHZ) 
  #define DMD_ADCK_FREQ 25000
  #else
  #endif
#else
#endif

/* **************************************************** */
/* Customization functions *//* < User define 4 > */
/* **************************************************** */
//DVB-T2
#if (DEMOD_TYPE == DMD_TYPE_MN88473)
#define DMD_88473_AUTO_SET_T2LITE_IF_T2BASE_UNLOCKED
#endif

//DVB-T & DVB-T2
//#define DMD_SKIP_SCAN_IF_RF_POWER_LOWER_THAN_SENSITIVITY_LINE //troy, 20140327, to decrease auto-scan time by check RF power




/* **************************************************** */
/* OPEN ALL debug to do debugging : OPEN IT to know WHY you cannot lock signal*/
/* **************************************************** */
#define COMMON_DEBUG_ON  
#ifdef COMMON_DEBUG_ON
#define DMD_DEBUG
#endif
//#define DETAIL_DIG_DEBUG_ON

/* **************************************************** */
/* for Debug */
/* **************************************************** */
#ifdef DMD_DEBUG // this is for PC control tool 
#define DMD_DBG_TRACE(str)	{HI_INFO_TUNER("----> %s\n",str);}
#define DMD_CHECK_ERROR( var, msg )	\
	if( var == DMD_E_ERROR ){ \
	 HI_ERR_TUNER("---->%s\n",msg) ;}
#else
#define DMD_DBG_TRACE(str)   {HI_INFO_TUNER("----> %s\n",str)}
#define DMD_CHECK_ERROR( var ,msg );
#endif

/*for PANASONIC SZDC debug only*/
//#define PC_CONTROL_FE
#ifdef PC_CONTROL_FE 
#define TUNER_I2C_MODE TUNER_CONTROL_BY_DEMOD

//only choose one method. 
//#define MXL_I2C_CONTROL //using Maxlinear USB-I2C chip to do debugging < Currently, only support /TUNER_CONTROL_BY_SOC/ > 
#define PANA_I2C_CONTROL //using Panasonic USB-I2C chip to do debugging, default setting. 
#endif

/* **************************************************** */
/* DMD Defines */
/* **************************************************** */
#define DMD_NOT_SUPPORT		0
#define DMD_SYSTEM_MAX		15
#define DMD_TCB_DATA_MAX	256
#define DMD_REGISTER_MAX	2048
#define DMD_INFORMATION_MAX	512

/* **************************************************** */
/* Structure & types */
/* **************************************************** */

/*! DMD Text List */
typedef struct 
{
	DMD_u32_t		size;
	const DMD_text_t		list[];
} DMD_textlist_t;

/*! ARRAY FLAG */
typedef enum {
	DMD_E_END_OF_ARRAY = 0,
	DMD_E_ARRAY
} DMD_ARRAY_FLAG_t;

/*! I2C Register Structure */
typedef struct {
	DMD_u8_t	slvadr; 
	DMD_u8_t	adr;
	DMD_u8_t	data;
	DMD_ARRAY_FLAG_t	flag;
} DMD_I2C_Register_t;

/* **************************************************** */


/* **************************************************** */
/* I2C useful function */
/* **************************************************** */

extern DMD_ERROR_t	MN8847X_DMD_I2C_MaskWrite( DMD_u8_t	slvadr , DMD_u8_t adr , DMD_u8_t mask , DMD_u8_t  data );

/* **************************************************** */
/*  Functions for Local (not exported) */
/* **************************************************** */
extern DMD_ERROR_t MN8847X_DMD_trans_reg(  DMD_I2C_Register_t* t);
extern DMD_u32_t MN8847X_log10_easy( DMD_u32_t cnr );

extern DMD_text_t	MN8847X_DMD_value_decimal( DMD_u32_t val );
extern DMD_text_t	MN8847X_DMD_value_hex( DMD_u32_t val );
extern DMD_text_t	MN8847X_DMD_value_textlist( DMD_textlist_t* list ,DMD_u32_t val );
extern DMD_u32_t	MN8847X_DMD_get_freq_kHz( DMD_PARAMETER_t* param );

extern DMD_u32_t	MN8847X_DMD_calc_SQI(  DMD_PARAMETER_t* param);

extern DMD_ERROR_t MN8847X_DMD_send_registers( DMD_u8_t	*slaveset , DMD_u8_t* regset );
extern DMD_ERROR_t MN8847X_DMD_read_registers( void );//Troy.wang added, for dumpping register. 

/* **************************************************** */
/*  Text List */
/* **************************************************** */
extern DMD_text_t MN8847X_DMD_info_value_common( DMD_u32_t id , DMD_u32_t val );
extern DMD_text_t MN8847X_DMD_info_value_dvbt( DMD_u32_t id , DMD_u32_t val );
extern DMD_text_t MN8847X_DMD_info_value_dvbt2( DMD_u32_t id , DMD_u32_t val );
extern DMD_text_t MN8847X_DMD_info_value_isdbt( DMD_u32_t id , DMD_u32_t val );
extern DMD_text_t MN8847X_DMD_info_value_isdbs( DMD_u32_t id , DMD_u32_t val );
extern DMD_text_t MN8847X_DMD_info_value_vq( DMD_u32_t id , DMD_u32_t val );
extern DMD_text_t MN8847X_DMD_info_value_dvbc( DMD_u32_t id , DMD_u32_t val );

extern DMD_text_t MN8847X_DMD_INFO_TITLE_COMMON[];
extern DMD_text_t MN8847X_DMD_INFO_TITLE_DVBT[];
extern DMD_text_t MN8847X_DMD_INFO_TITLE_DVBT2[];
extern DMD_text_t MN8847X_DMD_INFO_TITLE_DVBC[];
extern DMD_text_t MN8847X_DMD_INFO_TITLE_ISDBT[];
extern DMD_text_t MN8847X_DMD_INFO_TITLE_ISDBS[];
extern DMD_text_t MN8847X_DMD_INFO_TITLE_ATSC[];
extern DMD_text_t MN8847X_DMD_INFO_TITLE_QAM[];

extern	DMD_textlist_t	MN8847X_DMD_TEXTLIST_SYSTEM	;
extern	DMD_textlist_t	MN8847X_DMD_TEXTLIST_BW	;
extern	DMD_textlist_t	MN8847X_DMD_TEXTLIST_YESNO	;

extern	DMD_textlist_t	MN8847X_DMD_TEXTLIST_LOCK	;
extern	DMD_textlist_t	MN8847X_DMD_TEXTLIST_DVBT_STATUS	;
extern	DMD_textlist_t	MN8847X_DMD_TEXTLIST_ERRORFREE	;
extern	DMD_textlist_t	MN8847X_DMD_TEXTLIST_HIERARCHY_SEL	;
extern	DMD_textlist_t	MN8847X_DMD_TEXTLIST_TPS_ALL	;
extern	DMD_textlist_t	MN8847X_DMD_TEXTLIST_DVBT_MODE	;
extern	DMD_textlist_t	MN8847X_DMD_TEXTLIST_DVBT_GI	;
extern	DMD_textlist_t	MN8847X_DMD_TEXTLIST_DVBT_CONST	;
extern	DMD_textlist_t	MN8847X_DMD_TEXTLIST_DVBT_HIERARCHY	;
extern	DMD_textlist_t	MN8847X_DMD_TEXTLIST_DVBT_CR	;

extern	DMD_textlist_t	MN8847X_DMD_TEXTLIST_DVBC_STATUS;

extern	DMD_textlist_t	MN8847X_DMD_TEXTLIST_DVBT2_STATUS;
extern	DMD_textlist_t	MN8847X_DMD_TEXTLIST_DVBT2_MODE	;
extern	DMD_textlist_t	MN8847X_DMD_TEXTLIST_DVBT2_GI	;
extern	DMD_textlist_t	MN8847X_DMD_TEXTLIST_DVBT2_TYPE	;
extern	DMD_textlist_t	MN8847X_DMD_TEXTLIST_DVBT2_PAPR	;
extern	DMD_textlist_t	MN8847X_DMD_TEXTLIST_DVBT2_L1_MOD	;
extern	DMD_textlist_t	MN8847X_DMD_TEXTLIST_DVBT2_COD	;
extern	DMD_textlist_t	MN8847X_DMD_TEXTLIST_DVBT2_FEC_TYPE	;
extern	DMD_textlist_t	MN8847X_DMD_TEXTLIST_DVBT2_PILOT_PATTERN	;
extern	DMD_textlist_t	MN8847X_DMD_TEXTLIST_DVBT2_PLP_MODE	;
extern	DMD_textlist_t	MN8847X_DMD_TEXTLIST_DVBT2_PLP_TYPE	;
extern	DMD_textlist_t	MN8847X_DMD_TEXTLIST_DVBT2_PAYLOAD_TYPE	;
extern	DMD_textlist_t	MN8847X_DMD_TEXTLIST_DVBT2_PLP_MOD	;

extern	DMD_textlist_t	MN8847X_DMD_TEXTLIST_ISDBT_MODE	;
extern	DMD_textlist_t	MN8847X_DMD_TEXTLIST_ISDBT_GI	;
extern	DMD_textlist_t	MN8847X_DMD_TEXTLIST_ISDBT_SYSTEM	;
extern	DMD_textlist_t	MN8847X_DMD_TEXTLIST_ISDBT_MAP	;
extern	DMD_textlist_t	MN8847X_DMD_TEXTLIST_ISDBT_CR	;
extern	DMD_textlist_t	MN8847X_DMD_TEXTLIST_ISDBT_INT	;

extern	DMD_textlist_t	MN8847X_DMD_TEXTLIST_ISDBS_MOD	;

#ifdef __cplusplus
}
#endif

#endif
