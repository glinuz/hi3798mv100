//#include "..\stdafx.h"
#ifndef  _R836_H_ 
#define _R836_H_

#define VERSION   "R836_v2.9H_InitTime_S11"    
#define VER_NUM   9

#include <linux/delay.h>
#include "hi_type.h"
//----------------------------------------------------------//
//                   Type Define                                    //
//----------------------------------------------------------//
#define UINT8  HI_U8
#define UINT16 HI_U16
#define UINT32 HI_U32
#define INT32 HI_S32
#define SINT8  HI_S8
#define SINT16 HI_S16
#define SINT32 HI_S32

#define TRUE 1
#define FALSE 0
//----------------------------------------------------------//
//                   Define                                            //
//----------------------------------------------------------//

#define R840_INIT_LT_ON                       FALSE      //if(desire LT enable after init), set to TRUE 
#define DTV_EXT_LNA                            FALSE      //if(use external LNA), set to TRUE
#if(DTV_EXT_LNA==TRUE)
    #define DTV_VHF_ACI2_ENHANCE         FALSE //do not change this setting
	#define DTV_LTE_ONE_EQ						FALSE  //do not change this setting
#else
    #define DTV_VHF_ACI2_ENHANCE         FALSE 
	#define DTV_LTE_ONE_EQ						TRUE
#endif

//--------------------------------Xtal setting --------------------------------------- //
// Support 16MHz Xtal, or 16/24/27MHz clock from other device.               //
// 1. if single Xtal application, only 16Mhz Xtal can be used!!                      //
// 2. share Xtal application and clock from other device, can use 16/24/27M  //
//--------------------------------------------------------------------------------------//
#define R840_Xtal	                             16000       //change the value when change clock input frequency
#define R840_SHARE_XTAL_OUT     FALSE     //if(share Xtal application), set to TRUE
#define R840_INIT_SMALL_XTAL_CAP   FALSE      //if(share Xtal & tuner as slave), set to TRUE

//Regular Define
#define R840_REG_NUM         32
#define R840_TF_HIGH_NUM  7  
#define R840_TF_MID_NUM    8
#define R840_TF_LOW_NUM   8
#define R840_TF_LOWEST_NUM   8
#define R840_RING_POWER_FREQ  115000
#define R840_IMR_IF              5300         
#define R840_IMR_TRIAL       9

//----------------------------------------------------------//
//                   Internal Structure                            //
//----------------------------------------------------------//
typedef struct _Sys_Info_Type
{
	UINT8          BW;
	UINT8		    V17M; 
	UINT8		    HPF_COR;
	UINT8          FILT_EXT_ENA;
	UINT8          FILT_EXT_WIDEST;
	UINT8          FILT_EXT_POINT;
	UINT8          LNA_DET_MODE; 
	//UINT8          RF_DISCHARGE;
	UINT8          HPF_NOTCH;
	UINT8          NA_DISCHARGE;
	UINT8          AGC_CLK;
	UINT8		    FILT_COMP;
	UINT8		    FILT_CUR;  
	UINT8		    FILT_3DB; 
	UINT8		    SWBUF_CUR;  
	UINT8          TF_CUR;              
	UINT8		    RF_BUF_CUR;  
	UINT8		    LNA_CUR;  
	UINT8		    SD_IM;  
	UINT8          SWCAP_CLK;
	UINT8		    NA_PWR_DET;  
	UINT16		IF_KHz;
	UINT16	    FILT_CAL_IF;
}Sys_Info_Type;

typedef struct _Freq_Info_Type
{
	UINT8		RF_POLY;
	UINT8		LNA_BAND;
	UINT8		LPF_CAP;
	UINT8		LPF_NOTCH;
    UINT8      XTAL_POW0;
	UINT8      CP_CUR;
	UINT8		IMR_MEM;
	UINT8      Q_CTRL;   
}Freq_Info_Type;

typedef struct _SysFreq_Info_Type
{
	UINT8		LNA_TOP;
	UINT8		LNA_VTH_L;
	UINT8		MIXER_TOP;
	UINT8		MIXER_VTH_L;
	UINT8      RF_TOP;
	UINT8      NRB_TOP;
	UINT8      NRB_BW;
	UINT8      BYP_LPF;
	UINT8      LNA_DISCHARGE;
	UINT8      NRB_TOP_ADDER;
	UINT8      RF_DISCHARGE;
	UINT8      TEMP;         //no use
}SysFreq_Info_Type;

typedef struct _R840_Cal_Info_Type
{
	UINT8		FILTER_6DB;
	UINT8		MIXER_AMP_GAIN;
	UINT8		MIXER_BUFFER_GAIN;
	UINT8		LNA_GAIN;
	UINT8		LNA_POWER;
	UINT8		RFBUF_OUT;
}R840_Cal_Info_Type;

typedef struct _R840_SectType
{
	UINT8   Phase_Y;
	UINT8   Gain_X;
	UINT8   Iqcap;
	UINT8   Value;
}R840_SectType;

typedef struct _R840_TF_Result
{
	UINT8   TF_Set;
	UINT8   TF_Value;
}R840_TF_Result;

