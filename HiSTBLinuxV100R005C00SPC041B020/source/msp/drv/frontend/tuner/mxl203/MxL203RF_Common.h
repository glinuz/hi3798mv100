/*******************************************************************************
 *
 * FILE NAME          : MxL_Common.h
 * 
 * AUTHOR             : Howard Chan
 * DATE CREATED       : January 10, 2010
 *
 * DESCRIPTION        : MxL203RF V6 Type enumeration & struct declaration
 *
 *******************************************************************************
 *                Copyright (c) 2010, MaxLinear, Inc.
 ******************************************************************************/
 
#ifndef __MxL203RF_COMMON_H
#define __MxL203RF_COMMON_H

#include "MxL_User_Define.h"


/******************************/
/*	MxL203RF Err message  	  */
/******************************/
typedef enum{
	MxL_OK					=   0x0,
	MxL_ERR_INIT			=   0x1,
	MxL_ERR_RFTUNE			=   0x2,
	MxL_ERR_SET_REG			=   0x3,
	MxL_ERR_GET_REG			=   0x4,
	MxL_ERR_MODE			=   0x10,
	MxL_ERR_IF_FREQ			=   0x11,
	MxL_ERR_XTAL_FREQ		=   0x12,
	MxL_ERR_BANDWIDTH		=   0x13,
	MxL_GET_ID_FAIL			=   0x14,
	MxL_ERR_OTHERS			=   0x0A
}MxL_ERR_MSG;

/******************************/
/*	MxL203RF Chip verstion     */
/******************************/
typedef enum{
	MxL_UNKNOWN_ID		= 0x00,
	MxL_203RF_ES5		= 0x06
}MxL203RF_ChipVersion;


/******************************************************************************
    CONSTANTS
******************************************************************************/

#ifndef MHz
	#define MHz 1000000
#endif

#define MAX_ARRAY_SIZE 100


/* Enumeration of Mode */
typedef enum 
{	
	MxL_MODE_CAB_STD = 0x11  /*Cable "Standard" setting */
} MxL203RF_Mode ;

/* Enumeration of Acceptable IF Frequencies */
typedef enum
{
	MxL_IF_4_57_MHZ	  =	4570000,
	MxL_IF_5_MHZ	  =	5000000,
	MxL_IF_6_MHZ	  =	6000000,
	MxL_IF_7_2_MHZ    = 7200000
} MxL203RF_IF_Freq ;

/* Enumeration of Acceptable Crystal Frequencies */
typedef enum
{
	MxL_XTAL_16_MHZ		= 16000000,
	MxL_XTAL_20_MHZ		= 20000000,
	MxL_XTAL_20_25_MHZ	= 20250000,
	MxL_XTAL_20_48_MHZ	= 20480000,
	MxL_XTAL_24_MHZ		= 24000000,
	MxL_XTAL_25_MHZ		= 25000000,
	MxL_XTAL_25_14_MHZ	= 25140000,
	MxL_XTAL_28_8_MHZ	= 28800000,
	MxL_XTAL_32_MHZ		= 32000000,
	MxL_XTAL_40_MHZ		= 40000000,
	MxL_XTAL_44_MHZ		= 44000000,
	MxL_XTAL_48_MHZ		= 48000000,
	MxL_XTAL_49_3811_MHZ = 49381100	
} MxL203RF_Xtal_Freq ;

/* Enumeration of Acceptable IF Bandwidths */
typedef enum
{
	MxL_BW_6MHz = 6,
	MxL_BW_8MHz = 8
} MxL203RF_BW_MHz;

/* Enumeration of Inverted/Normal IF Spectrum */
typedef enum
{
	MxL_NORMAL_IF = 0 ,
	MxL_INVERT_IF

} MxL203RF_IF_Spectrum ;

/* Enumeration of Clock out Enable/Disable */
typedef enum
{
	MxL_CLKOUT_DISABLE = 0 ,
	MxL_CLKOUT_ENABLE

} MxL203RF_ClkOut;

