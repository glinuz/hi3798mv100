//#include "..\stdafx.h"
#ifndef  _R828_H_ 
#define _R828_H_

#include <linux/delay.h>
#include "hi_type.h"

//***************************************************************
//*                       INCLUDES.H
//***************************************************************
#define VERSION   "R820C_v2.14B_FilterCal"

#define TRUE 1
#define FALSE 0
#define VER_NUM  14
#define USE_16M_XTAL     TRUE
#define R828_Xtal	  16000

//#define USE_DIPLEXER      FALSE
#define TUNER_CLK_OUT  FALSE

//----------------------------------------------------------//
//                   Type Define                            //
//----------------------------------------------------------//

//#define UINT8  unsigned char
//#define UINT16 unsigned short
//#define UINT32 unsigned long
#define UINT8   HI_U8
#define UINT16 HI_U16
#define UINT32 HI_U32

typedef enum _R828_ErrCode
{
	RT_Success = true,
	RT_Fail    = false
}R828_ErrCode;

typedef enum _Rafael_Chip_Type  //Don't modify chip list
{
	R828 = 0,
	R828D,
	R828S,
	R820T,
	R820C,
	R620D,
	R620S
}Rafael_Chip_Type;
//----------------------------------------------------------//
//                   R828 Parameter                        //
//----------------------------------------------------------//

extern UINT8 R828_ExtraArry[5];
extern UINT8  USE_DIPLEXER;
extern UINT8  R828_ADDRESS;

#define DIP_FREQ  	  320000
#define IMR_TRIAL    9
#define VCO_pwr_ref   0x01

extern UINT32 R828_IF_khz;
extern UINT32 R828_CAL_LO_khz;
extern UINT8  R828_IMR_point_num;
extern UINT8  R828_IMR_done_flag;
extern UINT8  Rafael_Chip;

typedef enum _R828_Standard_Type  //Don't remove standand list!!
{

	NTSC_MN = 0,  //IF=5.1MHz
	NTSC_KOREA,  //IF=5.8MHz
	PAL_I,
	PAL_DK,
	PAL_B_7M,       //no use
	PAL_BGH_8M,     //for PAL B/G, PAL G/H
	SECAM_L,
	SECAM_L1_INV,   //for SECAM L'
	SECAM_L1,       //no use
	ATV_SIZE,
	DVB_T_6M = ATV_SIZE,
	DVB_T_7M,
	DVB_T_7M_2,   //default for DVB-T 7M, IF=4.57MHz
	DVB_T_8M,
    DVB_T2_6M,
	DVB_T2_7M,
	DVB_T2_7M_2,  //default for DVB-T2 7M, IF=4.57MHz
	DVB_T2_8M,
	DVB_T2_1_7M,
	DVB_T2_10M,
	DVB_C_8M,
	DVB_C_6M_4063,
	DVB_C_6M_5070, //default for DVB-C 6M, IF=5.07MHz
	J83B,
	ISDB_T,
    ISDB_T_7M,  //IF=4.57M
    ISDB_T_8M,  //IF=4.57M
	DTMB,
	DTMB_5570,
	ATSC_4063,
	ATSC_5070,  //default for ATSC, IF=5.07MHz
	ATSC_KOREA, 
	FM,
	STD_SIZE
}R828_Standard_Type;

extern UINT8  R828_Fil_Cal_flag[STD_SIZE];

typedef enum _R828_SetFreq_Type
{
	FAST_MODE = TRUE,
	NORMAL_MODE = FALSE
}R828_SetFreq_Type;

typedef enum _R828_LoopThrough_Type
{
	LOOP_THROUGH = TRUE,
	SIGLE_IN     = FALSE
}R828_LoopThrough_Type;

typedef enum _R828_LoopThroughAtt_Type
{
	LT_ATT_ON = TRUE,
	LT_ATT_OFF = FALSE
}R828_LoopThroughAtt_Type;

typedef enum _R828_InputMode_Type
{
	AIR_IN = 0,
	CABLE_IN_1,
	CABLE_IN_2
}R828_InputMode_Type;

typedef enum _R828_IfAgc_Type
{
	IF_AGC1 = 0,
	IF_AGC2
}R828_IfAgc_Type;

typedef enum _R828_GPIO_Type
{
	HI_SIG = TRUE,
	LO_SIG = FALSE
}R828_GPIO_Type;

typedef struct _R828_Set_Info
{
	UINT32        RF_KHz;
	R828_Standard_Type R828_Standard;
	R828_LoopThrough_Type RT_Input;
	R828_InputMode_Type   RT_InputMode;
	R828_IfAgc_Type R828_IfAgc_Select; 
}R828_Set_Info;

typedef struct _R828_RF_Gain_Info
{
	UINT8   RF_gain1;
	UINT8   RF_gain2;
	UINT8   RF_gain_comb;
}R828_RF_Gain_Info;

typedef enum _R828_RF_Gain_TYPE
{
	RF_AUTO = 0,
	RF_MANUAL
}R828_RF_Gain_TYPE;

//----------------------------------------------------------//
//                   R828 Function                         //
//----------------------------------------------------------//

#define R828_Delay_MS	msleep

R828_ErrCode R828_Init(void);
R828_ErrCode R828_Standby(R828_LoopThrough_Type R828_LoopSwitch);
R828_ErrCode R828_GPIO(R828_GPIO_Type R828_GPIO_Conrl);
R828_ErrCode R828_SetStandard(R828_Standard_Type RT_Standard);
R828_ErrCode R828_SetFrequency(R828_Set_Info R828_INFO, R828_SetFreq_Type R828_SetFreqMode);
R828_ErrCode R828_GetRfGain(R828_RF_Gain_Info *pR828_rf_gain);
R828_ErrCode R828_RfGainMode(R828_RF_Gain_TYPE R828_RfGainType);
R828_ErrCode R828_SetLoopThrough(R828_LoopThrough_Type R828_LT_Type);
R828_ErrCode R828_SetLoopThroughAttenu(R828_LoopThroughAtt_Type R828_LT_Att_Type, UINT8 R828_LT_Att_Gain);  //LT_Att_Gain:0~7

extern UINT8 R828_IMR_XT[6];
extern UINT8 R828_IMR_YT[6];
R828_ErrCode R828_ReadIMR(void);


#endif