typedef enum _R840_TF_Band_Type
{
    TF_HIGH = 0,
	TF_MID,
	TF_LOW
}R840_TF_Band_Type;

typedef enum _R840_TF_Type
{
	R840_TF_NARROW = 0,             //270n/68n   (ISDB-T, DVB-T/T2)
    R840_TF_BEAD,                        //Bead/68n   (DTMB)
	R840_TF_BEAD_TERR,              //Bead/68n   (ISDB-T, DVB-T/T2)
	R840_TF_NARROW_ATV_LIN,  //270n/68n   (ATV)
	R840_TF_BEAD_LIN,                 //Bead/68n   (ATV)
	R840_TF_NARROW_ATSC,        //270n/68n   (ATSC, DVB-C, J83B)
	R840_TF_BEAD_LIN_ATSC,      //Bead/68n   (ATSC, DVB-C, J83B)
	R840_TF_SIZE
}R840_TF_Type;

typedef enum _R840_UL_TF_Type
{
	R840_UL_USING_BEAD = 0,            
    R840_UL_USING_270NH,                      
}R840_UL_TF_Type;

typedef enum _R840_Cal_Type
{
	R840_IMR_CAL = 0,
	R840_IMR_LNA_CAL,
	R840_TF_CAL,
	R840_TF_LNA_CAL,
	R840_LPF_CAL,
	R840_LPF_LNA_CAL
}R840_Cal_Type;

typedef enum _R840_BW_Type
{
	BW_6M = 0,
	BW_7M,
	BW_8M,
	BW_1_7M,
	BW_10M,
	BW_200K
}R840_BW_Type;

enum XTAL_PWR_VALUE
{
	XTAL_NOR_LOWEST = 0,
    XTAL_NOR_LOW,
    XTAL_NOR_HIGH,
    XTAL_NOR_HIGHEST,
//    XTAL_GM3V_HIGHEST,
	XTAL_GM3V_STRONG,
	XTAL_CHECK_SIZE
};

typedef enum _Rafael_Chip_Type  //Don't modify chip list
{
	R840=0,
	R836,
	R840_R,
	R836_R,
}Rafael_Chip_Type;

//----------------------------------------------------------//
//                   R840 Public Parameter                     //
//----------------------------------------------------------//
typedef enum _R840_ErrCode
{
	RT_Success = true,
	RT_Fail    = false
}R840_ErrCode;

typedef enum _R840_Standard_Type  //Don't remove standand list!!
{
	//ATV normal IF (prefer to use)
	R840_MN_5100 = 0,                     //for NTSC_MN, PAL_M (IF=5.1M)
	R840_MN_5800,                           //for NTSC_MN, PLA_M (IF=5.8M)
	R840_PAL_I,                                //for PAL-I
	R840_PAL_DK,                            //for PAL DK 
	R840_PAL_B_7M,                        //for PAL-B 7M
	R840_PAL_BGH_8M,                   //for PAL G/H 8M
	R840_SECAM_L,                         //for SECAM L
	R840_SECAM_L1,                        //for SECAM L'
	R840_SECAM_L1_INV,  
	//ATV CIF=5M (not prefer to use)
	R840_MN_CIF_5M,                      //for NTSC_MN, PLA_M (CIF=5.0M)
	R840_PAL_I_CIF_5M,                  //for PAL-I (CIF=5.0M)
	R840_PAL_DK_CIF_5M,              //for PAL DK (CIF=5M)
	R840_PAL_B_7M_CIF_5M,          //for PAL-B 7M (CIF=5M)
	R840_PAL_BGH_8M_CIF_5M,     //for PAL G/H 8M (CIF=5M)
	R840_SECAM_L_CIF_5M,           //for SECAM L (CIF=5M)
	R840_SECAM_L1_CIF_5M,          //for SECAM L' (CIF=5M)
	R840_SECAM_L1_INV_CIF_5M,                        //(CIF=5M)
	R840_ATV_SIZE,
	//DTV
	R840_DVB_T_6M = R840_ATV_SIZE,  
	R840_DVB_T_7M,
	R840_DVB_T_8M, 
    R840_DVB_T2_6M,       //IF=4.57M
	R840_DVB_T2_7M,       //IF=4.57M
	R840_DVB_T2_8M,       //IF=4.57M
	R840_DVB_T2_1_7M,
	R840_DVB_T2_10M,
	R840_DVB_C_8M,
	R840_DVB_C_6M, 
	R840_J83B,
	R840_ISDB_T_4063,           //IF=4.063M
	R840_ISDB_T_4570,           //IF=4.57M
	R840_DTMB_8M_4570,      //IF=4.57M
	R840_DTMB_6M_4500,      //IF=4.5M, BW=6M
	R840_ATSC,  
	//DTV IF=5M
	R840_DVB_T_6M_IF_5M,
	R840_DVB_T_7M_IF_5M,
	R840_DVB_T_8M_IF_5M,
	R840_DVB_T2_6M_IF_5M,
	R840_DVB_T2_7M_IF_5M,
	R840_DVB_T2_8M_IF_5M,
	R840_DVB_T2_1_7M_IF_5M,
	R840_DVB_C_8M_IF_5M,
	R840_DVB_C_6M_IF_5M, 
	R840_J83B_IF_5M,
	R840_ISDB_T_IF_5M,            
	R840_DTMB_8M_IF_5M,     
	R840_DTMB_6M_IF_5M,     
	R840_ATSC_IF_5M,  
	R840_FM,
	R840_STD_SIZE,
}R840_Standard_Type;