/* Enumeration of Clock out Amplitude */
typedef enum
{
	MxL_CLKOUT_AMP_0 = 0 , 	/* min Clock out Amplitude */
	MxL_CLKOUT_AMP_1,
	MxL_CLKOUT_AMP_2,
	MxL_CLKOUT_AMP_3,
	MxL_CLKOUT_AMP_4,
	MxL_CLKOUT_AMP_5,
	MxL_CLKOUT_AMP_6,
	MxL_CLKOUT_AMP_7,
	MxL_CLKOUT_AMP_8,
	MxL_CLKOUT_AMP_9,
	MxL_CLKOUT_AMP_10,
	MxL_CLKOUT_AMP_11,
	MxL_CLKOUT_AMP_12,
	MxL_CLKOUT_AMP_13,
	MxL_CLKOUT_AMP_14,
	MxL_CLKOUT_AMP_15  /* max Clock out Amplitude */

} MxL203RF_ClkOut_Amp;

/* Enumeration of I2C Addresses */
typedef enum
{
	MxL_I2C_ADDR_96 = 96 ,
	MxL_I2C_ADDR_97 = 97 ,
	MxL_I2C_ADDR_98 = 98 ,
	MxL_I2C_ADDR_99 = 99 	
} MxL203RF_I2CAddr ;

/* Enumeration of Acceptable Crystal Capacitor values */
typedef enum
{
	MxL_XTAL_CAP_0_PF = 0,
	MxL_XTAL_CAP_1_PF = 1,
	MxL_XTAL_CAP_2_PF = 2,
	MxL_XTAL_CAP_3_PF = 3,
	MxL_XTAL_CAP_4_PF = 4,
	MxL_XTAL_CAP_5_PF = 5,
	MxL_XTAL_CAP_6_PF = 6,
	MxL_XTAL_CAP_7_PF = 7,
	MxL_XTAL_CAP_8_PF = 8,
	MxL_XTAL_CAP_9_PF = 9,
	MxL_XTAL_CAP_10_PF = 10,
	MxL_XTAL_CAP_11_PF = 11,
	MxL_XTAL_CAP_12_PF = 12,
	MxL_XTAL_CAP_13_PF = 13,
	MxL_XTAL_CAP_14_PF = 14,
	MxL_XTAL_CAP_15_PF = 15,
	MxL_XTAL_CAP_16_PF = 16,
	MxL_XTAL_CAP_17_PF = 17,
	MxL_XTAL_CAP_18_PF = 18,
	MxL_XTAL_CAP_19_PF = 19,
	MxL_XTAL_CAP_20_PF = 20,
	MxL_XTAL_CAP_21_PF = 21,
	MxL_XTAL_CAP_22_PF = 22,
	MxL_XTAL_CAP_23_PF = 23,
	MxL_XTAL_CAP_24_PF = 24,
	MxL_XTAL_CAP_25_PF = 25
}	MxL203RF_Xtal_Cap;


/* SHFLUT */
typedef struct
{
	UINT32	Ch_Freq_Hz;
	UINT8	Reg1_Val;
	UINT8	Reg2_Val;
} ShiftLUT;

extern ShiftLUT SHIFT_LUT6[];
extern ShiftLUT SHIFT_LUT8[];


/* =====================
   MxL203RF Tuner Struct
   ===================== */ 
typedef struct _MxL203RF_TunerConfigS
{
	MxL203RF_I2CAddr	I2C_Addr;
	MxL203RF_Mode		Mode;	
	MxL203RF_Xtal_Freq	Xtal_Freq;
	MxL203RF_IF_Freq	IF_Freq;
	MxL203RF_IF_Spectrum IF_Spectrum;
	MxL203RF_ClkOut		ClkOut_Setting;
    MxL203RF_ClkOut_Amp	ClkOut_Amp;
	MxL203RF_BW_MHz		BW_MHz;
	UINT32				RF_Freq_Hz;
	MxL203RF_Xtal_Cap	Xtal_Cap;
} MxL203RF_TunerConfigS ;

#endif /* __MxL203RF_COMMON_H__*/