typedef enum _R840_LoopThrough_Type
{
	LT_ON = TRUE,
	LT_OFF= FALSE
}R840_LoopThrough_Type;

typedef enum _R840_GPIO_Type
{
	HI_SIG = TRUE,
	LO_SIG = FALSE
}R840_GPIO_Type;

typedef enum _R840_ClkOutMode_Type
{
	CLK_OUT_OFF = 0,
	CLK_OUT_ON
}R840_ClkOutMode_Type;

typedef enum _R840_RF_Gain_TYPE
{
	RF_AUTO = 0,
	RF_MANUAL
}R840_RF_Gain_TYPE;

typedef enum _R840_Xtal_Div_TYPE
{
	XTAL_DIV1 = 0,
	XTAL_DIV2
}R840_Xtal_Div_TYPE;

typedef enum _R840_IfAgc_Type
{
	IF_AGC1 = 0,
	IF_AGC2
}R840_IfAgc_Type;

typedef struct _R840_Set_Info
{
	UINT32                              RF_KHz;
	R840_Standard_Type           R840_Standard;
	R840_LoopThrough_Type    R840_LT;
	R840_ClkOutMode_Type     R840_ClkOutMode;
	R840_IfAgc_Type               R840_IfAgc_Select; 
}R840_Set_Info;

typedef struct _R840_RF_Gain_Info
{
	UINT16   RF_gain_comb;
	UINT8   RF_gain1;
	UINT8   RF_gain2;
	UINT8   RF_gain3;
}R840_RF_Gain_Info;

typedef enum _R840_Xtal_Cap_TYPE
{
	XTAL_CAP_LARGE = 0,
	XTAL_CAP_SMALL
}R840_Xtal_Cap_TYPE;

typedef enum _R840_Vga_Mode_TYPE
{
	VGA_AUTO = 0,
	VGA_MANUAL
}R840_Vga_Mode_TYPE;

//----------------------------------------------------------//
//                   R840 Public Function                       //
//----------------------------------------------------------//
#define R840_Delay_MS	msleep
#define R840_Init_Delay_MS	mdelay

R840_ErrCode R840_Init(HI_U32 u32TunerPort);
R840_ErrCode R840_SetPllData(HI_U32 u32TunerPort,R840_Set_Info R840_INFO);
R840_ErrCode R840_SetPllData_AutoSearch(HI_U32 u32TunerPort,R840_Set_Info R840_INFO);
R840_ErrCode R840_Standby(R840_LoopThrough_Type R840_LoopSwitch);
R840_ErrCode R840_WakeUp(void);
R840_ErrCode R840_GPIO(R840_GPIO_Type R840_GPIO_Conrl);
R840_ErrCode R840_GetRfGain(R840_RF_Gain_Info *pR840_rf_gain);
R840_ErrCode R840_RfGainMode(R840_RF_Gain_TYPE R840_RfGainType);
R840_ErrCode R840_SetXtalIntCap(R840_Xtal_Cap_TYPE R840_XtalCapType);
R840_ErrCode R840_VgaCtrl(R840_Vga_Mode_TYPE R840_VgaModeType, UINT8 u1ManualCode);
R840_ErrCode R840_GetRfRssi(UINT32 RF_Freq_Khz, R840_Standard_Type RT_Standard, INT32 *RfLevelDbm, UINT8 *fgRfMaxGain);
UINT8 R840_PLL_Lock(void);
R840_ErrCode R840_GetIfRssi(INT32 *VgaGain);
R840_ErrCode R840_GetTotalRssi(UINT32 RF_Freq_Khz, R840_Standard_Type RT_Standard, INT32 *RssiDbm);
R840_ErrCode R840_SetIfFreq(UINT16 IF_KHz, UINT32 RF_KHz, R840_Standard_Type u1SystemStd);
R840_ErrCode R840_SetLpfBw(UINT8 LPF_BW);
R840_ErrCode R840_SetLpfOffset(UINT8 LPF_Offset);
R840_ErrCode R840_SetHpfOffset(UINT8 HPF_Offset);
R840_ErrCode R840_SetIfLpf(UINT32 LPF_Cor_Freq);
R840_ErrCode R840_SetLnaTop(UINT8 LNA_Top);
R840_ErrCode R840_set_tuner(HI_U32 u32TunerPort,HI_U32 u32PuRF,HI_U32 u32BandWidth,HI_U32 u32DVBMode);
R840_ErrCode R840_PulseGain(void);

extern UINT8   R840_Initial_done_flag;  
extern UINT8   R840_IMR_done_flag;    
#endif
